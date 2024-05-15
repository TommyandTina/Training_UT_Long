/******************************************************************************
     IMP Sample Code (How to use IMP Framework with IMP Driver)
     Copyright (c) 2017-2019 Renesas Electronics Corporation.

    [File] impDemo.c
******************************************************************************/ 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* OSAL API*/
#include "rcar-xos/osal/r_osal.h"
//#include "rcar-xos/osal_usr_cfg/r_osal_usr_cfg_id.h"
//#include "r_osal_usr_cfg_id.h"

/* IMP Framework */
#include "rcar-xos/impfw/r_impfw.h"

/* Atomic Library */
#include "rcar-xos/atmlib/r_atmlib_types.h"
#include "rcar-xos/atmlib/r_atmlib_prot.h"

/* Demo */
#include "impDemo.h"
#include "impDemo_data.h"
//#include "rcv_memdrv.h"

/* define */
//#define CLQUE_NUM (10 * R_IMPFW_MODULE_NUM)
#define R_IMPDEMO_CORE_TYPE_MAX	5

#define PRINT_ADDRESS_LOG   0

/* typedef */
typedef struct {
	char				*coaname;
	e_impfw_core_type_t	core_type;
} R_IMPDEMO_CORE;

/* prototype */
static void impDemo_SetCoreType(char** argv, e_impfw_core_type_t *core_type);
static int impDemo_SetTestCase(char* argv, int32_t *test_case);
static int impDemo_main(int32_t test_case, e_impfw_core_type_t core_type, uint32_t core_num);
static void CallBackFunc(e_impfw_callback_reason_t Status, const st_impfw_core_info_t * const core, int32_t code, void const * CallBackArgs);
static void CL_Setting(uint32_t *CL_data, uint32_t CL_count, int32_t test_case, e_impfw_core_type_t core_type, uint32_t ImgA_phys, uint32_t ImgOut_phys);
static void CallBackFuncFatal(e_impfw_fatalcode_t  reason, uint32_t value);

/* global */
//static R_IMPFW_CL		test_clque[CLQUE_NUM];
static volatile int complete_flg;
static int additional_data;

static osal_memory_manager_handle_t   osal_mmngr         = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
static osal_axi_bus_id_t              imp_dev_axi_bus_id = OSAL_AXI_BUS_ID_INVALID;

static int32_t mmngr_init(void)
{
	int32_t	ercd = 0;
    e_osal_return_t ret_osal;
	st_osal_mmngr_config_t         osal_mmngr_cfg;
	extern osal_axi_bus_id_t ReturnAxiID(void);

    ret_osal = R_OSAL_MmngrGetOsalMaxConfig(&osal_mmngr_cfg);
    if (ret_osal != OSAL_RETURN_OK)
    {
        ercd = -1;
    }
	else
	{
		osal_mmngr_cfg.memory_limit                = 0x2000000; /* 32MB */
		osal_mmngr_cfg.max_allowed_allocations     = 256u;
		osal_mmngr_cfg.max_registered_monitors_cbs = 0u;

		ret_osal = R_OSAL_MmngrOpen(&osal_mmngr_cfg, &osal_mmngr);
		if (ret_osal != OSAL_RETURN_OK)
		{
			ercd = -1;
		}
		else
		{
			imp_dev_axi_bus_id = ReturnAxiID();
		}
	}

	return ercd;
}

static int32_t mmngr_free(osal_memory_buffer_handle_t buffer)
{
	int32_t	ercd = 0;
    e_osal_return_t ret_osal;

    ret_osal = R_OSAL_MmngrDealloc(osal_mmngr, buffer);
	if (ret_osal != OSAL_RETURN_OK)
	{
		ercd = -1;
	}

	return ercd;
}
static int32_t mmngr_alloc(uint32_t size, osal_memory_buffer_handle_t *buffer, void **virt, uintptr_t *phys)
{
	int32_t	ercd = 0;
    e_osal_return_t ret_osal;

    ret_osal = R_OSAL_MmngrAlloc(osal_mmngr, size, 128u, buffer);
	if (ret_osal != OSAL_RETURN_OK)
	{
		ercd = -1;
	}
	else
	{
		if (virt)
		{
		   ret_osal = R_OSAL_MmngrGetCpuPtr(*buffer, virt);
			if (ret_osal != OSAL_RETURN_OK)
			{
				ercd = -1;
			}
		}
		if (phys)
		{
			ret_osal = R_OSAL_MmngrGetHwAddr(*buffer, imp_dev_axi_bus_id, phys);
			if (ret_osal != OSAL_RETURN_OK)
			{
				ercd = -1;
			}
		}
	}

	return ercd;
}

static int32_t mmngr_pargecache(osal_memory_buffer_handle_t buffer)
{
	int32_t	ercd = 0;
    e_osal_return_t ret_osal;
	size_t  size;

	ret_osal = R_OSAL_MmngrGetSize(buffer, &size);
	if (ret_osal != OSAL_RETURN_OK)
	{
		ercd = -1;
	}
	else
	{
		ret_osal = R_OSAL_MmngrFlush(buffer, 0, size);
		if (ret_osal != OSAL_RETURN_OK)
		{
			ercd = -1;
		}
		else
		{
			ret_osal = R_OSAL_MmngrInvalidate(buffer, 0, size);
			if (ret_osal != OSAL_RETURN_OK)
			{
				ercd = -1;
			}
		}
	}
	return ercd;
}

