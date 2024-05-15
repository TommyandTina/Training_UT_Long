/*
* $QNXLicenseC:
* Copyright (c) 2018, QNX Software Systems. All Rights Reserved.
* Copyright 2022, Renesas Electronics Corporation.
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
#include "rcar_startup.h"
#include "board_type.h"

/*
 * This code is board dependant and may have to be
 * changed by end users.
 */

uint8_t board_type  = BOARD_UNKNOWN;

const char * board_name_tbl[] = {
    [BOARD_UNKNOWN] = "Unknown",
    [BOARD_SPIDER]  = "Spider",
};

void init_board_type(void)
{
    board_type  = BOARD_SPIDER;
}

const char * get_board_name(void)
{
    return board_name_tbl[board_type];
}
