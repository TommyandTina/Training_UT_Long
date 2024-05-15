/*
* $QNXLicenseC:
* Copyright 2020, QNX Software Systems.
* Copyright 2020, Renesas Electronics Corporation
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

#ifndef __ISP_H__
#define __ISP_H__

#include "common.h"
#include "rcar_vin.h"
#include "rcar_csi2.h"
#include "soc.h"


#define RCAR_ISPCS_DT_CODE03_ENABLE0            (1 << 7)
#define RCAR_ISPCS_DT_CODE03_ENABLE1            (1 << 15)
#define RCAR_ISPCS_DT_CODE03_ENABLE2            (1 << 23)
#define RCAR_ISPCS_DT_CODE03_ENABLE3            (1 << 31)

#define	RCAR_ISPWP_CTRL_UNLOCK_CODE_U           (0xC97E << 16)
#define	RCAR_ISPWP_CTRL_UNLOCK_CODE_L           (0xFB69 << 0)

#define RCAR_ISPINPUTSEL0_SEL_CSI0              (1 << 31)
#define RCAR_ISPINPUTSEL0_TRACE_SEL0(n)         ((n & 0xF) << 8)
#define RCAR_ISPINPUTSEL0_TRACE_SEL1(n)         ((n & 0xF) << 12)

#define	RCAR_ISP_PADDING_CTRL_PADDING_NUM(n)    ((n & 0xFF) << 0)

#define RCAR_ISP_PROCMODE_DT_RGB888             (0x15 << 0)
#define RCAR_ISP_PROCMODE_DT_YU422_8BIT         (0x0C << 0)
#define RCAR_ISP_PROCMODE_DT_YU422_10BIT         (0x0d << 0)
#define RCAR_ISP_PROCMODE_DT_RAW10              (0x01 << 0)

#define RCAR_ISPSTART_START_ISP                 (0xFFFF << 0)
#define RCAR_ISPSTART_STOP_ISP                  (0x0000 << 0)
	
#define	RCAR_ISPCS_OUTPUT_MODE_OUTPUT_SEL(n)	((n & 0x3) << 24)

#define	RCAR_ISPFIFOCTRL_FIFO_PUSH_CSI          (1 << 2)
#define	RCAR_ISPFIFOCTRL_TRACE_SEL2(n)          ((n & 0xF) << 8)
#define	RCAR_ISPFIFOCTRL_TRACE_SEL3(n)          ((n & 0xF) << 12)

int rcar_isp_init(rcar_vin_t *vin, int isp_chan);
void rcar_isp_fini(rcar_vin_t *vin);

#endif // __ISP_H__
