/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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


#define DESC_NUM (2)

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
static int do_dma(off64_t dst_paddr, off64_t src_paddr, int size);
static void *dma_event_handler(void *handle);

/* Global variable */
pthread_mutex_t mutex;
volatile int flag = 0;
int     iid;
int     chid;

int burst_unit = 0;
int buffer_size = 0;

struct timespec tp_sta;
struct timespec tp_end;

static int init_dma(const char * dma_type, unsigned chan_idx)
{
    int                    coid;
    int                    tid;
    pthread_attr_t         attr;
    struct sigevent        event;
    struct sched_param     param;

    ThreadCtl( _NTO_TCTL_IO, 0 );
    pthread_mutex_init (&mutex, NULL);

    if ((chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1){
        printf("err ChannelCreate\n");
        return -1;
    }

    if ((coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0)) == -1){
        printf("err ConnectAttach\n");
        return -1;
    }
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = 21;
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 8192);

    event.sigev_notify   = SIGEV_PULSE;
    event.sigev_coid     = coid;
    event.sigev_code     = DMA_PULSE;
    event.sigev_priority = 21;

    if (get_dmafuncs(&dmafuncs, sizeof(dmafuncs)) == -1)
    {
    	printf("get_dmafuncs failed.\n");
    	return -1;
    } 

    dmafuncs.init(NULL);

#if INTER
    if ((xferdma = dmafuncs.channel_attach(dma_type, &event, &chan_idx, chan_idx, DMA_ATTACH_PRIORITY_STRICT | DMA_ATTACH_EVENT_ON_COMPLETE)) == NULL) {
#else
    if ((xferdma = dmafuncs.channel_attach(dma_type, &event, &chan_idx, chan_idx, DMA_ATTACH_PRIORITY_STRICT )) == NULL) {
#endif
	  printf("Unable to attach to DMA Channel %d\n", chan_idx);
	  return -1;
    }

    if (pthread_create(&tid, &attr, (void *) dma_event_handler, xferdma)) {
        fprintf(stderr, "%s:  Unable to create event handler\n", __FUNCTION__);
    }

    printf("init_dma channel %d done!\n", chan_idx);

  return 0;
}

static int do_dma(off64_t dst_paddr, off64_t src_paddr, int size)
{
  int bw = 64;
  int desc_num = DESC_NUM;
  dma_addr_t saddr[DESC_NUM+2], daddr[DESC_NUM+2];

  dma_channel_query_t  chinfo;

  printf("==%s: 0x%" PRIx64 " > 0x%" PRIx64 ", %d\n", __func__, src_paddr,  dst_paddr, size);

  tinfo.xfer_unit_size = bw;
  tinfo.xfer_bytes     = size;
#if DES
  for(int i = 0; i < desc_num; i++) {
    saddr[i].paddr   = src_paddr + size * i / desc_num;
    saddr[i].len     = size / desc_num;
    daddr[i].paddr   = dst_paddr + size * i / desc_num;
  }
  tinfo.src_addrs      = &saddr[0];
  tinfo.dst_addrs      = &daddr[0];	
  tinfo.mode_flags     = DMA_MODE_FLAG_REPEAT;
  tinfo.src_flags      = DMA_ADDR_FLAG_SEGMENTED;
  tinfo.src_fragments  = desc_num;
#else
    saddr[0].paddr   = src_paddr; 
    saddr[0].len     = size /2;
    daddr[0].paddr   = dst_paddr;
  tinfo.src_addrs      = &saddr[0];
  tinfo.dst_addrs      = &daddr[0];	
//  tinfo.mode_flags     = DMA_MODE_FLAG_REPEAT;
#endif

  if(burst_unit || buffer_size) {
	  tinfo.mode_flags |= DMA_MODE_FLAG_BUFFER_CONTROL;
	  if(burst_unit) {
		  tinfo.buffer_mbu = burst_unit;
	  }
	  if(buffer_size) {
		  tinfo.buffer_ulb = buffer_size;
	  }
  }

  dmafuncs.query_channel (xferdma, &chinfo); 
  dmafuncs.setup_xfer(xferdma, &tinfo);
  dmafuncs.xfer_start(xferdma);
#if INTER
  while(0 == flag); // wait untill has intterupt
#else
  while(!dmafuncs.check_xfer_complete(xferdma)) {
    usleep(1);
  }
#endif
  dmafuncs.xfer_complete(xferdma);

  return 0;
}

void * dma_event_handler(void *data)
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
                pthread_mutex_lock(&mutex);
                printf("receive pulse\n");
                dmafuncs.xfer_complete(data);
                flag = 1;
                pthread_mutex_unlock(&mutex);
                break;
            default:
                if (rcvid)
                    MsgReplyv(rcvid, ENOTSUP, &iov, 1);
                break;
        }
    }
 return NULL;
}

int main(int argc, char *argv[])
{
    unsigned char *src_vaddr = NULL;
    unsigned char *dst_vaddr = NULL;
    off_t src_paddr, dst_paddr;
    int i, j, n=0;
    int dma_use_chan = 4;
    char *options = argv[1];
    char *value;
    int  opt;
    char dma_type[256];
    static char *opts[] = {
        "dma",     // dmac type, "sys" or "rt"
        "desc",    // number of internal descriptors required
        "chan",    // dma channel index
        "mbu",     // maximum burst unit size
        "ulb",     // upper limit on buffer size
        NULL
    };
    memset(dma_type, 0, 256);
    while (options && *options != '\0') {
        if ((opt = getsubopt(&options, opts, &value)) == -1) {
            fprintf(stderr, "%s:%d - getsubopt()failed - %s\n", __func__, __LINE__, strerror(errno));
            return EINVAL;
        }
        switch (opt) {
            case 0:
                if (++n == 1)
                    sprintf(dma_type, "dma=%s", value);
                else {
                    strcat(dma_type, ",dma=");
                    strcat(dma_type, value);
                }
                break;
            case 1:
                if (++n == 1)
                    sprintf(dma_type, "desc=%s", value);
                else {
                    strcat(dma_type, ",desc=");
                    strcat(dma_type, value);
                }
                break;
            case 2:
                dma_use_chan = strtoul(value, 0, 0);
                break;
            case 3:
                	burst_unit = strtoul(value, 0, 0);
                	break;
            case 4:
                	buffer_size = strtoul(value, 0, 0);
                	break;
            default:
                fprintf(stderr, "%s:%d - Invalid option %s\n", __func__, __LINE__, opts[opt]);
                return EINVAL;
        }
    }
    
     init_dma(dma_type, dma_use_chan);

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
     
     /* check content src */
     printf("===source buffer:\n");
     for (i=CHECK_SAMPLING, j=0; i<DMA_SIZE; i+=CHECK_SAMPLING) {
     	printf("%02x\t", src_vaddr[i]);
     	if (++j % FORMAT_NUMBER == 0) {
     		printf("\n");
     	}
     }
     printf("\n");

     clock_gettime(CLOCK_MONOTONIC, &tp_sta);
     do_dma(dst_paddr, src_paddr, DMA_SIZE);
     clock_gettime(CLOCK_MONOTONIC, &tp_end);

     dmafuncs.channel_release(xferdma);
     /* check content src v.s. dst */
     printf("===destionation buffer:\n");
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
     printf("\n");
     printf("transfer time (ns): %ld\n", timespec2nsec(&tp_end) - timespec2nsec(&tp_sta));

     return 0;
}


