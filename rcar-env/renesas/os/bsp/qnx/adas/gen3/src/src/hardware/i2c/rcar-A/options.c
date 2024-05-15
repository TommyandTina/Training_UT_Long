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

#include <string.h>
#include "proto.h"

/**
 * Finds I2C device in hwi table.
 *
 * @param dev  Pointer to I2C driver device structure.
 * @param unit I2C unit number.
 *
 * @return Execution status.
 * @retval 1 Success.
 * @retval 0 Not found.
 */
int query_hwi_device(rcar_i2c_dev_t *dev, unsigned unit)
{
    unsigned hwi_off = hwi_find_bus(HWI_ITEM_BUS_I2C, unit);
    hwi_tag *tag;
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            dev->physbase = tag->location.base;
            dev->irq = hwitag_find_ivec(hwi_off, NULL);
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, NULL);
            if (tag) {
                dev->pck = tag->inputclk.clk;
            }
        }
        return 1;
    }
    /*
     * No default device, the base address and irq have been specified
     */
    return 0;
}

/**
 * Parses I2C driver specific options.
 *
 * @param dev  Pointer to I2C driver device structure.
 * @param argc Number of arguments.
 * @param argv Pointer to arguments.
 *
 * @return Execution status.
 * @retval  0 Success.
 * @retval -1 Fail.
 */
