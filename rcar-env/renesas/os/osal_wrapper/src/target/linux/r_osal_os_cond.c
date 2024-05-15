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
* File Name :    r_osal_os_cond.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for Condition variable
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <errno.h>
#include <pthread.h>
#include "target/linux/r_osal_os_types.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_131
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondCreate()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondCreate(osal_os_cond_t* p_cond)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == p_cond)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 1 # Required cast on system, not modifiable. */
        *p_cond = (osal_os_cond_t)R_OSAL_OS_Malloc(sizeof(st_osal_os_cond_t));
        if (NULL == *p_cond)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_cond_init(&(*p_cond)->cond, NULL);
        if (0 != linux_ret)
        {
            /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
            R_OSAL_OS_Free((void *)*p_cond);
            *p_cond = NULL;
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_132
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondDestroy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondDestroy(osal_os_cond_t cond)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == cond)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_cond_destroy(&cond->cond);
        switch (linux_ret)
        {
            case 0 :
                break;

            case EBUSY :
                osal_ret = OSAL_RETURN_BUSY;
                break;

            default :
                osal_ret = OSAL_RETURN_FAIL;
                break;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
        R_OSAL_OS_Free((void *)cond);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondDestroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_133
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondTimedWait()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_CondTimedWait(osal_os_cond_t cond, osal_os_mutex_t mutex, const st_osal_time_t* const p_time_stamp)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    struct timespec abstime;

    if ((NULL == cond) ||
        (NULL == mutex) ||
        (NULL == p_time_stamp))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        abstime.tv_sec  = p_time_stamp->tv_sec;
        abstime.tv_nsec = p_time_stamp->tv_nsec;

        linux_ret = pthread_cond_timedwait(&cond->cond, &mutex->mutex, &abstime);
        switch (linux_ret)
        {
            case 0 :
                osal_ret = OSAL_RETURN_OK;
                break;

            case ETIMEDOUT :
                osal_ret = OSAL_RETURN_TIME;
                break;

            default :
                osal_ret = OSAL_RETURN_FAIL;
                break;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondTimedWait()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_134
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondSignal()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_CondSignal(osal_os_cond_t cond)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == cond)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_cond_signal(&cond->cond);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondSignal()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_THS_135
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondBroadcast()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondBroadcast(osal_os_cond_t cond)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == cond)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = pthread_cond_broadcast(&cond->cond);
        if (0 != linux_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondBroadcast()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_THS_034]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
