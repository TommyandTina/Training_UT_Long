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
* Version :      0.6.0
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
#define R_PMA_HW_DEP_HWA_BIT_V4H_NONE           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(0ULL)            /* No dependency */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMPCNN         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 0ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_IMPCNN */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC0         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 1ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_IMPSDMAC0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC1         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 2ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_IMPSDMAC1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SPMC           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 3ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_SPMC */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMP0           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 4ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_IMP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMP1           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 5ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_IMP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMP2           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 6ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_IMP2 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMP3           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 7ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_IMP3 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMPDMAC0       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 8ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_IMPDMAC0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMPDMAC1       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 9ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_IMPDMAC1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMPPSC0        R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 10ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_IMPPSC0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_CVE0           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 11ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_CVE0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_CVE1           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 12ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_CVE1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_CVE2           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 13ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_CVE2 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_CVE3           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 14ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_CVE3 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMPDTA         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 15ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_IMPDTA */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IMPSLV         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 16ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_IMPSLV */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SPMI           R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 17ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VDSP0_CSB      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 18ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VDSP0_CSB */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VDSP0_BUS      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 19ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VDSP0_BUS */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VDSP1_CSB      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 20ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VDSP1_CSB */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VDSP1_BUS      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 21ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VDSP1_BUS */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VDSP2_BUS      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 22ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VDSP2_BUS */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VDSP2_CSB      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 23ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VDSP2_CSB */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VDSP3_BUS      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 24ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VDSP3_BUS */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VDSP3_CSB      R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 25ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VDSP3_CSB */
#define R_PMA_HW_DEP_HWA_BIT_V4H_IPMMU          R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 26ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR */
#define R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMI       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 27ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_REG_SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMC       R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 28ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_REG_SPMC */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SPMC01         R_PMA_HW_DEP_HWA_GROUP_0 | (uint64_t)(1ULL << 29ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_SPMC01 */

#define R_PMA_HW_DEP_HWA_BIT_V4H_SMPS0          R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 0ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_SMPS0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SMPO0          R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 1ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_SMPO0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_DOF            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 2ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_DOF */
#define R_PMA_HW_DEP_HWA_BIT_V4H_DOF_FCPRM      R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 3ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_DOF_FCPRM */
#define R_PMA_HW_DEP_HWA_BIT_V4H_PAPSDMA        R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 4ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_PAPSDMA */
#define R_PMA_HW_DEP_HWA_BIT_V4H_HSM            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 5ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_HSM */
#define R_PMA_HW_DEP_HWA_BIT_V4H_PAPBUS         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 6ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_PAPBUS */
#define R_PMA_HW_DEP_HWA_BIT_V4H_DCM            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 7ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_DCM */
#define R_PMA_HW_DEP_HWA_BIT_V4H_CFB            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 8ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_CBF */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SBF            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 9ULL)    /* Bit assignment of R_PMA_HWA_ID_V4H_SBF */
#define R_PMA_HW_DEP_HWA_BIT_V4H_OSM            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 10ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_OSM */
#define R_PMA_HW_DEP_HWA_BIT_V4H_ISM0           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 11ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_ISM0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_ISM1           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 12ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_ISM1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_FBS            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 13ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_FBS */
#define R_PMA_HW_DEP_HWA_BIT_V4H_APB            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 14ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_APB */
#define R_PMA_HW_DEP_HWA_BIT_V4H_DUL            R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 15ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_DUL */
#define R_PMA_HW_DEP_HWA_BIT_V4H_ISP1           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 16ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_ISP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_ISP0           R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 17ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_ISP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_ISPCS1         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 18ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_ISPCS1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_ISPCS0         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 19ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_ISPCS0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VSPX1          R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 20ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VSPX1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_VSPX0          R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 21ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_VSPX0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_FCPVX1         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 22ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_FCPVX1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_FCPVX0         R_PMA_HW_DEP_HWA_GROUP_1 | (uint64_t)(1ULL << 23ULL)   /* Bit assignment of R_PMA_HWA_ID_V4H_FCPVX0 */

/* Bit assignment of A1 children */
#define R_PMA_HW_DEP_HWA_BIT_V4H_A1_CHILDREN  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_IMPCNN          | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_VDSP0_CSB       | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_VDSP0_BUS       | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_VDSP1_CSB       | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_VDSP1_BUS       | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_VDSP2_BUS       | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_VDSP2_CSB       | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_VDSP3_BUS       | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_VDSP3_CSB)

/* Bit assignment of A2 children */
#define R_PMA_HW_DEP_HWA_BIT_V4H_A2_CHILDREN  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SPMC     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC0   | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC1   | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMP0     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMP1     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMP2     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMP3     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMPDMAC0 | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMPDMAC1 | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMPPSC0  | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_CVE0     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_CVE1     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_CVE2     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_CVE3     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMC | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_SPMC01   | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMPDTA)

/* Bit assignment of A2 parent */
#define R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SPMC     | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC0   | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC1   | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMC | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_SPMC01)

/* Bit assignment of A3IR parent */
#define R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_IMPSLV | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_SPMI   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_IPMMU  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMI)

/* Bit assignment of SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V4H_CONCLUSION_SPMI  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SPMI   | \
                                                             R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMI)

/* Bit assignment of SPMC */
#define R_PMA_HW_DEP_HWA_BIT_V4H_CONCLUSION_SPMC  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SPMC     | \
                                                             R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMC | \
                                                             R_PMA_HW_DEP_HWA_BIT_V4H_SPMC01)

