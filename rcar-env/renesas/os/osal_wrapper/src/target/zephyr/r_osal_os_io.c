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
* File Name :    r_osal_os_io.c
* Version :      0.3.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  IO Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "target/zephyr/r_osal_os_type.h"
#include "target/zephyr/r_osal_os_dev_cfg_tbl_v4h.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * @var gsp_device_config_addr
 * @brief The value for the user to operate the device without being aware of the device configuration index.
***********************************************************************************************************************/
OSAL_STATIC st_osal_device_config_addr_t *gsp_device_config_addr = NULL;

/***********************************************************************************************************************
 * @var g_osal_num_of_device_config
 * @brief Number of the device configuration table.
***********************************************************************************************************************/
/* PRQA S 1504 1 # QAC false indication. Referenced from r_pma_bsp_wrapper.c. */
size_t g_osal_num_of_device_config = 0;

/***********************************************************************************************************************
 * @var gp_osal_device_info
 * @brief The device configuration table.
***********************************************************************************************************************/
/* PRQA S 1504 1 # gp_osal_device_info is also used in another file. */
st_osal_device_info_t *gp_osal_device_info = NULL;

/*******************************************************************************************************************//**
 * @def DT_INTERRUPTS
 * @brief Get the irq value from dtsi
***********************************************************************************************************************/
#define DT_INTERRUPTS(node_id, prop, idx, pos) COND_CODE_1( DT_IRQ_HAS_IDX(node_id, idx), \
            ( gp_osal_device_info[pos].interrupt[idx].irq_number = ((uint64_t)DT_IRQ_BY_IDX(node_id, idx, irq) - 32U) ), \
            ( ) \
    );

/*******************************************************************************************************************//**
 * @def DT_INTERRUPT_PROC
 * Get the irq value list from dtsi
***********************************************************************************************************************/
#define DT_INTERRUPT_PROC(node_id, pos) COND_CODE_1( DT_NODE_HAS_PROP(node_id, interrupts), \
    (DT_FOREACH_PROP_ELEM_VARGS(node_id, interrupts, DT_INTERRUPTS, pos);pos = pos + 1U;), (pos = pos + 1U;) \
    )

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Sort device configuration.
* @param[in]        array_num               The num of array.
* @param[in]        p_osal_device_config    The pointer of st_osal_device_info_t.
* @param[in,out]    p_device_config_addr    The pointer of st_osal_device_config_addr_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_io_sort_dev_config(size_t array_num,
                                              const st_osal_device_info_t *p_osal_device_config,
                                              st_osal_device_config_addr_t *p_device_config_addr);

