/**
 * @file memory_manager.c
 * @brief OSAL Memory Manager evaluation file.
 * @author K.Takagi
 * @date 2020/11/04
 */

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include "osal_test.h"
#include "osal_test_common.h"
#include "memory_manager.h"

#define OSAL_MAX_MEMORY_LIMIT   (256 * 1024 * 1024)

/* prototype */
static void osal_test_R_OSAL_MmngrOpen();
static void osal_test_R_OSAL_MmngrClose();
static void osal_test_R_OSAL_MmngrAlloc();
static void osal_test_R_OSAL_MmngrDealloc();
static void osal_test_R_OSAL_MmngrGetConfig();
static void osal_test_R_OSAL_MmngrGetOsalMaxConfig();
static void osal_test_R_OSAL_MmngrInitializeMmngrConfigSt();
static void osal_test_R_OSAL_MmngrRegisterMonitor();
static void osal_test_R_OSAL_MmngrGetCpuPtr();
static void osal_test_R_OSAL_MmngrGetConstCpuPtr();
static void osal_test_R_OSAL_MmngrFlush();
static void osal_test_R_OSAL_MmngrInvalidate();
static void osal_test_R_OSAL_MmngrGetHwAddr();
static void osal_test_R_OSAL_MmngrGetSize();
static void osal_test_R_OSAL_MmngrCheckBuffer();
static void osal_test_R_OSAL_MmngrOpenMulti();
#if IT_DEBUG_ON
static void osal_test_R_OSAL_MmngrDebugMmngrPrint();
static void osal_test_R_OSAL_MmngrDebugBufferPrint();
#endif

/* typedef function pointer */
typedef e_osal_return_t (*p_alloc_f_t)(void * p_ptr_self, size_t bytes, size_t alignment,
                                          osal_memory_buffer_handle_t * const buffer_obj);
typedef e_osal_return_t (*p_dealloc_f_t)(void * p_ptr_self, osal_memory_buffer_handle_t buffer_obj);
typedef e_osal_return_t (*p_getcfg_f_t)(const void * p_ptr_self, st_osal_mmngr_config_t * const config);
typedef e_osal_return_t (*p_reg_mon_f_t)(void * p_ptr_self, e_osal_mmngr_event_t monitored_event,
                                         fp_osal_memory_mngr_cb_t monitor_handler, void * monitor_handler_user_arg);

typedef e_osal_return_t (*p_cpuptr_f_t)(void * p_ptr_self, void ** const cpu_ptr);
typedef e_osal_return_t (*p_const_cpuptr_f_t)(void * p_ptr_self, const void ** const const_cpu_ptr);
typedef e_osal_return_t (*p_flush_f_t)(void * p_ptr_self, size_t offset, size_t size);
typedef e_osal_return_t (*p_invalid_f_t)(void * p_ptr_self, size_t offset, size_t size);
typedef e_osal_return_t (*p_hwaddr_f_t)(const void * p_ptr_self, osal_axi_bus_id_t axi_id, uintptr_t * const hw_addr);
typedef e_osal_return_t (*p_size_f_t)(const void * p_ptr_self, size_t * const size);
typedef e_osal_return_t (*p_checkbuffer_f_t)(const void * p_ptr_self);

#if IT_DEBUG_ON
typedef e_osal_return_t (*p_mprint_t)(const void * p_ptr_self, FILE * output);
typedef e_osal_return_t (*p_bprint_t)(const void * p_ptr_self, FILE * output);

#define OSAL_DEBUG_LOG_PATH "/tmp/mmngr_test.log"
#endif

static osal_memory_buffer_handle_t  *buffer_obj_list;

void osal_test_memory_manager_display_menu()
{

    printf("*** OSAL Memory Manager ***\n");
    printf("***  09_001: R_OSAL_MmngrOpen                    ***\n");
    printf("***  09_002: R_OSAL_MmngrClose                   ***\n");
    printf("***  09_003: R_OSAL_MmngrAlloc                   ***\n");
    printf("***  09_004: R_OSAL_MmngrDealloc                 ***\n");
    printf("***  09_005: R_OSAL_MmngrGetConfig               ***\n");
    printf("***  09_006: R_OSAL_MmngrGetOsalMaxConfig        ***\n");
    printf("***  09_007: R_OSAL_MmngrInitializeMmngrConfigSt ***\n");
    printf("***  09_008: R_OSAL_MmngrRegisterMonitor         ***\n");
    printf("***  09_009: R_OSAL_MmngrGetCpuPtr               ***\n");
    printf("***  09_010: R_OSAL_MmngrGetConstCpuPtr          ***\n");
    printf("***  09_011: R_OSAL_MmngrFlush                   ***\n");
    printf("***  09_012: R_OSAL_MmngrInvalidate              ***\n");
    printf("***  09_013: R_OSAL_MmngrGetHwAddr               ***\n");
    printf("***  09_014: R_OSAL_MmngrGetSize                 ***\n");
    printf("***  09_015: R_OSAL_MmngrCheckBuffer             ***\n");
    printf("***  09_016: Multi open scenario test            ***\n");
#if IT_DEBUG_ON
    printf("***  09_017: R_OSAL_MmngrDebugMmngrPrint         ***\n");
    printf("***  09_018: R_OSAL_MmngrDebugBufferPrint        ***\n");
#endif
    printf("***  09_100: memory manager module all exec ***\n\n");
}

