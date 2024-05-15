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

#include "hw/inout.h"
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/hwinfo.h>
#include <drvr/hwinfo.h>
#include <aarch64/r-car.h>   
#include <hw/hwinfo_rcar.h>  
#include <proto.h>

#define PFC_PMMR        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_pfc_reg_t *)gpio_dev->pfc_reg)->PMMR : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->PMMR)
#define PFC_GPSR        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_pfc_reg_t *)gpio_dev->pfc_reg)->GPSR[gpio_dev->group] : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->GPSR)
#define PFC_PUEN        (uintptr_t)(gpio_dev->gen3_soc_type ? 0 : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->PUEN)
#define PFC_PUD         (uintptr_t)(gpio_dev->gen3_soc_type ? 0 : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->PUD)
#define PFC_POC         (uintptr_t)(gpio_dev->gen3_soc_type ? 0 : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->POC)

#define GPIO_IOINTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->IOINTSEL : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->IOINTSEL)
#define GPIO_INOUTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INOUTSEL : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INOUTSEL)
#define GPIO_OUTDT       (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDT    : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDT)
#define GPIO_INDT        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INDT     : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INDT)
#define GPIO_INTDT       (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTDT    : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INTDT)
#define GPIO_INTCLR      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTCLR   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INTCLR)
#define GPIO_INTMSK      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSK   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSK)
#define GPIO_MSKCLR      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLR   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLR)
#define GPIO_POSNEG      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->POSNEG   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->POSNEG)
#define GPIO_EDGLEVEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->EDGLEVEL : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->EDGLEVEL)
#define GPIO_FILONOFF    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->FILONOFF : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->FILONOFF)
#define GPIO_INTMSKS     (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSKS  : 0)
#define GPIO_MSKCLRS     (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLRS  : 0)
#define GPIO_OUTDTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTSEL : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTSEL)
#define GPIO_OUTDTH      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTH   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTH)
#define GPIO_OUTDTL      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTL   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTL)
#define GPIO_BOTHEDGE    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->BOTHEDGE : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->BOTHEDGE)
#define GPIO_INEN        (uintptr_t)(gpio_dev->gen3_soc_type ? 0                                                  : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INEN)

uint32_t gpio_read(uintptr_t reg)
{
    if (reg) {
        return in32(reg);
    } else {
        return 0;
    }
}

void gpio_write(uintptr_t reg, uint32_t val)
{
    if (reg) {
        out32(reg, val);
    }
}

void gpio_pfc_set_gpio(uint32_t port_num)
{
    uint32_t tmp;

    /* Set GPSR to GPIO */
    tmp = gpio_read(PFC_GPSR);
    tmp &= ~port_num;
    gpio_write(PFC_PMMR, ~tmp);
    gpio_write(PFC_GPSR, tmp);

    /* V4H rev 1.0 limitation:
     * GP1_23 - 28 cannot be used when 1.8V voltage level is supplied to VDDQ18_33_SPI power rail
     * GP1_23 - 28 can be used when 3.3V voltage level is supplied to VDDQ18_33_SPI power rail */
    if ((gpio_dev->product_id == PRODUCT_ID_RCAR_V4H && gpio_dev->version_id == PRODUCT_REV_1_0) && 
        (gpio_dev->group == 1 && port_num >= 23 && port_num <= 28)) {
        tmp = gpio_read(PFC_POC);
        tmp |= port_num;
        gpio_write(PFC_PMMR, ~tmp);
        gpio_write(PFC_POC, tmp);
    }
}

