/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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
#include <hw/rcar_cpg.h>
#include "rcar_startup.h"
#include <aarch64/r-car.h>

#define CPG_MSTP_GEN4_I2C(n)            (n == 0 ? CPG_MSTP_GEN4_I2C0 : \
                                        (n == 1 ? CPG_MSTP_GEN4_I2C1 : \
                                        (n == 2 ? CPG_MSTP_GEN4_I2C2 : \
                                        (n == 3 ? CPG_MSTP_GEN4_I2C3 : \
                                        (n == 4 ? CPG_MSTP_GEN4_I2C4 : CPG_MSTP_GEN4_I2C5)))))


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

void init_hwinfo()
{
    { // SCIF
        unsigned i, hwi_off;

        hwiattr_uart_t uart_attr = HWIATTR_UART_T_INITIALIZER;
        struct hwi_inputclk uart_clk = {.clk = RCAR_GEN4_CPG_CLK_SASYNCPERD4, .div = 16};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_DMA(&uart_attr, 2);

        dev_info_t dev[] = {
            {.base=RCAR_SCIF0_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_251},
            {.base=RCAR_SCIF1_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_252},
            {.base=0, .irq=0},
            {.base=RCAR_SCIF3_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_254},
            {.base=RCAR_SCIF4_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_255},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_UART_SET_LOCATION(&uart_attr, dev[i].base,  RCAR_SCIF_SIZE, 0, hwi_find_as(dev[i].base , 1));
            hwi_off = hwidev_add_uart(RCAR_HWI_UART, &uart_attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &uart_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
            hwitag_set_dma(hwi_off, 0, RCAR_DREQ_SCIF_TXI(i));
            hwitag_set_dma(hwi_off, 1, RCAR_DREQ_SCIF_RXI(i));
        }
    }

    { // HSCIF
        unsigned i, hwi_off;

        hwiattr_uart_t uart_attr = HWIATTR_UART_T_INITIALIZER;
        struct hwi_inputclk uart_clk = {.clk = RCAR_GEN4_CPG_CLK_SASYNCPERD1, .div = 16};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_DMA(&uart_attr, 2);

        dev_info_t dev[] = {
            {.base=RCAR_HSCIF0_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_246},
            {.base=RCAR_HSCIF1_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_247},
            {.base=RCAR_HSCIF2_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_248},
            {.base=RCAR_HSCIF3_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_249},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_UART_SET_LOCATION(&uart_attr, dev[i].base,  RCAR_HSCIF_SIZE, 0, hwi_find_as(dev[i].base , 1));
            hwi_off = hwidev_add_uart(RCAR_HWI_HSUART, &uart_attr, HWI_NULL_OFF);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &uart_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
            hwitag_set_dma(hwi_off, 0, RCAR_DREQ_HSCIF_TXI(i));
            hwitag_set_dma(hwi_off, 1, RCAR_DREQ_HSCIF_RXI(i));
        }
    }

    // SYS-DMAC & RT-DMAC
    {
        {
            unsigned i, irq, hwi_off;
            hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;
            /* SYS-DMAC of Gen3 and Gen4 have the same the base address */
            uint32_t dev_base[] = { RCAR_GEN3_SYSDMAC1_BASE, RCAR_GEN3_SYSDMAC2_BASE };
            uint32_t dev_com_base[] = { RCAR_GEN4_SYSDMAC1_COM_BASE, RCAR_GEN4_SYSDMAC2_COM_BASE };

            uint32_t irq0[] = {
                RCAR_GEN4_INTCSYS_SPI_82,  RCAR_GEN4_INTCSYS_SPI_83,  RCAR_GEN4_INTCSYS_SPI_84, RCAR_GEN4_INTCSYS_SPI_85,
                RCAR_GEN4_INTCSYS_SPI_86,  RCAR_GEN4_INTCSYS_SPI_87,  RCAR_GEN4_INTCSYS_SPI_88, RCAR_GEN4_INTCSYS_SPI_89,
                RCAR_GEN4_INTCSYS_SPI_90,  RCAR_GEN4_INTCSYS_SPI_91,  RCAR_GEN4_INTCSYS_SPI_92, RCAR_GEN4_INTCSYS_SPI_93,
                RCAR_GEN4_INTCSYS_SPI_94,  RCAR_GEN4_INTCSYS_SPI_95,  RCAR_GEN4_INTCSYS_SPI_96, RCAR_GEN4_INTCSYS_SPI_97
            };
            uint32_t irq1[] = {
                RCAR_GEN4_INTCSYS_SPI_100,  RCAR_GEN4_INTCSYS_SPI_101,  RCAR_GEN4_INTCSYS_SPI_102, RCAR_GEN4_INTCSYS_SPI_103,
                RCAR_GEN4_INTCSYS_SPI_104,  RCAR_GEN4_INTCSYS_SPI_105,  RCAR_GEN4_INTCSYS_SPI_106, RCAR_GEN4_INTCSYS_SPI_107,
                RCAR_GEN4_INTCSYS_SPI_108,  RCAR_GEN4_INTCSYS_SPI_109,  RCAR_GEN4_INTCSYS_SPI_110, RCAR_GEN4_INTCSYS_SPI_111,
                RCAR_GEN4_INTCSYS_SPI_112,  RCAR_GEN4_INTCSYS_SPI_113,  RCAR_GEN4_INTCSYS_SPI_114, RCAR_GEN4_INTCSYS_SPI_115
            };

            uint32_t *irqs[] = { irq0, irq1 };

            HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

            for(i = 0; i < NUM_ELTS(dev_base); i++) {
                HWIATTR_USB_SET_LOCATION(&attr, dev_base[i], RCAR_GEN4_SYSDMAC_SIZE, 0, hwi_find_as(dev_base[i], 1));
                hwi_off = hwidev_add_dma(RCAR_HWI_SYSDMAC, &attr, HWI_NULL_OFF);
                ASSERT(hwi_find_unit(hwi_off) == (int)i);
                hwitag_add_location(hwi_off, dev_com_base[i], RCAR_GEN4_SYSDMAC_COM_SIZE, 0, 0);
                hwitag_add_regname(hwi_off, RCAR_HWI_DMA_DESC, RCAR_GEN4_SYSDMAC_DESC);
                hwitag_add_regname(hwi_off, RCAR_HWI_DMA_SIZE, RCAR_GEN4_SYSDMAC_REGS);
                
                for(irq = 0; irq < NUM_ELTS(irq0); irq++) {
                    hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
                }
            }
        }
        {
            unsigned i, irq, hwi_off;
            hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

            uint32_t dev_base[] = { RCAR_GEN4_RTDMAC0_BASE, RCAR_GEN4_RTDMAC1_BASE, RCAR_GEN4_RTDMAC2_BASE, RCAR_GEN4_RTDMAC3_BASE };
            uint32_t dev_com_base[] = { RCAR_GEN4_RTDMAC0_COM_BASE, RCAR_GEN4_RTDMAC1_COM_BASE, RCAR_GEN4_RTDMAC2_COM_BASE, RCAR_GEN4_RTDMAC3_COM_BASE };

            uint32_t irq0[] = {
                RCAR_GEN4_INTCSYS_SPI_10, RCAR_GEN4_INTCSYS_SPI_11,  RCAR_GEN4_INTCSYS_SPI_12,  RCAR_GEN4_INTCSYS_SPI_13,
                RCAR_GEN4_INTCSYS_SPI_14, RCAR_GEN4_INTCSYS_SPI_15,  RCAR_GEN4_INTCSYS_SPI_16,  RCAR_GEN4_INTCSYS_SPI_17,
                RCAR_GEN4_INTCSYS_SPI_18, RCAR_GEN4_INTCSYS_SPI_19,  RCAR_GEN4_INTCSYS_SPI_20,  RCAR_GEN4_INTCSYS_SPI_21,
                RCAR_GEN4_INTCSYS_SPI_22, RCAR_GEN4_INTCSYS_SPI_23,  RCAR_GEN4_INTCSYS_SPI_24,  RCAR_GEN4_INTCSYS_SPI_25
            };
            uint32_t irq1[] = {
                RCAR_GEN4_INTCSYS_SPI_28, RCAR_GEN4_INTCSYS_SPI_29, RCAR_GEN4_INTCSYS_SPI_30,  RCAR_GEN4_INTCSYS_SPI_31,
                RCAR_GEN4_INTCSYS_SPI_32, RCAR_GEN4_INTCSYS_SPI_33, RCAR_GEN4_INTCSYS_SPI_34,  RCAR_GEN4_INTCSYS_SPI_35,
                RCAR_GEN4_INTCSYS_SPI_36, RCAR_GEN4_INTCSYS_SPI_37, RCAR_GEN4_INTCSYS_SPI_38,  RCAR_GEN4_INTCSYS_SPI_39,
                RCAR_GEN4_INTCSYS_SPI_40, RCAR_GEN4_INTCSYS_SPI_41, RCAR_GEN4_INTCSYS_SPI_42,  RCAR_GEN4_INTCSYS_SPI_43
            };
            uint32_t irq2[] = {
                RCAR_GEN4_INTCSYS_SPI_46, RCAR_GEN4_INTCSYS_SPI_47, RCAR_GEN4_INTCSYS_SPI_48,  RCAR_GEN4_INTCSYS_SPI_49,
                RCAR_GEN4_INTCSYS_SPI_50, RCAR_GEN4_INTCSYS_SPI_51, RCAR_GEN4_INTCSYS_SPI_52,  RCAR_GEN4_INTCSYS_SPI_53,
                RCAR_GEN4_INTCSYS_SPI_54, RCAR_GEN4_INTCSYS_SPI_55, RCAR_GEN4_INTCSYS_SPI_56,  RCAR_GEN4_INTCSYS_SPI_57,
                RCAR_GEN4_INTCSYS_SPI_58, RCAR_GEN4_INTCSYS_SPI_59, RCAR_GEN4_INTCSYS_SPI_60,  RCAR_GEN4_INTCSYS_SPI_61
            };
            uint32_t irq3[] = {
                RCAR_GEN4_INTCSYS_SPI_64,  RCAR_GEN4_INTCSYS_SPI_65,  RCAR_GEN4_INTCSYS_SPI_66, RCAR_GEN4_INTCSYS_SPI_67,
                RCAR_GEN4_INTCSYS_SPI_68,  RCAR_GEN4_INTCSYS_SPI_69,  RCAR_GEN4_INTCSYS_SPI_70, RCAR_GEN4_INTCSYS_SPI_71,
                RCAR_GEN4_INTCSYS_SPI_72,  RCAR_GEN4_INTCSYS_SPI_73,  RCAR_GEN4_INTCSYS_SPI_74, RCAR_GEN4_INTCSYS_SPI_75,
                RCAR_GEN4_INTCSYS_SPI_76,  RCAR_GEN4_INTCSYS_SPI_77,  RCAR_GEN4_INTCSYS_SPI_78, RCAR_GEN4_INTCSYS_SPI_79
            };

            uint32_t *irqs[] = {irq0, irq1, irq2, irq3};

            HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

            for(i = 0; i < NUM_ELTS(dev_base); i++) {
                HWIATTR_USB_SET_LOCATION(&attr,  dev_base[i], RCAR_GEN4_SYSDMAC_SIZE, 0, hwi_find_as( dev_base[i], 1));
                hwi_off = hwidev_add_dma(RCAR_HWI_RTDMAC, &attr, HWI_NULL_OFF);
                ASSERT(hwi_find_unit(hwi_off) == (int)i);
                hwitag_add_location(hwi_off, dev_com_base[i], RCAR_GEN4_SYSDMAC_COM_SIZE, 0, 0);
                hwitag_add_regname(hwi_off, RCAR_HWI_DMA_DESC, RCAR_GEN4_SYSDMAC_DESC);
                hwitag_add_regname(hwi_off, RCAR_HWI_DMA_SIZE, RCAR_GEN4_SYSDMAC_REGS);
                for(irq = 0; irq < NUM_ELTS(irq0); irq++) {
                    hwitag_set_avail_ivec(hwi_off, irq, irqs[i][irq]);
                }
            }
        }

        {
            unsigned hwi_off;
            hwi_off = hwidev_add("dma,options", 0, HWI_NULL_OFF);
            hwitag_add_regname(hwi_off, "sysdmac_chans",         32);
            hwitag_add_regname(hwi_off, "sysdmac_chans_per_grp", 16);
            hwitag_add_regname(hwi_off, "rtdmac_chans",          64);
            hwitag_add_regname(hwi_off, "rtdmac_chans_per_grp",  16);
        }
    }

    // WDT
    {
        unsigned hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        const struct hwi_inputclk clksrc_kick = {.clk = 10000, .div = 1};

        HWIATTR_TIMER_SET_NUM_CLK(&attr, 1);

        HWIATTR_TIMER_SET_LOCATION(&attr, RCAR_RWDT_BASE, RCAR_WDT_SIZE, 0, hwi_find_as(RCAR_RWDT_BASE, 1));
        hwi_off = hwidev_add_timer(RCAR_HWI_WDT, &attr,  HWI_NULL_OFF);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);

        hwi_off = hwidev_add("wdt,options", 0, HWI_NULL_OFF);

        hwitag_add_regname(hwi_off, "enable_mask",      0x080);
        hwitag_add_regname(hwi_off, "enable_condition", 0x080);
        hwitag_add_regname(hwi_off, "enable_offset",    0x4);

        /* use wdtkick cmd param -W to set wdt,regwrite value
         * since value=0x5A5AFF00 but egname.offset is Uint16t */
    }

    { // CPG
        unsigned hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        uint32_t clk = board_ext_clk, div = 1;
        const struct hwi_inputclk clksrc_kick = {.clk = clk, .div = div};

        HWIATTR_TIMER_SET_NUM_CLK(&attr, 1);

        HWIATTR_TIMER_SET_LOCATION(&attr, RCAR_GEN4_CPG_DM0_BASE, RCAR_GEN4_CPG_SIZE, 0, hwi_find_as(RCAR_GEN4_CPG_DM0_BASE, 1));
        hwi_off = hwidev_add_timer(RCAR_HWI_CPG, &attr,  HWI_NULL_OFF);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_inputclk(hwi_off, 0, (struct hwi_inputclk *)&clksrc_kick);
    }

    { // SYSC
        unsigned hwi_off;
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        
        hwi_off = hwidev_add(RCAR_HWI_SYSC, hwi_devclass_NONE, HWI_NULL_OFF);
        HWIATTR_SET_LOCATION(&attr, RCAR_GEN4_SYSC_DM0_BASE, RCAR_GEN4_SYSC_SIZE, 0, hwi_find_as(RCAR_GEN4_SYSC_DM0_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
    }

    // SDHI
    {
        unsigned i, hwi_off;

        hwiattr_sdio_t sdio_attr = HWIATTR_SDIO_T_INITIALIZER;
        /* RCAR_GEN4_CPG_CLK_SD0CKCR = 0x4 SDnH = 800 MHz SDn = 200 Mhz*/
        struct hwi_inputclk sdio_clk = {.clk = RCAR_GEN4_CPG_CLK_SD0, .div = 1};

        HWIATTR_SDIO_SET_NUM_IRQ(&sdio_attr, 1);
        HWIATTR_SDIO_SET_NUM_CLK(&sdio_attr, 1);
        HWIATTR_SDIO_SET_DLL(&sdio_attr, "rcar");

        dev_info_t dev[] = {
            {.base=RCAR_MMCIF0_BASE, .irq=RCAR_GEN4_INTCSYS_SPI_440},
        };

        sd_info_t sd[] = {
            {.pwr_vdd_base=RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP3, .pwr_vdd_offset=RCAR_GEN4_GPIO_OUTDT, .pwr_vdd_shift=12,
             .pwr_if_base=RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP8, .pwr_if_offset=RCAR_GEN4_GPIO_OUTDT, .pwr_if_shift=12,
             .pfc_base=RCAR_GEN4_GPIO_DM0_BASE+RCAR_V4H_GPIO_GP3, .pfc_offset=RCAR_GEN4_GPIO_POC, .pfc_mask=0xFFF, .pfc_shift=0},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_SDIO_SET_LOCATION(&sdio_attr, dev[i].base, RCAR_SDHI_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_sdio(HWI_NULL_OFF, &sdio_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &sdio_clk);
            hwitag_set_avail_ivec(hwi_find_bus(HWI_ITEM_BUS_SDIO, i), -1, dev[i].irq);

            hwi_off = hwidev_add(RCAR_HWI_SDHI_VDD, hwi_devclass_NONE, HWI_NULL_OFF);
            if(sd[i].pwr_vdd_base != 0) {
                hwitag_add_location(hwi_off, sd[i].pwr_vdd_base, RCAR_GEN4_GPIO_GP_SIZE, 0, hwi_find_as(sd[i].pwr_vdd_base, 1));
                hwitag_add_regname(hwi_off, RCAR_HWI_PFC_REG, sd[i].pwr_vdd_offset);
                hwitag_add_regname(hwi_off, RCAR_HWI_PFC_SHIFT, sd[i].pwr_vdd_shift);
            }

            hwi_off = hwidev_add(RCAR_HWI_SDHI_IF, hwi_devclass_NONE, HWI_NULL_OFF);
            if(sd[i].pwr_if_base != 0) {
                hwitag_add_location(hwi_off, sd[i].pwr_if_base, RCAR_GEN4_GPIO_GP_SIZE, 0, hwi_find_as(sd[i].pwr_if_base, 1));
                hwitag_add_regname(hwi_off, RCAR_HWI_PFC_REG, sd[i].pwr_if_offset);
                hwitag_add_regname(hwi_off, RCAR_HWI_PFC_SHIFT, sd[i].pwr_if_shift);
            }

            hwi_off = hwidev_add(RCAR_HWI_SDHI_PFC, hwi_devclass_NONE, HWI_NULL_OFF);
            if(sd[i].pfc_base != 0) {
                hwitag_add_location(hwi_off, sd[i].pfc_base, RCAR_GEN4_GPIO_GP_SIZE, 0, hwi_find_as(sd[i].pfc_base, 1));
                hwitag_add_regname(hwi_off, RCAR_HWI_PFC_REG, sd[i].pfc_offset);
                hwitag_add_regname(hwi_off, RCAR_HWI_PFC_PMMR, RCAR_GEN4_GPIO_PMMR);
                hwitag_add_regname(hwi_off, RCAR_HWI_PFC_MASK, sd[i].pfc_mask);
                hwitag_add_regname(hwi_off, RCAR_HWI_PFC_SHIFT, sd[i].pfc_shift);
            }
        }
    }

    // I2C
    {
        unsigned i, hwi_off;

        hwiattr_i2c_t i2c_attr = HWIATTR_I2C_T_INITIALIZER;
        hwiattr_common_t i2c_poc_attr = HWIATTR_COMMON_INITIALIZER;
        /* All I2Cs operate on 133.33 MHz only */
        struct hwi_inputclk i2c_clk = {.clk = RCAR_GEN4_CPG_CLK_S0D6_PER, .div = 1};

        /* each I2C has an interrupt */
        HWIATTR_I2C_SET_NUM_IRQ(&i2c_attr, 1);
        HWIATTR_I2C_SET_NUM_CLK(&i2c_attr, 1);
        HWIATTR_SET_NUM_DMA(&i2c_attr.common, 2);

        dev_info_t dev[] = {
            {.base = RCAR_I2C0_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_610},
            {.base = RCAR_I2C1_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_611},
            {.base = RCAR_I2C2_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_612},
            {.base = RCAR_I2C3_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_613},
            {.base = RCAR_I2C4_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_614},
            {.base = RCAR_I2C5_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_615},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_I2C_SET_LOCATION(&i2c_attr, dev[i].base, RCAR_I2C_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_i2c(HWI_NULL_OFF, &i2c_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &i2c_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
            hwitag_set_dma(hwi_off, 0, RCAR_DREQ_I2C_TXI(i));
            hwitag_set_dma(hwi_off, 1, RCAR_DREQ_I2C_RXI(i));
            hwitag_add_regname(hwi_off, RCAR_HWI_CPG_MOD_ID, CPG_MSTP_GEN4_I2C(i));

            /* Use for setting PFC POC in Fast Mode+ */
            hwi_off = hwidev_add(RCAR_HWI_I2C_POC, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&i2c_poc_attr, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP8, RCAR_GEN4_GPIO_GP_SIZE, 0, 
                        hwi_find_as(RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP8, 1));
            hwitag_add_common(hwi_off, &i2c_poc_attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_add_regname(hwi_off, RCAR_HWI_PFC_PMMR, RCAR_GEN4_GPIO_PMMR);
            hwitag_add_regname(hwi_off, RCAR_HWI_PFC_REG, RCAR_GEN4_GPIO_POC);
            hwitag_add_regname(hwi_off, RCAR_HWI_PFC_MASK, 3);
            hwitag_add_regname(hwi_off, RCAR_HWI_PFC_SHIFT, 2*i);
        }
    }

    {   // SPI
        unsigned i, hwi_off;

        hwiattr_spi_t msiof_attr = HWIATTR_SPI_T_INITIALIZER;
        struct hwi_inputclk msiof_clk = {.clk = 26000000, .div = 1};
        HWIATTR_SPI_SET_NUM_CLK(&msiof_attr, 1);

        /* each MSIOF SPI has an interrupt */
        HWIATTR_SPI_SET_NUM_IRQ(&msiof_attr, 1);
        HWIATTR_SET_NUM_DMA(&msiof_attr.common, 2);

        dev_info_t dev[] = {
            {.base = RCAR_MSIOF0_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_239},
            {.base = RCAR_MSIOF1_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_240},
            {.base = RCAR_MSIOF2_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_241},
            {.base = RCAR_MSIOF3_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_242},
            {.base = RCAR_MSIOF4_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_243},
            {.base = RCAR_MSIOF5_BASE, .irq = RCAR_GEN4_INTCSYS_SPI_244},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_SPI_SET_LOCATION(&msiof_attr, dev[i].base, RCAR_MSIOF_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwibus_add_msiof(HWI_NULL_OFF, &msiof_attr);
            ASSERT(hwi_find_unit(hwi_off) == i);
            hwitag_set_inputclk(hwi_off, 0, &msiof_clk);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
            hwitag_set_dma(hwi_off, 0, RCAR_DREQ_MSIOF_TXI(i));
            hwitag_set_dma(hwi_off, 1, RCAR_DREQ_MSIOF_RXI(i));
        }
    }

    // RPC-SPI
    {
        unsigned hwi_off;
        hwiattr_spi_t attr = HWIATTR_SPI_T_INITIALIZER;
        struct hwi_inputclk rpc_clk = {.clk = RCAR_GEN4_CPG_CLK_RPCD2, .div = 2};
        HWIATTR_SPI_SET_NUM_CLK(&attr, 1);

        HWIATTR_SPI_SET_LOCATION(&attr, RCAR_RPC_BASE, RCAR_RPC_SIZE, 0, hwi_find_as(RCAR_RPC_BASE, 1));
        hwi_off = hwibus_add_spi(HWI_NULL_OFF, &attr);
        ASSERT(hwi_find_unit(hwi_off) == 0);
        hwitag_set_inputclk(hwi_off, 0, &rpc_clk);

        hwi_off = hwidev_add(RCAR_HWI_SPI_DEV, hwi_devclass_NONE, HWI_NULL_OFF);
        hwitag_add_location(hwi_off, RCAR_RPC_DEV_ADDR, RCAR_RPC_DEV_SIZE, 0, hwi_find_as(RCAR_RPC_DEV_ADDR, 1));
        
    }

    //GPIO
    {
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_NUM_IRQ(&attr, 1);

        dev_info_t dev[] = {
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP0, .irq = 992 + (32 * 0) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP1, .irq = 992 + (32 * 1) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP2, .irq = 992 + (32 * 2) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP3, .irq = 992 + (32 * 3) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP4, .irq = 992 + (32 * 4) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP5, .irq = 992 + (32 * 5) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP6, .irq = 992 + (32 * 6) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP7, .irq = 992 + (32 * 7) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP8, .irq = 992 + (32 * 8) }, /* this irq is 'vector base' in init_intrinfo.c */
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_GPIO, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN4_GPIO_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
            hwitag_add_regname(hwi_off, RCAR_HWI_GPIO_GROUP_NUM, 8);
        }
    }

    // VIN
    {
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;
        HWIATTR_SET_NUM_IRQ(&attr, 1);

        dev_info_t dev[] = {
            { .base = RCAR_VIN0_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_529 },
            { .base = RCAR_VIN1_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_530 },
            { .base = RCAR_VIN2_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_531 },
            { .base = RCAR_VIN3_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_532 },
            { .base = RCAR_VIN4_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_533 },
            { .base = RCAR_VIN5_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_534 },
            { .base = RCAR_VIN6_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_535 },
            { .base = RCAR_VIN7_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_536 },
            { .base = RCAR_VIN8_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_537 },
            { .base = RCAR_VIN9_BASE,   .irq=RCAR_GEN4_INTCSYS_SPI_538 },
            { .base = RCAR_VIN10_BASE,  .irq=RCAR_GEN4_INTCSYS_SPI_539 },
            { .base = RCAR_VIN11_BASE,  .irq=RCAR_GEN4_INTCSYS_SPI_540 },
            { .base = RCAR_VIN12_BASE,  .irq=RCAR_GEN4_INTCSYS_SPI_541 },
            { .base = RCAR_VIN13_BASE,  .irq=RCAR_GEN4_INTCSYS_SPI_542 },
            { .base = RCAR_VIN14_BASE,  .irq=RCAR_GEN4_INTCSYS_SPI_543 },
            { .base = RCAR_VIN15_BASE,  .irq=RCAR_GEN4_INTCSYS_SPI_544 },
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_VIN, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_VIN_SIZE, 0, hwi_find_as(dev[i].base, 1));
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
            {.base = RCAR_GEN4_CSI20_BASE},
            {.base = RCAR_GEN4_CSI21_BASE},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_CSI, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN4_CSI2_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
        }
    }

    // ISP
    {
        unsigned i, hwi_off;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        dev_info_t dev[] = {
            {.base = RCAR_ISP0_BASE},
            {.base = RCAR_ISP1_BASE},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_ISP, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_ISP_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
        }
    }

    { // CMT
        unsigned i, j, hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        dev_info_t dev[] = {
            {.base=RCAR_CMT0_BASE, .irq=RCAR_CMT0_REG_OFFSET},
            {.base=RCAR_CMT1_BASE, .irq=RCAR_CMT1_REG_OFFSET},
            {.base=RCAR_CMT2_BASE, .irq=RCAR_CMT1_REG_OFFSET},
            {.base=RCAR_CMT3_BASE, .irq=RCAR_CMT1_REG_OFFSET}
        };
        struct hwi_inputclk clksrc_kick[] = {
            {.clk = 32800,             .div = 1}, // RCLK
            {.clk = board_ext_clk / 2, .div = 1}, // CPEX
            {.clk = 32000,             .div = 1}  // PSEU
        };
        uint32_t irq0[] = {
            RCAR_GEN4_INTCSYS_SPI_260,  RCAR_GEN4_INTCSYS_SPI_261, 0, 0, 0, 0, 0, 0
        };
        uint32_t irq1[] = {
            RCAR_GEN4_INTCSYS_SPI_262,  RCAR_GEN4_INTCSYS_SPI_263,  RCAR_GEN4_INTCSYS_SPI_264,  RCAR_GEN4_INTCSYS_SPI_265,
            RCAR_GEN4_INTCSYS_SPI_266,  RCAR_GEN4_INTCSYS_SPI_267,  RCAR_GEN4_INTCSYS_SPI_268,  RCAR_GEN4_INTCSYS_SPI_269
        };
        uint32_t irq2[] = {
            RCAR_GEN4_INTCSYS_SPI_270,  RCAR_GEN4_INTCSYS_SPI_271,  RCAR_GEN4_INTCSYS_SPI_272,  RCAR_GEN4_INTCSYS_SPI_273,
            RCAR_GEN4_INTCSYS_SPI_274,  RCAR_GEN4_INTCSYS_SPI_275,  RCAR_GEN4_INTCSYS_SPI_276,  RCAR_GEN4_INTCSYS_SPI_277
        };
        uint32_t irq3[] = {
            RCAR_GEN4_INTCSYS_SPI_278,  RCAR_GEN4_INTCSYS_SPI_279,  RCAR_GEN4_INTCSYS_SPI_280,  RCAR_GEN4_INTCSYS_SPI_281,
            RCAR_GEN4_INTCSYS_SPI_282,  RCAR_GEN4_INTCSYS_SPI_283,  RCAR_GEN4_INTCSYS_SPI_284,  RCAR_GEN4_INTCSYS_SPI_285
        };
        uint32_t *irqs[] = { irq0, irq1, irq2, irq3 };

        if (!(in32(RCAR_GEN4_RST_DM0_BASE + RCAR_GEN4_RST_MODEMR0) & BIT(29))) {
            clksrc_kick[0].clk = 32768; // EXTALR pin
        }

        HWIATTR_TIMER_SET_NUM_IRQ(&attr, NUM_ELTS(irq1));
        HWIATTR_TIMER_SET_NUM_CLK(&attr, NUM_ELTS(clksrc_kick));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_TIMER_SET_LOCATION(&attr, dev[i].base, RCAR_CMT_SIZE, 0, hwi_find_as(dev[i].base, 1));
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

    { // TMU
        unsigned i, j, hwi_off;
        hwiattr_timer_t attr = HWIATTR_TIMER_T_INITIALIZER;
        dev_info_t dev[] = {
            {.base = RCAR_TMU_0_2_BASE,  },
            {.base = RCAR_TMU_3_5_BASE,  },
            {.base = RCAR_TMU_6_8_BASE,  },
            {.base = RCAR_TMU_9_11_BASE, },
            {.base = RCAR_TMU_12_14_BASE,}
        };
        uint32_t irq0[] = {RCAR_GEN4_INTCSYS_SPI_512,  0, RCAR_GEN4_INTCSYS_SPI_513,  0, RCAR_GEN4_INTCSYS_SPI_514, 0};
        uint32_t irq1[] = {RCAR_GEN4_INTCSYS_SPI_504,  0, RCAR_GEN4_INTCSYS_SPI_505,  0, RCAR_GEN4_INTCSYS_SPI_506, RCAR_GEN4_INTCSYS_SPI_507};
        uint32_t irq2[] = {RCAR_GEN4_INTCSYS_SPI_508,  0, RCAR_GEN4_INTCSYS_SPI_509,  0, RCAR_GEN4_INTCSYS_SPI_510, RCAR_GEN4_INTCSYS_SPI_511};
        uint32_t irq3[] = {RCAR_GEN4_INTCSYS_SPI_472,  0, RCAR_GEN4_INTCSYS_SPI_473,  0, RCAR_GEN4_INTCSYS_SPI_474, RCAR_GEN4_INTCSYS_SPI_475};
        uint32_t irq4[] = {RCAR_GEN4_INTCSYS_SPI_476,  0, RCAR_GEN4_INTCSYS_SPI_477,  0, RCAR_GEN4_INTCSYS_SPI_478, RCAR_GEN4_INTCSYS_SPI_479};
        uint32_t *irqs[] = { irq0, irq1, irq2, irq3, irq4 };

        struct hwi_inputclk clk0[] = {
            {.clk=board_ext_clk, .div=1}, {.clk=133333333, .div=1},  {.clk=133333333, .div=1}, {.clk=133333333, .div=1}, {.clk=133333333, .div=1}
        };

        HWIATTR_TIMER_SET_NUM_IRQ(&attr, NUM_ELTS(irq1));
        HWIATTR_TIMER_SET_NUM_CLK(&attr, NUM_ELTS(clk0));

        for(i = 0; i < NUM_ELTS(dev); i++) {
            HWIATTR_TIMER_SET_LOCATION(&attr, dev[i].base, RCAR_TMU_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwi_off = hwidev_add_timer(RCAR_HWI_TMU, &attr,  HWI_NULL_OFF);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_set_inputclk(hwi_off, 0, &clk0[i]);
            for (j = 0; j < NUM_ELTS(irq1); j++) {
                hwitag_set_avail_ivec(hwi_off, j, irqs[i][j]);
            }
        }
    }

    { // Audio
        unsigned hwi_off;
        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        hwi_off = hwidev_add(RCAR_HWI_AUDIO_ADG, hwi_devclass_NONE, HWI_NULL_OFF);
        HWIATTR_SET_LOCATION(&attr, RCAR_ADG_BASE, RCAR_ADG_SIZE, 0, hwi_find_as(RCAR_ADG_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);

        hwi_off = hwidev_add(RCAR_HWI_AUDIO_SSI, hwi_devclass_NONE, HWI_NULL_OFF);
        HWIATTR_SET_LOCATION(&attr, RCAR_SSI_BASE, RCAR_SSI_SIZE, 0, hwi_find_as(RCAR_SSI_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);

        hwi_off = hwidev_add(RCAR_HWI_AUDIO_SSIU, hwi_devclass_NONE, HWI_NULL_OFF);
        HWIATTR_SET_LOCATION(&attr, RCAR_SSIU_BASE, RCAR_SSIU_SIZE, 0, hwi_find_as(RCAR_SSIU_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
    }
}
