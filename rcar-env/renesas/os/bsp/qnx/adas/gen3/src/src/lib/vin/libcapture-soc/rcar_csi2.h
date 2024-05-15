/*
* $QNXLicenseC:
* Copyright 2020, QNX Software Systems.
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


#ifndef __CSI2_H__
#define __CSI2_H__

#include "common.h"
#include "rcar_vin.h"

#define RCAR_CSI2_UYVY_DT_FORMAT			0x1e
#define RCAR_CSI2_RGB888_DT_FORMAT			0x24

/* Bits defined */
#define RCAR_CSI2_LSWAP_L3SEL(n)			(((n) & 0x3) << 6)
#define RCAR_CSI2_LSWAP_L2SEL(n)			(((n) & 0x3) << 4)
#define RCAR_CSI2_LSWAP_L1SEL(n)			(((n) & 0x3) << 2)
#define RCAR_CSI2_LSWAP_L0SEL(n)			(((n) & 0x3) << 0)
#define RCAR_CSI2_PHYCNT_SHUTDOWNZ          (1 << 17)
#define RCAR_CSI2_PHYCNT_RSTZ               (1 << 16)
#define RCAR_CSI2_PHYCNT_ENABLECLK          (1 << 4)
#define RCAR_CSI2_PHYCNT_ENABLE_3           (1 << 3)
#define RCAR_CSI2_PHYCNT_ENABLE_2           (1 << 2)
#define RCAR_CSI2_PHYCNT_ENABLE_1           (1 << 1)
#define RCAR_CSI2_PHYCNT_ENABLE_0           (1 << 0)
#define RCAR_CSI2_VCDT_VCDT_EN              (1 << 15)
#define RCAR_CSI2_VCDT_VCDT2_EN             (1 << 31)
#define RCAR_CSI2_VCDT_VCDT3_EN             (1 << 15)
#define RCAR_CSI2_VCDT_VCDT4_EN             (1 << 31)
#define RCAR_CSI2_VCDT_SEL_VC(n)			(((n) & 0x3) << 8)
#define RCAR_CSI2_VCDT_SEL_VC2(n)			(((n) & 0x3) << 24)
#define RCAR_CSI2_VCDT_SEL_VC3(n)			(((n) & 0x3) << 8)
#define RCAR_CSI2_VCDT_SEL_VC4(n)			(((n) & 0x3) << 24)
#define RCAR_CSI2_VCDT_SEL_DTN_ON           (1 << 6)
#define RCAR_CSI2_VCDT_SEL_DTN              (1 << 0)
#define RCAR_CSI2_LINKCNT_MONITOR_EN        (1 << 31)
#define RCAR_CSI2_LINKCNT_MONI_PACT_EN  	(1 << 25)
#define RCAR_CSI2_LINKCNT_ICLK_NONSTOP      (1 << 24)
#define RCAR_CSI2_FLD_FLD_NUM(n)			(((n) & 0xff) << 16)
#define RCAR_CSI2_FLD_FLD_EN4				(1 << 3)
#define RCAR_CSI2_FLD_FLD_EN3				(1 << 2)
#define RCAR_CSI2_FLD_FLD_EN2				(1 << 1)
#define RCAR_CSI2_FLD_FLD_EN				(1 << 0)
#define RCAR_GEN3_CSI2_CHKSUM_CRC_EN		(1 << 0)
#define RCAR_GEN3_CSI2_CHKSUM_ECC_EN		(1 << 1)
#define RCAR_GEN3_CSI2_FRDT_DT_FE			(1 << 16)
#define RCAR_CSI2_PHTC_TESTCLR              (1 << 0)
#define RCAR_CSI2_PHTW_DWEN					(1 << 24)
#define RCAR_CSI2_PHTW_TESTDIN_DATA(n)		(((n & 0xff)) << 16)
#define RCAR_CSI2_PHTW_CWEN					(1 << 8)
#define RCAR_CSI2_PHTW_TESTDIN_CODE(n)		((n & 0xff))

