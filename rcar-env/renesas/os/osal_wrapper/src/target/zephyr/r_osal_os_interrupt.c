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
* File Name :    r_osal_os_interrupt.c
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Interrupt Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <device.h>
#include <devicetree.h>

#include "target/zephyr/r_osal_os_type.h"
#include "rcar-xos/osal_configuration/target/common/r_osal_dev_cfg_info.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/
#define DT_INTERRUPTS_TABLE(node_id, prop, idx) COND_CODE_1( DT_IRQ_HAS_IDX(node_id, idx), \
    ( {(uint64_t)(DT_IRQ_BY_IDX(node_id, idx, irq) - 32), (DT_LABEL(node_id)), (DT_IRQ_BY_IDX(node_id, idx, priority))}, ), \
    ( ) \
    )

#define DT_INTERRUPT_TABLE_PROC(node_id) COND_CODE_1( DT_NODE_HAS_PROP(node_id, interrupts), \
    (DT_FOREACH_PROP_ELEM(node_id, interrupts, DT_INTERRUPTS_TABLE)), () \
    )

/*******************************************************************************************************************//**
 * @def ISR_PRIORITY_LIMIT
 * ISR Driver maximum priority.
***********************************************************************************************************************/
#define ISR_PRIORITY_LIMIT (OSAL_INTERRUPT_PRIORITY_HIGHEST)

/*******************************************************************************************************************//**
 * @def ISR_SPI_OFFSET
 * Offset value for SPI number
***********************************************************************************************************************/
#define ISR_SPI_OFFSET (32U)

/*======================================================================================================================
 Private data types
======================================================================================================================*/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_014
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @typedef st_osal_interrupt_config_t
 * @brief typedef of st_osal_interrupt_config.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * @struct  st_osal_interrupt_config
 * @brief structure for configuration interrupt.
 **********************************************************************************************************************/
typedef struct st_osal_interrupt_config
{
    uint64_t                    irq_number;             /*!< IRQ number */
    const char                  *id;                    /*!< Device ID */
    e_osal_interrupt_priority_t priority;               /*!< interrupt priority */
} st_osal_interrupt_config_t;
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_026]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * @typedef st_osal_interrupt_control_t
 * @brief typedef of st_osal_interrupt_control.
 **********************************************************************************************************************/
typedef struct
{
    uint64_t              handle_id;        /*!< Fixed value: OSAL_INTERRUPT_HANDLE_ID */
    osal_device_handle_t  device_handle;    /*!< Handle of device according to interrupt */
    p_osal_isr_func_t     irq_handler;      /*!< Pointer of Worker function of Interrupt */
    void                  *irq_arg;         /*!< Pointer of Worker function argument */
    uint64_t              irq_channel;      /*!< irq number channel */
    bool                  irq_enable;       /*!< interrupt status (true: Enable interrupt, false: Disable interrupt) */
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                   event_id;         /*!< Attach event ID */

    const struct st_osal_interrupt_config *usr_config; /*!< User configuration for message queue */
} st_osal_interrupt_control_t;

/*======================================================================================================================
 Global variables
======================================================================================================================*/

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var gs_osal_os_interrupt_init_flag
 * @brief Interrupt Internal operating condition variable.
***********************************************************************************************************************/
OSAL_STATIC bool gs_osal_os_interrupt_init_flag = false;

