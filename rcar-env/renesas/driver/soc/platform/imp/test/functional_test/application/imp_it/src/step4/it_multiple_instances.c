#define TEST_MSG_NAME "Multiple_instances"

#include "it_common.h"

#if !defined (V4H) && !defined (V4H2)
  #define	INSTANCE_NUM		(5)
#else
  #define	INSTANCE_NUM		(3)
#endif
#define CNV_TO_IDX(instance)	(instance - 1)
#define CALLBACK_CODE_1			(0xAA)
#define CALLBACK_CODE_2			(0x55)

extern int32_t cbfunc_fatal(
    const st_impdrv_core_info_t *const p_core_info,     /**< Core information                   */
    const e_impdrv_cb_ret_t     ercd,                   /**< Error code                         */
    const int32_t               code,                   /**< TRAP or INT code (8 bit is set)    */
    void                        *const p_callback_args  /**< User argument                      */
);

#ifdef IT_ABNORMAL
static void clear_resource(const e_impdrv_errorcode_t ercd_sub[], impdrv_ctrl_handle_t handle[], int8_t size);
static e_impdrv_errorcode_t err_init_same_instances();
static e_impdrv_errorcode_t err_init_differed_mutex_id();
static e_impdrv_errorcode_t err_init_differed_mutex_time_period();
static e_impdrv_errorcode_t err_init_instances_with_same_cores();
static void Return_error(struct test_params_t *tparam, struct test_result_t *tret);
#endif
static void init_impdrv_ctl(st_impdrv_ctl_t impdrv_ctl[], const uint32_t size);
static int32_t init_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[]);
static int32_t set_attr_init_for_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[]);
static int32_t start_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[]);
static int32_t execute_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[]);
static int32_t resume_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[]);
static int32_t stop_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[]);
static int32_t quit_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[]);
static void Device_multiple_instances(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static void *g_msgcnf[INSTANCE_NUM];
static void *claddr;

static int32_t CbFunc_ins0(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_0)], ercd) | send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_0)], code));
}

static int32_t CbFunc_ins1(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_1)], ercd) | send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_1)], code));
}

static int32_t CbFunc_ins2(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_2)], ercd) | send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_2)], code));
}

static int32_t CbFunc_ins3(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_3)], ercd) | send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_3)], code));
}

static int32_t CbFunc_ins4(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_4)], ercd) | send_msg(g_msgcnf[CNV_TO_IDX(IMPDRV_INSTANCE_4)], code));
}

static int32_t (*p_CbFunc[INSTANCE_NUM])(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
) = {
#if !defined (V4H) && !defined (V4H2)
	[CNV_TO_IDX(IMPDRV_INSTANCE_0)]	=	&CbFunc_ins0,
	[CNV_TO_IDX(IMPDRV_INSTANCE_1)]	=	&CbFunc_ins1,
	[CNV_TO_IDX(IMPDRV_INSTANCE_2)]	=	&CbFunc_ins2,
	[CNV_TO_IDX(IMPDRV_INSTANCE_3)]	=	&CbFunc_ins3,
	[CNV_TO_IDX(IMPDRV_INSTANCE_4)]	=	&CbFunc_ins4
#else
	[CNV_TO_IDX(IMPDRV_INSTANCE_0)]	=	&CbFunc_ins0,
	[CNV_TO_IDX(IMPDRV_INSTANCE_1)]	=	&CbFunc_ins1,
	[CNV_TO_IDX(IMPDRV_INSTANCE_2)]	=	&CbFunc_ins2
#endif
};

int32_t it_Multiple_Instance_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};
	claddr = tparam->long_claddr;

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		create_msg(&g_msgcnf[i], IT_OSAL_MQ_ID_CORE_EXEC1 + i);
	}

#ifdef IT_ABNORMAL
	testno = 1;
	Return_error(tparam, &tret);
#endif
	testno = 5;
	Device_multiple_instances(tparam, &tret);

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		destroy_msg(g_msgcnf[i]);
	}

	PrintTestResult(tret, test_result);

	return 0;
}

