
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by R_OSAL_Initialize() */
int32_t      g_expected_initialize    = OSAL_RETURN_CONF;

/* Expected value by R_OSAL_MqCreate() */
int32_t      g_expected_mq_create     = OSAL_RETURN_OK;

/* The value of mq id */
osal_mq_id_t g_mq_id                  = TEST_MQ_ID1;

/* The number of configuration check */
int32_t      g_config_check           = 19;
