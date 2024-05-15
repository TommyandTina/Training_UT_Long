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
* File Name    : r_impdrv.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver public header for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_H_
#define R_IMPDRV_H_

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "rcar-xos/osal/r_osal.h"

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_API
 * @defgroup IMPDRV_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
#define IMPDRV_HANDLE_INVALID       ((impdrv_ctrl_handle_t)NULL)    /**< Invalid control handle value       */

#define IMPDRV_COREMAP_MAXID            (16U)           /**< Number of elements in the core map array   */
#define IMPDRV_IRQGROUP_MAXID           (22U)           /**< Core specification limits for IRQ groups   */
#define IMPDRV_IRQMASK_MAX              (9U)            /**< Number of elements in the IRQ mask array   */
#define IMPDRV_INNER_FIXED_VALUE        (22U)           /**< Definition values for internal management  */
#define IMPDRV_CLBRK_ADDR_INVALID       (0xFFFFFFFFU)   /**< Invalid value of CL break address data     */
#define IMPDRV_VDSP_INT_PROG_MEM_SIZE   (0x2000U)       /**< Maximum size of the dsp_fw                 */
#define IMPDRV_VDSP_DTCM_MEM_SIZE       (0x40000U)      /**< Maximum size of the dsp_dtcm               */
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_API
 * @defgroup IMPDRV_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @enum e_impdrv_errorcode_t
 * IMPDRV Return code.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_EC_OK                    = 0,        /**< Successful completion                      */
    IMPDRV_EC_NG_CHECKFAIL          = -200,     /**< Hardware check failure                     */
    IMPDRV_EC_NG_SEQSTATE           = -201,     /**< Sequence Error                             */
    IMPDRV_EC_NG_SYSTEMERROR        = -205,     /**< System error                               */
    IMPDRV_EC_NG_ARGNULL            = -207,     /**< Arguments is NULL                          */
    IMPDRV_EC_NG_PARAM              = -208,     /**< Parameter error                            */
    IMPDRV_EC_NG_ATTRIBUTE          = -209,     /**< Attribute setting error                    */
    IMPDRV_EC_NG_NOTSUPPORT         = -210,     /**< Not supported parameter error              */
    IMPDRV_EC_NG_PMSYSTEMERROR      = -211,     /**< PMSystem error                             */
    IMPDRV_EC_NG_INSTANCE           = -212,     /**< Instance parameter error                   */
    IMPDRV_EC_NG_DSP_HALT           = -213      /**< VDSP error notice                          */
} e_impdrv_errorcode_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_core_type_t
 * IMPDRV Core type.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_CORE_TYPE_INVALID        = 0,        /**< Core type number of Invalid value.         */
    IMPDRV_CORE_TYPE_IMP            ,           /**< Core type number of IMP core.              */
    IMPDRV_CORE_TYPE_IMP_SLIM       ,           /**< Core type number of Slim-IMP core.         */
    IMPDRV_CORE_TYPE_OCV            ,           /**< Core type number of OCV core.              */
    IMPDRV_CORE_TYPE_DMAC           ,           /**< Core type number of DMAC core.             */
    IMPDRV_CORE_TYPE_DMAC_SLIM      ,           /**< Core type number of Slim-DMAC core.        */
    IMPDRV_CORE_TYPE_PSCEXE         ,           /**< Core type number of PSC(exe) core.         */
    IMPDRV_CORE_TYPE_PSCOUT         ,           /**< Core type number of PSC(out) core.         */
    IMPDRV_CORE_TYPE_CNN            ,           /**< Core type number of CNN core.              */
    IMPDRV_CORE_TYPE_DSP            ,           /**< Core type number of DSP.                   */
    IMPDRV_CORE_TYPE_DTA            ,           /**< Core type number of Debug Trace Agent.     */
    IMPDRV_CORE_TYPE_IMR            = 1000,     /**< Core type number of IMR. (Other than IMP-Xn) */
    IMPDRV_CORE_TYPE_LDMAC                      /**< Core type number of Lock Step DMAC core.   */
} e_impdrv_core_type_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_instance_t
 * IMPDRV Instance number.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_INSTANCE_INVALID         = 0,        /**< Invalid value                                              */
    IMPDRV_INSTANCE_0               ,           /**< Identification value for assigning to the IRQ number (0th) */
    IMPDRV_INSTANCE_1               ,           /**< Identification value for assigning to the IRQ number (1st) */
    IMPDRV_INSTANCE_2               ,           /**< Identification value for assigning to the IRQ number (2nd) */
    IMPDRV_INSTANCE_3               ,           /**< Identification value for assigning to the IRQ number (3rd) */
    IMPDRV_INSTANCE_4               ,           /**< Identification value for assigning to the IRQ number (4th) */
    IMPDRV_INSTANCE_5               ,           /**< Identification value for assigning to the IRQ number (5th) */
    IMPDRV_INSTANCE_6                           /**< Identification value for assigning to the IRQ number (6th) */
} e_impdrv_instance_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_irq_group_t
 * IMPDRV Interrupt group number.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_IRQ_GROUP_INVALID        = 0,        /**< Invalid value          */
    IMPDRV_IRQ_GROUP_NONE           ,           /**< No group settings      */
    IMPDRV_IRQ_GROUP_0              ,           /**< IRQ group 0            */
    IMPDRV_IRQ_GROUP_1              ,           /**< IRQ group 1            */
    IMPDRV_IRQ_GROUP_2                          /**< IRQ group 2            */
} e_impdrv_irq_group_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_cb_ret_t
 * IMPDRV Result values for callback function.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_CB_RET_OK                = 0,        /**< Normal end                                             */
    IMPDRV_CB_RET_BEFORE_EXEC       ,           /**< Before CL execute                                      */
    IMPDRV_CB_RET_RESERVED          ,           /**< Reserved definition                                    */
    IMPDRV_CB_RET_ILLEGAL           ,           /**< Illegal error                                          */
    IMPDRV_CB_RET_INT               ,           /**< INT interrupt                                          */
    IMPDRV_CB_RET_CLBRK             ,           /**< CLBRK interrupt                                        */
    IMPDRV_CB_RET_PBCOVF            ,           /**< In the case of "Performance Busy Counter Overflow"     */
    IMPDRV_CB_RET_INT_PBCOVF        ,           /**< In the case of "PBCOVF" and "INT interrupt"            */
    IMPDRV_CB_RET_USIER             ,           /**< In the case of "USIER" error                           */
    IMPDRV_CB_RET_SBO0ME            ,           /**< In the case of "SBO0ME"                                */
    IMPDRV_CB_RET_INT_SBO0ME        ,           /**< In the case of "SBO0ME" and "INT interrupt"            */
    IMPDRV_CB_RET_TRAP_SBO0ME       ,           /**< In the case of "SBO0ME" and "TRAP interrupt"           */
    IMPDRV_CB_RET_WUPCOVF           ,           /**< In the case of "WUPCOVF" error                         */
    IMPDRV_CB_RET_HPINT             ,           /**< In the case of "HPINT interrupt"                       */
    IMPDRV_CB_RET_APIPINT           ,           /**< In the case of "APIPINT interrupt"                     */
    IMPDRV_CB_RET_USINT             ,           /**< In the case of "USINT interrupt"                       */
    IMPDRV_CB_RET_END               ,           /**< In the case of "END interrupt"                         */
    IMPDRV_CB_RET_MSCO              ,           /**< In the case of "MSCO interrupt"                        */
    IMPDRV_CB_RET_UDIVSBRK          ,           /**< In the case of "UDIVSBRK interrupt"                    */
    IMPDRV_CB_RET_UDIPSBRK          ,           /**< In the case of "UDIPSBRK interrupt"                    */
    IMPDRV_CB_RET_DSP_HALT          ,           /**< In the case of "VDSP error notice"                     */
    IMPDRV_CB_DSP_CRC_ERROR         ,           /**< In the case of "VDSP error notice"  Transferred data to internal memory potentially corrupt  */
    IMPDRV_CB_DSP_NORMAL_INT        ,           /**< In the case of "VDSP error notice"  Normal interrupt occurs in DSP                           */
    IMPDRV_CB_DSP_CRITICAL_INT      ,           /**< In the case of "VDSP error notice"  Critical interrupt occurs in DSP                         */
    IMPDRV_CB_DSP_SAFETY_ERROR      ,           /**< In the case of "VDSP error notice"  Error returns                                            */
    IMPDRV_CB_DSP_DIV0              ,           /**< In the case of "VDSP error notice"  Division 0 occurs in DSP                                 */
    IMPDRV_CB_DSP_WDOG_EXPIRE       ,           /**< In the case of "VDSP error notice"  Watchdog timer expires in DSP                            */
    IMPDRV_CB_DSP_STACK_VIOLATION   ,           /**< In the case of "VDSP error notice"  Stack violation occurs in DSP                            */
    IMPDRV_CB_DSP_TIM_FAULT                     /**< In the case of "VDSP error notice"  Fault of timers in DSP (potentially H/W failure)         */
} e_impdrv_cb_ret_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_param_t
 * IMPDRV Generic enum for parameters ON and OFF.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_PARAM_INVALID            = 0,        /**< Invalid value          */
    IMPDRV_PARAM_OFF                ,           /**< Parameter OFF          */
    IMPDRV_PARAM_ON                             /**< Parameter ON           */
} e_impdrv_param_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_irqmask_t
 * IMPDRV interrupt mask.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_IRQMASK_END              = 0,        /**< interrupt mask for END       */
    IMPDRV_IRQMASK_WUPCOVF          ,           /**< interrupt mask for WUPCOVF   */
    IMPDRV_IRQMASK_USIER            ,           /**< interrupt mask for USIER     */
    IMPDRV_IRQMASK_USINT            ,           /**< interrupt mask for USINT     */
    IMPDRV_IRQMASK_PBCOVF           ,           /**< interrupt mask for PBCOVF    */
    IMPDRV_IRQMASK_SBO0ME           ,           /**< interrupt mask for SBO0ME    */
    IMPDRV_IRQMASK_HPINT            ,           /**< interrupt mask for HPINT     */
    IMPDRV_IRQMASK_APIPINT          ,           /**< interrupt mask for APIPINT   */
    IMPDRV_IRQMASK_MSCO                         /**< interrupt mask for MSCO      */
} e_impdrv_irqmask_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_fatalcode_t
 * IMPDRV fatal error code.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_FC_DRV_ERROR            = 0,           /**< Driver Internal Error      */
    IMPDRV_FC_UNEXPECT_INT                        /**< Unexpected interrupt       */
} e_impdrv_fatalcode_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_pm_policy_t
 * IMPDRV Power management policy.
