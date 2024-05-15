/*
 * $QNXLicenseC:
 * Copyright 2016, QNX Software Systems.
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

#include "startup.h"
#include <aarch64/r-car.h>
#include "rcar_startup.h"

bool apply_errata = false;
product_id_t  product_id  = ID_NUM;
product_rev_t product_rev = REV_NUM;
uint32_t      rcar_product_id = 0;
static char fullname[80];

static char *soc_name[ID_NUM+1] = {
    [RCAR_H3]  = "H3",
    [RCAR_M3W] = "M3W",
    [RCAR_M3N] = "M3N",
    [RCAR_V3M] = "V3M",
    [RCAR_V3H] = "V3H",
    [RCAR_V3U] = "V3U",
    [RCAR_D3]  = "D3",
    [RCAR_E3]  = "E3",
    [RCAR_S4]  = "S4",
    [RCAR_V4H] = "V4H",
    [ID_NUM]   = "unknown"
};

static char *soc_rev[REV_NUM+1] = {
    [REV_1_0]  = "1.0",
    [REV_1_1]  = "1.1",
    [REV_1_2]  = "1.2",
    [REV_1_3]  = "1.3",
    [REV_2_0]  = "2.0",
    [REV_2_1]  = "2.1",
    [REV_2_2]  = "2.2",
    [REV_3_0]  = "3.0",
    [REV_NUM]  = "unknown"
};

void init_product_id()
{
    rcar_product_id = in32(RCAR_PRODUCT_REGISTER);

    switch(RCAR_PRODUCT_ID(rcar_product_id)) {
        case PRODUCT_ID_RCAR_H3:
            product_id = RCAR_H3;
            switch (RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_H3_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                case PRODUCT_H3_REV_1_1:
                    product_rev = REV_1_1;
                    break;
                case PRODUCT_H3_REV_2_0:
                    product_rev = REV_2_0;
                    break;
                case PRODUCT_H3_REV_3_0:
                    product_rev = REV_3_0;
                    break;
                default:
                    break;
            }
            break;

        case PRODUCT_ID_RCAR_M3W:
            product_id = RCAR_M3W;
            switch(RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_M3_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                case PRODUCT_M3_REV_1_1:
                    product_rev = REV_1_1;
                    break;
                case PRODUCT_M3_REV_3_0:
                    product_rev = REV_3_0;
                    break;
                default:
                    break;
            }
            break;

        case PRODUCT_ID_RCAR_M3N:
            product_id = RCAR_M3N;
            switch(RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_M3N_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                case PRODUCT_M3N_REV_1_1:
                    product_rev = REV_1_1;
                    break;
                default:
                    break;
            }
            break;

        case PRODUCT_ID_RCAR_V3M:
            product_id = RCAR_V3M;
            switch (RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_V3M_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                case PRODUCT_V3M_REV_2_0:
                    product_rev = REV_2_0;
                    break;
                default:
                    break;
            }
            break;

        case PRODUCT_ID_RCAR_V3H:
            product_id = RCAR_V3H;
            switch (RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_V3H_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                case PRODUCT_V3H_REV_1_1:
                    product_rev = REV_1_1;
                    break;
                case PRODUCT_V3H_REV_2_0:
                    product_rev = REV_2_0;
                    break;
                case PRODUCT_V3H_REV_2_1:
                    product_rev = REV_2_1;
                    break;   
                default:
                    break;
            }
            break;

        case PRODUCT_ID_RCAR_V3U:
            product_id = RCAR_V3U;
            /* product_rev currently unknown */
            product_rev = REV_1_0;
            break;

        case PRODUCT_ID_RCAR_D3:
            product_id = RCAR_D3;
            switch (RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_D3_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                default:
                    break;
            }
            break;
        case PRODUCT_ID_RCAR_E3:
            product_id = RCAR_E3;
            switch (RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_E3_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                case PRODUCT_E3_REV_1_1:
                    product_rev = REV_1_1;
                    break;
                default:
                    break;
            }
            break;
        case PRODUCT_ID_RCAR_S4:
            product_id = RCAR_S4;
            switch (RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_S4_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                case PRODUCT_S4_REV_1_1:
                    product_rev = REV_1_1;
                    break;
                case PRODUCT_S4_REV_1_2:
                    product_rev = REV_1_2;
                    break;
                default:
                    break;
            }
            break;
        case PRODUCT_ID_RCAR_V4H:
            product_id = RCAR_V4H;
            switch (RCAR_PRODUCT_REV(rcar_product_id)) {
                case PRODUCT_V4H_REV_1_0:
                    product_rev = REV_1_0;
                    break;
                case PRODUCT_V4H_REV_1_1:
                    product_rev = REV_1_1;
                    break;
                case PRODUCT_V4H_REV_2_0:
                    product_rev = REV_2_0;
                    break;
                case PRODUCT_V4H_REV_2_1:
                    product_rev = REV_2_1;
                    break;
                case PRODUCT_V4H_REV_2_2:
                    product_rev = REV_2_2;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

bool inter_core_intr_errata(void)
{
    if (apply_errata == false) {
        return false;
    }

    /* Check the product id and revision
     * The Errata only applies:
     * S4: 1.0 and 1.1
     * V4H: 1.0
     */
    if (product_id == RCAR_S4) {
        if ((product_rev == REV_1_0) || (product_rev == REV_1_1)) {
            return true;
        }
    } else if (product_id == RCAR_V4H) {
        if (product_rev == REV_1_0) {
            return true;
        }
    }

    return false;
}

char *get_soc_name()
{
    const char *board_name;
    if (product_id > ID_NUM  || product_rev > REV_NUM ||
        soc_name[product_id] == NULL || soc_rev[product_rev] == NULL) {
        crash("Invalid Soc product id (%d), rev (%d) or no soc name/rev is set\n",
                product_id, product_rev);
    }

    ksprintf(fullname, "R-Car %s rev %s", soc_name[product_id], soc_rev[product_rev]);

    board_name = get_board_name();

    if (board_name != NULL) {
        ksprintf(fullname, "%s %s", fullname, board_name);
    }

    return fullname;
}
