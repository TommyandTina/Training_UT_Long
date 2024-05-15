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
* File Name    : r_impdrv_osdep.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_osdep for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_osdep.c
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
 * @def IMPDRV_IMPTOP_DEV_TYPE
 * Static OSAL resource definition for "imp_top".
***********************************************************************************************************************/
#define IMPDRV_IMPTOP_DEV_TYPE      ("imp_top")         /**< Static OSAL resource definition for "imp_top". */
/*******************************************************************************************************************//**
 * @def IMPDRV_DEV_LIST_SIZE
 * Buffer size for R_OSAL_IoGetDeviceList.
***********************************************************************************************************************/
#define IMPDRV_DEV_LIST_SIZE        (255U)              /**< Buffer size for R_OSAL_IoGetDeviceList. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OFFSET_MAX
 * Maximum value for Register offset.
***********************************************************************************************************************/
#define IMPDRV_REG_OFFSET_MAX       (1024U * 256U * 8U) /**< Maximum value for Register offset. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQ_CHANNEL_0
 * IRQ channel number (0).
***********************************************************************************************************************/
#define IMPDRV_IRQ_CHANNEL_0        (0U)                /**< IRQ channel number (0) */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQ_CHANNEL_1
 * IRQ channel number (1).
***********************************************************************************************************************/
#define IMPDRV_IRQ_CHANNEL_1        (1U)                /**< IRQ channel number (1) */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQ_CHANNEL_2
 * IRQ channel number (2).
***********************************************************************************************************************/
#define IMPDRV_IRQ_CHANNEL_2        (2U)                /**< IRQ channel number (2) */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQ_CHANNEL_3
 * IRQ channel number (3).
***********************************************************************************************************************/
#define IMPDRV_IRQ_CHANNEL_3        (3U)                /**< IRQ channel number (3) */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQ_CHANNEL_4
 * IRQ channel number (4).
***********************************************************************************************************************/
#define IMPDRV_IRQ_CHANNEL_4        (4U)                /**< IRQ channel number (4) */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQ_CHANNEL_5
 * IRQ channel number (5).
***********************************************************************************************************************/
#define IMPDRV_IRQ_CHANNEL_5        (5U)                /**< IRQ channel number (5) */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQ_CHANNEL_6
 * IRQ channel number (6).
***********************************************************************************************************************/
#define IMPDRV_IRQ_CHANNEL_6        (6U)                /**< IRQ channel number (6) */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQ_CHANNEL_MAX
 * Maximum value for IRQ channel.
***********************************************************************************************************************/
#define IMPDRV_IRQ_CHANNEL_MAX      (7U)                /**< Maximum value for IRQ channel  */
/*******************************************************************************************************************//**
 * @def IMPDRV_DUMMY_UINT64
 * Uint64_t dummy value.
***********************************************************************************************************************/
#define IMPDRV_DUMMY_UINT64         (0xFFFFFFFFFFFFFFFFULL) /**< Uint64_t dummy value. */
/*******************************************************************************************************************//**
 * @def IMPDRV_QUEUE_MAX_NUMBER
 * Maximum number of message queues.
***********************************************************************************************************************/
#define IMPDRV_QUEUE_MAX_NUMBER     (2U)                /**< Maximum number of message queues.      */
/*******************************************************************************************************************//**
 * @def IMPDRV_QUEUE_MAX_MSG_SIZE
 * Message size from interrupt handler.
***********************************************************************************************************************/
#define IMPDRV_QUEUE_MAX_MSG_SIZE   (sizeof(uint32_t))  /**< Message size from interrupt handler.   */
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_CORE_0
 * DSP core number (0).
***********************************************************************************************************************/
#define IMPDRV_DSP_CORE_0           (0U)                /**< DSP core number (0) */
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_CORE_1
 * DSP core number (1).
***********************************************************************************************************************/
#define IMPDRV_DSP_CORE_1           (1U)                /**< IRQ core number (1) */
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_CORE_2
 * DSP core number (2).
***********************************************************************************************************************/
#define IMPDRV_DSP_CORE_2           (2U)                /**< IRQ core number (2) */
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_CORE_3
 * DSP core number (3).
***********************************************************************************************************************/
#define IMPDRV_DSP_CORE_3           (3U)                /**< IRQ core number (3) */
/**********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def IMPDRV_DSP_SUB_DEV_NUM_MAX
 * num of max sub device for DSP core.
***********************************************************************************************************************/
#define IMPDRV_DSP_SUB_DEV_NUM_MAX  (4U)
/**********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def IMPDRV_ADD_CORE_DEVICE_ID
 * num of device id for each core.
***********************************************************************************************************************/
#define IMPDRV_ADD_CORE_DEVICE_ID   (5U)
/**********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def IMPDRV_ADD_CORE_DEVICE_ID_DSP
 * num of device id for each dsp core.
***********************************************************************************************************************/
#define IMPDRV_ADD_CORE_DEVICE_ID_DSP (7U)
/**********************************************************************************************************************/




/** @} */

/*======================================================================================================================
Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Static_Variables Private file static variables
 *
 * @{
***********************************************************************************************************************/
IMPDRV_STATIC
const char *const sp_type_cnv_table[IMPDRV_CORE_TYPE_MAX] =
{
    "",             /**< Not Assigned   */
    "imp",          /**< IMP core       */
    "imp_slim",     /**< Slim-IMP core  */
    "imp_ocv",      /**< OCV core       */
    "imp_dma",      /**< DMAC core      */
    "imp_dma_slim", /**< Slim-DMAC core */
    "imp_psc",      /**< PSC(exe) core  */
    "",             /**< PSC(out) core  */
    "imp_cnn",      /**< CNN core       */
    "vdsp",         /**< DSP            */
    "imp_dta"       /**< DTA            */
};

static uint32_t s_in_interrupts;


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
 * @brief           Check the Input value of core information.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       core_info           Specified core information
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_chk_core_info(
    const st_impdrv_core_info_t     core_info
);

