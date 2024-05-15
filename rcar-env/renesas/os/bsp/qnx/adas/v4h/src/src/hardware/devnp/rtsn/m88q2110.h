/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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

#ifndef M88Q2110_H
#define M88Q2110_H

#define MV_PCS_DEV   3

#ifndef MARVELLX
#define MARVELLX 0xac2
#endif

#define GIGE_T1_CONTROL_REG           0x0900
#define GIGE_T1_CONTROL_REG_RESET     (1 << 15)
#define GIGE_T1_CONTROL_REG_LOW_PWR   (1 << 11)


#define GIGE_T1_STATUS_REG     0x0901
#define GIGE_T1_STATUS_REG_LINK_STATUS     (1 << 2)

#define AUTONEG_STATUS_REG     0x8001
#define AUTONEG_STATUS_REG_LOCAL_RX_STATUS  (1 << 13)
#define AUTONEG_STATUS_REG_REMOTE_RX_STATUS (1 << 12)
#define AUTONEG_STATUS_BOTH_RX_STATUS  \
	(AUTONEG_STATUS_REG_LOCAL_RX_STATUS | AUTONEG_STATUS_REG_REMOTE_RX_STATUS)

#define PMA_PMD_CTRL           0x834

typedef enum {
	M88Q2110_A0 = 0x1,
	M88Q2110_A1 = 0x2,
	M88Q2110_A2 = 0x3,
} m88q2110_rev_t ;

#define MRVL_88Q2112_OP_MODE_LEGACY		0x06B0
#define MRVL_88Q2112_OP_MODE_DEFAULT	0x0000

void mv_mii_write(void *handle, uint8_t dev_id,
				  uint16_t location, uint16_t data);
uint16_t mv_mii_read(void *handle, uint8_t dev_id,
					 uint16_t location);

#endif /* M88Q2110_H */
