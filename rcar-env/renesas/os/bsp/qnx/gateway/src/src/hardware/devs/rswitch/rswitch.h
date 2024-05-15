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

#ifndef RSWITCH2_H
#define RSWITCH2_H

#include <unistd.h>
#include <sys/cdefs.h>
#include <sys/param.h>
// #include <sys/bus.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/module.h>
#include <sys/mutex.h>
#include <sys/rman.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <sys/taskqueue.h>
#include <sys/callout.h>
#include <net/bpf.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <net/if_dl.h>
#include <net/if_media.h>
#include <net/if_types.h>
#include <net/if_var.h>
#include <machine/bus.h>
#include <dev/fdt/fdt_common.h>
#include <dev/mii/mii.h>
#include <dev/mii/miivar.h>
#include <dev/ofw/ofw_bus.h>
#include <dev/ofw/ofw_bus_subr.h>
#include <qnx/qnx_modload.h>
#include <qnx-gen/miibus_if.h>
#include <qnx-gen/miidevs.h>
#include <qnx/ptp.h>
#include <hw/nicinfo.h>
#include <sys/kthread.h>
// #if (_NTO_VERSION < 660)
// #include <drvr/mdi.h>
// #else
// #include <netdrvr/mdi.h>
// #endif

#define BIT(x)                          (1UL << (x))
#define BITS(x,y)                       ((x) << (y))
#define BITMASK(x,y)                    (((2ULL << (y-x)) - 1) << (x))

#define VERBOSE_PTP                     (1 << 4)
#define VERBOSE_TX                      (1 << 3)
#define VERBOSE_RX                      (1 << 2)
#define VERBOSE_PHY                     (1 << 1)

#define RSWITCH2_BASE_NUM               2
#define RSWITCH2_IRQ_NUM                2
#define RSWITCH2_IRQ_REG_NUM            1
/* Change to 3 if use 3 port at the same time */
#define RSWITCH2_TSN_PORT_NUM           3

#define TX_DESC_COUNT                   127
#define RX_DESC_COUNT                   127
#define NUM_TX_DESC_WITH_LINK           (TX_DESC_COUNT + 1)
#define NUM_RX_DESC_WITH_LINK           (RX_DESC_COUNT + 1)
//#define PKT_BUF_SZ                      1538//(2048 - ETHER_CRC_LEN + sizeof(uint16_t))
#define PKT_BUF_SZ                      ETHER_MAX_LEN + ETHER_VLAN_ENCAP_LEN
#define RSWITCH2_DESC_RING_ALIGN        4
#define NUM_TX_QUEUES                   1           
#define NUM_RX_QUEUES                   1          

#define TX_DESC_SIZE                    (sizeof(RSWITCH2_TX_DESC) * NUM_TX_DESC_WITH_LINK)
#define RX_DESC_SIZE                    (sizeof(RSWITCH2_RX_DESC) * NUM_RX_DESC_WITH_LINK)

#define DBAT_ENTRY_NUM                  (NUM_TX_QUEUES + NUM_RX_QUEUES)
#define TX_QUEUE_OFFSET                 0
#define RX_QUEUE_OFFSET                 NUM_TX_QUEUES
#define TX_QUEUE_LEN                    4096
#define TX_MAX_SEGS                     32

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
#define RSWITCH2_GWCA0_HW_INDEX       3
#define RSWITCH2_GWCA1_HW_INDEX       4

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
 #define RRC_RR                     BIT(0)
  #define RCEC_RCE                  BIT(16)
#define RCEC                        (RSWITCH2_CARO + 0x0008)
#define RCDC                        (RSWITCH2_CARO + 0x000C)
#define CABPIRM                     (RSWITCH2_CARO + 0x0140)
 #define CABPIRM_BPIOG              BIT(0)
 #define CABPIRM_BPR                BIT(1)

