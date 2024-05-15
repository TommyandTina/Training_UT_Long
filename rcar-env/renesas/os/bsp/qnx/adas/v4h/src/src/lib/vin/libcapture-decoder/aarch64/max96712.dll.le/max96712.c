/*
 * MAXIM max96712 GMSL2 driver
 *
 * Copyright (C) 2019-2020 Cogent Embedded, Inc.
 * Copyright 2020, Renesas Electronics Corporation
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include "max96712.h"
#include "max9295.h"
#include "decoder.h"

#define MAX96712_I2C_8B_WRITE(a, b, c, d) if(i2c_8b_write(max->i2c_des_fd, a, b, c, d)) \
        return -1

#define MAX96712_I2C_8B_READ(a, b, c, d) if(i2c_8b_read(a, b, c, d)) \
        return -1

#define MAX96712_I2C_16B_WRITE(a, b, c, d) if(i2c_16b_write(max->i2c_des_fd, a, b, c, d)) \
        return -1

#define MAX96712_I2C_16B_READ(a, b, c) if(i2c_16b_read(max->i2c_des_fd, a, b, c)) \
        return -1

#define MAX96712_BIT_CHANGE(a, b, c) if(max96712_bit_change(max, a, b, c)) \
        return -1

#define max96712_gmsl_rev_channel_setup(max, link_n) (max->gmsl_mode == MODE_GMSL1 ? max96712_gmsl1_channel_setup(max, link_n) : \
                                                                                    max96712_gmsl2_channel_setup(max, link_n))
#define max96712_gmsl_cc_enable(max,link,on)    (max->gmsl_mode == MODE_GMSL1 ? max96712_gmsl1_cc_enable(max, link, on) : 0)
#define max96712_gmsl_init(max)    (max->gmsl_mode == MODE_GMSL1 ? max96712_gmsl1_init(max) : \
                                                                   max96712_gmsl2_init(max))
#define max96712_gmsl_link_ser_setup(max,link_n)    (max->gmsl_mode == MODE_GMSL1 ? max96712_gmsl1_link_ser_setup(max, link_n) : 0)
#define max96712_gmsl_link_pipe_setup(max,link_n)    (max->gmsl_mode == MODE_GMSL1 ? max96712_gmsl1_link_pipe_setup(max, link_n) : \
                                                                                     max96712_gmsl2_link_pipe_setup(max, link_n))
#define max96712_gmsl_link_crossbar_setup(max,link,dt)    (max->gmsl_mode == MODE_GMSL1 ? max96712_gmsl1_link_crossbar_setup(max, link, dt) : 0)
#define max96712_gmsl_postinit(max)    (max->gmsl_mode == MODE_GMSL1 ? max96712_gmsl1_postinit(max) : \
                                                                       max96712_gmsl2_postinit(max))
#define max96712_gmsl_fsync_setup(max)    (max->gmsl_mode == MODE_GMSL1 ? max96712_gmsl1_fsync_setup(max) : 0)

dec_info_t dec_info[] = {
    {
        .board_id = RCAR_V3U_FALCON,
        .dec_cfg  = {
            /* mipi_chan         des_sla_addr         pcae_sla_addr   poc_sla_addr        */
            { MAX96712_CSI_OUT1, MAX96712_0_SLA_ADDR, PCAE9654_0_SLA, MAX20087_0_SLA_ADDR },
            { MAX96712_CSI_OUT2, MAX96712_0_SLA_ADDR, PCAE9654_0_SLA, MAX20087_0_SLA_ADDR },
            { MAX96712_CSI_OUT1, MAX96712_1_SLA_ADDR, PCAE9654_1_SLA, MAX20087_1_SLA_ADDR },
            { MAX96712_CSI_OUT1, MAX96712_2_SLA_ADDR, PCAE9654_2_SLA, MAX20087_2_SLA_ADDR },
        }
    },
    {
        .board_id = RCAR_V4H_WHITEHAWK,
        .dec_cfg  = {
            /* mipi_chan         des_sla_addr         pcae_sla_addr   poc_sla_addr        */
            { MAX96712_CSI_OUT1, MAX96712_1_SLA_ADDR, PCAE9654_1_SLA, MAX20087_1_SLA_ADDR },
            { MAX96712_CSI_OUT1, MAX96712_0_SLA_ADDR, PCAE9654_0_SLA, MAX20087_0_SLA_ADDR },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        }
    },
};

gmsl2_pipe_map_t gmsl2_pipe_mipi_mapping[] = {
    {0x00,            0x00},            /* FS */
    {0x01,            0x01},            /* FE */
    {MIPI_DT_RAW10,   MIPI_DT_RAW10},   /* payload data */
    {0x01,   0x01},
    {0x01,   0x01},
};

uint32_t max96712_color_pattern_generator[] = {    
    // 0x1050, 0xF3,    // Set VTG mode VRX_Patgen 0, Generate VS, HS, DE", Invert the VS
    0x1050, 0xE3,        // Set VTG mode VRX_Patgen 0, Generate VS, HS, DE", Not Invert the VS
    0x1051, 0x20,        // Set Patgen mode = 2 (Color Gradient), Grad Mode = 0"

    0x0010, 0x00,        // Set VS_DLY = 0
    0x1053, 0x00,
    0x1054, 0x00,
    0x1055, 0x25,        // Set VS High
    0x1056, 0x99,
    0x1057, 0x00,
    0x1058, 0x00,        // Set VS Low
    0x1059, 0x2A,
    0x105A, 0xF8,
    // 0x105B, 0x01,    // Set HS Delay V2H
    // 0x105C, 0x5F,
    // 0x105D, 0x10,
    0x105B, 0x00,        // Set HS Delay V2H
    0x105C, 0x00,
    0x105D, 0x00,
    0x105E, 0x08,        // Set HS_HIGH
    0x105F, 0x6C,
    0x1060, 0x00,        // Set HS_LOW
    0x1061, 0x2C,
    0x1062, 0x04,        // Set HS_CNT
    0x1063, 0x65,
    // 0x1064, 0x01,    // Set DE Delay
    // 0x1065, 0x59,
    // 0x1066, 0x00,
    0x1064, 0x01,        // Set DE Delay
    0x1065, 0x61,
    0x1066, 0x18,
    0x1067, 0x07,        // Set DE_HIGH
    0x1068, 0x80,
    0x1069, 0x01,        // Set DE_LOW
    0x106A, 0x18,
    0x106B, 0x04,        // Set DE_CNT
    0x106C, 0x38,
    0x106D, 0x03,        // Set Grad_INCR_0_0

// CHECKERBOARD SETUP - PATGEN MODE = 1
    0x106E, 0x88,        // Set CHKR_COLOR_A_L_0
    0x106F, 0xAA,        // Set CHKR_COLOR_A_M_0
    0x1070, 0x55,        // Set CHKR_COLOR_A_H_0
    0x1071, 0x00,        // Set CHKR_COLOR_B_L_0
    0x1072, 0x08,        // Set CHKR_COLOR_B_M_0
    0x1073, 0x80,        // Set CHKR_COLOR_B_H_0
    0x1074, 0x50,        // Set CHKR_RPT_A_0
    0x1075, 0xA0,        // Set CHKR_RPT_B_0
    0x1076, 0x50,        // Set CHKR_ALT_0

// Set Patgen Clk frequency 75MHz firstly
    0x0009, 0x01,        //75MHz -> 150MHz
    // 0x0009, 0x02,    //150MHz
    0x01DC, 0x00,
    0x01FC, 0x00,
    0x021C, 0x00,
    0x023C, 0x00,

// RGB888 Software override VC/DT/BPP for Pipe 1,2,3 and 4; 0x24 = 6'b100100, 24 = 5'b11000; 0x2C = 6'b101100, 12 = 5'b01100
    0x040B, 0xC2,        // BPP override for 1
    // 0x040B, 0x62,
    // 0x040C, 0x00,     // VC override for 1/2/3/4
    // 0x040D, 0x00,
    0x040C, 0x10,        // VC override for 1/2/3/4
    0x040D, 0x32,
    0x040E, 0xA4,        // DT override for 1/2/3/4
    0x040F, 0x94,
    0x0410, 0x90,
    // 0x040E, 0xAC,
    // 0x040F, 0xBC,
    // 0x0410, 0xB0,
    0x0411, 0xD8,        // BPP override for 2/3/4
    0x0412, 0x60,
    // 0x0411, 0x6C,
    // 0x0412, 0x30,

    0xFFFFFFFF, 0xFFFFFFFF    // End Table
};


