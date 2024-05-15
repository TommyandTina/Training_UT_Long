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
 

#ifndef __MAX96712_H__
#define __MAX96712_H__

#include "common.h"
#include "decoder.h"

#define ENABLE					1
#define DISABLE					0

#define MAX9271_SLAVE_ADDR      0x80
#define MAX9271_ID				0x09
#define BROADCAST				0x6f

// CSI2-GMSL Deserializer
#define MAX96712_0_SLA_ADDR		0x92 // Falcon CSI U7  MAX96712GTB/VY+  0x92 0x93
#define MAX96712_1_SLA_ADDR		0x96 // Falcon CSI U10 MAX96712GTB/VY+  0x96 0x97
#define MAX96712_2_SLA_ADDR		0xD6 // Falcon CSI U13 MAX96712GTB/VY+  0xD6 0xD7

// GMSL Camera POC controller
#define MAX20087_0_SLA_ADDR		0x50 // Falcon CSI U6  MAX20087ATPA     0x50 0x51
#define MAX20087_1_SLA_ADDR		0x54 // Falcon CSI U9  MAX20087ATPB     0x54 0x55
#define MAX20087_2_SLA_ADDR		0x5A // Falcon CSI U9  MAX20087ATPC     0x5A 0x5B

// PCA9654E
#define PCAE9654_0_SLA			0x42 // Falcon U8 PCA9654EDTR2G   0x42 0x43
#define PCAE9654_1_SLA			0x44 // Falcon U11 PCA9654EDTR2G  0x44 0x45
#define PCAE9654_2_SLA			0x46 // Falcon U14 PCA9654EDTR2G  0x46 0x47

// PCA9654E register
#define PCAE9654_IN_PORT		0x00 // Register0: Input Port Register
#define PCAE9654_OUT_PORT		0x01 // Register1: Output Port Register
#define PCAE9654_POL_INV		0x02 // Register2: Polarity Inversion Register
#define PCAE9654_CONFIG			0x03 // Register3: Configuration Register

// MAX96712 register
#define MAX96712_MAX_LINKS				4
#define MAX96712_MAX_PIPES				8
#define MAX96712_MAX_PIPE_MAPS			16
#define MAX96712_MAX_MIPI				4
#define MAX96712_CSI_OUT1				1
#define MAX96712_CSI_OUT2				2
#define MAX96712_FSYNC_PERIOD			3210000 /* 96MHz/30fps */

#define MAX96712_ID						0x20

#define MAX96712_REG4					0x04
#define MAX96712_REG5					0x05
#define MAX96712_REG6					0x06
#define MAX96712_REG14					0x0e
#define MAX96712_REG26					0x10
#define MAX96712_REG27					0x11
#define MAX96712_CTRL0					0x17
#define MAX96712_CTRL1					0x18
#define MAX96712_CTRL2					0x19
#define MAX96712_CTRL3					0x1a
#define MAX96712_CTRL11					0x22
#define MAX96712_CTRL12					0x0a
#define MAX96712_CTRL13					0x0b
#define MAX96712_CTRL14					0x0c
#define MAX96712_PWR1					0x13
#define MAX96712_DEV_ID					0x4a
#define MAX96712_REV					0x4c
#define MAX96712_VIDEO_PIPE_SEL(n) 		(0xf0 + n)
#define MAX96712_VIDEO_PIPE_EN			0xf4
#define MAX96712_I2C_0(n)				(0x640 + (0x10 * n))
#define MAX96712_I2C_1(n)				(0x641 + (0x10 * n))
#define MAX96712_RX0(n)					(0x50 + n)
#define MAX_VIDEO_RX_BASE(n)			(n < 5 ? (0x100 + (0x12 * n)) : \
										(0x160 + (0x12 * (n - 5))))
