/**
 * @file io_manager.c
 * @brief OSAL IO Manager evaluation file.
 * @author Khanh Hoang
 * @date Aug 04, 2020
 */

/* include */
#include "osal_test.h"
#include "osal_test_common.h"
#include "io_manager.h"

/* prototype */
static int osal_test_rename(const char* old_path, const char* new_path);

static void osal_test_R_OSAL_IoGetNumOfDevices();
static void osal_test_R_OSAL_IoGetDeviceList();
static void osal_test_R_OSAL_IoDeviceOpen();
static void osal_test_R_OSAL_IoDeviceClose();
static void osal_test_R_OSAL_IoRead8();
static void osal_test_R_OSAL_IoRead16();
static void osal_test_R_OSAL_IoRead32();
static void osal_test_R_OSAL_IoRead64();
static void osal_test_R_OSAL_IoWrite8();
static void osal_test_R_OSAL_IoWrite16();
static void osal_test_R_OSAL_IoWrite32();
static void osal_test_R_OSAL_IoWrite64();
static void osal_test_R_OSAL_IoBlockRead8();
static void osal_test_R_OSAL_IoBlockRead16();
static void osal_test_R_OSAL_IoBlockRead32();
static void osal_test_R_OSAL_IoBlockRead64();
static void osal_test_R_OSAL_IoBlockWrite8();
static void osal_test_R_OSAL_IoBlockWrite16();
static void osal_test_R_OSAL_IoBlockWrite32();
static void osal_test_R_OSAL_IoBlockWrite64();
static void osal_test_R_OSAL_IoGetAxiBusIdFromDeviceName();
static void osal_test_R_OSAL_IoGetAxiBusName();
static void osal_test_R_OSAL_IoGetAxiBusNameList();
static void osal_test_R_OSAL_IoGetDeviceAxiBusId();
static void osal_test_R_OSAL_IoGetAxiIdListForAxiBusName();
static void osal_test_R_OSAL_IoGetAxiIdListForDeviceHdl();
static void osal_test_R_OSAL_IoGetAxiIdListForDeviceName();
static void osal_test_R_OSAL_IoGetNumOfAxiIdsForAxiBusName();
static void osal_test_R_OSAL_IoGetNumOfAxiIdsForDeviceHdl();
static void osal_test_R_OSAL_IoGetNumOfAxiIdsForDeviceName();

void osal_test_io_manager_display_menu()
{
	printf("*** OSAL IO Manager ***\n");
	printf("***  06_001: R_OSAL_IoGetNumOfDevices                   ***\n");
	printf("***  06_002: R_OSAL_IoGetDeviceList                     ***\n");
	printf("***  06_003: R_OSAL_IoDeviceOpen                        ***\n");
	printf("***  06_004: R_OSAL_IoDeviceClose                       ***\n");
	printf("***  06_005: R_OSAL_IoRead8                             ***\n");
	printf("***  06_006: R_OSAL_IoRead16                            ***\n");
	printf("***  06_007: R_OSAL_IoRead32                            ***\n");
	printf("***  06_008: R_OSAL_IoRead64                            ***\n");
	printf("***  06_009: R_OSAL_IoBlockRead8                        ***\n");
	printf("***  06_010: R_OSAL_IoBlockRead16                       ***\n");
	printf("***  06_011: R_OSAL_IoBlockRead32                       ***\n");
	printf("***  06_012: R_OSAL_IoBlockRead64                       ***\n");
	printf("***  06_013: R_OSAL_IoWrite8                            ***\n");
	printf("***  06_014: R_OSAL_IoWrite16                           ***\n");
	printf("***  06_015: R_OSAL_IoWrite32                           ***\n");
	printf("***  06_016: R_OSAL_IoWrite64                           ***\n");
	printf("***  06_017: R_OSAL_IoBlockWrite8                       ***\n");
	printf("***  06_018: R_OSAL_IoBlockWrite16                      ***\n");
	printf("***  06_019: R_OSAL_IoBlockWrite32                      ***\n");
	printf("***  06_020: R_OSAL_IoBlockWrite64                      ***\n");
	printf("***  06_021: R_OSAL_IoGetAxiBusIdFromDeviceName         ***\n");
	printf("***  06_022: R_OSAL_IoGetAxiBusName                     ***\n");
	printf("***  06_023: R_OSAL_IoGetAxiBusNameList                 ***\n");
	printf("***  06_024: R_OSAL_IoGetDeviceAxiBusId                 ***\n");
	printf("***  06_025: R_OSAL_IoGetAxiIdListForAxiBusName         ***\n");
	printf("***  06_026: R_OSAL_IoGetAxiIdListForDeviceHdl          ***\n");
	printf("***  06_027: R_OSAL_IoGetAxiIdListForDeviceName         ***\n");
	printf("***  06_028: R_OSAL_IoGetNumOfAxiIdsForAxiBusName       ***\n");
	printf("***  06_029: R_OSAL_IoGetNumOfAxiIdsForDeviceHdl        ***\n");
	printf("***  06_030: R_OSAL_IoGetNumOfAxiIdsForDeviceName       ***\n");
	printf("***  06_100: io manager module all exec                 ***\n\n");
	return;
}

