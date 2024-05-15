/*
* $QNXLicenseC:
* Copyright (c) 2017, QNX Software Systems. All Rights Reserved.
* Copyright 2021, Renesas Electronics Corporation.
*
* You must obtain a written license from and pay applicable license fees to QNX
* Software Systems before you may reproduce, modify or distribute this software,
* or any work that includes all or part of this software. Free development
* licenses are available for evaluation and non-commercial purposes. For more
* information visit http://licensing.qnx.com or email licensing@qnx.com.
*
* This file may contain contributions from others. Please review this entire
* file for other proprietary rights or license notices, as well as the QNX
* Development Suite License Guide at http://licensing.qnx.com/license-guide/
* for other information.
* $
*/

#include "startup.h"
#include <aarch64/r-car-gen3.h>
#include <rcar_board.h>
#include <rcar_startup.h>
#include "rcar_soc_board.h"

rcar_config *rcar_get_board_config(size_t *num_devs) 
{
    if (num_devs == NULL) {
        return NULL;
    }

    rcar_config *config = NULL;

    switch(product_rev) {
        case REV_1_0:
        case REV_1_1:
            config = e3_config;
            *num_devs = NUM_ELTS(e3_config);
            break;
        default:
            /* Invalid product ID found */
            config = NULL;
            break;
    }

    return config;
}