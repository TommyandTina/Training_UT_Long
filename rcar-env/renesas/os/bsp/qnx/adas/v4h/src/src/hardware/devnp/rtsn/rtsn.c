/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

#include "rtsn.h"
#include <stdlib.h>
#include <fcntl.h>
#include <drvr/hwinfo.h>
#include <net/ifdrvcom.h>
#include <sys/sockio.h>
#include <netdrvr/smmu.h>
#include <net/if_vlanvar.h>
#include <netdrvr/avb.h>
#include <netinet/ip.h>
#include <bpfilter.h>

#if NBPFILTER > 0
#include <net/bpf.h>
#include <net/bpfdesc.h>
#endif

static void rtsnes_mhd_reset(rtsnes_dev_t *rtsnes)
{
    rtsnes_mhd_write(rtsnes, TSNMHD_SWR, TSNMHD_SWR_SWR);
    rtsnes_mhd_reg_wait(rtsnes, TSNMHD_SWR, TSNMHD_SWR_SWR, 0);
}


static int rtsnes_mhd_mode_change(rtsnes_dev_t *rtsnes, int mode)
{
    /* Change expected mode */
    rtsnes_mhd_write(rtsnes, TSNMHD_OCR, mode);

    return (rtsnes_mhd_reg_wait(rtsnes, TSNMHD_OSR, TSNMHD_OSR_OPS_MASK, 1 << mode));
}

int rtsnes_set_speed(rtsnes_dev_t *rtsnes, uint32_t speed)
{
    uint32_t        val;
    int             rc;

    if (rtsnes->phy_speed == speed) {
        return EOK;
    }

    /* Change MHD is in to CONFIG mode */
    rc = rtsnes_mhd_mode_change(rtsnes, TSNMHD_OCR_OPC_CONFIG);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: unable to change CONFIG mode to set speed");
        return rc;
    }

    val = rtsnes_rmac_read(rtsnes, TSNRMAC_MPIC) & ~TSNRMAC_MPIC_LSC_MASK;

    switch (speed) {
    case PHY_SPEED_1000M:
        val |= TSNRMAC_MPIC_LSC_1000M;
        break;
    case PHY_SPEED_100M:
         val |= TSNRMAC_MPIC_LSC_100M;
        break;
    case PHY_SPEED_10M:
        break;
    default:
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Don't support this speed(%d mbps)", speed/1000);
        return -ENOTSUP;
    }

    /* Set RMAC speed */
    rtsnes_rmac_write(rtsnes, TSNRMAC_MPIC, val);

    /* Change MHD is in to OPERATION mode */
    rc = rtsnes_mhd_mode_change(rtsnes, TSNMHD_OCR_OPC_DISABLE);
    if (rc == EOK) {
        rc = rtsnes_mhd_mode_change(rtsnes, TSNMHD_OCR_OPC_OPERATION);
    }

    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: unable to change back OPERATION mode in speed setting");
        return rc;
    }

    rtsnes->phy_speed = speed;

    return EOK;
}

static int rtsnes_check_link(rtsnes_dev_t *rtsnes)
{
    int rc;
    int timeout = 1000;

    /* Request Link Verification */
    rtsnes_rmac_write(rtsnes, TSNRMAC_MLVC, TSNRMAC_MLVC_PLV);

    for(; timeout; --timeout) {
        /* Complete Link Verification */
        if ((rc = rtsnes_rmac_reg_wait(rtsnes, TSNRMAC_MLVC, TSNRMAC_MLVC_PLV, 0)) == 0) {
            break;
        }
    }

    if (timeout == 0) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Link verification timeout!");
        return rc;
    }

    return EOK;
}

static void rtsnes_rmac_clock(rtsnes_dev_t *rtsnes, int enable)
{
    uint32_t val;

    val = rtsnes_rmac_read(rtsnes, TSNRMAC_MPIC);
    if (enable) {
        val &= ~(TSNRMAC_MPIC_PSMCS_MASK | TSNRMAC_MPIC_PSMHT_MASK);
        val |= TSNRMAC_MPIC_PSMCS(0x0A) | TSNRMAC_MPIC_PSMHT(0x07);
    } else {
        val &= ~(TSNRMAC_MPIC_PSMCS_MASK);
    }

    rtsnes_rmac_write(rtsnes, TSNRMAC_MPIC, val);
}

static int rtsnes_rmac_init(rtsnes_dev_t *rtsnes)
{
    nic_config_t    *cfg = &rtsnes->cfg;
    unsigned char   *mac = cfg->current_address;
    int             rc = EOK;
    uint32_t        val;

    /* Set MAC address */
    rtsnes_rmac_write(rtsnes, TSNRMAC_MRMAC1, mac[5] | (mac[4] << 8) | (mac[3] << 16)  | (mac[2] << 24));
    rtsnes_rmac_write(rtsnes, TSNRMAC_MRMAC0, mac[1] | (mac[0] << 8));

    val = rtsnes_rmac_read(rtsnes, TSNRMAC_MPIC);
    val &= ~(TSNRMAC_MPIC_PIS_MASK | TSNRMAC_MPIC_LSC_MASK);

    /* Set MIIx */
    switch (rtsnes->mii) {
    case TSNES_MII:
        val |= TSNRMAC_MPIC_PIS_MII;
        break;
    case TSNES_RMII:
        val |= TSNRMAC_MPIC_PIS_RMII;
        break;
    case TSNES_GMII:
    case TSNES_RGMII:
        /*  R-Car V4H v1.0 does not support RMAC/RGMII,
         *  but it has internal GMII-RGMII converter. */
        val |= TSNRMAC_MPIC_PIS_GMII;
        break;
    default:
        return EINVAL;
    }

    /* Set Speed */
    switch (rtsnes->phy_speed) {
    case PHY_SPEED_10M:
        val |= TSNRMAC_MPIC_LSC_10M;
        break;
    case PHY_SPEED_100M:
        val |= TSNRMAC_MPIC_LSC_100M;
        break;
    case PHY_SPEED_1000M:
        val |= TSNRMAC_MPIC_LSC_1000M;
        break;
    default:
        return EINVAL;
    }

    rtsnes_rmac_write(rtsnes, TSNRMAC_MPIC, val);

    /* Enable MII */
    rtsnes_rmac_clock(rtsnes, 1);

    /* Check link */
    rc = rtsnes_check_link(rtsnes);

    if (rtsnes->internal_loopback)
        rtsnes_rmac_write(rtsnes, TSNRMAC_MLBC, 1);

    return rc;
}

