/*
 * MAXIM max96712 GMSL2 driver
 *
 * Copyright (C) 2019-2020 Cogent Embedded, Inc.
 * Copyright 2021, Renesas Electronics Corporation
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */


#include "max96712.h"


#define MAX96712_I2C_8B_WRITE(a, b, c, d) if(i2c_8b_write(max->i2c_des, a, b, c, d)) \
        return -1

#define MAX96712_I2C_8B_READ(a, b, c, d) if(i2c_8b_read(a, b, c, d)) \
        return -1
        
#define MAX96712_I2C_16B_WRITE(a, b, c, d) if(i2c_16b_write(max->i2c_des, a, b, c, d)) \
        return -1

#define MAX96712_I2C_16B_READ(a, b, c) if(i2c_16b_read(max->i2c_des, a, b, c)) \
        return -1

#define MAX96712_BIT_CHANGE(a, b, c) if(max96712_bit_change(max, a, b, c)) \
        return -1

max96712_info_t *max;

uint8_t max96712_slave_addr[4] = 
{
    MAX96712_0_SLA_ADDR, 
    MAX96712_0_SLA_ADDR, 
    MAX96712_1_SLA_ADDR, 
    MAX96712_2_SLA_ADDR
};

uint8_t pcae9654_slave_addr[4] = 
{
    PCAE9654_0_SLA, 
    PCAE9654_0_SLA, 
    PCAE9654_1_SLA, 
    PCAE9654_2_SLA
};

uint8_t max20087_slave_addr[4] = 
{
    MAX20087_0_SLA_ADDR, 
    MAX20087_0_SLA_ADDR, 
    MAX20087_1_SLA_ADDR, 
    MAX20087_2_SLA_ADDR
};

int i2c_8b_read(int fd, uint8_t sla_addr, uint8_t reg_addr, uint8_t *data)
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

    return 0;
}

int i2c_8b_write(int fd, uint8_t sla_addr, uint8_t reg_addr, uint8_t data, int mdelay)
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
	
    return 0;
}

int i2c_16b_write(int fd, uint8_t sla_addr, uint16_t reg_addr, uint8_t data, int mdelay)
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

    return 0;
}

int i2c_16b_read(int fd, uint8_t sla_addr, uint16_t reg_addr, uint8_t *data)
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

    return 0;
}

int max96712_bit_change(max96712_info_t *max, uint16_t reg, uint8_t mask, uint8_t val) 
{ 
    int     ret; 
    uint8_t tmp, ori; 
 
    ret = i2c_16b_read(max->i2c_des, max->des_sla, reg, &ori); 
    if (ret != 0) 
        return ret; 
 
    tmp = ori & ~mask; 
    tmp |= val & mask; 
 
    if (tmp != ori) 
        ret = i2c_16b_write(max->i2c_des, max->des_sla, reg, tmp, 0); 
 
    return ret; 
} 

int max96712_enable(video_info_t *video, int channel, int enable)
{
    if(enable) {
        /* Require setting for CSI42 and CSI43, inverse polarity */
        if((video->csi2_ch == 2) || (video->csi2_ch == 3)) {
            MAX96712_I2C_16B_WRITE(max->des_sla, 0x08A5, 0x10, 0);
        }	
#if MAX96712_COLOR_PATTERN_TEST
        max96712_output_enable(max, 1);
#else
        for (int i = 0; i < max->n_links; i++) {
            int pipes_mask = max->link[i]->pipes_mask;
            max96712_gmsl1_cc_enable(max, i, enable);
            /* Enable link pipes */
            MAX96712_BIT_CHANGE(MAX96712_VIDEO_PIPE_EN, pipes_mask, pipes_mask);
            /* CSI output enable */
            MAX96712_BIT_CHANGE(MAX_BACKTOP12(0), 0x02, 0x02); 
        }
#endif
    }
    else {
#if MAX96712_COLOR_PATTERN_TEST
        max96712_output_enable(max, 0);
#else
        for (int i = 0; i < max->n_links; i++) {
            int pipes_mask = max->link[i]->pipes_mask;
            max96712_gmsl1_cc_enable(max, i, enable);
            /* CSI output disable */
            MAX96712_BIT_CHANGE(MAX_BACKTOP12(0), 0x02, 0); 
            /* Disable link pipes */
            MAX96712_BIT_CHANGE(MAX96712_VIDEO_PIPE_EN, pipes_mask, 0);
        }
#endif
    }

    return 0;
}

