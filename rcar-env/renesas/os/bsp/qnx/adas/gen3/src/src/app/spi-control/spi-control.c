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


/* NOTE:
 *
 *  "-c mode=" option for advanced user or debugging only, should not show up for general end user
 *
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

/* prototypes */
void hexdump(void *mem, unsigned int len);
void process_opts(int argc, char *argv[]);
int show_spi_info();

#define MAX_DATA_XFER   64 // max data buffer
#define DATA_SIZE_8BIT  8
#define DATA_SIZE_16BIT 16
#define DATA_SIZE_32BIT 32

char *spidir;           // device directory
uint8_t spidev  = 0;    // device id
uint8_t txlen   = 0;    // write data length
uint8_t rxlen   = 0;    // read data length
uint8_t txbuf[MAX_DATA_XFER]; //Tx buffer
uint8_t rxbuf[MAX_DATA_XFER]; //Rx buffer
uint32_t txbuf_32b[MAX_DATA_XFER];

uint8_t flag_opt   = 0;     // otpion flags
#define FLAG_CFG    (1<<0)
#define FLAG_INFO   (1<<1)
#define FLAG_DMA    (1<<2)
uint32_t cfg_mode  = 0;     // store 'mode' from cmd
uint32_t cfg_clock = 0;     // store 'clock' from cmd
uint32_t cfg_size  = 0;     //store 'size' from cmd
uint8_t verbose    = 0;

spi_devinfo_t dev_info; // store device info
spi_cfg_t cfg;          // store device config

