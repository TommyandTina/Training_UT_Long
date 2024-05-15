/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2020, Renesas Electronics Corporation
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

#include "rcar_kcrc.h"
#include "rcar_wcrc.h"
#include <sys/mman.h>
#include <hw/inout.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <aarch64/r-car-gen3.h>
#include <hw/crc.h>

/***********************************************
 *	MACROS/DEFINES
 ***********************************************/

/***********************************************
 *	GLOBAL VARIABLE
 ***********************************************/
static uint32_t   gdwpolynomial = 0;
static bool       gbreflectIn = false;
static bool       gbreflectOut = false;
static uint32_t   gdwoutputXORValue = 0x00000000;
static uint32_t   gdwinitValue = 0x00000000;


/***********************************************
 *	FUNCTION DECLARATION
 ***********************************************/
extern int rcar_dma_init(CRC_DEVICE_Type* dev);
extern int rcar_crc_dma_xfer(void *hdl, bool device_dir, uint32_t fifo_port_addr, void* paddr, int len_done, int dlen, int len);

static int rcar_kcrc_dma_xfer_total(void *hdl, bool device_dir, uint32_t fifo_port_addr, void* paddr, int len);

static int RCAR_KCRC_FIFO_DATA_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize);
//static int RCAR_KCRC_FIFO_DATA_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize);

static int RCAR_KCRC_FIFO_RESULT_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize);
//static int RCAR_KCRC_FIFO_RESULT_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize);

//static int RCAR_KCRC_FIFO_COMMAND_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize);
static int RCAR_KCRC_FIFO_COMMAND_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize);

/***********************************************
 *	APIs IMPLEMENATION
 ***********************************************/

/*!
 *    @brief        Write value to specific register in KCRC module
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    uint32_t reg
 *    @param[in]    Value need to to write
 *    @return       None
 */
static void RCAR_KCRC_REG_WRITE (CRC_DEVICE_Type* dev, uint32_t reg, uint32_t dwValue)
{
    out32(dev->kcrc_base_ptr + reg, dwValue);
    return;
}

/*!
 *    @brief        Read value to specific register in KCRC module
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    uint32_t reg
 *    @return       Value of register
 */
static uint32_t  RCAR_KCRC_REG_READ (CRC_DEVICE_Type* dev, uint32_t reg)
{
    return in32(dev->kcrc_base_ptr + reg);
}

/*!
 *    @brief        Supply or stop clock for CRC module.
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    bool enable
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED 
 */
