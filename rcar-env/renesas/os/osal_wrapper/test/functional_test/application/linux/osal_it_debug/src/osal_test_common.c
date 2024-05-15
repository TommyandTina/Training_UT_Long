/**
 * @file osal_test_common.c
 * @brief OSAL test application common module file.
 */
#include "osal_test.h"
#include "osal_test_common.h"

/* define */
#define OSAL_TEST_EXPECT_DATA_TABLE_MAX (888)

/* OSAL test Application expect value table. */
osal_test_expect_data_t expect_data[OSAL_TEST_EXPECT_DATA_TABLE_MAX] =
{
/******************************************************
	OSAL Memory Manager
******************************************************/
/*---------------------------------------------------------------------------------------------------------------*/
/*  OSAL Manager                    API ID                                      TEST ID             Expect Data  */
/*---------------------------------------------------------------------------------------------------------------*/
	{OSAL_TEST_IO,					OSAL_TEST_IODEVICEOPEN,						OSAL_TEST_ID001,	OSAL_RETURN_CONF},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRDEBUGMMNGRPRINT,				OSAL_TEST_ID001,	OSAL_RETURN_IO},
	{OSAL_TEST_MEMORY,				OSAL_TEST_MMNGRDEBUGBUFFERPRINT,			OSAL_TEST_ID001,	OSAL_RETURN_IO}
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

	unsigned int  count    = 0;
	e_osal_return_t osal_ret = OSAL_RETURN_OK;

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
