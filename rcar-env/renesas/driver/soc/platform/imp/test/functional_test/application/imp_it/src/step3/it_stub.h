#ifndef IT_STUB_H_
#define IT_STUB_H_

#include "rcar-xos/osal/r_osal.h"
#include "r_impdrv.h"
#include "r_impdrv_private.h"
#include "r_impdrv_osdep.h"
#include "r_impdrv_corectl.h"
#include "r_impdrv_genctl.h"
#include "r_impdrv_cmnctl.h"

/* offset for register access stub func */

#define STAADDR_FCPR 0x80401000

/* For DSP register check */
#define WRITE_REG_TIME                      (100U)
#define WRITE_REG_IDX_MSS_DMBA              (0U)
#define WRITE_REG_IDX_P_ADD0_ATT0           (1U)
#define WRITE_REG_IDX_P_ADD2_ATT0           (2U)
#define WRITE_REG_IDX_P_ADD3_START          (3U)
#define WRITE_REG_IDX_P_ADD3_ATT0           (4U)
#define WRITE_REG_IDX_P_ADD4_START          (5U)
#define WRITE_REG_IDX_D_DDCL                (6U)
#define WRITE_REG_IDX_VDSP_ICU_ACU_LOCK     (7U)
#define WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_2ND (8U)
#define WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_3RD (9U)
#define WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_4TH (10U)
#define WRITE_REG_IDX_D_SFT_SCPD_0          (11U)
#define WRITE_REG_IDX_D_SFT_SCPD_0_2ND      (12U)
#define WRITE_REG_IDX_D_SFT_SCPD_1          (13U)
#define WRITE_REG_IDX_D_SFT_SCPD_2          (14U)
#define WRITE_REG_IDX_D_SFT_SCPD_3          (15U)
#define WRITE_REG_IDX_VDSP_OUT_INT_CLR      (16U)

/* stub functions */
void it_stub_set_R_OSAL_InterruptRegisterIsr(e_osal_return_t ret);
void it_stub_set_R_OSAL_InterruptEnableIsr(e_osal_return_t ret);
void it_stub_set_R_OSAL_InterruptDisableIsr(e_osal_return_t ret);
void it_stub_set_R_OSAL_InterruptUnregisterIsr(e_osal_return_t ret);
void it_stub_set_R_OSAL_PmSetRequiredState(e_osal_return_t ret, const char *const type, e_osal_pm_required_state_t pm_required_state);
void it_stub_set_R_OSAL_PmGetRequiredState(e_osal_return_t ret, e_osal_pm_required_state_t pm_required_state);
void it_stub_set_R_OSAL_PmSetPolicy(e_osal_return_t ret);
void it_stub_set_R_OSAL_PmGetState(e_osal_return_t ret);
void it_stub_set_R_OSAL_PmGetPolicy(e_osal_return_t ret);
void it_stub_set_R_OSAL_PmSetResetState(e_osal_return_t ret);
void it_stub_set_R_OSAL_PmGetResetState(e_osal_return_t ret);
void it_stub_set_R_OSAL_PmWaitForState(e_osal_return_t ret);
void it_stub_set_R_OSAL_ThsyncMutexCreate(e_osal_return_t ret);
void it_stub_set_R_OSAL_ThsyncMutexDestroy(e_osal_return_t ret);
void it_stub_set_R_OSAL_ThsyncMutexLockForTimePeriod(e_osal_return_t ret);
void it_stub_set_R_OSAL_ThsyncMutexUnlock(e_osal_return_t ret);
void it_stub_set_R_OSAL_IoDeviceOpen(e_osal_return_t ret, const char *const type);
void it_stub_set_R_OSAL_IoDeviceClose(e_osal_return_t ret, const char *const type);
void it_stub_set_R_OSAL_IoRead32(e_osal_return_t ret, uint32_t index, uint32_t read_value);
void it_stub_set_R_OSAL_IoWrite32(e_osal_return_t ret, uint32_t offset, uint32_t read_back);
void it_stub_R_OSAL_ThsyncMemoryBarrier(e_osal_return_t ret);
void it_stub_set_R_OSAL_MmngrGetSize(e_osal_return_t ret, uint32_t index, size_t mem_size);
void it_stub_set_R_OSAL_MmngrGetCpuPtr(e_osal_return_t ret, uint32_t index, void *cpu_addr);
void it_stub_set_R_OSAL_MmngrGetHwAddr(e_osal_return_t ret, uint32_t index, uint32_t phys_addr);
void it_stub_set_R_OSAL_MmngrFlush(e_osal_return_t ret);
void it_stub_set_R_OSAL_MmngrInvalidate(e_osal_return_t ret);
void it_stub_set_R_OSAL_MqCreate(e_osal_return_t ret);
void it_stub_set_R_OSAL_MqReset(e_osal_return_t ret);
void it_stub_set_R_OSAL_MqDelete(e_osal_return_t ret);
void it_stub_set_R_OSAL_MqReceiveForTimePeriod(e_osal_return_t ret, uint32_t core_num);
void it_stub_set_R_OSAL_MqSendForTimePeriod(e_osal_return_t ret);
void it_stub_set_impdrv_cmnctl_reg_set_checksum(e_impdrv_errorcode_t ret);

extern volatile uint32_t g_write_value[WRITE_REG_TIME];
extern volatile bool init_test_flag;
extern volatile bool start_test_flag;
extern volatile bool stop_test_flag;
extern volatile bool quit_test_flag;
extern volatile bool dsp_register_cbfunc_flag[4];
extern volatile bool dsp_unregister_cbfunc_flag[4];
extern volatile bool it_stub_check_version_reg_val_flag;
extern volatile bool update_read_back_flag;
extern osal_device_handle_t g_osal_device_handle[4];
extern void (*g_int_handler[4])(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
);

#endif /* IT_STUB_H_ */