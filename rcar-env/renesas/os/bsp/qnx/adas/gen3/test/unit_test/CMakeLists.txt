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
set(app_name    "bsp_unit_test_qnx")
set(app_version "0.1.0")

set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.10.2)
project(${app_name})


#----------------------------------------------------------------------------------------------------------------------
# R-Car xOS SoC/Board selection
#----------------------------------------------------------------------------------------------------------------------

if(NOT RCAR_SOC) # This guard allows to set RCAR_SOC externally, e.g. via CMake Commandline during configuration
    set(RCAR_SOC
        V3H2 # Allowed values are at the moment V3H1, V3M2. Availability depends on the package you have.
    )
endif()


find_package(rcar-xos REQUIRED)


#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_pm.cpp

)

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header
)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
)


set(link_lib
    utils
    gtest
    gmock
)





include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_CONFIG "http.sslVerify=false"
    GIT_REPOSITORY https://rcar-env.dgn.renesas.com/gitlab/google/googletest.git
    GIT_TAG        release-1.10.0
)
set(googletest_BUILD_GMOCK TRUE)
set(BUILD_GMOCK TRUE)
set(googletest_gtest_force_shared_crt ON)
set(gtest_force_shared_crt ON)


FetchContent_MakeAvailable(googletest)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL QNX)
    # on QNX the googletest requires the extensions to be enabled to compile. Enable them only for the targets
    set_target_properties(gtest gmock gtest_main gmock_main PROPERTIES CXX_EXTENSIONS ON)
    set(CMAKE_CXX_EXTENSIONS ON)

    if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 8.0.0)
        # newer QNX version do have regex seperated for the C-Library. This is not covered by googletest yet.
        list(APPEND link_lib regex)
    endif()
endif()

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()




