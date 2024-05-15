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

#include "adv7180.h"

int i2c_fd = -1;
const char* adv7180_i2c_dev = "/dev/i2c0";

int adv7180_i2c_speed = 100000; /* I2C bus speed */

struct adv7180_video_std {
    uint32_t width;
    uint32_t height;
    char format[64];
};

struct adv7180_video_std input_std[] = {
    /* width   height  format                            */
    { 720,     480,    {CAPTURE_NORM_NTSC_M_J}           },  
    { 720,     480,    {CAPTURE_NORM_NTSC_4_43}          },
    { 720,     576,    {CAPTURE_NORM_PAL_M},             }, 
    { 720,     576,    {CAPTURE_NORM_PAL_60}             },       
    { 720,     576,    {CAPTURE_NORM_PAL_B_G_H_I_D}      },
    { 720,     576,    {CAPTURE_NORM_SECAM}              },
    { 720,     576,    {CAPTURE_NORM_PAL_COMBINATION_N}  },
    { 720,     576,    {CAPTURE_NORM_SECAM}              }
};  

static int adv7180_i2c_read(uint8_t addr, uint8_t *value)
{
    int status = EOK;
    iov_t siov[2], riov[2];
    i2c_sendrecv_t hdr; 

    hdr.slave.addr = ADV7180_I2C_IO;
    hdr.slave.fmt = I2C_ADDRFMT_7BIT;
    hdr.stop = 1;

    hdr.send_len = 1;
    hdr.recv_len = 1;

    SETIOV(&siov[0], &hdr, sizeof(hdr) );
    SETIOV(&siov[1], &addr, 1 );

    SETIOV(&riov[0], &hdr, sizeof(hdr) );
    SETIOV(&riov[1], value, 1 );

    status = devctlv(i2c_fd, DCMD_I2C_SENDRECV, 2, 2, siov, riov, NULL);

    if (status != EOK) {
        errno = status;
        perror("I2C_SEND read data");
        return -1;
    }

    return 0;
}

static int adv7180_i2c_write(uint8_t addr, uint8_t value)
{
    int status = EOK;

    struct {
        i2c_send_t hdr;     
        unsigned char bytes[8]; 
    } omsg;
    
    omsg.hdr.slave.addr = ADV7180_I2C_IO;
    omsg.hdr.slave.fmt = I2C_ADDRFMT_7BIT;
    omsg.hdr.len = 2;
    omsg.hdr.stop = 1;
    omsg.bytes[0] = addr;
    omsg.bytes[1] = value;

    status = devctl(i2c_fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if(status != EOK) {
        fprintf(stderr, "%s: Send failed, addr=%x, val=%x\n", __FUNCTION__, 
                                addr, value);
        return -1;
    }   
    
    return 0;
}

int adv7180_power(video_info_t *video, int channel, int enable) {

    int ret = 0;
    if(enable == 1)
        ret += adv7180_i2c_write(ADV7180_PWR_MAN_REG, ADV7180_PWR_MAN_UP);
    else 
        ret += adv7180_i2c_write(ADV7180_PWR_MAN_REG, ADV7180_PWR_MAN_DOWN);

    if(ret < 0) {
        fprintf(stderr, "%s: failed\n", __FUNCTION__);
    }

    return ret;
}

static int adv7180_get_video_info(video_info_t* video)
{
    uint8_t val;
    int idx;

    /* Detect input signal */
    if(adv7180_i2c_read(ADV7180_STAT1_REG, &val) < 0)
        return -1;

    video->signal = 0;
    video->interlace = 0;

    if (!(val & ADV7180_STAT1_IN_LOCK)) {
        return -1;
    }
    video->signal = 1;

    idx = (val & ADV7180_STAT1_AD_RESULT_MASK) >> 4;

    strcpy(video->format, input_std[idx].format);
    video->width  = input_std[idx].width;
    video->height = input_std[idx].height;

    /* Detect interlaced video mode*/
    if(adv7180_i2c_read(ADV7180_STAT3_REG, &val) < 0)
        return -1;

    if(val && ADV7180_STAT3_INTERLACED)
        video->interlace = 1;

    fprintf(stdout, "Detected Format: %s\n", video->format);
    
    return 0;
}

int adv7180_update(video_info_t* video, int channel)
{
    /* Contrast */
    if(video->update & DECODER_COLOR_CON_UPDATE) {
        if ((video->con < ADV7180_CONTRACT_MINIMUM) || (video->con > ADV7180_CONTRACT_MAXIMUM))
            return -1;

        if (adv7180_i2c_write(ADV7180_CONTRACT_CTRL_REG, video->con) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_CON_UPDATE;
    }

    /* Brightness */
    if(video->update & DECODER_COLOR_BRI_UPDATE) {
        if ((video->bri < ADV7180_BRIGHTNESS_MINIMUM) || (video->bri > ADV7180_BRIGHTNESS_MAXIMUM))
            return -1;

        if (adv7180_i2c_write(ADV7180_BRIGHTNESS_CTRL_REG, video->bri) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_BRI_UPDATE;
    }

    /* Hue */
    if(video->update & DECODER_COLOR_HUE_UPDATE) {
        if ((video->hue < ADV7180_HUE_MINIMUM) || (video->hue > ADV7180_HUE_MAXIMUM))
            return -1;

        if (adv7180_i2c_write(ADV7180_HUE_CTRL_REG, video->hue) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_HUE_UPDATE;
    }

    /* Saturation */
    if(video->update & DECODER_COLOR_SAT_UPDATE) {
        if ((video->sat < ADV7180_SATURATION_MINIMUM) || (video->sat > ADV7180_SATURATION_MAXIMUM))
            return -1;

        if (adv7180_i2c_write(ADV7180_SATURATION_CB_REG, video->sat) < 0)
            return -1;

        if (adv7180_i2c_write(ADV7180_SATURATION_CR_REG, video->sat) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_SAT_UPDATE;
    }

    return 0;
}

int adv7180_init(video_info_t* video, int channel)
{
    int status = -1;

    if(i2c_fd == -1) {
        i2c_fd = open(adv7180_i2c_dev, O_RDWR);
    }
    if (i2c_fd < 0) {
        fprintf(stderr, "%s: adv7180_init: open i2c \n", __FUNCTION__);
        return -1;
    }
    status = devctl(i2c_fd, DCMD_I2C_SET_BUS_SPEED, &adv7180_i2c_speed,
            sizeof(adv7180_i2c_speed), NULL);
    if (status != EOK) {
        fprintf(stderr, "%s: Set bus speed failed\n", __FUNCTION__);
        return -1;
    }

    adv7180_i2c_write(ADV7180_PWR_MAN_REG, 0x04);    // Bit has priority (pin disregarded)
    adv7180_i2c_write(ADV7180_OUTPUT_CONTROL_REG, 0x0C);     //8-bit at LLC 4:2:2 ITU-R BT.656  
                                
    /* Power up */
    adv7180_power(video, channel, ADV7180_POWER_ON);

    /* Select CVBS input on AIN1 */
    if (adv7180_i2c_write(ADV7180_INPUT_CONTROL_REG, 0x0) < 0)
        return -1;

    adv7180_i2c_write(0x17, 41);
    adv7180_i2c_write(0x52, 0x0B);

    /* Get video information */
    adv7180_get_video_info(video);

    /* Check video signal */
    if(video->signal == 0) {
        fprintf(stderr, "No cable connected.\n");
        return -1;
    }

    return 0;
}

int adv7180_fini(video_info_t* video, int channel)
{
    if(!(i2c_fd < 0)) {
        close(i2c_fd);
    }
    
    return 0;
}