int max96712_power(max96712_info_t *max, int on)
{
    uint8_t data;
    
    // Polarity Select
    if(i2c_8b_write(max->i2c_pcae, max->pcae_sla, PCAE9654_POL_INV, 0x00, 0)) 
		return -1;

    // Select IO0 output
    if(i2c_8b_write(max->i2c_pcae, max->pcae_sla, PCAE9654_CONFIG, 0xFF & ~(1 << 0), 0)) 
		return -1;
    
    MAX96712_I2C_8B_READ(max->i2c_pcae, max->pcae_sla, PCAE9654_OUT_PORT, &data);

    if(i2c_8b_write(max->i2c_pcae, max->pcae_sla, PCAE9654_OUT_PORT, on ? (data | (1 << 0)) : (data & ~(1 << 0)), 0)) 
        return -1;

    return 0;
}

int gmsl_cam_power(max96712_info_t *max, int on)
{
    uint8_t data;

    MAX96712_I2C_16B_READ(max->des_sla, 0x30C, &data);
    data |= (1 << 4);		// GPIO4 = High = Camemra POC Protector ON
    MAX96712_I2C_16B_WRITE(max->des_sla, 0x30C, data, 0);

    // MAX20087 Reg 0x02(ID)
    MAX96712_I2C_8B_READ(max->i2c_poc, max->poc_sla, 0x02, &data);

    if((data >> 4) != 2) {	// 0x2x: MAX20087
        fprintf(stderr, "Wrong ID code:0x%02x (not MAX20087)\n", data);
        return -1;
    }

    // MAX20087 Reg 0x01(CONFIG)
    MAX96712_I2C_8B_READ(max->i2c_poc, max->poc_sla, 0x01, &data);
    on ? (data |= (0xF)) : (data &= ~(0xF));
    if(i2c_8b_write(max->i2c_poc, max->poc_sla, 0x01, data, 0))
		return -1;
    
    return 0;
}

int i2c_table_write(max96712_info_t *max, uint32_t *set_data)
{
    uint16_t reg_addr;
    uint8_t  data;

    while(*set_data != 0xFFFFFFFF) {
        reg_addr  = *set_data & 0xFFFF;
        set_data++;
        data = *set_data & 0xFF;
        set_data++;
        // printf("W CH:0x%02x SLA:0x%02x REG:0x%04x DATA: 0x%x \r\n", max->i2c_des, max->des_sla, reg_addr, data);
        MAX96712_I2C_16B_WRITE(max->des_sla, reg_addr, data, 1);
    }

    return 0;
}

uint32_t max96712_color_pattern_setup(max96712_info_t *max)
{
    uint32_t vs_low, vs_high, hs_low, hs_high, hs_count;
    uint32_t v2d, de_high, de_low, de_count;
    
    model_line_t def_res[] = {
        // FullHD(1920x1080p)
        { "1920x1080", "148.50", 1920, 2008, 2052, 2200, 1080, 1084, 1089, 1125, "+hsync", "+vsync", 0, 0, 0 },
    };

    vs_low   =  def_res[0].v_end - def_res[0].v_start;
    vs_high  =  def_res[0].v_total - vs_low;
    vs_low   *= def_res[0].h_total;		// PCLK
    vs_high  *= def_res[0].h_total;		// PCLK
    hs_low   =  def_res[0].h_end - def_res[0].h_start;
    hs_high  =  def_res[0].h_total - hs_low;
    hs_count =  def_res[0].v_total;
    // v2d      =  def_res[0].v_total - def_res[0].v_end;
    v2d      =  def_res[0].v_total - def_res[0].v_start;
    v2d      *= def_res[0].h_total;		// PCLK
    v2d      += (def_res[0].h_total - def_res[0].h_start);
    de_high  =  def_res[0].h_disp;
    de_low   =  def_res[0].h_total - de_high;
    de_count =  def_res[0].v_disp;

    // VS delay=0 (PCLK)
    max96712_color_pattern_generator[5]		 = 0;
    max96712_color_pattern_generator[5 + 2]	 = 0;
    max96712_color_pattern_generator[5 + 4]	 = 0;
    // VS high (PCLK)                        
    max96712_color_pattern_generator[5 + 6]	 = (vs_high >> 16) & 0x0ff;
    max96712_color_pattern_generator[5 + 8]	 = (vs_high >>  8) & 0x0ff;
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
    // HS low                                   hs
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

    return 0;
}	

