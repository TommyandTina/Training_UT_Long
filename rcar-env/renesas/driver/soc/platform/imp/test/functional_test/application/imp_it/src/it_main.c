#include "it_common.h"
#include "it_step.h"
#ifdef IMPDRV_S0S1_ENABLE
#include "s0s1_macro.h"
S0S1_EXTERN(r_impdrv_api);
S0S1_EXTERN(r_impdrv_genctl);
#if defined(STEP2) || defined(STEP3) || defined(STEP4)
#if defined(STEP3) || defined(STEP4)
	S0S1_EXTERN(r_impdrv_osdep);
	S0S1_EXTERN(r_impdrv_osdep_mmngr);
#endif
#if defined (V3H1)
	S0S1_EXTERN(target_v3h1_r_impdrv_cmnctl);
	S0S1_EXTERN(target_v3h1_r_impdrv_cmnctl_reg);
	S0S1_EXTERN(target_v3h1_r_impdrv_impctl);
	S0S1_EXTERN(target_v3h1_r_impdrv_impsctl);
	S0S1_EXTERN(target_v3h1_r_impdrv_ocvctl);
	S0S1_EXTERN(target_v3h1_r_impdrv_dmactl);
	S0S1_EXTERN(target_v3h1_r_impdrv_pscctl);
	S0S1_EXTERN(target_v3h1_r_impdrv_cnnctl);
#elif defined (V3H2)
	S0S1_EXTERN(target_v3h2_r_impdrv_cmnctl);
	S0S1_EXTERN(target_v3h2_r_impdrv_cmnctl_reg);
	S0S1_EXTERN(target_v3h2_r_impdrv_impctl);
	S0S1_EXTERN(target_v3h2_r_impdrv_impsctl);
	S0S1_EXTERN(target_v3h2_r_impdrv_impsctl);
	S0S1_EXTERN(target_v3h2_r_impdrv_ocvctl);
	S0S1_EXTERN(target_v3h2_r_impdrv_dmactl);
	S0S1_EXTERN(target_v3h2_r_impdrv_dmasctl);
	S0S1_EXTERN(target_v3h2_r_impdrv_pscctl);
	S0S1_EXTERN(target_v3h2_r_impdrv_cnnctl);
#elif defined (V3M2)
	S0S1_EXTERN(target_v3m2_r_impdrv_cmnctl);
	S0S1_EXTERN(target_v3m2_r_impdrv_cmnctl_reg);
	S0S1_EXTERN(target_v3m2_r_impdrv_impctl);
	S0S1_EXTERN(target_v3m2_r_impdrv_ocvctl);
	S0S1_EXTERN(target_v3m2_r_impdrv_dmactl);
	S0S1_EXTERN(target_v3m2_r_impdrv_pscctl);
	S0S1_EXTERN(target_v3m2_r_impdrv_cnnctl);
#elif defined (V3U)
	S0S1_EXTERN(target_v3u_r_impdrv_cmnctl);
	S0S1_EXTERN(target_v3u_r_impdrv_cmnctl_reg);
	S0S1_EXTERN(target_v3u_r_impdrv_impctl);
	S0S1_EXTERN(target_v3u_r_impdrv_ocvctl);
	S0S1_EXTERN(target_v3u_r_impdrv_dmactl);
	S0S1_EXTERN(target_v3u_r_impdrv_pscctl);
	S0S1_EXTERN(target_v3u_r_impdrv_cnnctl);
#elif defined (V4H)
	S0S1_EXTERN(target_v4h_r_impdrv_cmnctl);
	S0S1_EXTERN(target_v4h_r_impdrv_cmnctl_reg);
	S0S1_EXTERN(target_v4h_r_impdrv_impctl);
	S0S1_EXTERN(target_v4h_r_impdrv_ocvctl);
	S0S1_EXTERN(target_v4h_r_impdrv_dmactl);
	S0S1_EXTERN(target_v4h_r_impdrv_dmasctl);
	S0S1_EXTERN(target_v4h_r_impdrv_dspctl);
	S0S1_EXTERN(target_v4h_r_impdrv_pscctl);
	S0S1_EXTERN(target_v4h_r_impdrv_cnnctl);
