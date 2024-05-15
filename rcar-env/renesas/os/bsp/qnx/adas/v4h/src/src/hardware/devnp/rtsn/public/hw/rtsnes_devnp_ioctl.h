/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
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

#ifndef RTSNES_DEVNP_IOCTL_H
#define RTSNES_DEVNP_IOCTL_H

#include <stdint.h>


#define RTSNES_IOCTL SIOCGDRVSPEC

#define GENERIC_MII_OP		0x1005

/*mii op*/
#define SLEEP		0
#define READ		1
#define WRITE		2
#define	CL45_READ	3
#define	CL45_WRITE	4
#define	SW_READ		5
#define	SW_WRITE	6

union phy_ops_t {
	/* total size of mii_op follow */
	struct {
		uint8_t		op;             /* mii op,  0 = sleep; 1 = read, 2 = write */
		uint8_t		addr;           /* mii MDIO phy address  */
		uint8_t		reg;            /* mii register address  */
		uint16_t	data;           /* for read, data is the value read from register, for write, data is the value write to the register, for sleep, it's the ms to sleep*/
		int			err;            /* result of this access/sleep. 0 means success. otherwise failed*/
	} mii_op;
	struct {
		uint8_t		op;
		uint8_t     phy_id;
		uint8_t     phy_dev;
		uint16_t    phy_reg;
		uint16_t    phy_data;
		int			err;
	} cl45_op;
};

typedef struct {
    uint32_t	        size;
    union phy_ops_t		phy_ops;
} rtsnes_mii_ops_req_t;

#endif


