
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by R_OSAL_Initialize() */
int32_t g_expected_initialize    = OSAL_RETURN_CONF;

/* Expected value by R_OSAL_ThreadCreate() */
int32_t g_expected_thread_create = OSAL_RETURN_OK;

/* The value of thread id */
osal_thread_id_t g_thread_id     = TEST_THREAD_ID1;

/* The number of configuration check */
int32_t g_config_check           = 6;
