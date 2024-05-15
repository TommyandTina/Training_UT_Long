/**
 * @file osal_test_common.c
 * @brief OSAL test application common module file.
 * @author K.Takagi
 * @date 2019/11/12
 */
#include "osal_test.h"
#include "osal_test_common.h"

/* define */
#define OSAL_TEST_EXPECT_DATA_TABLE_MAX (888)

/* OSAL test Application expect value table. */
osal_test_expect_data_t expect_data[OSAL_TEST_EXPECT_DATA_TABLE_MAX] =
{
/******************************************************
	OSAL Thread Manager
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID002,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID003,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID004,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID005,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID006,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID007,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID008,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID009,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID010,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID011,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID012,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID013,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID014,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADCREATE,							OSAL_TEST_ID015,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADSLEEPUNTILTIMESTAMP,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADSLEEPFORTIMEPERIOD,				OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADSELF,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADEQUAL,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADJOIN,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_THREAD,				OSAL_TEST_THREADINITIALIZETHREADCONFIGST,		OSAL_TEST_ID001,	OSAL_RETURN_OK},

/******************************************************
	OSAL Synchronization Manager
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXCREATE,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXCREATE,					OSAL_TEST_ID002,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXCREATE,					OSAL_TEST_ID003,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXCREATE,					OSAL_TEST_ID004,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXCREATE,					OSAL_TEST_ID005,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXCREATE,					OSAL_TEST_ID006,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXCREATE,					OSAL_TEST_ID007,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXCREATE,					OSAL_TEST_ID008,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXDESTROY,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,		OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXTRYLOCK,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMUTEXUNLOCK,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDCREATE,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDCREATE,						OSAL_TEST_ID002,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDCREATE,						OSAL_TEST_ID003,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDCREATE,						OSAL_TEST_ID004,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDCREATE,						OSAL_TEST_ID005,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDCREATE,						OSAL_TEST_ID006,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDCREATE,						OSAL_TEST_ID007,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDCREATE,						OSAL_TEST_ID008,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDDESTROY,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDBROADCAST,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCCONDSIGNAL,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_SYNCHRONIZATION,		OSAL_TEST_THSYNCMEMORYBARRIER,					OSAL_TEST_ID001,	OSAL_RETURN_OK},

/******************************************************
	OSAL Message Manager
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQCREATE,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQCREATE,								OSAL_TEST_ID002,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQCREATE,								OSAL_TEST_ID003,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQCREATE,								OSAL_TEST_ID004,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQCREATE,								OSAL_TEST_ID005,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQCREATE,								OSAL_TEST_ID006,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQCREATE,								OSAL_TEST_ID007,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQCREATE,								OSAL_TEST_ID008,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQDELETE,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQSENDUNTILTIMESTAMP,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQSENDFORTIMEPERIOD,					OSAL_TEST_ID001,	OSAL_RETURN_TIME},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQRECEIVEUNTILTIMESTAMP,				OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQRECEIVEFORTIMEPERIOD,				OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQISFULL,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQISEMPTY,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQGETCONFIG,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQRESET,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MESSAGE,				OSAL_TEST_MQINITIALIZEMQCONFIGST,				OSAL_TEST_ID001,	OSAL_RETURN_OK},

/******************************************************
	OSAL Clock&Time Manager
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_CLOCK_TIME,			OSAL_TEST_CLOCKTIMEGETTIMESTAMP,				OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_CLOCK_TIME,			OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE,		OSAL_TEST_ID001,	OSAL_RETURN_OK},

/******************************************************
	OSAL Interrupt Manager
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTGETNUMOFIRQCHANNELS,			OSAL_TEST_ID001,	OSAL_RETURN_HANDLE},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTREGISTERISR,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTREGISTERISR,					OSAL_TEST_ID002,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTREGISTERISR,					OSAL_TEST_ID003,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTREGISTERISR,					OSAL_TEST_ID004,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTREGISTERISR,					OSAL_TEST_ID005,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTREGISTERISR,					OSAL_TEST_ID006,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTUNREGISTERISR,				OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTENABLEISR,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTDISABLEISR,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTSETISRPRIORITY,				OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_INTERRUPT,			OSAL_TEST_INTERRUPTISISRCONTEXT,				OSAL_TEST_ID001,	OSAL_RETURN_OK},

/******************************************************
	OSAL IO Manager
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_IO,					OSAL_TEST_IOGETNUMOFDEVICES,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETDEVICELIST,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IODEVICEOPEN,							OSAL_TEST_ID001,	OSAL_RETURN_ID},
	{OSAL_TEST_IO,					OSAL_TEST_IODEVICEOPEN,							OSAL_TEST_ID002,	OSAL_RETURN_ID},
	{OSAL_TEST_IO,					OSAL_TEST_IODEVICECLOSE,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOREAD8,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOREAD16,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOREAD32,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOREAD64,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOBLOCKREAD8,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOBLOCKREAD16,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOBLOCKREAD32,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOBLOCKREAD64,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOWRITE8,								OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOWRITE16,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOWRITE32,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOWRITE64,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOBLOCKWRITE8,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOBLOCKWRITE16,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOBLOCKWRITE32,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOBLOCKWRITE64,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETAXIBUSIDFROMDEVICENAME,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETAXIBUSNAME,						OSAL_TEST_ID001,	OSAL_RETURN_PAR},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETAXIBUSNAMELIST,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETDEVICEAXIBUSID,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETAXIIDLISTFORAXIBUSNAME,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETAXIIDLISTFORDEVICEHDL,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETAXIIDLISTFORDEVICENAME,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETNUMOFAXIIDSFORAXIBUSNAME,		OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETNUMOFAXIIDSFORDEVICEHDL,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_IO,					OSAL_TEST_IOGETNUMOFAXIIDSFORDEVICENAME,		OSAL_TEST_ID001,	OSAL_RETURN_OK},

/******************************************************
	OSAL PM Manager
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_PM,					OSAL_TEST_PMSETREQUIREDSTATE,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMGETREQUIREDSTATE,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMWAITFORSTATE,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMGETSTATE,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMSETPOLICY,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMGETPOLICY,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMSETRESETSTATE,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMGETRESETSTATE,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMSETPOSTCLOCK,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_PM,					OSAL_TEST_PMGETLOWESTINTERMEDIATEPOWERSTATE,	OSAL_TEST_ID001,	OSAL_RETURN_OK},

/******************************************************
	OSAL Memory Manager
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGROPEN,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGROPENFROMREGIONIDX,				OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRCLOSE,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRALLOC,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRDEALLOC,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRGETCONFIG,						OSAL_TEST_ID001,	OSAL_RETURN_CONF},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRGETOSALMAXCONFIG,				OSAL_TEST_ID001,	OSAL_RETURN_CONF},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRGETOSALMAXCONFIGFROMREGIONIDX,	OSAL_TEST_ID001,	OSAL_RETURN_CONF},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRINITIALIZEMMNGRCONFIGST,			OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRREGISTERMONITOR,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRGETCPUPTR,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRGETCONSTCPUPTR,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRFLUSH,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRINVALIDATE,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRGETHWADDR,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRGETSIZE,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRCHECKBUFFER,						OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRDEBUGMMNGRPRINT,					OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRDEBUGBUFFERPRINT,				OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRINITIALIZEMEMORYMANAGEROBJ,		OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRINITIALIZEMEMORYBUFFEROBJ,		OSAL_TEST_ID001,	OSAL_RETURN_OK},

/******************************************************
	OSAL Common Function
******************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                          TEST ID             Expect Data  */
/*-------------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_COMMON_FUNCTION,		OSAL_TEST_INITIALIZE,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_COMMON_FUNCTION,		OSAL_TEST_DEINITIALIZE,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
	{OSAL_TEST_COMMON_FUNCTION,		OSAL_TEST_GETVERSION,							OSAL_TEST_ID001,	OSAL_RETURN_OK},
};

void osal_test_result_output(int parent, int api_id, int test_id, int result, char *param)
{
	if (param == NULL)
	{
		if (result == OSAL_TEST_OK)
		{
			printf("test case %02d_%03d_%03d result OK.\n\n", parent, api_id, test_id);
		}
		else
		{
			printf("test case %02d_%03d_%03d result NG.\n\n", parent, api_id, test_id);
		}
	}
	else
	{
		if (result == OSAL_TEST_OK)
		{
			printf("test case %02d_%03d_%03d result OK. %s\n\n", parent, api_id, test_id, param);
			test_pass ++;
		}
		else
		{
			printf("test case %02d_%03d_%03d result NG. %s\n\n", parent, api_id, test_id, param);
			test_fail ++;
		}
	}
}

uint64_t osal_test_get_expectdata(int parent, int api_id, int test_id)
{
	unsigned int	count		= 0;
	e_osal_return_t	osal_ret	= OSAL_RETURN_OK;

	/* expect data search */
	for(count = 0; count < OSAL_TEST_EXPECT_DATA_TABLE_MAX; count++)
	{
		if ((expect_data[count].parent_id == parent) &&
			(expect_data[count].api_id    == api_id) &&
			(expect_data[count].test_id   == test_id))
		{
			osal_ret = expect_data[count].expect;
			break;
		}
	}

	if (count == OSAL_TEST_EXPECT_DATA_TABLE_MAX)
	{
		printf("expect data is not registered. %02d_%03d_%03d \n", parent, api_id, test_id);
	}

	return (uint64_t)osal_ret;

}

