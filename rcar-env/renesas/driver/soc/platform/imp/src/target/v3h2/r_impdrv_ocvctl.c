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
* File Name    : r_impdrv_ocvctl.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_ocvctl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_ocvctl.c
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
 * @def IMPDRV_OCV_CORENUM_VAL
 * Maximum number of cores for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_CORENUM_VAL      (5U)                /**< Maximum number of cores for OCV. */
/*******************************************************************************************************************//**
 * @def #define IMPDRV_OCV_OFFSET_32B
 * Offset 32byte for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_OFFSET_32B       (32U)               /**< Offset 32byte for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_VCR0
 * Offset address of VCR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_VCR0         (0x0000U)           /**< Offset address of VCR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_RSTR
 * Offset address of VCR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_RSTR         (0x0008U)           /**< Offset address of RSTR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_CR
 * Offset address of VCR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_CR           (0x000CU)           /**< Offset address of CR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SR0
 * Offset address of SR0 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SR0          (0x0010U)           /**< Offset address of SR0 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SR1
 * Offset address of SR1 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SR1          (0x0014U)           /**< Offset address of SR1 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SCR1
 * Offset address of SCR1 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SCR1         (0x0018U)           /**< Offset address of SCR1 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_ICR1
 * Offset address of ICR1 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_ICR1         (0x001CU)           /**< Offset address of ICR1 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_IMR1
 * Offset address of IMR1 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_IMR1         (0x0020U)           /**< Offset address of IMR1 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_MCR0
 * Offset address of MCR0 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_MCR0         (0x0040U)           /**< Offset address of MCR0 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SR2
 * Offset address of SR2 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SR2          (0x0024U)           /**< Offset address of SR2 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SCR2
 * Offset address of SCR2 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SCR2         (0x0028U)           /**< Offset address of SCR2 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_ICR2
 * Offset address of ICR2 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_ICR2         (0x002CU)           /**< Offset address of ICR2 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_DLSAR
 * Offset address of DLSAR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_DLSAR        (0x0180U)           /**< Offset address of DLSAR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SYNCCR0
 * Offset address of SYNCCR0 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SYNCCR0      (0x04C0U)           /**< Offset address of SYNCCR0 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SYNCCR1
 * Offset address of SYNCCR1 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SYNCCR1      (0x04C4U)           /**< Offset address of SYNCCR1 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SYNCCR2
 * Offset address of SYNCCR2 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SYNCCR2      (0x04C8U)           /**< Offset address of SYNCCR2 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SYNCCR3
 * Offset address of SYNCCR3 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SYNCCR3      (0x04CCU)           /**< Offset address of SYNCCR3 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_MEMINITR
 * Offset address of MEMINITR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_MEMINITR     (0x00F8U)           /**< Offset address of MEMINITR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_VCR
 * Hardware version value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_VCR              (0x00000014U)       /**< Hardware version value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_RESET_STATUS_VAL
 * RESET_STATUS value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_RESET_STATUS_VAL (0x00F10000U)          /**< RESET_STATUS value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_WM_INIT_VAL
 * 0X001 value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_WM_INIT_VAL        (0x00F1U)           /**< Working Memory init value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_OFST_START_VAL
 * OFST_START value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_OFST_START_VAL   (0xC000U)           /**< OFST_START value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_OFST_END_VAL
 * OFST_END value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_OFST_END_VAL     (0xE000U)           /**< OFST_END value for OCV. */
/*******************************************************************************************************************//**
 * @def #define IMPDRV_OCV_1KB_VAL
 * 1KB value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_1KB_VAL          (0x80064100U)       /**< 1KB value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_MCR0_INIT_VAL
 * MCR0 value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_MCR0_INIT_VAL    (0x01000000U)       /**< MCR0 value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_CR_PS
 * PS register value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_CR_PS            (0x00000001U)       /**< PS register value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_RSTR_SWRST
 *SWRST register value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_RSTR_SWRST       (0x00000001U)       /**< SWRST register value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_RSTR_CLR
 * CLR register value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_RSTR_CLR         (0x00000000U)       /**< CLR register value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_MCR0_DCBANKSEL0
 * DCBANKSEL0 register value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_MCR0_DCBANKSEL0  (0x00040000U)       /**< DCBANKSEL0register value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_MCR0_DCBANKSEL1
 * DCBANKSEL1 register value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_MCR0_DCBANKSEL1  (0x05000000U)       /**< DCBANKSEL1 register value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_TRAP
 * TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_TRAP         (0x00000001U)       /**< TRAP interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_IER
 * IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_IER          (0x00000002U)       /**< IER interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_INT
 * INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_INT          (0x00000004U)       /**< INT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_PBCOVF
 *PBCOVF interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_PBCOVF       (0x00000008U)       /**< PBCOVF interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_SBO0ME
 * SBO0ME interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_SBO0ME       (0x00000010U)       /**< SBO0ME interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_USIER
 * USIER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_USIER        (0x00001000U)       /**< USIER interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_USINT
 * USINT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_USINT        (0x00002000U)       /**< USINT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_WUPCOVF
 * WUPCOVF interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_WUPCOVF      (0x00004000U)       /**< WUPCOVF interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_UDIPSBRK
 * UDIPSBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_UDIPSBRK     (0x10000000U)       /**< UDIPSBRK interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_UDIVSBRK
 * UDIVSBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_UDIVSBRK     (0x20000000U)       /**< UDIVSBRK interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR2_MASK
 * Shader thread mask bits of used in SR2 register.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR2_MASK         (0xFFFFFFFFU)       /**< Shader thread mask bits of used in SR2 register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SCR2_MASK
 * Shader thread mask bits of used in SCR2 register.
***********************************************************************************************************************/
#define IMPDRV_OCV_SCR2_MASK        (0xFFFFFFFFU)       /**< Shader thread mask bits of used in SCR2 register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_ICR2_MASK
 * Shader thread mask bits of used in ICR2 register.