int max96712_one_reset(max96712_info_t *max, int mask)
{
    uint8_t reg = 0;

    mask &= 0x0f;
    MAX96712_BIT_CHANGE(MAX96712_CTRL1, mask, mask); /* set reset one-shot */

    /* wait for one-shot bit self-cleared */
    for (int timeout = 0; timeout < 100; timeout++) {
        MAX96712_I2C_16B_READ(max->des_sla, MAX96712_CTRL1, &reg);
        if (!(reg & mask))
            break;
        delay(1);
    }

    if (reg & mask) {
        fprintf(stderr, "Failed reset oneshot 0x%x\n", mask);
        return -1;
    }
	
	delay(2);
    
    return 0;
}

int max96712_pipe_override(max96712_info_t *max, uint32_t pipe, uint32_t dt, uint32_t vc)
{
    int bpp, bank;

    bpp = mipi_dt_to_bpp(dt);
    bank = pipe / 4;
    pipe %= 4;

    if (max->dbl == 1) {
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
    
    return 0;
}

int max96712_pipe_mipi_mapping(max96712_info_t *max, uint32_t pipe, uint32_t map_n, uint32_t in_dt, 
                                      uint32_t in_vc, uint32_t out_dt, uint32_t out_vc, uint32_t out_mipi)
{
    int offset = 2 * (map_n % 4);

    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_MAP_SRC(pipe, map_n), (in_vc << 6) | in_dt, 0);
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_MAP_DST(pipe, map_n), (out_vc << 6) | out_dt, 0);
    MAX96712_BIT_CHANGE(MAX_MIPI_MAP_DST_PHY(pipe, map_n / 4), 0x03 << offset, out_mipi << offset);
    MAX96712_BIT_CHANGE(MAX_MIPI_TX11(pipe), (1 << map_n), (1 << map_n));	/* enable SRC_n to DST_n mapping */
    
    return 0;
}

int max96712_mipi_setup(max96712_info_t *max)
{
    /* disable all pipes */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_VIDEO_PIPE_EN, 0, 0);
    
    /* Disable all MIPI clocks running force */
    MAX96712_BIT_CHANGE(MAX_MIPI_PHY0, 0x80, 0x00);
    /* CSI output disable */
    MAX96712_BIT_CHANGE(MAX_BACKTOP12(0), 0x02, 0);

    /* MIPI Phy 2x4 mode */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_PHY0, 0x04, 0);
    /* Lane map: straight */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_PHY3, 0xe4, 0);
    /* Lane map: straight */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_PHY4, 0xe4, 0);
    /* HS_prepare time, non-inverted polarity */
    // MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_PHY5, 0x10, 0);
    // MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_PHY6, 0x10, 0);

    /* MIPI1: 4 lanes */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_TX10(1), 0xc0, 0);
    /* MIPI2: 4 lanes */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_TX10(2), 0xc0, 0);

    /* MIPI rate */
	MAX96712_BIT_CHANGE(MAX_BACKTOP22(0), 0x3f, ((max->csi_rate[max->link[0]->out_mipi] / 100) & 0x1f) | (1 << 5));
	MAX96712_BIT_CHANGE(MAX_BACKTOP25(0), 0x3f, ((max->csi_rate[max->link[1]->out_mipi] / 100) & 0x1f) | (1 << 5));
	MAX96712_BIT_CHANGE(MAX_BACKTOP28(0), 0x3f, ((max->csi_rate[max->link[2]->out_mipi] / 100) & 0x1f) | (1 << 5));
	MAX96712_BIT_CHANGE(MAX_BACKTOP31(0), 0x3f, ((max->csi_rate[max->link[3]->out_mipi] / 100) & 0x1f) | (1 << 5));

    /* Enable all MIPI PHYs */
    MAX96712_BIT_CHANGE(MAX_MIPI_PHY2, 0xf0, 0xf0);
    
    return 0;
}

