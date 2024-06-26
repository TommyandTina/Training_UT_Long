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
* File Name    : r_impdrv_pscctl.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_pscctl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_pscctl.c
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
 * @def IMPDRV_PSC_CORENUM_VAL
 * Maximum number of cores for PSC.
***********************************************************************************************************************/
#define IMPDRV_PSC_CORENUM_VAL      (1U)                /**< Maximum number of cores for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_EXEPARAM_CLBRK
 * Index value of extend parameter for CL break address.
***********************************************************************************************************************/
#define IMPDRV_PSC_EXEPARAM_CLBRK   (0U)                /**< Index value of extend parameter for CL break address.  */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_EXEPARAM_GOSUB
 * Index value of extend parameter for conditional GOSUB instruction.
***********************************************************************************************************************/
#define IMPDRV_PSC_EXEPARAM_GOSUB   (1U)                /**< Index value of extend parameter for conditional GOSUB. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_VCR
 * Offset address of VCR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_VCR          (0x0000U)           /**< Offset address of VCR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SCTLR0
 * Offset address of CTLR0 register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SCTLR0       (0x0004U)           /**< Offset address of CTLR0 register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SR
 * Offset address of SR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SR           (0x0008U)           /**< Offset address of SR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SCR
 * Offset address of SCR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SCR          (0x000CU)           /**< Offset address of SCR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SER
 * Offset address of SER register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SER          (0x0010U)           /**< Offset address of SER register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_IMR
 * Offset address of IMR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_IMR          (0x0014U)           /**< Offset address of IMR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SCTLR1
 * Offset address of SCTLR1 register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SCTLR1       (0x0018U)           /**< Offset address of SCTLR1 register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SR1
 * Offset address of SR1 register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SR1          (0x003CU)           /**< Offset address of SR1 register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_CLSAR
 * Offset address of CLSAR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_CLSAR        (0x0038U)           /**< Offset address of CLSAR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SYNCCR0
 * Offset address of SYNCCR0 register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SYNCCR0      (0x0058U)           /**< Offset address of SYNCCR0 register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SYNCCR1
 * Offset address of SYNCCR1 register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SYNCCR1      (0x0060U)           /**< Offset address of SYNCCR1 register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SYNCCR2
 * Offset address of SYNCCR2 register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SYNCCR2      (0x0064U)           /**< Offset address of SYNCCR2 register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SYNCCR3
 * Offset address of SYNCCR3 register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SYNCCR3      (0x0068U)           /**< Offset address of SYNCCR3 register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_CLBRKADDRR
 * Offset address of CLBRKADDRR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_CLBRKADDRR   (0x03F4U)           /**< Offset address of CLBRKADDRR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_CLCNDGSBR
 * Offset address of CLCNDGSBR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_CLCNDGSBR    (0x03FCU)           /**< Offset address of CLCNDGSBR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_VCR
 * Hardware version value for PSC.
***********************************************************************************************************************/
#define IMPDRV_PSC_VCR              (0x20040718U)       /**< Hardware version value for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SCTLR0_SWRST
 * SWRST register value for PSC.
***********************************************************************************************************************/
#define IMPDRV_PSC_SCTLR0_SWRST     (0x80000000U)       /**< SWRST register value for PSC.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SCTLR0_CLR
 * CLR register value for PSC.
***********************************************************************************************************************/
#define IMPDRV_PSC_SCTLR0_CLR       (0x00000000U)       /**< CLR register value for PSC.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SCTLR1_CLE
 * CLE register value for PSC.
***********************************************************************************************************************/
#define IMPDRV_PSC_SCTLR1_CLE       (0x00000001U)       /**< CLE register value for PSC.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_CLCNDGSBR_MASK
 * Bit mask for Condition for conditional GOSUB instruction.
***********************************************************************************************************************/
#define IMPDRV_PSC_CLCNDGSBR_MASK   (0x00000001U)       /**< Bit mask for Condition for conditional GOSUB. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_TEND
 *  TEND interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_TEND          (0x00000001U)       /**< TEND interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_TRAP
 * TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_TRAP          (0x00000010U)       /**< TRAP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_IER
 *  IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_IER           (0x00000020U)       /**< IER interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_INT
 *  INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_INT           (0x00000040U)       /**< INT interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_WUP
 *  WUP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_WUP           (0x00010000U)       /**< WUP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_CLBRK
 *  CLBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_CLBRK         (0x00000400U)       /**< CL break interrupt enable registration. */