***********************************************************************************************************************/
#define IMPDRV_OCV_ICR2_MASK        (0xFFFFFFFFU)       /**< Shader thread mask bits of used in ICR2 register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_REG_MAX_ADRS_CNFCHK
 *  End of offset address for ConfRegCheck of OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_REG_MAX_ADRS_CNFCHK  (0x0000FFFFU)   /** End of offset address for ConfRegCheck of OCV. */

/** @} */

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
e_impdrv_errorcode_t impdrv_ocvctl_init_core(
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
e_impdrv_errorcode_t impdrv_ocvctl_check_core_busy(
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
e_impdrv_errorcode_t impdrv_ocvctl_soft_reset(
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
e_impdrv_errorcode_t impdrv_ocvctl_set_syncc_config(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           IRQ status clear for the Execute routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       mask_val            IRQ status mask value
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_irq_status_clear(
    const st_impdrv_corectl_t   *const p_corectl,
    uint32_t                    mask_val
);

/*******************************************************************************************************************//**
 * @brief           Convert IRQ mask value for specified core.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @return          Bit mask value for Interrupt enable register.
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_ocvctl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           Checking interrupt factors.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       stat_sr1            SR1 register value.
 * @param[in]       stat_sr2            SR2 register value.
 * @param[out]      p_mask_val          Interrupt mask value.
 * @param[out]      cb_ercd             Callback error code.
 * @param[out]      p_cb_ercd_num       Number of error codes in callback.
 * @param[out]      p_is_check_intclear Interrupt clear wait execution flag.
 * @param[out]      p_state             IMP Driver state to be transitioned.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_check_inten(
    const uint32_t          stat_sr1,
    const uint32_t          stat_sr2,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
);

/*******************************************************************************************************************//**
 * @brief           Checking interrupt factors. The first.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       stat_sr1            SR1 register value.
 * @param[in]       stat_sr2            SR2 register value.
 * @param[out]      p_mask_val          Interrupt mask value.
 * @param[out]      cb_ercd             Callback error code.
 * @param[out]      p_cb_ercd_num       Number of error codes in callback.
 * @param[out]      p_is_check_intclear Interrupt clear wait execution flag.
 * @param[out]      p_state             IMP Driver state to be transitioned.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_check_inten_1st(
    const uint32_t          stat_sr1,
    const uint32_t          stat_sr2,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
);

/*******************************************************************************************************************//**
 * @brief           Checking interrupt factors. The second.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       stat_sr1            SR1 register value.
 * @param[out]      cb_ercd             Callback error code.
 * @param[out]      p_cb_ercd_num       Number of error codes in callback.
 * @param[out]      p_state             IMP Driver state to be transitioned.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_check_inten_2nd(
    const uint32_t          stat_sr1,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
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
e_impdrv_errorcode_t impdrv_ocvctl_int_cb(
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
* Start of function impdrv_ocvctl_is_valid_core()
* CD_PD_02_04_2001
* [Covers: UD_PD_UD02_04_0001]
***********************************************************************************************************************/
bool impdrv_ocvctl_is_valid_core(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    bool is_valid_core = false;

    /** Compare core number to valid core number */
    if (core_num < IMPDRV_OCV_CORENUM_VAL)
    {
        is_valid_core = true;
    }

    return is_valid_core;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_is_valid_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_check_state()
* CD_PD_02_04_2002
* [Covers: UD_PD_UD02_04_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_check_state(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);

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
* End of function impdrv_ocvctl_check_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_set_state()
* CD_PD_02_04_2003
* [Covers: UD_PD_UD02_04_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_set_state(
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
* End of function impdrv_ocvctl_set_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_init_start()
* CD_PD_02_04_2004
* [Covers: UD_PD_UD02_04_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                ret = 0U;
    st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_OCV, core_num};

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
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
        ercd = impdrv_osdep_read_reg(&(p_coretypectl->core_ctl[core_num].device_io), IMPDRV_REG_OCV_VCR0, &ret);
    }

    /** Check core version (unsupported core?) */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_OCV_VCR != ret)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_init_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_init_end()
* CD_PD_02_04_2005
* [Covers: UD_PD_UD02_04_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_init_end(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Request osdep to close the core */
        ercd = impdrv_osdep_dev_close(&(p_coretypectl->core_ctl[core_num].device_io));
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_init_end()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_start()
* CD_PD_02_04_2006
* [Covers: UD_PD_UD02_04_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_start(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_INIT);
    }

    /** Request for power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        core_info.core_type = IMPDRV_CORE_TYPE_OCV;
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
            ercd = impdrv_ocvctl_init_core(&p_coretypectl->core_ctl[core_num]);
        }
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_READY);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_stop()
* CD_PD_02_04_2007
* [Covers: UD_PD_UD02_04_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_stop(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
    }

    /** Release for Power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off_imp(&(p_coretypectl->core_ctl[core_num].device_io));
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_INIT);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_stop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_quit()
* CD_PD_02_04_2024
* [Covers: UD_PD_UD02_04_0024]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_quit(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;
    st_impdrv_core_info_t       core_info;
    st_impdrv_device_handle_t   device_io;

    /** Check core number */
    is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
    if (true != is_valid_core)
    {
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    /** Request osdep to open the core */
    if (IMPDRV_EC_OK == ercd)
    {
        core_info.core_type = IMPDRV_CORE_TYPE_OCV;
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
* End of function impdrv_ocvctl_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_attr_init()
* CD_PD_02_04_2008
* [Covers: UD_PD_UD02_04_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_attr_init(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
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

        p_corectl->irq_mask[IMPDRV_IRQMASK_USIER] = true;
        p_corectl->irq_mask[IMPDRV_IRQMASK_USINT] = true;

        p_corectl->irq_mask[IMPDRV_IRQMASK_WUPCOVF] = true;
        p_corectl->irq_mask[IMPDRV_IRQMASK_PBCOVF] = true;
        p_corectl->irq_mask[IMPDRV_IRQMASK_SBO0ME] = true;

        for (i = 0U; i < IMPDRV_EXEPARAM_MAX; i++)
        {
            p_corectl->param[i] = 0U;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_attr_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_set_mem_init()
* CD_PD_02_04_2009
* [Covers: UD_PD_UD02_04_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_set_mem_init(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
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
* End of function impdrv_ocvctl_set_mem_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_set_core_map()
* CD_PD_02_04_2010
* [Covers: UD_PD_UD02_04_0010]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_set_core_map(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_ocvctl_set_core_map()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_set_cl()
* CD_PD_02_04_2011
* [Covers: UD_PD_UD02_04_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_set_cl(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_ocvctl_set_cl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_execute()
* CD_PD_02_04_2014
* [Covers: UD_PD_UD02_04_0014]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                all_mask_val;
    uint32_t                reg_val;
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Check core busy */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_core_busy(p_corectl);
    }

    /** Operate the hardware register to execute on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Initialization of interrupt mask variable */
        all_mask_val = IMPDRV_OCV_SR1_TRAP | IMPDRV_OCV_SR1_IER | IMPDRV_OCV_SR1_INT
                    | IMPDRV_OCV_SR1_PBCOVF | IMPDRV_OCV_SR1_SBO0ME
                    | IMPDRV_OCV_SR1_USIER | IMPDRV_OCV_SR1_USINT | IMPDRV_OCV_SR1_WUPCOVF;

        /** Set callback function */
        p_corectl->cbinfo.cbfunc = callback_func;
        p_corectl->cbinfo.p_cbarg = p_callback_args;

        /** Soft reset */
        ercd = impdrv_ocvctl_soft_reset(p_corectl);

        /** SYNCC register setting */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_ocvctl_set_syncc_config(p_corectl);
        }

        /** IRQ status clear register setting */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_ocvctl_irq_status_clear(p_corectl, all_mask_val);
        }

        /** Setting registers related to interrupt control */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Interrupt Mask Register(IMR1) */
            inten_val = impdrv_ocvctl_get_inten_val(p_corectl);
            if (0U == inten_val)
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }

            /** Write the Interrupt Control Register(ICR1) */
            if (IMPDRV_EC_OK == ercd)
            {
                reg_val = inten_val;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_ICR1,
                                                                    reg_val, true, reg_val);
            }

            /** Write the Interrupt Mask Register(IMR1) for mask release */
            if (IMPDRV_EC_OK == ercd)
            {
                /** Bit invert of enable interrupts, and mask the specified factor. */
                reg_val = all_mask_val & ~inten_val;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_IMR1,
                                                                    reg_val, false, 0U);
            }
        }

        /** Set the CL physical address */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Descriptor List Start Address Register(DLSAR) */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_DLSAR,
                                            p_corectl->claddr_phys, true, p_corectl->claddr_phys);
        }

        /** Write the Memory Control Register(MCR0) */
        if (IMPDRV_EC_OK == ercd)
        {
            reg_val = (IMPDRV_OCV_MCR0_DCBANKSEL1 | IMPDRV_OCV_MCR0_DCBANKSEL0);
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_MCR0,
                                                                    reg_val, true, reg_val);
        }

        /** CL execute on specific core */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Change core state before CL Executed */
            ercd = impdrv_ocvctl_set_state(p_corectl, IMPDRV_STATE_EXEC);
            if (IMPDRV_EC_OK == ercd)
            {
                /** guarantees memory synchronization between contexts */
                ercd = impdrv_osdep_memory_barrier();
            }

            /** Start CL execution on the target core */
            if (IMPDRV_EC_OK == ercd)
            {
                /** CL execute start callback */
                core_info.core_type = IMPDRV_CORE_TYPE_OCV;
                core_info.core_num  = core_num;
                (void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);

                /** Write the Control Register(CR) for CL execution */
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_CR,
                                                            IMPDRV_OCV_CR_PS, false, 0U);
            }
        }

        /** Recovery processing at the CL execute error. */
        if (IMPDRV_EC_OK != ercd)
        {
            /** Interrupt mask setting */
            (void)impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_IMR1,
                                                            all_mask_val, false, 0U);

            /** Restore core state */
            (void)impdrv_ocvctl_set_state(p_corectl, IMPDRV_STATE_READY);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_execute()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_int_handler()
* CD_PD_02_04_2015
* [Covers: UD_PD_UD02_04_0015]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    param_chk = false;
    uint32_t                i;
    bool                    is_valid_core;
    uint32_t                reg_val;
    uint32_t                all_mask_val;
    uint32_t                mask_val = 0U;
    uint32_t                inten_val;
    uint32_t                stat_sr2;
    uint32_t                stat_sr1;
    uint32_t                stat_sr0;
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX];
    uint32_t                cb_ercd_num = 0U;
    e_impdrv_state_t        state = IMPDRV_STATE_EXEC;
    bool                    is_check_intclear = true;

    /** Initialization of interrupt mask variable */
    all_mask_val = IMPDRV_OCV_SR1_TRAP | IMPDRV_OCV_SR1_IER | IMPDRV_OCV_SR1_INT
                    | IMPDRV_OCV_SR1_PBCOVF | IMPDRV_OCV_SR1_SBO0ME
                    | IMPDRV_OCV_SR1_USIER | IMPDRV_OCV_SR1_USINT | IMPDRV_OCV_SR1_WUPCOVF;

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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
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

    /** Read the Status Register(SR1) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SR1, &stat_sr1);
    }

    /** Read the Status Register(SR2) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SR2, &stat_sr2);
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(p_corectl, core_num, IMPDRV_STATE_EXEC);
    }

    /** Call back occurrence of an interrupt to the CL execute requester */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_inten(stat_sr1, stat_sr2,
                                            &mask_val, cb_ercd, &cb_ercd_num, &is_check_intclear, &state);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Set the interrupt mask */
        if ((IMPDRV_STATE_EXEC == state) && (0U != mask_val))
        {
            inten_val = impdrv_ocvctl_get_inten_val(p_corectl);
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
            /** Write the Interrupt Mask Register(IMR1) */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_IMR1, reg_val, true, reg_val);
        }
    }

    /** Read the Status Register(SR0) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SR0, &stat_sr0);
    }

    /** Change core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_set_state(p_corectl, state);
        if (IMPDRV_EC_OK == ercd)
        {
            /** guarantees memory synchronization between contexts */
            ercd = impdrv_osdep_memory_barrier();
        }
    }

    /** Interrupt result callback */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_int_cb(p_corectl, core_num, cb_ercd, cb_ercd_num, is_check_intclear, stat_sr0);
    }

    if ((IMPDRV_EC_OK != ercd) && (true == param_chk))
    {
        /** Write the Interrupt Mask Register(IMR1) */
        (void)impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_IMR1, all_mask_val, false, 0U);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_resume_exe()
