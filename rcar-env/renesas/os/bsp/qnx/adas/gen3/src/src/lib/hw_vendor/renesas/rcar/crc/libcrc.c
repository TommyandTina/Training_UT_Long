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

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <sys/neutrino.h>

#include <hw/crc.h>
#include "gen3/rcar_wcrc.h"
#include "gen3/rcar_crc.h"
#include "gen3/rcar_kcrc.h"
#include "gen3/rcar_crc_common.h"

/***********************************************
 *	MACROS/DEFINES
 ***********************************************/
 
 
/***********************************************
 *	GLOBAL VARIABLE
 ***********************************************/
CRC_DEVICE_Type *crc_dev = NULL;

/***********************************************
 *	APIs IMPLEMENATION
 ************************************************/

/*!
 *	@brief		Start allocate resource for CRC module.
 *  @param[in]  CRC_Type* base
 *  @param[in]  const crc_config_t* config
 *	@return		0: success.
 *              -1: failed.
 *			    crc_dev variable is initialized
 */
int CRC_Init(CRC_Type* base, const crc_config_t* config)
{
    int rc = 0;

    crc_dev = malloc(sizeof(CRC_DEVICE_Type));
    if (crc_dev == NULL) {
        fprintf(stderr, "%s: allocate memory for crc device is failed\n", __func__);
        rc = -1;
        return rc;
    }

    switch (base->crc_base) {
        case RCAR_GEN3_CRC0_BASE:
            crc_dev->crc_base                     = base->crc_base;
            crc_dev->wcrc_base                    = RCAR_GEN3_WCRC0_BASE;
            crc_dev->wcrc_fifo_data_port          = RCAR_GEN3_WCRC0_FIFO_BASE + CRC_FIFO_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_command_port       = RCAR_GEN3_WCRC0_FIFO_BASE + CRC_FIFO_COMMAND_PORT_OFFSET;
            crc_dev->wcrc_fifo_expected_data_port = RCAR_GEN3_WCRC0_FIFO_BASE + CRC_FIFO_EXPECTED_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_result_port        = RCAR_GEN3_WCRC0_FIFO_BASE + CRC_FIFO_RESULT_PORT_OFFSET;
            break;
        case RCAR_GEN3_CRC1_BASE:
            crc_dev->crc_base                     = base->crc_base;
            crc_dev->wcrc_base                    = RCAR_GEN3_WCRC1_BASE;
            crc_dev->wcrc_fifo_data_port          = RCAR_GEN3_WCRC1_FIFO_BASE + CRC_FIFO_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_command_port       = RCAR_GEN3_WCRC1_FIFO_BASE + CRC_FIFO_COMMAND_PORT_OFFSET;
            crc_dev->wcrc_fifo_expected_data_port = RCAR_GEN3_WCRC1_FIFO_BASE + CRC_FIFO_EXPECTED_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_result_port        = RCAR_GEN3_WCRC1_FIFO_BASE + CRC_FIFO_RESULT_PORT_OFFSET;
            break;
         case RCAR_GEN3_CRC2_BASE:
            crc_dev->crc_base                     = base->crc_base;
            crc_dev->wcrc_base                    = RCAR_GEN3_WCRC2_BASE;
            crc_dev->wcrc_fifo_data_port          = RCAR_GEN3_WCRC2_FIFO_BASE + CRC_FIFO_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_command_port       = RCAR_GEN3_WCRC2_FIFO_BASE + CRC_FIFO_COMMAND_PORT_OFFSET;
            crc_dev->wcrc_fifo_expected_data_port = RCAR_GEN3_WCRC2_FIFO_BASE + CRC_FIFO_EXPECTED_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_result_port        = RCAR_GEN3_WCRC2_FIFO_BASE + CRC_FIFO_RESULT_PORT_OFFSET;
            break;
         case RCAR_GEN3_CRC3_BASE:
            crc_dev->crc_base                     = base->crc_base;
            crc_dev->wcrc_base                    = RCAR_GEN3_WCRC3_BASE;
            crc_dev->wcrc_fifo_data_port          = RCAR_GEN3_WCRC3_FIFO_BASE + CRC_FIFO_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_command_port       = RCAR_GEN3_WCRC3_FIFO_BASE + CRC_FIFO_COMMAND_PORT_OFFSET;
            crc_dev->wcrc_fifo_expected_data_port = RCAR_GEN3_WCRC3_FIFO_BASE + CRC_FIFO_EXPECTED_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_result_port        = RCAR_GEN3_WCRC3_FIFO_BASE + CRC_FIFO_RESULT_PORT_OFFSET;
            break;
         case RCAR_GEN3_KCRC0_BASE:
            crc_dev->kcrc_base                    = base->crc_base;
            crc_dev->wcrc_base                    = RCAR_GEN3_WCRC0_BASE;
            crc_dev->wcrc_fifo_data_port          = RCAR_GEN3_WCRC0_FIFO_BASE + KCRC_FIFO_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_command_port       = RCAR_GEN3_WCRC0_FIFO_BASE + KCRC_FIFO_COMMAND_PORT_OFFSET;
            crc_dev->wcrc_fifo_expected_data_port = RCAR_GEN3_WCRC0_FIFO_BASE + KCRC_FIFO_EXPECTED_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_result_port        = RCAR_GEN3_WCRC0_FIFO_BASE + KCRC_FIFO_RESULT_PORT_OFFSET;
            break;
         case RCAR_GEN3_KCRC1_BASE:
            crc_dev->kcrc_base                    = base->crc_base;
            crc_dev->wcrc_base                    = RCAR_GEN3_WCRC1_BASE;
            crc_dev->wcrc_fifo_data_port          = RCAR_GEN3_WCRC1_FIFO_BASE + KCRC_FIFO_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_command_port       = RCAR_GEN3_WCRC1_FIFO_BASE + KCRC_FIFO_COMMAND_PORT_OFFSET;
            crc_dev->wcrc_fifo_expected_data_port = RCAR_GEN3_WCRC1_FIFO_BASE + KCRC_FIFO_EXPECTED_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_result_port        = RCAR_GEN3_WCRC1_FIFO_BASE + KCRC_FIFO_RESULT_PORT_OFFSET;
            break;
         case RCAR_GEN3_KCRC2_BASE:
            crc_dev->kcrc_base                     = base->crc_base;
            crc_dev->wcrc_base                    = RCAR_GEN3_WCRC2_BASE;
            crc_dev->wcrc_fifo_data_port          = RCAR_GEN3_WCRC2_FIFO_BASE + KCRC_FIFO_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_command_port       = RCAR_GEN3_WCRC2_FIFO_BASE + KCRC_FIFO_COMMAND_PORT_OFFSET;
            crc_dev->wcrc_fifo_expected_data_port = RCAR_GEN3_WCRC2_FIFO_BASE + KCRC_FIFO_EXPECTED_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_result_port        = RCAR_GEN3_WCRC2_FIFO_BASE + KCRC_FIFO_RESULT_PORT_OFFSET;
            break;
         case RCAR_GEN3_KCRC3_BASE:
            crc_dev->kcrc_base                    = base->crc_base;
            crc_dev->wcrc_base                    = RCAR_GEN3_WCRC3_BASE;
            crc_dev->wcrc_fifo_data_port          = RCAR_GEN3_WCRC3_FIFO_BASE + KCRC_FIFO_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_command_port       = RCAR_GEN3_WCRC3_FIFO_BASE + KCRC_FIFO_COMMAND_PORT_OFFSET;
            crc_dev->wcrc_fifo_expected_data_port = RCAR_GEN3_WCRC3_FIFO_BASE + KCRC_FIFO_EXPECTED_DATA_PORT_OFFSET;
            crc_dev->wcrc_fifo_result_port        = RCAR_GEN3_WCRC3_FIFO_BASE + KCRC_FIFO_RESULT_PORT_OFFSET;
            break;
         default:
            break;
    }

    switch (base->crc_base) {
        case RCAR_GEN3_CRC0_BASE:
        case RCAR_GEN3_CRC1_BASE:
        case RCAR_GEN3_CRC2_BASE:
        case RCAR_GEN3_CRC3_BASE:
            rc = RCAR_CRC_Open (crc_dev); 
            break;
            
        case RCAR_GEN3_KCRC0_BASE:
        case RCAR_GEN3_KCRC1_BASE:
        case RCAR_GEN3_KCRC2_BASE:
        case RCAR_GEN3_KCRC3_BASE:
            rc = RCAR_KCRC_Open (crc_dev); 
            break;
        default:
            fprintf(stderr, "%s: CRC module does not support device with base = 0x%x \r\n", __func__, base->crc_base);
            rc = -1;
    }
    
    return rc;
}