/* GWCA */
#define GWMC                        (RSWITCH2_GWRO + 0x0000)
 #define GWMC_OPC_RESET             0
 #define GWMC_OPC_DISABLE           1
 #define GWMC_OPC_CONFIG            2
 #define GWMC_OPC_OPERATION         3
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
 #define GWMTIRM_MTIOG              BIT(0)
 #define GWMTIRM_MTR                BIT(1)
#define GWVCC                       (RSWITCH2_GWRO + 0x0130)
 #define GWVCC_VEM_SC_TAG           (0x3 << 16)
#define GWTTFC                      (RSWITCH2_GWRO + 0x0138)
#define GWMDNC                      (RSWITCH2_GWRO + 0x01A0)
 #define GWMDNC_TSDMN(val)          ((val & 0x1f) << 16)
 #define GWMDNC_RXDMN(val)          ((val & 0x1f) << 0)
 #define GWMDNC_TXDMN(val)          ((val & 0x1f) << 8)
#define GWDCBAC0                    (RSWITCH2_GWRO + 0x0194)
 #define GWDCBAC0_DCBAUP             (0xff)
#define GWDCBAC1                    (RSWITCH2_GWRO + 0x0198)
#define GWIICBSC                    (RSWITCH2_GWRO + 0x019C)
#define GWTRC(i)                    (RSWITCH2_GWRO + 0x0200 + (i) * 0x04)
#define GWARIRM                     (RSWITCH2_GWRO + 0x0380)
 #define GWARIRM_ARIOG              BIT(0)
 #define GWARIRM_ARR                BIT(1)
#define GWDCC(ch)                   (RSWITCH2_GWRO + 0x0400 + (ch) * 4)
 #define GWDCC_BALR                 BIT(24)
 #define GWDCC_DCP(q, idx)          ((q + (idx * 2)) << 16)
 #define GWDCC_DQT                  BIT(11)
 #define GWDCC_ETS                  BIT(9)
 #define GWDCC_EDE                  BIT(8)
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
#define FWPC0(i)                    (RSWITCH2_FWRO + 0x0100 + (i) * 0x10)
 #define FWPC0_LTHTA                BIT(0)
 #define FWPC0_IP4UE                BIT(3)
 #define FWPC0_IP4TE                BIT(4)
 #define FWPC0_IP4OE                BIT(5)
 #define FWPC0_L2SE                 BIT(9)
 #define FWPC0_IP4EA                BIT(10)
 #define FWPC0_IPDSA                BIT(12)
 #define FWPC0_IPHLA                BIT(18)
 #define FWPC0_MACSDA               BIT(20)
 #define FWPC0_MACHLA               BIT(26)
 #define FWPC0_MACHMA               BIT(27)
 #define FWPC0_VLANSA               BIT(28)
 #define FWPC0_DEFAULT              (FWPC0_LTHTA | FWPC0_IP4UE | FWPC0_IP4TE | \
                                    FWPC0_IP4OE | FWPC0_L2SE | FWPC0_IP4EA | \
                                    FWPC0_IPDSA | FWPC0_IPHLA | FWPC0_MACSDA | \
                                    FWPC0_MACHLA | FWPC0_MACHMA | FWPC0_VLANSA)
#define FWPC1(i)                    (RSWITCH2_FWRO + 0x104 + (i) * 0x10)
#define FWPC2(i)                    (RSWITCH2_FWRO + 0x108 + (i) * 0x10)
#define FWPBFC(i)                   (RSWITCH2_FWRO + 0x4A00 + (i) * 0x10)
#define FWPBFCSDC(j, i)             (RSWITCH2_FWRO + 0x4A04 + (i) * 0x10 + (j) * 0x04)

/* ETHA */
#define EAMC                        (RSWITCH2_TARO + 0x0000)
 #define EAMC_OPC_RESET             0
 #define EAMC_OPC_DISABLE           1
 #define EAMC_OPC_CONFIG            2
 #define EAMC_OPC_OPERATION         3
 #define EAMS_OPS_MASK              EAMC_OPC_OPERATION
