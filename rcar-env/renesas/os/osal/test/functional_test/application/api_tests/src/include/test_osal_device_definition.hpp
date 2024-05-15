/***********************************************************************************************************************
* Copyright [2022-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    test_osal_device_definition.hpp
* Version :      3.10.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header file used in OSAL API Functional Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     10.02.2022    Create New
 *           2.0.1     15.02.2022    Update device and its setting definition for V3x/V4H/S4
 *           2.0.2     28.02.2022    Change to use rtdmac_control_00 for TEST_INIT_DEINT_DEVICE_1 on S4.
 *                                   Change to use system_ram_00 for TEST_INT_DEVICE_2 on S4.
 *                                   Update to use "rtdmac_control_00" and "rtdmac_control_01" device for PM testing on S4.
 *                                   Update device infomation for IO and IO Extention API test
 *                                   Add new macro TEST_IO_DEVICE_LIST_SIZE for required device list buffer size
 *           3.0.0     01.03.2022    Add device ID system_ram_00 and its setting for System Ram test.
 *                     04.03.2022    Update S4 AXI Bus Name Information(using rt_dmac_control_00 and rt_dmac_control_01)
 *                                   Update V4H Device information
 *                     08.03.2022    Update S4 device ID and its related value setting used to generate HW interrupt
 *                                   Change macro name TEST_IMR_DL_DATA to TEST_DUMMY_BUFFER_DATA for S4/V4H/V3x
 *                                   Change macro name TEST_IMR_REG_SRCR to TEST_REG_CLEAR_REGISTER for S4/V4H/V3x
 *                                   Change macro name TEST_IMR_SRCR_VALUE to TEST_CLEAR_REGISTER_VALUE for S4/V4H/V3x
 *                                   Add new macro related to number of buffers, buffer size and alignment for S4/V4H/V3x
 *           3.1.0     23.03.2022    Add new macro TEST_IO_SIZE_ZERO for S4/V4H/V3x
 *                                   Add new macro TEST_IO_ACCESS_SIZE for S4/V4H/V3x
 *           3.1.1     19.05.2022    Update TEST_AXI_BUS_NAME_DEVICE_INVALID macro for SIL
 *           3.1.2     26.05.2022    Update target register and written data for PM sequence test on V4H and V3x
 *                                   Update target register and written data for IO API test on V4H and V3x
 *           3.1.3     03.06.2022    Update target device of V4H about SDK 3.3.0 Device information
 *                                   Update target device of S4 about SDK 3.3.0 Device information
 *                     06.06.2022    Update device name used to generate HW interrupt for S4
 *                                   Update target test device name and type used for S4 testing
 *                                   Update axi id number of rtdmac_control0_00 
 *                                       from 16U to 1U on S4 (SDK 3.3.0)
 *                     08.06.2022    Add TEST_IO_BUSY_CHECK_DEVICE
 *           3.2.0     24.06.2022    Add new TEST_AXI_BUS_NAME and TEST_NUM_AXI_ID definition
 *           3.3.0     11.07.2022    Add the below new macros for PM test:
 *                                      - TEST_UNSUPPORT_POWER_DEVICE
 *                                      - TEST_UNSUPPORT_CLOCK_DEVICE
 *                                      - TEST_UNSUPPORT_RESET_DEVICE
 *                                      - PM_UNSUPPORT_PM_EXPECTED_VALUE
 *           3.4.0     09.11.2022    Add the below new macros for PM test:
 *                                      - TEST_PM_DEVICE_2_DEPENDENCE
 *                                      - TEST_PM_DEVICE_SEQUENCE_PG
 *           3.5.0     05.12.2022    Add the below macros for POST Clock test for S4:
 *                                      - TEST_FBA_DEVICE_ID (fba_ca550)
 *                                      - TEST_CLOCK_DIVIDER (0xBU)
 *           3.6.0     06.01.2023   Add the blow macros for IO test:
 *                                      - OSAL_DEVICE_TYPE_NAME_LENGTH_OUT_RANGE (OSAL_DEVICE_TYPE_NAME_MAX_LENGTH+1)
 *                                      - ARRAY_SIZE_IO_TEST (1)
 *                                      - DEVICE_HANDLE_INITIALIZED_VALUE (0xaa)
 *           3.7.0     27.02.2023   Update Expect Value and targe device of TEST_UNSUPPORT_RESET_DEVICE for Linux V3H2/V3M2
 *           3.8.0     03.03.2023   Added Expect Value of Normal Cases for AXI Bus API
 *                                       - added test Sequential device
 *                                       - for example) ims_00, ims_01 
 *           3.9.0     03.23.2023   Added Expect Value of POST Clock API boundary test
 *                                       - when Clock Divider = 0
 *                                       - with Linux, API returns OSAL_RETURN_FAIL
 *                                       - with other OS, API returns OSAL_RETURN_OK
 *           3.9.1     10.04.2023   Update macro PM_WAITFORSTATE_DELTATIME for SIL Windows
 *                                  Update value of the following macros for SIL test V3M2/V3Hx:
 *                                      - TEST_UNSUPPORT_POWER_DEVICE
 *                                      - TEST_UNSUPPORT_CLOCK_DEVICE
 *                                      - TEST_UNSUPPORT_RESET_DEVICE
 *                                      - PM_UNSUPPORT_PM_EXPECTED_VALUE
 *           3.10.0    11.09.2023   Add definition of V4M
***********************************************************************************************************************/

/**
 * @page Test_Device_Defined_value Defined OSAL resource value used in OSAL API Functional Test
 * @section Device_Defined_value OSAL device defined value
 * The defined OSAL device value used in each test cases. These values can be changed.
 * @snippet test_osal_device_definition.hpp OSAL_Device_Defined_value_In_Test
 */
// [OSAL_Device_Defined_value_In_Test]
#if defined(OSAL_SOC_S4) /* R-Car S4 */
/* Test device TYPE */
#define TEST_IO_DEVICE_TYPE_1               "rtdmac_control0" // device type used in IO APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_1     "rtdmac_control0" // device type used in IO Extension APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_2     "rtdmac_control0" // device type used in IO Extension APIs testing

/* Test device ID used in interrupt context */
#define TEST_INTCTX_DEVICE_1                "rtdmac_control0_00" // 1st device used to generate interrupt and use for testing in interrupt context.
#define TEST_INTCTX_DEVICE_2                "rtdmac_control1_00" // device use for testing in interrupt context.
#define TEST_INTCTX_DEVICE_3                "rtdmac_descriptor0_00" // 2nd device used to generate interrupt.

/* Test device ID used in normal context */
#define TEST_IO_DEVICE_1                    "rtdmac_control0_00" // device use for IO APIs testing in normal context
#define TEST_IO_DEVICE_2                    "rtdmac_descriptor0_00" // device use to turn ON clock and power used in S4 IO test
#define TEST_INIT_DEINT_DEVICE_1            "rtdmac_control0_00" // device use for Init/Deinit testing related to Interrupt APIs in normal context
#define TEST_INIT_DEINT_DEVICE_2            "rtdmac_descriptor0_00" // device use for Init/Deinit testing related to PM APIs in normal context
#define TEST_INT_DEVICE_1                   "iccom_00" // device use for Interrupt APIs testing in normal context
#define TEST_INT_DEVICE_2                   "system_ram_00" // device doesn't have interrupt channel. Use for Interrupt APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_1          "rtdmac_control0_00" // device use for IO extension APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_2          "rtdmac_control0_01" // device use for IO extension APIs testing in normal context
#define TEST_PM_DEVICE_1                    "rtdmac_descriptor0_00" // device (CG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2                    "rtdmac_descriptor1_00" // S4 doesn't have device (PG). So we will use CG device and change expected value.
#define TEST_PM_DEVICE_SEQUENCE             "rtdmac_descriptor0_00" // device use for PM sequence test
#define TEST_PERFORMANCE_DEVICE_1           "rtdmac_control0_00" // device use to measure performance of IO open/close device APIs.
#define TEST_FBA_DEVICE_ID                  "fba_ca550" // device use for testing R_OSAL_PmSetPostClock API
#define TEST_SYSTEM_RAM_ID                  "system_ram_00"
#define TEST_IO_BUSY_CHECK_DEVICE           "rtdmac_descriptor2_00" // Test target device when IoClose while executing PM API

/* Device list buffer size */
#define TEST_DEVICE_LIST_BUFFER_SIZE        100U

/* Clock divider */
#define TEST_CLOCK_DIVIDER                  0xBU

/* Device interrupt channel */
#define TEST_IRQ_CHANNEL_DEVICE_GENE_INT    0U // irq channle of device used to generate HW interrupt
#define TEST_IRQ_CHANNEL_DEVICE_INT         0U // irq channle of device has interrupt and use for testing in interrupt context
#define TEST_IRQ_CHANNEL_DEVICE_VALID       0U // irq channel of device has interrupt and use for testing in normal context
#define TEST_IRQ_CHANNEL_DEVICE_INVALID     0U // irq channel of device does not have interrupt and use for testing in normal context

/* Test device offsets */
#define TEST_OFFSET_OUT_RANGE               0x10000 /* offset out-of-range */

#define TEST_READ16_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ32_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ64_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */

#define TEST_WRITE16_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE32_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE64_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */

#define TEST_READBLOCK16_OFFSET_UNALIGNED   0x0081 /* offset is unalgined */
#define TEST_READBLOCK32_OFFSET_UNALIGNED   0x0035 /* offset is unalgined */
#define TEST_READBLOCK64_OFFSET_UNALIGNED   0x00C1 /* offset is unalgined */

#define TEST_WRITEBLOCK16_OFFSET_UNALIGNED  0x0081 /* offset is unalgined */
#define TEST_WRITEBLOCK32_OFFSET_UNALIGNED  0x0035 /* offset is unalgined */
#define TEST_WRITEBLOCK64_OFFSET_UNALIGNED  0x00C1 /* offset is unalgined */

/*  read offset */
#define TEST_READ8_REG_OFFSET_INT           0x0110 // valid offset used in interrupt context
#define TEST_READ8_REG_OFFSET_NC            0x0110 // valid offset normal case in normal context
#define TEST_READ8_REG_OFFSET_AC            0x0110 // valid offset abnormal case in normal context

#define TEST_READ16_REG_OFFSET_INT          0x0110 // valid offset used in interrupt context
#define TEST_READ16_REG_OFFSET_NC           0x0110 // valid offset used in Normal case in normal context
#define TEST_READ16_REG_OFFSET_AC           0x0110 // valid offset used in Abnormal case in normal context

#define TEST_READ32_REG_OFFSET_INT          0x0110 // valid offset used in interrupt context 
#define TEST_READ32_REG_OFFSET_NC           0x0110 // valid offset used in Normal case in normal context
#define TEST_READ32_REG_OFFSET_AC           0x0110 // valid offset used in Abnormal case in normal context

#define TEST_READ64_REG_OFFSET_INT          0x0000 // valid offset used in interrupt context
#define TEST_READ64_REG_OFFSET_NC           0x0110 // valid offset used in Normal case in normal context
#define TEST_READ64_REG_OFFSET_AC           0x0110 // valid offset used in Abnormal case in normal context

