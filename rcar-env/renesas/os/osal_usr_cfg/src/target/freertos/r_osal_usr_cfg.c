/*************************************************************************************************************
* OSAL Resource Configurator
* Copyright (c) [2021-2022] Renesas Electronics Corporation
* This software is released under the MIT License.
* http://opensource.org/licenses/mit-license.php
*************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_usr_cfg.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL resource configuration interface
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#include "config/freertos/r_osal_usr_cfg.h"
#include "config/freertos/r_osal_resource_info.h"

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfThread()
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfThread(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num = (sizeof(gs_osal_thread_config) / sizeof(st_osal_rcfg_thread_config_t));

    if (num > (sizeof(gs_osal_thread_control) / sizeof(st_osal_thread_control_t)))
    {
        num = 0;
    }
    else if ( OSAL_THREAD_INVALID_ID == gs_osal_thread_config[num -1].id)
    {
        num = num -1;
    }
    else
    {
        /* Do nothing */
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfThread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfMutex()
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfMutex(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num = sizeof(gs_osal_mutex_config) / sizeof(st_osal_rcfg_mutex_config_t);

    if (num > sizeof(gs_osal_mutex_control) / sizeof(st_osal_mutex_control_t))
    {
        num = 0;
    }
    else if (OSAL_MUTEX_INVALID_ID == gs_osal_mutex_config[num -1].id)
    {
        num = num -1;
    }
    else
    {
        /* Do nothing */
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfMutex()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfCond()
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfCond(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num = sizeof(gs_osal_cond_config) / sizeof(st_osal_rcfg_cond_config_t);

    if (num > sizeof(gs_osal_cond_control) / sizeof(st_osal_cond_control_t))
    {
        num = 0;
    }
    else if (OSAL_COND_INVALID_ID == gs_osal_cond_config[num -1].id)
    {
        num = num -1;
    }
    else
    {
        /* Do nothing */
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfCond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfMq()
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfMq(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num = sizeof(gs_osal_mq_config) / sizeof(st_osal_mq_rcfg_config_t);

    if (num > sizeof(gs_osal_mq_control) / sizeof(st_osal_mq_control_t))
    {
        num = 0;
    }
    else if (OSAL_MQ_INVALID_ID == gs_osal_mq_config[num -1].id)
    {
        num = num -1;
    }
    else
    {
        /* Do nothing */
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfMq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetNumOfInterruptThread()
***********************************************************************************************************************/
size_t R_OSAL_RCFG_GetNumOfInterruptThread(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    size_t num       = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    num = ((sizeof(gs_osal_interrupt_thread_config) / sizeof(st_osal_interrupt_config_t)) - 1);

    if (num > sizeof(gs_osal_thread_control) / sizeof(st_osal_thread_control_t))
    {
        num = 0;
    }
    else if (OSAL_THREAD_INVALID_ID == gs_osal_thread_config[num -1].id)
    {
        num = num -1;
    }
    else
    {
        /* Do nothing */
    }

    return num;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetNumOfInterruptThread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetThreadInfo()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetThreadInfo(uint32_t num,
                                          st_osal_thread_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t num_of_config     = 0;
    size_t num_of_control    = 0;

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

        num_of_config = sizeof(gs_osal_thread_config) / sizeof(st_osal_rcfg_thread_config_t);

        if ((num < num_of_config) && (num < num_of_control))
        {
            gs_osal_thread_control[num].usr_config = &gs_osal_thread_config[num];
            gs_osal_thread_control[num].inner_timeout = OSAL_RCFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_thread_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetThreadInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetMutexInfo()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetMutexInfo(uint32_t num,
                                         st_osal_mutex_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t num_of_config     = 0;
    size_t num_of_control    = 0;

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

        num_of_config = sizeof(gs_osal_mutex_config) / sizeof(st_osal_rcfg_mutex_config_t);

        if ((num < num_of_config) && (num < num_of_control))
        {
            gs_osal_mutex_control[num].usr_config = &gs_osal_mutex_config[num];
            gs_osal_mutex_control[num].inner_timeout = OSAL_RCFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_mutex_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetMutexInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetCondInfo()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetCondInfo(uint32_t num,
                                        st_osal_cond_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t num_of_config     = 0;
    size_t num_of_control    = 0;

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

        num_of_config = sizeof(gs_osal_cond_config) / sizeof(st_osal_rcfg_cond_config_t);

        if ((num < num_of_config) && (num < num_of_control))
        {
            gs_osal_cond_control[num].usr_config = &gs_osal_cond_config[num];
            gs_osal_cond_control[num].inner_timeout = OSAL_RCFG_INNER_TIMEOUT;
            gs_osal_cond_control[num].inner_cond_timeout = OSAL_COND_TIMEOUT;
            *pp_info = &gs_osal_cond_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetMutexInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetMqInfo()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetMqInfo(uint32_t num,
                                      st_osal_mq_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t num_of_config     = 0;
    size_t num_of_control    = 0;

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

        num_of_config = sizeof(gs_osal_mq_config) / sizeof(st_osal_mq_rcfg_config_t);

        if ((num < num_of_config) && (num < num_of_control))
        {
            gs_osal_mq_control[num].usr_config = &gs_osal_mq_config[num];
            gs_osal_mq_control[num].inner_timeout = OSAL_RCFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_mq_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetMqInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_OSAL_RCFG_GetInterruptThreadInfo()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_RCFG_GetInterruptThreadInfo(uint32_t num,
                                                   st_osal_interrupt_control_t **pp_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t num_of_config     = 0;
    size_t num_of_control    = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == pp_info)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        num_of_control = (sizeof(gs_osal_interrupt_thread_control) / sizeof(st_osal_interrupt_control_t));

        num_of_config = sizeof(gs_osal_interrupt_thread_config) / sizeof(st_osal_interrupt_config_t);

        if ((num < num_of_config) && (num < num_of_control))
        {
            gs_osal_interrupt_thread_control[num].usr_config = &gs_osal_interrupt_thread_config[num];
            gs_osal_interrupt_thread_control[num].inner_timeout = OSAL_RCFG_INNER_TIMEOUT;
            *pp_info = &gs_osal_interrupt_thread_control[num];
        }
        else
        {
            osal_ret = OSAL_RETURN_PAR;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_RCFG_GetInterruptThreadInfo()
***********************************************************************************************************************/

