/***********************************************************************************************************************
* Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.5.0
* Description  : Hardware dependant function for Dummy PM API
***********************************************************************************************************************/
#ifndef R_PMA_HWDEPEND_H
/** Multiple inclusion protection macro */
#define R_PMA_HWDEPEND_H

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************************************************//**
 * @enum e_pma_hwa_id_t
 * Hardware accelerator id.
***********************************************************************************************************************/
typedef enum
{
    R_PMA_HWA_ID_IMP_CORE0              = 0,    /*!< IMP0 */
    R_PMA_HWA_ID_V3M_IMP_CORE01         = R_PMA_HWA_ID_IMP_CORE0,

    R_PMA_HWA_ID_IMP_CORE1              = 1,    /*!< IMP1 */
    R_PMA_HWA_ID_V3M_IMP_CORE23         = R_PMA_HWA_ID_IMP_CORE1,

    R_PMA_HWA_ID_IMP_CORE2              = 2,    /*!< IMP2 */
    R_PMA_HWA_ID_V3M_IMP_OCV_CORE0      = R_PMA_HWA_ID_IMP_CORE2,

    R_PMA_HWA_ID_IMP_CORE3              = 3,    /*!< IMP3 */
    R_PMA_HWA_ID_V3M_IMP_OCV_CORE1      = R_PMA_HWA_ID_IMP_CORE3,

    R_PMA_HWA_ID_IMP_CORE4              = 4,    /*!< IMP4 */
    R_PMA_HWA_ID_V3M_IMP_DP             = R_PMA_HWA_ID_IMP_CORE4,

    R_PMA_HWA_ID_SIMP_CORE              = 5,    /*!< SIMP */
    R_PMA_HWA_ID_V3M_IMP_CNN            = R_PMA_HWA_ID_SIMP_CORE,

    R_PMA_HWA_ID_IMP_OCV_CORE0          = 6,    /*!< IMP-OCV0 */
    R_PMA_HWA_ID_V3M_IMP_RAM            = R_PMA_HWA_ID_IMP_OCV_CORE0,

    R_PMA_HWA_ID_IMP_OCV_CORE1          = 7,    /*!< IMP-OCV1 */
    R_PMA_HWA_ID_V3M_VCP_IVCP1E         = R_PMA_HWA_ID_IMP_OCV_CORE1,

    R_PMA_HWA_ID_IMP_OCV_CORE2          = 8,    /*!< IMP-OCV2 */
    R_PMA_HWA_ID_V3M_ISP                = R_PMA_HWA_ID_IMP_OCV_CORE2,

    R_PMA_HWA_ID_IMP_OCV_CORE3          = 9,    /*!< IMP-OCV3 */
    R_PMA_HWA_ID_V3M_IMR0               = R_PMA_HWA_ID_IMP_OCV_CORE3,

    R_PMA_HWA_ID_IMP_OCV_CORE4          = 10,   /*!< IMP-OCV4 */
    R_PMA_HWA_ID_V3M_IMR1               = R_PMA_HWA_ID_IMP_OCV_CORE4,

    R_PMA_HWA_ID_IMP_DMAC0              = 11,   /*!< IMPDMA0 */
    R_PMA_HWA_ID_V3M_IMR2               = R_PMA_HWA_ID_IMP_DMAC0,

    R_PMA_HWA_ID_IMP_DMAC1              = 12,   /*!< IMPDMA1 */
    R_PMA_HWA_ID_V3M_IMR3               = R_PMA_HWA_ID_IMP_DMAC1,

    R_PMA_HWA_ID_IMP_PSC0               = 13,   /*!< IMPPSC */
    R_PMA_HWA_ID_IMP_PSC                = R_PMA_HWA_ID_IMP_PSC0,
    R_PMA_HWA_ID_V3M_NUM                = R_PMA_HWA_ID_IMP_PSC0,

    R_PMA_HWA_ID_IMP_PSC1               = 14,   /*!< IMPPSC1 */
    R_PMA_HWA_ID_SIMP_DMA               = R_PMA_HWA_ID_IMP_PSC1,

    R_PMA_HWA_ID_IMP_CNN,                       /*!< IMPCNN */
    R_PMA_HWA_ID_IMP_RAM,                       /*!< IMPRAM */
    R_PMA_HWA_ID_VIP_DOF,                       /*!< DOF */
    R_PMA_HWA_ID_VIP_STV,                       /*!< STV */
    R_PMA_HWA_ID_VIP_ACF0,                      /*!< ACF0(CLE0) */
    R_PMA_HWA_ID_VIP_ACF1,                      /*!< ACF1(CLE1) */
    R_PMA_HWA_ID_VIP_ACF2,                      /*!< ACF2(CLE2) */
    R_PMA_HWA_ID_VIP_ACF3,                      /*!< ACF3(CLE3) */
    R_PMA_HWA_ID_VIP_ACF4,                      /*!< ACF4(CLE4) */
    R_PMA_HWA_ID_VCP_IVCP1E,                    /*!< iVCP1E */
    R_PMA_HWA_ID_ISP_CH0,                       /*!< ISP ch0 */
    R_PMA_HWA_ID_ISP_CH1,                       /*!< ISP ch1 */
    R_PMA_HWA_ID_IMR_LX4_CH0,                   /*!< IMR-LX4 ch0 */
    R_PMA_HWA_ID_IMR_LX4_CH1,                   /*!< IMR-LX4 ch1 */
    R_PMA_HWA_ID_IMR_LX4_CH2,                   /*!< IMR-LX4 ch2 */
    R_PMA_HWA_ID_IMR_LX4_CH3,                   /*!< IMR-LX4 ch3 */
    R_PMA_HWA_ID_IMR_LX4_CH4,                   /*!< IMR-LX4 ch4 */
    R_PMA_HWA_ID_IMR_LX4_CH5,                   /*!< IMR-LX4 ch5 */

    /* TODO: DUMMY definition for R-Car V4H */
    R_PMA_HWA_ID_V4H_000,
    R_PMA_HWA_ID_V4H_001,
    R_PMA_HWA_ID_V4H_002,
    R_PMA_HWA_ID_V4H_003,
    R_PMA_HWA_ID_V4H_004,
    R_PMA_HWA_ID_V4H_005,
    R_PMA_HWA_ID_V4H_006,
    R_PMA_HWA_ID_V4H_007,
    R_PMA_HWA_ID_V4H_008,
    R_PMA_HWA_ID_V4H_009,
    R_PMA_HWA_ID_V4H_010,
    R_PMA_HWA_ID_V4H_011,
    R_PMA_HWA_ID_V4H_012,
    R_PMA_HWA_ID_V4H_013,
    R_PMA_HWA_ID_V4H_014,
    R_PMA_HWA_ID_V4H_015,
    R_PMA_HWA_ID_V4H_016,
    R_PMA_HWA_ID_V4H_017,
    R_PMA_HWA_ID_V4H_018,
    R_PMA_HWA_ID_V4H_019,
    R_PMA_HWA_ID_V4H_020,
    R_PMA_HWA_ID_V4H_021,
    R_PMA_HWA_ID_V4H_022,
    R_PMA_HWA_ID_V4H_023,
    R_PMA_HWA_ID_V4H_024,
    R_PMA_HWA_ID_V4H_025,
    R_PMA_HWA_ID_V4H_026,
    R_PMA_HWA_ID_V4H_027,
    R_PMA_HWA_ID_V4H_028,
    R_PMA_HWA_ID_V4H_029,
    R_PMA_HWA_ID_V4H_030,
    R_PMA_HWA_ID_V4H_031,
    R_PMA_HWA_ID_V4H_032,
    R_PMA_HWA_ID_V4H_033,
    R_PMA_HWA_ID_V4H_034,
    R_PMA_HWA_ID_V4H_035,
    R_PMA_HWA_ID_V4H_036,
    R_PMA_HWA_ID_V4H_037,
    R_PMA_HWA_ID_V4H_038,
    R_PMA_HWA_ID_V4H_039,
    R_PMA_HWA_ID_V4H_040,
    R_PMA_HWA_ID_V4H_041,
    R_PMA_HWA_ID_V4H_042,
    R_PMA_HWA_ID_V4H_043,
    R_PMA_HWA_ID_V4H_044,
    R_PMA_HWA_ID_V4H_045,
    R_PMA_HWA_ID_V4H_046,
    R_PMA_HWA_ID_V4H_047,
    R_PMA_HWA_ID_V4H_048,
    R_PMA_HWA_ID_V4H_049,
    R_PMA_HWA_ID_V4H_050,
    R_PMA_HWA_ID_V4H_051,
    R_PMA_HWA_ID_V4H_052,
    R_PMA_HWA_ID_V4H_053,
    R_PMA_HWA_ID_V4H_054,
    R_PMA_HWA_ID_V4H_055,
    R_PMA_HWA_ID_V4H_056,
    R_PMA_HWA_ID_V4H_057,
    R_PMA_HWA_ID_V4H_058,
    R_PMA_HWA_ID_V4H_059,
    R_PMA_HWA_ID_V4H_060,
    R_PMA_HWA_ID_V4H_061,
    R_PMA_HWA_ID_V4H_062,
    R_PMA_HWA_ID_V4H_063,
    R_PMA_HWA_ID_V4H_064,
    R_PMA_HWA_ID_V4H_065,
    R_PMA_HWA_ID_V4H_066,
    R_PMA_HWA_ID_V4H_067,
    R_PMA_HWA_ID_V4H_068,
    R_PMA_HWA_ID_V4H_069,
    R_PMA_HWA_ID_V4H_070,
    R_PMA_HWA_ID_V4H_071,
    R_PMA_HWA_ID_V4H_072,
    R_PMA_HWA_ID_V4H_073,
    R_PMA_HWA_ID_V4H_074,
    R_PMA_HWA_ID_V4H_075,
    R_PMA_HWA_ID_V4H_076,
    R_PMA_HWA_ID_V4H_077,
    R_PMA_HWA_ID_V4H_078,
    R_PMA_HWA_ID_V4H_079,
    R_PMA_HWA_ID_V4H_080,
    R_PMA_HWA_ID_V4H_081,
    R_PMA_HWA_ID_V4H_082,
    R_PMA_HWA_ID_V4H_083,
    R_PMA_HWA_ID_V4H_084,
    R_PMA_HWA_ID_V4H_085,
    R_PMA_HWA_ID_V4H_086,
    R_PMA_HWA_ID_V4H_087,
    R_PMA_HWA_ID_V4H_088,
    R_PMA_HWA_ID_V4H_089,
    R_PMA_HWA_ID_V4H_090,
    R_PMA_HWA_ID_V4H_091,
    R_PMA_HWA_ID_V4H_092,
    R_PMA_HWA_ID_V4H_093,
    R_PMA_HWA_ID_V4H_094,
    R_PMA_HWA_ID_V4H_095,
    R_PMA_HWA_ID_V4H_096,
    R_PMA_HWA_ID_V4H_097,
    R_PMA_HWA_ID_V4H_098,
    R_PMA_HWA_ID_V4H_099,
    R_PMA_HWA_ID_V4H_100,
    R_PMA_HWA_ID_V4H_101,
    R_PMA_HWA_ID_V4H_102,
    R_PMA_HWA_ID_V4H_103,
    R_PMA_HWA_ID_V4H_104,
    R_PMA_HWA_ID_V4H_105,
    R_PMA_HWA_ID_V4H_106,
    R_PMA_HWA_ID_V4H_107,
    R_PMA_HWA_ID_V4H_108,
    R_PMA_HWA_ID_V4H_109,
    R_PMA_HWA_ID_V4H_110,
    R_PMA_HWA_ID_V4H_111,

    R_PMA_HWA_ID_NUM,
    R_PMA_HWA_ID_NONE = R_PMA_HWA_ID_NUM
} e_pma_hwa_id_t;

