#######################################################################################################################
# Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
#            3.0.0    18.04.2023    Newly create
#----------------------------------------------------------------------------------------------------------------------
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
list(APPEND source
    ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/src/common_func.c
)

if(RCAR_TARGET_OS STREQUAL "zephyr")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/src/time_measurement_zephyr.c
    )
elseif(RCAR_TARGET_OS STREQUAL "linux")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/src/time_measurement_linux.c
    )
elseif(RCAR_TARGET_OS STREQUAL "qnx")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/src/time_measurement_qnx.c
    )
elseif(RCAR_TARGET_OS STREQUAL "windows")
    list(APPEND source
        ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/src/time_measurement_windows.c
    )
endif()

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
list(APPEND header
    ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/include/common_header.h
)

if((RCAR_SOC STREQUAL "s4") OR (RCAR_SOC STREQUAL "s4n"))
    add_definitions(-DOSAL_PERF_TEST_S4)
    list(APPEND header
        ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/include/expected_perf_s4.h
    )
elseif((RCAR_SOC STREQUAL "v4h") OR (RCAR_SOC STREQUAL "v4h2"))
    add_definitions(-DOSAL_PERF_TEST_V4H)
    list(APPEND header
        ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/include/expected_perf_v4h.h
    )
elseif((RCAR_SOC STREQUAL "v3h2") OR (RCAR_SOC STREQUAL "v3h1"))
    add_definitions(-DOSAL_PERF_TEST_V3H)
    list(APPEND header
        ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/include/expected_perf_v3h.h
    )
elseif(RCAR_SOC STREQUAL "v3m2")
    add_definitions(-DOSAL_PERF_TEST_V3M2)
    list(APPEND header
        ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/include/expected_perf_v3m.h
    )
endif()

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/../based_source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${rcar-xos_INCLUDE_DIRS}
)

#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(link_lib
    osal
    osal_wrapper
)

if(RCAR_XIL STREQUAL "hil")
    list(APPEND link_lib
        osal_configuration
    )
endif()

# [optional] for 3rd Party libraries
set(link_dir)

#----------------------------------------------------------------------------------------------------------------------
# [Optional] Any specifics that an application may require, like OS specifics, SoC specifics
#----------------------------------------------------------------------------------------------------------------------
if(RCAR_XIL STREQUAL hil)
    if(RCAR_TARGET_OS STREQUAL "zephyr")
        add_definitions(-DTEST_PERF_ZEPHYR)
    elseif(RCAR_TARGET_OS STREQUAL "linux")
        add_definitions(-DTEST_PERF_LINUX_HIL)
        add_definitions(-D_POSIX_C_SOURCE=200809L)
    elseif(RCAR_TARGET_OS STREQUAL "qnx")
        add_definitions(-DTEST_PERF_QNX)
    endif()
elseif(RCAR_XIL STREQUAL sil)
    if(RCAR_TARGET_OS STREQUAL "linux")
        add_definitions(-DTEST_PERF_LINUX_SIL)
        add_definitions(-D_POSIX_C_SOURCE=200809L)
    elseif(RCAR_TARGET_OS STREQUAL "windows")
        add_definitions(-DTEST_PERF_WINDOWS)
    endif()
endif()

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
    "================== osal_functional_test_performance finish =================="
    )
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