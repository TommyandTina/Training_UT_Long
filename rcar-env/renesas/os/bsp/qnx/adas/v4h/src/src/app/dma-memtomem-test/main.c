/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <hw/dma.h>


#define DESC_NUM (128)

#define DMA_WIDTH (1280)
#define DMA_HEIGHT (960)
#define DMA_SIZE (DMA_WIDTH * DMA_HEIGHT * 3)


#define FORMAT_NUMBER   (10)

#define CHECK_SAMPLING  (1024)
#define DMA_PULSE  55

#define DES 1
#define INTER 1

/* Function decleration */
static dma_functions_t dmafuncs;
static dma_transfer_t tinfo;
static void *xferdma;
static int do_dma(off64_t dst_paddr, off64_t src_paddr, int size, int desc_num);

/* Global variable */
int     iid;
int     chid;
int     use_intr = 0;

int burst_unit = 0;
int buffer_size = 0;

struct timespec tp_sta;
struct timespec tp_end;

static void usage()
{
    printf("Syntax:\n");
    printf("# dma-memtomem-test dma=<dma_type>[,desc=<desc_num>][,chan=<chan_idx>][,event][,debug][,mbu=<burst_unit>][,ulb=<buffer_size>]\n");
    printf("\n");
    printf("Options:\n");
    printf("   <dma_type>    : rt or sys\n");
    printf("   <desc_num>    : descriptor number (1-255)\n");
    printf("   <chan_idx>    : channel index (depends on SoC, it can be 0-64)\n");
    printf("                 :   If not specify, will attach any free channel\n");
    printf("                 :   Not specify is default \n");
    printf("   event         : If specfiy, will use use to check completion\n");
    printf("   debug         : If specfiy, will enable register dump\n");
    printf("                 :   Disable is default\n");
    printf("   <burst_unit>  : Maximum burst unit for SDRAM, power-of-two values are recommended\n");
    printf("                 :  Default is 128 (bytes), maximum is 512 (bytes)\n");
    printf("   <buffer_size> : Upper limit on buffer size, power-of-two values are recommended\n");
    printf("                    Default is 512 (bytes), maximum is 1024 (bytes)\n");
    printf("\n");
    printf("Examples:\n");
    printf("   # dma-memtomem-test dma=sys,event\n");
    printf("   # dma-memtomem-test dma=sys,desc=2,chan=28,event\n");
    printf("\n");
}

void wait_dma_complete(void *data)
{
    struct _pulse pulse;
    iov_t iov;
    int rcvid;

    SETIOV(&iov, &pulse, sizeof(pulse));

    for (;;) {
        if ((rcvid = MsgReceivev(chid, &iov, 1, NULL)) == -1)
            continue;

        switch (pulse.code)
        {
            case DMA_PULSE:
                //printf("receive pulse\n");
                return;
            default:
                if (rcvid)
                    MsgReplyv(rcvid, ENOTSUP, &iov, 1);
                break;
        }
    }
}

static int init_dma(const char * dma_type, unsigned chan_idx)
{
    int                    coid;
    struct sigevent        event;
    dma_channel_query_t    chinfo;
    dma_channel_info_t     tmp;

    ThreadCtl( _NTO_TCTL_IO, 0 );

    if (get_dmafuncs(&dmafuncs, sizeof(dmafuncs)) == -1)
    {
    	printf("get_dmafuncs failed.\n");
    	return -1;
    }

    dmafuncs.init(NULL);
    dmafuncs.channel_info(chan_idx, &tmp);

    if(burst_unit || buffer_size) {
        if(!(tmp.caps & DMA_CAP_BUFFER_CONTROL)) {
            printf("err buffer control is not supported by driver\n");
            return -1;
        }
    }

    if (use_intr) {
        if ((chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1){
            printf("err ChannelCreate\n");
            return -1;
        }

        if ((coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0)) == -1){
            printf("err ConnectAttach\n");
            return -1;
        }

        event.sigev_notify   = SIGEV_PULSE;
        event.sigev_coid     = coid;
        event.sigev_code     = DMA_PULSE;
        event.sigev_priority = 15;

        xferdma = dmafuncs.channel_attach(dma_type, &event, &chan_idx, chan_idx, (chan_idx == -1 ? DMA_ATTACH_ANY_CHANNEL : DMA_ATTACH_PRIORITY_STRICT) | DMA_ATTACH_EVENT_ON_COMPLETE);
    } else {
        xferdma = dmafuncs.channel_attach(dma_type, NULL, &chan_idx, chan_idx, chan_idx == -1 ? DMA_ATTACH_ANY_CHANNEL : DMA_ATTACH_PRIORITY_STRICT );
    }
    if (!xferdma) {
        if (chan_idx == -1)
            printf("Unable to attach to a DMA Channel\n");
        else
            printf("Unable to attach to DMA Channel %d\n", chan_idx);
        return -1;
    }
    dmafuncs.query_channel(xferdma, &chinfo);

    printf("init_dma channel %d done!\n", chinfo.chan_idx);

  return 0;
}

