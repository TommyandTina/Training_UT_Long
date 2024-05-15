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

#include <stdio.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <atomic.h>
#include <sys/syspage.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>
#include <sys/mman.h>
#include <aarch64/r-car-gen3.h>
#include "proto.h"


struct tmu_data_s {
    uintptr_t	paddr;
    int         tcor;
    int         tcnt;
    int         tcr;
    int         tcpr;
    int         tstrt_mask;
    int			undr_irq;
    int			incap_irq;
};

static struct tmu_data_s gen3_tmu_data[] = {
    /* Channel 0 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU00, -1 },
    /* Channel 1 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU01, -1 },
    /* Channel 2 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, 0,                   TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU02, -1 },
    /* Channel 3 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU10, -1 },
    /* Channel 4 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU11, -1 },
    /* Channel 5 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU12, RCAR_GEN3_INTCSYS_TMU13 },
    /* Channel 6 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU20, -1 },
    /* Channel 7 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU21, -1 },
    /* Channel 8 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU22, RCAR_GEN3_INTCSYS_TMU23 },
    /* Channel 9 */  { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU30, -1 },
    /* Channel 10 */ { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU31, -1 },
    /* Channel 11 */ { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU32, RCAR_GEN3_INTCSYS_TMU33 },    
    /* Channel 12 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU40, -1 },
    /* Channel 13 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU41, -1 },
    /* Channel 14 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU42, RCAR_GEN3_INTCSYS_TMU43 },
};


static struct tmu_data_s v3mh_tmu_info[] = {
    /* Channel 0 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU00, -1 },
    /* Channel 1 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU01, -1 },
    /* Channel 2 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, 0,                   TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU02, -1 },
    /* Channel 3 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU10, -1 },
    /* Channel 4 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU11, -1 },
    /* Channel 5 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU12, RCAR_GEN3_INTCSYS_TMU13 },
    /* Channel 6 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU20, -1 },
    /* Channel 7 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU21, -1 },
    /* Channel 8 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU22, RCAR_GEN3_INTCSYS_TMU23 },
    /* Channel 9 */  { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_TMU30, -1 },
    /* Channel 10 */ { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_TMU31, -1 },
    /* Channel 11 */ { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_TMU32, RCAR_GEN3_INTCSYS_TMU33 },    
    /* Channel 12 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_GEN3_INTCSYS_V3_TMU40, -1 },
    /* Channel 13 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_GEN3_INTCSYS_V3_TMU41, -1 },
    /* Channel 14 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_GEN3_INTCSYS_V3_TMU42, RCAR_GEN3_INTCSYS_TMU43 },
};

static struct tmu_data_s v3u_tmu_data[] = {
    /* Channel 0 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_V3U_INTCSYS_TMU00, -1 },
    /* Channel 1 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_V3U_INTCSYS_TMU01, -1 },
    /* Channel 2 */  { RCAR_GEN3_TMU_0_2_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, 0,                   TMU_TSTRT_STR2,  RCAR_V3U_INTCSYS_TMU02, -1 },
    /* Channel 3 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_V3U_INTCSYS_TMU10, -1 },
    /* Channel 4 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_V3U_INTCSYS_TMU11, -1 },
    /* Channel 5 */  { RCAR_GEN3_TMU_3_5_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_V3U_INTCSYS_TMU12, RCAR_V3U_INTCSYS_TMU13 },
    /* Channel 6 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_V3U_INTCSYS_TMU20, -1 },
    /* Channel 7 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_V3U_INTCSYS_TMU21, -1 },
    /* Channel 8 */  { RCAR_GEN3_TMU_6_8_BASE,   RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_V3U_INTCSYS_TMU22, RCAR_V3U_INTCSYS_TMU23 },
    /* Channel 9 */  { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_V3U_INTCSYS_TMU30, -1 },
    /* Channel 10 */ { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_V3U_INTCSYS_TMU31, -1 },
    /* Channel 11 */ { RCAR_GEN3_TMU_9_11_BASE,  RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_V3U_INTCSYS_TMU32, RCAR_V3U_INTCSYS_TMU33 },    
    /* Channel 12 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR0, RCAR_GEN3_TMU_TCNT0, RCAR_GEN3_TMU_TCR0, 0,                   TMU_TSTRT_STR0,  RCAR_V3U_INTCSYS_TMU40, -1 },
    /* Channel 13 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR1, RCAR_GEN3_TMU_TCNT1, RCAR_GEN3_TMU_TCR1, 0,                   TMU_TSTRT_STR1,  RCAR_V3U_INTCSYS_TMU41, -1 },
    /* Channel 14 */ { RCAR_GEN3_TMU_12_14_BASE, RCAR_GEN3_TMU_TCOR2, RCAR_GEN3_TMU_TCNT2, RCAR_GEN3_TMU_TCR2, RCAR_GEN3_TMU_TCPR2, TMU_TSTRT_STR2,  RCAR_V3U_INTCSYS_TMU42, RCAR_V3U_INTCSYS_TMU43 },
};

