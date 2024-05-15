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
* File Name    : r_impdrv_dmactl.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_dmactl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_dmactl.c
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
 * @def IMPDRV_DMA_CORENUM_VAL
 * Maximum number of cores for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_CORENUM_VAL          (4U)                /**< Maximum number of cores for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_CORENUM0MAIN
 * DMA Core 0 Main thread identification value.
***********************************************************************************************************************/
#define IMPDRV_DMA_CORENUM0MAIN         (0U)                /**< DMA Core 0 Main thread identification value. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_CORENUM0SUB
 * DMA Core 0 Sub thread identification value.
***********************************************************************************************************************/
#define IMPDRV_DMA_CORENUM0SUB          (1U)                /**< DMA Core 0 Sub thread identification value.  */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_CORENUM1MAIN
 * DMA Core 1 Main thread identification value.
***********************************************************************************************************************/
#define IMPDRV_DMA_CORENUM1MAIN         (2U)                /**< DMA Core 1 Main thread identification value. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_CORENUM1SUB
 * DMA Core 1 Sub thread identification value.
***********************************************************************************************************************/
#define IMPDRV_DMA_CORENUM1SUB          (3U)                /**< DMA Core 1 Sub thread identification value.  */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_VCR
 * Offset address of VCR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_VCR              (0x0000U)           /**< Offset address of VCR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SCTLR
 * Offset address of SCTLR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SCTLR            (0x0004U)           /**< Offset address of SCTLR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SR
 * Offset address of SR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SR               (0x0008U)           /**< Offset address of SR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SCR
 * Offset address of SCR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SCR              (0x000CU)           /**< Offset address of SCR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SER
 * Offset address of VCR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SER              (0x0010U)           /**< Offset address of VCR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_IMR
 * Offset address of IMR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_IMR              (0x0014U)           /**< Offset address of IMR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SCTLR1
 * Offset address of SCTLR1 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SCTLR1           (0x0018U)           /**< Offset address of SCTLR1 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_CLSAR
 * Offset address of CLSAR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_CLSAR            (0x0028U)           /**< Offset address of CLSAR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SR1
 * Offset address of SR1 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SR1              (0x0030U)           /**< Offset address of SR1 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SYNCCR0
 * Offset address of SYNCCR0 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SYNCCR0          (0x00E8U)           /**< Offset address of SYNCCR0 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SYNCCR1
 * Offset address of SYNCCR1 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SYNCCR1          (0x00ECU)           /**< Offset address of SYNCCR1 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SYNCCR2
 * Offset address of SYNCCR2 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SYNCCR2          (0x00F0U)           /**< Offset address of SYNCCR2 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SYNCCR3
 * Offset address of SYNCCR3 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SYNCCR3          (0x00F4U)           /**< Offset address of SYNCCR3 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_VCR
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_VCR                  (0x17021420U)       /**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SCTLR_SWRST
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_SCTLR_SWRST          (0x80000000U)       /**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SCTLR_CLR
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_SCTLR_CLR            (0x00000000U)       /**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_VIMPDRV_DMA_SCTLR1_PSCR
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_SCTLR1_PS            (0x80000000U)       /**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_THREAD_OFFSET
 * H/W Register offset for DMA sub-thread.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_THREAD_OFFSET    (0x00000800U)       /**< H/W Register offset for DMA sub-thread. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_TEND
 *  SA_TEND interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_TEND              (0x00000001U)       /**<SA_TEND interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_TRAP
 * TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_TRAP              (0x00000010U)       /**< TRAP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_IER
 * IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_IER               (0x00000020U)       /**< IER interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_INT
 * INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_INT               (0x00000040U)       /**< INT interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_WUP
 * WUP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_WUP               (0x00000080U)       /**< WUP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SER_FIX
 * Status Enable Registers that are always bit 1.
***********************************************************************************************************************/
#define IMPDRV_DMA_SER_FIX              (0x0000F300U)       /**< Status Enable Registers that are always bit 1.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_RAMTSTR
 * Offset address of RAMTSTR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_RAMTSTR              (0x00D0U)           /**< Offset address of RAMTSTR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MASK
 * Bit mask of RAMTSTR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_MASK                 (0x8000U)           /**< Bit mask of RAMTSTR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_LOOPTIME_VAL
 * read count for register.
***********************************************************************************************************************/
#define IMPDRV_DMA_LOOPTIME_VAL         (8U)                /**< read count for register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MB_INFO_SIZE1
 * Mult-Bank memory init register info1.
***********************************************************************************************************************/
#define IMPDRV_DMA_MB_INFO_SIZE1         (3U)               /**< Mult-Bank memory init register info1. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MB_INFO_SIZE2
 * Mult-Bank memory init register info2.
***********************************************************************************************************************/
#define IMPDRV_DMA_MB_INFO_SIZE2         (7U)               /**< Mult-Bank memory init register info2. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MB_INFO_SIZE3
 * Mult-Bank memory init register info3.
***********************************************************************************************************************/
#define IMPDRV_DMA_MB_INFO_SIZE3         (5U)               /**< Mult-Bank memory init register info3. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MB_WRITE_NUM_ODD
 * Odd mult-Bank memory init register write number of times.
***********************************************************************************************************************/
#define IMPDRV_DMA_MB_WRITE_NUM_ODD      (8U)       /**< Odd mult-Bank memory init register write number of times. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MB_WRITE_NUM_EVEN
 * Even mult-Bank memory init register write number of times.
***********************************************************************************************************************/
#define IMPDRV_DMA_MB_WRITE_NUM_EVEN     (8U)       /**< Even mult-Bank memory init register write number of times. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_IMGSIZER
 * Offset address of IMGSIZER register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_IMGSIZER          (0x0094U)           /**< Offset address of IMGSIZER register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_IMGSIZER_VAL
 * IMGSIZER register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_IMGSIZER_VAL          (0x00800001U)       /**< IMGSIZER register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_FCR0
 * Offset address of FCR0 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_FCR0              (0x00A0U)           /**< Offset address of FCR0 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_FCR0_VAL
 * FCR0 register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_FCR0_VAL              (0x000000CCU)       /**< FCR0 register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_FCR1
 * Offset address of FCR1 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_FCR1              (0x00ACU)           /**< Offset address of FCR1 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_FCR1_VAL
 * FCR1 register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_FCR1_VAL              (0x90000000U)       /**< FCR1 register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_S0SAR
 * Offset address of S0SAR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_S0SAR             (0x0040U)           /**< Offset address of S0SAR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S0SAR_ODD_BASE_ADDR
 * S0SAR register value for DMA odd bank.
***********************************************************************************************************************/
#define IMPDRV_DMA_S0SAR_ODD_BASE_ADDR    (0xED020000U)       /**< S0SAR register value for DMA odd bank. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S0SAR_EVEN_BASE_ADDR
 * S0SAR register value for DMA even bank.
***********************************************************************************************************************/
#define IMPDRV_DMA_S0SAR_EVEN_BASE_ADDR   (0xED000000U)       /**< S0SAR register value for DMA even bank. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S0SAR_OFFSET
 * S0SAR register offset for DMA even bank.
***********************************************************************************************************************/
#define IMPDRV_DMA_S0SAR_OFFSET           (0x00040000U)       /**< S0SAR register offset for DMA even bank. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_S0STR
 * Offset address of S0STR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_S0STR             (0x0044U)           /**< Offset address of S0STR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S0STR_VAL
 * S0STR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_S0STR_VAL             (0x00000020U)       /**< S0STR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_S0DATAR
 * Offset address of S0DATAR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_S0DATAR           (0x004CU)           /**< Offset address of S0DATAR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S0DATAR_VAL
 * S0DATAR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_S0DATAR_VAL           (0x00000000U)       /**< S0DATAR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_S0CR
 * Offset address of S0CR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_S0CR              (0x0050U)           /**< Offset address of S0CR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S0CR_VAL
 * S0CR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_S0CR_VAL              (0x80000003U)       /**< S0CR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_S1SAR
 * Offset address of S1SAR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_S1SAR             (0x0060U)           /**< Offset address of S1SAR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S1SAR_VAL
 * S1SAR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_S1SAR_VAL             (0x00000000U)       /**< S1SAR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_S1STR
 * Offset address of S1STR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_S1STR             (0x0064U)           /**< Offset address of S1STR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S1STR_VAL
 * S1STR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_S1STR_VAL             (0x00000020U)       /**< S1STR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_S1DATAR
 * Offset address of S1DATAR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_S1DATAR           (0x006CU)           /**< Offset address of S1DATAR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S1DATAR_VAL
 * S1DATAR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_S1DATAR_VAL           (0x00000000U)       /**< S1DATAR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_S1CR
 * Offset address of S1CR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_S1CR              (0x0070U)           /**< Offset address of S1CR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_S1CR_VAL
 * S1CR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_S1CR_VAL              (0x00000003U)       /**< S1CR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_D0SAR
 * Offset address of D0SAR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_D0SAR             (0x0080U)           /**< Offset address of D0SAR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_D0STR
 * Offset address of D0STR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_D0STR             (0x0084U)           /**< Offset address of D0STR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_D0STR_VAL
 * D0STR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_D0STR_VAL             (0x00000020U)       /**< D0STR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_D0CR
 * Offset address of D0CR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_D0CR              (0x0090U)           /**< Offset address of D0CR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_D0CR_VAL
 * D0CR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_D0CR_VAL              (0x00000003U)       /**< D0CR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_DSWPR
 * Offset address of DSWPR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_DSWPR             (0x0038U)           /**< Offset address of DSWPR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_DSWPR_VAL
 * DSWPR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_DSWPR_VAL             (0x00000000U)       /**< DSWPR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_TSCR
 * Offset address of TSCR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_TSCR              (0x0020U)           /**< Offset address of TSCR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_TSCR_VAL
 * TSCR register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_TSCR_VAL              (0x00330003U)       /**< TSCR register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SCTLR0_MB_VAL
 * SCTLR0 register value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_SCTLR0_MB_VAL          (0x00000001U)       /**< SCTLR0 multi-bank register value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MEM_SIZE_BUSCHK
 *  Memory size for Bus interface check.
***********************************************************************************************************************/
#define IMPDRV_DMA_MEM_SIZE_BUSCHK      (512U)          /** Memory size for Bus interface check. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MEM_ALIGN_BUSCHK
 *  Memory alignment for Bus interface check.
