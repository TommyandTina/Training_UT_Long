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
* File Name :    r_osal_usr_cfg.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL resource configuration interface
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#include "target/emcos/usr_cfg/r_osal_usr_cfg.h"


/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Private_Defines_Static_Variables_DevConfig Device Condifurator Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gsp_osal_thread_config
 * osal resource configuration : thread resource pointer
***********************************************************************************************************************/
static const st_osal_rcfg_thread_config_t      *gsp_osal_thread_config;

/*******************************************************************************************************************//**
 * @var gsp_osal_mutex_config
 * osal resource configuration : mutex resource pointer
***********************************************************************************************************************/
static const st_osal_rcfg_mutex_config_t       *gsp_osal_mutex_config;

/*******************************************************************************************************************//**
 * @var gsp_osal_cond_config
 * osal resource configuration : condition resource pointer
***********************************************************************************************************************/
static const st_osal_rcfg_cond_config_t        *gsp_osal_cond_config;

/*******************************************************************************************************************//**
 * @var gsp_osal_mq_config
 * osal resource configuration : message resource pointer
***********************************************************************************************************************/
static const st_osal_mq_rcfg_config_t          *gsp_osal_mq_config;

/*******************************************************************************************************************//**
 * @var gsp_osal_interrupt_thread_config
 * osal resource configuration : interrupt thread resource pointer
***********************************************************************************************************************/
static const st_osal_interrupt_thread_config_t *gsp_osal_interrupt_thread_config;

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_Init()
* XOS3_OSAL_EMCOS_CD_CD_NON_006
* [Covers: XOS3_OSAL_EMCOS_UD_DD_NON_006]
***********************************************************************************************************************/
void R_OSAL_RCFG_Init(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gsp_osal_thread_config              = g_osal_thread_config;
    gsp_osal_mutex_config               = g_osal_mutex_config;
    gsp_osal_cond_config                = g_osal_cond_config;
    gsp_osal_mq_config                  = g_osal_mq_config;
    gsp_osal_interrupt_thread_config    = g_osal_interrupt_thread_config;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_Deinit()
***********************************************************************************************************************/
void R_OSAL_RCFG_Deinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gsp_osal_thread_config              = NULL;
    gsp_osal_mutex_config               = NULL;
    gsp_osal_cond_config                = NULL;
    gsp_osal_mq_config                  = NULL;
    gsp_osal_interrupt_thread_config    = NULL;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_Deinit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfThread()
* XOS3_OSAL_EMCOS_CD_CD_TH_013
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_021]
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfThread(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;
    size_t count;
    bool   last_data = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (count = 0;
         (((sizeof(gs_osal_thread_control) / sizeof(st_osal_thread_control_t)) > count) && (false == last_data));
         count++)
    {
        if((uint64_t)OSAL_THREAD_INVALID_ID == gsp_osal_thread_config[count].id)
        {
            last_data = true;
            num = count;
        }
        else
        {
            /* Do nothing */
        }
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfThread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfMutex()
* XOS3_OSAL_EMCOS_CD_CD_THS_024
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_030]
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfMutex(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;
    size_t count;
    bool   last_data = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (count = 0;
         (((sizeof(gs_osal_mutex_control) / sizeof(st_osal_mutex_control_t)) > count) && (false == last_data));
         count++)
    {
        if((uint64_t)OSAL_MUTEX_INVALID_ID == gsp_osal_mutex_config[count].id)
        {
            last_data = true;
            num = count;
        }
        else
        {
            /* Do nothing */
        }
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfMutex()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfCond()
* XOS3_OSAL_EMCOS_CD_CD_THS_025
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_029]
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfCond(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;
    size_t count;
    bool   last_data = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (count = 0;
         (((sizeof(gs_osal_cond_control) / sizeof(st_osal_cond_control_t)) > count) && (false == last_data));
         count++)
    {
        if((uint64_t)OSAL_COND_INVALID_ID == gsp_osal_cond_config[count].id)
        {
            last_data = true;
            num = count;
        }
        else
        {
            /* Do nothing */
        }
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfCond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfMq()
* XOS3_OSAL_EMCOS_CD_CD_MSG_020
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_035]
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfMq(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;
    size_t count;
    bool   last_data = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (count = 0;
         (((sizeof(gs_osal_cond_control) / sizeof(st_osal_cond_control_t)) > count) && (false == last_data));
         count++)
    {
        if((uint64_t)OSAL_MQ_INVALID_ID == gsp_osal_mq_config[count].id)
        {
            last_data = true;
            num = count;
        }
        else
        {
            /* Do nothing */
        }
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfMq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfInterruptThread()
* XOS3_OSAL_EMCOS_CD_CD_INT_016
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_022]
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfInterruptThread(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;
    size_t count;
    bool   last_data = false;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (count = 0;
         (((sizeof(gs_osal_interrupt_thread_control) / sizeof(st_osal_interrupt_thread_control_t)) > count) && (false == last_data));
         count++)
    {
        if(OSAL_INTERRUPT_INVALID_ID == gsp_osal_interrupt_thread_config[count].id)
        {
            last_data = true;
            num = count;
        }
        else
        {
            /* Do nothing */
        }
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfInterruptThread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetThreadInfo()
* XOS3_OSAL_EMCOS_CD_CD_TH_014
* [Covers: XOS3_OSAL_EMCOS_UD_DD_TH_022]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetThreadInfo(uint32_t num,
                                          st_osal_thread_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool   last_data         = false;
    size_t count;
    size_t num_of_config     = 0;
    size_t num_of_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        num_of_control = (sizeof(gs_osal_thread_control) / sizeof(st_osal_thread_control_t));

        for (count = 0;
             (num_of_control > count) && (false == last_data);
             count++)
        {
            if((uint64_t)OSAL_THREAD_INVALID_ID == gsp_osal_thread_config[count].id)
            {
                last_data = true;
                num_of_config = count;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (num < num_of_config)
        {
            gs_osal_thread_control[num].usr_config    = &gsp_osal_thread_config[num];
            gs_osal_thread_control[num].inner_timeout = (osal_milli_sec_t)OSAL_USR_CFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_thread_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }   /* end of if(NULL == pp_info). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetThreadInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetMutexInfo()
* XOS3_OSAL_EMCOS_CD_CD_THS_026
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_032]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetMutexInfo(uint32_t num,
                                         st_osal_mutex_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool   last_data         = false;
    size_t count;
    size_t num_of_config     = 0;
    size_t num_of_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        num_of_control = (sizeof(gs_osal_mutex_control) / sizeof(st_osal_mutex_control_t));

        for (count = 0;
             (num_of_control > count) && (false == last_data);
             count++)
        {
            if((uint64_t)OSAL_MUTEX_INVALID_ID == gsp_osal_mutex_config[count].id)
            {
                last_data = true;
                num_of_config = count;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (num < num_of_config)
        {
            gs_osal_mutex_control[num].usr_config    = &gsp_osal_mutex_config[num];
            gs_osal_mutex_control[num].inner_timeout = (osal_milli_sec_t)OSAL_USR_CFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_mutex_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }   /* end of if(NULL == pp_info). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetMutexInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetCondInfo()
* XOS3_OSAL_EMCOS_CD_CD_THS_027
* [Covers: XOS3_OSAL_EMCOS_UD_DD_THS_031]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetCondInfo(uint32_t num,
                                        st_osal_cond_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool   last_data         = false;
    size_t count;
    size_t num_of_config     = 0;
    size_t num_of_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        num_of_control = (sizeof(gs_osal_cond_control) / sizeof(st_osal_cond_control_t));

        for (count = 0;
             (num_of_control > count) && (false == last_data);
             count++)
        {
            if((uint64_t)OSAL_COND_INVALID_ID == gsp_osal_cond_config[count].id)
            {
                last_data = true;
                num_of_config = count;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (num < num_of_config)
        {
            gs_osal_cond_control[num].usr_config    = &gsp_osal_cond_config[num];
            gs_osal_cond_control[num].inner_timeout = (osal_milli_sec_t)OSAL_USR_CFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_cond_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }   /* end of if(NULL == pp_info). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetCondInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetMqInfo()
* XOS3_OSAL_EMCOS_CD_CD_MSG_021
* [Covers: XOS3_OSAL_EMCOS_UD_DD_MSG_036]
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetMqInfo(uint32_t num,
                                      st_osal_mq_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool   last_data         = false;
    size_t count;
    size_t num_of_config     = 0;
    size_t num_of_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        num_of_control = (sizeof(gs_osal_mq_control) / sizeof(st_osal_mq_control_t));

        for (count = 0;
             (num_of_control > count) && (false == last_data);
             count++)
        {
            if((uint64_t)OSAL_MQ_INVALID_ID == gsp_osal_mq_config[count].id)
            {
                last_data = true;
                num_of_config = count;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (num < num_of_config)
        {
            gs_osal_mq_control[num].usr_config    = &gsp_osal_mq_config[num];
            gs_osal_mq_control[num].inner_timeout = (osal_milli_sec_t)OSAL_USR_CFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_mq_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }   /* end of if(NULL == pp_info). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetMqInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetInterruptThreadInfo()
* XOS3_OSAL_EMCOS_CD_CD_INT_017
* [Covers: XOS3_OSAL_EMCOS_UD_DD_INT_023]
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_return_t R_OSAL_RCFG_GetInterruptThreadInfo(uint32_t num,
                                                   st_osal_interrupt_thread_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool   last_data         = false;
    size_t count;
    size_t num_of_config     = 0;
    size_t num_of_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        num_of_control = (sizeof(gs_osal_interrupt_thread_control) / sizeof(st_osal_interrupt_thread_control_t));

        for (count = 0;
             (num_of_control > count) && (false == last_data);
             count++)
        {
            if(OSAL_INTERRUPT_INVALID_ID == gsp_osal_interrupt_thread_config[count].id)
            {
                last_data = true;
                num_of_config = count;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (num < num_of_config)
        {
            gs_osal_interrupt_thread_control[num].usr_config    = &gsp_osal_interrupt_thread_config[num];
            gs_osal_interrupt_thread_control[num].inner_timeout = (osal_milli_sec_t)OSAL_USR_CFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_interrupt_thread_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }   /* end of if(NULL == pp_info). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetInterruptThreadInfo()
***********************************************************************************************************************/

