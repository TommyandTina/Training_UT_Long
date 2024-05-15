/***********************************************************************************************************************
* Copyright [2023-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.2.0
* Description  : Hardware information table for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "target/common/pma/hw/r_pma_hwdepend.h"

/*======================================================================================================================
Private macr definitios
======================================================================================================================*/
/* Bit assignment of HWA that has dependency */
#define R_PMA_HW_DEP_HWA_GROUP_0                (uint64_t)(0ULL << 63ULL)   /* Bit assignment group 0 */
#define R_PMA_HW_DEP_HWA_GROUP_1                (uint64_t)(1ULL << 63ULL)   /* Bit assignment group 1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_NONE           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(0ULL)            /* No dependency */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMPCNN         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 0ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_IMPCNN */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SPMC           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 1ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_SPMC */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMP0           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 2ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_IMP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMP1           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 3ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_IMP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMPDMAC0       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 4ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_IMPDMAC0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMPDMAC1       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 5ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_IMPDMAC1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMPPSC0        R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 6ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_IMPPSC */
#define R_PMA_HW_DEP_HWA_BIT_V4M_CVE0           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 7ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_CVE0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_CVE1           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 8ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_CVE1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_CVE2           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 9ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_CVE2 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_CVE3           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 10ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_CVE3 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMPDTA         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 11ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_IMPDTA */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMPSLV         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 12ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_IMPSLV */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SPMI           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 13ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V4M_VDSP0_CSB      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 14ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_VDSP0_CSB */
#define R_PMA_HW_DEP_HWA_BIT_V4M_VDSP0_BUS      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 15ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_VDSP0_BUS */
#define R_PMA_HW_DEP_HWA_BIT_V4M_VDSP1_CSB      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 16ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_VDSP1_CSB */
#define R_PMA_HW_DEP_HWA_BIT_V4M_VDSP1_BUS      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 17ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_VDSP1_BUS */
#define R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMI       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 18ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_REG_SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMC       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 19ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_REG_SPMC */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SPMC01         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 20ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_SPMC01 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP0       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 21ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP1       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 22ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV0   R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 23ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_OCV0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV1   R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 24ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_OCV1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV2   R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 25ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_OCV2 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV3   R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 26ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_OCV3 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DP0    R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 27ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_DP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DP1    R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 28ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_DP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNN0   R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 29ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_CNN0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DSP0   R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 30ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_DSP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DSP1   R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 31ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_DSP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNRAM0 R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 32ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0 */

#define R_PMA_HW_DEP_HWA_BIT_V4M_SMPS0          R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 0ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_SMPS0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SMPO0          R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 1ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_SMPO0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_DOF            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 2ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_DOF */
#define R_PMA_HW_DEP_HWA_BIT_V4M_DOF_FCPRM      R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 3ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_DOF_FCPRM */
#define R_PMA_HW_DEP_HWA_BIT_V4M_PAPSDMA        R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 4ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_PAPSDMA */
#define R_PMA_HW_DEP_HWA_BIT_V4M_HSM            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 5ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_HSM */
#define R_PMA_HW_DEP_HWA_BIT_V4M_PAPBUS         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 6ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_PAPBUS */
#define R_PMA_HW_DEP_HWA_BIT_V4M_DCM            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 7ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_DCM */
#define R_PMA_HW_DEP_HWA_BIT_V4M_CFB            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 8ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_CBF */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SBF            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 9ULL)    /* Bit assignment of R_PMA_HWA_ID_V4M_SBF */
#define R_PMA_HW_DEP_HWA_BIT_V4M_OSM            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 10ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_OSM */
#define R_PMA_HW_DEP_HWA_BIT_V4M_ISM0           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 11ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_ISM0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_ISM1           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 12ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_ISM1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBS            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 13ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBS */
#define R_PMA_HW_DEP_HWA_BIT_V4M_APB            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 14ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_APB */
#define R_PMA_HW_DEP_HWA_BIT_V4M_DUL            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 15ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_DUL */
#define R_PMA_HW_DEP_HWA_BIT_V4M_ISP0           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 16ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_ISP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_ISPCS0         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 17ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_ISPCS0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_ISPCS1         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 18ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_ISPCS1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_VSPX0          R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 19ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_VSPX0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FCPVX0         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 20ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FCPVX0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_UMFL0      R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 21ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_UMFL0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_SMPS0      R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 22ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_SMPS0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_SMPO0      R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 23ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_SMPO0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_PAP0       R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 24ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_PAP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_ISP0       R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 25ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_ISP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMR0       R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 26ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMR0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMR1       R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 27ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMR1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMS0       R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 28ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMS0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMS1       R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 29ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_FBA_IMS1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMR0           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 30ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_IMR0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMR1           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 31ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_IMR1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMS0           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 32ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_IMS0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_IMS1           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 33ULL)   /* Bit assignment of R_PMA_HWA_ID_V4M_IMS1 */

/* Bit assignment of A2 parent */
#define R_PMA_HW_DEP_HWA_BIT_V4M_A2_PARENTS  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_SPMC     | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMC | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4M_SPMC01   | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNRAM0)

/* Bit assignment of SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMI  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_SPMI   | \
                                                             R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMI)

/* Bit assignment of SPMC */
#define R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMC  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_SPMC     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMC | \
                                                             R_PMA_HW_DEP_HWA_BIT_V4M_SPMC01)
/* Share SYSC domain of A1CN0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1CN0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_IMPCNN | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNN0)

/* Share SYSC domain of A1DSP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1DSP0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_VDSP0_CSB | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_VDSP0_BUS | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DSP0)

/* Share SYSC domain of A1DSP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1DSP1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_VDSP1_CSB | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_VDSP1_BUS | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DSP1)

/* Share SYSC domain of A2CN0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CN0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_SPMC     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMC | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_SPMC01   | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNRAM0)

/* Share SYSC domain of A2IMP01 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2IMP01  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_IMP1     | \
                                                           R_PMA_HW_DEP_HWA_BIT_V4M_IMP0     | \
                                                           R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP0 | \
                                                           R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP1)

/* Share SYSC domain of A2PSC */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2PSC  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_IMPPSC0  | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_IMPDMAC0 | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DP0)
/* Share SYSC domain of A2PSC */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2DMA  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_IMPDMAC1  | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DP1)

/* Share SYSC domain of A3DUL */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_PAPSDMA | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_HSM     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_PAPBUS  | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_DCM     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_CFB     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_SBF     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_OSM     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_ISM0    | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_ISM1    | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_FBS     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_APB     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_DUL     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4M_FBA_PAP0)

/* Share SYSC domain of A3ISP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3ISP0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_ISP0   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_ISPCS0 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_ISPCS1 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_VSPX0  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FCPVX0 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_ISP0)

/* Share SYSC domain of A2CV0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_CVE0   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV0)

/* Share SYSC domain of A2CV1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_CVE1   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV1)

/* Share SYSC domain of A2CV2 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV2  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_CVE2   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV2)

/* Share SYSC domain of A2CV3 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_CVE3   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV3)

/* Share SYSC domain of A3VIP2 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3VIP2  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_SMPS0     | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_SMPO0     | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_SMPS0 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_SMPO0)

/* Share SYSC domain of A3IMR0 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_IMR0   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMR0)

/* Share SYSC domain of A3IMR1 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_IMR1   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMR1)
/* Share SYSC domain of A3IMR2 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR2  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_IMS0   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMS0)
/* Share SYSC domain of A3IMR3 */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_IMS1   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMS1)