void osal_test_io_manager_menu(int child)
{
	int test_number = child;
	unsigned int test_count = 1;
	unsigned int loop_count = 0;

	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number	= OSAL_TEST_IOGETNUMOFDEVICES;
		test_count	= OSAL_TEST_IO_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number)
		{
			case OSAL_TEST_IOGETNUMOFDEVICES:
				osal_test_R_OSAL_IoGetNumOfDevices();
				break;
			case OSAL_TEST_IOGETDEVICELIST:
				osal_test_R_OSAL_IoGetDeviceList();
				break;
			case OSAL_TEST_IODEVICEOPEN:
				osal_test_R_OSAL_IoDeviceOpen();
				break;
			case OSAL_TEST_IODEVICECLOSE:
				osal_test_R_OSAL_IoDeviceClose();
				break;
			case OSAL_TEST_IOREAD8:
				osal_test_R_OSAL_IoRead8();
				break;
			case OSAL_TEST_IOREAD16:
				osal_test_R_OSAL_IoRead16();
				break;
			case OSAL_TEST_IOREAD32:
				osal_test_R_OSAL_IoRead32();
				break;
			case OSAL_TEST_IOREAD64:
				osal_test_R_OSAL_IoRead64();
				break;
			case OSAL_TEST_IOBLOCKREAD8:
				osal_test_R_OSAL_IoBlockRead8();
				break;
			case OSAL_TEST_IOBLOCKREAD16:
				osal_test_R_OSAL_IoBlockRead16();
				break;
			case OSAL_TEST_IOBLOCKREAD32:
				osal_test_R_OSAL_IoBlockRead32();
				break;
			case OSAL_TEST_IOBLOCKREAD64:
				osal_test_R_OSAL_IoBlockRead64();
				break;
			case OSAL_TEST_IOWRITE8:
				osal_test_R_OSAL_IoWrite8();
				break;
			case OSAL_TEST_IOWRITE16:
				osal_test_R_OSAL_IoWrite16();
				break;
			case OSAL_TEST_IOWRITE32:
				osal_test_R_OSAL_IoWrite32();
				break;
			case OSAL_TEST_IOWRITE64:
				osal_test_R_OSAL_IoWrite64();
				break;
			case OSAL_TEST_IOBLOCKWRITE8:
				osal_test_R_OSAL_IoBlockWrite8();
				break;
			case OSAL_TEST_IOBLOCKWRITE16:
				osal_test_R_OSAL_IoBlockWrite16();
				break;
			case OSAL_TEST_IOBLOCKWRITE32:
				osal_test_R_OSAL_IoBlockWrite32();
				break;
			case OSAL_TEST_IOBLOCKWRITE64:
				osal_test_R_OSAL_IoBlockWrite64();
				break;
			case OSAL_TEST_IOGETAXIBUSIDFROMDEVICENAME:
				osal_test_R_OSAL_IoGetAxiBusIdFromDeviceName();
				break;
			case OSAL_TEST_IOGETAXIBUSNAME:
				osal_test_R_OSAL_IoGetAxiBusName();
				break;
			case OSAL_TEST_IOGETAXIBUSNAMELIST:
				osal_test_R_OSAL_IoGetAxiBusNameList();
				break;
			case OSAL_TEST_IOGETDEVICEAXIBUSID:
				osal_test_R_OSAL_IoGetDeviceAxiBusId();
				break;
			case OSAL_TEST_IOGETAXIIDLISTFORAXIBUSNAME:
				osal_test_R_OSAL_IoGetAxiIdListForAxiBusName();
				break;
			case OSAL_TEST_IOGETAXIIDLISTFORDEVICEHDL:
				osal_test_R_OSAL_IoGetAxiIdListForDeviceHdl();
				break;
			case OSAL_TEST_IOGETAXIIDLISTFORDEVICENAME:
				osal_test_R_OSAL_IoGetAxiIdListForDeviceName();
				break;
			case OSAL_TEST_IOGETNUMOFAXIIDSFORAXIBUSNAME:
				osal_test_R_OSAL_IoGetNumOfAxiIdsForAxiBusName();
				break;
			case OSAL_TEST_IOGETNUMOFAXIIDSFORDEVICEHDL:
				osal_test_R_OSAL_IoGetNumOfAxiIdsForDeviceHdl();
				break;
			case OSAL_TEST_IOGETNUMOFAXIIDSFORDEVICENAME:
				osal_test_R_OSAL_IoGetNumOfAxiIdsForDeviceName();
				break;

			default:
				printf("That menu is not available.\n\n");
				break;
		}
		test_number++;
	}
	return;
}

