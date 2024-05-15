#######################################################################################################################
# Copyright [2020-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
set(module_name        "osal_wrapper" )

if(RCAR_XIL STREQUAL "sil" OR RCAR_TARGET_OS STREQUAL "qnx")
    # Set OSAL_WRAPPER release type of SIL env and QNX OS to BINARY regardless of package JSON config
    set(RCAR_OSAL_WRAPPER_TYPE "BINARY" CACHE STRING "Release type" FORCE)
endif()

if(RCAR_XIL STREQUAL sil)
    set(module_version     "2.10.0")
	set(current_quality     "Reference")       # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
	set(target_quality      "Reference")       # Target Quality level:  Reference
else()
    set(document)

    # Additinal information (product number, current quality, target quality)
    if (RCAR_TARGET_OS STREQUAL "linux")
        if (RCAR_TARGET_QUALITY STREQUAL "qm")
            # OSAL for Linux QM
            set(product_number      "RTM8RC0000ZSAL3S00JPL3E/RTM8RC0000ZSAL3Q00JPL3E")
            set(target_quality      "Reference/QM")        # Target Quality level:  Reference/QM
            if (RCAR_SOC MATCHES "s4") # Target Gateway
                set(module_version     "3.12.0")
                set(current_quality     "Reference/QM")    # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
            elseif (RCAR_SOC MATCHES "(v3|v4h)")
                set(module_version     "3.13.0")
                set(current_quality     "Reference/QM")    # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
            elseif (RCAR_SOC MATCHES "(v4m)")
                set(module_version     "3.13.0")
                set(current_quality     "Beta")             # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
            endif()
        else() #Other than QM
            # OSAL for Linux Reference
            set(product_number      "RTM8RC0000ZSAL3S00JPL3E")
            set(target_quality      "Reference")           # Target Quality level:  Reference
            if (RCAR_SOC MATCHES "s4") # Target Gateway
                set(module_version     "3.13.0")
                set(current_quality     "Reference")       # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
            elseif (RCAR_SOC MATCHES "(v3|v4h)")
                set(module_version     "3.13.0")
                set(current_quality     "Beta")            # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
            elseif (RCAR_SOC MATCHES "(v4m)")
                set(module_version     "3.13.0")
                set(current_quality     "Beta")             # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
            endif()
        endif()
    elseif (RCAR_TARGET_OS STREQUAL "qnx")
        if (RCAR_TARGET_QUALITY STREQUAL "asil")
            # OSAL for QNX ASIL D
            set(product_number      "RTM8RC0000ZSAL3D00JPQ4E") # Product number for  ASIL D
            set(target_quality      "ASIL")                # Target Quality level:  ASIL
            if (RCAR_SOC MATCHES "s4") # Target Gateway
                set(current_quality     "ASIL")            # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
                set(module_version     "2.14.0")
            elseif (RCAR_SOC MATCHES "(v3|v4h)")
                set(current_quality     "ASIL")            # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
                set(module_version     "2.15.0")
            elseif (RCAR_SOC MATCHES "(v4m)")
                set(current_quality     "Beta")            # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
                set(module_version     "2.18.0")
            endif()
            # OSAL for QNX ASIL B
#            set(product_number      "RTM8RC0000ZSAL3B00JPQ4E") # Product number for ASIL B
#            set(target_quality      "ASIL")               # Target Quality level:  ASIL
#           if (RCAR_SOC MATCHES "s4") # Target Gateway
#               set(current_quality     "ASIL")            # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
#           elseif (RCAR_SOC MATCHES "(v3|v4)")
#               set(current_quality     "Beta")            # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
#           endif()
        else()
            set(product_number      "-")               # No product number(Unintentional)
        endif()
    elseif (RCAR_TARGET_OS STREQUAL "emcos")
        set(module_version     "0.6.0")
        if (RCAR_TARGET_QUALITY STREQUAL "asil")
            # OSAL for eMCOS ASIL D
            set(product_number      "RTM8RC0000ZSAL3D00JPM1E")
            set(current_quality     "Alpha")       # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
            set(target_quality      "ASIL")        # Target Quality level:  ASIL D
            # OSAL for eMCOS ASIL B
