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

#ifndef _R_Car_SSIU_REG_H
#define _R_Car_SSIU_REG_H

#include <errno.h>
#include <stdint.h>
#include <aarch64/r-car.h>

char* ssi_reg;
char* ssiu_reg_map;

/*
 * Bit field mappings for SSIq_BUSIF_MODE register
 *
 * 21-31:        Not used:  set to 0
 * 20:           SFT_DIR:   bit-shift direction for valid bit position adjustment: default=0 (shift to left)
 * 16,17,18,19:  SFT_NUM:   bit-shift count for valid bit position adjustment: default=0 (no shift)
 * 9-15:         Not used:  set to 0
 * 8:            WORD_SWAP: enable word swap for 16 bit data: default=0 (no word swap)
 * 1-7:          Not used:  set to 0
 * 0:            DMA        only allowed value is 1: DMA access
 *
 */
#define SSI_BUSIF_MODE_SFTDIR_MASK      (0x1<<20)
#define SSI_BUSIF_MODE_SFTDIR_LEFT      0
#define SSI_BUSIF_MODE_SFTDIR_RIGHT     (0x1<<20)
#define SSI_BUSIF_MODE_SFTNUM_MASK      (0xF<<16)
#define SSI_BUSIF_MODE_WORDSWAP_MASK    (0x1<<8)
#define SSI_BUSIF_MODE_WORDSWAP_ENABLE  (0x1<<8)
#define SSI_BUSIF_MODE_WORDSWAP_DISABLE 0
#define SSI_BUSIF_MODE_DMA_MASK         0x1
#define SSI_BUSIF_MODE_DMA_ENABLE       0x1

/*
 * Bit field mappings for SSIq_BUSIF_ADINR register
 *
 * 21-31:          Not used:  set to 0
 * 16,17,18,19,20: OTBL:  output data bit length: default=0 (24 bits)
 * 5-15:           Not used:  set to 0
 * 0-4:            CHNUM: channel number: default=0 (none)
 *
 */
#define SSI_BUSIF_ADINR_OTBL_MASK     (0x1F<<16)
#define SSI_BUSIF_ADINR_OTBL_24BITS   0
#define SSI_BUSIF_ADINR_OTBL_22BITS   (0x2<<16)
#define SSI_BUSIF_ADINR_OTBL_20BITS   (0x4<<16)
#define SSI_BUSIF_ADINR_OTBL_18BITS   (0x6<<16)
#define SSI_BUSIF_ADINR_OTBL_16BITS   (0x8<<16)
#define SSI_BUSIF_ADINR_OTBL_8BITS    (0x10<<16)
#define SSI_BUSIF_ADINR_CHNUM_MASK    0x1F
#define SSI_BUSIF_ADINR_CHNUM_NONE    0
#define SSI_BUSIF_ADINR_CHNUM_ONE     1
#define SSI_BUSIF_ADINR_CHNUM_TWO     2
#define SSI_BUSIF_ADINR_CHNUM_FOUR    4
#define SSI_BUSIF_ADINR_CHNUM_SIX     6
#define SSI_BUSIF_ADINR_CHNUM_EIGHT   8
#define SSI_BUSIF_ADINR_CHNUM_TEN     10
#define SSI_BUSIF_ADINR_CHNUM_SIXTEEN 16

/*
 * Bit field mappings for SSIq-MODE register
 *
 * 14-31:   Not used:  set to 0
 * 13:      FS_MODE:   select fs for TDM split mode: 0=256fs (stereox4), 1=128fs (monauralx4), default=0
 * 9-12:    Not used:  set to 0
 * 8:       TDM_SPLIT: enables TDM split mode when set to 1, default=0
 * 1-7:     Not used:  set to 0
 * 0:       TDM_EXT:   enables TDM extended mode when set to 1, default=0
 *
 */
#define SSI_MODE_FSMODE_MASK    (0x1<<13)
#define SSI_MODE_FSMODE_DISABLE (0x0<<13)
#define SSI_MODE_FSMODE_ENABLE  (0x1<<13)
#define SSI_MODE_TDMSPLIT_MASK    (0x1<<8)
#define SSI_MODE_TDMSPLIT_DISABLE (0x0<<8)
#define SSI_MODE_TDMSPLIT_ENABLE  (0x1<<8)
#define SSI_MODE_TDMEXT_MASK    0x1
#define SSI_MODE_TDMEXT_DISABLE 0
#define SSI_MODE_TDMEXT_ENABLE  0x1