#define EAMS                        (RSWITCH2_TARO + 0x0004)
#define EATDQDC(q)                  (RSWITCH2_TARO + 0x0060 + (q) * 0x04)
 #define EATDQDC_DQD                (NUM_TX_DESC_WITH_LINK)
#define EATTFC                      (RSWITCH2_TARO + 0x0138)
#define EATASRIRM                   (RSWITCH2_TARO + 0x03E4)
 #define EATASRIRM_TASRIOG          BIT(0)
 #define EATASRIRM_TASRR            BIT(1)
 #define EAIRC                      (RSWITCH2_TARO + 0x0010)

/* RMAC */
#define MPSM                        (RSWITCH2_RMRO + 0x0000)
 #define MPSM_MFF_C45               BIT(2)
 #define MPSM_MFF_C22               (0x0 << 2)
 #define MPSM_PSME                  BIT(0)
 #define MPSM_POP_MASK              (0x03 << 13)
 #define MPSM_PRA_MASK              (0x1f << 8)
 #define MPSM_PDA_MASK              (0x1f << 3)
 #define MPSM_PRD_MASK              (0xffff << 16)
#define MPIC                        (RSWITCH2_RMRO + 0x0004)
 #define MPIC_PIS_MASK              0x07
 #define MPIC_PIS_MII               0x00
 #define MPIC_PIS_GMII              0x02
 #define MPIC_PIS_XGMII             0x04
 #define MPIC_LSC_MASK              (0x07 << 3)
 #define MPIC_LSC_100               (0x01 << 3)
 #define MPIC_LSC_1000              (0x02 << 3)
 #define MPIC_LSC_2500              (0x03 << 3)
 #define MPIC_PSMCS_SHIFT           16
 #define MPIC_PSMCS_MASK            (0x7f << 16)
 #define MPIC_PSMCS(val)            ((val) << MPIC_PSMCS_SHIFT)
 #define MPIC_PSMHT_SHIFT           24
 #define MPIC_PSMHT_MASK            (0x07 << 24)
 #define MPIC_PSMHT(val)            ((val) << MPIC_PSMHT_SHIFT)
 #define MPIC_MDC_CLK_SET           (0x06050000)
#define MRMAC0                      (RSWITCH2_RMRO + 0x0084)
#define MRMAC1                      (RSWITCH2_RMRO + 0x0088)
#define MRAFC                       (RSWITCH2_RMRO + 0x008C)
#define MRSCE                       (RSWITCH2_RMRO + 0x0090)
#define MRSCP                       (RSWITCH2_RMRO + 0x0094)
#define MLVC                        (RSWITCH2_RMRO + 0x0180)
 #define MLVC_PLV                   BIT(16)
 #define MLVC_LVT                   0x09
#define MLBC                        (RSWITCH2_RMRO + 0x0188)
#define MXGMIIC                     (RSWITCH2_RMRO + 0x0190)
#define MPCH                        (RSWITCH2_RMRO + 0x0194)
#define MANM                        (RSWITCH2_RMRO + 0x019C)
#define MEIS                        (RSWITCH2_RMRO + 0x0200)
#define MMIS0                       (RSWITCH2_RMRO + 0x0210)
 #define MMIS0_LVSS                 0x02
#define MMIS1                       (RSWITCH2_RMRO + 0x0220)
 #define MMIS1_PAACS                BIT(2) /* Address */
 #define MMIS1_PWACS                BIT(1) /* Write */
 #define MMIS1_PRACS                BIT(0) /* Read */
 #define MMIS1_CLEAR_FLAGS          0xf

/* Serdes */
#define RSWITCH2_SERDES_BASE                0xE6444000
#define RSWITCH2_SERDES_OFFSET              0x0400
#define RSWITCH2_SERDES_BANK_OFFSET         0x03fc
#define RSWITCH2_SERDES_FUSE_OVERRIDE(n)    (0x2600 - (n) * 0x400)
#define RSWITCH2_SERDES_SIZE                0x2000

