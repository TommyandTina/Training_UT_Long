/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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


#include "startup.h"
#include <aarch64/r-car-gen3.h>
#include "rcar_startup.h"

extern void init_board(void);
extern void init_board_type(void);
extern void init_timer(void);
extern int wdt_enable(void);
extern uint32_t aarch64_midr(void);
extern void  init_product_id(void);
extern char* get_soc_name(void);
extern void board_mmu_enable();
extern void board_mmu_disable();

extern uint8_t detect_ram;
extern uint8_t max_cpus_cluster0;

struct rcar_reserved_ram *rcar_reserved_ram_list = NULL;

extern struct callout_rtn   reboot_rcargen3;
const struct callout_slot callouts[] = {
    { CALLOUT_SLOT( reboot, _rcargen3 ) },
};

const struct debug_device debug_devices[] = {

    {   "scif2",
        {   "0xE6E88000.115200.14745600.16",    /* SCIF2, the default used by boot ROM for most rcar3 SoCs
                                                   use the baud rate set by boot loader */
        },
        init_scif,
        put_scif,
        {   &display_char_scif,
            &poll_key_scif,
            &break_detect_scif,
        }
    },

    {   "scif0",
        {   "0xE6E60000.115200.14745600.16",    /* SCIF0, use the baud rate set by boot loader */
        },
        init_scif,
        put_scif,
        {   &display_char_scif,
            &poll_key_scif,
            &break_detect_scif,
        }
    },

    {   "scif1",
        {   "0xE6E68000.115200.14745600.16",    /* SCIF1, use the baud rate set by boot loader */
        },
        init_scif,
        put_scif,
        {   &display_char_scif,
            &poll_key_scif,
            &break_detect_scif,
        }
    },
};

static void
allocate_reserved_memory(const paddr_t addr, const paddr_t size, const char * name)
{
    alloc_ram(addr, size, 1);
    if((name == NULL) || (name[0] == '\0')) {
        name = "rcar_reserved";
    }
    hwi_add_device(HWI_ITEM_BUS_UNKNOWN, HWI_ITEM_DEVCLASS_DMA, name, 0);
    hwi_add_location(addr, size, 0, hwi_find_as(addr, 0));
    as_add_containing(addr, addr + size - 1, AS_ATTR_RAM, name, "ram");
}

/*
 * main()
 *  Startup program executing out of RAM
 *
 * 1. It gathers information about the system and places it in a structure
 *    called the system page. The kernel references this structure to
 *    determine everything it needs to know about the system. This structure
 *    is also available to user programs (read only if protection is on)
 *    via _syspage->.
 *
 * 2. It (optionally) turns on the MMU and starts the next program
 *    in the image file system.
 */
int main(int argc, char **argv, char **envv)
{
    int     opt;
    char    *p;
    struct rcar_reserved_ram *next;

    cpu_freq = 0;
    init_product_id();
    add_callout_array(callouts, sizeof(callouts));

    while ((opt = getopt(argc, argv, COMMON_OPTIONS_STRING "L:Wdp:")) != -1) {
        switch (opt) {
            case 'W':
                /* Enable WDT */
                wdt_enable();
                break;
            case 'L':
                next = ws_alloc(sizeof(struct rcar_reserved_ram));
                if(next == NULL) {
                    crash("No memory for rcar_reserved_ram structure.\n");
                }
                next->next = rcar_reserved_ram_list;
                rcar_reserved_ram_list = next;

                rcar_reserved_ram_list->size = 0;
                rcar_reserved_ram_list->name = NULL;

                rcar_reserved_ram_list->addr = getsize(optarg, &p);
                if (*p == ',') {
                    rcar_reserved_ram_list->size = getsize(p + 1, &p);
                }
                if (*p == ',') {
                    rcar_reserved_ram_list->name = p+1;
                }
                break;
            case 'd':
                detect_ram = 1;
                break;
            case 'p':
                max_cpus_cluster0 = strtoul(optarg, &optarg, 0);
                break;
            default:
                handle_common_option(opt);
                break;
        }
    }
    /*
     * Initialize debugging output
     * By default, SCIF2 is used for debug output.  Use the startup option
     * -D (ex -Dscif0) to change to a different SCIF for debug output.
     */
    select_debug(debug_devices, sizeof(debug_devices));

    /*
     * Collect information on all free RAM in the system
     */
    init_board_clock(); 
    init_board();
    init_board_type();
    init_raminfo();

    /*
     * Set Timer frequency
     */
    init_timer();

    /*
     * Remove RAM used by modules in the image
     */
    alloc_ram(shdr->ram_paddr, shdr->ram_size, 1);

    /* Reserve RAM for -L */
    while(rcar_reserved_ram_list != NULL) {
        if(rcar_reserved_ram_list->size > 0) {
            allocate_reserved_memory(rcar_reserved_ram_list->addr, rcar_reserved_ram_list->size, rcar_reserved_ram_list->name);
        }
        rcar_reserved_ram_list = rcar_reserved_ram_list->next;
    }

    /*
    * Initialise SMP
    */
    init_smp();

    if (shdr->flags1 & STARTUP_HDR_FLAGS1_VIRTUAL){
        board_mmu_enable();
        init_mmu();
    }

    init_intrinfo();
    init_qtime();
    init_cacheattr();
    init_cpuinfo();
    init_hwinfo();

    add_typed_string(_CS_MACHINE, get_soc_name());

    /*
     * Load bootstrap executables in the image file system and Initialise
     * various syspage pointers. This must be the _last_ initialisatnion done
     * before transferring control to the next program.
     */
    init_system_private();
    board_mmu_disable();
    /*
     * This is handy for debugging a new version of the startup program.
     * Commenting this line out will save a great deal of code.
     */
    print_syspage();

    return 0;
}