* CD_PD_02_04_2016
* [Covers: UD_PD_UD02_04_0016]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                stat_sr2;
    uint32_t                stat_sr1;
    uint32_t                ctrl_icr1;
    uint32_t                all_mask_val;
    uint32_t                reg_val = 0U;
    uint32_t                inten_val;

    /** Initialization of interrupt mask variable */
    all_mask_val = IMPDRV_OCV_SR1_TRAP | IMPDRV_OCV_SR1_IER | IMPDRV_OCV_SR1_INT
                    | IMPDRV_OCV_SR1_PBCOVF | IMPDRV_OCV_SR1_SBO0ME
                    | IMPDRV_OCV_SR1_USIER | IMPDRV_OCV_SR1_USINT | IMPDRV_OCV_SR1_WUPCOVF;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(p_corectl, core_num, IMPDRV_STATE_INT);
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_set_state(p_corectl, IMPDRV_STATE_EXEC);
        if (IMPDRV_EC_OK == ercd)
        {
            /** guarantees memory synchronization between contexts */
            ercd = impdrv_osdep_memory_barrier();
        }
    }

    /** CL resume on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Read the Status Register(SR1) */
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SR1, &stat_sr1);

        /** Read the Status Register(SR2) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SR2, &stat_sr2);
        }

        /** Read the Control Register(ICR1) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_ICR1, &ctrl_icr1);
        }

        /** Clear interrupt status of INT status */
        if ((IMPDRV_EC_OK == ercd) && (0U != (IMPDRV_OCV_SR1_INT & stat_sr1)))
        {
            /** Write the Status Clear Register(SCR1) */
            reg_val = IMPDRV_OCV_SR1_INT;
            reg_val |= (IMPDRV_OCV_SR1_PBCOVF & stat_sr1);
            reg_val |= (IMPDRV_OCV_SR1_SBO0ME & stat_sr1);
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SCR1, reg_val, false, 0U);
        }

        /** Clear interrupt status of USINT status */
        if ((IMPDRV_EC_OK == ercd) && (0U != (IMPDRV_OCV_SR1_USINT & stat_sr1)))
        {
            /** Write the Status Clear Register(SCR2) */
            reg_val = IMPDRV_OCV_SCR2_MASK & stat_sr2;
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SCR2, reg_val, false, 0U);
            if (IMPDRV_EC_OK == ercd)
            {
                /** Write the Status Clear Register(SCR1) */
                reg_val = IMPDRV_OCV_SR1_USINT;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SCR1, reg_val, false, 0U);
            }
        }

        /** Setting registers related to interrupt control */
        if (IMPDRV_EC_OK == ercd)
        {
            inten_val = impdrv_ocvctl_get_inten_val(p_corectl);
            if (0U == inten_val)
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }

            /** Write the Interrupt Control Register(ICR1) */
            if (IMPDRV_EC_OK == ercd)
            {
                reg_val = inten_val;
                reg_val |= (IMPDRV_OCV_SR1_UDIPSBRK & ctrl_icr1);
                reg_val |= (IMPDRV_OCV_SR1_UDIVSBRK & ctrl_icr1);
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_ICR1,
                                                                    reg_val, true, reg_val);
            }

            /** Write the Interrupt Mask Register(IMR1) for mask release */
            if (IMPDRV_EC_OK == ercd)
            {
                /** Bit invert of enable interrupts, and mask the specified factor. */
                reg_val = all_mask_val & ~inten_val;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_IMR1,
                                                                    reg_val, false, 0U);
            }
        }

        /** Recovery processing at the CL resume error. */
        if (IMPDRV_EC_OK != ercd)
        {
            /** Restore core state */
            (void)impdrv_ocvctl_set_state(p_corectl, IMPDRV_STATE_READY);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_resume_exe()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_pm_set_policy()
* CD_PD_02_04_2017
* [Covers: UD_PD_UD02_04_0017]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_pm_set_policy(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_OCV, core_num};

        ercd = impdrv_osdep_pm_set_policy(&(p_coretypectl->core_ctl[core_num].device_io), core_info, policy);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_pm_set_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_pm_get_policy()
* CD_PD_02_04_2018
* [Covers: UD_PD_UD02_04_0018]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_pm_get_policy(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_OCV, core_num};

        ercd = impdrv_osdep_pm_get_policy(&(p_coretypectl->core_ctl[core_num].device_io), core_info, p_policy);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_pm_get_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_pm_get_state()
* CD_PD_02_04_2013
* [Covers: UD_PD_UD02_04_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_pm_get_state(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_OCV, core_num};

        ercd = impdrv_osdep_pm_get_state(&(p_coretypectl->core_ctl[core_num].device_io), core_info, p_pmstate);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_conf_reg_check()
* CD_PD_02_04_2054
* [Covers: UD_PD_UD02_04_0054]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_conf_reg_check(
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
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
        else if (IMPDRV_OCV_REG_MAX_ADRS_CNFCHK < p_chk_param[index].offset)
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
* End of function impdrv_ocvctl_conf_reg_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_set_irq_mask()
* CD_PD_02_04_2012
* [Covers: UD_PD_UD02_04_0012]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_ocvctl_set_irq_mask(
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
            is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
            if (true != is_valid_core)
            {
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_ocvctl_set_irq_mask()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_ocvctl_init_core()
* CD_PD_02_04_2019
* [Covers: UD_PD_UD02_04_0019]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_init_core(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                offset;
    uint32_t                reg_val;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** State check the Working Memory and CPG reset */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Read the Memory Initialization Register(MEMINITR) */
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io,IMPDRV_REG_OCV_MEMINITR, &reg_val);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Check reset status */
            if (0U != (IMPDRV_OCV_RESET_STATUS_VAL & reg_val))
            {
                /** CPG reset function for IMP core */
                ercd = impdrv_osdep_reset_core(&p_corectl->device_io);
            }
        }
    }

    /** State check the Working Memory */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Read the Memory Initialization Register(MEMINITR) */
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_MEMINITR, &reg_val);
    }

    /** Initialize the Working Memory */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check reset status */
        if (0U != (IMPDRV_OCV_RESET_STATUS_VAL & reg_val))
        {
            /** nothing to do */
        }
        else
        {
            /** Write the Memory Initialization Register(MEMINITR) for Initialization Working Memory */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_MEMINITR,
                                                            IMPDRV_OCV_WM_INIT_VAL, false, 0U);
            if (IMPDRV_EC_OK == ercd)
            {
                do
                {
                    ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_MEMINITR, &reg_val);
                    if (IMPDRV_EC_OK != ercd)
                    {
                        break;
                    }
                    reg_val &= IMPDRV_OCV_RESET_STATUS_VAL;
                }
                while (0U == reg_val);
            }
        }
    }

    /** Initialize the Data Cache Memory(D-cache) */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Write the Memory Control Register(MCR0) */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_MCR0,
                                        IMPDRV_OCV_1KB_VAL, true, IMPDRV_OCV_1KB_VAL);
        if (IMPDRV_EC_OK == ercd)
        {
            offset = IMPDRV_OCV_OFST_START_VAL;
            while (offset < IMPDRV_OCV_OFST_END_VAL)
            {
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, offset, 0U, true, 0U);
                if (IMPDRV_EC_OK != ercd)
                {
                    break;
                }
                offset += IMPDRV_OCV_OFFSET_32B;
            }
        }
    }
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_MCR0,
                                        IMPDRV_OCV_MCR0_INIT_VAL, true, IMPDRV_OCV_MCR0_INIT_VAL);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_init_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_check_core_busy()