static int osal_test_rename(const char* old_path, const char* new_path)
{
	int		test_ret = OSAL_TEST_OK;
	int		linux_ret;
	int32_t	cnt = 0;
	int32_t loop_max = 10; // 1sec

	do
	{
		linux_ret = rename(old_path, new_path);
		if (0 != linux_ret)
		{
			test_ret = OSAL_TEST_NG;
			printf("rename=%d %s\n", linux_ret, strerror(errno));
			break;
		}

		linux_ret = access(new_path, F_OK);
		if (0 == linux_ret)
		{
			break;
		}

		(void)R_OSAL_ThreadSleepForTimePeriod(100);
		cnt++;

	} while (cnt < loop_max);

	if (loop_max <= cnt)
	{
		printf("rename timeout\n");
		test_ret = OSAL_TEST_NG;
	}

	return test_ret;
}

/* ------------------------06_003_001 [OSAL_RETURN_ID]------------------------ */
static void Test06_003_001()
{
	int						test_ret = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t			osal_test_ret = OSAL_RETURN_OK;
	e_osal_return_t			expect_data;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t	handle;

#if defined(RCAR_S4)
	const char				device_type[]		= "caiplite";
	const char				device_id[]			= "caiplite_00";
	const char				device_old_path[]	= "/dev/caiplite_00";
	const char				device_new_path[]	= "/dev/caiplit_00";
#else
	const char				device_type[]		= "imp_top";
	const char				device_id[]			= "imp_top_00";
	const char				device_old_path[]	= "/dev/imp_top_00";
	const char				device_new_path[]	= "/dev/imp_to_00";
#endif

	expect_data = (e_osal_return_t)osal_test_get_expectdata(OSAL_TEST_IO,
															OSAL_TEST_IODEVICEOPEN,
															OSAL_TEST_ID001);

	test_ret = osal_test_rename(device_old_path, device_new_path);
	if (OSAL_TEST_OK != test_ret)
	{
		OSAL_TEST_ERR("rename(imp_to_00) error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
	}

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
		osal_test_ret = osal_ret;
		test_ret = OSAL_TEST_NG;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
	TIME_MEASURE_STOP
	osal_test_ret = osal_ret;
	if (expect_data != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceClose(handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
			osal_test_ret = osal_ret;
			test_ret = OSAL_TEST_NG;
		}
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
		osal_test_ret = osal_ret;
		test_ret = OSAL_TEST_NG;
	}

	test_ret = osal_test_rename(device_new_path, device_old_path);
	if (OSAL_TEST_OK != test_ret)
	{
		OSAL_TEST_ERR("rename(imp_top_00) error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_test_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID001, test_ret, opt_info);
	return;
}

/* ------------------------06_003_002 [OSAL_RETURN_ID]------------------------ */
static void Test06_003_002()
{
	int						test_ret = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t			osal_test_ret = OSAL_RETURN_OK;
	e_osal_return_t			expect_data;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t	handle;
	size_t					numOfLists;

#if defined(RCAR_S4)
	const char				device_type[]		= "caiplite";
	const char				device_id[]			= "caiplite_00";
	const char				device_old_path[]	= "/dev/caiplite_00";
	const char				device_new_path[]	= "/dev/caiplite_99";
#else
	const char				device_type[]		= "imp_top";
	const char				device_id[]			= "imp_top_00";
	const char				device_old_path[]	= "/dev/imp_top_00";
	const char				device_new_path[]	= "/dev/imp_top_01";
#endif

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO,
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID002);

	test_ret = osal_test_rename(device_old_path, device_new_path);
	if (OSAL_TEST_OK != test_ret)
	{
		OSAL_TEST_ERR("rename(imp_top_01) error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID002);
	}

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID002);
		osal_test_ret = osal_ret;
		test_ret = OSAL_TEST_NG;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
	TIME_MEASURE_STOP
	osal_test_ret = osal_ret;
	if (expect_data != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID002);

		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		osal_ret = R_OSAL_IoDeviceClose(handle);
		if (OSAL_RETURN_OK != osal_ret)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID002);
			osal_test_ret = osal_ret;
			test_ret = OSAL_TEST_NG;
		}
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID002);
		osal_test_ret = osal_ret;
		test_ret = OSAL_TEST_NG;
	}

	test_ret = osal_test_rename(device_new_path, device_old_path);
	if (OSAL_TEST_OK != test_ret)
	{
		OSAL_TEST_ERR("rename(imp_top_00) error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID002);
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_test_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID002, test_ret, opt_info);

	return;
}

