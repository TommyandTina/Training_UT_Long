/**
 * @file interrupt_manager.c
 * @brief OSAL Interrupt Manager evaluation file.
 * @author Khanh Hoang
 * @date Aug 04, 2020
 */

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcar-xos/osal/r_osal.h"
#include "osal_test.h"
#include "osal_test_common.h"
#include "interrupt_manager.h"
//#include "rcar-xos/tmu/tmu.h"

/* prototype */
static void osal_test_R_OSAL_InterruptGetNumOfIrqChannels();
static void osal_test_R_OSAL_InterruptRegisterIsr();
static void osal_test_R_OSAL_InterruptUnregisterIsr();
static void osal_test_R_OSAL_InterruptEnableIsr();
static void osal_test_R_OSAL_InterruptDisableIsr();
static void osal_test_R_OSAL_InterruptSetIsrPriority();
static void osal_test_R_OSAL_InterruptIsISRContext();

volatile int g_int_cb_flag = 0;
osal_device_handle_t 	g_dev_handle;

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

}

e_osal_return_t osal_init_interrupt(void)
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

e_osal_return_t osal_deinit_interrupt(void)
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

void osal_test_interrupt_manager_menu(int child)
{

	int test_number = child;
	unsigned int test_count  = 1;
	unsigned int loop_count  = 0;

	/* Reset stub flags */
	//memset(&g_stub_flag, 0, sizeof(st_stub));
	
	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number = OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS;
		test_count  = OSAL_TEST_INTERRUPT_MAX;
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
}

/* test CB func */
void osal_test_cb_sample(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{

	printf("%s() user call back function exec!! dev_handle:%p irq_channel:%ld  irq_arg:%p \n", 
	    __func__, (void *)device_handle, irq_channel, irq_arg);

	return;
}

/* test CB func */
void osal_test_unresgister_dummy(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{

	printf("%s() user call back function exec!! dev_handle:%p irq_channel:%ld  irq_arg:%p \n", 
	    __func__, (void *)device_handle, irq_channel, irq_arg);

	return;
}

/* test CB func */
void osal_test_cb_long_wait(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{
    (void)device_handle;
    (void)irq_channel;
    (void)irq_arg;

#if 0
	printf("%s() user call back function exec!! dev_handle:%p irq_channel:%ld  irq_arg:%p \n",
	    __func__, (void *)device_handle, irq_channel, irq_arg);
#endif

    R_OSAL_ThreadSleepForTimePeriod(300);

	return;
}

/* ------------------------05_001_001 [Success]------------------------ */
static void Test05_001_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	size_t              	num_channels = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
														   OSAL_TEST_ID001);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (num_channels == 1))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------05_001_002 [Success]------------------------ */
static void Test05_001_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	size_t              	num_channels = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
														   OSAL_TEST_ID002);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID002);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (num_channels == 1))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------05_001_003 [Success]------------------------ */
static void Test05_001_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	size_t              	num_channels = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
														   OSAL_TEST_ID003);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (num_channels == 1))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID003,
							test_ret, opt_info);
	
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------05_001_004 [Success]------------------------ */
static void Test05_001_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	size_t              	num_channels = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
														   OSAL_TEST_ID004);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (num_channels == 1))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID004,
							test_ret, opt_info);
	
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------05_001_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test05_001_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t              	num_channels = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
														   OSAL_TEST_ID005);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(NULL, &num_channels);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID005,
							test_ret, opt_info);
}

/* ------------------------05_001_006 [OSAL_RETURN_PAR]------------------------ */
static void Test05_001_006()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
														   OSAL_TEST_ID006);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID006);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID006,
							test_ret, opt_info);
	
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------05_001_007 [OSAL_RETURN_STATE]------------------------ */
static void Test05_001_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	size_t              	num_channels = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
														   OSAL_TEST_ID007);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_interrupt();
    if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID007);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}

#if 0
void Test05_001_008_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t              	num_channels = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
														   OSAL_TEST_ID008);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(g_dev_handle, &num_channels);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (num_channels == 5))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,
							OSAL_TEST_ID008,
							test_ret, opt_info);
	
	g_int_cb_flag = 1;
}

