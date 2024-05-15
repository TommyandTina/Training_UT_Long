/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include "gether.h"
#include <fcntl.h>
#include <drvr/hwinfo.h>
#include <net/ifdrvcom.h>
#include <sys/sockio.h>

static int gether_tx_desc_init(gether_dev_t *gether);
static int gether_rx_desc_init(gether_dev_t *gether);
static void gether_config(gether_dev_t *gether);

struct sigevent		event;

static int gether_wait(gether_dev_t *gether, int reg, uint32_t mask, uint32_t value)
{
    int i;

    for (i = 0; i < 200; i++) {
        if ((in32(gether->base + reg) & mask) == value) {
            return EOK;
        }
        nanospin_ns(64);
    }
    return -ETIMEDOUT;
}

static void gether_reset (gether_dev_t *gether)
{
    uint32_t        val;
    uint32_t        timeout = 200;

    /* Start e-dmac transmitter and receiver */
    out32(gether->base +  GETHER_EDSR, GETHER_EDSR_ENT | GETHER_EDSR_ENR);

    /* Perform a software reset and wait for it to complete */
    out32(gether->base + GETHER_EDMR, GETHER_EDMR_SWRR | GETHER_EDMR_SWRT);
    /* Need to wait 64 clock cycles after reset before accessing registers */
    do{
        timeout--;
        if(timeout == 0){
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to come out of reset");
            break;
        }
        nanospin_ns(64);
        val = in32(gether->base + GETHER_EDMR);
    } while((val & GETHER_EDMR_SWRR ) || (val & GETHER_EDMR_SWRT));

    out32(gether->base + GETHER_EDSR,  0);
    out32(gether->base + GETHER_RDLAR, 0);
    out32(gether->base + GETHER_RDFAR, 0);
    out32(gether->base + GETHER_RDFXR, 0);
    out32(gether->base + GETHER_TDFFR, 0);
    out32(gether->base + GETHER_TDLAR, 0);
    out32(gether->base + GETHER_TDFAR, 0);
    out32(gether->base + GETHER_TDFXR, 0);
    out32(gether->base + GETHER_TDFFR, 0);
}

static void gether_EDMAC(gether_dev_t *gether, int status)
{
    /*
     * Enable / Disable the e-dmac receiver and transmitter.
     */
    if(status){
        out32(gether->base + GETHER_EDRRR, GETHER_EDRRR_RR);
        out32(gether->base + GETHER_EDTRR, GETHER_EDTRR_TR_DMA);
    } else {
        out32(gether->base + GETHER_EDRRR, 0);
        out32(gether->base + GETHER_EDTRR, 0);
    }
}

static void gether_shutdown (void *arg)
{
    gether_dev_t        *gether;

    gether = arg;

    /* Reset to stop any DMA */
    out32(gether->base + GETHER_EDMR, GETHER_EDMR_SWRR | GETHER_EDMR_SWRT);
}

static void gether_cleanup (gether_dev_t *gether)
{
    struct mbuf        *m;
    unsigned int    i;

    if (gether->bs_options) {
        (free)(gether->bs_options);
        gether->bs_options = NULL;
    }

    if (gether->iid) {
       InterruptDetach(gether->iid);
    }
    if (gether->tid != 0) {
        /* ISR thread blocks in InterruptWait(), a cancellation point */
        pthread_cancel(gether->tid);
        pthread_join(gether->tid, NULL);
        gether->tid = 0;
    }

    /* Checks and returns EINVAL if not in the interrupt_entry list */
    interrupt_entry_remove(&gether->rx_ient, NULL);
    interrupt_entry_remove(&gether->tx_ient, NULL);

    if (gether->base) {
        gether_reset(gether);
        munmap_device_io(gether->base, gether->cfg.io_window_size[0]);
    }

    shutdownhook_disestablish(gether->sdhook);

    /* Cleanup Tx allocations */
    if (gether->tx_pkts) {
        for (i = 0; i < NUM_TX_DESC; i++) {
            m = gether->tx_pkts[i];
            if (m != NULL) {
                m_freem(m);
            }
        }
        free(gether->tx_pkts, M_DEVBUF);
        gether->tx_pkts = NULL;
    }
    if (gether->tx_bd) {
       munmap(gether->tx_bd, sizeof(GETHER_TX_TD) * NUM_TX_DESC);
       gether->tx_bd = NULL;
    }

    /* Cleanup Rx allocations */
    if (gether->rx_pkts) {
        for (i = 0; i < NUM_RX_DESC; i++) {
            m = gether->rx_pkts[i];
            if (m != NULL) {
                m_freem(m);
            }
        }
        free(gether->rx_pkts, M_DEVBUF);
        gether->rx_pkts = NULL;
    }
    if (gether->rx_bd) {
       munmap(gether->rx_bd, sizeof(GETHER_RX_RD) * NUM_RX_DESC);
       gether->rx_bd = NULL;
    }

    cache_fini(&gether->cachectl);
}

