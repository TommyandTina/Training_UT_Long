######################################################################################################################
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
#######################################################################################################################
# History   Version   DD.MM.YYYY   Description
#           1.0.0     06.09.2022   Initial revision
#######################################################################################################################

# Set the version of the application
set(app_version "1.0.0")
cmake_minimum_required(VERSION 3.16.3)

set (IMPDRV_DSPCRC_DETECT "yes")

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

# common macro for compiling test app
macro (imp_test_build test_step)
    #----------------------------------------------------------------------------------------------------------------------
    # Set app name
    #----------------------------------------------------------------------------------------------------------------------
    set(app_name    "imp_functional_test_${test_step}")    # Give the name a meaning full name

    project(${app_name})

    # configuration of this app
    if(RCAR_TARGET_OS STREQUAL "freertos")
        set(TEST_FLAG -DTARGET_OS_RTOS)
    elseif(RCAR_TARGET_OS STREQUAL "linux")
        set(TEST_FLAG -DTARGET_OS_LINUX)
    elseif(RCAR_TARGET_OS STREQUAL "qnx")
        set(TEST_FLAG -DTARGET_OS_QNX)
        # add_compile_options(-fno-stack-protector)
    endif()

    if(ABNORMAL STREQUAL "on")
        list(APPEND TEST_FLAG
            -DIT_ABNORMAL
        )
    endif()

    # TEST_S0S1 is one that means enabling S0S1 report for IMP and IMP RTT Driver
    if(${TEST_S0S1} EQUAL 1)
        list(APPEND TEST_FLAG
            -DIMPDRV_S0S1_ENABLE
        )
        if (${test_step} STREQUAL "step1")
            list(APPEND TEST_FLAG
                -DSTEP1
        )
        elseif (${test_step} STREQUAL "step2")
            list(APPEND TEST_FLAG
                -DSTEP2
        )
        elseif (${test_step} STREQUAL "step3")
            list(APPEND TEST_FLAG
                -DSTEP3
        )
        elseif (${test_step} STREQUAL "step4")
            list(APPEND TEST_FLAG
                -DSTEP4
        )
        endif()

        # append s0s1 string to test app
        set(app_name ${app_name}_s0s1)
    endif()

    if (${RCAR_SOC} STREQUAL "v3h2")
        list(APPEND TEST_FLAG
            -DV3H2
        )
    elseif (${RCAR_SOC} STREQUAL "v3h1")
        list(APPEND TEST_FLAG
            -DV3H1
        )
    elseif (${RCAR_SOC} STREQUAL "v3m2")
        list(APPEND TEST_FLAG
            -DV3M2
        )
    elseif (${RCAR_SOC} STREQUAL "v3u")
        list(APPEND TEST_FLAG
            -DV3U
        )
    elseif (${RCAR_SOC} STREQUAL "v4h")
        list(APPEND TEST_FLAG
            -DV4H
        )
    elseif (${RCAR_SOC} STREQUAL "v4h2")
        list(APPEND TEST_FLAG
            -DV4H2
        )
    endif()

        #----------------------------------------------------------------------------------------------------------------------
    # Link libraries and Link directories used by the application
    #----------------------------------------------------------------------------------------------------------------------
    set(link_lib
        # This sample does not need any other sw compoents
        atmlib
    )

        #----------------------------------------------------------------------------------------------------------------------
    # Include directories used by the application
    #----------------------------------------------------------------------------------------------------------------------
    set(include_dir
        # add any additional 3rd party include directories your application requires
        # Note: Include directories from xOS Software components are automatically added when you list them in link_libs
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/${RCAR_SOC}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/${test_step}
        ${CMAKE_SOURCE_DIR}/renesas/os/osal/include
        ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/include
        ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/include
        ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/${RCAR_SOC}/include
        ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/include/rcar-xos/imp
    )

    #----------------------------------------------------------------------------------------------------------------------
    # Source files
    #----------------------------------------------------------------------------------------------------------------------
    file(GLOB IMP_TEST_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/${RCAR_SOC}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/${test_step}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    )

    if (${RCAR_SOC} STREQUAL "v3h2")
        set(IMP_TARGET_SRC
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_cmnctl_reg.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_impctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_impsctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_cnnctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_dmactl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_dmasctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_ocvctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_pscctl.c
        )

        if (NOT ${test_step} STREQUAL "step3")
            list(APPEND IMP_TARGET_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h2/r_impdrv_cmnctl.c
            )
        endif()

        list(APPEND include_dir
            ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/atmlib/include/rcar-xos/atmlib/hwdep/v3h2/rcar-xos/atmlib
        )
    elseif (${RCAR_SOC} STREQUAL "v3h1")
        set(IMP_TARGET_SRC
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h1/r_impdrv_cmnctl_reg.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h1/r_impdrv_impctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h1/r_impdrv_impsctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h1/r_impdrv_cnnctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h1/r_impdrv_dmactl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h1/r_impdrv_ocvctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h1/r_impdrv_pscctl.c
        )

        if (NOT ${test_step} STREQUAL "step3")
            list(APPEND IMP_TARGET_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3h1/r_impdrv_cmnctl.c
            )
        endif()

        list(APPEND include_dir
            ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/atmlib/include/rcar-xos/atmlib/hwdep/v3h1/rcar-xos/atmlib
        )
    elseif (${RCAR_SOC} STREQUAL "v3m2")
        set(IMP_TARGET_SRC
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3m2/r_impdrv_cmnctl_reg.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3m2/r_impdrv_impctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3m2/r_impdrv_cnnctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3m2/r_impdrv_dmactl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3m2/r_impdrv_ocvctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3m2/r_impdrv_pscctl.c
        )

        if (NOT ${test_step} STREQUAL "step3")
            list(APPEND IMP_TARGET_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3m2/r_impdrv_cmnctl.c
            )
        endif()

        list(APPEND include_dir
            ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/atmlib/include/rcar-xos/atmlib/hwdep/v3m2/rcar-xos/atmlib
        )
    elseif (${RCAR_SOC} STREQUAL "v3u")
        set(IMP_TARGET_SRC
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3u/r_impdrv_cmnctl_reg.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3u/r_impdrv_impctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3u/r_impdrv_cnnctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3u/r_impdrv_dmactl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3u/r_impdrv_ocvctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3u/r_impdrv_pscctl.c
        )

        if (NOT ${test_step} STREQUAL "step3")
            list(APPEND IMP_TARGET_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v3u/r_impdrv_cmnctl.c
            )
        endif()

        list(APPEND include_dir
            ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/atmlib/include/rcar-xos/atmlib/hwdep/v3u/rcar-xos/atmlib
        )
    elseif (${RCAR_SOC} STREQUAL "v4h")
        set(IMP_TARGET_SRC
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_cmnctl_reg.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_impctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_cnnctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_dmactl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_dmasctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_ocvctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_pscctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_dspctl.c
        )

        if (NOT ${test_step} STREQUAL "step3")
            list(APPEND IMP_TARGET_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h/r_impdrv_cmnctl.c
            )
        endif()

        list(APPEND include_dir
            ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/atmlib/include/rcar-xos/atmlib/hwdep/v4h/rcar-xos/atmlib
        )
    elseif (${RCAR_SOC} STREQUAL "v4h2")
        set(IMP_TARGET_SRC
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_cmnctl_reg.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_impctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_cnnctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_dmactl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_dmasctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_ocvctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_pscctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_dspctl.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_udefctl.c
        )

        if (NOT ${test_step} STREQUAL "step3")
            list(APPEND IMP_TARGET_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/v4h2/r_impdrv_cmnctl.c
            )
        endif()

        list(APPEND include_dir
            ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/atmlib/include/rcar-xos/atmlib/hwdep/v4h2/rcar-xos/atmlib
        )
    endif()

    if (${test_step} STREQUAL "step1")
        # Target test: IMPDRV APIs + General Control
        # Stub: Common Control + *** Control
        list(APPEND TEST_FLAG
            -DSTEP1
        )
        if(RCAR_XIL STREQUAL sil)
            file(GLOB IMP_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_api.c
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_genctl_sil.c
            )
        else()
            file(GLOB IMP_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_api.c
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_genctl.c
            )
        endif()
        set(IMP_TARGET_SRC)
    elseif (${test_step} STREQUAL "step2")
        # Target test: IMPDRV APIs + General Control + Common Control + *** Control
        # Stub: OS dep Layer (OSAL wrapper)
        list(APPEND TEST_FLAG
            -DSTEP2
        )
        if(RCAR_XIL STREQUAL sil)
            file(GLOB IMP_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_api.c
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_genctl_sil.c
            )
        else()
            file(GLOB IMP_SRC
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_api.c
                ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_genctl.c
            )
        endif()
    elseif (${test_step} STREQUAL "step3")
        # Disable crc check in fault injection test
        set (IMPDRV_DSPCRC_DETECT "no")
        # Target test: IMPDRV APIs + General Control + Common Control + *** Control + OS dep Layer (OSAL wrapper)
        # Stub: OSAL + Broken Work Memory Detection
        list(APPEND TEST_FLAG
            -DSTEP3
        )
        file(GLOB IMP_SRC
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_api.c
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/r_impdrv_genctl.c
        )
        # include dir for r_impdrv_osdep.c
        list(APPEND include_dir
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src
            ${CMAKE_SOURCE_DIR}/renesas/driver/soc/platform/imp/src/target/${RCAR_SOC}
        )
    else()
        # Target test: All Components
        list(APPEND TEST_FLAG
            -DSTEP4
        )
        set(IMP_SRC)
        set(IMP_TARGET_SRC)
        # use real IMP lib
        list(APPEND link_lib
            imp
        )
    endif()

    if("${IMPDRV_DSPCRC_DETECT}" STREQUAL "yes")           # use user difined crc
        add_definitions(-DIMPDRV_DSPCRC_DETECT_ADDDEF)
    endif()

    set (source
        ${IMP_TEST_SRC}
        ${IMP_SRC}
        ${IMP_TARGET_SRC}
    )

    #----------------------------------------------------------------------------------------------------------------------
    # Header files of the application
    #----------------------------------------------------------------------------------------------------------------------
    set(header
        # add more header files here
    )

    # optional for 3rd Party libraries
    set(link_dir
        # Additional Link directories for 3rd party libraries
    )

    #----------------------------------------------------------------------------------------------------------------------
    # [optional] Any specifics that an application may require, like OS specifics, SoC specifics
    #----------------------------------------------------------------------------------------------------------------------

    # Example for enabling OSAL for certain environments
    if(RCAR_TARGET_OS STREQUAL "baremetal" )
        list(APPEND link_lib
            startup         # In baremetal environments the application requires at least startup code defined for xOS
        )
    else() # Any other Target OS
        list(APPEND link_lib
            osal            # This application makes use of the OSAL API
            osal_wrapper    # The application also needs the implementation of the OSAL API to link.
        )
        if(RCAR_XIL STREQUAL "hil")
            list(APPEND link_lib
                osal_configuration
            )
        endif()
    endif()

    # Linker dependencies for baremetal/freertos environments
    if (CMAKE_LINKER MATCHES "armlink")
        set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/application/lscript.scat")
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
        "(Segmentation fault|( )*,( )*[123456789]( )*error( )*)"
    )

    set(test_timeout
        # Default timeout is 10m, change it if your test take more time (unit is second)
        # 45 minutes
        2700
    )

    set(test_pretest_cmd
        # Custom command before running application on board. E.g: modprobe cmemdrv bsize=0x01000000,0x00200000 cached=0
    )

    #======================================================================================================================
    # DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
    # Adding SoC specific suffix to the application, comment next line if you don't care
    set(app_name ${app_name}_${RCAR_SOC})

    # Add executable
    add_executable(${app_name})

    # Set sources and headers
    target_sources(${app_name} PRIVATE
        "$<BUILD_INTERFACE:${source};${header}>"
    )

    # Set applications include directories
    target_include_directories(${app_name} PRIVATE
        ${include_dir} # 3rd Party include directories
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include # Default include directory for Renesas Applications
    )

    # Set applications link library directories
    target_link_directories(${app_name} PRIVATE
        ${link_dir}
    )

    # Set applications link libraries
    target_link_libraries(${app_name}
        PRIVATE ${link_lib}
    )

    # Set applications compilation flags
    target_compile_options(${app_name} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${RCAR_QM_C_FLAGS}>
        $<$<COMPILE_LANGUAGE:CXX>:${RCAR_QM_CXX_FLAGS}>
        ${TEST_FLAG}
    )

    # Linker dependencies for baremetal/freertos environments
    if (CMAKE_LINKER MATCHES "armlink")
        if(NOT linker_script)
            message(FATAL_ERROR "${app_name} is required to be linked with armlink which requires a linker script. "
                "Please set \"linker_script\" variable.")
        endif()
        rcar_set_linker_script(${app_name} ${linker_script})
    endif()

    # Configure for testing on device
    set_target_properties(${app_name} PROPERTIES
        TEST_ARG                    "${test_arg}"
        TEST_FAIL_REGEX             "${test_fail_regex}"
        TEST_TIMEOUT                "${test_timeout}"
        TEST_PRETEST_COMMAND        "${test_pretest_cmd}"
        TEST_OUTPUT                 "${test_output}"
        LINKER_LANGUAGE             C
    )
endmacro()

set(TEST_S0S1    0)
set(ABNORMAL    "on")

if(RCAR_XIL STREQUAL hil)
    imp_test_build(step1)
    imp_test_build(step2)
    imp_test_build(step3)
endif()
imp_test_build(step4)