* CD_PD_02_04_2039
* [Covers: UD_PD_UD02_04_0039]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_check_core_busy(
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
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_OCV_CR, &reg_val);
        if (IMPDRV_EC_OK == ercd)
        {
            /** When Check busy error, it is judged that preparation for execution is not in place */
            if (0U != (IMPDRV_OCV_CR_PS & reg_val))
            {
                ercd = IMPDRV_EC_NG_SEQSTATE;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_check_core_busy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_soft_reset()
* CD_PD_02_04_2041
* [Covers: UD_PD_UD02_04_0041]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_soft_reset(
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
        /** Write the Reset Register(RSTR) for soft reset */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_RSTR, 
                                                IMPDRV_OCV_RSTR_SWRST, false, 0U);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_RSTR, 
                                                        IMPDRV_OCV_RSTR_CLR, false, 0U);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_soft_reset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_set_syncc_config()
* CD_PD_02_04_2043
* [Covers: UD_PD_UD02_04_0043]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_set_syncc_config(
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
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SYNCCR0,
                                                        syncc_val[0], true, syncc_val[0]);

        /** Write the Synchronization Control Register 0(SYNCCR1) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SYNCCR1,
                                                        syncc_val[1], true, syncc_val[1]);
        }
        /** Write the Synchronization Control Register 0(SYNCCR2) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SYNCCR2,
                                                        syncc_val[2], true, syncc_val[2]);
        }
        /** Write the Synchronization Control Register 0(SYNCCR3) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SYNCCR3,
                                                        syncc_val[3], true, syncc_val[3]);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_set_syncc_config()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_irq_status_clear()
* CD_PD_02_04_2049
* [Covers: UD_PD_UD02_04_0049]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_irq_status_clear(
    const st_impdrv_corectl_t   *const p_corectl,
    uint32_t                    mask_val
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
        /** Write the Status Clear Register(SCR1) */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SCR1, mask_val, false, 0U);

        /** Write the Status Clear Register(SCR2) */
        if (IMPDRV_EC_OK == ercd)
        {
            reg_val = IMPDRV_OCV_SCR2_MASK;
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_SCR2,
                                                                reg_val, false, 0U);
        }

        /** Write the Interrupt Control Register(ICR2) */
        if (IMPDRV_EC_OK == ercd)
        {
            reg_val = IMPDRV_OCV_ICR2_MASK;
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_OCV_ICR2,
                                                                reg_val, true, reg_val);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_irq_status_clear()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_get_inten_val()
