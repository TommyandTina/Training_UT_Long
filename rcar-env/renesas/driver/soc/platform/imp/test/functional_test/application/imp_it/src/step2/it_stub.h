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
#define RCVDRV_IMP_OFFSET_ALL		(0xFFFFFFFF)

#define RCVDRV_DEINITASK_TIMEOUT		(1000)

extern volatile uint32_t g_read_reg_cnt;

/* stub functions */
void it_stub_set_impdrv_osdep_mutex_create(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_mutex_lock(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_pow_on_imp_ram(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_pow_off_imp_ram(e_impdrv_errorcode_t ret); 
void it_stub_set_impdrv_osdep_register_irq(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_unregister_irq(e_impdrv_errorcode_t ret); 
void it_stub_set_impdrv_osdep_enable_irq(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_disable_irq(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_read_reg(e_impdrv_errorcode_t ret, uint32_t index, uint32_t read_value);
void it_stub_set_impdrv_osdep_write_reg(e_impdrv_errorcode_t ret, uint32_t offset);
void it_stub_set_impdrv_osdep_pow_on_imp(e_impdrv_errorcode_t ret, st_impdrv_core_info_t *core_info);
void it_stub_set_impdrv_osdep_pow_off_imp(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_mutex_unlock(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_mutex_destroy(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_pm_set_policy(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_pm_get_policy(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_reset_core(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_memory_barrier(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_queue_send_period(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_memory_get_size(e_impdrv_errorcode_t ret, uint32_t index, size_t mem_size);
void it_stub_set_impdrv_osdep_memory_get_logical(e_impdrv_errorcode_t ret, uint32_t index, void *cpu_addr);
void it_stub_set_impdrv_osdep_memory_get_physical(e_impdrv_errorcode_t ret, uint32_t index, uint32_t phys_addr);
void it_stub_set_impdrv_osdep_memory_data_set(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_queue_create(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_queue_wait_period(e_impdrv_errorcode_t ret, uint32_t core_num);
void it_stub_set_impdrv_osdep_queue_destroy(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_memory_cache_clean(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_memory_compare(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_memory_cache_flush(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_reset_core_dsp(e_impdrv_errorcode_t ret);
void it_stub_set_impdrv_osdep_dev_close(e_impdrv_errorcode_t ret);

#endif /* IT_STUB_H_ */