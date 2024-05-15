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
* File Name :    r_osal_os_memory.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Memory Related OS IFs
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#include "target/qnx/r_osal_os_type.h"
#include "target/common/res_cfg/r_osal_res_cfg_memory.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/***********************************************************************************************************************
* @brief            Map memory object.
* @param[in,out]    mngr_control      Memory manager manager control table.
* @param[in]        p_info            Memory region information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mmngr_mmap(osal_mem_mmngr_ptr_self_t mngr_control,
                                                 const st_osal_mmngr_region_info_t *p_info);

/***********************************************************************************************************************
* @brief            Get the physical address of a virtually mapped memory block.
* @param[in,out]    mngr_control      Memory manager manager control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_mmngr_convert_virt_to_phy_addr(osal_mem_mmngr_ptr_self_t mngr_control);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MEM_001
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MmngrMap()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MmngrMap(osal_mem_mmngr_ptr_self_t mngr_control,
                                   const st_osal_mmngr_config_t * const p_usr_config,
                                   osal_memory_region_idx_t region_idx)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                     osal_ret    = OSAL_RETURN_OK;
    int                                 os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    size_t                              page_size;
    size_t                              mem_size;
    const st_osal_mmngr_region_info_t   *p_info     = &g_osal_memory_region_info_list.p_info[region_idx];

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 1281 1 # QAC false indication (CCT change). */
    if (p_usr_config->memory_limit == OSAL_MEMORY_MANAGER_CFG_UNLIMITED)
    {
        mem_size = p_info->range;
    }
    else
    {
        mem_size = p_usr_config->memory_limit + OSAL_RES_CFG_MMNGR_MEM_RESERVE_SIZE;
    }

    page_size = (size_t)sysconf(_SC_PAGESIZE);
    mngr_control->size = mem_size & ~(page_size - 1U);

    if ('\0' != p_info->name[0])
    {
        /* PRQA S 0339 2 # No problem because the system is requesting the O_RDWR. */
        mngr_control->mem_fd = posix_typed_mem_open(p_info->name,
                                                    O_RDWR,
                                                    POSIX_TYPED_MEM_ALLOCATE_CONTIG);
        if (-1 == mngr_control->mem_fd)
        {
            osal_ret = OSAL_RETURN_DEV;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        mngr_control->mem_fd = NOFD;
    }   /* end of if ('\0' != p_info->name[0]). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mmngr_mmap(mngr_control, p_info);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_os_mmngr_convert_virt_to_phy_addr(mngr_control);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NOFD != mngr_control->mem_fd)
        {
            os_ret = close(mngr_control->mem_fd);
            if (0 != os_ret)
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                mngr_control->mem_fd = NOFD;
            }
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;

}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MmngrMap()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MEM_044]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MEM_002
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MmngrUnmap()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MmngrUnmap(osal_mem_mmngr_ptr_self_t mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL == mngr_control)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        os_ret = munmap(mngr_control->p_virtual_addr, mngr_control->size);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            mngr_control->p_virtual_addr = NULL;
        }
    }   /* end of if (NULL == mngr_control). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MmngrUnmap()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MEM_045]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MEM_003
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MmngrFlush()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_MmngrFlush(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                     uintptr_t addr,
                                     size_t offset,
                                     size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)memory_buffer_obj_hndl;

    /* PRQA S 4542 2 # MS_SYNC and MS_CACHE_ONLY are used according to the QNX specification. */
    /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
    os_ret = msync((void *)(addr + offset), size, MS_SYNC | MS_CACHE_ONLY);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MmngrFlush()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MEM_046]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MEM_004
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_MmngrInvalidate()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_OS_MmngrInvalidate(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                          uintptr_t addr,
                                          size_t offset,
                                          size_t size)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)memory_buffer_obj_hndl;

    /* PRQA S 4542 2 # MS_INVALIDATE and MS_CACHE_ONLY are used according to the QNX specification. */
    /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
    os_ret = msync((void *)(addr + offset), size, MS_INVALIDATE | MS_CACHE_ONLY);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_MmngrInvalidate()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MEM_047]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MEM_007
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mmngr_mmap()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_MmngrMap() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_mmngr_mmap(osal_mem_mmngr_ptr_self_t mngr_control,
                                                 const st_osal_mmngr_region_info_t *p_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    int             mmap_flags  = MAP_SHARED; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */
    off64_t         mmap_offset = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NOFD == mngr_control->mem_fd)
    {
        mmap_offset = (off64_t)p_info->offset;
        if (0 == mmap_offset)
        {
            /* PRQA S 4542,4532 1 # MAP_PHYS and MAP_ANON are used according to the QNX specification. */
            mmap_flags |= (MAP_PHYS | MAP_ANON);
        }
        else
        {
            /* PRQA S 4542,4532 1 # MAP_PHYS is used according to the QNX specification. */
            mmap_flags |= (MAP_PHYS);
        }
    }
    else
    {
        /* Do nothing */
    }

    /* PRQA S 4542 3 # PROT_READ and PROT_WRITE are used according to the QNX specification. */
    mngr_control->p_virtual_addr = mmap64(NULL,
                                          mngr_control->size,
                                          (PROT_READ | PROT_WRITE),
                                          mmap_flags,
                                          mngr_control->mem_fd,
                                          mmap_offset);
    /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
    if (MAP_FAILED == mngr_control->p_virtual_addr)
    {
        osal_ret = OSAL_RETURN_MEM;
        if (NOFD != mngr_control->mem_fd)
        {
            (void)close(mngr_control->mem_fd);
            mngr_control->mem_fd = NOFD;
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

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mmngr_mmap()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MEM_044]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_MEM_008
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_mmngr_convert_virt_to_phy_addr()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_OS_MmngrMap() in order to improve the result of 
   static code analysis (STPTH of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_os_mmngr_convert_virt_to_phy_addr(osal_mem_mmngr_ptr_self_t mngr_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    int             os_ret; /* PRQA S 5209 # System is requesting an int, so it cannot be changed. */

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = mlock(mngr_control->p_virtual_addr, mngr_control->size);
    if (0 != os_ret)
    {
        osal_ret = OSAL_RETURN_FAIL;
        (void)munmap(mngr_control->p_virtual_addr, mngr_control->size);
        if (NOFD != mngr_control->mem_fd)
        {
            (void)close(mngr_control->mem_fd);
            mngr_control->mem_fd = NOFD;
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

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0310 5 # No problem because the system is requesting the off64_t. */
        os_ret = mem_offset64(mngr_control->p_virtual_addr,
                              NOFD,
                              mngr_control->size,
                              (off64_t*)&mngr_control->physical_addr,
                              NULL);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)munlock(mngr_control->p_virtual_addr, mngr_control->size);
            (void)munmap(mngr_control->p_virtual_addr, mngr_control->size);
            if (NOFD != mngr_control->mem_fd)
            {
                (void)close(mngr_control->mem_fd);
                mngr_control->mem_fd = NOFD;
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
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    if (OSAL_RETURN_OK == osal_ret)
    {
        os_ret = munlock(mngr_control->p_virtual_addr, mngr_control->size);
        if (0 != os_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
            (void)munmap(mngr_control->p_virtual_addr, mngr_control->size);
            if (NOFD != mngr_control->mem_fd)
            {
                (void)close(mngr_control->mem_fd);
                mngr_control->mem_fd = NOFD;
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
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_mmngr_convert_virt_to_phy_addr()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_MEM_044]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
