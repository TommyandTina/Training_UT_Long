/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2020, Renesas Electronics Corporation.
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
        struct hwi_inputclk uart_clk = {.clk = RCAR_V3U_CLK_S1D8, .div = 16};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_SCIF0_BASE, .irq=RCAR_V3U_INTCSYS_SCIF0},
            {.base=RCAR_GEN3_SCIF1_BASE, .irq=RCAR_V3U_INTCSYS_SCIF1},
            {.base=RCAR_GEN3_SCIF3_BASE, .irq=RCAR_V3U_INTCSYS_SCIF3},
            {.base=RCAR_GEN3_SCIF4_BASE, .irq=RCAR_V3U_INTCSYS_SCIF4},
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
        struct hwi_inputclk uart_clk = {.clk = RCAR_V3U_CLK_S1D2, .div = 16};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_HSCIF0_BASE, .irq=RCAR_V3U_INTCSYS_HSCIF0},
            {.base=RCAR_GEN3_HSCIF1_BASE, .irq=RCAR_V3U_INTCSYS_HSCIF1},
            {.base=RCAR_GEN3_HSCIF2_BASE, .irq=RCAR_V3U_INTCSYS_HSCIF2},
            {.base=RCAR_GEN3_HSCIF3_BASE, .irq=RCAR_V3U_INTCSYS_HSCIF3},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_UART_SET_LOCATION(&uart_attr, dev[i].base,  RCAR_GEN3_SCIF_SIZE, 0, hwi_find_as(dev[i].base , 1));
            hwi_off = hwidev_add_uart(RCAR_HWI_HSUART, &uart_attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &uart_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    // SDHI
    {
        unsigned i, hwi_off;

        hwiattr_sdio_t sdio_attr = HWIATTR_SDIO_T_INITIALIZER;
        /* RCAR_GEN3_CPG_SD0CKCR = 0x4 SDnH = 800 MHz SDn = 200 Mhz*/
        struct hwi_inputclk sdio_clk = {.clk = RCAR_V3U_CLK_SD0, .div = 1};

        HWIATTR_SDIO_SET_NUM_IRQ(&sdio_attr, 1);
        HWIATTR_SDIO_SET_NUM_CLK(&sdio_attr, 1);
        HWIATTR_SDIO_SET_DLL(&sdio_attr, "rcar_gen3");

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_MMCIF0_BASE, .irq=RCAR_V3U_INTCSYS_SDHI0},
        };

        sd_info_t sd[] = {
            {.pwr_base=RCAR_V3U_GPIO_DM0_BASE+RCAR_V3U_GPIO_GP0, .pwr_vdd_offset=RCAR_V3U_GPIO_OUTDT, .pwr_vdd_shift=15, .pfc_base=RCAR_V3U_GPIO_DM0_BASE+RCAR_V3U_GPIO_GP0, .pfc_offset=RCAR_V3U_GPIO_POC, .pfc_mask=0xFFF, .pfc_shift=16},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_SDIO_SET_LOCATION(&sdio_attr, dev[i].base, RCAR_GEN3_SDHI_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_sdio(HWI_NULL_OFF, &sdio_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &sdio_clk);
            hwitag_set_avail_ivec(hwi_find_bus(HWI_ITEM_BUS_SDIO, i), -1, dev[i].irq);

            hwi_off = hwidev_add(RCAR_HWI_SDHI_PWR, hwi_devclass_NONE, HWI_NULL_OFF);
            if(sd[i].pwr_base != 0) {
                hwitag_add_location(hwi_off, sd[i].pwr_base, RCAR_V3U_GPIO_GP_SIZE, 0, hwi_find_as(sd[i].pwr_base, 1));
                hwitag_add_regname(hwi_off, RCAR_HWI_SDHI_PWR_VDD_OFF, sd[i].pwr_vdd_offset);
                hwitag_add_regname(hwi_off, RCAR_HWI_SDHI_PWR_VDD_SHIFT, sd[i].pwr_vdd_shift);
            }

            hwi_off = hwidev_add(RCAR_HWI_SDHI_PFC, hwi_devclass_NONE, HWI_NULL_OFF);
            if(sd[i].pfc_base != 0) {
                hwitag_add_location(hwi_off, sd[i].pfc_base, RCAR_V3U_GPIO_GP_SIZE, 0, hwi_find_as(sd[i].pfc_base, 1));
                hwitag_add_regname(hwi_off, RCAR_HWI_SDHI_PFC_REG, sd[i].pfc_offset);
                hwitag_add_regname(hwi_off, RCAR_HWI_SDHI_PFC_PMMR, RCAR_V3U_GPIO_PMMR);
                hwitag_add_regname(hwi_off, RCAR_HWI_SDHI_PFC_MASK, sd[i].pfc_mask);
                hwitag_add_regname(hwi_off, RCAR_HWI_SDHI_PFC_SHIFT, sd[i].pfc_shift);
            }
        }
    }

    // I2C
    {
        unsigned i, hwi_off;

        hwiattr_i2c_t i2c_attr = HWIATTR_I2C_T_INITIALIZER;
        /* All I2Cs operate on 133.33 MHz only */
        struct hwi_inputclk i2c_clk = {.clk = 133330000, .div = 1};

        /* each I2C has an interrupt */
        HWIATTR_I2C_SET_NUM_IRQ(&i2c_attr, 1);
        HWIATTR_I2C_SET_NUM_CLK(&i2c_attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_I2C0_BASE, .irq=RCAR_V3U_INTCSYS_I2C0},
            {.base=RCAR_GEN3_I2C1_BASE, .irq=RCAR_V3U_INTCSYS_I2C1},
            {.base=RCAR_GEN3_I2C2_BASE, .irq=RCAR_V3U_INTCSYS_I2C2},
            {.base=RCAR_GEN3_I2C3_BASE, .irq=RCAR_V3U_INTCSYS_I2C3},
            {.base=RCAR_GEN3_I2C4_BASE, .irq=RCAR_V3U_INTCSYS_I2C4},
            {.base=RCAR_GEN3_I2C5_BASE, .irq=RCAR_V3U_INTCSYS_I2C5},
            {.base=RCAR_GEN3_I2C6_BASE, .irq=RCAR_V3U_INTCSYS_I2C6},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_I2C_SET_LOCATION(&i2c_attr, dev[i].base, RCAR_GEN3_I2C_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_i2c(HWI_NULL_OFF, &i2c_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &i2c_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    // RPC-SPI
    {
        unsigned hwi_off;
        hwiattr_spi_t attr = HWIATTR_SPI_T_INITIALIZER;
        struct hwi_inputclk rpc_clk = {.clk = RCAR_V3U_CLK_RPCD2, .div = 2};
        HWIATTR_SPI_SET_NUM_CLK(&attr, 1);

        HWIATTR_SPI_SET_LOCATION(&attr, RCAR_GEN3_RPC_BASE, RCAR_GEN3_RPC_SIZE, 0, hwi_find_as(RCAR_GEN3_RPC_BASE, 1));
        hwi_off = hwibus_add_spi(HWI_NULL_OFF, &attr);
        ASSERT(hwi_find_unit(hwi_off) == 0);
        hwitag_set_inputclk(hwi_off, 0, &rpc_clk);

        hwi_off = hwidev_add(RCAR_HWI_SPI_DEV, hwi_devclass_NONE, HWI_NULL_OFF);
        hwitag_add_location(hwi_off, RCAR_GEN3_RPC_DEV_ADDR, RCAR_GEN3_RPC_DEV_SIZE, 0, hwi_find_as(RCAR_GEN3_RPC_DEV_ADDR, 1));
        
    }

    // SYS-DMAC & RT-DMAC
    {
        {
            unsigned i, irq, hwi_off;
            hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

            dev_info2_t dev[] = {
                {.base=RCAR_GEN3_SYSDMAC1_BASE, .base2=RCAR_V3U_SYSDMAC1_COM_BASE},
                {.base=RCAR_GEN3_SYSDMAC2_BASE, .base2=RCAR_V3U_SYSDMAC2_COM_BASE},
            };

            uint32_t irq0[] = {
                RCAR_V3U_INTCSYS_SYSDMAC1_0,  RCAR_V3U_INTCSYS_SYSDMAC1_1,  RCAR_V3U_INTCSYS_SYSDMAC1_2,  RCAR_V3U_INTCSYS_SYSDMAC1_3,
                RCAR_V3U_INTCSYS_SYSDMAC1_4,  RCAR_V3U_INTCSYS_SYSDMAC1_5,  RCAR_V3U_INTCSYS_SYSDMAC1_6,  RCAR_V3U_INTCSYS_SYSDMAC1_7,
                RCAR_V3U_INTCSYS_SYSDMAC1_8,  RCAR_V3U_INTCSYS_SYSDMAC1_9,  RCAR_V3U_INTCSYS_SYSDMAC1_10,  RCAR_V3U_INTCSYS_SYSDMAC1_11,
                RCAR_V3U_INTCSYS_SYSDMAC1_12,  RCAR_V3U_INTCSYS_SYSDMAC1_13,  RCAR_V3U_INTCSYS_SYSDMAC1_14,  RCAR_V3U_INTCSYS_SYSDMAC1_15
            };
            uint32_t irq1[] = {
                RCAR_V3U_INTCSYS_SYSDMAC2_0,  RCAR_V3U_INTCSYS_SYSDMAC2_1,  RCAR_V3U_INTCSYS_SYSDMAC2_2,  RCAR_V3U_INTCSYS_SYSDMAC2_3,
                RCAR_V3U_INTCSYS_SYSDMAC2_4,  RCAR_V3U_INTCSYS_SYSDMAC2_5,  RCAR_V3U_INTCSYS_SYSDMAC2_6,  RCAR_V3U_INTCSYS_SYSDMAC2_7,
            };

            uint32_t *irqs[] = { irq0, irq1 };

            HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

            for(i = 0; i < NUM_ELTS(dev); i++) {
                HWIATTR_USB_SET_LOCATION(&attr, dev[i].base, RCAR_V3U_SYSDMAC_SIZE, 0, hwi_find_as(dev[i].base, 1));
                hwi_off = hwidev_add_dma(RCAR_HWI_SYSDMAC, &attr, HWI_NULL_OFF);
                ASSERT(hwi_find_unit(hwi_off) == i);
                hwitag_add_location(hwi_off, dev[i].base2, RCAR_V3U_SYSDMAC_COM_SIZE, 0, 0);
                hwitag_add_regname(hwi_off, RCAR_HWI_DMA_DESC, RCAR_V3U_SYSDMAC_DESC);
                hwitag_add_regname(hwi_off, RCAR_HWI_DMA_SIZE, RCAR_V3U_SYSDMAC_CHREG_SIZE);
                
                for(irq = 0; irq < NUM_ELTS(irq0); irq++) {
                    hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
                }
            }
        }
        {
            unsigned i, irq, hwi_off;
            hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

            dev_info2_t dev[] = {
                {.base=RCAR_V3U_RTDMAC0_BASE, .base2=RCAR_V3U_RTDMAC0_COM_BASE},
                {.base=RCAR_V3U_RTDMAC1_BASE, .base2=RCAR_V3U_RTDMAC1_COM_BASE},
                {.base=RCAR_V3U_RTDMAC2_BASE, .base2=RCAR_V3U_RTDMAC2_COM_BASE},
                {.base=RCAR_V3U_RTDMAC3_BASE, .base2=RCAR_V3U_RTDMAC3_COM_BASE},
            };

            uint32_t irq0[] = {
                RCAR_V3U_INTCSYS_RTDMAC0_0,  RCAR_V3U_INTCSYS_RTDMAC0_1,  RCAR_V3U_INTCSYS_RTDMAC0_2,   RCAR_V3U_INTCSYS_RTDMAC0_3,
                RCAR_V3U_INTCSYS_RTDMAC0_4,  RCAR_V3U_INTCSYS_RTDMAC0_5,  RCAR_V3U_INTCSYS_RTDMAC0_6,   RCAR_V3U_INTCSYS_RTDMAC0_7,
                RCAR_V3U_INTCSYS_RTDMAC0_8,  RCAR_V3U_INTCSYS_RTDMAC0_9,  RCAR_V3U_INTCSYS_RTDMAC0_10,  RCAR_V3U_INTCSYS_RTDMAC0_11,
                RCAR_V3U_INTCSYS_RTDMAC0_12, RCAR_V3U_INTCSYS_RTDMAC0_13, RCAR_V3U_INTCSYS_RTDMAC0_14,  RCAR_V3U_INTCSYS_RTDMAC0_15
            };
            uint32_t irq1[] = {
                RCAR_V3U_INTCSYS_RTDMAC1_0,  RCAR_V3U_INTCSYS_RTDMAC1_1,  RCAR_V3U_INTCSYS_RTDMAC1_2,   RCAR_V3U_INTCSYS_RTDMAC1_3,
                RCAR_V3U_INTCSYS_RTDMAC1_4,  RCAR_V3U_INTCSYS_RTDMAC1_5,  RCAR_V3U_INTCSYS_RTDMAC1_6,   RCAR_V3U_INTCSYS_RTDMAC1_7,
                RCAR_V3U_INTCSYS_RTDMAC1_8,  RCAR_V3U_INTCSYS_RTDMAC1_9,  RCAR_V3U_INTCSYS_RTDMAC1_10,  RCAR_V3U_INTCSYS_RTDMAC1_11,
                RCAR_V3U_INTCSYS_RTDMAC1_12, RCAR_V3U_INTCSYS_RTDMAC1_13, RCAR_V3U_INTCSYS_RTDMAC1_14,  RCAR_V3U_INTCSYS_RTDMAC1_15
            };
            uint32_t irq2[] = {
                RCAR_V3U_INTCSYS_RTDMAC2_0,  RCAR_V3U_INTCSYS_RTDMAC2_1,  RCAR_V3U_INTCSYS_RTDMAC2_2,   RCAR_V3U_INTCSYS_RTDMAC2_3,
                RCAR_V3U_INTCSYS_RTDMAC2_4,  RCAR_V3U_INTCSYS_RTDMAC2_5,  RCAR_V3U_INTCSYS_RTDMAC2_6,   RCAR_V3U_INTCSYS_RTDMAC2_7,
                RCAR_V3U_INTCSYS_RTDMAC2_8,  RCAR_V3U_INTCSYS_RTDMAC2_9,  RCAR_V3U_INTCSYS_RTDMAC2_10,  RCAR_V3U_INTCSYS_RTDMAC2_11,
                RCAR_V3U_INTCSYS_RTDMAC2_12, RCAR_V3U_INTCSYS_RTDMAC2_13, RCAR_V3U_INTCSYS_RTDMAC2_14,  RCAR_V3U_INTCSYS_RTDMAC2_15
            };
            uint32_t irq3[] = {
                RCAR_V3U_INTCSYS_RTDMAC3_0,  RCAR_V3U_INTCSYS_RTDMAC3_1,  RCAR_V3U_INTCSYS_RTDMAC3_2,   RCAR_V3U_INTCSYS_RTDMAC3_3,
                RCAR_V3U_INTCSYS_RTDMAC3_4,  RCAR_V3U_INTCSYS_RTDMAC3_5,  RCAR_V3U_INTCSYS_RTDMAC3_6,   RCAR_V3U_INTCSYS_RTDMAC3_7,
                RCAR_V3U_INTCSYS_RTDMAC3_8,  RCAR_V3U_INTCSYS_RTDMAC3_9,  RCAR_V3U_INTCSYS_RTDMAC3_10,  RCAR_V3U_INTCSYS_RTDMAC3_11,
                RCAR_V3U_INTCSYS_RTDMAC3_12, RCAR_V3U_INTCSYS_RTDMAC3_13, RCAR_V3U_INTCSYS_RTDMAC3_14,  RCAR_V3U_INTCSYS_RTDMAC3_15
            };

            uint32_t *irqs[] = {irq0, irq1, irq2, irq3};

            HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

            for(i = 0; i < NUM_ELTS(dev); i++) {
                HWIATTR_USB_SET_LOCATION(&attr, dev[i].base, RCAR_V3U_SYSDMAC_SIZE, 0, hwi_find_as(dev[i].base, 1));
                hwi_off = hwidev_add_dma(RCAR_HWI_RTDMAC, &attr, HWI_NULL_OFF);
                ASSERT(hwi_find_unit(hwi_off) == i);
                hwitag_add_location(hwi_off, dev[i].base2, RCAR_V3U_SYSDMAC_COM_SIZE, 0, 0);
                hwitag_add_regname(hwi_off, RCAR_HWI_DMA_DESC, RCAR_V3U_SYSDMAC_DESC);
                for(irq = 0; irq < NUM_ELTS(irq0); irq++) {
                    hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
                }
            }
        }

        {
            unsigned hwi_off;
            hwi_off = hwidev_add("dma,options", 0, HWI_NULL_OFF);
            hwitag_add_regname(hwi_off, "sysdmac_chans",         24);
            hwitag_add_regname(hwi_off, "sysdmac_chans_per_grp", 16);
            hwitag_add_regname(hwi_off, "rtdmac_chans",          64);
            hwitag_add_regname(hwi_off, "rtdmac_chan_per_grp",   16);
        }
    }

    // CMT
    {
        unsigned i, irq, hwi_off;
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        dev_info2_t dev[] = {
            {.base=RCAR_GEN3_CMT0_BASE, .base2=RCAR_GEN3_CMT0_REG_OFFSET },
            {.base=RCAR_GEN3_CMT1_BASE, .base2=RCAR_GEN3_CMT1_REG_OFFSET },
            {.base=RCAR_GEN3_CMT2_BASE, .base2=RCAR_GEN3_CMT1_REG_OFFSET },
            {.base=RCAR_GEN3_CMT3_BASE, .base2=RCAR_GEN3_CMT1_REG_OFFSET },
        };

        uint32_t irq0[] = {
            RCAR_V3U_INTCSYS_CMT00,  RCAR_V3U_INTCSYS_CMT01, 0, 0, 0, 0, 0, 0
        };
        uint32_t irq1[] = {
            RCAR_V3U_INTCSYS_CMT10,  RCAR_V3U_INTCSYS_CMT11,  RCAR_V3U_INTCSYS_CMT12,  RCAR_V3U_INTCSYS_CMT13,
            RCAR_V3U_INTCSYS_CMT14,  RCAR_V3U_INTCSYS_CMT15,  RCAR_V3U_INTCSYS_CMT16,  RCAR_V3U_INTCSYS_CMT17,
        };
        uint32_t irq2[] = {
            RCAR_V3U_INTCSYS_CMT20,  RCAR_V3U_INTCSYS_CMT21,  RCAR_V3U_INTCSYS_CMT22,  RCAR_V3U_INTCSYS_CMT23,
            RCAR_V3U_INTCSYS_CMT24,  RCAR_V3U_INTCSYS_CMT25,  RCAR_V3U_INTCSYS_CMT26,  RCAR_V3U_INTCSYS_CMT27,
        };
        uint32_t irq3[] = {
            RCAR_V3U_INTCSYS_CMT30,  RCAR_V3U_INTCSYS_CMT31,  RCAR_V3U_INTCSYS_CMT32,  RCAR_V3U_INTCSYS_CMT33,
            RCAR_V3U_INTCSYS_CMT34,  RCAR_V3U_INTCSYS_CMT35,  RCAR_V3U_INTCSYS_CMT36,  RCAR_V3U_INTCSYS_CMT37,
        };

        uint32_t *irqs[] = { irq0, irq1, irq2, irq3 };

        HWIATTR_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_CMT, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_CMT_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_add_regname(hwi_off, RCAR_HWI_CMT_REG, dev[i].base2);
            for(irq = 0; irq < NUM_ELTS(irq0); irq++) {
                hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
            }
        }
    }

    // SPI-MSIOF
    {
        unsigned i, hwi_off;

        hwiattr_spi_t attr = HWIATTR_SPI_T_INITIALIZER;
        /* All the UARTs operate from a fixed functional clock of 26.000.000 MHz only */
        struct hwi_inputclk msiof_clk = {.clk = 26000000, .div = 1};

        /* each UART has an interrupt */
        HWIATTR_SPI_SET_NUM_IRQ(&attr, 1);
        HWIATTR_SPI_SET_NUM_CLK(&attr, 1);

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_MSIOF0_BASE, .irq=RCAR_V3U_INTCSYS_MSIOF0},
            {.base=RCAR_GEN3_MSIOF1_BASE, .irq=RCAR_V3U_INTCSYS_MSIOF1},
            {.base=RCAR_GEN3_MSIOF2_BASE, .irq=RCAR_V3U_INTCSYS_MSIOF2},
            {.base=RCAR_GEN3_MSIOF3_BASE, .irq=RCAR_V3U_INTCSYS_MSIOF3},
            {.base=RCAR_GEN3_MSIOF4_BASE, .irq=RCAR_V3U_INTCSYS_MSIOF4},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_SPI_SET_LOCATION(&attr, dev[i].base,  RCAR_GEN3_MSIOF_SIZE, 0, hwi_find_as(dev[i].base , 1));
            hwi_off = hwibus_add(RCAR_HWI_MSIOF, HWI_NULL_OFF);
            hwitag_add_common(hwi_off, &attr.common);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &msiof_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
    }

    // VIN
    {
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_NUM_IRQ(&attr, 1);

        dev_info_t dev[] = {
            { .base=RCAR_GEN3_VIN0_BASE,   .irq=RCAR_V3U_INTCSYS_VIN0  },
            { .base=RCAR_GEN3_VIN1_BASE,   .irq=RCAR_V3U_INTCSYS_VIN1  },
            { .base=RCAR_GEN3_VIN2_BASE,   .irq=RCAR_V3U_INTCSYS_VIN2  },
            { .base=RCAR_GEN3_VIN3_BASE,   .irq=RCAR_V3U_INTCSYS_VIN3  },
            { .base=RCAR_GEN3_VIN4_BASE,   .irq=RCAR_V3U_INTCSYS_VIN4  },
            { .base=RCAR_GEN3_VIN5_BASE,   .irq=RCAR_V3U_INTCSYS_VIN5  },
            { .base=RCAR_GEN3_VIN6_BASE,   .irq=RCAR_V3U_INTCSYS_VIN6  },
            { .base=RCAR_GEN3_VIN7_BASE,   .irq=RCAR_V3U_INTCSYS_VIN7  },
            { .base=RCAR_GEN3_VIN8_BASE,   .irq=RCAR_V3U_INTCSYS_VIN8  },
            { .base=RCAR_GEN3_VIN9_BASE,   .irq=RCAR_V3U_INTCSYS_VIN9  },
            { .base=RCAR_GEN3_VIN10_BASE,  .irq=RCAR_V3U_INTCSYS_VIN10 },
            { .base=RCAR_GEN3_VIN11_BASE,  .irq=RCAR_V3U_INTCSYS_VIN11 },
            { .base=RCAR_GEN3_VIN12_BASE,  .irq=RCAR_V3U_INTCSYS_VIN12 },
            { .base=RCAR_GEN3_VIN13_BASE,  .irq=RCAR_V3U_INTCSYS_VIN13 },
            { .base=RCAR_GEN3_VIN14_BASE,  .irq=RCAR_V3U_INTCSYS_VIN14 },
            { .base=RCAR_GEN3_VIN15_BASE,  .irq=RCAR_V3U_INTCSYS_VIN15 },
            { .base=RCAR_GEN3_VIN16_BASE,  .irq=RCAR_V3U_INTCSYS_VIN16 },
            { .base=RCAR_GEN3_VIN17_BASE,  .irq=RCAR_V3U_INTCSYS_VIN17 },
            { .base=RCAR_GEN3_VIN18_BASE,  .irq=RCAR_V3U_INTCSYS_VIN18 },
            { .base=RCAR_GEN3_VIN19_BASE,  .irq=RCAR_V3U_INTCSYS_VIN19 },
            { .base=RCAR_GEN3_VIN20_BASE,  .irq=RCAR_V3U_INTCSYS_VIN20 },
            { .base=RCAR_GEN3_VIN21_BASE,  .irq=RCAR_V3U_INTCSYS_VIN21 },
            { .base=RCAR_GEN3_VIN22_BASE,  .irq=RCAR_V3U_INTCSYS_VIN22 },
            { .base=RCAR_GEN3_VIN23_BASE,  .irq=RCAR_V3U_INTCSYS_VIN23 },
            { .base=RCAR_GEN3_VIN24_BASE,  .irq=RCAR_V3U_INTCSYS_VIN24 },
            { .base=RCAR_GEN3_VIN25_BASE,  .irq=RCAR_V3U_INTCSYS_VIN25 },
            { .base=RCAR_GEN3_VIN26_BASE,  .irq=RCAR_V3U_INTCSYS_VIN26 },
            { .base=RCAR_GEN3_VIN27_BASE,  .irq=RCAR_V3U_INTCSYS_VIN27 },
            { .base=RCAR_GEN3_VIN28_BASE,  .irq=RCAR_V3U_INTCSYS_VIN28 },
            { .base=RCAR_GEN3_VIN29_BASE,  .irq=RCAR_V3U_INTCSYS_VIN29 },
            { .base=RCAR_GEN3_VIN30_BASE,  .irq=RCAR_V3U_INTCSYS_VIN30 },
            { .base=RCAR_GEN3_VIN31_BASE,  .irq=RCAR_V3U_INTCSYS_VIN31 },
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_VIN, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_VIN_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
        }
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

    // ISP
    {
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        dev_info_t dev[] = {
            {.base=RCAR_GEN3_ISP0_BASE},
            {.base=RCAR_GEN3_ISP1_BASE},
            {.base=RCAR_GEN3_ISP2_BASE},
            {.base=RCAR_GEN3_ISP3_BASE},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_ISP, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN3_ISP_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
        }
    }

    //GPIO
    {
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        dev_info_t dev[] = {
            {.base=RCAR_V3U_GPIO_DM0_BASE},
            {.base=RCAR_V3U_GPIO_DM1_BASE},
            {.base=RCAR_V3U_GPIO_DM2_BASE},
            {.base=RCAR_V3U_GPIO_DM3_BASE},
            
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_GPIO, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_V3U_GPIO_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
        }
    }

    // WDT
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

    {  // CANFD
        hwi_tag *tag;
        unsigned hwi_off;
        hwiattr_can_t attr = HWIATTR_CAN_T_INITIALIZER;
        HWIATTR_CAN_SET_NUM_IRQ(&attr, 2);
        HWIATTR_CAN_SET_NUM_MEMADDR(&attr, 1);
        /*
        * canfd-rcar configures clock source to use clkp2 which is CANFD on R-Car Gen3
        * We confiugre CANFDCKCR to be 80mhz in rcar_soc_board.h
        *   NOTE: This is the CANFD clock not fCAN, fCAN is half this value. Figure 52A.1
        */
        struct hwi_inputclk can_clk = {.clk = 80000000, .div = 1};
        HWIATTR_SPI_SET_NUM_CLK(&attr, 1);

        /* create CANFD */
        HWIATTR_CAN_SET_LOCATION(&attr, RCAR_V3U_CANFD_BASE, RCAR_GEN3_CANFD_SIZE, 0, hwi_find_as(RCAR_V3U_CANFD_BASE, 1));
        hwi_off = rcar_hwibus_add_can(HWI_NULL_OFF, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, &can_clk);
        hwitag_set_ivec(hwi_off, 0, RCAR_V3U_INTCSYS_CANFD_IRQ_GL);
        hwitag_set_ivec(hwi_off, 1, RCAR_V3U_INTCSYS_CANFD_IRQ_CH);
        tag = hwi_alloc_tag(HWI_TAG_INFO(hwversion));
        tag->hwversion.name = (_Uint16t)add_string("v3u");
    }
    { // FLexray
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_NUM_IRQ(&attr, 1);
        dev_info_t dev[] = {
            {.base=RCAR_GEN3_FLEXRAY_BASE, .irq=RCAR_V3U_INTCSYS_FRAY},   
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

        HWIATTR_TIMER_SET_LOCATION(&attr, RCAR_V3U_CPG_DM0_BASE, RCAR_V3U_CPG_SIZE, 0, hwi_find_as(RCAR_V3U_CPG_DM0_BASE, 1));
        hwi_off = hwidev_add_timer(RCAR_HWI_CPG, &attr,  HWI_NULL_OFF);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);
    }

    { // SYSC
        unsigned hwi_off;
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        hwi_off = hwidev_add(RCAR_HWI_SYSC, hwi_devclass_NONE, HWI_NULL_OFF);
        HWIATTR_SET_LOCATION(&attr, RCAR_V3U_SYSC_DM0_BASE, RCAR_V3U_SYSC_SIZE, 0, hwi_find_as(RCAR_V3U_SYSC_DM0_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
    }

    { // CMT
        unsigned i, j, hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        dev_info_t dev[] = {
            {.base=RCAR_GEN3_CMT0_BASE, .irq=RCAR_GEN3_CMT0_REG_OFFSET},
            {.base=RCAR_GEN3_CMT1_BASE, .irq=RCAR_GEN3_CMT1_REG_OFFSET},
            {.base=RCAR_GEN3_CMT2_BASE, .irq=RCAR_GEN3_CMT1_REG_OFFSET},
            {.base=RCAR_GEN3_CMT3_BASE, .irq=RCAR_GEN3_CMT1_REG_OFFSET}
        };
        struct hwi_inputclk clksrc_kick[] = {
            {.clk = 32800,             .div = 1}, // RCLK
            {.clk = board_ext_clk / 4, .div = 1}, // CPEX
            {.clk = 32000,             .div = 1}  // PSEU
        };
        uint32_t irq0[] = {
            RCAR_V3U_INTCSYS_CMT00,  RCAR_V3U_INTCSYS_CMT01, 0, 0, 0, 0, 0, 0
        };
        uint32_t irq1[] = {
            RCAR_V3U_INTCSYS_CMT10,  RCAR_V3U_INTCSYS_CMT11,  RCAR_V3U_INTCSYS_CMT12,  RCAR_V3U_INTCSYS_CMT13,
            RCAR_V3U_INTCSYS_CMT14,  RCAR_V3U_INTCSYS_CMT15,  RCAR_V3U_INTCSYS_CMT16,  RCAR_V3U_INTCSYS_CMT17
        };
        uint32_t irq2[] = {
            RCAR_V3U_INTCSYS_CMT20,  RCAR_V3U_INTCSYS_CMT21,  RCAR_V3U_INTCSYS_CMT22,  RCAR_V3U_INTCSYS_CMT23,
            RCAR_V3U_INTCSYS_CMT24,  RCAR_V3U_INTCSYS_CMT25,  RCAR_V3U_INTCSYS_CMT26,  RCAR_V3U_INTCSYS_CMT27
        };
        uint32_t irq3[] = {
            RCAR_V3U_INTCSYS_CMT30,  RCAR_V3U_INTCSYS_CMT31,  RCAR_V3U_INTCSYS_CMT32,  RCAR_V3U_INTCSYS_CMT33,
            RCAR_V3U_INTCSYS_CMT34,  RCAR_V3U_INTCSYS_CMT35,  RCAR_V3U_INTCSYS_CMT36,  RCAR_V3U_INTCSYS_CMT37
        };
        uint32_t *irqs[] = { irq0, irq1, irq2, irq3 };

        if (!(in32(RCAR_V3U_RST_DM0_BASE + RCAR_V3U_RST_MODEMR0) & BIT(29))) {
            clksrc_kick[0].clk = 32768; // EXTALR pin
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
