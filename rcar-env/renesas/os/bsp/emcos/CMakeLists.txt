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
######################################################################################################################$#
# Set version
if(RCAR_SOC STREQUAL "v3u")
    set(version     "2.0.0")
    set(remote_path "rcar-tools/renesas/emcosbsp/${version}.alpha2+1")
else()
    # V4H
    set(version     "2.1.0.rev3.Alpha+3")
    set(remote_path "rcar-tools/renesas/emcosbsp/${version}")
endif()

# Set local_path based on call source (from build system or package vefication CI job)
if(NOT RCAR_EMCOS_DOWNLOAD_DIR)
    set(local_path "${CMAKE_BINARY_DIR}/${RCAR_PACKAGE_VERSION_DIR}/os/emcos")
else()
    set(local_path "${RCAR_EMCOS_DOWNLOAD_DIR}")
endif()

# Download eMCOS pre-build
rcar_tool_setup_server()
message(STATUS "Downloading eMCOS pre-build ...")
execute_process(COMMAND ${JFROG} "rt" "dl" "--insecure-tls=true" "--flat=true"
    "${remote_path}/emcos_bsp_prebuild.tar.xz" "${local_path}/emcos_bsp_prebuild.tar.xz"
    ERROR_VARIABLE  error_log
    RESULT_VARIABLE exec_status
)
if(NOT exec_status EQUAL 0)
    message(FATAL_ERROR "Download load prebuilt eMCOS fail")
endif()

# Extract eMCOS pre-build
message(STATUS "Extracting eMCOS pre-build ...")
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf
    ${local_path}/emcos_bsp_prebuild.tar.xz
    WORKING_DIRECTORY ${local_path}
)

# If this file is called from build system
if(NOT RCAR_CALL_FROM_CTEST_SCRIPT)
    # Install eMCOS CMake file to the package
    add_custom_target(emcos_cmake ALL)
    if(RCAR_SOC STREQUAL "v3u")
        set(cmake_file
            os/emcos/cmake/emcos-aarch64-armclang-emcos-v3u-config.cmake
            os/emcos/cmake/emcos-aarch64-armclang-emcos-v3u-config-version.cmake
            os/emcos/cmake/emcos-aarch64-armclang-emcos-v3u-export.cmake
            os/emcos/cmake/emcos-config.cmake
            os/emcos/cmake/emcos-config-version.cmake
        )
    else()
        set(cmake_file
            os/emcos/cmake/emcos-aarch64-armclang-emcos-v4h-config.cmake
            os/emcos/cmake/emcos-aarch64-armclang-emcos-v4h-config-version.cmake
            os/emcos/cmake/emcos-aarch64-armclang-emcos-v4h-export.cmake
            os/emcos/cmake/emcos-config.cmake
            os/emcos/cmake/emcos-config-version.cmake
        )
    endif()
    set_target_properties(emcos_cmake PROPERTIES __CMAKES "${cmake_file}")

    # Install CMake file
    list(TRANSFORM cmake_file REPLACE "os/emcos" ${CMAKE_CURRENT_SOURCE_DIR}/)
    set(install_dir ${RCAR_PACKAGE_VERSION_DIR}/os/emcos/cmake)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${install_dir})

    # Configuration stage
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy ${cmake_file}
                ${CMAKE_BINARY_DIR}/${install_dir}
    )
    # Installation stage
    install(
        FILES ${cmake_file}
        DESTINATION ${install_dir}
    )

    # emcos patches
    add_custom_target(emcos_patch ALL)
    file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/patch/*)
    set_target_properties(emcos_patch PROPERTIES
        SOURCES "${sources}"
        _SOURCE_INSTALL_DIR ${RCAR_PACKAGE_VERSION_DIR}/os/emcos/patch
    )   
    set(RCAR_EMCOS_PATCH_TYPE "SOURCE" CACHE STRING "" FORCE)

    message(STATUS "Finding emcos package ...")
    find_package(emcos REQUIRED
        HINTS ${CMAKE_BINARY_DIR}/${install_dir}
    )
endif()
