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

# For all supported options, refer to https://www.doxygen.nl/manual/config.html
# User need to add `DOXYGEN_` prefix for each option. e.g: set(DOXYGEN_EXTRACT_LOCAL_CLASSES NO)

set(DOXYGEN_EXTRACT_LOCAL_CLASSES NO)
set(DOXYGEN_INLINE_SOURCES NO)
set(DOXYGEN_EXAMPLE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/include/test_osal_device_definition.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/include/test_osal_resource_definition.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/include/test_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_cond_var.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_memory_barrier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_mq.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_timeclock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_interrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_io_extension_api.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_pm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_memory_api_basic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_mmngr_generic_interface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_resource_measurement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_init_deinit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_version.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_io_axi_bus_name.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/test_system_ram.cpp
)
set(DOXYGEN_EXCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/include/test_common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/r_osal_functional_test_resource_info.c
    ${CMAKE_CURRENT_SOURCE_DIR}/renesas/os/osal/test/functional_test/application/api_tests/src/r_osal_functional_test_configuration.c
)
set(DOXYGEN_PREDEFINED 
    AXI_BUS_NAME_SUPPORT
    OSAL_SOC_V4H
)
