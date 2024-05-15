/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2017, Renesas Electronics Corporation.
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

#include "max9286.h"


#define MAX9286_I2C_WRITE(a, b, c, d) if(max9286_i2c_write(i2c_fd, a, b, c, d)) \
        return -1
        
#define MAX9286_16BIT_I2C_WRITE(a, b, c, d) if(max9286_16bit_i2c_write(i2c_fd, a, b, c, d)) \
        return -1
        
#define RCAR_YUV8_SIZE         8
#define RCAR_PHYSICAL_CLOCK    100
#define RCAR_CSI_LANES         4

uint8_t max9286_slave_addr[RCAR_V3_SUPPORT_NUM][RCAR_CSI2_NUM] = 
{
    { 0x90, 0x94 }, //condor board
    { 0xD8, 0x00 }, //starterkit board
    { 0x90, 0x00 }, //eagle board
};

uint8_t pcae9654_slave_addr[RCAR_V3_SUPPORT_NUM][RCAR_CSI2_NUM] = 
{
    { 0x40, 0x42 }, //condor board
    { 0x00, 0x00 }, //starterkit board
    { 0x40, 0x00 }, //eagle board
};

uint8_t max9271_addr_map[RCAR_CSI2_NUM][RCAR_CSI_LANES] = 
{
    {0xA0, 0xA2, 0xA4, 0xA6}, //CSI2-0
    {0xA8, 0xAA, 0xAC, 0xAE}, //CSI2-1
};

typedef struct reg_info {
    uint16_t    reg_addr;
    uint8_t     value;
}reg_info_t;

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

struct _csi2_link_
{
    uint32_t links;
    uint32_t order;
} csi2_link;

const char*	max9286_i2c_dev[RCAR_V3_SUPPORT_NUM] = {"/dev/i2c1", "/dev/i2c3", "/dev/i2c3"};
const char*	pcae9654_i2c_dev = "/dev/i2c0";	

int i2c_fd = -1;
int i2c_speed = 50000;

