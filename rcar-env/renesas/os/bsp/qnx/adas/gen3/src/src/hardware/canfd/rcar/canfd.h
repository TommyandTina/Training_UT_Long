/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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
#ifndef _CANFD_RCAR_H
#define _CANFD_RCAR_H
#include <stdbool.h>

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <devctl.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <atomic.h>
#include <hw/libcan.h>
#include <sys/can_dcmd.h>
#include <hw/sysinfo.h>
#include <drvr/hwinfo.h>
#include <aarch64/r-car-gen3.h>
#include "canfd_slog.h"
    
#define PROD                                        ((product_id == PRODUCT_ID_RCAR_V3U)? 1 : 0)
    
#define CANFD_REG_BASE                              (PROD? 0xE6660000 : 0xE66C0000)

/* IRQ vector */        
#define CANFD_IRQ_GL                                (PROD? 26 + 32 : 30 + 32)
#define CANFD_IRQ_CH                                (PROD? 25 + 32 : 29 + 32)
#define CANFD_TOTAL_CHANNELS_MAX                    (8)
#define CANFD_TOTAL_CHANNELS                        (PROD? 8 : ((product_id == PRODUCT_ID_RCAR_V3H) && \
                                                                (product_rev == PRODUCT_V3H_REV_2_0 || product_rev == PRODUCT_V3H_REV_2_1)) ? 3 : 2)

/* FIFO buffer */
#define CANFD_RECEIVE_FIFO_BUFFER_MAX               (4)         
#define CANFD_RECEIVE_FIFO_BUFFER                   (1)                     /* Each channel has 1 receive FIFO buffer */
#define CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER_MAX      (3)
#define CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER          (PROD? 3 : 1)
    
#define CANFD_REG_SIZE                              (0x10000)
    
#define CANFD_BIT_MASK                              (3)
#define MAX_STANDARD_ID                             (0x7FF)
    
/* Timestamp */ 
#define N                                           (15)                    /* Default N = 15 <=> transmition interval timer = 75us */
#define pclk                                        (133.33f)               /* Clock S3D2 */
                                                                            /* Modifying depend on board: page 8-3 to 8-9 */
/*************************************************
 * Lists the RS-CANFD registers used in classical*
 * CAN mode.                                     *
 *************************************************/

#define RCAR_CAN_RSCFD0CFDGRMCFG                    (0x04FC)

/* RSCFDnCmCFG — Channel Configuration Register (m = 0, 1) */
#define RCAR_CAN_RSCFD0CmCFG(m)                     (0x0000 + (0x10 * (m)))
#define BRP(val)                                    (((val) & 0x3FF) << 0)
#define TSEG1(val)                                  (((val) & 0x0F) << 16)
#define TSEG2(val)                                  (((val) & 0x07) << 20)
#define CSJW(val)                                   (((val) & 0x03) << 24)
/* End of RSCFDnCmCFG — Channel Configuration Register */

#define RCAR_CAN_RSCFD0CmCTR(m)                     (0x0004 + (0x10 * (m)))
#define RCAR_CAN_RSCFD0CmSTS(m)                     (0x0008 + (0x10 * (m)))
#define RCAR_CAN_RSCFD0CmERFL(m)                    (0x000C + (0x10 * (m)))

#define RCAR_CAN_RSCFD0GCFG                         (0x0084)
#define RCAR_CAN_RSCFD0GCTR                         (0x0088)

#define RCAR_CAN_RSCFD0GSTS                         (0x008C)

#define RCAR_CAN_RSCFD0GERFL                        (0x0090)
#define RCAR_CAN_RSCFD0GTSC                         (0x0094)
#define RCAR_CAN_RSCFD0GTINTSTS0                    (0x0460)
#define RCAR_CAN_RSCFD0GTINTSTS1                    (0x0464)
#define RCAR_CAN_RSCFD0GAFLECTR                     (0x0098)

#define RCAR_CAN_RSCFD0GAFLCFG0                     (0x009C)

#define RCAR_CAN_RSCFD0GAFLCFG1                     (0x00A0)

#define RCAR_CAN_RSCFD0GAFLID(j)                    (0x0500 + (0x10 * (j)))
#define RCAR_CAN_RSCFD0GAFLM(j)                     (0x0504 + (0x10 * (j)))
#define RCAR_CAN_RSCFD0GAFLP0(j)                    (0x0508 + (0x10 * (j)))
#define RCAR_CAN_RSCFD0GAFLP1(j)                    (0x050C + (0x10 * (j)))

#define RCAR_CAN_RSCFD0RMNB                         (0x00A4)

#define RCAR_CAN_RSCFD0RMND(y)                      (0x00A8 + (0x04 * (y)))

#define RCAR_CAN_RSCFD0RMID(q)                      (0x0600 + (0x10 * (q)))
#define RCAR_CAN_RSCFD0RMPTR(q)                     (0x0604 + (0x10 * (q)))
#define RCAR_CAN_RSCFD0RMDF0(q)                     (0x0608 + (0x10 * (q)))
#define RCAR_CAN_RSCFD0RMDF1(j)                     (0x060C + (0x10 * (q)))

#define RCAR_CAN_RSCFD0RFCC(x)                      (0x00B8 + (0x04 * (x)))
#define RCAR_CAN_RSCFD0RFSTS(x)                     (0x00D8 + (0x04 * (x)))
#define RCAR_CAN_RSCFD0RFPCTR(x)                    (0x00F8 + (0x04 * (x)))

#define RCAR_CAN_RSCFD0RFID(x)                      (0x0E00 + (0x10 * (x)))
#define RCAR_CAN_RSCFD0RFPTR(x)                     (0x0E04 + (0x10 * (x)))
#define RCAR_CAN_RSCFD0RFDF0(x)                     (0x0E08 + (0x10 * (x)))
#define RCAR_CAN_RSCFD0RFDF1(x)                     (0x0E0C + (0x10 * (x)))

