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

#include "adv7482.h"
#include "decoder.h"

int i2c_fd = -1;
const char* adv7482_i2c_dev;
int adv7482_i2c_speed = 50000;

/* Structure for decoder register */
struct adv7482_reg_info {
    uint8_t addr;        /* i2c slave address */
    uint8_t sub_addr;    /* register address */
    uint8_t value;       /* register value */
};

struct adv7482_video_config {
    uint32_t width;
    uint32_t height;
    int interlace;
    char format[64];
    uint8_t standard;
    uint8_t pad[3];
};

const struct adv7482_reg_info Adv7482_Power_Up_Hdmi_Rx[] = {
    { ADV7482_I2C_MAP_IO, 0x00, 0x40 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_EOR, 0x00 }  /* End of register table */
};

const struct adv7482_reg_info Adv7482_Software_Reset[] = {
    { ADV7482_I2C_MAP_IO, 0xFF, 0xFF },  /* Apply main I2C reset */
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x06 },  /* delay 6ms */
    { ADV7482_I2C_MAP_IO, 0x01, 0x76 },
    { ADV7482_I2C_MAP_IO, 0x05, 0x53 },
    { ADV7482_I2C_MAP_IO, 0xF2, 0x01 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_EOR, 0x00 }   /* End of register table */
};

const struct adv7482_reg_info Adv7482_Hdmi_to_Mipi_Txa_Csi4[] = {
    { ADV7482_I2C_MAP_IO, 0x00, 0x30 },  /* Power down HDMI Rx */
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x05 }, /* Delay 5ms */
    { ADV7482_I2C_MAP_IO, 0xF3, 0x4C },  /* DPLL Map Address Set to 0x4C */
    { ADV7482_I2C_MAP_IO, 0xF4, 0x44 },  /* CP Map Address Set to 0x44 */
    { ADV7482_I2C_MAP_IO, 0xF5, 0x68 },  /* HDMI RX Map Address Set to 0x68 */
    { ADV7482_I2C_MAP_IO, 0xF6, 0x6C },  /* EDID Map Address Set to 0x6C */
    { ADV7482_I2C_MAP_IO, 0xF7, 0x64 },  /* HDMI RX Repeater Map Address Set to 0x64 */
    { ADV7482_I2C_MAP_IO, 0xF8, 0x62 },  /* HDMI RX Infoframe Map Address Set to 0x62 */
    { ADV7482_I2C_MAP_IO, 0xF9, 0xF0 },  /* CBUS Map Address Set to 0xF0 */
    { ADV7482_I2C_MAP_IO, 0xFA, 0x82 },  /* CEC Map Address Set to 0x82 */
    { ADV7482_I2C_MAP_IO, 0xFB, 0xF2 },  /* SDP Main Map Address Set to 0xF2 */
    { ADV7482_I2C_MAP_IO, 0xFC, 0x90 },  /* CSI-TXB Map Address Set to 0x90 */
    { ADV7482_I2C_MAP_IO, 0xFD, 0x94 },  /* CSI-TXA Map Address Set to 0x94 */
    { ADV7482_I2C_MAP_IO, 0x00, 0x40 },  /* Enable HDMI Rx & Power up HDMI Rx */
    { ADV7482_I2C_MAP_REPEATER, 0x40, 0x83 },
    { ADV7482_I2C_MAP_HDMI, 0x00, 0x08 },
    { ADV7482_I2C_MAP_HDMI, 0x98, 0xFF },
    { ADV7482_I2C_MAP_HDMI, 0x99, 0xA3 },
    { ADV7482_I2C_MAP_HDMI, 0x9A, 0x00 },
    { ADV7482_I2C_MAP_HDMI, 0x9B, 0x0A },
    { ADV7482_I2C_MAP_HDMI, 0x9D, 0x40 },
    { ADV7482_I2C_MAP_HDMI, 0xCB, 0x09 },
    { ADV7482_I2C_MAP_HDMI, 0x3D, 0x10 },
    { ADV7482_I2C_MAP_HDMI, 0x3E, 0x7B },
    { ADV7482_I2C_MAP_HDMI, 0x3F, 0x5E },
    { ADV7482_I2C_MAP_HDMI, 0x4E, 0xFE },
    { ADV7482_I2C_MAP_HDMI, 0x4F, 0x18 },
    { ADV7482_I2C_MAP_HDMI, 0x57, 0xA3 },
    { ADV7482_I2C_MAP_HDMI, 0x58, 0x04 },
    { ADV7482_I2C_MAP_HDMI, 0x85, 0x10 },
    { ADV7482_I2C_MAP_HDMI, 0x83, 0x00 },
    { ADV7482_I2C_MAP_HDMI, 0xA3, 0x01 },
    { ADV7482_I2C_MAP_HDMI, 0xBE, 0x00 },
    { ADV7482_I2C_MAP_HDMI, 0x6C, 0x01 },
    { ADV7482_I2C_MAP_HDMI, 0xF8, 0x01 },
    { ADV7482_I2C_MAP_HDMI, 0x0F, 0x00 },
    { ADV7482_I2C_MAP_IO, 0x04, 0x02 },
    { ADV7482_I2C_MAP_IO, 0x12, 0xF0 },
    { ADV7482_I2C_MAP_IO, 0x17, 0x80 },
    { ADV7482_I2C_MAP_IO, 0x03, 0x86 },
    { ADV7482_I2C_MAP_CP, 0x7C, 0x00 },
    { ADV7482_I2C_MAP_IO, 0x0C, 0xE0 },
    { ADV7482_I2C_MAP_IO, 0x0E, 0xDD },
    { ADV7482_I2C_MAP_IO, 0x10, 0xA0 },
    { ADV7482_I2C_MAP_TXA, 0x00, 0x84 },
    { ADV7482_I2C_MAP_TXA, 0x00, 0xA4 },
    { ADV7482_I2C_MAP_TXA, 0xDB, 0x10 },
    { ADV7482_I2C_MAP_TXA, 0xD6, 0x07 },
    { ADV7482_I2C_MAP_TXA, 0xC4, 0x0A },
    { ADV7482_I2C_MAP_TXA, 0x71, 0x33 },
    { ADV7482_I2C_MAP_TXA, 0x72, 0x11 },
    { ADV7482_I2C_MAP_TXA, 0xF0, 0x00 },
    { ADV7482_I2C_MAP_TXA, 0x31, 0x82 },
    { ADV7482_I2C_MAP_TXA, 0x1E, 0x40 },
    { ADV7482_I2C_MAP_TXA, 0xDA, 0x01 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x02 },  /* delay 2ms */
    { ADV7482_I2C_MAP_TXA, 0x00, 0x24 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x01 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXA, 0xC1, 0x2B },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x01 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXA, 0x31, 0x80 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_EOR, 0x00 }   /* End of register table */
};

