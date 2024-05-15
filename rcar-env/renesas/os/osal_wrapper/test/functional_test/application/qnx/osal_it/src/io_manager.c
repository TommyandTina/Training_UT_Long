/**
 * @file io_manager.c
 * @brief OSAL IO Manager evaluation file.
 * @author Khanh Hoang
 * @date Aug 04, 2020
 */

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "rcar-xos/osal/r_osal.h"
#include "osal_test.h"
#include "osal_test_common.h"
#include "io_manager.h"
//#include "rcar-xos/tmu/tmu.h"

/* global variable */
//static osal_device_handle_t g_io_cb_handle;
//static int g_io_cb_flag = 0;
static st_osal_test_info_t gs_osal_test_info[] =
{
#if defined(RCAR_V3U)
	{"ims",               2,  14,  "ims_00\nims_01"},
	{"imr",               4,  28,  "imr_00\nimr_01\nimr_02\nimr_03"},
	{"dof",               2,  14,  "dof_00\ndof_01"},
	{"stv",               2,  14,  "stv_00\nstv_01"},
	{"acf",               4,  28,  "acf_00\nacf_01\nacf_02\nacf_03"},
	{"iccom",             1,  9,   "iccom_00"},
	{"ivcp1e",            1,  10,  "ivcp1e_00"},
	{"vcp4",              1,  8,   "vcp4_00"},
	{"fcpc",              1,  8,   "fcpc_00"},
	{"imp_top",           1,  11,  "imp_top_00"},
	{"imp_ldmac_master",  1,  20,  "imp_ldmac_master_00"},
	{"imp_ldmac_checker", 1,  21,  "imp_ldmac_checker_00"},
	{"imp_spmi",          2,  24,  "imp_spmi_00\nimp_spmi_01"},
	{"imp_spmc",          3,  36,  "imp_spmc_00\nimp_spmc_01\nimp_spmc_02"},
	{"imp",               4,  28,  "imp_00\nimp_01\nimp_02\nimp_03"},
	{"imp_ocv",           8,  88,  "imp_ocv_00\nimp_ocv_01\nimp_ocv_02\nimp_ocv_03\nimp_ocv_04\nimp_ocv_05\nimp_ocv_06\nimp_ocv_07"},
	{"imp_dma",           2,  22,  "imp_dma_00\nimp_dma_01"},
	{"imp_psc",           2,  22,  "imp_psc_00\nimp_psc_01"},
	{"imp_cnn",           3,  33,  "imp_cnn_00\nimp_cnn_01\nimp_cnn_02"},
	{"imp_reg_spmi",      2,  32,  "imp_reg_spmi_00\nimp_reg_spmi_01"},
	{"imp_reg_spmc",      3,  48,  "imp_reg_spmc_00\nimp_reg_spmc_01\nimp_reg_spmc_02"},
	{"imp_dta",           1,  11,  "imp_dta_00"},
	{"cisp",              4,  32,  "cisp_00\ncisp_01\ncisp_02\ncisp_03"},
	{"tisp",              4,  32,  "tisp_00\ntisp_01\ntisp_02\ntisp_03"},
	{"vspx",              4,  32,  "vspx_00\nvspx_01\nvspx_02\nvspx_03"},
	{"fbc",               1,  7,   "fbc_00"},
    {"fba",              68, 929,  "fba_ims0\nfba_ims1\nfba_imr0\nfba_imr1\nfba_imr2\nfba_imr3\nfba_imp0\nfba_imp1\nfba_imp2\nfba_imp3\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv0\nfba_imp_ocv3\nfba_imp_ocv4\nfba_imp_ocv5\nfba_imp_ocv6\nfba_imp_ocv7\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0_main\nfba_imp_cnn0_sub0\nfba_imp_cnn0_sub1\nfba_imp_cnn0_sub2\nfba_imp_cnn0_sub3\nfba_imp_cnn1_main\nfba_imp_cnn1_sub0\nfba_imp_cnn1_sub1\nfba_imp_cnn1_sub2\nfba_imp_cnn1_sub3\nfba_imp_cnn2_main\nfba_imp_cnn2_sub0\nfba_imp_cnn2_sub1\nfba_imp_cnn2_sub2\nfba_imp_cnn2_sub3\nfba_imp_dsp0\nfba_imp_dsp1\nfba_imp_cnram0\nfba_imp_cnram1\nfba_imp_cnram2\nfba_dof0\nfba_dof1\nfba_stv0\nfba_stv1\nfba_acf0\nfba_acf1\nfba_smps0\nfba_smps1\nfba_smes0\nfba_smes1\nfba_smpo0\nfba_smpo1\nfba_isp0\nfba_isp1\nfba_isp2\nfba_isp3\nfba_ca76_0_cl0\nfba_ca76_0_cl1\nfba_ca76_1_cl0\nfba_ca76_1_cl1\nfba_ca76_0_cl0_cpu0\nfba_ca76_0_cl0_cpu1\nfba_ca76_0_cl1_cpu0\nfba_ca76_0_cl1_cpu1\nfba_ca76_1_cl0_cpu0\nfba_ca76_1_cl0_cpu1\nfba_ca76_1_cl1_cpu0\nfba_ca76_1_cl1_cpu1"},
	{"rfso",             11,  88,  "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10"},
	{"ipmmu_mm",          1,  12,  "ipmmu_mm_00"},
	{"ipmmu_cache",      11, 177,  "ipmmu_cache_vi0\nipmmu_cache_vi1\nipmmu_cache_vc0\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_rt1\nipmmu_cache_ds0\nipmmu_cache_ds1\nipmmu_cache_vip0\nipmmu_cache_vip1\nipmmu_cache_3dg"},
	{"fcprr",            23, 311,  "fcprr_dof_00\nfcprr_dof_01\nfcprr_stv_00\nfcprr_stv_01\nfcprr_acf_00\nfcprr_acf_01\nfcprr_acf_02\nfcprr_acf_03\nfcprr_imp_00\nfcprr_imp_01\nfcprr_ims_00\nfcprr_ims_01\nfcprr_imr_00\nfcprr_imr_01\nfcprr_imr_02\nfcprr_imr_03\nfcprr_vspd_00\nfcprr_vspd_01\nfcprr_vspx_00\nfcprr_vspx_01\nfcprr_vspx_02\nfcprr_vspx_03\nfcprr_rtdmac_00_03"},
	{"fcprw",             8, 116,  "fcprw_dof_00\nfcprw_dof_01\nfcprw_imp_00\nfcprw_imp_01\nfcprw_imr_00_01\nfcprw_imr_02_03\nfcprw_vin_00_29\nfcprw_isp_00_03"},
	{"fcpra",             7,  75,  "fcpra_vip0\nfcpra_vip1\nfcpra_vip2\nfcpra_imp\nfcpra_vc\nfcpra_vio\nfcpra_rtdmac"}

#elif defined(RCAR_V3H2)/* V3H */
	{"ims",               3,  21,  "ims_00\nims_01\nims_02"},
	{"imr",               2,  14,  "imr_00\nimr_01"},
	{"dof",               1,  7,   "dof_00"},
	{"stv",               1,  7,   "stv_00"},
	{"acf",               5,  35,  "acf_00\nacf_01\nacf_02\nacf_03\nacf_04"},
	{"iccom",             1,  9,   "iccom_00"},
	{"ivcp1e",            1,  10,  "ivcp1e_00"},
	{"imp_top",           1,  11,  "imp_top_00"},
	{"imp_spmi",          1,  12,  "imp_spmi_00"},
	{"imp",               5,  35,  "imp_00\nimp_01\nimp_02\nimp_03\nimp_04"},
	{"imp_ocv",           5,  55,  "imp_ocv_00\nimp_ocv_01\nimp_ocv_02\nimp_ocv_03\nimp_ocv_04"},
	{"imp_dma",           2,  22,  "imp_dma_00\nimp_dma_01"},
	{"imp_psc",           1,  11,  "imp_psc_00"},
	{"imp_cnn",           1,  11,  "imp_cnn_00"},
	{"imp_slim",          1,  12,  "imp_slim_00"},
	{"imp_dma_slim",      1,  16,  "imp_dma_slim_00"},
	{"cisp",              2,  16,  "cisp_00\ncisp_01"},
	{"tisp",              2,  16,  "tisp_00\ntisp_01"},
	{"fbc",               1,  7,   "fbc_00"},
    {"fba",              28, 292,  "fba_imr0\nfba_imr1\nfba_ims0\nfba_ims1\nfba_ims2\nfba_imp0\nfba_imp1\nfba_imp2\nfba_imp3\nfba_imp4\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_ocv2\nfba_imp_ocv3\nfba_imp_ocv4\nfba_imp_dp0\nfba_imp_dp1\nfba_imp_cnn0\nfba_imp_slim0\nfba_dof0\nfba_stv0\nfba_acf0\nfba_acf1\nfba_ca53d\nfba_ca530\nfba_ca531\nfba_ca532\nfba_ca533"},
	{"rfso",             11,  88,  "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10"},
	{"ipmmu_mm",          1,  12,  "ipmmu_mm_00"},
	{"ipmmu_cache",       7, 113,  "ipmmu_cache_vi0\nipmmu_cache_vc0\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_ds1\nipmmu_cache_vip0\nipmmu_cache_vip1"},
	{"fcprr",            18, 240,  "fcprr_stv_00\nfcprr_dof_00\nfcprr_acf_00\nfcprr_acf_01\nfcprr_acf_02\nfcprr_acf_03\nfcprr_acf_04\nfcprr_imp_00\nfcprr_imp_01\nfcprr_ims_00\nfcprr_ims_01\nfcprr_ims_02\nfcprr_imr_00\nfcprr_imr_01\nfcprr_vspd_00\nfcprr_ivcp1e_00\nfcprr_pcie_00\nfcprr_ca53_00"},
	{"fcprw",             7,  91,  "fcprw_dof_00\nfcprw_imp_00\nfcprw_imp_01\nfcprw_imr_00\nfcprw_imr_01\nfcprw_isp_00\nfcprw_isp_01"},
	{"fcpra",             6,  63,  "fcpra_vip0\nfcpra_vip1\nfcpra_imp\nfcpra_imr\nfcpra_vio\nfcpra_ca53"}

#elif defined(RCAR_V3M2)/* V3M */
	{"ims",               4,  28,  "ims_00\nims_01\nims_02\nims_03"},
	{"iccom",             1,  9,   "iccom_00"},
	{"ivcp1e",            1,  10,  "ivcp1e_00"},
	{"imp_top",           1,  11,  "imp_top_00"},
	{"imp",               4,  28,  "imp_00\nimp_01\nimp_02\nimp_03"},
	{"imp_ocv",           2,  22,  "imp_ocv_00\nimp_ocv_01"},
	{"imp_dma",           1,  11,  "imp_dma_00"},
	{"imp_psc",           1,  11,  "imp_psc_00"},
	{"imp_cnn",           1,  11,  "imp_cnn_00"},
	{"imp_spmi",          1,  12,  "imp_spmi_00"},
	{"cisp",              1,  8,   "cisp_00"},
	{"tisp",              1,  8,   "tisp_00"},
	{"fbc",               1,  7,   "fbc_00"},
    {"fba",              11, 117,  "fba_ims0\nfba_ims1\nfba_imp0\nfba_imp1\nfba_imp_ocv0\nfba_imp_ocv1\nfba_imp_dp0\nfba_imp_cnn0\nfba_ca53d\nfba_ca530\nfba_ca531"},
	{"rfso",             11,  88,  "rfso_00\nrfso_01\nrfso_02\nrfso_03\nrfso_04\nrfso_05\nrfso_06\nrfso_07\nrfso_08\nrfso_09\nrfso_10"},
	{"ipmmu_mm",          1,  12,  "ipmmu_mm_00"},
	{"ipmmu_cache",       4,  63,  "ipmmu_cache_vi0\nipmmu_cache_ir\nipmmu_cache_rt0\nipmmu_cache_ds1"}
#else
#error  Device is not defined
#endif
};

