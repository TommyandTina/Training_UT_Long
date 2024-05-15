/*
 * $QNXLicenseC:
 * Copyright 2014, 2016 QNX Software Systems.
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

#include <audio_driver.h>
#include "mem_map.h"
#include "rcar_support.h"
#include "ssiu_reg.h"
#include "ssiu.h"

/* the SSIU and SSI register maps are stored as static global variables and
 * shared across multiple rcar deva instances if more than one mounted; the only thing
 * that is important is to not map the memory multiple times, and to unmap it
 * only when the last rcar deva instance is unmounted; this is checked in ssiu_init
 * from where the SSIU/SSI register mapping functions are called
 */
static unsigned long ssiu_base = 0;
static uint32_t ssiu_len = 0;
static ssiu_reg_map_t* ssiu_reg_map = MAP_FAILED;
static unsigned long ssi_base = 0;
static uint32_t ssi_len = 0;
static ssi_reg_t* ssi_reg = MAP_FAILED;

int ssiu_mem_map(void)
{
    if (!ssiu_base || !ssiu_len) {
#if _NTO_VERSION >= 700
        /* the hwinfo query works for all gen3 variants, if it fails, then
         * this is a gen2 variant, use the RCAR_GEN2_SSIU_BASE as base and
         * as len the offset of ssiu_reg_ssi_0_9_busif2 within ssiu_reg_map_t
         */
        if (rcar_query_hwinfo(RCAR_HWI_AUDIO_SSIU, &ssiu_base, &ssiu_len) != EOK) {
            ssiu_base = RCAR_GEN2_SSIU_BASE;
            ssiu_len = offsetof(ssiu_reg_map_t, ssiu_reg_ssi_0_9_busif2);
        }
#else
        ssiu_base = RCAR_SSIU_BASE;
        ssiu_len = offsetof(ssiu_reg_map_t, ssiu_reg_ssi_0_9_busif2);
#endif
    }

    if (!ssi_base || !ssi_len) {
#if _NTO_VERSION >= 700
        /* the hwinfo query works for all gen3 variants, if it fails, then
         * this is a gen2 variant, use the RCAR_GEN2_SSI_BASE as base and
         * as len the size of ssi_reg_t times the max number of SSIs
         */
        if (rcar_query_hwinfo(RCAR_HWI_AUDIO_SSI, &ssi_base, &ssi_len) != EOK) {
            ssi_base = RCAR_GEN2_SSI_BASE;
            ssi_len = SSI_CHANNEL_NUM * sizeof(ssi_reg_t);
        }
#else
        ssi_base = RCAR_SSI_BASE;
        ssi_len = SSI_CHANNEL_NUM * sizeof(ssi_reg_t);
#endif
    }

    /* SSIU registers
     * We can pass NULL for the card pointer if we do not need to register the mapping with SMMU
     * The DMAC accesses the SSI with different register addresses which are registered with SMMU
     * in dmac.c
     */
    ssiu_reg_map = ADO_DEVICE_MMAP (NULL, ssiu_base, ssiu_len);
    if (ssiu_reg_map == MAP_FAILED )
    {
        ado_error_fmt ("SSIU register map failed");
        return ENOMEM;
    }

    /* SSI registers
     * We can pass NULL for the card pointer if we do not need to register the mapping with SMMU
     * The DMAC accesses the SSI with different register addresses which are registered with SMMU
     * in dmac.c
     */
    ssi_reg = ADO_DEVICE_MMAP (NULL, ssi_base, ssi_len);
    if (ssi_reg == MAP_FAILED )
    {
        ADO_DEVICE_MUNMAP (NULL, ssiu_reg_map, ssiu_len);
        ssiu_reg_map = MAP_FAILED;
        ado_error_fmt ("SSI register map failed");
        return ENOMEM;
    }

    return EOK;
}

