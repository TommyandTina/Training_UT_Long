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
#            0.0.1    10.07.2023    Newly create base on CMakeLists.txt of osal_functional_test (v3.14.0) with
#                                   following changes:
#                                       -  Added a configuration file that combines thread priority and
#                                          CPU Affinity settings.
#            0.0.2    02.11.2023    Move definition of SoC dependency to CMakeLists.txt in parent directory
#----------------------------------------------------------------------------------------------------------------------
#----------------------------------------------------------------------------------------------------------------------
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "osal_wrapper_functional_test_it_prio_affinity_api_1-3")
set(app_version "0.0.1")

set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.16.3)
project(${app_name})

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
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests//src/main.cpp

    ## ---- OSAL API ------
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests/src/test_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests/src/test_cond_var.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests/src/test_mq.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests/src/test_interrupt.cpp

    ## ---- Static Env Resource Info ----
    ${CMAKE_CURRENT_SOURCE_DIR}/r_osal_functional_test_configuration.c
)

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests/src/include/test_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests/src/include/test_osal_resource_definition.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests/src/include/test_osal_device_definition.hpp
)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/../../api_tests/src/include
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
