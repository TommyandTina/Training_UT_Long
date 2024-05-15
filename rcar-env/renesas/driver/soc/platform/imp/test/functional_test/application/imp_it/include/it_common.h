#ifndef __IT_COMMON_H__
#define __IT_COMMON_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#if defined (TARGET_OS_LINUX)
#include <linux/membarrier.h>
#include <sys/syscall.h>
#endif

#include "rcar-xos/imp/r_impdrv.h"
#include "rcar-xos/imp/r_impdrv_version.h"

#include "rcar-xos/osal/r_osal.h"

#include "it_util.h"
#include "it_osal_config.h"

/* macro used to log print */
#define LOGPRINT(ARG)   (void)(printf ARG, fflush(stdout))

enum
{
	HOOK_TEST_RET_NONE = 0,
	HOOK_TEST_RET_OK,
	HOOK_TEST_RET_NG
};

/*** struct ***/
struct test_params_t
{
	st_impdrv_initdata_t initdata;
	st_impdrv_dsp_data_t dsp_data;
	st_impdrv_dsp_data_t dsp_fw;
	st_impdrv_dsp_data_t dsp_app;
	st_impdrv_dsp_data_t dsp_dtcm;
	st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID];
	void *claddr[SYNC_MAX_NUM];
	void *long_claddr;
	uint32_t *busif_mem;
	st_impdrv_chk_resource_t chk_resource;
	bool check_no_dmac_flag
};

struct test_result_t
{
	int32_t passed_cnt;
	int32_t failed_cnt;
	int32_t skipped_cnt
};

/*** macro ***/
#define testmsg_printf(...) printf(__VA_ARGS__)

#define print_tn(__test, ___category, ...)                 \
	do                                                     \
	{                                                      \
		testmsg_printf("%s : %s : ", __test, ___category); \
		printf(__VA_ARGS__);                               \
	} while (0);

