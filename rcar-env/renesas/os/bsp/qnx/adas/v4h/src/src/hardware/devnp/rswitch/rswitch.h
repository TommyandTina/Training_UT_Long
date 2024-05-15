/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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

#ifndef __RCAR_RSWITCH2_H__
#define __RCAR_RSWITCH2_H__

#include <io-pkt/iopkt_driver.h>
#include <sys/device.h>
#include <sys/io-pkt.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <sys/kthread.h>
#include <netdrvr/nicsupport.h>
#include <netdrvr/nic_mutex.h>
#include <netdrvr/ptp.h>
#include <sys/netmgr.h>
#include <quiesce.h>
#include <siglock.h>
#include <nw_thread.h>
#include <quiesce.h>


/*
 * io-pkt defaults to 8192 clusters of 2048 bytes each.
 * On an rswitch2 we have 4 queues of Tx and 18 queues of Rx.
 * We need to ensure that we don't use all the clusters here.
 *
 * Use only 1/4 of 8192 at most. Rx needs to be big enough to
 * absorb bursts from the network, but that likely only applies
 * to queue 0. Tx can typically be smaller than Rx.
 */

#define BIT(x)                        (1 << (x))
#define BITS(x,y)                     ((x) << (y))
#define BITMASK(x,y)                  (((2ULL << (y-x)) - 1) << (x))

#define VERBOSE_PTP                   BIT(4)
#define VERBOSE_TX                    BIT(3)
#define VERBOSE_RX                    BIT(2)
#define VERBOSE_PHY                   BIT(1)

#define NUM_TX_DESC                   127
#define NUM_RX_DESC                   127
#define NUM_TX_DESC_WITH_LINK         (NUM_TX_DESC + 1)
#define NUM_RX_DESC_WITH_LINK         (NUM_RX_DESC + 1)
                                      
#define NUM_TX_QUEUES                 4
#define NUM_RX_QUEUES                 1
#define DBAT_ENTRY_NUM                (NUM_TX_QUEUES + NUM_RX_QUEUES)
#define RX_QUEUE_OFFSET               0
#define TX_QUEUE_OFFSET               (RX_QUEUE_OFFSET + NUM_RX_QUEUES)
#define RSWITCH2_FRAGS                1

#define PKT_BUF_SZ                    ETHER_MAX_LEN + ETHER_VLAN_ENCAP_LEN
#define RSWITCH2_ALIGN                128
#define ALIGN_MASK(x, mask)           (((x) + (mask)) & ~(mask))
#define ALIGN(x, a)                   ALIGN_MASK(x, (typeof(x))(a) - 1)

/* PTP */
#define RSWITCH2_RX_TIMESTAMP_BUF_SZ  64
#define RSWITCH2_TX_TIMESTAMP_BUF_SZ  16

/* Definition for S4 */
#define RCAR_RSWITCH2_REG_BASE        0xE68C0000
#define RCAR_RSWITCH2_REG_SIZE        0x20000

#define GWCA0_RXTX_CPU(n)             (312 + n)
#define GWCA1_RXTX_CPU(n)             (320 + n)
#define ETHA_STATUS(n)                (304 + n)

#define RSWITCH2_QUIESCE_PULSE        _PULSE_CODE_MINAVAIL
#define RSWITCH2_RX_PULSE             (RSWITCH2_QUIESCE_PULSE + 1)
#define RSWITCH2_RX_THREAD_NAME       "rswitch Rx"

#define RSWITCH2_TSN_PORT_MAX         3
#define RSWITCH2_NUM_IRQS             8
#define RSWITCH2_NUM_GWCA             2
#define RSWITCH2_NUM_ETHA             3
#define RSWITCH2_NUM_HW               5
#define RSWITCH2_NUM_GWCA_CHAIN       128
#define RSWITCH2_MAX_CTAG_PCP         7
#define RSWITCH2_IRQ_NUM_USE         ((DBAT_ENTRY_NUM * RSWITCH2_TSN_PORT_MAX / 32) + 1)
#define RSWITCH2_GWCA0_HW_INDEX       3
#define RSWITCH2_GWCA1_HW_INDEX       4
#define RSWITCH2_IRQ_NUM              1//(128 / 32)

