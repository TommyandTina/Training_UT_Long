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
* File Name :    r_osal_os_message.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Message Queue Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "target/qnx/r_osal_os_type.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_OS_MQ_MS_SLEEP_TIME
 * @brief Sleep time[ms] for Message queue Manager.
***********************************************************************************************************************/
#define OSAL_OS_MQ_MS_SLEEP_TIME          (1)

/*******************************************************************************************************************//**
 * @def OSAL_OS_MQ_MUTEX_DESTROY_TIMEOUT
 * Mutex destruction timeout value [msec].
***********************************************************************************************************************/
#define OSAL_OS_MQ_MUTEX_DESTROY_TIMEOUT  (500L)

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            Allocate message queue buffer.
* @param[in]        p_config The struct of Message queue configuration.
* @param[in,out]    mq       The struct of st_osal_os_mq_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_alloc_queue_buff(const st_osal_mq_config_t* const p_config, osal_os_mq_t mq);

/*******************************************************************************************************************//**
* @brief            Initialize conditon variable object.
* @param[in,out]    mq         The struct of st_osal_os_mq_t.
* @param[in]        p_mq_attr  The address of st_osal_os_mqattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_init_cond(osal_os_mq_t mq, const st_osal_os_mqattr_t* p_mq_attr);

/*******************************************************************************************************************//**
* @brief            Undo message create.
* @param[in]        p_config The struct of Message queue configuration.
* @param[in,out]    mq       The struct of st_osal_os_mq_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_undo_create(const st_osal_mq_config_t* const p_config, osal_os_mq_t mq);

/*******************************************************************************************************************//**
* @brief            Lock a mutex(timed lock).
* @param[in]        p_mutex     The pointor of mutex object.
* @param[in]        timeout     Specify the timeout time in milliseconds.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_timedlock_mutex(pthread_mutex_t *p_mutex,
                                                         osal_milli_sec_t timeout);

/*******************************************************************************************************************//**
* @brief            Calculate timestamp.
* @param[in]        p_config The struct of Message queue configuration.
* @param[in,out]    mq       The struct of st_osal_os_mq_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_calc_time_stamp(osal_milli_sec_t timeout, struct timespec* p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Destroy cond object.
* @param[in,out]    mq       The struct of st_osal_os_mq_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_destroy_cond(osal_os_mq_t mq);

/*******************************************************************************************************************//**
* @brief            Free message queue buffer.
* @param[in]        p_config The struct of Message queue configuration.
* @param[in,out]    mq       The struct of st_osal_os_mq_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_free_queue_buff(const st_osal_mq_config_t* const p_config, osal_os_mq_t mq);

/*******************************************************************************************************************//**
* @brief            Destroy mutex object.
* @param[in,out]    mq       The struct of st_osal_os_mq_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_destroy_mutex(osal_os_mq_t mq);

/*******************************************************************************************************************//**
* @brief            Sleep in milli second.
* @param[in]        milliseconds  Specify the sleep time in milliseconds.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_ms_sleep(osal_milli_sec_t milliseconds);

/*******************************************************************************************************************//**
* @brief            Initialize mutex attribute.
* @param[in]        p_mq_attr        The address of st_osal_os_mqattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_init_mutex_attr(osal_os_mqattr_t p_mq_attr);

/*******************************************************************************************************************//**
* @brief            Initialize condition vaiable attribute.
* @param[in]        p_mq_attr        The address of st_osal_os_mqattr_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_init_cond_attr(osal_os_mqattr_t p_mq_attr);

/***********************************************************************************************************************
* @brief            Send Message.
* @param[in]        mq           The struct of st_osal_os_mq_t.
* @param[in]        p_buffer     Data for sending a message.
* @param[in]        buffer_size  The value for buffer size.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_send(osal_os_mq_t mq, const void *p_buffer, size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Receive Message.
* @param[in]        mq           The struct of st_osal_os_mq_t.
* @param[out]       p_buffer     The address of void* for the receive data.
* @param[in]        buffer_size  The value for buffer size.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_receive(osal_os_mq_t mq, void *p_buffer, size_t buffer_size);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqCreate()
***********************************************************************************************************************/
/* PRQA S 3673 2 # Since the pointer type is typedefed, const cannot be added. */
e_osal_return_t R_OSAL_OS_MqCreate(const st_osal_mq_config_t* const p_config,
                                   const osal_os_mqattr_t mq_attr,
                                   osal_os_mq_t * p_mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    st_osal_os_mq_t *p_tmp_mq;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == p_config) || (NULL == mq_attr) || (NULL == p_mq))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_MqDelete(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_tmp_mq = (st_osal_os_mq_t*)calloc(1, sizeof(st_osal_os_mq_t));
        if (NULL == p_tmp_mq)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            osal_ret = r_osal_os_mq_alloc_queue_buff(p_config, p_tmp_mq);
            if (OSAL_RETURN_OK == osal_ret)
            {
                /* PRQA S 2736 1 # There is no problem because it will be released at R_OSAL_OS_MqDelete(). */
                os_ret = pthread_mutex_init(&p_tmp_mq->inner_mtx, &mq_attr->mutex_attr);
                if (EOK != os_ret)
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
                osal_ret = r_osal_os_mq_init_cond(p_tmp_mq, mq_attr);
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                p_tmp_mq->send_wait          = false;
                p_tmp_mq->receive_wait       = false;
                p_tmp_mq->head               = -1;
                p_tmp_mq->tail               = 0;
                p_tmp_mq->numofmsg           = 0;
                p_tmp_mq->config.max_num_msg = p_config->max_num_msg;
                p_tmp_mq->config.msg_size    = p_config->msg_size;
                p_tmp_mq->inner_timeout      = OSAL_INNER_TIMEOUT;

                *p_mq = p_tmp_mq;
            }
            else
            {
                r_osal_os_mq_undo_create(p_config, p_tmp_mq);
            }
        }   /* end of if (NULL == p_tmp_mq). */
    }   /* end of if ((NULL == p_config) || (NULL == mq_attr) || (NULL == p_mq)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqDelete()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqDelete(osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mq)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = r_osal_os_mq_timedlock_mutex(&mq->inner_mtx, mq->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if ((0U != mq->numofmsg) || (true == mq->receive_wait) || (true == mq->send_wait))
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                osal_ret = r_osal_os_mq_destroy_cond(mq);

                r_osal_os_mq_free_queue_buff(&mq->config, mq);
                mq->send_wait    = false;
                mq->receive_wait = false;
                mq->head         = -1;
                mq->tail         = 0;
                mq->numofmsg     = 0;
            }   /* end of if ((0U != mq->numofmsg) || (true == mq->receive_wait) || (true == mq->send_wait)) */

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_mq_destroy_mutex(mq);
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* PRQA S 5118 2 # Memory deallocation functions are available during deinitialization. */
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            free((void*)mq);
        }
        else
        {
            /* Do nothing */
        }
   }    /* end of if (NULL == mq) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqDelete()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqInitAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqInitAttr(osal_os_mqattr_t * p_mq_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;
    st_osal_os_mqattr_t *p_tmp_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_mq_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_MqDelete(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_tmp_attr = (st_osal_os_mqattr_t*)calloc(1, sizeof(st_osal_os_mqattr_t));
        if (NULL == p_tmp_attr)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            osal_ret = r_osal_os_mq_init_mutex_attr(p_tmp_attr);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_mq_init_cond_attr(p_tmp_attr);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)pthread_mutexattr_destroy(&p_tmp_attr->mutex_attr);
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
                *p_mq_attr = p_tmp_attr;
            }
            else
            {
                /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                free((void*)p_tmp_attr);
            }
        }   /* end of if (NULL == p_tmp_attr) */
    }   /* end of if ((NULL == cond) || (NULL == mutex)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqInitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqDeinitAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqDeinitAttr(osal_os_mqattr_t mq_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mq_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_mutexattr_destroy(&mq_attr->mutex_attr);
        if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        os_ret = pthread_condattr_destroy(&mq_attr->cond_attr);
        if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }

        /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        free((void*)mq_attr);
    }   /* end of if (NULL == mq_attr) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqDeinitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqSend()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqSend(osal_os_mq_t mq,
                                 const void * p_buffer,
                                 size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == mq) || (NULL == p_buffer))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = r_osal_os_mq_timedlock_mutex(&mq->inner_mtx, mq->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (mq->config.max_num_msg == mq->numofmsg)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_mq_send(mq, p_buffer, buffer_size);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (true == mq->receive_wait))
            {
                os_ret = pthread_cond_broadcast(&mq->receive_cond);
                if (EOK != os_ret)
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

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }   /* end of if ((NULL == mq) || (NULL == p_buffer)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqSend()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqTimedSend()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqTimedSend(osal_os_mq_t mq,
                                      const st_osal_time_t* const p_time_stamp,
                                      const void * p_buffer,
                                      size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec abstime;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == mq) || (NULL == p_time_stamp) || (NULL == p_buffer))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        abstime.tv_sec  = p_time_stamp->tv_sec;
        /* PRQA S 5209 1 # System is requesting an long, so it cannot be changed. */
        abstime.tv_nsec = (long)p_time_stamp->tv_nsec;

        os_ret = pthread_mutex_timedlock_monotonic(&mq->inner_mtx, &abstime);
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
            while ((mq->config.max_num_msg == mq->numofmsg) && (OSAL_RETURN_OK == osal_ret))
            {
                /* PRQA S 2982 1 # This flag is referenced in other thread. No problem. */
                mq->send_wait = true;
                os_ret = pthread_cond_timedwait(&mq->send_cond, &mq->inner_mtx, &abstime);
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
                mq->send_wait = false;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_mq_send(mq, p_buffer, buffer_size);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (true == mq->receive_wait))
            {
                os_ret = pthread_cond_broadcast(&mq->receive_cond);
                if (EOK != os_ret)
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

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
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
        }   /* end of if (OSAL_RETURN_OK == osal_ret) */
    }   /* end of if ((NULL == mq) || (NULL == p_time_stamp) || (NULL == p_buffer)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqTimedSend()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_035]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqReceive()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqReceive(osal_os_mq_t mq,
                                    void * p_buffer,
                                    size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)buffer_size; /* unused */

    if ((NULL == mq) || (NULL == p_buffer))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = r_osal_os_mq_timedlock_mutex(&mq->inner_mtx, mq->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (0U == mq->numofmsg)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_mq_receive(mq, p_buffer, mq->config.msg_size);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (true == mq->send_wait))
            {
                os_ret = pthread_cond_broadcast(&mq->send_cond);
                if (EOK != os_ret)
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

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }   /* end of if ((NULL == mq) || (NULL == p_buffer)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqReceive()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_036]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqTimedReceive()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqTimedReceive(osal_os_mq_t mq,
                                         const st_osal_time_t* const p_time_stamp,
                                         void * p_buffer,
                                         size_t buffer_size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec abstime;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)buffer_size; /* unused */

    if ((NULL == mq) || (NULL == p_time_stamp) || (NULL == p_buffer))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        abstime.tv_sec  = p_time_stamp->tv_sec;
        /* PRQA S 5209 1 # System is requesting an long, so it cannot be changed. */
        abstime.tv_nsec = (long)p_time_stamp->tv_nsec;

        os_ret = pthread_mutex_timedlock_monotonic(&mq->inner_mtx, &abstime);
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
            while ((0U == mq->numofmsg) && (OSAL_RETURN_OK == osal_ret))
            {
                /* PRQA S 2982 1 # This flag is referenced in other thread. No problem. */
                mq->receive_wait = true;
                os_ret = pthread_cond_timedwait(&mq->receive_cond, &mq->inner_mtx, &abstime);
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

                mq->receive_wait = false;
            }   /* end of while((0U == mq->numofmsg) && (OSAL_RETURN_OK == osal_ret)). */

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_mq_receive(mq, p_buffer, mq->config.msg_size);
            }
            else
            {
                /* Do nothing */
            }

            if ((OSAL_RETURN_OK == osal_ret) && (true == mq->send_wait))
            {
                os_ret = pthread_cond_broadcast(&mq->send_cond);
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

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
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
        }   /* end of if (OSAL_RETURN_OK == osal_ret) */
    }   /* end of if ((NULL == mq) || (NULL == p_time_stamp) || (NULL == p_buffer)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqTimedReceive()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqIsEmpty()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqIsEmpty(osal_os_mq_t mq,
                                    bool* const p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == mq) || (NULL == p_result))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = r_osal_os_mq_timedlock_mutex(&mq->inner_mtx, mq->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (0U == mq->numofmsg)
            {
                *p_result = true;
            }
            else    /* 0 != mq->numofmsg */
            {
                *p_result = false;
            }

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if (EOK != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }   /* end of if (NULL == mq) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqIsEmpty()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_038]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqIsFull()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqIsFull(osal_os_mq_t mq,
                                   bool* const p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == mq) || (NULL == p_result))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = r_osal_os_mq_timedlock_mutex(&mq->inner_mtx, mq->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (mq->config.max_num_msg == mq->numofmsg)
            {
                *p_result = true;
            }
            else    /* mq->config.max_num_msg != mq->numofmsg */
            {
                *p_result = false;
            }

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if (EOK != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }   /* end of if (NULL == mq) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqIsFull()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqReset()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqReset(osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          buffer_count;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mq)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = r_osal_os_mq_timedlock_mutex(&mq->inner_mtx, mq->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (true == mq->send_wait)
            {
                os_ret = pthread_cond_broadcast(&mq->send_cond);
                if (EOK != os_ret)
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
                if (NULL == mq->queue)
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

            buffer_count = 0U;
            while ((mq->config.max_num_msg > buffer_count) && (OSAL_RETURN_OK == osal_ret))
            {
                if (NULL == mq->queue[buffer_count].p_buffer)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    (void)memset(mq->queue[buffer_count].p_buffer,
                                 0x00,
                                 mq->config.msg_size);
                }
                buffer_count++;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                mq->head     = -1;
                mq->tail     = 0;
                mq->numofmsg = 0;
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }   /* end of if ((NULL == mq) || (NULL == p_result)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqReset()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqCheckBusy(osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mq)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        osal_ret = r_osal_os_mq_timedlock_mutex(&mq->inner_mtx, mq->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if ((0U != mq->numofmsg) || (true == mq->send_wait) || (true == mq->receive_wait))
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            os_ret = pthread_mutex_unlock(&mq->inner_mtx);
            if ((0 != os_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret) */
    }   /* end of if (NULL == mq) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_041]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_alloc_queue_buff()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_MqCreate() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_mq_alloc_queue_buff(const st_osal_mq_config_t* const p_config,
                                                          osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
    mq->queue = (st_osal_queue_t*)calloc(1, (sizeof(st_osal_queue_t) * p_config->max_num_msg));
    if (NULL == mq->queue)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        count = 0;
        while ((p_config->max_num_msg > count) && (OSAL_RETURN_OK == osal_ret))
        {
            /* PRQA S 2706 2 # There is no problem because it will be released at R_OSAL_OS_MqDelete(). */
            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            mq->queue[count].p_buffer = calloc(1, p_config->msg_size);
            if (NULL == mq->queue[count].p_buffer)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }
            count++;
        }
    }   /* end of if (NULL == p_tmp_mq->queue). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_alloc_queue_buff()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_init_cond()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_MqCreate() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_mq_init_cond(osal_os_mq_t mq,
                                                   const st_osal_os_mqattr_t* p_mq_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_cond_init(&mq->send_cond, &p_mq_attr->cond_attr);
    if (EOK != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = pthread_cond_init(&mq->receive_cond, &p_mq_attr->cond_attr);
        if (EOK != os_ret)
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
* End of function r_osal_os_mq_init_cond()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_030]
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_017
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_undo_create()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_MqCreate() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC void r_osal_os_mq_undo_create(const st_osal_mq_config_t* const p_config,
                                          osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)pthread_mutex_destroy(&mq->inner_mtx);
    (void)r_osal_os_mq_destroy_cond(mq);
    r_osal_os_mq_free_queue_buff(p_config, mq);

    /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
    free((void*)mq);

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_undo_create()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_025
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_timedlock_mutex()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_os_mq_timedlock_mutex(pthread_mutex_t *p_mutex,
                                                         osal_milli_sec_t timeout)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec time_stamp;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
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
        r_osal_os_mq_calc_time_stamp(timeout, &time_stamp);
        os_ret = pthread_mutex_timedlock_monotonic(p_mutex, &time_stamp);
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
* End of function r_osal_os_mq_timedlock_mutex()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_042]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_018
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_calc_time_stamp()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_mq_timedlock_mutex() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
OSAL_STATIC void r_osal_os_mq_calc_time_stamp(osal_milli_sec_t timeout,
                                              struct timespec* p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 4393 1 # QAC false indication (CCT change). */
    p_time_stamp->tv_sec  += (time_t)(timeout / OSAL_SECTOMSEC);
    p_time_stamp->tv_nsec += (int32_t)((timeout % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);
    if (0L != (p_time_stamp->tv_nsec % OSAL_MSECTONSEC))
    {
        p_time_stamp->tv_nsec = (int32_t)(((p_time_stamp->tv_nsec / OSAL_MSECTONSEC) + 1) * OSAL_MSECTONSEC);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_MAX_TV_NSEC < p_time_stamp->tv_nsec)
    {
        /* PRQA S 4393 1 # QAC false indication (CCT change). */
        p_time_stamp->tv_sec  += (time_t)(p_time_stamp->tv_nsec / (OSAL_MAX_TV_NSEC + 1));
        p_time_stamp->tv_nsec  = p_time_stamp->tv_nsec % (OSAL_MAX_TV_NSEC + 1);
    }
    else
    {
        /* Do nothing */
    }

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_calc_time_stamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_042]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_019
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_destroy_cond()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_destroy_cond(osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_cond_destroy(&mq->send_cond);
    if (EOK != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    os_ret = pthread_cond_destroy(&mq->receive_cond);
    if ((EOK != os_ret) && (OSAL_RETURN_OK == osal_ret))
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
* End of function r_osal_os_mq_destroy_cond()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_043]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_020
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_free_queue_buff()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_free_queue_buff(const st_osal_mq_config_t* const p_config,
                                              osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t buffer_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != mq->queue)
    {
        buffer_count = 0;
        while (p_config->max_num_msg > buffer_count)
        {
            if (NULL != mq->queue[buffer_count].p_buffer)
            {
                /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
                free((void*)mq->queue[buffer_count].p_buffer);
                mq->queue[buffer_count].p_buffer = NULL;
            }
            else
            {
                /* Do nothing */
            }
            buffer_count++;
        }

        /* PRQA S 5118 2 # Memory deallocation functions are available during deinitialization. */
        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        free((void*)mq->queue);
        mq->queue = NULL;
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != p_mq_control->queue). */

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_free_queue_buff()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_044]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_021
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_destroy_mutex()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_MqDelete() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_mq_destroy_mutex(osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    int32_t         waiting_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutex_destroy(&mq->inner_mtx);
    if (0 != os_ret)
    {
        waiting_time = 0;
        while ((OSAL_OS_MQ_MUTEX_DESTROY_TIMEOUT > waiting_time) && (EBUSY == os_ret))
        {
            (void)r_osal_os_mq_ms_sleep(OSAL_OS_MQ_MS_SLEEP_TIME);
            waiting_time += OSAL_OS_MQ_MS_SLEEP_TIME;
            os_ret = pthread_mutex_destroy(&mq->inner_mtx);
        }

        if ((OSAL_OS_MQ_MUTEX_DESTROY_TIMEOUT <= waiting_time) || (0 != os_ret))
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
* End of function r_osal_os_mq_destroy_mutex()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_022
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_ms_sleep()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_os_mq_destroy_mutex() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_mq_ms_sleep(osal_milli_sec_t milliseconds)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec current_time;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = clock_gettime(CLOCK_MONOTONIC, &current_time);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 4393 1 # QAC false indication (CCT change). */
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

        /* PRQA S 5209,2855 1 # System is requesting an int, so it cannot be changed. */
        os_ret = clock_nanosleep(CLOCK_MONOTONIC, (int)TIMER_ABSTIME, &current_time, NULL); 
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if(0 != os_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_ms_sleep()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_023
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_init_mutex_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_MqInitAttr() in order to improve the result of 
   static code analysis (STCAL of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_mq_init_mutex_attr(osal_os_mqattr_t p_mq_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_mutexattr_init(&p_mq_attr->mutex_attr);
    if (EOK != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_mutexattr_settype(&p_mq_attr->mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
        if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)pthread_mutexattr_destroy(&p_mq_attr->mutex_attr);
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if (EOK != os_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_init_mutex_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_024
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_init_cond_attr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_MqInitAttr() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_mq_init_cond_attr(osal_os_mqattr_t p_mq_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = pthread_condattr_init(&p_mq_attr->cond_attr);
    if (EOK != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_condattr_setclock(&p_mq_attr->cond_attr, CLOCK_MONOTONIC);
        if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)pthread_condattr_destroy(&p_mq_attr->cond_attr);
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if (EOK != os_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_init_cond_attr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_send()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_send(osal_os_mq_t mq,
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
    if (NULL == mq->queue)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if (NULL == mq->queue[mq->tail].p_buffer)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 1496 1 # OSAL Specifies the API argument as is. Guaranteed to be a constant size. */
        (void)memcpy(mq->queue[mq->tail].p_buffer, p_buffer, buffer_size);

        /* Registration of initial reading position */
        if (-1 == mq->head)
        {
            mq->head = mq->tail;
        }
        else
        {
            /* Do nothing */
        }

        /* Registration of next writing position */
        if ((mq->config.max_num_msg - 1U) == (uint64_t)mq->tail)
        {
            mq->tail = 0;
        }
        else
        {
            mq->tail++;
        }

        mq->numofmsg++;
    }   /* end of if(NULL == handle->queue). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_send()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MSG_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_receive()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_receive(osal_os_mq_t mq,
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
    if (NULL == mq->queue)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else if (NULL == mq->queue[mq->head].p_buffer)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 1496 1 # OSAL Specifies the API argument as is. Guaranteed to be a constant size. */
        (void)memcpy(p_buffer, mq->queue[mq->head].p_buffer, buffer_size);
        (void)memset(mq->queue[mq->head].p_buffer, 0x00, mq->config.msg_size);

        mq->numofmsg--;

        /* Registration of next reading position */
        if (0U == mq->numofmsg)
        {
            mq->head = -1;
        }
        else if ((mq->config.max_num_msg - (uint64_t)1) == (uint64_t)mq->head)
        {
            mq->head = 0;
        }
        else
        {
            mq->head++;
        }
    }   /* end of if(NULL == handle->queue). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_receive()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MSG_018]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