int rcar_kcrc_clock_enable(CRC_DEVICE_Type* dev, bool enable)
{
    uintptr_t smstpcr13 = (uintptr_t) NULL;
    uintptr_t cpgwpr    = (uintptr_t) NULL;
    uint32_t  dwValue;
    int rc = 0;

    uintptr_t smstpcr12 =  (uintptr_t) NULL;
    uintptr_t smstpcr9 =  (uintptr_t) NULL;


    ThreadCtl(_NTO_TCTL_IO, 0);

    smstpcr13 = (uintptr_t) mmap_device_io (4,  RCAR_GEN3_CPG_BASE + RCAR_V3U_CPG_MSTPCR13);
    if (smstpcr13 == (uintptr_t) NULL) {
        fprintf(stderr, "%s: mmap_device_io failed \r\n", __func__);
        rc = -1;
        goto fail;
    }

    smstpcr12 = (uintptr_t) mmap_device_io (4,  RCAR_GEN3_CPG_BASE + RCAR_V3U_CPG_MSTPCR12);
    if (smstpcr12 == (uintptr_t) NULL) {
        fprintf(stderr, "%s: mmap_device_io failed \r\n", __func__);
        rc = -1;
        goto fail;
    }

    smstpcr9 = (uintptr_t) mmap_device_io (4,  RCAR_GEN3_CPG_BASE + RCAR_V3U_CPG_MSTPCR9);
    if (smstpcr9 == (uintptr_t) NULL) {
        fprintf(stderr, "%s: mmap_device_io failed \r\n", __func__);
        rc = -1;
        goto fail;
    }

    cpgwpr = (uintptr_t) mmap_device_io(4, RCAR_GEN3_CPG_BASE + RCAR_GEN3_CPG_CPGWPR);
    if (cpgwpr == (uintptr_t) NULL) {
        fprintf(stderr, "%s: mmap_device_io failed \r\n", __func__);
        rc = -1;
        goto fail;
    }

    if (enable == true) { // Supply clock to module
        switch (dev->kcrc_base) {
            case RCAR_GEN3_KCRC0_BASE:
               dwValue = in32(smstpcr12);
               dwValue &= ~(1 << 31);
               out32 (cpgwpr, ~dwValue);
               out32 (smstpcr12, dwValue);
               break;
            case RCAR_GEN3_KCRC1_BASE:
                dwValue = in32(smstpcr13);
                dwValue &= ~(1 << 0);
                out32 (cpgwpr, ~dwValue);
                out32 (smstpcr13, dwValue);
                break;
            case RCAR_GEN3_KCRC2_BASE:
                dwValue = in32(smstpcr13);
                dwValue &= ~(1 << 1);
                out32 (cpgwpr, ~dwValue);
                out32 (smstpcr13, dwValue);
                break;
            case RCAR_GEN3_KCRC3_BASE:
                dwValue = in32(smstpcr13);
                dwValue &= ~(1 << 2);
                out32 (cpgwpr, ~dwValue);
                out32 (smstpcr13, dwValue);
                break;
            default:
                fprintf(stderr, "%s: Do not support crc_base = 0x%x \r\n", __func__, dev->crc_base);
                rc = -1;
                goto fail;
        }
    } else { // Stop clock to module
        switch (dev->kcrc_base) {
            case RCAR_GEN3_KCRC0_BASE:
               dwValue = in32(smstpcr12);
               dwValue |= (1 << 31);
               out32 (cpgwpr, ~dwValue);
               out32 (smstpcr12, dwValue);
               break;
            case RCAR_GEN3_KCRC1_BASE:
                dwValue = in32(smstpcr13);
                dwValue |= (1 << 0);
                out32 (cpgwpr, ~dwValue);
                out32 (smstpcr13, dwValue);
                break;
            case RCAR_GEN3_KCRC2_BASE:
                dwValue = in32(smstpcr13);
                dwValue |= (1 << 1);
                out32 (cpgwpr, ~dwValue);
                out32 (smstpcr13, dwValue);
                break;
            case RCAR_GEN3_KCRC3_BASE:
                dwValue = in32(smstpcr13);
                dwValue |= (1 << 2);
                out32 (cpgwpr, ~dwValue);
                out32 (smstpcr13, dwValue);
                break;
            default:
                fprintf(stderr, "%s: Do not support crc_base = 0x%x \r\n", __func__, dev->crc_base);
                rc = -1;
                goto fail;
        }
    }

    dwValue = in32(smstpcr9);
    if (enable == true) {
        switch (dev->wcrc_base) { // Supply clock to module
        case RCAR_GEN3_WCRC0_BASE:
            dwValue &= ~(1 << 3);
            break;
        case RCAR_GEN3_WCRC1_BASE:
            dwValue &= ~(1 << 4);
            break;
        case RCAR_GEN3_WCRC2_BASE:
            dwValue &= ~(1 << 5);
            break;
        case RCAR_GEN3_WCRC3_BASE:
            dwValue &= ~(1 << 6);
            break;
        default:
           fprintf(stderr, "%s: Do not support wcrc_base = 0x%x \r\n", __func__, dev->wcrc_base);
            rc = -1;
            goto fail;
        }
    } else { // Stop clock to module
       switch (dev->wcrc_base) { // Supply clock to module
        case RCAR_GEN3_WCRC0_BASE:
            dwValue |= (1 << 3);
            break;
        case RCAR_GEN3_WCRC1_BASE:
            dwValue |= (1 << 4);
            break;
        case RCAR_GEN3_WCRC2_BASE:
            dwValue |= (1 << 5);
            break;
        case RCAR_GEN3_WCRC3_BASE:
            dwValue |= (1 << 6);
            break;
        default:
           fprintf(stderr, "%s: Do not support wcrc_base = 0x%x \r\n", __func__, dev->wcrc_base);
            rc = -1;
            goto fail;
        }
    }
    out32 (cpgwpr, ~dwValue);
    out32 (smstpcr9, dwValue);

fail:

   if ( smstpcr13 != (uintptr_t)NULL) {
       munmap_device_io (smstpcr13, 4);
   }

   if ( smstpcr12 != (uintptr_t)NULL) {
       munmap_device_io (smstpcr12, 4);
   }

   if ( smstpcr9 != (uintptr_t)NULL) {
       munmap_device_io (smstpcr9, 4);
   }

   if ( cpgwpr != (uintptr_t)NULL) {
       munmap_device_io (cpgwpr, 4);
   }

   return rc;
}


