/*
 * $QNXLicenseC: 
 * Copyright 2017, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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
    unsigned hwi_off;
    unsigned tag_idx;
    hwi_tag  *tag;

    hwi_off = hwi_find_bus(HWI_ITEM_BUS_I2C, unit);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            dev->physbase = tag->location.base;
        }
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, 0);
        if (tag) {
            if (!dev->irq)
                dev->irq = tag->irq.vector;
        }

        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, 0);
        if (tag) {
            if (!dev->pck)
                dev->pck = tag->inputclk.clk;
        }
        if (!dev->dma.tx_reqid || !dev->dma.rx_reqid) {
            tag_idx = 0;
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                dev->dma.tx_reqid = (uint8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                dev->dma.rx_reqid = (uint8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                dev->dma.chan_min = (int8_t) tag->dma.chnl;
            }

            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_dma, &tag_idx);
            if (tag) {
                dev->dma.chan_max = (int8_t) tag->dma.chnl;
            }
        }
        tag_idx = 0;
        while ((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
            char* regname = __hwi_find_string(tag->regname.regname);
            if (strcmp(RCAR_HWI_CPG_MOD_ID, regname) == 0) {
                dev->reset_id = tag->regname.offset;
            }
        }

        hwi_off = hwi_find_device(RCAR_HWI_I2C_POC, unit);
        if(hwi_off != HWI_NULL_OFF) {
            tag_idx = 0;
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
            if(tag) {
                dev->poc.pbase = tag->location.base;
                dev->poc.size = tag->location.len;

                while((tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_regname, &tag_idx)) != NULL) {
                    char* regname = __hwi_find_string(tag->regname.regname);
                    if(strcmp(RCAR_HWI_PFC_PMMR,  regname) == 0) {
                        dev->poc.pmmr = tag->regname.offset;
                    } else if(strcmp(RCAR_HWI_PFC_REG,  regname) == 0) {
                        dev->poc.reg = tag->regname.offset;
                    } else if(strcmp(RCAR_HWI_PFC_MASK,  regname) == 0) {
                        dev->poc.mask = tag->regname.offset;
                    } else if(strcmp(RCAR_HWI_PFC_SHIFT,  regname) == 0) {
                        dev->poc.shift = tag->regname.offset;
                    }
                }
            }
        }
        return EOK;
    }

    /*
     * No default device, the base address and irq may have been specified as options
     */
    rcar_i2c_slogf(dev, VERBOSE_QUIET, "idx (%d) failed to find device\n", unit);
    return EIO;
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
    char *cp;
    int len;

    optind = 0;

    dev->physbase      = 0;
    dev->irq           = 0;
    dev->pck           = 0;
    dev->bus_speed     = I2C_STANDARD_BAUD;
    dev->output_mode   = OUTPUT_MODE_OD;
    dev->product_id    = 0;
    dev->timeout       = 0;
    dev->chan_idx      = -1;
    dev->dma.tx_reqid  = 0;
    dev->dma.rx_reqid  = 0;
    dev->dma.chan_min  = -1;
    dev->dma.chan_max  = -1;
    dev->dma.typed_mem = NULL;
    dev->reset_id      = -1;
    dev->dma.en        = 0;
#ifdef USE_SMMU
    dev->dma.smmu      = 0;
#endif 
    dev->fast_mode_plus = 0;

    while (!done) {
        prev_optind = optind;
#ifdef USE_SMMU
        c = getopt(argc, argv, "D:p:i:I:o:vdsem:");
#else
        c = getopt(argc, argv, "D:p:i:I:o:vdem:");
#endif
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
                if ((strcmp(optarg,"od") == 0) || (strcmp(optarg,"od33") == 0)) {
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
            case 't':
                dev->timeout = strtoul(optarg, NULL, 0);
                break;
            case 'v':
                dev->verbose++;
                break;
            case 'd':   // Enable DMA transfer
                dev->dma.en = 1;
                break;
#ifdef USE_SMMU
            case 's':    // smmu
                if (smmu_init(0) == -1)
                    fprintf(stderr, "failed to connect to SMMU Manager\n");
                else
                    dev->dma.smmu = 1;
                break;
#endif
            case 'D':       // Enable DMA (default DMA disabled)
                dev->dma.en = 1;
                dev->dma.tx_reqid = (uint8_t) strtoul(optarg, NULL, 0);
                cp = strchr(optarg, ':');
                if (cp == NULL) break;
                cp++;
                dev->dma.tx_reqid = (uint8_t) strtoul(cp, NULL, 0);
                cp = strchr(cp, ':');
                if (cp == NULL) break;
                cp++;
                dev->dma.chan_min = (int8_t) strtoul(cp, NULL, 0);
                cp = strchr(cp, ':');
                if (cp == NULL) break;
                cp++;
                dev->dma.chan_max = (int8_t) strtoul(cp, NULL, 0);
                break;
            case 'm':
                if (optarg) {
                    len = strlen(optarg) + 1;
                    dev->dma.typed_mem = (char *)malloc(len);
                    snprintf(dev->dma.typed_mem, len, "%s", optarg);
                }
                break;
            default:
                return -1;
        }
    }

    /* Get Product ID */
    unsigned hwi_off = hwi_find_device("r-car", 0);
    if (hwi_off != HWI_NULL_OFF) {
        hwi_tag *tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, 0);
        if (tag) {
             dev->product_id = tag->hwversion.version;
         }
    }

    if (dev->product_id == 0) {
        rcar_i2c_slogf(dev, VERBOSE_QUIET, "[chan %d]%s: Invalid product id (%x)\n", dev->chan_idx, __FUNCTION__, dev->product_id);
    }

    if (dev->output_mode == OUTPUT_MODE_LVTTL && IS_RCAR_GEN4(dev->product_id)) {
        /* Only Gen3 supports OUTPUT_MODE_LVTTL */
        dev->output_mode = OUTPUT_MODE_OD;
    }

    if (dev->dma.en && (dev->dma.tx_reqid  == -1 || dev->dma.rx_reqid == -1))  {
        dev->dma.en = 0; /* use PIO to read/write data */
    }

    if (dev->verbose == 2) {
        rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: dev->physbase = %lx", dev->chan_idx, __FUNCTION__, dev->physbase);
        rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: dev->irq = H'%x", dev->chan_idx, __FUNCTION__, dev->irq);
        rcar_i2c_slogf(dev, VERBOSE_LEVEL1, "[chan %d]%s: dev->pck = %dHz", dev->chan_idx, __FUNCTION__, dev->pck);
    }

    return 0;
}
