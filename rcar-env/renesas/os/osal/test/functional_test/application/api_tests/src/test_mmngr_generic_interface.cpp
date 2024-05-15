/***********************************************************************************************************************
* Copyright [2021-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_mmngr_generic_interface.cpp
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Memory Manager API Functional Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           2.0.1     15.11.2021    Update Test Name
 *                                   MmngrHandle -> MmngrHandle_TC_09_020_001
 *                                   BufferObject -> BufferObject_TC_09_020_002
 *           2.0.2     18.11.2021    Fix Typo
 *           3.0.0     28.04.2021    Assign a valid value to the invalid buffer_hndl in test MmngrHandle_TC_09_020_001
***********************************************************************************************************************/
#include <stdio.h>
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#if !defined(OSAL_TEST_FRAMEWORK)
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include "rcar-xos/utils/r_cb_wrapper.hpp"
using namespace std;
using namespace renesas::rcarxos::utils;
#define NULL_PTR nullptr
#else
#include "osal_test_framework.hpp"
#include "osal_test_mock_framework.hpp"
#include <stdlib.h>
using namespace OsalFWHelper;
using namespace OsalFWHelper::utils;
#define NULL_PTR NULL
#endif


#if defined (TEST_SIL)
#endif

// [MockMmngr_Interface]
class MockMmngr_Interface {
protected:
    st_osal_memory_manager_obj m_mmngr_obj;

public:
    MockMmngr_Interface() = delete;
    MockMmngr_Interface(osal_memory_manager_handle_t * const mmngr)
    {
        R_OSAL_MmngrInitializeMemoryManagerObj(&m_mmngr_obj);
        m_mmngr_obj.p_ptr_self = this;
        m_mmngr_obj.p_allocate = cb_wrapper<decltype(&MockMmngr_Interface::allocate)>::template make_cb<
            &MockMmngr_Interface::allocate>();
        m_mmngr_obj.p_deallocate = cb_wrapper<decltype(&MockMmngr_Interface::deallocate)>::template make_cb<
            &MockMmngr_Interface::deallocate>();

        m_mmngr_obj.p_getConfig = cb_wrapper<decltype(&MockMmngr_Interface::getConfig)>::template make_cb<
            &MockMmngr_Interface::getConfig>();

        m_mmngr_obj.p_registerMonitor = cb_wrapper<
            decltype(&MockMmngr_Interface::registerMonitor)>::template make_cb<&MockMmngr_Interface::registerMonitor>();

        m_mmngr_obj.p_print =
            cb_wrapper<decltype(&MockMmngr_Interface::print)>::template make_cb<&MockMmngr_Interface::print>();
        *mmngr = &m_mmngr_obj;
    }

    virtual e_osal_return_t allocate(size_t bytes, size_t alignment,
                                     osal_memory_buffer_handle_t * const p_buffer_obj) = 0;
    virtual e_osal_return_t deallocate(osal_memory_buffer_handle_t buffer_obj)         = 0;
    MOCK_METHOD(e_osal_return_t, getConfig, (st_osal_mmngr_config_t * const p_config), (const));
    MOCK_METHOD(e_osal_return_t, registerMonitor,
                (e_osal_mmngr_event_t monitored_event, fp_osal_memory_mngr_cb_t monitor_handler,
                 void * p_monitor_handler_user_arg));
    MOCK_METHOD(e_osal_return_t, print, (FILE * p_output), (const));

    class MockMbuff {
    private:
        st_osal_memory_buffer_obj m_mem_buff;

