/*
 * Copyright (c) 2016, 2022, 2023, BlackBerry Limited.
 * Copyright (c) 2022, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ravb.h"
#include "m88q2110.h"

#include <fcntl.h>
#include <drvr/hwinfo.h>
#include <net/ifdrvcom.h>
#include <sys/sockio.h>
#include <netdrvr/smmu.h>
#include <aarch64/r-car.h>

#if defined(VARIANT_ipmmu)
#include <hw/rcar_ipmmu-api.h>
#endif /* defined(VARIANT_ipmmu) */

int ravb_port_num = 1;

static void ravb_set_mac (ravb_dev_t *ravb);
static void ravb_cleanup (ravb_dev_t *ravb);

int ravb_wait(ravb_dev_t *ravb, int reg, uint32_t mask, uint32_t value)
{
    int i;

    for (i = 0; i < 10000; i++) {
        if ((in32(ravb->base + reg) & mask) == value) {
            return EOK;
        }
        nic_delay(10);
    }
    return -ETIMEDOUT;
}

int ravb_operate(ravb_dev_t *ravb)
{
    uint32_t queue;

    if ((in32(ravb->base + CCC) & CCC_OPC_OPERATION) != CCC_OPC_OPERATION) {
        /*
         * When opmode is changed to operation
         * CDAR gets descriptor from DBAT, which stores the head of descriptor queue
         */
        for (queue = 0; queue < NUM_TX_QUEUES; queue++)
        {
            ravb->tx_pidx[queue] = 0;
            ravb->tx_cidx[queue] = 0;
        }
        for (queue = 0; queue < NUM_RX_QUEUES; queue++)
        {
            ravb->rx_idx[queue] = 0;
        }

        out32(ravb->base + CCC, (in32(ravb->base + CCC) & ~CCC_OPC) | CCC_OPC_OPERATION);

        if((in32(ravb->base + CSR) & CSR_OPS) != CSR_OPS_OPERATION) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "failed to switch device to operation mode CSR=0x%x", in32(ravb->base + CSR));
        }
    }

    return EOK;
}

int ravb_config(ravb_dev_t *ravb)
{
    int error;

    nic_mutex_lock(&ravb->drv_mutex);
    /* Set config mode */
    out32(ravb->base + CCC,
        (in32(ravb->base + CCC) & ~CCC_OPC) | CCC_OPC_CONFIG);
    /* Check if the operating mode is changed to the config mode */
    error = ravb_wait(ravb, CSR, CSR_OPS, CSR_OPS_CONFIG);
    nic_mutex_unlock(&ravb->drv_mutex);
    if (error) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "failed to switch device to config mode");
    }

    return error;
}

/* Device init function for Ethernet AVB */
static int ravb_dmac_init(ravb_dev_t *ravb)
{
    int reg32;

    /* Set AVB RX */
    out32(ravb->base + RCR, RCR_EFFS | RCR_ENCF | RCR_ETS0 | RCR_ESF | 0x18000000);

    /* Set FIFO size */
    out32(ravb->base + TGC, TGC_TQP_AVBMODE1 | 0x00112200);

    /* clk delay setting */
    if (!(strstr(ravb->options.soc_name, "D3") || 
        strstr(ravb->options.soc_name, "E3") ||
        strstr(ravb->options.soc_name, "V4H"))) {
        reg32 = in32(ravb->base + APSR) & ~(APSR_TDM | APSR_RDM);
        /* Limitation for V4H 1.0: If using internal Tx clock Delay Mode, need to change IO voltage to 1.8V at 1Gbps (can change in startup) */
        if (ravb->options.txc_delay) {
            reg32 |= APSR_TDM;
        }
        if (ravb->options.rxc_delay) {
            reg32 |= APSR_RDM;
       }
        out32(ravb->base + APSR, reg32);
    }

    /* Timestamp enable */
    out32(ravb->base + TCCR, TCCR_TFEN);

    /* Clear DIL.DPLx */
    out32(ravb->base + DIL, 0);

    /* Set queue specific interrupt */
    out32(ravb->base + CIE, CIE_CRIE | CIE_CTIE | CIE_CL0M);

    /* Frame receive */
    out32(ravb->base + RIC0 , RIC0_FRE0 | RIC0_FRE1);

    /* Disable FIFO full warning */
    out32(ravb->base + RIC1 , 0);

    /* Receive FIFO full error, descriptor empty */
    out32(ravb->base + RIC2 , RIC2_QFE0 | RIC2_QFE1 | RIC2_RFFE);

    /* Frame transmitted, timestamp FIFO updated */
    out32(ravb->base + TIC , TIC_FTE0 | TIC_FTE1);

    return 0;
}

/* E-MAC init function */
static int ravb_emac_init(ravb_dev_t *ravb)
{
    /* Disable MAC Interrupt */
    out32(ravb->base + ECSIPR, 0);

    /* Recv frame limit set register */
    out32(ravb->base + RFLR, RFLR_RFL_MIN);

    /* PAUSE prohibition */
    out32(ravb->base + ECMR, ECMR_ZPF | ECMR_DM | ECMR_TE | ECMR_RE);

    /* Set MAC address */
    ravb_set_mac(ravb);

    /* E-MAC status register clear */
    out32(ravb->base + ECSR, ECSR_ICD | ECSR_MPD);

    /* E-MAC interrupt enable register */
    out32(ravb->base + ECSIPR, ECSIPR_ICDIP | ECSIPR_MPDIP | ECSIPR_LCHNGIP);

    return 0;
}

