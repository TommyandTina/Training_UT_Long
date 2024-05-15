/***********************************************************************************************************************
* Copyright [2020-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name    : r_impdrv_impctl.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_impctl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_impctl.c
 */

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "rcar-xos/imp/r_impdrv.h"
#include "r_impdrv_private.h"

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Defines Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_CORENUM_VAL
 * CORENUM_VAL register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_CORENUM_VAL      (4U)                /**< Maximum number of cores for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_VCR
 * Offset address of VCR register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_VCR          (0x0004U)           /**< Offset address of VCR register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCF
 * Offset address of CNF register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_CNF          (0x0008U)           /**< Offset address of CNF register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCFG
 * Offset address of CNFG register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IFCFG        (0x0018U)           /**< Offset address of CNFG register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCTL
 * Offset address of CIFCTL register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IFCTL        (0x001CU)           /**< Offset address of IFCTL register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR0
 * Offset address of SYNCCR0 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR0      (0x0040U)           /**< Offset address of SYNCCR0 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR1
 * Offset address of SYNCCR1 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR1      (0x0044U)           /**< Offset address of SYNCCR1 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR2
 * Offset address of SYNCCR2 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR2      (0x0048U)           /**< Offset address of SYNCCR2 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR03
 * Offset address of SYNCCR3 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR3      (0x004CU)           /**< Offset address of SYNCCR03 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_INTSTS
 * Offset address of INTSTS register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_INTSTS       (0x0010U)           /**< Offset address of INTSTS register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_INTEN
 * Offset address of INTEN register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_INTEN        (0x0014U)           /**< Offset address of INTEN register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_PSA
 * Offset address of PSA register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_PSA          (0x0020U)           /**< Offset address of PSA register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_HMPTR
 * Offset address of HMPTR register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_HMPTR        (0x0514U)           /**< Offset address of HMPTR register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_HMDATA
 * Offset address of HMDATA register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_HMDATA       (0x0518U)           /**< Offset address of HMDATA register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_HM1PTR
 * Offset address of HM1PTR register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_HM1PTR       (0x0594U)           /**< Offset address of HM1PTR register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_HM1DATA
 * Offset address of HM1DATA register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_HM1DATA      (0x0598U)           /**< Offset address of HM1DATA register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_MEM_FUNC_SA
 * Offset address of SA register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_MEM_FUNC_SA  (0x0B78U)           /**< Offset address of SA register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APSASP
 * Offset address of APSASP register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APSASP       (0x0100U)           /**< Offset address of APSASP register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APSBSP
 * Offset address of APSBSP register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APSBSP       (0x0104U)           /**< Offset address of APSBSP register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APDSP
 * Offset address of APSBSP register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APDSP        (0x0108U)           /**< Offset address of APSBSP register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APLNG
 * Offset address of APLNG register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APLNG        (0x010CU)           /**< Offset address of APLNG register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APDLY
 * Offset address of APDLY register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APDLY        (0x0110U)           /**< Offset address of APDLY register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APMAG
 * Offset address of APMAG register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APMAG        (0x0114U)           /**< Offset address of APMAG register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APSIZE_SA
 * Offset address of APSIZE_SA register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APSIZE_SA    (0x0118U)           /**< Offset address of APSIZE_SA register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APSIZE_SB
 * Offset address of APSIZE_SB register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APSIZE_SB    (0x011CU)           /**< Offset address of APSIZE_SB register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APSIZE_DST
 * Offset address of APSIZE_DST register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APSIZE_DST   (0x0120U)           /**< Offset address of APSIZE_DST register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APCMD
 * Offset address of APCMD register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APCMD        (0x0128U)           /**< Offset address of APCMD register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APCLPX
 * Offset address of APCLPX register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APCLPX       (0x012CU)           /**< Offset address of APCLPX register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_APCFG
 * Offset address of APCFG register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_APCFG        (0x013CU)           /**< Offset address of APCFG register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IPFUN
 * Offset address of IPFUN register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IPFUN        (0x0800U)           /**< Offset address of IPFUN register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IPFUN2
 * Offset address of IPFUN2 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IPFUN2       (0x0804U)           /**< Offset address of IPFUN2 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IPFORM
 * Offset address of IPFORM register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IPFORM       (0x0808U)           /**< Offset address of IPFORM register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_CNST
 * Offset address of CNST register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_CNST         (0x0814U)           /**< Offset address of CNST register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_BINTHR
 * Offset address of BINTHR register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_BINTHR       (0x0818U)           /**< Offset address of BINTHR register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_BINTHR2
 * Offset address of BINTHR2 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_BINTHR2      (0x081CU)           /**< Offset address of BINTHR2 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_KNLMSK
 * Offset address of KNLMSK register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_KNLMSK       (0x0820U)           /**< Offset address of KNLMSK register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_KNLMSK2
 * Offset address of KNLMSK2 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_KNLMSK2      (0x0824U)           /**< Offset address of KNLMSK2 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_LMCTL
 * Offset address of LMCTL register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_LMCTL        (0x0828U)           /**< Offset address of LMCTL register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_LABCNT
 * Offset address of LABCNT register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_LABCNT       (0x082CU)           /**< Offset address of LABCNT register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF02
 * Offset address of COEFF02 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF02      (0x0830U)           /**< Offset address of COEFF02 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF35
 * Offset address of COEFF35 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF35      (0x0834U)           /**< Offset address of COEFF35 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF68
 * Offset address of COEFF68 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF68      (0x0838U)           /**< Offset address of COEFF68 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF911
 * Offset address of COEFF911 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF911     (0x083CU)           /**< Offset address of COEFF911 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF1214
 * Offset address of COEFF1214 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF1214    (0x0840U)           /**< Offset address of COEFF1214 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF15
 * Offset address of COEFF15 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF15      (0x0844U)           /**< Offset address of COEFF15 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF1820
 * Offset address of COEFF1820 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF1820    (0x0848U)           /**< Offset address of COEFF1820 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF2123
 * Offset address of COEFF2123 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF2123    (0x084CU)           /**< Offset address of COEFF2123 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF24
 * Offset address of COEFF24 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF24      (0x0850U)           /**< Offset address of COEFF24 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF02H
 * Offset address of COEFF02H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF02H     (0x0854U)           /**< Offset address of COEFF02H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF35H
 * Offset address of COEFF35H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF35H     (0x0858U)           /**< Offset address of COEFF35H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF68H
 * Offset address of COEFF68H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF68H     (0x085CU)           /**< Offset address of COEFF68H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF911H
 * Offset address of COEFF911H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF911H    (0x0860U)           /**< Offset address of COEFF911H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF1214H
 * Offset address of COEFF1214H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF1214H   (0x0864U)           /**< Offset address of COEFF1214H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF1517H
 * Offset address of COEFF1517H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF1517H   (0x0868U)           /**< Offset address of COEFF1517H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF1820H
 * Offset address of COEFF1820H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF1820H   (0x086CU)           /**< Offset address of COEFF1820H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF2123H
 * Offset address of COEFF2123H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF2123H   (0x0870U)           /**< Offset address of COEFF2123H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_COEFF24H
 * Offset address of COEFF24H register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_COEFF24H     (0x0874U)           /**< Offset address of COEFF24H register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_PFFTASEL1
 * Offset address of PFFTASEL1 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_PFFTASEL1    (0x0A80U)           /**< Offset address of PFFTASEL1 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_PFFTAMSK
 * Offset address of PFFTAMSK register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_PFFTAMSK     (0x0A88U)           /**< Offset address of PFFTAMSK register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_PFFTBMSK
 * Offset address of PFFTBMSK register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_PFFTBMSK     (0x0A94U)           /**< Offset address of PFFTBMSK register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_VCR
 * Hardware version value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_VCR              (0x00600000U)       /**< Hardware version value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_CNF_VAL
 * CNF_VAL register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_CNF_VAL          (0x00000001U)       /**< CNF_VAL register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_CNF_SWRST
 * CNF_SWRST register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_CNF_SWRST        (0x80000000U)       /**< CNF_SWRST register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_IFCTL_EXE
 * EXE register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_IFCTL_EXE        (0x00000001U)       /**< EXE register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_IFCFG_VAL
 * IFCFG_VAL register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_IFCFG_VAL        (0x010010CCU)       /**< IFCFG_VAL register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_IFCTL_ENDIAN
 * ENDIAN register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_IFCTL_ENDIAN     (0x22020200U)       /**< ENDIAN register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_HMPTR_VAL
 * HMPTR_VAL register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_HMPTR_VAL        (0x87FFU)           /**< HMPTR_VAL register value for IMP.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_HMDATA_VAL
 * HMDATA_VAL register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_HMDATA_VAL       (0x0000U)           /**< HMDATA_VAL register value for IMP.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_MEM_FUNC_SA_VAL
 * SA_VAL register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_MEM_FUNC_SA_VAL  (0x00FFU)           /**< SA_VAL register value for IMP.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_MASKCPU
 *  MASKCPU interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_MASKCPU   (0x10000000U)       /**< MASKCPU interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_APIPINT
 *  APIPINT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_APIPINT   (0x00000001U)       /**< APIPINT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_HPINT
 *  HPINT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_HPINT     (0x00000002U)       /**< HPINT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_INT
 *  INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_INT       (0x00000020U)       /**< INT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_IER
 *  IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_IER       (0x00000080U)       /**< IER interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_TRAP
 *  TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_TRAP      (0x00000040U)       /**< TRAP interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_CLR
 * CLR register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_CLR       (IMPDRV_IMP_INTSTS_TRAP | IMPDRV_IMP_INTSTS_IER | IMPDRV_IMP_INTSTS_INT | \
                                                             IMPDRV_IMP_INTSTS_HPINT |  IMPDRV_IMP_INTSTS_APIPINT)
                                                                                    /**< CLR register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_APCMD_EX
 * EX bit in APCMD Register.
***********************************************************************************************************************/
#define IMPDRV_IMP_APCMD_EX         (0x80000000U)       /**< EX bit in APCMD Register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_APCMD_ACSCNT_SA
 * ACSCNT_SA bit in APCMD Register.