static int max9286_i2c_read(int fd, uint8_t addr, uint8_t sub_addr, uint8_t *value)
{
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
    
    if (fd < 0) {
        fprintf(stderr, "%s: i2c dev is not opened.\n", __func__);
        return -1;
    }

    slave.addr = addr >> 1;
    slave.fmt = I2C_ADDRFMT_7BIT;

    status = devctl(fd, DCMD_I2C_SET_SLAVE_ADDR, &slave, sizeof(slave), NULL);

    if (status != EOK) {
        return -1;
    }

    omsg.hdr.slave = slave;
    omsg.hdr.len = 1;
    omsg.hdr.stop = 0; 
    omsg.bytes[0] = sub_addr;

    status = devctl(fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if (status != EOK) {
        return -1;
    }
    
    imsg.hdr.slave = slave;
    imsg.hdr.len = 1;
    imsg.hdr.stop = 1;

    status = devctl(fd, DCMD_I2C_RECV, &imsg, sizeof(imsg.hdr) + imsg.hdr.len, NULL);

    if (status != EOK) {
        return -1;
    }
    
    *value = imsg.bytes[0];
      
    return 0;
}

static int max9286_i2c_write(int fd, uint8_t addr, uint8_t sub_addr, uint8_t value, int mdelay)
{
    int	status = EOK;
    i2c_addr_t slave;

    struct {
        i2c_send_t hdr;		
        unsigned char bytes[8];	
    } omsg;
    
    if (fd < 0) {
        fprintf(stderr, "%s: i2c dev is not opened.\n", __func__);
        return -1;
    }
              
    slave.addr = addr >> 1;
    slave.fmt = I2C_ADDRFMT_7BIT;

    status = devctl(fd, DCMD_I2C_SET_SLAVE_ADDR, &slave, sizeof(slave), NULL);

    if (status != EOK) {
        return -1;
    }

    omsg.hdr.slave = slave;
    omsg.hdr.slave.fmt = I2C_ADDRFMT_7BIT;
    omsg.hdr.len = 2;
    omsg.hdr.stop = 1;
    omsg.bytes[0] = sub_addr;
    omsg.bytes[1] = value;

    status = devctl(fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if(status != EOK) {
        return -1;
    }
    
    if (mdelay) delay(mdelay);
        
    return 0;
}

static int max9286_16bit_i2c_write(int fd, uint8_t addr, uint16_t sub_addr, uint8_t value, int mdelay)
{
    int	status = EOK;
    i2c_addr_t slave;

    struct {
        i2c_send_t      hdr;
        uint16_t        reg;
        uint8_t         val;
    } omsg;

    if (fd < 0) {
        fprintf(stderr, "%s: i2c dev is not opened.\n", __func__);
        return -1;
    }
    
    slave.addr = addr >> 1;
    slave.fmt = I2C_ADDRFMT_7BIT;

    status = devctl(fd, DCMD_I2C_SET_SLAVE_ADDR, &slave, sizeof(slave), NULL);

    if (status != EOK) {
        return -1;
    }

    omsg.hdr.slave = slave;
    omsg.hdr.slave.fmt = I2C_ADDRFMT_7BIT;
    omsg.hdr.len = 3;
    omsg.hdr.stop = 1;
    omsg.reg = ((sub_addr & 0xff) << 8) | ((sub_addr & 0xff00) >> 8);
    omsg.val = value;

    status = devctl(fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if(status != EOK) {
        return -1;
    }
    
    if(mdelay) delay(mdelay);
	
    return 0;
}

static int max9286_16bit_i2c_read(int fd, uint8_t addr, uint16_t sub_addr, uint8_t *value)
{
    int status = EOK;
    i2c_addr_t slave;

    struct  {
        i2c_send_t hdr;		
        uint16_t reg;
    } omsg;

    struct {
        i2c_recv_t hdr;	
        uint8_t val;
    } imsg;

    if (fd < 0) {
        fprintf(stderr, "%s: i2c dev is not opened.\n", __func__);
        return -1;
    }
    
    slave.addr = addr >> 1;
    slave.fmt = I2C_ADDRFMT_7BIT;

    status = devctl(fd, DCMD_I2C_SET_SLAVE_ADDR, &slave, sizeof(slave), NULL);

    if (status != EOK) {
        return -1;
    }

    omsg.hdr.slave = slave;
    omsg.hdr.len = 2;
    omsg.hdr.stop = 1; 
    omsg.reg = ((sub_addr & 0xff) << 8) | ((sub_addr & 0xff00) >> 8);

    status = devctl(fd, DCMD_I2C_SEND, &omsg, sizeof(omsg.hdr) + omsg.hdr.len, NULL);

    if (status != EOK) {
        return -1;
	}

    imsg.hdr.slave = slave;
    imsg.hdr.len = 1;
    imsg.hdr.stop = 1;

    status = devctl(fd, DCMD_I2C_RECV, &imsg, sizeof(imsg.hdr) + imsg.hdr.len, NULL);

    if (status != EOK) {
        return -1;
	}

    *value = imsg.val;	

    return 0;
}


int ov490_init(uint8_t max9286_addr, uint8_t max9271_addr, int port)
{
    uint8_t trans_addr;
    int width, height;
    uint8_t otp[6];
    uint16_t ver;
    uint8_t val;
    int i, len;

    trans_addr = max9271_addr - 0x2E;
    MAX9286_I2C_WRITE(max9286_addr, 0x0a, 0x11 << port, 2);
    /* setup I2C translator address */
    MAX9286_I2C_WRITE(max9271_addr, 0x09, trans_addr, 0);	        /* Sensor translated I2C address */		
	MAX9286_I2C_WRITE(max9271_addr, 0x0A, OV490_I2C_ADDR, 2);	    /* Sensor native I2C address */
    MAX9286_I2C_WRITE(max9286_addr, 0x0a, 0x11 << port, 2);
    
    /* Program table of registers */
    len = sizeof(ov490_conf_regs) / sizeof(ov490_conf_regs[0]);
    for (i = 0; i < len; i ++) {
        MAX9286_16BIT_I2C_WRITE(OV490_I2C_ADDR, ov490_conf_regs[i].reg_addr, ov490_conf_regs[i].value, 0);
    }

    /* Set DVP order */
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_LOW_OFFSET, 0x28, 1);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_ISP_DVP_CTRL3, OV490_DVP_ORDER << 4, 0);
            
    /* Read product ID and manufacturer ID */
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_LOW_OFFSET, 0x80, 1);     
    if (max9286_16bit_i2c_read(i2c_fd, trans_addr, OV490_PID, &val) < 0)
        return -1;
        
    ver = val << 8;
    
    if (max9286_16bit_i2c_read(i2c_fd, trans_addr, OV490_VER, &val) < 0)
        return -1;
        
    ver |= val;
    
    if (ver != OV490_VERSION) {
        fprintf(stderr, "Invalid product ID %x\n", ver);
		MAX9286_I2C_WRITE(max9286_addr, 0x0a, 0, 2);
        return -1;
    }

    /* Read firmware resolution */
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_HIGH_OFFSET, 0x80, 0);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_LOW_OFFSET, 0x82, 1);
        
    if (max9286_16bit_i2c_read(i2c_fd, trans_addr, OV490_ISP_IMAGE0_OHSIZE1, &val) < 0) 
        return-1;
        
    width = val << 8;
    
    if (max9286_16bit_i2c_read(i2c_fd, trans_addr, OV490_ISP_IMAGE0_OHSIZE0, &val) < 0) 
        return -1;
        
    width |= val;
    
    if (max9286_16bit_i2c_read(i2c_fd, trans_addr, OV490_ISP_IMAGE0_OVSIZE1, &val) < 0)
        return -1;
        
    height = val << 8;
    
    if (max9286_16bit_i2c_read(i2c_fd, trans_addr, OV490_ISP_IMAGE0_OVSIZE0, &val) < 0)
        return -1;
        
    height |= val;

    /* Read OPT address */
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_HIGH_OFFSET, 0x80, 1);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_LOW_OFFSET, 0x19, 1);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV10640_WRITE_OPERATION, 0x00, 0);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV10640_HIGH_OFFSET, 0x34, 0);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV10640_LOW_OFFSET, 0x9C, 0);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV10640_REGISTER_VALUE, 1, 0);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_LOW_OFFSET, 0x80, 1);
    MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_HOST_COMMAND, 0xc1, 25);
    for (int i = 0; i < 6; i++) {
        MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_LOW_OFFSET, 0x19, 1);
        MAX9286_16BIT_I2C_WRITE(trans_addr, OV10640_WRITE_OPERATION, 0x01, 0);
        MAX9286_16BIT_I2C_WRITE(trans_addr, OV10640_HIGH_OFFSET, 0x34, 0);
        MAX9286_16BIT_I2C_WRITE(trans_addr, OV10640_LOW_OFFSET, 0x9e + i + 6, 0);
        MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_LOW_OFFSET, 0x80, 1);
        MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_HOST_COMMAND, 0xc1, 0);
        MAX9286_16BIT_I2C_WRITE(trans_addr, OV490_LOW_OFFSET, 0x19, 1);
        
        if (max9286_16bit_i2c_read(i2c_fd, trans_addr, OV10640_WRITE_OPERATION, &otp[i]))
            return -1;
    }

    fprintf(stdout, "Camera %d ov490/ov10640, resolution %dx%d, OTP: %02x-%02x-%02x-%02x-%02x-%02x\n", 
                port, width, height, otp[0], otp[1], otp[2], otp[3], otp[4], otp[5]);      
            
	MAX9286_I2C_WRITE(max9286_addr, 0x0a, 0, 2);
	
    return 0;
}

