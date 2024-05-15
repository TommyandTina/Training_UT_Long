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

#include "common.h"
#include "decoder.h"

#ifndef __ADV7612_H__
#define __ADV7612_H__

#define ADV7612_I2C_MAP_NOT_ADDR    0xFF
#define ADV7612_I2C_MAP_WAIT        0xFE
#define ADV7612_I2C_MAP_EOR         0xFD   

/*****************I2C slave address**********************/
#define ADV7612_I2C_MAP_IO          0x4C    
#define ADV7612_I2C_MAP_CEC         0x40    
#define ADV7612_I2C_MAP_INFOFRAME   0x3E    
#define ADV7612_I2C_MAP_DPLL        0x26   
#define ADV7612_I2C_MAP_KSV         0x32   
#define ADV7612_I2C_MAP_EDID        0x36   
#define ADV7612_I2C_MAP_HDMI        0x34    
#define ADV7612_I2C_MAP_CP          0x22   


/*****************IO registers**********************/
 /* Power Control register */
#define ADV7612_IO_PWR_CTRL_REG  0x0C
/* Power up */  
#define ADV7612_IO_PWR_UP        0x42
/* Power down */    
#define ADV7612_IO_PWR_DOWN      0x62    


/*****************CP registers**********************/
/* Contrast control register */
#define ADV7612_CP_CON_CTRL_REG  0x3a   
#define ADV7612_CP_CON_CTRL_MIN  0    
#define ADV7612_CP_CON_CTRL_DEF  128    
#define ADV7612_CP_CON_CTRL_MAX  255     

/* Saturation control register */
#define ADV7612_CP_SAT_CTRL_REG  0x3b 
#define ADV7612_CP_SAT_CTRL_MIN  0    
#define ADV7612_CP_SAT_CTRL_DEF  128  
#define ADV7612_CP_SAT_CTRL_MAX  255     

/* Brightness control register */
#define ADV7612_CP_BRI_CTRL_REG  0x3c    
#define ADV7612_CP_BRI_CTRL_MIN  -128    
#define ADV7612_CP_BRI_CTRL_DEF  240     
#define ADV7612_CP_BRI_CTRL_MAX  127     

/* Hue control register */
#define ADV7612_CP_HUE_CTRL_REG  0x3d    
#define ADV7612_CP_HUE_CTRL_MIN  0     
#define ADV7612_CP_HUE_CTRL_DEF  0      
#define ADV7612_CP_HUE_CTRL_MAX  255     

/* Video adjustment register */
#define ADV7612_CP_VID_ADJ_REG      0x3e
/* Enable color controls */
#define ADV7612_CP_VID_ADJ_ENABLE   0x80


/*****************HDMI registers**********************/
#define ADV7612_HDMI_LINE_WIDTH_1_REG           0x07
#define ADV7612_HDMI_VERT_FILTER_LOCKED         0x80     
#define ADV7612_HDMI_DE_REGEN_FILTER_LOCKED     0x20       
#define ADV7612_HDMI_LINE_WIDTH_MSBS_MASK       0x1F      
#define ADV7612_HDMI_LINE_WIDTH_2_REG           0x08  
#define ADV7612_HDMI_FIELD0_HEIGHT_1_REG        0x09    
#define ADV7612_HDMI_FIELD0_HEIGHT_1_MASK       0x1F    
#define ADV7612_HDMI_FIELD0_HEIGHT_2_REG        0x0A    
#define ADV7612_HDMI_FIELD1_HEIGHT_REG          0x0B   
#define ADV7612_HDMI_HDMI_INTERLACE             0x20  

#endif // __ADV7612_H__
