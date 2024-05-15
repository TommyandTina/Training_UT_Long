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

#ifndef __RCAR_RTSNES_H__
#define __RCAR_RTSNES_H__

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
 * On an rtsnes we have 4 queues of Tx and 18 queues of Rx.
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

#define NUM_TX_DESC                   1023
#define NUM_RX_DESC                   1023
#define NUM_TX_DESC_WITH_LINK         (NUM_TX_DESC + 1)
#define NUM_RX_DESC_WITH_LINK         (NUM_RX_DESC + 1)

#define NUM_TX_QUEUES                 1
#define NUM_RX_QUEUES                 1
#define DBAT_ENTRY_NUM                (NUM_TX_QUEUES + NUM_RX_QUEUES)
#define TX_QUEUE_OFFSET               0
#define RX_QUEUE_OFFSET               0

#define TX_QUEUE_ADDR_OFFSET          sizeof(RTSNES_DESC) * TX_QUEUE_OFFSET
#define RX_QUEUE_ADDR_OFFSET          sizeof(RTSNES_DESC) * RX_QUEUE_OFFSET

#define PKT_BUF_SZ                    ETHER_MAX_LEN + ETHER_VLAN_ENCAP_LEN
#define RTSNES_ALIGN                  128
#define ALIGN_MASK(x, mask)           (((x) + (mask)) & ~(mask))
#define ALIGN(x, a)                   ALIGN_MASK(x, (typeof(x))(a) - 1)

#define PHY_SPEED_10M                 (10 * 1000)
#define PHY_SPEED_100M                (100 * 1000)
#define PHY_SPEED_1000M               (1000 * 1000)
#define PHY_SPEED_2500M               (2500 * 1000)

/* PTP */
#define RTSNES_RX_TIMESTAMP_BUF_SZ    64
#define RTSNES_TX_TIMESTAMP_BUF_SZ    16

#define RTSNES_QUIESCE_PULSE            _PULSE_CODE_MINAVAIL
#define RTSNES_RX_PULSE                 (RTSNES_QUIESCE_PULSE + 1)
#define RTSNES_RX_THREAD_NAME           "rtsnes Rx"

#define RTSNES_PORT_MAX                 1
#define RTSNES_NUM_IRQS                 2
#define RTSNES_NUM_QUEUE_GRP            ((DBAT_ENTRY_NUM * RTSNES_PORT_MAX / 32) + 1)

/* Define for V4H */
#define RCAR_TSNES_REG_BASE             0xE6460000
#define RCAR_TSNES_REG_SIZE             0x7000
#define TSNES_AXIBMI_BASE_OFFSET        0x0000
#define TSNES_AXIBMI_REG_SIZE           0x1000
#define TSNES_MHD_BASE_OFFSET           0x1000
#define TSNES_MHD_REG_SIZE              0x1000
#define TSNES_RMS_BASE_OFFSET           0x2000
#define TSNES_RMAC_BASE_OFFSET          0x3800
#define TSNES_RMAC_REG_SIZE             0x2000
#define RCAR_TSNES_PTP_REG_BASE         0xE6449000
#define RCAR_TSNES_PTP_REG_SIZE         0x500

#define TSNES_MII		                (0)
#define TSNES_RMII		                (0x01)
#define TSNES_GMII		                (0x02)
#define TSNES_RGMII		                (0x03)
    
/* Define for MHD Begin */
#define TSNMHD_OCR                      0x000 // Operation Control Register
    #define TSNMHD_OCR_OPC_DISABLE      0x0
    #define TSNMHD_OCR_OPC_CONFIG       0x1
    #define TSNMHD_OCR_OPC_OPERATION    0x2
#define TSNMHD_OSR                      0x004 // Operation Status Register
    #define TSNMHD_OSR_OPS_DISABLE      0x1
    #define TSNMHD_OSR_OPS_CONFIG       0x2
    #define TSNMHD_OSR_OPS_OPERATION    0x4
    #define TSNMHD_OSR_OPS_MASK         0xF
#define TSNMHD_SWR                      0x008
    #define TSNMHD_SWR_SWR               0x1