#            set(product_number      "RTM8RC0000ZSAL3B00JPM1E")
#            set(current_quality     "Alpha")       # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
#            set(target_quality      "ASIL")        # Target Quality level:  ASIL D
        else()
            set(product_number      "-")           # No product number(Unintentional)
        endif()
    elseif (RCAR_TARGET_OS STREQUAL "zephyr")
        set(module_version     "0.9.0")
        set(product_number      "RTM8RC0000ZSAL3S00JPE0E")
        set(current_quality     "Reference")       # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
        set(target_quality      "Reference")       # Target Quality level:  Reference
    endif()
endif()
#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------

# SoC dependent or not (TRUE or FALSE)
if(RCAR_XIL STREQUAL sil)
    set(soc_dependent       TRUE)
#    if(RCAR_SOC STREQUAL "v4h")
#        set(private_compile_def_sil RCAR_V3U)
#        #add_definitions(-DRCAR_V3U)
#        message(WARNING "V4H simulators not available. Using V3U instead.")
#    endif()
else()
    set(soc_dependent       FALSE)
endif()

# Sets the modules linkage type. Allowed values are [<empty>, STATIC, INTERFACE, SHARED, MODULE]
set(module_linkage      STATIC)

#----------------------------------------------------------------------------------------------------------------------
# Common Source/Header configuration
#----------------------------------------------------------------------------------------------------------------------

if(NOT RCAR_XIL STREQUAL sil)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_STANDARD 14) #required shared_lock and unique_lock for threadsafety without major performance impact
endif()

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
)

# Link lib
set(link_lib
    osal
)


#----------------------------------------------------------------------------------------------------------------------
# OS/SoC or environment specific Source/Header configuration
# Any specifics you may need add a suffix (lower letters) to the variable defined in the common section
# E.g. Specific sources for FreeRTOS:           source_freertos
# E.g. Specific sources for Linux:              source_linux
# E.g. Specific sources for HIL Env:            source_hil
# E.g. Specific sources for V3H1 SoC:           source_v3h1
# E.g. Specific sources for FreeRTOS and V3H1:  source_freertos_v3h1
#----------------------------------------------------------------------------------------------------------------------

# SIL specific
if("${RCAR_XIL}" STREQUAL "sil")
    # Threads configs
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)
    set_property(GLOBAL APPEND PROPERTY RCAR_PACKAGE_DEPENDENCY "Threads")
    if(CMAKE_USE_PHTREADS OR CMAKE_USE_PTHREADS_INIT)
        list(APPEND private_compile_def_sil
            RCAR_XOS_HAVE_PTHREAD
        )
    else()
        message(WARNING "OSAL Wrapper for Simulation requires PTHREAD to enable all OSAL Thread/Task Priorities. "
            "Priorities will be ignored by the OSAL API implementation")
    endif()

    # override static compilation to move for DLL to get rid of the need to have generic_sim API exposed to customers
    set(module_linkage      SHARED)
endif()

set(source_sil
    # Osal Managers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_osal_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_osal_interrupt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_osal_io.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_osal_power.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_osal_message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_osal_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_osal_threadsync.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_osal_timeclock.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_xos_osal_memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/r_xos_osal_memory_generic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/memory_access_router.cpp
    # SoC abstraction
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/soc_abstraction/generic_simulator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/soc_abstraction/soc_unit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/soc_abstraction/generic_soc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/soc_abstraction/dev_cfg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/simulation/soc_abstraction/soc_init.cpp
)

set(private_header_sil
    # SoC abstraction
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/dev_cfg.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/generic_simulator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/irq_interface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/peripheral_interface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/register_interface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/soc_unit.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/tlb.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/generic_soc.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/generic_soc.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/soc_abstraction/soc_init.hpp
    #osal_wrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/impc_memory_access_router.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/sysram_memory_access_router.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/memory_access_router.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/message_queue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/resource_managers/device_manager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/resource_managers/cond_var_manager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/resource_managers/mq_manager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/resource_managers/mutex_manager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/resource_managers/thread_manager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/r_xos_osal_memory_interface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/simulation/r_osal_common.hpp
)

