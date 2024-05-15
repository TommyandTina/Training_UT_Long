/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <sys/resmgr.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <hw/gpio.h>

 /**
 * @brief  Definitions
 */
#define	MAX_IOV_SIZE	4			// max ctp->iov[] size
#define	IOBUF_SIZE		512		    // size of io_buffer used to for msg read/write with client
#define	MAX_MSG_SIZE	IOBUF_SIZE	// max size of a message (should be max(IOBUF, devctl_t))
#define	ENG				512			// Error Code


#define GPIO_NUM		8
#define GPIO_PORT		32
//#define DUMP_REGISTERS  1


/**
 * @brief GPIO structure
 */
typedef struct _gen4_gpio_reg_t {
    volatile uint32_t   IOINTSEL;
    volatile uint32_t   INOUTSEL;
    volatile uint32_t   OUTDT;
    volatile uint32_t   INDT;
    volatile uint32_t   INTDT;
    volatile uint32_t   INTCLR;
    volatile uint32_t   INTMSK;
    volatile uint32_t   MSKCLR;
    volatile uint32_t   POSNEG;
    volatile uint32_t   EDGLEVEL;
    volatile uint32_t   FILONOFF;
    volatile uint32_t   FILCLKSEL;
    volatile uint32_t   reserved[4];
    volatile uint32_t   OUTDTSEL;
    volatile uint32_t   OUTDTH;
    volatile uint32_t   OUTDTL;
    volatile uint32_t   BOTHEDGE;
    volatile uint32_t   INEN;
} gen4_gpio_reg_t;

/**
 * @brief PFC structure
 */
typedef struct {
    volatile uint32_t   PMMR;
    volatile uint32_t   reserved0[15];
    volatile uint32_t   GPSR;
    volatile uint32_t   reserved1[31];
    volatile uint32_t   PUEN;
    volatile uint32_t   reserved2[7];
    volatile uint32_t   PUD;

} gen4_pfc_reg_t;

typedef struct _gen3_gpio_reg_t {
    volatile uint32_t   IOINTSEL;
    volatile uint32_t   INOUTSEL;
    volatile uint32_t   OUTDT;
    volatile uint32_t   INDT;
    volatile uint32_t   INTDT;
    volatile uint32_t   INTCLR;
    volatile uint32_t   INTMSK;
    volatile uint32_t   MSKCLR;
    volatile uint32_t   POSNEG;
    volatile uint32_t   EDGLEVEL;
    volatile uint32_t   FILONOFF;
    volatile uint32_t   reserved[3];
    volatile uint32_t   INTMSKS;
    volatile uint32_t   MSKCLRS;
    volatile uint32_t   OUTDTSEL;
    volatile uint32_t   OUTDTH;
    volatile uint32_t   OUTDTL;
    volatile uint32_t   BOTHEDGE;
} gen3_gpio_reg_t;

typedef struct {
    volatile uint32_t   PMMR;
    volatile uint32_t   reserved0[63];
    volatile uint32_t   GPSR[GPIO_NUM];
} gen3_pfc_reg_t;

/**
 * @brief GPIO info structure
 */
typedef struct {
    uint32_t	pfc_phybase; // PFC phy base
    uint32_t	pfc_size;
    uintptr_t   pfc_reg;     // PFC vbase 
    uintptr_t   gpio_reg;    // GPIO vbase 
    uint32_t	phybase;     // -p : the base address
    uint32_t	size;
    int         group;       // -g : the GPIO index
    int         groups;
    int         verbose;     // -v : verbose operation
    int         irq;         // -i : IRQ
    int         gen3_soc_type; // 0 : V3U or S4; 1 : Other SoC
} gpio_drv_info_t;

/**
 * @brief Define the functions
 */
void gpio_slogf(const int minVerbose, const char *fmt, ...);

/**
 * @brief Definitions for IO
 */
int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra);
int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);

#endif  //IO_H_INCLUDED