static int32_t impfw_init(impfw_ctrl_handle_t *impfwctl)
{
	#define IMPFW_WORKSIZE  65536
	static uint64_t  work_impfwinit[IMPFW_WORKAREA_TYPE_NUM][IMPFW_WORKSIZE/sizeof(uint64_t)];
//	static impfw_ctrl_handle_t   impfwctl;
	st_impfw_initdata_t      impfwinit;
#if defined TEST_RCAR_V3U
	e_impfw_instance_t       impfw_useinstance = IMPFW_INSTANCE_6;
	st_impfw_core_info_t     impfw_usecore[] = {
		{ IMPFW_CORE_TYPE_IMP, 0u }, /* 0 */
		{ IMPFW_CORE_TYPE_IMP, 1u }, /* 1 */
		{ IMPFW_CORE_TYPE_IMP, 2u }, /* 2 */
		{ IMPFW_CORE_TYPE_IMP, 3u }, /* 3 */
		{ IMPFW_CORE_TYPE_OCV, 0u }, /* 4 */
		{ IMPFW_CORE_TYPE_OCV, 1u }, /* 5 */
		{ IMPFW_CORE_TYPE_OCV, 2u }, /* 6 */
		{ IMPFW_CORE_TYPE_OCV, 3u }, /* 7 */
		{ IMPFW_CORE_TYPE_OCV, 4u }, /* 8 */
		{ IMPFW_CORE_TYPE_OCV, 5u }, /* 9 */
		{ IMPFW_CORE_TYPE_OCV, 6u }, /* 10 */
		{ IMPFW_CORE_TYPE_OCV, 7u }, /* 11 */
		{ IMPFW_CORE_TYPE_DMAC, 0u }, /* 12 */
		{ IMPFW_CORE_TYPE_DMAC, 1u }, /* 13 */
		{ IMPFW_CORE_TYPE_DMAC, 2u }, /* 14 */
		{ IMPFW_CORE_TYPE_DMAC, 3u }, /* 15 */
		{ IMPFW_CORE_TYPE_PSCEXE, 0u }, /* 16 */
		{ IMPFW_CORE_TYPE_PSCEXE, 1u }, /* 17 */
		{ IMPFW_CORE_TYPE_CNN, 0u }, /* 18 */
		{ IMPFW_CORE_TYPE_CNN, 1u }, /* 19 */
		{ IMPFW_CORE_TYPE_CNN, 2u }, /* 20 */
	};
#elif defined TEST_RCAR_V3H2
	e_impfw_instance_t       impfw_useinstance = IMPFW_INSTANCE_0;
	st_impfw_core_info_t     impfw_usecore[] = {
		{ IMPFW_CORE_TYPE_IMP, 0u }, /* 0 */
		{ IMPFW_CORE_TYPE_IMP, 1u }, /* 1 */
		{ IMPFW_CORE_TYPE_IMP, 2u }, /* 2 */
		{ IMPFW_CORE_TYPE_IMP, 3u }, /* 3 */
		{ IMPFW_CORE_TYPE_IMP, 4u }, /* 4 */
//		{ IMPFW_CORE_TYPE_SIMP, 0u }, /* 5 */
		{ IMPFW_CORE_TYPE_OCV, 0u }, /* 6 */
		{ IMPFW_CORE_TYPE_OCV, 1u }, /* 7 */
		{ IMPFW_CORE_TYPE_OCV, 2u }, /* 8 */
		{ IMPFW_CORE_TYPE_OCV, 3u }, /* 9 */
		{ IMPFW_CORE_TYPE_OCV, 4u }, /* 10 */
		{ IMPFW_CORE_TYPE_DMAC, 0u }, /* 11 */
		{ IMPFW_CORE_TYPE_DMAC, 1u }, /* 12 */
		{ IMPFW_CORE_TYPE_DMAC, 2u }, /* 13 */
		{ IMPFW_CORE_TYPE_DMAC, 3u }, /* 14 */
//		{ IMPFW_CORE_TYPE_SDMAC, 0u }, /* 15 */
//		{ IMPFW_CORE_TYPE_SDMAC, 1u }, /* 16 */
		{ IMPFW_CORE_TYPE_PSCEXE, 0u }, /* 17 */
		{ IMPFW_CORE_TYPE_CNN, 0u }, /* 18 */
	};
#elif defined TEST_RCAR_V3H1
	e_impfw_instance_t       impfw_useinstance = IMPFW_INSTANCE_0;
	st_impfw_core_info_t     impfw_usecore[] = {
		{ IMPFW_CORE_TYPE_IMP, 0u }, /* 0 */
		{ IMPFW_CORE_TYPE_IMP, 1u }, /* 1 */
		{ IMPFW_CORE_TYPE_IMP, 2u }, /* 2 */
		{ IMPFW_CORE_TYPE_IMP, 3u }, /* 3 */
		{ IMPFW_CORE_TYPE_IMP, 4u }, /* 4 */
//		{ IMPFW_CORE_TYPE_SIMP, 0u }, /* 5 */
		{ IMPFW_CORE_TYPE_OCV, 0u }, /* 6 */
		{ IMPFW_CORE_TYPE_OCV, 1u }, /* 7 */
		{ IMPFW_CORE_TYPE_OCV, 2u }, /* 8 */
		{ IMPFW_CORE_TYPE_OCV, 3u }, /* 9 */
		{ IMPFW_CORE_TYPE_OCV, 4u }, /* 10 */
		{ IMPFW_CORE_TYPE_DMAC, 0u }, /* 11 */
		{ IMPFW_CORE_TYPE_DMAC, 1u }, /* 12 */
		{ IMPFW_CORE_TYPE_DMAC, 2u }, /* 13 */
		{ IMPFW_CORE_TYPE_DMAC, 3u }, /* 14 */
		{ IMPFW_CORE_TYPE_PSCEXE, 0u }, /* 15 */
		{ IMPFW_CORE_TYPE_PSCEXE, 1u }, /* 16 */
		{ IMPFW_CORE_TYPE_CNN, 0u }, /* 17 */
	};
#elif defined TEST_RCAR_V3M2
	e_impfw_instance_t       impfw_useinstance = IMPFW_INSTANCE_0;
	st_impfw_core_info_t     impfw_usecore[] = {
		{ IMPFW_CORE_TYPE_IMP, 0u }, /* 0 */
		{ IMPFW_CORE_TYPE_IMP, 1u }, /* 1 */
		{ IMPFW_CORE_TYPE_IMP, 2u }, /* 2 */
		{ IMPFW_CORE_TYPE_IMP, 3u }, /* 3 */
		{ IMPFW_CORE_TYPE_OCV, 0u }, /* 4 */
		{ IMPFW_CORE_TYPE_OCV, 1u }, /* 5 */
		{ IMPFW_CORE_TYPE_DMAC, 0u }, /* 6 */
		{ IMPFW_CORE_TYPE_DMAC, 1u }, /* 7 */
		{ IMPFW_CORE_TYPE_PSCEXE, 0u }, /* 8 */
		{ IMPFW_CORE_TYPE_CNN, 0u }, /* 9 */
	};
#elif defined TEST_RCAR_V4H
	e_impfw_instance_t       impfw_useinstance = IMPFW_INSTANCE_0;
	st_impfw_core_info_t     impfw_usecore[] = {
		{ IMPFW_CORE_TYPE_IMP, 0u }, /* 0 */
		{ IMPFW_CORE_TYPE_IMP, 1u }, /* 1 */
		{ IMPFW_CORE_TYPE_IMP, 2u }, /* 2 */
		{ IMPFW_CORE_TYPE_IMP, 3u }, /* 3 */
		{ IMPFW_CORE_TYPE_PSCEXE, 0u }, /* 4 */
		{ IMPFW_CORE_TYPE_DMAC, 0u }, /* 5 */
		{ IMPFW_CORE_TYPE_DMAC, 1u }, /* 6 */
		{ IMPFW_CORE_TYPE_DMAC, 2u }, /* 7 */
		{ IMPFW_CORE_TYPE_DMAC, 3u }, /* 8 */
		{ IMPFW_CORE_TYPE_OCV, 0u }, /* 9 */
		{ IMPFW_CORE_TYPE_OCV, 1u }, /* 10 */
		{ IMPFW_CORE_TYPE_OCV, 2u }, /* 11 */
		{ IMPFW_CORE_TYPE_OCV, 3u }, /* 12 */
		{ IMPFW_CORE_TYPE_CNN, 0u }, /* 13 */
//		{ IMPFW_CORE_TYPE_DMAC_SLIM, 0u }, /* 14 */
//		{ IMPFW_CORE_TYPE_DMAC_SLIM, 1u }, /* 15 */
//		{ IMPFW_CORE_TYPE_DMAC_SLIM, 2u }, /* 16 */
//		{ IMPFW_CORE_TYPE_DMAC_SLIM, 3u }, /* 17 */
	};
#endif
    e_impfw_api_retcode_t      impfw_ret;

    /*----- IMP Framework Initialization -----*/
	memset(&impfwinit, 0, sizeof(impfwinit));

    for (uint32_t i = 0U; IMPFW_WORKAREA_TYPE_NUM > i; i++)
    {
        impfwinit.work_area_info[i].p_work_addr = &work_impfwinit[i];
        impfwinit.work_area_info[i].work_size   = sizeof(work_impfwinit[0]);
    }
    impfwinit.instance_num  = impfw_useinstance;
    impfwinit.use_core_num  = sizeof(impfw_usecore)/sizeof(impfw_usecore[0]);		
    impfwinit.core_info     = impfw_usecore;			
    impfwinit.callback_func_fatal = CallBackFuncFatal;
    impfwinit.fw_resource.max_queue_num = 4u;
    impfwinit.fw_resource.max_msg_num   = 6u*21u;
    impfwinit.fw_resource.msg_id[0]     = 10u;
    impfwinit.fw_resource.msg_id[1]     = 11u;
    impfwinit.fw_resource.mutex_id[0]   = 12u;
    impfwinit.fw_resource.mutex_id[1]   = 13u;
    impfwinit.fw_resource.mutex_id[2]   = 14u;
    impfwinit.fw_resource.task_id[0]    = 15u;
    impfwinit.fw_resource.timeout       = 10000u;
    impfwinit.fw_resource.task_priority = OSAL_THREAD_PRIORITY_TYPE6;
    impfwinit.fw_resource.task_stacksize  = 8192u;
    impfwinit.drv_resource.mutex_id     = 20u;
    impfwinit.drv_resource.mutex_timeout= 10000u;
    impfwinit.drv_resource.int_priority = OSAL_INTERRUPT_PRIORITY_TYPE7;		

    impfw_ret = R_IMPFW_Init(&impfwinit, impfwctl);
    if (impfw_ret != IMPFW_EC_OK)
    {
        printf("<< [ERROR] r_impfw_Init  : code %d>>\n", impfw_ret);
		return -1;
    }
	return 0;
}