#define RCAR_CAN_RSCFD0CFCC(k)                      (0x0118 + (0x04 * (k)))
#define RCAR_CAN_RSCFD0CFSTS(k)                     (0x0178 + (0x04 * (k)))
#define RCAR_CAN_RSCFD0CFPCTR(k)                    (0x01D8 + (0x04 * (k)))

#define RCAR_CAN_RSCFD0CFID(k)                      (0x0E80 + (0x10 * (k)))
#define RCAR_CAN_RSCFD0CFPTR(k)                     (0x0E84 + (0x10 * (k)))
#define RCAR_CAN_RSCFD0CFDF0(k)                     (0x0E88 + (0x10 * (k)))
#define RCAR_CAN_RSCFD0CFDF1(k)                     (0x0E8C + (0x10 * (k)))

#define RCAR_CAN_RSCFD0FESTS                        (0x0238)
#define RCAR_CAN_RSCFD0FFSTS                        (0x023C)
#define RCAR_CAN_RSCFD0FMSTS                        (0x0240)
#define RCAR_CAN_RSCFD0RFISTS                       (0x0244)
#define RCAR_CAN_RSCFD0CFRISTS                      (0x0248)
#define RCAR_CAN_RSCFD0CFTISTS                      (0x024C)

#define RCAR_CAN_RSCFD0TMC(p)                       (0x0250 + (0x01 * (p)))
#define RCAR_CAN_RSCFD0TMSTS(p)                     (0x02D0 + (0x01 * (p)))
#define RCAR_CAN_RSCFD0TMID(p)                      (0x1000 + (0x10 * (p)))
#define RCAR_CAN_RSCFD0TMPTR(p)                     (0x1004 + (0x10 * (p)))
#define RCAR_CAN_RSCFD0TMDF0(p)                     (0x1008 + (0x10 * (p)))
#define RCAR_CAN_RSCFD0TMDF1(p)                     (0x100C + (0x10 * (p)))

#define RCAR_CAN_RSCFD0TMIEC(y)                     (0x0390 + (0x04 * (y)))
#define RCAR_CAN_RSCFD0TMTRSTS(y)                   (0x0350 + (0x04 * (y)))
#define RCAR_CAN_RSCFD0TMTARSTS(y)                  (0x0360 + (0x04 * (y)))
#define RCAR_CAN_RSCFD0TMTCSTS(y)                   (0x0370 + (0x04 * (y)))
#define RCAR_CAN_RSCFD0TMTASTS(y)                   (0x0380 + (0x04 * (y)))

#define RCAR_CAN_RSCFD0TXQCC(m)                     (0x03A0 + (0x04 * (m)))
#define RCAR_CAN_RSCFD0TXQSTS(m)                    (0x03C0 + (0x04 * (m)))
#define RCAR_CAN_RSCFD0TXQPCTR(m)                   (0x03E0 + (0x04 * (m)))
#define RCAR_CAN_RSCFD0THLCC(m)                     (0x0400 + (0x04 * (m)))
#define RCAR_CAN_RSCFD0THLSTS(m)                    (0x0420 + (0x04 * (m)))
#define RCAR_CAN_RSCFD0THLPCTR(m)                   (0x0440 + (0x04 * (m)))
#define RCAR_CAN_RSCFD0THLACC(m)                    (0x1800 + (0x04 * (m)))

#define RCAR_CAN_RSCFD0GTSTCFG                      (0x0468)
#define RCAR_CAN_RSCFD0GTSTCTR                      (0x046C)
#define RCAR_CAN_RSCFD0GLOCKK                       (0x047C)

#define RCAR_CAN_RSCFD0RPGACC(r)                    (0x1900 + (0x04 * (r)))

/****************************************************
 * Lists the RS-CANFD registers used in CAN FD mode.*
 ****************************************************/

/* Global Interface Mode Select Register */
#define RCAR_CANFD_RSCFD0CFDGRMCFG              (PROD? (0x1314) : (0x04FC))
 //#define RCAR_V3U_CLOE                          (1 << 30)
 //#define RCAR_V3U_FDOE                          (1 << 28)
#define RCMC(val)                               (PROD?((0x01) << (30 - ((val)*2))) : (((val) & 0x01) << 0))
/* End of Global Interface Mode Select Register */

/* Channel Normal Bit Rate Configuration Register */
#define RCAR_CANFD_RSCFD0CmNCFG(m)              (0x0000 + (0x10 * (m)))
#define NTSEG2(val)                             (PROD?(((val) & 0x7F) << 25) : (((val) & 0x1F) << 24))
#define NTSEG1(val)                             (PROD?(((val) & 0xFF) << 17) : (((val) & 0x7F) << 16))
#define NSJW(val)                               (PROD?(((val) & 0x7F) << 10) : (((val) & 0x1F) << 11))
#define NBRP(val)                               (((val) & 0x3FF) << 0) 
/* End of Channel Normal Bit Rate Configuration Register */

