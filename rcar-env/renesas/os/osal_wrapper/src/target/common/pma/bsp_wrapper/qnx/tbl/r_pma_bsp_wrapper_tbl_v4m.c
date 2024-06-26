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
const int g_pma_bsp_wrap_sysc_id_tbl_v4m[] =
{
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A1CN0,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A1CN0   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A1DSP0,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP0   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A1DSP1,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP1   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A1DSP2,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP2   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A1DSP3,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP3   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A2CN0,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A2CN0    */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A2IMP0,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A2IMP01  */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A2DP0,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A2PSC    */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A2DP1,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A2DMA    */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A2CV0,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A2CV0    */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A2CV1,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A2CV1    */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A2CV2,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A2CV2    */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A2CV3,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A2CV3    */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3VIP0,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP0   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3VIP1,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP1   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3VIP2,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP2   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3DUL,      /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL    */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3ISP0,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3ISP0   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3ISP1,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3ISP1   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_3DGB,       /*    R_PMA_SYSC_DOMAIN_ID_V4M_A23DGB   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_3DGA,       /*    R_PMA_SYSC_DOMAIN_ID_V4M_A33DGA   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3IMR0,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR0   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3IMR1,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR1   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3IMR2,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR2   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3IMR3,     /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR3   */
    R_PMA_BSP_WRAP_SYSC_PDR_GEN4_A3VC        /*    R_PMA_SYSC_DOMAIN_ID_V4M_A3VC     */
};