***********************************************************************************************************************/
typedef enum 
{
    IMPDRV_PM_POLICY_INVALID        = 0,        /**< Invalid value              */
    IMPDRV_PM_POLICY_PG             ,           /**< Power gating mode          */
    IMPDRV_PM_POLICY_CG             ,           /**< Clock gating mode          */
    IMPDRV_PM_POLICY_HP                         /**< High performance mode      */
} e_impdrv_pm_policy_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_protect_mode_t
 * IMPDRV Select of Memory protection mode (EDC/ECC).
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_PROTECT_INVALID          = 0,        /**< Invalid value              */
    IMPDRV_PROTECT_EDC              ,           /**< EDC mode                   */
    IMPDRV_PROTECT_ECC                          /**< ECC mode                   */
} e_impdrv_protect_mode_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_reg_req_state_t
 * IMPDRV Required state to the Register read/write function.
***********************************************************************************************************************/
typedef enum {
    IMPDRV_REG_REQ_STATE_INVALID    = 0,        /**< Invalid state              */
    IMPDRV_REG_REQ_STATE_REQUIRED   ,           /**< Required state             */
    IMPDRV_REG_REQ_STATE_RELEASED               /**< Release state              */
} e_impdrv_reg_req_state_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_gosub_cond_t
 * IMPDRV Specifies the condition in case of a conditional GOSUB instruction.
