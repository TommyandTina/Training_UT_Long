
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by R_OSAL_Initialize() */
int32_t g_expected_initialize    = OSAL_RETURN_CONF;

/* The number of configuration check */
int32_t g_config_check           = 77;
