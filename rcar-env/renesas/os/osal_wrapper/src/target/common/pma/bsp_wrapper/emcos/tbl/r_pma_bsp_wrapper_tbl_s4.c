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
* Version :      0.0.1
* Description  : BSP Wrapper information table for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h"
#include <drv/sysc/sysc.h>
#include <drv/cpg/cpg_driver.h>

/*======================================================================================================================
Private global variables
======================================================================================================================*/
/* PRQA S 1531,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const uint32_t g_pma_bsp_wrap_sysc_id_tbl_s4[] = {0};

/* PRQA S 1531,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const uint32_t g_pma_bsp_wrap_cpg_id_tbl_s4[] =
{
    /* PRQA S 4542,4543,0499 ++ # Cannot be avoided due to the definition of BSP */
    CPGDRV_MSSR3_NAME_CSDBGPAP,     /*    R_PMA_CPG_DOMAIN_ID_S4_CSDBGPAP  */
    CPGDRV_MSSR3_NAME_CR0,          /*    R_PMA_CPG_DOMAIN_ID_S4_CR0       */
    CPGDRV_MSSR4_NAME_DDR1_DDR0,    /*    R_PMA_CPG_DOMAIN_ID_S4_DDR1_DDR0 */
    CPGDRV_MSSR5_NAME_I2C5,         /*    R_PMA_CPG_DOMAIN_ID_S4_I2C5      */
    CPGDRV_MSSR5_NAME_I2C4,         /*    R_PMA_CPG_DOMAIN_ID_S4_I2C4      */
    CPGDRV_MSSR5_NAME_I2C3,         /*    R_PMA_CPG_DOMAIN_ID_S4_I2C3      */
    CPGDRV_MSSR5_NAME_I2C2,         /*    R_PMA_CPG_DOMAIN_ID_S4_I2C2      */
    CPGDRV_MSSR5_NAME_I2C1,         /*    R_PMA_CPG_DOMAIN_ID_S4_I2C1      */
    CPGDRV_MSSR5_NAME_I2C0,         /*    R_PMA_CPG_DOMAIN_ID_S4_I2C0      */
    CPGDRV_MSSR5_NAME_HSCIF3,       /*    R_PMA_CPG_DOMAIN_ID_S4_HSCIF3    */
    CPGDRV_MSSR5_NAME_HSCIF2,       /*    R_PMA_CPG_DOMAIN_ID_S4_HSCIF2    */
    CPGDRV_MSSR5_NAME_HSCIF1,       /*    R_PMA_CPG_DOMAIN_ID_S4_HSCIF1    */
    CPGDRV_MSSR5_NAME_HSCIF0,       /*    R_PMA_CPG_DOMAIN_ID_S4_HSCIF0    */
    CPGDRV_MSSR6_NAME_RTDM1,        /*    R_PMA_CPG_DOMAIN_ID_S4_RTDM1     */
    CPGDRV_MSSR6_NAME_RTDM0,        /*    R_PMA_CPG_DOMAIN_ID_S4_RTDM0     */
    CPGDRV_MSSR6_NAME_RPC,          /*    R_PMA_CPG_DOMAIN_ID_S4_RPC       */
    CPGDRV_MSSR6_NAME_PCIE1,        /*    R_PMA_CPG_DOMAIN_ID_S4_PCIE10    */
    CPGDRV_MSSR6_NAME_PCIE0,        /*    R_PMA_CPG_DOMAIN_ID_S4_PCIE00    */
    CPGDRV_MSSR6_NAME_MSI3,         /*    R_PMA_CPG_DOMAIN_ID_S4_MSI3      */
    CPGDRV_MSSR6_NAME_MSI2,         /*    R_PMA_CPG_DOMAIN_ID_S4_MSI2      */
    CPGDRV_MSSR6_NAME_MSI1,         /*    R_PMA_CPG_DOMAIN_ID_S4_MSI1      */
    CPGDRV_MSSR6_NAME_MSI0,         /*    R_PMA_CPG_DOMAIN_ID_S4_MSI0      */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC7  */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC6  */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC5  */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC4  */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC3  */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC2  */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC1  */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC0  */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC_WRAPPER   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_SHIP_S0 */
    CPGDRV_MSSR7_NAME_TMU4,         /*    R_PMA_CPG_DOMAIN_ID_S4_TMU4    */
    CPGDRV_MSSR7_NAME_TMU3,         /*    R_PMA_CPG_DOMAIN_ID_S4_TMU3    */
    CPGDRV_MSSR7_NAME_TMU2,         /*    R_PMA_CPG_DOMAIN_ID_S4_TMU2    */
    CPGDRV_MSSR7_NAME_TMU1,         /*    R_PMA_CPG_DOMAIN_ID_S4_TMU1    */
    CPGDRV_MSSR7_NAME_TMU0,         /*    R_PMA_CPG_DOMAIN_ID_S4_TMU0    */
    CPGDRV_MSSR7_NAME_SYDM2,        /*    R_PMA_CPG_DOMAIN_ID_S4_SYDM2   */
    CPGDRV_MSSR7_NAME_SYDM1,        /*    R_PMA_CPG_DOMAIN_ID_S4_SYDM1   */
    CPGDRV_MSSR7_NAME_SDHI0,        /*    R_PMA_CPG_DOMAIN_ID_S4_SDHI0   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_SDHI    */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_SCIF4   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_SCIF3   */
    CPGDRV_MSSR7_NAME_SCIF1,        /*    R_PMA_CPG_DOMAIN_ID_S4_SCIF1   */
    CPGDRV_MSSR7_NAME_SCIF0,        /*    R_PMA_CPG_DOMAIN_ID_S4_SCIF0   */
    CPGDRV_MSSR7_NAME_RTDM3,        /*    R_PMA_CPG_DOMAIN_ID_S4_RTDM3   */
    CPGDRV_MSSR7_NAME_RTDM2,        /*    R_PMA_CPG_DOMAIN_ID_S4_RTDM2   */
    CPGDRV_MSSR9_NAME_UCMT,         /*    R_PMA_CPG_DOMAIN_ID_S4_UCMT    */
    CPGDRV_MSSR9_NAME_TSC4_TSC3_TSC2_TSC1,  /*    R_PMA_CPG_DOMAIN_ID_S4_TSC4_TSC3_TSC2_TSC1 */
    CPGDRV_MSSR9_NAME_PFC0,         /*    R_PMA_CPG_DOMAIN_ID_S4_PFC0    */
    CPGDRV_MSSR9_NAME_CMT3,         /*    R_PMA_CPG_DOMAIN_ID_S4_CMT3    */
    CPGDRV_MSSR9_NAME_CMT2,         /*    R_PMA_CPG_DOMAIN_ID_S4_CMT2    */
    CPGDRV_MSSR9_NAME_CMT1,         /*    R_PMA_CPG_DOMAIN_ID_S4_CMT1    */
    CPGDRV_MSSR9_NAME_CMT0,         /*    R_PMA_CPG_DOMAIN_ID_S4_CMT0    */
    CPGDRV_MSSR9_NAME_WDT1_WDT0,    /*    R_PMA_CPG_DOMAIN_ID_S4_WDT1_WDT0 */
    CPGDRV_MSSR9_NAME_WCRC3,        /*    R_PMA_CPG_DOMAIN_ID_S4_WCRC3     */
    CPGDRV_MSSR9_NAME_WCRC2,        /*    R_PMA_CPG_DOMAIN_ID_S4_WCRC2     */
    CPGDRV_MSSR9_NAME_WCRC1,        /*    R_PMA_CPG_DOMAIN_ID_S4_WCRC1     */
    CPGDRV_MSSR9_NAME_WCRC0,        /*    R_PMA_CPG_DOMAIN_ID_S4_WCRC0     */
    CPGDRV_MSSR11_NAME_SWDT1_SWDT0, /*    R_PMA_CPG_DOMAIN_ID_S4_SWDT1_SWDT0   */
    CPGDRV_MSSR11_NAME_PCIE1,       /*    R_PMA_CPG_DOMAIN_ID_S4_PCIE11    */
    CPGDRV_MSSR11_NAME_PCIE0,       /*    R_PMA_CPG_DOMAIN_ID_S4_PCIE01    */
    CPGDRV_MSSR11_NAME_AULK2P,      /*    R_PMA_CPG_DOMAIN_ID_S4_AULK2P    */
    CPGDRV_MSSR12_NAME_KCRC4,       /*    R_PMA_CPG_DOMAIN_ID_S4_KCRC4     */
    CPGDRV_MSSR12_NAME_CRC3,        /*    R_PMA_CPG_DOMAIN_ID_S4_CRC3      */
    CPGDRV_MSSR12_NAME_CRC2,        /*    R_PMA_CPG_DOMAIN_ID_S4_CRC2      */
    CPGDRV_MSSR12_NAME_CRC1,        /*    R_PMA_CPG_DOMAIN_ID_S4_CRC1      */
    CPGDRV_MSSR12_NAME_CRC0,        /*    R_PMA_CPG_DOMAIN_ID_S4_CRC0      */
    CPGDRV_MSSR12_NAME_SCMT,        /*    R_PMA_CPG_DOMAIN_ID_S4_SCMT      */
    CPGDRV_MSSR12_NAME_PFC0D3,      /*    R_PMA_CPG_DOMAIN_ID_S4_PFC0D3    */
    CPGDRV_MSSR12_NAME_PFC0D2,      /*    R_PMA_CPG_DOMAIN_ID_S4_PFC0D2    */
    CPGDRV_MSSR12_NAME_PFC0D1,      /*    R_PMA_CPG_DOMAIN_ID_S4_PFC0D1    */
    CPGDRV_MSSR12_NAME_WWDT9_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT9_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT8_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT8_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT7_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT7_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT6_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT6_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT5_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT5_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT4_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT4_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT3_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT3_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT2_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT2_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT1_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT1_RCLK    */
    CPGDRV_MSSR12_NAME_WWDT0_RCLK,  /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT0_RCLK    */
    CPGDRV_MSSR13_NAME_KCRC7,       /*    R_PMA_CPG_DOMAIN_ID_S4_KCRC7     */
    CPGDRV_MSSR13_NAME_KCRC6,       /*    R_PMA_CPG_DOMAIN_ID_S4_KCRC6     */
    CPGDRV_MSSR13_NAME_KCRC5,       /*    R_PMA_CPG_DOMAIN_ID_S4_KCRC5     */
    CPGDRV_MSSR14_NAME_CKMMM,       /*    R_PMA_CPG_DOMAIN_ID_S4_CKMMM     */
    CPGDRV_MSSR14_NAME_CKMPE0,      /*    R_PMA_CPG_DOMAIN_ID_S4_CKMPE0    */
    CPGDRV_MSSR14_NAME_CKMRT,       /*    R_PMA_CPG_DOMAIN_ID_S4_CKMRT     */
    CPGDRV_MSSR15_NAME_CKM,         /*    R_PMA_CPG_DOMAIN_ID_S4_CKM       */
    CPGDRV_MSSR15_NAME_CKMHSC,      /*    R_PMA_CPG_DOMAIN_ID_S4_CKMHSC    */
    CPGDRV_MSSR15_NAME_CKMCR52,     /*    R_PMA_CPG_DOMAIN_ID_S4_CKMCR52   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_UFS       */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_SHIP_S2   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_SHIP_S1   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /*    R_PMA_CPG_DOMAIN_ID_S4_ETHER_TSN_IF  */
    R_PMA_BSP_WRAP_UNDEFINED_ID     /*    R_PMA_CPG_DOMAIN_ID_S4_R_SWITCH2 */
};

