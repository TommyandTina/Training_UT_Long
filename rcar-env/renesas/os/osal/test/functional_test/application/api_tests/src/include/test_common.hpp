/***********************************************************************************************************************
* Copyright [2020-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_common.hpp
* Version :      3.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header file used in OSAL API Functional Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     10.02.2022    Include new header files:
 *                                        - test_osal_device_definition.hpp
 *                                        - test_osal_resource_definition.hpp
 *           2.0.1     15.02.2022    Apply new device and its setting definition.
 *                                   Temporarily disable interrupt context test for S4.
 *                                   Separate setup_hw_register() function for S4.
 *           3.0.0     08.03.2022    Add new OSALINTTESTS SetUp/TearDown function for S4
 *                                   Update setup_hw_register function for S4
 *                                   Update allocate_setup_data_memory, deallocate_data_memory function for all targets
 *           3.0.1     01.04.2022    Remove checking hw addr with "0" in allocate_setup_data_memory function
 *           3.0.2     26.05.2022    Remove software reset applying using OSAL APIs in OSALINTTESTS
 *                                       Add read/write-back initialized value to OSALINTTESTS SetUp/TearDown
 *                                   Update OSALINTTESTS SetUp/TearDown to read/write-back init value for all targets
 *           3.1.0     26.10.2022    Update allocate_setup_data_memory function to use R_OSAL_IoGetAxiIdListForDeviceName
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#if !defined(OSAL_TEST_FRAMEWORK)
#include "gtest/gtest.h"
#include <cstdlib>
#include <vector>
#include <string>
#define PARENT_TEST_FW_CLASS ::testing::Test
#define PARENT_TEST_FW_PARAMETERIZED_CLASS ::testing::TestWithParam<int>
using namespace std;
#define NULL_PTR nullptr
#else
#include "osal_test_framework.hpp"
#include <stdlib.h>
#include <string.h>
#define PARENT_TEST_FW_CLASS OsalTestFrameWork
#define PARENT_TEST_FW_PARAMETERIZED_CLASS OsalTestFrameWorkParameterizedTest
using namespace OsalFWHelper;
#define NULL_PTR NULL
#endif
#include "test_osal_resource_definition.hpp"
#include "test_osal_device_definition.hpp"

#if defined(OSAL_SOC_S4)
#define OSALINTTESTS DISABLED_OSALINTTESTS
#endif

class OSALTESTS : public PARENT_TEST_FW_CLASS
{
public:
// [OSALTESTS_SetUp]
    void SetUp() override
    {
        /* R_OSAL_Initialize is already called before test start */
    }
// [OSALTESTS_SetUp]

// [OSALTESTS_TearDown]
    void TearDown() override
    {
        /* R_OSAL_Deinitialize is called after test end */
    }
// [OSALTESTS_TearDown]

// [available_devices]
    vector<string> available_devices()
    {
        vector<string> ret;
        vector<string> types;
        if (types.empty())
        {
            types = available_types();
        }
        
        //for (const char* type : types)
        for (auto i = types.begin(); i != types.end(); i++)
        {
            size_t bytes, written;
            e_osal_return_t osal_ret = R_OSAL_IoGetDeviceList((*i).c_str(), NULL_PTR, 0, &bytes);
            if (osal_ret == OSAL_RETURN_UNKNOWN_NAME)
            {
                continue;
            }else
            {
                char* buffer = (char*)malloc(bytes);
                EXPECT_EQ(R_OSAL_IoGetDeviceList((*i).c_str(), buffer, bytes, &written), OSAL_RETURN_OK);
                EXPECT_EQ(bytes, written);
                char* token = strtok(buffer, "\n");
                while (token != NULL)
                {
                    ret.push_back(string(token));
                    token = strtok(NULL, "\n");
                }
                free(buffer);
            }
        }
        return ret;
    }
// [available_devices]

// [available_types]
    vector<string> available_types()
    {
        vector<string> ret;
        const char* types[] = {
              "imp_top",
              "imp",
              "imp_slim",
              "imp_ocv",
              "imp_dma",
              "imp_dma_slim",
              "imp_psc",
#if !defined (UNSUPPORT_IMP_CNN)
              "imp_cnn",
#endif
              "imp_spmi",
              "imp_spmc",
              "imp_reg_spmi",
              "imp_ldmac_master",
              "imp_ldmac_checker",
              "imp_dta",
              "ims",
              "imr",
              "cisp",
              "tisp",
              "dof",
              "stv",
              "acf",
              "iccom",
              "ivcp1e",
              "dsp",
              "dsp_dmac",
              "vspx",
              "fbc",
              "fba",
              "rfso",
              "vcp4",
              "fcpc",
#if !defined(UNSUPPORT_IPMMU)
              "ipmmu_mm",
              "ipmmu_cache",
#endif
              "fpcrw",
              "fpcrr",
              "fpcra",
        };
        
        for(auto i = 0; i < sizeof(types)/sizeof(types[0]); i++)
        {
            size_t bytes;
            e_osal_return_t osal_ret = R_OSAL_IoGetDeviceList(types[i], NULL_PTR, 0, &bytes);
            if (osal_ret == OSAL_RETURN_UNKNOWN_NAME)
            {
                continue;
            }else
            {
                ret.push_back(string(types[i]));
            }
        }
        return ret;
    }
