/*
* $QNXLicenseC:
* Copyright 2020, QNX Software Systems.
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

#include "rcar_csi2.h"

#define BITMASK(x,y)        (((1ULL << (y-x)) - 1) << x)
#define PHY_MODE_DPHY       0
#define PHY_MODE_CPHY       1

#define RCAR_CSI2_PHY_CLK_MONITOR(_id)          ((_id) == PRODUCT_ID_RCAR_V4H ?  RCAR_GEN4_CSI2_ST_PHYST : RCAR_CSI2_PHCLM)
#define RCAR_CSI2_PHY_DATA_MONITOR(_id)         ((_id) == PRODUCT_ID_RCAR_V4H ?  RCAR_GEN4_CSI2_ST_PHYST : RCAR_CSI2_PHDLM)
#define RCAR_CSI2_LANE_STOPSTATE(_id, n)        ((_id) == PRODUCT_ID_RCAR_V4H ?  BITMASK(0, n) : 0x01)

/* CSI2 bandwidth */
enum rcar_csi2_freq {
    CSI2_FREQ_80MBPS,
    CSI2_FREQ_90MBPS,
    CSI2_FREQ_100MBPS,
    CSI2_FREQ_110MBPS,
    CSI2_FREQ_120MBPS,
    CSI2_FREQ_130MBPS,
    CSI2_FREQ_140MBPS,
    CSI2_FREQ_150MBPS,
    CSI2_FREQ_160MBPS,
    CSI2_FREQ_170MBPS,
    CSI2_FREQ_180MBPS,
    CSI2_FREQ_190MBPS,
    CSI2_FREQ_200MBPS,
    CSI2_FREQ_205MBPS,
    CSI2_FREQ_220MBPS,
    CSI2_FREQ_235MBPS,
    CSI2_FREQ_240MBPS,
    CSI2_FREQ_250MBPS,
    CSI2_FREQ_270MBPS, 
    CSI2_FREQ_275MBPS,
    CSI2_FREQ_300MBPS,
    CSI2_FREQ_325MBPS,
    CSI2_FREQ_330MBPS,
    CSI2_FREQ_350MBPS,
    CSI2_FREQ_360MBPS,
    CSI2_FREQ_400MBPS,
    CSI2_FREQ_450MBPS,
    CSI2_FREQ_500MBPS,
    CSI2_FREQ_550MBPS,
    CSI2_FREQ_600MBPS,
    CSI2_FREQ_650MBPS,
    CSI2_FREQ_700MBPS,
    CSI2_FREQ_750MBPS,
    CSI2_FREQ_800MBPS,
    CSI2_FREQ_850MBPS,
    CSI2_FREQ_900MBPS,
    CSI2_FREQ_950MBPS,
    CSI2_FREQ_1000MBPS,
    CSI2_FREQ_1050MBPS,
    CSI2_FREQ_1100MBPS,
    CSI2_FREQ_1125MBPS,
    CSI2_FREQ_1150MBPS,
    CSI2_FREQ_1200MBPS,
    CSI2_FREQ_1250MBPS,
    CSI2_FREQ_1300MBPS,
    CSI2_FREQ_1350MBPS,
    CSI2_FREQ_1400MBPS,
    CSI2_FREQ_1450MBPS,
    CSI2_FREQ_1500MBPS,
    CSI2_FREQ_1550MBPS,
    CSI2_FREQ_1600MBPS,
    CSI2_FREQ_1650MBPS,
    CSI2_FREQ_1700MBPS,
    CSI2_FREQ_1750MBPS,
    CSI2_FREQ_1800MBPS,
    CSI2_FREQ_1850MBPS,
    CSI2_FREQ_1900MBPS,
    CSI2_FREQ_1950MBPS,
    CSI2_FREQ_2000MBPS,
    CSI2_FREQ_2050MBPS,
    CSI2_FREQ_2100MBPS,
    CSI2_FREQ_2150MBPS,
    CSI2_FREQ_2200MBPS,
    CSI2_FREQ_2300MBPS,
    CSI2_FREQ_2350MBPS,
    CSI2_FREQ_2400MBPS,
    CSI2_FREQ_2450MBPS,
    CSI2_FREQ_2500MBPS,
    CSI2_FREQ_MAX
};

uint16_t csi2_freqs_h3_v3h_m3n[CSI2_FREQ_MAX] = {
    0x00, 0x10, 0x20, 0x30, 0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x22, 0x32, 0xff, 0x03, 0x13, 0x23, 
    0xff, 0x33, 0xff, 0x04, 0x14, 0x25, 0xff, 0x35, 0xff, 0x05, 0x26, 0x36, 0x37, 0x07, 0x18, 0x28, 
    0x39, 0x09, 0x19, 0x29, 0x3a, 0x0a, 0x1a, 0x2a, 0xff, 0x3b, 0x0b, 0x1b, 0x2b, 0x3c, 0x0c, 0x1c, 
    0x2c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff
};

uint16_t csi2_freqs_m3w_h3es1[CSI2_FREQ_MAX] = {
    0x00, 0x10, 0x20, 0x30, 0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x22, 0x32, 0xff, 0x03, 0x13, 0x23,
    0xff, 0x33, 0xff, 0x04, 0x14, 0x05, 0xff, 0x15, 0xff, 0x25, 0x06, 0x16, 0x07, 0x17, 0x08, 0x18,
    0x09, 0x19, 0x29, 0x39, 0x0a, 0x1a, 0x2a, 0x3a, 0xff, 0x0b, 0x1b, 0x2b, 0x3b, 0x0c, 0x1c, 0x2c,
    0x3c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff
};

uint16_t csi2_freqs_v3m_e3[CSI2_FREQ_MAX] = {
    0x00, 0x20, 0x40, 0x02, 0xff, 0x22, 0x42, 0x04, 0xff, 0x24, 0x44, 0xff, 0x06, 0xff, 0x26, 0xff,
    0x46, 0x08, 0x28, 0xff, 0x0a, 0xff, 0x2a, 0xff, 0x4a, 0x0c, 0x2c, 0x0e, 0x2e, 0x10, 0x30, 0x12,
    0x32, 0x52, 0x72, 0x14, 0x34, 0x54, 0x74, 0xff, 0x16, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff
};

