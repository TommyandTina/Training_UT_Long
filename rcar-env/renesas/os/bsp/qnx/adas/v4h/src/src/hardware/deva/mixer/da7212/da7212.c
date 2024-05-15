/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
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
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <sys/mman.h>
#include <audio_driver.h>
#include "da7212.h"
#include "../i2c2.h"

/* da7212 registers */
#define DA7212_SYSTEM_ACTIVE                          0xFD
#define DA7212_RESET                                  0x1D
#define DA7212_IO_CTRL                                0x91
#define DA7212_GAIN_RAMP_CTRL                         0x92
#define DA7212_LDO_CTRL                               0x90

#define DA7212_SR                                     0x22
 #define DA7212_SR_MASK                                (0xF << 0)
 #define DA7212_SR_8K                                  (0x1 << 0)
 #define DA7212_SR_11025K                              (0x2 << 0)
 #define DA7212_SR_12K                                 (0x3 << 0)
 #define DA7212_SR_16K                                 (0x5 << 0)
 #define DA7212_SR_22050K                              (0x6 << 0)
 #define DA7212_SR_24K                                 (0x7 << 0)
 #define DA7212_SR_32K                                 (0x9 << 0)
 #define DA7212_SR_441K                                (0xA << 0)
 #define DA7212_SR_48K                                 (0xB << 0)
 #define DA7212_SR_882K                                (0xE << 0)
 #define DA7212_SR_96K                                 (0xF << 0)
#define DA7212_MIXED_SAMPLE_MODE                      0x84
#define DA7212_PC_COUNT                               0x94
#define DA7212_PLL_FRAC_TOP                           0x24
#define DA7212_PLL_FRAC_BOT                           0x25
#define DA7212_PLL_INTEGER                            0x26
#define DA7212_PLL_STATUS                             0x03
 #define DA7219_PLL_STATUS_SRM_LOC                     (0x1 << 1)
#define DA7212_PLL_CTRL                               0x27
 #define DA7212_PLL_CTRL_INDIV_2_TO_10_MHZ             (0x0 << 2)
 #define DA7212_PLL_CTRL_INDIV_10_TO_20_MHZ            (0x1 << 2)
 #define DA7212_PLL_CTRL_INDIV_20_TO_40_MHZ            (0x2 << 2)
 #define DA7212_PLL_CTRL_INDIV_40_TO_80_MHZ            (0x3 << 2)
 #define DA7212_PLL_CTRL_INDIV_MASK                    (0x3 << 2)
 #define DA7212_PLL_CTRL_MCLK_SQR_EN                   (0x1 << 4)
 #define DA7212_PLL_CTRL_32K_MODE                      (0x1 << 5)
 #define DA7212_PLL_CTRL_SRM_EN                        (0x1 << 6)
 #define DA7212_PLL_CTRL_EN                            (0x1 << 7)
 #define DA7212_PLL_CTRL_MODE_MASK                     (0x7 << 5)

#define DA7212_MIC_1_CTRL                             0x63
 #define DA7212_MIC_1_CTRL_AMP_IN_SEL                  (0x00 << 1)
 #define DA7212_MUTE_EN                                (0x1 << 6)
 #define DA7212_MIC_1_CTRL_AMP_EN                      (0x1 << 7)
#define DA7212_MIC_2_CTRL                             0x64
 #define DA7212_MIC_2_CTRL_AMP_IN_SEL                  (0x00 << 1)
 #define DA7212_MIC_2_CTRL_AMP_EN                      (0x1 << 7)
#define DA7212_REFERENCES                             0x23
#define DA7212_MICBIAS_CTRL                           0x62
 #define DA7212_MICBIAS_CTRL_MICBIAS1_LEVEL_3V         (0x11 << 0)
 #define DA7212_MICBIAS_CTRL_MICBIAS1_EN               (0x1 << 3)
 #define DA7212_MICBIAS_CTRL_MICBIAS2_LEVEL_3V         (0x11 << 2)
 #define DA7212_MICBIAS_CTRL_MICBIAS2_EN               (0x1 << 7)
#define DA7212_MIC_1_GAIN                             0x39
 #define DA7212_MIC_GAIN_MASK                          0x07
#define DA7212_MIC_2_GAIN                             0x3A
#define DA7212_MIC_CONFIG                             0x93

#define DA7212_ADC_L_CTRL                             0x67
 #define DA7212_ADC_L_CTRL_RAMP_EN                     (0x1 << 5)
 #define DA7212_ADC_L_CTRL_EN                          (0x1 << 7)
#define DA7212_ADC_R_CTRL                             0x68
 #define DA7212_ADC_R_CTRL_EN                          (0x1 << 5)
 #define DA7212_ADC_R_CTRL_RAMP_EN                     (0x1 << 7)
#define DA7212_ADC_FILTERS1                           0x38
#define DA7212_ADC_L_GAIN                             0x36
#define DA7212_ADC_R_GAIN                             0x37

#define DA7212_ALC_NOISE                              0x9C
#define DA7212_ALC_TARGET_MIN                         0x9D
#define DA7212_ALC_TARGET_MAX                         0x9E
#define DA7212_ALC_GAIN_LIMITS                        0x9F
#define DA7212_ALC_ANA_GAIN_LIMITS                    0xA0
#define DA7212_ALC_ANTICLIP_CTRL                      0xA1
#define DA7212_ALC_ANTICLIP_LEVEL                     0xA2
#define DA7212_ALC_CTRL1                              0x2B

#define DA7212_DIG_ROUTING_DAI                        0x21
 #define DA7212_DIG_ROUTING_DAI_DAI_INPUT_SRC          0x32
 #define DA7212_DIG_ROUTING_DAI_ADC_SRC                0x10
#define DA7212_DAI_CLK_MODE                           0x28
 #define DA7212_DAI_CLK_MODE_BCLKS_PER_WCLK_32         (0x0 << 0)
 #define DA7212_DAI_CLK_MODE_BCLKS_PER_WCLK_64         (0x1 << 0)
 #define DA7212_DAI_CLK_MODE_BCLKS_PER_WCLK_128        (0x2 << 0)
 #define DA7212_DAI_CLK_MODE_BCLKS_PER_WCLK_256        (0x3 << 0)
 #define DA7212_DAI_CLK_MODE_BCLKS_PER_WCLK_MASK       (0x3 << 0)
 #define DA7212_DAI_CLK_MODE_CLK_POL_INV               (0x1 << 2)
 #define DA7212_DAI_CLK_MODE_CLK_POL_MASK              (0x1 << 2)
 #define DA7212_DAI_CLK_MODE_WCLK_POL_INV              (0x1 << 3)
 #define DA7212_DAI_CLK_MODE_WCLK_POL_MASK             (0x1 << 3)
 #define DA7212_DAI_CLK_MODE_CLK_EN_MASK               (0x1 << 7)
#define DA7212_DAI_CTRL                               0x29
 #define DA7212_DAI_CTRL_FORMAT_I2S_MODE               (0x0 << 0)
 #define DA7212_DAI_CTRL_FORMAT_LEFT_J                 (0x1 << 0)
 #define DA7212_DAI_CTRL_FORMAT_RIGHT_J                (0x2 << 0)
 #define DA7212_DAI_CTRL_FORMAT_DSP                    (0x3 << 0)
 #define DA7212_DAI_CTRL_FORMAT_MASK                   (0x3 << 0)
 #define DA7212_DAI_CTRL_WORD_LENGTH_S16_LE            (0x0 << 2)
 #define DA7212_DAI_CTRL_WORD_LENGTH_S20_LE            (0x1 << 2)
 #define DA7212_DAI_CTRL_WORD_LENGTH_S24_LE            (0x2 << 2)
 #define DA7212_DAI_CTRL_WORD_LENGTH_S32_LE            (0x3 << 2)
 #define DA7212_DAI_CTRL_WORD_LENGTH_MASK              (0x3 << 2)

#define DA7212_DIG_ROUTING_DAC                        0x2A
 #define DA7212_DIG_ROUTING_DAC_ADC_OUTPUT_SRC         0x10
 #define DA7212_DIG_ROUTING_DAC_DAC_MONO_SRC          0x32
