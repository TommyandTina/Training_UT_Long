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
set(app_name    "osal_sample_memory")       # Give the name a meaning full name
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
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/osal_sample_memory_main.c
    # add more files here
)

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
    # add more header files here
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/osal_sample_memory_common.h
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
set(link_lib
    # This sample does not need any other sw compoents
    # Examples could be: osal, osal_wrapper
)

# [Optional] for 3rd Party libraries
set(link_dir
    # Additional Link directories for 3rd party libraries
)

#----------------------------------------------------------------------------------------------------------------------
# Set document file list
# If not set, it will search for document in ${CMAKE_CURRENT_SOURCE_DIR}/doc by default
#----------------------------------------------------------------------------------------------------------------------
set(document
    # List of document file to install to package
)

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

    if(RCAR_XIL STREQUAL "hil")
        list(APPEND link_lib
            osal_configuration
        )
    endif()

    if(RCAR_TARGET_OS STREQUAL "emcos")
        list(APPEND link_lib
            emcos
        )
    endif()

endif()

add_definitions(-DSUPPORT_MULTI_MEMORY_REGION)

if(RCAR_XIL STREQUAL hil)
    if(RCAR_TARGET_OS STREQUAL "linux")
        add_definitions(-DSAMPLE_LINUX)
    endif()
endif()

if(RCAR_TARGET_OS STREQUAL "zephyr")
    add_definitions(-DCONFIG_ADDR_STACK)
    add_definitions(-DCONFIG_SMALL_STACKSIZE)
    add_definitions(-DENABLE_RESOURCE_OSAL)
    remove_definitions(-DSUPPORT_MULTI_MEMORY_REGION)
endif()

# Linker dependencies for baremetal/freertos environments
if (CMAKE_LINKER MATCHES "armlink")
    if(RCAR_TARGET_OS STREQUAL "emcos")
        set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/default_cxx_proc_scatter.txt")
    else()
        set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/application/lscript.scat")
    endif()
endif()

set(test_end_pattern
    # Only for Zephyr OS
    # Set this variable to help Zephyr OS test knows when the application is completed
    # When the log matches this pattern, R-Car Env test script will stop and turn off the board
    # E.g:
    # set(test_end_pattern "<APP_END>")
    # set(test_end_pattern "<Finish running app>")
    "================== osal_sample_memory finish =================="
)

set(doxygen_enable_for_source TRUE)

if (RCAR_SOC MATCHES "s4") # Target Gateway
    if("${RCAR_TARGET_OS}" STREQUAL "linux")
        set(api_doc "NO")
        set(document_remote_path "rcar-docs/xos3/dev-xos3/osal/3.14.0.gateway")
        set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal")
        set(document_files
            api_manual/r11an0632ej0340-osal.pdf
        )
    elseif ("${RCAR_TARGET_OS}" STREQUAL "qnx")
        set(api_doc "YES")
    endif()
elseif (RCAR_SOC MATCHES "(v3|v4)") # Target ADAS
    set(api_doc "YES")
endif()
#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()

if (RCAR_SOC MATCHES "s4") # Target Gateway
    if("${RCAR_TARGET_OS}" STREQUAL "linux")
        if(COMMAND rcar_artifactory_download)
            rcar_artifactory_download(DOCUMENT
                TARGET          ${app_name}
                REMOTE_PATH     ${document_remote_path}
                LOCAL_PATH      ${document_local_path}
                FILES           ${document_files}
            )
        endif()
    elseif ("${RCAR_TARGET_OS}" STREQUAL "qnx")
        if(RCAR_PACKAGE_DOC_DIR)
        set_property(TARGET ${app_name} PROPERTY __DOCUMENT_INSTALL_DIR
            ${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal/)
        endif()
    endif()
elseif (RCAR_SOC MATCHES "(v3|v4)") # Target ADAS
    if(RCAR_PACKAGE_DOC_DIR)
    set_property(TARGET ${app_name} PROPERTY __DOCUMENT_INSTALL_DIR
        ${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal/)
    endif()
endif()