static int ravb_tx_desc_init(ravb_dev_t *ravb)
{
    size_t          size;
    off64_t         offset;
    uint32_t        i, queue;
    RAVB_TX_DESC    *tx_desc;
#if defined(VARIANT_ipmmu)
    iommu_node_t *node;
#endif /* defined(VARIANT_ipmmu) */

    for (queue = 0; queue < NUM_TX_QUEUES; queue++)
    {
        /* mbuf pointer array, corresponding to tx descriptor ring */
        size = sizeof(struct mbuf *) * NUM_TX_DESC;
        ravb->tx_pkts[queue] = malloc(size, M_DEVBUF, M_NOWAIT);
        if (ravb->tx_pkts[queue] == NULL) {
            return errno;
        }

        /* No packets in tx ring to start */
        memset(ravb->tx_pkts[queue], 0x00, size);

        /* Allocate all TX descriptors. */
        size = sizeof(RAVB_TX_DESC) * NUM_TX_DESC_WITH_LINK;
        ravb->tx_bd[queue] = mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE,
                            MAP_SHARED, ravb->fd, 0);
        if (ravb->tx_bd[queue] == MAP_FAILED) {
            return errno;
        }
        if (mem_offset64((void *)ravb->tx_bd[queue], NOFD, 1, &offset, 0) == -1) {
            return errno;
        }
        memset((void *)ravb->tx_bd[queue], 0x00, size);

#if !defined(VARIANT_ipmmu)
        if (smmu_map_driver_memory((uint64_t)offset, size) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed tx_bd smmu map, queue %d "
                  "errno %d", queue, errno);
            return errno;
        }

#else /* defined(VARIANT_ipmmu) */
        node = iommu_get_node(ravb, offset, size);
        if (node == NULL) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: iommu failed to get node, Tx phys 0x%lx, size %ld, queue %d",
                offset, size, queue);
            return ENOMEM;
        }

#endif /* !defined(VARIANT_ipmmu) */

        /* Build TX ring buffer */
        for (i = 0; i < NUM_TX_DESC; i++)
        {
            tx_desc = &ravb->tx_bd[queue][i];
            tx_desc->die_dt = DT_EEMPTY;
        }
        tx_desc = &ravb->tx_bd[queue][i];
#if !defined(VARIANT_ipmmu)
        tx_desc->dptr = (uint32_t)offset;
#else /* defined(VARIANT_ipmmu) */
        tx_desc->dptr = node->iova + (/*start offset*/offset - node->paddr);
#endif /* !defined(VARIANT_ipmmu) */
        tx_desc->die_dt = DT_LINKFIX; /* type */

        CACHE_FLUSH(&ravb->cachectl, (void *)ravb->tx_bd[queue], offset, size);
    }

    return EOK;
}

static int ravb_rx_desc_init(ravb_dev_t *ravb)
{
    size_t          size;
    off64_t         offset;
    uint32_t        i, queue;
    struct mbuf     *m;
    RAVB_RX_DESC    *rx_desc;
#if defined(VARIANT_ipmmu)
    uint64_t        m_phys;
    size_t          m_size;
    iommu_node_t *node, *m_node;
#endif /* defined(VARIANT_ipmmu) */

    for (queue = 0; queue < NUM_RX_QUEUES; queue++) {
        /* mbuf pointer array, corresponding to rx descriptor ring */
        size = sizeof(struct mbuf *) * NUM_RX_DESC;
        ravb->rx_pkts[queue] = malloc(size, M_DEVBUF, M_NOWAIT);
        if (ravb->rx_pkts[queue] == NULL) {
            return errno;
        }
        memset(ravb->rx_pkts[queue], 0, size);

        /* Allocate all RX descriptors. */
        size = sizeof(RAVB_RX_DESC) * NUM_RX_DESC_WITH_LINK;
        ravb->rx_bd[queue] = mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE,
                            MAP_SHARED, ravb->fd, 0);
        if (ravb->rx_bd[queue] == MAP_FAILED) {
            return errno;
        }
        if (mem_offset64((void *)ravb->rx_bd[queue], NOFD, 1, &offset, 0) == -1) {
            return errno;
        }
        memset((void *)ravb->rx_bd[queue], 0, size);
#if !defined(VARIANT_ipmmu)
        if (smmu_map_driver_memory((uint64_t)offset, size) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed rx_bd smmu map, queue %d "
                  "errno %d", queue, errno);
            return errno;
        }

#else /* defined(VARIANT_ipmmu) */
        node = iommu_get_node(ravb, offset, size);
        if (node == NULL) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: iommu failed to get node, Rx phys 0x%lx, size %ld, queue %d",
                offset, size, queue);
            return ENOMEM;
        }

#endif /* !defined(VARIANT_ipmmu) */

        /* Build RX ring buffer */
        for (i = 0; i < NUM_RX_DESC; i++) {
            m = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
            if (m == NULL) {
                return ENOMEM;
            }
            /* RX descriptor */
            rx_desc = &ravb->rx_bd[queue][i];

            /* The size of the buffer should be on 4-byte boundary. */
            rx_desc->ds_cc = ALIGN(PKT_BUF_SZ, DPTR_ALIGN);
            ravb->rx_pkts[queue][i] = m;
            rx_desc->die_dt = DT_FEMPTY;
#if !defined(VARIANT_ipmmu)
            rx_desc->dptr = mbuf_phys(m);
            CACHE_FLUSH(&ravb->cachectl, m->m_data, rx_desc->dptr,
                            m->m_ext.ext_size);

#else /* defined(VARIANT_ipmmu) */
            m_phys = mbuf_phys(m);
            m_size = m->m_ext.ext_size;
            m_node = iommu_get_node(ravb, m_phys, m_size);
            if (m_node == NULL) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                        "ravb: iommu failed to get node, Rx mbuf idx %d, phys 0x%lx, size %ld",
                        i, m_phys, m_size);
                return ENOMEM;
            }
            rx_desc->dptr = m_node->iova + (/*start offset*/m_phys - m_node->paddr);
            CACHE_FLUSH(&ravb->cachectl, m->m_data, m_phys, m_size);

#endif /* !defined(VARIANT_ipmmu) */
        }
        rx_desc = &ravb->rx_bd[queue][i];

#if !defined(VARIANT_ipmmu)
        rx_desc->dptr = (uint32_t)offset;
#else /* defined(VARIANT_ipmmu) */
        rx_desc->dptr = node->iova + (/*start offset*/offset - node->paddr);
#endif /* !defined(VARIANT_ipmmu) */

        rx_desc->die_dt = DT_LINKFIX; /* type */

        CACHE_FLUSH(&ravb->cachectl, (void *)ravb->rx_bd[queue], offset, size);
    }

    return EOK;
}