***********************************************************************************************************************/
#define IMPDRV_IMP_APCMD_ACSCNT_SA  (0x00080000U)       /**< ACSCNT_SA bit in APCMD Register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_APCMD_ACSCNT_DS
 * ACSCNT_DS bit in APCMD Register.
***********************************************************************************************************************/
#define IMPDRV_IMP_APCMD_ACSCNT_DS  (0x00010000U)       /**< ACSCNT_DS bit in APCMD Register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_MEM_SIZE_BUSCHK
 *  Memory size for Bus interface check.
***********************************************************************************************************************/
#define IMPDRV_IMP_MEM_SIZE_BUSCHK      (512U)          /** Memory size for Bus interface check. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_MEM_ALIGN_BUSCHK
 *  Memory alignment for Bus interface check.
***********************************************************************************************************************/
#define IMPDRV_IMP_MEM_ALIGN_BUSCHK     (127U)          /** Memory alignment for Bus interface check. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_MEM_MAX_ADRS_BUSCHK
 *  Maximum memory address for Bus interface check.
***********************************************************************************************************************/
#define IMPDRV_IMP_MEM_MAX_ADRS_BUSCHK  (0xFFFFFFFFU)   /** Maximum memory address for Bus interface check. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_SEND_TIMEOUT_BUSCHK
 *  Timeout period for send queue of bus interface check.
***********************************************************************************************************************/
#define IMPDRV_IMP_SEND_TIMEOUT_BUSCHK  (0U)            /** Timeout period for send queue of bus interface check. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_REG_MAX_ADRS_CNFCHK
 *  End of offset address for ConfRegCheck of IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_REG_MAX_ADRS_CNFCHK  (0x0001FFFFU)   /** End of offset address for ConfRegCheck of IMP. */

/** @} */

/*======================================================================================================================
Private data types
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Types Private type definitions
 *
 * @{
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_impdrv_imp_write_info_tbl_t
 * Information to write using the register table array
***********************************************************************************************************************/
typedef struct
{
    uint32_t                  offset;           /**< Offset to get to register */
    uint32_t                  val;              /**< Write values to registers */
    bool                      read_back;        /**< Whether to read back */
    uint32_t                  chk_val;          /**< Value to read back register */
} st_impdrv_imp_write_info_tbl_t;