void osal_test_memory_manager_menu(int child)
{

    int test_number = child;
    unsigned int test_count  = 1;
    unsigned int loop_count  = 0;

    /* ALL check */
    /* when select all */
    if (child == OSAL_TEST_ALL_EXEC)
    {
        test_number = OSAL_TEST_MMNGROPEN;
        test_count  = OSAL_TEST_MMNGR_MAX;
    }

    for (loop_count = 0; loop_count < test_count; loop_count++)
    {
        switch (test_number)
        {
            case OSAL_TEST_MMNGROPEN:
                osal_test_R_OSAL_MmngrOpen();
                break;
            case OSAL_TEST_MMNGRCLOSE:
                osal_test_R_OSAL_MmngrClose();
                break;
            case OSAL_TEST_MMNGRALLOC:
                osal_test_R_OSAL_MmngrAlloc();
                break;
            case OSAL_TEST_MMNGRDEALLOC:
                osal_test_R_OSAL_MmngrDealloc();
                break;
            case OSAL_TEST_MMNGRGETCONFIG:
                osal_test_R_OSAL_MmngrGetConfig();
                break;
            case OSAL_TEST_MMNGRGETOSALMAXCONFIG:
                osal_test_R_OSAL_MmngrGetOsalMaxConfig();
                break;
            case OSAL_TEST_MMNGRINITIALIZEMMNGRCONFIGST:
                osal_test_R_OSAL_MmngrInitializeMmngrConfigSt();
                break;
            case OSAL_TEST_MMNGRREGISTERMONITOR:
                osal_test_R_OSAL_MmngrRegisterMonitor();
                break;
            case OSAL_TEST_MMNGRGETCPUPTR:
                osal_test_R_OSAL_MmngrGetCpuPtr();
                break;
            case OSAL_TEST_MMNGRGETCONSTCPUPTR:
                osal_test_R_OSAL_MmngrGetConstCpuPtr();
                break;
            case OSAL_TEST_MMNGRFLUSH:
                osal_test_R_OSAL_MmngrFlush();
                break;
            case OSAL_TEST_MMNGRINVALIDATE:
                osal_test_R_OSAL_MmngrInvalidate();
                break;
            case OSAL_TEST_MMNGRGETHWADDR:
                osal_test_R_OSAL_MmngrGetHwAddr();
                break;
            case OSAL_TEST_MMNGRGETSIZE:
                osal_test_R_OSAL_MmngrGetSize();
                break;
            case OSAL_TEST_MMNGRCHECKBUFFER:
                osal_test_R_OSAL_MmngrCheckBuffer();
                break;
            case OSAL_TEST_MULTIOPEN:
                osal_test_R_OSAL_MmngrOpenMulti();
                break;
#if IT_DEBUG_ON
            case OSAL_TEST_MMNGRDEBUGMMNGRPRINT:
                osal_test_R_OSAL_MmngrDebugMmngrPrint();
                break;
            case OSAL_TEST_MMNGRDEBUGBUFFERPRINT:
                osal_test_R_OSAL_MmngrDebugBufferPrint();
                break;
#endif
            default:
                printf("That menu is not available.\n\n");
                break;
        }

        test_number++;
    }
}

e_osal_return_t osal_init_memory()
{
    e_osal_return_t ret = OSAL_RETURN_OK;
    if(false == init_flag)
    {
        ret = R_OSAL_Initialize();
        if(ret == OSAL_RETURN_OK)
        {
            init_flag = true;
        }

    }

    return ret;
}

e_osal_return_t osal_deinit_memory()
{
    e_osal_return_t ret = OSAL_RETURN_OK;
    if(true == init_flag)
    {
        ret = R_OSAL_Deinitialize();
        if(ret == OSAL_RETURN_OK)
        {
            init_flag = false;
        }
    }

    return ret;
}

/* Pseudo user callback functions */
void osal_memory_mngr_cb(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error,
                         osal_memory_buffer_handle_t buffer_hndl)
{
    (void)user_arg;
    (void)error;
    (void)buffer_hndl;

	char * event_str;

	switch (event)
	{
		case OSAL_MMNGR_EVENT_ALLOCATE:
			event_str = "Occurred to allocate event.";
			break;
		case OSAL_MMNGR_EVENT_DEALLOCATE:
			event_str = "Occurred to deallocate event.";
			break;
		case OSAL_MMNGR_EVENT_OVERFLOW_DETECTED:
			event_str = "Occurred to overflow event.";
			break;
		case OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED:
			event_str = "Occurred to underflow event.";
			break;
		default:
			event_str = "Occurred to unknown event.";
			break;
	}

//	printf("%s() In usrCB, %s result[%d]\n", __func__, event_str, error);

	return;
}

static e_osal_return_t osal_test_mmngr_open_preprocess(int api_id, st_osal_mmngr_config_t *config)
{
    e_osal_return_t osal_ret;

    osal_ret = osal_init_memory();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.", OSAL_TEST_MEMORY, api_id, 0);
        return osal_ret;
    }

    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetOsalMaxConfig() error.", OSAL_TEST_MEMORY, api_id, 0);
        return osal_ret;
    }

    return osal_ret;
}

static e_osal_return_t osal_test_mmngr_alloc_preprocess(int api_id, st_osal_mmngr_config_t *config,
                                                        osal_memory_manager_handle_t *hndl_mmngr)
{
    e_osal_return_t osal_ret;

    osal_ret = osal_test_mmngr_open_preprocess(api_id, config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return osal_ret;
    }

    config->max_allowed_allocations     = 100;
    config->max_registered_monitors_cbs = 100;
    osal_ret = R_OSAL_MmngrOpen(config, hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.", OSAL_TEST_MEMORY, api_id, 0);
        return osal_ret;
    }

    return osal_ret;
}

static e_osal_return_t osal_test_mmngr_close_post(int api_id, osal_memory_manager_handle_t hndl_mmngr)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrClose() error.", OSAL_TEST_MEMORY, api_id, 0);
        return osal_ret;
    }

    osal_ret = osal_deinit_memory();
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.", OSAL_TEST_MEMORY, api_id, 0);
        return osal_ret;
    }

    return osal_ret;
}

static e_osal_return_t osal_test_mmngr_dealloc_post(int api_id, osal_memory_manager_handle_t hndl_mmngr,
                                                    osal_memory_buffer_handle_t buffer_obj)
{
    e_osal_return_t osal_ret;

    osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrDealloc() error.", OSAL_TEST_MEMORY, api_id, 0);
        return osal_ret;
    }

    osal_ret = osal_test_mmngr_close_post(api_id, hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return osal_ret;
    }

    return osal_ret;
}

/* ------------------------09_001_001 [success]------------------------ */
/* ------------------------09_001_002 [OSAL_RETURN_STATE]------------------------ */
/* ------------------------09_001_003 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_001_004 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_001_005 [OSAL_RETURN_CONF]------------------------ */
/* ------------------------09_001_006 [OSAL_RETURN_CONF]------------------------ */
/* ------------------------09_001_007 [OSAL_RETURN_CONF]------------------------ */
/* ------------------------09_001_008 [OSAL_RETURN_CONF]------------------------ */
/* ------------------------09_001_009 [OSAL_RETURN_CONF]------------------------ */
/* ------------------------09_001_010 [OSAL_RETURN_CONF]------------------------ */
/* ------------------------09_001_011 [OSAL_RETURN_MEM]------------------------ */
static void Test09_001_001_011(osal_memory_manager_handle_t *hndl_mmngr, st_osal_mmngr_config_t *config, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGROPEN, test_id);

    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrOpen(config, hndl_mmngr);
    TIME_MEASURE_STOP
    
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGROPEN,
                            test_id, 
                            test_ret, opt_info);
}