static int ravb_desc_bat_init(ravb_dev_t *ravb)
{
    size_t          size;
    off64_t         offset;
    uint32_t        i;
    RAVB_DESC       *desc;

    /* Allocate descriptor base address table. They should be aligned */
    /* to size of struct ravb_desc. */
    size = sizeof(RAVB_DESC) * DBAT_ENTRY_NUM;
    ravb->desc_bat = mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE,
                        MAP_SHARED, ravb->fd, 0);
    if (ravb->desc_bat == MAP_FAILED) {
        return errno;
    }

    if (mem_offset64((void *)ravb->desc_bat, NOFD, 1, &offset, 0) == -1) {
        return errno;
    }

    /* Initialize all descriptors */
    memset((void *)ravb->desc_bat, 0x0, size);

#if !defined(VARIANT_ipmmu)
    if (smmu_map_driver_memory((uint64_t)offset, size) != 0) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed desc_bat smmu map, "
              "errno %d", errno);
        return errno;
    }
    ravb->desc_bat_addr = offset;

#else /* defined(VARIANT_ipmmu) */
    iommu_node_t *node = iommu_get_node(ravb, offset, size);
    if (node == NULL) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "ravb: iommu failed to get node, BAT phys 0x%lx, size %ld",
              offset, size);
        return ENOMEM;
    }
    ravb->desc_bat_dma = node->iova + (/*start offset*/node->paddr - offset);

#endif /* !defined(VARIANT_ipmmu) */

    for (i = 0; i < DBAT_ENTRY_NUM; i++) {
        ravb->desc_bat[i].die_dt = DT_EOS;
    }

    /* Register the descriptor base address table */
#if !defined(VARIANT_ipmmu)
    out32(ravb->base + DBAT, offset);
#else /* defined(VARIANT_ipmmu) */
    out32(ravb->base + DBAT, ravb->desc_bat_dma);
#endif /* !defined(VARIANT_ipmmu) */

    for(i = 0; i < NUM_RX_QUEUES; i++) {
        /* RX descriptor base address for all queues */
        desc = &ravb->desc_bat[RX_QUEUE_OFFSET + i];
        desc->die_dt = DT_LINKFIX; /* type */
        desc->dptr = ravb->rx_bd[i][NUM_RX_DESC].dptr;
    }

    for(i = 0; i < NUM_TX_QUEUES; i++) {
        /* TX descriptor base address for all queues */
        desc = &ravb->desc_bat[TX_QUEUE_OFFSET + i];
        desc->die_dt = DT_LINKFIX; /* type */
        desc->dptr = ravb->tx_bd[i][NUM_TX_DESC].dptr;
    }

    CACHE_FLUSH(&ravb->cachectl, (void *)ravb->desc_bat, offset, size);

    return EOK;
}

static int ravb_reset (ravb_dev_t *ravb)
{
    int error;

    nic_mutex_lock(&ravb->drv_mutex);
    /* set reset mode */
    out32(ravb->base + CCC, CCC_OPC_RESET);
    error = ravb_wait(ravb, CSR, CSR_OPS, CSR_OPS_RESET);
    nic_mutex_unlock(&ravb->drv_mutex);
    if (error) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "failed to switch device to reset mode");
    }
    return error;
}

static void ravb_shutdown (void *arg)
{
    ravb_dev_t      *ravb;

    ravb = arg;

    /*
     * Flow for Emergency stop
     * At this point, reads cannot be guranteed,
     * skipping steps to verify CSR register.
     */
    out32(ravb->base + CCC, in32(ravb->base + CCC) | CCC_DTSR);
    out32(ravb->base + CCC, (in32(ravb->base + CCC) & ~(CCC_OPC)) | CCC_OPC_RESET);

#if defined(VARIANT_ipmmu)
    /* To ensure memory are freed during slay.
     * We can ignore any other iommu errors after this since
     *   the connection with smmuman has been severed
     */
    iommu_cleanup(ravb);
#endif /* defined(VARIANT_ipmmu) */

}

static void ravb_cleanup (ravb_dev_t *ravb)
{
    struct mbuf   *m;
    int           i, queue;
#if !defined(VARIANT_ipmmu)
    off64_t       offset;
#endif /* !defined(VARIANT_ipmmu) */
    nic_config_t  *cfg;

    /* Disable interrupts by clearing the interrupt masks. */
    out32(ravb->base + RIC0, 0);
    out32(ravb->base + RIC2, 0);
    out32(ravb->base + TIC, 0);

    cfg = &ravb->options.cfg;
    for (i = 0; i < cfg->num_irqs; i++) {
        if (ravb->iid[i]) {
            InterruptDetach(ravb->iid[i]);
        }
    }

    ravb_ptp_stop(ravb);

    /* Checks and returns EINVAL if not in the interrupt_entry list */
    interrupt_entry_remove(&ravb->ient_rx, NULL);
    interrupt_entry_remove(&ravb->ient_tx, NULL);

#if defined(VARIANT_ipmmu)
    iommu_cleanup(ravb);
#endif /* defined(VARIANT_ipmmu) */

    if (ravb->base) {
        ravb_reset(ravb);
#if !defined(VARIANT_ipmmu)
        if (smmu_unregister_mmio_device(ravb->options.cfg.io_window_base[0],
            ravb->options.cfg.io_window_size[0]) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed unregister mmio device, "
                 "errno %d", errno);
        }
#endif /* !defined(VARIANT_ipmmu) */
        munmap_device_io(ravb->base, ravb->options.cfg.io_window_size[0]);
    }

    shutdownhook_disestablish(ravb->sdhook);

    /* Cleanup Tx allocations */
    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        if (ravb->tx_pkts[queue]) {
            for (i = 0; i < NUM_TX_DESC; i++) {
                m = ravb->tx_pkts[queue][i];
                if (m != NULL) {
                    m_freem(m);
                }
            }
            free(ravb->tx_pkts[queue], M_DEVBUF);
        }
        if (ravb->tx_bd[queue]) {
#if !defined(VARIANT_ipmmu)
            offset = ravb->tx_bd[queue][NUM_TX_DESC].dptr;
            if (smmu_unmap_driver_memory((uint64_t)offset,
                                         sizeof(RAVB_TX_DESC) *
                                         NUM_TX_DESC_WITH_LINK) != 0) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed unmap tx_bd, "
                      "queue %d, errno %d", queue, errno);
            }