/***********************************************************************************************************************
* @brief            Map device memory
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_CONF
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_mmap_device(void);


/***********************************************************************************************************************
* @brief            Initialize device information
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_init_device_info(void);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_IO_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoStaticInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoStaticInit(size_t * p_num_of_device_config,
                                       osal_device_config_addr_t * p_device_config_addr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if ((NULL == p_num_of_device_config) || (NULL == p_device_config_addr))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        switch (g_osal_device_soc_type.device_soc_type)
        {
            /* PRQA S 2900,0572 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V4H:
            case OSAL_DEVICE_SOC_TYPE_V4H2:
            {
                gp_osal_device_info = gs_osal_device_info_v4h;
                break;
            }
            /* PRQA S 2900,0572 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_INVALID:
            default:
            {
                osal_ret = OSAL_RETURN_FAIL;
                break;
            }
        }   /* end of switch(g_osal_device_soc_type.device_soc_type). */

        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Count up the number of valid items in the device information table. */
            g_osal_num_of_device_config = 0U;
            while (OSAL_DEVICE_INVALID_ID != gp_osal_device_info[g_osal_num_of_device_config].id)
            {
                g_osal_num_of_device_config++;
            }

            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            gsp_device_config_addr = (st_osal_device_config_addr_t*)calloc(1, sizeof(st_osal_device_config_addr_t) * g_osal_num_of_device_config);
            if (NULL == gsp_device_config_addr)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Set up the utlb and iterrupt information in the device configuration table. */
                osal_ret = r_osal_os_io_init_device_info();
                if (OSAL_RETURN_OK == osal_ret)
                {
                    /* Sort the device configuration by ID. */
                    r_osal_os_io_sort_dev_config(g_osal_num_of_device_config, gp_osal_device_info, gsp_device_config_addr);

                    /* Set up the virtual address information of the device information. */
                    osal_ret = r_osal_os_io_mmap_device();
                }
                else
                {
                    /* Do nothing */
                }
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                *p_num_of_device_config = g_osal_num_of_device_config;
                *p_device_config_addr   = gsp_device_config_addr;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */
    }   /* end of if ((NULL == p_num_of_device_config) || (NULL == p_device_config_addr)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoStaticInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_IO_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoStaticDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoStaticDeinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t       osal_ret = OSAL_RETURN_OK;
    size_t                loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_device_config_addr)
    {
        /* Deinitialize the virtual address setting and deallocate the buffer for the interrupt information table. */
        loop_count = 0;
        while (g_osal_num_of_device_config > loop_count)
        {
            /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
            gsp_device_config_addr[loop_count].virtual_addr = (uintptr_t)NULL;

            if (NULL != gsp_device_config_addr[loop_count].p_dev_info->interrupt)
            {
                /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
                /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
                free((void *)gsp_device_config_addr[loop_count].p_dev_info->interrupt);
            }
            else
            {
                /* Do nothing */
            }

            loop_count++;
        }
        /* PRQA S 0314 2 # No problem because it follows the specification of the calling function. */
        /* PRQA S 5118 1 # Memory deallocation functions are available during deinitialization. */
        free((void*)gsp_device_config_addr);
        gsp_device_config_addr       = NULL;
        g_osal_num_of_device_config = 0U;
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != gsp_device_config_addr). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoStaticDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_IO_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoRead()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_IoRead(osal_device_handle_t const handle,
                                 uintptr_t offset,
                                 size_t size,
                                 size_t unit_size,
                                 void* p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          loop_count;
    const uint8_t   *p_buf8;
    const uint16_t  *p_buf16;
    const uint32_t  *p_buf32;
    uint8_t         *p_data8;
    uint16_t        *p_data16;
    uint32_t        *p_data32;
    uint64_t        *p_data64;
    size_t          data_num;
    uintptr_t       local_offset;
    uint32_t        index;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_data)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Read operations for each byte-size. */
        data_num     = size / unit_size;
        switch (unit_size)
        {
            case sizeof(uint8_t):
            {
                /* PRQA S 2985 1 # unit_size is 1 in this case, but not fixed for readability. */
                local_offset = offset / (uintptr_t)unit_size;
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint8_t* is possible. */
                p_buf8  = (const uint8_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data8 = (uint8_t*)p_data;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_data8[loop_count] = p_buf8[local_offset + loop_count];
                }
                break;
            }
            /* PRQA S 2880,0736 1 # QAC false indication (CCT change). */
            case sizeof(uint16_t):
            {
                local_offset = offset / (uintptr_t)unit_size;
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint16_t* is possible. */
                p_buf16  = (const uint16_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data16 = (uint16_t*)p_data;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_data16[loop_count] = p_buf16[local_offset + loop_count];
                }
                break;
            }
            /* PRQA S 0736 1 # QAC false indication (CCT change). */
            case sizeof(uint32_t):
            {
                local_offset = offset / (uintptr_t)unit_size;
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint32_t* is possible. */
                p_buf32  = (const uint32_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data32 = (uint32_t*)p_data;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_data32[loop_count] = p_buf32[local_offset + loop_count];
                }
                break;
            }
            /* PRQA S 0736 1 # QAC false indication (CCT change). */
            case sizeof(uint64_t):
            {
                local_offset = offset / (uintptr_t)sizeof(uint32_t);
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint32_t* is possible. */
                p_buf32  = (const uint32_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data64 = (uint64_t*)p_data;

                /* Read the upper byte and lower byte respectively. */
                index = 0;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_data64[loop_count] = (uint64_t)p_buf32[local_offset + index];
                    index++;
                    p_data64[loop_count] = (p_data64[loop_count] | ((uint64_t)p_buf32[local_offset + index]  << 32));
                    index++;
                }
                break;
            }
            default:
            {
                osal_ret = OSAL_RETURN_FAIL;
                break;
            }
        }   /* end of switch (unit_size). */
    }   /* end of if(NULL == p_data). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoRead()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_IO_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_IoWrite()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_IoWrite(osal_device_handle_t const handle,
                                  uintptr_t offset,
                                  size_t size,
                                  size_t unit_size,
                                  const void* p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          loop_count;
    uint8_t         *p_buf8;
    uint16_t        *p_buf16;
    uint32_t        *p_buf32;
    const uint8_t   *p_data8;
    const uint16_t  *p_data16;
    const uint32_t  *p_data32;
    const uint64_t  *p_data64;
    size_t          data_num;
    uintptr_t       local_offset;
    uint32_t        index;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_data)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Write operations for each byte-size. */
        data_num     = size / unit_size;
        switch (unit_size)
        {
            case sizeof(uint8_t):
            {
                /* PRQA S 2985 1 # unit_size is 1 in this case, but not fixed for readability. */
                local_offset = offset / (uintptr_t)unit_size;
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint8_t* is possible. */
                p_buf8  = (uint8_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data8 = (const uint8_t*)p_data;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_buf8[local_offset + loop_count] = p_data8[loop_count];
                }
                break;
            }
            /* PRQA S 2880,0736 1 # QAC false indication (CCT change). */
            case sizeof(uint16_t):
            {
                local_offset = offset / (uintptr_t)unit_size;
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint16_t* is possible. */
                p_buf16  = (uint16_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data16 = (const uint16_t*)p_data;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_buf16[local_offset + loop_count] = p_data16[loop_count];
                }
                break;
            }
            /* PRQA S 0736 1 # QAC false indication (CCT change). */
            case sizeof(uint32_t):
            {
                local_offset = offset / (uintptr_t)unit_size;
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint32_t* is possible. */
                p_buf32  = (uint32_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data32 = (const uint32_t*)p_data;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_buf32[local_offset + loop_count] = p_data32[loop_count];
                }
                break;
            }
            /* PRQA S 0736 1 # QAC false indication (CCT change). */
            case sizeof(uint64_t):
            {
                local_offset = offset / (uintptr_t)sizeof(uint32_t);
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint32_t* is possible. */
                p_buf32  = (uint32_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data64 = (const uint64_t*)p_data;

                /* Write the upper byte and lower byte respectively. */
                index = 0;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_buf32[local_offset + index] = (uint32_t)(p_data64[loop_count]);
                    index++;
                    p_buf32[local_offset + index] = (uint32_t)(p_data64[loop_count] >> 32);
                    index++;
                }
                break;
            }
            default:
            {
                osal_ret = OSAL_RETURN_FAIL;
                break;
            }
        }   /* end of switch (unit_size). */
    }   /* end of if(NULL == p_data). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_IoWrite()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_030]
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_osal_os_io_sort_dev_config()
***********************************************************************************************************************/
OSAL_STATIC void r_osal_os_io_sort_dev_config(size_t array_num,
                                              const st_osal_device_info_t *p_osal_device_config,
                                              st_osal_device_config_addr_t *p_device_config_addr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int       os_ret;
    char      tmp[OSAL_DEVICE_NAME_MAX_LENGTH];
    size_t    loop_count;
    size_t    cmp_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* Sort the table by ID. */
    p_device_config_addr[0U].p_dev_info = &p_osal_device_config[0U];
    for (loop_count = 1U; loop_count < array_num; loop_count++)
    {
        (void)strncpy(tmp, p_osal_device_config[loop_count].id, (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);
        for (cmp_count = loop_count; cmp_count > 0U; cmp_count--)
        {
            os_ret = strncmp(p_device_config_addr[(cmp_count - 1U)].p_dev_info->id, tmp, (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);
            if (0 < os_ret)
            {
                p_device_config_addr[cmp_count].p_dev_info = p_device_config_addr[(cmp_count - 1U)].p_dev_info;
            }
            else
            {
                break;
            }
        }
        p_device_config_addr[cmp_count].p_dev_info = &p_osal_device_config[loop_count];
    }

    return;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_sort_dev_config()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_osal_os_io_mmap_device()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_mmap_device(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret   = OSAL_RETURN_OK;
    size_t          loop_count = 0U;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* Set the virtual address of each devcie. */
    while ((g_osal_num_of_device_config > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        gsp_device_config_addr[loop_count].fd = 0;

        /* PRQA S 0326 4 # Cast between a pointer to void and uintptr_t is possible. */
        if ((((uintptr_t)NULL == gsp_device_config_addr[loop_count].p_dev_info->address) &&
             (0U != gsp_device_config_addr[loop_count].p_dev_info->range)) ||
            (((uintptr_t)NULL != gsp_device_config_addr[loop_count].p_dev_info->address) &&
             (0U == gsp_device_config_addr[loop_count].p_dev_info->range)) )
        {
            /* In case of the "address is NULL and range of address is not 0" case
               or the "address is not NULL and range of address is 0" case, set the return value to OSAL_RETURN_CONF. */
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
            if ((uintptr_t)NULL != gsp_device_config_addr[loop_count].p_dev_info->address)
            {
                gsp_device_config_addr[loop_count].virtual_addr = (uintptr_t)gsp_device_config_addr[loop_count].p_dev_info->address;
            }
            else
            {
                /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                gsp_device_config_addr[loop_count].virtual_addr = (uintptr_t)NULL;
            }   /* end of if((uintptr_t)NULL != gsp_device_config_addr[loop_count].p_dev_info->address). */
        }   /* end of if((((uintptr_t)NULL == gsp_device_config_addr[loop_count].p_dev_info->address). */
        loop_count++;
    }   /* end of while((g_osal_num_of_device_config > loop_count)&&(OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_mmap_device()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function r_osal_os_io_init_device_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_init_device_info(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t dev_idx = 0;
    uint32_t irq_idx = 0;
    size_t   irq_num;

    while ((g_osal_num_of_device_config > dev_idx) && (OSAL_RETURN_OK == osal_ret))
    {
        /* Allocate memory for interrupt data */
        irq_num = gp_osal_device_info[dev_idx].irq_channels;
        if(0U != irq_num)
        {
            /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_OS_MutexDestroy(). */
            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            gp_osal_device_info[dev_idx].interrupt = (st_osal_interrupt_info_t*)calloc(1, irq_num*sizeof(st_osal_interrupt_info_t));
            if(NULL == gp_osal_device_info[dev_idx].interrupt)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }

        /* Set the utlb_assignment data */
        if (0U != gs_osal_utlb_table[dev_idx].table_size)
        {
            gp_osal_device_info[dev_idx].utlb_assignment = &gs_osal_utlb_table[dev_idx];
        }
        else
        {
            /* Do nothing */
        }
        dev_idx++;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
#ifndef OSAL_ZEPHYR_UT
        /* Set the interrupt data from dtsi */
        /* PRQA S 2983 2 # gp_osal_device_info is also used in another file. */
        /* PRQA S 1035 1 # Caused by the implementation of macros defined inside the Zephyr OS. No side effect. */
        DT_FOREACH_CHILD_VARGS(DT_NODELABEL(osal_device), DT_INTERRUPT_PROC, (irq_idx));
#endif
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_MEM == osal_ret)
    {
        /* Deinitialize the device information in case of error. */
        dev_idx = 0;
        while (NULL != gp_osal_device_info[dev_idx].interrupt)
        {
            /* PRQA S 5118 2 # Memory deallocation functions are available during deinitialization. */
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            free((void *)gp_osal_device_info[dev_idx].interrupt);
            gp_osal_device_info[dev_idx].interrupt = NULL;
            dev_idx++;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
/* PRQA S 9109 3 # he termination function has already multiple sub-functions to reduce complexity.
   We have decided that further division is not desirable from perspective of maintainability and readability. */
}
/***********************************************************************************************************************
* End of function r_osal_os_io_init_device_info()
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
