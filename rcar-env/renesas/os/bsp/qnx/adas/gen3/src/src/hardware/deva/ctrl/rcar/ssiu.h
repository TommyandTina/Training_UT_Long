/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
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

#ifndef _R_Car_SSIU_H
#define _R_Car_SSIU_H

#include <stdint.h>
#include <errno.h>
#include "ssiu_reg.h"

typedef enum
{
    SSI_CHANNEL_0,
    SSI_CHANNEL_1,
    SSI_CHANNEL_2,
    SSI_CHANNEL_3,
    SSI_CHANNEL_4,
    SSI_CHANNEL_5,
    SSI_CHANNEL_6,
    SSI_CHANNEL_7,
    SSI_CHANNEL_8,
    SSI_CHANNEL_9,
    SSI_CHANNEL_NUM
} ssi_channel_t;

#define SSI_CHANNEL_MASK_0  (1 << SSI_CHANNEL_0)
#define SSI_CHANNEL_MASK_1  (1 << SSI_CHANNEL_1)
#define SSI_CHANNEL_MASK_2  (1 << SSI_CHANNEL_2)
#define SSI_CHANNEL_MASK_3  (1 << SSI_CHANNEL_3)
#define SSI_CHANNEL_MASK_4  (1 << SSI_CHANNEL_4)
#define SSI_CHANNEL_MASK_5  (1 << SSI_CHANNEL_5)
#define SSI_CHANNEL_MASK_6  (1 << SSI_CHANNEL_6)
#define SSI_CHANNEL_MASK_7  (1 << SSI_CHANNEL_7)
#define SSI_CHANNEL_MASK_8  (1 << SSI_CHANNEL_8)
#define SSI_CHANNEL_MASK_9  (1 << SSI_CHANNEL_9)
#define SSI_CHANNEL_MASK_01  (SSI_CHANNEL_MASK_0 | SSI_CHANNEL_MASK_1)
#define SSI_CHANNEL_MASK_02  (SSI_CHANNEL_MASK_0 | SSI_CHANNEL_MASK_2)
#define SSI_CHANNEL_MASK_03  (SSI_CHANNEL_MASK_0 | SSI_CHANNEL_MASK_3)
#define SSI_CHANNEL_MASK_09  (SSI_CHANNEL_MASK_0 | SSI_CHANNEL_MASK_9)
#define SSI_CHANNEL_MASK_012  (SSI_CHANNEL_MASK_01 | SSI_CHANNEL_MASK_2)
#define SSI_CHANNEL_MASK_0123  (SSI_CHANNEL_MASK_012 | SSI_CHANNEL_MASK_3)
#define SSI_CHANNEL_MASK_0129  (SSI_CHANNEL_MASK_012 | SSI_CHANNEL_MASK_9)
#define SSI_CHANNEL_MASK_34  (SSI_CHANNEL_MASK_3 | SSI_CHANNEL_MASK_4)
#define SSI_CHANNEL_MASK_39  (SSI_CHANNEL_MASK_3 | SSI_CHANNEL_MASK_9)
#define SSI_CHANNEL_MASK_349  (SSI_CHANNEL_MASK_34 | SSI_CHANNEL_MASK_9)
#define SSI_CHANNEL_MASK_78  (SSI_CHANNEL_MASK_7 | SSI_CHANNEL_MASK_8)
#define SSI_CHANNEL_MASK_ALL \
    (SSI_CHANNEL_MASK_0129 | SSI_CHANNEL_MASK_34 | SSI_CHANNEL_MASK_5 | \
    SSI_CHANNEL_MASK_6 | SSI_CHANNEL_MASK_78)

#define SSI_CHANNEL_MASK(ssi_chan) (1 << ssi_chan)

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
    SSI_HDMI_CHANNEL_0,
    SSI_HDMI_CHANNEL_1,
    SSI_HDMI_CHANNEL_NUM
} ssi_hdmi_channel_t;

