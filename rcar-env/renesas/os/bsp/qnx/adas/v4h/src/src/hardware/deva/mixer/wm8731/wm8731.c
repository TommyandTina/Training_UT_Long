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
#include "wm8731.h"
#include "../i2c2.h"

/* wm8731 registers */
#define WM8731_LEFT_LINE_IN                    0x0
 #define WM8731_LEFT_LINE_IN_LINVOL             0x1f
 #define WM8731_LEFT_LINE_IN_LINMUTE            (1 << 7)
#define WM8731_RIGHT_LINE_IN                   0x1
 #define WM8731_RIGHT_LINE_IN_LINVOL            0x1f
 #define WM8731_RIGHT_LINE_IN_LINMUTE           (1 << 7) 
#define WM8731_LEFT_HEADP_OUT                  0x2
 #define WM8731_LEFT_HEADP_OUT_MASK             0x7F
#define WM8731_RIGHT_HEADP_OUT                 0x3
 #define WM8731_RIGHT_HEADP_OUT_MASK            0x7F
#define WM8731_ANALOG_PATH_CONTROL             0x4
 #define WM8731_ANALOG_PATH_CONTROL_MICBOOST    (1 << 0)  
 #define WM8731_ANALOG_PATH_CONTROL_MUTEMIC     (1 << 1)  
 #define WM8731_ANALOG_PATH_CONTROL_INSEL_MASK  (1 << 2)  
 #define WM8731_ANALOG_PATH_CONTROL_INSEL_MIC   (1 << 2)  
 #define WM8731_ANALOG_PATH_CONTROL_INSEL_LIN   (0 << 2)  
 #define WM8731_ANALOG_PATH_CONTROL_BYPASS      (1 << 3)
 #define WM8731_ANALOG_PATH_CONTROL_DACSEL      (1 << 4)
 #define WM8731_ANALOG_PATH_CONTROL_SIDETONE    (1 << 5)
#define WM8731_DIGITAL_PATH_CONTROL            0x05
#define WM8731_DIGITAL_PATH_CONTROL_DACMU       (1 << 3)
#define WM8731_POWER_DOWN_CONTROL              0x06
#define WM8731_DIGITAL_INTERFACE_FORMAT        0x07
#define WM8731_SAMPLING_CONTROL                0x08
 #define WM8731_SAMPLING_CONTROL_SR_MASK        (0xf << 2)
 #define WM8731_SAMPLING_CONTROL_8K             (0x3 << 2)
 #define WM8731_SAMPLING_CONTROL_32K            (0x6 << 2)
 #define WM8731_SAMPLING_CONTROL_441K           (0x8 << 2)
 #define WM8731_SAMPLING_CONTROL_48K            (0x0 << 2)
 #define WM8731_SAMPLING_CONTROL_882K           (0xF << 2)
 #define WM8731_SAMPLING_CONTROL_96K            (0x7 << 2)
#define WM8731_ACTIVE_CONTROL                  0x9
#define WM8731_RESET                           0xF

static int wm8731_open_i2c_fd(wm8731_context_t *wm8731)
{
    int status = EOK;

    if ( wm8731 ) {
        if ( wm8731->params.codec_open ) {
            status = wm8731->params.codec_open( wm8731->params.hw_context );
        } else {
            char i2c_devname[20];
            sprintf ( i2c_devname, "/dev/i2c%d", wm8731->params.i2c_dev );
            status = open_i2c_fd ( i2c_devname );
        }
        if ( status != EOK ) {
            ado_error_fmt ( "wm8731 codec open failed" );
        }
    } else {
        ado_error_fmt( "wm8731 codec is not init" );
        status = EINVAL;
    }
    return status;
}

static int wm8731_close_i2c_fd(wm8731_context_t *wm8731)
{
    int status = EOK;

    if ( wm8731 ) {
        if ( wm8731->params.codec_close ) {
            wm8731->params.codec_close( wm8731->params.hw_context );
        } else {
            close_i2c_fd();
        }
    } else {
        ado_error_fmt( "wm8731 codec is not init" );
        status = EINVAL;
    }

    return status;
}

