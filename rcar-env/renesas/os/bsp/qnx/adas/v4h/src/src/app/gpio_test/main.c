/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <hw/gpio.h>
#include <stdint.h>

#define MODE_PIN_IN  1
#define MODE_PIN_OUT 2
#define MODE_PIN_INT 3

#define TEST_PULSE_CODE (_PULSE_CODE_MINAVAIL + 10)
/**
 * @brief GPIO device structure
 */
typedef struct {
    char    *gpio_device;
    int     port;
    int     value;
    int 	mode;
} str_info_t;

struct sigaction act;
int gpio_iid = -1;
int gpio_irq;
int chid;

volatile int test_done = 0;

void gpio_test_shutdown(int signo)
{
    test_done = 1;
}

/** 
 * @brief gpio_intr_thread - A thread is used in Interrupt test.
 * @param data: No use
 * @return NULL when terminate the thread
 */
void *gpio_intr_thread(void *data) {
    struct _pulse   pulse;
    iov_t           iov;
    int	            rcvid;

    SETIOV(&iov, &pulse, sizeof(pulse));
    memset(&pulse, 0, sizeof( pulse));

    fprintf(stdout, "Press any key on the board or press Ctrl-C to stop the test\n");

    while (!test_done)  {
        if ((rcvid = MsgReceivePulse(chid, &pulse, sizeof(pulse), NULL)) == -1)
            continue;

        switch (pulse.code) {
            case TEST_PULSE_CODE:
                printf("Receving an interrupt\n");
                InterruptUnmask(gpio_irq, gpio_iid);
                break;
            default:
                if (rcvid)
                    MsgReplyv(rcvid, ENOTSUP, &iov, 1);
                break;
        }
    }

    return (NULL);
}

/** 
 * @brief parse_command_line - Parse command line when start the GPIO test.
 * @param str_info: Store options for processing.
 * @param argc:
 * @param argv: 
 * @return EOK - No error
 */
void parse_command_line (str_info_t *str_info, gpio_cmd_t *gpio_cmd, int argc, char *argv[])
{ 
    int opt = 0, i = 0;
    char    *value;
    char    *opts[] = {"device","port","value","mode", NULL};

    memset(str_info, 0, sizeof(str_info_t));
    str_info->gpio_device = "/dev/gpio0";
    str_info->port = 0;
    str_info->value = 0;
    str_info->mode = GPIO_INOUT_IN;

    memset(gpio_cmd, 0, sizeof(gpio_cmd_t));
    gpio_cmd->posneg = GPIO_POSNEG_POSITIVE;
    gpio_cmd->edglevel = GPIO_EDGLEV_EDGE;
    gpio_cmd->bothedge = GPIO_BOTHEDGE_BOTH;
    gpio_cmd->pud = GPIO_PUD_UP;

    while (argv[i] != NULL && argv[0] != 0) {

        switch ((opt = getsubopt (&argv[i], opts, &value))) {
            case 0: //device
                if (value != NULL) {
                    str_info->gpio_device = value;
                }
                break;
            case 1: //port pin
                if (value != NULL) {
                    str_info->port = atoi(value);
                }
                break;
            case 2: //value
                if (value != NULL) {
                    str_info->value = atoi(value);
                }
                break;
            case 3: //mode
                if (value != NULL) {
                    str_info->mode = (strncmp(value, "int",3)==0) ? MODE_PIN_INT : ((strncmp(value, "out",3)==0) ? MODE_PIN_OUT : MODE_PIN_IN);
                    value += (str_info->mode == MODE_PIN_IN) ? 2 : 3;
                    if (str_info->mode == MODE_PIN_INT) gpio_cmd->posneg = GPIO_POSNEG_NEGATIVE;
                    while (value && *value !='\0') {
                        if(strncmp(value, ":pos", 4) == 0) {
                            gpio_cmd->posneg = GPIO_POSNEG_POSITIVE;
                            value += 4;
                        } else if(strncmp(value, ":neg", 4) == 0) {
                            gpio_cmd->posneg = GPIO_POSNEG_NEGATIVE;
                            value += 4;
                        } else if(strncmp(value, ":edg", 4) == 0) {
                            gpio_cmd->edglevel = GPIO_EDGLEV_EDGE;
                            value += 4;
                        } else if(strncmp(value, ":lev", 4) == 0) {
                            gpio_cmd->edglevel = GPIO_EDGLEV_LEVEL;
                            value += 4;
                        } else if(strncmp(value, ":one", 4) == 0) {
                            gpio_cmd->bothedge = GPIO_BOTHEDGE_ONE;
                            value += 4;
                        } else if(strncmp(value, ":bot", 4) == 0) {
                            gpio_cmd->bothedge = GPIO_BOTHEDGE_BOTH;
                            value += 4;
                        } else if(strncmp(value, ":pds", 4) == 0) {
                            gpio_cmd->pud = GPIO_PUD_DIS;
                            value += 4;
                        } else if(strncmp(value, ":pup", 4) == 0) {
                            gpio_cmd->pud = GPIO_PUD_UP;
                            value += 4;
                        } else if(strncmp(value, ":pdn", 4) == 0) {
                            gpio_cmd->pud = GPIO_PUD_DOWN;
                            value += 4;
                        } else {
                            fprintf(stderr, "Unknow option(%s), use default values\n", value);
                            break;
                        }
                    }
                }
                break;
            default:
                break;
        };
        i++;
    }
}


