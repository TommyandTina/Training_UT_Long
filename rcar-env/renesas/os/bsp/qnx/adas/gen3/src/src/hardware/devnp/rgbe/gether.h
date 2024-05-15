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
#ifndef GETHER_H
#define GETHER_H

#include <io-pkt/iopkt_driver.h>
#include <sys/device.h>
#include <sys/io-pkt.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#if (_NTO_VERSION < 660)
#include <drvr/nicsupport.h>
#include <drvr/nic_mutex.h>
#else
#include <netdrvr/nicsupport.h>
#include <netdrvr/nic_mutex.h>
#endif

/***********************************************
 ********** Configurable items *****************
 ***********************************************/
/* gether register base */
#define GETHER_REG_BASE          0xE7400000
#define GETHER_REG_SIZE          0x1000
#define GETHER_IRQ               (21 + 32)
/* gether register offsets */
#define GETHER_EDSR              (0x0)     /* 32-bit    E-DMAC start register */
#define GETHER_EDSR_ENT          (1 << 1)
#define GETHER_EDSR_ENR          (1 << 0)
#define GETHER_TDLAR             (0x10)    /* 32-bit    Transmit descriptor list start address register */
#define GETHER_TDFAR             (0x14)    /* 32-bit    Transmit Descriptor Fetch Address Register */
#define GETHER_TDFXR             (0x18)    /* 32-bit    Transmit Descriptor Finished Address Register */
#define GETHER_TDFFR             (0x1C)    /* 32-bit    Transmit Descriptor Final Flag Register */
#define GETHER_RDLAR             (0x30)    /* 32-bit    Receive Descriptor List Start Address Register */
#define GETHER_RDFAR             (0x34)    /* 32-bit    Receive Descriptor Fetch Address Register */
#define GETHER_RDFXR             (0x38)    /* 32-bit    Receive Descriptor Finished Address Register */
#define GETHER_RDFFR             (0x3C)    /* 32-bit    Receive Descriptor Final Flag Register */
#define GETHER_EDMR              (0x400)   /* 32-bit    E-DMAC Mode Register */
#define GETHER_EDMR_SWRR         (0x1 << 0)
#define GETHER_EDMR_SWRT         (0x1 << 1)
#define GETHER_EDMR_DL_16        (0x0 << 4)
#define GETHER_EDMR_DL_32        (0x01 << 4)
#define GETHER_EDMR_DL_64        (0x10 << 4)
#define GETHER_EDMR_NBST         (0x1 << 7)
#define GETHER_EDMR_DE_BIG       (0x0 << 6)
#define GETHER_EDMR_DE_LITTLE    (0x1 << 6)

#define GETHER_EDTRR             (0x408)   /* 32-bit    E-DMAC Transmit Request Register */
#define GETHER_EDTRR_TR_DMA      (0x3 << 0)
#define GETHER_EDRRR             (0x410)   /* 32-bit    E-DMAC Receive Request Register */
#define GETHER_EDRRR_RR          (0x1 << 0)

#define GETHER_EESR              (0x428)   /* 32-bit    E-MAC/E-DMAC Status Register */
#define GETHER_EESR_TWB          (0x11 << 30)
#define GETHER_EESR_TC1          (0x1 << 29)
#define GETHER_EESR_TUC          (0x1 << 28)
#define GETHER_EESR_ROC          (0x1 << 27)
#define GETHER_EESR_TABT         (0x1 << 26)
#define GETHER_EESR_RABT         (0x1 << 30)
#define GETHER_EESR_RFCOF        (0x1 << 24)
#define GETHER_EESR_ECI          (0x1 << 22)
#define GETHER_EESR_TC0          (0x1 << 21)
#define GETHER_EESR_TDE          (0x1 << 20)
#define GETHER_EESR_TFUF         (0x1 << 19)
#define GETHER_EESR_FR           (0x1 << 18)
#define GETHER_EESR_RDE          (0x1 << 17)
#define GETHER_EESR_RFOF         (0x1 << 16)
#define GETHER_EESR_DLC          (0x1 << 10)
#define GETHER_EESR_CD           (0x1 << 9)
#define GETHER_EESR_TRO          (0x1 << 8)
#define GETHER_EESR_RMAF         (0x1 << 7)
#define GETHER_EESR_CELF         (0x1 << 5)
#define GETHER_EESR_RRF          (0x1 << 4)
#define GETHER_EESR_RTLF         (0x1 << 3)
#define GETHER_EESR_RTSF         (0x1 << 2)
#define GETHER_EESR_PRE          (0x1 << 1)
#define GETHER_EESR_CERF         (0x1 << 0)