static int wm8731_i2c_read (wm8731_context_t *wm8731, uint16_t reg, uint32_t *val, int index )
{
    int status = EOK;
    uint32_t reg_val;

    if ( wm8731 && val ) {
        if ( wm8731->params.codec_read ) {
            status = wm8731->params.codec_read( wm8731->params.hw_context, reg, &reg_val, index );
            if ( status == EOK ) {
                *val = reg_val;
            }
        } else {
            status = i2c_read ( wm8731->params.i2c_addr, reg, val, index );
        }
        if ( status != EOK ) {
            ado_error_fmt ( "WM8731 i2c read failed" );
        }
    } else {
        ado_error_fmt("WM8731 codec is not init or val is NULL");
        status = EINVAL;
    }

    return status;
}

static int wm8731_i2c_write ( wm8731_context_t *wm8731, uint16_t reg, uint16_t val, int index )
{
    int status = EOK;

    if ( wm8731 ) {
        if ( wm8731->params.codec_write ) {
            status = wm8731->params.codec_write( wm8731->params.hw_context, reg, val, index );
        } else {
            status = i2c_write ( wm8731->params.i2c_addr, reg, val, index );
        }
        if ( status != EOK ) {
            ado_error_fmt ( "wm8731 i2c write failed" );
        }
    } else {
        ado_error_fmt("wm8731 codec is not init");
        status = EINVAL;
    }

    return status;
}

static int wm8731_bit_set(wm8731_context_t *wm8731, uint16_t reg, uint32_t val)
{
    uint32_t data;

    if(wm8731_i2c_read(wm8731, reg, &data, 0) != EOK)
        return EINVAL;

    data &= ~val;
    data |= val;
    return wm8731_i2c_write(wm8731, reg, val, 0);
}

static int wm8731_bit_clr(wm8731_context_t *wm8731, uint16_t reg, uint32_t val)
{
    uint32_t data;
    if(wm8731_i2c_read(wm8731, reg, &data, 0) != EOK)
        return EINVAL;
    data &= ~val;
    return wm8731_i2c_write(wm8731, reg, val, 0);
}

// static int wm8731_count_bits ( uint8_t bitmask )
// {
//     int count = 0;
//     while ( bitmask ) {
//         bitmask &= (bitmask - 1);
//         count++;
//     }
//     return count;
// }

