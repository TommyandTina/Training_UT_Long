/***********************************************************************************************************************
* Copyright [2022-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_os_memory.c
* Version :      1.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for Memory
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <linux/cmemdrv.h>
#include "target/linux/r_osal_os_types.h"
#include "target/common/res_cfg/r_osal_res_cfg_memory.h"

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MEM_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MmngrMap()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MmngrMap(osal_mem_mmngr_ptr_self_t mngr_control, const st_osal_mmngr_config_t* const p_usr_config, osal_memory_region_idx_t region_idx)
{
    e_osal_return_t                     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                                 linux_ret;
    size_t                              page_size;
    size_t                              max_memory_size;
    const st_osal_mmngr_region_info_t*  p_region_info = &g_osal_memory_region_info_list.p_info[region_idx];
    struct mem_info                     ioinfo;

    if ((NULL == mngr_control) ||
        (NULL == p_usr_config))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    /* Open the memory device */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* File path refer to the osal configuration */
        /* PRQA S 4542 2 # O_RDWR and O_SYNC are used in accordance with the specification, verified OK. */
        /* PRQA S 0339 1 # O_RDWR and O_SYNC uses octal, verified OK. */
        mngr_control->mem_fd = R_OSAL_OS_Open(p_region_info->name, O_RDWR | O_SYNC);
        if (-1 == mngr_control->mem_fd)
        {
            osal_ret = OSAL_RETURN_DEV;
        }
    }

    /* Set the memory size and physical address */
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* All chunks must be page-size aligned */
        if (OSAL_MEMORY_MANAGER_CFG_UNLIMITED == p_usr_config->memory_limit)
        {
            max_memory_size = p_region_info->range;
        }
        else
        {
            max_memory_size = p_usr_config->memory_limit + OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE;
        }

        /* Round down to the page size unit */
        page_size = (size_t)R_OSAL_OS_GetSysPageSize();
        mngr_control->size = max_memory_size & ~(page_size - 1U);

        /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
        ioinfo.phys_addr = (uintptr_t)NULL;

        _Static_assert( (
            sizeof(ioinfo.phys_addr)
            ) == (
            sizeof(size_t)
            ), "Structure must not have any padding");

        /* PRQA S 0314 1 # The 3rd arguments must be (void *) but no side effects in this case. */
        linux_ret = ioctl(mngr_control->mem_fd, GET_PHYS_ADDR, (void *)&ioinfo);

        /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
        if ((-1 == linux_ret) || ((uintptr_t)NULL == ioinfo.phys_addr))
        {
            osal_ret = OSAL_RETURN_DEV;
        }
    }

    /* Set the virtual address */
    if (OSAL_RETURN_OK == osal_ret)
    {
        mngr_control->physical_addr = ioinfo.phys_addr;

        /* PRQA S 4542 1 # PROT_READ and PROT_WRITE are used in accordance with the specification, verified OK. */
        mngr_control->p_virtual_addr = mmap(NULL, mngr_control->size, PROT_READ | PROT_WRITE, MAP_SHARED, mngr_control->mem_fd, (__off_t)p_region_info->offset);

        /* PRQA S 0326 1 # MAP_FAILED is used in accordance with the specification. */
        if (MAP_FAILED == mngr_control->p_virtual_addr)
        {
            osal_ret = OSAL_RETURN_DEV;
            mngr_control->p_virtual_addr = NULL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MmngrMap()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MEM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MEM_102
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MmngrUnmap()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MmngrUnmap(osal_mem_mmngr_ptr_self_t mngr_control)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_return_t close_ret;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    if (NULL == mngr_control)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Unset the virtual address */
        if (NULL != mngr_control->p_virtual_addr)
        {
            linux_ret = munmap(mngr_control->p_virtual_addr, mngr_control->size);
            if (0 != linux_ret)
            {
                osal_ret = OSAL_RETURN_DEV;
            }
            mngr_control->p_virtual_addr = NULL;
        }

        /* Close the memory device */
        if (-1 != mngr_control->mem_fd)
        {
            close_ret = R_OSAL_OS_Close(mngr_control->mem_fd);
            if ((OSAL_RETURN_OK != close_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_DEV;
            }
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MmngrUnmap()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MEM_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MEM_103
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MmngrFlush()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_MmngrFlush(osal_memory_buffer_handle_t memory_buffer_obj_hndl, uintptr_t addr, size_t offset, size_t size)
{
    e_osal_return_t                     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                                 linux_ret;
    st_osal_mem_mmngr_ptr_self_t const* p_allocator;
    st_osal_mem_buf_ptr_self_t const*   p_buffer;
    struct mem_mlock                    ioinfo;

    /* addr does not use on Linux */
    (void)addr;

    if (NULL == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because cast from void * to object is allowed. */
        p_allocator = (st_osal_mem_mmngr_ptr_self_t const *)memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self;
        p_buffer    = (st_osal_mem_buf_ptr_self_t const *)memory_buffer_obj_hndl->p_ptr_self;

        ioinfo.offset   = (p_buffer->buffer_pa + offset) - p_allocator->physical_addr;
        ioinfo.size     = size;
        ioinfo.dir      = IOCTL_FROM_CPU_TO_DEV;

        _Static_assert( (
            sizeof(ioinfo.offset) +
            sizeof(ioinfo.size) +
            sizeof(ioinfo.dir)
            ) == (
            sizeof(size_t) +
            sizeof(size_t) +
            sizeof(size_t)
            ), "Structure must not have any padding");

        /* PRQA S 0314 1 # The 3rd arguments must be (void *) but no side effects in this case. */
        linux_ret = ioctl(p_allocator->mem_fd, M_LOCK, (void *)&ioinfo);
        if (-1 == linux_ret)
        {
            osal_ret = OSAL_RETURN_PROHIBITED;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MmngrFlush()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MEM_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_MEM_104
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MmngrInvalidate()
***********************************************************************************************************************/
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_MmngrInvalidate(osal_memory_buffer_handle_t memory_buffer_obj_hndl, uintptr_t addr, size_t offset, size_t size)
{
    e_osal_return_t                     osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int                                 linux_ret;
    st_osal_mem_mmngr_ptr_self_t const* p_allocator;
    st_osal_mem_buf_ptr_self_t const*   p_buffer;
    struct mem_mlock                    ioinfo;

    /* addr does not use on Linux */
    (void)addr;

    if (NULL == memory_buffer_obj_hndl)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0316 2 # No problem because cast from void * to object is allowed. */
        p_allocator = (st_osal_mem_mmngr_ptr_self_t const *)memory_buffer_obj_hndl->mmngr_owner_hndl->p_ptr_self;
        p_buffer    = (st_osal_mem_buf_ptr_self_t const *)memory_buffer_obj_hndl->p_ptr_self;

        ioinfo.offset   = (p_buffer->buffer_pa + offset) - p_allocator->physical_addr;
        ioinfo.size     = size;
        ioinfo.dir      = IOCTL_FROM_DEV_TO_CPU;

        _Static_assert( (
            sizeof(ioinfo.offset) +
            sizeof(ioinfo.size) +
            sizeof(ioinfo.dir)
            ) == (
            sizeof(size_t) +
            sizeof(size_t) +
            sizeof(size_t)
            ), "Structure must not have any padding");

        /* PRQA S 0314 1 # The 3rd arguments must be (void *) but no side effects in this case. */
        linux_ret = ioctl(p_allocator->mem_fd, M_UNLOCK, (void *)&ioinfo);
        if (-1 == linux_ret)
        {
            osal_ret = OSAL_RETURN_PROHIBITED;
        }
    } /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MmngrInvalidate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_MEM_004]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