/* TMU has 5 Groups TMU0, TMU1, TMU2, TMU3, TMU4 */
#define RCAR_TMU_MAX_MMAP_GROUPS        5   /* support groups TMU0/1/2/3/4 */

typedef struct
{
    paddr_t         paddr;
    uintptr_t       vaddr;
    unsigned        num;    // attach count
} tmu_mem_mmap_t;

tmu_mem_mmap_t tmu_mem_mmap[RCAR_TMU_MAX_MMAP_GROUPS] = { {0, 0}, };

static int rcar_moder_reg = 0;
int soc_id = 0;

static int rcar_tmu_incap_support(int chan_idx)
{
    switch(chan_idx){
    case 5:
    case 8:
    case 11:
    case 14:
        return 1;
    default:
        return 0;
    }
}

static int rcar_tmu_extclock_support(int chan_idx)
{
    switch(chan_idx){
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        return 1;
    default:
        return 0;
    }
}

static int rcar_tmu_interclock_support(int chan_idx)
{
    switch(chan_idx){
    case 0:
    case 1:
    case 2:
        return SYSPAGE_ENTRY(qtime)->cycles_per_sec;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12: 
    case 13:
    case 14:
        if(soc_id == PRODUCT_ID_RCAR_E3) {
            return (rcar_moder_reg & (1 << 12)) ? RCAR_GEN3_CPG_125: RCAR_GEN3_CPG_133;
        } else {
            if(SYSPAGE_ENTRY(qtime)->cycles_per_sec == RCAR_GEN3_EXT_CLK_XS / 2) { /* Salvator-XS */
                return 16 * SYSPAGE_ENTRY(qtime)->cycles_per_sec;
            } else {
                return RCAR_GEN3_CPG_133;
            }
        }
    default:
        return 0;
    }
}

static int rcar_tmu_init(char* options)
{
    const char  *option = NULL;
    int         opt;
    char        *value;
    static char	*opts[] = { "verbose", "soc", NULL };

    while( *options != '\0' ) {
        if( ( opt = getsubopt( &options, opts, &value ) ) == -1 ) {
            continue;
        }

        switch( opt ) {
            case 0:			// verbose
                tmu_slog_verbose = strtoul(value, NULL, 0);
                break;
            case 1:			// soc
                option = value;
                break;                
            default:
               break;
        }
    }

#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };

    if (option == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        option = cs_machine_str;
    }
#endif

    if (option) {
        if ( strstr( option, "H3") || strstr( option, "h3") ) {
            soc_id = PRODUCT_ID_RCAR_H3;
        } else if ( strstr( option, "M3N") || strstr( option, "m3n") ) {
            soc_id = PRODUCT_ID_RCAR_M3N;
        } else if ( strstr( option, "M3") || strstr( option, "m3") ) {
            soc_id = PRODUCT_ID_RCAR_M3W;
        } else if ( strstr( option, "D3") || strstr( option, "d3") ||
                    strstr( option, "E3") || strstr( option, "e3") ) {
            uintptr_t vmoder = (uintptr_t)mmap_device_io(4, RCAR_GEN3_RESET_BASE + RCAR_GEN3_MODEMR);
            if (vmoder != (uintptr_t)MAP_FAILED) {
                rcar_moder_reg = in32(vmoder);
                munmap_device_io(vmoder, 4);
            }
            soc_id = PRODUCT_ID_RCAR_E3;
        } else if ( strstr( option, "V3M") || strstr( option, "v3m") ) {
            soc_id = PRODUCT_ID_RCAR_V3M;
        } else if ( strstr( option, "V3H") || strstr( option, "v3h") ) {
            soc_id = PRODUCT_ID_RCAR_V3H;
        } else if ( strstr( option, "V3U") || strstr( option, "v3u") ) {
            soc_id = PRODUCT_ID_RCAR_V3U;
        } else {
            TMU_SLOG_ERROR( "%s : Unknown SoC", __FUNCTION__);
            return (-1);
        }
    }

    return EOK;
}

static void rcar_tmu_fini()
{
}

