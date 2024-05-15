/*
 * $QNXLicenseC: 
 * Copyright 2012, QNX Software Systems.  
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

/*
 * IOMUXC offsets applicable for i.MX6 Solo and i.MX6 DualLite
 * The register offsets below are NOT compatible with the i.MX6 Dual/Quad!
 */

#ifndef	__ARM_MX6SDL_IOMUX_H_INCLUDED
#define	__ARM_MX6SDL_IOMUX_H_INCLUDED

/*
 * offsets of IOMUXC registers from MX6X_IOMUX_SWMUX
 * where MX6X_IOMUX_SWMUX = MX6X_IOMUXC_BASE + 0x004C
 */

#define SWMUX_SDL_CSI0_DAT10		0
#define SWMUX_SDL_CSI0_DAT11		1
#define SWMUX_SDL_CSI0_DAT12		2
#define SWMUX_SDL_CSI0_DAT13		3
#define SWMUX_SDL_CSI0_DAT14		4
#define SWMUX_SDL_CSI0_DAT15		5
#define SWMUX_SDL_CSI0_DAT16		6
#define SWMUX_SDL_CSI0_DAT17		7
#define SWMUX_SDL_CSI0_DAT18		8
#define SWMUX_SDL_CSI0_DAT19		9
#define SWMUX_SDL_CSI0_DAT4		10
#define SWMUX_SDL_CSI0_DAT5		11
#define SWMUX_SDL_CSI0_DAT6		12
#define SWMUX_SDL_CSI0_DAT7		13
#define SWMUX_SDL_CSI0_DAT8		14
#define SWMUX_SDL_CSI0_DAT9		15
#define SWMUX_SDL_CSI0_DATA_EN		16
#define SWMUX_SDL_CSI0_MCLK		17
#define SWMUX_SDL_CSI0_PIXCLK		18
#define SWMUX_SDL_CSI0_VSYNC		19
#define SWMUX_SDL_DI0_DISP_CLK		20
#define SWMUX_SDL_DI0_PIN15		21
#define SWMUX_SDL_DI0_PIN2		22
#define SWMUX_SDL_DI0_PIN3		23
#define SWMUX_SDL_DI0_PIN4		24
#define SWMUX_SDL_DISP0_DAT0		25
#define SWMUX_SDL_DISP0_DAT1		26
#define SWMUX_SDL_DISP0_DAT10		27
#define SWMUX_SDL_DISP0_DAT11		28
#define SWMUX_SDL_DISP0_DAT12		29
#define SWMUX_SDL_DISP0_DAT13		30
#define SWMUX_SDL_DISP0_DAT14		31
#define SWMUX_SDL_DISP0_DAT15		32
#define SWMUX_SDL_DISP0_DAT16		33
#define SWMUX_SDL_DISP0_DAT17		34
#define SWMUX_SDL_DISP0_DAT18		35
#define SWMUX_SDL_DISP0_DAT19		36
#define SWMUX_SDL_DISP0_DAT2		37
#define SWMUX_SDL_DISP0_DAT20		38
#define SWMUX_SDL_DISP0_DAT21		39
#define SWMUX_SDL_DISP0_DAT22		40
#define SWMUX_SDL_DISP0_DAT23		41
#define SWMUX_SDL_DISP0_DAT3		42
#define SWMUX_SDL_DISP0_DAT4		43
#define SWMUX_SDL_DISP0_DAT5		44
#define SWMUX_SDL_DISP0_DAT6		45
#define SWMUX_SDL_DISP0_DAT7		46
#define SWMUX_SDL_DISP0_DAT8		47
#define SWMUX_SDL_DISP0_DAT9		48
#define SWMUX_SDL_EIM_A16		49
#define SWMUX_SDL_EIM_A17		50
#define SWMUX_SDL_EIM_A18		51
#define SWMUX_SDL_EIM_A19		52
#define SWMUX_SDL_EIM_A20		53
#define SWMUX_SDL_EIM_A21		54
#define SWMUX_SDL_EIM_A22		55
#define SWMUX_SDL_EIM_A23		56
#define SWMUX_SDL_EIM_A24		57
#define SWMUX_SDL_EIM_A25		58
#define SWMUX_SDL_EIM_BCLK		59
#define SWMUX_SDL_EIM_CS0		60
#define SWMUX_SDL_EIM_CS1		61
#define SWMUX_SDL_EIM_D16		62
#define SWMUX_SDL_EIM_D17		63
#define SWMUX_SDL_EIM_D18		64
#define SWMUX_SDL_EIM_D19		65
#define SWMUX_SDL_EIM_D20		66
#define SWMUX_SDL_EIM_D21		67
#define SWMUX_SDL_EIM_D22		68
#define SWMUX_SDL_EIM_D23		69
#define SWMUX_SDL_EIM_D24		70
#define SWMUX_SDL_EIM_D25		71
#define SWMUX_SDL_EIM_D26		72
#define SWMUX_SDL_EIM_D27		73
#define SWMUX_SDL_EIM_D28		74
#define SWMUX_SDL_EIM_D29		75
#define SWMUX_SDL_EIM_D30		76
#define SWMUX_SDL_EIM_D31		77
#define SWMUX_SDL_EIM_DA0		78
#define SWMUX_SDL_EIM_DA1		79
#define SWMUX_SDL_EIM_DA10		80
#define SWMUX_SDL_EIM_DA11		81
#define SWMUX_SDL_EIM_DA12		82
#define SWMUX_SDL_EIM_DA13		83
#define SWMUX_SDL_EIM_DA14		84
#define SWMUX_SDL_EIM_DA15		85
#define SWMUX_SDL_EIM_DA2		86
#define SWMUX_SDL_EIM_DA3		87
#define SWMUX_SDL_EIM_DA4		88
#define SWMUX_SDL_EIM_DA5		89
#define SWMUX_SDL_EIM_DA6		90
#define SWMUX_SDL_EIM_DA7		91
#define SWMUX_SDL_EIM_DA8		92
#define SWMUX_SDL_EIM_DA9		93
#define SWMUX_SDL_EIM_EB0		94
#define SWMUX_SDL_EIM_EB1		95
#define SWMUX_SDL_EIM_EB2		96
#define SWMUX_SDL_EIM_EB3		97
#define SWMUX_SDL_EIM_LBA		98
#define SWMUX_SDL_EIM_OE		99
#define SWMUX_SDL_EIM_RW		100
#define SWMUX_SDL_EIM_WAIT		101
#define SWMUX_SDL_ENET_CRS_DV		102
#define SWMUX_SDL_ENET_MDC		103
#define SWMUX_SDL_ENET_MDIO		104
#define SWMUX_SDL_ENET_REF_CLK		105
#define SWMUX_SDL_ENET_RX_ER		106
#define SWMUX_SDL_ENET_RXD0		107
#define SWMUX_SDL_ENET_RXD1		108
#define SWMUX_SDL_ENET_TX_EN		109
#define SWMUX_SDL_ENET_TXD0		110
#define SWMUX_SDL_ENET_TXD1		111
#define SWMUX_SDL_GPIO_0		112
#define SWMUX_SDL_GPIO_1		113
#define SWMUX_SDL_GPIO_16		114
#define SWMUX_SDL_GPIO_17		115
#define SWMUX_SDL_GPIO_18		116
#define SWMUX_SDL_GPIO_19		117
#define SWMUX_SDL_GPIO_2		118
#define SWMUX_SDL_GPIO_3		119
#define SWMUX_SDL_GPIO_4		120
#define SWMUX_SDL_GPIO_5		121
#define SWMUX_SDL_GPIO_6		122
#define SWMUX_SDL_GPIO_7		123
#define SWMUX_SDL_GPIO_8		124
#define SWMUX_SDL_GPIO_9		125
#define SWMUX_SDL_KEY_COL0		126
#define SWMUX_SDL_KEY_COL1		127
#define SWMUX_SDL_KEY_COL2		128
#define SWMUX_SDL_KEY_COL3		129
#define SWMUX_SDL_KEY_COL4		130
#define SWMUX_SDL_KEY_ROW0		131
#define SWMUX_SDL_KEY_ROW1		132
#define SWMUX_SDL_KEY_ROW2		133
#define SWMUX_SDL_KEY_ROW3		134
#define SWMUX_SDL_KEY_ROW4		135
#define SWMUX_SDL_NANDF_ALE		136
#define SWMUX_SDL_NANDF_CLE		137
#define SWMUX_SDL_NANDF_CS0		138
#define SWMUX_SDL_NANDF_CS1		139
#define SWMUX_SDL_NANDF_CS2		140
#define SWMUX_SDL_NANDF_CS3		141
#define SWMUX_SDL_NANDF_D0		142
#define SWMUX_SDL_NANDF_D1		143
#define SWMUX_SDL_NANDF_D2		144
#define SWMUX_SDL_NANDF_D3		145
#define SWMUX_SDL_NANDF_D4		146
#define SWMUX_SDL_NANDF_D5		147
#define SWMUX_SDL_NANDF_D6		148
#define SWMUX_SDL_NANDF_D7		149
#define SWMUX_SDL_NANDF_RB0		150
#define SWMUX_SDL_NANDF_WP_B		151
#define SWMUX_SDL_RGMII_RD0		152
#define SWMUX_SDL_RGMII_RD1		153
#define SWMUX_SDL_RGMII_RD2		154
#define SWMUX_SDL_RGMII_RD3		155
#define SWMUX_SDL_RGMII_RX_CTL		156
#define SWMUX_SDL_RGMII_RXC		157
#define SWMUX_SDL_RGMII_TD0		158
#define SWMUX_SDL_RGMII_TD1		159
#define SWMUX_SDL_RGMII_TD2		160
#define SWMUX_SDL_RGMII_TD3		161
#define SWMUX_SDL_RGMII_TX_CTL		162
#define SWMUX_SDL_RGMII_TXC		163
#define SWMUX_SDL_SD1_CLK		164
#define SWMUX_SDL_SD1_CMD		165
#define SWMUX_SDL_SD1_DAT0		166
#define SWMUX_SDL_SD1_DAT1		167
#define SWMUX_SDL_SD1_DAT2		168
#define SWMUX_SDL_SD1_DAT3		169
#define SWMUX_SDL_SD2_CLK		170
#define SWMUX_SDL_SD2_CMD		171
#define SWMUX_SDL_SD2_DAT0		172
#define SWMUX_SDL_SD2_DAT1		173
#define SWMUX_SDL_SD2_DAT2		174
#define SWMUX_SDL_SD2_DAT3		175
#define SWMUX_SDL_SD3_CLK		176
#define SWMUX_SDL_SD3_CMD		177
#define SWMUX_SDL_MXSDL_PAD_SD3_CMD	177
#define SWMUX_SDL_SD3_CMD		177
#define SWMUX_SDL_SD3_DAT0		178
#define SWMUX_SDL_SD3_DAT1		179
#define SWMUX_SDL_SD3_DAT2		180
#define SWMUX_SDL_SD3_DAT3		181
#define SWMUX_SDL_SD3_DAT4		182
#define SWMUX_SDL_SD3_DAT5		183
#define SWMUX_SDL_SD3_DAT6		184
#define SWMUX_SDL_SD3_DAT7		185
#define SWMUX_SDL_SD3_RST		186
#define SWMUX_SDL_SD4_CLK		187
#define SWMUX_SDL_SD4_CMD		188
#define SWMUX_SDL_SD4_DAT0		189
#define SWMUX_SDL_SD4_DAT1		190
#define SWMUX_SDL_SD4_DAT2		191
#define SWMUX_SDL_SD4_DAT3		192
#define SWMUX_SDL_SD4_DAT4		193
#define SWMUX_SDL_SD4_DAT5		194
#define SWMUX_SDL_SD4_DAT6		195
#define SWMUX_SDL_SD4_DAT7		196

