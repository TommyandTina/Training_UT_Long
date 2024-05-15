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

#include "rcar_crc.h"
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
extern CRC_DEVICE_Type *crc_dev;

static uint32_t   gdwpolynomial = 0;
static bool       gbreflectIn = false;
static bool       gbreflectOut = false;
static uint32_t   gdwoutputXORValue = 0x00000000;
static uint32_t   gdwinitValue = 0x00000000;

/***********************************************
 *      FUNCTION DECLARATION
 ***********************************************/
extern int rcar_dma_init(CRC_DEVICE_Type* dev);
extern int rcar_crc_dma_xfer(void *hdl, bool device_dir, uint32_t fifo_port_addr, void* paddr, int len_done, int dlen, int len);

static int RCAR_CRC_FIFO_DATA_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize);
//static int RCAR_CRC_FIFO_DATA_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize);

//static int RCAR_CRC_FIFO_RESULT_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize);
static int RCAR_CRC_FIFO_RESULT_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize);

static int RCAR_CRC_FIFO_COMMAND_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize);
//static int RCAR_CRC_FIFO_COMMAND_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize);

static int rcar_crc_dma_xfer_total(CRC_DEVICE_Type* dev, bool device_dir, uint32_t fifo_port_addr, void* paddr, int len);

/***********************************************
 *	APIs IMPLEMENATION
 ***********************************************/

/*!
 *    @brief        Write value to specific register in CRC module
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    uint32_t reg
 *    @param[in]    Value need to to write
 *    @return       None
 */
static void RCAR_CRC_REG_WRITE (CRC_DEVICE_Type* dev, uint32_t reg, uint32_t dwValue)
{
    out32(dev->crc_base_ptr + reg, dwValue);
}

/*!
 *    @brief        Read value to specific register in CRC module
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    uint32_t reg
 *    @return       Value of register
 */
static uint32_t  RCAR_CRC_REG_READ (CRC_DEVICE_Type* dev, uint32_t reg)
{
    return in32(dev->crc_base_ptr + reg);
}


/*!
 *    @brief        Supply or stop clock for CRC module.
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    bool enable
  *    @return       0 : SUCCESSFUL
 *                  -1: FAILED 
 */
