/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <hw/inout.h>
#include <aarch64/r-car.h>
#include "ipl.h"
#include <private/rcar_sdhi.h>
#include <private/sdmmc.h>
#include <private/fat-fs.h>
#include "rcar_gen4.h"
#include "rpc.h"
#include "board.h"
#include "../rcar_ipl.h"

rcar_sdmmc_bs_t sdmmc_bs;

extern void init_board(int uart_boot_idx);

#if defined(RCAR_IPL_EMMC_SUPPORT) || defined(RCAR_IPL_SDMMC_SUPPORT)
/*
 * The buffer used by fat-fs.c as the FAT info, which can be cache-enabled,
 * so we make it in SRAM for fast access
 */
static unsigned char fat_buf1[FAT_FS_INFO_BUF_SIZE];
static unsigned char fat_buf2[FAT_COMMON_BUF_SIZE];
#endif

#define IMAGE_SCAN_SIZE             0x2000
#define IFS_FILENAME                "QNX-IFS"

static int
start_image_from_ram (unsigned image, int checksum)
{
    ser_putstr("Scanning for image at @ 0x");
    ser_puthex(image);
    ser_putstr("\n");
    image = image_scan_2(image, image + IMAGE_SCAN_SIZE, checksum);

    if (image != 0xffffffff) {
        ser_putstr("Found image           @ 0x");
        ser_puthex(image);
        ser_putstr("\n");
        image_setup_2(image);

        ser_putstr("Jumping to startup    @ 0x");
        ser_puthex(startup_hdr.startup_vaddr);
        ser_putstr("\n\n");
        image_start_2(image);

        /* Never reach here */
        return 0;
    }

    ser_putstr("Image_scan failed...\n");
    return -1;
}


#if defined(RCAR_IPL_EMMC_SUPPORT) || defined(RCAR_IPL_SDMMC_SUPPORT)
static int
boot_from_sdhi(card_type_t type, paddr_t address, const char *fn, paddr_t base, uint32_t clk)
{
    sdmmc_t  sdmmc;
    int      status;

    ser_putstr("SD/MMC download....\n");

    /* initialize the sdmmc board specific funtions */
    rcar_sdmmc_init_bs(&sdmmc_bs);

    /* initialize the sdmmc interface and card */
    if (type == eMMC) {
        rcar_sdmmc_init_hc(&sdmmc, base, clk, SDMMC_VERBOSE_LVL_0);

#ifdef RCAR_EMMC_CAPS
        sdmmc.caps = RCAR_EMMC_CAPS;
#endif
        status = sdmmc_init_mmc(&sdmmc);

    } else if (type == eSDC) {
        rcar_sdmmc_init_hc(&sdmmc, base, clk, SDMMC_VERBOSE_LVL_0);
        status = sdmmc_init_sd(&sdmmc);
    } else {
        ser_putstr("Unknown card type\n");
        return -1;
    }

    if (status != SDMMC_OK) {
        ser_putstr("SD/MMC interface init failed\n");
        return -1;
    }

    fat_sdmmc_t fat = {
        .ext = &sdmmc,
        .buf1 = fat_buf1,
        .buf1_len = FAT_FS_INFO_BUF_SIZE,
        .buf2 = fat_buf2,
        .buf2_len = FAT_COMMON_BUF_SIZE,
        .verbose = 1
    };

    if (fat_init(&fat)) {
        ser_putstr("Failed to init fat-fs\n");
        status = SDMMC_ERROR;
        goto done;
    }

    status = fat_copy_named_file((unsigned char *)address, (char *)fn);

done:
    sdmmc_fini(&sdmmc);

    if (status) {
        ser_putstr("Load image failed.\n");
        return -1;
    }

    ser_putstr("load image done.\n");
    return start_image_from_ram(address, 1);
}
#endif

#if defined(RCAR_IPL_HYPERFLASH_SUPPORT) || defined(RCAR_IPL_QSPIFLASH_SUPPORT)
static int boot_from_flash(uint32_t image, uint32_t flashAddr)
{
    uint32_t   imageaddr;
    uint32_t   size;

    ser_putstr("Flash download header...");

    /* Copy just the first IMAGE_SCAN_SIZE bytes to scan for the startup header */
    if (flash_copy(image, flashAddr, IMAGE_SCAN_SIZE)) {
        ser_putstr("failed\n");
        return 1;
    }
    ser_putstr("\n");

    /* Just check the signature, no checksum */
    imageaddr = image_scan_2(image, image + IMAGE_SCAN_SIZE, 0);

    if (imageaddr == -1) {
        return imageaddr;
    }

    // Copy the rest of the image
    flashAddr += IMAGE_SCAN_SIZE;
    size       = (imageaddr - image) + startup_hdr.stored_size - IMAGE_SCAN_SIZE;
    ser_putstr("Flash download image...");
    if (flash_copy(image + IMAGE_SCAN_SIZE, flashAddr, size)) {
        ser_putstr("failed\n");
        return 1;
    }
    ser_putstr("\n");

    return start_image_from_ram(imageaddr, 1);
}
#endif