typedef enum
{
    SSI_OP_MODE_MONO,
    SSI_OP_MODE_STEREO,
    SSI_OP_MODE_MULTICH,
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
    SSI_MS_MODE_ALL_SLAVE = 0,      /* all SSIs controlled by this driver instance are configured as slave */
    SSI_MS_MODE_MASTER_SLAVE,        /* one of the SSIs controlled by this driver instance is configured as master */
    SSI_MS_MODE_NUM
} ssi_masterslave_mode_t;

typedef enum
{
    SSI_INDEPENDENT_START = 0, /* independent start via SSICR.EN */
    SSI_SYNC_SSI0129_START,    /* synchronized start of SSI0,1,2,9 via SSI_CONTROL.SSI0129 */
    SSI_SYNC_SSI34_START       /* synchronized start of SSI3,4 via SSI_CONTROL.SSI34 */
} ssi_start_mode_t;

typedef enum
{
    SSI_INDEPENDENT_TRANSFER,  /* independent transfer not involving the SSIU BUSIF */
    SSI_BUSIF_TRANSFER         /* transfer involving the SSIU BUSIF */
} ssi_transfer_mode_t;

typedef enum
{
    SSI_INDEPENDENT_PINS, // SCK and WS pins are independent
    SSI_SHARED_PINS       // SCK and WS pins are shared across SSIs
} ssi_pin_mode_t;

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

int ssiu_init(void);
int ssiu_deinit(void);

/* common SSIU level setup of one or more SSI channels operating independently and set-up in pin sharing mode:
   ssi_channel_mask           - SSI channel mask indicating SSI channels to use
   busif_ssi_channel_mask     - SSI channel mask indicating SSI channels using BUSIF mode
   masterslave_mode           - if set to SSI_MASTER_SLAVE, configure sharedclk_ssi_channel as master, and the
                                ssi_channel_rx/ssi_channel_tx value that doesn't match sharedclk_ssi_channel as slave,
                                else configure ssi_channel_tx and ssi_channel_rx both as slave
   bitnum                     - num bits per sample
   voicenum                   - num voices
   voicenum_ext               - num voices on the external serial sound interface (SSI) bus, different
                                from voicenum only when extended TDM mode is used
   hdmi_channel
   config                     - audio format config
   tdm_exsplit_map            - subchannel map for TDM exsplit mode - array of SSI_CHANNEL_NUM entries indicating
                                for each SSI how its subchannels are used for exsplit TDM mode, each 32 bit entry
                                has eight bit fields of four bits each, indicating the number of voices (2-10)
                                per subchannel for the eight subchannels of the SSI
*/
int ssiu_indepchannel_common_setup
(
    uint32_t ssi_chan_mask,
    uint32_t busif_ssi_chan_mask,
    ssi_masterslave_mode_t masterslave_mode,
    ssi_start_mode_t start_mode
);

/* setup of one or more SSI channels operating independently and set-up in pin sharing mode:
   ssi_channel                - SSI channel mask indicating SSI channels to use
   sharedclk_ssi_channel      - SSI channel providing shared clock if pin sharing is used, SSI_CHANNEL_NUM otherwise
   is_transmit                - indicates whether ssi_channel is used for transmit or receive
   op_mode                    - operation mode to use
   masterslave_mode           - if set to SSI_MASTER_SLAVE, configure sharedclk_ssi_channel as master, and the
                                ssi_channel value that doesn't match sharedclk_ssi_channel as slave,
                                else configure sharedclk_ssi_channel and ssi_channel as slave
   ssi_busif_mask             - SSI channels mask indicating SSI channels using BUSIF mode
   start_mode
   bitnum                     - num bits per sample
   ssi_voices                 - num voices used at SSI level
   hdmi_channel
   config                     - audio format config
   tdm_exsplit_map            - subchannel map for TDM exsplit mode - 32 bit entry has eight bit fields of four bits each,
                                indicating the number of voices (2-10) per subchannel for the eight subchannels of the SSI
*/
int ssiu_indepchannel_setup
(
    uint32_t ssi_channel,
    uint32_t sharedclk_ssi_channel,
    uint32_t is_transmit,
    uint32_t uses_busif,
    ssi_op_mode_t op_mode,
    ssi_masterslave_mode_t masterslave_mode,
    ssi_start_mode_t start_mode,
    uint32_t bitnum,
    uint32_t ssi_voices,
    ssi_hdmi_channel_t hdmi_channel,
    ssi_config_t* config,
    uint32_t tdm_exsplit_map
);

