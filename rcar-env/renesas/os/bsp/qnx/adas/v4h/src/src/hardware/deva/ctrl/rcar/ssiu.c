/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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
#include <drvr/hwinfo.h>
#include <aarch64/r-car.h>
#include <hw/hwinfo_rcar.h>
#include "ssiu.h"

/* the SSIU and SSI register maps are shared across multiple rcar deva
 * instances if more than one mounted; the only thing that is important is
 * to not map the memory multiple times, and to unmap it only when the
 * last rcar deva instance is unmounted; this is checked using the
 * global static variable ssiu_ref_count that counts how many driver
 * instances are mounted; additionally most SSIU common  registers can be
 * accessed from multiple driver instances, as these registers have global
 * mode or routing settings; use a mutex (also a static global variable)
 * to control concurrent access to these registers and to the static
 * ssiu_ref_count
 */
static uint32_t ssiu_ref_count = 0;
static ado_mutex_t ssiu_lock = PTHREAD_MUTEX_INITIALIZER;

/* the SSIU and SSI register maps are stored as static global variables and
 * shared across multiple rcar deva instances if more than one mounted; the only thing
 * that is important is to not map the memory multiple times, and to unmap it
 * only when the last rcar deva instance is unmounted; this is checked in ssiu_init
 * from where the SSIU/SSI register mapping functions are called
 */
static uint32_t ssiu_len = 0;
static uint32_t ssi_len = 0;

extern char* ssi_reg;
extern char* ssiu_reg_map;