void gether_hw_stop (gether_dev_t *gether)
{
    struct mbuf         *m;
    int                 i;

    /*
     * Stop Tx and Rx
     * According to register ECMR.RE and ECMR.TE: When disabling while a frame is being transmitted
     * received, the Tx/Rx will stay enabled until transmit/reception of frame is completed
     */

    if (gether_wait(gether, GETHER_EDTRR, GETHER_EDTRR_TR_DMA, 0) != EOK) {
       slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to clear tx");
    }

    out32(gether->base + GETHER_ECMR, in32(gether->base + GETHER_ECMR) &
          ~(GETHER_ECMR_TE | GETHER_ECMR_RE));

    if (gether_wait(gether, GETHER_ECMR, (GETHER_ECMR_TE | GETHER_ECMR_RE), 0) != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to stop tx/rx");
    }

    gether_EDMAC(gether, EDMAC_STATUS_DISABLE);

    if (gether_wait(gether, GETHER_EDRRR, GETHER_EDRRR_RR, 0) != EOK) {
        out32(gether->base + GETHER_EDMR, in32(gether->base + GETHER_EDMR) &
              (GETHER_EDMR_SWRR | GETHER_EDMR_SWRT));

        if (gether_wait(gether, GETHER_EDMR, (GETHER_EDMR_SWRR | GETHER_EDMR_SWRT), 0) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to reset E-DMAC");
        }
    }

    gether_reap_tx(gether);

    /* Clear any pending Tx buffers */
    for (i = 0; i < NUM_TX_DESC; i++) {
        if ((m = gether->tx_pkts[i])) {
            m_freem(m);
        }
        gether->tx_bd[i].status = 0;
    }

    /*
     * Make sure we start queueing descriptors
     * from where the hardware stopped
     */
    gether->tx_pidx = gether->tx_cidx;
}

static void gether_stop (struct ifnet *ifp, int disable)
{
    gether_dev_t        *gether;
    nic_config_t        *cfg;
    struct nw_work_thread    *wtp;

    gether = ifp->if_softc;
    cfg = &gether->cfg;
    wtp = WTP;

    nic_mutex_lock(&gether->drv_mutex);

    NW_SIGLOCK_P(&ifp->if_snd_ex, gether->iopkt, wtp);
    callout_stop(&gether->mii_callout);
    MDI_DisableMonitor(gether->mdi);
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);
    ifp->if_flags &= ~IFF_RUNNING;
    ifp->if_flags_tx &= ~IFF_OACTIVE;
    NW_SIGUNLOCK_P(&ifp->if_snd_ex, gether->iopkt, wtp);

    gether_hw_stop(gether);

    MDI_PowerdownPhy(gether->mdi, cfg->phy_addr);
    gether_clear_stats(gether);

    nic_mutex_unlock(&gether->drv_mutex);
}