int wm8731_init ( wm8731_context_t* wm8731 )
{
    int status = EOK;
    
    ado_debug ( DB_LVL_DRIVER, "" );

    if ( wm8731 == NULL ) {
        ado_error_fmt ( "wm8731 is NULL" );
        return EINVAL;
    }

    status = wm8731_open_i2c_fd( wm8731 );
    if ( status != EOK ) {
        return status;
    }

    /* Reset wm8731 codec */
    status = wm8731_i2c_write(wm8731, WM8731_RESET, 0, 0);
    if ( status != EOK ) {
        return status;
    }

    delay(1);
     
    /* (0x09) Active Control (default=0x0)
    *  [0]   ACTIVE =0    Activate Interface (0: Active, 1: Inactive)
    */
    status = wm8731_i2c_write(wm8731, WM8731_ACTIVE_CONTROL, 0x000, 0);
    if ( status != EOK ) {
        return status;
    }

    /* (0x00) Left Line In (default=0x97)
    *  [8] LRINBOTH  =0     Left to Right Channel Line InputVolume 
                              and Mute Data Load Control (0: Disable, 1:Enable)
    *  [7] LINMUTE   =0     Left Channel Line Input Mute to ADC (0:Disable, 1:Enable)
    *  [6:5] Fixed   =0
    *  [4:0] LINVOL  =10111 Left Channel Line Input Volume Control
    */
    status = wm8731_i2c_write(wm8731, WM8731_LEFT_LINE_IN, 0x017, 0);
    if ( status != EOK ) {
        return status;
    }

    /* (0x01) Right Line In (default=0x97)
    *  [8] RLINBOTH  =0     Right to Left Channel Line InputVolume 
                            and Mute Data Load Control (0: Disable, 1:Enable)
    *  [7] RINMUTE   =0     Right Channel Line Input Mute to ADC (0:Disable, 1:Enable)
    *  [6:5] Fixed   =0
    *  [4:0] RINVOL  =10111 Right Channel Line Input Volume Control
    */
    status = wm8731_i2c_write(wm8731, WM8731_RIGHT_LINE_IN, 0x017, 0);
    if ( status != EOK ) {
        return status;
    }

    /* (0x02) Left Headphone Out (default=0x079)
    *  [8] LRHPBOTH  =0       Left to Right Channel HeadphoneVolume, Mute 
                              and Zero Cross DataLoad Control (0: Disable, 1:Enable)
    *  [7] LZCEN   =0         Left Channel Zero Cross detect Enable (0:Disable, 1:Enable)
    *  [6:0] LHPVOL  =1110000 Left Channel Headphone Output Volume Control
    */
    status = wm8731_i2c_write(wm8731, WM8731_LEFT_HEADP_OUT, 0x070, 0);
    if ( status != EOK ) {
        return status;
    }

    /* (0x03) Right Headphone Out (default=0x079)
    *  [8] LRHPBOTH  =0       Right to left Channel HeadphoneVolume, Mute 
                              and Zero Cross DataLoad Control (0: Disable, 1:Enable)
    *  [7] LZCEN   =0         Right Channel Zero Cross detect Enable (0:Disable, 1:Enable)
    *  [6:0] LHPVOL  =1110000 Right Channel Headphone Outpu tVolume Control
    */
    status = wm8731_i2c_write(wm8731, WM8731_RIGHT_HEADP_OUT, 0x070, 0);
    if ( status != EOK ) {
        return status;
    }

    /* (0x04) Analogue AudioPath Control (default=0x0A)
    *  [7:6] SIDEATT =0        Side Tone Attenuation (11:-15dB, 10:-12dB, 01:-9dB, 00:-6dB)
    *  [5] SIDETONE  =0        Side Tone Switch (0:Disable, 1:Enable)
    *  [4] DACSEL    =1        DAC Select (0:Don't select, 1:select)
    *  [3] BYPASS    =0        Bypass Switch (0:Disable, 1:Enable)
    *  [2] INSEL     =0        Microphone/Line Input Select to ADC (0:Microphone, 1: Line Input)
    *  [1] MUTEMIC   =0        Line Input Mute to ADC (0:Disable, 1: Enable)
    *  [0] MICBOOST  =1110000  Microphone Input Level Boost (0:Disable, 1: Enable)
    */
    status = wm8731_i2c_write(wm8731, WM8731_ANALOG_PATH_CONTROL, 0x010, 0);
    if ( status != EOK ) {
        return status;
    }

#if 0
    status = wm8731_i2c_write(wm8731, WM8731_ANALOG_PATH_CONTROL, 0x014, 0);
    if ( status != EOK ) {
        return status;
    }
#endif

    /* (0x05) Digital Audio Path Control (default=0x008)
    *  [3] DACMU   =0  DAC Soft Mute Control (0:Disable, 1:Enable)
    *  [2:1] DEEMP =00 De-emphasis Control (00:Disable, 01:32kHz, 10:44.1kHz, 11:48kHz)
    *  [0] ADCHPD  =0  ADC High Pass Filter Enable (0:Disable, 1: Enable)
    */
    status = wm8731_i2c_write(wm8731, WM8731_DIGITAL_PATH_CONTROL, 0x001, 0);
    if ( status != EOK ) {
        return status;
    }

    /* (0x06) Power Down Control(default=0x09F)
    *  [7] POWEROFF =0  POWEROFF mode (0:Disable, 1: Enable)
    *  [6] CLKOUTPD =0  Side Tone Switch CLKOUT power down (0:Disable, 1:Enable)
    *  [5] OSCPD    =0  Oscillator Power Down (0:Disable, 1:Enable)
    *  [4] OUTPD    =0  Outputs Power Down (0:Disable, 1:Enable)
    *  [3] DACPD    =0  DAC Power Down (0:Disable, 1:Enable)
    *  [2] ADCPD    =0  ADC Power Down (0:Disable, 1: Enable)
    *  [1] MICPD    =0  Microphone Input an Bias PowerDown (0:Disable, 1: Enable)
    *  [0] LINEINPD =0  Line Input Power Down (0:Disable, 1: Enable)
    */
    status = wm8731_i2c_write(wm8731, WM8731_POWER_DOWN_CONTROL, 0x000, 0);
    if ( status != EOK ) {
        return status;
    }

    /* (0x07) Digital AudioInterface Format(default=0x00A)
    *  [7] BCLKINV =1  Bit Clock Invert (0:Don’t invert BCLK, 1:invert BCLK)
    *  [6] MS      =1  Master Slave Mode Control (0:Enable Slave Mode, 1:Enable Master Mod)
    *  [5] LRSWAP  =1  DAC Left Right Clock Swap (0: Right Channel DAC Data Right, 1:Right Channel DAC Data Left)
    *  [4] LRP     =0  DACLRC phase control (
    *  [3:2] IWL   =00  Input Audio Data Bit Length Select (00:16 bits,01:20 bits, 10:24 bits, 11:32 bits)
    *  [1:0] ADCPD =00  Audio Data Format Select
    */
    status = wm8731_i2c_write(wm8731, WM8731_DIGITAL_INTERFACE_FORMAT, 0x00E, 0);
    if ( status != EOK ) {
        return status;
    }

     /* (0x08) Sampling Control(default=0x000)
    *  [7] CLKODIV2   =0  CLKOUT divider select
    *  [6] CLKIDIV2   =0  Core Clock divider select
    *  [5:2] SR       =00 ADC and DAC sample rate control;
    *  [1] BOSR       =0  Base Over-Sampling Rate
    *  [0] USB/NORMAL =0  Mode Select (0: Normal mode, 1: USB mode)
    */
    status = wm8731_i2c_write(wm8731, WM8731_SAMPLING_CONTROL, 0x000, 0);
    if ( status != EOK ) {
        return status;
    }

    /* (0x09) Active Control (default=0x0)
    *  [0]   ACTIVE =1    Activate Interface (0: Active, 1: Inactive)
    */
    status = wm8731_i2c_write(wm8731, WM8731_ACTIVE_CONTROL, 0x001, 0);
    if ( status != EOK ) {
        return status;
    }

    if ( wm8731->debug ) {
        wm8731_register_dump ( wm8731 );
    }

    return EOK;
}

