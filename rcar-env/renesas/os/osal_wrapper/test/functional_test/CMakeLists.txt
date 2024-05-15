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

if(RCAR_TARGET_OS STREQUAL "linux")
    add_subdirectory(application/linux/osal_it)
    add_subdirectory(application/linux/osal_it_debug)
    # add_subdirectory(application/linux/memory_test)
    add_subdirectory(application/linux/osal_it_sequence)
    add_subdirectory(application/linux/osal_it_memory)
    add_subdirectory(application/linux/performance)
elseif(RCAR_TARGET_OS STREQUAL "qnx")
    add_subdirectory(application/qnx/pma)
    # add_subdirectory(application/qnx/osal_it)
    # add_subdirectory(application/qnx/memory_test)
    add_subdirectory(application/qnx/performance)
    add_subdirectory(application/qnx/sequence)
    add_subdirectory(application/qnx/it_test)
    add_subdirectory(application/qnx/it_configuration)
    add_subdirectory(application/qnx/it_thread_setting)
    add_subdirectory(application/qnx/it_device_configuration)
    add_subdirectory(application/qnx/it_memory)
    add_subdirectory(application/qnx/it_interrupt)
    add_subdirectory(application/qnx/it_exec_order)
    add_subdirectory(application/qnx/it_prio_affinity)
    elseif(RCAR_TARGET_OS STREQUAL "emcos")
    add_subdirectory(application/emcos/osal_it)
    #add_subdirectory(application/emcos/memory_test)
elseif(RCAR_TARGET_OS STREQUAL "zephyr")
    add_subdirectory(application/zephyr/mq_send_receive_test)
endif()