static int rcar_tmu_create_resources(tmu_chan_t *chan)
{
    rsrc_alloc_t rsrc_req;
    int rsrc_ret = EOK;

    memset(&rsrc_req, 0, sizeof(rsrc_req));

    rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -1, chan->name, RSRCDBMGR_IO_PORT);

    if (rsrc_ret < 0) {
        rsrc_ret = errno;
        TMU_SLOG_ERROR("%s: Failed querying tmu resources\n", __FUNCTION__);
        return rsrc_ret;
    } else if (rsrc_ret == 0) {
        rsrc_req.start = 0;
        rsrc_req.end = chan->channels - 1;
        rsrc_req.flags = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name = chan->name;

        rsrc_ret = rsrcdbmgr_create (&rsrc_req, 1);

        if( rsrc_ret != EOK ) {
            rsrc_ret = errno;
            TMU_SLOG_ERROR("%s: Failed creating tmu resources: err %d\n", __FUNCTION__, rsrc_ret );
            return rsrc_ret;
        }
    } else {
        return EALREADY;
    }

    return EOK;
}

static uintptr_t rcar_tmu_map_registers(tmu_chan_t *chan)
{
    uint32_t mem_map_idx;
    
    if( chan->paddr == 0) {
        return (uintptr_t)MAP_FAILED;
    }

    // check whether the memory for the CMT channel group is already mapped in.
    for (mem_map_idx = 0; mem_map_idx < RCAR_TMU_MAX_MMAP_GROUPS; mem_map_idx++) {
        if (tmu_mem_mmap[mem_map_idx].paddr == chan->paddr) {
            atomic_add(&tmu_mem_mmap[mem_map_idx].num, 1);
            chan->vbase = tmu_mem_mmap[mem_map_idx].vaddr;
            break;
        }
    }

    if (chan->vbase == 0) {
        chan->vbase = (uintptr_t)mmap_device_io(RCAR_GEN3_TMU_SIZE, chan->paddr);
        if (chan->vbase == (uintptr_t)MAP_FAILED)
            return (uintptr_t)MAP_FAILED;

        for (mem_map_idx = 0; mem_map_idx < RCAR_TMU_MAX_MMAP_GROUPS; mem_map_idx++) {
            if (tmu_mem_mmap[mem_map_idx].num == 0) {
                tmu_mem_mmap[mem_map_idx].paddr = chan->paddr;
                tmu_mem_mmap[mem_map_idx].vaddr = chan->vbase;
                tmu_mem_mmap[mem_map_idx].num   = 1;
                break;
            }
        }
    }

    return (chan->vbase);
}

static void rcar_tmu_unmap_registers(tmu_chan_t *chan)
{
    uint32_t mem_map_idx;

    // unmap memory if not in use for other CMT channels in same group
    for (mem_map_idx = 0; mem_map_idx < RCAR_TMU_MAX_MMAP_GROUPS; mem_map_idx++) {
        if (tmu_mem_mmap[mem_map_idx].paddr == chan->paddr) {
            atomic_sub(&tmu_mem_mmap[mem_map_idx].num, 1);
            if (tmu_mem_mmap[mem_map_idx].num == 0) {
                munmap_device_io(tmu_mem_mmap[mem_map_idx].vaddr, RCAR_GEN3_TMU_SIZE);
                tmu_mem_mmap[mem_map_idx].num = tmu_mem_mmap[mem_map_idx].paddr = tmu_mem_mmap[mem_map_idx].vaddr = 0;
            }
            chan->vbase = 0;
        }
    }
}