/*
 * offsets of IOMUXC registers from MX6X_IOMUX_SWPAD
 * where MX6X_IOMUX_SWPAD = MX6X_IOMUXC_BASE + 0x0360
 */

#define SWPAD_SDL_CSI0_DAT10		0
#define SWPAD_SDL_CSI0_DAT11		1
#define SWPAD_SDL_CSI0_DAT12		2
#define SWPAD_SDL_CSI0_DAT13		3
#define SWPAD_SDL_CSI0_DAT14		4
#define SWPAD_SDL_CSI0_DAT15		5
#define SWPAD_SDL_CSI0_DAT16		6
#define SWPAD_SDL_CSI0_DAT17		7
#define SWPAD_SDL_CSI0_DAT18		8
#define SWPAD_SDL_CSI0_DAT19		9
#define SWPAD_SDL_CSI0_DAT4		10
#define SWPAD_SDL_CSI0_DAT5		11
#define SWPAD_SDL_CSI0_DAT6		12
#define SWPAD_SDL_CSI0_DAT7		13
#define SWPAD_SDL_CSI0_DAT8		14
#define SWPAD_SDL_CSI0_DAT9		15
#define SWPAD_SDL_CSI0_DATA_EN		16
#define SWPAD_SDL_CSI0_MCLK		17
#define SWPAD_SDL_CSI0_PIXCLK		18
#define SWPAD_SDL_CSI0_VSYNC		19
#define SWPAD_SDL_DI0_DISP_CLK		20
#define SWPAD_SDL_DI0_PIN15		21
#define SWPAD_SDL_DI0_PIN2		22
#define SWPAD_SDL_DI0_PIN3		23
#define SWPAD_SDL_DI0_PIN4		24
#define SWPAD_SDL_DISP0_DAT0		25
#define SWPAD_SDL_DISP0_DAT1		26
#define SWPAD_SDL_DISP0_DAT10		27
#define SWPAD_SDL_DISP0_DAT11		28
#define SWPAD_SDL_DISP0_DAT12		29
#define SWPAD_SDL_DISP0_DAT13		30
#define SWPAD_SDL_DISP0_DAT14		31
#define SWPAD_SDL_DISP0_DAT15		32
#define SWPAD_SDL_DISP0_DAT16		33
#define SWPAD_SDL_DISP0_DAT17		34
#define SWPAD_SDL_DISP0_DAT18		35
#define SWPAD_SDL_DISP0_DAT19		36
#define SWPAD_SDL_DISP0_DAT2		37
#define SWPAD_SDL_DISP0_DAT20		38
#define SWPAD_SDL_DISP0_DAT21		39
#define SWPAD_SDL_DISP0_DAT22		40
#define SWPAD_SDL_DISP0_DAT23		41
#define SWPAD_SDL_DISP0_DAT3		42
#define SWPAD_SDL_DISP0_DAT4		43
#define SWPAD_SDL_DISP0_DAT5		44
#define SWPAD_SDL_DISP0_DAT6		45
#define SWPAD_SDL_DISP0_DAT7		46
#define SWPAD_SDL_DISP0_DAT8		47
#define SWPAD_SDL_DISP0_DAT9		48
#define SWPAD_SDL_DRAM_A0		49
#define SWPAD_SDL_DRAM_A1		50
#define SWPAD_SDL_DRAM_A10		51
#define SWPAD_SDL_DRAM_A11		52
#define SWPAD_SDL_DRAM_A12		53
#define SWPAD_SDL_DRAM_A13		54
#define SWPAD_SDL_DRAM_A14		55
#define SWPAD_SDL_DRAM_A15		56
#define SWPAD_SDL_DRAM_A2		57
#define SWPAD_SDL_DRAM_A3		58
#define SWPAD_SDL_DRAM_A4		59
#define SWPAD_SDL_DRAM_A5		60
#define SWPAD_SDL_DRAM_A6		61
#define SWPAD_SDL_DRAM_A7		62
#define SWPAD_SDL_DRAM_A8		63
#define SWPAD_SDL_DRAM_A9		64
#define SWPAD_SDL_DRAM_CAS		65
#define SWPAD_SDL_DRAM_CS0		66
#define SWPAD_SDL_DRAM_CS1		67
#define SWPAD_SDL_DRAM_DQM0		68
#define SWPAD_SDL_DRAM_DQM1		69
#define SWPAD_SDL_DRAM_DQM2		70
#define SWPAD_SDL_DRAM_DQM3		71
#define SWPAD_SDL_DRAM_DQM4		72
#define SWPAD_SDL_DRAM_DQM5		73
#define SWPAD_SDL_DRAM_DQM6		74
#define SWPAD_SDL_DRAM_DQM7		75
#define SWPAD_SDL_DRAM_RAS		76
#define SWPAD_SDL_DRAM_RESET		77
#define SWPAD_SDL_DRAM_SDBA0		78
#define SWPAD_SDL_DRAM_SDBA1		79
#define SWPAD_SDL_DRAM_SDBA2		80
#define SWPAD_SDL_DRAM_SDCKE0		81
#define SWPAD_SDL_DRAM_SDCKE1		82
#define SWPAD_SDL_DRAM_SDCLK_0		83
#define SWPAD_SDL_DRAM_SDCLK_1		84
#define SWPAD_SDL_DRAM_SDODT0		85
#define SWPAD_SDL_DRAM_SDODT1		86
#define SWPAD_SDL_DRAM_SDQS0		87
#define SWPAD_SDL_DRAM_SDQS1		88
#define SWPAD_SDL_DRAM_SDQS2		89
#define SWPAD_SDL_DRAM_SDQS3		90
#define SWPAD_SDL_DRAM_SDQS4		91
#define SWPAD_SDL_DRAM_SDQS5		92
#define SWPAD_SDL_DRAM_SDQS6		93
#define SWPAD_SDL_DRAM_SDQS7		94
#define SWPAD_SDL_DRAM_SDWE		95
#define SWPAD_SDL_EIM_A16		96
#define SWPAD_SDL_EIM_A17		97
#define SWPAD_SDL_EIM_A18		98
#define SWPAD_SDL_EIM_A19		99
#define SWPAD_SDL_EIM_A20		100
#define SWPAD_SDL_EIM_A21		101
#define SWPAD_SDL_EIM_A22		102
#define SWPAD_SDL_EIM_A23		103
#define SWPAD_SDL_EIM_A24		104
#define SWPAD_SDL_EIM_A25		105
#define SWPAD_SDL_EIM_BCLK		106
#define SWPAD_SDL_EIM_CS0		107
#define SWPAD_SDL_EIM_CS1		108
#define SWPAD_SDL_EIM_D16		109
#define SWPAD_SDL_EIM_D17		110
#define SWPAD_SDL_EIM_D18		111
#define SWPAD_SDL_EIM_D19		112
#define SWPAD_SDL_EIM_D20		113
#define SWPAD_SDL_EIM_D21		114
#define SWPAD_SDL_EIM_D22		115
#define SWPAD_SDL_EIM_D23		116
#define SWPAD_SDL_EIM_D24		117
#define SWPAD_SDL_EIM_D25		118
#define SWPAD_SDL_EIM_D26		119
#define SWPAD_SDL_EIM_D27		120
#define SWPAD_SDL_EIM_D28		121
#define SWPAD_SDL_EIM_D29		122
#define SWPAD_SDL_EIM_D30		123
#define SWPAD_SDL_EIM_D31		124
#define SWPAD_SDL_EIM_DA0		125
#define SWPAD_SDL_EIM_DA1		126
#define SWPAD_SDL_EIM_DA10		127
#define SWPAD_SDL_EIM_DA11		128
#define SWPAD_SDL_EIM_DA12		129
#define SWPAD_SDL_EIM_DA13		130
#define SWPAD_SDL_EIM_DA14		131
#define SWPAD_SDL_EIM_DA15		132
#define SWPAD_SDL_EIM_DA2		133
#define SWPAD_SDL_EIM_DA3		134
#define SWPAD_SDL_EIM_DA4		135
#define SWPAD_SDL_EIM_DA5		136
#define SWPAD_SDL_EIM_DA6		137
#define SWPAD_SDL_EIM_DA7		138
#define SWPAD_SDL_EIM_DA8		139
#define SWPAD_SDL_EIM_DA9		140
#define SWPAD_SDL_EIM_EB0		141
#define SWPAD_SDL_EIM_EB1		142
#define SWPAD_SDL_EIM_EB2		143
#define SWPAD_SDL_EIM_EB3		144
#define SWPAD_SDL_EIM_LBA		145
#define SWPAD_SDL_EIM_OE		146
#define SWPAD_SDL_EIM_RW		147
#define SWPAD_SDL_EIM_WAIT		148
#define SWPAD_SDL_ENET_CRS_DV		149
#define SWPAD_SDL_ENET_MDC		150
#define SWPAD_SDL_ENET_MDIO		151
#define SWPAD_SDL_ENET_REF_CLK		152
#define SWPAD_SDL_ENET_RX_ER		153
#define SWPAD_SDL_ENET_RXD0		154
#define SWPAD_SDL_ENET_RXD1		155
#define SWPAD_SDL_ENET_TX_EN		156
#define SWPAD_SDL_ENET_TXD0		157
#define SWPAD_SDL_ENET_TXD1		158
#define SWPAD_SDL_GPIO_0		159
#define SWPAD_SDL_GPIO_1		160
#define SWPAD_SDL_GPIO_16		161
#define SWPAD_SDL_GPIO_17		162
#define SWPAD_SDL_GPIO_18		163
#define SWPAD_SDL_GPIO_19		164
#define SWPAD_SDL_GPIO_2		165
#define SWPAD_SDL_GPIO_3		166
#define SWPAD_SDL_GPIO_4		167
#define SWPAD_SDL_GPIO_5		168
#define SWPAD_SDL_GPIO_6		169
#define SWPAD_SDL_GPIO_7		170
#define SWPAD_SDL_GPIO_8		171
#define SWPAD_SDL_GPIO_9		172
#define SWPAD_SDL_JTAG_MOD		173
#define SWPAD_SDL_JTAG_TCK		174
#define SWPAD_SDL_JTAG_TDI		175
#define SWPAD_SDL_JTAG_TDO		176
#define SWPAD_SDL_JTAG_TMS		177
#define SWPAD_SDL_JTAG_TRSTB		178
#define SWPAD_SDL_KEY_COL0		179
#define SWPAD_SDL_KEY_COL1		180
#define SWPAD_SDL_KEY_COL2		181
#define SWPAD_SDL_KEY_COL3		182
#define SWPAD_SDL_KEY_COL4		183
#define SWPAD_SDL_KEY_ROW0		184
#define SWPAD_SDL_KEY_ROW1		185
#define SWPAD_SDL_KEY_ROW2		186
#define SWPAD_SDL_KEY_ROW3		187
#define SWPAD_SDL_KEY_ROW4		188
#define SWPAD_SDL_NANDF_ALE		189
#define SWPAD_SDL_NANDF_CLE		190
#define SWPAD_SDL_NANDF_CS0		191
#define SWPAD_SDL_NANDF_CS1		192
#define SWPAD_SDL_NANDF_CS2		193
#define SWPAD_SDL_NANDF_CS3		194
#define SWPAD_SDL_NANDF_D0		195
#define SWPAD_SDL_NANDF_D1		196
#define SWPAD_SDL_NANDF_D2		197
#define SWPAD_SDL_NANDF_D3		198
#define SWPAD_SDL_NANDF_D4		199
#define SWPAD_SDL_NANDF_D5		200
#define SWPAD_SDL_NANDF_D6		201
#define SWPAD_SDL_NANDF_D7		202
#define SWPAD_SDL_NANDF_RB0		203
#define SWPAD_SDL_NANDF_WP_B		204
#define SWPAD_SDL_RGMII_RD0		205
#define SWPAD_SDL_RGMII_RD1		206
#define SWPAD_SDL_RGMII_RD2		207
#define SWPAD_SDL_RGMII_RD3		208
#define SWPAD_SDL_RGMII_RX_CTL		209
#define SWPAD_SDL_RGMII_RXC		210
#define SWPAD_SDL_RGMII_TD0		211
#define SWPAD_SDL_RGMII_TD1		212
#define SWPAD_SDL_RGMII_TD2		213
#define SWPAD_SDL_RGMII_TD3		214
#define SWPAD_SDL_RGMII_TX_CTL		215
#define SWPAD_SDL_RGMII_TXC		216
#define SWPAD_SDL_SD1_CLK		217
#define SWPAD_SDL_SD1_CMD		218
#define SWPAD_SDL_SD1_DAT0		219
#define SWPAD_SDL_SD1_DAT1		220
#define SWPAD_SDL_SD1_DAT2		221
#define SWPAD_SDL_SD1_DAT3		222
#define SWPAD_SDL_SD2_CLK		223
#define SWPAD_SDL_SD2_CMD		224
#define SWPAD_SDL_SD2_DAT0		225
#define SWPAD_SDL_SD2_DAT1		226
#define SWPAD_SDL_SD2_DAT2		227
#define SWPAD_SDL_SD2_DAT3		228
#define SWPAD_SDL_SD3_CLK		229
#define SWPAD_SDL_SD3_CMD		230
#define SWPAD_SDL_SD3_DAT0		231
#define SWPAD_SDL_SD3_DAT1		232
#define SWPAD_SDL_SD3_DAT2		233
#define SWPAD_SDL_SD3_DAT3		234
#define SWPAD_SDL_SD3_DAT4		235
#define SWPAD_SDL_SD3_DAT5		236
#define SWPAD_SDL_SD3_DAT6		237
#define SWPAD_SDL_SD3_DAT7		238
#define SWPAD_SDL_SD3_RST		239
#define SWPAD_SDL_SD4_CLK		240
#define SWPAD_SDL_SD4_CMD		241
#define SWPAD_SDL_SD4_DAT0		242
#define SWPAD_SDL_SD4_DAT1		243
#define SWPAD_SDL_SD4_DAT2		244
#define SWPAD_SDL_SD4_DAT3		245
#define SWPAD_SDL_SD4_DAT4		246
#define SWPAD_SDL_SD4_DAT5		247
#define SWPAD_SDL_SD4_DAT6		248
#define SWPAD_SDL_SD4_DAT7		249
#define SWPAD_SDL_GRP_B7DS		250
#define SWPAD_SDL_GRP_ADDS		251
#define SWPAD_SDL_GRP_DDRMODE_CTL	252
#define SWPAD_SDL_GRP_DDRPKE		253
#define SWPAD_SDL_GRP_DDRPK		254
#define SWPAD_SDL_GRP_DDRHYS		255
#define SWPAD_SDL_GRP_DDRMODE		256
#define SWPAD_SDL_GRP_B0DS		257
#define SWPAD_SDL_GRP_DDR_TYPE_RGMII	258
#define SWPAD_SDL_GRP_CTLDS		259
#define SWPAD_SDL_GRP_B1DS		260
#define SWPAD_SDL_GRP_DDR_TYPE		261
#define SWPAD_SDL_GRP_B2DS		262
#define SWPAD_SDL_GRP_B3DS		263
#define SWPAD_SDL_GRP_B4DS		264
#define SWPAD_SDL_GRP_B5DS		265
#define SWPAD_SDL_GRP_RGMII_TERM	266
#define SWPAD_SDL_GRP_B6DS		267


