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
#            3.0.0    06.06.2023    Newly created
#----------------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------------------------------------
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "osal_functional_test_performance_io")
set(app_version "3.0.0")

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_perf_io.c
)

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
    # add more header files here
    ${CMAKE_CURRENT_SOURCE_DIR}/include/io_perf_header.h
)

if((RCAR_SOC STREQUAL "s4") OR (RCAR_SOC STREQUAL "s4n"))
    list(APPEND header
        ${CMAKE_CURRENT_SOURCE_DIR}/include/io_perf_device_definition_s4.h
    )
elseif((RCAR_SOC STREQUAL "v4h") OR (RCAR_SOC STREQUAL "v4h2"))
    list(APPEND header
        ${CMAKE_CURRENT_SOURCE_DIR}/include/io_perf_device_definition_v4h.h
    )
elseif((RCAR_SOC STREQUAL "v3h2") OR (RCAR_SOC STREQUAL "v3h1"))
    list(APPEND header
        ${CMAKE_CURRENT_SOURCE_DIR}/include/io_perf_device_definition_v3h.h
    )
elseif(RCAR_SOC STREQUAL "v3m2")
    list(APPEND header
        ${CMAKE_CURRENT_SOURCE_DIR}/include/io_perf_device_definition_v3m.h
    )
endif()

add_definitions(-DTEST_PERF_IO)
include(${CMAKE_CURRENT_SOURCE_DIR}/../performance_common.cmake)
