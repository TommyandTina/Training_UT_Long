/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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


#ifndef _R_Car_SSIU_H
#define _R_Car_SSIU_H

#include <stdint.h>
#include <errno.h>
#include "ssiu_reg.h"

typedef enum
{
    SSI_CHANNEL_0,
    SSI_CHANNEL_NUM
} ssi_channel_t;

/* Comment to disable, uncomment to enable TDM split mode support */
#define RCAR_TDMSPLIT_SUPPORT   1

/* Comment to disable, uncomment to enable TDM ex-split mode support */
#define RCAR_TDMEXSPLIT_SUPPORT 1

#define SSI_SUBCHANNEL_MASK(ssi_subchan) (1 << ssi_subchan)

typedef enum
{
    SSI_SUB_CHANNEL_0,
    SSI_SUB_CHANNEL_1,
    SSI_SUB_CHANNEL_2,
    SSI_SUB_CHANNEL_3,
    SSI_SUB_CHANNEL_4,
    SSI_SUB_CHANNEL_5,
    SSI_SUB_CHANNEL_6,
    SSI_SUB_CHANNEL_7,
    SSI_SUB_CHANNEL_NUM
} ssi_sub_channel_t;

typedef enum
{
    SSI_OP_MODE_MONO,
    SSI_OP_MODE_STEREO,
    SSI_OP_MODE_TDM,
    SSI_OP_MODE_TDMEXT,
    SSI_OP_MODE_TDMSPLIT_4XMONO,
    SSI_OP_MODE_TDMSPLIT_4XSTEREO,
    SSI_OP_MODE_TDMEXSPLIT_8CHAN,
    SSI_OP_MODE_TDMEXSPLIT_16CHAN,
    SSI_OP_MODE_TDM_16CHAN,  /* particular case of TDM mode, requires SSI_MODE2_EX_FUNC_ENABLE to be set */
    SSI_OP_MODE_NUM,
    SSI_OP_MODE_TDMEXSPLIT = SSI_OP_MODE_TDMEXSPLIT_8CHAN, /* default case for tdm exsplit */
    SSI_OP_MODE_TDMSPLIT = SSI_OP_MODE_TDMSPLIT_4XSTEREO   /* default case for tdm split */
} ssi_op_mode_t;

typedef enum
{
    SSI_MS_MODE_SLAVE = 0,    /* SSI is configured as slave */
    SSI_MS_MODE_MASTER,       /* SSI is configured as master */
    SSI_MS_MODE_NUM
} ssi_masterslave_mode_t;

/* Used to configure the details of a particular protocol used for streaming audio */
typedef struct
{
    ssi_bit_clk_pol_t clk_pol;
    ssi_ws_pol_t ws_pol;
    ssi_bit_delay_t bit_delay;
    ssi_sys_word_len_t sys_word_length;   // number of bits in a system word
    ssi_data_word_len_t data_word_length;  // number of bits in a data word
    ssi_padding_pol_t padding_pol;  // serial padding polarity
    ssi_serial_data_align_t serial_data_alignment; // order of data and padding bits
} ssi_config_t;

typedef struct
{
    uint32_t dma;
    uint32_t underflow;
    uint32_t overflow;
    uint32_t idle_mode;
    uint32_t data;
} ssi_interrupt_t;

/* Setup of one or more SSI channels operating independently and set-up in pin sharing mode:
   is_transmit                - indicates whether ssi_channel is used for transmit or receive
   op_mode                    - operation mode to use
   masterslave_mode           - if set to SSI_MASTER_SLAVE, configure sharedclk_ssi_channel as master, and the
                                ssi_channel value that doesn't match sharedclk_ssi_channel as slave,
                                else configure sharedclk_ssi_channel and ssi_channel as slave
   ssi_busif_mask             - SSI channels mask indicating SSI channels using BUSIF mode
   bitnum                     - num bits per sample
   ssi_voices                 - num voices used at SSI level
   config                     - audio format config
   tdm_exsplit_map            - subchannel map for TDM exsplit mode - 32 bit entry has eight bit fields of four bits each,
                                indicating the number of voices (2-10) per subchannel for the eight subchannels of the SSI
*/
int ssiu_setup
(
    uint32_t is_transmit,
    ssi_op_mode_t op_mode,
    ssi_masterslave_mode_t masterslave_mode,
    uint32_t bitnum,
    uint32_t ssi_voices,
    ssi_config_t* config,
    uint32_t tdm_exsplit_map
);

/* Cleanup of SSI */
void ssiu_ssi_cleanup(void);

int ssiu_init(void);
int ssiu_deinit(void);

/* Start SSI operation - this includes enabling the DMA and interrupts and setting the
   SSICR.EN bit */
int ssi_start(void);
/* Stop SSI operation */
int ssi_stop(void);
/* Stop SSI idle interrupt */
int ssi_stop_idle_int(void);
/* wait for status bit to clear while stopping SSI operation */
int ssi_wait_status(uint32_t bitmask);

/* Start busif for SSI and subchannel */
int ssiu_busif_start(ssi_sub_channel_t ssi_sub_channel);

/* Stop busif for SSI and subchannel */
int ssiu_busif_stop(ssi_sub_channel_t ssi_sub_channel);

/* Cleanup busif for a SSI, all subchannels */
int ssiu_busif_cleanup(void);

/* Configure SSI clock divisor */
int ssi_set_divisor(uint32_t divisor);

/* Register dump functions */
void ssiu_common_register_dump(void);
void ssiu_ssi_register_dump(void);

#endif /* _R_Car_SSIU_H */

