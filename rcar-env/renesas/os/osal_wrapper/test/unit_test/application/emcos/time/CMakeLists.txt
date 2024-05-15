#######################################################################################################################
# Copyright [2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
#find_package(rcar-xos REQUIRED)

#----------------------------------------------------------------------------------------------------------------------
# Set app name
#----------------------------------------------------------------------------------------------------------------------
set(app_name "osal_wrapper_unit_test_time")
set(app_version "0.1.0")

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------
# Same definition as when soc_dependent=TRUE
if(RCAR_SOC STREQUAL "v4h")
    add_definitions(-DRCAR_V4H)
endif()

set(stub true) # Make this line true when using stubs.

if(stub) # use stub
    add_definitions(-DUSE_STUB)
    message(STATUS "*** use stub ***")
else()   # no stub
    message(STATUS "*** no stub ***")
endif()

#----------------------------------------------------------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------------------------------------------------------
if(stub) # use stub
    set(source
        # configuration
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/src/target/emcos/r_osal_resource_info.c

        # pma
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/pma/r_pma_dummy_api.c
#       ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/pma/r_pma_dummy_power.c

        # dev_cfg
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/dev_cfg/r_osal_dev_cfg.c

        # usr_cfg
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/usr_cfg/r_osal_usr_cfg.c

        # wrapper
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_common.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_interrupt.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_io.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_message.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_power.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_thread.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_threadsync.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_memory.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_timeclock.c
        ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/emcos/r_osal_version.c

        # memory allocator library
        ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/memory_allocator/src/r_meml_allocator.c

        #liblfs
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/liblfs/lchown.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/liblfs/lfs_svc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/liblfs/lstat.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/liblfs/poll.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/liblfs/readlink.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/liblfs/symlink.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/liblfs/ulfs.c

        #libmcos
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libmcos/mcos_svc.c

        #libmmap
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libmmap/lfa.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libmmap/lfa_sys.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libmmap/libmmap_svc.c

        #libpthread
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libpthread/pthread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libpthread/pthread_barrier.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libpthread/pthread_cond.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libpthread/pthread_mutex.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libpthread/pthread_rwlock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libpthread/pthread_svc.c

        #libtime
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libtime/clock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libtime/ftime.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libtime/time_svc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/libtime/usleep.c

        #liburing
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub_lib/liburing/uring.c

        # unit test
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/stub.c
    )
else() # no stub
    set(source
        ${CMAKE_CURRENT_SOURCE_DIR}/application/src/main.c
    )
endif()

#----------------------------------------------------------------------------------------------------------------------
# Header files of the application
#----------------------------------------------------------------------------------------------------------------------
set(header)

#----------------------------------------------------------------------------------------------------------------------
# Include directories used by the application
#----------------------------------------------------------------------------------------------------------------------
set(include_dir
    ${CMAKE_SOURCE_DIR}/renesas/os/osal/include
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_configuration/include
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include
    ${CMAKE_SOURCE_DIR}/renesas/middleware/libraries/memory_allocator/include
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/include/
)

if(stub) # use stub
    get_target_property(_INCLUDE_DIR_EMCOS emcos INTERFACE_INCLUDE_DIRECTORIES)
#    message(STATUS "${_INCLUDE_DIR_EMCOS}")
    list(APPEND include_dir ${_INCLUDE_DIR_EMCOS})
endif()
#----------------------------------------------------------------------------------------------------------------------
# Link libraries and Link directories used by the application
#----------------------------------------------------------------------------------------------------------------------
if(stub) # use stub
    get_target_property(_LINK_LIBRARIES_EMCOS emcos INTERFACE_LINK_LIBRARIES)
    list(FILTER _LINK_LIBRARIES_EMCOS EXCLUDE REGEX "(libmcos\\.a$|liblfs\\.a$|libpthread\\.a$|libmmap\\.a$|libtime\\.a$|liburing\\.a$)")
#    message(STATUS "${_LINK_LIBRARIES_EMCOS}")
    list(APPEND link_lib ${_LINK_LIBRARIES_EMCOS})
else() # no stub
    set(link_lib
        osal
        osal_wrapper
        osal_configuration
        emcos
    )
endif()

# [Optional] for 3rd Party libraries
set(link_dir)

# Linker dependencies for baremetal/freertos environments
if (CMAKE_LINKER MATCHES "armlink")
    set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/default_cxx_proc_scatter.txt")
endif()

#----------------------------------------------------------------------------------------------------------------------
# Set document file list
# If not set, it will search for document in ${CMAKE_CURRENT_SOURCE_DIR}/doc by default
#----------------------------------------------------------------------------------------------------------------------
set(document)

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()