/* ------------------------09_002_001 [success]------------------------ */
/* ------------------------09_002_002 [OSAL_RETURN_STATE]------------------------ */
/* ------------------------09_002_003 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_002_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test09_002_001_004(osal_memory_manager_handle_t hndl_mmngr, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGRCLOSE, test_id);

    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
    TIME_MEASURE_STOP
    
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRCLOSE,
                            test_id,
                            test_ret, opt_info);
}


/* ------------------------09_003_001 [success]------------------------ */
/* ------------------------09_003_002 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_003_003 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_003_004 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_003_005 [OSAL_RETURN_HANDLE_NOT_SUPPORTED]------------------------ */
/* ------------------------09_003_006 [OSAL_RETURN_CONF]------------------------ */
static void Test09_003_001_006(osal_memory_manager_handle_t hndl_mmngr, size_t size, size_t alignment,
                               osal_memory_buffer_handle_t *buffer_obj, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGRALLOC, test_id);

    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, buffer_obj);
    TIME_MEASURE_STOP
    
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRALLOC,
                            test_id, 
                            test_ret, opt_info);
}

/* ------------------------09_003_007 [success]------------------------ */
static void Test09_003_007(osal_memory_manager_handle_t hndl_mmngr, st_osal_mmngr_config_t config)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	buffer_obj_list = malloc(sizeof(osal_memory_buffer_handle_t) * config.max_allowed_allocations);
    size_t size      = 4096;
    size_t alignment = 4096;

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGRALLOC, OSAL_TEST_ID007);

    TIME_MEASURE_INIT
    TIME_MEASURE_START

    for (size_t i = 0; i < config.max_allowed_allocations - 1; i++) {
		osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj_list[i]);
    }

#if IT_DEBUG_ON
    FILE * output;

	output = fopen(OSAL_DEBUG_LOG_PATH, "a+");
	R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr, output);
#endif
	
	for (size_t i = 0; i < config.max_allowed_allocations - 1; i++) {
		R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj_list[i]);
    }

    TIME_MEASURE_STOP

    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY,
                            OSAL_TEST_MMNGRALLOC,
                            OSAL_TEST_ID007,
                            test_ret, opt_info);
}


/* ------------------------09_004_001 [success]------------------------ */
/* ------------------------09_004_002 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_004_003 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_004_004 [OSAL_RETURN_HANDLE_NOT_SUPPORTED]------------------------ */
/* ------------------------09_004_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test09_004_001_005(osal_memory_manager_handle_t hndl_mmngr, osal_memory_buffer_handle_t buffer_obj, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEALLOC,
                                                             test_id);

    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj);
    TIME_MEASURE_STOP
    
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRDEALLOC,
                            test_id,
                            test_ret, opt_info);
}


/* ------------------------09_005_001 [success]------------------------ */
/* ------------------------09_005_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_005_003 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_005_003 [OSAL_RETURN_HANDLE_NOT_SUPPORTED]------------------------ */
static void Test09_005_001_004(osal_memory_manager_handle_t hndl_mmngr, st_osal_mmngr_config_t *config, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCONFIG, test_id);

    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrGetConfig(hndl_mmngr, config);
    TIME_MEASURE_STOP
    
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRGETCONFIG,
                            test_id, 
                            test_ret, opt_info);
}


/* ------------------------09_006_001 [success]------------------------ */
/* ------------------------09_006_002 [OSAL_RETURN_PAR]------------------------ */
static void Test09_006_001_002(st_osal_mmngr_config_t *config, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIG,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(config);
    TIME_MEASURE_STOP

    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRGETOSALMAXCONFIG,
                            test_id,
                            test_ret, opt_info);
}


/* ------------------------09_007_001 [success]------------------------ */
/* ------------------------09_007_002 [OSAL_RETURN_PAR]------------------------ */
static void Test09_007_001_002(st_osal_mmngr_config_t *config, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINITIALIZEMMNGRCONFIGST,
                                                             test_id);

    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(config);
    TIME_MEASURE_STOP

    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRINITIALIZEMMNGRCONFIGST,
                            test_id,
                            test_ret, opt_info);
}


/* ------------------------09_008_001 [success]------------------------ */
/* ------------------------09_008_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_008_003 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_008_004 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_008_005 [OSAL_RETURN_PAR]------------------------ */
static void Test09_008_001_005(osal_memory_manager_handle_t hndl_mmngr, e_osal_mmngr_event_t event_id,
                               fp_osal_memory_mngr_cb_t monitor_handler, void* arg, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRREGISTERMONITOR,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrRegisterMonitor(hndl_mmngr, event_id, monitor_handler, arg);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRREGISTERMONITOR,
                            test_id, test_ret, opt_info);
}

/* ------------------------09_009_001 [success]------------------------ */
/* ------------------------09_009_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_009_003 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_009_004 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_009_005 [OSAL_RETURN_PAR]------------------------ */
static void Test09_009_001_005(osal_memory_buffer_handle_t hndl_buffer, uintptr_t** cpu_ptr, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR,
                                                             test_id);

    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrGetCpuPtr(hndl_buffer, (void **)cpu_ptr);
    TIME_MEASURE_STOP

    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRGETCPUPTR,
                            test_id,
                            test_ret, opt_info);
}

/* ------------------------09_010_001 [success]------------------------ */
/* ------------------------09_010_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_010_003 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_010_004 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_010_005 [OSAL_RETURN_PAR]------------------------ */
static void Test09_010_001_005(osal_memory_buffer_handle_t hndl_buffer, uintptr_t** cpu_ptr, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRGETCONSTCPUPTR,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrGetConstCpuPtr(hndl_buffer, (const void ** const)cpu_ptr);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRGETCONSTCPUPTR,
                            test_id, test_ret, opt_info);
}


/* ------------------------09_011_001 [success]------------------------ */
/* ------------------------09_011_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_011_003 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_011_004 [OSAL_RETURN_PAR]------------------------ */
static void Test09_011_001_004(osal_memory_buffer_handle_t hndl_buffer, size_t offset, size_t size, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRFLUSH,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrFlush(hndl_buffer, offset, size);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRFLUSH,
                            test_id, test_ret, opt_info);
}


/* ------------------------09_012_001 [success]------------------------ */
/* ------------------------09_012_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_012_003 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_012_004 [OSAL_RETURN_PAR]------------------------ */
static void Test09_012_001_004(osal_memory_buffer_handle_t hndl_buffer, size_t offset, size_t size, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRINVALIDATE,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrInvalidate(hndl_buffer, offset, size);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRINVALIDATE,
                            test_id, test_ret, opt_info);
}


