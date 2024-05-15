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

#include "hw/inout.h"
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/hwinfo.h>
#include <drvr/hwinfo.h>
#include <aarch64/r-car.h>   
#include <hw/hwinfo_rcar.h>  
#include <proto.h>


extern gpio_drv_info_t *gpio_dev;

#define PMMR        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_pfc_reg_t *)gpio_dev->pfc_reg)->PMMR : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->PMMR)
#define GPSR        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_pfc_reg_t *)gpio_dev->pfc_reg)->GPSR[gpio_dev->group] : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->GPSR)
#define PUEN        (uintptr_t)(gpio_dev->gen3_soc_type ? 0 : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->PUEN)
#define PUD         (uintptr_t)(gpio_dev->gen3_soc_type ? 0 : &((gen4_pfc_reg_t *)gpio_dev->pfc_reg)->PUD)

#define IOINTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->IOINTSEL : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->IOINTSEL)
#define INOUTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INOUTSEL : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INOUTSEL)
#define OUTDT       (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDT    : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDT)
#define INDT        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INDT     : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INDT)
#define INTDT       (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTDT    : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INTDT)
#define INTCLR      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTCLR   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INTCLR)
#define INTMSK      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSK   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSK)
#define MSKCLR      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLR   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLR)
#define POSNEG      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->POSNEG   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->POSNEG)
#define EDGLEVEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->EDGLEVEL : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->EDGLEVEL)
#define FILONOFF    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->FILONOFF : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->FILONOFF)
#define INTMSKS     (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSKS  : 0)
#define MSKCLRS     (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLRS  : 0)
#define OUTDTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTSEL : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTSEL)
#define OUTDTH      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTH   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTH)
#define OUTDTL      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTL   : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTL)
#define BOTHEDGE    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->BOTHEDGE : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->BOTHEDGE)
#define INEN        (uintptr_t)(gpio_dev->gen3_soc_type ? 0                                                  : &((gen4_gpio_reg_t *)gpio_dev->gpio_reg)->INEN)

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
    tmp = gpio_read(GPSR);
    tmp &= ~port_num;
    gpio_write(PMMR, ~tmp);
    gpio_write(GPSR, tmp);
}

int gpio_interrupt_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    tmp = gpio_read(EDGLEVEL);
    if(data->edglevel == GPIO_EDGLEV_EDGE) {
        tmp |= port_num;
        gpio_write(EDGLEVEL, tmp);

        tmp = gpio_read(BOTHEDGE);
        if(data->bothedge == GPIO_BOTHEDGE_BOTH) {
            tmp |= port_num;
        } else if(data->bothedge == GPIO_BOTHEDGE_ONE) {
            tmp &= ~port_num;
        } else {
            printf("Invalid BOTHEDGE value\n");
            return (-1);
        }
        gpio_write(BOTHEDGE, tmp);

        tmp = gpio_read(IOINTSEL);
        tmp |= port_num;
        gpio_write(IOINTSEL, tmp);

        tmp = gpio_read(INTCLR);
        tmp |= port_num;
        gpio_write(INTCLR, tmp);
    } else if(data->edglevel == GPIO_EDGLEV_LEVEL) {
        tmp &= ~port_num;
        gpio_write(EDGLEVEL, tmp);

        tmp = gpio_read(IOINTSEL);
        tmp |= port_num;
        gpio_write(IOINTSEL, tmp);
    } else {
        printf("Invalid EDGLEV value\n");
        return (-1);
    }

    tmp = gpio_read(MSKCLR);
    tmp |= port_num;
    gpio_write(MSKCLR, tmp);

    data->value = gpio_dev->irq + data->port;

    return 0;
}

int gpio_pud_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    if (!gpio_dev->gen3_soc_type) {
        if(data->pud == GPIO_PUD_DIS) {
            // Disable pull up/down
            tmp = gpio_read(PUEN);
            tmp &= ~port_num;
            gpio_write(PUEN, tmp);
        } else {
            tmp = gpio_read(PUD);
            if(data->pud == GPIO_PUD_UP) {
                tmp |= port_num;
            } else if(data->pud == GPIO_PUD_DOWN) {
                tmp &= ~port_num;
            } else {
                printf("Invalid PUD value\n");
                return (-1);
            }
            gpio_write(PUD, tmp);

            // Enable pull up/down
            tmp = gpio_read(PUEN);
            tmp |= port_num;
            gpio_write(PUEN, tmp);
        }
    }

    return 0;
}