/*!
 *    @brief        Start enable KCRC module. Allocate necessary memories, fifo dma
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED 
 */
int RCAR_KCRC_Open(CRC_DEVICE_Type* dev)
{
    int rc = 0;

    if (rcar_kcrc_clock_enable(dev, true) == -1) {
        rc = -1;
        goto fail;
    }

    dev->kcrc_base_ptr = (uintptr_t) mmap_device_memory (NULL, RCAR_GEN3_KCRCn_SIZE, PROT_READ | PROT_WRITE | PROT_NOCACHE, 0, dev->kcrc_base);
    if (dev->kcrc_base_ptr == (uintptr_t)NULL) {
        fprintf(stderr, "%s: Allocation error \r\n", __func__);
        rc = -1;
        goto fail;
    }

    dev->wcrc_base_ptr = (uintptr_t) mmap_device_memory (NULL, RCAR_GEN3_WCRCn_SIZE, PROT_READ | PROT_WRITE | PROT_NOCACHE, 0, dev->wcrc_base);
    if (dev->wcrc_base_ptr == (uintptr_t)NULL) {
        fprintf(stderr, "%s: Allocation error \r\n", __func__);
        // Free memory for previous map
        munmap_device_memory((void *)dev->crc_base_ptr,  RCAR_GEN3_CRCn_SIZE);
        rc = -1;
        goto fail;
    }

    rc = rcar_dma_init(dev);
    if (rc == -1) {
        fprintf(stderr, "%s: Initilize dma for crc is failed \r\n", __func__);
         // Free memory for previous map
        munmap_device_memory((void *)dev->kcrc_base_ptr,  RCAR_GEN3_KCRCn_SIZE);
        munmap_device_memory((void *)dev->wcrc_base_ptr,  RCAR_GEN3_WCRCn_SIZE);
        goto fail;
    }

fail:
   return rc;
}