/* ------------------------06_022_001 [OSAL_RETURN_ID]------------------------ */
static void Test06_022_001()
{
#ifdef SUPPORT_IO_EXTENSION
	e_osal_return_t			osal_ret;
	e_osal_return_t			osal_test_ret = OSAL_RETURN_OK;
	int 					test_ret = OSAL_TEST_NG;
	e_osal_return_t			expect_data;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t					buffer_size;
	char					axi_name[100];
	size_t					numOfName = 0;
	osal_axi_bus_id_t		axi_id = 0;
	size_t					numOfDevices = 0;
	int						i = 0;
	char					*device[100];
	char					list[200];
	size_t					list_size;
	size_t					numOfLists = 0;
	const char				s[2] = "\n";
	osal_device_handle_t	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO,
															OSAL_TEST_IOGETAXIBUSNAME,
															OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETAXIBUSNAME, OSAL_TEST_ID001);
		osal_test_ret = osal_ret;
		test_ret = OSAL_TEST_NG;
	}

	axi_id = 1000;
	buffer_size = sizeof(axi_name);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoGetAxiBusName(axi_id, axi_name, buffer_size, &numOfName);
	TIME_MEASURE_STOP
	osal_test_ret = osal_ret;
	if(osal_ret == expect_data && numOfName == 0)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETAXIBUSNAME, OSAL_TEST_ID001);
		osal_test_ret = osal_ret;
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_test_ret));
							osal_test_result_output(OSAL_TEST_IO, OSAL_TEST_IOGETAXIBUSNAME,
							OSAL_TEST_ID001, test_ret, opt_info);
#endif
	return;
}

/* 06_001: R_OSAL_IoGetNumOfDevices */
static void osal_test_R_OSAL_IoGetNumOfDevices()
{
	// There are no test cases for this API.
	return;
}

/* 06_002: R_OSAL_IoGetDeviceList */
static void osal_test_R_OSAL_IoGetDeviceList()
{
	// There are no test cases for this API.
	return;
}

/* 06_003: R_OSAL_IoDeviceOpen */
static void osal_test_R_OSAL_IoDeviceOpen()
{
	Test06_003_001();
	Test06_003_002();
	return;
}

/* 06_004: R_OSAL_IoDeviceClose */
static void osal_test_R_OSAL_IoDeviceClose()
{
	// There are no test cases for this API.
	return;
}

/* 06_005: R_OSAL_IoRead8 */
static void osal_test_R_OSAL_IoRead8()
{
	// There are no test cases for this API.
	return;
}

/* 06_006: R_OSAL_IoRead16 */
static void osal_test_R_OSAL_IoRead16()
{
	// There are no test cases for this API.
	return;
}

/* 06_007: R_OSAL_IoRead32 */
static void osal_test_R_OSAL_IoRead32()
{
	// There are no test cases for this API.
	return;
}