/* write offset */
#define TEST_WRITE8_REG_OFFSET_INT          0x0000 // valid offset used in interrupt context
#define TEST_WRITE8_REG_OFFSET_NC           0x0000 // valid offset used in Normal case in normal context
#define TEST_WRITE8_REG_OFFSET_AC           0x0000 // valid offset used in Abnormal case in normal context

#define TEST_WRITE16_REG_OFFSET_INT         0x0000 // valid offset used in interrupt context
#define TEST_WRITE16_REG_OFFSET_NC          0x0000 // valid offset used in Normal case in normal context
#define TEST_WRITE16_REG_OFFSET_AC          0x0000 // valid offset used in Abnormal case in normal context

#define TEST_WRITE32_REG_OFFSET_INT         0x0000 // valid offset used in interrupt context
#define TEST_WRITE32_REG_OFFSET_NC          0x0000 // valid offset used in Normal case in normal context
#define TEST_WRITE32_REG_OFFSET_AC          0x0000 // valid offset used in Abnormal case in normal context

#define TEST_WRITE64_REG_OFFSET_INT         0x0000 // valid offset used in interrupt context
#define TEST_WRITE64_REG_OFFSET_NC          0x0000 // valid offset used in Normal case in normal context
#define TEST_WRITE64_REG_OFFSET_AC          0x0000 // valid offset used in Abnormal case in normal context

/* readblock offset */
#define TEST_READBLOCK8_REG_OFFSET_INT      0x0110 // valid offset used in interrupt context
#define TEST_READBLOCK8_REG_OFFSET_NC       0x0110 // valid offset used in Normal case in normal context
#define TEST_READBLOCK8_REG_OFFSET_AC       0x0110 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK16_REG_OFFSET_INT     0x0110 // valid offset used in interrupt context
#define TEST_READBLOCK16_REG_OFFSET_NC      0x0110 // valid offset used in Normal case in normal context
#define TEST_READBLOCK16_REG_OFFSET_AC      0x0110 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK32_REG_OFFSET_INT     0x0110 // valid offset used in interrupt context
#define TEST_READBLOCK32_REG_OFFSET_NC      0x0110 // valid offset used in Normal case in normal context
#define TEST_READBLOCK32_REG_OFFSET_AC      0x0110 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK64_REG_OFFSET_INT     0x0000 // valid offset used in interrupt context
#define TEST_READBLOCK64_REG_OFFSET_NC      0x0110 // valid offset used in Normal case in normal context
#define TEST_READBLOCK64_REG_OFFSET_AC      0x0110 // valid offset used in Abnormal case in normal context

/* writeblock offset */
#define TEST_WRITEBLOCK8_REG_OFFSET_INT     0x0000 // valid offset used in interrupt context
#define TEST_WRITEBLOCK8_REG_OFFSET_NC      0x0000 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK8_REG_OFFSET_AC      0x0000 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK16_REG_OFFSET_INT    0x0000 // valid offset used in interrupt context
#define TEST_WRITEBLOCK16_REG_OFFSET_NC     0x0000 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK16_REG_OFFSET_AC     0x0000 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK32_REG_OFFSET_INT    0x0000 // valid offset used in interrupt context
#define TEST_WRITEBLOCK32_REG_OFFSET_NC     0x0000 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK32_REG_OFFSET_AC     0x0000 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK64_REG_OFFSET_INT    0x0000 // valid offset used in interrupt context
#define TEST_WRITEBLOCK64_REG_OFFSET_NC     0x0000 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK64_REG_OFFSET_AC     0x0000 // valid offset used in Abnormal case in normal context

/* Test read/write value related to device offset */

/* read value */
#define TEST_READ8_VAL_INT                  0x00U // read value in interrupt context
#define TEST_READ8_VAL_NC                   0x00U // read value of normal case in normal context

#define TEST_READ16_VAL_INT                 0x0000U // read value in interrupt context
#define TEST_READ16_VAL_NC                  0x0000U // read value of normal case in normal context

#define TEST_READ32_VAL_INT                 0x00000000U // read value in interrupt context
#define TEST_READ32_VAL_NC                  0x00000000U // read value of normal case in normal context

#define TEST_READ64_VAL_INT                 0x0000000000000000U // read value in interrupt context
#define TEST_READ64_VAL_NC                  0x0000000000000000U // read value of normal case in normal context

/* write value */
#define TEST_WRITE8_VAL_INT                 0xAAU // write value in interrupt context
#define TEST_WRITE8_VAL_NC                  0xAAU // write value of normal case in normal context
#define TEST_WRITE8_VAL_AC                  0xFFU // write value of abnormal case in normal context

#define TEST_WRITE16_VAL_INT                0x00AAU // write value in interrupt context
#define TEST_WRITE16_VAL_NC                 0x00AAU // write value of normal case in normal context
#define TEST_WRITE16_VAL_AC                 0x00FFU // write value of abnormal case in normal context

#define TEST_WRITE32_VAL_INT                0x000000AAU // write value in interrupt context
#define TEST_WRITE32_VAL_NC                 0x000000AAU // write value of normal case in normal context
#define TEST_WRITE32_VAL_AC                 0x000000FFU // write value of abnormal case in normal context

#define TEST_WRITE64_VAL_INT                0x000000AA000000AAU // write value in interrupt context
#define TEST_WRITE64_VAL_NC                 0x000000AA000000AAU // write value of normal case in normal context
#define TEST_WRITE64_VAL_AC                 0x000000FF000000FFU // write value of abnormal case in normal context

/* readblock value */
#define TEST_READBLOCK8_VAL_INT             (uint8_t[]){0x00, 0x00, 0x00, 0x00} // readblock value in interrupt context
#define TEST_READBLOCK8_VAL_NC              (uint8_t[]){0x00, 0x00, 0x00, 0x00} // readblock value of normal case in normal context

#define TEST_READBLOCK16_VAL_INT            (uint16_t[]){0x0000, 0x0000} // readblock value in interrupt context
#define TEST_READBLOCK16_VAL_NC             (uint16_t[]){0x0000, 0x0000} // readblock value of normal case in normal context

#define TEST_READBLOCK32_VAL_INT            (uint32_t[]){0x00000000, 0x00000000} // readblock value in interrupt context
#define TEST_READBLOCK32_VAL_NC             (uint32_t[]){0x00000000, 0x00000000} // readblock value of normal case in normal context

#define TEST_READBLOCK64_VAL_INT            (uint64_t[]){0x0000000000000000, 0x0000000000000000} // readblock value in interrupt context
#define TEST_READBLOCK64_VAL_NC             (uint64_t[]){0x0000000000000000} // readblock value of normal case in normal context

/* writeblock value */
#define TEST_WRITEBLOCK8_VAL_INT            (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00} // write value in interrupt context
#define TEST_WRITEBLOCK8_VAL_NC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00} // write value of normal case in normal context
#define TEST_WRITEBLOCK8_VAL_AC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK16_VAL_INT           (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0xFF00, 0x0000, 0x0000} // write value in interrupt context
#define TEST_WRITEBLOCK16_VAL_NC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0xFF00, 0x0000, 0x0000} // write value of normal case in normal context
#define TEST_WRITEBLOCK16_VAL_AC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0xFF00, 0x0000, 0x0000} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK32_VAL_INT           (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000000} // write value in interrupt context
#define TEST_WRITEBLOCK32_VAL_NC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000000} // write value of normal case in normal context
#define TEST_WRITEBLOCK32_VAL_AC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000000} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK64_VAL_INT           (uint64_t[]){0x000000AA000000AA, 0x0000000000AA00AA} // write value in interrupt context
#define TEST_WRITEBLOCK64_VAL_NC            (uint64_t[]){0x000000AA000000AA, 0x0000000000AA00AA} // write value of normal case in normal context
#define TEST_WRITEBLOCK64_VAL_AC            (uint64_t[]){0x000000AA000000AA, 0x0000000000AA00AA} // write value of abnormal case in normal context

/* IO read/write block size */
#define TEST_IO_SIZE_OUT_RANGE              0x10001U /* IO Block Read/Write with size is out-of-range */
#define TEST_IO_SIZE_ZERO                   0 /* IO Block Read/Write with size is 0 byte */
#define TEST_IO_ACCESS_SIZE                 16 /* IO Block Read/Write with size is 16 bytes */

/* Interrupt setting */
#define TEST_NUM_OF_BUFFERS                 2U // the number of buffers need to be allocate for HW interrupt generation
#define TEST_BUFFER_SIZE                    64U // the buffer allocation size
#define TEST_BUFFER_ALIGNMENT               64U // the buffer allocation aligment
#define TEST_DUMMY_BUFFER_DATA              (uint32_t[]){2908, 0} // Dummy data that is the initialized value of allocated buffers
#define TEST_REG_RDMSAR                     0x0000 // source address
#define TEST_REG_RDMDAR                     0x0004 // destination address
#define TEST_REG_RDMTCR                     0x0008 // transfer count
#define TEST_REG_RDMCHCRB                   0x001C // DMA Channel Control Register B
#define TEST_REG_RDMCHCR                    0x000C // DMA channel control register for RTDMAC0
#define TEST_REG_RDMOR                      0x0060 // DMA operation register for RT-DMAC0
#define TEST_REG_CLEAR_REGISTER             0x0100 // Clear Register
#define TEST_TRANSFER_COUNT_VALUE           0x00000001U // 1 transfer
#define TEST_TRANSFER_STEP_VALUE            0x00000000U // 1 descriptor
#define TEST_TRANSFER_RDMCHCR_VALUE         0x10040415U // setting for DMA channel control register
#define TEST_TRANSFER_RDMOR_VALUE           0x0301U // setting for DMA operation register
#define TEST_CLEAR_REGISTER_VALUE           0x00000001U // write value of Clear Register

/* PM test expected value for PG normal testing using TEST_PM_DEVICE_2 */
#define PM_STATE_EXPECTED_VALUE             OSAL_PM_STATE_CG

/* Register and its value used for read/write in PM sequence test */
#define PM_WRITE32_REG_OFFSET               0x00F4 // Read Rate control register for RT-DMAC0
#define PM_WRITE32_VALUE                    0x000000AA // Bit [7:0]: Rate Control counter setting for read transaction (3-225: valid setting)

#elif defined(OSAL_SOC_V4H) /* R-Car V4H */
/* Test device TYPE */
#define TEST_IO_DEVICE_TYPE_1               "ims" // device type used in IO APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_1     "ims" // device type used in IO Extension APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_2     "imp" // device type used in IO Extension APIs testing

/* Test device ID used in interrupt context */
#define TEST_INTCTX_DEVICE_1                "ims_00" // device used to generate interrupt and use for testing in interrupt context.
#define TEST_INTCTX_DEVICE_2                "imp_top_00" // device use for testing in interrupt context.

