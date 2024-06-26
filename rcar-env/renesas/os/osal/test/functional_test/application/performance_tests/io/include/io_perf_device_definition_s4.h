/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    io_perf_device_definition_s4.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header IO Device definition on S4
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           3.0.0     13.06.2023    Create New
***********************************************************************************************************************/
#ifndef OSAL_PERF_IO_DEVICE_DEFINITION_S4_H_
#define OSAL_PERF_IO_DEVICE_DEFINITION_S4_H_

/* Definition */
#define TEST_IO_DEV_TYPE                  "rtdmac_control0"
#define TEST_IO_DEV                       "rtdmac_control0_01"

/* write register value */
#define TEST_IO_WRITE8_VAL                0xAAU
#define TEST_IO_WRITE16_VAL               0x00AAU
#define TEST_IO_WRITE32_VAL               0x000000AAU
#define TEST_IO_WRITE64_VAL               0x000000AA000000AAU

/* block write register value */
#define TEST_IO_WRITEBLOCK8_VAL           (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00}
#define TEST_IO_WRITEBLOCK16_VAL          (uint16_t[]){0x1FFF, 0x1000, 0x1FFF, 0x1000, 0x1FFF, 0xFF00, 0x1000, 0x1000}
#define TEST_IO_WRITEBLOCK32_VAL          (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x10000A40, 0x10000000}
#define TEST_IO_WRITEBLOCK64_VAL          (uint64_t[]){0x100000AA000000AA, 0x1000000000AA00AA}

/*  read offset */
#define TEST_IO_READ8_OFFSET_ISR          0x0110 // valid offset used in interrupt context
#define TEST_IO_READ8_OFFSET_NOR          0x0110 // valid offset abnormal case in normal context

#define TEST_IO_READ16_OFFSET_ISR         0x0110 // valid offset used in interrupt context
#define TEST_IO_READ16_OFFSET_NOR         0x0110 // valid offset used in Normal case in normal context

#define TEST_IO_READ32_OFFSET_ISR         0x0110 // valid offset used in interrupt context
#define TEST_IO_READ32_OFFSET_NOR         0x0110 // valid offset used in Normal case in normal context

#define TEST_IO_READ64_OFFSET_ISR         0x0000 // valid offset used in interrupt context
#define TEST_IO_READ64_OFFSET_NOR         0x0110 // valid offset used in Normal case in normal context

/* write offset */
#define TEST_IO_WRITE8_OFFSET_ISR         0x0000 // valid offset used in interrupt context
#define TEST_IO_WRITE8_OFFSET_NOR         0x0000 // valid offset used in Normal case in normal context

#define TEST_IO_WRITE16_OFFSET_ISR        0x0000 // valid offset used in interrupt context
#define TEST_IO_WRITE16_OFFSET_NOR        0x0000 // valid offset used in Normal case in normal context

#define TEST_IO_WRITE32_OFFSET_ISR        0x0000 // valid offset used in interrupt context
#define TEST_IO_WRITE32_OFFSET_NOR        0x0000 // valid offset used in Normal case in normal context

#define TEST_IO_WRITE64_OFFSET_ISR        0x0000 // valid offset used in interrupt context
#define TEST_IO_WRITE64_OFFSET_NOR        0x0000 // valid offset used in Normal case in normal context

/* readblock offset */
#define TEST_IO_BLOCKREAD8_OFFSET_ISR     0x0110 // valid offset used in interrupt context
#define TEST_IO_BLOCKREAD8_OFFSET_NOR     0x0110 // valid offset used in Normal case in normal context

#define TEST_IO_BLOCKREAD16_OFFSET_ISR    0x0110 // valid offset used in interrupt context
#define TEST_IO_BLOCKREAD16_OFFSET_NOR    0x0110 // valid offset used in Normal case in normal context

#define TEST_IO_BLOCKREAD32_OFFSET_ISR    0x0110 // valid offset used in interrupt context
#define TEST_IO_BLOCKREAD32_OFFSET_NOR    0x0110 // valid offset used in Normal case in normal context

#define TEST_IO_BLOCKREAD64_OFFSET_ISR    0x0000 // valid offset used in interrupt context
#define TEST_IO_BLOCKREAD64_OFFSET_NOR    0x0110 // valid offset used in Normal case in normal context

/* writeblock offset */
#define TEST_IO_BLOCKWRITE8_OFFSET_ISR    0x0000 // valid offset used in interrupt context
#define TEST_IO_BLOCKWRITE8_OFFSET_NOR    0x0000 // valid offset used in Normal case in normal context

#define TEST_IO_BLOCKWRITE16_OFFSET_ISR   0x0000 // valid offset used in interrupt context
#define TEST_IO_BLOCKWRITE16_OFFSET_NOR   0x0000 // valid offset used in Normal case in normal context

#define TEST_IO_BLOCKWRITE32_OFFSET_ISR   0x0000 // valid offset used in interrupt context
#define TEST_IO_BLOCKWRITE32_OFFSET_NOR   0x0000 // valid offset used in Normal case in normal context

#define TEST_IO_BLOCKWRITE64_OFFSET_ISR   0x0000 // valid offset used in interrupt context
#define TEST_IO_BLOCKWRITE64_OFFSET_NOR   0x0000 // valid offset used in Normal case in normal context

/* Definition for AXI bus API testing */
#define TEST_AXI_BUS_NAME                 "rt1_16-31"
#define TEST_NUM_OF_AXI_ID                16U

#endif /* End of File */