const uint32_t max96712_out_enable[] = {
// ----------Deserializer Setup--------------------------------------------------------------------
    0x0006, 0x00,        // Disable GMSL link
    0x0415, 0xEC,        // Set 1200M DPLL frequency, Enable the software override for 1/2/3/4
    0x0418, 0xEC,
    // 0x041B, 0xEC,
    // 0x041E, 0xEC,
    // 0x01DD, 0x1B,      // Set Alternate Crossbar for Video Pipes 0-3 - REMOVE ALTERNATE CROSSBAR???
    // 0x01FD, 0x1B,
    // 0x021D, 0x1B,
    // 0x023D, 0x1B,

    // 0x0409, 0x0F,     // Set Line Start, Line End for Video Pipe 0-3 (Backtop //1-4)"
    // 0x094A, 0xC0,     // Set Lane Count - 4 for script  (CSI_NUM_LANES), DPHY ONLY"
    // 0x08A1, 0x00,     // Set Phy data lane HS Timing
    0x08A3, 0xE4,        // Set Phy lane Map for all MIPI PHYs
    0x08A4, 0xE4,        
    // 0x08A5, 0x00,     // Set Phy lane Map for all MIPI PHYs

    0x090B, 0x07,        // Set MAP_EN_L_0 for map 0
    0x094B, 0x07,        // Set MAP_EN_L_0 for map 0
    0x098B, 0x07,        // Set MAP_EN_L_0 for map 0
    0x09CB, 0x07,        // Set MAP_EN_L_0 for map 0
    0x092D, 0x15,        // Set MAP_DPHY_DEST TO CTRL 1
    0x090D, 0x24,        // Set MAP_SRC AND DEST for data and FE/FS; [7:6]=VC, [5:0]=DT
    0x094D, 0x64,        // Set MAP_SRC AND DEST for data and FE/FS; [7:6]=VC, [5:0]=DT
    0x098D, 0xA4,        // Set MAP_SRC AND DEST for data and FE/FS; [7:6]=VC, [5:0]=DT
    0x09CD, 0xE4,        // Set MAP_SRC AND DEST for data and FE/FS; [7:6]=VC, [5:0]=DT
    0x090E, 0x24,
    // 0x090D, 0x2C,
    // 0x090E, 0x2C,
    0x090F, 0x00,
    0x0910, 0x00,
    0x0911, 0x01,
    0x0912, 0x01,

    0x0009, 0x02,        // Set to 150MHz <= (Changed issue order) =================================

    // 0x08A2, 0x0C,     // Turn off unused PHYs
    0x08A0, 0x84,        // Enable CSI clock w/ MIPI PHY 2x4 configuration
    0x08A2, 0x30,        // Turn off unused PHYs

    // 0x0009, 0x02,    // Set to 150MHz
// End of Setup Script - MUST ONE-SHOT RESET!!!! <=================================================
    0x0018, 0x0F,        // One shot reset for PHY A - HS86

    0xFFFFFFFF, 0xFFFFFFFF    // End Table
};

const uint32_t max96712_out_disable[] = {
    0x08A0,     0x04,         // Disable CSI clock w/ MIPI PHY 2x4 configuration,,
    0x08A2,     0x00,         // Stanby mode PHYs
    0xFFFFFFFF, 0xFFFFFFFF    // End Table
};

reg_info_t ov490_conf_regs[] = {
    /* IDC */
    {0xFFFD, 0x80},
    {0xFFFE, 0x82},
    {0x0071, 0x11},
    {0x0075, 0x11},

    /* VIFFO */
    {0xFFFD, 0x80},
    {0xFFFE, 0x29},
    {0x6010, 0x01},
    {0xe000, 0x14},

    /* DVP */
    {0xFFFD, 0x80},
    {0xFFFE, 0x28},
    {0x6000, 0x04},
    {0x6004, 0x00},
    {0x6008, 0x00},

    /* SC */
    {0xFFFD, 0x80},
    {0xFFFE, 0x80},
    {0x0091, 0x00},
    {0x00bb, 0x1d}, 

    /* VSGEN */
    {0xFFFD, 0x80},
    {0xFFFE, 0x85},
    {0x0008, 0x00},
    {0x0009, 0x01},
    {0x000A, 0x05},
    {0x000B, 0x00},
    {0x0030, 0x02}, 
    {0x0031, 0x00},
    {0x0032, 0x00},
    {0x0033, 0x00},
    {0x0038, 0x02}, 
    {0x0039, 0x00},
    {0x003A, 0x00},
    {0x003B, 0x00},
    {0x0070, 0x2C},
    {0x0071, 0x01},
    {0x0072, 0x00},
    {0x0073, 0x00},
    {0x0074, 0x64}, 
    {0x0075, 0x00},
    {0x0076, 0x00},
    {0x0077, 0x00},
    {0x0000, 0x14},
    {0x0001, 0x00},
    {0x0002, 0x00},
    {0x0003, 0x00},
    {0x0004, 0x32}, 
    {0x0005, 0x00},
    {0x0006, 0x00},
    {0x0007, 0x00},
    {0xFFFE, 0x80},
    {0x0081, 0x00},

    /* OV1064  sensor ctrl */
    {0xFFFD, 0x80},
    {0xFFFE, 0x19},
    {0x5000, 0x00},
    {0x5001, 0x30},
    {0x5002, 0x8C},
    {0x5003, 0xb2},
    {0xFFFE, 0x80},
    {0x00c0, 0xc1},
};

max9295_reg_info_t ar0231_conf_regs[] = {
    /* VIDEO_TX */
    {0x0002, 0x03},
    {0x0100, 0x60},
    {0x0101, 0x0A},

    /* GPIO8 */
    {0x02D6, 0x63},
    {0x02D7, 0x2B},
    {0x02D8, 0x0B},

    /* GPIO7 */
    {0x02D3, 0x84},
    {0x02D4, 0x2C},
    {0x02D5, 0x0C},

    {0x0007, 0xC7},
    {0x0332, 0xEE},
    {0x0333, 0xE4},
    {0x0314, 0x2B},
    {0x0316, 0x22},
    {0x0318, 0x22},
    {0x031A, 0x22},
    {0x031C, 0x2A},
    {0x0002, 0x13},
    {0x03F1, 0x89}
};

char mbus_default[10] = "mipi"; /* mipi, dvp */

static int mipi_dt_to_bpp(unsigned int dt)
{
    switch (dt) {
        case 0x2a:
        case 0x10 ... 0x12:
        case 0x31 ... 0x37:
            return 0x08;
        case 0x2b:
            return 0x0a;
        case 0x2c:
            return 0x0c;
        case 0x0d:
            return 0x0e;
        case 0x22:
        case 0x1e:
        case 0x2e:
            return 0x10;
        case 0x23:
            return 0x12;
        case 0x1f:
        case 0x2f:
            return 0x14;
        case 0x24:
        case 0x30:
            return 0x18;
        default:
            return 0x08;
    }
}

static char *chip_name(int id)
{
    switch(id) {
    case MAX9271_ID:
        return "MAX9271";
    case MAX9295A_ID:
        return "MAX9295A";
    case MAX9295B_ID:
        return "MAX9295B";
    default:
        return "Serializer";
    }
}

static int i2c_8b_read(int fd, uint8_t sla_addr, uint8_t reg_addr, uint8_t *data)
{
    int status = EOK;
    iov_t siov[2], riov[2];
    i2c_sendrecv_t hdr; 

    hdr.slave.addr = sla_addr >> 1;
    hdr.slave.fmt  = I2C_ADDRFMT_7BIT;
    hdr.stop       = 1;
    hdr.send_len   = 1;
    hdr.recv_len   = 1;

    SETIOV(&siov[0], &hdr, sizeof(hdr));
    SETIOV(&siov[1], &reg_addr, 1 );

    SETIOV(&riov[0], &hdr, sizeof(hdr));
    SETIOV(&riov[1], data, 1);

    status = devctlv(fd, DCMD_I2C_SENDRECV, 2, 2, siov, riov, NULL);

    if (status != EOK) {
        return -1;
    }

    return EOK;
}

