/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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
#include "rcar_startup.h"
#include "board_config.h"

/*
 * This code is board dependant and may have to be
 * changed by end users.
 */

static const scif_debug_config_t scif_debug_config[] = {
    /* PFC */
    { "scif0",  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_GPSR,  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(6, 10) | BIT(0),          BITMASK(6, 10) | BIT(0) },
    { "scif0",  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_IP0SR, RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(0,  3) | BITMASK(24, 31), 0x00000000              },
    { "scif0",  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_IP1SR, RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(0, 11),                   0x00000000              },
    { "scif0",  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_POC,   RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(6,  7),                   0x00000000              },
    { "scif3",  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_GPSR,  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(0,  5),                   BITMASK(0, 5)           },
    { "scif3",  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_IP0SR, RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(0, 23),                   0x00111110              },
    { "scif3",  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_POC,   RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(2,  3),                   0x00000000              },
    { "hscif0", RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_GPSR,  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(0,  5),                   BITMASK(0, 5)           },
    { "hscif0", RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_IP0SR, RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(0, 23),                   0x00000000              },
    { "hscif0", RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_POC,   RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(2,  3),                   0x00000000              },
    { "hscif1", RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_GPSR,  RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(6, 10) | BIT(0),          BITMASK(6, 10) | BIT(0) },
    { "hscif1", RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_IP0SR, RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(0,  3) | BITMASK(24, 31), 0x11000000              },
    { "hscif1", RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_IP1SR, RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(0, 11),                   0x00000111              },
    { "hscif1", RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_POC,   RCAR_GEN4_GPIO_DM0_BASE + RCAR_S4_GPIO_GP0 + RCAR_GEN4_GPIO_PMMR, BITMASK(6,  7),                   0x00000000              },

    /* CPG */
    { "scif0",  RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_MSTPCR(7), RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_CPGWPR, BIT(2),  0 },
    { "scif3",  RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_MSTPCR(7), RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_CPGWPR, BIT(4),  0 },
    { "hscif0", RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_MSTPCR(5), RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_CPGWPR, BIT(14), 0 },
    { "hscif1", RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_MSTPCR(5), RCAR_GEN4_CPG_DM0_BASE + RCAR_GEN4_CPG_CPGWPR, BIT(15), 0 },
    { NULL, 0, 0, 0, 0 },
};

void init_debug_scif(char *s) 
{
    unsigned len, i;
    uint32_t tmp;

    if (s == NULL || s[0] == '\0') 
        return;

    // TODO PFC, CPG for SCIF and HSCIF

    for (i = 0; scif_debug_config[i].name; i++) {
        len = strlen(scif_debug_config[i].name);
        if(memcmp(s, scif_debug_config[i].name, len) == 0) {
            tmp  = in32(scif_debug_config[i].adr);
            tmp &= ~scif_debug_config[i].msk;
            tmp |= scif_debug_config[i].val;
            out32(scif_debug_config[i].wpa, ~tmp);
            out32(scif_debug_config[i].adr, tmp);
        }
    }
}

void init_board(void)
{
	/* Configure SYSC, PFC, CPG */
	soc_config(s4_config);
}
