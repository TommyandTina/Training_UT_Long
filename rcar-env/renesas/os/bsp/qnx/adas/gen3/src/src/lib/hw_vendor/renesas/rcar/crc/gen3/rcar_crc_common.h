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

#ifndef _RCAR_CRC_COMMON_
#define _RCAR_CRC_COMMON_

#include <hw/dma.h>
#include <hw/crc.h>
#include <stdio.h> 
#include <inttypes.h>
#include <devctl.h>
#include <stdbool.h>

#define CRC_EVENT        0x71
#define CRC_RXDMA_EVENT  0x72
#define CRC_TXDMA_EVENT  0x73
#define CRC_PRIORITY     21 

#define DEVICE_IN        0
#define DEVICE_OUT       1

#define CRC32_IEEE_802_3_POL   0x4C11DB7
#define CRC16_CCITT_FALSE_POL  0x1021
#define CRC8_SAE_J1850_POL     0x1D
#define CRC8_0x2F_POL          0x2F
#define CRC32_0xF4ACFB13_POL   0xF4ACFB13
#define CRC32C_0x1EDC6F41_POL  0x1EDC6F41
#define CRC21_0x102899_POL     0x102899
#define CRC17_0x1685B_POL      0x1685B
#define CRC15_0x4599_POL       0x4599

#define CRC32_OUTPUT_MASS      0xFFFFFFFF
#define CRC21_OUTPUT_MASS      0x1FFFFF
#define CRC17_OUTPUT_MASS      0x1FFFF
#define CRC16_OUTPUT_MASS      0xFFFF
#define CRC15_OUTPUT_MASS      0x7FFF
#define CRC8_OUTPUT_MASS       0xFF

typedef void *PVOID;

//typedef enum crc_mode {
//    INDEPENDENT_CRC_MODE     = 0x00u,  
//    E2E_CRC_MODE             = 0x01u,     
//    CAIP_Lite_MODE           = 0x02u,   
//    DATA_THROUGH_MODE        = 0x03u, 
//    E2E_AND_DATA_THROUGH_MODE = 0x04u,     
//    CRC_PROHIBITED_MODE
//} crc_base_mode_t;


/** CRC DEVICE TYPE*/
typedef struct {
    uint32_t  crc_base;
    uint32_t  kcrc_base;
    uint32_t  wcrc_base;
    uint32_t  wcrc_fifo_data_port;
    uint32_t  wcrc_fifo_command_port;
    uint32_t  wcrc_fifo_expected_data_port;
    uint32_t  wcrc_fifo_result_port;
    uintptr_t crc_base_ptr;
    uintptr_t kcrc_base_ptr;
    uintptr_t wcrc_base_ptr;

    crc_base_mode_t crc_mode;

    struct sigevent crcevent;
    
    /* for DMA transfer */
    uint8_t         fifo_size; /* FIFO size */
    int             irq_crc;
    int             chid;
    int             coid;
    int             iid_crc;
    int             dtime; /* usec per data, for time out use */
    int             xlen;
    dma_functions_t dmafuncs;
    void            *tx_dma;
    void            *rx_dma;
    dma_addr_t      pdmabuf;
    uint8_t         tx_mid_rid;
    uint8_t         rx_mid_rid;
    int             smmu;
    struct smmu_object *smmu_obj;
    
} CRC_DEVICE_Type;

#endif
