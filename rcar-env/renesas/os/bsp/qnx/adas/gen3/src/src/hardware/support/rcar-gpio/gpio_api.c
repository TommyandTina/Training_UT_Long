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

#include <stdbool.h>
#include <sys/mman.h>
#include <sys/hwinfo.h>
#include <drvr/hwinfo.h>
#include <aarch64/r-car-gen3.h>   
#include <hw/hwinfo_rcar.h>  
#include <proto.h>

struct gen3_base_irq_s {
    uint32_t base;
    int irq;
};

struct gen3_base_irq_s v3u_base[] = { 
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP0, .irq = 992 + (32 * 0) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP1, .irq = 992 + (32 * 1) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP2, .irq = 992 + (32 * 2) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP3, .irq = 992 + (32 * 3) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP4, .irq = 992 + (32 * 4) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP5, .irq = 992 + (32 * 5) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP6, .irq = 992 + (32 * 6) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP7, .irq = 992 + (32 * 7) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP8, .irq = 992 + (32 * 8) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_V3U_GPIO_DM0_BASE + RCAR_V3U_GPIO_GP9, .irq = 992 + (32 * 9) }, /* this irq is 'vector base' in init_intrinfo.c */
};

struct gen3_base_irq_s gen3_base[] = { 
    { .base = RCAR_GEN3_GPIO0_BASE, .irq = 512 + (32 * 0) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_GEN3_GPIO1_BASE, .irq = 512 + (32 * 1) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_GEN3_GPIO2_BASE, .irq = 512 + (32 * 2) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_GEN3_GPIO3_BASE, .irq = 512 + (32 * 3) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_GEN3_GPIO4_BASE, .irq = 512 + (32 * 4) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_GEN3_GPIO5_BASE, .irq = 512 + (32 * 5) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_GEN3_GPIO6_BASE, .irq = 512 + (32 * 6) }, /* this irq is 'vector base' in init_intrinfo.c */
    { .base = RCAR_GEN3_GPIO7_BASE, .irq = 512 + (32 * 7) }, /* this irq is 'vector base' in init_intrinfo.c */
};

/** 
 * @brief dump_registers - dump register address and value of the register 
 * @param dev  Pointer to GPIO driver device structure.
 */
void dump_registers(gpio_drv_info_t *dev) 
{
#if defined DUMP_REGISTERS
    if (!dev->gen3_soc_type) {
        v3u_gpio_reg_t *gpio_reg = (v3u_gpio_reg_t *)dev->gpio_reg;
        v3u_pfc_reg_t *pfc_reg   = (v3u_pfc_reg_t *)dev->pfc_reg;
        {
            v3u_pfc_reg_t *pfc_preg = (v3u_pfc_reg_t *)((uintptr_t)dev->phybase);
            gpio_slogf(1, "PFC registers:");
            gpio_slogf(1, "    PMMR: address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PMMR, pfc_reg->PMMR);
            gpio_slogf(1, "    GPSR: address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->GPSR, pfc_reg->GPSR);
            gpio_slogf(1, "    PUEN: address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PUEN, pfc_reg->PUEN);
            gpio_slogf(1, "    PUD:  address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PUD, pfc_reg->PUD  );
        }
        {
            v3u_gpio_reg_t *gpio_preg = (v3u_gpio_reg_t *)((uintptr_t)dev->phybase + RCAR_V3U_GPIO_IOINTSEL);
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
        gen3_gpio_reg_t *gpio_reg = (gen3_gpio_reg_t *)dev->gpio_reg;
        gen3_pfc_reg_t *pfc_reg   = (gen3_pfc_reg_t *)dev->pfc_reg;
        {
            gen3_pfc_reg_t *pfc_preg   = (gen3_pfc_reg_t *)((uintptr_t)RCAR_GEN3_PFC_BASE);
            gpio_slogf(1, "PFC registers");
            gpio_slogf(1, "    PMMR:     address = 0x%08lx, value = 0x%08x", (uintptr_t)&pfc_preg->PMMR, pfc_reg->PMMR);
            gpio_slogf(1, "    GPSR[%d]: address = 0x%08lx, value = 0x%08x", dev->group, (uintptr_t)&pfc_preg->GPSR[dev->group], pfc_reg->GPSR[dev->group]);
        }
        {
            gen3_gpio_reg_t *gpio_preg   = (gen3_gpio_reg_t *)((uintptr_t)dev->phybase);
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


/** 
 * @brief gpio_init - initialize GPIO.
 * @param dev: Store information related to gpio for processing.
 * @return :
 *      EOK - No error
 *      Other - Error
 */
int gpio_init(gpio_drv_info_t *dev) 
{
    if (!dev->phybase) {
        if (!dev->gen3_soc_type) { //for V3U
            dev->phybase = v3u_base[dev->group].base;
         } else { //for other Gen3
            dev->phybase = gen3_base[dev->group].base;
        }
    }

    if (!dev->irq) {
        if (!dev->gen3_soc_type) { //for V3U
            dev->irq = v3u_base[dev->group].irq;
         } else { //for other Gen3
            dev->irq = gen3_base[dev->group].irq;
        }
    }

    gpio_slogf(1, "GPIO base = 0x%08x, irq = %d", dev->phybase, dev->irq);

    if (!dev->gen3_soc_type) { //for V3U
        if ((dev->pfc_reg = mmap_device_io(RCAR_V3U_GPIO_SIZE, dev->phybase)) == MAP_DEVICE_FAILED) {
            perror("mmap_device_io with regbase");
            return -ENOMEM;
        } 
        dev->gpio_reg = (dev->pfc_reg + RCAR_V3U_GPIO_IOINTSEL);
     } else { //for other Gen3
        if ((dev->gpio_reg = mmap_device_io(RCAR_GEN3_GPIO_SIZE, dev->phybase)) ==  MAP_DEVICE_FAILED) {
            perror("mmap_device_io with regbase");
            return -ENOMEM;
        }
        if ((dev->pfc_reg = mmap_device_io(RCAR_GEN3_PFC_SIZE, RCAR_GEN3_PFC_BASE)) == MAP_DEVICE_FAILED) {
            munmap_device_io(dev->gpio_reg, RCAR_GEN3_GPIO_SIZE);
            perror("mmap_device_io with regbase");
            return -ENOMEM;
        }
     }


    return EOK;
}


/** 
 * @brief gpio_fini - GPIO finish.
 * @param dev: Store information related to gpio for processing.
 */
void gpio_fini (gpio_drv_info_t *dev) {
    
    if (!dev->gen3_soc_type){ //  For V3U
        munmap_device_io (dev->pfc_reg, RCAR_V3U_GPIO_SIZE);
    } else { // For other Gen3
        munmap_device_io (dev->pfc_reg, RCAR_GEN3_PFC_SIZE);
        munmap_device_io (dev->gpio_reg, RCAR_GEN3_GPIO_SIZE);
     }
}