void ssiu_indepchannel_common_cleanup ( uint32_t ssi_channel_mask );

/* cleanup of one or more SSI channels operating independently and set-up in pin sharing mode:
   ssi_channel                - SSI channel used
 */
void ssiu_indepchannel_cleanup
(
    uint32_t ssi_channel
);

/* multi-channel set-up: SSI0,1,2 or SSI0,1,2,9 grouped together for multi-channel operation
   ssi_channel_mask   - SSI channel mask for SSI channels being grouped together
   is_transmit        - indicates whether this mode is used for transmit or receive
   masterslave_mode   - shows whether SSI0 is configured as master
   bitnum             - num bits per sample
   hdmi_channel       - HDMI channel to which the multi channel output is routed to, or HDMI_CHANNEL_NUM
                        if output not routed to HDMI i/f module, ignored in receive mode
   config             - audio format config
*/
int ssiu_multichannel_setup
(
    uint32_t ssi_channel_mask,
    uint32_t is_transmit,
    ssi_masterslave_mode_t masterslave_mode,
    uint32_t bitnum,
    ssi_hdmi_channel_t hdmi_channel,
    ssi_config_t* config
);

/* multi-channel clean-up: SSI0,1,2 or SSI0,1,2,9 grouped together for multi-channel operation
   ssi_channel_mask   - SSI channel mask for SSI channels being grouped together
*/
void ssiu_multichannel_cleanup(uint32_t ssi_channel_mask);

/* start SSI operation - this includes enabling the DMA and interrupts and setting the
   SSICR.EN bit; in case of SSI3,4 or SSI0,1,2,9 synchronized start, the setting of the
   SSICR.EN bit is not included */
int ssi_start(ssi_channel_t ssi_channel, ssi_start_mode_t start_mode);
/* stop SSI operation */
int ssi_stop(ssi_channel_t ssi_channel);
/* stop SSI idle interrupt */
int ssi_stop_idle_int(ssi_channel_t ssi_channel);
/* wait for status bit to clear while stopping SSI operation */
int ssi_wait_status(ssi_channel_t ssi_channel, uint32_t bitmask);

/* synchronized start for SSI0,1,2 or SSI0,1,2,9 or SSI3,4 */
int ssiu_start(ssi_start_mode_t start_mode);

/* synchronized stop for SSI0,1,2 or SSI0,1,2,9 or SSI3,4 */
int ssiu_stop(ssi_start_mode_t start_mode);

/* start busif for a given SSI channel and subchannel */
int ssiu_busif_start(ssi_channel_t ssi_channel, ssi_sub_channel_t ssi_sub_channel);

/* stop busif for a given SSI channel and subchannel */
int ssiu_busif_stop(ssi_channel_t ssi_channel, ssi_sub_channel_t ssi_sub_channel);

/* cleanup busif for a given SSI channel, all subchannels */
int ssiu_busif_cleanup(ssi_channel_t ssi_channel);

/* configure SSI clock divisor for specific SSI channel */
int ssi_set_divisor(uint32_t ssi_channel, uint32_t divisor);

/* register dump functions */
void ssiu_common_register_dump(void);
void ssiu_ssi_register_dump(ssi_channel_t ssi_channel);

#endif /* _R_Car_SSIU_H */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/ssiu.h $ $Rev: 911285 $")
#endif
