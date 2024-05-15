
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by R_OSAL_Initialize() */
int32_t g_expected_initialize    = OSAL_RETURN_OK;

/* Expected value by R_OSAL_ThsyncCondCreate() */
int32_t g_expected_cond_create   = OSAL_RETURN_ID;

/* The value of cond id */
osal_cond_id_t g_cond_id         = TEST_COND_ID1;

/* The number of configuration check */
int32_t g_config_check           = 12;