#ifdef IT_ABNORMAL
static void clear_resource(const e_impdrv_errorcode_t ercd_sub[], impdrv_ctrl_handle_t handle[], int8_t size)
{
	e_impdrv_errorcode_t ercd;

	for (int8_t i = 0; i < size; i++)
	{
		if (IMPDRV_EC_OK == ercd_sub[i])
		{
			(void)R_IMPDRV_Quit(handle[i]);
		}
	}
}

static void init_data_for_abnormal_cases(st_impdrv_initdata_t initdata[], const st_impdrv_ctl_t impdrv_ctl[], int8_t size)
{
	e_impdrv_errorcode_t ercd;

	for (int8_t i = 0; i < size; i++)
	{
		initdata[i].use_core_num					= 1u;
		initdata[i].core_info[0].core_type			= IMPDRV_CORE_TYPE_IMP;
		initdata[i].core_info[0].core_num			= 0u + i;
		initdata[i].p_work_addr						= (void *)&impdrv_ctl[i];
		initdata[i].work_size						= sizeof(impdrv_ctl[i]);
		initdata[i].callback_func_fatal				= (p_impdrv_cbfunc_fatal_t)&cbfunc_fatal;
		initdata[i].instance_num					= IMPDRV_INSTANCE_0 + i;
		initdata[i].osal_resource.mutex_id			= OSAL_MUTEX_IMP_DRV_LOCK;
		initdata[i].osal_resource.mutex_time_period	= 0;
	}
}

#define ERR_INSTANCE_NUM	(2)
static e_impdrv_errorcode_t err_init_same_instances()
{
	int8_t i, j, k;
	e_impdrv_errorcode_t ercd = IMPDRV_EC_OK;
	e_impdrv_errorcode_t ercd_sub[ERR_INSTANCE_NUM];
	impdrv_ctrl_handle_t handle[ERR_INSTANCE_NUM];
	st_impdrv_initdata_t initdata[ERR_INSTANCE_NUM];
	st_impdrv_ctl_t impdrv_ctl[ERR_INSTANCE_NUM];
	int32_t result = -1;

	memset(initdata, 0, sizeof(initdata));

	/* initialize working area for all instance */
	memset(impdrv_ctl, 0, sizeof(impdrv_ctl));
	init_impdrv_ctl(impdrv_ctl, ERR_INSTANCE_NUM);

	/* initialize initdata */
	init_data_for_abnormal_cases(initdata, impdrv_ctl, ERR_INSTANCE_NUM);
	initdata[0].instance_num = IMPDRV_INSTANCE_0;
	initdata[1].instance_num = IMPDRV_INSTANCE_0;

	for (i = 0; i < ERR_INSTANCE_NUM; i++)
	{
		ercd_sub[i] = R_IMPDRV_Init(&initdata[i], &handle[i]);
		if (IMPDRV_EC_OK == ercd)
		{
			ercd = ercd_sub[i];
		}
	}

	clear_resource(ercd_sub, handle, ERR_INSTANCE_NUM);

	return ercd;
}

static e_impdrv_errorcode_t err_init_differed_mutex_id()
{
	int8_t i, j, k;
	e_impdrv_errorcode_t ercd = IMPDRV_EC_OK;
	e_impdrv_errorcode_t ercd_sub[ERR_INSTANCE_NUM];
	impdrv_ctrl_handle_t handle[ERR_INSTANCE_NUM];
	st_impdrv_initdata_t initdata[ERR_INSTANCE_NUM];
	st_impdrv_ctl_t impdrv_ctl[ERR_INSTANCE_NUM];
	int32_t result = -1;

	memset(initdata, 0, sizeof(initdata));

	/* initialize working area for all instance */
	memset(impdrv_ctl, 0, sizeof(impdrv_ctl));
	init_impdrv_ctl(impdrv_ctl, ERR_INSTANCE_NUM);

	/* initialize initdata */
	init_data_for_abnormal_cases(initdata, impdrv_ctl, ERR_INSTANCE_NUM);
	initdata[0].osal_resource.mutex_id = OSAL_MUTEX_IMP_DRV_LOCK;
	initdata[1].osal_resource.mutex_id = OSAL_MUTEX_IMP_DRV_LOCK + 1;

	for (i = 0; i < ERR_INSTANCE_NUM; i++)
	{
		ercd_sub[i] = R_IMPDRV_Init(&initdata[i], &handle[i]);
		if (IMPDRV_EC_OK == ercd)
		{
			ercd = ercd_sub[i];
		}
	}

	clear_resource(ercd_sub, handle, ERR_INSTANCE_NUM);

	return ercd;
}