static int max9286_channel_establish(uint8_t max9286_addr, uint8_t max9271_addr, int ch)
{
    uint8_t max9271_id = 0;
    int timeout = 100;
    
    /* Enable channel for setting */ 
    MAX9286_I2C_WRITE(max9286_addr, 0x3f, 0x4f, 0);    
    MAX9286_I2C_WRITE(max9286_addr, 0x34, 0xb6, 2); //I2C speed
    
    /* Enable GMSL link for available camera */	
    MAX9286_I2C_WRITE(max9286_addr, 0x00, 0xe0 | BIT(ch), 0);
    MAX9286_I2C_WRITE(max9286_addr, 0x0a, 0x11 << ch, 2);
    
    while (1) {          
        MAX9286_I2C_WRITE(max9286_addr, 0x3b, 0x1e, 2);

        /* If i2c sets MAX9271_SLAVE_ADDR failed, MAX9271 i2c address was changed */
        max9286_i2c_write(i2c_fd, MAX9271_SLAVE_ADDR, 0x04, 0x43, 2);
        max9286_i2c_write(i2c_fd, MAX9271_SLAVE_ADDR, 0x08, 0x1, 2);
        MAX9286_I2C_WRITE(max9286_addr, 0x3b, 0x19, 2);
        
        /* Read max9271 ID */	
        max9286_i2c_read(i2c_fd, MAX9271_SLAVE_ADDR, 0x1e, &max9271_id);
        
        if (max9271_id == MAX9271_ID) {
            fprintf(stdout, "Link %d MAX9271 detected at address 0x%x\n", ch, max9271_addr >> 1);            
            
            /* gmsl link setup */
            MAX9286_I2C_WRITE(MAX9271_SLAVE_ADDR, 0x0d, 0x36, 0); //I2C speed
            MAX9286_I2C_WRITE(MAX9271_SLAVE_ADDR, 0x07, 0x94, 2);
            MAX9286_I2C_WRITE(MAX9271_SLAVE_ADDR, 0x02, 0xff, 2);
            MAX9286_I2C_WRITE(max9286_addr, 0x34, 0x36, 2); //I2C speed

            /* I2C translator setup */        
            MAX9286_I2C_WRITE(MAX9271_SLAVE_ADDR, 0x0B, MAXIM_BROADCAST_ADDR << 1, 0);
            MAX9286_I2C_WRITE(MAX9271_SLAVE_ADDR, 0x0C, max9271_addr, 0);

            /* Change MAX9271 I2C address */
            MAX9286_I2C_WRITE(MAX9271_SLAVE_ADDR, 0x01, max9286_addr, 2);        
            MAX9286_I2C_WRITE(MAX9271_SLAVE_ADDR, 0x00, max9271_addr, 2);
            MAX9286_I2C_WRITE(max9271_addr, 0x04, 0x43, 2);

            /* Reset GPIO1  */
            MAX9286_I2C_WRITE(max9271_addr, 0x0f, 0xfe &~(1 << 1), 0); //Reset
            MAX9286_I2C_WRITE(max9271_addr, 0x0e, 0x42 | (1 << 1), 2);  //Output high level
            MAX9286_I2C_WRITE(max9271_addr, 0x0f, 0xfe | (1 << 1), 2);  //Remove-reset
            
            return 0;
        }
        
        /* Check if already setup (after reset) */
        max9286_i2c_read(i2c_fd, max9271_addr, 0x1e, &max9271_id);
		
        if (max9271_id == MAX9271_ID) {
            MAX9286_I2C_WRITE(max9271_addr, 0x04, 0x43, 2);
            fprintf(stdout, "Link %d MAX9271 already at address 0x%x\n", ch, max9271_addr >> 1);
            return 0;
        }
        
        if (--timeout == 0) {
            fprintf(stdout, "Link %d MAX9271 at address 0x%x cannot be established.\n", ch, max9271_addr >> 1);
            return -1;
        } 
    }
    
    return 0;
}

