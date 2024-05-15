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
set(module_name         "osal_dev_cfg" )

set(module_version      "0.1.0")

# Additinal information (product number, current quality, target quality)
set(product_number      "-")
set(current_quality     "Reference")     # Current Quality level: Sample, Reference, Alpha, Beta, QM-MP, ASILx-MP
set(target_quality      "Reference")     # Target Quality level:  Sample, Reference, QM-MP, ASILx-MP

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------

# SoC dependent or not. Allowed values are [TRUE, FALSE]
set(soc_dependent       TRUE)

# Sets the modules linkage type. Allowed values are [<empty>, STATIC, INTERFACE, SHARED, MODULE]
set(module_linkage      STATIC)

# Workaround of emcos header file issue
if (RCAR_TARGET_OS STREQUAL "emcos")
   set(api_doc "NO")
endif()

#----------------------------------------------------------------------------------------------------------------------
# Common Source/Header configuration
#----------------------------------------------------------------------------------------------------------------------

# Source files
set(source
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
set(external_public_include_dir_emcos
    # Add external (3rd Party) include directories that are required by this module
    # Only add directories that are really required by the public interface of this module. Handle with care as this
    # can break the export possibilities of the module
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
)

# [Optional]
set(link_lib_private
    # add any link dependency you may have that you do not want to export.
    # Note if you select build type "INTERFACE" this variable must be empty or not set
)

# [Optional] for 3rd Party libraries
set(link_dir
    # Additional Link directories for 3rd party libraries
)

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_module()
