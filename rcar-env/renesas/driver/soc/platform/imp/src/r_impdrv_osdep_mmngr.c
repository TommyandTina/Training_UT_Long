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
* File Name    : r_impdrv_osdep_mmngr.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_osdep for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_osdep_mmngr.c
 */

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "rcar-xos/imp/r_impdrv.h"
#include "r_impdrv_private.h"

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/

/*======================================================================================================================
Private file static variables
======================================================================================================================*/

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/

/*======================================================================================================================
Public function definitions
======================================================================================================================*/

/***********************************************************************************************************************
* Start of function impdrv_osdep_memory_get_size()
* CD_PD_02_05_0033
* [Covers: UD_PD_UD02_05_0033]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_get_size(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    size_t                          *const p_size
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if ((NULL == p_memory_handle) || (NULL == p_size))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_memory_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Get size of memory buffer. */
        lRetOsal = R_OSAL_MmngrGetSize(p_memory_handle->handle, p_size);
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
* End of function impdrv_osdep_memory_get_size()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_memory_get_logical()
* CD_PD_02_05_0034
* [Covers: UD_PD_UD02_05_0034]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_get_logical(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    void                            **const p_logical_adrs
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if ((NULL == p_memory_handle) || (NULL == p_logical_adrs))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_memory_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        /** Get the logical address of memory buffer. */
        lRetOsal = R_OSAL_MmngrGetCpuPtr(p_memory_handle->handle, p_logical_adrs);
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
* End of function impdrv_osdep_memory_get_logical()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_memory_get_physical()
* CD_PD_02_05_0035
* [Covers: UD_PD_UD02_05_0035]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_get_physical(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const osal_axi_bus_id_t         axi_id,
    uintptr_t                       *const p_physical_adrs
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if ((NULL == p_memory_handle) || (NULL == p_physical_adrs))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_memory_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if (OSAL_AXI_BUS_ID_INVALID == axi_id)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Get the physical address of memory buffer. */
        lRetOsal = R_OSAL_MmngrGetHwAddr(p_memory_handle->handle, axi_id, p_physical_adrs);
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
* End of function impdrv_osdep_memory_get_physical()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_memory_cache_flush()
* CD_PD_02_05_0036
* [Covers: UD_PD_UD02_05_0036]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_cache_flush(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const uintptr_t                 offset,
    const size_t                    size
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_memory_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_memory_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if (0U != (offset & IMPDRV_CHK_CACHE_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Flush of the memory cache. */
        lRetOsal = R_OSAL_MmngrFlush(p_memory_handle->handle, (size_t)offset, size);
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
* End of function impdrv_osdep_memory_cache_flush()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_memory_cache_clean()
* CD_PD_02_05_0037
* [Covers: UD_PD_UD02_05_0037]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_cache_clean(
    const st_impdrv_memory_handle_t *const p_memory_handle,
    const uintptr_t                 offset,
    const size_t                    size
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_osal_return_t         lRetOsal;

    /** Input parameter confirmation process */
    if (NULL == p_memory_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_memory_handle->handle)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else if (0U != (offset & IMPDRV_CHK_CACHE_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Clean of the memory cache. */
        lRetOsal = R_OSAL_MmngrInvalidate(p_memory_handle->handle, (size_t)offset, size);
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
* End of function impdrv_osdep_memory_cache_clean()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_memory_data_set()
* CD_PD_02_05_0038
* [Covers: UD_PD_UD02_05_0038]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_data_set(
    uint32_t                    *const p_buffer,
    const uint32_t              data,
    const size_t                size
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                count;
    uint32_t                *p_pointer;

    /** Input parameter confirmation process */
    if (NULL == p_buffer)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U != (size & IMPDRV_CHK_MEM_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Sets buffers to a specified character. */
        p_pointer = p_buffer;
        count = (uint32_t)(size / sizeof(uint32_t));
        for (; 0U < count; count--)
        {
            *p_pointer = data;
            p_pointer++;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_memory_data_set()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_osdep_memory_compare()
* CD_PD_02_05_0039
* [Covers: UD_PD_UD02_05_0039]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_osdep_memory_compare(
    const uint32_t              *const p_expected_area,
    const uint32_t              *const p_result_area,
    const size_t                size
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                count;
    const uint32_t          *p_area1;
    const uint32_t          *p_area2;

    /** Input parameter confirmation process */
    if ((NULL == p_expected_area) || (NULL == p_result_area))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U != (size & IMPDRV_CHK_MEM_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Sets buffers to a specified character. */
        p_area1 = p_expected_area;
        p_area2 = p_result_area;
        count = (uint32_t)(size / sizeof(uint32_t));
        while (0U < count)
        {
            if (*p_area1 != *p_area2)
            {
                ercd = IMPDRV_EC_NG_CHECKFAIL;
                break;
            }
            p_area1++;
            p_area2++;
            count--;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_osdep_memory_compare()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/

/*======================================================================================================================
End of File
======================================================================================================================*/
