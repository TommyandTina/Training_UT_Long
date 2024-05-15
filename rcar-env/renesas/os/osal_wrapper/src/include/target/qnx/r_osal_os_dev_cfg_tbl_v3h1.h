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
* File Name :    r_osal_os_dev_cfg_tbl_v3h1.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car V3H1
* Description :  Device Configuration file for R-Car V3H1
***********************************************************************************************************************/

#ifndef OSAL_OS_DEV_CFG_V3H1_H
#define OSAL_OS_DEV_CFG_V3H1_H

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_008
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_int_ims
 * @brief osal device configuration : IMS interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ims_v3h1[4][1] =
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
 * @brief osal device configuration : IMR interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imr_v3h1[2][1] =
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
 * @brief osal device configuration : DOF interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_dof_v3h1[1][2] =
{
    {
        {242}, {243}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_stv
 * @brief osal device configuration : STV interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_stv_v3h1[1][2] =
{
    {
        {238}, {239}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_acf
 * @brief osal device configuration : ACF interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_acf_v3h1[5][2] =
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
 * @brief osal device configuration : ICCOM interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_iccom_v3h1[8][1] =
{
    {
        {224}
    },
    {
        {225}
    },
    {
        {226}
    },
    {
        {227}
    },
    {
        {228}
    },
    {
        {229}
    },
    {
        {230}
    },
    {
        {231}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_ivcp1e
 * @brief osal device configuration : IVCP1E interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ivcp1e_v3h1[2] =
{
    {300}, {170}
};

/*******************************************************************************************************************//**
 * @var gs_int_imp_top
 * @brief osal device configuration : IMP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imp_top_v3h1[5] =
{
    {281}, {282}, {283}, {284}, {285}
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_cisp
 * @brief osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_cisp_v3h1[2][1] =
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
 * @brief osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_tisp_v3h1[2][1] =
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
 * @brief osal device configuration : FBC interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbc_v3h1[2] =
{
    {14}, {15}
};

/*******************************************************************************************************************//**
 * @var gs_int_rtt
 * @brief osal device configuration : RTT interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtt_v3h1[11][1] =
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

/*******************************************************************************************************************//**
 * @var gs_int_ipmmu
 * @brief osal device configuration : IPMMU interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ipmmu_v3h1[1] =
{
    {196}
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_032]
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @var gs_utlb_number_04_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_04_v3h1 =
{
    1, { 4 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_05_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_05_v3h1 =
{
    1, { 5 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_06_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_06_v3h1 =
{
    1, { 6 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_07_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_07_v3h1 =
{
    1, { 7 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_10_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_10_v3h1 =
{
    1, { 10 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_11_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_11_v3h1 =
{
    1, { 11 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_13_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_13_v3h1 =
{
    1, { 13 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_14_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_14_v3h1 =
{
    1, { 14 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_00to01_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_00to01_v3h1 =
{
    2, { 0, 1 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_02to03_v3h1
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_02to03_v3h1 =
{
    2, { 2, 3 }
};

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_009
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_device_info_v3h1
 * @brief osal device configuration : v3h1 device information structure
***********************************************************************************************************************/
/* PRQA S 3218 1 # Definition within a function reduces readability and maintainability, so no change. */
static const st_osal_device_info_t gs_osal_device_info_v3h1[] =
{
    /* group name,   device unique ID,      device type,    physical address,    range,     interrupt,    number of interrupts, power manager id,               POSTCKCR,               AXI bus name,    assignment */
    /* IMR */
    {NULL,           "ims_00",                "ims",               0xFE860000U, 0x00002000U, gs_int_ims_v3h1[0],           1U, R_PMA_HWA_ID_V3H1_IMR_LX4_CH0,   OSAL_PM_POSTCKCR_INVALID, "vc0",        &gs_utlb_number_04_v3h1 },
    {NULL,           "ims_01",                "ims",               0xFE870000U, 0x00002000U, gs_int_ims_v3h1[1],           1U, R_PMA_HWA_ID_V3H1_IMR_LX4_CH1,   OSAL_PM_POSTCKCR_INVALID, "vc0",        &gs_utlb_number_05_v3h1 },
    {NULL,           "ims_02",                "ims",               0xFE880000U, 0x00002000U, gs_int_ims_v3h1[2],           1U, R_PMA_HWA_ID_V3H1_IMR_LX4_CH2,   OSAL_PM_POSTCKCR_INVALID, "vc0",        &gs_utlb_number_06_v3h1 },
    {NULL,           "ims_03",                "ims",               0xFE890000U, 0x00002000U, gs_int_ims_v3h1[3],           1U, R_PMA_HWA_ID_V3H1_IMR_LX4_CH3,   OSAL_PM_POSTCKCR_INVALID, "vc0",        &gs_utlb_number_07_v3h1 },
    {NULL,           "imr_00",                "imr",               0xFE8A0000U, 0x00002000U, gs_int_imr_v3h1[0],           1U, R_PMA_HWA_ID_V3H1_IMR_LX4_CH4,   OSAL_PM_POSTCKCR_INVALID, "vc0",        &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imr_01",                "imr",               0xFE8B0000U, 0x00002000U, gs_int_imr_v3h1[1],           1U, R_PMA_HWA_ID_V3H1_IMR_LX4_CH5,   OSAL_PM_POSTCKCR_INVALID, "vc0",        &gs_utlb_number_02to03_v3h1 },

    /* VISIONIP */
    {NULL,           "dof_00",                "dof",               0xE7A10000U, 0x00010000U, gs_int_dof_v3h1[0],           2U, R_PMA_HWA_ID_V3H1_VIP_DOF,       OSAL_PM_POST4CKCR,        "invalid",    NULL},
    {NULL,           "stv_00",                "stv",               0xE7A00000U, 0x00010000U, gs_int_stv_v3h1[0],           2U, R_PMA_HWA_ID_V3H1_VIP_STV,       OSAL_PM_POST4CKCR,        "invalid",    NULL},
    {NULL,           "acf_00",                "acf",               0xE7A50000U, 0x00010000U, gs_int_acf_v3h1[0],           2U, R_PMA_HWA_ID_V3H1_VIP_ACF0,      OSAL_PM_POST4CKCR,        "invalid",    NULL},
    {NULL,           "acf_01",                "acf",               0xE7A60000U, 0x00010000U, gs_int_acf_v3h1[1],           2U, R_PMA_HWA_ID_V3H1_VIP_ACF1,      OSAL_PM_POST4CKCR,        "invalid",    NULL},
    {NULL,           "acf_02",                "acf",               0xE7A70000U, 0x00010000U, gs_int_acf_v3h1[2],           2U, R_PMA_HWA_ID_V3H1_VIP_ACF2,      OSAL_PM_POST4CKCR,        "invalid",    NULL},
    {NULL,           "acf_03",                "acf",               0xE7A80000U, 0x00010000U, gs_int_acf_v3h1[3],           2U, R_PMA_HWA_ID_V3H1_VIP_ACF3,      OSAL_PM_POST4CKCR,        "invalid",    NULL},
    {NULL,           "acf_04",                "acf",               0xE7A90000U, 0x00010000U, gs_int_acf_v3h1[4],           2U, R_PMA_HWA_ID_V3H1_VIP_ACF4,      OSAL_PM_POST4CKCR,        "invalid",    NULL},

    /* ICCOM(IO) */
    {NULL,           "iccom_reg_00",          "iccom_reg",         0xE6260000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* ICCOM(Interrupt) */
    {NULL,           "iccom_00",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3h1[0],         1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_01",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3h1[1],         1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_02",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3h1[2],         1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_03",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3h1[3],         1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_04",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3h1[4],         1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_05",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3h1[5],         1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_06",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3h1[6],         1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_07",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3h1[7],         1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* ENCODER */
    {NULL,           "ivcp1e_00",             "ivcp1e",            0xFEA00000U, 0x00010000U, gs_int_ivcp1e_v3h1,           2U, R_PMA_HWA_ID_V3H1_VCP_IVCP1E,    OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_number_04_v3h1 },

     /* IMP */
    {NULL,           "imp_top_00",            "imp_top",           0xFFA00000U, 0x00010000U, gs_int_imp_top_v3h1,          5U, R_PMA_HWA_ID_V3H1_IMP_TOP,       OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_00",                "imp",               0xFF900000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_CORE0,     OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_01",                "imp",               0xFF920000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_CORE1,     OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_02",                "imp",               0xFF940000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_CORE2,     OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_03",                "imp",               0xFF960000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_CORE3,     OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_04",                "imp",               0xFFA80000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_CORE4,     OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_slim_00",           "imp_slim",          0xFF9C0000U, 0x00010000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_SIMP_CORE,     OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_ocv_00",            "imp_ocv",           0xFF980000U, 0x00010000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_OCV_CORE0, OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_ocv_01",            "imp_ocv",           0xFF990000U, 0x00010000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_OCV_CORE1, OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_ocv_02",            "imp_ocv",           0xFF9A0000U, 0x00010000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_OCV_CORE2, OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_ocv_03",            "imp_ocv",           0xFF9B0000U, 0x00010000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_OCV_CORE3, OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_ocv_04",            "imp_ocv",           0xFFA40000U, 0x00010000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_OCV_CORE4, OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_dma_00",            "imp_dma",           0xFFA10000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_DMAC0,     OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_dma_01",            "imp_dma",           0xFFA14000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_DMAC1,     OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_psc_00",            "imp_psc",           0xFFA20000U, 0x00004000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_PSC0,      OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_psc_01",            "imp_psc",           0xFFA24000U, 0x00004000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_PSC1,      OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_cnn_00",            "imp_cnn",           0xFF9E0000U, 0x00010000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_CNN,       OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },
    {NULL,           "imp_spmi_00",           "imp_spmi",          0xED000000U, 0x00200000U, NULL,                         0U, R_PMA_HWA_ID_V3H1_IMP_SPMI,      OSAL_PM_POSTCKCR_INVALID, "ir",         &gs_utlb_number_00to01_v3h1 },

    /* ISP */
    {NULL,           "cisp_00",               "cisp",              0xFEC00000U, 0x00020000U, gs_int_isp_cisp_v3h1[0],      1U, R_PMA_HWA_ID_V3H1_CISP0,         OSAL_PM_POSTCKCR_INVALID, "vi0",         &gs_utlb_number_11_v3h1 },
    {NULL,           "cisp_01",               "cisp",              0xFEE00000U, 0x00020000U, gs_int_isp_cisp_v3h1[1],      1U, R_PMA_HWA_ID_V3H1_CISP1,         OSAL_PM_POSTCKCR_INVALID, "vi0",         &gs_utlb_number_14_v3h1 },
    {NULL,           "tisp_00",               "tisp",              0xFED00000U, 0x00010000U, gs_int_isp_tisp_v3h1[0],      1U, R_PMA_HWA_ID_V3H1_TISP0,         OSAL_PM_POSTCKCR_INVALID, "vi0",         &gs_utlb_number_10_v3h1 },
    {NULL,           "tisp_01",               "tisp",              0xFED20000U, 0x00010000U, gs_int_isp_tisp_v3h1[1],      1U, R_PMA_HWA_ID_V3H1_TISP1,         OSAL_PM_POSTCKCR_INVALID, "vi0",         &gs_utlb_number_13_v3h1 },

    /* FieldBIST */
    {NULL,           "fbc_00",                "fbc",               0xFF830000U, 0x00002000U, gs_int_fbc_v3h1,              2U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imr0",              "fba",               0xFFF80000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imr1",              "fba",               0xFFF81000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_ims0",              "fba",               0xFFF90000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_ims1",              "fba",               0xFFF91000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_ims2",              "fba",               0xFFF92000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_ims3",              "fba",               0xFFF93000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp0",              "fba",               0xFF8D0000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp1",              "fba",               0xFFF50000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp2",              "fba",               0xFFF60000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp3",              "fba",               0xFFF70000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp4",              "fba",               0xFF8D1000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv0",          "fba",               0xFFFA0000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv1",          "fba",               0xFFFB0000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv2",          "fba",               0xFF8D2000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv3",          "fba",               0xFF8D3000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv4",          "fba",               0xFF8D4000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_dp0",           "fba",               0xFF8D5000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_dp1",           "fba",               0xFF8D6000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_cnn0",          "fba",               0xFF8D7000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_slim0",         "fba",               0xFF8DC000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_dof0",              "fba",               0xE7930000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST4CKCR,        "invalid",     NULL},
    {NULL,           "fba_stv0",              "fba",               0xE7950000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST4CKCR,        "invalid",     NULL},
    {NULL,           "fba_acf0",              "fba",               0xE7940000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST4CKCR,        "invalid",     NULL},
    {NULL,           "fba_acf1",              "fba",               0xE7941000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST4CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca53d",             "fba",               0xFF87C000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca530",             "fba",               0xFF877000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca531",             "fba",               0xFF878000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca532",             "fba",               0xFF879000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca533",             "fba",               0xFF87A000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POST2CKCR,        "invalid",     NULL},

    /* RTT */
    {NULL,           "rfso_00",               "rfso",              0xFFE80000U, 0x00001000U, gs_int_rtt_v3h1[0],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_01",               "rfso",              0xFFE81000U, 0x00001000U, gs_int_rtt_v3h1[1],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_02",               "rfso",              0xFFE82000U, 0x00001000U, gs_int_rtt_v3h1[2],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_03",               "rfso",              0xFFE83000U, 0x00001000U, gs_int_rtt_v3h1[3],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_04",               "rfso",              0xFFE84000U, 0x00001000U, gs_int_rtt_v3h1[4],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_05",               "rfso",              0xFFE85000U, 0x00001000U, gs_int_rtt_v3h1[5],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_06",               "rfso",              0xFFE86000U, 0x00001000U, gs_int_rtt_v3h1[6],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_07",               "rfso",              0xFFE87000U, 0x00001000U, gs_int_rtt_v3h1[7],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_08",               "rfso",              0xFFE88000U, 0x00001000U, gs_int_rtt_v3h1[8],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_09",               "rfso",              0xFFE89000U, 0x00001000U, gs_int_rtt_v3h1[9],           1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_10",               "rfso",              0xFFE8A000U, 0x00001000U, gs_int_rtt_v3h1[10],          1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* IPMMU */
    {NULL,           "ipmmu_mm_00",           "ipmmu_mm",          0xE67B0000U, 0x00020000U, gs_int_ipmmu_v3h1,            1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vi0",       "ipmmu_cache",       0xFEBD0000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vc0",       "ipmmu_cache",       0xFE990000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_ir",        "ipmmu_cache",       0xFF8B0000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_rt0",       "ipmmu_cache",       0xFFC80000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_ds1",       "ipmmu_cache",       0xE7740000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vip0",      "ipmmu_cache",       0xE7B00000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vip1",      "ipmmu_cache",       0xE7960000U, 0x00020000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* FCPR */
    {NULL,           "fcprr_stv_00",          "fcprr",             0xE7AA0000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_dof_00",          "fcprw",             0xE7AB1000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_dof_00",          "fcprr",             0xE7AA1000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_acf_00",          "fcprr",             0xE7AA5000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_acf_01",          "fcprr",             0xE7AA6000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_acf_02",          "fcprr",             0xE7AA7000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_acf_03",          "fcprr",             0xE7AA8000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_acf_04",          "fcprr",             0xE7AA9000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_imp_00",          "fcprw",             0xFF8DA000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_imp_00",          "fcprr",             0xFF8D8000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_imp_01",          "fcprw",             0xFF8DB000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_imp_01",          "fcprr",             0xFF8D9000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_ims_00",          "fcprr",             0xFE9C0000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_ims_01",          "fcprr",             0xFE9C1000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_ims_02",          "fcprr",             0xFE9C2000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_imr_00",          "fcprw",             0xFE9C6000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_imr_00",          "fcprr",             0xFE9C4000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_imr_01",          "fcprw",             0xFE9C7000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_imr_01",          "fcprr",             0xFE9C5000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_vspd_00",         "fcprr",             0xFEA90000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_ivcp1e_00",       "fcprr",             0xFEA91000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_pcie_00",         "fcprr",             0xFEA92000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_vin_00_03",       "fcprw",             0xFEA93000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_vin_04_07",       "fcprw",             0xFEA94000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_isp_00",          "fcprw",             0xFEA96000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprw_isp_01",          "fcprw",             0xFEA98000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_rtdmac_00",       "fcprr",             0xFFC71000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcprr_ca53_00",         "fcprr",             0xF12F0000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcpra_vip0",            "fcpra",             0xE7AAA000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcpra_vip1",            "fcpra",             0xE7AB5000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcpra_imp",             "fcpra",             0xFF8DD000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcpra_imr",             "fcpra",             0xFE9CF000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcpra_vio",             "fcpra",             0xFEA9A000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcpra_rtdmac",          "fcpra",             0xFFC79000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcpra_ca53",            "fcpra",             0xF12A0000U, 0x00001000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* SYSTEM RAM */
    {NULL,           "system_ram_00",         "system_ram",        0xE6300000U, 0x00060000U, NULL,                         0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    {.id = OSAL_DEVICE_INVALID_ID} 
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_031]
 * [Covers: XOS3_OSAL_QNX_UD_DD_NON_002]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

#endif /* OSAL_OS_DEV_CFG_V3H1_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