/** @} */

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
/******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Functions Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief           Initialization of specified core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_init_core(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           Core busy check for the Execute routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_check_core_busy(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           Soft reset for the Execute routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_soft_reset(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           SYNCC register setting for the Execute routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_set_syncc_config(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           Convert IRQ mask value for specified core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Bit mask value for Interrupt enable register.
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_impctl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           Interrupt handler for Main function.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       core_num            Core number
 * @return          Return value
 * @retval           IMPDRV_EC_OK                    Successful completion
 * @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval           IMPDRV_EC_NG_PARAM              Parameter error
 * @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_int_main_func(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Interrupt handler for DMA transfer test.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       core_num            Core number
 * @return          Return value
 * @retval           IMPDRV_EC_OK                    Successful completion
 * @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval           IMPDRV_EC_NG_PARAM              Parameter error
 * @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_int_safety_func(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Start the DMA transfer test.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_coretypectl       Control data structure for each core type
 * @param[in]       core_num            Specified core number
 * @param[in]       p_chk_resource      Pointer to the OSAL resource for bus interface check.
 * @param[in]       physical_adrs       Physical address of the forwarding source
 * @param[in]       transfer_size       Transfer size
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_dma_transfer(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource,
    const uintptr_t                 physical_adrs,
    const uint32_t                  transfer_size
);

/*******************************************************************************************************************//**
 * @brief           Write data from register table.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       write_info_tbl      Write info table
 * @param[in]       write_info_tbl_num  Number of writing info tables
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_tbl_write_reg(
    const st_impdrv_corectl_t               *const p_corectl,
    const st_impdrv_imp_write_info_tbl_t    write_info_tbl[],
    const uint32_t                          write_info_tbl_num
);

/*******************************************************************************************************************//**
 * @brief           Checking interrupt factors.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       stat_val            Status register value.
 * @param[out]      p_mask_val          Interrupt mask value.
 * @param[out]      cb_ercd             Callback error code.
 * @param[out]      p_cb_ercd_num       Number of error codes in callback.
 * @param[out]      p_is_check_intclear Interrupt clear wait execution flag.
 * @param[out]      p_state             IMP Driver state to be transitioned.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_check_inten(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
);

/*******************************************************************************************************************//**
 * @brief           Interrupt Callback.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core.
 * @param[in]       core_num            Specified core number.
 * @param[in]       cb_ercd             Callback error code.
 * @param[in]       cb_ercd_num         Number of error codes in callback.
 * @param[in]       is_check_intclear   Interrupt clear wait execution flag.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_int_cb(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_cb_ret_t     cb_ercd[IMPDRV_CB_RET_MAX],
    const uint32_t              cb_ercd_num,
    const bool                  is_check_intclear
);

/** @} */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_impctl_is_valid_core()
* CD_PD_02_04_1001
* [Covers: UD_PD_UD02_04_0001]
***********************************************************************************************************************/
bool impdrv_impctl_is_valid_core(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    bool is_valid_core = false;

    /** Compare core number to valid core number */
    if (core_num < IMPDRV_IMP_CORENUM_VAL)
    {
        is_valid_core = true;
    }

    return is_valid_core;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_is_valid_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_check_state()
* CD_PD_02_04_1002
* [Covers: UD_PD_UD02_04_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core = false;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_STATE_UNINIT != state) && (IMPDRV_STATE_INIT != state)
        && (IMPDRV_STATE_READY != state) && (IMPDRV_STATE_EXEC != state)
        && (IMPDRV_STATE_INT != state))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);

        /** When core number is valid */
        if (true == is_valid_core)
        {
            /** When core state and specified state is not match */
            if (state != p_corectl->core_state)
            {
                ercd = IMPDRV_EC_NG_SEQSTATE;
            }
        }
        else
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_check_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_set_state()
* CD_PD_02_04_1003
* [Covers: UD_PD_UD02_04_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_STATE_UNINIT != state) && (IMPDRV_STATE_INIT != state)
        && (IMPDRV_STATE_READY != state) && (IMPDRV_STATE_EXEC != state)
        && (IMPDRV_STATE_INT != state))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Update for Core state values */
        p_corectl->core_state = state;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_set_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_init_start()
* CD_PD_02_04_1004
* [Covers: UD_PD_UD02_04_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                ret = 0U;
    st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_IMP, core_num};

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Request osdep to power on core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_on_imp(&(p_coretypectl->core_ctl[core_num].device_io), core_info);
    }

    /** Read VCR register */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&(p_coretypectl->core_ctl[core_num].device_io), IMPDRV_REG_IMP_VCR, &ret);
    }

    /** Check core version (unsupported core?) */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_IMP_VCR != (ret & IMPDRV_MASK_U16BIT))
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_init_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_init_end()
* CD_PD_02_04_1005
* [Covers: UD_PD_UD02_04_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Request osdep to close the core */
        ercd = impdrv_osdep_dev_close(&(p_coretypectl->core_ctl[core_num].device_io));

        /** Initialize of safetymechanism status */
        p_coretypectl->core_ctl[core_num].is_progress_bus_if_check = false;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_init_end()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_start()
* CD_PD_02_04_1006
* [Covers: UD_PD_UD02_04_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    st_impdrv_core_info_t   core_info;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_INIT);
    }

    /** Request for power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        core_info.core_type = IMPDRV_CORE_TYPE_IMP;
        core_info.core_num  = core_num;
        ercd = impdrv_osdep_pow_on_imp(
            &(p_coretypectl->core_ctl[core_num].device_io),
            core_info
        );
    }

    /** Initialization of specified core. */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_PARAM_OFF != p_coretypectl->core_ctl[core_num].core_mem_init_enable)
        {
            ercd = impdrv_impctl_init_core(&p_coretypectl->core_ctl[core_num]);
        }
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_READY);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_stop()
* CD_PD_02_04_1007
* [Covers: UD_PD_UD02_04_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
    }

    /** Release for Power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off_imp(&(p_coretypectl->core_ctl[core_num].device_io));
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_INIT);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_stop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_quit()
* CD_PD_02_04_1024
* [Covers: UD_PD_UD02_04_0024]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_quit(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    st_impdrv_core_info_t       core_info;
    st_impdrv_device_handle_t   device_io;

    /** Check core number */
    is_valid_core = impdrv_impctl_is_valid_core(core_num);
    if (true != is_valid_core)
    {
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    /** Request osdep to open the core */
    if (IMPDRV_EC_OK == ercd)
    {
        core_info.core_type = IMPDRV_CORE_TYPE_IMP;
        core_info.core_num  = core_num;
        device_io.handle = NULL;
        ercd = impdrv_osdep_dev_open_imp(&device_io, core_info);
    }

    /** Release for Power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off_imp(&device_io);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_attr_init()
* CD_PD_02_04_1008
* [Covers: UD_PD_UD02_04_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                i;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
    }

    /** Initialize core control informations(p_corectl) */
    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->core_mem_init_enable = IMPDRV_PARAM_OFF;

        for (i = 0U; i < IMPDRV_COREMAP_MAXID; i++)
        {
            p_corectl->cur_core_map[i] = (uint8_t)0U;
        }

        p_corectl->claddr_phys = IMPDRV_INIT_CL_ADDR;

        p_corectl->irq_mask[IMPDRV_IRQMASK_WUPCOVF] = true;
        p_corectl->irq_mask[IMPDRV_IRQMASK_HPINT] = true;
        p_corectl->irq_mask[IMPDRV_IRQMASK_APIPINT] = true;

        for (i = 0U; i < IMPDRV_EXEPARAM_MAX; i++)
        {
            p_corectl->param[i] = 0U;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_attr_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_set_mem_init()
* CD_PD_02_04_1009
* [Covers: UD_PD_UD02_04_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PARAM_OFF != enable) && (IMPDRV_PARAM_ON != enable))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
    }

    /** Set data to core_mem_init_enable */
    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->core_mem_init_enable = enable;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_set_mem_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_set_core_map()
