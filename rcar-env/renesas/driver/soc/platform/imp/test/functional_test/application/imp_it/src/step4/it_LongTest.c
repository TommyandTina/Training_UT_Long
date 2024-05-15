#define TEST_MSG_NAME "LongTest"

#include "it_common.h"

#define CALLBACK_CODE1		(0xFD)
#define CALLBACK_CODE2		(0xDF)
#define CALLBACK_CODE3		(0xEE)

#define INTERFACE_NUM		(30)

#if defined (TARGET_OS_QNX)
#define LONGTEST_LOOPS		(10)
#else
#define LONGTEST_LOOPS		(100)
#endif

extern int32_t cbfunc_fatal(
    const st_impdrv_core_info_t *const p_core_info,     /**< Core information                   */
    const e_impdrv_cb_ret_t     ercd,                   /**< Error code                         */
    const int32_t               code,                   /**< TRAP or INT code (8 bit is set)    */
    void                        *const p_callback_args  /**< User argument                      */
);

static void Device_long_time(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;
static void *g_msgcnf;
static void *claddr;

static int32_t CbFunc(
	const st_impdrv_core_info_t *p_core_info,		/**< Core information					*/
	const e_impdrv_cb_ret_t		ercd,				/**< Error code							*/
	const int32_t				code,				/**< TRAP or INT code (8 bit is set)	*/
	const void					*p_callback_args
)
{
	return (send_msg(g_msgcnf, ercd) | send_msg(g_msgcnf, code));
}

int32_t it_Long_Time_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};
	claddr = tparam->long_claddr;

	create_msg(&g_msgcnf, IT_OSAL_MQ_ID_CORE_EXEC1);

	testno = 1;
	Device_long_time(tparam, &tret);

	destroy_msg(g_msgcnf);

	PrintTestResult(tret, test_result);

	return 0;
}

static void Device_long_time(struct test_params_t *tparam, struct test_result_t *tret)
{
	e_impdrv_errorcode_t ercd;
	st_impdrv_initdata_t *p_initdata = &tparam->initdata;
	impdrv_ctrl_handle_t handle;
	int32_t result = 0;
	int32_t i, k, count;
	st_impdrv_version_t *s_version;
	st_impdrv_reg_info_t reg_info;
	const char *device_id = "imp_spmi_00";
	uint32_t write_data = 5U;
	uint32_t read_data;
	e_impdrv_pm_policy_t policy;
	uint32_t callback_args;
	bool irq_mask[IMPDRV_IRQMASK_MAX];
	st_impdrv_irq_group_t irq_param;
	st_impdrv_chk_resource_t chk_resource;
	tp_arr_info_t data_info;
	st_impdrv_chk_param_t *p_chk_param;
	uint32_t param_num;
	e_impdrv_pm_state_t pmstate;

	uint64_t start[INTERFACE_NUM];
	uint64_t end[INTERFACE_NUM];
	uint64_t total[INTERFACE_NUM];
	uint64_t pm_start[INTERFACE_NUM];
	uint64_t pm_end[INTERFACE_NUM];
	uint64_t pm_total[INTERFACE_NUM];

	memset(start, 0, sizeof(start));
	memset(end, 0, sizeof(end));
	memset(total, 0, sizeof(total));
	memset(pm_start, 0, sizeof(pm_start));
	memset(pm_end, 0, sizeof(pm_end));
	memset(pm_total, 0, sizeof(pm_total));

	memset(irq_mask, true, sizeof(irq_mask));

	initialize_impdrvctl(tparam);
	set_input_data_before_init(tparam);
	memset(tparam->core_map, 0, sizeof(tparam->core_map));

	chk_resource.memory_handle = util_mem_get_handle(tparam->busif_mem);
	chk_resource.axi_id = imp_dev_axi_bus_id;
	chk_resource.mq_id = OSAL_MQ_IMP_RTT_DRVCB_00;
	chk_resource.mq_time_period = 0;

	for (i = 0; i < p_initdata->use_core_num; i++)
	{
		st_impdrv_ctl_t *p_impdrv_ctl = (st_impdrv_ctl_t *)tparam->initdata.p_work_addr;
		st_impdrv_core_info_t *p_core_info = &p_initdata->core_info[i];
		e_impdrv_core_type_t core_type = p_core_info->core_type;
		uint32_t core_num = p_core_info->core_num;
		st_impdrv_coretypectl_t *coretype_ctl = &p_impdrv_ctl->coretype_ctl[core_type];

#if defined (V3H1)
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
#elif defined (V3H2)
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#elif defined (V3M2)
		if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 2;
		}
		if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 3;
		}
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 4;
		}
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno += 1;
		}