/* Channel Control Register */
#define RCAR_CANFD_RSCFD0CFDCmCTR(m)            (0x0004 + (0x10 * (m)))
#define CHMDC(val)                              (((val) & 0x03) << 0)
#define CSLPR                                   (1 << 2)
#define RTBO                                    (1 << 3)
#define BEIE                                    (1 << 8)
#define EWIE                                    (1 << 9)
#define EPIE                                    (1 << 10)
#define BOEIE                                   (1 << 11)
#define BORIE                                   (1 << 12)
#define OLIE                                    (1 << 13)
#define BLIE                                    (1 << 14)
#define ALIE                                    (1 << 15)
#define TAIE                                    (1 << 16)
#define EOCOIE                                  (1 << 17)
#define SOCOIE                                  (1 << 18)
#define TDCVFIE                                 (1 << 19)
#define BOM(val)                                (((val) & 0x03) << 21)
#define ERRD                                    (1 << 23)
#define CTME                                    (1 << 24)
#define CTMS(val)                               (((val) & 0x03) << 25)
#define CRCT                                    (1 << 30)
#define ROM                                     (1 << 31)
 /* End of Channel Control Register */

#define RCAR_CANFD_RSCFD0CFDCmSTS(m)            (0x0008 + (0x10 * (m)))
#define CRSTSTS                                 (1 << 0)
#define CHLTSTS                                 (1 << 1)
#define CSLPSTS                                 (1 << 2)
#define EPSTS                                   (1 << 3)
#define BOSTS                                   (1 << 4)
#define TRMSTS                                  (1 << 5)
#define RECSTS                                  (1 << 6)
#define COMSTS                                  (1 << 7)
#define ESIF                                    (1 << 8)
#define REC(val)                                (((val) & 0xFF) << 16)
#define TEC(val)                                (((val) & 0xFF) << 24)

/* Channel Error Flag Status Register (m = 0-7) */
#define RCAR_CANFD_RSCFD0CFDCmERFL(m)           (0x000C + (0x10 * (m)))
#define BEF                                     (1 << 0)
#define EWF                                     (1 << 1)
#define EPF                                     (1 << 2)
#define BOEF                                    (1 << 3)
#define BORF                                    (1 << 4)
#define OVLF                                    (1 << 5)
#define BLF                                     (1 << 6)
#define ALF                                     (1 << 7)
#define SERR                                    (1 << 8)
#define FERR                                    (1 << 9)
#define AERR                                    (1 << 10)
#define CERR                                    (1 << 11)
#define B1ERR                                   (1 << 12)
#define B0ERR                                   (1 << 13)
#define ADERR                                   (1 << 14)
#define CRCREG(val)                             (((val) & 0x7FFF) << 16)
/* End of Channel Error Flag Status Register */

/* Channel Data Bit Rate Configuration Register (m = 0-7) */
#define RCAR_CANFD_RSCFD0CFDCmDCFG(m)           (PROD?(0x1400 + (0x20 * (m))) : (0x0500 + (0x20 * (m))))
#define DSJW(val)                               (((val) & 0x07) << 24)
#define DTSEG2(val)                             (PROD?(((val) & 0xF) << 16) : (((val) & 0x07) << 20))
#define DTSEG1(val)                             (PROD?(((val) & 0x1F) << 8) : (((val) & 0x0F) << 16))
#define DBRP(val)                               (((val) & 0x0FF) << 0)
/* End of Channel Data Bit Rate Configuration Register */

#define RCAR_CANFD_RSCFD0CFDCmFDCFG(m)          (0x0504 + (0x20 * (m)))
#define RCAR_CANFD_RSCFD0CFDCmFDCTR(m)          (0x0508 + (0x20 * (m)))
#define RCAR_CANFD_RSCFD0CFDCmFDSTS(m)          (0x050C + (0x20 * (m)))
#define RCAR_CANFD_RSCFD0CFDCmFDCRC(m)          (0x0510 + (0x20 * (m)))

/* Global Configuration Register */
#define RCAR_CANFD_RSCFD0CFDGCFG                (0x0084)
#define TPRI                                    (1 << 0)
#define DCE                                     (1 << 1)
#define DRE                                     (1 << 2)
#define MME                                     (1 << 3)
#define DCS                                     (1 << 4)
#define CMPOC                                   (1 << 5)
#define EEFE                                    (1 << 6)
#define TSP(val)                                (((val) & 0x0F) << 8)
#define TSSS                                    (1 << 12)
#define TSBTCS(val)                             (((val) & 0x07) << 13)
#define ITRCP(val)                              (((val) & 0xFFFF) << 16)
/* End of Global Configuration Register */

/* Global Control Register */
#define RCAR_CANFD_RSCFD0CFDGCTR                (0x0088)
#define GMDC(val)                               (((val) & 0x03) << 0)
#define GSLPR                                   (1 << 2)
#define DEIE                                    (1 << 8)
#define MEIE                                    (1 << 9)
#define THLEIE                                  (1 << 10)
#define CMPOFIE                                 (1 << 11)
#define TSRST                                   (1 << 16)
 /* End of Global Control Register */

/* Global Status Register */
#define RCAR_CANFD_RSCFD0CFDGSTS                (0x008C)
#define GRSTSTS                                 (1 << 0)
#define GHLTSTS                                 (1 << 1)
#define GSLPSTS                                 (1 << 2)
#define GRAMINIT                                (1 << 3)
/* End of Global Status Register */

/* Global Error Flag Register */
#define RCAR_CANFD_RSCFD0CFDGERFL               (0x0090)
#define DEF                                     (1 << 0)
#define MES                                     (1 << 1)
#define THLES                                   (1 << 2)
#define CMPOF                                   (1 << 3)
#define EEF0                                    (1 << 16)
#define EEF1                                    (1 << 17)
#define EEF2                                    (1 << 18)
#define EEF3                                    (1 << 19)
#define EEF4                                    (1 << 20)
#define EEF5                                    (1 << 21)
#define EEF6                                    (1 << 22)
#define EEF7                                    (1 << 23)
#define CANFD_GERFL_ERR_V3U(caninit)            (((caninit)->interface? CMPOF : 0) | \
                                                (EEF0 | EEF1 | EEF2 | EEF3 | EEF4| EEF5 | EEF6 | EEF7 | \
                                                 DEF | MES | THLES))                                         