/*!
 *	@brief		Get configuration of current device
 *  @param[in]  CRC_Type* base
 *  @param[out] crc_config_t* config
 *	@return		0: success.
 *              -1: failed.
 */
int CRC_GetConfig(CRC_Type* base, crc_config_t* config)
{
    int rc = 0;

    if (crc_dev == NULL) {
       fprintf(stderr, "%s: allocate memory for crc device is failed\n", __func__);
       rc = -1;
       return rc;
    }
    
    switch (base->crc_base) {
        case RCAR_GEN3_CRC0_BASE:
        case RCAR_GEN3_CRC1_BASE:
        case RCAR_GEN3_CRC2_BASE:
        case RCAR_GEN3_CRC3_BASE:
            rc = RCAR_CRC_GetConfig (crc_dev, config); 
            break;
            
        case RCAR_GEN3_KCRC0_BASE:
        case RCAR_GEN3_KCRC1_BASE:
        case RCAR_GEN3_KCRC2_BASE:
        case RCAR_GEN3_KCRC3_BASE:
            rc = RCAR_KCRC_GetConfig (crc_dev, config);  
            break;
        default:
            fprintf(stderr, "%s: CRC module does not support device with base = 0x%x \r\n", __func__, base->crc_base);
            rc = -1;
    }
    
    return rc;
}


/*!
 *	@brief		Set configuration info to device
 *  @param[in]  CRC_Type* base
 *  @param[in]  crc_config_t* config
 *	@return		0: success.
 *              -1: failed.
 */
