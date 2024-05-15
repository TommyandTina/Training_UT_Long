/*
* $QNXLicenseC:
* Copyright 2020, QNX Software Systems.
* Copyright 2021, Renesas Electronics Corporation.
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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <devctl.h>

#include <hw/pwm.h>

int main(int argc, char **argv)
{
	/* pmw descriptor */
	int fd = -1;
	int32_t cmd_ret;
	pwm_config_t config;
	
	/* Open pmw driver */
    fd = open("/dev/pwm", O_RDWR);
    if (fd ==-1){
        fprintf(stderr, "Open pwm driver failed errno %s\r\n",strerror(errno));
    }
	
	config.period_ns = 10000;
	config.duty_ns = 5000;
	
	devctl(fd, DCMD_PWM_SETPERIOD, (void *)&config, sizeof(config), &cmd_ret);
	
	devctl(fd, DCMD_PWM_ENABLEOUTPUT, NULL, 0, &cmd_ret);
	
	devctl(fd, DCMD_PWM_DISABLEOUTPUT, NULL, 0, &cmd_ret);

	close(fd);
	return EXIT_SUCCESS;
}


 
