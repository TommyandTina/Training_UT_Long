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

#ifndef _RCAR_WCRC_
#define _RCAR_WCRC_

#include "rcar_crc_common.h"

/*
/bin/bash: :q: command not found
 */
#define RCAR_GEN3_WCRC0_BASE               0xE7080000
#define RCAR_GEN3_WCRC1_BASE               0xE7090000
#define RCAR_GEN3_WCRC2_BASE               0xE70A0000
#define RCAR_GEN3_WCRC3_BASE               0xE70B0000

#define RCAR_GEN3_WCRCn_SIZE               0x0FC4

#define RCAR_GEN3_WCRC0_FIFO_BASE          0xFFFF0000
#define RCAR_GEN3_WCRC1_FIFO_BASE          0xFFFF4000
#define RCAR_GEN3_WCRC2_FIFO_BASE          0xFFFF8000
#define RCAR_GEN3_WCRC3_FIFO_BASE          0xFFFFC000

#define CRC_FIFO_DATA_PORT_OFFSET          0x800
#define CRC_FIFO_COMMAND_PORT_OFFSET       0x900
#define CRC_FIFO_EXPECTED_DATA_PORT_OFFSET 0xA00
#define CRC_FIFO_RESULT_PORT_OFFSET        0xB00

#define KCRC_FIFO_DATA_PORT_OFFSET          0xC00
#define KCRC_FIFO_COMMAND_PORT_OFFSET       0xD00
#define KCRC_FIFO_EXPECTED_DATA_PORT_OFFSET 0xE00
#define KCRC_FIFO_RESULT_PORT_OFFSET        0xF00

/* WCRC registers, offset from RCAR_GEN3_WCRCn_BASE */
#define RCAR_GEN3_WCRCm_CAIPn_EN	       0x0000 //WCRCm CAIPn transfer enable register
#define RCAR_GEN3_WCRCm_CAIPn_STOP	       0x0020 //WCRCm CAIPn transfer stop register
#define RCAR_GEN3_WCRCm_CAIPn_CMDEN	       0x0030 //WCRCm CAIPn command enable register
#define RCAR_GEN3_WCRCm_CAIPn_WAIT	       0x0080 //WCRCm CAIPn wait register
#define RCAR_GEN3_WCRCm_CAIPn_STS	       0x0200 //WCRCm CAIPn status register
#define RCAR_GEN3_WCRCm_CAIPn_INTEN	       0x0240 //WCRCm CAIPn interrupt enable register
#define RCAR_GEN3_WCRCm_CAIPp_EN	       0x0400 //WCRCm CAIPp transfer enable register
#define RCAR_GEN3_WCRCm_CAIPp_STOP	       0x0420 //WCRCm CAIPp transfer stop register
#define RCAR_GEN3_WCRCm_CAIPp_CMDEN	       0x0430 //WCRCm CAIPp command enable register
#define RCAR_GEN3_WCRCm_CAIPp_WAIT	       0x0480 //WCRCm CAIPp wait register
#define RCAR_GEN3_WCRCm_CAIPp_STS	       0x0600 //WCRCm CAIPp status register
#define RCAR_GEN3_WCRCm_CAIPp_INTEN	       0x0640 //WCRCm CAIPp interrupt enable register
#define RCAR_GEN3_WCRCm_CRCm_EN	           0x0800 //WCRCm CRCm transfer enable register
#define RCAR_GEN3_WCRCm_CRCm_STOP	       0x0820 //WCRCm CRCm transfer stop register
#define RCAR_GEN3_WCRCm_CRCm_CMDEN	       0x0830 //WCRCm CRCm transfer  command enable register
#define RCAR_GEN3_WCRCm_CRCm_COMP	       0x0840 //WCRCm CRCm compare setting register	
#define RCAR_GEN3_WCRCm_CRCm_COMP_RES	   0x0850 //WCRCm CRCm compare result register	
#define RCAR_GEN3_WCRCm_CRCm_CONV	       0x0870 //WCRCm CRCm conversion setting register
#define RCAR_GEN3_WCRCm_CRCm_WAIT	       0x0880 //WCRCm CRCm wait register
#define RCAR_GEN3_WCRCm_CRCm_INIT_CRC	   0x0910 //WCRCm CRCm initial CRC code register
#define RCAR_GEN3_WCRCm_CRCm_STS	       0x0A00 //WCRCm CRCm status register
#define RCAR_GEN3_WCRCm_CRCm_INTEN	       0x0A40 //WCRCm CRCm interrupt enable register
#define RCAR_GEN3_WCRCm_CRCm_ECMEN	       0x0A80 //WCRCm CRCm ECM output enable register
#define RCAR_GEN3_WCRCm_KCRCm_EN	       0x0C00 //WCRCm KCRCm transfer enable register
#define RCAR_GEN3_WCRCm_KCRCm_STOP	       0x0C20 //WCRCm KCRCm transfer stop register
#define RCAR_GEN3_WCRCm_KCRCm_CMDEN	       0x0C30 //WCRCm KCRCm  command enable register
#define RCAR_GEN3_WCRCm_KCRCm_COMP	       0x0C40 //WCRCm KCRCm compare setting register
#define RCAR_GEN3_WCRCm_KCRCm_COMP_RES	   0x0C50 //WCRCm KCRCm compare result register
#define RCAR_GEN3_WCRCm_KCRCm_CONV	       0x0C70 //WCRCm KCRCm conversion setting register
#define RCAR_GEN3_WCRCm_KCRCm_WAIT	       0x0C80 //WCRCm KCRCm wait register
#define RCAR_GEN3_WCRCm_KCRCm_INIT_CRC	   0x0D10 //WCRCm KCRCm initial CRC code register
#define RCAR_GEN3_WCRCm_KCRCm_STS	       0x0E00 //WCRCm KCRCm status register
#define RCAR_GEN3_WCRCm_KCRCm_INTEN	       0x0E40 //WCRCm KCRCm interrupt enable register
#define RCAR_GEN3_WCRCm_KCRCm_ECMEN	       0x0E80 //WCRCm KCRCm ECM output enable register
#define RCAR_GEN3_WCRCm_COMMON_STS	       0x0F00 //WCRCm common status register
#define RCAR_GEN3_WCRCm_COMMON_INTEN	   0x0F40 //WCRCm common interrupt enable register
#define RCAR_GEN3_WCRCm_COMMON_ECMEN	   0x0F80 //WCRCm common ECM output enable register
#define RCAR_GEN3_WCRCm_ERRINJ	           0x0FC0 //WCRCm error injection register