/* prototype */
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

void osal_test_io_manager_display_menu()
{
	printf("*** OSAL IO Manager ***\n");
	printf("***  06_001: R_OSAL_IoGetNumOfDevices   ***\n");
	printf("***  06_002: R_OSAL_IoGetDeviceList     ***\n");
	printf("***  06_003: R_OSAL_IoDeviceOpen        ***\n");
	printf("***  06_004: R_OSAL_IoDeviceClose       ***\n");
	printf("***  06_005: R_OSAL_IoRead8             ***\n");
	printf("***  06_006: R_OSAL_IoRead16            ***\n");
	printf("***  06_007: R_OSAL_IoRead32            ***\n");
	printf("***  06_008: R_OSAL_IoRead64            ***\n");
	printf("***  06_009: R_OSAL_IoBlockRead8        ***\n");
	printf("***  06_010: R_OSAL_IoBlockRead16       ***\n");
	printf("***  06_011: R_OSAL_IoBlockRead32       ***\n");
	printf("***  06_012: R_OSAL_IoBlockRead64       ***\n");
	printf("***  06_013: R_OSAL_IoWrite8            ***\n");
	printf("***  06_014: R_OSAL_IoWrite16           ***\n");
	printf("***  06_015: R_OSAL_IoWrite32           ***\n");
	printf("***  06_016: R_OSAL_IoWrite64           ***\n");
	printf("***  06_017: R_OSAL_IoBlockWrite8       ***\n");
	printf("***  06_018: R_OSAL_IoBlockWrite16      ***\n");
	printf("***  06_019: R_OSAL_IoBlockWrite32      ***\n");
	printf("***  06_020: R_OSAL_IoBlockWrite64      ***\n");
	printf("***  06_100: io manager module all exec ***\n\n");
}

void osal_test_io_manager_menu(int child)
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
		test_number = OSAL_TEST_IOGETNUMOFDEVICES;
		test_count  = OSAL_TEST_IO_MAX;
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
			default:
				printf("That menu is not available.\n\n");
				break;
		}
		test_number++;
	}
}
/* test CB func */
void osal_test_cb_sample_io(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{

	printf("%s() user call back function exec!! dev_handle:%p irq_channel:%ld  irq_arg:%p \n", 
	    __func__, (void *)device_handle, irq_channel, irq_arg);

	return;
}

e_osal_return_t osal_init_io(void)
{
	e_osal_return_t ret = OSAL_RETURN_OK;
	/*
	osal_device_handle_t dev_handle;
	e_osal_return_t osal_ret;
	e_osal_pm_policy_t policy;
*/
	if(false == init_flag)
	{
		ret = R_OSAL_Initialize();
		init_flag = true;
		/*
		if(ret == OSAL_RETURN_OK)
		{
			init_flag = true;
			osal_ret = R_OSAL_IoDeviceOpen("ims_00", &dev_handle);
			if (osal_ret != OSAL_RETURN_OK)
			{
				OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.", OSAL_TEST_IO, 0, 0);
				return osal_ret;
			}
			
			osal_ret = R_OSAL_PmGetPolicy(dev_handle, &policy);
			if (osal_ret != OSAL_RETURN_OK)
			{
				OSAL_TEST_ERR("R_OSAL_PmGetPolicy() error.", OSAL_TEST_IO, 0, 0);
				return osal_ret;
			}
			
			if (policy != OSAL_PM_POLICY_HP)
			{
				osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_HP, true);
				if (osal_ret != OSAL_RETURN_OK)
				{
					OSAL_TEST_ERR("R_OSAL_PmSetPolicy() error.",  OSAL_TEST_IO, 0, 0);
					return osal_ret;
				}

				osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
				if (osal_ret != OSAL_RETURN_OK)
				{
					OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",  OSAL_TEST_IO, 0, 0);
					return osal_ret;
				}

				osal_ret= R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);
				if (osal_ret != OSAL_RETURN_OK)
				{
					OSAL_TEST_ERR("R_OSAL_PmSetRequiredState() error.",  OSAL_TEST_IO, 0, 0);
					return osal_ret;
				}

			}

			osal_ret = R_OSAL_IoDeviceClose(dev_handle);
			if (osal_ret != OSAL_RETURN_OK)
			{
				OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
						OSAL_TEST_IO, 0, 0);
				return osal_ret;
			}
		}
*/
	}

    return ret;
}

e_osal_return_t osal_deinit_io(void)
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

/* ------------------------06_001_001 [success]------------------------ */
static void Test06_001_001()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t        			numOfDevices;
	size_t					numOfTest;
	int						count;


	numOfTest   = sizeof(gs_osal_test_info) / sizeof(st_osal_test_info_t);
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETNUMOFDEVICES,
														OSAL_TEST_ID001);		
	for (count = 0; count < (int)numOfTest; count++)
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoGetNumOfDevices(gs_osal_test_info[count].device_type, &numOfDevices);
		TIME_MEASURE_STOP
		if ((osal_ret == expect_data) && (numOfDevices == gs_osal_test_info[count].numOfDevices))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
            printf("target %s(expect num:%zu): %zu\n", gs_osal_test_info[count].device_type,
                                                       gs_osal_test_info[count].numOfDevices,
                                                       numOfDevices);
			test_ret = OSAL_TEST_NG;
			break;
		}

	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETNUMOFDEVICES,
							OSAL_TEST_ID001,
							test_ret, opt_info);
}

/* ------------------------06_001_002 [OSAL_RETURN_PAR]------------------------ */
static void Test06_001_002()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t        			numOfDevices;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETNUMOFDEVICES,
														OSAL_TEST_ID002);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoGetNumOfDevices(NULL, &numOfDevices);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETNUMOFDEVICES,
							OSAL_TEST_ID002, test_ret, opt_info);
}

/* ------------------------06_001_003 [OSAL_RETURN_OK]------------------------ */
static void Test06_001_003()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t        			numOfDevices;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETNUMOFDEVICES,
														OSAL_TEST_ID003);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoGetNumOfDevices("aaa", &numOfDevices);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETNUMOFDEVICES,
							OSAL_TEST_ID003, test_ret, opt_info);
}

/* ------------------------06_001_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_001_004()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETNUMOFDEVICES,
														OSAL_TEST_ID004);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoGetNumOfDevices("imr", NULL);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETNUMOFDEVICES,
							OSAL_TEST_ID004, test_ret, opt_info);
}
#if 0
/* ------------------------06_001_028 [OSAL_RETURN_OK]------------------------ */
void Test06_001_028_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t numOfDevices;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOGETNUMOFDEVICES,
															OSAL_TEST_ID028);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoGetNumOfDevices("imr", &numOfDevices);
	TIME_MEASURE_STOP

	if ((target_ret == expect_data) && (numOfDevices == 6))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETNUMOFDEVICES,
							OSAL_TEST_ID028,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_001_028()
{
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETNUMOFDEVICES, OSAL_TEST_ID028);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_001_028_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETNUMOFDEVICES, OSAL_TEST_ID028);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETNUMOFDEVICES, OSAL_TEST_ID028);
		return;
	}
}
#endif

#if 0
/* ------------------------06_001_029 [OSAL_RETURN_FAIL]------------------------ */
static void Test06_001_029()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t        			numOfDevices;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETNUMOFDEVICES,
														OSAL_TEST_ID029);

	/* Set stub flag */
	g_stub_flag.R_OSAL_DCFG_GetDeviceInfo = R_OSAL_ERR_NULL_CONTROL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoGetNumOfDevices("imr", &numOfDevices);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.R_OSAL_DCFG_GetDeviceInfo = 0;
	
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETNUMOFDEVICES,
							OSAL_TEST_ID029, test_ret, opt_info);
}
#endif

/* ------------------------06_002_001 [success]------------------------ */
static void Test06_002_001()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[500] = {'\0'};
	size_t          		list_size;
	size_t          		numOfDevices;
	size_t          		numOfLists;
	size_t					numOfTest;
	int						count_dev;

	numOfTest   = sizeof(gs_osal_test_info) / sizeof(st_osal_test_info_t);
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID001);
	for (count_dev = 0; count_dev < (int)numOfTest; count_dev++)
	{
        printf("target type: %s\n", gs_osal_test_info[count_dev].device_type);
	
		osal_ret = R_OSAL_IoGetNumOfDevices(gs_osal_test_info[count_dev].device_type, &numOfDevices);
		if ((osal_ret != OSAL_RETURN_OK) || (numOfDevices !=  gs_osal_test_info[count_dev].numOfDevices))
		{
			OSAL_TEST_ERR("R_Osal_IoGetNumOfDevices() error.",
					OSAL_TEST_IO, OSAL_TEST_IOGETDEVICELIST, OSAL_TEST_ID001);
			return;
		}

		list_size = sizeof(list);
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret  = R_OSAL_IoGetDeviceList(gs_osal_test_info[count_dev].device_type, list, list_size, &numOfLists);
		TIME_MEASURE_STOP

		if ((osal_ret == expect_data) && (numOfLists == gs_osal_test_info[count_dev].numOfList))
		{
			if(!strcmp(list, gs_osal_test_info[count_dev].list))
			{
				test_ret = OSAL_TEST_OK;
			}
			else
			{
                printf("target list(num:%zu): %s\n", gs_osal_test_info[count_dev].numOfList, gs_osal_test_info[count_dev].list);
                printf("actual list(num:%zu): %s\n", numOfLists, list);
				test_ret = OSAL_TEST_NG;
				break;
			}
		}
		else
		{
			test_ret = OSAL_TEST_NG;
			break;
		}

		memset(list, '\0', list_size);
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID001,
							test_ret, opt_info);
}

/* ------------------------06_002_002 [OSAL_RETURN_PAR]------------------------ */
static void Test06_002_002()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[100];
	size_t          		numOfLists;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID002);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_IoGetDeviceList(NULL, list, sizeof(list), &numOfLists);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID002, test_ret, opt_info);
}

/* ------------------------06_002_003 [OSAL_RETURN_OK]------------------------ */
static void Test06_002_003()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[100];
	size_t          		numOfLists;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID003);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_IoGetDeviceList("aaa", list, sizeof(list), &numOfLists);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID003, test_ret, opt_info);
}

