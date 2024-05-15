/*
* $QNXLicenseC:
* Copyright (c) 2018, QNX Software Systems. All Rights Reserved.
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
#include "rcar_startup.h"
#include "init_board_type.h"

/*
 * This code is board dependant and may have to be
 * changed by end users.
 */

#define SLAVE_ADDR_EEPROM	(0x50U)
#define	REG_ADDR_BOARD_ID	(0x70U)

#define BOARD_CODE_MASK		(0xF8U)
#define BOARD_REV_MASK		(0x07U)
#define BOARD_CODE_SHIFT	(3U)

#define BOARD_ID_UNKNOWN	(0xFFU)

uint8_t board_type  = BOARD_UNKNOWN;
uint8_t board_rev = BOARD_REV_UNKNOWN;

const char *board_name_tbl[] = {
	[BOARD_SALVATOR_X]	    = "Salvator-X",
	[BOARD_STARTER_KIT]	    = "Starter Kit",
	[BOARD_SALVATOR_XS]	    = "Salvator-XS",
	[BOARD_EBISU]		    = "Ebisu",
	[BOARD_STARTER_KIT_PRE]	= "Starter Kit",
	[BOARD_EBISU_4D]	    = "Ebisu-4D",
    [BOARD_DRAAK]	        = "Draak",
	[BOARD_UNKNOWN]		    = "unknown"
};

const uint8_t board_rev_tbl[][8U] = {
    [BOARD_SALVATOR_X]	    = {0x10U, 0x11U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU},
    [BOARD_STARTER_KIT]	    = {0x10U, 0x30U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU},
    [BOARD_SALVATOR_XS]	    = {0x10U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU},
    [BOARD_EBISU]		    = {0x10U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU},
    [BOARD_STARTER_KIT_PRE]	= {0x10U, 0x10U, 0x20U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU},
    [BOARD_EBISU_4D]	    = {0x10U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU},
    [BOARD_DRAAK]	        = {0x10U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU},
};

static uint8_t get_board_default()
{
    if (product_id == RCAR_E3) {
        return (BOARD_EBISU << BOARD_CODE_SHIFT);
    } else if (product_id == RCAR_D3) {
        return (BOARD_DRAAK << BOARD_CODE_SHIFT);
    } else {
        return (BOARD_SALVATOR_X << BOARD_CODE_SHIFT);
    }
}

void init_board_type(void)
{
	uint8_t read_rev;
    int32_t ret = -1;
	static uint8_t board_id = BOARD_ID_UNKNOWN;

	if (BOARD_ID_UNKNOWN == board_id) {
        /* Board ID detection from EEPROM */
#ifdef PMIC_ROHM_BD9571
        ret = iic_read_reg(SLAVE_ADDR_EEPROM, REG_ADDR_BOARD_ID, &board_id, sizeof(board_id));
#else
        ret = i2c_read_reg(0, SLAVE_ADDR_EEPROM << 1, REG_ADDR_BOARD_ID, &board_id, sizeof(board_id));
#endif
        if (0 != ret) {
            board_id = BOARD_ID_UNKNOWN;
        } else if (BOARD_ID_UNKNOWN == board_id) {
            /* Can't recognize the board */
            board_id = get_board_default();
        } else {
            /* none */
        }
    }
	board_type = ((uint32_t)board_id & BOARD_CODE_MASK) >> BOARD_CODE_SHIFT;
	if (board_type < (sizeof(board_rev_tbl) / sizeof(board_rev_tbl[0]))) {
		read_rev = (uint8_t)(board_id & BOARD_REV_MASK);
		board_rev = board_rev_tbl[board_type][read_rev];
	}
}

const char * get_board_name(void)
{
	switch (board_type) {
	case BOARD_SALVATOR_X:
	case BOARD_STARTER_KIT:
	case BOARD_SALVATOR_XS:
	case BOARD_EBISU:
	case BOARD_STARTER_KIT_PRE:
	case BOARD_EBISU_4D:
    case BOARD_DRAAK:
		/* Do nothing. */
		break;
	default:
		board_type = BOARD_UNKNOWN;
		break;
	}

	if (board_type != BOARD_UNKNOWN) {
        return board_name_tbl[board_type];
	}

    return NULL;
}