/* Test device ID used in normal context */
#define TEST_IO_DEVICE_1                    "ims_00" // device use for IO APIs testing in normal context
#define TEST_INIT_DEINT_DEVICE_1            "ims_00" // device use for Init/Deinit testing related to Interrupt APIs in normal context
#define TEST_INIT_DEINT_DEVICE_2            "ims_00" // device use for Init/Deinit testing related to PM APIs in normal context
#define TEST_INT_DEVICE_1                   "ims_00" // device use for Interrupt APIs testing in normal context
#define TEST_INT_DEVICE_2                   "imp_00" // device doesn't have interrupt channel. Use for Interrupt APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_1          "ims_00" // device use for IO extension APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_2          "ims_01" // device use for IO extension APIs testing in normal context
#define TEST_PM_DEVICE_1                    "ims_00" // device (CG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2                    "imp_00" // device (PG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2_DEPENDENCE         "imp_01" // depending device of TEST_PM_DEVICE_2 (PG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_3                    "imp_top_00" // device use to power on "imp_00". This is Linux Restriction.
#define TEST_PM_DEVICE_SEQUENCE             "ims_00" // device use for PM sequence test.
#define TEST_PM_DEVICE_SEQUENCE_PG          "imp_00" // (PG) device use for PM sequence test
#define TEST_PERFORMANCE_DEVICE_1           "ims_00" // device use to measure performance of IO open/close device APIs.
#define TEST_FBA_DEVICE_ID                  "fba_ims0" // device use for testing R_OSAL_PmSetPostClock API
#define TEST_SYSTEM_RAM_ID                  "system_ram_00"
#define TEST_IO_BUSY_CHECK_DEVICE           "ims_00" // Test target device when IoClose while executing PM API

/* Device list buffer size */
#define TEST_DEVICE_LIST_BUFFER_SIZE        100U

/* Clock divider */
#define TEST_CLOCK_DIVIDER                  0xB

/* Device interrupt channel */
#define TEST_IRQ_CHANNEL_DEVICE_GENE_INT    0U // irq channle of device used to generate HW interrupt
#define TEST_IRQ_CHANNEL_DEVICE_INT         0U // irq channle of device has interrupt and use for testing in interrupt context
#define TEST_IRQ_CHANNEL_DEVICE_VALID       0U // irq channel of device has interrupt and use for testing in normal context
#define TEST_IRQ_CHANNEL_DEVICE_INVALID     0U // irq channel of device does not have interrupt and use for testing in normal context

/* Test device offsets */
#define TEST_OFFSET_OUT_RANGE               0x10000 /* offset out-of-range */

#define TEST_READ16_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ32_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ64_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */

#define TEST_WRITE16_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE32_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE64_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */

#define TEST_READBLOCK16_OFFSET_UNALIGNED   0x0081 /* offset is unalgined */
#define TEST_READBLOCK32_OFFSET_UNALIGNED   0x0035 /* offset is unalgined */
#define TEST_READBLOCK64_OFFSET_UNALIGNED   0x00C1 /* offset is unalgined */

#define TEST_WRITEBLOCK16_OFFSET_UNALIGNED  0x0081 /* offset is unalgined */
#define TEST_WRITEBLOCK32_OFFSET_UNALIGNED  0x0035 /* offset is unalgined */
#define TEST_WRITEBLOCK64_OFFSET_UNALIGNED  0x00C1 /* offset is unalgined */

/*  read offset */
#define TEST_READ8_REG_OFFSET_INT           0x000C // valid offset used in interrupt context
#define TEST_READ8_REG_OFFSET_NC            0x0018 // valid offset normal case in normal context
#define TEST_READ8_REG_OFFSET_AC            0x00C0 // valid offset abnormal case in normal context

#define TEST_READ16_REG_OFFSET_INT          0x000C // valid offset used in interrupt context
#define TEST_READ16_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ16_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_READ32_REG_OFFSET_INT          0x000C // valid offset used in interrupt context 
#define TEST_READ32_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ32_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_READ64_REG_OFFSET_INT          0x00C0 // valid offset used in interrupt context
#define TEST_READ64_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ64_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

/* write offset */
#define TEST_WRITE8_REG_OFFSET_INT          0x0030 // valid offset used in interrupt context
#define TEST_WRITE8_REG_OFFSET_NC           0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE8_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE16_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE16_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE16_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE32_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE32_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE32_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE64_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE64_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE64_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

/* readblock offset */
#define TEST_READBLOCK8_REG_OFFSET_INT      0x000C // valid offset used in interrupt context
#define TEST_READBLOCK8_REG_OFFSET_NC       0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK8_REG_OFFSET_AC       0x0080 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK16_REG_OFFSET_INT     0x000C // valid offset used in interrupt context
#define TEST_READBLOCK16_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK16_REG_OFFSET_AC      0x0080 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK32_REG_OFFSET_INT     0x000C // valid offset used in interrupt context
#define TEST_READBLOCK32_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK32_REG_OFFSET_AC      0x0034 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK64_REG_OFFSET_INT     0x00C0 // valid offset used in interrupt context
#define TEST_READBLOCK64_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK64_REG_OFFSET_AC      0x00C0 // valid offset used in Abnormal case in normal context

/* writeblock offset */
#define TEST_WRITEBLOCK8_REG_OFFSET_INT     0x0080 // valid offset used in interrupt context
#define TEST_WRITEBLOCK8_REG_OFFSET_NC      0x0080 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK8_REG_OFFSET_AC      0x0080 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK16_REG_OFFSET_INT    0x0080 // valid offset used in interrupt context
#define TEST_WRITEBLOCK16_REG_OFFSET_NC     0x0080 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK16_REG_OFFSET_AC     0x0080 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK32_REG_OFFSET_INT    0x0034 // valid offset used in interrupt context
#define TEST_WRITEBLOCK32_REG_OFFSET_NC     0x0034 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK32_REG_OFFSET_AC     0x0034 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK64_REG_OFFSET_INT    0x0030 // valid offset used in interrupt context
#define TEST_WRITEBLOCK64_REG_OFFSET_NC     0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK64_REG_OFFSET_AC     0x00C0 // valid offset used in Abnormal case in normal context

/* Test read/write value related to device offset */

/* read value */
#define TEST_READ8_VAL_INT                  0x00U // read value in interrupt context
#define TEST_READ8_VAL_NC                   0x1FU // read value of normal case in normal context

#define TEST_READ16_VAL_INT                 0x0000U // read value in interrupt context
#define TEST_READ16_VAL_NC                  0x001FU // read value of normal case in normal context

#define TEST_READ32_VAL_INT                 0x00000000U // read value in interrupt context
#define TEST_READ32_VAL_NC                  0x0000001FU // read value of normal case in normal context

#define TEST_READ64_VAL_INT                 0x000000FF000000FFU // read value in interrupt context
#define TEST_READ64_VAL_NC                  0x000000000000001FU // read value of normal case in normal context

/* write value */
#define TEST_WRITE8_VAL_INT                 0xA8U // write value in interrupt context
#define TEST_WRITE8_VAL_NC                  0xA8U // write value of normal case in normal context
#define TEST_WRITE8_VAL_AC                  0xFFU // write value of abnormal case in normal context

#define TEST_WRITE16_VAL_INT                0x00A8U // write value in interrupt context
#define TEST_WRITE16_VAL_NC                 0x00A8U // write value of normal case in normal context
#define TEST_WRITE16_VAL_AC                 0x00FFU // write value of abnormal case in normal context

#define TEST_WRITE32_VAL_INT                0x0000AAA8U // write value in interrupt context
#define TEST_WRITE32_VAL_NC                 0x0000AAA8U // write value of normal case in normal context
#define TEST_WRITE32_VAL_AC                 0x000000FFU // write value of abnormal case in normal context

#define TEST_WRITE64_VAL_INT                0x0000AAC00000AAA8U // write value in interrupt context
#define TEST_WRITE64_VAL_NC                 0x0000AAC00000AAA8U // write value of normal case in normal context
#define TEST_WRITE64_VAL_AC                 0x000000FF000000FFU // write value of abnormal case in normal context

/* readblock value */
#define TEST_READBLOCK8_VAL_INT             (uint8_t[]){0x00, 0x00, 0x00, 0x00} // readblock value in interrupt context
#define TEST_READBLOCK8_VAL_NC              (uint8_t[]){0x1F, 0x00, 0x00, 0x00} // readblock value of normal case in normal context

#define TEST_READBLOCK16_VAL_INT            (uint16_t[]){0x0000, 0x0000} // readblock value in interrupt context
#define TEST_READBLOCK16_VAL_NC             (uint16_t[]){0x001F, 0x0000} // readblock value of normal case in normal context

#define TEST_READBLOCK32_VAL_INT            (uint32_t[]){0x00000000, 0x00000000} // readblock value in interrupt context
#define TEST_READBLOCK32_VAL_NC             (uint32_t[]){0x0000001F, 0x00000000} // readblock value of normal case in normal context

#define TEST_READBLOCK64_VAL_INT            (uint64_t[]){0x000000FF000000FF, 0x0000000000000000} // readblock value in interrupt context
#define TEST_READBLOCK64_VAL_NC             (uint64_t[]){0x000000000000001F} // readblock value of normal case in normal context

/* writeblock value */
#define TEST_WRITEBLOCK8_VAL_INT            (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value in interrupt context
#define TEST_WRITEBLOCK8_VAL_NC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value of normal case in normal context
#define TEST_WRITEBLOCK8_VAL_AC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK16_VAL_INT           (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value in interrupt context
#define TEST_WRITEBLOCK16_VAL_NC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value of normal case in normal context
#define TEST_WRITEBLOCK16_VAL_AC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK32_VAL_INT           (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value in interrupt context
#define TEST_WRITEBLOCK32_VAL_NC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value of normal case in normal context
#define TEST_WRITEBLOCK32_VAL_AC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK64_VAL_INT           (uint64_t[]){0x0000AAC00000AAA8, 0x0000000000000000} // write value in interrupt context
#define TEST_WRITEBLOCK64_VAL_NC            (uint64_t[]){0x0000AAC00000AAA8, 0x0000000000000000} // write value of normal case in normal context
#define TEST_WRITEBLOCK64_VAL_AC            (uint64_t[]){0x000000AA000000AA, 0x0000000000000000} // write value of abnormal case in normal context

/* IO read/write block size */
#define TEST_IO_SIZE_OUT_RANGE              0x10001U /* IO Block Read/Write with size is out-of-range */
#define TEST_IO_SIZE_ZERO                   0 /* IO Block Read/Write with size is 0 byte */
#define TEST_IO_ACCESS_SIZE                 16 /* IO Block Read/Write with size is 16 bytes */

/* Interrupt context setting */
#define TEST_NUM_OF_BUFFERS                 1U // the number of buffers need to be allocate for HW interrupt generation
#define TEST_BUFFER_SIZE                    64U // the buffer allocation size
#define TEST_BUFFER_ALIGNMENT               64U // the buffer allocation aligment
/* Display list data */
#define TEST_DUMMY_BUFFER_DATA              (uint32_t[]){0x8f000000U} // Dummy data that is the initialized value of allocated buffers

/* IMR register to generate HW interrupt */
#define TEST_IMR_REG_DLSAR                  0x0030 // DL Start Address Register
#define TEST_IMR_REG_IMR                    0x0018 // Interrupt Mask Register
#define TEST_IMR_REG_CR                     0x0008 // Control Register
#define TEST_REG_CLEAR_REGISTER             0x0010 // Status Clear Register