/* ------------------------06_002_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_002_004()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char 					list[100];
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t          		numOfLists;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID004);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_IoGetDeviceList("ims", NULL, sizeof(list), &numOfLists);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID004, test_ret, opt_info);
}

/* ------------------------06_002_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_002_005()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[100];
	size_t          		numOfLists;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID005);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_IoGetDeviceList("ims", list, 0, &numOfLists);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID005, test_ret, opt_info);
}

/* ------------------------06_002_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_002_006()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[100];
	size_t          		numOfLists = gs_osal_test_info[1].numOfList;
	size_t                  numOfDevices;
	size_t					list_size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID006);
	osal_ret = R_OSAL_IoGetNumOfDevices(gs_osal_test_info[1].device_type, &numOfDevices);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR(" R_OSAL_IoGetNumOfDevices(): ERROR",
						OSAL_TEST_IO, OSAL_TEST_IOGETDEVICELIST, OSAL_TEST_ID006);
		return;
	}

	list_size = numOfLists - 1;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_IoGetDeviceList(gs_osal_test_info[1].device_type, list, list_size, &numOfLists);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
	    printf("list_size %zu\n", list_size);
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID006, test_ret, opt_info);
}

/* ------------------------06_002_007 [OSAL_RETURN_PAR]------------------------ */
static void Test06_002_007()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[100];
	size_t          		numOfLists;
	size_t					list_size;
	size_t					numOfDevices;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID007);

	osal_ret = R_OSAL_IoGetNumOfDevices("ims", &numOfDevices);
	if (OSAL_RETURN_OK != osal_ret)
	{
		OSAL_TEST_ERR(" R_OSAL_IoGetNumOfDevices(): ERROR",
						OSAL_TEST_IO, OSAL_TEST_IOGETDEVICELIST, OSAL_TEST_ID007);
		return;
	}

	list_size = sizeof(char *) * numOfDevices;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_IoGetDeviceList("ims", list, (list_size - list_size / numOfDevices), &numOfLists);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID007, test_ret, opt_info);
}

/* ------------------------06_002_008 [OSAL_RETURN_OK]------------------------ */
static void Test06_002_008()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[100];
	size_t          		numOfLists;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID008);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_IoGetDeviceList("ims", list, sizeof(list) + 1, &numOfLists);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID008, test_ret, opt_info);
}

/* ------------------------06_002_009 [OSAL_RETURN_PAR]------------------------ */
static void Test06_002_009()
{
	int      				test_ret    = OSAL_TEST_OK;
	e_osal_return_t			osal_ret;
	e_osal_return_t 		expect_data;
	char          			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[100];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IOGETDEVICELIST,
														OSAL_TEST_ID009);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_IoGetDeviceList("ims", list, sizeof(list), NULL);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID009, test_ret, opt_info);
}
#if 0
/* ------------------------06_002_033 [OSAL_RETURN_OK]------------------------ */
void Test06_002_033_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int              i;
	int              test_ret;
	int              expect_ret = OSAL_TEST_OK;
	e_osal_return_t  target_ret;
	e_osal_return_t  expect_data;
	char             opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char* list[32];
	char* expect_list[6] = {
		"ims_00",
		"imr_ims_01",
		"imr_01",
		"imr_imr_01",
		"imr_imr_02",
		"imr_imr_03"
	};
	size_t           numOfLists;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOGETDEVICELIST,
															OSAL_TEST_ID033);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoGetDeviceList("imr", list, sizeof(list), &numOfLists);
	TIME_MEASURE_STOP

	for (i = 0; i < 6; i++)
	{
		if (strcmp(expect_list[i], list[i]) != 0)
		{
			expect_ret = OSAL_TEST_NG;
		}
	}

	if ((target_ret == expect_data) && (numOfLists == 6) && (expect_ret == OSAL_TEST_OK))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOGETDEVICELIST,
							OSAL_TEST_ID033,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_002_033()
{
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETDEVICELIST, OSAL_TEST_ID033);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_002_033_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETDEVICELIST, OSAL_TEST_ID033);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETDEVICELIST, OSAL_TEST_ID033);
		return;
	}
}
#endif

/* ------------------------06_003_001 [success]------------------------ */
static void Test06_003_001()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[500] = {'\0'};
	size_t          		list_size;
	size_t          		numOfDevices;
	size_t          		numOfLists;
	size_t					numOfTest;
	int						count_dev;
	size_t					count_list;
	osal_device_handle_t 	handle;
	char*					device[32] = {'\0'};
	const char 				s[2] = "\n";
	int						i = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID001);
/*TODO: debug
	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD8,
														   OSAL_TEST_ID001);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID001);
		return;
	}
*/
	
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	numOfTest = sizeof(gs_osal_test_info) / sizeof(st_osal_test_info_t);
	for (count_dev = 0; count_dev < (int)numOfTest; count_dev++)
	{
		osal_ret = R_OSAL_IoGetNumOfDevices(gs_osal_test_info[count_dev].device_type, &numOfDevices);
		if ((osal_ret != OSAL_RETURN_OK) || (numOfDevices !=  gs_osal_test_info[count_dev].numOfDevices))
		{
			OSAL_TEST_ERR("R_Osal_IoGetNumOfDevices() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
			return;
		}

		list_size = sizeof(list);
		osal_ret  = R_OSAL_IoGetDeviceList(gs_osal_test_info[count_dev].device_type, list, list_size, &numOfLists);

		device[i] = strtok(list, s);
		while( device[i] != NULL ) {
			i++;
			device[i] = strtok(NULL, s);
		}

		if(i > (int)numOfDevices){
			OSAL_TEST_ERR("parsing device error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
			return;
		}

		for (count_list = 0; count_list < numOfDevices; count_list++)
		{
			TIME_MEASURE_INIT
			TIME_MEASURE_START
			osal_ret = R_OSAL_IoDeviceOpen(device[count_list], &handle);
			TIME_MEASURE_STOP
			if ((osal_ret == expect_data) && (handle != NULL))
			{
				test_ret = OSAL_TEST_OK;
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}

			osal_ret = R_OSAL_IoDeviceClose(handle);
			if (osal_ret != OSAL_RETURN_OK)
			{
				OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
						OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID001);
				return;
			}
		}

		if (OSAL_TEST_NG == test_ret)
		{
			break;
		}
		i = 0;
		memset(list, '\0', list_size);
		memset(device, '\0', sizeof(device));
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
					OSAL_TEST_IODEVICEOPEN,
					OSAL_TEST_ID001, test_ret, opt_info);

}

/* ------------------------06_003_002 [OSAL_RETURN_PAR]------------------------ */
static void Test06_003_002()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID002);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen(NULL, &handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID002, test_ret, opt_info);
}

/* ------------------------06_003_003 [OSAL_RETURN_ID]------------------------ */
static void Test06_003_003()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID003);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen("aaa", &handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID003, test_ret, opt_info);
}

/* ------------------------06_003_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_003_004()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID004);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", NULL);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID004, test_ret, opt_info);
}

/* ------------------------06_003_005 [OSAL_RETURN_BUSY]------------------------ */
static void Test06_003_005()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID005);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_01", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoDeviceOpen("ims_01", &handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID005, test_ret, opt_info);
	
	if(osal_ret != OSAL_RETURN_OK)
	{			
		osal_ret = R_OSAL_IoDeviceClose(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID005);
		}
	}
}

/* ------------------------06_003_006 [OSAL_RETURN_OK]------------------------ */
static void Test06_003_006()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID006);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_01", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoDeviceOpen("ims_01", &handle);
		TIME_MEASURE_STOP
		if ((osal_ret == expect_data) && (handle != NULL))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID006, test_ret, opt_info);
							
	if(osal_ret == OSAL_RETURN_OK)
	{			
		osal_ret = R_OSAL_IoDeviceClose(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID006);
		}
	}
}

/* ------------------------06_003_007 [OSAL_RETURN_STATE]------------------------ */
static void Test06_003_007()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID007);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID007, test_ret, opt_info);
}

/* ------------------------06_003_008 [OSAL_RETURN_STATE]------------------------ */
static void Test06_003_008()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID008);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID008);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID008, test_ret, opt_info);
}

#if 0
/* ------------------------06_003_070 [OSAL_RETURN_FAIL]------------------------ */
static void Test06_003_070()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID070);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID070);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID070, test_ret, opt_info);

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID070);
		return;
	}
}

/* ------------------------06_003_071 [OSAL_RETURN_CONF]------------------------ */
static void Test06_003_071()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICEOPEN,
														OSAL_TEST_ID071);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	/****************************/
	/* Pre-process				*/
	/****************************/
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID071);
		return;
	}

	/* Set stub flag */
	g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = R_OSAL_ERR_OTHER_CONTROL;

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	TIME_MEASURE_STOP

	/****************************/
	/* Post-process				*/
	/****************************/
	/* Clear stub flag */
	g_stub_flag.R_OSAL_RCFG_GetInterruptThreadInfo = 0;
	
	/****************************/
	/* Check result				*/
	/****************************/
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICEOPEN,
							OSAL_TEST_ID071, test_ret, opt_info);

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, OSAL_TEST_ID071);
		return;
	}
}
#endif

/* ------------------------06_004_001 [success]------------------------ */
static void Test06_004_001()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	char 					list[500] = {'\0'};
	size_t          		list_size;
	size_t          		numOfDevices;
	size_t          		numOfLists;
	size_t					numOfTest;
	int						count_dev;
	size_t					count_list;
	osal_device_handle_t 	handle;
	char*					device[32] = {'\0'};
	const char 				s[2] = "\n";
	int						i = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICECLOSE,
														OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	numOfTest = sizeof(gs_osal_test_info) / sizeof(st_osal_test_info_t);
	for (count_dev = 0; count_dev < (int)numOfTest; count_dev++)
	{
		osal_ret = R_OSAL_IoGetNumOfDevices(gs_osal_test_info[count_dev].device_type, &numOfDevices);
		if ((osal_ret != OSAL_RETURN_OK) || (numOfDevices !=  gs_osal_test_info[count_dev].numOfDevices))
		{
			OSAL_TEST_ERR("R_Osal_IoGetNumOfDevices() error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID001);
			return;
		}

		list_size = sizeof(list);
		osal_ret  = R_OSAL_IoGetDeviceList(gs_osal_test_info[count_dev].device_type, list, list_size, &numOfLists);
		device[i] = strtok(list, s);
		while( device[i] != NULL ) {
			i++;
			device[i] = strtok(NULL, s);
		}

		if(i > (int)numOfDevices){
			OSAL_TEST_ERR("parsing device error.",
					OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID001);
			return;
		}

		for (count_list = 0; count_list < numOfDevices; count_list++)
		{
			osal_ret = R_OSAL_IoDeviceOpen(device[count_list], &handle);
			if (osal_ret != OSAL_RETURN_OK)
			{
				OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
						OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID001);
				return;
			}
			TIME_MEASURE_INIT
			TIME_MEASURE_START
			osal_ret = R_OSAL_IoDeviceClose(handle);
			TIME_MEASURE_STOP
			if (osal_ret == expect_data)
			{
				test_ret = OSAL_TEST_OK;
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}
		}

		if (OSAL_TEST_NG == test_ret)
		{
			break;
		}
		i = 0;
		memset(list, '\0', list_size);
		memset(device, '\0', sizeof(device));
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
					OSAL_TEST_IODEVICECLOSE,
					OSAL_TEST_ID001, test_ret, opt_info);
}

/* ------------------------06_004_002 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_004_002()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICECLOSE,
														OSAL_TEST_ID002);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceClose(NULL);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICECLOSE,
							OSAL_TEST_ID002, test_ret, opt_info);
}

/* ------------------------06_004_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_004_003()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICECLOSE,
														OSAL_TEST_ID003);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID003);
		return;
	}
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID003);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoDeviceClose(handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICECLOSE,
							OSAL_TEST_ID003, test_ret, opt_info);
}

/* ------------------------06_004_004 [OSAL_RETURN_STATE]------------------------ */
static void Test06_004_004()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICECLOSE,
														OSAL_TEST_ID004);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceClose(handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICECLOSE,
							OSAL_TEST_ID004, test_ret, opt_info);
}