// [available_types]

// [findMedian]
    static int sort_compare(const void * a, const void * b)
    {
        return (*(osal_nano_sec_t*)a - *(osal_nano_sec_t*)b);
    }

    double findMedian(osal_nano_sec_t a[], int n)
    {
        qsort(a, n, sizeof(osal_nano_sec_t), sort_compare);
        if (n % 2 != 0)
        {
            return (double)a[n / 2];
        }
        return (double)(a[(n - 1) / 2] + a[n / 2]) / 2.0;
    }
// [findMedian]
};

class OSALINTTESTS : public PARENT_TEST_FW_CLASS
{
public:
    osal_device_handle_t device_handle;
    osal_device_handle_t device_handle2;
    osal_cond_handle_t cond;
    osal_mutex_handle_t mtx;

    osal_mq_handle_t isr_mq;
    st_osal_mq_config mq_config = {1, 4};

#if defined(OSAL_SOC_S4)
    uint16_t init_rdmor_data;
    uint32_t init_data[5];
#else
    uint32_t init_data[3];
#endif
    uint32_t init_clear_data;

// [allocate_setup_data_memory]
    void allocate_setup_data_memory(osal_memory_manager_handle_t* hndl_mmngr, uint32_t *hw_addr, osal_memory_buffer_handle_t* p_buffer_obj)
    {
        osal_memory_buffer_handle_t local_buffer_obj = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
        osal_memory_manager_handle_t local_hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
        uint32_t data;
        void *cpu_ptr[TEST_NUM_OF_BUFFERS];
        size_t size = 0;
        osal_axi_bus_id_t *axi_id = NULL_PTR;
        uintptr_t local_hw_addr = 0;
        st_osal_mmngr_config_t config;
        size_t num_of_max_entry = 0;
        size_t num_of_written_axi_ids = 0;

        /* Open Memory manager */
        EXPECT_EQ(R_OSAL_MmngrGetOsalMaxConfig(&config), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MmngrOpen(&config, &local_hndl_mmngr), OSAL_RETURN_OK);
        EXPECT_NE(local_hndl_mmngr, OSAL_MEMORY_MANAGER_HANDLE_INVALID);
        *hndl_mmngr = local_hndl_mmngr;

        /* Allocate buffer for Display list */
        for(size_t i = 0; i < TEST_NUM_OF_BUFFERS; i++)
        {
            EXPECT_EQ(R_OSAL_MmngrAlloc(local_hndl_mmngr, TEST_BUFFER_SIZE, TEST_BUFFER_ALIGNMENT, &local_buffer_obj), OSAL_RETURN_OK);
            EXPECT_NE(local_buffer_obj, OSAL_MEMORY_BUFFER_HANDLE_INVALID) << "buffer " << i;
            *(p_buffer_obj + i) = local_buffer_obj;

            /* Get HW address of buffer */
            EXPECT_EQ(R_OSAL_IoGetNumOfAxiIdsForDeviceName(TEST_INTCTX_DEVICE_1, &num_of_max_entry), OSAL_RETURN_OK);
            axi_id = (osal_axi_bus_id_t *)malloc(sizeof(osal_axi_bus_id_t) * num_of_max_entry);
            EXPECT_NE(axi_id, NULL_PTR);
            EXPECT_EQ(R_OSAL_IoGetAxiIdListForDeviceName(TEST_INTCTX_DEVICE_1, axi_id, num_of_max_entry, &num_of_written_axi_ids), OSAL_RETURN_OK);
            EXPECT_EQ(R_OSAL_MmngrGetHwAddr(local_buffer_obj, axi_id[0], &local_hw_addr), OSAL_RETURN_OK);
            hw_addr[i] = (uint32_t)local_hw_addr;

            /* Get CPU accessible pointer of the buffer */
            cpu_ptr[i] = NULL_PTR;
            EXPECT_EQ(R_OSAL_MmngrGetCpuPtr(local_buffer_obj, &cpu_ptr[i] ), OSAL_RETURN_OK);
            EXPECT_NE(cpu_ptr[i],NULL_PTR) << "buffer " << i;

            /* Write display list data to the beginning of the buffer */
            data = TEST_DUMMY_BUFFER_DATA[i];
            (void)memcpy(cpu_ptr[i], (void*)&data, sizeof(uint32_t));
            EXPECT_EQ(R_OSAL_MmngrGetSize(local_buffer_obj, &size), OSAL_RETURN_OK);
            EXPECT_NE(size, 0) << "buffer " << i;
            EXPECT_EQ(R_OSAL_MmngrFlush(local_buffer_obj, 0 , size), OSAL_RETURN_OK);
            free(axi_id);
            axi_id = NULL_PTR;
        }
    }
// [allocate_setup_data_memory]

// [deallocate_data_memory]
    void deallocate_data_memory(osal_memory_manager_handle_t hndl_mmngr, osal_memory_buffer_handle_t* buffer_obj)
    {
        ASSERT_NE(hndl_mmngr, OSAL_MEMORY_MANAGER_HANDLE_INVALID);
        for(int i = TEST_NUM_OF_BUFFERS - 1; i >= 0; i--)
        {
            ASSERT_NE(buffer_obj[i], OSAL_MEMORY_BUFFER_HANDLE_INVALID) << "buffer " << i;
            EXPECT_EQ(R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj[i]), OSAL_RETURN_OK) << "buffer " << i;
        }
        EXPECT_EQ(R_OSAL_MmngrClose(hndl_mmngr), OSAL_RETURN_OK);
    }