static int i2c_8b_write(int fd, uint8_t sla_addr, uint8_t reg_addr, uint8_t data, int mdelay)
{
    int status = EOK;

    struct {
        i2c_send_t hdr;
        unsigned char bytes[8];
    } omsg;

    omsg.hdr.slave.addr = sla_addr >> 1;
    omsg.hdr.slave.fmt  = I2C_ADDRFMT_7BIT;
    omsg.hdr.len        = 2;
    omsg.hdr.stop       = 1;
    omsg.bytes[0]       = reg_addr;
    omsg.bytes[1]       = data;

    status = devctl(fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if(status != EOK) {
        return -1;
    }

    if(mdelay) delay(mdelay);

    return EOK;
}

static int i2c_16b_write(int fd, uint8_t sla_addr, uint16_t reg_addr, uint8_t data, int mdelay)
{
    int status = EOK;

    struct {
        i2c_send_t hdr;
        uint16_t   reg;
        uint8_t    val;
    } omsg;

    omsg.hdr.slave.addr = sla_addr >> 1;
    omsg.hdr.slave.fmt  = I2C_ADDRFMT_7BIT;
    omsg.hdr.len        = 3;
    omsg.hdr.stop       = 1;
    omsg.reg            = ((reg_addr & 0xff) << 8) | ((reg_addr & 0xff00) >> 8);
    omsg.val            = data;

    status = devctl(fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if(status != EOK) {
        return -1;
    }

    if(mdelay) delay(mdelay);

    return EOK;
}

static int i2c_16b_read(int fd, uint8_t sla_addr, uint16_t reg_addr, uint8_t *data)
{
    int status = EOK;

    struct  {
        i2c_send_t hdr;
        uint16_t   reg;
    } omsg;

    struct {
        i2c_recv_t hdr;
        uint8_t    val;
    } imsg;

    omsg.hdr.slave.addr = sla_addr >> 1;
    omsg.hdr.slave.fmt  = I2C_ADDRFMT_7BIT;
    omsg.hdr.len        = 2;
    omsg.hdr.stop       = 0;
    omsg.reg            = ((reg_addr & 0xff) << 8) | ((reg_addr & 0xff00) >> 8);

    status = devctl(fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if (status != EOK) {
        return -1;
    }

    imsg.hdr.slave.addr = sla_addr >> 1;
    imsg.hdr.slave.fmt  = I2C_ADDRFMT_7BIT;
    imsg.hdr.len        = 1;
    imsg.hdr.stop       = 1;

    status = devctl(fd, DCMD_I2C_RECV, &imsg, sizeof(imsg.hdr) + imsg.hdr.len, NULL);

    if (status != EOK) {
        return -1;
    }

    *data = imsg.val;

    return EOK;
}

static int max96712_bit_change(max96712_info_t *max, uint16_t reg, uint8_t mask, uint8_t val)
{ 
    int     ret; 
    uint8_t tmp, ori; 

    ret = i2c_16b_read(max->i2c_des_fd, max->des_sla_addr, reg, &ori);
    if (ret != 0)
        return ret;

    tmp = ori & ~mask;
    tmp |= val & mask;

    if (tmp != ori)
        ret = i2c_16b_write(max->i2c_des_fd, max->des_sla_addr, reg, tmp, 0);

    return ret;
}

#ifdef MAX96712_COLOR_PATTERN_TEST
int i2c_table_write(max96712_info_t *max, uint32_t *set_data)
{
    uint16_t reg_addr;
    uint8_t  data;

    while(*set_data != 0xFFFFFFFF) {
        reg_addr  = *set_data & 0xFFFF;
        set_data++;
        data = *set_data & 0xFF;
        set_data++;
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, reg_addr, data, 1);
    }

    return EOK;
}

int max96712_output_enable(max96712_info_t *max, int on)
{
    if(on) {
        // Enable all links
        if(i2c_table_write(max, (uint32_t*)max96712_out_enable))
            return -1;
        delay(300);
    }
    else {
        if(i2c_table_write(max, (uint32_t*)max96712_out_disable))
            return -1;
    }

    return EOK;
}
#endif

int max96712_power(max96712_info_t *max, int on)
{
    dec_cfg_t *dec_cfg;
    uint8_t data;

    dec_cfg = &max->dec_cfg[max->device];

    /*
     * Power max96712
     */

    MAX96712_I2C_8B_READ(max->i2c_pcae_fd, dec_cfg->pcae_sla_addr, PCAE9654_OUT_PORT, &data);

    if (on) {
        // Polarity Select
        if(i2c_8b_write(max->i2c_pcae_fd, dec_cfg->pcae_sla_addr, PCAE9654_POL_INV, 0x00, 0)) 
            return -1;

        // Select IO0 output
        if(i2c_8b_write(max->i2c_pcae_fd, dec_cfg->pcae_sla_addr, PCAE9654_CONFIG, 0xFF & ~(1 << 0), 0)) 
            return -1;

        data |= (1 << 0);
    } else {
        data &= ~(1 << 0);
    }

    if(i2c_8b_write(max->i2c_pcae_fd, dec_cfg->pcae_sla_addr, PCAE9654_OUT_PORT, data, 0)) 
        return -1;

    if (on) delay(300);

    /*
     * gmsl_cam_power
     */
    if (on) {
        MAX96712_I2C_16B_READ(dec_cfg->des_sla_addr, 0x30C, &data);
        data |= (1 << 4);        // GPIO4 = High = Camera POC Protector ON
        MAX96712_I2C_16B_WRITE(dec_cfg->des_sla_addr, 0x30C, data, 0);
    }

    // MAX20087 Reg 0x02(ID)
    MAX96712_I2C_8B_READ(max->i2c_poc_fd, dec_cfg->poc_sla_addr, 0x02, &data);

    if((data >> 4) != 2) {    // 0x2x: MAX20087
        fprintf(stderr, "Wrong ID code:0x%02x (not MAX20087)\n", data);
        return -1;
    }

    // MAX20087 Reg 0x01(CONFIG)
    MAX96712_I2C_8B_READ(max->i2c_poc_fd, dec_cfg->poc_sla_addr, 0x01, &data);
    if (on) {
        data |= (0xF);
    } else {
        data &= ~(0xF);
    }
    if(i2c_8b_write(max->i2c_poc_fd, dec_cfg->poc_sla_addr, 0x01, data, 0))
        return -1;

    if (!on) {
        MAX96712_I2C_16B_READ(dec_cfg->des_sla_addr, 0x30C, &data);
        data &= ~(1 << 4);        // GPIO4 = High = Camera POC Protector ON
        MAX96712_I2C_16B_WRITE(dec_cfg->des_sla_addr, 0x30C, data, 0);
    }

    return EOK;
}

int max96712_des_power(max96712_info_t *max, int on)
{
    uint8_t data;

    // Polarity Select
    if(i2c_8b_write(max->i2c_pcae_fd, max->pcae_sla_addr, PCAE9654_POL_INV, 0x00, 0)) 
        return -1;

    // Select IO0 output
    if(i2c_8b_write(max->i2c_pcae_fd, max->pcae_sla_addr, PCAE9654_CONFIG, 0xFF & ~(1 << 0), 0)) 
        return -1;

    MAX96712_I2C_8B_READ(max->i2c_pcae_fd, max->pcae_sla_addr, PCAE9654_OUT_PORT, &data);

    if(i2c_8b_write(max->i2c_pcae_fd, max->pcae_sla_addr, PCAE9654_OUT_PORT, on ? (data | (1 << 0)) : (data & ~(1 << 0)), 0)) 
        return -1;

    return EOK;
}

int gmsl_cam_power(max96712_info_t *max, int on)
{
    uint8_t data;

    MAX96712_I2C_16B_READ(max->des_sla_addr, 0x30C, &data);
    data |= (1 << 4);        // GPIO4 = High = Camera POC Protector ON
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, 0x30C, data, 0);

    // MAX20087 Reg 0x02(ID)
    MAX96712_I2C_8B_READ(max->i2c_poc_fd, max->poc_sla_addr, 0x02, &data);

    if((data >> 4) != 2) {    // 0x2x: MAX20087
        fprintf(stderr, "Wrong ID code:0x%02x (not MAX20087)\n", data);
        return -1;
    }

    // MAX20087 Reg 0x01(CONFIG)
    MAX96712_I2C_8B_READ(max->i2c_poc_fd, max->poc_sla_addr, 0x01, &data);
    on ? (data |= (0xF)) : (data &= ~(0xF));
    if(i2c_8b_write(max->i2c_poc_fd, max->poc_sla_addr, 0x01, data, 0))
        return -1;

    return EOK;
}

#ifdef MAX96712_COLOR_PATTERN_TEST
static uint32_t max96712_color_pattern_setup(max96712_info_t *max)
{
    uint32_t vs_low, vs_high, hs_low, hs_high, hs_count;
    uint32_t v2d, de_high, de_low, de_count;

    model_line_t def_res[] = {
        // FullHD(1920x1080p)
        { "1920x1080", "148.50", 1920, 2008, 2052, 2200, 1080, 1084, 1089, 1125, "+hsync", "+vsync", 0, 0, 0 },
    };

    vs_low   = def_res[0].v_end - def_res[0].v_start;
    vs_high  = def_res[0].v_total - vs_low;
    vs_low  *= def_res[0].h_total;        // PCLK
    vs_high *= def_res[0].h_total;        // PCLK
    hs_low   = def_res[0].h_end - def_res[0].h_start;
    hs_high  = def_res[0].h_total - hs_low;
    hs_count =  def_res[0].v_total;
    //v2d      =  def_res[0].v_total - def_res[0].v_end;
    v2d      = def_res[0].v_total - def_res[0].v_start;
    v2d     *= def_res[0].h_total;        // PCLK
    v2d     += (def_res[0].h_total - def_res[0].h_start);
    de_high  = def_res[0].h_disp;
    de_low   = def_res[0].h_total - de_high;
    de_count = def_res[0].v_disp;

    // VS delay=0 (PCLK)
    max96712_color_pattern_generator[5]      = 0;
    max96712_color_pattern_generator[5 + 2]  = 0;
    max96712_color_pattern_generator[5 + 4]  = 0;
    // VS high (PCLK)
    max96712_color_pattern_generator[5 + 6]  = (vs_high >> 16) & 0x0ff;
    max96712_color_pattern_generator[5 + 8]  = (vs_high >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 10] =  vs_high & 0x0FF;
    // VS low (PCLK)
    max96712_color_pattern_generator[5 + 12] = (vs_low >> 16) & 0x0ff;
    max96712_color_pattern_generator[5 + 14] = (vs_low >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 16] =  vs_low & 0x0FF;
    // V2H=0 (PCLK)
    max96712_color_pattern_generator[5 + 18] = 0;
    max96712_color_pattern_generator[5 + 20] = 0;
    max96712_color_pattern_generator[5 + 22] = 0;
    // HS high
    max96712_color_pattern_generator[5 + 24] = (hs_high >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 26] =  hs_high & 0x0FF;
    // HS low
    max96712_color_pattern_generator[5 + 28] = (hs_low >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 30] =  hs_low & 0x0FF;
    // HS count
    max96712_color_pattern_generator[5 + 32] = (hs_count >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 34] =  hs_count & 0x0FF;
    // V2D (PCLK)
    max96712_color_pattern_generator[5 + 36] = (v2d >> 16) & 0x0ff;
    max96712_color_pattern_generator[5 + 38] = (v2d >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 40] =  v2d & 0x0FF;
    // DE high
    max96712_color_pattern_generator[5 + 42] = (de_high >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 44] =  de_high & 0x0FF;
    // DE low
    max96712_color_pattern_generator[5 + 46] = (de_low >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 48] =  de_low & 0x0FF;
    // DE count
    max96712_color_pattern_generator[5 + 50] = (de_count >>  8) & 0x0ff;
    max96712_color_pattern_generator[5 + 52] =  de_count & 0x0FF;

    // MAX96712(GMSL2 => CSI2) Video setup
    if(i2c_table_write(max, (uint32_t*)max96712_color_pattern_generator))
        return -1;
    delay(300);

    return EOK;
}
#endif

int max96712_one_reset(max96712_info_t *max, int mask)
{
    uint8_t reg = 0;

    mask &= 0x0f;
    MAX96712_BIT_CHANGE(MAX96712_CTRL1, mask, mask); /* set reset one-shot */

    /* wait for one-shot bit self-cleared */
    for (int timeout = 0; timeout < 100; timeout++) {
        MAX96712_I2C_16B_READ(max->des_sla_addr, MAX96712_CTRL1, &reg);
        if (!(reg & mask))
            break;
        delay(1);
    }

    if (reg & mask) {
        fprintf(stderr, "Failed reset oneshot 0x%x\n", mask);
        return -1;
    }
    if(max->gmsl_mode == MODE_GMSL1)
        delay(2);

    return EOK;
}

int max96712_gmsl2_get_link_status(max96712_info_t *max, int link_n)
{
    uint8_t lock_reg[] = {MAX96712_CTRL3, MAX96712_CTRL12, MAX96712_CTRL13, MAX96712_CTRL14};
    uint8_t val = 0;

    MAX96712_I2C_16B_READ(max->des_sla_addr, lock_reg[link_n], &val);

    return !!(val & BIT(3));
}

int max96712_pipe_override(max96712_info_t *max, uint32_t pipe, uint32_t dt, uint32_t vc)
{
    int bpp, bank;

    bpp = mipi_dt_to_bpp(dt);
    bank = pipe / 4;
    pipe %= 4;

    if ((max->dbl == 1) && (max->gmsl_mode != MODE_GMSL2)) {
        /* DBL=1 is MUX mode, DBL=0 is Normal mode */
        /* enable MUX mode */
        MAX96712_BIT_CHANGE(MAX_BACKTOP27(bank), (1 << (pipe + 4)), (1 << (pipe + 4)));
        bpp = bpp / 2;	/* divide because of MUX=1 */
    }

    switch (pipe) {
    case 0:
        /* Pipe X: 0 or 4 */
        MAX96712_BIT_CHANGE(MAX_BACKTOP12(bank), 0x1f << 3, bpp << 3);
        MAX96712_BIT_CHANGE(MAX_BACKTOP13(bank), 0x0f, vc);
        MAX96712_BIT_CHANGE(MAX_BACKTOP15(bank), 0x3f, dt);
        MAX96712_BIT_CHANGE(bank ? MAX_BACKTOP28(0) : MAX_BACKTOP22(0), (1 << 6), (1 << 6)); /* enalbe s/w override */
        break;
    case 1:
        /* Pipe Y: 1 or 5 */
        MAX96712_BIT_CHANGE(MAX_BACKTOP18(bank), 0x1f, bpp);
        MAX96712_BIT_CHANGE(MAX_BACKTOP13(bank), 0x0f << 4, vc << 4);
        MAX96712_BIT_CHANGE(MAX_BACKTOP16(bank), 0x0f, dt & 0x0f);
        MAX96712_BIT_CHANGE(MAX_BACKTOP15(bank), 0x03 << 6, (dt & 0x30) << 2);
        MAX96712_BIT_CHANGE(bank ? MAX_BACKTOP28(0) : MAX_BACKTOP22(0), (1 << 7), (1 << 7)); /* enable s/w override */
        break;
    case 2:
        /* Pipe Z: 2 or 6 */
        MAX96712_BIT_CHANGE(MAX_BACKTOP19(bank), 0x03, bpp & 0x03);
        MAX96712_BIT_CHANGE(MAX_BACKTOP18(bank), 0xe0, (bpp & 0x1c) << 3);
        MAX96712_BIT_CHANGE(MAX_BACKTOP14(bank), 0x0f, vc);
        MAX96712_BIT_CHANGE(MAX_BACKTOP17(bank), 0x03, dt & 0x03);
        MAX96712_BIT_CHANGE(MAX_BACKTOP16(bank), 0x0f << 4, (dt & 0x3c) << 2);
        MAX96712_BIT_CHANGE(bank ? MAX_BACKTOP31(0) : MAX_BACKTOP25(0), (1 << 6), (1 << 6)); /* enable s/w override */
        break;
    case 3:
        /* Pipe U: 3 or 7 */
        MAX96712_BIT_CHANGE(MAX_BACKTOP19(bank), 0xfc, bpp << 2);
        MAX96712_BIT_CHANGE(MAX_BACKTOP14(bank), 0x0f << 4, vc << 4);
        MAX96712_BIT_CHANGE(MAX_BACKTOP17(bank), 0x3f << 2, dt << 2);
        MAX96712_BIT_CHANGE(bank ? MAX_BACKTOP31(0) : MAX_BACKTOP25(0), (1 << 7), (1 << 7)); /* enable s/w override */
        break;
    }

    return EOK;
}

int max96712_pipe_mipi_mapping(max96712_info_t *max, uint32_t pipe, uint32_t map_n, uint32_t in_dt, 
                                      uint32_t in_vc, uint32_t out_dt, uint32_t out_vc, uint32_t out_mipi)
{
    int offset = 2 * (map_n % 4);

    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_MAP_SRC(pipe, map_n), (in_vc << 6) | in_dt, 0);
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_MAP_DST(pipe, map_n), (out_vc << 6) | out_dt, 0);
    MAX96712_BIT_CHANGE(MAX_MIPI_MAP_DST_PHY(pipe, map_n / 4), 0x03 << offset, out_mipi << offset);
    MAX96712_BIT_CHANGE(MAX_MIPI_TX11(pipe), (1 << map_n), (1 << map_n));	/* enable SRC_n to DST_n mapping */
    
    if(max->gmsl_mode == MODE_GMSL2) {   
        /* Enable SRC_n to DST_n mapping */
        MAX96712_BIT_CHANGE(MAX_MIPI_TX11(pipe), BIT(map_n), BIT(map_n));
        MAX96712_BIT_CHANGE(MAX_MIPI_TX11(pipe), BIT(map_n), BIT(map_n));
        nanospin_ns(20000000);
    }

    return EOK;
}