* CD_PD_02_04_1010
* [Covers: UD_PD_UD02_04_0010]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                i;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || (NULL == syncc_val))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Set data to cur_core_map */
    if (IMPDRV_EC_OK == ercd)
    {
        for (i = 0U;  i < IMPDRV_COREMAP_MAXID; i++)
        {
            p_corectl->cur_core_map[i] = syncc_val[i];
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_set_core_map()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_set_cl()
* CD_PD_02_04_1011
* [Covers: UD_PD_UD02_04_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U != (claddr_phys & IMPDRV_CHK_CL_MEM_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Set data to claddr_phys */
    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->claddr_phys  = claddr_phys;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_set_cl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_set_irq_mask()
* CD_PD_02_04_1012
* [Covers: UD_PD_UD02_04_0012]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                i;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || (NULL == irq_mask))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        i = 0U;
        while (i < IMPDRV_IRQMASK_MAX)
        {
            if ((false != irq_mask[i]) && (true != irq_mask[i]))
            {
                ercd = IMPDRV_EC_NG_PARAM;
                break;
            }
            i++;
        }

        if (IMPDRV_EC_OK == ercd)
        {
            /** Check core number */
            is_valid_core = impdrv_impctl_is_valid_core(core_num);
            if (true != is_valid_core)
            {
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Set data to irq_mask */
    if (IMPDRV_EC_OK == ercd)
    {
        for (i = 0U;  i < IMPDRV_IRQMASK_MAX; i++)
        {
            p_corectl->irq_mask[i]  = irq_mask[i];
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_set_irq_mask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_execute()
* CD_PD_02_04_1014
* [Covers: UD_PD_UD02_04_0014]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                inten_val;
    st_impdrv_core_info_t   core_info;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || (NULL == callback_func))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (IMPDRV_INIT_CL_ADDR == p_corectl->claddr_phys)
    {
        ercd = IMPDRV_EC_NG_ATTRIBUTE;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Check core busy */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_core_busy(p_corectl);
    }

    /** Operate the hardware register to execute on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Set callback function */
        p_corectl->cbinfo.cbfunc = callback_func;
        p_corectl->cbinfo.p_cbarg = p_callback_args;

        /** Write the Configuration Register(CNF) */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_CNF,
                                                    IMPDRV_IMP_CNF_VAL, false, 0U);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Instruction Fetch Configuration(IFCFG) */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_IFCFG,
                                            IMPDRV_IMP_IFCFG_VAL, true, IMPDRV_IMP_IFCFG_VAL);
        }

        /** Soft reset */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_impctl_soft_reset(p_corectl);
        }

        /** SYNCC register setting */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_impctl_set_syncc_config(p_corectl);
        }

        /** Release the interrupt mask */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Interrupt Status Register(INTSTS) */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTSTS,
                                                        IMPDRV_IMP_INTSTS_CLR, false, 0U);
            if (IMPDRV_EC_OK == ercd)
            {
                /** Write the Interrupt Enable Register(INTEN) for release setting */
                inten_val = impdrv_impctl_get_inten_val(p_corectl);
                if (0U == inten_val)
                {
                    ercd = IMPDRV_EC_NG_PARAM;
                }
                else
                {
                    ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTEN,
                                                                    inten_val, true, inten_val);
                }
            }
        }

        /** Set the CL physical address */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Program Start Address Register(PSA) */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_PSA,
                                            p_corectl->claddr_phys, true, p_corectl->claddr_phys);
        }

        /** CL execute on specific core */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Change core state before CL Executed */
            ercd = impdrv_impctl_set_state(p_corectl, IMPDRV_STATE_EXEC);
            if (IMPDRV_EC_OK == ercd)
            {
                /** guarantees memory synchronization between contexts */
                ercd = impdrv_osdep_memory_barrier();
            }

            /** Start CL execution on the target core */
            if (IMPDRV_EC_OK == ercd)
            {
                /** CL execute start callback */
                core_info.core_type = IMPDRV_CORE_TYPE_IMP;
                core_info.core_num  = core_num;
                (void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);

                /** Write the Instruction Fetch Control Register(IFCTL) for CL execution */
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_IFCTL,
                    (IMPDRV_IMP_IFCTL_ENDIAN | IMPDRV_IMP_IFCTL_EXE), false, 0U
                );
            }
        }

        /** Recovery processing at the CL execute error. */
        if (IMPDRV_EC_OK != ercd)
        {
            /** Interrupt mask setting */
            (void)impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTEN,
                                                    IMPDRV_IMP_INTSTS_MASKCPU, false, 0U);

            /** Restore core state */
            (void)impdrv_impctl_set_state(p_corectl, IMPDRV_STATE_READY);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_execute()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_int_handler()
* CD_PD_02_04_1015
* [Covers: UD_PD_UD02_04_0015]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((false != p_corectl->is_progress_bus_if_check)
        && (true != p_corectl->is_progress_bus_if_check))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Execution of interrupt handler */
        if (false == p_corectl->is_progress_bus_if_check)
        {
            /** Handler routine for main function */
            ercd = impdrv_impctl_int_main_func(p_corectl, core_num);
        }
        else
        {
            /** Handler routine for bus interface check */
            ercd = impdrv_impctl_int_safety_func(p_corectl, core_num);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_resume_exe()
* CD_PD_02_04_1016
* [Covers: UD_PD_UD02_04_0016]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                inten_val;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_INT);
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_set_state(p_corectl, IMPDRV_STATE_EXEC);
        if (IMPDRV_EC_OK == ercd)
        {
            /** guarantees memory synchronization between contexts */
            ercd = impdrv_osdep_memory_barrier();
        }
    }

    /** CL resume on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Write the Interrupt Status Register(INTSTS) */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTSTS, IMPDRV_IMP_INTSTS_INT, false, 0U);

        /** Write the Interrupt Enable Register(INTEN) */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Interrupt Enable Register(INTEN) for release setting */
            inten_val = impdrv_impctl_get_inten_val(p_corectl);
            if (0U == inten_val)
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }
            else
            {
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTEN, inten_val, false, 0U);
            }
        }

        /** Recovery processing at the CL resume error. */
        if (IMPDRV_EC_OK != ercd)
        {
            /** Restore core state */
            (void)impdrv_impctl_set_state(p_corectl, IMPDRV_STATE_READY);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_resume_exe()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_pm_set_policy()
* CD_PD_02_04_1017
* [Covers: UD_PD_UD02_04_0017]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PM_POLICY_CG != policy) && (IMPDRV_PM_POLICY_PG != policy) && (IMPDRV_PM_POLICY_HP != policy))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_STATE_UNINIT == p_coretypectl->core_ctl[core_num].core_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
    }

    /* Changes the Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_IMP, core_num};

        ercd = impdrv_osdep_pm_set_policy(&(p_coretypectl->core_ctl[core_num].device_io), core_info, policy);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_pm_set_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_pm_get_policy()
* CD_PD_02_04_1018
* [Covers: UD_PD_UD02_04_0018]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_policy))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_STATE_UNINIT == p_coretypectl->core_ctl[core_num].core_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
    }

    /** Get Power Management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_IMP, core_num};

        ercd = impdrv_osdep_pm_get_policy(&(p_coretypectl->core_ctl[core_num].device_io), core_info, p_policy);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_pm_get_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_pm_get_state()
* CD_PD_02_04_1013
* [Covers: UD_PD_UD02_04_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_pmstate))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_STATE_UNINIT == p_coretypectl->core_ctl[core_num].core_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
    }

    /** Get Power Management state */
    if (IMPDRV_EC_OK == ercd)
    {
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_IMP, core_num};

        ercd = impdrv_osdep_pm_get_state(&(p_coretypectl->core_ctl[core_num].device_io), core_info, p_pmstate);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_bus_if_check()
