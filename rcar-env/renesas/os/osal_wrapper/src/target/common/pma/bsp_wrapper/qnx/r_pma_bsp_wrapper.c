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
* Version :      1.0.0
* Description  : Internal function for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include <string.h>
#include <unistd.h>

#include <hw/rcar_cpg.h>
#include <hw/rcar_sysc.h>

#include "target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h"
/*======================================================================================================================
Private global variables
======================================================================================================================*/
static cpg_mgr_funcs_t clkhwfuncs;
static sysc_mgr_funcs_t syschwfuncs;

static e_cpg_domain_id_t cpg_idmax;
static e_sysc_domain_id_t sysc_idmax;

/* PRQA S 5209 3 # System is requesting an int, so it cannot be changed. */
/* PRQA S 3684 2 # The array size is defined by the initializer, preventing buffer overruns with termination data in the last element. */
extern const int g_pma_bsp_wrap_sysc_id_tbl_v3m[];
extern const int g_pma_bsp_wrap_cpg_id_tbl_v3m[];

/* PRQA S 5209 3 # System is requesting an int, so it cannot be changed. */
/* PRQA S 3684 2 # The array size is defined by the initializer, preventing buffer overruns with termination data in the last element. */
extern const int g_pma_bsp_wrap_sysc_id_tbl_v3h1[];
extern const int g_pma_bsp_wrap_cpg_id_tbl_v3h1[];

/* PRQA S 5209 3 # System is requesting an int, so it cannot be changed. */
/* PRQA S 3684 2 # The array size is defined by the initializer, preventing buffer overruns with termination data in the last element. */
extern const int g_pma_bsp_wrap_sysc_id_tbl_v3h2[];
extern const int g_pma_bsp_wrap_cpg_id_tbl_v3h2[];

/* PRQA S 5209 3 # System is requesting an int, so it cannot be changed. */
/* PRQA S 3684 2 # The array size is defined by the initializer, preventing buffer overruns with termination data in the last element. */
extern const int g_pma_bsp_wrap_sysc_id_tbl_v4h[];
extern const int g_pma_bsp_wrap_cpg_id_tbl_v4h[];

/* PRQA S 5209 3 # System is requesting an int, so it cannot be changed. */
/* PRQA S 3684 2 # The array size is defined by the initializer, preventing buffer overruns with termination data in the last element. */
extern const int g_pma_bsp_wrap_sysc_id_tbl_v4m[];
extern const int g_pma_bsp_wrap_cpg_id_tbl_v4m[];

/* PRQA S 5209 3 # System is requesting an int, so it cannot be changed. */
/* PRQA S 3684 2 # The array size is defined by the initializer, preventing buffer overruns with termination data in the last element. */
extern const int g_pma_bsp_wrap_sysc_id_tbl_s4[];
extern const int g_pma_bsp_wrap_cpg_id_tbl_s4[];

/* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
static const int * g_pma_bsp_wrap_sysc_id_tbl;
static const int * g_pma_bsp_wrap_cpg_id_tbl;

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
static e_result_t R_PMA_BSP_WRAP_Exchange_ErrCode(int32_t errNo);
/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_Init()
* XOS3_OSAL_NOS_CD_CD_POW_149
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_149]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_Init(uint64_t pma_soc_type)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    drv_ret = cpg_mgr_getfuncs(&clkhwfuncs, (int)sizeof(cpg_mgr_funcs_t));
    if (CPG_RET_OK == drv_ret)
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        drv_ret = sysc_mgr_getfuncs(&syschwfuncs, (int)sizeof(sysc_mgr_funcs_t));
        if (SYSC_RET_OK == drv_ret)
        {
            drv_ret = syschwfuncs.init();
            if (SYSC_RET_OK == drv_ret)
            {
                drv_ret =  clkhwfuncs.init(0);
            }
        }
    }
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    if ((int)R_PMA_BSP_RET_OK != drv_ret)
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    
    if (R_PMA_OK == result)
    {
        switch (pma_soc_type)
        {
            case R_PMA_DEVICE_SOC_TYPE_V3M:
                cpg_idmax  = R_PMA_CPG_DOMAIN_ID_V3M_NUM;
                sysc_idmax = R_PMA_SYSC_DOMAIN_ID_V3M_NUM;
                g_pma_bsp_wrap_sysc_id_tbl = g_pma_bsp_wrap_sysc_id_tbl_v3m;
                g_pma_bsp_wrap_cpg_id_tbl  = g_pma_bsp_wrap_cpg_id_tbl_v3m;
                break;
            case R_PMA_DEVICE_SOC_TYPE_V3H1:
                cpg_idmax  = R_PMA_CPG_DOMAIN_ID_V3H1_NUM;
                sysc_idmax = R_PMA_SYSC_DOMAIN_ID_V3H_NUM;
                g_pma_bsp_wrap_sysc_id_tbl = g_pma_bsp_wrap_sysc_id_tbl_v3h1;
                g_pma_bsp_wrap_cpg_id_tbl  = g_pma_bsp_wrap_cpg_id_tbl_v3h1;
                break;
            case R_PMA_DEVICE_SOC_TYPE_V3H2:
                cpg_idmax  = R_PMA_CPG_DOMAIN_ID_V3H2_NUM;
                sysc_idmax = R_PMA_SYSC_DOMAIN_ID_V3H_NUM;
                g_pma_bsp_wrap_sysc_id_tbl = g_pma_bsp_wrap_sysc_id_tbl_v3h2;
                g_pma_bsp_wrap_cpg_id_tbl  = g_pma_bsp_wrap_cpg_id_tbl_v3h2;
                break;
            case R_PMA_DEVICE_SOC_TYPE_V4H:
                cpg_idmax  = R_PMA_CPG_DOMAIN_ID_V4H_NUM;
                sysc_idmax = R_PMA_SYSC_DOMAIN_ID_V4H_NUM;
                g_pma_bsp_wrap_sysc_id_tbl = g_pma_bsp_wrap_sysc_id_tbl_v4h;
                g_pma_bsp_wrap_cpg_id_tbl  = g_pma_bsp_wrap_cpg_id_tbl_v4h;
                break;
            case R_PMA_DEVICE_SOC_TYPE_V4M:
                cpg_idmax  = R_PMA_CPG_DOMAIN_ID_V4M_NUM;
                sysc_idmax = R_PMA_SYSC_DOMAIN_ID_V4M_NUM;
                g_pma_bsp_wrap_sysc_id_tbl = g_pma_bsp_wrap_sysc_id_tbl_v4m;
                g_pma_bsp_wrap_cpg_id_tbl  = g_pma_bsp_wrap_cpg_id_tbl_v4m;
                break;
            case R_PMA_DEVICE_SOC_TYPE_S4:
                cpg_idmax  = R_PMA_CPG_DOMAIN_ID_S4_NUM;
                sysc_idmax = R_PMA_SYSC_DOMAIN_ID_S4_NUM;
                g_pma_bsp_wrap_sysc_id_tbl = g_pma_bsp_wrap_sysc_id_tbl_s4;
                g_pma_bsp_wrap_cpg_id_tbl  = g_pma_bsp_wrap_cpg_id_tbl_s4;
                break;
            default:
                result = R_PMA_INVALID_ARGUMENT;
                break;
        }   /* end of switch (pma_soc_type) */
    }   /* end of if (R_PMA_OK == result) */

    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_Quit()