// [deallocate_data_memory]

// [setup_hw_register]
    void setup_hw_register(osal_device_handle_t* dev, uint32_t* hw_addr)
    {
#if defined(OSAL_SOC_S4)
        /* Config DMA operation register */
        EXPECT_EQ(R_OSAL_IoWrite16(dev[1], TEST_REG_RDMOR, TEST_TRANSFER_RDMOR_VALUE), OSAL_RETURN_OK);
        /* Config transfer src address */
        EXPECT_EQ(R_OSAL_IoWrite32(dev[0], TEST_REG_RDMSAR, hw_addr[0]), OSAL_RETURN_OK);
        /* Config transfer dst address */
        EXPECT_EQ(R_OSAL_IoWrite32(dev[0], TEST_REG_RDMDAR, hw_addr[1]), OSAL_RETURN_OK);
        /* Config transfer count */
        EXPECT_EQ(R_OSAL_IoWrite32(dev[0], TEST_REG_RDMTCR, TEST_TRANSFER_COUNT_VALUE), OSAL_RETURN_OK);
        /* Config transfer steps */
        EXPECT_EQ(R_OSAL_IoWrite32(dev[0], TEST_REG_RDMCHCRB, TEST_TRANSFER_STEP_VALUE), OSAL_RETURN_OK);
        /* Config DMA channel control register */
        EXPECT_EQ(R_OSAL_IoWrite32(dev[0], TEST_REG_RDMCHCR, TEST_TRANSFER_RDMCHCR_VALUE), OSAL_RETURN_OK);
#else
        EXPECT_EQ(R_OSAL_IoWrite32(dev[0], TEST_IMR_REG_DLSAR, hw_addr[0]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(dev[0], TEST_IMR_REG_IMR, TEST_IMR_IMR_VALUE), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(dev[0], TEST_IMR_REG_CR, TEST_IMR_CR_VALUE), OSAL_RETURN_OK);
#endif
    }
// [setup_hw_register]

// [OSALINTTESTS_SetUp]
    void SetUp() override
    {
#if defined(OSAL_SOC_S4)
        /* Open test target device */
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_3, &device_handle2), OSAL_RETURN_OK);
        /* TEST_INTCTX_DEVICE_1 and TEST_INTCTX_DEVICE_3 have same clock/power/reset domain */
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_REG_CLEAR_REGISTER, &init_clear_data), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead16(device_handle2, TEST_REG_RDMOR, &init_rdmor_data), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_REG_RDMSAR, &init_data[0]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_REG_RDMDAR, &init_data[1]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_REG_RDMTCR, &init_data[2]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_REG_RDMCHCRB, &init_data[3]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_REG_RDMCHCR, &init_data[4]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncCondCreate(ISR_COND_ID, &cond), OSAL_RETURN_OK);
        EXPECT_NE(cond, OSAL_COND_HANDLE_INVALID);
        EXPECT_EQ(R_OSAL_ThsyncMutexCreate(ISR_MUTEX_ID, &mtx), OSAL_RETURN_OK);
        EXPECT_NE(mtx, OSAL_MUTEX_HANDLE_INVALID);
        EXPECT_EQ(R_OSAL_MqCreate(&mq_config, INT_SYNC_MQ_ID, &isr_mq), OSAL_RETURN_OK);
        EXPECT_NE(isr_mq, OSAL_MQ_HANDLE_INVALID);
        EXPECT_EQ(R_OSAL_MqReset(isr_mq), OSAL_RETURN_OK);
        EXPECT_NE(device_handle, NULL_PTR);
