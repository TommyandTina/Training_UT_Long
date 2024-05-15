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
* Description  : BSP Wrapper information table for PM API
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
/* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
/* PRQA S 1531,1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const int g_pma_bsp_wrap_sysc_id_tbl_s4[] = {0};

/* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
/* PRQA S 1531,1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const int g_pma_bsp_wrap_cpg_id_tbl_s4[] =
{
    /* PRQA S 4542,4543,0499 ++ # These are used according to the QNX specification. */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CSD,      /*    R_PMA_CPG_DOMAIN_ID_S4_CSDBGPAP  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CR0,      /*    R_PMA_CPG_DOMAIN_ID_S4_CR0     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_DDR1_DDR0,/*    R_PMA_CPG_DOMAIN_ID_S4_DDR1_DDR0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C5,     /*    R_PMA_CPG_DOMAIN_ID_S4_I2C5    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C4,     /*    R_PMA_CPG_DOMAIN_ID_S4_I2C4    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C3,     /*    R_PMA_CPG_DOMAIN_ID_S4_I2C3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C2,     /*    R_PMA_CPG_DOMAIN_ID_S4_I2C2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C1,     /*    R_PMA_CPG_DOMAIN_ID_S4_I2C1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C0,     /*    R_PMA_CPG_DOMAIN_ID_S4_I2C0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_HSCIF3,   /*    R_PMA_CPG_DOMAIN_ID_S4_HSCIF3  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_HSCIF2,   /*    R_PMA_CPG_DOMAIN_ID_S4_HSCIF2  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_HSCIF1,   /*    R_PMA_CPG_DOMAIN_ID_S4_HSCIF1  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_HSCIF0,   /*    R_PMA_CPG_DOMAIN_ID_S4_HSCIF0  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RTDM1,    /*    R_PMA_CPG_DOMAIN_ID_S4_RTDM1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RTDM0,    /*    R_PMA_CPG_DOMAIN_ID_S4_RTDM0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RPC,      /*    R_PMA_CPG_DOMAIN_ID_S4_RPC     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PCI11,    /*    R_PMA_CPG_DOMAIN_ID_S4_PCIE10  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PCI01,    /*    R_PMA_CPG_DOMAIN_ID_S4_PCIE00  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI3,     /*    R_PMA_CPG_DOMAIN_ID_S4_MSI3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI2,     /*    R_PMA_CPG_DOMAIN_ID_S4_MSI2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI1,     /*    R_PMA_CPG_DOMAIN_ID_S4_MSI1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI0,     /*    R_PMA_CPG_DOMAIN_ID_S4_MSI0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH7,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC7    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH6,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC6    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH5,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC5    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH4,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC4    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH3,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH2,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH1,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH0,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_WRAPPER,    /*    R_PMA_CPG_DOMAIN_ID_S4_AES_ACC_WRAPPER    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SHIPS0,   /*    R_PMA_CPG_DOMAIN_ID_S4_SHIP_S0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU4,     /*    R_PMA_CPG_DOMAIN_ID_S4_TMU4    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU3,     /*    R_PMA_CPG_DOMAIN_ID_S4_TMU3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU2,     /*    R_PMA_CPG_DOMAIN_ID_S4_TMU2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU1,     /*    R_PMA_CPG_DOMAIN_ID_S4_TMU1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU0,     /*    R_PMA_CPG_DOMAIN_ID_S4_TMU0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SYDM2,    /*    R_PMA_CPG_DOMAIN_ID_S4_SYDM2   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SYDM1,    /*    R_PMA_CPG_DOMAIN_ID_S4_SYDM1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SECROM,   /*    R_PMA_CPG_DOMAIN_ID_S4_SDHI0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SDHI0,    /*    R_PMA_CPG_DOMAIN_ID_S4_SDHI     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCIF4,    /*    R_PMA_CPG_DOMAIN_ID_S4_SCIF4    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCIF3,    /*    R_PMA_CPG_DOMAIN_ID_S4_SCIF3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCIF1,    /*    R_PMA_CPG_DOMAIN_ID_S4_SCIF1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCIF0,    /*    R_PMA_CPG_DOMAIN_ID_S4_SCIF0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RTDM3,    /*    R_PMA_CPG_DOMAIN_ID_S4_RTDM3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RTDM2,    /*    R_PMA_CPG_DOMAIN_ID_S4_RTDM2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_UCMT,     /*    R_PMA_CPG_DOMAIN_ID_S4_UCMT     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TSC,      /*    R_PMA_CPG_DOMAIN_ID_S4_TSC4_TSC3_TSC2_TSC1  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC0,     /*    R_PMA_CPG_DOMAIN_ID_S4_PFC0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CMT3,     /*    R_PMA_CPG_DOMAIN_ID_S4_CMT3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CMT2,     /*    R_PMA_CPG_DOMAIN_ID_S4_CMT2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CMT1,     /*    R_PMA_CPG_DOMAIN_ID_S4_CMT1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CMT0,     /*    R_PMA_CPG_DOMAIN_ID_S4_CMT0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RWDT,     /*    R_PMA_CPG_DOMAIN_ID_S4_WDT1_WDT0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WCRC3,    /*    R_PMA_CPG_DOMAIN_ID_S4_WCRC3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WCRC2,    /*    R_PMA_CPG_DOMAIN_ID_S4_WCRC2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WCRC1,    /*    R_PMA_CPG_DOMAIN_ID_S4_WCRC1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WCRC0,    /*    R_PMA_CPG_DOMAIN_ID_S4_WCRC0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SWDT0,    /*    R_PMA_CPG_DOMAIN_ID_S4_SWDT1_SWDT0  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PCI12P,   /*    R_PMA_CPG_DOMAIN_ID_S4_PCIE11  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PCI02P,   /*    R_PMA_CPG_DOMAIN_ID_S4_PCIE01  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AULK2P,   /*    R_PMA_CPG_DOMAIN_ID_S4_AULK2P  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_KCRC4,    /*    R_PMA_CPG_DOMAIN_ID_S4_KCRC4   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CRC3,     /*    R_PMA_CPG_DOMAIN_ID_S4_CRC3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CRC2,     /*    R_PMA_CPG_DOMAIN_ID_S4_CRC2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CRC1,     /*    R_PMA_CPG_DOMAIN_ID_S4_CRC1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CRC0,     /*    R_PMA_CPG_DOMAIN_ID_S4_CRC0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCMT,     /*    R_PMA_CPG_DOMAIN_ID_S4_SCMT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC0_2,   /*    R_PMA_CPG_DOMAIN_ID_S4_PFC0D3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC0_1,   /*    R_PMA_CPG_DOMAIN_ID_S4_PFC0D2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC0_0,   /*    R_PMA_CPG_DOMAIN_ID_S4_PFC0D1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT90,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT9_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT80,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT8_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT70,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT7_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT60,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT6_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT50,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT5_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT40,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT4_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT30,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT3_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT20,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT2_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT10,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT1_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT00,   /*    R_PMA_CPG_DOMAIN_ID_S4_WWDT0_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_KCRC7,    /*    R_PMA_CPG_DOMAIN_ID_S4_KCRC7      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_KCRC6,    /*    R_PMA_CPG_DOMAIN_ID_S4_KCRC6      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_KCRC5,    /*    R_PMA_CPG_DOMAIN_ID_S4_KCRC5      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMMM,    /*    R_PMA_CPG_DOMAIN_ID_S4_CKMMM      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMPE,    /*    R_PMA_CPG_DOMAIN_ID_S4_CKMPE0     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMRT,    /*    R_PMA_CPG_DOMAIN_ID_S4_CKMRT      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKM,      /*    R_PMA_CPG_DOMAIN_ID_S4_CKM        */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMHSC,   /*    R_PMA_CPG_DOMAIN_ID_S4_CKMHSC     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMCR52,  /*    R_PMA_CPG_DOMAIN_ID_S4_CKMCR52    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_UFS0,     /*    R_PMA_CPG_DOMAIN_ID_S4_UFS        */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SHIPS2,   /*    R_PMA_CPG_DOMAIN_ID_S4_SHIP_S2        */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SHIPS1,   /*    R_PMA_CPG_DOMAIN_ID_S4_SHIP_S1        */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ETHPHY,   /*    R_PMA_CPG_DOMAIN_ID_S4_ETHER_TSN_IF   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RSW2,     /*    R_PMA_CPG_DOMAIN_ID_S4_R_SWITCH2      */
};