/*******************************************************************************************************************//**
 * @enum e_pma_hwa_req_t
 * Requirement of Hardware accelerator.
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @enum e_pma_hwa_req_t
***********************************************************************************************************************/
typedef enum
{
    R_PMA_CPG_DOMAIN_ID_IMP0            = 0,    /** IMP0 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMP01       = R_PMA_CPG_DOMAIN_ID_IMP0,

    R_PMA_CPG_DOMAIN_ID_IMP1            = 1,    /** IMP1 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMP23       = R_PMA_CPG_DOMAIN_ID_IMP1,

    R_PMA_CPG_DOMAIN_ID_IMP2            = 2,    /** IMP2 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMP_OCV0    = R_PMA_CPG_DOMAIN_ID_IMP2,

    R_PMA_CPG_DOMAIN_ID_IMP3            = 3,    /** IMP3 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMP_OCV1    = R_PMA_CPG_DOMAIN_ID_IMP3,

    R_PMA_CPG_DOMAIN_ID_IMP4            = 4,    /** IMP4 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMP_DP      = R_PMA_CPG_DOMAIN_ID_IMP4,

    R_PMA_CPG_DOMAIN_ID_SIMP            = 5,    /** SIMP */
    R_PMA_CPG_DOMAIN_ID_V3M_IMP_CNN     = R_PMA_CPG_DOMAIN_ID_SIMP,

    R_PMA_CPG_DOMAIN_ID_IMP_OCV0        = 6,    /** IMP-OCV0 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMP_RAM     = R_PMA_CPG_DOMAIN_ID_IMP_OCV0,

    R_PMA_CPG_DOMAIN_ID_IMP_OCV1        = 7,    /** IMP-OCV1 */
    R_PMA_CPG_DOMAIN_ID_V3M_VCP_IVCP1E  = R_PMA_CPG_DOMAIN_ID_IMP_OCV1,

    R_PMA_CPG_DOMAIN_ID_IMP_OCV2        = 8,    /** IMP-OCV2 */
    R_PMA_CPG_DOMAIN_ID_V3M_ISP         = R_PMA_CPG_DOMAIN_ID_IMP_OCV2,

    R_PMA_CPG_DOMAIN_ID_IMP_OCV3        = 9,    /** IMP-OCV3 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMR0        = R_PMA_CPG_DOMAIN_ID_IMP_OCV3,

    R_PMA_CPG_DOMAIN_ID_IMP_OCV4        = 10,   /** IMP-OCV4 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMR1        = R_PMA_CPG_DOMAIN_ID_IMP_OCV4,

    R_PMA_CPG_DOMAIN_ID_IMP_DMA0        = 11,   /** IMPDMA0 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMR2        = R_PMA_CPG_DOMAIN_ID_IMP_DMA0,

    R_PMA_CPG_DOMAIN_ID_IMP_DMA1        = 12,   /** IMPDMA1 */
    R_PMA_CPG_DOMAIN_ID_V3M_IMR3        = R_PMA_CPG_DOMAIN_ID_IMP_DMA1,

    R_PMA_CPG_DOMAIN_ID_IMP_PSC0        = 13,   /** IMPPSC0 */
    R_PMA_CPG_DOMAIN_ID_IMP_PSC         = R_PMA_CPG_DOMAIN_ID_IMP_PSC0,
    R_PMA_CPG_DOMAIN_ID_V3M_NUM         = R_PMA_CPG_DOMAIN_ID_IMP_PSC0,

    R_PMA_CPG_DOMAIN_ID_IMP_PSC1        = 14,   /** IMPPSC1 */
    R_PMA_CPG_DOMAIN_ID_SIMP_DMA        = R_PMA_CPG_DOMAIN_ID_IMP_PSC1,

    R_PMA_CPG_DOMAIN_ID_IMP_CNN,                /** IMPCNN */
    R_PMA_CPG_DOMAIN_ID_IMP_RAM,                /** IMPRAM */
    R_PMA_CPG_DOMAIN_ID_VIP_DOF,                /** DOF */
    R_PMA_CPG_DOMAIN_ID_VIP_STV,                /** STV */
    R_PMA_CPG_DOMAIN_ID_VIP_ACF0,               /** ACF0 */
    R_PMA_CPG_DOMAIN_ID_VIP_ACF1,               /** ACF1 */
    R_PMA_CPG_DOMAIN_ID_VIP_ACF2,               /** ACF2 */
    R_PMA_CPG_DOMAIN_ID_VIP_ACF3,               /** ACF3 */
    R_PMA_CPG_DOMAIN_ID_VIP_ACF4,               /** ACF4 */
    R_PMA_CPG_DOMAIN_ID_VCP_IVCP1E,             /** iVCP1E */
    R_PMA_CPG_DOMAIN_ID_ISP_CH0,                /** ISP0 */
    R_PMA_CPG_DOMAIN_ID_ISP_CH1,                /** ISP1 */
    R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH0,            /** IMR-LX4 0 */
    R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH1,            /** IMR-LX4 1 */
    R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH2,            /** IMR-LX4 2 */
    R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH3,            /** IMR-LX4 3 */
    R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH4,            /** IMR-LX4 4 */
    R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH5,            /** IMR-LX4 5 */
    R_PMA_CPG_DOMAIN_ID_NUM,                    /** the number of CPG domain */
    R_PMA_CPG_DOMAIN_ID_NONE                    /** No depedent CPG domain */
} e_cpg_domain_id_t;
/***********************************************************************************************************************
 * @enum e_sysc_domain_id_t
***********************************************************************************************************************/
typedef enum
{
    R_PMA_SYSC_DOMAIN_ID_A2CN           = 0,    /** A2CN */
    R_PMA_SYSC_DOMAIN_ID_V3M_A2IR0      = R_PMA_SYSC_DOMAIN_ID_A2CN,

    R_PMA_SYSC_DOMAIN_ID_A2DP0          = 1,    /** A2DP0 */
    R_PMA_SYSC_DOMAIN_ID_V3M_A2IR1      = R_PMA_SYSC_DOMAIN_ID_A2DP0,

    R_PMA_SYSC_DOMAIN_ID_A2DP1          = 2,    /** A2DP1 */
    R_PMA_SYSC_DOMAIN_ID_V3M_A2SC0      = R_PMA_SYSC_DOMAIN_ID_A2DP1,

    R_PMA_SYSC_DOMAIN_ID_A2SC0          = 3,    /** A2SC0 */
    R_PMA_SYSC_DOMAIN_ID_V3M_A2SC1      = R_PMA_SYSC_DOMAIN_ID_A2SC0,

    R_PMA_SYSC_DOMAIN_ID_A2SC1          = 4,    /** A2SC1 */
    R_PMA_SYSC_DOMAIN_ID_V3M_A2DP       = R_PMA_SYSC_DOMAIN_ID_A2SC1,

    R_PMA_SYSC_DOMAIN_ID_A2SC2          = 5,    /** A2SC2 */
    R_PMA_SYSC_DOMAIN_ID_V3M_A2CN       = R_PMA_SYSC_DOMAIN_ID_A2SC2,

    R_PMA_SYSC_DOMAIN_ID_A2SC3          = 6,    /** A2SC3 */
    R_PMA_SYSC_DOMAIN_ID_V3M_A3IR       = R_PMA_SYSC_DOMAIN_ID_A2SC3,

    R_PMA_SYSC_DOMAIN_ID_A2SC4          = 7,    /** A2SC4 */
    R_PMA_SYSC_DOMAIN_ID_V3M_NUM        = R_PMA_SYSC_DOMAIN_ID_A2SC4,

    R_PMA_SYSC_DOMAIN_ID_A2IR0,                 /** A2IR0 */
    R_PMA_SYSC_DOMAIN_ID_A2IR1,                 /** A2IR1 */
    R_PMA_SYSC_DOMAIN_ID_A2IR2,                 /** A2IR2 */
    R_PMA_SYSC_DOMAIN_ID_A2IR3,                 /** A2IR3 */
    R_PMA_SYSC_DOMAIN_ID_A2IR4,                 /** A2IR4 */
    R_PMA_SYSC_DOMAIN_ID_A2IR5,                 /** A2IR5 */
    R_PMA_SYSC_DOMAIN_ID_A3IR,                  /** A3IR */
    R_PMA_SYSC_DOMAIN_ID_A3VIP0,                /** A3VIP0 */
    R_PMA_SYSC_DOMAIN_ID_A3VIP1,                /** A3VIP1 */
    R_PMA_SYSC_DOMAIN_ID_A3VIP2,                /** A3VIP2 */
    R_PMA_SYSC_DOMAIN_ID_NUM,                   /** the number of SYSC domain */
    R_PMA_SYSC_DOMAIN_ID_NONE                   /** No depedent SYSC domain */
} e_sysc_domain_id_t;



#ifdef __cplusplus
}
#endif
#endif /** R_PMA_HWDEPEND_H */
