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

static void osal_test_R_OSAL_IoDeviceOpen();

void osal_test_io_manager_display_menu()
{
	printf("*** OSAL IO Manager ***\n");
	printf("***  06_003: R_OSAL_IoDeviceOpen                        ***\n");
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
			case OSAL_TEST_IOGETDEVICELIST:
			case OSAL_TEST_IODEVICECLOSE:
			case OSAL_TEST_IOREAD8:
			case OSAL_TEST_IOREAD16:
			case OSAL_TEST_IOREAD32:
			case OSAL_TEST_IOREAD64:
			case OSAL_TEST_IOBLOCKREAD8:
			case OSAL_TEST_IOBLOCKREAD16:
			case OSAL_TEST_IOBLOCKREAD32:
			case OSAL_TEST_IOBLOCKREAD64:
			case OSAL_TEST_IOWRITE8:
			case OSAL_TEST_IOWRITE16:
			case OSAL_TEST_IOWRITE32:
			case OSAL_TEST_IOWRITE64:
			case OSAL_TEST_IOBLOCKWRITE8:
			case OSAL_TEST_IOBLOCKWRITE16:
			case OSAL_TEST_IOBLOCKWRITE32:
			case OSAL_TEST_IOBLOCKWRITE64:
			case OSAL_TEST_IOGETAXIBUSIDFROMDEVICENAME:
			case OSAL_TEST_IOGETAXIBUSNAME:
			case OSAL_TEST_IOGETAXIBUSNAMELIST:
			case OSAL_TEST_IOGETDEVICEAXIBUSID:
			case OSAL_TEST_IOGETAXIIDLISTFORAXIBUSNAME:
			case OSAL_TEST_IOGETAXIIDLISTFORDEVICEHDL:
			case OSAL_TEST_IOGETAXIIDLISTFORDEVICENAME:
			case OSAL_TEST_IOGETNUMOFAXIIDSFORAXIBUSNAME:
			case OSAL_TEST_IOGETNUMOFAXIIDSFORDEVICEHDL:
			case OSAL_TEST_IOGETNUMOFAXIIDSFORDEVICENAME:
				break;

			case OSAL_TEST_IODEVICEOPEN:
				osal_test_R_OSAL_IoDeviceOpen();
				break;

			default:
				printf("That menu is not available.\n\n");
				break;
		}
		test_number++;
	}
	return;
}

/* ------------------------06_003_001 [OSAL_RETURN_CONF]------------------------ */
static void Test06_003_001()
{
	int						test_ret = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t			expect_data;
	char					opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	osal_device_handle_t	handle;
	ssize_t					len;
	char					uio[64];
	char					map_addr_file[PATH_MAX];
	FILE*					p_fd = NULL;
	uint64_t				map_addr;

#if defined(RCAR_S4)
	const char				device_id[]		= "caiplite_00";
	const char				device_path[]	= "/dev/caiplite_00";
#else
	const char				device_id[]		= "imp_top_00";
	const char				device_path[]	= "/dev/imp_top_00";
#endif

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO,
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID001);

	osal_ret = R_OSAL_Initialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		len = readlink(device_path, uio, sizeof(uio));
		if (-1 == len)
		{
			OSAL_TEST_ERR("readlink() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
			osal_ret = OSAL_RETURN_FAIL;
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		uio[len] = '\0';
		snprintf(map_addr_file, sizeof(map_addr_file), "/sys/class/uio/%s/maps/map0/addr", basename(uio));

		p_fd = fopen(map_addr_file, "r");
		if (NULL == p_fd)
		{
			printf("map_addr_file=%s\n", map_addr_file);
			OSAL_TEST_ERR("fopen() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
			osal_ret = OSAL_RETURN_FAIL;
			test_ret = OSAL_TEST_NG;
		}
	}

	if (OSAL_RETURN_OK == osal_ret)
	{
		len = fscanf(p_fd, "%lx", &map_addr);
		if (1 != len)
		{
			OSAL_TEST_ERR("fscanf() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
			osal_ret = OSAL_RETURN_FAIL;
			test_ret = OSAL_TEST_NG;
		}
	}

	if (NULL != p_fd)
	{
		fclose(p_fd);
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	if (0U == map_addr)
	{
		if (OSAL_RETURN_OK == osal_ret)
		{
			TIME_MEASURE_INIT
			TIME_MEASURE_START
			osal_ret = R_OSAL_IoDeviceOpen(device_id, &handle);
			TIME_MEASURE_STOP
			if (expect_data != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
						OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
			OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
		}

		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_IoDeviceClose(handle);
			if (OSAL_RETURN_OK != osal_ret)
			{
				OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
						OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
				test_ret = OSAL_TEST_NG;
			}
		}
	}
	else
	{
		printf("This test needs device tree blob edit.\n");
		test_ret = OSAL_TEST_NG;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}

	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID001, test_ret, opt_info);

	return;
}

/* 06_003: R_OSAL_IoDeviceOpen */
static void osal_test_R_OSAL_IoDeviceOpen()
{
	Test06_003_001();
	return;
}