***********************************************************************************************************************/
typedef enum {
    IMPDRV_GOSUB_COND_INVALID       = 0,        /**< Invalid value                                  */
    IMPDRV_GOSUB_COND_NOTEXEC       ,           /**< GOSUB is not executed, NOP is instead executed */
    IMPDRV_GOSUB_COND_EXEC                      /**< GOSUB is executed                              */
} e_impdrv_gosub_cond_t;

/*******************************************************************************************************************//**
 * @enum e_impdrv_pm_state_t
 * IMPDRV Power management state.
***********************************************************************************************************************/
typedef enum {
    IMPDRV_PM_STATE_INVALID         = 0,        /**< Invalid state              */
    IMPDRV_PM_STATE_PG              = 1,        /**< Power Gated state          */
    IMPDRV_PM_STATE_CG              = 2,        /**< Clock Gated state          */
    IMPDRV_PM_STATE_HP              = 4,        /**< High Performance state     */
    IMPDRV_PM_STATE_RESET           = 8,        /**< Reset state                */
    IMPDRV_PM_STATE_READY           = 16        /**< Ready state                */
} e_impdrv_pm_state_t;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_API
 * @defgroup IMPDRV_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @struct st_impdrv_core_info_t
 * IMPDRV Structure of core information.
***********************************************************************************************************************/
typedef struct
{
    e_impdrv_core_type_t    core_type;          /**< Core type                  */
    uint32_t                core_num;           /**< Core number                */
} st_impdrv_core_info_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_resource_t
 * IMPDRV Structure of OSAL resources parameter.