uint16_t csi2_freqs_v3u[CSI2_FREQ_MAX] = {
    0x00, 0x10, 0x20, 0x30, 0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x22, 0x32, 0xff, 0x03, 0x13, 0x23,
    0xff, 0x33, 0xff, 0x04, 0x14, 0x25, 0xff, 0x35, 0xff, 0x05, 0x16, 0x26, 0x37, 0x07, 0x18, 0x28,
    0x39, 0x09, 0x19, 0x29, 0x3a, 0x0a, 0x1a, 0x2a, 0xff, 0x3b, 0x0b, 0x1b, 0x2b, 0x3c, 0x0c, 0x1c,
    0x2c, 0x3d, 0x0d, 0x1d, 0x2e, 0x3e, 0x0e, 0x1e, 0x2f, 0x3f, 0x0f, 0x40, 0x41, 0x42, 0x43, 0x45,
    0x46, 0x47, 0x48, 0x49
};

/* V4H Static Register Access */
uint16_t deskew_fine_mem[] = {
    0x0404, 0x040C, 0x0414, 0x041C, 0x0423, 0x0429, 0x0430, 0x043A, 0x0445, 0x044A, 0x0450, 0x045A, 0x0465, 0x0469, 0x0472, 0x047A,
    0x0485, 0x0489, 0x0490, 0x049A, 0x04A4, 0x04AC, 0x04B4, 0x04BC, 0x04C4, 0x04CC, 0x04D4, 0x04DC, 0x04E4, 0x04EC, 0x04F4, 0x04FC,
    0x0504, 0x050C, 0x0514, 0x051C, 0x0523, 0x0529, 0x0530, 0x053A, 0x0545, 0x054A, 0x0550, 0x055A, 0x0565, 0x0569, 0x0572, 0x057A,
    0x0585, 0x0589, 0x0590, 0x059A, 0x05A4, 0x05AC, 0x05B4, 0x05BC, 0x05C4, 0x05CC, 0x05D4, 0x05DC, 0x05E4, 0x05EC, 0x05F4, 0x05FC,
    0x0604, 0x060C, 0x0614, 0x061C, 0x0623, 0x0629, 0x0632, 0x063A, 0x0645, 0x064A, 0x0650, 0x065A, 0x0665, 0x0669, 0x0672, 0x067A,
    0x0685, 0x0689, 0x0690, 0x069A, 0x06A4, 0x06AC, 0x06B4, 0x06BC, 0x06C4, 0x06CC, 0x06D4, 0x06DC, 0x06E4, 0x06EC, 0x06F4, 0x06FC,
    0x0704, 0x070C, 0x0714, 0x071C, 0x0723, 0x072A, 0x0730, 0x073A, 0x0745, 0x074A, 0x0750, 0x075A, 0x0765, 0x0769, 0x0772, 0x077A,
    0x0785, 0x0789, 0x0790, 0x079A, 0x07A4, 0x07AC, 0x07B4, 0x07BC, 0x07C4, 0x07CC, 0x07D4, 0x07DC, 0x07E4, 0x07EC, 0x07F4, 0x07FC,
};