/* ------------------------05_001_008 [ISR: success]------------------------ */
static void Test05_001_008()
{
	e_osal_return_t	osal_ret;
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID008);
		return;
	}

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_001_008_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID008);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID008);
		return;
	}

	osal_ret = osal_deinit_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, OSAL_TEST_ID008);
		return;
	}
}
#endif
/* ------------------------05_002_001 [Success]------------------------ */
static void Test05_002_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID001);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);	
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID001,
								test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR,
				OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------05_002_002 [Success]------------------------ */
static void Test05_002_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID002);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,NULL);
	TIME_MEASURE_STOP
	
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID002,
								test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR,
				OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------05_002_003 [Success]------------------------ */
static void Test05_002_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID003);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID003,
								test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR,
				OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------05_002_004 [Success]------------------------ */
static void Test05_002_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID004);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID004,
								test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR,
				OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------05_002_005 [Success]------------------------ */
static void Test05_002_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID005);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID005,
								test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR,
				OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------05_002_006 [OSAL_RETURN_HANDLE]------------------------ */
static void Test05_002_006() /* Add July 12, 2020 */
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID006);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("imp_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID006,
								test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------05_002_007 [OSAL_RETURN_HANDLE]------------------------ */
static void Test05_002_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID007);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(NULL, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);	
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID007,
								test_ret, opt_info);
}

/* ------------------------05_002_008 [OSAL_RETURN_PAR]------------------------ */
static void Test05_002_008()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID008);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID007);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 100, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);	
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID008,
								test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------05_002_009 [OSAL_RETURN_PAR]------------------------ */
static void Test05_002_009()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID009);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID009);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10, NULL, (void *)&arg);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID009,
								test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID009);
		return;
	}
}

/* ------------------------05_002_010 [OSAL_RETURN_BUSY]------------------------ */
static void Test05_002_010()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID010);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
					OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID010);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			
			TIME_MEASURE_INIT
			TIME_MEASURE_START
			osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
			TIME_MEASURE_STOP
			if (osal_ret == expect_data)
			{
				test_ret = OSAL_TEST_OK;
			}
			else
			{
				test_ret = OSAL_TEST_NG;
			}
		}
		
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID010,
								test_ret, opt_info);

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR,
				OSAL_TEST_ID010);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR,
				OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID010);
		return;
	}
}

/* ------------------------05_002_011 [OSAL_RETURN_BUSY]------------------------ */
static void Test05_002_011()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID011);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID011);
		return;
	}


	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID011);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID011,
								test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR,
				OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID011);
		return;
	}
}

/* ------------------------05_002_012 [OSAL_RETURN_STATE]------------------------ */
static void Test05_002_012()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID012);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_interrupt();
    if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID012);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);	
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID012,
								test_ret, opt_info);
}

#if 0
/* ------------------------05_002_013 [OSAL_RETURN_DEV]------------------------ */
static void Test05_002_013()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID013);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID013);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID013);
		return;
	}

	/* Set stub flag */
	g_stub_flag.drvISR_SetupSPI = drvISR_BUS_INTERFACE_FAULT;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.drvISR_SetupSPI = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID013,
								test_ret, opt_info);

    osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
    if (osal_ret != OSAL_RETURN_OK)
    {
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID013);
        return;
    }


	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID013);
		return;
	}
}

/* ------------------------05_002_014 [OSAL_RETURN_FAIL]------------------------ */
static void Test05_002_014()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID014);
	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID014);
		return;
	}

	/* Set stub flag */
	g_stub_flag.drvISR_SetupSPI = drvISR_FAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.drvISR_SetupSPI = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID014,
								test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID014);
		return;
	}
	osal_ret = osal_deinit_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID014);
		return;
	}
}
/* ------------------------05_002_015 [OSAL_RETURN_FAIL]------------------------ */
static void Test05_002_015()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTREGISTERISR,
														   OSAL_TEST_ID015);
	
	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID015);
		return;
	}

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID015);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT,
								OSAL_TEST_INTERRUPTREGISTERISR,
								OSAL_TEST_ID015,
								test_ret, opt_info);

	osal_ret = osal_deinit_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID015);
		return;
	}
}
#endif
/* ------------------------05_003_001 [Success]------------------------ */
static void Test05_003_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID001);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID001);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID001,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------05_003_002 [Success]------------------------ */
static void Test05_003_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID002);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID002);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID002,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------05_003_003 [Success]------------------------ */
static void Test05_003_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID003);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID003);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID003,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------05_003_004 [Success]------------------------ */
static void Test05_003_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID004);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID004);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID004,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------05_003_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test05_003_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID005);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(NULL, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID005,
						test_ret, opt_info);
}