#define GETHER_EESIPR            (0x430)   /* 32-bit    E-MAC/E-DMAC Status Interrupt Permission Register */
#define GETHER_EESIPR_TWB1IP     (0x1 << 31)
#define GETHER_EESIPR_TWB0IP     (0x1 << 30)
#define GETHER_EESIPR_TC1IP      (0x1 << 29)
#define GETHER_EESIPR_TUCIP      (0x1 << 28)
#define GETHER_EESIPR_ROCIP      (0x1 << 27)
#define GETHER_EESIPR_TABTIP     (0x1 << 26)
#define GETHER_EESIPR_RABTIP     (0x1 << 25)
#define GETHER_EESIPR_RFCOFIP    (0x1 << 24)
#define GETHER_EESIPR_ECIIP      (0x1 << 22)
#define GETHER_EESIPR_TC0IP      (0x1 << 21)
#define GETHER_EESIPR_TDEIP      (0x1 << 20)
#define GETHER_EESIPR_TFUFIP     (0x1 << 19)
#define GETHER_EESIPR_FRIP       (0x1 << 18)
#define GETHER_EESIPR_RDEIP      (0x1 << 17)
#define GETHER_EESIPR_RFOFIP     (0x1 << 16)
#define GETHER_EESIPR_DLCIP      (0x1 << 10)
#define GETHER_EESIPR_CDIP       (0x1 << 9)
#define GETHER_EESIPR_TROIP      (0x1 << 8)
#define GETHER_EESIPR_RMAFIP     (0x1 << 7)
#define GETHER_EESIPR_CELFIP     (0x1 << 5)
#define GETHER_EESIPR_RRFIP      (0x1 << 4)
#define GETHER_EESIPR_RTLFIP     (0x1 << 3)
#define GETHER_EESIPR_RTSFIP     (0x1 << 2)
#define GETHER_EESIPR_PREIP      (0x1 << 1)
#define GETHER_EESIPR_CERFIP     (0x1 << 0)


#define GETHER_TRSCER            (0x438)   /* 32-bit    Transmit/Receive Status Copy Enable Register */
#define GETHER_RMFCR             (0x440)   /* 32-bit    Receive Missed-Frame Counter Register */
#define GETHER_TFTR              (0x448)   /* 32-bit    Transmit FIFO Threshold Register */
#define GETHER_FDR               (0x450)   /* 32-bit    FIFO Depth Register */
#define GETHER_FDR_TFD_4K        (0xF)
#define GETHER_FDR_RFD_8K        (0x1F)
#define GETHER_FDR_TFD(x)        ((x & 0xF) << 8 )
#define GETHER_FDR_RFD(x)        ((x & 0x1F) << 0 )

#define GETHER_RMCR              (0x458)   /* 32-bit    Receiving Method Control Register */
#define GETHER_RMCR_RNC          (0x1 << 0)
#define GETHER_RPADIR            (0x460)   /* 32-bit    Receive Data Padding Insert Register */
#define GETHER_FCFTR             (0x468)   /* 32-bit    Overflow Alert FIFO Threshold Register */
/* FCFTR */
enum FCFTR_BIT {
    FCFTR_RFF2 = 0x00040000, FCFTR_RFF1 = 0x00020000,
    FCFTR_RFF0 = 0x00010000, FCFTR_RFD2 = 0x00000004,
    FCFTR_RFD1 = 0x00000002, FCFTR_RFD0 = 0x00000001,
};
#define FIFO_F_D_RFF    (FCFTR_RFF2|FCFTR_RFF1|FCFTR_RFF0)
#define FIFO_F_D_RFD    (FCFTR_RFD2|FCFTR_RFD1|FCFTR_RFD0)

