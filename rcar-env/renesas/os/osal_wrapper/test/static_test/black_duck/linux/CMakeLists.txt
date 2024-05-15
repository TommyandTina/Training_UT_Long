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
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "osal_wrapper_static_test_black_duck")

#######################################################################################################################
# CTest configuration

# Disable in-source build
rcar_check_for_insource_build()

# CTest configuration
add_test(NAME load_${app_name}
    COMMAND python ${CMAKE_SOURCE_DIR}/.ci/static_test_black_duck/static_test_black_duck.py
    -d $ENV{BD_TMP_DIR}
    -p ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/src/target/linux
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/include/rcar-xos/osal_configuration/target/linux
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/include/rcar-xos/osal_configuration/target/common
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/common/dev_cfg
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/common/dev_cfg/dynamic
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/common/helper
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/common/os_if
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/common/res_cfg
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/common/res_cfg/dynamic
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/common/res_cfg
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/common
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/linux
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/linux
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/dev_cfg/dynamic
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/helper
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg
       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/common/res_cfg/dynamic
    -m $ENV{BD_TARGET_MODULE_SHORT_NAME}
    -t $ENV{BD_TARGET_PROJECT}
    -s $ENV{BD_TARGET_PKG}
)
#######################################################################################################################