/*******************************************************************************************************************//**
 * @brief           Check the Instance number value.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num        Specified instance number
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_chk_instance_num(
    const e_impdrv_instance_t       instance_num
);

/*******************************************************************************************************************//**
* @brief            Get irq channel number.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        instance_num      IMPDRV Instance number
* @param[out]       irq_channel       Irq channel number
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_get_irq_channel(
    const e_impdrv_instance_t    instance_num,
    uint64_t                     *const irq_channel
);

/*******************************************************************************************************************//**
 * @brief           Open the device with OSAL IO Manager.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_device_handle     OSAL device handle
 * @param[in]       type                Device type
 * @param[in]       dev_num             Device number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_dev_open(
    st_impdrv_device_handle_t   *const p_device_handle,
    const char                  *const type,
    const size_t                dev_num
);

/*******************************************************************************************************************//**
 * @brief           Open the device with OSAL IO Manager for DSP.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_device_handle     OSAL device handle
 * @param[in]       type                Device type
 * @param[in]       dev_num             Device number
 * @param[in]       sub_dev_num         Sub device number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_dev_open_dsp_sub(
    st_impdrv_device_handle_t   *const p_device_handle,
    const char                  *const type,
    const size_t                dev_num,
    const uint32_t              sub_dev_num
);

/*******************************************************************************************************************//**
 * @brief           Power ON the device with OSAL Power Manager.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_device_handle     OSAL device handle
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_pow_on(
    const st_impdrv_device_handle_t *const p_device_handle
);

/*******************************************************************************************************************//**
 * @brief           Power OFF the device with OSAL Power Manager.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_device_handle     OSAL device handle
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_pow_off(
    const st_impdrv_device_handle_t *const p_device_handle
);

/*******************************************************************************************************************//**
 * @brief           Interrupt handler with OSAL Interrupt Manager.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       device_handle   OSAL device handle
 * @param[in]       irq_channel     Interrupt channel
 * @param[in]       irq_arg         User-specified argument
 * @return          None.
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
);

/*******************************************************************************************************************//**
 * @brief           DSP core0 interrupt handler with OSAL Interrupt Manager.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       device_handle   OSAL device handle
 * @param[in]       irq_channel     Interrupt channel
 * @param[in]       irq_arg         User-specified argument
 * @return          None.
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_dsp0_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
);

/*******************************************************************************************************************//**
 * @brief           DSP core1 interrupt handler with OSAL Interrupt Manager.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       device_handle   OSAL device handle
 * @param[in]       irq_channel     Interrupt channel
 * @param[in]       irq_arg         User-specified argument
 * @return          None.
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_dsp1_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
);

/*******************************************************************************************************************//**
 * @brief           DSP core2 interrupt handler with OSAL Interrupt Manager.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       device_handle   OSAL device handle
 * @param[in]       irq_channel     Interrupt channel
 * @param[in]       irq_arg         User-specified argument
 * @return          None.
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_dsp2_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
);

/*******************************************************************************************************************//**
 * @brief           DSP core3 interrupt handler with OSAL Interrupt Manager.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       device_handle   OSAL device handle
 * @param[in]       irq_channel     Interrupt channel
 * @param[in]       irq_arg         User-specified argument
 * @return          None.
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_dsp3_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
);

/*******************************************************************************************************************//**
* @brief            Get irq channel number.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       irq_channel                     Irq channel number
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_get_dsp_irq_channel(
    uint64_t                     *const irq_channel
);

/** @} */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_on_imp_top()
* CD_PD_02_05_0001
* [Covers: UD_PD_UD02_05_0001]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_imp_top(
    st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    const char*             type = IMPDRV_IMPTOP_DEV_TYPE;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Open the Hardware resource of IMP TOP */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_dev_open(p_device_handle, type, 0U);
    }

    /** Power on of IMP TOP */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request osdep to power on of IMP TOP */
        ercd = impdrv_osdep_pow_on(p_device_handle);
        if (IMPDRV_EC_OK != ercd)
        {
            /** Recovery in case of power on failed */
            (void)impdrv_osdep_dev_close(p_device_handle);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_on_imp_top()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_off_imp_top()
* CD_PD_02_05_0002
* [Covers: UD_PD_UD02_05_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_off_imp_top(
    st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Power OFF IMP TOP */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off(p_device_handle);
    }

    /** Close the Hardware resource of OS Abstraction layer */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_dev_close(p_device_handle);
    }

    /** return code return ercd */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_off_imp_top()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_register_irq()
* CD_PD_02_05_0003
* [Covers: UD_PD_UD02_05_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_register_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
)
{

    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_osal_return_t             lRetOsal;

    uint64_t irq_channel;

    s_in_interrupts = 0U;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == osal_cb_args))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if ((OSAL_INTERRUPT_PRIORITY_TYPE0 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE1 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE2 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE3 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE4 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE5 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE6 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE7 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE8 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE9 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE10 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE11 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE12 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE13 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE14 != irq_priority))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        ercd = impdrv_osdep_get_irq_channel(instance_num, &irq_channel);
    }

    /** Register the interrupt handler */
    if (IMPDRV_EC_OK == ercd)
    {

        /** Request OSAL to register the interrupt handler */
        lRetOsal = R_OSAL_InterruptRegisterIsr(p_device_handle->handle, irq_channel,
                        irq_priority, (p_osal_isr_func_t)impdrv_osdep_int_handler, (void*)osal_cb_args);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_register_irq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_register_dsp_irq()
* CD_PD_02_05_0040
* [Covers: UD_PD_UD02_05_0040]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_register_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    uint32_t                            const core_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
)
{

    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_osal_return_t             lRetOsal;
    p_osal_isr_func_t           int_handler;

    uint64_t irq_channel;

    s_in_interrupts = 0U;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == osal_cb_args))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if ((OSAL_INTERRUPT_PRIORITY_TYPE0 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE1 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE2 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE3 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE4 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE5 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE6 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE7 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE8 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE9 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE10 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE11 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE12 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE13 != irq_priority)
                && (OSAL_INTERRUPT_PRIORITY_TYPE14 != irq_priority))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        ercd = impdrv_osdep_get_dsp_irq_channel(&irq_channel);
    }

    /** Register the interrupt handler */
    if (IMPDRV_EC_OK == ercd)
    {
        switch (core_num)
        {
            case IMPDRV_DSP_CORE_0:
                int_handler = impdrv_osdep_dsp0_int_handler;
                break;
            case IMPDRV_DSP_CORE_1:
                int_handler = impdrv_osdep_dsp1_int_handler;
                break;
            case IMPDRV_DSP_CORE_2:
                int_handler = impdrv_osdep_dsp2_int_handler;
                break;
            case IMPDRV_DSP_CORE_3:
                int_handler = impdrv_osdep_dsp3_int_handler;
                break;
            default:
                /** Error Handle */
                ercd = IMPDRV_EC_NG_PARAM;
                break;
        }
        if (IMPDRV_EC_OK == ercd)
        {
            /** Request OSAL to register the interrupt handler */
            lRetOsal = R_OSAL_InterruptRegisterIsr(p_device_handle->handle, irq_channel,
                            irq_priority, int_handler, (void*)osal_cb_args);
            if (OSAL_RETURN_OK != lRetOsal)
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_register_dsp_irq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_on_dsp()
* CD_PD_02_05_xxxx
* [Covers: UD_PD_UD02_05_xxxx]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_dsp(
    st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /* N/A */
    }

    /** Open the Hardware resource of core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request osdep power on core */
        ercd = impdrv_osdep_pow_on(p_device_handle);
        if (IMPDRV_EC_OK != ercd)
        {
            /** Recovery in case of power on failed */
            (void)impdrv_osdep_dev_close(p_device_handle);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_on_dsp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_off_dsp()
* CD_PD_02_05_xxxx
* [Covers: UD_PD_UD02_05_xxxx]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_off_dsp(
    st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /* N/A */
    }

    /** Power OFF core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off(p_device_handle);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_off_dsp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dsp_close()
* CD_PD_02_05_xxxx
* [Covers: UD_PD_UD02_05_xxxx]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_dsp_close(
    st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Request OSAL to close the core */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_IoDeviceClose(p_device_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
        else
        {
            /** Clear handle data */
            p_device_handle->handle = NULL;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dsp_close()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_enable_irq()
* CD_PD_02_05_0004
* [Covers: UD_PD_UD02_05_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_enable_irq(
    const st_impdrv_device_handle_t *const p_device_handle,
    const e_impdrv_instance_t       instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    uint64_t                irq_channel;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        ercd = impdrv_osdep_get_irq_channel(instance_num, &irq_channel);
    }

    /** Request OSAL to enable interrupt function already registered */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_InterruptEnableIsr(p_device_handle->handle, irq_channel);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_enable_irq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_disable_irq()
* CD_PD_02_05_0005
* [Covers: UD_PD_UD02_05_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_disable_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    uint64_t                irq_channel;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        ercd = impdrv_osdep_get_irq_channel(instance_num, &irq_channel);
    }

    /** call R_OSAL_InterruptDisableIsr() Disable interrupt function already registered */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_InterruptDisableIsr(p_device_handle->handle, irq_channel);
        /** check error code */
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** set RCV_EC_NG_DRV_SYSTEMERROR in ercd */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** return code return ercd */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_disable_irq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_unregister_irq()
* CD_PD_02_05_0006
* [Covers: UD_PD_UD02_05_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_unregister_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const e_impdrv_instance_t           instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    uint64_t                irq_channel;
    uint32_t                lWaitTime;

    for (lWaitTime = 0U; (0U != s_in_interrupts) && (100U > lWaitTime); lWaitTime++)
    {
        lRetOsal = R_OSAL_ThreadSleepForTimePeriod(100L);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            break;
        }
    }

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        ercd = impdrv_osdep_get_irq_channel(instance_num, &irq_channel);
    }

    /** call R_OSAL_InterruptUnregisterIsr() Unregister a function called when interrupt */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_InterruptUnregisterIsr(p_device_handle->handle, irq_channel, impdrv_osdep_int_handler);
        /** check error code */
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** set RCV_EC_NG_DRV_SYSTEMERROR in ercd */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** return code return ercd */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_unregister_irq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_on_imp()
* CD_PD_02_05_0007
* [Covers: UD_PD_UD02_05_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_imp(
    st_impdrv_device_handle_t   *const p_device_handle,
    const st_impdrv_core_info_t core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_osdep_chk_core_info(core_info);
    }

    /** Open the Hardware resource of core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_dev_open_imp(p_device_handle, core_info);
        /** Power on core */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Request osdep power on core */
            ercd = impdrv_osdep_pow_on(p_device_handle);
            if (IMPDRV_EC_OK != ercd)
            {
                /** Recovery in case of power on failed */
                (void)impdrv_osdep_dev_close(p_device_handle);
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_on_imp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_off_imp()
* CD_PD_02_05_0008
* [Covers: UD_PD_UD02_05_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_off_imp(
    st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Power OFF core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off(p_device_handle);
        /** Close the Hardware resource of OS Abstraction layer */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_dev_close(p_device_handle);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_off_imp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pm_set_policy()
* CD_PD_02_05_0009
* [Covers: UD_PD_UD02_05_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pm_set_policy(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    const e_impdrv_pm_policy_t          policy
)
{
    /* initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t        ercd_sub = IMPDRV_EC_OK;
    e_osal_return_t             lRetOsal = OSAL_RETURN_OK;
    bool                        device_open = false;
    e_osal_pm_policy_t          osal_policy = OSAL_PM_POLICY_INVALID;
    st_impdrv_device_handle_t   *p_pm_device_handle;
    st_impdrv_device_handle_t   pm_device_handle;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PM_POLICY_CG != policy) && (IMPDRV_PM_POLICY_PG != policy) && (IMPDRV_PM_POLICY_HP != policy))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        ercd = impdrv_osdep_chk_core_info(core_info);
    }

    /* Check to open the device */
    if (IMPDRV_EC_OK == ercd)
    {
        if (NULL == p_device_handle->handle)
        {
            /** Open the Hardware resource of core */
            pm_device_handle.handle = NULL;
            ercd = impdrv_osdep_dev_open_imp(&pm_device_handle, core_info);
            /* Save Device Open State */
            if (IMPDRV_EC_OK == ercd)
            {
                device_open = true;
                p_pm_device_handle = &pm_device_handle;
            }
        }
        else
        {
            p_pm_device_handle = (st_impdrv_device_handle_t*)p_device_handle;
        }
    }

    /* Set of Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        if (IMPDRV_PM_POLICY_HP == policy)
        {
            osal_policy = OSAL_PM_POLICY_HP;
        }
        else if (IMPDRV_PM_POLICY_CG == policy)
        {
            osal_policy = OSAL_PM_POLICY_CG;
        }
        else
        {
            osal_policy = OSAL_PM_POLICY_PG;
        }

        lRetOsal = R_OSAL_PmSetPolicy(p_pm_device_handle->handle, osal_policy, false);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }
    }

    /* Check if the device is open */
    if (true == device_open)
    {
        /** Close the Hardware resource of OS Abstraction layer */
        ercd_sub = impdrv_osdep_dev_close(p_pm_device_handle);

        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_sub;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pm_set_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pm_get_policy()
* CD_PD_02_05_0010
* [Covers: UD_PD_UD02_05_0010]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pm_get_policy(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    e_impdrv_pm_policy_t                *const p_policy
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t        ercd_sub = IMPDRV_EC_OK;
    e_osal_return_t             lRetOsal = OSAL_RETURN_OK;
    e_osal_pm_policy_t          osal_policy = OSAL_PM_POLICY_INVALID;
    bool                        device_open = false;
    st_impdrv_device_handle_t   *p_pm_device_handle;
    st_impdrv_device_handle_t   pm_device_handle;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == p_policy))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_osdep_chk_core_info(core_info);
    }

    /* Check to open the device */
    if (IMPDRV_EC_OK == ercd)
    {
        if (NULL == p_device_handle->handle)
        {
            /** Open the Hardware resource of core */
            pm_device_handle.handle = NULL;
            ercd = impdrv_osdep_dev_open_imp(&pm_device_handle, core_info);
            /* Save Device Open State */
            if (IMPDRV_EC_OK == ercd)
            {
                device_open = true;
                p_pm_device_handle = &pm_device_handle;
            }
        }
        else
        {
            p_pm_device_handle = (st_impdrv_device_handle_t*)p_device_handle;
        }
    }

    /* Get of Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_PmGetPolicy(p_pm_device_handle->handle, &osal_policy);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }
        else
        {
            if (OSAL_PM_POLICY_HP == osal_policy)
            {
                *p_policy = IMPDRV_PM_POLICY_HP;
            }
            else if (OSAL_PM_POLICY_CG == osal_policy)
            {
                *p_policy = IMPDRV_PM_POLICY_CG;
            }
            else if (OSAL_PM_POLICY_PG == osal_policy)
            {
                *p_policy = IMPDRV_PM_POLICY_PG;
            }
            else
            {
                /* unexpected policy return from OSAL */
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }
    }

    /* Check if the device is open */
    if (true == device_open)
    {
        /** Close the Hardware resource of OS Abstraction layer */
        ercd_sub = impdrv_osdep_dev_close(p_pm_device_handle);

        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_sub;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pm_get_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pm_get_state()
* CD_PD_02_05_004
* [Covers: UD_PD_UD02_05_0044]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pm_get_state(
    st_impdrv_device_handle_t           *const p_device_handle,
    const st_impdrv_core_info_t         core_info,
    e_impdrv_pm_state_t                 *const p_pmstate
)
{
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t        ercd_sub = IMPDRV_EC_OK;
    e_osal_return_t             lRetOsal = OSAL_RETURN_OK;
    e_osal_pm_state_t           osal_state;
    bool                        device_open = false;
    st_impdrv_device_handle_t   *p_pm_device_handle;
    st_impdrv_device_handle_t   pm_device_handle;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == p_pmstate))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_osdep_chk_core_info(core_info);
    }

    /* Check to open the device */
    if (IMPDRV_EC_OK == ercd)
    {
        if (NULL == p_device_handle->handle)
        {
            /** Open the Hardware resource of core */
            pm_device_handle.handle = NULL;
            if (IMPDRV_CORE_TYPE_DSP == core_info.core_type)
            {
                /* force open subdevice 0 */
                ercd = impdrv_osdep_dev_open_dsp(&pm_device_handle, core_info, 0u);
            }
            else
            {
                ercd = impdrv_osdep_dev_open_imp(&pm_device_handle, core_info);
            }
            /* Save Device Open State */
            if (IMPDRV_EC_OK == ercd)
            {
                device_open = true;
                p_pm_device_handle = &pm_device_handle;
            }
        }
        else
        {
            p_pm_device_handle = (st_impdrv_device_handle_t*)p_device_handle;
        }
    }

    /* Get of Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_PmGetState(p_pm_device_handle->handle, &osal_state);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }
        else
        {
            if (OSAL_PM_STATE_READY == osal_state)
            {
                *p_pmstate = IMPDRV_PM_STATE_READY;
            }
            else if (OSAL_PM_STATE_RESET == osal_state)
            {
                *p_pmstate = IMPDRV_PM_STATE_RESET;
            }
            else if (OSAL_PM_STATE_ENABLED == osal_state)
            {
                *p_pmstate = IMPDRV_PM_STATE_HP;
            }
            else if (OSAL_PM_STATE_CG == osal_state)
            {
                *p_pmstate = IMPDRV_PM_STATE_CG;
            }
            else if (OSAL_PM_STATE_PG == osal_state)
            {
                *p_pmstate = IMPDRV_PM_STATE_PG;
            }
            else
            {
                /* unexpected state return from OSAL */
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }
    }

    /* Check if the device is open */
    if (true == device_open)
    {
        /** Close the Hardware resource of OS Abstraction layer */
        ercd_sub = impdrv_osdep_dev_close(p_pm_device_handle);

        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_sub;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_write_reg()
* CD_PD_02_05_0011
* [Covers: UD_PD_UD02_05_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_write_reg(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const uint32_t                      offset,
    const uint32_t                      val,
    const bool                          read_back,
    const uint32_t                      chk_val
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    uint32_t                read_val = 0U;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if ((IMPDRV_REG_OFFSET_MAX <= offset) || (0U != (offset & IMPDRV_CHK_MEM_ALIGN)))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if ((true != read_back) && (false != read_back))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Write register */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_IoWrite32(p_device_handle->handle, (uintptr_t)offset, val);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Check value */
    if ((IMPDRV_EC_OK == ercd) && (true == read_back))
    {
        ercd = impdrv_osdep_read_reg(p_device_handle, offset, &read_val);
        if (IMPDRV_EC_OK == ercd)
        {
            if (chk_val != read_val)
            {
                ercd = IMPDRV_EC_NG_CHECKFAIL;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_write_reg()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_read_reg()
* CD_PD_02_05_0012
* [Covers: UD_PD_UD02_05_0012]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_read_reg(
    const st_impdrv_device_handle_t *const p_device_handle,
    const uint32_t                  offset,
    uint32_t                        *p_val
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    uint32_t                lRegData = 0U;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == p_val))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (IMPDRV_REG_OFFSET_MAX <= offset)
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
        lRetOsal = R_OSAL_IoRead32(p_device_handle->handle, (uintptr_t)offset, &lRegData);
        if (OSAL_RETURN_OK == lRetOsal)
        {
            /** Set the read register value */
            *p_val = lRegData;

        }
        else
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_read_reg()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_mutex_create()
* CD_PD_02_05_0013
* [Covers: UD_PD_UD02_05_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_create(
    st_impdrv_mutex_handle_t    *const p_mutex_handle,
    const osal_mutex_id_t       mutex_id
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_mutex_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL != p_mutex_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Create the mutex resource */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request OSAL to create the mutex resource */
        lRetOsal = R_OSAL_ThsyncMutexCreate(mutex_id, &p_mutex_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_mutex_create()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_mutex_destroy()
* CD_PD_02_05_0014
* [Covers: UD_PD_UD02_05_0014]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_destroy(
    st_impdrv_mutex_handle_t    *const p_mutex_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_mutex_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_mutex_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Destroy the mutex resource */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request OSAL to destroy the mutex resource */
        lRetOsal = R_OSAL_ThsyncMutexDestroy(p_mutex_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
        else
        {
            /** Clear handle data */
            p_mutex_handle->handle = NULL;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_mutex_destroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_mutex_lock()
* CD_PD_02_05_0015
* [Covers: UD_PD_UD02_05_0015]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_lock(
    const st_impdrv_mutex_handle_t  *const p_mutex_handle,
    const osal_milli_sec_t          time_period
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_mutex_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_mutex_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if (0 > time_period)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Lock the mutex */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request OSAL to lock the mutex with timeout */
        lRetOsal = R_OSAL_ThsyncMutexLockForTimePeriod(p_mutex_handle->handle, time_period);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_mutex_lock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_mutex_unlock()
* CD_PD_02_05_0016
* [Covers: UD_PD_UD02_05_0016]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_mutex_unlock(
    const st_impdrv_mutex_handle_t  *const p_mutex_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_mutex_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_mutex_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Release mutex lock */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request OSAL to release mutex lock */
        lRetOsal = R_OSAL_ThsyncMutexUnlock(p_mutex_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_mutex_unlock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_reset_core()
* CD_PD_02_05_0025
* [Covers: UD_PD_UD02_05_0025]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_reset_core(
    const st_impdrv_device_handle_t     *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Reset IMP core */
    if (IMPDRV_EC_OK == ercd)
    {
        /* set of Power management reset state apply */
        lRetOsal = R_OSAL_PmSetResetState(p_device_handle->handle, OSAL_PM_RESET_STATE_APPLIED);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }

        /* set of Power management reset state release */
        lRetOsal = R_OSAL_PmSetResetState(p_device_handle->handle, OSAL_PM_RESET_STATE_RELEASED);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_reset_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_on_hwrsc()
* CD_PD_02_05_0026
* [Covers: UD_PD_UD02_05_0026]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_hwrsc(
    st_impdrv_device_handle_t           *const p_device_handle,
    const char                          *const p_device_id
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == p_device_id))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL != p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Open the Hardware resource of hardware resource */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request OSAL to open the hardware resource */
        lRetOsal = R_OSAL_IoDeviceOpen(p_device_id, &p_device_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** Error Handring */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }

        /** Power on hardware resource */
        if (IMPDRV_EC_OK == ercd)
        {
            /** Request osdep power on hardware resource */
            ercd = impdrv_osdep_pow_on(p_device_handle);
            if (IMPDRV_EC_OK != ercd)
            {
                /** Recovery in case of power on failed */
                (void)impdrv_osdep_dev_close(p_device_handle);
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_on_hwrsc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_off_hwrsc()
* CD_PD_02_05_0027
* [Covers: UD_PD_UD02_05_0027]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_off_hwrsc(
    st_impdrv_device_handle_t           *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Power OFF hardware resource */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_pow_off(p_device_handle);
        /** Close the Hardware resource of OS Abstraction layer */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_dev_close(p_device_handle);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_off_hwrsc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_memory_barrier()
* CD_PD_02_05_0028
* [Covers: UD_PD_UD02_05_0028]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_barrier(
    void
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Call the OSAL dependent memory barrier */
    lRetOsal = R_OSAL_ThsyncMemoryBarrier();
    if (OSAL_RETURN_OK != lRetOsal)
    {
        /** Error Handring */
        ercd = IMPDRV_EC_NG_SYSTEMERROR;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_memory_barrier()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_queue_create()
* CD_PD_02_05_0029
* [Covers: UD_PD_UD02_05_0029]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_queue_create(
    st_impdrv_queue_handle_t    *const p_queue_handle,
    const osal_mq_id_t          queue_id
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    st_osal_mq_config_t     lCfgQueue = {
        .max_num_msg    = IMPDRV_QUEUE_MAX_NUMBER,
        .msg_size       = IMPDRV_QUEUE_MAX_MSG_SIZE
    };

    /** Input parameter confirmation process */
    if (NULL == p_queue_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL != p_queue_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if ((osal_mq_id_t)0UL == queue_id)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Create message queue. */
        lRetOsal = R_OSAL_MqCreate(&lCfgQueue, queue_id, &(p_queue_handle->handle));
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** Error Handring */
            if (OSAL_RETURN_ID == lRetOsal)
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }
            else
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_queue_create()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_queue_destroy()
* CD_PD_02_05_0030
* [Covers: UD_PD_UD02_05_0030]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_queue_destroy(
    st_impdrv_queue_handle_t    *const p_queue_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_queue_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_queue_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Reset the message queue. */
        lRetOsal = R_OSAL_MqReset(p_queue_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** Error Handring */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
        else
        {
            /* Delete the message queue */
            lRetOsal = R_OSAL_MqDelete(p_queue_handle->handle);
            if (OSAL_RETURN_OK != lRetOsal)
            {
                /** Error Handring */
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
            else
            {
                /** Clear the handle of message queue. */
                p_queue_handle->handle = NULL;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_queue_destroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_queue_wait_period()
* CD_PD_02_05_0031
* [Covers: UD_PD_UD02_05_0031]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_queue_wait_period(
    const st_impdrv_queue_handle_t  *const p_queue_handle,
    const osal_milli_sec_t          time_period,
    uint32_t                        *const p_receive_buffer
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if ((NULL == p_queue_handle) || (NULL == p_receive_buffer))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_queue_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if (0 > time_period)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Wait for receive message. */
        lRetOsal = R_OSAL_MqReceiveForTimePeriod(
            p_queue_handle->handle,
            time_period,
            p_receive_buffer,
            IMPDRV_QUEUE_MAX_MSG_SIZE
        );
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** Error Handring */
            if (OSAL_RETURN_TIME == lRetOsal)
            {
                ercd = IMPDRV_EC_NG_CHECKFAIL;
            }
            else
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_queue_wait_period()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_queue_send_period()
* CD_PD_02_05_0032
* [Covers: UD_PD_UD02_05_0032]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_queue_send_period(
    const st_impdrv_queue_handle_t  *const p_queue_handle,
    const osal_milli_sec_t          time_period,
    const uint32_t                  *const p_send_buffer
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if ((NULL == p_queue_handle) || (NULL == p_send_buffer))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_queue_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if (0 > time_period)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Send to message queue. */
        lRetOsal = R_OSAL_MqSendForTimePeriod(
            p_queue_handle->handle,
            time_period,
            p_send_buffer,
            IMPDRV_QUEUE_MAX_MSG_SIZE
        );
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** Error Handring */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_queue_send_period()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dev_open_imp()
* CD_PD_02_05_0046
* [Covers: UD_PD_UD02_05_0046]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_dev_open_imp(
    st_impdrv_device_handle_t   *const p_device_handle,
    const st_impdrv_core_info_t core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    const char*             type;
    uint32_t                core_num = core_info.core_num;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_osdep_chk_core_info(core_info);
    }

    /** Open the Hardware resource of core */
    if (IMPDRV_EC_OK == ercd)
    {
        type = sp_type_cnv_table[core_info.core_type];
        ercd = impdrv_osdep_dev_open(p_device_handle, type, core_num);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dev_open_imp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dev_close()
* CD_PD_02_05_0018
* [Covers: UD_PD_UD02_05_0018]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_dev_close(
    st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Request OSAL to close the core */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_IoDeviceClose(p_device_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
        else
        {
            /** Clear handle data */
            p_device_handle->handle = NULL;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dev_close()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dev_open_dsp()
* CD_PD_02_05_0045
* [Covers: UD_PD_UD02_05_0045]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_dev_open_dsp(
    st_impdrv_device_handle_t   *const p_device_handle,
    const st_impdrv_core_info_t core_info,
    const uint32_t              sub_dev_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    const char*             type;
    uint32_t                core_num = core_info.core_num;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_osdep_chk_core_info(core_info);
    }

    /** Open the Hardware resource of core */
    if (IMPDRV_EC_OK == ercd)
    {
        type = sp_type_cnv_table[core_info.core_type];
        ercd = impdrv_osdep_dev_open_dsp_sub(p_device_handle, type, core_num, sub_dev_num);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dev_open_dsp()
***********************************************************************************************************************/
/*======================================================================================================================
Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_osdep_chk_core_info()
* CD_PD_02_05_0022
* [Covers: UD_PD_UD02_05_0022]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_chk_core_info(
    const st_impdrv_core_info_t     core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Check the Input value of core information */
    if ((IMPDRV_CORE_TYPE_IMP != core_info.core_type)
        && (IMPDRV_CORE_TYPE_IMP_SLIM != core_info.core_type)
        && (IMPDRV_CORE_TYPE_OCV != core_info.core_type)
        && (IMPDRV_CORE_TYPE_DMAC != core_info.core_type)
        && (IMPDRV_CORE_TYPE_DMAC_SLIM != core_info.core_type)
        && (IMPDRV_CORE_TYPE_PSCEXE != core_info.core_type)
        && (IMPDRV_CORE_TYPE_CNN != core_info.core_type)
        && (IMPDRV_CORE_TYPE_DTA != core_info.core_type)
        && (IMPDRV_CORE_TYPE_DSP != core_info.core_type))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (IMPDRV_CORE_NUM_MAX <= core_info.core_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do */
    }

    /** Set error code in return value */
    return ercd;
}

/***********************************************************************************************************************
* End of function impdrv_osdep_chk_core_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_chk_instance_num()
* CD_PD_02_05_0023
* [Covers: UD_PD_UD02_05_0024]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_chk_instance_num(
    const e_impdrv_instance_t       instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Check the Instance number value */
    if ((IMPDRV_INSTANCE_0 != instance_num)
        && (IMPDRV_INSTANCE_1 != instance_num)
        && (IMPDRV_INSTANCE_2 != instance_num)
        && (IMPDRV_INSTANCE_3 != instance_num)
        && (IMPDRV_INSTANCE_4 != instance_num)
        && (IMPDRV_INSTANCE_5 != instance_num)
        && (IMPDRV_INSTANCE_6 != instance_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_chk_instance_num()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_get_irq_channel()
* CD_PD_02_05_0024
* [Covers: UD_PD_UD02_05_0023]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_get_irq_channel(
    const e_impdrv_instance_t    instance_num,
    uint64_t                     *const irq_channel
)
{
    /** Private static variables */
    IMPDRV_STATIC
    const uint64_t s_impdrv_irq_channel_table[IMPDRV_IRQ_CHANNEL_MAX + 1U] =
    {
        /** IRQ channel number */
        IMPDRV_DUMMY_UINT64,    /**< Dummy for Index adjustment */
        IMPDRV_IRQ_CHANNEL_0,   /**< IRQ channel number (0)     */
        IMPDRV_IRQ_CHANNEL_1,   /**< IRQ channel number (1)     */
        IMPDRV_IRQ_CHANNEL_2,   /**< IRQ channel number (2)     */
        IMPDRV_IRQ_CHANNEL_3,   /**< IRQ channel number (3)     */
        IMPDRV_IRQ_CHANNEL_4,   /**< IRQ channel number (4)     */
        IMPDRV_IRQ_CHANNEL_5,   /**< IRQ channel number (5)     */
        IMPDRV_IRQ_CHANNEL_6    /**< IRQ channel number (6)     */
    };

    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == irq_channel)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_osdep_chk_instance_num(instance_num);
    }

     /** Get irq channel */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Set irq channel */
        *irq_channel = s_impdrv_irq_channel_table[instance_num];
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_get_irq_channel()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dev_open()
* CD_PD_02_05_0017
* [Covers: UD_PD_UD02_05_0017]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_dev_open(
    st_impdrv_device_handle_t   *const p_device_handle,
    const char                  *const type,
    const size_t                dev_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    char                    devid_list[IMPDRV_DEV_LIST_SIZE];
    const char              num_to_char[] = "0123456789";
    uint32_t                index;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == type))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL != p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if (IMPDRV_CORE_NUM_MAX <= dev_num)
    {
        ercd = IMPDRV_EC_NG_SYSTEMERROR;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Create name to open the device */
    if (IMPDRV_EC_OK == ercd)
    {
        /* Copy type string */
        index = 0U;
        while ('\0' != type[index])
        {
            if ((IMPDRV_DEV_LIST_SIZE - IMPDRV_ADD_CORE_DEVICE_ID) <= index)
            {
                ercd = IMPDRV_EC_NG_PARAM;
                break;
            }
            devid_list[index] = type[index];
            index++;
        }

        /* Add core number */
        if (IMPDRV_EC_OK == ercd)
        {
            devid_list[index] = '_';
            index++;
            devid_list[index] = num_to_char[dev_num / 10U];
            index++;
            devid_list[index] = num_to_char[dev_num % 10U];
            index++;
            devid_list[index] = '\0';
        }
    }

    /** Open the core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request OSAL to open the core */
        lRetOsal = R_OSAL_IoDeviceOpen((const char*)&devid_list[0], &p_device_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** Error Handring */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dev_open()
***********************************************************************************************************************/

/************************************************************************************************************************
* Start of function impdrv_osdep_dev_open_dsp_sub()
* CD_PD_02_05_xxxx
* [Covers: UD_PD_UD02_05_0017]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_dev_open_dsp_sub(
    st_impdrv_device_handle_t   *const p_device_handle,
    const char                  *const type,
    const size_t                dev_num,
    const uint32_t              sub_dev_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    char                    devid_list[IMPDRV_DEV_LIST_SIZE];
    const char              num_to_char[] = "0123456789";
    uint32_t                index;

    /** Input parameter confirmation process */
    if ((NULL == p_device_handle) || (NULL == type))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL != p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if (IMPDRV_CORE_NUM_MAX <= dev_num)
    {
        ercd = IMPDRV_EC_NG_SYSTEMERROR;
    }
    else if (IMPDRV_DSP_SUB_DEV_NUM_MAX <= sub_dev_num)
    {
        ercd = IMPDRV_EC_NG_SYSTEMERROR;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Create name to open the device */
    if (IMPDRV_EC_OK == ercd)
    {
        /* Copy type string */
        index = 0U;
        while ('\0' != type[index])
        {
            /* ex: vdsp00_00 -> 00_00 (+\0) is additional device id */
            if ((IMPDRV_DEV_LIST_SIZE - IMPDRV_ADD_CORE_DEVICE_ID_DSP) <= index)
            {
                ercd = IMPDRV_EC_NG_PARAM;
                break;
            }
            devid_list[index] = type[index];
            index++;
        }

        /* Add core number */
        if (IMPDRV_EC_OK == ercd)
        {
            devid_list[index] = num_to_char[dev_num / 10U];
            index++;
            devid_list[index] = num_to_char[dev_num % 10U];
            index++;
            devid_list[index] = '_';
            index++;
            devid_list[index] = num_to_char[sub_dev_num / 10U];
            index++;
            devid_list[index] = num_to_char[sub_dev_num % 10U];
            index++;
            devid_list[index] = '\0';
        }
    }

    /** Open the core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request OSAL to open the core */
        lRetOsal = R_OSAL_IoDeviceOpen((const char*)&devid_list[0], &p_device_handle->handle);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** Error Handring */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dev_open_dsp_sub()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_on()
* CD_PD_02_05_0019
* [Covers: UD_PD_UD02_05_0019]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_pow_on(
    const st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_osal_return_t             lRetOsal;
    e_osal_pm_required_state_t  lState;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Check required state */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_PmGetRequiredState(p_device_handle->handle, &lState);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }
    }

    /** Request OSAL to set power management control(REQUIRED) */
    if ((IMPDRV_EC_OK == ercd) && (OSAL_PM_REQUIRED_STATE_REQUIRED != lState))
    {
        lRetOsal = R_OSAL_PmSetRequiredState(p_device_handle->handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_on()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_off()
* CD_PD_02_05_0020
* [Covers: UD_PD_UD02_05_0020]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_pow_off(
    const st_impdrv_device_handle_t   *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_osal_return_t             lRetOsal;
    e_osal_pm_required_state_t  lState;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Check required state */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_PmGetRequiredState(p_device_handle->handle, &lState);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }
    }

    /** Power OFF core */
    if ((IMPDRV_EC_OK == ercd) && (OSAL_PM_REQUIRED_STATE_RELEASED != lState))
    {
        /** Request OSAL to set power management control(RELEASE) */
        lRetOsal = R_OSAL_PmSetRequiredState(p_device_handle->handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_PMSYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_off()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_int_handler()
* CD_PD_02_05_0021
* [Covers: UD_PD_UD02_05_0021]
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    s_in_interrupts = 1U;

    /** Input parameter confirmation process */
    if ((NULL == device_handle) || (NULL == irq_arg))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** call the interrupt handler of general control */
    impdrv_genctl_int_handler(
        (st_impdrv_ctl_t *const)irq_arg,
        irq_channel,
        ercd
    );

    s_in_interrupts = 0U;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dsp0_int_handler()
* CD_PD_02_05_0xx0
* [Covers: UD_PD_UD02_05_00xx]
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_dsp0_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_core_info_t   core_info;
    core_info.core_type = IMPDRV_CORE_TYPE_DSP;
    core_info.core_num  = 0U;

    s_in_interrupts = 1U;

    /** Input parameter confirmation process */
    if ((NULL == device_handle) || (NULL == irq_arg))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** call the interrupt handler of general control */
    impdrv_genctl_dsp_int_handler(
        (st_impdrv_ctl_t *const)irq_arg,
        irq_channel,
        ercd,
        core_info
    );

    s_in_interrupts = 0U;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dsp0_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dsp1_int_handler()
* CD_PD_02_05_0xx0
* [Covers: UD_PD_UD02_05_00xx]
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_dsp1_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_core_info_t   core_info;
    core_info.core_type = IMPDRV_CORE_TYPE_DSP;
    core_info.core_num  = 1U;

    s_in_interrupts = 1U;

    /** Input parameter confirmation process */
    if ((NULL == device_handle) || (NULL == irq_arg))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** call the interrupt handler of general control */
    impdrv_genctl_dsp_int_handler(
        (st_impdrv_ctl_t *const)irq_arg,
        irq_channel,
        ercd,
        core_info
    );

    s_in_interrupts = 0U;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dsp1_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dsp2_int_handler()
* CD_PD_02_05_0xx0
* [Covers: UD_PD_UD02_05_00xx]
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_dsp2_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_core_info_t   core_info;
    core_info.core_type = IMPDRV_CORE_TYPE_DSP;
    core_info.core_num  = 2U;

    s_in_interrupts = 1U;

    /** Input parameter confirmation process */
    if ((NULL == device_handle) || (NULL == irq_arg))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** call the interrupt handler of general control */
    impdrv_genctl_dsp_int_handler(
        (st_impdrv_ctl_t *const)irq_arg,
        irq_channel,
        ercd,
        core_info
    );

    s_in_interrupts = 0U;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dsp2_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_dsp3_int_handler()
* CD_PD_02_05_0xx0
* [Covers: UD_PD_UD02_05_00xx]
***********************************************************************************************************************/
IMPDRV_STATIC
void impdrv_osdep_dsp3_int_handler(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_core_info_t   core_info;
    core_info.core_type = IMPDRV_CORE_TYPE_DSP;
    core_info.core_num  = 3U;

    s_in_interrupts = 1U;

    /** Input parameter confirmation process */
    if ((NULL == device_handle) || (NULL == irq_arg))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** call the interrupt handler of general control */
    impdrv_genctl_dsp_int_handler(
        (st_impdrv_ctl_t *const)irq_arg,
        irq_channel,
        ercd,
        core_info
    );

    s_in_interrupts = 0U;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_dsp3_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_get_dsp_irq_channel()
* CD_PD_02_05_00xx
* [Covers: UD_PD_UD02_05_00xx]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_osdep_get_dsp_irq_channel(
    uint64_t                     *const irq_channel
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == irq_channel)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /* nothing to do */
    }

     /** Get irq channel */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Set irq channel0 */
        *irq_channel = IMPDRV_IRQ_CHANNEL_0;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_get_dsp_irq_channel()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_enable_dsp_irq()
* CD_PD_02_05_00xx
* [Covers: UD_PD_UD02_05_00xx]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_enable_dsp_irq(
    const st_impdrv_device_handle_t *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    uint64_t                irq_channel;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        ercd = impdrv_osdep_get_dsp_irq_channel(&irq_channel);
    }

    /** Request OSAL to enable interrupt function already registered */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_InterruptEnableIsr(p_device_handle->handle, irq_channel);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_enable_dsp_irq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_disable_dsp_irq()
* CD_PD_02_05_00xx
* [Covers: UD_PD_UD02_05_00xx]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_disable_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    uint64_t                irq_channel;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_osdep_get_dsp_irq_channel(&irq_channel);
    }

    /** call R_OSAL_InterruptDisableIsr() Disable interrupt function already registered */
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_InterruptDisableIsr(p_device_handle->handle, irq_channel);
        /** check error code */
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** set RCV_EC_NG_DRV_SYSTEMERROR in ercd */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** return code return ercd */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_disable_dsp_irq()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_pow_on_imp_dsp()
* CD_PD_02_05_xxxx
* [Covers: UD_PD_UD02_05_xxxx]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_pow_on_imp_dsp(
    st_impdrv_device_handle_t   *const p_device_handle,
    const st_impdrv_core_info_t core_info,
    const uint32_t              sub_dev_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_osdep_chk_core_info(core_info);
    }

    /** Open the Hardware resource of core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_dev_open_dsp(p_device_handle, core_info, sub_dev_num);

        /** Power on core */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_pow_on(p_device_handle);
            if (IMPDRV_EC_OK != ercd)
            {
                /** Recovery in case of power on failed */
                (void)impdrv_osdep_dev_close(p_device_handle);
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_pow_on_imp_dsp()
***********************************************************************************************************************/


/***********************************************************************************************************************
* Start of function impdrv_osdep_unregister_dsp_irq()
* CD_PD_02_05_00xx
* [Covers: UD_PD_UD02_05_00xx]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_unregister_dsp_irq(
    const st_impdrv_device_handle_t     *const p_device_handle,
    const uint32_t                      core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;
    uint64_t                irq_channel;
    uint32_t                lWaitTime;
    p_osal_isr_func_t           int_handler;

    for (lWaitTime = 0U; (0U != s_in_interrupts) && (100U > lWaitTime); lWaitTime++)
    {
        lRetOsal = R_OSAL_ThreadSleepForTimePeriod(100U);
        if (OSAL_RETURN_OK != lRetOsal)
        {
            break;
        }
    }

    /** Input parameter confirmation process */
    if (NULL == p_device_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_device_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        ercd = impdrv_osdep_get_dsp_irq_channel(&irq_channel);
    }

    /** call R_OSAL_InterruptUnregisterIsr() Unregister a function called when interrupt */
    if (IMPDRV_EC_OK == ercd)
    {
        switch (core_num)
        {
            case IMPDRV_DSP_CORE_0:
                int_handler = impdrv_osdep_dsp0_int_handler;
                break;
            case IMPDRV_DSP_CORE_1:
                int_handler = impdrv_osdep_dsp1_int_handler;
                break;
            case IMPDRV_DSP_CORE_2:
                int_handler = impdrv_osdep_dsp2_int_handler;
                break;
            case IMPDRV_DSP_CORE_3:
                int_handler = impdrv_osdep_dsp3_int_handler;
                break;
            default:
                /** Error Handle */
                ercd = IMPDRV_EC_NG_PARAM;
                break;
        }
    }
    if (IMPDRV_EC_OK == ercd)
    {
        lRetOsal = R_OSAL_InterruptUnregisterIsr(p_device_handle->handle, irq_channel, int_handler);
        /** check error code */
        if (OSAL_RETURN_OK != lRetOsal)
        {
            /** set RCV_EC_NG_DRV_SYSTEMERROR in ercd */
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** return code return ercd */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_unregister_dsp_irq()
***********************************************************************************************************************/
/*======================================================================================================================
End of File
======================================================================================================================*/