static void *
rcar_tmu_channel_attach(const struct sigevent *unevent, const struct sigevent * (* unhandler)(void *, int), 
                   const struct sigevent *inevent, const struct sigevent * (* inhandler)(void *, int), 
                   const void * data, int priority, tmu_attach_flags flags)
{
    tmu_chan_t *chan;
    rsrc_request_t  req = { 0 };

    chan = calloc(1, sizeof(*chan));
    if (chan == NULL) {
        TMU_SLOG_ERROR("%s: calloc failed\n", __FUNCTION__);
        return NULL;
    }

    chan->channels = RCAR_TMU_CHANNEL_MAX;
    strcpy(chan->name, "tmu");

    rcar_tmu_create_resources(chan);

    req.length = 1;
    req.start  = 0;
    req.end    = chan->channels - 1;
    req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;

    // The lower TMU channel numbers have highest priority
    if (flags & TMU_ATTACH_PRIORITY_STRICT) {
        req.start = req.end = priority;
    } else {
        req.flags |= RSRCDBMGR_FLAG_TOPDOWN;
    }

    if (rsrcdbmgr_attach(&req, 1) == -1) {
        TMU_SLOG_ERROR("%s: TMU channel rsrcdbmgr_attach failed: %s\n", __FUNCTION__, strerror(errno));
        goto fail1;
    }

    chan->chan_idx   = req.start;
    chan->isr_prio   = 21;
    chan->regs.tstrt = RCAR_GEN3_TMU_TSTR;
    if(soc_id == PRODUCT_ID_RCAR_V3U) {
        chan->paddr      = v3u_tmu_data[chan->chan_idx].paddr;
        chan->tstrt_mask = v3u_tmu_data[chan->chan_idx].tstrt_mask;
        chan->undr_irq   = v3u_tmu_data[chan->chan_idx].undr_irq;
        chan->incap_irq  = v3u_tmu_data[chan->chan_idx].incap_irq; 
        chan->regs.tcor  = v3u_tmu_data[chan->chan_idx].tcor;
        chan->regs.tcnt  = v3u_tmu_data[chan->chan_idx].tcnt;
        chan->regs.tcr   = v3u_tmu_data[chan->chan_idx].tcr;
        chan->regs.tcpr  = v3u_tmu_data[chan->chan_idx].tcpr;
    } else if((soc_id == PRODUCT_ID_RCAR_V3M) || (soc_id == PRODUCT_ID_RCAR_V3H)) {
        chan->paddr      = v3mh_tmu_info[chan->chan_idx].paddr;
        chan->tstrt_mask = v3mh_tmu_info[chan->chan_idx].tstrt_mask;
        chan->undr_irq   = v3mh_tmu_info[chan->chan_idx].undr_irq;
        chan->incap_irq  = v3mh_tmu_info[chan->chan_idx].incap_irq;
        chan->regs.tcor  = v3mh_tmu_info[chan->chan_idx].tcor;
        chan->regs.tcnt  = v3mh_tmu_info[chan->chan_idx].tcnt;
        chan->regs.tcr   = v3mh_tmu_info[chan->chan_idx].tcr;
        chan->regs.tcpr  = v3mh_tmu_info[chan->chan_idx].tcpr;
    } else {
        chan->paddr      = gen3_tmu_data[chan->chan_idx].paddr;
        chan->tstrt_mask = gen3_tmu_data[chan->chan_idx].tstrt_mask;
        chan->undr_irq   = gen3_tmu_data[chan->chan_idx].undr_irq;
        chan->incap_irq  = gen3_tmu_data[chan->chan_idx].incap_irq;                   
        chan->regs.tcor  = gen3_tmu_data[chan->chan_idx].tcor;
        chan->regs.tcnt  = gen3_tmu_data[chan->chan_idx].tcnt;
        chan->regs.tcr   = gen3_tmu_data[chan->chan_idx].tcr;
        chan->regs.tcpr  = gen3_tmu_data[chan->chan_idx].tcpr;
    }
    if ((flags & (TMU_ATTACH_INCAP_EVENT | TMU_ATTACH_INCAP_ISR)) && !rcar_tmu_incap_support(chan->chan_idx)) {
        TMU_SLOG_ERROR("TMU channel %d doesn't support input capture mode", chan->chan_idx);
        goto fail2;
    }

    if (rcar_tmu_map_registers(chan) == (uintptr_t)MAP_FAILED) {
        TMU_SLOG_ERROR("Error mapping TMU channel %d", chan->chan_idx);
        goto fail2;
    }

    TMU_SLOG_INFO( 1, "TMU channel %d (%08x) mapped to %08x", chan->chan_idx, chan->paddr,
            (uint32_t)chan->vbase);

    /* Initially disabled */
    rcar_tmu_stop_counter(chan);

    /* Clear and disable interrupts at this time */
    rcar_tmu_disable_interrupt(chan);

    chan->undr_iid = -1;
    chan->incap_iid = -1;


    if (flags & TMU_ATTACH_UNDR_EVENT && unevent != NULL) {
        chan->undr_iid = InterruptAttachEvent(chan->undr_irq, unevent, _NTO_INTR_FLAGS_TRK_MSK);

        if (chan->undr_iid == -1) {
            goto fail3;
        }
    }
    else if (flags & TMU_ATTACH_UNDR_ISR && unhandler != NULL) {
        chan->undr_iid = InterruptAttach(chan->undr_irq, unhandler, data, 0, _NTO_INTR_FLAGS_TRK_MSK);

        if (chan->undr_iid == -1) {
            goto fail3;
        }
    }

    if (flags & TMU_ATTACH_INCAP_EVENT && inevent != NULL) {
        chan->incap_iid = InterruptAttachEvent(chan->incap_irq, unevent, _NTO_INTR_FLAGS_TRK_MSK);
        if (chan->incap_iid == -1) {
            goto fail4;
        }
        chan->mode = TMU_MODE_INCAP;
    }
    else if (flags & TMU_ATTACH_INCAP_ISR && inhandler != NULL) {
        chan->incap_iid = InterruptAttach(chan->incap_irq, inhandler, data, 0, _NTO_INTR_FLAGS_TRK_MSK);

        if (chan->incap_iid == -1) {
            goto fail4;
        }
        chan->mode = TMU_MODE_INCAP;
    }

    /* Configure a default operating mode */
    chan->clkp       = rcar_tmu_interclock_support(chan->chan_idx);
    chan->mode       = TMU_MODE_NORMAL;
    chan->clock_edge = TMU_TCR_CKEG_RISE;
    chan->clock_div  = TMU_CLOCK_DIV_4;
    chan->constant   = 0xFFFFFFFF;    
    rcar_tmu_set_mode(chan, chan->mode);
    rcar_tmu_set_clockdiv(chan, chan->clock_div);
    if (chan->mode == TMU_MODE_INCAP)
        rcar_tmu_set_clock_edge(chan, chan->clock_edge);
    rcar_tmu_set_constant(chan, chan->constant);

    chan->flags = flags;

    return chan;
fail4:
    InterruptDetach( chan->undr_iid);
fail3:
    rcar_tmu_unmap_registers(chan);
fail2:
    req.length = 1;
    req.start  = req.end = chan->chan_idx;
    req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
    req.name   = chan->name;
    rsrcdbmgr_detach(&req, 1);

fail1:
    free(chan);

    return (NULL);
}

