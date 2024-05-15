/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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

#include "canfd.h"

/* The program name, which is set immediately when main is called, used by canfd_log.h */
char *prog_name;
int prog_name_len;
int log_verbosity;
/* Specified product */
int32_t  product_id;
int32_t  product_rev;
/* Private functions */
static int canfd_create_device(candev_init_t    *candev_init);
static int get_canfd_hwinfo(CANFDDEV_HWINFO    *candev, unsigned unit);
static int single_instance(char* resmgr_name);
/* End of private functions */

void canfd_calculate_bittiming(candev_init_t * pcinit, uint32_t speed, uint32_t fcan, bool is_bitrate)
{
    uint32_t    Tq = 0;
    int         tseg1 = 0;
    int         tseg2 = 0;
    uint32_t    prescaler = 0;
    uint8_t     count = 0;
    /*                                                           */
    /* |ss|<-------------tseg1------------->|<------tseg2------>|*/
    /*                              |<-sjw->|           |<-sjw->|*/
    /*                                                           */
    /*************************************************************/
    /* Communication speed = (fCAN / (Baudrate Prescaler x Tq))  */
    /* Sample point % = (SS + dtseg1) / Tq                        */
    /* Calculating bitrate */
    if (is_bitrate)
    {
        if ((CANFD_NORMAL_BITRATE_MAX < speed) || (0==speed))
        {
            speed = CANFD_NORMAL_BITRATE_MAX;
            LOG_WARNING("You set an invalid normal speed, it will be cut off at 1 Mbit/s.\n");
        }
loop1:
        for (prescaler = 1; prescaler < CANFD_PRESCALER_N_MAX; prescaler++)
        {
            Tq = (uint32_t)roundf((float)((fcan / prescaler) / speed));
            tseg1 = (int)roundf((float)(Tq * SAMPLE_POINT - SS));
            tseg2 = (int)(Tq - SS - tseg1);
            
            if (CLASSIC_CAN_MODE == pcinit->interface)
            {
                /* Classic CAN interface */
                if ((tseg2 < tseg1) &&
                (CAN_TSEG1_MIN <= tseg1) &&
                (CAN_TSEG1_MAX >= tseg1) &&
                (CAN_TSEG2_MIN <= tseg2) &&
                (CAN_TSEG2_MAX >= tseg2))
                {
                    pcinit->tseg1 = tseg1;
                    pcinit->tseg2 = tseg2;
                    pcinit->brp = prescaler;
                    break;
                }
            }
            else
            {
                /* CANFD interface */
                if ((tseg2 < tseg1) &&
                (CANFD_NTSEG1_MIN <= tseg1) &&
                (CANFD_NTSEG1_MAX >= tseg1) &&
                (CANFD_NTSEG2_MIN <= tseg2) &&
                (CANFD_NTSEG2_MAX >= tseg2))
                {
                    pcinit->ntseg1 = tseg1;
                    pcinit->ntseg2 = tseg2;
                    pcinit->nbrp = prescaler;
                    break;
                }
            }
        }
        if (CANFD_PRESCALER_N_MAX == prescaler)
        {
            LOG_WARNING("The driver could not configure a valid normal bitrate corresponding to your input,\
                    a default speed 250 Kbit/s is using now.\n");
            /* Calculating bitrate */
            if (count==0)
            {   
                speed = 250000;
                count++;
                goto loop1;
            }
        }
    }
    else
    {
        /* Only accept CANFD */
        if (CLASSIC_CAN_MODE != pcinit->interface)
        {
            if ((CANFD_DATA_BITRATE_MAX < speed) || (0 == speed))
            {
                speed = CANFD_DATA_BITRATE_MAX;
                LOG_WARNING("You set an invalid data speed, it will be cut off at 8 Mbit/s.\n");
            }
loop2:
            for (prescaler = 1; prescaler < CANFD_PRESCALER_D_MAX; prescaler++)
            {
                /* Calculating bitrate */
                Tq = (uint32_t)roundf((float)((fcan / prescaler) / speed));
                tseg1 = (int)roundf((float)(Tq * SAMPLE_POINT - SS));
                tseg2 = (int)(Tq - SS - tseg1);
                if ((tseg2 < tseg1) &&
                (CANFD_DTSEG1_MIN <= tseg1) &&
                (CANFD_DTSEG1_MAX >= tseg1) &&
                (CANFD_DTSEG2_MIN <= tseg2) &&
                (CANFD_DTSEG2_MAX >= tseg2))
                {
                    pcinit->dtseg1 = tseg1;
                    pcinit->dtseg2 = tseg2;
                    pcinit->dbrp = prescaler;
                    break;
                }
            }

            if (CANFD_PRESCALER_D_MAX == prescaler)
            {
                LOG_WARNING("The driver could not configure a valid data bitrate corresponding to your input,\
                        a default speed 2 Mbit/s is using now.\n");
                /* Calculating bitrate */
                if (count==0)
                {
                    speed = 2000000;
                    count++;
                    goto loop2;
                }
            }
        }
    } 
}

