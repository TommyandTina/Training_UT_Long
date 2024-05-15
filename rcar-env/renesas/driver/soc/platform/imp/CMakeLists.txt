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
set(module_name        "imp")
set(module_version     "3.19.0")

# Additinal information (product number, current quality, target quality)
set(product_number      "-")             # No product number
set(current_quality     "Reference")     # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
set(target_quality      "Reference")     # Target Quality level:  Sample, Reference, QM, ASIL

set (IMPDRV_DSPCRC_DETECT "yes")
if("${IMPDRV_DSPCRC_DETECT}" STREQUAL "yes")           # use user difined crc
    add_definitions(-DIMPDRV_DSPCRC_DETECT_ADDDEF)
endif()


if("${RCAR_TARGET_OS}" STREQUAL "qnx")
    set(current_quality     "Alpha")         # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
    set(target_quality      "-")             # Target Quality level:  Sample, Reference, QM, ASIL
    if("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3m2_hil")
        #xOS2 QNX ASIL V3M2
        set(product_number      "RTM8RC7797ZRPDSD00JPQ0E")
    elseif("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3h2_hil")
        #xOS2 QNX ASIL V3H2
        set(product_number      "RTM8RC7798ZRPDSD10JPQ0E")
    elseif("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3u_hil")
        #xOS2 QNX ASIL V3U
        set(product_number      "RTM8RC779AZRPDSD00JPQ0E")
    endif()
elseif("${RCAR_TARGET_OS}" STREQUAL "windows")
    # products for simulator do not have product number for this module
elseif("${RCAR_TARGET_OS}" STREQUAL "linux")
    if("${CMAKE_C_COMPILER}" MATCHES "gcc")
        if("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3m2_hil")
            #R-Car SDK Linux Reference GCC V3M2
            set(product_number      "-")
        elseif("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3h1_hil")
            #R-Car SDK Linux Reference GCC V3H1.1
            set(product_number      "-")
        elseif("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3h2_hil")
            #R-Car SDK Linux Reference GCC V3H2
            set(product_number      "RTM8RC7798ZRPDFS10JPL2E")
        elseif("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3u_hil")
            #R-Car SDK Linux Reference GCC V3U
            set(product_number      "RTM8RC779AZRPDSS01JPL2E")
        endif()
    elseif("${CMAKE_C_COMPILER}" MATCHES "armclang")
        set(current_quality     "Alpha")         # Current Quality level: Sample, Reference, Alpha, Beta, QM, ASIL
        set(target_quality      "-")             # Target Quality level:  Sample, Reference, QM, ASIL
        if("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3m2_hil")
            #R-Car SDK Linux ARM ASIL V3M2
            set(product_number      "RTM8RC7798ZRPDSD00JPB0E")
        elseif("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3h1_hil")
            #R-Car SDK Linux ARM ASIL V3H1.1
            set(product_number      "RTM8RC7797ZRPDSD00JPB0E")
        elseif("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3h2_hil")
            #R-Car SDK Linux ARM ASIL V3H2
            set(product_number      "RTM8RC7798ZRPDSD10JPB0E")
        elseif("${RCAR_SOC}_${RCAR_XIL}" STREQUAL "v3u_hil")
            #R-Car SDK Linux ARM ASIL V3U
            set(product_number      "RTM8RC779AZRPDSD00JPB0E")
        endif()
    endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------

# SoC dependent or not (TRUE or FALSE)
set(soc_dependent       TRUE)

# Sets the modules linkage type. Allowed values are [<empty>, STATIC, INTERFACE, SHARED, MODULE]
set(module_linkage      STATIC)

#----------------------------------------------------------------------------------------------------------------------
# Common Source/Header configuration
#----------------------------------------------------------------------------------------------------------------------

# Source files
if(RCAR_XIL STREQUAL sil)
    set(source
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_api.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_genctl_sil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_sil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr_sil.c
    )
else()
    set(source
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_api.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_genctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr.c
    )
endif()

if(RCAR_XIL STREQUAL sil)
#simulator do not support of DSP control for V4H
#also, change the VCR check of the IMP core for simulator.
	set(source_v4h
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_cmnctl_sil.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_cmnctl_reg.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_impctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_cnnctl_sil.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_dmactl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_dmasctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_ocvctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_pscctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_dspctl_sil.c
	)
	set(source_v4h2
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_cmnctl_sil.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_cmnctl_reg.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_impctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_cnnctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_dmactl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_dmasctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_ocvctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_pscctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_dspctl_sil.c
	)
    set(source_v4m
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_cmnctl_sil.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_cmnctl_reg_sil.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_impctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_cnnctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_dmactl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_dmasctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_ocvctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_pscctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_dspctl_sil.c
	)
else()
	set(source_v4h
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_cmnctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_cmnctl_reg.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_impctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_cnnctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_dmactl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_dmasctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_ocvctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_pscctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h/r_impdrv_dspctl.c
	)
	set(source_v4h2
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_cmnctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_cmnctl_reg.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_impctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_cnnctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_dmactl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_dmasctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_ocvctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_pscctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_dspctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4h2/r_impdrv_udefctl.c
	)
	set(source_v4m
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_cmnctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_cmnctl_reg.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_impctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_cnnctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_dmactl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_dmasctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_ocvctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_pscctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_dspctl.c
	    ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v4m/r_impdrv_udefctl.c
	)
