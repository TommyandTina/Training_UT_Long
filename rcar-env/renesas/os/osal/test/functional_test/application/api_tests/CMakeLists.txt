#######################################################################################################################
# Copyright [2020-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
#
# The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
# and/or its licensors ("Renesas") and subject to statutory and contractual protections.
#
# Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
# display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
# purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
# SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
# NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
# INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
# OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
# be subject to different terms.
#######################################################################################################################
#----------------------------------------------------------------------------------------------------------------------
# History   Version   DD.MM.YYYY    Description
#            2.0.0     29.10.2021    Create New
#            2.1.0     11.11.2021    Delete DebugPrint Tests using output file
#                                        - 09_015_009
#                                        - 09_016_005
#                                    Add post-test process for Test Case of PM
#                                        - Set OSAL_PM_RESET_STATE_RELEASED at the end of the test
#            2.2.0     11.11.2021    Add MQ Tests
#                                        - 03_007_011, 03_008_010, 03_003_022, 03_004_020, 03_005_021, 03_006_019
#            2.2.1     15.11.2021    Update Test Case Name
#                                    (About test cases that did not have a Test Number)
#            2.3.0     18.11.2021    Add OSAL IO Manager API(AXI Bus Name) Functional Test
#                                    (About test_io_axi_bus_name.cpp)
#            2.3.1     18.11.2021    Fix Typo (Fix Doxygen Comment)
#            2.4.0     15.12.2021    Add Interrupt Sequence Tests
#            3.0.0     24.01.2022    Update ThreadEqual_TC_01_005_002
#                      10.02.2022    Update Header files
#                                        - Remove test_definition.hpp
#                                        - Add test_osal_device_definition.hpp and test_osal_resource_definition.hpp
#                      01.03.2022    Add test source file test_system_ram.cpp
#                      04.03.2022    Add Unsupport Device Flag for S4
#            3.1.0     25.03.2022    Update MqReceiveForTimePeriod_TC_03_006_005
#                                    Update test InterruptIsISRContext_TC_05_007_004
#                                    Add ThreadSelf_TC_01_004_004
#                                    Add tests of IoBlockRead8/16/32/64 and IoBlockWrite8/16/32/64 with input size is 0.
#                                        - IoBlockRead8_TC_06_009_010, IoBlockWrite8_TC_06_017_010
#                                        - IoBlockRead16_TC_06_010_010, IoBlockWrite16_TC_06_018_010
#                                        - IoBlockRead32_TC_06_011_010, IoBlockWrite32_TC_06_019_010
#                                        - IoBlockRead64_TC_06_012_010, IoBlockWrite64_TC_06_020_010
#                                    Add new test InterruptUnregisterIsr_TC_05_003_013
#                                    Add new macro TEST_IO_SIZE_ZERO for S4/V4H/V3x
#                                    Add new macro TEST_IO_ACCESS_SIZE for S4/V4H/V3x
#                                    Add new macro NEGATIVE_TIME
#            3.1.1     30.03.2022    Enable disabled test on SIL
#                                    Add size and align in test case MmngrDebugMmngrPrint_TC_09_015_003
#            3.1.2     01.04.2022    Remove checking hw addr with "0" in allocate_setup_data_memory function
#            3.1.3     04.04.2022    Add new macro SYNC_MQ_TIME
#                                    Update description comment for TEST_MQ_COND_ID
#                                    Update synchronization method for the below ThreadSync tests:
#                                        - CondVar_TC_12_001_002
#                                        - ThsyncCondDestroy_TC_02_008_004
#                                    Add new macro SYNC_MQ_ID
#                                    Update synchronization method for the below Thread tests:
#                                        - ThreadEqual_TC_01_005_001
#                                    Update Synchronization method for the below IO test:
#                                        - IoDeviceClose_TC_06_004_006
#                                    Update Synchronization method for the below MQ test:
#                                        - MqDelete_TC_03_002_005
#                                        - MqSendUntilTimeStamp_TC_03_003_013
#                                        - MqSendUntilTimeStamp_TC_03_003_014
#                                        - MqSendForTimePeriod_TC_03_004_009
#                                        - MqSendForTimePeriod_TC_03_004_010
#                                        - MqReceiveUntilTimeStamp_TC_03_005_013
#                                        - MqReceiveUntilTimeStamp_TC_03_005_014
#                                        - MqReceiveForTimePeriod_TC_03_006_009
#                                        - MqReceiveForTimePeriod_TC_03_006_010
#                                        - MqReceiveForTimePeriod_TC_03_006_012
#                                        - MqReset_TC_03_010_004
#                                        - MqReset_TC_03_010_005
#                                        - MqReset_TC_03_010_008
#                                        - MqReset_TC_03_010_009
#                                    Update Synchronization method for the below init/deinit test:
#                                        - InitDeinit_TC_10_001_008
#                                        - InitDeinit_TC_10_001_010
#                                        - InitDeinit_TC_10_001_011
#                                        - InitDeinit_TC_10_001_014
#            3.1.4     19.04.2022    Apply API functional test for eMCOS
#                      28.04.2022    Assign a valid value to the invalid buffer_hndl in test MmngrHandle_TC_09_020_001
#            3.1.5     19.05.2022    Update AXI Bus Name test data for V3M2 SIL
#                                    Update TEST_AXI_BUS_NAME_DEVICE_INVALID macro for SIL
#                      23.05.2022    Disable un-supported devices for SIL
#                      24.05.2022    Add new test PmSoftwareReset_TC_07_012_001
#                                    Add new test PmSoftwareReset_TC_07_012_002
#                                    Add new test PmSoftwareReset_TC_07_012_003
#                                    Add new test PmSoftwareReset_TC_07_012_004
#                      26.05.2022    Remove software reset applying using OSAL APIs in OSALINTTESTS test
#                                    Add read/write-back initialized value to OSALINTTESTS SetUp/TearDown
#                                    Remove software reset applying using OSAL APIs in PM sequence test
#                                    Add read/write back the initialize register to PMApiSequenceTest SetUp/TearDown
#                                    Update target register and written data for PM sequence test on V4H and V3x
#                                    Remove clean_register function in IO test file
#                                    Add read/write-back initialized value to IO test that used to reset target register
#                                    Update target register and written data for IO API test on V4H and V3x
#                      30.05.2022    Enable disabled test cases related to shared power domain device
#                      03.06.2022    Update target test device name and type used for S4 testing
#            3.2.0     24.06.2022    Update the below test cases of R_OSAL_IoGetAxiBusName, R_OSAL_IoGetAxiBusNameList:
#                                        - IoGetAxiBusName_TC_06_022_001
#                                        - IoGetAxiBusName_TC_06_022_002
#                                        - IoGetAxiBusName_TC_06_022_003
#                                        - IoGetAxiBusName_TC_06_022_004
#                                        - IoGetAxiBusNameList_TC_06_023_001
#                                    Add new TEST_AXI_BUS_NAME and TEST_NUM_AXI_ID definition
#                                    Add new macro TEST_NUM_OF_MEMORY_REGION and TEST_MEMORY_REGION_DEFAULT
#                                    Add new test cases for new multi memory region APIs
#                      04.07.2022    Add SUPPORT_NEW_AXI_BUS_NAME_RULE for Linux HIL
#                      11.07.2022    Add Support Multi Memory Region Test for Linux/QNX HIL
#            3.3.0     13.07.2022    Add new test cases using assigned device handle that isn't supported by PM
#                                    Add the below new macros for PM test:
#                                       - TEST_UNSUPPORT_POWER_DEVICE
#                                       - TEST_UNSUPPORT_CLOCK_DEVICE
#                                       - TEST_UNSUPPORT_RESET_DEVICE
#                                       - PM_UNSUPPORT_PM_EXPECTED_VALUE
#            3.3.0     28.07.2022    Add new memory test cases in multi-threads
#                                    Move InitDeinit_TC_10_001_015 and InitDeinit_TC_10_001_016 to InitDeinitApiTest
#            3.4.0     26.08.2022    Add IO Tests about one handle access from Multi threads
#                                       - IoDeviceOpen_TC_06_003_008
#                                       - IoWrite8_TC_06_013_008, IoWrite16_TC_06_014_008
#                                       - IoWrite32_TC_06_015_008, IoWrite64_TC_06_016_008
#                                       - IoBlockWrite8_TC_06_017_011, IoBlockWrite16_TC_06_018_011
#                                       - IoBlockWrite32_TC_06_019_011, IoBlockWrite64_TC_06_020_011
#            3.5.0     05.10.2022    Enable ipmmu functional test for QNX/V4H
#            3.6.0     26.10.2022    Update Header file test_common.hpp
#                                        - Update allocate_setup_data_memory function to use R_OSAL_IoGetAxiIdListForDeviceName
#            3.7.0     11.11.2022    Add test cases of MmngrGetHwAddr with invalid AXI ID as below
#                                        - MmngrGetHwAddr_TC_09_012_009
#                                        - MmngrGetHwAddr_TC_09_012_010
#                                        - MmngrGetHwAddr_TC_09_012_011
#                                        - MmngrGetHwAddr_TC_09_012_012
#            3.8.0     21.11.2022    Add the below new macros for PM test:
#                                      - TEST_PM_DEVICE_2_DEPENDENCE
#                                      - TEST_PM_DEVICE_SEQUENCE_PG
#                                    Add reinitialize power policy of devices in the SetUp and TearDown of PMApiTest
#                                    Add new test for initialize leak check of power lowest state 07_010_012 to 07_010_014
#                                    Add new test for checking set policy of device into PG 07_011_022 to 07_011_027
#                                    Add powering down operation of "imp_01" device for test cases that set "imp_00" to PG
#                      25.11.2022    Add new test cases to confirm the PM required state transition:
#                                       - PmSetRequiredState_TC_07_001_012
#                                       - PmSetRequiredState_TC_07_001_013
#                                       - PmSetRequiredState_TC_07_001_014
#                                       - PmSetRequiredState_TC_07_001_015
#                                       - PmSetRequiredState_TC_07_001_016
#                                       - PmSetRequiredState_TC_07_001_017
#                                       - PmSetRequiredState_TC_07_001_018
#                                       - PmSetRequiredState_TC_07_001_019
#                                       - PmSetRequiredState_TC_07_001_020
#                      05.12.2022    Enable of POSTCLOCK_API for S4
#            3.9.0     15.12.2022    Add new test cases as below:
#                                       - ClockTimeCalculateTimeDifference_TC_04_002_014
#                                       - ClockTimeCalculateTimeDifference_TC_04_002_015
#                                       - ClockTimeCalculateTimeDifference_TC_04_002_016
#                                       - ClockTimeCalculateTimeDifference_TC_04_002_017
#                                       - ClockTimeCalculateTimeDifference_TC_04_002_018
#                                       - ClockTimeCalculateTimeDifference_TC_04_002_019
#            3.10.0     10.01.2023    Disable overflow/underflow without PROT test cases:
#                                       - MmngrCheckBuffer_TC_09_014_005
#                                       - MmngrCheckBuffer_TC_09_014_009
#                                       - MmngrCheckBuffer_TC_09_014_007
#                                       - MmngrCheckBuffer_TC_09_014_011
#                       12.01.2023    Add new tests and update the exsting tests in resource managers to cover OSAL API specification
#                                     Add new tests and update the exsting tests in device managers to cover OSAL API specification
#            3.11.0     02.02.2023    Add the new boundary test cases for resource and device managers
#                       06.02.2023    Add new boundary test cases to memory manager
#                                     Add new tests and update the exsting tests in memory manager to cover OSAL API specification
#            3.12.0     21.02.2023    Add definition of S4N
#                                     Add the new test cases for R_OSAL_MmngrClose
#            3.13.0     07.03.2023    Add new interrupt context test for MQ receive APIs
#            3.14.0     10.04.2023    Update expected value for test case MmngrClose_TC_09_002_019
#                                     Update expected value of median time on SIL Windows performance test
#                                     Update the thread synchronization for test ThreadJoin_TC_01_006_006
#                                     Update value of the following macros for SIL test V3M2/V3Hx:
#                                       - TEST_UNSUPPORT_POWER_DEVICE
#                                       - TEST_UNSUPPORT_CLOCK_DEVICE
#                                       - TEST_UNSUPPORT_RESET_DEVICE
#                                       - PM_UNSUPPORT_PM_EXPECTED_VALUE
#            3.14.1     05.10.2023    Enable SUPPORT_VIP_WRAPPER Test on OSAL for SIL
#            3.14.2     09.11.2023    Add Test Setting of V4M
#            3.15.0     09.01.2023    Add new test cases for memory manager of OSAL for SIL
#----------------------------------------------------------------------------------------------------------------------
#----------------------------------------------------------------------------------------------------------------------
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "osal_functional_test")
set(app_version "3.15.0")

