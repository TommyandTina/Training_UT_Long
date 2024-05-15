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

# Disable in-source build
rcar_check_for_insource_build()

#----------------------------------------------------------------------------------------------------------------------
# Set module information
#----------------------------------------------------------------------------------------------------------------------
set(module_name        "freertos" )
set(module_version     "10.1.0")

# Additinal information (product number, current quality, target quality)
set(product_number      )
set(current_quality     )           # Current Quality level: Sample, Reference, Alpha, Beta, QM-MP, ASILx-MP
set(target_quality      "ASIL B")     # Target Quality level:  Sample, Reference, QM-MP, ASILx-MP

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------

# SoC dependent or not (TRUE or FALSE)
set(soc_dependent       TRUE)

# Sets the modules linkage type. Allowed values are [<empty>, STATIC, INTERFACE, SHARED, MODULE]
set(module_linkage      INTERFACE)

#----------------------------------------------------------------------------------------------------------------------
# Common Source/Header configuration
#----------------------------------------------------------------------------------------------------------------------

# Source files
set(source
    # Note if you select build type "INTERFACE" this variable must be empty or not set
)

# Interface source files that always needs to be compiled with the application and not during artifact generation
# Example: UDF sources
set(interface_source

)

# Public Header that are exported and can be used by application or other modules
set(public_header
)

# Private Header files that are not exported and cannot be used by other components or modules
set(private_header
    # Note if you select build type "INTERFACE" this variable must be empty or not set
)

# [Optional] An external include directory. This information is exported with the modules
set(external_public_include_dir
    # Add external (3rd Party) include directories that are required by this module
    # Only add directories that are really required by the public interface of this module. Handle with care as this
    # can break the export possibilities of the module
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/port/GCC/ARM_CA53_64_BIT_EL1
)

# [Optional] An external include directory required to build this module
set(external_private_include_dir
    # Exceptions are only allowed in case of 3rd Party dependencies
    # Note if you select build type "INTERFACE" this variable must be empty or not set
)

#----------------------------------------------------------------------------------------------------------------------
# Common link dependencies
#----------------------------------------------------------------------------------------------------------------------
set(link_lib
    osal 
)

# [optional]
set(link_lib_private
    # add any link dependency you may have that you do not want to export.
    # Note if you select build type "INTERFACE" this variable must be empty or not set
)

# [optional] for 3rd Party libraries
set(link_dir
    # Additional Link directories for 3rd party libraries
)

# [optional] for 3rd Party libraries
set(link_dir_private
    # Additional Link directories for 3rd party libraries that you do not want to export
)

#----------------------------------------------------------------------------------------------------------------------
# OS/SoC or environment specific Source/Header configuration
# Any specifics you may need add a suffix (lower letters) to the variable defined in the common section
# E.g. Specific sources for FreeRTOS:   source_freertos
# E.g. Specific sources for Linux:      source_linux
# E.g. Specific sources for HIL Env:    source_hil
# E.g. Specific sources for V3H1 SoC:   source_v3h1
#----------------------------------------------------------------------------------------------------------------------

# FreeRTOS specific
set(interface_source_freertos
    ${CMAKE_CURRENT_SOURCE_DIR}/port/freeRTOSHooks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/port/FreeRTOS_tick_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/port/platform_post_lib_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/port/GCC/ARM_CA53_64_BIT_EL1/port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/port/GCC/ARM_CA53_64_BIT_EL1/portASM.S
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/stream_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/timers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/portable/MemMang/heap_4.c
)

set(public_header_freertos
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/croutine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/deprecated_definitions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/event_groups.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/FreeRTOS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/message_buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/mpu_prototypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/mpu_wrappers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/portable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/projdefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/queue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/semphr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/stack_macros.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/StackMacros.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/stdint.readme
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/stream_buffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/task.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Source/include/timers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/port/GCC/ARM_CA53_64_BIT_EL1/portmacro.h
)

# FreeRTOS and V3H1 specific
set(link_lib_freertos
    xil_common
    xil_ttcps
    xil_scugic
    xil_uartps
    startup
)

set(link_lib_freertos_v3h1
    isr_v3h1
)

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_module()