#define DA7212_DAC_L_CTRL                             0x69
#define DA7212_DAC_R_CTRL                             0x6A
#define DA7212_DAC_L_GAIN                             0x45
 #define DA7212_DAC_L_GAIN_MASK                        0x7F
#define DA7212_DAC_L_GAIN_STATUS                      0x0C
#define DA7212_DAC_R_GAIN                             0x46
 #define DA7212_DAC_R_GAIN_MASK                        0x7F
#define DA7212_DAC_R_GAIN_STATUS                      0x0D
#define DA7212_DAC_NG_SETUP_TIME                      0xAF
#define DA7212_DAC_NG_CTRL                            0xB2
#define DA7212_DAC_NG_OFF_THRESHOLD                   0xB0
#define DA7212_DAC_NG_ON_THRESHOLD                    0xB1
#define DA7212_DAC_FILTERS2                           0x41
#define DA7212_DAC_FILTERS3                           0x42
#define DA7212_DAC_FILTERS4                           0x43
#define DA7212_DAC_FILTERS1                           0x44
#define DA7212_HP_L_CTRL                              0x6B
 #define DA7212_HP_AMP_OE                              (0x1 << 3)
 #define DA7212_HP_AMP_EN                              (0x1 << 7)
#define DA7212_HP_R_CTRL                              0x6C

#define DA7212_HP_L_GAIN                              0x48
 #define DA7212_HP_GAIN_MASK                           0x3F
#define DA7212_HP_L_GAIN_STATUS                       0x0E
#define DA7212_HP_R_GAIN                              0x49
#define DA7212_HP_R_GAIN_STATUS                       0x0F
#define DA7212_CP_CTRL                                0x47
#define DA7212_CP_VOL_THRESHOLD1                      0x95
#define DA7212_CP_DELAY                               0x96

#define DA7212_MIXIN_L_SELECT                         0x32
#define DA7212_MIXIN_R_SELECT                         0x33
 #define DA7212_MIXIN_L_SELECT_MIC1                    (0x1 << 1)
 #define DA7212_MIXIN_R_SELECT_MIC1                    (0x1 << 2)
#define DA7212_MIXIN_L_GAIN                           0x34
#define DA7212_MIXIN_R_GAIN                           0x35
 #define DA7212_MIXIN_GAIN_MASK                        0x0F
#define DA7212_MIXIN_L_CTRL                           0x65
#define DA7212_MIXIN_R_CTRL                           0x66
#define DA7212_MIXOUT_L_SELECT                        0x4B
#define DA7212_MIXOUT_R_SELECT                        0x4C
 #define DA7212_MIXOUT_SELECT_DAC                      (0x1 << 3)
#define DA7212_MIXOUT_L_CTRL                          0x6E
#define DA7212_MIXOUT_R_CTRL                          0x6F

#define DA7212_SYSTEM_MODES_INPUT                     0x50
#define DA7212_SYSTEM_MODES_OUTPUT                    0x51
#define DA7212_SYSTEM_STATUS                          0xE0

/* dec2hex: get integer-part of float number */
int dec2hex ( float fnum )
{
	int inum = (int)fnum;
	return inum;
}

/* dec2hex_frac_4num:  get 4 most significant digits (in hexa format)
   of fractional part of float number */
int dec2hex_frac_4num ( float fnum )
{
	int i, inum, ret = 0;

	inum = dec2hex(fnum);
	for (i = 3; i >= 0; i--) {
		fnum -= inum;  //get fraction part
		fnum *= 16;
		inum = dec2hex ( fnum );
		ret |= inum << (i*4);
	}
	return ret;
}

static int da7212_open_i2c_fd ( da7212_context_t *da7212 )
{
    int status = EOK;

    if ( da7212 ) {
        if ( da7212->params.codec_open ) {
            status = da7212->params.codec_open( da7212->params.hw_context );
        } else {
            char i2c_devname[20];
            sprintf ( i2c_devname, "/dev/i2c%d", da7212->params.i2c_dev );
            status = open_i2c_fd ( i2c_devname );
        }
        if ( status != EOK ) {
            ado_error_fmt ( "da7212 codec open failed" );
        }
    } else {
        ado_error_fmt ( "da7212 codec is not init" );
        status = EINVAL;
    }
    return status;
}

static int da7212_close_i2c_fd ( da7212_context_t *da7212)
{
    int status = EOK;

    if ( da7212 ) {
        if ( da7212->params.codec_close ) {
            da7212->params.codec_close( da7212->params.hw_context );
        } else {
            close_i2c_fd();
        }
    } else {
        ado_error_fmt ( "da7212 codec is not init" );
        status = EINVAL;
    }

    return status;
}

static int da7212_i2c_read ( da7212_context_t *da7212, uint16_t reg, uint32_t *val, int index )
{
    int status = EOK;
    uint32_t reg_val;

    if ( da7212 && val ) {
        if ( da7212->params.codec_read ) {
            status = da7212->params.codec_read( da7212->params.hw_context, reg, &reg_val, index );
            if ( status == EOK ) {
                *val = reg_val;
            }
        } else {
            status = i2c_read ( da7212->params.i2c_addr, reg, val, index );
        }
        if ( status != EOK ) {
            ado_error_fmt ( "DA7212 i2c read failed" );
        }
    } else {
        ado_error_fmt ( "DA7212 codec is not init or val is NULL ");
        status = EINVAL;
    }

    return status;
}

static int da7212_i2c_write ( da7212_context_t *da7212, uint16_t reg, uint16_t val, int index )
{
    int status = EOK;

    if ( da7212 ) {
        if ( da7212->params.codec_write ) {
            status = da7212->params.codec_write( da7212->params.hw_context, reg, val, index );
        } else {
            status = i2c_write ( da7212->params.i2c_addr, reg, val, index );
        }
        if ( status != EOK ) {
            ado_error_fmt ( "da7212 i2c write failed" );
        }
    } else {
        ado_error_fmt ( "da7212 codec is not init" );
        status = EINVAL;
    }

    return status;
}

static int da7212_bit_set ( da7212_context_t *da7212, uint16_t reg, uint32_t val )
{
    uint32_t data;

    if ( da7212_i2c_read ( da7212, reg, &data, 0 ) != EOK )
        return EINVAL;

    data &= ~val;
    data |= val;
    return da7212_i2c_write ( da7212, reg, data, 0 );
}

static int da7212_bit_clr ( da7212_context_t *da7212, uint16_t reg, uint32_t val )
{
    uint32_t data;

    if ( da7212_i2c_read ( da7212, reg, &data, 0 ) != EOK )
        return EINVAL;
    data &= ~val;
    return da7212_i2c_write ( da7212, reg, data, 0 );
}

// static int da7212_count_bits ( uint8_t bitmask )
// {
//     int count = 0;
//     while ( bitmask ) {
//         bitmask &= (bitmask - 1);
//         count++;
//     }
//     return count;
// }

int da7212_mic_1_input_vol_get ( da7212_context_t* da7212, uint32_t *vol )
{
    int status = EOK;
    uint32_t data;

    if ( vol == NULL ) {
        return EINVAL;
    }

    status = da7212_i2c_read ( da7212, DA7212_MIC_1_GAIN, &data, 0 );
    if ( status != EOK ) {
        return status;
    }

    *vol = data;

    return EOK;
}

int da7212_mic_1_input_vol_set ( da7212_context_t* da7212, uint32_t vol )
{
    int status = EOK;

    status = da7212_i2c_write ( da7212, DA7212_MIC_1_GAIN, (vol & DA7212_MIC_GAIN_MASK), 0 );
    if ( status != EOK ) {
        return status;
    }

    return EOK;
}

int da7212_mic_1_input_mute_get ( da7212_context_t* da7212, uint32_t* mute )
{
    int status = EOK;
    uint32_t data;

    if ( mute ) {
        status = da7212_i2c_read ( da7212, DA7212_MIC_1_CTRL, &data, 0 );
        if ( status == EOK ) {
            *mute = ( (data & DA7212_MUTE_EN) ? 1 : 0 );
        }
    }

    return status;
}