/* IMR register value */
#define TEST_IMR_IMR_VALUE                  0x00000000U // write value of Interrupt Mask Register
#define TEST_IMR_CR_VALUE                   0x00000001U // write value of Control Register
#define TEST_CLEAR_REGISTER_VALUE           0x00000007U // write value of Status Clear Register

/* PM test expected value for PG normal testing using TEST_PM_DEVICE_2 */
#define PM_STATE_EXPECTED_VALUE             OSAL_PM_STATE_PG

/* Register and its value used for read/write in PM sequence test */
#define PM_WRITE32_REG_OFFSET               0x0030 // DL Start Address Register (DLSAR)
#define PM_WRITE32_VALUE                    0x00000AA8U

#elif defined(OSAL_SOC_V4H2) /* R-Car V4H2 */
/* Test device TYPE */
#define TEST_IO_DEVICE_TYPE_1               "ims" // device type used in IO APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_1     "ims" // device type used in IO Extension APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_2     "imp" // device type used in IO Extension APIs testing

/* Test device ID used in interrupt context */
#define TEST_INTCTX_DEVICE_1                "ims_00" // device used to generate interrupt and use for testing in interrupt context.
#define TEST_INTCTX_DEVICE_2                "imp_top_00" // device use for testing in interrupt context.

/* Test device ID used in normal context */
#define TEST_IO_DEVICE_1                    "ims_00" // device use for IO APIs testing in normal context
#define TEST_INIT_DEINT_DEVICE_1            "ims_00" // device use for Init/Deinit testing related to Interrupt APIs in normal context
#define TEST_INIT_DEINT_DEVICE_2            "ims_00" // device use for Init/Deinit testing related to PM APIs in normal context
#define TEST_INT_DEVICE_1                   "ims_00" // device use for Interrupt APIs testing in normal context
#define TEST_INT_DEVICE_2                   "imp_00" // device doesn't have interrupt channel. Use for Interrupt APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_1          "ims_00" // device use for IO extension APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_2          "ims_01" // device use for IO extension APIs testing in normal context
#define TEST_PM_DEVICE_1                    "ims_00" // device (CG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2                    "imp_00" // device (PG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2_DEPENDENCE         "imp_01" // depending device of TEST_PM_DEVICE_2 (PG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_3                    "imp_top_00" // device use to power on "imp_00". This is Linux Restriction.
#define TEST_PM_DEVICE_SEQUENCE             "ims_00" // device use for PM sequence test.
#define TEST_PM_DEVICE_SEQUENCE_PG          "imp_00" // (PG) device use for PM sequence test
#define TEST_PERFORMANCE_DEVICE_1           "ims_00" // device use to measure performance of IO open/close device APIs.
#define TEST_FBA_DEVICE_ID                  "fba_ims0" // device use for testing R_OSAL_PmSetPostClock API
#define TEST_SYSTEM_RAM_ID                  "system_ram_00"
#define TEST_IO_BUSY_CHECK_DEVICE           "ims_00" // Test target device when IoClose while executing PM API

/* Device list buffer size */
#define TEST_DEVICE_LIST_BUFFER_SIZE        100U

/* Clock divider */
#define TEST_CLOCK_DIVIDER                  0xB

/* Device interrupt channel */
#define TEST_IRQ_CHANNEL_DEVICE_GENE_INT    0U // irq channle of device used to generate HW interrupt
#define TEST_IRQ_CHANNEL_DEVICE_INT         0U // irq channle of device has interrupt and use for testing in interrupt context
#define TEST_IRQ_CHANNEL_DEVICE_VALID       0U // irq channel of device has interrupt and use for testing in normal context
#define TEST_IRQ_CHANNEL_DEVICE_INVALID     0U // irq channel of device does not have interrupt and use for testing in normal context

/* Test device offsets */
#define TEST_OFFSET_OUT_RANGE               0x10000 /* offset out-of-range */

#define TEST_READ16_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ32_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ64_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */

#define TEST_WRITE16_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE32_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE64_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */

#define TEST_READBLOCK16_OFFSET_UNALIGNED   0x0081 /* offset is unalgined */
#define TEST_READBLOCK32_OFFSET_UNALIGNED   0x0035 /* offset is unalgined */
#define TEST_READBLOCK64_OFFSET_UNALIGNED   0x00C1 /* offset is unalgined */

#define TEST_WRITEBLOCK16_OFFSET_UNALIGNED  0x0081 /* offset is unalgined */
#define TEST_WRITEBLOCK32_OFFSET_UNALIGNED  0x0035 /* offset is unalgined */
#define TEST_WRITEBLOCK64_OFFSET_UNALIGNED  0x00C1 /* offset is unalgined */

/*  read offset */
#define TEST_READ8_REG_OFFSET_INT           0x000C // valid offset used in interrupt context
#define TEST_READ8_REG_OFFSET_NC            0x0018 // valid offset normal case in normal context
#define TEST_READ8_REG_OFFSET_AC            0x00C0 // valid offset abnormal case in normal context

#define TEST_READ16_REG_OFFSET_INT          0x000C // valid offset used in interrupt context
#define TEST_READ16_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ16_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_READ32_REG_OFFSET_INT          0x000C // valid offset used in interrupt context 
#define TEST_READ32_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ32_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_READ64_REG_OFFSET_INT          0x00C0 // valid offset used in interrupt context
#define TEST_READ64_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ64_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

/* write offset */
#define TEST_WRITE8_REG_OFFSET_INT          0x0030 // valid offset used in interrupt context
#define TEST_WRITE8_REG_OFFSET_NC           0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE8_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE16_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE16_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE16_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE32_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE32_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE32_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE64_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE64_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE64_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

/* readblock offset */
#define TEST_READBLOCK8_REG_OFFSET_INT      0x000C // valid offset used in interrupt context
#define TEST_READBLOCK8_REG_OFFSET_NC       0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK8_REG_OFFSET_AC       0x0080 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK16_REG_OFFSET_INT     0x000C // valid offset used in interrupt context
#define TEST_READBLOCK16_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK16_REG_OFFSET_AC      0x0080 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK32_REG_OFFSET_INT     0x000C // valid offset used in interrupt context
#define TEST_READBLOCK32_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK32_REG_OFFSET_AC      0x0034 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK64_REG_OFFSET_INT     0x00C0 // valid offset used in interrupt context
#define TEST_READBLOCK64_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK64_REG_OFFSET_AC      0x00C0 // valid offset used in Abnormal case in normal context

/* writeblock offset */
#define TEST_WRITEBLOCK8_REG_OFFSET_INT     0x0080 // valid offset used in interrupt context
#define TEST_WRITEBLOCK8_REG_OFFSET_NC      0x0080 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK8_REG_OFFSET_AC      0x0080 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK16_REG_OFFSET_INT    0x0080 // valid offset used in interrupt context
#define TEST_WRITEBLOCK16_REG_OFFSET_NC     0x0080 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK16_REG_OFFSET_AC     0x0080 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK32_REG_OFFSET_INT    0x0034 // valid offset used in interrupt context
#define TEST_WRITEBLOCK32_REG_OFFSET_NC     0x0034 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK32_REG_OFFSET_AC     0x0034 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK64_REG_OFFSET_INT    0x0030 // valid offset used in interrupt context
#define TEST_WRITEBLOCK64_REG_OFFSET_NC     0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK64_REG_OFFSET_AC     0x00C0 // valid offset used in Abnormal case in normal context

/* Test read/write value related to device offset */

/* read value */
#define TEST_READ8_VAL_INT                  0x00U // read value in interrupt context
#define TEST_READ8_VAL_NC                   0x1FU // read value of normal case in normal context

#define TEST_READ16_VAL_INT                 0x0000U // read value in interrupt context
#define TEST_READ16_VAL_NC                  0x001FU // read value of normal case in normal context

#define TEST_READ32_VAL_INT                 0x00000000U // read value in interrupt context
#define TEST_READ32_VAL_NC                  0x0000001FU // read value of normal case in normal context

#define TEST_READ64_VAL_INT                 0x000000FF000000FFU // read value in interrupt context
#define TEST_READ64_VAL_NC                  0x000000000000001FU // read value of normal case in normal context

/* write value */
#define TEST_WRITE8_VAL_INT                 0xA8U // write value in interrupt context
#define TEST_WRITE8_VAL_NC                  0xA8U // write value of normal case in normal context
#define TEST_WRITE8_VAL_AC                  0xFFU // write value of abnormal case in normal context

#define TEST_WRITE16_VAL_INT                0x00A8U // write value in interrupt context
#define TEST_WRITE16_VAL_NC                 0x00A8U // write value of normal case in normal context
#define TEST_WRITE16_VAL_AC                 0x00FFU // write value of abnormal case in normal context

#define TEST_WRITE32_VAL_INT                0x0000AAA8U // write value in interrupt context
#define TEST_WRITE32_VAL_NC                 0x0000AAA8U // write value of normal case in normal context
#define TEST_WRITE32_VAL_AC                 0x000000FFU // write value of abnormal case in normal context

#define TEST_WRITE64_VAL_INT                0x0000AAC00000AAA8U // write value in interrupt context
#define TEST_WRITE64_VAL_NC                 0x0000AAC00000AAA8U // write value of normal case in normal context
#define TEST_WRITE64_VAL_AC                 0x000000FF000000FFU // write value of abnormal case in normal context

/* readblock value */
#define TEST_READBLOCK8_VAL_INT             (uint8_t[]){0x00, 0x00, 0x00, 0x00} // readblock value in interrupt context
#define TEST_READBLOCK8_VAL_NC              (uint8_t[]){0x1F, 0x00, 0x00, 0x00} // readblock value of normal case in normal context

#define TEST_READBLOCK16_VAL_INT            (uint16_t[]){0x0000, 0x0000} // readblock value in interrupt context
#define TEST_READBLOCK16_VAL_NC             (uint16_t[]){0x001F, 0x0000} // readblock value of normal case in normal context

#define TEST_READBLOCK32_VAL_INT            (uint32_t[]){0x00000000, 0x00000000} // readblock value in interrupt context
#define TEST_READBLOCK32_VAL_NC             (uint32_t[]){0x0000001F, 0x00000000} // readblock value of normal case in normal context

#define TEST_READBLOCK64_VAL_INT            (uint64_t[]){0x000000FF000000FF, 0x0000000000000000} // readblock value in interrupt context
#define TEST_READBLOCK64_VAL_NC             (uint64_t[]){0x000000000000001F} // readblock value of normal case in normal context

/* writeblock value */
#define TEST_WRITEBLOCK8_VAL_INT            (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value in interrupt context
#define TEST_WRITEBLOCK8_VAL_NC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value of normal case in normal context
#define TEST_WRITEBLOCK8_VAL_AC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK16_VAL_INT           (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value in interrupt context
#define TEST_WRITEBLOCK16_VAL_NC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value of normal case in normal context
#define TEST_WRITEBLOCK16_VAL_AC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK32_VAL_INT           (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value in interrupt context
#define TEST_WRITEBLOCK32_VAL_NC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value of normal case in normal context
#define TEST_WRITEBLOCK32_VAL_AC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK64_VAL_INT           (uint64_t[]){0x0000AAC00000AAA8, 0x0000000000000000} // write value in interrupt context
#define TEST_WRITEBLOCK64_VAL_NC            (uint64_t[]){0x0000AAC00000AAA8, 0x0000000000000000} // write value of normal case in normal context
#define TEST_WRITEBLOCK64_VAL_AC            (uint64_t[]){0x000000AA000000AA, 0x0000000000000000} // write value of abnormal case in normal context