set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.16.3)
project(${app_name})

#----------------------------------------------------------------------------------------------------------------------
# R-Car xOS SoC/Board selection
#----------------------------------------------------------------------------------------------------------------------

if(NOT RCAR_SOC) # This guard allows to set RCAR_SOC externally, e.g. via CMake Commandline during configuration
    set(RCAR_SOC
        V3H1 # Allowed values are at the moment V3H1, V3M2. Availability depends on the package you have.
    )
endif()

# Alternative: Selection via your board:
# if(NOT RCAR_BOARD) # This guard allows to set RCAR_BOARD externally, e.g. via CMake Commandline during configuration
#     set(RCAR_BOARD
#         v3hsk # eagle, v3msk, condor, v3hsk. Availability depends on the package you have.
#     )
# endif()

#----------------------------------------------------------------------------------------------------------------------
# Find R-Car xOS platform with your requested configuration
# - In case you need a specific version, usage: find_package(rcar-xos v0.3.0 EXACT REQUIRED)
# - In case you need at least version, usage: find_package(rcar-xos v0.3.0 REQUIRED)
#
# If the command fails to find the package:
#   Make sure rcar-xos config files can be found via standard variables set in your system:
#   PATH environment variable or rcar-xos_DIR (environment or CMake variable set via commandline)
#
# See R-Car xOS documentation for more help or https://cmake.org/cmake/help/latest/command/find_package.html
#----------------------------------------------------------------------------------------------------------------------
find_package(rcar-xos REQUIRED)

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

    ## ---- OSAL API ------
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_memory_barrier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_cond_var.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_mq.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_timeclock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_io_extension_api.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_memory_api_basic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_mmngr_generic_interface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_interrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_pm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_version.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_resource_measurement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_init_deinit.cpp
)

