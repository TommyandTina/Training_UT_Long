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

static unsigned rcar_hwibus_add_can(unsigned parent_hwi_off, hwiattr_can_t *attr)
{
    unsigned hwi_off = hwibus_add(HWI_ITEM_BUS_CAN, parent_hwi_off);
    if ((hwi_off != HWI_NULL_OFF) && (attr != NULL))
    {
        unsigned i;
        hwitag_add_common(hwi_off, &attr->common);
        for (i=0; i<attr->num_clks; i++)
        {
            hwitag_add_inputclk(hwi_off, 0, 1);
        }
    }
    return hwi_off;
}

void init_hwinfo()
{
    {
        unsigned i, hwi_off;

        hwiattr_uart_t uart_attr = HWIATTR_UART_T_INITIALIZER;
        /* All the UARTs operate from a fixed functional clock of 14.7456 MHz only */
        struct hwi_inputclk uart_clk = {.clk = 14745600, .div = 16};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_SCIF0_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF0},
            {.base=RCAR_GEN3_SCIF1_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF1},
            {.base=RCAR_GEN3_SCIF2_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF2},
            {.base=RCAR_GEN3_SCIF3_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF3},
            {.base=RCAR_GEN3_SCIF4_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF4},
            {.base=RCAR_GEN3_SCIF5_BASE, .irq=RCAR_GEN3_INTCSYS_SCIF5},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_UART_SET_LOCATION(&uart_attr, dev[i].base,  RCAR_GEN3_SCIF_SIZE, 0, hwi_find_as(dev[i].base , 1));
            hwi_off = hwidev_add_uart(RCAR_HWI_UART, &uart_attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &uart_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    {
        unsigned i, hwi_off;

        hwiattr_uart_t uart_attr = HWIATTR_UART_T_INITIALIZER;
        /* All the UARTs operate from a fixed functional clock of 14.7456 MHz only */
        struct hwi_inputclk uart_clk = {.clk = 14745600, .div = 16};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_HSCIF0_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF0},
            {.base=RCAR_GEN3_HSCIF1_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF1},
            {.base=RCAR_GEN3_HSCIF2_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF2},
            {.base=RCAR_GEN3_HSCIF3_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF3},
            {.base=RCAR_GEN3_HSCIF4_BASE, .irq=RCAR_GEN3_INTCSYS_HSCIF4},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_UART_SET_LOCATION(&uart_attr, dev[i].base,  RCAR_GEN3_SCIF_SIZE, 0, hwi_find_as(dev[i].base , 1));
            hwi_off = hwidev_add_uart(RCAR_HWI_HSUART, &uart_attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &uart_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    {
        unsigned i, hwi_off;

        hwiattr_sdio_t sdio_attr = HWIATTR_SDIO_T_INITIALIZER;
        /* RCAR_GEN3_CPG_SD2CKCR = 0x1 SDnH = 800 MHz SDn = 200 Mhz*/
        struct hwi_inputclk sdio_clk = {.clk = 200000000, .div = 1};

        HWIATTR_SDIO_SET_NUM_IRQ(&sdio_attr, 1);
        HWIATTR_SDIO_SET_NUM_CLK(&sdio_attr, 1);
        HWIATTR_SDIO_SET_DLL(&sdio_attr, "rcar_gen3");


        dev_info_t dev[] = {
            {.base=0, .irq=0},
            {.base=0, .irq=0},
            {.base=RCAR_GEN3_SDHI2_BASE, .irq=RCAR_GEN3_INTCSYS_SDHI2},
            {.base=0, .irq=0},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_SDIO_SET_LOCATION(&sdio_attr, dev[i].base, RCAR_GEN3_SDHI_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_sdio(HWI_NULL_OFF, &sdio_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &sdio_clk);
            hwitag_set_avail_ivec(hwi_find_bus(HWI_ITEM_BUS_SDIO, i), -1, dev[i].irq);
        }
    }

    {
        unsigned hwi_off;
        hwiattr_spi_t attr = HWIATTR_SPI_T_INITIALIZER;
        struct hwi_inputclk rpc_clk = {.clk = 320000000, .div = 1};
        HWIATTR_SPI_SET_NUM_CLK(&attr, 1);

        HWIATTR_SPI_SET_LOCATION(&attr, RCAR_GEN3_RPC_BASE, RCAR_GEN3_RPC_SIZE, 0, hwi_find_as(RCAR_GEN3_RPC_BASE, 1));
        hwi_off = hwibus_add_spi(HWI_NULL_OFF, &attr);
        ASSERT(hwi_find_unit(hwi_off) == 0);
        hwitag_set_inputclk(hwi_off, 0, &rpc_clk);

        hwitag_add_regname(hwi_off, RCAR_HWI_RPC_WBUF, RCAR_GEN3_RPC_BUFFER);
        hwitag_add_regname(hwi_off, RCAR_HWI_RPC_WBUF_SIZE, RCAR_GEN3_RPC_BUFFER_SIZE);

        hwi_off = hwidev_add(RCAR_HWI_SPI_DEV, hwi_devclass_NONE, HWI_NULL_OFF);
        hwitag_add_location(hwi_off, RCAR_GEN3_RPC_DEV_ADDR, RCAR_GEN3_RPC_DEV_SIZE, 0, hwi_find_as(RCAR_GEN3_RPC_DEV_ADDR, 1));
    }

    {
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
            {.base=RCAR_GEN3_I2C5_BASE, .irq=RCAR_GEN3_INTCSYS_I2C5},
            {.base=RCAR_GEN3_I2C6_BASE, .irq=RCAR_GEN3_INTCSYS_I2C6},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_I2C_SET_LOCATION(&i2c_attr, dev[i].base, RCAR_GEN3_I2C_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_i2c(HWI_NULL_OFF, &i2c_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &i2c_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    {
        unsigned i, irq, hwi_off;
        hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_SYSDMAC0_BASE},
            {.base=RCAR_GEN3_SYSDMAC1_BASE},
            {.base=RCAR_GEN3_SYSDMAC2_BASE},
        };

        uint32_t irq0[] = {
            RCAR_GEN3_INTCSYS_SYSDMAC0_0,  RCAR_GEN3_INTCSYS_SYSDMAC0_1,  RCAR_GEN3_INTCSYS_SYSDMAC0_2,  RCAR_GEN3_INTCSYS_SYSDMAC0_3,
            RCAR_GEN3_INTCSYS_SYSDMAC0_4,  RCAR_GEN3_INTCSYS_SYSDMAC0_5,  RCAR_GEN3_INTCSYS_SYSDMAC0_6,  RCAR_GEN3_INTCSYS_SYSDMAC0_7,
            RCAR_GEN3_INTCSYS_SYSDMAC0_8,  RCAR_GEN3_INTCSYS_SYSDMAC0_9,  RCAR_GEN3_INTCSYS_SYSDMAC0_10, RCAR_GEN3_INTCSYS_SYSDMAC0_11,
            RCAR_GEN3_INTCSYS_SYSDMAC0_12, RCAR_GEN3_INTCSYS_SYSDMAC0_13, RCAR_GEN3_INTCSYS_SYSDMAC0_14, RCAR_GEN3_INTCSYS_SYSDMAC0_15
        };
        uint32_t irq1[] = {
            RCAR_GEN3_INTCSYS_SYSDMAC1_0,  RCAR_GEN3_INTCSYS_SYSDMAC1_1,  RCAR_GEN3_INTCSYS_SYSDMAC1_2,  RCAR_GEN3_INTCSYS_SYSDMAC1_3,
            RCAR_GEN3_INTCSYS_SYSDMAC1_4,  RCAR_GEN3_INTCSYS_SYSDMAC1_5,  RCAR_GEN3_INTCSYS_SYSDMAC1_6,  RCAR_GEN3_INTCSYS_SYSDMAC1_7,
            RCAR_GEN3_INTCSYS_SYSDMAC1_8,  RCAR_GEN3_INTCSYS_SYSDMAC1_9,  RCAR_GEN3_INTCSYS_SYSDMAC1_10, RCAR_GEN3_INTCSYS_SYSDMAC1_11,
            RCAR_GEN3_INTCSYS_SYSDMAC1_12, RCAR_GEN3_INTCSYS_SYSDMAC1_13, RCAR_GEN3_INTCSYS_SYSDMAC1_14, RCAR_GEN3_INTCSYS_SYSDMAC1_15
        };
        uint32_t irq2[] = {
            RCAR_GEN3_INTCSYS_SYSDMAC2_0,  RCAR_GEN3_INTCSYS_SYSDMAC2_1,  RCAR_GEN3_INTCSYS_SYSDMAC2_2,  RCAR_GEN3_INTCSYS_SYSDMAC2_3,
            RCAR_GEN3_INTCSYS_SYSDMAC2_4,  RCAR_GEN3_INTCSYS_SYSDMAC2_5,  RCAR_GEN3_INTCSYS_SYSDMAC2_6,  RCAR_GEN3_INTCSYS_SYSDMAC2_7,
            RCAR_GEN3_INTCSYS_SYSDMAC2_8,  RCAR_GEN3_INTCSYS_SYSDMAC2_9,  RCAR_GEN3_INTCSYS_SYSDMAC2_10, RCAR_GEN3_INTCSYS_SYSDMAC2_11,
            RCAR_GEN3_INTCSYS_SYSDMAC2_12, RCAR_GEN3_INTCSYS_SYSDMAC2_13, RCAR_GEN3_INTCSYS_SYSDMAC2_14, RCAR_GEN3_INTCSYS_SYSDMAC2_15
        };

        uint32_t *irqs[] = { irq0, irq1, irq2 };

        HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_USB_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_SYSDMAC_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwidev_add_dma(RCAR_HWI_SYSDMAC, &attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_REG, RCAR_GEN3_SYSDMAC_REGS);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_SIZE, RCAR_GEN3_SYSDMAC_REGSIZE);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_DESC, RCAR_GEN3_SYSDMAC_DESC);
            for(irq = 0; irq < NUM_ELTS(irq0); irq++) {
                hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
            }
        }
    }

    {
        unsigned i, irq, hwi_off;
        hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_AUDIODMAC0_BASE},
        };

        uint32_t irq0[] = {
            RCAR_GEN3_INTCSYS_AUDIODMAC0_0,  RCAR_GEN3_INTCSYS_AUDIODMAC0_1,  RCAR_GEN3_INTCSYS_AUDIODMAC0_2,  RCAR_GEN3_INTCSYS_AUDIODMAC0_3,
            RCAR_GEN3_INTCSYS_AUDIODMAC0_4,  RCAR_GEN3_INTCSYS_AUDIODMAC0_5,  RCAR_GEN3_INTCSYS_AUDIODMAC0_6,  RCAR_GEN3_INTCSYS_AUDIODMAC0_7,
            RCAR_GEN3_INTCSYS_AUDIODMAC0_8,  RCAR_GEN3_INTCSYS_AUDIODMAC0_9,  RCAR_GEN3_INTCSYS_AUDIODMAC0_10, RCAR_GEN3_INTCSYS_AUDIODMAC0_11,
            RCAR_GEN3_INTCSYS_AUDIODMAC0_12, RCAR_GEN3_INTCSYS_AUDIODMAC0_13, RCAR_GEN3_INTCSYS_AUDIODMAC0_14, RCAR_GEN3_INTCSYS_AUDIODMAC0_15
        };

        uint32_t *irqs[] = { irq0 };

        HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_USB_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_SYSDMAC_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwidev_add_dma(RCAR_HWI_AUDIODMAC, &attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_REG, RCAR_GEN3_SYSDMAC_REGS);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_SIZE, RCAR_GEN3_SYSDMAC_REGSIZE);
            hwitag_add_regname(hwi_off, RCAR_HWI_DMA_DESC, RCAR_GEN3_SYSDMAC_DESC);
            for(irq = 0; irq < NUM_ELTS(irq0); irq++) {
                hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
            }
        }
    }

    {
        unsigned hwi_off;
        hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

        HWIATTR_USB_SET_LOCATION(&attr, RCAR_GEN3_AUDIODMACPP_BASE, RCAR_GEN3_AUDIODMACPP_SIZE, 0, hwi_find_as(RCAR_GEN3_AUDIODMACPP_BASE, 1));
        hwi_off = hwidev_add_dma(RCAR_HWI_AUDIOPPDMAC, &attr, HWI_NULL_OFF);
        ASSERT(hwi_find_unit(hwi_off) == 0);
    }

    {
        unsigned hwi_off = hwidev_add(RCAR_HWI_AUDIO_ADG, hwi_devclass_NONE, HWI_NULL_OFF);
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_LOCATION(&attr, RCAR_GEN3_ADG_BASE, RCAR_GEN3_ADG_SIZE, 0, hwi_find_as(RCAR_GEN3_ADG_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
    }

    {
        unsigned hwi_off = hwidev_add(RCAR_HWI_AUDIO_SSI, hwi_devclass_NONE, HWI_NULL_OFF);
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_LOCATION(&attr, RCAR_GEN3_SSI_BASE, RCAR_GEN3_SSI_SIZE, 0, hwi_find_as(RCAR_GEN3_SSI_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
    }

    {
        unsigned hwi_off = hwidev_add(RCAR_HWI_AUDIO_SSIU, hwi_devclass_NONE, HWI_NULL_OFF);
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_LOCATION(&attr, RCAR_GEN3_SSIU_BASE, RCAR_GEN3_SSIU_SIZE, 0, hwi_find_as(RCAR_GEN3_SSIU_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
    }

    {
        unsigned hwi_off = hwidev_add(RCAR_HWI_AUDIO_SCU, hwi_devclass_NONE, HWI_NULL_OFF);
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_LOCATION(&attr, RCAR_GEN3_SCU_BASE, RCAR_GEN3_SCU_SIZE, 0, hwi_find_as(RCAR_GEN3_SCU_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
    }

    {
        unsigned i, hwi_off;
        hwi_tag *tag;
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        dev_info_t dev[] = {
            {.base=RCAR_GEN3_THERMAL0_BASE, .irq=RCAR_GEN3_INTCSYS_THERMAL0},
        };

        HWIATTR_SET_NUM_IRQ(&attr, 1);

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_THERMAL, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_THERMAL_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);

            if(dev[i].base) {
                hwitag_set_ivec(hwi_off, 0, RCAR_GEN3_INTCSYS_THERMAL0);
            }

            tag = hwi_alloc_tag(HWI_TAG_INFO(hwversion));
            tag->hwversion.name = (_Uint16t)add_string("d3");
        }
    }

    {
        unsigned i, hwi_off;
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        dev_info_t dev[] = {
            {.base = 0, .irq = 0},
            {.base = 0, .irq = 0},
            {.base = 0, .irq = 0},
            {.base = 0, .irq = 0},
            {.base=RCAR_GEN3_VIN4_BASE, .irq=RCAR_GEN3_INTCSYS_VIN4},
        };

        HWIATTR_SET_NUM_IRQ(&attr, 1);

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_VIN, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_VIN_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    {
        unsigned i, hwi_off;
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        dev_info_t dev[] = {
            {.base=RCAR_GEN3_CSI40_BASE},
            {.base=RCAR_GEN3_CSI20_BASE},
            {.base=RCAR_GEN3_CSI41_BASE},
            {.base=RCAR_GEN3_CSI21_BASE},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_CAMERA, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_CSI2_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
        }
    }

    {
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

#define USE_CANFD 
#if defined USE_CANFD
    {  // CANFD
        unsigned hwi_off;
        hwiattr_can_t attr = HWIATTR_CAN_T_INITIALIZER;
        hwi_tag *tag;
        /*
        * We configure CANFDCKCR to be 40mhz in rcar_soc_board.h
        * NOTE: This is the CANFD clock not fCAN, fCAN is half this value. Figure 52A.1
        */
        struct hwi_inputclk can_clk = {.clk = 40000000, .div = 1};
        HWIATTR_CAN_SET_NUM_CLK(&attr, 1);
        HWIATTR_CAN_SET_NUM_IRQ(&attr, 2);

        /* CANFD */
        HWIATTR_CAN_SET_LOCATION(&attr, RCAR_GEN3_CANFD_BASE, RCAR_GEN3_CANFD_SIZE, 0, hwi_find_as(RCAR_GEN3_CANFD_BASE, 1));
        hwi_off = rcar_hwibus_add_can(HWI_NULL_OFF, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, &can_clk);
        hwitag_set_ivec(hwi_off, 0, RCAR_GEN3_INTCSYS_CANFD_GL);
        hwitag_set_ivec(hwi_off, 1, RCAR_GEN3_INTCSYS_CANFD_CH);
        tag = hwi_alloc_tag(HWI_TAG_INFO(hwversion));
        tag->hwversion.name = (_Uint16t)add_string("d3");
    }
#else
    { // CANBUS
        unsigned hwi_off;
        hwiattr_can_t attr = HWIATTR_CAN_T_INITIALIZER;

        /*
         * dev-can-rcar configures clock source to use clkp2 which is CANFD on R-Car Gen3
         * We configure CANFDCKCR to be 80mhz in rcar_soc_board.h
         */
        struct hwi_inputclk can_clk = {.clk = 40000000, .div = 1};
        HWIATTR_CAN_SET_NUM_CLK(&attr, 1);
        HWIATTR_CAN_SET_NUM_IRQ(&attr, 1);

        /* CAN0 */
        HWIATTR_CAN_SET_LOCATION(&attr, RCAR_GEN3_CAN0_BASE + RCAR_GEN3_CAN_REG_OFFSET, RCAR_GEN3_CAN_SIZE, 0, hwi_find_as(RCAR_GEN3_CAN0_BASE + RCAR_GEN3_CAN_REG_OFFSET, 1));
        hwi_off = rcar_hwibus_add_can(HWI_NULL_OFF, &attr);
        hwitag_add_location(hwi_off, RCAR_GEN3_CAN0_BASE, RCAR_GEN3_CAN_REG_OFFSET, 0, 0);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, &can_clk);
        hwitag_set_ivec(hwi_off, 0, RCAR_GEN3_INTCSYS_CAN0);

        /* CAN1 */
        HWIATTR_CAN_SET_LOCATION(&attr, RCAR_GEN3_CAN1_BASE + RCAR_GEN3_CAN_REG_OFFSET, RCAR_GEN3_CAN_SIZE, 0, hwi_find_as(RCAR_GEN3_CAN1_BASE + RCAR_GEN3_CAN_REG_OFFSET, 1));
        hwi_off = rcar_hwibus_add_can(HWI_NULL_OFF, &attr);
        hwitag_add_location(hwi_off, RCAR_GEN3_CAN1_BASE, RCAR_GEN3_CAN_REG_OFFSET, 0, 0);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, &can_clk);
        hwitag_set_ivec(hwi_off, 0, RCAR_GEN3_INTCSYS_CAN1);
    }
#endif

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
            {.clk = board_ext_clk / 4, .div = 1}, // CPEX
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
            RCAR_GEN3_INTCSYS_CMT20,  RCAR_GEN3_INTCSYS_CMT21,  RCAR_GEN3_INTCSYS_CMT22,  RCAR_GEN3_INTCSYS_CMT23,
            RCAR_GEN3_INTCSYS_CMT24,  RCAR_GEN3_INTCSYS_CMT25,  RCAR_GEN3_INTCSYS_CMT26,  RCAR_GEN3_INTCSYS_CMT27
        };
        uint32_t irq3[] = {
            RCAR_GEN3_INTCSYS_CMT30,  RCAR_GEN3_INTCSYS_CMT31,  RCAR_GEN3_INTCSYS_CMT32,  RCAR_GEN3_INTCSYS_CMT33,
            RCAR_GEN3_INTCSYS_CMT34,  RCAR_GEN3_INTCSYS_CMT35,  RCAR_GEN3_INTCSYS_CMT36,  RCAR_GEN3_INTCSYS_CMT37
        };
        uint32_t *irqs[] = { irq0, irq1, irq2, irq3 };

        if (!(in32(RCAR_GEN3_CPG_BASE + RCAR_GEN3_CPG_RCKCR) & BIT(15))) {
            clksrc_kick[0].clk = 31250;
        }

        HWIATTR_TIMER_SET_NUM_IRQ(&attr, NUM_ELTS(irq1));
        HWIATTR_TIMER_SET_NUM_CLK(&attr, NUM_ELTS(clksrc_kick));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_TIMER_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_CMT_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwidev_add_timer(RCAR_HWI_CMT, &attr,  HWI_NULL_OFF);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_add_regname(hwi_off, RCAR_HWI_CMT_REG, dev[i].irq);
            for (j = 0; j < NUM_ELTS(irq1); j++) {
                hwitag_set_ivec(hwi_off, j, irqs[i][j]);
            }
            for (j = 0; j < NUM_ELTS(clksrc_kick); j++) {
                hwitag_set_inputclk(hwi_off, j, &clksrc_kick[j]);
            }
        }
    }
}