d_phy_lut_t d_phy_lut[] = {
    {   80, 0x0068, 0x0802, 0x0002, 0x0A30, 0x1B1C, 0x0008, 0xE69B, 0x0A20, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  100, 0x0068, 0x0802, 0x0002, 0x0A30, 0x1B1C, 0x0008, 0xE69B, 0x0A20, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  200, 0x0068, 0x0802, 0x0002, 0x0A50, 0x161C, 0x0008, 0xE69B, 0x0A40, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  300, 0x0068, 0x0802, 0x0002, 0x0A50, 0x111C, 0x0008, 0xE69B, 0x0A40, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  400, 0x0068, 0x0802, 0x0002, 0x0A70, 0x111C, 0x0008, 0xE69B, 0x0A60, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  500, 0x0068, 0x0802, 0x0002, 0x0A70, 0x0F1C, 0x0008, 0xE69B, 0x0A60, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  600, 0x0068, 0x0802, 0x0002, 0x0A70, 0x0F1C, 0x0008, 0xE69B, 0x0A60, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  700, 0x0068, 0x0802, 0x0002, 0x0A90, 0x0F1C, 0x0008, 0xE69B, 0x0A80, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  800, 0x0068, 0x0802, 0x0002, 0x0A90, 0x0E1C, 0x0008, 0xE69B, 0x0A80, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    {  900, 0x0068, 0x0802, 0x0002, 0x0A90, 0x0E1C, 0x0008, 0xE69B, 0x0A80, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    { 1000, 0x0068, 0x0802, 0x0002, 0x0A90, 0x0E1C, 0x0008, 0xE69B, 0x0A80, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    { 1100, 0x0068, 0x0802, 0x0002, 0x0A90, 0x0E1C, 0x0008, 0xE69B, 0x0A80, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    { 1200, 0x0068, 0x0802, 0x0002, 0x0A90, 0x0E1C, 0x0008, 0xE69B, 0x0A80, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    { 1300, 0x0068, 0x0802, 0x0002, 0x0AB0, 0x0E1C, 0x0008, 0xE69B, 0x0AA0, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    { 1400, 0x0068, 0x0802, 0x0002, 0x0AB0, 0x0E1C, 0x0008, 0xE69B, 0x0AA0, 0x0000, REG(0),          REG(0),           REG(0),           REG(0),           REG(0),          REG(0),          REG(0),          },
    { 1500, 0x0068, 0x0802, 0x0002, 0x0AB0, 0x0D1C, 0x0008, 0xE69B, 0x0AA0, 0x0000, REG(1,7,0,0x8F), REG(0),           REG(1,9,0,0x113), REG(1,12,12,0x0), REG(1,7,0,0x0F), REG(1,8,0,0x42), REG(1,15,8,0x1D) },
    { 1600, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x87), REG(1,9,0,0x07D), REG(1,9,0,0x102), REG(1,12,12,0x0), REG(1,7,0,0x10), REG(1,8,0,0x42), REG(1,15,8,0x1B) },
    { 1700, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x7F), REG(1,9,0,0x085), REG(1,9,0,0x0F2), REG(1,12,12,0x0), REG(1,7,0,0x10), REG(1,8,0,0x42), REG(1,15,8,0x1A) },
    { 1800, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x77), REG(1,9,0,0x08D), REG(1,9,0,0x0E2), REG(1,12,12,0x0), REG(1,7,0,0x10), REG(1,8,0,0x32), REG(1,15,8,0x18) },
    { 1900, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x77), REG(1,9,0,0x095), REG(1,9,0,0x0E2), REG(1,12,12,0x0), REG(1,7,0,0x10), REG(1,8,0,0x32), REG(1,15,8,0x18) },
    { 2000, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x6F), REG(1,9,0,0x09D), REG(1,9,0,0x0D2), REG(1,12,12,0x0), REG(1,7,0,0x10), REG(1,8,0,0x32), REG(1,15,8,0x17) },
    { 2100, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x67), REG(1,9,0,0x0A5), REG(1,9,0,0x0C1), REG(1,12,12,0x0), REG(1,7,0,0x10), REG(1,8,0,0x32), REG(1,15,8,0x15) },
    { 2200, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x67), REG(1,9,0,0x0AC), REG(1,9,0,0x0C1), REG(1,12,12,0x0), REG(1,7,0,0x10), REG(1,8,0,0x32), REG(1,15,8,0x15) },
    { 2300, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x5F), REG(1,9,0,0x0B4), REG(1,9,0,0x0B1), REG(1,12,12,0x0), REG(1,7,0,0x0B), REG(1,8,0,0x41), REG(1,15,8,0x13) },
    { 2400, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x5F), REG(1,9,0,0x0BC), REG(1,9,0,0x0B1), REG(1,12,12,0x0), REG(1,7,0,0x0B), REG(1,8,0,0x41), REG(1,15,8,0x13) },
    { 2500, 0x0028, 0x0800, 0x0000, 0x0AB0, 0x0D1C, 0x0004, 0xE69A, 0x0AA4, 0x0003, REG(1,7,0,0x57), REG(1,9,0,0x0C4), REG(1,9,0,0x0A1), REG(1,12,12,0x0), REG(1,7,0,0x0C), REG(1,8,0,0x31), REG(1,15,8,0x12) },
    { 2600, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x57), REG(1,9,0,0x0CC), REG(1,9,0,0x0A1), REG(1,12,12,0x0), REG(1,7,0,0x0C), REG(1,8,0,0x31), REG(1,15,8,0x12) },
    { 2700, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x4F), REG(1,9,0,0x0D3), REG(1,9,0,0x091), REG(1,12,12,0x0), REG(1,7,0,0x0E), REG(1,8,0,0x31), REG(1,15,8,0x10) },
    { 2800, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x4F), REG(1,9,0,0x0DB), REG(1,9,0,0x091), REG(1,12,12,0x0), REG(1,7,0,0x0E), REG(1,8,0,0x31), REG(1,15,8,0x10) },
    { 2900, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x4F), REG(1,9,0,0x0E3), REG(1,9,0,0x091), REG(1,12,12,0x0), REG(1,7,0,0x0E), REG(1,8,0,0x31), REG(1,15,8,0x10) },
    { 3000, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x47), REG(1,9,0,0x0EB), REG(1,9,0,0x080), REG(1,12,12,0x0), REG(1,7,0,0x0F), REG(1,8,0,0x19), REG(1,15,8,0x0F) },
    { 3100, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x47), REG(1,9,0,0x0F3), REG(1,9,0,0x080), REG(1,12,12,0x0), REG(1,7,0,0x0F), REG(1,8,0,0x19), REG(1,15,8,0x0F) },
    { 3200, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x47), REG(1,9,0,0x0FA), REG(1,9,0,0x080), REG(1,12,12,0x0), REG(1,7,0,0x0F), REG(1,8,0,0x19), REG(1,15,8,0x0F) },
    { 3300, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x57), REG(1,9,0,0x102), REG(1,9,0,0x0A1), REG(1,12,12,0x1), REG(1,7,0,0x0C), REG(1,8,0,0x31), REG(1,15,8,0x12) },
    { 3400, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x57), REG(1,9,0,0x10A), REG(1,9,0,0x0A1), REG(1,12,12,0x1), REG(1,7,0,0x0C), REG(1,8,0,0x31), REG(1,15,8,0x12) },
    { 3500, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x57), REG(1,9,0,0x112), REG(1,9,0,0x0A1), REG(1,12,12,0x1), REG(1,7,0,0x0C), REG(1,8,0,0x31), REG(1,15,8,0x12) },
    { 3600, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x4F), REG(1,9,0,0x11A), REG(1,9,0,0x091), REG(1,12,12,0x1), REG(1,7,0,0x0E), REG(1,8,0,0x31), REG(1,15,8,0x10) },
    { 3700, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x4F), REG(1,9,0,0x122), REG(1,9,0,0x091), REG(1,12,12,0x1), REG(1,7,0,0x0E), REG(1,8,0,0x31), REG(1,15,8,0x10) },
    { 3800, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x4F), REG(1,9,0,0x129), REG(1,9,0,0x091), REG(1,12,12,0x1), REG(1,7,0,0x0E), REG(1,8,0,0x31), REG(1,15,8,0x10) },
    { 3900, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x4F), REG(1,9,0,0x131), REG(1,9,0,0x091), REG(1,12,12,0x1), REG(1,7,0,0x0E), REG(1,8,0,0x31), REG(1,15,8,0x10) },
    { 4000, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x47), REG(1,9,0,0x139), REG(1,9,0,0x081), REG(1,12,12,0x1), REG(1,7,0,0x10), REG(1,8,0,0x29), REG(1,15,8,0x0F) },
    { 4100, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x47), REG(1,9,0,0x141), REG(1,9,0,0x081), REG(1,12,12,0x1), REG(1,7,0,0x10), REG(1,8,0,0x29), REG(1,15,8,0x0F) },
    { 4200, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x47), REG(1,9,0,0x149), REG(1,9,0,0x081), REG(1,12,12,0x1), REG(1,7,0,0x10), REG(1,8,0,0x29), REG(1,15,8,0x0F) },
    { 4300, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x47), REG(1,9,0,0x150), REG(1,9,0,0x081), REG(1,12,12,0x1), REG(1,7,0,0x10), REG(1,8,0,0x29), REG(1,15,8,0x0F) },
    { 4400, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x47), REG(1,9,0,0x158), REG(1,9,0,0x081), REG(1,12,12,0x1), REG(1,7,0,0x10), REG(1,8,0,0x29), REG(1,15,8,0x0F) },
    { 4500, 0x0028, 0x0800, 0x0000, 0x0AD0, 0x0D1C, 0x0004, 0x669A, 0x0AD4, 0x0003, REG(1,7,0,0x3F), REG(1,9,0,0x160), REG(1,9,0,0x070), REG(1,12,12,0x1), REG(1,7,0,0x10), REG(1,8,0,0x11), REG(1,15,8,0x0D) },
};

