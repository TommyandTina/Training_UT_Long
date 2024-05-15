/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include "thermal.h"

const char	*devname = DEV_NAME;

/**********************************************************************************
* Outline      : Print Usage options and exit
* Function Name: usage
* Description  :
* Arguments    : char*  pcPrognam
* Return Value : void
* Note         :
***********************************************************************************/
//Print Usage options and exit
void usage(char* pcProgname)
{
    printf( "Usage: %s [options]\n"
            "\t-h       Prints this info\n"
            "\n",
            pcProgname);
}

/**********************************************************************************
* Outline      : Get temperature and operate voltage of all tsc available
* Function Name: main
* Description  :
* Arguments    : int    argc
*                char   *argv[]
* Return Value :
*                EOK
* Note         :
***********************************************************************************/
int main(int argc, char *argv[])
{
    int fd;
    int status;
    int c;
    int i;
    thermal_packet_t data;
#if 0
    while ((c = getopt(argc, argv, "h")) != -1)
    {
        switch (c)
        {
            case 'h':
                usage(argv[0]);
                exit(1);
                break;
            default:
                usage(argv[0]);
                exit(-1);
                break;
        }
    }
#endif
        status = system("rcar-thermal &");

        if (status == -1) {
            printf("rcar-thermal & result NG");
            return -1;
        }
    delay(500);

    printf("\n");
    printf("************************************\n");
    printf("*   Thermal test start             *\n");
    printf("************************************\n");

    fd = open(devname, O_RDWR);
	if (fd < 0)
	{
		fprintf(stderr, "open(%s) result NG errno=%d - %s\n", devname, errno, strerror(errno));
		return -1;
	}

	/* Test getting temperature from thermal */
	printf("1. Get LSI temperature\n");
	memset((void *)&data, 0, sizeof(thermal_packet_t));
	status = devctl( fd, DCMD_THSCIVM_GET_TEMPERATURE, &data, sizeof(data), NULL);
	if (status != EOK)
	{
		errno = status;
		perror("devctl(DCMD_THSCIVM_GET_TEMPERATURE) result NG");
        close(fd);
		return -1;
	}
	else
	{
        for (i = data.tsc_min; i <= data.tsc_max; i++) {
            printf("   TSC%d temperature = %doC\n", i, data.tsc_val[i]);
        }
	}

	/* Test getting voltage from thermal */
     printf("2. Get LSI core voltage \n");
     memset((void *)&data, 0, sizeof(thermal_packet_t));
     status = devctl( fd, DCMD_THSCIVM_GET_VOLTAGE, &data, sizeof(data), NULL);
     if (status != EOK)
     {
        if(status == EOPNOTSUPP) {
	        printf("  Operation not supported \n");
        } else {
            errno = status;
		    perror("devctl(DCMD_THSCIVM_GET_VOLTAGE) result NG");
            close(fd);
		    return -1;
        }
	 }
	 else
	 {
         for (i = data.tsc_min; i <= data.tsc_max; i++) {
            printf("   TSC%d voltage = %dmV\n", i, data.tsc_val[i]);
        }
	 }

    printf("Test is done\n");
	close(fd);

    return 0;
}