if(NOT RCAR_SOC STREQUAL "v3h1")
    list(APPEND source
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_system_ram.cpp
    )
endif()


# ---- Static Env Resource Info ----
if(RCAR_TARGET_OS STREQUAL "qnx")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_osal_functional_test_configuration.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test_io_axi_bus_name.cpp
    )
endif()

if(RCAR_TARGET_OS STREQUAL "emcos")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_osal_functional_test_resource_info.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test_io_axi_bus_name.cpp
    )
endif()


if(RCAR_XIL STREQUAL hil)
    if(RCAR_TARGET_OS STREQUAL "linux")
        list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_osal_functional_test_configuration.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test_io_axi_bus_name.cpp
        )
    endif()
endif()

if(RCAR_XIL STREQUAL sil)
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test_io_axi_bus_name.cpp
    )
endif()

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/test_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/test_osal_resource_definition.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/test_osal_device_definition.hpp
)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${rcar-xos_INCLUDE_DIRS}
)

#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(link_lib
    utils
    gtest
    gmock
    osal
    osal_wrapper
)

if(RCAR_TARGET_OS STREQUAL "emcos")
    message("OSAL for emcos Google Test Build")
    # When armclang, It will be a warning　of duplicate link, so remave it.
    list(REMOVE_ITEM link_lib
        gtest
    )
    # Add google test configuration, don't use cxxabi.h
	add_definitions(-DGTEST_HAS_CXXABI_H_=0)
