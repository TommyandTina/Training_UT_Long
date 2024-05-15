/*
* $QNXLicenseC:
* Copyright (c) 2019, QNX Software Systems. All Rights Reserved.
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

static const uint32_t i2c_bases[] = {
    RCAR_I2C0_BASE,
    RCAR_I2C1_BASE,
    RCAR_I2C2_BASE,
    RCAR_I2C3_BASE,
    RCAR_I2C4_BASE,
    RCAR_I2C5_BASE,
    RCAR_I2C6_BASE,
};

static uint8_t i2c_bus_inited[7] = { 0 };

/* Offset from I2C i2c_base */
#define RCAR_I2C_ICSCR      0x00        /* Slave Control Register */
#define RCAR_I2C_ICMCR      0x04        /* Master Control Register */
#define RCAR_I2C_ICSSR      0x08        /* Slave Status Register */
#define RCAR_I2C_ICMSR      0x0C        /* Master Status Register */
#define RCAR_I2C_ICSIER     0x10        /* Slave Interrupt Enable Register */
#define RCAR_I2C_ICMIER     0x14        /* Master Interrupt Enable Register */
#define RCAR_I2C_ICCCR      0x18        /* Clock Control Register */
#define RCAR_I2C_ICSAR      0x1C        /* Slave Address Register */
#define RCAR_I2C_ICMAR      0x20        /* Master Address Register */
#define RCAR_I2C_ICRXD      0x24        /* Receive Data Register */
#define RCAR_I2C_ICTXD      0x24        /* Transmit Data Register */
#define RCAR_I2C_ICCCR2     0x28
#define RCAR_I2C_ICMPR      0x2C
#define RCAR_I2C_ICHPR      0x30
#define RCAR_I2C_ICLPR      0x34

/* ICMSR bit definition */
#define RCAR_ICMSR_MNR      (1 << 6)
#define RCAR_ICMSR_MAL      (1 << 5)
#define RCAR_ICMSR_MST      (1 << 4)
#define RCAR_ICMSR_MDE      (1 << 3)
#define RCAR_ICMSR_MDT      (1 << 2)
#define RCAR_ICMSR_MDR      (1 << 1)
#define RCAR_ICMSR_MAT      (1 << 0)

/* ICMCR bit definition */
#define RCAR_ICMCR_MDBS     (1 << 7)
#define RCAR_ICMCR_FSCL     (1 << 6)
#define RCAR_ICMCR_FSDA     (1 << 5)
#define RCAR_ICMCR_OBPC     (1 << 4)
#define RCAR_ICMCR_MIE      (1 << 3)
#define RCAR_ICMCR_TSBE     (1 << 2)
#define RCAR_ICMCR_FSB      (1 << 1)
#define RCAR_ICMCR_ESG      (1 << 0)

//#define I2C_DEBUG

static uint8_t i2c_wait(uint32_t i2c_base, uint32_t flag)
{
    unsigned retries = 500;
    uint8_t reg;

    do {
        reg = in32(i2c_base + RCAR_I2C_ICMSR);
        retries--;
    } while(retries && !(reg & flag));

    if(!retries) {
#ifdef I2C_DEBUG
        kprintf("i2c_wait failed, flag 0x%x, ICMSR 0x%x\n", flag, reg);
#endif
        return 1;
    }

    if (in32(i2c_base + RCAR_I2C_ICMSR) & RCAR_ICMSR_MNR){
#ifdef I2C_DEBUG
        kprintf("i2c slave NACK!\n");
#endif
        return 2;
    }

    return 0;
}

/* return 0: Success, 1: Error, 2: Success but NACK */
static int i2c_recv(uint32_t i2c_base, uint8_t slave_addr, uint8_t *data, uint8_t len)
{
    int ret;
    uint32_t mcr = 0;

#ifdef I2C_DEBUG
    kprintf("%s: Slave address: 0x%x data len %d\n", __FUNCTION__, slave_addr, len);
#endif

    out32(i2c_base + RCAR_I2C_ICMAR, (slave_addr | 1));
    out32(i2c_base + RCAR_I2C_ICMSR, 0);
    out32(i2c_base + RCAR_I2C_ICMCR, RCAR_ICMCR_MIE | RCAR_ICMCR_MDBS | RCAR_ICMCR_ESG);

    ret = i2c_wait(i2c_base, RCAR_ICMSR_MAT | RCAR_ICMSR_MDR);
    if(ret){
        return ret;
    }

    mcr = RCAR_ICMCR_MIE | RCAR_ICMCR_MDBS;
    /* disable start if one byte to be rx'd */
    if (len == 1) {
        mcr |= RCAR_ICMCR_FSB;
    }

    out32(i2c_base + RCAR_I2C_ICMCR, mcr);

    /* Clear MAT and MDR */
    out32(i2c_base + RCAR_I2C_ICMSR,
        in32(i2c_base + RCAR_I2C_ICMSR) & ~(RCAR_ICMSR_MAT | RCAR_ICMSR_MDR));

    while(len) {
        /* wait for data rx */
        ret = i2c_wait(i2c_base, RCAR_ICMSR_MDR);
        if(ret){
            return ret;
        }

        if(len == 2) { /* last 2nd byte to be rx'd */
            out32(i2c_base + RCAR_I2C_ICMCR,
                in32(i2c_base + RCAR_I2C_ICMCR) | RCAR_ICMCR_FSB );
        }

        *data = in32(i2c_base + RCAR_I2C_ICRXD);

        out32(i2c_base + RCAR_I2C_ICMSR,
            in32(i2c_base + RCAR_I2C_ICMSR) & ~(RCAR_ICMSR_MDR));

        data++;
        len--;
    }

    /* wait for end of tx */
    ret = i2c_wait(i2c_base, RCAR_ICMSR_MST);
    if(ret) {
        return ret;
    }

    out32(i2c_base + RCAR_I2C_ICMSR,
        in32(i2c_base + RCAR_I2C_ICMSR) & ~(RCAR_ICMSR_MST));

    return 0;
}