/* IMP Framework Demo Main Function */
int main(int argc, char** argv)
{
	int32_t				test_case = 1;
	e_impfw_core_type_t	core_type = IMPFW_CORE_TYPE_IMP;
	uint32_t			core_num = 0;
	int ret = -1;
	size_t len;
	size_t i;

	switch(argc)
	{
		case 1:
			printf("\narg : None\n\n");
			break;
		case 2:
			if(strcmp(argv[1], "h") == 0 )
			{
				printf("\nCommand Line\n");
				printf("output/debug/bin/impsample.exe arg1 arg2 arg3\n\n");
				printf("arg1 : testcae number : 1,2,3,4,5\n");
				printf("       Case arg1 is none,  test number 1 and IMP and core 0\n");
				printf("       Case arg1 is h, help\n");
				printf("arg2 : Core : IMP CVe DMA PSC CNN\n");
				printf("       Case arg2 is none, IMP and core 0\n");
				printf("arg3 : Core numver\n");
				printf("       Case arg3 is none,  Use core number is 0\n");
		        return 0;
			}
			else
			{
				impDemo_SetTestCase(argv[1], &test_case);
				impDemo_SetCoreType(argv, &core_type);
				printf("\narg : %s xxx x\n\n", argv[1]);
			}
			break;
		case 3:
			impDemo_SetTestCase(argv[1], &test_case);
			impDemo_SetCoreType(argv, &core_type);
			printf("\narg : %s %s x\n\n", argv[1], argv[2]);
			break;
		case 4:
		default:
			impDemo_SetTestCase(argv[1], &test_case);
			len = strlen(argv[3]);
			for(i=0; i<len; i++)
			{
				ret = isdigit(argv[3][i]);
				if(ret == 0 )
				{
					return -1;
				}
			}

			if(ret != 0)
			{
				core_num = (uint32_t)atoi(argv[3]);
			}
			else
			{
				core_num = 0;
			}
			impDemo_SetCoreType(argv, &core_type);
			printf("\narg : %s\t%s\t%s\t\n\n", argv[1], argv[2], argv[3]);
			break;
	}
	printf("TESTCASE\tCORETYPE\tNUM\n");
	printf("%d\t\t%d\t\t%d\n\n", test_case, core_type, core_num);

	/* note: test_case 0 is all test*/
	impDemo_main(test_case, core_type, core_num);

	return 0;
}