endif()

# [optional] for 3rd Party libraries
set(link_dir)


if (CMAKE_LINKER MATCHES "armlink")
    if(RCAR_TARGET_OS STREQUAL "emcos")
        set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/default_cxx_proc_scatter.txt")
     endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# [Optional] Any specifics that an application may require, like OS specifics, SoC specifics
#----------------------------------------------------------------------------------------------------------------------
include(CheckSymbolExists)
check_symbol_exists(mallinfo2 "malloc.h" HAVE_MALLINFO2)
check_symbol_exists(mallinfo "malloc.h" HAVE_MALLINFO)

#if(TARGET imp)
#    list(APPEND source
#        ${CMAKE_CURRENT_SOURCE_DIR}/src/test_impdrv.cpp
#    )
#    list(APPEND link_lib
#        imp
#        )
#endif()

if(RCAR_TARGET_OS STREQUAL "qnx")
    # Remove -fpic flag when using googletest
    string(REPLACE "-fpic" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REPLACE "-fpic" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_CONFIG "http.sslVerify=false"
    GIT_REPOSITORY https://rcar-env.dgn.renesas.com/gitlab/google/googletest.git
    GIT_TAG        release-1.10.0
)
set(googletest_BUILD_GMOCK TRUE)
set(BUILD_GMOCK TRUE)
set(googletest_gtest_force_shared_crt ON)
set(gtest_force_shared_crt ON)