static void rcar_tmu_deinit(void * hdl)
{
    tmu_chan_t *chan = hdl;
    rsrc_request_t  req = { 0 };

    if( NULL != chan) {
        chan->stop = 1;
        rcar_tmu_stop_counter(chan);
        rcar_tmu_disable_interrupt(chan);
        if( -1 != chan->undr_iid) {
            InterruptDetach( chan->undr_iid);
            chan->undr_iid = -1;
        }
        if( -1 != chan->incap_iid) {
            InterruptDetach( chan->incap_iid);
            chan->incap_iid = -1;
        }

        // release TMU resource
        req.length = 1;
        req.start  = req.end = chan->chan_idx;
        req.flags  = RSRCDBMGR_IO_PORT | RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = chan->name;
        rsrcdbmgr_detach(&req, 1);
        
        // unmap registers
        rcar_tmu_unmap_registers(chan);

        free(chan);
        chan = NULL;
    }
}

static int rcar_tmu_is_running(tmu_chan_t *chan)
{
    return (in8(chan->vbase + chan->regs.tstrt) & chan->tstrt_mask);
}

static void rcar_tmu_disable_interrupt(tmu_chan_t *chan)
{
    /* Disable interrupt */
    out16(chan->vbase + chan->regs.tcr, in16(chan->vbase + chan->regs.tcr) 
                & ~(TMU_TCR_ICPF | TMU_TCR_UNF | TMU_TCR_ICP_INT | TMU_TCR_UNIE));
}

static int rcar_tmu_start_counter(void * hdl)
{
    tmu_chan_t *chan = hdl;

    if ( NULL == hdl ) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    /* Start counter */
    out8(chan->vbase + chan->regs.tstrt, in8(chan->vbase + chan->regs.tstrt) | chan->tstrt_mask);

    TMU_SLOG_INFO(2, "%s: TSTRT=0x%02x", __func__, in8(chan->vbase + chan->regs.tstrt));

    return( EOK );
}

static int rcar_tmu_stop_counter(void * hdl)
{
    tmu_chan_t *chan = hdl;
    int is_running = 0;

    if ( NULL == hdl ) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    is_running = rcar_tmu_is_running(chan);

    /* Stop counter */
    out8(chan->vbase + chan->regs.tstrt, in8(chan->vbase + chan->regs.tstrt) & ~chan->tstrt_mask);

    return(is_running);
}

static int rcar_tmu_set_clockdiv(tmu_chan_t * const chan, tmu_clock_div_t clockDiv)
{
    uint16_t tcr, ckdiv;
    int is_running;
    int ret = EOK;

    if(clockDiv == TMU_CLOCK_DIV_EXT && !rcar_tmu_extclock_support(chan->chan_idx)) {
        TMU_SLOG_ERROR("%s: TMU Channel %d doesn't support external input clock", __func__, chan->chan_idx);
        ret = EINVAL;            
    }
    else {
        switch(clockDiv) {
            case TMU_CLOCK_DIV_4:		ckdiv = TMU_TCR_TPSC_4;		break;
            case TMU_CLOCK_DIV_16:		ckdiv = TMU_TCR_TPSC_16;	break;
            case TMU_CLOCK_DIV_64:		ckdiv = TMU_TCR_TPSC_64;	break;
            case TMU_CLOCK_DIV_256:		ckdiv = TMU_TCR_TPSC_256;	break;
            case TMU_CLOCK_DIV_1024:	ckdiv = TMU_TCR_TPSC_1024;	break;
            case TMU_CLOCK_DIV_EXT:		ckdiv = TMU_TCR_TPSC_EXT;	break;
            default:					ret = EINVAL;			    break;
        }
    }

    if(EOK == ret) {
        chan->clock_div = clockDiv;

        /* Stop timer when changing registers */
        is_running = rcar_tmu_stop_counter(chan);

        tcr = in16(chan->vbase + chan->regs.tcr) & ~TMU_TCR_TPSC_MSK;
        out16(chan->vbase + chan->regs.tcr, tcr | ckdiv);

        TMU_SLOG_INFO(2, "%s: TCR=0x%04x (0x%x)", __func__, in16(chan->vbase + chan->regs.tcr), ckdiv);

        if(0 != is_running) rcar_tmu_start_counter(chan);
    }

    return(ret);
}