#endif /* !defined(VARIANT_ipmmu) */
            munmap((void *)ravb->tx_bd[queue], sizeof(RAVB_TX_DESC) * NUM_TX_DESC_WITH_LINK);
        }
    }

    /* Cleanup Rx allocations */
    for (queue = 0; queue < NUM_RX_QUEUES; queue++) {
        if (ravb->rx_pkts[queue]) {
            for (i = 0; i < NUM_RX_DESC; i++) {
                m = ravb->rx_pkts[queue][i];
                if (m != NULL) {
                    m_freem(m);
                }
            }
            free(ravb->rx_pkts[queue], M_DEVBUF);
        }
        if (ravb->rx_bd[queue]) {
#if !defined(VARIANT_ipmmu)
            offset = ravb->rx_bd[queue][NUM_RX_DESC].dptr;
            if (smmu_unmap_driver_memory((uint64_t)offset,
                                         sizeof(RAVB_RX_DESC) *
                                         NUM_RX_DESC_WITH_LINK) != 0) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed unmap rx_bd, "
                      "queue %d, errno %d", queue, errno);
            }
#endif /* !defined(VARIANT_ipmmu) */
            munmap((void *)ravb->rx_bd[queue], sizeof(RAVB_RX_DESC) * NUM_RX_DESC_WITH_LINK);
        }
    }

    if (ravb->desc_bat) {
#if !defined(VARIANT_ipmmu)
        offset = ravb->desc_bat_addr;
        if (smmu_unmap_driver_memory((uint64_t)offset,
                                     sizeof(RAVB_DESC) *
                                     DBAT_ENTRY_NUM) != 0) {
               slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed unmap desc_bat, "
                     "errno %d", errno);
        }
#endif /* !defined(VARIANT_ipmmu) */
        munmap((void *)ravb->desc_bat, sizeof(RAVB_DESC) * DBAT_ENTRY_NUM);
    }
    if (ravb->options.avb) {
        nw_pthread_reap (ravb->tid);
    }

    if (ravb->fd) {
        close(ravb->fd);
    }
    cache_fini(&ravb->cachectl);
}

void ravb_hw_stop (ravb_dev_t *ravb)
{
    struct mbuf         *m;
    uint32_t            i, queue;

    nic_mutex_lock(&ravb->drv_mutex);
    /* Wait for stopping the hardware TX process */
    ravb_wait(ravb, TCCR, TCCR_TSRQ0 | TCCR_TSRQ1 | TCCR_TSRQ2 | TCCR_TSRQ3, 0);
    ravb_wait(ravb, CSR, CSR_TPO0 | CSR_TPO1 | CSR_TPO2 | CSR_TPO3, 0);

    /* Stop Tx and Rx */
    out32(ravb->base + ECMR, in32(ravb->base + ECMR) & ~(ECMR_RE | ECMR_TE));

    /* Wait for stopping the RX DMA process */
    ravb_wait(ravb, CSR, CSR_RPO, 0);
    nic_mutex_unlock(&ravb->drv_mutex);

    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        ravb_reap_tx(ravb, queue);
        /* Clear any pending Tx buffers */
        for (i = 0; i < NUM_TX_DESC; i++) {
            m = ravb->tx_pkts[queue][i];
            if (m != NULL) {
                m_freem(m);
            }
            ravb->tx_bd[queue][i].die_dt = DT_EEMPTY;
        }
    }
}

static void ravb_stop (struct ifnet *ifp, int disable)
{
    ravb_dev_t          *ravb;
    nic_config_t        *cfg;
    struct nw_work_thread   *wtp;

    ravb = ifp->if_softc;
    cfg = &ravb->options.cfg;
    wtp = WTP;

    NW_SIGLOCK_P(&ifp->if_snd_ex, ravb->options.iopkt, wtp);
    callout_stop(&ravb->mii_callout);
    MDI_DisableMonitor(ravb->mdi);
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);
    ifp->if_flags &= ~(IFF_RUNNING | IFF_OACTIVE);
    NW_SIGUNLOCK_P(&ifp->if_snd_ex, ravb->options.iopkt, wtp);

    ravb_hw_stop(ravb);

    MDI_PowerdownPhy(ravb->mdi, cfg->phy_addr);
    ravb_clear_stats(ravb);
}

int ravb_detach (struct device *dev, int flags)
{
    ravb_dev_t      *ravb;
    struct ifnet    *ifp;

    ravb = (ravb_dev_t *)dev;
    ifp = &ravb->ecom.ec_if;

    callout_stop(&ravb->mii_callout);
    MDI_PowerdownPhy(ravb->mdi, ravb->options.cfg.phy_addr);

    ether_ifdetach(ifp);
    if_detach(ifp);
    ravb_phy_fini(ravb);

    ravb_cleanup(ravb);

    return EOK;
}

static void ravb_set_mac (ravb_dev_t *ravb)
{
    uint32_t mach, macl;
    uint8_t *mac;

    mac = ravb->options.cfg.current_address;
    mach = (mac[0] << 24) | (mac[1] << 16) | (mac[2] << 8) | mac[3];
    macl = (mac[4] << 8) | mac[5];
    out32(ravb->base + MAHR, mach);
    out32(ravb->base + MALR, macl);
}