    public:
        MockMbuff()
        {
            R_OSAL_MmngrInitializeMemoryBufferObj(&m_mem_buff);
            m_mem_buff.p_ptr_self = this;
        };
        e_osal_return_t allocate(osal_memory_buffer_handle_t * const mbuff, osal_memory_manager_handle_t parent)
        {
            m_mem_buff.mmngr_owner_hndl = parent;
            m_mem_buff.p_getCpuPtr =
                cb_wrapper<decltype(&MockMbuff::cpuPtr)>::template make_cb<&MockMbuff::cpuPtr>();
            m_mem_buff.p_getConstCpuPtr =
                cb_wrapper<decltype(&MockMbuff::constCpuPtr)>::template make_cb<&MockMbuff::constCpuPtr>();
            m_mem_buff.p_flush = cb_wrapper<decltype(&MockMbuff::flush)>::template make_cb<&MockMbuff::flush>();
            m_mem_buff.p_invalidate =
                cb_wrapper<decltype(&MockMbuff::invalidate)>::template make_cb<&MockMbuff::invalidate>();
            m_mem_buff.p_getHwAddr =
                cb_wrapper<decltype(&MockMbuff::hwAddr)>::template make_cb<&MockMbuff::hwAddr>();
            m_mem_buff.p_getSize = cb_wrapper<decltype(&MockMbuff::size)>::template make_cb<&MockMbuff::size>();

            m_mem_buff.p_checkBuffer =
                cb_wrapper<decltype(&MockMbuff::checkBuffer)>::template make_cb<&MockMbuff::checkBuffer>();

            m_mem_buff.p_print = cb_wrapper<decltype(&MockMbuff::print)>::template make_cb<&MockMbuff::print>();

            *mbuff = &m_mem_buff;
            return OSAL_RETURN_OK;
        }

        osal_memory_buffer_handle_t osal_hndl(void)
        {
            return &m_mem_buff;
        }

        MOCK_METHOD(e_osal_return_t, cpuPtr, (void ** const pp_cpu_ptr));
        MOCK_METHOD(e_osal_return_t, constCpuPtr, (const void ** const pp_const_cpu_ptr));
        MOCK_METHOD(e_osal_return_t, flush, (size_t offset, size_t size));
        MOCK_METHOD(e_osal_return_t, invalidate, (size_t offset, size_t size));
        MOCK_METHOD(e_osal_return_t, hwAddr, (osal_axi_bus_id_t axi_id, uintptr_t * const p_hw_addr), (const));
        MOCK_METHOD(e_osal_return_t, size, (size_t * const p_size), (const));
        MOCK_METHOD(e_osal_return_t, checkBuffer, (), (const));
        MOCK_METHOD(e_osal_return_t, print, (FILE * p_output), (const));
    };
};
// [MockMmngr_Interface]

// [MockMmngr_Allocate]
class MockMmngr_Allocate : public MockMmngr_Interface {
public:
    MockMmngr_Allocate() = delete;
    MockMmngr_Allocate(osal_memory_manager_handle_t * const mmngr) : MockMmngr_Interface(mmngr)
    {
    }
    e_osal_return_t allocate(size_t bytes, size_t alignment, osal_memory_buffer_handle_t * const p_buffer_obj) override
    {
        (void)bytes;     // ignored
        (void)alignment; // ignored
        MockMmngr_Interface::MockMbuff * buffer =
            static_cast<MockMmngr_Interface::MockMbuff *>((*p_buffer_obj)->p_ptr_self);

        return buffer->allocate(p_buffer_obj, &this->m_mmngr_obj);
    }
    e_osal_return_t deallocate(osal_memory_buffer_handle_t buffer_obj) override
    {
        return OSAL_RETURN_OK;
    }
};
// [MockMmngr_Allocate]

// [MockMmngr_No_Allocate]
class MockMmngr_No_Allocate : public MockMmngr_Interface {
public:

    MockMmngr_No_Allocate() = delete;
    MockMmngr_No_Allocate(osal_memory_manager_handle_t * const mmngr) : MockMmngr_Interface(mmngr)
    {
    }
    MOCK_METHOD(e_osal_return_t, allocate,
                (size_t bytes, size_t alignment, osal_memory_buffer_handle_t * const p_buffer_obj), (override));
    MOCK_METHOD(e_osal_return_t, deallocate, (osal_memory_buffer_handle_t buffer_obj));
};
// [MockMmngr_No_Allocate]

// [monitor_handler_function]
void monitor_handler_function(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error,
                              osal_memory_buffer_handle_t buffer_hndl)
{
    (void)user_arg;
    (void)event;
    (void)error;
    (void)buffer_hndl;
}
// [monitor_handler_function]

