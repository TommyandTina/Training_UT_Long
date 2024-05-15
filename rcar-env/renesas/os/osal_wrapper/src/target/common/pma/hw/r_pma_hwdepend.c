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
* Version :      1.5.0
* Description  : Hardware dependant function for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "target/common/pma/hw/r_pma_hwdepend.h"
#include "target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h"

/*======================================================================================================================
Private global variables
======================================================================================================================*/
static uint32_t g_pma_hw_dep_hwa_bit_num;
static const uint64_t * g_pma_hw_dep_hwa_bit_tbl;
static const e_pma_hwa_id_t * g_pma_hw_dep_hwa_bit_to_id_tbl;
static uint64_t * g_pma_hw_dep_sysc_ref_tbl;
static uint64_t * g_pma_hw_dep_cpg_ref_tbl;
static st_hwa_t * g_pma_hw_dep_hwa_info_tbl;
static const st_hwa_dependent_flag_t * g_pma_hw_dep_hwa_dependency_tbl;

static e_sysc_domain_id_t g_pma_hw_dep_max_sysc_id_num;
static e_cpg_domain_id_t  g_pma_hw_dep_max_cpg_id_num;

/* Function pointer of callback to UpdateLowestPowerState */
static p_sm_stat_chg_notice_func_t g_pma_hw_dep_callback_func;