static int rtsnes_axibmi_init(rtsnes_dev_t *rtsnes)
{
    int rc;

    /* Reset RAM */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RR, TSNAXIBMI_RR_RATRR | TSNAXIBMI_RR_TATRR);
    nanospin_ns(100);
    /* Wait for reset complete */
    if ((rc = rtsnes_axibmi_reg_wait(rtsnes, TSNAXIBMI_RR, TSNAXIBMI_RR_RATRR | TSNAXIBMI_RR_TATRR, 
            TSNAXIBMI_RR_RATRR | TSNAXIBMI_RR_TATRR)))
        return rc;

    /* Set AXIWC */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_AXIWC, TSNAXIBMI_AXIWC_DEFAULT);

    /* Set AXIRC */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_AXIRC, TSNAXIBMI_AXIRC_DEFAULT);

    /* TX Descritor chain setting */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TATLS0, (TX_QUEUE_OFFSET << 24) | TSNAXIBMI_TATLS0_TEDE); // extended descriptor
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TATLS1, rtsnes->tx_bat_desc_addr + TX_QUEUE_ADDR_OFFSET); // TX Descriptor Chain Current Address
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TATLR, TSNAXIBMI_TATLR_TATL);
    if ((rc = rtsnes_axibmi_reg_wait(rtsnes, TSNAXIBMI_TATLR, TSNAXIBMI_TATLR_TATL, 0)))
        return rc;

    /* RX Descriptor chain setting */
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RATLS0, (RX_QUEUE_OFFSET << 24) | TSNAXIBMI_RATLS0_REDE | TSNAXIBMI_RATLS0_RETS); // extended desc, Timestamp
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RATLS1, rtsnes->rx_bat_desc_addr + RX_QUEUE_ADDR_OFFSET); // RX Descriptor Chain Current Address        
    rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RATLR, TSNAXIBMI_RATLR_RATL);
    if ((rc = rtsnes_axibmi_reg_wait(rtsnes, TSNAXIBMI_RATLR, TSNAXIBMI_RATLR_RATL, 0)))
        return rc;

    //rtsnes_axibmi_write(rtsnes, TSNAXIBMI_DIE, DIE_DID_TDICX(0) | DIE_DID_RDICX(0)); 
    //rtsnes_axibmi_write(rtsnes, TSNAXIBMI_TDIE(0), TDIE_TDID_TDX(0)); 
    //rtsnes_axibmi_write(rtsnes, TSNAXIBMI_RDIE(0), RDIE_RDID_RDX(0));

    return EOK;
}

static int rtsnes_mhd_init(rtsnes_dev_t *rtsnes)
{
    /* TX General setting */
	rtsnes_mhd_write(rtsnes, TSNMHD_TGC1, TSNMHD_TGC1_STTV_DEFAULT | TSNMHD_TGC1_TQTM_SFM);
	rtsnes_mhd_write(rtsnes, TSNMHD_TMS0, TSNMHD_TMS_MFS_MAX);

	/* RX Filter IP */
	rtsnes_rms_write(rtsnes, TSNRMS_CFCR0, TSNRMS_CFCR_SDID(RX_QUEUE_OFFSET));
	rtsnes_rms_write(rtsnes, TSNRMS_FMSCR, TSNRMS_FMSCR_FMSIE(RX_QUEUE_OFFSET));

	/* TX General setting */   
    //rtsnes_mhd_write(rtsnes, TSNMHD_TGC1, TSNMHD_TGC1_TQTM_SFM);
    //rtsnes_mhd_write(rtsnes, TSNMHD_TGC2, 0xF0);

    return EOK;
}

static void rtsnes_set_delay_mode(rtsnes_dev_t *rtsnes)
{
	uint32_t val;

    /* Limitation for V4H 1.0: If using Tx clock internal Delay Mode, need to change IO voltage to 1.8V at 1Gbps (can change in startup) */
	if (rtsnes->tx_internal_delay)
		out32(rtsnes->base + TSN_GPOUT, TSN_GPOUT_TDM);

	if (rtsnes->rx_internal_delay) {
		val = in32(rtsnes->base + TSN_GPOUT);
		out32(rtsnes->base + TSN_GPOUT, val | TSN_GPOUT_RDM);
	}
}

static int rtsnes_hw_init (rtsnes_dev_t *rtsnes)
{
    int rc;

    /* Change MHD to DISABLE mode */
    rc = rtsnes_mhd_mode_change(rtsnes, TSNMHD_OCR_OPC_DISABLE);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: unable to change DISABLE mode: %d", rc);
    }

    rtsnes_mhd_reset(rtsnes);

    /* Change MHD to CONFIG mode */
    rc = rtsnes_mhd_mode_change(rtsnes, TSNMHD_OCR_OPC_CONFIG);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: unable to change CONFIG mode: %d", rc);
        return rc;
    }

    rc = rtsnes_axibmi_init(rtsnes);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: rtsnes_axibmi_init failed: %d", rc);
        return rc;
    }

    rc = rtsnes_mhd_init(rtsnes);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: rtsnes_mhd_init failed: %d", rc);
        return rc;
    }

    rc = rtsnes_rmac_init(rtsnes);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: rtsnes_rmac_init failed: %d", rc);
        return rc;
    }

    /* Change MHD is in to Disable mode*/
    rc = rtsnes_mhd_mode_change(rtsnes, TSNMHD_OCR_OPC_DISABLE);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: unable to change DISABLE mode: %d", rc);
        return rc;
    }

    rtsnes_set_delay_mode(rtsnes);

    /* Change MHD is in to Operation mode */
    rc = rtsnes_mhd_mode_change(rtsnes, TSNMHD_OCR_OPC_OPERATION);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: unable to change OPERATION mode: %d", rc);
        return rc;
    }

    return EOK;
}

