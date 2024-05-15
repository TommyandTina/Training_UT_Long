#######################################################################################################################
# Copyright [2020-2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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

# Set app name
set(app_name    "bsp_static_test_coverity_qnx")
set(app_version "0.1.0")


# Disable in-source build
rcar_check_for_insource_build()

set(qnx_bsp_build_dir ${CMAKE_BINARY_DIR}/../renesas/os/bsp/qnx/gateway/src)

# CTest configuration
add_test(NAME load_${app_name}
    COMMAND python ${CMAKE_SOURCE_DIR}/.ci/static_test_coverity/static_test_coverity.py -c ${CMAKE_C_COMPILER}
        -t build_bsp -b ${CMAKE_BINARY_DIR} -u ${CI_COVERITY_USERID} -p ${CI_COVERITY_PASSWORD}
        -s R-Car_Environment_BSP-${RCAR_SOC}-${RCAR_TARGET_OS}-${RCAR_COMPILER_ID}-${CMAKE_SYSTEM_PROCESSOR}
)

set_tests_properties(load_${app_name} PROPERTIES TIMEOUT 4500)
