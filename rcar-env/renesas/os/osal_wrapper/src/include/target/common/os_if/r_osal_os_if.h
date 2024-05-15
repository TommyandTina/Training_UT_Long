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
* File Name :    r_osal_os_if.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Wrapper for specific OS(OSAL OS IF) Public Header
***********************************************************************************************************************/

#ifndef OSAL_OS_IF_H
#define OSAL_OS_IF_H

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @typedef osal_device_config_addr_t
 * @brief   Handle of opened device. \n
 *          This type is a typedef definition for a pointer to a struct #st_osal_device_control.
***********************************************************************************************************************/
typedef struct st_osal_device_config_addr *osal_device_config_addr_t;

/*******************************************************************************************************************//**
 * @typedef osal_device_table_t
 * @brief   Handle of opened device. \n
 *          This type is a typedef definition for a pointer to a struct #st_osal_device_control.
***********************************************************************************************************************/
typedef struct st_osal_device_control *osal_device_table_t;

/*******************************************************************************************************************//**
 * @typedef osal_os_threadattr_t
 * @brief   Handle of Thread attribute.
***********************************************************************************************************************/
typedef struct st_osal_os_threadattr *osal_os_threadattr_t;

/*******************************************************************************************************************//**
 * @typedef osal_os_mutexattr_t
 * @brief   Handle of mutex attribute.
***********************************************************************************************************************/
typedef struct st_osal_os_mutexattr *osal_os_mutexattr_t;

/*******************************************************************************************************************//**
 * @typedef osal_os_condattr_t
 * @brief   Handle of condition variable attribute.
***********************************************************************************************************************/
typedef struct st_osal_os_condattr *osal_os_condattr_t;

/*******************************************************************************************************************//**
 * @typedef p_osal_compare_func_t
 * @brief   compare function for quick sort.
***********************************************************************************************************************/
typedef int (*p_osal_compare_func_t)(const void *p_cmp_info1, const void *p_cmp_info2);

/*******************************************************************************************************************//**
 * @typedef osal_axi_bus_info_t
 * @brief   Handle of axi bus information.
***********************************************************************************************************************/
typedef struct st_osal_axi_bus_info *osal_axi_bus_info_t;

/*******************************************************************************************************************//**
 * @typedef osal_os_mutex_t
 * @brief   Handle of mutex information.
***********************************************************************************************************************/
typedef struct st_osal_os_mutex *osal_os_mutex_t;

/*******************************************************************************************************************//**
 * @typedef osal_os_cond_t
 * @brief   Handle of condition variable information.
***********************************************************************************************************************/
typedef struct st_osal_os_cond *osal_os_cond_t;

/*******************************************************************************************************************//**
 * @typedef p_osal_os_thread_func_t
 * @brief   Thread worker function.
***********************************************************************************************************************/
typedef void* (*p_osal_os_thread_func_t)(void* user_arg);

/*******************************************************************************************************************//**
 * @typedef osal_os_thread_t
 * @brief   Handle of thread information.
***********************************************************************************************************************/
typedef struct st_osal_os_thread *osal_os_thread_t;

/*******************************************************************************************************************//**
 * @typedef osal_interrupt_thread_control_t
 * @brief   Handle of interrupt thread information.
***********************************************************************************************************************/
typedef struct st_osal_interrupt_thread_control *osal_interrupt_thread_control_t;

/*******************************************************************************************************************//**
 * @struct osal_mem_mmngr_ptr_self_t
 * @brief structure to memory manager control table
***********************************************************************************************************************/
typedef struct st_osal_mem_mmngr_ptr_self *osal_mem_mmngr_ptr_self_t;

/*******************************************************************************************************************//**
 * @struct osal_os_mqattr_t
 * @brief Handle of Message queue attribute.
***********************************************************************************************************************/
typedef struct st_osal_os_mqattr *osal_os_mqattr_t;

/*******************************************************************************************************************//**
 * @struct osal_os_mq_t
 * @brief Handle of Message queue information.
***********************************************************************************************************************/
typedef struct st_osal_os_mq *osal_os_mq_t;

/***********************************************************************************************************************
 * @typedef e_osal_get_axi_bus_type_t
 * @brief Axi bus getting type.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @enum e_osal_get_axi_bus_type
 * @brief Axi bus getting type.
***********************************************************************************************************************/
typedef enum e_osal_get_axi_bus_type
{
    OSAL_OS_IO_GET_DEVICE_AXI_BUS_ID               = 0,
    OSAL_OS_IO_GET_AXI_BUS_ID_FROM_DEVICE_NAME     = 1,
    OSAL_OS_IO_GET_AXI_BUS_NAME_LIST               = 2,
    OSAL_OS_IO_GET_AXI_BUS_NAME                    = 3,
    OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_AXI_BUS_NAME = 4,
    OSAL_OS_IO_GET_AXI_ID_LIST_FOR_AXI_BUS_NAME    = 5,
    OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_HDL   = 6,
    OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_HDL      = 7,
    OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_NAME  = 8,
    OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_NAME     = 9,
} e_osal_get_axi_bus_type_t;