#define MAX_VIDEO_RX0(n)				(MAX_VIDEO_RX_BASE(n) + 0x00)
#define MAX_VIDEO_RX3(n)				(MAX_VIDEO_RX_BASE(n) + 0x03)
#define MAX_VIDEO_RX8(n)				(MAX_VIDEO_RX_BASE(n) + 0x08)
#define MAX_VIDEO_RX10(n)				(MAX_VIDEO_RX_BASE(n) + 0x0a)
#define MAX_VPRBS(n)					(0x1dc + (0x20 * n))
#define MAX_CROSS_BASE(n)				(0x1c0 + (0x20 * n))
#define MAX_CROSS(n, m)					(MAX_CROSS_BASE(n) + m)
#define MAX_BACKTOP_BASE(bank)			(0x400 + (0x20 * bank))
#define MAX_BACKTOP1(bank)				(MAX_BACKTOP_BASE(bank) + 0x00)
#define MAX_BACKTOP11(bank)				(MAX_BACKTOP_BASE(bank) + 0x0a)
#define MAX_BACKTOP12(bank)				(MAX_BACKTOP_BASE(bank) + 0x0b)
#define MAX_BACKTOP13(bank)				(MAX_BACKTOP_BASE(bank) + 0x0c)
#define MAX_BACKTOP14(bank)				(MAX_BACKTOP_BASE(bank) + 0x0d)
#define MAX_BACKTOP15(bank)				(MAX_BACKTOP_BASE(bank) + 0x0e)
#define MAX_BACKTOP16(bank)				(MAX_BACKTOP_BASE(bank) + 0x0f)
#define MAX_BACKTOP17(bank)				(MAX_BACKTOP_BASE(bank) + 0x10)
#define MAX_BACKTOP18(bank)				(MAX_BACKTOP_BASE(bank) + 0x11)
#define MAX_BACKTOP19(bank)				(MAX_BACKTOP_BASE(bank) + 0x12)
#define MAX_BACKTOP20(bank)				(MAX_BACKTOP_BASE(bank) + 0x13)
#define MAX_BACKTOP21(bank)				(MAX_BACKTOP_BASE(bank) + 0x14)
#define MAX_BACKTOP22(bank)				(MAX_BACKTOP_BASE(bank) + 0x15)
#define MAX_BACKTOP23(bank)				(MAX_BACKTOP_BASE(bank) + 0x16)
#define MAX_BACKTOP24(bank)				(MAX_BACKTOP_BASE(bank) + 0x17)
#define MAX_BACKTOP25(bank)				(MAX_BACKTOP_BASE(bank) + 0x18)
#define MAX_BACKTOP26(bank)				(MAX_BACKTOP_BASE(bank) + 0x19)
#define MAX_BACKTOP27(bank)				(MAX_BACKTOP_BASE(bank) + 0x1a)
#define MAX_BACKTOP28(bank)				(MAX_BACKTOP_BASE(bank) + 0x1b)
#define MAX_BACKTOP29(bank)				(MAX_BACKTOP_BASE(bank) + 0x1c)
#define MAX_BACKTOP30(bank)				(MAX_BACKTOP_BASE(bank) + 0x1d)
#define MAX_BACKTOP31(bank)				(MAX_BACKTOP_BASE(bank) + 0x1e)
#define MAX_BACKTOP32(bank)				(MAX_BACKTOP_BASE(bank) + 0x1f)
#define MAX96712_FSYNC_0				0x4a0
#define MAX96712_FSYNC_5				0x4a5
#define MAX96712_FSYNC_6				0x4a6
#define MAX96712_FSYNC_7				0x4a7
#define MAX96712_FSYNC_8				0x4a8
#define MAX96712_FSYNC_9				0x4a9
#define MAX96712_FSYNC_10				0x4aa
#define MAX96712_FSYNC_11				0x4ab
#define MAX96712_FSYNC_15				0x4af
#define MAX_MIPI_PHY_BASE				0x8a0
#define MAX_MIPI_PHY0					(MAX_MIPI_PHY_BASE + 0x00)
#define MAX_MIPI_PHY2					(MAX_MIPI_PHY_BASE + 0x02)
#define MAX_MIPI_PHY3					(MAX_MIPI_PHY_BASE + 0x03)
#define MAX_MIPI_PHY4					(MAX_MIPI_PHY_BASE + 0x04)
#define MAX_MIPI_PHY5					(MAX_MIPI_PHY_BASE + 0x05)
#define MAX_MIPI_PHY6					(MAX_MIPI_PHY_BASE + 0x06)
#define MAX_MIPI_PHY8					(MAX_MIPI_PHY_BASE + 0x08)
#define MAX_MIPI_PHY9					(MAX_MIPI_PHY_BASE + 0x09)
#define MAX_MIPI_PHY10					(MAX_MIPI_PHY_BASE + 0x0a)
#define MAX_MIPI_PHY11					(MAX_MIPI_PHY_BASE + 0x0b)
#define MAX_MIPI_PHY13					(MAX_MIPI_PHY_BASE + 0x0d)
#define MAX_MIPI_PHY14					(MAX_MIPI_PHY_BASE + 0x0e)
#define MAX_MIPI_TX_BASE(n)				(0x900 + 0x40 * n)
#define MAX_MIPI_TX2(n)					(MAX_MIPI_TX_BASE(n) + 0x02)
#define MAX_MIPI_TX10(n)				(MAX_MIPI_TX_BASE(n) + 0x0a)
#define MAX_MIPI_TX11(n)				(MAX_MIPI_TX_BASE(n) + 0x0b)
#define MAX_MIPI_TX12(n)				(MAX_MIPI_TX_BASE(n) + 0x0c)
/* 16 pairs of source-dest registers */
#define MAX_MIPI_MAP_SRC(pipe, n)		(MAX_MIPI_TX_BASE(pipe) + 0x0d + (2 * n))
#define MAX_MIPI_MAP_DST(pipe, n)		(MAX_MIPI_TX_BASE(pipe) + 0x0e + (2 * n))
/* Phy dst. Each reg contains 4 dest */
#define MAX_MIPI_MAP_DST_PHY(pipe, n)	(MAX_MIPI_TX_BASE(pipe) + 0x2d + n)
#define MAX_GMSL1_2(ch)					(0xb02 + (0x100 * ch))
#define MAX_GMSL1_4(ch)					(0xb04 + (0x100 * ch))
#define MAX_GMSL1_6(ch)					(0xb06 + (0x100 * ch))
#define MAX_GMSL1_7(ch)					(0xb07 + (0x100 * ch))
#define MAX_GMSL1_8(ch)					(0xb08 + (0x100 * ch))
#define MAX_GMSL1_D(ch)					(0xb0d + (0x100 * ch))
#define MAX_GMSL1_F(ch)					(0xb0f + (0x100 * ch))
#define MAX_GMSL1_19(ch)				(0xb19 + (0x100 * ch))
#define MAX_GMSL1_1B(ch)				(0xb1b + (0x100 * ch))
#define MAX_GMSL1_1D(ch)				(0xb1d + (0x100 * ch))
#define MAX_GMSL1_20(ch)				(0xb20 + (0x100 * ch))
#define MAX_GMSL1_96(ch)				(0xb96 + (0x100 * ch))
#define MAX_GMSL1_CA(ch)				(0xbca + (0x100 * ch))
#define MAX_GMSL1_CB(ch)				(0xbcb + (0x100 * ch))
#define MAX_RLMS4(ch)					(0x1404 + (0x100 * ch))
#define MAX_RLMSA(ch)					(0x140A + (0x100 * ch))
#define MAX_RLMSB(ch)					(0x140B + (0x100 * ch))
#define MAX_RLMSA4(ch)					(0x14a4 + (0x100 * ch))
#define MAX_RLMS58(ch)					(0x1458 + (0x100 * ch))
#define MAX_RLMS59(ch)					(0x1459 + (0x100 * ch))
#define MAX_RLMS95(ch)					(0x1495 + (0x100 * ch))
#define MAX_RLMSC4(ch)					(0x14c4 + (0x100 * ch))
#define MAX_RLMSC5(ch)					(0x14c5 + (0x100 * ch))
#define MAXIM_I2C_SPEED_837KHZ			(0x7 << 2) /* 837kbps */
#define MAXIM_I2C_SPEED_533KHZ			(0x6 << 2) /* 533kbps */
#define MAXIM_I2C_SPEED_339KHZ			(0x5 << 2) /* 339 kbps */
#define MAXIM_I2C_SPEED_173KHZ			(0x4 << 2) /* 174kbps */
#define MAXIM_I2C_SPEED_105KHZ			(0x3 << 2) /* 105 kbps */
#define MAXIM_I2C_SPEED_085KHZ			(0x2 << 2) /* 84.7 kbps */
#define MAXIM_I2C_SPEED_028KHZ			(0x1 << 2) /* 28.3 kbps */
#define MAXIM_I2C_SPEED					MAXIM_I2C_SPEED_339KHZ
#define MIPI_DT_GENERIC					0x10
#define MIPI_DT_GENERIC_1				0x11
#define MIPI_DT_EMB						0x12
#define MIPI_DT_YUV8					0x1e
#define MIPI_DT_YUV10					0x1f
#define MIPI_DT_RGB565					0x22
#define MIPI_DT_RGB666					0x23
#define MIPI_DT_RGB888					0x24
#define MIPI_DT_RAW8					0x2a
#define MIPI_DT_RAW10					0x2b
#define MIPI_DT_RAW12					0x2c
#define MIPI_DT_RAW14					0x2d
#define MIPI_DT_RAW16					0x2e
#define MIPI_DT_RAW20					0x2f
#define MIPI_DT_YUV12					0x30

