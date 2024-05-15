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
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "osal_wrapper_functional_test_MQ")     # Give the name a meaning full name
set(app_version "3.3.0")                    # Set the version of the application

cmake_minimum_required(VERSION 3.16.3)
project(${app_name})

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
find_package(rcar-xos REQUIRED)

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/test_mq_send_receive.c
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/r_osal_configuration_zephyr.c
    # add more files here
)

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
    # add more header files here
     ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/test_mq_send_receive.h
)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    # add any additional 3rd party include directories your application requires
    # Note: Include directories from xOS Software components are automatically added when you list them in link_libs
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include
    ${rcar-xos_INCLUDE_DIRS}
)

#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(link_lib)

# [optional] for 3rd Party libraries
set(link_dir)

#----------------------------------------------------------------------------------------------------------------------
# [Optional] Any specifics that an application may require, like OS specifics, SoC specifics
#----------------------------------------------------------------------------------------------------------------------
# Example for enabling OSAL for certain environments
if(RCAR_TARGET_OS STREQUAL "baremetal" )
    list(APPEND link_lib
        startup         # In baremetal environments the application requires at least startup code defined for xOS
    )
else() # Any other Target OS
    list(APPEND link_lib
        osal            # This application makes use of the OSAL API
        osal_wrapper    # The application also needs the implementation of the OSAL API to link.
    )
endif()

if(RCAR_TARGET_OS STREQUAL "zephyr")
    add_definitions(-DCONFIG_ADDR_STACK)
    add_definitions(-DCONFIG_SMALL_STACKSIZE)
    add_definitions(-DENABLE_RESOURCE_OSAL)
    add_definitions(-DENABLE_RESOURCE_IMR)
    add_definitions(-DCONFIG_MQ_TEST_ZEPHYR)
endif()

set(test_end_pattern
    # Only for Zephyr OS
    # Set this variable to help Zephyr OS test knows when the application is completed
    # When the log matches this pattern, R-Car Env test script will stop and turn off the board
    # E.g:
    # set(test_end_pattern "<APP_END>")
    # set(test_end_pattern "<Finish running app>")
    "================== mq_message_test finish =================="
)

set(test_fail_regex
    # Set this variable if this test has other fail log than the default
    # Default fail regex is: [^RESULT]<FAIL>)|ERROR|error:|Segmentation fault|result NG
    "(([^RESULT]<FAIL>)|ERROR|error:|Error:|Segmentation fault|result NG|error\\.)"
    # TODO: Correct the number of NGs as the implementation improves
)
#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()

if(HAVE_MALLINFO OR HAVE_MALLINFO2)
    target_compile_definitions(${app_name} PRIVATE HAVE_MALLINFO)
endif()

if(HAVE_MALLINFO2)
    target_compile_definitions(${app_name} PRIVATE HAVE_MALLINFO2)
endif()
if(TARGET osal_configuration)
    # W/A to include OSAL configuration without actual link to it
    if(COMMAND rcar_get_public_include_dirs)
        rcar_get_public_include_dirs(osal_configuration_include_dir osal_configuration)
    endif()
    target_include_directories(${app_name} PRIVATE
        ${osal_configuration_include_dir}
    )

    get_target_property(osal_configuration_compile_definition osal_configuration INTERFACE_COMPILE_DEFINITIONS)
    target_compile_definitions(${app_name} PRIVATE ${osal_configuration_compile_definition})
endif()
