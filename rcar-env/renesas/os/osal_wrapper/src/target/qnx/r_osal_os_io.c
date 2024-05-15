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
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  IO Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>

#include "target/qnx/r_osal_os_type.h"
#include "target/qnx/r_osal_os_dev_cfg_tbl_v3h1.h"
#include "target/qnx/r_osal_os_dev_cfg_tbl_v3h2.h"
#include "target/qnx/r_osal_os_dev_cfg_tbl_v3m.h"
#include "target/qnx/r_osal_os_dev_cfg_tbl_v4h.h"
#include "target/qnx/r_osal_os_dev_cfg_tbl_v4m.h"
#include "target/qnx/r_osal_os_dev_cfg_tbl_s4.h"

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
 * @var gs_osal_num_of_device_config
 * @brief Number of the device configuration table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_osal_num_of_device_config = 0;

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
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_io_mmap_device(void);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_001
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
    const st_osal_device_info_t *p_osal_device_config;

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
            case OSAL_DEVICE_SOC_TYPE_V3M:
            {
                p_osal_device_config         = gs_osal_device_info_v3m;
                break;
            }
            /* PRQA S 2900,0572,2880 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V3H1:
            {
                p_osal_device_config         = gs_osal_device_info_v3h1;
                break;
            }
            /* PRQA S 2900,0572 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V3H2:
            {
                p_osal_device_config         = gs_osal_device_info_v3h2;
                break;
            }
            /* PRQA S 2900,0572 2 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V4H:
            case OSAL_DEVICE_SOC_TYPE_V4H2:
            {
                p_osal_device_config         = gs_osal_device_info_v4h;
                break;
            }
            /* PRQA S 2900,0572 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_V4M:
            {
                p_osal_device_config         = gs_osal_device_info_v4m;
                break;
            }
            /* PRQA S 2900,0572 1 # QAC false indication (CCT change). */
            case OSAL_DEVICE_SOC_TYPE_S4:
            {
                p_osal_device_config         = gs_osal_device_info_s4;
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
            gs_osal_num_of_device_config = 0U;
            while (OSAL_DEVICE_INVALID_ID != p_osal_device_config[gs_osal_num_of_device_config].id)
            {
                gs_osal_num_of_device_config++;
            }

            /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
            gsp_device_config_addr = (st_osal_device_config_addr_t*)calloc(1, sizeof(st_osal_device_config_addr_t) * gs_osal_num_of_device_config);
            if (NULL == gsp_device_config_addr)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                r_osal_os_io_sort_dev_config(gs_osal_num_of_device_config, p_osal_device_config, gsp_device_config_addr);

                osal_ret = r_osal_os_io_mmap_device();
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                *p_num_of_device_config = gs_osal_num_of_device_config;
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_002
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
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                   os_ret;
    size_t                loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != gsp_device_config_addr)
    {
        loop_count = 0;
        while ((gs_osal_num_of_device_config > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
            if ((uintptr_t)NULL != gsp_device_config_addr[loop_count].virtual_addr)
            {
                /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                os_ret = munmap_device_memory((void *)gsp_device_config_addr[loop_count].virtual_addr,
                                               gsp_device_config_addr[loop_count].p_dev_info->range);
                if (0 != os_ret)
                {
                    osal_ret = OSAL_RETURN_MEM;
                }
                else
                {
                    /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                    gsp_device_config_addr[loop_count].virtual_addr = (uintptr_t)NULL;
                }
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
        gs_osal_num_of_device_config = 0U;
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_003
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
    const uint64_t  *p_buf64;
    uint8_t         *p_data8;
    uint16_t        *p_data16;
    uint32_t        *p_data32;
    uint64_t        *p_data64;
    size_t          data_num;
    uintptr_t       local_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_data)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        data_num     = size / unit_size;
        local_offset = offset / (uintptr_t)unit_size;
        switch (unit_size)
        {
            case sizeof(uint8_t):
            {
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
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint64_t* is possible. */
                p_buf64  = (const uint64_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data64 = (uint64_t*)p_data;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_data64[loop_count] = p_buf64[local_offset + loop_count];
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_029]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_004
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
    uint64_t        *p_buf64;
    const uint8_t   *p_data8;
    const uint16_t  *p_data16;
    const uint32_t  *p_data32;
    const uint64_t  *p_data64;
    size_t          data_num;
    uintptr_t       local_offset;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == p_data)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        data_num     = size / unit_size;
        local_offset = offset / (uintptr_t)unit_size;
        switch (unit_size)
        {
            case sizeof(uint8_t):
            {
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
                /* PRQA S 0306 1 # Cast between a pointer to uintptr_t and uint64_t* is possible. */
                p_buf64  = (uint64_t*)handle->base_addr;
                /* PRQA S 0316 1 # No problem because it returns to the type at the time of setting. */
                p_data64 = (const uint64_t*)p_data;
                for (loop_count = 0; data_num > loop_count; loop_count++)
                {
                    p_buf64[local_offset + loop_count] = p_data64[loop_count];
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_030]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_005
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_sort_dev_config()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_IoStaticInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
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
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_016
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_io_mmap_device()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_IoStaticInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
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
    while ((gs_osal_num_of_device_config > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        gsp_device_config_addr[loop_count].fd = 0;

        /* PRQA S 0326 4 # Cast between a pointer to void and uintptr_t is possible. */
        if ((((uintptr_t)NULL == gsp_device_config_addr[loop_count].p_dev_info->address) &&
             (0U != gsp_device_config_addr[loop_count].p_dev_info->range)) ||
            (((uintptr_t)NULL != gsp_device_config_addr[loop_count].p_dev_info->address) &&
             (0U == gsp_device_config_addr[loop_count].p_dev_info->range)) )
        {
            osal_ret = OSAL_RETURN_CONF;
        }
        else
        {
            /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
            if ((uintptr_t)NULL != gsp_device_config_addr[loop_count].p_dev_info->address)
            {
                /* PRQA S 0326 2 # Cast between a pointer to void and uintptr_t is possible. */
                /* PRQA S 4542 3 # PROT_READ, PROT_WRITE and PROT_NOCACHE are used according to the QNX. */
                gsp_device_config_addr[loop_count].virtual_addr = (uintptr_t)mmap_device_memory(NULL,
                                                                                                gsp_device_config_addr[loop_count].p_dev_info->range,
                                                                                                (PROT_READ | PROT_WRITE | PROT_NOCACHE),
                                                                                                MAP_SHARED,
                                                                                                gsp_device_config_addr[loop_count].p_dev_info->address);
                /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                if ((uintptr_t)MAP_FAILED == gsp_device_config_addr[loop_count].virtual_addr)
                {
                    osal_ret = OSAL_RETURN_MEM;
                    /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                    gsp_device_config_addr[loop_count].virtual_addr = (uintptr_t)NULL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
                gsp_device_config_addr[loop_count].virtual_addr = (uintptr_t)NULL;
            }   /* end of if((uintptr_t)NULL != gsp_device_config_addr[loop_count].p_dev_info->address). */
        }   /* end of if((((uintptr_t)NULL == gsp_device_config_addr[loop_count].p_dev_info->address). */
        loop_count++;
    }   /* end of while((gs_osal_num_of_device_config > loop_count)&&(OSAL_RETURN_OK == osal_ret)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_io_mmap_device()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_027]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