static int rcar_tmu_set_constant(void * hdl, const uint32_t tmucor)
{
    tmu_chan_t *chan = hdl;
    int is_running;

    if ( NULL == hdl ) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    /* Stop timer when changing registers */
    is_running = rcar_tmu_stop_counter(chan);

    chan->constant = tmucor;
    out32(chan->vbase + chan->regs.tcor, chan->constant);
    out32(chan->vbase + chan->regs.tcnt, chan->constant);

    TMU_SLOG_INFO(2, "%s: TCOR=0x%04x, TCNT=0x%04x", __func__, 
            in32(chan->vbase + chan->regs.tcor), in32(chan->vbase + chan->regs.tcnt));

    if( 0 != is_running) rcar_tmu_start_counter(chan);

    return( EOK );
}

static int rcar_tmu_get_constant(void * hdl, uint32_t * tmucor)
{
    tmu_chan_t *chan = hdl;

    if ( NULL == hdl ) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    *tmucor = in32(chan->vbase + chan->regs.tcor);

    return( EOK );
}

static int rcar_tmu_set_mode(void * hdl, const tmu_mode_t mode)
{
    tmu_chan_t *chan = hdl;
    uint16_t tcr, flg = 0;
    int ret = EOK;
    int is_running;

    if (NULL == hdl || mode == 0) {
        TMU_SLOG_ERROR("%s: Invalid mode: %d", __func__, mode);
        return EINVAL;
    }

    if ((mode == TMU_MODE_INCAP) && !rcar_tmu_incap_support(chan->chan_idx)) {
        TMU_SLOG_ERROR("%s: TMU Channel %d doesn't support mode input capture", __func__, chan->chan_idx);
        return EINVAL;
    }

    /* Stop timer when changing registers */
    is_running = rcar_tmu_stop_counter(chan);

    tcr = in16(chan->vbase + chan->regs.tcr) & ~(TMU_TCR_ICPF | TMU_TCR_UNF | TMU_TCR_UNIE | TMU_TCR_ICP_INT);

    switch( mode ) {
        case TMU_MODE_NORMAL:
            if (chan->undr_iid != -1) {
                flg = TMU_TCR_UNIE;
            }
            break;
        case TMU_MODE_INCAP:
            if (chan->incap_iid != -1) {
                flg = TMU_TCR_ICP_INT;
            } else {
                flg = TMU_TCR_ICP;
            }
            break;
        default:
            TMU_SLOG_ERROR("%s: Invalid mode: %d", __func__, mode);
            ret = EINVAL;
            break;
    }

    if (ret == EOK) {
        chan->mode = mode;
        out16(chan->vbase + chan->regs.tcr, tcr | flg);

        TMU_SLOG_INFO(2, "%s: TCR=0x%04x (0x%x)", __func__, in16(chan->vbase + chan->regs.tcr), flg);
    }

    if( 0 != is_running) rcar_tmu_start_counter(chan);
    
    return( ret);
}

static int rcar_tmu_get_mode(void * hdl, tmu_mode_t * mode)
{
    tmu_chan_t *chan = hdl;

    if (hdl == NULL || mode == NULL) {
        TMU_SLOG_ERROR("%s: Invalid argument pointer", __func__);
        return EINVAL;
    }

    *mode = chan->mode;

    return( EOK );
}

static int rcar_tmu_set_clock_edge(void * hdl, const tmu_clock_edge_t clock_edge)
{
    tmu_chan_t *chan = hdl;
    uint16_t tcr, cked = 0;
    int ret = EOK;
    int is_running;

    if (NULL == hdl) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    if (clock_edge == 0) {
        TMU_SLOG_ERROR("%s: Invalid clock edge: %d", __func__, clock_edge);
        return EINVAL;
    }
    else if (chan->clock_div != TMU_CLOCK_DIV_EXT && chan->mode != TMU_MODE_INCAP) {
        TMU_SLOG_ERROR("%s: Not support to change clock edge", __func__);
        return ENOTSUP;
    }

    /* Stop timer when changing registers */
    is_running = rcar_tmu_stop_counter(chan);

    tcr = in16(chan->vbase + chan->regs.tcr) & ~ TMU_TCR_CKEG_MSK;

    switch(clock_edge) {
        case TMU_CLOCK_EDGE_BOTH:
            cked = TMU_TCR_CKEG_BOTH;
            break;
        case TMU_CLOCK_EDGE_RISE:
            cked = TMU_TCR_CKEG_RISE;
            break;
        case TMU_CLOCK_EDGE_FALL:
            cked = TMU_TCR_CKEG_FALL;
            break;
        default:
            TMU_SLOG_ERROR("%s: Invalid clock edge: %d", __func__, clock_edge);
            ret = EINVAL;
            break;
    }
    if (ret == EOK) {
        chan->clock_edge = clock_edge;
        out16(chan->vbase + chan->regs.tcr, tcr | cked);
        TMU_SLOG_INFO( 2, "%s: TCR=0x%04x (0x%x)", __func__, in16(chan->vbase + chan->regs.tcr), cked);
    }

    if( 0 != is_running) rcar_tmu_start_counter(chan);

    return( ret );
}