/** @} */

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
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
e_impdrv_errorcode_t impdrv_pscctl_check_core_busy(
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
e_impdrv_errorcode_t impdrv_pscctl_soft_reset(
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
e_impdrv_errorcode_t impdrv_pscctl_set_syncc_config(
    const st_impdrv_corectl_t   *const p_corectl
);

/*******************************************************************************************************************//**
 * @brief           Set extended configuration to related registers.
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
e_impdrv_errorcode_t impdrv_pscctl_set_extend_config(
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
uint32_t impdrv_pscctl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
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
e_impdrv_errorcode_t impdrv_pscctl_check_inten(
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
e_impdrv_errorcode_t impdrv_pscctl_int_cb(
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
* Start of function impdrv_pscctl_is_valid_core()
* CD_PD_02_04_4001
* [Covers: UD_PD_UD02_04_0001]
***********************************************************************************************************************/
bool impdrv_pscctl_is_valid_core(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    bool is_valid_core = false;

    /** Compare core number to valid core number */
    if (core_num < IMPDRV_PSC_CORENUM_VAL)
    {
        is_valid_core = true;
    }

    return is_valid_core;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_is_valid_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_check_state()
* CD_PD_02_04_4002
* [Covers: UD_PD_UD02_04_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_check_state(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);

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
* End of function impdrv_pscctl_check_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_state()
* CD_PD_02_04_4003
* [Covers: UD_PD_UD02_04_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_set_state(
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
* End of function impdrv_pscctl_set_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_init_start()
* CD_PD_02_04_4004
* [Covers: UD_PD_UD02_04_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                ret = 0U;
    st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_PSCEXE, core_num};

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
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
        ercd = impdrv_osdep_read_reg(&(p_coretypectl->core_ctl[core_num].device_io), IMPDRV_REG_PSC_VCR, &ret);
    }

    /** Check core version (unsupported core?) */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_PSC_VCR != ret)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_init_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_init_end()
* CD_PD_02_04_4005
* [Covers: UD_PD_UD02_04_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_init_end(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
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
* End of function impdrv_pscctl_init_end()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_start()
* CD_PD_02_04_4006
* [Covers: UD_PD_UD02_04_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_start(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_INIT);
    }

    /** Request for power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        core_info.core_type = IMPDRV_CORE_TYPE_PSCEXE;
        core_info.core_num  = core_num;
        ercd = impdrv_osdep_pow_on_imp(
            &(p_coretypectl->core_ctl[core_num].device_io),
            core_info
        );
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_READY);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_stop()
* CD_PD_02_04_4007
* [Covers: UD_PD_UD02_04_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_stop(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
    }

    /** Release for Power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off_imp(&(p_coretypectl->core_ctl[core_num].device_io));
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_INIT);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_stop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_quit()
* CD_PD_02_04_4024
* [Covers: UD_PD_UD02_04_0024]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_quit(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    st_impdrv_core_info_t       core_info;
    st_impdrv_device_handle_t   device_io;

    /** Check core number */
    is_valid_core = impdrv_pscctl_is_valid_core(core_num);
    if (true != is_valid_core)
    {
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    /** Request osdep to open the core */
    if (IMPDRV_EC_OK == ercd)
    {
        core_info.core_type = IMPDRV_CORE_TYPE_PSCEXE;
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
* End of function impdrv_pscctl_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_attr_init()
* CD_PD_02_04_4008
* [Covers: UD_PD_UD02_04_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_attr_init(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
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

        p_corectl->irq_mask[IMPDRV_IRQMASK_END] = true;
        p_corectl->irq_mask[IMPDRV_IRQMASK_WUPCOVF] = true;

        for (i = 0U; i < IMPDRV_EXEPARAM_MAX; i++)
        {
            p_corectl->param[i] = 0U;
        }
        p_corectl->param[IMPDRV_PSC_EXEPARAM_CLBRK] = IMPDRV_CLBRK_ADDR_INVALID;
        p_corectl->param[IMPDRV_PSC_EXEPARAM_GOSUB] = IMPDRV_EXEPARAM_INVALID;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_attr_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_mem_init()
* CD_PD_02_04_4009
* [Covers: UD_PD_UD02_04_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_set_mem_init(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
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
* End of function impdrv_pscctl_set_mem_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_core_map()
* CD_PD_02_04_4010
* [Covers: UD_PD_UD02_04_0010]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_set_core_map(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_pscctl_set_core_map()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_cl()
* CD_PD_02_04_4011
* [Covers: UD_PD_UD02_04_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_set_cl(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_pscctl_set_cl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_irq_mask()
* CD_PD_02_04_4012
* [Covers: UD_PD_UD02_04_0012]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_set_irq_mask(
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
            is_valid_core = impdrv_pscctl_is_valid_core(core_num);
            if (true != is_valid_core)
            {
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_pscctl_set_irq_mask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_cl_brk_addr()
* CD_PD_02_04_4055
* [Covers: UD_PD_UD02_04_0055]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_set_cl_brk_addr(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          cl_brk_addr
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
    else if ((IMPDRV_CLBRK_ADDR_INVALID != cl_brk_addr)
                && (0U != (cl_brk_addr & IMPDRV_CHK_CL_MEM_ALIGN)))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
        if (IMPDRV_EC_NG_SEQSTATE == ercd)
        {
            ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_INT);
        }
    }

    /** Set data to CL break address */
    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->param[IMPDRV_PSC_EXEPARAM_CLBRK] = cl_brk_addr;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_set_cl_brk_addr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_cond_gosub()
* CD_PD_02_04_4058
* [Covers: UD_PD_UD02_04_0058]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_set_cond_gosub(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_gosub_cond_t condition
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
    else if ((IMPDRV_GOSUB_COND_NOTEXEC != condition) && (IMPDRV_GOSUB_COND_EXEC != condition))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
        if (IMPDRV_EC_NG_SEQSTATE == ercd)
        {
            ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_INT);
        }
    }

    /** Set data to conditional GOSUB instruction setting. */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_GOSUB_COND_NOTEXEC == condition)
        {
            p_corectl->param[IMPDRV_PSC_EXEPARAM_GOSUB] = IMPDRV_EXEPARAM_OFF;
        }
        else
        {
            p_corectl->param[IMPDRV_PSC_EXEPARAM_GOSUB] = IMPDRV_EXEPARAM_ON;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_set_cond_gosub()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_execute()
* CD_PD_02_04_4014
* [Covers: UD_PD_UD02_04_0014]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_execute(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    }

    /** Check core busy */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_core_busy(p_corectl);
    }

    /** Operate the hardware register to execute on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Initialization of interrupt mask variable */
        all_mask_val = IMPDRV_PSC_SR_TRAP | IMPDRV_PSC_SR_IER | IMPDRV_PSC_SR_INT
                        | IMPDRV_PSC_SR_TEND | IMPDRV_PSC_SR_WUP | IMPDRV_PSC_SR_CLBRK;

        /** Set callback function */
        p_corectl->cbinfo.cbfunc = callback_func;
        p_corectl->cbinfo.p_cbarg = p_callback_args;

        /** Soft reset */
        ercd = impdrv_pscctl_soft_reset(p_corectl);

        /** SYNCC register setting */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_pscctl_set_syncc_config(p_corectl);
        }

        /** Set extended attribute to related registers */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_pscctl_set_extend_config(p_corectl);
        }

        /** Release the interrupt mask */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Status Clear Register(SCR) */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SCR, all_mask_val, false, 0U);
            if (IMPDRV_EC_OK == ercd)
            {
                inten_val = impdrv_pscctl_get_inten_val(p_corectl);
                if (0U == inten_val)
                {
                    ercd = IMPDRV_EC_NG_PARAM;
                }

                /** Write the Status Enable Register(SER) */
                if (IMPDRV_EC_OK == ercd)
                {
                    reg_val = inten_val;
                    ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SER,
                                                                        reg_val, true, reg_val);
                }

                /** Write the Interrupt Mask Register(IMR) for mask release */
                if (IMPDRV_EC_OK == ercd)
                {
                    /** Bit invert of enable interrupts, and mask the specified factor. */
                    reg_val = all_mask_val & ~inten_val;
                    ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_IMR,
                                                                        reg_val, true, reg_val);
                }
            }
        }

        /** Set the CL physical address */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Write the Command List Start Address Register(CLSAR) */
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_CLSAR,
                                            p_corectl->claddr_phys, true, p_corectl->claddr_phys);
        }

        /** CL execute on specific core */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Change core state before CL Executed */
            ercd = impdrv_pscctl_set_state(p_corectl, IMPDRV_STATE_EXEC);
            if (IMPDRV_EC_OK == ercd)
            {
                /** guarantees memory synchronization between contexts */
                ercd = impdrv_osdep_memory_barrier();
            }

            /** Start CL execution on the target core */
            if (IMPDRV_EC_OK == ercd)
            {
                /** CL execute start callback */
                core_info.core_type = IMPDRV_CORE_TYPE_PSCEXE;
                core_info.core_num  = core_num;
                (void)callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, IMPDRV_CALLBACK_CODE_DEF, p_callback_args);

                /** Write the System Control Register(SCTLR1) for CL execution */
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SCTLR1,
                                                            IMPDRV_PSC_SCTLR1_CLE, false, 0U);
            }
        }

        /** Recovery processing at the CL execute error. */
        if (IMPDRV_EC_OK != ercd)
        {
            /** Interrupt mask setting */
            (void)impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_IMR,
                                                            all_mask_val, true, 0U);

            /** Restore core state */
            (void)impdrv_pscctl_set_state(p_corectl, IMPDRV_STATE_READY);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_execute()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_int_handler()
* CD_PD_02_04_4015
* [Covers: UD_PD_UD02_04_0015]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    param_chk = false;
    uint32_t                i;
    bool                    is_valid_core;
    uint32_t                stat_val;
    uint32_t                stat_sr1;
    uint32_t                reg_val;
    uint32_t                all_mask_val;
    uint32_t                mask_val = 0U;
    uint32_t                inten_val;
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX];
    uint32_t                cb_ercd_num = 0U;
    e_impdrv_state_t        state = IMPDRV_STATE_EXEC;
    bool                    is_check_intclear = true;

    /** Initialization of interrupt mask variable */
    all_mask_val = IMPDRV_PSC_SR_TRAP | IMPDRV_PSC_SR_IER | IMPDRV_PSC_SR_INT
                    | IMPDRV_PSC_SR_TEND | IMPDRV_PSC_SR_WUP | IMPDRV_PSC_SR_CLBRK;

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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
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

    /** Read the Status Register(SR) */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SR, &stat_val);
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_EXEC);
    }

    /** Call back occurrence of an interrupt to the CL execute requester */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_inten(stat_val,
                                            &mask_val, cb_ercd, &cb_ercd_num, &is_check_intclear, &state);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Set the interrupt mask */
        if ((IMPDRV_STATE_EXEC == state) && (0U != mask_val))
        {
            inten_val = impdrv_pscctl_get_inten_val(p_corectl);
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
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_IMR,
                                                                reg_val, true, reg_val);
        }

        if (IMPDRV_EC_OK == ercd)
        {
            /** Read the Status Register(SR1) */
            ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SR1, &stat_sr1);
        }

        /** Change core state */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_pscctl_set_state(p_corectl, state);
            if (IMPDRV_EC_OK == ercd)
            {
                /** guarantees memory synchronization between contexts */
                ercd = impdrv_osdep_memory_barrier();
            }
        }

        /** Interrupt result callback */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_pscctl_int_cb(p_corectl, core_num, cb_ercd, cb_ercd_num, is_check_intclear, stat_sr1);
        }
    }

    if ((IMPDRV_EC_OK != ercd) && (true == param_chk))
    {
        /** Write the Interrupt Mask Register(IMR) for mask setting */
        (void)impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_IMR, all_mask_val, false, 0U);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_resume_exe()