/*!
 *    @brief        Get current CRC configuration value
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[out]   crc_config_t* config
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
int RCAR_KCRC_GetConfig(CRC_DEVICE_Type* dev, crc_config_t* config)
{
    config->polynomial = gdwpolynomial;
    config->reflectIn = gbreflectIn;
    config->reflectOut = gbreflectOut;
    config->outputXORValue = gdwoutputXORValue;
    config->initValue = gdwinitValue;

    return 0;
}

/*!
 *    @brief        Write configuration data from app to KCRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const crc_config_t* config
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
int RCAR_KCRC_SetConfig(CRC_DEVICE_Type* dev, crc_config_t* config)
{
    uint32_t dwValue;
    int rc = 0;

    gdwpolynomial = config->polynomial;
    gbreflectIn = config->reflectIn;
    gbreflectOut = config->reflectOut;
    gdwoutputXORValue = config->outputXORValue;
    gdwinitValue = config->initValue;

    switch (config->polynomial) {
        case CRC32_IEEE_802_3_POL:
            break;
        case CRC16_CCITT_FALSE_POL:
            break;
        case CRC8_SAE_J1850_POL:
            break;
        case CRC8_0x2F_POL:
            break;
        case CRC32_0xF4ACFB13_POL:
            break;
        case CRC32C_0x1EDC6F41_POL:
            break;
        default:
            fprintf(stderr, "%s: do not support 0x%x polinominal \r\n", __func__, config->polynomial);
            rc = -1;
            return rc;
            break;

    }

    /*
    <For KCRC module>
    1.	Set KCRCmCTL, KCRCmPOLY, KCRCmXOR registers.
    2.	Set initial value to KCRCmDOUT register.
    3.	Set input value to KCRCmDIN register.
    4.	Read outthe operated data from KCRCmDOUT register.

    */
    if (dev->crc_mode == INDEPENDENT_CRC_MODE) {
        dwValue = RCAR_KCRC_REG_READ (dev, RCAR_GEN3_KCRCmCTL);
        dwValue &= ~(RCAR_GEN3_KCRCmCTL_DW_MASS << RCAR_GEN3_KCRCmCTL_DW);
        dwValue |= (RCAR_GEN3_KCRCmCTL_DW_8 << RCAR_GEN3_KCRCmCTL_DW);
    } else {
        dwValue = RCAR_KCRC_REG_READ (dev, RCAR_GEN3_KCRCmCTL);
        dwValue &= ~(RCAR_GEN3_KCRCmCTL_DW_MASS << RCAR_GEN3_KCRCmCTL_DW);
        dwValue |= (RCAR_GEN3_KCRCmCTL_DW_32 << RCAR_GEN3_KCRCmCTL_DW);
    }

    gbreflectOut = config->reflectOut;
    gdwoutputXORValue = config->outputXORValue;

    dwValue &= ~(RCAR_GEN3_KCRCmCTL_PSIZE_MASS << RCAR_GEN3_KCRCmCTL_PSIZE);
    if (config->polynomial < 0xFF) {
       dwValue |= (RCAR_GEN3_KCRCmCTL_PSIZE_8 << RCAR_GEN3_KCRCmCTL_PSIZE);
    } else if (config->polynomial < 0xFFFF) {
       dwValue |= (RCAR_GEN3_KCRCmCTL_PSIZE_16 << RCAR_GEN3_KCRCmCTL_PSIZE);
    } else {
       dwValue |= (RCAR_GEN3_KCRCmCTL_PSIZE_32 << RCAR_GEN3_KCRCmCTL_PSIZE);
    }

    if (config->reflectIn == true) {
        dwValue |= (1 << RCAR_GEN3_KCRCmCTL_CMD1);
    } else {
        dwValue &= ~(1 << RCAR_GEN3_KCRCmCTL_CMD1);
    }

    if (config->reflectOut == true) {
        dwValue |= (1 << RCAR_GEN3_KCRCmCTL_CMD0);
    } else {
        dwValue &= ~(1 << RCAR_GEN3_KCRCmCTL_CMD0);
    }

    RCAR_KCRC_REG_WRITE (dev, RCAR_GEN3_KCRCmCTL, dwValue);
    RCAR_KCRC_REG_WRITE (dev, RCAR_GEN3_KCRCmPOLY, config->polynomial);
    RCAR_KCRC_REG_WRITE (dev, RCAR_GEN3_KCRCmXOR, config->outputXORValue);
    RCAR_KCRC_REG_WRITE (dev, RCAR_GEN3_KCRCmDOUT, config->initValue);
    
    return rc;
}