set(private_link_lib_sil
    generic_sim_handler
    vmsav8
    utils
    Threads::Threads
    memory_allocator
)

option(RCAR_OSAL_TRACE_MEMORY_ACCESS "For Simulation memory access can written to a trace file." FALSE)
if(RCAR_OSAL_TRACE_MEMORY_ACCESS)
   set(private_compile_def_sil RCAR_OSAL_TRACE_REGWRITES)
endif()

string(REGEX MATCHALL "^([0-9]+).([0-9]+).([0-9]+)(.*)?" _tmp "${module_version}")
if(CMAKE_MATCH_COUNT GREATER 2)
    list(APPEND private_compile_def_sil
        _OSAL_WRAPPER_VERSION_MAJOR=${CMAKE_MATCH_1}
        _OSAL_WRAPPER_VERSION_MINOR=${CMAKE_MATCH_2}
        _OSAL_WRAPPER_VERSION_PATCH=${CMAKE_MATCH_3}
        )
else()
    message(FATAL_ERROR "Invalid Module Version format. Please at least specify Major.Minor.Patch")
endif()

# Linux specific
set(source_linux_hil
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_threadsync.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_version.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/r_osal_res_cfg_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/dynamic/r_osal_res_cfg_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/dynamic/r_osal_res_cfg_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/dynamic/r_osal_res_cfg_threadsync.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/dynamic/r_osal_res_cfg_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/dynamic/r_osal_dev_cfg_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/dynamic/r_osal_dev_cfg_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/dynamic/r_osal_dev_cfg_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_cond.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_mutex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_cond.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_memorybarrier.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_mutex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/linux/r_osal_os_timeclock.c
)

set(private_header_linux_hil
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/linux/r_osal_os_types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/dynamic/r_osal_res_cfg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/dynamic/r_osal_res_cfg_share.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/dynamic/r_osal_dev_cfg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/dynamic/r_osal_dev_cfg_share.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/r_osal_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/r_osal_manager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/r_osal_res_cfg_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/r_osal_res_cfg_memory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/r_osal_dev_cfg_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/helper/r_osal_helper_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/os_if/r_osal_os_if.h
)

set(link_lib_linux_hil
    rt
    pthread
    #mmngr
    memory_allocator
)

set(external_private_include_dir_linux
    ${CMAKE_SYSROOT}/usr/local/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/linux
)

set(source_qnx
    # pma
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/r_pma_if.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/manager/r_pma_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/statemachine/r_pma_statemachine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/r_pma_hwdepend.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3h1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3h2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3m.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v4h.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_s4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v4m.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/qnx/r_pma_bsp_wrapper.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v3h1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v3h2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v3m.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v4h.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_s4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/qnx/tbl/r_pma_bsp_wrapper_tbl_v4m.c

    # OSAL Manager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_threadsync.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_version.c

    # OSAL Device Configurator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/static/r_osal_dev_cfg_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/static/r_osal_dev_cfg_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/static/r_osal_dev_cfg_interrupt.c

    # OSAL Resource Configurator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/r_osal_res_cfg_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/static/r_osal_res_cfg_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/static/r_osal_res_cfg_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/static/r_osal_res_cfg_threadsync.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/static/r_osal_res_cfg_timeclock.c

    # OSAL Helper IF
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_cond.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_mutex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_message.c

    # OSAL OS IF
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_cond.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_memorybarrier.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_mutex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/qnx/r_osal_os_message.c
)


# Public Header that are exported and can be used by application or other modules
set(public_header_qnx
    # None
)

set(private_header_qnx
    # pma
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/r_pma.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/r_pma_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/manager/r_pma_manager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/statemachine/r_pma_statemachine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/hw/r_pma_hwdepend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/hw/r_pma_hwa_id.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/hw/r_pma_domain_id.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h

    # OSAL Manager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/r_osal_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/r_osal_manager.h

    # OSAL Device Configurator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/r_osal_dev_cfg_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/static/r_osal_dev_cfg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/static/r_osal_dev_cfg_share.h

    # OSAL Resource Configurator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/r_osal_res_cfg_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/r_osal_res_cfg_memory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/static/r_osal_res_cfg.h

    # OSAL Helper IF
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/helper/r_osal_helper_if.h

    # OSAL OS IF
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/os_if/r_osal_os_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/qnx/r_osal_os_type.h
)


