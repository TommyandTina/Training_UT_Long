/***********************************************************************************************************************
* Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.5.0
* Description  : Set power / clock to all hwip
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "target/emcos/pma/r_pma.h"

#include "target/emcos/r_osal_common.h"


#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <drv/sysc/sysc.h>
#include <drv/cpg/cpg_driver.h>

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
#define PMA_PM_CPG_SIZE        0x3000
#define PMA_PM_CPG_BASE_ADDR   0xE6150000
#define PMA_PM_OFFSET_CPGWPR   0x0000
#define PMA_PM_OFFSET_SRCR     0x2C00
#define PMA_PM_OFFSET_SRSTCLR  0x2C80
#define PMA_PM_OFFSET_MSTPCR   0x2D00
#define PMA_PM_OFFSET_MSTPSR   0x2E00

#define DRIVER_PDR_NUM_V4H      23
#define DRIVER_CLOCK_NUM_V4H    160
#define DRIVER_RESET_NUM_V4H    200

/*======================================================================================================================
Private type definitions
======================================================================================================================*/
typedef struct st_pma_power_reg
{
    const uint32_t  pdr;
    uint32_t        value;
} st_pma_power_reg_t;

typedef struct st_pma_reset_reg
{
    const int32_t   module;
    uint32_t        value;
} st_pma_reset_reg_t;

typedef struct st_pma_clock_reg
{
    const int32_t   module;
    uint32_t        value;
} st_pma_clock_reg_t;

typedef struct st_pma_reg
{
    uint32_t pdr_num;
    st_pma_power_reg_t *power_reg;
    uint32_t clock_reg_num;
    st_pma_clock_reg_t *clock_reg;
    uint32_t reset_reg_num;
    st_pma_reset_reg_t *reset_reg;
}st_pma_reg_t;

/*======================================================================================================================
Private global variables
======================================================================================================================*/
static st_pma_power_reg_t power_reg_v4h[DRIVER_PDR_NUM_V4H] = {
    { SYSC_A3IR,    0 },   /*   A3IR     */
    { SYSC_A33DGA,  0 },   /*   A33DGA   */
    { SYSC_A3VIP0,  0 },   /*   A3VIP0   */
    { SYSC_A3VIP1,  0 },   /*   A3VIP1   */
    { SYSC_A3VIP2,  0 },   /*   A3VIP2   */
    { SYSC_A3DUL,   0 },   /*   A3DUL    */
    { SYSC_A3ISP0,  0 },   /*   A3ISP0   */
    { SYSC_A3ISP1,  0 },   /*   A3ISP1   */
    { SYSC_A23DGB,  0 },   /*   A23DGB   */
    { SYSC_A2CN0,   0 },   /*   A2CN0    */
    { SYSC_A2IMP01, 0 },   /*   A2IMP01  */
    { SYSC_A2IMP23, 0 },   /*   A2IMP23  */
    { SYSC_A2PSC,   0 },   /*   A2PSC    */
    { SYSC_A2DMA,   0 },   /*   A2DMA    */
    { SYSC_A2CV0,   0 },   /*   A2CV0    */
    { SYSC_A2CV1,   0 },   /*   A2CV1    */
    { SYSC_A2CV2,   0 },   /*   A2CV2    */
    { SYSC_A2CV3,   0 },   /*   A2CV3    */
    { SYSC_A1CNN0,  0 },   /*   A1CNN0   */
    { SYSC_A1DSP0,  0 },   /*   A1DSP0   */
    { SYSC_A1DSP1,  0 },   /*   A1DSP1   */
    { SYSC_A1DSP2,  0 },   /*   A1DSP2   */
    { SYSC_A1DSP3,  0 },   /*   A1DSP3   */
};