/*
 * Bit field mappings for SSIq-CONTROL register
 *
 * 29-31:   Not used:  set to 0
 * 28:      START_7:   start/stop subchannel 7: stop=0, start=1, default=0
 * 25-27:   Not used:  set to 0
 * 24:      START_6:   start/stop subchannel 6: stop=0, start=1, default=0
 * 21-23:   Not used:  set to 0
 * 20:      START_5:   start/stop subchannel 5: stop=0, start=1, default=0
 * 17-19:   Not used:  set to 0
 * 16:      START_4:   start/stop subchannel 4: stop=0, start=1, default=0
 * 13-15:   Not used:  set to 0
 * 12:      START_3:   start/stop subchannel 3: stop=0, start=1, default=0
 * 9-11:    Not used:  set to 0
 * 8:       START_2:   start/stop subchannel 2: stop=0, start=1, default=0
 * 5-7:     Not used:  set to 0
 * 4:       START_1:   start/stop subchannel 1: stop=0, start=1, default=0
 * 1-3:     Not used:  set to 0
 * 0:       START_0:   start/stop subchannel 0, or the actual channel: stop=0, start=1, default=0
 *
 */
#define SSI_CONTROL_START_MASK(subchan)   (0x1<<(subchan<<2))
#define SSI_CONTROL_START_START(subchan)  (0x1<<(subchan<<2))
#define SSI_CONTROL_START_STOP(subchan)   0

/*
 * Bit field mappings for SSIq-STATUS register
 *
 * 30-31:   Not used:  read as 0
 * 29:      FCST:      state of WS signal: WS signal stopped=1, default=0
 * 28:      DTST:      state of frequency switching: frequency switching detected=1, default=0
 * 27:      UIRQ:      underflow indication: underflow occured=1, default=0
 * 26:      OIRQ:      overflow indication: overflow occured=1, default=0
 * 25:      IIRQ:      interrupt indication: idle state(?)=1, default=0
 * 24:      DIRQ:      unread data indication: unread data available=1, default=0
 * 16-23:   Not used:  read as 0
 * 15:      UF_3:      buffer underflow in subchannel 3
 * 14:      UF_2:      buffer underflow in subchannel 2
 * 13:      UF_1:      buffer underflow in subchannel 1
 * 12:      UF_0:      buffer underflow in subchannel 0
 * 11:      OF_3:      buffer overflow in subchannel 3
 * 10:      OF_2:      buffer overflow in subchannel 2
 * 9:       OF_1:      buffer overflow in subchannel 1
 * 8:       OF_0:      buffer overflow in subchannel 0
 * 0-7:     Not used:  read as 0
 *
 */
#define SSI_STATUS_FCST_MASK (0x1<<29)
#define SSI_STATUS_DTST_MASK (0x1<<28)
#define SSI_STATUS_UIRQ_MASK (0x1<<27)
#define SSI_STATUS_OIRQ_MASK (0x1<<26)
#define SSI_STATUS_IIRQ_MASK (0x1<<25)
#define SSI_STATUS_DIRQ_MASK (0x1<<24)
/* below macros valid for subchan=0..3 */
#define SSI_STATUS_UF_MASK(subchan) (0x1 << (subchan+12))
#define SSI_STATUS_OF_MASK(subchan) (0x1 << (subchan+8))

/*
 * Bit field mappings for SSIq_INT_ENABLE_MAIN
 * 30-31:   Not used:  set as 0
 * 29:      FCST_IE:   FCST interrupt enable: enable=1, disable=0, default=0
 * 28:      DTST_IE:   DTST interrupt enable: enable=1, disable=0, default=0
 * 27:      UIRQ_IE:   UIRQ interrupt enable: enable=1, disable=0, default=0
 * 26:      OIRQ_IE:   OIRQ interrupt enable: enable=1, disable=0, default=0
 * 25:      IIRQ_IE:   IIRQ interrupt enable: enable=1, disable=0, default=0
 * 24:      DIRQ_IE:   DIRQ interrupt enable: enable=1, disable=0, default=0
 * 16-23:   Not used:  set as 0
 * 15:      UF_3_IE:   UF_3 interrupt enable: enable=1, disable=0, default=0
 * 14:      UF_2_IE:   UF_2 interrupt enable: enable=1, disable=0, default=0
 * 13:      UF_1_IE:   UF_1 interrupt enable: enable=1, disable=0, default=0
 * 12:      UF_0_IE:   UF_0 interrupt enable: enable=1, disable=0, default=0
 * 11:      OF_3_IE:   OF_3 interrupt enable: enable=1, disable=0, default=0
 * 10:      OF_2_IE:   OF_2 interrupt enable: enable=1, disable=0, default=0
 * 9:       OF_1_IE:   OF_1 interrupt enable: enable=1, disable=0, default=0
 * 8:       OF_0_IE:   OF_0 interrupt enable: enable=1, disable=0, default=0
 * 0-7:     Not used:  read as 0
 *
 */
