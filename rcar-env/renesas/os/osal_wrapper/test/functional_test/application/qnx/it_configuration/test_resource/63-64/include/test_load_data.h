
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by R_OSAL_Initialize() */
int32_t g_expected_initialize         = OSAL_RETURN_OK;

/* Expected value by R_OSAL_MmngrOpen() */
int32_t g_expected_mmngr_open                  = OSAL_RETURN_CONF;

/* Expected value by R_OSAL_MmngrOpenFromRegionIdx() */
int32_t g_expected_mmngr_open_region           = OSAL_RETURN_CONF;

/* The number of calling  R_OSAL_MmngrOpenFromRegionIdx() or OSAL_MmngrGetOsalMaxConfigFromRegionIdx()*/
int32_t g_num_of_idx                  = 1;

/* The value of region index */
osal_memory_region_idx_t g_region_idx = 0;

/* The number of configuration check */
int32_t g_config_check                = 63;