if (RCAR_TARGET_OS STREQUAL "zephyr")
    add_definitions(-DCONFIG_ADDR_STACK)
endif()

# Zephyr specific
set(source_zephyr
    # pma
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/r_pma_if.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/manager/r_pma_manager.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/statemachine/r_pma_statemachine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/r_pma_hwdepend.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/zephyr/r_pma_hw_tbl_v4h.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3h1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3h2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v3m.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_s4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/hw/tbl/r_pma_hw_tbl_v4m.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/zephyr/r_pma_bsp_wrapper.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/pma/bsp_wrapper/zephyr/tbl/r_pma_bsp_wrapper_tbl_v4h.c
    
    # OSAL Manager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_threadsync.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/r_osal_version.c

    # OSAL Device Configurator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/static/r_osal_dev_cfg_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/static/r_osal_dev_cfg_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/dev_cfg/static/r_osal_dev_cfg_interrupt.c

    # OSAL Resource Configurator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/r_osal_res_cfg_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/static/r_osal_res_cfg_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/static/r_osal_res_cfg_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/static/r_osal_res_cfg_threadsync.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/res_cfg/static/r_osal_res_cfg_timeclock.c
    
    # OSAL Helper IF
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_cond.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_mutex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/common/helper/r_osal_hif_message.c

    # OSAL OS IF
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_cond.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_memorybarrier.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_mutex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/zephyr/r_osal_os_message.c
)

set(private_header_zephyr
    # pma
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/r_pma.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/r_pma_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/manager/r_pma_manager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/statemachine/r_pma_statemachine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/hw/r_pma_hwdepend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/hw/r_pma_hwa_id.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/hw/r_pma_domain_id.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h

    # OSAL Manager
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/r_osal_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/r_osal_manager.h

    # OSAL Device Configurator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/r_osal_dev_cfg_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/static/r_osal_dev_cfg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/dev_cfg/static/r_osal_dev_cfg_share.h

    # OSAL Resource Configurator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/r_osal_res_cfg_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/r_osal_res_cfg_memory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/res_cfg/static/r_osal_res_cfg.h

    # OSAL Helper IF
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/helper/r_osal_helper_if.h

    # OSAL OS IF
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/common/os_if/r_osal_os_if.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/zephyr/r_osal_os_type.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/zephyr/r_osal_os_dev_cfg_tbl_v4h.h
)

set(link_lib_zephyr
    memory_allocator
)

set(include_dir_zephyr
    ${ZEPHYR_BASE}/kernel/include
    ${ZEPHYR_BASE}/include/zephyr/sys
    ${ZEPHYR_BASE}/include/zephyr
    ${ARCH_DIR}/${ARCH}/include
)


set(link_lib_qnx
    rcar-cpgS
    rcar-syscS
    memory_allocator
)

set(compile_def_qnx
    _QNX_SOURCE
    _LARGEFILE64_SOURCE
)

# eMCOS specific

if (RCAR_TARGET_OS STREQUAL "emcos")
    add_definitions(-DEMCOS_GIC_WA_ENABLE)
endif ()

set(source_emcos
    # pma
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/pma/r_pma_dummy_api.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/pma/r_pma_dummy_power.c

    # dev_cfg
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/dev_cfg/r_osal_dev_cfg.c

    # usr_cfg
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/usr_cfg/r_osal_usr_cfg.c

    # wrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_io.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_message.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_power.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_thread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_threadsync.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_memory.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_timeclock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/emcos/r_osal_version.c
)

set(private_header_emcos
    # pma
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/emcos/pma/r_pma_hwdepend.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/emcos/pma/r_pma_common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/emcos/pma/r_pma.h

    # dev_cfg
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/emcos/dev_cfg/r_osal_dev_cfg_control.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/emcos/dev_cfg/r_osal_dev_cfg.h

    # usr_cfg
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/emcos/usr_cfg/r_osal_usr_cfg_control.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/emcos/usr_cfg/r_osal_usr_cfg.h

    # wrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/target/emcos/r_osal_common.h
)