/*******************************************************************************************************************//**
 * @struct  st_osal_device_control
 * @brief   control of device (Defined in OSAL Wrapper implementation files).
***********************************************************************************************************************/
struct st_osal_device_config_addr;

/*******************************************************************************************************************//**
 * @struct  st_osal_device_control
 * @brief   control of device (Defined in OSAL Wrapper implementation files).
***********************************************************************************************************************/
struct st_osal_device_control;

/*******************************************************************************************************************//**
 * @struct  st_osal_os_threadattr
 * @brief   Structure to thread attribute.
***********************************************************************************************************************/
struct st_osal_os_threadattr;

/*******************************************************************************************************************//**
 * @struct  st_osal_os_mutexattr
 * @brief   Structure to mutex attribute.
***********************************************************************************************************************/
struct st_osal_os_mutexattr;

/*******************************************************************************************************************//**
 * @struct  st_osal_os_condattr
 * @brief   Structure to condition variable attribute.
***********************************************************************************************************************/
struct st_osal_os_condattr;

/*******************************************************************************************************************//**
 * @struct  st_osal_axi_bus_info
 * @brief   Structure to hold axi bus information.
***********************************************************************************************************************/
struct st_osal_axi_bus_info;

/*******************************************************************************************************************//**
 * @struct  st_osal_os_mutex
 * @brief   Control of mutex information.
***********************************************************************************************************************/
struct st_osal_os_mutex;

/*******************************************************************************************************************//**
 * @struct  st_osal_os_cond
 * @brief   Control of condition variable information.
***********************************************************************************************************************/
struct st_osal_os_cond;

/*******************************************************************************************************************//**
 * @struct  st_osal_os_thread
 * @brief   Control of thread information.
***********************************************************************************************************************/
struct st_osal_os_thread;

/*******************************************************************************************************************//**
 * @struct  st_osal_interrupt_thread_control
 * @brief   Control of interrupt thread information.
***********************************************************************************************************************/
struct st_osal_interrupt_thread_control;

/*******************************************************************************************************************//**
 * @struct st_osal_mem_mmngr_ptr_self
 * @brief structure to memory manager control table
***********************************************************************************************************************/
struct st_osal_mem_mmngr_ptr_self;

/*******************************************************************************************************************//**
 * @struct st_osal_os_mqattr
 * @brief Structure to message queue attribute.
***********************************************************************************************************************/
struct st_osal_os_mqattr;

/*******************************************************************************************************************//**
 * @struct st_osal_os_mq
 * @brief structure to message queue information.
***********************************************************************************************************************/
struct st_osal_os_mq;

