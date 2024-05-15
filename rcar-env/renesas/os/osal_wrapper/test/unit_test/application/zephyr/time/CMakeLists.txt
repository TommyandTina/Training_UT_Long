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
#find_package(rcar-xos REQUIRED)

#----------------------------------------------------------------------------------------------------------------------
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name "osal_wrapper_unit_test_time")
set(app_version "0.1.0")

cmake_minimum_required(VERSION 3.16.3)
project(${app_name})

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------
# Same definition as when soc_dependent=TRUE
if(RCAR_SOC STREQUAL "v4h")
    add_definitions(-DRCAR_V4H)
elseif(RCAR_SOC STREQUAL "v4h2")
    add_definitions(-DRCAR_V4H2)
endif()

set(stub true) # Make this line true when using stubs.

if(stub) # use stub
    message(STATUS "*** use stub ***")
    add_definitions(-DUSE_STUB)
    add_definitions(-DOSAL_ZEPHYR_UT)
    add_definitions(-DOSAL_ZEPHYR_UT_PERFORMANCE)
    add_definitions(-DCONFIG_ADDR_STACK)
    add_definitions(-DENABLE_RESOURCE_OSAL)
else()   # no stub
    message(STATUS "*** no stub ***")
endif()

set(test_end_pattern
    # Only for Zephyr OS
    # Set this variable to help Zephyr OS test knows when the application is completed
    # When the log matches this pattern, R-Car Env test script will stop and turn off the board
    # E.g:
    # set(test_end_pattern "<APP_END>")
    # set(test_end_pattern "<Finish running app>")
    "UT performance test finish"
)

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
if(stub) # use stub
    set(source
        # configuration
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/src/target/zephyr/r_osal_configuration.c

        # memory allocator library
        ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/memory_allocator/src/r_meml_allocator.c

        # pma
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/r_pma_if.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/manager/r_pma_manager.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/statemachine/r_pma_statemachine.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/r_pma_hwdepend.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/zephyr/r_pma_hw_tbl_v4h.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3h1.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3h2.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3m.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_s4.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v4m.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/zephyr/r_pma_bsp_wrapper.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/zephyr/tbl/r_pma_bsp_wrapper_tbl_v4h.c

        # OSAL Manager
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_common.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_interrupt.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_io.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_message.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_power.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_thread.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_threadsync.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_memory.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_timeclock.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/r_osal_version.c

        # OSAL Device Configurator
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/dev_cfg/static/r_osal_dev_cfg_io.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/dev_cfg/static/r_osal_dev_cfg_power.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/dev_cfg/static/r_osal_dev_cfg_interrupt.c

        # OSAL Resource Configurator
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/r_osal_res_cfg_memory.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/static/r_osal_res_cfg_message.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/static/r_osal_res_cfg_thread.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/static/r_osal_res_cfg_threadsync.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/static/r_osal_res_cfg_timeclock.c

        # OSAL Helper IF
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_cond.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_mutex.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_thread.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_timeclock.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_message.c

        # OSAL OS IF
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_common.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_cond.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_interrupt.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_io.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_memory.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_memorybarrier.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_mutex.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_thread.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_timeclock.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/zephyr/r_osal_os_message.c

        # Unit Performance Test App
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub.c
    )
else() # no stub
    set(source
        # Unit Performance Test App
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/main.c
    )
endif()

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    ${CMAKE_SOURCE_DIR}/renesas/os/osal/include
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/include
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include
    ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/memory_allocator/include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyrproject/zephyr/include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyrproject/zephyr/include/zephyr
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyrproject/zephyr/include/zephyr/posix
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyrproject/zephyr/include/generated
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyrproject/zephyr/soc/arm/renesas_rcar/gen4
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyrproject/zephyr/lib/libc/newlib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyrproject/modules/hal/cmsis/CMSIS/Core_R/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyrproject/modules/hal/atmel/include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyr-sdk-0.14.2/arm-zephyr-eabi/lib/gcc/arm-zephyr-eabi/10.3.0/include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/zephyr-sdk-0.14.2/arm-zephyr-eabi/lib/gcc/arm-zephyr-eabi/10.3.0/include-fixed
)

#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
if(stub) # use stub
    set(link_lib)
else() # no stub
    set(link_lib
        osal
        osal_wrapper
        osal_configuration
    )
endif()

# [Optional] for 3rd Party libraries
set(link_dir)

#----------------------------------------------------------------------------------------------------------------------
# Set document file list
# If not set, it will search for document in ${CMAKE_CURRENT_SOURCE_DIR}/doc by default
#----------------------------------------------------------------------------------------------------------------------
set(document)

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()