/* ------------------------09_013_001 [success]------------------------ */
/* ------------------------09_013_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_013_003 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_013_004 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_013_005 [OSAL_RETURN_PAR]------------------------ */
static void Test09_013_001_005(osal_memory_buffer_handle_t hndl_buffer, uintptr_t *hw_addr, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    osal_axi_bus_id_t ax_id = 1;

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRGETHWADDR,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrGetHwAddr(hndl_buffer, ax_id, hw_addr);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRGETHWADDR,
                            test_id, test_ret, opt_info);
}

/* ------------------------09_014_001 [success]------------------------ */
/* ------------------------09_014_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_014_003 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_014_004 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_014_005 [OSAL_RETURN_PAR]------------------------ */
static void Test09_014_001_005(osal_memory_buffer_handle_t hndl_buffer, size_t *size, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRGETSIZE,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrGetSize(hndl_buffer, size);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRGETSIZE,
                            test_id, test_ret, opt_info);
}


/* ------------------------09_015_001 [success]------------------------ */
/* ------------------------09_015_002 [OSAL_RETURN_PAR]------------------------ */
/* ------------------------09_015_003 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_015_004 [OSAL_RETURN_PAR]------------------------ */
static void Test09_015_001_004(osal_memory_buffer_handle_t hndl_buffer, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRCHECKBUFFER,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrCheckBuffer(hndl_buffer);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRCHECKBUFFER,
                            test_id, test_ret, opt_info);
}

/* ------------------------09_015_005 [success]------------------------ */
static void Test09_015_005(osal_memory_manager_handle_t hndl_mmngr)
{
    int                          test_ret = OSAL_TEST_OK;
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
//    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    uint8_t *guard_page;
    uintptr_t* cpu_ptr = NULL;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
    size_t size      = 4096;
    size_t alignment = 4096;

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRCHECKBUFFER,
                                                             OSAL_TEST_ID005);

//    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRCHECKBUFFER, &config, &hndl_mmngr);
//    if (OSAL_RETURN_OK != osal_ret)
//    {
//        return;
//    }

	(void)R_OSAL_MmngrRegisterMonitor(hndl_mmngr, OSAL_MMNGR_EVENT_OVERFLOW_DETECTED,  osal_memory_mngr_cb, NULL);
	(void)R_OSAL_MmngrRegisterMonitor(hndl_mmngr, OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED, osal_memory_mngr_cb, NULL);

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRCHECKBUFFER, 0);
        return;
    }

    osal_ret = R_OSAL_MmngrGetCpuPtr(buffer_obj, (void **)&cpu_ptr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrGetCpuPtr() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRCHECKBUFFER, 0);
        return;
    }

    guard_page = cpu_ptr;
    memset((void *)(guard_page - 1000uL), 0xcc, 1);
    osal_ret = R_OSAL_MmngrCheckBuffer(buffer_obj);
    memset((void *)(guard_page - 1000uL), 0xaa, 1);
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRCHECKBUFFER,
                            OSAL_TEST_ID005, test_ret, opt_info);

    guard_page = cpu_ptr;
    memset((void *)(guard_page + 5000uL), 0xbb, 1);
    osal_ret = R_OSAL_MmngrCheckBuffer(buffer_obj);
    memset((void *)(guard_page + 5000uL), 0xaa, 1);
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRCHECKBUFFER,
                            OSAL_TEST_ID005, test_ret, opt_info);

    osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}

#if IT_DEBUG_ON
/* ------------------------09_016_001 [success]------------------------ */
/* ------------------------09_016_002 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_016_003 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_016_004 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_016_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test09_016_001_005(osal_memory_manager_handle_t hndl_mmngr, FILE * output, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRDEBUGMMNGRPRINT,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr, output);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRDEBUGMMNGRPRINT,
                            test_id, test_ret, opt_info);
}


/* ------------------------09_017_001 [success]------------------------ */
/* ------------------------09_017_002 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_017_003 [OSAL_RETURN_HANDLE]------------------------ */
/* ------------------------09_017_004 [OSAL_RETURN_UNSUPPORTED_OPERATION]------------------------ */
/* ------------------------09_017_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test09_017_001_005(osal_memory_buffer_handle_t hndl_buffer, FILE * output, int test_id)
{
    int             test_ret = OSAL_TEST_OK;
    e_osal_return_t osal_ret;
    e_osal_return_t expect_data;
    char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

    expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_MEMORY,
                                                             OSAL_TEST_MMNGRDEBUGBUFFERPRINT,
                                                             test_id);
    TIME_MEASURE_INIT
    TIME_MEASURE_START
    osal_ret = R_OSAL_MmngrDebugBufferPrint(hndl_buffer, output);
    TIME_MEASURE_STOP
    if (expect_data == osal_ret)
    {
        test_ret = OSAL_TEST_OK;
    }
    else
    {
        test_ret = OSAL_TEST_NG;
    }

    OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
    osal_test_result_output(OSAL_TEST_MEMORY, 
                            OSAL_TEST_MMNGRDEBUGBUFFERPRINT,
                            test_id, test_ret, opt_info);
}
#endif


/* 09_001: R_OSAL_MmngrOpen */
static void osal_test_R_OSAL_MmngrOpen()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;

    Test09_001_001_011(&hndl_mmngr, &config, OSAL_TEST_ID002);

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_open_preprocess(OSAL_TEST_MMNGROPEN, &config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    /* ------------------------Test------------------------ */
    config.max_allowed_allocations     = 100;
    config.max_registered_monitors_cbs = 100;
    Test09_001_001_011(&hndl_mmngr, &config, OSAL_TEST_ID001);

	R_OSAL_MmngrClose(hndl_mmngr);

	Test09_001_001_011(NULL,        &config, OSAL_TEST_ID003);
    Test09_001_001_011(&hndl_mmngr, NULL,    OSAL_TEST_ID004);

    config.mode = OSAL_MMNGR_ALLOC_MODE_INVALID;
    Test09_001_001_011(&hndl_mmngr, &config, OSAL_TEST_ID005);

    config.mode         = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit = 3 * 1024;
    Test09_001_001_011(&hndl_mmngr, &config,OSAL_TEST_ID006);

    config.memory_limit = OSAL_MEMORY_MANAGER_CFG_UNLIMITED + 1;
    Test09_001_001_011(&hndl_mmngr, &config, OSAL_TEST_ID007);

    config.memory_limit            = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations = 0;
    Test09_001_001_011(&hndl_mmngr, &config, OSAL_TEST_ID008);

    config.max_allowed_allocations = OSAL_MEMORY_MANAGER_CFG_UNLIMITED + 1;
    Test09_001_001_011(&hndl_mmngr, &config, OSAL_TEST_ID009);

    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_registered_monitors_cbs = OSAL_MEMORY_MANAGER_CFG_UNLIMITED + 1;
    Test09_001_001_011(&hndl_mmngr, &config, OSAL_TEST_ID010);

    config.max_registered_monitors_cbs = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    Test09_001_001_011(&hndl_mmngr, &config, OSAL_TEST_ID011);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_deinit_memory();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRCLOSE, 0);
        return;
    }
}