endif()

if(RCAR_XIL STREQUAL sil)
#simulator do not support initialization of scratchpad memory for V3U
	set(source_v3u
		${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cmnctl_sil.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cmnctl_reg.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_impctl.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cnnctl.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_dmactl.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_ocvctl.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_pscctl.c
	)
else()
	if (RCAR_TARGET_OS STREQUAL "linux")
#linux OS may not support all devices which need to initialize SPM.
		set(source_v3u
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cmnctl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cmnctl_reg.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_impctl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cnnctl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_dmactl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_ocvctl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_pscctl.c
		)
	else()
		set(source_v3u
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cmnctl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cmnctl_reg.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_impctl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_cnnctl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_dmactl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_ocvctl.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3u/r_impdrv_pscctl.c
		)
	endif()
endif()

if(RCAR_XIL STREQUAL sil)
#Use simulator specific implementation for IMP core and slim IMP core.
    set(source_v3h2
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr_sil.c

        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_cmnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_cmnctl_reg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_impctl_sil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_impsctl_sil.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_cnnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_dmactl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_dmasctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_ocvctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_pscctl.c
    )
else()
    set(source_v3h2
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr.c

        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_cmnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_cmnctl_reg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_impctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_impsctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_cnnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_dmactl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_dmasctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_ocvctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h2/r_impdrv_pscctl.c
    )
endif()

if(RCAR_XIL STREQUAL sil)
    set(source_v3h1
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr_sil.c

        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_cmnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_cmnctl_reg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_impctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_impsctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_cnnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_dmactl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_ocvctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_pscctl.c
    )
else()
    set(source_v3h1
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr.c

        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_cmnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_cmnctl_reg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_impctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_impsctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_cnnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_dmactl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_ocvctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3h1/r_impdrv_pscctl.c
    )
endif()

if(RCAR_XIL STREQUAL sil)
    set(source_v3m2
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr_sil.c

        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_cmnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_cmnctl_reg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_impctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_cnnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_dmactl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_ocvctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_pscctl.c
    )
else()
    set(source_v3m2
        ${CMAKE_CURRENT_SOURCE_DIR}/src/r_impdrv_osdep_mmngr.c

        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_cmnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_cmnctl_reg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_impctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_cnnctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_dmactl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_ocvctl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/target/v3m2/r_impdrv_pscctl.c
    )
endif()

# Public Header that are exported and can be used by application or other modules
set(public_header
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/imp/r_impdrv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rcar-xos/imp/r_impdrv_version.h
)


# Private Header files that are not exported and cannot be used by other components or modules
set(private_header
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_corectl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_genctl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_cnnctl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_osdep.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_dmactl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_ocvctl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_pscctl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_cmnctl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_cmnctl_reg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_impctl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_private.h

    #  Header for V4H and V4M
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_udefctl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_dspctl.h

    #  Header for V4H and V3H2 and V4M
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_dmasctl.h

    #  Header for V3H1 and V3H2
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/r_impdrv_impsctl.h
)

#----------------------------------------------------------------------------------------------------------------------
# Common link dependencies
#----------------------------------------------------------------------------------------------------------------------
set(link_lib
    osal 
)

#----------------------------------------------------------------------------------------------------------------------
# Set document file list
# If not set, it will search for document in ${CMAKE_CURRENT_SOURCE_DIR}/doc by default
#----------------------------------------------------------------------------------------------------------------------
set(document
    # List of document file to install to package
)
#----------------------------------------------------------------------------------------------------------------------
# Doxygen document setting
#----------------------------------------------------------------------------------------------------------------------
set(api_doc "YES")

#----------------------------------------------------------------------------------------------------------------------
# OS/SoC or environment specific Source/Header configuration
# Any specifics you may need add a suffix (lower letters) to the variable defined in the common section
# E.g. Specific sources for FreeRTOS:   source_freertos
# E.g. Specific sources for Linux:      source_linux
# E.g. Specific sources for HIL Env:    source_hil
# E.g. Specific sources for V3H1 SoC:   source_v3h1
#----------------------------------------------------------------------------------------------------------------------


#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_module()

#Add module specific compiler flag
if ((RCAR_TARGET_OS STREQUAL "linux") AND (RCAR_XIL STREQUAL sil))
    target_compile_options(${module_name} PUBLIC -mcmodel=large)
endif()

# Shared library config
if("${RCAR_SOC}" MATCHES "v4h|v4h2|v4m" AND "${RCAR_TARGET_OS}_${RCAR_XIL}" STREQUAL "linux_sil")
    set(module_name "imp_shared")
    set(module_linkage "SHARED")
    set(link_lib 
        osal
    )
    set(api_doc OFF)
    set(remote_document OFF)
    set(release_note OFF)
    set(remote_dir_document OFF)
    rcar_configure_module()
    # Make the lib output filename is libimp_<soc>.so instead of libimp_shared_lib_<soc>.so
    if(soc_dependent)
        set_target_properties(${module_name} PROPERTIES OUTPUT_NAME "imp_${RCAR_SOC}")
    else()
        set_target_properties(${module_name} PROPERTIES OUTPUT_NAME "imp")
    endif()
endif()