static int rtsnes_bat_desc_init(rtsnes_dev_t *rtsnes)
{
    size_t          size;
    uint32_t        i;
    RTSNES_DESC     *desc;

    /* Allocate descriptor base address table. They should be aligned */
    /* to size of struct rtsnes_desc. */

    /* TX */
    size = sizeof(RTSNES_DESC) * NUM_TX_QUEUES;
    rtsnes->tx_bat_desc =  mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, rtsnes->fd, 0);
    if (rtsnes->tx_bat_desc == MAP_FAILED) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed bat_desc map, "
              "errno %d", errno);
        return errno;
    }
    if (mem_offset64((void *)rtsnes->tx_bat_desc, NOFD, 1, &rtsnes->tx_bat_desc_addr, 0) == -1) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed bat_desc mem_offset64, "
              "errno %d", errno);
        return errno;
    }
    if (smmu_map_driver_memory((_Uint64t)rtsnes->tx_bat_desc_addr, size) != 0) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed bat_desc smmu_map, "
              "errno %d", errno);
        return errno;
    }

    /* Initialize all descriptors */
    memset((void *)rtsnes->tx_bat_desc, 0x0, size);

    for (i = 0; i < NUM_TX_QUEUES; i++) {
        rtsnes->tx_bat_desc[i].die_dt = DT_EOS;
    }

    /* TX descriptor base address for all queues */
    for(i = 0; i < NUM_TX_QUEUES; i++) {
        desc = &rtsnes->tx_bat_desc[i];
        desc->die_dt = DT_LINK;
        desc->dptr = rtsnes->tx_bd[i][NUM_TX_DESC].dptr;
    }

    CACHE_FLUSH(&rtsnes->cachectl, (void *)rtsnes->tx_bat_desc, rtsnes->tx_bat_desc_addr, size);

    /* RX */
    size = sizeof(RTSNES_DESC) * NUM_RX_QUEUES;
    rtsnes->rx_bat_desc =  mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, rtsnes->fd, 0);
    if (rtsnes->rx_bat_desc == MAP_FAILED) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed bat_desc map, "
              "errno %d", errno);
        return errno;
    }
    if (mem_offset64((void *)rtsnes->rx_bat_desc, NOFD, 1, &rtsnes->rx_bat_desc_addr, 0) == -1) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed bat_desc mem_offset64, "
              "errno %d", errno);
        return errno;
    }
    if (smmu_map_driver_memory((_Uint64t)rtsnes->rx_bat_desc_addr, size) != 0) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed bat_desc smmu_map, "
              "errno %d", errno);
        return errno;
    }

    /* Initialize all descriptors */
    memset((void *)rtsnes->rx_bat_desc, 0x0, size);

    for (i = 0; i < NUM_RX_QUEUES; i++) {
        rtsnes->rx_bat_desc[i].die_dt = DT_EOS;
    }

    /* RX descriptor base address for all queues */
    for(i = 0; i < NUM_RX_QUEUES; i++) {
        desc = &rtsnes->rx_bat_desc[i];
        desc->die_dt = DT_LINK; /* type */
        desc->dptr = rtsnes->rx_bd[i][NUM_RX_DESC].dptr;
    }

    CACHE_FLUSH(&rtsnes->cachectl, (void *)rtsnes->rx_bat_desc, rtsnes->rx_bat_desc_addr, size);

    return EOK;
}

static int rtsnes_tx_desc_init(rtsnes_dev_t *rtsnes)
{
    size_t               size;
    off64_t              offset;
    uint32_t             i, queue;
    RTSNES_TX_DESC      *tx_desc;

    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        /* mbuf pointer array, corresponding to tx descriptor ring */
        size = sizeof(struct mbuf *) * NUM_TX_DESC;
        rtsnes->tx_pkts[queue] = malloc(size, M_DEVBUF, M_NOWAIT);
        if (rtsnes->tx_pkts[queue] == NULL) {
            return errno;
        }
        /* No packets in tx ring to start */
        memset(rtsnes->tx_pkts[queue], 0x00, size);

        /* Allocate all TX descriptors. */
        size = sizeof(RTSNES_TX_DESC) * (NUM_TX_DESC_WITH_LINK);
        rtsnes->tx_bd[queue] = mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, rtsnes->fd, 0);
        if (rtsnes->tx_bd[queue] == MAP_FAILED) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed tx_bd mmap, queue %d "
                  "errno %d", queue, errno);
            return errno;
        }
        if (mem_offset64((void *)rtsnes->tx_bd[queue], NOFD, 1, &offset, 0) == -1) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed tx_bd mem_offset64, queue %d "
                  "errno %d", queue, errno);
            return errno;
        }
        if (smmu_map_driver_memory((_Uint64t)offset, size) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed tx_bd smmu map, queue %d "
                  "errno %d", queue, errno);
            return errno;
        }

        memset((void *)rtsnes->tx_bd[queue], 0x00, size);
        /* Build TX ring buffer */
        for (i = 0; i < NUM_TX_DESC; i++) {
            tx_desc = &rtsnes->tx_bd[queue][i];
            tx_desc->die_dt = DT_FEMPTY;
        }
        tx_desc = &rtsnes->tx_bd[queue][NUM_TX_DESC];
        tx_desc->dptr = (uint32_t)offset;
        tx_desc->die_dt = DT_LINK;

        CACHE_FLUSH(&rtsnes->cachectl, (void *)rtsnes->tx_bd[queue], offset, size);

        rtsnes->tx_irq |= BIT(queue);
    }

    return EOK;
}