c_phy_lut_t c_phy_lut[] = {
    {   80, 0x0038, 0x024A, 0x006A, 0x0134, 0x0A24, 0x0000 },
    {  100, 0x0038, 0x024A, 0x0055, 0x00F5, 0x0A24, 0x0000 },
    {  200, 0x0038, 0x024A, 0x002B, 0x0077, 0x0A44, 0x0000 },
    {  300, 0x0038, 0x024A, 0x001D, 0x004D, 0x0A44, 0x0000 },
    {  400, 0x0038, 0x024A, 0x0016, 0x0038, 0x0A64, 0x0000 },
    {  500, 0x0038, 0x024A, 0x0012, 0x002B, 0x0A64, 0x0000 },
    {  600, 0x0038, 0x024A, 0x000F, 0x0023, 0x0A64, 0x0000 },
    {  700, 0x0038, 0x024A, 0x000D, 0x001D, 0x0A84, 0x0000 },
    {  800, 0x0038, 0x024A, 0x000C, 0x0018, 0x0A84, 0x0000 },
    {  900, 0x0038, 0x024A, 0x000B, 0x0015, 0x0A84, 0x0000 },
    { 1000, 0x003E, 0x024A, 0x000A, 0x0012, 0x0A84, 0x0400 },
    { 1100, 0x0044, 0x024A, 0x0009, 0x000F, 0x0A84, 0x0800 },
    { 1200, 0x004A, 0x024A, 0x0008, 0x000E, 0x0A84, 0x0C00 },
    { 1300, 0x0051, 0x024A, 0x0008, 0x000C, 0x0AA4, 0x0C00 },
    { 1400, 0x0057, 0x024A, 0x0007, 0x000B, 0x0AA4, 0x1000 },
    { 1500, 0x005D, 0x044A, 0x0007, 0x0009, 0x0AA4, 0x1000 },
    { 1600, 0x0063, 0x044A, 0x0007, 0x0008, 0x0AA4, 0x1400 },
    { 1700, 0x006A, 0x044A, 0x0006, 0x0007, 0x0AA4, 0x1400 },
    { 1800, 0x0070, 0x044A, 0x0006, 0x0007, 0x0AA4, 0x1400 },
    { 1900, 0x0076, 0x044A, 0x0006, 0x0006, 0x0AA4, 0x1400 },
    { 2000, 0x007C, 0x044A, 0x0006, 0x0005, 0x0AA4, 0x1800 },
    { 2100, 0x0083, 0x044A, 0x0005, 0x0005, 0x0AA4, 0x1800 },
    { 2200, 0x0089, 0x064A, 0x0005, 0x0004, 0x0AA4, 0x1800 },
    { 2300, 0x008F, 0x064A, 0x0005, 0x0003, 0x0AA4, 0x1800 },
    { 2400, 0x0095, 0x064A, 0x0005, 0x0003, 0x0AA4, 0x1800 },
    { 2500, 0x009C, 0x064A, 0x0005, 0x0003, 0x0AA4, 0x1C00 },
    { 2600, 0x00A2, 0x064A, 0x0005, 0x0002, 0x0AD4, 0x1C00 },
    { 2700, 0x00A8, 0x064A, 0x0005, 0x0002, 0x0AD4, 0x1C00 },
    { 2800, 0x00AE, 0x064A, 0x0004, 0x0002, 0x0AD4, 0x1C00 },
    { 2900, 0x00B5, 0x084A, 0x0004, 0x0001, 0x0AD4, 0x1C00 },
    { 3000, 0x00BB, 0x084A, 0x0004, 0x0001, 0x0AD4, 0x1C00 },
    { 3100, 0x00C1, 0x084A, 0x0004, 0x0001, 0x0AD4, 0x1C00 },
    { 3200, 0x00C7, 0x084A, 0x0004, 0x0001, 0x0AD4, 0x1C00 },
    { 3300, 0x00CE, 0x084A, 0x0004, 0x0001, 0x0AD4, 0x1C00 },
    { 3400, 0x00D4, 0x084A, 0x0004, 0x0001, 0x0AD4, 0x1C00 },
    { 3500, 0x00DA, 0x084A, 0x0004, 0x0001, 0x0AD4, 0x1C00 },
};

uint8_t product_id = PRODUCT_ID_RCAR_UNKNOWN;
uint8_t product_rev = PRODUCT_REV_UNKNOWN;