/* Rswitch2 module base offsets */
#define RSWITCH2_COMA_OFFSET          0x00009000
#define RSWITCH2_ETHA_OFFSET          0x0000a000      /* with RMAC */
#define RSWITCH2_ETHA_SIZE            0x00002000      /* with RMAC */
#define RSWITCH2_GWCA_OFFSET          0x00010000
#define RSWITCH2_GWCA_SIZE            0x00002000

#define RSWITCH2_FWRO                 0
#define RSWITCH2_CARO                 RSWITCH2_COMA_OFFSET
#define RSWITCH2_GWRO                 0  // Offser from RSWITCH2_GWCA_OFFSET
#define RSWITCH2_TARO                 0  // Offser from RSWITCH2_ETHA_OFFSET
#define RSWITCH2_RMRO                 0x1000
#define RSWITCH2_PTPRO                0x18000

/* Rswitch2 register offsets and bits */
/* COMA */
#define RRC                         (RSWITCH2_CARO + 0x0004)
 #define RRC_RR                      BIT(0)
  #define RCEC_RCE                   BIT(16)
#define RCEC                        (RSWITCH2_CARO + 0x0008)
#define RCDC                        (RSWITCH2_CARO + 0x000C)
#define CABPIRM                     (RSWITCH2_CARO + 0x0140)
 #define CABPIRM_BPIOG               BIT(0)
 #define CABPIRM_BPR                 BIT(1)

/* GWCA */
#define GWMC                        (RSWITCH2_GWRO + 0x0000)
 #define GWMC_OPC_RESET              0
 #define GWMC_OPC_DISABLE            1
 #define GWMC_OPC_CONFIG             2
 #define GWMC_OPC_OPERATION          3
 #define GWMS_OPS_MASK               GWMC_OPC_OPERATION
#define GWMS                        (RSWITCH2_GWRO + 0x0004)
#define GWIRC                       (RSWITCH2_GWRO + 0x0010)
#define GWRDQDC(idx)                (RSWITCH2_GWRO + 0x0060 + (idx * 4))
#define GWRDQM(idx)                 (RSWITCH2_GWRO + 0x0080 + (idx * 4))
#define GWRDQMLM(idx)               (RSWITCH2_GWRO + 0x00A0 + (idx * 4))
 #define GWRDQDC_DQD                (NUM_RX_DESC_WITH_LINK)
#define GWMAC0                      (RSWITCH2_GWRO + 0x0120)
#define GWMAC1                      (RSWITCH2_GWRO + 0x0124)
#define GWMTIRM                     (RSWITCH2_GWRO + 0x0100)
 #define GWMTIRM_MTIOG               BIT(0)
 #define GWMTIRM_MTR                 BIT(1)
#define GWVCC                       (RSWITCH2_GWRO + 0x0130)
 #define GWVCC_VEM_SC_TAG            (0x3 << 16)
#define GWTTFC                      (RSWITCH2_GWRO + 0x0138)
#define GWMDNC                      (RSWITCH2_GWRO + 0x01A0)
 #define GWMDNC_TSDMN(val)           ((val & 0x1f) << 16)
 #define GWMDNC_RXDMN(val)           ((val & 0x1f) << 0)
 #define GWMDNC_TXDMN(val)           ((val & 0x1f) << 8)
#define GWDCBAC0                    (RSWITCH2_GWRO + 0x0194)
 #define GWDCBAC0_DCBAUP             (0xff)
#define GWDCBAC1                    (RSWITCH2_GWRO + 0x0198)
#define GWIICBSC                    (RSWITCH2_GWRO + 0x019C)
#define GWTRC(i)                    (RSWITCH2_GWRO + 0x0200 + (i) * 0x04)
#define GWARIRM                     (RSWITCH2_GWRO + 0x0380)
 #define GWARIRM_ARIOG               BIT(0)
 #define GWARIRM_ARR                 BIT(1)
#define GWDCC(ch)                   (RSWITCH2_GWRO + 0x0400 + (ch) * 4)
 #define GWDCC_BALR                  BIT(24)
 #define GWDCC_DCP(q, idx)          ((q + (idx * 2)) << 16)
 #define GWDCC_DQT                   BIT(11)
 #define GWDCC_ETS                   BIT(9)
 #define GWDCC_EDE                   BIT(8)
