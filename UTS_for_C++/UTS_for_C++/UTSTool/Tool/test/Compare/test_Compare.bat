@echo off

REM set LOG_PATH="C:\Users\quangla\repo\a050b\Tool\test\Compare\log_test_imr.txt"
REM set SPEC_PATH="C:\Users\quangla\repo\a050\trunk\03_PCL\00_IMR\IMR_FRT_V3H_B_V1_unit_test_specification_imr_osal.xlsx"
REM set GIT_REVISION="6d46c51"

set LOG_PATH="log_sample.txt"
set SPEC_PATH="C:\Users\quangla\repo\a050\trunk\03_PCL\Template_TestSpecificationReport_sample.xlsx"
set GIT_REVISION=""

call python ../../src/Compare/compare_LogVsPCL.py -log %LOG_PATH% -spec %SPEC_PATH% -rev %GIT_REVISION%