* XOS3_OSAL_NOS_CD_CD_POW_150
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_150]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_Quit(void)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    drv_ret = clkhwfuncs.deinit();
    if (CPG_RET_OK == drv_ret)
    {
        syschwfuncs.deinit();
    }
    else
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_Quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetCpgON()
* XOS3_OSAL_NOS_CD_CD_POW_151
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_151]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetCpgON(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (cpg_idmax > modId)
    {
        drv_ret = clkhwfuncs.mstp_enable(g_pma_bsp_wrap_cpg_id_tbl[modId]);
        if (CPG_RET_OK == drv_ret)
        {
            drv_ret = clkhwfuncs.mstp_is_enabled(g_pma_bsp_wrap_cpg_id_tbl[modId]);
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            if ((int)R_PMA_CPG_CLOCK_ON == drv_ret)
            {
                drv_ret = CPG_RET_OK;
            }
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            else if ((int)R_PMA_CPG_CLOCK_OFF == drv_ret)
            {
                result = R_PMA_NG;
            }
            else
            {
                /** BSP Failure */
            }
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }   /* end of if (cpg_idmax > modId) */
    
    if ((result == R_PMA_OK)
         && (CPG_RET_OK != drv_ret))
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetCpgON()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetCpgOFF()
* XOS3_OSAL_NOS_CD_CD_POW_152
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_152]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetCpgOFF(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (cpg_idmax > modId)
    {
        drv_ret = clkhwfuncs.mstp_disable(g_pma_bsp_wrap_cpg_id_tbl[modId]);
        if (CPG_RET_OK == drv_ret)
        {
            drv_ret = clkhwfuncs.mstp_is_enabled(g_pma_bsp_wrap_cpg_id_tbl[modId]);
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            if ((int)R_PMA_CPG_CLOCK_OFF == drv_ret)
            {
                drv_ret = CPG_RET_OK;
            }
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            else if ((int)R_PMA_CPG_CLOCK_ON == drv_ret)
            {
                result = R_PMA_NG;
            }
            else
            {
                /** BSP Failure */
            }
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }   /* end of if (cpg_idmax > modId) */
    
    if ((result == R_PMA_OK)
         && (CPG_RET_OK != drv_ret))
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetCpgOFF()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_GetCpgState()
* XOS3_OSAL_NOS_CD_CD_POW_153
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_153]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_GetCpgState(e_cpg_domain_id_t modId, e_pma_clock_onoff_t *modState)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (cpg_idmax > modId)
    {
        drv_ret = clkhwfuncs.mstp_is_enabled(g_pma_bsp_wrap_cpg_id_tbl[modId]);
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        if ((int)R_PMA_CPG_CLOCK_OFF == drv_ret)
        {
            *modState = R_PMA_CPG_CLOCK_OFF;
        }
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        else if ((int)R_PMA_CPG_CLOCK_ON == drv_ret)
        {
            *modState = R_PMA_CPG_CLOCK_ON;
        }
        else
        {
            /** BSP Failure */
            result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }   /* end of if (cpg_idmax > modId) */
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_GetCpgState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetSyscON()
* XOS3_OSAL_NOS_CD_CD_POW_154
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_154]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetSyscON(e_sysc_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    if (sysc_idmax > modId)
    {
        drv_ret = syschwfuncs.power_on(g_pma_bsp_wrap_sysc_id_tbl[modId]);
        if (SYSC_RET_OK == drv_ret)
        {
            drv_ret = syschwfuncs.power_is_off(g_pma_bsp_wrap_sysc_id_tbl[modId]);
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            if ((int)R_PMA_SYSC_POWER_ON == drv_ret)
            {
                drv_ret = SYSC_RET_OK;
            }
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            else if ((int)R_PMA_SYSC_POWER_OFF == drv_ret)
            {
                result = R_PMA_NG;
            }
            else
            {
                /** BSP Failure */
            }
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }   /* end of if (sysc_idmax > modId) */
    
    if ((result == R_PMA_OK)
         && (SYSC_RET_OK != drv_ret))
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetSyscON()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetSyscOFF()
* XOS3_OSAL_NOS_CD_CD_POW_155
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_155]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetSyscOFF(e_sysc_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (sysc_idmax > modId)
    {
        drv_ret = syschwfuncs.power_off(g_pma_bsp_wrap_sysc_id_tbl[modId]);
        if (SYSC_RET_OK == drv_ret)
        {
            drv_ret = syschwfuncs.power_is_off(g_pma_bsp_wrap_sysc_id_tbl[modId]);
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            if ((int)R_PMA_SYSC_POWER_OFF == drv_ret)
            {
                drv_ret = SYSC_RET_OK;
            }
            /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
            else if ((int)R_PMA_SYSC_POWER_ON == drv_ret)
            {
                result = R_PMA_NG;
            }
            else
            {
                /** BSP Failure */
            }
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }   /* end of if (sysc_idmax > modId) */
    
    if ((result == R_PMA_OK)
         && (SYSC_RET_OK != drv_ret))
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetSyscOFF()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_GetSyscState()
* XOS3_OSAL_NOS_CD_CD_POW_156
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_156]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_GetSyscState(e_sysc_domain_id_t modId, e_pma_power_onoff_t *modState)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (sysc_idmax > modId)
    {
        drv_ret = syschwfuncs.power_is_off(g_pma_bsp_wrap_sysc_id_tbl[modId]);
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        if ((int)R_PMA_SYSC_POWER_OFF == drv_ret)
        {
            *modState = R_PMA_SYSC_POWER_OFF;
        }
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        else if ((int)R_PMA_SYSC_POWER_ON == drv_ret)
        {
            *modState = R_PMA_SYSC_POWER_ON;
        }
        else
        {
            /** BSP Failure */
            result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }   /* end of if (sysc_idmax > modId) */
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_GetSyscState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_AssertReset()
* XOS3_OSAL_NOS_CD_CD_POW_157
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_157]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_AssertReset(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (cpg_idmax > modId)
    {
        drv_ret = clkhwfuncs.mstp_assert(g_pma_bsp_wrap_cpg_id_tbl[modId]);
        if (CPG_RET_OK != drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_AssertReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_DeassertReset()
* XOS3_OSAL_NOS_CD_CD_POW_158
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_158]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_DeassertReset(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (cpg_idmax > modId)
    {
        drv_ret = clkhwfuncs.mstp_deassert(g_pma_bsp_wrap_cpg_id_tbl[modId]);
        if (CPG_RET_OK != drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_DeassertReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetPostDivider()
* XOS3_OSAL_NOS_CD_CD_POW_159
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_159]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetPostDivider(uint32_t chanIdx, uint32_t const * pClockDivider)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    /* PRQA S 5209,0311 1 # This is an input to BSP, don't care if modified inside.  */
    drv_ret = clkhwfuncs.clk_divider_set(CPG_CLK_POST, (int)chanIdx, (uint32_t*)pClockDivider);
    if (CPG_RET_OK != drv_ret)
    {
        /** BSP Failure */
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetPostDivider()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_EnablePostClock()
* XOS3_OSAL_NOS_CD_CD_POW_160
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_160]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_EnablePostClock(uint32_t chanIdx)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    /* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
    drv_ret = clkhwfuncs.clk_is_enabled(CPG_CLK_POST, (int)chanIdx);
    if ((int)R_PMA_CLK_ENABLE == drv_ret)
    {
        /** As expected */
        drv_ret = CPG_RET_OK;
    }
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    else if ((int)R_PMA_CLK_DISABLE == drv_ret)
    {
        /** Enable clock */
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        drv_ret = clkhwfuncs.clk_enable(CPG_CLK_POST, (int)chanIdx);
    }
    else
    {
        /** BSP Failure */
    }
    
    
    if (CPG_RET_OK != drv_ret)
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_EnablePostClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_DisablePostClock()
* XOS3_OSAL_NOS_CD_CD_POW_161
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_161]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_DisablePostClock(uint32_t chanIdx)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    /* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
    drv_ret = clkhwfuncs.clk_is_enabled(CPG_CLK_POST, (int)chanIdx);
    if ((int)R_PMA_CLK_DISABLE == drv_ret)
    {
        /** As expected */
        drv_ret = CPG_RET_OK;
    }
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    else if ((int)R_PMA_CLK_ENABLE == drv_ret)
    {
        /** Disable clock */
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        drv_ret = clkhwfuncs.clk_disable(CPG_CLK_POST, (int)chanIdx);
    }
    else
    {
        /** BSP Failure */
    }
    
    if (CPG_RET_OK != drv_ret)
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_DisablePostClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_Exchange_ErrCode()
* XOS3_OSAL_NOS_CD_CD_POW_162
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_162]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static e_result_t R_PMA_BSP_WRAP_Exchange_ErrCode(int32_t errNo)
{
    e_result_t result;

    switch(errNo)
    {
    /* PRQA S 5209 11 # System is requesting an int, so it cannot be changed. */
        case (int)R_PMA_BSP_RET_OK:
            result = R_PMA_OK;
            break;
        case (int)R_PMA_BSP_RET_ERROR:
        case (int)R_PMA_BSP_RET_HWFAIL:
            result = R_PMA_BSP_EINVAL;
            break;
        case (int)R_PMA_BSP_RET_TIMEOUT:
            result = R_PMA_BSP_ETIMEDOUT;
            break;
        case (int)R_PMA_BSP_RET_INVALPARA:
            result = R_PMA_BSP_EIO;
            break;
        default:
            result = R_PMA_NG;
            break;
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_Exchange_ErrCode()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_ReadPdmode()
* XOS3_OSAL_NOS_CD_CD_POW_163
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_163]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_ReadPdmode(e_pma_pd_mode_t *pdMode)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    int pd;
    
    pd = syschwfuncs.read_pdmode();
    if (0 > pd)
    {
        drv_ret = pd;
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        if ((int)R_PMA_SYSC_PDMODE_0 == pd)
        {
            *pdMode = R_PMA_SYSC_PDMODE_0;
        }
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        else if ((int)R_PMA_SYSC_PDMODE_1 == pd)
        {
            *pdMode = R_PMA_SYSC_PDMODE_1;
        }
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        else if ((int)R_PMA_SYSC_PDMODE_3 == pd)
        {
            *pdMode = R_PMA_SYSC_PDMODE_3;
        }
        else
        {
            /** do nothing */
        }
    }   /* end of if (0 > pd) */
    return result;
}

/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_ReadPdmode()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_GetResetState()
* XOS3_OSAL_NOS_CD_CD_POW_164
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_164]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_GetResetState(e_cpg_domain_id_t modId, e_pma_reset_req_t *pRstStat)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    int rst;
    
    rst = clkhwfuncs.mstp_reset_state(g_pma_bsp_wrap_cpg_id_tbl[modId]);
    if (0 > rst)
    {
        drv_ret = rst;
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    else
    {
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        if ((int)R_PMA_RELEASED_RESET == rst)
        {
            *pRstStat = R_PMA_RELEASED_RESET;
        }
        /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
        else if ((int)R_PMA_APPLIED_RESET == rst)
        {
            *pRstStat = R_PMA_APPLIED_RESET;
        }
        else
        {
            /** do nothing */
        }
    }   /* end of if (0 > rst) */
    
    return result;
}

/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_GetResetState()
***********************************************************************************************************************/