/* CPGDRV_FRQ_NAME_POST[X] */
const uint32_t g_pma_bsp_wrap_post_clk_id_tbl_s4[R_PMA_MNG_CLK_POSTCKCR] =
{
    CPGDRV_FRQ_NAME_POST,
    CPGDRV_FRQ_NAME_POST2,
    R_PMA_BSP_WRAP_UNDEFINED_ID,
    R_PMA_BSP_WRAP_UNDEFINED_ID
};
/* CPGDRV_POST[X]_DIV[YY} */
const uint32_t g_pma_bsp_wrap_cpg_div0_id_tbl_s4[(R_PMA_MNG_CLK_DIVIDER-1)] =
{
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x1   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x2   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x3   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x4   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x5   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x6   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x7   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x8   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x9   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0xA   */
    CPGDRV_POST_DIV48,              /* 0xB   */
    CPGDRV_POST_DIV52,              /* 0xC   */
    CPGDRV_POST_DIV56,              /* 0xD   */
    CPGDRV_POST_DIV60,              /* 0xE   */
    CPGDRV_POST_DIV64,              /* 0xF   */
    CPGDRV_POST_DIV68,              /* 0x10  */
    CPGDRV_POST_DIV72,              /* 0x11  */
    CPGDRV_POST_DIV76,              /* 0x12  */
    CPGDRV_POST_DIV80,              /* 0x13  */
    CPGDRV_POST_DIV84,              /* 0x14  */
    CPGDRV_POST_DIV88,              /* 0x15  */
    CPGDRV_POST_DIV92,              /* 0x16  */
    CPGDRV_POST_DIV96,              /* 0x17  */
    CPGDRV_POST_DIV100,             /* 0x18  */
    CPGDRV_POST_DIV104,             /* 0x19  */
    CPGDRV_POST_DIV108,             /* 0x1A  */
    CPGDRV_POST_DIV112,             /* 0x1B  */
    CPGDRV_POST_DIV116,             /* 0x1C  */
    CPGDRV_POST_DIV120,             /* 0x1D  */
    CPGDRV_POST_DIV124,             /* 0x1E  */
    CPGDRV_POST_DIV128,             /* 0x1F  */
    CPGDRV_POST_DIV132,             /* 0x20  */
    CPGDRV_POST_DIV136,             /* 0x21  */
    CPGDRV_POST_DIV140,             /* 0x22  */
    CPGDRV_POST_DIV144,             /* 0x23  */
    CPGDRV_POST_DIV148,             /* 0x24  */
    CPGDRV_POST_DIV152,             /* 0x25  */
    CPGDRV_POST_DIV156,             /* 0x26  */
    CPGDRV_POST_DIV160,             /* 0x27  */
    CPGDRV_POST_DIV164,             /* 0x28  */
    CPGDRV_POST_DIV168,             /* 0x29  */
    CPGDRV_POST_DIV172,             /* 0x2A  */
    CPGDRV_POST_DIV176,             /* 0x2B  */
    CPGDRV_POST_DIV180,             /* 0x2C  */
    CPGDRV_POST_DIV184,             /* 0x2D  */
    CPGDRV_POST_DIV188,             /* 0x2E  */
    CPGDRV_POST_DIV192,             /* 0x2F  */
    CPGDRV_POST_DIV196,             /* 0x30  */
    CPGDRV_POST_DIV200,             /* 0x31  */
    CPGDRV_POST_DIV204,             /* 0x32  */
    CPGDRV_POST_DIV208,             /* 0x33  */
    CPGDRV_POST_DIV212,             /* 0x34  */
    CPGDRV_POST_DIV216,             /* 0x35  */
    CPGDRV_POST_DIV220,             /* 0x36  */
    CPGDRV_POST_DIV224,             /* 0x37  */
    CPGDRV_POST_DIV228,             /* 0x38  */
    CPGDRV_POST_DIV232,             /* 0x39  */
    CPGDRV_POST_DIV236,             /* 0x3A  */
    CPGDRV_POST_DIV240,             /* 0x3B  */
    CPGDRV_POST_DIV244,             /* 0x3C  */
    CPGDRV_POST_DIV248,             /* 0x3D  */
    CPGDRV_POST_DIV252,             /* 0x3E  */
    CPGDRV_POST_DIV256              /* 0x3F  */
};