#define SERDES_SGMII                        0
#define SERDES_USXGMII                      1


/* gPTP */
/* Currently PTP uses queue 0 */
#define NUM_PTP_TX_QUEUES 1
#define PTP_TX_BUF_LEN 64
#define PTP_TX_MSGTYPE 4
#define RSWITCH2_RX_TIMESTAMP_BUF_SZ  64
#define RSWITCH2_TX_TIMESTAMP_BUF_SZ  16
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
/* OTHERS */

//Number of Retries
#define NUM_AUTO_NEGO_RETRIES       (1000)
#define NUM_RSWITCH2_RESET_RETRIES  (10)
#define NUM_PHY_WAIT_RETRIES        (10)
#define NUM_RSWITCH2_WAIT_SETMODE   (100)

#define MII_MODEL_MV88Q2110         0x18
#define MII_MODEL_BCM89811          0x2

#define PHY_SPEED_10M               (10000L)
#define PHY_SPEED_100M              (100000L)
#define PHY_SPEED_1000M             (1000000L)
#define PHY_SPEED_2500M             (2500000L)

/* Clause 45 extended register definitions */
/* MDIO Manageable Devices (MMDs). */
#define MDI_MMD_PMA_PMD		1	/* Physical Medium Attachment */
					            /* Physical Medium Dependent */
#define MDI_MMD_WIS		    2	/* WAN Interface Sublayer */
#define MDI_MMD_PCS		    3	/* Physical Coding Sublayer */
#define MDI_MMD_PHY_XS		4	/* PHY Extender Sublayer */
#define MDI_MMD_DTE_XS		5	/* DTE Extender Sublayer */
#define MDI_MMD_TC		    6	/* Transmission Convergence */
#define MDI_MMD_AN		    7	/* Auto-Negotiation */
#define MDI_MMD_C22_EXT		29	/* Clause 22 extension */
#define MDI_MMD_VEND_1		30	/* Vendor specific 1 */
#define MDI_MMD_VEND_2		31	/* Vendor specific 2 */

typedef enum {
    RSRCDB_TYPE_GWCA,
    RSRCDB_TYPE_ETHA,
    RSRCDB_TYPE_QUEUE,
} rsrcdb_type_t;

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

enum RX_DS_CC_BIT {
    RX_DS       = 0x0fff, /* Data size */
    RX_TR       = 0x1000, /* Truncation indication */
    RX_EI       = 0x2000, /* Error indication */
    RX_PS       = 0xc000, /* Padding selection */
};

/* E-MAC status code */
enum MSC_BIT {
    MSC_CRC     = 0x01, /* Frame CRC error */
    MSC_RFE     = 0x02, /* Frame reception error (flagged by PHY) */
    MSC_RTSF    = 0x04, /* Frame length error (frame too short) */
    MSC_RTLF    = 0x08, /* Frame length error (frame too long) */
    MSC_FRE     = 0x10, /* Fraction error (not a multiple of 8 bits) */
    MSC_CRL     = 0x20, /* Carrier lost */
    MSC_CEEF    = 0x40, /* Carrier extension error */
    MSC_MC      = 0x80, /* Multicast frame reception */
};

/* The Ethernet RSWITCH2 descriptor definitions. */
typedef  struct {
    uint16_t   info_ds;       /* Descriptor size bit[11:0] and INFO0 bit[15:12] */
    uint8_t    die_dt;        /* ERR bit[0], DSE bit[1], AXIE bit[2], DIE bit[3]; DT bit[7:4] */
    uint8_t    dptrh;         /* Descriptor pointer MSB */
    uint32_t   dptrl;         /* Descriptor pointer LSW */
} RSWITCH2_DESC;

