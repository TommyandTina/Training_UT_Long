/*
* $QNXLicenseC:
* Copyright 2022, QNX Software Systems.
* Copyright 2022, Renesas Electronics Corporation.
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <devctl.h>

#include <hw/rcar_pwm.h>

int duty = 50;
int terminate = 0;
struct timespec tp = { 0 };

pwm_config_t config = { 
    .oneshot = false, 
    .channel = 0,
    .period_ns = 1000000000,
    .duty_ns = 500000000
};

void signal_handler(int signum)
{
    switch(signum) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGKILL:
        case SIGSEGV:
            terminate = 1;
            break;
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    int fd, rc, opt, index;

    static struct option options[] = {
        { "channel",    required_argument,  0, 'c' },
        { "cycle_sec",  required_argument,  0, 's' },
        { "cycle_nsec", required_argument,  0, 'n' },
        { "duty",       required_argument,  0, 'd' },
        { "oneshot",    no_argument,        0, 'o' },
        { NULL,         0,                  0,  0  }
    };

    while((opt = getopt_long(argc, argv,"c:s:n:d:o", options, &index )) != -1) {
        switch (opt) {
            case 'c':
                config.channel = atoi(optarg);
                break;
            case 's':
                tp.tv_sec = strtoul(optarg, NULL, 10);
                break;
            case 'n':
                tp.tv_nsec = strtoull(optarg, NULL, 10);
                break;
            case 'd':
                if((duty = atoi(optarg)) > 100) {
                    fprintf(stderr, "Invalid duty cycle\n");
                    return -1;
                }
                break;
            case 'o':
                config.oneshot = true;
                break;
            default:
                fprintf(stderr, "Unknown option\n");
                return -1;
        }
    }

    signal(SIGINT,  signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGSEGV, signal_handler);

    config.period_ns = (uint64_t)tp.tv_sec * 1000000000L + tp.tv_nsec;
    config.duty_ns = (config.period_ns * duty) / 100;

    if((fd = open(PWM_DEVICE, O_RDWR)) < 0) {
        fprintf(stderr, "Unable to open %s (%s)\n", PWM_DEVICE, strerror(errno));
        return -1;
    }

    if((rc = devctl(fd, DCMD_PWM_ATTACH, &config.channel, sizeof(int), NULL) != EOK)) {
        fprintf(stderr, "DCMD_PWM_ATTACH() failed (%s)\n", strerror(rc));
        goto cleanup;
    }

    if((rc = devctl(fd, DCMD_PWM_CONFIGURE, &config, sizeof(pwm_config_t), NULL) != EOK)) {
        fprintf(stderr, "DCMD_PWM_CONFIGURE() failed (%s)\n", strerror(rc));
        goto detach;
    }

    if((rc = devctl(fd, DCMD_PWM_ENABLE, &config.channel, sizeof(int), NULL) != EOK)) {
        fprintf(stderr, "DCMD_PWM_ENABLE() failed (%s)\n", strerror(rc));
        goto detach;
    }

    fprintf(stdout, "PWM output is enabled. Press Ctrl-C to stop PWM and terminate test program\n");

    while(!terminate) {
        delay(100);
    }

    if((rc = devctl(fd, DCMD_PWM_DISABLE, &config.channel, sizeof(int), NULL) != EOK)) {
        fprintf(stderr, "DCMD_PWM_DISABLE() failed (%s)\n", strerror(rc));
        goto detach;
    }

detach:
    if((rc = devctl(fd, DCMD_PWM_DETACH, &config.channel, sizeof(int), NULL) != EOK)) {
        fprintf(stderr, "DCMD_PWM_DETACH() failed (%s)\n", strerror(rc));
    }

cleanup:
    if(fd > 0) {
        close(fd);
    }
    return 0;
}


 
