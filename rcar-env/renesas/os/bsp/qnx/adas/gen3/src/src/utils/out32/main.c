/*
 * $QNXLicenseC:
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
#include <hw/inout.h>
#include <sys/mman.h>
#include <sys/neutrino.h>

void usage()
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "\tWrite a long word to a physical register/memory\n");
    fprintf(stderr, "\t----------------------------------------------\n");
    fprintf(stderr, "\tSynctax : out32 addr val\n\n");

    fprintf(stderr, "\targuments:\n");
    fprintf(stderr, "\t\taddr  : the physical address\n\n");
    fprintf(stderr, "\t\tval   : the value is written to the physical register/memory\n\n");

    fprintf(stderr, "\tEx: out32 0xe6060000 0x000000FF\n");
}

int main(int argc, char *argv[])
{
    uint32_t paddr = 0;
    uint32_t value = 0;
    uintptr_t vaddr;

    if(argc != 3){
        usage();
        return -1;
    } 

    paddr = strtoul(argv[1], NULL, 0);
    value = strtoul(argv[2], NULL, 0);

    ThreadCtl(_NTO_TCTL_IO, 0);
    if((uintptr_t)MAP_DEVICE_FAILED == (vaddr = mmap_device_io(4, paddr))){ 
        fprintf(stderr,"Error: Memory map is failed\n");
        return -1;
    }

    out32(vaddr, value);
    fprintf(stderr,"in32(0x%x) = 0x%08x\n", paddr, in32(vaddr)); 

    munmap_device_io(vaddr, 4);

    return 0;
}
