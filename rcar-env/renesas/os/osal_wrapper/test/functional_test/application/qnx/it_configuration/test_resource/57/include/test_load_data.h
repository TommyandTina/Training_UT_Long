
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by OSAL_MmngrGetOsalMaxConfigFromRegionIdx() */
int32_t g_expected_mmngr_get_max_config_region = OSAL_RETURN_PAR;

/* The number of calling  R_OSAL_MmngrOpenFromRegionIdx() or OSAL_MmngrGetOsalMaxConfigFromRegionIdx()*/
int32_t g_num_of_idx                           = 1;

/* The value of region index */
osal_memory_region_idx_t g_region_idx          = 2;

/* The number of configuration check */
int32_t g_config_check                         = 57;