typedef struct {
    uint16_t   info_ds;       /* Descriptor size bit[11:0] and INFO0 bit[15:12] */
    uint8_t    die_dt;        /* ERR bit[0], DSE bit[1], AXIE bit[2], DIE bit[3]; DT bit[7:4] */
    uint8_t    dptrh;         /* Descriptor pointer MSB */
    uint32_t   dptrl;         /* Descriptor pointer LSW */
    uint64_t   info1;
    uint32_t   ts_n;          /* Timestamp */
    uint32_t   ts_sl;         /* Timestamp */
} RSWITCH2_RX_DESC;

typedef struct {
    uint16_t   info_ds;       /* Descriptor size bit[11:0] and INFO0 bit[15:12] */
    uint8_t    die_dt;        /* ERR bit[0], DSE bit[1], AXIE bit[2], DIE bit[3]; DT bit[7:4] */
    uint8_t    dptrh;         /* Descriptor pointer MSB */
    uint32_t   dptrl;         /* Descriptor pointer LSW */
    uint16_t   info1;
    uint8_t    ts_ras;
    uint8_t    info1_3;
    uint8_t    csd0;
    uint8_t    csd1;
    uint8_t    dv;
    uint8_t    rsv;
} RSWITCH2_TX_DESC;


#define MII_NUM_REGS            29
#define RFLR_RFL_MIN            1518  /* Min rx frame length: 1518 bytes */
#define EVP_ARRAY_SIZE          3

#define PHY_MODE_MII            (0)
#define PHY_MODE_RMII           (0x01)
#define PHY_MODE_GMII           (0x02)
#define PHY_MODE_RGMII          (0x03)

#define DPTR_ALIGN              4   /* Required descriptor pointer alignment */
#define RSWITCH2_ALIGN          128
#define ALIGN_MASK(x, mask)     (((x) + (mask)) & ~(mask))
#define PKT_ALIGN(x, a)         ALIGN_MASK(x, (int)(a) - 1)
#define PKT_BUF_SIZE            PKT_ALIGN(PKT_BUF_SZ, DPTR_ALIGN)

#define WATCHDOG_TIMEOUT_SECS   10

typedef struct rswitch2_etha {
    int                 index;
    uint32_t            addr_offset;
    int                 mode;
    struct resource     *serdes_res;
    uint32_t            phy_speed;
    pthread_mutex_t     etha_mutex;
} rswitch2_etha_t;

typedef struct rswitch2_gwca {
    int                 index;
    uint32_t            addr_offset;
    int                 mode;
    uint32_t            speed;
    int                 rsrc_idx;
    int                 used;
} rswitch2_gwca_t;

struct rswitch2_bufmap {
    bus_dmamap_t map;
    struct mbuf *m;
};