int rcar_i2c_parse_options(rcar_i2c_dev_t *dev, int argc, char *argv[])
{
    int c;
    int prev_optind;
    int done = 0;
    int chan = 0;
    char *option = NULL;

    optind = 0;

    dev->physbase    = 0;
    dev->irq         = -1;
    dev->pck         = I2C_DEFAULT_PCLK;
    dev->busSpeed    = I2C_STANDARD_BAUD;
    dev->output_mode = OUTPUT_MODE_OD;
    dev->reset_id      = -1;
    dev->fast_mode_plus = 0;

    while (!done) {
        prev_optind = optind;
        c = getopt(argc, argv, "p:i:I:o:vdsea:");
        switch (c) {
            case 'I':
                dev->chan_idx = strtoul(optarg, &optarg, 0);
                /* Getting the I2C Base addresss and irq from the Hwinfo Section if available */
                query_hwi_device(dev, dev->chan_idx);
                break;            
            case 'p':
                dev->physbase = strtoul(optarg, NULL, 0);
                break;
            case 'i':
                dev->irq      = strtoul(optarg, NULL, 0);
                break;
            case 'e':
                dev->isr = 0;
                break;
            case 'o':
                if (strcmp(optarg,"od") == 0 || strcmp(optarg,"od33") == 0){
                    dev->output_mode = OUTPUT_MODE_OD;
                } else if (strcmp(optarg,"od18") == 0) {
                    dev->output_mode = OUTPUT_MODE_OD;
                    dev->fast_mode_plus = 1;
                } else if (strcmp(optarg,"lvttl") == 0) {
                    dev->output_mode = OUTPUT_MODE_LVTTL;
                } else {
                    fprintf(stderr,"Illegal option, -o argument must be od, od18, od33 or lvttl\n");
                }
                break;
            case '?':
                if (optopt == '-') {
                    ++optind;
                    break;
                }
                return -1;

            case -1:
                if (prev_optind < optind) /* -- */
                    return -1;

                if (argv[optind] == NULL) {
                    done = 1;
                    break;
                }

                if (*argv[optind] != '-') {
                    ++optind;
                    break;
                }
                return -1;

            case 'v':
                dev->verbose++;
                break;
            case 'd':   // Enable DMA transfer
                dev->dma = 1;
                break;
            case 's':    // smmu
                if(smmu_init(0) == -1)
                    fprintf(stderr, "failed to connect to SMMU Manager\n");
                else
                    dev->smmu = 1;
                break;
            case 'a':
                option = optarg;
                break;
            default:
                return -1;
        }
    }

#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };
    if (option == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        option = cs_machine_str;
    }
#endif

    dev->product_id = (strstr(option, "V3U") || strstr(option, "v3u")) ? PRODUCT_ID_RCAR_V3U : 0;

#define MSTP_RESET_ID(x)  ((strstr(option, "V3U") || strstr(option, "v3u")) ? CPG_MSTP_V3U_I2C##x : CPG_MSTP_GEN3_I2C##x)
#define INTCSYS_IRQ(x)    ((strstr(option, "V3U") || strstr(option, "v3u")) ? RCAR_V3U_INTCSYS_I2C##x : RCAR_GEN3_INTCSYS_I2C##x)

    if (dev->physbase == 0) {
        /* Device not found in HWI table */
        switch (chan) {
            case 0:
                dev->physbase = RCAR_GEN3_I2C0_BASE;
                dev->irq = INTCSYS_IRQ(0);
                break;
            case 1:
                dev->physbase = RCAR_GEN3_I2C0_BASE;
                dev->irq = INTCSYS_IRQ(1);
                break;
            case 2:
                dev->physbase = RCAR_GEN3_I2C0_BASE;
                dev->irq = INTCSYS_IRQ(2);
                break;
            case 3:
                dev->physbase = RCAR_GEN3_I2C0_BASE;
                dev->irq = INTCSYS_IRQ(3);
                break;
            case 4:
                dev->physbase = RCAR_GEN3_I2C0_BASE;
                dev->irq = INTCSYS_IRQ(4);
                break;
            case 5:
                dev->physbase = RCAR_GEN3_I2C0_BASE;
                dev->irq = INTCSYS_IRQ(5);
                break;
            case 6:
                dev->physbase = RCAR_GEN3_I2C0_BASE;
                dev->irq = INTCSYS_IRQ(6);
                break;
            default:
                dev->physbase = RCAR_GEN3_I2C0_BASE;
                dev->irq = INTCSYS_IRQ(0);
                break;
        }
    }
    
    switch (dev->physbase) {
    case RCAR_GEN3_I2C0_BASE:
        dev->dma_tx_midrid = RCAR_GEN3_DREQ_I2C0_TXI;
        dev->dma_rx_midrid = RCAR_GEN3_DREQ_I2C0_RXI;
        dev->reset_id = MSTP_RESET_ID(0);
        break;
    case RCAR_GEN3_I2C1_BASE:
        dev->dma_tx_midrid = RCAR_GEN3_DREQ_I2C1_TXI;
        dev->dma_rx_midrid = RCAR_GEN3_DREQ_I2C1_RXI;
        dev->reset_id = MSTP_RESET_ID(1);
        break;
    case RCAR_GEN3_I2C2_BASE:
        dev->dma_tx_midrid = RCAR_GEN3_DREQ_I2C2_TXI;
        dev->dma_rx_midrid = RCAR_GEN3_DREQ_I2C2_RXI;
        dev->reset_id = MSTP_RESET_ID(2);
        break;
    case RCAR_GEN3_I2C3_BASE:
        dev->dma_tx_midrid = RCAR_GEN3_DREQ_I2C3_TXI;
        dev->dma_rx_midrid = RCAR_GEN3_DREQ_I2C3_RXI;
        dev->reset_id = MSTP_RESET_ID(3);
        break;
    case RCAR_GEN3_I2C4_BASE:
        dev->dma_tx_midrid = RCAR_GEN3_DREQ_I2C4_TXI;
        dev->dma_rx_midrid = RCAR_GEN3_DREQ_I2C4_RXI;
        dev->reset_id = MSTP_RESET_ID(4);
        break;
    case RCAR_GEN3_I2C5_BASE:
        dev->dma_tx_midrid = RCAR_GEN3_DREQ_I2C5_TXI;
        dev->dma_rx_midrid = RCAR_GEN3_DREQ_I2C5_RXI;
        dev->reset_id = MSTP_RESET_ID(5);
        break;	
    case RCAR_GEN3_I2C6_BASE:
        dev->dma_tx_midrid = RCAR_GEN3_DREQ_I2C6_TXI;
        dev->dma_rx_midrid = RCAR_GEN3_DREQ_I2C6_RXI;
        dev->reset_id = MSTP_RESET_ID(6);
        break;
    default:
        return -1;
    }

    if (dev->verbose == 2){
        rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: dev->physbase = %lx", dev->chan_idx, __FUNCTION__, dev->physbase);
        rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: dev->irq = H'%x", dev->chan_idx, __FUNCTION__, dev->irq);
        rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: dev->pck = %dHz", dev->chan_idx, __FUNCTION__, dev->pck);
    }

    return 0;
}