static void impDemo_SetCoreType(char** argv, e_impfw_core_type_t *core_type)
{
	int	i;
	R_IMPDEMO_CORE		core_info[R_IMPDEMO_CORE_TYPE_MAX] = {
							{"IMP", IMPFW_CORE_TYPE_IMP},
							{"CVe", IMPFW_CORE_TYPE_OCV},
							{"DMA", IMPFW_CORE_TYPE_DMAC},
							{"PSC", IMPFW_CORE_TYPE_PSCEXE},
							{"CNN", IMPFW_CORE_TYPE_CNN},		
						};

	for( i=0; i<R_IMPDEMO_CORE_TYPE_MAX; i++)
	{
		*core_type = IMPFW_CORE_TYPE_INVALID;
		if( strcmp(argv[2], core_info[i].coaname) == 0)
		{
			*core_type = core_info[i].core_type;
			break;
		}
	}
	if (i == R_IMPDEMO_CORE_TYPE_MAX)
	{
		printf("Argument2 should use IMP,CVe,DMA,PSC,CNN");
		abort();
	}
}

static int impDemo_SetTestCase(char* argv, int32_t *test_case)
{
	int ret;
	size_t len = strlen(argv);
	size_t i;

	for(i=0; i<len; i++)
	{
		ret = isdigit(argv[i]);
		if(ret == 0 )
		{
			return -1;
		}
	}
	*test_case = atoi(argv);
	return 0;
}

