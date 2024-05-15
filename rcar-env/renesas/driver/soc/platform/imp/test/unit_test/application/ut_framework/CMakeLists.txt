#######################################################################################################################
# Copyright [2020-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "imp_unit_test_on_target")
set(app_version "0.1.0")

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
find_package(Python3 REQUIRED COMPONENTS Interpreter)
execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/replace_source.py)

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
#----------------------------------------------------------------------------------------------------------------------
# Common Source/Header configuration
#----------------------------------------------------------------------------------------------------------------------
# UT framework sources
file(GLOB ut_framework_source
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/framework/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ut/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/util/*.c
    )

# Driver sources
file(GLOB driver_source
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_api.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr.c

    )

# UT stub sources
file(GLOB stub_source
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/impdrv_util.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/ut_hook.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/ut_osal_stub.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_genctl_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_cmnctl_reg_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_impctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_ocvctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_cmnctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_dmactl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_cnnctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_pscctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_osdep_stub_ut.c
    )
if (RCAR_SOC STREQUAL "v3h1")
    list (APPEND stub_source
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_impsctl_stub_ut.c
    )
elseif (RCAR_SOC STREQUAL "v3h2")
    list (APPEND stub_source
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_impsctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_dmasctl_stub_ut.c
    )
elseif (RCAR_SOC STREQUAL "v4h")
    list (APPEND stub_source
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_dmasctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_dspctl_stub_ut.c
    )
elseif (RCAR_SOC STREQUAL "v4h2")
    list (APPEND stub_source
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_dmasctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_dspctl_stub_ut.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/r_impdrv_udefctl_stub_ut.c
    )
endif()

# PCL sources
file(GLOB pcl_source
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_api/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_genctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_osdep/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cmnctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cmnctl_reg/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_impctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_ocvctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dmactl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_pscctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_udefctl/*.c

    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_api/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_genctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_osdep/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cmnctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cmnctl_reg/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_impctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_ocvctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dmactl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_pscctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_udefctl/*.c
)

if (RCAR_SOC STREQUAL "v3m2")
    list(REMOVE_ITEM pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_force_reset_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_force_reset.c

            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_extend_config_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_extend_config.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cl_brk_addr_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cl_brk_addr.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cond_gosub_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cond_gosub.c
    )
    file(GLOB private_pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cmnctl_reg/v3x/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dmactl/v3x/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_impctl/v3x/*.c

            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cmnctl_reg/v3x/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dmactl/v3x/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_impctl/v3x/*.c
        )
elseif (RCAR_SOC STREQUAL "v3h1")
    list(REMOVE_ITEM pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_force_reset_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_force_reset.c

            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_extend_config_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_extend_config.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cl_brk_addr_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cl_brk_addr.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cond_gosub_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cond_gosub.c
        )
    file(GLOB private_pcl_source

        )
elseif (RCAR_SOC STREQUAL "v3h2")
    list(REMOVE_ITEM pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_force_reset_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_force_reset.c

            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_extend_config_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_extend_config.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cl_brk_addr_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cl_brk_addr.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cond_gosub_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_set_cond_gosub.c
        )
    file(GLOB private_pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_impsctl/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dmasctl/*.c

            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_impsctl/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dmasctl/*.c
        )
elseif (RCAR_SOC STREQUAL "v4h2")
    list(REMOVE_ITEM pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cnnctl/impdrv_cnnctl_force_reset_v4h1_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cnnctl/impdrv_cnnctl_force_reset_v4h1.c

            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_cmnctl_reg/impdrv_cmnctl_reg_get_handle_v4h1_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_cmnctl_reg/impdrv_cmnctl_reg_get_handle_v4h1.c

            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dmactl/impdrv_dmactl_int_safety_func_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dmactl/impdrv_dmactl_int_safety_func.c
        )
        file(GLOB private_pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dmasctl/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dspctl/*.c

            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dmasctl/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dspctl/*.c
            )
elseif (RCAR_SOC STREQUAL "v4h")
    list(REMOVE_ITEM pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dmactl/impdrv_dmactl_int_safety_func_table.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dmactl/impdrv_dmactl_int_safety_func.c
    )
    file(GLOB private_pcl_source
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dmasctl/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/r_impdrv_dspctl/*.c

            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dmasctl/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/r_impdrv_dspctl/*.c
        )
endif()

# Source files
set(source
    ${ut_framework_source}
    ${driver_source}
    ${stub_source}
    ${pcl_source}
    ${private_pcl_source}
)

# filter out test source of some HW specific
if (RCAR_SOC MATCHES "v4h")
    list(FILTER source EXCLUDE REGEX "impdrv_genctl_set_dsp_v3x")
else()
    list(FILTER source EXCLUDE REGEX "impdrv_genctl_set_dsp_v4x")
endif()

set(include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp_stub/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/base/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/base/imp/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/ut/imp/include/
    ${CMAKE_SOURCE_DIR}/renesas/os/osal/include/
    ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/include/
    ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/include/
    ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/${RCAR_SOC}/include/
    ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/${RCAR_SOC}/
    ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/
)


# Public Header that are exported and can be used by application or other modules
set(public_header
)

# Private Header files that are not exported and cannot be used by other components or modules
set(private_header
    #TODO: This is copied from the OSAL Wrapper implementation, find a common place this header to avoid copying
)

# [Optional] Compile definition
set(compile_def
)

set(private_compile_def
)

# [Optional]
set(private_link_lib
)

# [Optional] for 3rd Party libraries
set(link_dir
)

# [Optional] for 3rd Party libraries
set(private_link_dir
)

#----------------------------------------------------------------------------------------------------------------------
# Test settings
#----------------------------------------------------------------------------------------------------------------------
# message(WARNING
# "How do you plan to apply test settings?
#  Is this to be done by the user oder part of the section below?
#  Example: Special build of this component for Unit Testing that requires all static symbols to be visilible.
#  What is the mechanism for this?
#  If no user interaction is required, remove \"Test settings\" section from this file")

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
# Adding SoC specific suffix to the application, comment next line if you don't care

# Linker dependencies for baremetal/freertos environments
if (CMAKE_LINKER MATCHES "armlink")
    if(NOT linker_script)
        message(FATAL_ERROR "${app_name} is required to be linked with armlink which requires a linker script. "
            "Please set \"linker_script\" variable.")
    endif()
    rcar_set_linker_script(${linker_script})
endif()

#----------------------------------------------------------------------------------------------------------------------
# Test settings, e.g. which and how to run the tests for this application
#----------------------------------------------------------------------------------------------------------------------
set(test_args
    # Set this variable of your test requires some command line arguments to be given.
)

set(test_fail_regex
    # Set this variable if this test has other fail log than the default
    # Default fail regex is: [^RESULT]<FAIL>)|ERROR|error:|Segmentation fault|result NG
    "(Segmentation fault|( )*NG( )*=( )*[123456789])"
)

set(test_timeout
    # Default timeout is 10m, change it if your test take more time (unit is second)
)

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()

# Set applications compilation flags
if (RCAR_TARGET_OS STREQUAL "linux")
    set(FLAG "LINUX_OS")
    add_definitions(-DFULL_LOG)
elseif(RCAR_TARGET_OS STREQUAL "qnx")
    set(FLAG "QNX_OS")
    add_definitions(-DFULL_LOG)
elseif(RCAR_TARGET_OS STREQUAL "windows")
    set(FLAG "WINDOWS_OS")
elseif(RCAR_TARGET_OS STREQUAL "freeRTOS")
    set(FLAG "FREERTOS")
endif()

string(TOUPPER ${RCAR_SOC} TOUPPER_RCAR_SOC)

target_compile_options(${app_name} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${RCAR_DEBUG_C_FLAGS}>
    -DTEST_ENV_UT -D${FLAG}
    -DIMPDRV_STATIC
    -DUPDATE_IMP_TOP
    -DIMPDRV_BUILD_UNIT_TEST
    -DRCAR_${TOUPPER_RCAR_SOC}
    -DTEST_SOC="${TOUPPER_RCAR_SOC}"
    -DIMPDRV_DSPCRC_DETECT_ADDDEF
)
