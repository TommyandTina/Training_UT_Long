/***********************************************************************************************************************
* Copyright [2022-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_manager.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Manager Private Header
***********************************************************************************************************************/

#ifndef OSAL_MANAGER_H
#define OSAL_MANAGER_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "target/common/r_osal_common.h"
#include "target/common/dev_cfg/r_osal_dev_cfg_if.h"
#include "target/common/res_cfg/r_osal_res_cfg_if.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Public global functions
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            Initialize Thread Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_ThreadInitialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Thread Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_ThreadDeinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Thread Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_ThreadCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Set the Thread Manager initialization status to OSAL_MANAGER_STATE_INITIALIZED.
* @return           void
***********************************************************************************************************************/
extern void R_OSAL_ThreadSetInit(void);

/*******************************************************************************************************************//**
* @brief            Initialize ThreadSync Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_ThsyncInitialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize ThreadSync Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_ThsyncDeinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize ThreadSync Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_ThsyncCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Set the ThreadSync Manager initialization status to OSAL_MANAGER_STATE_INITIALIZED.
* @return           void
***********************************************************************************************************************/
extern void R_OSAL_ThsyncSetInit(void);

/*******************************************************************************************************************//**
* @brief            Initialize Message Queue Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_MqInitialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Message Queue Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_MqDeinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Message Queue Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_MqCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Set the Message Queue Manager initialization status to OSAL_MANAGER_STATE_INITIALIZED.
* @return           void
***********************************************************************************************************************/
extern void R_OSAL_MqSetInit(void);

/*******************************************************************************************************************//**
* @brief            Initialize Clock Time Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_ClockTimeInitialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Clock Time Manager.
* @return           void
***********************************************************************************************************************/
extern void R_OSAL_ClockTimeDeinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Clock Time Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
extern e_osal_return_t R_OSAL_ClockTimeCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Set the Clock Time Manager initialization status to OSAL_MANAGER_STATE_INITIALIZED.
* @return           void
***********************************************************************************************************************/
extern void R_OSAL_ClockTimeSetInit(void);

/*******************************************************************************************************************//**
* @brief            Initialize interrupt Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptInitialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize interrupt Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptDeinitialize(void);

/***********************************************************************************************************************
* @brief            Pre-check for Deinitialize interrupt Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
e_osal_return_t R_OSAL_InterruptCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Set the interrupt Manager initialization status to OSAL_MANAGER_STATE_INITIALIZED.
* @return           void
***********************************************************************************************************************/
void R_OSAL_InterruptSetInit(void);

/*******************************************************************************************************************//**
* @brief            Initialize IO Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoInitialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize IO Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoDeinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize IO Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
e_osal_return_t R_OSAL_IoCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Set the IO Manager initialization status to OSAL_MANAGER_STATE_INITIALIZED.
* @return           void
***********************************************************************************************************************/
void R_OSAL_IoSetInit(void);

/*******************************************************************************************************************//**
* @brief            Initialize Power Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmInitialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Power Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmDeinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Power Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
e_osal_return_t R_OSAL_PmCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Set the Power Manager initialization status to OSAL_MANAGER_STATE_INITIALIZED.
* @return           void
***********************************************************************************************************************/
void R_OSAL_PmSetInit(void);

/*******************************************************************************************************************//**
* @brief            Initialize Memory Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrInitialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize Memory Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrDeinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize Memory Manager.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MmngrCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Set the Memory Manager initialization status to 
                    OSAL_MANAGER_STATE_INITIALIZED or OSAL_MANAGER_STATE_MEMOPENED.
* @return           void
***********************************************************************************************************************/
void R_OSAL_MmngrSetInit(void);

/** @} OSAL_WRAPPER */

#endif /* OSAL_MANAGER_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
