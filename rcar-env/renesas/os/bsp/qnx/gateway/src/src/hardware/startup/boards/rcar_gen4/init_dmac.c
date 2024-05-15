/*
* $QNXLicenseC:
* Copyright (c) 2018, QNX Software Systems. All Rights Reserved.
* Copyright 2022, Renesas Electronics Corporation.
*
* You must obtain a written license from and pay applicable license fees to QNX
* Software Systems before you may reproduce, modify or distribute this software,
* or any work that includes all or part of this software. Free development
* licenses are available for evaluation and non-commercial purposes. For more
* information visit http://licensing.qnx.com or email licensing@qnx.com.
*
* This file may contain contributions from others. Please review this entire
* file for other proprietary rights or license notices, as well as the QNX
* Development Suite License Guide at http://licensing.qnx.com/license-guide/
* for other information.
* $
*/

#include "startup.h"
#include <aarch64/r-car.h>
#include "rcar_startup.h"

/*
 * This code is board dependant and may have to be
 * changed by end users.
 */
void
init_dma(void) {
        
    /* If enable read rate control, please set one or more groups here: 
     * - Replace <READ_RATE_VALUE> to real value 
     * - And uncomment code
     */
    //out32(RCAR_GEN4_SYSDMAC1_COM_BASE + RCAR_DMAC_RATE_RD, BIT(31) | <READ_RATE_VALUE>);
    //out32(RCAR_GEN4_SYSDMAC2_COM_BASE + RCAR_DMAC_RATE_RD, BIT(31) | <READ_RATE_VALUE>);

    /* If enable write rate control, please set one or more group here: 
     * - Replace <WRITE_RATE_VALUE> to real value 
     * - And uncomment code
     */
    //out32(RCAR_GEN4_SYSDMAC1_COM_BASE + RCAR_DMAC_RATE_WR, BIT(31) | <WRITE_RATE_VALUE>);
    //out32(RCAR_GEN4_SYSDMAC2_COM_BASE + RCAR_DMAC_RATE_WR, BIT(31) | <WRITE_RATE_VALUE>);

    /* If enable read rate control, please set one or more groups here: 
     * - Replace <READ_RATE_VALUE> to real value 
     * - And uncomment code
     */
    //out32(RCAR_GEN4_RTDMAC0_COM_BASE + RCAR_DMAC_RATE_RD, BIT(31) | <READ_RATE_VALUE>);
    //out32(RCAR_GEN4_RTDMAC1_COM_BASE + RCAR_DMAC_RATE_RD, BIT(31) | <READ_RATE_VALUE>);
    //out32(RCAR_GEN4_RTDMAC2_COM_BASE + RCAR_DMAC_RATE_RD, BIT(31) | <READ_RATE_VALUE>);
    //out32(RCAR_GEN4_RTDMAC3_COM_BASE + RCAR_DMAC_RATE_RD, BIT(31) | <READ_RATE_VALUE>);

    /* If enable write rate control, please set one or more group here: 
     * - Replace <WRITE_RATE_VALUE> to real value 
     * - And uncomment code
     */
    //out32(RCAR_GEN4_RTDMAC0_COM_BASE + RCAR_DMAC_RATE_WR, BIT(31) | <WRITE_RATE_VALUE>);
    //out32(RCAR_GEN4_RTDMAC1_COM_BASE + RCAR_DMAC_RATE_WR, BIT(31) | <WRITE_RATE_VALUE>);
    //out32(RCAR_GEN4_RTDMAC2_COM_BASE + RCAR_DMAC_RATE_WR, BIT(31) | <WRITE_RATE_VALUE>);
    //out32(RCAR_GEN4_RTDMAC3_COM_BASE + RCAR_DMAC_RATE_WR, BIT(31) | <WRITE_RATE_VALUE>);
}