/* ------------------------05_003_006 [OSAL_RETURN_PAR]------------------------ */
static void Test05_003_006()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID006);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID006);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 100, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID006,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------05_003_007 [OSAL_RETURN_PAR]------------------------ */
static void Test05_003_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID007);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID007);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID007,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------05_003_008 [OSAL_RETURN_PAR]------------------------ */
static void Test05_003_008()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID008);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID008);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID008,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------05_003_009 [OSAL_RETURN_PAR]------------------------ */
static void Test05_003_009()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID009);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID009);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_unresgister_dummy);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID009,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID009);
		return;
	}
}

/* ------------------------05_003_010 [OSAL_RETURN_BUSY]------------------------ */
static void Test05_003_010()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID010);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
					OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID010);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			TIME_MEASURE_INIT
			TIME_MEASURE_START
			osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
			TIME_MEASURE_STOP
			if (osal_ret == expect_data)
			{
				test_ret = OSAL_TEST_OK;
			}
			else
			{
				test_ret = OSAL_TEST_NG;
			}
		}
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID010,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR,
				OSAL_TEST_ID010);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR,
				OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID010);
		return;
	}
}

/* ------------------------05_003_011 [OSAL_RETURN_PAR]------------------------ */
static void Test05_003_011()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID011);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID011);
		return;
	}						   

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID011);
		test_ret = OSAL_TEST_NG;
	}						   
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID011,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID011);
		return;
	}
}

#if 0
/* ------------------------05_003_012 [Success]------------------------ */
static void Test05_003_012() /* Add July 11, 2020 */
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	st_osal_time_t p_time_stamp;
	st_osal_time_t p_time_stamp2;
	osal_nano_sec_t nano_time;

    size_t num;
    uint32_t count = 0;
    st_osal_interrupt_thread_control_t *control;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID012);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_long_wait,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID012);
		return;
	}

	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &p_time_stamp);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_long_wait);
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &p_time_stamp2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID012);
		return;
	}
	R_OSAL_ClockTimeCalculateTimeDifference(&p_time_stamp2, &p_time_stamp, &nano_time);
	printf("past time:%lu\n", nano_time);

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_long_wait,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisbleIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID012);
		return;
	}

	/* Force notification to interrupt_thread */
	num = R_OSAL_RCFG_GetNumOfInterruptThread();

	for (count = 0; count < num; count++)
	{
		R_OSAL_RCFG_GetInterruptThreadInfo(count, &control);
		if (NULL != control)
		{
			if (dev_handle == control->deviceHandle)
			{
				xTaskNotify(control->thread, 0, eNoAction);
				break;
			}
		}
	}

	R_OSAL_ThreadSleepForTimePeriod(10);

	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &p_time_stamp);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_long_wait);
	TIME_MEASURE_STOP

	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &p_time_stamp2);
	R_OSAL_ClockTimeCalculateTimeDifference(&p_time_stamp2, &p_time_stamp, &nano_time);
	printf("past time:%lu\n", nano_time);
		printf("check time stamp!! About 300msec longer than normal unregist\n");

	if (osal_ret == expect_data)
	{
		if ((250000000 <= nano_time) && (340000000 >= nano_time))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID012,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID012);
		return;
	}
}
#endif
/* ------------------------05_003_013 [OSAL_RETURN_STATE]------------------------ */
static void Test05_003_013()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT,
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID013);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_interrupt();
    if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID013);
		return;
	}				   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID013,
						test_ret, opt_info);
}