static int impDemo_main(int32_t test_case, e_impfw_core_type_t core_type, uint32_t core_num)
{
	/* IMP Framework */
	impfw_ctrl_handle_t     impfwctl;
	impfw_attr_handle_t     impfw_attr;
	e_impfw_api_retcode_t	impfw_ret;
	st_impfw_core_info_t	core;
//	st_impfw_core_info_t	core_map[R_IMPFW_MODULE_NUM][R_IMPFW_COREMAP_MAXID];
//	const st_impfw_core_info_t	*p_core_map[R_IMPFW_MODULE_NUM];
	e_impfw_req_priority_t	priority = IMPFW_REQ_PRIORITY_0;
//	size_t				i;
	void* 				CallBackArgs = &additional_data;
	int32_t					iret;
	void					*CL_addr_virt;
	uintptr_t				CL_addr_phys;
	void					*ImgA_virt;
	uintptr_t				ImgA_phys;

	/* IMP Atomic */
	int32_t					atomic_ret = 0;
	uint32_t				Input_xsize = 16; 
	uint32_t				Input_ysize = 16;
	uint32_t				Output_xsize = 16;
	uint32_t				Output_ysize = 16;
	uint32_t				CL_size = 5120;
	R_ATMLIB_CLData			cldata;
	R_ATMLIB_IMPSubsetParam2	param;
	R_ATMLIB_ImageParam		imgparam;
	
	/* OSAL */
	e_osal_return_t           ret_osal;
	osal_memory_buffer_handle_t Cl_buffer;
	osal_memory_buffer_handle_t Output_buffer;
	void					*ImgOut_virt;
	uintptr_t				ImgOut_phys;
	uint32_t				*CL_data = NULL;
	uint32_t				CL_count = 9;	
	/**/
	osal_memory_buffer_handle_t Input_buffer;
	uint8_t					*Input_data = NULL;
	uint32_t				Input_size = (uint32_t)(Input_xsize * Input_ysize * sizeof(uint32_t));
	uint32_t				*Excute_data = NULL;
	uint8_t					Resume_flg = 0;

	core.core_type = core_type;

	ret_osal = R_OSAL_Initialize();
    if (ret_osal != OSAL_RETURN_OK) {
        return 0;
    }
	iret = mmngr_init();
    if (iret != 0) {
        return 0;
    }

	memset(&cldata, 0, sizeof(R_ATMLIB_CLData));
	memset(&param, 0, sizeof(R_ATMLIB_IMPSubsetParam2));
	memset(&imgparam, 0, sizeof(R_ATMLIB_ImageParam));
	
	printf("<IMP Framework Demo start>\n\n");
	
	/*----- CL Initialization -----*/
	/* Allocate Image memory */

	if(test_case == 1 ) {
		atomic_ret = 1;
	}
	if(test_case == 3 ) {
		Resume_flg = 1;
	}
	else if(test_case == 4) {
		Input_data = IMPcore_Input_data;
		Input_size = (uint32_t)(Input_xsize * Input_ysize * sizeof(uint8_t));
		CL_count = (uint32_t)(sizeof(Atomic_IMP_CL) / sizeof(Atomic_IMP_CL[0]));
		Excute_data = (uint32_t *)Expected_value_IMP;
	}
	else if(test_case == 5) {
		Input_xsize = 1024;
		Input_ysize = 1536;
		Output_xsize = 1024;
		Output_ysize = 2048;
		Input_data = CNN_Input_data;
		Input_size = (uint32_t)(Input_xsize * Input_ysize * sizeof(uint8_t));
		CL_count = sizeof(Atomic_CNN_CL) / sizeof(Atomic_CNN_CL[0]);
		Excute_data = (uint32_t *)Expected_value_CNN;
	}
	
	/*--*/
	iret = mmngr_alloc( (uint32_t)(Input_xsize * Input_ysize * sizeof(uint32_t)), &Input_buffer, &ImgA_virt, &ImgA_phys );
	if (iret<0){
		printf("<< [ERROR] allocate input buffer>>\n");
		goto LABEL_RETURN;
	}
	if(Input_data != NULL) {
		FillMemory_demo( (void *)ImgA_virt, Input_data,  Input_size);
	}
	else {
		FillMemory( (void *)ImgA_virt, 0, Input_size );
	}

#if PRINT_ADDRESS_LOG
	printf("ImgA_virt:    0x%p\n", ImgA_virt);
	printf("ImgA_phys:    0x%x\n", (uint32_t)ImgA_phys);
#endif

	mmngr_pargecache(Input_buffer);
	
	/*--*/
	iret = mmngr_alloc( (uint32_t)(CL_size * sizeof(uint32_t)), &Cl_buffer, &CL_addr_virt, &CL_addr_phys );
	if (iret<0){
		printf("<< [ERROR] allocate cl buffer>>\n");
		goto LABEL_RETURN;
	}
	FillMemory( CL_addr_virt, 0,  (uint32_t)(CL_size * sizeof(uint32_t)));

	mmngr_pargecache(Cl_buffer);

	/*--*/

#if PRINT_ADDRESS_LOG
	printf("CL_addr_virt:\t0x%p\n", CL_addr_virt);
	printf("CL_addr_phys:\t0x%x\n", (uint32_t)CL_addr_phys);
#endif

	iret = mmngr_alloc( (uint32_t)(Output_xsize * Output_ysize * sizeof(uint32_t)), &Output_buffer, &ImgOut_virt, &ImgOut_phys );
	if (iret<0){
		printf("<< [ERROR] allocate input buffer>>\n");
		goto LABEL_RETURN;
	}
	FillMemory( (void *)ImgOut_virt, 0, (uint32_t)(Output_xsize * Output_ysize * sizeof(uint32_t)) );
	
#if PRINT_ADDRESS_LOG
	printf("ImgOut_virt:    0x%p\n", ImgOut_virt);
	printf("ImgOut_phys:    0x%x\n", (uint32_t)ImgOut_phys);
#endif

	mmngr_pargecache(Output_buffer);

	/*----- IMP Framework Initialization -----*/
	iret = impfw_init(&impfwctl);
	if (iret<0){
		printf("<< [ERROR] r_impfw_Init>>\n");
		goto LABEL_RETURN;
	}
	if(atomic_ret == 1) {
		/*----- Atomic Initialization -----*/
		printf("Execute Atomic processing Start\n");
	
		atomic_ret = r_atmlib_InitializeIMPCL( &cldata, (uint32_t *)CL_addr_virt, 256 );
		if( atomic_ret != R_ATMLIB_E_OK){
			printf("<< [ERROR] r_atmlib_InitializeIMPCL  : code %d>>\n", atomic_ret);
			goto LABEL_RETURN;
		}
	
		/*----- Atomic CL Creation -----*/
		/* Set IMP Subset */
		param.leng			= (Input_ysize<<16)|Input_xsize;
		param.mag 			= (1<<16)|1;
		param.scale			= 0;
		param.const_a		= 0xAA;
		param.const_b		= 0x0;
		param.binthr_min	= 0;
		param.binthr_max	= 0;
		param.opt			= 0;
		param.srca_unpack	= R_ATMLIB_DISABLE;
		param.srcb_unpack	= R_ATMLIB_DISABLE;
		param.dst_pack		= R_ATMLIB_DISABLE;
	
		atomic_ret = r_atmlib_SetDataIMPCL2( &cldata, R_ATMLIB_IMP_CONST, &param );
		if( atomic_ret != R_ATMLIB_E_OK){
			printf("<< [ERROR] r_atmlib_SetDataIMPCL  : code %d>>\n", atomic_ret);
			goto LABEL_RETURN;
		}
	
		cldata.cltype = 1U;
	
		/* Set Image */
		imgparam.srca_type		= R_ATMLIB_IMG_8U;
		imgparam.dst_type		= R_ATMLIB_IMG_8U;
		imgparam.srca_stride	= Input_xsize;
		imgparam.dst_stride		= Input_xsize;
		imgparam.srca_addr		= (uint32_t)ImgA_phys;
		imgparam.dst_addr		= (uint32_t)ImgA_phys;
	
		atomic_ret = r_atmlib_SetImageIMPCL( &cldata, &imgparam );
		if(atomic_ret != R_ATMLIB_E_OK){
			printf("<< [ERROR] r_atmlib_SetImageIMPCL  : code %d>>\n", atomic_ret);
			goto LABEL_RETURN;
		}
	
		/*----- Atomic Finalization -----*/
		atomic_ret = r_atmlib_FinalizeIMPCL( &cldata );
		if( atomic_ret != R_ATMLIB_E_OK){
			printf("<< [ERROR] r_atmlib_FinalizeIMPCL  : code %d>>\n", atomic_ret);
			goto LABEL_RETURN;
		}
		printf("Execute Atomic processing End\n\n");
	}
	else {
		CL_data = (uint32_t *)CL_addr_virt;
		CL_Setting(CL_data, CL_count, test_case, core.core_type, (uint32_t)ImgA_phys, (uint32_t)ImgOut_phys);
	}
	mmngr_pargecache(Cl_buffer);
	
	/*----- IMP Framework Execution -----*/
	printf("Execute r_impfw_Execute Start\n");
	
	complete_flg = 0;
	core.core_num  = core_num;
	
	/* Cache maintenance before HW execution */
	
	impfw_ret = R_IMPFW_AttrInit(impfwctl, &core, &impfw_attr);
    if (impfw_ret != IMPFW_EC_OK)
    {
        printf("<< [ERROR] R_IMPFW_AttrInit  : code %d>>\n", impfw_ret);
    }

	impfw_ret = R_IMPFW_AttrSetCl(impfw_attr, (uint32_t)CL_addr_phys, 4, priority);
    if (impfw_ret != IMPFW_EC_OK)
    {
        printf("<< [ERROR] R_IMPFW_AttrSetCL  : code %d>>\n", impfw_ret);
    }
    /* Cache maintenance before HW execution */

    impfw_ret = R_IMPFW_Execute(impfwctl, &core, impfw_attr,
                                CallBackFunc, CallBackArgs);
	if (impfw_ret != IMPFW_EC_OK){
		printf("<< [ERROR] r_impfw_Execute  : code %d>>\n", impfw_ret);
		goto LABEL_RETURN;
	}
	printf("Execute r_impfw_Execute End\n\n");
	
	/* Wait for CL end before r_impfw_Quit, or inifinity process loop */
	while (!complete_flg)
	{
		R_OSAL_ThreadSleepForTimePeriod(1);
	}

	if(Resume_flg == 1) {
		printf("Execute r_impfw_ResumeCL Start\n\n");
		complete_flg = 0;
		impfw_ret = R_IMPFW_Resume(
									impfwctl,
									&core );
		if(impfw_ret != IMPFW_EC_OK) {
			printf("<< [ERROR] r_impfw_Resume  : code %d>>\n", impfw_ret);
			goto LABEL_RETURN;
		}
		else {
			printf("Execute r_impfw_Resume End\n\n");
			/* Wait for CL end before r_impfw_Quit, or inifinity process loop */
			while (!complete_flg)
			{
				R_OSAL_ThreadSleepForTimePeriod(1);
			}
		}
	}

	mmngr_pargecache(Output_buffer);
	mmngr_pargecache(Cl_buffer);
	mmngr_pargecache(Input_buffer);

	/* Cache maintenance after HW execution */
	
	printf(" <<<<<< Input Image >>>>>>\n");
	OutputMemory( (void *)ImgA_virt, 16, 16, 1 );
	
	if(test_case == 4) {
		printf(" <<<<<< Output Image >>>>>>\n");
		OutputMemory( (void *)ImgOut_virt, Output_xsize, Output_ysize, 2 );
		printf(" <<<<<< Output Image Check >>>>>>\n");
		Output_check( (void *)ImgOut_virt, Excute_data, Output_xsize * Output_ysize, 2 );
	}
	else if(test_case == 5) {
		printf(" <<<<<< Output Image >>>>>>\n");
		OutputMemory( (void *)ImgOut_virt, Output_xsize, Output_ysize, 2 );
		printf(" <<<<<< Output Image Check >>>>>>\n");
		Output_check( (void *)ImgOut_virt, Excute_data, Output_xsize * Output_ysize, 2 );
	}

	/*----- IMP Framework Finalization -----*/
	impfw_ret = R_IMPFW_Quit(impfwctl);
	if (impfw_ret != IMPFW_EC_OK){
		printf("<< [ERROR] r_impfw_Quit  : code %d>>\n", impfw_ret);
		goto LABEL_RETURN;
	}
	
	mmngr_free(Input_buffer);
	mmngr_free(Cl_buffer);
	mmngr_free(Output_buffer);
	ret_osal = R_OSAL_MmngrClose(osal_mmngr);
	
	{
		printf("Execute R_OSAL_MmngrClose End\n\n");
	}
//	rcv_memdrv_Quit();

	ret_osal = R_OSAL_Deinitialize();
    if (ret_osal != OSAL_RETURN_OK) {
        return 0;
    }
	
	printf("<IMP Framework Demo end>\n\n");
	

LABEL_RETURN:
	return 0;
}