#define GWGRLC                      (RSWITCH2_GWRO + 0x0A00)
#define GWGRLULC                    (RSWITCH2_GWRO + 0x0A04)
#define GWDIS                       (RSWITCH2_GWRO + 0x1100)
#define GWDIE                       (RSWITCH2_GWRO + 0x1104)
#define GWDID                       (RSWITCH2_GWRO + 0x1108)
#define GWDIDS                      (RSWITCH2_GWRO + 0x110C)
#define GWTSDIS                     (RSWITCH2_GWRO + 0x1180)
#define GWTSDIE                     (RSWITCH2_GWRO + 0x1184)
#define GWTSDID                     (RSWITCH2_GWRO + 0x1188)
#define GWEIS0                      (RSWITCH2_GWRO + 0x1190)
#define GWEIE0                      (RSWITCH2_GWRO + 0x1194)
#define GWEID0                      (RSWITCH2_GWRO + 0x1198)
#define GWEIS1                      (RSWITCH2_GWRO + 0x11A0)
#define GWEIE1                      (RSWITCH2_GWRO + 0x11A4)
#define GWEID1                      (RSWITCH2_GWRO + 0x11A8)
#define GWIDPC                      (RSWITCH2_GWRO + 0x0B80)
#define GWIDC(q)                    (RSWITCH2_GWRO + 0x0C00 + (q * 4))

/* MFWD */
#define FWPC0(i)                     (RSWITCH2_FWRO + 0x0100 + (i) * 0x10)
 #define FWPC0_LTHTA                 BIT(0)
 #define FWPC0_IP4UE                 BIT(3)
 #define FWPC0_IP4TE                 BIT(4)
 #define FWPC0_IP4OE                 BIT(5)
 #define FWPC0_L2SE                  BIT(9)
 #define FWPC0_IP4EA                 BIT(10)
 #define FWPC0_IPDSA                 BIT(12)
 #define FWPC0_IPHLA                 BIT(18)
 #define FWPC0_MACSDA                BIT(20)
 #define FWPC0_MACHLA                BIT(26)
 #define FWPC0_MACHMA                BIT(27)
 #define FWPC0_VLANSA                BIT(28)
 #define FWPC0_DEFAULT               (FWPC0_LTHTA | FWPC0_IP4UE | FWPC0_IP4TE | \
                                    FWPC0_IP4OE | FWPC0_L2SE | FWPC0_IP4EA | \
                                    FWPC0_IPDSA | FWPC0_IPHLA | FWPC0_MACSDA | \
                                    FWPC0_MACHLA | FWPC0_MACHMA | FWPC0_VLANSA)
#define FWPC1(i)                    (RSWITCH2_FWRO + 0x104 + (i) * 0x10)
#define FWPC2(i)                    (RSWITCH2_FWRO + 0x108 + (i) * 0x10)
#define FWPBFC(i)                  (RSWITCH2_FWRO + 0x4A00 + (i) * 0x10)
#define FWPBFCSDC(j, i)            (RSWITCH2_FWRO + 0x4A04 + (i) * 0x10 + (j) * 0x04)

/* ETHA */
#define EAMC                        (RSWITCH2_TARO + 0x0000)
 #define EAMC_OPC_RESET              0
 #define EAMC_OPC_DISABLE            1
 #define EAMC_OPC_CONFIG             2
 #define EAMC_OPC_OPERATION          3
 #define EAMS_OPS_MASK               EAMC_OPC_OPERATION
#define EAMS                        (RSWITCH2_TARO + 0x0004)
#define EATDQDC(q)                  (RSWITCH2_TARO + 0x0060 + (q) * 0x04)
 #define EATDQDC_DQD                (NUM_TX_DESC_WITH_LINK)
#define EATTFC                      (RSWITCH2_TARO + 0x0138)
#define EATASRIRM                   (RSWITCH2_TARO + 0x03E4)
 #define EATASRIRM_TASRIOG           BIT(0)
 #define EATASRIRM_TASRR             BIT(1)
 #define EAIRC                      (RSWITCH2_TARO + 0x0010)


