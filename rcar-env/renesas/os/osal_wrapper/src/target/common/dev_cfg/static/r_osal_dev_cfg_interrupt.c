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
* File Name :    r_osal_dev_cfg_interrupt.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configuration Functions for Interrupt Manager
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var gs_osal_interrupt_is_initialized_enu
 * @brief Interrupt Manager initial state flag.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_osal_interrupt_is_initialized_enu = OSAL_MANAGER_STATE_UNINITIALIZED;

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_041
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_InterruptGetInitStatus(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    return gs_osal_interrupt_is_initialized_enu;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_011]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_011]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_042
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_DCFG_InterruptSetInitStatus(e_osal_init_status_t status)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_interrupt_is_initialized_enu = status;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_043
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptControlInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_InterruptStaticInit(gp_osal_device_control, g_osal_num_of_device_table);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_003]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_003]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_044
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptControlDeinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_InterruptStaticDeinit(gp_osal_device_control, g_osal_num_of_device_table);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_002]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_002]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_045
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_InterruptCheckBusy();

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_046
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptRegisterIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptRegisterIsr(osal_device_handle_t handle,
                                                 uint64_t irq_channel,
                                                 e_osal_interrupt_priority_t priority_value,
                                                 p_osal_isr_func_t irqHandlerFunction,
                                                 void* p_irq_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 8 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (false == handle->active) ||
             (NULL == handle->dev_info) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (irq_channel >= handle->dev_info->irq_channels)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (OSAL_DEV_CFG_INTERRUPT_NONE_INDEX == handle->interrupt_index[irq_channel])
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = R_OSAL_OS_InterruptRegisterIsr(handle, irq_channel, priority_value, irqHandlerFunction, p_irq_arg);
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptRegisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_008]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_008]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_047
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptUnregisterIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptUnregisterIsr(osal_device_handle_t handle,
                                                   uint64_t irq_channel,
                                                   p_osal_isr_func_t irqHandlerFunction)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 8 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (false == handle->active) ||
             (NULL == handle->dev_info) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (irq_channel >= handle->dev_info->irq_channels)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (OSAL_DEV_CFG_INTERRUPT_NONE_INDEX == handle->interrupt_index[irq_channel])
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = R_OSAL_OS_InterruptUnregisterIsr(handle, irq_channel, irqHandlerFunction);
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptUnregisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_010]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_010]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_048
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptEnableIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptEnableIsr(osal_device_handle_t handle,
                                               uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 8 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (false == handle->active) ||
             (NULL == handle->dev_info) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (irq_channel >= handle->dev_info->irq_channels)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (OSAL_DEV_CFG_INTERRUPT_NONE_INDEX == handle->interrupt_index[irq_channel])
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = R_OSAL_OS_InterruptEnableIsr(handle, irq_channel);
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptEnableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_005]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_049
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptDisableIsr()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptDisableIsr(osal_device_handle_t handle,
                                                uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 8 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (false == handle->active) ||
             (NULL == handle->dev_info) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (irq_channel >= handle->dev_info->irq_channels)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (OSAL_DEV_CFG_INTERRUPT_NONE_INDEX == handle->interrupt_index[irq_channel])
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = R_OSAL_OS_InterruptDisableIsr(handle, irq_channel);
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptDisableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_004]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_050
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptIsISRContext()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptIsISRContext(bool *const p_is_isr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    bool            is_isr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_IsISRContext(&is_isr);
    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_is_isr = is_isr;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptIsISRContext()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_007]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_007]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_051
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptSetIsrPriority()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_InterruptSetIsrPriority(osal_device_handle_t handle,
                                                    uint64_t irq_channel,
                                                    e_osal_interrupt_priority_t priority_value)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 8 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (false == handle->active) ||
             (NULL == handle->dev_info) ||
             (NULL == handle->interrupt_index))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if (irq_channel >= handle->dev_info->irq_channels)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (OSAL_DEV_CFG_INTERRUPT_NONE_INDEX == handle->interrupt_index[irq_channel])
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = R_OSAL_OS_InterruptSetIsrPriority(handle, irq_channel, priority_value);
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptSetIsrPriority()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_009]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_009]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_INT_052
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_InterruptGetNumOfIrqChannels()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_InterruptGetNumOfIrqChannels(osal_device_handle_t handle,
                                                         size_t *const p_numOfChannels)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 9 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) ||
             (false == handle->active) ||
             (NULL == handle->dev_info))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret         = OSAL_RETURN_OK;
        *p_numOfChannels = handle->dev_info->irq_channels;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_InterruptGetNumOfIrqChannels()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_INT_006]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_006]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