/**
 * @page MM_Interface Memory Generic Interface Test
 * @test
 * @test @link MMngrGenericInterfaceImpl_MmngrHandle_TC_09_020_001 @endlink
 * @test @link MMngrGenericInterfaceImpl_BufferObject_TC_09_020_002 @endlink
 * @section MmngrGenericInterfaceClass MmngrGenericInterface test Class
 * Method to preprare the Mock objects for callback functions and initialize Generic Memory Handle Interface and Generic Memory Buffer Interface
 * @snippet test_mmngr_generic_interface.cpp MockMmngr_Interface
 * @snippet test_mmngr_generic_interface.cpp MockMmngr_Allocate
 * @snippet test_mmngr_generic_interface.cpp MockMmngr_No_Allocate
*/

/**
 * @page MM_Interface Memory Generic Interface Test
 * @section MMngrGenericInterfaceImpl_MmngrHandle_TC_09_020_001 MMngrGenericInterfaceImpl.MmngrHandle_TC_09_020_001
 * @subsection MmngrHandle_tsi Test Suite ID
 * MMngrGenericInterfaceImpl
 * @subsection MmngrHandle_tci Test Case ID
 * MmngrInterface_S3_N_TC_09_020_001
 * @subsection MmngrHandle_rr Related Requirement(s)
 * \#276402 \#276407 \#276410 \#276412 \#276544 \#276549 \#276572 \#276743
 * @subsection MmngrHandle_pp Prerequisite/Precondition
 * None
 * @subsection MmngrHandle_tcs Test Case Summary
 * This test shall verify the functionality of Generic Memory Handle Interface
 * @subsection MmngrHandle_tpts Test Procedure/Test Steps
 * -# Using MockMmngr_No_Allocate method to initialize the st_osal_memory_manager_obj "mmngr_hndl" as below:
 *     - Callback function "allocate" for p_allocate
 *     - Callback function "deallocate" for p_deallocate
 *     - Callback function "getConfig" for p_getConfig
 *     - Callback function "registerMonitor" for p_registerMonitor
 *     - Callback function "print" for p_print <br>
 * Create mock objects for these callback function above by using Google test MOCK_METHOD.
 * -# Calling function R_OSAL_MmngrAlloc with all valid parameters
 * -# Allocate a temporary buffer and pass it to buffer_hndl
 * -# Calling function R_OSAL_MmngrDealloc with all valid parameters
 * -# Calling function R_OSAL_MmngrGetConfig with all valid parameters
 * -# Calling function R_OSAL_MmngrRegisterMonitor with all valid parameters
 * -# Calling function R_OSAL_MmngrDebugMmngrPrint with all valid parameters
 * -# Free the temporary buffer
 * @subsection MmngrHandle_ereb Expected Result/Behavior
 * -# Initialize the st_osal_memory_manager_obj "mmngr_hndl" successfully.
 * -# Callback function "allocate" is called.
 * -# The allocation for temporary buffer is success.
 * -# Callback function "deallocate" is called.
 * -# Callback function "getConfig" is called.
 * -# Callback function "registerMonitor" is called.
 * -# Callback function "print" is called.
 * -# Temporary buffer is deallocated successfully.
 * @subsection MmngrHandle_testcase Test Case Example
 * @snippet test_mmngr_generic_interface.cpp MmngrHandle_TC_09_020_001
 * @snippet test_mmngr_generic_interface.cpp monitor_handler_function
 * @snippet test_mmngr_generic_interface.cpp MockMmngr_No_Allocate
 */
