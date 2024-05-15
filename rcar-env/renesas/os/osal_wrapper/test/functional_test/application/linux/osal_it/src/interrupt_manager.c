/**
 * @file interrupt_manager.c
 * @brief OSAL Interrupt Manager evaluation file.
 * @author Khanh Hoang
 * @date Aug 04, 2020
 */

/* include */
#define _GNU_SOURCE
#include <pthread.h>
#include "osal_test.h"
#include "osal_test_common.h"
#include "interrupt_manager.h"
#include "target/common/dev_cfg/dynamic/r_osal_dev_cfg.h"
#include "target/linux/r_osal_os_types.h"

/* prototype */
static void osal_test_R_OSAL_InterruptGetNumOfIrqChannels();
static void osal_test_R_OSAL_InterruptRegisterIsr();
static void osal_test_R_OSAL_InterruptUnregisterIsr();
static void osal_test_R_OSAL_InterruptEnableIsr();
static void osal_test_R_OSAL_InterruptDisableIsr();
static void osal_test_R_OSAL_InterruptSetIsrPriority();
static void osal_test_R_OSAL_InterruptIsISRContext();

void osal_test_interrupt_manager_display_menu()
{
	printf("*** OSAL Interrupt Manager ***\n");
	printf("***  05_001: R_OSAL_InterruptGetNumOfIrqChannels  ***\n");
	printf("***  05_002: R_OSAL_InterruptRegisterIsr          ***\n");
	printf("***  05_003: R_OSAL_InterruptUnregisterIsr        ***\n");
	printf("***  05_004: R_OSAL_InterruptEnableIsr            ***\n");
	printf("***  05_005: R_OSAL_InterruptDisableIsr           ***\n");
	printf("***  05_006: R_OSAL_InterruptSetIsrPriority       ***\n");
	printf("***  05_007: R_OSAL_InterruptIsISRContext         ***\n");
	printf("***  05_100: interrupt manager module all exec    ***\n\n");
	return;
}

void osal_test_interrupt_manager_menu(int child)
{
	int test_number = child;
	unsigned int test_count = 1;
	unsigned int loop_count = 0;

	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number	= OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS;
		test_count	= OSAL_TEST_INTERRUPT_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number)
		{
			case OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS:
				osal_test_R_OSAL_InterruptGetNumOfIrqChannels();
				break;
			case OSAL_TEST_INTERRUPTREGISTERISR:
				osal_test_R_OSAL_InterruptRegisterIsr();
				break;
			case OSAL_TEST_INTERRUPTUNREGISTERISR:
				osal_test_R_OSAL_InterruptUnregisterIsr();
				break;
			case OSAL_TEST_INTERRUPTENABLEISR:
				osal_test_R_OSAL_InterruptEnableIsr();
				break;
			case OSAL_TEST_INTERRUPTDISABLEISR:
				osal_test_R_OSAL_InterruptDisableIsr();
				break;
			case OSAL_TEST_INTERRUPTSETISRPRIORITY:
				osal_test_R_OSAL_InterruptSetIsrPriority();
				break;
			case OSAL_TEST_INTERRUPTISISRCONTEXT:
				osal_test_R_OSAL_InterruptIsISRContext();
				break;
			default:
				printf("That menu is not available. \n\n");
				break;
		}
		test_number++;
	}
	return;
}

static void osal_test_interrupt_sample(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
	(void)device_handle;
	(void)irq_channel;
	(void)irq_arg;
	R_OSAL_ThreadSleepForTimePeriod(1);
	return;
}

/* ------------------------05_001_001 [success]------------------------ */
static void Test05_001_001()
{
	e_osal_return_t			osal_ret;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t			expect_data;
	int						test_ret = OSAL_TEST_OK;

	osal_device_handle_t		handle;
	st_osal_device_control_t*	p_control;
	size_t						numOfChannels;

#if defined(RCAR_S4) || defined(RCAR_V4H) || defined(RCAR_V4M)
	const char					device_id[]	= "caiplite_00";
#else
	const char					device_id[]	= "ims_00";
#endif

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
															OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	if (OSAL_RETURN_OK == osal_ret)
	{
		p_control	= (st_osal_device_control_t *)handle;
		p_control->handle_id = OSAL_INVALID_HANDLE_ID;

		osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(handle, &numOfChannels);
		if (expect_data != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptGetNumOfIrqChannels() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}

		p_control->handle_id = OSAL_IO_HANDLE_ID;
	}
	TIME_MEASURE_STOP

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	return;
}

