/***********************************************************************************************************************
* Copyright [2021-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      1.4.0
* Description  : Internal function for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include <string.h>
#include <unistd.h>

#include "target/common/pma/statemachine/r_pma_statemachine.h"
#include "target/common/pma/hw/r_pma_hwdepend.h"

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
#define R_PMA_SM_UINT64_MAX    (0xFFFFFFFFFFFFFFFFU) /* The maximum value of the uint64_t definition */
/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
static void R_PMA_SM_SetLowestPowerState(e_pma_hwa_id_t hwaId);
static e_result_t R_PMA_SM_ChangeStateToHP(st_statemachine_t * pStatemachine);
static e_result_t R_PMA_SM_ChangeStateToCG(st_statemachine_t * pStatemachine);
static e_result_t R_PMA_SM_ChangeStateToPG(st_statemachine_t * pStatemachine);

/*======================================================================================================================
Private global variables
======================================================================================================================*/
static st_statemachine_t g_pma_sm_info_tbl[R_PMA_HWA_ID_NUM];
/***********************************************************************************************************************
* Start of function R_PMA_SM_Init()
* XOS2_OSAL_QNX_CD_CD_POW_126
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_201]
***********************************************************************************************************************/
e_result_t R_PMA_SM_Init(e_pma_hwa_id_t maxHwaId)
{
    e_result_t    result = R_PMA_OK;
    st_hwa_t     *pHwa   = NULL;
    e_pma_reset_req_t rst_stat;
    e_pma_onoff_t clk_stat;
    e_pma_onoff_t pwr_stat = R_PMA_UNDEFINED_STATE;

    /* PRQA S 1296,4527 1 # This implicit casting is no problem, because they are non-negative enum and positive integer, and this defines of minimum id vary from SoCs. */
    for (e_pma_hwa_id_t id = 0 ; id < maxHwaId ; id++)
    {
        result = R_PMA_HW_DEP_GetHwaObject(id, &pHwa);
        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_GetResetState(id, &rst_stat);
        }

        if (NULL == pHwa)
        {
            result = R_PMA_NG;
        }
        
        if (R_PMA_OK == result)
        {
            bool is_enable;

            result = R_PMA_HW_DEP_IsEnableControlPowerClock(id, &is_enable);
            if ((is_enable == false) && (result == R_PMA_OK))
            {
                g_pma_sm_info_tbl[id].enReqStat = R_PMA_HWA_REQUESTED;
            }
            else
            {
                g_pma_sm_info_tbl[id].enReqStat = R_PMA_HWA_RELEASED;
            }
            g_pma_sm_info_tbl[id].handle    = NULL;
            g_pma_sm_info_tbl[id].enRstStat = rst_stat;
            g_pma_sm_info_tbl[id].refCnt    = 0;
            g_pma_sm_info_tbl[id].targetHwa = pHwa;
        }
        
        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_GetHwaState(id, &clk_stat, &pwr_stat);
        }
        
        if (R_PMA_OK == result)
        {
            if (R_PMA_STATE_ON == pwr_stat)
            {
                if (R_PMA_STATE_ON == clk_stat)
                {
                    g_pma_sm_info_tbl[id].enPwrPolicy      = R_PMA_HP_MODE;
                    g_pma_sm_info_tbl[id].enPrevStbMode    = R_PMA_STATE_ENABLED;
                    g_pma_sm_info_tbl[id].enLowestPwrState = R_PMA_STATE_ENABLED;
                }
                else
                {
                    g_pma_sm_info_tbl[id].enPwrPolicy      = R_PMA_CG_MODE;
                    g_pma_sm_info_tbl[id].enPrevStbMode    = R_PMA_STATE_CG;
                    g_pma_sm_info_tbl[id].enLowestPwrState = R_PMA_STATE_CG;
                }
            }
            else if (R_PMA_STATE_OFF == clk_stat)
            {
                g_pma_sm_info_tbl[id].enPwrPolicy      = R_PMA_PG_MODE;
                g_pma_sm_info_tbl[id].enPrevStbMode    = R_PMA_STATE_PG;
                g_pma_sm_info_tbl[id].enLowestPwrState = R_PMA_STATE_PG;
            }
            else
            {
                g_pma_sm_info_tbl[id].enPwrPolicy      = R_PMA_PG_MODE;
                g_pma_sm_info_tbl[id].enPrevStbMode    = R_PMA_STATE_UNKNOWN;
                g_pma_sm_info_tbl[id].enLowestPwrState = R_PMA_STATE_UNKNOWN;
            }   /* end of if (R_PMA_STATE_ON == pwr_stat) */
        }
        else if(R_PMA_BSP_ECPG == result)
        {
            g_pma_sm_info_tbl[id].enPrevStbMode    = R_PMA_STATE_UNKNOWN;
            g_pma_sm_info_tbl[id].enLowestPwrState = R_PMA_STATE_UNKNOWN;
            
            /** Unknown status */
            if (R_PMA_STATE_ON == pwr_stat)
            {
                g_pma_sm_info_tbl[id].enPwrPolicy = R_PMA_CG_MODE;
                
            }
            else
            {
                g_pma_sm_info_tbl[id].enPwrPolicy = R_PMA_PG_MODE;
            }
            result = R_PMA_OK;
        }
        else
        {
            /** do nothing */
        }   /* end of if (R_PMA_OK == result) */
    }   /* end of for (e_pma_hwa_id_t id = 0 ; id < maxHwaId ; id++) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_Quit()
* XOS3_OSAL_NOS_CD_CD_POW_119
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_202]
***********************************************************************************************************************/
void R_PMA_SM_Quit(void)
{
    for (int32_t id = 0 ; id < (int32_t)g_pma_max_hwa_id_num ; id++)
    {
        g_pma_sm_info_tbl[id].handle    = NULL;
        g_pma_sm_info_tbl[id].enReqStat = R_PMA_HWA_RELEASED;
        g_pma_sm_info_tbl[id].refCnt    = 0;
    }
    return;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_Quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_GetLockHwa()
* XOS2_OSAL_QNX_CD_CD_POW_133
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_203]
***********************************************************************************************************************/
e_result_t R_PMA_SM_GetLockHwa(st_pma_handle_t handle,
                               e_pma_hwa_id_t  hwaId)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId))
    {
        /** do nothing */
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }

    if (R_PMA_OK == result)
    {
        if (NULL == g_pma_sm_info_tbl[hwaId].handle)
        {
            g_pma_sm_info_tbl[hwaId].handle = handle;
        }
        else
        {
            result = R_PMA_ALREADY_IN_USE;
        }
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_GetLockHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_UnlockHwa()
* XOS2_OSAL_QNX_CD_CD_POW_134
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_204]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_UnlockHwa(st_pma_handle_t handle,
                              e_pma_hwa_id_t  hwaId)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId))
    {
        if (g_pma_sm_info_tbl[hwaId].handle == handle)
        {
            g_pma_sm_info_tbl[hwaId].handle = NULL;
        }
        else
        {
            result = R_PMA_INVALID_HANDLE;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_UnlockHwa()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_RequestHWA()
* XOS2_OSAL_QNX_CD_CD_POW_145
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_205]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_RequestHWA(st_pma_handle_t handle,
                               e_pma_hwa_id_t  hwaId)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId))
    {
        /* PRQA S 2991,2995 1 # QAC false indication (CCT change). */
        if (R_PMA_SM_UINT64_MAX > g_pma_sm_info_tbl[hwaId].refCnt)
        {
            if (g_pma_sm_info_tbl[hwaId].handle == handle)
            {
                g_pma_sm_info_tbl[hwaId].refCnt++;
            }
            else
            {
                result = R_PMA_INVALID_HANDLE;
            }
        }
        else
        {
            /* PRQA S 2880 1 # QAC false indication (CCT change). */
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }   /* end of if ((NULL != handle)... */
    
    if (R_PMA_OK == result)
    {
        if (R_PMA_STATE_ENABLED == g_pma_sm_info_tbl[hwaId].enPrevStbMode)
        {
            /** HWA state is already in HP mode */
            g_pma_sm_info_tbl[hwaId].enReqStat = R_PMA_HWA_REQUESTED;
        }
        else if ((R_PMA_STATE_PG == g_pma_sm_info_tbl[hwaId].enPrevStbMode)
                  || (R_PMA_STATE_CG == g_pma_sm_info_tbl[hwaId].enPrevStbMode)
                  || (R_PMA_STATE_UNKNOWN == g_pma_sm_info_tbl[hwaId].enPrevStbMode))
        {
            result = g_pma_sm_info_tbl[hwaId].targetHwa->noticeChangeToHP(hwaId, hwaId);
            if (R_PMA_OK == result)
            {
                g_pma_sm_info_tbl[hwaId].enPrevStbMode = R_PMA_STATE_ENABLED;
                g_pma_sm_info_tbl[hwaId].enReqStat = R_PMA_HWA_REQUESTED;
            }
            else
            {
                g_pma_sm_info_tbl[hwaId].refCnt--;
            }
        }
        else
        {
            result = R_PMA_NG;
        }   /* end of if (R_PMA_STATE_ENABLED == g_pma_sm_info_tbl[hwaId].enPrevStbMode) */
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_RequestHWA()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_ReleaseHWA()
* XOS2_OSAL_QNX_CD_CD_POW_146
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_206]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_ReleaseHWA(st_pma_handle_t handle,
                               e_pma_hwa_id_t  hwaId)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId))
    {
        if (g_pma_sm_info_tbl[hwaId].handle != handle)
        {
            result = R_PMA_INVALID_HANDLE;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    if ((R_PMA_OK == result)
        && (0U != g_pma_sm_info_tbl[hwaId].refCnt))
    {
        g_pma_sm_info_tbl[hwaId].refCnt--;
        if (0U == g_pma_sm_info_tbl[hwaId].refCnt)
        {
            switch (g_pma_sm_info_tbl[hwaId].enPwrPolicy)
            {
                case R_PMA_HP_MODE:
                    g_pma_sm_info_tbl[hwaId].enPrevStbMode = R_PMA_STATE_ENABLED;
                    break;
                    
                case R_PMA_PG_MODE:
                    result = g_pma_sm_info_tbl[hwaId].targetHwa->noticeChangeToPG(hwaId, hwaId);
                    if (R_PMA_OK == result)
                    {
                        g_pma_sm_info_tbl[hwaId].enPrevStbMode = R_PMA_STATE_PG;
                    }
                    break;
                    
                case R_PMA_CG_MODE:
                    result = g_pma_sm_info_tbl[hwaId].targetHwa->noticeChangeHPtoCG(hwaId, hwaId);
                    if (R_PMA_OK == result)
                    {
                        g_pma_sm_info_tbl[hwaId].enPrevStbMode = R_PMA_STATE_CG;
                    }
                    break;
                    
                default:
                    result = R_PMA_NG;
                    break;
            }   /* end of switch (g_pma_sm_info_tbl[hwaId].enPwrPolicy) */
            
            if (R_PMA_OK == result)
            {
                R_PMA_SM_SetLowestPowerState(hwaId);
                g_pma_sm_info_tbl[hwaId].enReqStat = R_PMA_HWA_RELEASED;
            }
            else
            {
                g_pma_sm_info_tbl[hwaId].refCnt++;
            }
        }   /* end of if (0U == g_pma_sm_info_tbl[hwaId].refCnt) */
    }   /* end of if ((R_PMA_OK == result)... */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_ReleaseHWA()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_SetLowestPowerState()
* XOS3_OSAL_NOS_CD_CD_POW_215 
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_215]
***********************************************************************************************************************/
static void R_PMA_SM_SetLowestPowerState(e_pma_hwa_id_t hwaId)
{
    e_result_t result;
    e_pma_onoff_t clk_stat = R_PMA_UNDEFINED_STATE;
    e_pma_onoff_t pwr_stat = R_PMA_UNDEFINED_STATE;

    result = R_PMA_HW_DEP_GetHwaState(hwaId, &clk_stat, &pwr_stat);

    if (R_PMA_OK == result)
    {
        if (R_PMA_STATE_ON == pwr_stat)
        {
            if (R_PMA_STATE_ON == clk_stat)
            {
                g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_ENABLED;
            }
            else
            {
                g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_CG;
            }
        }
        else if (R_PMA_STATE_OFF == clk_stat)
        {
            g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_PG;
        }
        else
        {
            g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_UNKNOWN;
        }
    }
    else if(R_PMA_BSP_ECPG == result)
    {
        g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_UNKNOWN;
    }
    else
    {
        /** do nothing */
    }   /* end of if (R_PMA_OK == result) */

    return;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_SetLowestPowerState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_ChangePolicyHWA()
* XOS2_OSAL_QNX_CD_CD_POW_151
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_207]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_ChangePolicyHWA(st_pma_handle_t handle,
                                    e_pma_hwa_id_t  hwaId,
                                    e_pma_power_policy_t policy)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId))
    {
        if (g_pma_sm_info_tbl[hwaId].handle != handle)
        {
            result = R_PMA_INVALID_HANDLE;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    if (R_PMA_OK == result)
    {
        if (R_PMA_HWA_RELEASED == g_pma_sm_info_tbl[hwaId].enReqStat)
        {
            switch (policy)
            {
                case R_PMA_PG_MODE:
                    result = R_PMA_SM_ChangeStateToPG(&g_pma_sm_info_tbl[hwaId]);
                    break;
                    
                case R_PMA_CG_MODE:
                    result = R_PMA_SM_ChangeStateToCG(&g_pma_sm_info_tbl[hwaId]);
                    break;
                    
                case R_PMA_HP_MODE:
                    result = R_PMA_SM_ChangeStateToHP(&g_pma_sm_info_tbl[hwaId]);
                    break;
                default:
                    result = R_PMA_NG;
                    break;
            }
        }
        else if (R_PMA_HWA_REQUESTED == g_pma_sm_info_tbl[hwaId].enReqStat)
        {
            /** do nothing */
        }
        else
        {
            result = R_PMA_NG;
        }   /* end of if (R_PMA_HWA_RELEASED == g_pma_sm_info_tbl[hwaId].enReqStat) */
    }   /* end of if (R_PMA_OK == result) */
    
    if (R_PMA_OK == result)
    {
        g_pma_sm_info_tbl[hwaId].enPwrPolicy = policy;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_ChangePolicyHWA()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_ApplyResetHWA()
* XOS2_OSAL_QNX_CD_CD_POW_147
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_223]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_ApplyResetHWA(st_pma_handle_t handle,
                                  e_pma_hwa_id_t  hwaId)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId))
    {
        if (g_pma_sm_info_tbl[hwaId].handle != handle)
        {
            result = R_PMA_INVALID_HANDLE;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }

    if (R_PMA_OK == result)
    {
        if (R_PMA_APPLIED_RESET != g_pma_sm_info_tbl[hwaId].enRstStat)
        {
            result = R_PMA_HW_DEP_ApplyReset(hwaId);
            if (R_PMA_OK == result)
            {
                g_pma_sm_info_tbl[hwaId].enRstStat = R_PMA_APPLIED_RESET;
            }
        }
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_ApplyResetHWA()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_ReleaseResetHWA()
* XOS2_OSAL_QNX_CD_CD_POW_148
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_224]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_ReleaseResetHWA(st_pma_handle_t handle,
                                    e_pma_hwa_id_t  hwaId)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId))
    {
        if (g_pma_sm_info_tbl[hwaId].handle != handle)
        {
            result = R_PMA_INVALID_HANDLE;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }

    if (R_PMA_OK == result)
    {
        if (R_PMA_RELEASED_RESET != g_pma_sm_info_tbl[hwaId].enRstStat)
        {
            result = R_PMA_HW_DEP_ReleaseReset(hwaId);
            if (R_PMA_OK == result)
            {
                g_pma_sm_info_tbl[hwaId].enRstStat = R_PMA_RELEASED_RESET;
            }
        }
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_ReleaseResetHWA()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_GetPowerPolicy()
* XOS2_OSAL_QNX_CD_CD_POW_138
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_208]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_GetPowerPolicy(st_pma_handle_t       handle,
                                   e_pma_hwa_id_t        hwaId,
                                   e_pma_power_policy_t *pPolicy)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId)
       && (NULL != pPolicy))
    {
        if (g_pma_sm_info_tbl[hwaId].handle != handle)
        {
            result = R_PMA_INVALID_HANDLE;
        }
        else
        {
            *pPolicy = g_pma_sm_info_tbl[hwaId].enPwrPolicy;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_GetPowerPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_GetHwaState()
* XOS2_OSAL_QNX_CD_CD_POW_135
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_209]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_GetHwaState(st_pma_handle_t     handle,
                                e_pma_hwa_id_t      hwaId,
                                st_pma_hwa_state_t *pHwaState)
{
    e_result_t    result = R_PMA_OK;
    e_pma_onoff_t clk_stat;
    e_pma_onoff_t pwr_stat;

    if ((NULL != handle)
         && (g_pma_max_hwa_id_num > hwaId)
         && (NULL != pHwaState))
    {
        if (g_pma_sm_info_tbl[hwaId].handle != handle)
        {
            result = R_PMA_INVALID_HANDLE;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    if (R_PMA_OK == result)
    {
        if (NULL != g_pma_sm_info_tbl[hwaId].targetHwa)
        {
            result = R_PMA_HW_DEP_GetHwaState(hwaId, &clk_stat, &pwr_stat);
            if (R_PMA_OK == result)
            {
                pHwaState->enActualPwrStat = pwr_stat;
                pHwaState->enActualClkStat = clk_stat;
                pHwaState->enReqState      = g_pma_sm_info_tbl[hwaId].enReqStat;
            }
            else if (R_PMA_BSP_ECPG == result)
            {
                /** eMCOS BSP driver cannot return CPG status. So PMA API returns only SYSC status. */
                pHwaState->enActualPwrStat = pwr_stat;
                pHwaState->enReqState      = g_pma_sm_info_tbl[hwaId].enReqStat;
            }
            else
            {
                /** do nothing */
            }
        }
        else
        {
            result = R_PMA_NG;
        }   /* end of if (NULL != g_pma_sm_info_tbl[hwaId].targetHwa) */
    }   /* end of if (R_PMA_OK == result) */
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_GetHwaState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_GetResetState()
* XOS2_OSAL_QNX_CD_CD_POW_136
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_210]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_GetResetState(st_pma_handle_t    handle,
                                  e_pma_hwa_id_t     hwaId,
                                  e_pma_reset_req_t *pRstState)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId)
       && (NULL != pRstState))
    {
        if (g_pma_sm_info_tbl[hwaId].handle != handle)
        {
            result = R_PMA_INVALID_HANDLE;
        }
        else
        {
            *pRstState = g_pma_sm_info_tbl[hwaId].enRstStat;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_GetResetState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_GetLowestPowerState()
* XOS2_OSAL_QNX_CD_CD_POW_149
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_211]
***********************************************************************************************************************/
/*PRQA S 3673 1 # False warning. This handle type is a pointer to an internal data structure. */
e_result_t R_PMA_SM_GetLowestPowerState(st_pma_handle_t 	handle,
                                        e_pma_hwa_id_t      hwaId,
                                        e_pma_power_state_t *pLowestPower)
{
    e_result_t result = R_PMA_OK;

    if ((NULL != handle)
       && (g_pma_max_hwa_id_num > hwaId)
       && (NULL != pLowestPower))
    {
        if (g_pma_sm_info_tbl[hwaId].handle != handle)
        {
            result = R_PMA_INVALID_HANDLE;
        }
        else
        {
            *pLowestPower = g_pma_sm_info_tbl[hwaId].enLowestPwrState;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_GetLowestPowerState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_UpdateLowestPowerState()
* XOS2_OSAL_QNX_CD_CD_POW_131
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_131]
***********************************************************************************************************************/
e_result_t R_PMA_SM_UpdateLowestPowerState(e_pma_hwa_id_t hwaId)
{
    e_result_t    result;
    e_pma_onoff_t clk_stat = R_PMA_UNDEFINED_STATE;
    e_pma_onoff_t pwr_stat = R_PMA_UNDEFINED_STATE;
    
    if (g_pma_max_hwa_id_num > hwaId)
    {
        if (NULL != g_pma_sm_info_tbl[hwaId].targetHwa)
        {
            result = R_PMA_HW_DEP_GetHwaState(hwaId, &clk_stat, &pwr_stat);
        }
        else
        {
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    if ((R_PMA_OK == result)
        || (R_PMA_BSP_ECPG == result))
    {
        result = R_PMA_OK;
        switch (g_pma_sm_info_tbl[hwaId].enLowestPwrState)
        {
            case R_PMA_STATE_ENABLED:
                if ( R_PMA_STATE_ON == pwr_stat)
                {
                    if ( R_PMA_STATE_ON != clk_stat)
                    {
                        g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_CG;
                    }
                }
                else
                {
                    g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_PG;
                }
                break;
            case R_PMA_STATE_CG:
                if ( R_PMA_STATE_ON != pwr_stat)
                {
                    g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_PG;
                }
                break;
            case R_PMA_STATE_PG:
                /** do nothing */
                break;
            case R_PMA_STATE_UNKNOWN:
                if ( R_PMA_STATE_ON == pwr_stat)
                {
                    g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_CG;
                }
                else
                {
                    g_pma_sm_info_tbl[hwaId].enLowestPwrState = R_PMA_STATE_PG;
                }
                break;
            default:
                result = R_PMA_NG;
                break;
        }   /* end of switch (g_pma_sm_info_tbl[hwaId].enLowestPwrState) */
    }   /* end of if ((R_PMA_OK == result)... */
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_UpdateLowestPowerState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_ChangeStateToHP()
* XOS3_OSAL_NOS_CD_CD_POW_120
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_212]
***********************************************************************************************************************/
static e_result_t R_PMA_SM_ChangeStateToHP(st_statemachine_t *pStatemachine)
{
    e_result_t result;
    
    if (NULL != pStatemachine)
    {
        result = pStatemachine->targetHwa->noticeChangeToHP(pStatemachine->targetHwa->id, pStatemachine->targetHwa->id);
        if (R_PMA_OK == result)
        {
            pStatemachine->enPrevStbMode = R_PMA_STATE_ENABLED;
        }
    }
    else
    {
        result = R_PMA_NG;
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_ChangeStateToHP()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_ChangeStateToCG()
* XOS3_OSAL_NOS_CD_CD_POW_121
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_213]
***********************************************************************************************************************/
static e_result_t R_PMA_SM_ChangeStateToCG(st_statemachine_t *pStatemachine)
{
    e_result_t result = R_PMA_OK;
    e_pma_onoff_t clk_stat;
    e_pma_onoff_t pwr_stat;
    
    if (NULL != pStatemachine)
    {
        switch (pStatemachine->enPrevStbMode)
        {
            case R_PMA_STATE_ENABLED:
                result = pStatemachine->targetHwa->noticeChangeHPtoCG(pStatemachine->targetHwa->id, pStatemachine->targetHwa->id);
                break;

            case R_PMA_STATE_PG:
                result = pStatemachine->targetHwa->noticeChangePGtoCG(pStatemachine->targetHwa->id, pStatemachine->targetHwa->id);
                break;

            case R_PMA_STATE_CG:
                /* do nothing */
                break;

            default:
                result = R_PMA_HW_DEP_GetHwaState(pStatemachine->targetHwa->id, &clk_stat, &pwr_stat);

                if ((R_PMA_OK == result)
                    || (R_PMA_BSP_ECPG == result))
                {
                    if (R_PMA_STATE_ON == pwr_stat)
                    {
                        result = pStatemachine->targetHwa->noticeChangeHPtoCG(pStatemachine->targetHwa->id, pStatemachine->targetHwa->id);
                    }
                    else
                    {
                        result = pStatemachine->targetHwa->noticeChangePGtoCG(pStatemachine->targetHwa->id, pStatemachine->targetHwa->id);
                    }
                }
                else
                {
                    result = R_PMA_NG;
                }
                break;
        }   /* end of switch (pStatemachine->enPrevStbMode) */

        if (R_PMA_OK == result)
        {
            pStatemachine->enPrevStbMode = R_PMA_STATE_CG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if (NULL != pStatemachine) */
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_ChangeStateToCG()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_SM_ChangeStateToPG()
* XOS3_OSAL_NOS_CD_CD_POW_122
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_121]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_214]
***********************************************************************************************************************/
static e_result_t R_PMA_SM_ChangeStateToPG(st_statemachine_t *pStatemachine)
{
    e_result_t result;
    
    if (NULL != pStatemachine)
    {
        result = pStatemachine->targetHwa->noticeChangeToPG(pStatemachine->targetHwa->id, pStatemachine->targetHwa->id);
        if (R_PMA_OK == result)
        {
            pStatemachine->enPrevStbMode = R_PMA_STATE_PG;
        }
    }
    else
    {
        result = R_PMA_NG;
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_SM_ChangeStateToPG()
***********************************************************************************************************************/