static int rcar_tmu_get_clock_edge(void * hdl, tmu_clock_edge_t * clock_edge)
{
    tmu_chan_t *chan = hdl;

    if (NULL == hdl || NULL ==clock_edge) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    *clock_edge = 0;

    if (chan->clock_div == TMU_CLOCK_DIV_EXT || chan->mode == TMU_MODE_INCAP) {
        *clock_edge = chan->clock_edge;
    }

    return( EOK );
}

static int rcar_tmu_set_count(void * hdl, const uint32_t count)
{
    tmu_chan_t *chan = hdl;
    uint32_t tcor = 0;
    int ret = EOK;
    int is_running;

    if ( NULL == hdl ) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    if ((ret = rcar_tmu_get_constant(chan, &tcor)) == EOK) {
        if( count <= tcor) {
            /* Stop timer when changing registers */
            is_running = rcar_tmu_stop_counter( chan);
            //out32(chan->vbase + chan->regs.tcor, count);
            out32(chan->vbase + chan->regs.tcnt, count);

            TMU_SLOG_INFO( 2, "%s: TCNT=%08x", __func__, in32(chan->vbase + chan->regs.tcnt));

            if( 0 != is_running) rcar_tmu_start_counter( chan);
        }
        else {
            TMU_SLOG_ERROR("%s: ERROR: count (%x) must be <= constant(%x)", __func__, count, tcor);
            ret = EINVAL;
        }
    }

    return( ret);
}

static int rcar_tmu_get_count(void *hdl, uint32_t * count)
{
    tmu_chan_t *chan = hdl;

    if (NULL == hdl || NULL == count) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    *count = in32(chan->vbase + chan->regs.tcnt);

    return( EOK);
}

static int rcar_tmu_get_incapture(void *hdl, uint32_t * count)
{
    tmu_chan_t *chan = hdl;

    if (NULL == hdl || NULL ==count) {
        TMU_SLOG_ERROR("%s: Invalid pointer", __func__);
        return EINVAL;
    }

    *count = in32(chan->vbase + chan->regs.tcpr);

    return( EOK );
}

static int rcar_tmu_set_clock(void * hdl, tmu_clock_t * const clk)
{
    tmu_chan_t *chan = hdl;
    int ret = EOK;

    if(NULL == hdl || NULL == clk) {
        TMU_SLOG_ERROR("%s: Invalid argument pointer", __func__);
        ret = EINVAL;
    }
    else {
        ret = rcar_tmu_set_clockdiv(hdl, clk->div);

        if (!ret && (chan->mode == TMU_MODE_INCAP))
            ret = rcar_tmu_set_clock_edge(hdl, clk->clk_edge);

        if (chan->clock_div == TMU_CLOCK_DIV_EXT) 
            chan->clkp = clk->parent;
    }

    return( ret );
}

static int rcar_tmu_get_clock(void * hdl, tmu_clock_t * const clk)
{
    tmu_chan_t *chan = hdl;
    int ret = EOK;

    if(NULL == hdl || NULL ==clk) {
        TMU_SLOG_ERROR("%s: Invalid argument pointer", __func__);
        ret = EINVAL;
    }
    else {
        clk->parent = chan->clkp;
        clk->div = chan->clock_div;
        clk->req = chan->clkp / chan->clock_div;
        ret = rcar_tmu_get_clock_edge(hdl, &clk->clk_edge);
    }

    return( ret );
}

static int rcar_tmu_get_underflow_count(void *hdl, uint32_t * const underflow_count)
{
    tmu_chan_t *chan = hdl;

    if(NULL == hdl || NULL ==underflow_count)  {
        TMU_SLOG_ERROR("%s: Invalid argument pointer", __func__);
        return EINVAL;
    }
    
    *underflow_count = chan->underflow_cnt;

    return( EOK );
}

static int rcar_tmu_get_tick(void *hdl, uint64_t * const tick)
{
    tmu_chan_t *chan = hdl;

    if(NULL == hdl || NULL == tick) {
        TMU_SLOG_ERROR("%s: Invalid argument pointer", __func__);
        return EINVAL;
    }

    uint64_t tcor = (uint64_t)in32(chan->vbase + chan->regs.tcor);
    uint64_t tcnt = (uint64_t)in32(chan->vbase + chan->regs.tcnt);
    *tick = tcor - tcnt + (tcor * chan->underflow_cnt);

    return( EOK );
}

