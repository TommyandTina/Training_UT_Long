
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by R_OSAL_MmngrGetOsalMaxConfig() */
int32_t g_expected_mmngr_get_max_config        = OSAL_RETURN_CONF;

/* Expected value by OSAL_MmngrGetOsalMaxConfigFromRegionIdx() */
int32_t g_expected_mmngr_get_max_config_region = OSAL_RETURN_CONF;

/* The number of calling OSAL_MmngrGetOsalMaxConfigFromRegionIdx()*/
int32_t g_num_of_idx                  = 2;

/* The value of region index */
osal_memory_region_idx_t g_region_idx = 2;

/* The number of configuration check */
int32_t g_config_check                = 48;