#define _FailTestPrint(__ret, __condition, __tret, __test, ___category, ...)                \
	do                                                                                      \
	{                                                                                       \
		if (__ret == __condition)                                                           \
		{                                                                                   \
			print_tn(__test, ___category, ##__VA_ARGS__);                                   \
			testmsg_printf(" fail(test OK)\n");                                             \
			(__tret).passed_cnt++;                                                                \
		}                                                                                   \
		else if (__ret == IMPDRV_EC_OK)                                                     \
		{                                                                                   \
			print_tn(__test, ___category, ##__VA_ARGS__);                                   \
			testmsg_printf(" success(test NG) with ret %s\n", impdrv_err_to_name(__ret));   \
			(__tret).failed_cnt++;                                                                \
		}                                                                                   \
		else                                                                                \
		{                                                                                   \
			print_tn(__test, ___category, ##__VA_ARGS__);                                   \
			testmsg_printf(" fail(test NG, illegal ret %s)\n", impdrv_err_to_name(__ret));  \
			(__tret).failed_cnt++;                                                                \
		}                                                                                   \
	} while (0)

#define CountResult(__condition, __tret)               \
	do                                                 \
	{                                                  \
		__condition ? (tret->passed_cnt)++ : (tret->failed_cnt)++; \
	} while (0)

#define Core_CountResult(__core, __condition, __tret)   \
	do                                                  \
	{                                                   \
		if (__condition)                                \
		{                                               \
			core_testmsg(__core, "Success(test OK)\n"); \
			(__tret->passed_cnt)++;                           \
		}                                               \
		else                                            \
		{                                               \
			(__tret->failed_cnt)++;                           \
		}                                               \
	} while (0)

#define print_test_result(__test, __tret)                      \
	printf("%s : test result %d success, %d error, %d skip\n", \
		   __test, (__tret).passed_cnt, (__tret).failed_cnt, (__tret).skipped_cnt);

#define PRINT_FINAL_TEST_RESULT(__test, __tret, __test_prog_ret)                    \
	{ \
		printf("[SUMMARY-START]\n"); \
		printf("  [TOTAL-TC-DETECTED]           %d\n", ((__tret).passed_cnt + (__tret).failed_cnt + (__tret).skipped_cnt)); \
		printf("    [TOTAL-TC-NOT-EXEC]         %d\n", (__tret).skipped_cnt); \
		printf("      [TOTAL-TC-ARGS-FILTER]    -\n"); \
		printf("      [TOTAL-TC-ENV-FILTER]     -\n"); \
		printf("      [TOTAL-TC-SKIP]           %d\n", (__tret).skipped_cnt); \
		printf("    [TOTAL-TC-EXEC]             %d\n", ((__tret).passed_cnt + (__tret).failed_cnt)); \
		printf("      [TOTAL-TC-PASS]           %d\n", (__tret).passed_cnt); \
		printf("      [TOTAL-TC-FAIL]           %d\n", (__tret).failed_cnt); \
		printf("      [TOTAL-TC-ERR]            -\n"); \
		printf("      [TOTAL-TC-INCONSISTENCY]  -\n"); \
		printf("  [TEST-DURATION-US]            -\n"); \
		printf("  [EXIT-CODE]                   %d\n", __test_prog_ret); \
		printf("[SUMMARY-END]\n\n"); \
	}

#ifdef TEST_MSG_NAME
#define testmsg(...) print_tn(TEST_MSG_NAME, __func__, __VA_ARGS__)
#define core_testmsg(__core, ...)                                                   \
	do                                                                              \
	{                                                                               \
		testmsg_printf("%s%d ", type_to_name((__core)->core_type), (__core)->core_num); \
		testmsg(__VA_ARGS__);                                                       \
	} while (0)

#define SuccessTestPrint(__condition, __tret, ...)          \
	do                                                      \
	{                                                       \
		if (__condition)                                    \
		{                                                   \
			print_tn(TEST_MSG_NAME, __func__, __VA_ARGS__); \
			testmsg_printf("success(test OK)\n");          \
			((__tret).passed_cnt)++;                              \
		}                                                   \
		else                                                \
		{                                                   \
			print_tn(TEST_MSG_NAME, __func__, __VA_ARGS__); \
			testmsg_printf("fail(test NG)\n");             \
			((__tret).failed_cnt)++;                              \
		}                                                   \
	} while (0)

#define FailTestPrint(__ret, __condition, __tret, ...) \
	_FailTestPrint(__ret, __condition, __tret, TEST_MSG_NAME, __func__, __VA_ARGS__)

#define SkipTestPrint(__tret, ...)                      \
	do                                                  \
	{                                                   \
		print_tn(TEST_MSG_NAME, __func__, __VA_ARGS__); \
		testmsg_printf(" Not Tested(test Skip)\n");     \
		(__tret).skipped_cnt++;                                \
	} while (0)

#define PrintTestResult(__tret, __test_result)    \
	do                                            \
	{                                             \
		print_test_result(TEST_MSG_NAME, __tret); \
		if (__test_result)                        \
		{                                         \
			*__test_result = __tret;              \
		}                                         \
	} while (0)
#endif

// #define ARRAY_SIZE(arr)		(sizeof(arr)/sizeof(arr[0]))

/*** function ***/
void set_input_data_before_init(struct test_params_t *tparam);
void set_input_data_before_init_no_dma(struct test_params_t *tparam);
void initialize_impdrvctl();
int set_commandlist(uint32_t type, void *addr, uint32_t code);
int set_intcommandlist(uint32_t type, void *addr, uint32_t icode, uint32_t tcode);
int set_conint_commandlist(uint32_t type, void *addr, uint32_t icode, uint32_t tcode);
int set_interror_commandlist(uint32_t type, void *addr, uint32_t icode, uint32_t tcode);
int set_pbcovfcommandlist(uint32_t type, void *addr, uint32_t tcode);
int set_synccommandlist(uint32_t type, void *addr, uint32_t code, uint32_t sync);
int set_errcommandlist(uint32_t type, void *addr, uint32_t code);

/*** val ***/
extern struct test_params_t tparam;
extern int32_t hook_stop_int_ret;
extern uint32_t hook_cbfunc_exec_ret[IMPDRV_CORE_TYPE_MAX][IMPDRV_CORE_NUM_MAX];
extern uint32_t hook_cbfunc_int_ret[IMPDRV_CORE_TYPE_MAX][IMPDRV_CORE_NUM_MAX];
extern e_impdrv_fatalcode_t  fatal_code;
extern e_impdrv_errorcode_t  fatal_ercd;

#endif /* __IT_COMMON_H__ */