set(link_lib_emcos
    emcos
    memory_allocator
)

# Release Note Settings
set(release_note "YES")
if(RCAR_XIL STREQUAL hil)
    if(RCAR_TARGET_OS STREQUAL "linux")
       set(release_note_json "${CMAKE_CURRENT_SOURCE_DIR}/doc/release_note/linux/release_note.json")
       set(release_note_file_name "R-CarV4H_V4M_S4_V3H_V3M_Linux_OSALWrapper_ReleaseNote")
    elseif(RCAR_TARGET_OS STREQUAL "qnx")
       set(release_note_json "${CMAKE_CURRENT_SOURCE_DIR}/doc/release_note/qnx/release_note.json")
       set(release_note_file_name "R-CarV4H_V4M_S4_QNX_OSALWrapper_ReleaseNote")
    elseif(RCAR_TARGET_OS STREQUAL "emcos")
       set(release_note_json "${CMAKE_CURRENT_SOURCE_DIR}/doc/release_note/emcos/release_note.json")
       set(release_note_file_name "OSAL_Wrapper_eMCOS_ReleaseNote.pdf")
    elseif(RCAR_TARGET_OS STREQUAL "zephyr")
        set(release_note_json "${CMAKE_CURRENT_SOURCE_DIR}/doc/release_note/zephyr/release_note.json")
        set(release_note_file_name "R-CarV4H_Zephyr_OSALWrapper_ReleaseNote")
    else()
       set(release_note "NO")
    endif()
else()
   set(release_note_json "${CMAKE_CURRENT_SOURCE_DIR}/doc/release_note/simulator/release_note.json")
   set(release_note_file_name "R-CarV4H_V4M_V3H_V3M_Simulator_OSALWrapper_ReleaseNote")
endif()

if(RCAR_XIL STREQUAL sil)
    set(private_compile_def_linux "-D_POSIX_C_SOURCE=200809L")
else()
    set(compile_def_linux _POSIX_C_SOURCE=200809L)
endif()

set(external_private_include_dir_linux ${CMAKE_CURRENT_SOURCE_DIR}/../osal_configuration/include)
set(external_private_include_dir_qnx ${CMAKE_CURRENT_SOURCE_DIR}/../osal_configuration/include)
set(external_private_include_dir_emcos ${CMAKE_CURRENT_SOURCE_DIR}/../osal_configuration/include)
set(external_private_include_dir_zephyr ${CMAKE_CURRENT_SOURCE_DIR}/../osal_configuration/include)

#----------------------------------------------------------------------------------------------------------------------
# Test settings
#----------------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------------------------------------
# Artifactory document setting
#----------------------------------------------------------------------------------------------------------------------
if("${RCAR_XIL}" STREQUAL "hil")
    if("${RCAR_TARGET_OS}" STREQUAL "linux")
        if (RCAR_SOC MATCHES "s4") # Target Gateway
            set(document_remote_path "rcar-docs/xos3/dev-xos3/osal_wrapper/3.14.0.gateway")
            set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal_wrapper")
            set(document_files
                user_manual/linux/r11uz0126ej0312-osalwrapper.pdf
            )
        elseif (RCAR_SOC MATCHES "(v3|v4h)")
            set(document_remote_path "rcar-docs/xos3/dev-xos3/osal_wrapper/3.20.0.adas")
            set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal_wrapper")
            set(document_files
                user_manual/linux/r11uz0126ej0315-osalwrapper.pdf
            )
        elseif (RCAR_SOC MATCHES "(v4m)")
            set(document_remote_path "rcar-docs/xos3/dev-xos3/osal_wrapper/3.23.0.adas")
            set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal_wrapper")
            set(document_files
                user_manual/linux/r11uz0126ej0317-osalwrapper.pdf
            )
        endif()
    elseif ("${RCAR_TARGET_OS}" STREQUAL "qnx")
        if (RCAR_SOC MATCHES "s4") # Target Gateway
            set(document_remote_path "rcar-docs/xos3/dev-xos3/osal_wrapper/3.18.0.gateway")
            set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal_wrapper")
            set(document_files
                user_manual/qnx/r11uz0127ej0216-osalwrapper.pdf
            )
        elseif (RCAR_SOC MATCHES "(v3|v4h)")
            set(document_remote_path "rcar-docs/xos3/dev-xos3/osal_wrapper/3.20.0.adas")
            set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal_wrapper")
            set(document_files
                user_manual/qnx/r11uz0127ej0219-osalwrapper.pdf
            )
        elseif (RCAR_SOC MATCHES "(v4m)")
            set(document_remote_path "rcar-docs/xos3/dev-xos3/osal_wrapper/3.23.0.adas")
            set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal_wrapper")
            set(document_files
                user_manual/qnx/r11uz0127ej0222-osalwrapper.pdf
            )
        endif()
    elseif ("${RCAR_TARGET_OS}" STREQUAL "emcos")
        set(document_remote_path "rcar-docs/xos3/dev-xos3/osal_wrapper/3.3.0.rcN")
        set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal_wrapper")
        set(document_files
            user_manual/emcos/OSAL_eMCOS_Users_Manual.pdf
        )
    elseif ("${RCAR_TARGET_OS}" STREQUAL "zephyr")
        set(document_remote_path "rcar-docs/xos3/dev-xos3/osal_wrapper/3.15.0.adas")
        set(document_local_path  "${RCAR_PACKAGE_DOC_DIR}/sw/osal/osal_wrapper")
        set(document_files
            user_manual/zephyr/r11uz0129ej0090-osalwrapper.pdf
        )
    endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# Artifactory document setting (DIR)