#define SSI_INT_ENABLE_MAIN_FCST_MASK    (0x1<<29)
#define SSI_INT_ENABLE_MAIN_FCST_ENABLE  (0x1<<29)
#define SSI_INT_ENABLE_MAIN_FCST_DISABLE 0
#define SSI_INT_ENABLE_MAIN_DTST_MASK    (0x1<<28)
#define SSI_INT_ENABLE_MAIN_DTST_ENABLE  (0x1<<28)
#define SSI_INT_ENABLE_MAIN_DTST_DISABLE 0
#define SSI_INT_ENABLE_MAIN_UIRQ_MASK    (0x1<<27)
#define SSI_INT_ENABLE_MAIN_UIRQ_ENABLE  (0x1<<27)
#define SSI_INT_ENABLE_MAIN_UIRQ_DISABLE 0
#define SSI_INT_ENABLE_MAIN_OIRQ_MASK    (0x1<<26)
#define SSI_INT_ENABLE_MAIN_OIRQ_ENABLE  (0x1<<26)
#define SSI_INT_ENABLE_MAIN_OIRQ_DISABLE 0
#define SSI_INT_ENABLE_MAIN_IIRQ_MASK    (0x1<<25)
#define SSI_INT_ENABLE_MAIN_IIRQ_ENABLE  (0x1<<25)
#define SSI_INT_ENABLE_MAIN_IIRQ_DISABLE 0
#define SSI_INT_ENABLE_MAIN_DIRQ_MASK    (0x1<<24)
#define SSI_INT_ENABLE_MAIN_DIRQ_ENABLE  (0x1<<24)
#define SSI_INT_ENABLE_MAIN_DIRQ_DISABLE 0
/* below macros valid for subchan=0..3 */
#define SSI_INT_ENABLE_MAIN_UF_MASK(subchan)    (0x1 << (subchan+12))
#define SSI_INT_ENABLE_MAIN_UF_ENABLE(subchan)  (0x1 << (subchan+12))
#define SSI_INT_ENABLE_MAIN_UF_DISABLE(subchan) 0
#define SSI_INT_ENABLE_MAIN_OF_MASK(subchan)    (0x1 << (subchan+8))
#define SSI_INT_ENABLE_MAIN_OF_ENABLE(subchan)  (0x1 << (subchan+8))
#define SSI_INT_ENABLE_MAIN_OF_DISABLE(subchan) 0

/*
 * Bit field mappings for SSIq_MODE2 register
 *
 * 1-31:    Not used:  set to 0
 * 0:       EX_FUNC:   Extended Functionality enable: enable = 1, disable = 0, default = 0
 *
 */
#define SSI_MODE2_EX_FUNC_MASK    (0x1)
#define SSI_MODE2_EX_FUNC_DISABLE 0
#define SSI_MODE2_EX_FUNC_ENABLE  (0x1)

/*
 * Bit field mappings for SSIq_STATUS2 register
 *
 * 16-31:   Not used:  read as 0
 * 15:      UF_7:      buffer underflow in subchannel 7
 * 14:      UF_6:      buffer underflow in subchannel 6
 * 13:      UF_5:      buffer underflow in subchannel 5
 * 12:      UF_4:      buffer underflow in subchannel 4
 * 11:      OF_7:      buffer overflow in subchannel 7
 * 10:      OF_6:      buffer overflow in subchannel 6
 * 9:       OF_5:      buffer overflow in subchannel 5
 * 8:       OF_4:      buffer overflow in subchannel 4
 * 0-7:     Not used:  read as 0
 *
 */

/* Below macros valid for subchan=4..7 */
#define SSI_STATUS2_UF_MASK(subchan)    (0x1 << (subchan+8))
#define SSI_STATUS2_OF_MASK(subchan)    (0x1 << (subchan+4))

/*
 * Bit field mappings for SSIq_INT_ENABLE_MAIN2
 * 16-31:   Not used:  set as 0
 * 15:      UF_7_IE:   UF_7 interrupt enable: enable=1, disable=0, default=0
 * 14:      UF_6_IE:   UF_6 interrupt enable: enable=1, disable=0, default=0
 * 13:      UF_5_IE:   UF_5 interrupt enable: enable=1, disable=0, default=0
 * 12:      UF_4_IE:   UF_4 interrupt enable: enable=1, disable=0, default=0
 * 11:      OF_7_IE:   OF_7 interrupt enable: enable=1, disable=0, default=0
 * 10:      OF_6_IE:   OF_6 interrupt enable: enable=1, disable=0, default=0
 * 9:       OF_5_IE:   OF_5 interrupt enable: enable=1, disable=0, default=0
 * 8:       OF_4_IE:   OF_4 interrupt enable: enable=1, disable=0, default=0
 * 0-7:     Not used:  read as 0
 *
 */