int ov490_init(max96712_info_t *max, int link_n)
{
    uint8_t  trans_addr;
    int      width, height;
    uint8_t  otp[6];
    uint16_t ver;
    uint8_t  val;
    int      len;
    max96712_link_t *link = max->link[link_n];

    trans_addr = link->ser_addr - 0x6E;
    /* Setup I2C translator address */	
    i2c_8b_write(max->i2c_des, link->ser_addr, 0x09, trans_addr, 0);
    i2c_8b_write(max->i2c_des, link->ser_addr, 0x0A, OV490_I2C_ADDR, 2);

    
    /* Program table of registers */
    len = sizeof(ov490_conf_regs) / sizeof(ov490_conf_regs[0]);
    for (int i = 0; i < len; i ++) {
        i2c_16b_write(max->i2c_des, trans_addr, ov490_conf_regs[i].reg_addr, ov490_conf_regs[i].value, 2);
    }

    /* Set DVP order */
    i2c_16b_write(max->i2c_des, trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    i2c_16b_write(max->i2c_des, trans_addr, OV490_LOW_OFFSET, 0x28, 1);
    i2c_16b_write(max->i2c_des, trans_addr, OV490_ISP_DVP_CTRL3, OV490_DVP_ORDER << 4, 0);
            
    /* Read product ID and manufacturer ID */
    i2c_16b_write(max->i2c_des, trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    i2c_16b_write(max->i2c_des, trans_addr, OV490_LOW_OFFSET, 0x80, 1);     
    if (i2c_16b_read(max->i2c_des, trans_addr, OV490_PID, &val) < 0)
        return -1;

    ver = val << 8;
    
    if (i2c_16b_read(max->i2c_des, trans_addr, OV490_VER, &val) < 0)
        return -1;

    ver |= val;

    if (ver != OV490_VERSION) {
        fprintf(stderr, "Invalid product ID %x\n", ver);
        return -1;
    }
    
    /* Read firmware resolution */
    i2c_16b_write(max->i2c_des, trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    i2c_16b_write(max->i2c_des, trans_addr, OV490_LOW_OFFSET, 0x82, 1);

    if (i2c_16b_read(max->i2c_des, trans_addr, OV490_ISP_IMAGE0_OHSIZE1, &val) < 0) 
        return -1;

    width = val << 8;

    if (i2c_16b_read(max->i2c_des, trans_addr, OV490_ISP_IMAGE0_OHSIZE0, &val) < 0) 
        return -1;

    width |= val;

    if (i2c_16b_read(max->i2c_des, trans_addr, OV490_ISP_IMAGE0_OVSIZE1, &val) < 0)
        return -1;

    height = val << 8;
    
    if (i2c_16b_read(max->i2c_des, trans_addr, OV490_ISP_IMAGE0_OVSIZE0, &val) < 0)
        return -1;

    height |= val;

    /* Read OPT address */
    i2c_16b_write(max->i2c_des, trans_addr, OV490_HIGH_OFFSET, 0x80, 1);
    i2c_16b_write(max->i2c_des, trans_addr, OV490_LOW_OFFSET, 0x19, 1);
    i2c_16b_write(max->i2c_des, trans_addr, OV10640_WRITE_OPERATION, 0x00, 0);
    i2c_16b_write(max->i2c_des, trans_addr, OV10640_HIGH_OFFSET, 0x34, 0);
    i2c_16b_write(max->i2c_des, trans_addr, OV10640_LOW_OFFSET, 0x9C, 0);
    i2c_16b_write(max->i2c_des, trans_addr, OV10640_REGISTER_VALUE, 1, 0);
    i2c_16b_write(max->i2c_des, trans_addr, OV490_LOW_OFFSET, 0x80, 1);
    i2c_16b_write(max->i2c_des, trans_addr, OV490_HOST_COMMAND, 0xc1, 25);
    for (int i = 0; i < 6; i++) {
        i2c_16b_write(max->i2c_des, trans_addr, OV490_LOW_OFFSET, 0x19, 1);
        i2c_16b_write(max->i2c_des, trans_addr, OV10640_WRITE_OPERATION, 0x01, 0);
        i2c_16b_write(max->i2c_des, trans_addr, OV10640_HIGH_OFFSET, 0x34, 0);
        i2c_16b_write(max->i2c_des, trans_addr, OV10640_LOW_OFFSET, 0x9e + i + 6, 0);
        i2c_16b_write(max->i2c_des, trans_addr, OV490_LOW_OFFSET, 0x80, 1);
        i2c_16b_write(max->i2c_des, trans_addr, OV490_HOST_COMMAND, 0xc1, 0);
        i2c_16b_write(max->i2c_des, trans_addr, OV490_LOW_OFFSET, 0x19, 1);
        
        if (i2c_16b_read(max->i2c_des, trans_addr, OV10640_WRITE_OPERATION, &otp[i]))
            return -1;
    }

    fprintf(stdout, "Camera %d ov490/ov10640, resolution %dx%d, OTP: %02x-%02x-%02x-%02x-%02x-%02x\n", 
                     link_n, width, height, otp[0], otp[1], otp[2], otp[3], otp[4], otp[5]);                

    return 0;
}


int max96712_gmsl1_cc_enable(max96712_info_t *max, int link, int on)
{
    MAX96712_BIT_CHANGE(MAX_GMSL1_4(link), 0x03, on ? 0x03 : 0x00);
    
    if(max96712_one_reset(max, 0xF))
        return -1;

    return 0;
}

int max96712_gmsl1_link_crossbar_setup(max96712_info_t *max, int link, int dt)
{
    switch (dt) {
    case MIPI_DT_YUV8:
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 0), 7, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 1), 6, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 2), 5, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 3), 4, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 4), 3, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 5), 2, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 6), 1, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 7), 0, 0);

        if (max->dbl == 0) {
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 8), 15, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 9), 14, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 10), 13, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 11), 12, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 12), 11, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 13), 10, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 14), 9, 0);
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 15), 8, 0);
        }
        break;
    default:
        fprintf(stderr, "crossbar for dt %d is not supported\n", dt);
        break;
    }

    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 24), (max->hsync ? 0x40 : 0) + 24, 0);	/* invert HS polarity */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 25), (max->vsync ? 0 : 0x40) + 25, 0);	/* invert VS polarity */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_CROSS(link, 26), (max->hsync ? 0x40 : 0) + 26, 0);	/* invert DE polarity */

    return 0;
}
                                         
