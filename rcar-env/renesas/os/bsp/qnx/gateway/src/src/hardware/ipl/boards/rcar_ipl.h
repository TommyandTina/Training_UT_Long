/*
* $QNXLicenseC:
* Copyright (c) 2017, QNX Software Systems. All Rights Reserved.
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

#ifndef __RCAR_IPL_H_INCLUDED
#define __RCAR_IPL_H_INCLUDED

//#define BIT(x)              (1 << x)
#define BITS(x,y)           (((2ULL << (y-x)) - 1) << x)

#define DISABLE             0
#define ENABLE              1

#define UART_USE_SCIF0   0
#define UART_USE_SCIF1   1
#define UART_USE_SCIF2   2
#define UART_USE_SCIF3   3
#define UART_USE_HSCIF0  4
#define UART_USE_HSCIF1  5

typedef enum {
    IPL_BOOT_SD,
    IPL_BOOT_EMMC,
    IPL_BOOT_QSPIFLASH,
    IPL_BOOT_HYPERFLASH,
    IPL_BOOT_NOR,
    IPL_BOOT_SERIAL_SREC,
    IPL_BOOT_SERIAL_BIN,
    IPL_BOOT_RAM,
    IPL_USB_BOOT,
    IPL_BOOT_UNKNOWN
} IPL_BootOpt_t;

typedef struct {
    unsigned long base;
    unsigned long baud;
    unsigned clk;
    unsigned extclk;
} ipl_uart_boot_t;

#endif //__RCAR_BOARD_H_INCLUDED
