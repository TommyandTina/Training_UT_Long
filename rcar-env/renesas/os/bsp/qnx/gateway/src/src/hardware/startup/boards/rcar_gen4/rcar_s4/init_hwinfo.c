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
#include <hw/rcar_cpg.h>
#include "rcar_startup.h"
#include <aarch64/r-car.h>

#define CPG_MSTP_GEN4_I2C(n)            (n == 0 ? CPG_MSTP_GEN4_I2C0 : \
                                        (n == 1 ? CPG_MSTP_GEN4_I2C1 : \
                                        (n == 2 ? CPG_MSTP_GEN4_I2C2 : \
                                        (n == 3 ? CPG_MSTP_GEN4_I2C3 : \
                                        (n == 4 ? CPG_MSTP_GEN4_I2C4 : CPG_MSTP_GEN4_I2C5)))))

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
            {.base=RCAR_SCIF0_BASE, .irq=RCAR_S4_INTCSYS_SCIF0},
            {.base=RCAR_SCIF1_BASE, .irq=RCAR_S4_INTCSYS_SCIF1},
            {.base=0,               .irq=0                    },
            {.base=RCAR_SCIF3_BASE, .irq=RCAR_S4_INTCSYS_SCIF3},
            {.base=RCAR_SCIF4_BASE, .irq=RCAR_S4_INTCSYS_SCIF4},
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
        struct hwi_inputclk uart_clk = {.clk = RCAR_GEN4_CPG_CLK_SASYNCPERD1, .div = 1};

        /* each UART has an interrupt */
        HWIATTR_UART_SET_NUM_IRQ(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_CLK(&uart_attr, 1);
        HWIATTR_UART_SET_NUM_DMA(&uart_attr, 2);

        dev_info_t dev[] = {
            {.base=RCAR_HSCIF0_BASE, .irq=RCAR_S4_INTCSYS_HSCIF0},
            {.base=RCAR_HSCIF1_BASE, .irq=RCAR_S4_INTCSYS_HSCIF1},
            {.base=RCAR_HSCIF2_BASE, .irq=RCAR_S4_INTCSYS_HSCIF2},
            {.base=RCAR_HSCIF3_BASE, .irq=RCAR_S4_INTCSYS_HSCIF3},
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

    // SDHI
    {
        unsigned i, hwi_off;

        hwiattr_sdio_t sdio_attr = HWIATTR_SDIO_T_INITIALIZER;
        /* RCAR_GEN4_CPG_SD0CKCR = 0x4 SDnH = 800 MHz SDn = 200 Mhz*/
        struct hwi_inputclk sdio_clk = {.clk = RCAR_GEN4_CPG_CLK_SD0, .div = 1};

        HWIATTR_SDIO_SET_NUM_IRQ(&sdio_attr, 1);
        HWIATTR_SDIO_SET_NUM_CLK(&sdio_attr, 1);
        HWIATTR_SDIO_SET_DLL(&sdio_attr, "rcar");

        dev_info_t dev[] = {
            {.base=RCAR_MMCIF0_BASE, .irq=RCAR_S4_INTCSYS_SDHI0},
        };

        sd_info_t sd[] = {
            {.pwr_vdd_base=RCAR_GEN4_GPIO_DM0_BASE+RCAR_S4_GPIO_GP1, .pwr_vdd_offset=RCAR_GEN4_GPIO_OUTDT, .pwr_vdd_shift=24,
             .pfc_base=RCAR_GEN4_GPIO_DM0_BASE+RCAR_S4_GPIO_GP1, .pfc_offset=RCAR_GEN4_GPIO_POC, .pfc_mask=0xFFF, .pfc_shift=12},
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
            {.base = RCAR_I2C0_BASE, .irq = RCAR_S4_INTCSYS_I2C0},
            {.base = RCAR_I2C1_BASE, .irq = RCAR_S4_INTCSYS_I2C1},
            {.base = RCAR_I2C2_BASE, .irq = RCAR_S4_INTCSYS_I2C2},
            {.base = RCAR_I2C3_BASE, .irq = RCAR_S4_INTCSYS_I2C3},
            {.base = RCAR_I2C4_BASE, .irq = RCAR_S4_INTCSYS_I2C4},
            {.base = RCAR_I2C5_BASE, .irq = RCAR_S4_INTCSYS_I2C5},
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
            HWIATTR_SET_LOCATION(&i2c_poc_attr, RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP1, RCAR_GEN4_GPIO_GP_SIZE, 0, 
                        hwi_find_as(RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP1, 1));
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
            {.base = RCAR_MSIOF0_BASE, .irq = RCAR_S4_INTCSYS_MSIOF0},
            {.base = RCAR_MSIOF1_BASE, .irq = RCAR_S4_INTCSYS_MSIOF1},
            {.base = RCAR_MSIOF2_BASE, .irq = RCAR_S4_INTCSYS_MSIOF2},
            {.base = RCAR_MSIOF3_BASE, .irq = RCAR_S4_INTCSYS_MSIOF3},
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

    {   // Thermal
        unsigned i, hwi_off;
        //hwi_tag *tag;

        hwiattr_common_t attr = HWIATTR_COMMON_INITIALIZER;

        dev_info_t dev[] = {
            { 0 },
            {.base=RCAR_TSC1_BASE},
            {.base=RCAR_TSC2_BASE},
            {.base=RCAR_TSC3_BASE},
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_THERMAL, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_TSC_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
        }

        HWIATTR_SET_NUM_IRQ(&attr, 1);
        hwi_off = hwidev_add(RCAR_HWI_THERMAL_OPS, hwi_devclass_NONE, HWI_NULL_OFF);
        HWIATTR_SET_LOCATION(&attr, RCAR_ECM_BASE, RCAR_ECM_SIZE, 0, hwi_find_as(RCAR_ECM_BASE, 1));
        hwitag_add_common(hwi_off, &attr);
        ASSERT(hwi_off != HWI_NULL_OFF);
        hwitag_set_ivec(hwi_off, 0, RCAR_S4_INTCSYS_ECM_ERR16);
        hwitag_add_regname(hwi_off, RCAR_HWI_THERMAL_CHAN_MIN, 1);
        hwitag_add_regname(hwi_off, RCAR_HWI_THERMAL_CHAN_MAX, 3);
        hwitag_add_regname(hwi_off, RCAR_HWI_THERMAL_TYPE,  RCAR_THERMAL_TYPE_A);
        hwitag_add_regname(hwi_off, RCAR_HWI_THERMAL_IRQ_NUM,  1);
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
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0, .irq = 992 + (32 * 0) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP1, .irq = 992 + (32 * 1) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP2, .irq = 992 + (32 * 2) }, /* this irq is 'vector base' in init_intrinfo.c */
            { .base = RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP3, .irq = 992 + (32 * 3) }, /* this irq is 'vector base' in init_intrinfo.c */
        };

        for(i = 0; i < NUM_ELTS(dev); i++) {
            hwi_off = hwidev_add(RCAR_HWI_GPIO, hwi_devclass_NONE, HWI_NULL_OFF);
            HWIATTR_SET_LOCATION(&attr, dev[i].base, RCAR_GEN4_GPIO_SIZE, 0, hwi_find_as(dev[i].base, 1));
            hwitag_add_common(hwi_off, &attr);
            ASSERT(hwi_off != HWI_NULL_OFF);
            hwitag_set_ivec(hwi_off, 0, dev[i].irq);
            hwitag_add_regname(hwi_off, RCAR_HWI_GPIO_GROUP_NUM, 4);
        }
    }

    // SYS-DMAC & RT-DMAC
    {
        {
            unsigned i, irq, hwi_off;
            hwiattr_dma_t attr = HWIATTR_DMA_T_INITIALIZER;

            uint32_t dev_base[] = { RCAR_GEN3_SYSDMAC1_BASE, RCAR_GEN3_SYSDMAC2_BASE };
            uint32_t dev_com_base[] = { RCAR_GEN4_SYSDMAC1_COM_BASE, RCAR_GEN4_SYSDMAC2_COM_BASE };

            uint32_t irq0[] = {
                RCAR_S4_INTCSYS_SYDM10,  RCAR_S4_INTCSYS_SYDM11,  RCAR_S4_INTCSYS_SYDM12,  RCAR_S4_INTCSYS_SYDM13,
                RCAR_S4_INTCSYS_SYDM14,  RCAR_S4_INTCSYS_SYDM15,  RCAR_S4_INTCSYS_SYDM16,  RCAR_S4_INTCSYS_SYDM17,
                RCAR_S4_INTCSYS_SYDM18,  RCAR_S4_INTCSYS_SYDM19,  RCAR_S4_INTCSYS_SYDM110, RCAR_S4_INTCSYS_SYDM111,
                RCAR_S4_INTCSYS_SYDM112, RCAR_S4_INTCSYS_SYDM113, RCAR_S4_INTCSYS_SYDM114, RCAR_S4_INTCSYS_SYDM115
            };
            uint32_t irq1[] = {
                RCAR_S4_INTCSYS_SYDM20,  RCAR_S4_INTCSYS_SYDM21,  RCAR_S4_INTCSYS_SYDM22,  RCAR_S4_INTCSYS_SYDM23,
                RCAR_S4_INTCSYS_SYDM24,  RCAR_S4_INTCSYS_SYDM25,  RCAR_S4_INTCSYS_SYDM26,  RCAR_S4_INTCSYS_SYDM27,
                RCAR_S4_INTCSYS_SYDM28,  RCAR_S4_INTCSYS_SYDM29,  RCAR_S4_INTCSYS_SYDM210, RCAR_S4_INTCSYS_SYDM211,
                RCAR_S4_INTCSYS_SYDM212, RCAR_S4_INTCSYS_SYDM213, RCAR_S4_INTCSYS_SYDM214, RCAR_S4_INTCSYS_SYDM215
            };

            uint32_t *irqs[] = { irq0, irq1 };

            HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

            for(i = 0; i < NUM_ELTS(dev_base); i++) {
                HWIATTR_USB_SET_LOCATION(&attr, dev_base[i], RCAR_GEN4_SYSDMAC_SIZE, 0, hwi_find_as(dev_base[i], 1));
                hwi_off = hwidev_add_dma(RCAR_HWI_SYSDMAC, &attr, HWI_NULL_OFF);
                ASSERT(hwi_find_unit(hwi_off) == i);
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
                RCAR_S4_INTCSYS_RTDM00,  RCAR_S4_INTCSYS_RTDM01,  RCAR_S4_INTCSYS_RTDM02,   RCAR_S4_INTCSYS_RTDM03,
                RCAR_S4_INTCSYS_RTDM04,  RCAR_S4_INTCSYS_RTDM05,  RCAR_S4_INTCSYS_RTDM06,   RCAR_S4_INTCSYS_RTDM07,
                RCAR_S4_INTCSYS_RTDM08,  RCAR_S4_INTCSYS_RTDM09,  RCAR_S4_INTCSYS_RTDM010,  RCAR_S4_INTCSYS_RTDM011,
                RCAR_S4_INTCSYS_RTDM012, RCAR_S4_INTCSYS_RTDM013, RCAR_S4_INTCSYS_RTDM014,  RCAR_S4_INTCSYS_RTDM015
            };
            uint32_t irq1[] = {
                RCAR_S4_INTCSYS_RTDM10,  RCAR_S4_INTCSYS_RTDM11,  RCAR_S4_INTCSYS_RTDM12,   RCAR_S4_INTCSYS_RTDM13,
                RCAR_S4_INTCSYS_RTDM14,  RCAR_S4_INTCSYS_RTDM15,  RCAR_S4_INTCSYS_RTDM16,   RCAR_S4_INTCSYS_RTDM17,
                RCAR_S4_INTCSYS_RTDM18,  RCAR_S4_INTCSYS_RTDM19,  RCAR_S4_INTCSYS_RTDM110,  RCAR_S4_INTCSYS_RTDM111,
                RCAR_S4_INTCSYS_RTDM112, RCAR_S4_INTCSYS_RTDM113, RCAR_S4_INTCSYS_RTDM114,  RCAR_S4_INTCSYS_RTDM115
            };
            uint32_t irq2[] = {
                RCAR_S4_INTCSYS_RTDM20,  RCAR_S4_INTCSYS_RTDM21,  RCAR_S4_INTCSYS_RTDM22,   RCAR_S4_INTCSYS_RTDM23,
                RCAR_S4_INTCSYS_RTDM24,  RCAR_S4_INTCSYS_RTDM25,  RCAR_S4_INTCSYS_RTDM26,   RCAR_S4_INTCSYS_RTDM27,
                RCAR_S4_INTCSYS_RTDM28,  RCAR_S4_INTCSYS_RTDM29,  RCAR_S4_INTCSYS_RTDM210,  RCAR_S4_INTCSYS_RTDM211,
                RCAR_S4_INTCSYS_RTDM212, RCAR_S4_INTCSYS_RTDM213, RCAR_S4_INTCSYS_RTDM214,  RCAR_S4_INTCSYS_RTDM215
            };
            uint32_t irq3[] = {
                RCAR_S4_INTCSYS_RTDM30,  RCAR_S4_INTCSYS_RTDM31,  RCAR_S4_INTCSYS_RTDM32,   RCAR_S4_INTCSYS_RTDM33,
                RCAR_S4_INTCSYS_RTDM34,  RCAR_S4_INTCSYS_RTDM35,  RCAR_S4_INTCSYS_RTDM36,   RCAR_S4_INTCSYS_RTDM37,
                RCAR_S4_INTCSYS_RTDM38,  RCAR_S4_INTCSYS_RTDM39,  RCAR_S4_INTCSYS_RTDM310,  RCAR_S4_INTCSYS_RTDM311,
                RCAR_S4_INTCSYS_RTDM312, RCAR_S4_INTCSYS_RTDM313, RCAR_S4_INTCSYS_RTDM314,  RCAR_S4_INTCSYS_RTDM315
            };

            uint32_t *irqs[] = {irq0, irq1, irq2, irq3};

            HWIATTR_DMA_SET_NUM_IRQ(&attr, NUM_ELTS(irq0));

            for(i = 0; i < NUM_ELTS(dev_base); i++) {
                HWIATTR_USB_SET_LOCATION(&attr, dev_base[i], RCAR_GEN4_SYSDMAC_SIZE, 0, hwi_find_as(dev_base[i], 1));
                hwi_off = hwidev_add_dma(RCAR_HWI_RTDMAC, &attr, HWI_NULL_OFF);
                ASSERT(hwi_find_unit(hwi_off) == i);
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
            hwitag_add_regname(hwi_off, "rtdmac_chans_per_grp",   16);
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
}
