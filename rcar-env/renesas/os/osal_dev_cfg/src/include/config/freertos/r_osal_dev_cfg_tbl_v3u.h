/***********************************************************************************************************************
* Copyright [2020-2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_dev_cfg_v3u.h
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car V3U
* Description :  OSAL device configuration file for R-Car V3U
***********************************************************************************************************************/
#ifndef OSAL_DEV_CFG_V3U_H
#define OSAL_DEV_CFG_V3U_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal_dev_cfg/qnx/r_osal_dev_cfg_info.h"


/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_QNX_DEV_CFG
 * @defgroup OSAL_Private_Defines_Static_Variables_DevConfig Device Condifurator Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_int_ims
 * osal device configuration : IMS interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ims[2][1] =
{
    {
        {150}
    },
    {
        {151}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_imr
 * osal device configuration : IMR interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imr[4][1] =
{
    {
        {146}
    },
    {
        {147}
    },
    {
        {148}
    },
    {
        {149}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_dof
 * osal device configuration : DOF interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_dof[2][2] =
{
    {
        {570}, {571}
    },
    {
        {572}, {573}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_stv
 * osal device configuration : STV interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_stv[2][2] =
{
    {
        {554}, {555}
    },
    {
        {556}, {557}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_acf
 * osal device configuration : ACF interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_acf[4][2] =
{
    {
        {544}, {545}
    },
    {
        {546}, {547}
    },
    {
        {548}, {549}
    },
    {
        {550}, {551}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_iccom
 * osal device configuration : ICCOM interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_iccom[8] =
{
    {440}, {441}, {442}, {443}, {444}, {445}, {446}, {447}
};

/*******************************************************************************************************************//**
 * @var gs_int_ivcp1e
 * osal device configuration : IVCP1E interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ivcp1e[2] =
{
    {133}, {134}
};

/*******************************************************************************************************************//**
 * @var gs_int_vcp4
 * osal device configuration : VCP4 interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_vcp4[2] =
{
    {574}, {575}
};

/*******************************************************************************************************************//**
 * @var gs_int_imp_top
 * osal device configuration : IMP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imp_top[7] =
{
    {576}, {577}, {578}, {579}, {580}, {581}, {582}
};

/*******************************************************************************************************************//**
 * @var gs_int_imp_ldmac_master
 * osal device configuration : IMP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imp_ldmac_master[1] =
{
    {584}
};

/*******************************************************************************************************************//**
 * @var gs_int_imp_radsp
 * osal device configuration : IMP RADSP interrupt
***********************************************************************************************************************/
  static st_osal_interrupt_info_t gs_int_imp_radsp[2][1] =
{
    {
        {585}
    },
    {
        {586}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_imp_radsp
 * osal device configuration : IMP RADSP interrupt
***********************************************************************************************************************/
  static st_osal_interrupt_info_t gs_int_imp_radsp_dmac[2][4] =
{
    {
        {880}, {881}, {882}, {883}
    },
    {
        {896}, {897}, {898}, {899}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_cisp
 * osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_cisp[4][1] =
{
    {
        {153}
    },
    {
        {155}
    },
    {
        {157}
    },
    {
        {159}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_tisp
 * osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_tisp[4][1] =
{
    {
        {152}
    },
    {
        {154}
    },
    {
        {156}
    },
    {
        {158}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_vspx
 * osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_vspx[4][1] =
{
    {
        {600}
    },
    {
        {601}
    },
    {
        {602}
    },
    {
        {603}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_fbc
 * osal device configuration : FBC interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbc[96] =
{
    {640}, {641}, {642}, {643}, {644}, {645}, {646}, {647}, {648}, {649},
    {650}, {651}, {652}, {653}, {654}, {655}, {656}, {657}, {658}, {659},
    {660}, {661}, {662}, {663}, {664}, {665}, {666}, {667}, {668}, {669},
    {670}, {671}, {672}, {673}, {674}, {675}, {676}, {677}, {678}, {679},
    {680}, {681}, {682}, {683}, {684}, {685}, {686}, {687}, {688}, {689},
    {690}, {691}, {692}, {693}, {694}, {695}, {696}, {697}, {698}, {699},
    {700}, {701}, {702}, {703}, {704}, {705}, {706}, {707}, {708}, {709},
    {710}, {711}, {712}, {713}, {714}, {715}, {716}, {717}, {718}, {719},
    {720}, {721}, {722}, {723}, {724}, {725}, {726}, {727}, {728}, {729},
    {730}, {731}, {732}, {733}, {734}, {735}
};

/*******************************************************************************************************************//**
 * @var gs_int_rtt
 * osal device configuration : RTT interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtt[11][1] =
{
    {
        {524}
    },
    {
        {525}
    },
    {
        {526}
    },
    {
        {527}
    },
    {
        {528}
    },
    {
        {529}
    },
    {
        {530}
    },
    {
        {531}
    },
    {
        {532}
    },
    {
        {533}
    },
    {
        {534}
    }
};

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_QNX_DEV_CFG
 * @defgroup OSAL_Private_Defines_Extern_Variables_DevConfig Device Condifurator Private extern variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_device_info
 * osal device configuration : device information structure
***********************************************************************************************************************/
st_osal_device_info_t gs_osal_device_info[] =
{
    /* device unique ID,      device type,    physical address,    range,     interrupt,    number of interrupts, power manager id,   initial policy,      AXI bus name,    assignment */
    /* IMR */
    {"ims_00",                "ims",                0xFE860000U, 0x00010000U, gs_int_ims[0],           1U, R_PMA_HWA_ID_IMR_LX4_CH0,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"ims_01",                "ims",                0xFE870000U, 0x00010000U, gs_int_ims[1],           1U, R_PMA_HWA_ID_IMR_LX4_CH1,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"imr_00",                "imr",                0xFE880000U, 0x00010000U, gs_int_imr[0],           1U, R_PMA_HWA_ID_IMR_LX4_CH2,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"imr_01",                "imr",                0xFE890000U, 0x00010000U, gs_int_imr[1],           1U, R_PMA_HWA_ID_IMR_LX4_CH3,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"imr_02",                "imr",                0xFE8A0000U, 0x00010000U, gs_int_imr[2],           1U, R_PMA_HWA_ID_IMR_LX4_CH4,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"imr_03",                "imr",                0xFE8B0000U, 0x00010000U, gs_int_imr[3],           1U, R_PMA_HWA_ID_IMR_LX4_CH5,   OSAL_PM_POLICY_CG,       "vc0",         NULL},

    /* VISIONIP */
    {"dof_00",                "dof",                0xE7A10000U, 0x00010000U, gs_int_dof[0],           2U, R_PMA_HWA_ID_VIP_UMFL0,     OSAL_PM_POLICY_CG,       "vip0",        NULL},
    {"dof_01",                "dof",                0xE7BB0000U, 0x00010000U, gs_int_dof[1],           2U, R_PMA_HWA_ID_VIP_UMFL1,     OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"stv_00",                "stv",                0xE7A00000U, 0x00010000U, gs_int_stv[0],           2U, R_PMA_HWA_ID_VIP_DISP0,     OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"stv_01",                "stv",                0xE7BA0000U, 0x00010000U, gs_int_stv[1],           2U, R_PMA_HWA_ID_VIP_DISP1,     OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"acf_00",                "acf",                0xE7A50000U, 0x00010000U, gs_int_acf[0],           2U, R_PMA_HWA_ID_VIP_ACF0,      OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"acf_01",                "acf",                0xE7A60000U, 0x00010000U, gs_int_acf[1],           2U, R_PMA_HWA_ID_VIP_ACF1,      OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"acf_02",                "acf",                0xE7A70000U, 0x00010000U, gs_int_acf[2],           2U, R_PMA_HWA_ID_VIP_ACF2,      OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"acf_03",                "acf",                0xE7A80000U, 0x00010000U, gs_int_acf[3],           2U, R_PMA_HWA_ID_VIP_ACF3,      OSAL_PM_POLICY_CG,       "vip1",        NULL},

    /* ICCOM */
    {"iccom_00",              "iccom",              0xE6260000U, 0x00001000U, gs_int_iccom,            8U, R_PMA_HWA_ID_ICCOM,         OSAL_PM_POLICY_CG,       "invalid",     NULL},

    /* ENCODER */
    {"ivcp1e_00",             "ivcp1e",             0xFEA00000U, 0x00010000U, gs_int_ivcp1e,           2U, R_PMA_HWA_ID_VCP_IVCP1E,    OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"vcp4_00",               "vcp4",               0xFE910000U, 0x00010000U, gs_int_vcp4,             2U, R_PMA_HWA_ID_VCP_VCP4,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fcpc_00",               "fcpc",               0xFE90F000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_VCP_FCPC,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},

     /* IMP */
    {"imp_top_00",            "imp_top",            0xFF900000U, 0x00004000U, gs_int_imp_top,          7U, R_PMA_HWA_ID_IMP_TOP,       OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ldmac_master_00",   "imp_ldmac_master",   0xF1F00000U, 0x00001000U, gs_int_imp_ldmac_master, 1U, R_PMA_HWA_ID_IMP_MASTER,    OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ldmac_checker_00",  "imp_ldmac_checker",  0xF1F10000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CHECKER,   OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_spmi_00",           "imp_spmi",           0xED300000U, 0x00100000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMI0_00,  OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_spmi_01",           "imp_spmi",           0xEDA00000U, 0x00100000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMI1_01,  OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_spmc_00",           "imp_spmc",           0xED600000U, 0x00200000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMC0_00,  OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_spmc_01",           "imp_spmc",           0xED800000U, 0x00200000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMC1_01,  OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_spmc_02",           "imp_spmc",           0xED400000U, 0x00200000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMC2_02,  OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_00",                "imp",                0xFFA00000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE0,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_01",                "imp",                0xFFA20000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE1,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_02",                "imp",                0xFFB00000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE2,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_03",                "imp",                0xFFB20000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE3,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_00",            "imp_ocv",            0xFFA40000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE0, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_01",            "imp_ocv",            0xFFA50000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE1, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_02",            "imp_ocv",            0xFFB40000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE2, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_03",            "imp_ocv",            0xFFB50000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE3, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_04",            "imp_ocv",            0xFFA60000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE4, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_05",            "imp_ocv",            0xFFB60000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE5, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_06",            "imp_ocv",            0xFFA70000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE6, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_07",            "imp_ocv",            0xFFB70000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE7, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_dma_00",            "imp_dma",            0xFFA80000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_DMAC0,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_dma_01",            "imp_dma",            0xFFB80000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_DMAC1,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_psc_00",            "imp_psc",            0xFFA84000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_PSC0,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_psc_01",            "imp_psc",            0xFFB84000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_PSC1,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_cnn_00",            "imp_cnn",            0xFFAA0000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CNN0,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_cnn_01",            "imp_cnn",            0xFFBC0000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CNN1,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_cnn_02",            "imp_cnn",            0xFFAC0000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CNN2,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_reg_spmi_00",       "imp_reg_spmi",       0xFFA8C000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMI0,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_reg_spmi_01",       "imp_reg_spmi",       0xFFB8C000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMI1,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_reg_spmc_00",       "imp_reg_spmc",       0xFFAB0000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMC0,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_reg_spmc_01",       "imp_reg_spmc",       0xFFBD0000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMC1,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_reg_spmc_02",       "imp_reg_spmc",       0xFFAD0000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMC2,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_dta_00",            "imp_dta",            0xFFA88000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_DTA,       OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_radsp_00",          "imp_radsp",          0xEB860000U, 0x00008000U, gs_int_imp_radsp[0],     1U, R_PMA_HWA_ID_IMP_RADSP0,    OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"imp_radsp_01",          "imp_radsp",          0xEB8E0000U, 0x00008000U, gs_int_imp_radsp[1],     1U, R_PMA_HWA_ID_IMP_RADSP1,    OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"imp_radsp_dmac_00",     "imp_radsp_dmac",     0xEB840000U, 0x00020000U, gs_int_imp_radsp_dmac[0],4U, R_PMA_HWA_ID_IMP_RADSP_DMAC0, OSAL_PM_POLICY_CG,     "mm(ipa)",     NULL},
    {"imp_radsp_dmac_01",     "imp_radsp_dmac",     0xEB8C0000U, 0x00020000U, gs_int_imp_radsp_dmac[1],4U, R_PMA_HWA_ID_IMP_RADSP_DMAC1, OSAL_PM_POLICY_CG,     "mm(ipa)",     NULL},

    /* ISP */
    {"cisp_00",               "cisp",               0xFEC00000U, 0x00020000U, gs_int_isp_cisp[0],      1U, R_PMA_HWA_ID_ISP_CORE0,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"cisp_01",               "cisp",               0xFEE00000U, 0x00020000U, gs_int_isp_cisp[1],      1U, R_PMA_HWA_ID_ISP_CORE1,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"cisp_02",               "cisp",               0xFEF00000U, 0x00020000U, gs_int_isp_cisp[2],      1U, R_PMA_HWA_ID_ISP_CORE2,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"cisp_03",               "cisp",               0xFE400000U, 0x00020000U, gs_int_isp_cisp[3],      1U, R_PMA_HWA_ID_ISP_CORE3,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"tisp_00",               "tisp",               0xFED00000U, 0x00010000U, gs_int_isp_tisp[0],      1U, R_PMA_HWA_ID_ISP_TISP0,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"tisp_01",               "tisp",               0xFED20000U, 0x00010000U, gs_int_isp_tisp[1],      1U, R_PMA_HWA_ID_ISP_TISP1,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"tisp_02",               "tisp",               0xFED30000U, 0x00010000U, gs_int_isp_tisp[2],      1U, R_PMA_HWA_ID_ISP_TISP2,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"tisp_03",               "tisp",               0xFED40000U, 0x00010000U, gs_int_isp_tisp[3],      1U, R_PMA_HWA_ID_ISP_TISP3,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"vspx_00",               "vspx",               0xFEDD0000U, 0x00008000U, gs_int_isp_vspx[0],      1U, R_PMA_HWA_ID_ISP_VSPX0,     OSAL_PM_POLICY_CG,       "vi1",         NULL},
    {"vspx_01",               "vspx",               0xFEDD8000U, 0x00008000U, gs_int_isp_vspx[1],      1U, R_PMA_HWA_ID_ISP_VSPX1,     OSAL_PM_POLICY_CG,       "vi1",         NULL},
    {"vspx_02",               "vspx",               0xFEDE0000U, 0x00008000U, gs_int_isp_vspx[2],      1U, R_PMA_HWA_ID_ISP_VSPX2,     OSAL_PM_POLICY_CG,       "vi1",         NULL},
    {"vspx_03",               "vspx",               0xFEDE8000U, 0x00008000U, gs_int_isp_vspx[3],      1U, R_PMA_HWA_ID_ISP_VSPX3,     OSAL_PM_POLICY_CG,       "vi1",         NULL},

    /* FieldBIST */
    {"fbc_00",                "fbc",                0xFF830000U, 0x00002000U, gs_int_fbc,             96U, R_PMA_HWA_ID_FBC,           OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"fba_ims_00",            "fba_ims",            0xFE605000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMS0,      OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"fba_ims_01",            "fba_ims",            0xFE606000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMS1,      OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"fba_imr_00",            "fba_imr",            0xFE601000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMR0,      OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"fba_imr_01",            "fba_imr",            0xFE602000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMR1,      OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"fba_imr_02",            "fba_imr",            0xFE603000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMR2,      OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"fba_imr_03",            "fba_imr",            0xFE604000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMR3,      OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"fba_imp_00",            "fba_imp",            0xFF8C0000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP0,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_01",            "fba_imp",            0xFF8C1000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP1,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_02",            "fba_imp",            0xFF8C2000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP2,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_03",            "fba_imp",            0xFF8C3000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP3,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_ocv_00",        "fba_imp_ocv",        0xFF8C4000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV0,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_ocv_01",        "fba_imp_ocv",        0xFF8C5000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV1,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_ocv_02",        "fba_imp_ocv",        0xFF8C6000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV2,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_ocv_03",        "fba_imp_ocv",        0xFF8C7000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV3,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_ocv_04",        "fba_imp_ocv",        0xFF8F0000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV4,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_ocv_05",        "fba_imp_ocv",        0xFF8F1000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV5,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_ocv_06",        "fba_imp_ocv",        0xFF8F2000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV6,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_ocv_07",        "fba_imp_ocv",        0xFF8F3000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV7,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_dp_00",         "fba_imp_dp",         0xFF5F6000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DP0,       OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_dp_01",         "fba_imp_dp",         0xFF5F7000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DP1,       OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"fba_imp_cnn_main_00",   "fba_imp_cnn_main",   0xFFF50000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_00_sub_00", "fba_imp_cnn_00_sub", 0xFFF51000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN0_SUB0, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_00_sub_01", "fba_imp_cnn_00_sub", 0xFFF52000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN0_SUB1, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_00_sub_02", "fba_imp_cnn_00_sub", 0xFFF53000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN0_SUB2, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_00_sub_03", "fba_imp_cnn_00_sub", 0xFFF54000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN0_SUB3, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_main_01",   "fba_imp_cnn_main",   0xFFF60000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_01_sub_00", "fba_imp_cnn_01_sub", 0xFFF61000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN1_SUB0, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_01_sub_01", "fba_imp_cnn_01_sub", 0xFFF62000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN1_SUB1, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_01_sub_02", "fba_imp_cnn_01_sub", 0xFFF63000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN1_SUB2, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_01_sub_03", "fba_imp_cnn_01_sub", 0xFFF64000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN1_SUB3, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_main_02",   "fba_imp_cnn_main",   0xFFF70000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN2,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_02_sub_00", "fba_imp_cnn_02_sub", 0xFFF71000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN2_SUB0, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_02_sub_01", "fba_imp_cnn_02_sub", 0xFFF72000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN2_SUB1, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_02_sub_02", "fba_imp_cnn_02_sub", 0xFFF73000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN2_SUB2, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_02_sub_03", "fba_imp_cnn_02_sub", 0xFFF74000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN2_SUB3, OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_dsp_00",        "fba_imp_dsp",        0xFFF66000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DSP0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_dsp_01",        "fba_imp_dsp",        0xFFF67000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DSP1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnram_00",      "fba_imp_cnram",      0xFFF55000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNRAM0,    OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnram_01",      "fba_imp_cnram",      0xFFF65000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNRAM1,    OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnram_02",      "fba_imp_cnram",      0xFFF75000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNRAM2,    OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_dof_00",            "fba_dof",            0xE7B61000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DOF0,      OSAL_PM_POLICY_CG,       "vip0",        NULL},
    {"fba_dof_01",            "fba_dof",            0xE7B80000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DOF1,      OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"fba_stv_00",            "fba_stv",            0xE7B81000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_STV0,      OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"fba_stv_01",            "fba_stv",            0xE7B87000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_STV1,      OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"fba_acf_00",            "fba_acf",            0xE7B82000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ACF0,      OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"fba_acf_01",            "fba_acf",            0xE7B86000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ACF1,      OSAL_PM_POLICY_CG,       "vip1",        NULL},
    {"fba_smps_00",           "fba_smps",           0xE7B64000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_SMPS0,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_smps_01",           "fba_smps",           0xE7B85000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_SMPS1,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_smes_00",           "fba_smes",           0xE7B62000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_SMES0,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_smes_01",           "fba_smes",           0xE7B83000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_SMES1,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_smpo_00",           "fba_smpo",           0xE7B63000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_SMPO0,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_smpo_01",           "fba_smpo",           0xE7B84000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_SMPO1,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_isp_00",            "fba_isp",            0xFEAC1000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ISP0,      OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"fba_isp_01",            "fba_isp",            0xFEAC2000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ISP1,      OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"fba_isp_02",            "fba_isp",            0xFEAC3000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ISP2,      OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"fba_isp_03",            "fba_isp",            0xFEAC4000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ISP3,      OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"fba_ca76d_00",          "fba_ca76d",          0xFF870000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL0,       OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76d_01",          "fba_ca76d",          0xFF871000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL1,       OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76d_02",          "fba_ca76d",          0xFF850000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL2,       OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76d_03",          "fba_ca76d",          0xFF851000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL3,       OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76_00",           "fba_ca76",           0xFF872000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL0_CPU0,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76_01",           "fba_ca76",           0xFF873000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL0_CPU1,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76_02",           "fba_ca76",           0xFF874000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL1_CPU2,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76_03",           "fba_ca76",           0xFF875000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL1_CPU3,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76_04",           "fba_ca76",           0xFF852000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL2_CPU4,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76_05",           "fba_ca76",           0xFF853000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL2_CPU5,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76_06",           "fba_ca76",           0xFF854000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL3_CPU6,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca76_07",           "fba_ca76",           0xFF855000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL3_CPU7,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},

    /* RTT */
    {"rfso_000",              "rfso",               0xFFE80000U, 0x00001000U, gs_int_rtt[0],           1U, R_PMA_HWA_ID_RFSO0,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_001",              "rfso",               0xFFE81000U, 0x00001000U, gs_int_rtt[1],           1U, R_PMA_HWA_ID_RFSO1,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_002",              "rfso",               0xFFE82000U, 0x00001000U, gs_int_rtt[2],           1U, R_PMA_HWA_ID_RFSO2,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_003",              "rfso",               0xFFE83000U, 0x00001000U, gs_int_rtt[3],           1U, R_PMA_HWA_ID_RFSO3,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_004",              "rfso",               0xFFE84000U, 0x00001000U, gs_int_rtt[4],           1U, R_PMA_HWA_ID_RFSO4,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_005",              "rfso",               0xFFE85000U, 0x00001000U, gs_int_rtt[5],           1U, R_PMA_HWA_ID_RFSO5,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_006",              "rfso",               0xFFE86000U, 0x00001000U, gs_int_rtt[6],           1U, R_PMA_HWA_ID_RFSO6,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_007",              "rfso",               0xFFE87000U, 0x00001000U, gs_int_rtt[7],           1U, R_PMA_HWA_ID_RFSO7,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_008",              "rfso",               0xFFE88000U, 0x00001000U, gs_int_rtt[8],           1U, R_PMA_HWA_ID_RFSO8,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_009",              "rfso",               0xFFE89000U, 0x00001000U, gs_int_rtt[9],           1U, R_PMA_HWA_ID_RFSO9,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_010",              "rfso",               0xFFE8A000U, 0x00001000U, gs_int_rtt[10],          1U, R_PMA_HWA_ID_RFSO10,        OSAL_PM_POLICY_CG,       "invalid",     NULL},

    {.id = OSAL_DEVICE_INVALID_ID}
};

/** @} */

#endif /* OSAL_DEV_CFG_V3U_H */