int ssiu_mem_map(void)
{
    unsigned long ssiu_base = 0;
    unsigned long ssi_base = 0;
    unsigned hwi_off;
    
    hwi_off = hwi_find_device(RCAR_HWI_AUDIO_SSIU, 0);
    if(hwi_off != HWI_NULL_OFF) {
        hwi_tag *tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            ssiu_base = tag->location.base;
            ssiu_len = tag->location.len;
        }
    }

    hwi_off = hwi_find_device(RCAR_HWI_AUDIO_SSI, 0);
    if(hwi_off != HWI_NULL_OFF) {
        hwi_tag *tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            ssi_base = tag->location.base;
            ssi_len = tag->location.len;
        }
    }

    if (!ssi_base) {
        ssi_base = RCAR_SSI_BASE;
        ssi_len = RCAR_SSI_SIZE;
    }

    if (!ssiu_base) {
        ssiu_base = RCAR_SSIU_BASE;
        ssiu_len = RCAR_SSIU_SIZE;
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

/* Helper functions to set SSI registers bit fields */
static int ssi_set_role_bits(uint32_t* cr, uint32_t* cre, uint32_t* wsr, uint32_t is_master, uint32_t is_transmit)
{
    if(cr == 0 || wsr == 0) {
        return EINVAL;
    }

    (*cr) &= ~(SSICR_TRMD_MASK | SSICR_SWSD_MASK | SSICR_SCKD_MASK);
    (*wsr) &= ~SSIWSR_CONT_MASK;

    if(is_master) {
        (*cr) |= SSICR_SWSD_OUTPUT | SSICR_SCKD_OUTPUT;
        (*wsr) |= SSIWSR_CONT_ENABLED;
    } else {
        (*cr) |= SSICR_SWSD_INPUT | SSICR_SCKD_INPUT;
        (*wsr) |= SSIWSR_CONT_DISABLED; /* For slave the CONT mode must be disabled */
    }

    if(is_transmit) {
        (*cr) |= SSICR_TRMD_TX_MODE;
    } else {
        (*cr) |= SSICR_TRMD_RX_MODE;
    }

    return EOK;
}

static int ssi_set_mode_bits(uint32_t* cr, uint32_t* cre, uint32_t* wsr, ssi_op_mode_t mode, uint32_t voicenum)
{
    if(cr == 0 || cre == 0 || wsr == 0) {
        return EINVAL;
    }

    // For most modes, populate the CHNL and SDTA bit fields of SSICR,
    // the MONO and WSMODE bit fields of SSIWSR and if applicable,
    // the CHNL2 bit field of SSICRE
    // for the TDM split modes also overwrite the SWL and DWL bit fields
    // that were populated by ssi_set_config_bits
    (*cr) &= ~(SSICR_CHNL_MASK | SSICR_SDTA_MASK);
    (*wsr) &= ~(SSIWSR_MONO_MASK | SSIWSR_WSMODE_MASK);
    (*cre) &= ~SSICRE_CHNL2_MASK;

    if(mode == SSI_OP_MODE_MONO) {
        (*cr) &= ~SSICR_SDTA_MASK; // for monaural SDTA must be forced to 0
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_MONO;
        (*cr) |= SSICR_CHNL_1 | SSICR_SDTA_SDATA_FIRST;
    } else if(mode == SSI_OP_MODE_STEREO) {
        (*cr) |=  SSICR_CHNL_1 | SSICR_SDTA_SDATA_FIRST;
        (*wsr) |= SSIWSR_WSMODE_STEREO_MULTICH | SSIWSR_MONO_TDM ;
    } else if((mode == SSI_OP_MODE_TDM) || (mode == SSI_OP_MODE_TDMEXT)) {
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        if(voicenum == 4) {
            (*cr) |= SSICR_CHNL_2;
        } else if(voicenum == 6) {
            (*cr) |= SSICR_CHNL_3;
        } else if(voicenum == 8) {
            (*cr) |= SSICR_CHNL_4;
        } else {
            return EINVAL;
        }
    // For the TDM split modes SWL and DWL have special settings,
    // overwrite the default settings
    } else if(mode == SSI_OP_MODE_TDMSPLIT_4XMONO) {
        (*cr) &= ~(SSICR_SWL_MASK | SSICR_DWL_MASK);
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cr) |= SSICR_DWL_32BIT | SSICR_SWL_32BIT | SSICR_CHNL_2;
    } else if(mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO) {
        (*cr) &= ~(SSICR_SWL_MASK | SSICR_DWL_MASK);
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cr) |= SSICR_DWL_32BIT | SSICR_SWL_32BIT | SSICR_CHNL_4;
    } else if(mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN) {
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cre) |= SSICRE_CHNL2_MASK;
    } else if(mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN) {
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cr) |= SSICR_CHNL_4;
    } else if(mode == SSI_OP_MODE_TDM_16CHAN) {
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cre) |= SSICRE_CHNL2_MASK;
    }

    return EOK;
}

static int ssi_set_config_bits(uint32_t* cr, ssi_config_t * config)
{
    if(cr == 0) {
        return EINVAL;
    }

    (*cr) &= ~(SSICR_DWL_MASK | SSICR_SWL_MASK | SSICR_SCKP_MASK | SSICR_SWSP_MASK |
               SSICR_SPDP_MASK | SSICR_SDTA_MASK | SSICR_DEL_MASK);
    (*cr) |= (config->bit_delay << 8);
    (*cr) |= (config->serial_data_alignment<<10);
    (*cr) |= (config->padding_pol<<11);
    (*cr) |= (config->ws_pol << 12);
    (*cr) |= (config->clk_pol << 13);
    (*cr) |= (config->sys_word_length << 16);
    (*cr) |= (config->data_word_length << 19);

    return EOK;
}

static int ssi_set_clockdiv_bits(uint32_t* cr, uint32_t divisor)
{
    if(cr == 0) {
        return EINVAL;
    }

    *cr &= ~SSICR_CKDV_MASK;

    switch (divisor) {
        case 1:
            *cr |= SSICR_CKDV_1;
            break;
        case 2:
            *cr |= SSICR_CKDV_2;
            break;
        case 4:
            *cr |= SSICR_CKDV_4;
            break;
        case 6:
            *cr |= SSICR_CKDV_6;
            break;
        case 8:
            *cr |= SSICR_CKDV_8;
            break;
        case 12:
            *cr |= SSICR_CKDV_12;
            break;
        case 16:
            *cr |= SSICR_CKDV_16;
            break;
        default:
            return EINVAL;
    }
    return EOK;
}

