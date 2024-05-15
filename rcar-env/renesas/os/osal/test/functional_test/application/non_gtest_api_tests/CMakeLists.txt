#######################################################################################################################
# Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
#            0.0.1    16.10.2022    Newly create base on CMakeLists.txt of osal_functional_test (v3.5.0) with following changes:
#                                       -  Add the OSAL Test Framework source files
#                                       -  Remove the "Google Test" related parts
#                                       -  Remove the "utils" related parts
#                                       -  Add flag UNSUPPORT_TEST_FW_ASSERT to disable ASSERT macro on Windows
#                                       -  Add flag "-Wa,-mbig-obj" to solve "File too big" issue when using template on Windows
#                                       -  Add flag OSAL_TEST_FRAMEWORK which specializes for using OSAL Test Framework
#            0.0.2    10.02.2023    Added V4H2 Setting
#            0.0.3    10.04.2023    Add flag TEST_WINDOWS
#----------------------------------------------------------------------------------------------------------------------
#----------------------------------------------------------------------------------------------------------------------
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "osal_functional_test_non_gtest")
set(app_version "0.0.3")

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
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/main.cpp

    ## ---- OSAL API ------
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_memory_barrier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_cond_var.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_mq.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_timeclock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_io_extension_api.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_memory_api_basic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_mmngr_generic_interface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_interrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_pm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_version.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_resource_measurement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_init_deinit.cpp
)

if(NOT RCAR_SOC STREQUAL "v3h1")
    list(APPEND source
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_system_ram.cpp
    )
endif()


# ---- Static Env Resource Info ----
if(RCAR_TARGET_OS STREQUAL "qnx")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/r_osal_functional_test_configuration.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_io_axi_bus_name.cpp
    )
endif()

if(RCAR_TARGET_OS STREQUAL "emcos")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/r_osal_functional_test_resource_info.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_io_axi_bus_name.cpp
    )
endif()


if(RCAR_XIL STREQUAL hil)
    if(RCAR_TARGET_OS STREQUAL "linux")
        list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/r_osal_functional_test_configuration.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_io_axi_bus_name.cpp
        )
    endif()
endif()

if(RCAR_TARGET_OS STREQUAL "zephyr")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/r_osal_functional_test_resource_info_zephyr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_io_axi_bus_name.cpp
    )
endif()

if(RCAR_XIL STREQUAL sil)
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/test_io_axi_bus_name.cpp
    )
endif()

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/include/test_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/include/test_osal_resource_definition.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/include/test_osal_device_definition.hpp
    ## ---- OSAL Test Framework ------
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/include/osal_test_framework.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/include/osal_test_mock_framework.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/include/osal_test_framework_helper.hpp
)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/src/include
    ${rcar-xos_INCLUDE_DIRS}
)

#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(link_lib
    osal
    osal_wrapper
)

# [optional] for 3rd Party libraries
set(link_dir)


if (CMAKE_LINKER MATCHES "armlink")
    if(RCAR_TARGET_OS STREQUAL "emcos")
        set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/../api_tests/default_cxx_proc_scatter.txt")
     endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# [Optional] Any specifics that an application may require, like OS specifics, SoC specifics
#----------------------------------------------------------------------------------------------------------------------
include(CheckSymbolExists)
check_symbol_exists(mallinfo2 "malloc.h" HAVE_MALLINFO2)
check_symbol_exists(mallinfo "malloc.h" HAVE_MALLINFO)

