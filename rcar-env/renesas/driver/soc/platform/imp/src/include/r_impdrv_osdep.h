/***********************************************************************************************************************
* Copyright [2020-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name    : r_impdrv_osdep.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver private header of impdrv_osdep for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_OSDEP_H
#define R_IMPDRV_OSDEP_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_OSDEP
 * @defgroup IMPDRV_OSDEP_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_OSDEP
 * @defgroup IMPDRV_OSDEP_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_OSDEP
 * @defgroup IMPDRV_OSDEP_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @typedef st_impdrv_device_handle_t
 *
***********************************************************************************************************************/
typedef struct
{
    osal_device_handle_t            handle;    /**< OSAL device handle */
} st_impdrv_device_handle_t;

/*******************************************************************************************************************//**
 * @typedef st_impdrv_mutex_handle_t
 *
***********************************************************************************************************************/
typedef struct
{
    osal_mutex_handle_t             handle;     /**< OSAL mutex handle */
} st_impdrv_mutex_handle_t;

/*******************************************************************************************************************//**
 * @typedef st_impdrv_queue_handle_t
 *
***********************************************************************************************************************/
typedef struct
{
    osal_mq_handle_t                handle;     /**< OSAL message queue handle */
} st_impdrv_queue_handle_t;

/*******************************************************************************************************************//**
 * @typedef st_impdrv_memory_handle_t
 *
***********************************************************************************************************************/
typedef struct
{
    osal_memory_buffer_handle_t     handle;     /**< OSAL memory buffer handle */
} st_impdrv_memory_handle_t;