#else
        /* Open test target device */
        EXPECT_EQ(R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_REG_CLEAR_REGISTER, &init_clear_data), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_IMR_REG_DLSAR, &init_data[0]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_IMR_REG_IMR, &init_data[1]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoRead32(device_handle, TEST_IMR_REG_CR, &init_data[2]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncCondCreate(ISR_COND_ID, &cond), OSAL_RETURN_OK);
        EXPECT_NE(cond, OSAL_COND_HANDLE_INVALID);
        EXPECT_EQ(R_OSAL_ThsyncMutexCreate(ISR_MUTEX_ID, &mtx), OSAL_RETURN_OK);
        EXPECT_NE(mtx, OSAL_MUTEX_HANDLE_INVALID);
        EXPECT_EQ(R_OSAL_MqCreate(&mq_config, INT_SYNC_MQ_ID, &isr_mq), OSAL_RETURN_OK);
        EXPECT_NE(isr_mq, OSAL_MQ_HANDLE_INVALID);
        EXPECT_EQ(R_OSAL_MqReset(isr_mq), OSAL_RETURN_OK);
        EXPECT_NE(device_handle, NULL_PTR);
#endif
    }
// [OSALINTTESTS_SetUp]

// [OSALINTTESTS_TearDown]
    void TearDown() override
    {
#if defined(OSAL_SOC_S4)
        e_osal_pm_required_state_t req_state;
        /* Close test target device */
        /* TEST_INTCTX_DEVICE_1 and TEST_INTCTX_DEVICE_3 have same clock/power/reset domain */
        EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle2, &req_state), OSAL_RETURN_OK);
        EXPECT_EQ(req_state, OSAL_PM_REQUIRED_STATE_REQUIRED);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_REG_CLEAR_REGISTER, init_clear_data), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite16(device_handle2, TEST_REG_RDMOR, init_rdmor_data), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_REG_RDMSAR, init_data[0]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_REG_RDMDAR, init_data[1]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_REG_RDMTCR, init_data[2]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_REG_RDMCHCRB, init_data[3]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_REG_RDMCHCR, init_data[4]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle2), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqDelete(isr_mq), OSAL_RETURN_OK);
#else
        e_osal_pm_required_state_t req_state;
        /* Close test target device */
        EXPECT_EQ(R_OSAL_PmGetRequiredState(device_handle, &req_state), OSAL_RETURN_OK);
        EXPECT_EQ(req_state, OSAL_PM_REQUIRED_STATE_REQUIRED);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_REG_CLEAR_REGISTER, init_clear_data), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_IMR_REG_DLSAR, init_data[0]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_IMR_REG_IMR, init_data[1]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoWrite32(device_handle, TEST_IMR_REG_CR, init_data[2]), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_PmSetRequiredState(device_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_IoDeviceClose(device_handle), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncMutexDestroy(mtx), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_ThsyncCondDestroy(cond), OSAL_RETURN_OK);
        EXPECT_EQ(R_OSAL_MqDelete(isr_mq), OSAL_RETURN_OK);
#endif
    }
// [OSALINTTESTS_TearDown]

    typedef struct st_callback_arg
    {
        osal_cond_handle_t cond_handle;
        osal_mutex_handle_t mtx_handle;
        osal_mq_handle_t    mq_handle;
        int32_t send_msg;
        void* additional_arg;
    }st_callback_arg_t;
};

static st_osal_time_t abs_time_state;

class OsalApiStateTest : public PARENT_TEST_FW_CLASS
{
public:
// [OsalApiStateTest_SetUp]
    static void SetUpTestSuite()
    {
        EXPECT_EQ(R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &abs_time_state), OSAL_RETURN_OK);
        ASSERT_EQ(R_OSAL_Deinitialize(), OSAL_RETURN_OK);
    }
// [OsalApiStateTest_SetUp]

// [OsalApiStateTest_TearDown]
    static void TearDownTestSuite()
    {
        ASSERT_EQ(R_OSAL_Initialize(), OSAL_RETURN_OK);
    }
// [OsalApiStateTest_TearDown]
};