/* ------------------------05_002_001 [success]------------------------ */
static void Test05_002_001()
{
	e_osal_return_t			osal_ret;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t			expect_data;
	int						test_ret = OSAL_TEST_OK;
	int						linux_ret;

	osal_device_handle_t		handle;
	st_osal_device_control_t*	p_control;
	st_osal_os_thread_t*		p_thread;
	char						thread_name[16];

#if defined(RCAR_S4)
	const char					device_id[]		= "caiplite_00";
	const char					expect_name[]	= "int_task933";
#elif defined(RCAR_V4H) || defined(RCAR_V4M)
	const char					device_id[]		= "caiplite_00";
	const char					expect_name[]	= "int_task900";
#else
	const char					device_id[]		= "ims_00";
	const char					expect_name[]	= "int_task192";
#endif

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
															OSAL_TEST_INTERRUPTREGISTERISR,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_InterruptRegisterIsr(handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10, osal_test_interrupt_sample, NULL);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	if (OSAL_RETURN_OK == osal_ret)
	{
		p_control	= (st_osal_device_control_t *)handle;
		p_thread	= (st_osal_os_thread_t *)p_control->int_handle[0].irq_thread;

		linux_ret = pthread_getname_np(p_thread->thread, thread_name, sizeof(thread_name));
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("pthread_getname_np() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_TEST_OK == test_ret)
	{
		printf("thread_name=%s\n", thread_name);

		linux_ret = strcmp(expect_name, thread_name);
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("strcmp() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	osal_ret = R_OSAL_InterruptUnregisterIsr(handle, 0, osal_test_interrupt_sample);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTREGISTERISR,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	return;
}

/* ------------------------05_002_002 [success]------------------------ */
static void Test05_002_002()
{
	e_osal_return_t			osal_ret;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t			expect_data;
	int						test_ret = OSAL_TEST_OK;
	int						linux_ret;

	osal_device_handle_t		handle;
	st_osal_device_control_t*	p_control;
	st_osal_os_thread_t*		p_thread;
	char						thread_name[16];

#if defined(RCAR_S4)
	const char					device_id[]		= "caiplite_01";
	const char					expect_name[]	= "int_task935";
#elif defined(RCAR_V4H) || defined(RCAR_V4M)
	const char					device_id[]		= "caiplite_01";
	const char					expect_name[]	= "int_task902";
#else
	const char					device_id[]		= "ims_01";
	const char					expect_name[]	= "int_task193";
#endif

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
															OSAL_TEST_INTERRUPTREGISTERISR,
															OSAL_TEST_ID002);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_InterruptRegisterIsr(handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10, osal_test_interrupt_sample, NULL);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
			test_ret = OSAL_TEST_NG;
		}
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	if (OSAL_RETURN_OK == osal_ret)
	{
		p_control	= (st_osal_device_control_t *)handle;
		p_thread	= (st_osal_os_thread_t *)p_control->int_handle[0].irq_thread;

		linux_ret = pthread_getname_np(p_thread->thread, thread_name, sizeof(thread_name));
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("pthread_getname_np() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_TEST_OK == test_ret)
	{
		printf("thread_name=%s\n", thread_name);

		linux_ret = strcmp(expect_name, thread_name);
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("strcmp() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	osal_ret = R_OSAL_InterruptUnregisterIsr(handle, 0, osal_test_interrupt_sample);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTREGISTERISR,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	return;
}

/* ------------------------05_002_003 [success]------------------------ */
static void Test05_002_003()
{
	e_osal_return_t			osal_ret;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t			expect_data;
	int						test_ret = OSAL_TEST_OK;
	int						linux_ret;

	osal_device_handle_t		handle;
	st_osal_device_control_t*	p_control;
	st_osal_os_thread_t*		p_thread;
	char						thread_name[16];

#if defined(RCAR_S4)
	const char					device_id[]		= "caiplite_02";
	const char					expect_name[]	= "int_task937";
#elif defined(RCAR_V4H) || defined(RCAR_V4M)
	const char					device_id[]		= "caiplite_02";
	const char					expect_name[]	= "int_task904";
#else
	const char					device_id[]		= "ims_02";
	const char					expect_name[]	= "int_task194";
#endif

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
															OSAL_TEST_INTERRUPTREGISTERISR,
															OSAL_TEST_ID003);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_InterruptRegisterIsr(handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10, osal_test_interrupt_sample, NULL);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
			test_ret = OSAL_TEST_NG;
		}
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	if (OSAL_RETURN_OK == osal_ret)
	{
		p_control	= (st_osal_device_control_t *)handle;
		p_thread	= (st_osal_os_thread_t *)p_control->int_handle[0].irq_thread;

		linux_ret = pthread_getname_np(p_thread->thread, thread_name, sizeof(thread_name));
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("pthread_getname_np() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_TEST_OK == test_ret)
	{
		printf("thread_name=%s\n", thread_name);

		linux_ret = strcmp(expect_name, thread_name);
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("strcmp() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	osal_ret = R_OSAL_InterruptUnregisterIsr(handle, 0, osal_test_interrupt_sample);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTREGISTERISR,
							OSAL_TEST_ID003,
							test_ret, opt_info);
	return;
}

/* ------------------------05_002_004 [success]------------------------ */
static void Test05_002_004()
{
	e_osal_return_t			osal_ret;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t			expect_data;
	int						test_ret = OSAL_TEST_OK;
	int						linux_ret;

	osal_device_handle_t		handle;
	st_osal_device_control_t*	p_control;
	st_osal_os_thread_t*		p_thread;
	cpu_set_t					cpu_set;

#if defined(RCAR_S4) || defined(RCAR_V4H) || defined(RCAR_V4M)
	const char					device_id[]	= "caiplite_00";
#else
	const char					device_id[]	= "ims_00";
#endif

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
															OSAL_TEST_INTERRUPTREGISTERISR,
															OSAL_TEST_ID004);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_InterruptRegisterIsr(handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10, osal_test_interrupt_sample, NULL);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
			test_ret = OSAL_TEST_NG;
		}
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	if (OSAL_RETURN_OK == osal_ret)
	{
		p_control	= (st_osal_device_control_t *)handle;
		p_thread	= (st_osal_os_thread_t *)p_control->int_handle[0].irq_thread;

		linux_ret = pthread_getaffinity_np(p_thread->thread, sizeof(cpu_set), &cpu_set);
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("pthread_getaffinity_np() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
			test_ret = OSAL_TEST_NG;
		}
	}

	/* affinity is 0x1 */
	/* CPU0 : run */
	/* CPU1 : not run */
	if (OSAL_TEST_OK == test_ret)
	{
		if (0 == CPU_ISSET(0, &cpu_set))
		{
			test_ret = OSAL_TEST_NG;
		}

		if (0 != CPU_ISSET(1, &cpu_set))
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	osal_ret = R_OSAL_InterruptUnregisterIsr(handle, 0, osal_test_interrupt_sample);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTREGISTERISR,
							OSAL_TEST_ID004,
							test_ret, opt_info);
	return;
}

/* ------------------------05_002_005 [success]------------------------ */
static void Test05_002_005()
{
	e_osal_return_t			osal_ret;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t			expect_data;
	int						test_ret = OSAL_TEST_OK;
	int						linux_ret;

	osal_device_handle_t		handle;
	st_osal_device_control_t*	p_control;
	st_osal_os_thread_t*		p_thread;
	cpu_set_t					cpu_set;

#if defined(RCAR_S4) || defined(RCAR_V4H) || defined(RCAR_V4M)
	const char					device_id[]	= "caiplite_01";
#else
	const char					device_id[]	= "ims_01";
#endif

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
															OSAL_TEST_INTERRUPTREGISTERISR,
															OSAL_TEST_ID005);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_InterruptRegisterIsr(handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10, osal_test_interrupt_sample, NULL);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
			test_ret = OSAL_TEST_NG;
		}
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	if (OSAL_RETURN_OK == osal_ret)
	{
		p_control	= (st_osal_device_control_t *)handle;
		p_thread	= (st_osal_os_thread_t *)p_control->int_handle[0].irq_thread;

		linux_ret = pthread_getaffinity_np(p_thread->thread, sizeof(cpu_set), &cpu_set);
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("pthread_getaffinity_np() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
			test_ret = OSAL_TEST_NG;
		}
	}

	/* affinity is 0x3 */
	/* CPU0 : run */
	/* CPU1 : run */
	if (OSAL_TEST_OK == test_ret)
	{
		if (0 == CPU_ISSET(0, &cpu_set))
		{
			test_ret = OSAL_TEST_NG;
		}

		if (0 == CPU_ISSET(1, &cpu_set))
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	osal_ret = R_OSAL_InterruptUnregisterIsr(handle, 0, osal_test_interrupt_sample);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTREGISTERISR,
							OSAL_TEST_ID005,
							test_ret, opt_info);
	return;
}

/* ------------------------05_002_006 [success]------------------------ */
static void Test05_002_006()
{
	e_osal_return_t			osal_ret;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	e_osal_return_t			expect_data;
	int						test_ret = OSAL_TEST_OK;
	int						linux_ret;

	osal_device_handle_t		handle;
	st_osal_device_control_t*	p_control;
	st_osal_os_thread_t*		p_thread;
	cpu_set_t					cpu_set;

#if defined(RCAR_S4) || defined(RCAR_V4H) || defined(RCAR_V4M)
	const char					device_id[]	= "caiplite_02";
#else
	const char					device_id[]	= "ims_02";
#endif

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
															OSAL_TEST_INTERRUPTREGISTERISR,
															OSAL_TEST_ID006);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_InterruptRegisterIsr(handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10, osal_test_interrupt_sample, NULL);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
			test_ret = OSAL_TEST_NG;
		}
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	if (OSAL_RETURN_OK == osal_ret)
	{
		p_control	= (st_osal_device_control_t *)handle;
		p_thread	= (st_osal_os_thread_t *)p_control->int_handle[0].irq_thread;

		linux_ret = pthread_getaffinity_np(p_thread->thread, sizeof(cpu_set), &cpu_set);
		if (0 != linux_ret)
		{
			OSAL_TEST_ERR("pthread_getaffinity_np() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
			test_ret = OSAL_TEST_NG;
		}
	}

	/* affinity is 0x0 */
	/* CPU0 : run */
	/* CPU1 : run */
	if (OSAL_TEST_OK == test_ret)
	{
		if (0 == CPU_ISSET(0, &cpu_set))
		{
			test_ret = OSAL_TEST_NG;
		}

		if (0 == CPU_ISSET(1, &cpu_set))
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	TIME_MEASURE_STOP

	osal_ret = R_OSAL_InterruptUnregisterIsr(handle, 0, osal_test_interrupt_sample);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTREGISTERISR,
							OSAL_TEST_ID006,
							test_ret, opt_info);
	return;
}

/* 05_001: R_OSAL_InterruptGetNumOfIrqChannels */
static void osal_test_R_OSAL_InterruptGetNumOfIrqChannels()
{
	Test05_001_001();
	return;
}

/* 05_002: R_OSAL_InterruptRegisterIsr */
static void osal_test_R_OSAL_InterruptRegisterIsr(){

	Test05_002_001();
	Test05_002_002();
	Test05_002_003();
	Test05_002_004();
	Test05_002_005();
	Test05_002_006();
	return;
}

/* 05_003: R_OSAL_InterruptUnregisterIsr */
static void osal_test_R_OSAL_InterruptUnregisterIsr()
{
	// There are no test cases for this API.
	return;
}

/* 05_004: R_OSAL_InterruptEnableIsr */
static void osal_test_R_OSAL_InterruptEnableIsr()
{
	// There are no test cases for this API.
	return;
}

/* 05_005: R_OSAL_InterruptDisableIsr */
static void osal_test_R_OSAL_InterruptDisableIsr()
{
	// There are no test cases for this API.
	return;
}

/* 05_006: R_OSAL_InterruptSetIsrPriority */
static void osal_test_R_OSAL_InterruptSetIsrPriority()
{
	// There are no test cases for this API.
	return;

}

/* 05_007: R_OSAL_InterruptIsISRContext */
static void osal_test_R_OSAL_InterruptIsISRContext()
{
	// There are no test cases for this API.
	return;
}