***********************************************************************************************************************/
typedef struct
{
    osal_mutex_id_t                 mutex_id;           /**< Mutex ID used by IMP Driver                */
    osal_milli_sec_t                mutex_time_period;  /**< Millisecond Order Timeout value            */
    osal_mq_id_t                    mq_id;              /**< Queue ID used by IMP Driver                */
    osal_milli_sec_t                mq_time_period;     /**< Millisecond Order Timeout value            */
    e_osal_interrupt_priority_t     irq_priority;       /**< Interrupt Priority                         */
} st_impdrv_resource_t;

/*******************************************************************************************************************//**
 * @typedef p_impdrv_cbfunc_t
 * IMPDRV callback function of execute request.
 * @return          Return value
 * @retval          0                   Returns 0 if successful.
 * @retval          Other than 0        Returns non-zero on failure.
***********************************************************************************************************************/
typedef int32_t (*p_impdrv_cbfunc_t)(
    const st_impdrv_core_info_t *const p_core_info,     /**< Core information                   */
    const e_impdrv_cb_ret_t     ercd,                   /**< Error code                         */
    const int32_t               code,                   /**< TRAP or INT code (8 bit is set)    */
    void                        *const p_callback_args  /**< User argument                      */
);

/*******************************************************************************************************************//**
 * @typedef p_impdrv_cbfunc_fatal_t
 * IMPDRV callback function of fatal notification.
***********************************************************************************************************************/
typedef void (*p_impdrv_cbfunc_fatal_t)(
    const st_impdrv_core_info_t *const p_core_info,     /**< Core information                   */
    const e_impdrv_fatalcode_t  fc_code,                /**< Fatal callback result code         */
    const e_impdrv_errorcode_t  ercd,                   /**< IMP Driver return code             */
    void                        *const p_callback_args  /**< Callback function arguments        */
);

/*******************************************************************************************************************//**
 * @struct st_impdrv_initdata_t
 * IMPDRV Structure for R_IMPDRV_Init argument.
***********************************************************************************************************************/
typedef struct
{
    void                    *p_work_addr;                           /**< Work area address              */
    uint32_t                work_size;                              /**< Work area size                 */
    uint32_t                out_work_size;                          /**< Used memory size               */
    e_impdrv_instance_t     instance_num;                           /**< Instance number                */
    uint32_t                use_core_num;                           /**< Number of core lists           */
    st_impdrv_core_info_t   core_info[IMPDRV_INNER_FIXED_VALUE];    /**< Core lists information         */
    p_impdrv_cbfunc_fatal_t callback_func_fatal;                    /**< fatal callback function        */
    void                    *p_cb_args_fatal;                       /**< fatal callback args            */
    st_impdrv_resource_t    osal_resource;                          /**< OSAL resources information     */
} st_impdrv_initdata_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_irq_group_t
 * IMPDRV Structure for Interrupt group setting.
***********************************************************************************************************************/
typedef struct
{
    e_impdrv_irq_group_t    irq_group;                              /**< Interrupt group number         */
    uint32_t                group_core_num;                         /**< Number of Group cores          */
    st_impdrv_core_info_t   group_core_info[IMPDRV_IRQGROUP_MAXID]; /**< Group core list                */
} st_impdrv_irq_group_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_reg_info_t
 * IMPDRV Hardware register area specification information.
***********************************************************************************************************************/
typedef struct
{
    uintptr_t               addr_phys;                  /**< Top address of hardware register   */
    uint32_t                size;                       /**< Area size of hardware register     */
} st_impdrv_reg_info_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_chk_resource_t
 * OSAL resources in used by IMPDRV for bus interface check.