* CD_PD_02_04_4016
* [Covers: UD_PD_UD02_04_0016]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                all_mask_val;
    uint32_t                reg_val = 0U;
    uint32_t                inten_val;

    /** Initialization of interrupt mask variable */
    all_mask_val = IMPDRV_PSC_SR_TRAP | IMPDRV_PSC_SR_IER | IMPDRV_PSC_SR_INT
                    | IMPDRV_PSC_SR_TEND | IMPDRV_PSC_SR_WUP | IMPDRV_PSC_SR_CLBRK;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_check_state(p_corectl, core_num, IMPDRV_STATE_INT);
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_set_state(p_corectl, IMPDRV_STATE_EXEC);
        if (IMPDRV_EC_OK == ercd)
        {
            /** guarantees memory synchronization between contexts */
            ercd = impdrv_osdep_memory_barrier();
        }
    }

    /** Set extended attribute to related registers */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_pscctl_set_extend_config(p_corectl);
    }

    /** CL resume on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Write the Status Clear Register(SCR) */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SCR,
                                        (IMPDRV_PSC_SR_INT | IMPDRV_PSC_SR_CLBRK), false, 0U);

        /** Write the Interrupt Mask Register(IMR) for release setting */
        if (IMPDRV_EC_OK == ercd)
        {
            inten_val = impdrv_pscctl_get_inten_val(p_corectl);
            if (0U == inten_val)
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }

            /** Write the Status Enable Register(SER) */
            if (IMPDRV_EC_OK == ercd)
            {
                reg_val = inten_val;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SER,
                                                                    reg_val, true, reg_val);
            }

            /** Write the Interrupt Mask Register(IMR) for mask release */
            if (IMPDRV_EC_OK == ercd)
            {
                /** Bit invert of enable interrupts, and mask the specified factor. */
                reg_val = all_mask_val & ~inten_val;
                ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_IMR,
                                                                    reg_val, false, 0U);
            }
        }

        /** Recovery processing at the CL resume error. */
        if (IMPDRV_EC_OK != ercd)
        {
            /** Restore core state */
            (void)impdrv_pscctl_set_state(p_corectl, IMPDRV_STATE_READY);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_resume_exe()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_pm_set_policy()
* CD_PD_02_04_4017
* [Covers: UD_PD_UD02_04_0017]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_pm_set_policy(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_PSCEXE, core_num};

        ercd = impdrv_osdep_pm_set_policy(&(p_coretypectl->core_ctl[core_num].device_io), core_info, policy);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_pm_set_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_pm_get_policy()
* CD_PD_02_04_4018
* [Covers: UD_PD_UD02_04_0018]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_pm_get_policy(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_PSCEXE, core_num};

        ercd = impdrv_osdep_pm_get_policy(&(p_coretypectl->core_ctl[core_num].device_io), core_info, p_policy);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_pm_get_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_pm_get_state()
* CD_PD_02_04_4013
* [Covers: UD_PD_UD02_04_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_pm_get_state(
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_PSCEXE, core_num};

        ercd = impdrv_osdep_pm_get_state(&(p_coretypectl->core_ctl[core_num].device_io), core_info, p_pmstate);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_conf_reg_check()
* CD_PD_02_04_4054
* [Covers: UD_PD_UD02_04_0054]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_pscctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;

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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Not supported on this SoC */
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_conf_reg_check()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_pscctl_check_core_busy()
* CD_PD_02_04_4039
* [Covers: UD_PD_UD02_04_0039]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_pscctl_check_core_busy(
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
        ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SCTLR1, &reg_val);
        if (IMPDRV_EC_OK == ercd)
        {
            /** When Check busy error, it is judged that preparation for execution is not in place */
            if (0U != (IMPDRV_PSC_SCTLR1_CLE & reg_val))
            {
                ercd = IMPDRV_EC_NG_SEQSTATE;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_check_core_busy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_soft_reset()
* CD_PD_02_04_4041
* [Covers: UD_PD_UD02_04_0041]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_pscctl_soft_reset(
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
        /** Write the System Control Register(SCTLR0) for soft reset */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SCTLR0,
                                                    IMPDRV_PSC_SCTLR0_SWRST, false, 0U);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SCTLR0,
                                                        IMPDRV_PSC_SCTLR0_CLR, false, 0U);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_soft_reset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_syncc_config()
* CD_PD_02_04_4043
* [Covers: UD_PD_UD02_04_0043]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_pscctl_set_syncc_config(
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
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SYNCCR0,
                                                        syncc_val[0], true, syncc_val[0]);

        /** Write the Synchronization Control Register 0(SYNCCR1) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SYNCCR1,
                                                        syncc_val[1], true, syncc_val[1]);
        }
        /** Write the Synchronization Control Register 0(SYNCCR2) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SYNCCR2,
                                                        syncc_val[2], true, syncc_val[2]);
        }
        /** Write the Synchronization Control Register 0(SYNCCR3) */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_SYNCCR3,
                                                        syncc_val[3], true, syncc_val[3]);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_set_syncc_config()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_set_extend_config()
* CD_PD_02_04_4056
* [Covers: UD_PD_UD02_04_0056]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_pscctl_set_extend_config(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd            = IMPDRV_EC_OK;
    uint32_t                exeparam_clbrk  = IMPDRV_CLBRK_ADDR_INVALID;
    uint32_t                exeparam_gosub  = IMPDRV_EXEPARAM_INVALID;
    uint32_t                reg_val;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        exeparam_clbrk = p_corectl->param[IMPDRV_PSC_EXEPARAM_CLBRK];
        exeparam_gosub = p_corectl->param[IMPDRV_PSC_EXEPARAM_GOSUB];
    }

    /** Write the CL Break Address Register(CLBRKADDRR) */
    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_CLBRK_ADDR_INVALID != exeparam_clbrk))
    {
        reg_val = exeparam_clbrk;
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_CLBRKADDRR,
                                                                reg_val, true, reg_val);
    }

    /** Write the CL Conditional GOSUB Register(CLCNDGSBR) */
    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_EXEPARAM_INVALID != exeparam_gosub))
    {
        reg_val = 0U;
        if (IMPDRV_EXEPARAM_ON == exeparam_gosub)
        {
            reg_val |= IMPDRV_PSC_CLCNDGSBR_MASK;
        }
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REG_PSC_CLCNDGSBR,
                                                                    reg_val, true, reg_val);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_set_extend_config()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_get_inten_val()