/* Share SYSC domain of A1DSP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_VDSP0_CSB | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_VDSP0_BUS)

/* Share SYSC domain of A1DSP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_VDSP1_CSB | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_VDSP1_BUS)

/* Share SYSC domain of A1DSP2 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP2  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_VDSP2_BUS | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_VDSP2_CSB)

/* Share SYSC domain of A1DSP3 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP3  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_VDSP3_BUS | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_VDSP3_CSB)

/* Share SYSC domain of A2CN0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2CN0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SPMC     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC1   | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC0   | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMC | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_SPMC01)

/* Share SYSC domain of A2IMP01 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2IMP01  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_IMP1 | \
                                                           R_PMA_HW_DEP_HWA_BIT_V4H_IMP0)

/* Share SYSC domain of A2IMP23 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2IMP23  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_IMP3 | \
                                                           R_PMA_HW_DEP_HWA_BIT_V4H_IMP2)

/* Share SYSC domain of A2PSC */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2PSC  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_IMPPSC0 | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_IMPDMAC0)

/* Share SYSC domain of _A3IR */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3IR  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SPMI   | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_IMPSLV | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_IMPDTA | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_IPMMU  | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMI)

/* Share SYSC domain of A3DUL */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_PAPSDMA | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_HSM     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_PAPBUS  | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_DCM     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_CFB     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_SBF     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_OSM     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_ISM0    | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_ISM1    | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_FBS     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_APB     | \
                                                         R_PMA_HW_DEP_HWA_BIT_V4H_DUL)

/* Share SYSC domain of A3ISP0 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP0  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_ISP0   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_ISPCS0 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_VSPX0  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_FCPVX0)

/* Share SYSC domain of A3ISP1 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP1  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_ISP1   | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_ISPCS1 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_VSPX1  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_FCPVX1)

/* Share SYSC domain of A3VIP2 */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3VIP2  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SMPS0 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_SMPO0)

/* Share CPG domain of DOF */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_DOF  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_DOF | \
                                                       R_PMA_HW_DEP_HWA_BIT_V4H_DOF_FCPRM)

/* Share CPG domain of SPMC */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_SPMC  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SPMC     | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMC |\
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_SPMC01)

/* Share CPG domain of PAPTOP */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_HSM  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_DCM  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_CFB  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_SBF  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_OSM  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_ISM0 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_ISM1 | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_FBS  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_APB  | \
                                                          R_PMA_HW_DEP_HWA_BIT_V4H_DUL)
                                                          
/* Share CPG domain of SPMI */
#define R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_SPMI  (uint64_t)(R_PMA_HW_DEP_HWA_BIT_V4H_SPMI | \
                                                        R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMI)

