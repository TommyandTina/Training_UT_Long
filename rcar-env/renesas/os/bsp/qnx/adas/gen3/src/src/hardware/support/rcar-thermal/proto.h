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


#ifndef _IO_H_INCLUDED
#define _IO_H_INCLUDED

#include <sys/resmgr.h>
#include <sys/iomsg.h>
#include <sys/iofunc.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <sys/dispatch.h>
#include <sys/procmgr.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <hw/inout.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <devctl.h>
#include <aarch64/r-car-gen3.h>
#include <hw/thermal.h>

#define VERBOSE_QUIET       0
#define VERBOSE_LEVEL1      1
#define VERBOSE_LEVEL2      2
#define VERBOSE_LEVEL4      4
#define VERBOSE_LEVEL8      8

/* Macro to calculate voltage for B type sensor 
 * Voltage = (VOLT_CODE * 5 + 855)(mV) */
#define VOLT_CONVERT(volt_code)   (5 * volt_code + 855)

/*Resource manager value*/
// Max size of a message, should be max(IOBUF, devctl_t)
#define MAX_MSG_SIZE              IOBUF_SIZE
// Size of io_buffer used to for msg read/write with client
#define IOBUF_SIZE                1024
// Error Code
#define ENG                       512

#define THERMAL_PTAT_BASE         0xE6198000
#define PTAT_SIZE                 (RCAR_V3U_THERMAL_THSCP + 0x4)


/********************************************* STRUCTURE DEFINE *********************************************/
/* We have two type of device on Rcar*/
typedef enum
{
    TYPE_A = 1,
    TYPE_B,
    TYPE_C
} rcar_tsc_type_t;

/* Structure for TSC's information*/
typedef struct _rcar_tsc_
{
    /*TSC information*/
    paddr_t         phy_addr;
    uintptr_t       vaddr;
    uintptr_t       ecm_vaddr;
    int             reg_size;
    rcar_tsc_type_t type;
    int             ths_chan;
    int             product_id;
    int             product_rev;
    int             cur_temp;
    int             cur_volt;
    int             has_hw_code;
    int             tj_t;
    int            *thcode;
    int            *ptat;
    int            *ptat0;
    void           *dev;
} rcar_tsc_t;

/* Structure for resource manager channel information*/
typedef struct _rcar_tsc_dev_
{
    iofunc_attr_t       hdr;
    dispatch_t         *dpp;
    dispatch_context_t *ctp;
    int                 resmgr_id;
    int                 coid;
#define TSC_INTR_NUM    2   //Use 2 intrrupts Thermal Sensor.ch0 & Thermal Sensor.ch2    
    int                 intr_num;
    int                 chan_min;
    int                 chan_max;
    uintptr_t           ecm_vaddr;
    int                 intr_id[TSC_INTR_NUM];
    int                 intr_irq[TSC_INTR_NUM];
    int                 intr_pulse_code[TSC_INTR_NUM];
    struct sigevent     event[TSC_INTR_NUM];
    rcar_tsc_t         *tsc[TSC_MAX_NUM];
} rcar_tsc_dev_t;

int rcar_thermal_dev_init(rcar_tsc_dev_t *dev);
void rcar_thermal_dev_deinit(rcar_tsc_dev_t *dev);
int rcar_thermal_irq(rcar_tsc_t * tsc);
int rcar_thermal_get_temp(rcar_tsc_t * tsc, int *temp);
int rcar_thermal_get_vol(rcar_tsc_t * tsc, int *volt);
int thermal_slogf(const int minVerbose, const char *fmt, ...);
int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);
int thermal_get_temperature(thermal_packet_t *temp);
int thermal_get_voltage(thermal_packet_t *vol);
int thermal_voltage_monitor(thermal_packet_t *mon);

#endif /* _IO_H_INCLUDED */
