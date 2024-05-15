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
* File Name    : r_impdrv_dspctl.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_dspctl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_dspctl.c
 */

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "rcar-xos/imp/r_impdrv.h"
#include "r_impdrv_private.h"
#include "rcar-xos/osal/r_osal.h"
#include "r_impdrv_cmnctl.h"

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
 * @def IMPDRV_DSP_CORENUM_VAL
 * CORENUM_VAL register value for DSP.
***********************************************************************************************************************/
#define IMPDRV_DSP_CORENUM_VAL              (2U)                /**< Maximum number of cores for DSP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_GPIO
 * Offset address of DSP GPIO register.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_GPIO             (0x80624U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_D_ADDx_START
 * Offset address of DSP Data_Memory_Subsystem register.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_D_ADDx_START(x)  (0x80920U + (x)*0x10U)    /* 0 < (x) < 32 */
#define IMPDRV_REGOFS_VDSP_D_ADDx_ATT0(x)   (0x80924U + (x)*0x10U)    /* 0 < (x) < 32 */
#define IMPDRV_REGOFS_VDSP_D_ADDx_ATT1(x)   (0x80928U + (x)*0x10U)    /* 0 < (x) < 32 */
#define IMPDRV_REGOFS_P_ADD1_START          (0x80428U)
#define IMPDRV_REGOFS_P_ADD1_ATT0           (0x8042CU)
#define IMPDRV_REGOFS_MSS_PCR               (0x80420U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_D_XXX
 * Setting value of DSP Data_Memory_Subsystem register.
***********************************************************************************************************************/
#define IMPDRV_SPMX_ADDR_START              (0xED300000U)   /* SPMI/SPIC address */
#define IMPDRV_SPMX_ADDR_END                (0xED600000U)   /* SPMI/SPIC address */
#define IMPDRV_SHFT_D_ADDR_VAL              (12U)           /* 12 bit Shift of start address of external memory */
#define IMPDRV_SHFT_D_MID                   (20U)           /* Bit shift of REGION_MID of D_ADDx_START */
#define IMPDRV_SHFT_D_INACT                 (28U)           /* Bit shift of INACTIVE of D_ADDx_START */
#define IMPDRV_VDSP_EXT_REGION_EDP          (0x1U)
#define IMPDRV_VDSP_EXT_REGION_AXI0         (0x2U)
#define IMPDRV_VDSP_EXT_REGION_BLANK        (0x80U)
#define IMPDRV_VDSP_EXT_REGION_ACTIVE       (0U)
#define IMPDRV_VDSP_EXT_REGION_INACTIVE     (1U)
#define IMPDRV_VDSP_EXT_DPRAW               (0x1U)  /* Data protection Read-after-Write indication. EDP only  */
#define IMPDRV_VDSP_EXT_MOM                 (0x1U)  /* Memory Ordering Model. EDP only */
#define IMPDRV_VDSP_EXT_DABSZ8              (0xAU)  /* 8 transfer/INCR */
#define IMPDRV_VDSP_ACU_LOCK                (0x0U)
#define IMPDRV_VDSP_ACU_UNLOCK              (0x8U)
#define IMPDRV_VDSP_P_ADD1_ATT0             (0x1U)  /* L1IC */
#define IMPDRV_VDSP_MSS_PCR                 ((1u << 2) | (1u << 6) | (1u << 7) | (3u << 8) | (1u << 12))
                                          /* CAC_PFE   | CLBE      | PCAC_EN   | HWPF_SZ   | CAC_WPE */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_SIL_APPCHG
 * Offset address of DSP Application changing register for SIL.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_SIL_APPCHG       (0x83100U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_SIL_CON
 * Offset address of DSP simulator Start register for SIL.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_SIL_CON          (0x83104U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_SIL_EXTMEM_LOW
 * Offset address of external memory setting register for SIL.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_SIL_EXTMEM_LOW   (0x83108U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_SIL_EXTMEM_HIGH
 * Offset address of external memory setting register for SIL.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_SIL_EXTMEM_HIGH  (0x8310CU)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_SIL_EXTMEM_SZ
 * Offset address of external memory setting register for SIL.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_SIL_EXTMEM_SZ    (0x83110U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_SIL_GPIN
 * Offset address of GPIN setting register for SIL.
***********************************************************************************************************************/
#define IMPDRV_REGOFS_VDSP_SIL_GPIN         (0x83114U)
#define IMPDRV_REGVAL_VDSP_GPIO_START       (0x00000008U)
#define IMPDRV_REGVAL_VDSP_GPIO_STOP        (0x00000080U)
#define IMPDRV_REGVAL_VDSP_GPIO_STANDBY     (0x00000001U)
#define IMPDRV_REGVAL_VDSP_GPIO_INIT        (0x00000000U)
#define IMPDRV_REGVAL_VDSP_GPIO_RESUME      (0x00000001U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGOFS_VDSP_SIL_EXTMEM_LOW
 * Offset address of external memory setting register for SIL.
***********************************************************************************************************************/
#define IMPDRV_VDSP_BASE_OFS                (0x40000000U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_SIL_APPCHG
 *  DSP Start register value for SIL.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_SIL_APPCHG       (0x0001U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_APPRUN
 *  DSP Start register value.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_APPRUN           (0x0001U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_ISR_INT
 *  DSP GPOUT register value.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_INT              (0x00000002U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_VDSP_TRAP
 *  DSP GPOUT register value.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_VDSP_TRAP             (0x00000001U)
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMPSLV_DSP0CSTS
 * Offset address of DSP ch0 Communication STaTus.
***********************************************************************************************************************/
#define IMPDRV_REG_IMPSLV_DSP0CSTS          (0x00004014U)       /**< Offset address of DSP ch0 Communication STaTus. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMPSLV_DSP1CSTS
 * Offset address of DSP ch1 Communication STaTus.
***********************************************************************************************************************/
#define IMPDRV_REG_IMPSLV_DSP1CSTS          (0x00004024U)       /**< Offset address of DSP ch0 Communication STaTus. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REGVAL_IMPSLV_DSPNCSTS
 * DSPnCSTS register value.
***********************************************************************************************************************/
#define IMPDRV_REGVAL_IMPSLV_DSPNCSTS       (0x00000000U)   /**< DSPnCSTS register value. */
 
#define IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK     (0xC0004U)
#define IMPDRV_VDSP_ACU_LOCK                (0x0U)
#define IMPDRV_VDSP_ACU_UNLOCK              (0x8U)
#define IMPDRV_DSP_REG_MAX_ADRS_CNFCHK      (0x0019FFFFU)   /** End of offset address for ConfRegCheck of IMP. */
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
 * @brief           Initialization of DSP core.
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
e_impdrv_errorcode_t impdrv_dspctl_init_dsp(
    st_impdrv_corectl_t         *p_corectl
);

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_dspctl_is_valid_core()
* CD_PD_02_04_1001
* [Covers: UD_PD_UD02_04_0001]
***********************************************************************************************************************/
bool impdrv_dspctl_is_valid_core(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    bool is_valid_core = false;

    /** Compare core number to valid core number */
    if (core_num < IMPDRV_DSP_CORENUM_VAL)
    {
        is_valid_core = true;
    }

    return is_valid_core;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_is_valid_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_check_state()
* CD_PD_02_04_1002
* [Covers: UD_PD_UD02_04_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_check_state(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);

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
* End of function impdrv_dspctl_check_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_state()
* CD_PD_02_04_1003
* [Covers: UD_PD_UD02_04_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_state(
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
* End of function impdrv_dspctl_set_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_init_start()
* CD_PD_02_04_1004
* [Covers: UD_PD_UD02_04_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_init_start(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /* copy osal device handle */
    if (IMPDRV_EC_OK == ercd)
    {
        st_impdrv_device_handle_t   *device_io;

        ercd = impdrv_cmnctl_get_io_dsp(&device_io, core_num, 0);
        p_coretypectl->core_ctl[core_num].device_io = *device_io;
    }

#if 0
    st_impdrv_core_info_t   core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
    /** Request osdep to power on core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_on_imp(&(p_coretypectl->core_ctl[core_num].device_io), core_info);
    }
#endif
    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_init_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_init_end()
* CD_PD_02_04_1005
* [Covers: UD_PD_UD02_04_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_init_end(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }
#if 0 
    /** Request osdep to power off core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off_imp(&(p_coretypectl->core_ctl[core_num].device_io));
    }
#endif
    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_init_end()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_start()
* CD_PD_02_04_1006
* [Covers: UD_PD_UD02_04_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_dsp_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_coretypectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_INIT);
    }

    /** Request for power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_on_dsp(&(p_coretypectl->core_ctl[core_num].device_io));
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_enable_dsp_irq(&(p_coretypectl->core_ctl[core_num].device_io));
    }

    if (IMPDRV_EC_OK == ercd)
    {
        impdrv_dspctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_READY);
    }
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_stop()
* CD_PD_02_04_1007
* [Covers: UD_PD_UD02_04_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_stop(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** DSP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
    }

    /** Disable DSP IRQ */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_disable_dsp_irq(&(p_coretypectl->core_ctl[core_num].device_io));
    }

	/* sync with DSP */
    if (false != p_coretypectl->core_ctl[core_num].dsp_info.is_executed)
    {
        uint32_t gp_val;
        /* Trigger to stop processing DSP application. GPIN can use only 0 or START. */
        ercd = impdrv_osdep_write_reg(&(p_coretypectl->core_ctl[core_num].device_io),
                                    IMPDRV_REGOFS_VDSP_SIL_GPIN, IMPDRV_REGVAL_VDSP_GPIO_START, false ,0U);
        do {
            (void)impdrv_osdep_read_reg(&(p_coretypectl->core_ctl[core_num].device_io),
                                    IMPDRV_REGOFS_VDSP_GPIO, &gp_val);
        } while (gp_val != IMPDRV_REGVAL_VDSP_GPIO_STOP);
        (void)impdrv_osdep_write_reg(&(p_coretypectl->core_ctl[core_num].device_io),
                                    IMPDRV_REGOFS_VDSP_SIL_GPIN, 0x00000000, false ,0U);

        /* wait until standby */
        do {
            (void)impdrv_osdep_read_reg(&(p_coretypectl->core_ctl[core_num].device_io),
                                    IMPDRV_REGOFS_VDSP_GPIO, &gp_val);
        } while (gp_val != IMPDRV_REGVAL_VDSP_GPIO_STANDBY);
        (void)impdrv_osdep_write_reg(&(p_coretypectl->core_ctl[core_num].device_io),
                                IMPDRV_REGOFS_VDSP_SIL_GPIN, 0x00000000, false ,0U);
    }

    /** Release for Power manager control */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off_dsp(&(p_coretypectl->core_ctl[core_num].device_io));
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_set_state(&p_coretypectl->core_ctl[core_num], IMPDRV_STATE_INIT);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_stop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_quit()
* CD_PD_02_04_6024
* [Covers: UD_PD_UD02_04_0024]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_quit(
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    bool                        is_valid_core;

    /** Check core number */
    is_valid_core = impdrv_dspctl_is_valid_core(core_num);
    if (true != is_valid_core)
    {
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    /** No processing on the SIL */

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_attr_init()
* CD_PD_02_04_1008
* [Covers: UD_PD_UD02_04_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_attr_init(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
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
* End of function impdrv_dspctl_attr_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_mem_init()
* CD_PD_02_04_1009
* [Covers: UD_PD_UD02_04_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_mem_init(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_INIT);
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
* End of function impdrv_dspctl_set_mem_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_core_map()
* CD_PD_02_04_1010
* [Covers: UD_PD_UD02_04_0010]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_core_map(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_dspctl_set_core_map()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_cl()
* CD_PD_02_04_1011
* [Covers: UD_PD_UD02_04_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_cl(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_dspctl_set_cl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_dsp()
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_dsp(
    st_impdrv_corectl_t         *p_corectl,
    const uint32_t              core_num,
    const st_impdrv_dsp_data_t  *const p_dsp_app,
    const st_impdrv_dsp_data_t  *const p_dsp_fw,
    const st_impdrv_dsp_data_t  *const p_dsp_data,
    const st_impdrv_dsp_data_t  *const p_dsp_dtcm
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

//    ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    (void)core_num;
    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->dsp_info.dsp_app  = *p_dsp_app;
        p_corectl->dsp_info.dsp_fw   = *p_dsp_fw;
        p_corectl->dsp_info.dsp_data = *p_dsp_data;
        p_corectl->dsp_info.dsp_dtcm = *p_dsp_dtcm;
        if (0U != p_dsp_data->size)
        {
            p_corectl->dsp_info.is_update = true;
        }
        else
        {
            p_corectl->dsp_info.is_update = false;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_set_dsp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_set_irq_mask()
* CD_PD_02_04_1012
* [Covers: UD_PD_UD02_04_0012]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_irq_mask(
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
            is_valid_core = impdrv_dspctl_is_valid_core(core_num);
            if (true != is_valid_core)
            {
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
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
* End of function impdrv_dspctl_set_irq_mask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_dsp_execute()
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_dsp_execute(
    st_impdrv_corectl_t     *p_corectl,
    const uint32_t          core_num,
    const p_impdrv_cbfunc_t callback_func,
    void                    *const p_callback_args
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_core_info_t   core_info;

    ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_READY);
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_init_dsp(p_corectl);
    }

    /* Clear interrupt state (IMPSLV.DSPnCSTS) */
    if (IMPDRV_EC_OK == ercd)
    {
        st_impdrv_device_handle_t   handle;
        uint32_t    reg_addr_tbl[IMPDRV_DSP_CORENUM_VAL] = {
                        IMPDRV_REG_IMPSLV_DSP0CSTS,
                        IMPDRV_REG_IMPSLV_DSP1CSTS
        };

        ercd = impdrv_cmnctl_get_io_imptop(&handle);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&handle, reg_addr_tbl[core_num],
                                IMPDRV_REGVAL_IMPSLV_DSPNCSTS, false, 0U);
        }
    }

    if ((p_corectl->dsp_info.is_loaded  == true) &&
        (p_corectl->dsp_info.is_update == true))
    {
        /* Change DSP application */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REGOFS_VDSP_SIL_APPCHG,
                            IMPDRV_REGVAL_VDSP_SIL_APPCHG, false, 0U);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /* [PD 0108-1005] set callback function */
        p_corectl->cbinfo.cbfunc = callback_func;
        p_corectl->cbinfo.p_cbarg = p_callback_args;

        /* [PD 0108-1017] change state */
        impdrv_dspctl_set_state(p_corectl, IMPDRV_STATE_EXEC);

        /* [PD 0108-1100] start callback */
        core_info.core_type = IMPDRV_CORE_TYPE_DSP;
        core_info.core_num  = core_num;
        callback_func(&core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1, p_callback_args);
    }

    if (ercd == IMPDRV_EC_OK)
    {
#if 0 /* dsp_server error: */
      /* SystemC error Error from b_transport, response status = TLM_GENERIC_ERROR_RESPONSE */
        uintptr_t               dataaddr_phys = p_corectl->dsp_info.dsp_data.addr_phys;
        uintptr_t               progaddr_phys = p_corectl->dsp_info.dsp_app.addr_phys;
        uint32_t                data_low = (uint32_t)dataaddr_phys;
        uint32_t                data_high = (uint32_t)(dataaddr_phys >> 32);
        uint32_t                prog_low = (uint32_t)progaddr_phys;
        uint32_t                prog_high = (uint32_t)(progaddr_phys >> 32);
        uint32_t                l_register;
        //Enable writing by CPU (Unlock)
        e_impdrv_errorcode_t ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                                                IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK, IMPDRV_VDSP_ACU_UNLOCK, false, 0);

        /* Base address of dsp_server is 0x4000_0000. */
        /* If data address is over the 0x4000_0000, IMPDRV_REGOFS_VDSP_SIL_EXTMEM_LOW should be set 0x4000_0000. */
        if ((data_high == 0U) && (data_low > IMPDRV_VDSP_BASE_OFS))
        {
            data_low = IMPDRV_VDSP_BASE_OFS;
        }
        if ((prog_high == 0U) && (prog_low > IMPDRV_VDSP_BASE_OFS))
        {
            prog_low = IMPDRV_VDSP_BASE_OFS;
        }

        /* setting external memory config */
        /* D_ADDR0, EDP port */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            /* CEVA DSP Bug #35 W.A. */
            l_register = IMPDRV_VDSP_EXT_MOM << 16U;
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_D_ADDx_ATT0(0), l_register, false, l_register);
        }
        /* D_ADDR1 : Shared memory, EDP port */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = ((data_low >> IMPDRV_SHFT_D_ADDR_VAL) |
                            (IMPDRV_VDSP_EXT_REGION_EDP << IMPDRV_SHFT_D_MID) |
                            (IMPDRV_VDSP_EXT_REGION_ACTIVE << IMPDRV_SHFT_D_INACT) |
                            (IMPDRV_VDSP_EXT_DPRAW << 29U));
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_D_ADDx_START(1), l_register, false, l_register);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = IMPDRV_VDSP_EXT_MOM << 16U;
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_D_ADDx_ATT0(1), l_register, false, l_register);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = IMPDRV_VDSP_EXT_DABSZ8 << 9U;
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_D_ADDx_ATT1(1), l_register, false, l_register);
        }
        

        /* D_ADDR2 : Scratchpad memory(SPMI/SMPC), AXIM0 port */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = ((IMPDRV_SPMX_ADDR_START >> IMPDRV_SHFT_D_ADDR_VAL) |
                            (IMPDRV_VDSP_EXT_REGION_AXI0 << IMPDRV_SHFT_D_MID) |
                            (IMPDRV_VDSP_EXT_REGION_ACTIVE << IMPDRV_SHFT_D_INACT));
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_D_ADDx_START(2), l_register, false, l_register);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = IMPDRV_VDSP_EXT_DABSZ8 << 9U;
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_D_ADDx_ATT1(2), l_register, false, l_register);
        }

        /* D_ADDR3 : Blank area */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = ((IMPDRV_SPMX_ADDR_END >> IMPDRV_SHFT_D_ADDR_VAL) |
                            (IMPDRV_VDSP_EXT_REGION_BLANK << IMPDRV_SHFT_D_MID) |
                            (IMPDRV_VDSP_EXT_REGION_ACTIVE << IMPDRV_SHFT_D_INACT));
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_D_ADDx_START(3), l_register, false, l_register);
        }

        /* P_ADDR1 : Program memory */
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = ((prog_low >> IMPDRV_SHFT_D_ADDR_VAL) |
                            (IMPDRV_VDSP_EXT_REGION_ACTIVE << IMPDRV_SHFT_D_INACT));
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_P_ADD1_START, l_register, false, l_register);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = IMPDRV_VDSP_P_ADD1_ATT0;
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_P_ADD1_ATT0, l_register, false, l_register);
        }
        if (ercd_internal == IMPDRV_EC_OK)
        {
            l_register = IMPDRV_VDSP_MSS_PCR;
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_MSS_PCR, l_register, false, l_register);
        }

        //disable writing by CPU (Lock)
        if (ercd_internal == IMPDRV_EC_OK)
        {
            ercd_internal = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_ICU_ACU_LOCK, IMPDRV_VDSP_ACU_LOCK, false, l_register);
        }

#endif

        uint32_t gp_val;
        /* Trigger to start processing DSP application */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_SIL_GPIN, IMPDRV_REGVAL_VDSP_GPIO_START, false ,0U);
        do {
            (void)impdrv_osdep_read_reg(&p_corectl->device_io,IMPDRV_REGOFS_VDSP_GPIO, &gp_val);
        } while (gp_val != IMPDRV_REGVAL_VDSP_GPIO_START);
    }

    /* Run DSP Application */
    if (IMPDRV_EC_OK == ercd)
    {
        p_corectl->dsp_info.is_loaded = true;
        (void)impdrv_osdep_write_reg(&p_corectl->device_io,
                            IMPDRV_REGOFS_VDSP_SIL_GPIN, 0x00000000, false ,0U);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_dsp_execute()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_int_handler()
* CD_PD_02_04_1015
* [Covers: UD_PD_UD02_04_0015]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd    = IMPDRV_EC_OK;
    bool                    is_valid_core;
    uint32_t                intsts_val;
    e_impdrv_cb_ret_t       cb_ercd[IMPDRV_CB_RET_MAX] = {IMPDRV_CB_RET_OK};
    uint32_t                cb_ercd_num = 0U;
    uint32_t                cb_num;
    e_impdrv_state_t        state = IMPDRV_STATE_EXEC;
    st_impdrv_core_info_t   core_info;

    core_info.core_type = IMPDRV_CORE_TYPE_DSP;
    core_info.core_num  = core_num;

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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Read the GPOUT */
    if (IMPDRV_EC_OK == ercd)
    {
        /* Temporary processing */
        /* Read GPOUT to determine the type of interrupt */
        /* 10 times read is the minimum value that can read the GPOUT value correctly */
        for(int times = 0; times < 10; times++)
        {
            ercd = impdrv_osdep_read_reg(&p_corectl->device_io,IMPDRV_REGOFS_VDSP_GPIO, &intsts_val);
        }
    }

    /** Check core state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_EXEC);
    }

    /** Call back occurrence of an interrupt to the CL execute requester */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_REGVAL_VDSP_INT == intsts_val)
        {
            /** When the INT bit of INTSTS register is not 0. */
            cb_ercd[cb_ercd_num] = IMPDRV_CB_RET_INT;
            cb_ercd_num ++;
            if (IMPDRV_STATE_EXEC == state)
            {
                state = IMPDRV_STATE_INT;
            }
        }
        if (IMPDRV_REGVAL_VDSP_TRAP == intsts_val)
        {
            /** When the TRAP bit of INTSTS register is not 0. */
            cb_ercd[cb_ercd_num] = IMPDRV_CB_RET_OK;
            cb_ercd_num ++;
            if (IMPDRV_STATE_EXEC == state)
            {
                state = IMPDRV_STATE_READY;
            }
            else
            {
                /* unknown intterrupt */
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }

        /** Change core state */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dspctl_set_state(p_corectl, state);
        }

        /** Interrupt result callback */
        if (IMPDRV_EC_OK == ercd)
        {
            cb_num = 0;
            /** TODO: when a callback error occurs handling */
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
* End of function impdrv_dspctl_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_resume_exe()
* CD_PD_02_04_1016
* [Covers: UD_PD_UD02_04_0016]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_valid_core;

    /** Input parameter confirmation process */
    if (NULL == p_corectl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

     /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(p_corectl, core_num, IMPDRV_STATE_INT);
    }

    /** IMP driver status update */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_set_state(p_corectl, IMPDRV_STATE_EXEC);
        if (IMPDRV_EC_OK == ercd)
        {
            /** guarantees memory synchronization between contexts */
            ercd = impdrv_osdep_memory_barrier();
        }
    }

    /** CL resume on specific core */
    if (IMPDRV_EC_OK == ercd)
    {
        uint32_t gp_val;
        uint32_t l_loop;
        uint32_t max_loop = 0x00100000; /* 1M */
        /* Notify DSP that resume processing is completed by writing to GPIN  */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io, IMPDRV_REGOFS_VDSP_SIL_GPIN, 0x00000001, false ,0U);

        /* Waiting for receiving the start of stop processing from DSP */
        l_loop = 0U;
        do {
            if ((ercd == IMPDRV_EC_OK) && (l_loop < max_loop))
            {
                ercd = impdrv_osdep_read_reg(&p_corectl->device_io,
                                IMPDRV_REGOFS_VDSP_GPIO, &gp_val);
                l_loop++;
            } else {
                /*sequence error */
                ercd = IMPDRV_EC_NG_SEQSTATE;
                break;
            }
        } while (gp_val != IMPDRV_REGVAL_VDSP_GPIO_RESUME);

        /* Clear GPIO_IN register */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io,
                                    IMPDRV_REGOFS_VDSP_SIL_GPIN, 0x00000000, false ,0U);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_resume_exe()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_pm_set_policy()
* CD_PD_02_04_1017
* [Covers: UD_PD_UD02_04_0017]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_pm_set_policy(
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
    else if ((IMPDRV_PM_POLICY_CG != policy) 
            && (IMPDRV_PM_POLICY_PG != policy) 
            && (IMPDRV_PM_POLICY_HP != policy))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check core number */
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t       core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
        st_impdrv_device_handle_t   *device_io;

        ercd = impdrv_cmnctl_get_io_dsp(&device_io, core_info.core_num, 0);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_pm_set_policy(device_io, core_info, policy);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_pm_set_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_pm_get_policy()
* CD_PD_02_04_1018
* [Covers: UD_PD_UD02_04_0018]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_pm_get_policy(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t       core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
        st_impdrv_device_handle_t   *device_io;

        ercd = impdrv_cmnctl_get_io_dsp(&device_io, core_info.core_num, 0);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_pm_get_policy(device_io, core_info, p_policy);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_pm_get_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_pm_get_state()
* CD_PD_02_04_6013
* [Covers: UD_PD_UD02_04_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_pm_get_state(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
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
        st_impdrv_core_info_t       core_info = {IMPDRV_CORE_TYPE_DSP, core_num};
        st_impdrv_device_handle_t   *device_io;

        ercd = impdrv_cmnctl_get_io_dsp(&device_io, core_info.core_num, 0);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_pm_get_state(device_io, core_info, p_pmstate);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_dspctl_conf_reg_check()
* CD_PD_02_04_XXXX
* [Covers: UD_PD_UD02_04_XXXX]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_conf_reg_check(
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
        is_valid_core = impdrv_dspctl_is_valid_core(core_num);
        if (true != is_valid_core)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** IMP driver state check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_dspctl_check_state(&p_coretypectl->core_ctl[core_num], core_num, IMPDRV_STATE_READY);
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
        else if (IMPDRV_DSP_REG_MAX_ADRS_CNFCHK < p_chk_param[index].offset)
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
* End of function impdrv_dspctl_conf_reg_check()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_dspctl_init_dsp()
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_init_dsp(
    st_impdrv_corectl_t *p_corectl
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                lRegData = 0u;
    uintptr_t               dspuseraddr_phys = p_corectl->dsp_info.dsp_data.addr_phys;
    uint32_t                dspusersize = p_corectl->dsp_info.dsp_data.size;
    uint32_t                i = 0u;

    uint32_t                addr_low = (uint32_t)dspuseraddr_phys;
    uint32_t                addr_high = (uint32_t)(dspuseraddr_phys >> 32);

    if (p_corectl->dsp_info.is_update == true)
    {
        /* SET EXTERNAL MEMORY for DSP */
        ercd = impdrv_osdep_write_reg(&p_corectl->device_io,
                                        IMPDRV_REGOFS_VDSP_SIL_EXTMEM_LOW,
                                        addr_low, false, 0U);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io,
                                            IMPDRV_REGOFS_VDSP_SIL_EXTMEM_HIGH,
                                            addr_high, false, 0U);
        }
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&p_corectl->device_io,
                                            IMPDRV_REGOFS_VDSP_SIL_EXTMEM_SZ,
                                            dspusersize, false, 0U);
        }
    }

    /* Work Around for DSP simulator */
    /* Check DSP socket connection at the 1st time.  */
    if (p_corectl->dsp_info.is_loaded == false)
    {
        while (i < 100)
        {
            ercd = impdrv_osdep_read_reg(&p_corectl->device_io, IMPDRV_REGOFS_VDSP_SIL_CON, &lRegData);
            if (IMPDRV_EC_OK == ercd)
            {
                break;
            }
            i++;
        }
        if (100U == i)
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_dspctl_init_dsp()
***********************************************************************************************************************/
/*======================================================================================================================
End of File
======================================================================================================================*/