#define TSNMHD_GIE                      0x014 // General Interrupt Enable register
#define TSNMHD_GID                      0x018 // General Interrupt Disable
#define TSNMHD_TIS1                     0x020 // Transmission Interrupt Status
#define TSNMHD_TIS2                     0x030 // Transmission Interrupt Status register 2 
#define TSNMHD_TIE1                     0x024 // Transmission Interrupt Enable register 1 
#define TSNMHD_TID1                     0x028 // Transmission Interrupt Disable register 1 
#define TSNMHD_TIE2                     0x034 // Transmission Interrupt Enable register 2 
#define TSNMHD_TID2                     0x038 // Transmission Interrupt Disable register 2
#define TSNMHD_RIE                      0x044 // Reception Interrupt Enable register
#define TSNMHD_RID                      0x048 // Reception Interrupt Disable register
#define TSNMHD_TGC1                     0x050 // Transmission General Configuration Register1
    #define TSNMHD_TGC1_TQTM_SFM		0xff00
    #define TSNMHD_TGC1_STTV_DEFAULT	0x03
#define TSNMHD_TGC2                     0x054 // Transmission General Configuration Register2
#define TSNMHD_TCR1                     0x080 // Transmission Configuration Register 1
#define TSNMHD_TCR2                     0x084 // Transmission Configuration Register 2
#define TSNMHD_TCR3                     0x088 // Transmission Configuration Register 3
#define TSNMHD_TCR4                     0x08C // Transmission Configuration Register 4
#define TSNMHD_TMS0		                0x090
    #define TSNMHD_TMS_MFS_MAX		    0x2800
#define TSNMHD_RGC                      0x0D0 // Reception General Configuration register
#define TSNMHD_TPTPC                    0x1F0 // TAS gPGP timer configuration
#define TSNMHD_TTML                     0x1F4 // TAS transmission minimum latency
#define TSNMHD_TTJ                      0x1F8 // TAS transmission jitter 
#define TSNMHD_CACC                     0x260 // CBS Admin common configuration
#define TSNMHD_CCS                      0x264 // CBS configuration setting
#define TSNMHD_CAIV(q)                  (0x270 + q*4) // CBS Admin Increment Value q CAIVq (q=0..7)
#define TSNMHD_CAUL(q)                  (0x290 + q*4) //CBS Admin Upper Limit q CAULq CAULq(q=0..7)
#define TSNMHD_QSMSMC                   0x610 // Qci stream msdu mode configuration
#define TSNMHD_QSGTMC                   0x614 // Qci stream gate mode configuration
#define TSNMHD_DCR                      0xC00 // Debug Configuration Register DCR
#define TSNMHD_TDRRR                    0xC20 // TX Data RAM Read Request
#define TSNMHD_TDRRD                    0xC24 // TX Data RAM Read Data
#define TSNMHD_TCRRR                    0xC28 // TX Ctrl RAM Read Request
#define TSNMHD_TCRRD                    0xC2C // TX Ctrl RAM Read Data
#define TSNMHD_RDRRR                    0xC40 // RX Data RAM Read Request
#define TSNMHD_RDRRD                    0xC44 // RX Data RAM Read Data
#define TSNMHD_RCRRR                    0xC48 // RX Ctrl RAM Read Request
#define TSNMHD_RCRRD                    0xC4C // RX Ctrl RAM Read Data
#define TSNMHD_TCF(p)                   (0x070 + p*4) // Transmission Ctrl fifo Size p (p = 0..3) 
#define TSNMHD_TFS(p)                   (0x060 + p*4) //Transmission Fifo Size p (p = 0..3) 
#define TSNMHD_TSR1                     0x0B0 // Transmission Status Register 1
#define TSNMHD_TSR2                     0x0B4 // Transmission Status Register 2
#define TSNMHD_TSR3                     0x0B8 // Transmission Status Register 3
#define TSNMHD_TSR4                     0x0BC // Transmission Status Register 4
#define TSNMHD_TSR5                     0x0C0 // Transmission Status Register 5
#define TSNMHD_TMS(q)                   (0x090 + q*4) // Transmission Maximum Frame Size q (q = 0..7) 
#define TSNMHD_SIS                      0x00C // System Interrupt Status register
#define TSNMHD_TGS                      0x20C // TAS GateStates
#define TSNMHD_TCD                      0xC30 // TAS CBD Debug
#define TSNMHD_CACC                     0x260 // TCBS Admin common configuration 
#define TSNMHD_CCS                      0x264 // CBS configuration setting
#define TSNMHD_TCIS                     0x1E0 // TAS CBS Interrupt Status
/* Define for MHD End */

/* RMAC System */
#define TSNRMS_CFCR0                    0x0800
    #define TSNRMS_CFCR_SDID(n)		    ((n) << 16)
