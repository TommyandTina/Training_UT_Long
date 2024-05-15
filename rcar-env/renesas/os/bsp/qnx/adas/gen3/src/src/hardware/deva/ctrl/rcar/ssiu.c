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

#include <audio_driver.h>
#include "ssiu.h"
#include "rcar_support.h"

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


/* Helper functions to set SSI register bit fields */

static int ssi_set_role_bits(uint32_t* cr, uint32_t* cre, uint32_t* wsr, uint32_t is_master, uint32_t is_transmit)
{
    if( cr == 0 || wsr == 0 ) {
         return EINVAL;
    }

    (*cr) &= ~( SSICR_TRMD_MASK | SSICR_SWSD_MASK | SSICR_SCKD_MASK );
    (*wsr) &= ~SSIWSR_CONT_MASK;

    if( is_master ) {
        (*cr) |= SSICR_SWSD_OUTPUT | SSICR_SCKD_OUTPUT;
        (*wsr) |= SSIWSR_CONT_ENABLED;
    } else {
        (*cr) |= SSICR_SWSD_INPUT | SSICR_SCKD_INPUT;
        (*wsr) |= SSIWSR_CONT_DISABLED; // for slave the CONT mode must be disabled
    }

    if( is_transmit ) {
        (*cr) |= SSICR_TRMD_TX_MODE;
    } else {
        (*cr) |= SSICR_TRMD_RX_MODE;
    }

    return EOK;
}

static int ssi_set_mode_bits(uint32_t* cr, uint32_t* cre, uint32_t* wsr, ssi_op_mode_t mode, uint32_t voicenum)
{
    if( cr == 0 || cre == 0 || wsr == 0 ) {
         return EINVAL;
    }

    // for most modes, populate the CHNL and SDTA bit fields of SSICR,
    // the MONO and WSMODE bit fields of SSIWSR and if applicable,
    // the CHNL2 bit field of SSICRE
    // for the TDM split modes also overwrite the SWL and DWL bit fields
    // that were populated by ssi_set_config_bits
    (*cr) &= ~( SSICR_CHNL_MASK | SSICR_SDTA_MASK );
    (*wsr) &= ~( SSIWSR_MONO_MASK | SSIWSR_WSMODE_MASK );
    (*cre) &= ~SSICRE_CHNL2_MASK;

    if( mode == SSI_OP_MODE_MONO ) {
        (*cr) &= ~SSICR_SDTA_MASK; // for monaural SDTA must be forced to 0
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_MONO;
        (*cr) |= SSICR_CHNL_1 | SSICR_SDTA_SDATA_FIRST;
    } else if( mode == SSI_OP_MODE_STEREO ) {
        (*cr) |=  SSICR_CHNL_1 | SSICR_SDTA_SDATA_FIRST;
        (*wsr) |= SSIWSR_WSMODE_STEREO_MULTICH | SSIWSR_MONO_TDM ;
    } else if( mode == SSI_OP_MODE_MULTICH ) {
        (*cr) |= SSICR_CHNL_1; // two channels per SSI
        (*cr) |=  SSICR_SDTA_SDATA_FIRST;
        (*wsr) |= SSIWSR_WSMODE_STEREO_MULTICH | SSIWSR_MONO_TDM ;
    } else if( (mode == SSI_OP_MODE_TDM) || (mode == SSI_OP_MODE_TDMEXT) ) {
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        if( voicenum == 4 ) {
            (*cr) |= SSICR_CHNL_2;
        } else if( voicenum == 6 ) {
            (*cr) |= SSICR_CHNL_3;
        } else if( voicenum == 8 ) {
            (*cr) |= SSICR_CHNL_4;
        } else {
            return EINVAL;
        }
    // for the TDM split modes SWL and DWL have special settings,
    // overwrite the default settings
    } else if( mode == SSI_OP_MODE_TDMSPLIT_4XMONO ) {
        (*cr) &= ~( SSICR_SWL_MASK | SSICR_DWL_MASK );
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cr) |= SSICR_DWL_32BIT | SSICR_SWL_32BIT | SSICR_CHNL_2;
    } else if( mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ) {
        (*cr) &= ~( SSICR_SWL_MASK | SSICR_DWL_MASK );
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cr) |= SSICR_DWL_32BIT | SSICR_SWL_32BIT | SSICR_CHNL_4;
    } else if( mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cre) |= SSICRE_CHNL2_MASK;
    } else if( mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ) {
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cr) |= SSICR_CHNL_4;
    } else if( mode == SSI_OP_MODE_TDM_16CHAN ) {
        (*wsr) |=  SSIWSR_WSMODE_TDM_MONO | SSIWSR_MONO_TDM;
        (*cre) |= SSICRE_CHNL2_MASK;
    }

    return EOK;
}