int rcar_crc_clock_enable(CRC_DEVICE_Type* dev, bool enable)
{
    uintptr_t cpgwpr = (uintptr_t) NULL;
    uintptr_t smstpcr12 = (uintptr_t) NULL;
    uintptr_t smstpcr9 = (uintptr_t) NULL;
    uint32_t  dwValue;
    int rc = 0;

    ThreadCtl(_NTO_TCTL_IO, 0);

    smstpcr12 = (uintptr_t) mmap_device_io (4, RCAR_GEN3_CPG_BASE + RCAR_V3U_CPG_MSTPCR12);
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

    dwValue = in32(smstpcr12);

    if (enable == true) { // Supply clock to module
        switch (dev->crc_base) {
           case RCAR_GEN3_CRC0_BASE:
               dwValue &= ~(1 << 25);
               break;
           case RCAR_GEN3_CRC1_BASE:
               dwValue &= ~(1 << 26);
               break;
           case RCAR_GEN3_CRC2_BASE:
               dwValue &= ~(1 << 27);
               break;
           case RCAR_GEN3_CRC3_BASE:
               dwValue &= ~(1 << 28);
               break;
           default:
              fprintf(stderr,"%s: Do not support crc_base = 0x%x \r\n", __func__, dev->crc_base);
              rc = -1;
              goto fail;
        }
    } else {// Stop clock to the module
        switch (dev->crc_base) {
           case RCAR_GEN3_CRC0_BASE:
               dwValue |= (1 << 25);
               break;
           case RCAR_GEN3_CRC1_BASE:
               dwValue |= (1 << 26);
               break;
           case RCAR_GEN3_CRC2_BASE:
               dwValue |= (1 << 27);
               break;
           case RCAR_GEN3_CRC3_BASE:
               dwValue |= (1 << 28);
               break;
           default:
              fprintf(stderr,"%s: Do not support crc_base = 0x%x \r\n", __func__, dev->crc_base);
              rc = -1;
              goto fail;
        }       
    }
    out32 (cpgwpr, ~dwValue);
    out32 (smstpcr12, dwValue);

    dwValue = in32(smstpcr9);

    if (enable == true) { // Supply clock to module
        switch (dev->wcrc_base) {
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
    } else { // Stop clock to the module
        switch (dev->wcrc_base) {
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
 *    @brief        Start enable CRC module. Allocate necessary memories, fifo dma
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED 
 */
int RCAR_CRC_Open(CRC_DEVICE_Type* dev)
{
    int rc = 0;

    if (rcar_crc_clock_enable(dev, true) == -1) {
        rc = -1;
        goto fail;
    }

    dev->crc_base_ptr = (uintptr_t) mmap_device_memory (NULL, RCAR_GEN3_CRCn_SIZE, PROT_READ | PROT_WRITE | PROT_NOCACHE, 0, dev->crc_base);
    if (dev->crc_base_ptr == (uintptr_t)NULL) {
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
        munmap_device_memory((void *)dev->crc_base_ptr,  RCAR_GEN3_CRCn_SIZE);
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
int RCAR_CRC_GetConfig(CRC_DEVICE_Type* dev, crc_config_t* config)
{
    config->polynomial = gdwpolynomial;
    config->reflectIn = gbreflectIn;
    config->reflectOut = gbreflectOut;
    config->outputXORValue = gdwoutputXORValue;
    config->initValue = gdwinitValue;

    return 0;
}

/*!
 *    @brief        Write configuration data from app to CRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const crc_config_t* config
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
int RCAR_CRC_SetConfig(CRC_DEVICE_Type* dev, const crc_config_t* config)
{
    uint32_t  dwValue;
    int rc = 0;

    gdwpolynomial = config->polynomial;
    gbreflectIn = config->reflectIn;
    gbreflectOut = config->reflectOut;
    gdwoutputXORValue = config->outputXORValue;
    gdwinitValue = config->initValue;
 
    if ( ( dev->crc_mode == INDEPENDENT_CRC_MODE ) || ( dev->crc_mode == E2E_CRC_MODE ) )  {
        dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
        dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ); 
        if ( dev->crc_mode == INDEPENDENT_CRC_MODE ) {
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;// Using 8 bit for INDEPENDENT mode
        } else {
            if (config->polynomial == CRC8_0x2F_POL) {
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;
            } else if (config->polynomial == CRC8_SAE_J1850_POL) {
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;
            } else {
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;
            }
        }
        RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

        /*
        <For CRC module>
        1. Set DCRAnCTL, DCRAnCTL2 registers.
        2. Set initial value to DCRAnCOUT register.
        3. Set input value to DCRAnCIN register.
        4. Read out the operated data from DCRAnCOUT register.
        */

        RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCOUT, 0xFFFFFFFF);//be sure to write 0xFFFFFFFF as defalt such as document

        //1.Set DCRAnCTL, DCRAnCTL2 registers.
        //    Set DCRAnCTL.
        //        Notes: ISZ bit for size of input data will be set in write data
        dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
        dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_POL_MASS << RCAR_GEN3_CRC_DCRAnCTL_POL);// Clear POL[3:0] bits

        switch (config->polynomial) {
        case CRC32_IEEE_802_3_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC32_IEEE802_3;
            if ( (config->outputXORValue & CRC32_OUTPUT_MASS) % CRC32_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }
            break;
        case CRC16_CCITT_FALSE_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC16_CCITT_FALSE;
            if ( (config->outputXORValue & CRC16_OUTPUT_MASS) % CRC16_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }
            break;
        case CRC8_SAE_J1850_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC8_SAE_J1850;
            if ( (config->outputXORValue & CRC8_OUTPUT_MASS) % CRC8_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }
            break;
        case CRC8_0x2F_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC8_0x2F;
            if ( (config->outputXORValue & CRC8_OUTPUT_MASS) % CRC8_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }            
            break;
        case CRC32_0xF4ACFB13_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC32_0xF4ACFB13;
            if ( (config->outputXORValue & CRC32_OUTPUT_MASS) % CRC32_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }
            break;
        case CRC32C_0x1EDC6F41_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC32C_0x1EDC6F41;
            if ( (config->outputXORValue & CRC32_OUTPUT_MASS) % CRC32_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }
            break;
        case CRC21_0x102899_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC21_0x102899;
            if ( (config->outputXORValue & CRC21_OUTPUT_MASS) % CRC21_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }
            break;
        case CRC17_0x1685B_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC17_0x1685B;
            if ( (config->outputXORValue & CRC17_OUTPUT_MASS) % CRC17_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }
            break;
        case CRC15_0x4599_POL:
            dwValue |= RCAR_GEN3_CRC_DCRAnCTL_POL_CRC15_0x4599;
            if ( (config->outputXORValue & CRC15_OUTPUT_MASS) % CRC15_OUTPUT_MASS) {
                fprintf(stderr, "%s: do not support outputXORValue = 0x%x\r\n", __func__, config->outputXORValue);
                rc = -1;
                return rc;
            }
            break;
        default:
            fprintf(stderr, "%s: do not support 0x%x polinominal \r\n", __func__, config->polynomial);
            rc = -1;
            return rc;
        break;

        }
        RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

        // Set DCRAnCTL2
        dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
        dwValue &= (uint32_t)~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalinmode);

        // Register setting for outputXORValue
        if ((config->polynomial == CRC32_IEEE_802_3_POL)
         || (config->polynomial == CRC32_0xF4ACFB13_POL)
         || (config->polynomial == CRC32C_0x1EDC6F41_POL) 
         || (config->polynomial == CRC21_0x102899_POL )
         || (config->polynomial == CRC17_0x1685B_POL )
         || (config->polynomial == CRC15_0x4599_POL )) {

            if (config->outputXORValue != 0x00000000) { // This is confict with document, so fix me
                dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit
            } else { // This is conflict with document, so fix me
                dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode);// Set xorvalmode bit
            }

        } else if ( config->polynomial == CRC8_0x2F_POL) {
            if ( dev->crc_mode == INDEPENDENT_CRC_MODE ) {
                if (config->outputXORValue != 0x00000000) { // This is confict with document, so fix me
                    dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit
                } else { // This is conflict with document, so fix me
                    dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode);// Set xorvalmode bit
                }
            } else { //E2E_CRC_MODE
                if (config->outputXORValue != 0x00000000) { // This is confict with document, so fix me
                    dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit
                } else { // This is conflict with document, so fix me
                    dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode); // Set xorvalmode bit
                }
            }

        }else if ( config->polynomial == CRC8_SAE_J1850_POL) {
            if ( dev->crc_mode == INDEPENDENT_CRC_MODE ) {
                if (config->outputXORValue != 0x00000000) { // This is confict with document, so fix me
                    dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit
                } else { // This is conflict with document, so fix me
                    dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode);// Set xorvalmode bit
                }
            } else { //E2E_CRC_MODE
                if (config->outputXORValue != 0x00000000) { // This is confict with document, so fix me
                    dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit
                } else { // This is conflict with document, so fix me
                    dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode);// Set xorvalmode bit
                }
            }
        } else { // CRC16_CCITT_FALSE_POL
            if (config->outputXORValue == 0x00000000) {
                dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit
            } else {
                dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode); // Set xorvalmode bit
            }
        }

        // Register setting for reflectIn
        if ( (config->polynomial == CRC32_IEEE_802_3_POL) 
          || (config->polynomial == CRC32_0xF4ACFB13_POL)
          || (config->polynomial == CRC32C_0x1EDC6F41_POL)
          || (config->polynomial == CRC21_0x102899_POL)
          || (config->polynomial == CRC17_0x1685B_POL )
          || (config->polynomial == CRC15_0x4599_POL ) ) {
            if (config->reflectIn == true) { // This is conflict with document, so fix me
                dwValue &= (uint32_t)~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode);
                dwValue &= (uint32_t)~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode);
            } else {
                dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
            }

            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

        } else if (config->polynomial == CRC8_0x2F_POL) {
            if (dev->crc_mode == E2E_CRC_MODE) {
                // No affect so please use software and fix me 
                if (config->reflectIn == true) {
                    dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode);
                    dwValue &= (uint32_t)~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode);
                } else {
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                    dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                }
            } else {
                if (config->reflectIn == true) {
                    dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode);
                    dwValue &= (uint32_t)~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode);
                } else {
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                    dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                }
            }
            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
        } else if (config->polynomial == CRC8_SAE_J1850_POL) {
            if (dev->crc_mode == E2E_CRC_MODE) {
                // No affect so please use software and fix me
                if (config->reflectIn == true) {
                    dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode);
                    dwValue &= (uint32_t)~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode);
                } else {
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                    dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                }
            } else {
                if (config->reflectIn == true) { // This is conflict with document, so fix me
                    dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode);
                    dwValue &= (uint32_t)~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode);
                } else {
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                    dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                }
            }
            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
        }else{
            if (config->reflectIn == true) { // This is conflict with document, so fix me
                dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode);
                dwValue &= (uint32_t)~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode);
            } else {
                if (config->polynomial == CRC16_CCITT_FALSE_POL ) {
                    dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                    dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                } else {
                    dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                    dwValue &= (uint32_t)(~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                }
                if (config->polynomial == CRC8_0x2F_POL ) {
                    dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                    dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                } else {
                    dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode));
                    dwValue &= (uint32_t)(~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                }
            }
            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
        }

        if ( dev->crc_mode == INDEPENDENT_CRC_MODE ) {
            dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
        } 

       // Register setting for reflectOut
        if ( (config->polynomial == CRC32_IEEE_802_3_POL) 
          || (config->polynomial == CRC32_0xF4ACFB13_POL)
          || (config->polynomial == CRC32C_0x1EDC6F41_POL)
          || (config->polynomial == CRC21_0x102899_POL)
          || (config->polynomial == CRC17_0x1685B_POL)
          || (config->polynomial == CRC15_0x4599_POL )  ) {

            if (config->reflectOut == true) { // This is conflict with document, so fix me
                dwValue &= (uint32_t)~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapmode);
                dwValue &= (uint32_t)~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapmode);
            } else {
                dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapmode));
                dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapmode));
            }
            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

        } else {

            if (config->reflectOut == true) {
                dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapmode);
                if (config->polynomial == CRC16_CCITT_FALSE_POL) {
                   dwValue |= (uint32_t)(1 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapmode);
                }else {
                 dwValue &= (uint32_t)~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapmode);
                }
            } else {
                if (config->polynomial == CRC21_0x102899_POL) {
                  dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapmode));
                  dwValue &= (uint32_t)(~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapmode));
                } else {
                  dwValue &= (uint32_t)(~(1 << RCAR_GEN3_CRC_DCRAnCTL2_bitswapmode));
                  dwValue &= (uint32_t)(~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapmode));
                }
            }
            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
        }

        //2.Set initial value to DCRAnCOUT register.
        RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCOUT, config->initValue);

    } else if (dev->crc_mode == CAIP_Lite_MODE) {
        /*
        * TBD
        */
        fprintf(stderr, "%s: Do not support CAIP_Lite_MODE\r\n", __func__);
    } else if (dev->crc_mode == DATA_THROUGH_MODE) {
        /*
        * TBD 
        */
        fprintf(stderr, "%s: Do not support DATA_THROUGH_MODE\r\n", __func__);
    } else if (dev->crc_mode == E2E_AND_DATA_THROUGH_MODE) {
        /*
        * TBD
        */
        fprintf(stderr, "%s: Do not support E2E_AND_DATA_THROUGH_MODE\r\n", __func__);

    } else {
       fprintf(stderr, "%s: Do not support this mode \r\n", __func__); 
    }

    return 0;
}