static unsigned
image_download_bin_ser_start_image(unsigned long dst_address)
{
    unsigned   i;
    uint32_t   imageaddr;
    uint32_t   size;
    char       *dst = (char *)dst_address;

    ser_putstr("Ser download header...\n");

    /*
     * Get just the first IMAGE_SCAN_SIZE bytes to scan for the startup header
     */
    for (i = 0; i <= IMAGE_SCAN_SIZE; i++) {
        *dst = ser_getchar();
        dst++;
    }

    /* Just check the signature, no checksum */
    imageaddr = image_scan_2(dst_address, dst_address + IMAGE_SCAN_SIZE, 0);

    if (imageaddr == -1) {
        ser_putstr("Scan header is failed...\n");
        return imageaddr;
    }

    /*
     *  Get the rest of the image
     */
    size  = (imageaddr - dst_address) + startup_hdr.stored_size - IMAGE_SCAN_SIZE - 1;
    ser_putstr("Ser download image...\n");
    for (i = 0; i < size; i++) {
        *dst = ser_getchar();
        dst++;
    }
    ser_putstr("Ser download image...finished\n");

    return start_image_from_ram(imageaddr, 1);
}

IPL_BootOpt_t ipl_boot_menu() {

    ser_putstr("Commands:\n");
    ser_putstr(" Press 'S' for serial download, using the 'QNX IDE sendnto' utility\n");

    ser_putstr(" Press 'B' for serial download, IFS MUST be in 'raw' binary\n");

#ifdef RCAR_IPL_EMMC_SUPPORT
    ser_putstr(" Press 'E' for eMMC download, IFS filename MUST be '"IFS_FILENAME"'\n");
#endif

#ifdef RCAR_IPL_SDMMC_SUPPORT
    ser_putstr(" Press 'M' for SD/MMC download, IFS filename MUST be '"IFS_FILENAME"'\n");
#endif

#ifdef RCAR_IPL_HYPERFLASH_SUPPORT
    ser_putstr(" Press 'H' for Hyperflash Download\n");
#endif

#ifdef RCAR_IPL_QSPIFLASH_SUPPORT
    ser_putstr(" Press 'Q' for QSPI flash Download\n");
#endif

#ifdef RCAR_IPL_RAMBOOT_SUPPORT
    ser_putstr(" Press 'J' for Boot from IFS at 0x");ser_puthex(QNX_LOAD_ADDR);ser_putstr("\n");
#endif
    switch (ser_getchar()) {
        case 's':
        case 'S':
            return IPL_BOOT_SERIAL_SREC;
        case 'B':
        case 'b':
            return IPL_BOOT_SERIAL_BIN;
        case 'E':
        case 'e':
            return IPL_BOOT_EMMC;
        case 'M':
        case 'm':
            return IPL_BOOT_SD;
        case 'H':
        case 'h':
            return IPL_BOOT_HYPERFLASH;
        case 'Q':
        case 'q':
            return IPL_BOOT_QSPIFLASH;
        case 'J':
        case 'j':
            return IPL_BOOT_RAM;
        default:
            return IPL_BOOT_UNKNOWN;
    }
}

int main(void)
{
    IPL_BootOpt_t  boot_device;

    sdmmc_bs.init = NULL;
    sdmmc_bs.signal_voltage = NULL;

    init_board(RCAR_IPL_UART_USE_SCIF_OR_HSCIF);

    ser_putstr("\n\nQNX Neutrino Initial Program Loader for the "RCAR_GEN4_IPL_BOARD_NAME"\n");
    ser_putstr("IPL compiled ");
    ser_putstr(__DATE__);
    ser_putstr(" ");
    ser_putstr(__TIME__);
    ser_putstr("\n");

    while (1) {
#ifdef RCAR_IPL_AUTO_BOOT_DEVICE
    boot_device = RCAR_IPL_AUTO_BOOT_DEVICE;
#else
    boot_device = ipl_boot_menu();
#endif
        boot_device = ipl_boot_menu();
        switch (boot_device) {
#ifdef RCAR_IPL_EMMC_SUPPORT
            case IPL_BOOT_EMMC:
                boot_from_sdhi(eMMC, QNX_LOAD_ADDR, IFS_FILENAME,
                        RCAR_IPL_EMMC_BASE, RCAR_IPL_EMMC_CLK);
                /* Shouldn't get here if successful */
                break;
#endif

#ifdef RCAR_IPL_SDMMC_SUPPORT
            case IPL_BOOT_SD:
                boot_from_sdhi(eSDC, QNX_LOAD_ADDR, IFS_FILENAME,
                        RCAR_IPL_SDMMC_BASE, RCAR_IPL_SDMMC_CLK);
                break;
#endif

#ifdef RCAR_IPL_HYPERFLASH_SUPPORT
            case IPL_BOOT_HYPERFLASH:
                rpc_flash_init(RPC_FLASH_TYPE_HYPER);
                boot_from_flash(QNX_LOAD_ADDR, RCAR_IPL_HYPERFLASH_IMAGE_ADDR);
                break;
#endif

#ifdef RCAR_IPL_QSPIFLASH_SUPPORT
            case IPL_BOOT_QSPIFLASH:
#ifndef NO_FLASH_REINIT
                rpc_flash_init(RPC_FLASH_TYPE_QSPI);
#endif
                boot_from_flash(QNX_LOAD_ADDR, RCAR_IPL_QSPIFLASH_IMAGE_ADDR);
                break;
#endif

#ifdef RCAR_IPL_RAMBOOT_SUPPORT
            case IPL_BOOT_RAM:
                start_image_from_ram(QNX_LOAD_ADDR, 1);
                break;
#endif
            case IPL_BOOT_SERIAL_SREC:
                ser_putstr("send image now...\n");
                if (image_download_ser(QNX_LOAD_ADDR)) {
                    ser_putstr("download failed...\n");
                    continue;
                }
                else {
                    ser_putstr("download OK...\n");
                    start_image_from_ram(QNX_LOAD_ADDR, 1);
                }
                break;
            case IPL_BOOT_SERIAL_BIN:
                image_download_bin_ser_start_image(QNX_LOAD_ADDR);
                break;
            default:
                continue;
        }
    }

    return 0;
}