int max96712_gmsl1_init(max96712_info_t *max)
{
    /* Set GMSL1 mode */
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0xf0, 0);	
    /* 187.5M/3G */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_REG26, 0x11, 0);
    /* 187.5M/3G */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_REG27, 0x11, 0);

    for (int i = 0; i < max->n_links; i++) {
        /* Autodetect serial data rate range */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_2(i), 0x03, 0);
        /* Disable REV/FWD CC */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_4(i), 0, 0);
        /* HIM/Legacy mode */
        MAX96712_BIT_CHANGE(MAX96712_REG6, (1 << 7), (1 << 7));
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_7(i), (max->dbl  ? (1 << 7) : 0) |  	  /* DBL mode */
                                                             (max->bws  ? (1 << 5) : 0) |     /* BWS 32/24-bit */
                                                             (max->hibw ? (1 << 3) : 0) |     /* High-bandwidth mode */
                                                             (max->hven ? (1 << 2) : 0), 0);  /* HS/VS encoding enable */	   
        /* Disable artificial ACKs, RC conf disable */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_D(i), 0, 0);
        /* Disable DE processing */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_F(i), 0, 0);
        //* Color map: RAW12 double - i.e. bypass packet as is */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_96(i), (0x13 << 3) | 0x3, 0);
    }
    
    return 0;
}