int max96712_phy_mode_select(max96712_info_t *max)
{
    if(max->csi_phy_mode == CSI2_PHY_MODE_DPHY) {
        /* MIPI1: 4 lanes */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_TX10(1), 0xc0, 0);
        /* MIPI2: 4 lanes */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_TX10(2), 0xc0, 0);
    } else if(max->csi_phy_mode == CSI2_PHY_MODE_CPHY) {
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_TX10(1), 0xa0, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_TX10(2), 0xa0, 0);

        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_PHY13, 0x3F, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_PHY14, 0x7D, 0);
    }

    return EOK;
}

int max96712_mipi_setup(max96712_info_t *max)
{
    /* Disable all pipes */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_VIDEO_PIPE_EN, 0, 0);

    /* Disable all MIPI clocks running force */
    // MAX96712_BIT_CHANGE(MAX_MIPI_PHY0, 0x80, 0x00);
    /* CSI output disable */
    // MAX96712_BIT_CHANGE(MAX_BACKTOP12(0), 0x02, 0);

    /* MIPI Phy 2x4 mode */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_PHY0, 0x04, 0);
    /* Lane map: straight */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_PHY3, 0xe4, 0);
    /* Lane map: straight */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_PHY4, 0xe4, 0);
    /* HS_prepare time, non-inverted polarity */
    //MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_PHY5, 0x10, 0);
    //MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_PHY6, 0x10, 0);

    if(max96712_phy_mode_select(max))
        return -1;

    /* MIPI rate */
    MAX96712_BIT_CHANGE(MAX_BACKTOP22(0), 0x3f, ((max->csi_rate[max->link[0]->out_mipi] / 100) & 0x1f) | (1 << 5));
    MAX96712_BIT_CHANGE(MAX_BACKTOP25(0), 0x3f, ((max->csi_rate[max->link[1]->out_mipi] / 100) & 0x1f) | (1 << 5));
    MAX96712_BIT_CHANGE(MAX_BACKTOP28(0), 0x3f, ((max->csi_rate[max->link[2]->out_mipi] / 100) & 0x1f) | (1 << 5));
    MAX96712_BIT_CHANGE(MAX_BACKTOP31(0), 0x3f, ((max->csi_rate[max->link[3]->out_mipi] / 100) & 0x1f) | (1 << 5));

    /* Enable all MIPI PHYs */   
    MAX96712_BIT_CHANGE(MAX_MIPI_PHY2, 0xf0, 0xf0);

    if(max->phy_inv) {  
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_PHY5, 0x10, 0);
        nanospin_ns(200000000);
    }
    return EOK;
}