#define CANFD_GERFL_ERR_V3H20(caninit)          (((caninit)->interface? CMPOF : 0) | \
                                                (EEF0 | EEF1 | EEF2 | \
                                                 DEF | MES | THLES))

#define CANFD_GERFL_ERR_OTHER(caninit)          (((caninit)->interface? CMPOF : 0) | \
                                                (EEF0 | EEF1 | \
                                                 DEF | MES | THLES))

#define CANFD_GERFL_ERR(caninit)                (PROD?CANFD_GERFL_ERR_V3U(caninit) : ((product_id == PRODUCT_ID_RCAR_V3H) && (product_rev == PRODUCT_V3H_REV_2_0 || product_rev == PRODUCT_V3H_REV_2_1)) ? \
                                                CANFD_GERFL_ERR_V3H20(caninit) : CANFD_GERFL_ERR_OTHER(caninit))
/* End of Global Error Flag Register */

/* Global Timestamp Counter Register */
#define RCAR_CANFD_RSCFD0CFDGTSC                (0x0094)
#define TS(val)                                 (((val) & 0xFFFF) << 0)


#define RCAR_CANFD_RSCFD0CFDGTINTSTS0           (0x0460)
#define RCAR_CANFD_RSCFD0CFDGTINTSTS1           (0x0464)

/* Receive Rule Entry Control Register */
#define RCAR_CANFD_RSCFD0CFDGAFLECTR            (0x0098)
#define AFLPN(val)                              (PROD?(((val) & 0x7F) << 0) : (((val) & 0x1F) << 0))
#define AFLDAE                                  (1 << 8)
/* End of Receive Rule Entry Control Register */

/* Receive Rule Configuration Register 0 */
#define RCAR_CANFD_RSCFD0CFDGAFLCFG0(ch)        (PROD?(0x009C + (0x04 * ((ch) / 2))) : (0x009C))
#define RNC(m,val)                              (PROD?(((val) & 0x1FF) << ((m)?(0) : (16))) : (((val) & 0xFF) << (24 - (m) * 8)))
//#define RNC(m,val)                            (((val) >> (16 - (m) * 16)) & 0x1FF) 
/* End of Receive Rule Configuration Register 0 */

#define RCAR_CANFD_RSCFD0CFDGAFLCFG1             (0x00A0)


/******************    Receive Rule Related Registers   **************************/

 #define RCAR_CANFD_GAFL_OFFSET                  (PROD?(0x800) : (0x0))

/* Receive Rule ID Register (j = 0 to 15)*/
#define RCAR_CANFD_RSCFD0CFDGAFLID(j)            (0x1000 + RCAR_CANFD_GAFL_OFFSET + (0x10 * (j)))
#define GAFLID(val)                              (((val) & 0x1FFFFFFF) << 0)       /* GAFLID[28:0] */
#define GAFLLB                                   (1 << 29)
#define GAFLRTR                                  (1 << 30)
#define GAFLIDE                                  (1 << 31)
/* End of Receive Rule ID Register */

/* Receive Rule Mask Register (j = 0 to 15) */
#define RCAR_CANFD_RSCFD0CFDGAFLM(j)             (0x1004 + RCAR_CANFD_GAFL_OFFSET + (0x10 * (j)))
#define GAFLIDM(val)                             (((val) & 0x1FFFFFFF) << 0)       /* GAFLIDM [28:0]*/
#define GAFLRTRM                                 (1 << 30)
#define GAFLIDEM                                 (1 << 31)
/* End of Receive Rule Mask Register (j = 0 to 15) */

/* Receive Rule Pointer 0 Register (j = 0 to 15) */
#define RCAR_CANFD_RSCFD0CFDGAFLP0(j)            (0x1008 + RCAR_CANFD_GAFL_OFFSET + (0x10 * (j)))
#define GAFLRMDP(val)                            (((val) & 0x7F) << 8)
#define GAFLRMV                                  (1 << 15)
#define GAFLPTR(val)                             (((val) & 0xFFF) << 16)
#define GAFLDLC(val)                             (((val) & 0x0F) << 28)
/* End of Receive Rule Pointer 0 Register */

/* Receive Rule Pointer 1 Register (j = 0 to 15) */
#define RCAR_CANFD_RSCFD0CFDGAFLP1(j)            (0x100C + RCAR_CANFD_GAFL_OFFSET + (0x10 * (j)))
#define GAFLFDP0(val)                            (((val) & 0xFFFF) << 0)
#define GAFLFDP1(val)                            (((val) & 0x3FF) << 16)
/* End of Receive Rule Pointer 1 Register */

/******************    End of receive Rule Related Registers   *************************/

/* Receive Buffer Number Register */
#define RCAR_CANFD_RSCFD0CFDRMNB                 (0x00A4)
#define NRXMB(val)                               (((val) & 0xFF) << 0)
#define RMPLS(val)                               (((val) & 0x03) << 8)
/* End of Receive Buffer Number Register */

/* Receive Buffer New Data Register (y = 0 to 2) */
#define RCAR_CANFD_RSCFD0CFDRMND(y)              (0x00A8 + (0x04 * (y)))
#define RMNS(q)                                  (1 << (q))
/* End of Receive Buffer New Data Register */

/* Receive Buffer ID Register (q = 0 to 31) */
#define RCAR_CANFD_RSCFD0CFDRMID(q)              (0x2000 + (0x20 * (q)))
#define RMID                                     (0x1FFFFFFF << 0)
#define RMRTR                                    (1 << 30)
#define RMIDE                                    (1 << 31)
/* End of Receive Buffer ID Register */

