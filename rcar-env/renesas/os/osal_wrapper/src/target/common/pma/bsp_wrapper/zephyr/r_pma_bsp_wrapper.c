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
* Version :      0.4.0
* Description  : Internal function for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include <string.h>
#include <unistd.h>

#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/rcar_clock_control.h>
#include <pm/device.h>
#include <pm/device_runtime.h>
#include <device.h>
#include <devicetree.h>

#include "target/zephyr/r_osal_os_type.h"
#include "target/common/pma/hw/r_pma_hwdepend.h"
#include "target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h"

typedef struct
{
    const char *id;
    uint32_t   cpg;
    uint32_t   reset;
    uint32_t   sysc;
} st_bsp_wrap_pma_tbl_t;

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
#define DT_HAS_CPG(node_id) COND_CODE_1( DT_NODE_HAS_PROP(node_id, clocks), \
    (DT_CPG(node_id)), (R_PMA_CPG_INVALID_PARAM))

#define DT_CPG(node_id) COND_CODE_1( DT_CLOCKS_CELL_BY_IDX(node_id, 0, domain), \
    (DT_CLOCKS_CELL_BY_IDX(node_id, 0, module)), (R_PMA_CPG_INVALID_PARAM))

#define DT_RESET(node_id) COND_CODE_1( DT_NODE_HAS_PROP(node_id, resets), \
    (DT_PHA_BY_IDX(node_id, resets, 0, module)), (R_PMA_CPG_INVALID_PARAM))

#define DT_SYSC(node_id) COND_CODE_1( DT_NODE_HAS_PROP(node_id, power_domains), \
    (DT_PHA_BY_IDX(node_id, power_domains, 0, sysc)), (R_PMA_SYSC_INVALID_PARAM))

#define PMA_NODE_PROPERTIES(node_id) {DT_LABEL(node_id), DT_HAS_CPG(node_id), DT_RESET(node_id), DT_SYSC(node_id)},

/*======================================================================================================================
Private global variables
======================================================================================================================*/
/* PRQA S 1514,3408 1 # Definition within a function reduces readability and maintainability, so no change. */
const st_bsp_wrap_pma_tbl_t gs_osal_pma_table[] = 
{
#ifndef OSAL_ZEPHYR_UT
    /* PRQA S 1035 1 # Caused by the implementation of macros defined inside the Zephyr OS. No side effect. */
    DT_FOREACH_CHILD(DT_NODELABEL(osal_device), PMA_NODE_PROPERTIES)
#else
    {
        .id     = "ut_dummy_1",
        .cpg    = CPG_MSTP5_IMS0,
        .reset  = CPG_SR5_IMS0,
        .sysc   = R_PMA_SYSC_INVALID_PARAM
    },
    {
        .id     = "fbc_ut_dummy_1",
        .cpg    = R_PMA_CPG_INVALID_PARAM,
        .reset  = R_PMA_CPG_INVALID_PARAM,
        .sysc   = R_PMA_SYSC_INVALID_PARAM
    },
#endif
    {.id = OSAL_DEVICE_INVALID_ID}
};

static e_cpg_domain_id_t cpg_idmax;
static e_sysc_domain_id_t sysc_idmax;
static e_pma_hwa_id_t hwa_idmax;
/* PRQA S 3684 15 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const struct device * g_pma_bsp_wrap_sysc_tbl_v4h[];
extern const uint32_t g_pma_bsp_wrap_cpg_id_tbl_v4h[];
extern const uint32_t g_pma_bsp_wrap_reset_id_tbl_v4h[];
extern const uint32_t g_pma_bsp_wrap_sysc_id_tbl_v4h[];
/* PRQA S 1513 1 # HWA info and sysc references are required. It cannot be avoided */
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_v4h[];

extern const uint32_t g_pma_bsp_wrap_clock_id_tbl_v4h[];