#define TSNRMS_FMSCR                    0x0C10
    #define TSNRMS_FMSCR_FMSIE(n)		((n) << 0)

/* Define for RMAC Begin */
#define TSNRMAC_MCC                     (0x0000)
#define TSNRMAC_MPSM                    (0x0010)
    #define TSNRMAC_MPSM_PSMAD_READ      (0 << 1)
    #define TSNRMAC_MPSM_PSMAD_WRITE     (1 << 1)
    #define TSNRMAC_MPSM_PSME            BIT(0)
#define TSNRMAC_MPIC                    (0x0014)
    #define TSNRMAC_MPIC_PIS_MII         0
    #define TSNRMAC_MPIC_PIS_RMII        1
    #define TSNRMAC_MPIC_PIS_GMII        2
    #define TSNRMAC_MPIC_PIS_RGMII       3
    #define TSNRMAC_MPIC_PIS_MASK        3
    #define TSNRMAC_MPIC_LSC_10M         (0 << 2) 
    #define TSNRMAC_MPIC_LSC_100M        (1 << 2) 
    #define TSNRMAC_MPIC_LSC_1000M       (2 << 2) 
    #define TSNRMAC_MPIC_LSC_2500M       (3 << 2) 
    #define TSNRMAC_MPIC_LSC_MASK        (3 << 2)
    #define TSNRMAC_MPIC_PSMCS_MASK      (0x7f << 16)
    #define TSNRMAC_MPIC_PSMCS_DEFAULT   (0x1a << 16)
    #define TSNRMAC_MPIC_PSMHT_MASK      (0x07 << 24)
    #define TSNRMAC_MPIC_PSMHT_DEFAULT	 (0x07 << 24)
    #define TSNRMAC_MPIC_PSMCS(val)      ((val) << 16)
    #define TSNRMAC_MPIC_PSMHT(val)      ((val) << 24)
#define TSNRMAC_MRMAC0                  0x0084
#define TSNRMAC_MRMAC1                  0x0088
#define TSNRMAC_MRAFC                   0x008C
#define TSNRMAC_MRSCE                   0x0090
#define TSNRMAC_MRSCP                   0x0094
#define TSNRMAC_MLVC                    0x0340
    #define  TSNRMAC_MLVC_PLV            BIT(17)
#define TSNRMAC_MLBC                    0x0360 // MAC loopback configuration
#define TSNRMAC_MMIS0                   0x0510
#define TSNRMAC_MMIS1                   0x0520
    #define TSNRMAC_MMIS1_PWACS          BIT(1) /* Write */
    #define TSNRMAC_MMIS1_PRACS          BIT(0) /* Read */
#define TSNRMAC_MMIS2                   0x0530
#define TSNRMAC_MXMS                    0x0600
#define TSNRMAC_MRFSCE                  0x009C  // MAC reception frame size configuration for e-frames
#define TSNRMAC_MRFSCP                  0x00A0  // MAC reception frame size configuration for p-frames
#define TSNRMAC_MTRC                    0x00A4  // MAC Timestamp reception configuration register
#define TSNRMAC_MMPFTCT                 0x0410  // MAC manual pause frame transmit counter
#define TSNRMAC_MAPFTCT                 0x0414  // MAC automatic pause frame transmit counter
#define TSNRMAC_MPFRCT                  0x0418  // MAC pause frame receive counter
#define TSNRMAC_MFCICT                  0x041C  // MAC false carrier indication counter
#define TSNRMAC_MEEECT                  0x0420  // MAC energy efficient ethernet counter
#define TSNRMAC_MEIS                    0x0500  // MAC error interrupt status
#define TSNRMAC_MGMR                    0x0400  // MAC global monitoring register
#define TSNRMAC_TXBCE                   0x1100  // Transmitted byte counter E-frames refer from RMACS
#define TSNRMAC_TXBCP                   0x1104  // Transmitted byte counter P-frames refer from RMACS
#define TSNRMAC_TGFCE                   0x1108  // Transmitted good frame counter E-frames refer from RMACS
#define TSNRMAC_TGFCP                   0x110C  // Transmitted good frame counter P-frames refer from RMACS
#define TSNRMAC_TBFC                    0x1110  // Transmitted broadcast frame counter  refer from RMACS
#define TSNRMAC_TMFC                    0x1114  // Transmitted multicast frame counter  refer from RMACS
#define TSNRMAC_TEFC                    0x111C  // Transmitted error frame count  refer from RMACS
/* Define for RMAC End */