***********************************************************************************************************************/
typedef struct
{
    osal_memory_buffer_handle_t     memory_handle;      /**< OSAL Memory manager handle         */
    osal_axi_bus_id_t               axi_id;             /**< AXI bus ID                         */
    osal_mq_id_t                    mq_id;              /**< Queue ID used by IMP Driver        */
    osal_milli_sec_t                mq_time_period;     /**< Millisecond Order Timeout value    */
} st_impdrv_chk_resource_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_chk_param_t
 * OSAL resources in used by IMPDRV for configuration register check.
***********************************************************************************************************************/
typedef struct
{
    uintptr_t                       offset;             /**< Offset address of hardware register    */
    uint32_t                        data;               /**< Expected check data                    */
    uint32_t                        bit_mask;           /**< Bitmask value for check data           */
} st_impdrv_chk_param_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_dsp_data_t
 * IMPDRV structure of dsp data information.
***********************************************************************************************************************/
typedef struct
{
    uintptr_t                   addr_phys;              /**< Physical memory address for DSP data   */
    uint32_t                    size;                   /**< DSP data size                          */
} st_impdrv_dsp_data_t;

/*******************************************************************************************************************//**
 * @struct impdrv_ctrl_handle_t
 * Handle for IMPDRV control structure.
***********************************************************************************************************************/
typedef struct st_impdrv_ctl *impdrv_ctrl_handle_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_ctl
 * IMPDRV control structure.
***********************************************************************************************************************/
struct st_impdrv_ctl;

/* Global Function Variables */
extern long unsigned int GlobalBug[2][2];

