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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <devctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "spi_slave.h"

#define FLAG_INFO     1
#define FLAG_CFG      2
#define FLAG_WRITE    4
#define FLAG_READ     8
#define FLAG_XCHANGE  16
#define MAX_DATA_XFER   64 // max data buffer

/* Global variable */
char * spidir;
uint8_t flag_opt = 0;
uint8_t txlen    = 0;    // write data length
uint8_t rxlen    = 0;    // read data length

uint32_t txbuf[MAX_DATA_XFER]; //Tx buffer
uint8_t txbuf8[MAX_DATA_XFER];
uint16_t txbuf16[MAX_DATA_XFER];

uint8_t rxbuf[MAX_DATA_XFER*4]; //Rx buffer

int cpol = 0;
int cpha = 0;
int bitlen = 8;

spi_slave_cfg_t cfg;

static int parse_option(int argc, char *argv[]);
int get_spidir (int argc, char *argv[]);

int main(int argc, char *argv[])
{
    spi_data_t      txdata;
    spi_data_t      rxdata;
    cfg.cpol   = 0;
    cfg.cpha   = 0;
    cfg.id     = 0;
    cfg.bitlen = 8;
    cfg.wordlen= 1;
    int fd;
    int ret = 0;
    int count = 0;

    uint8_t  *buf_8;
    uint16_t *buf_16;
    uint32_t *buf_32;
    buf_8 = (uint8_t *)rxbuf;
    buf_16 = (uint16_t *)rxbuf;
    buf_32 = (uint32_t *)rxbuf;

    int rxlen_byte;
    int txlen_byte;

    if (get_spidir(argc, argv) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    fd = open(spidir, O_RDWR);

    if (fd < 0)
    {
        perror ("open");
        return EXIT_FAILURE;
    }

    devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);

    if (parse_option(argc, argv) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    if (spidir == NULL)
    {
        printf("Please specify spi path.\n");
        return EXIT_FAILURE;
    }

    /* -c option */
    if (flag_opt & FLAG_CFG)
    {
        if (devctl(fd, DCMD_SPI_SLAVE_SET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL) != 0)
        {
            printf("Failed to configure driver\n");
        }
    }

    /* -i option */
    if (flag_opt & FLAG_INFO)
    {
        devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        printf("SPI Slave Driver\n");
        printf("id     = %d\n", cfg.id);
        printf("CPOL   = %d\n", cfg.cpol);
        printf("CPHA   = %d\n", cfg.cpha);
        printf("Bitlen = %d\n", cfg.bitlen);
    }

    /* -r option */
    if (flag_opt & FLAG_READ)
    {
        devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        rxlen_byte = rxlen;

        if (cfg.bitlen == SPI_BIT_LEN_16)
        {
            rxlen_byte *= 2;
        }
        else if (cfg.bitlen == SPI_BIT_LEN_32)
        {
            rxlen_byte *= 4;
        }
        while (1)
        {
            if ((ret = read(fd, rxbuf, rxlen_byte)) != -1)
                break;
        }
        if (ret == rxlen_byte)
        {
            printf("Read done!!!\n");
            printf("Read data:\n");

            for(int i = 0; i < rxlen; i ++)
            {
                if (cfg.bitlen == 8)
                {
                    printf("byte[%d] = 0x%02x\n", i, buf_8[i]);
                }
                else if (cfg.bitlen == 16)
                {
                    printf("byte[%d] = 0x%04x\n", i, buf_16[i]);
                }
                else
                {
                    printf("byte[%d] = 0x%08x\n", i, buf_32[i]);
                }
            }
        }
        else if ((ret >= 0) && (ret < rxlen_byte))
        {
            printf("Frame Sync Error\n");
        }
        if (ret < 0)
        {
            printf("Error\n");
        }

        close(fd);
        return EXIT_SUCCESS;
    }

    /* -w option */
    if (flag_opt & FLAG_WRITE)
    {
        devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        for (int i = 0; i < txlen; i++)
        {
            if (cfg.bitlen == SPI_BIT_LEN_8)
            {
                txbuf8[i] = (uint8_t)txbuf[i];
            }
            else if (cfg.bitlen == SPI_BIT_LEN_16)
            {
                txbuf16[i] = (uint16_t)txbuf[i];
            }
        }

        txlen_byte = txlen;

        if (cfg.bitlen == SPI_BIT_LEN_16)
        {
            txlen_byte *=2;
        }
        else if (cfg.bitlen == SPI_BIT_LEN_32)
        {
            txlen_byte *=4;
        }

        while(1)
        {
            if (cfg.bitlen == SPI_BIT_LEN_8)
            {
                if ((ret = write(fd, txbuf8, txlen_byte)) != -1)
                    break;
            }
            else if (cfg.bitlen == SPI_BIT_LEN_16)
            {
                if ((ret = write(fd, txbuf16, txlen_byte)) != -1)
                    break;
            }
            else
            {
                if ((ret = write(fd, txbuf, txlen_byte)) != -1)
                    break;
            }
        }

        if (ret == txlen_byte)
        {
            printf("Write done!!!\n");
            printf("Number of writen byte = %d\n", ret);
        }
        else if ((ret >= 0) && (ret < txlen_byte))
        {
            printf("Frame Sync Error\n");
        }
        if (ret < 0)
        {
            printf("Error\n");
        }

        close(fd);
        return EXIT_SUCCESS;
    }

    /* -x option */
    if (flag_opt & FLAG_XCHANGE)
    {
        devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        cfg.wordlen = txlen;
        devctl(fd, DCMD_SPI_SLAVE_BUS_RESET, NULL, 0, NULL);
        devctl(fd, DCMD_SPI_SLAVE_SET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);

        for(int i = 0; i < txlen; i++)
        {
            txdata.byte = txbuf[i];
            devctl(fd, DCMD_SPI_SLAVE_WRITE_BYTE, &txdata, sizeof(spi_data_t), NULL);
        }

        if (devctl(fd, DCMD_SPI_SLAVE_ENABLE_XCHANGE, NULL, 0, NULL) != 0)
        {
            printf("Failed to enable xchange\n");
            close(fd);
            return EXIT_SUCCESS;
        }

        int ret;
        while (1)
        {

            ret = devctl(fd, DCMD_SPI_SLAVE_READ_BYTE, &rxdata, sizeof(spi_data_t), NULL);
            if (ret == 0)
            {
                if (!(rxdata.status & SPI_STATUS_NO_DATA_AVAILABLE))
                {
                    rxbuf[count] = rxdata.byte;
                    count++;
                }

                if (rxdata.status & SPI_STATUS_SYNC_ERR)
                {
                    printf("Sync err\n");
                    break;
                }

                if (rxdata.status & SPI_STATUS_TRANSFER_DONE)
                {
                    break;
                }
            }
            else
            {
                printf("devctl READ_BYTE failed\n");
                break;
            }

        }
        printf("Read data:\n");
        for(int i = 0; i < count; i ++)
        {
            if (cfg.bitlen == 8)
            {
                printf("byte[%d] = 0x%02x\n", i, rxbuf[i]);
            }
            else if (cfg.bitlen == 16)
            {
                printf("byte[%d] = 0x%04x\n", i, rxbuf[i]);
            }
            else
            {
                printf("byte[%d] = 0x%08x\n", i, rxbuf[i]);
            }
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}


int get_spidir (int argc, char *argv[])
{
    if ((argc < 2) || (argv[1][0] == '-'))
    {
        printf("First argument must be spi directory.\n");
        return EXIT_FAILURE;
    }
    spidir = argv[1];
    return EXIT_SUCCESS;
}

static int parse_option(int argc, char *argv[])
{
    extern char *optarg;
    char *subopts, *value;
    char *substr,*str ;
    int opt;
    char *cfg_opts[] = {"cpol", "cpha", NULL };

    uint8_t j = 0;

    while ((opt = getopt(argc-1, &argv[1], "ic:w:r:x:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                flag_opt |= FLAG_INFO;
                break;
            case 'c':
                flag_opt |= FLAG_CFG;
                subopts = optarg;
                while (*subopts != '\0')
                {
                    switch (getsubopt(&subopts, cfg_opts, &value))
                    {
                        case 0:
                            cfg.cpol = atoi(value);
                            break;
                        case 1:
                            cfg.cpha = atoi(value);
                            break;
                        default:
                            printf("Unknow option\n");
                            return EXIT_FAILURE;
                    }
                }
                break;
            case 'w':
                if (optarg == NULL)
                {
                    printf("Nothing to write\n");
                    return EXIT_FAILURE;
                }
                flag_opt |= FLAG_WRITE;
                str = optarg;
                j = 0;
                while (1) //loop until reach end of string or max buffer
                {
                    substr = strstr(str, ":");
                    if (substr)
                        *substr = '\0';
                    txbuf[j++] = (uint32_t)strtoul(str,NULL,0);
                    if (j >= MAX_DATA_XFER || !substr)
                        break;
                    str = substr + 1;
                }
                txlen = j;
                rxlen = 0;
                break;
            case 'r':
                if (optarg == NULL)
                {
                    printf("Unknow read length\n");
                    return EXIT_FAILURE;
                }
                flag_opt |= FLAG_READ;
                rxlen = strtol(optarg, NULL, 0);
                txlen = 0;
                break;
            case 'x':
                if (optarg == NULL)
                {
                    printf("Nothing to write\n");
                    return EXIT_FAILURE;
                }
                flag_opt |= FLAG_XCHANGE;
                str = optarg;
                j = 0;
                while (1) //loop until reach end of string or max buffer
                {
                    substr = strstr(str, ":");
                    if (substr)
                        *substr = '\0';
                    txbuf[j++] = (uint32_t)strtoul(str,NULL,0);
                    if (j >= MAX_DATA_XFER || !substr)
                        break;
                    str = substr + 1;
                }
                txlen = j;
                rxlen = j;
                break;
            default:
                printf("Unknow option\n");
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
