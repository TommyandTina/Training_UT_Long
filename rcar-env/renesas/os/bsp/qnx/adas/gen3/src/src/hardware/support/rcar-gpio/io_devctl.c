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
#include "proto.h"

extern gpio_drv_info_t *gpio_dev;

#define PMMR        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_pfc_reg_t *)gpio_dev->pfc_reg)->PMMR : &((v3u_pfc_reg_t *)gpio_dev->pfc_reg)->PMMR)
#define GPSR        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_pfc_reg_t *)gpio_dev->pfc_reg)->GPSR[gpio_dev->group] : &((v3u_pfc_reg_t *)gpio_dev->pfc_reg)->GPSR)
#define PUEN        (uintptr_t)(gpio_dev->gen3_soc_type ? 0 : &((v3u_pfc_reg_t *)gpio_dev->pfc_reg)->PUEN)
#define PUD         (uintptr_t)(gpio_dev->gen3_soc_type ? 0 : &((v3u_pfc_reg_t *)gpio_dev->pfc_reg)->PUD)

#define IOINTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->IOINTSEL : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->IOINTSEL)
#define INOUTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INOUTSEL : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->INOUTSEL)
#define OUTDT       (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDT    : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDT)
#define INDT        (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INDT     : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->INDT)
#define INTDT       (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTDT    : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->INTDT)
#define INTCLR      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTCLR   : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->INTCLR)
#define INTMSK      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSK   : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSK)
#define MSKCLR      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLR   : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLR)
#define POSNEG      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->POSNEG   : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->POSNEG)
#define EDGLEVEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->EDGLEVEL : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->EDGLEVEL)
#define FILONOFF    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->FILONOFF : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->FILONOFF)
#define INTMSKS     (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->INTMSKS  : 0)
#define MSKCLRS     (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->MSKCLRS  : 0)
#define OUTDTSEL    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTSEL : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTSEL)
#define OUTDTH      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTH   : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTH)
#define OUTDTL      (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTL   : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->OUTDTL)
#define BOTHEDGE    (uintptr_t)(gpio_dev->gen3_soc_type ? &((gen3_gpio_reg_t *)gpio_dev->gpio_reg)->BOTHEDGE : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->BOTHEDGE)
#define INEN        (uintptr_t)(gpio_dev->gen3_soc_type ? 0                                                  : &((v3u_gpio_reg_t *)gpio_dev->gpio_reg)->INEN)

static uint32_t gpio_read(uintptr_t reg)
{
    if (reg) {
        return in32(reg);
    } else {
        return 0;
    }
}

static void gpio_write(uintptr_t reg, uint32_t val)
{
    if (reg) {
        out32(reg, val);
    }
}

static void gpio_pfc_set_gpio(uint32_t port_num)
{
    uint32_t tmp;

    /* Set GPSR to GPIO */
    tmp = gpio_read(GPSR);
    tmp &= ~port_num;
    gpio_write(PMMR, ~tmp);
    gpio_write(GPSR, tmp);
}

static int gpio_interrupt_setup(gpio_cmd_t *data, uint32_t port_num)
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

        if (!gpio_dev->gen3_soc_type) {
            tmp = gpio_read(INEN);
            tmp |= port_num;
            gpio_write(INEN, tmp);
        }
        
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

    return 0;
}

static int gpio_pud_setup(gpio_cmd_t *data, uint32_t port_num)
{
    uint32_t tmp;

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

    return 0;
}

static int gpio_inout_setup(gpio_cmd_t *data, uint32_t port_num)
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

static int gpio_posneg_setup(gpio_cmd_t *data, uint32_t port_num)
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

static int gpio_data_write(gpio_cmd_t *data, uint32_t port_num)
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


static int gpio_data_read(gpio_cmd_t *data, uint32_t port_num)
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

/** 
 * @brief io_devctl - This routine gets called whenever the client does "devctl"
 * calls.  It is responsible for decoding the command sent via
 * the "dcmd" member, and then taking the appropriate action,
 * either getting data from the structure and putting it into
 * the global variables in the program, or the reverse.
 * Note: that SETIOV is used to setup an array of pointers to
 * the actual data.  Don't give SETIOV pointers to local variables!!!
 *
 * @param opts:
 * @param msg:
 * @param ocb: 
 * @return EOK - No error
 *         ENG - Error
 */
