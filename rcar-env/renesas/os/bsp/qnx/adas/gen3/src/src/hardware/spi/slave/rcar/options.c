/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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

#include "spi_slave.h"
#include "msiof.h"
#include <hw/hwinfo_rcar.h>
#include <drvr/hwinfo.h>

static void query_hwi_device(spi_slave_t *spi, unsigned interface);

static char *spi_opts[] =
{
"cpol",
"cpha",
"bitlen",
NULL
};

/*******************************************************************************
* Outline      : Parsing option
* Function Name: spi_slave_option
* Description  : Parse options from command line and save parameter to structure
* Arguments    : spi_slave_t* dev
*                int argc
*                char* argv[]
* Return Value : EXIT_SUCCESS
                 EXIT_FAILURE
* Note         :
*******************************************************************************/
int spi_slave_option (spi_slave_t * dev, int argc, char* argv[])
{
    int c, opt;
    char * options, * freeptr, * value;
    int channel, cpol, cpha, bitlen;
    while (-1 != (c = getopt(argc, argv, "u:i:c:v")))
    {
        switch (c)
        {
            case 'u':
            {
                channel = strtol(optarg, NULL, 0);
                if ((channel < NUMS_OF_SPI_DEV) && (channel >=0))
                {
                    dev->channel = channel;
                    query_hwi_device(dev, dev->channel);
                }
                else
                {
                    printf("Invalid channel\n");
                    printf("Please select channel from 0 to 3\n");
                    return EXIT_FAILURE;
                }
                break;
            }
            case 'i':
                /* NOTE: we do not support id yet */
                break;
            case 'c':
            {
                if (NULL == optarg)
                {
                    return EXIT_FAILURE;
                }
                else
                {
                    freeptr = options = strdup (optarg);
                    while (options && * options != '\0')
                    {
                        if (-1 == (opt = getsubopt(&options, spi_opts, &value)))
                        {
                            printf("Error: Invalid configuration parameter\n");
                            goto fail;
                        }

                        switch (opt)
                        {
                        case CPOL_OPTION:
                            cpol = strtol(value, NULL, 0);
                            if (CPOL_VALIDATE(cpol) != 0)
                            {
                                dev->cfg.cpol = cpol;
                            }
                            else
                            {
                                printf("Error: Invalid CPOL\n");
                                goto fail;
                            }
                            break;
                        case CPHA_OPTION:
                            cpha = strtol(value, NULL, 0);
                            if (CPHA_VALIDATE(cpha) != 0)
                            {
                                dev->cfg.cpha = cpha;
                            }
                            else
                            {
                                printf("Error: Invalid CPHA\n");
                                goto fail;
                            }
                            break;
                        case BIT_LEN_OPTION:
                            bitlen = strtol (value, NULL, 0);
                            if (BIT_LEN_VALIDATE(bitlen) == 0)
                            {
                                printf("Error: Invalid bitlen\n");
                                goto fail;

                            }
                            else
                            {
                                dev->cfg.bitlen = bitlen;
                            }
                            break;
                        default:
                            printf("Error: Unknow configuration options\n");
                            goto fail;
                        }
                    }
                    free(freeptr);
                }
                break;
            }
            case 'v':
            {
                dev->verbose++;
                break;
            }
            default:
            {
                printf("Error: Unknow options\n");
                return EXIT_FAILURE;
            }
        }
    }
    rcar_spi_slogf(dev, VERBOSE_LEVEL8, "%s: SPI Channel = %d, CPOL = %d, CPHA = %d",
                   __FUNCTION__, dev->channel, dev->cfg.cpol, dev->cfg.cpha);
    return EXIT_SUCCESS;
fail:
    free(freeptr);
    return EXIT_FAILURE;
}

static void query_hwi_device(spi_slave_t *spi, unsigned interface)
{
    hwi_tag *tag;
    unsigned hwi_off = hwi_find_bus(RCAR_HWI_MSIOF, interface);

    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag) {
            spi->pbase = tag->location.base;
        }
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, 0);
        if(tag) {
            spi->irq = tag->irq.vector;
        }
    }
}