#elif defined (V4H) || defined (V4H2)
		if ((IMPDRV_CORE_TYPE_OCV == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 7;
		}
		if ((IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 12;
		}
		if ((IMPDRV_CORE_TYPE_DMAC_SLIM == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 16;
		}
		if ((IMPDRV_CORE_TYPE_PSCEXE == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 20;
		}
		if ((IMPDRV_CORE_TYPE_CNN == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 22;
		}
		if ((IMPDRV_CORE_TYPE_DSP == p_core_info->core_type) && (0 == p_core_info->core_num))
		{
			testno = 23;
		}
#endif

		if (set_intcommandlist(p_core_info->core_type, claddr, CALLBACK_CODE1, CALLBACK_CODE2)) {
			core_testmsg(p_core_info, "set_intcommandlist fail\n");
			result = -1;
		}

		memset(start, 0, sizeof(start));
		memset(end, 0, sizeof(end));
		memset(total, 0, sizeof(total));
		memset(pm_start, 0, sizeof(pm_start));
		memset(pm_end, 0, sizeof(pm_end));
		memset(pm_total, 0, sizeof(pm_total));

		for (count = 0; (0 == result) && (LONGTEST_LOOPS > count); count++)
		{
			printf("(%d/%d)\r", count, LONGTEST_LOOPS);

			/* get version */
			fflush(stdout);
			start[0] = util_getcount();
			s_version = R_IMPDRV_GetVersion();
			end[0] = util_getcount();
			total[0] += (end[0] - start[0]);
			if ((NULL == s_version) ||
				(IMPDRV_VERSION_MAJOR != s_version->major) ||
				(IMPDRV_VERSION_MINOR != s_version->minor) ||
				(IMPDRV_VERSION_PATCH != s_version->patch))
			{
				core_testmsg(p_core_info, "R_IMPDRV_GetVersion fail\n");
				result = -1;
				continue;
			}

			/* init work */
			fflush(stdout);
			start[1] = util_getcount();
			ercd = R_IMPDRV_Init(p_initdata, &handle);
			end[1] = util_getcount();
			total[1] += (end[1] - start[1]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_Init fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				continue;
			}			

			/* set attribute */
			fflush(stdout);
			start[2] = util_getcount();
			ercd = R_IMPDRV_AttrInit(handle, p_core_info);
			end[2] = util_getcount();
			total[2] += (end[2] - start[2]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_AttrInit fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* get hardware info */
			fflush(stdout);
			start[3] = util_getcount();
			ercd = R_IMPDRV_RegGetHwInfo(handle, device_id, &reg_info);
			end[3] = util_getcount();
			total[3] += (end[3] - start[3]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_RegGetHwInfo fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* require for hardware */
			fflush(stdout);
			start[4] = util_getcount();
			ercd = R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_REQUIRED);
			end[4] = util_getcount();
			total[4] += (end[4] - start[4]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_RegRequired (required) fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* write for hardware */
			fflush(stdout);
			start[5] = util_getcount();
			ercd = R_IMPDRV_RegWrite32(handle, device_id, reg_info.addr_phys - OFFSETADDR_SPMI0, write_data);
			end[5] = util_getcount();
			total[5] += (end[5] - start[5]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_RegWrite32 fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* read for hardware */
			fflush(stdout);
			start[6] = util_getcount();
			ercd = R_IMPDRV_RegRead32(handle, device_id, reg_info.addr_phys - OFFSETADDR_SPMI0, &read_data);
			end[6] = util_getcount();
			total[6] += (end[6] - start[6]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_RegRead32 fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* release for hardware */
			fflush(stdout);
			start[7] = util_getcount();
			ercd = R_IMPDRV_RegRequired(handle, device_id, IMPDRV_REG_REQ_STATE_RELEASED);
			end[7] = util_getcount();
			total[7] += (end[7] - start[7]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_RegRequired (released) fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* set protection mode */
			fflush(stdout);
			start[8] = util_getcount();
			ercd = R_IMPDRV_SetMemProtect(handle, IMPDRV_PROTECT_EDC);
			end[8] = util_getcount();
			total[8] += (end[8] - start[8]);
#if defined (V3H2)
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_SetMemProtect fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}
#else
			if (IMPDRV_EC_NG_NOTSUPPORT != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_SetMemProtect fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}
#endif

			/* set PM policy */
			fflush(stdout);
			start[9] = util_getcount();
			ercd = R_IMPDRV_SetPMPolicy(handle, p_core_info, IMPDRV_PM_POLICY_CG);
			end[9] = util_getcount();
			total[9] += (end[9] - start[9]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_SetPMPolicy fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* get PM policy */
			fflush(stdout);
			start[10] = util_getcount();
			ercd = R_IMPDRV_GetPMPolicy(handle, p_core_info, &policy);
			end[10] = util_getcount();
			total[10] += (end[10] - start[10]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_GetPMPolicy fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

#if defined (V4H) || defined (V4H2)
			if (IMPDRV_CORE_TYPE_DSP == p_core_info->core_type)
			{
				/* set DSP data */
				fflush(stdout);
				tparam->dsp_app.addr_phys = 0x40000000U;
    			tparam->dsp_app.size = 0x2000U;
				const st_impdrv_dsp_data_t *const p_dspdata = (const st_impdrv_dsp_data_t *const)&tparam->dsp_data;
				const st_impdrv_dsp_data_t *const p_dspfw = (const st_impdrv_dsp_data_t *const)&tparam->dsp_fw;
				const st_impdrv_dsp_data_t *const p_dspapp = (const st_impdrv_dsp_data_t *const)&tparam->dsp_app;
				const st_impdrv_dsp_data_t *const p_dspdtcm = (const st_impdrv_dsp_data_t *const)&tparam->dsp_dtcm;
				start[11] = util_getcount();
				ercd = R_IMPDRV_AttrSetDsp(handle, p_core_info, p_dspapp, p_dspfw, p_dspdata, p_dspdtcm);
				end[11] = util_getcount();
				total[11] += (end[11] - start[11]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_AttrSetDsp fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
					goto _terminate;
				}
			}
#endif

			/* get PM state */
			fflush(stdout);
			start[12] = util_getcount();
			ercd = R_IMPDRV_GetPMState(handle, p_core_info, &pmstate);
			end[12] = util_getcount();
			total[12] += (end[12] - start[12]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_GetPMState fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* start */
			fflush(stdout);
			start[13] = util_getcount();
			ercd = R_IMPDRV_Start(handle, p_core_info);
			end[13] = util_getcount();
			total[13] += (end[13] - start[13]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_Start fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _terminate;
			}

			/* set core map */
			fflush(stdout);
			start[14] = util_getcount();
			ercd = R_IMPDRV_AttrSetCoreMap(handle, p_core_info, tparam->core_map);
			end[14] = util_getcount();
			total[14] += (end[14] - start[14]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_AttrSetCoreMap fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
				goto _stop;
			}

			if (IMPDRV_CORE_TYPE_DSP != p_core_info->core_type)
			{
				/* set irq mask */
				fflush(stdout);
				start[15] = util_getcount();
				ercd = R_IMPDRV_AttrSetIrqMask(handle, p_core_info, irq_mask);
				end[15] = util_getcount();
				total[15] += (end[15] - start[15]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_AttrSetIrqMask fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
					goto _stop;
				}

				/* set CL */
				fflush(stdout);
				start[16] = util_getcount();
				ercd = R_IMPDRV_AttrSetCl(handle, p_core_info, util_mem_get_paddr(claddr));
				end[16] = util_getcount();
				total[16] += (end[16] - start[16]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_AttrSetCl fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
					goto _stop;
				}

				/* execute */
				fflush(stdout);
				start[17] = util_getcount();
				ercd = R_IMPDRV_Execute(handle, p_core_info, (p_impdrv_cbfunc_t)&CbFunc, (void *)&callback_args);
				end[17] = util_getcount();
				total[17] += (end[17] - start[17]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_Execute fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
					goto _stop;
				}
				else
				{
					if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_EXEC)
					{
						core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not EXEC\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
						result = -1;
						goto _stop;
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_BEFORE_EXEC, -1))
					{
						core_testmsg(p_core_info, "callback(before exec) fail\n");
						result = -1;
					}
					else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_INT, CALLBACK_CODE1))
					{
						core_testmsg(p_core_info, "callback(int) fail\n");
						result = -1;
					}
					else
					{
						if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_INT)
						{
							core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not INT\n",
											core_type,
											core_num,
											state_to_name(coretype_ctl->core_ctl[core_num].core_state));
							result = -1;
						}
					}
				}

				/* execute */
				fflush(stdout);
				start[18] = util_getcount();
				ercd = R_IMPDRV_ResumeExecution(handle, p_core_info);
				end[18] = util_getcount();
				total[18] += (end[18] - start[18]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_ResumeExecution fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
				}
				else if (wait_and_check_callback(g_msgcnf, p_core_info, IMPDRV_CB_RET_OK, CALLBACK_CODE2))
				{
					core_testmsg(p_core_info, "callback(trap) fail\n");
					result = -1;
				}
				else
				{
					if (coretype_ctl->core_ctl[core_num].core_state != IMPDRV_STATE_READY)
					{
						core_testmsg(p_core_info, "core_type = %d, core_num = %u, state = %s, it is not READY\n",
										core_type,
										core_num,
										state_to_name(coretype_ctl->core_ctl[core_num].core_state));
						result = -1;
					}
				}

				/* set IRG group */
				fflush(stdout);
				irq_param.irq_group = IMPDRV_IRQ_GROUP_NONE;
				irq_param.group_core_num = 1;
				memcpy(&irq_param.group_core_info[0], p_core_info, sizeof(st_impdrv_core_info_t));
				start[19] = util_getcount();
				ercd = R_IMPDRV_SetIrqGroup(handle, &irq_param);
				end[19] = util_getcount();
				total[19] += (end[19] - start[19]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_SetIrqGroup fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
				}
			}


#if !defined (V4H) && !defined (V4H2)
			/* bus interface check */
			fflush(stdout);
			if ((IMPDRV_CORE_TYPE_IMP == p_core_info->core_type) ||
				(IMPDRV_CORE_TYPE_IMP_SLIM == p_core_info->core_type) ||
				(IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type))
			{
				chk_resource.memory_handle = util_mem_get_handle(tparam->busif_mem);
				chk_resource.axi_id = imp_dev_axi_bus_id;
				chk_resource.mq_id = OSAL_MQ_IMP_RTT_DRVCB_00;
				chk_resource.mq_time_period = 500U;
				start[20] = util_getcount();
				ercd = R_IMPDRV_BusIfCheck(handle, p_core_info, &chk_resource);
				end[20] = util_getcount();
				total[20] += (end[20] - start[20]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_BusIfCheck fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
				}
			}
			/* configuration register check */
			fflush(stdout);
			if (IMPDRV_CORE_TYPE_IMP == p_core_info->core_type)
			{
				data_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_CONF_REG_CHECK);
				p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
				param_num = data_info.range;
				start[21] = util_getcount();
				ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, p_core_info);
				end[21] = util_getcount();
				total[21] += (end[21] - start[21]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_ConfRegCheck fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
				}
			}

			/* module stop check */
			fflush(stdout);
			data_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
			p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
			param_num = data_info.range;
			start[22] = util_getcount();
			ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, p_core_info);
			end[22] = util_getcount();
			total[22] += (end[22] - start[22]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_ModuleStopCheck fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
			}
#else
			/* configuration register check */
			fflush(stdout);
			if (IMPDRV_CORE_TYPE_DSP == p_core_info->core_type)
			{
				data_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_CONF_REG_CHECK);
				p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
				param_num = data_info.range;
				start[20] = util_getcount();
				ercd = R_IMPDRV_ConfRegCheck(handle, p_chk_param, param_num, p_core_info);
				end[20] = util_getcount();
				total[20] += (end[20] - start[20]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_ConfRegCheck fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
				}

				/* module stop check */
				fflush(stdout);
				data_info = get_data_info(p_core_info->core_type, IT_IMPDRV_CMD_MODULE_STOP_CHECK);
				p_chk_param = (st_impdrv_chk_param_t *)data_info.info;
				param_num = data_info.range;
				start[21] = util_getcount();
				ercd = R_IMPDRV_ModuleStopCheck(handle, p_chk_param, param_num, p_core_info);
				end[21] = util_getcount();
				total[21] += (end[21] - start[21]);
				if (IMPDRV_EC_OK != ercd)
				{
					core_testmsg(p_core_info, "R_IMPDRV_ModuleStopCheck fail(%s)\n", impdrv_err_to_name(ercd));
					result = -1;
				}
			}
#endif

_stop:
			/* stop */
			fflush(stdout);
			start[23] = util_getcount();
			ercd = R_IMPDRV_Stop(handle, p_core_info);
			end[23] = util_getcount();
			total[23] += (end[23] - start[23]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_Stop fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
			}

			/* set core_mem_init */
			fflush(stdout);
			start[24] = util_getcount();
			ercd = R_IMPDRV_AttrSetCoreMemInit(handle, p_core_info, IMPDRV_PARAM_ON);
			end[24] = util_getcount();
			total[24] += (end[24] - start[24]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_AttrSetCoreMemInit fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
			}

_terminate:
			/* quit */
			fflush(stdout);
			start[25] = util_getcount();
			ercd = R_IMPDRV_Quit(handle);
			end[25] = util_getcount();
			total[25] += (end[25] - start[25]);
			if (IMPDRV_EC_OK != ercd)
			{
				core_testmsg(p_core_info, "R_IMPDRV_Quit fail(%s)\n", impdrv_err_to_name(ercd));
				result = -1;
			}

			R_OSAL_ThreadSleepForTimePeriod(10);
		}

		util_mem_invalidate(claddr);

		SuccessTestPrint(result == 0, *tret, "(No.%d) %s%d ",
			testno++, type_to_name(p_core_info->core_type), p_core_info->core_num);

		k = 0;
		printf("R_IMPDRV_GetVersion average %ld[us]\n",					util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_Init average %ld[us]\n",						util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_AttrInit average %ld[us]\n",					util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_RegGetHwInfo average %ld[us]\n",				util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_RegRequired (required) average %ld[us]\n",		util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_RegWrite32 average %ld[us]\n",					util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_RegRead32 average %ld[us]\n",					util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_RegRequired (released) average %ld[us]\n",		util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_SetMemProtect average %ld[us]\n",				util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_SetPMPolicy average %ld[us]\n",				util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_GetPMPolicy average %ld[us]\n",				util_counttousec(total[k++] / (uint64_t)count));
#if defined (V4H) || defined (V4H2)
		printf("R_IMPDRV_AttrSetDsp average %ld[us]\n",					util_counttousec(total[k++] / (uint64_t)count));
#else
		k++;
#endif
		printf("R_IMPDRV_GetPMState average %ld[us]\n",					util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_Start average %ld[us]\n",						util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_AttrSetCoreMap average %ld[us]\n",				util_counttousec(total[k++] / (uint64_t)count));

		if (IMPDRV_CORE_TYPE_DSP != p_core_info->core_type)
		{
			printf("R_IMPDRV_AttrSetIrqMask average %ld[us]\n",				util_counttousec(total[k++] / (uint64_t)count));
			printf("R_IMPDRV_AttrSetCl average %ld[us]\n",					util_counttousec(total[k++] / (uint64_t)count));
			printf("R_IMPDRV_Execute average %ld[us]\n",					util_counttousec(total[k++] / (uint64_t)count));
			printf("R_IMPDRV_ResumeExecution average %ld[us]\n",			util_counttousec(total[k++] / (uint64_t)count));
			printf("R_IMPDRV_SetIrqGroup average %ld[us]\n",				util_counttousec(total[k++] / (uint64_t)count));
		}
		else
		{
			k += 5;
		}


#if !defined (V4H) && !defined (V4H2)
		if ((IMPDRV_CORE_TYPE_IMP == p_core_info->core_type) || (IMPDRV_CORE_TYPE_IMP_SLIM == p_core_info->core_type) || (IMPDRV_CORE_TYPE_DMAC == p_core_info->core_type))
		{
			printf("R_IMPDRV_BusIfCheck average %ld[us]\n",				util_counttousec(total[k++] / (uint64_t)count));
		}
		if (IMPDRV_CORE_TYPE_IMP == p_core_info->core_type)
		{
			printf("R_IMPDRV_ConfRegCheck average %ld[us]\n",			util_counttousec(total[k++] / (uint64_t)count));
		}
		printf("R_IMPDRV_ModuleStopCheck average %ld[us]\n",			util_counttousec(total[k++] / (uint64_t)count));
#else
		if (IMPDRV_CORE_TYPE_DSP == p_core_info->core_type)
		{
			printf("R_IMPDRV_ConfRegCheck average %ld[us]\n",			util_counttousec(total[k++] / (uint64_t)count));
			printf("R_IMPDRV_ModuleStopCheck average %ld[us]\n",		util_counttousec(total[k++] / (uint64_t)count));
		}
		k++;
#endif
		printf("R_IMPDRV_Stop average %ld[us]\n",						util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_AttrSetCoreMemInit average %ld[us]\n",			util_counttousec(total[k++] / (uint64_t)count));
		printf("R_IMPDRV_Quit average %ld[us]\n",						util_counttousec(total[k++] / (uint64_t)count));
	}
}