/*!
 *    @brief        Write data from app to CRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const uint8_t * data
 *    @param[in]    size_t dataSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
int RCAR_CRC_WriteData(CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize)
{
    
    uint32_t dwValue;   
    uint8_t *data_tmp;
    uint32_t dwValue_result;
    int rc = 0;

    data_tmp = malloc(4);

    if (dev->crc_mode == INDEPENDENT_CRC_MODE) {
        /*
        <For CRC module>
        1.	Set DCRAnCTL, DCRAnCTL2 registers. DONE
        2.	Set initial value to DCRAnCOUT register.DONE
        3.	Set input value to DCRAnCIN register.
        4.	Read out the operated data from DCRAnCOUT register.
        */

        //3.      Set input value to DCRAnCIN register.
        for (int i = 0; i < dataSize; i++) {
            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCIN, *(data+i));
        }

    } else if (dev->crc_mode == E2E_CRC_MODE) {
        /*
        1. Set CRC conversion size to once in WCRCm_XXXX_CONV register.
        2. Set initial CRC code value in WCRCm_XXXX_INIT_CRC register. DONE
        3.	(For CRC) Set DCRAnCTL, DCRAnCTL2, DCRAnCOUT registers. DONE
        (For KCRC) Set KCRCmCTL, KCRCmPOLY, KCRCmXOR, KCRCmDOUT registers.
        4.	Set in_en=1, trans_en=1, res_en=1 in WCRCm_XXXX_EN register.
        5.	Set cmd_en=1 in WCRCm_XXXX_CMDEN register
        6.	Transfer input data to data port of FIFO by DMAC.
        7.	Read out result data from result port of FIFO by DMAC.
        8.	Set stop=1 in WCRCm_XXXX_STOP by command function.
        9.	Clear stop_done in WCRCm_XXXX_STS.
        */

        //1. Set CRC conversion size to once in WCRCm_XXXX_CONV register.
        //   Notes: maximum is 0x10 0000
        if (dataSize > RCAR_GEN3_WCRCm_CRCm_CONV_MAX) {
            fprintf(stderr, "%s: Maximum for data is %d \r\n", __func__,  RCAR_GEN3_WCRCm_CRCm_CONV_MAX);
            rc = -1;
            return rc;
        } else {
            RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, dataSize);
        }

        //4. Set in_en=1, trans_en=1, res_en=1 in WCRCm_XXXX_EN register.
        dwValue = RCAR_WCRC_REG_READ(dev, RCAR_GEN3_WCRCm_CRCm_EN);
        dwValue |= 1 << RCAR_GEN3_WCRCm_CRCm_EN_in_en;
        dwValue |= 1 << RCAR_GEN3_WCRCm_CRCm_EN_trans_en;
        dwValue |= 1 << RCAR_GEN3_WCRCm_CRCm_EN_res_en;
        RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_EN, dwValue);        

        // 5. Set cmd_en=1 in WCRCm_XXXX_CMDEN register
        dwValue =  RCAR_WCRC_REG_READ(dev, RCAR_GEN3_WCRCm_CRCm_CMDEN);
        dwValue |= 1 << RCAR_GEN3_WCRCm_CRCm_CMDEN_cmd_en;
        RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CMDEN, dwValue);

        if ((dataSize %  4) == 1) {
            if (dataSize /  4) {
                if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                    dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                    dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ; // Using 32 bit
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is no output ^ 0xFF. Fix me
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else if(gdwpolynomial == CRC16_CCITT_FALSE_POL) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                    dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                    dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;// Using 32 bit
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is no output ^ 0xFF
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                }

                RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, dataSize - 1);
                RCAR_CRC_FIFO_DATA_PORT_Write(dev, data, dataSize - 1);
                RCAR_CRC_FIFO_RESULT_PORT_Read (dev, &dwValue_result, 4);
                dwValue = 0;
                if (gdwpolynomial == CRC16_CCITT_FALSE_POL)  {

                    if(gbreflectOut == true) {
                        dwValue = bitswap16(dwValue_result);
                    } else {
                         dwValue = dwValue_result & CRC16_OUTPUT_MASS;
                    }
                }else if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)) {
                    if(gbreflectOut == true) {
                        dwValue = bitswap8(dwValue_result);
                    } else {
                        dwValue = dwValue_result & CRC8_OUTPUT_MASS;
                    }
                } else {
                    if(gbreflectOut == false) {
                        dwValue_result ^= gdwoutputXORValue;
                        dwValue = bitswap32(dwValue_result);
                    } else {
                        dwValue = dwValue_result ^ gdwoutputXORValue;
                    }
                }
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCOUT, dwValue);
            }

            if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)){
                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 8 bit as for 1 byte
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                if (gdwoutputXORValue != 0x00) { 
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is output ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is output not ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                    }
            } else if (gdwpolynomial == CRC16_CCITT_FALSE_POL) {

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 8 bit as for 1 byte
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                if (gdwoutputXORValue == 0x00) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is output ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is output not ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                }
            }

            RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, 1);
            *data_tmp = *(data + dataSize -1);
            *(data_tmp + 1) = 0x00;
            *(data_tmp + 2) = 0x00;
            *(data_tmp + 3) = 0x00;
            RCAR_CRC_FIFO_DATA_PORT_Write(dev,  data_tmp, 1);
         }else if ((dataSize %  4) == 2) {
            if (dataSize /  4) {
                if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                    dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                    dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 32 bit
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is no output ^ 0xFF. Fix me
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else if(gdwpolynomial == CRC16_CCITT_FALSE_POL) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                    dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                    dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;// Using 32 bit
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is no output ^ 0xFF
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                }

                RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, dataSize - 2);
                RCAR_CRC_FIFO_DATA_PORT_Write(dev, data, dataSize - 1);
                RCAR_CRC_FIFO_RESULT_PORT_Read (dev, &dwValue_result, 4);
                dwValue = 0;

                if ( gdwpolynomial == CRC16_CCITT_FALSE_POL)  {
                    if(gbreflectOut == true) {
                        dwValue = bitswap16(dwValue_result);
                    } else {
                         dwValue = dwValue_result & CRC16_OUTPUT_MASS;
                    }

                } else if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)) {
                    if(gbreflectOut == true) {
                        dwValue = bitswap8(dwValue_result);
                    } else {
                        dwValue = dwValue_result & CRC8_OUTPUT_MASS;
                    }
                } else {
                    if(gbreflectOut == false) {
                        dwValue_result ^= gdwoutputXORValue;
                        dwValue = bitswap32(dwValue_result);
                    } else {
                        dwValue = dwValue_result ^ gdwoutputXORValue;
                    }
                }
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCOUT, dwValue);
            }

            if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)){

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 32 bit as default
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                if (gdwoutputXORValue != 0x00) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is output ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is output not ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                }
            } else if (gdwpolynomial == CRC16_CCITT_FALSE_POL) {
                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 8 bit as for 1 byte
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                if (gdwoutputXORValue == 0x00) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is output ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is output not ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                }
            }

            RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, 2);
            *data_tmp = *(data + dataSize -2);
            *(data_tmp + 1) = *(data + dataSize -1);
            *(data_tmp + 2) = 0x00;
            *(data_tmp + 3) = 0x00;
            RCAR_CRC_FIFO_DATA_PORT_Write(dev,  data_tmp, 2);

         }else if ((dataSize %  4) == 3) {
            if (dataSize /  4) {
                if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL) ){
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                    dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                    dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 32 bit
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is no output ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else if(gdwpolynomial == CRC16_CCITT_FALSE_POL) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                    dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                    dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;// Using 32 bit
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is no output ^ 0xFF
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                }

                RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, dataSize - 3);
                RCAR_CRC_FIFO_DATA_PORT_Write(dev, data, dataSize - 3);
                RCAR_CRC_FIFO_RESULT_PORT_Read (dev, &dwValue_result, 4);
                dwValue = 0;

                if ( gdwpolynomial == CRC16_CCITT_FALSE_POL ) {
                    if(gbreflectOut == true) {
                        dwValue = bitswap16(dwValue_result);
                    } else {
                        dwValue = dwValue_result & CRC16_OUTPUT_MASS;
                    }
                } else if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL) ){
                    if(gbreflectOut == true) {
                        dwValue = bitswap8(dwValue_result);
                    } else {
                        dwValue = dwValue_result & CRC8_OUTPUT_MASS;
                    }
                } else {
                     if(gbreflectOut == false) {
                        dwValue_result ^= gdwoutputXORValue;
                        dwValue = bitswap32(dwValue_result);
                     } else {
                        dwValue = dwValue_result ^ gdwoutputXORValue;
                     }
                }
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCOUT, dwValue);
            }

            if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)){
                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 32 bit as default
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is no output ^ 0xFF ????
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

            } else if (gdwpolynomial == CRC16_CCITT_FALSE_POL) {

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 8 bit as for 1 byte
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is output ^ 0xFF ????
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
            }

             RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, 2);
            *data_tmp = *(data + dataSize - 3);
            *(data_tmp + 1) = *(data + dataSize -2);
            *(data_tmp + 2) = 0x00;
            *(data_tmp + 3) = 0x00;
            RCAR_CRC_FIFO_DATA_PORT_Write(dev,  data_tmp, 2);
            RCAR_CRC_FIFO_RESULT_PORT_Read (dev, &dwValue_result, 4);
            if (gdwpolynomial == CRC16_CCITT_FALSE_POL)  {
                if(gbreflectOut == true) {
                    dwValue = bitswap16(dwValue_result);
                } else {
                    dwValue = dwValue_result & CRC16_OUTPUT_MASS;
                }
            } else if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)) {
                if(gbreflectOut == true) {
                    dwValue = bitswap8(dwValue_result);
                } else {
                    dwValue = dwValue_result & CRC8_OUTPUT_MASS;
                }
            } else {
                if(gbreflectOut == false) {
                    dwValue_result ^= gdwoutputXORValue;
                    dwValue = bitswap32(dwValue_result);
                } else {
                    dwValue = dwValue_result ^ gdwoutputXORValue;
                }
            }

            RCAR_WCRC_REG_WRITE(dev,  RCAR_GEN3_WCRCm_CRCm_INIT_CRC, dwValue);
            RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, 1);
            //Set initial value to DCRAnCOUT register.
            RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCOUT, dwValue);

            if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL)){
                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 32 bit as default
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is output ^ 0xFF ????
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                if (gdwoutputXORValue != 0x00) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is output ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is output not ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                }

            } else if (gdwpolynomial == CRC16_CCITT_FALSE_POL) {
                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_8 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 8 bit as for 1 byte
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue &= (uint32_t)( ~(3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                if (gdwoutputXORValue == 0x00) {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue &= (uint32_t)( ~(1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Clear xorvalmode bit. This is output ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                } else {
                    dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                    dwValue |= (uint32_t)( (1 << RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode)); // Set xorvalmode bit. This is output not ^ 0xFF ????
                    RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);
                }
            }

            *data_tmp = *(data + dataSize -1);
            *(data_tmp + 1) = 0x00;
            *(data_tmp + 2) = 0x00;
            *(data_tmp + 3) = 0x00;
            RCAR_CRC_FIFO_DATA_PORT_Write(dev,  data_tmp, 1);

        } else {
           // fix me for the case 0x2F
            if(gdwpolynomial == CRC8_0x2F_POL) {
                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 32 bit as default
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);
           }

          // fix me for the case 0x1021
           if(gdwpolynomial == CRC16_CCITT_FALSE_POL) {
                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL2);
                dwValue |= (uint32_t)( (3 << RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode));
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL2, dwValue);

                dwValue = RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCTL);
                dwValue &= ~(RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS << RCAR_GEN3_CRC_DCRAnCTL_ISZ);
                dwValue |= RCAR_GEN3_CRC_DCRAnCTL_ISZ_32 << RCAR_GEN3_CRC_DCRAnCTL_ISZ;//use 32 bit as default
                RCAR_CRC_REG_WRITE(dev, RCAR_GEN3_CRC_DCRAnCTL, dwValue);
           }
           RCAR_CRC_FIFO_DATA_PORT_Write(dev, data, dataSize);
         }
    } else if (dev->crc_mode == CAIP_Lite_MODE) {
    } else if (dev->crc_mode == DATA_THROUGH_MODE) {
    } else if (dev->crc_mode == E2E_AND_DATA_THROUGH_MODE) {
    }

    free(data_tmp);

    return rc;
}