static int ssi_set_config_bits(uint32_t* cr, ssi_config_t * config)
{
    if( cr == 0 ) {
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
    if( cr == 0 ) {
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
    if( cr == 0 ) {
         return EINVAL;
    }

    (*cr) &= ~(SSICR_EN_MASK | SSICR_MUEN_MASK);
    if( enable ) {
        (*cr) |= SSICR_EN_ENABLE;
    } else {
        (*cr) |= SSICR_EN_DISABLE;
    }
    if( mute ) {
        (*cr) |= SSICR_MUEN_MUTED;
    } else {
        (*cr) |= SSICR_MUEN_UNMUTED;
    }

    return EOK;
}

#if 0
static int ssi_set_interrupt_bits(uint32_t* cr, ssi_interrupt_t* interrupts)
{
    if( cr == 0 || interrupts == 0 ) {
        return EINVAL;
    }
    (*cr) &= ~(SSICR_DMEN_MASK | SSICR_UIEN_MASK | SSICR_OIEN_MASK | SSICR_IIEN_MASK | SSICR_DIEN_MASK);
    if( interrupts->dma ) {
        (*cr) |= SSICR_DMEN_ENABLE;
    } else {
        (*cr) |= SSICR_DMEN_DISABLE;
    }
    if( interrupts->underflow ) {
        (*cr) |= SSICR_UIEN_ENABLE;
    } else {
        (*cr) |= SSICR_UIEN_DISABLE;
    }
    if( interrupts->overflow ) {
        (*cr) |= SSICR_OIEN_ENABLE;
    } else {
        (*cr) |= SSICR_OIEN_DISABLE;
    }
    if( interrupts->idle_mode ) {
        (*cr) |= SSICR_IIEN_ENABLE;
    } else {
        (*cr) |= SSICR_IIEN_DISABLE;
    }
    if( interrupts->data ) {
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
    ssi_channel_t ssi_channel,
    uint32_t is_master,
    uint32_t is_transmit,
    ssi_op_mode_t ssi_op_mode,
    uint32_t voicenum,
    ssi_config_t* config
)
{
    uint32_t cr, cre = 0, wsr;

    if( !rcar_ssi_supported(ssi_channel) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    if( !SSI(ssi_channel) ) {
        return EFAULT;
    }

    cr = SSI(ssi_channel)->cr;
    if( SSI_EXT(ssi_channel) ) {
        cre = SSI_EXT(ssi_channel)->cre;
    }
    wsr = SSI(ssi_channel)->wsr;

    if( ssi_set_role_bits( &cr, &cre, &wsr, is_master, is_transmit ) != EOK ) {
        return EINVAL;
    }

    if( ssi_set_config_bits( &cr, config) != EOK ) {
        return EINVAL;
    }

    // important: call ssi_set_mode_bits after ssi_set_config_bits, because it
    // overrides the SWL and DWL bit fields with special values for some ssi_op_mode values
    if( ssi_set_mode_bits( &cr, &cre, &wsr, ssi_op_mode, voicenum ) != EOK ) {
        return EINVAL;
    }

    if( ssi_set_control_bits( &cr, 0, 0 ) != EOK ) {
        return EINVAL;
    }

    cr |= SSICR_FORCE_FIXED;

    SSI(ssi_channel)->cr = cr;
    if( SSI_EXT(ssi_channel) ) {
        SSI_EXT(ssi_channel)->cre = cre;
    }
    SSI(ssi_channel)->wsr = wsr;

    return EOK;
}

static int ssi_clear_control_and_mode( ssi_channel_t ssi_channel )
{
    if (!rcar_ssi_supported(ssi_channel) || !SSI(ssi_channel))
    {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    SSI(ssi_channel)->cr &= ~0x7FFF;
    SSI(ssi_channel)->wsr = 0;

    return EOK;
}

/* Public SSI level functions - start/stop */

int ssi_start(ssi_channel_t ssi_channel, ssi_start_mode_t start_mode)
{
    if( !rcar_ssi_supported(ssi_channel) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    if( !SSI(ssi_channel) ) {
        return EFAULT;
    }

    // if this SSI uses independent start, set SSICR_EN at this time to start
    // the transfer; otherwise set only the other registers, the actual transfer
    // will start in that case via one of the SSI_CONTROL_SSI1029 or SSI_CONTROL_SSI34
    // bit masks of the SSI_CONTROL register (implemented as part of ssiu_start)
    if( start_mode == SSI_INDEPENDENT_START ) {
        SSI(ssi_channel)->cr |= SSICR_EN_ENABLE | SSICR_DMEN_ENABLE |
                                SSICR_UIEN_ENABLE | SSICR_OIEN_ENABLE | SSICR_FORCE_FIXED;
    } else {
        SSI(ssi_channel)->cr |= SSICR_DMEN_ENABLE |
                                SSICR_UIEN_ENABLE | SSICR_OIEN_ENABLE | SSICR_FORCE_FIXED;
    }

    return EOK;
}

int ssi_wait_status(ssi_channel_t ssi_channel, uint32_t bitmask)
{
    uint32_t i;
    uint32_t timeout = 100; /* 100 * 100 microseconds = 10 miliseconds */

    if( !rcar_ssi_supported(ssi_channel) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    if( !SSI(ssi_channel) ) {
        return EFAULT;
    }

    for (i = 0; i < timeout; i++)
    {
        nanospin_ns(100000); /* 100 microseconds */

        if (SSI(ssi_channel)->sr & bitmask)
        {
            return EOK;
        }
    }

    return ETIMEDOUT;
}

int ssi_stop(ssi_channel_t ssi_channel)
{
    int dirq_status;

    if( !rcar_ssi_supported(ssi_channel) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    if( !SSI(ssi_channel) ) {
        return EFAULT;
    }

    // old driver was not disabling the OIEN and UIEN interrupts here
    SSI(ssi_channel)->cr &= ~(SSICR_DMEN_MASK | SSICR_UIEN_MASK | SSICR_OIEN_MASK);

    dirq_status = ssi_wait_status(ssi_channel, SSISR_DIRQ_MASK);

    if ( dirq_status != EOK ) {
        ado_error_fmt("SSI%d timeout waiting for DIRQ");
    }

    SSI(ssi_channel)->cr &= ~SSICR_EN_MASK;
    SSI(ssi_channel)->cr |= SSICR_IIEN_ENABLE;

    return EOK;
}

int ssi_stop_idle_int(ssi_channel_t ssi_channel)
{
    if( !rcar_ssi_supported(ssi_channel) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    if( !SSI(ssi_channel) ) {
        return EFAULT;
    }
    SSI(ssi_channel)->cr &= ~(SSICR_EN_MASK|SSICR_IIEN_MASK);
    return EOK;
}

/* SSIU register configuration for independent channels */
static int ssiu_set_mode_indepchannel
(
    uint32_t ssi_channel_mask,
    uint32_t ssi_busif_mask,
    ssi_masterslave_mode_t masterslave_mode,
    ssi_start_mode_t start_mode
)
{
    ssi_channel_t ssi_channel;

    if ( !SSIU_COMMON ) {
        return EFAULT;
    }

    for (ssi_channel = SSI_CHANNEL_0; ssi_channel <= SSI_CHANNEL_9; ssi_channel++) {
        if ( !(SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask) ) {
            continue;
        }
        if ( !rcar_ssi_supported(ssi_channel) ) {
            ado_error_fmt("SSI%d is not supported", ssi_channel);
            return EINVAL;
        }
        if ( !SSI(ssi_channel)) {
            return EFAULT;
        }
    }

    ado_mutex_lock(&ssiu_lock);

    // MODE0

    // enable or disable the independent mode for each used ssi_channel
    for (ssi_channel = SSI_CHANNEL_0; ssi_channel <= SSI_CHANNEL_9; ssi_channel++) {
        if ( SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask ) {
            SSIU_COMMON->mode0 &= ~SSI_MODE0_IND_MASK(ssi_channel);
            if ( SSI_CHANNEL_MASK(ssi_channel) & ssi_busif_mask ) {
                SSIU_COMMON->mode0 |= SSI_MODE0_IND_DISABLE(ssi_channel);
            } else {
                SSIU_COMMON->mode0 |= SSI_MODE0_IND_ENABLE(ssi_channel);
            }
        }
    }

    //MODE1, MODE2, MODE3

    // to set up any of SSI 0,1,2,9 as an independent transfer channel, disable the
    // four channel mode of SSI 0,1,2,9
    if( ssi_channel_mask & SSI_CHANNEL_MASK_0129 ) {
        SSIU_COMMON->mode2 &= ~SSI_MODE2_SSI0129FOURMODULE_MASK;
        SSIU_COMMON->mode2 |= SSI_MODE2_SSI0129FOURMODULE_DISABLE;
    }
    // to set up any of SSI 0,1,2 as an independent transfer channel, disable the
    // three channel mode of SSI 0,1,2
    if( ssi_channel_mask & SSI_CHANNEL_MASK_012 ) {
        SSIU_COMMON->mode1 &= ~SSI_MODE1_SSI012THREEMODULE_MASK;
        SSIU_COMMON->mode1 |= SSI_MODE1_SSI012THREEMODULE_DISABLE;
    }
    // SSI1PIN: set up pin sharing for SSI 0,1 if both are used, otherwise
    // if SSI 0 or 1 used, disable the pin sharing mode of SSI 0,1
    if ( ssi_channel_mask & SSI_CHANNEL_MASK_01 ) {
        SSIU_COMMON->mode1 &= ~SSI_MODE1_SSI1PIN_MASK;
        if ( (ssi_channel_mask & SSI_CHANNEL_MASK_01) == SSI_CHANNEL_MASK_01 ) {
            if ( masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
                SSIU_COMMON->mode1 |= SSI_MODE1_SSI1PIN_SSI01_MASTERSLAVE;
            } else {
                SSIU_COMMON->mode1 |= SSI_MODE1_SSI1PIN_SSI01_BOTHSLAVE;
            }
        } else {
            SSIU_COMMON->mode1 |= SSI_MODE1_SSI1PIN_SSI01_INDEPENDENT;
        }
    }
    // SSI2PIN: to set up any of SSI 0,2 as an independent channel,                  // use pin 1 model
    // disable the pin sharing mode of SSI 0,2
    if ( ssi_channel_mask & SSI_CHANNEL_MASK_02 ) {
        SSIU_COMMON->mode1 &= ~SSI_MODE1_SSI2PIN_MASK;
        if ((ssi_channel_mask & SSI_CHANNEL_MASK_02) == SSI_CHANNEL_MASK_02) {
            if( masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
                SSIU_COMMON->mode1 |= SSI_MODE1_SSI2PIN_SSI02_MASTERSLAVE;
            } else {
                SSIU_COMMON->mode1 |= SSI_MODE1_SSI2PIN_SSI02_BOTHSLAVE;
            }
        } else {
            SSIU_COMMON->mode1 |= SSI_MODE1_SSI2PIN_SSI02_INDEPENDENT;
        }
    }
    // SSI9PIN - the story is a bit more complex here:
    // - to set up SSI 9 as an independent channel, disable the pin
    // sharing mode for both the SSI 0,9 and SSI 3,9 pairs;
    // - to set up SSI 0 as an independent channel, disable the pin
    // sharing mode for SSI 0,9 if currently set, don't alter the pin
    // sharing mode for SSI 3,9
    // - to set up SSI 3 as an independent channel, disable the pin
    // sharing mode for SSI 3,9 if currently set, don't alter the pin
    // sharing mode for SSI 0,9
    if ((ssi_channel_mask & SSI_CHANNEL_MASK_09) == SSI_CHANNEL_MASK_09){
        SSIU_COMMON->mode2 &= ~SSI_MODE2_SSI9PIN_MASK;
        if( masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
            SSIU_COMMON->mode2 |= SSI_MODE2_SSI9PIN_SSI09_MASTERSLAVE;
        } else {
            SSIU_COMMON->mode2 |= SSI_MODE2_SSI9PIN_SSI09_BOTHSLAVE;
        }
    } else if ((ssi_channel_mask & SSI_CHANNEL_MASK_39) == SSI_CHANNEL_MASK_39){
        SSIU_COMMON->mode2 &= ~SSI_MODE2_SSI9PIN_MASK;
        if( masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
            SSIU_COMMON->mode2 |= SSI_MODE2_SSI9PIN_SSI39_MASTERSLAVE;
        } else {
            SSIU_COMMON->mode2 |= SSI_MODE2_SSI9PIN_SSI39_BOTHSLAVE;
        }
    } else if ( ssi_channel_mask & SSI_CHANNEL_MASK_09 ) {
        if( (SSIU_COMMON->mode2 & SSI_MODE2_SSI9PIN_MASK) ==
            SSI_MODE2_SSI9PIN_SSI09_BOTHSLAVE ||
            (SSIU_COMMON->mode2 & SSI_MODE2_SSI9PIN_MASK) ==
            SSI_MODE2_SSI9PIN_SSI09_MASTERSLAVE ) {
            SSIU_COMMON->mode2 &= ~SSI_MODE2_SSI9PIN_MASK;
            SSIU_COMMON->mode2 |= SSI_MODE2_SSI9PIN_SSIO9_39_INDEPENDENT;
        }
    } else if ( ssi_channel_mask & SSI_CHANNEL_MASK_39 ) {
        if( (SSIU_COMMON->mode2 & SSI_MODE2_SSI9PIN_MASK) ==
            SSI_MODE2_SSI9PIN_SSI39_BOTHSLAVE ||
            (SSIU_COMMON->mode2 & SSI_MODE2_SSI9PIN_MASK) ==
            SSI_MODE2_SSI9PIN_SSI39_MASTERSLAVE ) {
            SSIU_COMMON->mode2 &= ~SSI_MODE2_SSI9PIN_MASK;
            SSIU_COMMON->mode2 |= SSI_MODE2_SSI9PIN_SSIO9_39_INDEPENDENT;
        }
    }

    // SSI3PIN: to set up any of SSI 0,3 as an independent channel,
    // disable the pin sharing mode of SSI 0,3
    if ( ssi_channel_mask & SSI_CHANNEL_MASK_03 ) {
        SSIU_COMMON->mode3 &= ~SSI_MODE3_SSI3PIN_MASK;
        if ((ssi_channel_mask & SSI_CHANNEL_MASK_03) == SSI_CHANNEL_MASK_03) {
            if( masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
                SSIU_COMMON->mode3 |= SSI_MODE3_SSI3PIN_SSI03_MASTERSLAVE;
            } else {
                SSIU_COMMON->mode3 |= SSI_MODE3_SSI3PIN_SSI03_BOTHSLAVE;
            }
        } else {
            SSIU_COMMON->mode3 |= SSI_MODE3_SSI3PIN_SSI03_INDEPENDENT;
        }
    }

    // SSI4PIN: to set up any of SSI 3,4 as an independent channel,
    // disable the pin sharing mode and the sync mode of SSI 3,4
    if ( ssi_channel_mask & SSI_CHANNEL_MASK_34 ) {
        SSIU_COMMON->mode1 &= ~(SSI_MODE1_SSI4PIN_MASK|SSI_MODE1_SSI34SYNC_MASK);
        if ((ssi_channel_mask & SSI_CHANNEL_MASK_34) == SSI_CHANNEL_MASK_34) {
            if( masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
                SSIU_COMMON->mode1 |= SSI_MODE1_SSI4PIN_SSI34_MASTERSLAVE;
            } else {
                SSIU_COMMON->mode1 |= SSI_MODE1_SSI4PIN_SSI34_BOTHSLAVE;
            }
            if( start_mode == SSI_SYNC_SSI34_START ) {
                SSIU_COMMON->mode1 |= SSI_MODE1_SSI34SYNC_ENABLE;
            }
        } else {
            SSIU_COMMON->mode1 |= SSI_MODE1_SSI4PIN_SSI34_INDEPENDENT |
                                  SSI_MODE1_SSI34SYNC_DISABLE;
        }
    }

    ado_mutex_unlock(&ssiu_lock);

    return EOK;
}

static int ssiu_clear_mode_indepchannel (uint32_t ssi_channel_mask)
{
    ssi_channel_t ssi_channel;

    if( !SSIU_COMMON ) {
        return EFAULT;
    }

    ado_mutex_lock(&ssiu_lock);

    for (ssi_channel = SSI_CHANNEL_0; ssi_channel <= SSI_CHANNEL_9; ssi_channel++) {
        if ( SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask ) {
            SSIU_COMMON->mode0 &= ~SSI_MODE0_IND_MASK(ssi_channel);
        }
    }

    //MODE1,2,3 - clear applicable pin sharing mask, and the sync3,4 mask for SSI 3,4
    if ( (ssi_channel_mask & SSI_CHANNEL_MASK_01) == SSI_CHANNEL_MASK_01 ) {
        SSIU_COMMON->mode1 &= ~SSI_MODE1_SSI1PIN_MASK;
    }
    if ( (ssi_channel_mask & SSI_CHANNEL_MASK_02) == SSI_CHANNEL_MASK_02 ) {
        SSIU_COMMON->mode1 &= ~SSI_MODE1_SSI2PIN_MASK;
    }
    if ( (ssi_channel_mask & SSI_CHANNEL_MASK_03) == SSI_CHANNEL_MASK_03 ) {
        SSIU_COMMON->mode3 &= ~SSI_MODE3_SSI3PIN_MASK;
    }
    if ( (ssi_channel_mask & SSI_CHANNEL_MASK_09) == SSI_CHANNEL_MASK_09 ) {
        SSIU_COMMON->mode2 &= ~SSI_MODE2_SSI9PIN_MASK;
    } else if ( (ssi_channel_mask & SSI_CHANNEL_MASK_39) == SSI_CHANNEL_MASK_39 ) {
        SSIU_COMMON->mode2 &= ~SSI_MODE2_SSI9PIN_MASK;
    }
    if ( (ssi_channel_mask & SSI_CHANNEL_MASK_34) == SSI_CHANNEL_MASK_34 ) {
        SSIU_COMMON->mode1 &= ~(SSI_MODE1_SSI4PIN_MASK|SSI_MODE1_SSI34SYNC_MASK);
    }

    ado_mutex_unlock(&ssiu_lock);

    return EOK;
}

static int ssiu_set_mode_multichannel (uint32_t ssi_channel_mask, ssi_masterslave_mode_t masterslave_mode)
{
    uint32_t mask;

    if( !SSIU_COMMON ) {
        return EFAULT;
    }

    ado_mutex_lock(&ssiu_lock);

    // MODE0
    mask = SSI_MODE0_IND_MASK(SSI_CHANNEL_0) |
           SSI_MODE0_IND_MASK(SSI_CHANNEL_1) |
           SSI_MODE0_IND_MASK(SSI_CHANNEL_2);
    if (ssi_channel_mask == SSI_CHANNEL_MASK_0129) {
        mask |= SSI_MODE0_IND_MASK(SSI_CHANNEL_9);
    }
    SSIU_COMMON->mode0 &= ~mask;

    // disable the independent mode for SSI channels 0,1,2(,9)
    mask = SSI_MODE0_IND_DISABLE(SSI_CHANNEL_0) |
           SSI_MODE0_IND_DISABLE(SSI_CHANNEL_1) |
           SSI_MODE0_IND_DISABLE(SSI_CHANNEL_2);
    if (ssi_channel_mask == SSI_CHANNEL_MASK_0129) {
        mask |= SSI_MODE0_IND_DISABLE(SSI_CHANNEL_9);
    }
    SSIU_COMMON->mode0 |= mask;

    //MODE1
    mask = SSI_MODE1_SSI012THREEMODULE_MASK |
           SSI_MODE1_SSI1PIN_MASK|
           SSI_MODE1_SSI2PIN_MASK;
    SSIU_COMMON->mode1 &= ~mask;

    // enable the pin sharing mode for SSI 0,1 and SSI 0,2
    if( masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
        mask = SSI_MODE1_SSI2PIN_SSI02_MASTERSLAVE |
               SSI_MODE1_SSI1PIN_SSI01_MASTERSLAVE;
    } else {
        mask = SSI_MODE1_SSI2PIN_SSI02_BOTHSLAVE |
               SSI_MODE1_SSI1PIN_SSI01_BOTHSLAVE;
    }
    if (ssi_channel_mask == SSI_CHANNEL_MASK_012) {
        // enable the SSI 0,1,2 three channel mode
        mask |= SSI_MODE1_SSI012THREEMODULE_ENABLE;
    } else {
        // disable the SSI 0,1,2 three channel mode
        mask |= SSI_MODE1_SSI012THREEMODULE_DISABLE;
    }
    SSIU_COMMON->mode1 |= mask;

    //MODE2
    mask = SSI_MODE2_SSI0129FOURMODULE_MASK;
    if ( ( (SSIU_COMMON->mode2 & SSI_MODE2_SSI9PIN_MASK) ==
           SSI_MODE2_SSI9PIN_SSI09_BOTHSLAVE) ||
         ( (SSIU_COMMON->mode2 & SSI_MODE2_SSI9PIN_MASK) ==
           SSI_MODE2_SSI9PIN_SSI09_MASTERSLAVE) ) {
        mask |= SSI_MODE2_SSI9PIN_MASK;
    }
    SSIU_COMMON->mode2 &= ~mask;

    if (ssi_channel_mask == SSI_CHANNEL_MASK_012) {
        // disable the SSI 0,1,2,9 four channel mode
        mask = SSI_MODE2_SSI0129FOURMODULE_DISABLE;
        // disable the pin sharing mode for SSI 0,9 if set up;
        // caution: same bit field can be used to set up SSI 0,3 pin
        // sharing, which we don't want to disturb
        if ( ( (SSIU_COMMON->mode2 & SSI_MODE2_SSI9PIN_MASK) ==
               SSI_MODE2_SSI9PIN_SSI09_BOTHSLAVE) ||
             ( (SSIU_COMMON->mode2 & SSI_MODE2_SSI9PIN_MASK) ==
               SSI_MODE2_SSI9PIN_SSI09_MASTERSLAVE) ) {
            mask |= SSI_MODE2_SSI9PIN_SSIO9_39_INDEPENDENT;
        }
    } else {
        // enable the SSI 0,1,2,9 four channel mode
        mask = SSI_MODE2_SSI0129FOURMODULE_ENABLE;
        // enable the pin sharing mode for SSI 0,9
        if ( masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ) {
            mask |= SSI_MODE2_SSI9PIN_SSI09_MASTERSLAVE;
        } else {
            mask |= SSI_MODE2_SSI9PIN_SSI09_BOTHSLAVE;
        }
    }
    SSIU_COMMON->mode2 |= mask;

    //MODE3
    // disable the pin sharing for SSI 0,3
    SSIU_COMMON->mode3 &= ~SSI_MODE3_SSI3PIN_MASK;
    SSIU_COMMON->mode3 |= SSI_MODE3_SSI3PIN_SSI03_INDEPENDENT;

    ado_mutex_unlock(&ssiu_lock);

    return EOK;
}

static int ssiu_clear_mode_multichannel (uint32_t ssi_channel_mask)
{
    uint32_t mask;

    if( !SSIU_COMMON ) {
        return EFAULT;
    }

    ado_mutex_lock(&ssiu_lock);

    //MODE0 - leave unchanged - independent mode disabled for SSI 0,1,2(,9) is the default

    //MODE1 - clear the three module bit and the pin1 and pin2 masks
    mask = SSI_MODE1_SSI1PIN_MASK|SSI_MODE1_SSI2PIN_MASK;
    if (ssi_channel_mask == SSI_CHANNEL_MASK_012) {
        mask |= SSI_MODE1_SSI012THREEMODULE_MASK;
    }
    SSIU_COMMON->mode1 &= ~mask;

    //MODE2 - clear the four module bit and the pin9 mask
    if (ssi_channel_mask == SSI_CHANNEL_MASK_0129) {
        SSIU_COMMON->mode2 &= ~(SSI_MODE2_SSI0129FOURMODULE_MASK | SSI_MODE2_SSI9PIN_MASK);
    }

    //MODE3 - leave unchanged

    ado_mutex_unlock(&ssiu_lock);

    return EOK;
}

static int ssiu_set_hdmi_mode
(
    ssi_channel_t clk_ssi_channel,
    uint32_t ssi_channel_mask,
    ssi_hdmi_channel_t hdmi_channel
)
{
    uint32_t ssi_channel;
    uint32_t hdmi_sel;

    if (!(ssi_channel_mask & SSI_CHANNEL_MASK_ALL)) {
        return EINVAL;
    }

    if( !SSIU_COMMON_EXT ) {
        return EINVAL;
    }

    hdmi_sel = SSI_HDMI_SCK_WS_SEL(clk_ssi_channel);
    ssi_channel = SSI_CHANNEL_0;

    while ( !(SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask) ) {
        ssi_channel++;
    }

    hdmi_sel |= SSI_HDMI_SD0_SEL(ssi_channel);
    ssi_channel_mask &= ~SSI_CHANNEL_MASK(ssi_channel);

    if ( ssi_channel_mask ) {
        ssi_channel++;
        while ( !(SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask) ) {
            ssi_channel++;
        }

        hdmi_sel |= SSI_HDMI_SD1_SEL(ssi_channel);
        ssi_channel_mask &= ~SSI_CHANNEL_MASK(ssi_channel);
    } else {
        hdmi_sel |= SSI_HDMI_SD1_SEL_MASK;
    }

    if ( ssi_channel_mask ) {
        ssi_channel++;
        while ( !(SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask) ) {
            ssi_channel++;
        }

        hdmi_sel |= SSI_HDMI_SD2_SEL(ssi_channel);
        ssi_channel_mask &= ~SSI_CHANNEL_MASK(ssi_channel);
    } else {
        hdmi_sel |= SSI_HDMI_SD2_SEL_MASK;
    }

    if ( ssi_channel_mask ) {
        ssi_channel++;
        while ( !(SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask) ) {
            ssi_channel++;
        }

        hdmi_sel |= SSI_HDMI_SD3_SEL(ssi_channel);
        ssi_channel_mask &= ~SSI_CHANNEL_MASK(ssi_channel);
    } else {
        hdmi_sel |= SSI_HDMI_SD3_SEL_MASK;
    }

    if ( ssi_channel_mask ) {
        return EINVAL;
    }

    ado_mutex_lock(&ssiu_lock);

    if( hdmi_channel == SSI_HDMI_CHANNEL_0 ) {
        SSIU_COMMON_EXT->hdmi0_sel = hdmi_sel;
    } else if( hdmi_channel == SSI_HDMI_CHANNEL_1 ) {
        SSIU_COMMON_EXT->hdmi1_sel = hdmi_sel;
    }

    ado_mutex_unlock(&ssiu_lock);

    return EOK;
}

// make sure no HDMI channel selects the indicated ssi_channel
// as either clock and ws source or as serial data source
// if it does, clear the corresponding HDMIx_SEL register
static int ssiu_clear_hdmi_mode (uint32_t ssi_channel_mask, ssi_hdmi_channel_t hdmi_channel)
{
    ssi_channel_t ssi_channel;

    uint32_t hdmi_sel_clear = SSI_HDMI_SCK_WS_SEL_MASK
                        | SSI_HDMI_SD0_SEL_MASK
                        | SSI_HDMI_SD1_SEL_MASK
                        | SSI_HDMI_SD2_SEL_MASK
                        | SSI_HDMI_SD3_SEL_MASK;

    if( !SSIU_COMMON_EXT ) {
        return EINVAL;
    }

    ado_mutex_lock(&ssiu_lock);

    if ( (hdmi_channel == SSI_HDMI_CHANNEL_0 || hdmi_channel == SSI_HDMI_CHANNEL_NUM) &&
         (SSIU_COMMON_EXT->hdmi0_sel & hdmi_sel_clear) != hdmi_sel_clear ) {
        for (ssi_channel = SSI_CHANNEL_0; ssi_channel < SSI_CHANNEL_NUM; ssi_channel++) {
            if (SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask) {
                if ( ( SSIU_COMMON_EXT->hdmi0_sel & SSI_HDMI_SD0_SEL_MASK ) ==
                     SSI_HDMI_SD0_SEL(ssi_channel) ||
                     ( SSIU_COMMON_EXT->hdmi0_sel & SSI_HDMI_SD1_SEL_MASK ) ==
                     SSI_HDMI_SD1_SEL(ssi_channel) ||
                     ( SSIU_COMMON_EXT->hdmi0_sel & SSI_HDMI_SD2_SEL_MASK ) ==
                     SSI_HDMI_SD2_SEL(ssi_channel) ||
                     ( SSIU_COMMON_EXT->hdmi0_sel & SSI_HDMI_SD3_SEL_MASK ) ==
                     SSI_HDMI_SD3_SEL(ssi_channel) ||
                     ( SSIU_COMMON_EXT->hdmi0_sel & SSI_HDMI_SCK_WS_SEL_MASK ) ==
                     SSI_HDMI_SCK_WS_SEL(ssi_channel) ) {
                    SSIU_COMMON_EXT->hdmi0_sel = hdmi_sel_clear;
                    break;
                }
            }
        }
    }

    if ( (hdmi_channel == SSI_HDMI_CHANNEL_1 || hdmi_channel == SSI_HDMI_CHANNEL_NUM) &&
         (SSIU_COMMON_EXT->hdmi1_sel & hdmi_sel_clear) != hdmi_sel_clear ) {
        for (ssi_channel = SSI_CHANNEL_0; ssi_channel < SSI_CHANNEL_NUM; ssi_channel++) {
            if (SSI_CHANNEL_MASK(ssi_channel) & ssi_channel_mask) {
                if ( ( SSIU_COMMON_EXT->hdmi1_sel & SSI_HDMI_SD0_SEL_MASK ) ==
                     SSI_HDMI_SD0_SEL(ssi_channel) ||
                     ( SSIU_COMMON_EXT->hdmi1_sel & SSI_HDMI_SD1_SEL_MASK ) ==
                     SSI_HDMI_SD1_SEL(ssi_channel) ||
                     ( SSIU_COMMON_EXT->hdmi1_sel & SSI_HDMI_SD2_SEL_MASK ) ==
                     SSI_HDMI_SD2_SEL(ssi_channel) ||
                     ( SSIU_COMMON_EXT->hdmi1_sel & SSI_HDMI_SD3_SEL_MASK ) ==
                     SSI_HDMI_SD3_SEL(ssi_channel) ||
                     ( SSIU_COMMON_EXT->hdmi1_sel & SSI_HDMI_SCK_WS_SEL_MASK ) ==
                     SSI_HDMI_SCK_WS_SEL(ssi_channel) ) {
                    SSIU_COMMON_EXT->hdmi1_sel = hdmi_sel_clear;
                    break;
                }
            }
        }
    }

    ado_mutex_unlock(&ssiu_lock);

    return EOK;
}

static int ssiu_set_ssi_mode(ssi_channel_t ssi_channel, ssi_op_mode_t op_mode)
{
    if( !rcar_ssi_supported( ssi_channel ) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    if( !SSIU_SSI(ssi_channel) ) {
        return EFAULT;
    }

    SSIU_SSI(ssi_channel)->mode = 0;
    if( SSIU_SSI2(ssi_channel) ) {
        SSIU_SSI2(ssi_channel)->mode2 = 0;
    }

    if( op_mode == SSI_OP_MODE_TDMEXT ) {
        SSIU_SSI(ssi_channel)->mode |= SSI_MODE_TDMEXT_ENABLE;
    } else if( op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO ) {
        SSIU_SSI(ssi_channel)->mode |= SSI_MODE_TDMSPLIT_ENABLE | SSI_MODE_FSMODE_ENABLE;
    } else if( op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO ) {
        SSIU_SSI(ssi_channel)->mode |= SSI_MODE_TDMSPLIT_ENABLE;
    } else if( op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ||
               op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
        SSIU_SSI(ssi_channel)->mode |= SSI_MODE_TDMSPLIT_ENABLE;
        SSIU_SSI2(ssi_channel)->mode2 |= SSI_MODE2_EX_FUNC_ENABLE;
    } else if( op_mode == SSI_OP_MODE_TDM_16CHAN ) {
        SSIU_SSI(ssi_channel)->mode |= SSI_MODE_TDMEXT_ENABLE;
        SSIU_SSI2(ssi_channel)->mode2 |= SSI_MODE2_EX_FUNC_ENABLE;
    }
    return EOK;
}

static int ssiu_clear_ssi_mode(ssi_channel_t ssi_channel)
{
    if( !rcar_ssi_supported( ssi_channel ) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    if( !SSIU_SSI(ssi_channel) ) {
        return EFAULT;
    }

    SSIU_SSI(ssi_channel)->mode = 0;
    if( SSIU_SSI2(ssi_channel) ) {
        SSIU_SSI2(ssi_channel)->mode2 = 0;
    }

    return EOK;
}

#define TDM_EXSPLIT_MAP_SUBCHN(map, subchn) ((map >> (subchn << 2)) & 0xF)
static int ssiu_validate_tdm_exsplit_map( uint32_t num_subchan, uint32_t tdm_exsplit_map )
{
    static const uint32_t tdm_exsplit_max_voices_map = 0x2428242A;
    int i, subchan_to_skip = 0;

    for( i = 0; i < num_subchan; i++ ) {
        uint32_t tdm_exsplit_map_subchn_entry = TDM_EXSPLIT_MAP_SUBCHN (tdm_exsplit_map, i);
        // if this subchan must be skipped, check that the entry for it is zero
        if( subchan_to_skip ) {
            if( tdm_exsplit_map_subchn_entry != 0 ) {
                return EINVAL;
            }
            subchan_to_skip --;
            continue;
        } else {
            // this subchan should be populated, check that the entry for it is non zero
            if( tdm_exsplit_map_subchn_entry == 0 ) {
                return EINVAL;
            }
        }
        // check that the subchan entry is even
        if( ( (tdm_exsplit_map_subchn_entry >> 1) << 1) != tdm_exsplit_map_subchn_entry ) {
            return EINVAL;
        }
        // check that the subchan entry does not exceed the max for this subchan
        if( tdm_exsplit_map_subchn_entry > TDM_EXSPLIT_MAP_SUBCHN(tdm_exsplit_max_voices_map, i) ) {
            return EINVAL;
        }

        // now we know that the entry is valid

        // if the entry for this subchan is greater than two, then the next subchan(s)
        // must be skipped, i.e. their entries must be 0; determine how many subchans to skip
        subchan_to_skip = (tdm_exsplit_map_subchn_entry >> 1) - 1;
    }
    return EOK;
}

static int ssiu_set_ssi_busif(ssi_channel_t ssi_channel, ssi_op_mode_t op_mode, uint32_t voicenum, uint32_t bitnum, uint32_t tdm_exsplit_map)
{
    uint32_t otbl, chnum;
    uint32_t num_subchan = 1;
    uint32_t i;
    int status = EOK;

    if( !rcar_ssi_supported( ssi_channel ) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }

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

    // validate bitnum for all modes except for the simple modes
    if( (op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN) ||
        (op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN) ) {
        if( bitnum != 16 ) {
            return EINVAL;
        }
        if( tdm_exsplit_map == 0 ) {
            return EINVAL;
        }
    } else if( (op_mode != SSI_OP_MODE_MONO && op_mode != SSI_OP_MODE_STEREO) &&
        (op_mode != SSI_OP_MODE_MULTICH) ) {
        if( bitnum != 16 && bitnum != 24 ) {
            return EINVAL;
        }
        tdm_exsplit_map = 0;
    }
    // force chnum to 2 for TDM split modes
    if( (op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO) || (op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO) ) {
        chnum = SSI_BUSIF_ADINR_CHNUM_TWO; //is this correct for 4XMONO?
        num_subchan = 4;
    } else if( op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN ) {
        // chnum can be different for each subchannel
        num_subchan = 4;
    } else if( op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN ) {
        // chnum can be different for each subchannel
        num_subchan = 8;
    }

    if( tdm_exsplit_map && ssiu_validate_tdm_exsplit_map( num_subchan, tdm_exsplit_map ) != EOK ) {
        return EINVAL;
    }

    for( i = 0; i < num_subchan; i++ ) {
        if( !SSIU_BUSIF(ssi_channel, i) ) {
            status = EFAULT;
            break;
        }
        if( (op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN) ||
            (op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN) ) {
            chnum = TDM_EXSPLIT_MAP_SUBCHN (tdm_exsplit_map, i);
        }
        SSIU_BUSIF(ssi_channel, i)->busif_adinr &= ~( SSI_BUSIF_ADINR_OTBL_MASK |
                                                      SSI_BUSIF_ADINR_CHNUM_MASK );
        SSIU_BUSIF(ssi_channel, i)->busif_adinr |= ( otbl | chnum );
    }
    return status;
}

static int ssiu_clear_ssi_busif( ssi_channel_t ssi_channel )
{
    // placeholder - not sure at this point if anything needs to be done here
    uint32_t min_subchan, max_subchan;
    uint32_t i;
    int status;

    status = rcar_ssi_subchan_get_supported_range( ssi_channel, &min_subchan, &max_subchan );

    if( status != EOK ) return status;

    for( i = min_subchan; i <= max_subchan; i++ ) {

        if( !SSIU_BUSIF( ssi_channel, i ) ) {
            status = EFAULT;
            break;
        }

        SSIU_BUSIF(ssi_channel, i)->busif_adinr &= ~( SSI_BUSIF_ADINR_OTBL_MASK |
                                                      SSI_BUSIF_ADINR_CHNUM_MASK );
    }

    return status;
}

/* Public SSIU level functions */

int ssiu_init( void )
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

int ssiu_indepchannel_common_setup
(
    uint32_t ssi_chan_mask,
    uint32_t busif_ssi_chan_mask,
    ssi_masterslave_mode_t masterslave_mode,
    ssi_start_mode_t start_mode
)
{
    int status;

    status = ssiu_set_mode_indepchannel ( ssi_chan_mask, busif_ssi_chan_mask, masterslave_mode, start_mode );

    if ( status != EOK ) {
        ado_error_fmt ( "failed setting the SSIU mode for indep channel ssi_chan_mask=0x%x, busif_ssi_chan_mask=0x%x, masterslave=%d",
            ssi_chan_mask, busif_ssi_chan_mask, masterslave_mode );
        return status;
    }

    /* clear the HDMI mode for all SSI in the SSI channel mask, before setting up individual independent channels */
    return ssiu_clear_hdmi_mode ( ssi_chan_mask,  SSI_HDMI_CHANNEL_NUM );
}

int ssiu_indepchannel_setup
(
    uint32_t ssi_channel,
    uint32_t sharedclk_ssi_channel,
    uint32_t is_transmit,
    uint32_t uses_busif,
    ssi_op_mode_t op_mode,
    ssi_masterslave_mode_t masterslave_mode,
    ssi_start_mode_t start_mode,
    uint32_t bitnum,
    uint32_t ssi_voices,
    ssi_hdmi_channel_t hdmi_channel,
    ssi_config_t* config,
    uint32_t tdm_exsplit_map
)
{
    int status;
    uint32_t is_master = 0;

    ado_debug (DB_LVL_DRIVER, "ssi_channel %d, sharedclk_ssi_channel %d, is_transmit %d, uses_busif %d, op_mode %d",
        ssi_channel, sharedclk_ssi_channel, is_transmit, uses_busif, op_mode);
    ado_debug (DB_LVL_DRIVER, "masterslave_mode %d, start_mode %d, bitnum %d, ssi_voices %d, hdmi_channel %d, tdm_exsplit_map 0x%x",
        masterslave_mode, start_mode, bitnum, ssi_voices, hdmi_channel, tdm_exsplit_map);

    /* first some validations */
    if ( (masterslave_mode == SSI_MS_MODE_ALL_SLAVE) && (hdmi_channel != SSI_HDMI_CHANNEL_NUM) ) {
        ado_error_fmt("HDMI output is only supported in master mode");
        return EINVAL;
    }

    if ( (sharedclk_ssi_channel != SSI_CHANNEL_NUM) && (sharedclk_ssi_channel != SSI_CHANNEL_0) &&
         (sharedclk_ssi_channel != SSI_CHANNEL_3) && (sharedclk_ssi_channel != SSI_CHANNEL_7) ) {
        ado_error_fmt("SSI%d is not supported as shared clock source", sharedclk_ssi_channel);
        return EINVAL;
    }

    if ( op_mode == SSI_OP_MODE_TDM ) {
        if ( !rcar_ssi_tdm_voices_supported ( ssi_voices ) ) {
            ado_error_fmt("%d voices not supported in TDM mode", ssi_voices);
            return EINVAL;
        }
    }

    if ( (op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN) || (op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN) ||
         ( (op_mode == SSI_OP_MODE_TDM) && (ssi_voices == 16) ) ) {
        if ( bitnum != 16 ) {
            ado_error_fmt("bitnum %d is not supported in op mode %d", bitnum, op_mode);
            return EINVAL;
        }
    }

    if ( (op_mode == SSI_OP_MODE_TDMEXT) || ( (op_mode == SSI_OP_MODE_TDM) && (ssi_voices <= 8) ) ||
         (op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO) || (op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO) ) {
        if ( bitnum != 16 && bitnum != 24 ) {
            ado_error_fmt("bitnum %d is not supported in op mode %d", bitnum, op_mode);
            return EINVAL;
        }
    }

    if ( !rcar_ssi_supported( ssi_channel ) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return EINVAL;
    }
    if ( op_mode == SSI_OP_MODE_TDM ) {
        if ( !rcar_ssi_tdm_supported( ssi_channel) ) {
            ado_error_fmt("SSI%d is not supported in TDM mode", ssi_channel);
            return EINVAL;
        }
    }
    else if ( op_mode == SSI_OP_MODE_TDMEXT ) {
        if ( !rcar_ssi_tdmext_supported( ssi_channel) ) {
            ado_error_fmt("SSI%d is not supported in TDM mode", ssi_channel);
            return EINVAL;
        }
    }
    else if ( (op_mode == SSI_OP_MODE_TDMSPLIT_4XMONO) || (op_mode == SSI_OP_MODE_TDMSPLIT_4XSTEREO) ) {
        if ( !rcar_ssi_tdmsplit_supported( ssi_channel ) ) {
            ado_error_fmt("SSI%d is not supported in TDM split mode", ssi_channel);
            return EINVAL;
        }
    }
    else if( (op_mode == SSI_OP_MODE_TDMEXSPLIT_8CHAN) || (op_mode == SSI_OP_MODE_TDMEXSPLIT_16CHAN) ) {
        if ( !rcar_ssi_tdmexsplit_supported( ssi_channel ) ) {
            ado_error_fmt("SSI%d is not supported in TDM exsplit mode", ssi_channel);
            return EINVAL;
        }
    }

    /* ssi_channel is configured as I2S/TDM clock master if masterslave_mode is not set to all slave and
     * we are in pin sharing mode and ssi_channel == sharedclk_ssi_channel or we are not in pin sharing mode
     * (in the latter case sharedclk_ssi_channel == SSI_CHANNEL_NUM
     */
    is_master = ((masterslave_mode == SSI_MS_MODE_MASTER_SLAVE) &&
        (ssi_channel == sharedclk_ssi_channel || sharedclk_ssi_channel == SSI_CHANNEL_NUM)) ? 1 : 0;

    do {
        status = ssi_set_control_and_mode ( ssi_channel, is_master, is_transmit, op_mode, ssi_voices, config );

        if ( status != EOK ) break;

        if ( hdmi_channel != SSI_HDMI_CHANNEL_NUM ) {
            uint32_t clk_ssi_channel = ( sharedclk_ssi_channel != SSI_CHANNEL_NUM ? sharedclk_ssi_channel : ssi_channel );
            status = ssiu_set_hdmi_mode ( clk_ssi_channel, SSI_CHANNEL_MASK(ssi_channel), hdmi_channel );
        }

        if ( status != EOK ) break;

        status = ssiu_set_ssi_mode ( ssi_channel, op_mode );

        if ( status != EOK ) break;

        if ( uses_busif ) {
            status = ssiu_set_ssi_busif ( ssi_channel, op_mode, ssi_voices, bitnum, tdm_exsplit_map );

            if ( status != EOK ) break;
        }
    } while ( false );

    if ( status != EOK ) {
        ssiu_indepchannel_cleanup ( ssi_channel );
    }

    return EOK;
}

void ssiu_indepchannel_common_cleanup ( uint32_t ssi_channel_mask )
{
    if ( ssi_channel_mask == SSI_CHANNEL_MASK_34 ) {
        (void) ssiu_stop ( SSI_SYNC_SSI34_START );
    }
    (void) ssiu_clear_mode_indepchannel ( ssi_channel_mask );
    (void) ssiu_clear_hdmi_mode ( ssi_channel_mask,  SSI_HDMI_CHANNEL_NUM );
}

void ssiu_indepchannel_cleanup
(
    uint32_t ssi_channel
)
{
    (void) ssi_clear_control_and_mode( ssi_channel );
    (void) ssiu_clear_ssi_mode( ssi_channel );

    (void) ssiu_busif_cleanup( ssi_channel );
    (void) ssiu_clear_ssi_busif( ssi_channel );
}

int ssiu_multichannel_setup
(
    uint32_t ssi_channel_mask,
    uint32_t is_transmit,
    ssi_masterslave_mode_t masterslave_mode,
    uint32_t bitnum,
    ssi_hdmi_channel_t hdmi_channel,
    ssi_config_t* config
)
{
    int status;
    uint32_t busif_voices;

    ado_debug (DB_LVL_DRIVER, "ssi_channel_mask 0x%x, is_transmit %d, masterslave_mode %d, bitnum %d, hdmi_channel %d",
        ssi_channel_mask, is_transmit, masterslave_mode, bitnum, hdmi_channel);

    if ( bitnum != 16 && bitnum != 24 ) {
        return EINVAL;
    }

    if ( ssi_channel_mask == SSI_CHANNEL_MASK_012 ) {
        busif_voices = 6;
    } else if ( ssi_channel_mask == SSI_CHANNEL_MASK_0129 ) {
        busif_voices = 8;
    } else {
        return EINVAL;
    }

    do {
        status = ssi_set_control_and_mode ( SSI_CHANNEL_0,
                                            masterslave_mode == SSI_MS_MODE_MASTER_SLAVE ? 1 : 0,
                                            is_transmit, SSI_OP_MODE_MULTICH, 2, config );
        if ( status != EOK ) break;

        status = ssi_set_control_and_mode ( SSI_CHANNEL_1, 0, is_transmit, SSI_OP_MODE_MULTICH, 2, config );

        if ( status != EOK ) break;

        status = ssi_set_control_and_mode ( SSI_CHANNEL_2, 0, is_transmit, SSI_OP_MODE_MULTICH, 2, config );

        if ( status != EOK ) break;

        if (ssi_channel_mask == SSI_CHANNEL_MASK_0129) {
            status = ssi_set_control_and_mode ( SSI_CHANNEL_9, 0, is_transmit, SSI_OP_MODE_MULTICH, 2, config );

            if( status != EOK ) break;
        }

        status = ssiu_set_mode_multichannel( ssi_channel_mask, masterslave_mode );

        if ( status != EOK ) break;

        status = ssiu_set_ssi_mode ( SSI_CHANNEL_0, SSI_OP_MODE_MULTICH );

        if ( status != EOK ) break;

        status = ssiu_set_ssi_mode ( SSI_CHANNEL_1, SSI_OP_MODE_MULTICH );

        if ( status != EOK ) break;

        status = ssiu_set_ssi_mode ( SSI_CHANNEL_2, SSI_OP_MODE_MULTICH );

        if ( status != EOK ) break;

        if ( ssi_channel_mask == SSI_CHANNEL_MASK_0129 ) {
            status = ssiu_set_ssi_mode ( SSI_CHANNEL_9, SSI_OP_MODE_MULTICH );

            if( status != EOK ) break;
        }

        status = ssiu_set_ssi_busif ( SSI_CHANNEL_0, SSI_OP_MODE_MULTICH, busif_voices, bitnum, 0 );

        if( status != EOK ) break;

        if( hdmi_channel != SSI_HDMI_CHANNEL_NUM ) {
            status = ssiu_set_hdmi_mode ( SSI_CHANNEL_0, ssi_channel_mask, hdmi_channel );
        } else {
            (void)ssiu_clear_hdmi_mode ( ssi_channel_mask, SSI_HDMI_CHANNEL_NUM );
        }
    } while( false );

    if( status != EOK ) {
        (void)ssiu_multichannel_cleanup ( ssi_channel_mask );
    }

    return status;
}

void ssiu_multichannel_cleanup ( uint32_t ssi_channel_mask )
{
    (void)ssiu_stop( SSI_SYNC_SSI0129_START );

    (void)ssi_clear_control_and_mode( SSI_CHANNEL_0 );
    (void)ssi_clear_control_and_mode( SSI_CHANNEL_1 );
    (void)ssi_clear_control_and_mode( SSI_CHANNEL_2 );
    if (ssi_channel_mask == SSI_CHANNEL_MASK_0129) {
        ssi_clear_control_and_mode( SSI_CHANNEL_9 );
    }

    (void)ssiu_clear_mode_multichannel(SSI_CHANNEL_MASK_012);

    (void)ssiu_clear_ssi_mode( SSI_CHANNEL_0 );
    (void)ssiu_clear_ssi_mode( SSI_CHANNEL_1 );
    (void)ssiu_clear_ssi_mode( SSI_CHANNEL_2 );
    if (ssi_channel_mask == SSI_CHANNEL_MASK_0129) {
        (void)ssiu_clear_ssi_mode( SSI_CHANNEL_9 );
    }

    (void)ssiu_busif_cleanup( SSI_CHANNEL_0 );
    (void)ssiu_clear_ssi_busif( SSI_CHANNEL_0 );

    (void)ssiu_clear_hdmi_mode( SSI_CHANNEL_MASK_012, SSI_HDMI_CHANNEL_NUM );
}

int ssiu_start( ssi_start_mode_t start_mode )
{
    if( !SSIU_COMMON ) {
        return EFAULT;
    }

    ado_mutex_lock(&ssiu_lock);

    if( start_mode == SSI_SYNC_SSI0129_START ) {
        SSIU_COMMON->control &= ~SSI_CONTROL_SSI0129_MASK;
        SSIU_COMMON->control |= SSI_CONTROL_SSI0129_ENABLE;
    } else if ( start_mode == SSI_SYNC_SSI34_START ) {
        SSIU_COMMON->control &= ~SSI_CONTROL_SSI34_MASK;
        SSIU_COMMON->control |= SSI_CONTROL_SSI34_ENABLE;
    }

    ado_mutex_unlock(&ssiu_lock);

    return EOK;
}

int ssiu_stop( ssi_start_mode_t start_mode )
{
    if( !SSIU_COMMON ) {
        return EFAULT;
    }

    ado_mutex_lock(&ssiu_lock);

    if( start_mode == SSI_SYNC_SSI0129_START ) {
        SSIU_COMMON->control &= ~SSI_CONTROL_SSI0129_MASK;
    } else if ( start_mode == SSI_SYNC_SSI34_START ) {
        SSIU_COMMON->control &= ~SSI_CONTROL_SSI34_MASK;
    }

    ado_mutex_unlock(&ssiu_lock);

    return EOK;
}

int ssiu_busif_start( ssi_channel_t ssi_channel, ssi_sub_channel_t ssi_sub_channel )
{
    if( !rcar_ssi_supported(ssi_channel) ) {
        return EINVAL;
    }
    if( !rcar_ssi_subchan_supported(ssi_channel, ssi_sub_channel) ) {
        return EINVAL;
    }
    if( !SSIU_SSI( ssi_channel ) ) {
        return EFAULT;
    }

    SSIU_SSI( ssi_channel )->control |= SSI_CONTROL_START_START(ssi_sub_channel);

    return EOK;
}

int ssiu_busif_stop( ssi_channel_t ssi_channel, ssi_sub_channel_t ssi_sub_channel )
{
    if( !rcar_ssi_supported(ssi_channel) ) {
        return EINVAL;
    }
    if( !rcar_ssi_subchan_supported(ssi_channel, ssi_sub_channel) ) {
        return EINVAL;
    }
    if( !SSIU_SSI( ssi_channel ) ) {
        return EFAULT;
    }

    SSIU_SSI( ssi_channel )->control &= ~SSI_CONTROL_START_MASK(ssi_sub_channel);

    return EOK;
}

int ssiu_busif_cleanup( ssi_channel_t ssi_channel )
{
    if( !rcar_ssi_supported(ssi_channel) ) {
        return EINVAL;
    }
    if( !SSIU_SSI( ssi_channel ) ) {
        return EFAULT;
    }

    SSIU_SSI( ssi_channel )->control = 0;

    return EOK;
}

int ssi_set_divisor(ssi_channel_t ssi_channel, uint32_t divisor)
{
    int status;
    uint32_t cr;

    if( !rcar_ssi_supported(ssi_channel) ) {
        return EINVAL;
    }

    if( !SSI(ssi_channel) ) {
        return EFAULT;
    }

    cr = SSI(ssi_channel)->cr;

    status = ssi_set_clockdiv_bits( &cr, divisor );
    cr |= SSICR_FORCE_FIXED;

    if( status == EOK ) {
        ado_debug (DB_LVL_DRIVER, "Setting SSI%d divisor to %u", ssi_channel, divisor);

        SSI(ssi_channel)->cr = cr;
    }

    return status;
}

/* SSIU register dump functions */

void ssiu_common_register_dump()
{
    if( !SSIU_COMMON ) {
        ado_error_fmt("SSIU memory not mapped");
        return;
    }
    ado_debug( DB_LVL_DRIVER, "SSIU COMMON reg dump: MODE[0:3]=%x %x %x %x CONTROL=%x",
               SSIU_COMMON->mode0, SSIU_COMMON->mode1, SSIU_COMMON->mode2,
               SSIU_COMMON->mode3, SSIU_COMMON->control );
    ado_debug( DB_LVL_DRIVER, "SSIU COMMON reg dump: SYSTEM_STATUS[0:3]=%x %x %x %x",
               SSIU_COMMON->system_status0, SSIU_COMMON->system_status1,
               SSIU_COMMON->system_status2, SSIU_COMMON->system_status3 );
    ado_debug( DB_LVL_DRIVER, "SSIU COMMON reg dump: SYSTEM_INT_ENABLE[0:3]=%x %x %x %x",
               SSIU_COMMON->system_int_enable0, SSIU_COMMON->system_int_enable1,
               SSIU_COMMON->system_int_enable2, SSIU_COMMON->system_int_enable3 );
    if( SSIU_COMMON_EXT ) {
        ado_debug( DB_LVL_DRIVER, "SSIU COMMON EXT reg dump: CONTROL2=%x HDMI0_SEL=%x HDMI1_SEL=%x",
                   SSIU_COMMON_EXT->control2, SSIU_COMMON_EXT->hdmi0_sel, SSIU_COMMON_EXT->hdmi1_sel );
        ado_debug( DB_LVL_DRIVER, "SSIU COMMON EXT reg dump: SYSTEM_STATUS[4:7]=%x %x %x %x",
                   SSIU_COMMON_EXT->system_status4, SSIU_COMMON_EXT->system_status5,
                   SSIU_COMMON_EXT->system_status6, SSIU_COMMON_EXT->system_status7 );
        ado_debug( DB_LVL_DRIVER, "SSIU COMMON EXT reg dump: SYSTEM_INT_ENABLE[4:7]=%x %x %x %x",
                   SSIU_COMMON_EXT->system_int_enable4, SSIU_COMMON_EXT->system_int_enable5,
                   SSIU_COMMON_EXT->system_int_enable6, SSIU_COMMON_EXT->system_int_enable7 );
    }
}

void ssiu_ssi_register_dump( ssi_channel_t ssi_channel )
{
    uint32_t min_subchannel = 0;
    uint32_t max_subchannel = 0;
    uint32_t i;
    if( !rcar_ssi_supported(ssi_channel) ) {
        ado_error_fmt("SSI%d is not supported", ssi_channel);
        return;
    }
    if( SSI(ssi_channel) ) {
        ado_debug( DB_LVL_DRIVER, "SSI%d reg dump: CR=%x SR=%x WSR=%x FMR=%x FSR=%x",
                   ssi_channel, SSI(ssi_channel)->cr, SSI(ssi_channel)->sr,
                   SSI(ssi_channel)->wsr, SSI(ssi_channel)->fmr, SSI(ssi_channel)->fsr );
    } else {
        ado_error_fmt("SSI memory not mapped");
    }
    if( SSI_EXT(ssi_channel) ) {
        ado_debug( DB_LVL_DRIVER, "SSI%d EXT reg dump: CRE=%x", ssi_channel, SSI_EXT(ssi_channel)->cre );
    }
    if( SSIU_SSI(ssi_channel) ) {
        ado_debug( DB_LVL_DRIVER, "SSIU SSI%d reg dump: MODE=%x CONTROL=%x STATUS=%x, INT_ENABLE_MAIN=%x",
                   ssi_channel, SSIU_SSI(ssi_channel)->mode, SSIU_SSI(ssi_channel)->control,
                   SSIU_SSI(ssi_channel)->status, SSIU_SSI(ssi_channel)->int_enable_main );
    } else {
        ado_error_fmt("SSIU memory not mapped");
    }
    if( SSIU_SSI2(ssi_channel) ) {
        ado_debug( DB_LVL_DRIVER, "SSIU SSI%d EXT reg dump: MODE2=%x STATUS2=%x INT_ENABLE_MAIN2=%x",
                   ssi_channel, SSIU_SSI2(ssi_channel)->mode2, SSIU_SSI2(ssi_channel)->status2,
                   SSIU_SSI2(ssi_channel)->int_enable_main2 );
    }

    rcar_ssi_subchan_get_supported_range( ssi_channel, &min_subchannel, &max_subchannel );

    for( i = min_subchannel; i <= max_subchannel; i++ ) {
        if( SSIU_BUSIF(ssi_channel, i) ) {
            ado_debug( DB_LVL_DRIVER, "SSIU BUSIF%d-%d reg dump: BUSIF_MODE=%x BUSIF_ADINR=%x BUSIF_DALIGN=%x",
                       ssi_channel, i,
                       SSIU_BUSIF(ssi_channel, i)->busif_mode,
                       SSIU_BUSIF(ssi_channel, i)->busif_adinr,
                       SSIU_BUSIF(ssi_channel, i)->busif_dalign );
        } else {
            ado_error_fmt("SSIU memory not mapped");
        }
    }

    if( SSIU_BUSIF2(ssi_channel) ) {
        ado_debug( DB_LVL_DRIVER, "SSIU BUSIF%d EXT reg dump: BUSIF_DALIGN2=%x",
                   ssi_channel, SSIU_BUSIF2(ssi_channel)->busif_dalign2 );
    }
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/ssiu.c $ $Rev: 911285 $")
#endif