static int ssi_set_control_bits(uint32_t* cr, uint32_t enable, uint32_t mute)
{
    if(cr == 0) {
        return EINVAL;
    }

    (*cr) &= ~(SSICR_EN_MASK | SSICR_MUEN_MASK);
    if(enable) {
        (*cr) |= SSICR_EN_ENABLE;
    } else {
        (*cr) |= SSICR_EN_DISABLE;
    }
    if(mute) {
        (*cr) |= SSICR_MUEN_MUTED;
    } else {
        (*cr) |= SSICR_MUEN_UNMUTED;
    }

    return EOK;
}

#if 0
static int ssi_set_interrupt_bits(uint32_t* cr, ssi_interrupt_t* interrupts)
{
    if(cr == 0 || interrupts == 0) {
        return EINVAL;
    }
    (*cr) &= ~(SSICR_DMEN_MASK | SSICR_UIEN_MASK | SSICR_OIEN_MASK | SSICR_IIEN_MASK | SSICR_DIEN_MASK);
    if(interrupts->dma) {
        (*cr) |= SSICR_DMEN_ENABLE;
    } else {
        (*cr) |= SSICR_DMEN_DISABLE;
    }
    if(interrupts->underflow) {
        (*cr) |= SSICR_UIEN_ENABLE;
    } else {
        (*cr) |= SSICR_UIEN_DISABLE;
    }
    if(interrupts->overflow) {
        (*cr) |= SSICR_OIEN_ENABLE;
    } else {
        (*cr) |= SSICR_OIEN_DISABLE;
    }
    if(interrupts->idle_mode) {
        (*cr) |= SSICR_IIEN_ENABLE;
    } else {
        (*cr) |= SSICR_IIEN_DISABLE;
    }
    if(interrupts->data) {
        (*cr) |= SSICR_DIEN_ENABLE;
    } else {
        (*cr) |= SSICR_DIEN_DISABLE;
    }
    return EOK;
}
#endif

/* SSI register configuration functions */
static int ssi_set_control_and_mode
(
    uint32_t is_master,
    uint32_t is_transmit,
    ssi_op_mode_t ssi_op_mode,
    uint32_t voicenum,
    ssi_config_t* config
)
{
    uint32_t cr, cre = 0, wsr;

    if( !SSI || !SSI_EXT ) {
        return EFAULT;
    }

    cr = SSI->cr;
    cre = SSI_EXT->cre;
    wsr = SSI->wsr;

    if(ssi_set_role_bits(&cr, &cre, &wsr, is_master, is_transmit) != EOK) {
        return EINVAL;
    }

    if(ssi_set_config_bits(&cr, config) != EOK) {
        return EINVAL;
    }

    // Important: call ssi_set_mode_bits after ssi_set_config_bits, because it
    // overrides the SWL and DWL bit fields with special values for some ssi_op_mode values
    if(ssi_set_mode_bits(&cr, &cre, &wsr, ssi_op_mode, voicenum) != EOK) {
        return EINVAL;
    }

    if(ssi_set_control_bits(&cr, 0, 0) != EOK) {
        return EINVAL;
    }

    cr |= SSICR_FORCE_FIXED;

    SSI->cr = cr;
    if( SSI_EXT ) {
        SSI_EXT->cre = cre;
    }

    SSI->wsr = wsr;

    return EOK;
}

static int ssi_clear_control_and_mode(void)
{
    SSI->cr &= ~0x7FFF;
    SSI->wsr = 0;

    return EOK;
}

/* Public SSI level functions - start/stop */
int ssi_start(void)
{
    if( !SSI ) {
        return EFAULT;
    }

    SSI->cr |= SSICR_EN_ENABLE | SSICR_DMEN_ENABLE |
                                SSICR_UIEN_ENABLE | SSICR_OIEN_ENABLE | SSICR_FORCE_FIXED;

    return EOK;
}