#if 0
/* ------------------------05_003_014 [OSAL_RETURN_DEV]------------------------ */
static void Test05_003_014()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID014);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID014);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID014);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID014);
		return;
	}

	/* Set stub flag */
	g_stub_flag.drvISR_RemoveSPI = drvISR_BUS_INTERFACE_FAULT;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.drvISR_RemoveSPI = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID014,
						test_ret, opt_info);

	R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	
	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID014);
		return;
	}
}

/* ------------------------05_003_015 [OSAL_RETURN_FAIL]------------------------ */
static void Test05_003_015()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTUNREGISTERISR,
														   OSAL_TEST_ID015);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID015);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID015);
		return;
	}

	/* Set stub flag */
	g_stub_flag.drvISR_RemoveSPI = drvISR_FAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.drvISR_RemoveSPI = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTUNREGISTERISR,
						OSAL_TEST_ID015,
						test_ret, opt_info);

	R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, OSAL_TEST_ID015);
		return;
	}
	osal_ret = osal_deinit_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID015);
		return;
	}
}
#endif

/* ------------------------05_004_001 [Success]------------------------ */
static void Test05_004_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID001);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID001);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID001,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID001);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------05_004_002 [Success]------------------------ */
static void Test05_004_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID002);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID002);
		return;
	}						   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID002,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID002);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------05_004_003 [Success]------------------------ */
static void Test05_004_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID003);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID003,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID003);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------05_004_004 [Success]------------------------ */
static void Test05_004_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID004);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID004,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID004);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------05_004_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test05_004_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID005);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(NULL, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID005,
						test_ret, opt_info);
}

/* ------------------------05_004_006 [OSAL_RETURN_PAR]------------------------ */
static void Test05_004_006()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID006);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID006);
		return;
	}					   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 100);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID006,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------05_004_007 [Success]------------------------ */
static void Test05_004_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID007);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID007);
		return;
	}						   

	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID007);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID007,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID007);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------05_004_008 [OSAL_RETURN_FAIL]------------------------ */
static void Test05_004_008()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID008);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID008);
		return;
	}   

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID008,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------05_004_009 [OSAL_RETURN_STATE]------------------------ */
static void Test05_004_009()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID009);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));				   

	osal_ret = osal_deinit_interrupt();
    if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID009);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID009,
						test_ret, opt_info);
}

#if 0
void Test05_004_010_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t		irq_channel = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID010);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(g_dev_handle, irq_channel);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID010,
						test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_004_010 [ISR: Success]------------------------ */
static void Test05_004_010()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	e_osal_return_t osal_ret;
	int              arg         = 10;
	char* device_id   = "ims_00";
	uint64_t         irq_channel = 0;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID010);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(g_dev_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID010);
		return;
	}						   

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_004_010_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID010);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;

	osal_ret = R_OSAL_InterruptDisableIsr(g_dev_handle, irq_channel);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID010);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(g_dev_handle, irq_channel, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID010);
		return;
	}
}

void Test05_004_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t		irq_channel = 4;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID011);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(g_dev_handle, irq_channel);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID011,
						test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_004_011 [ISR: Success]------------------------ */
static void Test05_004_011()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	e_osal_return_t osal_ret;
	int              arg         = 10;
	char* device_id   = "ims_00";
	uint64_t         irq_channel = 4;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID011);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(g_dev_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID011);
		return;
	}						   

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_004_011_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID011);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;

	osal_ret = R_OSAL_InterruptDisableIsr(g_dev_handle, irq_channel);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID011);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(g_dev_handle, irq_channel, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID011);
		return;
	}
}

void Test05_004_012_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t		irq_channel = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID012);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(g_dev_handle, irq_channel);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID012,
						test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_004_012 [ISR: Success]------------------------ */
static void Test05_004_012()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	e_osal_return_t osal_ret;
	int              arg         = 10;
	char* device_id   = "ims_00";
	uint64_t         irq_channel = 0;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID012);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(g_dev_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID012);
		return;
	}						   

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_004_012_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID012);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;

	osal_ret = R_OSAL_InterruptDisableIsr(g_dev_handle, irq_channel);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID012);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(g_dev_handle, irq_channel, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID012);
		return;
	}
}