static int rtsnes_rx_desc_init(rtsnes_dev_t *rtsnes)
{
    size_t               size;
    off64_t              offset;
    uint32_t             i, queue;
    struct mbuf          *m;
    RTNSES_RX_EXT_DESC   *rx_desc;

    for (queue = 0; queue < NUM_RX_QUEUES; queue++) {
        /* mbuf pointer array, corresponding to rx descriptor ring */
        size = sizeof(struct mbuf *) * NUM_RX_DESC;
        rtsnes->rx_pkts[queue] = malloc(size, M_DEVBUF, M_NOWAIT);
        if (rtsnes->rx_pkts[queue] == NULL) {
            return errno;
        }
        memset(rtsnes->rx_pkts[queue], 0, size);

        /* Allocate all RX descriptors. */
        size = sizeof(RTNSES_RX_EXT_DESC) * (NUM_RX_DESC_WITH_LINK);
        rtsnes->rx_bd[queue] = mmap (NULL, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, rtsnes->fd, 0);
        if (rtsnes->rx_bd[queue] == MAP_FAILED) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed rx_bd mmap, queue %d "
                  "errno %d", queue, errno);
            return errno;
        }
        if (mem_offset64((void *)rtsnes->rx_bd[queue], NOFD, 1, &offset, 0) == -1) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed rx_bd mem_offset64, queue %d "
                  "errno %d", queue, errno);
            return errno;
        }
        if (smmu_map_driver_memory((_Uint64t)offset, size) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed rx_bd smmu map, queue %d "
                  "errno %d", queue, errno);
            return errno;
        }

        memset((void *)rtsnes->rx_bd[queue], 0, size);

        /* Build RX ring buffer */
        for (i = 0; i < NUM_RX_DESC; i++) {
            m = m_getcl(M_NOWAIT, MT_DATA, M_PKTHDR);
            if (m == NULL) {
                return ENOMEM;
            }
            rtsnes->rx_pkts[queue][i] = m;
            /* RX descriptor */
            rx_desc = &rtsnes->rx_bd[queue][i];

            /* The size of the buffer should be on 4-byte boundary. */
            rx_desc->info_ds = ALIGN(PKT_BUF_SZ, DPTR_ALIGN);

            rx_desc->dptr = (uint32_t)mbuf_phys(m);
            rx_desc->die_dt =  DT_FEMPTY | DIE;

            CACHE_FLUSH(&rtsnes->cachectl, m->m_data, ((off64_t)rx_desc->dptr), m->m_ext.ext_size);
        }
        rx_desc = &rtsnes->rx_bd[queue][NUM_RX_DESC];
        rx_desc->dptr = (uint32_t)offset;
        rx_desc->die_dt = DT_LINK ; /* type */

        CACHE_FLUSH(&rtsnes->cachectl, (void *)rtsnes->rx_bd[queue], offset, size);

        rtsnes->rx_irq |= BIT(queue);
    }

    return EOK;
}

static void rtsnes_shutdown (void *arg)
{
    rtsnes_rmac_clock(arg, 0);
    rtsnes_mhd_reset(arg);
}

static void rtsnes_cleanup (rtsnes_dev_t *rtsnes)
{
    nic_config_t *cfg = &rtsnes->cfg;
    struct mbuf  *m;
    uint32_t     i, queue;
    off64_t      offset;

    /*Detach all interrupts, when attached all interrupts */

    for (i = 0; i < cfg->num_irqs; i++) {
        InterruptDetach(rtsnes->iid[i]);
    }

#if 0
    InterruptDetach(rtsnes->rmac_iid);
#endif

    /* Checks and returns EINVAL if not in the interrupt_entry list */
    interrupt_entry_remove(&rtsnes->ient_rx, NULL);
    interrupt_entry_remove(&rtsnes->ient_tx, NULL);

    shutdownhook_disestablish(rtsnes->sdhook);

    /* Cleanup Tx allocations */
    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        if (rtsnes->tx_pkts[queue]) {
            for (i = 0; i < NUM_TX_DESC; i++) {
                m = rtsnes->tx_pkts[queue][i];
                if (m != NULL) {
                    m_freem(m);
                }
            }
            free(rtsnes->tx_pkts[queue], M_DEVBUF);
        }
        if (rtsnes->tx_bd[queue]) {
            offset = (off64_t)rtsnes->tx_bd[queue][NUM_TX_DESC].dptr;
            if (smmu_unmap_driver_memory((_Uint64t)offset,
                                         sizeof(RTSNES_TX_DESC) *
                                         NUM_TX_DESC_WITH_LINK) != 0) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed unmap tx_bd, "
                      "queue %d, errno %d", queue, errno);
            }

            munmap((void *)rtsnes->tx_bd[queue], sizeof(RTSNES_TX_DESC) * NUM_TX_DESC_WITH_LINK);
            rtsnes->tx_bd[queue] = NULL;
        }
    }

    /* Cleanup Rx allocations */
    for (queue = 0; queue < NUM_RX_QUEUES; queue++) {
        if (rtsnes->rx_pkts[queue]) {
            for (i = 0; i < NUM_RX_DESC; i++) {
                m = rtsnes->rx_pkts[queue][i];
                if (m != NULL) {
                    m_freem(m);
                }
            }
            free(rtsnes->rx_pkts[queue], M_DEVBUF);
        }
        if (rtsnes->rx_bd[queue]) {
            offset = (off64_t)rtsnes->rx_bd[queue][NUM_RX_DESC].dptr;
            if (smmu_unmap_driver_memory((_Uint64t)offset,
                                         sizeof(RTNSES_RX_EXT_DESC) *
                                         NUM_RX_DESC_WITH_LINK) != 0) {
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed unmap rx_bd, "
                      "queue %d, errno %d", queue, errno);
            }

            munmap((void *)rtsnes->rx_bd[queue], sizeof(RTNSES_RX_EXT_DESC) * NUM_RX_DESC_WITH_LINK);
            rtsnes->rx_bd[queue] = NULL;
        }
    }

    if (rtsnes->base) {
        if (smmu_unregister_mmio_device(rtsnes->cfg.io_window_base[0],
            rtsnes->cfg.io_window_size[0]) != 0) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed unregister mmio device, "
                 "errno %d", errno);
        }

        munmap_device_io(rtsnes->base, rtsnes->cfg.io_window_size[0]);
        rtsnes->base = (uintptr_t)MAP_FAILED;
    }

    if (rtsnes->tx_bat_desc) {
        offset = rtsnes->tx_bat_desc_addr;
        if (smmu_unmap_driver_memory((_Uint64t)offset,
                                     sizeof(RTSNES_DESC) *
                                     NUM_TX_QUEUES) != 0) {
               slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed unmap tx_bat_desc, "
                     "errno %d", errno);
        }

        munmap((void *)rtsnes->tx_bat_desc, sizeof(RTSNES_DESC) * NUM_TX_QUEUES);
        rtsnes->tx_bat_desc = NULL;
    }

    if (rtsnes->rx_bat_desc) {
        offset = rtsnes->rx_bat_desc_addr;
        if (smmu_unmap_driver_memory((_Uint64t)offset,
                                     sizeof(RTSNES_DESC) *
                                     NUM_TX_QUEUES) != 0) {
               slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: Failed unmap rx_bat_desc, "
                     "errno %d", errno);
        }

        munmap((void *)rtsnes->rx_bat_desc, sizeof(RTSNES_DESC) * NUM_RX_QUEUES);
        rtsnes->rx_bat_desc = NULL;
    }

    if (rtsnes->avb) {
        nw_pthread_reap (rtsnes->tid);
    }

    if (rtsnes->fd) {
        close(rtsnes->fd);
    }

    cache_fini(&rtsnes->cachectl);
}

