/*
 * $QNXLicenseC:
 * Copyright 2016,2021 QNX Software Systems.
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

/*
#ifdef __USAGE
%C - Serial driver for SCIF/HSCIF

%C [options] [scif#|hscif#]&
Options:
 -b number    Define initial baud rate (default )
 -B number    Base Address
 -c clk[/div] Set the input clock rate and divisor
 -C number    Size of canonical input buffer (default 256)
 -e           Set options to "edit" mode
 -E           Set options to "raw" mode (default)
 -h           Enable RS-232 use of RTS and CTS lines (default)
 -H           Permanently disable RS-232 use of RTS and CTS lines (for use as GPIOs)
 -i num       IRQ
 -I number    Size of raw input buffer (default 2048)
 -x           Use external clock
 -X           Use internal clock2 (266.66MHz) input to BRG
 -f           Enable hardware flow control (default)
 -F           Disable hardware flow control
 -O number    Size of output buffer (default 2048)
 -r num       Set RTS trigger level (SCIF default 15)
 -s           Enable software flow control
 -S           Disable software flow control (default)
 -t num       Set receive FIFO trigger level (SCIF default 14)
 -T num       Set transmit FIFO data trigger level (SCIF default 0)
 -u unit      Set serial unit number (default 1)
 -m (0|1)     Interrupt mode (0 = event, 1 = isr) (default = 1)
 -d           Enable DMA using dma channel info from the hwinfo (default DMA is disabled)
 -D dma_info  Enable DMA using dma channel info (Colon seperated): rx_reqid:tx_req_id:chan_min:chan_max
 -o options   string options:
                  disable=rx (Disable receiver on startup)
                  smmu=0/1/off/on
                      0/off disables SMMU Manager support (default)
                      1/on requires SMMU Manager support.
                      Only take affect when DMA is enabled with '-d' opt.
                      If SMMU Manager isn't started, the service fails to start.
                      If an invalid option is provided, defaults to OFF.
 -M           CPG MSTP module id
 -p pm_name   (devc-serscif-pm variant only) Enable powerman integration (default disabled).
 -y typed_mem Specify memory type for DMA transfer buffer.

 scif#|hscif# UART type. scif or hscif must be set in command line
              # is the UART device# defined in HWINFO

examples:

   devc-serscif -e -F -b115200 -x -t 14 scif0
   devc-serscif -F -b115200 -x -t 14 -d -o smmu=on scif0

#endif
*/

#include "externs.h"
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include <arm/scif.h>
#include <hw/hwinfo_rcar.h>

#define HSCIF_HSFCR_TTRG_127    127
#define HSCIF_HSFCR_RTRG_127    127
#define HSCIF_HSFCR_RSTRG_127   127

/*
 * Specify parameters for default devices from hwi_info tags.
 */
