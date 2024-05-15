/*
 * $QNXLicenseC:
 * Copyright 2016, 2017 QNX Software Systems.
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
#include <errno.h>
#include <aarch64/r-car-gen3.h>
#include <rcar_board.h>
#include "rcar_startup.h"


void init_board(void)
{
    rcar_config *config = NULL;
    size_t num_devs = 0;
    int i;

    config = rcar_get_board_config(&num_devs);

    if(config) {
        for(i = 0; i < num_devs; i++) {
            PFC_DEBUG_PRINT("init module %d\n", i);
            module_setup(&config[i]);
        }
    } else {
        crash("No R-Car PFC config found!!\n");
    }
}