/* OV490 */
#define OV490_I2C_ADDR          		0x48
#define OV490_HIGH_OFFSET          		0xFFFD
#define OV490_LOW_OFFSET          		0xFFFE
#define OV490_PID               		0x300a
#define OV490_VER               		0x300b
#define OV490_VERSION       		    0x0490
#define OV490_ISP_IMAGE0_OHSIZE0  		0x60
#define OV490_ISP_IMAGE0_OHSIZE1   		0x61
#define OV490_ISP_IMAGE0_OVSIZE0   		0x62
#define OV490_ISP_IMAGE0_OVSIZE1   		0x63
#define OV490_ISP_DVP_CTRL3				0x6009
#define OV490_DVP_ORDER         		1
#define OV490_ISP_IMAGE0_FORMAT1		0x0071
#define OV490_ISP_IMAGE1_FORMAT1		0x0075
#define OV490_IMAGE_CONTROl				0x6010
#define OV490_EMBEDLINE_CTRL			0xe000
#define OV490_DVP_CTRL0					0x6000
#define OV490_DVP_CTRL1					0x6004
#define OV490_DVP_CTRL2					0x6008
#define OV490_IMAGE_PATH_CONTROL1		0x0091
#define OV490_MIPI_CONTROL_F			0x00bb
#define OV490_HOST_COMMAND				0x00c0
/* OV10640 */
#define OV10640_WRITE_OPERATION			0x5000
#define OV10640_HIGH_OFFSET				0x5001
#define OV10640_LOW_OFFSET				0x5002
#define OV10640_REGISTER_VALUE			0x5003
#define OV10640_SENSOR_CTRL				0x308C