/* IO read/write block size */
#define TEST_IO_SIZE_OUT_RANGE              0x10001U /* IO Block Read/Write with size is out-of-range */
#define TEST_IO_SIZE_ZERO                   0 /* IO Block Read/Write with size is 0 byte */
#define TEST_IO_ACCESS_SIZE                 16 /* IO Block Read/Write with size is 16 bytes */

/* Interrupt context setting */
#define TEST_NUM_OF_BUFFERS                 1U // the number of buffers need to be allocate for HW interrupt generation
#define TEST_BUFFER_SIZE                    64U // the buffer allocation size
#define TEST_BUFFER_ALIGNMENT               64U // the buffer allocation aligment
/* Display list data */
#define TEST_DUMMY_BUFFER_DATA              (uint32_t[]){0x8f000000U} // Dummy data that is the initialized value of allocated buffers

/* IMR register to generate HW interrupt */
#define TEST_IMR_REG_DLSAR                  0x0030 // DL Start Address Register
#define TEST_IMR_REG_IMR                    0x0018 // Interrupt Mask Register
#define TEST_IMR_REG_CR                     0x0008 // Control Register
#define TEST_REG_CLEAR_REGISTER             0x0010 // Status Clear Register

/* IMR register value */
#define TEST_IMR_IMR_VALUE                  0x00000000U // write value of Interrupt Mask Register
#define TEST_IMR_CR_VALUE                   0x00000001U // write value of Control Register
#define TEST_CLEAR_REGISTER_VALUE           0x00000007U // write value of Status Clear Register

/* PM test expected value for PG normal testing using TEST_PM_DEVICE_2 */
#define PM_STATE_EXPECTED_VALUE             OSAL_PM_STATE_PG

/* Register and its value used for read/write in PM sequence test */
#define PM_WRITE32_REG_OFFSET               0x0030 // DL Start Address Register (DLSAR)
#define PM_WRITE32_VALUE                    0x00000AA8U

#elif defined(OSAL_SOC_V4M) /* R-Car V4M */
/* Test device TYPE */
#define TEST_IO_DEVICE_TYPE_1               "ims" // device type used in IO APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_1     "ims" // device type used in IO Extension APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_2     "imp" // device type used in IO Extension APIs testing

/* Test device ID used in interrupt context */
#define TEST_INTCTX_DEVICE_1                "ims_00" // device used to generate interrupt and use for testing in interrupt context.
#define TEST_INTCTX_DEVICE_2                "imp_top_00" // device use for testing in interrupt context.

/* Test device ID used in normal context */
#define TEST_IO_DEVICE_1                    "ims_00" // device use for IO APIs testing in normal context
#define TEST_INIT_DEINT_DEVICE_1            "ims_00" // device use for Init/Deinit testing related to Interrupt APIs in normal context
#define TEST_INIT_DEINT_DEVICE_2            "ims_00" // device use for Init/Deinit testing related to PM APIs in normal context
#define TEST_INT_DEVICE_1                   "ims_00" // device use for Interrupt APIs testing in normal context
#define TEST_INT_DEVICE_2                   "imp_00" // device doesn't have interrupt channel. Use for Interrupt APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_1          "ims_00" // device use for IO extension APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_2          "ims_01" // device use for IO extension APIs testing in normal context
#define TEST_PM_DEVICE_1                    "tisp_00" // device (CG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2                    "imp_00" // device (PG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2_DEPENDENCE         "imp_01" // depending device of TEST_PM_DEVICE_2 (PG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_3                    "imp_top_00" // device use to power on "imp_00". This is Linux Restriction.
#define TEST_PM_DEVICE_SEQUENCE             "tisp_00" // device use for PM sequence test.
#define TEST_PM_DEVICE_SEQUENCE_PG          "imp_00" // (PG) device use for PM sequence test
#define TEST_PM_DEVICE_SEQUENCE_PG_OFF_1    "fba_imp0" // device to be released before starting the PG device sequence test.
#define TEST_PM_DEVICE_SEQUENCE_PG_OFF_2    "fba_imp1" // device to be released before starting the PG device sequence test.

#define TEST_PERFORMANCE_DEVICE_1           "ims_00" // device use to measure performance of IO open/close device APIs.
#define TEST_FBA_DEVICE_ID                  "fba_ims0" // device use for testing R_OSAL_PmSetPostClock API
#define TEST_SYSTEM_RAM_ID                  "system_ram_00"
#define TEST_IO_BUSY_CHECK_DEVICE           "ims_00" // Test target device when IoClose while executing PM API

/* Device list buffer size */
#define TEST_DEVICE_LIST_BUFFER_SIZE        100U

/* Clock divider */
#define TEST_CLOCK_DIVIDER                  0xB

/* Device interrupt channel */
#define TEST_IRQ_CHANNEL_DEVICE_GENE_INT    0U // irq channle of device used to generate HW interrupt
#define TEST_IRQ_CHANNEL_DEVICE_INT         0U // irq channle of device has interrupt and use for testing in interrupt context
#define TEST_IRQ_CHANNEL_DEVICE_VALID       0U // irq channel of device has interrupt and use for testing in normal context
#define TEST_IRQ_CHANNEL_DEVICE_INVALID     0U // irq channel of device does not have interrupt and use for testing in normal context

/* Test device offsets */
#define TEST_OFFSET_OUT_RANGE               0x10000 /* offset out-of-range */

#define TEST_READ16_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ32_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ64_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */

#define TEST_WRITE16_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE32_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE64_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */

#define TEST_READBLOCK16_OFFSET_UNALIGNED   0x0081 /* offset is unalgined */
#define TEST_READBLOCK32_OFFSET_UNALIGNED   0x0035 /* offset is unalgined */
#define TEST_READBLOCK64_OFFSET_UNALIGNED   0x00C1 /* offset is unalgined */

#define TEST_WRITEBLOCK16_OFFSET_UNALIGNED  0x0081 /* offset is unalgined */
#define TEST_WRITEBLOCK32_OFFSET_UNALIGNED  0x0035 /* offset is unalgined */
#define TEST_WRITEBLOCK64_OFFSET_UNALIGNED  0x00C1 /* offset is unalgined */

/*  read offset */
#define TEST_READ8_REG_OFFSET_INT           0x000C // valid offset used in interrupt context
#define TEST_READ8_REG_OFFSET_NC            0x0018 // valid offset normal case in normal context
#define TEST_READ8_REG_OFFSET_AC            0x00C0 // valid offset abnormal case in normal context

#define TEST_READ16_REG_OFFSET_INT          0x000C // valid offset used in interrupt context
#define TEST_READ16_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ16_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_READ32_REG_OFFSET_INT          0x000C // valid offset used in interrupt context 
#define TEST_READ32_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ32_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_READ64_REG_OFFSET_INT          0x00C0 // valid offset used in interrupt context
#define TEST_READ64_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ64_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

/* write offset */
#define TEST_WRITE8_REG_OFFSET_INT          0x0030 // valid offset used in interrupt context
#define TEST_WRITE8_REG_OFFSET_NC           0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE8_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE16_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE16_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE16_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE32_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE32_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE32_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE64_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE64_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE64_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

/* readblock offset */
#define TEST_READBLOCK8_REG_OFFSET_INT      0x000C // valid offset used in interrupt context
#define TEST_READBLOCK8_REG_OFFSET_NC       0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK8_REG_OFFSET_AC       0x0080 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK16_REG_OFFSET_INT     0x000C // valid offset used in interrupt context
#define TEST_READBLOCK16_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK16_REG_OFFSET_AC      0x0080 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK32_REG_OFFSET_INT     0x000C // valid offset used in interrupt context
#define TEST_READBLOCK32_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK32_REG_OFFSET_AC      0x0034 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK64_REG_OFFSET_INT     0x00C0 // valid offset used in interrupt context
#define TEST_READBLOCK64_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK64_REG_OFFSET_AC      0x00C0 // valid offset used in Abnormal case in normal context

/* writeblock offset */
#define TEST_WRITEBLOCK8_REG_OFFSET_INT     0x0080 // valid offset used in interrupt context
#define TEST_WRITEBLOCK8_REG_OFFSET_NC      0x0080 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK8_REG_OFFSET_AC      0x0080 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK16_REG_OFFSET_INT    0x0080 // valid offset used in interrupt context
#define TEST_WRITEBLOCK16_REG_OFFSET_NC     0x0080 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK16_REG_OFFSET_AC     0x0080 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK32_REG_OFFSET_INT    0x0034 // valid offset used in interrupt context
#define TEST_WRITEBLOCK32_REG_OFFSET_NC     0x0034 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK32_REG_OFFSET_AC     0x0034 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK64_REG_OFFSET_INT    0x0030 // valid offset used in interrupt context
#define TEST_WRITEBLOCK64_REG_OFFSET_NC     0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK64_REG_OFFSET_AC     0x00C0 // valid offset used in Abnormal case in normal context

/* Test read/write value related to device offset */

/* read value */
#define TEST_READ8_VAL_INT                  0x00U // read value in interrupt context
#define TEST_READ8_VAL_NC                   0x1FU // read value of normal case in normal context

#define TEST_READ16_VAL_INT                 0x0000U // read value in interrupt context
#define TEST_READ16_VAL_NC                  0x001FU // read value of normal case in normal context

#define TEST_READ32_VAL_INT                 0x00000000U // read value in interrupt context
#define TEST_READ32_VAL_NC                  0x0000001FU // read value of normal case in normal context

#define TEST_READ64_VAL_INT                 0x000000FF000000FFU // read value in interrupt context
#define TEST_READ64_VAL_NC                  0x000000000000001FU // read value of normal case in normal context

/* write value */
#define TEST_WRITE8_VAL_INT                 0xA8U // write value in interrupt context
#define TEST_WRITE8_VAL_NC                  0xA8U // write value of normal case in normal context
#define TEST_WRITE8_VAL_AC                  0xFFU // write value of abnormal case in normal context

#define TEST_WRITE16_VAL_INT                0x00A8U // write value in interrupt context
#define TEST_WRITE16_VAL_NC                 0x00A8U // write value of normal case in normal context
#define TEST_WRITE16_VAL_AC                 0x00FFU // write value of abnormal case in normal context

#define TEST_WRITE32_VAL_INT                0x0000AAA8U // write value in interrupt context
#define TEST_WRITE32_VAL_NC                 0x0000AAA8U // write value of normal case in normal context
#define TEST_WRITE32_VAL_AC                 0x000000FFU // write value of abnormal case in normal context

#define TEST_WRITE64_VAL_INT                0x0000AAC00000AAA8U // write value in interrupt context
#define TEST_WRITE64_VAL_NC                 0x0000AAC00000AAA8U // write value of normal case in normal context
#define TEST_WRITE64_VAL_AC                 0x000000FF000000FFU // write value of abnormal case in normal context