if(CMAKE_TOOLCHAIN_FILE MATCHES "armclang") # WA for for broken MOCK wiht ARMClang for Linux environment
    set(gtest_disable_pthreads ON)
endif()

FetchContent_MakeAvailable(googletest)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL QNX)
    # on QNX the googletest requires the extensions to be enabled to compile. Enable them only for the targets
    set_target_properties(gtest gmock gtest_main gmock_main PROPERTIES CXX_EXTENSIONS ON)
    set(CMAKE_CXX_EXTENSIONS ON)

    if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0.0)
        # newer QNX version do have regex seperated for the C-Library. This is not covered by googletest yet.
        list(APPEND link_lib regex)
    endif()
endif()

if(RCAR_SOC STREQUAL "v3u")
    add_definitions(-DOSAL_SOC_V3U)
    add_definitions(-DSUPPORT_32BIT_BUS)
    add_definitions(-DSUPPORT_64BIT_BUS)
elseif(RCAR_SOC STREQUAL "v3h1")
    add_definitions(-DOSAL_SOC_V3H1)
    add_definitions(-DSUPPORT_32BIT_BUS)
    add_definitions(-DSUPPORT_64BIT_BUS)
elseif(RCAR_SOC STREQUAL "v3m2")
    add_definitions(-DOSAL_SOC_V3M2)
    add_definitions(-DSUPPORT_32BIT_BUS)
    add_definitions(-DSUPPORT_64BIT_BUS)
elseif(RCAR_SOC STREQUAL "v3h2")
    add_definitions(-DOSAL_SOC_V3H2)
    add_definitions(-DSUPPORT_32BIT_BUS)
    add_definitions(-DSUPPORT_64BIT_BUS)
elseif(RCAR_SOC STREQUAL "v4h")
    add_definitions(-DOSAL_SOC_V4H)
    add_definitions(-DSUPPORT_32BIT_BUS)
    add_definitions(-DSUPPORT_64BIT_BUS)
elseif(RCAR_SOC STREQUAL "v4h2")
    add_definitions(-DOSAL_SOC_V4H2)
    add_definitions(-DSUPPORT_32BIT_BUS)
    add_definitions(-DSUPPORT_64BIT_BUS)
elseif((RCAR_SOC STREQUAL "s4") OR (RCAR_SOC STREQUAL "s4n"))
    add_definitions(-DOSAL_SOC_S4)
    add_definitions(-DSUPPORT_32BIT_BUS)
    add_definitions(-DSUPPORT_64BIT_BUS)
elseif(RCAR_SOC STREQUAL "v4m")
    add_definitions(-DOSAL_SOC_V4M)
    add_definitions(-DSUPPORT_32BIT_BUS)
    add_definitions(-DSUPPORT_64BIT_BUS)
endif()