/* WCRC bit define for registers*/
#define RCAR_GEN3_WCRCm_CRCm_STOP_stop         0

#define RCAR_GEN3_WCRCm_CRCm_STS_stop_done     31

#define RCAR_GEN3_WCRCm_CRCm_CONV_MAX          0x100000

#define RCAR_GEN3_WCRCm_CRCm_EN_in_en          0
#define RCAR_GEN3_WCRCm_CRCm_EN_trans_en       1
#define RCAR_GEN3_WCRCm_CRCm_EN_res_en         8
#define RCAR_GEN3_WCRCm_CRCm_EN_out_en         16

#define RCAR_GEN3_WCRCm_CRCm_CMDEN_cmd_en      0

#define RCAR_GEN3_WCRCm_KCRCm_STOP_stop        0

#define RCAR_GEN3_WCRCm_KCRCm_STS_stop_done    31

#define RCAR_GEN3_WCRCm_KCRCm_CONV_MAX         0x100000

#define RCAR_GEN3_WCRCm_KCRCm_EN_in_en         0
#define RCAR_GEN3_WCRCm_KCRCm_EN_trans_en      1
#define RCAR_GEN3_WCRCm_KCRCm_EN_res_en        8
#define RCAR_GEN3_WCRCm_KCRCm_EN_out_en        16

#define RCAR_GEN3_WCRCm_KCRCm_CMDEN_cmd_en     0

/*
 * CRC module  
 */
#define RCAR_GEN3_CRC0_BASE               0xE6F00000
#define RCAR_GEN3_CRC1_BASE               0xE6F10000
#define RCAR_GEN3_CRC2_BASE               0xE7000000
#define RCAR_GEN3_CRC3_BASE               0xE7010000

#define RCAR_GEN3_CRCn_SIZE               0x0044

/* CRC registers, offset from RCAR_GEN3_CRCn_BASE */
#define RCAR_GEN3_CRC_DCRAnCIN	          0x0000 //CRC input register
#define RCAR_GEN3_CRC_DCRAnCOUT	          0x0004 //CRC data register
#define RCAR_GEN3_CRC_DCRAnCTL	          0x0020 //CRC control register
#define RCAR_GEN3_CRC_DCRAnCTL2	          0x0040 //CRC control register2

/* CRC bit define for registers*/
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC32_IEEE802_3   0x0
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC16_CCITT_FALSE 0x1
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC8_SAE_J1850    0x2
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC8_0x2F         0x3
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC32_0xF4ACFB13  0x4
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC32C_0x1EDC6F41 0x5
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC21_0x102899    0x6
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC17_0x1685B     0x7
#define RCAR_GEN3_CRC_DCRAnCTL_POL_CRC15_0x4599      0x8
#define RCAR_GEN3_CRC_DCRAnCTL_POL                   0
#define RCAR_GEN3_CRC_DCRAnCTL_POL_MASS              15


#define RCAR_GEN3_CRC_DCRAnCTL_ISZ         4
#define RCAR_GEN3_CRC_DCRAnCTL_ISZ_32      0x0
#define RCAR_GEN3_CRC_DCRAnCTL_ISZ_16      0x1
#define RCAR_GEN3_CRC_DCRAnCTL_ISZ_8       0x2
#define RCAR_GEN3_CRC_DCRAnCTL_ISZ_MASS    3