void Test05_004_013_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t		irq_channel = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID013);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(g_dev_handle, irq_channel);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID013,
						test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_004_013 [ISR: Success]------------------------ */
static void Test05_004_013()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	e_osal_return_t osal_ret;
	int              arg         = 10;
	char* device_id   = "imr_03";
	uint64_t         irq_channel = 0;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID013);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID013);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(g_dev_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID013);
		return;
	}						   

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_004_013_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID013);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;

	osal_ret = R_OSAL_InterruptDisableIsr(g_dev_handle, irq_channel);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID013);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(g_dev_handle, irq_channel, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID013);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID013);
		return;
	}
}
#endif

#if 0
/* ------------------------05_004_014 [OSAL_RETURN_DEV]------------------------ */
static void Test05_004_014()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID014);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID014);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID014);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID014);
		return;
	}

	/* Set stub flag */
	g_stub_flag.drvISR_EnableSPI = drvISR_BUS_INTERFACE_FAULT;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.drvISR_EnableSPI = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID014,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID014);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID014);
		return;
	}
}

/* ------------------------05_004_015 [OSAL_RETURN_FAIL]------------------------ */
static void Test05_004_015()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTENABLEISR,
														   OSAL_TEST_ID015);
   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID015);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID015);
		return;
	}

	/* Set stub flag */
	g_stub_flag.drvISR_EnableSPI = drvISR_FAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.drvISR_EnableSPI = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTENABLEISR,
						OSAL_TEST_ID015,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR,
				OSAL_TEST_ID015);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, OSAL_TEST_ID015);
		return;
	}
	osal_ret = osal_deinit_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, OSAL_TEST_ID015);
		return;
	}
}
#endif

/* ------------------------05_005_001 [Success]------------------------ */
static void Test05_005_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID001);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);

	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID001,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------05_005_002 [Success]------------------------ */
static void Test05_005_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID002);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);

	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID002);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID002,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------05_005_003 [Success]------------------------ */
static void Test05_005_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID003);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);

	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID003,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------05_005_004 [Success]------------------------ */
static void Test05_005_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID004);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);

	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	TIME_MEASURE_STOP

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID004,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------05_005_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test05_005_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID005);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(NULL, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID005,
						test_ret, opt_info);
}

/* ------------------------05_005_006 [OSAL_RETURN_PAR]------------------------ */
static void Test05_005_006()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID006);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID006);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 100);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID006,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------05_005_007 [Success]------------------------ */
static void Test05_005_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID007);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);

	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID007);
		return;
	}

    R_OSAL_ThreadSleepForTimePeriod(500); // need sleep to wait R_OSAL_ThreadCreate finish

	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID007);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID007,
						test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------05_005_008 [Success]------------------------ */
static void Test05_005_008()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID008);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID008);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID008,
						test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------05_005_009 [OSAL_RETURN_STATE]------------------------ */
static void Test05_005_009()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID009);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_interrupt();
    if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID009);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID009,
						test_ret, opt_info);
}

#if 0
void Test05_005_010_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t		irq_channel = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID010);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(g_dev_handle, irq_channel);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID010,
						test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_004_010 [ISR: Success]------------------------ */
static void Test05_005_010()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	e_osal_return_t osal_ret;
	int              arg         = 10;
	char* device_id   = "ims_00";
	uint64_t         irq_channel = 0;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID010);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(g_dev_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID010);
		return;
	}
	osal_ret = R_OSAL_InterruptEnableIsr(g_dev_handle, irq_channel);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID010);
		return;
	}				   

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_005_010_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID010);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;

	osal_ret = R_OSAL_InterruptUnregisterIsr(g_dev_handle, irq_channel, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID010);
		return;
	}
}

void Test05_005_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t		irq_channel = 4;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID011);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(g_dev_handle, irq_channel);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID011,
						test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_004_011 [ISR: Success]------------------------ */