static st_pma_reset_reg_t reset_reg_v4h[DRIVER_RESET_NUM_V4H] = {
    /* register 0 */
    { CPGDRV_MSSR0_NAME_IMPPSC,         0U },
    { CPGDRV_MSSR0_NAME_IMPDMAC0,       0U },
    { CPGDRV_MSSR0_NAME_IMP1,           0U },
    { CPGDRV_MSSR0_NAME_IMP0,           0U },
    { CPGDRV_MSSR0_NAME_SPMC,           0U },
    { CPGDRV_MSSR0_NAME_IMPCNN,         0U },
    { CPGDRV_MSSR0_NAME_ISP1,           0U },
    { CPGDRV_MSSR0_NAME_ISP0,           0U },
    { CPGDRV_MSSR0_NAME_UMFL0,          0U },
    { CPGDRV_MSSR0_NAME_SMPS0,          0U },
    { CPGDRV_MSSR0_NAME_SMPO0,          0U },
    { CPGDRV_MSSR0_NAME_RGX,            0U },

    /* register 1 */
    { CPGDRV_MSSR1_NAME_ADVFSC,         0U },
    { CPGDRV_MSSR1_NAME_ADG,            0U },
    { CPGDRV_MSSR1_NAME_SPMI,           0U },
    { CPGDRV_MSSR1_NAME_IMPSLV,         0U },
    { CPGDRV_MSSR1_NAME_IMPDTA,         0U },
    { CPGDRV_MSSR1_NAME_IMPDMAC1,       0U },
    { CPGDRV_MSSR1_NAME_IMP3,           0U },
    { CPGDRV_MSSR1_NAME_IMP2,           0U },

    /* register 2 */
    { CPGDRV_MSSR2_NAME_AVB2,           0U },
    { CPGDRV_MSSR2_NAME_AVB1,           0U },
    { CPGDRV_MSSR2_NAME_AVB0,           0U },
    { CPGDRV_MSSR2_NAME_ARMREG,         0U },

    /* register 3 */
    { CPGDRV_MSSR3_NAME_CSITOP0,        0U },
    { CPGDRV_MSSR3_NAME_CSDBGPAP,       0U },
    { CPGDRV_MSSR3_NAME_CR0,            0U },
    { CPGDRV_MSSR3_NAME_CANFD,          0U },

    /* register 4 */
    { CPGDRV_MSSR4_NAME_DSITXLINK1,     0U },
    { CPGDRV_MSSR4_NAME_DSITXLINK0,     0U },
    { CPGDRV_MSSR4_NAME_DOC2CH,         0U },
    { CPGDRV_MSSR4_NAME_DIS0,           0U },
    { CPGDRV_MSSR4_NAME_DDR3_DDR2,      0U },
    { CPGDRV_MSSR4_NAME_DDR1_DDR0,      0U },
    { CPGDRV_MSSR4_NAME_CSITOP1,        0U },

    /* register 5 */
    { CPGDRV_MSSR5_NAME_INTAP,          0U },
    { CPGDRV_MSSR5_NAME_IMS1,           0U },
    { CPGDRV_MSSR5_NAME_IMS0,           0U },
    { CPGDRV_MSSR5_NAME_IMR2,           0U },
    { CPGDRV_MSSR5_NAME_IMR1,           0U },
    { CPGDRV_MSSR5_NAME_IMR0,           0U },
    { CPGDRV_MSSR5_NAME_I2C5,           0U },
    { CPGDRV_MSSR5_NAME_I2C4,           0U },
    { CPGDRV_MSSR5_NAME_I2C3,           0U },
    { CPGDRV_MSSR5_NAME_I2C2,           0U },
    { CPGDRV_MSSR5_NAME_I2C1,           0U },
    { CPGDRV_MSSR5_NAME_I2C0,           0U },
    { CPGDRV_MSSR5_NAME_HSCIF3,         0U },
    { CPGDRV_MSSR5_NAME_HSCIF2,         0U },
    { CPGDRV_MSSR5_NAME_HSCIF1,         0U },
    { CPGDRV_MSSR5_NAME_HSCIF0,         0U },
    { CPGDRV_MSSR5_NAME_FRAY00,         0U },
    { CPGDRV_MSSR5_NAME_FCPVD1,         0U },
    { CPGDRV_MSSR5_NAME_FCPVD0,         0U },
    { CPGDRV_MSSR5_NAME_FCPCS,          0U },

    /* register 6 */
    { CPGDRV_MSSR6_NAME_RTDM1,          0U },
    { CPGDRV_MSSR6_NAME_RTDM0,          0U },
    { CPGDRV_MSSR6_NAME_RPC,            0U },
    { CPGDRV_MSSR6_NAME_PWM,            0U },
    { CPGDRV_MSSR6_NAME_PCIE1,          0U },
    { CPGDRV_MSSR6_NAME_PCIE0,          0U },
    { CPGDRV_MSSR6_NAME_MSI5,           0U },
    { CPGDRV_MSSR6_NAME_MSI4,           0U },
    { CPGDRV_MSSR6_NAME_MSI3,           0U },
    { CPGDRV_MSSR6_NAME_MSI2,           0U },
    { CPGDRV_MSSR6_NAME_MSI1,           0U },
    { CPGDRV_MSSR6_NAME_MSI0,           0U },
    { CPGDRV_MSSR6_NAME_IVCP1E,         0U },
    { CPGDRV_MSSR6_NAME_ISPCS1,         0U },
    { CPGDRV_MSSR6_NAME_ISPCS0,         0U },
    { CPGDRV_MSSR6_NAME_IRQC,           0U },
    { CPGDRV_MSSR6_NAME_IPC,            0U },

    /* register 7 */
    { CPGDRV_MSSR7_NAME_VIN01,          0U },
    { CPGDRV_MSSR7_NAME_VIN00,          0U },
    { CPGDRV_MSSR7_NAME_VCPL4,          0U },
    { CPGDRV_MSSR7_NAME_TPU0,           0U },
    { CPGDRV_MSSR7_NAME_TMU4,           0U },
    { CPGDRV_MSSR7_NAME_TMU3,           0U },
    { CPGDRV_MSSR7_NAME_TMU2,           0U },
    { CPGDRV_MSSR7_NAME_TMU1,           0U },
    { CPGDRV_MSSR7_NAME_TMU0,           0U },
    { CPGDRV_MSSR7_NAME_SYDM2,          0U },
    { CPGDRV_MSSR7_NAME_SYDM1,          0U },
    { CPGDRV_MSSR7_NAME_SDHI0,          0U },
    { CPGDRV_MSSR7_NAME_SCIF4,          0U },
    { CPGDRV_MSSR7_NAME_SCIF3,          0U },
    { CPGDRV_MSSR7_NAME_SCIF2,          0U },
    { CPGDRV_MSSR7_NAME_SCIF1,          0U },
    { CPGDRV_MSSR7_NAME_SCIF0,          0U },
    { CPGDRV_MSSR7_NAME_RTDM3,          0U },
    { CPGDRV_MSSR7_NAME_RTDM2,          0U },

    /* register 8 */
    { CPGDRV_MSSR8_NAME_VSPD1,          0U },
    { CPGDRV_MSSR8_NAME_VSPD0,          0U },
    { CPGDRV_MSSR8_NAME_VIN17,          0U },
    { CPGDRV_MSSR8_NAME_VIN16,          0U },
    { CPGDRV_MSSR8_NAME_VIN15,          0U },
    { CPGDRV_MSSR8_NAME_VIN14,          0U },
    { CPGDRV_MSSR8_NAME_VIN13,          0U },
    { CPGDRV_MSSR8_NAME_VIN12,          0U },
    { CPGDRV_MSSR8_NAME_VIN11,          0U },
    { CPGDRV_MSSR8_NAME_VIN10,          0U },
    { CPGDRV_MSSR8_NAME_VIN07,          0U },
    { CPGDRV_MSSR8_NAME_VIN06,          0U },
    { CPGDRV_MSSR8_NAME_VIN05,          0U },
    { CPGDRV_MSSR8_NAME_VIN04,          0U },
    { CPGDRV_MSSR8_NAME_VIN03,          0U },
    { CPGDRV_MSSR8_NAME_VIN02,          0U },

    /* register 9 */
    { CPGDRV_MSSR9_NAME_UCMT,           0U },
    { CPGDRV_MSSR9_NAME_TSC4_TSC3_TSC2_TSC1, 0U },
    { CPGDRV_MSSR9_NAME_PFC3,           0U },
    { CPGDRV_MSSR9_NAME_PFC2,           0U },
    { CPGDRV_MSSR9_NAME_PFC1,           0U },
    { CPGDRV_MSSR9_NAME_PFC0,           0U },
    { CPGDRV_MSSR9_NAME_CMT3,           0U },
    { CPGDRV_MSSR9_NAME_CMT2,           0U },
    { CPGDRV_MSSR9_NAME_CMT1,           0U },
    { CPGDRV_MSSR9_NAME_CMT0,           0U },
    { CPGDRV_MSSR9_NAME_WDT1_WDT0,      0U },
    { CPGDRV_MSSR9_NAME_WCRC3,          0U },
    { CPGDRV_MSSR9_NAME_WCRC2,          0U },
    { CPGDRV_MSSR9_NAME_WCRC1,          0U },
    { CPGDRV_MSSR9_NAME_WCRC0,          0U },

    /* register 10 */
    { CPGDRV_MSSR10_NAME_VSPX1,         0U },
    { CPGDRV_MSSR10_NAME_VSPX0,         0U },
    { CPGDRV_MSSR10_NAME_RGXC,          0U },
    { CPGDRV_MSSR10_NAME_RGXB,          0U },

    /* register 11 */
    { CPGDRV_MSSR11_NAME_SWDT1_SWDT0,   0U },
    { CPGDRV_MSSR11_NAME_RGXPVC,        0U },
    { CPGDRV_MSSR11_NAME_RGXPVDE,       0U },
    { CPGDRV_MSSR11_NAME_PCIE1,         0U },
    { CPGDRV_MSSR11_NAME_PCIE0,         0U },
    { CPGDRV_MSSR11_NAME_INTAP,         0U },
    { CPGDRV_MSSR11_NAME_FRAY01,        0U },
    { CPGDRV_MSSR11_NAME_AULK4P,        0U },
    { CPGDRV_MSSR11_NAME_AULK2P,        0U },
    { CPGDRV_MSSR11_NAME_FCPVX1,        0U },
    { CPGDRV_MSSR11_NAME_FCPVX0,        0U },

    /* register 12-13 not control */

    /* register 14 */
    { CPGDRV_MSSR14_NAME_CKMMM,         0U },
    { CPGDRV_MSSR14_NAME_CKMIR,         0U },
    { CPGDRV_MSSR14_NAME_CKMVIP,        0U },
    { CPGDRV_MSSR14_NAME_CKMVC,         0U },
    { CPGDRV_MSSR14_NAME_CKMVIO,        0U },
    { CPGDRV_MSSR14_NAME_CKMPE0,        0U },
    { CPGDRV_MSSR14_NAME_CKMRT,         0U },
    { CPGDRV_MSSR14_NAME_CKM3DG,        0U },

    /* register 15 */
    { CPGDRV_MSSR15_NAME_CKMDSP_CKMIR,  0U },
    { CPGDRV_MSSR15_NAME_CKMCNR_CKMIR,  0U },
    { CPGDRV_MSSR15_NAME_CKM,           0U },
    { CPGDRV_MSSR15_NAME_CKMHSC,        0U },
    { CPGDRV_MSSR15_NAME_CKMCR52,       0U },

    /* register 16-26 not control */

    /* register 27 */
    { CPGDRV_MSSR27_NAME_TSN,           0U },
    { CPGDRV_MSSR27_NAME_IMPSDMAC1,     0U },
    { CPGDRV_MSSR27_NAME_IMPSDMAC0,     0U },
    { CPGDRV_MSSR27_NAME_CVE3,          0U },
    { CPGDRV_MSSR27_NAME_CVE2,          0U },
    { CPGDRV_MSSR27_NAME_CVE1,          0U },
    { CPGDRV_MSSR27_NAME_CVE0,          0U },

    /* register 28 */
    { CPGDRV_MSSR28_NAME_VDSP1_REG,     0U },
    { CPGDRV_MSSR28_NAME_VDSP1_CSB,     0U },
    { CPGDRV_MSSR28_NAME_VDSP0_SYSPO,   0U },
    { CPGDRV_MSSR28_NAME_VDSP0_APBDBG,  0U },
    { CPGDRV_MSSR28_NAME_VDSP0_OCEM,    0U },
    { CPGDRV_MSSR28_NAME_VDSP0_CORE,    0U },
    { CPGDRV_MSSR28_NAME_VDSP0_SYS,     0U },
    { CPGDRV_MSSR28_NAME_VDSP0_GLOBAL,  0U },
    { CPGDRV_MSSR28_NAME_VDSP0_REG,     0U },
    { CPGDRV_MSSR28_NAME_VDSP0_CSB,     0U },
    { CPGDRV_MSSR28_NAME_DSC,           0U },
    { CPGDRV_MSSR28_NAME_FCPRC,         0U },
    { CPGDRV_MSSR28_NAME_PAPSDMA,       0U },
    { CPGDRV_MSSR28_NAME_PAPTOP,        0U },
    { CPGDRV_MSSR28_NAME_PAPBUS,        0U },
    { CPGDRV_MSSR28_NAME_VDSP3_BUS,     0U },
    { CPGDRV_MSSR28_NAME_VDSP2_BUS,     0U },
    { CPGDRV_MSSR28_NAME_VDSP1_BUS,     0U },
    { CPGDRV_MSSR28_NAME_VDSP0_BUS,     0U },

    /* register 29 */
    { CPGDRV_MSSR29_NAME_SSI,           0U },
    { CPGDRV_MSSR29_NAME_SSIU,          0U },
    { CPGDRV_MSSR29_NAME_VDSP3_SYSPO,   0U },
    { CPGDRV_MSSR29_NAME_VDSP3_APBDBG,  0U },
    { CPGDRV_MSSR29_NAME_VDSP3_OCEM,    0U },
    { CPGDRV_MSSR29_NAME_VDSP3_CORE,    0U },
    { CPGDRV_MSSR29_NAME_VDSP3_SYS,     0U },
    { CPGDRV_MSSR29_NAME_VDSP3_GLOBAL,  0U },
    { CPGDRV_MSSR29_NAME_VDSP3_REG,     0U },
    { CPGDRV_MSSR29_NAME_VDSP3_CSB,     0U },
    { CPGDRV_MSSR29_NAME_VDSP2_SYSPO,   0U },
    { CPGDRV_MSSR29_NAME_VDSP2_APBDBG,  0U },
    { CPGDRV_MSSR29_NAME_VDSP2_OCEM,    0U },
    { CPGDRV_MSSR29_NAME_VDSP2_CORE,    0U },
    { CPGDRV_MSSR29_NAME_VDSP2_SYS,     0U },
    { CPGDRV_MSSR29_NAME_VDSP2_GLOBAL,  0U },
    { CPGDRV_MSSR29_NAME_VDSP2_REG,     0U },
    { CPGDRV_MSSR29_NAME_VDSP2_CSB,     0U },
    { CPGDRV_MSSR29_NAME_VDSP1_SYSPO,   0U },
    { CPGDRV_MSSR29_NAME_VDSP1_APBDBG,  0U },
    { CPGDRV_MSSR29_NAME_VDSP1_OCEM,    0U },
    { CPGDRV_MSSR29_NAME_VDSP1_CORE,    0U },
    { CPGDRV_MSSR29_NAME_VDSP1_SYS,     0U },
    { CPGDRV_MSSR29_NAME_VDSP1_GLOBAL,  0U }
};