int gether_detach (struct device *dev, int flags)
{
    gether_dev_t        *gether;
    struct ifnet    *ifp;

    gether = (gether_dev_t *)dev;
    ifp = &gether->ecom.ec_if;

    atomic_set(&gether->flags, FLAG_DYING);

    /* If a kthread has been scheduled, wait for it to complete */
    while (gether->flags & FLAG_KTHREAD) {
        nic_delay(1);
    }

    nic_mutex_lock(&gether->drv_mutex);

    callout_stop(&gether->mii_callout);
    if (!gether_is_mv_phy(gether)) {
        MDI_PowerdownPhy(gether->mdi, gether->cfg.phy_addr);
    }
    ether_ifdetach(ifp);
    if_detach(ifp);
    gether_phy_fini(gether);
    gether_cleanup(gether);

    nic_mutex_unlock(&gether->drv_mutex);

    return EOK;
}

static void gether_set_mac (gether_dev_t *gether)
{
    uint32_t mach, macl;
    uint8_t *mac;

    mac = gether->cfg.current_address;
    mach = (mac[0] << 24) | (mac[1] << 16) | (mac[2] << 8) | mac[3];
    macl = (mac[4] << 8) | mac[5];
    out32(gether->base + GETHER_MAHR, mach);
    out32(gether->base + GETHER_MALR, macl);
}

static int gether_init (struct ifnet *ifp)
{
    gether_dev_t        *gether;
    nic_config_t    *cfg;
    uint32_t        val;

    gether = ifp->if_softc;
    cfg = &gether->cfg;

    if (gether->flags & FLAG_DYING) {
        /* The interface is getting detached. */
        return EOK;
    }

    if (memcmp(cfg->current_address, LLADDR(ifp->if_sadl), ifp->if_addrlen)) {
        memcpy(cfg->current_address, LLADDR(ifp->if_sadl), ifp->if_addrlen);
        gether_set_mac(gether);
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: MAC address updated");
        }
    }

    nic_mutex_lock(&gether->drv_mutex);

    val = in32(gether->base + GETHER_ECMR);
    if (((ifp->if_flags & IFF_PROMISC) == IFF_PROMISC) !=
        ((val & GETHER_ECMR_PRM) == GETHER_ECMR_PRM)) {

        val &= ~(GETHER_ECMR_RE | GETHER_ECMR_TE);
        out32(gether->base + GETHER_ECMR, val);
        if (ifp->if_flags & IFF_PROMISC) {
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Promiscuous mode enabled");
            }
            val |= GETHER_ECMR_PRM;
        } else {
            if (cfg->verbose) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Promiscuous mode disabled");
            }
            val &= ~GETHER_ECMR_PRM;
        }
    }
    val |= (GETHER_ECMR_ZPF | GETHER_ECMR_RE | GETHER_ECMR_TE);
    out32(gether->base + GETHER_ECMR, val);

    if ((ifp->if_flags & IFF_RUNNING) == 0) {
        ifp->if_flags |= IFF_RUNNING;
        if (!gether_is_mv_phy(gether)) {
            gether_mediachange(ifp);
        } else {
            gether_MonitorPhy(gether);
        }
    }

    nic_mutex_unlock(&gether->drv_mutex);

    return EOK;
}

