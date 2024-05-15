/***********************************************************************************************************************
* Copyright [2022-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for Message queue
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "target/linux/r_osal_os_types.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Send the message.
 * @param[in]        mq             A handle of created message queue.
 * @param[in]        p_buffer       A pointer to the message to send.
 * @param[in]        buffer_size    Size of the message to send.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_send(osal_os_mq_t mq, const void* p_buffer, size_t buffer_size);

/***********************************************************************************************************************
 * @brief            Receive the message.
 * @param[in]        mq             A handle of created message queue.
 * @param[in]        p_buffer       A pointer to the message to send.
 * @param[in]        buffer_size    Size of the message to send.
 * @return           #void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_receive(osal_os_mq_t mq, void* p_buffer, size_t buffer_size);

/***********************************************************************************************************************
 * @brief            Common function for destroy the mutex.
 * @param[in]        p_mutex    A pointer to the mutex of message manager.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_mutex_destroy(osal_os_mutex_t* p_mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex for time period.
 * @param[in]        mutex    A handle of mutex of message manager.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_mutex_lock_for_time_period(osal_os_mutex_t mutex);

/***********************************************************************************************************************
 * @brief            Common function for lock the mutex until timestamp.
 * @param[in]        mutex         A handle of mutex of message manager.
 * @param[in]        p_time_stamp  Time to wait until the queue is available.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_TIME
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_mutex_until_timestamp(osal_os_mutex_t mutex, const st_osal_time_t* const p_time_stamp);

/***********************************************************************************************************************
 * @brief            Common function for destroy the condition variable.
 * @param[in]        p_cond    A pointer to the condition variable of pthread.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_cond_destroy(osal_os_cond_t* p_cond);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqCheckBusy()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_MqCheckBusy(osal_os_mq_t mq)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;

    if (NULL == mq)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_lock_for_time_period(mq->mq_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if ((0U != mq->numofmsg) ||
                (false != mq->send_wait_flag) ||
                (false != mq->receive_wait_flag))
            {
                osal_ret = OSAL_RETURN_BUSY;
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(mq->mq_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_102
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqCreate()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_MqCreate(const st_osal_mq_config_t* const p_config, const osal_os_mqattr_t mq_attr, osal_os_mq_t* p_mq)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if ((NULL == p_config) ||
        (NULL == mq_attr) ||
        (NULL == p_mq))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        *p_mq = (osal_os_mq_t)R_OSAL_OS_Malloc(sizeof(st_osal_os_mq_t));
        if (NULL == *p_mq)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        (*p_mq)->p_osal_config      = p_config;
        (*p_mq)->numofmsg           = 0;
        (*p_mq)->send_wait_flag     = false;
        (*p_mq)->receive_wait_flag  = false;
        (*p_mq)->head               = NULL;
        (*p_mq)->tail               = NULL;

        osal_ret = R_OSAL_OS_CondCreate(&(*p_mq)->send_cond);

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_CondCreate(&(*p_mq)->receive_cond);
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_MutexCreate(mq_attr->mutex_attr, &(*p_mq)->mq_mtx);
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)r_osal_os_mq_cond_destroy(&(*p_mq)->send_cond);
            (void)r_osal_os_mq_cond_destroy(&(*p_mq)->receive_cond);

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)*p_mq);
            *p_mq = NULL;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_103
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqDelete()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqDelete(osal_os_mq_t mq)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == mq)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_destroy(&mq->mq_mtx);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_cond_destroy(&mq->send_cond);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_cond_destroy(&mq->receive_cond);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)mq);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqDelete()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_104
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqSend()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqSend(osal_os_mq_t mq, const void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;

    if ((NULL == mq) ||
        (NULL == p_buffer))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_lock_for_time_period(mq->mq_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* If message queue is full, return BUSY */
            if (mq->numofmsg == mq->p_osal_config->max_num_msg)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_mq_send(mq, p_buffer, buffer_size);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_OS_CondSignal(mq->receive_cond);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(mq->mq_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_os_mq_mutex_lock_for_time_period */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from if ((NULL == mq) || (NULL == p_buffer)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqSend()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_105
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqTimedSend()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqTimedSend(osal_os_mq_t mq, const st_osal_time_t* const p_time_stamp, const void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;

    if ((NULL == mq) ||
        (NULL == p_time_stamp) ||
        (NULL == p_buffer))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_until_timestamp(mq->mq_mtx, p_time_stamp);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* If message queue is full, wait for receive */
            while ((mq->numofmsg == mq->p_osal_config->max_num_msg) && (OSAL_RETURN_OK == osal_ret))
            {
                /* PRQA S 2982 1 # This flag is referenced in other thread. No problem. */
                mq->send_wait_flag = true;
                osal_ret = R_OSAL_OS_CondTimedWait(mq->send_cond, mq->mq_mtx, p_time_stamp);
                mq->send_wait_flag = false;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_os_mq_send(mq, p_buffer, buffer_size);
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_OS_CondSignal(mq->receive_cond);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(mq->mq_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_os_mq_mutex_until_timestamp */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from if ((NULL == mq) || (NULL == p_time_stamp) || (NULL == p_buffer)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqTimedSend()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_026]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_106
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqReceive()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqReceive(osal_os_mq_t mq, void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;

    if ((NULL == mq) ||
        (NULL == p_buffer))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_lock_for_time_period(mq->mq_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* If message queue is empty, return BUSY */
            if (0U == mq->numofmsg)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                r_osal_os_mq_receive(mq, p_buffer, buffer_size);

                osal_ret = R_OSAL_OS_CondSignal(mq->send_cond);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(mq->mq_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqReceive()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_107
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqTimedReceive()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqTimedReceive(osal_os_mq_t mq, const st_osal_time_t* const p_time_stamp, void* p_buffer, size_t buffer_size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;

    if ((NULL == mq) ||
        (NULL == p_time_stamp) ||
        (NULL == p_buffer))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_until_timestamp(mq->mq_mtx, p_time_stamp);

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* If message queue is empty, wait for send */
            while ((0U == mq->numofmsg) && (OSAL_RETURN_OK == osal_ret))
            {
                /* PRQA S 2982 1 # This flag is referenced in other thread. No problem. */
                mq->receive_wait_flag = true;
                osal_ret = R_OSAL_OS_CondTimedWait(mq->receive_cond, mq->mq_mtx, p_time_stamp);
                mq->receive_wait_flag = false;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                r_osal_os_mq_receive(mq, p_buffer, buffer_size);

                osal_ret = R_OSAL_OS_CondSignal(mq->send_cond);
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(mq->mq_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_os_mq_mutex_until_timestamp */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from if ((NULL == mq) || (NULL == p_time_stamp) || (NULL == p_buffer)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqTimedReceive()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_108
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqIsFull()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_MqIsFull(osal_os_mq_t mq, bool* const p_result)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;

    if ((NULL == mq) ||
        (NULL == p_result))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_lock_for_time_period(mq->mq_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (mq->numofmsg == mq->p_osal_config->max_num_msg)
            {
                *p_result = true;
            }
            else
            {
                *p_result = false;
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(mq->mq_mtx);
            if (OSAL_RETURN_OK != mutex_ret)
            {
                osal_ret = mutex_ret;
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqIsFull()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_109
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqIsEmpty()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_MqIsEmpty(osal_os_mq_t mq, bool* const p_result)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;

    if ((NULL == mq) ||
        (NULL == p_result))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_lock_for_time_period(mq->mq_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (0U == mq->numofmsg)
            {
                *p_result = true;
            }
            else
            {
                *p_result = false;
            }

            mutex_ret = R_OSAL_OS_MutexUnlock(mq->mq_mtx);
            if (OSAL_RETURN_OK != mutex_ret)
            {
                osal_ret = mutex_ret;
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqIsEmpty()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_110
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqReset()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqReset(osal_os_mq_t mq)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t mutex_ret;
    bool            isfull = false;
    void*           p_buffer;

    if (NULL == mq)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mq_mutex_lock_for_time_period(mq->mq_mtx);

        if (OSAL_RETURN_OK == osal_ret)
        {
            if (mq->numofmsg == mq->p_osal_config->max_num_msg) /* Check if current queue is FULL or not*/
            {
                isfull = true;
            }

            p_buffer = R_OSAL_OS_Malloc(mq->p_osal_config->msg_size);
            if (NULL == p_buffer)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                /* Reset the message queue */
                while (0U != mq->numofmsg)
                {
                    r_osal_os_mq_receive(mq, p_buffer, mq->p_osal_config->msg_size);
                }

                if (true == isfull) /* Unlock waiting thread if queue before reset is FULL*/
                {
                    osal_ret = R_OSAL_OS_CondBroadcast(mq->send_cond);
                }
            }

            R_OSAL_OS_Free(p_buffer);

            mutex_ret = R_OSAL_OS_MutexUnlock(mq->mq_mtx);
            if ((OSAL_RETURN_OK != mutex_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = mutex_ret;
            }
        } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from r_osal_os_mq_mutex_lock_for_time_period */
    } /* end of if (OSAL_RETURN_OK == osal_ret) osal_ret is return value from if (NULL == mq) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqReset()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_111
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqInitAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqInitAttr(osal_os_mqattr_t* p_mq_attr)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == p_mq_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        *p_mq_attr = (osal_os_mqattr_t)R_OSAL_OS_Malloc(sizeof(st_osal_os_mqattr_t));
        if (NULL == *p_mq_attr)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexInitAttr(&(*p_mq_attr)->mutex_attr);

        if (OSAL_RETURN_OK != osal_ret)
        {
            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)*p_mq_attr);
            *p_mq_attr = NULL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqInitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_112
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqDeinitAttr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_MqDeinitAttr(osal_os_mqattr_t mq_attr)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == mq_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_MutexDeinitAttr(mq_attr->mutex_attr);
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)mq_attr);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqDeinitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_113
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_send()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_send(osal_os_mq_t mq, const void* p_buffer, size_t buffer_size)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    st_osal_mq_node_t*  tmp_node;

    /* PRQA S 0316 1 # Required cast on system, not modifiable. */
    tmp_node = (st_osal_mq_node_t *)R_OSAL_OS_Malloc(sizeof(st_osal_mq_node_t));
    if (NULL == tmp_node)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        tmp_node->buffer = R_OSAL_OS_Malloc(buffer_size);
        if (NULL == tmp_node->buffer)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL == mq->tail)
        {
            mq->head = tmp_node;
            mq->tail = tmp_node;
            /* PRQA S 1496 1 # Caller of this API should guarantee size of buffer */
            (void)memcpy(tmp_node->buffer, p_buffer, buffer_size);
            tmp_node->next = NULL;
        }
        else
        {
            mq->tail->next = tmp_node;
            /* PRQA S 1496 1 # Caller of this API should guarantee size of buffer */
            (void)memcpy(tmp_node->buffer, p_buffer, buffer_size);
            tmp_node->next = NULL;
            mq->tail = tmp_node;
        }
        mq->numofmsg++;
    }
    else
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)tmp_node);
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_send()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_114
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_receive()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_receive(osal_os_mq_t mq, void* p_buffer, size_t buffer_size)
{
    st_osal_mq_node_t* tmp_node;

    if (NULL != mq->head)
    {
        if (NULL == mq->head->next)
        {
            /* PRQA S 1496 1 # Caller of this API should guarantee size of buffer */
            (void)memcpy(p_buffer, mq->head->buffer, buffer_size);

            R_OSAL_OS_Free(mq->head->buffer);
            mq->head->buffer = NULL;

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)mq->head);
            mq->head = NULL;
            mq->tail = NULL;
        }
        else
        {
            tmp_node = mq->head->next;
            /* PRQA S 1496 1 # Caller of this API should guarantee size of buffer */
            (void)memcpy(p_buffer, mq->head->buffer, buffer_size);

            R_OSAL_OS_Free(mq->head->buffer);
            mq->head->buffer = NULL;

            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)mq->head);
            mq->head = tmp_node;
        } /* end of if (NULL == mq->head->next) */

        mq->numofmsg--;
    }
    else
    {
        mq->numofmsg = 0;
    } /* end of if (NULL != mq->head) */

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_receive()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_035]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_115
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_mutex_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_mutex_destroy(osal_os_mutex_t* p_mutex)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_MutexDestroy(*p_mutex);
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_mutex = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_mutex_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_036]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_116
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_mutex_lock_for_time_period()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
OSAL_STATIC e_osal_return_t r_osal_os_mq_mutex_lock_for_time_period(osal_os_mutex_t mutex)
{
    e_osal_return_t osal_ret;
    st_osal_time_t  time_stamp;

    osal_ret = R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(&time_stamp);

    if (OSAL_RETURN_OK == osal_ret)
    {
        time_stamp.tv_nsec += (int32_t)((OSAL_INNER_TIMEOUT % OSAL_SECTOMSEC) * OSAL_MSECTONSEC);

        if (0L != (time_stamp.tv_nsec % OSAL_MSECTONSEC))
        {
            time_stamp.tv_nsec = (int32_t)(((time_stamp.tv_nsec / OSAL_MSECTONSEC) + 1) * OSAL_MSECTONSEC);
        }

        if (OSAL_MAX_TV_NSEC < time_stamp.tv_nsec)
        {
            /* PRQA S 4393 1 # System is requesting a time_t, so it cannot be changed. */
            time_stamp.tv_sec   += (time_t)(time_stamp.tv_nsec / (OSAL_MAX_TV_NSEC + 1));
            time_stamp.tv_nsec   = (int32_t)(time_stamp.tv_nsec % (OSAL_MAX_TV_NSEC + 1));
        }

        osal_ret = R_OSAL_OS_MutexTimedLock(mutex, &time_stamp);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_mutex_lock_for_time_period()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_118
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_mutex_until_timestamp()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_mutex_until_timestamp(osal_os_mutex_t mutex, const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_MutexTimedLock(mutex, p_time_stamp);
    if ((OSAL_RETURN_OK != osal_ret) &&
        (OSAL_RETURN_TIME != osal_ret))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_mutex_until_timestamp()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MSG_117
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mq_cond_destroy()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mq_cond_destroy(osal_os_cond_t* p_cond)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_OS_CondDestroy(*p_cond);
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_cond = NULL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mq_cond_destroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MSG_038]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