int ssi_wait_status(uint32_t bitmask)
{
    uint32_t i;
    uint32_t timeout = 100; /* 100 * 100 microseconds = 10 miliseconds */

    if( !SSI ) {
        return EFAULT;
    }

    for (i = 0; i < timeout; i++)
    {
        nanospin_ns(100000); /* 100 microseconds */

        if (SSI->sr & bitmask)
        {
            return EOK;
        }
    }

    return ETIMEDOUT;
}

int ssi_stop(void)
{
    int dirq_status;

    if( !SSI ) {
        return EFAULT;
    }

    SSI->cr &= ~(SSICR_DMEN_MASK | SSICR_UIEN_MASK | SSICR_OIEN_MASK);

    dirq_status = ssi_wait_status(SSISR_DIRQ_MASK);

    if (dirq_status != EOK) {
        ado_error_fmt("SSI timeout waiting for DIRQ");
    }

    SSI->cr &= ~SSICR_EN_MASK;
    SSI->cr |= SSICR_IIEN_ENABLE;

    return EOK;
}

int ssi_stop_idle_int(void)
{
    if( !SSI ) {
        return EFAULT;
    }

    SSI->cr &= ~(SSICR_EN_MASK|SSICR_IIEN_MASK);

    return EOK;
}

static int ssiu_set_ssi_mode(ssi_op_mode_t op_mode)
{
    if( !SSIU_SSI || !SSIU_SSI2 ) {
        return EFAULT;
    }

    SSIU_SSI->mode = 0;
    SSIU_SSI2->mode2 = 0;

    if( op_mode == SSI_OP_MODE_TDMEXT ) {
        SSIU_SSI->mode |= SSI_MODE_TDMEXT_ENABLE;
    } else if( op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ) {
        SSIU_SSI->mode |= SSI_MODE_TDMSPLIT_ENABLE | SSI_MODE_FSMODE_ENABLE;
    } else if( op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ) {
        SSIU_SSI->mode |= SSI_MODE_TDMSPLIT_ENABLE;
    } else if( op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
               op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
        SSIU_SSI->mode |= SSI_MODE_TDMSPLIT_ENABLE;
        SSIU_SSI2->mode2 |= SSI_MODE2_EX_FUNC_ENABLE;
    } else if( op_mode == SSI_OP_MODE_TDM_16CHAN ) {
        SSIU_SSI->mode |= SSI_MODE_TDMEXT_ENABLE;
        SSIU_SSI2->mode2 |= SSI_MODE2_EX_FUNC_ENABLE;
    }
    return EOK;
}

static int ssiu_clear_ssi_mode(void)
{
    if( !SSIU_SSI || !SSIU_SSI2 ) {
        return EFAULT;
    }

    SSIU_SSI->mode = 0;

    SSIU_SSI2->mode2 = 0;

    return EOK;
}

#define TDM_EXSPLIT_MAP_SUBCHN(map, subchn) ((map >> (subchn << 2)) & 0xF)
static int ssiu_validate_tdm_exsplit_map(uint32_t num_subchan, uint32_t tdm_exsplit_map)
{
    static const uint32_t tdm_exsplit_max_voices_map = 0x2428242A;
    int i, subchan_to_skip = 0;

    for(i = 0; i < num_subchan; i++) {
        uint32_t tdm_exsplit_map_subchn_entry = TDM_EXSPLIT_MAP_SUBCHN(tdm_exsplit_map, i);
        // If this subchan must be skipped, check that the entry for it is zero
        if(subchan_to_skip) {
            if(tdm_exsplit_map_subchn_entry != 0) {
                return EINVAL;
            }
            subchan_to_skip--;
            continue;
        } else {
            // this subchan should be populated, check that the entry for it is non zero
            if(tdm_exsplit_map_subchn_entry == 0) {
                return EINVAL;
            }
        }
        // Check that the subchan entry is even
        if(((tdm_exsplit_map_subchn_entry >> 1) << 1) != tdm_exsplit_map_subchn_entry) {
            return EINVAL;
        }
        // Check that the subchan entry does not exceed the max for this subchan
        if(tdm_exsplit_map_subchn_entry > TDM_EXSPLIT_MAP_SUBCHN(tdm_exsplit_max_voices_map, i)) {
            return EINVAL;
        }

        // Now we know that the entry is valid

        // If the entry for this subchan is greater than two, then the next subchan(s)
        // must be skipped, i.e. their entries must be 0; determine how many subchans to skip
        subchan_to_skip = (tdm_exsplit_map_subchn_entry >> 1) - 1;
    }
    return EOK;
}