/*!
 *    @brief        Write data from app to KCRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const uint8_t * data
 *    @param[in]    size_t dataSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
int RCAR_KCRC_WriteData(CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize)
{
     uint32_t dwValue;
     int rc = 0;
     uint8_t *data_tmp;
     uint32_t dwValue_result;

     data_tmp = malloc(4);

    if (dev->crc_mode == INDEPENDENT_CRC_MODE) {
        //3. Set input value to KCRCmDIN register.
        for (int i = 0; i < dataSize; i++) {
            RCAR_KCRC_REG_WRITE (dev, RCAR_GEN3_KCRCmDIN, *(data+i));
        }

    } else if (dev->crc_mode == E2E_CRC_MODE) {

        /*
        1. Set CRC conversion size to once in WCRCm_XXXX_CONV register.
        2. Set initial CRC code value in WCRCm_XXXX_INIT_CRC register.DONE
        3. (For KCRC) Set KCRCmCTL, KCRCmPOLY, KCRCmXOR, KCRCmDOUT registers. DONE
        4. Set in_en=1, trans_en=1, res_en=1 in WCRCm_XXXX_EN register.
        5. Set cmd_en=1 in WCRCm_XXXX_CMDEN register
        6. Transfer input data to data port of FIFO by DMAC.
        7. Read out result data from result port of FIFO by DMAC.
        8. Set stop=1 in WCRCm_XXXX_STOP by command function.
        9. Clear stop_done in WCRCm_XXXX_STS.
        */

        //1. Set CRC conversion size to once in WCRCm_XXXX_CONV register.
        //   Notes: maximum is 0x10 0000
        if (dataSize > RCAR_GEN3_WCRCm_KCRCm_CONV_MAX) {
            fprintf(stderr, "%s: Maximum for data is %d \r\n", __func__,  RCAR_GEN3_WCRCm_CRCm_CONV_MAX);
            rc = -1;
            return rc;
        } else {
            RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_CONV, dataSize);
        }

        //4.      Set in_en=1, trans_en=1, res_en=1 in WCRCm_XXXX_EN register.
        dwValue = RCAR_WCRC_REG_READ(dev, RCAR_GEN3_WCRCm_KCRCm_EN);
        dwValue |= 1 << RCAR_GEN3_WCRCm_KCRCm_EN_in_en;
        dwValue |= 1 << RCAR_GEN3_WCRCm_KCRCm_EN_trans_en;
        dwValue |= 1 << RCAR_GEN3_WCRCm_KCRCm_EN_res_en;
        RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_EN, dwValue);        

        // 5.	Set cmd_en=1 in WCRCm_XXXX_CMDEN register
        dwValue =  RCAR_WCRC_REG_READ(dev, RCAR_GEN3_WCRCm_KCRCm_CMDEN);
        dwValue |= 1 << RCAR_GEN3_WCRCm_KCRCm_CMDEN_cmd_en;
        RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_CMDEN, dwValue);

        if ((dataSize %  4) != 0) {
            if (dataSize /  4) {
                RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_CONV, dataSize - (dataSize %  4));
                RCAR_KCRC_FIFO_DATA_PORT_Write(dev, data, dataSize - (dataSize %  4));
                RCAR_KCRC_FIFO_RESULT_PORT_Read (dev, &dwValue_result, 4);
                dwValue = 0;
                if(gbreflectOut == false) {
                    dwValue = dwValue_result ^ gdwoutputXORValue;
                } else {
                    dwValue_result ^= gdwoutputXORValue;
                    if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL) ){
                       dwValue = bitswap8(dwValue_result);
                    } else if (gdwpolynomial == CRC16_CCITT_FALSE_POL){
                       dwValue = bitswap16(dwValue_result);
                    } else {
                       dwValue = bitswap32(dwValue_result);
                    }
                }
                RCAR_KCRC_REG_WRITE (dev, RCAR_GEN3_KCRCmDOUT, dwValue);
            }

            if((dataSize %  4) == 1) {
                RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_CONV, 1);
                *data_tmp = *(data + dataSize -1);
                *(data_tmp + 1) = 0x00;
                *(data_tmp + 2) = 0x00;
                *(data_tmp + 3) = 0x00;
                RCAR_KCRC_FIFO_DATA_PORT_Write(dev,  data_tmp, 1);
            } else if ((dataSize %  4) == 2) {
                RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_CONV, 2);
                *data_tmp = *(data + dataSize -2);
                *(data_tmp + 1) = *(data + dataSize -1);
                *(data_tmp + 2) = 0x00;
                *(data_tmp + 3) = 0x00;
                RCAR_KCRC_FIFO_DATA_PORT_Write(dev,  data_tmp, 2);
            }else if ((dataSize %  4) == 3) {
                 RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_CONV, 2);
                *data_tmp = *(data + dataSize - 3);
                *(data_tmp + 1) = *(data + dataSize -2);
                *(data_tmp + 2) = 0x00;
                *(data_tmp + 3) = 0x00;
                RCAR_KCRC_FIFO_DATA_PORT_Write(dev,  data_tmp, 2);
                RCAR_KCRC_FIFO_RESULT_PORT_Read (dev, &dwValue_result, 4);
                if(gbreflectOut == false) {
                    dwValue = dwValue_result ^ gdwoutputXORValue;
                } else {
                     dwValue_result ^= gdwoutputXORValue;
                     if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL) ){
                           dwValue = bitswap8(dwValue_result);
                     } else if (gdwpolynomial == CRC16_CCITT_FALSE_POL){
                         dwValue = bitswap16(dwValue_result);
                     } else { 
                         dwValue = bitswap32(dwValue_result);
                     }
                }
                RCAR_WCRC_REG_WRITE(dev,  RCAR_GEN3_WCRCm_CRCm_INIT_CRC, dwValue);
                RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_CONV, 1);
                //Set initial value to DCRAnCOUT register.
                RCAR_KCRC_REG_WRITE(dev, RCAR_GEN3_KCRCmDOUT, dwValue);
                *data_tmp = *(data + dataSize -1);
                *(data_tmp + 1) = 0x00;
                *(data_tmp + 2) = 0x00;
                *(data_tmp + 3) = 0x00;
                RCAR_KCRC_FIFO_DATA_PORT_Write(dev,  data_tmp, 1);
            }
        } else {
           RCAR_KCRC_FIFO_DATA_PORT_Write(dev, data, dataSize);
        }

    } else {
        fprintf(stderr, "%s: Do not support mode = %d \r\n", __func__, dev->crc_mode);
        rc = -1;
    }

    free(data_tmp);

    return rc;
}