// [MmngrHandle_TC_09_020_001]
TEST(MMngrGenericInterfaceImpl, MmngrHandle_TC_09_020_001)
{
    osal_memory_manager_handle_t mmngr_hndl;
    MockMmngr_No_Allocate        mmngr_mock(&mmngr_hndl);
    osal_memory_buffer_handle_t  buffer_hndl;
    osal_memory_buffer_handle_t tmp_buffer_hndl;

    size_t size      = rand();
    size_t alignment = rand();
    EXPECT_CALL(mmngr_mock, allocate(size, alignment, &buffer_hndl));
    R_OSAL_MmngrAlloc(mmngr_hndl, size, alignment, &buffer_hndl);

    tmp_buffer_hndl = (osal_memory_buffer_handle_t)malloc(sizeof(st_osal_memory_buffer_obj));
    EXPECT_NE(tmp_buffer_hndl, NULL_PTR);
    buffer_hndl = tmp_buffer_hndl;

    EXPECT_CALL(mmngr_mock, deallocate(buffer_hndl));
    R_OSAL_MmngrDealloc(mmngr_hndl, buffer_hndl);

    st_osal_mmngr_config_t config;
    EXPECT_CALL(mmngr_mock, getConfig(&config));
    R_OSAL_MmngrGetConfig(mmngr_hndl, &config);

    e_osal_mmngr_event_t monitored_event = OSAL_MMNGR_EVENT_ALLOCATE;
    void *               p_monitor_handler_user_arg = NULL_PTR;
    EXPECT_CALL(mmngr_mock, registerMonitor(monitored_event, monitor_handler_function, p_monitor_handler_user_arg));
    R_OSAL_MmngrRegisterMonitor(mmngr_hndl, monitored_event, monitor_handler_function, p_monitor_handler_user_arg);

    FILE file;
    EXPECT_CALL(mmngr_mock, print(&file));
    R_OSAL_MmngrDebugMmngrPrint(mmngr_hndl, &file);

    free(tmp_buffer_hndl);
}
// [MmngrHandle_TC_09_020_001]

/**
 * @page MM_Interface Memory Generic Interface Test
 * @section MMngrGenericInterfaceImpl_BufferObject_TC_09_020_002 MMngrGenericInterfaceImpl.BufferObject_TC_09_020_002
 * @subsection BufferObject_tsi Test Suite ID
 * MMngrGenericInterfaceImpl
 * @subsection BufferObject_tci Test Case ID
 * MmngrInterface_S3_N_TC_09_020_002
 * @subsection BufferObject_rr Related Requirement(s)
 * \#276399 \#276406 \#276553 \#276554 \#276556 \#276557 \#276560 \#276561 \#276562 \#276574
 * @subsection BufferObject_pp Prerequisite/Precondition
 * None
 * @subsection BufferObject_tcs Test Case Summary
 * This test shall verify the functionality of Generic Memory Buffer Interface
 * @subsection BufferObject_tpts Test Procedure/Test Steps
 * -# Using MockMmngr_Allocate method to initialize the st_osal_memory_manager_obj "mmngr_hndl" as below:
 *     - Callback function "allocate" for p_allocate
 *     - Callback function "deallocate" for p_deallocate
 *     - Callback function "getConfig" for p_getConfig
 *     - Callback function "registerMonitor" for p_registerMonitor
 *     - Callback function "print" for p_print <br>
 * Create mock objects for "getConfig", "registerMonitor" and "print" callback function by using Google test MOCK_METHOD.<br>
 * Overwrite virtual callback function "allocate" and "deallocate" by MockMmngr_Allocate::allocate() and MockMmngr_Allocate::deallocate()
 * -# Using MockMmngr_Interface::MockMbuff method to initialize the st_osal_memory_buffer_obj pointed by "buffer_hndl" with context as below:<br>
 * p_ptr_self is the "this" pointer of MockMmngr_Interface::MockMbuff. <br>
 * Create mock objects for "cpuPtr", "constCpuPtr", "flush", "invalidate", "hwAddr", "size", "checkBuffer" and "print" callback function by using Google test MOCK_METHOD.
 * -# Calling function R_OSAL_MmngrAlloc with all valid parameters
 * -# Calling function R_OSAL_MmngrGetCpuPtr with all valid parameters
 * -# Calling function R_OSAL_MmngrGetConstCpuPtr with all valid parameters
 * -# Calling function R_OSAL_MmngrFlush with all valid parameters
 * -# Calling function R_OSAL_MmngrInvalidate with all valid parameters
 * -# Calling function R_OSAL_MmngrGetHwAddr with all valid parameters
 * -# Calling function R_OSAL_MmngrGetSize with all valid parameters
 * -# Calling function R_OSAL_MmngrCheckBuffer with all valid parameters
 * -# Calling function R_OSAL_MmngrDebugBufferPrint with all valid parameters
 * -# Calling function R_OSAL_MmngrDealloc with all valid parameters
 * @subsection BufferObject_ereb Expected Result/Behavior
 * -# Initialize the st_osal_memory_manager_obj "mmngr_hndl" successfully.
 * -# Initialize the st_osal_memory_buffer_obj pointed by "buffer_hndl" successfully.
 * -# All function pointers of st_osal_memory_buffer_obj pointed by "buffer_hndl" are assigned a valid callback function:
 *     - Callback function "cpuPtr" for p_getCpuPtr
 *     - Callback function "constCpuPtr" for p_getConstCpuPtr
 *     - Callback function "flush" for p_flush
 *     - Callback function "invalidate" for p_invalidate
 *     - Callback function "hwAddr" for p_getHwAddr
 *     - Callback function "size" for p_getSize
 *     - Callback function "checkBuffer" for p_checkBuffer
 *     - Callback function "print" for p_print<br>
 * Function R_OSAL_MmngrAlloc return OSAL_RETURN_OK
 * -# Callback function "cpuPtr" is called.
 * -# Callback function "constCpuPtr" is called.
 * -# Callback function "flush" is called.
 * -# Callback function "invalidate" is called.
 * -# Callback function "hwAddr" is called.
 * -# Callback function "size" is called.
 * -# Callback function "checkBuffer" is called.
 * -# Callback function "print" is called.
 * -# Function R_OSAL_MmngrDealloc is called successfully.
 * @subsection BufferObject_testcase Test Case Example
 * @snippet test_mmngr_generic_interface.cpp BufferObject_TC_09_020_002
 * @snippet test_mmngr_generic_interface.cpp MockMmngr_Allocate
 * @snippet test_mmngr_generic_interface.cpp MockMmngr_Interface
 */
