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

message(STATUS "OS")

rcar_add_subdirectory(bsp)
rcar_add_subdirectory(osal)

if(RCAR_TARGET_OS STREQUAL "freertos")
    #add_subdirectory(osal_wrapper_freertos)
else()
    if(RCAR_TARGET_OS STREQUAL "qnx")
        rcar_add_subdirectory(osal_configuration)
    elseif (RCAR_TARGET_OS STREQUAL "emcos")
        rcar_add_subdirectory(osal_configuration)
    elseif (RCAR_TARGET_OS STREQUAL "zephyr")
        rcar_add_subdirectory(osal_configuration)
    elseif (RCAR_TARGET_OS STREQUAL "linux")
        if(RCAR_XIL STREQUAL hil)
            rcar_add_subdirectory(osal_configuration)
        endif()
    endif()

    rcar_add_subdirectory(osal_wrapper)

endif()
