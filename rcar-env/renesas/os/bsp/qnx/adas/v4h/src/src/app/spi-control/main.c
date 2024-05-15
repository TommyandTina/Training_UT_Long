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
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <sys/mman.h>
#include <stdint.h>
#include <ctype.h>

#include <hw/spi-master.h>
#include <wchar.h>
#include <sys/slog.h>

/* Prototypes */
void hexdump(void *mem, uint16_t len);
void process_opts(int argc, char *argv[]);
int show_spi_info();
#define RCAR_SPI_FIFO_SIZE      256 
#define DATA_SIZE_8BIT          8
#define DATA_SIZE_16BIT         16
#define DATA_SIZE_32BIT         32

char *spidir;           // device directory
uint8_t spidev  = 0;    // device id
uint16_t txlen   = 0;    // write data length
uint16_t rxlen   = 0;    // read data length
uint8_t txbuf[RCAR_SPI_FIFO_SIZE*4]; //Tx buffer
uint8_t rxbuf[RCAR_SPI_FIFO_SIZE*4]; //Rx buffer
uint8_t flag_opt   = 0;     // otpion flags
#define FLAG_CFG    (1<<0)
#define FLAG_INFO   (1<<1)
#define FLAG_DMA    (1<<2)
#define FLAG_XDMA   (1<<3)
uint32_t cfg_mode  = (SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF | SPI_MODE_BODER_MSB);    // store 'mode' from cmd
uint32_t cfg_clock = 1000000;     			// store 'clock' from cmd
uint32_t cfg_size  = DATA_SIZE_8BIT;     //store 'size' from cmd
uint8_t verbose    = 0;

uint16_t data_size = 0;
// fifo size for transfer is 256*4 bytes data(R-CarV4h) or is 64*4 bytes data(R-CarGen3/S4/V3U)
uint32_t fifo_size = 256;

spi_devinfo_t dev_info; // store device info
spi_cfg_t cfg;          // store device config