static int ravb_init (struct ifnet *ifp)
{
    ravb_dev_t      *ravb;
    nic_config_t    *cfg;
    struct nw_work_thread   *wtp;
    uint32_t        promisc_change, mac_change, hw_stop=0;
    uint32_t        rfl;

    ravb = ifp->if_softc;
    cfg = &ravb->options.cfg;
    wtp = WTP;

    /* check if configuration is required */
    mac_change = memcmp(cfg->current_address, LLADDR(ifp->if_sadl), ifp->if_addrlen);
    promisc_change = (((ifp->if_flags & IFF_PROMISC) == IFF_PROMISC) != (in32(ravb->base + ECMR) & ECMR_PRM))?1:0;
    if (mac_change | promisc_change) {
        if (in32(ravb->base + ECMR) & (ECMR_RE | ECMR_TE)) {
            hw_stop = 1;
        }
    }
    if (hw_stop) {
        ravb_hw_stop(ravb);
    }

    if (mac_change) {
        memcpy(cfg->current_address, LLADDR(ifp->if_sadl), ifp->if_addrlen);
        ravb_set_mac(ravb);

        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: MAC address updated");
        }
    }

    if (promisc_change) {
        out32(ravb->base + ECMR, in32(ravb->base + ECMR) ^ ECMR_PRM);
    }
    if (cfg->verbose) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Promiscuous mode %s", (in32(ravb->base + ECMR) & ECMR_PRM)?"enabled":"disabled");
    }

    if (ravb->ecom.ec_capenable & ETHERCAP_VLAN_MTU) {
        rfl = ifp->if_mtu + ETHER_HDR_LEN + ETHER_CRC_LEN + ETHER_VLAN_ENCAP_LEN;
    } else {
        rfl = ifp->if_mtu + ETHER_HDR_LEN + ETHER_CRC_LEN;
    }
    if(rfl != in32(ravb->base + RFLR)) {
        out32(ravb->base + RFLR, rfl);
    }
    if (cfg->verbose) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Receive frame length %d", rfl);
    }

    if (hw_stop) {
        out32(ravb->base + ECMR, in32(ravb->base + ECMR) | ECMR_RE | ECMR_TE);
    }
    if ((ifp->if_flags & IFF_RUNNING) == 0) {
        NW_SIGLOCK_P(&ifp->if_snd_ex, ravb->options.iopkt, wtp);
        ifp->if_flags |= IFF_RUNNING;
        NW_SIGUNLOCK_P(&ifp->if_snd_ex, ravb->options.iopkt, wtp);

        if (!ravb_is_br_phy(ravb) && !ravb_is_mv_phy(ravb)) {
            ravb_mediachange(ifp);
        } else {
            ravb_MDI_MonitorPhy(ravb);
        }
    }

    ravb_operate(ravb);

    return EOK;
}

static void *ravb_rx_thread (void *arg)
{
    ravb_dev_t      *ravb = arg;
    int             rc, queue;
    struct _pulse   pulse;

    while (1) {
        rc = MsgReceivePulse(ravb->chid, &pulse, sizeof(pulse), NULL);
        if (rc == EOK) {
            switch (pulse.code) {
                case RAVB_RX_PULSE:
                    queue = pulse.value.sival_int;

                    if (queue == 1 && (ravb->ris0 & RIS0_FRF1)) {
                        ravb_receive(ravb, WTP, 1);
                    }

                    if (queue == 0 && (ravb->ris0 & RIS0_FRF0)) {
                        ravb_receive(ravb, WTP, 0);
                    }
                    break;
                case RAVB_QUIESCE_PULSE:
                    quiesce_block(pulse.value.sival_int);
                    break;
                default:
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb Rx: Unknown pulse %d received", pulse.code);
                    break;
            }
        } else {
          slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb Rx: MsgReceivePulse: %s", strerror(rc));
        }
    }
    return NULL;
}

static void ravb_rx_thread_quiesce (void *arg, int die)
{
    ravb_dev_t   *ravb = arg;

    MsgSendPulse(ravb->coid, SIGEV_PULSE_PRIO_INHERIT, RAVB_QUIESCE_PULSE, die);
    return;
}

static int ravb_rx_thread_init (void *arg)
{
    ravb_dev_t          *ravb = arg;
    struct nw_work_thread   *wtp = WTP;

    pthread_setname_np(gettid(), RAVB_RX_THREAD_NAME);

    wtp->quiesce_callout = ravb_rx_thread_quiesce;
    wtp->quiesce_arg = ravb;
    return EOK;
}

static int ravb_interrupt_init(ravb_dev_t  *ravb)
{
    int rc;
    nic_config_t *cfg = &ravb->options.cfg;

    if (cfg->num_irqs == 0) {
        // AVB0
        if (strstr(ravb->options.soc_name, "V3U")) {
            cfg->irq[0] = RCAR_GEN4_INTCSYS_SPI_256; //ch0 for rx
            cfg->irq[1] = RCAR_GEN4_INTCSYS_SPI_257; //ch1 for rx
            cfg->irq[2] = RCAR_GEN4_INTCSYS_SPI_274; //ch18 for tx
            cfg->irq[3] = RCAR_GEN4_INTCSYS_SPI_275; //ch19 for tx
            cfg->irq[4] = RCAR_GEN4_INTCSYS_SPI_278; //ch22 for error & gPTP,...
            cfg->irq[5] = RCAR_GEN4_INTCSYS_SPI_280; //ch24 for emac
        } else if (strstr(ravb->options.soc_name, "V4H")) {
            cfg->irq[0] = RCAR_GEN4_INTCSYS_SPI_335; //ch0 for rx
            cfg->irq[1] = RCAR_GEN4_INTCSYS_SPI_336; //ch1 for rx
            cfg->irq[2] = RCAR_GEN4_INTCSYS_SPI_353; //ch18 for tx
            cfg->irq[3] = RCAR_GEN4_INTCSYS_SPI_354; //ch19 for tx
            cfg->irq[4] = RCAR_GEN4_INTCSYS_SPI_357; //ch22 for error & gPTP,...
            cfg->irq[5] = RCAR_GEN4_INTCSYS_SPI_359; //ch24 for emac
        } else { // Gen3
            cfg->irq[0] = RCAR_GEN3_INTCSYS_RAVB0;  //ch0 for rx
            cfg->irq[1] = RCAR_GEN3_INTCSYS_RAVB1;  //ch1 for rx
            cfg->irq[2] = RCAR_GEN3_INTCSYS_RAVB18; //ch18 for tx
            cfg->irq[3] = RCAR_GEN3_INTCSYS_RAVB19; //ch19 for tx
            cfg->irq[4] = RCAR_GEN3_INTCSYS_RAVB22; //ch22 for error and gPTP,...
            cfg->irq[5] = RCAR_GEN3_INTCSYS_RAVB24; //ch24 for emac
        }
        cfg->num_irqs = 6;
        // AVB1, AVB2,...
        {
            int k = ((cfg->io_window_base[0] >> 16) & 0xF);
            if (k > 0) {
                for (int i = 0; i < cfg->num_irqs; i++) {
                    cfg->irq[i] += 25 * k;
                }
            }
        }
    }

    rc = InterruptAttach_r(cfg->irq[0], ravb_rx_isr, ravb, sizeof(ravb),
                   _NTO_INTR_FLAGS_TRK_MSK);
    if (rc < 0) {
        return -rc;
    }
    ravb->iid[0] = rc;

    rc = InterruptAttach_r(cfg->irq[1], ravb_rx_isr, ravb, sizeof(ravb),
        _NTO_INTR_FLAGS_TRK_MSK);
    if (rc < 0) {
        return -rc;
    }
    ravb->iid[1] = rc;

    rc = InterruptAttach_r(cfg->irq[2], ravb_tx_isr, ravb, sizeof(ravb),
        _NTO_INTR_FLAGS_TRK_MSK);
    if (rc < 0) {
        return -rc;
    }
    ravb->iid[2] = rc;

    rc = InterruptAttach_r(cfg->irq[3], ravb_tx_isr, ravb, sizeof(ravb),
        _NTO_INTR_FLAGS_TRK_MSK);
    if (rc < 0) {
        return -rc;
    }
    ravb->iid[3] = rc;

    rc = InterruptAttach_r(cfg->irq[4], ravb_err_ptp_isr, ravb, sizeof(ravb),
        _NTO_INTR_FLAGS_TRK_MSK);
    if (rc < 0) {
        return -rc;
    }
    ravb->iid[4] = rc;

    rc = InterruptAttach_r(cfg->irq[5], ravb_emac_isr, ravb, sizeof(ravb),
        _NTO_INTR_FLAGS_TRK_MSK);
    if (rc < 0) {
        return -rc;
    }
    ravb->iid[5] = rc;

    return EOK;
}