static e_impdrv_errorcode_t err_init_differed_mutex_time_period()
{
	int8_t i, j, k;
	e_impdrv_errorcode_t ercd = IMPDRV_EC_OK;
	e_impdrv_errorcode_t ercd_sub[ERR_INSTANCE_NUM];
	impdrv_ctrl_handle_t handle[ERR_INSTANCE_NUM];
	st_impdrv_initdata_t initdata[ERR_INSTANCE_NUM];
	st_impdrv_ctl_t impdrv_ctl[ERR_INSTANCE_NUM];
	int32_t result = -1;

	memset(initdata, 0, sizeof(initdata));

	/* initialize working area for all instance */
	memset(impdrv_ctl, 0, sizeof(impdrv_ctl));
	init_impdrv_ctl(impdrv_ctl, ERR_INSTANCE_NUM);

	/* initialize initdata */
	init_data_for_abnormal_cases(initdata, impdrv_ctl, ERR_INSTANCE_NUM);
	initdata[0].osal_resource.mutex_time_period = 0;
	initdata[0].osal_resource.mutex_time_period = 1;

	for (i = 0; i < ERR_INSTANCE_NUM; i++)
	{
		ercd_sub[i] = R_IMPDRV_Init(&initdata[i], &handle[i]);
		if (IMPDRV_EC_OK == ercd)
		{
			ercd = ercd_sub[i];
		}
	}

	clear_resource(ercd_sub, handle, ERR_INSTANCE_NUM);

	return ercd;
}

static e_impdrv_errorcode_t err_init_instances_with_same_cores()
{
	int8_t i, j, k;
	e_impdrv_errorcode_t ercd = IMPDRV_EC_OK;
	e_impdrv_errorcode_t ercd_sub[ERR_INSTANCE_NUM];
	impdrv_ctrl_handle_t handle[ERR_INSTANCE_NUM];
	st_impdrv_initdata_t initdata[ERR_INSTANCE_NUM];
	st_impdrv_ctl_t impdrv_ctl[ERR_INSTANCE_NUM];
	int32_t result = -1;

	memset(initdata, 0, sizeof(initdata));

	/* initialize working area for all instance */
	memset(impdrv_ctl, 0, sizeof(impdrv_ctl));
	init_impdrv_ctl(impdrv_ctl, ERR_INSTANCE_NUM);

	/* initialize initdata */
	init_data_for_abnormal_cases(initdata, impdrv_ctl, ERR_INSTANCE_NUM);
	initdata[0].core_info[0].core_type	= IMPDRV_CORE_TYPE_IMP;
	initdata[0].core_info[0].core_num	= 0u;
	initdata[1].core_info[0].core_type	= IMPDRV_CORE_TYPE_IMP;
	initdata[1].core_info[0].core_num	= 0u;

	for (i = 0; i < ERR_INSTANCE_NUM; i++)
	{
		ercd_sub[i] = R_IMPDRV_Init(&initdata[i], &handle[i]);
		if (IMPDRV_EC_OK == ercd)
		{
			ercd = ercd_sub[i];
		}
	}

	clear_resource(ercd_sub, handle, ERR_INSTANCE_NUM);

	return ercd;
}