typedef struct _rswitch2_dev {
    device_t                dev;
    struct resource         *res[RSWITCH2_BASE_NUM];
    struct resource         *irq_res[RSWITCH2_IRQ_NUM];
    void                    *intr_cookie[RSWITCH2_IRQ_NUM];
    int                     mactype;
    device_t                miibus;
    struct mii_data         *mii_softc;
    struct ifnet            *ifp;
    int                     if_flags;
    struct mtx              mtx;
    struct mtx              tx_mtx;
    struct mtx              rx_mtx;
    struct callout          mii_callout;
    boolean_t               link_is_up;
    boolean_t               is_attached;
    boolean_t               is_detaching;
    uint32_t                phy_mode;
    /* RX */
    RSWITCH2_RX_DESC        *rx_ring[NUM_RX_QUEUES];
    bus_addr_t              rx_ring_paddr[NUM_TX_QUEUES];
    bus_dma_tag_t           rx_desc_tag[NUM_TX_QUEUES];
    bus_dmamap_t            rx_desc_map[NUM_TX_QUEUES];
    bus_dma_tag_t           rx_buf_tag[NUM_RX_QUEUES];
    struct rswitch2_bufmap  rx_buf_map[NUM_RX_QUEUES][RX_DESC_COUNT];
    /* TX */
    RSWITCH2_TX_DESC        *tx_ring[NUM_TX_QUEUES];
    bus_addr_t              tx_ring_paddr[NUM_TX_QUEUES];
    bus_dma_tag_t           tx_desc_tag[NUM_TX_QUEUES];
    bus_dmamap_t            tx_desc_map[NUM_TX_QUEUES];
    bus_dma_tag_t           tx_buf_tag[NUM_TX_QUEUES];
    struct rswitch2_bufmap  tx_buf_map[NUM_TX_QUEUES][TX_DESC_COUNT];
    struct buf_ring         *br;
    struct mbuf             *rpkt[NUM_RX_QUEUES];
    struct mbuf             *rpkt_tail[NUM_RX_QUEUES];
    uint32_t                length[NUM_RX_QUEUES];
    uint32_t                tx_cidx[NUM_TX_QUEUES];
    uint32_t                tx_pidx[NUM_TX_QUEUES];
    uint32_t                rx_idx[NUM_RX_QUEUES];
    uint32_t                tx_irq_mask[RSWITCH2_IRQ_REG_NUM];
    uint32_t                rx_irq_mask[RSWITCH2_IRQ_REG_NUM];
    uint32_t                dis[RSWITCH2_IRQ_REG_NUM];
    int                     pkts_received;
    uint16_t                *phy_regs;
    int                     txc_delay;
    int                     rxc_delay;
    int                     tx_mtx_is_up;
    struct taskqueue        *tq;
    struct task             tx;
    int                     device_index;
    char                    options[1024];
    uint32_t                flags;
    uint32_t                media_rate;
    int                     mii;
    uint8_t                 current_address[ETHER_ADDR_LEN];
    int                     tx_watchdog_count;
    int                     verbose;
    rswitch2_gwca_t         gwca;
    rswitch2_etha_t         etha;
    int                     port;
    int                     instance;
    int                     parallel_mode;
    int                     tx_queue_index;
    int                     rx_queue_index;
    struct mii_data         bsd_mii;
    int                     set_flow;
    int                     flow_status;
    uint32_t                set_speed;
    int                     set_duplex;
    int                     duplex;
    nic_config_t            cfg;
    int                     port0_reset;
} rswitch2_dev_t;

/* Currently PTP uses queue 0 */
#define NUM_PTP_TX_QUEUES   1
#define PTP_TX_BUF_LEN      64
#define PTP_TX_MSGTYPE      4

ptpv2hdr_t  tx_ph[NUM_PTP_TX_QUEUES][PTP_TX_BUF_LEN * PTP_TX_MSGTYPE];
int rswitch2_ioctl(struct ifnet *ifp, unsigned long cmd, caddr_t data);

void rswitch2_media_status(struct ifnet * ifp, struct ifmediareq *ifmr);
int  rswitch2_media_change(struct ifnet * ifp);
void rswitch2_miibus_statchg(device_t dev);
void rswitch2_miibus_linkchg(device_t dev);
void rswitch2_miibus_mediainit(device_t dev);
void rswitch2_clear_stats (rswitch2_dev_t *rswitch2);
int  rswitch2_miibus_read_reg(device_t dev, int phy, int reg);
int  rswitch2_miibus_write_reg(device_t dev, int phy, int reg, int val);
int  rswitch2_phy_init(rswitch2_dev_t *rswitch2);
void rswitch2_phy_fini(rswitch2_dev_t *rswitch2);

int  rswitch2_transmit(struct ifnet *ifp, struct mbuf *m);
void rswitch2_tq_tx(void *arg, int pending);
void rswitch2_qflush(struct ifnet *ifp);
void rswitch2_start(struct ifnet *ifp);

void rswitch2_setup_rxfilter(rswitch2_dev_t *rswitch2);
void rswitch2_rxfinish_locked(rswitch2_dev_t *rswitch2, int queue);
int  rswitch2_setup_rxbuf(rswitch2_dev_t *rswitch2, int queue, int idx);