/* Below macro valid for subchan=4..7 */
#define SSI_INT_ENABLE_MAIN2_UF_MASK(subchan)    (0x1 << (subchan+8))
#define SSI_INT_ENABLE_MAIN2_UF_ENABLE(subchan)  (0x1 << (subchan+8))
#define SSI_INT_ENABLE_MAIN2_UF_DISABLE(subchan) 0
#define SSI_INT_ENABLE_MAIN2_OF_MASK(subchan)    (0x1 << (subchan+4))
#define SSI_INT_ENABLE_MAIN2_OF_ENABLE(subchan)  (0x1 << (subchan+4))
#define SSI_INT_ENABLE_MAIN2_OF_DISABLE(subchan) 0

/*
 * Enum values that relate to SSICRn register
 */

// Defines on what edge of the bit clk data is sampled
typedef enum
{
    SSI_BIT_CLK_POL_RISING = 0,
    SSI_BIT_CLK_POL_FALLING
} ssi_bit_clk_pol_t;

// Defines on what edge of the bit clk data is sampled
typedef enum
{
    SSI_WS_POL_0 = 0,
    SSI_WS_POL_1
} ssi_ws_pol_t;

typedef enum
{
    SSI_BIT_DELAY_ONE = 0,
    SSI_BIT_DELAY_NONE
} ssi_bit_delay_t;

typedef enum
{
    /* mono values */
    SSI_SYS_WORD_LEN_16BIT_MONO = 0,
    SSI_SYS_WORD_LEN_32BIT_MONO,
    SSI_SYS_WORD_LEN_48BIT_MONO,
    SSI_SYS_WORD_LEN_64BIT_MONO,
    SSI_SYS_WORD_LEN_96BIT_MONO,
    SSI_SYS_WORD_LEN_128BIT_MONO,
    SSI_SYS_WORD_LEN_256BIT_MONO,
    SSI_SYS_WORD_LEN_512BIT_MONO,
    SSI_SYS_WORD_LEN_NOSUP_MONO,
    /* stereo/multi channel values */
    SSI_SYS_WORD_LEN_8BIT_STEREO = 0,
    SSI_SYS_WORD_LEN_16BIT_STEREO,
    SSI_SYS_WORD_LEN_24BIT_STEREO,
    SSI_SYS_WORD_LEN_32BIT_STEREO,
    SSI_SYS_WORD_LEN_48BIT_STEREO,
    SSI_SYS_WORD_LEN_64BIT_STEREO,
    SSI_SYS_WORD_LEN_128BIT_STEREO,
    SSI_SYS_WORD_LEN_256BIT_STEREO,
    SSI_SYS_WORD_LEN_NOSUP_STEREO
} ssi_sys_word_len_t;

typedef enum
{
   SSI_DATA_WORD_LEN_8BIT = 0,
   SSI_DATA_WORD_LEN_16BIT,
   SSI_DATA_WORD_LEN_18BIT,
   SSI_DATA_WORD_LEN_20BIT,
   SSI_DATA_WORD_LEN_22BIT,
   SSI_DATA_WORD_LEN_24BIT,
   SSI_DATA_WORD_LEN_32BIT
} ssi_data_word_len_t;

typedef enum
{
   SSI_PARALLEL_DATA_ALIGN_LEFT = 0,
   SSI_PARALLEL_DATA_ALIGN_RIGHT
} ssi_parallel_data_align_t;

typedef enum
{
   SSI_SER_DATA_ALIGN_DATA_FIRST = 0,
   SSI_SER_DATA_ALIGN_PADDING_FIRST = 1
} ssi_serial_data_align_t;

typedef enum
{
   SSI_PADDING_POL_LOW = 0,
   SSI_PADDING_POL_HIGH
} ssi_padding_pol_t;

/*
 * Bit field mappings for SSICRn register
 *
 * 31:       FORCE      Fixed to the value 1; default value=1
 * 30:       Not Used   Set to 0
 * 29:       FIEN       Frequency Switching Detection Interrupt Enable setting - enable=1, disable=0, default=0
 * 28:       DMEN       DMA enable setting - enable=1, disable=0, default=0
 * 27:       UIEN       Underflow IRQ enable setting - enable=1, disable=0, default=0
 * 26:       OIEN       Overflow IRQ enable setting - enable=1, disable=0, default=0
 * 25:       IIEN       Idle Mode IRQ enable setting - enable=1, disable=0, default=0
 * 24:       DIEN       Data IRQ enable setting - enable=1, disable=0, default=0
 * 22,23:    CHNL       Channels
 * 19,20,21: DWL        Data Word Length, default value=3 (32 bits)
 * 16,17,18: SWL        System Word Length, default value=3 (32 bits)
 * 15:       SCKD       Serial Clock Direction - SCK is input/slave mode=0, WS is output/master mode=1, default=0
 * 14:       SWSD       Serial WS Direction - WS is input/slave mode=0, WS is output/master mode=1, default=0
 * 13:       SCKP       Serial Clock Polarity - rising edge=0, falling edge=1, default=0
 * 12:       SWSP       Serial WS Polarity - meaning of 0,1 values is context dependent
 * 11:       SPDP       Serial Padding Polarity - padding bits are zero/low=0, one/high=1, default=0
 * 10:       SDTA       Serial Data Alignment - serial data followed by padding bits=0,
 *                                              padding bits followed by serial data=1, default=0
 * 9:        PDTA       Parallel Data Alignment - left alignment=0, right alignment=1, default=0
 * 8:        DEL        Serial Data Delay - no delay=0, 1 clock cycle delay=1, default=0
 * 7:        Not used   Set to 0
 * 4,5,6:    CKDV       Serial Oversample Clock Divide Ratio, default=3 (CKDV_8)
 * 3:        MUEN       Mute setting - mute=1, unmute=0, default=0
 * 2:        Not used   Set to 0
 * 1:        TRMD       Transmit/receive mode, transmit=1, receive=0, default=1
 * 0:        EN         SSI Enable setting - enable=1, disable=0, default=0
 */
