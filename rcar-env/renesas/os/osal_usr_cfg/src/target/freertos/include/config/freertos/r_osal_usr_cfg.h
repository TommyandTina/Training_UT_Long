/*************************************************************************************************************
* OSAL Resource Configurator
* Copyright (c) [2021-2022] Renesas Electronics Corporation
* This software is released under the MIT License.
* http://opensource.org/licenses/mit-license.php
*************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_usr_cfg.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL resource configuration file
***********************************************************************************************************************/

#ifndef OSAL_USR_CFG_H
#define OSAL_USR_CFG_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal_usr_cfg/freertos/r_osal_usr_cfg_info.h"
#include "rcar-xos/osal_usr_cfg/freertos/r_osal_usr_cfg_control.h"


/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_FREERTOS_USR_CFG
 * @defgroup OSAL_Private_Defines_UsrConfig User Configurator Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_RCFG_INNER_TIMEOUT
 * osal internal timeout value.
***********************************************************************************************************************/
#define OSAL_RCFG_INNER_TIMEOUT         (500L)

/*******************************************************************************************************************//**
 * @def OSAL_COND_TIMEOUT
 * Timeout value when relocking mutex with CondWait.
***********************************************************************************************************************/
#define OSAL_COND_TIMEOUT (600000L)

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_FREERTOS_USR_CFG
 * @defgroup OSAL_Private_Defines_Static_Variables_UsrConfig User Condifurator Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_thread_control
 * osal user configuration : thread control structure
***********************************************************************************************************************/
static st_osal_thread_control_t gs_osal_thread_control[OSAL_RCFG_THREAD_MAX_NUM] =
{
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0}
};

/*******************************************************************************************************************//**
 * @var gs_osal_mutex_control
 * osal user configuration : mutex control structure
***********************************************************************************************************************/
static st_osal_mutex_control_t gs_osal_mutex_control[OSAL_RCFG_MUTEX_MAX_NUM] =
{
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0}
};

/*******************************************************************************************************************//**
 * @var gs_osal_cond_control
 * osal user configuration : cond control structure
***********************************************************************************************************************/
static st_osal_cond_control_t gs_osal_cond_control[OSAL_RCFG_COND_MAX_NUM] =
{
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0}
};

/*******************************************************************************************************************//**
 * @var gs_osal_mq_control
 * osal user configuration : message queue control structure
***********************************************************************************************************************/
static st_osal_mq_control_t gs_osal_mq_control[OSAL_RCFG_MQ_MAX_NUM] =
{
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0}
};

/*******************************************************************************************************************//**
 * @var gs_osal_interrupt_thread_control
 * osal user configuration : interrupt thread control structure
***********************************************************************************************************************/
static st_osal_interrupt_control_t gs_osal_interrupt_thread_control[OSAL_RCFG_INTERRUPT_MAX_NUM] =
{
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0},
    {.handle_id = 0}
};

/** @} */

#endif /* OSAL_USR_CFG_H */