#elif defined (V4H2)
	S0S1_EXTERN(target_v4h2_r_impdrv_cmnctl);
	S0S1_EXTERN(target_v4h2_r_impdrv_cmnctl_reg);
	S0S1_EXTERN(target_v4h2_r_impdrv_impctl);
	S0S1_EXTERN(target_v4h2_r_impdrv_ocvctl);
	S0S1_EXTERN(target_v4h2_r_impdrv_dmactl);
	S0S1_EXTERN(target_v4h2_r_impdrv_dmasctl);
	S0S1_EXTERN(target_v4h2_r_impdrv_dspctl);
	S0S1_EXTERN(target_v4h2_r_impdrv_pscctl);
	S0S1_EXTERN(target_v4h2_r_impdrv_cnnctl);
#endif
#endif
#endif

#define run_test(__func, __tparam, __tret, __test_result) \
do { \
	ret = __func(&__tparam, &__tret); \
	__test_result.passed_cnt += __tret.passed_cnt; \
	__test_result.failed_cnt += __tret.failed_cnt; \
	__test_result.skipped_cnt += __tret.skipped_cnt; \
	__tret.passed_cnt = 0; \
	__tret.failed_cnt = 0; \
	__tret.skipped_cnt = 0; \
	puts(""); \
} while(0)

int32_t hook_stop_int_ret = -1;
uint32_t hook_cbfunc_exec_ret[IMPDRV_CORE_TYPE_MAX][IMPDRV_CORE_NUM_MAX];
uint32_t hook_cbfunc_int_ret[IMPDRV_CORE_TYPE_MAX][IMPDRV_CORE_NUM_MAX];
e_impdrv_fatalcode_t  fatal_code;
e_impdrv_errorcode_t  fatal_ercd;

/* test function */
osal_memory_manager_handle_t	osal_mmngr   = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
static st_impdrv_ctl_t impdrv_ctl;

#ifdef IMPDRV_S0S1_ENABLE
static uint32_t count_s0;
static uint32_t count_s1;

static void reset_s0s1_info(void)
{
	unsigned char *b; uint32_t size;

	S0S1_GETINFO(r_impdrv_api, b, size);		memset(b, 0, size);
	S0S1_GETINFO(r_impdrv_genctl, b, size);		memset(b, 0, size);
#if defined(STEP2) || defined(STEP3) || defined(STEP4)
#if defined(STEP3) || defined(STEP4)
	S0S1_GETINFO(r_impdrv_osdep, b, size);		memset(b, 0, size);
	S0S1_GETINFO(r_impdrv_osdep_mmngr, b, size);		memset(b, 0, size);
#endif
#if defined (V3H1)
	S0S1_GETINFO(target_v3h1_r_impdrv_cmnctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_cmnctl_reg, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_impctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_impsctl, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_ocvctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_dmactl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_pscctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_cnnctl, b, size);		memset(b, 0, size);
#elif defined (V3H2)
	S0S1_GETINFO(target_v3h2_r_impdrv_cmnctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_cmnctl_reg, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_impctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_impsctl, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_ocvctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_dmactl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_dmasctl, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_pscctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_cnnctl, b, size);		memset(b, 0, size);
#elif defined (V3M2)
	S0S1_GETINFO(target_v3m2_r_impdrv_cmnctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_cmnctl_reg, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_impctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_ocvctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_dmactl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_pscctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_cnnctl, b, size);		memset(b, 0, size);
#elif defined (V3U)
	S0S1_GETINFO(target_v3u_r_impdrv_cmnctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3u_r_impdrv_cmnctl_reg, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v3u_r_impdrv_impctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3u_r_impdrv_ocvctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3u_r_impdrv_dmactl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3u_r_impdrv_pscctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v3u_r_impdrv_cnnctl, b, size);		memset(b, 0, size);
#elif defined (V4H)
	S0S1_GETINFO(target_v4h_r_impdrv_cmnctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h_r_impdrv_cmnctl_reg, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v4h_r_impdrv_impctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h_r_impdrv_ocvctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h_r_impdrv_dmactl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h_r_impdrv_dmasctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h_r_impdrv_dspctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h_r_impdrv_pscctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h_r_impdrv_cnnctl, b, size);		memset(b, 0, size);
#elif defined (V4H2)
	S0S1_GETINFO(target_v4h2_r_impdrv_cmnctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_cmnctl_reg, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_impctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_ocvctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_dmactl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_dmasctl, b, size);	memset(b, 0, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_dspctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_pscctl, b, size);		memset(b, 0, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_cnnctl, b, size);		memset(b, 0, size);
#endif
#endif
}