/* Table of V3M */
extern const uint64_t g_pma_hw_dep_hwa_bit_tbl_v3m[R_PMA_HWA_ID_V3M_NUM];
extern const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v3m[R_PMA_HW_DEP_HWA_DEPEND_V3M_NUM];
extern uint64_t g_pma_hw_dep_sysc_ref_tbl_v3m[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V3M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern uint64_t g_pma_hw_dep_cpg_ref_tbl_v3m[(uint64_t)R_PMA_CPG_DOMAIN_ID_V3M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3m[R_PMA_HWA_ID_V3M_NUM];
extern const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3m[R_PMA_HWA_ID_V3M_NUM];

/* Table of V3H1 */
extern const uint64_t g_pma_hw_dep_hwa_bit_tbl_v3h1[R_PMA_HWA_ID_V3H1_NUM];
extern const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v3h1[R_PMA_HW_DEP_HWA_DEPEND_V3H1_NUM];
extern uint64_t g_pma_hw_dep_sysc_ref_tbl_v3h1[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V3H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern uint64_t g_pma_hw_dep_cpg_ref_tbl_v3h1[(uint64_t)R_PMA_CPG_DOMAIN_ID_V3H1_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3h1[R_PMA_HWA_ID_V3H1_NUM];
extern const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3h1[R_PMA_HWA_ID_V3H1_NUM];

/* Table of V3H2 */
extern const uint64_t g_pma_hw_dep_hwa_bit_tbl_v3h2[R_PMA_HWA_ID_V3H2_NUM];
extern const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v3h2[R_PMA_HW_DEP_HWA_DEPEND_V3H2_NUM];
extern uint64_t g_pma_hw_dep_sysc_ref_tbl_v3h2[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V3H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern uint64_t g_pma_hw_dep_cpg_ref_tbl_v3h2[(uint64_t)R_PMA_CPG_DOMAIN_ID_V3H2_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3h2_pd0[R_PMA_HWA_ID_V3H2_NUM];
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3h2_pd1[R_PMA_HWA_ID_V3H2_NUM];
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_v3h2_pd3[R_PMA_HWA_ID_V3H2_NUM];
extern const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd0[R_PMA_HWA_ID_V3H2_NUM];
extern const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd1[R_PMA_HWA_ID_V3H2_NUM];
extern const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd3[R_PMA_HWA_ID_V3H2_NUM];

/* Table of V4H */
extern const uint64_t g_pma_hw_dep_hwa_bit_tbl_v4h[R_PMA_HWA_ID_V4H_NUM];
extern const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v4h[R_PMA_HW_DEP_HWA_DEPEND_V4H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern uint64_t g_pma_hw_dep_sysc_ref_tbl_v4h[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V4H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern uint64_t g_pma_hw_dep_cpg_ref_tbl_v4h[(uint64_t)R_PMA_CPG_DOMAIN_ID_V4H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_v4h[R_PMA_HWA_ID_V4H_NUM];
extern const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v4h[R_PMA_HWA_ID_V4H_NUM];

/* Table of V4M */
extern const uint64_t g_pma_hw_dep_hwa_bit_tbl_v4m[R_PMA_HWA_ID_V4M_NUM];
extern const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v4m[R_PMA_HW_DEP_HWA_DEPEND_V4M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern uint64_t g_pma_hw_dep_sysc_ref_tbl_v4m[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V4M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern uint64_t g_pma_hw_dep_cpg_ref_tbl_v4m[(uint64_t)R_PMA_CPG_DOMAIN_ID_V4M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_v4m[R_PMA_HWA_ID_V4M_NUM];
extern const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v4m[R_PMA_HWA_ID_V4M_NUM];

/* Table of S4 */
extern const uint64_t g_pma_hw_dep_hwa_bit_tbl_s4[R_PMA_HWA_ID_S4_NUM];
extern uint64_t g_pma_hw_dep_cpg_ref_tbl_s4[(uint64_t)R_PMA_CPG_DOMAIN_ID_S4_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM];
extern st_hwa_t g_pma_hw_dep_hwa_info_tbl_s4[R_PMA_HWA_ID_S4_NUM];
extern const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_s4[R_PMA_HWA_ID_S4_NUM];

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
static void R_PMA_HW_DEP_NoticeToCpgOn(e_pma_hwa_id_t hwaId, e_cpg_domain_id_t cpgId);
static void R_PMA_HW_DEP_NoticeToCpgOff(e_pma_hwa_id_t hwaId, e_cpg_domain_id_t cpgId);
static void R_PMA_HW_DEP_NoticeToSyscOn(e_pma_hwa_id_t hwaId, e_sysc_domain_id_t syscId);
static void R_PMA_HW_DEP_NoticeToSyscOff(e_pma_hwa_id_t hwaId, e_sysc_domain_id_t syscId);
static e_result_t R_PMA_HW_DEP_SetCpgOn(e_cpg_domain_id_t modId);
static e_result_t R_PMA_HW_DEP_SetCpgOff(e_cpg_domain_id_t modId);
static e_result_t R_PMA_HW_DEP_SetSyscOn(e_sysc_domain_id_t modId);
static e_result_t R_PMA_HW_DEP_SetSyscOff(e_sysc_domain_id_t modId);
static e_result_t R_PMA_HW_DEP_CheckResetDepChild(e_pma_hwa_id_t hwaId);
static e_result_t R_PMA_HW_DEP_CheckResetDepParent(e_pma_hwa_id_t hwaId);
static e_result_t R_PMA_HW_DEP_SetResetState(e_pma_hwa_id_t hwaId);
static e_result_t R_PMA_HW_DEP_SetHwaState(e_pma_hwa_id_t hwaId);
static e_result_t R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(e_pma_hwa_id_t targetHwaId);

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_Init()
* XOS3_OSAL_NOS_CD_CD_POW_123
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_127]
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_103]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_Init(uint64_t pma_soc_type,
                             p_sm_stat_chg_notice_func_t pFunc)
{
    e_result_t      result = R_PMA_OK;
    e_pma_pd_mode_t pd_mode;

    switch (pma_soc_type)
    {
        case R_PMA_DEVICE_SOC_TYPE_V3M:
            g_pma_hw_dep_hwa_bit_num        = R_PMA_HW_DEP_HWA_DEPEND_V3M_NUM;
            g_pma_hw_dep_hwa_bit_tbl        = (const uint64_t *)g_pma_hw_dep_hwa_bit_tbl_v3m;
            g_pma_hw_dep_hwa_bit_to_id_tbl  = (const e_pma_hwa_id_t *)g_pma_hw_dep_hwa_bit_to_id_tbl_v3m;
            g_pma_hw_dep_sysc_ref_tbl       = (uint64_t *)g_pma_hw_dep_sysc_ref_tbl_v3m;
            g_pma_hw_dep_cpg_ref_tbl        = (uint64_t *)g_pma_hw_dep_cpg_ref_tbl_v3m;
            g_pma_hw_dep_hwa_info_tbl       = (st_hwa_t *)g_pma_hw_dep_hwa_info_tbl_v3m;
            g_pma_hw_dep_hwa_dependency_tbl = (const st_hwa_dependent_flag_t *)g_pma_hw_dep_hwa_dependency_tbl_v3m;
            g_pma_hw_dep_max_sysc_id_num    = R_PMA_SYSC_DOMAIN_ID_V3M_NUM;
            g_pma_hw_dep_max_cpg_id_num     = R_PMA_CPG_DOMAIN_ID_V3M_NUM;
            break;

        case R_PMA_DEVICE_SOC_TYPE_V3H1:
            g_pma_hw_dep_hwa_bit_num        = R_PMA_HW_DEP_HWA_DEPEND_V3H1_NUM;
            g_pma_hw_dep_hwa_bit_tbl        = (const uint64_t *)g_pma_hw_dep_hwa_bit_tbl_v3h1;
            g_pma_hw_dep_hwa_bit_to_id_tbl  = (const e_pma_hwa_id_t *)g_pma_hw_dep_hwa_bit_to_id_tbl_v3h1;
            g_pma_hw_dep_sysc_ref_tbl       = (uint64_t *)g_pma_hw_dep_sysc_ref_tbl_v3h1;
            g_pma_hw_dep_cpg_ref_tbl        = (uint64_t *)g_pma_hw_dep_cpg_ref_tbl_v3h1;
            g_pma_hw_dep_hwa_info_tbl       = (st_hwa_t *)g_pma_hw_dep_hwa_info_tbl_v3h1;
            g_pma_hw_dep_hwa_dependency_tbl = (const st_hwa_dependent_flag_t *)g_pma_hw_dep_hwa_dependency_tbl_v3h1;
            g_pma_hw_dep_max_sysc_id_num    = R_PMA_SYSC_DOMAIN_ID_V3H_NUM;
            g_pma_hw_dep_max_cpg_id_num     = R_PMA_CPG_DOMAIN_ID_V3H1_NUM;
            break;

        case R_PMA_DEVICE_SOC_TYPE_V3H2:
            result = R_PMA_BSP_WRAP_ReadPdmode(&pd_mode);
            if (R_PMA_OK == result)
            {
                switch (pd_mode)
                {
                    case R_PMA_SYSC_PDMODE_0:
                        g_pma_hw_dep_hwa_info_tbl = (st_hwa_t *)g_pma_hw_dep_hwa_info_tbl_v3h2_pd0;
                        g_pma_hw_dep_hwa_dependency_tbl = (const st_hwa_dependent_flag_t *)g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd0;
                        break;
                    case R_PMA_SYSC_PDMODE_1:
                        g_pma_hw_dep_hwa_info_tbl = (st_hwa_t *)g_pma_hw_dep_hwa_info_tbl_v3h2_pd1;
                        g_pma_hw_dep_hwa_dependency_tbl = (const st_hwa_dependent_flag_t *)g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd1;
                        break;
                    case R_PMA_SYSC_PDMODE_3:
                        g_pma_hw_dep_hwa_info_tbl = (st_hwa_t *)g_pma_hw_dep_hwa_info_tbl_v3h2_pd3;
                        g_pma_hw_dep_hwa_dependency_tbl = (const st_hwa_dependent_flag_t *)g_pma_hw_dep_hwa_dependency_tbl_v3h2_pd3;
                        break;
                    default:
                        result = R_PMA_NG;
                        break;
                }
                if (R_PMA_OK == result)
                {
                    g_pma_hw_dep_hwa_bit_num        = R_PMA_HW_DEP_HWA_DEPEND_V3H2_NUM;
                    g_pma_hw_dep_hwa_bit_tbl        = (const uint64_t *)g_pma_hw_dep_hwa_bit_tbl_v3h2;
                    g_pma_hw_dep_hwa_bit_to_id_tbl  = (const e_pma_hwa_id_t *)g_pma_hw_dep_hwa_bit_to_id_tbl_v3h2;
                    g_pma_hw_dep_sysc_ref_tbl       = (uint64_t *)g_pma_hw_dep_sysc_ref_tbl_v3h2;
                    g_pma_hw_dep_cpg_ref_tbl        = (uint64_t *)g_pma_hw_dep_cpg_ref_tbl_v3h2;
                    g_pma_hw_dep_max_sysc_id_num    = R_PMA_SYSC_DOMAIN_ID_V3H_NUM;
                    g_pma_hw_dep_max_cpg_id_num     = R_PMA_CPG_DOMAIN_ID_V3H2_NUM;
                }
            }   /* end of if (R_PMA_OK == result) */
            break;

        case R_PMA_DEVICE_SOC_TYPE_V4H:
            g_pma_hw_dep_hwa_bit_num        = R_PMA_HW_DEP_HWA_DEPEND_V4H_NUM;
            g_pma_hw_dep_hwa_bit_tbl        = (const uint64_t *)g_pma_hw_dep_hwa_bit_tbl_v4h;
            g_pma_hw_dep_hwa_bit_to_id_tbl  = (const e_pma_hwa_id_t *)g_pma_hw_dep_hwa_bit_to_id_tbl_v4h;
            g_pma_hw_dep_sysc_ref_tbl       = (uint64_t *)g_pma_hw_dep_sysc_ref_tbl_v4h;
            g_pma_hw_dep_cpg_ref_tbl        = (uint64_t *)g_pma_hw_dep_cpg_ref_tbl_v4h;
            g_pma_hw_dep_hwa_info_tbl       = (st_hwa_t *)g_pma_hw_dep_hwa_info_tbl_v4h;
            g_pma_hw_dep_hwa_dependency_tbl = (const st_hwa_dependent_flag_t *)g_pma_hw_dep_hwa_dependency_tbl_v4h;
            g_pma_hw_dep_max_sysc_id_num    = R_PMA_SYSC_DOMAIN_ID_V4H_NUM;
            g_pma_hw_dep_max_cpg_id_num     = R_PMA_CPG_DOMAIN_ID_V4H_NUM;
            break;

        case R_PMA_DEVICE_SOC_TYPE_V4M:
            g_pma_hw_dep_hwa_bit_num        = R_PMA_HW_DEP_HWA_DEPEND_V4M_NUM;
            g_pma_hw_dep_hwa_bit_tbl        = (const uint64_t *)g_pma_hw_dep_hwa_bit_tbl_v4m;
            g_pma_hw_dep_hwa_bit_to_id_tbl  = (const e_pma_hwa_id_t *)g_pma_hw_dep_hwa_bit_to_id_tbl_v4m;
            g_pma_hw_dep_sysc_ref_tbl       = (uint64_t *)g_pma_hw_dep_sysc_ref_tbl_v4m;
            g_pma_hw_dep_cpg_ref_tbl        = (uint64_t *)g_pma_hw_dep_cpg_ref_tbl_v4m;
            g_pma_hw_dep_hwa_info_tbl       = (st_hwa_t *)g_pma_hw_dep_hwa_info_tbl_v4m;
            g_pma_hw_dep_hwa_dependency_tbl = (const st_hwa_dependent_flag_t *)g_pma_hw_dep_hwa_dependency_tbl_v4m;
            g_pma_hw_dep_max_sysc_id_num    = R_PMA_SYSC_DOMAIN_ID_V4M_NUM;
            g_pma_hw_dep_max_cpg_id_num     = R_PMA_CPG_DOMAIN_ID_V4M_NUM;
            break;

        case R_PMA_DEVICE_SOC_TYPE_S4:
            g_pma_hw_dep_hwa_bit_num        = R_PMA_HW_DEP_HWA_DEPEND_S4_NUM;
            g_pma_hw_dep_hwa_bit_tbl        = (const uint64_t *)g_pma_hw_dep_hwa_bit_tbl_s4;
            g_pma_hw_dep_cpg_ref_tbl        = (uint64_t *)g_pma_hw_dep_cpg_ref_tbl_s4;
            g_pma_hw_dep_hwa_info_tbl       = (st_hwa_t *)g_pma_hw_dep_hwa_info_tbl_s4;
            g_pma_hw_dep_hwa_dependency_tbl = (const st_hwa_dependent_flag_t *)g_pma_hw_dep_hwa_dependency_tbl_s4;
            g_pma_hw_dep_max_cpg_id_num     = R_PMA_CPG_DOMAIN_ID_S4_NUM;
            /* S4 has no SYSC domain and HWA dependency */
            g_pma_hw_dep_hwa_bit_to_id_tbl  = NULL;
            g_pma_hw_dep_sysc_ref_tbl       = NULL;
            g_pma_hw_dep_max_sysc_id_num    = R_PMA_SYSC_DOMAIN_ID_S4_NUM;
            break;

        /* Unsuppoerted SoC */
        default:
            result = R_PMA_NG;
            break;
    }   /* end of switch (pma_soc_type) */

    if (R_PMA_OK == result)
    {
        /* PRQA S 1296,4527 1 # This implicit casting is no problem, because they are non-negative enum and positive integer, and this defines of minimum id vary from SoCs. */
        for (e_pma_hwa_id_t id = 0 ; id < g_pma_max_hwa_id_num ; id++)
        {
            result = R_PMA_HW_DEP_SetHwaState(id);

            if (R_PMA_OK == result)
            {
                result = R_PMA_HW_DEP_SetResetState(id);
            }
        }
        
        /* Set function pointer of callback for UpdateLowestPowerState */
        if (NULL != pFunc)
        {
            g_pma_hw_dep_callback_func = pFunc;
        }
        else
        {
            result = R_PMA_NG;
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}/* PRQA S 9104,9109 # The HW initialization function has already multiple sub-functions to reduce complexity.
                        We have decided that further division is not desirable from perspective of maintainability and readability.*/
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_SetHwaState()
* XOS3_OSAL_NOS_CD_CD_POW_189
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_189]
***********************************************************************************************************************/
static e_result_t R_PMA_HW_DEP_SetHwaState(e_pma_hwa_id_t hwaId)
{
    e_result_t      result;
    e_pma_onoff_t   clk_stat;
    e_pma_onoff_t   pwr_stat;
    
    result = R_PMA_HW_DEP_GetHwaState(hwaId, &clk_stat, &pwr_stat);

    if (R_PMA_OK == result)
    {
        if ((R_PMA_STATE_ON == pwr_stat)
            && (NULL != g_pma_hw_dep_sysc_ref_tbl)
            && (R_PMA_SYSC_DOMAIN_ID_NONE != g_pma_hw_dep_hwa_info_tbl[hwaId].sysc_id))
        {
            R_PMA_HW_DEP_NoticeToSyscOn(hwaId, g_pma_hw_dep_hwa_info_tbl[hwaId].sysc_id);
        }
        
        if ((R_PMA_STATE_ON == clk_stat)
            && (R_PMA_CPG_DOMAIN_ID_NONE != g_pma_hw_dep_hwa_info_tbl[hwaId].cpg_id))
        {
            R_PMA_HW_DEP_NoticeToCpgOn(hwaId, g_pma_hw_dep_hwa_info_tbl[hwaId].cpg_id);
        }
    }
    
    if (R_PMA_BSP_ECPG == result)
    {
        if ((R_PMA_STATE_ON == pwr_stat)
            && (NULL != g_pma_hw_dep_sysc_ref_tbl)
            && (R_PMA_SYSC_DOMAIN_ID_NONE != g_pma_hw_dep_hwa_info_tbl[hwaId].sysc_id))
        {
            R_PMA_HW_DEP_NoticeToSyscOn(hwaId, g_pma_hw_dep_hwa_info_tbl[hwaId].sysc_id);
        }
        result = R_PMA_OK;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_SetHwaState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_SetResetState()
* XOS3_OSAL_NOS_CD_CD_POW_190
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_190]
***********************************************************************************************************************/
static e_result_t R_PMA_HW_DEP_SetResetState(e_pma_hwa_id_t hwaId)
{
    e_result_t      result = R_PMA_OK;
    e_pma_reset_req_t   rst_stat;
    
    if (R_PMA_CPG_DOMAIN_ID_NONE != g_pma_hw_dep_hwa_info_tbl[hwaId].reset_id)
    {
        /* Set Reset state */
        result = R_PMA_BSP_WRAP_GetResetState(g_pma_hw_dep_hwa_info_tbl[hwaId].reset_id, &rst_stat);
        if (R_PMA_OK == result)
        {
            g_pma_hw_dep_hwa_info_tbl[hwaId].actual_rst_stat = rst_stat;
        }
    }
    else
    {
        g_pma_hw_dep_hwa_info_tbl[hwaId].actual_rst_stat = R_PMA_UNDEFINED_RESET;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_SetResetState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_GetHwaState()
* XOS3_OSAL_NOS_CD_CD_POW_124
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_124]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
e_result_t R_PMA_HW_DEP_GetHwaState(e_pma_hwa_id_t hwaId,
                                    e_pma_onoff_t *pClkStat,
                                    e_pma_onoff_t *pPwrStat)
{
    e_result_t          result;
    e_pma_power_onoff_t pow_state;
    e_pma_clock_onoff_t clk_state;

    if ((g_pma_max_hwa_id_num > hwaId)
        && (NULL != pClkStat)
        && (NULL != pPwrStat))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if (R_PMA_OK == result)
    {
        /* Get Power status */
        if (R_PMA_SYSC_DOMAIN_ID_NONE != g_pma_hw_dep_hwa_info_tbl[hwaId].sysc_id)
        {
            result = R_PMA_BSP_WRAP_GetSyscState(g_pma_hw_dep_hwa_info_tbl[hwaId].sysc_id, &pow_state);
            if (R_PMA_OK == result)
            {
                if (R_PMA_SYSC_POWER_ON == pow_state)
                {
                    *pPwrStat = R_PMA_STATE_ON;
                }
                else
                {
                    *pPwrStat = R_PMA_STATE_OFF;
                }
            }
        }
        else
        {
            *pPwrStat = R_PMA_STATE_ON;
        }
    }   /* end of if (R_PMA_OK == result) */

    if (R_PMA_OK == result)
    {
        if ( R_PMA_CPG_DOMAIN_ID_NONE != g_pma_hw_dep_hwa_info_tbl[hwaId].cpg_id)
        {
            /* Get Clock status */
            result = R_PMA_BSP_WRAP_GetCpgState(g_pma_hw_dep_hwa_info_tbl[hwaId].cpg_id, &clk_state);
            if (R_PMA_OK == result)
            {
                if (R_PMA_CPG_CLOCK_ON == clk_state)
                {
                    *pClkStat = R_PMA_STATE_ON;
                }
                else
                {
                    *pClkStat = R_PMA_STATE_OFF;
                }
            }
        }
        else
        {
            /* HWA without MSTP, the power state and CPG state are linked */
            *pClkStat = *pPwrStat;
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_GetHwaState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_GetHwaObject()
* XOS3_OSAL_NOS_CD_CD_POW_125
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_125]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_GetHwaObject(e_pma_hwa_id_t hwaId,
                                     st_hwa_t     **pHwa)
{
    e_result_t result = R_PMA_OK;

    if (NULL != g_pma_hw_dep_hwa_info_tbl)
    {
        if ((g_pma_max_hwa_id_num > hwaId)
            && (NULL != pHwa))
        {
            *pHwa = &g_pma_hw_dep_hwa_info_tbl[hwaId];
        }
        else
        {
            result = R_PMA_INVALID_ARGUMENT;
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_GetHwaObject()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_ApplyReset()
* XOS3_OSAL_NOS_CD_CD_POW_126
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_126]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_ApplyReset(e_pma_hwa_id_t hwaId)
{
    e_result_t        result;
    e_cpg_domain_id_t reset_id;

    if (g_pma_max_hwa_id_num > hwaId)
    {
        /* Check parent HWA status */
        result = R_PMA_HW_DEP_CheckResetDepParent(hwaId);

        if (R_PMA_OK == result)
        {
            /* Check child HWA status */
            result = R_PMA_HW_DEP_CheckResetDepChild(hwaId);

            if (R_PMA_OK == result)
            {
                /* Assert */
                reset_id = g_pma_hw_dep_hwa_info_tbl[hwaId].reset_id;
                result   = R_PMA_BSP_WRAP_AssertReset(reset_id);
                if (R_PMA_OK == result)
                {
                    g_pma_hw_dep_hwa_info_tbl[hwaId].actual_rst_stat = R_PMA_APPLIED_RESET;
                }
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if (g_pma_max_hwa_id_num > hwaId) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_ApplyReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_ReleaseReset()
* XOS3_OSAL_NOS_CD_CD_POW_127
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_127]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_ReleaseReset(e_pma_hwa_id_t hwaId)
{
    e_result_t        result;
    e_cpg_domain_id_t reset_id;

    if (g_pma_max_hwa_id_num > hwaId)
    {
        /* Check parent HWA status */
        result = R_PMA_HW_DEP_CheckResetDepParent(hwaId);

        if (R_PMA_OK == result)
        {
            /* Check child HWA status */
            result = R_PMA_HW_DEP_CheckResetDepChild(hwaId);

            if (R_PMA_OK == result)
            {
                /* Deassert */
                reset_id = g_pma_hw_dep_hwa_info_tbl[hwaId].reset_id;
                result   = R_PMA_BSP_WRAP_DeassertReset(reset_id);
                if (R_PMA_OK == result)
                {
                    g_pma_hw_dep_hwa_info_tbl[hwaId].actual_rst_stat = R_PMA_RELEASED_RESET;
                }
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if (g_pma_max_hwa_id_num > hwaId) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_ReleaseReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToPGforTypeA()
* XOS3_OSAL_NOS_CD_CD_POW_128
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_128]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforTypeA(e_pma_hwa_id_t refHwaId,
                                                 e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOff(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG first */
        (void)R_PMA_HW_DEP_SetCpgOff(cpg_id);
        result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

        /* Control SYSC secondly */
        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_SetSyscOff(sysc_id);

            if (R_PMA_OK == result)
            {
                /* Control CPG finally */
                result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

                if (R_PMA_OK == result)
                {
                    /* Callback to UpdateLowestPowerState */
                    result = g_pma_hw_dep_callback_func(targetHwaId);
                }
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId)... */

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToPG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToPGforTypeA()
***********************************************************************************************************************/


/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR()
* XOS3_OSAL_NOS_CD_CD_POW_199
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_199]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR(e_pma_hwa_id_t refHwaId,
                                                 e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOff(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control SYSC first*/
        result = R_PMA_HW_DEP_SetSyscOff(sysc_id);

        /* Control CPG secondly */
        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_SetCpgOff(cpg_id);


            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId)... */

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToPG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToPGforV3xA3IR()
***********************************************************************************************************************/


/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToPGforTypeB()
* XOS3_OSAL_NOS_CD_CD_POW_129
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_129]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforTypeB(e_pma_hwa_id_t refHwaId,
                                                 e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOff(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG first */
        result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

        /* Control SYSC secondly */
        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_SetSyscOff(sysc_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId)... */

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToPG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToPGforTypeB()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToPGforTypeC()
* XOS3_OSAL_NOS_CD_CD_POW_130
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_130]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforTypeC(e_pma_hwa_id_t refHwaId,
                                                 e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOff(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG first. It must be operated while Power is ON according to HW team */
        result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

        /* Control SYSC secondly */
        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_SetSyscOff(sysc_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId)... */

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToPG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
             }
        }
    }
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToPGforTypeC()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC()
* XOS3_OSAL_NOS_CD_CD_POW_131
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_131]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC(e_pma_hwa_id_t refHwaId,
                                                    e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG */
        result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId)... */

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToPG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPforTypeAC()
* XOS3_OSAL_NOS_CD_CD_POW_132
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_132]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforTypeAC(e_pma_hwa_id_t refHwaId,
                                                  e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToHP(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
             }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOn(refHwaId, cpg_id);

        /* Control SYSC first */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Control CPG secondly. It must be operated while Power is ON according to HW team */
            result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPforTypeAC()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR()
* XOS3_OSAL_NOS_CD_CD_POW_197
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_197]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR(e_pma_hwa_id_t refHwaId,
                                                 e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToHP(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOn(refHwaId, cpg_id);

        /* Control CPG first */
        result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Control SYSC secondly */
            result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPforV3xA3IR()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPforTypeB()
* XOS3_OSAL_NOS_CD_CD_POW_133
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_133]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforTypeB(e_pma_hwa_id_t refHwaId,
                                                 e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToHP(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOn(refHwaId, cpg_id);

        /* Control SYSC first */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Control CPG secondly */
            result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPforTypeB()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC()
* XOS3_OSAL_NOS_CD_CD_POW_134
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_134]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC(e_pma_hwa_id_t refHwaId,
                                                    e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToHP(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToCpgOn(refHwaId, cpg_id);

        /* Control CPG */
        result = R_PMA_HW_DEP_SetCpgOn(cpg_id);
        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC()
* XOS3_OSAL_NOS_CD_CD_POW_135
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_135]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC(e_pma_hwa_id_t refHwaId,
                                                     e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG */
        result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId)... */

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeHPtoCG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC()
* XOS3_OSAL_NOS_CD_CD_POW_136
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_136]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC(e_pma_hwa_id_t refHwaId,
                                                    e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG */
        result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId)... */

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeHPtoCG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }

    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC()
* XOS3_OSAL_NOS_CD_CD_POW_137
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_137]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC(e_pma_hwa_id_t refHwaId,
                                                    e_pma_hwa_id_t targetHwaId)
{
    e_result_t          result;
    e_sysc_domain_id_t  sysc_id;
    e_cpg_domain_id_t   cpg_id;
    e_pma_hwa_id_t      hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangePGtoCG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);

        /* Control SYSC secondly */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Control CPG finally */
            result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR()
* XOS3_OSAL_NOS_CD_CD_POW_198
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_198]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR(e_pma_hwa_id_t refHwaId,
                                                   e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangePGtoCG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;
        
        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);

        result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Control SYSC secondly */
            result = R_PMA_HW_DEP_SetSyscOn(sysc_id);
        }

        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_SetCpgOff(cpg_id);
        }

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangePGtoCGforV3xA3IR()
***********************************************************************************************************************/



/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB()
* XOS3_OSAL_NOS_CD_CD_POW_138
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_138]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB(e_pma_hwa_id_t refHwaId,
                                                   e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangePGtoCG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);

        /* Control SYSC secondly */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeToCpgOn()
* XOS3_OSAL_NOS_CD_CD_POW_139
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_139]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static void R_PMA_HW_DEP_NoticeToCpgOn(e_pma_hwa_id_t    hwaId,
                                       e_cpg_domain_id_t cpgId)
{
    uint64_t bit;
    uint64_t group;
    uint64_t flag;

    bit = g_pma_hw_dep_hwa_bit_tbl[hwaId];
    /* PRQA S 2790 1 # QAC false indication (CCT change). */
    group = bit >> R_PMA_HW_DEP_HWA_GROUP_MASK;
    /* PRQA S 2985 1 # QAC false indication (CCT change). */
    flag  = bit & (uint64_t)R_PMA_HW_DEP_HWA_BIT_MASK;
    /* PRQA S 2986 1 # QAC false indication (CCT change). */
    g_pma_hw_dep_cpg_ref_tbl[(group * (uint64_t)g_pma_hw_dep_max_cpg_id_num) + (uint64_t)cpgId]  |= flag;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeToCpgOn()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeToCpgOff()
* XOS3_OSAL_NOS_CD_CD_POW_140
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_140]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static void R_PMA_HW_DEP_NoticeToCpgOff(e_pma_hwa_id_t    hwaId,
                                        e_cpg_domain_id_t cpgId)
{
    uint64_t bit;
    uint64_t group;
    uint64_t flag;

    bit = g_pma_hw_dep_hwa_bit_tbl[hwaId];
    /* PRQA S 2790 1 # QAC false indication (CCT change). */
    group = bit >> R_PMA_HW_DEP_HWA_GROUP_MASK;
    /* PRQA S 2985 1 # QAC false indication (CCT change). */
    flag  = bit & (uint64_t)R_PMA_HW_DEP_HWA_BIT_MASK;

    /* PRQA S 2986 1 # QAC false indication (CCT change). */
    g_pma_hw_dep_cpg_ref_tbl[(group * (uint64_t)g_pma_hw_dep_max_cpg_id_num) + (uint64_t)cpgId] &= ~flag;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeToCpgOff()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeToSyscOn()
* XOS3_OSAL_NOS_CD_CD_POW_141
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_141]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static void R_PMA_HW_DEP_NoticeToSyscOn(e_pma_hwa_id_t     hwaId,
                                        e_sysc_domain_id_t syscId)
{
    uint64_t bit;
    uint64_t group;
    uint64_t flag;
    
    bit = g_pma_hw_dep_hwa_bit_tbl[hwaId];
    /* PRQA S 2790 1 # QAC false indication (CCT change). */
    group = bit >> R_PMA_HW_DEP_HWA_GROUP_MASK;
    /* PRQA S 2985 1 # QAC false indication (CCT change). */
    flag  = bit & (uint64_t)R_PMA_HW_DEP_HWA_BIT_MASK;

    /* PRQA S 2986 1 # QAC false indication (CCT change). */
    g_pma_hw_dep_sysc_ref_tbl[(group * (uint64_t)g_pma_hw_dep_max_sysc_id_num) + (uint64_t)syscId] |= flag;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeToSyscOn()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeToSyscOff()
* XOS3_OSAL_NOS_CD_CD_POW_142
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_142]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static void R_PMA_HW_DEP_NoticeToSyscOff(e_pma_hwa_id_t     hwaId,
                                         e_sysc_domain_id_t syscId)
{
    uint64_t bit;
    uint64_t group;
    uint64_t flag;

    bit = g_pma_hw_dep_hwa_bit_tbl[hwaId];
    /* PRQA S 2790 1 # QAC false indication (CCT change). */
    group = bit >> R_PMA_HW_DEP_HWA_GROUP_MASK;
    /* PRQA S 2985 1 # QAC false indication (CCT change). */
    flag  = bit & (uint64_t)R_PMA_HW_DEP_HWA_BIT_MASK;

    /* PRQA S 2986 1 # QAC false indication (CCT change). */
    g_pma_hw_dep_sysc_ref_tbl[(group * (uint64_t)g_pma_hw_dep_max_sysc_id_num) + (uint64_t)syscId] &= ~flag;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeToSyscOff()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_SetCpgOn()
* XOS3_OSAL_NOS_CD_CD_POW_143
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_143]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static e_result_t R_PMA_HW_DEP_SetCpgOn(e_cpg_domain_id_t modId)
{
    e_result_t result;

    if (modId < g_pma_hw_dep_max_cpg_id_num)
    {
        result = R_PMA_BSP_WRAP_SetCpgON(modId);
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_SetCpgOn()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_SetCpgOff()
* XOS3_OSAL_NOS_CD_CD_POW_144
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_144]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static e_result_t R_PMA_HW_DEP_SetCpgOff(e_cpg_domain_id_t modId)
{
    e_result_t result   = R_PMA_OK;

    if (modId < g_pma_hw_dep_max_cpg_id_num)
    {
        /* Check whether target domain is referenced by any HWA */
        if ((0U == g_pma_hw_dep_cpg_ref_tbl[modId])
            && (0U == g_pma_hw_dep_cpg_ref_tbl[(uint64_t)g_pma_hw_dep_max_cpg_id_num + (uint64_t)modId]))
        {
            result = R_PMA_BSP_WRAP_SetCpgOFF(modId);
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_SetCpgOff()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_SetSyscOn()
* XOS3_OSAL_NOS_CD_CD_POW_145
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_145]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static e_result_t R_PMA_HW_DEP_SetSyscOn(e_sysc_domain_id_t modId)
{
    e_result_t result;

    if (modId < g_pma_hw_dep_max_sysc_id_num)
    {
        result = R_PMA_BSP_WRAP_SetSyscON(modId);
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_SetSyscOn()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_SetSyscOff()
* XOS3_OSAL_NOS_CD_CD_POW_146
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_146]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static e_result_t R_PMA_HW_DEP_SetSyscOff(e_sysc_domain_id_t modId)
{
    e_result_t result   = R_PMA_OK;

    if (modId < g_pma_hw_dep_max_sysc_id_num)
    {
        /* Check whether target domain is referenced by any HWA */
        if ((0U == g_pma_hw_dep_sysc_ref_tbl[modId])
            && (0U == g_pma_hw_dep_sysc_ref_tbl[(uint64_t)g_pma_hw_dep_max_sysc_id_num + (uint64_t)modId]))
        {
            result = R_PMA_BSP_WRAP_SetSyscOFF(modId);
        }
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_SetSyscOff()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_CheckResetDepChild()
* XOS3_OSAL_NOS_CD_CD_POW_147
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_147]
***********************************************************************************************************************/
static e_result_t R_PMA_HW_DEP_CheckResetDepChild(e_pma_hwa_id_t hwaId)
{
    e_result_t        result;
    e_cpg_domain_id_t cpg_id;
    e_pma_hwa_id_t    child_id;

    if (hwaId < g_pma_max_hwa_id_num)
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    /* Refer to HWA dependency table to Check dependency */
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[hwaId].children))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[hwaId].children & (uint64_t)(1ULL << bit)))
            {
                /* Check reset status */
                child_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                if ((R_PMA_APPLIED_RESET != g_pma_hw_dep_hwa_info_tbl[child_id].actual_rst_stat)
                    && (P_PMA_CPG_NO_DEPEND != g_pma_hw_dep_hwa_info_tbl[child_id].cpg_dep_flag))
                {
                    /* If reset is deasserted, check clock status next */
                    cpg_id = g_pma_hw_dep_hwa_info_tbl[child_id].cpg_id;

                    /* If clock is ON, reset control cannot be done */
                    if ((R_PMA_CPG_DOMAIN_ID_NONE != cpg_id)
                        && ((0U != g_pma_hw_dep_cpg_ref_tbl[cpg_id])
                        || (0U != g_pma_hw_dep_cpg_ref_tbl[(uint64_t)g_pma_hw_dep_max_cpg_id_num + (uint64_t)cpg_id])))
                    {
                        result = R_PMA_DEPENDENCY;
                        /* go to return */
                        break;
                    }
                }
            }
        }   /* end of for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++) */
    }   /* end of if ((R_PMA_OK == result) */

    return result;
}/* PRQA S 9110 # It is a series of processes and is necessary so as not to reduce the readability based on design */
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_CheckResetDepChild()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_CheckResetDepParent()
* XOS3_OSAL_NOS_CD_CD_POW_148
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_148]
***********************************************************************************************************************/
static e_result_t R_PMA_HW_DEP_CheckResetDepParent(e_pma_hwa_id_t hwaId)
{
    e_result_t        result;
    e_cpg_domain_id_t cpg_id;
    e_pma_hwa_id_t    parent_id;

    if (hwaId < g_pma_max_hwa_id_num)
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    /* Refer to HWA dependency table to Check dependency */
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[hwaId].parent)
        && (P_PMA_CPG_NO_DEPEND != g_pma_hw_dep_hwa_info_tbl[hwaId].cpg_dep_flag))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[hwaId].parent & (uint64_t)(1ULL << bit)))
            {
                /* Check reset status */
                parent_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];

                if ((R_PMA_RELEASED_RESET == g_pma_hw_dep_hwa_info_tbl[parent_id].actual_rst_stat)
                    || (R_PMA_CPG_DOMAIN_ID_NONE == g_pma_hw_dep_hwa_info_tbl[parent_id].reset_id))
                {
                    /* If reset is deasserted, check clock status next */
                    cpg_id = g_pma_hw_dep_hwa_info_tbl[parent_id].cpg_id;

                    /* If clock is OFF, reset control cannot be done */
                    if ((P_PMA_CPG_NO_DEPEND != g_pma_hw_dep_hwa_info_tbl[parent_id].cpg_dep_flag)
                        && (R_PMA_CPG_DOMAIN_ID_NONE != cpg_id)
                        && (0U == g_pma_hw_dep_cpg_ref_tbl[cpg_id])
                        && (0U == g_pma_hw_dep_cpg_ref_tbl[(uint64_t)g_pma_hw_dep_max_cpg_id_num + (uint64_t)cpg_id]))
                    {
                        result = R_PMA_DEPENDENCY;
                    }
                }
                else
                {
                    /* If reset is asserted, reset control cannot be done */
                    result = R_PMA_DEPENDENCY;
                }

                if (R_PMA_DEPENDENCY == result)
                {
                    /* go to return */
                    break;
                }
            }   /* end of if (0U != (g_pma_hw_dep_hwa_dependency_tbl[hwaId].parent & (uint64_t)(1ULL << bit))) */
        }   /* end of for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++) */
    }   /* end of if ((R_PMA_OK == result)... */

    return result;
}/* PRQA S 9110 # It is a series of processes and is necessary so as not to reduce the readability based on design */
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_CheckResetDepParent()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H()
* XOS3_OSAL_NOS_CD_CD_POW_178
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_178]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H(e_pma_hwa_id_t refHwaId,
                                                    e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_cpg_domain_id_t  cpg_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        cpg_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOff(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A23DGB);
        R_PMA_HW_DEP_NoticeToSyscOff(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A33DGA);
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG first */
        result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

        /* Control SYSC secondly */
        if (R_PMA_OK == result)
        {
            result = R_PMA_HW_DEP_SetSyscOff(R_PMA_SYSC_DOMAIN_ID_V4H_A23DGB);

            if (R_PMA_OK == result)
            {
                /* Control SYSC finally */
                result = R_PMA_HW_DEP_SetSyscOff(R_PMA_SYSC_DOMAIN_ID_V4H_A33DGA);

                if (R_PMA_OK == result)
                {
                    /* Callback to UpdateLowestPowerState */
                    result = g_pma_hw_dep_callback_func(targetHwaId);
                }
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId) */
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H()
* XOS3_OSAL_NOS_CD_CD_POW_179
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_179]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H(e_pma_hwa_id_t refHwaId,
                                                    e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result = R_PMA_OK;
    e_cpg_domain_id_t  cpg_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        cpg_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice ON event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A33DGA);
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A23DGB);
        R_PMA_HW_DEP_NoticeToCpgOn(refHwaId, cpg_id);
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if (R_PMA_OK == result)
    {
        /* Control SYSC first */
        result = R_PMA_HW_DEP_SetSyscOn(R_PMA_SYSC_DOMAIN_ID_V4H_A33DGA);
        if (R_PMA_OK == result)
        {
            /* Control SYSC secondly */
            result = R_PMA_HW_DEP_SetSyscOn(R_PMA_SYSC_DOMAIN_ID_V4H_A23DGB);
            if (R_PMA_OK == result)
            {
                /* Control CPG finally */
                result = R_PMA_HW_DEP_SetCpgOn(cpg_id);
                if (R_PMA_OK == result)
                {
                    /* Callback to UpdateLowestPowerState */
                    result = g_pma_hw_dep_callback_func(targetHwaId);
                }
            }
        }
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H()
* XOS3_OSAL_NOS_CD_CD_POW_180
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_180]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H(e_pma_hwa_id_t refHwaId,
                                                      e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_cpg_domain_id_t  cpg_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        cpg_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG */
        result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId) */
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H()
* XOS3_OSAL_NOS_CD_CD_POW_181
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_181]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H(e_pma_hwa_id_t refHwaId,
                                                      e_pma_hwa_id_t targetHwaId)
{
    e_result_t  result;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A33DGA);
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A23DGB);

        /* Control SYSC first */
        result = R_PMA_HW_DEP_SetSyscOn(R_PMA_SYSC_DOMAIN_ID_V4H_A33DGA);

        if (R_PMA_OK == result)
        {
            /* Control SYSC secondly */
            result = R_PMA_HW_DEP_SetSyscOn(R_PMA_SYSC_DOMAIN_ID_V4H_A23DGB);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3()
* XOS3_OSAL_NOS_CD_CD_POW_182
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_182]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3(e_pma_hwa_id_t refHwaId,
                                                       e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOff(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG first */
        result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Control SYSC secondly */
            result = R_PMA_HW_DEP_SetSyscOff(sysc_id);

            if (R_PMA_OK == result)
            {
                /* Control CPG finally */
                result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

                if (R_PMA_OK == result)
                {
                    /* Callback to UpdateLowestPowerState */
                    result = g_pma_hw_dep_callback_func(targetHwaId);
                }
            }
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId) */

    if ((R_PMA_OK == result)
    && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToPG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3()
* XOS3_OSAL_NOS_CD_CD_POW_183
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_183]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3(e_pma_hwa_id_t refHwaId,
                                                        e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToHP(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
             }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOn(refHwaId, cpg_id);

        /* Control CPG first */
        result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Control SYSC secondly */
            result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3()
* XOS3_OSAL_NOS_CD_CD_POW_184
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_184]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3(e_pma_hwa_id_t refHwaId,
                                                          e_pma_hwa_id_t targetHwaId)
{
    e_result_t          result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t      hwa_id;
    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangePGtoCG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);

        /* Control CPG first */
        result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Control SYSC secondly */
            result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

            if (R_PMA_OK == result)
            {
                /* Control CPG finally */
                result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

                if (R_PMA_OK == result)
                {
                    /* Callback to UpdateLowestPowerState */
                    result = g_pma_hw_dep_callback_func(targetHwaId);
                }
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG()
* XOS3_OSAL_NOS_CD_CD_POW_185
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_185]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG(e_pma_hwa_id_t refHwaId,
                                                   e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOff(refHwaId, sysc_id);

        /* Control SYSC */
        result = R_PMA_HW_DEP_SetSyscOff(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId)... */

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToPG(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
             }
        }
    }
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG()
* XOS3_OSAL_NOS_CD_CD_POW_186
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_186]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG(e_pma_hwa_id_t refHwaId,
                                                   e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_pma_hwa_id_t     hwa_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangeToHP(refHwaId, hwa_id);
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
             }
        }
    }

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);

        /* Control SYSC */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_GetResetState()
* XOS3_OSAL_NOS_CD_CD_POW_188
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_188]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_GetResetState(e_pma_hwa_id_t hwaId,
                                      e_pma_reset_req_t *pRstStat)
{
    e_result_t result = R_PMA_OK;

    if ((g_pma_max_hwa_id_num > hwaId)
        && (NULL != pRstStat))
    {
        *pRstStat = g_pma_hw_dep_hwa_info_tbl[hwaId].actual_rst_stat;
    }
    else
    {
        result = R_PMA_NG;
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_GetResetState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPforVDSP()
* XOS3_OSAL_NOS_CD_CD_POW_191
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_191]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforVDSP(e_pma_hwa_id_t refHwaId,
                                                e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;
    e_sysc_domain_id_t parent_sysc_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }
    
    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                /* Get parent's  ID from table */
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                parent_sysc_id  = g_pma_hw_dep_hwa_info_tbl[hwa_id].sysc_id;
                
                switch(g_pma_hw_dep_sysc_ref_tbl[parent_sysc_id])
                {
                    case 0U:
                        /* If power is OFF, set CG MODE */
                        result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangePGtoCG(refHwaId, hwa_id);
                        break;
                    default:
                        /* If power is ON, only update the SYSC reference flag */
                        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A2CN0);
                        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A3IR);
                        break;
                }
                
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }   /* end of if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit))) */
        }   /* end of for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++) */
    }   /* end of if ((R_PMA_OK == result) */

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOn(refHwaId, cpg_id);

        /* Control SYSC first */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Control CPG secondly. It must be operated while Power is ON according to HW team */
            result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPforVDSP()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP()