/*!
 *    @brief        Get output value from KCRC and return upper layer
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @return       Calculated result of CRC for 32 bits
 */
uint32_t RCAR_KCRC_GetResult(CRC_DEVICE_Type * dev)
{
    uint32_t dwValue;
    uint32_t dwValue_result;
    uint8_t data[4] = {0x00, 0x00, 0x00, 0x01};

    if (dev->crc_mode == INDEPENDENT_CRC_MODE) {
        usleep(1000);
        dwValue_result = RCAR_KCRC_REG_READ(dev,  RCAR_GEN3_KCRCmDOUT);
        if (gdwpolynomial == CRC8_0x2F_POL){
            return (dwValue_result & CRC8_OUTPUT_MASS);
        } else if (gdwpolynomial == CRC8_SAE_J1850_POL){
            return (dwValue_result & CRC8_OUTPUT_MASS);
        } else if  (gdwpolynomial == CRC16_CCITT_FALSE_POL){
            return (dwValue_result & CRC16_OUTPUT_MASS);
        } else {
            return (dwValue_result);
        }
    } else if (dev->crc_mode == E2E_CRC_MODE) {
        usleep(1000);
        //7.    Read out result data from result port of FIFO by DMAC.
        RCAR_KCRC_FIFO_RESULT_PORT_Read (dev, &dwValue_result, 4);
        
        //8.    Set stop=1 in WCRCm_XXXX_STOP by command function.
        data[3] = 1 << RCAR_GEN3_WCRCm_KCRCm_STOP_stop;
        RCAR_KCRC_FIFO_COMMAND_PORT_Write(dev, data, sizeof(data));

        //9.    Clear stop_done in WCRCm_XXXX_STS.
        dwValue =  RCAR_WCRC_REG_READ(dev, RCAR_GEN3_WCRCm_KCRCm_STS);
         //Clear stop_done bit 31
        dwValue |= (1 << RCAR_GEN3_WCRCm_KCRCm_STS_stop_done); // Write 1 to clear
        RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_KCRCm_STS, dwValue);

        if (gdwpolynomial == CRC8_0x2F_POL){
            return (dwValue_result & CRC8_OUTPUT_MASS);
        } else if (gdwpolynomial == CRC8_SAE_J1850_POL){
            return (dwValue_result & CRC8_OUTPUT_MASS);
        } else if  (gdwpolynomial == CRC16_CCITT_FALSE_POL){
            return (dwValue_result & CRC16_OUTPUT_MASS);
        } else {
            return (dwValue_result);
        }
    } else {
        fprintf(stderr, "%s: Do not support mode = %d \r\n", __func__, dev->crc_mode);
        return 0x00000000;
    }

}

/*!
 *    @brief        Close KCRC module, free alllowed memmory
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @return       None
 */
int  RCAR_KCRC_Close(CRC_DEVICE_Type* dev)
{
    munmap_device_memory ((void *)dev->kcrc_base_ptr, RCAR_GEN3_KCRCn_SIZE);
    munmap_device_memory ((void *)dev->wcrc_base_ptr, RCAR_GEN3_WCRCn_SIZE);

    dev->dmafuncs.channel_release(dev->rx_dma);
    dev->dmafuncs.channel_release(dev->tx_dma);

    rcar_kcrc_clock_enable(dev, false);

    if (dev != NULL) {
        free(dev);
    }
    return 0;
}

/*!
 *    @brief        Write data to FIFO DATA port of KCRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const uint8_t * data
 *    @param[in]    size_t dataSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_KCRC_FIFO_DATA_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize)
{
    int rc = 0;
    dma_addr_t pdmabuf;

    /* Allocate buffer for DMA */
    if (dev->dmafuncs.alloc_buffer(dev->tx_dma, &pdmabuf, dataSize, DMA_BUF_FLAG_NOCACHE) != 0) {
        fprintf(stderr, "Unable to allocate DMA buffer");
        dev->dmafuncs.channel_release(dev->tx_dma);
        rc = -1;
        return rc;
    }

    /* Copy data from upper layer to dma memory */
    memcpy ((void *)pdmabuf.vaddr, data, dataSize );

    rc = rcar_kcrc_dma_xfer_total(dev, DEVICE_IN, dev->wcrc_fifo_data_port, (void*)pdmabuf.paddr, dataSize);

    /* Free dma buffer */
    dev->dmafuncs.free_buffer(dev->tx_dma, &pdmabuf);

    return rc;
}