/* Share CPG domain of DOF */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_DOF  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_DOF       | \
                                                       R_PMA_HW_DEP_HWA_BIT_V4M_DOF_FCPRM |\
                                                       R_PMA_HW_DEP_HWA_BIT_V4M_FBA_UMFL0)

/* Share CPG domain of SPMC */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_SPMC  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_SPMC     | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMC |\
                                                        R_PMA_HW_DEP_HWA_BIT_V4M_SPMC01)

/* Share CPG domain of PAPTOP */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_HSM  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_DCM  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_CFB  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_SBF  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_OSM  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_ISM0 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_ISM1 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_FBS  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_APB  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4M_DUL)
                                                          
/* Share CPG domain of SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_SPMI  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4M_SPMI | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMI)

/*======================================================================================================================
Private global variables
======================================================================================================================*/
/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v4m[R_PMA_HWA_ID_V4M_NUM] = {
    /* parent,                                      children                                      share sysc domain*/
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2PSC,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMPPSC                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2PSC,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMPDMAC0              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2IMP01,   R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMP1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2IMP01,   R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMP0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMI,     R_PMA_HW_DEP_HWA_BIT_V4M_IMPCNN,         R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_SPMC  },     /* R_PMA_HWA_ID_V4M_SPMC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMC,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1CN0,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMPCNN                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ISP0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANRT1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_DOF   },     /* R_PMA_HWA_ID_V4M_DOF                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3VIP2,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SMPS0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3VIP2,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SMPO0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_RGX                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANVI1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANVI0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANVC1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANVC0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANSP0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANRT                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANPV0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ADVFSC                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ADG                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_IMPSLV,              R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMC,         R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_SPMI  },     /* R_PMA_HWA_ID_V4M_SPMI                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMI,         R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMPSLV                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMI,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE,         R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMPDTA                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXIMP0_AXRS           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXIMP0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANIMP1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANIMP0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2DMA,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMPDMAC1              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSM_AXRS             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSD0_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSD0_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRS_AXSC             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRT_AXRS             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRT                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRD_1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRD_0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRC_1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRC_0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRS_AXPV             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXPV                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANMM_AXMM             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AVB2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AVB1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AVB0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ARMREG                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ARMM                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APSP3_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APSP3_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APSP4_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APSP4_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APRT0_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APRT0_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANVIP2                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANVIP1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANVIP0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CSITOP0               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CSDBGPAP              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CR0                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CANFD                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVIP1_1              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVIP1_0              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVIP0_AXRS           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVIP0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVIFF                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVI1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVI1_AXRS            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVI                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVC_AXRS             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXTBH                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSTM                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSTM_AXRS            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRS_AXSP0            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSP0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSN                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXRS_AXSN             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXSM                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_DSITXLINK0            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_DOC2CH                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_DIS0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_DDR3_DDR2             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_DDR1_DDR0             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CSITOP1               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_INTAP                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR3,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMS1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR2,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMS0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR1,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMR1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IMR0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_I2C5                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_I2C4                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_I2C3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_I2C2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_I2C1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_I2C0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_HSCIF3                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_HSCIF2                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_HSCIF1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_HSCIF0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FRAY00                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FCPVD1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FCPVD0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_RTDM1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_RTDM0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_RPC                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PWM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PCIE10                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PCIE00                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_MSI5                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_MSI4                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_MSI3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_MSI2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_MSI1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_MSI0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IVCP1E                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ISPCS0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ISPCS1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IRQC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_INTTP                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_IPC                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN01                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN00                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC7              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC6              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC5              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC4              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC3              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC2              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC1              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC0              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AES_ACC_WRAP          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_TPU0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_TMU4                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_TMU3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_TMU2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_TMU1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_TMU0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SYDM2                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SYDM1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SECROM                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SDHI0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SCIF4                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SCIF3                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SCIF1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SCIF0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VSPD0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN15                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN14                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN13                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN12                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN11                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN10                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN09                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN08                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN07                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN06                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN05                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN04                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN03                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VIN02                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_UCMT                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_TSC4_TSC3_TSC2_TSC1   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CMT3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CMT2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CMT1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CMT0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WDT1_WDT0             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WCRC3                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WCRC2                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WCRC1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WCRC0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VSPX0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_RGXC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_RGXB                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SWDT1_SWDT0           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_RGXPVC                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_RGXPVDE               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PCIE11                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PCIE01                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_INTAP11               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FRAY01                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXVI11                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AULK4P                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AULK2P                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FCPVX0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_KCRC4                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FSO                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CRC3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CRC2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CRC1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CRC0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ADVFS                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SCMT                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC3D3                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC3D2                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC3D1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC2D3                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC2D2                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC2D1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC1D3                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC1D2                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC1D1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC0D3                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC0D2                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PFC0D1                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT9_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT8_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT7_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT6_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT5_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT4_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT3_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT2_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT1_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT0_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT9_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT8_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT7_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT6_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT5_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT4_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT3_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT2_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT1_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_WWDT0_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_DBE                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_MTI                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_KCRC7                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_KCRC6                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_KCRC5                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMMM                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMIR                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMVIP                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMVC                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMVIO                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMPE0                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMRT                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKM3DG                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APVI4_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APVI4_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APVC1_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_APVC1_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMDSP_CKMIR          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMCNR_CKMIR          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMHSC                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CKMCR52               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_ANHC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXHC_AXRS             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_AXHC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CSBRG_IR_A2           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CSBRG_IR_A3           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_TSN                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV3,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CVE3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV2,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CVE2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV1,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CVE1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV0,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_CVE0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_REG             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1DSP1,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_CSB             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_SYSPO           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_APBDBG          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_OCEM            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_CORE            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_SYS             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_GLOBAL          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_REG             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1DSP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_CSB             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FCPRC                 */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PAPSDMA               */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_HSM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_PAPBUS                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1DSP1,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_BUS             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1DSP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_BUS             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SSI                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_SSIU                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_SYSPO           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_APBDBG          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_OCEM            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_CORE            */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_SYS             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_GLOBAL          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_IMPSLV,              R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMC,         R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_SPMI  },     /* R_PMA_HWA_ID_V4M_REG_SPMI              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMI,     R_PMA_HW_DEP_HWA_BIT_V4M_IMPCNN,         R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_SPMC  },     /* R_PMA_HWA_ID_V4M_REG_SPMC              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_DOF   },     /* R_PMA_HWA_ID_V4M_DOF_FCPRM             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_DCM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_CBF                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_SBF                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_OSM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_ISM0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_ISM1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_FBS                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_APB                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4M_DUL                   */
    { R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMI,     R_PMA_HW_DEP_HWA_BIT_V4M_IMPCNN,         R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_SPMC  },     /* R_PMA_HWA_ID_V4M_SPMC01                */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2IMP01,   R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP0              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2IMP01,   R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP1              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV0,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV0          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV1,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV1          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV2,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV2          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CV3,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV3          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2PSC,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_DP0           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2DMA,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_DP1           */
    { R_PMA_HW_DEP_HWA_BIT_V4M_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1CN0,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_CNN0          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1DSP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP0          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A1DSP1,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP1          */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0        */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_DOF,       R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_UMFL0             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3VIP2,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_SMPS0             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3VIP2,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_SMPO0             */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_PAP0              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_ISP0              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR0,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMR0              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR1,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMR1              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR2,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMS0              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_SHARE_A3IMR3,    R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_FBA_IMS1              */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET       */
    { R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,                R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            R_PMA_HW_DEP_HWA_BIT_V4M_NONE        },     /* R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET       */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const uint64_t g_pma_hw_dep_hwa_bit_tbl_v4m[R_PMA_HWA_ID_V4M_NUM] = {
    R_PMA_HW_DEP_HWA_BIT_V4M_IMPPSC0,         /* R_PMA_HWA_ID_V4M_IMPPSC                */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMPDMAC0,        /* R_PMA_HWA_ID_V4M_IMPDMAC0              */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMP1,            /* R_PMA_HWA_ID_V4M_IMP1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMP0,            /* R_PMA_HWA_ID_V4M_IMP0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_SPMC,            /* R_PMA_HWA_ID_V4M_SPMC                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMPCNN,          /* R_PMA_HWA_ID_V4M_IMPCNN                */
    R_PMA_HW_DEP_HWA_BIT_V4M_ISP0,            /* R_PMA_HWA_ID_V4M_ISP0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANRT1                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_DOF,             /* R_PMA_HWA_ID_V4M_DOF                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_SMPS0,           /* R_PMA_HWA_ID_V4M_SMPS0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_SMPO0,           /* R_PMA_HWA_ID_V4M_SMPO0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_RGX                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANVI1                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANVI0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANVC1                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANVC0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANSP0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANRT                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANPV0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ADVFSC                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ADG                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_SPMI,            /* R_PMA_HWA_ID_V4M_SPMI                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMPSLV,          /* R_PMA_HWA_ID_V4M_IMPSLV                */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMPDTA,          /* R_PMA_HWA_ID_V4M_IMPDTA                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXIMP0_AXRS           */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXIMP0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANIMP1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANIMP0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMPDMAC1,        /* R_PMA_HWA_ID_V4M_IMPDMAC1              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSM_AXRS             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSD0_1               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSD0_0               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSC                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRS_AXSC             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRT_AXRS             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRT                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRD_1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRD_0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRC_1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRC_0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRS_AXPV             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXPV                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANMM_AXMM             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AVB2                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AVB1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AVB0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ARMREG                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ARMM                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APSP3_1               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APSP3_0               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APSP4_1               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APSP4_0               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APRT0_1               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APRT0_0               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANVIP2                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANVIP1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANVIP0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CSITOP0               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CSDBGPAP              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CR0                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CANFD                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVIP1_1              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVIP1_0              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVIP0_AXRS           */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVIP0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVIFF                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVI1                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVI1_AXRS            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVI                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVC_AXRS             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVC                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXTBH                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSTM                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSTM_AXRS            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRS_AXSP0            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSP0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSN                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXRS_AXSN             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXSM                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_DSITXLINK0            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_DOC2CH                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_DIS0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_DDR3_DDR2             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_DDR1_DDR0             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CSITOP1               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_INTAP                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMS1,            /* R_PMA_HWA_ID_V4M_IMS1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMS0,            /* R_PMA_HWA_ID_V4M_IMS0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMR1,            /* R_PMA_HWA_ID_V4M_IMR1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_IMR0,            /* R_PMA_HWA_ID_V4M_IMR0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_I2C5                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_I2C4                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_I2C3                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_I2C2                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_I2C1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_I2C0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_HSCIF3                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_HSCIF2                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_HSCIF1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_HSCIF0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_FRAY00                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_FCPVD1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_FCPVD0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_RTDM1                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_RTDM0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_RPC                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PWM                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PCIE10                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PCIE00                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_MSI5                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_MSI4                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_MSI3                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_MSI2                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_MSI1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_MSI0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_IVCP1E                */
    R_PMA_HW_DEP_HWA_BIT_V4M_ISPCS0,          /* R_PMA_HWA_ID_V4M_ISPCS0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_ISPCS1,          /* R_PMA_HWA_ID_V4M_ISPCS1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_IRQC                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_INTTP                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_IPC                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN01                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN00                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC7              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC6              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC5              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC4              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC3              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC2              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC1              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC0              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AES_ACC_WRAP          */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_TPU0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_TMU4                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_TMU3                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_TMU2                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_TMU1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_TMU0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SYDM2                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SYDM1                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SECROM                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SDHI0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SCIF4                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SCIF3                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SCIF1                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SCIF0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VSPD0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN15                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN14                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN13                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN12                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN11                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN10                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN09                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN08                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN07                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN06                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN05                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN04                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN03                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VIN02                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_UCMT                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_TSC4_TSC3_TSC2_TSC1   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC3                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC2                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CMT3                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CMT2                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CMT1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CMT0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WDT1_WDT0             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WCRC3                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WCRC2                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WCRC1                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WCRC0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_VSPX0,           /* R_PMA_HWA_ID_V4M_VSPX0                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_RGXC                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_RGXB                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SWDT1_SWDT0           */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_RGXPVC                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_RGXPVDE               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PCIE11                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PCIE01                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_INTAP11               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_FRAY01                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXVI11                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AULK4P                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AULK2P                */
    R_PMA_HW_DEP_HWA_BIT_V4M_FCPVX0,          /* R_PMA_HWA_ID_V4M_FCPVX0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_KCRC4                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_FSO                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CRC3                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CRC2                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CRC1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CRC0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ADVFS                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SCMT                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC3D3                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC3D2                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC3D1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC2D3                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC2D2                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC2D1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC1D3                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC1D2                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC1D1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC0D3                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC0D2                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_PFC0D1                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT9_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT8_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT7_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT6_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT5_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT4_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT3_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT2_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT1_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT0_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT9_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT8_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT7_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT6_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT5_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT4_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT3_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT2_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT1_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_WWDT0_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_DBE                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_MTI                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_KCRC7                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_KCRC6                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_KCRC5                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMMM                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMIR                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMVIP                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMVC                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMVIO                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMPE0                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMRT                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKM3DG                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APVI4_1               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APVI4_0               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APVC1_1               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_APVC1_0               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMDSP_CKMIR          */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMCNR_CKMIR          */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKM                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMHSC                */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CKMCR52               */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_ANHC                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXHC_AXRS             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_AXHC                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CSBRG_IR_A2           */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_CSBRG_IR_A3           */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_TSN                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_CVE3,            /* R_PMA_HWA_ID_V4M_CVE3                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_CVE2,            /* R_PMA_HWA_ID_V4M_CVE2                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_CVE1,            /* R_PMA_HWA_ID_V4M_CVE1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_CVE0,            /* R_PMA_HWA_ID_V4M_CVE0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP1_REG             */
    R_PMA_HW_DEP_HWA_BIT_V4M_VDSP1_CSB,       /* R_PMA_HWA_ID_V4M_VDSP1_CSB             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP0_SYSPO           */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP0_APBDBG          */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP0_OCEM            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP0_CORE            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP0_SYS             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP0_GLOBAL          */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP0_REG             */
    R_PMA_HW_DEP_HWA_BIT_V4M_VDSP0_CSB,       /* R_PMA_HWA_ID_V4M_VDSP0_CSB             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_FCPRC                 */
    R_PMA_HW_DEP_HWA_BIT_V4M_PAPSDMA,         /* R_PMA_HWA_ID_V4M_PAPSDMA               */
    R_PMA_HW_DEP_HWA_BIT_V4M_HSM,             /* R_PMA_HWA_ID_V4M_HSM                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_PAPBUS,          /* R_PMA_HWA_ID_V4M_PAPBUS                */
    R_PMA_HW_DEP_HWA_BIT_V4M_VDSP1_BUS,       /* R_PMA_HWA_ID_V4M_VDSP1_BUS             */
    R_PMA_HW_DEP_HWA_BIT_V4M_VDSP0_BUS,       /* R_PMA_HWA_ID_V4M_VDSP0_BUS             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SSI                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_SSIU                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP1_SYSPO           */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP1_APBDBG          */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP1_OCEM            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP1_CORE            */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP1_SYS             */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP1_GLOBAL          */
    R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMI,        /* R_PMA_HWA_ID_V4M_REG_SPMI              */
    R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMC,        /* R_PMA_HWA_ID_V4M_REG_SPMC              */
    R_PMA_HW_DEP_HWA_BIT_V4M_DOF_FCPRM,       /* R_PMA_HWA_ID_V4M_DOF_FCPRM             */
    R_PMA_HW_DEP_HWA_BIT_V4M_DCM,             /* R_PMA_HWA_ID_V4M_DCM                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_CFB,             /* R_PMA_HWA_ID_V4M_CBF                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_SBF,             /* R_PMA_HWA_ID_V4M_SBF                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_OSM,             /* R_PMA_HWA_ID_V4M_OSM                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_ISM0,            /* R_PMA_HWA_ID_V4M_ISM0                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_ISM1,            /* R_PMA_HWA_ID_V4M_ISM1                  */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBS,             /* R_PMA_HWA_ID_V4M_FBS                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_APB,             /* R_PMA_HWA_ID_V4M_APB                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_DUL,             /* R_PMA_HWA_ID_V4M_DUL                   */
    R_PMA_HW_DEP_HWA_BIT_V4M_SPMC01,          /* R_PMA_HWA_ID_V4M_SPMC01                */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP0,        /* R_PMA_HWA_ID_V4M_FBA_IMP0              */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP1,        /* R_PMA_HWA_ID_V4M_FBA_IMP1              */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV0          */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV1,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV1          */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV2,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV2          */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV3,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV3          */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DP0,     /* R_PMA_HWA_ID_V4M_FBA_IMP_DP0           */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DP1,     /* R_PMA_HWA_ID_V4M_FBA_IMP_DP1           */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNN0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_CNN0          */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DSP0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP0          */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DSP1,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP1          */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNRAM0,  /* R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0        */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_UMFL0,       /* R_PMA_HWA_ID_V4M_FBA_UMFL0             */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_SMPS0,       /* R_PMA_HWA_ID_V4M_FBA_SMPS0             */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_SMPO0,       /* R_PMA_HWA_ID_V4M_FBA_SMPO0             */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_PAP0,        /* R_PMA_HWA_ID_V4M_FBA_PAP0              */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_ISP0,        /* R_PMA_HWA_ID_V4M_FBA_ISP0              */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMR0,        /* R_PMA_HWA_ID_V4M_FBA_IMR0              */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMR1,        /* R_PMA_HWA_ID_V4M_FBA_IMR1              */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMS0,        /* R_PMA_HWA_ID_V4M_FBA_IMS0              */
    R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMS1,        /* R_PMA_HWA_ID_V4M_FBA_IMS1              */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET       */
    R_PMA_HW_DEP_HWA_BIT_V4M_NONE,            /* R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET       */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v4m[R_PMA_HW_DEP_HWA_DEPEND_V4M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM] = {
    R_PMA_HWA_ID_V4M_IMPCNN,          /* R_PMA_HW_DEP_HWA_BIT_V4M_IMPCNN           */
    R_PMA_HWA_ID_V4M_SPMC,            /* R_PMA_HW_DEP_HWA_BIT_V4M_SPMC             */
    R_PMA_HWA_ID_V4M_IMP0,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMP0             */
    R_PMA_HWA_ID_V4M_IMP1,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMP1             */
    R_PMA_HWA_ID_V4M_IMPDMAC0,        /* R_PMA_HW_DEP_HWA_BIT_V4M_IMPDMAC0         */
    R_PMA_HWA_ID_V4M_IMPDMAC1,        /* R_PMA_HW_DEP_HWA_BIT_V4M_IMPDMAC1         */
    R_PMA_HWA_ID_V4M_IMPPSC,          /* R_PMA_HW_DEP_HWA_BIT_V4M_IMPPSC0          */
    R_PMA_HWA_ID_V4M_CVE0,            /* R_PMA_HW_DEP_HWA_BIT_V4M_CVE0             */
    R_PMA_HWA_ID_V4M_CVE1,            /* R_PMA_HW_DEP_HWA_BIT_V4M_CVE1             */
    R_PMA_HWA_ID_V4M_CVE2,            /* R_PMA_HW_DEP_HWA_BIT_V4M_CVE2             */
    R_PMA_HWA_ID_V4M_CVE3,            /* R_PMA_HW_DEP_HWA_BIT_V4M_CVE3             */
    R_PMA_HWA_ID_V4M_IMPDTA,          /* R_PMA_HW_DEP_HWA_BIT_V4M_IMPDTA           */
    R_PMA_HWA_ID_V4M_IMPSLV,          /* R_PMA_HW_DEP_HWA_BIT_V4M_IMPSLV           */
    R_PMA_HWA_ID_V4M_SPMI,            /* R_PMA_HW_DEP_HWA_BIT_V4M_SPMI             */
    R_PMA_HWA_ID_V4M_VDSP0_CSB,       /* R_PMA_HW_DEP_HWA_BIT_V4M_VDSP0_CSB        */
    R_PMA_HWA_ID_V4M_VDSP0_BUS,       /* R_PMA_HW_DEP_HWA_BIT_V4M_VDSP0_BUS        */
    R_PMA_HWA_ID_V4M_VDSP1_CSB,       /* R_PMA_HW_DEP_HWA_BIT_V4M_VDSP1_CSB        */
    R_PMA_HWA_ID_V4M_VDSP1_BUS,       /* R_PMA_HW_DEP_HWA_BIT_V4M_VDSP1_BUS        */
    R_PMA_HWA_ID_V4M_REG_SPMI,        /* R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMI         */
    R_PMA_HWA_ID_V4M_REG_SPMC,        /* R_PMA_HW_DEP_HWA_BIT_V4M_REG_SPMC         */
    R_PMA_HWA_ID_V4M_SPMC01,          /* R_PMA_HW_DEP_HWA_BIT_V4M_SPMC01           */
    R_PMA_HWA_ID_V4M_FBA_IMP0,        /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP0         */
    R_PMA_HWA_ID_V4M_FBA_IMP1,        /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP1         */
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV0,    /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV0     */
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV1,    /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV1     */
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV2,    /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV2     */
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV3,    /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_OCV3     */
    R_PMA_HWA_ID_V4M_FBA_IMP_DP0,     /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DP0      */
    R_PMA_HWA_ID_V4M_FBA_IMP_DP1,     /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DP1      */
    R_PMA_HWA_ID_V4M_FBA_IMP_CNN0,    /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNN0     */
    R_PMA_HWA_ID_V4M_FBA_IMP_DSP0,    /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DSP0     */
    R_PMA_HWA_ID_V4M_FBA_IMP_DSP1,    /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_DSP1     */
    R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0,  /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMP_CNRAM0   */
    R_PMA_HWA_ID_V4M_CVE3,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMP_OCV0         */
    R_PMA_HWA_ID_V4M_CVE2,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMP_OCV1         */
    R_PMA_HWA_ID_V4M_CVE1,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMP_OCV2         */
    R_PMA_HWA_ID_V4M_CVE0,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMP_OCV3         */
    R_PMA_HWA_ID_V4M_SMPS0,           /* R_PMA_HW_DEP_HWA_BIT_V4M_SMPS0            */
    R_PMA_HWA_ID_V4M_SMPO0,           /* R_PMA_HW_DEP_HWA_BIT_V4M_SMPO0            */
    R_PMA_HWA_ID_V4M_DOF,             /* R_PMA_HW_DEP_HWA_BIT_V4M_DOF              */
    R_PMA_HWA_ID_V4M_DOF_FCPRM,       /* R_PMA_HW_DEP_HWA_BIT_V4M_DOF_FCPRM        */
    R_PMA_HWA_ID_V4M_PAPSDMA,         /* R_PMA_HW_DEP_HWA_BIT_V4M_PAPSDMA          */
    R_PMA_HWA_ID_V4M_HSM,             /* R_PMA_HW_DEP_HWA_BIT_V4M_HSM              */
    R_PMA_HWA_ID_V4M_PAPBUS,          /* R_PMA_HW_DEP_HWA_BIT_V4M_PAPBUS           */
    R_PMA_HWA_ID_V4M_DCM,             /* R_PMA_HW_DEP_HWA_BIT_V4M_DCM              */
    R_PMA_HWA_ID_V4M_CBF,             /* R_PMA_HW_DEP_HWA_BIT_V4M_CFB              */
    R_PMA_HWA_ID_V4M_SBF,             /* R_PMA_HW_DEP_HWA_BIT_V4M_SBF              */
    R_PMA_HWA_ID_V4M_OSM,             /* R_PMA_HW_DEP_HWA_BIT_V4M_OSM              */
    R_PMA_HWA_ID_V4M_ISM0,            /* R_PMA_HW_DEP_HWA_BIT_V4M_ISM0             */
    R_PMA_HWA_ID_V4M_ISM1,            /* R_PMA_HW_DEP_HWA_BIT_V4M_ISM1             */
    R_PMA_HWA_ID_V4M_FBS,             /* R_PMA_HW_DEP_HWA_BIT_V4M_FBS              */
    R_PMA_HWA_ID_V4M_APB,             /* R_PMA_HW_DEP_HWA_BIT_V4M_APB              */
    R_PMA_HWA_ID_V4M_DUL,             /* R_PMA_HW_DEP_HWA_BIT_V4M_DUL              */
    R_PMA_HWA_ID_V4M_ISP0,            /* R_PMA_HW_DEP_HWA_BIT_V4M_ISP0             */
    R_PMA_HWA_ID_V4M_ISPCS0,          /* R_PMA_HW_DEP_HWA_BIT_V4M_ISPCS0           */
    R_PMA_HWA_ID_V4M_ISPCS1,          /* R_PMA_HW_DEP_HWA_BIT_V4M_ISPCS1           */
    R_PMA_HWA_ID_V4M_VSPX0,           /* R_PMA_HW_DEP_HWA_BIT_V4M_VSPX0            */
    R_PMA_HWA_ID_V4M_FCPVX0,          /* R_PMA_HW_DEP_HWA_BIT_V4M_FCPVX0           */
    R_PMA_HWA_ID_V4M_FBA_UMFL0,       /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_UMFL0        */
    R_PMA_HWA_ID_V4M_FBA_SMPS0,       /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_SMPS0        */
    R_PMA_HWA_ID_V4M_FBA_SMPO0,       /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_SMPO0        */
    R_PMA_HWA_ID_V4M_FBA_PAP0,        /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_PAP0         */
    R_PMA_HWA_ID_V4M_FBA_ISP0,        /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_ISP0         */
    R_PMA_HWA_ID_V4M_FBA_IMR0,        /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMR0         */
    R_PMA_HWA_ID_V4M_FBA_IMR1,        /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMR1         */
    R_PMA_HWA_ID_V4M_FBA_IMS0,        /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMS0         */
    R_PMA_HWA_ID_V4M_FBA_IMS1,        /* R_PMA_HW_DEP_HWA_BIT_V4M_FBA_IMS1         */
    R_PMA_HWA_ID_V4M_IMR0,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMR0             */
    R_PMA_HWA_ID_V4M_IMR1,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMR1             */
    R_PMA_HWA_ID_V4M_IMS0,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMS0             */
    R_PMA_HWA_ID_V4M_IMS1,            /* R_PMA_HW_DEP_HWA_BIT_V4M_IMS1             */
    R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM
};

/* PRQA S 1533,3408 2 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
uint64_t g_pma_hw_dep_sysc_ref_tbl_v4m[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V4M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM] = { 0 };
uint64_t g_pma_hw_dep_cpg_ref_tbl_v4m[(uint64_t)R_PMA_CPG_DOMAIN_ID_V4M_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM]   = { 0 };

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
st_hwa_t g_pma_hw_dep_hwa_info_tbl_v4m[R_PMA_HWA_ID_V4M_NUM] = {
    /* hwa id, sysc_id, cpg_id, reset_id, actual_rst_stat, ToPG, PGtoCG, HPtoCG, ToHP */
    {
        R_PMA_HWA_ID_V4M_IMPPSC,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2PSC,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPPSC,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPPSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMPDMAC0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2PSC,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPDMAC0,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPDMAC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMP1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2IMP01,
        R_PMA_CPG_DOMAIN_ID_V4M_IMP1,
        R_PMA_CPG_DOMAIN_ID_V4M_IMP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMP0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2IMP01,
        R_PMA_CPG_DOMAIN_ID_V4M_IMP0,
        R_PMA_CPG_DOMAIN_ID_V4M_IMP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SPMC,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CN0,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMC,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMPCNN,
        R_PMA_SYSC_DOMAIN_ID_V4M_A1CN0,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPCNN,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPCNN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ISP0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4M_ISP0,
        R_PMA_CPG_DOMAIN_ID_V4M_ISP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANRT1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANRT1,
        R_PMA_CPG_DOMAIN_ID_V4M_ANRT1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DOF,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP0,
        R_PMA_CPG_DOMAIN_ID_V4M_UMFL0,
        R_PMA_CPG_DOMAIN_ID_V4M_UMFL0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SMPS0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V4M_SMPS0,
        R_PMA_CPG_DOMAIN_ID_V4M_SMPS0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SMPO0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V4M_SMPO0,
        R_PMA_CPG_DOMAIN_ID_V4M_SMPO0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_RGX,
        R_PMA_SYSC_DOMAIN_ID_V4M_A23DGB,
        R_PMA_CPG_DOMAIN_ID_V4M_RGX,
        R_PMA_CPG_DOMAIN_ID_V4M_RGX,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H,
        &R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANVI1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVI1,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVI1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANVI0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVI0,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVI0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANVC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVC1,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANVC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVC0,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANSP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANSP0,
        R_PMA_CPG_DOMAIN_ID_V4M_ANSP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANRT,
        R_PMA_CPG_DOMAIN_ID_V4M_ANRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANPV0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANPV0,
        R_PMA_CPG_DOMAIN_ID_V4M_ANPV0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ADVFSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ADVFSC,
        R_PMA_CPG_DOMAIN_ID_V4M_ADVFSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ADG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ADG,
        R_PMA_CPG_DOMAIN_ID_V4M_ADG,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SPMI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMI,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMI,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMPSLV,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPSLV,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPSLV,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMPDTA,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPDTA,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPDTA,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXIMP0_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXIMP0_AXRS,
        R_PMA_CPG_DOMAIN_ID_V4M_AXIMP0_AXRS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXIMP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXIMP0,
        R_PMA_CPG_DOMAIN_ID_V4M_AXIMP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANIMP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANIMP1,
        R_PMA_CPG_DOMAIN_ID_V4M_ANIMP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANIMP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANIMP0,
        R_PMA_CPG_DOMAIN_ID_V4M_ANIMP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMPDMAC1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2DMA,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPDMAC1,
        R_PMA_CPG_DOMAIN_ID_V4M_IMPDMAC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSM_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSM_AXRS,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSM_AXRS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSD0_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSD0_1,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSD0_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSD0_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSD0_0,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSD0_0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSC,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRS_AXSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSC,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRT_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRT_AXRS,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRT_AXRS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRT,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRD_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRD_1,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRD_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRD_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRD_0,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRD_0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRC_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRC_1,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRC_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRC_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRC_0,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRC_0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRS_AXPV,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXPV,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXPV,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXPV,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXPV,
        R_PMA_CPG_DOMAIN_ID_V4M_AXPV,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANMM_AXMM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANMM_AXMM,
        R_PMA_CPG_DOMAIN_ID_V4M_ANMM_AXMM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AVB2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AVB2,
        R_PMA_CPG_DOMAIN_ID_V4M_AVB2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AVB1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AVB1,
        R_PMA_CPG_DOMAIN_ID_V4M_AVB1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AVB0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AVB0,
        R_PMA_CPG_DOMAIN_ID_V4M_AVB0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ARMREG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ARMREG,
        R_PMA_CPG_DOMAIN_ID_V4M_ARMREG,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ARMM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ARMM,
        R_PMA_CPG_DOMAIN_ID_V4M_ARMM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APSP3_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APSP3_1,
        R_PMA_CPG_DOMAIN_ID_V4M_APSP3_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APSP3_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APSP3_0,
        R_PMA_CPG_DOMAIN_ID_V4M_APSP3_0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APSP4_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APSP4_1,
        R_PMA_CPG_DOMAIN_ID_V4M_APSP4_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APSP4_0 ,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APSP4_0 ,
        R_PMA_CPG_DOMAIN_ID_V4M_APSP4_0 ,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APRT0_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APRT0_1,
        R_PMA_CPG_DOMAIN_ID_V4M_APRT0_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APRT0_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APRT0_0,
        R_PMA_CPG_DOMAIN_ID_V4M_APRT0_0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANVIP2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVIP2,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVIP2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANVIP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVIP1,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVIP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANVIP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVIP0,
        R_PMA_CPG_DOMAIN_ID_V4M_ANVIP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CSITOP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CSITOP0,
        R_PMA_CPG_DOMAIN_ID_V4M_CSITOP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CSDBGPAP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CSDBGPAP,
        R_PMA_CPG_DOMAIN_ID_V4M_CSDBGPAP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CR0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CR0,
        R_PMA_CPG_DOMAIN_ID_V4M_CR0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CANFD,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CANFD,
        R_PMA_CPG_DOMAIN_ID_V4M_CANFD,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVIP1_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIP1_1,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIP1_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVIP1_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIP1_0,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIP1_0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVIP0_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIP0_AXRS,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIP0_AXRS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVIP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIP0,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVIFF,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIFF,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVIFF,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVI1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVI1,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVI1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVI1_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVI1_AXRS,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVI1_AXRS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVI,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVI,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVC_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVC_AXRS,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVC_AXRS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVC,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXTBH,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXTBH,
        R_PMA_CPG_DOMAIN_ID_V4M_AXTBH,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSTM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSTM,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSTM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSTM_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSTM_AXRS,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSTM_AXRS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRS_AXSP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSP0,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSP0,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSN,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSN,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXRS_AXSN,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSN,
        R_PMA_CPG_DOMAIN_ID_V4M_AXRS_AXSN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXSM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSM,
        R_PMA_CPG_DOMAIN_ID_V4M_AXSM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DSITXLINK0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_DSITXLINK0,
        R_PMA_CPG_DOMAIN_ID_V4M_DSITXLINK0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DOC2CH,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_DOC2CH,
        R_PMA_CPG_DOMAIN_ID_V4M_DOC2CH,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DIS0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_DIS0,
        R_PMA_CPG_DOMAIN_ID_V4M_DIS0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DDR3_DDR2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_DDR3_DDR2,
        R_PMA_CPG_DOMAIN_ID_V4M_DDR3_DDR2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DDR1_DDR0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_DDR1_DDR0,
        R_PMA_CPG_DOMAIN_ID_V4M_DDR1_DDR0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CSITOP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CSITOP1,
        R_PMA_CPG_DOMAIN_ID_V4M_CSITOP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_INTAP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_INTAP,
        R_PMA_CPG_DOMAIN_ID_V4M_INTAP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMS1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR3,
        R_PMA_CPG_DOMAIN_ID_V4M_IMS1,
        R_PMA_CPG_DOMAIN_ID_V4M_IMS1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangeToHPforA3IMR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMS0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR2,
        R_PMA_CPG_DOMAIN_ID_V4M_IMS0,
        R_PMA_CPG_DOMAIN_ID_V4M_IMS0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangeToHPforA3IMR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMR1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR1,
        R_PMA_CPG_DOMAIN_ID_V4M_IMR1,
        R_PMA_CPG_DOMAIN_ID_V4M_IMR1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangeToHPforA3IMR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IMR0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR0,
        R_PMA_CPG_DOMAIN_ID_V4M_IMR0,
        R_PMA_CPG_DOMAIN_ID_V4M_IMR0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforA3IMR,
        &R_PMA_HW_DEP_NoticeChangeToHPforA3IMR,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_I2C5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C5,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_I2C4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C4,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_I2C3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C3,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_I2C2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C2,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_I2C1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C1,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_I2C0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C0,
        R_PMA_CPG_DOMAIN_ID_V4M_I2C0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_HSCIF3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_HSCIF3,
        R_PMA_CPG_DOMAIN_ID_V4M_HSCIF3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_HSCIF2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_HSCIF2,
        R_PMA_CPG_DOMAIN_ID_V4M_HSCIF2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_HSCIF1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_HSCIF1,
        R_PMA_CPG_DOMAIN_ID_V4M_HSCIF1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_HSCIF0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_HSCIF0,
        R_PMA_CPG_DOMAIN_ID_V4M_HSCIF0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_FRAY00,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_FRAY00,
        R_PMA_CPG_DOMAIN_ID_V4M_FRAY00,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_FCPVD1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_FCPVD1,
        R_PMA_CPG_DOMAIN_ID_V4M_FCPVD1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_FCPVD0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_FCPVD0,
        R_PMA_CPG_DOMAIN_ID_V4M_FCPVD0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_RTDM1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_RTDM1,
        R_PMA_CPG_DOMAIN_ID_V4M_RTDM1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_RTDM0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_RTDM0,
        R_PMA_CPG_DOMAIN_ID_V4M_RTDM0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_RPC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_RPC,
        R_PMA_CPG_DOMAIN_ID_V4M_RPC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PWM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PWM,
        R_PMA_CPG_DOMAIN_ID_V4M_PWM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PCIE10,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PCIE10,
        R_PMA_CPG_DOMAIN_ID_V4M_PCIE10,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PCIE00,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PCIE00,
        R_PMA_CPG_DOMAIN_ID_V4M_PCIE00,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_MSI5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI5,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_MSI4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI4,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_MSI3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI3,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_MSI2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI2,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_MSI1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI1,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_MSI0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI0,
        R_PMA_CPG_DOMAIN_ID_V4M_MSI0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IVCP1E,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3VC,
        R_PMA_CPG_DOMAIN_ID_V4M_IVCP1E,
        R_PMA_CPG_DOMAIN_ID_V4M_IVCP1E,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ISPCS0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4M_ISPCS0,
        R_PMA_CPG_DOMAIN_ID_V4M_ISPCS0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ISPCS1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4M_ISPCS1,
        R_PMA_CPG_DOMAIN_ID_V4M_ISPCS1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IRQC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_IRQC,
        R_PMA_CPG_DOMAIN_ID_V4M_IRQC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_INTTP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_INTTP,
        R_PMA_CPG_DOMAIN_ID_V4M_INTTP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_IPC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_IPC,
        R_PMA_CPG_DOMAIN_ID_V4M_IPC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN01,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN01,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN01,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN00,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN00,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN00,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC7,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC7,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC7,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC6,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC6,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC6,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC5,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC4,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC3,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC2,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC1,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC0,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AES_ACC_WRAP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC_WRAP,
        R_PMA_CPG_DOMAIN_ID_V4M_AES_ACC_WRAP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_TPU0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_TPU0,
        R_PMA_CPG_DOMAIN_ID_V4M_TPU0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_TMU4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU4,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_TMU3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU3,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_TMU2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU2,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_TMU1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU1,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_TMU0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU0,
        R_PMA_CPG_DOMAIN_ID_V4M_TMU0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SYDM2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SYDM2,
        R_PMA_CPG_DOMAIN_ID_V4M_SYDM2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SYDM1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SYDM1,
        R_PMA_CPG_DOMAIN_ID_V4M_SYDM1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SECROM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SECROM,
        R_PMA_CPG_DOMAIN_ID_V4M_SECROM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SDHI0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SDHI0,
        R_PMA_CPG_DOMAIN_ID_V4M_SDHI0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SCIF4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SCIF4,
        R_PMA_CPG_DOMAIN_ID_V4M_SCIF4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SCIF3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SCIF3,
        R_PMA_CPG_DOMAIN_ID_V4M_SCIF3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SCIF1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SCIF1,
        R_PMA_CPG_DOMAIN_ID_V4M_SCIF1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SCIF0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SCIF0,
        R_PMA_CPG_DOMAIN_ID_V4M_SCIF0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VSPD0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VSPD0,
        R_PMA_CPG_DOMAIN_ID_V4M_VSPD0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN15,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN15,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN15,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN14,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN14,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN14,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN13,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN13,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN13,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN12,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN12,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN12,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN11,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN11,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN10,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN10,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN10,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN09,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN09,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN09,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN08,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN08,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN08,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN07,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN07,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN07,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN06,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN06,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN06,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN05,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN05,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN05,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN04,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN04,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN04,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN03,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN03,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN03,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VIN02,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN02,
        R_PMA_CPG_DOMAIN_ID_V4M_VIN02,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_UCMT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_UCMT,
        R_PMA_CPG_DOMAIN_ID_V4M_UCMT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_TSC4_TSC3_TSC2_TSC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_TSC4_TSC3_TSC2_TSC1,
        R_PMA_CPG_DOMAIN_ID_V4M_TSC4_TSC3_TSC2_TSC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC3,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC2,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC1,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC0,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CMT3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CMT3,
        R_PMA_CPG_DOMAIN_ID_V4M_CMT3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CMT2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CMT2,
        R_PMA_CPG_DOMAIN_ID_V4M_CMT2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CMT1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CMT1,
        R_PMA_CPG_DOMAIN_ID_V4M_CMT1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CMT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CMT0,
        R_PMA_CPG_DOMAIN_ID_V4M_CMT0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WDT1_WDT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WDT1_WDT0,
        R_PMA_CPG_DOMAIN_ID_V4M_WDT1_WDT0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WCRC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WCRC3,
        R_PMA_CPG_DOMAIN_ID_V4M_WCRC3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WCRC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WCRC2,
        R_PMA_CPG_DOMAIN_ID_V4M_WCRC2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WCRC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WCRC1,
        R_PMA_CPG_DOMAIN_ID_V4M_WCRC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WCRC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WCRC0,
        R_PMA_CPG_DOMAIN_ID_V4M_WCRC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VSPX0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4M_VSPX0,
        R_PMA_CPG_DOMAIN_ID_V4M_VSPX0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_RGXC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_RGXC,
        R_PMA_CPG_DOMAIN_ID_V4M_RGXC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_RGXB,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_RGXB,
        R_PMA_CPG_DOMAIN_ID_V4M_RGXB,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SWDT1_SWDT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SWDT1_SWDT0,
        R_PMA_CPG_DOMAIN_ID_V4M_SWDT1_SWDT0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_RGXPVC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_RGXPVC,
        R_PMA_CPG_DOMAIN_ID_V4M_RGXPVC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_RGXPVDE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_RGXPVDE,
        R_PMA_CPG_DOMAIN_ID_V4M_RGXPVDE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PCIE11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PCIE11,
        R_PMA_CPG_DOMAIN_ID_V4M_PCIE11,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PCIE01,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PCIE01,
        R_PMA_CPG_DOMAIN_ID_V4M_PCIE01,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_INTAP11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_INTAP11,
        R_PMA_CPG_DOMAIN_ID_V4M_INTAP11,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_FRAY01,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_FRAY01,
        R_PMA_CPG_DOMAIN_ID_V4M_FRAY01,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXVI11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVI11,
        R_PMA_CPG_DOMAIN_ID_V4M_AXVI11,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AULK4P,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AULK4P,
        R_PMA_CPG_DOMAIN_ID_V4M_AULK4P,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AULK2P,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AULK2P,
        R_PMA_CPG_DOMAIN_ID_V4M_AULK2P,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_FCPVX0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4M_FCPVX0,
        R_PMA_CPG_DOMAIN_ID_V4M_FCPVX0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_KCRC4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_KCRC4,
        R_PMA_CPG_DOMAIN_ID_V4M_KCRC4,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_FSO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_FSO,
        R_PMA_CPG_DOMAIN_ID_V4M_FSO,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CRC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CRC3,
        R_PMA_CPG_DOMAIN_ID_V4M_CRC3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CRC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CRC2,
        R_PMA_CPG_DOMAIN_ID_V4M_CRC2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CRC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CRC1,
        R_PMA_CPG_DOMAIN_ID_V4M_CRC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CRC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CRC0,
        R_PMA_CPG_DOMAIN_ID_V4M_CRC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ADVFS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ADVFS,
        R_PMA_CPG_DOMAIN_ID_V4M_ADVFS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SCMT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SCMT,
        R_PMA_CPG_DOMAIN_ID_V4M_SCMT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC3D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC3D3,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC3D3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC3D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC3D2,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC3D2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC3D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC3D1,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC3D1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC2D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC2D3,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC2D3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC2D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC2D2,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC2D2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC2D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC2D1,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC2D1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC1D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC1D3,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC1D3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC1D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC1D2,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC1D2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC1D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC1D1,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC1D1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC0D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC0D3,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC0D3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC0D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC0D2,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC0D2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PFC0D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC0D1,
        R_PMA_CPG_DOMAIN_ID_V4M_PFC0D1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT9_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT9_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT9_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT8_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT8_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT8_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT7_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT7_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT7_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT6_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT6_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT6_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT5_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT5_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT5_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT4_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT4_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT4_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT3_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT3_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT3_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT2_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT2_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT2_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT1_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT1_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT1_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT0_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT0_RCLK,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT0_RCLK,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT9_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT9_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT9_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT8_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT8_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT8_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT7_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT7_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT7_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT6_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT6_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT6_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT5_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT5_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT5_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT4_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT4_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT4_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT3_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT3_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT3_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT2_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT2_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT2_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT1_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT1_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT1_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_WWDT0_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT0_SASYNCRT,
        R_PMA_CPG_DOMAIN_ID_V4M_WWDT0_SASYNCRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DBE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_DBE,
        R_PMA_CPG_DOMAIN_ID_V4M_DBE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_MTI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_MTI,
        R_PMA_CPG_DOMAIN_ID_V4M_MTI,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_KCRC7,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_KCRC7,
        R_PMA_CPG_DOMAIN_ID_V4M_KCRC7,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_KCRC6,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_KCRC6,
        R_PMA_CPG_DOMAIN_ID_V4M_KCRC6,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_KCRC5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_KCRC5,
        R_PMA_CPG_DOMAIN_ID_V4M_KCRC5,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMMM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMMM,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMMM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMIR,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMIR,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMIR,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMVIP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMVIP,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMVIP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMVC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMVC,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMVC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMVIO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMVIO,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMVIO,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMPE0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMPE0,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMPE0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMRT,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMRT,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKM3DG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKM3DG,
        R_PMA_CPG_DOMAIN_ID_V4M_CKM3DG,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APVI4_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APVI4_1,
        R_PMA_CPG_DOMAIN_ID_V4M_APVI4_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APVI4_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APVI4_0,
        R_PMA_CPG_DOMAIN_ID_V4M_APVI4_0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APVC1_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APVC1_1,
        R_PMA_CPG_DOMAIN_ID_V4M_APVC1_1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APVC1_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_APVC1_0,
        R_PMA_CPG_DOMAIN_ID_V4M_APVC1_0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMDSP_CKMIR,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMDSP_CKMIR,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMDSP_CKMIR,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMCNR_CKMIR,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMCNR_CKMIR,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMCNR_CKMIR,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKM,
        R_PMA_CPG_DOMAIN_ID_V4M_CKM,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMHSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMHSC,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMHSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CKMCR52,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMCR52,
        R_PMA_CPG_DOMAIN_ID_V4M_CKMCR52,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ANHC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_ANHC,
        R_PMA_CPG_DOMAIN_ID_V4M_ANHC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXHC_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXHC_AXRS,
        R_PMA_CPG_DOMAIN_ID_V4M_AXHC_AXRS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_AXHC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_AXHC,
        R_PMA_CPG_DOMAIN_ID_V4M_AXHC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CSBRG_IR_A2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CSBRG_IR_A2,
        R_PMA_CPG_DOMAIN_ID_V4M_CSBRG_IR_A2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CSBRG_IR_A3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_CSBRG_IR_A3,
        R_PMA_CPG_DOMAIN_ID_V4M_CSBRG_IR_A3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_TSN,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_TSN,
        R_PMA_CPG_DOMAIN_ID_V4M_TSN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CVE3,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CV3,
        R_PMA_CPG_DOMAIN_ID_V4M_CVE3,
        R_PMA_CPG_DOMAIN_ID_V4M_CVE3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CVE2,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CV2,
        R_PMA_CPG_DOMAIN_ID_V4M_CVE2,
        R_PMA_CPG_DOMAIN_ID_V4M_CVE2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CVE1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CV1,
        R_PMA_CPG_DOMAIN_ID_V4M_CVE1,
        R_PMA_CPG_DOMAIN_ID_V4M_CVE1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CVE0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CV0,
        R_PMA_CPG_DOMAIN_ID_V4M_CVE0,
        R_PMA_CPG_DOMAIN_ID_V4M_CVE0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_REG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_REG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_CSB,
        R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP1,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_CSB,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_CORE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSPofV4M,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSPofV4M,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_SYSPO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_SYSPO,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_APBDBG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_APBDBG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_OCEM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_OCEM,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_CORE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_CORE,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_SYS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_SYS,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_GLOBAL,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_GLOBAL,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_REG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_REG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_CSB,
        R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP0,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_CSB,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_CORE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSPofV4M,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSPofV4M,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FCPRC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_FCPRC,
        R_PMA_CPG_DOMAIN_ID_V4M_FCPRC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PAPSDMA,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPSDMA,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPSDMA,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_HSM,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_PAPBUS,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPBUS,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPBUS,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_BUS,
        R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP1,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_BUS,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_CSB,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSPofV4M,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSPofV4M,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_BUS,
        R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP0,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_BUS,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_CSB,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSPofV4M,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSPofV4M,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_SSI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SSI,
        R_PMA_CPG_DOMAIN_ID_V4M_SSI,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SSIU,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SSIU,
        R_PMA_CPG_DOMAIN_ID_V4M_SSIU,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_SYSPO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_SYSPO,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_APBDBG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_APBDBG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_OCEM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_OCEM,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_CORE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_CORE,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_SYS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_SYS,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_GLOBAL,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_GLOBAL,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_REG_SPMI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMI,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMI,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_REG_SPMC,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CN0,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMC,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DOF_FCPRM,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP0,
        R_PMA_CPG_DOMAIN_ID_V4M_UMFL0,
        R_PMA_CPG_DOMAIN_ID_V4M_UMFL0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DCM,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_CBF,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SBF,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_OSM,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ISM0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_ISM1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_FBS,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_APB,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_DUL,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_CPG_DOMAIN_ID_V4M_PAPTOP,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_SPMC01,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CN0,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMC,
        R_PMA_CPG_DOMAIN_ID_V4M_SPMC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2IMP01,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2IMP01,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_OCV0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CV0,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_OCV1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CV1,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_OCV2,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CV2,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_OCV3,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CV3,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_DP0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2PSC,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_DP1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2DMA,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_CNN0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A1CN0,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_DSP0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP0,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_DSP1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A1DSP1,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A2CN0,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_UMFL0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP0,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_SMPS0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_SMPO0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_PAP0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_ISP0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMR0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR0,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMR1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR1,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMS0,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR2,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_FBA_IMS1,
        R_PMA_SYSC_DOMAIN_ID_V4M_A3IMR3,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP0_BUS,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4M_VDSP1_BUS,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
};
