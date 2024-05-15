/*
* $QNXLicenseC:
* Copyright (c) 2018, QNX Software Systems. All Rights Reserved.
* Copyright 2021, Renesas Electronics Corporation.
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
#include <aarch64/r-car-gen3.h>
#include <rcar_board.h>
#include "rcar_soc_board.h"
#include "init_board_type.h"
#include "rcar_startup.h"

rcar_config i2c0_config = {
    .mod = (rcar_module[])
    {
        { .clk.val = ENABLE, .clk.reg = SMSTPCR(9), .clk.bits = BIT(31) },   /* I2C-IF0 */
        { { 0 } }
    },
    .pinmux = (rcar_pinmux[])
    {
        { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(0),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 4,  .ipsr.val = 0 }, /* I2C-IF0 */
        { .gpsr.reg = GPSR(4), .gpsr.bits = BIT(1),  .gpsr.val = GPSR_IP, .ipsr.reg = IPSR(7),  .ipsr.bits = 8,  .ipsr.val = 0 }, /* I2C-IF0 */
        { { 0 } }
    },
};

rcar_config *rcar_get_board_config(size_t *num_devs) 
{
    char* config_name   = NULL;
    rcar_config* config = NULL;

    if (num_devs == NULL) {
        return NULL;
    }

    /*  May need to init I2C0 before pinging to I2C device on the board */
    module_setup(&i2c0_config);

    /* Detect board here. The generic rcar gen3 startup code calls the init_board_type
     * after the init_board call.
     */
    init_board_type();

    if (board_type == BOARD_STARTERKIT) {
        config_name = "v3h_ws10_sk_config";
        config      = v3h_ws10_sk_config;
        *num_devs   = NUM_ELTS(v3h_ws10_sk_config);
    } else if (board_type == BOARD_CONDOR) {
        config_name = "v3h_ws10_condor_config";
        config      = v3h_ws10_condor_config;
        *num_devs   = NUM_ELTS(v3h_ws10_condor_config);
    } else { // Default is Condor-I
        config_name = "v3h_ws20_condor_config";
        config      = v3h_ws20_condor_config;
        *num_devs   = NUM_ELTS(v3h_ws20_condor_config);
    }

    if (debug_flag) {
        kprintf("board_config: %s\n", config_name);
    }

    return config;
}