int max96712_gmsl1_rev_channel_setup(max96712_info_t *max, int link_n)
{
    max96712_link_t *link = max->link[link_n];
    int ser_addrs[] = { MAX9271_SLAVE_ADDR };
    uint8_t lock_sts;
    int timeout = max->timeout;
    char timeout_str[40];
    uint8_t val = 0;
    int ret = 0;

    /* Enable artificial ACKs, RC conf mode */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_D(link_n), 0x81, 0);
    /* Override RC pulse length */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_RLMSC5(link_n), 0xa0, 0);
    /* Override RC rise/fall time */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_RLMSC4(link_n), 0x80, 2);
    /* Enable REV/FWD CC */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_4(link_n), 0x3, 0);
    /* GMSL1 mode, enable GMSL link# */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_REG6, (1 << link_n), 0);
    max96712_one_reset(max, (1 << link_n));

    for (; timeout > 0; timeout--) {
        if (max->him) {
            /* HIM mode setup */
            i2c_8b_write(max->i2c_des, ser_addrs[0], 0x4d, 0xc0, 2);
            /* Wake-up, enable RC, conf_link */
            i2c_8b_write(max->i2c_des, ser_addrs[0], 0x04, 0x43, 2);
            if (max->bws) {
                i2c_8b_write(max->i2c_des, ser_addrs[0], 0x07, (max->hven ? 0x04 : 0) | 		  /* HS/VS encoding enable */
                                                               (max->pclk_rise_edge ? 0 : 0x10) | /* PCLK edge */
                                                               (0x80) | 						  /* DBL=1 in serializer */
                                                               (max->bws ? 0x20 : 0), 2);
            }
        } else {
            /* Legacy mode setup */
            /* Override RC Tx amplitude */
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_RLMS95(link_n), 0x88, 2);
            /* Wake-up, enable RC, conf_link */
            i2c_8b_write(max->i2c_des, ser_addrs[0], 0x04, 0x43, 2);
            /* RC receiver high threshold enable */
            i2c_8b_write(max->i2c_des, ser_addrs[0], 0x08, 0x01, 2);

            if (max->bws) {
                i2c_8b_write(max->i2c_des, ser_addrs[0], 0x07, (max->hven ? 0x04 : 0) | 			/* HS/VS encoding enable */
                                                               (max->pclk_rise_edge ? 0 : 0x10) |   /* PCLK edge */
                                                               (0x80) | 							/* DBL=1 in serializer */
                                                               (max->bws ? 0x20 : 0), 2); 			/* BWS 32/24-bit */
            }

            /* Increase RC Tx amplitude */
            MAX96712_I2C_16B_WRITE(max->des_sla, MAX_RLMS95(link_n), 0xd3, 2);
        }

        i2c_8b_read(max->i2c_des, ser_addrs[0], 0x1e, &val);
        if (val == MAX9271_ID) {
            link->ser_id = val;
            /* Relocate serizlizer on I2C bus */
            MAX96712_I2C_8B_WRITE(ser_addrs[0], 0x01, max->des_sla, 0);
            MAX96712_I2C_8B_WRITE(ser_addrs[0], 0x00, link->ser_addr, 2);
            // ov490_init(max, link_n);
            break;
        }

        /* Check if already initialized (after reboot/reset) */
        i2c_8b_read(max->i2c_des, link->ser_addr, 0x1e, &val);
        if (val == MAX9271_ID) {
            link->ser_id = val;
            /* enable RC, conf_link */
            MAX96712_I2C_8B_WRITE(link->ser_addr, 0x04, 0x43, 2);
            ret = -EADDRINUSE;
            break;
        }
    }
    
    /* Sensor reset/unreset */
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0f, (0xfe & ~(1 << 1)), 0);
    /* Set GPIOn direction output */
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0e, 0x42 | (1 << 1), 2);
    //Remove-reset
    MAX96712_I2C_8B_WRITE(link->ser_addr, 0x0f, 0xfe | (1 << 1), 300);  

    /* disable artificial ACKs, RC conf disable */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_D(link_n), 0, 2);
    MAX96712_I2C_16B_READ(max->des_sla, MAX_GMSL1_CB(link_n), &lock_sts);
    lock_sts = !!(lock_sts & 0x01);

    if (!timeout) {
        ret = -ETIMEDOUT;
        goto out;
    }

    max->links_mask |= (1 << link_n);

out:
    sprintf(timeout_str, "retries=%d lock_sts=%d", max->timeout - timeout, lock_sts);
    fprintf(stdout, " GMSL1 link%d %s %sat 0x%x %s %s\n", link_n, chip_name(link->ser_id),
                   ret == -EADDRINUSE ? "already " : "", link->ser_addr,
                   ret == -ETIMEDOUT ? "not found: timeout GMSL link establish" : "",
                   max->timeout - timeout ? timeout_str : "");
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

    return 0;
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
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_TX11(pipe), 0x00, 0);
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_MIPI_TX12(pipe), 0x00, 0);
    
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
    
    return 0;
}

