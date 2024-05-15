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
#           1.0.0     14.09.2021   Initial version
#######################################################################################################################

if (CMAKE_VERSION VERSION_LESS 3.16.3)
    message(FATAL_ERROR "emcos requires at least CMake version 3.16.3")
endif()

# Check RCAR_SOC, RCAR_BOARD is set
if(NOT RCAR_SOC)
    if(NOT RCAR_BOARD)
        set(emcos_NOT_FOUND_MESSAGE
            "The emcos package requires variable RCAR_SOC (environment, cmake cache or current cmake scope) to be set. Alternative set the variable RCAR_BOARD (environment, cmake cache or current cmake scope) to a valid value (eagle, v3msk, condor, v3hsk)"
        )
        set(emcos_FOUND False)
        return()
    endif()
endif()

# The REQUIRED option stops processing with an error message if the package cannot be found
set(_emcos_FIND_PARTS_REQUIRED)
if (emcos_FIND_REQUIRED)
    set(_emcos_FIND_PARTS_REQUIRED REQUIRED)
endif()

# The QUIET option disables informational messages, including those indicating that the package cannot be found if it is not REQUIRED
set(_emcos_FIND_PARTS_QUIET)
if (emcos_FIND_QUIETLY)
    set(_emcos_FIND_PARTS_QUIET QUIET)
endif()

# Get sub package configuration (Export set) name
set(processor "aarch64")

string(TOLOWER "${RCAR_TARGET_OS}" l_os)
set(CMAKE_SYSTEM_NAME "emcos")
set(platform        "emcos")
string(TOLOWER ${CMAKE_C_COMPILER_ID}   compiler)
string(TOLOWER ${CMAKE_SYSTEM_NAME}     system)
string(TOLOWER ${RCAR_SOC}              soc)
set(export_set ${platform}-${processor}-${compiler}-${system}-${soc})

# Finding sub package configuration file
#message(STATUS "Finding export set: ${export_set} ...")
find_package(${export_set} 3.0.0
    ${_emcos_FIND_PARTS_QUIET}
    ${_emcos_FIND_PARTS_REQUIRED}
    PATHS "${CMAKE_CURRENT_LIST_DIR}"  NO_DEFAULT_PATH
)
if (NOT ${export_set}_FOUND)
    if (emcos_FIND_REQUIRED)
        set(_emcos_NOTFOUND_MESSAGE "${_emcos_NOTFOUND_MESSAGE} Failed to find matching export set\n")
    elseif(NOT emcos_FIND_QUIETLY)
        message(WARNING "Failed to find emcos component")
    endif()
    unset(_expected_module_location)
endif()

# <PackageName>_FOUND will be set to indicate whether the package was found
if (_emcos_NOTFOUND_MESSAGE)
    set(emcos_NOT_FOUND_MESSAGE "${_emcos_NOTFOUND_MESSAGE}")
    set(emcos_FOUND False)
endif()