static int do_dma(off64_t dst_paddr, off64_t src_paddr, int size, int desc_num)
{
    int bw = 64;
    dma_addr_t saddr[DESC_NUM], daddr[DESC_NUM];
   
    printf("%s: 0x%" PRIx64 " > 0x%" PRIx64 ", %d... ", __func__, src_paddr,  dst_paddr, size);

    tinfo.xfer_unit_size = bw;
    tinfo.xfer_bytes     = size;
    if (desc_num >  1) {
        for(int i = 0; i < desc_num; i++) {
            saddr[i].paddr   = src_paddr + size * i / desc_num;
            saddr[i].len     = size / desc_num;
            daddr[i].paddr   = dst_paddr + size * i / desc_num;
        }
        tinfo.src_addrs      = &saddr[0];
        tinfo.dst_addrs      = &daddr[0];	
        //tinfo.mode_flags     = DMA_MODE_FLAG_REPEAT;
        tinfo.src_flags      = DMA_ADDR_FLAG_SEGMENTED;
        tinfo.src_fragments  = desc_num;
    } else {
        saddr[0].paddr   = src_paddr; 
        //saddr[0].len     = size /2;
        saddr[0].len     = size;
        daddr[0].paddr   = dst_paddr;
        tinfo.src_addrs  = &saddr[0];
        tinfo.dst_addrs  = &daddr[0];	
        //  tinfo.mode_flags     = DMA_MODE_FLAG_REPEAT;
    }

    if(burst_unit || buffer_size) {
	  tinfo.mode_flags |= DMA_MODE_FLAG_BUFFER_CONTROL;
	  if(burst_unit) {
		  tinfo.buffer_mbu = burst_unit;
	  }
	  if(buffer_size) {
		  tinfo.buffer_ulb = buffer_size;
	  }
    }

    //dmafuncs.query_channel (xferdma, &chinfo); 
    dmafuncs.setup_xfer(xferdma, &tinfo);
    dmafuncs.xfer_start(xferdma);
    if (use_intr) {
        wait_dma_complete(xferdma);
    } else {
        while(!dmafuncs.check_xfer_complete(xferdma)) {
            usleep(1);
        }
    }
    dmafuncs.xfer_complete(xferdma);

    printf("done!\n");

    return 0;
}