static int max9286_setup(uint8_t max9286_addr, int csi2_ch)
{
    /* Disable all cameras */
    MAX9286_I2C_WRITE(max9286_addr, 0x0a, 0x00, 0);
    /* Disable all GMSL links */	
    MAX9286_I2C_WRITE(max9286_addr, 0x00, 0x00, 2);
    /* Disable CSI output */
    MAX9286_I2C_WRITE(max9286_addr, 0x15, 0x13, 0); 
    MAX9286_I2C_WRITE(max9286_addr, 0x69, 0x0f, 0);
    
    /* Setup CSI lanes number */
    if (RCAR_CSI_LANES == 1){
        MAX9286_I2C_WRITE(max9286_addr, 0x12, 0x33, 0);
    } else if (RCAR_CSI_LANES == 2){
        MAX9286_I2C_WRITE(max9286_addr, 0x12, 0x73, 0);
    } else if (RCAR_CSI_LANES == 3){
        MAX9286_I2C_WRITE(max9286_addr, 0x12, 0xd3, 0);
    } else if (RCAR_CSI_LANES == 4){
        MAX9286_I2C_WRITE(max9286_addr, 0x12, 0xf3, 0);
    } else {
        fprintf(stderr, "%d lanes is not supported \n", RCAR_CSI_LANES);
        return -1;
    }
    
    MAX9286_I2C_WRITE(max9286_addr, 0x01, 0xc0, 0);
    MAX9286_I2C_WRITE(max9286_addr, 0x06, 0x00, 0); //set to 0x30d400
    MAX9286_I2C_WRITE(max9286_addr, 0x07, 0xd4, 0);
    MAX9286_I2C_WRITE(max9286_addr, 0x08, 0x30, 0);
    MAX9286_I2C_WRITE(max9286_addr, 0x63, 0x00, 0);
    MAX9286_I2C_WRITE(max9286_addr, 0x64, 0x00, 0);
    MAX9286_I2C_WRITE(max9286_addr, 0x0c, 0x89, 0);
    MAX9286_I2C_WRITE(max9286_addr, 0x19, 0x0c, 0);

    /* Setup channel connect  */
    for (int ch = 0; ch < RCAR_CSI_LANES; ch++) {
        if (max9286_channel_establish(max9286_addr, max9271_addr_map[csi2_ch][ch], ch) == 0) {
            int r;
            csi2_link.links |= BIT(ch);
            r = csi2_link.links - ((csi2_link.links >> 1) & 0x55);
            r = (r & 0x33) + ((r >> 2) & 0x33);
            r = (r + (r >> 4)) & 0x0F;            
            csi2_link.order &= ~(0x3 << (ch * 2));
            csi2_link.order |= (r - 1) << (ch * 2);
        }
    }

    MAX9286_I2C_WRITE(max9286_addr, 0x0a, 0x00, 0);
    /* Enable GMSL link for available cameras */	
    MAX9286_I2C_WRITE(max9286_addr, 0x00, 0xe0 | csi2_link.links, 0);    
    /* CSI2 output order */
    MAX9286_I2C_WRITE(max9286_addr, 0x0b, csi2_link.order, 0);
    /* Enable CSI output */
    MAX9286_I2C_WRITE(max9286_addr, 0x15, 0x9b, 0);
    /* Enable equalizer for available cameras */
    MAX9286_I2C_WRITE(max9286_addr, 0x1b, csi2_link.links, 2);
    
    /* Set frame sync mode to auto mode */
    MAX9286_I2C_WRITE(max9286_addr, 0x01, 0x02, 0);
    
    /* Wait if we did hard reset */
    delay(300);

    return 0;
}