#define GETHER_ECMR              (0x500)   /* 32-bit    E-MAC Mode Register */
enum ECMR_BIT {
  GETHER_ECMR_TRCCM = 0x04000000, GETHER_ECMR_RCSC = 0x00800000,
  GETHER_ECMR_DPAD = 0x00200000,  GETHER_ECMR_RZPF = 0x00100000,
  GETHER_ECMR_ZPF = 0x00080000,   GETHER_ECMR_PFR = 0x00040000, GETHER_ECMR_RXF = 0x00020000,
  GETHER_ECMR_TXF = 0x00010000,   GETHER_ECMR_MCT = 0x00002000, GETHER_ECMR_PRCEF = 0x00001000,
  GETHER_ECMR_PMDE = 0x00000200,  GETHER_ECMR_RE = 0x00000040,  GETHER_ECMR_TE = 0x00000020,
  GETHER_ECMR_ILB = 0x00000008,   GETHER_ECMR_ELB = 0x00000004, GETHER_ECMR_DM = 0x00000002,
  GETHER_ECMR_PRM = 0x00000001,
};
#if 0
#define GETHER_ECMR_CHG_DM (GETHER_ECMR_TRCCM | GETHER_ECMR_RZPF | GETHER_ECMR_ZPF | GETHER_ECMR_PFR | \
                            GETHER_ECMR_RXF | GETHER_ECMR_TXF | GETHER_ECMR_MCT)
#else
#define GETHER_ECMR_CHG_DM (GETHER_ECMR_TRCCM | GETHER_ECMR_RZPF | GETHER_ECMR_ZPF | GETHER_ECMR_PFR)
#endif


#define GETHER_RFLR              (0x508)   /* 32-bit    Receive Frame Length Register */
#define GETHER_ECSR              (0x510)   /* 32-bit    E-MAC Status Register */
#define GETHER_ECSIPR            (0x518)   /* 32-bit    E-MAC Interrupt Permission Register */
#define GETHER_PIR               (0x520)   /* 32-bit    PHY Interface Register */
#define GETHER_PIR_MDI           (0x1 << 3)
#define GETHER_PIR_MDO           (0x1 << 2)
#define GETHER_PIR_MMD           (0x1 << 1)
#define GETHER_PIR_MDC           (0x1 << 0)
#define GETHER_PSR               (0x528)   /* 32-bit    PHY Status Register */


#define GETHER_PIPR              (0x52C)   /* 32-bit    PHY_INT Polarity Register */
#define GETHER_APR               (0x554)   /* 32-bit    Automatic PAUSE Frame Register */
enum APR_BIT {
    APR_AP = 0x00000004,
};
#define GETHER_MPR               (0x558)   /* 32-bit    Manual PAUSE Frame Register */
/* MPR */
enum MPR_BIT {
    MPR_MP = 0x00000006,
};
#define GETHER_PFTCR             (0x55C)   /* 32-bit    PAUSE Frame Transmit Counter Register */
#define GETHER_PFRCR             (0x560)   /* 32-bit    PAUSE Frame Receive Counter Register */
#define GETHER_TPAUSER           (0x564)   /* 32-bit    Automatic PAUSE Frame Retransmit Count Register */
/* TPAUSER */
enum TPAUSER_BIT {
    TPAUSER_TPAUSE = 0x0000ffff,
    TPAUSER_UNLIMITED = 0,
};
#define GETHER_GECMR             (0x5B0)        /* 32-bit    GETHER Mode Register */
#define GETHER_GECMR_SPEED0      (0x1 << 0)     /* 1-Gbps transfer */
#define GETHER_GECMR_BSE         (0x1 << 1)     /* Burst Transfer Enable */
#define GETHER_GECMR_SPEED1      (0x0 << 2)     /* Transfer Speed */
#define GETHER_GECMR_SPEED_BITS  (GETHER_GECMR_SPEED0 | GETHER_GECMR_SPEED1)
#define GETHER_GECMR_SPEED_GIGE  GETHER_GECMR_SPEED0