/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_OSDEP IMPDRV OSDEP
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/*******************************************************************************************************************//**
* @brief            Power ON for "IMP TOP" device.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_imp_top(
    st_impdrv_device_handle_t           *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Power OFF for "IMP TOP" device.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_off_imp_top(
    st_impdrv_device_handle_t           *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Register of interrupt handler.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        instance_num        Instance number
* @param[in]        irq_priority        To set the value of priority.
* @param[in]        p_osal_cb_args      OSAL callback argument
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_register_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
);

/*******************************************************************************************************************//**
* @brief            Register of dsp interrupt handler.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        core_num            DSP core number
* @param[in]        irq_priority        To set the value of priority.
* @param[in]        p_osal_cb_args      OSAL callback argument
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_register_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    uint32_t                            const core_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
);

/*******************************************************************************************************************//**
* @brief            Enable interrupt.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        instance_num        Number of instance
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_enable_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num
);

/*******************************************************************************************************************//**
* @brief            Disable interrupt.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        instance_num        Number of instance
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_disable_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num
);

/*******************************************************************************************************************//**
* @brief            Unregister of interrupt handler.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        instance_num        Number of instance
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_unregister_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num
);

/*******************************************************************************************************************//**
* @brief            Power ON for specific IMP device.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_device_handle     OSAL device handle
* @param[in]        core_info           Core information
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_imp(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info
);

/*******************************************************************************************************************//**
* @brief            Power OFF for specific IMP device.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_off_imp(
    st_impdrv_device_handle_t           *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Set Power Management policy to OSAL.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_device_handle     OSAL device handle
* @param[in]        core_info           Core information
* @param[in]        policy              Power management policy
* @return           Return value
* @retval           R_IMPDRV_EC_OK                  Success completion.
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pm_set_policy(
    st_impdrv_device_handle_t          *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    const e_impdrv_pm_policy_t          policy
);

/*******************************************************************************************************************//**
* @brief            Get Power Management policy from OSAL.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_device_handle     OSAL device handle
* @param[in]        core_info           Core information
* @param[out]       policy              Power management policy
* @return           Return value
* @retval           R_IMPDRV_EC_OK                  Success completion.
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pm_get_policy(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    e_impdrv_pm_policy_t                *const p_policy
);

/*******************************************************************************************************************//**
* @brief            Get Power Management state from OSAL.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_device_handle     OSAL device handle
* @param[in]        core_info           Core information
* @param[out]       p_pmstate           Power management state.
* @return           Return value
* @retval           R_IMPDRV_EC_OK                  Success completion.
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pm_get_state(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    e_impdrv_pm_state_t                 *const p_pmstate
);

/*******************************************************************************************************************//**
* @brief            Write data to register.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        offset              Offset for write register
* @param[in]        val                 Value for write register
* @param[in]        read_back           Whether to read back
* @param[in]        chk_val             Value to read back register
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_write_reg(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const uint32_t                      offset,
    const uint32_t                      val,
    const bool                          read_back,
    const uint32_t                      chk_val
);

/*******************************************************************************************************************//**
* @brief            Read data from register.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        offset              Offset for read register
* @param[out]       p_val               Buffer for read register
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_read_reg(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const uint32_t                      offset,
    uint32_t                            *p_val
);

/*******************************************************************************************************************//**
* @brief            Creating mutex object.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_mutex_handle      OSAL mutex handle
* @param[in]        mutex_id            Mutex ID
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_create(
    st_impdrv_mutex_handle_t            *const p_mutex_handle,
    const osal_mutex_id_t               mutex_id
);

/*******************************************************************************************************************//**
* @brief            Discard mutex object.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_mutex_handle      OSAL mutex handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_destroy(
    st_impdrv_mutex_handle_t            *const p_mutex_handle
);

/*******************************************************************************************************************//**
* @brief            Mutex lock.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_mutex_handle      OSAL mutex handle
* @param[in]        time_period         Timeout value
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_lock(
    const st_impdrv_mutex_handle_t      *const p_mutex_handle,
    const osal_milli_sec_t              time_period
);

/*******************************************************************************************************************//**
* @brief            Mutex unlock.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_mutex_handle      OSAL mutex handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_unlock(
    const st_impdrv_mutex_handle_t      *const p_mutex_handle
);

/*******************************************************************************************************************//**
* @brief            Reset IMP core using OSAL PM.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_reset_core(
    const st_impdrv_device_handle_t     *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Power ON for Specific hardware resource.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_device_handle     OSAL device handle
* @param[in]        p_device_id         Pointer to the Target OSAL device ID.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_hwrsc(
    st_impdrv_device_handle_t           *const p_device_handle,
    const char                          *const p_device_id
);

/*******************************************************************************************************************//**
* @brief            Power OFF for Specific hardware resource.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_off_hwrsc(
    st_impdrv_device_handle_t           *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            OSAL dependent memory barrier, guarantees memory synchronization between contexts.
* @syncasync        Synchronous
* @reentrant        Non-Reentrant
* @param            None
* @return           Function result
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_barrier(
    void
);

/*******************************************************************************************************************//**
* @brief            Create the message queue.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_queue_handle      OSAL message queue handle
* @param[in]        queue_id            OSAL message queue ID
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_queue_create(
    st_impdrv_queue_handle_t    *const p_queue_handle,
    const osal_mq_id_t          queue_id
);

/*******************************************************************************************************************//**
* @brief            Delete the message queue.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_queue_handle      OSAL message queue handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_queue_destroy(
    st_impdrv_queue_handle_t    *const p_queue_handle
);

/*******************************************************************************************************************//**
* @brief            Wait for reception in message queue.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_queue_handle      OSAL message queue handle
* @param[in]        time_period         Timeout period of millisecond order
* @param[in, out]   p_receive_buffer    Message receive buffer pointer
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_queue_wait_period(
    const st_impdrv_queue_handle_t  *const p_queue_handle,
    const osal_milli_sec_t          time_period,
    uint32_t                        *const p_receive_buffer
);

/*******************************************************************************************************************//**
* @brief            Send data to message queue.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_queue_handle      OSAL message queue handle
* @param[in]        time_period         Timeout period of millisecond order
* @param[in]        p_send_buffer       Message send buffer pointer
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_queue_send_period(
    const st_impdrv_queue_handle_t  *const p_queue_handle,
    const osal_milli_sec_t          time_period,
    const uint32_t                  *const p_send_buffer
);

/*******************************************************************************************************************//**
* @brief            Open the device with OSAL IO Manager.
* @syncasync        Synchronous
* @reentrant        Non-Reentrant
* @param[in,out]    p_device_handle     OSAL device handle
* @param[in]        core_info           Core information
* @return           Function result
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_dev_open_imp(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info
);

/*******************************************************************************************************************//**
* @brief            Close the device with OSAL IO Manager.
* @syncasync        Synchronous
* @reentrant        Non-Reentrant
* @param[in,out]    p_device_handle     OSAL device handle
* @return           Function result
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_dev_close(
        st_impdrv_device_handle_t *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Get size of the memory buffer.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_memory_handle     OSAL memory buffer handle
* @param[in, out]   p_size              buffer size pointer
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_get_size(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    size_t                          *const p_size
);

/*******************************************************************************************************************//**
* @brief            Get logical address of the memory buffer.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_memory_handle     OSAL memory buffer handle
* @param[in, out]   p_logical_adrs      logical addrese pointer
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_get_logical(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    void                            **const p_logical_adrs
);

/*******************************************************************************************************************//**
* @brief            Get physical address of the memory buffer.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_memory_handle     OSAL memory buffer handle
* @param[in]        axi_id              OSAL AXI bus ID
* @param[in, out]   p_physical_adrs     physical addrese pointer
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_get_physical(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const osal_axi_bus_id_t         axi_id,
    uintptr_t                       *const p_physical_adrs
);

/*******************************************************************************************************************//**
* @brief            Cache flush of the memory buffer.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_memory_handle     OSAL memory buffer handle
* @param[in]        offset              Offset addrese
* @param[in]        size                Cache flush size
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_cache_flush(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const uintptr_t                 offset,
    const size_t                    size
);

/*******************************************************************************************************************//**
* @brief            Cache clean of the memory buffer.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_memory_handle     OSAL memory buffer handle
* @param[in]        offset              Offset addrese
* @param[in]        size                Cache clean size
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_cache_clean(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const uintptr_t                 offset,
    const size_t                    size
);

/*******************************************************************************************************************//**
* @brief            Data setting to the memory buffer.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_buffer            Memory buffer address
* @param[in]        data                Data to write
* @param[in]        size                Data size
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_data_set(
    uint32_t                    *const p_buffer,
    const uint32_t              data,
    const size_t                size
);

/*******************************************************************************************************************//**
* @brief            Data compare of the memory buffer.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_expected_area     Data area for test expectations
* @param[in]        p_result_area       Data area for test result
* @param[in]        size                Data size
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_compare(
    const uint32_t              *const p_expected_area,
    const uint32_t              *const p_result_area,
    const size_t                size
);

/*******************************************************************************************************************//**
* @brief            Enable DSP interrupt.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_enable_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Disable DSP interrupt.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_disable_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Unregister of DSP interrupt handler.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        core_num            DSP core number
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_unregister_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const uint32_t                      core_num
);

/*******************************************************************************************************************//**
* @brief            Power ON for DSP device.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_dsp(
    st_impdrv_device_handle_t           *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Power OFF for DSP device.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_off_dsp(
    st_impdrv_device_handle_t           *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Close for DSP device.
* @syncasync        Synchronous
* @reentrant        Non-Reentrant
* @param[in,out]    p_device_handle     OSAL device handle
* @param[in]        core_info           Core information
* @param[in]        sub_dev_num         Sub device number
* @return           Function result
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_dev_open_dsp(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    const uint32_t                      sub_dev_num
);

/*******************************************************************************************************************//**
* @brief            Close for DSP device.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_device_handle     OSAL device handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_dsp_close(
    st_impdrv_device_handle_t   *const p_device_handle
);

/*******************************************************************************************************************//**
* @brief            Power ON for specific IMP device.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_device_handle     OSAL device handle
* @param[in]        core_info           Core information
* @param[in]        sub_dev_num         Sub device number
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_imp_dsp(
    st_impdrv_device_handle_t   *const p_device_handle,
    const st_impdrv_core_info_t core_info,
    const uint32_t              sub_dev_num
);

/*******************************************************************************************************************//**
* @brief            Reset control using OSAL PM.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_device_handle     OSAL device handle
* @param[in]        bool                reset state
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_reset_core_dsp(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const bool    reset
);

#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_OSDEP_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