int main(int argc, char *argv[])
{
    int fd,rc;
    int typed_mem_fd = -1;
    void    *txvaddr;
    void	*rxvaddr;

    memset(&txbuf[0], 0, sizeof(txbuf)); //Clear transmit buffer;
    memset(&rxbuf[0], 0, sizeof(rxbuf)); //Clear receive buffer;

    spidir = argv[1];
    if (spidir == NULL) {
        fprintf(stderr,"SPI master: please specify spi path.\n");
        return 1;
    }
    if ((fd = spi_open(spidir)) < 0) {
        fprintf(stderr,"SPI master: open '%s' failed. %s\n",spidir, strerror(errno));
       return 1;
    }
    if (spi_getdevinfo(fd, spidev | SPI_DEV_DEFAULT, &dev_info) == EOK) {
        data_size = dev_info.cfg.mode & SPI_MODE_CHAR_LEN_MASK;
    }
    process_opts(argc, argv); 

    if (flag_opt & FLAG_INFO) {
        if(show_spi_info(fd)) goto exit_error1;
        goto exit_normal;
    }

    /* Process -c: set device config */
    if (flag_opt & FLAG_CFG){
        //Get existing config
        rc = spi_getdevinfo(fd, spidev | SPI_DEV_DEFAULT, &dev_info);
        if (rc == EOK) {
            cfg = dev_info.cfg;
        } else {
            fprintf(stderr,"SPI master: Get device info failed! %d\n",rc);
            goto exit_error1;
        }
        //Set new config
        if (cfg_mode) cfg.mode = cfg_mode;
        if (cfg_clock) cfg.clock_rate = cfg_clock;
        rc = spi_setcfg(fd, spidev, &cfg);
        if (rc != EOK) {
            fprintf(stderr,"SPI master: set device config failed! %d\n",rc);
            goto exit_error1;
        }
        if (verbose) printf("SPI master: set device config successful!\n");
        goto exit_normal;
    }

    if (flag_opt & FLAG_DMA) {
        if (txlen || rxlen) {
            int     prot_flags = PROT_READ | PROT_WRITE | PROT_NOCACHE;
            int     map_flags  = MAP_SHARED;
            int     len = txlen ? txlen : rxlen;
            off64_t txpaddr;
            off64_t rxpaddr;
            spi_drvinfo_t spi_drvinfo;

            /* check if device supports DMA or not */
            spi_getdrvinfo(fd, &spi_drvinfo);
            if (!(spi_drvinfo.feature & SPI_FEATURE_DMA)) {
                fprintf(stderr,"SPI master: no support DMA transfer\n");
                goto exit_error1;
            }

            typed_mem_fd = posix_typed_mem_open("below4G", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
            if (typed_mem_fd == -1) {
                fprintf(stderr, "SPI master: Unable to find typed mem sysram&below4G: , error:  %s\n",strerror(errno));
                goto exit_error1;
            }

            txvaddr = mmap(NULL, len, prot_flags, map_flags, typed_mem_fd, 0);               
            rxvaddr = mmap(NULL, len, prot_flags, map_flags, typed_mem_fd, 0);

            if ((rxvaddr== MAP_FAILED)  || (txvaddr==MAP_FAILED)) {
                close(typed_mem_fd);
                fprintf(stderr, "SPI master: mmap failed: %s\n", strerror(errno));
                goto exit_error1;
            }
            rc =mem_offset(txvaddr, NOFD, 1, &txpaddr, 0);
            if (rc == -1) {
                fprintf(stderr, "SPI master: mem_offset failed: , error:  %s\n",strerror(errno));
                close(typed_mem_fd);
                goto exit_error1;
            }
            rc = mem_offset(rxvaddr, NOFD, 1, &rxpaddr, 0);
            if (rc == -1) {
                fprintf(stderr, "SPI master: mem_offset failed: , error:  %s\n",strerror(errno));
                close(typed_mem_fd);
                goto exit_error1;
            }
            if (txlen) {
                memcpy(txvaddr, txbuf, len);
            } else {
                memset(txvaddr, 0, len);
                memset(rxvaddr, 0, len);
            }

            spi_dma_paddr_t dma_paddr;
            dma_paddr.rpaddr = rxpaddr;
            dma_paddr.wpaddr = txpaddr;

            int rc = spi_dma_xfer(fd, spidev, &dma_paddr , len);
            if (rc > 0) {
                if (verbose) {
                    if (txlen) {
                        fprintf(stdout,"SPI master: Transmit (%d bytes)\n", len);
                        hexdump(txvaddr, len);
                    } else {
                        fprintf(stdout,"SPI master: Receive (%d bytes)\n", len);
                        hexdump(rxvaddr, len);
                    }
                }
            } else {
                fprintf(stderr,"SPI master: spi_dma_xfer failed! %d errno = %d\n", rc, errno);
                close(typed_mem_fd);
                goto exit_normal;
            }
            close(typed_mem_fd);
            goto exit_normal;
        }
    } else if (flag_opt & FLAG_XDMA) {
        if (txlen) {
            int     prot_flags = PROT_READ | PROT_WRITE | PROT_NOCACHE;
            int     map_flags  = MAP_SHARED;
            spi_drvinfo_t spi_drvinfo;

            /* Check if device supports DMA or not */
            spi_getdrvinfo(fd, &spi_drvinfo);
            if (!(spi_drvinfo.feature & SPI_FEATURE_DMA)) {
                fprintf(stderr,"SPI master: no support DMA transfer\n");
                goto exit_error1;
            }
            typed_mem_fd = posix_typed_mem_open("below4G", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG);
            if (typed_mem_fd == -1) {
                fprintf(stderr, "SPI master: Unable to find typed mem sysram&below4G: , error:  %s\n",strerror(errno));
                goto exit_error1;
            }

            txvaddr = mmap(NULL, txlen, prot_flags, map_flags, typed_mem_fd, 0);               
            rxvaddr = mmap(NULL, txlen, prot_flags, map_flags, typed_mem_fd, 0);

            if ((rxvaddr== MAP_FAILED)  || (txvaddr==MAP_FAILED)) {
                close(typed_mem_fd);
                fprintf(stderr, "SPI master: mmap failed: %s\n", strerror(errno));
                goto exit_error1;
            }
            memcpy(txvaddr, txbuf, txlen);
            memset(rxvaddr, 0, txlen);

            int rc = spi_dma_xchange(fd, spidev, txvaddr,rxvaddr , txlen);
            if (rc > 0) {
                if (verbose) {
                    fprintf(stdout,"SPI master: Exchanged  %d bytes\n", txlen);
                    fprintf(stdout,"SPI master: Transmit\n");
                    hexdump(txvaddr, txlen);
                    fprintf(stdout,"SPI master: Receive\n");
                    hexdump(rxvaddr, txlen);                   
                }
            } else {
                fprintf(stderr,"SPI master: spi_dma_xchange failed,: %s\n", strerror(errno) );
                close(typed_mem_fd);
                goto exit_normal;
            }
            close(typed_mem_fd);
            goto exit_normal;
        }
    } else {
        /* Process -m: read data with command */
        if (txlen && rxlen) {
            rc = spi_cmdread(fd, spidev, txbuf, txlen, rxbuf, rxlen);
            if (rc != -1) {
                if (verbose) {
                    fprintf(stdout,"SPI master: Read %d bytes\n", rxlen);
                    fprintf(stdout,"SPI master: Command:\n");
                    hexdump(txbuf, txlen);
                    fprintf(stdout,"SPI master: Receive\n");
                    hexdump(rxbuf,rxlen);
                }
                //TODO: Output rx data to environment variable or file
            } else {
                fprintf(stderr,"SPI master: : spi_cmdread failed: %s\n",strerror(errno));
                goto exit_error1;
            }
            goto exit_normal;
        }
        /* Process -r: read data */
        if (rxlen){
            rc = spi_read(fd, spidev, rxbuf, rxlen);
            if (rc != -1) {
                if (verbose) {
                    fprintf(stdout,"SPI master: Receive (%d bytes)\n", rxlen);
                    hexdump(rxbuf,rxlen);
                }
                //TODO: Output rx data to environment variable or file
            } else {
                fprintf(stderr,"SPI master: : spi_read failed: %s\n",strerror(errno));
                goto exit_error1;
            }
            goto exit_normal;
        }
        /* Process -w: write data */
        if (txlen) {
            rc = spi_xchange(fd, spidev, txbuf, rxbuf, txlen);
            if (rc != -1) {
                if (verbose) {
                    fprintf(stdout,"SPI master: Transmit (%d bytes)\n", txlen);
                    hexdump(txbuf, txlen);
                }
                //TODO: Output rx data to environment variable or file
            } else {
                fprintf(stderr,"SPI master: spi_xchange failed: %s\n",strerror(errno));
                goto exit_error1;
            }
            goto exit_normal;
        }
    }

    fprintf(stderr,"SPI master: No valid command.\nRun 'use spi-control' for command usage.\n");

exit_normal:
    spi_close(fd);
    return 0;

exit_error1:
    spi_close(fd);
    return -1;
}

int show_spi_info(int fd)
{
    spi_drvinfo_t drv_info;
    
    if (spi_getdrvinfo(fd, &drv_info) == EOK) {
         uint8_t ver_major, ver_minor,ver_rev;
         ver_major = (uint8_t)(drv_info.version >> SPI_VERMAJOR_SHIFT);
         ver_minor = (uint8_t)(drv_info.version >> SPI_VERMINOR_SHIFT);
         ver_rev = (uint8_t)(drv_info.version >> SPI_VERREV_SHIFT);
         printf("SPI master driver   : %s v%d.%d%d\n",drv_info.name, ver_major,ver_minor,ver_rev);
    } else {
        fprintf(stderr,"SPI master: Get driver info failed!\n");
        return -1;
    }

    if (spi_getdevinfo(fd, spidev | SPI_DEV_DEFAULT, &dev_info) == EOK) {
        printf(" Device  : %d\n",dev_info.device);
        printf(" Name    : %s\n",dev_info.name);
        printf(" Config\n");
        printf("  Clock  : %d\n",dev_info.cfg.clock_rate);
        printf("  Mode   : 0x%x\n",dev_info.cfg.mode);
    //Print out which modes was applied
    if (dev_info.cfg.mode & SPI_MODE_CKPOL_HIGH)
        printf("\tSPI_MODE_CKPOL_HIGH\n");
    if (dev_info.cfg.mode & SPI_MODE_CKPHASE_HALF)
        printf("\tSPI_MODE_CKPHASE_HALF\n");
    if (dev_info.cfg.mode & SPI_MODE_BODER_MSB)
        printf("\tSPI_MODE_BODER_MSB\n");
    if (dev_info.cfg.mode & SPI_MODE_CSPOL_HIGH)
        printf("\tSPI_MODE_CSPOL_HIGH\n");
    if (dev_info.cfg.mode & SPI_MODE_CSSTAT_HIGH)
        printf("\tSPI_MODE_CSSTAT_HIGH\n");
    if (dev_info.cfg.mode & SPI_MODE_CSHOLD_HIGH)
        printf("\tSPI_MODE_CSHOLD_HIGH\n");
    if ((dev_info.cfg.mode & SPI_MODE_RDY_MASK) == SPI_MODE_RDY_NONE)
        printf("\tSPI_MODE_RDY_NONE\n");
    if ((dev_info.cfg.mode & SPI_MODE_RDY_MASK) == SPI_MODE_RDY_EDGE)
        printf("\tSPI_MODE_RDY_EDGE\n");
    if ((dev_info.cfg.mode & SPI_MODE_RDY_MASK) == SPI_MODE_RDY_LEVEL)
        printf("\tSPI_MODE_RDY_LEVEL\n");
    if (dev_info.cfg.mode & SPI_MODE_IDLE_INSERT)
        printf("\tSPI_MODE_IDLE_INSERT\n");
    if (dev_info.cfg.mode & SPI_MODE_LOCKED)
        printf("\tSPI_MODE_LOCKED\n");
    }else{
        printf("SPI master: Get device info failed!\n");
        return -1;
    }

    return 0;
}

void process_opts(int argc, char *argv[])
{
    extern char *optarg;
    char *cfg_opts[] = {"clock", "mode", "size", NULL };
    char *cmdrx_opts[] = {"cmd", "rlen", NULL };
    char *subopts, *value;
    char *substr,*str ;
    uint16_t j = 0;
    int opt, rnd = 0;

    if ((argc < 2) || (argv[1][0] == '-')) {
        fprintf(stderr,"First argument must be spi directory.\n");
        return;
    }

    txlen = rxlen = 0;

    while ((opt = getopt(argc-1, &argv[1], "s:ic:f:w:o:W:r:m:x:dv")) != -1) {
        switch (opt){
        case 'd':
            flag_opt |= FLAG_DMA;
            break;
        case 's':
            spidev = atoi(optarg);
            break;
        case 'i':
            flag_opt |= FLAG_INFO;
            break;
        case 'c':
            flag_opt |= FLAG_CFG;
            subopts = optarg;
            while (*subopts != '\0') {
                switch (getsubopt(&subopts, cfg_opts, &value))
                {   
                case 0: //"clock"
                    cfg_clock = (uint32_t)atol(value);
                    break;

                case 1: //"mode"
                    if (value[0] == '0') {
                        cfg_mode &= ~(SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF);
                    } else if(value[0] == '1') {
                        cfg_mode = SPI_MODE_CKPHASE_HALF;
                    } else if(value[0] == '2') {
                        cfg_mode = SPI_MODE_CKPOL_HIGH;
                    } else if(value[0] == '3') {
                        cfg_mode = SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF;
                    } else {
                        break;
                    }
                    cfg_mode |= cfg_size & SPI_MODE_CHAR_LEN_MASK;
                    break;
                case 2: //"size"
                    cfg_size = (uint8_t)atoi(value);
                    if (!(cfg_size == DATA_SIZE_8BIT || cfg_size == DATA_SIZE_16BIT || cfg_size == DATA_SIZE_32BIT))
                    {
                        fprintf(stderr,"Not support data size '%s', set to 8bit size\n", value);
                        cfg_size = DATA_SIZE_8BIT;    
                    }
                    cfg_mode |= cfg_size & SPI_MODE_CHAR_LEN_MASK;
                    break;    
                default:
                    fprintf(stderr,"Unknown -c sub option '%s'\n", value);
                    break;
                }
            }
            break;
        case 'f':
            fifo_size = atoi(optarg);
            if (fifo_size==0) {
                fifo_size = 64;
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
            break;
        case 'o':
            rnd = atoi(optarg);
            srand(rnd);
            for (j=0;j<txlen;j++) {
                txbuf[j] = rand();
            }
            break;
        case 'W':
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
            break;
        case 'r':
            rxlen = atoi(optarg);
            if (rxlen==0) {
                rxlen = 8;
            }
            for (j=0;j<rxlen;j++) {
                rxbuf[j] = 0;
            }
            break;
        case 'x':
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
            flag_opt |= FLAG_XDMA;
            break;
        case 'm':
            if ((optarg != NULL) && (txlen==0)) {
                subopts = optarg;
                while (*subopts != '\0')
                    switch (getsubopt(&subopts, cmdrx_opts, &value))
                    {
                    case 0: //"cmd"
                        j = 0;
                        str = value; //str = strdup(value);
                        while (1){ //loop until reach end of string or max buffer
                            substr = strstr(str, ":");
                            if (substr) *substr = '\0';
                            txbuf[j++] = strtoul(str,NULL,16);
                            if (j >= fifo_size || !substr) break;
                            str = substr + 1;
                        }
                        txlen = j;
                        break;
                    case 1: //"rlen"
                        rxlen = (uint16_t)atoi(value);
                        if (rxlen==0) {
                            rxlen = 8;
                        }
                        if (rxlen >  (fifo_size * (data_size/DATA_SIZE_8BIT))) {
                            fprintf(stderr,"Warning: rlen >: %d.\n", (int)fifo_size);
                            rxlen = fifo_size * (data_size/DATA_SIZE_8BIT);
                        }
                        break;
                    default:
                        fprintf(stderr,"Unknown -r sub option '%s'\n", value);
                        break;
                    }

                if ((txlen==0) || (rxlen==0)) {
                    fprintf(stderr,"Both 'cmd' & 'rlen' must be specficed for -r argument.\n");
                    txlen = 0;
                    rxlen = 0;
                }
            }
            break;
        case 'v':
            verbose=1;
            break;
        default:
            break;
        }
    }
}

void hexdump(void *mem, uint16_t len)
{
    for (int i = 0; i < len ; i++) {
         fprintf(stdout,"%02x ",((uint8_t*)mem)[i]);
    }
    fprintf(stdout,"\n");
}