uint32_t max96712_color_pattern_generator[] = {	
	// 0x1050, 0xF3,	// Set VTG mode VRX_Patgen 0, Generate VS, HS, DE", Invert the VS
	0x1050, 0xE3,		// Set VTG mode VRX_Patgen 0, Generate VS, HS, DE", Not Invert the VS
	0x1051, 0x20,		// Set Patgen mode = 2 (Color Gradient), Grad Mode = 0"

	0x0010, 0x00,		// Set VS_DLY = 0
	0x1053, 0x00,
	0x1054, 0x00,
	0x1055, 0x25,		// Set VS High 
	0x1056, 0x99,
	0x1057, 0x00,
	0x1058, 0x00,		// Set VS Low 
	0x1059, 0x2A,
	0x105A, 0xF8,
	// 0x105B, 0x01,	// Set HS Delay V2H
	// 0x105C, 0x5F,
	// 0x105D, 0x10,
	0x105B, 0x00,		// Set HS Delay V2H
	0x105C, 0x00,
	0x105D, 0x00,
	0x105E, 0x08,		// Set HS_HIGH
	0x105F, 0x6C,
	0x1060, 0x00,		// Set HS_LOW
	0x1061, 0x2C,
	0x1062, 0x04,		// Set HS_CNT
	0x1063, 0x65,
	// 0x1064, 0x01,	// Set DE Delay
	// 0x1065, 0x59,
	// 0x1066, 0x00,
	0x1064, 0x01,		// Set DE Delay
	0x1065, 0x61,
	0x1066, 0x18,
	0x1067, 0x07,		// Set DE_HIGH
	0x1068, 0x80,
	0x1069, 0x01,		// Set DE_LOW
	0x106A, 0x18,
	0x106B, 0x04,		// Set DE_CNT
	0x106C, 0x38,
	0x106D, 0x03,		// Set Grad_INCR_0_0

// CHECKERBOARD SETUP - PATGEN MODE = 1
	0x106E, 0x88,		// Set CHKR_COLOR_A_L_0
	0x106F, 0xAA,		// Set CHKR_COLOR_A_M_0
	0x1070, 0x55,		// Set CHKR_COLOR_A_H_0
	0x1071, 0x00,		// Set CHKR_COLOR_B_L_0
	0x1072, 0x08,		// Set CHKR_COLOR_B_M_0
	0x1073, 0x80,		// Set CHKR_COLOR_B_H_0
	0x1074, 0x50,		// Set CHKR_RPT_A_0
	0x1075, 0xA0,		// Set CHKR_RPT_B_0
	0x1076, 0x50,		// Set CHKR_ALT_0

// Set Patgen Clk frequency 75MHz firstly
	0x0009, 0x01,		//75MHz -> 150MHz
	// 0x0009, 0x02,	//150MHz
	0x01DC, 0x00,
	0x01FC, 0x00,
	0x021C, 0x00,
	0x023C, 0x00,

// RGB888 Software override VC/DT/BPP for Pipe 1,2,3 and 4; 0x24 = 6'b100100, 24 = 5'b11000; 0x2C = 6'b101100, 12 = 5'b01100
	0x040B, 0xC2,		// BPP override for 1
	// 0x040B, 0x62,
	// 0x040C, 0x00,	// VC override for 1/2/3/4
	// 0x040D, 0x00,
	0x040C, 0x10,		// VC override for 1/2/3/4
	0x040D, 0x32,
	0x040E, 0xA4,		// DT override for 1/2/3/4
	0x040F, 0x94,
	0x0410, 0x90,
	// 0x040E, 0xAC,
	// 0x040F, 0xBC,
	// 0x0410, 0xB0,
	0x0411, 0xD8,		// BPP override for 2/3/4
	0x0412, 0x60,
	// 0x0411, 0x6C,
	// 0x0412, 0x30,

	0xFFFFFFFF, 0xFFFFFFFF	// End Table
};


