/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resmgr.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <errno.h>
#include <sys/procmgr.h>
#include <drvr/hwinfo.h>
#include <aarch64/r-car.h>

#define MAX_NUM_REG		1
#define DEFAULT_REG_LEN	0x1000

typedef enum {
	BITSWAP,
	WRITE,
	NUM_OPERATIONS
} operation_id_t;

typedef enum {
	REG_OFFSET,
	REG_VAL
} reg_pairs_t;

static int isValidRange(uint32_t val, int err)
{
	if(val != 0 && err == ERANGE) {
		return 0;
	}
	return 1;
}

static int isValidSeparator(char separator) {
	if(separator == ':') {
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int opt;
	int priority = 10;	/*default priority:default 10 */
	int time = -1;		/*default time for watchdog timer kick*/
	uintptr_t base = 0;
	size_t len = DEFAULT_REG_LEN;
	paddr_t physbase = 0;
	hwiattr_timer_t wdog_attr;
	unsigned hwi_off;
	int r;
	struct sched_param threadparam;
	int sched_type;
	uint32_t enable_mask = 0;
	uint32_t enable_condition = 0;
	uint32_t enable_reg_offset = RCAR_WDT_CSRA;
	uint32_t write_reg[MAX_NUM_REG][2] = {{0, 0}, };
	uint32_t num_writes = 0;
	hwi_tag  *tag;
	unsigned  tag_idx;
	int operation = -1;
	int verbose = 0;

	/* Getting the WDOG Base addresss from the Hwinfo Section if available */
	hwi_off = hwi_find_device("wdog", 0);
	if (hwi_off != HWI_NULL_OFF) {
		r = hwiattr_get_timer(hwi_off, &wdog_attr);
		if (r == EOK) {
			if(wdog_attr.common.location.len > len) {
				len = wdog_attr.common.location.len;
			}
			if(wdog_attr.common.location.base > 0)
				physbase = wdog_attr.common.location.base;
			time = hwitag_find_clkfreq(hwi_off, NULL);
		}
	}

	/* Getting the WDOG options from the Hwinfo Section if available */
	hwi_off = hwi_find_device("wdt,options", 0);
	if (hwi_off != HWI_NULL_OFF) {
		tag_idx = 0;
		while(((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL)) {
			if (strcmp("enable_mask",  __hwi_find_string(tag->regname.regname)) == 0) {
				enable_mask = tag->regname.offset;
			} else if (strcmp("enable_condition",  __hwi_find_string(tag->regname.regname)) == 0) {
				enable_condition = tag->regname.offset;
			} else if(strcmp("enable_offset",  __hwi_find_string(tag->regname.regname)) == 0) {
				enable_reg_offset = tag->regname.offset;
				if(enable_reg_offset > len) {
					len = enable_reg_offset;
				}
			} else {
				slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : %s is not a valid Hwinfo default setting.", \
					__hwi_find_string(tag->regname.regname));
				break;
			}
		}
	}

	if ((hwi_off = hwi_find_device("wdt,bitswap", 0)) != HWI_NULL_OFF) {
		operation = BITSWAP;
	} else if ((hwi_off = hwi_find_device("wdt,regwrite", 0)) != HWI_NULL_OFF) {
		operation = WRITE;
	}

	tag_idx = 0;
	while ((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
		if (strcmp("offset",  __hwi_find_string(tag->regname.regname)) == 0) {
			write_reg[num_writes][REG_VAL] = tag->regname.offset;
		} else if (strcmp("value",  __hwi_find_string(tag->regname.regname)) == 0) {
			write_reg[num_writes][REG_VAL] = tag->regname.offset;
		} else {
			slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid offset/value entry. Please check the command line or Hwinfo default setting.");
			return EXIT_FAILURE;
		}
	}

	/* Process dash options.*/
	while ((opt = getopt(argc, argv, "a:l:p:t:m:R:E:B:W:v")) != -1) {
		switch (opt) {
			case 'a':	// WDOG register physical base address
				physbase = strtoull(optarg, NULL, 0);
				break;
			case 'l':
				len = strtoul(optarg, NULL, 0);
				break;
			case 'p':	// priority
				priority = strtoul(optarg, NULL, 0);
				break;
			case 't':
				time = strtoul(optarg, NULL, 0);
				break;
			case 'E':
				enable_reg_offset = strtol(optarg, &optarg, 16);
				if(enable_reg_offset > len) {
					len = enable_reg_offset;
				}
				if(!isValidRange(enable_reg_offset, errno)) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Enable offset is out of range. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}
				if(!isValidSeparator(optarg[0])) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid separator for option '-E'. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}

				if(optarg[1] == 0) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid args for option '-E'. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}

				enable_mask = strtoul(optarg + 1, &optarg, 16);
				if(!isValidRange(enable_mask, errno)) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Enable mask is out of range. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}

				if(optarg[0] == 0) {
					enable_condition = enable_mask; //If the enable condition is not present, set it to enable_mask
				} else if(!isValidSeparator(optarg[0])) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid separator for option '-E'. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				} else {
					enable_condition = strtoul(optarg + 1, NULL, 16);
					if(enable_condition != 0) {
						if(!isValidRange(enable_condition, errno)) {
							slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Enable condition is out of range. Please check "
								"the command line or Hwinfo default setting.");
							return EXIT_FAILURE;
						}
					} else {
						enable_condition = enable_mask; //If the enable condition is not present, set it to enable_mask
					}
				}
				break;
			case 'B':
				write_reg[num_writes][REG_OFFSET] = strtol(optarg, &optarg, 16);
				if(!isValidRange(write_reg[num_writes][REG_OFFSET], errno)) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Offset to bitswap is out of range. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}

				if(write_reg[num_writes][REG_OFFSET] > len) {
					len = write_reg[num_writes][REG_OFFSET];
				}

				if(optarg[0] != 0 && !isValidSeparator(optarg[0])) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid separator for option '-B'. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}

				write_reg[num_writes][REG_VAL] = strtoul(optarg + 1, NULL, 16);
				if(write_reg[num_writes][REG_VAL] != 0) {
					if(!isValidRange(write_reg[num_writes][REG_VAL], errno)) {
						slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Value to bitswap is out of range. Please check "
							"the command line or Hwinfo default setting.");
						return EXIT_FAILURE;
					}
				} else {
					//Default to 0xFFFFFFFF for bitswap if no value entered
					write_reg[num_writes][REG_VAL] = 0xFFFFFFFF;
				}
				operation = BITSWAP;
				break;
			case 'W':
				write_reg[num_writes][REG_OFFSET] = strtol(optarg, &optarg, 16);
				if(!isValidRange(write_reg[num_writes][REG_OFFSET], errno)) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Offset to write is out of range. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}

				if(write_reg[num_writes][REG_OFFSET] > len) {
					len = write_reg[num_writes][REG_OFFSET];
				}

				if(!isValidSeparator(optarg[0])) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid separator for option '-W'. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}

				write_reg[num_writes][REG_VAL] = strtoul(optarg + 1, NULL, 16);
				if(!isValidRange(write_reg[num_writes][REG_VAL], errno)) {
					slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Value to write is out of range. Please check "
						"the command line or Hwinfo default setting.");
					return EXIT_FAILURE;
				}

				operation = WRITE;
				break;
			case 'v':
				verbose++;
				break;
		}
	}

	/*check if the params are valid*/
	if (physbase == 0) {
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid  WDOG register physical base address. Please check the command line or Hwinfo default setting.");
		return EXIT_FAILURE;
	}
	if (time == -1) {
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid default time for watchdog timer kick. Please check the command line or Hwinfo default setting.");
		return EXIT_FAILURE;
	}
	if (operation == -1) {
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick error : Invalid operation for watchdog timer kick. Please check the command line or Hwinfo default setting.");
		return EXIT_FAILURE;
	}

	// Enable IO capability.
	if ( ThreadCtl( _NTO_TCTL_IO, NULL ) == -1 ) {
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick: failure to acquire IO privity");
		return EXIT_FAILURE;
	}
	//run in the background
	if ( procmgr_daemon( EXIT_SUCCESS, PROCMGR_DAEMON_NOCLOSE | PROCMGR_DAEMON_NODEVNULL ) == -1 ) {
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"%s:  procmgr_daemon",argv[0]);
		return EXIT_FAILURE;
	}

	// configure information
	if (verbose) {
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "wdtkick: paddr = 0x%zx, size = 0x%zx, kick = %d, priority = %d",
			  physbase, len, time, priority);
	}

	// Set priority
	if (pthread_getschedparam(pthread_self(),&sched_type, &threadparam) != EOK){
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_WARNING,"wdtkick: get priority request failed");
		return EXIT_FAILURE;
	}
	if (priority != threadparam.sched_priority){
		threadparam.sched_priority = priority;
		if (pthread_setschedparam(pthread_self(),sched_type, &threadparam) != EOK) {
			slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_WARNING,"wdtkick: can't change priority");
		}
	}

	base = mmap_device_io(len, physbase);
	if (base == MAP_DEVICE_FAILED) {
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick: failed to map watchdog timer registers");
		return EXIT_FAILURE;
	}

	slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick: enable mask: 0x%x", enable_mask);

	// Configure the enable register
	enable_mask &= 0xFF;
	if (enable_mask && ((in8(base + enable_reg_offset) & enable_mask) != enable_mask)) {
		slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO,"wdtkick: enabling the watchdog timer");
		out32(base + enable_reg_offset, (0xA5A5A5 << 8) | in8(base + enable_reg_offset) | enable_mask);
	}

	// Validate 16-bit value to write to Timer Counter register
	write_reg[num_writes][REG_VAL] &= 0xFFFF;

	while (1) {
		if (operation == BITSWAP) {
			out32(base + write_reg[num_writes][REG_OFFSET], (0x5A5A << 16) | (in16(base + write_reg[num_writes][REG_OFFSET]) ^ write_reg[num_writes][REG_VAL]));
		} else if (operation == WRITE) {
			out32(base + write_reg[num_writes][REG_OFFSET], (0x5A5A << 16) | write_reg[num_writes][REG_VAL]);
		}
		delay(time);
	}

	return EXIT_SUCCESS;
}