#define SSICR_EN_MASK             0x1
#define SSICR_EN_ENABLE           0x1
#define SSICR_EN_DISABLE          0
#define SSICR_TRMD_MASK           (0x1<<1)
#define SSICR_TRMD_TX_MODE        (0x1<<1)
#define SSICR_TRMD_RX_MODE        0
#define SSICR_MUEN_MASK           (0x1<<3)
#define SSICR_MUEN_MUTED          (0x1<<3)
#define SSICR_MUEN_UNMUTED        0
#define SSICR_CKDV_MASK           (0x7<<4)
#define SSICR_CKDV_1              0
#define SSICR_CKDV_2              (0x1<<4)
#define SSICR_CKDV_4              (0x2<<4)
#define SSICR_CKDV_8              (0x3<<4)
#define SSICR_CKDV_16             (0x4<<4)
#define SSICR_CKDV_6              (0x5<<4)
#define SSICR_CKDV_12             (0x6<<4)
#define SSICR_DEL_MASK            (0x1<<8)
#define SSICR_DEL_1CYCLE          (SSI_BIT_DELAY_ONE<<8)
#define SSICR_DEL_NONE            (SSI_BIT_DELAY_NONE<<8)
#define SSICR_PDTA_MASK           (0x1<<9)
#define SSICR_PDTA_LEFT           (SSI_PARALLEL_DATA_ALIGN_LEFT<<9)
#define SSICR_PDTA_RIGHT          (SSI_PARALLEL_DATA_ALIGN_RIGHT<<9)
#define SSICR_SDTA_MASK           (0x1<<10)
#define SSICR_SDTA_SDATA_FIRST    (SSI_SER_DATA_ALIGN_DATA_FIRST<<10)
#define SSICR_SDTA_PADDING_FIRST  (SSI_SER_DATA_ALIGN_PADDING_FIRST<<10)
#define SSICR_SPDP_MASK           (0x1<<11)
#define SSICR_SPDP_PADDING_LOW    (SSI_PADDING_POL_LOW<<11)
#define SSICR_SPDP_PADDING_HIGH   (SSI_PADDING_POL_HIGH<<11)
#define SSICR_SWSP_MASK           (0x1<<12)
#define SSICR_SWSP_0              (SSI_WS_POL_0<<12)  /* meaning of SWSP is context dependent */
#define SSICR_SWSP_1              (SSI_WS_POL_1<<12)  /* meaning of SWSP is context dependent */
#define SSICR_SCKP_MASK           (0x1<<13)
#define SSICR_SCKP_RISING         (SSI_BIT_CLK_POL_RISING<<13)
#define SSICR_SCKP_FALLING        (SSI_BIT_CLK_POL_FALLING<<13)
#define SSICR_SWSD_MASK           (0x1<<14)
#define SSICR_SWSD_INPUT          0
#define SSICR_SWSD_OUTPUT         (0x1<<14)
#define SSICR_SCKD_MASK           (0x1<<15)
#define SSICR_SCKD_INPUT          0
#define SSICR_SCKD_OUTPUT         (0x1<<15)
#define SSICR_SWL_MASK            (0x7<<16)
#define SSICR_SWL_8BIT            (SSI_SYS_WORD_LEN_8BIT_STEREO<<16)
#define SSICR_SWL_16BIT           (SSI_SYS_WORD_LEN_16BIT_STEREO<<16)
#define SSICR_SWL_24BIT           (SSI_SYS_WORD_LEN_24BIT_STEREO<<16)
#define SSICR_SWL_32BIT           (SSI_SYS_WORD_LEN_32BIT_STEREO<<16)
#define SSICR_SWL_48BIT           (SSI_SYS_WORD_LEN_48BIT_STEREO<<16)
#define SSICR_SWL_64BIT           (SSI_SYS_WORD_LEN_64BIT_STEREO<<16)
#define SSICR_SWL_128BIT          (SSI_SYS_WORD_LEN_128BIT_STEREO<<16)
#define SSICR_SWL_256BIT          (SSI_SYS_WORD_LEN_256BIT_STEREO<<16)
#define SSICR_SWL_16BIT_MONO      (SSI_SYS_WORD_LEN_16BIT_MONO<<16)
#define SSICR_SWL_32BIT_MONO      (SSI_SYS_WORD_LEN_32BIT_MONO<<16)
#define SSICR_SWL_48BIT_MONO      (SSI_SYS_WORD_LEN_48BIT_MONO<<16)
#define SSICR_SWL_64BIT_MONO      (SSI_SYS_WORD_LEN_64BIT_MONO<<16)
#define SSICR_SWL_96BIT_MONO      (SSI_SYS_WORD_LEN_96BIT_MONO<<16)
#define SSICR_SWL_128BIT_MONO     (SSI_SYS_WORD_LEN_128BIT_MONO<<16)
#define SSICR_SWL_256BIT_MONO     (SSI_SYS_WORD_LEN_256BIT_MONO<<16)
#define SSICR_SWL_512BIT_MONO     (SSI_SYS_WORD_LEN_512BIT_MONO<<16)
#define SSICR_DWL_MASK            (0x7<<19)
#define SSICR_DWL_8BIT            0
#define SSICR_DWL_16BIT           (0x1<<19)
#define SSICR_DWL_18BIT           (0x2<<19)
#define SSICR_DWL_20BIT           (0x3<<19)
#define SSICR_DWL_22BIT           (0x4<<19)
#define SSICR_DWL_24BIT           (0x5<<19)
#define SSICR_DWL_32BIT           (0x6<<19)
#define SSICR_CHNL_MASK           (0x3<<22)
#define SSICR_CHNL_1              0         /* system word=1 channel, or mono frame=1 system word */                     
#define SSICR_CHNL_2              (0x1<<22) /* system word=2 channel, or TDM frame=4 system words */
#define SSICR_CHNL_3              (0x2<<22) /* system word=3 channel, or TDM frame=6 system words */
#define SSICR_CHNL_4              (0x3<<22) /* system word=4 channel, or TDM frame=8 system words */
#define SSICR_DIEN_MASK           (0x1<<24)
#define SSICR_DIEN_DISABLE        0
#define SSICR_DIEN_ENABLE         (0x1<<24)
#define SSICR_IIEN_MASK           (0x1<<25)
#define SSICR_IIEN_DISABLE        0
#define SSICR_IIEN_ENABLE         (0x1<<25)
#define SSICR_OIEN_MASK           (0x1<<26)
#define SSICR_OIEN_DISABLE        0
#define SSICR_OIEN_ENABLE         (0x1<<26)
#define SSICR_UIEN_MASK           (0x1<<27)
#define SSICR_UIEN_DISABLE        0
#define SSICR_UIEN_ENABLE         (0x1<<27)
#define SSICR_DMEN_MASK           (0x1<<28)
#define SSICR_DMEN_DISABLE        0
#define SSICR_DMEN_ENABLE         (0x1<<28)
#define SSICR_FIEN_MASK           (0x1<<29)
#define SSICR_FIEN_DISABLE        0
#define SSICR_FIEN_ENABLE         (0x1<<29)
#define SSICR_FORCE_MASK          (0x1<<31)
#define SSICR_FORCE_FIXED         (0x1<<31)