/*======================================================================================================================
 Public global functions
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            Initialization of thread control in a static environment.
* @param[in]        thread_cfg_num     The number of thread configuration table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadStaticInit(size_t thread_cfg_num);

/*******************************************************************************************************************//**
* @brief            Deinitialization of thread control in a static environment.
* @return           void
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_OS_ThreadStaticDeinit(void);

/*******************************************************************************************************************//**
* @brief            Thread creation to OS independent part.
* @param[in]        p_config        The address of st_osal_thread_config_t.
* @param[in]        func            Function called from the created thread.
* @param[in]        p_arg           Argument passed to func.
* @param[in,out]    thread          The address of OS thread type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_ThreadCreate
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadCreate(const st_osal_thread_config_t* const p_config,
                                       p_osal_os_thread_func_t func,
                                       void* p_arg,
                                       osal_os_thread_t * p_thread);

/*******************************************************************************************************************//**
* @brief            Thread creation to OS independent part(static environment).
* @param[in]        p_config        The address of st_osal_thread_config_t.
* @param[in]        func            Function called from the created thread.
* @param[in]        p_arg           Argument passed to func.
* @param[in]        thread_attr     The address of OS thread attribute type.
* @param[in,out]    thread          The address of OS thread type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_ThreadCreate
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadStaticCreate(const st_osal_thread_config_t* const p_config,
                                             p_osal_os_thread_func_t func,
                                             void* p_arg,
                                             const osal_os_threadattr_t thread_attr,
                                             osal_os_thread_t * p_thread);

/*******************************************************************************************************************//**
* @brief            Thread join to OS independent part.
* @param[in]        thread           The address of OS thread type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_HANDLE
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_ThreadJoin
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadWaitFinish(osal_os_thread_t thread);

/*******************************************************************************************************************//**
* @brief            Thread id obtainment to OS independent part.
* @param[in,out]    p_self_id           Thread ID of the current thread. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_ThreadSelf
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSelf(osal_os_thread_t * p_self_id);

/*******************************************************************************************************************//**
* @brief            Compares two thread identifiers.
* @param[in]        thread1            The OS thread type.
* @param[in]        thread2            The OS thread type.
* @param[in,out]    p_result           The result compares the 2 threads.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_ThreadEqual
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadEqual(osal_os_thread_t thread1,
                                      osal_os_thread_t thread2,
                                      bool* const p_result);

/*******************************************************************************************************************//**
* @brief            Current thread sleep by time stamp to OS independent part.
* @param[in]        p_time_stamp       The pointer addresses of st_osal_time_t 
*                   that holds the timestamp on which the thread will continue.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_HIF_ThreadSleepForTimePeriod
*                   R_OSAL_HIF_ThreadSleepUntilTimeStamp
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSleep(const st_osal_time_t* const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Initialize the thread attribute object.
* @param[in]        priority           The priority.
* @param[in]        stack_size         The stack size.
* @param[in]        p_thread_attr      The address of OS thread attribute type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadInitAttr(e_osal_thread_priority_t priority,
                                         size_t stack_size,
                                         osal_os_threadattr_t* p_thread_attr);

/*******************************************************************************************************************//**
* @brief            Deinitialize the thread attribute object.
* @param[in]        thread_attr        The address of OS thread attribute type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadDeinitAttr(osal_os_threadattr_t thread_attr);

/*******************************************************************************************************************//**
* @brief            Set the thread ID and control table address.
* @param[in]        thread        The address of OS thread type.
* @param[in]        p_arg         The address of thread control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSetSelf(const osal_os_thread_t thread, void *p_arg);

/*******************************************************************************************************************//**
* @brief            Get the address of the control table associated with the thread ID..
* @param[in]        pp_arg         The address of thread control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadStaticSelf(void **pp_arg);

/*******************************************************************************************************************//**
* @brief            Set CPU Affinity.
* @param[in]        id               The value of thread id.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSetCpuAffinity(osal_thread_id_t id);

/*******************************************************************************************************************//**
* @brief            Set task name.
* @param[in]        p_task_name      Pointer of const char task name
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ThreadSetTaskName(const char* p_task_name);

/*******************************************************************************************************************//**
* @brief            Mutex creation to OS independent part.
* @param[in]        mutex_attr        The address of OS mutex attribute type.
* @param[in,out]    p_mutex           Address of OS mutex type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MutexCreate
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MutexCreate(const osal_os_mutexattr_t mutex_attr,
                                      osal_os_mutex_t * p_mutex);

/*******************************************************************************************************************//**
* @brief            Mutex creation to OS independent part.
* @param[in]        mutex           Address of OS mutex type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MutexDestroy
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MutexDestroy(osal_os_mutex_t mutex);

/*******************************************************************************************************************//**
* @brief            Mutex creation to OS independent part.
* @param[in]        mutex           Address of OS mutex type.
* @param[in]        p_time_stamp    The pointer addresses of st_osal_time_t
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_HIF_MutexLockForTimePeriod
*                   R_OSAL_HIF_MutexLockUntilTimeStamp
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MutexTimedLock(osal_os_mutex_t mutex,
                                         const st_osal_time_t* const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Mutex creation to OS independent part.
* @param[in]        mutex           Address of OS mutex type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MutexTryLock
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MutexTryLock(osal_os_mutex_t mutex);

/*******************************************************************************************************************//**
* @brief            Mutex unlock to OS independent part.
* @param[in]        mutex           Address of OS mutex type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MutexUnlock
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MutexUnlock(osal_os_mutex_t mutex);

/*******************************************************************************************************************//**
* @brief            Initialize the mutex attribute object.
* @param[in]        p_mutex_attr        The address of OS mutex attribute type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MutexInitAttr
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MutexInitAttr(osal_os_mutexattr_t * p_mutex_attr);

/*******************************************************************************************************************//**
* @brief            Deinitialize the mutex attribute object.
* @param[in]        mutex_attr        The address of OS mutex attribute type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MutexInitAttr
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MutexDeinitAttr(osal_os_mutexattr_t mutex_attr);

/*******************************************************************************************************************//**
* @brief            Condition creation to OS independent part.
* @param[in,out]    p_cond          Address of OS condition variable type to store the address of the created condition variable.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_CondCreate
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondCreate(osal_os_cond_t * p_cond);

/*******************************************************************************************************************//**
* @brief            Condition creation to OS independent part(static environment).
* @param[in]        cond_attr         The address of OS condition variable attribute type.
* @param[in,out]    p_cond            Address of OS condition variable type to store the address of the created condition variable.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_CondCreate
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondStaticCreate(const osal_os_condattr_t cond_attr,
                                           osal_os_cond_t * p_cond);

/*******************************************************************************************************************//**
* @brief            Condition creation to OS independent part.
* @param[in,out]    cond            Address of OS condition variable type to destroy.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_CondDestroy
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondDestroy(osal_os_cond_t cond);

/*******************************************************************************************************************//**
* @brief            Condition wait by time stamp to OS independent part.
* @param[in,out]    cond            Address of OS condition variable type.
* @param[in]        mutex           Address of OS mutex type.
* @param[in]        p_time_stamp    The pointer addresses of st_osal_time_t
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             This function works on the assumption that the mutex is locked before it is called.
*                   The minimum unit of time is 1 milli second. Digits less than 1 milli second is rounded up to 1 milli second.
*                   If the time stamp is past time, use the current time stamp.
*                   The waiting time may be longer than specified due to the influence of other threads.
* @see              Refer to following the function in this specification
*                   R_OSAL_HIF_CondWaitForTimePeriod
*                   R_OSAL_HIF_CondWaitUntilTimeStamp
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondTimedWait(osal_os_cond_t cond,
                                        osal_os_mutex_t mutex,
                                        const st_osal_time_t* const p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Condition signal to OS independent part.
* @param[in,out]    cond            Address of OS condition variable type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_CondSignal
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondSignal(osal_os_cond_t cond);

/*******************************************************************************************************************//**
* @brief            Condition broadcast to OS independent part.
* @param[in,out]    cond            Address of OS condition variable type.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_CondBroadcast
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondBroadcast(osal_os_cond_t cond);

/*******************************************************************************************************************//**
* @brief            Initialize the condition variable attribute object.
* @param[in,out]    p_cond_attr     The address of OS condition variable attribute type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondInitAttr(osal_os_condattr_t * p_cond_attr);

/*******************************************************************************************************************//**
* @brief            Deinitialize the condition variable attribute object.
* @param[in,out]    cond_attr       The address of OS condition variable attribute type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_CondDeinitAttr(osal_os_condattr_t cond_attr);

/*******************************************************************************************************************//**
* @brief            Memory barrier initialization to OS independent part.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
*                   R_OSAL_RCFG_ThsyncControlInit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MemoryBarrierInit(void);

/*******************************************************************************************************************//**
* @brief            Memory barrier to OS independent part.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             When the memory is shared with multiple devices, such as other CPUs in a multiprocessor system,
*                   or memory-mapped peripherals, out-of-order execution may affect program behavior.
*                   For example, a second CPU may see memory changes made by the first CPU in a sequence
*                   which differs from program order.
*                   This applies to two or more software threads running in a single process 
*                   where by these multiple software threads share a single memory space.
*                   Multiple software threads, within a single process, may run concurrently in a multi-core processor environment.
*                   Together with out-of-order execution this may lead to unpredictable behavior of the system.
* @see              R_OSAL_RCFG_MemoryBarrier
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MemoryBarrier(void);

/*******************************************************************************************************************//**
* @brief            Initializes clock and time related controls on OS dependent.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_ClockTimeControlInit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ClockTimeInit(void);

/*******************************************************************************************************************//**
* @brief            Get time out value by time stamp.
* @param[in,out]    p_time_stamp    The address of output st_osal_time_t. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(st_osal_time_t * p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Get time out value by time stamp.
* @param[in,out]    p_time_stamp    The address of output st_osal_time_t. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_ClockTimeGetHighResoTimeStamp(st_osal_time_t * p_time_stamp);

/*******************************************************************************************************************//**
* @brief            Initializes interrupt related controls on OS dependent.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptControlInit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptInit(void);

/*******************************************************************************************************************//**
* @brief            Initializes interrupt related controls on OS dependent(static environment).
* @param[in]        device_control_table       Address of control of device.
* @param[in]        device_table_num           Num of device control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptControlInit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptStaticInit(osal_device_table_t device_control_table,
                                              size_t device_table_num);

/*******************************************************************************************************************//**
* @brief            Deinitializes interrupt related controls on OS dependent.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptControlDeinit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptDeinit(void);

/*******************************************************************************************************************//**
* @brief            Deinitializes interrupt related controls on OS dependent(static environment).
* @param[in]        device_control_table       Address of control of device.
* @param[in]        device_table_num           Num of device control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptControlDeinit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptStaticDeinit(osal_device_table_t device_control_table,
                                                size_t device_table_num);

/*******************************************************************************************************************//**
* @brief            Returns the check result of whether there is active interrupt resource.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptControlDeinit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptCheckBusy(void);

/*******************************************************************************************************************//**
* @brief            Interrupt service routine registration to OS independent part.
* @param[in]        handle             To set the handle unique value of device handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        irq_channel        To set the value of Irq channel number. This value is from 0 to
*                                      less than the value acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @param[in]        priority_value     The value of interrupt priority. 
* @param[in]        irqHandlerFunction pointer of CallBackFucntion. 
* @param[in]        irq_arg            interrupt handler function's argument. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptRegisterIsr
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptRegisterIsr(osal_device_handle_t handle,
                                               uint64_t irq_channel,
                                               e_osal_interrupt_priority_t priority_value,
                                               p_osal_isr_func_t irqHandlerFunction,
                                               void* p_irq_arg);

/*******************************************************************************************************************//**
* @brief            Interrupt service routine unregistration to OS independent part.
* @param[in]        handle             To set the handle unique value of device handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        irq_channel        To set the value of Irq channel number. This value is from 0 to
*                                      less than the value acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @param[in]        irqHandlerFunction pointer of CallBackFucntion. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptUnregisterIsr
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptUnregisterIsr(osal_device_handle_t handle,
                                                 uint64_t irq_channel,
                                                 p_osal_isr_func_t irqHandlerFunction);

/*******************************************************************************************************************//**
* @brief            Enabling interrupt to OS independent part.
* @param[in]        handle             To set the handle unique value of device handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        irq_channel        To set the value of Irq channel number. This value is from 0 to
*                                      less than the value acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptEnableIsr
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptEnableIsr(osal_device_handle_t handle,
                                             uint64_t irq_channel);

/*******************************************************************************************************************//**
* @brief            Disabling interrupt to OS independent part.
* @param[in]        handle             To set the handle unique value of device handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        irq_channel        To set the value of Irq channel number. This value is from 0 to
*                                      less than the value acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptDisableIsr
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptDisableIsr(osal_device_handle_t handle,
                                              uint64_t irq_channel);

/*******************************************************************************************************************//**
* @brief            Interrupt priority setting to OS independent part.
* @param[in]        handle             To set the handle unique value of device handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        irq_channel        To set the value of Irq channel number. This value is from 0 to
*                                      less than the value acquired with R_OSAL_InterruptGetNumOfIrqChannels().
* @param[in]        priority_value     The value of interrupt priority.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_HANDLE
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_InterruptSetIsrPriority
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptSetIsrPriority(osal_device_handle_t handle,
                                                  uint64_t irq_channel,
                                                  e_osal_interrupt_priority_t priority_value);

/*******************************************************************************************************************//**
* @brief            Get the Interrupt information.
* @param[in]        num             Index of interrupt thread information.
* @param[in,out]    p_result        The pointer for setting the result of the judgment.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_InterruptIsIsrHandler(uint32_t num,
                                                bool * p_result);

/*******************************************************************************************************************//**
* @brief            Initializes IO related controls on OS dependent.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoInit(void);

/*******************************************************************************************************************//**
* @brief            Initialization of IO control in a static environment.
* @param[in,out]    p_num_of_device_config  Pointer to the axi bus information.
* @param[in,out]    p_device_config_addr    Pointer to the device control table.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoStaticInit(size_t * p_num_of_device_config,
                                       osal_device_config_addr_t * p_device_config_addr);

/*******************************************************************************************************************//**
* @brief            Deinitializes IO related controls on OS dependent.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoDeinit(void);

/*******************************************************************************************************************//**
* @brief            Deinitialization of IO control in a static environment.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoStaticDeinit(void);

/*******************************************************************************************************************//**
* @brief            Gets the number of devices and returns it to OS independent part.
* @param[in]        p_deviceType    To set the string of device type.
* @param[in,out]    p_numOfDevices  To set the address of size_t.
*                                   The OSAL set the number of devices corresponding to deviceType.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             This function is called only in dynamic environments to get the number of devices from
*                   OS dependent part.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_IoGetNumOfDevices
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoGetNumOfDevices(const char* p_deviceType,
                                            size_t* const p_numOfDevices);

/*******************************************************************************************************************//**
* @brief            Gets the list of device identifier names for a specific device type and returns it to
*                   OS independent part.
* @param[in]        p_device_type_name  To set the string of device type. 
* @param[in,out]    p_buffer            To set the buffer address.
*                                       The OSAL sets the device id corresponding to the device TYPE as a separated
*                                       string to p_buffer. Needs to acquire the memory area according to list_size.
*                                       The buffer shall use the '\n' character as a delimiter for the string instances.
*                                       The buffer shall be null-terminated, meaning it shall use '\0' at the end of
*                                       the last instance.
* @param[in]        buffer_size         To set the value of p_buffer size by byte. 
* @param[in,out]    p_num_of_byte       To set the address of size_t. The OSAL sets the number of characters in p_buffer. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_UNKNOWN_NAME
* @note             This function is called only in dynamic environments to get the number of devices from
*                   OS dependent part.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_IoGetDeviceList
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoGetDeviceList(const char* p_device_type_name,
                                          char* p_buffer,
                                          size_t buffer_size,
                                          size_t* const p_num_of_byte);

/*******************************************************************************************************************//**
* @brief            Open the device to OS independent part.
* @param[in]        p_id        To set the value of target device name (a.k.a device id) acquired by
*                               R_OSAL_IoGetDeviceList().
* @param[in,out]    p_handle    To set the address of osal_device_handle_t. The OSAL sets the handle. 
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_IoDeviceOpen
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoDeviceOpen(const char* p_id,
                                       osal_device_handle_t * p_handle);

/*******************************************************************************************************************//**
* @brief            Close the device to OS independent part.
* @param[in]        handle      To set the handle obtained by R_OSAL_IoDeviceOpen().
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_IoDeviceClose
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoDeviceClose(osal_device_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Read device to OS independent part.
* @param[in]        handle             To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        offset             To set the value of offset from the base address of handle by byte.
* @param[in]        size               To set the value of register data size by byte.
* @param[in]        unit_size          To set the access unit of the register.
* @param[in,out]    p_data             To set the address of data array to store the data.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_IO
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_IoRead
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoRead(osal_device_handle_t const handle,
                                 uintptr_t offset,
                                 size_t size,
                                 size_t unit_size,
                                 void* p_data);

/*******************************************************************************************************************//**
* @brief            Write device to OS independent part.
* @param[in]        handle             To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        offset             To set the value of offset from the base address of handle by byte.
* @param[in]        size               To set the value of register data size by byte.
* @param[in]        unit_size          To set the access unit of the register.
* @param[in]        p_data             To set the address of data array to store the data.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_IO
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_IoWrite
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoWrite(osal_device_handle_t const handle,
                                  uintptr_t offset,
                                  size_t size,
                                  size_t unit_size,
                                  const void* p_data);

/*******************************************************************************************************************//**
* @brief            Provides AXI bus information.
* @param[in]        get_axi_bus_type    To set the AXI bus getting type.
* @param[in,out]    p_param             To set the address of structure for each type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_IoGetDeviceAxiBusId
*                   R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName
*                   R_OSAL_DCFG_IoGetAxiBusNameList
*                   R_OSAL_DCFG_IoGetAxiBusName
*                   R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName
*                   R_OSAL_DCFG_IoGetAxiIdListForAxiBusName
*                   R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl
*                   R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl
*                   R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName
*                   R_OSAL_DCFG_IoGetAxiIdListForDeviceName
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IoGetAxiBusInformation(e_osal_get_axi_bus_type_t get_axi_bus_type,
                                                 void* p_param);

/*******************************************************************************************************************//**
* @brief            Get the power state.
* @param[in]        handle      To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in,out]    p_value     To set the address of power state.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_PmGetRequiredState
*                   R_OSAL_DCFG_PmGetPolicy
***********************************************************************************************************************/
/* PRQA S 5209 2 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_PmGetPowerState(osal_device_handle_t const handle,
                                          int* p_value);

/*******************************************************************************************************************//**
* @brief            Set the power state.
* @param[in]        handle      To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        value       To set the power state.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_PmSetRequiredState
*                   R_OSAL_DCFG_PmSetPolicy
***********************************************************************************************************************/
/* PRQA S 5209 2 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_PmSetPowerState(osal_device_handle_t const handle,
                                          int value);

/*******************************************************************************************************************//**
* @brief            Get the clock state.
* @param[in]        handle      To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in,out]    p_value     To set the address of clock state.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_PmGetRequiredState
*                   R_OSAL_DCFG_PmGetPolicy
***********************************************************************************************************************/
/* PRQA S 5209 2 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_PmGetClockState(osal_device_handle_t const handle,
                                          int* p_value);

/*******************************************************************************************************************//**
* @brief            Set the clock state.
* @param[in]        handle      To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        value       To set the clock state.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_PmSetRequiredState
*                   R_OSAL_DCFG_PmSetPolicy
***********************************************************************************************************************/
/* PRQA S 5209 2 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_PmSetClockState(osal_device_handle_t const handle,
                                          int value);

/*******************************************************************************************************************//**
* @brief            Get the reset state.
* @param[in]        handle      To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in,out]    p_value     To set the address of reset state.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_PmGetRequiredState
*                   R_OSAL_DCFG_PmGetResetState
***********************************************************************************************************************/
/* PRQA S 5209 2 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_PmGetResetState(osal_device_handle_t const handle,
                                          int* p_value);

/*******************************************************************************************************************//**
* @brief            Set the reset state.
* @param[in]        handle      To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        value       To set the reset state.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_PmSetRequiredState
*                   R_OSAL_DCFG_PmSetResetState
***********************************************************************************************************************/
/* PRQA S 5209 2 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_PmSetResetState(osal_device_handle_t const handle,
                                          int value);

/*******************************************************************************************************************//**
* @brief            Set the post clock.
* @param[in]        handle          To set the handle obtained by R_OSAL_IoDeviceOpen().
* @param[in]        clock_enable    Enable POST Clock for the hierarchy the device.
* @param[in]        clock_divider   Clock divider setting. [clock_divider < 64]
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_DCFG_PmSetPostClock
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_PmSetPostClock(osal_device_handle_t const handle,
                                         bool clock_enable,
                                         uint32_t clock_divider);

/*******************************************************************************************************************//**
* @brief            Memory map to OS independent part.
* @param[in,out]    mngr_control    Memory manager internal information.
* @param[in]        p_usr_config    Pointer to the user specified configuration.
* @param[in]        region_idx      ID of memory region
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MmngrOpen
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MmngrMap(osal_mem_mmngr_ptr_self_t mngr_control,
                                   const st_osal_mmngr_config_t * const p_usr_config,
                                   osal_memory_region_idx_t region_idx);

/*******************************************************************************************************************//**
* @brief            Memory unmap to OS independent part.
* @param[in,out]    mngr_control    Memory manager internal information.
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_DEV
* @retval           OSAL_RETURN_MEM
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MmngrClose
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MmngrUnmap(osal_mem_mmngr_ptr_self_t mngr_control);

/*******************************************************************************************************************//**
* @brief            Memory flush to OS independent part.
* @param[in,out]    memory_buffer_obj_hndl  Handle of the buffer object.
* @param[in]        addr                    Base address for flush.
* @param[in]        offset                  Offset with the buffer object limits
*                                           [range: 0, n*minimumCachelineSize ... maxBufferSize]
*                                           n = 1,2,3,... (e.g. 0, n* 64 Byte)].
* @param[in]        size                    Size of the region to be synchronized
*                                           [range: [buffer_specific, typical 64Bytes]-buffer_size].
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PROHIBITED
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MmngrFlush
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MmngrFlush(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                     uintptr_t addr,
                                     size_t offset,
                                     size_t size);

/*******************************************************************************************************************//**
* @brief            Memory invalidate to OS independent part.
* @param[in,out]    memory_buffer_obj_hndl  Handle of the buffer object.
* @param[in]        addr                    Base address for flush.
* @param[in]        offset                  Offset with the buffer object limits
*                                           [range: 0, n*minimumCachelineSize ... maxBufferSize]
*                                           n = 1,2,3,... (e.g. 0, n* 64 Byte)].
* @param[in]        size                    Size of the region to be synchronized
*                                           [range: [buffer_specific, typical 64Bytes]-buffer_size].
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PROHIBITED
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MmngrInvalidate
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MmngrInvalidate(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                          uintptr_t addr,
                                          size_t offset,
                                          size_t size);

/*******************************************************************************************************************//**
* @brief            Open a file.
* @param[in]        p_pathname      File path
* @param[in]        flags           Open flags
* @return           int
* @retval           File descriptor.
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 5209 2 # The int type of the argument is a specification and cannot be changed. */
int R_OSAL_OS_Open(const char* p_pathname,
                   int flags);

