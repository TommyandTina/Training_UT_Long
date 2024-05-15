/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#ifndef __HWINFO_RCAR_H_INCLUDED
#define __HWINFO_RCAR_H_INCLUDED

#include <hw/sysinfo.h>

/*
 * =============================================================================
 *
 *                                B U S E S
 *
 * =============================================================================
*/
#define RCAR_HWI_I2C                        HWI_ITEM_BUS_I2C
#define RCAR_HWI_IIC                        "iic"
#define RCAR_HWI_I2C_POC                    "i2c,poc"

#define RCAR_HWI_SPI                        HWI_ITEM_BUS_SPI
 #define RCAR_HWI_RPC_WBUF                  "wbuf"
 #define RCAR_HWI_RPC_WBUF_SIZE             "wbuf_size"
#define RCAR_HWI_SPI_DEV                    "spi_dev"

#define RCAR_HWI_MSIOF                      "msiof_spi"
#define RCAR_HWI_MSIOF_OPS                  "msiof_options"
 #define RCAR_HWI_MSIOF_DREQ_TXI            "msiof_dreq_txi"
 #define RCAR_HWI_MSIOF_DREQ_RXI            "msiof_dreq_Rxi"

#define RCAR_HWI_SDHI                       HWI_ITEM_BUS_SDIO
 #define RCAR_HWI_SDHI_VDD                  "sdio,vdd"
 #define RCAR_HWI_SDHI_IF                   "sdio,if"
 #define RCAR_HWI_SDHI_PFC                  "sdio,pfc"

/*
 * =============================================================================
 *
 *                              D E V I C E S
 *
 * =============================================================================
*/
#define RCAR_HWI_PFC                        "pfc"
 #define RCAR_HWI_PFC_PMMR                  "pfc,pmmr"
 #define RCAR_HWI_PFC_REG                   "pfc,reg"
 #define RCAR_HWI_PFC_MASK                  "pfc,mask"
 #define RCAR_HWI_PFC_SHIFT                 "pfc,shift"

#define RCAR_HWI_UART                       "scif"

#define RCAR_HWI_HSUART                     "hscif"

#define RCAR_HWI_SYSDMAC                    "sys"
#define RCAR_HWI_AUDIODMAC                  "audio"
#define RCAR_HWI_RTDMAC                     "rt"
 #define RCAR_HWI_DMA_REG                   "dma_reg"
 #define RCAR_HWI_DMA_SIZE                  "dma_size"
 #define RCAR_HWI_DMA_DESC                  "dma_desc"
#define RCAR_HWI_AUDIOPPDMAC                "audiopp"

#define RCAR_HWI_AUDIO_ADG                  "adg"
#define RCAR_HWI_AUDIO_SSI                  "ssi"
#define RCAR_HWI_AUDIO_SSIU                 "ssiu"
#define RCAR_HWI_AUDIO_SCU                  "scu"

#define RCAR_HWI_VIN                        "vin"
#define RCAR_HWI_CSI                        "csi"
#define RCAR_HWI_ISP                        "isp"
#define RCAR_HWI_CAMERA                     "csi"
#define RCAR_HWI_ISP                        "isp"
#define RCAR_HWI_CAN                        "canfd"

#define RCAR_HWI_WDT                        "wdog"
#define RCAR_HWI_CPG                        "cpg"
 #define RCAR_HWI_CPG_MOD_ID                "cpg,mod_id"

#define RCAR_HWI_THERMAL                    "thermal"
#define RCAR_HWI_THERMAL_OPS                "thermal,option"
 #define RCAR_HWI_THERMAL_CHAN_MIN          "thermal_min"
 #define RCAR_HWI_THERMAL_CHAN_MAX          "thermal_max"
 #define RCAR_HWI_THERMAL_TYPE              "thermal_type"
 #define RCAR_HWI_THERMAL_IRQ_NUM           "thermal_irq_num"

#define RCAR_HWI_CMT                        "cmt"
 #define RCAR_HWI_CMT_REG                   "cmt_reg"
 #define RCAR_HWI_CMT_CLK                   "cmt_clk"

#define RCAR_HWI_TMU                        "tmu"
#define RCAR_HWI_GPIO                       "gpio"
 #define RCAR_HWI_GPIO_GROUP_NUM            "gpio_group_num"
#define RCAR_HWI_CPG                        "cpg"
#define RCAR_HWI_SYSC                       "sysc"
#define RCAR_HWI_FLEXRAY                    "flexray"

#define RCAR_HWI_AVS                        "avs"

#define RCAR_HWI_PMIC                       "pmic"
#define RCAR_HWI_PMIC_MODEL                 "pmic_model"
#define RCAR_HWI_PMIC_I2C_BUS               "pmic_bus"
#define RCAR_HWI_PMIC_I2C_SLAVE_ADDR        "pmic_slave_addr"

#endif //__HWINFO_RCAR_H_INCLUDED