void rtsnes_hw_stop (rtsnes_dev_t *rtsnes)
{
    struct mbuf *m;
    uint32_t    i, queue;

    rtsnes_axibmi_enable_interrupt(rtsnes, 1, 0);
    rtsnes_axibmi_enable_interrupt(rtsnes, 0, 0);
    rtsnes_rmac_clock(rtsnes, 0);

    for (queue = 0; queue < NUM_TX_QUEUES; queue++) {
        /* Clear any pending Tx buffers */
        for (i = 0; i < NUM_TX_DESC; i++) {
            m = rtsnes->tx_pkts[queue][i];
            if (m != NULL) {
                m_freem(m);
            }
            rtsnes->tx_bd[queue][i].die_dt = DT_EEMPTY;
        }
    }
}

static void rtsnes_stop (struct ifnet *ifp, int disable)
{
    rtsnes_dev_t          *rtsnes;
    nic_config_t            *cfg;
    struct nw_work_thread   *wtp;

    rtsnes = ifp->if_softc;
    cfg = &rtsnes->cfg;
    wtp = WTP;

    NW_SIGLOCK_P(&ifp->if_snd_ex, rtsnes->iopkt, wtp);
    callout_stop(&rtsnes->mii_callout);
    MDI_DisableMonitor(rtsnes->mdi);
    cfg->flags |= NIC_FLAG_LINK_DOWN;
    if_link_state_change(ifp, LINK_STATE_DOWN);
    ifp->if_flags &= ~(IFF_RUNNING | IFF_OACTIVE);
    NW_SIGUNLOCK_P(&ifp->if_snd_ex, rtsnes->iopkt, wtp);

    rtsnes_hw_stop(rtsnes);

    MDI_PowerdownPhy(rtsnes->mdi, cfg->phy_addr);
    rtsnes_clear_stats(rtsnes);
}

static int rtsnes_init (struct ifnet *ifp)
{
    rtsnes_dev_t            *rtsnes;
    nic_config_t            *cfg;
    struct nw_work_thread   *wtp;
    uint32_t                mac_change;

    rtsnes = ifp->if_softc;
    cfg = &rtsnes->cfg;
    wtp = WTP;

    /* check if configuration is required */
    mac_change = memcmp(cfg->current_address, LLADDR(ifp->if_sadl), ifp->if_addrlen);
    if (mac_change) {
        memcpy(cfg->current_address, LLADDR(ifp->if_sadl), ifp->if_addrlen);
        {
            unsigned char * mac = cfg->current_address;
            rtsnes_rmac_write(rtsnes, TSNRMAC_MRMAC1, mac[5] | (mac[4] << 8) | (mac[3] << 16) | (mac[2] << 24));
            rtsnes_rmac_write(rtsnes, TSNRMAC_MRMAC0, mac[1] | (mac[0] << 8));
        }
        if (cfg->verbose) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: MAC address updated");
        }
    }

    if ((ifp->if_flags & IFF_RUNNING) == 0) {
        NW_SIGLOCK_P(&ifp->if_snd_ex, rtsnes->iopkt, wtp);
        ifp->if_flags |= IFF_RUNNING;
        NW_SIGUNLOCK_P(&ifp->if_snd_ex, rtsnes->iopkt, wtp);
        rtsnes_MDI_MonitorPhy(rtsnes);
    }

    return EOK;
}

static void *rtsnes_rx_thread (void *arg)
{
    rtsnes_dev_t  *rtsnes = arg;
    int             rc, queue, mask;
    struct _pulse   pulse;
    int             idx, i;

    while (1) {
        rc = MsgReceivePulse(rtsnes->chid, &pulse, sizeof(pulse), NULL);
        if (rc == EOK) {
            switch (pulse.code) {
                case RTSNES_RX_PULSE:
                    queue = pulse.value.sival_int;
                    for (i = 0; i < NUM_RX_QUEUES; i++) {
                        idx = i + RX_QUEUE_OFFSET;
                        mask = rtsnes->tx_dis & rtsnes->rx_irq;
                        if ((queue == i) && (mask & (BIT(idx)))) {
                            rtsnes_receive(rtsnes, WTP, i); // For queue i
                        }
                    }
                    break;
                case RTSNES_QUIESCE_PULSE:
                    quiesce_block(pulse.value.sival_int);
                    break;
                default:
                    slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes Rx: Unknown pulse %d received", pulse.code);
                    break;
            }
        } else {
          slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes Rx: MsgReceivePulse: %s", strerror(rc));
        }
    }

    return NULL;
}

static void rtsnes_rx_thread_quiesce (void *arg, int die)
{
    rtsnes_dev_t          *rtsnes = arg;

    MsgSendPulse(rtsnes->coid, SIGEV_PULSE_PRIO_INHERIT, RTSNES_QUIESCE_PULSE, die);

    return;
}

static int rtsnes_rx_thread_init (void *arg)
{
    rtsnes_dev_t          *rtsnes = arg;
    struct nw_work_thread   *wtp = WTP;

    pthread_setname_np(gettid(), RTSNES_RX_THREAD_NAME);

    wtp->quiesce_callout = rtsnes_rx_thread_quiesce;
    wtp->quiesce_arg = rtsnes;

    return EOK;
}

int rtsnes_interrupt_attach (rtsnes_dev_t *rtsnes)
{
    nic_config_t   *cfg = &rtsnes->cfg;
    int            i, rc;

    if (rtsnes->avb) {
        /* When test code on V4H board, will test this one */
        for (i = 0; i < cfg->num_irqs; i++) {
            rc = InterruptAttach_r(cfg->irq[i], rtsnes_axibmi_isr_avb, rtsnes, sizeof(rtsnes),
                _NTO_INTR_FLAGS_TRK_MSK);
            if (rc < 0) {
                rc = -rc;
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                    "rtsnes: Failed to attach to the interrupt: %d", rc);
                rtsnes_cleanup(rtsnes);
                return rc;
            }
            rtsnes->iid[i] = rc;
        }
    } else {
#if 0
        for (i = 0; i < cfg->num_irqs; i++) {
            rc = InterruptAttachArray_r(cfg->irq[i], rtsnes_axibmi_isr, rtsnes, sizeof(rtsnes),
                _NTO_INTR_FLAGS_TRK_MSK);
            if (rc < 0) {
                rc = -rc;
                slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                    "rtsnes: Failed to attach to the interrupt: %d", rc);
                rtsnes_cleanup(rtsnes);
                return rc;
            }
            rtsnes->iid[i] = rc;
        }