void rswitch2_ptp_interrupt (rswitch2_dev_t *rswitch2);
int  rswitch2_enadis_data_irq(rswitch2_dev_t *rswitch2, int tx, bool enable);

void rswitch2_init_locked(rswitch2_dev_t *rswitch2);
void rswitch2_stop_locked(rswitch2_dev_t *rswitch2);
int  rswitch2_gwca_enable_interrupt_rx(void * arg);
int  rswitch2_gwca_enable_interrupt_tx(void * arg);
void rswitch2_gwca_isr(void *arg);
void rswitch2_gwca_rx_isr(void *arg);
void rswitch2_gwca_tx_isr(void *arg);
void rswitch2_gwca_ts_isr(void *arg);

void rswitch2_hw_stop (rswitch2_dev_t *rswitch2);

void rswitch2_ptp_init(rswitch2_dev_t *rswitch2);
void rswitch2_ptp_cal(rswitch2_dev_t *rswitch2);
int  rswitch2_ptp_is_eventmsg(struct mbuf *m, ptpv2hdr_t **ph);
void rswitch2_ptp_add_rx_timestamp(rswitch2_dev_t *rswitch2, ptpv2hdr_t *ph, RSWITCH2_RX_DESC *bd);
void rswitch2_ptp_add_tx_timestamp(rswitch2_dev_t *rswitch2);
int rswitch2_ptp_ioctl(rswitch2_dev_t *rswitch2, struct ifdrv *ifd);

int rswitch2_set_speed(rswitch2_dev_t *etha, int mode, uint32_t speed);

void rswitch2_mdi_write(void *handle, uint8_t phyid, uint8_t location, uint16_t data);
uint16_t rswitch2_mdi_read(void *handle, uint8_t phyid, uint8_t location);
uint16_t rswitch2_mii_read_c45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad);
void rswitch2_mii_write_c45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad, uint16_t val);
int rswitch2_phy_init(rswitch2_dev_t *rswitch2);
void rswitch2_phy_fini(rswitch2_dev_t *rswitch2);
int rswitch2_mediachange(struct ifnet *ifp);

int mii_attach_nw(device_t dev, device_t *miibus, if_t ifp,
    ifm_change_cb_t ifmedia_upd, ifm_stat_cb_t ifmedia_sts, int capmask,
    int phyloc, int offloc, int flags);

int rswitch2_rsrcdb_create_resources(rswitch2_dev_t *rswitch2);
int rswitch2_rsrcdb_attach_resources(rswitch2_dev_t *rswitch2, const rsrcdb_type_t type, const int length);
int rswitch2_rsrcdb_detach_resources(rswitch2_dev_t *rswitch2, const rsrcdb_type_t type, const int start, const int length);
int rswitch2_rsrcdb_query_gwca_resources(rswitch2_dev_t *rswitch2, int for_reset);
int rswitch2_rsrcdb_check_parallel_mode(rswitch2_dev_t *rswitch2);
int rswitch2_rsrcdb_check_gwca_used(rswitch2_dev_t *rswitch2);

void static inline rswitch2_modify(struct resource *res, int reg, uint32_t clear, uint32_t set)
{
    bus_write_4(res, reg, (bus_read_4(res, reg) & ~clear) | set);
}

void static inline rswitch2_etha_modify(struct resource *res, int reg, uint32_t clear, uint32_t set)
{
    bus_write_4(res, reg, (bus_read_4(res, reg) & ~clear) | set);
}

#define RSWITCH2_TIMEOUT_MS    1000
static int inline rswitch2_reg_wait(struct resource *res, int reg, uint32_t mask, uint32_t value)
{

    int i = 0;
    while (i < RSWITCH2_TIMEOUT_MS) {
          if ((bus_read_4(res,  reg) & mask) == value)
            return EOK;
        nanospin_ns(1000);
        i++;
    }

    return -ETIMEDOUT;
}

#endif // define RSWITCH2_H
