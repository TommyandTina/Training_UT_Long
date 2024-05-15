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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <devctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/resmgr.h>
#include <sys/procmgr.h>
#include <hw/spi_slave.h>

#define FLAG_INFO     1
#define FLAG_CFG      2
#define FLAG_WRITE    4
#define FLAG_READ     8
#define FLAG_XCHANGE  16


/* Global variable */
char * spidir;
uint8_t flag_opt = 0;
uint16_t txlen    = 0;    // write data length
uint16_t rxlen    = 0;    // read data length

uint8_t txbuf[MSIOF_SPI_FIFO_SIZE_256*4]; //Tx buffer
uint8_t rxbuf[MSIOF_SPI_FIFO_SIZE_256*4]; //Rx buffer

int cpol = 0;
int cpha = 0;
int bitlen = 8;
uint16_t fifo_size = MSIOF_SPI_FIFO_SIZE_64;
spi_slave_cfg_t cfg;
int verbose = 0;

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
    int fd = -1;
    int ret = 0;
    int count = 0;

    procmgr_daemon( 0,PROCMGR_DAEMON_NOCHDIR|PROCMGR_DAEMON_NOCLOSE|PROCMGR_DAEMON_NODEVNULL|PROCMGR_DAEMON_KEEPUMASK);

    fifo_size = MSIOF_SPI_FIFO_SIZE_64;

    if (get_spidir(argc, argv) != 0)
        return EXIT_FAILURE;

    fd = open(spidir, O_RDWR);

    if (fd < 0) {
        printf ("SPI slave: open device %s failed: %s\n",spidir,strerror(errno));
        return EXIT_FAILURE;
    }

    ret = devctl(fd, DCMD_SPI_SLAVE_GET_FIFO_SIZE, &fifo_size, sizeof(uint32_t), NULL);
    if (ret) {
        printf("SPI slave: DCMD_SPI_SLAVE_GET_FIFO_SIZE failed\n");
    }
    ret = devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
    if (ret) {
        printf("SPI slave: DCMD_SPI_SLAVE_GET_CONFIG failed\n");
    }
    if (parse_option(argc, argv) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (spidir == NULL) {
        printf("SPI slave: Please specify spi path.\n");
        return EXIT_FAILURE;
    }

    /* -c option */
    if (flag_opt & FLAG_CFG) {
        if (devctl(fd, DCMD_SPI_SLAVE_SET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL) != 0) {
            printf("SPI slave: DCMD_SPI_SLAVE_SET_CONFIG failed\n");
        }
    }

    /* -i option */
    if (flag_opt & FLAG_INFO) {
        devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        printf("SPI Slave Driver\n");
        printf("id     = %d\n", cfg.id);
        printf("CPOL   = %d\n", cfg.cpol);
        printf("CPHA   = %d\n", cfg.cpha);
        printf("Bitlen = %d\n", cfg.bitlen);
    }

    /* -r option */
    if (flag_opt & FLAG_READ) {
        devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        ret = read(fd, rxbuf, rxlen);
        if (ret == rxlen) {
            //usleep(10000);
            fprintf(stdout,"\nSPI slave: read data:\n");          
            for (int i = 0; i < rxlen; i ++) {
                fprintf(stdout,"%02x ",rxbuf[i]);
            }
            fprintf(stdout,"\n");
        } else if (ret == 0) {
            fprintf(stdout,"\nSPI slave: Read timeout\n");
        } else if (ret > 0) {
            fprintf(stdout,"\nSPI slave: Frame Sync Error\n");
        }
        if (ret < 0) {
            fprintf(stdout,"\nSPI slave: Error\n");
        }

        close(fd);
        return EXIT_SUCCESS;
    }

    /* -w option */
    if (flag_opt & FLAG_WRITE) {
        devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        if (verbose) {
            fprintf(stdout,"\nSPI slave: transmiting data:\n");          
            for (int i = 0; i < txlen; i ++) {
                fprintf(stdout,"%02x ", txbuf[i]);
            }
            fprintf(stdout,"\n");
        }
        ret = write(fd, txbuf, txlen);
        if (ret == txlen) {
            //printf("\nSPI slave: number of writen byte = %d\n", ret);
        } else if ((ret >= 0) && (ret < txlen)) {
            printf("\nSPI slave: Frame Sync Error: \n");
        }
        if (ret < 0) {
            printf("\nSPI slave: Write data failed: error: %d \n",errno);
        }

        close(fd);
        return EXIT_SUCCESS;
    }

    /* -x option */
    if (flag_opt & FLAG_XCHANGE) {
        ret = devctl(fd, DCMD_SPI_SLAVE_GET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        if (ret) {
            printf("SPI slave: DCMD_SPI_SLAVE_GET_CONFIG failed\n");
            return EXIT_FAILURE ;
        }
        cfg.wordlen = txlen/cfg.bitlen;
        ret = devctl(fd, DCMD_SPI_SLAVE_BUS_RESET, NULL, 0, NULL);
        if(ret){
            printf("SPI slave: DCMD_SPI_SLAVE_BUS_RESET failed\n");
            return EXIT_FAILURE ;
        }
        ret = devctl(fd, DCMD_SPI_SLAVE_SET_CONFIG, &cfg, sizeof(spi_slave_cfg_t), NULL);
        if (ret) {
            printf("SPI slave: DCMD_SPI_SLAVE_SET_CONFIG failed\n");
            return EXIT_FAILURE ;
        }
        for (int i = 0; i < txlen; i++) {
            txdata.byte = txbuf[i];
            ret = devctl(fd, DCMD_SPI_SLAVE_WRITE_BYTE, &txdata, sizeof(spi_data_t), NULL);
            if (ret) {
                printf("SPI slave: DCMD_SPI_SLAVE_WRITE_BYTE failed\n");
                return EXIT_FAILURE ;
            }
        }
        ret = devctl(fd, DCMD_SPI_SLAVE_ENABLE_XCHANGE, NULL, 0, NULL);
        if (ret) {
            printf("SPI slave: DCMD_SPI_SLAVE_ENABLE_XCHANGE failed\n");
            close(fd);
            return EXIT_FAILURE;
        }

        int ret;
        while (1) {

            ret = devctl(fd, DCMD_SPI_SLAVE_READ_BYTE, &rxdata, sizeof(spi_data_t), NULL);
            if (ret == 0) {
                if (!(rxdata.status & SPI_STATUS_NO_DATA_AVAILABLE)){
                    rxbuf[count] = rxdata.byte;
                    count++;
                }

                if (rxdata.status & SPI_STATUS_SYNC_ERR) {
                    printf("\nSPI slave: Sync err\n");
                    break;
                }

                if (rxdata.status & SPI_STATUS_TRANSFER_DONE) {
                    break;
                }
            } else {
                printf("\nSPI slave: DCMD_SPI_SLAVE_READ_BYTE failed\n");
                close(fd);
                return EXIT_FAILURE;
            }

        }
        usleep(10000);
        printf("\nSPI slave: read data:\n");
        for (int i = 0; i < count; i ++) {
            printf("0x%02x ", rxbuf[i]);
        }

    }

    close(fd);
    return EXIT_SUCCESS;
}

int get_spidir (int argc, char *argv[])
{
    if ((argc < 2) || (argv[1][0] == '-')) {
        printf("First argument must be spi directory.\n");
        return -1;
    }
    spidir = argv[1];
    return 0;
}

static int parse_option(int argc, char *argv[])
{
    extern char *optarg;
    char *subopts, *value;
    int opt, rnd = 0;
    char *cfg_opts[] = {"cpol", "cpha", NULL };
    char *substr,*str;
    uint8_t j = 0;

    while ((opt = getopt(argc-1, &argv[1], "ic:w:r:x:W:o:v")) != -1) {
        switch (opt) {
            case 'i':
                flag_opt |= FLAG_INFO;
                break;
            case 'c':
                flag_opt |= FLAG_CFG;
                subopts = optarg;
                while (*subopts != '\0') {
                    switch (getsubopt(&subopts, cfg_opts, &value)) {
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
                txlen = atoi(optarg);
                if (txlen==0) {
                    txlen = 8;
                }
                for (j=0;j<txlen;j++) {
                    if (rnd == 0) {
                        txbuf[j] = j;
                    } else {
                        txbuf[j] = rand();
                    }
                }
                rxlen = 0;
                flag_opt |= FLAG_WRITE;
                break;
            case 'r':
                rxlen = (uint16_t)atoi(optarg);
                if (rxlen==0) {
                    rxlen = 8;
                    fprintf(stderr,"Warning: Specified read 0 byte, will read 8 bytes\n");
                }
                if (rxlen >  fifo_size*(cfg.bitlen/SPI_BIT_LEN_8)) {
                    fprintf(stderr,"Warning: Specify byte number of data > %d.\n", (int)fifo_size*(cfg.bitlen/SPI_BIT_LEN_8));
                    rxlen = fifo_size*(cfg.bitlen/SPI_BIT_LEN_8);
                }
                txlen = 0;
                flag_opt |= FLAG_READ;
                break;
            case 'x':
                txlen = atoi(optarg);
                if (txlen==0) {
                    txlen = 8;
                    fprintf(stderr,"Warning: Specify transfer 0 data byte, will transfer 8 bytes\n");
                }
                for (j=0;j<txlen;j++) {
                    if (rnd == 0) {
                        txbuf[j] = j;
                    } else {
                        txbuf[j] = rand();
                    }
                }
                rxlen = txlen;
                flag_opt |= FLAG_XCHANGE;
                break;
            case 'W':
                if (optarg == NULL)
                {
                    printf("Nothing to write\n");
                    return EXIT_FAILURE;
                }
                flag_opt |= FLAG_WRITE;
                str = optarg;
                j = 0;
                while (str) //loop until reach end of string or max buffer
                {
                    txbuf[j++] = (uint8_t)strtoul(str, NULL, 0);
                    substr = strstr(str, ":");
                    str = substr;
                    if (str) str++;
                }
                txlen = j;
                rxlen = 0;
                break;
            case 'o':
                rnd = atoi(optarg);
                srand(rnd);
                if (txlen > 0) {
                    for (j=0;j<txlen;j++) {
                        txbuf[j] = rand();
                    }
                }
            case 'v':
                verbose=1;
                break;
            default:
                printf("Unknow option\n");
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