static int rcar_tmu_change_tick_to_sec(void *hdl, uint64_t tick, double * const sec)
{
    tmu_chan_t *chan = hdl;

    if(NULL == hdl || NULL == sec) {
        TMU_SLOG_ERROR("%s: Invalid argument pointer", __func__);
        return EINVAL;
    }

    *sec = (double)tick / (chan->clkp / chan->clock_div);

    return( EOK );
}

static int rcar_tmu_start(void * hdl, uint32_t * count)
{
    tmu_chan_t *chan = hdl;
    uint32_t tcor = 0;

    if( NULL == hdl ) {
        TMU_SLOG_ERROR("%s: Invalid argument pointer", __func__);
        return EINVAL;
    }

    rcar_tmu_stop_counter( chan);

    /* Reset underflow count */
    chan->underflow_cnt = 0;

    /* Start count from value of constant resgiter */
    rcar_tmu_get_constant(chan, &tcor);
    rcar_tmu_set_count(chan, tcor);

    if (count)
        rcar_tmu_get_count(chan, count);

    /* Start counter */
    rcar_tmu_start_counter(chan);

    TMU_SLOG_INFO(2, "%s: TCR=0x%04x", __func__, in16(chan->vbase + chan->regs.tcr));

    return( EOK );
}

static int rcar_tmu_stop(void * hdl, uint32_t * count)
{
    tmu_chan_t *chan = hdl;

    if( NULL == hdl ) {
        TMU_SLOG_ERROR("%s: Invalid argument pointer", __func__);
        return EINVAL;
    }

    /* Stop counter */
    rcar_tmu_stop_counter(chan);

    TMU_SLOG_INFO(2, "%s: TCR=%04x", __func__, in16(chan->vbase + chan->regs.tcr));

    if (count)
        rcar_tmu_get_count(chan, count);
    
    /* Disable interrupt */
    rcar_tmu_disable_interrupt(chan);

    return( EOK );
}

static void rcar_tmu_underflow_isr(void * hdl)
{
    if( hdl ) {
        tmu_chan_t *chan = hdl;
        uint32_t tcr = in16(chan->vbase + chan->regs.tcr);

        if(tcr & TMU_TCR_UNF) {
            chan->underflow_cnt++;
            /* Clear underflow interrupt */
            tcr &= ~TMU_TCR_UNF;
            out16(chan->vbase + chan->regs.tcr, tcr);
        }

        if ((chan->flags & TMU_ATTACH_UNDR_EVENT) && (chan->undr_iid != -1)) {
            InterruptUnmask(chan->undr_irq, chan->undr_iid);
        }
    }
}

static void rcar_tmu_incapture_isr(void * hdl)
{
    if( hdl ) {
        tmu_chan_t *chan = hdl;
        uint32_t tcr = in16(chan->vbase + chan->regs.tcr);

        if(tcr & TMU_TCR_ICPF) {
            /* Clear input capture interrupt */
            tcr &= ~TMU_TCR_ICPF;
            out16(chan->vbase + chan->regs.tcr, tcr);
        }

        if ((chan->flags & TMU_ATTACH_INCAP_EVENT) && (chan->incap_iid != -1)) {
            InterruptUnmask(chan->incap_irq, chan->incap_iid);
        }
    }
}

struct tmu_factory tmu_support = {
    .init                = rcar_tmu_init,
    .fini                = rcar_tmu_fini,
    .channel_attach      = rcar_tmu_channel_attach,
    .channel_release     = rcar_tmu_deinit,
    .start_counter       = rcar_tmu_start_counter,
    .stop_counter        = rcar_tmu_stop_counter,
    .start               = rcar_tmu_start,
    .stop                = rcar_tmu_stop,
    .set_mode            = rcar_tmu_set_mode,
    .get_mode            = rcar_tmu_get_mode,
    .set_constant        = rcar_tmu_set_constant,
    .get_constant        = rcar_tmu_get_constant,
    .set_count           = rcar_tmu_set_count,
    .get_count           = rcar_tmu_get_count,
    .get_incapture       = rcar_tmu_get_incapture,
    .set_clock           = rcar_tmu_set_clock,
    .get_clock           = rcar_tmu_get_clock,
    .get_underflow_count = rcar_tmu_get_underflow_count,
    .get_tick            = rcar_tmu_get_tick,
    .change_tick_to_sec  = rcar_tmu_change_tick_to_sec,
    .underflow_isr       = rcar_tmu_underflow_isr,
    .incapture_isr       = rcar_tmu_incapture_isr,
};