* CD_PD_02_04_2025
* [Covers: UD_PD_UD02_04_0025]
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_ocvctl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;
    uint32_t    i;

    /** Input parameter confirmation process */
    if (NULL != p_corectl)
    {
        /** Sets the default interrupt mask values. */
        inten_val = (IMPDRV_OCV_SR1_TRAP | IMPDRV_OCV_SR1_IER | IMPDRV_OCV_SR1_INT);

        /** Sets the remaining interrupt mask values. */
        for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
        {
            if (((uint32_t)IMPDRV_IRQMASK_PBCOVF == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_PBCOVF;
            }
            if (((uint32_t)IMPDRV_IRQMASK_SBO0ME == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_SBO0ME;
            }
            if (((uint32_t)IMPDRV_IRQMASK_USIER == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_USIER;
            }
            if (((uint32_t)IMPDRV_IRQMASK_USINT == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_USINT;
            }
            if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_WUPCOVF;
            }
        }
    }

    /** Set data in return value */
    return inten_val;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_get_inten_val()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_check_inten()
* CD_PD_02_04_2048
* [Covers: UD_PD_UD02_04_0048]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_check_inten(
    const uint32_t          stat_sr1,
    const uint32_t          stat_sr2,
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

        ercd = impdrv_ocvctl_check_inten_1st(stat_sr1, stat_sr2, p_mask_val, cb_ercd,
                                            p_cb_ercd_num, p_is_check_intclear, p_state);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_ocvctl_check_inten_2nd(stat_sr1, cb_ercd, p_cb_ercd_num, p_state);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_check_inten()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_check_inten_1st()
* CD_PD_02_04_2050
* [Covers: UD_PD_UD02_04_0050]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_check_inten_1st(
    const uint32_t          stat_sr1,
    const uint32_t          stat_sr2,
    uint32_t                *const p_mask_val,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    bool                    *const p_is_check_intclear,
    e_impdrv_state_t        *const p_state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_cb_ret_t       l_cb_ercd[IMPDRV_CB_RET_MAX];
    uint32_t                l_cb_ercd_num = 0U;
    uint32_t                cb_ercd_sum;
    uint32_t                i;

    /** Input parameter confirmation process */
    if ((NULL == p_mask_val) || (NULL == cb_ercd) || (NULL == p_cb_ercd_num) || 
            (NULL == p_is_check_intclear) || (NULL == p_state))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        if ((0U != (IMPDRV_OCV_SR1_PBCOVF & stat_sr1)) && (0U == (IMPDRV_OCV_SR1_INT & stat_sr1)))
        {
            /** When the PBCOVF bit of SR1 register is not 0. */
            *p_mask_val = *p_mask_val | IMPDRV_OCV_SR1_PBCOVF;
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_PBCOVF;
            l_cb_ercd_num++;
        }
        if ((0U != (IMPDRV_OCV_SR1_SBO0ME & stat_sr1))
                && (0U == (IMPDRV_OCV_SR1_INT & stat_sr1)) && (0U == (IMPDRV_OCV_SR1_TRAP & stat_sr1)))
        {
            /** When the SBO0ME bit of SR1 register is not 0. */
            *p_mask_val = *p_mask_val | IMPDRV_OCV_SR1_SBO0ME;
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_SBO0ME;
            l_cb_ercd_num++;
        }
        if (0U != (IMPDRV_OCV_SR1_WUPCOVF & stat_sr1))
        {
            /** When the WUPCOVF bit of SR1 register is not 0. */
            *p_is_check_intclear = false;
            *p_mask_val = *p_mask_val | IMPDRV_OCV_SR1_WUPCOVF;
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_WUPCOVF;
            l_cb_ercd_num++;
        }
        if (0U != (IMPDRV_OCV_SR1_IER & stat_sr1))
        {
            /** When the IER bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_ILLEGAL;
            l_cb_ercd_num++;
            *p_state = IMPDRV_STATE_READY;
        }
        if (0U != (IMPDRV_OCV_SR1_USIER & stat_sr1))
        {
            /** When the USIER bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_USIER;
            l_cb_ercd_num++;
            *p_state = IMPDRV_STATE_READY;
        }
        if ((0U != (IMPDRV_OCV_SR1_USINT & stat_sr1)) && (0U != (IMPDRV_OCV_SR2_MASK & stat_sr2)))
        {
            /** When the USINT bit of SR1 register and SR2 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_USINT;
            l_cb_ercd_num++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_INT;
            }
        }
        if ((0U != (IMPDRV_OCV_SR1_PBCOVF & stat_sr1)) && (0U != (IMPDRV_OCV_SR1_INT & stat_sr1)))
        {
            /** When the INT and PBCOVF bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_INT_PBCOVF;
            l_cb_ercd_num++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_INT;
            }
        }

        /** Check array size. */
        cb_ercd_sum = *p_cb_ercd_num + l_cb_ercd_num;
        if (IMPDRV_CB_RET_MAX > cb_ercd_sum)
        {
            for (i = 0U; i < l_cb_ercd_num; i++)
            {
                cb_ercd[*p_cb_ercd_num] = l_cb_ercd[i];
                (*p_cb_ercd_num)++;
            }
        }
        else
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_check_inten_1st()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_check_inten_2nd()
* CD_PD_02_04_2051
* [Covers: UD_PD_UD02_04_0051]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_check_inten_2nd(
    const uint32_t          stat_sr1,
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX],
    uint32_t                *const p_cb_ercd_num,
    e_impdrv_state_t        *const p_state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_cb_ret_t       l_cb_ercd[IMPDRV_CB_RET_MAX];
    uint32_t                l_cb_ercd_num = 0U;
    uint32_t                cb_ercd_sum;
    uint32_t                i;

    /** Input parameter confirmation process */
    if ((NULL == cb_ercd) || (NULL == p_cb_ercd_num) || (NULL == p_state))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        if ((0U != (IMPDRV_OCV_SR1_SBO0ME & stat_sr1)) && (0U != (IMPDRV_OCV_SR1_INT & stat_sr1)))
        {
            /** When the INT and SBO0ME bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_INT_SBO0ME;
            l_cb_ercd_num++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_INT;
            }
        }
        if ((0U != (IMPDRV_OCV_SR1_INT & stat_sr1)) &&
                (0U == (IMPDRV_OCV_SR1_PBCOVF & stat_sr1)) && (0U == (IMPDRV_OCV_SR1_SBO0ME & stat_sr1)))
        {
            /** When the IER bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_INT;
            l_cb_ercd_num++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_INT;
            }
        }
        if ((0U != (IMPDRV_OCV_SR1_SBO0ME & stat_sr1)) && (0U != (IMPDRV_OCV_SR1_TRAP & stat_sr1)))
        {
            /** When the TRAP and SBO0ME bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_TRAP_SBO0ME;
            l_cb_ercd_num++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_READY;
            }
        }
        if ((0U == (IMPDRV_OCV_SR1_SBO0ME & stat_sr1)) && (0U != (IMPDRV_OCV_SR1_TRAP & stat_sr1)))
        {
            /** When the TRAP bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_OK;
            l_cb_ercd_num++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_READY;
            }
        }
        if (0U != (IMPDRV_OCV_SR1_UDIPSBRK & stat_sr1))
        {
            /** When the UDIPSBRK bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_UDIPSBRK;
            l_cb_ercd_num++;
        }
        if (0U != (IMPDRV_OCV_SR1_UDIVSBRK & stat_sr1))
        {
            /** When the UDIPSBRK bit of SR1 register is not 0. */
            l_cb_ercd[l_cb_ercd_num] = IMPDRV_CB_RET_UDIVSBRK;
            l_cb_ercd_num++;
        }

        /** Check array size. */
        cb_ercd_sum = *p_cb_ercd_num + l_cb_ercd_num;
        if (IMPDRV_CB_RET_MAX > cb_ercd_sum)
        {
            for (i = 0U; i < l_cb_ercd_num; i++)
            {
                cb_ercd[*p_cb_ercd_num] = l_cb_ercd[i];
                (*p_cb_ercd_num)++;
            }
        }
        else
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_ocvctl_check_inten_2nd()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_ocvctl_int_cb()
* CD_PD_02_04_2045
* [Covers: UD_PD_UD02_04_0045]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_ocvctl_int_cb(
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

    core_info.core_type = IMPDRV_CORE_TYPE_OCV;
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
        is_valid_core = impdrv_ocvctl_is_valid_core(core_num);
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
            cb_num = 0U;
            while (cb_num < cb_ercd_num)
            {
                switch (cb_ercd[cb_num])
                {
                    case IMPDRV_CB_RET_INT_SBO0ME:
                    case IMPDRV_CB_RET_INT_PBCOVF:
                    case IMPDRV_CB_RET_INT:
                    {
                        reg_val = (dtl_stat_val & IMPDRV_MASK_U8BIT) >> IMPDRV_SHIFT_24BIT;
                        cb_code = (int32_t)reg_val;
                        break;
                    }

                    case IMPDRV_CB_RET_TRAP_SBO0ME:
                    case IMPDRV_CB_RET_OK:
                    {
                        reg_val = (dtl_stat_val & IMPDRV_MASK_U9_16BIT) >> IMPDRV_SHIFT_16BIT;
                        cb_code = (int32_t)reg_val;
                        break;
                    }

                    default:  /** Other than INT and TRAP */
                    {
                        cb_code = IMPDRV_CALLBACK_CODE_DEF;
                        break;
                    }
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
* End of function impdrv_ocvctl_int_cb()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/
