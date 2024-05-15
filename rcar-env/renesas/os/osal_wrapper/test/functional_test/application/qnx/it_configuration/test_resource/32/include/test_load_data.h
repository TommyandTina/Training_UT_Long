
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* Expected value by R_OSAL_Initialize() */
int32_t  g_expected_initialize    = OSAL_RETURN_CONF;

/* Expected value by R_OSAL_InterruptGetNumOfIrqChannels() */
int32_t  g_expected_numOfChannels = OSAL_RETURN_OK;

/* Expected value by R_OSAL_InterruptRegisterIsr() */
int32_t  g_expected_register      = OSAL_RETURN_OK;

/* Expected value by R_OSAL_Deinitialize() */
int32_t  g_expected_deinitialize  = OSAL_RETURN_OK;

/* The value of device id */
/* RCAR_V3H1 */
#if defined (RCAR_V3H1)
char     *g_device_id             = "imr_00";
/* RCAR_V3H2 */
#elif (RCAR_V3H2)
char     *g_device_id             = "imr_00";
/* RCAR_V3M */
#elif (RCAR_V3M)
char     *g_device_id             = "iccom_00";
/* RCAR_V4H */
#elif (RCAR_V4H)
char      *g_device_id             = "imr_00";
/* RCAR_V4M */
#elif (RCAR_V4M)
char      *g_device_id             = "imr_00";
/* RCAR_S4 */
#elif (RCAR_S4)
char     *g_device_id             = "rtdmac_control0_00";
#endif

/* The number of irq */
uint64_t g_irq_channel            = 0;

/* The number of configuration check */
int32_t  g_config_check           = 32;