/*!
 *    @brief        Get output value from CRC and return upper layer
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @return       Calculated result of CRC for 32 bits
 */
uint32_t RCAR_CRC_GetResult(CRC_DEVICE_Type* dev)
{
    uint32_t dwValue_result;
    uint32_t dwValue;
    uint8_t data[4] = {0x00, 0x00, 0x00, 0x01};

    if (dev->crc_mode == INDEPENDENT_CRC_MODE) {
       usleep(1000); // Need delay a bit.
       if (gdwpolynomial == CRC21_0x102899_POL ) {
            // Get 21 bits high.
            if (gbreflectOut == true) {
               return ((RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT) & CRC21_OUTPUT_MASS));
            } else {
               return ((RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT) >> 11));
            } 
       } else if (gdwpolynomial == CRC17_0x1685B_POL ) {
            // Get 17 bits high.
            if (gbreflectOut == true) {
               return ((RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT) & CRC17_OUTPUT_MASS));
            } else {
               return ((RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT) >> 15));
            }
       } else if (gdwpolynomial == CRC15_0x4599_POL ) {
            // Get 15 bits high.
            if (gbreflectOut == true) {
               return ((RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT) & CRC15_OUTPUT_MASS));
            } else {
               return ((RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT) >> 17));
            }
        } else if ((gdwpolynomial == CRC8_0x2F_POL) || (gdwpolynomial == CRC8_SAE_J1850_POL) ){
            return (RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT) & CRC8_OUTPUT_MASS);
        } else if (gdwpolynomial == CRC16_CCITT_FALSE_POL){
            return (RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT) & CRC16_OUTPUT_MASS);
        }else {
           return RCAR_CRC_REG_READ(dev, RCAR_GEN3_CRC_DCRAnCOUT);
        }

    } else if (dev->crc_mode == E2E_CRC_MODE) {
        usleep(1000); // This delay can work with max 0x10 0000 
        RCAR_CRC_FIFO_RESULT_PORT_Read (dev, &dwValue_result, 4);

        //8.    Set stop=1 in WCRCm_XXXX_STOP by command function.
        data[3] = 1 << RCAR_GEN3_WCRCm_CRCm_STOP_stop;
        RCAR_CRC_FIFO_COMMAND_PORT_Write(dev, data, sizeof(data));
        //9.    Clear stop_done in WCRCm_XXXX_STS.
        dwValue =  RCAR_WCRC_REG_READ(dev, RCAR_GEN3_WCRCm_CRCm_STS);
         //Clear stop_done bit 31
        dwValue |= (1 << RCAR_GEN3_WCRCm_CRCm_STS_stop_done);// Write 1 to clear
        RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_STS, dwValue);

        // Add new       
        RCAR_WCRC_REG_WRITE(dev, RCAR_GEN3_WCRCm_CRCm_CONV, 0);
       
        if (gdwpolynomial == CRC21_0x102899_POL ) {
            if (gbreflectOut == true) {
                return (dwValue_result & CRC21_OUTPUT_MASS);
            } else {
                return (dwValue_result >> 11);
            }
        }else if (gdwpolynomial == CRC17_0x1685B_POL) {
            if (gbreflectOut == true) {
                return (dwValue_result & CRC17_OUTPUT_MASS);
            } else {
                return (dwValue_result >> 15);
            }

        } else if (gdwpolynomial == CRC15_0x4599_POL) {
            if (gbreflectOut == true) {
                return (dwValue_result & CRC15_OUTPUT_MASS);
            } else {
                return (dwValue_result >> 17);
            }
        } else if (gdwpolynomial == CRC8_0x2F_POL){
            return (dwValue_result & CRC8_OUTPUT_MASS);
        } else if (gdwpolynomial == CRC8_SAE_J1850_POL){
            return (dwValue_result & CRC8_OUTPUT_MASS);
        } else if  (gdwpolynomial == CRC16_CCITT_FALSE_POL){
            return (dwValue_result & CRC16_OUTPUT_MASS);
        } else {
            return (dwValue_result);
        }
    } else if (dev->crc_mode == CAIP_Lite_MODE) {
    } else if (dev->crc_mode == DATA_THROUGH_MODE) {
    } else if (dev->crc_mode == E2E_AND_DATA_THROUGH_MODE) {
    }
    return 0x00000000;   
}