int gpio_ioint_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    if(data->ioint == GPIO_IOINT_INTERRUPT) {
        if (gpio_interrupt_setup(data, port_num)) {
            return (ENG);
        }
    } else if(data->ioint == GPIO_IOINT_INOUT) {
        tmp = gpio_read(IOINTSEL);
        tmp &= ~port_num;
        gpio_write(IOINTSEL, tmp);
    } else {
        printf("Invalid IOINT value\n");
        return (ENG);
    }

    return 0;
}

int gpio_inout_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    tmp = gpio_read(IOINTSEL);
    tmp &= ~port_num;
    gpio_write(IOINTSEL, tmp);

    tmp = gpio_read(INOUTSEL);
    if(data->inout == GPIO_INOUT_OUT) {
        tmp |= port_num;
        gpio_write(INOUTSEL, tmp);

        tmp = gpio_read(OUTDTSEL);
        tmp &= ~port_num;
        gpio_write(OUTDTSEL, tmp);

    } else if(data->inout == GPIO_INOUT_IN) {
        tmp &= ~port_num;
        gpio_write(INOUTSEL, tmp);

        if (!gpio_dev->gen3_soc_type) {
            tmp = gpio_read(INEN);
            tmp |= port_num;
            gpio_write(INEN, tmp);
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

    tmp = gpio_read(POSNEG);
    if(data->posneg == GPIO_POSNEG_NEGATIVE) {
        tmp |= port_num;
    } else if(data->posneg == GPIO_POSNEG_POSITIVE) {
        tmp &= ~port_num;
    } else {
        printf("Invalid POSNEG value\n");
        return (-1);
    }
    gpio_write(POSNEG, tmp);

    return 0;
}

int gpio_data_write(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

    tmp = gpio_read(OUTDT);
    if(data->value == GPIO_OUT_HIGH) {
        tmp |= port_num;				
    } else if(data->value == GPIO_OUT_LOW) {
        tmp &= ~port_num;
    } else {
        printf("Invalid OUTDT level\n");
        return (-1);			
    }
    gpio_write(OUTDT, tmp);

    /* Read back and return */
    tmp = gpio_read(OUTDT);
    if(tmp & port_num) {
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
    tmp = gpio_read(INDT);
    if(tmp & port_num) {
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
        tmp = gpio_read(INEN);
        tmp |= port_num;
        gpio_write(INEN, tmp);
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
    if (!dev->gen3_soc_type) {
        gen4_gpio_reg_t *gpio_reg = (gen4_gpio_reg_t *)gpio_dev->gpio_reg;
        gen4_pfc_reg_t *pfc_reg   = (gen4_pfc_reg_t *)gpio_dev->pfc_reg;
        {
            gen4_pfc_reg_t *pfc_preg = (gen4_pfc_reg_t *)((uintptr_t)gpio_dev->phybase);
            gpio_slogf(1, "PFC registers:");
            gpio_slogf(1, "    PMMR: address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PMMR, pfc_reg->PMMR);
            gpio_slogf(1, "    GPSR: address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->GPSR, pfc_reg->GPSR);
            gpio_slogf(1, "    PUEN: address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PUEN, pfc_reg->PUEN);
            gpio_slogf(1, "    PUD:  address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PUD, pfc_reg->PUD  );
        }
        {
            gen4_gpio_reg_t *gpio_preg = (gen4_gpio_reg_t *)((uintptr_t)gpio_dev->phybase + RCAR_V3U_GPIO_IOINTSEL);
            gpio_slogf(1, "GPIO registers:");
            gpio_slogf(1, "    IOINTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->IOINTSEL , gpio_reg->IOINTSEL );
            gpio_slogf(1, "    INOUTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INOUTSEL , gpio_reg->INOUTSEL );
            gpio_slogf(1, "    OUTDT:    address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDT    , gpio_reg->OUTDT    );
            gpio_slogf(1, "    INDT:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INDT     , gpio_reg->INDT     );
            gpio_slogf(1, "    INTDT:    address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTDT    , gpio_reg->INTDT    );
            gpio_slogf(1, "    INTCLR:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTCLR   , gpio_reg->INTCLR   );
            gpio_slogf(1, "    INTMSK:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTMSK   , gpio_reg->INTMSK   );
            gpio_slogf(1, "    MSKCLR:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->MSKCLR   , gpio_reg->MSKCLR   );
            gpio_slogf(1, "    POSNEG:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->POSNEG   , gpio_reg->POSNEG   );
            gpio_slogf(1, "    EDGLEVEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->EDGLEVEL , gpio_reg->EDGLEVEL );
            gpio_slogf(1, "    FILONOFF: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->FILONOFF , gpio_reg->FILONOFF );
            gpio_slogf(1, "    FILCLKSEL:address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->FILCLKSEL, gpio_reg->FILCLKSEL);
            gpio_slogf(1, "    OUTDTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTSEL , gpio_reg->OUTDTSEL );
            gpio_slogf(1, "    OUTDTH:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTH   , gpio_reg->OUTDTH   );
            gpio_slogf(1, "    OUTDTL:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTL   , gpio_reg->OUTDTL   );
            gpio_slogf(1, "    BOTHEDGE: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->BOTHEDGE , gpio_reg->BOTHEDGE );
            gpio_slogf(1, "    INEN:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INEN     , gpio_reg->INEN     );
        }
    }
    else {
        gen3_gpio_reg_t *gpio_reg = (gen3_gpio_reg_t *)gpio_dev->gpio_reg;
        gen3_pfc_reg_t *pfc_reg   = (gen3_pfc_reg_t *)gpio_dev->pfc_reg;
        {
            gen3_pfc_reg_t *pfc_preg   = (gen3_pfc_reg_t *)((uintptr_t)RCAR_GEN3_PFC_BASE);
            gpio_slogf(1, "PFC registers");
            gpio_slogf(1, "    PMMR:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PMMR, pfc_reg->PMMR);
            gpio_slogf(1, "    GPSR[%d]: address = 0x%08lx, value = 0x%08x", gpio_dev->group, (uintptr_t)&pfc_preg->GPSR[gpio_dev->group], pfc_reg->GPSR[gpio_dev->group]);
        }
        {
            gen3_gpio_reg_t *gpio_preg   = (gen3_gpio_reg_t *)((uintptr_t)gpio_dev->phybase);
            gpio_slogf(1, "GPIO registers:");
            gpio_slogf(1, "    IOINTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->IOINTSEL , gpio_reg->IOINTSEL);
            gpio_slogf(1, "    INOUTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INOUTSEL , gpio_reg->INOUTSEL);
            gpio_slogf(1, "    OUTDT:    address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDT    , gpio_reg->OUTDT   );
            gpio_slogf(1, "    INDT:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INDT     , gpio_reg->INDT    );
            gpio_slogf(1, "    INTDT:    address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTDT    , gpio_reg->INTDT   );
            gpio_slogf(1, "    INTCLR:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTCLR   , gpio_reg->INTCLR  );
            gpio_slogf(1, "    INTMSK:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTMSK   , gpio_reg->INTMSK  );
            gpio_slogf(1, "    MSKCLR:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->MSKCLR   , gpio_reg->MSKCLR  );
            gpio_slogf(1, "    POSNEG:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->POSNEG   , gpio_reg->POSNEG  );
            gpio_slogf(1, "    EDGLEVEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->EDGLEVEL , gpio_reg->EDGLEVEL);
            gpio_slogf(1, "    FILONOFF: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->FILONOFF , gpio_reg->FILONOFF);
            gpio_slogf(1, "    INTMSKS:  address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->INTMSKS  , gpio_reg->INTMSKS );
            gpio_slogf(1, "    MSKCLRS:  address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->MSKCLRS  , gpio_reg->MSKCLRS );
            gpio_slogf(1, "    OUTDTSEL: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTSEL , gpio_reg->OUTDTSEL);
            gpio_slogf(1, "    OUTDTH:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTH   , gpio_reg->OUTDTH  );
            gpio_slogf(1, "    OUTDTL:   address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->OUTDTL   , gpio_reg->OUTDTL  );
            gpio_slogf(1, "    BOTHEDGE: address = 0x%08lx, value = 0x%08x", (uintptr_t)&gpio_preg->BOTHEDGE , gpio_reg->BOTHEDGE);
        }
    }
#endif
}