/*
 * Bit field mappings for SSICREx register
 *
 * 1-31:      Not used
 * 0:         CHNL2:         Extension of the CHNL bitfield in SSICR, set to 1 only for 16 channel TDM mode
 *
 */
#define SSICRE_CHNL2_MASK         0x1

/*
 * Bit field mappings for SSISRn register
 *
 * 29-31:   Not used
 * 28:      DMRQ:      DMA Request Status Flag; DMA request active=1; inactive=0, default=0
 * 27:      UIRQ:      Underflow Interrupt Status Flag; underflow condition present=1, absent=0, default=0
 * 26:      OIRQ:      Overflow Interrupt Status Flag; overflow condition present=1, absent=0, default=0
 * 25:      IIRQ:      Idle Mode Interrupt Status Flag; idle state detected=1, not detected=0, default=0
 * 24:      DIRQ:      Data Interrupt Status Flag; data available to be read, or data needs to be written=1, otherwise=0, default=0
 * 4-23:    Not used
 * 2,3:     CHNO:      Current channel number that is being written to or read from (not applicable for 8 or 16 bit data)
 * 1:       SWNO:      Current system word number that is being written or read (not applicable for 8 or 16 bit data)
 * 0:       IDST:      Idle Mode Status Flag: serial bus activity stopped=1; otherwise=0; default=0
 *
 */