static int gether_rx_desc_init(gether_dev_t *gether){
    size_t      size;
    off64_t     offset;
    int         i, rc;
    struct mbuf     *m;
    /* mbuf pointer array, corresponding to rx descriptor ring */
    size = sizeof(struct mbuf *) * NUM_RX_DESC;
    gether->rx_pkts = malloc(size, M_DEVBUF, M_NOWAIT);
    if (gether->rx_pkts == NULL) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to allocate rx mbuf array");
        return rc;
    }
    /* Map in uncached memory for rx descriptor ring */
    if ((gether->rx_bd = mmap  (NULL, sizeof(GETHER_RX_RD) * NUM_RX_DESC,
                               PROT_READ | PROT_WRITE | PROT_NOCACHE,
                               MAP_SHARED, gether->fd, 0)) == MAP_FAILED) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to allocate rx descriptors");
        return rc;
    }
    if (mem_offset64(gether->rx_bd, NOFD, 1, &offset, 0) == -1) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "gether: Failed to get offset of Rx descriptors: %d", rc);
        return rc;
    }

    if (offset > 0xFFFFFFFF) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "gether: Error: Rx descriptor address has to be in the 32bit range. offset 0x%lx", offset);
        return EINVAL;
    }

    /* Fill Rx ring with mbufs */
    for (i = 0; i < NUM_RX_DESC; i++) {
        m = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
        if (m == NULL) {
            return ENOMEM;
        }
        gether->rx_pkts[i] = m;
        gether->rx_bd[i].status = RX_RD0_RACT;
        gether->rx_bd[i].buf_len = m->m_ext.ext_size;
        gether->rx_bd[i].addr = mbuf_phys(m);
        CACHE_FLUSH(&gether->cachectl, m->m_data, gether->rx_bd[i].addr, m->m_ext.ext_size);
    }
    gether->rx_bd[i - 1].status |= RX_RD0_RDLE;
    out32(gether->base + GETHER_RDLAR, offset);
    out32(gether->base + GETHER_RDFAR, offset);
    out32(gether->base + GETHER_RDFXR, 0);
    out32(gether->base + GETHER_RDFFR, 1);
    return EOK;
}


static int gether_tx_desc_init(gether_dev_t *gether){
    size_t      size;
    off64_t     offset;
    int         i, rc;

    /* mbuf pointer array, corresponding to tx descriptor ring */
    size = sizeof(struct mbuf *) * NUM_TX_DESC;
    gether->tx_pkts = malloc(size, M_DEVBUF, M_NOWAIT);
    if (gether->tx_pkts == NULL) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to allocate tx mbuf array");
        return rc;
    }
    /* No packets in tx ring to start */
    memset(gether->tx_pkts, 0x00, size);
    /* Map in uncached memory for tx descriptor ring */
    if ((gether->tx_bd = mmap (NULL, sizeof(GETHER_TX_TD) * NUM_TX_DESC,
                                PROT_READ | PROT_WRITE | PROT_NOCACHE,
                                 MAP_SHARED, gether->fd, 0)) == MAP_FAILED) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to allocate tx descriptors");
        return rc;
    }
    if (mem_offset64(gether->tx_bd, NOFD, 1, &offset, 0) == -1) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "gether: Failed to get offset of Tx descriptors: %d", rc);
        return rc;
    }

    if (offset > 0xFFFFFFFF) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "gether: Error: Tx descriptor address has to be in the 32bit range. offset 0x%lx", offset);
        return EINVAL;
    }

    for (i = 0; i < NUM_TX_DESC; i++) {
       gether->tx_bd[i].status = 0;
    }
    gether->tx_bd[i - 1].status = TX_TD0_TDLE;

    out32(gether->base + GETHER_TDLAR, offset);
    out32(gether->base + GETHER_TDFAR, offset);
    out32(gether->base + GETHER_TDFXR, 0);
    out32(gether->base + GETHER_TDFFR, 0x1);
    return EOK;
}

static void gether_config(gether_dev_t *gether){

    out32(gether->base + GETHER_EDSR, GETHER_EDSR_ENT | GETHER_EDSR_ENR);
    out32(gether->base + GETHER_EDMR, GETHER_EDMR_DL_16 | GETHER_EDMR_DE_LITTLE | GETHER_EDMR_NBST);
    out32(gether->base + GETHER_EESIPR, 0);
    out32(gether->base + GETHER_TRSCER, 0);
    out32(gether->base + GETHER_TFTR,0);
    out32(gether->base + GETHER_FDR,GETHER_FDR_TFD(GETHER_FDR_TFD_4K) | GETHER_FDR_RFD(GETHER_FDR_RFD_8K));
    out32(gether->base + GETHER_RMCR, GETHER_RMCR_RNC);
    out32(gether->base + GETHER_RPADIR, 0);

    /* Configure e-mac registers */
    out32(gether->base + GETHER_ECSIPR, 0);
    out32(gether->base + GETHER_RFLR, RFLR_RFL_MIN);
    out32(gether->base + GETHER_PIPR, 0);
    out32(gether->base + GETHER_APR, APR_AP);
    out32(gether->base + GETHER_MPR, MPR_MP);
    out32(gether->base + GETHER_TPAUSER, TPAUSER_TPAUSE);
    out32(gether->base + GETHER_ECSR, 0);

    gether_EDMAC(gether, EDMAC_STATUS_ENABLE);

}