int wm8731_deinit(wm8731_context_t* wm8731)
{
    int status = EOK;
    /* For both playback and capture */
   
    /* (0x06) Power Down Control(default=0x09F)
    *  [7] POWEROFF =1  POWEROFF mode (0:Disable, 1: Enable)
    *  [6] CLKOUTPD =1  Side Tone SwitchCLKOUT power down (0:Disable, 1:Enable)
    *  [5] OSCPD    =1  Oscillator Power Down (0:Disable, 1:Enable)
    *  [4] OUTPD    =1  Outputs Power Down (0:Disable, 1:Enable)
    *  [3] DACPD    =1  DAC Power Down (0:Disable, 1:Enable)
    *  [2] ADCPD    =1  ADC Power Down (0:Disable, 1: Enable)
    *  [1] MICPD    =1  Microphone Input an Bias PowerDown (0:Disable, 1: Enable)
    *  [0] LINEINPD =1  Line Input Power Down (0:Disable, 1: Enable)
    */
    status = wm8731_i2c_write(wm8731, WM8731_POWER_DOWN_CONTROL, 0xFF, 0);
    if ( status != EOK ) {
        return status;
    }

    if ( wm8731->debug ) {
        wm8731_register_dump(wm8731);
    }

    wm8731_close_i2c_fd(wm8731);

    return EOK;
};

int wm8731_output_vol_get(wm8731_context_t* wm8731, uint32_t *vol)
{
    int status = EOK;
    uint32_t data;

    if ( vol == NULL ) {
        return EINVAL;
    }

    status = wm8731_i2c_read (wm8731, WM8731_LEFT_HEADP_OUT, &data, 0);
    if ( status != EOK ) {
        return status;
    }
    vol[LEFT_CHAN] = data & WM8731_LEFT_HEADP_OUT_MASK;

    status = wm8731_i2c_read (wm8731, WM8731_RIGHT_HEADP_OUT, &data, 0);
    if ( status != EOK ) {
        return status;
    }
    vol[RIGHT_CHAN] = data & WM8731_RIGHT_HEADP_OUT_MASK;

    return EOK;
}

int wm8731_output_vol_set(wm8731_context_t* wm8731, int channel, uint32_t vol)
{
    int status = EOK;

    if(channel == LEFT_CHAN) {
        status = wm8731_bit_set(wm8731, WM8731_LEFT_HEADP_OUT, (vol & WM8731_LEFT_HEADP_OUT_MASK));
        if ( status != EOK ) {
            return status;
        }
    } else if(channel == RIGHT_CHAN) {
        status = wm8731_bit_set(wm8731, WM8731_RIGHT_HEADP_OUT, (vol & WM8731_RIGHT_HEADP_OUT_MASK));
        if ( status != EOK ) {
            return status;
        }
    }

    return EOK;;
}