#define SSISR_IDST_MASK     0x1
#define SSISR_SWNO_MASK     (0x1<<1)
#define SSISR_SWNO_0        0
#define SSISR_SWNO_1        (0x1<<1)
#define SSISR_CHNO_MASK     (0x3<<2)
#define SSISR_CHNO_0        0
#define SSISR_CHNO_1        (0x1<<2)
#define SSISR_CHNO_2        (0x2<<2)
#define SSISR_CHNO_3        (0x3<<2)
#define SSISR_DIRQ_MASK     (0x1<<24)
#define SSISR_IIRQ_MASK     (0x1<<25)
#define SSISR_OIRQ_MASK     (0x1<<26)
#define SSISR_UIRQ_MASK     (0x1<<27)
#define SSISR_DMRQ_MASK     (0x1<<28)

/*
 * Bit field mappings for SSIWSRn register
 *
 * 21-31:   Not used     Set to 0
 * 16-20:   WIDTH:       Sync Pulse Width in clock cycles (1 to 31), 0 if TDM mode is used
 * 9-15:    Not used     Set to 0
 * 8:       CONT:        WS Continue function: enabled=1, disabled=0, default=0
 * 2-7:     Not used     Set to 0
 * 1:       MONO:        TDM/Monaural Format: TDM=0, Mono=1, default=0
 * 0:       WS_MODE:     WS Mode: Stereo, multichannel=0, TDM, Monaural=1, default=0
 *
 */
#define SSIWSR_WSMODE_MASK            0x1
#define SSIWSR_WSMODE_STEREO_MULTICH  0
#define SSIWSR_WSMODE_TDM_MONO        0x1
#define SSIWSR_MONO_MASK              (0x1<<1)
#define SSIWSR_MONO_TDM               0
#define SSIWSR_MONO_MONO              (0x1<<1)
#define SSIWSR_CONT_MASK              (0x1<<8)
#define SSIWSR_CONT_DISABLED          0
#define SSIWSR_CONT_ENABLED           (0x1<<8)
#define SSIWSR_WIDTH_MASK             (0x1F<<16)

/*
 * Bit field mappings for SSIFMRn register
 *
 * 22-31:   Not used     Set to 0
 * 16-21:   DTCT         Frequency Switching Detection Range
 * 6-15:    Not used     Set to 0
 * 4,5:     CTDV         BUs clock division ratio
 * 1-3:     Not used     Set to 0
 * 0:       FSEN         Frequency Switching Detection Enable - enable=1, disable=0, default=0
 *
 */
#define SSIFMR_DTCT_MASK     (0x3F<<16)
#define SSIFMR_CTDV_MASK     (0x3<<4)
#define SSIFMR_FSEN_MASK     0x1
#define SSIFMR_FSEN_DISABLE  0
#define SSIFMR_FSEN_ENABLE   0x1

/*
 * Bit field mappings for SSIFSRn register
 *
 * 16-31:   Not used     Read as 0
 * 15:      FCST         WS Stopped Status Flag - WS signal stopped=1, otherwise=0
 * 14:      DTST         Frequency Switching Detection Status - switching detected=1, otherwise=0
 * 12,13:   Not used     Read as 0
 * 0-11:    FCNT         Frequency Count Monitor
 *
 */
#define SSIFSR_FCST_MASK    (0x1<<15)
#define SSIFSR_DTST_MASK    (0x1<<14)
#define SSIFSR_FCNT_MASK    0xFFF

/* structure used for memory mapped SSI registers*/
typedef struct
{
    volatile uint32_t   cr;         /* Control register - SSICRn for n=0 */
    volatile uint32_t   sr;         /* Status register - SSISRn  for n=0 */
    volatile uint32_t   tdr;        /* Transmit data register - SSITDRnfor n=0 */
    volatile uint32_t   rdr;        /* Receive data register - SSIRDRn for n=0 */
    volatile uint32_t   dummy1[4];
    volatile uint32_t   wsr;        /* WS mode register - SSIWSRn for n=0 */
    volatile uint32_t   fmr;        /* FS mode register - SSIFMRn for n=0 */
    volatile uint32_t   fsr;        /* FS status register - SSIFSRn for n=0 */
} ssi_reg_t;

typedef struct
{
    volatile uint32_t   cr;         /* Control register - SSICRn for n=0*/
    volatile uint32_t   sr;         /* Status register - SSISRn for n=0*/
    volatile uint32_t   tdr;        /* Transmit data register - SSITDRn for n=0*/
    volatile uint32_t   rdr;        /* Receive data register - SSIRDRn for n=0*/
    volatile uint32_t   dummy1[4];
    volatile uint32_t   wsr;        /* WS mode register - SSIWSRn for n=0*/
    volatile uint32_t   fmr;        /* FS mode register - SSIFMRn for n=0*/
    volatile uint32_t   fsr;        /* FS status register - SSIFSRn for n=0*/
    volatile uint32_t   dummy2;
    volatile uint32_t   cre;        /* Control register extend - SSIRCEn for n=0*/
} ssi_ext_reg_t;

