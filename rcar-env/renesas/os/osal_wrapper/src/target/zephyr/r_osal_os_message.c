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
* Version :      0.3.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Message Queue Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <zephyr/posix/semaphore.h>

#include "target/zephyr/r_osal_os_type.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

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
* @brief            Undo message create.
* @param[in]        p_config The struct of Message queue configuration.
* @param[in,out]    mq       The struct of st_osal_os_mq_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_undo_create(const st_osal_mq_config_t* const p_config, osal_os_mq_t mq);

/*******************************************************************************************************************//**
* @brief            Free message queue buffer.
* @param[in]        p_config The struct of Message queue configuration.
* @param[in,out]    mq       The struct of st_osal_os_mq_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_free_queue_buff(const st_osal_mq_config_t* const p_config, osal_os_mq_t mq);

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
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_001
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
    (void)mq_attr; /* unused */
    if ((NULL == p_config) || (NULL == p_mq))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2707 3 # There is no problem because it will be released at R_OSAL_OS_MqDelete(). */
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
                /* PRQA S 2736 1 # There is no problem because it will be destroyed at R_OSAL_OS_MqDelete(). */
                os_ret = sem_init(&p_tmp_mq->sync_semaphore, 0, 0);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                p_tmp_mq->snd_wait_count    = 0;
                p_tmp_mq->rcv_wait_count    = 0;
                p_tmp_mq->head               = -1;
                p_tmp_mq->tail               = 0;
                p_tmp_mq->numofmsg           = 0;
                p_tmp_mq->config.max_num_msg = p_config->max_num_msg;
                p_tmp_mq->config.msg_size    = p_config->msg_size;

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
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqDelete()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqDelete(osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
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
        mq->spin_key = k_spin_lock(&mq->spin_lock);
#ifdef OSAL_ZEPHYR_UT_PERFORMANCE
        mq->numofmsg = 0;
#endif
        if ((0U != mq->numofmsg) || (0U < mq->rcv_wait_count) || (0U < mq->snd_wait_count))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            os_ret = sem_destroy(&mq->sync_semaphore);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                r_osal_os_mq_free_queue_buff(&mq->config, mq);
                mq->snd_wait_count = 0;
                mq->rcv_wait_count = 0;
                mq->head            = -1;
                mq->tail            = 0;
                mq->numofmsg        = 0;
            }
        }   /* end of if ((0U != mq->numofmsg) || (0 < mq->rcv_wait_count) || (0 < mq->snd_wait_count)) */

        k_spin_unlock(&mq->spin_lock, mq->spin_key);

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
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqInitAttr()
***********************************************************************************************************************/
/*Note: This function exists only for compatibility with the OSAL common part and only returns OSAL_RETURN_OK. 
        There is no actual processing for the Zephyr OSAL wrapper. */
/* PRQA S 3673 1 # Implemented in accordance with the specification. */
e_osal_return_t R_OSAL_OS_MqInitAttr(osal_os_mqattr_t * p_mq_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)p_mq_attr; /* unused */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqInitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqDeinitAttr()
***********************************************************************************************************************/
/*Note: This function exists only for compatibility with the OSAL common part and only returns OSAL_RETURN_OK. 
        There is no actual processing for the Zephyr OSAL wrapper. */