/* RMAC */
#define MPSM                        (RSWITCH2_RMRO + 0x0000)
 #define MPSM_MFF_C45                BIT(2)
 #define MPSM_MFF_C22                (0x0 << 2)
 #define MPSM_PSME                   BIT(0)
 #define MPSM_POP_MASK               (0x03 << 13)
 #define MPSM_PRA_MASK               (0x1f << 8)
 #define MPSM_PDA_MASK               (0x1f << 3)
 #define MPSM_PRD_MASK               (0xffff << 16)
#define MPIC                        (RSWITCH2_RMRO + 0x0004)
 #define MPIC_PIS_MASK               0x07
 #define MPIC_PIS_MII                0x00
 #define MPIC_PIS_GMII               0x02
 #define MPIC_PIS_XGMII              0x04
 #define MPIC_LSC_MASK               (0x07 << 3)
 #define MPIC_LSC_100                (0x01 << 3)
 #define MPIC_LSC_1000               (0x02 << 3)
 #define MPIC_LSC_2500               (0x03 << 3)
 #define MPIC_PSMCS_SHIFT            16
 #define MPIC_PSMCS_MASK            (0x7f << 16)
 #define MPIC_PSMCS(val)            ((val) << MPIC_PSMCS_SHIFT)
 #define MPIC_PSMHT_SHIFT            24
 #define MPIC_PSMHT_MASK            (0x07 << 24)
 #define MPIC_PSMHT(val)            ((val) << MPIC_PSMHT_SHIFT)
 #define MPIC_MDC_CLK_SET            (0x06050000)
#define MRMAC0                      (RSWITCH2_RMRO + 0x0084)
#define MRMAC1                      (RSWITCH2_RMRO + 0x0088)
#define MRAFC                       (RSWITCH2_RMRO + 0x008C)
#define MRSCE                       (RSWITCH2_RMRO + 0x0090)
#define MRSCP                       (RSWITCH2_RMRO + 0x0094)
#define MLVC                        (RSWITCH2_RMRO + 0x0180)
 #define MLVC_PLV                    BIT(16)
 #define MLVC_LVT                    0x09
#define MLBC                        (RSWITCH2_RMRO + 0x0188)
#define MXGMIIC                     (RSWITCH2_RMRO + 0x0190)
#define MPCH                        (RSWITCH2_RMRO + 0x0194)
#define MANM                        (RSWITCH2_RMRO + 0x019C)
#define MEIS                        (RSWITCH2_RMRO + 0x0200)
#define MMIS0                       (RSWITCH2_RMRO + 0x0210)
 #define MMIS0_LVSS                  0x02
#define MMIS1                       (RSWITCH2_RMRO + 0x0220)
 #define MMIS1_PAACS                 BIT(2) /* Address */
 #define MMIS1_PWACS                 BIT(1) /* Write */
 #define MMIS1_PRACS                 BIT(0) /* Read */
 #define MMIS1_CLEAR_FLAGS           0xf

/* Serdes */
#define RSWITCH2_SERDES_BASE                0xE6444000
#define RSWITCH2_SERDES_OFFSET              0x0400
#define RSWITCH2_SERDES_BANK_OFFSET         0x03fc
#define RSWITCH2_SERDES_FUSE_OVERRIDE(n)    (0x2600 - (n) * 0x400)
#define RSWITCH2_SERDES_SIZE                0x1000

#define SERDES_SGMII                0
#define SERDES_USXGMII              1

/* gPTP */
/* Currently PTP uses queue 0 */
#define NUM_PTP_TX_QUEUES 1
#define PTP_TX_BUF_LEN 64
#define PTP_TX_MSGTYPE 4
ptpv2hdr_t  tx_ph[NUM_PTP_TX_QUEUES][PTP_TX_BUF_LEN * PTP_TX_MSGTYPE];

/* Bit definitions for the GPTI register */
#define TIV_MASK                            0xFFFFFFFF

#define  PTPIPV                    (RSWITCH2_PTPRO + 0x0000)
#define  PTPTMEC                   (RSWITCH2_PTPRO + 0x0010)
 #define PTPTMEC_TE                 BIT(0)
#define  PTPTMDC                   (RSWITCH2_PTPRO + 0x0014)
 #define PTPTMDC_TD                 BIT(0)
