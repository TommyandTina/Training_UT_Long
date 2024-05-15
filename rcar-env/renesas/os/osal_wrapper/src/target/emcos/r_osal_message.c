/***********************************************************************************************************************
* Copyright [2021-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_message.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper Message Manager for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "target/emcos/r_osal_common.h"


/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_Message Message Queue Manager Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_mq_is_initialized
 * Initialize flag
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_mq_is_initialized = false;

/** @} */

/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Functions_Message Message Queue Manager Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Set mutex and cond attributes.
* @param[in]        p_mutex_attr    The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_set_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Destroy mutex and cond attributes.
* @param[in]        p_mutex_attr    The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_destroy_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Initialize message queue.
* @param[in]        p_mutex_attr    The pointor of mutex attributes.
* @param[in]        p_cond_attr     The pointor of cond attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_init_queue(const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Internal initialize message queue process.
* @param[in]        p_mq_control    The pointor of st_osal_mq_control_t.
* @param[in]        p_mutex_attr    The pointor of pthread_mutexattr_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_init_queue_proc(st_osal_mq_control_t *p_mq_control,
                                                             const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Initialize mutex and cond object.
* @param[in]        p_mq_control    The pointor of message queue control information.
* @param[in]        p_mutex_attr    The pointor of mutex attributes.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_init_mutex_cond(st_osal_mq_control_t *p_mq_control,
                                                             const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Internal deinitialize nessage queue process.
* @param[in]        p_mq_control    The pointor of st_osal_mq_control_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_deinit_proc(st_osal_mq_control_t *p_mq_control);

/*******************************************************************************************************************//**
* @brief            Internal deinitialize message queue control information resource.
* @param[in]        p_mq_control    The pointor of st_osal_mq_control_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_mq_deinit_resource(st_osal_mq_control_t *p_mq_control);

/*******************************************************************************************************************//**
* @brief            Destroy cond object.
* @param[in]        p_mq_control    The pointor of message queue control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_cnd_destroy(st_osal_mq_control_t *p_mq_control);

/*******************************************************************************************************************//**
* @brief            Destroy mutex object.
* @param[in]        p_mq_control    The pointor of message queue control information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_mtx_destroy(st_osal_mq_control_t *p_mq_control);

/*******************************************************************************************************************//**
* @brief            Internal pre-check for deinitialize message queue process.
* @param[in]        p_mq_control    The pointor of st_osal_mq_control_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_deinit_pre_proc(st_osal_mq_control_t *p_mq_control);

/*******************************************************************************************************************//**
* @brief            Internal message queue create.
* @param[in]        mq_Id       The value of message queue id.
* @param[in]        p_handle    The pointor of osal_mq_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_ID
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_create(osal_mq_id_t mq_Id, osal_mq_handle_t *const p_handle);

/*******************************************************************************************************************//**
* @brief            Internal message queue delete.
* @param[in]        handle    The pointor of osal_mq_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_delete(osal_mq_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Internal message queue reset.
* @param[in]        handle    The pointor of osal_mq_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_reset(osal_mq_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Internal message queue reset process.
* @param[in]        handle    The pointor of osal_mq_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_reset_proc(osal_mq_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Check parameter for function on the UntilTimeStamp labels.
* @param[in]        handle        The struct of osal_mq_handle_t.
* @param[in]        p_time_stamp  Time to wait until the queue is available.
* @param[in]        p_buffer      Data for sending a message, or OSAL sets the receive data.
* @param[in]        buffer_size   The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_check_untiltime(osal_mq_handle_t const handle,
                                                             const st_osal_time_t* const p_time_stamp,
                                                             const void *p_buffer,
                                                             size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Check parameter for function on the TimePeriod labels.
* @param[in]        handle        The struct of osal_mq_handle_t.
* @param[in]        p_time_stamp  Time to wait until the queue is available.
* @param[in]        p_buffer      Data for sending a message, or OSAL sets the receive data.
* @param[in]        buffer_size   The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_check_timeperiod(osal_mq_handle_t const handle,
                                                              osal_milli_sec_t time_period,
                                                              const void *p_buffer,
                                                              size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Get the time period.
* @param[in]        time_period  The time specified by the user.
* @param[out]       p_timespec   A pointer sets the obtained time information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_timeperiod(osal_milli_sec_t time_period,
                                                         struct timespec *p_timespec);

/*******************************************************************************************************************//**
* @brief            Send Message.
* @param[in]        handle       The struct of osal_mq_handle_t.
* @param[in]        p_buffer     Data for sending a message.
* @param[in]        buffer_size  The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_snd(osal_mq_handle_t handle,
                                                 const void *p_buffer,
                                                 size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Receive Message.
* @param[in]        handle       The struct of osal_mq_handle_t.
* @param[out]       p_buffer     The OSAL sets the receive data.
* @param[in]        buffer_size  The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_rcv(osal_mq_handle_t handle,
                                                 void *p_buffer,
                                                 size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Send mq for ISR context
* @param[in]        handle       The struct of osal_mq_handle_t.
* @param[in]        p_buffer     Data for sending a message.
* @param[in]        buffer_size  The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_snd_is_isr(osal_mq_handle_t handle,
                                                        const void *p_buffer,
                                                        size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Send mq for not ISR context
* @param[in]        handle        The struct of osal_mq_handle_t.
* @param[in]        p_time_stamp  The wait time specified by the user.
* @param[in]        p_buffer      Data for sending a message.
* @param[in]        buffer_size   The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_snd_not_isr(osal_mq_handle_t handle,
                                                         const struct timespec *p_time_stamp,
                                                         const void *p_buffer,
                                                         size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Recieve mq for ISR context
* @param[in]        handle       The struct of osal_mq_handle_t.
* @param[in]        p_buffer     Data for sending a message.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_rcv_is_isr(osal_mq_handle_t handle,
                                                        void *p_buffer);

/*******************************************************************************************************************//**
* @brief            Recieve mq for not ISR context
* @param[in]        handle        The struct of osal_mq_handle_t.
* @param[in]        p_time_stamp  The wait time specified by the user.
* @param[in]        p_buffer      Data for sending a message.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_rcv_not_isr(osal_mq_handle_t handle,
                                                         const struct timespec *p_time_stamp,
                                                         void *p_buffer);

/*******************************************************************************************************************//**
* @brief            Wait until the message can be sent and send the message.
* @param[in]        handle       The struct of osal_mq_handle_t.
* @param[in]        p_wait_time  The wait time specified by the user.
* @param[out]       p_buffer     Data for sending a message.
* @param[in]        buffer_size  The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_wait_and_snd(osal_mq_handle_t handle,
                                                          const struct timespec *p_wait_time,
                                                          const void * p_buffer,
                                                          size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Wait until the message can be receive and receive the message.
* @param[in]        handle       The struct of osal_mq_handle_t.
* @param[in]        p_wait_time  The wait time specified by the user.
* @param[out]       p_buffer     Data for sending a message.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_wait_and_rcv(osal_mq_handle_t handle,
                                                          const struct timespec *p_wait_time,
                                                          void *p_buffer);

/*******************************************************************************************************************//**
* @brief            Internal check message queue is empty.
* @param[in]        handle    The pointor of osal_mq_handle_t.
* @param[out]       p_result  The address of bool for result.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_is_empty(osal_mq_handle_t handle, bool* const p_result);

/*******************************************************************************************************************//**
* @brief            Internal check message queue is full.
* @param[in]        handle    The pointor of osal_mq_handle_t.
* @param[out]       p_result  The address of bool for result.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_is_full(osal_mq_handle_t handle, bool* const p_result);

/*******************************************************************************************************************//**
* @brief            Internal get message queue configuration information.
* @param[in]        handle      The pointor of osal_mq_handle_t.
* @param[in]        p_config    The address of st_osal_mq_config_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_HANDLE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_get_config(osal_mq_handle_t handle, st_osal_mq_config_t *const p_config);

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_MqCreate()
* XOS3_OSAL_EMCOS_CD_CD_MSG_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_009]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_010]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_011]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqCreate(const st_osal_mq_config_t *const p_config, osal_mq_id_t mq_Id, osal_mq_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t  osal_ret = OSAL_RETURN_OK;
    osal_mq_handle_t local_handle = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL == p_config) || (NULL == p_handle))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((0U == p_config->max_num_msg) || (0U == p_config->msg_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mq_create(mq_Id, &local_handle);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        /* PRQA S 2962 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        if ((OSAL_RETURN_BUSY != osal_ret) && (NULL != local_handle))
        {
            /* PRQA S 2812 1 # The pointer refers to the Realities, so there is no problem. */
            *p_handle = OSAL_MQ_HANDLE_INVALID;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* PRQA S 2816,2916 1 # The pointer refers to the Realities, so there is no problem. */
        *p_handle = local_handle;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqCreate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqDelete()
* XOS3_OSAL_EMCOS_CD_CD_MSG_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_012]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_013]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqDelete(osal_mq_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_MESSAGE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
             (false == handle->active) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_mq_delete(handle);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqDelete()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqReset()
* XOS3_OSAL_EMCOS_CD_CD_MSG_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_027]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqReset(osal_mq_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_MESSAGE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
              True if the correct handle was passed. False detection of the tool. */
             (false == handle->active) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_mq_reset(handle);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqSendUntilTimeStamp()
* XOS3_OSAL_EMCOS_CD_CD_MSG_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_032]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_033]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_034]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqSendUntilTimeStamp(osal_mq_handle_t handle,
                                            const st_osal_time_t *const p_time_stamp,
                                            const void * p_buffer,
                                            size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    struct timespec time_stamp     = {0, 0};
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mq_check_untiltime(handle, p_time_stamp, p_buffer, buffer_size);
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_get_value = R_OSAL_Internal_GetThreadSpecific();

        /* Is ISR Context */
        if (NULL != p_get_value)
        {
            osal_ret = osal_internal_mq_snd_is_isr(handle, p_buffer, buffer_size);
        }
        else /* Is Normal Context */
        {
            time_stamp.tv_sec  = p_time_stamp->tv_sec;
            time_stamp.tv_nsec = p_time_stamp->tv_nsec;
            osal_ret = R_OSAL_Internal_TimeRoundUp(&time_stamp);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mq_snd_not_isr(handle, &time_stamp, p_buffer, buffer_size);
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqSendUntilTimeStamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqSendForTimePeriod()
* XOS3_OSAL_EMCOS_CD_CD_MSG_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_028]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_029]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_030]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_031]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqSendForTimePeriod(osal_mq_handle_t handle,
                                           osal_milli_sec_t time_period,
                                           const void *p_buffer,
                                           size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    struct timespec time_stamp     = {0, 0};
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mq_check_timeperiod(handle, time_period, p_buffer, buffer_size);
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_get_value = R_OSAL_Internal_GetThreadSpecific();

        /* Is ISR Context */
        if (NULL != p_get_value)
        {
            osal_ret = osal_internal_mq_snd_is_isr(handle, p_buffer, buffer_size);
        }
        else /* Is Normal Context */
        {
            osal_ret = osal_internal_get_timeperiod(time_period, &time_stamp);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mq_snd_not_isr(handle, &time_stamp, p_buffer, buffer_size);
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqSendForTimePeriod()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqReceiveUntilTimeStamp()
* XOS3_OSAL_EMCOS_CD_CD_MSG_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_023]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_024]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_025]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_026]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqReceiveUntilTimeStamp(osal_mq_handle_t handle,
                                               const st_osal_time_t* const p_time_stamp,
                                               void *p_buffer,
                                               size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    struct timespec time_stamp    = {0, 0};
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mq_check_untiltime(handle, p_time_stamp, p_buffer, buffer_size);
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_get_value = R_OSAL_Internal_GetThreadSpecific();

        /* Is ISR Context */
        if (NULL != p_get_value)
        {
            osal_ret = osal_internal_mq_rcv_is_isr(handle, p_buffer);
        }
        else /* Is Normal Context */
        {
            time_stamp.tv_sec  = p_time_stamp->tv_sec;
            time_stamp.tv_nsec = p_time_stamp->tv_nsec;
            osal_ret = R_OSAL_Internal_TimeRoundUp(&time_stamp);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mq_rcv_not_isr(handle, &time_stamp, p_buffer);
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqReceiveUntilTimeStamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqReceiveForTimePeriod()
* XOS3_OSAL_EMCOS_CD_CD_MSG_007
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_020]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_021]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_022]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqReceiveForTimePeriod(osal_mq_handle_t handle,
                                              osal_milli_sec_t time_period,
                                              void *p_buffer,
                                              size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    struct timespec time_stamp    = {0, 0};
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_mq_check_timeperiod(handle, time_period, p_buffer, buffer_size);
    if (OSAL_RETURN_OK == osal_ret)
    {
        p_get_value = R_OSAL_Internal_GetThreadSpecific();

        /* Is ISR Context */
        if (NULL != p_get_value)
        {
            osal_ret = osal_internal_mq_rcv_is_isr(handle, p_buffer);
        }
        else /* Is Normal Context */
        {
            osal_ret = osal_internal_get_timeperiod(time_period, &time_stamp);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mq_rcv_not_isr(handle, &time_stamp, p_buffer);
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqReceiveForTimePeriod()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqIsEmpty()
* XOS3_OSAL_EMCOS_CD_CD_MSG_008
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_016]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_017]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqIsEmpty(osal_mq_handle_t handle,
                                 bool* const p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    bool            result;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (NULL == p_result)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_mq_is_empty(handle, &result);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880,2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        *p_result = result;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqIsEmpty()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqIsFull()
* XOS3_OSAL_EMCOS_CD_CD_MSG_009
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_018]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_019]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqIsFull(osal_mq_handle_t handle,
                                bool* const p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    bool            result;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (NULL == p_result)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_mq_is_full(handle, &result);
    }
    else
    {
        /* Do nothing */
    }
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        *p_result = result;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqIsFull()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqGetConfig()
* XOS3_OSAL_EMCOS_CD_CD_MSG_010
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_014]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqGetConfig(osal_mq_handle_t handle,
                                   st_osal_mq_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2991,2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_MESSAGE_HANDLE_ID != handle->handle_id) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if (NULL == p_config)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }
    
    /* PRQA S 2992,2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
         True if the correct handle was passed. False detection of the tool. */
        osal_ret = osal_internal_mq_get_config(handle, p_config);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqGetConfig()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_MqInitializeMqConfigSt()