static st_pma_clock_reg_t clock_reg_v4h[DRIVER_CLOCK_NUM_V4H] = {
    /* register 0 */
    { CPGDRV_MSSR0_NAME_IMPPSC,         0U },
    { CPGDRV_MSSR0_NAME_IMPDMAC0,       0U },
    { CPGDRV_MSSR0_NAME_IMP1,           0U },
    { CPGDRV_MSSR0_NAME_IMP0,           0U },
    { CPGDRV_MSSR0_NAME_SPMC,           0U },
    { CPGDRV_MSSR0_NAME_IMPCNN,         0U },
    { CPGDRV_MSSR0_NAME_ISP1,           0U },
    { CPGDRV_MSSR0_NAME_ISP0,           0U },
    { CPGDRV_MSSR0_NAME_UMFL0,          0U },
    { CPGDRV_MSSR0_NAME_SMPS0,          0U },
    { CPGDRV_MSSR0_NAME_SMPO0,          0U },
    { CPGDRV_MSSR0_NAME_RGX,            0U },

    /* register 1 */
    { CPGDRV_MSSR1_NAME_ADVFSC,         0U },
    { CPGDRV_MSSR1_NAME_ADG,            0U },
    { CPGDRV_MSSR1_NAME_SPMI,           0U },
    { CPGDRV_MSSR1_NAME_IMPSLV,         0U },
    { CPGDRV_MSSR1_NAME_IMPDTA,         0U },
    { CPGDRV_MSSR1_NAME_IMPDMAC1,       0U },
    { CPGDRV_MSSR1_NAME_IMP3,           0U },
    { CPGDRV_MSSR1_NAME_IMP2,           0U },

    /* register 2 */
    { CPGDRV_MSSR2_NAME_AVB2,           0U },
    { CPGDRV_MSSR2_NAME_AVB1,           0U },
    { CPGDRV_MSSR2_NAME_AVB0,           0U },

    /* register 3 */
    { CPGDRV_MSSR3_NAME_CSITOP0,        0U },
    { CPGDRV_MSSR3_NAME_CR0,            0U },
    { CPGDRV_MSSR3_NAME_CANFD,          0U },

    /* register 4 */
    { CPGDRV_MSSR4_NAME_DSITXLINK1,     0U },
    { CPGDRV_MSSR4_NAME_DSITXLINK0,     0U },
    { CPGDRV_MSSR4_NAME_DOC2CH,         0U },
    { CPGDRV_MSSR4_NAME_DIS0,           0U },
    { CPGDRV_MSSR4_NAME_DDR3_DDR2,      0U },
    { CPGDRV_MSSR4_NAME_DDR1_DDR0,      0U },
    { CPGDRV_MSSR4_NAME_CSITOP1,        0U },

    /* register 5 */
    { CPGDRV_MSSR5_NAME_INTAP,          0U },
    { CPGDRV_MSSR5_NAME_IMS1,           0U },
    { CPGDRV_MSSR5_NAME_IMS0,           0U },
    { CPGDRV_MSSR5_NAME_IMR2,           0U },
    { CPGDRV_MSSR5_NAME_IMR1,           0U },
    { CPGDRV_MSSR5_NAME_IMR0,           0U },
    { CPGDRV_MSSR5_NAME_I2C5,           0U },
    { CPGDRV_MSSR5_NAME_I2C4,           0U },
    { CPGDRV_MSSR5_NAME_I2C3,           0U },
    { CPGDRV_MSSR5_NAME_I2C2,           0U },
    { CPGDRV_MSSR5_NAME_I2C1,           0U },
    { CPGDRV_MSSR5_NAME_I2C0,           0U },
    { CPGDRV_MSSR5_NAME_HSCIF3,         0U },
    { CPGDRV_MSSR5_NAME_HSCIF2,         0U },
    { CPGDRV_MSSR5_NAME_HSCIF1,         0U },
    { CPGDRV_MSSR5_NAME_HSCIF0,         0U },
    { CPGDRV_MSSR5_NAME_FRAY00,         0U },
    { CPGDRV_MSSR5_NAME_FCPVD1,         0U },
    { CPGDRV_MSSR5_NAME_FCPVD0,         0U },
    { CPGDRV_MSSR5_NAME_FCPCS,          0U },

    /* register 6 */
    { CPGDRV_MSSR6_NAME_RTDM1,          0U },
    { CPGDRV_MSSR6_NAME_RTDM0,          0U },
    { CPGDRV_MSSR6_NAME_RPC,            0U },
    { CPGDRV_MSSR6_NAME_PWM,            0U },
    { CPGDRV_MSSR6_NAME_PCIE1,          0U },
    { CPGDRV_MSSR6_NAME_PCIE0,          0U },
    { CPGDRV_MSSR6_NAME_MSI5,           0U },
    { CPGDRV_MSSR6_NAME_MSI4,           0U },
    { CPGDRV_MSSR6_NAME_MSI3,           0U },
    { CPGDRV_MSSR6_NAME_MSI2,           0U },
    { CPGDRV_MSSR6_NAME_MSI1,           0U },
    { CPGDRV_MSSR6_NAME_MSI0,           0U },
    { CPGDRV_MSSR6_NAME_IVCP1E,         0U },
    { CPGDRV_MSSR6_NAME_ISPCS1,         0U },
    { CPGDRV_MSSR6_NAME_ISPCS0,         0U },
    { CPGDRV_MSSR6_NAME_IRQC,           0U },
    { CPGDRV_MSSR6_NAME_INTTP,          0U },
    { CPGDRV_MSSR6_NAME_IPC,            0U },

    /* register 7 */
    { CPGDRV_MSSR7_NAME_VIN01,          0U },
    { CPGDRV_MSSR7_NAME_VIN00,          0U },
    { CPGDRV_MSSR7_NAME_VCPL4,          0U },
    { CPGDRV_MSSR7_NAME_TPU0,           0U },
    { CPGDRV_MSSR7_NAME_TMU4,           0U },
    { CPGDRV_MSSR7_NAME_TMU3,           0U },
    { CPGDRV_MSSR7_NAME_TMU2,           0U },
    { CPGDRV_MSSR7_NAME_TMU1,           0U },
    { CPGDRV_MSSR7_NAME_TMU0,           0U },
    { CPGDRV_MSSR7_NAME_SYDM2,          0U },
    { CPGDRV_MSSR7_NAME_SYDM1,          0U },
    { CPGDRV_MSSR7_NAME_SDHI0,          0U },
    { CPGDRV_MSSR7_NAME_SCIF4,          0U },
    { CPGDRV_MSSR7_NAME_SCIF3,          0U },
    { CPGDRV_MSSR7_NAME_SCIF2,          0U },
    { CPGDRV_MSSR7_NAME_SCIF1,          0U },
    { CPGDRV_MSSR7_NAME_SCIF0,          0U },
    { CPGDRV_MSSR7_NAME_RTDM3,          0U },
    { CPGDRV_MSSR7_NAME_RTDM2,          0U },

    /* register 8 */
    { CPGDRV_MSSR8_NAME_VSPD1,          0U },
    { CPGDRV_MSSR8_NAME_VSPD0,          0U },
    { CPGDRV_MSSR8_NAME_VIN17,          0U },
    { CPGDRV_MSSR8_NAME_VIN16,          0U },
    { CPGDRV_MSSR8_NAME_VIN15,          0U },
    { CPGDRV_MSSR8_NAME_VIN14,          0U },
    { CPGDRV_MSSR8_NAME_VIN13,          0U },
    { CPGDRV_MSSR8_NAME_VIN12,          0U },
    { CPGDRV_MSSR8_NAME_VIN11,          0U },
    { CPGDRV_MSSR8_NAME_VIN10,          0U },
    { CPGDRV_MSSR8_NAME_VIN07,          0U },
    { CPGDRV_MSSR8_NAME_VIN06,          0U },
    { CPGDRV_MSSR8_NAME_VIN05,          0U },
    { CPGDRV_MSSR8_NAME_VIN04,          0U },
    { CPGDRV_MSSR8_NAME_VIN03,          0U },
    { CPGDRV_MSSR8_NAME_VIN02,          0U },

    /* register 9 */
    { CPGDRV_MSSR9_NAME_UCMT,           0U },
    { CPGDRV_MSSR9_NAME_TSC4_TSC3_TSC2_TSC1, 0U },
    { CPGDRV_MSSR9_NAME_PFC3,           0U },
    { CPGDRV_MSSR9_NAME_PFC2,           0U },
    { CPGDRV_MSSR9_NAME_PFC1,           0U },
    { CPGDRV_MSSR9_NAME_PFC0,           0U },
    { CPGDRV_MSSR9_NAME_CMT3,           0U },
    { CPGDRV_MSSR9_NAME_CMT2,           0U },
    { CPGDRV_MSSR9_NAME_CMT1,           0U },
    { CPGDRV_MSSR9_NAME_CMT0,           0U },
    { CPGDRV_MSSR9_NAME_WDT1_WDT0,      0U },
    { CPGDRV_MSSR9_NAME_WCRC3,          0U },
    { CPGDRV_MSSR9_NAME_WCRC2,          0U },
    { CPGDRV_MSSR9_NAME_WCRC1,          0U },
    { CPGDRV_MSSR9_NAME_WCRC0,          0U },

    /* register 10 */
    { CPGDRV_MSSR10_NAME_VSPX1,         0U },
    { CPGDRV_MSSR10_NAME_VSPX0,         0U },

    /* register 11 */
    { CPGDRV_MSSR11_NAME_AULK4P,        0U },
    { CPGDRV_MSSR11_NAME_AULK2P,        0U },
    { CPGDRV_MSSR11_NAME_FCPVX1,        0U },
    { CPGDRV_MSSR11_NAME_FCPVX0,        0U },

    /* register 12 */
    { CPGDRV_MSSR12_NAME_KCRC4,         0U },
    { CPGDRV_MSSR12_NAME_FSO,           0U },
    { CPGDRV_MSSR12_NAME_CRC3,          0U },
    { CPGDRV_MSSR12_NAME_CRC2,          0U },
    { CPGDRV_MSSR12_NAME_CRC1,          0U },
    { CPGDRV_MSSR12_NAME_CRC0,          0U },
    { CPGDRV_MSSR12_NAME_ADVFSC,        0U },

    /* register 13 */
    { CPGDRV_MSSR13_NAME_MTI,           0U },
    { CPGDRV_MSSR13_NAME_KCRC7,         0U },
    { CPGDRV_MSSR13_NAME_KCRC6,         0U },
    { CPGDRV_MSSR13_NAME_KCRC5,         0U },

    /* register 14-26 not control */

    /* register 27 */
    { CPGDRV_MSSR27_NAME_TSN,           0U },
    { CPGDRV_MSSR27_NAME_IMPSDMAC1,     0U },
    { CPGDRV_MSSR27_NAME_IMPSDMAC0,     0U },
    { CPGDRV_MSSR27_NAME_CVE3,          0U },
    { CPGDRV_MSSR27_NAME_CVE2,          0U },
    { CPGDRV_MSSR27_NAME_CVE1,          0U },
    { CPGDRV_MSSR27_NAME_CVE0,          0U },

    /* register 28 */
    { CPGDRV_MSSR28_NAME_VDSP1_CSB,     0U },
    { CPGDRV_MSSR28_NAME_VDSP0_CSB,     0U },
    { CPGDRV_MSSR28_NAME_DSC,           0U },
    { CPGDRV_MSSR28_NAME_FCPRC,         0U },
    { CPGDRV_MSSR28_NAME_PAPSDMA,       0U },
    { CPGDRV_MSSR28_NAME_PAPTOP,        0U },
    { CPGDRV_MSSR28_NAME_PAPBUS,        0U },
    { CPGDRV_MSSR28_NAME_VDSP3_BUS,     0U },
    { CPGDRV_MSSR28_NAME_VDSP2_BUS,     0U },
    { CPGDRV_MSSR28_NAME_VDSP1_BUS,     0U },
    { CPGDRV_MSSR28_NAME_VDSP0_BUS,     0U },

    /* register 29 */
    { CPGDRV_MSSR29_NAME_SSI,           0U },
    { CPGDRV_MSSR29_NAME_SSIU,          0U },
    { CPGDRV_MSSR29_NAME_VDSP3_CSB,     0U },
    { CPGDRV_MSSR29_NAME_VDSP2_CSB,     0U },
};