/* Define for AXIBMI Begin */
#define TSNAXIBMI_AXIWC                 0x0000 // AXI Bus Write Configuration
    #define TSNAXIBMI_AXIWC_DEFAULT		(0xffff)
    #define TSNAXIBMI_AXIWC_WTEON(n)    (((n) & 0xF) << 0)
    #define TSNAXIBMI_AXIWC_WTPON(n)    (((n) & 0xF) << 4)
    #define TSNAXIBMI_AXIWC_WREON(n)    (((n) & 0xF) << 8)
    #define TSNAXIBMI_AXIWC_WRPON(n)    (((n) & 0xF) << 12)
    #define TSNAXIBMI_AXIWC_WTSON(n)    (((n) & 0xF) << 16)
#define TSNAXIBMI_AXIRC                 0x0004 // AXI Bus Read Configuration
    #define TSNAXIBMI_AXIRC_DEFAULT		(0xffff)
    #define TSNAXIBMI_AXIRC_RTEON(n)    (((n) & 0xF) << 0)
    #define TSNAXIBMI_AXIRC_RTPON(n)    (((n) & 0xF) << 4)
    #define TSNAXIBMI_AXIRC_RREON(n)    (((n) & 0xF) << 8)
    #define TSNAXIBMI_AXIRC_RRPON(n)    (((n) & 0xF) << 12)
    #define TSNAXIBMI_AXIRC_RTSON(n)    (((n) & 0xF) << 16)

#define TSNAXIBMI_TDPC                  0x0010
#define TSNAXIBMI_TFT                   0x0090 // TX Descriptor Chain Type
#define TSNAXIBMI_TATLS0                0x00A0 // TX Descriptor Address Table Learning Setting 0
    #define TSNAXIBMI_TATLS0_TEDE       BIT(1)
#define TSNAXIBMI_TATLS1                0x00A4 // TX Descriptor Address Table Learning Setting 1
#define TSNAXIBMI_TATLR                 0x00A8 // TX Descriptor Address Table Learning Result
    #define TSNAXIBMI_TATLR_TATL         (1 << 31)
    #define TSNAXIBMI_TATLR_TATLO        (1 << 1)
    #define TSNAXIBMI_TATLR_TATLF        (1 << 0)
#define TSNAXIBMI_RATLS0                0x00B0 // RX Descriptor Address Table Learning Setting 0
    #define TSNAXIBMI_RATLS0_REDE       BIT(3)
    #define TSNAXIBMI_RATLS0_RETS       BIT(2)
#define TSNAXIBMI_RATLS1                0x00B4 // RX Descriptor Address Table Learning Setting 1
#define TSNAXIBMI_RATLR                 0x00B8 // RX Descriptor Address Table Learning Result
    #define TSNAXIBMI_RATLR_RATL         (1 << 31)
    #define TSNAXIBMI_RATLR_RATLO        (1 << 1)
    #define TSNAXIBMI_RATLR_RATLF        (1 << 0)
#define TSNAXIBMI_TSA(s)                (0x00C0 + 8 * s) // TS Descriptor Chain Address s (s=0 to TS_CHAIN_N-1)
#define TSNAXIBMI_TSS(s)                (0x00C4 + 8 * s) // TS Descriptor Chain Setting s (s=0 to TS_CHAIN_N-1)
#define TSNAXIBMI_TRCR(i)               (0x0140 + (i * 0x4))
#define TSNAXIBMI_RR                    0x0200 // RAM Reset
    #define TSNAXIBMI_RR_RATRR          (1 << 0)
    #define TSNAXIBMI_RR_TATRR          (1 << 1)
    #define TSNAXIBMI_RR_TATRR          (1 << 1)
    #define TSNAXIBMI_RR_MASK           (0x3)