/* readblock value */
#define TEST_READBLOCK8_VAL_INT             (uint8_t[]){0x00, 0x00, 0x00, 0x00} // readblock value in interrupt context
#define TEST_READBLOCK8_VAL_NC              (uint8_t[]){0x1F, 0x00, 0x00, 0x00} // readblock value of normal case in normal context

#define TEST_READBLOCK16_VAL_INT            (uint16_t[]){0x0000, 0x0000} // readblock value in interrupt context
#define TEST_READBLOCK16_VAL_NC             (uint16_t[]){0x001F, 0x0000} // readblock value of normal case in normal context

#define TEST_READBLOCK32_VAL_INT            (uint32_t[]){0x00000000, 0x00000000} // readblock value in interrupt context
#define TEST_READBLOCK32_VAL_NC             (uint32_t[]){0x0000001F, 0x00000000} // readblock value of normal case in normal context

#define TEST_READBLOCK64_VAL_INT            (uint64_t[]){0x000000FF000000FF, 0x0000000000000000} // readblock value in interrupt context
#define TEST_READBLOCK64_VAL_NC             (uint64_t[]){0x000000000000001F} // readblock value of normal case in normal context

/* writeblock value */
#define TEST_WRITEBLOCK8_VAL_INT            (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value in interrupt context
#define TEST_WRITEBLOCK8_VAL_NC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value of normal case in normal context
#define TEST_WRITEBLOCK8_VAL_AC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK16_VAL_INT           (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value in interrupt context
#define TEST_WRITEBLOCK16_VAL_NC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value of normal case in normal context
#define TEST_WRITEBLOCK16_VAL_AC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK32_VAL_INT           (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value in interrupt context
#define TEST_WRITEBLOCK32_VAL_NC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value of normal case in normal context
#define TEST_WRITEBLOCK32_VAL_AC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK64_VAL_INT           (uint64_t[]){0x0000AAC00000AAA8, 0x0000000000000000} // write value in interrupt context
#define TEST_WRITEBLOCK64_VAL_NC            (uint64_t[]){0x0000AAC00000AAA8, 0x0000000000000000} // write value of normal case in normal context
#define TEST_WRITEBLOCK64_VAL_AC            (uint64_t[]){0x000000AA000000AA, 0x0000000000000000} // write value of abnormal case in normal context

/* IO read/write block size */
#define TEST_IO_SIZE_OUT_RANGE              0x10001U /* IO Block Read/Write with size is out-of-range */
#define TEST_IO_SIZE_ZERO                   0 /* IO Block Read/Write with size is 0 byte */
#define TEST_IO_ACCESS_SIZE                 16 /* IO Block Read/Write with size is 16 bytes */

/* Interrupt context setting */
#define TEST_NUM_OF_BUFFERS                 1U // the number of buffers need to be allocate for HW interrupt generation
#define TEST_BUFFER_SIZE                    64U // the buffer allocation size
#define TEST_BUFFER_ALIGNMENT               64U // the buffer allocation aligment
/* Display list data */
#define TEST_DUMMY_BUFFER_DATA              (uint32_t[]){0x8f000000U} // Dummy data that is the initialized value of allocated buffers

/* IMR register to generate HW interrupt */
#define TEST_IMR_REG_DLSAR                  0x0030 // DL Start Address Register
#define TEST_IMR_REG_IMR                    0x0018 // Interrupt Mask Register
#define TEST_IMR_REG_CR                     0x0008 // Control Register
#define TEST_REG_CLEAR_REGISTER             0x0010 // Status Clear Register

/* IMR register value */
#define TEST_IMR_IMR_VALUE                  0x00000000U // write value of Interrupt Mask Register
#define TEST_IMR_CR_VALUE                   0x00000001U // write value of Control Register
#define TEST_CLEAR_REGISTER_VALUE           0x00000007U // write value of Status Clear Register

/* PM test expected value for PG normal testing using TEST_PM_DEVICE_2 */
#define PM_STATE_EXPECTED_VALUE             OSAL_PM_STATE_PG

/* Register and its value used for read/write in PM sequence test */
#define PM_WRITE32_REG_OFFSET               0x0004 // ISP Input FIFO Control Register (ISPFIFOCTRL0)
#define PM_WRITE32_VALUE                    0x0000AA00U

#elif defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) /* R-Car V3x */
/* Test device TYPE */
#define TEST_IO_DEVICE_TYPE_1               "ims" // device type used in IO APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_1     "ims" // device type used in IO Extension APIs testing
#define TEST_IO_EXTENSION_DEVICE_TYPE_2     "imp" // device type used in IO Extension APIs testing

/* Test device ID used in interrupt context */
#define TEST_INTCTX_DEVICE_1                "ims_00" // device used to generate interrupt and use for testing in interrupt context.
#define TEST_INTCTX_DEVICE_2                "imp_top_00" // device use for testing in interrupt context.

/* Test device ID used in normal context */
#define TEST_IO_DEVICE_1                    "ims_00" // device use for IO APIs testing in normal context
#define TEST_INIT_DEINT_DEVICE_1            "ims_00" // device use for Init/Deinit testing related to Interrupt APIs in normal context
#define TEST_INIT_DEINT_DEVICE_2            "ims_00" // device use for Init/Deinit testing related to PM APIs in normal context
#define TEST_INT_DEVICE_1                   "ims_00" // device use for Interrupt APIs testing in normal context
#define TEST_INT_DEVICE_2                   "imp_00" // device doesn't have interrupt channel. Use for Interrupt APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_1          "ims_00" // device use for IO extension APIs testing in normal context
#define TEST_IO_EXTENSION_DEVICE_2          "ims_01" // device use for IO extension APIs testing in normal context
#define TEST_PM_DEVICE_1                    "ims_00" // device (CG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2                    "imp_00" // device (PG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_2_DEPENDENCE         "imp_01" // depending device of TEST_PM_DEVICE_2 (PG) use for PM APIs testing in normal context
#define TEST_PM_DEVICE_3                    "imp_top_00" // device use to power on "imp_00". This is Linux Restriction.
#define TEST_PM_DEVICE_SEQUENCE             "ims_00" // device use for PM sequence test.
#define TEST_PM_DEVICE_SEQUENCE_PG          "imp_00" // (PG) device use for PM sequence test
#define TEST_PERFORMANCE_DEVICE_1           "ims_00" // device use to measure performance of IO open/close device APIs.
#define TEST_FBA_DEVICE_ID                  "fba_ims0" // device use for testing R_OSAL_PmSetPostClock API
#define TEST_SYSTEM_RAM_ID                  "system_ram_00"
#define TEST_IO_BUSY_CHECK_DEVICE           "ims_00" // Test target device when IoClose while executing PM API

/* Device list buffer size */
#define TEST_DEVICE_LIST_BUFFER_SIZE        100U

/* Clock divider */
#define TEST_CLOCK_DIVIDER                  0xB

/* Device interrupt channel */
#define TEST_IRQ_CHANNEL_DEVICE_GENE_INT    0U // irq channle of device used to generate HW interrupt
#define TEST_IRQ_CHANNEL_DEVICE_INT         0U // irq channle of device has interrupt and use for testing in interrupt context
#define TEST_IRQ_CHANNEL_DEVICE_VALID       0U // irq channel of device has interrupt and use for testing in normal context
#define TEST_IRQ_CHANNEL_DEVICE_INVALID     0U // irq channel of device does not have interrupt and use for testing in normal context

/* Test device offsets */
#define TEST_OFFSET_OUT_RANGE               0x10000 /* offset out-of-range */

#define TEST_READ16_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ32_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */
#define TEST_READ64_OFFSET_UNALIGNED        0x00C1 /* offset is unalgined */

#define TEST_WRITE16_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE32_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */
#define TEST_WRITE64_OFFSET_UNALIGNED       0x00C1 /* offset is unalgined */

#define TEST_READBLOCK16_OFFSET_UNALIGNED   0x0081 /* offset is unalgined */
#define TEST_READBLOCK32_OFFSET_UNALIGNED   0x0035 /* offset is unalgined */
#define TEST_READBLOCK64_OFFSET_UNALIGNED   0x00C1 /* offset is unalgined */

#define TEST_WRITEBLOCK16_OFFSET_UNALIGNED  0x0081 /* offset is unalgined */
#define TEST_WRITEBLOCK32_OFFSET_UNALIGNED  0x0035 /* offset is unalgined */
#define TEST_WRITEBLOCK64_OFFSET_UNALIGNED  0x00C1 /* offset is unalgined */

/*  read offset */
#define TEST_READ8_REG_OFFSET_INT           0x000C // valid offset used in interrupt context
#define TEST_READ8_REG_OFFSET_NC            0x0018 // valid offset normal case in normal context
#define TEST_READ8_REG_OFFSET_AC            0x00C0 // valid offset abnormal case in normal context

#define TEST_READ16_REG_OFFSET_INT          0x000C // valid offset used in interrupt context
#define TEST_READ16_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ16_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_READ32_REG_OFFSET_INT          0x000C // valid offset used in interrupt context 
#define TEST_READ32_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ32_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_READ64_REG_OFFSET_INT          0x00C0 // valid offset used in interrupt context
#define TEST_READ64_REG_OFFSET_NC           0x0018 // valid offset used in Normal case in normal context
#define TEST_READ64_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

/* write offset */
#define TEST_WRITE8_REG_OFFSET_INT          0x0030 // valid offset used in interrupt context
#define TEST_WRITE8_REG_OFFSET_NC           0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE8_REG_OFFSET_AC           0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE16_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE16_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE16_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE32_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE32_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE32_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

#define TEST_WRITE64_REG_OFFSET_INT         0x0030 // valid offset used in interrupt context
#define TEST_WRITE64_REG_OFFSET_NC          0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITE64_REG_OFFSET_AC          0x00C0 // valid offset used in Abnormal case in normal context

/* readblock offset */
#define TEST_READBLOCK8_REG_OFFSET_INT      0x000C // valid offset used in interrupt context
#define TEST_READBLOCK8_REG_OFFSET_NC       0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK8_REG_OFFSET_AC       0x0080 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK16_REG_OFFSET_INT     0x000C // valid offset used in interrupt context
#define TEST_READBLOCK16_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK16_REG_OFFSET_AC      0x0080 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK32_REG_OFFSET_INT     0x000C // valid offset used in interrupt context
#define TEST_READBLOCK32_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK32_REG_OFFSET_AC      0x0034 // valid offset used in Abnormal case in normal context

#define TEST_READBLOCK64_REG_OFFSET_INT     0x00C0 // valid offset used in interrupt context
#define TEST_READBLOCK64_REG_OFFSET_NC      0x0018 // valid offset used in Normal case in normal context
#define TEST_READBLOCK64_REG_OFFSET_AC      0x00C0 // valid offset used in Abnormal case in normal context

/* writeblock offset */
#define TEST_WRITEBLOCK8_REG_OFFSET_INT     0x0080 // valid offset used in interrupt context
#define TEST_WRITEBLOCK8_REG_OFFSET_NC      0x0080 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK8_REG_OFFSET_AC      0x0080 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK16_REG_OFFSET_INT    0x0080 // valid offset used in interrupt context
#define TEST_WRITEBLOCK16_REG_OFFSET_NC     0x0080 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK16_REG_OFFSET_AC     0x0080 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK32_REG_OFFSET_INT    0x0034 // valid offset used in interrupt context
#define TEST_WRITEBLOCK32_REG_OFFSET_NC     0x0034 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK32_REG_OFFSET_AC     0x0034 // valid offset used in Abnormal case in normal context

