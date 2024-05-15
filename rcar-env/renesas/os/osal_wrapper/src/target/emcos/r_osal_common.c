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
* File Name :    r_osal_common.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper common functions for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <inttypes.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
/* PRQA S 5127 1 # Need to use clock function. */
#include <time.h>

#include "target/emcos/r_osal_common.h"


/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Types_Variables_Common Common Private type variables.
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct osal_inner_thread_attr_t
 * structure to thread attribute.
***********************************************************************************************************************/
typedef struct st_osal_inner_thread_attr
{
    pthread_attr_t      attr_obj;
}st_osal_inner_thread_attr_t;

/*******************************************************************************************************************//**
 * @struct st_osal_inner_thread_control_t
 * structure to Handle of thread.
***********************************************************************************************************************/
typedef struct st_osal_inner_thread_control
{
    pthread_t           thread;         /*!< Handle of thread for POSIX */
    uint16_t            num;            /*!< Index number containing the handle */
}st_osal_inner_thread_control_t;

/*******************************************************************************************************************//**
 * @struct st_osal_inner_mutexattr_t
 * structure to mutex attribute.
***********************************************************************************************************************/
typedef struct st_osal_inner_mutexattr
{
    pthread_mutexattr_t attr;
}st_osal_inner_mutexattr_t;

/*******************************************************************************************************************//**
 * @struct st_osal_inner_mutex_control_t
 * structure to Handle of Mutex.
***********************************************************************************************************************/
typedef struct st_osal_inner_mutex_control
{
    pthread_mutex_t     mutex;         /*!< Handle of Mutex of POSIX */
}st_osal_inner_mutex_control_t;

/*******************************************************************************************************************//**
 * @struct st_osal_inner_mq_control_t
 * structure to Handle of message.
***********************************************************************************************************************/
typedef struct st_osal_inner_mq_control
{
    bool                      send_wait;     /*!< Send wait flag */
    bool                      receive_wait;  /*!< Receive wait flag */
    st_osal_queue_t           *queue;        /*!< Pointer of queue */
    int32_t                   head;          /*!< Element number of the next queue to read */
    int32_t                   tail;          /*!< Element number of the next queue to write */
    pthread_mutex_t           inner_mtx;     /*!< Internal mutex object */
    osal_milli_sec_t          inner_timeout; /*!< Waiting time of lock inner_mtx */
    pthread_cond_t            send_cond;     /*!< Handle of Condition Variable for waiting to send */
    pthread_cond_t            receive_cond;  /*!< Handle of Condition Variable for waiting to receive */
    uint32_t                  numofmsg;      /*!< Number of messages in the queue */
    st_osal_inner_mq_config_t config;        /*!< Structure to Message queue configuration */
}st_osal_inner_mq_control_t;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Static_Variables_Common Common Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_context_key
 * thread-specific data key
***********************************************************************************************************************/
OSAL_STATIC pthread_key_t gs_context_key;

/*******************************************************************************************************************//**
 * @var gs_inner_thread_attr_mngr_num
 * the numbert of the thread attribute manager table
***********************************************************************************************************************/
OSAL_STATIC size_t gs_inner_thread_attr_mngr_num = 0;

/*******************************************************************************************************************//**
 * @var gsp_inner_thread_attr_mngr_tbl
 * the thread attribute manager table
***********************************************************************************************************************/
OSAL_STATIC osal_inner_thread_attr_t *gsp_inner_thread_attr_mngr_tbl = NULL;

/*******************************************************************************************************************//**
 * @var gs_inner_mutex_attr_mngr_num
 * the numbert of the mutex attribute manager table
***********************************************************************************************************************/
OSAL_STATIC size_t gs_inner_mutex_attr_mngr_num = 0;

/*******************************************************************************************************************//**
 * @var gsp_inner_mutex_attr_mngr_tbl
 * the mutex attribute manager table
***********************************************************************************************************************/
OSAL_STATIC osal_inner_mutexattr_t *gsp_inner_mutex_attr_mngr_tbl = NULL;

/** @} */