#if 0
/* ------------------------06_004_066 [OSAL_RETURN_FAIL]------------------------ */
static void Test06_004_066()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICECLOSE,
														OSAL_TEST_ID066);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID066);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID066);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceClose(handle);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICECLOSE,
							OSAL_TEST_ID066, test_ret, opt_info);

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID066);
		return;
	}
}

/* ------------------------06_004_067 [OSAL_RETURN_DEV]------------------------ */
static void Test06_004_067()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICECLOSE,
														OSAL_TEST_ID067);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	
	/****************************/
	/* Pre-process				*/
	/****************************/
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID067);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID067);
		return;
	}

	/* Set stub flag */
	g_stub_flag.R_PMA_UnlockHwa = R_PMA_BUS_IF_CHK_NG;

	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceClose(handle);
	TIME_MEASURE_STOP
	/****************************/
	/* Post-process				*/
	/****************************/
	/* Clear stub flag */
	g_stub_flag.R_PMA_UnlockHwa = 0;
	
	/****************************/
	/* Check result				*/
	/****************************/
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICECLOSE,
							OSAL_TEST_ID067, test_ret, opt_info);

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID067);
		return;
	}
}

/* ------------------------06_004_068 [OSAL_RETURN_BUSY]------------------------ */
static void Test06_004_068()
{
	int      				test_ret = OSAL_TEST_OK;
	e_osal_return_t   		osal_ret;
	e_osal_return_t   		expect_data;
	char            		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	int 					arg = 10;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														OSAL_TEST_IODEVICECLOSE,
														OSAL_TEST_ID068);
	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	
	/****************************/
	/* Pre-process				*/
	/****************************/
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID068);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID068);
		return;
	}
	osal_ret = R_OSAL_InterruptRegisterIsr(handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
						(p_osal_isr_func_t)osal_test_cb_sample_io,
						(void *)&arg);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptRegisterIsr() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID068);
		return;
	}
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoDeviceClose(handle);
	TIME_MEASURE_STOP
	/****************************/
	/* Post-process				*/
	/****************************/

	/****************************/
	/* Check result				*/
	/****************************/
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IODEVICECLOSE,
							OSAL_TEST_ID068, test_ret, opt_info);

	osal_ret = R_OSAL_InterruptUnregisterIsr(handle, 0, (p_osal_isr_func_t)osal_test_cb_sample_io);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_InterruptUnregisterIsr() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID068);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID068);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, OSAL_TEST_ID068);
		return;
	}
}
#endif

/* ------------------------06_005_001 [success]------------------------ */
static void Test06_005_001()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD8,
														   OSAL_TEST_ID001);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID001);
		return;
	}

	/* TODO: debug */
	offset = 0x0030;
	osal_ret = R_OSAL_IoRead8(handle, offset, &read_data);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("Before R_OSAL_IoRead8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID001);
		return;
	}

	/* ------ */

	offset = 0x0030;
	osal_ret = R_OSAL_IoWrite8(handle, offset, 0xAF);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead8(handle, offset, &read_data);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (read_data == 0xA8))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD8,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_005_002 [success]------------------------ */
static void Test06_005_002()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD8,
														   OSAL_TEST_ID002);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID002);
		return;
	}

	/* TODO: debug */
	offset = 0x0030;
	osal_ret = R_OSAL_IoRead8(handle, offset, &read_data);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("Before R_OSAL_IoRead8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID002);
		return;
	}

	/* ------ */
	offset = 0x0030;
	osal_ret = R_OSAL_IoWrite8(handle, offset, 0xD0);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID002);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead8(handle, offset, &read_data);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (read_data == 0xD0))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD8,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID002);
		return;
	}
}
/* ------------------------06_005_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_005_003()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint8_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD8,
														   OSAL_TEST_ID003);

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead8(NULL, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD8,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------06_005_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_005_004()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD8,
														   OSAL_TEST_ID004);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID004);
		return;
	}

	offset = 0x10000;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead8(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD8,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------06_005_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_005_005()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD8,
														   OSAL_TEST_ID005);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID005);
		return;
	}

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead8(handle, offset, NULL);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD8,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_005_006 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_005_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD8,
														   OSAL_TEST_ID006);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoRead8(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD8,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

/* ------------------------06_005_007 [OSAL_RETURN_STATE]------------------------ */
static void Test06_005_007()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD8,
														   OSAL_TEST_ID007);

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID007);
		return;
	}

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead8(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD8,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}
#if 0
/* ------------------------06_005_008 [OSAL_RETURN_OK]------------------------ */
void Test06_005_008_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint8_t read_data;
	uint8_t expect_read_data;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOREAD8,
															OSAL_TEST_ID008);

	offset = 0x00C0;
	read_data = 0x00;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoRead8(g_io_cb_handle, offset, &read_data);
	TIME_MEASURE_STOP
	expect_read_data = 0xFF;

	if ((target_ret == expect_data) && (read_data == expect_read_data))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD8,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_005_008()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_005_008_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID008);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID008);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, OSAL_TEST_ID008);
		return;
	}
}
#endif

/* ------------------------06_006_001 [success]------------------------ */
static void Test06_006_001()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD16,
														   OSAL_TEST_ID001);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID001);
		return;
	}

	/* TODO: debug */
	offset = 0x0030;
	osal_ret = R_OSAL_IoRead16(handle, offset, &read_data);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("Before R_OSAL_IoRead16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID001);
		return;
	}

	/* ------ */
	offset = 0x0030;
	osal_ret = R_OSAL_IoWrite16(handle, offset, 0x0A90);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead16(handle, offset, &read_data);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (read_data == 0x0A90))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD16,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_006_002 [success]------------------------ */
static void Test06_006_002()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD16,
														   OSAL_TEST_ID002);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID002);
		return;
	}
	/* TODO: debug */
	offset = 0x0030;
	osal_ret = R_OSAL_IoRead16(handle, offset, &read_data);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("Before R_OSAL_IoRead16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID002);
		return;
	}

	/* ------ */
	offset = 0x0030;
	osal_ret = R_OSAL_IoWrite16(handle, offset, 0xA420);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID002);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead16(handle, offset, &read_data);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (read_data == 0xA420))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD16,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_006_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_006_003()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint16_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD16,
														   OSAL_TEST_ID003);

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead16(NULL, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD16,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------06_006_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_006_004()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD16,
														   OSAL_TEST_ID004);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID004);
		return;
	}

	offset = 0x10000;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead16(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD16,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------06_006_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_006_005()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD16,
														   OSAL_TEST_ID005);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID005);
		return;
	}

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead16(handle, offset, NULL);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD16,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_006_006 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_006_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD16,
														   OSAL_TEST_ID006);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoRead16(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD16,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

/* ------------------------06_006_007 [OSAL_RETURN_STATE]------------------------ */
static void Test06_006_007()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD16,
														   OSAL_TEST_ID007);

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID007);
		return;
	}

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead16(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD16,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}

#if 0
/* ------------------------06_006_008 [OSAL_RETURN_OK]------------------------ */
void Test06_006_008_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint16_t read_data;
	uint16_t expect_read_data;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOREAD16,
															OSAL_TEST_ID008);

	offset = 0x00C0;
	read_data = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoRead16(g_io_cb_handle, offset, &read_data);
	TIME_MEASURE_STOP
	expect_read_data = 0x00FF;

	if ((target_ret == expect_data) && (read_data == expect_read_data))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD16,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_006_008()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_006_008_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID008);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID008);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, OSAL_TEST_ID008);
		return;
	}
}
#endif

/* ------------------------06_007_001 [success]------------------------ */
static void Test06_007_001()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD32,
														   OSAL_TEST_ID001);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID001);
		return;
	}

	/* TODO: debug */
	offset = 0x0030;
	osal_ret = R_OSAL_IoRead32(handle, offset, &read_data);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("Before R_OSAL_IoRead32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID001);
		return;
	}

	/* -------- */
	offset = 0x0030;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite32(handle, offset, 0xFFFFF190);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID001);
		return;
	}
	osal_ret = R_OSAL_IoRead32(handle, offset, &read_data);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (read_data == 0xFFFFF190))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD32,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_007_002 [success]------------------------ */
static void Test06_007_002()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD32,
														   OSAL_TEST_ID002);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID002);
		return;
	}

	/* TODO: debug */
	offset = 0x0030;
	osal_ret = R_OSAL_IoRead32(handle, offset, &read_data);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("Before R_OSAL_IoRead32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID002);
		return;
	}

	/* ------ */
	offset = 0x0030;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite32(handle, offset, 0xFFFFFF10);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID002);
		return;
	}
	osal_ret = R_OSAL_IoRead32(handle, offset, &read_data);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (read_data == 0xFFFFFF10))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD32,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID002);
		return;
	}
}
/* ------------------------06_007_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_007_003()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint32_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD32,
														   OSAL_TEST_ID003);

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead32(NULL, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD32,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------06_007_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_007_004()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD32,
														   OSAL_TEST_ID004);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID004);
		return;
	}

	offset = 0x10000;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead32(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD32,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------06_007_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_007_005()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD32,
														   OSAL_TEST_ID005);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID005);
		return;
	}

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead32(handle, offset, NULL);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD32,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_007_006 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_007_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD32,
														   OSAL_TEST_ID006);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoRead32(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD32,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

/* ------------------------06_007_007 [OSAL_RETURN_STATE]------------------------ */
static void Test06_007_007()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD32,
														   OSAL_TEST_ID007);

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID007);
		return;
	}

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead32(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD32,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}

#if 0
/* ------------------------06_007_008 [OSAL_RETURN_OK]------------------------ */
void Test06_007_008_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint32_t read_data;
	uint32_t expect_read_data;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOREAD32,
															OSAL_TEST_ID008);

	offset = 0x00C0;
	read_data = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoRead32(g_io_cb_handle, offset, &read_data);
	TIME_MEASURE_STOP
	expect_read_data = 0x000000FF;

	if ((target_ret == expect_data) && (read_data == expect_read_data))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD32,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_007_008()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_007_008_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID008);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID008);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, OSAL_TEST_ID008);
		return;
	}
}
#endif

/* ------------------------06_008_001 [success]------------------------ */
static void Test06_008_001()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD64,
														   OSAL_TEST_ID001);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	osal_ret = R_OSAL_IoWrite64(handle, offset, 0x000000FF000000FF);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead64(handle, offset, &read_data);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (read_data == 0x000000FF000000FF))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD64,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_008_002 [success]------------------------ */
static void Test06_008_002()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD64,
														   OSAL_TEST_ID002);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	osal_ret = R_OSAL_IoWrite64(handle, offset, 0x000000FF000000FF);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID002);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead64(handle, offset, &read_data);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (read_data == 0x000000FF000000FF))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD64,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID002);
		return;
	}
}
/* ------------------------06_008_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_008_003()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD64,
														   OSAL_TEST_ID003);

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead64(NULL, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD64,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------06_008_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_008_004()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD64,
														   OSAL_TEST_ID004);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID004);
		return;
	}

	offset = 0x10000;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead64(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD64,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------06_008_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_008_005()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD64,
														   OSAL_TEST_ID005);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID005);
		return;
	}

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead64(handle, offset, NULL);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD64,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_008_006 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_008_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD64,
														   OSAL_TEST_ID006);

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoRead64(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD64,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

/* ------------------------06_008_007 [OSAL_RETURN_STATE]------------------------ */
static void Test06_008_007()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data = 0;

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOREAD64,
														   OSAL_TEST_ID007);

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID007);
		return;
	}

	offset = 0x00C0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoRead64(handle, offset, &read_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD64,
							OSAL_TEST_ID007,
							test_ret, opt_info);
}
#if 0
/* ------------------------06_008_008 [OSAL_RETURN_OK]------------------------ */
void Test06_008_008_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint64_t read_data;
	uint64_t expect_read_data;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOREAD64,
															OSAL_TEST_ID008);

	offset = 0x00C0;
	read_data = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoRead64(g_io_cb_handle, offset, &read_data);
	TIME_MEASURE_STOP
	expect_read_data = 0x000000FF000000FF;

	if ((target_ret == expect_data) && (read_data == expect_read_data))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOREAD64,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_008_008()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_008_008_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID008);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID008);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID008);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, OSAL_TEST_ID008);
		return;
	}
}
#endif

