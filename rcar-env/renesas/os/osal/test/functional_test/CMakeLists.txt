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

message(STATUS "OS")

if(NOT RCAR_TARGET_OS STREQUAL "zephyr")
    add_subdirectory(application/api_tests)
    add_subdirectory(application/non_gtest_api_tests)
else()
    add_subdirectory(application/non_gtest_api_tests_zephyr/cond)
    add_subdirectory(application/non_gtest_api_tests_zephyr/initdeinit)
    add_subdirectory(application/non_gtest_api_tests_zephyr/interrupt)
    add_subdirectory(application/non_gtest_api_tests_zephyr/io1)
    add_subdirectory(application/non_gtest_api_tests_zephyr/io2)
    add_subdirectory(application/non_gtest_api_tests_zephyr/io_axibus)
    add_subdirectory(application/non_gtest_api_tests_zephyr/io_extension)
    add_subdirectory(application/non_gtest_api_tests_zephyr/memory_api_buf1)
    add_subdirectory(application/non_gtest_api_tests_zephyr/memory_api_buf2)
    add_subdirectory(application/non_gtest_api_tests_zephyr/memory_api_buf3)
    add_subdirectory(application/non_gtest_api_tests_zephyr/memory_api_debug)
    add_subdirectory(application/non_gtest_api_tests_zephyr/memory_api_mm1)
    add_subdirectory(application/non_gtest_api_tests_zephyr/memory_api_mm2)
    add_subdirectory(application/non_gtest_api_tests_zephyr/memorybarrier)
    add_subdirectory(application/non_gtest_api_tests_zephyr/mmngr_generic)
    add_subdirectory(application/non_gtest_api_tests_zephyr/mq1)
    add_subdirectory(application/non_gtest_api_tests_zephyr/mq2)
    add_subdirectory(application/non_gtest_api_tests_zephyr/mutex)
    add_subdirectory(application/non_gtest_api_tests_zephyr/pm1)
    add_subdirectory(application/non_gtest_api_tests_zephyr/pm2)
    add_subdirectory(application/non_gtest_api_tests_zephyr/resource)
    add_subdirectory(application/non_gtest_api_tests_zephyr/thread)
    add_subdirectory(application/non_gtest_api_tests_zephyr/timeclock)
    add_subdirectory(application/non_gtest_api_tests_zephyr/version)
endif()
if(RCAR_TARGET_OS STREQUAL "qnx")
    add_subdirectory(application/chage_prio_affinity_api_tests)
endif()
# add_subdirectory(application/performance_tests/thread)
# add_subdirectory(application/performance_tests/timeclock)
# add_subdirectory(application/performance_tests/interrupt)
# add_subdirectory(application/performance_tests/cond)
# add_subdirectory(application/performance_tests/mutex)
# add_subdirectory(application/performance_tests/message)
# add_subdirectory(application/performance_tests/common)
# add_subdirectory(application/performance_tests/power)
# add_subdirectory(application/performance_tests/io)
# add_subdirectory(application/performance_tests/memory)
# add_subdirectory(application/legacy)