static uint32_t reset_init_val[30] = {
    0x00000000U,     /*  SRCR0  */
    0x00000000U,     /*  SRCR1  */
    0x00000000U,     /*  SRCR2  */
    0x00000000U,     /*  SRCR3  */
    0x00000000U,     /*  SRCR4  */
    0x00000000U,     /*  SRCR5  */
    0x00000000U,     /*  SRCR6  */
    0x00000000U,     /*  SRCR7  */
    0x00000000U,     /*  SRCR8  */
    0x00000000U,     /*  SRCR9  */
    0x00000000U,     /*  SRCR10 */
    0x00000000U,     /*  SRCR11 */
    0x00000000U,     /*  SRCR12 */
    0x00000000U,     /*  SRCR13 */
    0x00000000U,     /*  SRCR14 */
    0x00000000U,     /*  SRCR15 */
    0x00000000U,     /*  SRCR16 */
    0x00000000U,     /*  SRCR17 */
    0x00000000U,     /*  SRCR18 */
    0x00000000U,     /*  SRCR19 */
    0x00000000U,     /*  SRCR20 */
    0x00000000U,     /*  SRCR21 */
    0x00000000U,     /*  SRCR22 */
    0x00000000U,     /*  SRCR23 */
    0x00000000U,     /*  SRCR24 */
    0x00000000U,     /*  SRCR25 */
    0x00000000U,     /*  SRCR26 */
    0x00000000U,     /*  SRCR27 */
    0x00000000U,     /*  SRCR28 */
    0x00000000U      /*  SRCR29 */
};