/* Below code can be used in the future so comment out temperary*/
#if 0
/*!
 *    @brief        Read data to FIFO DATA port of KCRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[out]   uint32_t *recv_data
 *    @param[in]    size_t dataRecvSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_KCRC_FIFO_DATA_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize)
{

    off64_t offset;
    int rc = 0;

    if(mem_offset64(recv_data, NOFD, 1, &offset, 0) == -1) {
        fprintf(stderr, "%s: Get Physical address of receive buffer is falied \r\n", __func__);
    }

    rc = rcar_kcrc_dma_xfer_total(dev, DEVICE_OUT, dev->wcrc_fifo_data_port, (void *)offset, dataRecvSize);

    return rc;
}

/*!
 *    @brief        Write data to FIFO RESULT port of KCRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const uint8_t * data
 *    @param[in]    size_t dataSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_KCRC_FIFO_RESULT_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t *data, size_t dataSize)
{
    off64_t offset;
    int rc = 0;

    if(mem_offset64(data, NOFD, 1, &offset, 0) == -1) {
        fprintf(stderr, "%s: Get Physical address of receive buffer is falied \r\n", __func__);
    }

    rc = rcar_kcrc_dma_xfer_total(dev, DEVICE_IN, dev->wcrc_fifo_result_port, (void *)offset, dataSize);

    return rc;

}
#endif

/*!
 *    @brief        Read data to FIFO RESULT port of KCRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[out]    uint32_t *recv_data
 *    @param[in]    size_t dataRecvSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_KCRC_FIFO_RESULT_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize)
{
    int rc = 0;

    rc = rcar_kcrc_dma_xfer_total(dev, DEVICE_OUT, dev->wcrc_fifo_result_port, NULL, dataRecvSize);
    if (rc != -1) {
        *recv_data =  in32((uintptr_t)dev->pdmabuf.vaddr);
    }

    return rc;
}

/*!
 *    @brief        Write data to FIFO COMMAND port of KCRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const uint8_t * data
 *    @param[in]    size_t dataSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_KCRC_FIFO_COMMAND_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t *data, size_t dataSize)
{
    off64_t offset;
    int rc;

    if(mem_offset64(data, NOFD, 1, &offset, 0) == -1) {
      fprintf(stderr, "%s: Get Physical address of receive buffer is falied \r\n", __func__);
    }

    rc = rcar_kcrc_dma_xfer_total(dev, DEVICE_IN, dev->wcrc_fifo_command_port, (void *)offset, dataSize);

    return rc;

}

/* Below code can be used in the future so comment out temperary*/
#if 0
/*!
 *    @brief        Read data to FIFO COMMAND port of KCRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[out]    uint32_t *recv_data
 *    @param[in]    size_t dataRecvSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_KCRC_FIFO_COMMAND_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize)
{
    int rc = 0;

    rc = rcar_kcrc_dma_xfer_total(dev, DEVICE_OUT, dev->wcrc_fifo_command_port, NULL, dataRecvSize);
    if (rc != -1) {
        *recv_data = in32((uintptr_t)dev->pdmabuf.vaddr);
    }

    return rc;
}
#endif

/*!
 *    @brief        Get data from app and send to specific FIFO port using DMA
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    bool device_dir
 *    @param[in]    uint32_t fifo_port_addr
 *    @param[inout] void* paddr (in/out depend on device_dir)
 *    @param[in]    int len
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int rcar_kcrc_dma_xfer_total(void *hdl, bool device_dir, uint32_t fifo_port_addr, void* paddr, int len)
{
    CRC_DEVICE_Type    *dev = hdl;
    int n, len_left, len_txrx_once;

    len_left = len;

    while (len_left > 0) {
        len_txrx_once = len_left;
        n = rcar_crc_dma_xfer(dev, device_dir, fifo_port_addr, paddr, 0, 1, len_txrx_once);
        if (n < 0) return  -1;

        len_left -= n;
    }

    return len;
}