/* 09_002: R_OSAL_MmngrClose */
static void osal_test_R_OSAL_MmngrClose()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr = NULL;
    void *                       tmp_ptr;

    Test09_002_001_004(hndl_mmngr, OSAL_TEST_ID002);

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_open_preprocess(OSAL_TEST_MMNGRCLOSE, &config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    config.max_allowed_allocations     = 100;
    config.max_registered_monitors_cbs = 100;
    osal_ret = R_OSAL_MmngrOpen(&config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRCLOSE, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_002_001_004(NULL,       OSAL_TEST_ID003);

    tmp_ptr              = hndl_mmngr->p_ptr_self;
    hndl_mmngr->p_ptr_self = NULL;
    Test09_002_001_004(hndl_mmngr, OSAL_TEST_ID004);
    hndl_mmngr->p_ptr_self = tmp_ptr;

    Test09_002_001_004(hndl_mmngr, OSAL_TEST_ID001);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_deinit_memory();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRCLOSE, 0);
        return;
    }
}


/* 09_003: R_OSAL_MmngrAlloc */
static void osal_test_R_OSAL_MmngrAlloc()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    void *                       tmp_ptr;
    p_alloc_f_t                  tmp_func_ptr;
    size_t size      = 4096;
    size_t alignment = 4096;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_open_preprocess(OSAL_TEST_MMNGRALLOC, &config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT;
    config.memory_limit                = OSAL_MAX_MEMORY_LIMIT;
    config.max_allowed_allocations     = 1000;
    config.max_registered_monitors_cbs = 100;
    osal_ret = R_OSAL_MmngrOpen(&config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRALLOC, 0);
        return;
    }

    /* ------------------------Test------------------------ */
	(void)R_OSAL_MmngrRegisterMonitor(hndl_mmngr, OSAL_MMNGR_EVENT_ALLOCATE, osal_memory_mngr_cb, NULL);
    Test09_003_001_006(hndl_mmngr, size, alignment, &buffer_obj, OSAL_TEST_ID001);
    Test09_003_001_006(NULL,       size, alignment, &buffer_obj, OSAL_TEST_ID002);
    Test09_003_001_006(hndl_mmngr, size, alignment, NULL,        OSAL_TEST_ID003);

    tmp_ptr              = hndl_mmngr->p_ptr_self;
    hndl_mmngr->p_ptr_self = NULL;
    Test09_003_001_006(hndl_mmngr, size, alignment, &buffer_obj, OSAL_TEST_ID004);
    hndl_mmngr->p_ptr_self = tmp_ptr;

    tmp_func_ptr         = hndl_mmngr->p_allocate;
    hndl_mmngr->p_allocate = NULL;
    Test09_003_001_006(hndl_mmngr, size, alignment, &buffer_obj, OSAL_TEST_ID005);
    hndl_mmngr->p_allocate = tmp_func_ptr;

    size = 0;
    Test09_003_001_006(hndl_mmngr, size, alignment, &buffer_obj, OSAL_TEST_ID006);
    size = 4096;

    /* many allocate memory */
	Test09_003_007(hndl_mmngr, config);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRALLOC, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_004: R_OSAL_MmngrDealloc */
static void osal_test_R_OSAL_MmngrDealloc()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    void*                        tmp_ptr;
    p_dealloc_f_t                tmp_func_ptr;

    size_t size      = 4096;
    size_t alignment = 64;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRDEALLOC, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEALLOC, 0);
        return;
    }

    /* ------------------------Test------------------------ */
	(void)R_OSAL_MmngrRegisterMonitor(hndl_mmngr, OSAL_MMNGR_EVENT_DEALLOCATE, osal_memory_mngr_cb, NULL);
    Test09_004_001_005(NULL,       buffer_obj, OSAL_TEST_ID002);
    Test09_004_001_005(hndl_mmngr, NULL,       OSAL_TEST_ID003);

    tmp_func_ptr           = hndl_mmngr->p_deallocate;
    hndl_mmngr->p_deallocate = NULL;
    Test09_004_001_005(hndl_mmngr, buffer_obj, OSAL_TEST_ID004);
    hndl_mmngr->p_deallocate = tmp_func_ptr;

    tmp_ptr              = hndl_mmngr->p_ptr_self;
    hndl_mmngr->p_ptr_self = NULL;
    Test09_004_001_005(hndl_mmngr, buffer_obj, OSAL_TEST_ID005);
    hndl_mmngr->p_ptr_self = tmp_ptr;

    Test09_004_001_005(hndl_mmngr, buffer_obj, OSAL_TEST_ID001);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_close_post(OSAL_TEST_MMNGRDEALLOC, hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_005: R_OSAL_MmngrGetConfig */
static void osal_test_R_OSAL_MmngrGetConfig()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    p_getcfg_f_t                 tmp_func_ptr;

    /* ------------------------Initialize------------------------ */
    memset(&config, 0x00, sizeof(st_osal_mmngr_config_t));

    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRGETCONFIG, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_005_001_004(hndl_mmngr, &config, OSAL_TEST_ID001);
    Test09_005_001_004(NULL,       &config, OSAL_TEST_ID002);
    Test09_005_001_004(hndl_mmngr, NULL,    OSAL_TEST_ID003);

    tmp_func_ptr          = hndl_mmngr->p_getConfig;
    hndl_mmngr->p_getConfig = NULL;
    Test09_005_001_004(hndl_mmngr, &config, OSAL_TEST_ID004);
    hndl_mmngr->p_getConfig = tmp_func_ptr;

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_close_post(OSAL_TEST_MMNGRGETCONFIG, hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }
}


/* 09_006: R_OSAL_MmngrGetOsalMaxConfig */
static void osal_test_R_OSAL_MmngrGetOsalMaxConfig()
{
    e_osal_return_t        osal_ret;
    st_osal_mmngr_config_t config;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_init_memory();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIG, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_006_001_002(&config, OSAL_TEST_ID001);
    Test09_006_001_002(NULL,    OSAL_TEST_ID002);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_deinit_memory();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETOSALMAXCONFIG, 0);
        return;
    }
}


