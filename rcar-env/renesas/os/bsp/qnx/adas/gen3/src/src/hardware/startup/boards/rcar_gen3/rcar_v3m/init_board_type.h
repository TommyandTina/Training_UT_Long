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

#ifndef __INIT_BOARD_TYPE_H_INCLUDED
#define __INIT_BOARD_TYPE_H_INCLUDED

enum {
    BOARD_UNKNOWN,
    BOARD_EAGLE,
    BOARD_STARTERKIT,
};

void init_board_type(void);
const char * get_board_name(void);

#endif //__INIT_BOARD_TYPE_H_INCLUDED