/* Receive Buffer Pointer Register (q = 0 to 31) */
#define RCAR_CANFD_RSCFD0CFDRMPTR(q)             (0x2004 + (0x20 * (q)))
#define RMTS                                     (0xFFFF << 0)
#define RMPTR                                    (0xFFF << 16)
#define RMDLC                                    (0x0F << 28)
/* End of Receive Buffer Pointer Register */

/* Receive Buffer Data Field b Register (b = 0 to 4, q = 0 to 31) */
#define RCAR_CANFD_RSCFD0CFDRMDF(b,q)            (0x200C + (0x04 * (b)) + (0x20 * (q)))
/* End of Receive Buffer Data Field b Register */

#define RCAR_CANFD_RSCFD0CFDRMFDSTS(q)           (0x2008 + (0x20 * (q)))

/* Receive FIFO Buffer Configuration and Control Register (x = 0 to 7) */
#define RCAR_CANFD_RSCFD0CFDRFCC(x)              (PROD?(0x00C0 + (0x04 * (x))) : (0x00B8 + (0x04 * (x))))

#define RFE                                      (1 << 0)
#define RFIE                                     (1 << 1)
#define RFPLS(val)                               (((val) & 0x07) << 4)
#define RFDC(val)                                (((val) & 0x07) << 8)
#define RFIM                                     (1 << 12)
#define RFIGCV(val)                              (((val) & 0x07) << 13)
/* End of Receive FIFO Buffer Configuration and Control Register */

#define RCAR_CANFD_RSCFD0CFDRFSTS(x)             (PROD?(0x00E0 + (0x04 * (x))) : (0x00D8 + (0x04 * (x))))

#define RFEMP                                    (1 << 0)
#define RFFLL                                    (1 << 1)
#define RFMLT                                    (1 << 2)
#define RFIF                                     (1 << 3)
#define RFMC(val)                                (((val) & 0xFF) << 8)

#define RCAR_CANFD_RSCFD0CFDRFPCTR(x)            (PROD?(0x0100 + (0x04 * (x))) : (0x00F8 + (0x04 * (x))))
#define RFPC(val)                                (((val) & 0xFF) << 0)
 
#define RCAR_CANFD_RSCFD0CFDRFID(x)              (PROD?(0x6000 + (0x80 * (x))) : (0x3000 + (0x80 * (x))))
#define RFID(val)                                (((val) & 0x1FFFFFFF) << 0)
#define RFRTR                                    (1 << 30)
#define RFIDE                                    (1 << 31)

/* Receive FIFO Buffer Access Pointer Register (x = 0 to 7) */
#define RCAR_CANFD_RSCFD0CFDRFPTR(x)             (PROD?(0x6000 + 0x04 + (0x80 * (x))) : (0x3004 + (0x80 * (x))))
#define RFTS                                     (0xFFFF << 0)
#define RFPTR                                    (0xFFF << 16)
#define RFDLC(val)                               (((val) >> 28) & 0x0F)
/* End of Receive FIFO Buffer Access Pointer Register */

#define RCAR_CANFD_RSCFD0CFDRFFDSTS(x)           (PROD?(0x6000 + 0x08 + (0x80 * (x))) : (0x3008 + (0x80 * (x))))

#define RCAR_CANFD_RSCFD0CFDRFDF(d, x)           (PROD?(0x6000 + 0x0C + (0x04 * (d)) + (0x80 * (x))) : (0x300C + (0x04 * (d)) + (0x80 * (x))))

/* Transmit/receive FIFO Buffer Configuration and Control Register k (k = 0 to 2 for each channel) */
#define RCAR_CANFD_RSCFD0CFDCFCC(k)              (PROD?(0x0120 + (0x04 * (k))) : (0x0118 + (0x04 * (k))))
#define CFDC(val)                                (PROD?(((val) & 0x7) << 21) : (((val) & 0x07) << 8))
#define CFTML(val)                               (PROD?(((val) & 0xF) << 16) : (((val) & 0x0F) << 20))
#define CFM(val)                                 (PROD?(((val) & 0x3) << 8) : (((val) & 0x03) << 16))
#define CFE                                      (1 << 0)
#define CFRXIE                                   (1 << 1)
#define CFTXIE                                   (1 << 2)
#define CFPLS(val)                               (((val) & 0x07) << 4)
#define CFIGCV(val)                              (((val) & 0x07) << 13)
#define CFIM                                     (1 << 12)
#define CFITSS                                   (1 << 18)
#define CFITR                                    (1 << 19)
#define CFITT(val)                               (((val) & 0xFF) << 24)
/* End of Transmit/receive FIFO Buffer Configuration and Control Register k */

/* Transmit/receive FIFO Buffer Status Register (k = 0 to 2 for each channel) */
#define RCAR_CANFD_RSCFD0CFDCFSTS(k)             (PROD?(0x01E0 + (0x04 * (k))) : (0x0178 + (0x04 * (k))))
#define CFEMP                                    (1 << 0)
#define CFFLL                                    (1 << 1)
#define CFMLT                                    (1 << 2)
#define CFRXIF                                   (1 << 3)
#define CFTXIF                                   (1 << 4)
#define CFMC(val)                                (((val) & 0xFF) << 8)

/* Transmit/receive FIFO Buffer Pointer Control Register (k = 0 to 2 for each channel) */
#define RCAR_CANFD_RSCFD0CFDCFPCTR(k)            (PROD?(0x0240 + (0x04 * (k))) : (0x01D8 + (0x04 * (k))))
#define CFPC(val)                                (((val) & 0xFF) << 0)
/* End of Transmit/receive FIFO Buffer Pointer Control Register */

