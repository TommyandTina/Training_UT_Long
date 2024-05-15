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

#ifndef __BOARD_H_INCLUDED
#define __BOARD_H_INCLUDED

#define RCAR_GEN4_IPL_BOARD_NAME                 "R-Car S4-Spider"
#define QNX_LOAD_ADDR                             0x52000000

#define RCAR_IPL_AUTO_BOOT_DEVICE           IPL_BOOT_EMMC

/* EMMC Support */
#define RCAR_IPL_EMMC_SUPPORT
#define RCAR_IPL_EMMC_BASE                   RCAR_SDHI2_BASE
#define RCAR_IPL_EMMC_CLK                    (200*1000*1000)

/* Hyperflash Support */
//#define RCAR_IPL_HYPERFLASH_SUPPORT
//#define RCAR_IPL_HYPERFLASH_IMAGE_ADDR       0x8740000
//#define RCAR_IPL_SYSDMAC_BASE                RCAR_GEN3_SYSDMAC1_BASE

/* QSPI flash Support */
#define RCAR_IPL_QSPIFLASH_SUPPORT
#define RCAR_IPL_QSPI_IMAGE_ADDR             0xF00000
#define RCAR_IPL_QSPIFLASH_IMAGE_ADDR        (0x8000000 + RCAR_IPL_QSPI_IMAGE_ADDR)
#define RCAR_IPL_SYSDMAC_BASE                RCAR_GEN3_SYSDMAC1_BASE

/* Use SCIF0 @115200 */
//#define RCAR_IPL_UART_USE_SCIF_OR_HSCIF      UART_USE_SCIF0
/* Use SCIF3 @115200 */
//#define RCAR_IPL_UART_USE_SCIF_OR_HSCIF      UART_USE_SCIF3
/* Use HSCIF0 @1843200 */
#define RCAR_IPL_UART_USE_SCIF_OR_HSCIF      UART_USE_HSCIF0
/* Use HSCIF1 @1843200 */
//#define RCAR_IPL_UART_USE_SCIF_OR_HSCIF      UART_USE_HSCIF1

#endif /* __BOARD_H_INCLUDED */