int ov490_init(max96712_info_t *max, int link_n)
{
    uint8_t  trans_addr;
    uint8_t  otp[6];
    uint16_t ver;
    uint8_t  val;
    int      len;
    max96712_link_t *link = max->link[link_n];
    sensor_fmt_t *sensor_fmt =  &max->sensor;

    trans_addr = link->ser_addr - 0x6E;
    /* Setup I2C translator address */	
    i2c_8b_write(max->i2c_des_fd, link->ser_addr, 0x09, trans_addr, 0);
    i2c_8b_write(max->i2c_des_fd, link->ser_addr, 0x0A, OV490_I2C_ADDR, 2);

    /* Program table of registers */
    len = sizeof(ov490_conf_regs) / sizeof(ov490_conf_regs[0]);
    for (int i = 0; i < len; i ++) {
        i2c_16b_write(max->i2c_des_fd, trans_addr, ov490_conf_regs[i].reg_addr, ov490_conf_regs[i].value, 2);
    }

    /* Set DVP order */
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_LOW_OFFSET, 0x28, 1);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_ISP_DVP_CTRL3, OV490_DVP_ORDER << 4, 0);

    /* Read product ID and manufacturer ID */
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_LOW_OFFSET, 0x80, 1);     
    if (i2c_16b_read(max->i2c_des_fd, trans_addr, OV490_PID, &val) < 0)
        return -1;

    ver = val << 8;

    if (i2c_16b_read(max->i2c_des_fd, trans_addr, OV490_VER, &val) < 0)
        return -1;

    ver |= val;

    if (ver != OV490_VERSION) {
        fprintf(stderr, "Invalid product ID %x\n", ver);
        return -1;
    }

    /* Read firmware resolution */
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_LOW_OFFSET, 0x82, 1);

    if (i2c_16b_read(max->i2c_des_fd, trans_addr, OV490_ISP_IMAGE0_OHSIZE1, &val) < 0) 
        return -1;

    sensor_fmt->width = val << 8;

    if (i2c_16b_read(max->i2c_des_fd, trans_addr, OV490_ISP_IMAGE0_OHSIZE0, &val) < 0) 
        return -1;

    sensor_fmt->width |= val;

    if (i2c_16b_read(max->i2c_des_fd, trans_addr, OV490_ISP_IMAGE0_OVSIZE1, &val) < 0)
        return -1;

    sensor_fmt->height = val << 8;

    if (i2c_16b_read(max->i2c_des_fd, trans_addr, OV490_ISP_IMAGE0_OVSIZE0, &val) < 0)
        return -1;

    sensor_fmt->height |= val;

    /* Read OPT address */
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_HIGH_OFFSET, 0x80, 1);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_LOW_OFFSET, 0x19, 1);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV10640_WRITE_OPERATION, 0x00, 0);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV10640_HIGH_OFFSET, 0x34, 0);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV10640_LOW_OFFSET, 0x9C, 0);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV10640_REGISTER_VALUE, 1, 0);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_LOW_OFFSET, 0x80, 1);
    i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_HOST_COMMAND, 0xc1, 25);
    for (int i = 0; i < 6; i++) {
        i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_LOW_OFFSET, 0x19, 1);
        i2c_16b_write(max->i2c_des_fd, trans_addr, OV10640_WRITE_OPERATION, 0x01, 0);
        i2c_16b_write(max->i2c_des_fd, trans_addr, OV10640_HIGH_OFFSET, 0x34, 0);
        i2c_16b_write(max->i2c_des_fd, trans_addr, OV10640_LOW_OFFSET, 0x9e + i + 6, 0);
        i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_LOW_OFFSET, 0x80, 1);
        i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_HOST_COMMAND, 0xc1, 0);
        i2c_16b_write(max->i2c_des_fd, trans_addr, OV490_LOW_OFFSET, 0x19, 1);

        if (i2c_16b_read(max->i2c_des_fd, trans_addr, OV10640_WRITE_OPERATION, &otp[i]))
            return -1;
    }

    link->active = 1;

    fprintf(stdout, "Camera %d ov490/ov10640, resolution %dx%d, OTP: %02x-%02x-%02x-%02x-%02x-%02x\n", 
                     link_n, sensor_fmt->width, sensor_fmt->height, otp[0], otp[1], otp[2], otp[3], otp[4], otp[5]);                

    return EOK;
}

int max9295_sensor_init(max96712_info_t *max, int link_n)
{
    int len;
    int ret;
    max96712_link_t *link = max->link[link_n];

    /* SW reset */
    i2c_16b_write(max->i2c_des_fd, link->ser_addr, 0x0010, 0x21, 0); 
    delay(200);

    /* Program table of registers */
    len = ARRAY_SIZE(ar0231_conf_regs);
    for (int i = 0; i < len; i ++) {
        ret = i2c_16b_write(max->i2c_des_fd, link->ser_addr, ar0231_conf_regs[i].reg_addr, ar0231_conf_regs[i].value, 5);
        if(ret)
            return -1;
    }
    delay(200);

    return EOK;
}

int max96712_gmsl1_cc_enable(max96712_info_t *max, int link, int on)
{
    MAX96712_BIT_CHANGE(MAX_GMSL1_4(link), 0x03, on ? 0x03 : 0x00);

    if(max96712_one_reset(max, 0xF))
        return -1;

    return EOK;
}

int max96712_gmsl2_cc_enable(max96712_info_t *max, int link, int on)
{
    /* Do nothing */
    return EOK;
}

int max96712_gmsl1_link_crossbar_setup(max96712_info_t *max, int link, int dt)
{
    switch (dt) {
    case MIPI_DT_YUV8:
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 0), 7, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 1), 6, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 2), 5, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 3), 4, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 4), 3, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 5), 2, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 6), 1, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 7), 0, 0);

        if (max->dbl == 0) {
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 8), 15, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 9), 14, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 10), 13, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 11), 12, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 12), 11, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 13), 10, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 14), 9, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 15), 8, 0);
        }
        break;
    default:
        fprintf(stderr, "crossbar for dt %d is not supported\n", dt);
        break;
    }

    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 24), (max->hsync ? 0x40 : 0) + 24, 0);	/* invert HS polarity */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 25), (max->vsync ? 0 : 0x40) + 25, 0);	/* invert VS polarity */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 26), (max->hsync ? 0x40 : 0) + 26, 0);	/* invert DE polarity */

    return EOK;
}

int max96712_gmsl2_link_crossbar_setup(max96712_info_t *max, int link, int dt)
{
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 24), (max->hsync ? 0x40 : 0) + 24, 0);  /* invert HS polarity */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 25), (max->vsync ? 0 : 0x40) + 25, 0);  /* invert VS polarity */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_CROSS(link, 26), (max->hsync ? 0x40 : 0) + 26, 0);  /* invert DE polarity */

    return EOK;
}
                                         
int max96712_gmsl1_init(max96712_info_t *max)
{
    /* Set GMSL1 mode */
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0xf0, 0);	
    /* 187.5M/3G */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_REG26, 0x11, 0);
    /* 187.5M/3G */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_REG27, 0x11, 0);

    for (int i = 0; i < max->n_links; i++) {
        /* Autodetect serial data rate range */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_2(i), 0x03, 0);
        /* Disable REV/FWD CC */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_4(i), 0, 0);
        /* HIM/Legacy mode */
        MAX96712_BIT_CHANGE(MAX96712_REG6, (1 << 7), (1 << 7));
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_7(i), (max->dbl  ? (1 << 7) : 0) |     /* DBL mode */
                                                             (max->bws  ? (1 << 5) : 0) |     /* BWS 32/24-bit */
                                                             (max->hibw ? (1 << 3) : 0) |     /* High-bandwidth mode */
                                                             (max->hven ? (1 << 2) : 0), 0);  /* HS/VS encoding enable */
        /* Disable artificial ACKs, RC conf disable */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_D(i), 0, 0);
        /* Disable DE processing */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_F(i), 0, 0);
        //* Color map: RAW12 double - i.e. bypass packet as is */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_96(i), (0x13 << 3) | 0x3, 0);
    }

    return EOK;
}

int max96712_gmsl2_init(max96712_info_t *max)
{
    /* set GMSL2 mode */
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0xf0, 0xf0); 
    /* 187.5M/6G */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_REG26, 0x22, 0);        
    /* 187.5M/6G */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_REG27, 0x22, 0);        

    return EOK;
}

