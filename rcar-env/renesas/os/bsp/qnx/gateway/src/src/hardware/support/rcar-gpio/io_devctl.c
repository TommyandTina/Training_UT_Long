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


#include "proto.h"

extern void gpio_pfc_set_gpio(uint32_t port_num);
extern int gpio_interrupt_setup(gpio_cmd_t *data, uint32_t port_num);
extern int gpio_pud_setup(gpio_cmd_t *data, uint32_t port_num);
extern int gpio_ioint_setup(gpio_cmd_t *data, uint32_t port_num);
extern int gpio_inout_setup(gpio_cmd_t *data, uint32_t port_num);
extern int gpio_posneg_setup(gpio_cmd_t *data, uint32_t port_num);
extern int gpio_data_write(gpio_cmd_t *data, uint32_t port_num);
extern int gpio_data_read(gpio_cmd_t *data, uint32_t port_num);
extern int gpio_inen_setup(gpio_cmd_t *data, uint32_t port_num);
extern void dump_registers();
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
            dump_registers();
            break;
        case IOCTL_GPIO_SET_IOINT:
            if (gpio_ioint_setup(data, port_num)) {
                return (ENG);
            }
            dump_registers();
            break;
        case IOCTL_GPIO_SET_INOUT:
            if(gpio_inout_setup(data, port_num)) {
                return (ENG);
            }
            dump_registers();
            break;
        case IOCTL_GPIO_SET_INEN: //Only for V3U
            if (gpio_inen_setup(data, port_num)) {
                return (ENG);
            }
            dump_registers();
            break;
        case IOCTL_GPIO_SET_PUD: //Only for V3U
            if (gpio_pud_setup(data, port_num)) {
                return (ENG);
            }
            dump_registers();
            break;
        case IOCTL_GPIO_SET_POSNEG:
            if (gpio_posneg_setup(data, port_num)) {
                return (ENG);
            }
            dump_registers();
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
            if (gpio_pud_setup(data, port_num)) {
                return (ENG);
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
            dump_registers();
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