char *osal_ret_to_str(e_osal_return_t ret_val)
{
	static char *str;

	switch(ret_val)
	{
		case OSAL_RETURN_OK:
			str = "OSAL_RETURN_OK";
			break;
		case OSAL_RETURN_PAR:
			str = "OSAL_RETURN_PAR";
			break;

		case OSAL_RETURN_MEM:
			str = "OSAL_RETURN_MEM";
			break;

		case OSAL_RETURN_DEV:
			str = "OSAL_RETURN_DEV";
			break;

		case OSAL_RETURN_TIME:
			str = "OSAL_RETURN_TIME";
			break;

		case OSAL_RETURN_CONF:
			str = "OSAL_RETURN_CONF";
			break;

		case OSAL_RETURN_FAIL:
			str = "OSAL_RETURN_FAIL";
			break;

		case OSAL_RETURN_IO:
			str = "OSAL_RETURN_IO";
			break;

		case OSAL_RETURN_BUSY:
			str = "OSAL_RETURN_BUSY";
			break;

		case OSAL_RETURN_ID:
			str = "OSAL_RETURN_ID";
			break;

		case OSAL_RETURN_HANDLE:
			str = "OSAL_RETURN_HANDLE";
			break;

		case OSAL_RETURN_STATE:
			str = "OSAL_RETURN_STATE";
			break;

		case OSAL_RETURN_PROHIBITED:
			str = "OSAL_RETURN_PROHIBITED";
			break;

		case OSAL_RETURN_OK_RQST:
			str = "OSAL_RETURN_OK_RQST";
			break;

		case OSAL_RETURN_UNKNOWN_NAME:
			str = "OSAL_RETURN_UNKNOWN_NAME";
			break;

		default:
			str = "UNKNOW ERROR";
			break;
	}

	return str;
}