int main(int argc, char *argv[])
{
    unsigned char *src_vaddr = NULL;
    unsigned char *dst_vaddr = NULL;
    off_t src_paddr, dst_paddr;
    int i, j;
    int dma_use_chan = -1, desc_num = 0, ext_desc = 1, debug = 0;
    char *options = argv[1];
    char *value;
    int  opt;
    char dma_type[256];
    static char *opts[] = {
        "dma",      // dmac type, "sys" or "rt"
        "desc",     // number of internal descriptors required
        "chan",     // dma channel index
        "event",   // use intr event
        //"extdesc",  // use external descriptor
        "debug",    // Debug
        "mbu",     // maximum burst unit size
        "ulb",     // upper limit on buffer size
        NULL
    };

    memset(dma_type, 0, 256);
    while (options && *options != '\0') {
        if ((opt = getsubopt(&options, opts, &value)) == -1) {
            fprintf(stderr, "%s:%d - getsubopt()failed - %s\n", argv[0], __LINE__, strerror(errno));
            return EINVAL;
        }
        switch (opt) {
            case 0:
                sprintf(dma_type, "dma=%s", value);
                break;
            case 1:
                desc_num = strtoul(value, 0, 0);
                break;
            case 2:
                dma_use_chan = strtoul(value, 0, 0);
                break;
            case 3:
                use_intr = 1;
                break;
            //case 4:
            //    ext_desc = 1;
            //    break;
            case 4:
                debug = 1;
                break;
            case 5:
                burst_unit = strtoul(value, 0, 0);
                break;
            case 6:
                buffer_size = strtoul(value, 0, 0);
                break;
            default:
                usage();
                return EINVAL;
        }
    }

    if(dma_type[0] == 0) {
        usage();
        exit(-1);
    }

    // S4, V4H doesn't support built-in descriptor ?
    if (desc_num > 1 && ext_desc == 0)  {
        char s[8];
        strcat(dma_type, ",desc=");
        itoa(desc_num,s,10);
        strcat(dma_type, s);
    }

    if (debug) {
        strcat(dma_type, ",debug=1");
    }

    if (init_dma(dma_type, dma_use_chan)) {
        exit(-1);
    }

     // setup DMA src
     src_vaddr = (unsigned char *) mmap(0, DMA_SIZE, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_ANON | MAP_PHYS | MAP_PRIVATE, NOFD, 0);
     if (mem_offset((void *)src_vaddr, NOFD, 1, &src_paddr, 0) == -1) {
     	printf("%d: mem_offset failed, Use PIO\n", __LINE__);
     	munmap(src_vaddr,DMA_SIZE);
     	exit(-1);
     }
     // setup DMA dst
     dst_vaddr = (unsigned char *) mmap(0, DMA_SIZE, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_ANON | MAP_PHYS | MAP_PRIVATE, NOFD, 0);
     if (mem_offset((void *)dst_vaddr, NOFD, 1, &dst_paddr, 0) == -1) {
     	printf("%d: mem_offset failed, Use PIO\n", __LINE__);
     	munmap(src_vaddr,DMA_SIZE);
     	munmap(dst_vaddr,DMA_SIZE);
     	exit(-1);
     }

     /* init src buffer content */
     for (i=0; i<DMA_SIZE; i++) {
     	src_vaddr[i] = (i % 255) == 0 ? 0x80 : (i % 255); //no zero, as dst will be all 0s
     }
     /* clean dst buffer content */
     memset(dst_vaddr, 0, DMA_SIZE);

     clock_gettime(CLOCK_MONOTONIC, &tp_sta);
     do_dma(dst_paddr, src_paddr, DMA_SIZE, desc_num);
     clock_gettime(CLOCK_MONOTONIC, &tp_end);

     dmafuncs.channel_release(xferdma);

     /* check content src vs dst */
     printf("Verifying data ...");
     if (!memcmp(dst_vaddr, src_vaddr, DMA_SIZE)) {
        printf("\rVerifying data done\n");
     } else {
         printf("\rVerifying data failed at");
         for(i=0;i<DMA_SIZE;i++) {
            if(dst_vaddr[i] != src_vaddr[i]) {
                printf(" %d\n", i);
                break;
            }
        }

        for (i=CHECK_SAMPLING, j=0; i<DMA_SIZE; i+=CHECK_SAMPLING) {
            if(dst_vaddr[i] != src_vaddr[i]) {
                printf("%02x(%02x)\t", dst_vaddr[i], src_vaddr[i]);
            } else {
                printf("%02x\t", dst_vaddr[i]);
            }
            if (++j % FORMAT_NUMBER == 0) {
                printf("\n");
            }
        }
    }

    printf("transfer %dbytes in %ldns => speed = %ldMBps\n", DMA_SIZE, timespec2nsec(&tp_end) - timespec2nsec(&tp_sta), 
            (uint64_t)DMA_SIZE * 1000 / (timespec2nsec(&tp_end) - timespec2nsec(&tp_sta)));

    munmap(src_vaddr,DMA_SIZE);
    munmap(dst_vaddr,DMA_SIZE);

    return 0;
}