const struct adv7482_reg_info Adv7482_Hdmi_to_Mipi_Txa_Csi2[] = {
    { ADV7482_I2C_MAP_IO, 0x00, 0x30 },  /* Power up HDMI Rx */
    { ADV7482_I2C_MAP_IO, 0xF3, 0x4C },  /* DPLL Map Address Set to 0x4C */
    { ADV7482_I2C_MAP_IO, 0xF4, 0x44 },  /* CP Map Address Set to 0x44 */
    { ADV7482_I2C_MAP_IO, 0xF5, 0x68 },  /* HDMI RX Map Address Set to 0x68 */
    { ADV7482_I2C_MAP_IO, 0xF6, 0x6C },  /* EDID Map Address Set to 0x6C */
    { ADV7482_I2C_MAP_IO, 0xF7, 0x64 },  /* HDMI RX Repeater Map Address Set to 0x64 */
    { ADV7482_I2C_MAP_IO, 0xF8, 0x62 },  /* HDMI RX Infoframe Map Address Set to 0x62 */
    { ADV7482_I2C_MAP_IO, 0xF9, 0xF0 },  /* CBUS Map Address Set to 0xF0 */
    { ADV7482_I2C_MAP_IO, 0xFA, 0x82 },  /* CEC Map Address Set to 0x82 */
    { ADV7482_I2C_MAP_IO, 0xFB, 0xF2 },  /* SDP Main Map Address Set to 0xF2 */
    { ADV7482_I2C_MAP_IO, 0xFC, 0x90 },  /* CSI-TXB Map Address Set to 0x90 */
    { ADV7482_I2C_MAP_IO, 0xFD, 0x94 },  /* CSI-TXA Map Address Set to 0x94 */
    { ADV7482_I2C_MAP_IO, 0x00, 0x40 },  /* Enable HDMI Rx & Power up HDMI Rx */
    { ADV7482_I2C_MAP_REPEATER, 0x40, 0x83 },
    { ADV7482_I2C_MAP_HDMI, 0x00, 0x08 },
    { ADV7482_I2C_MAP_HDMI, 0x98, 0xFF },
    { ADV7482_I2C_MAP_HDMI, 0x99, 0xA3 },
    { ADV7482_I2C_MAP_HDMI, 0x9A, 0x00 },
    { ADV7482_I2C_MAP_HDMI, 0x9B, 0x0A },
    { ADV7482_I2C_MAP_HDMI, 0x9D, 0x40 },
    { ADV7482_I2C_MAP_HDMI, 0xCB, 0x09 },
    { ADV7482_I2C_MAP_HDMI, 0x3D, 0x10 },
    { ADV7482_I2C_MAP_HDMI, 0x3E, 0x7B },
    { ADV7482_I2C_MAP_HDMI, 0x3F, 0x5E },
    { ADV7482_I2C_MAP_HDMI, 0x4E, 0xFE },
    { ADV7482_I2C_MAP_HDMI, 0x4F, 0x18 },
    { ADV7482_I2C_MAP_HDMI, 0x57, 0xA3 },
    { ADV7482_I2C_MAP_HDMI, 0x58, 0x04 },
    { ADV7482_I2C_MAP_HDMI, 0x85, 0x10 },
    { ADV7482_I2C_MAP_HDMI, 0x83, 0x00 },
    { ADV7482_I2C_MAP_HDMI, 0xA3, 0x01 },
    { ADV7482_I2C_MAP_HDMI, 0xBE, 0x00 },
    { ADV7482_I2C_MAP_HDMI, 0x6C, 0x01 },
    { ADV7482_I2C_MAP_HDMI, 0xF8, 0x01 },
    { ADV7482_I2C_MAP_HDMI, 0x0F, 0x00 },
    { ADV7482_I2C_MAP_IO, 0x00, 0x40 },
    { ADV7482_I2C_MAP_IO, 0x04, 0x02 },
    { ADV7482_I2C_MAP_IO, 0x12, 0xF0 },
    { ADV7482_I2C_MAP_IO, 0x17, 0x80 },
    { ADV7482_I2C_MAP_IO, 0x03, 0x86 },
    { ADV7482_I2C_MAP_CP, 0x7C, 0x00 },
    { ADV7482_I2C_MAP_IO, 0x0C, 0xE0 },
    { ADV7482_I2C_MAP_IO, 0x0E, 0xDD },
    { ADV7482_I2C_MAP_IO, 0x10, 0xA8 },
    { ADV7482_I2C_MAP_TXA, 0x00, 0x82 },
    { ADV7482_I2C_MAP_TXA, 0x00, 0xA2 },
    { ADV7482_I2C_MAP_TXA, 0xD2, 0x40 },
    { ADV7482_I2C_MAP_TXA, 0xC4, 0x0A },
    { ADV7482_I2C_MAP_TXA, 0x71, 0x33 },
    { ADV7482_I2C_MAP_TXA, 0x72, 0x11 },
    { ADV7482_I2C_MAP_TXA, 0xF0, 0x00 },
    { ADV7482_I2C_MAP_TXA, 0x31, 0x82 },
    { ADV7482_I2C_MAP_TXA, 0x1E, 0x40 },
    { ADV7482_I2C_MAP_TXA, 0xDA, 0x00 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x02 },  /* delay 2ms */
    { ADV7482_I2C_MAP_TXA, 0x00, 0x22 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x01 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXA, 0xC1, 0x2B },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x01 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXA, 0x31, 0x80 },

    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_EOR, 0x00 }   /* End of register table */
};