#define TEST_WRITEBLOCK64_REG_OFFSET_INT    0x0030 // valid offset used in interrupt context
#define TEST_WRITEBLOCK64_REG_OFFSET_NC     0x0030 // valid offset used in Normal case in normal context
#define TEST_WRITEBLOCK64_REG_OFFSET_AC     0x00C0 // valid offset used in Abnormal case in normal context

/* Test read/write value related to device offset */

/* read value */
#define TEST_READ8_VAL_INT                  0x00U // read value in interrupt context
#define TEST_READ8_VAL_NC                   0x1FU // read value of normal case in normal context

#define TEST_READ16_VAL_INT                 0x0000U // read value in interrupt context
#define TEST_READ16_VAL_NC                  0x001FU // read value of normal case in normal context

#define TEST_READ32_VAL_INT                 0x00000000U // read value in interrupt context
#define TEST_READ32_VAL_NC                  0x0000001FU // read value of normal case in normal context

#define TEST_READ64_VAL_INT                 0x000000FF000000FFU // read value in interrupt context
#define TEST_READ64_VAL_NC                  0x000000000000001FU // read value of normal case in normal context

/* write value */
#define TEST_WRITE8_VAL_INT                 0xA8U // write value in interrupt context
#define TEST_WRITE8_VAL_NC                  0xA8U // write value of normal case in normal context
#define TEST_WRITE8_VAL_AC                  0xFFU // write value of abnormal case in normal context

#define TEST_WRITE16_VAL_INT                0x00A8U // write value in interrupt context
#define TEST_WRITE16_VAL_NC                 0x00A8U // write value of normal case in normal context
#define TEST_WRITE16_VAL_AC                 0x00FFU // write value of abnormal case in normal context

#define TEST_WRITE32_VAL_INT                0x0000AAA8U // write value in interrupt context
#define TEST_WRITE32_VAL_NC                 0x0000AAA8U // write value of normal case in normal context
#define TEST_WRITE32_VAL_AC                 0x000000FFU // write value of abnormal case in normal context

#define TEST_WRITE64_VAL_INT                0x0000AAC00000AAA8U // write value in interrupt context
#define TEST_WRITE64_VAL_NC                 0x0000AAC00000AAA8U // write value of normal case in normal context
#define TEST_WRITE64_VAL_AC                 0x000000FF000000FFU // write value of abnormal case in normal context

/* readblock value */
#define TEST_READBLOCK8_VAL_INT             (uint8_t[]){0x00, 0x00, 0x00, 0x00} // readblock value in interrupt context
#define TEST_READBLOCK8_VAL_NC              (uint8_t[]){0x1F, 0x00, 0x00, 0x00} // readblock value of normal case in normal context

#define TEST_READBLOCK16_VAL_INT            (uint16_t[]){0x0000, 0x0000} // readblock value in interrupt context
#define TEST_READBLOCK16_VAL_NC             (uint16_t[]){0x001F, 0x0000} // readblock value of normal case in normal context

#define TEST_READBLOCK32_VAL_INT            (uint32_t[]){0x00000000, 0x00000000} // readblock value in interrupt context
#define TEST_READBLOCK32_VAL_NC             (uint32_t[]){0x0000001F, 0x00000000} // readblock value of normal case in normal context

#define TEST_READBLOCK64_VAL_INT            (uint64_t[]){0x000000FF000000FF, 0x0000000000000000} // readblock value in interrupt context
#define TEST_READBLOCK64_VAL_NC             (uint64_t[]){0x000000000000001F} // readblock value of normal case in normal context

