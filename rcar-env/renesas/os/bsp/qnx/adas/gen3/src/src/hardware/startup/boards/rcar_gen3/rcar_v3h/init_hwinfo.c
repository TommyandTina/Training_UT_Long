/*
 * $QNXLicenseC:
 * Copyright 2018, QNX Software Systems.
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

#include <startup.h>
#include <hwinfo_private.h>
#include <drvr/hwinfo.h>
#include <hw/hwinfo_rcar.h>

#include <aarch64/r-car-gen3.h>
#include <rcar_board.h>

extern unsigned board_ext_clk;

/* Helper function modeled on hwibus_add_spi */
static unsigned hwibus_add_msiof(unsigned parent_hwi_off, hwiattr_spi_t *attr)
{
  unsigned hwi_off = hwibus_add(RCAR_HWI_MSIOF, parent_hwi_off);

  if ((hwi_off != HWI_NULL_OFF) && (attr != NULL))
  {
      unsigned i;
      hwitag_add_common(hwi_off, &attr->common);
      for (i=0; i<attr->num_clks; i++)
          hwitag_add_inputclk(hwi_off, 0, 1);     // make divisor 1
      for (i=0; i<attr->num_busattr; i++)
          hwitag_add_busattr(hwi_off, NULL);      // create empty busattr tags
  }
  return hwi_off;
}

/* Helper function for setting canbus */
static unsigned rcar_hwibus_add_can(unsigned parent_hwi_off, hwiattr_can_t *attr)
{
    unsigned hwi_off = hwibus_add(HWI_ITEM_BUS_CAN, parent_hwi_off);
    if ((hwi_off != HWI_NULL_OFF) && (attr != NULL))
    {
        unsigned i;
        hwitag_add_common(hwi_off, &attr->common);
        for (i=0; i<attr->num_clks; i++) {
            hwitag_add_inputclk(hwi_off, 0, 1);
        }
    }
    return hwi_off;
}