* XOS3_OSAL_EMCOS_CD_CD_MSG_011
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_015]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_MqInitializeMqConfigSt(st_osal_mq_config_t* const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t config;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if(NULL == p_config)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        config.max_num_msg = 0;
        config.msg_size    = 0;
    }
    
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_config = config;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_MqInitializeMqConfigSt()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_mq_initialize()
* XOS3_OSAL_EMCOS_CD_CD_MSG_012
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_007]
***********************************************************************************************************************/
e_osal_return_t osal_mq_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    pthread_mutexattr_t mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (true == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_mq_is_initialized = true;

        osal_ret = osal_internal_mq_set_attr(&mutex_attr);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 2982 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
            osal_ret = osal_internal_mq_init_queue(&mutex_attr);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mq_destroy_attr(&mutex_attr);
            }
            else  
            {
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)osal_internal_mq_destroy_attr(&mutex_attr);
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)osal_mq_deinitialize();
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(true == gs_osal_mq_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_mq_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_mq_deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_MSG_013
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_005]
***********************************************************************************************************************/
e_osal_return_t osal_mq_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret      = OSAL_RETURN_OK;
    e_osal_return_t         api_ret;
    size_t                  num_of_info;
    size_t                  loop_count    = 0;
    st_osal_mq_control_t    *mq_control   = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_mq_is_initialized = false;

    num_of_info = R_OSAL_RCFG_GetNumOfMq();
    while ((num_of_info > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_RCFG_GetMqInfo((uint32_t)loop_count, &mq_control);
        if ((OSAL_RETURN_OK != api_ret) ||
            (NULL == mq_control) ||
            (NULL == mq_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = osal_internal_mq_deinit_proc(mq_control);
        }
        loop_count++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_mq_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_mq_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_MSG_014
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_006]
***********************************************************************************************************************/
e_osal_return_t osal_mq_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret    = OSAL_RETURN_OK;
    e_osal_return_t      api_ret;
    size_t               num_of_info;
    size_t               loop_count  = 0;
    st_osal_mq_control_t *mq_control = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        gs_osal_mq_is_initialized = false;

        num_of_info = R_OSAL_RCFG_GetNumOfMq();
        while ((num_of_info > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            api_ret = R_OSAL_RCFG_GetMqInfo((uint32_t)loop_count, &mq_control);
            if ((OSAL_RETURN_OK != api_ret) ||
                (NULL == mq_control) ||
                (NULL == mq_control->usr_config))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                osal_ret = osal_internal_mq_deinit_pre_proc(mq_control);
            }
            loop_count++;
        }
    }   /* end of if(false == gs_osal_mq_is_initialized). */

    if (OSAL_RETURN_BUSY == osal_ret)
    {
        gs_osal_mq_is_initialized = true;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_mq_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_mq_set_is_init()
* XOS3_OSAL_EMCOS_CD_CD_MSG_015
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_008]
***********************************************************************************************************************/
void osal_mq_set_is_init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_mq_is_initialized = true;

    return;
}
/***********************************************************************************************************************
* End of function osal_mq_set_is_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_set_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_set_attr(pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutexattr_init(p_mutex_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_mutexattr_settype(p_mutex_attr, (int)PTHREAD_MUTEX_ERRORCHECK);
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_mutexattr_destroy(p_mutex_attr);
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_set_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_destroy_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_destroy_attr(pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutexattr_destroy(p_mutex_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_destroy_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_init_queue()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_init_queue(const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret      = OSAL_RETURN_OK;
    e_osal_return_t      api_ret;
    size_t               num_of_info;
    size_t               loop_count    = 0;
    st_osal_mq_control_t *mq_control   = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num_of_info = R_OSAL_RCFG_GetNumOfMq();
    if (0U == num_of_info)
    {
        osal_ret = OSAL_RETURN_CONF;
    }
    else
    {
        /* Do nothing */
    }

    while ((num_of_info > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        api_ret = R_OSAL_RCFG_GetMqInfo((uint32_t)loop_count, &mq_control);
        if ((OSAL_RETURN_OK != api_ret) ||
            (NULL == mq_control) ||
            (NULL == mq_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if ((0U == mq_control->usr_config->config.max_num_msg) ||
                (0U == mq_control->usr_config->config.msg_size))
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                osal_ret = osal_internal_mq_init_queue_proc(mq_control, p_mutex_attr);
            }
        }
        loop_count++;
    }   /* end of while((num_of_info > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_init_queue()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_init_queue_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_init_queue_proc(st_osal_mq_control_t *p_mq_control,
                                                             const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    size_t          buffer_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118 1 # Safty Manual, malloc use are allowed. No problem because it is called by Initialize. */
    p_mq_control->queue = malloc(sizeof(st_osal_queue_t) * (uint32_t)p_mq_control->usr_config->config.max_num_msg);
    if (NULL == p_mq_control->queue)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        buffer_count = 0;
        while ((p_mq_control->usr_config->config.max_num_msg > buffer_count) &&
               (OSAL_RETURN_OK == osal_ret))
        {
            /* PRQA S 5118,2706 1 # Safty Manual, malloc use are allowed. No problem because Messag eManager osal_internal_mq_deinit_resource free is called*/
            p_mq_control->queue[buffer_count].p_buffer = malloc((uint32_t)p_mq_control->usr_config->config.msg_size);
            if (NULL == p_mq_control->queue[buffer_count].p_buffer)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }
            buffer_count++;
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mq_init_mutex_cond(p_mq_control, p_mutex_attr);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            p_mq_control->active       = false;
            p_mq_control->send_wait    = false;
            p_mq_control->receive_wait = false;
            p_mq_control->head         = -1;
            p_mq_control->tail         = 0;
            p_mq_control->numofmsg     = 0;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(NULL == p_mq_control->queue). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_init_queue_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_init_mutex_cond()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_init_mutex_cond(st_osal_mq_control_t *p_mq_control,
                                                             const pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 2736 1 # No problem because osal_internal_mq_mtx_destroy is called pthread_mutex_destroy. */
    os_ret = pthread_mutex_init(&p_mq_control->inner_mtx, p_mutex_attr);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_init(&p_mq_control->send_cond, NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_init(&p_mq_control->receive_cond, NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_init_mutex_cond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_deinit_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_deinit_proc(st_osal_mq_control_t *p_mq_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = R_OSAL_Internal_TimedlockMutex(&p_mq_control->inner_mtx, p_mq_control->inner_timeout);
    if (OSAL_RETURN_OK != api_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = osal_internal_mq_cnd_destroy(p_mq_control);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_internal_mq_deinit_resource(p_mq_control);
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_mutex_unlock(&p_mq_control->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mq_mtx_destroy(p_mq_control);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_deinit_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_deinit_resource()
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_mq_deinit_resource(st_osal_mq_control_t *p_mq_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t buffer_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if(NULL != p_mq_control->queue)
    {
        buffer_count = 0;
        while (p_mq_control->usr_config->config.max_num_msg > buffer_count)
        {
            if (NULL != p_mq_control->queue[buffer_count].p_buffer)
            {
                /* PRQA S 5118 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
                free(p_mq_control->queue[buffer_count].p_buffer);
                p_mq_control->queue[buffer_count].p_buffer = NULL;
            }
            else
            {
                /* Do nothing */
            }
            buffer_count++;
        }
        /* PRQA S 5118,0314 1 # Safty Manual, free use are allowed. No problem because it is called by DeInitialize. */
        free((void*)p_mq_control->queue);
        p_mq_control->queue = NULL;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != p_mq_control->queue). */

    p_mq_control->handle_id    = OSAL_INVALID_USER_HANDLE_ID;
    p_mq_control->active       = false;
    p_mq_control->send_wait    = false;
    p_mq_control->receive_wait = false;
    p_mq_control->head         = -1;
    p_mq_control->tail         = 0;
    p_mq_control->numofmsg     = 0;

    return;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_deinit_resource()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_cnd_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_cnd_destroy(st_osal_mq_control_t *p_mq_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_cond_destroy(&p_mq_control->send_cond);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_destroy(&p_mq_control->receive_cond);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_cnd_destroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_mtx_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_mtx_destroy(st_osal_mq_control_t *p_mq_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    int32_t         waiting_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutex_destroy(&p_mq_control->inner_mtx);
    if (0 != os_ret)
    {
        waiting_time = 0;
        while ((OSAL_MUTEX_DESTROY_TIMEOUT > waiting_time) && (EBUSY == os_ret))
        {
            (void)R_OSAL_Internal_MsSleep(OSAL_MESSAGE_MS_SLEEP_TIME);
            waiting_time += OSAL_MESSAGE_MS_SLEEP_TIME;
            os_ret = pthread_mutex_destroy(&p_mq_control->inner_mtx);
        }

        if ((OSAL_MUTEX_DESTROY_TIMEOUT <= waiting_time) || (0 != os_ret))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(0 != os_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_mtx_destroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_deinit_pre_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_deinit_pre_proc(st_osal_mq_control_t *p_mq_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = R_OSAL_Internal_TimedlockMutex(&p_mq_control->inner_mtx, p_mq_control->inner_timeout);
    if (OSAL_RETURN_OK != api_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if ((true == p_mq_control->send_wait) || (true == p_mq_control->receive_wait))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_mutex_unlock(&p_mq_control->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_deinit_pre_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_create()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_create(osal_mq_id_t mq_Id, osal_mq_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t      osal_ret     = OSAL_RETURN_OK;
    e_osal_return_t      api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                  os_ret;
    size_t               num_of_info;
    size_t               loop_count   = 0;
    st_osal_mq_control_t *control     = NULL;
    st_osal_mq_control_t *mp_control  = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* find message queue control information */
    num_of_info = R_OSAL_RCFG_GetNumOfMq();
    while ((num_of_info > loop_count) &&
           (OSAL_RETURN_OK == osal_ret) &&
           (NULL == control))
    {
        api_ret = R_OSAL_RCFG_GetMqInfo((uint32_t)loop_count,
                                        &mp_control);
        if ((OSAL_RETURN_OK != api_ret) ||
            (NULL == mp_control) ||
            (NULL == mp_control->usr_config))
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (mq_Id == mp_control->usr_config->id)
            {
                control = mp_control;
            }
            else
            {
                /* do nothing */
            }
        }
        loop_count++;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* check message queue control information */
        if (NULL == control)
        {
            osal_ret = OSAL_RETURN_ID;
        }
        else
        {
            api_ret = R_OSAL_Internal_TimedlockMutex(&control->inner_mtx,
                                                    control->inner_timeout);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                if (true == control->active)
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }
                else
                {
                    control->handle_id = (uint64_t)OSAL_MESSAGE_HANDLE_ID;
                    control->active    = true;
                    /* PRQA S 2916 1 # The pointer refers to the Realitiesn, so there is no problem. */
                    *p_handle = control;
                }

                os_ret = pthread_mutex_unlock(&control->inner_mtx);
                if ((0 != os_ret) && (osal_ret == OSAL_RETURN_OK))
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* do nothing */
                }
            }   /* end of if(OSAL_RETURN_OK != api_ret). */
        }   /* end of if(NULL == control). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_create()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_delete()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_delete(osal_mq_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
    if (OSAL_RETURN_OK != api_ret)
    {
       osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if ((0U != handle->numofmsg) ||
            (true == handle->receive_wait) ||
            (true == handle->send_wait))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            handle->handle_id = OSAL_INVALID_USER_HANDLE_ID;
            handle->head      = -1;
            handle->tail      = 0;
            handle->active    = false;
        }

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_delete()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_reset()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_reset(osal_mq_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx,
                                              handle->inner_timeout);
    if (OSAL_RETURN_OK != api_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (true == handle->send_wait)
        {
            os_ret = pthread_cond_broadcast(&handle->send_cond);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (NULL == handle->queue)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mq_reset_proc(handle);
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_reset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_reset_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_reset_proc(osal_mq_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    size_t          buffer_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    buffer_count = 0U;
    while ((handle->usr_config->config.max_num_msg > buffer_count) &&
           (OSAL_RETURN_OK == osal_ret))
    {
        if (NULL == handle->queue[buffer_count].p_buffer)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            (void)memset(handle->queue[buffer_count].p_buffer,
                         0x00,
                         (size_t)handle->usr_config->config.msg_size);
        }

        buffer_count++;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        handle->head     = -1;
        handle->tail     = 0;
        handle->numofmsg = 0;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_reset_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_check_untiltime()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_mq_check_untiltime(osal_mq_handle_t const handle,
                                                             const st_osal_time_t* const p_time_stamp,
                                                             const void *p_buffer,
                                                             size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_MESSAGE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
              True if the correct handle was passed. False detection of the tool. */
             (false == handle->active) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((NULL == p_time_stamp) ||
             ((time_t)0 > p_time_stamp->tv_sec) ||
             (0 > p_time_stamp->tv_nsec) ||
             (OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec) ||
             (NULL == p_buffer) ||
             (handle->usr_config->config.msg_size != buffer_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }   /* end of if(false == gs_osal_mq_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_check_untiltime()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_check_timeperiod()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
OSAL_STATIC e_osal_return_t osal_internal_mq_check_timeperiod(osal_mq_handle_t const handle,
                                                              osal_milli_sec_t time_period,
                                                              const void *p_buffer,
                                                              size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (false == gs_osal_mq_is_initialized)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((OSAL_MESSAGE_HANDLE_ID != handle->handle_id) ||
             /* PRQA S 2991 2 # It is a check for robustness, because it is carried out based on the design is no problem.
              True if the correct handle was passed. False detection of the tool. */
             (false == handle->active) ||
             (NULL == handle->usr_config))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2880 2 # It is a check for robustness, because it is carried out based on the design is no problem.
     True if the correct handle was passed. False detection of the tool. */
    else if ((0 > time_period) ||
             (NULL == p_buffer) ||
             (handle->usr_config->config.msg_size != buffer_size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }   /* end of if(false == gs_osal_mq_is_initialized). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_check_timeperiod()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_get_timeperiod()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_timeperiod(osal_milli_sec_t time_period,
                                                         struct timespec *p_timespec)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    osal_milli_sec_t time_period_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    time_period_tmp = time_period;
    api_ret = R_OSAL_Internal_GetTimestamp(p_timespec);
    if (OSAL_RETURN_OK != api_ret)
    {
       osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        api_ret = R_OSAL_Internal_TimespecAdd(p_timespec, &time_period_tmp);
        if (OSAL_RETURN_OK != api_ret)
        {
           osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_get_timeperiod()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_snd()
* XOS3_OSAL_EMCOS_CD_CD_MSG_016
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_003]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_snd(osal_mq_handle_t handle,
                                                 const void *p_buffer, 
                                                 size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == handle->queue)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if (NULL == handle->queue[handle->tail].p_buffer)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 1496 1 # OSAL Specifies the API argument as is. The OSAL user is guaranteed to be of a certain size. */
        (void)memcpy(handle->queue[handle->tail].p_buffer,
                     p_buffer,
                     buffer_size);

        /* Registration of initial reading position */
        if (-1 == handle->head)
        {
            handle->head = handle->tail;
        }
        else
        {
            /* Do nothing */
        }

        /* Registration of next writing position */
        if ((handle->usr_config->config.max_num_msg - 1U) == (uint64_t)handle->tail)
        {
            handle->tail = 0;
        }
        else
        {
            handle->tail++;
        }

        handle->numofmsg++;
    }   /* end of if(NULL == handle->queue). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_snd()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_rcv()
* XOS3_OSAL_EMCOS_CD_CD_MSG_017
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_001]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_rcv(osal_mq_handle_t handle,
                                                 void *p_buffer,
                                                 size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == handle->queue)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if (NULL == handle->queue[handle->head].p_buffer)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 1496 1 # OSAL Specifies the API argument as is. The OSAL user is guaranteed to be of a certain size. */
        (void)memcpy(p_buffer,
                     handle->queue[handle->head].p_buffer,
                     buffer_size);

        (void)memset(handle->queue[handle->head].p_buffer,
                     0x00,
                     (size_t)handle->usr_config->config.msg_size);

        handle->numofmsg--;

        /* Registration of next reading position */
        if (0U == handle->numofmsg)
        {
            handle->head = -1;
        }
        else if ((handle->usr_config->config.max_num_msg - (uint64_t)1) == (uint64_t)handle->head)
        {
            handle->head = 0;
        }
        else
        {
            handle->head++;
        }
    }   /* end of if(NULL == handle->queue). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_rcv()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_snd_is_isr()
* XOS3_OSAL_EMCOS_CD_CD_MSG_018
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_004]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_snd_is_isr(osal_mq_handle_t handle,
                                                        const void *p_buffer,
                                                        size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_mq_is_initialized)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (handle->usr_config->config.max_num_msg == handle->numofmsg)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            api_ret = osal_internal_mq_snd(handle, p_buffer, buffer_size);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if ((OSAL_RETURN_OK == osal_ret) && (true == handle->receive_wait))
        {
            os_ret = pthread_cond_broadcast(&handle->receive_cond);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_snd_is_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_snd_not_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_snd_not_isr(osal_mq_handle_t handle,
                                                         const struct timespec *p_time_stamp,
                                                         const void *p_buffer,
                                                         size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutex_timedlock(&handle->inner_mtx, p_time_stamp);
    switch (os_ret)
    {
        case 0:
            osal_ret = OSAL_RETURN_OK;
            break;
        case ETIMEDOUT:
            osal_ret = OSAL_RETURN_TIME;
            break;
        default:
            osal_ret = OSAL_RETURN_FAIL;
            break;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mq_wait_and_snd(handle, p_time_stamp, p_buffer, buffer_size);

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_snd_not_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_rcv_is_isr()
* XOS3_OSAL_EMCOS_CD_CD_MSG_019
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_002]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_rcv_is_isr(osal_mq_handle_t handle, void *p_buffer)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_mq_is_initialized)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (0U == handle->numofmsg)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            api_ret = osal_internal_mq_rcv(handle, p_buffer, (size_t)handle->usr_config->config.msg_size);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        if ((OSAL_RETURN_OK == osal_ret) && (true == handle->send_wait))
        {
            os_ret = pthread_cond_broadcast(&handle->send_cond);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_rcv_is_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_rcv_not_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_rcv_not_isr(osal_mq_handle_t handle,
                                                         const struct timespec *p_time_stamp,
                                                         void *p_buffer)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutex_timedlock(&handle->inner_mtx, p_time_stamp);
    switch (os_ret)
    {
        case 0:
            osal_ret = OSAL_RETURN_OK;
            break;
        case ETIMEDOUT:
            osal_ret = OSAL_RETURN_TIME;
            break;
        default:
            osal_ret = OSAL_RETURN_FAIL;
            break;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mq_wait_and_rcv(handle, p_time_stamp, p_buffer);

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_rcv_not_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_wait_and_snd()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_wait_and_snd(osal_mq_handle_t handle,
                                                          const struct timespec *p_wait_time,
                                                          const void * p_buffer,
                                                          size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (handle->usr_config->config.max_num_msg == handle->numofmsg)
    {
        while ((handle->usr_config->config.max_num_msg == handle->numofmsg) &&
               (OSAL_RETURN_OK == osal_ret))
        {
            /* Check if the block is released after the de-initialization. */
            /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
            if (false == gs_osal_mq_is_initialized)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else
            {
                /* PRQA S 2982 2 # It is a check for robustness, because it is carried out based on the design is no problem.
                 True if the correct handle was passed. False detection of the tool. */
                handle->send_wait = true;
                os_ret = pthread_cond_timedwait(&handle->send_cond,
                                                &handle->inner_mtx,
                                                p_wait_time);
                handle->send_wait = false;
                switch (os_ret)
                {
                    case 0:
                        /* Do nothing */
                        break;
                    case ETIMEDOUT:
                        osal_ret = OSAL_RETURN_TIME;
                        break;
                    default:
                        osal_ret = OSAL_RETURN_FAIL;
                        break;
                }
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(handle->usr_config->config.max_num_msg == handle->numofmsg). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        api_ret = osal_internal_mq_snd(handle,
                                        p_buffer,
                                        buffer_size);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if ((OSAL_RETURN_OK == osal_ret) &&
        (true == handle->receive_wait))
    {
        os_ret = pthread_cond_broadcast(&handle->receive_cond);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_wait_and_snd()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_wait_and_rcv()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_wait_and_rcv(osal_mq_handle_t handle,
                                                          const struct timespec *p_wait_time,
                                                          void *p_buffer)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0U == handle->numofmsg)
    {
        while ((0U == handle->numofmsg) && (OSAL_RETURN_OK == osal_ret))
        {
            /* Check if the block is released after the de-initialization. */
            /* PRQA S 2995 2 # It is a check for robustness, because it is carried out based on the design is no problem.
             True if the correct handle was passed. False detection of the tool. */
            if (false == gs_osal_mq_is_initialized)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else
            {
                /* PRQA S 2982 2 # It is a check for robustness, because it is carried out based on the design is no problem.
                 True if the correct handle was passed. False detection of the tool. */
                handle->receive_wait = true;
                os_ret = pthread_cond_timedwait(&handle->receive_cond,
                                                   &handle->inner_mtx,
                                                   p_wait_time);
                handle->receive_wait = false;
                switch (os_ret)
                {
                    case 0:
                        /* Do nothing */
                        break;
                    case ETIMEDOUT:
                        osal_ret = OSAL_RETURN_TIME;
                        break;
                    default:
                        osal_ret = OSAL_RETURN_FAIL;
                        break;
                }
            }   /* end of if((OSAL_RETURN_OK == osal_ret) && (false == gs_osal_mq_is_initialized)). */
        }   /* end of while((0U == handle->numofmsg) && (OSAL_RETURN_OK == osal_ret)). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(0U == handle->numofmsg). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        api_ret = osal_internal_mq_rcv(handle,
                                       p_buffer,
                                       (size_t)handle->usr_config->config.msg_size);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    if ((OSAL_RETURN_OK == osal_ret) &&
        (true == handle->send_wait))
    {
        os_ret = pthread_cond_broadcast(&handle->send_cond);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_wait_and_rcv()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_is_empty()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_is_empty(osal_mq_handle_t handle, bool* const p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
    if (OSAL_RETURN_OK != api_ret)
    {
       osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (false == handle->active)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (0U == handle->numofmsg)
        {
            *p_result = true;
        }
        else    /* 0 != handle->numofmsg */
        {
            *p_result = false;
        }

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_is_empty()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_is_full()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_is_full(osal_mq_handle_t handle, bool* const p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx,
                                              handle->inner_timeout);
    if (OSAL_RETURN_OK != api_ret)
    {
       osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (false == handle->active)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else if (handle->usr_config->config.max_num_msg == handle->numofmsg)
        {
            *p_result = true;
        }
        else    /* handle->usr_config->config.max_num_msg != handle->numofmsg */
        {
            *p_result = false;
        }

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_is_full()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_get_config()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_get_config(osal_mq_handle_t handle, st_osal_mq_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret  = R_OSAL_Internal_TimedlockMutex(&handle->inner_mtx, handle->inner_timeout);
    if (OSAL_RETURN_OK != api_ret)
    {
       osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (false == handle->active)
        {
            osal_ret = OSAL_RETURN_HANDLE;
        }
        else
        {
            p_config->max_num_msg = handle->usr_config->config.max_num_msg;
            p_config->msg_size    = handle->usr_config->config.msg_size;
        }

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_get_config()
***********************************************************************************************************************/