#define TSNAXIBMI_TATS                  0x0210 // TX Descriptor Address Table Searching
#define TSNAXIBMI_TATSR0                0x0214 // TX Descriptor Address Table Searching Result 0
#define TSNAXIBMI_TATSR1                0x0218 // TX Descriptor Address Table Searching Result 1
#define TSNAXIBMI_TATSR2                0x021C // TX Descriptor Address Table Searching Result 2
#define TSNAXIBMI_RATS                  0x0220 // RX Descriptor Address Table Searching
#define TSNAXIBMI_RATSR0                0x0224 // RX Descriptor Address Table Searching Result 0
#define TSNAXIBMI_RATSR1                0x0228 // RX Descriptor Address Table Searching Result 1
#define TSNAXIBMI_RATSR2                0x022C // RX Descriptor Address Table Searching Result 2
#define TSNAXIBMI_RIDASM(i)             0x0240
#define TSNAXIBMI_RIDASAM0              0x0244
#define TSNAXIBMI_EIS0                  0x0300 // Error Interrupt Status 0
#define TSNAXIBMI_EIE0                  0x0304 // Error Interrupt Enable 0 
#define TSNAXIBMI_EIE1                  0x0314 // Error Interrupt Enable 1
#define TSNAXIBMI_TCEIS(i)              (0x0340 + (0x010 * i))  //TX Descriptor Configuration Error Interrupt Status i (i=0 to TX_CHAIN_N/32-1)
#define TSNAXIBMI_TCEIE(i)              (0x0344 + (0x010 * i))  //TX Descriptor Configuration Error Interrupt Enable i (i=0 to TX_CHAIN_N/32-1)
#define TSNAXIBMI_RFSEIE(i)             (0x04C4 + (0x010 * i))  //RX Frame Size Error Interrupt Enable i (i=0 to RX_CHAIN_N/32-1)
#define TSNAXIBMI_RFEIE(i)              (0x0544 + (0x010 * i))  //RX Descriptor Full Error Interrupt Enable i (i=0 to RX_CHAIN_N/32-1)
#define TSNAXIBMI_RCEIE(i)              (0x05C4 + (0x010 * i))  //RX Descriptor Configuration Error Interrupt Enable i (i=0 to RX_CHAIN_N/32-1)
#define TSNAXIBMI_DIE                   0x0B04 // Data Interrupt Enable
#define TSNAXIBMI_DID                   0x0B08 // Data Interrupt Disable
#define TSNAXIBMI_TDIS(i)               (0x0B10 + (i * 0x10))
#define TSNAXIBMI_TDIE(i)               (0x0B14 + (i * 0x10))
#define TSNAXIBMI_TDID(i)               (0x0B18 + (i * 0x10))
#define TSNAXIBMI_RDIS(i)               (0x0B90 + (i * 0x10))
#define TSNAXIBMI_RDIE(i)               (0x0B94 + (i * 0x10))
#define TSNAXIBMI_RDID(i)               (0x0B98 + (i * 0x10))
#define TSNAXIBMI_TSDIE                 0x0C14 // TS Descriptor Data Interrupt Enable 
//Refer from linux
#define AXIWC_WREON_SHIFT	            (12)
#define AXIWC_WRPON_SHIFT	            (8)
#define AXIWC_WREON_DEFAULT	            (0x04 << AXIWC_WREON_SHIFT)
#define AXIWC_WRPON_DEFAULT	            (0x01 << AXIWC_WRPON_SHIFT)
#define AXIRC_RREON_SHIFT	            (12)
#define AXIRC_RRPON_SHIFT	            (8)
#define AXIRC_RREON_DEFAULT	            (0x01 << AXIRC_RREON_SHIFT)
#define AXIRC_RRPON_DEFAULT	            (0x08 << AXIRC_RRPON_SHIFT)
#define DIE_DID_TDICX(n)	            BIT((n))
#define DIE_DID_RDICX(n)	            BIT((n) + 8)
#define TDIE_TDID_TDX(n)	            BIT(n)
#define RDIE_RDID_RDX(n)	            BIT(n)
#define TDIS_TDS(n)		                BIT(n)
#define RDIS_RDS(n)		                BIT(n)
/* Define for AXIBMI End */

#define TSN_GPOUT		                0x6000
    #define TSN_GPOUT_RDM		        BIT(13)
    #define TSN_GPOUT_TDM		        BIT(14)

/* Define for gPTP Begin */

/* Currently PTP uses queue 0 */
#define NUM_PTP_TX_QUEUES               1
#define PTP_TX_BUF_LEN                  64
#define PTP_TX_MSGTYPE                  4
ptpv2hdr_t  tx_ph[NUM_PTP_TX_QUEUES][PTP_TX_BUF_LEN * PTP_TX_MSGTYPE];

/* Bit definitions for the GPTI register */
#define RTSNES_PTPRO                    0x18000
#define TIV_MASK                        0xFFFFFFFF
#define  TME                            (RTSNES_PTPRO + 0x0000)
    #define TME_TE                      BIT(0)
#define  TMD                            (RTSNES_PTPRO + 0x0004)
    #define TMD_TD                      BIT(0)