int da7212_mic_1_input_mute_set ( da7212_context_t* da7212, uint32_t mute )
{
    int status = EOK;

    if ( mute ) {
        status = da7212_bit_set ( da7212, DA7212_MIC_1_CTRL, DA7212_MUTE_EN );
    } else {
        status = da7212_bit_clr ( da7212, DA7212_MIC_1_CTRL, DA7212_MUTE_EN );
    }

    return status;
}

int da7212_mic_2_input_vol_get ( da7212_context_t* da7212, uint32_t *vol )
{
    int status = EOK;
    uint32_t data;

    if ( vol == NULL ) {
        return EINVAL;
    }

    status = da7212_i2c_read ( da7212, DA7212_MIC_2_GAIN, &data, 0 );
    if ( status != EOK ) {
        return status;
    }

    *vol = data;

    return EOK;
}

int da7212_mic_2_input_vol_set ( da7212_context_t* da7212, uint32_t vol )
{
    int status = EOK;

    status = da7212_i2c_write ( da7212, DA7212_MIC_2_GAIN, (vol & DA7212_MIC_GAIN_MASK), 0 );
    if ( status != EOK ) {
        return status;
    }

    return EOK;
}

int da7212_mic_2_input_mute_get ( da7212_context_t* da7212, uint32_t* mute )
{
    int status = EOK;
    uint32_t data;

    if ( mute ) {
        status = da7212_i2c_read ( da7212, DA7212_MIC_2_CTRL, &data, 0 );
        if ( status == EOK ) {
            *mute = ( (data & DA7212_MUTE_EN) ? 1 : 0 );
        }
    }

    return status;
}

int da7212_mic_2_input_mute_set ( da7212_context_t* da7212, uint32_t mute )
{
    int status = EOK;

    if( mute ) {
        status = da7212_bit_set ( da7212, DA7212_MIC_2_CTRL, DA7212_MUTE_EN );
    } else {
        status = da7212_bit_clr ( da7212, DA7212_MIC_2_CTRL, DA7212_MUTE_EN );
    }

    return status;
}

int da7212_mixin_input_vol_get ( da7212_context_t* da7212, uint32_t *vol )
{
    int status = EOK;
    uint32_t data;

    if ( vol == NULL ) {
        return EINVAL;
    }

    status = da7212_i2c_read ( da7212, DA7212_MIXIN_L_GAIN, &data, 0 );
    if ( status != EOK ) {
        return status;
    }
    vol[LEFT_CHAN] = data;

    status = da7212_i2c_read ( da7212, DA7212_MIXIN_R_GAIN, &data, 0 );
    if ( status != EOK ) {
        return status;
    }
    vol[RIGHT_CHAN] = data;

    return EOK;
}

int da7212_mixin_input_vol_set ( da7212_context_t* da7212, int channel, uint32_t vol )
{
    int status = EOK;

    if ( channel == LEFT_CHAN ) {
        status = da7212_i2c_write ( da7212, DA7212_MIXIN_L_GAIN, (vol & DA7212_MIXIN_GAIN_MASK), 0 );
        if ( status != EOK ) {
            return status;
        }
    } else if ( channel == RIGHT_CHAN ) {
        status = da7212_i2c_write ( da7212, DA7212_MIXIN_R_GAIN, (vol & DA7212_MIXIN_GAIN_MASK), 0 );
        if ( status != EOK ) {
            return status;
        }
    }

    return EOK;
}

int da7212_mixin_input_mute_get ( da7212_context_t* da7212, uint32_t *mute )
{
    int status = EOK;
    uint32_t data;

    if ( mute == NULL ) {
        return EINVAL;
    }

    status = da7212_i2c_read ( da7212, DA7212_MIXIN_L_CTRL, &data, 0 );
    if ( status == EOK ) {
        mute[LEFT_CHAN] = ( (data & DA7212_MUTE_EN) ? 1 : 0 );
    }

    status = da7212_i2c_read ( da7212, DA7212_MIXIN_R_CTRL, &data, 0 );
    if ( status == EOK ) {
        mute[RIGHT_CHAN] = ( (data & DA7212_MUTE_EN) ? 1 : 0 );
    }

    return status;
}

int da7212_mixin_input_mute_set(da7212_context_t* da7212, int channel, uint32_t mute)
{
    int status = EOK;

    if( mute ) {
        if ( channel == LEFT_CHAN ) {
            status = da7212_bit_set ( da7212, DA7212_MIXIN_L_CTRL, DA7212_MUTE_EN );
        } else if ( channel == RIGHT_CHAN ) {
            status = da7212_bit_set ( da7212, DA7212_MIXIN_R_CTRL, DA7212_MUTE_EN );
        }
    } else {
        if ( channel == LEFT_CHAN ) {
            status = da7212_bit_clr ( da7212, DA7212_MIXIN_L_CTRL, DA7212_MUTE_EN );
        } else if ( channel == RIGHT_CHAN ) {
            status = da7212_bit_clr ( da7212, DA7212_MIXIN_R_CTRL, DA7212_MUTE_EN );
        }
    }

    return status;
}

int da7212_hp_output_vol_get ( da7212_context_t* da7212, uint32_t *vol)
{
    int status = EOK;
    uint32_t data;

    if ( vol == NULL ) {
        return EINVAL;
    }

    status = da7212_i2c_read ( da7212, DA7212_HP_L_GAIN, &data, 0 );
    if ( status != EOK ) {
        return status;
    }
    vol[LEFT_CHAN] = data & DA7212_HP_GAIN_MASK;

    status = da7212_i2c_read ( da7212, DA7212_HP_R_GAIN, &data, 0 );
    if ( status != EOK ) {
        return status;
    }
    vol[RIGHT_CHAN] = data & DA7212_HP_GAIN_MASK;

    return EOK;
}

int da7212_hp_output_vol_set ( da7212_context_t* da7212, int channel, uint32_t vol )
{
    int status = EOK;

    if ( channel == LEFT_CHAN ) {
        status = da7212_i2c_write ( da7212, DA7212_HP_L_GAIN, (vol & DA7212_HP_GAIN_MASK), 0 );
        if ( status != EOK ) {
            return status;
        }
    } else if(channel == RIGHT_CHAN) {
        status = da7212_i2c_write ( da7212, DA7212_HP_R_GAIN, (vol & DA7212_HP_GAIN_MASK), 0 );
        if ( status != EOK ) {
            return status;
        }
    }

    return EOK;
}

int da7212_hp_output_mute_get ( da7212_context_t* da7212, uint32_t *mute )
{
    int status = EOK;
    uint32_t data;

    if ( mute == NULL ) {
        return EINVAL;
    }

    status = da7212_i2c_read ( da7212, DA7212_HP_L_CTRL, &data, 0 );
    if ( status == EOK ) {
        mute[LEFT_CHAN] = ( (data & DA7212_MUTE_EN) ? 1 : 0 );
    }

    status = da7212_i2c_read ( da7212, DA7212_HP_R_CTRL, &data, 0 );
    if ( status == EOK ) {
        mute[RIGHT_CHAN] = ( (data & DA7212_MUTE_EN) ? 1 : 0 );
    }

    return status;
}

int da7212_hp_output_mute_set ( da7212_context_t* da7212, int channel, uint32_t mute )
{
    int status = EOK;

    if( mute ) {
        if ( channel == LEFT_CHAN ) {
            status = da7212_bit_set ( da7212, DA7212_HP_L_CTRL, DA7212_MUTE_EN );
        } else if ( channel == RIGHT_CHAN ) {
            status = da7212_bit_set ( da7212, DA7212_HP_R_CTRL, DA7212_MUTE_EN );
        }
    } else {
        if ( channel == LEFT_CHAN ) {
            status = da7212_bit_clr ( da7212, DA7212_HP_L_CTRL, DA7212_MUTE_EN );
        } else if ( channel == RIGHT_CHAN ) {
            status = da7212_bit_clr ( da7212, DA7212_HP_R_CTRL, DA7212_MUTE_EN );
        }
    }

    return status;
}