int gether_attach (struct device *parent, struct device *self, void *aux)
{
    gether_dev_t    *gether;
    attach_args_t   *attach_args;
    struct  ifnet   *ifp;
    nic_config_t    *cfg;
    int             rc;
    extern int      pkt_typed_mem_fd; // use io-pkt typed memory

    gether = (gether_dev_t*)self;
    attach_args = aux;

    /* Copy the config across */
    cfg = &gether->cfg;
    gether->drv_mutex = NIC_MUTEX_UNLOCKED_VALUE;

    memcpy(cfg, &attach_args->cfg, sizeof(*cfg));
    gether->set_flow = attach_args->set_flow;

    cfg->connector = NIC_CONNECTOR_MII;

    gether->iopkt = attach_args->iopkt;
    gether->dev.dv_dll_hdl = attach_args->dll_hdl;
    callout_init(&gether->mii_callout);

    if (attach_args->bs_options) {
        gether->bs_options = attach_args->bs_options;
        attach_args->bs_options = NULL;
    }

    gether->cachectl.fd = NOFD;
    if (cache_init(0, &gether->cachectl, NULL) == -1) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to init cache");
        gether_cleanup(gether);
        return rc;
    }

    /* Mapping device */
    gether->base = mmap_device_io(cfg->io_window_size[0], cfg->io_window_base[0]);
    if (gether->base == (uintptr_t)MAP_FAILED) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to mmap device");
        gether_cleanup(gether);
        return rc;
    }

    /* Setup the hardware */
    gether_reset(gether);
    /* Use typed mem for descriptors and packets since ethernet DMA has 32-bit boundary */
    gether->fd = pkt_typed_mem_fd;
    if (gether->fd == NOFD) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Error: pkt_typed_mem_fd is NOFD");
        gether_cleanup(gether);
        return ENOMEM;
    }

    rc = gether_rx_desc_init(gether);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to init RX Descriptors: %d", rc);
        gether_cleanup(gether);
        return rc;
    }

    rc = gether_tx_desc_init(gether);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to init TX Descriptors: %d", rc);
        gether_cleanup(gether);
        return rc;
    }
    gether->sdhook = shutdownhook_establish(gether_shutdown, gether);

    gether_config(gether);

    gether_set_mac(gether);

    rc = gether_phy_init(gether);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to find PHY: %d", rc);
        gether_cleanup(gether);
        return rc;
    }

    gether->rx_ient.func = gether_process_interrupt_rx;
    gether->rx_ient.enable = gether_enable_interrupt_rx;
    gether->rx_ient.arg = gether;

    if ((rc = interrupt_entry_init(&gether->rx_ient, 0, NULL,
        cfg->priority)) != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to init the interrupt rx entry: %d", rc);
        gether_cleanup(gether);
        return rc;
    }

    gether->tx_ient.func = gether_process_interrupt_tx;
    gether->tx_ient.enable = gether_enable_interrupt_tx;
    gether->tx_ient.arg = gether;

    if ((rc = interrupt_entry_init(&gether->tx_ient, 0, NULL,
        cfg->priority)) != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to init the interrupt tx entry: %d", rc);
        gether_cleanup(gether);
        return rc;
    }

    if(attach_args->isr == 1) {
        rc = InterruptAttachArray_r(cfg->irq[0], gether_isr, gether, sizeof(gether),
                               _NTO_INTR_FLAGS_TRK_MSK);
        if (rc < 0) {
            rc = -rc;
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Failed to attach to the interrupt: %d", rc);
            gether_cleanup(gether);
            return rc;
        }
        gether->iid = rc;
    } else {
        pthread_create(&gether->tid, NULL, gether_isr_thread, gether);
    }
    gether_clear_stats(gether);

    strcpy((char *)cfg->device_description, "gether Network Device");

    ifp = &gether->ecom.ec_if;
    strcpy(ifp->if_xname, gether->dev.dv_xname);
    ifp->if_softc = gether;
    /* No multicast filtering in hardware so set IFF_ALLMULTI */
    ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST | IFF_ALLMULTI;
    ifp->if_ioctl = gether_ioctl;
    ifp->if_start = gether_start;
    ifp->if_init  = gether_init;
    ifp->if_stop  = gether_stop;
    IFQ_SET_READY(&ifp->if_snd);

    if_attach(ifp);
    ether_ifattach(ifp, cfg->current_address);

    atomic_clr(&gether->flags, 0xFFFFFFFF);

    out32(gether->base + GETHER_EESR, 0xFFFFFFFF);
    gether_enable_interrupt_rx(gether);
    gether_enable_interrupt_tx(gether);
    return EOK;
}

