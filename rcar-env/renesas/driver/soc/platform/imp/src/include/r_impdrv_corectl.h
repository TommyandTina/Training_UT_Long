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
* File Name    : r_impdrv_corectl.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver  private header of impdrv_corectl for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_CORECTL_H
#define R_IMPDRV_CORECTL_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CORECTL
 * @defgroup IMPDRV_CORECTL_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
#define IMPDRV_MASK_U8BIT           (0xFF000000U)       /**< Bit mask for Upper 8 bits                  */
#define IMPDRV_MASK_U9_16BIT        (0x00FF0000U)       /**< Bit mask for Upper 9-16 bits               */
#define IMPDRV_MASK_U16BIT          (0xFFFF0000U)       /**< Bit mask for Upper 16 bits                 */
#define IMPDRV_MASK_L8BIT           (0x000000FFU)       /**< Bit mask for Lower 8 bits                  */
#define IMPDRV_MASK_BIT15           (0x00008000U)       /**< Bit mask for Only 15 bit                   */
#define IMPDRV_INIT_CL_ADDR         (0xFFFFFFFFU)       /**< Initial value of CL address data           */
#define IMPDRV_INIT_DSPMEM_ADDR     (0xFFFFFFFFU)       /**< Initial value of DSP memory address data   */
#define IMPDRV_CALLBACK_CODE_DEF    (-1)                /**< Callback code default value                */
#define IMPDRV_RESERVE_NUM          (2U)                /**< Number of reserve parameter arrays         */
#define IMPDRV_EXEPARAM_MAX         (4U)                /**< Number of extended parameter arrays        */
#define IMPDRV_CORE_NUM_MAX         (8U)                /**< Maximum value of Core number definition    */
#define IMPDRV_CB_RET_MAX           (18U)               /**< Maximum value of Call back reason          */
#define IMPDRV_OFFSET_4BYTE         (4U)                /**< For 4 byte offset calculation              */
#define IMPDRV_SHIFT_8BIT           (8U)                /**< For 8 bit shift calculation                */
#define IMPDRV_SHIFT_16BIT          (16U)               /**< For 16 bit shift calculation               */
#define IMPDRV_SHIFT_24BIT          (24U)               /**< For 24 bit shift calculation               */
#define IMPDRV_SYNCC_REGNUM         (4U)                /**< Array size for SYNCC register              */
#define IMPDRV_EXEPARAM_INVALID     (0U)                /**< Invalid value for Extended parameter       */
#define IMPDRV_EXEPARAM_OFF         (1U)                /**< OFF value for Extended parameter           */
#define IMPDRV_EXEPARAM_ON          (2U)                /**< ON value for Extended parameter            */

/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CORECTL
 * @defgroup IMPDRV_CORECTL_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @enum e_impdrv_state_t
 * IMPDRV Driver Status.
 **********************************************************************************************************************/
typedef enum
{
    IMPDRV_STATE_UNINIT,        /**< Uninitialized state        */
    IMPDRV_STATE_INIT,          /**< Initialization state       */
    IMPDRV_STATE_READY,         /**< Ready state                */
    IMPDRV_STATE_EXEC,          /**< Execution state            */
    IMPDRV_STATE_INT            /**< Interrupting state         */
} e_impdrv_state_t;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CORECTL
 * @defgroup IMPDRV_CORECTL_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @struct st_impdrv_cbinfo_t
 * IMPDRV structure of call back info.
***********************************************************************************************************************/
typedef struct
{
    p_impdrv_cbfunc_t   cbfunc;
    void                *p_cbarg;
} st_impdrv_cbinfo_t;

typedef struct
{
    st_impdrv_dsp_data_t     dsp_app;
    st_impdrv_dsp_data_t     dsp_fw;
    st_impdrv_dsp_data_t     dsp_data;
    st_impdrv_dsp_data_t     dsp_dtcm;
    bool                     is_update;              /**< Data needs to be updated */
    bool                     is_loaded;              /**< Data already stored on DSP. */
    bool                     is_executed;            /**< Application already executed on DSP. */
} st_impdrv_dsp_info_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_corectl_t
 * IMPDRV structure of core control handle.
***********************************************************************************************************************/
typedef struct
{
    st_impdrv_device_handle_t   device_io;                              /**< Device handle                          */
    st_impdrv_queue_handle_t    queue_handle;                           /**< Message queue handle                   */
    st_impdrv_cbinfo_t          cbinfo;                                 /**< Callback function information          */
    e_impdrv_state_t            core_state;                             /**< OSAL device handle                     */
    e_impdrv_param_t            core_mem_init_enable;                   /**< Memory initialization possible flag    */
    uint32_t                    claddr_phys;                            /**< CL Physical address                    */
    uint8_t                     cur_core_map[IMPDRV_COREMAP_MAXID];     /**< Current core map                       */
    bool                        irq_mask[IMPDRV_IRQMASK_MAX];           /**< Mask of IRQ Grouping                   */
    bool                        is_progress_bus_if_check;               /**< Bus interface check in progress        */
    uint8_t                     reserve[IMPDRV_RESERVE_NUM];            /**< Unused area (reserve)                  */
    uint32_t                    param[IMPDRV_EXEPARAM_MAX];             /**< Execution parameters (reserve)         */
    st_impdrv_dsp_info_t        dsp_info;                               /**< DSP information                        */
    st_impdrv_device_handle_t   device_io_dsp[IMPDRV_DSP_DEV_NUM_MAX];  /**< Device handle (DSP Core).              */
} st_impdrv_corectl_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_coretypectl_t
 * IMPDRV structure of core control.
***********************************************************************************************************************/
typedef struct
{
    st_impdrv_corectl_t core_ctl[IMPDRV_CORE_NUM_MAX];      /**< Array of The Core Control handles      */
} st_impdrv_coretypectl_t;

/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_CORECTL IMPDRV CORECTL
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */



#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_CORECTL_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