/* PRQA S 5209 2 # System is requesting an int, so it cannot be changed. */
/* PRQA S 1531,1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const int g_pma_bsp_wrap_cpg_id_tbl_v4m[] =
{
    /* PRQA S 4542,4543,0499 ++ # These are used according to the QNX specification. */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMPPSC0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_IMPPSC    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMPDMA0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_IMPDMAC0  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMP1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_IMP1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMP0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_IMP0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SPMC0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SPMC    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMPCNN0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_IMPCNN  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ISP0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_ISP0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANRT1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_ANRT1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_UMFL0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_UMFL0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SMPS0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SMPS0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SMPO0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SMPO0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RGX,              /*    R_PMA_CPG_DOMAIN_ID_V4M_RGX     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANVI1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_ANVI1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANVI0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_ANVI0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANVC1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_ANVC1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANVC0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_ANVC0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANSP0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_ANSP0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANRT,             /*    R_PMA_CPG_DOMAIN_ID_V4M_ANRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANPV0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_ANPV0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ADVFSC,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ADVFSC  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ADG,              /*    R_PMA_CPG_DOMAIN_ID_V4M_ADG     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SPMI0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SPMI    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMPSLV,           /*    R_PMA_CPG_DOMAIN_ID_V4M_IMPSLV  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMPDTA,           /*    R_PMA_CPG_DOMAIN_ID_V4M_IMPDTA  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXIMP0_AXRS,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AXIMP0_AXRS */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXIMP0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXIMP0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANIMP1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ANIMP1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANIMP0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ANIMP0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMPDMA1,          /*    R_PMA_CPG_DOMAIN_ID_V4M_IMPDMAC1  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSM_AXRS,        /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSM_AXRS */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSD0_1,          /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSD0_1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSD0_0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSD0_0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSC,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSC      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRS_AXSC,        /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSC */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRT_AXRS,        /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRT_AXRS */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRT,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRT      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRD_1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRD_1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRD_0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRD_0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRC_1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRC_1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRC_0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRC_0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRS_AXPV,        /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXPV */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXPV,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AXPV      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANMM_AXMM,        /*    R_PMA_CPG_DOMAIN_ID_V4M_ANMM_AXMM */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AVB2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AVB2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AVB1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AVB1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AVB0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AVB0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ARMREG,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ARMREG  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ARMM,             /*    R_PMA_CPG_DOMAIN_ID_V4M_ARMM    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APSP3_1,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APSP3_1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APSP3_0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APSP3_0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APSP4_1,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APSP4_1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APSP4_0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APSP4_0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APRT0_1,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APRT0_1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APRT0_0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APRT0_0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANVIP2,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ANVIP2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANVIP1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ANVIP1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANVIP0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ANVIP0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CSI4LNK0,         /*    R_PMA_CPG_DOMAIN_ID_V4M_CSITOP0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CSD,              /*    R_PMA_CPG_DOMAIN_ID_V4M_CSDBGPAP  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CR0,              /*    R_PMA_CPG_DOMAIN_ID_V4M_CR0       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CANFD,            /*    R_PMA_CPG_DOMAIN_ID_V4M_CANFD     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVIP1_AXRS,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVIP1_1  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVIP1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVIP1_0  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVIP0_AXRS,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVIP0_AXRS */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVIP0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVIP0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVIFF,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVIFF    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVI1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVI1     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVI1_AXRS,       /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVI1_AXRS */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVI,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVI       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVC_AXRS,        /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVC_AXRS */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVC,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVC     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXTBH,            /*    R_PMA_CPG_DOMAIN_ID_V4M_AXTBH    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSTM,            /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSTM    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSTM_AXRS,       /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSTM_AXRS */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRS_AXSP0,       /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSP0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSP0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSP0     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSN,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSN      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXRS_AXSN,        /*    R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSN */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXSM,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AXSM      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_DSITXLINK0,       /*    R_PMA_CPG_DOMAIN_ID_V4M_DSITXLINK0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_DOC,              /*    R_PMA_CPG_DOMAIN_ID_V4M_DOC2CH    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_DU,               /*    R_PMA_CPG_DOMAIN_ID_V4M_DIS0      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_DDR3_DDR2,        /*    R_PMA_CPG_DOMAIN_ID_V4M_DDR3_DDR2 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_DDR1_DDR0,        /*    R_PMA_CPG_DOMAIN_ID_V4M_DDR1_DDR0 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CSI4LNK1,         /*    R_PMA_CPG_DOMAIN_ID_V4M_CSITOP1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_INTC_AP,          /*    R_PMA_CPG_DOMAIN_ID_V4M_INTAP     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMR1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_IMS1      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMR0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_IMS0      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMR4,             /*    R_PMA_CPG_DOMAIN_ID_V4M_IMR2      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMR3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_IMR1      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMR2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_IMR0      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C5,             /*    R_PMA_CPG_DOMAIN_ID_V4M_I2C5      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C4,             /*    R_PMA_CPG_DOMAIN_ID_V4M_I2C4      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_I2C3      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_I2C2      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_I2C1      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_I2C0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_I2C0      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_HSCIF3,           /*    R_PMA_CPG_DOMAIN_ID_V4M_HSCIF3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_HSCIF2,           /*    R_PMA_CPG_DOMAIN_ID_V4M_HSCIF2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_HSCIF1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_HSCIF1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_HSCIF0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_HSCIF0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_FRAY,             /*    R_PMA_CPG_DOMAIN_ID_V4M_FRAY00    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_FCPVD1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_FCPVD1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_FCPVD0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_FCPVD0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_FCPCS,            /*    R_PMA_CPG_DOMAIN_ID_V4M_FCPCS    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RTDM1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_RTDM1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RTDM0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_RTDM0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RPC,              /*    R_PMA_CPG_DOMAIN_ID_V4M_RPC      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PWM,              /*    R_PMA_CPG_DOMAIN_ID_V4M_PWM      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PCI11,            /*    R_PMA_CPG_DOMAIN_ID_V4M_PCIE10    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PCI01,            /*    R_PMA_CPG_DOMAIN_ID_V4M_PCIE00    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI5,             /*    R_PMA_CPG_DOMAIN_ID_V4M_MSI5     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI4,             /*    R_PMA_CPG_DOMAIN_ID_V4M_MSI4     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_MSI3     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_MSI2     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_MSI1     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MSI0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_MSI0     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IVCP1E,           /*    R_PMA_CPG_DOMAIN_ID_V4M_IVCP1E   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ISPCS1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ISPCS1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ISPCS0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_ISPCS0   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IRQC,             /*    R_PMA_CPG_DOMAIN_ID_V4M_IRQC     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_INTC,             /*    R_PMA_CPG_DOMAIN_ID_V4M_INTTP    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IPC,              /*    R_PMA_CPG_DOMAIN_ID_V4M_IPC      */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN01    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN00    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VCP4L,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VCPL4    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH7,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC7    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH6,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC6    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH5,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC5    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH4,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC4    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH3,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH2,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH1,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_CH0,      /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AES_ACC_WRAPPER,  /*    R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC_WRAP    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TPU,              /*    R_PMA_CPG_DOMAIN_ID_V4M_TPU0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU4,             /*    R_PMA_CPG_DOMAIN_ID_V4M_TMU4    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_TMU3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_TMU2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_TMU1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TMU0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_TMU0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SYDM2,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SYDM2   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SYDM1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SYDM1   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SECROM,           /*    R_PMA_CPG_DOMAIN_ID_V4M_SECROM  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SDHI0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SDHI0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCIF4,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SCIF4    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCIF3,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SCIF3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCIF1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SCIF1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCIF0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SCIF0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RTDM3,            /*    R_PMA_CPG_DOMAIN_ID_V4M_RTDM3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RTDM2,            /*    R_PMA_CPG_DOMAIN_ID_V4M_RTDM2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VSPD1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VSPD1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VSPD0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VSPD0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN17,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN17    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN16,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN16    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN15,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN15    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN14,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN14    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN13,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN13    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN12,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN12    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN11,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN11    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN10,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN10    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN7,             /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN07    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN6,             /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN06    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN5,             /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN05    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN4,             /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN04    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN03    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VIN2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_VIN02    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_UCMT,             /*    R_PMA_CPG_DOMAIN_ID_V4M_UCMT     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TSC,              /*    R_PMA_CPG_DOMAIN_ID_V4M_TSC4_TSC3_TSC2_TSC1 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CMT3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CMT3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CMT2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CMT2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CMT1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CMT1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CMT0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CMT0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RWDT,             /*    R_PMA_CPG_DOMAIN_ID_V4M_WDT1_WDT0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WCRC3,            /*    R_PMA_CPG_DOMAIN_ID_V4M_WCRC3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WCRC2,            /*    R_PMA_CPG_DOMAIN_ID_V4M_WCRC2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WCRC1,            /*    R_PMA_CPG_DOMAIN_ID_V4M_WCRC1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WCRC0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_WCRC0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VSPX0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_VSPX0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RGXC,             /*    R_PMA_CPG_DOMAIN_ID_V4M_RGXC    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RGXB,             /*    R_PMA_CPG_DOMAIN_ID_V4M_RGXB    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SWDT0,            /*    R_PMA_CPG_DOMAIN_ID_V4M_SWDT1_SWDT0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RGXPVC,           /*    R_PMA_CPG_DOMAIN_ID_V4M_RGXPVC  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_RGXPVD,           /*    R_PMA_CPG_DOMAIN_ID_V4M_RGXPVDE */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PCI12P,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PCIE11  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PCI02P,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PCIE01  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_INTAP11,          /*    R_PMA_CPG_DOMAIN_ID_V4M_INTAP11 */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_FRAY01,           /*    R_PMA_CPG_DOMAIN_ID_V4M_FRAY01  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXVI_1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AXVI11  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AULK4P,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AULK4P  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AULK2P,           /*    R_PMA_CPG_DOMAIN_ID_V4M_AULK2P  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_FCPVX0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_FCPVX0  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_KCRC4,            /*    R_PMA_CPG_DOMAIN_ID_V4M_KCRC4   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_FSO,              /*    R_PMA_CPG_DOMAIN_ID_V4M_FSO     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CRC3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CRC3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CRC2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CRC2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CRC1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CRC1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CRC0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CRC0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ADVFS,            /*    R_PMA_CPG_DOMAIN_ID_V4M_ADVFS   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SCMT,             /*    R_PMA_CPG_DOMAIN_ID_V4M_SCMT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC3_2,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC3D3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC3_1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC3D2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC3_0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC3D1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC2_2,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC2D3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC2_1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC2D2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC2_0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC2D1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC1_2,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC1D3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC1_1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC1D2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC1_0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC1D1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC0_2,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC0D3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC0_1,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC0D2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PFC0_0,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PFC0D1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT90,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT9_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT80,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT8_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT70,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT7_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT60,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT6_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT50,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT5_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT40,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT4_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT30,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT3_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT20,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT2_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT10,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT1_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT00,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT0_RCLK    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT91,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT9_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT81,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT8_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT71,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT7_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT61,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT6_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT51,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT5_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT41,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT4_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT31,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT3_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT21,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT2_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT11,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT1_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_WWDT01,           /*    R_PMA_CPG_DOMAIN_ID_V4M_WWDT0_SASYNCRT    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_DBE,              /*    R_PMA_CPG_DOMAIN_ID_V4M_DBE       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_MTI,              /*    R_PMA_CPG_DOMAIN_ID_V4M_MTI       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_KCRC7,            /*    R_PMA_CPG_DOMAIN_ID_V4M_KCRC7     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_KCRC6,            /*    R_PMA_CPG_DOMAIN_ID_V4M_KCRC6     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_KCRC5,            /*    R_PMA_CPG_DOMAIN_ID_V4M_KCRC5     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMMM,            /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMMM     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMIR,            /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMIR     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMVIP,           /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMVIP    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMVC,            /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMVC     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMVIO,           /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMVIO    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMPE,            /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMPE0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMRT,            /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMRT     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKM3DG,           /*    R_PMA_CPG_DOMAIN_ID_V4M_CKM3DG    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APVI4_0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APVI4_1       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APVI4_1,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APVI4_0       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APVC1_0,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APVC1_1       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_APVC1_1,          /*    R_PMA_CPG_DOMAIN_ID_V4M_APVC1_0       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMDSP_CKMIR,     /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMDSP_CKMIR  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMCNR_CKMIR,     /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMCNR_CKMIR  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKM,              /*    R_PMA_CPG_DOMAIN_ID_V4M_CKM           */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMHSC,           /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMHSC        */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CKMCR52,          /*    R_PMA_CPG_DOMAIN_ID_V4M_CKMCR52       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_ANHC,             /*    R_PMA_CPG_DOMAIN_ID_V4M_ANHC          */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXHC_AXRS,        /*    R_PMA_CPG_DOMAIN_ID_V4M_AXHC_AXRS     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_AXHC,             /*    R_PMA_CPG_DOMAIN_ID_V4M_AXHC          */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CSBRG_IR_A2,      /*    R_PMA_CPG_DOMAIN_ID_V4M_CSBRG_IR_A2   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CSBRG_IR_A3,      /*    R_PMA_CPG_DOMAIN_ID_V4M_CSBRG_IR_A3   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_TSN,              /*    R_PMA_CPG_DOMAIN_ID_V4M_TSN           */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMPSDMAC1,        /*    R_PMA_CPG_DOMAIN_ID_V4M_IMPSDMAC1     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_IMPSDMAC0,        /*    R_PMA_CPG_DOMAIN_ID_V4M_IMPSDMAC0     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CVE3,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CVE3    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CVE2,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CVE2    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CVE1,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CVE1    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_CVE0,             /*    R_PMA_CPG_DOMAIN_ID_V4M_CVE0    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_REG,        /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_REG     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_CSB,        /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_CSB     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_SYSPO,      /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_SYSPO   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_APBDBG,     /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_APBDBG  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_OCEM,       /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_OCEM    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_CORE,       /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_CORE    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_SYS,        /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_SYS     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_GLOBAL,     /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_GLOBAL  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_REG,        /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_REG     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_CSB,        /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_CSB     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_FCPRC,            /*    R_PMA_CPG_DOMAIN_ID_V4M_FCPRC         */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PAPSDMA,          /*    R_PMA_CPG_DOMAIN_ID_V4M_PAPSDMA       */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PAPTOP,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP        */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_PAPBUS,           /*    R_PMA_CPG_DOMAIN_ID_V4M_PAPBUS        */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_BUS,        /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_BUS     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP0_BUS,        /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_BUS     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SSI,              /*    R_PMA_CPG_DOMAIN_ID_V4M_SSI    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_SSIU,             /*    R_PMA_CPG_DOMAIN_ID_V4M_SSIU   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_SYSPO,      /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_SYSPO   */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_APBDBG,     /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_APBDBG  */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_OCEM,       /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_OCEM    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_CORE,       /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_CORE    */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_SYS,        /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_SYS     */
    R_PMA_BSP_WRAP_CPG_MSTP_GEN4_VDSP1_GLOBAL      /*    R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_GLOBAL  */
};
