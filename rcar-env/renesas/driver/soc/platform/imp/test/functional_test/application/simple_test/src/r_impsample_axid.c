#include <rcar-xos/osal/r_osal.h>

osal_axi_bus_id_t ReturnAxiID(void)
{
    osal_axi_bus_id_t   id;
    e_osal_return_t     ret_osal = OSAL_RETURN_FAIL;

    ret_osal = R_OSAL_IoGetAxiBusIdFromDeviceName("imp_top_00", &id);
	if (ret_osal != OSAL_RETURN_OK)
	{
		id = OSAL_AXI_BUS_ID_INVALID;
	}
	return id;
}