int gpio_interrupt_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    tmp = gpio_read(GPIO_EDGLEVEL);
    if (data->edglevel == GPIO_EDGLEV_EDGE) {
        tmp |= port_num;
        gpio_write(GPIO_EDGLEVEL, tmp);

        tmp = gpio_read(GPIO_BOTHEDGE);
        if (data->bothedge == GPIO_BOTHEDGE_BOTH) {
            tmp |= port_num;
        } else if (data->bothedge == GPIO_BOTHEDGE_ONE) {
            tmp &= ~port_num;
        } else {
            printf("Invalid BOTHEDGE value\n");
            return (-1);
        }
        gpio_write(GPIO_BOTHEDGE, tmp);

        tmp = gpio_read(GPIO_IOINTSEL);
        tmp |= port_num;
        gpio_write(GPIO_IOINTSEL, tmp);

        tmp = gpio_read(GPIO_INTCLR);
        tmp |= port_num;
        gpio_write(GPIO_INTCLR, tmp);
    } else if (data->edglevel == GPIO_EDGLEV_LEVEL) {
        tmp &= ~port_num;
        gpio_write(GPIO_EDGLEVEL, tmp);

        tmp = gpio_read(GPIO_IOINTSEL);
        tmp |= port_num;
        gpio_write(GPIO_IOINTSEL, tmp);
    } else {
        printf("Invalid EDGLEV value\n");
        return (-1);
    }
    if (!gpio_dev->gen3_soc_type) {
        tmp = gpio_read(GPIO_INEN);
        tmp |= port_num;
        gpio_write(GPIO_INEN, tmp);
    }
    tmp = gpio_read(GPIO_MSKCLR);
    tmp |= port_num;
    gpio_write(GPIO_MSKCLR, tmp);

    data->value = gpio_dev->irq + data->port;

    return 0;
}

int gpio_pud_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    if (!gpio_dev->gen3_soc_type) {
        if (data->pud == GPIO_PUD_DIS) {
            // Disable pull up/down
            tmp = gpio_read(PFC_PUEN);
            tmp &= ~port_num;
            gpio_write(PFC_PMMR, ~tmp);
            gpio_write(PFC_PUEN, tmp);
            /* FIXME : Need delay ? */
            delay(50);
        } else {
            tmp = gpio_read(PFC_PUD);
            if(data->pud == GPIO_PUD_UP) {
                tmp |= port_num;
            } else if(data->pud == GPIO_PUD_DOWN) {
                tmp &= ~port_num;
            } else {
                printf("Invalid PUD value\n");
                return (-1);
            }
            gpio_write(PFC_PMMR, ~tmp);
            gpio_write(PFC_PUD, tmp);

            // Enable pull up/down
            tmp = gpio_read(PFC_PUEN);
            tmp |= port_num;
            gpio_write(PFC_PMMR, ~tmp);
            gpio_write(PFC_PUEN, tmp);
        }
    }

    return 0;
}

int gpio_ioint_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    if (data->ioint == GPIO_IOINT_INTERRUPT) {
        if (gpio_interrupt_setup(data, port_num)) {
            return (ENG);
        }
    } else if (data->ioint == GPIO_IOINT_INOUT) {
        tmp = gpio_read(GPIO_IOINTSEL);
        tmp &= ~port_num;
        gpio_write(GPIO_IOINTSEL, tmp);
    } else {
        printf("Invalid IOINT value\n");
        return (ENG);
    }

    return 0;
}

int gpio_inout_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    tmp = gpio_read(GPIO_IOINTSEL);
    tmp &= ~port_num;
    gpio_write(GPIO_IOINTSEL, tmp);

    tmp = gpio_read(GPIO_INOUTSEL);
    if (data->inout == GPIO_INOUT_OUT) {
        tmp |= port_num;
        gpio_write(GPIO_INOUTSEL, tmp);

        tmp = gpio_read(GPIO_OUTDTSEL);
        tmp &= ~port_num;
        gpio_write(GPIO_OUTDTSEL, tmp);

    } else if (data->inout == GPIO_INOUT_IN) {
        tmp &= ~port_num;
        gpio_write(GPIO_INOUTSEL, tmp);

        if (!gpio_dev->gen3_soc_type) {
            tmp = gpio_read(GPIO_INEN);
            tmp |= port_num;
            gpio_write(GPIO_INEN, tmp);
        }
    } else {
        printf("Invalid INOUT value\n");
        return (-1);
    }

    return 0;
}