if(RCAR_XIL STREQUAL hil)
    if(RCAR_TARGET_OS STREQUAL "linux")
        add_definitions(-DTEST_LINUX)
        add_definitions(-DSUPPORT_NEW_AXI_BUS_NAME_RULE)
        if(RCAR_SOC STREQUAL "v3u")
            add_definitions(-DUNSUPPORT_SYSTEM_RAM_TEST)
        elseif(RCAR_SOC STREQUAL "v3h1")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_SYSTEM_RAM_TEST)
        elseif(RCAR_SOC STREQUAL "v3h2")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
        elseif(RCAR_SOC STREQUAL "v3m2")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
        elseif(RCAR_SOC STREQUAL "v4h")
            add_definitions(-DUNSUPPORT_GFX)
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_FCPR)
            add_definitions(-DUNSUPPORT_VIDEO_INPUT)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_VSPD)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_CSI2)
            add_definitions(-DUNSUPPORT_VDSP)
        elseif(RCAR_SOC STREQUAL "v4h2")
            add_definitions(-DUNSUPPORT_GFX)
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_FCPR)
            add_definitions(-DUNSUPPORT_VIDEO_INPUT)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_VSPD)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_CSI2)
            add_definitions(-DUNSUPPORT_VDSP)
        elseif((RCAR_SOC STREQUAL "s4") OR (RCAR_SOC STREQUAL "s4n"))
        elseif(RCAR_SOC STREQUAL "v4m")
            add_definitions(-DUNSUPPORT_GFX)
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_FCPR)
            add_definitions(-DUNSUPPORT_VIDEO_INPUT)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_VSPD)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_CSI2)
            add_definitions(-DUNSUPPORT_VDSP)
            add_definitions(-DUNSUPPORT_FCPC)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DSC)
            add_definitions(-DUNSUPPORT_QOS)
            add_definitions(-DUNSUPPORT_RCARBUSMONI)
        endif()
    elseif(RCAR_TARGET_OS STREQUAL "qnx")
        set(test_arg "--gtest_filter=-InitDeinitApiTest.InitDeinit_TC_10_001_001:OsalResourcePerformance*")
        add_definitions(-DTEST_QNX)
        add_definitions(-DSUPPORT_NEW_AXI_BUS_NAME_RULE)
        add_definitions(-DSUPPORT_STATIC_ENV)
        if(RCAR_SOC STREQUAL "v3u")
            add_definitions(-DUNSUPPORT_SYSTEM_RAM_TEST)
        elseif(RCAR_SOC STREQUAL "v3h1")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_SYSTEM_RAM_TEST)
        elseif(RCAR_SOC STREQUAL "v3h2")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_SYSTEM_RAM_TEST)
        elseif(RCAR_SOC STREQUAL "v3m2")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_SYSTEM_RAM_TEST)
        elseif(RCAR_SOC STREQUAL "v4h")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_FCPR)
        elseif(RCAR_SOC STREQUAL "v4h2")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_FCPR)
        elseif((RCAR_SOC STREQUAL "s4") OR (RCAR_SOC STREQUAL "s4n"))
        elseif(RCAR_SOC STREQUAL "v4m")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_FCPR)
            add_definitions(-DUNSUPPORT_FCPC)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DSC)
            add_definitions(-DUNSUPPORT_QOS)
            add_definitions(-DUNSUPPORT_RCARBUSMONI)
        endif()
    elseif(RCAR_TARGET_OS STREQUAL "emcos")
        add_definitions(-DTEST_EMCOS)
        add_definitions(-DSUPPORT_STATIC_ENV)
        if(RCAR_SOC STREQUAL "v4h")
            add_definitions(-DUNSUPPORT_FBC_DEVICE)
            add_definitions(-DUNSUPPORT_IPMMU)
            add_definitions(-DUNSUPPORT_FCPR)
        endif()
    endif()