static void print_s0s1log(const char *file, unsigned char *b, uint32_t length)
{
	size_t i;
	const int32_t S0_FLAG = 0x1;
	const int32_t S1_FLAG = 0x2;
	printf("----------------------------\nS0/S1 mark info for %s\n", file);
	for (i = 0; i < length; i++) {
		if (b[i] & S0_FLAG) {
			count_s0++;
		}
		if (b[i] & S1_FLAG) {
			count_s1++;
		}
		printf("%u\n", (uint32_t)b[i]);
	}
}

static void showt_s0s1_info(void)
{
	unsigned char *b; uint32_t size;

	count_s0 = count_s1 = 0;

	S0S1_GETINFO(r_impdrv_api, b, size);		print_s0s1log("r_impdrv_api", b, size);
	S0S1_GETINFO(r_impdrv_genctl, b, size);		print_s0s1log("r_impdrv_genctl", b, size);
#if defined(STEP2) || defined(STEP3) || defined(STEP4)
#if defined(STEP3) || defined(STEP4)
	S0S1_GETINFO(r_impdrv_osdep, b, size);		print_s0s1log("r_impdrv_osdep", b, size);
	S0S1_GETINFO(r_impdrv_osdep_mmngr, b, size);		print_s0s1log("r_impdrv_osdep_mmngr", b, size);
#endif
#if defined (V3H1)
	S0S1_GETINFO(target_v3h1_r_impdrv_cmnctl, b, size);		print_s0s1log("target_v3h1_r_impdrv_cmnctl", b, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_cmnctl_reg, b, size);	print_s0s1log("target_v3h1_r_impdrv_cmnctl_reg", b, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_impctl, b, size);		print_s0s1log("target_v3h1_r_impdrv_impctl", b, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_impsctl, b, size);	print_s0s1log("target_v3h1_r_impdrv_impsctl", b, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_ocvctl, b, size);		print_s0s1log("target_v3h1_r_impdrv_ocvctl", b, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_dmactl, b, size);		print_s0s1log("target_v3h1_r_impdrv_dmactl", b, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_pscctl, b, size);		print_s0s1log("target_v3h1_r_impdrv_pscctl", b, size);
	S0S1_GETINFO(target_v3h1_r_impdrv_cnnctl, b, size);		print_s0s1log("target_v3h1_r_impdrv_cnnctl", b, size);
#elif defined (V3H2)
	S0S1_GETINFO(target_v3h2_r_impdrv_cmnctl, b, size);		print_s0s1log("target_v3h2_r_impdrv_cmnctl", b, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_cmnctl_reg, b, size);	print_s0s1log("target_v3h2_r_impdrv_cmnctl_reg", b, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_impctl, b, size);		print_s0s1log("target_v3h2_r_impdrv_impctl", b, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_impsctl, b, size);	print_s0s1log("target_v3h2_r_impdrv_impsctl", b, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_ocvctl, b, size);		print_s0s1log("target_v3h2_r_impdrv_ocvctl", b, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_dmactl, b, size);		print_s0s1log("target_v3h2_r_impdrv_dmactl", b, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_dmasctl, b, size);	print_s0s1log("target_v3h2_r_impdrv_dmasctl", b, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_pscctl, b, size);		print_s0s1log("target_v3h2_r_impdrv_pscctl", b, size);
	S0S1_GETINFO(target_v3h2_r_impdrv_cnnctl, b, size);		print_s0s1log("target_v3h2_r_impdrv_cnnctl", b, size);
#elif defined (V3M2)
	S0S1_GETINFO(target_v3m2_r_impdrv_cmnctl, b, size);		print_s0s1log("target_v3m2_r_impdrv_cmnctl", b, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_cmnctl_reg, b, size);	print_s0s1log("target_v3m2_r_impdrv_cmnctl_reg", b, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_impctl, b, size);		print_s0s1log("target_v3m2_r_impdrv_impctl", b, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_ocvctl, b, size);		print_s0s1log("target_v3m2_r_impdrv_ocvctl", b, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_dmactl, b, size);		print_s0s1log("target_v3m2_r_impdrv_dmactl", b, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_pscctl, b, size);		print_s0s1log("target_v3m2_r_impdrv_pscctl", b, size);
	S0S1_GETINFO(target_v3m2_r_impdrv_cnnctl, b, size);		print_s0s1log("target_v3m2_r_impdrv_cnnctl", b, size);
#elif defined (V3U)
	S0S1_GETINFO(target_v3u_r_impdrv_cmnctl, b, size);		print_s0s1log("target_v3u_r_impdrv_cmnctl", b, size);
	S0S1_GETINFO(target_v3u_r_impdrv_cmnctl_reg, b, size);	print_s0s1log("target_v3u_r_impdrv_cmnctl_reg", b, size);
	S0S1_GETINFO(target_v3u_r_impdrv_impctl, b, size);		print_s0s1log("target_v3u_r_impdrv_impctl", b, size);
	S0S1_GETINFO(target_v3u_r_impdrv_ocvctl, b, size);		print_s0s1log("target_v3u_r_impdrv_ocvctl", b, size);
	S0S1_GETINFO(target_v3u_r_impdrv_dmactl, b, size);		print_s0s1log("target_v3u_r_impdrv_dmactl", b, size);
	S0S1_GETINFO(target_v3u_r_impdrv_pscctl, b, size);		print_s0s1log("target_v3u_r_impdrv_pscctl", b, size);
	S0S1_GETINFO(target_v3u_r_impdrv_cnnctl, b, size);		print_s0s1log("target_v3u_r_impdrv_cnnctl", b, size);
#elif defined (V4H)
	S0S1_GETINFO(target_v4h_r_impdrv_cmnctl, b, size);		print_s0s1log("target_v4h_r_impdrv_cmnctl", b, size);
	S0S1_GETINFO(target_v4h_r_impdrv_cmnctl_reg, b, size);	print_s0s1log("target_v4h_r_impdrv_cmnctl_reg", b, size);
	S0S1_GETINFO(target_v4h_r_impdrv_impctl, b, size);		print_s0s1log("target_v4h_r_impdrv_impctl", b, size);
	S0S1_GETINFO(target_v4h_r_impdrv_ocvctl, b, size);		print_s0s1log("target_v4h_r_impdrv_ocvctl", b, size);
	S0S1_GETINFO(target_v4h_r_impdrv_dmactl, b, size);		print_s0s1log("target_v4h_r_impdrv_dmactl", b, size);
	S0S1_GETINFO(target_v4h_r_impdrv_dmasctl, b, size);		print_s0s1log("target_v4h_r_impdrv_dmasctl", b, size);
	S0S1_GETINFO(target_v4h_r_impdrv_dspctl, b, size);		print_s0s1log("target_v4h_r_impdrv_dspctl", b, size);
	S0S1_GETINFO(target_v4h_r_impdrv_pscctl, b, size);		print_s0s1log("target_v4h_r_impdrv_pscctl", b, size);
	S0S1_GETINFO(target_v4h_r_impdrv_cnnctl, b, size);		print_s0s1log("target_v4h_r_impdrv_cnnctl", b, size);
#elif defined (V4H2)
	S0S1_GETINFO(target_v4h2_r_impdrv_cmnctl, b, size);		print_s0s1log("target_v4h2_r_impdrv_cmnctl", b, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_cmnctl_reg, b, size);	print_s0s1log("target_v4h2_r_impdrv_cmnctl_reg", b, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_impctl, b, size);		print_s0s1log("target_v4h2_r_impdrv_impctl", b, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_ocvctl, b, size);		print_s0s1log("target_v4h2_r_impdrv_ocvctl", b, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_dmactl, b, size);		print_s0s1log("target_v4h2_r_impdrv_dmactl", b, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_dmasctl, b, size);	print_s0s1log("target_v4h2_r_impdrv_dmasctl", b, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_dspctl, b, size);		print_s0s1log("target_v4h2_r_impdrv_dspctl", b, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_pscctl, b, size);		print_s0s1log("target_v4h2_r_impdrv_pscctl", b, size);
	S0S1_GETINFO(target_v4h2_r_impdrv_cnnctl, b, size);		print_s0s1log("target_v4h2_r_impdrv_cnnctl", b, size);
#endif
#endif
	printf("----------------------------\ntotal S0 mark: %u\ntotal S1 mark: %u\n", count_s0, count_s1);
}
#endif