/* PRQA S 3673 1 # Implemented in accordance with the specification. */
e_osal_return_t R_OSAL_OS_MqDeinitAttr(osal_os_mqattr_t mq_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)mq_attr; /* unused */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqDeinitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_005
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
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
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
        mq->spin_key = k_spin_lock(&mq->spin_lock);

        if (mq->config.max_num_msg == mq->numofmsg)
        {
            /* No wait and return the busy error immediately. */
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

        /* Notify the waiting thread of the arrival of the message. */
        if ((OSAL_RETURN_OK == osal_ret) && (0U < mq->rcv_wait_count) && (1U >= mq->numofmsg))
        {
            k_spin_unlock(&mq->spin_lock, mq->spin_key);
            os_ret = sem_post(&mq->sync_semaphore);
            mq->spin_key = k_spin_lock(&mq->spin_lock);
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

        k_spin_unlock(&mq->spin_lock, mq->spin_key);
    }   /* end of if ((NULL == mq) || (NULL == p_buffer)) */

    return osal_ret;
/* PRQA S 9107,9110 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqSend()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_006
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
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    int             sem_count = 0;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
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
        abstime.tv_nsec = (long)p_time_stamp->tv_nsec;/* PRQA S 5209 # System is requesting an long, so it cannot be changed. */

        mq->spin_key = k_spin_lock(&mq->spin_lock);

        /* Wait for the available message queue. */
        while ((mq->config.max_num_msg == mq->numofmsg) && (OSAL_RETURN_OK == osal_ret))
        {
            /* If the message queue is empty, wait for the semaphore signaled by the other task or ISR. */
            /* PRQA S 2982 1 # This flag is referenced in other thread. No problem. */
            mq->snd_wait_count++;
            k_spin_unlock(&mq->spin_lock, mq->spin_key);
            os_ret = sem_timedwait(&mq->sync_semaphore, (struct timespec*)&abstime);
            mq->spin_key = k_spin_lock(&mq->spin_lock);
            mq->snd_wait_count--;
            if (0 != os_ret)
            {
                /* PRQA S 5209 1 # tv_nsec is declared by the OS code and the following check has no side-effect. */
                if ((abstime.tv_sec >= (time_t)0) && (abstime.tv_nsec < (long)NSEC_PER_SEC))
                {
                    osal_ret = OSAL_RETURN_TIME;
                }
                else
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }
            else
            {
                /* do nothing */
            }

            os_ret = sem_getvalue(&mq->sync_semaphore, &sem_count);
            if (0 == os_ret)
            {
                while (0 < sem_count)
                {
                    os_ret = sem_trywait(&mq->sync_semaphore);
                    if (0 != os_ret)
                    {
                        /* sem_trywait error do nothing */
                    }
                    else
                    {
                        /* do nothing */
                    }
                    sem_count--;
                }
            }
            else
            {
                /* do nothing */
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_mq_send(mq, p_buffer, buffer_size);
        }
        else
        {
            /* Do nothing */
        }

        /* Notify the waiting thread of the arrival of the message. */
        if ((OSAL_RETURN_OK == osal_ret) && (0U < mq->rcv_wait_count) && (1U >= mq->numofmsg))
        {
            /* Unlock the spin lock before calling sem_post so that the context switching is performed. */
            /* Re-lock the spin lock after calling sem_post for the exclusion control of this context. */
            k_spin_unlock(&mq->spin_lock, mq->spin_key);
            os_ret = sem_post(&mq->sync_semaphore);
            k_yield();
            mq->spin_key = k_spin_lock(&mq->spin_lock);
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

        k_spin_unlock(&mq->spin_lock, mq->spin_key);
    }   /* end of if ((NULL == mq) || (NULL == p_time_stamp) || (NULL == p_buffer)) */

    return osal_ret;
/* PRQA S 9107,9110 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqTimedSend()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_035]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_007
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
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
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
        mq->spin_key = k_spin_lock(&mq->spin_lock);
        if (0U == mq->numofmsg)
        {
            /* No wait and return the busy error immediately. */
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            /* Do nothing */
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_mq_receive(mq, p_buffer, (size_t)mq->config.msg_size);
        }
        else
        {
            /* Do nothing */
        }

        /* Notify the waiting thread that the queue is available. */
        if ((OSAL_RETURN_OK == osal_ret) && (0U < mq->snd_wait_count) && ((mq->config.max_num_msg - 1U) <= mq->numofmsg))
        {
            k_spin_unlock(&mq->spin_lock, mq->spin_key);
            os_ret = sem_post(&mq->sync_semaphore);
            mq->spin_key = k_spin_lock(&mq->spin_lock);
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

        k_spin_unlock(&mq->spin_lock, mq->spin_key);
    }   /* end of if ((NULL == mq) || (NULL == p_buffer)) */

    return osal_ret;
/* PRQA S 9107,9110 2 # The termination function has already multiple sub-functions to reduce complexity.
   We have decided that further division is not desirable from perspective of maintainability and readability. */
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqReceive()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_036]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_008
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
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    int             sem_count = 0;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
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
        abstime.tv_nsec = (long)p_time_stamp->tv_nsec;/* PRQA S 5209 # System is requesting an long, so it cannot be changed. */

        mq->spin_key = k_spin_lock(&mq->spin_lock);

        /* Wait for the arival of the message. */
        while ((0U == mq->numofmsg) && (OSAL_RETURN_OK == osal_ret))
        {
            /* If the message queue is empty, wait for the semaphore signaled by the other task or ISR. */
            /* PRQA S 2982 1 # This flag is referenced in other thread. No problem. */
            mq->rcv_wait_count++;
            k_spin_unlock(&mq->spin_lock, mq->spin_key);
            os_ret = sem_timedwait(&mq->sync_semaphore, (struct timespec*)&abstime);
            mq->spin_key = k_spin_lock(&mq->spin_lock);
            mq->rcv_wait_count--;
            if (0 != os_ret)
            {
                /* PRQA S 5209 1 # tv_nsec is declared by the OS code and the following check has no side-effect. */
                if ((abstime.tv_sec >= (time_t)0) && (abstime.tv_nsec < (long)NSEC_PER_SEC))
                {
                    osal_ret = OSAL_RETURN_TIME;
                }
                else
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
            }
            else
            {
                /* do nothing */
            }

            os_ret = sem_getvalue(&mq->sync_semaphore, &sem_count);
            if (0 == os_ret)
            {
                while (0 < sem_count)
                {
                    os_ret = sem_trywait(&mq->sync_semaphore);
                    if (0 != os_ret)
                    {
                        /* sem_trywait error do nothing */
                    }
                    else
                    {
                        /* do nothing */
                    }
                    sem_count--;
                }
            }
            else
            {
                /* do nothing */
            }
        }   /* end of while((0U == mq->numofmsg) && (OSAL_RETURN_OK == osal_ret)). */

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_os_mq_receive(mq, p_buffer, (size_t)mq->config.msg_size);
        }
        else
        {
            /* Do nothing */
        }

        /* Notify the waiting thread that the queue is available. */
        if ((OSAL_RETURN_OK == osal_ret) && (0U < mq->snd_wait_count) && ((mq->config.max_num_msg - 1U) <= mq->numofmsg))
        {
            /* Unlock the spin lock before calling sem_post so that the context switching is performed. */
            /* Re-lock the spin lock after calling sem_post for the exclusion control of this context. */
            k_spin_unlock(&mq->spin_lock, mq->spin_key);
            os_ret = sem_post(&mq->sync_semaphore);
            k_yield();
            mq->spin_key = k_spin_lock(&mq->spin_lock);
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

        k_spin_unlock(&mq->spin_lock, mq->spin_key);
    }   /* end of if ((NULL == mq) || (NULL == p_time_stamp) || (NULL == p_buffer)) */

    return osal_ret;