/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_API IMPDRV API
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/*******************************************************************************************************************//**
* @brief            This function initializes of IMP Driver.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_initdata      Pointer to structure of the Initialize information.
* @param[out]       p_handle        Pointer to handle of the IMP Driver control.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
* @retval           IMPDRV_EC_NG_INSTANCE           Instance parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Init(
    st_impdrv_initdata_t            *const p_initdata,
    impdrv_ctrl_handle_t            *const p_handle
);

/*******************************************************************************************************************//**
* @brief            This function is sets the Memory protection mode.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        protect_mode    Memory protection setting.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_SetMemProtect(
    impdrv_ctrl_handle_t            handle,
    const e_impdrv_protect_mode_t   protect_mode
);

/*******************************************************************************************************************//**
* @brief            This function initializes the Core attribute data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrInit(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function uninitializes of IMP Driver.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Quit(
    impdrv_ctrl_handle_t      handle
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for Core memory initialize.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        enable          Memory initialize setting.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetCoreMemInit(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_param_t          enable
);

/*******************************************************************************************************************//**
* @brief            This function set the core specified by "core" to Ready state.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Start(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function set the core specified by "core" to  Initialized state.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Stop(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for Sync core map.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        core_map        Sync core map array
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetCoreMap(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_core_info_t     core_map[IMPDRV_COREMAP_MAXID]
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for CL address data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        claddr_phys     Physical address of CL data
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetCl(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const uintptr_t                 claddr_phys
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for IRQ mask data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        irq_mask        Array for IRQ mask setting, with refer 'e_impdrv_irqmask_t' definition.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetIrqMask(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const bool                      irq_mask[IMPDRV_IRQMASK_MAX]
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for IRQ group data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_irq_param     IRQ group setting
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_SetIrqGroup(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_irq_group_t     *const p_irq_param
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for CL break address.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        cl_brk_addr     Physical address of CL break
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @note             'cl_brk_addr' is must be aligned with 4 bytes, except for invalid data(IMPDRV_CLBRK_ADDR_INVALID). 
* @note             Invalid data set is same disable interrupt of CL break.
* @note             CL break interrupt are disabled by default.
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetClBrkAddr(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const uintptr_t                 cl_brk_addr
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for conditional GOSUB instruction.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        condition       Conditional GOSUB instruction attribute.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @note             Conditional GOSUB instruction attribute are 'GOSUB is not executed' by default.
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetGosubCond(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_gosub_cond_t     condition
);

/*******************************************************************************************************************//**
* @brief            This function starts executing the command list.
* @syncasync        Asynchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        callback_func   Pointer to Callback function.
* @param[in]        p_callback_args Pointer to Callback argument.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Execute(
    impdrv_ctrl_handle_t        handle,
    const st_impdrv_core_info_t *const p_core_info,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
);

/*******************************************************************************************************************//**
* @brief            This function resumes execution of the command list.
* @syncasync        Asynchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_ResumeExecution(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function changes the Power management policy.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        policy          Power management policy.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_SetPMPolicy(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_pm_policy_t      policy
);

/*******************************************************************************************************************//**
* @brief            This function returns the Power management policy.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[out]       p_policy        Pointer to the Power management policy.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_GetPMPolicy(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    e_impdrv_pm_policy_t            *const p_policy
);

/*******************************************************************************************************************//**
* @brief            This function returns the Power management state.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[out]       p_pmstate       Pointer to the Power management state.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_GetPMState(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    e_impdrv_pm_state_t             *const p_pmstate
);

/*******************************************************************************************************************//**
* @brief            This function returns the Hardware register area specification information in the argument.
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in]        handle          Pointer to Handle of the IMP Driver control.
* @param[in]        p_device_id     Pointer to Target OSAL device ID.
* @param[out]       p_reg_info      Pointer to Hardware register area specification information.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_RegGetHwInfo(
    impdrv_ctrl_handle_t            handle,
    const char                      *const p_device_id,
    st_impdrv_reg_info_t            *const p_reg_info
);

/*******************************************************************************************************************//**
* @brief            This function returns the result read from hardware register.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_device_id     Pointer to the Target OSAL device ID.
* @param[in]        offset          Sets the offset address from Register top.
* @param[out]       p_data          Pointer to the read data to hardware registers.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_RegRead32(
    impdrv_ctrl_handle_t            handle,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    uint32_t                        *const p_data
);

/*******************************************************************************************************************//**
* @brief            This function writes data to hardware register.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_device_id     Pointer to the Target OSAL device ID.
* @param[in]        offset          Sets the Offset address from Register top.
* @param[in]        data            Data written to hardware registers.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_RegWrite32(
    impdrv_ctrl_handle_t            handle,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    const uint32_t                  data
);

/*******************************************************************************************************************//**
* @brief            This function is Open/Close the device associated with the target OSAL ID.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_device_id     Pointer to the Target OSAL device ID.
* @param[in]        new_state       Required state to the Register read/write function.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_RegRequired(
    impdrv_ctrl_handle_t            handle,
    const char                      *const p_device_id,
    const e_impdrv_reg_req_state_t  new_state
);

/*******************************************************************************************************************//**
* @brief            This function set the DSP setting
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in]        p_handle        Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        p_dsp_app       Pointer to the application which run on the DSP
* @param[in]        p_dsp_fw        Pointer to the DSP FW which run on the DSP
* @param[in]        p_dsp_data      Pointer to the data which is used by application on the DSP
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetDsp (
    const impdrv_ctrl_handle_t      handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_dsp_data_t      *const p_dsp_app,
    const st_impdrv_dsp_data_t      *const p_dsp_fw,
    const st_impdrv_dsp_data_t      *const p_dsp_data,
    const st_impdrv_dsp_data_t      *const p_dsp_dtcm
);

/*******************************************************************************************************************//**
* @brief            This function is check the interface of internal bus. However, does not execute fault control.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        p_chk_resource  Pointer to the OSAL resource for bus interface check.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_BusIfCheck(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_chk_resource_t  *const p_chk_resource
);

/*******************************************************************************************************************//**
* @brief            This function is check the configuration register. However, does not execute fault control.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_chk_param     Pointer to Array of check parameters.
* @param[in]        param_num       Number of check parameters.
* @param[in]        p_core_info     Pointer to the Core information data.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_ConfRegCheck(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num,
    const st_impdrv_core_info_t     *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function is check the unintended module stop. However, does not execute fault control.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        handle          Pointer to handle of the IMP Driver control.
* @param[in]        p_chk_param     Pointer to Array of check parameters.
* @param[in]        param_num       Number of check parameters.
* @param[in]        p_core_info     Pointer to the Core information data.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_ModuleStopCheck(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num,
    const st_impdrv_core_info_t     *const p_core_info
);

#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_H_ */

/*======================================================================================================================
End of File
======================================================================================================================*/