int max96712_gmsl1_postinit(max96712_info_t *max)
{
    uint8_t val = 0;

    for (int i = 0; i < max->n_links; i++) {
        max96712_link_t *link = max->link[i];

        if (!(max->links_mask & (1 << i)))
            continue;
        
        /* Enable REV/FWD CC */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_4(i), 0x3, 0);
        /* GMSL1 mode, enable GMSL link# */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_REG6, (1 << i), 0);
        
        if(max96712_one_reset(max, (1 << i)))
            return -1;
        
        /* Camera setup */
        if(ov490_init(max, i)) 
            return -1;

        MAX96712_I2C_8B_READ(max->i2c_des, link->ser_addr, 0x15, &val);
        if (!(val & (1 << 1)))
            fprintf(stderr, "Link%d valid PCLK is not detected\n", i);

        /* Switch to GMSL serial_link for streaming video */
        MAX96712_I2C_8B_WRITE(link->ser_addr, 0x04, 0x83, 2);

        /* Disable REV/FWD CC */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_4(i), 0x00, 0);

        /* Use D14/15 for HS/VS */
        MAX96712_BIT_CHANGE(MAX_GMSL1_6(i), 0x07, 0x01);
    }

    for (int i = 0; i < max->n_links; i++)
        /* Enable REV/FWD CC */
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_4(i), max->links_mask & (1 << i) ? 0x03 : 0, 0);

    /* Enable detected links */
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0x0f, max->links_mask);
    
    if(max96712_one_reset(max, max->links_mask))
        return -1;
    
    return 0;
}

int max96712_gmsl1_fsync_setup(max96712_info_t *max)
{
    /* Fsync Period L */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_FSYNC_5, MAX96712_FSYNC_PERIOD & 0xff, 0);
    /* Fsync Period M */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_FSYNC_6, (MAX96712_FSYNC_PERIOD >> 8) & 0xff, 0);
    /* Fsync Period H */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_FSYNC_7, MAX96712_FSYNC_PERIOD >> 16, 0);
    /* Disable Overlap */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_FSYNC_10, 0x0, 0);
    // des_write(MAX96712_FSYNC_11, 0x00);
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_FSYNC_11, 0x0, 0);

    /* Manual method, Internal GMSL1 generator mode */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_FSYNC_0, 0x0, 0);
    /* Fsync Tx Enable on Link A */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_8(0), 0x11, 0);
    /* Fsync Tx Enable on Link B */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_8(1), 0x11, 0);
    /* Fsync Tx Enable on Link C */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_8(2), 0x11, 0);
    /* Fsync Tx Enable on Link D */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX_GMSL1_8(3), 0x11, 0);

    /* GMSL1 Type Fsync, Enable all pipes */
    MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_FSYNC_15, 0x1f, 0);
    
    return 0;
}

int max96712_preinit(max96712_info_t *max)
{
    MAX96712_BIT_CHANGE(MAX96712_PWR1, (1 << 6), (1 << 6));
    delay(5);

    /* enable internal regulator for 1.2V VDD supply */
    MAX96712_BIT_CHANGE(MAX96712_CTRL0, (1 << 2), (1 << 2));
    MAX96712_BIT_CHANGE(MAX96712_CTRL2, (1 << 4), (1 << 4));

    /* I2C-I2C timings */
    for (int i = 0; i < 8; i++) {
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_I2C_0(i), 0x01, 0);
        MAX96712_I2C_16B_WRITE(max->des_sla, MAX96712_I2C_1(i), 0x51, 0);		
    }

    MAX96712_BIT_CHANGE(MAX96712_CTRL11, 0x55, max->is_coax ? 0x55 : 0);
    MAX96712_BIT_CHANGE(MAX96712_REG6, 0x0f, 0);

    return 0;
}


