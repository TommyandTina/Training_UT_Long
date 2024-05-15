/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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
 
#ifndef _GPIODRV_H_
#define _GPIODRV_H_

#include <stdint.h>
#include <sys/siginfo.h>
#include <devctl.h>

/**
 * @brief GPIO port structure
 */
typedef struct{
    uint32_t	port;  	    //Selete port pin, set the GPSR to choose function
#define GPIO_OUT_LOW				0	//write low
#define GPIO_OUT_HIGH				1	//write high
    uint32_t	value;      //Value on port
#define GPIO_PUD_DIS				1	//pull-disable
#define GPIO_PUD_UP				    2	//pull-up
#define GPIO_PUD_DOWN				3	//pull-down
    uint32_t	pud;        //Select the PUD to choose Pull-UP or Pull-DOWN
#define GPIO_POSNEG_POSITIVE		0	//Positive logic
#define GPIO_POSNEG_NEGATIVE		1	//Negative logic
    uint32_t	posneg;     //Select the positive (not inverted) or negative (inverted) logic for processing the input signals in POSNEG.
#define GPIO_IOINT_INOUT			0	//inout mode
#define GPIO_IOINT_INTERRUPT		1	//interrupt mode
    uint32_t	ioint; 	    //Select general IO/Interrupt mode in IOINTSEL.
#define GPIO_INOUT_IN				0	//input mode
#define GPIO_INOUT_OUT				1	//output mode
    uint32_t	inout; 	    //Select IN/OUT value on port
    /* For interrupt only */
#define GPIO_EDGLEV_LEVEL			0	//level
#define GPIO_EDGLEV_EDGE			1	//edge
    uint32_t	edglevel;   //Edge/level Select
#define GPIO_BOTHEDGE_ONE			0	//edge
#define GPIO_BOTHEDGE_BOTH			1	//level
    uint32_t	bothedge;   //One Edge/Both Edge Select 
} gpio_cmd_t;


/*
 * @brief IO command
 */
#define _DCMD_GPIO   _DCMD_MISC
#define IOCTL_GPIO_SET_PORT          __DIOTF (_DCMD_GPIO, 1,  gpio_cmd_t)
#define IOCTL_GPIO_SET_EDGLEVEL      __DIOTF (_DCMD_GPIO, 2,  gpio_cmd_t)
#define IOCTL_GPIO_SET_IOINT      	 __DIOTF (_DCMD_GPIO, 3,  gpio_cmd_t)
#define IOCTL_GPIO_SET_INOUT         __DIOTF (_DCMD_GPIO, 7,  gpio_cmd_t)
#define IOCTL_GPIO_SET_INEN          __DIOTF (_DCMD_GPIO, 8,  gpio_cmd_t)
#define IOCTL_GPIO_SET_PUD           __DIOTF (_DCMD_GPIO, 9,  gpio_cmd_t)
#define IOCTL_GPIO_SET_POSNEG      	 __DIOTF (_DCMD_GPIO, 10, gpio_cmd_t)
#define IOCTL_GPIO_WRITE    	     __DIOTF (_DCMD_GPIO, 11, gpio_cmd_t)
#define IOCTL_GPIO_READ              __DIOTF (_DCMD_GPIO, 12, gpio_cmd_t)
#define IOCTL_GPIO_INIT              __DIOTF (_DCMD_GPIO, 13, gpio_cmd_t)


#endif // _GPIODRV_H_