/*******************************************************************************************************************//**
 * @var gs_osal_num_of_interrupt_table
 * @brief Number of the Interrupt control information table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_num_of_interrupt_table = 0;

/*******************************************************************************************************************//**
 * @var gsp_osal_interrupt_control
 * @brief The value for the user to operate the interrupt without being aware of the interrupt information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_interrupt_control_t *gsp_osal_interrupt_control = NULL;

/*******************************************************************************************************************//**
 * @var gs_osal_interrupt_config
 * @brief The interrupt configuration table to store the value obtained by DTS.
***********************************************************************************************************************/
/* PRQA S 3218 1 # Definition within a function reduces readability and maintainability, so no change. */
OSAL_STATIC const st_osal_interrupt_config_t  gs_osal_interrupt_config[] =
{
#ifndef OSAL_ZEPHYR_UT
    /* PRQA S 1296 2 # This implicit casting is no problem, because they are non-negative enum and positive integer. */
    /* PRQA S 1035 1 # Caused by the implementation of macros defined inside the Zephyr OS. No side effect. */
    DT_FOREACH_CHILD(DT_NODELABEL(osal_device), DT_INTERRUPT_TABLE_PROC)
#else
    {
        .irq_number     = 0,
        .id             = "ut_dummy_1",
        .priority       = OSAL_INTERRUPT_PRIORITY_LOWEST
    },
    {
        .irq_number     = 1,
        .id             = "ut_dummy_2",
        .priority       = OSAL_INTERRUPT_PRIORITY_LOWEST
    },
#endif
    {.id = OSAL_INTERRUPT_INVALID_ID}
};

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Initialize the interrupt information based on the device information.
* @param[in]        p_mutex_attr            The pointer of mutex attributes.
* @param[in]        device_table_num        The number of device table.
* @param[in,out]    p_device_control_table  The pointer to st_osal_device_control_t.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_proc(size_t device_table_num,
                                                          osal_device_table_t device_control_table);