/* 09_007: R_OSAL_MmngrInitializeMmngrConfigSt */
static void osal_test_R_OSAL_MmngrInitializeMmngrConfigSt()
{
    e_osal_return_t        osal_ret;
    st_osal_mmngr_config_t config;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_init_memory();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Initialize() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINITIALIZEMMNGRCONFIGST, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_007_001_002(&config, OSAL_TEST_ID001);
    Test09_007_001_002(NULL,    OSAL_TEST_ID002);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_deinit_memory();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINITIALIZEMMNGRCONFIGST, 0);
        return;
    }
}


/* 09_008: R_OSAL_MmngrRegisterMonitor */
static void osal_test_R_OSAL_MmngrRegisterMonitor()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    void*                        tmp_ptr;
    p_reg_mon_f_t                tmp_func_ptr;
    int                          usr_data = 55; /* tentative */

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_open_preprocess(OSAL_TEST_MMNGRREGISTERMONITOR, &config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    config.max_allowed_allocations     = 100;
    config.max_registered_monitors_cbs = 100;
    osal_ret = R_OSAL_MmngrOpen(&config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRREGISTERMONITOR, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_008_001_005(hndl_mmngr, OSAL_MMNGR_EVENT_ALLOCATE, osal_memory_mngr_cb, (void*)&usr_data, OSAL_TEST_ID001);
    Test09_008_001_005(NULL,       OSAL_MMNGR_EVENT_ALLOCATE, osal_memory_mngr_cb, (void*)&usr_data, OSAL_TEST_ID002);
    Test09_008_001_005(hndl_mmngr, OSAL_MMNGR_EVENT_ALLOCATE, NULL,                (void*)&usr_data, OSAL_TEST_ID003);
    Test09_008_001_005(hndl_mmngr, OSAL_MMNGR_EVENT_ALLOCATE, osal_memory_mngr_cb, NULL,             OSAL_TEST_ID004);

    tmp_ptr              = hndl_mmngr->p_ptr_self;
    hndl_mmngr->p_ptr_self = NULL;
    Test09_008_001_005(hndl_mmngr, OSAL_MMNGR_EVENT_ALLOCATE, osal_memory_mngr_cb, (void*)&usr_data, OSAL_TEST_ID005);
    hndl_mmngr->p_ptr_self = tmp_ptr;

    Test09_008_001_005(hndl_mmngr, OSAL_MMNGR_EVENT_INVALID,   osal_memory_mngr_cb, (void*)&usr_data, OSAL_TEST_ID006);
    Test09_008_001_005(hndl_mmngr, OSAL_MMNGR_EVENT_ERROR + 1, osal_memory_mngr_cb, (void*)&usr_data, OSAL_TEST_ID007);

    tmp_func_ptr                = hndl_mmngr->p_registerMonitor;
    hndl_mmngr->p_registerMonitor = NULL;
    Test09_008_001_005(hndl_mmngr, OSAL_MMNGR_EVENT_ALLOCATE, osal_memory_mngr_cb, (void*)&usr_data, OSAL_TEST_ID008);
    hndl_mmngr->p_registerMonitor = tmp_func_ptr;
    
    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_close_post(OSAL_TEST_MMNGRREGISTERMONITOR, hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_009: R_OSAL_MmngrGetCpuPtr */
static void osal_test_R_OSAL_MmngrGetCpuPtr()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    uintptr_t*                   cpu_ptr;
    void*                        tmp_ptr;
    p_cpuptr_f_t                 tmp_func_ptr;

    size_t size      = 4096;
    size_t alignment = 64;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRGETCPUPTR, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCPUPTR, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_009_001_005(buffer_obj, &cpu_ptr, OSAL_TEST_ID001);
    Test09_009_001_005(NULL,       &cpu_ptr, OSAL_TEST_ID002);
    Test09_009_001_005(buffer_obj, NULL,     OSAL_TEST_ID003);

    tmp_func_ptr          = buffer_obj->p_getCpuPtr;
    buffer_obj->p_getCpuPtr = NULL;
    Test09_009_001_005(buffer_obj, &cpu_ptr, OSAL_TEST_ID004);
    buffer_obj->p_getCpuPtr = tmp_func_ptr;

    tmp_ptr               = buffer_obj->p_ptr_self;
    buffer_obj->p_ptr_self  = NULL;
    Test09_009_001_005(buffer_obj, &cpu_ptr, OSAL_TEST_ID005);
    buffer_obj->p_ptr_self  = tmp_ptr;

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRGETCPUPTR, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_010: R_OSAL_MmngrGetConstCpuPtr */
static void osal_test_R_OSAL_MmngrGetConstCpuPtr()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    uintptr_t*                   cpu_ptr;
    void*                        tmp_ptr;
    p_const_cpuptr_f_t           tmp_func_ptr;

    size_t size      = 4096;
    size_t alignment = 64;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRGETCONSTCPUPTR, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
                      OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETCONSTCPUPTR, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_010_001_005(buffer_obj, &cpu_ptr, OSAL_TEST_ID001);
    Test09_010_001_005(NULL,       &cpu_ptr, OSAL_TEST_ID002);
    Test09_010_001_005(buffer_obj, NULL,     OSAL_TEST_ID003);

    tmp_func_ptr               = buffer_obj->p_getConstCpuPtr;
    buffer_obj->p_getConstCpuPtr = NULL;
    Test09_010_001_005(buffer_obj, &cpu_ptr, OSAL_TEST_ID004);
    buffer_obj->p_getConstCpuPtr = tmp_func_ptr;

    tmp_ptr                    = buffer_obj->p_ptr_self;
    buffer_obj->p_ptr_self       = NULL;
    Test09_010_001_005(buffer_obj, &cpu_ptr, OSAL_TEST_ID005);
    buffer_obj->p_ptr_self       = tmp_ptr;

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRGETCONSTCPUPTR, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_011: R_OSAL_MmngrFlush */
static void osal_test_R_OSAL_MmngrFlush()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    void*                        tmp_ptr;
    p_flush_f_t                  tmp_func_ptr;

    size_t size      = 4096;
    size_t alignment = 64;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRFLUSH, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRFLUSH, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_011_001_004(buffer_obj, 0, size, OSAL_TEST_ID001);
    Test09_011_001_004(NULL,       0, size, OSAL_TEST_ID002);

    tmp_func_ptr      = buffer_obj->p_flush;
    buffer_obj->p_flush = NULL;
    Test09_011_001_004(buffer_obj, 0, size, OSAL_TEST_ID003);
    buffer_obj->p_flush = tmp_func_ptr;

    tmp_ptr              = buffer_obj->p_ptr_self;
    buffer_obj->p_ptr_self = NULL;
    Test09_011_001_004(buffer_obj, 0, size, OSAL_TEST_ID004);
    buffer_obj->p_ptr_self = tmp_ptr;

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRFLUSH, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_012: R_OSAL_MmngrInvalidate */
static void osal_test_R_OSAL_MmngrInvalidate()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    void*                        tmp_ptr;
    p_invalid_f_t                tmp_func_ptr;

    size_t size      = 4096;
    size_t alignment = 64;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRINVALIDATE, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRINVALIDATE, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_012_001_004(buffer_obj, 0, size, OSAL_TEST_ID001);
    Test09_012_001_004(NULL,       0, size, OSAL_TEST_ID002);

    tmp_func_ptr           = buffer_obj->p_invalidate;
    buffer_obj->p_invalidate = NULL;
    Test09_012_001_004(buffer_obj, 0, size, OSAL_TEST_ID003);
    buffer_obj->p_invalidate = tmp_func_ptr;

    tmp_ptr                = buffer_obj->p_ptr_self;
    buffer_obj->p_ptr_self   = NULL;
    Test09_012_001_004(buffer_obj, 0, size, OSAL_TEST_ID004);
    buffer_obj->p_ptr_self = tmp_ptr;

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRINVALIDATE, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_013: R_OSAL_MmngrGetHwAddr */
static void osal_test_R_OSAL_MmngrGetHwAddr()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    void*                        tmp_ptr;
    p_hwaddr_f_t                 tmp_func_ptr;

    size_t    size      = 0x10000000;
    size_t    alignment = 4096;
    uintptr_t hw_addr   = 0;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRGETHWADDR, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETHWADDR, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_013_001_005(buffer_obj, &hw_addr, OSAL_TEST_ID001);
    Test09_013_001_005(NULL,       &hw_addr, OSAL_TEST_ID002);

    tmp_func_ptr          = buffer_obj->p_getHwAddr;
    buffer_obj->p_getHwAddr = NULL;
    Test09_013_001_005(buffer_obj, &hw_addr, OSAL_TEST_ID003);
    buffer_obj->p_getHwAddr = tmp_func_ptr;

    tmp_ptr              = buffer_obj->p_ptr_self;
    buffer_obj->p_ptr_self = NULL;
    Test09_013_001_005(buffer_obj, &hw_addr, OSAL_TEST_ID004);
    buffer_obj->p_ptr_self = tmp_ptr;

    Test09_013_001_005(buffer_obj, NULL,     OSAL_TEST_ID005);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRGETHWADDR, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_014: R_OSAL_MmngrGetSize */
static void osal_test_R_OSAL_MmngrGetSize()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    void*                        tmp_ptr;
    p_size_f_t                   tmp_func_ptr;
    size_t size      = 4096;
    size_t alignment = 64;
    size_t osize     = 0;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRGETSIZE, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRGETSIZE, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_014_001_005(buffer_obj, &osize, OSAL_TEST_ID001);
    Test09_014_001_005(NULL,       &osize, OSAL_TEST_ID002);

    tmp_func_ptr        = buffer_obj->p_getSize;
    buffer_obj->p_getSize = NULL;
    Test09_014_001_005(buffer_obj, &osize, OSAL_TEST_ID003);
    buffer_obj->p_getSize = tmp_func_ptr;

    tmp_ptr              = buffer_obj->p_ptr_self;
    buffer_obj->p_ptr_self = NULL;
    Test09_014_001_005(buffer_obj, &osize, OSAL_TEST_ID004);
    buffer_obj->p_ptr_self = tmp_ptr;

    Test09_014_001_005(buffer_obj, NULL,   OSAL_TEST_ID005);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRGETSIZE, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_015: R_OSAL_MmngrCheckBuffer */
static void osal_test_R_OSAL_MmngrCheckBuffer()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    void*                        tmp_ptr;
    p_checkbuffer_f_t            tmp_func_ptr;

    size_t size      = 4096;
    size_t alignment = 64;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRCHECKBUFFER, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

	osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.", OSAL_TEST_MEMORY, OSAL_TEST_MMNGRCHECKBUFFER, 0);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_015_001_004(buffer_obj, OSAL_TEST_ID001);
    Test09_015_001_004(NULL,       OSAL_TEST_ID002);

    tmp_func_ptr            = buffer_obj->p_checkBuffer;
    buffer_obj->p_checkBuffer = NULL;
    Test09_015_001_004(buffer_obj, OSAL_TEST_ID003);
    buffer_obj->p_checkBuffer = tmp_func_ptr;

    tmp_ptr              = buffer_obj->p_ptr_self;
    buffer_obj->p_ptr_self = NULL;
    Test09_015_001_004(buffer_obj, OSAL_TEST_ID004);
    buffer_obj->p_ptr_self = tmp_ptr;

	/* over run & under run */
    Test09_015_005(hndl_mmngr);

    /* ------------------------Deinitialize------------------------ */
    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRCHECKBUFFER, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}

/* 09_016: R_OSAL_MmngrOpenMulti */
static void osal_test_R_OSAL_MmngrOpenMulti()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr[5]
        = {OSAL_MEMORY_MANAGER_HANDLE_INVALID, OSAL_MEMORY_MANAGER_HANDLE_INVALID,
           OSAL_MEMORY_MANAGER_HANDLE_INVALID, OSAL_MEMORY_MANAGER_HANDLE_INVALID,
           OSAL_MEMORY_MANAGER_HANDLE_INVALID};
    osal_memory_buffer_handle_t  buffer_obj[5]
        = {OSAL_MEMORY_BUFFER_HANDLE_INVALID, OSAL_MEMORY_BUFFER_HANDLE_INVALID,
           OSAL_MEMORY_BUFFER_HANDLE_INVALID, OSAL_MEMORY_BUFFER_HANDLE_INVALID,
           OSAL_MEMORY_BUFFER_HANDLE_INVALID};
    int i=0;

    printf("\n");

    /* open(0)->open(1)->alloc(0,0)->alloc(1,0) */
    printf("===== open(0)->open(1)->alloc(0,0)->alloc(1,0)\n");
    printf("R_OSAL_Initialize(): ");
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(0): ");
    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(0, &config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrOpen(0): ");
    osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config, 0, &hndl_mmngr[0]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrGetOsalMaxConfig(1): ");
    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(1, &config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");


    printf("R_OSAL_MmngrOpenFromRegionIdx(1): ");
    osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config, 1, &hndl_mmngr[1]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrGetOsalMaxConfig(2)expect RETURN_PAR: ");
    osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx(2, &config);
    if (OSAL_RETURN_PAR != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrOpenFromRegionIdx(2)expect RETURN_PAR: ");
    osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&config, 2, &hndl_mmngr[2]);
    if (OSAL_RETURN_PAR != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("===== open(0)->open(1) result\n");
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[0], stdout);
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[1], stdout);

    printf("R_OSAL_MmngrAlloc(region_0,128MB): ");
    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr[0], 128*1024*1024, 64, &buffer_obj[0]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrAlloc(region_0,128MB): ");
    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr[0], 128*1024*1024, 64, &buffer_obj[1]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("===== alloc(0,0) result\n");
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[0], stdout);
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[1], stdout);

    printf("R_OSAL_MmngrAlloc(region_1,100MB): ");
    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr[1], 100*1024*1024, 64, &buffer_obj[2]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrAlloc(region_1,100MB)expect RETURN_MEM:: ");
    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr[1], 100*1024*1024, 64, &buffer_obj[3]);
    if (OSAL_RETURN_MEM != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");


    printf("===== alloc(1,0) result\n");
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[0], stdout);
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[1], stdout);

    printf("R_OSAL_MmngrDealloc(0,0): ");
    osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr[0], buffer_obj[1]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrDealloc(0,1): ");
    osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr[0], buffer_obj[0]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");


    printf("===== dealloc(0,0) result\n");
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[0], stdout);
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[1], stdout);

    printf("R_OSAL_MmngrDealloc(1,0): ");
    osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr[1],buffer_obj[2]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("===== dealloc(1,0) result\n");
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[0], stdout);
    R_OSAL_MmngrDebugMmngrPrint(hndl_mmngr[1], stdout);

    printf("R_OSAL_MmngrClose(0): ");
    osal_ret = R_OSAL_MmngrClose(hndl_mmngr[0]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    printf("R_OSAL_MmngrClose(1): ");
    osal_ret = R_OSAL_MmngrClose(hndl_mmngr[1]);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    /* ------------------------Deinitialize------------------------ */
    printf("R_OSAL_Deinitialize(1): ");
    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("FAIL(%u)\n", osal_ret);
        return;
    }
    printf("OK\n ");

    return;
}