void cbfunc_fatal(
    const st_impdrv_core_info_t *const p_core_info,     /**< Core information                   */
    const e_impdrv_fatalcode_t  fc_code,                /**< Fatal callback result code         */
    const e_impdrv_errorcode_t  ercd,                   /**< IMP Driver return code             */
    void                        *const p_callback_args  /**< Callback function arguments        */
)
{
	fatal_code = fc_code;
	fatal_ercd = ercd;
}

void set_input_data_before_init(struct test_params_t *tparam)
{
	uint32_t i, j;
	tp_core_list_t run_core;
	if (tparam->check_no_dmac_flag == true)
	{
		run_core = get_core_list(IT_IMPDRV_CMD_PLATFORM_SOC_NO_DMA, 0);
	}
	else
	{
		run_core = get_core_list(IT_IMPDRV_CMD_PLATFORM_SOC, 0);
	}
	tp_core_list_t core_map = get_core_list(IT_IMPDRV_CMD_COREMAP, 0);

	memset(&tparam->initdata, 0, sizeof(tparam->initdata));
	memset(tparam->core_map, 0, sizeof(tparam->core_map));
	tparam->initdata.use_core_num = run_core.num;
	memcpy(tparam->initdata.core_info, run_core.core_info, run_core.size);
	memcpy(tparam->core_map, core_map.core_info, core_map.size);
	tparam->initdata.osal_resource.mutex_id = OSAL_MUTEX_IMP_DRV_LOCK;

	for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++)
	{
		for (j = 0; j < IMPDRV_CORE_NUM_MAX; j++)
		{
			impdrv_ctl.coretype_ctl[i].core_ctl[j].core_state = IMPDRV_STATE_UNINIT;
		}
	}

	tparam->initdata.p_work_addr = (void *)&impdrv_ctl;
	tparam->initdata.work_size = sizeof(impdrv_ctl);