static int get_product_id()
{
    hwi_tag *tag;
    unsigned hwi_off;

    product_id = PRODUCT_ID_RCAR_UNKNOWN;
    product_rev = PRODUCT_REV_UNKNOWN;

    hwi_off = hwi_find_device("r-car", 0);
    if (hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, NULL);
        if (tag) {
            product_id = tag->hwversion.version;
            product_rev = tag->hwversion.hclass;
        } else {
            printf("can-fd: Unable to query product_id from HW info\n");
            return (-1);
        }
    } else {
        printf("can-fd: Unable to find product version name from HW info\n");
        return (-1);
    }

    return EOK;
}

/*******************************************************************************
* Outline      : This function run as soon as start this driver.
* Function Name: main
* Description  : Initializing can parrameters, resource manager, parse options,
*                then creating can device and handling clients.
* Arguments    : argc
*                *argv[]
* Return Value : EXIT_SUCCESS
                 EXIT_FAILURE
* Note         :
*******************************************************************************/
int main(int argc, char *argv[])
{
    int                 ret = EXIT_SUCCESS;
    int                 opt = 0;
    int                 mode = 0;
    uint8_t             channel = 0;
    uint32_t            speed = 0;
    uint32_t            dspeed = 0;
    uint32_t            fcan = fCAN;
    uint32_t            tx_interval = 0;
    uint8_t             index = 0;
    uint32_t            n = 0;

    //Default speed 250 Kbit/s
    speed = 250000;
    //Default data speed 2 Mbit/s
    dspeed = 2000000;

    /* Check if CANFD driver already started */
    char name[NAME_MAX];
    sprintf(name, "%s", "can");
    if (single_instance(name)) {
        exit(EXIT_FAILURE);
    }

    /* Store name for logging */
    prog_name = argv[0];
    prog_name_len = strlen(prog_name);

    /* Always log start message before verbosity is set in the configuration */
    log_verbosity = LOGGING_MAX_VERBOSITY;
    LOG_INFO("Start Driver\n");

    can_drvr_funcs_t     canfd_funcs_cb = {canfd_transmit_cb, canfd_control_cb};

    /* Init candev_init parameters */
    candev_init_t     candev_init =
    {
        {
            CANDEV_TYPE_RX,
            0,
            0,
            100,
            8
        },

        CANFD_MODE,                            /* CANFD interface */
        INTERNAL_CLOCK_MODE,

        /* Classical bit timing settings 0*/
        0,
        0,
        0,
        0,

        /* Nominal bit timing settings 0 */
        0,
        0,
        0,
        0,

        /* Data bit timing settings 0 */
        0,
        0,
        0,
        0,

        { /* HWINFO CANFD INFO */
            0x0,        /* reg base */
            0x0,        /* reg len */
            {0x0,0x0},  /* IRQ vector */
            0x0,        /* clock source rate */
        },

        {
            /* Channel 0 */
            {
                INTERNAL_LOOP_BACK_MODE,
                /* Initial RX IDs */
                {
                    RX0_C0_ID
                },
                /* Initial TX IDs */
                {
                    TX0_C0_ID
                },
                1,                              /* Switching baudrate */
                CANFD_FRAME,                    /* CANFD frame */
                {
                    CANFD_DEFAULT_TX_INTERVAL   /* Initial TX0 interval  */
                }
            },

            /* Channel 1 */
            {
                INTERNAL_LOOP_BACK_MODE,
                /* Initial RX IDs */
                {
                    RX0_C1_ID
                },
                /* Initial TX IDs */
                {
                    TX0_C1_ID
                },
                1,                              /* Switching baudrate */
                CANFD_FRAME,                    /* CANFD frame */
                {
                    CANFD_DEFAULT_TX_INTERVAL   /* Initial TX0 interval */
                }
            },

            /* Channel 2 */
            {
                INTERNAL_LOOP_BACK_MODE,
                /* Initial RX IDs */
                {
                    RX0_C2_ID
                },
                /* Initial TX IDs */
                {
                    TX0_C2_ID
                },
                1,                              /* Switching baudrate */
                CANFD_FRAME,                    /* CANFD frame */
                {
                    CANFD_DEFAULT_TX_INTERVAL   /* Initial TX0 interval */
                }
            },

            /* Channel 3 */
            {
                INTERNAL_LOOP_BACK_MODE,
                /* Initial RX IDs */
                {
                    RX0_C3_ID
                },
                /* Initial TX IDs */
                {
                    TX0_C3_ID
                },
                1,                              /* Switching baudrate */
                CANFD_FRAME,                    /* CANFD frame */
                {
                    CANFD_DEFAULT_TX_INTERVAL   /* Initial TX0 interval */
                }
            },

            /* Channel 4 */
            {
                INTERNAL_LOOP_BACK_MODE,
                /* Initial RX IDs */
                {
                    RX0_C4_ID
                },
                /* Initial TX IDs */
                {
                    TX0_C4_ID
                },
                1,                              /* Switching baudrate */
                CANFD_FRAME,                    /* CANFD frame */
                {
                    CANFD_DEFAULT_TX_INTERVAL   /* Initial TX0 interval */
                }
            },

            /* Channel 5 */
            {
                INTERNAL_LOOP_BACK_MODE,
                /* Initial RX IDs */
                {
                    RX0_C5_ID
                },
                /* Initial TX IDs */
                {
                    TX0_C5_ID
                },
                1,                              /* Switching baudrate */
                CANFD_FRAME,                    /* CANFD frame */
                {
                    CANFD_DEFAULT_TX_INTERVAL   /* Initial TX0 interval */
                }
            },

            /* Channel 6 */
            {
                INTERNAL_LOOP_BACK_MODE,
                /* Initial RX IDs */
                {
                    RX0_C6_ID
                },
                /* Initial TX IDs */
                {
                    TX0_C6_ID
                },
                1,                              /* Switching baudrate */
                CANFD_FRAME,                    /* CANFD frame */
                {
                    CANFD_DEFAULT_TX_INTERVAL   /* Initial TX0 interval */
                }
            },

            /* Channel 7 */
            {
                INTERNAL_LOOP_BACK_MODE,
                /* Initial RX IDs */
                {
                    RX0_C7_ID
                },
                /* Initial TX IDs */
                {
                    TX0_C7_ID
                },
                1,                              /* Switching baudrate */
                CANFD_FRAME,                    /* CANFD frame */
                {
                    CANFD_DEFAULT_TX_INTERVAL   /* Initial TX0 interval */
                }
            }
        },
        0,                                      /* Classic tolerance */
    };

    /* Specified product */
    if (get_product_id() != EOK) {
        LOG_ERROR("Get info error\n");
        return( EXIT_FAILURE );
    }

    /* get hwinfo for our hardware */
    if ( !get_canfd_hwinfo(&candev_init.canfd_hwi,0))
    {
        LOG_DEBUG2("HWINFO pulled in sucessfully\n");
    }
    else
    {
        candev_init.canfd_hwi.reglen = CANFD_REG_SIZE;
        candev_init.canfd_hwi.regbase = CANFD_REG_BASE;
        candev_init.canfd_hwi.irqvector[0] = CANFD_IRQ_GL;
        candev_init.canfd_hwi.irqvector[1] = CANFD_IRQ_CH;
    }

    /* Init resource manager */
    ThreadCtl( _NTO_TCTL_IO, 0 );
    can_resmgr_init(&canfd_funcs_cb);

    /* Parsing options */
    while ((opt = getopt(argc, argv, "b:d:u:m:t:x:i:e")) != -1)
    {
        switch (opt)
        {
            /* Set normal bitrate */
            case 'b':
                speed = strtoul(optarg, NULL, 0);
                break;

            /* Set data bitrate */
            case 'd':
                dspeed = strtoul(optarg, NULL, 0);
                /* Enable bit rate switch all channels */
                for (int chan = 0;chan < CANFD_TOTAL_CHANNELS;chan++) {
                    candev_init.channel[chan].txbrs = 1;
                }
                LOG_INFO("Enable bit rate switch all channels.\n");
                break;

            /* Select channel to support for case 'm' */
            case 'u':
                channel = strtoul(optarg, NULL, 0);
                break;

            /* Set mode for a channel */
            case 'm':
                /* There are 7 modes:
                    STANDARD_TEST_MODE         : -m0
                    LISTEN_ONLY_MODE           : -m1
                    EXTERNAL_LOOP_BACK_MODE    : -m2
                    INTERNAL_LOOP_BACK_MODE    : -m3
                    CHANNEL_COMMUNICATION_MODE : -m4
                    CHANNEL_RESET_MODE         : -m5
                    CHANNEL_HALT_MODE          : -m6
                */
                mode = strtoul(optarg, NULL, 0);
                if (CHANNEL_HALT_MODE >= mode)
                {
                    /* Set mode */
                    candev_init.channel[channel].mode = mode;
                }
                else
                {
                    /* If get a wrong setting, default is communication mode */
                    candev_init.channel[channel].mode = CHANNEL_COMMUNICATION_MODE;
                }
                break;

            /* Select internal clock mode or external clock mode for CANFD module */
            case 'x':
                /* External clock */
                candev_init.clks = EXTERNAL_CLOCK_MODE;
                fcan = strtoul(optarg, NULL, 0);
                break;

            /* Select classical mode or CAN FD mode or FD mode */
            case 'i':
                candev_init.interface = strtoul(optarg, NULL, 0);
                if (!IS_RCAR_GEN4(product_id))
                {
                    // FD mode is only supported for R-CarGen4/R-CarV3U
                    if (candev_init.interface > CANFD_MODE)
                    {
                        candev_init.interface = CANFD_MODE;
                        fprintf(stderr,"Driver is only supported for\n\
                            the classical mode or CAN FD mode\n\
                            . A default CAN FD mode is using now\n");
                        break;
                    }
                }
                if (candev_init.interface > FD_MODE)
                {
                    candev_init.interface = CANFD_MODE;
                    fprintf(stderr,"Driver is only supported for\n\
                        the classical mode or CAN FD mode or FD mode\n\
                        . A default CAN FD mode is using now\n");
                }
                break;

            case 't':
                tx_interval = strtoul(optarg, NULL, 0);
                if ((CANFD_TX_INTERVAL_MAX < tx_interval) || (CANFD_TX_INTERVAL_MIN > tx_interval))
                {
                    LOG_WARNING("Cannot set this value of transmition interval. Timestamp range is from 0 to 1275\n\
                            The current transmition interval: %d\n", CANFD_DEFAULT_TX_INTERVAL);
                    for (channel = 0;channel < CANFD_TOTAL_CHANNELS;channel++)
                    {
                        for (index = 0;index < CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER;index++)
                        {
                            candev_init.channel[channel].tx_interval[index] = CANFD_DEFAULT_TX_INTERVAL;
                        }
                    }
                }
                else
                {
                    n = roundf((float)(tx_interval / CANFD_TX_INTERVAL_UNIT));
                    for (channel = 0;channel < CANFD_TOTAL_CHANNELS;channel++)
                    {
                        for (index = 0;index < CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER;index++)
                        {
                            candev_init.channel[channel].tx_interval[index] = CANFD_TX_INTERVAL_UNIT * n;
                        }
                    }
                }
                break;

            /* Select classical tolerance mode */
            case 'e':
                // Classical CAN only mode, FD tolerant mode: R-CarGen4/R-CarV3U
                if (IS_RCAR_GEN4(product_id))
                {
                    candev_init.classic_tol = 1;
                }
                break;
            default:
                /* Do nothing */
                break;
        }
    }

    canfd_calculate_bittiming(&candev_init, speed, fcan, true);
    canfd_calculate_bittiming(&candev_init, dspeed, fcan, false);
    /* Initializing CANFD device */
    ret = canfd_create_device(&candev_init);

    /* Start handling clients */
    can_resmgr_start();

    return (ret);
}