#define RCAR_GEN3_CRC_DCRAnCTL2_xorvalmode           7
#define RCAR_GEN3_CRC_DCRAnCTL2_bitswapmode          6
#define RCAR_GEN3_CRC_DCRAnCTL2_byteswapmode         4
#define RCAR_GEN3_CRC_DCRAnCTL2_xorvalinmode         3
#define RCAR_GEN3_CRC_DCRAnCTL2_bitswapinmode        2
#define RCAR_GEN3_CRC_DCRAnCTL2_byteswapinmode       0

/*
 * KCRC module
 */
#define RCAR_GEN3_KCRC0_BASE               0xE7020000
#define RCAR_GEN3_KCRC1_BASE               0xE7030000
#define RCAR_GEN3_KCRC2_BASE               0xE7040000
#define RCAR_GEN3_KCRC3_BASE               0xE7050000

#define RCAR_GEN3_KCRCn_SIZE               0x00B4

/* KCRC registers, offset from RCAR_GEN3_KCRCn_BASE */
#define RCAR_GEN3_KCRCmDIN	           0x00 //KCRC data input register
#define RCAR_GEN3_KCRCmDOUT	           0x80 //KCRC data output register	
#define RCAR_GEN3_KCRCmCTL	           0x90 //KCRC control register
#define RCAR_GEN3_KCRCmPOLY	           0xA0 //KCRC Polynomial register
#define RCAR_GEN3_KCRCmXOR	           0xB0 //KCRC XOR mask register

/* KCRC bit define for registers*/
#define RCAR_GEN3_KCRCmCTL_DW          0
#define RCAR_GEN3_KCRCmCTL_DW_32       0
#define RCAR_GEN3_KCRCmCTL_DW_16       1
#define RCAR_GEN3_KCRCmCTL_DW_8        3
#define RCAR_GEN3_KCRCmCTL_DW_MASS     0x7

#define RCAR_GEN3_KCRCmCTL_PSIZE       16
#define RCAR_GEN3_KCRCmCTL_PSIZE_32    0x1F
#define RCAR_GEN3_KCRCmCTL_PSIZE_16    0xF
#define RCAR_GEN3_KCRCmCTL_PSIZE_8     0x7
#define RCAR_GEN3_KCRCmCTL_PSIZE_MASS  0x1F

#define RCAR_GEN3_KCRCmCTL_CMD0        8
#define RCAR_GEN3_KCRCmCTL_CMD1        5

/* DMA REQUEST */
#define RCAR_GEN3_DREQ_CRC0_RXI            0x18 // dmareq_wcrc0_crc0_res
#define RCAR_GEN3_DREQ_CRC0_TXI            0x19 // dmareq_wcrc0_crc0_in

#define RCAR_GEN3_DREQ_CRC1_RXI            0x48 // dmareq_wcrc1_crc1_res
#define RCAR_GEN3_DREQ_CRC1_TXI            0x49 // dmareq_wcrc1_crc1_in

#define RCAR_GEN3_DREQ_CRC2_RXI            0x88 // dmareq_wcrc2_crc2_res
#define RCAR_GEN3_DREQ_CRC2_TXI            0x89 // dmareq_wcrc2_crc2_in

#define RCAR_GEN3_DREQ_CRC3_RXI            0xC8 // dmareq_wcrc3_crc3_res
#define RCAR_GEN3_DREQ_CRC3_TXI            0xC9 // dmareq_wcrc3_crc3_in

#define RCAR_GEN3_DREQ_KCRC0_RXI           0x1C // dmareq_wcrc0_kcrc4_res
#define RCAR_GEN3_DREQ_KCRC0_TXI           0x1D // dmareq_wcrc0_kcrc4_in 

#define RCAR_GEN3_DREQ_KCRC1_RXI           0x4C // dmareq_wcrc1_kcrc5_res
#define RCAR_GEN3_DREQ_KCRC1_TXI           0x4D // dmareq_wcrc1_kcrc5_in

#define RCAR_GEN3_DREQ_KCRC2_RXI           0x8C // dmareq_wcrc2_kcrc6_res
#define RCAR_GEN3_DREQ_KCRC2_TXI           0x8D // dmareq_wcrc2_kcrc6_in

#define RCAR_GEN3_DREQ_KCRC3_RXI           0xCC // dmareq_wcrc3_kcrc7_res
#define RCAR_GEN3_DREQ_KCRC3_TXI           0xCD // dmareq_wcrc3_kcrc7_in

void RCAR_WCRC_REG_WRITE (CRC_DEVICE_Type* dev, uint32_t reg, uint32_t dwValue);
uint32_t  RCAR_WCRC_REG_READ (CRC_DEVICE_Type* dev, uint32_t reg);
uint32_t bitswap32 (uint32_t dwValue_In);
uint32_t bitswap16 (uint32_t dwValue_In);
uint32_t bitswap8 (uint32_t dwValue_In);

#endif