#if IT_DEBUG_ON
/* 09_017: R_OSAL_MmngrDebugMmngrPrint */
static void osal_test_R_OSAL_MmngrDebugMmngrPrint()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    void*                        tmp_ptr;
    p_mprint_t                   tmp_func_ptr;
    FILE * output;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_open_preprocess(OSAL_TEST_MMNGRDEBUGMMNGRPRINT, &config);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    output = fopen(OSAL_DEBUG_LOG_PATH, "a+");
    if (NULL == output)
    {
        OSAL_TEST_ERR("fopen() error.",
            OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, 0);
        return;
    }

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = 0x10000000;
    config.max_allowed_allocations     = 256;
    config.max_registered_monitors_cbs = 10;
    osal_ret = R_OSAL_MmngrOpen(&config, &hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrOpen() error.",
            OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGMMNGRPRINT, 0);
        fclose(output);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_016_001_005(hndl_mmngr, output, OSAL_TEST_ID001);
    Test09_016_001_005(NULL,       output, OSAL_TEST_ID002);
    Test09_016_001_005(hndl_mmngr, NULL,   OSAL_TEST_ID003);

    tmp_func_ptr        = hndl_mmngr->p_print;
    hndl_mmngr->p_print = NULL;
    Test09_016_001_005(hndl_mmngr, output, OSAL_TEST_ID004);
    hndl_mmngr->p_print = tmp_func_ptr;

    tmp_ptr              = hndl_mmngr->p_ptr_self;
    hndl_mmngr->p_ptr_self = NULL;
    Test09_016_001_005(hndl_mmngr, output, OSAL_TEST_ID005);
    hndl_mmngr->p_ptr_self = tmp_ptr;

    /* ------------------------Deinitialize------------------------ */
    fclose(output);

    osal_ret = osal_test_mmngr_close_post(OSAL_TEST_MMNGRDEBUGMMNGRPRINT, hndl_mmngr);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}