static int ssiu_set_ssi_busif(ssi_op_mode_t op_mode, uint32_t voicenum, uint32_t bitnum, uint32_t tdm_exsplit_map)
{
    uint32_t otbl, chnum;
    uint32_t num_subchan = 1;
    uint32_t i;
    int status = EOK;

    switch(bitnum) {
        case 8: otbl = SSI_BUSIF_ADINR_OTBL_8BITS; break;
        case 16: otbl = SSI_BUSIF_ADINR_OTBL_16BITS; break;
        case 18: otbl = SSI_BUSIF_ADINR_OTBL_18BITS; break;
        case 20: otbl = SSI_BUSIF_ADINR_OTBL_20BITS; break;
        case 22: otbl = SSI_BUSIF_ADINR_OTBL_22BITS; break;
        case 24: otbl = SSI_BUSIF_ADINR_OTBL_24BITS; break;
        default: return EINVAL;
    }
    switch(voicenum) {
        case 0: chnum = SSI_BUSIF_ADINR_CHNUM_NONE; break;
        case 1: chnum = SSI_BUSIF_ADINR_CHNUM_ONE; break;
        case 2: chnum = SSI_BUSIF_ADINR_CHNUM_TWO; break;
        case 4: chnum = SSI_BUSIF_ADINR_CHNUM_FOUR; break;
        case 6: chnum = SSI_BUSIF_ADINR_CHNUM_SIX; break;
        case 8: chnum = SSI_BUSIF_ADINR_CHNUM_EIGHT; break;
        case 16: chnum = SSI_BUSIF_ADINR_CHNUM_SIXTEEN; break;
        default: return EINVAL;
    }

    // Validate bitnum for all modes except for the simple modes
    if((op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN) ||
        (op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN)) {
        if(bitnum != 16) {
            return EINVAL;
        }
        if(tdm_exsplit_map == 0) {
            return EINVAL;
        }
    } else if(op_mode != SSI_OP_MODE_MONO && op_mode != SSI_OP_MODE_STEREO) {
        if(bitnum != 16 && bitnum != 24) {
            return EINVAL;
        }
        tdm_exsplit_map = 0;
    }
    // Force chnum to 2 for TDM split modes
    if((op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO) || (op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO)) {
        chnum = SSI_BUSIF_ADINR_CHNUM_TWO; //is this correct for 4XMONO?
        num_subchan = 4;
    } else if(op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN) {
        // chnum can be different for each subchannel
        num_subchan = 4;
    } else if(op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN) {
        // chnum can be different for each subchannel
        num_subchan = 8;
    }

    if(tdm_exsplit_map && ssiu_validate_tdm_exsplit_map(num_subchan, tdm_exsplit_map) != EOK) {
        return EINVAL;
    }

    for(i = 0; i < num_subchan; i++) {
        if( !SSIU_BUSIF(i) ) {
            status = EFAULT;
            break;
        }
        if((op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN) ||
            (op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN)) {
            chnum = TDM_EXSPLIT_MAP_SUBCHN(tdm_exsplit_map, i);
        }
        SSIU_BUSIF(i)->busif_adinr &= ~(SSI_BUSIF_ADINR_OTBL_MASK |
                                                      SSI_BUSIF_ADINR_CHNUM_MASK);
        SSIU_BUSIF(i)->busif_adinr |= (otbl | chnum);
    }
    return status;
}