***********************************************************************************************************************/
#define IMPDRV_DMA_MEM_ALIGN_BUSCHK     (127U)          /** Memory alignment for Bus interface check. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MEM_MAX_ADRS_BUSCHK
 *  Maximum memory address for Bus interface check.
***********************************************************************************************************************/
#define IMPDRV_DMA_MEM_MAX_ADRS_BUSCHK  (0xFFFFFFFFU)   /** Maximum memory address for Bus interface check. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SEND_TIMEOUT_BUSCHK
 *  Timeout period for send queue of bus interface check.
***********************************************************************************************************************/
#define IMPDRV_DMA_SEND_TIMEOUT_BUSCHK  (0U)            /** Timeout period for send queue of bus interface check. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_REG_MAX_ADRS_CNFCHK
 *  End of offset address for ConfRegCheck of DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_REG_MAX_ADRS_CNFCHK  (0x0000FFFFU)   /** End of offset address for ConfRegCheck of DMA. */

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
 * @struct st_impdrv_dma_write_info_t
 * Definition for write register information
***********************************************************************************************************************/
typedef struct
{
    st_impdrv_device_handle_t *p_device_handle; /**< OSAL device handle */
    uint32_t                  offset;           /**< Offset to get to register */
    uint32_t                  val;              /**< Write values to registers */
    bool                      is_sub_thread;    /**< Select for DMAC Internal thread */
    bool                      read_back;        /**< Whether to read back */
    uint32_t                  chk_val;          /**< Value to read back register */
} st_impdrv_dma_write_info_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_dma_write_info_t
 * Multi-bank memory init information
***********************************************************************************************************************/
typedef struct
{
    uint32_t                  offset;           /**< Offset to get to register */
    uint32_t                  val;              /**< Write values to registers */
    bool                      read_back;        /**< Whether to read back */
    uint32_t                  chk_val;          /**< Value to read back register */
} st_impdrv_dma_write_info_tbl_t;

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
 * @brief           Check if main thread or sub thread
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       core_num            Core number
 * @return          sub thread check result
 * @retval          true
 * @retval          false
***********************************************************************************************************************/
IMPDRV_STATIC
bool impdrv_dmactl_is_sub_thread(
    const uint32_t                  core_num
);

/*******************************************************************************************************************//**
 * @brief           Get osal core number from core number
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       core_num            Core number
 * @return          Osal core number
 * @retval          0              core number 0
 * @retval          1              core number 1
 * @retval          -1             out of range
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_dmactl_osal_core_num(
    const uint32_t                  core_num
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
e_impdrv_errorcode_t impdrv_dmactl_int_main_func(
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
 * @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval           IMPDRV_EC_NG_PARAM              Parameter error
 * @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_int_safety_func(
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
e_impdrv_errorcode_t impdrv_dmactl_dma_transfer(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_resource_t  *const p_chk_resource,
    const uintptr_t                 physical_adrs,
    const uint32_t                  transfer_size
);

/*******************************************************************************************************************//**
 * @brief           Read data from register.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_device_handle     OSAL device handle
 * @param[in]       addr                Offset for read register
 * @param[out]      p_val               Buffer for read register
 * @param[in]       is_sub_thread       Thread selection
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_read_reg(
    const st_impdrv_device_handle_t *const p_device_handle,
    const uint32_t                  offset,
    uint32_t                        *p_val,
    const bool                      is_sub_thread
);

/*******************************************************************************************************************//**
 * @brief           Write data from register.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       write_info          Write register information
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_write_reg(
    const st_impdrv_dma_write_info_t *const p_write_info
);

/*******************************************************************************************************************//**
 * @brief           Write data from register table.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       is_sub_thread       Thread selection
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
e_impdrv_errorcode_t impdrv_dmactl_tbl_write_reg(
    st_impdrv_corectl_t                     *const p_corectl,
    const bool                              is_sub_thread,
    const st_impdrv_dma_write_info_tbl_t    write_info_tbl[],
    const uint32_t                          write_info_tbl_num
);

/*******************************************************************************************************************//**
 * @brief           Initialization of specified core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_init_core(
    st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           Core busy check for the Execute routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       is_sub_thread       Thread selection
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_check_core_busy(
    const st_impdrv_corectl_t   *const p_corectl,
    const bool                  is_sub_thread
);

/*******************************************************************************************************************//**
 * @brief           Soft reset for the Execute routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       is_sub_thread       Thread selection
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_soft_reset(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
);

/*******************************************************************************************************************//**
 * @brief           SYNCC register setting for the Execute routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       is_sub_thread       Thread selection
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_set_syncc_config(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
);

/*******************************************************************************************************************//**
 * @brief           Convert IRQ mask value for specified core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Bit mask value for Interrupt enable register.
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_dmactl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           DMAC Multi-Bank memory initialization Pre-preparation.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       is_sub_thread       Thread selection
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_mb_init_pre(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
);

/*******************************************************************************************************************//**
 * @brief           DMAC Multi-Bank memory initialization.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       is_sub_thread       Thread selection
 * @param[in]       start_addr          Start address.
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_mb_init_main(
    st_impdrv_corectl_t             *const p_corectl,
    const bool                      is_sub_thread,
    const uint32_t                  start_addr
);

/*******************************************************************************************************************//**
 * @brief           CL execution Pre-preparation.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       is_sub_thread       Thread selection
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_cl_pre(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
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
e_impdrv_errorcode_t impdrv_dmactl_check_inten(
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
 * @param[in]       dtl_stat_val        Detail status value.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_int_cb(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_cb_ret_t     cb_ercd[IMPDRV_CB_RET_MAX],
    const uint32_t              cb_ercd_num,
    const bool                  is_check_intclear,
    const uint32_t              dtl_stat_val
);

/** @} */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_dmactl_is_valid_core()
* CD_PD_02_04_3001
* [Covers: UD_PD_UD02_04_0001]
***********************************************************************************************************************/
bool impdrv_dmactl_is_valid_core(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    bool is_valid_core = false;

    /** Compare core number to valid core number */
    if (core_num < IMPDRV_DMA_CORENUM_VAL)
    {
        is_valid_core = true;
    }

    /** Set valid core check in return value */
    return is_valid_core;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_is_valid_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_check_state()
* CD_PD_02_04_3002
* [Covers: UD_PD_UD02_04_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_check_state(
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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);

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
* End of function impdrv_dmactl_check_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_set_state()
* CD_PD_02_04_3003
* [Covers: UD_PD_UD02_04_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_set_state(
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
* End of function impdrv_dmactl_set_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_init_start()
* CD_PD_02_04_3004
* [Covers: UD_PD_UD02_04_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                ret = 0U;
    st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_DMAC, 0U};

    uint32_t                pair_thead_num = core_num + 1U;
    bool                    is_sub_thread = false;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check error code */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);

        /** When it is sub thread */
        if (true == is_sub_thread)
        {
            /** Set core number(core_num -1) */
            pair_thead_num = core_num -1U;
        }

        /** When there is no paired thread */
        if (NULL == p_coretypectl->core_ctl[pair_thead_num].device_io.handle)
        {
            /** Get osal core number from core number */
            core_info.core_num =  impdrv_dmactl_osal_core_num(core_num);
            if (((uint32_t)-1) != core_info.core_num)
            {
                /** Request osdep to power on core */
                ercd = impdrv_osdep_pow_on_imp(&(p_coretypectl->core_ctl[core_num].device_io), core_info);
            }
            else
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }
        }
        else
        {
            /** Update own device handle */
            p_coretypectl->core_ctl[core_num].device_io.handle =
                p_coretypectl->core_ctl[pair_thead_num].device_io.handle;
        }
    }

    /** Read VCR register */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_read_reg(
            &(p_coretypectl->core_ctl[core_num].device_io), IMPDRV_REG_DMA_VCR, &ret, is_sub_thread);
    }

    /** Check core version (unsupported core?) */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_DMA_VCR != ret)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_init_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_init_end()