/* writeblock value */
#define TEST_WRITEBLOCK8_VAL_INT            (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value in interrupt context
#define TEST_WRITEBLOCK8_VAL_NC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value of normal case in normal context
#define TEST_WRITEBLOCK8_VAL_AC             (uint8_t[]){0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, \
                                            0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK16_VAL_INT           (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value in interrupt context
#define TEST_WRITEBLOCK16_VAL_NC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value of normal case in normal context
#define TEST_WRITEBLOCK16_VAL_AC            (uint16_t[]){0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000, 0x1FFF, 0x0000} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK32_VAL_INT           (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value in interrupt context
#define TEST_WRITEBLOCK32_VAL_NC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value of normal case in normal context
#define TEST_WRITEBLOCK32_VAL_AC            (uint32_t[]){0xAAAA0000, 0xAAAA0000, 0x00000A40, 0x00000A00} // write value of abnormal case in normal context

#define TEST_WRITEBLOCK64_VAL_INT           (uint64_t[]){0x0000AAC00000AAA8, 0x0000000000000000} // write value in interrupt context
#define TEST_WRITEBLOCK64_VAL_NC            (uint64_t[]){0x0000AAC00000AAA8, 0x0000000000000000} // write value of normal case in normal context
#define TEST_WRITEBLOCK64_VAL_AC            (uint64_t[]){0x000000AA000000AA, 0x0000000000000000} // write value of abnormal case in normal context

/* IO read/write block size */
#define TEST_IO_SIZE_OUT_RANGE              0x10001U /* IO Block Read/Write with size is out-of-range */
#define TEST_IO_SIZE_ZERO                   0 /* IO Block Read/Write with size is 0 byte */
#define TEST_IO_ACCESS_SIZE                 16 /* IO Block Read/Write with size is 16 bytes */

/* Interrupt context setting */
#define TEST_NUM_OF_BUFFERS                 1U // the number of buffers need to be allocate for HW interrupt generation
#define TEST_BUFFER_SIZE                    64U // the buffer allocation size
#define TEST_BUFFER_ALIGNMENT               64U // the buffer allocation aligment
/* Display list data */
#define TEST_DUMMY_BUFFER_DATA              (uint32_t[]){0x8f000000U} // Dummy data that is the initialized value of allocated buffers

/* IMR register to generate HW interrupt */
#define TEST_IMR_REG_DLSAR                  0x0030 // DL Start Address Register
#define TEST_IMR_REG_IMR                    0x0018 // Interrupt Mask Register
#define TEST_IMR_REG_CR                     0x0008 // Control Register
#define TEST_REG_CLEAR_REGISTER             0x0010 // Status Clear Register

/* IMR register value */
#define TEST_IMR_IMR_VALUE                  0x00000000U // write value of Interrupt Mask Register
#define TEST_IMR_CR_VALUE                   0x00000001U // write value of Control Register
#define TEST_CLEAR_REGISTER_VALUE           0x00000007U // write value of Status Clear Register

/* PM test expected value for PG normal testing using TEST_PM_DEVICE_2 */
#define PM_STATE_EXPECTED_VALUE             OSAL_PM_STATE_PG

/* Register and its value used for read/write in PM sequence test */
#define PM_WRITE32_REG_OFFSET               0x0030 // DL Start Address Register (DLSAR)
#define PM_WRITE32_VALUE                    0x00000AA8U
#endif

#if defined(TEST_SIL) // Definition for SIL Test
#if defined(OSAL_SOC_V4H)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "cisp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "cisp_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "system_ram_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V4H2)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "cisp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "cisp_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "system_ram_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V4M)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "cisp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "imp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "system_ram_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          8U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V3U)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "imp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "imp_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "system_ram_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            8U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          8U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) /* R-Car V3x */
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "ims_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "ims_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "system_ram_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            1U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          1U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#endif
#elif defined(TEST_LINUX) || defined(TEST_QNX) || defined(TEST_EMCOS) || defined(TEST_ZEPHYR) // Definition for HIL Linux, QNX, EMCOS, ZEPHYR test
#if defined(OSAL_SOC_S4)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "rtdmac_control0_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "rtdmac_control0_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "iccom_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            1U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          1U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V4H)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "cisp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "cisp_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "iccom_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V4H2)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "cisp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "cisp_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "iccom_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V4M)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "cisp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "imp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "iccom_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            5U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          8U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V3U)
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "imp_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "imp_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "iccom_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            8U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          8U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#elif defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) /* R-Car V3x */
/* Device ID for AXI BUS name test */
#define TEST_AXI_BUS_NAME_DEVICE_VALID      "ims_00" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_VALID_1    "ims_01" // device (have axi bus id) use for Axi bus name APIs testing in normal context
#define TEST_AXI_BUS_NAME_DEVICE_INVALID    "iccom_00" // device (dont have axi bus id) use for Axi bus name APIs testing in normal context
/* Device AXI IDs */
#define TEST_DEVICE_AXI_ID_VALID            1U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_VALID_1          1U // axi id of device support axi bus
#define TEST_DEVICE_AXI_ID_INVALID          0U // axi id of device doesn't support axi bus
#endif
#endif

#if defined(OSAL_SOC_S4)
/* Required buffer size of device list */
#define TEST_IO_DEVICE_LIST_SIZE            304U // the required buffer size of device list related to TEST_IO_DEVICE_TYPE_1
#elif defined(OSAL_SOC_V4H)
#define TEST_IO_DEVICE_LIST_SIZE            14U // the required buffer size of device list related to TEST_IO_DEVICE_TYPE_1
#elif defined(OSAL_SOC_V4H2)
#define TEST_IO_DEVICE_LIST_SIZE            14U // the required buffer size of device list related to TEST_IO_DEVICE_TYPE_1
#elif defined(OSAL_SOC_V4M)
#define TEST_IO_DEVICE_LIST_SIZE            14U // the required buffer size of device list related to TEST_IO_DEVICE_TYPE_1
#elif defined(OSAL_SOC_V3U)
#define TEST_IO_DEVICE_LIST_SIZE            14U // the required buffer size of device list related to TEST_IO_DEVICE_TYPE_1
#elif defined(OSAL_SOC_V3H1)
#define TEST_IO_DEVICE_LIST_SIZE            28U // the required buffer size of device list related to TEST_IO_DEVICE_TYPE_1
#elif defined(OSAL_SOC_V3H2)
#define TEST_IO_DEVICE_LIST_SIZE            21U // the required buffer size of device list related to TEST_IO_DEVICE_TYPE_1
#elif defined(OSAL_SOC_V3M2)
#define TEST_IO_DEVICE_LIST_SIZE            28U // the required buffer size of device list related to TEST_IO_DEVICE_TYPE_1
#endif

/* System Ram read/write offset/value */
#if defined(OSAL_SOC_S4)
#define TEST_SYSTEM_RAM_OFFSET_NC           0x00000000 // valid offset used in normal case
#define TEST_SYSTEM_RAM_OFFSET_BN           0x0005FFFC // valid offset used in boundary normal case
#define TEST_SYSTEM_RAM_OFFSET_BA           0x00060000 // invalid offset used in boundary abnormal case
#define TEST_SYSTEM_RAM_OFFSET_AC           0x00060004 // invalid offset used in abnormal case

#define TEST_SYSTEM_RAM_VALID_VALUE         0x0000AAAA
#define TEST_SYSTEM_RAM_CLEAN_VALUE         0x00000000
#elif defined(OSAL_SOC_V4H)
#define TEST_SYSTEM_RAM_OFFSET_NC           0x00000000 // valid offset used in normal case
#define TEST_SYSTEM_RAM_OFFSET_BN           0x000FFFFC // valid offset used in boundary normal case
#define TEST_SYSTEM_RAM_OFFSET_BA           0x00100000 // invalid offset used in boundary abnormal case
#define TEST_SYSTEM_RAM_OFFSET_AC           0x00100004 // invalid offset used in abnormal case

#define TEST_SYSTEM_RAM_VALID_VALUE         0x0000AAAA
#define TEST_SYSTEM_RAM_CLEAN_VALUE         0x00000000
#elif defined(OSAL_SOC_V4H2)
#define TEST_SYSTEM_RAM_OFFSET_NC           0x00000000 // valid offset used in normal case
#define TEST_SYSTEM_RAM_OFFSET_BN           0x000FFFFC // valid offset used in boundary normal case
#define TEST_SYSTEM_RAM_OFFSET_BA           0x00100000 // invalid offset used in boundary abnormal case
#define TEST_SYSTEM_RAM_OFFSET_AC           0x00100004 // invalid offset used in abnormal case

#define TEST_SYSTEM_RAM_VALID_VALUE         0x0000AAAA
#define TEST_SYSTEM_RAM_CLEAN_VALUE         0x00000000
#elif defined(OSAL_SOC_V4M)
#define TEST_SYSTEM_RAM_OFFSET_NC           0x00000000 // valid offset used in normal case
#define TEST_SYSTEM_RAM_OFFSET_BN           0x000FFFFC // valid offset used in boundary normal case
#define TEST_SYSTEM_RAM_OFFSET_BA           0x00100000 // invalid offset used in boundary abnormal case
#define TEST_SYSTEM_RAM_OFFSET_AC           0x00100004 // invalid offset used in abnormal case

#define TEST_SYSTEM_RAM_VALID_VALUE         0x0000AAAA
#define TEST_SYSTEM_RAM_CLEAN_VALUE         0x00000000
#elif defined(OSAL_SOC_V3U)
#define TEST_SYSTEM_RAM_OFFSET_NC           0x00000000 // valid offset used in normal case
#define TEST_SYSTEM_RAM_OFFSET_BN           0x000FFFFC // valid offset used in boundary normal case
#define TEST_SYSTEM_RAM_OFFSET_BA           0x00100000 // invalid offset used in boundary abnormal case
#define TEST_SYSTEM_RAM_OFFSET_AC           0x00100004 // invalid offset used in abnormal case

#define TEST_SYSTEM_RAM_VALID_VALUE         0x0000AAAA
#define TEST_SYSTEM_RAM_CLEAN_VALUE         0x00000000
#elif defined(OSAL_SOC_V3H2)
#define TEST_SYSTEM_RAM_OFFSET_NC           0x00000000 // valid offset used in normal case
#define TEST_SYSTEM_RAM_OFFSET_BN           0x0005FFFC // valid offset used in boundary normal case
#define TEST_SYSTEM_RAM_OFFSET_BA           0x00060000 // invalid offset used in boundary abnormal case
#define TEST_SYSTEM_RAM_OFFSET_AC           0x00060004 // invalid offset used in abnormal case

#define TEST_SYSTEM_RAM_VALID_VALUE         0x0000AAAA
#define TEST_SYSTEM_RAM_CLEAN_VALUE         0x00000000
#elif defined(OSAL_SOC_V3M2)
#define TEST_SYSTEM_RAM_OFFSET_NC           0x00000000 // valid offset used in normal case
#define TEST_SYSTEM_RAM_OFFSET_BN           0x0006FFFC // valid offset used in boundary normal case
#define TEST_SYSTEM_RAM_OFFSET_BA           0x00070000 // invalid offset used in boundary abnormal case
#define TEST_SYSTEM_RAM_OFFSET_AC           0x00070004 // invalid offset used in abnormal case

#define TEST_SYSTEM_RAM_VALID_VALUE         0x0000AAAA // valid write data
#define TEST_SYSTEM_RAM_CLEAN_VALUE         0x00000000 // value use to clean register
#endif

/* AXI bus definition */
#if defined(OSAL_SOC_S4)
#define TEST_AXI_BUS_NAME_1                 "rt1_00"
#define TEST_AXI_BUS_NAME_2                 "rt1_01"
#define TEST_NUM_AXI_ID_1                   1
#define TEST_NUM_AXI_ID_2                   1
#elif defined(OSAL_SOC_V4H)
#define TEST_AXI_BUS_NAME_1                 "vc_08"
#define TEST_AXI_BUS_NAME_2                 "vc_09"
#define TEST_NUM_AXI_ID_1                   1
#define TEST_NUM_AXI_ID_2                   1
#elif defined(OSAL_SOC_V4H2)
#define TEST_AXI_BUS_NAME_1                 "vc_08"
#define TEST_AXI_BUS_NAME_2                 "vc_09"
#define TEST_NUM_AXI_ID_1                   1
#define TEST_NUM_AXI_ID_2                   1
#elif defined(OSAL_SOC_V4M)
#define TEST_AXI_BUS_NAME_1                 "vc_08"
#define TEST_AXI_BUS_NAME_2                 "vc_09"
#define TEST_NUM_AXI_ID_1                   1
#define TEST_NUM_AXI_ID_2                   1
#elif defined(OSAL_SOC_V3U)
#define TEST_AXI_BUS_NAME_1                 "vc0_08"
#define TEST_AXI_BUS_NAME_2                 "vc0_09"
#define TEST_NUM_AXI_ID_1                   1
#define TEST_NUM_AXI_ID_2                   1
#elif defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3H1)
#define TEST_AXI_BUS_NAME_1                 "vc0_04"
#define TEST_AXI_BUS_NAME_2                 "vc0_05"
#define TEST_NUM_AXI_ID_1                   1
#define TEST_NUM_AXI_ID_2                   1
#elif defined(OSAL_SOC_V3M2)
#define TEST_AXI_BUS_NAME_1                 "vi0_02"
#define TEST_AXI_BUS_NAME_2                 "vi0_02"
#define TEST_NUM_AXI_ID_1                   1
#define TEST_NUM_AXI_ID_2                   1
#endif

#if defined(OSAL_SOC_S4)
#define TEST_UNSUPPORT_POWER_DEVICE         "system_ram_00" // Unsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "iccom_00" // Unsupport clock domain device
#define TEST_UNSUPPORT_RESET_DEVICE         "icumx_cmd_00" // Unsupport reset domain device
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_UNSUPPORTED_OPERATION
#elif defined(OSAL_SOC_V4H)
#define TEST_UNSUPPORT_POWER_DEVICE         "system_ram_00" // Unsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "iccom_00" // Unsupport clock domain device
#if defined(TEST_ZEPHYR)
#define TEST_UNSUPPORT_RESET_DEVICE         "fba_imp0" // Unsupport reset domain device
#else
#define TEST_UNSUPPORT_RESET_DEVICE         "icumx_cmd_00" // Unsupport reset domain device
#endif
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_UNSUPPORTED_OPERATION
#elif defined(OSAL_SOC_V4H2)
#define TEST_UNSUPPORT_POWER_DEVICE         "system_ram_00" // Unsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "iccom_00" // Unsupport clock domain device
#define TEST_UNSUPPORT_RESET_DEVICE         "icumx_cmd_00" // Unsupport reset domain device
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_UNSUPPORTED_OPERATION
#elif defined(OSAL_SOC_V4M)
#define TEST_UNSUPPORT_POWER_DEVICE         "imp_dma_slim_00" // Unsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "iccom_00" // Unsupport clock domain device
#define TEST_UNSUPPORT_RESET_DEVICE         "icumx_cmd_00" // Unsupport reset domain device
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_UNSUPPORTED_OPERATION
#elif defined(OSAL_SOC_V3U)
#define TEST_UNSUPPORT_POWER_DEVICE         "system_ram_00" // Upsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "rtdmac_descriptor_00" // Unsupport clock domain device
#define TEST_UNSUPPORT_RESET_DEVICE         "imp_radsp_dmac_00" // Unsupport reset domain device
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_OK
#elif defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3H1)
#if !defined(TEST_SIL)
#define TEST_UNSUPPORT_POWER_DEVICE         "ipmmu_cache_vi0" // Upsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "ipmmu_cache_vi0" // Unsupport clock domain device
#define TEST_UNSUPPORT_RESET_DEVICE         "ipmmu_mm_00" // Unsupport reset domain device
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_UNSUPPORTED_OPERATION
#else
#define TEST_UNSUPPORT_POWER_DEVICE         "system_ram_00" // Upsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "system_ram_00" // Unsupport clock domain device
#define TEST_UNSUPPORT_RESET_DEVICE         "imp_spmi_00" // Unsupport reset domain device
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_OK
#endif
#elif defined(OSAL_SOC_V3M2)
#if !defined(TEST_SIL)
#define TEST_UNSUPPORT_POWER_DEVICE         "system_ram_00" // Upsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "system_ram_00" // Unsupport clock domain device
#define TEST_UNSUPPORT_RESET_DEVICE         "ipmmu_mm_00" // Unsupport reset domain device
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_UNSUPPORTED_OPERATION
#else
#define TEST_UNSUPPORT_POWER_DEVICE         "system_ram_00" // Upsupport power domain device
#define TEST_UNSUPPORT_CLOCK_DEVICE         "system_ram_00" // Unsupport clock domain device
#define TEST_UNSUPPORT_RESET_DEVICE         "imp_spmi_00" // Unsupport reset domain device
#define PM_UNSUPPORT_PM_EXPECTED_VALUE      OSAL_RETURN_OK
#endif
#endif

/* Common device setting */
#define UNKNOW_DEVICE_TYPE                  "aaa"
#define PM_TIMEOUT                          10 // 10 msec
#define UNKNOW_AXI_BUS_ID                   1000
#define OSAL_DEVICE_TYPE_NAME_LENGTH_OUT_RANGE     OSAL_DEVICE_TYPE_NAME_MAX_LENGTH+1
#define ARRAY_SIZE_IO_TEST                  1
#define DEVICE_HANDLE_INITIALIZED_VALUE     0xaa

#ifdef TEST_LINUX
#define PM_WAITFORSTATE_DELTATIME           1000000
#elif TEST_QNX
#define PM_WAITFORSTATE_DELTATIME           2500000
#elif TEST_SIL
#if defined(TEST_WINDOWS)
#define PM_WAITFORSTATE_DELTATIME           6000000
#else
#define PM_WAITFORSTATE_DELTATIME           1000000
#endif
#else
#define PM_WAITFORSTATE_DELTATIME           2500000
#endif

/* Expect Value of Post Clock API Boundary Test */
#if defined(TEST_LINUX)
#define POST_CLOCK_BOUNDARY_TEST_EXPECTED_VALUE OSAL_RETURN_FAIL // IOCTL of UIO Driver return FAIL (when clock_divider = 0)
#else
#define POST_CLOCK_BOUNDARY_TEST_EXPECTED_VALUE OSAL_RETURN_OK
#endif
// [OSAL_Device_Defined_value_In_Test]