static void Test05_005_011()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	e_osal_return_t osal_ret;
	int              arg         = 10;
	char* device_id   = "ims_00";
	uint64_t         irq_channel = 4;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID011);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(g_dev_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID011);
		return;
	}
	osal_ret = R_OSAL_InterruptEnableIsr(g_dev_handle, irq_channel);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID011);
		return;
	}			   

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_005_011_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID011);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(g_dev_handle, irq_channel, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID011);
		return;
	}
}

void Test05_005_012_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t		irq_channel = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID012);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(g_dev_handle, irq_channel);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID012,
						test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_004_012 [ISR: Success]------------------------ */
static void Test05_005_012()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	e_osal_return_t osal_ret;
	int              arg         = 10;
	char* device_id   = "ims_00";
	uint64_t         irq_channel = 0;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID012);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(g_dev_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID012);
		return;
	}
	osal_ret = R_OSAL_InterruptEnableIsr(g_dev_handle, irq_channel);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID012);
		return;
	}	   

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_005_012_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID012);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(g_dev_handle, irq_channel, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID012);
		return;
	}
}

void Test05_005_013_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t		irq_channel = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID013);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(g_dev_handle, irq_channel);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID013,
						test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_004_013 [ISR: Success]------------------------ */
static void Test05_005_013()
{
	UINT16          Result;
	drvTMU_Time_t   Period;
	UINT8           Channel;
	e_osal_return_t osal_ret;
	int              arg         = 10;
	char* device_id   = "imr_03";
	uint64_t         irq_channel = 0;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID013);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID013);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(g_dev_handle, irq_channel, OSAL_INTERRUPT_PRIORITY_TYPE10,
						       (p_osal_isr_func_t)osal_test_cb_sample,
						       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID013);
		return;
	}
	osal_ret = R_OSAL_InterruptEnableIsr(g_dev_handle, irq_channel);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID013);
		return;
	}		   

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_005_013_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID013);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(g_dev_handle, irq_channel, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID013);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(g_dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID013);
		return;
	}
}
#endif

#if 0
/* ------------------------05_005_014 [OSAL_RETURN_FAIL]------------------------ */
static void Test05_005_014()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int 					arg=10;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID014);
		return;
	}

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTDISABLEISR,
														   OSAL_TEST_ID014);

   	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID014);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);

	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID014);
		return;
	}

	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID014);
		return;
	}

	/* Set stub flag */
	g_stub_flag.drvISR_DisableSPI = drvISR_FAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.drvISR_DisableSPI = 0;
	
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
						OSAL_TEST_INTERRUPTDISABLEISR,
						OSAL_TEST_ID014,
						test_ret, opt_info);

	R_OSAL_InterruptDisableIsr(dev_handle, 0);

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR,
				OSAL_TEST_ID014);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID014);
		return;
	}
	osal_ret = osal_deinit_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, OSAL_TEST_ID014);
		return;
	}
}
#endif

/* ------------------------05_006_001 [Success]------------------------ */
static void Test05_006_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID001);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID001);
		return;
	}
	
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------05_006_002 [Success]------------------------ */
static void Test05_006_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID002);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID002);
		return;
	}
	
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID002);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID002);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------05_006_003 [Success]------------------------ */
static void Test05_006_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID003);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID003);
		return;
	}
	
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID003);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------05_006_004 [Success]------------------------ */
static void Test05_006_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID004);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID004);
		return;
	}
	
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID004);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------05_006_005 [Success]------------------------ */
static void Test05_006_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID005);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID005);
		return;
	}
	
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID005);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_LOWEST);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID005);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------05_006_006 [Success]------------------------ */
static void Test05_006_006()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID006);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID006);
		return;
	}
	
	osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptEnableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID006);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_HIGHEST);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptDisableIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID006);
		return;
	}
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------05_006_007 [OSAL_RETURN_HANDLE]------------------------ */
static void Test05_006_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID007);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(NULL, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID007, 
							test_ret, opt_info);
}