#define  GTIVC                          (RTSNES_PTPRO + 0x0010)
#define  GTOV00                         (RTSNES_PTPRO + 0x0014)
#define  GTOV01                         (RTSNES_PTPRO + 0x0018)
#define  GTOV02                         (RTSNES_PTPRO + 0x001C)

/* Define for gPTP End */ 

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
    DT_FMID         = 0xA0,
    DT_FEND         = 0xB0,

    /* Chain control */
    DT_LEMPTY       = 0xC0,
    DT_EEMPTY       = 0xD0,
    DT_LINK         = 0xE0,
    DT_EOS          = 0xF0,

    /* HW/SW arbitration */
    DT_FEMPTY       = 0x40,
    DT_FEMPTY_IS    = 0x10,
    DT_FEMPTY_IC    = 0x20,
    DT_FEMPTY_ND    = 0x30,
    DT_FEMPTY_START = 0x50,
    DT_FEMPTY_MID   = 0x60,
    DT_FEMPTY_END   = 0x70,

    DT_MASK         = 0xF0,
    DIE             = 0x08, /* Descriptor Interrupt Enable */
};

#define DPTR_ALIGN       4   /* Required descriptor pointer alignment */

/* The Ethernet RTSNES descriptor definitions. */
typedef volatile struct {
    volatile uint16_t   info_ds;        /* Descriptor size bit[11:0] and INFO0 bit[15:12] */
    volatile uint8_t    info;   
    volatile uint8_t    die_dt;         /* ERR bit[0], DSE bit[1], AXIE bit[2], DIE bit[3]; DT bit[7:4] */
    volatile uint32_t   dptr;           /* Descriptor pointer */
} __attribute__((packed)) RTSNES_DESC;

typedef volatile struct {
    volatile uint16_t   info_ds;        /* Descriptor size bit[11:0] and INFO0 bit[15:12] */
    volatile uint8_t    info; 
    volatile uint8_t    die_dt;         /* ERR bit[0], DSE bit[1], AXIE bit[2], DIE bit[3]; DT bit[7:4] */
    volatile uint32_t   dptr;           /* Descriptor pointer */
    volatile uint64_t   info1;
    volatile uint32_t   ts_n;           /* Timestamp    */
    volatile uint32_t   ts_sl;          /* Timestamp    */
} __attribute__((packed)) RTNSES_RX_EXT_DESC;

typedef volatile struct {
    volatile uint16_t   info_ds;        /* DS[7:0] */
    volatile uint8_t    info;           /* INFO0[3:0] DS[11:8] */
    volatile uint8_t    die_dt;         /* INFO0[11:4] */
    volatile uint32_t   dptr;           /* Descriptor pointer */
    volatile uint64_t   info1;
} __attribute__((packed)) RTSNES_TX_DESC;

#define MII_NUM_REGS      29

typedef struct {
    nic_config_t        cfg;
    int                 set_flow;
    int                 mii;
    int                 avb;
    struct _iopkt_self  *iopkt;
    void                *dll_hdl;
    struct _iopkt_inter ient_rx;
    struct _iopkt_inter ient_tx;
    struct _iopkt_inter inter_queue;
    int                 emu_phy;
    int                 slave;
    int                 mv88q5072;
    int                 miiLite;
    int                 internal_loopback;
    int                 tx_internal_delay;
    int                 rx_internal_delay;
    char                *bs_options;
} attach_args_t;

#define EVP_ARRAY_SIZE 3

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
    int                     iid[RTSNES_NUM_IRQS];
    int                     irq[RTSNES_NUM_IRQS];
    struct _iopkt_inter     ient_rx;
    struct _iopkt_inter     ient_tx;
    struct _iopkt_inter     inter_queue;
    struct _iopkt_self      *iopkt;
    uintptr_t               base;
    uintptr_t               ptp_base;
    int (*stack_output)(struct ifnet *, struct mbuf *,
                struct sockaddr *, struct rtentry *);
    int                     fd;
    int                     tid;    
    RTSNES_DESC             *tx_bat_desc;
    off64_t                 tx_bat_desc_addr;
    RTSNES_DESC             *rx_bat_desc;
    off64_t                 rx_bat_desc_addr;
    RTSNES_TX_DESC          *tx_bd[NUM_TX_QUEUES];
    RTNSES_RX_EXT_DESC      *rx_bd[NUM_RX_QUEUES];
    uint32_t                length[NUM_RX_QUEUES];
    int                     tx_cidx[NUM_TX_QUEUES];
    int                     tx_pidx[NUM_TX_QUEUES];
    int                     rx_idx[NUM_RX_QUEUES];
    uint32_t                tx_irq;
    uint32_t                rx_irq;
    uint32_t                tx_dis;
    uint32_t                rx_dis;
    int                     rx_running;
    struct sigevent         *evp[EVP_ARRAY_SIZE];
    struct sigevent         isr_event[NUM_RX_QUEUES];
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
    int                     emu_phy;
    int                     slave;
    int                     miiLite;
    int                     avb;
    int                     coid;
    int                     chid;
    int                     internal_loopback;
    int                     tx_internal_delay;
    int                     rx_internal_delay;
    int                     is_mv_phy;
    uint16_t                *phy_regs;
    uint32_t                phy_speed;
    pthread_mutex_t         phy_mutex;
    char                   *bs_options;
} rtsnes_dev_t;