/* PRQA S 3678 1 # g_pma_bsp_wrap_sysc_tbl stores g_pma_bsp_wrap_sysc_tbl_v4h. No need to be const. */
static const struct device **g_pma_bsp_wrap_sysc_tbl;
static const uint32_t * g_pma_bsp_wrap_cpg_id_tbl;
static const uint32_t * g_pma_bsp_wrap_reset_id_tbl;
static const uint32_t * g_pma_bsp_wrap_clock_id_tbl;
static const uint32_t * g_pma_bsp_wrap_sysc_id_tbl;
static const st_hwa_t * g_pma_hw_dep_hwa_info_tbl;
#ifndef OSAL_ZEPHYR_UT
static const struct device * g_cpg_device = DEVICE_DT_GET(DT_NODELABEL(cpg));
#else
static struct device * g_cpg_device;
#endif

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
static e_result_t r_pma_bsp_wrap_get_pma_id(void);
static void r_pma_bsp_wrap_get_pma_id_from_pma_tbl(const uint32_t *cpg, const uint32_t *reset, const uint32_t *sysc, e_pma_hwa_id_t *pma_id);
static e_result_t r_pma_bsp_wrap_init_sysc_enable(void);

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_Init()
* XOS3_OSAL_NOS_CD_CD_POW_149
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_Init(uint64_t pma_soc_type)
{
    e_result_t result = R_PMA_OK;

    switch (pma_soc_type)
    {
    case R_PMA_DEVICE_SOC_TYPE_V4H:
        cpg_idmax  = R_PMA_CPG_DOMAIN_ID_V4H_NUM;
        sysc_idmax = R_PMA_SYSC_DOMAIN_ID_V4H_NUM;
        hwa_idmax  = R_PMA_HWA_ID_V4H_NUM;
        g_pma_bsp_wrap_sysc_tbl     = g_pma_bsp_wrap_sysc_tbl_v4h;
        g_pma_bsp_wrap_cpg_id_tbl   = g_pma_bsp_wrap_cpg_id_tbl_v4h;
        g_pma_bsp_wrap_reset_id_tbl = g_pma_bsp_wrap_reset_id_tbl_v4h;
        g_pma_bsp_wrap_clock_id_tbl = g_pma_bsp_wrap_clock_id_tbl_v4h;
        g_pma_bsp_wrap_sysc_id_tbl  = g_pma_bsp_wrap_sysc_id_tbl_v4h;
        g_pma_hw_dep_hwa_info_tbl   = g_pma_hw_dep_hwa_info_tbl_v4h;
        break;
    default:
        result = R_PMA_INVALID_ARGUMENT;
        break;
    }

    if (R_PMA_OK == result)
    {
        result = r_pma_bsp_wrap_get_pma_id();
    }
    else
    {
        /* Do nothing */
    }

    if (R_PMA_OK == result)
    {
        result = r_pma_bsp_wrap_init_sysc_enable();
    }
    else
    {
        /* Do nothing */
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_Quit()
* XOS3_OSAL_NOS_CD_CD_POW_150
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_Quit(void)
{
    e_result_t result = R_PMA_OK;
    uint32_t tbl_count = 0;

    while (g_osal_num_of_device_config > tbl_count)
    {
        gp_osal_device_info[tbl_count].pma_id = R_PMA_HWA_ID_NONE;
        tbl_count++;
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
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetCpgON(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    enum clock_control_status clk_state;
    struct rcar_cpg_clk clk_cpg = {CPG_MOD, 0, 0, 0};
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    if (cpg_idmax > modId)
    {
        clk_cpg.module = g_pma_bsp_wrap_cpg_id_tbl[modId];
        /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
        drv_ret = clock_control_on(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
        if (0 == drv_ret)
        {
            /* Confirm that the clock control status becomes ON. */
            /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
            clk_state = clock_control_get_status(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
            if (CLOCK_CONTROL_STATUS_ON == clk_state)
            {
                /* do nothing */
            }
            else if (CLOCK_CONTROL_STATUS_OFF == clk_state)
            {
                result = R_PMA_NG;
            }
            else
            {
                /** BSP Failure */
                result = R_PMA_BSP_ECPG;
            }
        }
        else
        {
            result = R_PMA_BSP_ECPG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
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
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetCpgOFF(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    enum clock_control_status clk_state;
    struct rcar_cpg_clk clk_cpg = {CPG_MOD, 0, 0, 0};
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (cpg_idmax > modId)
    {
        clk_cpg.module = g_pma_bsp_wrap_cpg_id_tbl[modId];
        /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
        drv_ret = clock_control_off(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
        if (0 == drv_ret)
        {
            /* Confirm that the clock control status becomes OFF. */
            /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
            clk_state = clock_control_get_status(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
            if (CLOCK_CONTROL_STATUS_OFF == clk_state)
            {
                /* do nothing */
            }
            else if (CLOCK_CONTROL_STATUS_ON == clk_state)
            {
                result = R_PMA_NG;
            }
            else
            {
                /** BSP Failure */
                result = R_PMA_BSP_ECPG;
            }
        }
        else
        {
            result = R_PMA_BSP_ECPG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
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
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_GetCpgState(e_cpg_domain_id_t modId, e_pma_clock_onoff_t *modState)
{
    e_result_t result = R_PMA_OK;
    enum clock_control_status clk_state;
    struct rcar_cpg_clk clk_cpg = {CPG_MOD, 0, 0, 0};
    
    if (cpg_idmax > modId)
    {
        clk_cpg.module = g_pma_bsp_wrap_cpg_id_tbl[modId];

        /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
        clk_state = clock_control_get_status(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
        if (CLOCK_CONTROL_STATUS_OFF == clk_state)
        {
            *modState = R_PMA_CPG_CLOCK_OFF;
        }
        else if (CLOCK_CONTROL_STATUS_ON == clk_state)
        {
            *modState = R_PMA_CPG_CLOCK_ON;
        }
        else
        {
            /** BSP Failure */
            result = R_PMA_BSP_ECPG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_GetCpgState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetSyscON()
* XOS3_OSAL_NOS_CD_CD_POW_154
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetSyscON(e_sysc_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    enum pm_device_state pwr_state;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    if (sysc_idmax > modId)
    {
        drv_ret = pm_device_state_get(g_pma_bsp_wrap_sysc_tbl[modId], &pwr_state);
        if (0 != drv_ret)
        {
            result = R_PMA_BSP_ECPG;
        }
        else if (PM_DEVICE_STATE_ACTIVE == pwr_state)
        {
            /* do nothing */
        }
        else if (PM_DEVICE_STATE_SUSPENDED == pwr_state)
        {
            /* Set sysc to ON in case that the current status is SUSPENDED. */
            drv_ret = pm_device_runtime_get(g_pma_bsp_wrap_sysc_tbl[modId]);
            if (0 == drv_ret)
            {
                /* Confirm that the device state becomes active. */
                drv_ret = pm_device_state_get(g_pma_bsp_wrap_sysc_tbl[modId], &pwr_state);
                if (0 != drv_ret)
                {
                    result = R_PMA_BSP_ECPG;
                }
                else if (PM_DEVICE_STATE_ACTIVE == pwr_state)
                {
                    /* do nothing */
                }
                else if (PM_DEVICE_STATE_SUSPENDED == pwr_state)
                {
                    result = R_PMA_NG;
                }
                else
                {
                    result = R_PMA_BSP_ECPG;
                }
            }
            else
            {
                result = R_PMA_BSP_ECPG;
            }
        }
        else
        {
            result = R_PMA_BSP_ECPG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    return result;
/* PRQA S 9110 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetSyscON()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetSyscOFF()
* XOS3_OSAL_NOS_CD_CD_POW_155
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetSyscOFF(e_sysc_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    enum pm_device_state pwr_state;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    if (sysc_idmax > modId)
    {
        drv_ret = pm_device_state_get(g_pma_bsp_wrap_sysc_tbl[modId], &pwr_state);
        if (0 != drv_ret)
        {
            result = R_PMA_BSP_ECPG;
        }
        else if (PM_DEVICE_STATE_SUSPENDED == pwr_state)
        {
            /* do nothing */
        }
        else if (PM_DEVICE_STATE_ACTIVE == pwr_state)
        {
            /* Set sysc to OFF in case that the current status is ACTIVE. */
            drv_ret = pm_device_runtime_put(g_pma_bsp_wrap_sysc_tbl[modId]);
            if (0 == drv_ret)
            {
                /* Confirm that the device state becomes suspended. */
                drv_ret = pm_device_state_get(g_pma_bsp_wrap_sysc_tbl[modId], &pwr_state);
                if (0 != drv_ret)
                {
                    result = R_PMA_BSP_ECPG;
                }
                else if (PM_DEVICE_STATE_SUSPENDED == pwr_state)
                {
                    /* do nothing */
                }
                else if (PM_DEVICE_STATE_ACTIVE == pwr_state)
                {
                    result = R_PMA_NG;
                }
                else
                {
                    result = R_PMA_BSP_ECPG;
                }
            }
            else
            {
                result = R_PMA_BSP_ECPG;
            }
        }
        else
        {
            result = R_PMA_BSP_ECPG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    return result;
/* PRQA S 9110 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetSyscOFF()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_GetSyscState()
* XOS3_OSAL_NOS_CD_CD_POW_156
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_GetSyscState(e_sysc_domain_id_t modId, e_pma_power_onoff_t *modState)
{
    e_result_t result = R_PMA_OK;
    enum pm_device_state pwr_state;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    if (sysc_idmax > modId)
    {
        drv_ret = pm_device_state_get(g_pma_bsp_wrap_sysc_tbl[modId], &pwr_state);
        if (0 != drv_ret)
        {
            result = R_PMA_BSP_ECPG;
        }
        else if (PM_DEVICE_STATE_SUSPENDED == pwr_state)
        {
            *modState = R_PMA_SYSC_POWER_OFF;
        }
        else if (PM_DEVICE_STATE_ACTIVE == pwr_state)
        {
            *modState = R_PMA_SYSC_POWER_ON;
        }
        else
        {
            /** BSP Failure */
            result = R_PMA_BSP_ECPG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_GetSyscState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_AssertReset()
* XOS3_OSAL_NOS_CD_CD_POW_157
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_AssertReset(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    struct rcar_cpg_clk clk_cpg = {CPG_RESET, 0, 0, 1};
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (cpg_idmax > modId)
    {
        clk_cpg.module = g_pma_bsp_wrap_reset_id_tbl[modId];

        /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
        drv_ret = clock_control_off(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
        if (0 != drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_BSP_ECPG;
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
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_DeassertReset(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    struct rcar_cpg_clk clk_cpg = {CPG_RESET, 0, 0, 0};
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    
    if (cpg_idmax > modId)
    {
        clk_cpg.module = g_pma_bsp_wrap_reset_id_tbl[modId];

        /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
        drv_ret = clock_control_off(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
        if (0 != drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_BSP_ECPG;
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
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetPostDivider(uint32_t chanIdx, uint32_t const * pClockDivider)
{
    e_result_t result = R_PMA_OK;
    struct rcar_cpg_clk clk_cpg = {CPG_DIVISION_RATIO, 0, 0, 0};
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    clk_cpg.module = g_pma_bsp_wrap_clock_id_tbl[chanIdx];

    /* PRQA S 0314 2 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
    /* PRQA S 0326 1 # Casted to uint32_t inside the BSP code and this function used as per the BSP specification.*/
    drv_ret = clock_control_set_rate(g_cpg_device, (clock_control_subsys_t)&clk_cpg, (clock_control_subsys_rate_t)*pClockDivider);
    if (0 != drv_ret)
    {
        /** BSP Failure */
        result = R_PMA_BSP_ECPG;
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
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_EnablePostClock(uint32_t chanIdx)
{
    e_result_t result = R_PMA_OK;
    enum clock_control_status clk_state;
    struct rcar_cpg_clk clk_cpg = {CPG_CORE, 0, 0, 0};
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    clk_cpg.module = g_pma_bsp_wrap_clock_id_tbl[chanIdx];

    /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
    clk_state = clock_control_get_status(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
    if (CLOCK_CONTROL_STATUS_ON == clk_state)
    {
        /** As expected */
    }
    else if (CLOCK_CONTROL_STATUS_OFF == clk_state)
    {
        /** Enable clock */
        /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
        drv_ret = clock_control_on(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
        if (0 != drv_ret)
        {
            result = R_PMA_BSP_ECPG;
        }
    }
    else
    {
        /** BSP Failure */
        result = R_PMA_BSP_ECPG;
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
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_DisablePostClock(uint32_t chanIdx)
{
    e_result_t result = R_PMA_OK;
    enum clock_control_status clk_state;
    struct rcar_cpg_clk clk_cpg = {CPG_CORE, 0, 0, 0};
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    clk_cpg.module = g_pma_bsp_wrap_clock_id_tbl[chanIdx];

    /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
    clk_state = clock_control_get_status(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
    if (CLOCK_CONTROL_STATUS_OFF == clk_state)
    {
        /** As expected */
    }
    else if (CLOCK_CONTROL_STATUS_ON == clk_state)
    {
        /** Disable clock */
        /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
        drv_ret = clock_control_off(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
        if (0 != drv_ret)
        {
            result = R_PMA_BSP_ECPG;
        }
    }
    else
    {
        /** BSP Failure */
        result = R_PMA_BSP_ECPG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_DisablePostClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_ReadPdmode()
* XOS3_OSAL_NOS_CD_CD_POW_163
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
/*Note: This function exists only for compatibility with the OSAL common part and only returns OSAL_RETURN_OK. 
        There is no actual processing for the Zephyr OSAL wrapper. */
/* PRQA S 3673 1 # Implemented in accordance with the specification. */
e_result_t R_PMA_BSP_WRAP_ReadPdmode(e_pma_pd_mode_t *pdMode)
{
    e_result_t result = R_PMA_OK;
    (void)*pdMode; /* unused */
    return result;
}

/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_ReadPdmode()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_GetResetState()
* 
* [Covers: ]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_GetResetState(e_cpg_domain_id_t modId, e_pma_reset_req_t *pRstStat)
{
    e_result_t result = R_PMA_OK;
    enum clock_control_status clk_state;
    struct rcar_cpg_clk clk_cpg = {CPG_RESET, 0, 0, 0};

    clk_cpg.module = g_pma_bsp_wrap_reset_id_tbl[modId];

    /* PRQA S 0314 1 # The arguments must be clock_control_subsys_t (= void *) but no side effects in this case. */
    clk_state = clock_control_get_status(g_cpg_device, (clock_control_subsys_t)&clk_cpg);
    if (CLOCK_CONTROL_STATUS_OFF == clk_state)
    {
        *pRstStat = R_PMA_RELEASED_RESET;
    }
    else if (CLOCK_CONTROL_STATUS_ON == clk_state)
    {
        *pRstStat = R_PMA_APPLIED_RESET;
    }
    else
    {
        result = R_PMA_BSP_ECPG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_GetResetState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_pma_bsp_wrap_get_pma_id()
***********************************************************************************************************************/
static e_result_t r_pma_bsp_wrap_get_pma_id(void)
{
    e_result_t result = R_PMA_OK;
    size_t osal_num_of_device_config = 0;
    uint32_t dev_count = 0;
    uint32_t tbl_count = 0;
    uint32_t cpg;
    uint32_t reset;
    uint32_t sysc;
    e_pma_hwa_id_t pma_hwa_id = R_PMA_HWA_ID_NONE;

    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int os_ret;
    bool pma_found;

    while (OSAL_DEVICE_INVALID_ID != gs_osal_pma_table[osal_num_of_device_config].id)
    {
        osal_num_of_device_config++;
    }

    if (osal_num_of_device_config != g_osal_num_of_device_config)
    {
        result = R_PMA_NG;
    }

    /* Set up the pma_id of each device that has the power management function. */
    while ((g_osal_num_of_device_config > dev_count) && (R_PMA_OK == result))
    {
        pma_found = false;
        while ((g_osal_num_of_device_config > tbl_count) && (false == pma_found))
        {
            pma_hwa_id = R_PMA_HWA_ID_NONE;

            /* PRQA S 2836 1 # id is guaranteed to be null-terminated by the error check above. */
            os_ret = strcmp(gp_osal_device_info[dev_count].id, gs_osal_pma_table[tbl_count].id);
            if (0 == os_ret)
            {
                cpg = gs_osal_pma_table[tbl_count].cpg;
                reset = gs_osal_pma_table[tbl_count].reset;
                sysc = gs_osal_pma_table[tbl_count].sysc;
                if (((uint32_t)R_PMA_CPG_INVALID_PARAM == cpg) && ((uint32_t)R_PMA_CPG_INVALID_PARAM == reset) &&
                    ((uint32_t)R_PMA_SYSC_INVALID_PARAM == sysc))
                {
                    pma_hwa_id = R_PMA_HWA_ID_NONE;
                }
                else
                {
                    /* Get the corresponding PMA ID. */
                    r_pma_bsp_wrap_get_pma_id_from_pma_tbl(&cpg, &reset, &sysc, &pma_hwa_id);
                }
                pma_found = true;
            }
            else
            {
                /* Do nothing */
            }
            tbl_count++;
        }
        gp_osal_device_info[dev_count].pma_id = pma_hwa_id;
        dev_count++;
    }

    return result;
}
/***********************************************************************************************************************
* End of function r_pma_bsp_wrap_get_pma_id()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_pma_bsp_wrap_get_pma_id_from_pma_tbl()
***********************************************************************************************************************/
static void r_pma_bsp_wrap_get_pma_id_from_pma_tbl(const uint32_t *cpg, const uint32_t *reset, const uint32_t *sysc, e_pma_hwa_id_t *pma_id)
{
    e_cpg_domain_id_t  cpg_id   = R_PMA_CPG_DOMAIN_ID_NONE;
    e_cpg_domain_id_t  reset_id = R_PMA_CPG_DOMAIN_ID_NONE;
    e_sysc_domain_id_t sysc_id  = R_PMA_SYSC_DOMAIN_ID_NONE;
    uint32_t loop_count;
    uint32_t tbl_count;
    bool cpg_found  = false;
    bool rst_found  = false;
    bool pma_found  = false;

    /* Find sysc_id */
    if ((uint32_t)R_PMA_SYSC_INVALID_PARAM != *sysc)
    {
        /* PRQA S 4442 1 # No problem because it is an enum and an integer cast */
        sysc_id = g_pma_bsp_wrap_sysc_id_tbl[*sysc];
    }
    else
    {
        /* Do nothing */
    }

    /* Find cpg_id */
    loop_count = 0;
    while (((uint32_t)cpg_idmax > loop_count) && (false == cpg_found))
    {
        if ((uint32_t)R_PMA_CPG_INVALID_PARAM == *cpg)
        {
            cpg_found = true;
        }
        else if (g_pma_bsp_wrap_cpg_id_tbl[loop_count] == *cpg)
        {
            /* PRQA S 4442 1 # No problem because it is an enum and an integer cast */
            cpg_id = loop_count;
            cpg_found = true;
        }
        else
        {
            /* Do nothing */
        }

        loop_count++;
    }

    /* Find rst_id */
    loop_count = 0;
    while (((uint32_t)cpg_idmax > loop_count) && (false == rst_found))
    {
        if ((uint32_t)R_PMA_CPG_INVALID_PARAM == *reset)
        {
            rst_found = true;
        }
        else if (g_pma_bsp_wrap_reset_id_tbl[loop_count] == *reset)
        {
            /* PRQA S 4442 1 # No problem because it is an enum and an integer cast */
            reset_id = loop_count;
            rst_found = true;
        }
        else
        {
            /* Do nothing */
        }

        loop_count++;
    }

    /* Search PMA ID matching cpg_id/reset_id/sysc_id */
    if ((true == cpg_found) && (true == rst_found))
    {
        loop_count = 0;
        while (((uint32_t)hwa_idmax > loop_count) && (false == pma_found))
        {
            if ((g_pma_hw_dep_hwa_info_tbl[loop_count].cpg_id == cpg_id)
                && (g_pma_hw_dep_hwa_info_tbl[loop_count].reset_id == reset_id)
                && (g_pma_hw_dep_hwa_info_tbl[loop_count].sysc_id == sysc_id))
            {
                tbl_count = 0;
                /* Check the registered HWA ID from above conditions. */
                /* The num of g_osal_num_of_device_config is same the num of gp_osal_device_info,
                   because they are given from same device tree source. */
                while ((g_osal_num_of_device_config > tbl_count)
                    && (g_pma_hw_dep_hwa_info_tbl[loop_count].id != gp_osal_device_info[tbl_count].pma_id))
                {
                    tbl_count++;
                }

                /* Set the pma_id to HWA ID of the PMA manage table,
                   because there is no same pma_id of Device configuration table. */
                if (g_osal_num_of_device_config == tbl_count)
                {
                    *pma_id = g_pma_hw_dep_hwa_info_tbl[loop_count].id;
                    pma_found = true;
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
            loop_count++;
        }
    }
    else
    {
        /* Do nothing */
    }

    return;
/* PRQA S 9104 1 # For better readability this rule is ignored, and this function is not split. */
}
/***********************************************************************************************************************
* End of function r_pma_bsp_wrap_get_pma_id_from_pma_tbl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_pma_bsp_wrap_init_sysc_enable()
***********************************************************************************************************************/
static e_result_t r_pma_bsp_wrap_init_sysc_enable(void)
{
    e_result_t result = R_PMA_OK;
    uint32_t   loop_count = 0;
    enum pm_device_state pwr_state = PM_DEVICE_STATE_ACTIVE;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    while (((uint32_t)sysc_idmax > loop_count) && (R_PMA_OK == result))
    {
        drv_ret = pm_device_state_get(g_pma_bsp_wrap_sysc_tbl[loop_count], &pwr_state);
        if (0 == drv_ret)
        {
            drv_ret = pm_device_runtime_enable(g_pma_bsp_wrap_sysc_tbl[loop_count]);
            if (0 != drv_ret)
            {
                result = R_PMA_BSP_ECPG;
            }
            else
            {
                /* Do nothing */
            }
        }
        loop_count++;
    }

    return result;
}
/***********************************************************************************************************************
* End of function r_pma_bsp_wrap_init_sysc_enable()
***********************************************************************************************************************/
