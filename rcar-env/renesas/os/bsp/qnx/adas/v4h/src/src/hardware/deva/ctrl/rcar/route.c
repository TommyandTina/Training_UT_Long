/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include <unistd.h>
#include <string.h>
#include <hw/gpio.h>
#include "rcar.h"

int rcar_gpio_route(rcar_context_t * rcar, uint32_t chan) 
{
    int fd;
    int ret = EOK;
    gpio_cmd_t  gpio_cmd;

    if (rcar->route_name == NULL) {
        /* Not support a route */
        return EOK;
    }

    fd = open(rcar->route_name, O_RDWR);
    if (fd < 0){
        if(chan == ADO_PCM_CHANNEL_PLAYBACK) {
            ado_error_fmt("Error in open(%s) to switch to playback route (%s)\n", rcar->route_name, strerror(errno));
        } else {
            ado_error_fmt("Error in open(%s) to switch to capture route (%s)\n", rcar->route_name, strerror(errno));
        }
        return EIO;
    }

    if(chan == ADO_PCM_CHANNEL_PLAYBACK) {
        gpio_cmd.value  = GPIO_OUT_LOW;
    } else {
        gpio_cmd.value  = GPIO_OUT_HIGH;
    }
    gpio_cmd.port   = rcar->route_port;
    gpio_cmd.ioint  = GPIO_IOINT_INOUT;
    gpio_cmd.inout  = GPIO_INOUT_OUT;

    ret = devctl(fd, IOCTL_GPIO_CONFIG, &gpio_cmd, sizeof(gpio_cmd_t), NULL);
    if (ret != EOK) {
        if(chan == ADO_PCM_CHANNEL_PLAYBACK) {
            ado_error_fmt("Unable to switch to playback route\n");
        } else {
            ado_error_fmt("Unable to switch to capture route\n");
        }
        ret = EIO;
    }

    close(fd);
    return ret;
}