int ravb_attach (struct device *parent, struct device *self, void *aux)
{
    ravb_dev_t      *ravb;
    attach_args_t   *attach_args;
    struct  ifnet   *ifp;
    nic_config_t    *cfg;
    int             rc;

    ravb = (ravb_dev_t*)self;
    attach_args = aux;

    /* Copy the config across */
    memcpy(&ravb->options, attach_args, sizeof(attach_args_t));
    cfg = &ravb->options.cfg;
    ravb->drv_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    ravb->ptp_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    ravb->mdi_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    ravb->tx_cleanup_needed = 0;
    cfg->connector = NIC_CONNECTOR_MII;
    ravb->dev.dv_dll_hdl = attach_args->dll_hdl;
    ravb->is_mv_phy = -1;

    ravb->base = mmap_device_io(cfg->io_window_size[0],
                 cfg->io_window_base[0]);
    if (ravb->base == (uintptr_t)MAP_FAILED) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed to mmap device");
        ravb_cleanup(ravb);
        return rc;
    }

#if !defined(VARIANT_ipmmu)
    if (smmu_register_mmio_device(cfg->io_window_base[0], cfg->io_window_size[0]) != 0) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed mmio device smmu register, errno %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }
#else /* defined(VARIANT_ipmmu) */
    ravb->ipmmu_tree_root = NULL;
    ravb->ipmmu_node_count = 0;
    ravb->ipmmu_handle = ipmmu_open("ethernet-avb", cfg->device_index);
    if (ravb->ipmmu_handle < 0) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed to ipmmu_open, errno %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }
#endif /* !defined(VARIANT_ipmmu) */

    callout_init(&ravb->mii_callout);

    ravb->cachectl.fd = NOFD;
    if (cache_init(0, &ravb->cachectl, NULL) == -1) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed to init cache");
        ravb_cleanup(ravb);
        return rc;
    }

    /* Set AVB reset mode */
    ravb_reset(ravb);
    /* Set AVB config mode */
    ravb_config(ravb);

    /* Use typed mem for descriptors and packets since
     * ethernet DMA has 32-bit boundary */
    ravb->fd = posix_typed_mem_open(attach_args->drv_mem, O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
    if (ravb->fd == -1) {
        return ENOMEM;
    }

    rc = ravb_tx_desc_init(ravb);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: Failed to init TX Descriptors: %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }

    rc = ravb_rx_desc_init(ravb);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: Failed to init RX Descriptors: %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }

    rc = ravb_desc_bat_init(ravb);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: Failed to init BAT Descriptors: %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }

    ravb->sdhook = shutdownhook_establish(ravb_shutdown, ravb);

    ravb_emac_init(ravb);
    ravb_dmac_init(ravb);
    ravb_ptp_init(ravb);
    rc = ravb_phy_init(ravb);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: Failed to find PHY: %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }

    ravb->ient_rx.func = ravb_process_interrupt_rx;
    ravb->ient_rx.enable = ravb_dmac_enable_interrupt_rx;
    ravb->ient_rx.arg = ravb;

    if ((rc = interrupt_entry_init(&ravb->ient_rx, 0, NULL,
        cfg->priority)) != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: Failed to init the interrupt rx entry: %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }

    ravb->ient_tx.func = ravb_process_interrupt_tx;
    ravb->ient_tx.enable = ravb_dmac_enable_interrupt_tx;
    ravb->ient_tx.arg = ravb;

    if ((rc = interrupt_entry_init(&ravb->ient_tx, 0, NULL,
        cfg->priority)) != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: Failed to init the interrupt tx entry: %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }
    memset(&ravb->spinlock, 0, sizeof(ravb->spinlock));

    if (ravb->options.avb) {
        int queue;
        struct nw_stk_ctl  *sctlp = &stk_ctl;

        if ((ravb->chid = ChannelCreate(0)) == -1) {
            rc = errno;
            slogf (_SLOGC_NETWORK, _SLOG_ERROR, "%s: ChannelCreate failed %s", __func__, strerror (errno));
            ravb_cleanup (ravb);
            return rc;
        }
        if ((ravb->coid = ConnectAttach(ND_LOCAL_NODE, 0, ravb->chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
            rc = errno;
            slogf (_SLOGC_NETWORK, _SLOG_ERROR, "%s: ConnectAttach failed %s", __func__, strerror (errno));
            ravb_cleanup (ravb);
            return rc;
        }

        for (queue = 0; queue < NUM_RX_QUEUES; queue++)
        {
            /*
             * Create a pulse for each class of traffic. Start at the
             * default rx_prio but increment by 2 for each class to
             * allow a process to run at (class - 1) receiving and still
             * have priority over the lower class traffic without
             * impacting the dequeueing of packets from the limited Rx
             * descriptors.
             */
            SIGEV_PULSE_INIT(&ravb->isr_event[queue], ravb->coid, sctlp->rx_prio + (2 * queue),
                 RAVB_RX_PULSE, queue);
        }

        /* pseudo interrupt for Rx queue */
        ravb->inter_queue.func = ravb_process_queue;
        ravb->inter_queue.enable = ravb_enable_queue;
        ravb->inter_queue.arg = ravb;

        if ((rc = interrupt_entry_init(&ravb->inter_queue, 0, NULL, cfg->priority)) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s(): interrupt_entry_init(rx) failed: %d", __func__, rc);
            ravb_cleanup(ravb);
            return rc;
        }

        if ((rc = pthread_mutex_init(&ravb->rx_mutex, NULL)) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s(): rx_mutex init failed: %d", __func__, rc);
            ravb_cleanup(ravb);
            return rc;
        }

        IFQ_SET_MAXLEN(&ravb->rx_queue, IFQ_MAXLEN);
        nw_pthread_create(&ravb->tid, NULL, ravb_rx_thread, ravb, 0, ravb_rx_thread_init, ravb);
    }

    ravb_clear_stats(ravb);

    if ((rc = ravb_interrupt_init(ravb))) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Failed to attach to the interrupt: %d", rc);
        ravb_cleanup(ravb);
        return rc;
    }

    strcpy((char *)cfg->device_description, "Renesas AVB Network Device");

    ifp = &ravb->ecom.ec_if;
    snprintf(ravb->dev.dv_xname, sizeof(ravb->dev.dv_xname), "ravb%d", cfg->device_index);
    strcpy(ifp->if_xname, ravb->dev.dv_xname);
    ifp->if_softc = ravb;
    /* No multicast filtering in hardware so set IFF_ALLMULTI */
    ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST | IFF_ALLMULTI;
    ifp->if_ioctl = ravb_ioctl;
    ifp->if_start = ravb_start;
    ifp->if_init  = ravb_init;
    ifp->if_stop  = ravb_stop;
    IFQ_SET_READY(&ifp->if_snd);
    ravb->ecom.ec_capabilities |= ETHERCAP_VLAN_MTU;

    if (cfg->verbose) {
        nic_dump_config(cfg);
    }

    if_attach(ifp);
    ether_ifattach(ifp, cfg->current_address);

    if (ravb->options.avb) {
        /* Intercept if_output for pulling off AVB packets */
        ravb->stack_output = ravb->ecom.ec_if.if_output;
        ravb->ecom.ec_if.if_output = ravb_output;
    }

    ravb_dmac_enable_interrupt_tx(ravb);
    ravb_dmac_enable_interrupt_rx(ravb);

    return EOK;
}