* CD_PD_02_04_1053
* [Covers: UD_PD_UD02_04_0053]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_bus_if_check(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    st_impdrv_memory_handle_t   memory_handle = { NULL };
    size_t                      memory_size;
    uint32_t                    *p_logical_adrs;
    uintptr_t                   physical_adrs;
    uint8_t                     *p_work;
    uint32_t                    count;
    uint32_t                    index;

    IMPDRV_STATIC const struct {
        uint32_t                pattern;
        size_t                  size;
    } s_test_pattern_tbl[] = {
        { 0x55555555U, (sizeof(uint32_t) * 8U) },
        { 0xAAAAAAAAU, (sizeof(uint32_t) * 8U) },
        { 0x33333333U, (sizeof(uint32_t) * 8U) },
        { 0xCCCCCCCCU, (sizeof(uint32_t) * 8U) },
        { 0x0F0F0F0FU, (sizeof(uint32_t) * 8U) },
        { 0xF0F0F0F0U, (sizeof(uint32_t) * 8U) },
        { 0xFFFFFFFFU, (sizeof(uint32_t) * 8U) },
        { 0x00000000U, (sizeof(uint32_t) * 8U) }
    };

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_chk_resource)
        || (OSAL_MEMORY_BUFFER_HANDLE_INVALID == p_chk_resource->memory_handle))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (OSAL_AXI_BUS_ID_INVALID == p_chk_resource->axi_id)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else
        {
            memory_handle.handle = p_chk_resource->memory_handle;
        }
    }

    /** Get the memory address from OSAL memory object. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_memory_get_size(&memory_handle, &memory_size);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_memory_get_logical(&memory_handle, (void**)&p_logical_adrs);
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_osdep_memory_get_physical(&memory_handle, p_chk_resource->axi_id, &physical_adrs);
            }
        }
        /** Input parameter confirmation process */
        if (IMPDRV_EC_OK == ercd)
        {
            if ((IMPDRV_IMP_MEM_SIZE_BUSCHK > memory_size)
                || (NULL == p_logical_adrs)
                || (0U != (physical_adrs & IMPDRV_IMP_MEM_ALIGN_BUSCHK))
                || (IMPDRV_IMP_MEM_MAX_ADRS_BUSCHK < physical_adrs))
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }
        }
    }

    /** Write a test pattern to the test buffer */
    if (IMPDRV_EC_OK == ercd)
    {
        count = (sizeof(s_test_pattern_tbl) / sizeof(s_test_pattern_tbl[0]));
        index = 0;
        p_work = (uint8_t*)p_logical_adrs;
        while (0U < count)
        {
            ercd = impdrv_osdep_memory_data_set(
                (uint32_t*)p_work,
                s_test_pattern_tbl[index].pattern,
                s_test_pattern_tbl[index].size
            );
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            p_work += s_test_pattern_tbl[index].size;
            index++;
            count--;
        }
    }

    /** Flush of test data written in memory buffer. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_memory_cache_flush(
            &memory_handle,
            0U,
            (IMPDRV_IMP_MEM_SIZE_BUSCHK / 2U)
        );
    }

    /** Bus interface check at using DMA transfer. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_dma_transfer(
            p_coretypectl,
            core_num,
            p_chk_resource,
            physical_adrs,
            (IMPDRV_IMP_MEM_SIZE_BUSCHK / 2U)
        );
    }

    /** Clean the memory cache for test buffer. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_memory_cache_clean(
            &memory_handle,
            (IMPDRV_IMP_MEM_SIZE_BUSCHK / 2U),
            (IMPDRV_IMP_MEM_SIZE_BUSCHK / 2U)
        );
    }

    /** Check if the result of DMA transfer as expected. */
    if (IMPDRV_EC_OK == ercd)
    {
        p_work = (uint8_t*)p_logical_adrs;
        p_work += (IMPDRV_IMP_MEM_SIZE_BUSCHK / 2U);
        ercd = impdrv_osdep_memory_compare(
            p_logical_adrs,
            (uint32_t*)p_work,
            (IMPDRV_IMP_MEM_SIZE_BUSCHK / 2U)
        );
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_bus_if_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_conf_reg_check()
* CD_PD_02_04_1054
* [Covers: UD_PD_UD02_04_0054]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_impctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    uint32_t                    index;
    uint32_t                    reg_val;

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_chk_param))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U == param_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
    }

    /** Confirmation loop of Configuration register. */
    index = 0;
    while ((IMPDRV_EC_OK == ercd) && (param_num > index))
    {
        /** Input parameter confirmation process */
        if (0U != (p_chk_param[index].offset & IMPDRV_CHK_REG_ALIGN))
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
        else if (IMPDRV_IMP_REG_MAX_ADRS_CNFCHK < p_chk_param[index].offset)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else
        {
            /** Nothing to do. */
        }

        /** Read the configuration register */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_read_reg(
                &(p_coretypectl->core_ctl[core_num].device_io),
                (uint32_t)p_chk_param[index].offset,
                &reg_val
            );
        }
        if (IMPDRV_EC_OK == ercd)
        {
            /** Configuration register check */
            if ((reg_val & p_chk_param[index].bit_mask)
                != (p_chk_param[index].data & p_chk_param[index].bit_mask))
            {
                /** Error Handring */
                ercd = IMPDRV_EC_NG_CHECKFAIL;
            }
        }

        /** Check the next register */
        index++;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_conf_reg_check()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_impctl_init_core()