/*!
 *    @brief        Close CRC module, free alllowed memmory
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @return       None
 */
int  RCAR_CRC_Close(CRC_DEVICE_Type* dev)
{
    munmap_device_memory ((void *)dev->crc_base_ptr, RCAR_GEN3_CRCn_SIZE);
    munmap_device_memory ((void *)dev->wcrc_base_ptr, RCAR_GEN3_WCRCn_SIZE);

    dev->dmafuncs.free_buffer(dev->rx_dma, &dev->pdmabuf);
    dev->dmafuncs.channel_release(dev->rx_dma);
    dev->dmafuncs.channel_release(dev->tx_dma);

    rcar_crc_clock_enable(dev, false);

    if (crc_dev != NULL) {
        free(crc_dev);
    }
    return 0;
}

/*!
 *    @brief        Write data to FIFO DATA port of CRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const uint8_t * data
 *    @param[in]    size_t dataSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_CRC_FIFO_DATA_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t * data,size_t dataSize)
{
        int rc = 0;
        dma_addr_t      pdmabuf;

        /* Allocate buffer for DMA */
        if (dev->dmafuncs.alloc_buffer(dev->tx_dma, &pdmabuf, dataSize, DMA_BUF_FLAG_NOCACHE) != 0) {
            fprintf(stderr, "Unable to allocate DMA buffer\r\n");
            dev->dmafuncs.channel_release(dev->tx_dma);
            return (-1);
        }

        /* Copy data from upper layer to dma memory */
        (void *)memcpy ((void *)pdmabuf.vaddr, data, dataSize );

        rc = rcar_crc_dma_xfer_total(dev, DEVICE_IN, dev->wcrc_fifo_data_port, (void*)pdmabuf.paddr, dataSize);

        /* Free dma buffer */
        dev->dmafuncs.free_buffer(dev->tx_dma, &pdmabuf);
  
        return rc;
}