#----------------------------------------------------------------------------------------------------------------------
if("${RCAR_TARGET_OS}" STREQUAL "qnx")
    if (RCAR_SOC MATCHES "s4") # Target Gateway
        set(remote_dir_document
            "rcar-docs/rcar-xos/v3.x.x/osal_wrapper/3.18.0.gateway/sw_safety_package/R-CarV4H_S4_QNX_OSAL_SafetyApplicationNote.pdf"
            "rcar-docs/rcar-xos/v3.x.x/osal_wrapper/3.18.0.gateway/sw_safety_package/R-CarS4_QNX_OSAL_SafetyCaseSummary.pdf"
            "rcar-docs/rcar-xos/v3.x.x/osal_wrapper/3.18.0.gateway/sw_safety_package/R-CarS4_QNX_OSAL_SafetyRequirementsSpecificationSummary.pdf"
            "rcar-docs/rcar-xos/v3.x.x/osal_wrapper/3.18.0.gateway/sw_safety_package/LLWEB-20173351_FSA_report_Common_1_00E.pdf"
        )
    elseif (RCAR_SOC MATCHES "(v3|v4h)")
        set(remote_dir_document
            "rcar-docs/rcar-xos/v3.x.x/osal_wrapper/3.21.0.adas/sw_safety_package/R-CarV4H_S4_QNX_OSAL_SafetyApplicationNote.pdf"
            "rcar-docs/rcar-xos/v3.x.x/osal_wrapper/3.21.0.adas/sw_safety_package/R-CarV4H_QNX_OSAL_SafetyCaseSummary.pdf"
            "rcar-docs/rcar-xos/v3.x.x/osal_wrapper/3.21.0.adas/sw_safety_package/R-CarV4H_QNX_OSAL_SafetyRequirementsSpecificationSummary.pdf"
            "rcar-docs/rcar-xos/v3.x.x/osal_wrapper/3.21.0.adas/sw_safety_package/LLWEB-20179680_FSA_report_Common_V4H_1_00E.pdf"
        )
    endif()
endif()

#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_module()

# Delete generic_sim_handler after linking osal_wrapper shared lib
if(RCAR_PACKAGE_TYPE AND RCAR_XIL STREQUAL sil)
   add_custom_command(TARGET ${module_name} POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E rm -f $<TARGET_FILE:generic_sim_handler>
           COMMENT "Delete generic_sim_handler binary after generating osal_wrapper shared lib")
endif()

if(COMMAND rcar_artifactory_download)
    rcar_artifactory_download(DOCUMENT
        TARGET          ${module_name}
        REMOTE_PATH     ${document_remote_path}
        LOCAL_PATH      ${document_local_path}
        FILES           ${document_files}
    )
endif()
