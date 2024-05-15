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
* File Name :    r_osal_os_mutex.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Mutex Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <pthread.h>
#include <stdlib.h>

#include "target/qnx/r_osal_os_type.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexCreate()
***********************************************************************************************************************/
/* PRQA S 3673 2 # Since the pointer type is typedefed, const cannot be added */
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexCreate(const osal_os_mutexattr_t mutex_attr,
                                      osal_os_mutex_t * p_mutex)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    int                 os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    st_osal_os_mutex_t  *p_tmp_mutex;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == mutex_attr) || (NULL == p_mutex))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_MutexDestroy(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_tmp_mutex = (st_osal_os_mutex_t*)calloc(1, sizeof(st_osal_os_mutex_t));
        if (NULL == p_tmp_mutex)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* PRQA S 2736 1 # There is no problem because it will be released at R_OSAL_OS_MutexDestroy(). */
            os_ret = pthread_mutex_init(&p_tmp_mutex->mutex, &mutex_attr->attr);
            if (EOK != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                free((void*)p_tmp_mutex);
            }
            else
            {
                *p_mutex = p_tmp_mutex;
            }
        }
    }   /* end of if ((NULL == mutex_attr) || (NULL == p_mutex)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexDestroy()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexDestroy(osal_os_mutex_t mutex)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mutex)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_mutex_destroy(&mutex->mutex);
        if (EBUSY == os_ret)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 5118 2 # Memory deallocation functions are available during deinitialization. */
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            free((void*)mutex);
        }
    }   /* end of if (NULL == mutex). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexDestroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexTimedLock()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MutexTimedLock(osal_os_mutex_t mutex,
                                         const st_osal_time_t* const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec abs_timeout;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == mutex) || (NULL == p_time_stamp))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        abs_timeout.tv_sec  = p_time_stamp->tv_sec;
        /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
        abs_timeout.tv_nsec = (long)p_time_stamp->tv_nsec;

        os_ret = pthread_mutex_timedlock_monotonic(&mutex->mutex, &abs_timeout);
        if (ETIMEDOUT == os_ret)
        {
            osal_ret = OSAL_RETURN_TIME;
        }
        else if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* Do nothing */
        }
    }   /* end of if ((NULL == mutex) || (NULL == p_time_stamp)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexTimedLock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_043]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexTryLock()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexTryLock(osal_os_mutex_t mutex)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mutex)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_mutex_trylock(&mutex->mutex);
        if ((EBUSY == os_ret) || (EAGAIN == os_ret))
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else if (EOK != os_ret)
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
* End of function R_OSAL_OS_MutexTryLock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_041]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexUnlock()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexUnlock(osal_os_mutex_t mutex)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mutex)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_mutex_unlock(&mutex->mutex);
        if (EOK != os_ret)
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
* End of function R_OSAL_OS_MutexUnlock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_042]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexInitAttr()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexInitAttr(osal_os_mutexattr_t * p_mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    int                     os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    st_osal_os_mutexattr_t  *p_tmp_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_mutex_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_MutexDeinitAttr(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_tmp_attr = (st_osal_os_mutexattr_t*)calloc(1, sizeof(st_osal_os_mutexattr_t));
        if (NULL == p_tmp_attr)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            os_ret = pthread_mutexattr_init(&p_tmp_attr->attr);
            if (EOK != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                free((void*)p_tmp_attr);
            }
            else
            {
                os_ret = pthread_mutexattr_settype(&p_tmp_attr->attr, PTHREAD_MUTEX_ERRORCHECK);
                if (EOK != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                    (void)pthread_mutexattr_destroy(&p_tmp_attr->attr);
                    /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
                    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                    free((void*)p_tmp_attr);
                }
                else
                {
                    *p_mutex_attr = p_tmp_attr;
                }
            }   /* end of if (EOK != os_ret). */
        }   /* end of if (NULL == p_tmp_attr). */
    }   /* end of if (NULL == p_mutex_attr). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexInitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MutexDeinitAttr()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_MutexDeinitAttr(osal_os_mutexattr_t mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mutex_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_mutexattr_destroy(&mutex_attr->attr);
        if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            free((void*)mutex_attr);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MutexDeinitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_038]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