int ssiu_mem_unmap(void)
{

    if (ssiu_reg_map != MAP_FAILED) {
        ADO_DEVICE_MUNMAP(NULL, (void*)ssiu_reg_map, ssiu_len);
        ssiu_reg_map = MAP_FAILED;
    }

    if (ssi_reg != MAP_FAILED) {
        ADO_DEVICE_MUNMAP(NULL, (void*)ssi_reg, ssi_len);
        ssi_reg = MAP_FAILED;
    }

    return EOK;
}

ssi_reg_t* get_ssi_reg(uint32_t ssi_idx)
{
    if( ssi_reg == MAP_FAILED ) {
        return NULL;
    }

    if( ssi_idx >= SSI_CHANNEL_NUM ) {
        return NULL;
    }

    return ssi_reg + ssi_idx;
}

ssi_ext_reg_t* get_ssi_ext_reg(uint32_t ssi_idx)
{
    if( ssi_reg == MAP_FAILED ) {
        return NULL;
    }

    if( ssi_idx >= SSI_CHANNEL_NUM ) {
        return NULL;
    }

    if( !rcar_ssi_ext_reg_supported() ) {
        return NULL;
    }

    return (ssi_ext_reg_t*)ssi_reg + ssi_idx;
}


ssiu_busif_reg_t* get_ssiu_busif_reg(uint32_t ssi_idx, uint32_t ssi_subchan_idx)
{
    ssiu_reg_t* ssiu_reg = NULL;

    if( ssiu_reg_map == MAP_FAILED ) {
        return NULL;
    }

    if( !rcar_ssi_subchan_supported(ssi_idx, ssi_subchan_idx) ) {
        return NULL;
    }

    if( ssi_subchan_idx <= 3 ) {
        ssiu_reg = &ssiu_reg_map->ssiu_reg_ssi_0_9_busif_0_3[ssi_idx][ssi_subchan_idx];
    } else if (ssi_idx <= 5) {
        ssiu_reg = &ssiu_reg_map->ssiu_reg_ssi_0_5_busif_4_7[ssi_idx][ssi_subchan_idx-4];
    } else {
        ssiu_reg = &ssiu_reg_map->ssiu_reg_ssi_9_busif_4_7[ssi_subchan_idx-4];
    }

    return &ssiu_reg->busif;
}

ssiu_busif2_reg_t* get_ssiu_busif2_reg(uint32_t ssi_idx)
{
    if( ssiu_reg_map == MAP_FAILED ) {
        return NULL;
    }

    if( !rcar_ssi_supported(ssi_idx) ) {
        return NULL;
    }

    if( !rcar_ssi_busif2_reg_supported(ssi_idx) ) {
        return NULL;
    }

    return &ssiu_reg_map->ssiu_reg_ssi_0_9_busif2[ssi_idx].busif2;
}

ssiu_ssi_reg_t* get_ssiu_ssi_reg(uint32_t ssi_idx)
{
    if( ssiu_reg_map == MAP_FAILED ) {
        return NULL;
    }

    if( !rcar_ssi_supported(ssi_idx) ) {
        return NULL;
    }

    return &ssiu_reg_map->ssiu_reg_ssi_0_9_busif_0_3[ssi_idx][0].ssi;
}

ssiu_ssi2_reg_t* get_ssiu_ssi2_reg(uint32_t ssi_idx)
{
    if( ssiu_reg_map == MAP_FAILED ) {
        return NULL;
    }

    if( !rcar_ssi_supported(ssi_idx) ) {
        return NULL;
    }

    return &ssiu_reg_map->ssiu_reg_ssi_0_9_busif2[ssi_idx].ssi2;
}

ssiu_common_reg_t* get_ssiu_common_reg(void)
{
    if( ssiu_reg_map == MAP_FAILED ) {
        return NULL;
    }

    return &ssiu_reg_map->common;
}

ssiu_common_ext_reg_t* get_ssiu_common_ext_reg(void)
{
    if( ssiu_reg_map == MAP_FAILED ) {
        return NULL;
    }

    return (ssiu_common_ext_reg_t*) &ssiu_reg_map->common;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/ssiu_reg.c $ $Rev: 911285 $")
#endif