* CD_PD_02_04_3005
* [Covers: UD_PD_UD02_04_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    uint32_t                pair_thead_num = core_num +1U;
    bool                    is_sub_thread = false;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check error code */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);

        /** When it is sub thread */
        if (true == is_sub_thread)
        {
            /** Set core number(core_num -1) */
            pair_thead_num = core_num - 1U;
        }

        /** Power off core */
        if (NULL == p_coretypectl->core_ctl[pair_thead_num].device_io.handle)
        {
            /** Request osdep to close the core */
            ercd = impdrv_osdep_dev_close(&(p_coretypectl->core_ctl[core_num].device_io));
        }
        else
        {
            /** Clear own device handle */
            p_coretypectl->core_ctl[core_num].device_io.handle = NULL;
        }

        /** Initialize of safetymechanism status */
        p_coretypectl->core_ctl[core_num].is_progress_bus_if_check = false;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_init_end()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_start()
* CD_PD_02_04_3006
* [Covers: UD_PD_UD02_04_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    st_impdrv_core_info_t   core_info;
    uint32_t                pair_thead_num = core_num + 1U;
    bool                    is_sub_thread;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_INIT);
    }

    /** Request for power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);
        if (true == is_sub_thread)
        {
            /** Set core number(core_num -1) */
            pair_thead_num = core_num - 1U;
        }

        /** When there is no paired thread */
        if (NULL == p_coretypectl->core_ctl[pair_thead_num].device_io.handle)
        {
            /** Get osal core number from core number */
            core_info.core_type = IMPDRV_CORE_TYPE_DMAC;
            core_info.core_num =  impdrv_dmactl_osal_core_num(core_num);
            if (((uint32_t)-1) != core_info.core_num)
            {
                /** Request osdep to power on core */
                ercd = impdrv_osdep_pow_on_imp(&(p_coretypectl->core_ctl[core_num].device_io), core_info);
            }
            else
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }

            /** Initialization of specified core. Can only be initialized on the main thread. */
            if (IMPDRV_EC_OK == ercd)
            {
                if (IMPDRV_PARAM_OFF != p_coretypectl->core_ctl[core_num].core_mem_init_enable)
                {
                    ercd = impdrv_dmactl_init_core(&p_coretypectl->core_ctl[core_num]);
                }
            }
        }
        else
        {
            /** Update own device handle */
            p_coretypectl->core_ctl[core_num].device_io.handle =
                p_coretypectl->core_ctl[pair_thead_num].device_io.handle;
        }
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_READY);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_stop()
* CD_PD_02_04_3007
* [Covers: UD_PD_UD02_04_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                pair_thead_num = core_num + 1U;
    bool                    is_sub_thread;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
    }

    /** Release for Power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);
        if (true == is_sub_thread)
        {
            /** Set core number(core_num -1) */
            pair_thead_num = core_num - 1U;
        }

        /** Power off core */
        if (NULL == p_coretypectl->core_ctl[pair_thead_num].device_io.handle)
        {
            /** Request osdep to power off core */
            ercd = impdrv_osdep_pow_off_imp(&(p_coretypectl->core_ctl[core_num].device_io));
        }
        else
        {
            /** Clear own device handle */
            p_coretypectl->core_ctl[core_num].device_io.handle = NULL;
        }
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_INIT);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_stop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_quit()
* CD_PD_02_04_3024
* [Covers: UD_PD_UD02_04_0024]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_quit(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    st_impdrv_core_info_t       core_info;
    st_impdrv_device_handle_t   device_io;

    /** Check core number */
    is_valid_core = impdrv_dmactl_is_valid_core(core_num);
    if (true != is_valid_core)
    {
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    /** Release for Power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get osal core number from core number */
        core_info.core_type = IMPDRV_CORE_TYPE_DMAC;
        core_info.core_num =  impdrv_dmactl_osal_core_num(core_num);
        if (((uint32_t)-1) != core_info.core_num)
        {
            /** Request osdep to open the core */
            device_io.handle = NULL;
            ercd = impdrv_osdep_dev_open_imp(&device_io, core_info);
        }
        else
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }

        /** Request osdep to power off core */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_pow_off_imp(&device_io);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_attr_init()
* CD_PD_02_04_3008
* [Covers: UD_PD_UD02_04_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_attr_init(
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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
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

        for (i = 0U; i < IMPDRV_EXEPARAM_MAX; i++)
        {
            p_corectl->param[i] = 0U;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_attr_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_set_mem_init()
* CD_PD_02_04_3009
* [Covers: UD_PD_UD02_04_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_set_mem_init(
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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
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
* End of function impdrv_dmactl_set_mem_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_set_core_map()
* CD_PD_02_04_3010
* [Covers: UD_PD_UD02_04_0010]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_set_core_map(
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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_dmactl_set_core_map()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_set_cl()
* CD_PD_02_04_3011
* [Covers: UD_PD_UD02_04_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_set_cl(
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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_dmactl_set_cl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_set_irq_mask()
* CD_PD_02_04_3012
* [Covers: UD_PD_UD02_04_0012]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_set_irq_mask(
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
            is_valid_core = impdrv_dmactl_is_valid_core(core_num);
            if (true != is_valid_core)
            {
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_dmactl_set_irq_mask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_execute()
* CD_PD_02_04_3014
* [Covers: UD_PD_UD02_04_0014]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    uint32_t                    all_mask_val;
    uint32_t                    inten_val;
    st_impdrv_core_info_t       core_info;
    bool                        is_sub_thread;
    st_impdrv_dma_write_info_t  write_info;

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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Initialize local variables after parameter confirm */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);

        /** Initialization of register write information */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = 0U;
        write_info.val = 0U;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        write_info.chk_val = 0U;
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check core state */
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Check core busy */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_core_busy(p_corectl, is_sub_thread);
    }

    /** Operate the hardware register to execute on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Initialization of interrupt mask variable */
        all_mask_val = IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT |
                    IMPDRV_DMA_SR_WUP | IMPDRV_DMA_SR_TEND;

        /** Set callback function */
        p_corectl->cbinfo.cbfunc = callback_func;
        p_corectl->cbinfo.p_cbarg = p_callback_args;

        /** Soft reset */
        ercd = impdrv_dmactl_soft_reset(p_corectl, is_sub_thread);

        /** SYNCC register setting */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dmactl_set_syncc_config(p_corectl, is_sub_thread);
        }

        /** Release the interrupt mask */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Status Clear Register(SCR) */
            write_info.offset = IMPDRV_REG_DMA_SCR;
            write_info.val =  all_mask_val;
            write_info.read_back = false;
            ercd = impdrv_dmactl_write_reg(&write_info);

            /** Write the Interrupt Mask Register(IMR) */
            if (IMPDRV_EC_OK == ercd)
            {
                inten_val = impdrv_dmactl_get_inten_val(p_corectl);
                if (0U == inten_val)
                {
                    ercd = IMPDRV_EC_NG_PARAM;
                }

                /** Write the Status Enable Register(SER) */
                if (IMPDRV_EC_OK == ercd)
                {
                    write_info.offset = IMPDRV_REG_DMA_SER;
                    write_info.val = inten_val | IMPDRV_DMA_SER_FIX;
                    write_info.read_back = true;
                    write_info.chk_val = write_info.val;
                    ercd = impdrv_dmactl_write_reg(&write_info);
                }

                /** Write the Interrupt Mask Register(IMR) for mask release */
                if (IMPDRV_EC_OK == ercd)
                {
                    /** Bit invert of enable interrupts, and mask the specified factor. */
                    write_info.offset = IMPDRV_REG_DMA_IMR;
                    write_info.val =  (all_mask_val & ~inten_val) | IMPDRV_DMA_SER_FIX;
                    write_info.chk_val = write_info.val;
                    ercd = impdrv_dmactl_write_reg(&write_info);
                }
            }
        }

        /** Set the CL physical address */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Command List Start Address Register(CLSAR) */
            write_info.offset = IMPDRV_REG_DMA_CLSAR;
            write_info.val =  p_corectl->claddr_phys;
            write_info.chk_val = write_info.val;
            ercd = impdrv_dmactl_write_reg(&write_info);
        }

        /** CL execute on specific core */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Change core state before CL Executed */
            ercd = impdrv_dmactl_set_state(p_corectl, IMPDRV_STATE_EXEC);
            if (IMPDRV_EC_OK == ercd)
            {
                /** guarantees memory synchronization between contexts */
                ercd = impdrv_osdep_memory_barrier();
            }

            /** Start CL execution on the target core */
            if (IMPDRV_EC_OK == ercd)
            {
                /** CL execute start callback */
                core_info.core_type = IMPDRV_CORE_TYPE_DMAC;
                core_info.core_num  = core_num;
                (void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);

                /** Write the System Control Register1(SCTLR1) for CL execution */
                write_info.offset = IMPDRV_REG_DMA_SCTLR1;
                write_info.val =  IMPDRV_DMA_SCTLR1_PS;
                write_info.read_back = false;
                ercd = impdrv_dmactl_write_reg(&write_info);
            }
        }

        /** Recovery processing at the CL execute error. */
        if (IMPDRV_EC_OK != ercd)
        {
            /** Interrupt mask setting */
            write_info.offset = IMPDRV_REG_DMA_IMR;
            write_info.val = all_mask_val | IMPDRV_DMA_SER_FIX;
            write_info.read_back = false;
            (void)impdrv_dmactl_write_reg(&write_info);

            /** Restore core state */
            (void)impdrv_dmactl_set_state(p_corectl, IMPDRV_STATE_READY);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_execute()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_int_handler()
* CD_PD_02_04_3015
* [Covers: UD_PD_UD02_04_0015]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_int_handler(
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
            ercd = impdrv_dmactl_int_main_func(p_corectl, core_num);
        }
        else
        {
            /** Handler routine for bus interface check */
            ercd = impdrv_dmactl_int_safety_func(p_corectl, core_num);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_resume_exe()
* CD_PD_02_04_3016
* [Covers: UD_PD_UD02_04_0016]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    bool                        is_sub_thread;
    st_impdrv_dma_write_info_t  write_info;
    uint32_t                    all_mask_val;
    uint32_t                    inten_val;

    /** Initialization of interrupt mask variable */
    all_mask_val = IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT |
                    IMPDRV_DMA_SR_WUP | IMPDRV_DMA_SR_TEND;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_INT);
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_set_state(p_corectl, IMPDRV_STATE_EXEC);
        if (IMPDRV_EC_OK == ercd)
        {
            /** guarantees memory synchronization between contexts */
            ercd = impdrv_osdep_memory_barrier();
        }
    }

    /** CL resume on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);

        /** Initialization of register write information */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.is_sub_thread = is_sub_thread;
        write_info.chk_val = 0U;

         /** Write the Status Clear Register(SCR) */
        write_info.offset = IMPDRV_REG_DMA_SCR;
        write_info.val =  IMPDRV_DMA_SR_INT;
        write_info.read_back = false;
        ercd = impdrv_dmactl_write_reg(&write_info);

        /** Write the Interrupt Mask Register(IMR) */
        if (IMPDRV_EC_OK == ercd)
        {
            inten_val = impdrv_dmactl_get_inten_val(p_corectl);
            if (0U == inten_val)
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }

            /** Write the Status Enable Register(SER) */
            if (IMPDRV_EC_OK == ercd)
            {
                write_info.offset = IMPDRV_REG_DMA_SER;
                write_info.val = inten_val | IMPDRV_DMA_SER_FIX;
                write_info.read_back = true;
                write_info.chk_val = write_info.val;
                ercd = impdrv_dmactl_write_reg(&write_info);
            }

            /** Write the Interrupt Mask Register(IMR) for mask release */
            if (IMPDRV_EC_OK == ercd)
            {
                /** Bit invert of enable interrupts, and mask the specified factor. */
                write_info.offset = IMPDRV_REG_DMA_IMR;
                write_info.val =  (all_mask_val & ~inten_val) | IMPDRV_DMA_SER_FIX;
                write_info.read_back = false;
                ercd = impdrv_dmactl_write_reg(&write_info);
            }
        }

        /** Recovery processing at the CL resume error. */
        if (IMPDRV_EC_OK != ercd)
        {
            /** Restore core state */
            (void)impdrv_dmactl_set_state(p_corectl, IMPDRV_STATE_READY);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_resume_exe()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_pm_set_policy()
* CD_PD_02_04_3017
* [Covers: UD_PD_UD02_04_0017]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    uint32_t                    pair_thead_num = core_num + 1U;
    bool                        is_sub_thread;
    st_impdrv_device_handle_t   device_handle;

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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
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

    if (IMPDRV_EC_OK == ercd)
    {
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_DMAC, core_num};

        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);

        /** When it is sub thread */
        if (true == is_sub_thread)
        {
            /** Set core number(core_num -1) */
            pair_thead_num = core_num -1U;
        }

        if (NULL == p_coretypectl->core_ctl[pair_thead_num].device_io.handle)
        {
            device_handle = p_coretypectl->core_ctl[core_num].device_io;
        }
        else
        {
            device_handle = p_coretypectl->core_ctl[pair_thead_num].device_io;
        }

        /** Get osal core number from core number */
        core_info.core_num =  impdrv_dmactl_osal_core_num(core_num);

        /* Changes the Power management policy */
        if (((uint32_t)-1) != core_info.core_num)
        {
            ercd = impdrv_osdep_pm_set_policy(&device_handle, core_info, policy);
        }
        else
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_pm_set_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_pm_get_policy()
* CD_PD_02_04_3018
* [Covers: UD_PD_UD02_04_0018]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    uint32_t                    pair_thead_num = core_num + 1U;
    bool                        is_sub_thread;
    st_impdrv_device_handle_t   device_handle;

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_policy))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
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
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_DMAC, core_num};

        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);

        /** When it is sub thread */
        if (true == is_sub_thread)
        {
            /** Set core number(core_num -1) */
            pair_thead_num = core_num -1U;
        }

        if (NULL == p_coretypectl->core_ctl[pair_thead_num].device_io.handle)
        {
            device_handle = p_coretypectl->core_ctl[core_num].device_io;
        }
        else
        {
            device_handle = p_coretypectl->core_ctl[pair_thead_num].device_io;
        }

        /** Get osal core number from core number */
        core_info.core_num =  impdrv_dmactl_osal_core_num(core_num);
        if (((uint32_t)-1) != core_info.core_num)
        {
            ercd = impdrv_osdep_pm_get_policy(&device_handle, core_info, p_policy);
        }
        else
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_pm_get_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_pm_get_state()
* CD_PD_02_04_3013
* [Covers: UD_PD_UD02_04_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    uint32_t                    pair_thead_num = core_num + 1U;
    bool                        is_sub_thread;
    st_impdrv_device_handle_t   device_handle;

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_pmstate))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
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
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_DMAC, core_num};

        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);

        /** When it is sub thread */
        if (true == is_sub_thread)
        {
            /** Set core number(core_num -1) */
            pair_thead_num = core_num -1U;
        }

        if (NULL == p_coretypectl->core_ctl[pair_thead_num].device_io.handle)
        {
            device_handle = p_coretypectl->core_ctl[core_num].device_io;
        }
        else
        {
            device_handle = p_coretypectl->core_ctl[pair_thead_num].device_io;
        }

        /** Get osal core number from core number */
        core_info.core_num =  impdrv_dmactl_osal_core_num(core_num);
        if (((uint32_t)-1) != core_info.core_num)
        {
            ercd = impdrv_osdep_pm_get_state(&device_handle, core_info, p_pmstate);
        }
        else
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_bus_if_check()
* CD_PD_02_04_3053
* [Covers: UD_PD_UD02_04_0053]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_bus_if_check(
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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
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
            if ((IMPDRV_DMA_MEM_SIZE_BUSCHK > memory_size)
                || (NULL == p_logical_adrs)
                || (0U != (physical_adrs & IMPDRV_DMA_MEM_ALIGN_BUSCHK))
                || (IMPDRV_DMA_MEM_MAX_ADRS_BUSCHK < physical_adrs))
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
            (IMPDRV_DMA_MEM_SIZE_BUSCHK / 2U)
        );
    }

    /** Bus interface check at using DMA transfer. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_dma_transfer(
            p_coretypectl,
            core_num,
            p_chk_resource,
            physical_adrs,
            (IMPDRV_DMA_MEM_SIZE_BUSCHK / 2U)
        );
    }

    /** Clean the memory cache for test buffer. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_memory_cache_clean(
            &memory_handle,
            (IMPDRV_DMA_MEM_SIZE_BUSCHK / 2U),
            (IMPDRV_DMA_MEM_SIZE_BUSCHK / 2U)
        );
    }

    /** Check if the result of DMA transfer as expected. */
    if (IMPDRV_EC_OK == ercd)
    {
        p_work = (uint8_t*)p_logical_adrs;
        p_work += (IMPDRV_DMA_MEM_SIZE_BUSCHK / 2U);
        ercd = impdrv_osdep_memory_compare(
            p_logical_adrs,
            (uint32_t*)p_work,
            (IMPDRV_DMA_MEM_SIZE_BUSCHK / 2U)
        );
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_bus_if_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_conf_reg_check()
* CD_PD_02_04_3054
* [Covers: UD_PD_UD02_04_0054]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_conf_reg_check(
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
    bool                        is_sub_thread = false;

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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else
        {
            /** Check if main thread or sub thread */
            is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
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
        else if (IMPDRV_DMA_REG_MAX_ADRS_CNFCHK < p_chk_param[index].offset)
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
            ercd = impdrv_dmactl_read_reg(
                &(p_coretypectl->core_ctl[core_num].device_io),
                (uint32_t)p_chk_param[index].offset,
                &reg_val,
                is_sub_thread
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
* End of function impdrv_dmactl_conf_reg_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_mb_init()
* CD_PD_02_04_3026
* [Covers: UD_PD_UD02_04_0026]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dmactl_mb_init(
    st_impdrv_corectl_t   *const p_corectl,
    const uint32_t        core_num
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_sub_thread;
    uint32_t                    write_num_cnt;
    uint32_t                    start_addr;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U != core_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_init_core(p_corectl);
    }

    /** Odd multi-bank memory init */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_cl_pre(p_corectl, is_sub_thread);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        write_num_cnt = 0U;
        start_addr = IMPDRV_DMA_S0SAR_ODD_BASE_ADDR;
        while (write_num_cnt < IMPDRV_DMA_MB_WRITE_NUM_ODD)
        {
            ercd = impdrv_dmactl_mb_init_pre(p_corectl, is_sub_thread);
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_dmactl_mb_init_main(p_corectl, is_sub_thread, start_addr);
            }
            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            start_addr += IMPDRV_DMA_S0SAR_OFFSET;
            write_num_cnt++;
        }
    }

    /** Even multi-bank memory init */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_cl_pre(p_corectl, is_sub_thread);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        start_addr = IMPDRV_DMA_S0SAR_EVEN_BASE_ADDR;
        write_num_cnt = 0U;
        while (write_num_cnt < IMPDRV_DMA_MB_WRITE_NUM_EVEN)
        {
            ercd = impdrv_dmactl_mb_init_pre(p_corectl, is_sub_thread);
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_dmactl_mb_init_main(p_corectl, is_sub_thread, start_addr);
            }
            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            start_addr += IMPDRV_DMA_S0SAR_OFFSET;
            write_num_cnt++;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_mb_init()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_dmactl_is_sub_thread()
* CD_PD_02_04_3021
* [Covers: UD_PD_UD02_04_0021]
***********************************************************************************************************************/
IMPDRV_STATIC
bool impdrv_dmactl_is_sub_thread(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    bool is_sub_thread = false;

    /** Sub thread check */
    if (1U == (core_num % 2U))
    {
        is_sub_thread = true;
    }

    /** Set sub thread check in return value */
    return is_sub_thread;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_is_sub_thread()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_osal_core_num()
* CD_PD_02_04_3022
* [Covers: UD_PD_UD02_04_0022]
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_dmactl_osal_core_num(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    uint32_t    osal_core_num;

    /** Convert osal core number */
    switch (core_num)
    {
        case IMPDRV_DMA_CORENUM0MAIN:
        case IMPDRV_DMA_CORENUM0SUB:
        {
            osal_core_num = 0U;
            break;
        }

        case IMPDRV_DMA_CORENUM1MAIN:
        case IMPDRV_DMA_CORENUM1SUB:
        {
            osal_core_num = 1U;
            break;
        }

        default:  /** out of range */
        {
            osal_core_num = ((uint32_t)-1);
            break;
        }
    }

    /** Set osal core number in return value */
    return osal_core_num;
}

/***********************************************************************************************************************
* End of function impdrv_dmactl_osal_core_num()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_read_reg()
* CD_PD_02_04_3023
* [Covers: UD_PD_UD02_04_0023]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_read_reg(
    const st_impdrv_device_handle_t *const p_device_handle,
    const uint32_t                  offset,
    uint32_t                        *p_val,
    const bool                      is_sub_thread
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                addr = offset;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == p_val))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (IMPDRV_REG_DMA_THREAD_OFFSET <= offset)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (0U != (offset & IMPDRV_CHK_MEM_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Read register */
    if (IMPDRV_EC_OK == ercd)
    {
        if (true == is_sub_thread)
        {
            /** When sub thread, Change register offset address */
            addr += IMPDRV_REG_DMA_THREAD_OFFSET;
        }
        ercd = impdrv_osdep_read_reg(p_device_handle, addr, p_val);
    }

    /** Set error code in return value */
    return ercd;
}

/***********************************************************************************************************************
* End of function impdrv_dmactl_read_reg()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_write_reg()
* CD_PD_02_04_3024
* [Covers: UD_PD_UD02_04_0027]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_write_reg(
    const st_impdrv_dma_write_info_t *const p_write_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                addr;

    /** Input parameter confirmation process */
    if (NULL == p_write_info)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_write_info->p_device_handle)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (NULL == p_write_info->p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Input parameter confirmation process */
    if (IMPDRV_EC_OK == ercd)
    {
        if ((IMPDRV_REG_DMA_THREAD_OFFSET <= p_write_info->offset) ||
                    (0U != (p_write_info->offset & IMPDRV_CHK_MEM_ALIGN)))
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
        else if ((true != p_write_info->read_back) && (false != p_write_info->read_back))
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
        else
        {
            /** Nothing to do. */
        }
    }

    /** Read register */
    if (IMPDRV_EC_OK == ercd)
    {
        addr = p_write_info->offset;
        if (true == p_write_info->is_sub_thread)
        {
            /** When sub thread, Change register offset address */
            addr += IMPDRV_REG_DMA_THREAD_OFFSET;
        }
        ercd = impdrv_osdep_write_reg(p_write_info->p_device_handle, addr,
                                        p_write_info->val, p_write_info->read_back, p_write_info->chk_val);
    }

    /** Set error code in return value */
    return ercd;
}

/***********************************************************************************************************************
* End of function impdrv_dmactl_write_reg()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_tbl_write_reg()
* CD_PD_02_04_3028
* [Covers: UD_PD_UD02_04_0028]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_tbl_write_reg(
    st_impdrv_corectl_t                     *const p_corectl,
    const bool                              is_sub_thread,
    const st_impdrv_dma_write_info_tbl_t    write_info_tbl[],
    const uint32_t                          write_info_tbl_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    st_impdrv_dma_write_info_t  write_info;
    uint32_t                    mb_info_cnt;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Initialize local variables after parameter confirm */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Initialization of register write information */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = 0U;
        write_info.val = 0U;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        write_info.chk_val = 0U;
    }

    /** Write register table */
    if (IMPDRV_EC_OK == ercd)
    {
        mb_info_cnt = 0U;
        while (mb_info_cnt < write_info_tbl_num)
        {
            write_info.offset = write_info_tbl[mb_info_cnt].offset;
            write_info.val = write_info_tbl[mb_info_cnt].val;
            write_info.read_back = write_info_tbl[mb_info_cnt].read_back;
            write_info.chk_val = write_info_tbl[mb_info_cnt].chk_val;
            ercd = impdrv_dmactl_write_reg(&write_info);

            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            mb_info_cnt++;
        }
    }

    /** Set error code in return value */
    return ercd;
}