/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Functions_Common Common Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Initialize the Device related Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
* @retval           OSAL_RETURN_CONF
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_initialize(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize the Device related Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinitialize(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_deinitialize_pre(void);

/*******************************************************************************************************************//**
* @brief            Pre-check for Deinitialize the Device related Manager.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_BUSY
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_STATE
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinitailize_pre(uint16_t *p_init_flg);

/*******************************************************************************************************************//**
* @brief            Recovery the OSAL Manager initialization flag.
* @param[in]        init_flg   Initialization target manager determination flag.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_recover_init_state(uint16_t init_flg);

/*******************************************************************************************************************//**
* @brief            Recovery the Device related Manager initialization flag.
* @param[in]        init_flg   Initialization target manager determination flag.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_recover_io_init_state(uint16_t init_flg);

/*******************************************************************************************************************//**
* @brief            Destroy attribute information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_free_attr_info(void);

/*******************************************************************************************************************//**
* @brief            Destroy mutex attribute information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_free_mutexattr_info(void);

/*******************************************************************************************************************//**
* @brief            Destroy thread attribute information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_free_threadattr_info(void);

/*******************************************************************************************************************//**
* @brief            Set the thread attribute process.
* @param[in]        p_attr_info    A pointer to thread attribute information.
* @param[in,out]    p_attr         A pointer to st_osal_inner_thread_attr_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_threadattr_proc(const st_osal_inner_thread_attrinfo_t *const p_attr_info,
                                                              st_osal_inner_thread_attr_t *p_attr);

/*******************************************************************************************************************//**
* @brief            Set mutex attribute for the message queue manager.
* @param[in]        p_mutex_attr    The address of pthread_mutexattr_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_mutex_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Allocate the maximum number of messages for the message queue manager.
* @param[in]        p_handle    The address of st_osal_inner_mq_control_t.
* @param[in]        p_config    The address of st_osal_inner_mq_config_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_alloc(st_osal_inner_mq_control_t *p_handle,
                                                   const st_osal_inner_mq_config_t *const p_config);

/*******************************************************************************************************************//**
* @brief            Initialize "mutex object" and "send cond object" and "recieve cond object" 
                    the message queue manager.
* @param[in]        p_handle        The address of st_osal_inner_mq_control_t.
* @param[in]        p_mutex_attr    The address of pthread_mutexattr_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_init_mutex_cond(st_osal_inner_mq_control_t *p_handle,
                                                          const pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Destroy mutex attribute for the message queue manager.
* @param[in]        p_mutex_attr    The address of pthread_mutexattr_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_destroy_mutex_attr(pthread_mutexattr_t *p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Get the current time and adds the specified time.
* @param[in,out]    p_wait_time    The time that the specified time was added.
* @param[in]        p_time_period  Pointer to add time.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_add_time(struct timespec *p_wait_time,
                                                       osal_milli_sec_t time_period);

/*******************************************************************************************************************//**
* @brief            Send mq for ISR context
* @param[in]        handle       The struct of osal_inner_mq_handle_t.
* @param[in]        p_buffer     The address of void* for the send data.
* @param[in]        buffer_size  The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_send_is_isr(osal_inner_mq_handle_t handle,
                                                         const void *p_buffer,
                                                         size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Send mq for not ISR context
* @param[in]        handle        The struct of osal_inner_mq_handle_t.
* @param[in]        p_time_stamp  The wait time specified by the user.
* @param[in]        p_buffer      The address of void* for the send data.
* @param[in]        buffer_size   The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_send_not_isr(osal_inner_mq_handle_t handle,
                                                          const struct timespec *p_time_stamp,
                                                          const void *p_buffer,
                                                          size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Recieve mq for ISR context
* @param[in]        handle       The struct of osal_inner_mq_handle_t.
* @param[out]       p_buffer     The address of void* for the receive data.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_recv_is_isr(osal_inner_mq_handle_t handle,
                                                         void *p_buffer);

/*******************************************************************************************************************//**
* @brief            Recieve mq for not ISR context
* @param[in]        handle        The struct of osal_inner_mq_handle_t.
* @param[in]        p_time_stamp  The wait time specified by the user.
* @param[out]       p_buffer      The address of void* for the receive data.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_recv_not_isr(osal_inner_mq_handle_t handle,
                                                          const struct timespec *p_time_stamp,
                                                          void *p_buffer);

/*******************************************************************************************************************//**
* @brief            Send Message.
* @param[in]        handle       The struct of osal_inner_mq_handle_t.
* @param[in]        p_buffer     The address of void* for the send data.
* @param[in]        buffer_size  The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_send(osal_inner_mq_handle_t handle,
                                                  const void *p_buffer, 
                                                  size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Receive Message.
* @param[in]        handle       The struct of osal_inner_mq_handle_t.
* @param[out]       p_buffer     The address of void* for the receive data.
* @param[in]        buffer_size  The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_receive(osal_inner_mq_handle_t handle,
                                                     void *p_buffer,
                                                     size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Wait until the message can be sent and send the message.
* @param[in]        handle       The struct of osal_inner_mq_handle_t.
* @param[in]        p_wait_time  The wait time specified by the user.
* @param[in]        p_buffer     The address of void* for the send data.
* @param[in]        buffer_size  The value for buffer size.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_wait_and_send(osal_inner_mq_handle_t handle,
                                                           const struct timespec *p_wait_time,
                                                           const void * p_buffer,
                                                           size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Wait until the message can be receive and receive the message.
* @param[in]        handle       The struct of osal_inner_mq_handle_t.
* @param[in]        p_wait_time  The wait time specified by the user.
* @param[out]       p_buffer     The address of void* for the receive data.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_wait_and_recv(osal_inner_mq_handle_t handle,
                                                           const struct timespec *p_wait_time,
                                                           void *p_buffer);

/*******************************************************************************************************************//**
* @brief            Delete message queue.
* @param[in,out]    handle       The struct of osal_inner_mq_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_queue_delete(osal_inner_mq_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Destroy cond object.
* @param[in,out]    handle       The struct of osal_inner_mq_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_cond_destroy(osal_inner_mq_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Destroy mutex object.
* @param[in,out]    handle       The struct of osal_inner_mq_handle_t.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_mutex_destroy(osal_inner_mq_handle_t handle);

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_Initialize()
* XOS3_OSAL_EMCOS_CD_CD_COM_001
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_004]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_018]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    R_OSAL_RCFG_Init();
    osal_ret = osal_clock_time_initialize();
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_thread_initialize();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_threadsync_initialize();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_mq_initialize();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_io_initialize();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Deinitialize()
* XOS3_OSAL_EMCOS_CD_CD_COM_002
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_002]
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_018]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_internal_deinitialize_pre();
    if (OSAL_RETURN_OK != osal_ret)
    {
        /* Do nothing */
    }
    else
    {
        osal_ret = osal_clock_time_deinitialize();
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_mq_deinitialize();
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_threadsync_deinitialize();
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_io_deinitialize();
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_thread_deinitialize();
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_free_attr_info();
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(OSAL_RETURN_OK != osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_CheckTimestamp()
* XOS3_OSAL_EMCOS_CD_CD_COM_003
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_005]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_CheckTimestamp(const st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    /* PRQA S 2996 2 # It is a check for robustness, because it is carried out based on the design is no problem.
                       True if the correct handle was passed. False detection of the tool. */
    else if (((time_t)OSAL_MAX_SEC < p_time_stamp->tv_sec) || ((time_t)0L > p_time_stamp->tv_sec))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if ((OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec) || (0L > p_time_stamp->tv_nsec))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_CheckTimestamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_GetTimestamp()
* XOS3_OSAL_EMCOS_CD_CD_COM_004
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_009]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_Internal_GetTimestamp(struct timespec *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_time_stamp)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        os_ret = clock_gettime(CLOCK_REALTIME, p_time_stamp);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if(NULL == p_time_stamp). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_GetTimestamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MsSleep()
* XOS3_OSAL_EMCOS_CD_CD_COM_005
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_010]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_Internal_MsSleep(osal_milli_sec_t milliseconds)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    struct timespec current_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0 <= milliseconds)
    {
        os_ret = clock_gettime(CLOCK_REALTIME, &current_time);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 4393 1 # System is requesting an time_t, so it cannot be changed. */
            current_time.tv_sec  += (time_t)(milliseconds / OSAL_SECTOMSEC);
            /* PRQA S 5209 1 # System is requesting an long, so it cannot be changed. */
            current_time.tv_nsec += (long)((milliseconds % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);
            if (OSAL_MAX_TV_NSEC < current_time.tv_nsec)
            {
                current_time.tv_sec++;
                current_time.tv_nsec = current_time.tv_nsec - (OSAL_MAX_TV_NSEC + 1);
            }
            else
            {
                /* Do nothing */
            }

            /* PRQA S 2855,5209 1 # System is requesting an int, so it cannot be changed. */
            os_ret = clock_nanosleep(CLOCK_REALTIME, (int)TIMER_ABSTIME, &current_time, NULL);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(0 != os_ret). */
    }
    else
    {
        osal_ret = OSAL_RETURN_PAR;
    }   /* end of if(0 <= milliseconds). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_MsSleep()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_TimedlockMutex()
* XOS3_OSAL_EMCOS_CD_CD_COM_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_012]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_Internal_TimedlockMutex(pthread_mutex_t *p_mutex, osal_milli_sec_t timeout)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    struct timespec time_stamp;
    osal_milli_sec_t timeout_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_mutex)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_Internal_GetTimestamp(&time_stamp);
        if (OSAL_RETURN_OK != osal_ret)
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
        timeout_tmp = timeout;
        osal_ret = R_OSAL_Internal_TimespecAdd(&time_stamp, &timeout_tmp);
        if (OSAL_RETURN_OK != osal_ret)
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
        os_ret = pthread_mutex_timedlock(p_mutex, &time_stamp);
        if (ETIMEDOUT == os_ret)
        {
            osal_ret = OSAL_RETURN_TIME;
        }
        else if (0 != os_ret)
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
* End of function R_OSAL_Internal_TimedlockMutex()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_TimeRoundUp()
* XOS3_OSAL_EMCOS_CD_CD_COM_007
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_013]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_Internal_TimeRoundUp(struct timespec *p_timespec)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    struct timespec time_spec;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_timespec)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        time_spec.tv_sec  = p_timespec->tv_sec;
        time_spec.tv_nsec = p_timespec->tv_nsec;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((time_spec.tv_nsec % 1000000L) != 0L)
        {
            /* PRQA S 5209 1 # System is requesting an long, so it cannot be changed. */
            time_spec.tv_nsec = (long)(((time_spec.tv_nsec / OSAL_MSECTONSEC) + 1) * OSAL_MSECTONSEC);
            if (time_spec.tv_nsec > OSAL_MAX_TV_NSEC)
            {
                time_spec.tv_sec++;
                time_spec.tv_nsec = time_spec.tv_nsec - (OSAL_MAX_TV_NSEC + 1);
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
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        p_timespec->tv_sec  = time_spec.tv_sec;
        p_timespec->tv_nsec = time_spec.tv_nsec;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_TimeRoundUp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_TimespecAdd()
* XOS3_OSAL_EMCOS_CD_CD_COM_008
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_014]
***********************************************************************************************************************/
/* PRQA S 3673 2 # Implemented in accordance with the requirement. */
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_Internal_TimespecAdd(struct timespec *p_timespec, osal_milli_sec_t *p_time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    struct timespec time_spec;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == p_timespec) || (NULL == p_time_period))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        time_spec.tv_sec  = p_timespec->tv_sec;
        time_spec.tv_nsec = p_timespec->tv_nsec;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4393 1 # System is requesting an time_t, so it cannot be changed. */
        time_spec.tv_sec  += (time_t)(*p_time_period / OSAL_SECTOMSEC);
        /* PRQA S 5209 1 # System is requesting an long, so it cannot be changed. */
        time_spec.tv_nsec += (long)((*p_time_period % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);

        if (OSAL_MAX_TV_NSEC < time_spec.tv_nsec)
        {
            time_spec.tv_sec++;
            time_spec.tv_nsec = time_spec.tv_nsec - (OSAL_MAX_TV_NSEC + 1);
        }
        else
        {
            /* Do nothing */
        }

        osal_ret = R_OSAL_Internal_TimeRoundUp(&time_spec);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
        p_timespec->tv_sec  = time_spec.tv_sec;
        p_timespec->tv_nsec = time_spec.tv_nsec;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_TimespecAdd()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_CreateThreadKey()
* XOS3_OSAL_EMCOS_CD_CD_COM_009
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_006]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_CreateThreadKey(void)
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
    os_ret = pthread_key_create(&gs_context_key, NULL);
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
* End of function R_OSAL_Internal_CreateThreadKey()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_DeleteThreadKey()
* XOS3_OSAL_EMCOS_CD_CD_COM_010
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_007]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_DeleteThreadKey(void)
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
    os_ret = pthread_key_delete(gs_context_key);
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
* End of function R_OSAL_Internal_DeleteThreadKey()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_GetThreadSpecific()
* XOS3_OSAL_EMCOS_CD_CD_COM_011
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_008]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
void * R_OSAL_Internal_GetThreadSpecific(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    void * p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_get_value = pthread_getspecific(gs_context_key);

    return p_get_value;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_GetThreadSpecific()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_SetThreadSpecific()
* XOS3_OSAL_EMCOS_CD_CD_COM_012
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_011]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_SetThreadSpecific(const void* p_set_value)
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
    if (NULL != p_set_value)
    {
        os_ret = pthread_setspecific(gs_context_key, p_set_value);
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
        osal_ret = OSAL_RETURN_PAR;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_SetThreadSpecific()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_SetThreadAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_SetThreadAttr(const st_osal_inner_thread_attrinfo_t *const p_attr_info,
                                              osal_inner_thread_attr_t *const p_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    st_osal_inner_thread_attr_t *p_tmp_attr;
    osal_inner_thread_attr_t    *p_mngr_tbl;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118,2707 2 # Safty Manual, malloc use are allowed.Because it is called from the API
                            assuming that it is called from Initialize, there is no problem */
    p_tmp_attr = (st_osal_inner_thread_attr_t *)malloc(sizeof(st_osal_inner_thread_attr_t));
    if (NULL == p_tmp_attr)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        osal_ret = osal_internal_set_threadattr_proc(p_attr_info, p_tmp_attr);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 0314 3 # The parameter must be void* and there is no side side-effect in this case. */
            /* PRQA S 5118,2707 3 # Safty Manual, realloc use are allowed.Because it is called from the API
                                    assuming that it is called from Initialize, there is no problem. */
            p_mngr_tbl = realloc((void *)gsp_inner_thread_attr_mngr_tbl,
                                 (gs_inner_thread_attr_mngr_num + 1U) * (sizeof(osal_inner_thread_attr_t)));
            if (NULL == p_mngr_tbl)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                gsp_inner_thread_attr_mngr_tbl = p_mngr_tbl;
                gsp_inner_thread_attr_mngr_tbl[gs_inner_thread_attr_mngr_num] = p_tmp_attr;
                gs_inner_thread_attr_mngr_num++;

                *p_attr = p_tmp_attr;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        if (OSAL_RETURN_OK != osal_ret)
        {
            /* PRQA S 0314 3 # The parameter must be void* and there is no side side-effect in this case. */
            /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
                               assuming that it is called from Initialize, there is no problem. */
            free((void*)p_tmp_attr);
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(NULL == p_tmp_attr). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_SetThreadAttr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_ThreadCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_ThreadCreate(const st_osal_inner_thread_config_t *const p_config,
                                             osal_inner_thread_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                             os_ret;
    st_osal_inner_thread_control_t  *p_tmp_handle;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118,2707 2 # Safty Manual, malloc use are allowed.Because it is called from the API
    assuming that it is called from Initialize, there is no problem. */
    p_tmp_handle = (st_osal_inner_thread_control_t *)malloc(sizeof(st_osal_inner_thread_control_t));
    if (NULL == p_tmp_handle)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        os_ret = pthread_create(&p_tmp_handle->thread,
                                &(*p_config->attr)->attr_obj,
                                p_config->func,
                                p_config->userarg);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            /* PRQA S 0314 3 # The parameter must be void* and there is no side side-effect in this case. */
            /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
             assuming that it is called from Initialize, there is no problem. */
            free((void*)p_tmp_handle);
        }
        else
        {
            p_tmp_handle->num = 0xFFFFU;
            *p_handle = p_tmp_handle;
        }
    }   /* end of if(NULL == p_tmp_handle). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_ThreadCreate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_ThreadTimedJoin()
***********************************************************************************************************************/
/* PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_osal_return_t R_OSAL_Internal_ThreadTimedJoin(osal_inner_thread_handle_t const handle,
                                                int64_t *const p_return_value,
                                                const osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    void            *p_get_value = NULL;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)time_period;  /* not use */
    os_ret = pthread_join(handle->thread, &p_get_value);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if ((NULL != p_get_value) && (NULL != p_return_value))
        {
            /* PRQA S 0326 1 # No problem for casting from void * to object pointer */
            *p_return_value = (int64_t)p_get_value;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 3 # The parameter must be void* and there is no side side-effect in this case. */
        /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
                           assuming that it is called from Initialize, there is no problem. */
        free((void*)handle);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_ThreadTimedJoin()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_SetMutexAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_SetMutexAttr(osal_inner_mutexattr_t *const p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;
    st_osal_inner_mutexattr_t   *p_tmp_attr;
    osal_inner_mutexattr_t      *p_mngr_tbl;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118,2707 1 # Safty Manual, malloc use are allowed.Because it is called from the API assuming that it is called from Initialize, there is no problem. */
    p_tmp_attr = (st_osal_inner_mutexattr_t *)malloc(sizeof(st_osal_inner_mutexattr_t));
    if (NULL == p_tmp_attr)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        os_ret = pthread_mutexattr_init(&p_tmp_attr->attr);
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
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            os_ret = pthread_mutexattr_settype(&p_tmp_attr->attr, (int)PTHREAD_MUTEX_ERRORCHECK);
            if (0 != os_ret)
            {
                /* For post-processing in the event of an error, do not perform a return value determination. */
                (void)pthread_mutexattr_destroy(&p_tmp_attr->attr);
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
            /* PRQA S 0314 3 # The parameter must be void* and there is no side side-effect in this case. */
            /* PRQA S 5118,2707 2 # Safty Manual, realloc use are allowed.Because it is called from the
                                    API assuming that it is called from Initialize, there is no problem. */
            p_mngr_tbl = realloc((void *)gsp_inner_mutex_attr_mngr_tbl,
                                 (gs_inner_mutex_attr_mngr_num + 1U) * sizeof(osal_inner_mutexattr_t));
            if (NULL == p_mngr_tbl)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                gsp_inner_mutex_attr_mngr_tbl = p_mngr_tbl;
                gsp_inner_mutex_attr_mngr_tbl[gs_inner_mutex_attr_mngr_num] = p_tmp_attr;
                gs_inner_mutex_attr_mngr_num++;

                *p_mutex_attr = p_tmp_attr;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK == osal_ret). */

        if (OSAL_RETURN_OK != osal_ret)
        {
            /* PRQA S 0314 2 # The parameter must be void* and there is no side side-effect in this case. */
            /* PRQA S 5118 1 # Safty Manual, malloc use are allowed.Because it is called from the API assuming that it is called from Initialize, there is no problem. */
            free((void*)p_tmp_attr);
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(NULL == p_tmp_attr). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_SetMutexAttr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MutexInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_MutexInit(const osal_inner_mutexattr_t *const p_mutex_attr,
                                          osal_inner_mutex_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                             os_ret;
    st_osal_inner_mutex_control_t   *p_tmp_handle;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118,2707 1 # Safty Manual, malloc use are allowed.Because it is called from the API assuming that it is called from Initialize, there is no problem. */
    p_tmp_handle = (st_osal_inner_mutex_control_t *)malloc(sizeof(st_osal_inner_mutex_control_t));
    if (NULL == p_tmp_handle)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 2736 1 # No problem because R_OSAL_Internal_MutexDestroy is called pthread_mutex_destroy. */
        os_ret = pthread_mutex_init(&p_tmp_handle->mutex, &(*p_mutex_attr)->attr);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            /* PRQA S 0314 2 # The parameter must be void*) and there is no side side-effect in this case. */
            /* PRQA S 5118,2707 1 # Safty Manual, malloc use are allowed.Because it is called from the API assuming that it is called from Initialize, there is no problem. */
            free((void*)p_tmp_handle);
        }
        else
        {
            *p_handle = p_tmp_handle;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_MutexInit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MutexTimedLock()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_Internal_MutexTimedLock(osal_inner_mutex_handle_t handle,
                                               osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    struct timespec time_stamp;
    osal_milli_sec_t time_period_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    time_period_tmp = time_period;
    api_ret = R_OSAL_Internal_GetTimestamp(&time_stamp);
    if (OSAL_RETURN_OK != api_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        api_ret = R_OSAL_Internal_TimespecAdd(&time_stamp, &time_period_tmp);
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_mutex_timedlock(&handle->mutex, &time_stamp);
        if (ETIMEDOUT == os_ret)
        {
            osal_ret = OSAL_RETURN_TIME;
        }
        else if (0 != os_ret)
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
* End of function R_OSAL_Internal_MutexTimedLock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MutexUnlock()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_Internal_MutexUnlock(osal_inner_mutex_handle_t handle)
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
    os_ret = pthread_mutex_unlock(&handle->mutex);
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
* End of function R_OSAL_Internal_MutexUnlock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MutexDestroy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_MutexDestroy(osal_inner_mutex_handle_t handle)
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
    os_ret = pthread_mutex_destroy(&handle->mutex);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 0314 2 # The parameter must be void* and there is no side side-effect in this case. */
        /* PRQA S 5118,2707 1 # Safty Manual, malloc use are allowed.Because it is called from the API assuming that it is called from DeInitialize, there is no problem. */
        free((void*)handle);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_MutexDestroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MqCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_MqCreate(const st_osal_inner_mq_config_t *const p_config,
                                         osal_inner_mq_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    e_osal_return_t             api_ret;
    st_osal_inner_mq_control_t  *p_tmp_handle;
    pthread_mutexattr_t         mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118 1 # Safty Manual, malloc use are allowed.Because it is called from the API assuming that it is called from Initialize, there is no problem. */
    p_tmp_handle = (st_osal_inner_mq_control_t *)malloc(sizeof(st_osal_inner_mq_control_t));
    if (NULL == p_tmp_handle)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        api_ret = osal_internal_set_mutex_attr(&mutex_attr);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if ((0U == p_config->max_num_msg) || (0U == p_config->msg_size))
            {
                osal_ret = OSAL_RETURN_CONF;
            }
            else
            {
                osal_ret = osal_internal_mq_alloc(p_tmp_handle, p_config);
            }
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_init_mutex_cond(p_tmp_handle, &mutex_attr);
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                p_tmp_handle->send_wait          = false;
                p_tmp_handle->receive_wait       = false;
                p_tmp_handle->head               = -1;
                p_tmp_handle->tail               = 0;
                p_tmp_handle->numofmsg           = 0;
                p_tmp_handle->config.max_num_msg = p_config->max_num_msg;
                p_tmp_handle->config.msg_size    = p_config->msg_size;
                p_tmp_handle->inner_timeout      = OSAL_INNER_TIMEOUT;

                *p_handle = p_tmp_handle;
            }
            else
            {
                /* PRQA S 0314 3 # The parameter must be void* and there is no side side-effect in this case. */
                /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
                                   assuming that it is called from Initialize, there is no problem. */
                free((void*)p_tmp_handle);
            }

            api_ret = osal_internal_destroy_mutex_attr(&mutex_attr);
            if ((OSAL_RETURN_OK != api_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != api_ret). */
    }   /* end of if(NULL == p_tmp_attr). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_MqCreate()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MqTimedSend()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_MqTimedSend(osal_inner_mq_handle_t handle,
                                            osal_milli_sec_t time_period,
                                            const void * p_buffer,
                                            size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    struct timespec time_stamp = {0, 0};
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_get_value = R_OSAL_Internal_GetThreadSpecific();

    /* Is ISR Context */
    if (NULL != p_get_value)
    {
        osal_ret = osal_internal_mq_send_is_isr(handle, p_buffer, buffer_size);
    }
    else /* Is Normal Context */
    {
        osal_ret = osal_internal_get_add_time(&time_stamp, time_period);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mq_send_not_isr(handle, &time_stamp, p_buffer, buffer_size);
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_MqTimedSend()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MqTimedReceive()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_MqTimedReceive(osal_inner_mq_handle_t handle,
                                               osal_milli_sec_t time_period,
                                               void * p_buffer,
                                               size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    struct timespec time_stamp = {0, 0};
    const void      *p_get_value;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)buffer_size; /*  not use */
    p_get_value = R_OSAL_Internal_GetThreadSpecific();

    /* Is ISR Context */
    if (NULL != p_get_value)
    {
        osal_ret = osal_internal_mq_recv_is_isr(handle, p_buffer);
    }
    else /* Is Normal Context */
    {
        osal_ret = osal_internal_get_add_time(&time_stamp, time_period);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = osal_internal_mq_recv_not_isr(handle, &time_stamp, p_buffer);
        }
        else
        {
            /* Do nothing */
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_MqTimedReceive()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_MqDelete()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_MqDelete(osal_inner_mq_handle_t handle)
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
            osal_ret = osal_internal_mq_cond_destroy(handle);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = osal_internal_mq_queue_delete(handle);
            }
            else
            {
                /* Do nothing */
            }
        }

        os_ret = pthread_mutex_unlock(&handle->inner_mtx);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }   /* end of if(OSAL_RETURN_OK != api_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_internal_mq_mutex_destroy(handle);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 2 # The parameter must be void* and there is no side side-effect in this case. */
        /* PRQA S 5118 1 # Safty Manual, malloc use are allowed.Because it is called from the API assuming that it is called from DeInitialize, there is no problem. */
        free((void*)handle);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_MqDelete()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_GetHighPrecisTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_GetHighPrecisTimeStamp(st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    struct timespec time_stamp = {0, 0};

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = R_OSAL_Internal_GetTimestamp(&time_stamp);
    if (OSAL_RETURN_OK != api_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        api_ret = R_OSAL_Internal_TimeRoundUp(&time_stamp);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* output time_stamp */
            p_time_stamp->tv_sec  = time_stamp.tv_sec;
            p_time_stamp->tv_nsec = (int32_t)time_stamp.tv_nsec;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_GetHighPrecisTimeStamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_GetHighResoTimeStamp()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_GetHighResoTimeStamp(st_osal_time_t *const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    e_osal_return_t api_ret;
    struct timespec time_stamp = {0, 0};

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = R_OSAL_Internal_GetTimestamp(&time_stamp);
    if (OSAL_RETURN_OK != api_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        api_ret = R_OSAL_Internal_TimeRoundUp(&time_stamp);
        if (OSAL_RETURN_OK != api_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* output time_stamp */
            p_time_stamp->tv_sec  = time_stamp.tv_sec;
            p_time_stamp->tv_nsec = (int32_t)time_stamp.tv_nsec;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_GetHighResoTimeStamp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_Internal_CalcTimeDifference()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_Internal_CalcTimeDifference(const st_osal_time_t *const p_time2,
                                                   const st_osal_time_t *const p_time1,
                                                   osal_nano_sec_t *const p_time_difference)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_nano_sec_t nsec1;
    osal_nano_sec_t nsec2;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    nsec1 = ((osal_nano_sec_t)p_time1->tv_sec * (osal_nano_sec_t)OSAL_SECTONSEC) + p_time1->tv_nsec;
    nsec2 = ((osal_nano_sec_t)p_time2->tv_sec * (osal_nano_sec_t)OSAL_SECTONSEC) + p_time2->tv_nsec;

    *p_time_difference = nsec2 - nsec1;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_Internal_CalcTimeDifference()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_initialize()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_initialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    R_OSAL_DCFG_Init();
    osal_ret = osal_io_initialize();

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_pm_initialize();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_interrupt_initialize();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_memory_initialize();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_deinitialize()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinitialize(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_interrupt_deinitialize();
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_pm_deinitialize();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_io_deinitialize();
        R_OSAL_DCFG_Deinit();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_memory_deinitialize();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_io_deinitialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_deinitialize_pre()
* XOS3_OSAL_EMCOS_CD_CD_COM_013
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_001]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_deinitialize_pre(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    uint16_t        init_flg = 0U;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_clock_time_deinitialize_pre();
    if (OSAL_RETURN_OK == osal_ret)
    {
        init_flg |= OSAL_ISINIT_CLOCKTIME;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_mq_deinitialize_pre();
        if (OSAL_RETURN_OK == osal_ret)
        {
            init_flg |= OSAL_ISINIT_MQ;
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
        osal_ret = osal_threadsync_deinitialize_pre();
        if (OSAL_RETURN_OK == osal_ret)
        {
            init_flg |= OSAL_ISINIT_THREADSYNC;
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
        osal_ret = osal_internal_io_deinitailize_pre(&init_flg);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_thread_deinitialize_pre();
        init_flg |= OSAL_ISINIT_THREAD;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK != osal_ret)
    {
        osal_internal_recover_init_state(init_flg);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_deinitialize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_io_deinitailize_pre()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_io_deinitailize_pre(uint16_t *p_init_flg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = osal_interrupt_deinitialize_pre();
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_init_flg |= OSAL_ISINIT_INTERRUPT;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = osal_pm_deinitialize_pre();
        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_init_flg |= OSAL_ISINIT_PM;
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
        osal_ret = osal_io_deinitialize_pre();
        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_init_flg |= OSAL_ISINIT_IO;
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
        osal_ret = osal_memory_deinitialize_pre();
        if (OSAL_RETURN_OK == osal_ret)
        {
            *p_init_flg |= OSAL_ISINIT_MEMORY;
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
* End of function osal_internal_io_deinitailize_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_recover_init_state()
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_recover_init_state(uint16_t init_flg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_ISINIT_CLOCKTIME == (init_flg & OSAL_ISINIT_CLOCKTIME))
    {
        osal_clock_time_set_is_init();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_ISINIT_MQ == (init_flg & OSAL_ISINIT_MQ))
    {
        osal_mq_set_is_init();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_ISINIT_THREADSYNC == (init_flg & OSAL_ISINIT_THREADSYNC))
    {
        osal_threadsync_set_is_init();
    }
    else
    {
        /* Do nothing */
    }

    /* device related OSAL manager initialization flag recover. */
    osal_internal_recover_io_init_state(init_flg);

    if (OSAL_ISINIT_THREAD == (init_flg & OSAL_ISINIT_THREAD))
    {
        osal_thread_set_is_init();
    }
    else
    {
        /* Do nothing */
    }

    return;
}
/***********************************************************************************************************************
* End of function osal_internal_recover_init_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_recover_io_init_state()
***********************************************************************************************************************/
OSAL_STATIC void osal_internal_recover_io_init_state(uint16_t init_flg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (OSAL_ISINIT_INTERRUPT == (init_flg & OSAL_ISINIT_INTERRUPT))
    {
        osal_interrupt_set_is_init();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_ISINIT_PM == (init_flg & OSAL_ISINIT_PM))
    {
        osal_pm_set_is_init();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_ISINIT_IO == (init_flg & OSAL_ISINIT_IO))
    {
        osal_io_set_is_init();
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_ISINIT_MEMORY == (init_flg & OSAL_ISINIT_MEMORY))
    {
        osal_memory_set_is_init();
    }
    else
    {
        /* Do nothing */
    }

    return;
}
/***********************************************************************************************************************
* End of function osal_internal_recover_io_init_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_free_attr_info()
* XOS3_OSAL_EMCOS_CD_CD_COM_015
* [XOS3_OSAL_EMCOS_UD_DD_COM_002]
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_free_attr_info(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t api_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    api_ret = osal_internal_free_mutexattr_info();
    if (OSAL_RETURN_OK != api_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    api_ret = osal_internal_free_threadattr_info();
    if (OSAL_RETURN_OK != api_ret)
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
* End of function osal_internal_free_attr_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_free_mutexattr_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_free_mutexattr_info(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    size_t          i;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_inner_mutex_attr_mngr_tbl)
    {
        for (i = 0; i < gs_inner_mutex_attr_mngr_num; i++)
        {
            os_ret = pthread_mutexattr_destroy(&gsp_inner_mutex_attr_mngr_tbl[i]->attr);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
            /* PRQA S 0314 3 # Required cast on system, not modifiable. */
            /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
                               assuming that it is called from DeInitialize, there is no problem. */
            free((void*)gsp_inner_mutex_attr_mngr_tbl[i]);
        }
        /* PRQA S 0314 3 # Required cast on system, not modifiable. */
        /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
                           assuming that it is called from Initialize, there is no problem. */
        free((void*)gsp_inner_mutex_attr_mngr_tbl);
        gsp_inner_mutex_attr_mngr_tbl = NULL;
        gs_inner_mutex_attr_mngr_num  = 0;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != gsp_inner_mutex_attr_mngr_tbl). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_free_mutexattr_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_free_threadattr_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_free_threadattr_info(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    size_t          i;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_inner_thread_attr_mngr_tbl)
    {
        for (i = 0; i < gs_inner_thread_attr_mngr_num; i++)
        {
            os_ret = pthread_attr_destroy(&gsp_inner_thread_attr_mngr_tbl[i]->attr_obj);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
            /* PRQA S 0314 3 # Required cast on system, not modifiable. */
            /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
            assuming that it is called from DeInitialize, there is no problem. */
            free((void*)gsp_inner_thread_attr_mngr_tbl[i]);
        }
        /* PRQA S 0314 3 # Required cast on system, not modifiable. */
        /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
                           assuming that it is called from DeInitialize, there is no problem. */
        free((void*)gsp_inner_thread_attr_mngr_tbl);
        gsp_inner_thread_attr_mngr_tbl = NULL;
        gs_inner_thread_attr_mngr_num  = 0;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != gsp_inner_thread_attr_mngr_tbl). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_free_threadattr_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_set_threadattr_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_threadattr_proc(const st_osal_inner_thread_attrinfo_t *const p_attr_info,
                                                              st_osal_inner_thread_attr_t *p_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret    = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         policy;
    struct sched_param          th_priority;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_attr_init(&p_attr->attr_obj);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        os_ret = pthread_attr_setinheritsched(&p_attr->attr_obj, (int)PTHREAD_EXPLICIT_SCHED);
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_attr_destroy(&p_attr->attr_obj);
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (OSAL_SCHED_FIFO == p_attr_info->policy)
        {
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            policy = (int)SCHED_FIFO;
        }
        else
        {
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            policy = (int)SCHED_RR;
        }

        os_ret = pthread_attr_setschedpolicy(&p_attr->attr_obj, policy);
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_attr_destroy(&p_attr->attr_obj);
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
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        th_priority.sched_priority = (int)p_attr_info->priority;
        os_ret = pthread_attr_setschedparam(&p_attr->attr_obj, &th_priority);
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_attr_destroy(&p_attr->attr_obj);
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
        os_ret = pthread_attr_setstacksize(&p_attr->attr_obj,
                                           (p_attr_info->stack_size + (size_t)PTHREAD_STACK_MIN));
        if (0 != os_ret)
        {
            /* For post-processing in the event of an error, do not perform a return value determination. */
            (void)pthread_attr_destroy(&p_attr->attr_obj);
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
* End of function osal_internal_set_threadattr_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_set_mutex_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_set_mutex_attr(pthread_mutexattr_t *p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                 os_ret;

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
* End of function osal_internal_set_mutex_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_alloc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_alloc(st_osal_inner_mq_control_t *p_handle,
                                                   const st_osal_inner_mq_config_t *const p_config)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    uint64_t        buffer_count = 0U;

    /*------------------------------------------------------------------------------------------------------------------
    Function bodyj
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
     assuming that it is called from Initialize, there is no problem. */
    p_handle->queue = (st_osal_queue_t *)malloc(sizeof(st_osal_queue_t) * (uint32_t)p_config->max_num_msg);
    if (NULL == p_handle->queue)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        while ((p_config->max_num_msg > buffer_count) && (OSAL_RETURN_OK == osal_ret))
        {
            /* PRQA S 5118,2706 2 # Safty Manual, malloc use are allowed.Because it is called from the API
             assuming that it is called from Initialize, there is no problem. */
            p_handle->queue[buffer_count].p_buffer = malloc((uint32_t)p_config->max_num_msg);
            if (NULL == p_handle->queue[buffer_count].p_buffer)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }
            buffer_count++;
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            for (buffer_count = 0U; p_config->max_num_msg > buffer_count; buffer_count++)
            {
                if (NULL != p_handle->queue[buffer_count].p_buffer)
                {
                    /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
                    assuming that it is called from Initialize, there is no problem. */
                    free((void*)p_handle->queue[buffer_count].p_buffer);
                }
                else
                {
                    /* Do nothing */
                }
            }
            /* PRQA S 0314 3 # The parameter must be void* and there is no side side-effect in this case. */
            /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
            assuming that it is called from Initialize, there is no problem. */
            free((void*)p_handle->queue);
        }
        else
        {
            /* Do nothing */
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }   /* end of if(NULL == p_handle->queue). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_alloc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_init_mutex_cond()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_init_mutex_cond(st_osal_inner_mq_control_t *p_handle,
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
    /* PRQA S 2736 1 # No problem because osal_internal_mq_mutex_destroy is called pthread_mutex_destroy. */
    os_ret = pthread_mutex_init(&p_handle->inner_mtx, p_mutex_attr);
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
        os_ret = pthread_cond_init(&p_handle->send_cond, NULL);
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
        os_ret = pthread_cond_init(&p_handle->receive_cond, NULL);
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
* End of function osal_internal_init_mutex_cond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_destroy_mutex_attr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_destroy_mutex_attr(pthread_mutexattr_t *p_mutex_attr)
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
* End of function osal_internal_destroy_mutex_attr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_get_add_time()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_get_add_time(struct timespec *p_wait_time,
                                                       osal_milli_sec_t time_period)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    osal_milli_sec_t time_period_tmp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    time_period_tmp = time_period;
    osal_ret = R_OSAL_Internal_GetTimestamp(p_wait_time);
    if (OSAL_RETURN_OK != osal_ret)
    {
       osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = R_OSAL_Internal_TimespecAdd(p_wait_time, &time_period_tmp);
        if (OSAL_RETURN_OK != osal_ret)
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
* End of function osal_internal_get_add_time()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_send_is_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_send_is_isr(osal_inner_mq_handle_t handle,
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
        if (handle->config.max_num_msg == handle->numofmsg)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            api_ret = osal_internal_mq_send(handle, p_buffer, buffer_size);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
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
* End of function osal_internal_mq_send_is_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_send_not_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_send_not_isr(osal_inner_mq_handle_t handle,
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
        osal_ret = osal_internal_mq_wait_and_send(handle, p_time_stamp, p_buffer, buffer_size);

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
* End of function osal_internal_mq_send_not_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_recv_is_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_recv_is_isr(osal_inner_mq_handle_t handle,
                                                         void *p_buffer)
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
        if (0U == handle->numofmsg)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            api_ret = osal_internal_mq_receive(handle, p_buffer, (size_t)handle->config.msg_size);
            if (OSAL_RETURN_OK != api_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
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
* End of function osal_internal_mq_recv_is_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_recv_not_isr()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_recv_not_isr(osal_inner_mq_handle_t handle,
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
        osal_ret = osal_internal_mq_wait_and_recv(handle, p_time_stamp, p_buffer);

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
* End of function osal_internal_mq_recv_not_isr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_send()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_send(osal_inner_mq_handle_t handle,
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
        /* PRQA S 1496 1 # Do not use the return value, so do not change it. */
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
        if ((handle->config.max_num_msg - 1U) == (uint64_t)handle->tail)
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
* End of function osal_internal_mq_send()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_receive()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_receive(osal_inner_mq_handle_t handle,
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
        /* PRQA S 1496 1 # Do not use the return value, so do not change it. */
        (void)memcpy(p_buffer,
                     handle->queue[handle->head].p_buffer,
                     buffer_size);

        (void)memset(handle->queue[handle->head].p_buffer,
                     0x00,
                     (size_t)handle->config.msg_size);

        handle->numofmsg--;

        /* Registration of next reading position */
        if (0U == handle->numofmsg)
        {
            handle->head = -1;
        }
        else if ((handle->config.max_num_msg - 1U) == (uint64_t)handle->head)
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
* End of function osal_internal_mq_receive()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_wait_and_send()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_wait_and_send(osal_inner_mq_handle_t handle,
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
    if (handle->config.max_num_msg == handle->numofmsg)
    {
        while ((handle->config.max_num_msg == handle->numofmsg) &&
               (OSAL_RETURN_OK == osal_ret))
        {
            /* PRQA S 2982 1 # Because the pthread_cond_timedwait is in wait, this flag must be true. Do not change.. */
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
        }   /* end of while((handle->config.max_num_msg == handle->numofmsg) && (OSAL_RETURN_OK == osal_ret)). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(handle->config.max_num_msg == handle->numofmsg). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        api_ret = osal_internal_mq_send(handle,
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
* End of function osal_internal_mq_wait_and_send()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_wait_and_recv()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_wait_and_recv(osal_inner_mq_handle_t handle,
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
            /* PRQA S 2982 1 # Because the pthread_cond_timedwait is in wait, this flag must be true. Do not change.. */
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
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(0U == handle->numofmsg). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        api_ret = osal_internal_mq_receive(handle,
                                           p_buffer,
                                           (size_t)handle->config.msg_size);
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
* End of function osal_internal_mq_wait_and_recv()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_queue_delete()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_queue_delete(osal_inner_mq_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          buffer_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if(NULL != handle->queue)
    {
        buffer_count = 0;
        while (handle->config.max_num_msg > buffer_count)
        {
            if (NULL != handle->queue[buffer_count].p_buffer)
            {
                /* PRQA S 5118,2707 1 # Safty Manual, malloc use are allowed.Because it is called from the API assuming that it is called from DeInitialize, there is no problem. */
                free((void*)handle->queue[buffer_count].p_buffer);
                handle->queue[buffer_count].p_buffer = NULL;
            }
            else
            {
                /* Do nothing */
            }
            buffer_count++;
        }
        /* PRQA S 0314 3 # The parameter must be void* and there is no side side-effect in this case. */
        /* PRQA S 5118 2 # Safty Manual, malloc use are allowed.Because it is called from the API
         assuming that it is called from DeInitialize, there is no problem. */
        free((void*)handle->queue);
        handle->queue = NULL;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != handle->queue). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function osal_internal_mq_queue_delete()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_cond_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_cond_destroy(osal_inner_mq_handle_t handle)
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
    os_ret = pthread_cond_destroy(&handle->send_cond);
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
        os_ret = pthread_cond_destroy(&handle->receive_cond);
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
* End of function osal_internal_mq_cond_destroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function osal_internal_mq_mutex_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t osal_internal_mq_mutex_destroy(osal_inner_mq_handle_t handle)
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
    os_ret = pthread_mutex_destroy(&handle->inner_mtx);
    if (0 != os_ret)
    {
        waiting_time = 0;
        while ((OSAL_MUTEX_DESTROY_TIMEOUT > waiting_time) && (EBUSY == os_ret))
        {
            (void)R_OSAL_Internal_MsSleep(OSAL_MESSAGE_MS_SLEEP_TIME);
            waiting_time += OSAL_MESSAGE_MS_SLEEP_TIME;
            os_ret = pthread_mutex_destroy(&handle->inner_mtx);
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
* End of function osal_internal_mq_mutex_destroy()
***********************************************************************************************************************/