int max9286_power(video_info_t *video, int channel, int enable)
{
    if (enable == 1) {
        /* Do not mask */
        MAX9286_I2C_WRITE(max9286_slave_addr[video->board][video->csi2_ch], 0x69, csi2_link.links ^ 0x0f, 0);
    } else if (enable == 0) {
        /* Mask  */
        MAX9286_I2C_WRITE(max9286_slave_addr[video->board][video->csi2_ch], 0x69, 0x0f, 0);
    }	
    
    return 0;
}

static int max9286_gmsl_switch(uint8_t max9286_addr, int csi2_ch)
{	
    /* switch to GMSL serial link for video data transfer */
    for(int i = 0; i < RCAR_CSI_LANES; i++){
        max9286_i2c_write(i2c_fd, max9286_addr, 0x0a, 0x11 << i, 0);       
        max9286_i2c_write(i2c_fd, max9271_addr_map[csi2_ch][i], 0x04, 0x83, 2);                                		
        max9286_i2c_write(i2c_fd, max9286_addr, 0x0a, (csi2_link.links << 4) | csi2_link.links, 0); 
    }
	
	return 0;
}

int max9286_init(video_info_t *video, int max_channel)
{
    int i;
    uint8_t max9286_addr;
    int r;
    if ((max9286_addr = max9286_slave_addr[video->board][video->csi2_ch]) == 0) {
        fprintf(stderr, "%s: No support MAX9286 slave address 0x00\n", __func__);
        return -1;
    }
  
    memset(&csi2_link, 0, sizeof(struct _csi2_link_));
    csi2_link.order = 0xFF;
    
    if(i2c_fd == -1) {
        if ((i2c_fd = open(max9286_i2c_dev[video->board], O_RDWR)) < 0){
            fprintf(stderr, "%s: Open %s failed\n", __func__, max9286_i2c_dev[video->board]);
            return -1;
        }
    }

    if (devctl(i2c_fd, DCMD_I2C_SET_BUS_SPEED, &i2c_speed, sizeof(i2c_speed), NULL) != EOK) {
        fprintf(stderr, "%s: Set bus speed failed\n", __func__);
        return -1;
    }
    
    if(video->board == RCAR_V3H_CONDOR || video->board == RCAR_V3M_EAGLE) {
        uint8_t pcae9654_addr = pcae9654_slave_addr[video->board][video->csi2_ch];
        int pcae_i2c_fd;
        if ((pcae_i2c_fd = open(pcae9654_i2c_dev, O_RDWR)) < 0) {
            fprintf(stderr, "%s: Open %s failed\n", __func__, pcae9654_i2c_dev);
            return -1;
        }
        /* IO0->IO7 output mode */
        if(pcae9654_addr)
            max9286_i2c_write(pcae_i2c_fd, pcae9654_addr, 0x03, 0xfe, 200); 
        /* Camera power on */
        MAX9286_I2C_WRITE(max9286_addr, 0x0f, 0x0a, 300);   
        close(pcae_i2c_fd);
    }
    if (max9286_setup(max9286_addr, video->csi2_ch) < 0) {
        fprintf(stderr, "%s: Setup MAX9286 failed\n", __func__);
        return -1;
    }

    /* Camera init */
    for(i = 0; i < RCAR_CSI_LANES; i++) {
        ov490_init(max9286_addr, max9271_addr_map[video->csi2_ch][i], i);
    }

    /* MAX9286 GMSL serial link switch */
    max9286_gmsl_switch(max9286_addr, video->csi2_ch);

    /* CSI2_speed = (physical_clock * sizeof(YUV8) * links) / lanes */
	r = csi2_link.links - ((csi2_link.links >> 1) & 0x55);
	r = (r & 0x33) + ((r >> 2) & 0x33);
	r = (r + (r >> 4)) & 0x0F;
    video->csi_speed = RCAR_PHYSICAL_CLOCK * RCAR_YUV8_SIZE * r / RCAR_CSI_LANES;
    
    /* This is set accordingly to OV490/OV10640 hardware manual */
    video->width = 1280;
    video->height = 1080;
    video->interlace = 0;	/* Progressive */
    
    return 0;
}

int	max9286_fini(video_info_t* video, int max_channel)
{
    if(i2c_fd > 0) {
        max9286_power(video, video->csi2_ch, 0);

        close(i2c_fd);
    }
    
    i2c_fd = -1;

    return 0;
}

int	max9286_update(video_info_t* video, int max_channel)
{
    return 0;
}