/* Transmit/receive FIFO Buffer Access ID Register (k = 0 to 2 for each channel) */
#define RCAR_CANFD_RSCFD0CFDCFID(k)              (PROD?(0x6400 + (0x80 * (k))) : (0x3400 + (0x80 * (k))))
#define CFID(val)                                (((val) & 0x1FFFFFFF) << 0)
#define THLEN                                    (1 << 29)
#define CFRTR                                    (1 << 30)
#define CFIDE                                    (1 << 31)
/* End of Transmit/receive FIFO Buffer Access ID Register */

/* Transmit/receive FIFO Buffer Access Pointer Register (k = 0 to 2 for each channel) */
#define RCAR_CANFD_RSCFD0CFDCFPTR(k)             (PROD?(0x6404 + (0x80 * (k))) : (0x3404 + (0x80 * (k))))
#define CFTS(val)                                (((val) & 0xFFFF) << 0)
#define CFPTR(val)                               (((val) & 0xFFF) << 16)
#define CFDLC(val)                               (((val) & 0x0F) << 28)
/* End of Transmit/receive FIFO Buffer Access Pointer Register */

/* Transmit/receive FIFO CAN FD Configuration and Status Register (k = 0 to 2 for each channel) */
#define RCAR_CANFD_RSCFD0CFDCFFDCSTS(k)          (PROD?(0x6408 + (0x80 * (k))) : (0x3408 + (0x80 * (k))))
#define CFESI                                    (1 << 0)
#define CFBRS                                    (1 << 1)
#define CFFDF                                    (1 << 2)
/* End of Transmit/receive FIFO CAN FD Configuration and Status Register */

/* Transmit/receive FIFO Buffer Access Data Field d Register (d =
0 to 15, k = 0 to 2 for each channel) */
#define RCAR_CANFD_RSCFD0CFDCFDF(d,k)            (PROD?(0x640C + (0x04 * (d)) + (0x80 * (k))) : (0x340C + (0x04 * (d)) + (0x80 * (k))))
/* End of Transmit/receive FIFO Buffer Access Data Field d Register */

#define RCAR_CANFD_RSCFD0CFDFESTS                (0x0238)
#define RCAR_CANFD_RSCFD0CFDFFSTS                (0x023C)
#define RCAR_CANFD_RSCFD0CFDFMSTS                (0x0240)
#define RCAR_CANFD_RSCFD0CFDRFISTS               (0x0244)
#define RCAR_CANFD_RSCFD0CFDCFRISTS              (0x0248)
#define RCAR_CANFD_RSCFD0CFDCFTISTS              (0x024C)
#define RCAR_CANFD_RSCFD0CFDCDTCT                (0x0490)
#define RCAR_CANFD_RSCFD0CFDCDTSTS               (0x0494)

/* Transmit Buffer Control Register (p = 0 to 31) */
#define RCAR_CANFD_RSCFD0CFDTMC(p)               (0x0250 + (0x01 * (p)))
#define TMTR                                     (1 << 0)
#define TMTAR                                    (1 << 1)
#define TMOM                                     (1 << 2)
/* End of Transmit Buffer Control Register */

/* Transmit Buffer Status Register (p = 0 to 31) */
#define RCAR_CANFD_RSCFD0CFDTMSTS(p)             (0x02D0 + (0x01 * (p)))
#define TMTSTS                                   (1 << 0)
#define TMTRF(val)                               (((val) & 0x03) << 1)
#define TMTRM                                    (1 << 3)
#define TMTARM                                   (1 << 4)
/* End of Transmit Buffer Status Register */

/* Transmit Buffer ID Register (p = 0 to 31) */
#define RCAR_CANFD_RSCFD0CFDTMID(p)              (0x4000 + (0x20 * (p)))
#define TMID(val)                                (((val) & 0x1FFFFFFF) << 0)
#define THLEN                                    (1 << 29)
#define TMRTR                                    (1 << 30)
#define TMIDE                                    (1 << 31)
/* End  of Transmit Buffer ID Register */

/* Transmit Buffer Pointer Register */
#define RCAR_CANFD_RSCFD0CFDTMPTR(p)             (0x4004 + (0x20 * (p)))
#define TMPTR(val)                               (((val) & 0xFF) << 16)
#define TMDLC(val)                               (((val) & 0x0F) << 28)
/* End of Transmit Buffer Pointer Register */

/* Transmit Buffer CAN FD Configuration Register */
#define RCAR_CANFD_RSCFD0CFDTMFDCTR(p)           (0x4008 + (0x20 * (p)))
#define TMESI                                    (1 << 0)
#define TMBRS                                    (1 << 1)
#define TMFDF                                    (1 << 2)
/* End of Transmit Buffer CAN FD Configuration Register */

/* Transmit Buffer Data Field b Register (b = 0 to 4, p = 0 to 31) */
#define RCAR_CANFD_RSCFD0CFDTMDF(b,p)            (0x400C + (0x04 * (b)) + (0x20 * (p)))
/* End of Transmit Buffer Data Field b Register */

/* Transmit Buffer Interrupt Enable Configuration Register */
#define RCAR_CANFD_RSCFD0CFDTMIEC(y)             (0x0390 + (0x04 * (y)))
#define TMIE(p)                                  (1 << (p))

#define RCAR_CANFD_RSCFD0CFDTMTRSTS(y)           (0x0350 + (0x04 * (y)))
#define RCAR_CANFD_RSCFD0CFDTMTARSTS(y)          (0x0360 + (0x04 * (y)))
#define RCAR_CANFD_RSCFD0CFDTMTCSTS(y)           (0x0370 + (0x04 * (y)))
#define RCAR_CANFD_RSCFD0CFDTMTASTS(y)           (0x0380 + (0x04 * (y)))

