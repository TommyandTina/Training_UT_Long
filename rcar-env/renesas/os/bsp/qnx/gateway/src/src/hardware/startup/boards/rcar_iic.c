/*
* $QNXLicenseC:
* Copyright (c) 2018, QNX Software Systems. All Rights Reserved.
* Copyright 2021, Renesas Electronics Corporation.
*
* You must obtain a written license from and pay applicable license fees to QNX
* Software Systems before you may reproduce, modify or distribute this software,
* or any work that includes all or part of this software. Free development
* licenses are available for evaluation and non-commercial purposes. For more
* information visit http://licensing.qnx.com or email licensing@qnx.com.
*
* This file may contain contributions from others. Please review this entire
* file for other proprietary rights or license notices, as well as the QNX
* Development Suite License Guide at http://licensing.qnx.com/license-guide/
* for other information.
* $
*/

#include "startup.h"
#include <aarch64/r-car.h>

/* IIC commands */
#define IIC_CMD_START                   0x94        /* START condition */
#define IIC_CMD_STOP_TX                 0x90        /* STOP Write */
#define IIC_CMD_TX_TO_RX                0x81        /* Transmission to Receive mode change */
#define IIC_CMD_STOP_RX                 0xC0        /* STOP Read */

/* IIC registers, offset from base */
#define IIC_ICDR                        0x0000      // Data Register
#define IIC_ICCR                        0x0004      // Control Register
#define IIC_ICSR                        0x0008      // Status Register
#define IIC_ICIC                        0x000C      // Interrupt Control Register
#define IIC_ICCL                        0x0010      // Clock Control Register Low
#define IIC_ICCH                        0x0014      // Clock Control Register High

/* ICIC/ICSR bit definition */
#define IIC_BUSY                        (1 << 4)    // Transmit Busy
#define IIC_AL                          (1 << 3)    // Arbitration Lost
#define IIC_TACK                        (1 << 2)    // Transmit Acknowledge
#define IIC_WAIT                        (1 << 1)    // Wait
#define IIC_DTE                         (1 << 0)    // Data Transmit Enable

#define IIC_READ                        1
#define IIC_WRITE                       0
#define IIC_RETRY                       100
//#define IIC_DEBUG


#ifdef IIC_DEBUG
static void iic_debug(const char * debug_str)
{
    uint8_t icic, icsr;

    icic = in8(RCAR_IIC_BASE + IIC_ICIC);
    icsr = in8(RCAR_IIC_BASE + IIC_ICSR);
    kprintf("%s\n", debug_str);
    kprintf("ICIC=0x%x BUSY=%d WAIT=%d DTE=%d\n", icic, (icic >> 4) & 0x1, (icic >> 1) & 0x1, (icic >> 0) & 0x1);
    kprintf("ICSR=0x%x BUSY=%d WAIT=%d DTE=%d\n", icsr, (icsr >> 4) & 0x1, (icsr >> 1) & 0x1, (icsr >> 0) & 0x1);
}
#endif

static uint8_t iic_wait(uint8_t flag)
{
    unsigned try = IIC_RETRY;
    uint8_t reg;

    do {
        reg = in8(RCAR_IIC_BASE + IIC_ICSR);
        try--;
    } while(try && !(reg & flag));

    if(!try) {
#ifdef IIC_DEBUG
        kprintf("iic_wait %s failed\n", (flag == IIC_WAIT)?"IIC_WAIT":"IIC_DTE");
#endif
        return 1;
    }

    return 0;
}


