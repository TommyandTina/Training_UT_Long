/*
* $QNXLicenseC:
* Copyright (c) 2021, QNX Software Systems. All Rights Reserved.
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

#define BOARD_SALVATOR_X		(0x00U)
#define BOARD_STARTER_KIT		(0x02U)
#define BOARD_SALVATOR_XS		(0x04U)
#define BOARD_CONDOR            (0x06U)
#define BOARD_DRAAK			    (0x07U)
#define BOARD_EBISU			    (0x08U)
#define BOARD_STARTER_KIT_PRE	(0x0BU)
#define BOARD_EBISU_4D			(0x0DU)

#define BOARD_UNKNOWN			(BOARD_EBISU_4D+1U)

#define BOARD_REV_UNKNOWN		(0xFFU)

void init_board_type(void);
const char * get_board_name(void);

#endif //__INIT_BOARD_TYPE_H_INCLUDED