if(RCAR_TARGET_OS STREQUAL "qnx")
    string(REPLACE "-fpic" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REPLACE "-fpic" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

#if(TARGET imp)
#    list(APPEND source
#        ${CMAKE_CURRENT_SOURCE_DIR}/src/test_impdrv.cpp
#    )
#    list(APPEND link_lib
#        imp
#        )
#endif()

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
        set(test_arg "--test_filter=-InitDeinitApiTest.InitDeinit_TC_10_001_001:-OsalResourcePerformance*")
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
    elseif(RCAR_TARGET_OS STREQUAL "zephyr")
        add_definitions(-DTEST_ZEPHYR)
        add_definitions(-DSUPPORT_STATIC_ENV)
        add_definitions(-DSUPPORT_NEW_AXI_BUS_NAME_RULE)
        if(RCAR_SOC STREQUAL "v4h")
            add_definitions(-DUNSUPPORT_IVCP1E)
            add_definitions(-DUNSUPPORT_GFX)
            add_definitions(-DUNSUPPORT_VCP4)
            add_definitions(-DUNSUPPORT_FCPC)
            add_definitions(-DUNSUPPORT_FCPR)
            add_definitions(-DUNSUPPORT_CAIPLITE)
            add_definitions(-DUNSUPPORT_WCRC_CAIPLITE)
            add_definitions(-DUNSUPPORT_CAIPLITE_LITE)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT)
            add_definitions(-DUNSUPPORT_RTDMAC_CONTROL)
            add_definitions(-DUNSUPPORT_RTDMAC_DESCRIPTOR)
            add_definitions(-DUNSUPPORT_PAP)
            add_definitions(-DUNSUPPORT_ICUMX_CMD)
        elseif(RCAR_SOC STREQUAL "v4h2")
            add_definitions(-DUNSUPPORT_IVCP1E)
            add_definitions(-DUNSUPPORT_GFX)
            add_definitions(-DUNSUPPORT_VCP4)
            add_definitions(-DUNSUPPORT_FCPC)
            add_definitions(-DUNSUPPORT_FCPR)
            add_definitions(-DUNSUPPORT_CAIPLITE)
            add_definitions(-DUNSUPPORT_WCRC_CAIPLITE)
            add_definitions(-DUNSUPPORT_CAIPLITE_LITE)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT)
            add_definitions(-DUNSUPPORT_RTDMAC_CONTROL)
            add_definitions(-DUNSUPPORT_RTDMAC_DESCRIPTOR)
            add_definitions(-DUNSUPPORT_PAP)
            add_definitions(-DUNSUPPORT_ICUMX_CMD)
        elseif(RCAR_SOC STREQUAL "v4m")
            add_definitions(-DUNSUPPORT_IVCP1E)
            add_definitions(-DUNSUPPORT_GFX)
            add_definitions(-DUNSUPPORT_VCP4)
            add_definitions(-DUNSUPPORT_FCPC)
            add_definitions(-DUNSUPPORT_FCPR)
            add_definitions(-DUNSUPPORT_CAIPLITE)
            add_definitions(-DUNSUPPORT_WCRC_CAIPLITE)
            add_definitions(-DUNSUPPORT_CAIPLITE_LITE)
            add_definitions(-DUNSUPPORT_VIDEO_OUTPUT)
            add_definitions(-DUNSUPPORT_RTDMAC_CONTROL)
            add_definitions(-DUNSUPPORT_RTDMAC_DESCRIPTOR)
            add_definitions(-DUNSUPPORT_PAP)
            add_definitions(-DUNSUPPORT_ICUMX_CMD)
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
    if(RCAR_TARGET_OS STREQUAL "windows")
        # OSAL test FW haven't supported generating fatal failures and abort the current function
        add_definitions(-DUNSUPPORT_TEST_FW_ASSERT)
        add_definitions(-DTEST_WINDOWS)
    endif()
endif()

if(RCAR_XIL STREQUAL sil)
    if(RCAR_TARGET_OS STREQUAL "windows")
        # WA to fix "File too big" issue when using template method in minwg
        add_definitions("-Wa,-mbig-obj")
    endif()
endif()

# Flag used for selecting the OSAL test framework
add_definitions(-DOSAL_TEST_FRAMEWORK)

if(RCAR_TARGET_OS STREQUAL "zephyr")
    add_definitions(-DCONFIG_ADDR_STACK)
    add_definitions(-DCONFIG_SMALL_STACKSIZE)
    add_definitions(-DENABLE_RESOURCE_OSAL)
    set(test_end_pattern
    # Only for Zephyr OS
    # Set this variable to help Zephyr OS test knows when the application is completed
    # When the log matches this pattern, R-Car Env test script will stop and turn off the board
    # E.g:
    # set(test_end_pattern "<APP_END>")
    # set(test_end_pattern "<Finish running app>")
    "================== osal_functional_test_non_gtest finish =================="
    )
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
