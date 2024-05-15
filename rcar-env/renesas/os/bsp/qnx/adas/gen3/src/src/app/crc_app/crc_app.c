/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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
 
/****************************************************************************/

/***********************************************
 *	INCLUDE FILES
 ***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <hw/crc.h>

/***********************************************
 *	MACROS/DEFINES
 ***********************************************/
#define OPTSTR "b:m:p:r:R:x:i:d:h"

/***********************************************
 *	GLOBAL VALUE
 ***********************************************/
extern char *optarg;
char *progname = "crc_app";

/***********************************************
 *       SUBROUTINE
 ***********************************************/

/*
 * Print out usage messages
 */
static void usage(const char *progname)
{
    printf("Usage: %s \n \
           -b <CRC base address> /* Hex number:\n \
                                      CRC0: 0xE6F00000  CRC1: 0xE6F10000  CRC2: 0xE7000000  CRC3: 0xE7010000\n \
                                      KCRC0: 0xE7020000  KCRC1: 0xE7030000  KCRC2: 0xE7040000  KCRC3: 0xE7050000*/\n \
           -m <CRC mode>         /* 0: INDEPENDENT_CRC_MODE 1: E2E_CRC_MODE */ \n \
           -p <Polinomial>       /* Hex number */\n \
           -r <Reflected in>     /* 0 or 1 */\n \
           -R <Reflect out>      /* 0 or 1 */\n \
           -x <XOR Value>        /* Hex number: 0x00 or 0xFFFFFFFF */\n \
           -i <Initialize value> /* Hex number */\n \
           -d <string of data>        /* String of hex data */\n\n", progname);
    printf("    Ex: crc_app -b 0xE6F00000 -m 0 -p  0x4C11DB7 -r 0 -R 0 -x 0xFFFFFFFF -i 0xFFFFFFFF -d 0x01 0x02 0x03 0x04\r\n");
}

/***********************************************
 *	MAIN FUNCTION
 ***********************************************/
/*!
 *	@brief		main function
 *	@param[in]	argc - A count of arguments
 *	@param[in]	argv - A pointer of arguments
 *	@return		0=success, 1=error
 */
int main(int argc, char *argv[])
{
    CRC_Type base;
    crc_config_t config;
    uint8_t *data = NULL;
    uint32_t checksum;
    int opt;
    int arg_internal_count = 0;
    uint32_t datasize = 0;
    
    /* Initialize value for RCAR CRC module and configuration parameters */
    base.crc_mode     = E2E_CRC_MODE;
    base.crc_base     = 0xE7020000;
    config.polynomial = 0x04C11DB7;
    config.reflectIn  = true;
    config.reflectOut = true;
    config.outputXORValue = 0xFFFFFFFF;
    config.initValue      = 0xFFFFFFFF;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
    switch(opt) {
       case 'b':
          arg_internal_count =  arg_internal_count + 2;
          base.crc_base = strtoul(optarg, NULL, 16);
          break;
       case 'm':
          arg_internal_count =  arg_internal_count + 2;
          base.crc_mode = atoi(optarg);
          if ( (base.crc_mode != INDEPENDENT_CRC_MODE) && (base.crc_mode != E2E_CRC_MODE) ) {
             printf ("%s: crc mode = %d isn't supported \r\n", progname, base.crc_mode);
             usage(progname);
             return 0;
          }
          break;
       case 'p': // Polynomial
           arg_internal_count =  arg_internal_count + 2;
           config.polynomial = strtoul(optarg, NULL, 16);
           break;
       case 'r':// reflect in
          arg_internal_count =  arg_internal_count + 2;
          if  (atoi(optarg) > 1) {
              config.reflectIn = 1;
          }else{
              config.reflectIn = atoi(optarg);
          }
          break;
       case 'R': // reflect out
          arg_internal_count =  arg_internal_count + 2;
          if  (atoi(optarg) > 1) {
              config.reflectOut = 1;
          }else{
              config.reflectOut = atoi(optarg);
          }
          break;
       case 'i':// initial value
          arg_internal_count =  arg_internal_count + 2;
          config.initValue = strtoul(optarg, NULL, 16);
          break;
       case 'x': // XOR value
          arg_internal_count =  arg_internal_count + 2;
          config.outputXORValue = strtoul(optarg, NULL, 16);
          break;
       case 'd': // string of hex data
          datasize = argc - 2 - arg_internal_count;
          data = malloc(datasize);
          for(int i = 2 + arg_internal_count; i < argc; i++) {
            *(data + i - 2 - arg_internal_count) = strtoul(argv[i], NULL, 16);
          }
          break;
       case 'h':
          usage(progname);
          return 0;
       default:
          break;
       }
    
    if (datasize == 0) {
        
        printf("CAN NOT calculate, Something is wrong in the command !!! \r\n\r\n");
        usage(progname);
        return 0;
    }
    
    printf("RCAR CRC parameters: \r\n");
    printf("    rcar base address = 0x%x \r\n", base.crc_base);
    if (base.crc_mode == 0) {
       printf("    rcar crc mode: INDEPENDENT_CRC_MODE \r\n\r\n");
    } else if (base.crc_mode == 1){
       printf("    rcar crc mode: E2E_CRC_MODE \r\n\r\n");
    }

    printf("CRC calculate with parameters: \r\n");
    printf("    polynomial = 0x%x \r\n", config.polynomial);
    printf("    reflectIn = %d \r\n", config.reflectIn);
    printf("    reflectOut = %d \r\n", config.reflectOut);
    printf("    initValue = 0x%x \r\n", config.initValue);
    printf("    outputXORValue = 0x%x \r\n", config.outputXORValue);
   
    printf("\r\n");
    printf("    Data in: \r\n");
    printf("        ");

    for (int i = 0; i < datasize; i++) {
        if (((i % 32) == 0 ) && (i > 0)) {
           printf("\r\n");
           printf("        ");
        }
        printf(" 0x%02x ", *(data+i));
    }
    printf("\r\n\r\n");


    if (CRC_Init(&base, &config) == -1) {
        fprintf(stderr, "Can't open device!!\n");
        return 1;
    }

    if (CRC_SetConfig(&base, &config) == -1) {
        fprintf(stderr, "Can't configure for CRC!!\n");
        return -1;
    }

    if (CRC_WriteData(&base, data, datasize) == -1) {
        fprintf(stderr, "Can't write to  CRC!!\n");
        goto fail;
    }

    checksum = CRC_GetResult(&base);

    printf("    Checksum = 0x%x \r\n\n", checksum);

fail:
    free(data);
    CRC_Deinit(&base);

    printf("CRC END!!!!! \r\n");
    return 1;
}