* CD_PD_02_04_1019
* [Covers: UD_PD_UD02_04_0019]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_init_core(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                reg_val = 0U;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Write the Memory Functional Safety register(MFSR) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_MEM_FUNC_SA,
                                                    IMPDRV_IMP_MEM_FUNC_SA_VAL, false, 0U);
    }

    /** Write the HM Pointer register(HMPTR) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_HMPTR,
                                                    IMPDRV_IMP_HMPTR_VAL, false ,0U);
    }

    /** Write the HM Data register(HMDATA) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_HMDATA,
                                        IMPDRV_IMP_HMDATA_VAL, true, IMPDRV_IMP_HMDATA_VAL);
    }

    /** Write the HM1 Pointer register(HM1PTR) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_HM1PTR,
                                                    IMPDRV_IMP_HMPTR_VAL, false, 0U);
    }

    /** Write the HM1 Data register(HM1DATA) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_HM1DATA,
                                        IMPDRV_IMP_HMDATA_VAL, true, IMPDRV_IMP_HMDATA_VAL);
    }

    /** Readback confirm the Memory Functional Safety register(MFSR) */
    if (IMPDRV_EC_OK == ercd)
    {
        do
        {
            ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_IMP_MEM_FUNC_SA, &reg_val);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            reg_val &= IMPDRV_MASK_L8BIT;
        }
        while (0U != reg_val);
    }

    /** Readback confirm the HM Pointer register(HMPTR) */
    if (IMPDRV_EC_OK == ercd)
    {
        do
        {
            ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_IMP_HMPTR, &reg_val);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            reg_val &= IMPDRV_MASK_BIT15;
        }
        while (0U != reg_val);
    }

    /** Readback confirm the HM1 Pointer register(HM1PTR) */
    if (IMPDRV_EC_OK == ercd)
    {
        do
        {
            ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_IMP_HM1PTR, &reg_val);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            reg_val &= IMPDRV_MASK_BIT15;
        }
        while (0U != reg_val);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_init_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_check_core_busy()
* CD_PD_02_04_1039
* [Covers: UD_PD_UD02_04_0039]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_check_core_busy(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
    uint32_t                reg_val;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core busy */
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_IMP_IFCTL, &reg_val);
        if (IMPDRV_EC_OK == ercd)
        {
            /** When Check busy error, it is judged that preparation for execution is not in place */
            if (0U != (IMPDRV_IMP_IFCTL_EXE & reg_val))
            {
                ercd = IMPDRV_EC_NG_SEQSTATE;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_check_core_busy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_soft_reset()
* CD_PD_02_04_1041
* [Covers: UD_PD_UD02_04_0041]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_soft_reset(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Write the Configuration Register(CNF) for soft reset */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_CNF,
            IMPDRV_IMP_CNF_VAL | IMPDRV_IMP_CNF_SWRST, false, 0U
        );

        /** Write the Configuration Register(CNF) for reset release */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_CNF,
                                                        IMPDRV_IMP_CNF_VAL, false, 0U
            );
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_soft_reset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_set_syncc_config()
* CD_PD_02_04_1043
* [Covers: UD_PD_UD02_04_0043]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_set_syncc_config(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
    uint32_t                i;
    uint32_t                offset;
    uint32_t                syncc_val_tmp;
    uint32_t                syncc_val[IMPDRV_SYNCC_REGNUM];
    uint32_t                reg_val;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Make the Synchronization Control Register(SYNCC) values */
        for (i = 0U; IMPDRV_SYNCC_REGNUM > i; i++)
        {
            offset = i * IMPDRV_OFFSET_4BYTE;

            syncc_val_tmp = (uint32_t)(p_corectl->cur_core_map[offset]);
            reg_val = (uint32_t)(p_corectl->cur_core_map[offset + 1U]);
            syncc_val_tmp |= (reg_val << IMPDRV_SHIFT_8BIT);
            reg_val = (uint32_t)(p_corectl->cur_core_map[offset + 2U]);
            syncc_val_tmp |= (reg_val << IMPDRV_SHIFT_16BIT);
            reg_val = (uint32_t)(p_corectl->cur_core_map[offset + 3U]);
            syncc_val_tmp |= (reg_val << IMPDRV_SHIFT_24BIT);

            syncc_val[i] = syncc_val_tmp;
        }

        /** Write the Synchronization Control Register 0(SYNCCR0) */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_SYNCCR0,
                                                        syncc_val[0], true, syncc_val[0]);

        /** Write the Synchronization Control Register 0(SYNCCR1) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_SYNCCR1,
                                                        syncc_val[1], true, syncc_val[1]);
        }
        /** Write the Synchronization Control Register 0(SYNCCR2) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_SYNCCR2,
                                                        syncc_val[2], true, syncc_val[2]);
        }
        /** Write the Synchronization Control Register 0(SYNCCR3) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_SYNCCR3,
                                                        syncc_val[3], true, syncc_val[3]);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_set_syncc_config()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_get_inten_val()
* CD_PD_02_04_1025
* [Covers: UD_PD_UD02_04_0025]
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_impctl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;
    uint32_t    i;

    /** Input parameter confirmation process */
    if (NULL != p_corectl)
    {
        /** Set interrupt for IMP core by default */
        inten_val = IMPDRV_IMP_INTSTS_MASKCPU;
        inten_val |= (IMPDRV_IMP_INTSTS_TRAP | IMPDRV_IMP_INTSTS_IER | IMPDRV_IMP_INTSTS_INT);

        /** Set interrupt for IMP core */
        for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
        {
            if (((uint32_t)IMPDRV_IRQMASK_HPINT == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_IMP_INTSTS_HPINT;
            }
            if (((uint32_t)IMPDRV_IRQMASK_APIPINT == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_IMP_INTSTS_APIPINT;
            }
        }
    }

    /** Set data in return value */
    return inten_val;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_get_inten_val()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_int_main_func()
* CD_PD_02_04_1036
* [Covers: UD_PD_UD02_04_0036]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_int_main_func(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
    bool                    param_chk = false;
    uint32_t                i;
    bool                    is_valid_core;
    uint32_t                intsts_val;
    uint32_t                inten_val;
    uint32_t                mask_val = 0U;
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX];
    uint32_t                cb_ercd_num = 0U;
    e_impdrv_state_t        state = IMPDRV_STATE_EXEC;
    bool                    is_check_intclear = true;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_corectl->cbinfo.cbfunc)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else
        {
            param_chk = true;
            for (i = 0U; IMPDRV_CB_RET_MAX > i; i++)
            {
                /** Initialize of array  */
                cb_ercd[i] = IMPDRV_CB_RET_OK;
            }
        }
    }

    /** Read the Interrupt Status Register(INTSTS) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTSTS, &intsts_val);
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_EXEC);
    }

    /** Call back occurrence of an interrupt to the CL execute requester */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_inten(intsts_val,
                                            &mask_val, cb_ercd, &cb_ercd_num, &is_check_intclear, &state);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Set the interrupt mask */
        /** Read the Interrupt Enable Register(INTEN) */
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTEN, &inten_val);
        if (IMPDRV_EC_OK == ercd)
        {
            if ((IMPDRV_STATE_EXEC == state) && (0U != mask_val))
            {
                inten_val = inten_val & ~(mask_val);
            }
            else
            {
                inten_val = inten_val & ~(IMPDRV_IMP_INTSTS_MASKCPU);
            }

            /** Write the Interrupt Enable Register(INTEN) for mask setting */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTEN, inten_val, true, inten_val);
        }

        /** Change core state */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_impctl_set_state(p_corectl, state);
            if (IMPDRV_EC_OK == ercd)
            {
                /** guarantees memory synchronization between contexts */
                ercd = impdrv_osdep_memory_barrier();
            }
        }

        /** Interrupt result callback */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_impctl_int_cb(p_corectl, core_num, cb_ercd, cb_ercd_num, is_check_intclear);
        }
    }

    if ((IMPDRV_EC_OK != ercd) && (true == param_chk))
    {
        /** Write the Interrupt Enable Register(INTEN) for mask setting */
        (void)impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTEN,
                                                IMPDRV_IMP_INTSTS_MASKCPU, false, 0U);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_int_main_func()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_int_safety_func()
