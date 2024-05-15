#######################################################################################################################
# Copyright [2021-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
set(app_name "osal_wrapper_functional_test_it_sequence")
set(app_version "1.0.0")

#----------------------------------------------------------------------------------------------------------------------
# Include each SoC definition
#----------------------------------------------------------------------------------------------------------------------
include(${CMAKE_CURRENT_SOURCE_DIR}/../soc_definition.cmake)

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------
# Change the value at run time:
#   0 : no stub.
#   1 : Use stubs to check resources.
#   2 : Use stubs to check sequence.
set(value 2)

if(value EQUAL 1)
    add_definitions(-DUSE_STUB)
    add_definitions(-DRESOURCE_CHECK)
    string(REPLACE "-lc" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    message(STATUS "*** use stub (check resource) ***")

elseif(value EQUAL 2)
    add_definitions(-DUSE_STUB)
    add_definitions(-DSEQUENCE_CHECK)
    string(REPLACE "-lc" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    message(STATUS "*** use stub (check sequence) ***")

else()   # no stub
    message(STATUS "*** no stub ***")
endif()

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
if(value) # use stub
    set(source
        # configuration
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/src/target/qnx/r_osal_configuration.c

        # memory allocator library
        ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/memory_allocator/src/r_meml_allocator.c

        # pma
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/r_pma_if.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/manager/r_pma_manager.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/statemachine/r_pma_statemachine.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/r_pma_hwdepend.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3h1.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3h2.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3m.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v4h.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v4m.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/hw/tbl/r_pma_hw_tbl_s4.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/qnx/r_pma_bsp_wrapper.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v3h1.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v3h2.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v3m.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v4h.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v4m.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_s4.c

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
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_common.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_cond.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_interrupt.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_io.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_memory.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_memorybarrier.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_mutex.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_thread.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_timeclock.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/qnx/r_osal_os_message.c


        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub.c
    )
else() # no stub
    set(source
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
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/
)

if(value) # use stub
    list(APPEND include_dir
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_include
    )
endif()
#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
if(value) # use stub
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