/*******************************************************************************
* Outline      : Reprogramming the code flash while running in RAM
* Function Name: canfd_create_device
* Description  : This function will allocate device info, allocate an array of devices,
*                map device registers, set device mailbox as transmit or receive,
*                initialize the CAN device, create the resmgr device, initialize device
*                hardware and device mailboxes, initialize interrupts and attach interrupt
*                handler
* Arguments    : candev_init_t
* Return Value : EXIT_SUCCESS
                 EXIT_FAILURE
* Note         :
*******************************************************************************/
static int canfd_create_device(candev_init_t    *candev_init)
{
    candev_info_t            *candev_info;
    candev_exp_t             *candev_list;
    uint8_t                  i = 0;
    uint8_t                  channel = 0;
    uint32_t                 size = 0;
    /* Total TX/RX FIFO and RX FIFO specified for each channel */
    uint8_t       total_fifo = CANFD_RECEIVE_FIFO_BUFFER + CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER;

    /* Allocate device info */
    candev_info = (void *) calloc(1,sizeof(*candev_info));
    if (!candev_info)
    {
        LOG_ERROR("candev_info: smalloc failed\n");
        return (EXIT_FAILURE);
    }

    /* Allocate an array of devices - one for each mailbox */
    size = sizeof(*candev_list) * (CANFD_RECEIVE_FIFO_BUFFER + CANFD_TRANSMIT_RECEIVE_FIFO_BUFFER) * CANFD_TOTAL_CHANNELS;
    candev_list = (void *) calloc(1,size);

    if (!candev_list)
    {
        LOG_ERROR("candev_list: smalloc failed\n");
        free(candev_info);
        return (EXIT_FAILURE);
    }

    /* Map device registers */
    candev_info->base = mmap_device_io(candev_init->canfd_hwi.reglen, candev_init->canfd_hwi.regbase);
    if (MAP_DEVICE_FAILED == candev_info->base)
    {
        LOG_ERROR("mmap_device_io: map device I/O failed.\n");
        free(candev_info);
        free(candev_list);
        return (EXIT_FAILURE);
    }
    /* Copy candev_list and candev_init to candev_info */
    candev_info->candev_list = (candev_exp_t *)candev_list;
    candev_info->candev_init = candev_init;

    for (channel = 0; channel < CANFD_TOTAL_CHANNELS; channel++)
    {
        /* Set device unit depend on channel number*/
        candev_init->cinit.can_unit = channel;

        for (i = 0; i < total_fifo; i++)
        {
            candev_list[i + (total_fifo * channel)].channel = channel;
            candev_list[i + (total_fifo * channel)].index = i + (total_fifo * channel);

            /* Store a pointer to the device info */
            candev_list[i + (total_fifo * channel)].candev_info = candev_info;

            /* Set device mailbox as transmit or receive */
            if (CANFD_RECEIVE_FIFO_BUFFER > i)
            {
                candev_init->cinit.devtype = CANDEV_TYPE_RX;
                /* Set device mailbox unit number */
                candev_init->cinit.dev_unit = i;
            }
            else
            {
                candev_init->cinit.devtype = CANDEV_TYPE_TX;
                /* Set device mailbox unit number */
                candev_init->cinit.dev_unit = i - CANFD_RECEIVE_FIFO_BUFFER;
            }

            /* Initialize the CAN device */
            can_resmgr_init_device(&candev_list[i + (total_fifo * channel)].candev, &candev_init->cinit);
            /* Create the resmgr device */
            can_resmgr_create_device(&candev_list[i + (total_fifo * channel)].candev);
        }
    }
    /* Initialize device hardware and device mailboxes */
    canfd_init_hw(candev_init, candev_info);

    /* Initialize interrupts and attach interrupt handler */
    canfd_init_tx_irq(candev_list);
    canfd_init_rx_irq(candev_list);

    return (EXIT_SUCCESS);
}