int main(int argc, char *argv[])
{
    int fd,rc;
    int data_size = 0;
    memset(&txbuf[0], 0, sizeof(txbuf)); //Clear transmit buffer;
    memset(&rxbuf[0], 0, sizeof(rxbuf)); //Clear receive buffer;

    process_opts(argc, argv); 

    if (spidir == NULL)
    {
        fprintf(stderr,"Please specify spi path.\n");
        goto exit_error;
    }

    if ((fd = spi_open(spidir)) < 0)
    {
        fprintf(stderr,"Open '%s' failed. %s\n",spidir, strerror(errno));
        goto exit_error;
    }

    if (flag_opt & FLAG_INFO)
    {
        if(show_spi_info(fd)) goto exit_error1;
        goto exit_normal;
    }

    /* Process -c: set device config */
    if (flag_opt & FLAG_CFG)
    {
        //Get existing config
        rc = spi_getdevinfo(fd, spidev | SPI_DEV_DEFAULT, &dev_info);
        if (rc == EOK)
        {
            cfg = dev_info.cfg;
        }
        else
        {
            fprintf(stderr,"Get device info failed! %d\n",rc);
            goto exit_error1;
        }
        //Set new config
        if (cfg_mode) cfg.mode = cfg_mode | SPI_MODE_BODER_MSB;
        if (cfg_clock) cfg.clock_rate = cfg_clock;
        rc = spi_setcfg(fd, spidev, &cfg);
        if (rc != EOK)
        {
            fprintf(stderr,"Set device config failed! %d\n",rc);
            goto exit_error1;
        }
        if (verbose) printf("Set device config successful!\n");
        goto exit_normal;
    }

    if (flag_opt & FLAG_DMA)
    {
        if (txlen)
        {
            int     prot_flags = PROT_READ | PROT_WRITE | PROT_NOCACHE;
            int     map_flags  = MAP_PHYS | MAP_ANON | MAP_PRIVATE;
            void    *txvaddr;
            void	    *rxvaddr;
            off64_t txpaddr;
            off64_t rxpaddr;
            spi_drvinfo_t spi_drvinfo;
            int data_length;

            /* check if device supports DMA or not */
            spi_getdrvinfo(fd, &spi_drvinfo);
            if (!(spi_drvinfo.feature & SPI_FEATURE_DMA))
            {
                fprintf(stderr,"No support DMA!\n");
                goto exit_error1;
            }
            
            if(spi_getdevinfo(fd, spidev | SPI_DEV_DEFAULT, &dev_info) == EOK)
                data_size = dev_info.cfg.mode & SPI_MODE_CHAR_LEN_MASK;
            data_length = txlen*data_size/8;

            txvaddr = mmap(NULL, data_length, prot_flags, map_flags, NOFD, 0);
            mem_offset(txvaddr, NOFD, 1, &txpaddr, 0);
            if (data_size == DATA_SIZE_8BIT)
            {
                uint8_t * buf;
                buf = (uint8_t *)txvaddr;
                for (int i = 0; i < txlen; i ++)
                {
                    buf[i] = (uint8_t)txbuf_32b[i];
                }
            }
            else if(data_size == DATA_SIZE_16BIT)
            {
                uint16_t * buf;
                buf = (uint16_t *)txvaddr;
                for (int i = 0; i < txlen; i ++)
                {
                    buf[i] = (uint16_t)txbuf_32b[i];
                }
            }
            else if(data_size == DATA_SIZE_32BIT)
                memcpy(txvaddr, txbuf_32b, data_length); //buffer of driver can only handle 32 bit size data

            rxvaddr = mmap(NULL, txlen, prot_flags, map_flags, NOFD, 0);
            mem_offset(rxvaddr, NOFD, 1, &rxpaddr, 0);
          
            spi_dma_paddr_t dma_paddr;
            dma_paddr.rpaddr = rxpaddr;
            dma_paddr.wpaddr = txpaddr;
            int rc = spi_dma_xfer(fd, spidev, &dma_paddr , data_length);
            if(rc > 0) {
                if (verbose)
                {
                    printf("Read %d bytes\n", data_length);
                    printf("Command:\n");
                    hexdump(txvaddr, data_length);
                    printf("Receive\n");
                    hexdump(rxvaddr, data_length);
                }
            }
            else
            { 
                fprintf(stderr,"spi_dma_xfer failed! %d errno = %d\n", rc, errno);
                goto exit_normal;
            }

            goto exit_normal;
        }
    }
    else
    {
        /* Process -r: read data */
        if (txlen && rxlen)
        {
            rc = spi_cmdread(fd, spidev, txbuf,txlen, rxbuf,rxlen);
            if (rc != -1)
            {
                if (verbose)
                {
                    printf("Read %d bytes\n",rxlen);
                    printf("Command:\n");
                    hexdump(txbuf, txlen);
                    printf("Receive\n");
                    hexdump(rxbuf,rxlen);
                }
                //TODO: Output rx data to environment variable or file
            }
            else
            {
                fprintf(stderr,"spi_cmdread failed: %s\n",strerror(errno));
                goto exit_error1;
            }
            goto exit_normal;
        }

        /* Process -w: write data */
        if (txlen)
        {
            rc = spi_xchange(fd, spidev, txbuf, rxbuf, txlen);
            if (rc != -1)
            {
                if (verbose)
                {
                    printf("Exchanged  %d bytes\n",txlen);
                    printf("Transmit\n");
                    hexdump(txbuf, txlen);
                    printf("Receive\n");
                    hexdump(rxbuf,txlen);
                }
                //TODO: Output rx data to environment variable or file
            }
            else
            {
                fprintf(stderr,"spi_xchange failed: %s\n",strerror(errno));
                goto exit_error1;
            }
            goto exit_normal;
        }
    }

    fprintf(stderr,"No valid command.\nRun 'use spi-control' for command usage.\n");

exit_normal:
    spi_close(fd);
    return 0;

exit_error1:
    spi_close(fd);

exit_error:
    return -1;
}