#define  PTPTIVC0                  (RSWITCH2_PTPRO + 0x0020)
#define  PTPTOVC00                 (RSWITCH2_PTPRO + 0x0030)
#define  PTPTOVC10                 (RSWITCH2_PTPRO + 0x0034)
#define  PTPTOVC20                 (RSWITCH2_PTPRO + 0x0038)
#define  PTPGPTPTM00               (RSWITCH2_PTPRO + 0x0050)
#define  PTPGPTPTM10               (RSWITCH2_PTPRO + 0x0054)
#define  PTPGPTPTM20               (RSWITCH2_PTPRO + 0x0058)
#define  PTPMCCC0                  (RSWITCH2_PTPRO + 0x0200)
 #define  PTPMCCC0_MCCR            BIT(16)

enum RX_DS_CC_BIT {
    RX_DS   = 0x0fff, /* Data size */
    RX_TR   = 0x1000, /* Truncation indication */
    RX_EI   = 0x2000, /* Error indication */
    RX_PS   = 0xc000, /* Padding selection */
};

enum DIE_DT {
    /* Frame data */
    DT_FSINGLE      = 0x80,
    DT_FSTART       = 0x90,
    DT_FMID         = 0xa0,
    //DT_FEND         = 0xb8,
    DT_FEND         = 0xb0,

    /* Chain control */
    DT_LEMPTY       = 0xc0,
    DT_EEMPTY       = 0xd0,
    DT_LINKFIX      = 0x00,
    DT_LINK         = 0xe0,
    DT_EOS          = 0xf0,
    /* HW/SW arbitration */
    DT_FEMPTY       = 0x40,
    DT_FEMPTY_IS    = 0x10,
    DT_FEMPTY_IC    = 0x20,
    //DT_FEMPTY_ND    = 0x38,
    DT_FEMPTY_ND    = 0x30,
    DT_FEMPTY_START = 0x50,
    DT_FEMPTY_MID   = 0x60,
    DT_FEMPTY_END   = 0x70,

    DT_MASK         = 0xf0,
    DIE             = 0x08, /* Descriptor Interrupt Enable */
};

typedef enum {
    RSRCDB_TYPE_GWCA,
    RSRCDB_TYPE_ETHA,
    RSRCDB_TYPE_QUEUE,
} rsrcdb_type_t;

/* The Ethernet AVB descriptor definitions. */
typedef volatile struct {
    volatile uint16_t   info_ds;       /* Descriptor size bit[11:0] and INFO0 bit[15:12] */
    volatile uint8_t    die_dt;        /* ERR bit[0], DSE bit[1], AXIE bit[2], DIE bit[3]; DT bit[7:4] */
    volatile uint8_t    dptrh;         /* Descriptor pointer MSB */
    volatile uint32_t   dptrl;         /* Descriptor pointer LSW */
} __attribute__((packed)) RSWITCH2_DESC;

#define DPTR_ALIGN       4   /* Required descriptor pointer alignment */

typedef volatile struct {
    volatile uint16_t   info_ds;       /* Descriptor size bit[11:0] and INFO0 bit[15:12] */
    volatile uint8_t    die_dt;        /* ERR bit[0], DSE bit[1], AXIE bit[2], DIE bit[3]; DT bit[7:4] */
    volatile uint8_t    dptrh;         /* Descriptor pointer MSB */
    volatile uint32_t   dptrl;         /* Descriptor pointer LSW */
    volatile uint64_t   info1;
    volatile uint32_t   ts_n;          /* Timestamp */
    volatile uint32_t   ts_sl;         /* Timestamp */
} __attribute__((packed)) RSWITCH2_RX_DESC;

typedef volatile struct {
    volatile uint16_t   info_ds;       /* Descriptor size bit[11:0] and INFO0 bit[15:12] */
    volatile uint8_t    die_dt;        /* ERR bit[0], DSE bit[1], AXIE bit[2], DIE bit[3]; DT bit[7:4] */
    volatile uint8_t    dptrh;         /* Descriptor pointer MSB */
    volatile uint32_t   dptrl;         /* Descriptor pointer LSW */
    volatile uint16_t   info1;
    volatile uint8_t    ts_ras;
    volatile uint8_t    info1_3;
    volatile uint8_t    csd0;
    volatile uint8_t    csd1;
    volatile uint8_t    dv;
    volatile uint8_t    rsv;
} __attribute__((packed)) RSWITCH2_TX_DESC;