/* 09_018: R_OSAL_MmngrDebugBufferPrint */
static void osal_test_R_OSAL_MmngrDebugBufferPrint()
{
    e_osal_return_t              osal_ret;
    st_osal_mmngr_config_t       config;
    osal_memory_manager_handle_t hndl_mmngr;
    osal_memory_buffer_handle_t  buffer_obj;
    void*                        tmp_ptr;
    p_bprint_t                   tmp_func_ptr;

    size_t size      = 4096;
    size_t alignment = 4096;
    FILE * output;

    /* ------------------------Initialize------------------------ */
    osal_ret = osal_test_mmngr_alloc_preprocess(OSAL_TEST_MMNGRDEBUGBUFFERPRINT, &config, &hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        return;
    }

    output = fopen(OSAL_DEBUG_LOG_PATH, "a+");
    if (NULL == output)
    {
        OSAL_TEST_ERR("fopen() error.",
            OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, 0);
        return;
    }

    osal_ret = R_OSAL_MmngrAlloc(hndl_mmngr, size, alignment, &buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrAlloc() error.",
            OSAL_TEST_MEMORY, OSAL_TEST_MMNGRDEBUGBUFFERPRINT, 0);
        fclose(output);
        return;
    }

    /* ------------------------Test------------------------ */
    Test09_017_001_005(buffer_obj, output, OSAL_TEST_ID001);
    Test09_017_001_005(NULL,       output, OSAL_TEST_ID002);
    Test09_017_001_005(buffer_obj, NULL,   OSAL_TEST_ID003);

    tmp_func_ptr      = buffer_obj->p_print;
    buffer_obj->p_print = NULL;
    Test09_017_001_005(buffer_obj, output, OSAL_TEST_ID004);
    buffer_obj->p_print = tmp_func_ptr;

    tmp_ptr              = buffer_obj->p_ptr_self;
    buffer_obj->p_ptr_self = NULL;
    Test09_017_001_005(buffer_obj, output, OSAL_TEST_ID005);
    buffer_obj->p_ptr_self = tmp_ptr;

    /* ------------------------Deinitialize------------------------ */
    fclose(output);

    osal_ret = osal_test_mmngr_dealloc_post(OSAL_TEST_MMNGRDEBUGBUFFERPRINT, hndl_mmngr, buffer_obj);
    if (osal_ret != OSAL_RETURN_OK)
    {
        return;
    }
}
#endif