/*
 * offsets of IOMUXC registers from MX6DQ_IOMUX_INPUT
 * where MX6DQ_IOMUX_INPUT = MX6X_IOMUXC_BASE + 0x0790
 */

#define SWINPUT_SDL_ANATOP_USB_OTG_ID				0
#define SWINPUT_SDL_ASRC_ASRCK_CLOCK_6				1
#define SWINPUT_SDL_AUDMUX_P4_DA_AMX				2
#define SWINPUT_SDL_AUDMUX_P4_DB_AMX				3
#define SWINPUT_SDL_AUDMUX_P4_RXCLK_AMX				4
#define SWINPUT_SDL_AUDMUX_P4_RXFS_AMX				5
#define SWINPUT_SDL_AUDMUX_P4_TXCLK_AMX				6
#define SWINPUT_SDL_AUDMUX_P4_TXFS_AMX				7
#define SWINPUT_SDL_AUDMUX_P5_DA_AMX				8
#define SWINPUT_SDL_AUDMUX_P5_DB_AMX				9
#define SWINPUT_SDL_AUDMUX_P5_RXCLK_AMX				10
#define SWINPUT_SDL_AUDMUX_P5_RXFS_AMX				11
#define SWINPUT_SDL_AUDMUX_P5_TXCLK_AMX				12
#define SWINPUT_SDL_AUDMUX_P5_TXFS_AMX				13
#define SWINPUT_SDL_CAN1_IPP_IND_CANRX				14
#define SWINPUT_SDL_CAN2_IPP_IND_CANRX				15
#define SWINPUT_SDL_CCM_IPP_DI1_CLK				16
#define SWINPUT_SDL_CCM_PMIC_VFUNCTIONAL_READY			17
#define SWINPUT_SDL_ECSPI1_IPP_CSPI_CLK				18
#define SWINPUT_SDL_ECSPI1_IPP_IND_MISO				19
#define SWINPUT_SDL_ECSPI1_IPP_IND_MOSI				20
#define SWINPUT_SDL_ECSPI1_IPP_IND_SS_B_0			21
#define SWINPUT_SDL_ECSPI1_IPP_IND_SS_B_1			22
#define SWINPUT_SDL_ECSPI1_IPP_IND_SS_B_2			23
#define SWINPUT_SDL_ECSPI1_IPP_IND_SS_B_3			24
#define SWINPUT_SDL_ECSPI2_IPP_CSPI_CLK				25
#define SWINPUT_SDL_ECSPI2_IPP_IND_MISO				26
#define SWINPUT_SDL_ECSPI2_IPP_IND_MOSI				27
#define SWINPUT_SDL_ECSPI2_IPP_IND_SS_B_0			28
#define SWINPUT_SDL_ECSPI2_IPP_IND_SS_B_1			29
#define SWINPUT_SDL_ECSPI4_IPP_IND_SS_B_0			30
#define SWINPUT_SDL_ENET_IPG_CLK_RMII				31
#define SWINPUT_SDL_ENET_IPP_IND_MAC0_MDIO			32
#define SWINPUT_SDL_ENET_IPP_IND_MAC0_RXCLK			33
#define SWINPUT_SDL_ENET_IPP_IND_MAC_RXDATA_0			34
#define SWINPUT_SDL_ENET_IPP_IND_MAC_RXDATA_1			35
#define SWINPUT_SDL_ENET_IPP_IND_MAC_RXDATA_2			36
#define SWINPUT_SDL_ENET_IPP_IND_MAC_RXDATA_3			37
#define SWINPUT_SDL_ENET_IPP_IND_MAC0_RXEN			38
#define SWINPUT_SDL_ESAI1_IPP_IND_FSR				39
#define SWINPUT_SDL_ESAI1_IPP_IND_FST				40
#define SWINPUT_SDL_ESAI1_IPP_IND_HCKR				41
#define SWINPUT_SDL_ESAI1_IPP_IND_HCKT				42
#define SWINPUT_SDL_ESAI1_IPP_IND_SCKR				43
#define SWINPUT_SDL_ESAI1_IPP_IND_SCKT				44
#define SWINPUT_SDL_ESAI1_IPP_IND_SDO0				45
#define SWINPUT_SDL_ESAI1_IPP_IND_SDO1				46
#define SWINPUT_SDL_ESAI1_IPP_IND_SDO2_SDI3			47
#define SWINPUT_SDL_ESAI1_IPP_IND_SDO3_SDI2			48
#define SWINPUT_SDL_ESAI1_IPP_IND_SDO4_SDI1			49
#define SWINPUT_SDL_ESAI1_IPP_IND_SDO5_SDI0			50
#define SWINPUT_SDL_HDMI_TX_ICECIN				51
#define SWINPUT_SDL_HDMI_TX_II2C_MSTH13TDDC_CLKIN		52
#define SWINPUT_SDL_HDMI_TX_II2C_MSTH14TDDC_SDAIN		53
#define SWINPUT_SDL_I2C1_IPP_SCL_IN				54
#define SWINPUT_SDL_I2C1_IPP_SDA_IN				55
#define SWINPUT_SDL_I2C2_IPP_SCL_IN				56
#define SWINPUT_SDL_I2C2_IPP_SDA_IN				57
#define SWINPUT_SDL_I2C3_IPP_SCL_IN				58
#define SWINPUT_SDL_I2C3_IPP_SDA_IN				59
#define SWINPUT_SDL_I2C4_IPP_SCL_IN				60
#define SWINPUT_SDL_I2C4_IPP_SDA_IN				61
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_10			62
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_11			63
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_12			64
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_13			65
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_14			66
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_15			67
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_16			68
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_17			69
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_18			70
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_19			71
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_DATA_EN			72
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_HSYNC			73
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_PIX_CLK			74
#define SWINPUT_SDL_IPU1_IPP_IND_SENS1_VSYNC			75
#define SWINPUT_SDL_KPP_IPP_IND_COL_5				76
#define SWINPUT_SDL_KPP_IPP_IND_COL_6				77
#define SWINPUT_SDL_KPP_IPP_IND_COL_7				78
#define SWINPUT_SDL_KPP_IPP_IND_ROW_5				79
#define SWINPUT_SDL_KPP_IPP_IND_ROW_6				80
#define SWINPUT_SDL_KPP_IPP_IND_ROW_7				81
#define SWINPUT_SDL_LCDIF_LCDIF_BUSY				82
#define SWINPUT_SDL_MLB_MLB_CLK_IN				83
#define SWINPUT_SDL_MLB_MLB_DATA_IN				84
#define SWINPUT_SDL_MLB_MLB_SIG_IN				85
#define SWINPUT_SDL_SDMA_EVENTS_14				86
#define SWINPUT_SDL_SDMA_EVENTS_15				87
#define SWINPUT_SDL_SPDIF_SPDIF_IN1				88
#define SWINPUT_SDL_SPDIF_TX_CLK2				89
#define SWINPUT_SDL_UART1_IPP_UART_RTS_B			90
#define SWINPUT_SDL_UART1_IPP_UART_RXD_MUX			91
#define SWINPUT_SDL_UART2_IPP_UART_RTS_B			92
#define SWINPUT_SDL_UART2_IPP_UART_RXD_MUX			93
#define SWINPUT_SDL_UART3_IPP_UART_RTS_B			94
#define SWINPUT_SDL_UART3_IPP_UART_RXD_MUX			95
#define SWINPUT_SDL_UART4_IPP_UART_RTS_B			96
#define SWINPUT_SDL_UART4_IPP_UART_RXD_MUX			97
#define SWINPUT_SDL_UART5_IPP_UART_RTS_B			98
#define SWINPUT_SDL_UART5_IPP_UART_RXD_MUX			99
#define SWINPUT_SDL_USB0H3_IPP_IND_OTG_OC			100
#define SWINPUT_SDL_USB0H3_IPP_IND_UH1_OC			101
#define SWINPUT_SDL_USDHC1_IPP_CARD_CLK_IN			102
#define SWINPUT_SDL_USDHC1_IPP_WP_ON				103
#define SWINPUT_SDL_USDHC2_IPP_CARD_CLK_IN			104
#define SWINPUT_SDL_USDHC3_IPP_CARD_CLK_IN			105
#define SWINPUT_SDL_USDHC4_IPP_CARD_CLK_IN			106

#endif	/* __ARM_MX6SDL_IOMUX_H_INCLUDED */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
#ifdef __ASM__
__SRCVERSION "$URL: http://svn.ott.qnx.com/product/mainline/hardware/startup/lib/public/arm/mx6sdl_iomux.h $ $Rev: 870901 $"
#else
__SRCVERSION( "$URL: http://svn.ott.qnx.com/product/mainline/hardware/startup/lib/public/arm/mx6sdl_iomux.h $ $Rev: 870901 $" )
#endif
#endif