#else
        rc = InterruptAttach_r(cfg->irq[0], rtsnes_axibmi_tx_isr, rtsnes, sizeof(rtsnes),
            _NTO_INTR_FLAGS_TRK_MSK);
        if (rc < 0) {
            rc = -rc;
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "rtsnes: Failed to attach to tx interrupt: %d", rc);
            rtsnes_cleanup(rtsnes);
            return rc;
        }
        rtsnes->iid[0] = rc;

        rc = InterruptAttach_r(cfg->irq[1], rtsnes_axibmi_rx_isr, rtsnes, sizeof(rtsnes),
            _NTO_INTR_FLAGS_TRK_MSK);
        if (rc < 0) {
            rc = -rc;
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                "rtsnes: Failed to attach to rx interrupt: %d", rc);
            rtsnes_cleanup(rtsnes);
            return rc;
        }
        rtsnes->iid[1] = rc;
#endif
    }

    return EOK;
}

int rtsnes_detach (struct device *dev, int flags)
{
    rtsnes_dev_t      *rtsnes;
    struct ifnet        *ifp;

    rtsnes = (rtsnes_dev_t *)dev;
    ifp = &rtsnes->ecom.ec_if;

    callout_stop(&rtsnes->mii_callout);
    MDI_PowerdownPhy(rtsnes->mdi, rtsnes->cfg.phy_addr);

    ether_ifdetach(ifp);
    if_detach(ifp);
    rtsnes_phy_fini(rtsnes);

    rtsnes_cleanup(rtsnes);

    return EOK;
}

int rtsnes_attach (struct device *parent, struct device *self, void *aux)
{
    rtsnes_dev_t    *rtsnes;
    attach_args_t   *attach_args;
    struct  ifnet   *ifp;
    nic_config_t    *cfg;
    int             rc;

    rtsnes = (rtsnes_dev_t*)self;
    attach_args = aux;

     /* Copy the config across */
    cfg = &rtsnes->cfg;
    memcpy(cfg, &attach_args->cfg, sizeof(*cfg));

    rtsnes->drv_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    rtsnes->ptp_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    rtsnes->mdi_mutex = NIC_MUTEX_UNLOCKED_VALUE;
    rtsnes->set_flow = attach_args->set_flow;
    rtsnes->avb = attach_args->avb;
    rtsnes->mii = attach_args->mii;
    rtsnes->emu_phy = attach_args->emu_phy;
    rtsnes->internal_loopback =  attach_args->internal_loopback;
    rtsnes->tx_internal_delay =  attach_args->tx_internal_delay;
    rtsnes->rx_internal_delay =  attach_args->rx_internal_delay;
    cfg->connector = NIC_CONNECTOR_MII;
    rtsnes->iopkt = attach_args->iopkt;
    rtsnes->dev.dv_dll_hdl = attach_args->dll_hdl;
    rtsnes->is_mv_phy = -1;

    callout_init(&rtsnes->mii_callout);
    rtsnes->cachectl.fd = NOFD;
    if (cache_init(0, &rtsnes->cachectl, NULL) == -1) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
          "rtsnes: Failed to init cache");
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    rtsnes->base = mmap_device_io(cfg->io_window_size[0], cfg->io_window_base[0]);
    if (rtsnes->base == (uintptr_t)MAP_FAILED) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: mmap device failed");
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    if (smmu_register_mmio_device(cfg->io_window_base[0], cfg->io_window_size[0]) != 0) {
        rc = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: mmio device smmu register failed, errno %d", rc);
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    pthread_mutex_init(&rtsnes->phy_mutex, NULL);

    switch(cfg->media_rate) {
        case PHY_SPEED_10M:
        case PHY_SPEED_100M:
            rtsnes->phy_speed = cfg->media_rate;
            break;
        default:
            rtsnes->phy_speed = PHY_SPEED_1000M;
            break;
    }

    /* Use typed mem for descriptors and packets since
     * ethernet DMA has 32-bit boundary */
    rtsnes->fd = posix_typed_mem_open( "below4G", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
    if (rtsnes->fd == -1) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
        "rtsnes: get below4G memory for DMA transfer failed");
        rtsnes_cleanup(rtsnes);
        return -ENOMEM;
    }

    rc = rtsnes_tx_desc_init(rtsnes);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: init TX Descriptors failed: %d", rc);
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    rc = rtsnes_rx_desc_init(rtsnes);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: init RX Descriptors failed: %d", rc);
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    rc = rtsnes_bat_desc_init(rtsnes);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: init BAT Descriptors failed: %d", rc);
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    rtsnes->sdhook = shutdownhook_establish(rtsnes_shutdown, rtsnes);

    /* HW init */
    rc = rtsnes_hw_init(rtsnes);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: rtsnes_hw_init failed: %d", rc);
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    /* Initialize gPTP */
    //if (rtsnes_ptp_init(rtsnes)) {
    //    slogf(_SLOGC_NETWORK, _SLOG_ERROR,
    //          "rtsnes: rtsnes_ptp_init failed: %d", rc);
    //    rtsnes_cleanup(rtsnes);
    //    return rc;
    //}

    /* Initialize PHY */
    rc = rtsnes_phy_init(rtsnes);
    if (rc != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: rtsnes_phy_init failed: %d", rc);
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    rtsnes->ient_rx.func = rtsnes_axibmi_process_interrupt_rx;
    rtsnes->ient_rx.enable = rtsnes_axibmi_enable_interrupt_rx;
    rtsnes->ient_rx.arg = rtsnes;

    if ((rc = interrupt_entry_init(&rtsnes->ient_rx, 0, NULL,
        cfg->priority)) != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: init the interrupt rx entry failed: %d", rc);
        rtsnes_cleanup(rtsnes);
        return rc;
    }

    rtsnes->ient_tx.func = rtsnes_axibmi_process_interrupt_tx;
    rtsnes->ient_tx.enable = rtsnes_axibmi_enable_interrupt_tx;
    rtsnes->ient_tx.arg = rtsnes;

    if ((rc = interrupt_entry_init(&rtsnes->ient_tx, 0, NULL,
        cfg->priority)) != EOK) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: init the interrupt tx entry failed: %d", rc);
        rtsnes_cleanup(rtsnes);
        return rc;
    }
    memset(&rtsnes->spinlock, 0, sizeof(rtsnes->spinlock));

    /* Attach interrupts handler to an interrupt source */
    rtsnes_interrupt_attach(rtsnes);

    if (rtsnes->avb) {
        struct nw_stk_ctl *sctlp = &stk_ctl;

        if ((rtsnes->chid = ChannelCreate(0)) == -1) {
            rc = errno;
            slogf (_SLOGC_NETWORK, _SLOG_ERROR, "%s: ChannelCreate failed %s", __func__, strerror (errno));
            rtsnes_cleanup (rtsnes);
            return rc;
        }

        if ((rtsnes->coid = ConnectAttach(ND_LOCAL_NODE, 0, rtsnes->chid, _NTO_SIDE_CHANNEL, 0)) == -1) {
            rc = errno;
            slogf (_SLOGC_NETWORK, _SLOG_ERROR, "%s: ConnectAttach failed %s", __func__, strerror (errno));
            rtsnes_cleanup (rtsnes);
            return rc;
        }

        for (int queue = 0; queue < NUM_RX_QUEUES; queue++) {
            /*
             * Create a pulse for each class of traffic. Start at the
             * default rx_prio but increment by 2 for each class to
             * allow a process to run at (class - 1) receiving and still
             * have priority over the lower class traffic without
             * impacting the dequeueing of packets from the limited Rx
             * descriptors.
             */

            SIGEV_PULSE_INIT(&rtsnes->isr_event[queue], rtsnes->coid, sctlp->rx_prio + (2 * queue),
                 RTSNES_RX_PULSE, queue);
        }

        /* pseudo interrupt for Rx queue */
        rtsnes->inter_queue.func = rtsnes_process_queue;
        rtsnes->inter_queue.enable = rtsnes_enable_queue;
        rtsnes->inter_queue.arg = rtsnes;

        if ((rc = interrupt_entry_init(&rtsnes->inter_queue, 0, NULL, cfg->priority)) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s(): interrupt_entry_init(rx) failed: %d", __func__, rc);
            rtsnes_cleanup(rtsnes);
            return rc;
        }

        if ((rc = pthread_mutex_init(&rtsnes->rx_mutex, NULL)) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "%s(): rx_mutex init failed: %d", __func__, rc);
            rtsnes_cleanup(rtsnes);
            return rc;
        }

        IFQ_SET_MAXLEN(&rtsnes->rx_queue, IFQ_MAXLEN);
        nw_pthread_create(&rtsnes->tid, NULL, rtsnes_rx_thread, rtsnes, 0,
            rtsnes_rx_thread_init, rtsnes);
    }

    rtsnes_clear_stats(rtsnes);
    
    strcpy((char *)cfg->device_description, "Renesas rtsnes Network Device tsn0");
    strcpy(rtsnes->dev.dv_xname, "tsn0");

    ifp = &rtsnes->ecom.ec_if;
    strcpy(ifp->if_xname, rtsnes->dev.dv_xname);
    ifp->if_softc = rtsnes;
    /* No multicast filtering in hardware so set IFF_ALLMULTI */
    ifp->if_flags = IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST | IFF_ALLMULTI;
    ifp->if_ioctl = rtsnes_ioctl;
    ifp->if_start = rtsnes_start;
    ifp->if_init  = rtsnes_init;
    ifp->if_stop  = rtsnes_stop;
    IFQ_SET_READY(&ifp->if_snd);
    rtsnes->ecom.ec_capabilities |= ETHERCAP_VLAN_MTU;

    if (cfg->verbose) {
        nic_dump_config(cfg);
    }

    if_attach(ifp);
    ether_ifattach(ifp, cfg->current_address);

    if (rtsnes->avb) {
        /* Intercept if_output for pulling off AVB packets */
        rtsnes->stack_output = rtsnes->ecom.ec_if.if_output;
        rtsnes->ecom.ec_if.if_output = rtsnes_output;
    }

    rtsnes_axibmi_enable_interrupt(rtsnes, 1, 1);
    rtsnes_axibmi_enable_interrupt(rtsnes, 0, 1);

    return EOK;
}