static void CallBackFuncFatal(e_impfw_fatalcode_t  reason, uint32_t value)
{
    printf("CallBackFuncFatal : fatalcond:%d, value:%d\n", (uint32_t)reason, value);
}

/* Call Back Function from IMP Framework */
static void CallBackFunc(
	e_impfw_callback_reason_t Status,        /* CL execution Callback status     */
	const st_impfw_core_info_t * const core, /* Core ID related to this callback */
	int32_t code,                            /* INT code or TRAP code or -1(unavailable) */
	void const * CallBackArgs)               /* The pointer to user data */
{
	char msg_cbargs[80];
	if (CallBackArgs == (void*)&additional_data)
	{
		sprintf(msg_cbargs, "%s", "&additional_data");
	}
	else
	{
		sprintf(msg_cbargs, "address %p", CallBackArgs);
	}
	printf("%x\n",Status);
	
	if(Status == IMPFW_CB_STARTED)
	{
		printf("CallBack : status STARTED     , Core type %d, Core num %d, CallBackArgs %s %d\n",(core->core_type), (core->core_num), msg_cbargs, code);
	}
    else if(Status == IMPFW_CB_TRAP) 
	{
		printf("CallBack : status TRAP        , Core type %d, Core num %d, CallBackArgs %s %d\n",(core->core_type), (core->core_num), msg_cbargs, code);
		complete_flg = 1;
	}
    else if(Status == IMPFW_CB_INT) 
	{
		printf("CallBack : status INT         , Core type %d, Core num %d, CallBackArgs %s %d\n",(core->core_type), (core->core_num), msg_cbargs, code);
		complete_flg = 1;
	}
    else if(Status == IMPFW_CB_INT_PBCOVF) 
	{
		printf("CallBack : status PBCOVF       , Core type %d, Core num %d, CallBackArgs %s %d\n",(core->core_type), (core->core_num), msg_cbargs, code);
		complete_flg = 1;
	}
    else if(Status == IMPFW_CB_ERROR_ILLEGAL) 
	{
		printf("CallBack : status ERROR_ILLEGAL, Core type %d, Core num %d, CallBackArgs %s %d\n",(core->core_type), (core->core_num), msg_cbargs, code);
		complete_flg = 1;
	}
	else if(Status == IMPFW_CB_ERROR_INTERNAL) 
	{
		printf("CallBack : status ERROR_INTERNAL, Core type %d, Core num %d, CallBackArgs %s %d\n",(core->core_type), (core->core_num), msg_cbargs, code);
		complete_flg = 1;
	}
		
	return;
}