int da7212_dig_routing_dai_get ( da7212_context_t* da7212, uint32_t* val, int mode )
{
    int status = EOK;
    uint32_t data;

    if ( val ) {
        status = da7212_i2c_read ( da7212, DA7212_DIG_ROUTING_DAI, &data, 0 );
        if ( status == EOK ) {
            if ( mode ) {
                *val = ( (data != DA7212_DIG_ROUTING_DAI_ADC_SRC) ? 1 : 0 );
            } else {
                *val = ( (data != DA7212_DIG_ROUTING_DAI_DAI_INPUT_SRC) ? 1 : 0 );
            }
        }
    }

    return status;
}

int da7212_dig_routing_dai_set ( da7212_context_t* da7212, uint32_t mode )
{
    int status = EOK;

    if ( mode )
        status = da7212_i2c_write ( da7212, DA7212_DIG_ROUTING_DAI, DA7212_DIG_ROUTING_DAI_ADC_SRC, 0 );
    else
        status = da7212_i2c_write ( da7212, DA7212_DIG_ROUTING_DAI, DA7212_DIG_ROUTING_DAI_DAI_INPUT_SRC, 0 );

    return status;
}

int da7212_dig_routing_dac_get ( da7212_context_t* da7212, uint32_t* val, int mode )
{
    int status = EOK;
    uint32_t data;

    if ( val ) {
        status = da7212_i2c_read ( da7212, DA7212_DIG_ROUTING_DAC, &data, 0 );
        if ( status == EOK ) {
            if ( mode )
                *val = ( (data != DA7212_DIG_ROUTING_DAC_ADC_OUTPUT_SRC) ? 1 : 0 );
            else
                *val = ( (data != DA7212_DIG_ROUTING_DAC_DAC_MONO_SRC) ? 1 : 0 );
        }
    }

    return status;
}

int da7212_dig_routing_dac_set ( da7212_context_t* da7212, uint32_t mode )
{
    int status = EOK;

    if ( mode )
        status = da7212_i2c_write ( da7212, DA7212_DIG_ROUTING_DAC, DA7212_DIG_ROUTING_DAC_ADC_OUTPUT_SRC, 0 );
    else
        status = da7212_i2c_write ( da7212, DA7212_DIG_ROUTING_DAC, DA7212_DIG_ROUTING_DAC_DAC_MONO_SRC, 0 );

    return status;
}

int da7212_mixout_get ( da7212_context_t* da7212, uint32_t* val )
{
    int status = EOK;
    uint32_t data;

    if ( val ) {
        status = da7212_i2c_read ( da7212, DA7212_MIXOUT_L_SELECT, &data, 0 );
        if ( status == EOK ) {
            *val = ( (data & DA7212_DIG_ROUTING_DAC_DAC_MONO_SRC) ? 1 : 0 );
        }

        status = da7212_i2c_read ( da7212, DA7212_MIXOUT_R_SELECT, &data, 0 );
        if ( status == EOK ) {
            *val = ( (data & DA7212_DIG_ROUTING_DAC_DAC_MONO_SRC) ? 1 : 0 );
        }
    }

    return status;
}

int da7212_mixout_set ( da7212_context_t* da7212, uint32_t set )
{
    int status = EOK;
    int status_L = EOK;
    int status_R = EOK;

    if ( set ) {
        status_L = da7212_bit_set ( da7212, DA7212_MIXOUT_L_SELECT, DA7212_MIXOUT_SELECT_DAC );
        status_R = da7212_bit_set ( da7212, DA7212_MIXOUT_R_SELECT, DA7212_MIXOUT_SELECT_DAC );
    } else {
        status_L = da7212_bit_clr ( da7212, DA7212_MIXOUT_L_SELECT, DA7212_MIXOUT_SELECT_DAC );
        status_R = da7212_bit_clr ( da7212, DA7212_MIXOUT_R_SELECT, DA7212_MIXOUT_SELECT_DAC );
    }
    status = status_L | status_R;
    return status;
}

int da7212_pll_setting ( da7212_context_t* da7212, uint32_t sample_rate, uint32_t required_sys_clk_freq )
{
    float div = 1;
    float val;
    int status = EOK;

    status = da7212_bit_clr ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_INDIV_MASK );
    if ( status != EOK ) {
        return status;
    }

    if ( da7212->params.mclk >= 2000000 && da7212->params.mclk <= 50000000) {
        if ( da7212->params.mclk <= 10000000 ) {
            div = 2;
            status = da7212_bit_set ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_INDIV_2_TO_10_MHZ );
        } else if ( da7212->params.mclk <= 20000000 ) {
            div = 4;
            status = da7212_bit_set ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_INDIV_10_TO_20_MHZ );
        } else if ( da7212->params.mclk <= 40000000 ) {
            div = 8;
            status = da7212_bit_set ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_INDIV_20_TO_40_MHZ );
        } else {
            div = 16;
            status = da7212_bit_set ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_INDIV_40_TO_80_MHZ );
        }
    } else {
        return EINVAL;
    }

    if ( status != EOK ) {
        return status;
    }
    usleep(10);

    val = (required_sys_clk_freq * 8) / (da7212->params.mclk / div);
    /* PLL setting (13.27 SRM PLL Mode (DAI Slave))
    *  (0x24) Reg PLL_FRAC_TOP (default=0x00)
    *  [7:5] (reserved) = 000b
    *  [4:0] PLL_FBDIV_FRAC_TOP= 01110b (PLL_FBDIV_FRAC_TOP (MSB))
    */
    status = da7212_i2c_write ( da7212, DA7212_PLL_FRAC_TOP, dec2hex_frac_4num ( val ) >> 11, 0 );
    if ( status != EOK ) {
        return status;
    }
    /*
    *  (0x25) Reg PLL_FRAC_BOT (default=0x00)
    *  [7:0] PLL_FBDIV_FRAC_BOT = 10100010b (PLL integer division value)
    */
    status = da7212_i2c_write ( da7212, DA7212_PLL_FRAC_BOT, (dec2hex_frac_4num ( val ) & 0x7ff) >> 3, 0 );
    if ( status != EOK ) {
        return status;
    }
    /*
    *  (0x26) Reg PLL_INTEGER (default=0x20)
    *  [7] (reserved)  = 0b
    *  [6:0] PLL_INDIV = 0011111b (PLL integer division value)
    */
    status = da7212_i2c_write ( da7212, DA7212_PLL_INTEGER, dec2hex ( val ), 0 );
    if ( status != EOK ) {
        return status;
    }

	return EOK;
}

int da7212_sys_clk_setting ( da7212_context_t* da7212, uint32_t sample_rate )
{
    int status = EOK;
    if ( sample_rate == 8000 ||  sample_rate == 12000 || sample_rate == 16000
            || sample_rate == 24000 || sample_rate == 32000 || sample_rate == 48000
            || sample_rate == 96000 ) {
        status = da7212_pll_setting ( da7212, sample_rate, 12288000 );
    } else {
        status = da7212_pll_setting ( da7212, sample_rate, 11289600 );
    }

    return status;
}

