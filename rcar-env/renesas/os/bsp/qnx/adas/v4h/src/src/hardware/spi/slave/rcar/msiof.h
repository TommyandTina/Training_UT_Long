/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#ifndef SPI_SLAVE_RCAR_MSIOF_H_
#define SPI_SLAVE_RCAR_MSIOF_H_

#define NUMS_OF_SPI_DEV          (6)
#define SPI_REG_LEN              (0x64)
#define SPI_FIFO_BIT_LEN         (32)

/* SICTR bits*/
#define SICTR_TSCKIZ_MASK        (0x03 >> 30)
#define SICTR_TSCKIZ(opt)        ((opt) << 30) //Transmit clock polarity
#define TSCKIZ_0                 (2)
#define TSCKIZ_1                 (3)

#define SICTR_RSCKIZ_MASK        (0x03 >> 28)
#define SICTR_RSCKIZ(opt)        ((opt) << 28) //Receive clock polarity
#define RSCKIZ_0                 (2)
#define RSCKIZ_1                 (3)

#define SICTR_TEDG_MASK          (0x08 >> 24)
#define SICTR_TEDG(opt)          ((opt) << 27) //Transmit timing
#define TRANS_AT_RISING_EDGE     (0)
#define TRANS_AT_FALLING_EDGE    (1)

#define SICTR_REDG_MASK          (0x04 >> 24)
#define SICTR_REDG(opt)          ((opt) << 26) //Receive timing
#define RECV_AT_FALLING_EDGE     (0)
#define RECV_AT_RISING_EDGE      (1)

#define SICTR_TXDIZ              (1 << 22) //MSIOF_TXD output when transmission is disabled.
#define SICTR_TSCKE              (1 << 15) //Transmit Serial Clock Output Enable
#define SICTR_TFSE               (1 << 14) //Transmit Frame Sync Signal Output Enable
#define SICTR_TXE                (1 << 9)  //Transmit Enable
#define SICTR_RXE                (1 << 8)  //Receive Enable
#define SICTR_TXRST              (1 << 1)  //Transmit Reset
#define SICTR_RXRST              (1 << 0)  //Receive Reset

/* SIIER bit */
#define SIIER_TDMAE              (1 << 31) //Transmit Data DMA Transfer Request Enable
#define SIIER_TFEMPE             (1 << 29) //Transmit FIFO Empty Enable
#define SIIER_TDREQE             (1 << 28) //Transmit Data Transfer Request Enable
#define SIIER_TEOF               (1 << 23) //Frame Transmission End Enable
#define SIIER_TFSERR             (1 << 21) //Transmit Frame Synchronization Error Enable
#define SIIER_TFOVFE             (1 << 20) //Transmit FIFO Overflow Enable
#define SIIER_TFUDFE             (1 << 19) //Transmit FIFO Underflow Enable
#define SIIER_RDMAE              (1 << 15) //Receive Data DMA Transfer Request Enable
#define SIIER_RFFULE             (1 << 13) //Receive FIFO Full Enable
#define SIIER_RDREQE             (1 << 12) //Receive Data Transfer Request
#define SIIER_REOF               (1 << 7)  //Frame Reception End
#define SIIER_RFSERRE            (1 << 5)  //Receive Frame Synchronization Error
#define SIIER_RFUDFE             (1 << 4)  //Receive FIFO Underflow
#define SIIER_RFOVFE             (1 << 3)  //Receive FIFO Overflow Enable

/* SISTR bit */
#define SISTR_TFEMP              (1 << 29)
#define SISTR_TDREQ              (1 << 28)
#define SISTR_TEOF               (1 << 23)
#define SISTR_TFSERR             (1 << 21)
#define SISTR_TFOVF              (1 << 20)
#define SISTR_TFUDF              (1 << 19)
#define SISTR_RFFUL              (1 << 13)
#define SISTR_RDREQ              (1 << 12)
#define SISTR_REOF               (1 << 7)
#define SISTR_RFSERR             (1 << 5)
#define SISTR_RFUDF              (1 << 4)
#define SISTR_RFOVF              (1 << 3)

/* SITMDR1 register */
#define SITMDR1_PCON               (1 << 30)

#define SITMDR1_TRANSFER_MODE(opt) ((opt) << 31)
#define MODE_MASTER                (1)
#define MODE_SLAVE                 (0)

#define SITMDR1_SYNCMD_MODE(opt)   ((opt) << 28)
#define FRAME_START_PULSE_MODE     (0)
#define SPI_MODE                   (2)
#define L_R_MODE                   (3)

#define SITMDR1_SYNCCH_SELECT(opt) ((opt) << 26)
#define SYNCCH_MSIOF_SYNC          (0)
#define SYNCCH_MSIOF_SS_1          (1)
#define SYNCCH_MSIOF_SS_2          (2)

#define SITMDR1_SYNCAC(opt)        ((opt) << 25)
#define ACTIVE_HIGH                (0)
#define ACTIVE_LOW                 (1)

#define SITMDR1_BITLSB(opt)        ((opt) << 24)
#define MSB_FIRST                  (0)
#define LSB_FIRST                  (1)

#define SITMDR1_DTDL(opt)          ((opt) << 20)
#define DTDL_DELAY_0_CLK           (0)
#define DTDL_DELAY_1_CLK           (1)
#define DTDL_DELAY_2_CLK           (2)
#define DTDL_DELAY_0_5_CLK         (5)
#define DTDL_DELAY_1_5_CLK         (6)

#define SITMDR1_SYNCDL(opt)        ((opt) << 16)
#define SYNCDL_DELAY_0_CLK         (0)
#define SYNCDL_DELAY_1_CLK         (1)
#define SYNCDL_DELAY_2_CLK         (2)
#define SYNCDL_DELAY_3_CLK         (3)
#define SYNCDL_DELAY_0_5_CLK       (5)
#define SYNCDL_DELAY_1_5_CLK       (6)

#define SITMDR1_FLD(opt)           ((opt) << 2)
#define FLD_DELAY_0_CLK            (0)
#define FLD_DELAY_1_CLK            (1)
#define FLD_DELAY_2_CLK            (2)
#define FLD_DELAY_3_CLK            (3)

#define SITMDR1_TXSTP              (1 << 0)

//For SIRMDR1, only SYNCMD, SYNCAC and BITLSB are the same as SITMDR1, other bits are 0
#define SIRMDR1_MASK               (0x33000000)

#endif /* SPI_SLAVE_RCAR_MSIOF_H_ */