static int
query_hwi_device(TTYINIT_SCIF *dip, unsigned unit)
{
    unsigned hwi_off;
    unsigned tag_idx;
    hwi_tag  *tag;

    if (dip->scif == UART_TYPE_HSCIF) {
        hwi_off = hwi_find_device(RCAR_HWI_HSUART, unit);
    } else {
        hwi_off = hwi_find_device(RCAR_HWI_UART, unit);
    }

    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            if (!dip->tty.port) {
                dip->tty.port = tag->location.base;
            }
        }

        if (dip->dma.status) {
            dip->dma.Rx.fifo_addr = dip->tty.port + HSCIF_HSFRDR_OFF;
            dip->dma.Tx.fifo_addr = dip->tty.port + HSCIF_HSFTDR_OFF;
        }

        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, 0);
        if (tag) {
            if (!dip->tty.intr) {
                dip->tty.intr = tag->irq.vector;
            }
        }

        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, 0);
        if (tag) {
            if (!dip->tty.clk) {
                dip->tty.clk = tag->inputclk.clk;
            }
            if (!dip->tty.div) {
                dip->tty.div = tag->inputclk.div;
            }
        }

        if ((dip->dma.Tx.dmac_rs == 0) && (dip->dma.Rx.dmac_rs == 0) ) {
            tag_idx = 0;
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                dip->dma.Tx.dmac_rs = (uint8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                dip->dma.Rx.dmac_rs = (uint8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                dip->dma.chan_min = (int8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                dip->dma.chan_max = (int8_t) tag->dma.chnl;
            }
        }

        tag_idx = 0;
        while ((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
            char* regname = __hwi_find_string(tag->regname.regname);
            if (strcmp(RCAR_HWI_CPG_MOD_ID, regname) == 0) {
                dip->cpg_mod_id = tag->regname.offset;
            }
        }

        return 1;
    }
    return 0;
}


static uint16_t get_rstrg(unsigned rstrg, int scif, unsigned fifo_rx)
{
    uint16_t rstrg_val = 0;

    switch (scif) {
        case UART_TYPE_SCIF:
            if (!rstrg) {
                // If rstrg has not been set yet, set it to the default
                rstrg = 15;
            }

            // Need to make sure that the RTS trigger level > RX FIFO trigger
            if (rstrg <= fifo_rx) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "RTS trigger must be larger than RX FIFO trigger.");
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "RTS trigger will be set to 15.");
                rstrg = 15; // 15 is larger than the largest RX FIFO trigger
            }

            switch (rstrg) {
                default:
                    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "RTS Trigger number must be 1, 4, 6, 8, 10, 12, 14, 15.\nRTS trigger will be set to 15.");
                    /* Fall through */
                case 15:
                    rstrg_val = SCIF_SCFCR_RSTRG_15;
                    break;
                case 14:
                    rstrg_val = SCIF_SCFCR_RSTRG_14;
                    break;
                case 12:
                    rstrg_val = SCIF_SCFCR_RSTRG_12;
                    break;
                case 10:
                    rstrg_val = SCIF_SCFCR_RSTRG_10;
                    break;
                case 8:
                    rstrg_val = SCIF_SCFCR_RSTRG_8;
                    break;
                case 6:
                    rstrg_val = SCIF_SCFCR_RSTRG_6;
                    break;
                case 4:
                    rstrg_val = SCIF_SCFCR_RSTRG_4;
                    break;
                case 1:
                    rstrg_val = SCIF_SCFCR_RSTRG_1;
                    break;
            }
            break;
        case UART_TYPE_HSCIF:
            rstrg_val = rstrg;
            if (rstrg_val == 0) {
                // If rstrg has not been set yet, set it to the default
                rstrg_val = 15;
            } else if (rstrg_val > HSCIF_HSFCR_RSTRG_127) {
                rstrg_val = HSCIF_HSFCR_RSTRG_127;
            }
            break;
        default:
            rstrg_val = 0;
            break;
    }
    return (rstrg_val);
}

static int get_fifo(unsigned fifo_tx, unsigned fifo_rx, int scif)
{
    int fifo = 0;

    switch (scif) {
        case UART_TYPE_SCIF:
            switch (fifo_rx) {
                case 14:
                    fifo = SCIF_SCFCR_RTRG_E;
                    break;
                case 8:
                    fifo = SCIF_SCFCR_RTRG_8;
                    break;
                case 4:
                    fifo = SCIF_SCFCR_RTRG_4;
                    break;
                default:
                    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "RX FIFO Trigger number must be 1, 4, 8 or 14. \nRx trigger will be set to 1.");
                    /* Fall through */
                case 1:
                    fifo = SCIF_SCFCR_RTRG_1;
                    break;
            }
            switch (fifo_tx) {
                case 8:
                    fifo |= SCIF_SCFCR_TTRG_8;
                    break;
                case 4:
                    fifo |= SCIF_SCFCR_TTRG_4;
                    break;
                case 2:
                    fifo |= SCIF_SCFCR_TTRG_2;
                    break;
                default:
                    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_INFO, "TX FIFO Trigger number must be 0, 2, 4, or 8. \nTx trigger will be set to 0.");
                    /* Fall through */
                case 0:
                    fifo |= SCIF_SCFCR_TTRG_0;
                    break;
            }
            break;
        case UART_TYPE_HSCIF:
            if (fifo_rx == 0) {
                // If fifo_rx has not been set yet, set it to the default
                fifo_rx = 1;
            } else if (fifo_rx > HSCIF_HSFCR_RTRG_127) {
                fifo_rx = HSCIF_HSFCR_RTRG_127;
            } if (fifo_tx == 0) {
                // If fifo_tx has not been set yet, set it to the default
                fifo_tx = 8;
            } else if (fifo_tx > HSCIF_HSFCR_TTRG_127) {
                fifo_tx = HSCIF_HSFCR_TTRG_127;
            }
            fifo = (fifo_rx << 8) | fifo_tx;
            break;
        default:
            break;
    }
    return(fifo);
}