* CD_PD_02_04_1037
* [Covers: UD_PD_UD02_04_0037]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_int_safety_func(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
    bool                    param_chk = false;
    bool                    is_valid_core;
    uint32_t                intsts_val;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl)
        || (OSAL_MQ_HANDLE_INVALID == p_corectl->queue_handle.handle))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else
        {
            param_chk = true;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_impctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Read the Interrupt Status Register(INTSTS) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTSTS, &intsts_val);
        if ((IMPDRV_EC_OK == ercd)
            && (0U != (IMPDRV_IMP_INTSTS_APIPINT & intsts_val)))
        {
            ercd = impdrv_osdep_queue_send_period(
                &p_corectl->queue_handle,
                IMPDRV_IMP_SEND_TIMEOUT_BUSCHK,
                &core_num
            );
        }
        else
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Write the Interrupt Enable Register(INTEN) for mask setting */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTEN,
                                                IMPDRV_IMP_INTSTS_MASKCPU, false, 0U);
    }
    if ((IMPDRV_EC_OK != ercd) && (true == param_chk))
    {
        (void)impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_INTEN,
                                                IMPDRV_IMP_INTSTS_MASKCPU, false, 0U);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_int_safety_func()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_dma_transfer()
* CD_PD_02_04_1038
* [Covers: UD_PD_UD02_04_0038]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_dma_transfer(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource,
    const uintptr_t                 physical_adrs,
    const uint32_t                  transfer_size
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    const st_impdrv_corectl_t   *p_corectl;
    st_impdrv_queue_handle_t    queue_handle = { NULL };
    uintptr_t                   destination_adrs;
    uintptr_t                   source_adrs;
    uint32_t                    receive_buffer = 0U;
    uint32_t                    reg_val;

    /** Private static variables */
    IMPDRV_STATIC
    const st_impdrv_imp_write_info_tbl_t s_soft_reset[] =
    {
        { IMPDRV_REG_IMP_CNF, (IMPDRV_IMP_CNF_VAL | IMPDRV_IMP_CNF_SWRST), false, 0U },
        { IMPDRV_REG_IMP_CNF, (IMPDRV_IMP_CNF_VAL), false, 0U }
    };

    IMPDRV_STATIC
    const st_impdrv_imp_write_info_tbl_t s_interrupt_enable[] =
    {
        { IMPDRV_REG_IMP_INTSTS, IMPDRV_IMP_INTSTS_CLR, false, 0U },
        { IMPDRV_REG_IMP_INTEN,  (IMPDRV_IMP_INTSTS_MASKCPU | IMPDRV_IMP_INTSTS_APIPINT), false, 0U }
    };

    IMPDRV_STATIC
    const st_impdrv_imp_write_info_tbl_t s_transfer_config[] =
    {
        { IMPDRV_REG_IMP_IPFUN2,     0x00100000U,   true,   0x00100000U },
        { IMPDRV_REG_IMP_IPFORM,     0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_APMAG,      0x01010101U,   true,   0x01010101U },
        { IMPDRV_REG_IMP_APCLPX,     0x00001FFFU,   true,   0x00001FFFU },
        { IMPDRV_REG_IMP_CNST,       0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_BINTHR,     0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_BINTHR2,    0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_KNLMSK,     0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_KNLMSK2,    0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_LMCTL,      0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF02,    0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF35,    0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF68,    0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF911,   0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF1214,  0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF15,    0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF1820,  0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF2123,  0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF24,    0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF02H,   0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF35H,   0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF68H,   0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF911H,  0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF1214H, 0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF1517H, 0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF1820H, 0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF2123H, 0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_COEFF24H,   0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_PFFTASEL1,  0x76543210U,   true,   0x76543210U },
        { IMPDRV_REG_IMP_PFFTAMSK,   0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_PFFTBMSK,   0x00000000U,   true,   0x00000000U },
        { IMPDRV_REG_IMP_APSBSP,     0U         ,   true,   0U          },
        { IMPDRV_REG_IMP_APSIZE_SA,  256U       ,   true,   256U        },
        { IMPDRV_REG_IMP_APSIZE_SB,  0U         ,   true,   0U          },
        { IMPDRV_REG_IMP_APSIZE_DST, 256U       ,   true,   256U        },
        { IMPDRV_REG_IMP_APDLY,      0x00000004U,   true,   0x00000004U },
        { IMPDRV_REG_IMP_APCFG,      0x01033310U,   true,   0x01033310U },
        { IMPDRV_REG_IMP_IPFUN,      0xF2000000U,   true,   0xF2000000U },
        { IMPDRV_REG_IMP_IPFUN2,     0x60104000U,   true,   0x60104000U },
        { IMPDRV_REG_IMP_PFFTASEL1,  0x00000000U,   true,   0x00000000U }
    };

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_chk_resource))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (((osal_mq_id_t)0UL == p_chk_resource->mq_id)
        || (0 > p_chk_resource->mq_time_period)
        || (0U != (physical_adrs & IMPDRV_IMP_MEM_ALIGN_BUSCHK))
        || (IMPDRV_IMP_MEM_MAX_ADRS_BUSCHK < physical_adrs))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Bus interface check at using DMA transfer. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Create a message queue to use for interrupt waiting. */
        ercd = impdrv_osdep_queue_create(
            &queue_handle,
            p_chk_resource->mq_id
        );

        /** Perform the DMA transfer test. */
        if (IMPDRV_EC_OK == ercd)
        {
            p_coretypectl->core_ctl[core_num].is_progress_bus_if_check = true;
            p_coretypectl->core_ctl[core_num].queue_handle = queue_handle;
            p_corectl = &p_coretypectl->core_ctl[core_num];
            destination_adrs = physical_adrs;
            source_adrs = physical_adrs + transfer_size;

            /** software reset */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_impctl_tbl_write_reg(
                    p_corectl,
                    s_soft_reset,
                    (sizeof(s_soft_reset) / sizeof(s_soft_reset[0]))
                );
                if (IMPDRV_EC_OK == ercd)
                {
                    ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_IMP_CNF, &reg_val);
                }
            }

            /** Interrupt setting */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_impctl_tbl_write_reg(
                    p_corectl,
                    s_interrupt_enable,
                    (sizeof(s_interrupt_enable) / sizeof(s_interrupt_enable[0]))
                );
            }

            /** DMA configuration settings */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_impctl_tbl_write_reg(
                    p_corectl,
                    s_transfer_config,
                    (sizeof(s_transfer_config) / sizeof(s_transfer_config[0]))
                );
            }

            /** Forward address setting */
            if (IMPDRV_EC_OK == ercd)
            {
                reg_val = (uint32_t)destination_adrs;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_APDSP, reg_val, true, reg_val);
            }
            if (IMPDRV_EC_OK == ercd)
            {
                reg_val = (uint32_t)source_adrs;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_APSASP, reg_val, true, reg_val);
            }
            if (IMPDRV_EC_OK == ercd)
            {
                reg_val = (((transfer_size / 256U) << 16U) | 64U);
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_APLNG, reg_val, true, reg_val);
            }

            /** guarantees memory synchronization between contexts */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_osdep_memory_barrier();
            }

            /** Start DMA transfer */
            if (IMPDRV_EC_OK == ercd)
            {
                reg_val = (IMPDRV_IMP_APCMD_EX | IMPDRV_IMP_APCMD_ACSCNT_SA | IMPDRV_IMP_APCMD_ACSCNT_DS);
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_IMP_APCMD, reg_val, false, 0U);
            }
        }

        /** Wait for the completion of DMA transfer in the message queue. */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_queue_wait_period(
                &queue_handle,
                p_chk_resource->mq_time_period,
                &receive_buffer
            );
            if ((IMPDRV_EC_OK == ercd) && (core_num != receive_buffer))
            {
                /** Error Handring */
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }

        /** Delete a message queue to use for interrupt waiting. */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_queue_destroy(&queue_handle);
        }
        else
        {
            (void)impdrv_osdep_queue_destroy(&queue_handle);
        }

        /** Clear the safetymechanism status */
        p_coretypectl->core_ctl[core_num].is_progress_bus_if_check = false;
        if (IMPDRV_EC_OK == ercd)
        {
            /** guarantees memory synchronization between contexts */
            ercd = impdrv_osdep_memory_barrier();
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_dma_transfer()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_tbl_write_reg()
* CD_PD_02_04_1033
* [Covers: UD_PD_UD02_04_0033]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_tbl_write_reg(
    const st_impdrv_corectl_t               *const p_corectl,
    const st_impdrv_imp_write_info_tbl_t    write_info_tbl[],
    const uint32_t                          write_info_tbl_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    uint32_t                    index;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || (NULL == write_info_tbl))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Write register table */
        index = 0U;
        while (index < write_info_tbl_num)
        {
            ercd = impdrv_osdep_write_reg(
                &p_corectl->device_io,
                write_info_tbl[index].offset,
                write_info_tbl[index].val,
                write_info_tbl[index].read_back,
                write_info_tbl[index].chk_val
            );
            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            index++;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_tbl_write_reg()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_check_inten()
* CD_PD_02_04_1047
* [Covers: UD_PD_UD02_04_0047]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_check_inten(
    const uint32_t          stat_val,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if ((NULL == p_mask_val) || (NULL == cb_ercd) || (NULL == p_cb_ercd_num) || 
            (NULL == p_is_check_intclear) || (NULL == p_state))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        *p_mask_val = 0U;
        *p_cb_ercd_num = 0U;
        *p_is_check_intclear = true;
        *p_state = IMPDRV_STATE_EXEC;

        if (0U != (IMPDRV_IMP_INTSTS_HPINT & stat_val))
        {
            /** When the HPINT bit of INTSTS register is not 0. */
            *p_mask_val = *p_mask_val | IMPDRV_IMP_INTSTS_HPINT;
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_HPINT;
            (*p_cb_ercd_num)++;
        }
        if (0U != (IMPDRV_IMP_INTSTS_APIPINT & stat_val))
        {
            /** When the APIPINT bit of INTSTS register is not 0. */
            *p_mask_val = *p_mask_val | IMPDRV_IMP_INTSTS_APIPINT;
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_APIPINT;
            (*p_cb_ercd_num)++;
        }
        if (0U != (IMPDRV_IMP_INTSTS_IER & stat_val))
        {
            /** When the IER bit of INTSTS register is not 0. */
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_ILLEGAL;
            (*p_cb_ercd_num)++;
            *p_state = IMPDRV_STATE_READY;
        }
        if (0U != (IMPDRV_IMP_INTSTS_INT & stat_val))
        {
            /** When the INT bit of INTSTS register is not 0. */
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_INT;
            (*p_cb_ercd_num)++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_INT;
            }
        }
        if (0U != (IMPDRV_IMP_INTSTS_TRAP & stat_val))
        {
            /** When the TRAP bit of INTSTS register is not 0. */
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_OK;
            (*p_cb_ercd_num)++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_READY;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_check_inten()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_impctl_int_cb()
* CD_PD_02_04_1046
* [Covers: UD_PD_UD02_04_0046]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_impctl_int_cb(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_cb_ret_t     cb_ercd[IMPDRV_CB_RET_MAX],
    const uint32_t              cb_ercd_num,
    const bool                  is_check_intclear
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_core_info_t   core_info;
    bool                    is_valid_core;
    uint32_t                cb_num;

    core_info.core_type = IMPDRV_CORE_TYPE_IMP;
    core_info.core_num  = core_num;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl) || (NULL == cb_ercd))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (IMPDRV_CB_RET_MAX < cb_ercd_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_impctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Check callback num */
        if (0U == cb_ercd_num)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
        else if (true == is_check_intclear)
        {
            /** wait clear intstatus */
            ercd = impdrv_genctl_check_intclear(&core_info);
        }
        else
        {
            /** Nothing to do */
        }

        if (IMPDRV_EC_OK == ercd)
        {
            cb_num = 0;
            while (cb_num < cb_ercd_num)
            {
                (void)((p_corectl->cbinfo.cbfunc)(
                    &core_info,
                    cb_ercd[cb_num],
                    IMPDRV_CALLBACK_CODE_DEF,
                    p_corectl->cbinfo.p_cbarg
                ));
                cb_num++;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_impctl_int_cb()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/