uint8_t Adv7482_Program_EDID_HDTV[] = {
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x0C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4C, 0x20, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20, 0x58, 0x2C,
    0x25, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x98, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10,
    0x10, 0x3E, 0x96, 0x00, 0x58, 0xC2, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4D,
    0x59, 0x20, 0x48, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x3B, 0x3D, 0x0F, 0x2E, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x75,
                                                                                     /* Checksum */
    /* Setting for Version 3 */
    0x02, 0x03, 0x26, 0x41, 0x41, 0x85, 0x35, 0x0F, 0x06, 0x07, 0x17, 0x1F, 0x38, 0x1F, 0x07, 0x30,
    0x2F, 0x07, 0x72, 0x3F, 0x7F, 0x72, 0x57, 0x7F, 0x00, 0x37, 0x7F, 0x72, 0x83, 0x7F, 0x00, 0x00,
    0x65, 0x03, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20, 0x58, 0x2C,
    0x25, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x98, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0,

};

uint8_t Adv7482_Program_EDID_SDTV_VGA[] = {
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x0C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0A, 0x0D, 0xC9, 0xA0, 0x57, 0x47, 0x98, 0x27,
    0x12, 0x48, 0x4C, 0x20, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E,
    0x96, 0x00, 0x58, 0xC2, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xD0, 0x90, 0x20, 0x40, 0x31, 0x20,
    0x0C, 0x40, 0x55, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x4D,
    0x59, 0x20, 0x48, 0x44, 0x54, 0x56, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x3B, 0x3D, 0x0F, 0x2E, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x6F,
                                                                                     /* Checksum */
    /* Setting for Version 3 */
    0x02, 0x01, 0x04, 0x00, 0xD6, 0x09, 0x80, 0xA0, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00,
    0x58, 0xC2, 0x21, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x52, 0x45, 0x56,
    0x31, 0x2E, 0x30, 0x33, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x39,
    0x39, 0x46, 0x43, 0x35, 0x30, 0x30, 0x30, 0x31, 0x0A, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72
};

const struct adv7482_reg_info Adv7482_Cvbs_to_Mipi_Txb_Csi1[] = {
    { ADV7482_I2C_MAP_IO, 0x00, 0x30 },  // Disable chip powerdown - powerdown Rx
    { ADV7482_I2C_MAP_IO, 0xF2, 0x01 },  // Enable I2C Read Auto-Increment
    { ADV7482_I2C_MAP_IO, 0xF3, 0x4C },  // DPLL Map Address Set to 0x4C
    { ADV7482_I2C_MAP_IO, 0xF4, 0x44 },  // CP Map Address Set to 0x44
    { ADV7482_I2C_MAP_IO, 0xF5, 0x68 },  // HDMI RX Map Address Set to 0x68
    { ADV7482_I2C_MAP_IO, 0xF6, 0x6C },  // EDID Map Address Set to 0x6C
    { ADV7482_I2C_MAP_IO, 0xF7, 0x64 },  // HDMI RX Repeater Map Address Set to 0x64
    { ADV7482_I2C_MAP_IO, 0xF8, 0x62 },  // HDMI RX Infoframe Map Address Set to 0x62
    { ADV7482_I2C_MAP_IO, 0xF9, 0xF0 },  // CBUS Map Address Set to 0xF0
    { ADV7482_I2C_MAP_IO, 0xFA, 0x82 },  // CEC Map Address Set to 0x82
    { ADV7482_I2C_MAP_IO, 0xFB, 0xF2 },  // SDP Main Map Address Set to 0xF2
    { ADV7482_I2C_MAP_IO, 0xFC, 0x90 },  // CSI-TXB Map Address Set to 0x90
    { ADV7482_I2C_MAP_IO, 0xFD, 0x94 },  // CSI-TXA Map Address Set to 0x94
    { ADV7482_I2C_MAP_IO, 0x0E, 0xFF },
    { ADV7482_I2C_MAP_SDP, 0x0F, 0x00 },
    { ADV7482_I2C_MAP_SDP, 0x52, 0xCD },
    { ADV7482_I2C_MAP_SDP, 0x00, 0x07 },
    { ADV7482_I2C_MAP_SDP, 0x0E, 0x80 },
    { ADV7482_I2C_MAP_SDP, 0x9C, 0x00 },
    { ADV7482_I2C_MAP_SDP, 0x9C, 0xFF },
    { ADV7482_I2C_MAP_SDP, 0x0E, 0x00 },
    { ADV7482_I2C_MAP_SDP, 0x80, 0x51 },
    { ADV7482_I2C_MAP_SDP, 0x81, 0x51 },
    { ADV7482_I2C_MAP_SDP, 0x82, 0x68 },
    { ADV7482_I2C_MAP_SDP, 0x03, 0x42 },
    { ADV7482_I2C_MAP_SDP, 0x04, 0xB5 },
    { ADV7482_I2C_MAP_SDP, 0x13, 0x00 },
    { ADV7482_I2C_MAP_SDP, 0x17, 0x41 },
    { ADV7482_I2C_MAP_SDP, 0x31, 0x12 },
    { ADV7482_I2C_MAP_SDP, 0xE6, 0x4F },
    { ADV7482_I2C_MAP_IO, 0x10, 0x70 },
    { ADV7482_I2C_MAP_TXB, 0x00, 0x81 },
    { ADV7482_I2C_MAP_TXB, 0x00, 0xA1 },
    { ADV7482_I2C_MAP_TXB, 0xD2, 0x40 },
    { ADV7482_I2C_MAP_TXB, 0xC4, 0x0A },
    { ADV7482_I2C_MAP_TXB, 0x71, 0x33 },
    { ADV7482_I2C_MAP_TXB, 0x72, 0x11 },
    { ADV7482_I2C_MAP_TXB, 0xF0, 0x00 },
    { ADV7482_I2C_MAP_TXB, 0x31, 0x82 },
    { ADV7482_I2C_MAP_TXB, 0x1E, 0x40 },
    { ADV7482_I2C_MAP_TXB, 0xDA, 0x01 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x02 },  /* delay 2ms */
    { ADV7482_I2C_MAP_TXB, 0x00, 0x21 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x02 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXB, 0xC1, 0x2B },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x02 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXB, 0x31, 0x80 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_EOR, 0x00 }   /* End of register table */
};