int da7212_rate_setting ( da7212_context_t* da7212, uint32_t sample_rate, int channel )
{
    uint32_t sr = DA7212_SR_441K;
    int status = EOK;

    switch ( sample_rate ) {
        case 8000:
            sr = DA7212_SR_8K;
            break;
        case 11025:
            sr = DA7212_SR_11025K;
            break;
        case 12000:
            sr = DA7212_SR_12K;
            break;
        case 16000:
            sr = DA7212_SR_16K;
            break;
        case 22050:
            sr = DA7212_SR_22050K;
            break;
        case 24000:
            sr = DA7212_SR_24K;
            break;
        case 32000:
            sr = DA7212_SR_32K;
            break;
        case 44100:
            sr = DA7212_SR_441K;
            break;
        case 48000:
            sr = DA7212_SR_48K;
            break;
        case 88200:
            sr = DA7212_SR_882K;
            break;
        case 96000:
            sr = DA7212_SR_96K;
            break;
        default:
            printf("WARNING: sample rate %d Hz is not supported!", sample_rate);
            return EINVAL;
    }

    status = da7212_i2c_write ( da7212, DA7212_SR, sr, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* DA7212 codec is slave */
    if ( !da7212->params.clk_master ){
        /* (0x27) Reg PLL_CTRL (default=0x0C)
        *  [7] PLL_EN          = 1b (PLL enable, 1 = system clock is PLL output)
        *  [6] PLL_SRM_EN      = 1b (PLL sample rate tracking enable, 1 = SRM enabled)
        *  [5] PLL_32K_MODE    = 0b (Sets the PLL into 32 kHz mode, 0 = disabled)
        *  [4] PLL_MCLK_SQR_EN = 0b (Enables the squarer at the MCLK, 0 = disabled)
        *  [3:2] PLL_INDIV     = 11b (Sets the input clock range for the PLL, 11 = 40 - 80 MHz)
        *  [1:0] (reserved)    = 00b
        */
        status = da7212_bit_set ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_SRM_EN );
        if ( status != EOK ) {
            return status;
        }

        status = da7212_sys_clk_setting ( da7212, sample_rate );
        if ( status != EOK ) {
            return status;
        }

        status = da7212_bit_set ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_EN );
        if ( status != EOK ) {
            return status;
        }

        /* (0x28) Reg DAI_CLK_MODE (default=0x01)
        *  [7] DAI_CLK_EN         = 0b (DAI master mode enable, 0 = slave mode (BCLK/WCLK inputs))
        *  [6:4] (reserved)       = 000b
        *  [3] DAI_WCLK_POL       = 0b (DAI word clock polarity, 0 = normal polarity)
        *  [2] DAI_CLK_POL        = 0b (DAI bit clock polarity, 0 = normal)
        *  [1] DAI_BCLKS_PER_WCLK = 01b (DAI master mode BCLK number per WCLK period, 01 = BCLK = 64)
        */
        status = da7212_i2c_write ( da7212, DA7212_DAI_CLK_MODE, 0x01, 0 );
        if ( status != EOK ) {
            return status;
        }
    } else {
        /* (0x27) Reg PLL_CTRL (default=0x0C)
        *  [7] PLL_EN          = 0b (PLL enable, 1 = system clock is PLL output)
        *  [6] PLL_SRM_EN      = 0b (PLL sample rate tracking enable, 1 = SRM enabled)
        *  [5] PLL_32K_MODE    = 0b (Sets the PLL into 32 kHz mode, 0 = disabled)
        *  [4] PLL_MCLK_SQR_EN = 0b (Enables the squarer at the MCLK, 0 = disabled)
        *  [3:2] PLL_INDIV     = 11b (Sets the input clock range for the PLL, 11 = 40 - 80 MHz)
        *  [1:0] (reserved)    = 00b
        */
        status = da7212_bit_clr ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_SRM_EN );
        if ( status != EOK ) {
            return status;
        }

        status = da7212_bit_clr ( da7212, DA7212_PLL_CTRL, DA7212_PLL_CTRL_EN );
        if ( status != EOK ) {
            return status;
        }

        /* (0x28) Reg DAI_CLK_MODE (default=0x01)
        *  [7] DAI_CLK_EN         = 1b (DAI master mode enable, 0 = slave mode (BCLK/WCLK inputs))
        *  [6:4] (reserved)       = 000b
        *  [3] DAI_WCLK_POL       = 0b (DAI word clock polarity, 0 = normal polarity)
        *  [2] DAI_CLK_POL        = 0b (DAI bit clock polarity, 0 = normal)
        *  [1] DAI_BCLKS_PER_WCLK = 01b (DAI master mode BCLK number per WCLK period, 01 = BCLK = 64)
        */
        status = da7212_i2c_write ( da7212, DA7212_DAI_CLK_MODE, 0x81, 0 );
        if ( status != EOK ) {
            return status;
        }
    }

    return EOK;
}

int da7212_capabilities ( da7212_context_t* da7212, ado_mixer_dll_codec_capabilities_t *cap, int channel )
{
    if ( cap == NULL ) {
        return EINVAL;
    }
    memset ( cap, 0, sizeof(*cap) );
    cap->rates = SND_PCM_RATE_8000 | SND_PCM_RATE_11025 | SND_PCM_RATE_12000 | SND_PCM_RATE_16000 |
        SND_PCM_RATE_22050 | SND_PCM_RATE_24000 | SND_PCM_RATE_32000 | SND_PCM_RATE_44100 |
        SND_PCM_RATE_48000 | SND_PCM_RATE_88200 | SND_PCM_RATE_96000;
    cap->formats = SND_PCM_FMT_S24_4 | SND_PCM_FMT_S16 | SND_PCM_FMT_S32;
    cap->max_voices = cap->min_voices = 2;

    return EOK;
}

