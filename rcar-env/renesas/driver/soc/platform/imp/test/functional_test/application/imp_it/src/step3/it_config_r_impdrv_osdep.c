#define R_OSAL_InterruptRegisterIsr				stub_R_OSAL_InterruptRegisterIsr
#define R_OSAL_InterruptEnableIsr				stub_R_OSAL_InterruptEnableIsr
#define R_OSAL_InterruptDisableIsr				stub_R_OSAL_InterruptDisableIsr
#define R_OSAL_InterruptUnregisterIsr			stub_R_OSAL_InterruptUnregisterIsr
#define R_OSAL_PmSetPolicy						stub_R_OSAL_PmSetPolicy
#define R_OSAL_PmGetState						stub_R_OSAL_PmGetState
#define R_OSAL_PmGetPolicy						stub_R_OSAL_PmGetPolicy
#define R_OSAL_PmSetResetState					stub_R_OSAL_PmSetResetState
#define R_OSAL_PmGetResetState					stub_R_OSAL_PmGetResetState
#define R_OSAL_PmWaitForState					stub_R_OSAL_PmWaitForState
#define R_OSAL_PmSetRequiredState				stub_R_OSAL_PmSetRequiredState
#define R_OSAL_PmGetRequiredState               stub_R_OSAL_PmGetRequiredState
#define R_OSAL_IoWrite32						stub_R_OSAL_IoWrite32
#define R_OSAL_IoRead32							stub_R_OSAL_IoRead32
#define R_OSAL_ThsyncMutexCreate				stub_R_OSAL_ThsyncMutexCreate
#define R_OSAL_ThsyncMutexDestroy				stub_R_OSAL_ThsyncMutexDestroy
#define R_OSAL_ThsyncMutexLockForTimePeriod		stub_R_OSAL_ThsyncMutexLockForTimePeriod
#define R_OSAL_ThsyncMutexUnlock				stub_R_OSAL_ThsyncMutexUnlock
#define R_OSAL_IoDeviceOpen						stub_R_OSAL_IoDeviceOpen
#define R_OSAL_IoDeviceClose					stub_R_OSAL_IoDeviceClose
#define R_OSAL_ThsyncMemoryBarrier              stub_R_OSAL_ThsyncMemoryBarrier
#define R_OSAL_MmngrGetSize                     stub_R_OSAL_MmngrGetSize
#define R_OSAL_MmngrGetCpuPtr                   stub_R_OSAL_MmngrGetCpuPtr
#define R_OSAL_MmngrGetHwAddr                   stub_R_OSAL_MmngrGetHwAddr
#define R_OSAL_MmngrFlush                       stub_R_OSAL_MmngrFlush
#define R_OSAL_MmngrInvalidate                  stub_R_OSAL_MmngrInvalidate
#define R_OSAL_MqCreate                         stub_R_OSAL_MqCreate
#define R_OSAL_MqReset                          stub_R_OSAL_MqReset
#define R_OSAL_MqDelete                         stub_R_OSAL_MqDelete
#define R_OSAL_MqReceiveForTimePeriod           stub_R_OSAL_MqReceiveForTimePeriod
#define R_OSAL_MqSendForTimePeriod              stub_R_OSAL_MqSendForTimePeriod

#include "r_impdrv_osdep.c"
#include "r_impdrv_osdep_mmngr.c"

#undef R_OSAL_InterruptRegisterIsr
#undef R_OSAL_InterruptEnableIsr
#undef R_OSAL_InterruptDisableIsr
#undef R_OSAL_InterruptUnregisterIsr
#undef R_OSAL_PmSetPolicy
#undef R_OSAL_PmGetState
#undef R_OSAL_PmGetPolicy
#undef R_OSAL_PmSetResetState
#undef R_OSAL_PmGetResetState
#undef R_OSAL_PmWaitForState
#undef R_OSAL_PmSetRequiredState
#undef R_OSAL_IoWrite32
#undef R_OSAL_IoRead32
#undef R_OSAL_ThsyncMutexCreate
#undef R_OSAL_ThsyncMutexDestroy
#undef R_OSAL_ThsyncMutexLockForTimePeriod
#undef R_OSAL_ThsyncMutexUnlock
#undef R_OSAL_IoDeviceOpen
#undef R_OSAL_IoDeviceClose
#undef R_OSAL_ThsyncMemoryBarrier
#undef R_OSAL_MmngrGetSize
#undef R_OSAL_MmngrGetCpuPtr
#undef R_OSAL_MmngrGetHwAddr
#undef R_OSAL_MmngrFlush
#undef R_OSAL_MmngrInvalidate
#undef R_OSAL_MqCreate
#undef R_OSAL_MqReset
#undef R_OSAL_MqDelete
#undef R_OSAL_MqReceiveForTimePeriod
#undef R_OSAL_MqSendForTimePeriod
