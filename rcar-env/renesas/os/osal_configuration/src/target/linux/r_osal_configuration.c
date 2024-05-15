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
* File Name :    r_osal_configuration.c
* Version :      1.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Configurator for specific OS
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal_configuration/target/linux/r_osal_configuration.h"

/***********************************************************************************************************************
 * @var g_osal_memory_max_configration
 * @brief osal resource configuration : max memory configuration structure
 * @note The memory_limit is unused
***********************************************************************************************************************/
const st_osal_mmngr_config_t g_osal_memory_max_configration =
{
    .mode                           = OSAL_MMNGR_ALLOC_MODE_STACK_PROT,
    .memory_limit                   = OSAL_MEMORY_MANAGER_CFG_UNLIMITED,
    .max_allowed_allocations        = (4U * 1024U),
    .max_registered_monitors_cbs    = (4U * 1024U)
};

/***********************************************************************************************************************
 * @var g_osal_memory_region_info
 * osal device configuration : memory region information structure
***********************************************************************************************************************/
const st_osal_mmngr_region_info_list_t g_osal_memory_region_info_list =
{
#if defined(RCAR_V4H) || defined(RCAR_V4M)
    .num_of_memory_regions = 7,
    .default_memory_region = 0,
    .p_info = (st_osal_mmngr_region_info_t[])
    {
        /* Memory region 0 */
        {
            .name = "/dev/cmem0",
            .offset = 0,
            .range = (512U * 1024U * 1024U)
        },
        /* Memory region 1 */ /* for VDSP0 */
        {
            .name = "/dev/cmem_other0",
            .offset = 0,
            .range = (6U * 1024U * 1024U)
        },
        /* Memory region 2 */ /* for VDSP1 */
        {
            .name = "/dev/cmem_other1",
            .offset = 0,
            .range = (6U * 1024U * 1024U)
        },
        /* Memory region 3 */ /* for VDSP2 */
        {
            .name = "/dev/cmem_other2",
            .offset = 0,
            .range = (6U * 1024U * 1024U)
        },
        /* Memory region 4 */ /* for VDSP3 */
        {
            .name = "/dev/cmem_other3",
            .offset = 0,
            .range = (6U * 1024U * 1024U)
        },
        /* Memory region 5 */ /* for PAP */
        {
            .name = "/dev/cmem_other4",
            .offset = 0,
            .range = (256U * 1024U) + (2U * 1024U * 1024U)
        },
        /* Memory region 6 */ /* This memory region is located in a 40-bit address space */
        {
            .name = "/dev/cmem_other5",
            .offset = 0,
            .range = (1024U * 1024U * 1024U)
        }
#else
    .num_of_memory_regions = 2,
    .default_memory_region = 0,
    .p_info = (st_osal_mmngr_region_info_t[])
    {
        /* Memory region 0 */
        {
            .name = "/dev/cmem0",
            .offset = 0,
            .range = (512U * 1024U * 1024U)
        },
        /* Memory region 1 */ /* unnecessary, for example */
        {
            .name = "/dev/cmem1",
            .offset = 0,
            .range = (6U * 1024U * 1024U)
        }
#endif
    }
};

/***********************************************************************************************************************
 * @var g_osal_thread_affinity
 * @brief osal resource configuration : CPU affinity value for thread manager.
 **********************************************************************************************************************/
const st_osal_thread_affinity_t g_osal_thread_affinity[] =
{
    {.id = OSAL_THREAD_AFFINITY_INVALID_ID}
};

/***********************************************************************************************************************
 * @var g_osal_interrupt_thread_affinity
 * @brief osal device configuration : CPU affinity value for interrupt manager.
***********************************************************************************************************************/
const st_osal_interrupt_thread_affinity_t g_osal_interrupt_thread_affinity[] =
{
    {.spi_num = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};