int show_spi_info(int fd)
{
     spi_drvinfo_t drv_info;
    
     if(spi_getdrvinfo(fd, &drv_info) == EOK)
     {
         uint8_t ver_major, ver_minor,ver_rev;
         ver_major = (uint8_t)(drv_info.version >> SPI_VERMAJOR_SHIFT);
         ver_minor = (uint8_t)(drv_info.version >> SPI_VERMINOR_SHIFT);
         ver_rev = (uint8_t)(drv_info.version >> SPI_VERREV_SHIFT);
         printf("Driver   : %s v%d.%d%d\n",drv_info.name, ver_major,ver_minor,ver_rev);
     }
     else
     {
         fprintf(stderr,"Get driver info failed!\n");
         return -1;
     }

     if(spi_getdevinfo(fd, spidev | SPI_DEV_DEFAULT, &dev_info) == EOK)
     {
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
     }
     else
     {
         fprintf(stderr,"Get device info failed!\n");
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
    uint8_t j = 0;
    int opt;

    if ((argc < 2) || (argv[1][0] == '-'))
    {
        fprintf(stderr,"First argument must be spi directory.\n");
        return;
    }
    spidir = argv[1];
    while ((opt = getopt(argc-1, &argv[1], "s:ic:w:r:d:v")) != -1)
    {
        switch (opt)
        {
        case 'd':
            flag_opt |= FLAG_DMA;
            if ((optarg != NULL) && (txlen==0))
            {
                j = 0;
                str = optarg; //str = strdup(optarg);
                while (1) //loop until reach end of string or max buffer
                {
                    substr = strstr(str, ":");
                    if (substr) *substr = '\0';
                    txbuf[j] = (uint8_t)strtoul(str,NULL,16);
                    txbuf_32b[j] = (uint32_t)strtoul(str,NULL,16);
                    j++;
                    if (j >= MAX_DATA_XFER || !substr) break;
                    str = substr + 1;
                }
                txlen  = j;
            }
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
            while (*subopts != '\0')
            {
                switch (getsubopt(&subopts, cfg_opts, &value))
                {   
                case 0: //"clock"
                    cfg_clock = (uint32_t)atol(value);
                    break;

                case 1: //"mode"
                	if(value[0] == '0')
                	{
                		cfg_mode &= ~(SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF);
                    }
                	else if(value[0] == '1')
                	{
                        cfg_mode = SPI_MODE_CKPHASE_HALF;
                    }
                	else if(value[0] == '2')
                	{
                		cfg_mode = SPI_MODE_CKPOL_HIGH;
                    }
                	else if(value[0] == '3')
                	{
                		cfg_mode = SPI_MODE_CKPOL_HIGH | SPI_MODE_CKPHASE_HALF;
                    }
                	else
					{
                		break;
                	}
                    cfg_mode |= cfg_size & SPI_MODE_CHAR_LEN_MASK;
					break;
                case 2: //"size"
                    cfg_size = (uint8_t)atoi(value);
                    if(!(cfg_size == DATA_SIZE_8BIT || cfg_size == DATA_SIZE_16BIT || cfg_size == DATA_SIZE_32BIT))
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
        case 'w':
            if ((optarg != NULL) && (txlen==0))
            {
                j = 0;

                str = optarg; //str = strdup(optarg);
                while (1) //loop until reach end of string or max buffer
                {
                    substr = strstr(str, ":");
                    if (substr) *substr = '\0';
                    txbuf[j++] = (uint8_t)strtoul(str,NULL,16);
                    if (j >= MAX_DATA_XFER || !substr) break;
                    str = substr + 1;
                }
                txlen  = j;
            }
            break;
        case 'r':
            if ((optarg != NULL) && (txlen==0))
            {
                subopts = optarg;
                while (*subopts != '\0')
                    switch (getsubopt(&subopts, cmdrx_opts, &value))
                    {
                    case 0: //"cmd"
                        j = 0;
                        str = value; //str = strdup(value);
                        while (1) //loop until reach end of string or max buffer
                        {
                            substr = strstr(str, ":");
                            if (substr) *substr = '\0';
                            txbuf[j++] = (uint8_t)strtoul(str,NULL,16);
                            if (j >= MAX_DATA_XFER || !substr) break;
                            str = substr + 1;
                        }
                        txlen = j;
                        break;
                    case 1: //"rlen"
                        rxlen = (uint8_t)atoi(value);
                        if (rxlen >  MAX_DATA_XFER)
                        {
                            fprintf(stderr,"Warning: rlen exceed MAX_DATA_XFER: %d.\n", MAX_DATA_XFER);
                            rxlen = MAX_DATA_XFER;
                        }
                        break;
                    default:
                        fprintf(stderr,"Unknown -r sub option '%s'\n", value);
                        break;
                    }

                if ((txlen==0) || (rxlen==0))
                {
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

void hexdump(void *mem, unsigned int len)
{
#define HEXDUMP_COLS 8
    unsigned int i, j;

    printf("       0  1  2  3  4  5  6  7  ASCII\n");
    for (i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
    {
        /* print offset */
        if (i % HEXDUMP_COLS == 0)
        {
            printf(" 0x%02x: ", i);
        }

        /* print hex data */
        if (i < len)
        {
            printf("%02x ", 0xFF & ((char*) mem)[i]);
        }
        else /* end of hex data, padding ASCII dump */
        {
            printf("   ");
        }

        /* print ASCII */
        if (i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
        {
            for (j = i - (HEXDUMP_COLS - 1); j <= i; j++)
            {
                if (j >= len) /* end of block, not really printing */
                {
                    putchar(' ');
                }
                else if (isprint(((char*) mem)[j])) /* printable char */
                {
                    putchar(0xFF & ((char*) mem)[j]);
                }
                else /* other char */
                {
                    putchar('.');
                }
            }
            putchar('\n');
        }
    }
}