static char *ravb_opts[] = {
    "flow",
#define OPT_FLOW     0
    "mii",
#define OPT_MII      1
    "miiLite",
#define OPT_MII_LITE 2
    "emu_phy",
#define OPT_EMU_PHY  3
    "slave",
#define OPT_SLAVE    4
    "avb",
#define OPT_AVB      5
    "m88q2110",
#define OPT_M88Q2110   6
    "deviceidx",
#define OPT_DEVICEIDX   7
    "portmax",
#define OPT_PORTMAX     8
    "txc_delay",
#define OPT_TXC_DELAY   9
    "rxc_delay",
#define OPT_RXC_DELAY   10
    "port_num",
#define OPT_PORT_NUM    11
    "drv_mem",
#define OPT_DRV_MEM     12
    NULL
};

static int ravb_parse_options (const char *optstring,
                 attach_args_t *attach_args)
{
    nic_config_t *cfg;
    char         *options, *freeptr, *value;
    int          opt;

    if (optstring == NULL) {
        return 0;
    }

    cfg = &attach_args->cfg;

#if _NTO_VERSION >= 700
    confstr(_CS_MACHINE, attach_args->soc_name, CS_MACHINE_LEN);
#endif

    /* getsubopt() is destructive */
    freeptr = options = strdup(optstring);
    if (!options) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "%s:%d - strdup failed", __FUNCTION__, __LINE__);
        return ENOMEM;
    }

    while (options && *options != '\0') {
        opt = getsubopt(&options, ravb_opts, &value);
        switch (opt) {
        case OPT_FLOW:
            attach_args->set_flow = atoi(value);
            if (attach_args->set_flow > 3) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                    "ravb: Unknown flow value %d, forcing off",
                     attach_args->set_flow);
                attach_args->set_flow = 0;
            }
            break;

        case OPT_MII:
            attach_args->mii = 1;
            break;

        case OPT_MII_LITE:
            attach_args->miiLite = 1;
            break;

        case OPT_EMU_PHY:
            attach_args->emu_phy = atoi(value);
            break;

        case OPT_SLAVE:
            attach_args->slave = 1;
            break;

        case OPT_AVB:
            attach_args->avb = 1;
            break;

        case OPT_DEVICEIDX:
            attach_args->cfg.device_index = strtoul(value, 0, 0);
            break;

        case OPT_M88Q2110:
            attach_args->m88q2110 = 1;
            break;

        case OPT_TXC_DELAY:
            attach_args->txc_delay = 1;
            break;

        case OPT_RXC_DELAY:
            attach_args->rxc_delay = 1;
            break;

        case OPT_PORTMAX:
            ravb_port_num = strtoul(value, 0, 0);
            if (ravb_port_num > AVB_CHAN_NUM)
                ravb_port_num = AVB_CHAN_NUM;
            break;
        case OPT_DRV_MEM:
            attach_args->drv_mem = strdup(value);
            break;

        default:
            if (nic_parse_options(cfg, value) != EOK) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "ravb: Unknown option %s", value);
                (free)(freeptr);
                return EINVAL;
            }
        }
    }
    (free)(freeptr);
    return EOK;
}

CFATTACH_DECL(ravb,
    sizeof(ravb_dev_t),
    NULL,
    ravb_attach,
    ravb_detach,
    NULL);