static int rcar_csi2_phy_freq(rcar_vin_t *vin)
{
    int  mbps_idx = CSI2_FREQ_80MBPS;
    uint32_t  mbps = 80;

    switch (vin->csi_lanes) {
    case 1:
        mbps_idx = CSI2_FREQ_300MBPS;
         mbps = 300;
        break;
    case 2:
        if (!vin->csi_interlace) {
            if ((vin->csi_width == 1920) && (vin->csi_height == 1080)) {
                mbps_idx = CSI2_FREQ_1100MBPS; mbps = 1100;
            } else if ((vin->csi_width == 1280) && (vin->csi_height == 720)) {
                mbps_idx = CSI2_FREQ_900MBPS; mbps = 900;
            } else if ((vin->csi_width == 720) && (vin->csi_height == 480)) {
                mbps_idx = CSI2_FREQ_330MBPS; mbps= 330;
            } else if ((vin->csi_width == 720) && (vin->csi_height == 576)) {
                mbps_idx = CSI2_FREQ_400MBPS; mbps = 400;
            } else if ((vin->csi_width == 640) && (vin->csi_height == 480)) {
                mbps_idx = CSI2_FREQ_300MBPS; mbps = 300;
            } else
                goto reso_failed;
        } else {
            if ((vin->csi_width == 1920) && (vin->csi_height == 1080)) {
                mbps_idx  = CSI2_FREQ_1000MBPS; mbps = 1000;
            } else if (((vin->csi_width == 720) && (vin->csi_height == 576))
                || ((vin->csi_width == 720) && (vin->csi_height == 480))) {
                mbps_idx = CSI2_FREQ_80MBPS; mbps = 80;
            } else {
                goto reso_failed;
            }
        }
        break;
    case 4:
        if (!vin->csi_interlace) {
            if ((vin->csi_width == 1920) && (vin->csi_height == 1080)) {
                mbps_idx = CSI2_FREQ_900MBPS; mbps = 900;
             /* vin frequency setting for OV490/OV10640 only */
            } else if ((vin->csi_width == 1280) && (vin->csi_height == 1080)) {
                if (vin->csi_rate == 200) { mbps_idx = CSI2_FREQ_205MBPS; mbps = 300; }
                else if (vin->csi_rate == 400) { mbps_idx = CSI2_FREQ_400MBPS; mbps = 400; }
                else if (vin->csi_rate == 600) { mbps_idx = CSI2_FREQ_600MBPS; mbps = 600; }
                else if (vin->csi_rate == 800) { mbps_idx = CSI2_FREQ_800MBPS; mbps = 800; }
            }
            else if ((vin->csi_width == 1280) && (vin->csi_height == 720)) {
                mbps_idx = CSI2_FREQ_450MBPS; mbps = 500;
            } else if ((vin->csi_width == 720) && (vin->csi_height == 480)) {
                mbps_idx = CSI2_FREQ_180MBPS; mbps = 200;
            } else if ((vin->csi_width == 720) && (vin->csi_height == 576)) {
                mbps_idx = CSI2_FREQ_180MBPS; mbps = 200;
            } else if ((vin->csi_width == 640) && (vin->csi_height == 480)) {
                mbps_idx = CSI2_FREQ_100MBPS; mbps = 100;
            } else {
                goto reso_failed;
            }
        } else {
            if ((vin->csi_width == 1920) && (vin->csi_height == 1080)) {
                mbps_idx = CSI2_FREQ_450MBPS; mbps = 500;
            } else {
                goto reso_failed;
            }
        }
        break;
    default:
        fprintf(stderr, "%s: lanes is invalid (%d)\n", __FUNCTION__, vin->csi_lanes);
        return -EINVAL;
    }

    switch (product_id) 
    {
        case PRODUCT_ID_RCAR_H3:
            switch (product_rev)
            {
                case PRODUCT_REV_1_0:
                case PRODUCT_REV_1_1:
                    if(csi2_freqs_m3w_h3es1[mbps_idx] != FREQ_MBPS_ERROR)
                        out32(vin->csi_vbase + RCAR_CSI2_PHYPLL, csi2_freqs_m3w_h3es1[mbps_idx] << 16);
                    else
                        goto fregs_failed;
                    break;
                case PRODUCT_REV_2_0:
                case PRODUCT_REV_3_0:
                    if(csi2_freqs_h3_v3h_m3n[mbps_idx] != FREQ_MBPS_ERROR)
                        out32(vin->csi_vbase + RCAR_CSI2_PHYPLL, csi2_freqs_h3_v3h_m3n[mbps_idx] << 16);
                    else
                        goto fregs_failed;
                    break;
                default:
                    break;
            }
            break;
        case PRODUCT_ID_RCAR_M3W:
            if(csi2_freqs_m3w_h3es1[mbps_idx] != FREQ_MBPS_ERROR)
                out32(vin->csi_vbase + RCAR_CSI2_PHYPLL, csi2_freqs_m3w_h3es1[mbps_idx] << 16);
            else
                goto fregs_failed;
            break;
        case PRODUCT_ID_RCAR_V3H:
        case PRODUCT_ID_RCAR_M3N:
            if(csi2_freqs_h3_v3h_m3n[mbps_idx] != FREQ_MBPS_ERROR)
                out32(vin->csi_vbase + RCAR_CSI2_PHYPLL, csi2_freqs_h3_v3h_m3n[mbps_idx] << 16);
            else
                goto fregs_failed;
            break;
        case PRODUCT_ID_RCAR_V3M:
        case PRODUCT_ID_RCAR_E3:
            if(csi2_freqs_v3m_e3[mbps_idx] != FREQ_MBPS_ERROR)
                out32(vin->csi_vbase + RCAR_CSI2_PHTW, (RCAR_CSI2_PHTW_DWEN | (csi2_freqs_v3m_e3[mbps_idx] << 16) |
                                                             RCAR_CSI2_PHTW_CWEN | RCAR_CSI2_PHTW_TESTDIN_CODE(0x44)));   
            else
                goto fregs_failed;
            break;
        case PRODUCT_ID_RCAR_V3U:
            if(csi2_freqs_v3u[mbps_idx] != FREQ_MBPS_ERROR) 
                out32(vin->csi_vbase + RCAR_CSI2_PHYPLL, csi2_freqs_v3u[mbps_idx] << 16);  
            else
                goto fregs_failed;
            break;
        case PRODUCT_ID_RCAR_V4H:
            break;
        default:
            fprintf(stderr, "%s: Board is not supported CSI2 \n", __FUNCTION__);
            return -EINVAL;
    }

    return mbps;

reso_failed:
    fprintf(stderr, "%s: not support resolution (%dx%d%c)\n",
         __FUNCTION__, vin->csi_width, vin->csi_height, (vin->csi_interlace) ? 'i' : 'p');

    return -EINVAL;

fregs_failed:
    fprintf(stderr, "%s: Set CSI2 frequency failed\n",  __FUNCTION__);
    
    return -EINVAL;
}

int rcar_csi2_fini(rcar_vin_t *vin)
{
   if (vin->csi_vbase) {	   
        out32(vin->csi_vbase + RCAR_CSI2_PHYCNT, 0);

        /* Reset CSI2 hardware */
        out32(vin->csi_vbase + RCAR_CSI2_SRST, 0x01);
        delay(1);
        out32(vin->csi_vbase + RCAR_CSI2_SRST, 0x00);
    
        munmap_device_io(vin->csi_vbase, RCAR_CSI2_SIZE);
    }

    return 0;
}

static void csi2_accesst3_com(rcar_vin_t *vin)
{
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_ANACTRL_RW_COMMON_ANACTRL_0, 0x1BFD);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_STARTUP_1_1, 0x0233);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_6, 0x0027);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_CALIBCTRL_RW_COMMON_BG_0, 0x01F4);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_TERMCAL_CFG_0, 0x0013);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_OFFSETCAL_CFG_0, 0x0003);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_TIMEBASE, 0x004F);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_NREF, 0x0320);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_NREF_RANGE, 0x000F);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_TWAIT_CONFIG, 0xFE18);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_VT_CONFIG, 0x0C3C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_LPDCOCAL_COARSE_CFG, 0x0105);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_CB_CTRL_2_6, 0x1000);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_COMMON_CFG, 0x0003);
}