/*******************************************************************************************************************//**
* @brief            Close a file.
* @param[in]        fd              File descriptor
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 5209 1 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_Close(int fd);

/*******************************************************************************************************************//**
* @brief            Read a file.
* @param[in]        fd              File descriptor
* @param[in,out]    p_buf           Read buffer
* @param[in]        count           Read count
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 5209 1 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_Read(int fd,
                               void* p_buf,
                               size_t count);

/*******************************************************************************************************************//**
* @brief            Write a file.
* @param[in]        fd              File descriptor
* @param[in]        p_buf           Write buffer
* @param[in]        count           Write count
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 5209 1 # The int type of the argument is a specification and cannot be changed. */
e_osal_return_t R_OSAL_OS_Write(int fd,
                                const void* p_buf,
                                size_t count);

/*******************************************************************************************************************//**
* @brief            Get the check result whether the current context is in ISR or not to OS independent part.
* @param[in,out]    p_is_isr        To set the address of bool. The OSAL sets the context.
*                                   p_is_isr is set the result of judge.
*                                   - true: interrupt context
*                                   - false: not interrupt context
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_PAR
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              Refer to following the functions in this specification
*                   R_OSAL_DCFG_InterruptIsISRContext
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_IsISRContext(bool *const p_is_isr);

/*******************************************************************************************************************//**
* @brief            Get system page size.
* @return           long
* @retval           System page size.
* @note             None.
* @see              None.
***********************************************************************************************************************/
/* PRQA S 5209 1 # The long type of return value is a specification and cannot be changed. */
long R_OSAL_OS_GetSysPageSize(void);