elseif(RCAR_XIL STREQUAL sil)
    if(RCAR_TARGET_OS STREQUAL "windows")
        add_definitions(-DTEST_WINDOWS)
    endif()
    add_definitions(-DTEST_SIL)
    add_definitions(-DSUPPORT_NEW_AXI_BUS_NAME_RULE)
    if(RCAR_SOC STREQUAL "v3u")
        add_definitions(-DUNSUPPORT_IMP_CHILD_DEVICE)
        add_definitions(-DUNSUPPORT_IMP_CHILD_DEVICE_DMAC)
        add_definitions(-DUNSUPPORT_ICCOM)
        add_definitions(-DUNSUPPORT_IVCP1E)
        add_definitions(-DUNSUPPORT_VCP4)
        add_definitions(-DUNSUPPORT_FCPC)
        add_definitions(-DUNSUPPORT_VSPX)
        add_definitions(-DUNSUPPORT_FBC_DEVICE)
        add_definitions(-DUNSUPPORT_FBA)
        add_definitions(-DUNSUPPORT_RFSO)
        add_definitions(-DUNSUPPORT_IPMMU)
        add_definitions(-DUNSUPPORT_FCPR)
    elseif(RCAR_SOC STREQUAL "v3h1")
        add_definitions(-DUNSUPPORT_VIP)
        add_definitions(-DUNSUPPORT_ICCOM)
        add_definitions(-DUNSUPPORT_IVCP1E)
        add_definitions(-DUNSUPPORT_ISP)
        add_definitions(-DUNSUPPORT_FBC_DEVICE)
        add_definitions(-DUNSUPPORT_FBA)
        add_definitions(-DUNSUPPORT_RFSO)
        add_definitions(-DUNSUPPORT_IPMMU)
        add_definitions(-DUNSUPPORT_FCPR)
    elseif(RCAR_SOC STREQUAL "v3h2")
        add_definitions(-DUNSUPPORT_ICCOM)
        add_definitions(-DUNSUPPORT_IVCP1E)
        add_definitions(-DUNSUPPORT_FBC_DEVICE)
        add_definitions(-DUNSUPPORT_FBA)
        add_definitions(-DUNSUPPORT_RFSO)
        add_definitions(-DUNSUPPORT_IPMMU)
        add_definitions(-DUNSUPPORT_FCPR)
    elseif(RCAR_SOC STREQUAL "v3m2")
        add_definitions(-DUNSUPPORT_ICCOM)
        add_definitions(-DUNSUPPORT_IVCP1E)
        add_definitions(-DUNSUPPORT_ISP)
        add_definitions(-DUNSUPPORT_FBC_DEVICE)
        add_definitions(-DUNSUPPORT_FBA)
        add_definitions(-DUNSUPPORT_RFSO)
        add_definitions(-DUNSUPPORT_IPMMU)
    elseif(RCAR_SOC STREQUAL "v4h")
        add_definitions(-DUNSUPPORT_GFX)
        add_definitions(-DUNSUPPORT_ICCOM)
        add_definitions(-DUNSUPPORT_IVCP1E)
        add_definitions(-DUNSUPPORT_VCP4)
        add_definitions(-DUNSUPPORT_FCPC)
        add_definitions(-DUNSUPPORT_VSPX)
        add_definitions(-DUNSUPPORT_FBC_DEVICE)
        add_definitions(-DUNSUPPORT_FBA)
        add_definitions(-DUNSUPPORT_RFSO)
        add_definitions(-DUNSUPPORT_IPMMU)
        add_definitions(-DUNSUPPORT_FCPR)
        add_definitions(-DUNSUPPORT_CAIPLITE)
        add_definitions(-DUNSUPPORT_WCRC_CAIPLITE)
        add_definitions(-DUNSUPPORT_CAIPLITE_LITE)
        add_definitions(-DUNSUPPORT_VIDEO_INPUT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_VSPD)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_CSI2)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DISPLAY)
        add_definitions(-DUNSUPPORT_RTDMAC_CONTROL)
        add_definitions(-DUNSUPPORT_WWDT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DSC)
        add_definitions(-DUNSUPPORT_RTDMAC_DESCRIPTOR)
        add_definitions(-DUNSUPPORT_PAP)
        add_definitions(-DUNSUPPORT_ICUMX_CMD)
        add_definitions(-DUNSUPPORT_VDSP)
        add_definitions(-DUNSUPPORT_VIP)
    elseif(RCAR_SOC STREQUAL "v4h2")
        add_definitions(-DUNSUPPORT_VIP_WRAPPER)
        add_definitions(-DUNSUPPORT_GFX)
        add_definitions(-DUNSUPPORT_ICCOM)
        add_definitions(-DUNSUPPORT_IVCP1E)
        add_definitions(-DUNSUPPORT_VCP4)
        add_definitions(-DUNSUPPORT_FCPC)
        add_definitions(-DUNSUPPORT_VSPX)
        add_definitions(-DUNSUPPORT_FBC_DEVICE)
        add_definitions(-DUNSUPPORT_FBA)
        add_definitions(-DUNSUPPORT_RFSO)
        add_definitions(-DUNSUPPORT_IPMMU)
        add_definitions(-DUNSUPPORT_FCPR)
        add_definitions(-DUNSUPPORT_CAIPLITE)
        add_definitions(-DUNSUPPORT_WCRC_CAIPLITE)
        add_definitions(-DUNSUPPORT_CAIPLITE_LITE)
        add_definitions(-DUNSUPPORT_VIDEO_INPUT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_VSPD)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_CSI2)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DISPLAY)
        add_definitions(-DUNSUPPORT_RTDMAC_CONTROL)
        add_definitions(-DUNSUPPORT_WWDT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DSC)
        add_definitions(-DUNSUPPORT_RTDMAC_DESCRIPTOR)
        add_definitions(-DUNSUPPORT_PAP)
        add_definitions(-DUNSUPPORT_ICUMX_CMD)
        add_definitions(-DUNSUPPORT_VDSP)
        add_definitions(-DUNSUPPORT_VIP)
    elseif(RCAR_SOC STREQUAL "v4m")
        add_definitions(-DUNSUPPORT_VIP_WRAPPER)
        add_definitions(-DUNSUPPORT_GFX)
        add_definitions(-DUNSUPPORT_ICCOM)
        add_definitions(-DUNSUPPORT_IVCP1E)
        add_definitions(-DUNSUPPORT_VCP4)
        add_definitions(-DUNSUPPORT_FCPC)
        add_definitions(-DUNSUPPORT_VSPX)
        add_definitions(-DUNSUPPORT_FBC_DEVICE)
        add_definitions(-DUNSUPPORT_FBA)
        add_definitions(-DUNSUPPORT_RFSO)
        add_definitions(-DUNSUPPORT_IPMMU)
        add_definitions(-DUNSUPPORT_FCPR)
        add_definitions(-DUNSUPPORT_CAIPLITE)
        add_definitions(-DUNSUPPORT_WCRC_CAIPLITE)
        add_definitions(-DUNSUPPORT_CAIPLITE_LITE)
        add_definitions(-DUNSUPPORT_VIDEO_INPUT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_VSPD)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_CSI2)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DSI_ENCODER)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DISPLAY)
        add_definitions(-DUNSUPPORT_RTDMAC_CONTROL)
        add_definitions(-DUNSUPPORT_WWDT)
        add_definitions(-DUNSUPPORT_VIDEO_OUTPUT_DSC)
        add_definitions(-DUNSUPPORT_RTDMAC_DESCRIPTOR)
        add_definitions(-DUNSUPPORT_PAP)
        add_definitions(-DUNSUPPORT_ICUMX_CMD)
        add_definitions(-DUNSUPPORT_VDSP)
        add_definitions(-DUNSUPPORT_VIP)
        add_definitions(-DUNSUPPORT_FCPVX)
        add_definitions(-DUNSUPPORT_GICD)
        add_definitions(-DUNSUPPORT_QOS)
        add_definitions(-DUNSUPPORT_RCARBUSMONI)
    endif()