int io_devctl(resmgr_context_t* ctp, io_devctl_t* msg, RESMGR_OCB_T* ocb)
{
    gpio_cmd_t *data = NULL;
    int status;
    uint32_t port_num;
    uint32_t tmp;

    status = iofunc_devctl_default(ctp, msg, ocb);
    if( status != _RESMGR_DEFAULT ){
        perror("iofunc_devctl_default");
        return(ENG);
    }

    data = (gpio_cmd_t*) _DEVCTL_DATA(msg->i);

    if(data->port >= GPIO_PORT) {
        printf ("Invalid port number\n");
        return(ENG);
    }

    port_num = (1 << (data->port));

    switch( msg->i.dcmd ){
        case IOCTL_GPIO_SET_PORT:
            gpio_pfc_set_gpio(port_num);
            dump_registers(gpio_dev);
            break;
        case IOCTL_GPIO_SET_IOINT:
            if(data->ioint == GPIO_IOINT_INTERRUPT) {
                if (gpio_interrupt_setup(data, port_num)) {
                    return (ENG);
                }

                data->value = gpio_dev->irq + data->port;
                break;
            } else if(data->ioint == GPIO_IOINT_INOUT) {
                tmp = gpio_read(IOINTSEL);
                tmp &= ~port_num;
                gpio_write(IOINTSEL, tmp);
            } else {
                printf("Invalid IOINT value\n");
                return (ENG);
            }
            dump_registers(gpio_dev);
            break;
        case IOCTL_GPIO_SET_INOUT:
            if(gpio_inout_setup(data, port_num)) {
                return (ENG);
            }
            dump_registers(gpio_dev);
            break;
        case IOCTL_GPIO_SET_INEN: //Only for V3U
            if (!gpio_dev->gen3_soc_type) {
                tmp = gpio_read(INEN);
                tmp |= port_num;
                gpio_write(INEN, tmp);
            } else {
                printf("SoC no support\n");
                return (ENG);
            }
            dump_registers(gpio_dev);
            break;
        case IOCTL_GPIO_SET_PUD: //Only for V3U
            if (!gpio_dev->gen3_soc_type) {
                if (gpio_pud_setup(data, port_num)) {
                    return (ENG);
                }
            } else {
                printf("SoC no support\n");
                return (ENG);
            }
            dump_registers(gpio_dev);
            break;
        case IOCTL_GPIO_SET_POSNEG:
            if (gpio_posneg_setup(data, port_num)) {
                return (ENG);
            }
            dump_registers(gpio_dev);
            break;
        case IOCTL_GPIO_WRITE:
            if(gpio_data_write(data, port_num)) {
                return (ENG);
            }
            break;
        case IOCTL_GPIO_READ:
            if(gpio_data_read(data, port_num)) {
                return (ENG);
            }
            break;
        case IOCTL_GPIO_INIT:
            /* Set GPSR to GPIO */
            gpio_pfc_set_gpio(port_num);

            /* Set PUD */
            if (!gpio_dev->gen3_soc_type) {
                if (gpio_pud_setup(data, port_num)) {
                    return (ENG);
                }
            }

            /* Set Positive/negative logic */
            if (gpio_posneg_setup(data, port_num)) {
                return (ENG);
            }

            if(data->ioint == GPIO_IOINT_INTERRUPT) {
                /* Set Interrupt mode */
                if (gpio_interrupt_setup(data, port_num)) {
                    return (ENG);
                }

                data->value = gpio_dev->irq + data->port;
            } else if(data->ioint == GPIO_IOINT_INOUT) {
                /* Input/Output mode */
                if(gpio_inout_setup(data, port_num)) {
                    return (ENG);
                }
                if (data->inout == GPIO_INOUT_OUT) {
                    if(gpio_data_write(data, port_num)) {
                        return (ENG);
                    }
                } else if(data->inout == GPIO_INOUT_IN) {
                    if(gpio_data_read(data, port_num)) {
                        return (ENG);
                    }
                } else {
                    printf("Invalid IOINT value\n");
                    return (ENG);			
                }
            }
            dump_registers(gpio_dev);
            break;
        default:
            printf ("Illegal command\n");	
            return(ENG);
    }

    memset(&msg->o, 0, sizeof(msg->o));
    msg->o.ret_val = EOK;
    msg->o.nbytes = sizeof(gpio_cmd_t);

    return(_RESMGR_PTR(ctp, &msg->o, sizeof(msg->o) + msg->o.nbytes));
}