#if defined (V3U)
	tparam->initdata.instance_num = IMPDRV_INSTANCE_6;
#else
	tparam->initdata.instance_num = IMPDRV_INSTANCE_0;
#endif
	tparam->initdata.callback_func_fatal = (p_impdrv_cbfunc_fatal_t)&cbfunc_fatal;

	/* for Bus Interface settings */
	tparam->chk_resource.memory_handle = util_mem_get_handle(tparam->busif_mem);
	tparam->chk_resource.axi_id = imp_dev_axi_bus_id;
	tparam->chk_resource.mq_id = OSAL_MQ_IMP_RTT_DRVCB_00;
	tparam->chk_resource.mq_time_period = 500U;
}

void initialize_impdrvctl()
{
	memset(&impdrv_ctl, 0, sizeof(impdrv_ctl));
}

int32_t imp_it_main(void)
{
	struct test_params_t tparam;
	uint32_t i, j;
	int32_t ret = 0;
	int32_t ret_sub = 0;
	struct test_result_t test_result = {0};
	struct test_result_t tret = {0};

	ret = OsDepInit(&osal_mmngr);
	if (ret == 0)
	{
		puts("imp_it_main");

#ifdef IMPDRV_S0S1_ENABLE
		reset_s0s1_info();
#endif

		/* test data initialize  */
		memset(&tparam, 0, sizeof(tparam));

		/* alloc memory */
		for (i = 0; i < SYNC_MAX_NUM; i++) {
			tparam.claddr[i] = MemAlloc(osal_mmngr, CL_SIZE, 4096, IMP_MALLOC_HW);
			if (tparam.claddr[i] == NULL) {
				goto error_short_mem;
			}
		}

		tparam.long_claddr = MemAlloc(osal_mmngr, LONG_CL_SIZE, 4096, IMP_MALLOC_HW);
		if (tparam.long_claddr == NULL) {
			goto error_long_mem;
		}

		tparam.busif_mem = (uint32_t *)MemAlloc(osal_mmngr, IMPDRV_IMP_MEM_SIZE_BUSCHK, 128u, IMP_MALLOC_HW);
		if (NULL == tparam.busif_mem) {
			goto error_busif_mem;
		}

#if defined (IMPDRV_S0S1_ENABLE) && defined (STEP2)
		impdrv_cmnctl_get_io_dta(NULL);		/* This is just for S0 to be fulfiled */
#endif

		/* set up input data before testing */
		initialize_impdrvctl();
		set_input_data_before_init(&tparam);

		for (i = 0; i < IMPDRV_CORE_TYPE_MAX; i++) {
			for (j = 0; j < IMPDRV_CORE_NUM_MAX; j++) {
				hook_cbfunc_exec_ret[i][j] = HOOK_TEST_RET_NONE;
				hook_cbfunc_int_ret[i][j] = HOOK_TEST_RET_NONE;
			}
		}

		puts("imp it start");

		/* execute test */
		RUN_TEST();
#if defined (STEP4)
#if !defined (IMPDRV_S0S1_ENABLE)
		RUN_TEST_2();
#endif
		RUN_TEST_3();
#endif

		/* free memory */
		MemFree(osal_mmngr, tparam.long_claddr);
error_long_mem:
		MemFree(osal_mmngr, (void *)tparam.busif_mem);
error_busif_mem:
		i = SYNC_MAX_NUM;
error_short_mem:
		for (; i > 0; i--) {
			uint32_t free_index = i - 1;
			MemFree(osal_mmngr, tparam.claddr[free_index]);
		}

#ifdef IMPDRV_S0S1_ENABLE
		showt_s0s1_info();
#endif
	}

	/* Update return code */
	if (0 == ret)
	{
		ret = (0 != test_result.failed_cnt) ? (-1) : ret;
	}

	/* Deinitialize OSAL Library */
	ret_sub = OsDepDeinit(&osal_mmngr);
	if (0 == ret)
	{
		ret = ret_sub;
	}

	PRINT_FINAL_TEST_RESULT("imp it", test_result, ret)

	puts("imp it end");

	return ret;
}

int32_t main(void)
{
	return imp_it_main();
}