static int ssiu_clear_ssi_busif(void)
{
    // Placeholder - not sure at this point if anything needs to be done here
    uint32_t i;
    int status;

    for(i = 0; i <= 7; i++) {
        if( !SSIU_BUSIF(i) ) {
            status = EFAULT;
            break;
        }

        SSIU_BUSIF(i)->busif_adinr &= ~(SSI_BUSIF_ADINR_OTBL_MASK |
                                                      SSI_BUSIF_ADINR_CHNUM_MASK);
    }

    return status;
}

/* Public SSIU level functions */
int ssiu_init(void)
{
    ado_mutex_lock (&ssiu_lock);

    if (ssiu_ref_count == 0) {
        ssiu_mem_map();
    }

    ssiu_ref_count++;

    ado_mutex_unlock (&ssiu_lock);

    return EOK;
}

int ssiu_deinit(void)
{
    ado_mutex_lock (&ssiu_lock);

    if (ssiu_ref_count > 0) {
        ssiu_ref_count--;
    }

    if (ssiu_ref_count == 0) {
        ssiu_mem_unmap();
    }

    ado_mutex_unlock (&ssiu_lock);

    return EOK;
}

int ssiu_setup
(
    uint32_t is_transmit,
    ssi_op_mode_t op_mode,
    ssi_masterslave_mode_t masterslave_mode,
    uint32_t bitnum,
    uint32_t ssi_voices,
    ssi_config_t* config,
    uint32_t tdm_exsplit_map
)
{
    int status;
    uint32_t is_master = 0;

    ado_debug (DB_LVL_DRIVER, " is_transmit %d, op_mode %d",
        is_transmit, op_mode);
    ado_debug (DB_LVL_DRIVER, "masterslave_mode %d, bitnum %d, ssi_voices %d, tdm_exsplit_map 0x%x",
        masterslave_mode, bitnum, ssi_voices, tdm_exsplit_map);
    
    is_master = (masterslave_mode == SSI_MS_MODE_MASTER) ? 1 : 0;

    do {
        status = ssi_set_control_and_mode(is_master, is_transmit, op_mode, ssi_voices, config);
        if (status != EOK) break;

        status = ssiu_set_ssi_mode(op_mode);
        if (status != EOK) break;

        status = ssiu_set_ssi_busif (op_mode, ssi_voices, bitnum, tdm_exsplit_map);
        if (status != EOK) break;
        
    } while (false);

    if (status != EOK) {
        ssiu_ssi_cleanup();
    }

    return EOK;
}

void ssiu_ssi_cleanup(void)
{
    (void) ssi_clear_control_and_mode();
    (void) ssiu_clear_ssi_mode();

    (void) ssiu_busif_cleanup();
    (void) ssiu_clear_ssi_busif();
}

int ssiu_busif_start(ssi_sub_channel_t ssi_sub_channel)
{
    if( !SSIU_SSI ) {
        return EFAULT;
    }

    SSIU_SSI->control |= SSI_CONTROL_START_START(ssi_sub_channel);

    return EOK;
}

int ssiu_busif_stop(ssi_sub_channel_t ssi_sub_channel)
{
    if( !SSIU_SSI ) {
        return EFAULT;
    }

    SSIU_SSI->control &= ~SSI_CONTROL_START_MASK(ssi_sub_channel);

    return EOK;
}

int ssiu_busif_cleanup(void)
{
    if( !SSIU_SSI ) {
        return EFAULT;
    }

    SSIU_SSI->control = 0;

    return EOK;
}