/* PRQA S 9107,9110 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqTimedReceive()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_009
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
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
/*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == mq) || (NULL == p_result))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        mq->spin_key = k_spin_lock(&mq->spin_lock);
        if (0U == mq->numofmsg)
        {
            *p_result = true;
        }
        else    /* 0 != mq->numofmsg */
        {
            *p_result = false;
        }

        k_spin_unlock(&mq->spin_lock, mq->spin_key);
    }   /* end of if (NULL == mq) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqIsEmpty()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_038]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_010
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
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == mq) || (NULL == p_result))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        mq->spin_key = k_spin_lock(&mq->spin_lock);
        if (mq->config.max_num_msg == mq->numofmsg)
        {
            *p_result = true;
        }
        else    /* mq->config.max_num_msg != mq->numofmsg */
        {
            *p_result = false;
        }

        k_spin_unlock(&mq->spin_lock, mq->spin_key);
    }   /* end of if (NULL == mq) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqIsFull()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqReset()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqReset(osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          buffer_count;
    int             os_ret;/* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    uint32_t        send_count;
    bool            do_k_yield = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mq)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        mq->spin_key = k_spin_lock(&mq->spin_lock);

        /* Unblock threads waiting for message. */
        send_count = mq->snd_wait_count;
        while ((0U < send_count) && (OSAL_RETURN_OK == osal_ret))
        {
            os_ret = sem_post(&mq->sync_semaphore);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                do_k_yield = true;
            }
            send_count--;
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

        /* Reset message queues. */
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
                            (size_t)mq->config.msg_size);
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

        k_spin_unlock(&mq->spin_lock, mq->spin_key);

        /* Call k_yield to switch to the waiting thread. */
        if(true == do_k_yield)
        {
            k_yield();
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if ((NULL == mq) || (NULL == p_result)) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqReset()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MqCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqCheckBusy(osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mq)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        mq->spin_key = k_spin_lock(&mq->spin_lock);
        if ((0U != mq->numofmsg) || (0U < mq->snd_wait_count) || (0U < mq->rcv_wait_count))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            /* Do nothing */
        }

        k_spin_unlock(&mq->spin_lock, mq->spin_key);
    }   /* end of if (NULL == mq) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MqCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_041]
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_osal_os_mq_alloc_queue_buff()
***********************************************************************************************************************/
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
    /* Allocate the message queue. */
    /* PRQA S 2706 2 # There is no problem because it will be released at R_OSAL_OS_MqDelete(). */
    /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
    mq->queue = (st_osal_queue_t*)calloc(1, (sizeof(st_osal_queue_t) * (size_t)p_config->max_num_msg));
    if (NULL == mq->queue)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* Allocate the buffer for each message queue. */
        count = 0;
        while ((p_config->max_num_msg > count) && (OSAL_RETURN_OK == osal_ret))
        {
            /* PRQA S 2706 2 # There is no problem because it will be released at R_OSAL_OS_MqDelete(). */
            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            mq->queue[count].p_buffer = calloc(1, (size_t)p_config->msg_size);
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
* Start of function r_osal_os_mq_undo_create()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_mq_undo_create(const st_osal_mq_config_t* const p_config,
                                          osal_os_mq_t mq)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
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
        /* Deallocate the buffer of each message queue. */
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
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_013
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
        /* PRQA S 1496 1 # The input and output buffer sizes are guaranteed to be the same in this case. */
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
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_MSG_014
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
        /* PRQA S 1496 1 # The input and output buffer sizes are guaranteed to be the same in this case. */
        (void)memcpy(p_buffer, mq->queue[mq->head].p_buffer, buffer_size);
        (void)memset(mq->queue[mq->head].p_buffer, 0x00, (size_t)mq->config.msg_size);

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
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_MSG_018]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
