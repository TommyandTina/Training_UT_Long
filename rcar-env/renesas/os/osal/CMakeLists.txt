#######################################################################################################################
# Copyright [2020-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
set(module_name         "osal" )
set(module_version      "3.7.0")

# Additinal information (product number, current quality, target quality)
set(product_number      "-")
set(current_quality     "-")     # Current Quality level: Sample, Reference, Alpha, Beta, QM-MP, ASILx-MP
set(target_quality      "-")     # Target Quality level:  Sample, Reference, QM-MP, ASILx-MP

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------

# SoC dependent or not. Allowed values are [TRUE, FALSE]
set(soc_dependent       FALSE)

# Sets the modules linkage type. Allowed values are [<empty>, STATIC, INTERFACE, SHARED, MODULE]
set(module_linkage      INTERFACE)

#----------------------------------------------------------------------------------------------------------------------
# Common Source/Header configuration
#----------------------------------------------------------------------------------------------------------------------

# Source files
set(source)

# Interface source files that always needs to be compiled with the application and not during artifact generation
# Example: UDF sources
set(interface_source
)

# Public Header that are exported and can be used by application or other modules
set(public_header
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/osal/r_osal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/osal/r_osal_api.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/osal/r_osal_def.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/osal/r_osal_memory_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/osal/r_osal_types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/osal/r_osal_version.h

    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/osal/r_osal_memory.hpp
)

set(doxygen_exclude_pattern
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/osal/r_osal_memory.hpp
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

#----------------------------------------------------------------------------------------------------------------------
# Artifactory document setting
#----------------------------------------------------------------------------------------------------------------------
if (RCAR_SOC MATCHES "s4") # Target Gateway
    if("${RCAR_TARGET_OS}" STREQUAL "linux")
        set(document_remote_path "rcar-docs/xos3/dev-xos3/osal/3.14.0.gateway")
        set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal")
        set(document_files
            api_manual/r11an0630ej0311-osal.pdf
            test_spec/R-CarV4H_S4_V3H_V3M_OSAL_TestSpecification.pdf
            user_manual/r11uz0125ej0310-osal.pdf
        )
    elseif ("${RCAR_TARGET_OS}" STREQUAL "qnx")
        set(document_remote_path "rcar-docs/xos3/dev-xos3/osal/3.18.0.gateway")
        set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal")
        set(document_files
            api_manual/r11an0630ej0312-osal.pdf
            test_spec/R-CarV4H_S4_V3H_V3M_OSAL_TestSpecification.pdf
            user_manual/r11uz0125ej0311-osal.pdf
        )
    endif()
elseif (RCAR_SOC MATCHES "(v3|v4)") # Target ADAS
    if (RCAR_TARGET_OS STREQUAL "emcos" OR RCAR_SPECIFIC_NAME STREQUAL "platform-sdk7") # ADAS eMCOS(SDK7)
        set(document_remote_path "rcar-docs/xos3/dev-xos3/osal/3.3.0.rcN")
        set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal")
        set(document_files
            api_manual/OSAL_API_Application_Note.pdf
            test_spec/OSAL_API_Test_Specification.pdf
            user_manual/OSAL_API_Users_Manual.pdf
        )
    else() # ADAS Other OS
        set(document_remote_path "rcar-docs/xos3/dev-xos3/osal/3.22.0.adas")
        set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal")
        set(document_files
            api_manual/r11an0630ej0312-osal.pdf
            test_spec/R-Car_OSAL_TestSpecification.pdf
            user_manual/r11uz0125ej0311-osal.pdf
        )
    endif()
endif()
#----------------------------------------------------------------------------------------------------------------------
# Doxygen document setting
#----------------------------------------------------------------------------------------------------------------------
set(api_doc "YES")

#----------------------------------------------------------------------------------------------------------------------
# Enable release note automation generation (default NO)
#----------------------------------------------------------------------------------------------------------------------
set(release_note "YES")
set(release_note_file_name "R-CarV4H_V4M_S4_V3H_V3M_OSAL_ReleaseNote")

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_module()
if(COMMAND rcar_artifactory_download)
    rcar_artifactory_download(DOCUMENT
        TARGET          ${module_name}
        REMOTE_PATH     ${document_remote_path}
        LOCAL_PATH      ${document_local_path}
        FILES           ${document_files}
    )
endif()