/*******************************************************************************************************************//**
* @brief            Get string length.
* @param[in]        p_str           Target string.
* @param[in]        maxlen          Maximum length of p_str
* @return           size_t
* @retval           String length.
* @note             None.
* @see              None.
***********************************************************************************************************************/
size_t R_OSAL_OS_Strnlen(const char* p_str,
                         size_t maxlen);

/*******************************************************************************************************************//**
* @brief            OS functionality for binary search
* @param[in]        p_key                   Pointer to the search keyword.
* @param[in]        p_array                 Pointer to the search target.
* @param[in]        array_num               The number of elements of search target.
* @param[in]        array_size              The size of search target.
* @param[in]        compare_function        Pointer to the compare function.
* @return           void*
* @retval           Target address that matches the keyword
* @note             None.
* @see              None.
***********************************************************************************************************************/
void* R_OSAL_OS_BinarySearch(const void* p_key,
                             const void* p_array,
                             size_t array_num,
                             size_t array_size,
                             p_osal_compare_func_t compare_function);

/*******************************************************************************************************************//**
* @brief            OS memory free functionality to OS independent part.
* @param[in,out]    p_ptr                   Pointer to the memory block to free.
* @return           void
* @note             None.
* @see              None.
***********************************************************************************************************************/
void R_OSAL_OS_Free(void *p_ptr);

