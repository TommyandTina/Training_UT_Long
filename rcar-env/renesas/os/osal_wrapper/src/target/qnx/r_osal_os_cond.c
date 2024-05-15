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
* Description :  Condition Related OS IFs
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
 * XOS3_OSAL_QNX_CD_CD_THS_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondStaticCreate()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Since the pointer type is typedefed, const cannot be added. */
e_osal_return_t R_OSAL_OS_CondStaticCreate(const osal_os_condattr_t cond_attr,
                                           osal_os_cond_t * p_cond)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    int                 os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    st_osal_os_cond_t   *p_tmp_cond;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == cond_attr) || (NULL == p_cond))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_CondDestroy(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_tmp_cond = (st_osal_os_cond_t*)calloc(1, sizeof(st_osal_os_cond_t));
        if (NULL == p_tmp_cond)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            os_ret = pthread_cond_init(&p_tmp_cond->cond, &cond_attr->attr);
            if (EOK != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                free((void*)p_tmp_cond);
            }
            else
            {
                *p_cond = p_tmp_cond;
            }
        }
    }   /* end of if ((NULL == cond_attr) || (NULL == p_cond)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondStaticCreate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_010
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondDestroy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondDestroy(osal_os_cond_t cond)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == cond)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_cond_destroy(&cond->cond);
        if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 5118 2 # Memory deallocation functions are available during deinitialization. */
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            free((void*)cond);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondDestroy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_011
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondTimedWait()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondTimedWait(osal_os_cond_t cond,
                                        osal_os_mutex_t mutex,
                                        const st_osal_time_t* const p_time_stamp)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    struct timespec abstime;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == cond) || (NULL == mutex))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (NULL != p_time_stamp)
        {
            abstime.tv_sec  = p_time_stamp->tv_sec;
            /* PRQA S 5209 1 # System is requesting a long, so it cannot be changed. */
            abstime.tv_nsec = (long)p_time_stamp->tv_nsec;
            os_ret = pthread_cond_timedwait(&cond->cond, &mutex->mutex, &abstime);
        }
        else
        {
            os_ret = pthread_cond_wait(&cond->cond, &mutex->mutex);
        }

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
    }   /* end of if ((NULL == cond) || (NULL == mutex)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondTimedWait()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_031]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondSignal()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondSignal(osal_os_cond_t cond)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == cond)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_cond_signal(&cond->cond);
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
* End of function R_OSAL_OS_CondSignal()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_032]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondBroadcast()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_OS_CondBroadcast(osal_os_cond_t cond)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == cond)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_cond_broadcast(&cond->cond);
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
* End of function R_OSAL_OS_CondBroadcast()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_033]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_014
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondInitAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondInitAttr(osal_os_condattr_t * p_cond_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    int                     os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    st_osal_os_condattr_t   *p_tmp_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_cond_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_CondDeinitAttr(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_tmp_attr = (st_osal_os_condattr_t*)calloc(1, sizeof(st_osal_os_condattr_t));
        if (NULL == p_tmp_attr)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            os_ret = pthread_condattr_init(&p_tmp_attr->attr);
            if (EOK != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
                /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                free((void*)p_tmp_attr);
            }
            else
            {
                os_ret = pthread_condattr_setclock(&p_tmp_attr->attr, CLOCK_MONOTONIC);
                if (EOK != os_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                    (void)pthread_condattr_destroy(&p_tmp_attr->attr);
                    /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
                    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                    free((void*)p_tmp_attr);
                }
                else
                {
                    *p_cond_attr = p_tmp_attr;
                }
            }   /* end of if (EOK != os_ret). */
        }   /* end of if (NULL == p_tmp_attr). */
    }   /* end of if (NULL == p_cond_attr). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondInitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_THS_015
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_CondDeinitAttr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondDeinitAttr(osal_os_condattr_t cond_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == cond_attr)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = pthread_condattr_destroy(&cond_attr->attr);
        if (EOK != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            /* PRQA S 5118 2 # Memory deallocation functions are available during initialization. */
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            free((void*)cond_attr);
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_CondDeinitAttr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_THS_035]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