/* Transmit Queue Configuration and Control Register */
#define RCAR_CANFD_RSCFD0CFDTXQCC(m)             (0x03A0 + (0x04 * (m)))
#define TXQE                                     (1 << 0)
#define TXQDC(val)                               (((val) & 0x0F) << 8)
#define TXQIE                                    (1 << 12)
#define TXQIM                                    (1 << 13)
/* End of Transmit Queue Configuration and Control Register */

#define RCAR_CANFD_RSCFD0CFDTXQSTS(m)            (0x03C0 + (0x04 * (m)))
#define RCAR_CANFD_RSCFD0CFDTXQPCTR(m)           (0x03E0 + (0x04 * (m)))

/* Transmit History Configuration and Control Register */
#define RCAR_CANFD_RSCFD0CFDTHLCC(m)             (0x0400 + (0x04 * (m)))
#define THLE                                     (1 << 0)
#define THLIE                                    (1 << 8)
#define THLIM                                    (1 << 9)
#define THLDTE                                   (1 << 10)
/* End of Transmit History Configuration and Control Register */

/* Transmit History Status Register (m = 0-7) */
#define RCAR_CANFD_RSCFD0CFDTHLSTS(m)            (0x0420 + (0x04 * (m)))
#define THLELT                                   (1 << 2)
/* End of Transmit History Status Register */

#define RCAR_CANFD_RSCFD0CFDTHLPCTR(m)           (0x0440 + (0x04 * (m)))
#define RCAR_CANFD_RSCFD0CFDTHLACC(m)            (0x6000 + (0x04 * (m)))
 
#define RCAR_CANFD_RSCFD0CFDGTSTCFG              (0x0468)
#define RCAR_CANFD_RSCFD0CFDGTSTCTR              (0x046C)
#define RCAR_CANFD_RSCFD0CFDGLOCKK               (0x047C)
 
#define RCAR_CANFD_RSCFD0CFDRPGACC(r)            (0x6400 + (0x04 * (r)))
 
/* CANFD MASK */ 
#define CANFD_ID_MASK_STD                        (0x1FFC0000)
#define CANFD_ID_MASK_EXT                        (0x1FFFFFFF)

/* Default CANFD timestamp */
#define CANFD_DEFAULT_TX_INTERVAL     (75)



/* This formular calculates bitrate at sample point 80% */
#define CANFD_CLK                   ((product_id == PRODUCT_ID_RCAR_D3) ?(40000000) : (80000000))      /* Internal frequency (Hz) input to CANFD module */
#define fCAN                        (CANFD_CLK / 2)
#define CANFD_PRESCALER_N           (1)             /* Prescaler 2 */
#define CANFD_PRESCALER_N_MAX       (1024)
#define CANFD_PRESCALER_D           (1)             /* Prescaler 2 */
#define CANFD_PRESCALER_D_MAX       (256)
#define SAMPLE_POINT                (0.8f)          /* Fixed 80% */
#define SS                          (1)             /* Fixed */
#define SJW                         (1)             /* Fixed */

#define CAN_TSEG1_MIN               (4)
#define CAN_TSEG1_MAX               (16)
#define CAN_TSEG2_MIN               (2)
#define CAN_TSEG2_MAX               (8)

#define CANFD_NTSEG1_MIN            (4)
#define CANFD_NTSEG1_MAX            (128)
#define CANFD_NTSEG2_MIN            (2)
#define CANFD_NTSEG2_MAX            (32)

#define CANFD_DTSEG1_MIN            (2)
#define CANFD_DTSEG1_MAX            (16)
#define CANFD_DTSEG2_MIN            (2)
#define CANFD_DTSEG2_MAX            (8)

#define CANFD_NORMAL_BITRATE_MAX    (1000000)
#define CANFD_DATA_BITRATE_MAX      (5000000)

#define CANFD_TX_INTERVAL_UNIT      (5)             /* Timestamp unit : 5us */

/* transmition interval timer limitation */
#define CANFD_TX_INTERVAL_MAX       (255 * CANFD_TX_INTERVAL_UNIT)
#define CANFD_TX_INTERVAL_MIN       (0)
    
/* Timeout */   
#define CANFD_TIMEOUT               (20000)

/* Transmition in-progress check flag*/
#define CANDEV_TX_ENABLED           0x00000001

/* Initial TX/RX IDs */
typedef enum
{
    RX0_C0_ID = 0x00000001,
    RX1_C0_ID = 0x00000002,
    RX2_C0_ID = 0x00000003,
    RX3_C0_ID = 0x00000004,
    TX0_C0_ID = 0x00000001,

    RX0_C1_ID = 0x00000011,
    RX1_C1_ID = 0x00000012,
    RX2_C1_ID = 0x00000013,
    RX3_C1_ID = 0x00000014,
    TX0_C1_ID = 0x00000011,

    RX0_C2_ID = 0x00000021,
    RX1_C2_ID = 0x00000022,
    RX2_C2_ID = 0x00000023,
    RX3_C2_ID = 0x00000024,
    TX0_C2_ID = 0x00000021,

    RX0_C3_ID = 0x00000031,
    RX1_C3_ID = 0x00000032,
    RX2_C3_ID = 0x00000033,
    RX3_C3_ID = 0x00000034,
    TX0_C3_ID = 0x00000031,

    RX0_C4_ID = 0x00000041,
    RX1_C4_ID = 0x00000042,
    RX2_C4_ID = 0x00000043,
    RX3_C4_ID = 0x00000044,
    TX0_C4_ID = 0x00000041,

    RX0_C5_ID = 0x00000051,
    RX1_C5_ID = 0x00000052,
    RX2_C5_ID = 0x00000053,
    RX3_C5_ID = 0x00000054,
    TX0_C5_ID = 0x00000051,

    RX0_C6_ID = 0x00000061,
    RX1_C6_ID = 0x00000062,
    RX2_C6_ID = 0x00000063,
    RX3_C6_ID = 0x00000064,
    TX0_C6_ID = 0x00000061,

    RX0_C7_ID = 0x00000071,
    RX1_C7_ID = 0x00000072,
    RX2_C7_ID = 0x00000073,
    RX3_C7_ID = 0x00000074,
    TX0_C7_ID = 0x00000071,
} canfd_init_id_t;

