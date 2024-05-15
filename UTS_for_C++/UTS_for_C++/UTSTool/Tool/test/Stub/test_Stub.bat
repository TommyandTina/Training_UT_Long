@echo off

call del local_function.txt
call del static_function.txt
set MODULE=imr
set OUT_DIR="Output_imr"

call mkdir %OUT_DIR%
call mkdir %OUT_DIR%\include

set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_dlapi_cmn.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_dlapi_coor.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_dlapi_ctl.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_dlapi_ins.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_dlapi_ins_v3h.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_drvapi_conf.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_drvapi_conf_v3h.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_drvapi_ctl.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_drvapi_exe.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\api\r_imr_drvapi_rtt_sp.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:/repo/s920c/RTOS/BSP/imr/src/osal/r_imr_osal_ctl.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:/repo/s920c/RTOS/BSP/imr/src/osal/r_imr_osal_io.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:/repo/s920c/RTOS/BSP/imr/src/osal/r_imr_osal_msg.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:/repo/s920c/RTOS/BSP/imr/src/osal/r_imr_osal_sem.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\rttapi\r_imr_rttapi_ctl.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\rttapi\r_imr_rttapi_exe.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\rttapi\r_imr_rttapi_field_bist_ctl.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\rttosal\r_imr_rttosal_ctl.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\rttosal\r_imr_rttosal_io_ctl.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\rttosal\r_imr_rttosal_msg.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\state\r_imr_rtt_state_mng.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set SOURCE_PATH="C:\repo\s920c\RTOS\BSP\imr\src\state\r_imr_state_mng.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
set MODULE_DIR=
set MODULE_DIR=%MODULE_DIR%"C:/repo/s920c/RTOS/BSP/imr"
set MODULE_DIR=%MODULE_DIR%"C:/repo/s920c/RTOS/BSP/cpu_rtt"
set MODULE_DIR=%MODULE_DIR%"C:/repo/s920c/RTOS/OSAL"
call python ./src/stub_FindPrototype.py -i %MODULE_DIR%
call python ./src/stub_StubAndHook.py -o %OUT_DIR% -module %MODULE%

REM call del local_function.txt
REM call del static_function.txt
REM set MODULE=imp
REM set OUT_DIR="Output_imp"
REM set SOURCE_PATH="C:/Users/quangla/repo/a050b_master/ut_framework/target/imp/src/rcv_impdrv.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
REM set SOURCE_PATH="C:/Users/quangla/repo/a050b_master/ut_framework/target/imp/src/rcv_impdrv_chkglobal.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
REM set SOURCE_PATH="C:/Users/quangla/repo/a050b_master/ut_framework/target/imp/src/rcv_impdrv_hwdep.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
REM set SOURCE_PATH="C:/Users/quangla/repo/a050b_master/ut_framework/target/imp/src/rcv_impdrvosal.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static true -module %MODULE%
REM set MODULE_DIR=
REM set MODULE_DIR=%MODULE_DIR%"C:/Users/quangla/repo/a050b_master/ut_framework/target/imp"
REM set MODULE_DIR=%MODULE_DIR%"C:/repo/s920c/RTOS/OSAL"
REM call python ./src/stub_FindPrototype.py -i %MODULE_DIR%
REM call python ./src/stub_StubAndHook.py -o %OUT_DIR% -module %MODULE%