/** 
 * @brief main - Main function.
 * @return EOK - No error
 */
int main(int argc, char *argv[])
{
    int fd;
    int status;
    str_info_t	gpio_info;
    gpio_cmd_t  gpio_cmd;
    int 		index;

    parse_command_line(&gpio_info, &gpio_cmd, argc, argv);

    fd = open(gpio_info.gpio_device, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open(%s): %s\n", gpio_info.gpio_device, strerror(errno));
        exit(-1);
    }

    index = atoi (&gpio_info.gpio_device [strlen("/dev/gpio")]);

    //Initialize values for Read/Write on port pins
    gpio_cmd.port 		= gpio_info.port;
    if (gpio_info.value) {
        gpio_cmd.value 	= GPIO_OUT_HIGH;
    } else {
        gpio_cmd.value 	= GPIO_OUT_LOW;
    }

    if (gpio_info.mode == MODE_PIN_INT) {
        //gpio_cmd.posneg = GPIO_POSNEG_NEGATIVE;
        gpio_cmd.ioint	= GPIO_IOINT_INTERRUPT;
        //gpio_cmd.edglevel = GPIO_EDGLEV_EDGE;
        //gpio_cmd.bothedge = GPIO_BOTHEDGE_BOTH;
    } else {
        //gpio_cmd.posneg = GPIO_POSNEG_POSITIVE;
        gpio_cmd.ioint	= GPIO_IOINT_INOUT;
        if (gpio_info.mode == MODE_PIN_IN) {
            gpio_cmd.inout 	= GPIO_INOUT_IN;
        } else {
            gpio_cmd.inout 	= GPIO_INOUT_OUT;
        }
    }

    if (gpio_cmd.ioint	== GPIO_IOINT_INOUT) {
        // Configure GPIO to read/write
        status = devctl(fd, IOCTL_GPIO_CONFIG, &gpio_cmd, sizeof(gpio_cmd_t), NULL);
        if (status != EOK) {
            errno = status;
            perror("IOCTL_GPIO_CONFIG");
            close(fd);
            exit(-1);
        }

        // Read/Write value from and to port pin
        if (gpio_cmd.inout == GPIO_INOUT_OUT) {
            printf ("Write GP%d_%d to %d\n", index, gpio_info.port, gpio_cmd.value == GPIO_OUT_LOW ? 0 : 1);
        } else {
            printf ("Read GP%d_%d status: %d\n", index, gpio_info.port, gpio_cmd.value == GPIO_OUT_LOW ? 0 : 1);
        }
    } else {
        struct sigevent event;
        pthread_t       thread_id; 
        int             coid;

        //Get interrupt number
        status = devctl(fd, IOCTL_GPIO_GET_IRQ_NUM, &gpio_cmd, sizeof(gpio_cmd_t), NULL);
        if (status != EOK) {
            errno = status;
            perror("IOCTL_GPIO_GET_IRQ_NUM");
            close(fd);
            exit(-1);
        }

        /* Setup for exit */
        act.sa_handler = gpio_test_shutdown;
        sigaction(SIGHUP, &act, NULL);
        sigaction(SIGINT, &act, NULL);
        sigaction(SIGQUIT, &act, NULL);
        sigaction(SIGTERM, &act, NULL);

        ThreadCtl( _NTO_TCTL_IO, 0 );

        if ((status = pthread_create( &thread_id, NULL, gpio_intr_thread, NULL))) {
            errno = status;
            perror("pthread_create");
            close(fd);
            exit(-1);
        }

        chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK);
        event.sigev_notify = SIGEV_PULSE;
        event.sigev_coid = coid = ConnectAttach(0, 0, chid, _NTO_SIDE_CHANNEL, 0);
        event.sigev_code = TEST_PULSE_CODE;
        event.sigev_priority = 10;

        gpio_irq = gpio_cmd.value;
        if ((gpio_iid = InterruptAttachEvent(gpio_irq, &event, _NTO_INTR_FLAGS_PROCESS)) == -1) {
            perror("InterruptAttachEvent");
            close(fd);
            exit(-1);
        }

        // Configure GPIO pin to interrupt mode
        status = devctl(fd, IOCTL_GPIO_CONFIG, &gpio_cmd, sizeof(gpio_cmd_t), NULL);
        if (status != EOK) {
            errno = status;
            perror("IOCTL_GPIO_CONFIG");
            close(fd);
            exit(-1);
        }

        while (!test_done) {
            delay(1);
        }

        InterruptDetach(gpio_iid);
        ConnectDetach(coid);
        ChannelDestroy(chid);
    }

    close(fd);
    return status;
}
