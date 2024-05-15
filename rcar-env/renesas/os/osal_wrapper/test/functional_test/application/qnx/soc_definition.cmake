#######################################################################################################################
# Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
# Define SoC name
#----------------------------------------------------------------------------------------------------------------------
# Same definition as when soc_dependent=TRUE
if(RCAR_SOC STREQUAL "v3u")
    add_definitions(-DOSAL_SOC_V3U)
    add_definitions(-DRCAR_V3U)
elseif(RCAR_SOC STREQUAL "v3h1")
    add_definitions(-DOSAL_SOC_V3H1)
    add_definitions(-DRCAR_V3H1)
elseif(RCAR_SOC STREQUAL "v3m2")
    add_definitions(-DOSAL_SOC_V3M2)
    add_definitions(-DRCAR_V3M)
elseif(RCAR_SOC STREQUAL "v3h2")
    add_definitions(-DOSAL_SOC_V3H2)
    add_definitions(-DRCAR_V3H2)
elseif((RCAR_SOC STREQUAL "v4h") OR (RCAR_SOC STREQUAL "v4h2"))
    add_definitions(-DOSAL_SOC_V4H)
    add_definitions(-DRCAR_V4H)
elseif(RCAR_SOC STREQUAL "v4m")
    add_definitions(-DOSAL_SOC_V4M)
    add_definitions(-DRCAR_V4M)
elseif((RCAR_SOC STREQUAL "s4") OR (RCAR_SOC STREQUAL "s4n"))
    add_definitions(-DOSAL_SOC_S4)
    add_definitions(-DRCAR_S4)
endif()