const struct adv7482_reg_info Adv7482_Cvbs_to_Mipi_Txa_Csi2[] = {
    { ADV7482_I2C_MAP_IO, 0x00, 0x30 },  // Disable chip powerdown - powerdown Rx
    { ADV7482_I2C_MAP_IO, 0xF2, 0x01 },  // Enable I2C Read Auto-Increment
    { ADV7482_I2C_MAP_IO, 0xF3, 0x4C },  // DPLL Map Address Set to 0x4C
    { ADV7482_I2C_MAP_IO, 0xF4, 0x44 },  // CP Map Address Set to 0x44
    { ADV7482_I2C_MAP_IO, 0xF5, 0x68 },  // HDMI RX Map Address Set to 0x68
    { ADV7482_I2C_MAP_IO, 0xF6, 0x6C },  // EDID Map Address Set to 0x6C
    { ADV7482_I2C_MAP_IO, 0xF7, 0x64 },  // HDMI RX Repeater Map Address Set to 0x64
    { ADV7482_I2C_MAP_IO, 0xF8, 0x62 },  // HDMI RX Infoframe Map Address Set to 0x62
    { ADV7482_I2C_MAP_IO, 0xF9, 0xF0 },  // CBUS Map Address Set to 0xF0
    { ADV7482_I2C_MAP_IO, 0xFA, 0x82 },  // CEC Map Address Set to 0x82
    { ADV7482_I2C_MAP_IO, 0xFB, 0xF2 },  // SDP Main Map Address Set to 0xF2
    { ADV7482_I2C_MAP_IO, 0xFC, 0x90 },  // CSI-TXB Map Address Set to 0x90
    { ADV7482_I2C_MAP_IO, 0xFD, 0x94 },  // CSI-TXA Map Address Set to 0x94
    { ADV7482_I2C_MAP_IO, 0x0E, 0xFF },
    { ADV7482_I2C_MAP_SDP, 0x0F, 0x00 },
    { ADV7482_I2C_MAP_SDP, 0x52, 0xCD },
    { ADV7482_I2C_MAP_SDP, 0x00, 0x07 },
    { ADV7482_I2C_MAP_SDP, 0x0E, 0x80 },
    { ADV7482_I2C_MAP_SDP, 0x9C, 0x00 },
    { ADV7482_I2C_MAP_SDP, 0x9C, 0xFF },
    { ADV7482_I2C_MAP_SDP, 0x0E, 0x00 },
    { ADV7482_I2C_MAP_SDP, 0x80, 0x51 },
    { ADV7482_I2C_MAP_SDP, 0x81, 0x51 },
    { ADV7482_I2C_MAP_SDP, 0x82, 0x68 },
    { ADV7482_I2C_MAP_SDP, 0x03, 0x42 },
    { ADV7482_I2C_MAP_SDP, 0x04, 0xB5 },
    { ADV7482_I2C_MAP_SDP, 0x13, 0x00 },
    { ADV7482_I2C_MAP_SDP, 0x17, 0x41 },
    { ADV7482_I2C_MAP_SDP, 0x31, 0x12 },
    { ADV7482_I2C_MAP_SDP, 0xE6, 0x4F },
    { ADV7482_I2C_MAP_IO, 0x10, 0xA8 },
    { ADV7482_I2C_MAP_TXA, 0x00, 0x82 },
    { ADV7482_I2C_MAP_TXA, 0x00, 0xA2 },
    { ADV7482_I2C_MAP_TXA, 0xD2, 0x40 },
    { ADV7482_I2C_MAP_TXA, 0xC4, 0x0A },
    { ADV7482_I2C_MAP_TXA, 0x71, 0x33 },
    { ADV7482_I2C_MAP_TXA, 0x72, 0x11 },
    { ADV7482_I2C_MAP_TXA, 0xF0, 0x00 },
    { ADV7482_I2C_MAP_TXA, 0x31, 0x82 },
    { ADV7482_I2C_MAP_TXA, 0x1E, 0x40 },
    { ADV7482_I2C_MAP_TXA, 0xDA, 0x00 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x02 },  /* delay 2ms */
    { ADV7482_I2C_MAP_TXA, 0x00, 0x22 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x01 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXA, 0xC1, 0x2B },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x01 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXA, 0x31, 0x80 },
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_EOR, 0x00 }   /* End of register table */
};