unsigned options(int argc, char *argv[])
{
    int opt, numports = 0;
    int hwi_num = -1;   // hwi_num
    char *cp;
    unsigned unit = 1;  // dev name prefix
    unsigned fifo_tx = 0;
    unsigned fifo_rx = 0;
    unsigned rstrg = 0;
    int len;

    unit = 1;
    static TTYINIT_SCIF devinit = {
        {
            0,                         // Port
            0,                         // Port shift
            0,                         // Interrupt address
            0, 2048, 2048, 256,        // Set default Baud=0 is to use baud setting of bootloader, isize, osize, csize
            0, 0, 0, 0,                // cflag, iflag, lflag, oflag
            0, 0, 0,                   // fifo, clk, div
            "/dev/ser",                // Name
            NULL,                      // reserved1
            0,                         // reserved2
            0                          // verbose
        },
        0,                           // rstrg
        0,                           // scif
        RTS_CTS_HW_DISABLE,          // rts_hw_disable
        INTERNAL_CLOCK,              // Internal clock
        0,                           // FIFO
        1,                           // Interrupt service mode
        // DMA control
        {
            DMA_DISABLE,
            .chan_min = -1,
            .chan_max = -1,
            // dma_handle Rx
            {
                {0}, 0, {0}, NULL, 0, 0, 0, 0, 0
            },
            // dma_handle Tx
            {
                {0}, 0, {0}, NULL, 0, 0, 0, 0, 0
            },
            // dmafuncs
            {NULL},
            -1,
            -1,
            -1,
            NULL,
            NULL,
            PTHREAD_MUTEX_INITIALIZER,
        },
        -1, // cpg_mod_id
#if defined(VARIANT_pm)
            NULL,          // pm_name;
            PM_DISABLED,   // Disable Power Management by default
#endif
    };

    // Initialize the devinit to raw mode
    ttc(TTC_INIT_RAW, &devinit, 0);

    while (optind < argc) {
        // Process dash options.
        while ((opt = getopt(argc, argv, IO_CHAR_SERIAL_OPTIONS "D:m:B:i:u:c:T:t:r:hHxdp:M:Xy:")) != -1) {
            switch(ttc(TTC_SET_OPTION, &devinit, opt)) {
                case 'B':
                    devinit.tty.port = strtoul(optarg, NULL, 0);
                    break;
                case 'i':
                    devinit.tty.intr = atoi(optarg);
                    break;
                case 't':
                    // (Wait to process until we know if we are SCIF, HSCIF)
                    fifo_rx = atoi(optarg);
                    break;
                case 'T':
                    // (Wait to process until we know if we are SCIF, HSCIF)
                    fifo_tx = atoi(optarg);
                    break;
                case 'h':
                    // Enable the RTS/CTS lines to be used as RS-232 lines
                    devinit.rts_hw_disable = RTS_CTS_HW_ENABLE;
                    break;
                case 'H':
                    // Disable the RTS/CTS lines to be used as RS-232 lines - use as GPIO's instead
                    devinit.rts_hw_disable = RTS_CTS_HW_DISABLE;
                    break;
                case 'x':
                    // Use external clock
                    devinit.clock_source = EXTERNAL_CLOCK;
                    break;
                case 'r':
                    // Read in the RTS trigger value
                    rstrg = atoi(optarg);
                    break;
                case 'u':       // Where to start device name at (def 1)
                    unit = atoi(optarg);
                    break;
                case 'c':       //clk/div value for baud rate
                    devinit.tty.clk = atol(optarg);
                    if ((cp = strchr(optarg, '/')))
                        devinit.tty.div = atoi(cp + 1);
                    break;
                case 'm':       // set the Interrupt service mode 0=event 1=isr (def 1)
                    devinit.mode = strtoul(optarg, NULL, 0);
                    break;
                case 'd':       // Enable DMA (default DMA disabled)
                    devinit.dma.status = DMA_ENABLE;
                    break;
                case 'D':       // Enable DMA (default DMA disabled)
                    devinit.dma.status = DMA_ENABLE;
                    devinit.dma.Rx.dmac_rs = (uint8_t) strtoul(optarg, NULL, 0);

                    cp = strchr(optarg, ':');
                    if (cp == NULL) break;
                    cp++;
                    devinit.dma.Tx.dmac_rs = (uint8_t) strtoul(cp, NULL, 0);

                    cp = strchr(cp, ':');
                    if (cp == NULL) break;
                    cp++;
                    devinit.dma.chan_min = (int8_t) strtoul(cp, NULL, 0);

                    cp = strchr(cp, ':');
                    if (cp == NULL) break;
                    cp++;
                    devinit.dma.chan_max = (int8_t) strtoul(cp, NULL, 0);
                    break;
                case 'M':       // CPG module id
                    devinit.cpg_mod_id = (int) strtoul(optarg, NULL, 0);
                    break;
#if defined(VARIANT_pm)
                case 'p':       // Enable Power Management (default Powman disabled)
                    devinit.pm_name = strdup(optarg);
                    devinit.pm_status = PM_ENABLED;
                    break;
#endif
                case 'X':
                    // Use internal clock2
                    devinit.clock_source = INTERNAL_CLOCK2;
                    break;
                case 'y':       // Memory type
                    if (optarg) {
                        len = strlen(optarg) + 1;
                        devinit.dma.typed_mem = (char *)malloc(len);
                        snprintf(devinit.dma.typed_mem, len, "%s", optarg);
                    }
                    break;
            }
        }

        // Don't allow the RTS/CTS lines to be disabled if HW Flow control option is selected
        if (devinit.tty.c_cflag & IHFLOW || devinit.tty.c_cflag & OHFLOW) {
            devinit.rts_hw_disable = RTS_CTS_HW_ENABLE;
        }

        // Custom ports and interrupts
        while (optind < argc && *(optarg = argv[optind]) != '-') {
            char *p;

            if (strlen(optarg) < 3) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Illegal option, port must be scif# or hscif#");
                exit(0);
            }

            if ((p = strstr(optarg, "hscif")) != NULL) {
                /* Initialize based on device type */
                devinit.tty.port_shift = 0;
                if (strlen(optarg) > 5) {
                    hwi_num = strtoul(&p[5], NULL, 10);
                    if ((hwi_num < 0) || (hwi_num>4)) {
                        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Illegal port number option");
                        exit(0);
                    }
                    devinit.scif = UART_TYPE_HSCIF;
                } else {
                    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Port number option is missing\n\r");
                    exit(0);
                }
            } else if ((p = strstr(optarg, "scif")) != NULL) {
                /* Initialize based on device type */
                devinit.tty.port_shift = 0;
                if (strlen(optarg) > 4) {
                    hwi_num = strtoul(&p[4], NULL, 10);
                    if ((hwi_num < 0) || (hwi_num > 5)) {
                        slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Illegal port number option");
                        exit(0);
                    }
                    if (fifo_rx == 0) {
                        fifo_rx = 14;
                    }
                    devinit.scif = UART_TYPE_SCIF;
                } else {
                    slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Port number option is missing");
                    exit(0);
                }
            } else {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Illegal option, port must be scif or hscif");
                exit(0);
            }

            // Check if hwi is set, if yes,Getting the UART Base addresss
            // and irq from the Hwinfo Section if available
            if (hwi_num >= 0) {
                query_hwi_device(&devinit, hwi_num);
            }

            // Setup the values to program the FIFO
            //devinit.tty.fifo = get_fifo(fifo_tx, fifo_rx, devinit.scif);
            devinit.fifo = get_fifo(fifo_tx, fifo_rx, devinit.scif);
            devinit.rstrg = get_rstrg(rstrg, devinit.scif, fifo_rx);
            if (create_device(&devinit, unit++) == -1) {
                slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Fail to create a device");
                exit(0);
            }
            ++numports;
            ++optind;
        }
    }

    if (numports == 0) {
        if (devinit.tty.verbose) {
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Creating device %s%d", devinit.tty.name, unit);
        }
        // Setup the values to program the FIFO
        //devinit.tty.fifo = get_fifo(fifo_tx, fifo_rx, devinit.scif);
        devinit.fifo = get_fifo(fifo_tx, fifo_rx, devinit.scif);
        devinit.rstrg = get_rstrg(rstrg, devinit.scif, fifo_rx);
        if (create_device(&devinit, unit++) == -1) {
            slogf(_SLOG_SETCODE(_SLOGC_CHAR, 0), _SLOG_ERROR, "Fail to create a device");
            exit(0);
        }
        ++numports;
        ++optind;
    }
    return numports;
}