/* structures used for memory mapped SSIU registers*/
typedef struct
{
    volatile uint32_t   busif_mode;   /* SSIn_BUSIF_MODE for n = 0-0 to 0-7 */
    volatile uint32_t   busif_adinr;  /* SSIn_BUSIF_ADINR for n = 0-0 to 0-7 */
    volatile uint32_t   busif_dalign; /* SSIn_BUSIF_DALIGN for n = 0-0 to 0-7 */
} ssiu_busif_reg_t;

typedef struct
{
    volatile uint32_t   busif_dalign2; /* SSIm_BUSIF_DALIGN2 for m=0-0 */
} ssiu_busif2_reg_t;

typedef struct
{
    volatile uint32_t   dummy[3];
    volatile uint32_t   mode;            /* SSIq_MODE for q=0 */
    volatile uint32_t   control;         /* SSIq_CONTROL for q=0 */
    volatile uint32_t   status;          /* SSIq_STATUS for q=0 */
    volatile uint32_t   int_enable_main; /* SSIq_INT_ENABLE_MAIN for q=0 */
} ssiu_ssi_reg_t;

typedef struct
{
    volatile uint32_t   mode2;            /* SSIq_MODE2 for q=0 */
    volatile uint32_t   dummy[1];
    volatile uint32_t   status2;          /* SSIq_STATUS2 for q=0 */ 
    volatile uint32_t   int_enable_main2; /* SSIq_INT_ENABLE_MAIN2 for q=0 */
} ssiu_ssi2_reg_t;

typedef struct
{
    volatile uint32_t   system_status0;     /* SSI_SYSTEM_STATUS0 */
    volatile uint32_t   dummy1[1];
    volatile uint32_t   system_status2;     /* SSI_SYSTEM_STATUS2 */
    volatile uint32_t   dummy2[1];
    volatile uint32_t   system_int_enable0; /* SSI_SYSTEM_INT_ENABLE0 */
    volatile uint32_t   dummy3[1];
    volatile uint32_t   system_int_enable2; /* SSI_SYSTEM_INT_ENABLE2 */
    volatile uint32_t   dummy4[9];
    volatile uint32_t   system_status4;     /* SSI_SYSTEM_STATUS4 */
    volatile uint32_t   dummy5[1];
    volatile uint32_t   system_status6;     /* SSI_SYSTEM_STATUS6 */
    volatile uint32_t   dummy6[1];
    volatile uint32_t   system_int_enable4; /* SSI_SYSTEM_INT_ENABLE4 */
    volatile uint32_t   dummy7[1];
    volatile uint32_t   system_int_enable6; /* SSI_SYSTEM_INT_ENABLE6 */
} ssiu_common_reg_t;

int ssiu_mem_map(void);
int ssiu_mem_unmap(void);

#define BUSIF_GAP               RCAR_SSIU_SSI0_4_BUSIF_MODE - RCAR_SSIU_SSI0_0_BUSIF_MODE
#define SSIU_BUSIF2_OFFSET      RCAR_SSIU_SSI0_0_BUSIF_DALIGN2 - RCAR_SSIU_SSI0_0_BUSIF_MODE
#define SSIU_SSI_REG2_OFFSET    RCAR_SSIU_SSI0_0_MODE2 - RCAR_SSIU_SSI0_0_BUSIF_MODE
#define SSIU_COMMON_OFFSET      RCAR_SSIU_SSI_SYSTEM_STATUS0 - RCAR_SSIU_SSI0_0_BUSIF_MODE

#define SSI           ((ssi_reg_t*)(ssi_reg))
#define SSI_EXT       ((ssi_ext_reg_t*)(ssi_reg))
#define SSIU_BUSIF(u) ((ssiu_busif_reg_t*)(ssiu_reg_map + (u/4)*BUSIF_GAP  + (u%4)*0x20))
#define SSIU_BUSIF2   ((ssiu_busif2_reg_t*)(ssiu_reg_map + SSIU_BUSIF2_OFFSET))
#define SSIU_SSI      ((ssiu_ssi_reg_t*)(ssiu_reg_map))
#define SSIU_SSI2     ((ssiu_ssi2_reg_t*)(ssiu_reg_map + SSIU_SSI_REG2_OFFSET))
#define SSIU_COMMON   ((ssiu_common_reg_t*)(ssiu_reg_map + SSIU_COMMON_OFFSET))

#endif /* _R_Car_SSIU_REG_H */