int gpio_posneg_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    tmp = gpio_read(GPIO_POSNEG);
    if (data->posneg == GPIO_POSNEG_NEGATIVE) {
        tmp |= port_num;
    } else if (data->posneg == GPIO_POSNEG_POSITIVE) {
        tmp &= ~port_num;
    } else {
        printf("Invalid POSNEG value\n");
        return (-1);
    }

    gpio_write(GPIO_POSNEG, tmp);

    return 0;
}

int gpio_data_write(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    tmp = gpio_read(GPIO_OUTDT);
    if (data->value == GPIO_OUT_HIGH) {
        tmp |= port_num;				
    } else if (data->value == GPIO_OUT_LOW) {
        tmp &= ~port_num;
    } else {
        printf("Invalid OUTDT level\n");
        return (-1);			
    }
    gpio_write(GPIO_OUTDT, tmp);

    /* Read back and return */
    tmp = gpio_read(GPIO_OUTDT);
    if (tmp & port_num) {
        data->value = GPIO_OUT_HIGH;
    } else {
        data->value = GPIO_OUT_LOW;
    }

    return 0;
}

int gpio_data_read(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    /* Read and return data */
    tmp = gpio_read(GPIO_INDT);
    if (tmp & port_num) {
        data->value = GPIO_OUT_HIGH;
    } else {
        data->value = GPIO_OUT_LOW;
    }

    return 0;
}

int gpio_inen_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;
    if (!gpio_dev->gen3_soc_type) {
        tmp = gpio_read(GPIO_INEN);
        tmp |= port_num;
        gpio_write(GPIO_INEN, tmp);

    } else {
        printf("SoC no support\n");
        return (ENG);
    }

    return 0;
}

/** 
 * @brief dump_registers - dump register address and value of the register 
 * @param dev  Pointer to GPIO driver device structure.
 */
