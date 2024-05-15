/*
 * $QNXLicenseC:
 * Copyright 2014, 2016 QNX Software Systems.
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

#include "startup.h"
#include <arm/scif.h>

#define HSCIF_SCSCR_CKE_IN             (2 << 0)
#define HSCIF_HSSRR_SRCYC              (0x1F << 0)
#define SCBRR_VALUE(bps, clk)          (clk / bps / 32 - 1) /* Internal Clock */

static void my_delay(int dly) {
    volatile int i = dly;

    while (i--)
        ;
}

static void scif_hscif_init(unsigned address, unsigned baud, unsigned clk, unsigned extclk)
{
    unsigned scscr = 0;

    if (address & 0x00800000) {  //scif
        if (clk == 0) {
            clk = 66666666;
            extclk = 0;
        }
        in16(address + SCIF_SCLSR_OFF);             /* dummy read */
        out16(address + SCIF_SCLSR_OFF, 0x0000);    /* clear ORER bit */
        out16(address + SCIF_SCFSR_OFF, 0x0000);    /* clear all error bit */

        out16(address + SCIF_SCSCR_OFF, 0x0000);    /* clear SCR.TE & SCR.RE*/
        out16(address + SCIF_SCFCR_OFF, SCIF_SCFCR_TFRST | SCIF_SCFCR_RFRST);
        out16(address + SCIF_SCSMR_OFF, 0x0000);    /* 8bit data, no-parity, 1 stop, Po/1 */

        if (!extclk && ((clk/1000000) != 266)) { //internal PCK clk src
            out16(address + SCIF_SCBRR_OFF, SCBRR_VALUE(baud, clk));
            scscr = SCIF_SCSCR_TE | SCIF_SCSCR_RE; /* enable TE, RE; internal PCK clk */
        } else { // Input clock src (to BRG)
            my_delay(100);
            out16(address + SCIF_DL_OFF, clk / (16 * baud));
            if ((clk/1000000) == 266) {
                out16(address + SCIF_CKS_OFF, 0x4000);      /* select SC_CLK=SCKi */
            } else {
                out16(address + SCIF_CKS_OFF, 0x0000);      /* select SC_CLK=SCIF_CLK */
            }
            scscr = SCIF_SCSCR_TE | SCIF_SCSCR_RE | HSCIF_SCSCR_CKE_IN;
        }
        out16(address + SCIF_SCFCR_OFF, 0x0000);    /* reset-off tx-fifo, rx-fifo. */
        out16(address + SCIF_SCSCR_OFF, scscr);
    } else { //hscif
        unsigned sr, dl;

        if (clk == 0) {
            clk = 266666666;
            extclk = 0;
        }

        for (sr = 8; sr <= 32; sr++) {
            dl = clk / (baud  * sr);
            if (dl < 0xFFFF) {
                break;
            }
        }
        in16(address + HSCIF_HSLSR_OFF);             /* dummy read */
        out16(address + HSCIF_HSLSR_OFF, 0x0000);    /* clear ORER bit */
        out16(address + HSCIF_HSFSR_OFF, 0x0000);    /* clear all error bit */

        out16(address + HSCIF_HSSCR_OFF, 0x0000);    /* clear SCR.TE & SCR.RE*/
        out16(address + HSCIF_HSFCR_OFF, SCIF_SCFCR_TFRST | SCIF_SCFCR_RFRST);
        out16(address + HSCIF_HSSMR_OFF, 0x0000);    /* 8bit data, no-parity, 1 stop, Po/1 */
        my_delay(100);
        if (extclk) {
            out16(address + SCIF_CKS_OFF, 0x0000);      /* select scif_clk	 */
        } else {
            out16(address + SCIF_CKS_OFF, 0x4000);      /* select internal clk	 */
        }
        out16(address + SCIF_DL_OFF, clk / (sr * baud));
        out16(address + HSCIF_HSSRR_OFF, (in16(address + HSCIF_HSSRR_OFF) & ~HSCIF_HSSRR_SRE)   | HSCIF_HSSRR_SRE);
        out16(address + HSCIF_HSSRR_OFF, (in16(address + HSCIF_HSSRR_OFF) & ~HSCIF_HSSRR_SRCYC) | (sr - 1));
        scscr = SCIF_SCSCR_TE | SCIF_SCSCR_RE | HSCIF_SCSCR_CKE_IN;

        my_delay(100);
        out16(address + HSCIF_HSFCR_OFF, 0x0000);    /* reset-off tx-fifo, rx-fifo. */
        out16(address + HSCIF_HSSCR_OFF, scscr);
        my_delay(100);
    }
}

static void parse_line(unsigned channel, const char *line, unsigned long *baud,
            unsigned long *clk, unsigned long *div, unsigned *extclk) {
    if(*line != '.' && *line != '\0') {
        dbg_device[channel].base = strtopaddr(line, (char **)&line, 16);
        if(*line == '^') dbg_device[channel].shift = strtoul(line+1, (char **)&line, 0);
    }
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *baud = strtoul(line, (char **)&line, 0);
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *clk = strtoul(line, (char **)&line, 0);
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *div = strtoul(line, (char **)&line, 0);
    if(*line == '.') ++line;
    if(*line != '.' && *line != '\0') *extclk = strtoul(line, (char **)&line, 0);
}

void init_scif(unsigned channel, const char *init, const char *defaults)
{
    unsigned long	baud;
    unsigned long	div;
    unsigned long	clk;
    unsigned	n, extclk;

    baud = clk = extclk = n = 0;
    dbg_device[channel].base = R_CAR_H3_SCIF_BASE2;
    parse_line(channel, defaults, &baud, &clk, &div, &extclk);
    parse_line(channel, init, &baud, &clk, &div, &extclk);
    if (baud != 0) {
        scif_hscif_init(dbg_device[channel].base, baud, clk, extclk);
    }

    // wait for one bit interval, then turn on send/receive
    for (n = 0; n < clk / 10000; ++n) {
        if ((n & 0x3ff) == 0)
            mdriver_check();
    }
}

void put_scif(int c)
{
    unsigned base = dbg_device[0].base;
    while( !(in16(base + SCIF_SCFSR_OFF) & SCIF_SCSSR_TDFE) );
    out8(base + SCIF_SCFTDR_OFF, c);
    out16(base + SCIF_SCFSR_OFF, 0);
    while( !(in16(base + SCIF_SCFSR_OFF) & SCIF_SCSSR_TEND) );
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/mainline/hardware/startup/lib/common_arm/hw_serscif.c $ $Rev: 807382 $")
#endif
