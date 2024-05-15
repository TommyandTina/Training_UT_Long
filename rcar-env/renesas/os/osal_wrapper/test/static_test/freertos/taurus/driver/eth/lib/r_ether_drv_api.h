/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE
* MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS
* CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY
* DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
* REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/**@defgroup group1 Taurus ETHER server public library API
 *
 * @brief This Cortex-R7 Taurus ETHER server is a part of the Taurus firmware.
 *
 * This server depends on CIO and ROS to communicate with the kernel driver
 * The Interface to the kernel is a typical I/O control with functions like
 * open, read, ioctl, etc.
 *
 * @{
 */


#ifndef R_ETHER_DRV_API_H
#define R_ETHER_DRV_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rcar-xos/osal/r_osal.h"

/*******************************************************************************
 * Function Name: R_ETHER_DRV_Open
 */
/**
 * @brief Opens the ETHER server
 *
 * @param[in] Guest       - Guest ID
 * @param[in] Cmd         - Pointer to the command to process
 *
 * @return  == 0 - OK
 * @return  != 0 - NG
 */
int R_ETHER_DRV_Open(int Guest, R_TAURUS_CmdMsg_t *Cmd);


/*******************************************************************************
 * Function Name: R_ETHER_DRV_Close
 */
/**
 * @brief Closes the ETHER server
 *
 * @param[in] Guest       - Guest ID
 * @param[in] Cmd         - Pointer to the command to process
 *
 * @return  == 0 - OK
 * @return  != 0 - NG
 */
int R_ETHER_DRV_Close(int Guest, R_TAURUS_CmdMsg_t *Cmd);

/*******************************************************************************
 * Function Name: R_ETHER_DRV_Read
 */
/**
 * @brief Read request to the ETHER server
 *
 * @param[in] Guest       - Guest ID
 * @param[in] Cmd         - Pointer to the command to process
 *
 * @return  == 0 - OK
 * @return  != 0 - NG
 */
ssize_t R_ETHER_DRV_Read(int Guest, R_TAURUS_CmdMsg_t *Cmd);

/*******************************************************************************
 * Function Name: R_ETHER_DRV_Write
 */
/**
 * @brief Write request to the ETHER server
 *
 * @param[in] Guest       - Guest ID
 * @param[in] Cmd         - Pointer to the command to process
 *
 * @return  == 0 - OK
 * @return  != 0 - NG
 */
ssize_t R_ETHER_DRV_Write(int Guest, R_TAURUS_CmdMsg_t *Cmd);

/*******************************************************************************
 * Function Name: R_ETHER_DRV_IoCtl
 */
/**
 * @brief Request an IOCTL command to the ETHER server
 *
 * @param[in] Guest       - Guest ID
 * @param[in] Cmd         - Pointer to the command to process
 *
 * @return  == 0 - OK
 * @return  != 0 - NG
 */
ssize_t R_ETHER_DRV_IoCtl(int Guest, R_TAURUS_CmdMsg_t *Cmd);

/*******************************************************************************
 * Function Name: R_ETHER_DRV_Status
 */
/**
 * @brief Query status of the ETHER server. Currently implemented empty.
 *
 * @param[in] Guest       - Guest ID
 * @param[in] Cmd         - Pointer to the command to process
 *
 * @return  == 0 - OK
 * @return  != 0 - NG
 */
ssize_t R_ETHER_DRV_Status(int Guest, R_TAURUS_CmdMsg_t *Cmd);

/*******************************************************************************
 * Function Name: R_ETHER_DRV_SetSendSignalImpl
 */
/**
 * @brief Store the function pointer callback that is called from the server
 *
 * @param[in] SendSignalImpl       - Function callback
 *
 */
void R_ETHER_DRV_SetSendSignalImpl(int (*SendSignalImpl)(int Guest, R_TAURUS_SignalId_t Signal));

#ifdef __cplusplus
}
#endif

/** @} */


#endif /* R_ETHER_DRV_API_H */