void dump_registers()
{
#if defined DUMP_REGISTERS
    if (!gpio_dev->gen3_soc_type) {
        gen4_gpio_reg_t *gpio_reg = (gen4_gpio_reg_t *)gpio_dev->gpio_reg;
        gen4_pfc_reg_t *pfc_reg   = (gen4_pfc_reg_t *)gpio_dev->pfc_reg;
        {
            gen4_pfc_reg_t *pfc_preg = (gen4_pfc_reg_t *)((uintptr_t)gpio_dev->phybase);
            gpio_slogf(2, "PFC registers:");
            gpio_slogf(2, "    PMMR: address = 0x%08lx, value = 0x%08x", (uintptr_t)&(pfc_preg->PMMR), pfc_reg->PMMR);
            gpio_slogf(2, "    GPSR: address = 0x%08lx, value = 0x%08x", (uintptr_t)&(pfc_preg->GPSR), pfc_reg->GPSR);
            gpio_slogf(2, "    PUEN: address = 0x%08lx, value = 0x%08x", (uintptr_t)&(pfc_preg->PUEN), pfc_reg->PUEN);
            gpio_slogf(2, "    PUD:  address = 0x%08lx, value = 0x%08x", (uintptr_t)&(pfc_preg->PUD), pfc_reg->PUD  );
        }
        {
            gen4_gpio_reg_t *gpio_preg = (gen4_gpio_reg_t *)((uintptr_t)gpio_dev->phybase + RCAR_GEN4_GPIO_IOINTSEL);
            gpio_slogf(2, "GPIO registers:");
            gpio_slogf(2, "    IOINTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->IOINTSEL , gpio_reg->IOINTSEL );
            gpio_slogf(2, "    INOUTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INOUTSEL , gpio_reg->INOUTSEL );
            gpio_slogf(2, "    OUTDT:    address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDT    , gpio_reg->OUTDT    );
            gpio_slogf(2, "    INDT:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INDT     , gpio_reg->INDT     );
            gpio_slogf(2, "    INTDT:    address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTDT    , gpio_reg->INTDT    );
            gpio_slogf(2, "    INTCLR:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTCLR   , gpio_reg->INTCLR   );
            gpio_slogf(2, "    INTMSK:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTMSK   , gpio_reg->INTMSK   );
            gpio_slogf(2, "    MSKCLR:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->MSKCLR   , gpio_reg->MSKCLR   );
            gpio_slogf(2, "    POSNEG:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->POSNEG   , gpio_reg->POSNEG   );
            gpio_slogf(2, "    EDGLEVEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->EDGLEVEL , gpio_reg->EDGLEVEL );
            gpio_slogf(2, "    FILONOFF: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->FILONOFF , gpio_reg->FILONOFF );
            gpio_slogf(2, "    FILCLKSEL:address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->FILCLKSEL, gpio_reg->FILCLKSEL);
            gpio_slogf(2, "    OUTDTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTSEL , gpio_reg->OUTDTSEL );
            gpio_slogf(2, "    OUTDTH:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTH   , gpio_reg->OUTDTH   );
            gpio_slogf(2, "    OUTDTL:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTL   , gpio_reg->OUTDTL   );
            gpio_slogf(2, "    BOTHEDGE: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->BOTHEDGE , gpio_reg->BOTHEDGE );
            gpio_slogf(2, "    INEN:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INEN     , gpio_reg->INEN     );
        }
    }
    else {
        gen3_gpio_reg_t *gpio_reg = (gen3_gpio_reg_t *)gpio_dev->gpio_reg;
        gen3_pfc_reg_t *pfc_reg   = (gen3_pfc_reg_t *)gpio_dev->pfc_reg;
        {
           gen3_pfc_reg_t *pfc_preg   = (gen3_pfc_reg_t *)((uintptr_t)RCAR_GEN3_PFC_BASE);
           gpio_slogf(2, "PFC registers");
           gpio_slogf(2, "    PMMR:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PMMR, pfc_reg->PMMR);
           gpio_slogf(2, "    GPSR[%d]: address = 0x%08lx, value = 0x%08x", gpio_dev->group, (uintptr_t)&pfc_preg->GPSR[gpio_dev->group], pfc_reg->GPSR[gpio_dev->group]);
        
        }
        {
           gen3_gpio_reg_t *gpio_preg   = (gen3_gpio_reg_t *)((uintptr_t)gpio_dev->phybase);
           gpio_slogf(2, "GPIO registers:");
           gpio_slogf(2, "    IOINTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->IOINTSEL , gpio_reg->IOINTSEL);
           gpio_slogf(2, "    INOUTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INOUTSEL , gpio_reg->INOUTSEL);
           gpio_slogf(2, "    OUTDT:    address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDT    , gpio_reg->OUTDT   );
           gpio_slogf(2, "    INDT:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INDT     , gpio_reg->INDT    );
           gpio_slogf(2, "    INTDT:    address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTDT    , gpio_reg->INTDT   );
           gpio_slogf(2, "    INTCLR:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTCLR   , gpio_reg->INTCLR  );
           gpio_slogf(2, "    INTMSK:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTMSK   , gpio_reg->INTMSK  );
           gpio_slogf(2, "    MSKCLR:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->MSKCLR   , gpio_reg->MSKCLR  );
           gpio_slogf(2, "    POSNEG:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->POSNEG   , gpio_reg->POSNEG  );
           gpio_slogf(2, "    EDGLEVEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->EDGLEVEL , gpio_reg->EDGLEVEL);
           gpio_slogf(2, "    FILONOFF: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->FILONOFF , gpio_reg->FILONOFF);
           gpio_slogf(2, "    INTMSKS:  address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTMSKS  , gpio_reg->INTMSKS );
           gpio_slogf(2, "    MSKCLRS:  address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->MSKCLRS  , gpio_reg->MSKCLRS );
           gpio_slogf(2, "    OUTDTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTSEL , gpio_reg->OUTDTSEL);
           gpio_slogf(2, "    OUTDTH:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTH   , gpio_reg->OUTDTH  );
           gpio_slogf(2, "    OUTDTL:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTL   , gpio_reg->OUTDTL  );
           gpio_slogf(2, "    BOTHEDGE: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->BOTHEDGE , gpio_reg->BOTHEDGE);
        }   
    }
#endif
}