endif()

#======================================================================================================================
# Flag when running a long-running test in CI
#======================================================================================================================
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_definitions(-DENABLE_LONGTIME_TEST)
    set(test_timeout
        # Default timeout is 10m (600), change it if your test take more time (unit is second)
        #3 hour
        10800
    )
else()
    if(RCAR_TARGET_OS STREQUAL "qnx")
        set(test_timeout
            # Default timeout is 10m (600), change it if your test take more time (unit is second)
            #15 minutes
            900
        )
    endif()
endif()

#======================================================================================================================
# Automatic Document Create Flag
#======================================================================================================================
set(doxygen_enable_for_source TRUE)

set(api_doc "NO")

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()

if(HAVE_MALLINFO OR HAVE_MALLINFO2)
    target_compile_definitions(${app_name} PRIVATE HAVE_MALLINFO)
endif()

if(HAVE_MALLINFO2)
    target_compile_definitions(${app_name} PRIVATE HAVE_MALLINFO2)
endif()
if(TARGET osal_configuration)
    # W/A to include OSAL configuration without actual link to it
    if(COMMAND rcar_get_public_include_dirs)
        rcar_get_public_include_dirs(osal_configuration_include_dir osal_configuration)
    endif()
    target_include_directories(${app_name} PRIVATE
        ${osal_configuration_include_dir}
    )

    get_target_property(osal_configuration_compile_definition osal_configuration INTERFACE_COMPILE_DEFINITIONS)
    target_compile_definitions(${app_name} PRIVATE ${osal_configuration_compile_definition})
endif()

# Make all googletest target inherit public include path of emcos BSP target
if(RCAR_TARGET_OS STREQUAL "emcos")
    # Use new CMP0079 policy: Allow to use target_link_libraries for external target (not created in this CMake file)
    cmake_policy(SET CMP0079 NEW)
    foreach(googletest_target gtest;gmock;gtest_main;gmock_main)
        target_link_libraries(${googletest_target} PRIVATE
            emcos
        )
    endforeach()
endif()