/*Notes: Below code can be used in the future, so comment out temperary*/
#if 0
/*!
 *    @brief        Read data to FIFO DATA port of CRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[out]    uint32_t *recv_data
 *    @param[in]    size_t dataRecvSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_CRC_FIFO_DATA_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize)
{

        off64_t offset;
        int rc = 0;

        if(mem_offset64(recv_data, NOFD, 1, &offset, 0) == -1) {
          fprintf(stderr, "%s: Get Physical address of receive buffer is falied \r\n", __func__);
        }

        rc = rcar_crc_dma_xfer_total(dev, DEVICE_OUT, dev->wcrc_fifo_data_port, (void*)offset, dataRecvSize);

        return rc;
}

/*!
 *    @brief        Write data to FIFO RESULT port of CRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const uint8_t * data
 *    @param[in]    size_t dataSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_CRC_FIFO_RESULT_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t *data, size_t dataSize)
{
        off64_t offset;
        int rc = 0;

        if(mem_offset64(data, NOFD, 1, &offset, 0) == -1) {
          fprintf(stderr, "%s: Get Physical address of receive buffer is falied \r\n", __func__);
        }

        rc = rcar_crc_dma_xfer_total(dev, DEVICE_IN, dev->wcrc_fifo_result_port, (void*)offset, dataSize);

        return rc;

}
#endif
/*!
 *    @brief        Read data to FIFO RESULT port of CRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[out]    uint32_t *recv_data
 *    @param[in]    size_t dataRecvSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_CRC_FIFO_RESULT_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize)
{
        int rc = 0;
  
        rc = rcar_crc_dma_xfer_total(dev, DEVICE_OUT, dev->wcrc_fifo_result_port, NULL, dataRecvSize);
        if (rc != -1) {
            *recv_data =  in32((uintptr_t)dev->pdmabuf.vaddr);
        }

        return rc;
}

/*!
 *    @brief        Write data to FIFO COMMAND port of CRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[in]    const uint8_t * data
 *    @param[in]    size_t dataSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_CRC_FIFO_COMMAND_PORT_Write (CRC_DEVICE_Type* dev, const uint8_t *data, size_t dataSize)
{
        off64_t offset;
        int rc = 0;

        if(mem_offset64(data, NOFD, 1, &offset, 0) == -1) {
          fprintf(stderr, "%s: Get Physical address of receive buffer is falied \r\n", __func__);
        }

        rc = rcar_crc_dma_xfer_total(dev, DEVICE_IN, dev->wcrc_fifo_command_port, (void*)offset, dataSize);

        return rc;

}

/* Below code can be used in the future so comment out temperary*/
#if 0
/*!
 *    @brief        Read data to FIFO COMMAND port of CRC
 *    @param[in]    CRC_DEVICE_Type* dev
 *    @param[out]    uint32_t *recv_data
 *    @param[in]    size_t dataRecvSize
 *    @return       0 : SUCCESSFUL
 *                  -1: FAILED
 */
static int RCAR_CRC_FIFO_COMMAND_PORT_Read (CRC_DEVICE_Type* dev, uint32_t *recv_data, size_t dataRecvSize)
{
        int rc = 0;

        rc = rcar_crc_dma_xfer_total(dev, DEVICE_OUT, dev->wcrc_fifo_command_port, NULL, dataRecvSize);
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
static int rcar_crc_dma_xfer_total(CRC_DEVICE_Type *dev, bool device_dir, uint32_t fifo_port_addr, void* paddr, int len)
{
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