const struct adv7482_reg_info Adv7482_Cvbs_to_Mipi_Txa_Csi1[] = {
    { ADV7482_I2C_MAP_IO, 0x00, 0x30 },  /* Disable chip powerdown - powerdown Rx */
    { ADV7482_I2C_MAP_IO, 0xF2, 0x01 },  /* Enable I2C Read Auto-Increment */
    { ADV7482_I2C_MAP_IO, 0xF3, 0x4C },  /* DPLL Map Address Set to 0x4C */
    { ADV7482_I2C_MAP_IO, 0xF4, 0x44 },  /* CP Map Address Set to 0x44 */
    { ADV7482_I2C_MAP_IO, 0xF5, 0x68 },  /* HDMI RX Map Address Set to 0x68 */
    { ADV7482_I2C_MAP_IO, 0xF6, 0x6C },  /* EDID Map Address Set to 0x6C */
    { ADV7482_I2C_MAP_IO, 0xF7, 0x64 },  /* HDMI RX Repeater Map Address Set to 0x64 */
    { ADV7482_I2C_MAP_IO, 0xF8, 0x62 },  /* HDMI RX Infoframe Map Address Set to 0x6 */
    { ADV7482_I2C_MAP_IO, 0xF9, 0xF0 },  /* CBUS Map Address Set to 0xF0 */
    { ADV7482_I2C_MAP_IO, 0xFA, 0x82 },  /* CEC Map Address Set to 0x82 */
    { ADV7482_I2C_MAP_IO, 0xFB, 0xF2 },  /* SDP Main Map Address Set to 0xF2 */
    { ADV7482_I2C_MAP_IO, 0xFC, 0x90 },  /* CSI-TXB Map Address Set to 0x90 */
    { ADV7482_I2C_MAP_IO, 0xFD, 0x94 },  /* CSI-TXA Map Address Set to 0x94 */
    { ADV7482_I2C_MAP_IO, 0x0E, 0xFF },  
    { ADV7482_I2C_MAP_SDP, 0x0F, 0x00 },  
    { ADV7482_I2C_MAP_SDP, 0x52, 0xCD },  
    { ADV7482_I2C_MAP_SDP, 0x00, 0x07 },  /* CVBS on AIN8 */
    { ADV7482_I2C_MAP_SDP, 0x0E, 0x80 },  
    { ADV7482_I2C_MAP_SDP, 0x9C, 0x00 },  
    { ADV7482_I2C_MAP_SDP, 0x9C, 0xFF },  
    { ADV7482_I2C_MAP_SDP, 0x0E, 0x00 },  
    { ADV7482_I2C_MAP_SDP, 0x80, 0x51 },  
    { ADV7482_I2C_MAP_SDP, 0x81, 0x51 },  
    { ADV7482_I2C_MAP_SDP, 0x82, 0x68 },  
    { ADV7482_I2C_MAP_SDP, 0x03, 0x42 },  
    { ADV7482_I2C_MAP_SDP, 0x04, 0xB5 },  
    { ADV7482_I2C_MAP_SDP, 0x13, 0x00 },  
    { ADV7482_I2C_MAP_SDP, 0x17, 0x41 },  
    { ADV7482_I2C_MAP_SDP, 0x31, 0x12 },  
    { ADV7482_I2C_MAP_SDP, 0xE6, 0x4F },  
    { ADV7482_I2C_MAP_IO,  0x10, 0xB8 },   /* Enable 1-Lane MIPI Tx, enable pixel output and route SD through Pixel port */
    { ADV7482_I2C_MAP_TXA, 0x00, 0x81 },  /* Enable 1-lane MIPI */
    { ADV7482_I2C_MAP_TXA, 0x00, 0xA1 }, 
    { ADV7482_I2C_MAP_TXA, 0xDB, 0x10 },  
    { ADV7482_I2C_MAP_TXA, 0xD6, 0x07 },  
    { ADV7482_I2C_MAP_TXA, 0xC4, 0x0A },  
    { ADV7482_I2C_MAP_TXA, 0x71, 0x33 },  
    { ADV7482_I2C_MAP_TXA, 0x72, 0x11 },  
    { ADV7482_I2C_MAP_TXA, 0xF0, 0x00 },  
    { ADV7482_I2C_MAP_TXA, 0x31, 0x82 },  
    { ADV7482_I2C_MAP_TXA, 0x1E, 0x40 },  
    { ADV7482_I2C_MAP_TXA, 0xDA, 0x01 },  
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x2 },  /* delay 2ms */
    { ADV7482_I2C_MAP_TXA, 0x00, 0x24 }, 
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x01 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXA, 0xC1, 0x2B }, 
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_WAIT, 0x01 },  /* delay 1ms */
    { ADV7482_I2C_MAP_TXA, 0x31, 0x80 }, 
    { ADV7482_I2C_MAP_NOT_ADDR, ADV7482_I2C_MAP_EOR, 0x00 }   /* End of register table */
};

struct adv7482_video_config hdmi_config[] = {
    /* width   height  interlace  format       standard */
    { 640,      480,    0,         "VGA60",     ADV7482_VIDEO_STANDARD_VGA60 },
    { 720,      480,    0,         "480P",      ADV7482_VIDEO_STANDARD_480P  },
    { 720,      576,    0,         "576P",      ADV7482_VIDEO_STANDARD_576P  },
    { 1280,     720,    0,         "720P",      ADV7482_VIDEO_STANDARD_720P  },
    { 1920,     1080,   0,         "1080P",     ADV7482_VIDEO_STANDARD_1080P },
    { 1920,     1080,   1,         "1080I",     ADV7482_VIDEO_STANDARD_1080I },
    { 0,        0,      0,         "",          0                            }
};

struct adv7482_video_config cvbs_config[] = {
    /* width   height  interlace  format                            standard */
    { 720,     480,    0,         {CAPTURE_NORM_NTSC_M_J},          0 },
    { 720,     480,    0,         {CAPTURE_NORM_NTSC_4_43},         0 },
    { 720,     576,    0,         {CAPTURE_NORM_PAL_M},             0 },
    { 720,     576,    0,         {ADV7482_SDP_AD_RESULT_PAL_B_G},  0 },
    { 720,     576,    0,         {CAPTURE_NORM_PAL_COMBINATION_N}, 0 },
    { 720,     576,    0,         {CAPTURE_NORM_PAL_60},            0 },
    { 720,     576,    0,         {ADV7482_SDP_AD_RESULT_SECAM},    0 }
};