static void Return_error(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;

	/* Init the init-ed instance */
	ercd = err_init_same_instances();
	FailTestPrint(ercd, IMPDRV_EC_NG_INSTANCE, *tret, "(No.%d) Init the init-ed instance returns %s", testno++, impdrv_err_to_name(ercd));

	/* Init instances with different mutex_id */
	ercd = err_init_differed_mutex_id();
	FailTestPrint(ercd, IMPDRV_EC_NG_INSTANCE, *tret, "(No.%d) Init instances with different mutex_id returns %s", testno++, impdrv_err_to_name(ercd));

	/* Init instances with different mutex_time_period */
	ercd = err_init_differed_mutex_time_period();
	FailTestPrint(ercd, IMPDRV_EC_NG_INSTANCE, *tret, "(No.%d) Init instances with different mutex_time_period returns %s", testno++, impdrv_err_to_name(ercd));

	/* Init instances with same cores */
	ercd = err_init_instances_with_same_cores();
	FailTestPrint(ercd, IMPDRV_EC_NG_INSTANCE, *tret, "(No.%d) Init instances with the same cores returns %s", testno++, impdrv_err_to_name(ercd));
}
#endif

static void init_impdrv_ctl(st_impdrv_ctl_t impdrv_ctl[], const uint32_t size)
{
	int8_t instance, core_type, core_num;
	
	for (instance = 0; instance < size; instance++)
	{
		for (core_type = 0; core_type < IMPDRV_CORE_TYPE_MAX; core_type++)
		{
			for (core_num = 0; core_num < IMPDRV_CORE_NUM_MAX; core_num++)
			{
				impdrv_ctl[instance].coretype_ctl[core_type].core_ctl[core_num].core_state = IMPDRV_STATE_UNINIT;
			}
		}
	}
}

static void get_core_info_for_instance(st_impdrv_initdata_t initdata[])
{
	tp_core_list_t instance_cores;

	for (int8_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		instance_cores = get_core_list(IT_IMPDRV_CMD_INSTANCES, IMPDRV_INSTANCE_0 + i);
		initdata[i].use_core_num = instance_cores.num;
		memcpy(initdata[i].core_info, instance_cores.core_info, instance_cores.size);
	}
}

static int32_t init_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[])
{
	int32_t result = 0;
	e_impdrv_errorcode_t ercd;

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)initdata[i].p_work_addr;
		ercd = R_IMPDRV_Init(&initdata[i], &handle[i]);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("Init fail for instance %d (test NG %s)\n", i, impdrv_err_to_name(ercd));
			result = -1;
			break;
		}
	}

	return result;
}

static int32_t set_attr_init_for_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[])
{
	int32_t result = 0;
	e_impdrv_errorcode_t ercd;

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)handle[i];

		for (uint32_t j = 0; j < initdata[i].use_core_num; j++)
		{
			ercd = R_IMPDRV_AttrInit(handle[i], &initdata[i].core_info[j]);
			if (IMPDRV_EC_OK != ercd)
			{
				testmsg("Set Attr_Init fail for instance %d at core_info[%d] (test NG %s)\n", i, j, impdrv_err_to_name(ercd));
				result = -1;
			}
		}
	}

	return result;
}

static int32_t start_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[])
{
	int32_t result = 0;
	e_impdrv_errorcode_t ercd;

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		for (uint32_t j = 0; j < initdata[i].use_core_num; j++)
		{
			ercd = R_IMPDRV_Start(handle[i], &initdata[i].core_info[j]);
			if (IMPDRV_EC_OK != ercd)
			{
				testmsg("Start fail for instance %d at core_info[%d], core_type = %s, core_num = %d (test NG %s)\n", i, j, 
					type_to_name(initdata[i].core_info[j].core_type), initdata[i].core_info[j].core_num, impdrv_err_to_name(ercd));
				result = -1;
			}
		}
	}

	return result;
}