e_osal_return_t io_open(char *device_type, int device_number, osal_device_handle_t *dev_handle)
{
	e_osal_return_t 		osal_ret = OSAL_RETURN_OK;
	size_t					numOfDevices = 0;
	char					list[950];
	size_t					list_size;
	size_t					numOfLists = 0;
	char					*device[100];
	int 					i = 0;
	const char				s[2] = "\n";
	osal_device_handle_t	local_handle;

	memset(&local_handle, 0x00, sizeof(osal_device_handle_t));

	osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
	if (osal_ret != OSAL_RETURN_OK)
	{
		return osal_ret;
	}

	list_size = sizeof(list);
	osal_ret  = R_OSAL_IoGetDeviceList(device_type, list, list_size, &numOfLists);
	if (osal_ret != OSAL_RETURN_OK)
	{
		return osal_ret;
	}

	if (numOfLists != 0)
	{
		device[i] = strtok(list, s);
		while(device[i] != NULL) {
			i++;
			device[i] = strtok(NULL, s);
		}

		if(i > numOfDevices){
			osal_ret = OSAL_RETURN_FAIL;
			return osal_ret;
		}
	}

	printf("Num of device(%d)\n", numOfDevices);
	printf("device list: %s , list_size(%d)  numOfLists(%d)\n", list, list_size, numOfLists);
	printf("target device name: %s , %d\n", device[device_number], device_number);

	osal_ret = R_OSAL_IoDeviceOpen(device[device_number], &local_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		return osal_ret;
	}
	else
	{
		*dev_handle = local_handle;
	}

	return osal_ret;
}
