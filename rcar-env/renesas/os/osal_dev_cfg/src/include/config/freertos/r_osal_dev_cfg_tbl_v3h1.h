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
* File Name :    r_osal_dev_cfg_v3h1.h
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car V3H1
* Description :  OSAL device configuration file for R-Car V3H1
***********************************************************************************************************************/
#ifndef OSAL_DEV_CFG_V3H1_H
#define OSAL_DEV_CFG_V3H1_H

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
static st_osal_interrupt_info_t gs_int_ims[4][1] =
{
    {
        {192}
    },
    {
        {193}
    },
    {
        {194}
    },
    {
        {195}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_imr
 * osal device configuration : IMR interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imr[2][1] =
{
    {
        {254}
    },
    {
        {255}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_dof
 * osal device configuration : DOF interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_dof[1][2] =
{
    {
        {242}, {243}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_stv
 * osal device configuration : STV interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_stv[1][2] =
{
    {
        {238}, {239}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_acf
 * osal device configuration : ACF interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_acf[5][2] =
{
    {
        {71}, {97}
    },
    {
        {98}, {99}
    },
    {
        {100}, {101}
    },
    {
        {102}, {103}
    },
    {
        {104}, {108}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_iccom
 * osal device configuration : ICCOM interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_iccom[8] =
{
    {224}, {225}, {226}, {227}, {228}, {229}, {230}, {231}
};

/*******************************************************************************************************************//**
 * @var gs_int_ivcp1e
 * osal device configuration : IVCP1E interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ivcp1e[2] =
{
    {300}, {170}
};

/*******************************************************************************************************************//**
 * @var gs_int_imp_top
 * osal device configuration : IMP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imp_top[5] =
{
    {281}, {282}, {283}, {284}, {285}
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_cisp
 * osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_cisp[2][1] =
{
    {
        {26}
    },
    {
        {109}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_tisp
 * osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_tisp[2][1] =
{
    {
        {25}
    },
    {
        {27}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_fbc
 * osal device configuration : FBC interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbc[2] =
{
    {14}, {15}
};

/*******************************************************************************************************************//**
 * @var gs_int_rtt
 * osal device configuration : RTT interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtt[11][1] =
{
    {
        {32}
    },
    {
        {33}
    },
    {
        {34}
    },
    {
        {35}
    },
    {
        {36}
    },
    {
        {37}
    },
    {
        {221}
    },
    {
        {186}
    },
    {
        {187}
    },
    {
        {185}
    },
    {
        {247}
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
    {"ims_00",                "ims",               0xFE860000U, 0x00002000U, gs_int_ims[0],           1U, R_PMA_HWA_ID_IMR_LX4_CH0,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"ims_01",                "ims",               0xFE870000U, 0x00002000U, gs_int_ims[1],           1U, R_PMA_HWA_ID_IMR_LX4_CH1,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"ims_02",                "ims",               0xFE880000U, 0x00002000U, gs_int_ims[2],           1U, R_PMA_HWA_ID_IMR_LX4_CH2,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"ims_03",                "ims",               0xFE890000U, 0x00002000U, gs_int_ims[3],           1U, R_PMA_HWA_ID_IMR_LX4_CH3,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"imr_00",                "imr",               0xFE8A0000U, 0x00002000U, gs_int_imr[0],           1U, R_PMA_HWA_ID_IMR_LX4_CH4,   OSAL_PM_POLICY_CG,       "vc0",         NULL},
    {"imr_01",                "imr",               0xFE8B0000U, 0x00002000U, gs_int_imr[1],           1U, R_PMA_HWA_ID_IMR_LX4_CH5,   OSAL_PM_POLICY_CG,       "vc0",         NULL},

    /* VISIONIP */
    {"dof_00",                "dof",               0xE7A10000U, 0x00010000U, gs_int_dof[0],           2U, R_PMA_HWA_ID_VIP_UMFL0,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"stv_00",                "stv",               0xE7A00000U, 0x00010000U, gs_int_stv[0],           2U, R_PMA_HWA_ID_VIP_DISP0,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"acf_00",                "acf",               0xE7A50000U, 0x00010000U, gs_int_acf[0],           2U, R_PMA_HWA_ID_VIP_ACF0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"acf_01",                "acf",               0xE7A60000U, 0x00010000U, gs_int_acf[1],           2U, R_PMA_HWA_ID_VIP_ACF1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"acf_02",                "acf",               0xE7A70000U, 0x00010000U, gs_int_acf[2],           2U, R_PMA_HWA_ID_VIP_ACF2,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"acf_03",                "acf",               0xE7A80000U, 0x00010000U, gs_int_acf[3],           2U, R_PMA_HWA_ID_VIP_ACF3,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"acf_04",                "acf",               0xE7A90000U, 0x00010000U, gs_int_acf[4],           2U, R_PMA_HWA_ID_VIP_ACF4,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},

    /* ICCOM */
    {"iccom_00",              "iccom",             0xE6260000U, 0x00001000U, gs_int_iccom,            8U, R_PMA_HWA_ID_ICCOM,         OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},

    /* ENCODER */
    {"ivcp1e_00",             "ivcp1e",            0xFEA00000U, 0x00010000U, gs_int_ivcp1e,           2U, R_PMA_HWA_ID_VCP_IVCP1E,    OSAL_PM_POLICY_CG,       "vi0",         NULL},

     /* IMP */
    {"imp_top_00",            "imp_top",           0xFFA00000U, 0x00010000U, gs_int_imp_top,          5U, R_PMA_HWA_ID_IMP_TOP,       OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_00",                "imp",               0xFF900000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE0,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_01",                "imp",               0xFF920000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE1,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_02",                "imp",               0xFF940000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE2,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_03",                "imp",               0xFF960000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE3,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_04",                "imp",               0xFFA80000U, 0x00020000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CORE3,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_slim_00",           "imp_slim",          0xFF9C0000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SLIM0,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_00",            "imp_ocv",           0xFF980000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE0, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_01",            "imp_ocv",           0xFF990000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE1, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_02",            "imp_ocv",           0xFF9A0000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE2, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_03",            "imp_ocv",           0xFF9B0000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE3, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_ocv_04",            "imp_ocv",           0xFFA40000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_OCV_CORE4, OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_dma_00",            "imp_dma",           0xFFA10000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_IMP_DMAC0,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_dma_01",            "imp_dma",           0xFFA14000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_IMP_DMAC1,     OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_psc_00",            "imp_psc",           0xFFA20000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_PSC0,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_psc_01",            "imp_psc",           0xFFA24000U, 0x00004000U, NULL,                    0U, R_PMA_HWA_ID_IMP_PSC0,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_cnn_00",            "imp_cnn",           0xFF9E0000U, 0x00010000U, NULL,                    0U, R_PMA_HWA_ID_IMP_CNN0,      OSAL_PM_POLICY_CG,       "ir",          NULL},
    {"imp_spmi_00",           "imp_spmi",          0xED000000U, 0x00200000U, NULL,                    0U, R_PMA_HWA_ID_IMP_SPMI0_00,  OSAL_PM_POLICY_CG,       "ir",          NULL},

    /* ISP */
    {"cisp_00",               "cisp",              0xFEC00000U, 0x00020000U, gs_int_isp_cisp[0],      1U, R_PMA_HWA_ID_ISP_CORE0,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"cisp_01",               "cisp",              0xFEE00000U, 0x00020000U, gs_int_isp_cisp[1],      1U, R_PMA_HWA_ID_ISP_CORE1,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"tisp_00",               "tisp",              0xFED00000U, 0x00010000U, gs_int_isp_tisp[0],      1U, R_PMA_HWA_ID_ISP_TISP0,     OSAL_PM_POLICY_CG,       "vi0",         NULL},
    {"tisp_01",               "tisp",              0xFED20000U, 0x00010000U, gs_int_isp_tisp[1],      1U, R_PMA_HWA_ID_ISP_TISP1,     OSAL_PM_POLICY_CG,       "vi0",         NULL},

    /* FieldBIST */
    {"fbc_00",                "fbc",               0xFF830000U, 0x00002000U, gs_int_fbc,              2U, R_PMA_HWA_ID_FBC,           OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imr_00",            "fba_imr",           0xFFF80000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMR0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imr_01",            "fba_imr",           0xFFF81000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMR1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ims_00",            "fba_ims",           0xFFF90000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMS0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ims_01",            "fba_ims",           0xFFF91000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMS1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ims_02",            "fba_ims",           0xFFF92000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMS2,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ims_03",            "fba_ims",           0xFFF93000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMS3,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_00",            "fba_imp",           0xFF8D0000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_01",            "fba_imp",           0xFFF50000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_02",            "fba_imp",           0xFFF60000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP2,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_03",            "fba_imp",           0xFFF70000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP3,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_04",            "fba_imp",           0xFF8D1000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_IMP4,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_ocv_00",        "fba_imp_ocv",       0xFFFA0000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_ocv_01",        "fba_imp_ocv",       0xFFFB0000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_ocv_02",        "fba_imp_ocv",       0xFF8D2000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV2,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_ocv_03",        "fba_imp_ocv",       0xFF8D3000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV3,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_ocv_04",        "fba_imp_ocv",       0xFF8D4000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_OCV4,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_dp_00",         "fba_imp_dp",        0xFF8D5000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DP0,       OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_dp_01",         "fba_imp_dp",        0xFF8D6000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DP1,       OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_cnn_00",        "fba_imp_cnn",       0xFF8D7000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CNN0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_imp_slim_00",       "fba_imp_slim",      0xFF8DC000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_SLIM0,     OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_dof_00",            "fba_dof",           0xE7930000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_DOF0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_stv_00",            "fba_stv",           0xE7950000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_STV0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_acf_00",            "fba_acf",           0xE7940000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ACF0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_acf_01",            "fba_acf",           0xE7941000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ACF1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
/* TODO: Unnecessary?
    {"fba_isp_00",            "fba_isp",           0xFEAC1000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ISP0,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_isp_01",            "fba_isp",           0xFEAC2000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ISP1,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_isp_02",            "fba_isp",           0xFEAC3000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ISP2,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_isp_03",            "fba_isp",           0xFEAC4000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_ISP3,      OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
*/
    {"fba_ca53d_00",          "fba_ca53d",         0xFF87C000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL0,       OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca53_00",           "fba_ca53",          0xFF877000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL0_CPU0,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca53_01",           "fba_ca53",          0xFF878000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL0_CPU1,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca53_02",           "fba_ca53",          0xFF879000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL1_CPU2,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},
    {"fba_ca53_03",           "fba_ca53",          0xFF87A000U, 0x00001000U, NULL,                    0U, R_PMA_HWA_ID_FBA_CL1_CPU3,  OSAL_PM_POLICY_CG,       "mm(ipa)",     NULL},

    /* RTT */
    {"rfso_000",              "rfso",              0xFFE80000U, 0x00001000U, gs_int_rtt[0],           1U, R_PMA_HWA_ID_RFSO0,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_001",              "rfso",              0xFFE81000U, 0x00001000U, gs_int_rtt[1],           1U, R_PMA_HWA_ID_RFSO1,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_002",              "rfso",              0xFFE82000U, 0x00001000U, gs_int_rtt[2],           1U, R_PMA_HWA_ID_RFSO2,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_003",              "rfso",              0xFFE83000U, 0x00001000U, gs_int_rtt[3],           1U, R_PMA_HWA_ID_RFSO3,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_004",              "rfso",              0xFFE84000U, 0x00001000U, gs_int_rtt[4],           1U, R_PMA_HWA_ID_RFSO4,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_005",              "rfso",              0xFFE85000U, 0x00001000U, gs_int_rtt[5],           1U, R_PMA_HWA_ID_RFSO5,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_006",              "rfso",              0xFFE86000U, 0x00001000U, gs_int_rtt[6],           1U, R_PMA_HWA_ID_RFSO6,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_007",              "rfso",              0xFFE87000U, 0x00001000U, gs_int_rtt[7],           1U, R_PMA_HWA_ID_RFSO7,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_008",              "rfso",              0xFFE88000U, 0x00001000U, gs_int_rtt[8],           1U, R_PMA_HWA_ID_RFSO8,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_009",              "rfso",              0xFFE89000U, 0x00001000U, gs_int_rtt[9],           1U, R_PMA_HWA_ID_RFSO9,         OSAL_PM_POLICY_CG,       "invalid",     NULL},
    {"rfso_010",              "rfso",              0xFFE8A000U, 0x00001000U, gs_int_rtt[10],          1U, R_PMA_HWA_ID_RFSO10,        OSAL_PM_POLICY_CG,       "invalid",     NULL},

    {.id = OSAL_DEVICE_INVALID_ID}
};

/** @} */

#endif /* OSAL_DEV_CFG_V3H1_H */