static char *gether_opts[] = {
    "flow",
#define OPT_FLOW    0
    "event",
#define OPT_EVENT   1
    "bs",
#define OPT_BS      2
    NULL
};

static int gether_parse_options (const char *optstring,
                 attach_args_t *attach_args)
{
    nic_config_t   *cfg;
    char           *options, *freeptr, *value;
    int            opt;

    if (optstring == NULL) {
        return 0;
    }

    cfg = &attach_args->cfg;

    /* getsubopt() is destructive */
    freeptr = options = strdup(optstring);

    while (options && *options != '\0') {
        opt = getsubopt(&options, gether_opts, &value);
        switch (opt) {

        case OPT_FLOW:
            attach_args->set_flow = strtoul(value, 0, 0);
            if (attach_args->set_flow > 3) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Unknown flow value %d, forcing off",
                      attach_args->set_flow);
                attach_args->set_flow = 0;
            }
            break;
        case OPT_EVENT:
            attach_args->isr = 0;
            break;
        case OPT_BS:
            attach_args->bs_options = strdup(value);
            break;
        default:
            if (nic_parse_options(cfg, value) != EOK) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "gether: Unknown option %s", value);
                (free)(freeptr);
                return EINVAL;
            }
        }
    }
    (free)(freeptr);
    return EOK;
}

CFATTACH_DECL(gether,
    sizeof(gether_dev_t),
    NULL,
    gether_attach,
    gether_detach,
    NULL);

static void gether_parse_syspage (nic_config_t *cfg)
{
    hwi_tag        *tag;
    unsigned        item;

    /* Find network hardware information. */
    item = hwi_find_device("gether", 0);
    if (item == HWI_NULL_OFF) {
       return;
    }

    if (cfg->num_io_windows == 0) {
    /* Find base address */
        if ((tag = hwi_tag_find(item, HWI_TAG_NAME_location, NULL)) != NULL) {
            cfg->io_window_base[0] = (paddr_t)tag->location.base;
            cfg->num_io_windows = 1;
        }
    }

    if (cfg->num_irqs == 0) {
       /* Find IRQ */
        cfg->irq[0] = hwitag_find_ivec(item, NULL);
        if (cfg->irq[0] != HWI_ILLEGAL_VECTOR) {
            cfg->num_irqs = 1;
        }
    }

    /* Find MAC address */
    hwitag_find_nicaddr(item, NULL, cfg->permanent_address);
}

int gether_mounted() {
    struct drvcom_config    dcon;
    struct ifnet            *ifp;
    int rc;
    int mounted = 0;

    /* Check if it is already mounted by doing a "nicinfo" on each interface */
    IFNET_FOREACH(ifp) {
        dcon.dcom_cmd.ifdc_cmd = DRVCOM_CONFIG;
        dcon.dcom_cmd.ifdc_len = sizeof(dcon.dcom_config);
        rc = ifp->if_ioctl(ifp, SIOCGDRVCOM, (caddr_t)&dcon);
        if ((rc == EOK) && (dcon.dcom_config.num_io_windows > 0) &&
                (dcon.dcom_config.io_window_base[0] == GETHER_REG_BASE) ) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s:%d - Driver already loaded for %s",
                  __FUNCTION__, __LINE__, ifp->if_xname);
            mounted = 1;
            break;
        }
    }
    return (mounted ? EBUSY : EOK);
}