int max96712_gmsl1_channel_setup(max96712_info_t *max, int link_n)
{
    max96712_link_t *link = max->link[link_n];
    uint8_t lock_sts;
    int timeout = max->timeout;
    uint8_t val = 0;
    int ret = EOK;
    uint8_t ser_addr[] = { MAX9271_SLAVE_ADDR };

    /* Enable artificial ACKs, RC conf mode */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_D(link_n), 0x81, 0);
    /* Override RC pulse length */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_RLMSC5(link_n), 0xa0, 0);
    /* Override RC rise/fall time */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_RLMSC4(link_n), 0x80, 2);
    /* Enable REV/FWD CC */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_4(link_n), 0x3, 0);
    /* GMSL1 mode, enable GMSL link# */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_REG6, (1 << link_n), 0);
    // MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_REG6, 0xFF, 0);
    max96712_one_reset(max, BIT(link_n));

    for (; timeout > 0; timeout--) {
        if (max->him) {
            /* HIM mode setup */
            i2c_8b_write(max->i2c_des_fd, ser_addr[0], 0x4d, 0xc0, 2);
            /* Wake-up, enable RC, conf_link */
            i2c_8b_write(max->i2c_des_fd, ser_addr[0], 0x04, 0x43, 2);
            if (max->bws) {
                i2c_8b_write(max->i2c_des_fd, ser_addr[0], 0x07, (max->hven ? 0x04 : 0)         /* HS/VS encoding enable */
                                                         | (max->pclk_rise_edge ? 0 : 0x10)  /* PCLK edge */
                                                         | (0x80)                            /* DBL=1 in serializer */
                                                         | (max->bws ? 0x20 : 0), 2);
            }
        } else {
            /* Legacy mode setup */
            /* Override RC Tx amplitude */
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_RLMS95(link_n), 0x88, 2);
            /* Wake-up, enable RC, conf_link */
            i2c_8b_write(max->i2c_des_fd, ser_addr[0], 0x04, 0x43, 2);
            /* RC receiver high threshold enable */
            i2c_8b_write(max->i2c_des_fd, ser_addr[0], 0x08, 0x01, 2);

            if (max->bws) {
                i2c_8b_write(max->i2c_des_fd, ser_addr[0], 0x07, (max->hven ? 0x04 : 0)           /* HS/VS encoding enable */
                                                         | (max->pclk_rise_edge ? 0 : 0x10)    /* PCLK edge */
                                                         | (0x80)                              /* DBL=1 in serializer */
                                                         | (max->bws ? 0x20 : 0), 2);          /* BWS 32/24-bit */
            }

            /* Increase RC Tx amplitude */
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_RLMS95(link_n), 0xd3, 2);
        }
        i2c_8b_read(max->i2c_des_fd, ser_addr[0], 0x1e, &val);
        if (val == MAX9271_ID) {
            link->ser_id = val;
            /* Relocate serizelizer on I2C bus */
            MAX96712_I2C_8B_WRITE(ser_addr[0], 0x01, max->des_sla_addr, 0);
            MAX96712_I2C_8B_WRITE(ser_addr[0], 0x00, link->ser_addr, 2);
            // ov490_init(max, link_n);
            break;
        }

        /* Check if already initialized (after reboot/reset) */
        i2c_8b_read(max->i2c_des_fd, link->ser_addr, 0x1e, &val);
        if (val == MAX9271_ID) {
            link->ser_id = val;
            /* enable RC, conf_link */
            MAX96712_I2C_8B_WRITE(link->ser_addr, 0x04, 0x43, 2);
            ret = -EADDRINUSE;
            break;
        }
    }

    if(link->ser_id == MAX9271_ID) {
        /* Sensor reset/unreset */
        MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0f, (0xfe & ~(1 << 1)), 0);
        /* Set GPIOn direction output */
        MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0e, 0x42 | (1 << 1), 2);
        //Remove-reset
        MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0f, 0xfe | (1 << 1), 300);  

        /* disable artificial ACKs, RC conf disable */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_D(link_n), 0, 2);
        MAX96712_I2C_16B_READ(max->des_sla_addr, MAX_GMSL1_CB(link_n), &lock_sts);
        lock_sts = !!(lock_sts & 0x01);
    }

    if (!timeout) {
        return -ETIMEDOUT;
    }

    max->links_mask |= BIT(link_n);

    fprintf(stdout, " GMSL1 link%d %s %sat 0x%x\n", link_n, chip_name(link->ser_id), 
                          ret == -EADDRINUSE ? "already " : "", link->ser_addr);

    return ret;
}

int max96712_gmsl2_channel_setup(max96712_info_t *max, int link_n)
{
    max96712_link_t *link = max->link[link_n];
    int timeout = max->timeout;
    uint8_t val = 0;
    int ret = 0;
    int index;
    uint16_t ser_addr[] = {MAX9295_SLAVE_ADDR0, MAX9295_SLAVE_ADDR1, 
                            MAX9295_SLAVE_ADDR2, MAX9295_SLAVE_ADDR3};
    
    /* Set GMSL2 mode, enable GMSL link# */
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0xFF, 0xF0 | BIT(link_n));
    if(max96712_one_reset(max, BIT(link_n)))
        return -1;
        
    delay(100);
        
    /* Check if LOCKED bit is high */
    for (; timeout > 0; timeout--) {
        if (max96712_gmsl2_get_link_status(max, link_n))
            break;
        delay(1);
    }

    if (!timeout) {
        return -ETIMEDOUT;
    }
   
    for(index = 0; index < ARRAY_SIZE(ser_addr); index++) {
        i2c_16b_read(max->i2c_des_fd, ser_addr[index], 0x000d, &val);
        if ((val == MAX9295A_ID) || (val == MAX9295B_ID)) {
            link->ser_id = val;
            /* Relocate serizlizer on I2C bus */
            MAX96712_I2C_16B_WRITE(ser_addr[index], 0x0000, link->ser_addr, 0);
             nanospin_ns(2500000);  
            break;
        } 
    }
        
    if(index == ARRAY_SIZE(ser_addr))
        return -ETIMEDOUT;

    max->links_mask |= BIT(link_n);
    link->active = 1;

    fprintf(stdout, " GMSL2 link%d %s %sat 0x%x\n", link_n, chip_name(link->ser_id), 
                          ret == -EADDRINUSE ? "already " : "", link->ser_addr);
    return ret;
}

int max96712_gmsl1_link_ser_setup(max96712_info_t *max, int link_n)
{
    max96712_link_t *link = max->link[link_n];

    /* GMSL setup */
    /* Disable artificial ACK, I2C speed set */
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0d, 0x22 | MAXIM_I2C_SPEED, 0);
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x07, (max->hven ? 0x04 : 0) |           /* HS/VS encoding enable */
                                                (max->pclk_rise_edge ? 0 : 0x10) | /* PCLK edge */
                                                (0x80) |                           /* DBL=1 in serializer */
                                                (max->bws ? 0x20 : 0), 2);         /* BWS 32/24-bit */	 		   
                       
    /* Spread spectrum +-4%, pclk range automatic, Gbps automatic */
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x02, 0xff, 2);

    /* Serializer broadcast I2C translated */
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0B, BROADCAST, 0);
    /* Serializer broadcast I2C native */
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0C, link->ser_addr, 0);
    /* Put serializer in configuration link state  */
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x04, 0x43, 2);

    return EOK;
}

int max96712_gmsl2_link_ser_setup(max96712_info_t *max, int link_n)
{
    max96712_link_t *link = max->link[link_n];
    int i;
    int dt = max->dt;

    if (strcmp(max->mbus, "dvp") == 0) {

        MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_VIDEO_TX0(0),            /* Line CRC enable */
                               BIT(6) | (max->hven ? BIT(5) : 0), 0);           /* HS/VS encoding */
        MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_VIDEO_TX1(0), 0x0a, 0);  /* BPP = 10 */
        MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_REG7, 0x07, 0);          /* DVP stream, enable HS/VS, rising edge */

        switch (dt) {
        case MIPI_DT_YUV8:
        case MIPI_DT_RAW12:
            /* Setup crossbar: strait DVP mapping */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(0), max->cb[0], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(1), max->cb[1], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(2), max->cb[2], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(3), max->cb[3], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(4), max->cb[4], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(5), max->cb[5], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(6), max->cb[6], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(7), max->cb[7], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(8), max->cb[8], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(9), max->cb[9], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(10), max->cb[10], 0);
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_CROSS(11), max->cb[11], 0);
            break;
        }
    } else {
        /* Enable Pipe X from from CSI_A,CSI_B */
        MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_FRONTTOP_0, 0x71, 0);
        /* Primary DT for Pipe X */            
        MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_FRONTTOP_12, BIT(6) | max->dt, 0);
        /* Secondary DT for Pipe X */
        MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_FRONTTOP_13, BIT(6) | MIPI_DT_EMB, 0);
    }

    for (i = 0; i < 11; i++) {
        if (max->gpio[i] == 0) {
            /* GPIO set 0 */
            /* 1MOm, GPIO output low */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_GPIO_A(i), 0x80, 0); 
            /* push-pull, pull-down */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_GPIO_B(i), 0xa0, 0); 
        }
        if (max->gpio[i] == 1) {
            /* GPIO set 1 */
            /* 1MOm, GPIO output high */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_GPIO_A(i), 0x90, 0); 
            /* push-pull, pull-up */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_GPIO_B(i), 0x60, 0); 
        }
        if (max->gpio[i] == 2) {
            /* GPIO FSIN */
            /* 1MOm, GMSL2 RX from deserializer */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_GPIO_A(i), 0x84, 0); 
            /* pull-none, GPIO ID=8 assosiated with FSYNC transmission */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_GPIO_C(i), 0x08, 0); 
        }
        if (max->gpio[i] == 3) {
            /* GPIO Interrupt */
            /* 40kOm, GMSL2 TX to deserializer */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_GPIO_A(i), 0x63, 0); 
            /* push-pull, pull-none, GPIO stream ID=5 */
            MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_GPIO_B(i), 0x25, 0); 
        }
    }

    /* Serializer broadcast I2C translated */
    MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_I2C4, BROADCAST, 0);
    /* Serializer broadcast I2C native */
    MAX96712_I2C_16B_WRITE(link->ser_addr, MAX9295_I2C5, link->ser_addr, 2);

    return EOK;
}