static int csi2_phy_ready_checking(rcar_vin_t *vin) 
{
    int timeout;
    uint32_t status;

    /*  Leave Shutdown Mode */
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_DPHY_RSTZ, 0x1);    
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_PHY_SHUTDOWNZ, 0x1);

    for (timeout = 100; timeout > 0; --timeout) {
        status = in32(vin->csi_vbase + RCAR_GEN4_CSI2_ST_PHYST);
        if (status & RCAR_GEN4_CSI2_PHY_READY)
            break;
        nanospin_ns(50000);
    }

    if(!timeout) {
        fprintf(stderr, "%s: PHY calibration failed \n", __FUNCTION__);
        return -1;
    }

    return EOK;

}

static int csi2_dphy_accesst3_hs_setting(rcar_vin_t *vin, uint32_t mbps)
{
    int mbps_idx;

    for(mbps_idx = 0; mbps_idx < (sizeof(d_phy_lut) / sizeof(d_phy_lut_t)); mbps_idx++) {
        if (d_phy_lut[mbps_idx].mbps > mbps) {
            break;
        }
    }
    if (mbps_idx > 0) mbps_idx -= 1;
    if (mbps_idx >= (sizeof(d_phy_lut) / sizeof(d_phy_lut_t))) {
        fprintf(stderr, "%s: data rate (%d) is not supported \n", __FUNCTION__, mbps);
        return -EINVAL;
    }

    // T3 setting
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_7, 0x0000);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_7, d_phy_lut[mbps_idx].PPI_STARTUP_RW_COMMON_DPHY_7);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_8, 0x0050);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_0, 0x0063);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_7, 0x1132);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_1, 0x1340);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_2, 0x4B13);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_4, 0x000A);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_6, 0x800A);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_7, 0x1109);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_3, d_phy_lut[mbps_idx].PPI_RW_DDLCAL_CFG_3);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_1, d_phy_lut[mbps_idx].PPI_RW_DDLCAL_CFG_1);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_RW_DDLCAL_CFG_5, d_phy_lut[mbps_idx].PPI_RW_DDLCAL_CFG_5);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_8, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_8);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_8, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_8);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_8, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_8);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_8, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_8);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_8, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_8);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_LP_0, 0x463C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_LP_0, 0x463C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_LP_0, 0x463C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_LP_0, 0x463C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_2, 0x0000);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_2, 0x0000);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_2, 0x0001);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_2, 0x0000);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_2, 0x0000);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_6, 0x0009);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_12, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_12);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_12, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_12);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_12, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_12);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_12, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_12);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_12, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_12);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_13, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_12);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_13, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_13);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_13, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_13);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_13, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_13);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_13, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEx_CTRL_2_13);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_9, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_9);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_0, 0x091C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_HS_RX_7, 0x3B06);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_0, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_0, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_0, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_0, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_CFG_1, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_CFG_1);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_CFG_1, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_CFG_1);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_CFG_1, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_CFG_1);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_CFG_1, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_CFG_1);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_LP_0, 0x163C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_LP_0, 0x163C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_LP_0, 0x163C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_LP_0, 0x163C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_CLK_RW_LP_0, 0x163C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_1, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_1);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_1, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_1);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_1, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_1);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_1, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_1);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_2, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_3, 0x9209);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_3, 0x9209);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_3, 0x9209);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_3, 0x9209);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_4, 0x0096);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_4, 0x0096);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_4, 0x0096);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_4, 0x0096);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_5, 0x0100);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_5, 0x0100);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_5, 0x0100);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_5, 0x0100);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_6, 0x2D02);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_6, 0x2D02);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_6, 0x2D02);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_6, 0x2D02);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_7, 0x1B06);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_7, 0x1B06);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_7, 0x1B06);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_7, 0x1B06);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_3, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_3);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_3, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_3);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_3, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_3);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_3, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_3);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_0_RW_HS_RX_6, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_6);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_1_RW_HS_RX_6, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_6);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_2_RW_HS_RX_6, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_6);
    csi_bitset(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_DLANE_3_RW_HS_RX_6, d_phy_lut[mbps_idx].CORE_DIG_DLANE_n_RW_HS_RX_6);

    for (int i = 0; i < sizeof(deskew_fine_mem) / sizeof(deskew_fine_mem[0]); ++i) {
        out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_COMMON_RW_DESKEW_FINE_MEM, deskew_fine_mem[i]);
    }

    /*  Leave Shutdown Mode */
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_PHY_SHUTDOWNZ, 0x1);
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_DPHY_RSTZ, 0x1);

    // HS setting
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_9,  d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_9);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_15, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEn_CTRL_2_15);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_15, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEn_CTRL_2_15);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_15, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEn_CTRL_2_15);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_15, d_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANEn_CTRL_2_15);

    return EOK;
}

