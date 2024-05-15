
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "rcar-xos/osal/r_osal.h"
#include "osal_test.h"

void osal_test_result_output(int parent, int api_id, int result, char *param) 
{
    if (param == NULL)
    {
        if (result == OSAL_TEST_OK)
        {
            printf("test case %02d_%03d result OK.\n\n", parent, api_id);
        }
        else
        {
            printf("test case %02d_%03d result NG.\n\n", parent, api_id);
        }
    }
    else
    {
        if (result == OSAL_TEST_OK)
        {
            printf("test case %02d_%03d result OK. %s\n\n", parent, api_id, param);
            test_pass++;
        } 
        else 
        {
            printf("test case %02d_%03d result NG. %s\n\n", parent, api_id, param);
            test_fail++;
        }
    }
}

void osal_test_result_timer_output(int parent, int api_id, int result, int count, char *param,
                                   st_osal_time_t *before_timestmp, st_osal_time_t *after_timestmp) 
{
    if (param == NULL)
    {
        if (result == OSAL_TEST_OK)
        {
            printf("test case %02d_%03d result OK.\n\n", parent, api_id);
        }
        else
        {
            printf("test case %02d_%03d result NG.\n\n", parent, api_id);
        }
    }
    else
    {
        if (result == OSAL_TEST_OK)
        {
            printf("test case %02d_%03d count %2d\n", parent, api_id, count);
            printf("\tbefore time stamp : sec= %d, nsec= %d\n", before_timestmp->tv_sec, before_timestmp->tv_nsec);
            printf("\tafter time stamp  : sec= %d, nsec= %d\n", after_timestmp->tv_sec, after_timestmp->tv_nsec);
            printf("\tresult OK. %s\n\n", param);
            test_pass++;
        } 
        else 
        {
            printf("test case %02d_%03d count %2d\n", parent, api_id, count);
            printf("\tbefore time stamp : sec= %d, nsec= %d\n", before_timestmp->tv_sec, before_timestmp->tv_nsec);
            printf("\tafter time stamp  : sec= %d, nsec= %d\n", after_timestmp->tv_sec, after_timestmp->tv_nsec);
            printf("\tresult NG. %s\n\n", param);
            test_fail++;
        }
    }
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

        case OSAL_RETURN_UNSUPPORTED_OPERATION:
            str = "OSAL_RETURN_UNSUPPORTED_OPERATION";
            break;

        case OSAL_RETURN_HANDLE_NOT_SUPPORTED:
            str = "OSAL_RETURN_HANDLE_NOT_SUPPORTED";
            break;

        case OSAL_RETURN_UNKNOWN_NAME:
            str = "OSAL_RETURN_UNKNOWN_NAME";
            break;

        case OSAL_RETURN_CONF_UNSUPPORTED:
            str = "OSAL_RETURN_CONF_UNSUPPORTED";
            break;

        case OSAL_RETURN_OK_RQST:
            str = "OSAL_RETURN_OK_RQST";
            break;

        default:
            str = "UNKNOW ERROR";
            break;

    }

    return str;
}