/* Interrupt status registers */
#define RCAR_CSI2_INTSTATE_EBD_CH1          (1 << 29)
#define RCAR_CSI2_INTSTATE_LESS_THAN_WC     (1 << 28)
#define RCAR_CSI2_INTSTATE_AFIFO_OF         (1 << 27)
#define RCAR_CSI2_INTSTATE_VD4_START        (1 << 26)
#define RCAR_CSI2_INTSTATE_VD4_END          (1 << 25)
#define RCAR_CSI2_INTSTATE_VD3_START        (1 << 24)
#define RCAR_CSI2_INTSTATE_VD3_END          (1 << 23)
#define RCAR_CSI2_INTSTATE_VD2_START        (1 << 22)
#define RCAR_CSI2_INTSTATE_VD2_END          (1 << 21)
#define RCAR_CSI2_INTSTATE_VD1_START        (1 << 20)
#define RCAR_CSI2_INTSTATE_VD1_END          (1 << 19)
#define RCAR_CSI2_INTSTATE_SHP              (1 << 18)
#define RCAR_CSI2_INTSTATE_FSFE             (1 << 17)
#define RCAR_CSI2_INTSTATE_LNP              (1 << 16)
#define RCAR_CSI2_INTSTATE_CRC_ERR          (1 << 15)
#define RCAR_CSI2_INTSTATE_HD_WC_ZERO       (1 << 14)
#define RCAR_CSI2_INTSTATE_FRM_SEQ_ERR1     (1 << 13)
#define RCAR_CSI2_INTSTATE_FRM_SEQ_ERR2     (1 << 12)
#define RCAR_CSI2_INTSTATE_ECC_ERR          (1 << 11)
#define RCAR_CSI2_INTSTATE_ECC_CRCT_ERR     (1 << 10)
#define RCAR_CSI2_INTSTATE_LPDT_START       (1 << 9)
#define RCAR_CSI2_INTSTATE_LPDT_END         (1 << 8)
#define RCAR_CSI2_INTSTATE_ULPS_START       (1 << 7)
#define RCAR_CSI2_INTSTATE_ULPS_END         (1 << 6)
#define RCAR_CSI2_INTSTATE_RESERVED         (1 << 5)
#define RCAR_CSI2_INTSTATE_ERRSOTHS         (1 << 4)
#define RCAR_CSI2_INTSTATE_ERRSOTSYNCCHS    (1 << 3)
#define RCAR_CSI2_INTSTATE_ERRESC           (1 << 2)
#define RCAR_CSI2_INTSTATE_ERRSYNCESC       (1 << 1)
#define RCAR_CSI2_INTSTATE_ERRCONTROL       (1 << 0)

/* Monitoring registers of interrupt error status */
#define RCAR_CSI2_INTSTATE_ECC_ERR          (1 << 11)
#define RCAR_CSI2_INTSTATE_ECC_CRCT_ERR     (1 << 10)
#define RCAR_CSI2_INTSTATE_LPDT_START       (1 << 9)
#define RCAR_CSI2_INTSTATE_LPDT_END         (1 << 8)
#define RCAR_CSI2_INTSTATE_ULPS_START       (1 << 7)
#define RCAR_CSI2_INTSTATE_ULPS_END         (1 << 6)
#define RCAR_CSI2_INTSTATE_RESERVED         (1 << 5)
#define RCAR_CSI2_INTSTATE_ERRSOTHS         (1 << 4)
#define RCAR_CSI2_INTSTATE_ERRSOTSYNCCHS    (1 << 3)
#define RCAR_CSI2_INTSTATE_ERRESC           (1 << 2)
#define RCAR_CSI2_INTSTATE_ERRSYNCESC       (1 << 1)
#define RCAR_CSI2_INTSTATE_ERRCONTROL       (1 << 0)

#define FREQ_MBPS_ERROR                    	0xFF

#endif // __CSI2_H__