/* Recovery Mode */
typedef enum
{
    SO11898_1 = 0,
    ENTRT_CHANNEL_HALT_MODE_AT_BUS_OFF_ENTRY,
    ENTRT_CHANNEL_HALT_MODE_AT_BUS_OFF_END,
    ENTRT_CHANNEL_HALT_MODE_BY_PROGRAM
} canfd_recovery_modes_t;

/* global operation mode */
typedef enum
{
    GLOBAL_OPERAING_MODE = 0,
    GLOBAL_RESET_MODE,
    GLOBAL_TEST_MODE,
} canfd_global_modes_t;

/* Channel modes */
typedef enum
{
    STANDARD_TEST_MODE = 0,
    LISTEN_ONLY_MODE,
    EXTERNAL_LOOP_BACK_MODE,
    INTERNAL_LOOP_BACK_MODE,
    CHANNEL_COMMUNICATION_MODE,
    CHANNEL_RESET_MODE,
    CHANNEL_HALT_MODE
} canfd_channel_modes_t;

/* Channel options */
typedef enum
{
    CHANNEL_0 = 0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,
    CHANNEL_7
} canfd_channel_t;

/* Interface options */
typedef enum
{
    CLASSIC_CAN_MODE = 0,
    CANFD_MODE
} canfd_interface_t;

/* Type of clock */
typedef enum
{
    INTERNAL_CLOCK_MODE = 0,
    EXTERNAL_CLOCK_MODE
} canfd_clock_type_t;

/* CANFD HWINFO info */
typedef struct canfddev_hwinfo
{
    paddr_t                 regbase;
    paddr_t                 reglen;
    paddr_t                 membase;
    unsigned                irqvector[2];       /* irqvector[0] store IRQ_G, irqvector[1] store IRQ_L */
    uint32_t                clksrc_rate;
} CANFDDEV_HWINFO;

/* Type of frame */
typedef enum
{
    CLASSICAL_CAN_FRAME = 0,                   /* Classical frame */
    CANFD_FRAME = 1 << 2                       /* CANFD frame */
} canfd_frame_type_t;

typedef struct
{
    uint8_t                 mode;
    uint32_t                rxid[CANFD_RECEIVE_FIFO_BUFFER_MAX];                    /* ID */
    uint32_t                txid[CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER_MAX];           /* Message Identifier either 11 or 29 bits long */
    uint8_t                 txbrs;                                                  /* Bitrate switch */
    uint8_t                 frameType;                                              /* Either Classical CAN frame or CANFD frame */
    uint32_t                tx_interval[CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER_MAX];    /* Initial value for transmition interval */
} canfd_channel_init_t;

typedef struct
{
    CANDEV_INIT             cinit;              /* CAN Device initialization */

    uint8_t                 interface;          /* Classic CAN or CANFD */
    uint8_t                 clks;               /* Select can clock mode: internal or external */

                                                /* Set baudrate for CAN */
    uint8_t                 brp;                /* Bit Rate Prescaler Division Ratio Set */
    uint8_t                 sjw;                /* Bit Rate Resynchronization Jump Width Control */
    uint8_t                 tseg1;              /* Bit Rate Time Segment 1 Control */
    uint8_t                 tseg2;              /* Bit Rate Time Segment 2 Control*/

                                                /* Set baudrate for CANFD */
    uint8_t                 nbrp;               /* Normal Bit Rate Prescaler Division Ratio Set */
    uint8_t                 nsjw;               /* Normal Bit Rate Resynchronization Jump Width Control */
    uint8_t                 ntseg1;             /* Normal Bit Rate Time Segment 1 Control */
    uint8_t                 ntseg2;             /* Normal Bit Rate Time Segment 2 Control*/

    uint8_t                 dbrp;               /* Data Bit Rate Prescaler Division Ratio Set*/
    uint8_t                 dsjw;               /* Data Bit Rate Resynchronization Jump Width Control*/
    uint8_t                 dtseg1;             /* Data Bit Rate Time Segment 1 Control*/
    uint8_t                 dtseg2;             /* Data Bit Rate Time Segment 2 Control*/
    CANFDDEV_HWINFO         canfd_hwi;          /* Info pulled in from HWINFO stored here */
    canfd_channel_init_t    channel[CANFD_TOTAL_CHANNELS_MAX];
} candev_init_t;

struct  _candev_exp_;

typedef struct
{
    uintptr_t               base;               /* Device Virtual Register Mapping */
    struct  _candev_exp_    *candev_list;
    candev_init_t           *candev_init;
} candev_info_t;

typedef struct  _candev_exp_
{
    CANDEV                  candev;
    uint8_t                 channel;
    uint8_t                 index;
    volatile uint32_t       dflags;
    candev_info_t           *candev_info;
} candev_exp_t;

extern int32_t  product_id;
extern int32_t  product_rev;

void canfd_transmit_cb(CANDEV *cdev);
int  canfd_control_cb(CANDEV *cdev, int option, DCMD_DATA *dcmd_data);
void canfd_init_hw(candev_init_t    *candev_init, candev_info_t *candev_info);
void canfd_init_rx_irq(candev_exp_t *candev_exp);
void canfd_init_tx_irq(candev_exp_t *candev_exp);

#endif