const uint32_t max96712_out_enable[] = {
// ----------Deserializer Setup--------------------------------------------------------------------
	0x0006, 0x00,		// Disable GMSL link
	0x0415, 0xEC,		// Set 1200M DPLL frequency, Enable the software override for 1/2/3/4
	0x0418, 0xEC,		
	// 0x041B, 0xEC,
	// 0x041E, 0xEC,
	// 0x01DD, 0x1B,	// Set Alternate Crossbar for Video Pipes 0-3 - REMOVE ALTERNATE CROSSBAR???
	// 0x01FD, 0x1B,
	// 0x021D, 0x1B,
	// 0x023D, 0x1B,

	// 0x0409, 0x0F,	// Set Line Start, Line End for Video Pipe 0-3 (Backtop //1-4)"
	0x094A, 0xC0,		// Set Lane Count - 4 for script  (CSI_NUM_LANES), DPHY ONLY"
	// 0x08A1, 0x00,	// Set Phy data lane HS Timing
	0x08A3, 0xE4,		// Set Phy lane Map for all MIPI PHYs
	0x08A4, 0xE4,		
	// 0x08A5, 0x00,	// Set Phy lane Map for all MIPI PHYs

	0x090B, 0x07,		// Set MAP_EN_L_0 for map 0
	0x094B, 0x07,		// Set MAP_EN_L_0 for map 0
	0x098B, 0x07,		// Set MAP_EN_L_0 for map 0
	0x09CB, 0x07,		// Set MAP_EN_L_0 for map 0
	0x092D, 0x15,		// Set MAP_DPHY_DEST TO CTRL 1
	0x090D, 0x24,		// Set MAP_SRC AND DEST for data and FE/FS; [7:6]=VC, [5:0]=DT
	0x094D, 0x64,		// Set MAP_SRC AND DEST for data and FE/FS; [7:6]=VC, [5:0]=DT
	0x098D, 0xA4,		// Set MAP_SRC AND DEST for data and FE/FS; [7:6]=VC, [5:0]=DT
	0x09CD, 0xE4,		// Set MAP_SRC AND DEST for data and FE/FS; [7:6]=VC, [5:0]=DT
	0x090E, 0x24,
	// 0x090D, 0x2C,
	// 0x090E, 0x2C,
	0x090F, 0x00,
	0x0910, 0x00,
	0x0911, 0x01,
	0x0912, 0x01,

	0x0009, 0x02,		// Set to 150MHz <= (Changed issue order) =================================

	// 0x08A2, 0x0C,	// Turn off unused PHYs
	0x08A0, 0x84,		// Enable CSI clock w/ MIPI PHY 2x4 configuration
	0x08A2, 0x30,		// Turn off unused PHYs


	// 0x0009, 0x02,	// Set to 150MHz
// End of Setup Script - MUST ONE-SHOT RESET!!!! <=================================================
	0x0018, 0x0F,		// One shot reset for PHY A - HS86

	0xFFFFFFFF, 0xFFFFFFFF	// End Table
};