/* ------------------------06_013_001 [success]------------------------ */
static void Test06_013_001()
{
	uintptr_t 				offset;
	uint8_t            		write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE8,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	write_data = 0xAA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite8(handle, offset, write_data);
	TIME_MEASURE_STOP
	if(R_OSAL_IoRead8(handle, offset, &read_data) != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		if ((osal_ret == expect_data) && (read_data == 0xA8))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE8,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_013_002 [success]------------------------ */
static void Test06_013_002()
{
	uintptr_t 				offset;
	uint8_t            		write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE8,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	write_data = 0xBB;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite8(handle, offset, write_data);
	TIME_MEASURE_STOP
	if(R_OSAL_IoRead8(handle, offset, &read_data) != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID002);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		if ((osal_ret == expect_data) && (read_data == 0xB8))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE8,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_013_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_013_003()
{
	uintptr_t 				offset;
	uint8_t            		write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE8,
														   OSAL_TEST_ID003);

	offset = 0x00C0;
	write_data = 0xAA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite8(NULL, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE8,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------06_013_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_013_004()
{
	uintptr_t 				offset;
	uint8_t            		write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE8,
														   OSAL_TEST_ID004);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID004);
		return;
	}

	offset = 0x10000;
	write_data = 0xAA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite8(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE8,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------06_013_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_013_005()
{
	uintptr_t 				offset;
	uint8_t            		write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE8,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		write_data = 0xAA;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoWrite8(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE8,
							OSAL_TEST_ID005,
							test_ret, opt_info);
}

/* ------------------------06_013_006 [OSAL_RETURN_STATE]------------------------ */
static void Test06_013_006()
{
	uintptr_t 				offset;
	uint8_t            		write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE8,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	write_data = 0xAA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite8(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE8,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

#if 0
void Test06_013_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint8_t write_data;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE8,
														   OSAL_TEST_ID007);

	offset = 0x00C3;
	write_data = 0xAA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite8(g_io_cb_handle, offset, write_data);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	g_io_cb_flag = 1;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE8,
							OSAL_TEST_ID007,
							test_ret, opt_info);

}
/* ------------------------06_013_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_013_007()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	/* Init device */
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID007);
		return;
	}

	/* Init tmu Driver */
	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID007);
		return;
	}

	memset(&g_io_cb_handle, 0x00, sizeof(osal_device_handle_t));
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID007);
		return;
	}

	/* Start Interrupt Timer */
	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_013_007_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID007);
		return;
	}
	
	/****************************/
	/* Test execution			*/
	/****************************/
	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;


	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID007);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID007);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, OSAL_TEST_ID007);
		return;
	}
}
#endif

/* ------------------------06_014_001 [success]------------------------ */
static void Test06_014_001()
{
	uintptr_t 				offset;
	uint16_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE16,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	write_data = 0x00AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite16(handle, offset, write_data);
	TIME_MEASURE_STOP
	if(R_OSAL_IoRead16(handle, offset, &read_data) != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		if ((osal_ret == expect_data) && (read_data == 0x00A8))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE16,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_014_002 [success]------------------------ */
static void Test06_014_002()
{
	uintptr_t 				offset;
	uint16_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE16,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	write_data = 0x00CC;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite16(handle, offset, write_data);
	TIME_MEASURE_STOP
	if(R_OSAL_IoRead16(handle, offset, &read_data) != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID002);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		if ((osal_ret == expect_data) && (read_data == 0x00C8))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE16,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_014_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_014_003()
{
	uintptr_t 				offset;
	uint16_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE16,
														   OSAL_TEST_ID003);

	offset = 0x00C0;
	write_data = 0x00AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite16(NULL, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE16,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------06_014_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_014_004()
{
	uintptr_t 				offset;
	uint16_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE16,
														   OSAL_TEST_ID004);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID004);
		return;
	}

	offset = 0x10000;
	write_data = 0x00AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite16(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE16,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------06_014_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_014_005()
{
	uintptr_t 				offset;
	uint16_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE16,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		write_data = 0x00AA;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoWrite16(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE16,
							OSAL_TEST_ID005,
							test_ret, opt_info);
}

/* ------------------------06_014_006 [OSAL_RETURN_STATE]------------------------ */
static void Test06_014_006()
{
	uintptr_t 				offset;
	uint16_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE16,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	write_data = 0x00AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite16(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE16,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

#if 0
void Test06_014_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint8_t write_data;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE16,
														   OSAL_TEST_ID007);

	offset = 0x00C2;
	write_data = 0x00AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite16(g_io_cb_handle, offset, write_data);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	g_io_cb_flag = 1;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE16,
							OSAL_TEST_ID007,
							test_ret, opt_info);

}
/* ------------------------06_014_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_014_007()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	/* Init device */
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID007);
		return;
	}

	/* Init tmu Driver */
	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID007);
		return;
	}

	memset(&g_io_cb_handle, 0x00, sizeof(osal_device_handle_t));
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID007);
		return;
	}

	/* Start Interrupt Timer */
	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_014_007_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID007);
		return;
	}
	
	/****************************/
	/* Test execution			*/
	/****************************/
	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;


	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID007);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID007);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, OSAL_TEST_ID007);
		return;
	}
}
#endif

/* ------------------------06_015_001 [success]------------------------ */
static void Test06_015_001()
{
	uintptr_t 				offset;
	uint32_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE32,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	write_data = 0xFFFFFAA0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite32(handle, offset, write_data);
	TIME_MEASURE_STOP
	if(R_OSAL_IoRead32(handle, offset, &read_data) != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		if ((osal_ret == expect_data) && (read_data == write_data))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE32,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_015_002 [success]------------------------ */
static void Test06_015_002()
{
	uintptr_t 				offset;
	uint32_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE32,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	write_data = 0xFFFFFAA0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite32(handle, offset, write_data);
	TIME_MEASURE_STOP
	if(R_OSAL_IoRead32(handle, offset, &read_data) != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID002);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		if ((osal_ret == expect_data) && (read_data == write_data))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE32,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_015_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_015_003()
{
	uintptr_t 				offset;
	uint32_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE32,
														   OSAL_TEST_ID003);

	offset = 0x00C0;
	write_data = 0x000000AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite32(NULL, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE32,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------06_015_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_015_004()
{
	uintptr_t 				offset;
	uint32_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE32,
														   OSAL_TEST_ID004);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID004);
		return;
	}

	offset = 0x10000;
	write_data = 0x000000AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite32(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE32,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------06_015_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_015_005()
{
	uintptr_t 				offset;
	uint32_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE32,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		write_data = 0x000000AA;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoWrite32(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE32,
							OSAL_TEST_ID005,
							test_ret, opt_info);
}

/* ------------------------06_015_006 [OSAL_RETURN_STATE]------------------------ */
static void Test06_015_006()
{
	uintptr_t 				offset;
	uint32_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE32,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID006);
		return;
	}

	offset =  0x00C0;
	write_data = 0x000000AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite32(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE32,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

#if 0
void Test06_015_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint8_t write_data;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE32,
														   OSAL_TEST_ID007);

	offset =  0x00C0;
	write_data = 0x000000AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite32(g_io_cb_handle, offset, write_data);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	g_io_cb_flag = 1;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE32,
							OSAL_TEST_ID007,
							test_ret, opt_info);

}

/* ------------------------06_015_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_015_007()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	/* Init device */
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID007);
		return;
	}

	/* Init tmu Driver */
	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID007);
		return;
	}

	memset(&g_io_cb_handle, 0x00, sizeof(osal_device_handle_t));
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID007);
		return;
	}

	/* Start Interrupt Timer */
	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_015_007_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID007);
		return;
	}
	
	/****************************/
	/* Test execution			*/
	/****************************/
	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;


	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID007);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID007);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID007);
		return;
	}
}
#endif

/* ------------------------06_016_001 [success]------------------------ */
static void Test06_016_001()
{
	uintptr_t 				offset;
	uint64_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE64,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	write_data = 0x000000000000AAAA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite64(handle, offset, write_data);
	TIME_MEASURE_STOP
	if(R_OSAL_IoRead64(handle, offset, &read_data) != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID001);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		if ((osal_ret == expect_data) && (read_data == write_data))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE64,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_016_002 [success]------------------------ */
static void Test06_016_002()
{
	uintptr_t 				offset;
	uint64_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data = 0;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE64,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	write_data = 0x0000000000BBBB;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite64(handle, offset, write_data);
	TIME_MEASURE_STOP
	if(R_OSAL_IoRead64(handle, offset, &read_data) != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoWrite64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID002);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		if ((osal_ret == expect_data) && (read_data == write_data))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE64,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_016_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_016_003()
{
	uintptr_t 				offset;
	uint64_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE64,
														   OSAL_TEST_ID003);

	offset = 0x00C0;
	write_data = 0x000000AA000000AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite64(NULL, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE64,
							OSAL_TEST_ID003,
							test_ret, opt_info);
}

/* ------------------------06_016_004 [OSAL_RETURN_PAR]------------------------ */
static void Test06_016_004()
{
	uintptr_t 				offset;
	uint64_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE64,
														   OSAL_TEST_ID004);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID004);
		return;
	}

	offset = 0x10000;
	write_data = 0x000000AA000000AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite64(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE64,
							OSAL_TEST_ID004,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------06_016_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_016_005()
{
	uintptr_t 				offset;
	uint64_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE64,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID005);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		write_data = 0x000000AA000000AA;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoWrite64(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE64,
							OSAL_TEST_ID005,
							test_ret, opt_info);
}

/* ------------------------06_016_006 [OSAL_RETURN_STATE]------------------------ */
static void Test06_016_006()
{
	uintptr_t 				offset;
	uint64_t            	write_data;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE64,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID006);
		return;
	}

	offset =  0x00C0;
	write_data = 0x000000AA000000AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite64(handle, offset, write_data);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE64,
							OSAL_TEST_ID006,
							test_ret, opt_info);
}

#if 0
void Test06_016_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint64_t write_data;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOWRITE64,
														   OSAL_TEST_ID007);

	offset =  0x00C0;
	write_data = 0x000000AA000000AA;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoWrite64(g_io_cb_handle, offset, write_data);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	g_io_cb_flag = 1;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOWRITE64,
							OSAL_TEST_ID007,
							test_ret, opt_info);

}
/* ------------------------06_016_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_016_007()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	/* Init device */
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID007);
		return;
	}

	/* Init tmu Driver */
	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID007);
		return;
	}

	memset(&g_io_cb_handle, 0x00, sizeof(osal_device_handle_t));
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID007);
		return;
	}

	/* Start Interrupt Timer */
	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_016_007_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID007);
		return;
	}
	
	/****************************/
	/* Test execution			*/
	/****************************/
	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;


	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID007);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID007);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, OSAL_TEST_ID007);
		return;
	}
}
#endif

