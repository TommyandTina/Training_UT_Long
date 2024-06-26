/*
* $QNXLicenseC:
* Copyright (c) 2019, QNX Software Systems. All Rights Reserved.
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
#include "rcar_startup.h"
#include "init_board_type.h"

/* The PCA9654E I/O expander (U2) is mounted only on the Eagle board */
#define EAGLE_IO_EXPANDER_U2_I2C_ADDR      (0x40)

uint8_t board_type  = BOARD_UNKNOWN;

const char * board_name_tbl[] = {
    [BOARD_UNKNOWN]    = "Unknown",
    [BOARD_EAGLE]      = "Eagle",
    [BOARD_STARTERKIT] = "StarterKit",
};

void init_board_type(void) 
{
    uint8_t data = 0;
    int ret;
    int retries = 3;

    if (board_type != BOARD_UNKNOWN) {
        return;
    }

    while( retries-- && (board_type == BOARD_UNKNOWN) ) {
        /* ping the Condor io expander U2 */
        ret = i2c_read_reg(0, EAGLE_IO_EXPANDER_U2_I2C_ADDR, 0, &data, 1);
        switch (ret) {
            case 0: /* Success, ACK */
                board_type = BOARD_EAGLE;
                break;
            case 1: /* Success, NACK */
                board_type = BOARD_STARTERKIT;
                break;
            default: /* Error */
                break;
        }
    }

    if (retries == 0) {
        kprintf("%s: Warning: failed to detect board type\n", __FUNCTION__);
    }

    if (debug_flag) {
        kprintf("board_type: %d (%s)\n", board_type, board_name_tbl[board_type]);
    }
}

const char * get_board_name(void) 
{
    if(board_type != BOARD_UNKNOWN) {
        return board_name_tbl[board_type];
    }
    return NULL;
}