/* return 0: Success, 1: Error, 2: Success but NACK */
static int i2c_send(uint32_t i2c_base, uint8_t slave_addr, uint8_t *data, uint8_t len)
{
    int ret;
#ifdef I2C_DEBUG
    kprintf("%s: Slave address: 0x%x data len %d\n", __FUNCTION__, slave_addr, len);
#endif

    /* Set Slave Addr and data */
    out32(i2c_base + RCAR_I2C_ICMAR, (slave_addr | 0));
    out32(i2c_base + RCAR_I2C_ICTXD, *data);
    out32(i2c_base + RCAR_I2C_ICMSR, 0);
    out32(i2c_base + RCAR_I2C_ICMCR, RCAR_ICMCR_MIE | RCAR_ICMCR_MDBS | RCAR_ICMCR_ESG);

    /* Wait for address write */
    ret = i2c_wait(i2c_base, RCAR_ICMSR_MAT | RCAR_ICMSR_MDE);
    if(ret) {
        return ret;
    }

    /* Dont send repeated start */
    out32(i2c_base + RCAR_I2C_ICMCR,
        in32(i2c_base + RCAR_I2C_ICMCR) & ~RCAR_ICMCR_ESG );

    /* Clear address transmitted and data empty */
    out32(i2c_base + RCAR_I2C_ICMSR,
        in32(i2c_base + RCAR_I2C_ICMSR) & ~(RCAR_ICMSR_MAT | RCAR_ICMSR_MDE));

    while(len) {
        /* wait for the data write */
        ret = i2c_wait(i2c_base, RCAR_ICMSR_MDE);
        if(ret) {
            return ret;
        }

        len--;
        if(len) {
            data++;
            out32(i2c_base + RCAR_I2C_ICTXD, *data); /* send next byte */
        } else {
            /* Last byte, send stop */
            out32(i2c_base + RCAR_I2C_ICMCR,
                in32(i2c_base + RCAR_I2C_ICMCR) | RCAR_ICMCR_FSB );
        }

        /* Clear MDE */
        out32(i2c_base + RCAR_I2C_ICMSR,
            in32(i2c_base + RCAR_I2C_ICMSR) & ~(RCAR_ICMSR_MDE));
    }

    /* Wait for stop transmit */
    ret = i2c_wait(i2c_base, RCAR_ICMSR_MST);
    if(ret) {
        return ret;
    }

    out32(i2c_base + RCAR_I2C_ICMSR,
        in32(i2c_base + RCAR_I2C_ICMSR) & ~(RCAR_ICMSR_MST));

    return 0;
}

static uint32_t i2c_init(uint8_t bus_no)
{
    uint32_t i2c_base;

    if (bus_no >= NUM_ELTS(i2c_bases)) {
#ifdef I2C_DEBUG
        kprintf("%s: Invalid i2c bus: %d\n", __FUNCTION__, bus_no);
#endif
        return 0;
    }

    i2c_base = i2c_bases[bus_no];

    if (!i2c_bus_inited[bus_no]) {
        /* clock */
        out32(i2c_base + RCAR_I2C_ICCCR2, 0x7);
        out32(i2c_base + RCAR_I2C_ICCCR, 0x6);

        out32(i2c_base + RCAR_I2C_ICMPR, 0x1c);
        out32(i2c_base + RCAR_I2C_ICHPR, 0x73);
        out32(i2c_base + RCAR_I2C_ICLPR, 0x85);

        /* reset slave interface */
        out32(i2c_base + RCAR_I2C_ICSIER, 0);
        out32(i2c_base + RCAR_I2C_ICSCR, 0x00000008);
        out32(i2c_base + RCAR_I2C_ICSAR, 0);
        out32(i2c_base + RCAR_I2C_ICSSR, 0);

        /* reset master interface */
        out32(i2c_base + RCAR_I2C_ICMIER, 0);
        out32(i2c_base + RCAR_I2C_ICMCR, 0);
        out32(i2c_base + RCAR_I2C_ICMAR, 0);
        out32(i2c_base + RCAR_I2C_ICMSR, 0);
    }

    return i2c_base;
}

/* return 0: Success, 1: Error, 2: Success but NACK */
int i2c_read_reg(uint8_t bus_no, uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    int ret;
    uint8_t req = reg_addr;
    uint32_t i2c_base;

    i2c_base = i2c_init(bus_no);
    if (i2c_base == 0) {
        return 1;
    }

    ret = i2c_send(i2c_base, slave_addr, &req, 1);
    if (ret != 0) {
        return ret;
    }

    ret = i2c_recv(i2c_base, slave_addr, data, len);
    return ret;
}

