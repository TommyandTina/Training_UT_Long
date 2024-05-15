#######################################################################################################################
# Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
set(app_name    "osal_wrapper_functional_test_it_debug")
set(app_version "2.0.0")

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------

# SoC dependent or not. Allowed values are [TRUE, FALSE]
#note: It is not effective. Because osal_wrapper is soc_dependent=FALSE
#set(soc_dependent       TRUE)

# Same definition as when soc_dependent=TRUE
if(RCAR_SOC STREQUAL "v3u")
    add_definitions(-DRCAR_V3U)
elseif(RCAR_SOC STREQUAL "v3h1")
    add_definitions(-DRCAR_V3H1)
elseif(RCAR_SOC STREQUAL "v3m2")
    add_definitions(-DRCAR_V3M2)
elseif(RCAR_SOC STREQUAL "v3h2")
    add_definitions(-DRCAR_V3H2)
elseif((RCAR_SOC STREQUAL "v4h") OR (RCAR_SOC STREQUAL "v4h2"))
    add_definitions(-DRCAR_V4H)
elseif(RCAR_SOC STREQUAL "v4m")
    add_definitions(-DRCAR_V4M)
elseif(RCAR_SOC STREQUAL "s4")
    add_definitions(-DRCAR_S4)
endif()

add_definitions(-DSUPPORT_IO_EXTENSION)
#add_definitions(-DUNSUPPORT_IO_EXTENSION)


#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal_test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osal_test_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/io_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memory_manager.c
)

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../src/include
)

#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(link_lib)

# [optional] for 3rd Party libraries
set(link_dir)

#----------------------------------------------------------------------------------------------------------------------
# [optional] Any specifics that an application may require, like OS specifics, SoC specifics
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
        osal_configuration
    )
endif()

# Linker dependencies for baremetal/freertos environments
if (CMAKE_LINKER MATCHES "armlink")
    set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/application/lscript.scat")
endif()

# Linker dependencies for baremetal/freertos environments
if (CMAKE_LINKER MATCHES "armlink")
    set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/lscript.scat")
endif()


# CTest configuration
set(test_arg
    "100"
)

set(test_fail_regex
    # Set this variable if this test has other fail log than the default
    # Default fail regex is: [^RESULT]<FAIL>)|ERROR|error:|Segmentation fault|result NG
    "(([^RESULT]<FAIL>)|ERROR|error:|Segmentation fault|result NG|error\\.)"
    # TODO: Correct the number of NGs as the implementation improves
)

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()