static void CL_Setting( 
	uint32_t	*CL_data, 
	uint32_t	CL_count, 
	int32_t		test_case,
	e_impfw_core_type_t core_type,
	uint32_t	ImgA_phys,
	uint32_t	ImgOut_phys)
{
	size_t i;
	
	if( test_case == 2 ){
		if( core_type == IMPFW_CORE_TYPE_IMP ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = IMP_data_case1[i];
				CL_data++;
			}
		}
		else if( core_type == IMPFW_CORE_TYPE_OCV ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = OCV_data_case1[i];
				CL_data++;
			}
		}
		else if( core_type == IMPFW_CORE_TYPE_DMAC ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = DMAC_data_case1[i];
				CL_data++;
			}
		}
		else if( core_type == IMPFW_CORE_TYPE_PSCEXE ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = PSC_data_case1[i];
				CL_data++;
			}
		}
		else if( core_type == IMPFW_CORE_TYPE_CNN ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = CNN_data_case1[i];
				CL_data++;
			}
		}
	}
	else if( test_case == 3 ) {
		if( core_type == IMPFW_CORE_TYPE_IMP ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = IMP_data_case2[i];
				CL_data++;
			}
		}
		else if( core_type == IMPFW_CORE_TYPE_OCV ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = OCV_data_case2[i];
				CL_data++;
			}
		}
		else if( core_type == IMPFW_CORE_TYPE_DMAC ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = DMAC_data_case2[i];
				CL_data++;
			}
		}
		else if( core_type == IMPFW_CORE_TYPE_PSCEXE ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = PSC_data_case2[i];
				CL_data++;
			}
		}
		else if( core_type == IMPFW_CORE_TYPE_CNN ) {
			for(i = 0; i < CL_count; i++) {
				*CL_data = CNN_data_case2[i];
				CL_data++;
			}
		}
	}
	else if( test_case == 4 ) {
		for(i = 0; i < CL_count; i++) {
			*CL_data = Atomic_IMP_CL[i];
			if(*CL_data == 0x00924100){
				*CL_data = ImgA_phys;
			}
			else if(*CL_data == 0x00951700) {
				*CL_data = ImgOut_phys;
			}
			CL_data++;
		}
	}
	else if( test_case == 5 ) {
		for(i = 0; i < CL_count; i++) {
			*CL_data = Atomic_CNN_CL[i];
			if(*CL_data == 0x8ea7fc00) {
				*CL_data = ImgA_phys;
			}
			else if(*CL_data == 0x8ea7fc70) {
				*CL_data = ImgA_phys + 0x70;
			}
			else if(*CL_data == 0x8ea7fce2) {
				*CL_data = ImgA_phys + 0xe2;
			}
			else if(*CL_data == 0x8ea7fd54) {
				*CL_data = ImgA_phys + 0x154;
			}
			else if(*CL_data == 0x8ea7fdc6) {
				*CL_data = ImgA_phys + 0x1c6;
			}
			else if(*CL_data == 0x8ea7fe38) {
				*CL_data = ImgA_phys + 0x238;
			}
			else if(*CL_data == 0x8ea7feaa) {
				*CL_data = ImgA_phys + 0x2aa;
			}
			else if(*CL_data == 0x8ea7ff1c) {
				*CL_data = ImgA_phys + 0x31c;
			}
			else if(*CL_data == 0x8ea7ff8e) {
				*CL_data = ImgA_phys + 0x38e;
			}
			else if(*CL_data == 0x8ec00000) {
				*CL_data = ImgOut_phys;
			}
			else if(*CL_data == 0x8ec000e4) {
				*CL_data = ImgOut_phys + 0xe4;
			}
			else if(*CL_data == 0x8ec000e4) {
				*CL_data = ImgOut_phys + 0xe4;
			}
			else if(*CL_data == 0x8ec001c8) {
				*CL_data = ImgOut_phys + 0x1c8;
			}
			else if(*CL_data == 0x8ec002ac) {
				*CL_data = ImgOut_phys + 0x2ac;
			}
			else if(*CL_data == 0x8ec00390) {
				*CL_data = ImgOut_phys + 0x390;
			}
			else if(*CL_data == 0x8ec00474) {
				*CL_data = ImgOut_phys + 0x474;
			}
			else if(*CL_data == 0x8ec00558) {
				*CL_data = ImgOut_phys + 0x558;
			}
			else if(*CL_data == 0x8ec0063c) {
				*CL_data = ImgOut_phys + 0x63c;
			}
			else if(*CL_data == 0x8ec00720) {
				*CL_data = ImgOut_phys + 0x720;
			}
			CL_data++;
		}
	}
	
	return ;
}