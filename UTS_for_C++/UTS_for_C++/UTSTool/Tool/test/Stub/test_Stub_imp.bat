@echo off

REM set MODULE=imp
REM set OUT_DIR="impfw_stub"
REM call del local_function.txt
REM call del static_function.txt
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\impfw\src\r_impfw.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\impfw\src\r_impfw_func.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\impfw\src\r_impfw_func_dev.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\impfw\src\r_impfw_func_drv.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\impfw\src\r_impfw_func_rtt.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM copy NUL static_function.txt
REM set MODULE_DIR=
REM set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\imp-osal\V3H_FreeRTOS_Integration\RTOS\OSAL"
REM set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp"
REM set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt"
REM set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\impfw"
REM call python ./src/stub_FindPrototype.py -i %MODULE_DIR%
REM call python ./src/stub_StubAndHook.py -o %OUT_DIR% -module %MODULE%

REM set MODULE=imp
REM set OUT_DIR="imp_stub"
REM call del local_function.txt
REM call del static_function.txt
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp\src\rcv_impdrv.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp\src\rcv_impdrv_chkglobal.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp\src\rcv_impdrv_hwdep.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp\src\rcv_impdrvosal.c"
REM call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
REM copy NUL static_function.txt
REM set MODULE_DIR=
REM set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\imp-osal\V3H_FreeRTOS_Integration\RTOS\OSAL"
REM set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp"
REM set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt"
REM call python ./src/stub_FindPrototype.py -i %MODULE_DIR%
REM call python ./src/stub_StubAndHook.py -o %OUT_DIR% -module %MODULE%

set MODULE=imp
set OUT_DIR="imp_rtt_stub"
call del local_function.txt
call del static_function.txt
set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt\src\rcv_imprtt.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt\src\rcv_imprtt_chkglobal.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt\src\rcv_imprtt_hwdep.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt\src\rcv_imprtt_osaldep.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
set SOURCE_PATH="D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt_user\src\rcv_imprtt_userfunc.c"
call python ./src/stub_LocalFuncToStub.py -i %SOURCE_PATH% -o %OUT_DIR% -static False -module %MODULE%
copy NUL static_function.txt
set MODULE_DIR=
set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\imp-osal\V3H_FreeRTOS_Integration\RTOS\OSAL"
set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp"
set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt"
set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\imp_rtt_user"
set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\a050b\ut_framework\target\impfw"
set MODULE_DIR=%MODULE_DIR%"D:\05_IMP_RTOS\00_repos\imp-osal\V3H_FreeRTOS_Integration\RTOS\BSP\cpu_rtt"
call python ./src/stub_FindPrototype.py -i %MODULE_DIR%
call python ./src/stub_StubAndHook.py -o %OUT_DIR% -module %MODULE%

