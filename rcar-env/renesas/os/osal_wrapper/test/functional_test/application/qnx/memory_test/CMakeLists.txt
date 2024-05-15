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
#######################################################################################################################
# History   Version   DD.MM.YYYY   Description
#           1.0.0     02.09.2020   Initial revision
#           1.0.1     14.10.2020   Add project definition for standalone build
#######################################################################################################################

#----------------------------------------------------------------------------------------------------------------------
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "osal_wrapper_functional_test_memory")    # Give the name a meaning full name
set(app_version "1.0.0")                    # Set the version of the application

cmake_minimum_required(VERSION 3.16.3)
project(${app_name})

#----------------------------------------------------------------------------------------------------------------------
# R-Car xOS SoC/Board selection
#----------------------------------------------------------------------------------------------------------------------

# SoC dependent or not (TRUE or FALSE)
if(RCAR_XIL STREQUAL sil)
    set(soc_dependent       TRUE)
else()
    set(soc_dependent       FALSE)
endif()

if(NOT RCAR_SOC) # This guard allows to set RCAR_SOC externally, e.g. via CMake Commandline during configuration
    set(RCAR_SOC
        V3H1 # Allowed values are at the moment V3H1, V3M2. Availability depends on the package you have.
    )
endif()

# Alternative: Selection via your board:
# if(NOT RCAR_BOARD) # This guard allows to set RCAR_BOARD externally, e.g. via CMake Commandline during configuration
#     set(RCAR_BOARD
#         v3hsk # eagle, v3msk, condor, v3hsk. Availability depends on the package you have.
#     )
# endif()

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
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/osal_memory_sample.c
    # add more files here
)

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/osal_memory_sample.h
    # add more header files here
)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    # add any additional 3rd party include directories your application requires
    # Note: Include directories from xOS Software components are automatically added when you list them in link_libs
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include
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

    if(RCAR_TARGET_OS STREQUAL "qnx")
        list(APPEND link_lib
            osal_configuration
        )
    endif()

endif()

# Linker dependencies for baremetal/freertos environments
if (CMAKE_LINKER MATCHES "armlink")
    set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/application/lscript.scat")
endif()

if(RCAR_TARGET_OS STREQUAL "qnx")
    add_definitions(-DOSAL_NOT_TEST_QNX)
endif()

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()