static int32_t execute_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[])
{
	int32_t result = 0;
	e_impdrv_errorcode_t ercd;
	uint32_t callback_args;

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)handle[i];
		st_impdrv_coretypectl_t	*coretype_ctl;

		for (uint32_t j = 0; j < initdata[i].use_core_num; j++)
		{
			st_impdrv_core_info_t *p_core_info = &initdata[i].core_info[j];
			e_impdrv_core_type_t core_type = p_core_info->core_type;
			uint32_t core_num = p_core_info->core_num;

			coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

			if (set_commandlist(p_core_info->core_type, claddr, CALLBACK_CODE_1))
			{
				core_testmsg(p_core_info, "set_commandlist error in instance %d, core_info %d\n", i, j);
				result = -1;
			}
			else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_AttrSetCl(handle[i], p_core_info, util_mem_get_paddr(claddr))))
			{
				core_testmsg(p_core_info, "Set CL data error in instance %d, core_info %d, ercd %s\n", i, j, impdrv_err_to_name(ercd));
				result = -1;
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle[i], p_core_info, p_CbFunc[i], (void *)&callback_args);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "Execute=%s\n", impdrv_err_to_name(ercd));
					result = -1;
				}
				else
				{
					if (IMPDRV_STATE_EXEC != coretype_ctl->core_ctl[core_num].core_state)
					{
						core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not EXEC\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
						result = -1;
					}
					else if (wait_and_check_callback(g_msgcnf[i], p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback(before exec) fail\n");
						result = -1;
					}
					else if (wait_and_check_callback(g_msgcnf[i], p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE_1))
					{
						core_testmsg(p_core_info, "callback(trap) fail\n");
						result = -1;
					}
					else if (IMPDRV_STATE_READY != coretype_ctl->core_ctl[core_num].core_state)
					{
						core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not READY\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
						result = -1;
					}
				}

				util_mem_invalidate(claddr);
			}
		}
	}

	return result;
}

static int32_t resume_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[])
{
	int32_t result = 0;
	e_impdrv_errorcode_t ercd;
	uint32_t callback_args;

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)handle[i];
		st_impdrv_coretypectl_t	*coretype_ctl;

		for (uint32_t j = 0; j < initdata[i].use_core_num; j++)
		{
			st_impdrv_core_info_t *p_core_info = &initdata[i].core_info[j];
			e_impdrv_core_type_t core_type = p_core_info->core_type;
			uint32_t core_num = p_core_info->core_num;

			coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

			if (set_intcommandlist(p_core_info->core_type, claddr, CALLBACK_CODE_1, CALLBACK_CODE_2))
			{
				core_testmsg(p_core_info, "set_intcommandlist error in instance %d, core_info %d\n", i, j);
				result = -1;
			}
			else if (IMPDRV_EC_OK != (ercd = R_IMPDRV_AttrSetCl(handle[i], p_core_info, util_mem_get_paddr(claddr))))
			{
				core_testmsg(p_core_info, "Set CL data error in instance %d, core_info %d, ercd %s\n", i, j, impdrv_err_to_name(ercd));
				result = -1;
			}
			else
			{
				ercd = R_IMPDRV_Execute(handle[i], p_core_info, p_CbFunc[i], (void *)&callback_args);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "Execute=%s\n", impdrv_err_to_name(ercd));
					result = -1;
				}
				else
				{
					if (IMPDRV_STATE_EXEC != coretype_ctl->core_ctl[core_num].core_state)
					{
						core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not EXEC\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
						result = -1;
					}
					else if (wait_and_check_callback(g_msgcnf[i], p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback(before exec) fail\n");
						result = -1;
					}
					else if (wait_and_check_callback(g_msgcnf[i], p_core_info, IMPDRV_CB_RET_INT, CALLBACK_CODE_1))
					{
						core_testmsg(p_core_info, "callback(int) fail\n");
						result = -1;
					}
					else
					{
						if (IMPDRV_STATE_INT != coretype_ctl->core_ctl[core_num].core_state)
						{
							core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not INT\n",
											core_type,
											core_num,
											state_to_name(coretype_ctl->core_ctl[core_num].core_state));
							result = -1;
						}
						else
						{
							ercd = R_IMPDRV_ResumeExecution(handle[i], p_core_info);
							if (IMPDRV_EC_OK != ercd)
							{
								core_testmsg(p_core_info, "Resume=%s\n", impdrv_err_to_name(ercd));
								result = -1;
							}
							else if (wait_and_check_callback(g_msgcnf[i], p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE_2))
							{
								core_testmsg(p_core_info, "callback(trap) fail\n");
								result = -1;
							}
							else if (IMPDRV_STATE_READY != coretype_ctl->core_ctl[core_num].core_state)
							{
								core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not READY\n",
												core_type,
												core_num,
												state_to_name(coretype_ctl->core_ctl[core_num].core_state));
								result = -1;
							}
						}
					}
				}
			}

			util_mem_invalidate(claddr);
		}
	}

	return result;
}