typedef struct {
    nic_config_t        cfg;
    int                 set_flow;
    int                 mii;
    int                 avb;
    int                 port0_reset;
    int                 gwca_idx;
    int                 parallel_mode;
    struct _iopkt_self  *iopkt;
    void                *dll_hdl;
    struct _iopkt_inter ient_rx;
    struct _iopkt_inter ient_tx;
    struct _iopkt_inter inter_queue;

} attach_args_t;

#define EVP_ARRAY_SIZE  (3 * RSWITCH2_IRQ_NUM)

typedef struct rswitch2_etha {
    int                 index;
    uintptr_t           addr;
    int                 mode;
    uint32_t            phy_speed;
    pthread_mutex_t     etha_mutex;
} rswitch2_etha_t;

typedef struct rswitch2_gwca {
    int                 index;
    uintptr_t           addr;
    int                 mode;
    uint32_t            speed;
    int                 rsrc_idx;
    int                 used;
} rswitch2_gwca_t;

typedef struct rswitch2_serdes {
    int                 index;
    uintptr_t           addr;
    int                 bank_off;
} rswitch2_serdes_t;


typedef struct {
    struct device           dev;
    struct ethercom         ecom;
    struct mii_data         bsd_mii;
    mdi_t                   *mdi;
    int                     set_flow;
    int                     flow_status;
    uint32_t                set_speed;
    int                     set_duplex;
    struct callout          mii_callout;
    struct callout          isr_callout;
    nic_config_t            cfg;
    nic_stats_t             stats;
    void                    *sdhook;
    int                     iid[RSWITCH2_NUM_IRQS];
    int                     irq[RSWITCH2_NUM_IRQS];
    struct _iopkt_inter     ient_rx;
    struct _iopkt_inter     ient_tx;
    struct _iopkt_inter     inter_queue;
    struct _iopkt_self      *iopkt;
    uintptr_t               base;
    int                     port;
    int                     instance;
    int (*stack_output)(struct ifnet *, struct mbuf *,
                struct sockaddr *, struct rtentry *);
    int                     fd;
    int                     tx_cleanup_needed;
    int                     tid;
    uint32_t                bat_desc_dma;
    RSWITCH2_DESC           *bat_desc;
    off64_t                 bat_desc_addr;
    int                     tx_queue_index;
    int                     rx_queue_index;
    RSWITCH2_TX_DESC       *tx_bd[NUM_TX_QUEUES];
    RSWITCH2_RX_DESC       *rx_bd[NUM_RX_QUEUES];
    uint32_t                length[NUM_RX_QUEUES];
    int                     tx_pidx[NUM_TX_QUEUES];
    int                     rx_idx[NUM_RX_QUEUES];
    uint32_t                tx_irq_mask[RSWITCH2_IRQ_NUM];
    uint32_t                rx_irq_mask[RSWITCH2_IRQ_NUM];
    int                     rx_running;
    struct sigevent         *evp[EVP_ARRAY_SIZE];
    struct sigevent         isr_event[NUM_RX_QUEUES];
    uint32_t                dis[RSWITCH2_IRQ_NUM];
    pthread_mutex_t         rx_mutex;
    struct ifqueue          rx_queue;
    intrspin_t              spinlock;
    int                     mii;
    struct cache_ctrl       cachectl;
    struct mbuf             **tx_pkts[NUM_TX_QUEUES];
    struct mbuf             **rx_pkts[NUM_RX_QUEUES];
    struct mbuf             *rpkt[NUM_RX_QUEUES];
    struct mbuf             *rpkt_tail[NUM_RX_QUEUES];
    int                     pkts_received;
    nic_mutex_t             drv_mutex;
    nic_mutex_t             ptp_mutex;
    nic_mutex_t             mdi_mutex;
    int                     avb;
    int                     coid;
    int                     chid;
    rswitch2_gwca_t         gwca;
    rswitch2_etha_t         etha;
    rswitch2_serdes_t       serdes;
    int                     etha_irq;
    int                     etha_iid;
    int                     port0_reset;
    int                     parallel_mode;
} rswitch2_dev_t;