int max96712_gmsl1_link_pipe_setup(max96712_info_t *max, int link_n)
{
    max96712_link_t *link = max->link[link_n];
    int pipe = link_n;
    int dt = max->dt;  
    int in_vc = 0;

    /* Override dt, vc */
    if(max96712_pipe_override(max, pipe, dt, in_vc))
        return -1;

    /* Disable all mappings */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_TX11(pipe), 0x00, 0);
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_TX12(pipe), 0x00, 0);

    /* Use map #0 for payload data */
    if(max96712_pipe_mipi_mapping(max, pipe, 0, dt, in_vc, dt, link->out_vc, link->out_mipi))
        return -1;
    /* Use map #1 for FS */
    if(max96712_pipe_mipi_mapping(max, pipe, 1, 0x00, in_vc, 0x00, link->out_vc, link->out_mipi))
        return -1;                                           
    /* Use map #2 for FE */                                  
    if(max96712_pipe_mipi_mapping(max, pipe, 2, 0x01, in_vc, 0x01, link->out_vc, link->out_mipi))
        return -1;

    delay(5);

    link->pipes_mask |= (1 << pipe);

    return EOK;
}

int max96712_gmsl2_pipe_config_route(max96712_info_t *max, int pipe, int phy, int in_pipe)
{
    int offset = (pipe % 2) * 4;

    MAX96712_BIT_CHANGE(MAX96712_VIDEO_PIPE_SEL(pipe / 2), 0x0f << offset, (phy << (offset + 2)) |
                                       (in_pipe << offset));

    return EOK;
}

int max96712_gmsl2_link_pipe_setup(max96712_info_t *max, int link_n)
{
    max96712_link_t *link = max->link[link_n];
    int pipe = link_n;
    int dt = max->dt;
    int in_vc = 0;
    int i;

    /* Route Pipe X only */
    max96712_gmsl2_pipe_config_route(max, pipe, link_n, 0);          

    if (strcmp(max->mbus, "dvp") == 0) {
        /* Setting stream_id = 0 */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_RX0(pipe), 0, 0);               
        /* override dt, vc */
        max96712_pipe_override(max, pipe, dt, in_vc);         
    }
    
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_RX0(pipe), 0x00, 0);                   
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_VIDEO_RX0(pipe), 0x00, 0);
    max96712_pipe_override(max, pipe, dt, in_vc);     
     nanospin_ns(20000000);
    
    /* Disable all mappings */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_TX11(pipe), 0x00, 0);                   
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_MIPI_TX12(pipe), 0x00, 0);

    for (i = 0; i < ARRAY_SIZE(gmsl2_pipe_mipi_mapping); i++) {
        max96712_pipe_mipi_mapping(max, pipe, i,                              /* pipe, map# */
                          gmsl2_pipe_mipi_mapping[i].in_dt, in_vc,            /* src DT, VC */
                          gmsl2_pipe_mipi_mapping[i].out_dt, link->out_vc,    /* dst DT, VC */
                          link->out_mipi);                                    /* dst MIPI PHY */
    }

    link->pipes_mask |= BIT(pipe);

    return EOK;
}

int max96712_gmsl1_postinit(max96712_info_t *max)
{
    uint8_t val = 0;

    for (int i = 0; i < max->n_links; i++) {
        max96712_link_t *link = max->link[i];

        if (!(max->links_mask & (1 << i)))
            continue;

        /* Enable REV/FWD CC */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_4(i), 0x3, 0);
        /* GMSL1 mode, enable GMSL link# */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_REG6, (1 << i), 0);

        if(max96712_one_reset(max, (1 << i)))
            return -1;

        /* Camera setup */
        if(ov490_init(max, i)) 
            return -1;

        MAX96712_I2C_8B_READ(max->i2c_des_fd, link->ser_addr, 0x15, &val);
        if (!(val & (1 << 1)))
            fprintf(stderr, "Link%d valid PCLK is not detected\n", i);

        /* Switch to GMSL serial_link for streaming video */
        MAX96712_I2C_8B_WRITE(link->ser_addr, 0x04, 0x83, 2);

        /* Disable REV/FWD CC */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_4(i), 0x00, 0);

        /* Use D14/15 for HS/VS */
        MAX96712_BIT_CHANGE(MAX_GMSL1_6(i), 0x07, 0x01);
    }

    for (int i = 0; i < max->n_links; i++)
        /* Enable REV/FWD CC */
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_4(i), max->links_mask & (1 << i) ? 0x03 : 0, 0);

    /* Enable detected links */
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0x0f, max->links_mask);

    if(max96712_one_reset(max, max->links_mask))
        return -1;

    return EOK;
}

int max96712_gmsl2_postinit(max96712_info_t *max)
{
    /* Enable detected links */
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0x0f, max->links_mask);
    /* Enable link pipes */
    MAX96712_BIT_CHANGE(MAX96712_VIDEO_PIPE_EN, max->links_mask, max->links_mask);
    /* CSI output enable */
    MAX96712_BIT_CHANGE(MAX_BACKTOP12(0), 0x02, 0x02);

    if(max96712_one_reset(max, max->links_mask))
        return -1;
        
    delay(100);

    for (int i = 0; i < max->n_links; i++) {
        if (!(max->links_mask & (1 << i)))
            continue;

        if(max9295_sensor_init(max, i))
            return -1;
    }

    return EOK;
}

int max96712_gmsl1_fsync_setup(max96712_info_t *max)
{
    /* Fsync Period L */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_5, MAX96712_FSYNC_PERIOD & 0xff, 0);
    /* Fsync Period M */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_6, (MAX96712_FSYNC_PERIOD >> 8) & 0xff, 0);
    /* Fsync Period H */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_7, MAX96712_FSYNC_PERIOD >> 16, 0);
    /* Disable Overlap */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_10, 0x0, 0);
    // des_write(MAX96712_FSYNC_11, 0x00);
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_11, 0x0, 0);

    /* Manual method, Internal GMSL1 generator mode */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_0, 0x0, 0);
    /* Fsync Tx Enable on Link A */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_8(0), 0x11, 0);
    /* Fsync Tx Enable on Link B */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_8(1), 0x11, 0);
    /* Fsync Tx Enable on Link C */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_8(2), 0x11, 0);
    /* Fsync Tx Enable on Link D */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX_GMSL1_8(3), 0x11, 0);

    /* GMSL1 Type Fsync, Enable all pipes */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_15, 0x1f, 0);

    return EOK;
}

int max96712_gmsl2_fsync_setup(max96712_info_t *max)
{
    /* Fsync Period L */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_5, MAX96712_FSYNC_PERIOD & 0xff, 0);   
    /* Fsync Period M */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_6, (MAX96712_FSYNC_PERIOD >> 8) & 0xff, 0);
    /* Fsync Period H */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_7, MAX96712_FSYNC_PERIOD >> 16, 0);  
    /* Disable Overlap */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_10, 0x00, 0);         
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_11, 0x00, 0);

    /* Manual method, Internal GMSL2 generator mode */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_0, 0x00, 0);          
     /* GMSL2 Type Fsync, Disable all pipes for manual mode */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_15, 0x80, 0);        
    /* GPIO ID=8 assosiated with FSYNC transmission */
    MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_FSYNC_17, 8 << 3, 0);           

    return EOK;
}

int max96712_preinit(max96712_info_t *max)
{
    MAX96712_BIT_CHANGE(MAX96712_PWR1, (1 << 6), (1 << 6));
    if(max->gmsl_mode == MODE_GMSL2) {     
        nanospin_ns(20000000);
    
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, 0x0323, 0x84, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, 0x0325, 0x0B, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, 0x0326, 0x63, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, 0x0327, 0x2C, 0);

        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_REG5, 0x40, 0);
        nanospin_ns(20000000);
    } else
        delay(5);
 
    /* Enable 1.2V VDD supply */
    MAX96712_BIT_CHANGE(MAX96712_CTRL0, (1 << 2), (1 << 2));
    MAX96712_BIT_CHANGE(MAX96712_CTRL2, (1 << 4), (1 << 4));

    /* Set i2c timings */
    for (int i = 0; i < 8; i++) {
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_I2C_0(i), 0x01, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla_addr, MAX96712_I2C_1(i), 0x51, 0);		
    }

    MAX96712_BIT_CHANGE(MAX96712_CTRL11, 0x55, max->is_coax ? 0x55 : 0);
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0x0f, 0);

    return EOK;
}

