######################################################################################################################
# Copyright [2021-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
#Get folder path
#######################################################################################################################
#Get Current path
BASE_DIR=$(dirname "$0")
#R-Car ENV Root folder
cd ${BASE_DIR}/../../../../../../../
ROOT_DIR=${PWD}
#move base folder
cd ${BASE_DIR}
#######################################################################################################################

#######################################################################################################################
#User configuration parameters
#######################################################################################################################
#Maximum allowed number of active warnings
#   0    : (default) full compliancy
#   1..x : respective warnings shall be justfied separatelly
DIAG_COUNT_MAX=10
#(OPTIONAL) custom Analysis Configuration File (ACF)
#Note: if this parameter is not provided, the rcar platform default configuration file will be used
CUSTOM_ACF=${BASE_DIR}/../osal_wrapper_qac.acf
#(OPTIONAL) custom Compiler Configuration File (CCF)
#Note: if this parameter is not provided, the rcar platform default configuration file will be used
CUSTOM_CCF=${ROOT_DIR}/.ci/static_test_qac/config/GNU_GCC-gcc_8.3-x86_64.cct
#######################################################################################################################

#######################################################################################################################
#Do not change anything below this line.
#######################################################################################################################
#QAC static analysis script folder
STATIC_TEST_QAC_DIR=${ROOT_DIR}/.ci/static_test_qac
#QAC project output folder
QAC_OUTPUT_DIR=${ROOT_DIR}/build/static_test/osal_sample_resource
#QAC project input folder (path to SW sources in scope of the analysis)
QAC_INPUT_DIR=${ROOT_DIR}/renesas/os/osal/sample_app/osal_sample_resource

#PRQA Framework directory
QAC_DIR=$PRQA_PATH/PRQA-Framework-2.4.0

#Delete output directory if any
rmdir -p ${QAC_OUTPUT_DIR}
#Create static analysis report directory
mkdir -p ${QAC_OUTPUT_DIR}
#######################################################################################################################

#Execute static_test_qac.py file
python ${STATIC_TEST_QAC_DIR}/static_test_qac.py -i ${QAC_INPUT_DIR} -o ${QAC_OUTPUT_DIR} -qac ${QAC_DIR} -acf ${CUSTOM_ACF} -ccf ${CUSTOM_CCF} -dcmax ${DIAG_COUNT_MAX}
#python ${STATIC_TEST_QAC_DIR}/static_test_qac.py -i ${QAC_INPUT_DIR} -o ${QAC_OUTPUT_DIR} -qac ${QAC_DIR} -dcmax ${DIAG_COUNT_MAX}