/* ------------------------06_009_001 [success]------------------------ */
static void Test06_009_001()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[16];
	uint8_t					register_data[16] = {0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 
													0x00, 0xFF, 0x00, 0x00, 0x00, 0x00};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	size = 16;
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_009_002 [success]------------------------ */
static void Test06_009_002()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[8];
	uint8_t					register_data[8] = {0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00, 0xD0};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	size = 8;
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID002);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_009_003 [success]------------------------ */
static void Test06_009_003()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[17];
	uint8_t					register_data[17] = {0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00, 
													0xCC, 0x00, 0x00, 0x00, 0x00, 0x00};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID003);
		return;
	}

	offset = 0x0030;
	size = 17;
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID003);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID003,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------06_009_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_009_004()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint8_t             	read_data[16];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID004);

	offset = 0x0000;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead8(NULL, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------06_009_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_009_005()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[16];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID005);
		return;
	}

	offset = 0x10000;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID005,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_009_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_009_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead8(handle, offset, NULL, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID006,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------06_009_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_009_007()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[16];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID007);
		return;
	}

	offset = 0x0030;
	size = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID007,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------06_009_008 [OSAL_RETURN_PAR]------------------------ */
static void Test06_009_008()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[16];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID008);
		return;
	}

	offset = 0x00C0;
	size = 65345;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------06_009_009 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_009_009()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[16];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID009);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 16;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

/* ------------------------06_009_010 [OSAL_RETURN_STATE]------------------------ */
static void Test06_009_010()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[16];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD8,
														   OSAL_TEST_ID010);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 16;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockRead8(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID010,
							test_ret, opt_info);						
}

#if 0
/* ------------------------06_009_011 [OSAL_RETURN_OK]------------------------ */
void Test06_009_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	int expect_ret = OSAL_TEST_OK;
	int i;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint8_t read_data[16];
	uint8_t expect_read_data[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
	};

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOBLOCKREAD8,
															OSAL_TEST_ID011);

	offset = 0x0034;
	memset(read_data, 0, sizeof(read_data));
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoBlockRead8(g_io_cb_handle, offset, read_data, 16);
	TIME_MEASURE_STOP
	for (i = 0; i < 16; i ++)
	{
		IT_DEBUG("R_OSAL_IoBlockRead8, p_data[%d] = 0x%02X\n", i, read_data[i]);
		if (read_data[i] != expect_read_data[i])
		{
			expect_ret = OSAL_TEST_NG;
		}
	}

	if ((target_ret == expect_data) && (expect_ret == OSAL_TEST_OK))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD8,
							OSAL_TEST_ID011,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_009_011()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_009_011_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID011);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID011);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* ------------------------06_010_001 [success]------------------------ */
static void Test06_010_001()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[8];
	uint16_t				register_data[8] = {0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0001};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	size = 16;
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
#if !defined (RCAR_V3U)
			    printf("count: %d read data: 0x%08lx, reg data: 0x%08lx\n",count, read_data[0], register_data[count]);
                printf("If target reg DSTR: V3U expect value is 0x0001, but other device expect value is 0x0000\n ");
                if (( count == 7) && ( read_data[count] == 0x0000))
                {
    				test_ret = OSAL_TEST_OK;
                }
                else
#endif
                {
    				test_ret = OSAL_TEST_NG;
	    			break;
                }
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_010_002 [success]------------------------ */
static void Test06_010_002()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[4];
	uint16_t				register_data[4] = {0x0000, 0x0040, 0x0000, 0x0100};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	size = 8;
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID002);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_data, size);
	TIME_MEASURE_STOP 
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_010_003 [success]------------------------ */
static void Test06_010_003()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[8];
	uint16_t				register_data[8] = {0x0000, 0x0030, 0x0000, 0x0100, 0x0000, 0xFFFF, 0x0000, 0x0001};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID003);
		return;
	}

	offset = 0x0030;
	size = 16;
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID003);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
#if !defined (RCAR_V3U)
			    printf("count: %d read data: 0x%08lx, reg data: 0x%08lx\n",count, read_data[0], register_data[count]);
                printf("If target reg DSTR: V3U expect value is 0x0001, but other device expect value is 0x0000\n ");
                if (( count == 7) && ( read_data[count] == 0x0000))
                {
    				test_ret = OSAL_TEST_OK;
                }
                else
#endif
                {
        			test_ret = OSAL_TEST_NG;
        			break;
                }
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------06_010_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_010_004()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint16_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID004);

	offset = 0x0000;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead16(NULL, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------06_010_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_010_005()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID005);
		return;
	}

	offset = 0x10000;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_010_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_010_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead16(handle, offset, NULL, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID006,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------06_010_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_010_007()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID007);
		return;
	}

	offset = 0x0030;
	size = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID007,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------06_010_008 [OSAL_RETURN_PAR]------------------------ */
static void Test06_010_008()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID008);
		return;
	}

	offset = 0x00C0;
	size = 65345;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID008,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------06_010_009 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_010_009()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID009);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 16;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

/* ------------------------06_010_010 [OSAL_RETURN_STATE]------------------------ */
static void Test06_010_010()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD16,
														   OSAL_TEST_ID010);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 16;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockRead16(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

#if 0
/* ------------------------06_010_011 [OSAL_RETURN_OK]------------------------ */
void Test06_010_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	int expect_ret = OSAL_TEST_OK;
	int i;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint16_t read_data[8];
	uint16_t expect_read_data[8] = {
		0x0000, 0x0000, 0x0000, 0x0000, 0x0040, 0x0000, 0x0100, 0x0000
	};

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOBLOCKREAD16,
															OSAL_TEST_ID011);

	offset = 0x0034;
	memset(read_data, 0, sizeof(read_data));
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoBlockRead16(g_io_cb_handle, offset, read_data, sizeof(read_data));
	TIME_MEASURE_STOP
	for (i = 0; i < 8; i ++)
	{
		IT_DEBUG("R_OSAL_IoBlockRead16, p_data[%d] = 0x%04X\n", i, read_data[i]);
		if (read_data[i] != expect_read_data[i])
		{
			expect_ret = OSAL_TEST_NG;
		}
	}

	if ((target_ret == expect_data) && (expect_ret == OSAL_TEST_OK))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD16,
							OSAL_TEST_ID011,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_010_011()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_010_011_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID011);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID011);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD16, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* ------------------------06_011_001 [success]------------------------ */
static void Test06_011_001()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[4];
	uint32_t				register_data[4] = {0x00000000, 0x00000000, 0x00000040, 0x00000100};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	size = 16;
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_011_002 [success]------------------------ */
static void Test06_011_002()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[2];
	uint32_t				register_data[2] = {0x00000040, 0x00000100};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	size = 8;
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID002);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_011_003 [success]------------------------ */
static void Test06_011_003()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[4];
	uint32_t				register_data[4] = {0x00000000, 0x00000000, 0x00000040, 0x00000100};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID003);
		return;
	}

	offset = 0x0030;
	size = 16;
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID003);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
				break;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------06_011_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_011_004()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint32_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID004);

	offset = 0x0000;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead32(NULL, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------06_011_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_011_005()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID005);
		return;
	}

	offset = 0x10000;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_011_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_011_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead32(handle, offset, NULL, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID006,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------06_011_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_011_007()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID007);
		return;
	}

	offset = 0x0030;
	size = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID007,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------06_011_008 [OSAL_RETURN_PAR]------------------------ */
static void Test06_011_008()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID008);
		return;
	}

	offset = 0x00C0;
	size = 65345;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID008,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------06_011_009 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_011_009()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID009);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 16;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

/* ------------------------06_011_010 [OSAL_RETURN_STATE]------------------------ */
static void Test06_011_010()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD32,
														   OSAL_TEST_ID010);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 16;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockRead32(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

#if 0
/* ------------------------06_011_011 [OSAL_RETURN_OK]------------------------ */
void Test06_011_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	int expect_ret = OSAL_TEST_OK;
	int i;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint32_t read_data[4];
	uint32_t expect_read_data[4] = {
		0x00000000, 0x00000000, 0x00000040, 0x00000100
	};

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOBLOCKREAD32,
															OSAL_TEST_ID011);

	offset = 0x0034;
	memset(read_data, 0, sizeof(read_data));
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoBlockRead32(g_io_cb_handle, offset, read_data, sizeof(read_data));
	TIME_MEASURE_STOP
	for (i = 0; i < 4; i ++)
	{
		IT_DEBUG("R_OSAL_IoBlockRead32, p_data[%d] = 0x%08X\n", i, read_data[i]);
		if (read_data[i] != expect_read_data[i])
		{
			expect_ret = OSAL_TEST_NG;
		}
	}

	if ((target_ret == expect_data) && (expect_ret == OSAL_TEST_OK))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD32,
							OSAL_TEST_ID011,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_011_011()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_011_011_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID011);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID011);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD32, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* ------------------------06_012_001 [success]------------------------ */
static void Test06_012_001()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[2];
	uint64_t				register_data[2] = {0x000000FF000000FF, 0x0000000000000000};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID001);
		return;
	}

	size = 16;
	offset = 0x0030;
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID001,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_012_002 [success]------------------------ */
static void Test06_012_002()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[1];
	uint64_t				register_data[1] = {0x000000FF000000FF};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	size = 8;
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID002,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_012_003 [success]------------------------ */
static void Test06_012_003()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[2];
	uint64_t				register_data[2] = {0x000000FF000000FF, 0x0000000000000000};
	size_t					size;
	size_t					count;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID003);
		return;
	}

	offset = 0x0030;
	size = 17;
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, register_data, size);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoBlockWrite64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD8, OSAL_TEST_ID001);
		return;
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		for (count = 0; count < size/sizeof(read_data[0]); count++)
		{
			if(read_data[count] == register_data[count])
			{
				test_ret = OSAL_TEST_OK;		
			}
			else
			{
				test_ret = OSAL_TEST_NG;
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------06_012_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_012_004()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uint64_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID004);

	offset = 0x0000;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead64(NULL, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------06_012_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_012_005()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID005);
		return;
	}

	offset = 0x10000;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_012_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_012_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	size = 16;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead64(handle, offset, NULL, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID006,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------06_012_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_012_007()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID007);
		return;
	}

	offset = 0x0030;
	size = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID007,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------06_012_008 [OSAL_RETURN_PAR]------------------------ */
static void Test06_012_008()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID008);
		return;
	}

	offset = 0x00C0;
	size = 65345;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID008,
							test_ret, opt_info);
	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------06_012_009 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_012_009()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID009);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 16;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

