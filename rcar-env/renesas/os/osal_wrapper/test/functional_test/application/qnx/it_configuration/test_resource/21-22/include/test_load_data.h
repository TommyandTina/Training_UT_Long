
#include "rcar-xos/osal/r_osal.h"
#include "test_osal_resource_definition.h"

/* In the case of setting the same device id */
/* Expected value by R_OSAL_Initialize() */
int32_t  g_expected_initialize     = OSAL_RETURN_OK;

/* Expected value by R_OSAL_InterruptGetNumOfIrqChannels() */
int32_t  g_expected_numOfChannels  = OSAL_RETURN_OK;

/* Expected value by R_OSAL_InterruptRegisterIsr() */
int32_t  g_expected_register       = OSAL_RETURN_UNSUPPORTED_OPERATION;

/* Expected value by R_OSAL_Deinitialize() */
int32_t  g_expected_deinitialize   = OSAL_RETURN_OK;

/* The value of device id */
/* RCAR_V3H1 */
#if defined (RCAR_V3H1)
char     *g_device_id              = "dof_00";
/* RCAR_V3H2 */
#elif (RCAR_V3H2)
char     *g_device_id              = "dof_00";
/* RCAR_V3M */
#elif (RCAR_V3M)
char     *g_device_id              = "ivcp1e_00";
/* RCAR_V4H */
#elif (RCAR_V4H)
char     *g_device_id              = "dof_00";
/* RCAR_V4M */
#elif (RCAR_V4M)
char     *g_device_id              = "dof_00";
/* RCAR_S4 */
#elif (RCAR_S4)
char     *g_device_id              = "ships_00";
#endif

/* The number of irq */
uint64_t g_irq_channel             = 1;

/* The number of configuration check */
int32_t  g_config_check            = 21;


/* In the case of setting the different device id after "OSAL_INTERRUPT_INVALID_ID" */
/* Expected value by R_OSAL_InterruptGetNumOfIrqChannels() */
int32_t  g_expected_numOfChannels2 = OSAL_RETURN_OK;

/* Expected value by R_OSAL_InterruptRegisterIsr() */
int32_t  g_expected_register2      = OSAL_RETURN_UNSUPPORTED_OPERATION;

/* The value of device id */
/* RCAR_V3H1 */
#if defined (RCAR_V3H1)
char     *g_device_id2             = "ivcp1e_00";
/* RCAR_V3H2 */
#elif (RCAR_V3H2)
char     *g_device_id2             = "ivcp1e_00";
/* RCAR_V3M */
#elif (RCAR_V3M)
char     *g_device_id2             = "imp_top_00";
/* RCAR_V4H */
#elif (RCAR_V4H)
char     *g_device_id2             = "ivcp1e_00";
/* RCAR_V4M */
#elif (RCAR_V4M)
char     *g_device_id2             = "ivcp1e_00";
/* RCAR_S4 */
#elif (RCAR_S4)
char     *g_device_id2             = "icumx_cmd_00";
#endif