/***********************************************************************************************************************
* End of function impdrv_dmactl_tbl_write_reg()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_init_core()
* CD_PD_02_04_3020
* [Covers: UD_PD_UD02_04_0020]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_init_core(
    st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd    = IMPDRV_EC_OK;
    uint32_t                    reg_val = 0U;
    uint32_t                    loop    = 0U;
    st_impdrv_dma_write_info_t  write_info;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Write the RAM Test Register(RAMTSTR) */
    if (IMPDRV_EC_OK == ercd)
    {
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = IMPDRV_DMA_RAMTSTR;
        write_info.val = IMPDRV_DMA_MASK;
        write_info.is_sub_thread = false;
        write_info.read_back = false;
        write_info.chk_val = 0U;
        ercd = impdrv_dmactl_write_reg(&write_info);
    }

    /** Wait the register read 8 times */
    if (IMPDRV_EC_OK == ercd)
    {
        loop = 0U;
        while (loop < IMPDRV_DMA_LOOPTIME_VAL)
        {
            ercd = impdrv_dmactl_read_reg(&p_corectl->device_io, IMPDRV_DMA_RAMTSTR,
                &reg_val, false
            );
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            loop++;
        }
    }

    /** Readback confirm the RAM Test Register(RAMTSTR) */
    if (IMPDRV_EC_OK == ercd)
    {
        do
        {
            ercd = impdrv_dmactl_read_reg(&p_corectl->device_io, IMPDRV_DMA_RAMTSTR,
                &reg_val, false
            );
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            reg_val &= IMPDRV_DMA_MASK;
        }
        while (0U != reg_val);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_init_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_check_core_busy()
* CD_PD_02_04_3040
* [Covers: UD_PD_UD02_04_0040]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_check_core_busy(
    const st_impdrv_corectl_t   *const p_corectl,
    const bool                  is_sub_thread
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
        ercd = impdrv_dmactl_read_reg(&p_corectl->device_io, IMPDRV_REG_DMA_SCTLR1, &reg_val, is_sub_thread);
        if (IMPDRV_EC_OK == ercd)
        {
            /** When Check busy error, it is judged that preparation for execution is not in place */
            if (0U != (IMPDRV_DMA_SCTLR1_PS & reg_val))
            {
                ercd = IMPDRV_EC_NG_SEQSTATE;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_check_core_busy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_soft_reset()
* CD_PD_02_04_3042
* [Covers: UD_PD_UD02_04_0042]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_soft_reset(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd    = IMPDRV_EC_OK;
    st_impdrv_dma_write_info_t  write_info;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Write the System Control Register(SCTLR) for soft reset */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = IMPDRV_REG_DMA_SCTLR;
        write_info.val = IMPDRV_DMA_SCTLR_SWRST;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        write_info.chk_val = 0U;
        ercd = impdrv_dmactl_write_reg(&write_info);
        if (IMPDRV_EC_OK == ercd)
        {
            write_info.val = IMPDRV_DMA_SCTLR_CLR;
            ercd = impdrv_dmactl_write_reg(&write_info);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_soft_reset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_set_syncc_config()
* CD_PD_02_04_3044
* [Covers: UD_PD_UD02_04_0044]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_set_syncc_config(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
    uint32_t                i;
    uint32_t                offset;
    uint32_t                syncc_val_tmp;
    uint32_t                syncc_val[IMPDRV_SYNCC_REGNUM];
    uint32_t                reg_val;

    st_impdrv_dma_write_info_t  write_info;

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
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = IMPDRV_REG_DMA_SYNCCR0;
        write_info.val = syncc_val[0];
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = true;
        write_info.chk_val = write_info.val;
        ercd = impdrv_dmactl_write_reg(&write_info);

        /** Write the Synchronization Control Register 0(SYNCCR1) */
        if (IMPDRV_EC_OK == ercd)
        {
            write_info.offset = IMPDRV_REG_DMA_SYNCCR1;
            write_info.val = syncc_val[1];
            write_info.chk_val = write_info.val;
            ercd = impdrv_dmactl_write_reg(&write_info);
        }
        /** Write the Synchronization Control Register 0(SYNCCR2) */
        if (IMPDRV_EC_OK == ercd)
        {
            write_info.offset = IMPDRV_REG_DMA_SYNCCR2;
            write_info.val = syncc_val[2];
            write_info.chk_val = write_info.val;
            ercd = impdrv_dmactl_write_reg(&write_info);
        }
        /** Write the Synchronization Control Register 0(SYNCCR3) */
        if (IMPDRV_EC_OK == ercd)
        {
            write_info.offset = IMPDRV_REG_DMA_SYNCCR3;
            write_info.val = syncc_val[3];
            write_info.chk_val = write_info.val;
            ercd = impdrv_dmactl_write_reg(&write_info);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_set_syncc_config()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_get_inten_val()
* CD_PD_02_04_3025
* [Covers: UD_PD_UD02_04_0025]
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_dmactl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;
    uint32_t    i;

    /** Input parameter confirmation process */
    if (NULL != p_corectl)
    {
        /** Set interrupt for DMAC core by default */
        inten_val = (IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT);

        /** Set interrupt for DMAC core */
        for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
        {
            if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_DMA_SR_WUP;
            }
        }
    }

    /** Set data in return value */
    return inten_val;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_get_inten_val()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_int_main_func()
* CD_PD_02_04_3036
* [Covers: UD_PD_UD02_04_0036]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_int_main_func(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        param_chk = false;
    uint32_t                    i;
    bool                        is_valid_core;
    uint32_t                    reg_val;
    uint32_t                    all_mask_val;
    uint32_t                    mask_val = 0U;
    uint32_t                    inten_val;
    uint32_t                    stat_val;
    uint32_t                    stat_sr1;
    e_impdrv_cb_ret_t           cb_ercd[IMPDRV_CB_RET_MAX];
    uint32_t                    cb_ercd_num = 0U;
    e_impdrv_state_t            state = IMPDRV_STATE_EXEC;
    bool                        is_sub_thread;
    st_impdrv_dma_write_info_t  write_info;
    bool                        is_check_intclear = true;

    /** Initialization of interrupt mask variable */
    all_mask_val = IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT |
                    IMPDRV_DMA_SR_WUP | IMPDRV_DMA_SR_TEND;

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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
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

    if (IMPDRV_EC_OK == ercd)
    {
        /** Check if main thread or sub thread */
        is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);

        /** Read the Status Register(SR) */
        ercd = impdrv_dmactl_read_reg(&p_corectl->device_io, IMPDRV_REG_DMA_SR,
            &stat_val, is_sub_thread
        );
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_EXEC);
    }

    /** Call back occurrence of an interrupt to the CL execute requester */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_inten(stat_val,
                                            &mask_val, cb_ercd, &cb_ercd_num, &is_check_intclear, &state);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Set the interrupt mask */
        if ((IMPDRV_STATE_EXEC == state) && (0U != mask_val))
        {
            inten_val = impdrv_dmactl_get_inten_val(p_corectl);
            if (0U == inten_val)
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }
            else
            {
                reg_val = (all_mask_val & ~inten_val) | mask_val;
            }
        }
        else
        {
            reg_val = all_mask_val;
        }

        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Interrupt Mask Register(IMR) for mask setting */
            write_info.p_device_handle = &p_corectl->device_io;
            write_info.offset = IMPDRV_REG_DMA_IMR;
            write_info.val = reg_val | IMPDRV_DMA_SER_FIX;
            write_info.is_sub_thread = is_sub_thread;
            write_info.read_back = true;
            write_info.chk_val = write_info.val;
            ercd = impdrv_dmactl_write_reg(&write_info);
        }

        if (IMPDRV_EC_OK == ercd)
        {
            /** Read the Status Register(SR1) */
            ercd = impdrv_dmactl_read_reg(&p_corectl->device_io, IMPDRV_REG_DMA_SR1, &stat_sr1, is_sub_thread);
        }

        /** Change core state */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dmactl_set_state(p_corectl, state);
            if (IMPDRV_EC_OK == ercd)
            {
                /** guarantees memory synchronization between contexts */
                ercd = impdrv_osdep_memory_barrier();
            }
        }

        /** Interrupt result callback */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dmactl_int_cb(p_corectl, core_num, cb_ercd, cb_ercd_num, is_check_intclear, stat_sr1);
        }
    }

    if ((IMPDRV_EC_OK != ercd) && (true == param_chk))
    {
        /** Write the Interrupt Mask Register(IMR) for mask setting */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = IMPDRV_REG_DMA_IMR;
        write_info.val = all_mask_val | IMPDRV_DMA_SER_FIX;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        write_info.chk_val = 0U;
        (void)impdrv_dmactl_write_reg(&write_info);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_int_main_func()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_int_safety_func()
* CD_PD_02_04_3037
* [Covers: UD_PD_UD02_04_0037]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_int_safety_func(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd    = IMPDRV_EC_OK;
    bool                        param_chk = false;
    bool                        is_valid_core;
    bool                        is_sub_thread;
    uint32_t                    stat_val;
    uint32_t                    all_mask_val;
    st_impdrv_dma_write_info_t  write_info;

    /** Initialization of interrupt mask variable */
    all_mask_val = IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT |
                    IMPDRV_DMA_SR_WUP | IMPDRV_DMA_SR_TEND;

    /** Input parameter confirmation process */
    if ((NULL == p_corectl)
        || (OSAL_MQ_HANDLE_INVALID == p_corectl->queue_handle.handle))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else
        {
            is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);
            param_chk = true;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Read the Status Register(SR) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_read_reg(&p_corectl->device_io, IMPDRV_REG_DMA_SR, &stat_val, is_sub_thread);
        if ((IMPDRV_EC_OK == ercd)
            && (0U != (IMPDRV_DMA_SR_TEND & stat_val)))
        {
            ercd = impdrv_osdep_queue_send_period(
                &p_corectl->queue_handle,
                IMPDRV_DMA_SEND_TIMEOUT_BUSCHK,
                &core_num
            );
        }
        else
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Write the Interrupt Mask Register(IMR) for mask setting */
    if (IMPDRV_EC_OK == ercd)
    {
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = IMPDRV_REG_DMA_IMR;
        write_info.val = all_mask_val | IMPDRV_DMA_SER_FIX;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = true;
        write_info.chk_val = write_info.val;
        ercd = impdrv_dmactl_write_reg(&write_info);
    }
    if ((IMPDRV_EC_OK != ercd) && (true == param_chk))
    {
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = IMPDRV_REG_DMA_IMR;
        write_info.val = all_mask_val | IMPDRV_DMA_SER_FIX;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        write_info.chk_val = 0U;
        (void)impdrv_dmactl_write_reg(&write_info);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_int_safety_func()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_dma_transfer()
* CD_PD_02_04_3038
* [Covers: UD_PD_UD02_04_0038]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_dma_transfer(
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
    bool                        is_sub_thread;
    st_impdrv_corectl_t         *p_corectl;
    st_impdrv_queue_handle_t    queue_handle = { NULL };
    uintptr_t                   destination_adrs;
    uintptr_t                   source_adrs;
    uint32_t                    receive_buffer = 0U;
    uint32_t                    reg_val;
    st_impdrv_dma_write_info_t  write_info;

    /** Private static variables */
    IMPDRV_STATIC
    const st_impdrv_dma_write_info_tbl_t s_soft_reset[] =
    {
        { IMPDRV_REG_DMA_SCTLR, IMPDRV_DMA_SCTLR_SWRST, false, 0U },
        { IMPDRV_REG_DMA_SCTLR, IMPDRV_DMA_SCTLR_CLR, false, 0U }
    };

    IMPDRV_STATIC
    const st_impdrv_dma_write_info_tbl_t s_interrupt_enable[] =
    {
        { IMPDRV_REG_DMA_SCR, 0x00000071U, false, 0U },
        { IMPDRV_REG_DMA_SER, (0x00000071U | IMPDRV_DMA_SER_FIX), true, (0x00000071U | IMPDRV_DMA_SER_FIX) },
        { IMPDRV_REG_DMA_IMR, (0x00000070U | IMPDRV_DMA_SER_FIX), false, 0U }
    };

    IMPDRV_STATIC
    const st_impdrv_dma_write_info_tbl_t s_transfer_config[] =
    {
        { IMPDRV_REG_DMA_FCR0,      0x000000CCU,    true,   0x000000CCU },
        { IMPDRV_REG_DMA_S0STR,     0x00000100U,    true,   0x00000100U },
        { IMPDRV_REG_DMA_S0DATAR,   0x00000000U,    true,   0x00000000U },
        { IMPDRV_REG_DMA_S0CR,      0x80000003U,    true,   0x80000003U },
        { IMPDRV_REG_DMA_S1SAR,     0x00000000U,    true,   0x00000000U },
        { IMPDRV_REG_DMA_S1STR,     0x00000000U,    true,   0x00000000U },
        { IMPDRV_REG_DMA_S1DATAR,   0x00000000U,    true,   0x00000000U },
        { IMPDRV_REG_DMA_S1CR,      0x00000003U,    true,   0x00000003U },
        { IMPDRV_REG_DMA_D0STR,     0x00000100U,    true,   0x00000100U },
        { IMPDRV_REG_DMA_D0CR,      0x00000003U,    true,   0x00000003U },
        { IMPDRV_REG_DMA_DSWPR,     0x00000000U,    true,   0x00000000U }
    };

    /** Input parameter confirmation process */
    if ((NULL == p_coretypectl) || (NULL == p_chk_resource))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (((osal_mq_id_t)0UL == p_chk_resource->mq_id)
        || (0 > p_chk_resource->mq_time_period)
        || (0U != (physical_adrs & IMPDRV_DMA_MEM_ALIGN_BUSCHK))
        || (IMPDRV_DMA_MEM_MAX_ADRS_BUSCHK < physical_adrs))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else
        {
            is_sub_thread = impdrv_dmactl_is_sub_thread(core_num);
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
                ercd = impdrv_dmactl_tbl_write_reg(
                    p_corectl,
                    is_sub_thread,
                    s_soft_reset,
                    (sizeof(s_soft_reset) / sizeof(s_soft_reset[0]))
                );
                if (IMPDRV_EC_OK == ercd)
                {
                    ercd = impdrv_dmactl_read_reg(
                        &p_corectl->device_io,
                        IMPDRV_REG_DMA_SCTLR,
                        &reg_val,
                        is_sub_thread
                    );
                }
            }

            /** Interrupt setting */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_dmactl_tbl_write_reg(
                    p_corectl,
                    is_sub_thread,
                    s_interrupt_enable,
                    (sizeof(s_interrupt_enable) / sizeof(s_interrupt_enable[0]))
                );
            }

            /** DMA configuration settings */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_dmactl_tbl_write_reg(
                    p_corectl,
                    is_sub_thread,
                    s_transfer_config,
                    (sizeof(s_transfer_config) / sizeof(s_transfer_config[0]))
                );
            }

            /** Forward address setting */
            if (IMPDRV_EC_OK == ercd)
            {
                write_info.p_device_handle = &p_corectl->device_io;
                write_info.offset = IMPDRV_REG_DMA_D0SAR;
                write_info.val = (uint32_t)destination_adrs;
                write_info.is_sub_thread = is_sub_thread;
                write_info.read_back = true;
                write_info.chk_val = write_info.val;
                ercd = impdrv_dmactl_write_reg(&write_info);
            }
            if (IMPDRV_EC_OK == ercd)
            {
                write_info.offset = IMPDRV_REG_DMA_S0SAR;
                write_info.val = (uint32_t)source_adrs;
                write_info.chk_val = write_info.val;
                ercd = impdrv_dmactl_write_reg(&write_info);
            }
            if (IMPDRV_EC_OK == ercd)
            {
                write_info.offset = IMPDRV_REG_DMA_IMGSIZER;
                write_info.val = ((transfer_size / 256U) | 0x01000000U);
                write_info.chk_val = write_info.val;
                ercd = impdrv_dmactl_write_reg(&write_info);
            }

            /** guarantees memory synchronization between contexts */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_osdep_memory_barrier();
            }

            /** Start DMA transfer */
            if (IMPDRV_EC_OK == ercd)
            {
                write_info.offset = IMPDRV_REG_DMA_SCTLR;
                write_info.val = IMPDRV_DMA_SCTLR0_MB_VAL;
                write_info.read_back = false;
                write_info.chk_val = 0U;
                ercd = impdrv_dmactl_write_reg(&write_info);
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
* End of function impdrv_dmactl_dma_transfer()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_mb_init_pre()
* CD_PD_02_04_3029
* [Covers: UD_PD_UD02_04_0029]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_mb_init_pre(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    uint32_t                    mask_val;
    st_impdrv_dma_write_info_t  write_info;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Initialize local variables after parameter confirm */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Initialization of register write information */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = 0U;
        write_info.val = 0U;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        write_info.chk_val = 0U;
    }

    /** Operate the hardware register to execute on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Initialization of interrupt mask variable */
        mask_val = IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT |
                    IMPDRV_DMA_SR_WUP | IMPDRV_DMA_SR_TEND;

        /** Write the Status Clear Register(SCR) */
        write_info.offset = IMPDRV_REG_DMA_SCR;
        write_info.val =  mask_val;
        write_info.read_back = false;
        ercd = impdrv_dmactl_write_reg(&write_info);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Status Enable Register(SER) */
            write_info.offset = IMPDRV_REG_DMA_SER;
            write_info.val =  IMPDRV_DMA_SR_TEND | IMPDRV_DMA_SER_FIX;
            write_info.read_back = true;
            write_info.chk_val = write_info.val;
            ercd = impdrv_dmactl_write_reg(&write_info);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_mb_init_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_mb_init_main()
* CD_PD_02_04_3030
* [Covers: UD_PD_UD02_04_0030]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_mb_init_main(
    st_impdrv_corectl_t             *const p_corectl,
    const bool                      is_sub_thread,
    const uint32_t                  start_addr
)
{
    /** Private static variables */
    IMPDRV_STATIC
    const st_impdrv_dma_write_info_tbl_t s_mb_init_info1[IMPDRV_DMA_MB_INFO_SIZE1] =
    {
            {IMPDRV_REG_DMA_IMGSIZER,   IMPDRV_DMA_IMGSIZER_VAL,    true,   IMPDRV_DMA_IMGSIZER_VAL   },
            {IMPDRV_REG_DMA_FCR0,       IMPDRV_DMA_FCR0_VAL,        true,   IMPDRV_DMA_FCR0_VAL       },
            {IMPDRV_REG_DMA_FCR1,       IMPDRV_DMA_FCR1_VAL,        true,   IMPDRV_DMA_FCR1_VAL       }
    };

    IMPDRV_STATIC
    const st_impdrv_dma_write_info_tbl_t s_mb_init_info2[IMPDRV_DMA_MB_INFO_SIZE2] =
    {
            {IMPDRV_REG_DMA_S0STR,      IMPDRV_DMA_S0STR_VAL,       true,   IMPDRV_DMA_S0STR_VAL      },
            {IMPDRV_REG_DMA_S0DATAR,    IMPDRV_DMA_S0DATAR_VAL,     true,   IMPDRV_DMA_S0DATAR_VAL    },
            {IMPDRV_REG_DMA_S0CR,       IMPDRV_DMA_S0CR_VAL,        true,   IMPDRV_DMA_S0CR_VAL       },
            {IMPDRV_REG_DMA_S1SAR,      IMPDRV_DMA_S1SAR_VAL,       true,   IMPDRV_DMA_S1SAR_VAL      },
            {IMPDRV_REG_DMA_S1STR,      IMPDRV_DMA_S1STR_VAL,       true,   IMPDRV_DMA_S1STR_VAL      },
            {IMPDRV_REG_DMA_S1DATAR,    IMPDRV_DMA_S1DATAR_VAL,     true,   IMPDRV_DMA_S1DATAR_VAL    },
            {IMPDRV_REG_DMA_S1CR,       IMPDRV_DMA_S1CR_VAL,        true,   IMPDRV_DMA_S1CR_VAL       }
    };

    IMPDRV_STATIC
    const st_impdrv_dma_write_info_tbl_t s_mb_init_info3[IMPDRV_DMA_MB_INFO_SIZE3] =
    {
            {IMPDRV_REG_DMA_D0STR,      IMPDRV_DMA_D0STR_VAL,       true,   IMPDRV_DMA_D0STR_VAL      },
            {IMPDRV_REG_DMA_D0CR,       IMPDRV_DMA_D0CR_VAL,        true,   IMPDRV_DMA_D0CR_VAL       },
            {IMPDRV_REG_DMA_DSWPR,      IMPDRV_DMA_DSWPR_VAL,       true,   IMPDRV_DMA_DSWPR_VAL      },
            {IMPDRV_REG_DMA_TSCR,       IMPDRV_DMA_TSCR_VAL,        true,   IMPDRV_DMA_TSCR_VAL       },
            {IMPDRV_REG_DMA_SCTLR,      IMPDRV_DMA_SCTLR0_MB_VAL,   false,  0U                        }
    };

    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    st_impdrv_dma_write_info_t  write_info;
    uint32_t                    stat_val;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Initialize local variables after parameter confirm */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Initialization of register write information */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = 0U;
        write_info.val = 0U;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        write_info.chk_val = 0U;
    }

    /** Operate the hardware register to execute on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_tbl_write_reg(p_corectl, is_sub_thread, s_mb_init_info1, IMPDRV_DMA_MB_INFO_SIZE1);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        write_info.offset = IMPDRV_REG_DMA_S0SAR;
        write_info.val = start_addr;
        write_info.read_back = true;
        write_info.chk_val = start_addr;
        ercd = impdrv_dmactl_write_reg(&write_info);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_tbl_write_reg(p_corectl, is_sub_thread, s_mb_init_info2, IMPDRV_DMA_MB_INFO_SIZE2);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        write_info.offset = IMPDRV_REG_DMA_D0SAR;
        write_info.val = start_addr;
        write_info.read_back = true;
        write_info.chk_val = start_addr;
        ercd = impdrv_dmactl_write_reg(&write_info);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dmactl_tbl_write_reg(p_corectl, is_sub_thread, s_mb_init_info3, IMPDRV_DMA_MB_INFO_SIZE3);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        do
        {
            ercd = impdrv_dmactl_read_reg(&p_corectl->device_io,  IMPDRV_REG_DMA_SR, &stat_val, is_sub_thread);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
        }
        while (0U == (IMPDRV_DMA_SR_TEND & stat_val));
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_mb_init_main()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_cl_pre()
* CD_PD_02_04_3031
* [Covers: UD_PD_UD02_04_0031]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_cl_pre(
    st_impdrv_corectl_t         *const p_corectl,
    const bool                  is_sub_thread
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    uint32_t                    reg_val;
    st_impdrv_dma_write_info_t  write_info;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Initialize local variables after parameter confirm */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Initialization of register write information */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = 0U;
        write_info.val = 0U;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        write_info.chk_val = 0U;
    }

    /** Check core busy */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check busy */
        ercd = impdrv_dmactl_read_reg(&p_corectl->device_io, IMPDRV_REG_DMA_SCTLR1, &reg_val, is_sub_thread);
        if (IMPDRV_EC_OK == ercd)
        {
            /** When Check busy error, it is judged that preparation for execution is not in place */
            if (0U != (IMPDRV_DMA_SCTLR1_PS & reg_val))
            {
                ercd = IMPDRV_EC_NG_SEQSTATE;
            }
        }
    }

    /** Operate the hardware register to execute on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Write the System Control Register(SCTLR) for soft reset */
        write_info.p_device_handle = &p_corectl->device_io;
        write_info.offset = IMPDRV_REG_DMA_SCTLR;
        write_info.val = IMPDRV_DMA_SCTLR_SWRST;
        write_info.is_sub_thread = is_sub_thread;
        write_info.read_back = false;
        ercd = impdrv_dmactl_write_reg(&write_info);
        if (IMPDRV_EC_OK == ercd)
        {
            write_info.val = IMPDRV_DMA_SCTLR_CLR;
            ercd = impdrv_dmactl_write_reg(&write_info);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dmactl_cl_pre()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_check_inten()
* CD_PD_02_04_3047
* [Covers: UD_PD_UD02_04_0047]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_check_inten(
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

        if (0U != (IMPDRV_DMA_SR_WUP & stat_val))
        {
            /** When the PBCOVF bit of SR1 register is not 0. */
            *p_is_check_intclear = false;
            *p_mask_val = *p_mask_val | IMPDRV_DMA_SR_WUP;
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_WUPCOVF;
            (*p_cb_ercd_num)++;
        }
        if (0U != (IMPDRV_DMA_SR_IER & stat_val))
        {
            /** When the IER bit of SR register is not 0. */
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_ILLEGAL;
            (*p_cb_ercd_num)++;
            *p_state = IMPDRV_STATE_READY;
        }
        if (0U != (IMPDRV_DMA_SR_INT & stat_val))
        {
            /** When the INT bit of SR register is not 0. */
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_INT;
            (*p_cb_ercd_num)++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_INT;
            }
        }
        if (0U != (IMPDRV_DMA_SR_TRAP & stat_val))
        {
            /** When the TRAP bit of SR register is not 0. */
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
* End of function impdrv_dmactl_check_inten()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dmactl_int_cb()
* CD_PD_02_04_3045
* [Covers: UD_PD_UD02_04_0045]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_dmactl_int_cb(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_cb_ret_t     cb_ercd[IMPDRV_CB_RET_MAX],
    const uint32_t              cb_ercd_num,
    const bool                  is_check_intclear,
    const uint32_t              dtl_stat_val
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_core_info_t   core_info;
    bool                    is_valid_core;
    uint32_t                cb_num;
    uint32_t                reg_val;
    int32_t                 cb_code;

    core_info.core_type = IMPDRV_CORE_TYPE_DMAC;
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
        is_valid_core = impdrv_dmactl_is_valid_core(core_num);
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
                if (IMPDRV_CB_RET_INT == cb_ercd[cb_num])
                {
                    /** set SR1 & 0xFF000000U shifted right by 24 bit in code */
                    reg_val = (dtl_stat_val & IMPDRV_MASK_U8BIT) >> IMPDRV_SHIFT_24BIT;
                    cb_code = (int32_t)reg_val;
                }
                else if (IMPDRV_CB_RET_OK == cb_ercd[cb_num])
                {
                    /** Set SR1 & 0x00FF0000U shifted right by 16 bit in code */
                    reg_val = (dtl_stat_val & IMPDRV_MASK_U9_16BIT) >> IMPDRV_SHIFT_16BIT;
                    cb_code = (int32_t)reg_val;
                }
                else
                {
                    cb_code = IMPDRV_CALLBACK_CODE_DEF;
                }

                (void)((p_corectl->cbinfo.cbfunc)(
                    &core_info,
                    cb_ercd[cb_num],
                    cb_code,
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
* End of function impdrv_dmactl_int_cb()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/