int CRC_SetConfig(CRC_Type* base, crc_config_t* config)
{
    int rc = 0;

    if (crc_dev == NULL) {
        fprintf(stderr, "%s: CRC module isn't initialized \r\n", __func__);
        rc = -1;
        return rc;
    }

    crc_dev->crc_mode = base->crc_mode;
    switch (base->crc_base) {
        case RCAR_GEN3_CRC0_BASE:
        case RCAR_GEN3_CRC1_BASE:
        case RCAR_GEN3_CRC2_BASE:
        case RCAR_GEN3_CRC3_BASE:
            rc = RCAR_CRC_SetConfig(crc_dev, config); 
            break;

        case RCAR_GEN3_KCRC0_BASE:
        case RCAR_GEN3_KCRC1_BASE:
        case RCAR_GEN3_KCRC2_BASE:
        case RCAR_GEN3_KCRC3_BASE:
            rc = RCAR_KCRC_SetConfig(crc_dev, config); 
            break;
            
        default:
            fprintf(stderr, "%s: CRC module do not support crc base = 0x%x \r\n", __func__, base->crc_base);
            rc = -1;
    }

    return rc;
}


/*!
 *	@brief		Write data to register or FIFO
 *  @param[in]  CRC_Type* base
 *  @param[in]  const uint8_t* data
 *  @param[in]  size_t dataSize
 *	@return		0: success.
 *              -1: failed.
 */
int CRC_WriteData(CRC_Type* base, const uint8_t* data,size_t dataSize)
{
    int rc = 0;

    if (crc_dev == NULL) {
        fprintf(stderr, "%s: CRC module isn't initialized \r\n", __func__);
        rc = -1;
        return rc;
    }

     switch (base->crc_base) {
        case RCAR_GEN3_CRC0_BASE:
        case RCAR_GEN3_CRC1_BASE:
        case RCAR_GEN3_CRC2_BASE:
        case RCAR_GEN3_CRC3_BASE:
            rc = RCAR_CRC_WriteData(crc_dev, data, dataSize); 
            break;

        case RCAR_GEN3_KCRC0_BASE:
        case RCAR_GEN3_KCRC1_BASE:
        case RCAR_GEN3_KCRC2_BASE:
        case RCAR_GEN3_KCRC3_BASE:
            rc = RCAR_KCRC_WriteData(crc_dev, data, dataSize); 
            break;
            
        default:
            fprintf(stderr, "%s: CRC module do not support crc base = 0x%x \r\n", __func__, base->crc_base);
            rc = -1;
            
           
    }
    return rc;
}

/*!
 *	@brief		Get result in from CRC calculator
 *  @param[in]  CRC_Type* base
 *	@return		0: success.
 *	            -1: failed.
 */
uint32_t CRC_GetResult(CRC_Type* base)
{
    int rc = 0;

    if (crc_dev == NULL) {
        fprintf(stderr, "%s: CRC module isn't initialized \r\n", __func__);
        rc = -1;
        return rc;
    }   

    switch (base->crc_base) {
        case RCAR_GEN3_CRC0_BASE:
        case RCAR_GEN3_CRC1_BASE:
        case RCAR_GEN3_CRC2_BASE:
        case RCAR_GEN3_CRC3_BASE:
            rc = RCAR_CRC_GetResult(crc_dev);
            break;

        case RCAR_GEN3_KCRC0_BASE:
        case RCAR_GEN3_KCRC1_BASE:
        case RCAR_GEN3_KCRC2_BASE:
        case RCAR_GEN3_KCRC3_BASE:
            rc = RCAR_KCRC_GetResult(crc_dev); 
            break;
            
        default:
            fprintf(stderr, "%s: CRC module do not support crc base = 0x%x \r\n", __func__, base->crc_base);
            rc = -1;
    }
    return rc;
 
}


/*!
 *	@brief		Free memory related to crc module 
 *  @param[in]  CRC_Type* base
 *	@return		0: success.
 *	            -1: failed.
 */
int CRC_Deinit(CRC_Type* base)
{
    int rc = 0;

    if (crc_dev == NULL) {
        fprintf(stderr, "%s: CRC module isn't initialized \r\n", __func__);
        rc = -1;
        return rc;
    }

     switch (base->crc_base) {
        case RCAR_GEN3_CRC0_BASE:
        case RCAR_GEN3_CRC1_BASE:
        case RCAR_GEN3_CRC2_BASE:
        case RCAR_GEN3_CRC3_BASE:
            rc = RCAR_CRC_Close (crc_dev); 
            break;

        case RCAR_GEN3_KCRC0_BASE:
        case RCAR_GEN3_KCRC1_BASE:
        case RCAR_GEN3_KCRC2_BASE:
        case RCAR_GEN3_KCRC3_BASE:
            rc = RCAR_KCRC_Close (crc_dev); 
            break;
            
        default:
            fprintf(stderr, "%s: CRC module do not support crc base = 0x%x \r\n", __func__, base->crc_base);
            rc = -1;
    }
    return rc;
}