int iic_write_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    int i;

    out8(RCAR_IIC_BASE + IIC_ICCR, 0x01);
    out8(RCAR_IIC_BASE + IIC_ICCR, 0x81);

    out8(RCAR_IIC_BASE + IIC_ICCL, 0x07);
    out8(RCAR_IIC_BASE + IIC_ICCH, 0x01);
    out8(RCAR_IIC_BASE + IIC_ICIC, IIC_TACK | IIC_WAIT);

    /* START condition */
    out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) | IIC_DTE);
    out8(RCAR_IIC_BASE + IIC_ICCR, IIC_CMD_START);

    /* Set Slave Addr */
    if(iic_wait(IIC_DTE)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) & ~IIC_DTE);
    out8(RCAR_IIC_BASE + IIC_ICDR, (slave_addr << 1) | IIC_WRITE);

    /* Write Reg Addr */
    if(iic_wait(IIC_WAIT)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICDR, reg_addr);
    out8(RCAR_IIC_BASE + IIC_ICSR, in8(RCAR_IIC_BASE + IIC_ICSR) & ~IIC_WAIT);

    /* Write */
    for(i=0; i<len; i++) {
        if(iic_wait(IIC_WAIT)){
            return 1;
        }
        out8(RCAR_IIC_BASE + IIC_ICDR, data[i++]);
        out8(RCAR_IIC_BASE + IIC_ICSR, in8(RCAR_IIC_BASE + IIC_ICSR) & ~IIC_WAIT);
    }

    /* Stop */
    if(iic_wait(IIC_WAIT)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICCR, IIC_CMD_STOP_TX);
    out8(RCAR_IIC_BASE + IIC_ICSR, in8(RCAR_IIC_BASE + IIC_ICSR) & ~IIC_WAIT);

    while(in8(RCAR_IIC_BASE + IIC_ICSR) & IIC_BUSY);

    out8(RCAR_IIC_BASE + IIC_ICCR, 0x01);

    return 0;
}

int iic_read_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    int i;

    out8(RCAR_IIC_BASE + IIC_ICCR, 0x01);
    out8(RCAR_IIC_BASE + IIC_ICCR, 0x81);

    out8(RCAR_IIC_BASE + IIC_ICCL, 0x07);
    out8(RCAR_IIC_BASE + IIC_ICCH, 0x01);
    out8(RCAR_IIC_BASE + IIC_ICIC, IIC_TACK | IIC_WAIT);

    /* START condition */
    out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) | IIC_DTE);
    out8(RCAR_IIC_BASE + IIC_ICCR, IIC_CMD_START);

    /* Set Slave Addr */
    if(iic_wait(IIC_DTE)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) & ~IIC_DTE);
    out8(RCAR_IIC_BASE + IIC_ICDR, (slave_addr << 1) | IIC_WRITE);

    /* Write Reg Addr */
    if(iic_wait(IIC_WAIT)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICDR, reg_addr);
    out8(RCAR_IIC_BASE + IIC_ICSR, in8(RCAR_IIC_BASE + IIC_ICSR) & ~IIC_WAIT);

    /* Repeated START condition */
    if(iic_wait(IIC_WAIT)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICCR, IIC_CMD_START);
    out8(RCAR_IIC_BASE + IIC_ICSR, in8(RCAR_IIC_BASE + IIC_ICSR) & ~IIC_WAIT);
    out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) | IIC_DTE);

    /* Set Slave Addr (Read) */
    if(iic_wait(IIC_DTE)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) & ~IIC_DTE);
    out8(RCAR_IIC_BASE + IIC_ICDR, (slave_addr << 1) | IIC_READ);

    /* TX TO RX */
    if(iic_wait(IIC_WAIT)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICCR, IIC_CMD_TX_TO_RX);
    out8(RCAR_IIC_BASE + IIC_ICSR, in8(RCAR_IIC_BASE + IIC_ICSR) & ~IIC_WAIT);

    /* Read */
    for(i=0; i<len; i++) {
        if(iic_wait(IIC_WAIT)){
            return 1;
        }

        if((i+2) >= len) {
            out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) | IIC_DTE);
            out8(RCAR_IIC_BASE + IIC_ICCR, IIC_CMD_STOP_RX);
            out8(RCAR_IIC_BASE + IIC_ICSR, in8(RCAR_IIC_BASE + IIC_ICSR) & ~IIC_WAIT);
            break;
        }

        data[i++] = in8(RCAR_IIC_BASE + IIC_ICDR);
        out8(RCAR_IIC_BASE + IIC_ICSR, in8(RCAR_IIC_BASE + IIC_ICSR) & ~IIC_WAIT);
    }

    /* Read last byte */
    if(iic_wait(IIC_DTE)){
        return 1;
    }
    out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) & ~IIC_DTE);
    data[i] = in8(RCAR_IIC_BASE + IIC_ICDR);

    out8(RCAR_IIC_BASE + IIC_ICIC, in8(RCAR_IIC_BASE + IIC_ICIC) & ~IIC_WAIT);

    while(in8(RCAR_IIC_BASE + IIC_ICSR) & IIC_BUSY);

    out8(RCAR_IIC_BASE + IIC_ICCR, 0x01);

    return 0;
}