int rswitch2_ioctl(struct ifnet *ifp, unsigned long cmd, caddr_t data);
int rswitch2_attach (struct device *parent, struct device *self, void *aux);
int rswitch2_detach (struct device *dev, int flags);
void rswitch2_reap_tx(rswitch2_dev_t *rswitch2, uint8_t queue);
void rswitch2_start(struct ifnet *ifp);
int rswitch2_output(struct ifnet *ifp, struct mbuf *m, struct sockaddr *dst, struct rtentry *rt);
int rswitch2_receive(rswitch2_dev_t *rswitch2, struct nw_work_thread *wtp, uint8_t queue);

int rswitch2_gwca_process_interrupt_tx(void *arg, struct nw_work_thread *wtp);
int rswitch2_gwca_enable_interrupt_tx(void * arg);
int rswitch2_gwca_process_interrupt_rx(void *arg, struct nw_work_thread *wtp);
int rswitch2_gwca_process_interrupt_avb_rx(void *arg, struct nw_work_thread *wtp);
int rswitch2_gwca_enable_interrupt_rx(void * arg);
int rswitch2_gwca_enable_interrupt_avb_rx (void *arg);
int rswitch2_process_queue(void *arg, struct nw_work_thread *wtp);
int rswitch2_enable_queue(void *arg);
const struct sigevent * rswitch2_etha_isr (void *arg, int iid);

const struct sigevent * rswitch2_gwca_isr_avb (void *arg, int iid);
const struct sigevent * const * rswitch2_gwca_isr(void *arg, int iid);
int rswitch2_enadis_data_irq(rswitch2_dev_t *rswitch2, int tx, bool enable);

int rswitch2_set_speed(rswitch2_dev_t *etha, int mode, uint32_t speed);
void rswitch2_update_stats(rswitch2_dev_t *rswitch2);
void rswitch2_clear_stats(rswitch2_dev_t *rswitch2);

void rswitch2_mdi_write(void *handle, uint8_t phyid, uint8_t location, uint16_t data);
uint16_t rswitch2_mdi_read(void *handle, uint8_t phyid, uint8_t location);
uint16_t rswitch2_mii_read_c45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad);
void rswitch2_mii_write_c45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad, uint16_t val);
int rswitch2_phy_init(rswitch2_dev_t *rswitch2);
void rswitch2_phy_fini(rswitch2_dev_t *rswitch2);
void rswitch2_MDI_MonitorPhy(void *arg);
int rswitch2_mediachange(struct ifnet *ifp);
int rswitch2_is_br_phy (rswitch2_dev_t *rswitch2);

void rswitch2_ptp_init(rswitch2_dev_t *rswitch2);
void rswitch2_ptp_cal(rswitch2_dev_t *rswitch2);
int rswitch2_ptp_is_eventmsg(struct mbuf *m, ptpv2hdr_t **ph);
void rswitch2_ptp_add_rx_timestamp(rswitch2_dev_t *rswitch2, ptpv2hdr_t *ph, RSWITCH2_RX_DESC *bd);
void rswitch2_ptp_add_tx_timestamp(rswitch2_dev_t *rswitch2);
int rswitch2_ptp_ioctl(rswitch2_dev_t *rswitch2, struct ifdrv *ifd);

void static inline rswitch2_modify(uintptr_t addr, int reg, uint32_t clear, uint32_t set)
{
    out32(addr + reg, (in32(addr + reg) & ~clear) | set);
}

void static inline rswitch2_etha_modify(rswitch2_etha_t *etha, int reg, uint32_t clear, uint32_t set)
{
    out32(etha->addr + reg, (in32(etha->addr + reg) & ~clear) | set);
}

#define RSWITCH2_TIMEOUT_MS    1000

static int inline rswitch2_reg_wait(uintptr_t addr, uint32_t offset, uint32_t mask, uint32_t val)
{
    int i = 0;
    while (i < RSWITCH2_TIMEOUT_MS) {
        if ((in32(addr + offset) & mask) == val)
            return EOK;
        nanospin_ns(1000);
        i++;
    }

    return -ETIMEDOUT;
}

#endif // define __RCAR_RSWITCH2_H__