#define GETHER_BCULR             (0x5B4)   /* 32-bit    Burst Cycle Count Upper-Limit Register */
#define GETHER_MAHR              (0x5C0)   /* 32-bit    MAC Address High Register */
#define GETHER_MALR              (0x5C8)   /* 32-bit    MAC Address Low Register */
#define GETHER_TROCR             (0x700)   /* 32-bit    Transmit Retry Over Counter Register */
#define GETHER_CDCR              (0x708)   /* 32-bit    Delayed Collision Detect Counter Register */
#define GETHER_LCCR              (0x710)   /* 32-bit    Lost Carrier Counter Register */
#define GETHER_CEFCR             (0x740)   /* 32-bit    CRC Error Frame Receive Counter Register */
#define GETHER_FRECR             (0x748)   /* 32-bit    Frame Receive Error Counter Register */
#define GETHER_TSFRCR            (0x750)   /* 32-bit    Too-Short Frame Receive Counter Register */
#define GETHER_TLFRCR            (0x758)   /* 32-bit    Too-Long Frame Receive Counter Register */
#define GETHER_RFCR              (0x760)   /* 32-bit    Residual-Bit Frame Receive Counter Register */
#define GETHER_CERCR             (0x768)   /* 32-bit    Carrier Extension Loss Counter Register */
#define GETHER_CEECR             (0x770)   /* 32-bit    Carrier Extension Error Counter Register */
#define GETHER_MAFCR             (0x778)   /* 32-bit    Multicast Address Frame Receive Counter Register */
#define GETHER_PMSR              (0x790)   /* 32-bit    PHY Mode Select Register */
#define GETHER_RMII              (0x0 & 0x3)   /* 32-bit    RMII Mode */
#define GETHER_RGMII             (0x3 & 0x3)   /* 32-bit   RGMII Mode */

#define GETHER_GPSR              (0x820)   /* 32-bit    GETHER Product Specific Register */
#define GETHER_FSSTAT            (0x824)   /* 32-bit    Functional Safety Status Register */
#define GETHER_FSINTE            (0x828)   /* 32-bit    Functional Safety Interrupt Enable Register */
#define GETHER_DEIR              (0x82C)   /* 32-bit    Dummy Error Insertion Register */

#define EDMAC_STATUS_DISABLE     0
#define EDMAC_STATUS_ENABLE      1

#define NUM_TX_DESC              128
#define NUM_RX_DESC              256

  /* RFLR */
#define RFLR_RFL_MIN             0x05EE  /* Recv Frame length 1518 byte */
#define ETH_MAX_DATA_LEN         0x05EE

#define VERBOSE_TX               (1<<3)
#define VERBOSE_RX               (1<<2)
#define VERBOSE_PHY              (1<<1)

/* Transmit Descriptor */
#define TX_TD0_TACT       (1<<31)
#define TX_TD0_TDLE       (1<<30)
#define TX_TD0_TFP(x)     ((x)<<28)
#define TX_TD0_TFE        (1<<27)
#define TX_TD0_TWBI       (1<<26)
#define TX_TD0_TFS(x)     (x & 0xFFF)
#define TX_TD1_TDL(x)     (x & 0xFFFF)
#define TX_TD2_TBA(x)     (x & 0xFFFFFFFF)
typedef struct {
  uint32_t            status;
  uint16_t            reserved;
  uint16_t            buf_len;
  uint32_t            addr;
  uint32_t            pad;
} GETHER_TX_TD;
/* End Transmit Descriptor  */

/* Receive Descriptor */
#define RX_RD0_RACT       (1<<31)
#define RX_RD0_RDLE       (1<<30)
#define RX_RD0_RFP1       (1<<29)
#define RX_RD0_RFP0       (1<<28)
#define RX_RD0_RFE        (1<<27)
#define RX_RD0_RFS(x)     (((1<<(x)))<<16)
#define RX_RD0_RFS_MASK(x) ((x) & (0x3FF<<16))
#define RX_RD0_RWBI(x)    (((x & 0x3FF)<<16))
#define RX_RD1_RBL(x)     (x & 0xFFF)
#define RX_RD1_RDL(x)     (x & 0xFFFF)
#define RX_RD2_RBA(x)     (x & 0xFFFFFFFF)
typedef struct {
  uint32_t          status;
  uint16_t          frame_len;
  uint16_t          buf_len;
  uint32_t          addr;
  uint32_t          pad;
} GETHER_RX_RD;
/* End Receive Descriptor  */


typedef struct {
    nic_config_t          cfg;
    int                   set_flow;
    int                   isr;
    struct _iopkt_self    *iopkt;
    void                  *dll_hdl;
    char                  *bs_options;
} attach_args_t;

typedef enum {
    FLOW_AUTO = -1,
    FLOW_BOTH,
    FLOW_NONE,
    FLOW_TX,
    FLOW_RX
} flow_t;

