"""
***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************
***********************************************************************************************************************
* File Name    : osal_wrapper_ut.py(OSAL Zephyr)
* Version      : 1.0.0
* Product Name : RCAR environment project CI/CD
* Device(s)    : N/A
* Description  : Scripts to run WinAMS static analysis for source code
***********************************************************************************************************************
***********************************************************************************************************************
* History   Version   DD.MM.YYYY   Description
*           1.0.0     24.11.2021   Initial revision.
***********************************************************************************************************************
"""

#######################################################################################################################
# Import from standard libraries
#######################################################################################################################
import os
import subprocess
import argparse

#######################################################################################################################
# GetCommandlineArguments
#######################################################################################################################
def GetCommandlineArguments():
    parser = argparse.ArgumentParser(description='WinAMS analysis check')
    parser.add_argument('-bp', '--base_path', required=False,
                        help='Unit Test Python Script path')
    parser.add_argument('-pp', '--project_path', required=False,
                        help='Unit Test Project Path')

    args = parser.parse_args()
    return args

class ut_param:
    ut_script_path = ".ci/unit_test_winams/winams_script.py"
    cp2_path       = "cp2/osal_ut_cp2.vproj"
    winams_path    = "sst/osal_ut_sstm/osal_ut_sstm.amsy"
    object_path    = "osal_ut/osal_ut_mcdc"
    result_path    = "sst/osal_ut_sstm/Result"
    c0_threshold   = 99 # C0 Coverage Threshold
    c1_threshold   = 99 # C1 Coverage Threshold
    mcdc_threshold = 99 # MC/DC Coverage Threshold
    s0s1_threshold = 99 # S0/S1 Coverage Threshold

if __name__ == "__main__":
    ret = 0
    args = GetCommandlineArguments()
    base_path = args.base_path
    project_path = args.project_path

    # Unit Test Script Path
    winamsCommand = "python3"
    commands = [winamsCommand]
    commands.append("-X")
    commands.append("utf8")

    commands.append("{}/{}".format(base_path, ut_param.ut_script_path))

    # Make File path
    commands.append("-mp")
    commands.append("{}".format(project_path))

    # CasePlayer2 Project Path
    commands.append("-cv")
    commands.append("{}/{}".format(project_path, ut_param.cp2_path))

    # WinAMS(SSTManager) Project Path
    commands.append("-wp")
    commands.append("{}/{}".format(project_path, ut_param.winams_path))

    # Build Object Path
    commands.append("-ob")
    commands.append("{}/{}".format(project_path, ut_param.object_path))

    # Result Checker Setting 
    commands.append("-rc")
    commands.append("csv")
    commands.append("{}/{}".format(project_path, ut_param.result_path))
    commands.append("{}".format(ut_param.c0_threshold))   # C0 Coverage Threshold
    commands.append("{}".format(ut_param.c1_threshold))   # C1 Coverage Threshold
    commands.append("{}".format(ut_param.mcdc_threshold)) # MC/DC Coverage Threshold
    commands.append("{}".format(ut_param.s0s1_threshold)) # S0/S1 Coverage Threshold

    output = subprocess.run(commands)

    exit(output.returncode)