/* ------------------------05_006_008 [OSAL_RETURN_PAR]------------------------ */
static void Test05_006_008()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID008);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID008);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 100, OSAL_INTERRUPT_PRIORITY_TYPE13);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID008, 
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------05_006_009 [OSAL_RETURN_PAR]------------------------ */
static void Test05_006_009()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID009);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID009);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_HIGHEST + 1);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID009, 
							test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID009);
		return;
	}
}

/* ------------------------05_006_010 [OSAL_RETURN_PAR]------------------------ */
static void Test05_006_010()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID010);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID010);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, (e_osal_interrupt_priority_t)(OSAL_INTERRUPT_PRIORITY_LOWEST - 1));
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID010, 
							test_ret, opt_info);
	
	osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)osal_test_cb_sample);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY,
				OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID010);
		return;
	}
}

/* ------------------------05_006_011 [OSAL_RETURN_STATE]------------------------ */
static void Test05_006_011()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID011);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_interrupt();
    if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID011);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_HIGHEST);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID011, 
							test_ret, opt_info);
}

#if 0
/* ------------------------05_006_012 [OSAL_RETURN_FAIL]------------------------ */
static void Test05_006_012()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	dev_handle;
	int						arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTSETISRPRIORITY,
														   OSAL_TEST_ID012);

	memset(&dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID012);
		return;
	}
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
					       (p_osal_isr_func_t)osal_test_cb_sample,
					       (void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID012);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;
	
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_INTERRUPT,
							OSAL_TEST_INTERRUPTSETISRPRIORITY,
							OSAL_TEST_ID012, 
							test_ret, opt_info);
	
	osal_ret = osal_deinit_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, OSAL_TEST_ID012);
		return;
	}
}
#endif

/* ------------------------05_007_001 [Success]------------------------ */
static void Test05_007_001()
{
	e_osal_return_t 		osal_ret;
	bool 					result;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTISISRCONTEXT,
														   OSAL_TEST_ID001);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptIsISRContext(&result);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (result == false))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT, 
							OSAL_TEST_INTERRUPTISISRCONTEXT,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

#if 0
void Test05_007_002_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType,
						   drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t osal_ret;
	bool            result;
	e_osal_return_t expect_data;
	int             test_ret;
	char            opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTISISRCONTEXT,
														   OSAL_TEST_ID002);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptIsISRContext(&result);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (result == true))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT, 
							OSAL_TEST_INTERRUPTISISRCONTEXT,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	g_int_cb_flag = 1;
}

/* ------------------------05_007_002 [Success]------------------------ */
static void Test05_007_002()
{
	//UINT16        Result;
	drvTMU_Time_t Period;
	UINT8         Channel;
	
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTISISRCONTEXT, OSAL_TEST_ID002);
		return;
	}*/

	Channel = 0U; /* Channel number */
	Period.Sec = 1; /* 1 second */
	Period.Usec = 500000; /* 500000 usec */

	Result = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test05_007_002_CallbackFunc, TMU_EDGE_RISE);
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTISISRCONTEXT, OSAL_TEST_ID002);
		return;
	}

	while(g_int_cb_flag == 0);
	g_int_cb_flag = 0;

	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTISISRCONTEXT, OSAL_TEST_ID002);
		return;
	}*/
}
#endif

/* ------------------------05_007_003 [OSAL_RETURN_PAR]------------------------ */
static void Test05_007_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_INTERRUPT, 
														   OSAL_TEST_INTERRUPTISISRCONTEXT,
														   OSAL_TEST_ID003);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_InterruptIsISRContext(NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_INTERRUPT, 
							OSAL_TEST_INTERRUPTISISRCONTEXT,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* 05_001: R_OSAL_InterruptGetNumOfIrqChannels */
static void osal_test_R_OSAL_InterruptGetNumOfIrqChannels()
{

	e_osal_return_t osal_ret;
	//UINT16 Result;

	memset(&g_dev_handle, 0x00, sizeof(osal_device_handle_t));
		
	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoInitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, 0);
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, 0);
		return;
	}*/

	Test05_001_001();
	Test05_001_002();
	Test05_001_003();
	Test05_001_004();
	Test05_001_005();
	Test05_001_006();
	Test05_001_007();
	//Test05_001_008();

	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS, 0);
		return;
	}*/
}