void init_hwinfo()
{
    { // SCIF
        unsigned i, hwi_off;

        hwiattr_uart_t uart_attr = HWIATTR_UART_T_INITIALIZER;
        struct hwi_inputclk uart_clk = {.clk = 66666666, .div = 16};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_SCIF0_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF0},
            {.base=RCAR_GEN3_SCIF1_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF1},
            {.base=RCAR_GEN3_SCIF3_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF3},
            {.base=RCAR_GEN3_SCIF4_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF4},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_UART_SET_LOCATION(&uart_attr, dev[i].base,  RCAR_GEN3_SCIF_SIZE, 0, hwi_find_as(dev[i].base , 1));
            hwi_off = hwidev_add_uart(RCAR_HWI_UART, &uart_attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &uart_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    { // HSCIF
        unsigned i, hwi_off;

        hwiattr_uart_t uart_attr = HWIATTR_UART_T_INITIALIZER;
        struct hwi_inputclk uart_clk = {.clk = 266660000, .div = 16};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_HSCIF0_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF0},
            {.base=RCAR_GEN3_HSCIF1_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF1},
            {.base=RCAR_GEN3_HSCIF2_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF2},
            {.base=RCAR_GEN3_HSCIF3_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF3},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_UART_SET_LOCATION(&uart_attr, dev[i].base,  RCAR_GEN3_SCIF_SIZE, 0, hwi_find_as(dev[i].base , 1));
            hwi_off = hwidev_add_uart(RCAR_HWI_HSUART, &uart_attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &uart_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    { //SDMMC
        unsigned hwi_off;

        hwiattr_sdio_t sdio_attr = HWIATTR_SDIO_T_INITIALIZER;
        /* RCAR_GEN3_CPG_SD0CKCR = 0x4 SDnH = 800 MHz SDn = 200 Mhz*/
        struct hwi_inputclk sdio_clk = {.clk = 200000000, .div = 1};

        HWIATTR_SDIO_SET_NUM_IRQ(&sdio_attr, 1);
        HWIATTR_SDIO_SET_NUM_CLK(&sdio_attr, 1);
        HWIATTR_SDIO_SET_DLL(&sdio_attr, "rcar_gen3");

        HWIATTR_SDIO_SET_LOCATION(&sdio_attr, RCAR_GEN3_SDHI2_BASE, RCAR_GEN3_SDHI_SIZE, 0,
                                        hwi_find_as(RCAR_GEN3_SDHI2_BASE, 1));
        hwi_off = hwibus_add_sdio(HWI_NULL_OFF, &sdio_attr);
        ASSERT(hwi_find_unit(hwi_off) == 0);
        hwitag_set_inputclk(hwi_off, 0, &sdio_clk);
        hwitag_set_avail_ivec(hwi_find_bus(HWI_ITEM_BUS_SDIO, 0), -1, RCAR_GEN3_INTCSYS_SDHI0);
    }

    // RPC-SPI
    {
        unsigned hwi_off;
        hwiattr_spi_t attr = HWIATTR_SPI_T_INITIALIZER;
        struct hwi_inputclk rpc_clk = {.clk = 160000000, .div = 2};
        HWIATTR_SPI_SET_NUM_CLK(&attr, 1);

        HWIATTR_SPI_SET_LOCATION(&attr, RCAR_GEN3_RPC_BASE, RCAR_GEN3_RPC_SIZE, 0, hwi_find_as(RCAR_GEN3_RPC_BASE, 1));
        hwi_off = hwibus_add_spi(HWI_NULL_OFF, &attr);
        ASSERT(hwi_find_unit(hwi_off) == 0);
        hwitag_set_inputclk(hwi_off, 0, &rpc_clk);

        hwi_off = hwidev_add(RCAR_HWI_SPI_DEV, hwi_devclass_NONE, HWI_NULL_OFF);
        hwitag_add_location(hwi_off, RCAR_GEN3_RPC_DEV_ADDR, RCAR_GEN3_RPC_DEV_SIZE, 0, hwi_find_as(RCAR_GEN3_RPC_DEV_ADDR, 1));
        
    }

    // CSI
    {
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_CSI40_BASE},
            {.base=RCAR_GEN3_CSI41_BASE},
            {.base=RCAR_GEN3_CSI42_BASE},
            {.base=RCAR_GEN3_CSI43_BASE},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_CSI, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_CSI2_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
        }
    }

    {   // SPI
        unsigned i, hwi_off;

        hwiattr_spi_t msiof_attr = HWIATTR_SPI_T_INITIALIZER;
        struct hwi_inputclk msiof_clk = {.clk = 26000000, .div = 1};
        HWIATTR_SPI_SET_NUM_CLK(&msiof_attr, 1);

        /* each MSIOF SPI has an interrupt */
        HWIATTR_SPI_SET_NUM_IRQ(&msiof_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_MSIOF0_BASE, .irq=RCAR_GEN3_INTCSYS_MSIOF0 },
            {.base=RCAR_GEN3_MSIOF1_BASE, .irq=RCAR_GEN3_INTCSYS_MSIOF1 },
            {.base=RCAR_GEN3_MSIOF2_BASE, .irq=RCAR_GEN3_INTCSYS_MSIOF2 },
            {.base=RCAR_GEN3_MSIOF3_BASE, .irq=RCAR_GEN3_INTCSYS_MSIOF3 },
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_SPI_SET_LOCATION(&msiof_attr, dev[i].base, RCAR_GEN3_MSIOF_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_msiof(HWI_NULL_OFF, &msiof_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &msiof_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    { // I2C
        unsigned i, hwi_off;

        hwiattr_i2c_t i2c_attr = HWIATTR_I2C_T_INITIALIZER;
        /* All I2Cs operate on 133.33 MHz only */
        struct hwi_inputclk i2c_clk = {.clk = 133330000, .div = 1};

        /* each I2C has an interrupt */
        HWIATTR_I2C_SET_NUM_IRQ(&i2c_attr, 1);
        HWIATTR_I2C_SET_NUM_CLK(&i2c_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_I2C0_BASE, .irq=RCAR_GEN3_INTCSYS_I2C0},
            {.base=RCAR_GEN3_I2C1_BASE, .irq=RCAR_GEN3_INTCSYS_I2C1},
            {.base=RCAR_GEN3_I2C2_BASE, .irq=RCAR_GEN3_INTCSYS_I2C2},
            {.base=RCAR_GEN3_I2C3_BASE, .irq=RCAR_GEN3_INTCSYS_I2C3},
            {.base=RCAR_GEN3_I2C4_BASE, .irq=RCAR_GEN3_INTCSYS_I2C4},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_I2C_SET_LOCATION(&i2c_attr, dev[i].base, RCAR_GEN3_I2C_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_i2c(HWI_NULL_OFF, &i2c_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &i2c_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    { // SYSDMAC
        unsigned i, irq, hwi_off;
        hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_SYSDMAC1_BASE},
            {.base=RCAR_GEN3_SYSDMAC2_BASE},
        };

        uint32_t irq1[] = {
            RCAR_GEN3_INTCSYS_SYSDMAC1_0,  RCAR_GEN3_INTCSYS_SYSDMAC1_1,  RCAR_GEN3_INTCSYS_SYSDMAC1_2,  RCAR_GEN3_INTCSYS_SYSDMAC1_3,
            RCAR_GEN3_INTCSYS_SYSDMAC1_4,  RCAR_GEN3_INTCSYS_SYSDMAC1_5,  RCAR_GEN3_INTCSYS_SYSDMAC1_6,  RCAR_GEN3_INTCSYS_SYSDMAC1_7,
            RCAR_GEN3_INTCSYS_SCU1,        RCAR_GEN3_INTCSYS_SCU2,        RCAR_GEN3_INTCSYS_SCU3,        RCAR_GEN3_INTCSYS_SCU4,
            RCAR_GEN3_INTCSYS_SCU5,        RCAR_GEN3_INTCSYS_SCU6,        RCAR_GEN3_INTCSYS_SCU7,        RCAR_GEN3_INTCSYS_SCU8

        };
        uint32_t irq2[] = {
            RCAR_GEN3_INTCSYS_SYSDMAC1_8,  RCAR_GEN3_INTCSYS_SYSDMAC1_9,  RCAR_GEN3_INTCSYS_SYSDMAC1_10,  RCAR_GEN3_INTCSYS_SYSDMAC1_11,
            RCAR_GEN3_INTCSYS_SYSDMAC1_12, RCAR_GEN3_INTCSYS_SYSDMAC1_13, RCAR_GEN3_INTCSYS_SYSDMAC1_14,  RCAR_GEN3_INTCSYS_SYSDMAC1_15,
            RCAR_GEN3_INTCSYS_SCU9,        RCAR_GEN3_INTCSYS_MLM0,        RCAR_GEN3_INTCSYS_MLM1,         RCAR_GEN3_INTCSYS_MLM2,
            RCAR_GEN3_INTCSYS_MLM3,        RCAR_GEN3_INTCSYS_MLM4,        RCAR_GEN3_INTCSYS_MLM5,         RCAR_GEN3_INTCSYS_MLM6
        };

        uint32_t *irqs[] = { irq1, irq2 };

        HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq1));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_USB_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_SYSDMAC_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwidev_add_dma(RCAR_HWI_SYSDMAC, &attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_REG, RCAR_GEN3_SYSDMAC_REGS);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_SIZE, RCAR_GEN3_SYSDMAC_REGSIZE);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_DESC, RCAR_GEN3_SYSDMAC_DESC);
            for(irq = 0; irq < NUM_ELTS(irq1); irq++) {
                hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
            }
        }
    }

    { // RTDMAC
        unsigned i, irq, hwi_off;
        hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_RTDMAC0_BASE},
            {.base=RCAR_GEN3_RTDMAC1_BASE},
        };

        uint32_t irq0[] = {
            RCAR_GEN3_INTCSYS_V3_RTDMAC0_0,   RCAR_GEN3_INTCSYS_V3_RTDMAC0_1,   RCAR_GEN3_INTCSYS_V3_RTDMAC0_2,   RCAR_GEN3_INTCSYS_V3_RTDMAC0_3,
            RCAR_GEN3_INTCSYS_V3_RTDMAC0_4,   RCAR_GEN3_INTCSYS_V3_RTDMAC0_5,   RCAR_GEN3_INTCSYS_V3_RTDMAC0_6,   RCAR_GEN3_INTCSYS_V3_RTDMAC0_7,
            RCAR_GEN3_INTCSYS_V3_RTDMAC0_8,   RCAR_GEN3_INTCSYS_V3_RTDMAC0_9,   RCAR_GEN3_INTCSYS_V3_RTDMAC0_10,  RCAR_GEN3_INTCSYS_V3_RTDMAC0_11,
            RCAR_GEN3_INTCSYS_V3_RTDMAC0_12,  RCAR_GEN3_INTCSYS_V3_RTDMAC0_13,  RCAR_GEN3_INTCSYS_V3_RTDMAC0_14,  RCAR_GEN3_INTCSYS_V3_RTDMAC0_15
        };
        
        uint32_t irq1[] = {
            RCAR_GEN3_INTCSYS_V3_RTDMAC1_0,   RCAR_GEN3_INTCSYS_V3_RTDMAC1_1,   RCAR_GEN3_INTCSYS_V3_RTDMAC1_2,   RCAR_GEN3_INTCSYS_V3_RTDMAC1_3,
            RCAR_GEN3_INTCSYS_V3_RTDMAC1_4,   RCAR_GEN3_INTCSYS_V3_RTDMAC1_5,   RCAR_GEN3_INTCSYS_V3_RTDMAC1_6,   RCAR_GEN3_INTCSYS_V3_RTDMAC1_7,
            RCAR_GEN3_INTCSYS_V3_RTDMAC1_8,   RCAR_GEN3_INTCSYS_V3_RTDMAC1_9,   RCAR_GEN3_INTCSYS_V3_RTDMAC1_10,  RCAR_GEN3_INTCSYS_V3_RTDMAC1_11,
            RCAR_GEN3_INTCSYS_V3_RTDMAC1_12,  RCAR_GEN3_INTCSYS_V3_RTDMAC1_13,  RCAR_GEN3_INTCSYS_V3_RTDMAC1_14,  RCAR_GEN3_INTCSYS_V3_RTDMAC1_15
        };

        uint32_t *irqs[] = {irq0, irq1};

        HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_USB_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_SYSDMAC_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwidev_add_dma(RCAR_HWI_RTDMAC, &attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_REG, RCAR_GEN3_SYSDMAC_REGS);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_SIZE, RCAR_GEN3_SYSDMAC_REGSIZE);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_DESC, RCAR_GEN3_SYSDMAC_DESC);
            for(irq = 0; irq < NUM_ELTS(irq0); irq++) {
                hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
            }
        }
    }

    {   // Thermal
        unsigned i, hwi_off;
        hwi_tag *tag;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_NUM_IRQ(&attr, 3);

        dev_info_t dev[] = {
            { 0 },
            {.base=RCAR_GEN3_THERMAL1_BASE},
            {.base=RCAR_GEN3_THERMAL2_BASE},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_THERMAL, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_THERMAL_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);

            if(dev[i].base) {
                hwitag_set_ivec(hwi_off, 0, RCAR_GEN3_INTCSYS_THERMAL0);
                hwitag_set_ivec(hwi_off, 1, RCAR_GEN3_INTCSYS_THERMAL1);
                hwitag_set_ivec(hwi_off, 2, RCAR_GEN3_INTCSYS_THERMAL2);
            }
        }

        tag = hwi_alloc_tag(HWI_TAG_INFO(hwversion));
        tag->hwversion.name = (_Uint16t)add_string("v3h");
    }

    { // WDT
        unsigned hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        const struct hwi_inputclk clksrc_kick = {.clk = 10000, .div = 1};

        HWIATTR_TIMER_SET_NUM_CLK(&attr, 1);

        HWIATTR_TIMER_SET_LOCATION(&attr, RCAR_GEN3_RWDT_BASE, RCAR_GEN3_WDT_SIZE, 0, hwi_find_as(RCAR_GEN3_RWDT_BASE, 1));
        hwi_off = hwidev_add_timer(RCAR_HWI_WDT, &attr,  HWI_NULL_OFF);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);

        hwi_off = hwidev_add("wdt,options", 0, HWI_NULL_OFF);
        hwitag_add_regname(hwi_off, "enable_width",     16);

        hwitag_add_regname(hwi_off, "enable_mask",      0x080);
        hwitag_add_regname(hwi_off, "enable_condition", 0x080);
        hwitag_add_regname(hwi_off, "enable_offset",    0x4);

        /* use wdtkick cmd param -W to set wdt,regwrite value
         * since value=0x5A5AFF00 but egname.offset is Uint16t */
    }

    { // CPG
        unsigned hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        uint32_t clk = RCAR_GEN3_EXT_CLK_16MHZ, div = 1;
        switch ((in32(RCAR_GEN3_RESET_BASE + RCAR_GEN3_MODEMR) >> 13) & 3) {
            case 0: clk = RCAR_GEN3_EXT_CLK_16MHZ; div = 1; break;
            case 1: clk = RCAR_GEN3_EXT_CLK_20MHZ; div = 1; break;
            case 2: clk = RCAR_GEN3_EXT_CLK_25MHZ; div = 1; break;
            case 3: clk = RCAR_GEN3_EXT_CLK_33MHZ; div = 2; break;
        }
        const struct hwi_inputclk clksrc_kick = {.clk = clk, .div = div};

        HWIATTR_TIMER_SET_NUM_CLK(&attr, 1);

        HWIATTR_TIMER_SET_LOCATION(&attr, RCAR_GEN3_CPG_BASE, RCAR_GEN3_CPG_SIZE, 0, hwi_find_as(RCAR_GEN3_CPG_BASE, 1));
        hwi_off = hwidev_add_timer(RCAR_HWI_CPG, &attr,  HWI_NULL_OFF);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);
    }

    { // VIN (Capture)
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_NUM_IRQ(&attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_VIN0_BASE, .irq=RCAR_GEN3_INTCSYS_VIN0},
            {.base=RCAR_GEN3_VIN1_BASE, .irq=RCAR_GEN3_INTCSYS_VIN1},
            {.base=RCAR_GEN3_VIN2_BASE, .irq=RCAR_GEN3_INTCSYS_VIN2},
            {.base=RCAR_GEN3_VIN3_BASE, .irq=RCAR_GEN3_INTCSYS_VIN3},
            {.base=RCAR_GEN3_VIN4_BASE, .irq=RCAR_GEN3_INTCSYS_VIN4},
            {.base=RCAR_GEN3_VIN5_BASE, .irq=RCAR_GEN3_INTCSYS_VIN5},
            {.base=RCAR_GEN3_VIN6_BASE, .irq=RCAR_GEN3_INTCSYS_VIN6},
            {.base=RCAR_GEN3_VIN7_BASE, .irq=RCAR_GEN3_INTCSYS_VIN7},
            {.base=RCAR_GEN3_VIN8_BASE, .irq=RCAR_GEN3_INTCSYS_VIN8},
            {.base=RCAR_GEN3_VIN9_BASE, .irq=RCAR_GEN3_INTCSYS_VIN9},
            {.base=RCAR_GEN3_VIN10_BASE, .irq=RCAR_GEN3_INTCSYS_VIN10},
            {.base=RCAR_GEN3_VIN11_BASE, .irq=RCAR_GEN3_INTCSYS_VIN11},
            {.base=RCAR_GEN3_VIN12_BASE, .irq=RCAR_GEN3_INTCSYS_VIN12},
            {.base=RCAR_GEN3_VIN13_BASE, .irq=RCAR_GEN3_INTCSYS_VIN13},
            {.base=RCAR_GEN3_VIN14_BASE, .irq=RCAR_GEN3_INTCSYS_VIN14},
            {.base=RCAR_GEN3_VIN15_BASE, .irq=RCAR_GEN3_INTCSYS_VIN15},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_VIN, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_VIN_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    {  // CANFD
        /*_Uint16t name;*/
        /*hwi_tag *tag;*/
        unsigned hwi_off;
        hwiattr_can_t attr = HWIATTR_CAN_T_INITIALIZER;
        HWIATTR_CAN_SET_NUM_IRQ(&attr, 2);
        HWIATTR_CAN_SET_NUM_MEMADDR(&attr, 1);
	    /*unsigned product = in32(RCAR_GEN3_PRODUCT_REGISTER);*/

        /*
        * canfd-rcar configures clock source to use clkp2 which is CANFD on R-Car Gen3
        * We confiugre CANFDCKCR to be 80mhz in rcar_soc_board.h
        *   NOTE: This is the CANFD clock not fCAN, fCAN is half this value. Figure 52A.1
        */
        struct hwi_inputclk can_clk = {.clk = 80000000, .div = 1};
        HWIATTR_SPI_SET_NUM_CLK(&attr, 1);
        HWIATTR_SPI_SET_NUM_IRQ(&attr, 2);

        /* create CANFD */
        HWIATTR_CAN_SET_LOCATION(&attr, RCAR_GEN3_CANFD_BASE, RCAR_GEN3_CANFD_SIZE, 0, hwi_find_as(RCAR_GEN3_CANFD_BASE, 1));
        hwi_off = rcar_hwibus_add_can(HWI_NULL_OFF, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, &can_clk);
        hwitag_set_ivec(hwi_off, 0, RCAR_GEN3_INTCSYS_CANFD_GL);
        hwitag_set_ivec(hwi_off, 1, RCAR_GEN3_INTCSYS_CANFD_CH);
        /*tag = hwi_alloc_tag(HWI_TAG_INFO(hwversion));*/
    }

    { // FLexray
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_NUM_IRQ(&attr, 1);
        dev_info_t dev[] = {
            {.base=RCAR_GEN3_FLEXRAY_BASE, .irq=RCAR_GEN3_INTCSYS_FRAY},   
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_FLEXRAY, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_FLEXRAY_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }
    { // CPG
        unsigned hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        const struct hwi_inputclk clksrc_kick = {.clk = board_ext_clk, .div = 1};

        HWIATTR_TIMER_SET_NUM_CLK(&attr, 1);

        HWIATTR_TIMER_SET_LOCATION(&attr, RCAR_GEN3_CPG_BASE, RCAR_GEN3_CPG_SIZE, 0, hwi_find_as(RCAR_GEN3_CPG_BASE, 1));
        hwi_off = hwidev_add_timer(RCAR_HWI_CPG, &attr,  HWI_NULL_OFF);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);
    }

    { // SYSC
        unsigned hwi_off;
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        hwi_off = hwidev_add(RCAR_HWI_SYSC, hwi_devclass_NONE, HWI_NULL_OFF);
        HWIATTR_SET_LOCATION(&attr, RCAR_GEN3_SYSC_BASE, RCAR_GEN3_SYSC_SIZE, 0, hwi_find_as(RCAR_GEN3_SYSC_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
    }

    { // CMT
        unsigned i, j, hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        dev_info_t dev[] = {
            {.base = RCAR_GEN3_CMT0_BASE, .irq=RCAR_GEN3_CMT0_REG_OFFSET},
            {.base = RCAR_GEN3_CMT1_BASE, .irq=RCAR_GEN3_CMT1_REG_OFFSET},
            {.base = RCAR_GEN3_CMT2_BASE, .irq=RCAR_GEN3_CMT1_REG_OFFSET},
            {.base = RCAR_GEN3_CMT3_BASE, .irq=RCAR_GEN3_CMT1_REG_OFFSET}
        };
        struct hwi_inputclk clksrc_kick[] = {
            {.clk = 32800,             .div = 1}, // RCLK
            {.clk = board_ext_clk / 2, .div = 1}, // CPEX
            {.clk = 32000,             .div = 1}  // PSEU
        };
        uint32_t irq0[] = {
            RCAR_GEN3_INTCSYS_CMT00,  RCAR_GEN3_INTCSYS_CMT01, 0, 0, 0, 0, 0, 0
        };
        uint32_t irq1[] = {
            RCAR_GEN3_INTCSYS_CMT10,  RCAR_GEN3_INTCSYS_CMT11,  RCAR_GEN3_INTCSYS_CMT12,  RCAR_GEN3_INTCSYS_CMT13,
            RCAR_GEN3_INTCSYS_CMT14,  RCAR_GEN3_INTCSYS_CMT15,  RCAR_GEN3_INTCSYS_CMT16,  RCAR_GEN3_INTCSYS_CMT17
        };
        uint32_t irq2[] = {
            RCAR_GEN3_INTCSYS_V3_CMT20,  RCAR_GEN3_INTCSYS_V3_CMT21,  RCAR_GEN3_INTCSYS_V3_CMT22,  RCAR_GEN3_INTCSYS_V3_CMT23,
            RCAR_GEN3_INTCSYS_V3_CMT24,  RCAR_GEN3_INTCSYS_V3_CMT25,  RCAR_GEN3_INTCSYS_V3_CMT26,  RCAR_GEN3_INTCSYS_V3_CMT27
        };
        uint32_t irq3[] = {
            RCAR_GEN3_INTCSYS_V3_CMT30,  RCAR_GEN3_INTCSYS_V3_CMT31,  RCAR_GEN3_INTCSYS_V3_CMT32,  RCAR_GEN3_INTCSYS_V3_CMT33,
            RCAR_GEN3_INTCSYS_V3_CMT34,  RCAR_GEN3_INTCSYS_V3_CMT35,  RCAR_GEN3_INTCSYS_V3_CMT36,  RCAR_GEN3_INTCSYS_V3_CMT37
        };
        uint32_t *irqs[] = { irq0, irq1, irq2, irq3 };
        if (!(in32(RCAR_GEN3_RESET_BASE + RCAR_GEN3_MODEMR) & BIT(29))) {
            clksrc_kick[0].clk = 32768;
        }

        HWIATTR_TIMER_SET_NUM_IRQ(&attr, NUM_ELTS(irq1));
        HWIATTR_TIMER_SET_NUM_CLK(&attr, NUM_ELTS(clksrc_kick));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_TIMER_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_CMT_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwidev_add_timer(RCAR_HWI_CMT, &attr,  HWI_NULL_OFF);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_add_regname(hwi_off, RCAR_HWI_CMT_REG, dev[i].irq);
            for (j = 0; j < NUM_ELTS(clksrc_kick); j++) {
                hwitag_set_inputclk(hwi_off, j, &clksrc_kick[j]);
            }
            for (j = 0; j < NUM_ELTS(irq1); j++) {
                hwitag_set_avail_ivec(hwi_off, j, irqs[i][j]);
            }
        }
    }
}