/* ------------------------06_012_010 [OSAL_RETURN_STATE]------------------------ */
static void Test06_012_010()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[8];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKREAD64,
														   OSAL_TEST_ID010);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 16;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockRead64(handle, offset, read_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

#if 0
/* ------------------------06_012_011 [OSAL_RETURN_OK]------------------------ */
void Test06_012_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	int expect_ret = OSAL_TEST_OK;
	int i;
	e_osal_return_t target_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t offset;
	uint64_t read_data[2];
	uint64_t expect_read_data[2] = {
		0x000000FF000000FF, 0x0000000000000000
	};

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
															OSAL_TEST_IOBLOCKREAD64,
															OSAL_TEST_ID011);

	offset = 0x00C0;
	memset(read_data, 0, sizeof(read_data));
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	target_ret = R_OSAL_IoBlockRead64(g_io_cb_handle, offset, read_data, sizeof(read_data));
	TIME_MEASURE_STOP
	for (i = 0; i < 2; i++)
	{
		IT_DEBUG("R_OSAL_IoBlockRead64, p_data[%d] = 0x%016lX\n", i, read_data[i]);
		if (read_data[i] != expect_read_data[i])
		{
			expect_ret = OSAL_TEST_NG;
		}
	}

	if ((target_ret == expect_data) && (expect_ret == OSAL_TEST_OK))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(target_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKREAD64,
							OSAL_TEST_ID011,
							test_ret, opt_info);

	g_io_cb_flag = 1;
}

static void Test06_012_011()
{
	e_osal_return_t osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_012_011_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID011);
		return;
	}

	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;

	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID011);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKREAD64, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* ------------------------06_017_001 [success]------------------------ */
static void Test06_017_001()
{
	uintptr_t 				offset;
	uint8_t            		write_data[8] = {0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0xB0};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[8];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead8(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_017_002 [success]------------------------ */
static void Test06_017_002()
{
	uintptr_t 				offset;
	uint8_t            		write_data[8] = {0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0xB0};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[8];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID002);
		return;
	}

	//offset = 0x00C1;
	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead8(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID002);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{	
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_017_003 [success]------------------------ */
static void Test06_017_003()
{
	uintptr_t 				offset;
	uint8_t            		write_data[9] = {0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0xB0, 0x00};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint8_t             	read_data[9];
	size_t						count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID003);
		return;
	}

	offset = 0x0030;
	size = 9;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead8(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID003);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------06_017_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_017_004()
{
	uintptr_t 				offset;
	uint8_t            		write_data[8] = {0xAA, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID004);

	offset = 0x0000;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(NULL, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------06_017_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_017_005()
{
	uintptr_t 				offset;
	uint8_t            		write_data[8] = {0xAA, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID005);
		return;
	}

	offset = 0x10000;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_017_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_017_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, NULL, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID006,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------06_017_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_017_007()
{
	uintptr_t 				offset;
	uint8_t            		write_data[8] = {0xA0, 0x00, 0x00, 0x00, 0xB0, 0x00, 0x00, 0x00};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID007);
		return;
	}

	offset = 0x0030;
	size = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID007,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------06_017_008 [OSAL_RETURN_PAR]------------------------ */
static void Test06_017_008()
{
	uintptr_t 				offset;
	uint8_t            		write_data[8] = {0xAA, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID008);
		return;
	}

	offset = 0x00C0;
	size = 65345;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------06_017_009 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_017_009()
{
	uintptr_t 				offset;
	uint8_t            		write_data[8] = {0xAA, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID009);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 8;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockWrite8(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

/* ------------------------06_017_010 [OSAL_RETURN_STATE]------------------------ */
static void Test06_017_010()
{
	uintptr_t 				offset;
	uint8_t            		write_data[8] = {0xAA, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID010);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 8;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockWrite8(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

#if 0
void Test06_017_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	uint8_t			write_data[8] = { 0xAA, 0x00, 0x00, 0x00, 0xAA, 0x00, 0x00, 0x00};
	uint8_t			read_data[8];
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t 	offset;
	size_t		count;
	size_t		size;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE8,
														   OSAL_TEST_ID011);
	
	memset(read_data, 0x00, sizeof(read_data));

	offset = 0x00C0;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite8(g_io_cb_handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead8(g_io_cb_handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead8() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID011);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{	
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	g_io_cb_flag = 1;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE8,
							OSAL_TEST_ID011,
							test_ret, opt_info);

}

/* ------------------------06_017_011 [OSAL_RETURN_OK]------------------------ */
static void Test06_017_011()
{
	e_osal_return_t       	osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	/* Init device */
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID011);
		return;
	}

	/* Init tmu Driver */
	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID011);
		return;
	}
	
	memset(&g_io_cb_handle, 0x00, sizeof(osal_device_handle_t));
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID011);
		return;
	}
	
	/* Start Interrupt Timer */
	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_017_011_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID011);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;


	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID011);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE8, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* ------------------------06_018_001 [success]------------------------ */
static void Test06_018_001()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x0000, 0x00A0, 0x0000, 0x00B0};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[4];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead16(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_018_002 [success]------------------------ */
static void Test06_018_002()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x0000, 0x00A0, 0x0000, 0x00B0};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[4];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead16(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID002);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_018_003 [success]------------------------ */
static void Test06_018_003()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x0000, 0x00A0, 0x0000, 0x00B0};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint16_t             	read_data[4];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID003);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead16(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID003);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------06_018_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_018_004()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x00AA, 0x0000, 0x00AA, 0x000};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID004);

	offset = 0x0000;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(NULL, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------06_018_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_018_005()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x00AA, 0x0000, 0x00AA, 0x000};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID005);
		return;
	}

	offset = 0x10000;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_018_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_018_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, NULL, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID006,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------06_018_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_018_007()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x00A0, 0x0000, 0x00B0, 0x000};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID007);
		return;
	}

	offset = 0x0030;
	size = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID007,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------06_018_008 [OSAL_RETURN_PAR]------------------------ */
static void Test06_018_008()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x00AA, 0x0000, 0x00AA, 0x000};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID008);
		return;
	}

	offset = 0x00C0;
	size = 65345;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------06_018_009 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_018_009()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x00AA, 0x0000, 0x00AA, 0x000};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID009);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 8;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockWrite16(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

/* ------------------------06_018_010 [OSAL_RETURN_STATE]------------------------ */
static void Test06_018_010()
{
	uintptr_t 				offset;
	uint16_t            	write_data[4] = {0x00AA, 0x0000, 0x00AA, 0x000};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID010);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 8;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockWrite16(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

#if 0
void Test06_018_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	uint16_t        write_data[4] =  {0x00AA, 0x0000, 0x00AA, 0x000};
	uint16_t        read_data[4];
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t 	offset;
	size_t		count;
	size_t		size;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE16,
														   OSAL_TEST_ID011);
	
	memset(read_data, 0x00, sizeof(read_data));

	offset = 0x00C0;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite16(g_io_cb_handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead16(g_io_cb_handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead16() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID011);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	g_io_cb_flag = 1;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE16,
							OSAL_TEST_ID011,
							test_ret, opt_info);

}

/* ------------------------06_018_011 [OSAL_RETURN_OK]------------------------ */
static void Test06_018_011()
{
	e_osal_return_t       	osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	/* Init device */
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID011);
		return;
	}

	/* Init tmu Driver */
	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID011);
		return;
	}
	
	memset(&g_io_cb_handle, 0x00, sizeof(osal_device_handle_t));
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID011);
		return;
	}
	
	/* Start Interrupt Timer */
	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_018_011_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID011);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;


	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID011);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE16, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* ------------------------06_019_001 [success]------------------------ */
static void Test06_019_001()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x00000040, 0x00000100};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[2];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead32(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_019_002 [success]------------------------ */
static void Test06_019_002()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x00000040, 0x00000100};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[2];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead32(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID002);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_019_003 [success]------------------------ */
static void Test06_019_003()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x00000040, 0x00000100};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint32_t             	read_data[2];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID003);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead32(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID003);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
					break;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------06_019_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_019_004()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x000000AA, 0x000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID004);

	offset = 0x0000;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(NULL, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------06_019_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_019_005()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x000000AA, 0x000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID005);
		return;
	}

	offset = 0x10000;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_019_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_019_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, NULL, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID006,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------06_019_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_019_007()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x000000A0, 0x000000B0};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID007);
		return;
	}

	offset = 0x0030;
	size = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID007,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------06_019_008 [OSAL_RETURN_PAR]------------------------ */
static void Test06_019_008()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x000000AA, 0x000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID008);
		return;
	}

	offset = 0x00C0;
	size = 65345;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------06_019_009 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_019_009()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x000000AA, 0x000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID009);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 8;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockWrite32(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

/* ------------------------06_019_010 [OSAL_RETURN_STATE]------------------------ */
static void Test06_019_010()
{
	uintptr_t 				offset;
	uint32_t            	write_data[2] = {0x000000AA, 0x000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID010);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 8;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockWrite32(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

#if 0
void Test06_019_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	uint32_t            	write_data[2] = {0x000000AA, 0x000000AA};
	uint32_t             	read_data[2];
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t 	offset;
	size_t		count;
	size_t		size;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE32,
														   OSAL_TEST_ID011);
	
	memset(read_data, 0x00, sizeof(read_data));

	offset = 0x00C0;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite32(g_io_cb_handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead32(g_io_cb_handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead32() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	g_io_cb_flag = 1;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE32,
							OSAL_TEST_ID011,
							test_ret, opt_info);

}

/* ------------------------06_019_011 [OSAL_RETURN_OK]------------------------ */
static void Test06_019_011()
{
	e_osal_return_t       	osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	/* Init device */
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID011);
		return;
	}

	/* Init tmu Driver */
	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID011);
		return;
	}
	
	memset(&g_io_cb_handle, 0x00, sizeof(osal_device_handle_t));
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID011);
		return;
	}
	
	/* Start Interrupt Timer */
	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_019_011_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID011);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;


	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID011);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE32, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* ------------------------06_020_001 [success]------------------------ */
static void Test06_020_001()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[1];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID001);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead64(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID001,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------06_020_002 [success]------------------------ */
static void Test06_020_002()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[1];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID002);
		return;
	}

	offset = 0x0030;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead64(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID002);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID002,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------06_020_003 [success]------------------------ */
static void Test06_020_003()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	uint64_t             	read_data[1];
	size_t					count;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));
	memset(read_data, 0x00, sizeof(read_data));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID003);
		return;
	}

	offset = 0x0030;
	size = 9;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead64(handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID003);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID003,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------06_020_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_020_004()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID004);

	offset = 0x0000;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(NULL, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID004,
							test_ret, opt_info);
}

/* ------------------------06_020_005 [OSAL_RETURN_PAR]------------------------ */
static void Test06_020_005()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID005);
		return;
	}

	offset = 0x10000;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID005,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------06_020_006 [OSAL_RETURN_PAR]------------------------ */
static void Test06_020_006()
{
	uintptr_t 				offset;
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID006);
		return;
	}

	offset = 0x00C0;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, NULL, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID006,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------06_020_007 [OSAL_RETURN_OK]------------------------ */
static void Test06_020_007()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID007);
		return;
	}

	offset = 0x0030;
	size = 0;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID007,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------06_020_008 [OSAL_RETURN_PAR]------------------------ */
static void Test06_020_008()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID008);
		return;
	}

	offset = 0x00C0;
	size = 65345;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID008,
							test_ret, opt_info);

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------06_020_009 [OSAL_RETURN_HANDLE]------------------------ */
static void Test06_020_009()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_IoDeviceClose(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID009);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 8;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockWrite64(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID009,
							test_ret, opt_info);
}