int wm8731_output_mute_get(wm8731_context_t* wm8731, uint32_t *mute)
{
    int status = EOK;
    uint32_t data;

    if ( mute == NULL ) {
        return EINVAL;
    }

    /* Bit 3 of WM8731_DIGITAL_PATH_CONTROL register is mute status */
    if ( mute ) {
        status = wm8731_i2c_read ( wm8731, WM8731_DIGITAL_PATH_CONTROL, &data, 0 );
        if ( status == EOK ) {
            *mute = ( (data & WM8731_DIGITAL_PATH_CONTROL_DACMU) ? 1 : 0 );
        }
    }

    return status;
}

int wm8731_output_mute_set(wm8731_context_t* wm8731, uint32_t mute)
{
    int status = EOK;

    if ( mute )
    	status = wm8731_bit_set(wm8731, WM8731_DIGITAL_PATH_CONTROL, WM8731_DIGITAL_PATH_CONTROL_DACMU);
    else
        status = wm8731_bit_clr(wm8731, WM8731_DIGITAL_PATH_CONTROL, WM8731_DIGITAL_PATH_CONTROL_DACMU);

    return status;
}

int wm8731_line_input_vol_get(wm8731_context_t* wm8731, uint32_t *vol)
{
    int status = EOK;
    uint32_t data;

    if ( vol == NULL ) {
        return EINVAL;
    }

    status = wm8731_i2c_read(wm8731, WM8731_LEFT_LINE_IN, &data, 0);
    if ( status != EOK ) {
        return status;
    }
    vol[LEFT_CHAN] = data;

    status = wm8731_i2c_read(wm8731, WM8731_RIGHT_LINE_IN, &data, 0);
    if ( status != EOK ) {
        return status;
    }
    vol[RIGHT_CHAN] = data;

    return EOK;
}

int wm8731_line_input_vol_set(wm8731_context_t* wm8731, int channel, uint32_t vol)
{
    int status = EOK;

    if(channel == LEFT_CHAN) {
        status = wm8731_bit_set(wm8731, WM8731_LEFT_LINE_IN, vol);
        if ( status != EOK ) {
            return status;
        }
    } else if(channel == RIGHT_CHAN) {
        status = wm8731_bit_set(wm8731, WM8731_RIGHT_LINE_IN, vol);
        if ( status != EOK ) {
            return status;
        }
    }
    
    return EOK;;
}

int wm8731_line_input_mute_get(wm8731_context_t* wm8731, uint32_t *mute)
{
    int status = EOK;
    uint32_t data;

    if ( mute == NULL ) {
        return EINVAL;
    }

    /* Bit 7 of WM8731_LEFT_LINE_IN/WM8731_RIGHT_LINE_IN register is mute status */
    status = wm8731_i2c_read ( wm8731, WM8731_LEFT_LINE_IN, &data, 0 );
    if ( status == EOK ) {
        mute[LEFT_CHAN] = ( (data & WM8731_LEFT_LINE_IN_LINMUTE) ? 1 : 0 );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_RIGHT_LINE_IN, &data, 0 );
    if ( status == EOK ) {
        mute[RIGHT_CHAN] = ( (data & WM8731_RIGHT_LINE_IN_LINMUTE) ? 1 : 0 );
    }

    return status;
}

int wm8731_line_input_mute_set(wm8731_context_t* wm8731, int channel, uint32_t mute)
{
    int status = EOK;

    if(mute) {
        if ( channel == LEFT_CHAN ) {
            status = wm8731_bit_set(wm8731, WM8731_LEFT_LINE_IN, WM8731_LEFT_LINE_IN_LINMUTE);
        } else if ( channel == LEFT_CHAN ) {
            status = wm8731_bit_set(wm8731, WM8731_LEFT_LINE_IN, WM8731_RIGHT_LINE_IN_LINMUTE);
        }
    } else {
    	if ( channel == LEFT_CHAN ) {
            status = wm8731_bit_clr(wm8731, WM8731_LEFT_LINE_IN, WM8731_LEFT_LINE_IN_LINMUTE);
        } else if ( channel == LEFT_CHAN ) {
            status = wm8731_bit_clr(wm8731, WM8731_LEFT_LINE_IN, WM8731_RIGHT_LINE_IN_LINMUTE);
        }
    }
    
    return status;
}