typedef struct {
    struct device           dev;
    struct ethercom         ecom;
    struct mii_data         bsd_mii;
    mdi_t                   *mdi;
    flow_t                  set_flow;
    int                     flow_status;
    int                     set_speed;
    int                     set_duplex;
    struct callout          mii_callout;
    nic_config_t            cfg;
    nic_stats_t             stats;
    void                    *sdhook;
    int                     iid;
    pthread_t               tid;
    struct _iopkt_inter     rx_ient;
    struct _iopkt_inter     tx_ient;
    struct _iopkt_inter     cfg_ient;
    struct _iopkt_self      *iopkt;
    uintptr_t               base;
    int                     fd;
    struct cache_ctrl       cachectl;
    struct mbuf             **tx_pkts;
    GETHER_TX_TD            *tx_bd;
    int                     tx_cidx;
    int                     tx_pidx;
    struct mbuf             **rx_pkts;
    GETHER_RX_RD            *rx_bd;
    int                     rx_idx;
    int                     pkts_received;
#define EVP_ARRAY_SIZE 3
    struct sigevent	    *evp[EVP_ARRAY_SIZE];
    uint32_t                rx_eesr;
    uint32_t                tx_eesr;
#define FLAG_DYING      0x00000001
#define FLAG_KTHREAD    0x00000002
    volatile unsigned int   flags;
    nic_mutex_t             drv_mutex;
    char                    *bs_options;
} gether_dev_t;

void gether_update_stats(gether_dev_t *gether);
void gether_clear_stats(gether_dev_t *gether);
int gether_ioctl(struct ifnet *ifp, unsigned long cmd, caddr_t data);

void gether_reap_tx(gether_dev_t *gether);
void gether_start(struct ifnet *ifp);
void gether_hw_stop (gether_dev_t *gether);

void gether_receive(gether_dev_t *gether, struct nw_work_thread *wtp);

int gether_mediachange(struct ifnet *ifp);
int gether_phy_init(gether_dev_t *gether);
void gether_phy_fini(gether_dev_t *gether);

int gether_process_interrupt_rx(void *arg, struct nw_work_thread *wtp);
int gether_enable_interrupt_rx(void * arg);
int gether_process_interrupt_tx(void *arg, struct nw_work_thread *wtp);
int gether_enable_interrupt_tx(void * arg);
int gether_process_interrupt_cfg(void *arg, struct nw_work_thread *wtp);
int gether_enable_interrupt_cfg(void * arg);

void * gether_isr_thread(void *arg);
const struct sigevent * const * gether_isr(void *arg, int iid);

uint16_t gether_mii_read(void *handle, uint8_t phy_id, uint8_t location);
void gether_mii_write(void *handle, uint8_t phy_id, uint8_t location,
                      uint16_t data);

int gether_mv_phy_init (gether_dev_t *gether);
void gether_mv_phy_fini(gether_dev_t *gether);
int gether_is_mv_phy (gether_dev_t *gether);
void gether_MonitorPhy(void *arg);

#define TX_INT_FLAGS (GETHER_EESIPR_TABTIP | GETHER_EESIPR_TC0IP | GETHER_EESIPR_TC1IP | GETHER_EESIPR_TFUFIP)
#define RX_INT_FLAGS (GETHER_EESIPR_RABTIP | GETHER_EESIPR_FRIP | GETHER_EESIPR_RFOFIP)

static inline void unmask_tx_int(gether_dev_t *gether) {
    out32(gether->base + GETHER_EESIPR, in32(gether->base + GETHER_EESIPR) | TX_INT_FLAGS);
}

static inline void unmask_rx_int(gether_dev_t *gether) {
    out32(gether->base + GETHER_EESIPR, in32(gether->base + GETHER_EESIPR) | RX_INT_FLAGS);
}

static inline void mask_tx_int(gether_dev_t *gether) {
    out32(gether->base + GETHER_EESIPR, in32(gether->base + GETHER_EESIPR) & (~TX_INT_FLAGS));
}

static inline void mask_rx_int(gether_dev_t *gether) {
    out32(gether->base + GETHER_EESIPR, in32(gether->base + GETHER_EESIPR) & (~RX_INT_FLAGS));
}

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/devnp/rgbe/gether.h $ $Rev: 888807 $")
#endif