/* Marvell MV88Q5072 register 0 definitions */

#define PORT_STATUS             0x0000                      // Port status register
    #define TX_PAUSE_EN         (1 << 15)                   // Transmit pause enable
    #define RX_PAUSE_EN         (1 << 14)                   // Receive pause enable
    #define ALT_SPD_VAL         (1 << 13)                   // Alternate speed value
    #define PHY_DETECT          (1 << 12)                   // 802.3 PHY detected
    #define SW_LINK_STATUS      (1 << 11)                   // Link status
    #define DUPLEX_MODE         (1 << 10)                   // Duplex mode
    #define SPEED_SHIFT         8                           // Speed mode
    #define SPEED_MASK          (3 << SPEED_SHIFT)
    #define DUPLEX_FIXED        (1 << 7)                    // Duplex fixed
    #define EEE_ENABLED         (1 << 6)                    // EEE enabled
    #define TX_PAUSED           (1 << 5)                    // Transmitter paused
    #define FLOW_CTRL           (1 << 4)                    // Flow control
    #define C_MODE_MASK         0x0F                        // Config mode

int rtsnes_ioctl(struct ifnet *ifp, unsigned long cmd, caddr_t data);
int rtsnes_attach (struct device *parent, struct device *self, void *aux);
int rtsnes_detach (struct device *dev, int flags);
void rtsnes_start(struct ifnet *ifp);
int rtsnes_output(struct ifnet *ifp, struct mbuf *m, struct sockaddr *dst, struct rtentry *rt);
int rtsnes_receive(rtsnes_dev_t *rtsnes, struct nw_work_thread *wtp, uint8_t queue);

int rtsnes_axibmi_enable_interrupt(rtsnes_dev_t *rtsnes, int tx, int enable);
int rtsnes_axibmi_process_interrupt_tx(void *arg, struct nw_work_thread *wtp);
int rtsnes_axibmi_enable_interrupt_tx(void * arg);
int rtsnes_axibmi_process_interrupt_rx(void *arg, struct nw_work_thread *wtp);
int rtsnes_axibmi_process_interrupt_avb_rx(void *arg, struct nw_work_thread *wtp);
int rtsnes_axibmi_enable_interrupt_rx(void * arg);
int rtsnes_axibmi_enable_interrupt_avb_rx (void *arg);
int rtsnes_process_queue(void *arg, struct nw_work_thread *wtp);
int rtsnes_enable_queue(void *arg);
const struct sigevent * rtsnes_rmac_isr (void *arg, int iid);

const struct sigevent * rtsnes_axibmi_isr_avb (void *arg, int iid);
const struct sigevent * const * rtsnes_axibmi_isr(void *arg, int iid);
const struct sigevent * rtsnes_axibmi_tx_isr (void *arg, int iid);
const struct sigevent * rtsnes_axibmi_rx_isr (void *arg, int iid);

int rtsnes_set_speed(rtsnes_dev_t *rmac, uint32_t speed);
void rtsnes_update_stats(rtsnes_dev_t *rtsnes);
void rtsnes_clear_stats(rtsnes_dev_t *rtsnes);

void rtsnes_mii_write(void *handle, uint8_t phy_id, uint8_t location, uint16_t data);
uint16_t rtsnes_mii_read(void *handle, uint8_t phy_id, uint8_t location);
uint16_t rtsnes_mii_read_cl45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad);
void rtsnes_mii_write_cl45(void *handle, uint8_t phyad, uint16_t devad, uint16_t regad, uint16_t data);
int rtsnes_phy_init(rtsnes_dev_t *rtsnes);
void rtsnes_phy_fini(rtsnes_dev_t *rtsnes);
void rtsnes_MDI_MonitorPhy(void *arg);
int rtsnes_mediachange(struct ifnet *ifp);
int rtsnes_is_br_phy(rtsnes_dev_t *rtsnes);