int wm8731_mic_input_mute_get(wm8731_context_t* wm8731, uint32_t* mute)
{
    int status = EOK;
    uint32_t data;

    /* Bit 1 of WM8731_ANALOG_PATH_CONTROL register is line input mute status */
    if ( mute ) {
        status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
        if ( status == EOK ) {
            *mute = ( (data & WM8731_ANALOG_PATH_CONTROL_MUTEMIC) ? 1 : 0 );
        }
    }

    return status;
}

int wm8731_mic_input_mute_set(wm8731_context_t* wm8731, uint32_t mute)
{
    int status = EOK;
    uint32_t data;

    status = wm8731_i2c_read(wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0);
    if ( status != EOK ) {
        return status;
    }
    
    data  &= ~WM8731_ANALOG_PATH_CONTROL_MUTEMIC;
    if ( mute ) {
        data |= WM8731_ANALOG_PATH_CONTROL_MUTEMIC;
    }

    return wm8731_bit_set ( wm8731, WM8731_ANALOG_PATH_CONTROL, data);
} 

int wm8731_mic_boost_get(wm8731_context_t* wm8731, uint32_t* val)
{
    int status = EOK;
    uint32_t data;

    if ( val ) {
        status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
        if ( status == EOK ) {
            *val = ( (data & WM8731_ANALOG_PATH_CONTROL_MICBOOST) ? 1 : 0 );
        }
    }

    return status;
}

int wm8731_mic_boost_set(wm8731_context_t* wm8731, uint32_t set)
{
    int status = EOK;
    uint32_t data;

    status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
    if ( status != EOK ) {
        return status;
    }

    if ( set ) {
        data |= WM8731_ANALOG_PATH_CONTROL_MICBOOST;
    } else {
        data &= ~WM8731_ANALOG_PATH_CONTROL_MICBOOST;
    }

    return wm8731_bit_set ( wm8731, WM8731_ANALOG_PATH_CONTROL, data);
}

int wm8731_bypass_get(wm8731_context_t* wm8731, uint32_t* val)
{
    int status = EOK;
    uint32_t data;

    if ( val ) {
        status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
        if ( status == EOK ) {
            *val = ( (data & WM8731_ANALOG_PATH_CONTROL_BYPASS) ? 1 : 0 );
        }
    }

    return status;
}

int wm8731_bypass_set(wm8731_context_t* wm8731, uint32_t set)
{
    int status = EOK;
    uint32_t data;

    status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
    if ( status != EOK ) {
        return status;
    }

    if ( set ) {
        data |= WM8731_ANALOG_PATH_CONTROL_BYPASS;
    } else {
        data &= ~WM8731_ANALOG_PATH_CONTROL_BYPASS;
    }

    return wm8731_bit_set(wm8731, WM8731_ANALOG_PATH_CONTROL, data);
}


int wm8731_sidetone_get(wm8731_context_t* wm8731, uint32_t* val)
{
    int status = EOK;
    uint32_t data;

    if ( val ) {
        status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
        if ( status == EOK ) {
            *val = ( (data & WM8731_ANALOG_PATH_CONTROL_SIDETONE) ? 1 : 0 );
        }
    }

    return status;
}

int wm8731_sidetone_set(wm8731_context_t* wm8731, uint32_t set)
{
    int status = EOK;
    uint32_t data;

    status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
    if ( status != EOK ) {
        return status;
    }

    if ( set ) {
        data |= WM8731_ANALOG_PATH_CONTROL_SIDETONE;
    } else {
        data &= ~WM8731_ANALOG_PATH_CONTROL_SIDETONE;
    }

    return wm8731_bit_set(wm8731, WM8731_ANALOG_PATH_CONTROL, data);
}

int wm8731_dacsel_get(wm8731_context_t* wm8731, uint32_t* val)
{
    int status = EOK;
    uint32_t data;

    if ( val ) {
        status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
        if ( status == EOK ) {
            *val = ( (data & WM8731_ANALOG_PATH_CONTROL_DACSEL) ? 1 : 0 );
        }
    }

    return status;
}

int wm8731_dacsel_set(wm8731_context_t* wm8731, uint32_t set)
{
    int status = EOK;
    uint32_t data;

    status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &data, 0 );
    if ( status != EOK ) {
        return status;
    }

    if ( set ) {
        data |= WM8731_ANALOG_PATH_CONTROL_DACSEL;
    } else {
        data &= ~WM8731_ANALOG_PATH_CONTROL_DACSEL;
    }

    return wm8731_bit_set(wm8731, WM8731_ANALOG_PATH_CONTROL, data);
}