static void ravb_parse_syspage (nic_config_t *cfg, unsigned int deviceidx)
{
    hwi_tag     *tag;
    unsigned    item, ivec, ivec_idx=0;

    cfg->num_io_windows = 0;
    cfg->num_irqs = 0;

    /* Find network hardware information. */
    item = hwi_find_device("ravb", deviceidx);
    if (item == HWI_NULL_OFF) {
        slogf(_SLOGC_NETWORK, _SLOG_INFO,
            "ravb: device %d not found in syspage. "
            "will use default base addr and IRQ info", deviceidx);
        return;
    }

    /* Find base address */
    if ((tag = hwi_tag_find(item, HWI_TAG_NAME_location, NULL)) != NULL) {
        cfg->io_window_base[0] = (paddr_t)tag->location.base;
        cfg->num_io_windows = 1;
    } else {
        slogf(_SLOGC_NETWORK, _SLOG_INFO,
            "ravb: device %d: base addr not found in syspage. "
            "will use default.", deviceidx);
    }

    /* Find IRQ */
    while((ivec = hwitag_find_ivec(item, &ivec_idx)) != HWI_ILLEGAL_VECTOR) {
        cfg->irq[cfg->num_irqs++] = ivec;
    }
    if (cfg->num_irqs == 0) {
        slogf(_SLOGC_NETWORK, _SLOG_INFO,
            "ravb: device %d: IRQ info not found in syspage. "
            "will use default.", deviceidx);
    }

    /* Find MAC address */
    hwitag_find_nicaddr(item, NULL, cfg->current_address);
}

static int ravb_entry (void *dll_hdl,  struct _iopkt_self *iopkt, char *options)
{
    attach_args_t           *attach_args;
    nic_config_t            *cfg;
    struct device           *dev;
    int                     rc = 0, single, instance, port, done;
    struct ifnet            *ifp;
    struct drvcom_config    *dcon;

    /* Check if it is already mounted by doing a "nicinfo" on each interface */
    dcon = (malloc)(sizeof(*dcon));
    if (dcon == NULL) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "%s:%d - malloc failed", __FUNCTION__, __LINE__);
        return ENOMEM;
    }
    IFNET_FOREACH(ifp) {
        dcon->dcom_cmd.ifdc_cmd = DRVCOM_CONFIG;
        dcon->dcom_cmd.ifdc_len = sizeof(dcon->dcom_config);
        rc = ifp->if_ioctl(ifp, SIOCGDRVCOM, (caddr_t)dcon);
        if ((rc == EOK) && (dcon->dcom_config.num_io_windows > 0)) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "%s:%d - Driver already loaded for %s",
                  __FUNCTION__, __LINE__, ifp->if_xname);
            (free)(dcon);
            return EBUSY;
        }
    }
    (free)(dcon);

    attach_args = calloc(1,sizeof(*attach_args));
    if (!attach_args) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "%s:%d - calloc failed", __FUNCTION__, __LINE__);
        return ENOMEM;
    }
    attach_args->iopkt = iopkt;
    attach_args->dll_hdl = dll_hdl;

    /* Set defaults before parsing the command line */
    cfg = &attach_args->cfg;
    cfg->media_rate = -1;
    cfg->duplex = -1;
    cfg->device_index  = -1;
    attach_args->set_flow = -1;
    attach_args->emu_phy = -1;
    /*
     * Limitation for V4H 1.0: If using Tx clock internal Delay Mode at 1Gbps, need to change IO voltage to 1.8V
     */
    //attach_args->txc_delay = 1;
    cfg->mtu = ETHERMTU;
    cfg->mru = ETHERMTU;
    cfg->flags = NIC_FLAG_MULTICAST;
    cfg->mac_length = ETHER_ADDR_LEN;
    cfg->priority = stk_ctl.rx_prio;
    attach_args->drv_mem = "below4G";

    if (ravb_parse_options(options, attach_args) != EOK) {
        (free)(attach_args);
        return EINVAL;
    }

    /* If setting speed, must set duplex and vice versa */
    if (((cfg->media_rate != -1) && (cfg->duplex == -1)) ||
        ((cfg->media_rate == -1) && (cfg->duplex != -1))) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "ravb: Must set both speed and duplex if setting either");
        (free)(attach_args);
        return EINVAL;
    }
    /* If speed set then turn flow off if not set */
    if ((cfg->media_rate != -1) && (attach_args->set_flow == -1)) {
        attach_args->set_flow = 0;
    }

    port = (cfg->device_index  == -1) ? 0 : cfg->device_index;
    done = ravb_port_num;
    instance = 0;

    for (int i = 0; port < AVB_CHAN_NUM; port++,i++) {
        cfg->device_index = port;
        /*
         * If base address, IRQ, MAC address are not set from the command line,
         * set them from the syspage info and if not there then set to
         * default values.
         */
        ravb_parse_syspage(cfg, port);

        if (cfg->num_io_windows == 0) {
            cfg->io_window_base[0] = AVB_REG_BASE | (cfg->device_index << 16);            
            cfg->num_io_windows = 1;
        }

        cfg->io_window_size[0] = AVB_REG_SIZE;

        if ((ravb_port_num > 1) && (instance > 0)) {
            cfg->current_address[5] += 1;
        }

        if ((memcmp(cfg->current_address, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0) ||
            ETHER_IS_MULTICAST(cfg->current_address)) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "ravb: Invalid MAC address %02x%02x%02x%02x%02x%02x",
                  cfg->current_address[0], cfg->current_address[1],
                  cfg->current_address[2], cfg->current_address[3],
                  cfg->current_address[4], cfg->current_address[5]);
            continue;
        }

        memcpy(cfg->permanent_address, cfg->current_address, ETHER_ADDR_LEN);

        dev = NULL; /* No Parent */
        rc = dev_attach("ravb", options, &ravb_ca, attach_args, &single, &dev, NULL);
        if (rc == EOK) instance++;
        dev->dv_dll_hdl = dll_hdl;
        /* we're done */
        if (--done == 0) break;
    }

    (free)(attach_args);
    return rc;
}

struct _iopkt_drvr_entry IOPKT_DRVR_ENTRY_SYM(ravb) = IOPKT_DRVR_ENTRY_SYM_INIT(ravb_entry);

#ifdef VARIANT_a
#include <nw_dl.h>
/* This is what gets specified in the stack's dl.c */
struct nw_dll_syms ravb_syms[] = {
    {"iopkt_drvr_entry", &IOPKT_DRVR_ENTRY_SYM(ravb)},
    {NULL, NULL}
};
#endif