* XOS3_OSAL_NOS_CD_CD_POW_192
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_192]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP(e_pma_hwa_id_t refHwaId,
                                                  e_pma_hwa_id_t targetHwaId)
{
    e_result_t          result;
    e_sysc_domain_id_t  sysc_id;
    e_cpg_domain_id_t   cpg_id;
    e_pma_hwa_id_t      hwa_id;
    e_sysc_domain_id_t  parent_sysc_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                /* Get parent's  ID from table */
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                parent_sysc_id  = g_pma_hw_dep_hwa_info_tbl[hwa_id].sysc_id;
                
                switch(g_pma_hw_dep_sysc_ref_tbl[parent_sysc_id])
                {
                    case 0U:
                        /* If power is OFF, set CG MODE */
                        result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangePGtoCG(refHwaId, hwa_id);
                        break;
                    default:
                        /* If power is ON, only update the SYSC reference flag */
                        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A2CN0);
                        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4H_A3IR);
                        break;
                }
                
                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }   /* end of if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit))) */
        }   /* end of for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++) */
    }   /* end of if ((R_PMA_OK == result) */

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);

        /* Control SYSC secondly */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Control CPG finally */
            result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangePGtoCGforVDSPofV4M()
* XOS3_OSAL_NOS_CD_CD_POW_222
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_304]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangePGtoCGforVDSPofV4M(e_pma_hwa_id_t refHwaId,
                                                  e_pma_hwa_id_t targetHwaId)
{
    e_result_t          result;
    e_sysc_domain_id_t  sysc_id;
    e_cpg_domain_id_t   cpg_id;
    e_pma_hwa_id_t      hwa_id;
    e_sysc_domain_id_t  parent_sysc_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                /* Get parent's  ID from table */
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                parent_sysc_id  = g_pma_hw_dep_hwa_info_tbl[hwa_id].sysc_id;

                switch(g_pma_hw_dep_sysc_ref_tbl[parent_sysc_id])
                {
                    case 0U:
                        /* If power is OFF, set CG MODE */
                        result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangePGtoCG(refHwaId, hwa_id);
                        break;
                    default:
                        /* If power is ON, only update the SYSC reference flag */
                        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4M_A2CN0);
                        break;
                }

                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }   /* end of if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit))) */
        }   /* end of for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++) */
    }   /* end of if ((R_PMA_OK == result) */

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);

        /* Control SYSC secondly */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Control CPG finally */
            result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangePGtoCGforVDSPofV4M()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeToHPforVDSPofV4M()