static int adv7482_i2c_read(uint8_t addr, uint8_t sub_addr, uint8_t *value)
{
    int ret = 0;
    int status = EOK;
    i2c_addr_t slave;

    struct  {
        i2c_send_t hdr;
        unsigned char bytes[8];
    } omsg;

    struct {
        i2c_recv_t hdr;
        unsigned char bytes[16];
    } imsg;

    slave.addr = addr >> 1;
    slave.fmt = I2C_ADDRFMT_7BIT;

    status = devctl(i2c_fd, DCMD_I2C_SET_SLAVE_ADDR, &slave, sizeof(slave), NULL);

    if (status != EOK) {
        fprintf(stderr, "%s: Set slave addr failed\n", __FUNCTION__);
        return -1;
    }

    omsg.hdr.slave = slave;
    omsg.hdr.len = 1;
    omsg.hdr.stop = 0;
    omsg.bytes[0] = sub_addr;

    status = devctl(i2c_fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if (status != EOK) {
        fprintf(stderr, "%s: Send failed\n", __FUNCTION__);
        return -1;
    }

    imsg.hdr.slave = slave;
    imsg.hdr.len = 1;
    imsg.hdr.stop = 1;

    status = devctl(i2c_fd, DCMD_I2C_RECV, &imsg, sizeof(imsg.hdr) + imsg.hdr.len, NULL);

    if (status != EOK) {
        fprintf(stderr, "%s: Read failed\n", __FUNCTION__);
        return -1;
    }

    *value = imsg.bytes[0];

    return ret;
}

static int adv7482_i2c_write(uint8_t addr, uint8_t sub_addr, uint8_t value)
{
    int status = EOK;
    i2c_addr_t slave;

    struct {
        i2c_send_t hdr;
        unsigned char bytes[8];
    } omsg;


    slave.addr = addr >> 1;
    slave.fmt = I2C_ADDRFMT_7BIT;

    status = devctl(i2c_fd, DCMD_I2C_SET_SLAVE_ADDR, &slave, sizeof(slave), NULL);

    if (status != EOK) {
        fprintf(stderr, "%s: Set slave addr failed\n", __FUNCTION__);
        return -1;
    }

    omsg.hdr.slave = slave;
    omsg.hdr.slave.fmt = I2C_ADDRFMT_7BIT;
    omsg.hdr.len = 2;
    omsg.hdr.stop = 1;
    omsg.bytes[0] = sub_addr;
    omsg.bytes[1] = value;

    status = devctl(i2c_fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if(status != EOK) {
        fprintf(stderr, "%s: Send failed, addr=%x, reg=%x, val=%x\n", __FUNCTION__,
                                addr, sub_addr, value);
        return -1;
    }

    return 0;
}

static int adv7482_write_table(const struct adv7482_reg_info *regs)
{
    int status = EOK;

    while (1) {
        if(regs->addr == ADV7482_I2C_MAP_NOT_ADDR) {
            if(regs->sub_addr == ADV7482_I2C_MAP_EOR)
                break;      /* End of script */
            if(regs->sub_addr == ADV7482_I2C_MAP_WAIT)
                delay(regs->value); /* Wait a moment */
        }
        else {
            status = adv7482_i2c_write(regs->addr, regs->sub_addr, regs->value);
            if(status != EOK) {
                return -1;
            }
        }

        regs++;
    }

    return 0;
}

int adv7482_power(video_info_t* video, int channel, int enable)
{
    int ret = 0;
    uint32_t lane, addr;

    if(video->board == RCAR_E3_EBISU) {
        lane = 2;
        addr = ADV7482_I2C_MAP_TXA;
    } else if(video->board == RCAR_V3M_EAGLE) {
        if(channel == 0)
            lane = 4;
        else 
            lane = 1;
        addr = ADV7482_I2C_MAP_TXA;
    } 
    else {
        if(channel == 0) {
            lane = 4;
            addr = ADV7482_I2C_MAP_TXA;
        }
        else {
            lane = 1;
            addr = ADV7482_I2C_MAP_TXB;
        }
    }

    if (enable == 1) {
        ret += adv7482_i2c_write(addr, 0x00, 0x80 | lane);
        ret += adv7482_i2c_write(addr, 0x00, 0xA0 | lane);
        ret += adv7482_i2c_write(addr, 0x31, 0x82);
        ret += adv7482_i2c_write(addr, 0x1E, 0x40);
        ret += adv7482_i2c_write(addr, 0xDA, 0x01);
        delay(2);
        ret += adv7482_i2c_write(addr, 0x00, 0x20 | lane);
        delay(1);
        ret += adv7482_i2c_write(addr, 0xC1, 0x2B);
        delay(1);
        ret += adv7482_i2c_write(addr, 0x31, 0x80);
    }
    else {
        ret += adv7482_i2c_write(addr, 0x31, 0x82);
        ret += adv7482_i2c_write(addr, 0x1E, 0x00);
        ret += adv7482_i2c_write(addr, 0x00, 0x80 | lane);
        ret += adv7482_i2c_write(addr, 0xDA, 0x00);
        ret += adv7482_i2c_write(addr, 0xC1, 0x3B);
    }
    if(ret < 0) {
        fprintf(stderr, "%s: failed\n", __FUNCTION__);
    }

    return ret;
}

static int adv7482_detect_video_signal(video_info_t* video, int channel)
{
    uint8_t val;

    video->signal = 0;

    if (channel == 0) {
        /* Check cable pluged or not */
        if(adv7482_i2c_read(ADV7482_I2C_MAP_IO, 0x71, &val)) {
            return -1;
        }

        /* Exit when cable is not detected */
        if(!(val & 0x40)) {
            return -1;
        }

        /* Query input signal until signal is locked */
        while(1) {
            if(adv7482_i2c_read(ADV7482_I2C_MAP_HDMI, ADV7482_HDMI_LINE_WIDTH_1_REG, &val) < 0)
                return -1;

            if ((val & ADV7482_HDMI_VERT_FILTER_LOCKED) &&
                (val & ADV7482_HDMI_DE_REGEN_FILTER_LOCKED)) {
               break;
            }

            delay(1);
        }
    }
    else
    {
        if(adv7482_i2c_write(ADV7482_I2C_MAP_SDP, ADV7482_SDP_USR_MAP_RW_REG, ADV7482_SDP_RO_MAIN_MAP) < 0)
           return -1;

        delay(500);

        /* Detect input signal */
        if(adv7482_i2c_read(ADV7482_I2C_MAP_SDP, ADV7482_SDP_USR_MAP_R10_REG, &val)) {
            return -1;
        }

        if (!(val & ADV7482_SDP_IN_LOCK)) {
            return -1;
        }
    }

    video->signal = 1;

    return 0;
}

static void adv7482_hdmi_set_de_timings(int shift)
{
    uint8_t high, low;

    high = (1 << 6);
    high |= (shift & 0x300) >> 8;
    low = shift & 0xFF;

    adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_DE_POS_HIGH_REG, high);
    adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_DE_POS_END_LOW_REG, low);

    high |= (shift & 0x300) >> 6;

    adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_DE_POS_HIGH_REG, high);
    adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_DE_POS_START_LOW_REG, low);
}