static char *rtsnes_opts[] = {
    "flow",
#define OPT_FLOW        0
    "mii",
#define OPT_MII         1
    "avb",  
#define OPT_AVB         2
    "emu_phy",
#define OPT_EMU_PHY     3
    "internal_loopback",
#define OPT_INTERNAL_LOOPBACK   4
    "tx_internal_delay",
#define OPT_TX_INTERNAL_DELAY   5
    "rx_internal_delay",
#define OPT_RX_INTERNAL_DELAY   6
    "bs",
#define OPT_BS          7
    NULL
};

static int rtsnes_parse_options (const char *optstring,
                 attach_args_t *attach_args)
{
    nic_config_t    *cfg;
    char            *options, *freeptr, *value;
    int             opt;

    if (optstring == NULL) {
        return EOK;
    }

    cfg = &attach_args->cfg;

    /* getsubopt() is destructive */
    freeptr = options = strdup(optstring);

    while (options && *options != '\0') {
        opt = getsubopt(&options, rtsnes_opts, &value);
        switch (opt) {

        case OPT_FLOW:
            attach_args->set_flow = strtoul(value, 0, 0);
            if (attach_args->set_flow > 3) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rtsnes: Unknown flow value %d, forcing off",
                  attach_args->set_flow);
            attach_args->set_flow = 0;
            }
            break;

        case OPT_MII:
            attach_args->mii = strtoul(value, 0, 0);
            switch ( attach_args->mii) {
            case TSNES_MII:
            case TSNES_RMII:
            case TSNES_GMII:
            case TSNES_RGMII:
                break;
            default:
                slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rtsnes: invalid mii option %s", value);
                (free)(freeptr);
                return EINVAL;
            }

        case OPT_EMU_PHY:
           attach_args->emu_phy = strtol (value, NULL, 0);
           break;

        case OPT_AVB:
            attach_args->avb = 1;
            break;

        case OPT_INTERNAL_LOOPBACK:
            attach_args->internal_loopback = 1;
            break;

        case OPT_TX_INTERNAL_DELAY:
            attach_args->tx_internal_delay = 1;
            break;

        case OPT_RX_INTERNAL_DELAY:
            attach_args->rx_internal_delay = 1;
            break;

        case OPT_BS:
            attach_args->bs_options = strdup(value);
            break;

        default:
            if (nic_parse_options(cfg, value) != EOK) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "rtsnes: Unknown option %s", value);
            (free)(freeptr);
            return EINVAL;
            }
        }
    }

    (free)(freeptr);
    return EOK;
}