static int csi2_cphy_accesst3_hs_setting(rcar_vin_t *vin, int mbps)
{
    int mbps_idx;

    for(mbps_idx = 0; mbps_idx < (sizeof(c_phy_lut) / sizeof(c_phy_lut_t)); mbps_idx++) {
        if (d_phy_lut[mbps_idx].mbps > mbps) {
            break;
        }
    }
    if (mbps_idx > 0) mbps_idx -= 1;
    if (mbps_idx >= (sizeof(c_phy_lut) / sizeof(c_phy_lut_t))) {
        fprintf(stderr, "%s: data rate (%d) is not supported \n", __FUNCTION__, mbps);
        return -EINVAL;
    }

    // T3 setting
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_COMMON_7, 0x0155);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_7, 0x0068);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_PPI_STARTUP_RW_COMMON_DPHY_8, 0x0010);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_LP_0, 0x463C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_LP_0, 0x463C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_LP_0, 0x463C);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_0, 0x00D5);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_0, 0x00D5);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_0, 0x00D5);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_1, 0x0013);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_1, 0x0013);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_1, 0x0013);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_5, 0x0013);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_5, 0x0013);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_5, 0x0013);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_6, 0x000A);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_6, 0x000A);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_6, 0x000A);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_0_RW_HS_RX_2, c_phy_lut[mbps_idx].CORE_DIG_CLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_RX_2, c_phy_lut[mbps_idx].CORE_DIG_CLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_CLANE_2_RW_HS_RX_2, c_phy_lut[mbps_idx].CORE_DIG_CLANE_n_RW_HS_RX_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_2, 0x0001);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE1_CTRL_2_2, 0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE2_CTRL_2_2, 0x0001);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE3_CTRL_2_2, 0x0001);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE4_CTRL_2_2, 0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO0_0, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO1_0, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO2_0, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_0);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO0_2, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO1_2, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO2_2, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_2);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO0_1, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_1);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO1_1, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_1);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_RW_TRIO2_1, c_phy_lut[mbps_idx].CORE_DIG_RW_TRIOn_1);

    out16(vin->csi_vbase +  RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_CFG_0, 0xF5);
    out16(vin->csi_vbase +  RCAR_GEN4_CSI2_CORE_DIG_CLANE_1_RW_HS_TX_6, 0x5000);

    if(csi2_phy_ready_checking(vin))
        return -1;

    // HS setting
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_9, c_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_9);
    out16(vin->csi_vbase + RCAR_GEN4_CSI2_CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_7, c_phy_lut[mbps_idx].CORE_DIG_IOCTRL_RW_AFE_LANE0_CTRL_2_7);

    return EOK;
}

static int rcar_v4h_csi2_init(rcar_vin_t *vin) 
{
    uint32_t  n_lanes = vin->csi_lanes;
    int mbps;

    /********** Start-up Sequence ************/
    // T0 setting
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_RESETN, 0x0);
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_DPHY_RSTZ, 0x0);
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_PHY_SHUTDOWNZ, 0x0);

    // T1 setting
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_PHY_EN,  0x1);
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_FLDC, 0);
    // out32(vin->csi_vbase + RCAR_GEN4_CSI2_FLDC, fldc);
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_FLDD, 0);
    // out32(vin->csi_vbase + RCAR_GEN4_CSI2_FLDD, fldd);
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_IDIC, 1 );
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_PHY_MODE, vin->csi_phy_mode == CSI2_PHY_MODE_CPHY ? 1 : 0); // Select C-PHY/D-PHY

    switch (n_lanes) {
        case 4:
        case 3:
        case 2:
        case 1:
            out32(vin->csi_vbase + RCAR_GEN4_CSI2_N_LANES, n_lanes -1);
            break;
        default:
            fprintf(stderr, "%s: CSI2 not support %d lanes \r\n", __FUNCTION__, n_lanes);
            return -1;
    }

    // T2 setting
    out32(vin->csi_vbase + RCAR_GEN4_CSI2_RESETN, 0x1);

    // T3 setting
    csi2_accesst3_com(vin);

    if (vin->csi_rate == 0) {
        /* Get PHY frequency */
        if((mbps = rcar_csi2_phy_freq(vin)) < 0)
            return -EINVAL;
    } else {
        mbps = vin->csi_rate;
    }

    if (vin->csi_phy_mode == CSI2_PHY_MODE_CPHY) {
        if (csi2_cphy_accesst3_hs_setting(vin, mbps))
            return -EINVAL;
    } else {
        if (csi2_dphy_accesst3_hs_setting(vin, mbps))
            return -EINVAL;
    }

    return EOK;
}

static int rcar_csi2_get_hwi_device(rcar_vin_t *vin, int channel)
{
    int ret = 0;
    hwi_tag *tag;
    unsigned hwi_off;

    /* CSI Physical base address */
    hwi_off = hwi_find_device(RCAR_HWI_CSI, channel);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag) {
            vin->csi_pbase = tag->location.base;
        } else {
            ret = -1;
        }
    } else {
        ret = -1;
    }

    hwi_off = hwi_find_device("r-car", 0);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, NULL);
        if(tag) {
            product_id = tag->hwversion.version;
            product_rev = tag->hwversion.hclass;
        } else {
            ret = -1;
        }
    } else {
        ret = -1;
    }

    return ret;
}