static int adv7482_hdmi_get_video_info(video_info_t* video, uint8_t *value)
{
    uint8_t val;
    int i = 0;

    video->interlace = 0;
    video->width     = 0;
    video->width     = 0;

    /* Get active pixel per line */
    if(adv7482_i2c_read(ADV7482_I2C_MAP_HDMI, ADV7482_HDMI_LINE_WIDTH_1_REG, &val) < 0)
        return -1;

    video->width = (uint32_t)(ADV7482_HDMI_LINE_WIDTH_MSBS_MASK & val);


    if (adv7482_i2c_read(ADV7482_I2C_MAP_HDMI, ADV7482_HDMI_LINE_WIDTH_2_REG, &val) < 0)
        return -1;

    video->width = val | (video->width << 8);

    /* Get active lines per frame */
    if (adv7482_i2c_read(ADV7482_I2C_MAP_HDMI, ADV7482_HDMI_FIELD0_HEIGHT_1_REG, &val) < 0)
        return -1;

    video->height = (uint32_t)(ADV7482_HDMI_FIELD0_HEIGHT_1_MSBS_MASK & val);

    if(adv7482_i2c_read(ADV7482_I2C_MAP_HDMI, ADV7482_HDMI_FIELD0_HEIGHT_2_REG, &val) < 0)
        return -1;

    video->height = (val | (video->height << 8));

    /* Get interlaced or progressive mode */
    if ((adv7482_i2c_read(ADV7482_I2C_MAP_HDMI, ADV7482_HDMI_FIELD1_HEIGHT_1_REG, &val)) < 0)
        return -1;

    if (val & ADV7482_HDMI_INTERLACED) {
        video->interlace = 1;
        video->height *= 2;
    }

    if (video->width == 0 || video->height == 0) {
        fprintf(stderr, "Got invalid resolution(%dx%d).\n", video->width, video->height);
        return -1;
    }

    while (1) {
        if (hdmi_config[i].width == 0) {
            fprintf(stderr, "Not support resolution %dx%d%c\n", video->width, video->height, (video->interlace) ? 'i' : 'p');
            return -1;
        }
        else if ((video->width == hdmi_config[i].width) && (video->height == hdmi_config[i].height) && (video->interlace == hdmi_config[i].interlace))
            break;

        i++;
    }

    *value = hdmi_config[i].standard;
    strcpy(video->format, hdmi_config[i].format);
    
    /*
     * When setting to either 720p, 1080i, or 1080p, the video
     * will get shifted horizontally to the left in active video mode.
     * The de_h_start and de_h_end controls are used to centre the picture
     * correctly
    */
    switch (hdmi_config[i].standard) {
        case ADV7482_VIDEO_STANDARD_720P:
            adv7482_hdmi_set_de_timings(-40);
            break;
        case ADV7482_VIDEO_STANDARD_1080I: /* 1080i */
        case ADV7482_VIDEO_STANDARD_1080P: /* 1080p */
            adv7482_hdmi_set_de_timings(-44);
            break;
        default:
            adv7482_hdmi_set_de_timings(0);
            break;
    }

    return 0;
}

static int adv7482_cvbs_get_video_info(video_info_t* video)
{
    uint8_t val;
    int idx;

    video->interlace = 0;
    video->width     = 0;
    video->width     = 0;

    if (adv7482_i2c_read(ADV7482_I2C_MAP_SDP, ADV7482_SDP_USR_MAP_R10_REG, &val) < 0)
        return -1;

    idx = (val & ADV7482_SDP_USR_MAP_R10_AD_MASK) >> 4;

    strcpy(video->format, cvbs_config[idx].format);
    video->width  = cvbs_config[idx].width;
    video->height = cvbs_config[idx].height;
    video->interlace = 1;

    return 0;
}

static int adv7482_cp_color_setting(video_info_t* video)
{
    uint8_t val;

    /* Enable color controls */
    if(adv7482_i2c_read(ADV7482_I2C_MAP_CP, ADV7482_CP_VID_ADJ_0_REG, &val) < 0)
        return -1;

    if(adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_VID_ADJ_0_REG, val | ADV7482_CP_VID_ADJ_EN) < 0)
        return -1;

    /* Brightness */
    if(video->update & DECODER_COLOR_BRI_UPDATE) {
        if ((video->bri < ADV7482_CP_BRIGHTNEST_MINIMUM) || (video->bri > ADV7482_CP_BRIGHTNEST_MAXIMUM))
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_BRIGHTNEST_CTRL_REG, video->bri) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_BRI_UPDATE;
    }

    /* Hue */
    if(video->update & DECODER_COLOR_HUE_UPDATE) {
        if ((video->hue < ADV7482_CP_HUE_MINIMUM) || (video->hue > ADV7482_CP_HUE_MAXIMUM))
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_HUE_CTRL_REG, video->hue) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_HUE_UPDATE;
    }

    /* Contrast */
    if(video->update & DECODER_COLOR_CON_UPDATE) {
        if ((video->con < ADV7482_CP_CONTRACT_MINIMUM) || (video->con > ADV7482_CP_CONTRACT_MAXIMUM))
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_CONTRACT_CTRL_REG, video->con) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_CON_UPDATE;
    }

    /* Saturation */
    if(video->update & DECODER_COLOR_SAT_UPDATE) {
        if ((video->sat < ADV7482_CP_SATURATION_MINIMUM) || (video->sat > ADV7482_CP_SATURATION_MAXIMUM))
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_CP, ADV7482_CP_SATURATION_CTRL_REG, video->sat) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_SAT_UPDATE;
    }

    return 0;
}

static int adv7482_sdp_color_setting(video_info_t* video)
{
    if((adv7482_i2c_write(ADV7482_I2C_MAP_SDP, ADV7482_SDP_USR_MAP_RW_REG, ADV7482_SDP_MAP_RW)) < 0)
        return -1;

    /* Brightness */
    if(video->update & DECODER_COLOR_BRI_UPDATE) {
        if ((video->bri < ADV7482_SDP_BRIGHTNESS_MINIMUM) || (video->bri > ADV7482_SDP_BRIGHTNESS_MAXIMUM))
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_SDP, ADV7482_SDP_BRIGHTNESS_CTRL_REG, video->bri) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_BRI_UPDATE;
    }

    /* Hue */
    if(video->update & DECODER_COLOR_HUE_UPDATE) {
        if ((video->hue < ADV7482_SDP_HUE_MINIMUM) || (video->hue > ADV7482_SDP_HUE_MAXIMUM))
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_SDP, ADV7482_SDP_HUE_CTRL_REG, video->hue) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_HUE_UPDATE;
    }

    /* Contrast */
    if(video->update & DECODER_COLOR_CON_UPDATE) {
        if ((video->con < ADV7482_SDP_CONTRACT_MINIMUM) || (video->con > ADV7482_SDP_CONTRACT_MAXIMUM))
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_SDP, ADV7482_SDP_CONTRACT_CTRL_REG, video->con) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_CON_UPDATE;
    }

    /* Saturation */
    if(video->update & DECODER_COLOR_SAT_UPDATE) {
        if ((video->sat < ADV7482_SDP_SATURATION_MINIMUM) || (video->sat > ADV7482_SDP_SATURATION_MAXIMUM))
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_SDP, ADV7482_SDP_SD_SATURATION_U_REG, video->sat) < 0)
            return -1;

        if (adv7482_i2c_write(ADV7482_I2C_MAP_SDP, ADV7482_SDP_SD_SATURATION_V_REG, video->sat) < 0)
            return -1;

        video->update &= ~DECODER_COLOR_SAT_UPDATE;
    }

    return 0;
}