static int32_t stop_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[])
{
	int32_t result = 0;
	e_impdrv_errorcode_t ercd;

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		for (uint32_t j = 0; j < initdata[i].use_core_num; j++)
		{
			ercd = R_IMPDRV_Stop(handle[i], &initdata[i].core_info[j]);
			if (IMPDRV_EC_OK != ercd)
			{
				testmsg("Stop fail for instance %d at core_info[%d], core_type = %s, core_num = %d (test NG %s)\n", i, j, 
					type_to_name(initdata[i].core_info[j].core_type), initdata[i].core_info[j].core_num, impdrv_err_to_name(ercd));
				result = -1;
			}
		}
	}

	return result;
}

static int32_t quit_instances(st_impdrv_initdata_t initdata[], impdrv_ctrl_handle_t handle[])
{
	int32_t result = 0;
	e_impdrv_errorcode_t ercd;

	/* unused parameters */
	(void)initdata;

	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		ercd = R_IMPDRV_Quit(handle[i]);
		if (IMPDRV_EC_OK != ercd)
		{
			testmsg("Quit fail for instance %d (test NG %s)\n", i, impdrv_err_to_name(ercd));
			result = -1;
		}
	}

	return result;
}

static void Device_multiple_instances(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	impdrv_ctrl_handle_t handle[INSTANCE_NUM];
	st_impdrv_initdata_t initdata[INSTANCE_NUM];
	st_impdrv_ctl_t impdrv_ctl_for_ins[INSTANCE_NUM];
	int32_t result = -1;

	memset(initdata, 0, sizeof(initdata));

	/* initialize working area for all instance */
	memset(impdrv_ctl_for_ins, 0, sizeof(impdrv_ctl_for_ins));
	init_impdrv_ctl(impdrv_ctl_for_ins, INSTANCE_NUM);

	/* initialize initdata */
	get_core_info_for_instance(initdata);
	for (uint32_t i = CNV_TO_IDX(IMPDRV_INSTANCE_0); i < INSTANCE_NUM; i++)
	{
		initdata[i].p_work_addr				= (void *)&impdrv_ctl_for_ins[i];
		initdata[i].work_size				= sizeof(impdrv_ctl_for_ins[i]);
		initdata[i].instance_num			= IMPDRV_INSTANCE_0 + i;
		initdata[i].callback_func_fatal		= (p_impdrv_cbfunc_fatal_t)&cbfunc_fatal;
		initdata[i].osal_resource.mutex_id	= OSAL_MUTEX_IMP_DRV_LOCK;
	}

	/* init */
	result = init_instances(initdata, handle);

	/* set_attr_init for all instances */
	if (0 == result)
	{
		result = set_attr_init_for_instances(initdata, handle);
	}

	if (0 == result)
	{
		result = start_instances(initdata, handle);
	}

	if (0 == result)
	{
		result = execute_instances(initdata, handle);
	}

	if (0 == result)
	{
		result = resume_instances(initdata, handle);
	}

	if (0 == result)
	{
		result = stop_instances(initdata, handle);
	}

	if (0 == result)
	{
		result = quit_instances(initdata, handle);
	}

	if (0 == result)
	{
		testmsg("(No.%d) Success(test OK)\n", testno++);
	}
	else
	{
		testmsg("(No.%d) Fail(test NG)\n", testno++);
	}

	CountResult(result == 0, tret);
}