/*******************************************************************************************************************//**
* @brief            OS malloc functionality to OS independent part.
* @param[in,out]    size                    Size to allocate
* @return           void*
* @retval           A pointer to the memory of the allocated area, or NULL.
* @note             None.
* @see              None.
***********************************************************************************************************************/
void* R_OSAL_OS_Malloc(size_t size);

/*******************************************************************************************************************//**
* @brief            Initialize the message queue attribute object.
* @param[in,out]    p_mq_attr     The address of OS message queue attribute type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqInitAttr(osal_os_mqattr_t * p_mq_attr);

/*******************************************************************************************************************//**
* @brief            Deinitialize the message queue attribute object.
* @param[in,out]    p_mq_attr       The address of OS message queue attribute type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqDeinitAttr(osal_os_mqattr_t mq_attr);

/*******************************************************************************************************************//**
* @brief            Message queue creation to OS independent part.
* @param[in,out]    p_config      The address of #st_osal_mq_config_t
* @param[in]        mq_attr       The address of OS message queue attribute type.
* @param[in,out]    p_mq          Address of OS message queue type to store the address of the created message queue.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MqControlInit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqCreate(const st_osal_mq_config_t* const p_config, const osal_os_mqattr_t mq_attr, osal_os_mq_t * p_mq);

/*******************************************************************************************************************//**
* @brief            Message queue delete to OS independent part.
* @param[in,out]    mq            Address of OS message queue type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              Refer to following the function in this specification
*                   R_OSAL_RCFG_MqControlDeinit
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqDelete(osal_os_mq_t mq);

/*******************************************************************************************************************//**
* @brief            Send a message to message queue on interrupt context.
* @param[in,out]    mq            Address of OS message queue type.
* @param[in]        p_buffer      The address of void* for the send data.
* @param[in]        buffer_size   The p_buffer size by Byte.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqSend(osal_os_mq_t mq, const void * p_buffer, size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Send a message to message queue on normal context.
* @param[in,out]    mq            Address of OS message queue type.
* @param[in]        p_time_stamp  The pointer addresses of st_osal_time_t.
* @param[in]        p_buffer      The address of void* for the send data.
* @param[in]        buffer_size   The p_buffer size by Byte.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqTimedSend(osal_os_mq_t mq, const st_osal_time_t* const p_time_stamp, const void * p_buffer, size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Receive a message to message queue on interrupt context.
* @param[in,out]    mq            Address of OS message queue type.
* @param[in,out]    p_buffer      The address of void* for the receives data.
* @param[in]        buffer_size   The p_buffer size by Byte.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqReceive(osal_os_mq_t mq, void * p_buffer, size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Receive a message to message queue on normal context.
* @param[in,out]    mq            Address of OS message queue type.
* @param[in]        p_time_stamp  The pointer addresses of st_osal_time_t.
* @param[in]        p_buffer      The address of void* for the receives data.
* @param[in]        buffer_size   The p_buffer size by Byte.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_TIME
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqTimedReceive(osal_os_mq_t mq, const st_osal_time_t* const p_time_stamp, void * p_buffer, size_t buffer_size);

/*******************************************************************************************************************//**
* @brief            Check if a message queue is empty or not.
* @param[in,out]    mq            Address of OS message queue type.
* @param[in,out]    p_result      The address of bool for result.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqIsEmpty(osal_os_mq_t mq, bool* const p_result);

/*******************************************************************************************************************//**
* @brief            Check if a message queue is full or not.
* @param[in,out]    mq            Address of OS message queue type.
* @param[in,out]    p_result      The address of bool for result.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqIsFull(osal_os_mq_t mq, bool* const p_result);

/*******************************************************************************************************************//**
* @brief            Delete all messages in the message queue.
* @param[in,out]    mq            Address of OS message queue type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqReset(osal_os_mq_t mq);

/*******************************************************************************************************************//**
* @brief            Returns the check result of whether there is active message queue resource.
* @param[in,out]    mq            Address of OS message queue type.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
* @note             None.
* @see              None.
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_MqCheckBusy(osal_os_mq_t mq);

/** @} OSAL_WRAPPER */

#endif /* OSAL_OS_IF_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