/* pull in hardware info */
static int get_canfd_hwinfo(CANFDDEV_HWINFO *candev, unsigned unit)
{
    unsigned hwi_off;
    unsigned irq_0 = 0, irq_1 = 1;
    hwi_tag *tag;

    hwi_off = hwi_find_bus(HWI_ITEM_BUS_CAN, unit);
    if (hwi_off != HWI_NULL_OFF)
    {
        hwiattr_can_t attr;

        hwiattr_get_can(hwi_off, &attr);

        candev->regbase = attr.common.location.base;
        candev->reglen = attr.common.location.len;
        if (attr.common.num_irq > 0)
        {
            candev->irqvector[0] = hwitag_find_ivec(hwi_off, &irq_0);
            candev->irqvector[1] = hwitag_find_ivec(hwi_off, &irq_1);
        }

        if (attr.num_clks > 0)
        {
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_inputclk, 0);
            candev->clksrc_rate = tag->inputclk.clk/tag->inputclk.div;
        }

        return 0;
    }
    else
    {
        return -1;
    }
}

static int single_instance(char* resmgr_name)
{
    name_attach_t* attach;

    /* Create a local name */
    if ((attach = name_attach(NULL, resmgr_name, 0)) == NULL) {
        fprintf(stderr, "\nError: Is '%s' already started? ", resmgr_name);
        perror("name_attach Failed");
        return(-1);
    }

    if (-1 == ChannelDestroy(attach->chid)) {
        perror("ChannelDestroy failed:");
    }
    return 0;
}
