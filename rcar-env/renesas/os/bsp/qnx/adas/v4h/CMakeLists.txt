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

# Workaround for IPL installation
set(module_name         "bsp")
set(module_version      "7.1.0")

# Additinal information (product number, current quality, target quality)
set(product_number      "-")
set(current_quality     "Reference")            # Current Quality level: Sample, Reference, Alpha, Beta, QM-MP, ASILx-MP
set(target_quality      "Reference")            # Target Quality level:  Sample, Reference, QM-MP, ASILx-MP

set(qnx_bsp_install_dir ${CMAKE_CURRENT_SOURCE_DIR}/src/install)

# Public Header that are exported and can be used by application or other modules
set(public_header
    ${qnx_bsp_install_dir}/usr/include/hw/rcar_cpg.h
    ${qnx_bsp_install_dir}/usr/include/hw/rcar_sysc.h
)
# Include dir that are exported and can be used by application or other modules
set(include_dir
    ${qnx_bsp_install_dir}/usr/include
)

# Link lib that are exported and can be used by application or other modules
set(link_lib
    ${qnx_bsp_install_dir}/aarch64le/usr/lib/librcar-syscS.a
    ${qnx_bsp_install_dir}/aarch64le/usr/lib/librcar-cpgS.a
)

if(RCAR_SOC STREQUAL "v4h")
    set(binary
        ${CMAKE_CURRENT_SOURCE_DIR}/src/images/ifs-rcar_v4h.bin
    )
endif()

set(document_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/doc
)


#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
add_library(${module_name} INTERFACE)

if(NOT GIT_EXECUTABLE)
    find_package(Git REQUIRED)
endif()
if(NOT QNX_MAKE)
    find_program(QNX_MAKE NAMES   "make"  PATHS "${QNX_HOST}/usr/bin" NO_DEFAULT_PATH REQUIRED)
    #only search QNX installation for Make
endif()

# Call BSP Makefile
file(RELATIVE_PATH bsp_project ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add custom target required for QNX BSP target
add_custom_target(build_${module_name}
    COMMAND ${GIT_EXECUTABLE} lfs pull -I ${bsp_project}
    COMMAND ${QNX_MAKE} -C ${bsp_project}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
set_target_properties(build_${module_name} PROPERTIES INTERFACE_VIRTUAL_TARGET TRUE)
#add_dependencies(${module_name} build_${module_name})

# Install static libraries to package
install(FILES ${link_lib} DESTINATION ${RCAR_PACKAGE_BINARY_TYPE_LIBRARY_DIR})
foreach(lib ${link_lib})
    get_filename_component(lib_name ${lib} NAME)
    list(APPEND link_lib_install_inferface \${_IMPORT_PREFIX}/${RCAR_PACKAGE_BINARY_TYPE_LIBRARY_DIR}/${lib_name})
    list(APPEND link_lib_content_md        ${RCAR_PACKAGE_BINARY_TYPE_LIBRARY_DIR}/${lib_name})
endforeach()

target_link_libraries(${module_name} INTERFACE
    "$<BUILD_INTERFACE:${link_lib}>"
    "$<INSTALL_INTERFACE:${link_lib_install_inferface}>"
)

# Install public header to package
set(include_dir_install_interface   ${RCAR_PACKAGE_BINARY_TYPE_INCLUDE_DIR}/rcar-xos/bsp)
foreach(header ${public_header})
   get_filename_component(header_name ${header} NAME)
   list(APPEND public_header_install_interface ${include_dir_install_interface}/hw/${header_name})
endforeach()

# Install QNX BSP public header to include/rcar-xos/bsp/hw folder
install(FILES ${public_header} DESTINATION ${include_dir_install_interface}/hw)
target_include_directories(${module_name} INTERFACE
    "$<BUILD_INTERFACE:${include_dir}>"
    "$<INSTALL_INTERFACE:${include_dir_install_interface}>"
)

# Install binary to package
install(FILES ${binary}
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
get_filename_component(binary_name ${binary} NAME)

# Set require property for CONTENT.md
set_target_properties(${module_name} PROPERTIES
    INTERFACE_DOCUMENT_DIR  "${document_dir}"
    INTERFACE_SOURCES       "$<BUILD_INTERFACE:${sources}>"
    _SOURCE_DIR             "${CMAKE_CURRENT_SOURCE_DIR}"
    _PRODUCT_VERSION        "${module_version}"
    _PRODUCT_NUMBER         "${product_number}"
    _CURRENT_QUALITY        "${current_quality}"
    _TARGET_QUALITY         "${target_quality}"
    __LIBRARIES             "${link_lib_install_inferface}"
    __BINARIES              "${RCAR_PACKAGE_BINARY_TYPE_BINARY_DIR}/${binary_name}"
    __PUBLIC_HEADERS        "${public_header_install_interface}"
)