* XOS3_OSAL_NOS_CD_CD_POW_223
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_305]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeToHPforVDSPofV4M(e_pma_hwa_id_t refHwaId,
                                                e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_sysc_domain_id_t sysc_id;
    e_cpg_domain_id_t  cpg_id;
    e_pma_hwa_id_t     hwa_id;
    e_sysc_domain_id_t parent_sysc_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_NG;
    }

    if ((R_PMA_OK == result)
        && (0U != g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent))
    {
        for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
        {
            if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit)))
            {
                /* Get parent's  ID from table */
                hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[bit];
                parent_sysc_id  = g_pma_hw_dep_hwa_info_tbl[hwa_id].sysc_id;

                switch(g_pma_hw_dep_sysc_ref_tbl[parent_sysc_id])
                {
                    case 0U:
                        /* If power is OFF, set CG MODE */
                        result = g_pma_hw_dep_hwa_info_tbl[hwa_id].noticeChangePGtoCG(refHwaId, hwa_id);
                        break;
                    default:
                        /* If power is ON, only update the SYSC reference flag */
                        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, R_PMA_SYSC_DOMAIN_ID_V4M_A2CN0);
                        break;
                }

                if (R_PMA_OK != result)
                {
                    /* If error occur, go to return */
                    break;
                }
            }   /* end of if (0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].parent & (uint64_t)(1ULL << bit))) */
        }   /* end of for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++) */
    }   /* end of if ((R_PMA_OK == result) */

    if (R_PMA_OK == result)
    {
        /* Get domain ID from table */
        sysc_id = g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id;
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToSyscOn(refHwaId, sysc_id);
        R_PMA_HW_DEP_NoticeToCpgOn(refHwaId, cpg_id);

        /* Control SYSC first */
        result = R_PMA_HW_DEP_SetSyscOn(sysc_id);

        if (R_PMA_OK == result)
        {
            /* Control CPG secondly. It must be operated while Power is ON according to HW team */
            result = R_PMA_HW_DEP_SetCpgOn(cpg_id);

            if (R_PMA_OK == result)
            {
                /* Callback to UpdateLowestPowerState */
                result = g_pma_hw_dep_callback_func(targetHwaId);
            }
        }
    }   /* end of if (R_PMA_OK == result) */

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeToHPforVDSPofV4M()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP()
* XOS3_OSAL_NOS_CD_CD_POW_193
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_193]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP(e_pma_hwa_id_t refHwaId,
                                                  e_pma_hwa_id_t targetHwaId)
{
    e_result_t         result;
    e_cpg_domain_id_t  cpg_id;

    if ((g_pma_max_hwa_id_num > refHwaId)
       && (g_pma_max_hwa_id_num > targetHwaId))
    {
        /* Get domain ID from table */
        cpg_id  = g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id;

        /* Notice OFF event to domain */
        R_PMA_HW_DEP_NoticeToCpgOff(refHwaId, cpg_id);

        /* Control CPG */
        result = R_PMA_HW_DEP_SetCpgOff(cpg_id);

        if (R_PMA_OK == result)
        {
            /* Callback to UpdateLowestPowerState */
            result = g_pma_hw_dep_callback_func(targetHwaId);
        }
    }
    else
    {
        result = R_PMA_NG;
    }   /* end of if ((g_pma_max_hwa_id_num > refHwaId) */
    if (R_PMA_OK == result) 
    {
        result = R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(targetHwaId);
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_IsEnableControlPowerClock()
* XOS3_OSAL_NOS_CD_CD_POW_194 
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_194]
***********************************************************************************************************************/
e_result_t R_PMA_HW_DEP_IsEnableControlPowerClock(e_pma_hwa_id_t targetHwaId,
                                                  bool *isEnable)
{
    e_result_t result;

    if (g_pma_max_hwa_id_num > targetHwaId)
    {
        result = R_PMA_OK;
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }

    if (R_PMA_OK == result)
    {
        if ((R_PMA_SYSC_DOMAIN_ID_NONE != g_pma_hw_dep_hwa_info_tbl[targetHwaId].sysc_id)
            || (R_PMA_CPG_DOMAIN_ID_NONE != g_pma_hw_dep_hwa_info_tbl[targetHwaId].cpg_id))
        {
            *isEnable = true;
        }
        else
        {
            *isEnable = false;
        }
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_IsEnableControlPowerClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power()
* XOS3_OSAL_NOS_CD_CD_POW_196 
* [Covers: XOS2_OSAL_QNX_UD_DD_POW_125]
* [Covers: XOS3_OSAL_QNX_UD_DD_POW_196]
***********************************************************************************************************************/
/* PRQA S 9106 1 # Since it is created as a common function, calls from multiple functions are expected */
static e_result_t R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(e_pma_hwa_id_t targetHwaId)
{
    uint64_t       bit_num;
    uint64_t       group;
    e_result_t     result = R_PMA_OK;
    e_pma_hwa_id_t share_hwa_id;

    bit_num = g_pma_hw_dep_hwa_bit_tbl[targetHwaId];
    /* PRQA S 2790 1 # QAC false indication (CCT change). */
    group = bit_num >> R_PMA_HW_DEP_HWA_GROUP_MASK;

    for (uint64_t bit = 0; bit < g_pma_hw_dep_hwa_bit_num; bit++)
    {
        if ((0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].share_sysc_domain & (uint64_t)(1ULL << bit)))
            ||( 0U != (g_pma_hw_dep_hwa_dependency_tbl[targetHwaId].share_cpg_domain & (uint64_t)(1ULL << bit))))
        {
            /* PRQA S 2986 1 # QAC false indication (CCT change). */
            share_hwa_id = g_pma_hw_dep_hwa_bit_to_id_tbl[(uint64_t)(group * g_pma_hw_dep_hwa_bit_num) + (uint64_t)bit];
            result = g_pma_hw_dep_callback_func(share_hwa_id);
            if (R_PMA_OK != result)
            {
                /* If error occur, go to return */
                break;
            }
        }
    }

    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power()
***********************************************************************************************************************/