/* 05_002: R_OSAL_InterruptRegisterIsr */
static void osal_test_R_OSAL_InterruptRegisterIsr(){

	e_osal_return_t osal_ret;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoInitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTREGISTERISR, 0);
		return;
	}

	Test05_002_001();
	Test05_002_002();
	Test05_002_003();
	Test05_002_004();
	Test05_002_005();
	Test05_002_006(); /* Add July 12, 2020 */
	Test05_002_007();
	Test05_002_008();
	Test05_002_009();
	Test05_002_010();
	Test05_002_011();
	Test05_002_012();
	//Test05_002_013();
	//Test05_002_014();
	//Test05_002_015();
}

/* 05_003: R_OSAL_InterruptUnregisterIsr */
static void osal_test_R_OSAL_InterruptUnregisterIsr()
{
	e_osal_return_t osal_ret;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoInitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTUNREGISTERISR, 0);
		return;
	}

	Test05_003_001();
	Test05_003_002();
	Test05_003_003();
	Test05_003_004();
	Test05_003_005();
	Test05_003_006();
	Test05_003_007();
	Test05_003_008();
	Test05_003_009();
	Test05_003_010();
	Test05_003_011();
	//Test05_003_012(); /* Add July 11, 2020 */
	Test05_003_013();
	//Test05_003_014();
	//Test05_003_015();
}

/* 05_004: R_OSAL_InterruptEnableIsr */
static void osal_test_R_OSAL_InterruptEnableIsr()
{
	e_osal_return_t osal_ret;
	//UINT16 Result;
	
	memset(&g_dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoInitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, 0);
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, 0);
		return;
	}*/

	Test05_004_001();
	Test05_004_002();
	Test05_004_003();
	Test05_004_004();
	Test05_004_005();
	Test05_004_006();
	Test05_004_007();
	Test05_004_008();
	Test05_004_009();
	//Test05_004_010();
	//Test05_004_011();
	//Test05_004_012();
	//Test05_004_013();
	//Test05_004_014();
	//Test05_004_015();
	
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTENABLEISR, 0);
		return;
	}*/
}

/* 05_005: R_OSAL_InterruptDisableIsr */
static void osal_test_R_OSAL_InterruptDisableIsr()
{
	e_osal_return_t osal_ret;
	//UINT16 Result;
	
	memset(&g_dev_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoInitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, 0);
		return;
	}
	/*Result = drvTMU_Initialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, 0);
		return;
	}*/

	Test05_005_001();
	Test05_005_002();
	Test05_005_003();
	Test05_005_004();
	Test05_005_005();
	Test05_005_006();
	Test05_005_007();
	Test05_005_008();
	Test05_005_009();
	//Test05_005_010();
	//Test05_005_011();
	//Test05_005_012();
	//Test05_005_013();
	//Test05_005_014();
	
	/*Result = drvTMU_Deinitialize();
	if (Result != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTDISABLEISR, 0);
		return;
	}*/
}

/* 05_006: R_OSAL_InterruptSetIsrPriority */
static void osal_test_R_OSAL_InterruptSetIsrPriority()
{
	e_osal_return_t osal_ret;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoInitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTSETISRPRIORITY, 0);
		return;
	}

	Test05_006_001();
	Test05_006_002();
	Test05_006_003();
	Test05_006_004();
	Test05_006_005();
	Test05_006_006();
	Test05_006_007();
	Test05_006_008();
	Test05_006_009();
	Test05_006_010();
	Test05_006_011();
	//Test05_006_012();
}

/* 05_007: R_OSAL_InterruptIsISRContext */
static void osal_test_R_OSAL_InterruptIsISRContext()
{
	e_osal_return_t osal_ret;

	osal_ret = osal_init_interrupt();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoInitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTISISRCONTEXT, 0);
		return;
	}

	Test05_007_001();
	//Test05_007_002();
	Test05_007_003();

	osal_ret = osal_deinit_interrupt();
    if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_INTERRUPT, OSAL_TEST_INTERRUPTISISRCONTEXT, 0);
		return;
	}
}