/* 06_008: R_OSAL_IoRead64 */
static void osal_test_R_OSAL_IoRead64()
{
	// There are no test cases for this API.
	return;
}

/* 06_009: R_OSAL_IoBlockRead8 */
static void osal_test_R_OSAL_IoBlockRead8()
{
	// There are no test cases for this API.
	return;
}

/* 06_010: R_OSAL_IoBlockRead16 */
static void osal_test_R_OSAL_IoBlockRead16()
{
	// There are no test cases for this API.
	return;
}

/* 06_011: R_OSAL_IoBlockRead32 */
static void osal_test_R_OSAL_IoBlockRead32()
{
	// There are no test cases for this API.
	return;
}

/* 06_012: R_OSAL_IoBlockRead64 */
static void osal_test_R_OSAL_IoBlockRead64()
{
	// There are no test cases for this API.
	return;
}

/* 06_013: R_OSAL_IoWrite8 */
static void osal_test_R_OSAL_IoWrite8()
{
	// There are no test cases for this API.
	return;
}

/* 06_014: R_OSAL_IoWrite16 */
static void osal_test_R_OSAL_IoWrite16()
{
	// There are no test cases for this API.
	return;
}

/* 06_015: R_OSAL_IoWrite32 */
static void osal_test_R_OSAL_IoWrite32()
{
	// There are no test cases for this API.
	return;
}

/* 06_016: R_OSAL_IoWrite64 */
static void osal_test_R_OSAL_IoWrite64()
{
	// There are no test cases for this API.
	return;
}

/* 06_017: R_OSAL_IoBlockWrite8 */
static void osal_test_R_OSAL_IoBlockWrite8()
{
	// There are no test cases for this API.
	return;
}

/* 06_018: R_OSAL_IoBlockWrite16 */
static void osal_test_R_OSAL_IoBlockWrite16()
{
	// There are no test cases for this API.
	return;
}

/* 06_019: R_OSAL_IoBlockWrite32 */
static void osal_test_R_OSAL_IoBlockWrite32()
{
	// There are no test cases for this API.
	return;
}

/* 06_020: R_OSAL_IoBlockWrite64 */
static void osal_test_R_OSAL_IoBlockWrite64()
{
	// There are no test cases for this API.
	return;
}

/* 06_021: R_OSAL_IoGetAxiBusIdFromDeviceName */
static void osal_test_R_OSAL_IoGetAxiBusIdFromDeviceName()
{
	// There are no test cases for this API.
	return;
}

/* 06_022: R_OSAL_IoGetAxiBusName */
static void osal_test_R_OSAL_IoGetAxiBusName()
{
	Test06_022_001();
	return;
}

/* 06_023: R_OSAL_IoGetAxiBusNameList */
static void osal_test_R_OSAL_IoGetAxiBusNameList()
{
	// There are no test cases for this API.
	return;
}

/* 06_024: R_OSAL_IoGetDeviceAxiBusId */
static void osal_test_R_OSAL_IoGetDeviceAxiBusId()
{
	// There are no test cases for this API.
	return;
}

/* 06_025: R_OSAL_IoGetAxiIdListForAxiBusName */
static void osal_test_R_OSAL_IoGetAxiIdListForAxiBusName()
{
	// There are no test cases for this API.
	return;
}

/* 06_026: R_OSAL_IoGetAxiIdListForDeviceHdl */
static void osal_test_R_OSAL_IoGetAxiIdListForDeviceHdl()
{
	// There are no test cases for this API.
	return;
}

/* 06_027: R_OSAL_IoGetAxiIdListForDeviceName */
static void osal_test_R_OSAL_IoGetAxiIdListForDeviceName()
{
	// There are no test cases for this API.
	return;
}

/* 06_028: R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
static void osal_test_R_OSAL_IoGetNumOfAxiIdsForAxiBusName()
{
	// There are no test cases for this API.
	return;
}

/* 06_029: R_OSAL_IoGetNumOfAxiIdsForDeviceHdl */
static void osal_test_R_OSAL_IoGetNumOfAxiIdsForDeviceHdl()
{
	// There are no test cases for this API.
	return;
}

/* 06_030: R_OSAL_IoGetNumOfAxiIdsForDeviceName */
static void osal_test_R_OSAL_IoGetNumOfAxiIdsForDeviceName()
{
	// There are no test cases for this API.
	return;
}