int ssi_set_divisor(uint32_t divisor)
{
    int status;
    uint32_t cr;

    if( !SSI ) {
        return EFAULT;
    }

    if ( divisor == 1 && (SSI->wsr & (SSIWSR_WSMODE_TDM_MONO | SSIWSR_CONT_ENABLED))) {
        ado_error_fmt("Setting SSI divisor to %u is invalid when WS_MODE = 1 or CONT = 1", divisor);
        return EINVAL;
    }

    cr = SSI->cr;

    status = ssi_set_clockdiv_bits(&cr, divisor);
    cr |= SSICR_FORCE_FIXED;

    if(status == EOK) {
        ado_debug(DB_LVL_DRIVER, "Setting SSI divisor to %u", divisor);

        SSI->cr = cr;
    }

    return status;
}

/* SSIU register dump functions */
void ssiu_common_register_dump(void)
{
    if( !SSIU_COMMON ) {
        ado_error_fmt("SSIU memory not mapped");
        return;
    }

    ado_debug(DB_LVL_DRIVER, "SSIU COMMON reg dump: SYSTEM_STATUS[0]=%x SYSTEM_STATUS[2]=%x SYSTEM_STATUS[4]=%x SYSTEM_STATUS[5]=%x",
               SSIU_COMMON->system_status0, SSIU_COMMON->system_status2,
               SSIU_COMMON->system_status4, SSIU_COMMON->system_status6);
    ado_debug(DB_LVL_DRIVER, "SSIU COMMON reg dump: SYSTEM_INT_ENABLE[0]=%x SYSTEM_INT_ENABLE[2]=%x SYSTEM_INT_ENABLE[4]=%x SYSTEM_INT_ENABLE[6]=%x",
               SSIU_COMMON->system_int_enable0, SSIU_COMMON->system_int_enable2,
               SSIU_COMMON->system_int_enable4, SSIU_COMMON->system_int_enable6);
}

void ssiu_ssi_register_dump(void)
{
    uint32_t i;

    if( SSI ) {
        ado_debug(DB_LVL_DRIVER, "SSI reg dump: CR=%x SR=%x WSR=%x FMR=%x FSR=%x",
                    SSI->cr, SSI->sr,
                   SSI->wsr, SSI->fmr, SSI->fsr);
    } else {
        ado_error_fmt("SSI memory not mapped");
    }
    if( SSI_EXT ) {
        ado_debug(DB_LVL_DRIVER, "SSI EXT reg dump: CRE=%x", SSI_EXT->cre);
    }
    if( SSIU_SSI ) {
        ado_debug(DB_LVL_DRIVER, "SSIU SSI reg dump: MODE=%x CONTROL=%x STATUS=%x, INT_ENABLE_MAIN=%x",
                   SSIU_SSI->mode, SSIU_SSI->control,
                   SSIU_SSI->status, SSIU_SSI->int_enable_main);
    } else {
        ado_error_fmt("SSIU memory not mapped");
    }
    if( SSIU_SSI2 ) {
        ado_debug(DB_LVL_DRIVER, "SSIU SSI EXT reg dump: MODE2=%x STATUS2=%x INT_ENABLE_MAIN2=%x",
                   SSIU_SSI2->mode2, SSIU_SSI2->status2,
                   SSIU_SSI2->int_enable_main2);
    }

    for(i = 0; i <= 7; i++) {
        if( SSIU_BUSIF(i) ) {
            ado_debug(DB_LVL_DRIVER, "SSIU BUSIF0-%d reg dump: BUSIF_MODE=%x BUSIF_ADINR=%x BUSIF_DALIGN=%x",
                       i,
                       SSIU_BUSIF(i)->busif_mode,
                       SSIU_BUSIF(i)->busif_adinr,
                       SSIU_BUSIF(i)->busif_dalign);
        } else {
            ado_error_fmt("SSIU memory not mapped");
        }
    }

    if( SSIU_BUSIF2 ) {
        ado_debug(DB_LVL_DRIVER, "SSIU BUSIF EXT reg dump: BUSIF_DALIGN2=%x",
                   SSIU_BUSIF2->busif_dalign2);
    }
}
