#######################################################################################################################
# Copyright [2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
if(no_stub)
set(module_name "osal_wrapper_unit_test_time_no_stub")
else()
set(module_name "osal_wrapper_unit_test_time")
endif()
set(app_name ${module_name})
set(app_version "1.1.0")
project(${app_name})

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
set(source
    # configuration
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/src/target/linux/r_osal_configuration.c

    # memory allocator library
    ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/memory_allocator/src/r_meml_allocator.c

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
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/dev_cfg/dynamic/r_osal_dev_cfg_io.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/dev_cfg/dynamic/r_osal_dev_cfg_power.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/dev_cfg/dynamic/r_osal_dev_cfg_interrupt.c

    # OSAL Resource Configurator
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/r_osal_res_cfg_memory.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/dynamic/r_osal_res_cfg_message.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/dynamic/r_osal_res_cfg_thread.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/dynamic/r_osal_res_cfg_threadsync.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/dynamic/r_osal_res_cfg_timeclock.c

    # OSAL Helper IF
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_cond.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_message.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_mutex.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_thread.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper/r_osal_hif_timeclock.c

    # OSAL OS IF
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_common.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_cond.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_interrupt.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_io.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_memory.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_memorybarrier.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_message.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_mutex.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_power.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_thread.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux/r_osal_os_timeclock.c

    # Unit Performance Test App
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/main.c
)

if(NOT DEFINED no_stub)
list(APPEND source
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub.c
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
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/
)

#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(link_lib
 osal_configuration)

# [Optional] for 3rd Party libraries
set(link_dir)

#----------------------------------------------------------------------------------------------------------------------
# Set document file list
# If not set, it will search for document in ${CMAKE_CURRENT_SOURCE_DIR}/doc by default
#----------------------------------------------------------------------------------------------------------------------
set(document)

# for linux
add_definitions(-D_POSIX_C_SOURCE=200809L)
if(no_stub)
    list(APPEND link_lib
        rt
        pthread
    )
else()
    add_definitions(-D_USE_STUB)
endif()

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()
