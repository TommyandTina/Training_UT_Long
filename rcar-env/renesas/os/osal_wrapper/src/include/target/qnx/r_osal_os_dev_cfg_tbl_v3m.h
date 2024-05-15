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
* File Name :    r_osal_os_dev_cfg_tbl_v3m.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car V3M
* Description :  Device Configuration file for R-Car V3M
***********************************************************************************************************************/

#ifndef OSAL_OS_DEV_CFG_V3M_H
#define OSAL_OS_DEV_CFG_V3M_H

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_012
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_int_ims
 * @brief osal device configuration : IMS interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ims_v3m[4][1] =
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
 * @var gs_int_iccom
 * @brief osal device configuration : ICCOM interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_iccom_v3m[8][1] =
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
static st_osal_interrupt_info_t gs_int_ivcp1e_v3m[2] =
{
    {300}, {170}
};

/*******************************************************************************************************************//**
 * @var gs_int_imp_top
 * @brief osal device configuration : IMP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imp_top_v3m[5] =
{
    {281}, {282}, {283}, {284}, {285}
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_cisp
 * @brief osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_cisp_v3m[1][1] =
{
    {
        {26}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_tisp
 * @brief osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_tisp_v3m[1][1] =
{
    {
        {25}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_fbc
 * @brief osal device configuration : FBC interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbc_v3m[2] =
{
    {14}, {15}
};

/*******************************************************************************************************************//**
 * @var gs_int_rtt
 * @brief osal device configuration : RTT interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtt_v3m[11][1] =
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
static st_osal_interrupt_info_t gs_int_ipmmu_v3m[1] =
{
    {196}
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_032]
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @var gs_utlb_number_02_v3m
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_02_v3m =
{
    1, { 2 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_03_v3m
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_03_v3m =
{
    1, { 3 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_04_v3m
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_04_v3m =
{
    1, { 4 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_06_v3m
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_06_v3m =
{
    1, { 6 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_07_v3m
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_07_v3m =
{
    1, { 7 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_number_00to01_v3m
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_number_00to01_v3m =
{
    2, { 0, 1 }
};

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_013
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_device_info_v3m
 * @brief osal device configuration : v3m device information structure
***********************************************************************************************************************/
/* PRQA S 3218 1 # Definition within a function reduces readability and maintainability, so no change. */
static const st_osal_device_info_t gs_osal_device_info_v3m[] =
{
    /* group name,   device unique ID,      device type,    physical address,    range,     interrupt,    number of interrupts, power manager id,               POSTCKCR,               AXI bus name,    assignment */
    /* IMR */
    {NULL,           "ims_00",                "ims",               0xFE860000U, 0x00002000U, gs_int_ims_v3m[0],         1U, R_PMA_HWA_ID_V3M_IMR0,           OSAL_PM_POSTCKCR_INVALID, "vi0",      &gs_utlb_number_02_v3m },
    {NULL,           "ims_01",                "ims",               0xFE870000U, 0x00002000U, gs_int_ims_v3m[1],         1U, R_PMA_HWA_ID_V3M_IMR1,           OSAL_PM_POSTCKCR_INVALID, "vi0",      &gs_utlb_number_02_v3m },
    {NULL,           "ims_02",                "ims",               0xFE880000U, 0x00002000U, gs_int_ims_v3m[2],         1U, R_PMA_HWA_ID_V3M_IMR2,           OSAL_PM_POSTCKCR_INVALID, "vi0",      &gs_utlb_number_03_v3m },
    {NULL,           "ims_03",                "ims",               0xFE890000U, 0x00002000U, gs_int_ims_v3m[3],         1U, R_PMA_HWA_ID_V3M_IMR3,           OSAL_PM_POSTCKCR_INVALID, "vi0",      &gs_utlb_number_03_v3m },

    /* ICCOM(IO) */
    {NULL,           "iccom_reg_00",          "iccom_reg",         0xE6260000U, 0x00001000U, NULL,                      0U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* ICCOM(Interrupt) */
    {NULL,           "iccom_00",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3m[0],       1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_01",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3m[1],       1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_02",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3m[2],       1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_03",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3m[3],       1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_04",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3m[4],       1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_05",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3m[5],       1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_06",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3m[6],       1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "iccom_07",              "iccom",             0x00000000U, 0x00000000U, gs_int_iccom_v3m[7],       1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* ENCODER */
    {NULL,           "ivcp1e_00",             "ivcp1e",            0xFEA00000U, 0x00010000U, gs_int_ivcp1e_v3m,         2U, R_PMA_HWA_ID_V3M_VCP_IVCP1E,     OSAL_PM_POSTCKCR_INVALID, "vi0",      &gs_utlb_number_04_v3m },

     /* IMP */
    {NULL,           "imp_top_00",            "imp_top",           0xFFA00000U, 0x00010000U, gs_int_imp_top_v3m,        5U, R_PMA_HWA_ID_V3M_IMP_TOP,        OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_00",                "imp",               0xFF900000U, 0x00020000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_CORE0,      OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_01",                "imp",               0xFF920000U, 0x00020000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_CORE1,      OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_02",                "imp",               0xFF940000U, 0x00020000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_CORE2,      OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_03",                "imp",               0xFF960000U, 0x00020000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_CORE3,      OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_ocv_00",            "imp_ocv",           0xFF980000U, 0x00010000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_OCV_CORE0,  OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_ocv_01",            "imp_ocv",           0xFF990000U, 0x00010000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_OCV_CORE1,  OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_dma_00",            "imp_dma",           0xFFA10000U, 0x00001000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_DMA,        OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_psc_00",            "imp_psc",           0xFFA20000U, 0x00004000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_PSC,        OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_cnn_00",            "imp_cnn",           0xFF9E0000U, 0x00010000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_CNN,        OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },
    {NULL,           "imp_spmi_00",           "imp_spmi",          0xED000000U, 0x000E0000U, NULL,                      0U, R_PMA_HWA_ID_V3M_IMP_SPMI,       OSAL_PM_POSTCKCR_INVALID, "ir",       &gs_utlb_number_00to01_v3m },

    /* ISP */
    {NULL,           "cisp_00",               "cisp",              0xFEC00000U, 0x00100000U, gs_int_isp_cisp_v3m[0],    1U, R_PMA_HWA_ID_V3M_CISP,           OSAL_PM_POSTCKCR_INVALID, "vi0",      &gs_utlb_number_07_v3m },
    {NULL,           "tisp_00",               "tisp",              0xFED00000U, 0x00020000U, gs_int_isp_tisp_v3m[0],    1U, R_PMA_HWA_ID_V3M_TISP,           OSAL_PM_POSTCKCR_INVALID, "vi0",      &gs_utlb_number_06_v3m },

    /* FieldBIST */
    {NULL,           "fbc_00",                "fbc",               0xFF830000U, 0x00002000U, gs_int_fbc_v3m,              2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_ims0",              "fba",               0xFFF80000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_ims1",              "fba",               0xFFF90000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_imp0",              "fba",               0xFF8D0000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_imp1",              "fba",               0xFFF50000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_imp_ocv0",          "fba",               0xFFFA0000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_imp_ocv1",          "fba",               0xFFFB0000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_imp_dp0",           "fba",               0xFFF60000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_imp_cnn0",          "fba",               0xFFF70000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",  NULL},
    {NULL,           "fba_ca53d",             "fba",               0xFF87C000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",  NULL},
    {NULL,           "fba_ca530",             "fba",               0xFF877000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",  NULL},
    {NULL,           "fba_ca531",             "fba",               0xFF878000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",  NULL},

    /* RTT */
    {NULL,           "rfso_00",               "rfso",              0xFFE80000U, 0x00001000U, gs_int_rtt_v3m[0],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_01",               "rfso",              0xFFE81000U, 0x00001000U, gs_int_rtt_v3m[1],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_02",               "rfso",              0xFFE82000U, 0x00001000U, gs_int_rtt_v3m[2],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_03",               "rfso",              0xFFE83000U, 0x00001000U, gs_int_rtt_v3m[3],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_04",               "rfso",              0xFFE84000U, 0x00001000U, gs_int_rtt_v3m[4],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_05",               "rfso",              0xFFE85000U, 0x00001000U, gs_int_rtt_v3m[5],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_06",               "rfso",              0xFFE86000U, 0x00001000U, gs_int_rtt_v3m[6],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_07",               "rfso",              0xFFE87000U, 0x00001000U, gs_int_rtt_v3m[7],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_08",               "rfso",              0xFFE88000U, 0x00001000U, gs_int_rtt_v3m[8],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_09",               "rfso",              0xFFE89000U, 0x00001000U, gs_int_rtt_v3m[9],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "rfso_10",               "rfso",              0xFFE8A000U, 0x00001000U, gs_int_rtt_v3m[10],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},

    /* IPMMU */
    {NULL,           "ipmmu_mm_00",           "ipmmu_mm",          0xE67B0000U, 0x00020000U, gs_int_ipmmu_v3m,            1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "ipmmu_cache_vi0",       "ipmmu_cache",       0xFEBD0000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "ipmmu_cache_ir",        "ipmmu_cache",       0xFF8B0000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "ipmmu_cache_rt0",       "ipmmu_cache",       0xFFC80000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},
    {NULL,           "ipmmu_cache_ds1",       "ipmmu_cache",       0xE7740000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},

    /* SYSTEM RAM */
    {NULL,           "system_ram_00",         "system_ram",        0xE6300000U, 0x00070000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",  NULL},

    {.id = OSAL_DEVICE_INVALID_ID}
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_031]
 * [Covers: XOS3_OSAL_QNX_UD_DD_NON_002]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

#endif /* OSAL_OS_DEV_CFG_V3M_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