int rtsnes_mv_phy_init (rtsnes_dev_t *rtsnes);
int rtsnes_is_mv_phy (rtsnes_dev_t *rtsnes);
void rtsnes_mv_phy_fini(rtsnes_dev_t *rtsnes);

int rtsnes_ptp_init(rtsnes_dev_t *rtsnes);
void rtsnes_ptp_cal(rtsnes_dev_t *rtsnes);
int rtsnes_ptp_is_eventmsg(struct mbuf *m, ptpv2hdr_t **ph);
void rtsnes_ptp_add_rx_timestamp(rtsnes_dev_t *rtsnes, ptpv2hdr_t *ph, RTNSES_RX_EXT_DESC *bd);
void rtsnes_ptp_add_tx_timestamp(rtsnes_dev_t *rtsnes);
int rtsnes_ptp_ioctl(rtsnes_dev_t *rtsnes, struct ifdrv *ifd);

int emu_phy_init (rtsnes_dev_t *ravb);
uint16_t emu_phy_read (rtsnes_dev_t * ravb, uint8_t phy_add, uint8_t reg_add);
int emu_phy_dinit (rtsnes_dev_t *ravb);

void static inline rtsnes_mhd_write(rtsnes_dev_t *rtsnes, uint32_t reg, uint32_t data)
{
    out32(rtsnes->base + TSNES_MHD_BASE_OFFSET + reg, data);
}

uint32_t static inline rtsnes_mhd_read(rtsnes_dev_t *rtsnes, uint32_t reg )
{
     return in32(rtsnes->base + TSNES_MHD_BASE_OFFSET + reg);
} 

void static inline rtsnes_axibmi_write(rtsnes_dev_t *rtsnes, uint32_t reg, uint32_t data)
{
    out32(rtsnes->base + TSNES_AXIBMI_BASE_OFFSET + reg, data);
}

uint32_t static inline rtsnes_axibmi_read(rtsnes_dev_t *rtsnes, uint32_t reg )
{
     return in32(rtsnes->base + TSNES_AXIBMI_BASE_OFFSET + reg);
} 

void static inline rtsnes_rmac_write(rtsnes_dev_t *rtsnes, uint32_t reg, uint32_t data)
{
    out32(rtsnes->base + TSNES_RMAC_BASE_OFFSET + reg, data);
}

uint32_t static inline rtsnes_rmac_read(rtsnes_dev_t *rtsnes, uint32_t reg )
{
     return in32(rtsnes->base + TSNES_RMAC_BASE_OFFSET + reg);
} 

void static inline rtsnes_rms_write(rtsnes_dev_t *rtsnes, uint32_t reg, uint32_t data)
{
    out32(rtsnes->base + TSNES_RMS_BASE_OFFSET + reg, data);
}

#define TSNES_TIMEOUT_MS    10000

static int inline rtsnes_mhd_reg_wait(rtsnes_dev_t *rtsnes, uint32_t offset, uint32_t mask, uint32_t val)
{
    int i = 0;
    while (i < TSNES_TIMEOUT_MS) {
        if ((rtsnes_mhd_read(rtsnes, offset) & mask) == val)
            return EOK;
        nanospin_ns(1000);
        i++;
    }

    return -ETIMEDOUT;
}

static int inline rtsnes_rmac_reg_wait(rtsnes_dev_t *rtsnes, uint32_t offset, uint32_t mask, uint32_t val)
{
    int i = 0;
    while (i < TSNES_TIMEOUT_MS) {
        if ((rtsnes_rmac_read(rtsnes, offset) & mask) == val)
            return EOK;
        nanospin_ns(1000);
        i++;
    }

    return -ETIMEDOUT;
}

static int inline rtsnes_axibmi_reg_wait(rtsnes_dev_t *rtsnes, uint32_t offset, uint32_t mask, uint32_t val)
{
    int i = 0;
    while (i < TSNES_TIMEOUT_MS) {
        if ((rtsnes_axibmi_read(rtsnes, offset) & mask) == val)
            return EOK;
        nanospin_ns(1000);
        i++;
    }

    return -ETIMEDOUT;
}

#endif // define __RCAR_RTSNES_H__