int wm8731_adc8k_get(wm8731_context_t* wm8731, uint32_t* val)
{
    int status = EOK;
    uint32_t data;

    if ( val ) {

        status = wm8731_i2c_read ( wm8731, WM8731_SAMPLING_CONTROL, &data, 0 );
        if ( status == EOK ) {
            *val = ( (data == WM8731_SAMPLING_CONTROL_8K) ? 1 : 0 );
        }
    }

    return status;
}

int wm8731_adc8k_set(wm8731_context_t* wm8731, uint32_t adc8k)
{
    int status = EOK;
    uint32_t data;

    status = wm8731_i2c_read ( wm8731, WM8731_SAMPLING_CONTROL, &data, 0 );
    if ( status != EOK ) {
        return status;
    }

    if ( adc8k ) {
        data |= WM8731_SAMPLING_CONTROL_8K;
    } else {
        data |= WM8731_SAMPLING_CONTROL_48K;
    }

    return wm8731_bit_set(wm8731, WM8731_SAMPLING_CONTROL, data);
}

int wm8731_rate_setting(wm8731_context_t* wm8731, uint32_t sample_rate)
{
    uint32_t data;
    uint32_t sr = WM8731_SAMPLING_CONTROL_48K;
    int status;


    if ( sample_rate == 0 ) {
        return EINVAL;
    }

    switch ( sample_rate ) {
        case 8000:
            sr = WM8731_SAMPLING_CONTROL_8K;
            break;
        case 32000:
            sr = WM8731_SAMPLING_CONTROL_32K;
            break;
        case 44100:
            sr = WM8731_SAMPLING_CONTROL_441K;
            break;
        case 48000:
            sr = WM8731_SAMPLING_CONTROL_48K;
            break;
        case 88200:
            sr = WM8731_SAMPLING_CONTROL_882K;
            break;
        case 96000:
            sr = WM8731_SAMPLING_CONTROL_96K;
            break;
        default:
            return EINVAL;
    }

    status = wm8731_i2c_read ( wm8731, WM8731_SAMPLING_CONTROL, &data, 0 );
    if ( status == EOK ) {
        data &= WM8731_SAMPLING_CONTROL_SR_MASK;
        data |= sr;
        status = wm8731_bit_set(wm8731, WM8731_SAMPLING_CONTROL, data);
    }

    return status;
}

int wm8731_capabilities(wm8731_context_t* wm8731, ado_mixer_dll_codec_capabilities_t *cap, int channel)
{
    if (cap == NULL) {
        return EINVAL;
    }
    memset (cap, 0, sizeof(*cap));
    cap->rates = SND_PCM_RATE_32000 | SND_PCM_RATE_44100 | SND_PCM_RATE_48000 |
        SND_PCM_RATE_88200 | SND_PCM_RATE_96000;
    cap->formats = SND_PCM_FMT_S24_4 | SND_PCM_FMT_S16;
    cap->max_voices = cap->min_voices = 2;

    return EOK;
}

void wm8731_register_dump(wm8731_context_t* wm8731)
{
    int status = EOK;
    uint32_t reg_val = 0;

    status = wm8731_i2c_read ( wm8731, WM8731_LEFT_LINE_IN, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_LEFT_LINE_IN - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_RIGHT_LINE_IN, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_RIGHT_LINE_IN - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_LEFT_HEADP_OUT, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_LEFT_HEADP_OUT - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_RIGHT_HEADP_OUT, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_RIGHT_HEADP_OUT - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_ANALOG_PATH_CONTROL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_ANALOG_PATH_CONTROL - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_DIGITAL_PATH_CONTROL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_DIGITAL_PATH_CONTROL - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_POWER_DOWN_CONTROL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_POWER_DOWN_CONTROL - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_DIGITAL_INTERFACE_FORMAT, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_DIGITAL_INTERFACE_FORMAT - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_SAMPLING_CONTROL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_SAMPLING_CONTROL - 0x%03x", reg_val );
    }

    status = wm8731_i2c_read ( wm8731, WM8731_ACTIVE_CONTROL, &reg_val, 0 );
    if ( status == EOK ) {
        ado_debug ( DB_LVL_DRIVER, "WM8731_ACTIVE_CONTROL - 0x%03x", reg_val );
    }
}