#if MAX96712_COLOR_PATTERN_TEST
int max96712_setup(max96712_info_t *max, uint32_t csi_chan)
{
    return max96712_color_pattern_setup(max);
}
#else
int max96712_setup(max96712_info_t *max, uint32_t csi_chan)
{	
    uint8_t val;
    int 	ret;
    
    MAX96712_I2C_16B_READ(max->des_sla, MAX96712_DEV_ID, &val);
    if (val != MAX96712_ID)
        return -1;
    
    for (int i = 0; i < MAX96712_MAX_LINKS; i++) {
        max->link[i] = malloc(sizeof(max96712_link_t));
        if (!max->link[i])
            return -1;
    }
    
    max->n_links	 	= MAX96712_MAX_LINKS;
    max->is_coax 		= ENABLE;
    max->pclk_rise_edge = ENABLE;
    max->timeout 		= 2;
    max->him 			= DISABLE;
    max->bws 			= DISABLE;
    max->dbl			= ENABLE;
    max->hven 			= ENABLE;
    max->hibw 			= DISABLE;
    max->hsync 			= DISABLE;
    max->vsync 			= DISABLE;
    max->dt 			= MIPI_DT_YUV8;
    
    for (int i = 0; i < max->n_links; i++) {
        max->link[i]->ser_addr = 0xA0 + (i * 2);
		if((csi_chan == 0) || (csi_chan == 2) || (csi_chan == 3))
			max->link[i]->out_mipi = MAX96712_CSI_OUT1;	/* CSI forwarding is to MIPI1 */
		else if(csi_chan == 1) {
			max->link[i]->out_mipi = MAX96712_CSI_OUT2;	/* CSI forwarding is to MIPI2 */
		}
        max->link[i]->out_vc = i;	/* Default VC map: 0 1 2 3 */
        max->csi_rate[max->link[i]->out_mipi] = 1200;
    }
    
    if(max96712_preinit(max))
        return -1;
    
    if(max96712_gmsl1_init(max))
        return -1;
    
    if(max96712_mipi_setup(max))
        return -1;

    for (int link = 0; link < max->n_links; link++) {
        ret = max96712_gmsl1_rev_channel_setup(max, link);
        if (ret == -ETIMEDOUT)
            continue;
        if (!ret) {
            if(max96712_gmsl1_link_ser_setup(max, link)) 
                return -1;
        }

        if(max96712_gmsl1_link_pipe_setup(max, link))
            return -1;
        if(max96712_gmsl1_link_crossbar_setup(max, link, max->dt))
            return -1;

        if(max96712_gmsl1_cc_enable(max, link, 0))
            return -1;
    }
    
    if(max96712_gmsl1_postinit(max))
        return -1;
    
    if(max96712_gmsl1_fsync_setup(max))
        return -1;
    
    return 0;
}
#endif


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
    
    return 0;
}

int max96712_init(video_info_t *video, int channel)
{
    max = (max96712_info_t *)malloc(sizeof(max96712_info_t));

    if ((max->i2c_pcae = open("/dev/i2c0", O_RDWR)) < 0) {
        fprintf(stderr, "%s: Open %s failed\n", __func__, "/dev/i2c0");
        return -1;
    }

    if ((max->i2c_des = open("/dev/i2c1", O_RDWR)) < 0) {
        fprintf(stderr, "%s: Open %s failed\n", __func__, "/dev/i2c1");
        return -1;
    }

    max->des_sla 	= max96712_slave_addr[video->csi2_ch];
    max->pcae_sla	= pcae9654_slave_addr[video->csi2_ch];
    max->i2c_poc	= max->i2c_des;
    max->poc_sla	= max20087_slave_addr[video->csi2_ch];

    // Initialize I2C0 -> Control PCAE9654 -> MAX96712 PWEN = Low = Power OFF
    if(max96712_power(max, 0))
        return -1;
    delay(300);

    // Initialize I2C0 -> Control PCAE9654 -> MAX96712 PWEN = High = Power ON
    if(max96712_power(max, 1))
        return -1;
    delay(300);

    if(gmsl_cam_power(max, 1))
        return -1;

    if(max96712_setup(max, video->csi2_ch))
        return -1;

#if MAX96712_COLOR_PATTERN_TEST
    video->width 	 = 1920;
    video->height 	 = 1080;
    video->interlace = 0;	/* Progressive */
    video->csi_speed = 800;
#else
    video->width 	 = 1280;
    video->height 	 = 1080;
    video->interlace = 0;	/* Progressive */
    video->csi_speed = 800;
#endif
    
    return 0;
}

int	max96712_fini(video_info_t* video, int channel)
{
    if(max->i2c_des > 0) {	
        max96712_enable(video, video->csi2_ch, 0);
        gmsl_cam_power(max, 0);
        close(max->i2c_des);
    }

    if(max->i2c_pcae > 0) {
        max96712_power(max, 0);
        close(max->i2c_pcae);
    }

    if(max)
        free(max);

    return 0;
}

int	max96712_update(video_info_t* video, int channel)
{
    return 0;
}
