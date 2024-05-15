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
#######################################################################################################################
# History   Version   DD.MM.YYYY   Description
#           0.8.1     11.12.2020   Add sample for impfw.
#           0.8.1     11.05.2021   Update template.
#######################################################################################################################

#----------------------------------------------------------------------------------------------------------------------
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name    "imp_cdnn_test")
set(app_version "0.8.1")

cmake_minimum_required(VERSION 3.16.3)
project(${app_name})

#----------------------------------------------------------------------------------------------------------------------
# R-Car xOS SoC/Board selection
#----------------------------------------------------------------------------------------------------------------------

if(NOT RCAR_SOC) # This guard allows to set RCAR_SOC externally, e.g. via CMake Commandline during configuration
    set(RCAR_SOC
        V4H # Allowed values are at the moment V3H1, V3M2. Availability depends on the package you have.
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

if(RCAR_SOC STREQUAL "v4h")
    add_definitions(-DRCAR_V4H)
else()
    return()
endif()

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/main.c
    # add more files here
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/imp_demo_sub.c
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/imp_demo_expected_result.c
)

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
    # add more header files here
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/imp_demo.h
)


#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    ${rcar-xos_INCLUDE_DIRS}
    # add any additional 3rd party include directories your application requires
    # Note: Include directories from xOS Software components are automatically added when you list them in link_libs
)

#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(link_lib
    osal
    impfw
    imp
    atmlib
    osal_wrapper
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
    elseif(RCAR_TARGET_OS STREQUAL "emcos")
        list(APPEND link_lib
            osal_configuration
            emcos
        )
    endif()
endif()

# Linker dependencies for baremetal/freertos environments
if (CMAKE_LINKER MATCHES "armlink")
    if(RCAR_TARGET_OS STREQUAL "emcos")
        set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/application/default_cxx_proc_scatter.txt")
    else()
        set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/application/lscript.scat")
    endif()
endif()

# do not execute sample of multiple request if target is simulator
if(RCAR_XIL STREQUAL sil)
    add_definitions(-DENABLE_SAMPLE2=0)
else()
    add_definitions(-DENABLE_SAMPLE2=1)
endif()

# Test config
if(TARGET dsp_sample_cdnn AND TARGET simtools_dsp)
    # Get dsp server and dsp_samle elf file location
    get_target_property(dsp_verver simtools_dsp _BINARIES)
    get_target_property(dsp_sample_elf dsp_sample_cdnn _BINARIES)
    list(FILTER dsp_verver INCLUDE REGEX "(dsp_server$|dsp_server\\.exe$)")
    list(FILTER dsp_sample_elf INCLUDE REGEX "(\\.elf$)")
    string(REPLACE ";" " " dsp_sample_elf "${dsp_sample_elf}")
    set(test_pretest_cmd "${dsp_verver} 12345 ${dsp_sample_elf} &")
endif()

rcar_configure_application()

# dsp_sample and simtools_dsp need to be built before this app
if(TARGET dsp_sample_cdnn AND TARGET simtools_dsp)
    add_dependencies(${app_name} dsp_sample_cdnn)
    add_dependencies(${app_name} simtools_dsp)
endif()