CFATTACH_DECL(rtsnes,
    sizeof(rtsnes_dev_t),
    NULL,
    rtsnes_attach,
    rtsnes_detach,
    NULL);

static void rtsnes_hwinfo_find_nicadd (nic_config_t *cfg, unsigned hwi_off)
{
    unsigned  nicaddr_idx;

    if (hwi_off == HWI_NULL_OFF) {
        /* Find network hardware information. */
        hwi_off = hwi_find_device("rtsnes", 0);
    }

    if (hwi_off != HWI_NULL_OFF) {
        nicaddr_idx = cfg->device_index == -1 ? 0 : cfg->device_index;
        /* Find MAC address */
        hwitag_find_nicaddr(hwi_off, &nicaddr_idx, cfg->current_address);
    }
}

static void rtsnes_parse_syspage (nic_config_t *cfg)
{
    hwi_tag     *tag;
    unsigned    item;
    unsigned    irq_idx = 0;
    int         i;

    /* Find network hardware information. */
    item = hwi_find_device("rtsnes", 0);
    if (item == HWI_NULL_OFF) {
        return;
    }

    if (cfg->num_io_windows == 0) {
        /* Find base address */
        if ((tag = hwi_tag_find(item, HWI_TAG_NAME_location, NULL)) != NULL) {
            cfg->io_window_base[0] = (paddr_t)tag->location.base;
            cfg->io_window_size[0] = (size_t)tag->location.len;
            cfg->num_io_windows = 1;
        }
    }

    if (cfg->num_irqs == 0) {
        /* Find IRQ */
        for (i = 0; i < 8; i++) {
            cfg->irq[i] = hwitag_find_ivec(item, &irq_idx);
            if (cfg->irq[i] == HWI_ILLEGAL_VECTOR) {
                cfg->irq[i] = -1;
                break;
            }
            cfg->num_irqs++;
            irq_idx++;
        }
    }

    if ((memcmp(cfg->current_address, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0)) {
        rtsnes_hwinfo_find_nicadd(cfg, item);
    }
}

static int rtsnes_entry (void *dll_hdl,  struct _iopkt_self *iopkt, char *options)
{
   
    attach_args_t         *attach_args;
    nic_config_t          *cfg;
    int                   rc, single;
    struct ifnet          *ifp;
    struct drvcom_config  dcon;
    struct device	      *dev;

    /* Check if it is already mounted by doing a "nicinfo" on each interface */
    IFNET_FOREACH(ifp) {
        dcon.dcom_cmd.ifdc_cmd = DRVCOM_CONFIG;
        dcon.dcom_cmd.ifdc_len = sizeof(dcon.dcom_config);
        rc = ifp->if_ioctl(ifp, SIOCGDRVCOM, (caddr_t)&dcon);
        if ((rc == EOK) && (dcon.dcom_config.num_io_windows > 0) &&
                (dcon.dcom_config.io_window_base[0] == RCAR_TSNES_REG_BASE) ) {
            slogf(_SLOGC_NETWORK, _SLOG_ERROR,
                  "%s:%d - Driver already loaded for %s",
                  __FUNCTION__, __LINE__, ifp->if_xname);
            return EBUSY;
        }
    }

    attach_args = calloc(1,sizeof(*attach_args));
    attach_args->cfg.device_index = 0;
    attach_args->iopkt = iopkt;
    attach_args->dll_hdl = dll_hdl;
    attach_args->mii = TSNES_RGMII;
    /*
     * Limitation for V4H 1.0: If using Tx clock internal Delay Mode at 1Gbps, need to change IO voltage to 1.8V
     */
    attach_args->tx_internal_delay = 1;

    /* Set defaults before parsing the command line */
    cfg = &attach_args->cfg;
    cfg->media_rate = -1;
    cfg->duplex = -1;
    attach_args->set_flow = -1;
    attach_args->emu_phy = -1;
    cfg->mtu = ETHERMTU;
    cfg->mru = ETHERMTU;
    cfg->flags = NIC_FLAG_MULTICAST;
    cfg->mac_length = ETHER_ADDR_LEN;
    cfg->priority = stk_ctl.rx_prio;

    if (rtsnes_parse_options(options, attach_args) != EOK) {
        (free)(attach_args);
        return EINVAL;
    }

    /* If setting speed, must set duplex and vice versa */
    if (((cfg->media_rate != -1) && (cfg->duplex == -1)) ||
        ((cfg->media_rate == -1) && (cfg->duplex != -1))) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "rtsnes: Must set both speed and duplex if setting either");
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
    rtsnes_parse_syspage(cfg);

    if (cfg->num_io_windows == 0) {
        cfg->io_window_base[0] = RCAR_TSNES_REG_BASE;
        cfg->io_window_size[0] = RCAR_TSNES_REG_SIZE;
        cfg->num_io_windows = 1;
    }

    if (cfg->num_irqs == 0) {
        cfg->irq[0] = 429 + 32;//RCAR_GEN4_INTCSYS_SPI_429; // For TX
        cfg->num_irqs++;
        cfg->irq[1] = 430 + 32;//RCAR_GEN4_INTCSYS_SPI_430; // For RX
        cfg->num_irqs++;
    }

    if ((memcmp(cfg->current_address, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0) ||
        ETHER_IS_MULTICAST(cfg->current_address)) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR,
              "tsnes: Invalid MAC address %02x%02x%02x%02x%02x%02x",
              cfg->current_address[0], cfg->current_address[1],
              cfg->current_address[2], cfg->current_address[3],
              cfg->current_address[4], cfg->current_address[5]);
        return EINVAL;
    }

    dev = NULL; /* No Parent */
    rc = dev_attach("tsn", options, &rtsnes_ca, attach_args, &single, &dev, NULL);
    (free)(attach_args);

    return rc;
}

struct _iopkt_drvr_entry IOPKT_DRVR_ENTRY_SYM(rtsnes) = IOPKT_DRVR_ENTRY_SYM_INIT(rtsnes_entry);

#ifdef VARIANT_a
#include <nw_dl.h>
/* This is what gets specified in the stack's dl.c */
struct nw_dll_syms rtsnes_syms[] = {
    {"iopkt_drvr_entry", &IOPKT_DRVR_ENTRY_SYM(rtsnes)},
    {NULL, NULL}
};
#endif