/*======================================================================================================================
Private global variables
======================================================================================================================*/
/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const st_hwa_dependent_flag_t g_pma_hw_dep_hwa_dependency_tbl_v4h[R_PMA_HWA_ID_V4H_NUM] = {
    /* parent,                                      children                                      share sysc domain*/
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2PSC,     R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMPPSC                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2PSC,     R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMPDMAC0              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2IMP01,   R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMP1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2IMP01,   R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMP0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_A1_CHILDREN,         R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_SPMC  },     /* R_PMA_HWA_ID_V4H_SPMC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_CONCLUSION_SPMC,     R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMPCNN                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP1,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ISP1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ISP0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANRT1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_DOF   },     /* R_PMA_HWA_ID_V4H_DOF                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3VIP2,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SMPS0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3VIP2,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SMPO0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RGX                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANVI1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANVI0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANVC1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANVC0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANSP0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANRT                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANPV0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ADVFSC                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ADG                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_IMPSLV,              R_PMA_HW_DEP_HWA_BIT_V4H_A2_CHILDREN,         R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3IR,      R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_SPMI  },     /* R_PMA_HWA_ID_V4H_SPMI                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_CONCLUSION_SPMI,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3IR,      R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMPSLV                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3IR,      R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMPDTA                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXIMP0_AXRS           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXIMP0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANIMP1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANIMP0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMPDMAC1              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2IMP23,   R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMP3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2IMP23,   R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMP2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSM_AXRS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSD0_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSD0_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRS_AXSC             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRT_AXRS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRT                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRD_1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRD_0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRC_1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRC_0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRS_AXPV             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXPV                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANMM_AXMM             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AVB2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AVB1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AVB0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ARMREG                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ARMM                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APSP3_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APSP3_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APSP4_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APSP4_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APRT0_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APRT0_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANVIP2                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANVIP1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANVIP0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CSITOP0               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CSDBGPAP              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CR0                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CANFD                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVIP1_1              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVIP1_0              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVIP0_AXRS           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVIP0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVIFF                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVI1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVI1_AXRS            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVI                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVC_AXRS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXTBH                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSTM                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSTM_AXRS            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRS_AXSP0            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSP0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSN                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXRS_AXSN             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXSM                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_DSITXLINK1            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_DSITXLINK0            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_DOC2CH                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_DIS0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_DDR3_DDR2             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_DDR1_DDR0             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CSITOP1               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_INTAP                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMS1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMS0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMR2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMR1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMR0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_I2C5                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_I2C4                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_I2C3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_I2C2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_I2C1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_I2C0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_HSCIF3                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_HSCIF2                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_HSCIF1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_HSCIF0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FRAY00                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FCPVD1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FCPVD0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FCPCS                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RTDM1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RTDM0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RPC                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PWM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PCIE10                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PCIE00                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_MSI5                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_MSI4                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_MSI3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_MSI2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_MSI1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_MSI0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IVCP1E                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP1,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ISPCS1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ISPCS0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IRQC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_INTTP                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IPC                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN01                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN00                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VCPL4                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC7              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC6              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC5              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC4              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC3              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC2              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC1              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC0              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AES_ACC_WRAP          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_TPU0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_TMU4                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_TMU3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_TMU2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_TMU1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_TMU0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SYDM2                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SYDM1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SECROM                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SDHI0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SCIF4                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SCIF3                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SCIF1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SCIF0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RTDM3                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RTDM2                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VSPD1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VSPD0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN17                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN16                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN15                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN14                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN13                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN12                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN11                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN10                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN07                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN06                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN05                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN04                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN03                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VIN02                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_UCMT                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CMT3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CMT2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CMT1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CMT0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WDT1_WDT0             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WCRC3                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WCRC2                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WCRC1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WCRC0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP1,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VSPX1                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VSPX0                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RGXC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RGXB                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SWDT1_SWDT0           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RGXPVC                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_RGXPVDE               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PCIE11                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PCIE01                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_INTAP11               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FRAY01                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXVI11                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AULK4P                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AULK2P                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP1,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FCPVX1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3ISP0,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FCPVX0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_KCRC4                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FSO                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CRC3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CRC2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CRC1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CRC0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ADVFS                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SCMT                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC3D3                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC3D2                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC3D1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC2D3                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC2D2                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC2D1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC1D3                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC1D2                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC1D1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC0D3                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC0D2                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PFC0D1                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT9_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT8_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT7_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT6_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT5_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT4_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT3_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT2_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT1_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT0_RCLK            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_DBE                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_MTI                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_KCRC7                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_KCRC6                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_KCRC5                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMMM                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMIR                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMVIP                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMVC                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMVIO                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMPE0                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMRT                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKM3DG                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APVI4_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APVI4_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APVC1_1               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_APVC1_0               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMDSP_CKMIR          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMCNR_CKMIR          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMHSC                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CKMCR52               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_ANHC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXHC_AXRS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_AXHC                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CSBRG_IR_A2           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CSBRG_IR_A3           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_TSN                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_CONCLUSION_SPMC,     R_PMA_HW_DEP_HWA_BIT_V4H_A1_CHILDREN,         R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMPSDMAC1             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_CONCLUSION_SPMC,     R_PMA_HW_DEP_HWA_BIT_V4H_A1_CHILDREN,         R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IMPSDMAC0             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CVE3                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CVE2                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CVE1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_CVE0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_REG             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP1,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_CSB             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_SYSPO           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_APBDBG          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_OCEM            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_CORE            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_SYS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_GLOBAL          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_REG             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP0,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_CSB             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_DSC                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_FCPRC                 */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PAPSDMA               */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_HSM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_PAPBUS                */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP3,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_BUS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP2,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_BUS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP1,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_BUS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP0,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP0_BUS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SSI                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_SSIU                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_SYSPO           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_APBDBG          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_OCEM            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_CORE            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_SYS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_GLOBAL          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_REG             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP3,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP3_CSB             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_SYSPO           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_APBDBG          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_OCEM            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_CORE            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_SYS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_GLOBAL          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_REG             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A2_PARENTS,          R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A1DSP2,    R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP2_CSB             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_SYSPO           */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_APBDBG          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_OCEM            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_CORE            */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_SYS             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_VDSP1_GLOBAL          */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3IR,      R_PMA_HW_DEP_HWA_BIT_V4H_NONE        },     /* R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR        */
    { R_PMA_HW_DEP_HWA_BIT_V4H_IMPSLV,              R_PMA_HW_DEP_HWA_BIT_V4H_A2_CHILDREN,         R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3IR,      R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_SPMI  },     /* R_PMA_HWA_ID_V4H_REG_SPMI              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_A1_CHILDREN,         R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_SPMC  },     /* R_PMA_HWA_ID_V4H_REG_SPMC              */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,            R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_DOF   },     /* R_PMA_HWA_ID_V4H_DOF_FCPRM             */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_DCM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_CBF                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_SBF                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_OSM                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_ISM0                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_ISM1                  */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_FBS                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_APB                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_NONE,                R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A3DUL,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_PAPTOP},     /* R_PMA_HWA_ID_V4H_DUL                   */
    { R_PMA_HW_DEP_HWA_BIT_V4H_A3IR_PARENTS,        R_PMA_HW_DEP_HWA_BIT_V4H_A1_CHILDREN,         R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_A2CN0,     R_PMA_HW_DEP_HWA_BIT_V4H_SHARE_SPMC  }      /* R_PMA_HWA_ID_V4H_SPMC01                */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const uint64_t g_pma_hw_dep_hwa_bit_tbl_v4h[R_PMA_HWA_ID_V4H_NUM] = {
    R_PMA_HW_DEP_HWA_BIT_V4H_IMPPSC0,          /* R_PMA_HWA_ID_V4H_IMPPSC                */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMPDMAC0,         /* R_PMA_HWA_ID_V4H_IMPDMAC0              */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMP1,             /* R_PMA_HWA_ID_V4H_IMP1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMP0,             /* R_PMA_HWA_ID_V4H_IMP0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_SPMC,             /* R_PMA_HWA_ID_V4H_SPMC                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMPCNN,           /* R_PMA_HWA_ID_V4H_IMPCNN                */
    R_PMA_HW_DEP_HWA_BIT_V4H_ISP1,             /* R_PMA_HWA_ID_V4H_ISP1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_ISP0,             /* R_PMA_HWA_ID_V4H_ISP0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANRT1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_DOF,              /* R_PMA_HWA_ID_V4H_DOF                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_SMPS0,            /* R_PMA_HWA_ID_V4H_SMPS0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_SMPO0,            /* R_PMA_HWA_ID_V4H_SMPO0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RGX                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANVI1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANVI0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANVC1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANVC0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANSP0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANRT                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANPV0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ADVFSC                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ADG                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_SPMI,             /* R_PMA_HWA_ID_V4H_SPMI                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMPSLV,           /* R_PMA_HWA_ID_V4H_IMPSLV                */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMPDTA,           /* R_PMA_HWA_ID_V4H_IMPDTA                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXIMP0_AXRS           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXIMP0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANIMP1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANIMP0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMPDMAC1,         /* R_PMA_HWA_ID_V4H_IMPDMAC1              */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMP3,             /* R_PMA_HWA_ID_V4H_IMP3                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_IMP2,             /* R_PMA_HWA_ID_V4H_IMP2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSM_AXRS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSD0_1               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSD0_0               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSC                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRS_AXSC             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRT_AXRS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRT                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRD_1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRD_0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRC_1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRC_0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRS_AXPV             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXPV                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANMM_AXMM             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AVB2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AVB1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AVB0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ARMREG                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ARMM                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APSP3_1               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APSP3_0               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APSP4_1               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APSP4_0               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APRT0_1               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APRT0_0               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANVIP2                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANVIP1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANVIP0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CSITOP0               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CSDBGPAP              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CR0                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CANFD                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVIP1_1              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVIP1_0              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVIP0_AXRS           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVIP0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVIFF                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVI1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVI1_AXRS            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVI                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVC_AXRS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVC                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXTBH                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSTM                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSTM_AXRS            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRS_AXSP0            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSP0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSN                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXRS_AXSN             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXSM                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_DSITXLINK1            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_DSITXLINK0            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_DOC2CH                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_DIS0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_DDR3_DDR2             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_DDR1_DDR0             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CSITOP1               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_INTAP                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_IMS1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_IMS0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_IMR2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_IMR1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_IMR0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_I2C5                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_I2C4                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_I2C3                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_I2C2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_I2C1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_I2C0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_HSCIF3                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_HSCIF2                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_HSCIF1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_HSCIF0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_FRAY00                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_FCPVD1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_FCPVD0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_FCPCS                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RTDM1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RTDM0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RPC                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PWM                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PCIE10                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PCIE00                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_MSI5                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_MSI4                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_MSI3                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_MSI2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_MSI1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_MSI0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_IVCP1E                */
    R_PMA_HW_DEP_HWA_BIT_V4H_ISPCS1,           /* R_PMA_HWA_ID_V4H_ISPCS1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_ISPCS0,           /* R_PMA_HWA_ID_V4H_ISPCS0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_IRQC                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_INTTP                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_IPC                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN01                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN00                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VCPL4                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC7              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC6              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC5              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC4              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC3              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC2              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC1              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC0              */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AES_ACC_WRAP          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_TPU0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_TMU4                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_TMU3                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_TMU2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_TMU1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_TMU0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SYDM2                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SYDM1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SECROM                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SDHI0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SCIF4                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SCIF3                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SCIF1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SCIF0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RTDM3                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RTDM2                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VSPD1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VSPD0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN17                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN16                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN15                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN14                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN13                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN12                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN11                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN10                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN07                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN06                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN05                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN04                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN03                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VIN02                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_UCMT                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC3                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CMT3                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CMT2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CMT1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CMT0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WDT1_WDT0             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WCRC3                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WCRC2                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WCRC1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WCRC0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_VSPX1,            /* R_PMA_HWA_ID_V4H_VSPX1                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_VSPX0,            /* R_PMA_HWA_ID_V4H_VSPX0                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RGXC                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RGXB                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SWDT1_SWDT0           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RGXPVC                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_RGXPVDE               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PCIE11                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PCIE01                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_INTAP11               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_FRAY01                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXVI11                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AULK4P                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AULK2P                */
    R_PMA_HW_DEP_HWA_BIT_V4H_FCPVX1,           /* R_PMA_HWA_ID_V4H_FCPVX1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_FCPVX0,           /* R_PMA_HWA_ID_V4H_FCPVX0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_KCRC4                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_FSO                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CRC3                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CRC2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CRC1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CRC0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ADVFS                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SCMT                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC3D3                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC3D2                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC3D1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC2D3                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC2D2                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC2D1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC1D3                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC1D2                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC1D1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC0D3                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC0D2                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_PFC0D1                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT9_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT8_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT7_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT6_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT5_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT4_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT3_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT2_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT1_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT0_RCLK            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT        */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_DBE                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_MTI                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_KCRC7                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_KCRC6                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_KCRC5                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMMM                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMIR                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMVIP                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMVC                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMVIO                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMPE0                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMRT                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKM3DG                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APVI4_1               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APVI4_0               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APVC1_1               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_APVC1_0               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMDSP_CKMIR          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMCNR_CKMIR          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKM                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMHSC                */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CKMCR52               */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_ANHC                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXHC_AXRS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_AXHC                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CSBRG_IR_A2           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_CSBRG_IR_A3           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_TSN                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC1,           /* R_PMA_HWA_ID_V4H_IMPSDMAC1             */
    R_PMA_HW_DEP_HWA_BIT_V4H_SDMAC0,           /* R_PMA_HWA_ID_V4H_IMPSDMAC0             */
    R_PMA_HW_DEP_HWA_BIT_V4H_CVE3,             /* R_PMA_HWA_ID_V4H_CVE3                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_CVE2,             /* R_PMA_HWA_ID_V4H_CVE2                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_CVE1,             /* R_PMA_HWA_ID_V4H_CVE1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_CVE0,             /* R_PMA_HWA_ID_V4H_CVE0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP1_REG             */
    R_PMA_HW_DEP_HWA_BIT_V4H_VDSP1_CSB,        /* R_PMA_HWA_ID_V4H_VDSP1_CSB             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP0_SYSPO           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP0_APBDBG          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP0_OCEM            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP0_CORE            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP0_SYS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP0_GLOBAL          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP0_REG             */
    R_PMA_HW_DEP_HWA_BIT_V4H_VDSP0_CSB,        /* R_PMA_HWA_ID_V4H_VDSP0_CSB             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_DSC                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_FCPRC                 */
    R_PMA_HW_DEP_HWA_BIT_V4H_PAPSDMA,          /* R_PMA_HWA_ID_V4H_PAPSDMA               */
    R_PMA_HW_DEP_HWA_BIT_V4H_HSM,              /* R_PMA_HWA_ID_V4H_HSM                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_PAPBUS,           /* R_PMA_HWA_ID_V4H_PAPBUS                */
    R_PMA_HW_DEP_HWA_BIT_V4H_VDSP3_BUS,        /* R_PMA_HWA_ID_V4H_VDSP3_BUS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_VDSP2_BUS,        /* R_PMA_HWA_ID_V4H_VDSP2_BUS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_VDSP1_BUS,        /* R_PMA_HWA_ID_V4H_VDSP1_BUS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_VDSP0_BUS,        /* R_PMA_HWA_ID_V4H_VDSP0_BUS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SSI                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_SSIU                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP3_SYSPO           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP3_APBDBG          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP3_OCEM            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP3_CORE            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP3_SYS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP3_GLOBAL          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP3_REG             */
    R_PMA_HW_DEP_HWA_BIT_V4H_VDSP3_CSB,        /* R_PMA_HWA_ID_V4H_VDSP3_CSB             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP2_SYSPO           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP2_APBDBG          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP2_OCEM            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP2_CORE            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP2_SYS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP2_GLOBAL          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP2_REG             */
    R_PMA_HW_DEP_HWA_BIT_V4H_VDSP2_CSB,        /* R_PMA_HWA_ID_V4H_VDSP2_CSB             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP1_SYSPO           */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP1_APBDBG          */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP1_OCEM            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP1_CORE            */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP1_SYS             */
    R_PMA_HW_DEP_HWA_BIT_V4H_NONE,             /* R_PMA_HWA_ID_V4H_VDSP1_GLOBAL          */
    R_PMA_HW_DEP_HWA_BIT_V4H_IPMMU,            /* R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR        */
    R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMI,         /* R_PMA_HWA_ID_V4H_REG_SPMI              */
    R_PMA_HW_DEP_HWA_BIT_V4H_REG_SPMC,         /* R_PMA_HWA_ID_V4H_REG_SPMC              */
    R_PMA_HW_DEP_HWA_BIT_V4H_DOF_FCPRM,        /* R_PMA_HWA_ID_V4H_DOF_FCPRM             */
    R_PMA_HW_DEP_HWA_BIT_V4H_DCM,              /* R_PMA_HWA_ID_V4H_DCM                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_CFB,              /* R_PMA_HWA_ID_V4H_CBF                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_SBF,              /* R_PMA_HWA_ID_V4H_SBF                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_OSM,              /* R_PMA_HWA_ID_V4H_OSM                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_ISM0,             /* R_PMA_HWA_ID_V4H_ISM0                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_ISM1,             /* R_PMA_HWA_ID_V4H_ISM1                  */
    R_PMA_HW_DEP_HWA_BIT_V4H_FBS,              /* R_PMA_HWA_ID_V4H_FBS                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_APB,              /* R_PMA_HWA_ID_V4H_APB                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_DUL,              /* R_PMA_HWA_ID_V4H_DUL                   */
    R_PMA_HW_DEP_HWA_BIT_V4H_SPMC01            /* R_PMA_HWA_ID_V4H_SPMC01                */
};

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
const e_pma_hwa_id_t g_pma_hw_dep_hwa_bit_to_id_tbl_v4h[R_PMA_HW_DEP_HWA_DEPEND_V4H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM] = {
    R_PMA_HWA_ID_V4H_IMPCNN,
    R_PMA_HWA_ID_V4H_IMPSDMAC0,
    R_PMA_HWA_ID_V4H_IMPSDMAC1,
    R_PMA_HWA_ID_V4H_SPMC,
    R_PMA_HWA_ID_V4H_IMP0,
    R_PMA_HWA_ID_V4H_IMP1,
    R_PMA_HWA_ID_V4H_IMP2,
    R_PMA_HWA_ID_V4H_IMP3,
    R_PMA_HWA_ID_V4H_IMPDMAC0,
    R_PMA_HWA_ID_V4H_IMPDMAC1,
    R_PMA_HWA_ID_V4H_IMPPSC,
    R_PMA_HWA_ID_V4H_CVE0,
    R_PMA_HWA_ID_V4H_CVE1,
    R_PMA_HWA_ID_V4H_CVE2,
    R_PMA_HWA_ID_V4H_CVE3,
    R_PMA_HWA_ID_V4H_IMPDTA,
    R_PMA_HWA_ID_V4H_IMPSLV,
    R_PMA_HWA_ID_V4H_SPMI,
    R_PMA_HWA_ID_V4H_VDSP0_CSB,
    R_PMA_HWA_ID_V4H_VDSP0_BUS,
    R_PMA_HWA_ID_V4H_VDSP1_CSB,
    R_PMA_HWA_ID_V4H_VDSP1_BUS,
    R_PMA_HWA_ID_V4H_VDSP2_BUS,
    R_PMA_HWA_ID_V4H_VDSP2_CSB,
    R_PMA_HWA_ID_V4H_VDSP3_BUS,
    R_PMA_HWA_ID_V4H_VDSP3_CSB,
    R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR,
    R_PMA_HWA_ID_V4H_REG_SPMI,
    R_PMA_HWA_ID_V4H_REG_SPMC,
    R_PMA_HWA_ID_V4H_SPMC01,
    R_PMA_HWA_ID_V4H_SMPS0,
    R_PMA_HWA_ID_V4H_SMPO0,
    R_PMA_HWA_ID_V4H_DOF,
    R_PMA_HWA_ID_V4H_DOF_FCPRM,
    R_PMA_HWA_ID_V4H_PAPSDMA,
    R_PMA_HWA_ID_V4H_HSM,
    R_PMA_HWA_ID_V4H_PAPBUS,
    R_PMA_HWA_ID_V4H_DCM,
    R_PMA_HWA_ID_V4H_CBF,
    R_PMA_HWA_ID_V4H_SBF,
    R_PMA_HWA_ID_V4H_OSM,
    R_PMA_HWA_ID_V4H_ISM0,
    R_PMA_HWA_ID_V4H_ISM1,
    R_PMA_HWA_ID_V4H_FBS,
    R_PMA_HWA_ID_V4H_APB,
    R_PMA_HWA_ID_V4H_DUL,
    R_PMA_HWA_ID_V4H_ISP1,
    R_PMA_HWA_ID_V4H_ISP0,
    R_PMA_HWA_ID_V4H_ISPCS1,
    R_PMA_HWA_ID_V4H_ISPCS0,
    R_PMA_HWA_ID_V4H_VSPX1,
    R_PMA_HWA_ID_V4H_VSPX0,
    R_PMA_HWA_ID_V4H_FCPVX1,
    R_PMA_HWA_ID_V4H_FCPVX0,
    R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM
};