int max96712_enable(video_info_t *video, int channel, int enable)
{
    max96712_info_t *max = video->dec_data;

    if(enable) {
        /* Require setting for CSI42 and CSI43, inverse polarity */
        if((video->csi_ch == 2) || (video->csi_ch == 3)) {
            MAX96712_I2C_16B_WRITE(max->des_sla_addr, 0x08A5, 0x10, 0);
        }	
#ifdef MAX96712_COLOR_PATTERN_TEST
        max96712_output_enable(max, 1);
#else
        for (int i = 0; i < max->n_links; i++) {
            int pipes_mask = max->link[i]->pipes_mask;
            max96712_gmsl_cc_enable(max, i, enable);
            /* Enable link pipes */
            MAX96712_BIT_CHANGE(MAX96712_VIDEO_PIPE_EN, pipes_mask, pipes_mask);
            /* CSI output enable */
            MAX96712_BIT_CHANGE(MAX_BACKTOP12(0), 0x02, 0x02); 
        }
#endif
    }
    else {
#ifdef MAX96712_COLOR_PATTERN_TEST
        max96712_output_enable(max, 0);
#else
        for (int i = 0; i < max->n_links; i++) {
            int pipes_mask = max->link[i]->pipes_mask;
            max96712_gmsl_cc_enable(max, i, enable);
            /* CSI output disable */
            MAX96712_BIT_CHANGE(MAX_BACKTOP12(0), 0x02, 0); 
            /* Disable link pipes */
            MAX96712_BIT_CHANGE(MAX96712_VIDEO_PIPE_EN, pipes_mask, 0);
        }
#endif
    }

    return EOK;
}

#ifdef MAX96712_COLOR_PATTERN_TEST
int max96712_setup(max96712_info_t *max, uint32_t mipi_chan)
{
    if(max96712_phy_mode_select(max))
        return -1;

    return max96712_color_pattern_setup(max);
}
#else
int max96712_setup(max96712_info_t *max, uint32_t mipi_chan)
{
    uint8_t val;
    int ret;
    int i;

    MAX96712_I2C_16B_READ(max->des_sla_addr, MAX96712_DEV_ID, &val);
    if (val != MAX96712_ID)
        return -1;
    
    for (int i = 0; i < MAX96712_MAX_LINKS; i++) {
        max->link[i] = calloc(1, sizeof(max96712_link_t));
        if (!max->link[i])
            return -1;
    }

    max->n_links        = MAX96712_MAX_LINKS;
    max->is_coax        = ENABLE;
    max->pclk_rise_edge = ENABLE;
    max->timeout        = 50;
    max->him            = DISABLE;
    max->bws            = DISABLE;
    max->dbl            = ENABLE;
    max->hven           = ENABLE;
    max->hibw           = DISABLE;
    max->hsync          = DISABLE;
    max->vsync          = DISABLE;
    max->dt             = MIPI_DT_YUV8;
    if (max->gmsl_mode == MODE_GMSL2) {
        max->dt         = MIPI_DT_RAW10 ;
    } 
    max->crossbar       = 0xba9876543210;
    max->mbus           = mbus_default;

    for (i = 0; i < 11; i++) {
        max->gpio[i] = -1;
    }

    for ( i = 0; i < 16; i++) {
        max->cb[i] = max->crossbar % 16;
        max->crossbar /= 16;
    }

    for ( i = 0; i < max->n_links; i++) {
        if (max->device == 0)
            max->link[i]->ser_addr = 0xA0 + (i * 2);
        else
            max->link[i]->ser_addr = 0xB0 + (i * 2);

        max->link[i]->out_mipi = mipi_chan;
        max->link[i]->out_vc = i;    /* Default VC map: 0 1 2 3 */
        max->csi_rate[max->link[i]->out_mipi] = (max->gmsl_mode == MODE_GMSL1) ? 1200 : 1800;
        if (max->gmsl_mode == MODE_GMSL1) {
            max->csi_rate[max->link[i]->out_mipi] = 1300;        
        }
    }

    if(max96712_preinit(max))
        return -1;

    if(max96712_gmsl_init(max))
        return -1;

    if(max96712_mipi_setup(max))
        return -1;

    for (int link = 0; link < max->n_links; link++) {
        
        if(max96712_gmsl_link_pipe_setup(max, link))
           return -1;
            
        ret = max96712_gmsl_rev_channel_setup(max, link);
        if (ret == -ETIMEDOUT) {
            fprintf(stdout, "Serializer not found\n");
            continue;
        }
        if (!ret) {
           if(max96712_gmsl_link_ser_setup(max, link)) 
                return -1;
        }

        if(max96712_gmsl_link_crossbar_setup(max, link, max->dt))
            return -1;

        if(max96712_gmsl_cc_enable(max, link, 0))
            return -1;
    }

    if(max96712_gmsl_postinit(max))
        return -1;
    
    if(max96712_gmsl_fsync_setup(max))
        return -1;
   
    return EOK;
}
#endif

int sensor_fmt_get(max96712_info_t *max, video_info_t *video)
{
    sensor_fmt_t *sensor_fmt =  &max->sensor;

    video->width = sensor_fmt->width;
    video->height = sensor_fmt->height;
    video->interlace = 0;

    return 0;
}

int max96712_init(video_info_t *video, int device, int board_id)
{
    max96712_info_t *max;
    int i;

    if (!video->dec_data) {
        if ((max = (max96712_info_t *)calloc(1, sizeof(max96712_info_t))) == NULL) {
            fprintf(stderr, "%s: calloc failed\n", __FUNCTION__);
            return -1;
        }
        if (((max->i2c_pcae_fd = open("/dev/i2c0", O_RDWR)) < 0)) {
            fprintf(stderr, "%s: Open %s failed\n", __func__, "/dev/i2c0");
            return -1;
        }

        if (((max->i2c_des_fd = open("/dev/i2c1", O_RDWR)) < 0)) {
            fprintf(stderr, "%s: Open %s failed\n", __func__, "/dev/i2c1");
            return -1;
        }
    } else {
        max = (max96712_info_t *)video->dec_data;
    }

    for (i = 0 ; i < ARRAY_SIZE(dec_info); i++) {
        if (board_id == dec_info[i].board_id) {
            max->dec_cfg       = dec_info[i].dec_cfg;
            max->des_sla_addr  = dec_info[i].dec_cfg[device].des_sla_addr;
            max->pcae_sla_addr = dec_info[i].dec_cfg[device].pcae_sla_addr;
            max->poc_sla_addr  = dec_info[i].dec_cfg[device].poc_sla_addr;
            max->mipi_chan     = dec_info[i].dec_cfg[device].mipi_chan;
            break;
        }
    }

    if (i >= ARRAY_SIZE(dec_info)) {
        fprintf(stderr, "%s: Board is not supported\n", __func__);
        return -1;
    }

    max->i2c_poc_fd   = max->i2c_des_fd;
    max->csi_phy_mode = video->csi_phy_mode;
    max->device       = device;
    max->gmsl_mode    = MODE_GMSL1;
    max->phy_inv      = 0;
    video->dec_data   = max;

    return EOK;
}

int	max96712_update(video_info_t* video, int channel)
{
    max96712_info_t *max = video->dec_data;
    int mipi_chan = max->mipi_chan;

    if (video->gmsl_mode == 2) {
        max->gmsl_mode = MODE_GMSL2;
        if(max->device == 0) 
            max->phy_inv = 1;
    } else {
        max->gmsl_mode = MODE_GMSL1;
    }

    max96712_power(max, 1);

    if(max96712_setup(max, mipi_chan))
        return -1;

    if(max->gmsl_mode == MODE_GMSL1) {
        if(sensor_fmt_get(max, video)) {
            return -1;
        }
    } else { /* MODE_GMSL2 */
         video->width     = 1920;
         video->height    = 1020;
         video->interlace = 0;   /* Progressive */
    }

#ifdef MAX96712_COLOR_PATTERN_TEST
    video->width 	 = 1920;
    video->height 	 = 1080;
    video->interlace = 0;	/* Progressive */
#endif /* MAX96712_COLOR_PATTERN_TEST */

    if(video->csi_lanes == 3)
        video->csi_speed = 1300;
    else if(video->csi_lanes == 4)
        video->csi_speed = 800;

    return EOK;
}

int	max96712_disable(video_info_t* video, int channel)
{
    max96712_info_t *max = video->dec_data;

    if(max) {
        for (int i = 0; i < MAX96712_MAX_LINKS; i++) {
            if (max->link[i]) free(max->link[i]);
        }

        max96712_enable(video, video->csi_ch, 0);

        max96712_power(max, 0);
    }

    return EOK;
}

int	max96712_fini(video_info_t* video, int channel)
{
    max96712_info_t *max = video->dec_data;
    if (video) {
        max = video->dec_data;
        if(max) {
            for (int i = 0; i < MAX96712_MAX_LINKS; i++) {
                if (max->link[i]) free(max->link[i]);
            }

            max96712_enable(video, video->csi_ch, 0);
            max96712_power(max, 0);

            if (max->i2c_pcae_fd != -1)
                close(max->i2c_pcae_fd);
            if (max->i2c_des_fd != -1)
                close(max->i2c_des_fd);

            free(max);
            video->dec_data = NULL;
        }
    }

    return EOK;
}

int max96712_check_input_source(video_info_t* video, int channel)
{
#ifdef MAX96712_COLOR_PATTERN_TEST
    return 1;
#else
    max96712_info_t *max = video->dec_data;

    return (max->link[channel]->active);
#endif
}