int gether_entry (void *dll_hdl,  struct _iopkt_self *iopkt, char *options)
{
    attach_args_t    *attach_args;
    nic_config_t    *cfg;
    struct device    *dev;
    int            rc, single;

    /* Check if the driver is mounted already */
    rc = gether_mounted();
    if (rc != EOK) {
        return rc;
    }

    /* Ask for IO permission */
    if (ThreadCtl(_NTO_TCTL_IO_PRIV, 0) == -1) {
        perror("ThreadCtl");
        return -1;
    }

    attach_args = calloc(1,sizeof(*attach_args));
    if (attach_args == NULL) {
        return ENOMEM;
    }
    attach_args->iopkt = iopkt;
    attach_args->dll_hdl = dll_hdl;

    /* Set defaults before parsing the command line */
    cfg = &attach_args->cfg;
    cfg->media_rate = -1;
    cfg->duplex = -1;
    attach_args->set_flow = -1;
    cfg->mtu = ETHERMTU;
    cfg->mru = ETHERMTU;
    cfg->flags = NIC_FLAG_MULTICAST;
    cfg->mac_length = ETHER_ADDR_LEN;
    cfg->priority = stk_ctl.rx_prio;
    attach_args->isr = 1;

    if (gether_parse_options(options, attach_args) != EOK) {
        (free)(attach_args);
        return EINVAL;
    }

    /* If setting speed, must set duplex and vice versa */
    if (((cfg->media_rate != -1) && (cfg->duplex == -1)) ||
       ((cfg->media_rate == -1) && (cfg->duplex != -1))) {
       slogf(_SLOGC_NETWORK, _SLOG_ERROR,
           "gether: Must set both speed and duplex if setting either");
       (free)(attach_args);
       return EINVAL;
    }
    /* If speed set then turn flow off if not set */
    if ((cfg->media_rate != -1) && (attach_args->set_flow == -1)) {
       attach_args->set_flow = 0;
    }
    /*
     * If base address, IRQ, MAC address are not set from the command line,
     * set them from the syspage info and if not there then set to
     * default values.
     */
    gether_parse_syspage(cfg);

    if (cfg->num_io_windows == 0) {
        cfg->io_window_base[0] = GETHER_REG_BASE;
        cfg->num_io_windows = 1;
    }
    cfg->io_window_size[0] = GETHER_REG_SIZE;

    if (cfg->num_irqs == 0) {
        cfg->irq[0] = GETHER_IRQ;
        cfg->num_irqs = 1;
    }

    if (memcmp(cfg->current_address, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0) {
        memcpy(cfg->current_address, cfg->permanent_address, ETHER_ADDR_LEN);
    }

    if ((memcmp(cfg->current_address, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0) ||
       ETHER_IS_MULTICAST(cfg->current_address)) {
           slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "gether: Invalid MAC address %02x%02x%02x%02x%02x%02x",
              cfg->current_address[0], cfg->current_address[1],
              cfg->current_address[2], cfg->current_address[3],
              cfg->current_address[4], cfg->current_address[5]);
       (free)(attach_args);
       return EINVAL;
    }


    dev = NULL;
    rc = dev_attach("rg", options, &gether_ca, attach_args, &single, &dev, NULL);
    (free)(attach_args);

    return rc;
}

struct _iopkt_drvr_entry IOPKT_DRVR_ENTRY_SYM(gether) = IOPKT_DRVR_ENTRY_SYM_INIT(gether_entry);

#ifdef VARIANT_a
#include <nw_dl.h>
/* This is what gets specified in the stack's dl.c */
struct nw_dll_syms gether_syms[] = {
    {"iopkt_drvr_entry", &IOPKT_DRVR_ENTRY_SYM(gether)},
    {NULL, NULL}
};
#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devnp/rgbe/gether.c $ $Rev: 888807 $")
#endif