int rcar_csi2_init(rcar_vin_t *vin, int channel)
{
    int 		ret = 0;
    uint32_t 	phycnt = RCAR_CSI2_PHYCNT_ENABLECLK;
    uint32_t 	vcdt = 0;
    uint32_t 	vcdt2 = 0;
    int 		timeout;

    vin->csi_interlace = vin->dec.interlace;
    vin->csi_width 	   = vin->dec.sw;
    vin->csi_height    = vin->dec.sh;

    if (rcar_csi2_get_hwi_device(vin, channel)) {
        fprintf(stderr, "%s: Unable to get hwi device\n", __FUNCTION__);
        return -EINVAL;
    }

    /* Map base address */
    if((vin->csi_vbase = (uintptr_t)mmap_device_io(RCAR_GEN4_CSI2_SIZE, vin->csi_pbase)) == (uintptr_t)MAP_FAILED) {
        fprintf(stderr, "%s: CSI2 base mmap_device_io (0x%x) failed \n", __FUNCTION__, (uint32_t)vin->csi_pbase);
        return -ENOMEM;
    }

    if (product_id == PRODUCT_ID_RCAR_V4H) {
        if (((ret = rcar_v4h_csi2_init(vin))))
            return ret;
    } else {
        /* Registers are immediately reflected*/
        out32(vin->csi_vbase + RCAR_CSI2_TREF, 0x01);

        /* Reset CSI2 circuit */
        out32(vin->csi_vbase + RCAR_CSI2_SRST, 0x01);
        delay(1);
        out32(vin->csi_vbase + RCAR_CSI2_SRST, 0);
        out32(vin->csi_vbase + RCAR_CSI2_PHTC, 0);

        /* Set even field number and use even field detection */
        switch(vin->csi_lanes) {
            case 1:
                out32(vin->csi_vbase + RCAR_CSI2_FLD, RCAR_CSI2_FLD_FLD_NUM(1) | RCAR_CSI2_FLD_FLD_EN);
                phycnt 	|= RCAR_CSI2_PHYCNT_ENABLE_0;
                vcdt 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT_EN | RCAR_CSI2_VCDT_SEL_VC(0));
                break;
            case 2:
                out32(vin->csi_vbase + RCAR_CSI2_FLD, RCAR_CSI2_FLD_FLD_NUM(1) | RCAR_CSI2_FLD_FLD_EN |
                                                           RCAR_CSI2_FLD_FLD_EN2 | RCAR_CSI2_FLD_FLD_EN3 |
                                                           RCAR_CSI2_FLD_FLD_EN4);
                phycnt 	|= (RCAR_CSI2_PHYCNT_ENABLE_0 | RCAR_CSI2_PHYCNT_ENABLE_1);
                vcdt 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT_EN | RCAR_CSI2_VCDT_SEL_VC(0));
                vcdt 	|= ((RCAR_CSI2_UYVY_DT_FORMAT << 16) | RCAR_CSI2_VCDT_VCDT2_EN | RCAR_CSI2_VCDT_SEL_VC2(0));
                break;
            case 3:
                out32(vin->csi_vbase + RCAR_CSI2_FLD, RCAR_CSI2_FLD_FLD_NUM(1) | RCAR_CSI2_FLD_FLD_EN |
                                                           RCAR_CSI2_FLD_FLD_EN2 | RCAR_CSI2_FLD_FLD_EN3 |
                                                           RCAR_CSI2_FLD_FLD_EN4);
                phycnt 	|= (RCAR_CSI2_PHYCNT_ENABLE_0 | RCAR_CSI2_PHYCNT_ENABLE_1 | 
                            RCAR_CSI2_PHYCNT_ENABLE_2);
                vcdt 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT_EN | RCAR_CSI2_VCDT_SEL_VC(0));
                vcdt 	|= ((RCAR_CSI2_UYVY_DT_FORMAT << 16) | RCAR_CSI2_VCDT_VCDT2_EN | RCAR_CSI2_VCDT_SEL_VC2(1));
                vcdt2 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT3_EN | RCAR_CSI2_VCDT_SEL_VC3(2));
                break;
            case 4:
                out32(vin->csi_vbase + RCAR_CSI2_FLD, RCAR_CSI2_FLD_FLD_NUM(2) | RCAR_CSI2_FLD_FLD_EN |
                                                           RCAR_CSI2_FLD_FLD_EN2 | RCAR_CSI2_FLD_FLD_EN3 |
                                                           RCAR_CSI2_FLD_FLD_EN4);
                phycnt 	|= (RCAR_CSI2_PHYCNT_ENABLE_0 | RCAR_CSI2_PHYCNT_ENABLE_1 |
                            RCAR_CSI2_PHYCNT_ENABLE_2 | RCAR_CSI2_PHYCNT_ENABLE_3);
                vcdt 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT_EN | RCAR_CSI2_VCDT_SEL_VC(0));
                vcdt 	|= ((RCAR_CSI2_UYVY_DT_FORMAT << 16) | RCAR_CSI2_VCDT_VCDT2_EN | RCAR_CSI2_VCDT_SEL_VC2(1));
                vcdt2 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT3_EN | RCAR_CSI2_VCDT_SEL_VC3(2));
                vcdt2 	|= ((RCAR_CSI2_UYVY_DT_FORMAT << 16) | RCAR_CSI2_VCDT_VCDT4_EN | RCAR_CSI2_VCDT_SEL_VC3(3));
                break;
            default:
                fprintf(stderr, "%s: lanes is not supported (%d)\n", __FUNCTION__, vin->csi_lanes);
                return -EINVAL;
        }

        /* PHY frequency setting */
        if((ret = rcar_csi2_phy_freq(vin)) < 0)
            return ret;

        /* Enable data lanes operation */
        out32(vin->csi_vbase + RCAR_CSI2_PHYCNT, phycnt);
        out32(vin->csi_vbase + RCAR_CSI2_PHYCNT, phycnt | RCAR_CSI2_PHYCNT_SHUTDOWNZ);
        out32(vin->csi_vbase + RCAR_CSI2_PHYCNT, phycnt | RCAR_CSI2_PHYCNT_SHUTDOWNZ |
                                                               RCAR_CSI2_PHYCNT_RSTZ);
        out32(vin->csi_vbase + RCAR_CSI2_CHKSUM, RCAR_CSI2_CHKSUM_CRC_EN | 
                                                      RCAR_CSI2_CHKSUM_ECC_EN);

        if(product_id != PRODUCT_ID_RCAR_V3U) {
            out32(vin->csi_vbase + RCAR_CSI2_VCDT, vcdt);
            out32(vin->csi_vbase + RCAR_CSI2_VCDT2, vcdt2);
        }

        out32(vin->csi_vbase + RCAR_CSI2_FRDT, RCAR_CSI2_FRDT_DT_FE);
        delay(1);	
        out32(vin->csi_vbase + RCAR_CSI2_LINKCNT, RCAR_CSI2_LINKCNT_MONITOR_EN |
                                                       RCAR_CSI2_LINKCNT_MONI_PACT_EN |
                                                       RCAR_CSI2_LINKCNT_ICLK_NONSTOP);
        out32(vin->csi_vbase + RCAR_CSI2_LSWAP, RCAR_CSI2_LSWAP_L0SEL(0) |
                                                     RCAR_CSI2_LSWAP_L1SEL(1) |
                                                     RCAR_CSI2_LSWAP_L2SEL(2) |
                                                     RCAR_CSI2_LSWAP_L3SEL(3));
    }

    /* Wait for video decoder power down */
    delay(10);
    timeout = 100;
    /* Check stop state on the clock lane */
    while(!(in32(vin->csi_vbase + RCAR_CSI2_PHY_CLK_MONITOR(product_id)) & RCAR_CSI2_LANE_STOPSTATE(product_id, vin->csi_lanes))) {
        nanospin_ns(10000);
        if(!timeout--)
            break;
    }

    if (timeout == 0)
        fprintf(stderr, product_id == PRODUCT_ID_RCAR_V4H ? "%s: PHY is stopped\n" : "%s: Clock lane is stopped\n", __FUNCTION__);

    if(product_id != PRODUCT_ID_RCAR_V4H) {
        timeout = 100;
        /* Check stop state on the data lane */
        while(!(in32(vin->csi_vbase + RCAR_CSI2_PHY_CLK_MONITOR(product_id)) & RCAR_CSI2_LANE_STOPSTATE(product_id, vin->csi_lanes))) {
            nanospin_ns(10000);
            if(!timeout--)
                break;
        }

        if (timeout == 0) {
            fprintf(stderr, "%s: Data lane is stopped\n", __FUNCTION__);
        }
    }

    return 0;
}