static uint32_t clock_init_val[30] = {
    0x00000000U,     /*  MSTPSR0  */
    0x00000000U,     /*  MSTPSR1  */
    0x00000000U,     /*  MSTPSR2  */
    0x00000000U,     /*  MSTPSR3  */
    0x00000000U,     /*  MSTPSR4  */
    0x00000000U,     /*  MSTPSR5  */
    0x00000000U,     /*  MSTPSR6  */
    0x00000000U,     /*  MSTPSR7  */
    0x00000000U,     /*  MSTPSR8  */
    0x00000000U,     /*  MSTPSR9  */
    0x00000000U,     /*  MSTPSR10 */
    0x00000000U,     /*  MSTPSR11 */
    0x00000000U,     /*  MSTPSR12 */
    0x00000000U,     /*  MSTPSR13 */
    0x00000000U,     /*  MSTPSR14 */
    0x00000000U,     /*  MSTPSR15 */
    0x00000000U,     /*  MSTPSR16 */
    0x00000000U,     /*  MSTPSR17 */
    0x00000000U,     /*  MSTPSR18 */
    0x00000000U,     /*  MSTPSR19 */
    0x00000000U,     /*  MSTPSR20 */
    0x00000000U,     /*  MSTPSR21 */
    0x00000000U,     /*  MSTPSR22 */
    0x00000000U,     /*  MSTPSR23 */
    0x00000000U,     /*  MSTPSR24 */
    0x00000000U,     /*  MSTPSR25 */
    0x00000000U,     /*  MSTPSR26 */
    0x00000000U,     /*  MSTPSR27 */
    0x00000000U,     /*  MSTPSR28 */
    0x00000000U      /*  MSTPSR29 */
};

