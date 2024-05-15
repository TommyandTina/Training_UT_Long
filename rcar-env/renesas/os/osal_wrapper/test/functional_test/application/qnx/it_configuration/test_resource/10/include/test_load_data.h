
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by R_OSAL_Initialize() */
int32_t g_expected_initialize            = OSAL_RETURN_OK;

/* Expected value by R_OSAL_ThsyncMutexCreate() */
int32_t g_expected_mutex_create          = OSAL_RETURN_ID;

/* The value of mutex id */
osal_mutex_id_t g_mutex_id      = TEST_MUTEX_ID2;

/* The number of configuration check */
int32_t g_config_check                   = 10;
