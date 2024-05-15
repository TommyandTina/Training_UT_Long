/*
 * $QNXLicenseC:
 * Copyright 2014, 2017 QNX Software Systems.
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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

int main (int argc, char *argv[])
{
    char *boardname = NULL;

#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };

    if (boardname == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        boardname = cs_machine_str;
    }
#endif
    if (argc > 1)
        printf("%s", argv[1]); 

    if (boardname)
        printf(" %s\n", boardname);
    else
        printf("\n");

    return EXIT_SUCCESS;
}
