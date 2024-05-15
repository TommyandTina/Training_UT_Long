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

set(module_name         "zephyr_bsp")
set(module_version      "0.3.5")

# Additinal information (product number, current quality, target quality)
set(product_number      "-")
set(current_quality     "Reference")            # Current Quality level: Sample, Reference, Alpha, Beta, QM-MP, ASILx-MP
set(target_quality      "Reference")

add_custom_target(${module_name} ALL)

set_target_properties(${module_name} PROPERTIES
    _PRODUCT_VERSION             "${module_version}"
    _PRODUCT_NUMBER              "${product_number}"
    _CURRENT_QUALITY             "${current_quality}"
    _TARGET_QUALITY              "${target_quality}"
)

#----------------------------------------------------------------------------------------------------------------------
# Artifactory document setting
#----------------------------------------------------------------------------------------------------------------------
if (RCAR_TARGET_OS STREQUAL "zephyr")
    set(document_remote_path "rcar-docs/xos3/dev-xos3/zephyr")
    set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/zephyr")
    set(document_files
        users_manual/r11uz0160ej0035-zephyri2cdriver.pdf
        users_manual/r11uz0161ej0035-zephyrscifdriver.pdf
        users_manual/r11uz0162ej0035-zephyrsyscdriver.pdf
        users_manual/r11uz0163ej0035-zephyrcpgdriver.pdf
        users_manual/r11uz0246ej0035-zephyretheravbdriver.pdf
        users_manual/r11uz0259ej0035-zephyrcachedriver.pdf
        )
    rcar_artifactory_download(DOCUMENT
        TARGET          ${module_name}
        REMOTE_PATH     ${document_remote_path}
        LOCAL_PATH      ${document_local_path}
        FILES           ${document_files}
    )
endif()

# Enable release note automation generation (default NO)
set(release_note "YES")

set(release_note_json "${CMAKE_CURRENT_SOURCE_DIR}/doc/release_note/release_note.json")

# Release note PDF file name (if not set, use default file name ${module}_ReleaseNote)
set(release_note_file_name "R-CarV4H_Zephyr_Drivers_ReleaseNote")

set_target_properties(${module_name} PROPERTIES
   _ENABLE_RELEASE_NOTE "${release_note}"
   _RELEASE_NOTE_FILE_NAME "${release_note_file_name}"
   _RELEASE_NOTE_JSON "${release_note_json}"
)

# zephyr patches
add_custom_target(zephyr_patch ALL)
file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/patch/*)
list(FILTER sources         INCLUDE REGEX "bsp_zephyr|osal_zephyr|base-commit")
set_target_properties(zephyr_patch PROPERTIES
    SOURCES "${sources}"
    _SOURCE_INSTALL_DIR ${RCAR_PACKAGE_VERSION_DIR}/os/zephyr
)   
set(RCAR_ZEPHYR_PATCH_TYPE "SOURCE" CACHE STRING "" FORCE)