// [BufferObject_TC_09_020_002]
TEST(MMngrGenericInterfaceImpl, BufferObject_TC_09_020_002)
{
    osal_memory_manager_handle_t   mmngr_hndl;
    MockMmngr_Allocate             mmngr_mock(&mmngr_hndl);
    MockMmngr_Interface::MockMbuff buffer_mock;
    osal_memory_buffer_handle_t    buffer_hndl = buffer_mock.osal_hndl();

    size_t size      = rand();
    size_t alignment = rand();
    ASSERT_EQ(R_OSAL_MmngrAlloc(mmngr_hndl, size, alignment, &buffer_hndl), OSAL_RETURN_OK);

    void * cpu_ptr;
    EXPECT_CALL(buffer_mock, cpuPtr(&cpu_ptr));
    R_OSAL_MmngrGetCpuPtr(buffer_hndl, &cpu_ptr);

    const void * const_cpu_ptr;
    EXPECT_CALL(buffer_mock, constCpuPtr(&const_cpu_ptr));
    R_OSAL_MmngrGetConstCpuPtr(buffer_hndl, &const_cpu_ptr);

    size_t offset = rand();
    size               = rand();
    EXPECT_CALL(buffer_mock, flush(offset, size));
    R_OSAL_MmngrFlush(buffer_hndl, offset, size);

    offset = rand();
    size   = rand();
    EXPECT_CALL(buffer_mock, invalidate(offset, size));
    R_OSAL_MmngrInvalidate(buffer_hndl, offset, size);

    osal_axi_bus_id_t axi_id = rand();
    uintptr_t         hw_addr;
    EXPECT_CALL(buffer_mock, hwAddr(axi_id, &hw_addr));
    R_OSAL_MmngrGetHwAddr(buffer_hndl, axi_id, &hw_addr);

    EXPECT_CALL(buffer_mock, size(&size));
    R_OSAL_MmngrGetSize(buffer_hndl, &size);

    EXPECT_CALL(buffer_mock, checkBuffer());
    R_OSAL_MmngrCheckBuffer(buffer_hndl);

    FILE file;
    EXPECT_CALL(buffer_mock, print(&file));
    R_OSAL_MmngrDebugBufferPrint(buffer_hndl, &file);

    R_OSAL_MmngrDealloc(mmngr_hndl, buffer_hndl);
}
// [BufferObject_TC_09_020_002]