const uint32_t max96712_out_disable[] = {
	0x08A0, 0x04,			// Disable CSI clock w/ MIPI PHY 2x4 configuration,,
	0x08A2, 0x00,			// Stanby mode PHYs
	0xFFFFFFFF, 0xFFFFFFFF	// End Table
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

typedef struct {
	int			ser_id;
	int			ser_addr;
	int			pipes_mask;	/* Mask of pipes used by this link */
	int			out_mipi;
	int			out_vc;		
} max96712_link_t ;

typedef struct {
	uint32_t 		csiCh;		
	int 	 		i2c_des;	
	uint32_t 		des_sla;	
	int 	 		i2c_pcae;	
	uint32_t 		pcae_sla;	
	int 	 		i2c_poc;	
	uint32_t 		poc_sla;	

	int				n_links;
	int				links_mask;
	max96712_link_t *link[MAX96712_MAX_LINKS];
	int				pclk_rise_edge;
	int				is_coax;
	int				him;
	int				bws;
	int				dbl;
	int				hibw;
	int				hven;
	int				hsync;
	int				vsync;
	int				dt;
	int				gpio[11];
	int				timeout;
	int				csi_rate[MAX96712_MAX_MIPI];
} max96712_info_t;


typedef struct {
	char  		*res;
	char  		*clk;	
	uint16_t 	h_disp;
	uint16_t 	h_start;	
	uint16_t 	h_end;	
	uint16_t 	h_total;
	uint16_t 	v_disp;
	uint16_t 	v_start;	
	uint16_t 	v_end;
	uint16_t 	v_total;
	char  		*hysyn_pol;
	char  		*vsyn_pol;
	uint16_t 	clk_sel;
	uint16_t 	dpll_div;
	uint16_t 	du_div;
} model_line_t;


static inline int mipi_dt_to_bpp(unsigned int dt)
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

static inline char *chip_name(int id)
{
	switch(id) {
	case MAX9271_ID:
		return "MAX9271";
	case MAX96712_ID:
		return "MAX96712";
	default:
		return "Serializer";
	}
}

int max96712_output_enable(max96712_info_t *cam, int on);
int max96712_gmsl1_cc_enable(max96712_info_t *max, int link, int on);

#endif