static st_pma_reg_t gs_pma_reg;
static int gs_sysc_fd;
static int gs_cpg_fd;
static void *cpg_base_addr = NULL;

/*======================================================================================================================
Private function
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function power_call_back()
***********************************************************************************************************************/
static void power_call_back(int32_t ret)
{
    (void)ret;

    /* Do nothing */

    return ;
}
/***********************************************************************************************************************
* End of function power_call_back()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function sysc_power_on_driver()
***********************************************************************************************************************/
static void sysc_power_on_driver(int fd, st_pma_power_reg_t *power)
{
    dcmd_pd_on_t        request;
    dcmd_pd_sts_get_t   get_sts;
    int                 system_ret;

    if (power != NULL)
    {
        get_sts.pd_array[0]  = power->pdr;
        system_ret = ioctl(fd, SYSC_DCMD_PD_STATUS_GET, &get_sts);
        if (0 != system_ret)
        {
            return;
        }
        else
        {
            power->value = get_sts.result;
        }

        request.power_domain = power->pdr;
        request.ret_func     = power_call_back;
        system_ret = ioctl(fd, SYSC_DCMD_PD_ON, &request);
        if (0 != system_ret)
        {            return;
        }

        get_sts.pd_array[0]  = power->pdr;
        while (1)
        {
            system_ret = ioctl(fd, SYSC_DCMD_PD_STATUS_GET, &get_sts);
            if ((0 != system_ret) || (0U == get_sts.result))
            {
                break;
            }
        }
    }

    return;
}
/***********************************************************************************************************************
* End of function sysc_power_on_driver()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function sysc_power_off_driver()
***********************************************************************************************************************/
static void sysc_power_off_driver(int fd, st_pma_power_reg_t *power)
{
    (void)fd;
    (void)power;

#if 0
    dcmd_pd_off_t       request;
    dcmd_pd_sts_get_t   get_sts;
    int                 system_ret;

    if (power != NULL)
    {
        request.power_domain = power->pdr;
        request.ret_func     = power_call_back;
        system_ret = ioctl(fd, SYSC_DCMD_PD_OFF, &request);
        if (0 != system_ret)
        {
            return;
        }

        get_sts.pd_array[0]  = power->pdr;
        while (1)
        {
            system_ret = ioctl(fd, SYSC_DCMD_PD_STATUS_GET, &get_sts);
            if ((0 != system_ret) || (1U == get_sts.result))
            {
                break;
            }
        }
    }
#endif

    return;
}
/***********************************************************************************************************************
* End of function sysc_power_off_driver()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function cpg_clock_on_driver()
***********************************************************************************************************************/
static void cpg_clock_on_driver(int fd, st_pma_clock_reg_t *clock)
{
    ST_CPGDRV_FRQ_REQ   req;
    int                 system_ret;

    if (clock != NULL)
    {
        req.i32_req_clk = clock->module;
        system_ret = ioctl(fd, CPGDRV_MSSR_ON, &req);
        if (0 != system_ret)
        {
            return;
        }
    }

    return;
}
/***********************************************************************************************************************
* End of function cpg_clock_on_driver()
***********************************************************************************************************************/
#if 0
/***********************************************************************************************************************
* Start of function cpg_clock_off_driver()
***********************************************************************************************************************/
static void cpg_clock_off_driver(int fd, st_pma_clock_reg_t *clock)
{
    ST_CPGDRV_FRQ_REQ   req;
    int                 system_ret;

    if (clock != NULL)
    {
        req.i32_req_clk = clock->module;
        system_ret = ioctl(fd, CPGDRV_MSSR_OFF, &req);
        if (0 != system_ret)
        {
            return;
        }
    }

    return;
}
/***********************************************************************************************************************
* End of function cpg_clock_off_driver()
***********************************************************************************************************************/
#endif
/***********************************************************************************************************************
* Start of function cpg_reset_clear_driver()
***********************************************************************************************************************/
static void cpg_reset_clear_driver(int fd, st_pma_reset_reg_t *reset)
{
    ST_CPGDRV_FRQ_REQ   req;
    int                 system_ret;

    if (reset != NULL)
    {
        req.i32_req_clk = reset->module;
        system_ret = ioctl(fd, CPGDRV_MSSR_SOFTRST_OFF, &req);
        if (0 != system_ret)
        {
            return;
        }
    }

    return;
}
/***********************************************************************************************************************
* End of function cpg_reset_clear_driver()
***********************************************************************************************************************/
#if 0
/***********************************************************************************************************************
* Start of function cpg_reset_driver()
***********************************************************************************************************************/
static void cpg_reset_driver(int fd, st_pma_reset_reg_t *reset)
{
    ST_CPGDRV_FRQ_REQ   req;
    int                 system_ret;

    if (reset != NULL)
    {
        req.i32_req_clk = reset->module;
        system_ret = ioctl(fd, CPGDRV_MSSR_SOFTRST_ON, &req);
        if (0 != system_ret)
        {
            return;
        }
    }

    return;
}
/***********************************************************************************************************************
* End of function cpg_reset_driver()
***********************************************************************************************************************/
#endif
/***********************************************************************************************************************
* Start of function cpg_clock_on()
***********************************************************************************************************************/
static void cpg_clock_on(uintptr_t base_reg, uint32_t reg_num, uint32_t set_val)
{
    uint32_t    val;
    uintptr_t   reg;

    if (0x0u != base_reg)
    {
        /* Read the current value and set the bit position of the specified module to 0.(read MSTPCRn) */
        reg = base_reg + PMA_PM_OFFSET_MSTPCR + (reg_num * 4u);
        val = *((volatile uint32_t *)reg);
        val = val & (~set_val);

        /* Writes a bit-reversed value to Write Protect Register.(write CPGWPR) */
        reg = base_reg + PMA_PM_OFFSET_CPGWPR;
        *((volatile uint32_t *)reg) = ~val;

        /* Clock supply(write MSTPCRn) */
        reg = base_reg + PMA_PM_OFFSET_MSTPCR + (reg_num * 4u);
        *((volatile uint32_t *)reg) = val;

        /* Clock Supply Confirmation(check MSTPSRn) */
        reg = base_reg + PMA_PM_OFFSET_MSTPSR + (reg_num * 4u);
        while (1) {
            val = *((volatile uint32_t *)reg);
            if ((val & set_val) == 0) {
                break;
            }
        }
    }

    return;
}
/***********************************************************************************************************************
* End of function cpg_clock_on()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function cpg_clock_off()
***********************************************************************************************************************/
static void cpg_clock_off(uintptr_t base_reg, uint32_t num, uint32_t set_val)
{
    uint32_t    val;
    uintptr_t   reg;

    if (0x0u != base_reg)
    {
        /* Read the current value and set the bit position of the specified module to 1.(read MSTPCRn) */
        reg = base_reg + PMA_PM_OFFSET_MSTPCR + (num * 4u);
        val = *((volatile uint32_t *)reg);
        val |= set_val;

        /* Writes a bit-reversed value to Write Protect Register.(write CPGWPR) */
        reg = base_reg + PMA_PM_OFFSET_CPGWPR;
        *((volatile uint32_t *)reg) = ~val;

        /* Stop Clock Supply(write MSTPCRn) */
        reg = base_reg + PMA_PM_OFFSET_MSTPCR + (num * 4u);
        *((volatile uint32_t *)reg) = val;

        /* Clock Supply Stop Confirmation(check MSTPSRn) */
        reg = base_reg + PMA_PM_OFFSET_MSTPSR + (num * 4u);
        while (1) {
            val = *((volatile uint32_t *)reg);
            if ((val & set_val) == set_val) {
                break;
            }
        }
    }

    return;
}
/***********************************************************************************************************************
* End of function cpg_clock_off()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function cpg_reset_clear()
***********************************************************************************************************************/
static void cpg_reset_clear(uintptr_t base_reg, uint32_t reg_num, uint32_t set_val)
{
    uint32_t    val;
    uintptr_t   reg;

    if (0x0u != base_reg)
    {
        /* Read the current value and set the bit position of the specified module to 1.(read SRCRn) */
        reg = base_reg + PMA_PM_OFFSET_SRCR + (reg_num * 4u);
        val = *((volatile uint32_t *)reg);
        val = val | set_val;

        /* Writes a bit-reversed value to Write Protect Register.(write CPGWPR) */
        reg = base_reg + PMA_PM_OFFSET_CPGWPR;
        *((volatile uint32_t *)reg) = ~val;

        /* Clear reset(write SRSTCLRn) */
        reg = base_reg + PMA_PM_OFFSET_SRSTCLR + (reg_num * 4u);
        *((volatile uint32_t *)reg) = val;
    }

    return;
}
/***********************************************************************************************************************
* End of function cpg_reset_clear()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function cpg_reset()
***********************************************************************************************************************/
static void cpg_reset(uintptr_t base_reg, uint32_t reg_num, uint32_t set_val)
{
    uint32_t    val;
    uintptr_t   reg;

    if (0x0u != base_reg)
    {
        /* Read the current value and set the bit position of the specified module to 1.(read SRCRn) */
        reg = base_reg + PMA_PM_OFFSET_SRCR + (reg_num * 4u);
        val = *((volatile uint32_t *)reg);
        val |= set_val;

        /* Writes a bit-reversed value to Write Protect Register.(write CPGWPR) */
        reg = base_reg + PMA_PM_OFFSET_CPGWPR;
        *((volatile uint32_t *)reg) = ~val;

        /* module reset(write SRCRn) */
        reg = base_reg + PMA_PM_OFFSET_SRCR + (reg_num * 4u);
        *((volatile uint32_t *)reg) = val;
    }

    return;
}
/***********************************************************************************************************************
* End of function cpg_reset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_RegInfoInit()
***********************************************************************************************************************/
static void R_PMA_IF_RegInfoInit(uint64_t osal_soc_type)
{
    switch(osal_soc_type)
    {
        case OSAL_DEVICE_SOC_TYPE_V4H:
            gs_pma_reg.pdr_num          = DRIVER_PDR_NUM_V4H;
            gs_pma_reg.power_reg        = power_reg_v4h;
            gs_pma_reg.clock_reg_num    = DRIVER_CLOCK_NUM_V4H;
            gs_pma_reg.clock_reg        = clock_reg_v4h;
            gs_pma_reg.reset_reg_num    = DRIVER_RESET_NUM_V4H;
            gs_pma_reg.reset_reg        = reset_reg_v4h;
            break;
        default:
            gs_pma_reg.pdr_num          = 0;
            gs_pma_reg.power_reg        = NULL;
            gs_pma_reg.clock_reg_num    = 0;
            gs_pma_reg.clock_reg        = NULL;
            gs_pma_reg.reset_reg_num    = 0;
            gs_pma_reg.reset_reg        = NULL;
    }

    return;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_RegInfoInit()
***********************************************************************************************************************/

/*======================================================================================================================
Public function
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function R_PMA_IF_RegInit()
***********************************************************************************************************************/
e_pma_return_t R_PMA_IF_RegInit(uint64_t osal_soc_typev)
{
    uint32_t    i;
    int         pdev_cpg_fd;
    uintptr_t   reg;

    R_PMA_IF_RegInfoInit(osal_soc_typev);

    /* SYSC Driver Open */
    gs_sysc_fd = open("/dev/sysc", O_RDWR);
    if (-1 == gs_sysc_fd)
    {
        return R_PMA_ERR_IO;
    }

    /* CPG Driver Open */
    gs_cpg_fd = open("/dev/cpg", O_RDWR);
    if (-1 == gs_cpg_fd)
    {
        (void)close(gs_sysc_fd);
        return R_PMA_ERR_IO;
    }

    /* Power Domain ON */
    for (i = 0; i < gs_pma_reg.pdr_num; i++)
    {
        sysc_power_on_driver(gs_sysc_fd, &gs_pma_reg.power_reg[i]);
    }

    /* control other module */
    if (OSAL_DEVICE_SOC_TYPE_V4H == osal_soc_typev)
    {
        pdev_cpg_fd = open("/pdev/cpg", O_RDWR);
        if (-1 == pdev_cpg_fd)
        {
            return R_PMA_ERR_IO;
        }

        cpg_base_addr  = mmap(NULL,
                              (size_t)PMA_PM_CPG_SIZE,
                              (PROT_READ | PROT_WRITE | PROT_NOCACHE),
                              (MAP_SHARED | MAP_POPULATE),
                              pdev_cpg_fd,
                              (uint64_t)PMA_PM_CPG_BASE_ADDR);
        if (MAP_FAILED == cpg_base_addr)
        {
            return R_PMA_ERR_IO;
        }

        for (i = 0; i < 30; i++)
        {
            /* get initial value */
            reg = (uintptr_t)cpg_base_addr + PMA_PM_OFFSET_MSTPCR + (i * 4u);
            clock_init_val[i] = *((volatile uint32_t *)reg);
        }

        for (i = 0; i < 30; i++)
        {
            /* get initial value */
            reg = (uintptr_t)cpg_base_addr + PMA_PM_OFFSET_SRCR + (i * 4u);
            reset_init_val[i] = *((volatile uint32_t *)reg);
        }

        /* caiplite */
        cpg_clock_on((uintptr_t)cpg_base_addr, 7U, 0x1FF00000);
        cpg_reset_clear((uintptr_t)cpg_base_addr, 7U, 0x1FF00000);
        /* INTC-Monitor (INTTP) */
        cpg_reset_clear((uintptr_t)cpg_base_addr, 6U, 0x00000100);

        (void)close(pdev_cpg_fd);
    }

    /* Clock Supply */
    for(i = 0; i < gs_pma_reg.clock_reg_num; i++)
    {
        cpg_clock_on_driver(gs_cpg_fd, &gs_pma_reg.clock_reg[i]);
    }

    /* Reset Clearing */
    for(i = 0; i < gs_pma_reg.reset_reg_num; i++)
    {
        cpg_reset_clear_driver(gs_cpg_fd, &gs_pma_reg.reset_reg[i]);
    }

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_RegInit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_IF_RegDeinit()
***********************************************************************************************************************/
e_pma_return_t R_PMA_IF_RegDeinit(void)
{
    uint32_t    i;

    /* Reset */
    for(i = 0; i < 30; i++)
    {
        cpg_reset((uintptr_t)cpg_base_addr, i, reset_init_val[i]);
    }

    /* Stop Clock Supply  */
    for (i = 0; i < 30; i++)
    {
        cpg_clock_off((uintptr_t)cpg_base_addr, i, clock_init_val[i]);
    }

    /* Power OFF  */
    if (0 < gs_pma_reg.pdr_num)
    {
        for(i = (gs_pma_reg.pdr_num - 1) ; (int32_t)i >= 0; i--)
        {
            if(1U == gs_pma_reg.power_reg[i].value)
            {
                sysc_power_off_driver(gs_sysc_fd, &gs_pma_reg.power_reg[i]);
            }
        }
    }

    (void)munmap(cpg_base_addr, PMA_PM_CPG_SIZE);
    cpg_base_addr = NULL;
    (void)close(gs_sysc_fd);
    (void)close(gs_cpg_fd);

    return R_PMA_RESULT_OK;
}
/***********************************************************************************************************************
* End of function R_PMA_IF_RegDeinit()
***********************************************************************************************************************/

/*======================================================================================================================
End of file
======================================================================================================================*/
