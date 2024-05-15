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

#######################################################################################################################
# This file helps to identify which OSs and SoCs are supported by the application/module.
# RCar-Environment will check this file to decide to enable this application/module for building or not?
#######################################################################################################################

#######################################################################################################################
# SUPPORTED_SOC
#
# This variable specifies which SoCs are supported
#
#   Possible values can be:
#               any
#               V3M2
#               V3H1
#               <future SoCs>
#
#   Example 1: set(SUPPORTED_SOC    any)                --> enable for all SoCs
#   Example 2: set(SUPPORTED_SOC    V3M2)               --> enable for V3M2
#   Example 3: set(SUPPORTED_SOC    V3M2 V3H1)          --> enable for V3M2 and V3H1
#
#######################################################################################################################
set(SUPPORTED_SOC   V4H)

#######################################################################################################################
# SUPPORTED_OS
#
# This variable specifies which OSs are supported
#
#   Possible values can be:
#               any
#               FREERTOS
#               LINUX
#               <future OSs>
#
#   Example 1: set(SUPPORTED_OS      any)               --> enable for all OSs
#   Example 2: set(SUPPORTED_OS      FREERTOS)          --> enable for FREERTOS
#   Example 3: set(SUPPORTED_OS      FREERTOS LINUX)    --> enable for FREERTOS and LINUX
#
#######################################################################################################################
set(SUPPORTED_OS    EMCOS)

#######################################################################################################################
# SUPPORTED_XIL
#
# This variable specifies which XIL are supported
#
#   Possible values can be:
#               any
#               HIL
#               SIL
#
#   Example 1: set(SUPPORTED_XIL      any)              --> enable for all environments
#   Example 2: set(SUPPORTED_XIL      HIL)              --> enable for HIL 
#   Example 3: set(SUPPORTED_XIL      HIL SIL)          --> enable for HIL and SIL
#
#######################################################################################################################
set(SUPPORTED_XIL   HIL)
