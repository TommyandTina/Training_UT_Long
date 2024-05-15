/*
============================================================================
DESCRIPTION
PRR low level driver API
============================================================================
                            C O P Y R I G H T
============================================================================
                      Copyright (c) 2019 - 2020
                                  by
                       Renesas Electronics (Europe) GmbH.
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: to be used as sample S/W

DISCLAIMER
This software is supplied by Renesas Electronics Corporation and is only
intended for use with Renesas products. No other uses are authorized. This
software is owned by Renesas Electronics Corporation and is protected under
all applicable laws, including copyright laws.
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
Renesas reserves the right, without notice, to make changes to this software
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the
following link:
http://www.renesas.com/disclaimer *
Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*/

#ifndef _R_PRR_API_H_
#define _R_PRR_API_H_

#include <stdint.h>

/**
 * \defgroup  PRRgroup   PRR driver API
 * @brief Product Read Register (PRR) driver interface
 * @{
 */

/**
 * List of the supported R-Car devices
 */
typedef enum {
    R_PRR_RCARH3,       /**< RCar H3 */
    R_PRR_RCARM3W,      /**< RCar M3-W */
    R_PRR_RCARM3N,      /**< RCar M3-N */
    R_PRR_RCARD3,       /**< RCar D3 */
    R_PRR_RCARE3,       /**< Rcar E3 */
    R_PRR_RCARLAST      /**< Delimiter */
} r_prr_DevName_t;

/**
 * Return the R-Car device type currently in use.
 *
 * @retval      See @ref r_prr_DevName_t
 */
r_prr_DevName_t R_PRR_GetDevice(void);

/** @} */ /* end of group PRRgroup */

#endif /* _R_PRR_API_H_ */