/* ------------------------06_020_010 [OSAL_RETURN_STATE]------------------------ */
static void Test06_020_010()
{
	uintptr_t 				offset;
	uint64_t            	write_data[1] = {0x000000AA000000AA};
	int        				test_ret;
	e_osal_return_t       	osal_ret;
	e_osal_return_t       	expect_data;
	char                	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	osal_device_handle_t 	handle;
	size_t					size;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID010);

	memset(&handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID010);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		offset = 0x00C0;
		size = 8;
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_IoBlockWrite64(handle, offset, write_data, size);
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
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID010,
							test_ret, opt_info);
}

#if 0
void Test06_020_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	int test_ret;
	uint64_t		write_data[1] = {0x000000AA000000AA};
	uint64_t        read_data[1];
	e_osal_return_t osal_ret;
	e_osal_return_t expect_data;
	char opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	uintptr_t 	offset;
	size_t		count;
	size_t		size;

	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_IO, 
														   OSAL_TEST_IOBLOCKWRITE64,
														   OSAL_TEST_ID011);
	
	memset(read_data, 0x00, sizeof(read_data));

	offset = 0x00C0;
	size = 8;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_IoBlockWrite64(g_io_cb_handle, offset, write_data, size);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		if(R_OSAL_IoBlockRead64(g_io_cb_handle, offset, read_data, sizeof(read_data)) != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_IoBlockRead64() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID001);
			test_ret = OSAL_TEST_NG;
		}
		else
		{
			for (count = 0; count < size/sizeof(write_data[0]); count++)
			{
				if(write_data[count] == read_data[count])
				{
					test_ret = OSAL_TEST_OK;
				}
				else
				{
					test_ret = OSAL_TEST_NG;
				}	
			}
			
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	g_io_cb_flag = 1;

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));
	osal_test_result_output(OSAL_TEST_IO,
							OSAL_TEST_IOBLOCKWRITE64,
							OSAL_TEST_ID011,
							test_ret, opt_info);

}

/* ------------------------06_020_011 [OSAL_RETURN_OK]------------------------ */
static void Test06_020_011()
{
	e_osal_return_t       	osal_ret;
	UINT16 tmu_ret;
	UINT8 Channel;
	drvTMU_Time_t Period;

	Channel = 0;
	Period.Sec = 1;
	Period.Usec = 0;

	/* Init device */
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID011);
		return;
	}

	/* Init tmu Driver */
	tmu_ret = drvTMU_Initialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID011);
		return;
	}
	
	memset(&g_io_cb_handle, 0x00, sizeof(osal_device_handle_t));
	/* Open device */
	osal_ret = R_OSAL_IoDeviceOpen("ims_00", &g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceOpen() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID011);
		return;
	}
	
	/* Start Interrupt Timer */
	tmu_ret = drvTMU_Start(Channel, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test06_020_011_CallbackFunc, TMU_EDGE_RISE);
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID011);
		return;
	}

	/****************************/
	/* Test execution			*/
	/****************************/
	while (!g_io_cb_flag)
	{
		;
	}
	g_io_cb_flag = 0;


	osal_ret = R_OSAL_IoDeviceClose(g_io_cb_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoDeviceClose() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID011);
		return;
	}

	tmu_ret = drvTMU_Deinitialize();
	if (tmu_ret != drv_OK)
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID011);
		return;
	}

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOBLOCKWRITE64, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* 06_001: R_OSAL_IoGetNumOfDevices */
static void osal_test_R_OSAL_IoGetNumOfDevices()
{
	e_osal_return_t osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETNUMOFDEVICES, 0);
		return;
	}

	Test06_001_001();
	Test06_001_002();
	Test06_001_003();
	Test06_001_004();
	//Test06_001_028();
	//Test06_001_029();

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETNUMOFDEVICES, 0);
		return;
	}
}

/* 06_002: R_OSAL_IoGetDeviceList */
static void osal_test_R_OSAL_IoGetDeviceList()
{
	e_osal_return_t   osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETDEVICELIST, 0);
		return;
	}

	Test06_002_001();
	Test06_002_002();
	Test06_002_003();
	Test06_002_004();
	Test06_002_005();
	Test06_002_006();
	Test06_002_007();
	Test06_002_008();
	Test06_002_009();
	//Test06_002_033();

	osal_ret = osal_deinit_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOGETDEVICELIST, 0);
		return;
	}
}

/* 06_003: R_OSAL_IoDeviceOpen */
static void osal_test_R_OSAL_IoDeviceOpen()
{
	e_osal_return_t       osal_ret;

	Test06_003_007();
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICEOPEN, 0);
		return;
	}

	Test06_003_001();
	Test06_003_002();
	Test06_003_003();
	Test06_003_004();
	Test06_003_005();
	Test06_003_006();
	Test06_003_008();
	//Test06_003_070();
	//Test06_003_071();
}

/* 06_004: R_OSAL_IoDeviceClose */
static void osal_test_R_OSAL_IoDeviceClose()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_IoInitialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}
	Test06_004_001();
	Test06_004_002();
	Test06_004_003();
	Test06_004_004();
	//Test06_004_066();
	//Test06_004_067();
	//Test06_004_068();
}

/* 06_005: R_OSAL_IoRead8 */
static void osal_test_R_OSAL_IoRead8()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD8, 0);
		return;
	}

//	Test06_005_001();
//	Test06_005_002();
	Test06_005_003();
	Test06_005_004();
	Test06_005_005();
	Test06_005_006();
	Test06_005_007();
	//Test06_005_008();
}

/* 06_006: R_OSAL_IoRead16 */
static void osal_test_R_OSAL_IoRead16()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD16, 0);
		return;
	}

//	Test06_006_001();
//	Test06_006_002();
	Test06_006_003();
	Test06_006_004();
	Test06_006_005();
	Test06_006_006();
	Test06_006_007();
	//Test06_006_008();
}

/* 06_007: R_OSAL_IoRead32 */
static void osal_test_R_OSAL_IoRead32()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD32, 0);
		return;
	}

//	Test06_007_001();
//	Test06_007_002();
	Test06_007_003();
	Test06_007_004();
	Test06_007_005();
	Test06_007_006();
	Test06_007_007();
	//Test06_007_008();
}

/* 06_008: R_OSAL_IoRead64 */
static void osal_test_R_OSAL_IoRead64()
{
	/* Test functions are not excuted because QNX does not support R_OSAL_IoRead64 */
	return;

	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOREAD64, 0);
		return;
	}

	Test06_008_001();
	Test06_008_002();
	Test06_008_003();
	Test06_008_004();
	Test06_008_005();
	Test06_008_006();
	Test06_008_007();
	//Test06_008_008();
}

/* 06_009: R_OSAL_IoBlockRead8 */
static void osal_test_R_OSAL_IoBlockRead8()
{	
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}

//	Test06_009_001();
//	Test06_009_002();
//	Test06_009_003();
	Test06_009_004();
	Test06_009_005();
	Test06_009_006();
	Test06_009_007();
	Test06_009_008();
	Test06_009_009();
	Test06_009_010();
	//Test06_009_011();
}

/* 06_010: R_OSAL_IoBlockRead16 */
static void osal_test_R_OSAL_IoBlockRead16()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}

//	Test06_010_001();
//	Test06_010_002();
//	Test06_010_003();
	Test06_010_004();
	Test06_010_005();
	Test06_010_006();
	Test06_010_007();
	Test06_010_008();
	Test06_010_009();
	Test06_010_010();
	//Test06_010_011();
}

/* 06_011: R_OSAL_IoBlockRead32 */
static void osal_test_R_OSAL_IoBlockRead32()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}

//	Test06_011_001();
//	Test06_011_002();
//	Test06_011_003();
	Test06_011_004();
	Test06_011_005();
	Test06_011_006();
	Test06_011_007();
	Test06_011_008();
	Test06_011_009();
	Test06_011_010();
	//Test06_011_011();
}

/* 06_012: R_OSAL_IoBlockRead64 */
static void osal_test_R_OSAL_IoBlockRead64()
{
	/* Test functions are not excuted because QNX does not support R_OSAL_IoBlockRead64 */
	return;

	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}

	Test06_012_001();
	Test06_012_002();
	Test06_012_003();
	Test06_012_004();
	Test06_012_005();
	Test06_012_006();
	Test06_012_007();
	Test06_012_008();
	Test06_012_009();
	Test06_012_010();
	//Test06_012_011();
}

/* 06_013: R_OSAL_IoWrite8 */
static void osal_test_R_OSAL_IoWrite8()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE8, 0);
		return;
	}

//	Test06_013_001();
//	Test06_013_002();
	Test06_013_003();
	Test06_013_004();
	Test06_013_005();
	Test06_013_006();
	//Test06_013_007();
}

/* 06_014: R_OSAL_IoWrite16 */
static void osal_test_R_OSAL_IoWrite16()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE16, 0);
		return;
	}

//	Test06_014_001();
//	Test06_014_002();
	Test06_014_003();
	Test06_014_004();
	Test06_014_005();
	Test06_014_006();
	//Test06_014_007();
}

/* 06_015: R_OSAL_IoWrite32 */
static void osal_test_R_OSAL_IoWrite32()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE32, 0);
		return;
	}

//	Test06_015_001();
//	Test06_015_002();
	Test06_015_003();
	Test06_015_004();
	Test06_015_005();
	Test06_015_006();
	//Test06_015_007();
}

/* 06_016: R_OSAL_IoWrite64 */
static void osal_test_R_OSAL_IoWrite64()
{
	/* Test functions are not excuted because QNX does not support R_OSAL_IoWrite64 */
	return;

	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IOWRITE64, 0);
		return;
	}
	
	Test06_016_001();
	Test06_016_002();
	Test06_016_003();
	Test06_016_004();
	Test06_016_005();
	Test06_016_006();
	//Test06_016_007();
}

/* 06_017: R_OSAL_IoBlockWrite8 */
static void osal_test_R_OSAL_IoBlockWrite8()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}

//	Test06_017_001();
//	Test06_017_002();
//	Test06_017_003();
	Test06_017_004();
	Test06_017_005();
	Test06_017_006();
	Test06_017_007();
	Test06_017_008();
	Test06_017_009();
	Test06_017_010();
	//Test06_017_011();
}

/* 06_018: R_OSAL_IoBlockWrite16 */
static void osal_test_R_OSAL_IoBlockWrite16()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}

//	Test06_018_001();
//	Test06_018_002();
//	Test06_018_003();
	Test06_018_004();
	Test06_018_005();
	Test06_018_006();
	Test06_018_007();
	Test06_018_008();
	Test06_018_009();
	Test06_018_010();
	//Test06_018_011();
}

/* 06_019: R_OSAL_IoBlockWrite32 */
static void osal_test_R_OSAL_IoBlockWrite32()
{
	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}

//	Test06_019_001();
//	Test06_019_002();
//	Test06_019_003();
	Test06_019_004();
	Test06_019_005();
	Test06_019_006();
	Test06_019_007();
	Test06_019_008();
	Test06_019_009();
	Test06_019_010();
	//Test06_019_011();
}

/* 06_020: R_OSAL_IoBlockWrite64 */
static void osal_test_R_OSAL_IoBlockWrite64()
{
	/* Test functions are not excuted because QNX does not support R_OSAL_IoBlockWrite64 */
	return;

	e_osal_return_t       osal_ret;
	osal_ret = osal_init_io();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
				OSAL_TEST_IO, OSAL_TEST_IODEVICECLOSE, 0);
		return;
	}

	Test06_020_001();
	Test06_020_002();
	Test06_020_003();
	Test06_020_004();
	Test06_020_005();
	Test06_020_006();
	Test06_020_007();
	Test06_020_008();
	Test06_020_009();
	Test06_020_010();
	//Test06_020_011();
}