* CD_PD_02_04_4025
* [Covers: UD_PD_UD02_04_0025]
***********************************************************************************************************************/
IMPDRV_STATIC
uint32_t impdrv_pscctl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;
    uint32_t    clbrk_val;
    uint32_t    i;

    /** Input parameter confirmation process */
    if (NULL != p_corectl)
    {
        /** Set interrupt for PSC core by default */
        inten_val = (IMPDRV_PSC_SR_TRAP | IMPDRV_PSC_SR_IER | IMPDRV_PSC_SR_INT);

        /** Sets the CLBRK interrupt mask values. */
        clbrk_val = p_corectl->param[IMPDRV_PSC_EXEPARAM_CLBRK];
        if (IMPDRV_CLBRK_ADDR_INVALID != clbrk_val)
        {
            inten_val |= IMPDRV_PSC_SR_CLBRK;
        }

        /** Set interrupt for PSC core */
        for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
        {
            if (((uint32_t)IMPDRV_IRQMASK_END == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_PSC_SR_TEND;
            }
            if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == p_corectl->irq_mask[i]))
            {
                inten_val |= IMPDRV_PSC_SR_WUP;
            }
        }
    }

    /** Set data in return value */
    return inten_val;
}
/***********************************************************************************************************************
* End of function impdrv_pscctl_get_inten_val()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_check_inten()
* CD_PD_02_04_4047
* [Covers: UD_PD_UD02_04_0047]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_pscctl_check_inten(
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

        if (0U != (IMPDRV_PSC_SR_TEND & stat_val))
        {
            /** When the TEND bit of SR register is not 0. */
            *p_mask_val = *p_mask_val | IMPDRV_PSC_SR_TEND;
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_END;
            (*p_cb_ercd_num)++;
        }
        if (0U != (IMPDRV_PSC_SR_WUP & stat_val))
        {
            /** When the WUP bit of SR register is not 0. */
            *p_is_check_intclear = false;
            *p_mask_val = *p_mask_val | IMPDRV_PSC_SR_WUP;
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_WUPCOVF;
            (*p_cb_ercd_num)++;
        }
        if (0U != (IMPDRV_PSC_SR_IER & stat_val))
        {
            /** When the IER bit of SR register is not 0. */
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_ILLEGAL;
            (*p_cb_ercd_num)++;
            *p_state = IMPDRV_STATE_READY;
        }
        if (0U != (IMPDRV_PSC_SR_CLBRK & stat_val))
        {
            /** When the CLBRK bit of SR register is not 0. */
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_CLBRK;
            (*p_cb_ercd_num)++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_INT;
            }
        }
        if (0U != (IMPDRV_PSC_SR_INT & stat_val))
        {
            /** When the INT bit of SR register is not 0. */
            cb_ercd[*p_cb_ercd_num] = IMPDRV_CB_RET_INT;
            (*p_cb_ercd_num)++;
            if (IMPDRV_STATE_EXEC == *p_state)
            {
                *p_state = IMPDRV_STATE_INT;
            }
        }
        if (0U != (IMPDRV_PSC_SR_TRAP & stat_val))
        {
            /** When the previous conditions do not apply. */
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
* End of function impdrv_pscctl_check_inten()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_pscctl_int_cb()
* CD_PD_02_04_4045
* [Covers: UD_PD_UD02_04_0045]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_pscctl_int_cb(
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

    core_info.core_type = IMPDRV_CORE_TYPE_PSCEXE;
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
        is_valid_core = impdrv_pscctl_is_valid_core(core_num);
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
                    /** Set SR1 & 0xFF000000U shifted right by 24 bit in intcode */
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
* End of function impdrv_pscctl_int_cb()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/