int da7212_init ( da7212_context_t* da7212 )
{
    int status = EOK;
    uint32_t data;

    ado_debug ( DB_LVL_DRIVER, "" );

    if ( da7212 == NULL ) {
        ado_error_fmt ( "da7212 is NULL" );
        return EINVAL;
    }

    status = da7212_open_i2c_fd ( da7212 );
    if ( status != EOK ) {
        return status;
    }

    /* Soft-reset
    *  (0x1D) Reg CIF_CTRL (default=0x00)
    *  [7] CIF_REG_SOFT_RESET = 1b (Software reset that returns all the registers back to the default)
    *  [6:1] (reserved)       = 000000b
    *  [0] CIF_I2C_WRITE_MODE = 0b (0 = Pagrame Mode)
    */
    status = da7212_i2c_write ( da7212, DA7212_RESET, 0x80, 0 );
    if ( status != EOK ) {
        return status;
    }
    usleep(10);

    /* System power-on
    *  (0xFD) Reg SYSTEM_ACTIVE (default=0x00)
    *  [7:1] (reserved)  = 0000000b
    *  [0] SYSTEM_ACTIVE = 1b (Switch off the oscillator, 1 = oscillator on)
    *  Actual default value: 0x01
    */
    status = da7212_i2c_write ( da7212, DA7212_SYSTEM_ACTIVE, 0x01, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* Digital I/O voltage range
    *  (0x91) Reg IO_CTRL (default=0x00)
    *  [0] IO_VOLTAGE_LEVEL(0 = 1.2 to 2.8 V, 1 = 2.5 to 3.6 V)
    */
    status = da7212_i2c_write ( da7212, DA7212_IO_CTRL, 0x01, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* Set speed of the gain-ramping when activated
    *  (0x92) Reg GAIN_RAMP_CTRL (default=0x00)
    *  [7:2] (reserved)     = 000000b
    *  [1:0] GAIN_RAMP_RATE = 0b (Speed of the gain-ramping when activated, 00 = nominal rate/8)
    */
    status = da7212_i2c_write ( da7212, DA7212_GAIN_RAMP_CTRL, 0x00, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* Enable Digital LDO
    *  (0x90) Reg LDO_CTRL (default=0x00)
    *  [7] LDO_EN             = 1b (1 = LDO active)
    *  [6] (reserved)         = 0b
    *  [5:4] LDO_LEVEL_SELECT = 00b
    *  [3:0] (reserved)       = 0b
    */
    status = da7212_i2c_write ( da7212, DA7212_LDO_CTRL, 0x80, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* Mixed Sample Rates
    *  (0x84) Reg MIXED_SAMPLE_MODE (default=0x00)
    *  [7:1] (reserved) = 0000000b
    *  [0] 24_48_MODE   = 0b (24_48_MODE, 0 = Both ADC and DAC paths run at a speed determined by the setting of SR at 0x22[3:0])
    */
    status = da7212_i2c_write ( da7212, DA7212_MIXED_SAMPLE_MODE, 0x00, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* -2- Digital Audio Interface (DAI) setting =======================
    * (0x62) Reg MICBIAS_CTRL (default=0x11)
    *  [7] MICBIAS2_EN      = 0b (Microphone 2 bias enable, 0 = Disable)
    *  [6] (reserved)       = 0b
    *  [5:4] MICBIAS2_LEVEL = 01b (Microphone 2 bias level, 01 = 2.2 V)
    *  [3] MICBIAS1_EN      = 1b (Microphone 1 bias enable, 1 = Enable)
    *  [2] (reserved)       = 0b
    *  [1:0] MICBIAS1_LEVEL = 11b (Microphone 1 bias level, 01 = 2.2 V)
    */
    status = da7212_i2c_write ( da7212, DA7212_MICBIAS_CTRL, 0xBB, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* -3- Digital Audio Interface (DAI) setting =======================
    *  (0x38) ADC_FILTERS1 (default=0x80)
    *  [7] ADC_HPF_EN             = 1b (ADC high pass filter enable, 1 = enabled)
    *  [6] (reserved)             = 0b
    *  [5:4] ADC_AUDIO_HPF_CORNER = 00b (Cut-off frequency at the 3 dB for the High Pass Filter at 48 kHz, 00 = 2 Hz)
    *  [3] ALC_SYNC_MODE          = 0b (0 = disabled)
    *  [2:0] ALC_CALIB_MODE       = 000b (Cut-off frequency at the 3 dB for the Voice Filter at 8 kHz, 000 = 2.5 Hz)
    */
    status = da7212_i2c_write ( da7212, DA7212_ADC_FILTERS1, 0x88, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x67) ADC_L_CTRL (default=0x40)
    *  [7] ADC_L_EN      = 1b (ADC_L enable, 1 = enabled)
    *  [6] ADC_L_MUTE_EN = 1b (ADC_L mute enable, 0 = disabled)
    *  [5] ADC_L_RAMP_EN = 1b (ADC_L digital gain ramping enable (overrideszero crossing), 1 = enabled)
    *  [4:2] (reserved)  = 000b
    *  [1:0] (reserved)  = 00b
    */
    status = da7212_i2c_write ( da7212, DA7212_ADC_L_CTRL, 0xE0, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x67) ADC_R_CTRL (default=0x40)
    *  [7] ADC_R_EN      = 1b (ADC_R enable, 1 = enabled)
    *  [6] ADC_R_MUTE_EN = 1b (ADC_R mute enable, 0 = disabled)
    *  [5] ADC_R_RAMP_EN = 1b (ADC_R digital gain ramping enable (overrideszero crossing), 1 = enabled)
    *  [4:2] (reserved)  = 000b
    *  [1:0] (reserved)  = 00b
    */
    status = da7212_i2c_write ( da7212, DA7212_ADC_R_CTRL, 0xE0, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x9C) ALC_NOISE (default=0x3F)
    *  [7:6] (reserved)       = 00b
    *  [5:0] ALC_INTEG_ATTACK = 111111b (111111 = -94.5 dBFS)
    */
    status = da7212_i2c_write ( da7212, DA7212_ALC_NOISE, 0x17, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x9D) ALC_TARGET_MIN (default=0x3F)
    *  [7:6] (reserved)        = 00b
    *  [5:0] ALC_THRESHOLD_MIN = 111111b (111111 = -94.5 dBFS)
    */
    status = da7212_i2c_write ( da7212, DA7212_ALC_TARGET_MIN, 0x3F, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x9E) ALC_TARGET_MAX (default=0x00)
    *  [7:6] (reserved)        = 00b
    *  [5:0] ALC_THRESHOLD_MAX = 000000b (000000 = 0 dBFS)
    */
    status = da7212_i2c_write ( da7212, DA7212_ALC_TARGET_MAX, 0x30, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0xA1) ALC_ANTICLIP_CTRL (default=0x00)
    *  [7] ALC_ANTICLIP_EN  = 1b (Enables the ALC signal clip prevention mechanism, 1 = Enabled)
    *  [6:2] (reserved)     = 00000b
    *  [1:0] (reserved)     = 00b
    */
    status = da7212_i2c_write ( da7212, DA7212_ALC_ANTICLIP_CTRL, 0x80, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x2B) ALC_CTRL1 (default=0x00)
    *  [7] ALC_R_EN           = 0b (Enables the ALC operation on the right ADC channel, 0 = disabled)
    *  [6] (reserved)         = 0b
    *  [5] ALC_CALIB_OVERFLOW = 0b (Offset overflow during calibration, 0 = no offset overflow)
    *  [4] ALC_AUTO_CALIB_EN  = 1b (Automatic calibration enable (self clearing bit), 1 = enabled)
    *  [3] ALC_L_EN           = 0b (Enables the ALC operation on the left ADC channel, 0 = disabled)
    *  [2] ALC_CALIB_MODE     = 0b (Calibration mode, 0 = Automatic calibration)
    *  [1] ALC_SYNC_MODE      = 0b (Selects the ALC Operation Mode, 0 = Full digital gain solution)
    *  [0] ALC_OFFSET_EN      = 0b (Enable DC Offset cancellation, 0 = disabled)
    */
    status = da7212_i2c_write ( da7212, DA7212_ALC_CTRL1, 0x10, 0 );
    if ( status != EOK ) {
        return status;
    }
    usleep(40);

    /* (0x2B) ALC_CTRL1 (default=0x00)
    *  [7] ALC_R_EN           = 0b (Enables the ALC operation on the right ADC channel, 0 = disabled)
    *  [6] (reserved)         = 0b
    *  [5] ALC_CALIB_OVERFLOW = 0b (Offset overflow during calibration, 0 = no offset overflow)
    *  [4] ALC_AUTO_CALIB_EN  = 1b (Automatic calibration enable (self clearing bit), 1 = enabled)
    *  [3] ALC_L_EN           = 0b (Enables the ALC operation on the left ADC channel, 0 = disabled)
    *  [2] ALC_CALIB_MODE     = 0b (Calibration mode, 0 = Automatic calibration)
    *  [1] ALC_SYNC_MODE      = 0b (Selects the ALC Operation Mode, 0 = Full digital gain solution)
    *  [0] ALC_OFFSET_EN      = 0b (Enable DC Offset cancellation, 0 = disabled)
    */
    status = da7212_i2c_write ( da7212, DA7212_ALC_CTRL1, 0x8B, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* -4- Digital Audio Interface (DAI) setting =======================
    *  Digital Audio Interface (DAI) setting
    *  (0x21) Reg DIG_ROUTING_DAI (default=0x10)
    *  [7:6] (reserved) = 00b
    *  [5:4] DAI_R_SRC  = 11b (Data select for the DAI right output stream, 11 = DAI input right data)
    *  [3:2] (reserved) = 00b
    *  [1:0] DAI_L_SRC  = 10b (Data select for the DAI left output stream, 10 = DAI input left data)
    */
    da7212_dig_routing_dai_get ( da7212, &data, 0 );
    if ( data ) {
        da7212_dig_routing_dai_set ( da7212, 0 );
    }

    /* (0x29) Reg DAI_CTRL (default=0x08)
    *  [7] DAI_EN            = 1b (DAI enable, 1 = enabled)
    *  [6] DAI_OE            = 1b (DAI output enable, 1 = DATOUT pin is driven when required)
    *  [5] DAI_TDM_MODE_EN   = 0b (DAI TDM mode enable, 0 = DAI normal mode)
    *  [4] DAI_MONO_MODE_EN  = 0b (DAI mono mode enable, 0 = DAI stereo mode)
    *  [3:2] DAI_WORD_LENGTH = 11b (DAI data word length, 11 = 32 bits per channel)
    *  [1:0] DAI_FORMAT      = 00b (DAI data format, 00 = I2S mode)
    */
    status = da7212_i2c_write ( da7212, DA7212_DAI_CTRL, 0xCC, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* -5- Stereo Audio DAC setting =======================
    *  -5.1- Stereo Audio DAC setting =======================
    *  (0x2A) Reg DIG_ROUTING_DAC (default=0x32)
    *  [7] DAC_R_MONO  = 0b (0 = Right input stream)
    *  [6] (reserved)  = 0b
    *  [5:4] DAC_R_SRC = 11b (11 = Determined by DAC_R_MONO (bit [7] of this register))
    *  [3] DAC_L_MONO  = 0b (0 = Left input stream)
    *  [2] (reserved)  = 0b
    *  [1:0] DAC_L_SRC = 10b (10 = Determined by DAC_L_MONO (bit [3] of this register))
    */
    da7212_dig_routing_dac_get ( da7212, &data, 0 );
    if ( data ) {
        da7212_dig_routing_dac_set ( da7212, 0 );
    }

    /* DAC left/right control
    *  (0x69) Reg DAC_L_CTRL (default=0x48)
    *  [7] DAC_L_EN          = 1b    (1 = DAC enabled)
    *  [6] DAC_L_MUTE_EN     = 0b    (0 = mute disabled)
    *  [5] DAC_L_RAMP_EN     = 1b    (1 = ramp enabled)
    *  [4] (reserved)        = 0b
    *  [3] (reserved)        = 1b
    *  [2:0] (reserved)      = 000b
    */
    status = da7212_i2c_write ( da7212, DA7212_DAC_L_CTRL, 0xA8, 0 );
    if ( status != EOK ) {
        return status;
    }

    /*
    *  (0x6A) Reg DAC_R_CTRL (default=0x40)
    *  [7] DAC_R_EN          = 1b    (1 = DAC enabled)
    *  [6] DAC_R_MUTE_EN     = 0b    (0 = mute disabled)
    *  [5] DAC_R_RAMP_EN     = 1b    (1 = ramp enabled)
    *  [4] (reserved)        = 0b
    *  [3] (reserved)        = 0b
    *  [2:0] (reserved)      = 000b
    */
    status = da7212_i2c_write ( da7212, DA7212_DAC_R_CTRL, 0xA0, 0 );
    if ( status != EOK ) {
        return status;
    }

    /*
    *  (0xB2) Reg DAC_NG_CTRL (default=0x00)
    *  [7] DAC_NG_EN    = 1b (DAC noise-gate enable, 1 = enabled)
    *  [6:0] (reserved) = 0000000b
    */
    status = da7212_i2c_write ( da7212, DA7212_DAC_NG_CTRL, 0x80, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* -5.2- Headset setting  =======================
    * (0x6B) Reg HP_L_CTRL (default=0x41)
    * [7] HP_L_AMP_EN          = 1b  (HP_L amplifier enable, 1 = enabled)
    * [6] HP_L_AMP_MUTE_EN     = 0b  (HP_L amplifier mute enable, 1 = disabled)
    * [5] HP_L_AMP_RAMP_EN     = 1b  (HP_L amplifier gain ramping enable)
    * [4] HP_L_AMP_ZC_EN       = 0b  (HP_L amplifier zero cross gain update mode enable)
    * [3] HP_L_AMP_OE          = 1b  (HP_L amplifier output enable)
    * [2] HP_L_AMP_MIN_GAIN_EN = 0b (HP_L amplifiers gain held at the minimum value)
    * [1:0] (reserved)         = 01b
    */
    status = da7212_i2c_write ( da7212, DA7212_HP_L_CTRL, 0xA9, 0 );
    if ( status != EOK ) {
        return status;
    }

    /*
    # (0x6C) Reg HP_R_CTRL (default=0x40)
    * [7] HP_R_AMP_EN          = 1b  (HP_L amplifier enable, 1 = enabled)
    * [6] HP_R_AMP_MUTE_EN     = 0b  (HP_L amplifier mute enable, 1 = disabled)
    * [5] HP_R_AMP_RAMP_EN     = 1b  (HP_L amplifier gain ramping enable)
    * [4] HP_R_AMP_ZC_EN       = 0b  (HP_L amplifier zero cross gain update mode enable)
    * [3] HP_R_AMP_OE          = 1b  (HP_L amplifier output enable)
    * [2] HP_R_AMP_MIN_GAIN_EN = 0b (HP_L amplifiers gain held at the minimum value)
    * [1:0] (reserved)         = 00b
    */
    status = da7212_i2c_write ( da7212, DA7212_HP_R_CTRL, 0xA8, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* Charge pump (for headphone path) setting
    *  (0x47) Reg CP_CTRL (default=0x61)
    *  [7] CP_EN                   = 1b  (Charge pump enable)
    *  [6] CP_SMALL_SWITCH_FREQ_EN = 0b  (Charge pump low-load low-power mode enable)
    *  [5:4] CP_MCHANGE            = 10b (Charge pump tracking mode select - default)
    *  [3:2] CP_MOD                = 00b (Charge pump manual mode level control - default)
    *  [1:0] CP_ANALOGUE_LVL       = 01b (Charge pump analogue feedback control mode - default)
    */
    status = da7212_i2c_write ( da7212, DA7212_CP_CTRL, 0xA1, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* -6- Set input and output mixer paths and gains */
    /* -6.1- Mixin setting  =======================
    *  (0x32) MIXIN_L_SELECT (default=0x00)
    *  [7] DMIC_L_EN    = 0b (Enable the left DMIC input, 0 = disabled)
    *  [6] (reserved)   = 0b
    *  [5:4] (reserved) = 00b
    *  [3] MIXIN_R      = 0b (0 = MIXIN_R not mixed in)
    *  [2] MIC2_SEL     = 0b (0 = MIC2 input not mixed in)
    *  [1] MIC1_SEL     = 1b (1 = MIC1 input mixed in)
    *  [0] AUX_L_SEL    = 0b (0 = AUX_L input not mixed in)
    */
    status = da7212_i2c_write ( da7212, DA7212_MIXIN_L_SELECT, 0x02, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x33) MIXIN_R_SELECT (default=0x00)
    *  [7] DMIC_R_EN    = 0b (Enable the right DMIC input, 0 = disabled)
    *  [6] (reserved)   = 0b
    *  [5:4] (reserved) = 00b
    *  [3] MIXIN_L_SEL  = 0b (0 = MIXIN_L input not mixed in)
    *  [2] MIC1_SEL     = 1b (1 = MIC_1 input mixed in)
    *  [1] MIC2_SEL     = 0b (0 = MIC_2 input not mixed in)
    *  [0] AUX_R_SEL    = 0b (0 = AUX_R input not mixed in)
    */
    status = da7212_i2c_write ( da7212, DA7212_MIXIN_R_SELECT, 0x04, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x65) MIXIN_L_CTRL (default=0x40)
    *  [7] MIXIN_L_AMP_EN      = 1b (MIXIN_L amplifier enable, 1 = enabled)
    *  [6] MIXIN_L_AMP_MUTE_EN = 0b (MIXIN_L amplifier mute enable, 0 = disabled)
    *  [5] MIXIN_L_AMP_RAMP_EN = 1b (MIXIN_L amplifier gain ramping enable(overrides zero crossing), 1 = enabled)
    *  [4] MIXIN_L_AMP_ZC_EN   = 0b (MIXIN_L amplifier zero cross gain update mode enable, 0 = disabled)
    *  [3] MIXIN_L_MIX_EN      = 1b (MIXIN_L mixer enable, )
    *  [2] (reserved)          = 0b
    *  [1:0] (reserved)        = 00b
    */
    status = da7212_i2c_write ( da7212, DA7212_MIXIN_L_CTRL, 0xA8, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x66) MIXIN_R_CTRL (default=0x40)
    *  [7] MIXIN_R_AMP_EN      = 1b (MIXIN_R amplifier enable, 1 = enabled)
    *  [6] MIXIN_R_AMP_MUTE_EN = 0b (MIXIN_R amplifier mute enable, 0 = disabled)
    *  [5] MIXIN_R_AMP_RAMP_EN = 1b (MIXIN_R amplifier gain ramping enable(overrides zero crossing), 1 = enabled)
    *  [4] MIXIN_R_AMP_ZC_EN   = 0b (MIXIN_R amplifier zero cross gain update mode enable, 0 = disabled)
    *  [3] MIXIN_R_MIX_EN      = 1b (MIXIN_R mixer enable, 1 = mixer enabled)
    *  [2] (reserved)          = 0b
    *  [1:0] (reserved)        = 00b
    */
    status = da7212_i2c_write ( da7212, DA7212_MIXIN_R_CTRL, 0xA8, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* -6.2- Mixout setting ======================= */
    da7212_mixout_set ( da7212, 1 );

    /* (0x6E) MIXOUT_L_CTRL (default=0x10)
    *  [7] MIXOUT_L_AMP_EN     = 1b (MIXOUT_L mixer amp enable, 1 = enabled)
    *  [6] (reserved)          = 0b
    *  [5] (reserved)          = 0b
    *  [4] MIXOUT_L_SOFTMIX_EN = 1b (MIXOUT L soft mix enable, 1 = enabled)
    *  [3] MIXOUT_L_MIX_EN     = 1b (MIXOUT L mixer enable, 1 = enabled)
    *  [2] (reserved)          = 0b
    *  [1:0] (reserved)        = 00b
    */
    status = da7212_i2c_write ( da7212, DA7212_MIXOUT_L_CTRL, 0x98, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x6F) MIXOUT_R_CTRL (default=0x10)
    *  [7] MIXOUT_R_AMP_EN     = 1b (MIXOUT_R mixer amp enable, 1 = enabled)
    *  [6:5] (reserved)        = 00b
    *  [4] MIXOUT_R_SOFTMIX_EN = 1b (MIXOUT R soft mix enable, 1 = enabled)
    *  [3] MIXOUT_R_MIX_EN     = 0b (MIXOUT R mixer enable, 1 = enabled)
    *  [2:0] (reserved)        = 000b
    */
    status = da7212_i2c_write ( da7212, DA7212_MIXOUT_R_CTRL, 0x98, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* -7- System Level 1 & 2 System Controller activate */
    /* -7.1- Enable input/output paths using the Level 1 system controller (SLC1) =======================
    *  (0x50) SYSTEM_MODES_INPUT (default=0x00)
    *  [Bit 7] ADC_R       = 1b (Preconfigured system mode ADC_R Control, 0 = ADC_R not used)
    *  [Bit 6] ADC_L       = 1b (Preconfigured system mode ADC_L Control, 0 = ADC_L not used)
    *  [Bit 5] MIXIN_R     = 1b (Preconfigured system mode IN_R Control, 1 = use IN_R amplifer)
    *  [Bit 4] MIXIN_L     = 1b (Preconfigured system mode IN_L Control, 1 = use IN_L amplifer)
    *  [Bit 3] MIC_2       = 0b (Preconfigured system mode MIC_2 Control, 0 = MIC 2 amplifier not used)
    *  [Bit 2] MIC_1       = 1b (Preconfigured system mode MIC_1 Control, 1 = use MIC 1 amplifier)
    *  [Bit 1] (Reserved)  = 0b
    *  [Bit 0] MODE_SUBMIT = 1b (System Controller 2 activation control, 1 = ADC_mode and DAC_mode active)
    */
    status = da7212_i2c_write ( da7212, DA7212_SYSTEM_MODES_INPUT, 0xF5, 0 );
    if ( status != EOK ) {
        return status;
    }
    usleep(10);

    /* -7.2- System Level 2 System Controller activate =======================
    *  (0x51) SYSTEM_MODES_OUTPUT (default=0x00)
    *  [7] DAC_R = 1b (Preconfigured system mode DAC_R Control, 1 = use DAC R)
    *  [6] DAC_L = 1b (Preconfigured system mode DAC_L Control, 1 = use DAC L)
    *  [5] HP_R  = 1b (Preconfigured system mode HP_R Control, 1 = use HP_R amplifier)
    *  [4] HP_L  = 1b (Preconfigured system mode HP_L Control, 1 = use HP_L amplifier)
    *  [3] LINE  = 0b
    *  [2] AUX_R = 0b
    *  [1] AUX_L = 0b
    *  [0] MODE_SUBMIT = 1b (System Controller 2 activation control, 1 = ADC_mode and DAC_mode active)
    */
    status = da7212_i2c_write ( da7212, DA7212_SYSTEM_MODES_OUTPUT, 0xF1, 0 );
    if ( status != EOK ) {
        return status;
    }
    usleep(10);

    /* (0x63) MIC_1_CTRL (default=0x40)
    *  [7] MIC_1_AMP_EN       = 1b (MIC_1 amplifier enable, 1 = enabled)
    *  [6] MIC_1_AMP_MUTE_EN  = 0b (MIC_1 amplifier mute enable, 0 = amplifier unmuted)
    *  [5] (reserved)         = 0b
    *  [4] (reserved)         = 0b
    *  [3:2] MIC_1_AMP_IN_SEL = 00b (MIC_1 input source select, 00 = differential)
    *  [1:0] (reserved)       = 00b
    */
   status = da7212_i2c_write ( da7212, DA7212_MIC_1_CTRL, 0x80, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x67) ADC_L_CTRL (default=0x40)
    *  [7] ADC_L_EN      = 1b (ADC_L enable, 1 = enabled)
    *  [6] ADC_L_MUTE_EN = 0b (ADC_L mute enable, 0 = disabled)
    *  [5] ADC_L_RAMP_EN = 1b (ADC_L digital gain ramping enable (overrideszero crossing), 1 = enabled)
    *  [4:2] (reserved)  = 000b
    *  [1:0] (reserved)  = 00b
    */
   status = da7212_i2c_write ( da7212, DA7212_ADC_L_CTRL, 0xA0, 0 );
    if ( status != EOK ) {
        return status;
    }

    /* (0x68) ADC_R_CTRL (default=0x40)
    *  [7] ADC_R_EN      = 1b (ADC_R enable, 1 = enabled)
    *  [6] ADC_R_MUTE_EN = 0b (ADC_R mute enable, 0 = disabled)
    *  [5] ADC_R_RAMP_EN = 1b (ADC_R digital gain ramping enable (overrideszero crossing), 1 = enabled)
    *  [4:2] (reserved)  = 000b
    *  [1:0] (reserved)  = 00b
    */
   status = da7212_i2c_write ( da7212, DA7212_ADC_R_CTRL, 0xA0, 0 );
    if ( status != EOK ) {
        return status;
    }


    if ( da7212->debug ) {
        da7212_register_dump ( da7212 );
    }

    return EOK;
}

int da7212_deinit ( da7212_context_t* da7212 )
{
    int status = EOK;
    /* For both playback and capture */

    /* System power-on
    *  (0xFD) Reg SYSTEM_ACTIVE (default=0x00)
    *  [7:1] (reserved)  = 0000000b
    *  [0] SYSTEM_ACTIVE = 1b (Switch off the oscillator, 1 = oscillator on)
    *  Actual default value: 0x01
    */
    status = da7212_i2c_write ( da7212, DA7212_SYSTEM_ACTIVE, 0x00, 0 );
    if ( status != EOK ) {
        return status;
    }


    if ( da7212->debug ) {
        da7212_register_dump ( da7212 );
    }

    da7212_close_i2c_fd ( da7212 );

    return EOK;
};

void da7212_register_dump ( da7212_context_t* da7212 )
{
    int status = EOK;
    uint32_t reg_val = 0;

    status = da7212_i2c_read ( da7212, DA7212_SYSTEM_ACTIVE, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_SYSTEM_ACTIVE - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_SR, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_SR - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_MIXED_SAMPLE_MODE, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_MIXED_SAMPLE_MODE - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_PLL_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_PLL_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_DIG_ROUTING_DAI, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_DIG_ROUTING_DAI - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_DAI_CLK_MODE, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_DAI_CLK_MODE - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_DAI_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_DAI_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_DIG_ROUTING_DAC, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_DIG_ROUTING_DAC - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_DAC_L_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_DAC_L_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_DAC_R_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_DAC_R_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_DAC_L_GAIN, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DAC_L_GAIN - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_DAC_R_GAIN, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DAC_R_GAIN - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_HP_L_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_HP_L_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_HP_R_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_HP_R_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_HP_L_GAIN, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_HP_L_GAIN - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_HP_R_GAIN, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_HP_R_GAIN - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_CP_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_CP_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_MIXOUT_L_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "MIXOUT_L_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_MIXOUT_R_CTRL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "MIXOUT_R_CTRL - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_MIXOUT_L_SELECT, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "MIXOUT_L_SELECT - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_MIXOUT_R_SELECT, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_MIXOUT_R_SELECT - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_SYSTEM_MODES_OUTPUT, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_SYSTEM_MODES_OUTPUT - 0x%03x", reg_val );
    }

    status = da7212_i2c_read ( da7212, DA7212_SYSTEM_STATUS, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "DA7212_SYSTEM_STATUS - 0x%03x", reg_val );
    }
}