const uint32_t g_pma_bsp_wrap_cpg_div2_id_tbl_s4[(R_PMA_MNG_CLK_DIVIDER-1)] =
{
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x1   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x2   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x3   */
    R_PMA_BSP_WRAP_UNDEFINED_ID,    /* 0x4   */
    CPGDRV_POST2_DIV24,             /* 0x5   */
    CPGDRV_POST2_DIV28,             /* 0x6   */
    CPGDRV_POST2_DIV32,             /* 0x7   */
    CPGDRV_POST2_DIV36,             /* 0x8   */
    CPGDRV_POST2_DIV40,             /* 0x9   */
    CPGDRV_POST2_DIV44,             /* 0xA   */
    CPGDRV_POST2_DIV48,              /* 0xB   */
    CPGDRV_POST2_DIV52,              /* 0xC   */
    CPGDRV_POST2_DIV56,              /* 0xD   */
    CPGDRV_POST2_DIV60,              /* 0xE   */
    CPGDRV_POST2_DIV64,              /* 0xF   */
    CPGDRV_POST2_DIV68,              /* 0x10  */
    CPGDRV_POST2_DIV72,              /* 0x11  */
    CPGDRV_POST2_DIV76,              /* 0x12  */
    CPGDRV_POST2_DIV80,              /* 0x13  */
    CPGDRV_POST2_DIV84,              /* 0x14  */
    CPGDRV_POST2_DIV88,              /* 0x15  */
    CPGDRV_POST2_DIV92,              /* 0x16  */
    CPGDRV_POST2_DIV96,              /* 0x17  */
    CPGDRV_POST2_DIV100,             /* 0x18  */
    CPGDRV_POST2_DIV104,             /* 0x19  */
    CPGDRV_POST2_DIV108,             /* 0x1A  */
    CPGDRV_POST2_DIV112,             /* 0x1B  */
    CPGDRV_POST2_DIV116,             /* 0x1C  */
    CPGDRV_POST2_DIV120,             /* 0x1D  */
    CPGDRV_POST2_DIV124,             /* 0x1E  */
    CPGDRV_POST2_DIV128,             /* 0x1F  */
    CPGDRV_POST2_DIV132,             /* 0x20  */
    CPGDRV_POST2_DIV136,             /* 0x21  */
    CPGDRV_POST2_DIV140,             /* 0x22  */
    CPGDRV_POST2_DIV144,             /* 0x23  */
    CPGDRV_POST2_DIV148,             /* 0x24  */
    CPGDRV_POST2_DIV152,             /* 0x25  */
    CPGDRV_POST2_DIV156,             /* 0x26  */
    CPGDRV_POST2_DIV160,             /* 0x27  */
    CPGDRV_POST2_DIV164,             /* 0x28  */
    CPGDRV_POST2_DIV168,             /* 0x29  */
    CPGDRV_POST2_DIV172,             /* 0x2A  */
    CPGDRV_POST2_DIV176,             /* 0x2B  */
    CPGDRV_POST2_DIV180,             /* 0x2C  */
    CPGDRV_POST2_DIV184,             /* 0x2D  */
    CPGDRV_POST2_DIV188,             /* 0x2E  */
    CPGDRV_POST2_DIV192,             /* 0x2F  */
    CPGDRV_POST2_DIV196,             /* 0x30  */
    CPGDRV_POST2_DIV200,             /* 0x31  */
    CPGDRV_POST2_DIV204,             /* 0x32  */
    CPGDRV_POST2_DIV208,             /* 0x33  */
    CPGDRV_POST2_DIV212,             /* 0x34  */
    CPGDRV_POST2_DIV216,             /* 0x35  */
    CPGDRV_POST2_DIV220,             /* 0x36  */
    CPGDRV_POST2_DIV224,             /* 0x37  */
    CPGDRV_POST2_DIV228,             /* 0x38  */
    CPGDRV_POST2_DIV232,             /* 0x39  */
    CPGDRV_POST2_DIV236,             /* 0x3A  */
    CPGDRV_POST2_DIV240,             /* 0x3B  */
    CPGDRV_POST2_DIV244,             /* 0x3C  */
    CPGDRV_POST2_DIV248,             /* 0x3D  */
    CPGDRV_POST2_DIV252,             /* 0x3E  */
    CPGDRV_POST2_DIV256              /* 0x3F  */
};

const uint32_t g_pma_bsp_wrap_cpg_div3_id_tbl_s4[(R_PMA_MNG_CLK_DIVIDER-1)] = {R_PMA_BSP_WRAP_UNDEFINED_ID};
const uint32_t g_pma_bsp_wrap_cpg_div4_id_tbl_s4[(R_PMA_MNG_CLK_DIVIDER-1)] = {R_PMA_BSP_WRAP_UNDEFINED_ID};