/***********************************************************************************************************************
* @brief            Find interrupt control information.
* @param[in]        p_device_control  The pointer of device control information.
* @param[in]        channel_count     Channel count.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_find_control_info(st_osal_device_control_t *p_device_control,
                                                                  size_t channel_count);

/***********************************************************************************************************************
* @brief            Internal interrupt callback.
* @param[in]        p_arg  The pointer of Interrupt control information.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_interrupt_int_callback(const void *p_arg);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptStaticInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptStaticInit(osal_device_table_t device_control_table,
                                              size_t device_table_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t     osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_os_interrupt_init_flag = true;

    osal_ret = r_osal_os_interrupt_init_proc(device_table_num, device_control_table);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptStaticInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptStaticDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptStaticDeinit(osal_device_table_t device_control_table,
                                                size_t device_table_num)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret          = OSAL_RETURN_OK;
    size_t                      loop_count        = 0;
    st_osal_device_control_t    *p_device_control;
    st_osal_interrupt_control_t *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_os_interrupt_init_flag = false;

    if (NULL == device_control_table)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        if (NULL != gsp_osal_interrupt_control)
        {
            /* Deinitialize the interrupt control table.*/
            while (gs_osal_num_of_interrupt_table > loop_count)
            {
                p_interrupt_control = &gsp_osal_interrupt_control[loop_count];
                if ((uint64_t)OSAL_INTERRUPT_HANDLE_ID == p_interrupt_control->handle_id)
                {
                    p_interrupt_control->handle_id     = (uint64_t)OSAL_INVALID_INTERRUPT_HANDLE_ID;
                    p_interrupt_control->device_handle = NULL;
                }
                loop_count++;
            }

            /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
            /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
            free((void*)gsp_osal_interrupt_control);
            gsp_osal_interrupt_control = NULL;
        }
        else
        {
            /* Do nothing */
        }

        /* Free the interrupt index table of each device that has interrupt capability. */
        loop_count = 0;
        while (device_table_num > loop_count)
        {
            p_device_control = &device_control_table[loop_count];

            if ((0U != p_device_control->dev_info->irq_channels) &&
                (NULL != p_device_control->interrupt_index))
            {
                /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
                free((void*)p_device_control->interrupt_index);
                p_device_control->interrupt_index = NULL;
            }
            else
            {
                /* Do nothing */
            }
            loop_count++;
        }   /* end of while(device_table_num > loop_count). */

    }   /* end of if (NULL == device_control_table). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptStaticDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret   = OSAL_RETURN_OK;
    size_t                             loop_count = 0U;
    uint32_t                           key;
    st_osal_interrupt_control_t  const *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* Check if there is any active interrupt control. */
    while ((gs_osal_num_of_interrupt_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_interrupt_control = &gsp_osal_interrupt_control[loop_count];

        key = irq_lock();
        if (NULL != p_interrupt_control->irq_handler)
        {
            osal_ret = OSAL_RETURN_BUSY;
        }
        else
        {
            /* Do nothing */
        }

        irq_unlock(key);
        loop_count++;
    }   /* end of while((num_of_interrupt > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptRegisterIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptRegisterIsr(osal_device_handle_t handle,
                                               uint64_t irq_channel,
                                               e_osal_interrupt_priority_t priority_value,
                                               p_osal_isr_func_t irqHandlerFunction,
                                               void* p_irq_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    uint32_t                    key;
    uint16_t                    isr_priority;
    st_osal_interrupt_control_t *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)priority_value;   /* unused */

    p_interrupt_control = &gsp_osal_interrupt_control[handle->interrupt_index[irq_channel]];

    key = irq_lock();

    /* Check if the block is released after the de-initialization. */
    if (false == gs_osal_os_interrupt_init_flag)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if ((NULL != p_interrupt_control->irq_handler) ||
             (true == p_interrupt_control->irq_enable))
    {
        osal_ret = OSAL_RETURN_BUSY;
    }
    else
    {
        p_interrupt_control->irq_handler = irqHandlerFunction;
        p_interrupt_control->irq_arg     = p_irq_arg;

        if ((uint16_t)ISR_PRIORITY_LIMIT < ((uint16_t)(p_interrupt_control->usr_config->priority)))
        {
            /* Store the highest priority into isr_priority. */
            isr_priority = (uint16_t)0x00;
        }
        else
        {
            /* [7:4] of priority field bits */
            isr_priority = (uint16_t)(((uint16_t)ISR_PRIORITY_LIMIT - (uint16_t)(p_interrupt_control->usr_config->priority)) << 4);
        }

        /* PRQA S 5209 4 # The unsigned int type is required as arguments, so it cannot be changed. */
        /* PRQA S 0314 3 # The parameter must be (void *) and there is no side side-effect in this case. */
        (void)irq_connect_dynamic((unsigned int)p_interrupt_control->usr_config->irq_number + ISR_SPI_OFFSET,
                            isr_priority, r_osal_os_interrupt_int_callback, (void *)p_interrupt_control, 0);
    }

    irq_unlock(key);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptRegisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptUnregisterIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptUnregisterIsr(osal_device_handle_t handle,
                                                 uint64_t irq_channel,
                                                 p_osal_isr_func_t irqHandlerFunction)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    uint32_t                    key;
    st_osal_interrupt_control_t *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_interrupt_control = &gsp_osal_interrupt_control[handle->interrupt_index[irq_channel]];

    key = irq_lock();

    /* Check if the block is released after the de-initialization. */
    if (false == gs_osal_os_interrupt_init_flag)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (irqHandlerFunction != p_interrupt_control->irq_handler)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else if (true == p_interrupt_control->irq_enable)
    {
        osal_ret = OSAL_RETURN_BUSY;
    }
    else
    {
        p_interrupt_control->irq_handler = NULL;
        p_interrupt_control->irq_arg     = NULL;

        /* PRQA S 5209 4 # The unsigned int type is required as arguments, so it cannot be changed. */
        /* PRQA S 0314 3 # The parameter must be (void *) and there is no side side-effect in this case. */
        (void)irq_connect_dynamic((unsigned int)p_interrupt_control->usr_config->irq_number + ISR_SPI_OFFSET,
                            (unsigned int)ISR_PRIORITY_LIMIT, NULL,
                            (void *)p_interrupt_control, 0);
    }

    irq_unlock(key);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptUnregisterIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptEnableIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptEnableIsr(osal_device_handle_t handle,
                                             uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret = OSAL_RETURN_OK;
    uint32_t                     key;
    st_osal_interrupt_control_t  *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_interrupt_control = &gsp_osal_interrupt_control[handle->interrupt_index[irq_channel]];

    key = irq_lock();

    /* Check if the block is released after the de-initialization. */
    if (false == gs_osal_os_interrupt_init_flag)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == p_interrupt_control->irq_handler)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* do nothing */
    }

    /* Execute the interrupt enable setting in case that the current irq_enable is false.*/
    if ((OSAL_RETURN_OK == osal_ret) && (false == p_interrupt_control->irq_enable))
    {
        /* PRQA S 5209 3 # The unsigned int type is required as arguments, so it cannot be changed. */
        arch_irq_enable((unsigned int)p_interrupt_control->usr_config->irq_number + ISR_SPI_OFFSET);
        p_interrupt_control->irq_enable = true;
    }
    else
    {
        /*  Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    irq_unlock(key);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptEnableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptDisableIsr()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptDisableIsr(osal_device_handle_t handle,
                                              uint64_t irq_channel)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t              osal_ret = OSAL_RETURN_OK;
    uint32_t                     key;
    st_osal_interrupt_control_t  *p_interrupt_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_interrupt_control = &gsp_osal_interrupt_control[handle->interrupt_index[irq_channel]];

    key = irq_lock();

    /* Check if the block is released after the de-initialization. */
    if (false == gs_osal_os_interrupt_init_flag)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else if (NULL == p_interrupt_control->irq_handler)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }
    /* Execute the interrupt disable setting in case that the current irq_enable is true.*/
    if ((OSAL_RETURN_OK == osal_ret) && (true == p_interrupt_control->irq_enable))
    {
        /* PRQA S 5209 3 # The unsigned int type is required as arguments, so it cannot be changed. */
        arch_irq_disable((unsigned int)p_interrupt_control->usr_config->irq_number + ISR_SPI_OFFSET);
        p_interrupt_control->irq_enable = false;
    }
    else
    {
        /* Do nothing */
    }   /* end of if ((OSAL_RETURN_OK == osal_ret) && (true == p_interrupt_control->irq_enable)). */

    irq_unlock(key);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptDisableIsr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptSetIsrPriority()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_InterruptSetIsrPriority(osal_device_handle_t handle,
                                                  uint64_t irq_channel,
                                                  e_osal_interrupt_priority_t priority_value)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    /* PRQA S 3678 1 # Converted to the void pointer type, so no need to be const. */
    st_osal_interrupt_control_t *p_interrupt_control;
    uint32_t                    key;
    uint16_t                    isr_priority;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_interrupt_control = &gsp_osal_interrupt_control[handle->interrupt_index[irq_channel]];

    key = irq_lock();

    /* Check if the block is released after the de-initialization. */
    if (false == gs_osal_os_interrupt_init_flag)
    {
        osal_ret = OSAL_RETURN_STATE;
    }
    else
    {
        if ((uint16_t)ISR_PRIORITY_LIMIT < (uint16_t)(priority_value))
        {
            /* Store the highest priority into isr_priority. */
            isr_priority = (uint16_t)0x00;
        }
        else
        {
            /* [7:4] of priority field bits */
            isr_priority = (uint16_t)(((uint16_t)ISR_PRIORITY_LIMIT - (uint16_t)(priority_value)) << 4);
        }

        /* PRQA S 5209 4 # The unsigned int type is required as arguments, so it cannot be changed. */
        /* PRQA S 0314 3 # The parameter must be (void *) and there is no side side-effect in this case. */
        (void)irq_connect_dynamic((unsigned int)p_interrupt_control->usr_config->irq_number + ISR_SPI_OFFSET,
                            isr_priority, r_osal_os_interrupt_int_callback,
                            (void *)p_interrupt_control, 0);
    }   /* end of if (false == gs_osal_os_interrupt_init_flag). */

    irq_unlock(key);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptSetIsrPriority()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_009
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_InterruptIsIsrHandler()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptIsIsrHandler(uint32_t num,
                                                bool* p_result)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                    osal_ret = OSAL_RETURN_OK;
    bool                               is_isr_handler_ret   = false;
    st_osal_interrupt_control_t  const *p_interrupt_control;
    uint32_t                           key;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_result)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        p_interrupt_control = &gsp_osal_interrupt_control[num];

        key = irq_lock();

        /* Check if the block is released after the de-initialization. */
        if (false == gs_osal_os_interrupt_init_flag)
        {
            osal_ret = OSAL_RETURN_STATE;
        }
        else
        {
            if (NULL != p_interrupt_control->irq_handler)
            {
                is_isr_handler_ret = true;
            }
            else
            {
                is_isr_handler_ret = false;
            }   /* end of if (NULL != p_interrupt_control->irq_handler). */
        }   /* end of if (false == gs_osal_os_interrupt_init_flag). */

        irq_unlock(key);

        *p_result = is_isr_handler_ret;
    }   /* end of if (NULL == p_result). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_InterruptIsIsrHandler()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_INT_013
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_int_callback()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_interrupt_int_callback(const void *p_arg)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    uint32_t key;
    /* PRQA S 0316 1 # There is no problem because it is restored to the type before calling this function. */
    const st_osal_interrupt_control_t *p_interrupt_control = (const st_osal_interrupt_control_t *)p_arg;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_interrupt_control)
    {
        key = irq_lock();

        /* PRQA S 5209 3 # The unsigned int type is required as arguments, so it cannot be changed. */
        arch_irq_disable((unsigned int)p_interrupt_control->usr_config->irq_number + ISR_SPI_OFFSET);
        irq_unlock(key);

        p_interrupt_control->irq_handler(p_interrupt_control->device_handle, p_interrupt_control->irq_channel, p_interrupt_control->irq_arg);

        key = irq_lock();

        /* In case that irq_enable is set to false in irq_handler, the irq is not re-enabled. */
        if (true == p_interrupt_control->irq_enable)
        {
            /* PRQA S 5209 3 # The unsigned int type is required as arguments, so it cannot be changed. */
            arch_irq_enable((unsigned int)p_interrupt_control->usr_config->irq_number + ISR_SPI_OFFSET);
        }
        else
        {
            /* Do nothing */
        }
        irq_unlock(key);
    }
    else
    {
        /* Do nothing */
    }

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_int_callback()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_INT_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_init_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_init_proc(size_t device_table_num,
                                                          osal_device_table_t device_control_table)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret           = OSAL_RETURN_OK;
    size_t                   loop_count;
    size_t                   device_count       = 0;
    size_t                   channel_count;
    st_osal_device_control_t *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* Count up the number of valid items in the interrupt config table. */
    gs_osal_num_of_interrupt_table = 0;
    while (OSAL_INTERRUPT_INVALID_ID != gs_osal_interrupt_config[gs_osal_num_of_interrupt_table].id)
    {
        gs_osal_num_of_interrupt_table++;
    }

    /* Set up the interrupt control table. */
    if (0U != gs_osal_num_of_interrupt_table)
    {
        /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
        gsp_osal_interrupt_control = (st_osal_interrupt_control_t *)calloc(1,
                                     (sizeof(st_osal_interrupt_control_t) * gs_osal_num_of_interrupt_table));
        if (NULL == gsp_osal_interrupt_control)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            for (loop_count = 0; gs_osal_num_of_interrupt_table > loop_count; loop_count++)
            {
                gsp_osal_interrupt_control[loop_count].usr_config    = &gs_osal_interrupt_config[loop_count];
                gsp_osal_interrupt_control[loop_count].handle_id     = (uint64_t)OSAL_INVALID_INTERRUPT_HANDLE_ID;
            }
        }
    }
    else
    {
        osal_ret = OSAL_RETURN_MEM;
    }   /* end of if (0U != gs_osal_num_of_interrupt_table). */

    /* Set up the interrupt index table of each device that has interrupt capability. */
    while ((device_table_num > device_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_device_control = &device_control_table[device_count];
        p_device_control->interrupt_index = NULL;

        if (0U != p_device_control->dev_info->irq_channels)
        {
            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            p_device_control->interrupt_index = (int32_t *)calloc(1, (sizeof(int32_t) * p_device_control->dev_info->irq_channels));
            if (NULL == p_device_control->interrupt_index)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                channel_count = 0;
                while ((p_device_control->dev_info->irq_channels > channel_count) && (OSAL_RETURN_OK == osal_ret))
                {
                    /* Set up the each item of the interrupt index table. */
                    osal_ret = r_osal_os_interrupt_find_control_info(p_device_control, channel_count);
                    channel_count++;
                }
            }
        }
        else
        {
            /* Do nothing */
        }
        device_count++;
    }   /* end of while((device_table_num > device_count) && (OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_init_proc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_osal_os_interrupt_find_control_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_interrupt_find_control_info(st_osal_device_control_t *p_device_control,
                                                                  size_t channel_count)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;
    size_t                      interrupt_count;
    st_osal_interrupt_control_t *p_interrupt_control;
    st_osal_interrupt_control_t *p_local_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* Search the table by id to find the corresponding interrupt control. */
    interrupt_count     = 0;
    p_interrupt_control = NULL;
    while ((gs_osal_num_of_interrupt_table > interrupt_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_local_control = &gsp_osal_interrupt_control[interrupt_count];

        if (p_device_control->dev_info->interrupt[channel_count].irq_number == p_local_control->usr_config->irq_number)
        {
            os_ret = strncmp(p_device_control->dev_info->id,
                             p_local_control->usr_config->id,
                             (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);

            if ((0 == os_ret) && (NULL == p_interrupt_control))
            {
                p_interrupt_control = p_local_control;
                p_interrupt_control->irq_channel = (uint64_t)channel_count;
                p_device_control->interrupt_index[channel_count] = (int32_t)interrupt_count;

                if (OSAL_INTERRUPT_PRIORITY_HIGHEST < p_interrupt_control->usr_config->priority)
                {
                    osal_ret = OSAL_RETURN_CONF;
                }
                else
                {
                    p_interrupt_control->handle_id     = (uint64_t)OSAL_INTERRUPT_HANDLE_ID;
                    p_interrupt_control->device_handle = p_device_control;
                    p_interrupt_control->irq_handler   = NULL;
                    p_interrupt_control->irq_arg       = NULL;
                    p_interrupt_control->irq_enable    = false;
                }   /* end of if((OSAL_INTERRUPT_PRIORITY_HIGHEST < p_info->usr_config->priority)  ||
                                 (OSAL_INTERRUPT_PRIORITY_LOWEST > p_info->usr_config->priority)). */
            }
            else
            {
                osal_ret = OSAL_RETURN_CONF;
            }
        }
        else
        {
            /* Do nothing */
            /* PRQA S 2053 2 #  Comments for coding rule EST3.1 compliance. */
        }   /* end of if(p_device_control->dev_info->interrupt[channel_count].irq_number ==
                         p_local_control->usr_config->irq_number). */
        interrupt_count++;
    }   /* end of while((gs_osal_num_of_interrupt_table > interrupt_count) && (OSAL_RETURN_OK == osal_ret)). */

    /* Set if only the OSAL_INTERRUPT_INVALID_ID or undefined in the interrupt config */
    if ((OSAL_RETURN_OK == osal_ret) && (NULL == p_interrupt_control))
    {
        p_device_control->interrupt_index[channel_count] = OSAL_DEV_CFG_INTERRUPT_NONE_INDEX;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_interrupt_find_control_info()
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