int adv7482_update(video_info_t *video, int channel)
{
    int ret = 0;

    if(video->update) {
        if(channel == 0) {
            ret = adv7482_cp_color_setting(video);
        }
        else {
            ret = adv7482_sdp_color_setting(video);
        }
    }

    if(ret < 0) {
        fprintf(stderr, "%s: failed, flags=%x\n", __FUNCTION__, video->update);
    }

    return ret;
}

int adv7482_init(video_info_t *video, int channel)
{
    int ret = 0;
    int status;
    uint8_t value = 0;

    if((video->board == RCAR_E3_EBISU) || (video->board == RCAR_V3M_EAGLE)) {
        adv7482_i2c_dev = "/dev/i2c0";
    } else {
        adv7482_i2c_dev = "/dev/i2c4";
    }
    
    if(i2c_fd == -1) {
        i2c_fd = open(adv7482_i2c_dev, O_RDWR);
    }

    if (i2c_fd < 0) {
        fprintf(stderr, "%s: Open device failed\n", __FUNCTION__);
        return -1;
    }

    status = devctl(i2c_fd, DCMD_I2C_SET_BUS_SPEED, &adv7482_i2c_speed,
                sizeof(adv7482_i2c_speed), NULL);

    if (status != EOK) {
        fprintf(stderr, "%s: Set bus speed failed\n", __FUNCTION__);
        return -1;
    }

    /* Software reset */
    adv7482_write_table(&Adv7482_Software_Reset[0]);
    if(video->board == RCAR_E3_EBISU) {
        adv7482_write_table(&Adv7482_Hdmi_to_Mipi_Txa_Csi2[0]);
        adv7482_write_table(&Adv7482_Cvbs_to_Mipi_Txa_Csi2[0]);
    } else if(video->board == RCAR_V3M_EAGLE) {
        adv7482_write_table(&Adv7482_Hdmi_to_Mipi_Txa_Csi4[0]);
        adv7482_write_table(&Adv7482_Cvbs_to_Mipi_Txa_Csi1[0]);
    }
    else {
        adv7482_write_table(&Adv7482_Hdmi_to_Mipi_Txa_Csi4[0]);
        adv7482_write_table(&Adv7482_Cvbs_to_Mipi_Txb_Csi1[0]);
    }

    /* Powerup Hdmi Rx */
    adv7482_power(video, 1, 0);
    adv7482_write_table(&Adv7482_Power_Up_Hdmi_Rx[0]);
    adv7482_i2c_write(ADV7482_I2C_MAP_IO, 0x10, 0xE0);
   
    adv7482_i2c_write(ADV7482_I2C_MAP_IO, 0x10, 0xE0 | (channel == 0 ? 0 : (1 <<3)));

    /* Program EDID */
    adv7482_i2c_write(ADV7482_I2C_MAP_REPEATER, 0x74, 0x00);
    if(video->source_std == VIN_INPUT_STD_HDTV) {
        for(int i = 0; i < 256; i++) {
            adv7482_i2c_write(ADV7482_I2C_MAP_EDID, 0x0 + i, Adv7482_Program_EDID_HDTV[i]);
        }
    }
    else {
        for(int i = 0; i < 256; i++) {
            adv7482_i2c_write(ADV7482_I2C_MAP_EDID, 0x0 + i, Adv7482_Program_EDID_SDTV_VGA[i]);
        }
    }
    adv7482_i2c_write(ADV7482_I2C_MAP_REPEATER, 0x74, 0x01);

    /* Enable HDMI audio output */
    if(channel == 0) {
        adv7482_i2c_write(ADV7482_I2C_MAP_IO, 0x0E, 0xDD);
        adv7482_i2c_write(ADV7482_I2C_MAP_HDMI, 0x03, 0x30);
    }

    /* Power up */
    adv7482_power(video, channel, 1);

    /* Detect video signal */
    if (adv7482_detect_video_signal(video, channel) < 0) {
        fprintf(stderr, "Unable to detect video signal.\n");
        return -1;
    }

    /* Get video information */
    if(channel == 0) {
        ret = adv7482_hdmi_get_video_info(video, &value);
    }
    else {
        ret = adv7482_cvbs_get_video_info(video);
    }

    /* Check video signal */
    if(ret < 0) {
        fprintf(stderr, "Unable to get video info.\n");
        return -1;
    }

    fprintf(stdout, "Detected Resolution: %s\n", video->format);

    if(channel == 0) {
        /* Freerun setting */
        adv7482_i2c_write(ADV7482_I2C_MAP_CP, 0xBA, 0x03);
        adv7482_i2c_write(ADV7482_I2C_MAP_CP, 0xC9, 0x2D);
        adv7482_i2c_write(ADV7482_I2C_MAP_CP, 0xF3, 0xDE);
        adv7482_i2c_write(ADV7482_I2C_MAP_IO, 0x05, value);

        /* Enable free run */
        adv7482_i2c_write(ADV7482_I2C_MAP_IO, 0x03, 0x86);
    }

    /* Decoder will be powered up after CSI had been enabled */
    adv7482_power(video, channel, 0);

    return ret;
}

int adv7482_fini(video_info_t* video, int channel)
{
    /* Software reset */
    adv7482_write_table(&Adv7482_Software_Reset[0]);

    if(!(i2c_fd < 0)) {
        close(i2c_fd);
    }
      
    return 0;
}