/* PRQA S 1533,3408 2 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
uint64_t g_pma_hw_dep_sysc_ref_tbl_v4h[(uint64_t)R_PMA_SYSC_DOMAIN_ID_V4H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM] = { 0 };
uint64_t g_pma_hw_dep_cpg_ref_tbl_v4h[(uint64_t)R_PMA_CPG_DOMAIN_ID_V4H_NUM * R_PMA_HW_DEP_HWA_GROUP_NUM]   = { 0 };

/* PRQA S 1533,3408 1 # Because the SoC-dependent processing is divided by the file, it cannot be avoided */
st_hwa_t g_pma_hw_dep_hwa_info_tbl_v4h[R_PMA_HWA_ID_V4H_NUM] = {
    /* hwa id, sysc_id, cpg_id, reset_id, actual_rst_stat, ToPG, PGtoCG, HPtoCG, ToHP */
    {
        R_PMA_HWA_ID_V4H_IMPPSC,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2PSC,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPPSC,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPPSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMPDMAC0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2PSC,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPDMAC0,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPDMAC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMP1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2IMP01,
        R_PMA_CPG_DOMAIN_ID_V4H_IMP1,
        R_PMA_CPG_DOMAIN_ID_V4H_IMP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMP0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2IMP01,
        R_PMA_CPG_DOMAIN_ID_V4H_IMP0,
        R_PMA_CPG_DOMAIN_ID_V4H_IMP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SPMC,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CN0,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMC,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMPCNN,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1CNN0,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPCNN,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPCNN,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ISP1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3ISP1,
        R_PMA_CPG_DOMAIN_ID_V4H_ISP1,
        R_PMA_CPG_DOMAIN_ID_V4H_ISP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ISP0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4H_ISP0,
        R_PMA_CPG_DOMAIN_ID_V4H_ISP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANRT1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DOF,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3VIP0,
        R_PMA_CPG_DOMAIN_ID_V4H_UMFL0,
        R_PMA_CPG_DOMAIN_ID_V4H_UMFL0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SMPS0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V4H_SMPS0,
        R_PMA_CPG_DOMAIN_ID_V4H_SMPS0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SMPO0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3VIP2,
        R_PMA_CPG_DOMAIN_ID_V4H_SMPO0,
        R_PMA_CPG_DOMAIN_ID_V4H_SMPO0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RGX,
        R_PMA_SYSC_DOMAIN_ID_V4H_A23DGB,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H,
        &R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANVI1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANVI0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANVC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANVC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANSP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANPV0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ADVFSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ADG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SPMI,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMI,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMI,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMPSLV,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPSLV,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPSLV,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMPDTA,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPDTA,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPDTA,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXIMP0_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXIMP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANIMP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANIMP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMPDMAC1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2DMA,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPDMAC1,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPDMAC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMP3,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2IMP23,
        R_PMA_CPG_DOMAIN_ID_V4H_IMP3,
        R_PMA_CPG_DOMAIN_ID_V4H_IMP3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMP2,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2IMP23,
        R_PMA_CPG_DOMAIN_ID_V4H_IMP2,
        R_PMA_CPG_DOMAIN_ID_V4H_IMP2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSM_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSD0_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSD0_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRS_AXSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRT_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRD_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRD_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRC_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRC_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRS_AXPV,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXPV,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANMM_AXMM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AVB2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AVB1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AVB0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ARMREG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ARMM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APSP3_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APSP3_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APSP4_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APSP4_0 ,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE ,
        R_PMA_CPG_DOMAIN_ID_NONE ,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APRT0_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APRT0_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANVIP2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANVIP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANVIP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CSITOP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_CSITOP0,
        R_PMA_CPG_DOMAIN_ID_V4H_CSITOP0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CSDBGPAP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CR0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CANFD,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVIP1_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVIP1_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVIP0_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVIP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVIFF,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVI1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVI1_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVC_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXTBH,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSTM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSTM_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRS_AXSP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSP0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSN,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXRS_AXSN,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXSM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DSITXLINK1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_DSITXLINK1,
        R_PMA_CPG_DOMAIN_ID_V4H_DSITXLINK1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DSITXLINK0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_DSITXLINK0,
        R_PMA_CPG_DOMAIN_ID_V4H_DSITXLINK0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DOC2CH,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DIS0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_DIS0,
        R_PMA_CPG_DOMAIN_ID_V4H_DIS0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DDR3_DDR2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DDR1_DDR0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CSITOP1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_CSITOP1,
        R_PMA_CPG_DOMAIN_ID_V4H_CSITOP1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_INTAP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMS1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_IMS1,
        R_PMA_CPG_DOMAIN_ID_V4H_IMS1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMS0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_IMS0,
        R_PMA_CPG_DOMAIN_ID_V4H_IMS0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMR2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_IMR2,
        R_PMA_CPG_DOMAIN_ID_V4H_IMR2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMR1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_IMR1,
        R_PMA_CPG_DOMAIN_ID_V4H_IMR1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMR0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_IMR0,
        R_PMA_CPG_DOMAIN_ID_V4H_IMR0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_I2C5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_I2C4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_I2C3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_I2C2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_I2C1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_I2C0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_HSCIF3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_HSCIF2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_HSCIF1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_HSCIF0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FRAY00,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FCPVD1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_FCPVD1,
        R_PMA_CPG_DOMAIN_ID_V4H_FCPVD1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FCPVD0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_FCPVD0,
        R_PMA_CPG_DOMAIN_ID_V4H_FCPVD0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FCPCS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RTDM1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RTDM0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RPC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PWM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PCIE10,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PCIE00,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_MSI5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_MSI4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_MSI3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_MSI2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_MSI1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_MSI0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IVCP1E,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ISPCS1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3ISP1,
        R_PMA_CPG_DOMAIN_ID_V4H_ISPCS1,
        R_PMA_CPG_DOMAIN_ID_V4H_ISPCS1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ISPCS0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4H_ISPCS0,
        R_PMA_CPG_DOMAIN_ID_V4H_ISPCS0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IRQC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_INTTP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IPC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN01,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN01,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN01,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN00,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN00,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN00,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VCPL4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC7,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC6,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AES_ACC_WRAP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_TPU0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_TMU4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_TMU3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_TMU2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_TMU1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_TMU0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SYDM2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SYDM1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SECROM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SDHI0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SCIF4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SCIF3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SCIF1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SCIF0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RTDM3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RTDM2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VSPD1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VSPD1,
        R_PMA_CPG_DOMAIN_ID_V4H_VSPD1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VSPD0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VSPD0,
        R_PMA_CPG_DOMAIN_ID_V4H_VSPD0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN17,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN17,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN17,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN16,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN16,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN16,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN15,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN15,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN15,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN14,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN14,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN14,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN13,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN13,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN13,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN12,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN12,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN12,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN11,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN11,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN10,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN10,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN10,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN07,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN07,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN07,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN06,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN06,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN06,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN05,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN05,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN05,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN04,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN04,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN04,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN03,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN03,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN03,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VIN02,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN02,
        R_PMA_CPG_DOMAIN_ID_V4H_VIN02,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_UCMT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CMT3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CMT2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CMT1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CMT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WDT1_WDT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WCRC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WCRC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WCRC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WCRC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VSPX1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3ISP1,
        R_PMA_CPG_DOMAIN_ID_V4H_VSPX1,
        R_PMA_CPG_DOMAIN_ID_V4H_VSPX1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VSPX0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4H_VSPX0,
        R_PMA_CPG_DOMAIN_ID_V4H_VSPX0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RGXC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RGXB,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SWDT1_SWDT0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RGXPVC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_RGXPVDE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PCIE11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PCIE01,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_INTAP11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FRAY01,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXVI11,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AULK4P,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AULK2P,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FCPVX1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3ISP1,
        R_PMA_CPG_DOMAIN_ID_V4H_FCPVX1,
        R_PMA_CPG_DOMAIN_ID_V4H_FCPVX1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FCPVX0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3ISP0,
        R_PMA_CPG_DOMAIN_ID_V4H_FCPVX0,
        R_PMA_CPG_DOMAIN_ID_V4H_FCPVX0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_KCRC4,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FSO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CRC3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CRC2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CRC1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CRC0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ADVFS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SCMT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC3D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC3D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC3D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC2D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC2D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC2D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC1D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC1D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC1D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC0D3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC0D2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PFC0D1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT9_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT8_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT7_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT6_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT5_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT4_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT3_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT2_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT1_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT0_RCLK,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DBE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_MTI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_KCRC7,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_KCRC6,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_KCRC5,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMMM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMIR,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMVIP,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMVC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMVIO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMPE0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMRT,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKM3DG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APVI4_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APVI4_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APVC1_1,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APVC1_0,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMDSP_CKMIR,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMCNR_CKMIR,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMHSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CKMCR52,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ANHC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXHC_AXRS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_AXHC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CSBRG_IR_A2,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CSBRG_IR_A3,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_TSN,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMPSDMAC1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CN0,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPSDMAC1,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPSDMAC1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_IMPSDMAC0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CN0,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPSDMAC0,
        R_PMA_CPG_DOMAIN_ID_V4H_IMPSDMAC0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CVE3,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CV3,
        R_PMA_CPG_DOMAIN_ID_V4H_CVE3,
        R_PMA_CPG_DOMAIN_ID_V4H_CVE3,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CVE2,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CV2,
        R_PMA_CPG_DOMAIN_ID_V4H_CVE2,
        R_PMA_CPG_DOMAIN_ID_V4H_CVE2,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CVE1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CV1,
        R_PMA_CPG_DOMAIN_ID_V4H_CVE1,
        R_PMA_CPG_DOMAIN_ID_V4H_CVE1,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CVE0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CV0,
        R_PMA_CPG_DOMAIN_ID_V4H_CVE0,
        R_PMA_CPG_DOMAIN_ID_V4H_CVE0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_REG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_REG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_CSB,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1DSP1,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_CSB,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_CORE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSP,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_SYSPO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_SYSPO,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_APBDBG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_APBDBG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_OCEM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_OCEM,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_CORE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_SYS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_SYS,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_GLOBAL,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_GLOBAL,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_REG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_REG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_CSB,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1DSP0,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_CSB,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_CORE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSP,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_DSC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_DSC,
        R_PMA_CPG_DOMAIN_ID_V4H_DSC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FCPRC,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PAPSDMA,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_HSM,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_PAPBUS,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_BUS,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1DSP3,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_BUS,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_CSB,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSP,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_BUS,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1DSP2,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_BUS,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_CSB,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSP,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_BUS,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1DSP1,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_BUS,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_CSB,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSP,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP0_BUS,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1DSP0,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_BUS,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP0_CSB,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSP,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_SSI,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SSIU,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_SYSPO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_SYSPO,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_APBDBG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_APBDBG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_OCEM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_OCEM,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_CORE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_SYS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_SYS,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_GLOBAL,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_GLOBAL,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_REG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_REG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP3_CSB,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1DSP3,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_CSB,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP3_CORE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSP,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_SYSPO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_SYSPO,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_APBDBG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_APBDBG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_OCEM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_OCEM,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_CORE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_SYS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_SYS,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_GLOBAL,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_GLOBAL,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_REG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_REG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP2_CSB,
        R_PMA_SYSC_DOMAIN_ID_V4H_A1DSP2,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_CSB,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP2_CORE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforVDSP,
        &R_PMA_HW_DEP_NoticeChangeToHPforVDSP,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_SYSPO,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_SYSPO,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_APBDBG,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_APBDBG,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_OCEM,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_OCEM,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_CORE,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_SYS,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_SYS,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_VDSP1_GLOBAL,
        R_PMA_SYSC_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_V4H_VDSP1_GLOBAL,
        R_PMA_UNDEFINED_RESET,
        NULL,
        NULL,
        NULL,
        NULL,
        P_PMA_CPG_NO_DEPEND
    },
    {
        R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3IR,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        &R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_REG_SPMI,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3IR,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMI,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMI,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_REG_SPMC,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CN0,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMC,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DOF_FCPRM,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3VIP0,
        R_PMA_CPG_DOMAIN_ID_V4H_UMFL0,
        R_PMA_CPG_DOMAIN_ID_V4H_UMFL0,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeA,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DCM,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_CBF,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SBF,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_OSM,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ISM0,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_ISM1,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_FBS,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_APB,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_DUL,
        R_PMA_SYSC_DOMAIN_ID_V4H_A3DUL,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_CPG_DOMAIN_ID_NONE,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeB,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeB,
        R_PMA_CPG_DEPEND_SET
    },
    {
        R_PMA_HWA_ID_V4H_SPMC01,
        R_PMA_SYSC_DOMAIN_ID_V4H_A2CN0,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMC,
        R_PMA_CPG_DOMAIN_ID_V4H_SPMC,
        R_PMA_UNDEFINED_RESET,
        &R_PMA_HW_DEP_NoticeChangeToPGforTypeC,
        &R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC,
        &R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC,
        &R_PMA_HW_DEP_NoticeChangeToHPforTypeAC,
        R_PMA_CPG_DEPEND_SET
    }
};
