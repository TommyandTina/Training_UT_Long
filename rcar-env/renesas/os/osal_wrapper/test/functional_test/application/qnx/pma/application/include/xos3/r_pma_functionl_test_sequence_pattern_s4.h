#ifndef R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_S4_H
#define R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_S4_H

#include "r_pma_functionl_test_sequence.h"

st_expected_info_t expected_info_without_sysc =
{
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_1  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_2  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_3  */
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_4  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_5  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_6  */
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_7  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_ENABLED,  /* SEQUENCE_TEST_SCENARIO_8  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_9  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_10 */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_11 */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG        /* SEQUENCE_TEST_SCENARIO_12 */
};


const st_expected_info_t * gp_expected_info_tbl[R_PMA_HWA_ID_S4_NUM] = 
{
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CSDBGPAP            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CR0                 */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_DDR1_DDR0           */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_I2C5                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_I2C4                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_I2C3                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_I2C2                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_I2C1                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_I2C0                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_HSCIF3              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_HSCIF2              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_HSCIF1              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_HSCIF0              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_RTDM1               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_RTDM0               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_RPC                 */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_PCIE10              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_PCIE00              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_MSI3                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_MSI2                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_MSI1                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_MSI0                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC7            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC6            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC5            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC4            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC3            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC2            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC1            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC0            */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AES_ACC_WRAPPER     */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SHIP_S0             */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_TMU4                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_TMU3                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_TMU2                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_TMU1                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_TMU0                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SYDM2               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SYDM1               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SDHI0               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SDHI                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SCIF4               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SCIF3               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SCIF1               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SCIF0               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_RTDM3               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_RTDM2               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_UCMT                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_TSC4_TSC3_TSC2_TSC1 */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_PFC0                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CMT3                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CMT2                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CMT1                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CMT0                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WDT1_WDT0           */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WCRC3               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WCRC2               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WCRC1               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WCRC0               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SWDT1_SWDT0         */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_PCIE11              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_PCIE01              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_AULK2P              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_KCRC4               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CRC3                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CRC2                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CRC1                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CRC0                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SCMT                */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_PFC0D3              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_PFC0D2              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_PFC0D1              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT9_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT8_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT7_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT6_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT5_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT4_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT3_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT2_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT1_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_WWDT0_RCLK          */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_KCRC7               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_KCRC6               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_KCRC5               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CKMMM               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CKMPE0              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CKMRT               */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CKM                 */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CKMHSC              */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_CKMCR52             */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_UFS                 */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SHIP_S2             */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_SHIP_S1             */
    &expected_info_without_sysc,  /* R_PMA_HWA_ID_S4_ETHER_TSN_IF        */
    &expected_info_without_sysc   /* R_PMA_HWA_ID_S4_R_SWITCH2           */
};

const uint32_t gp_vdsp_info_flg[R_PMA_HWA_ID_V4H_NUM] = 
{
    0,              /* R_PMA_HWA_ID_S4_CSDBGPAP            */
    0,              /* R_PMA_HWA_ID_S4_CR0                 */
    1,              /* R_PMA_HWA_ID_S4_DDR1_DDR0           */
    0,              /* R_PMA_HWA_ID_S4_I2C5                */
    0,              /* R_PMA_HWA_ID_S4_I2C4                */
    0,              /* R_PMA_HWA_ID_S4_I2C3                */
    0,              /* R_PMA_HWA_ID_S4_I2C2                */
    0,              /* R_PMA_HWA_ID_S4_I2C1                */
    0,              /* R_PMA_HWA_ID_S4_I2C0                */
    0,              /* R_PMA_HWA_ID_S4_HSCIF3              */
    0,              /* R_PMA_HWA_ID_S4_HSCIF2              */
    0,              /* R_PMA_HWA_ID_S4_HSCIF1              */
    0,              /* R_PMA_HWA_ID_S4_HSCIF0              */
    0,              /* R_PMA_HWA_ID_S4_RTDM1               */
    0,              /* R_PMA_HWA_ID_S4_RTDM0               */
    0,              /* R_PMA_HWA_ID_S4_RPC                 */
    0,              /* R_PMA_HWA_ID_S4_PCIE10              */
    0,              /* R_PMA_HWA_ID_S4_PCIE00              */
    0,              /* R_PMA_HWA_ID_S4_MSI3                */
    0,              /* R_PMA_HWA_ID_S4_MSI2                */
    0,              /* R_PMA_HWA_ID_S4_MSI1                */
    0,              /* R_PMA_HWA_ID_S4_MSI0                */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC7            */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC6            */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC5            */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC4            */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC3            */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC2            */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC1            */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC0            */
    0,              /* R_PMA_HWA_ID_S4_AES_ACC_WRAPPER     */
    0,              /* R_PMA_HWA_ID_S4_SHIP_S0             */
    0,              /* R_PMA_HWA_ID_S4_TMU4                */
    0,              /* R_PMA_HWA_ID_S4_TMU3                */
    0,              /* R_PMA_HWA_ID_S4_TMU2                */
    0,              /* R_PMA_HWA_ID_S4_TMU1                */
    0,              /* R_PMA_HWA_ID_S4_TMU0                */
    0,              /* R_PMA_HWA_ID_S4_SYDM2               */
    0,              /* R_PMA_HWA_ID_S4_SYDM1               */
    0,              /* R_PMA_HWA_ID_S4_SDHI0               */
    0,              /* R_PMA_HWA_ID_S4_SDHI                */
    0,              /* R_PMA_HWA_ID_S4_SCIF4               */
    0,              /* R_PMA_HWA_ID_S4_SCIF3               */
    0,              /* R_PMA_HWA_ID_S4_SCIF1               */
    0,              /* R_PMA_HWA_ID_S4_SCIF0               */
    0,              /* R_PMA_HWA_ID_S4_RTDM3               */
    0,              /* R_PMA_HWA_ID_S4_RTDM2               */
    0,              /* R_PMA_HWA_ID_S4_UCMT                */
    0,              /* R_PMA_HWA_ID_S4_TSC4_TSC3_TSC2_TSC1 */
    0,              /* R_PMA_HWA_ID_S4_PFC0                */
    0,              /* R_PMA_HWA_ID_S4_CMT3                */
    0,              /* R_PMA_HWA_ID_S4_CMT2                */
    0,              /* R_PMA_HWA_ID_S4_CMT1                */
    0,              /* R_PMA_HWA_ID_S4_CMT0                */
    0,              /* R_PMA_HWA_ID_S4_WDT1_WDT0           */
    0,              /* R_PMA_HWA_ID_S4_WCRC3               */
    0,              /* R_PMA_HWA_ID_S4_WCRC2               */
    0,              /* R_PMA_HWA_ID_S4_WCRC1               */
    0,              /* R_PMA_HWA_ID_S4_WCRC0               */
    1,              /* R_PMA_HWA_ID_S4_SWDT1_SWDT0         */
    1,              /* R_PMA_HWA_ID_S4_PCIE11              */
    1,              /* R_PMA_HWA_ID_S4_PCIE01              */
    1,              /* R_PMA_HWA_ID_S4_AULK2P              */
    0,              /* R_PMA_HWA_ID_S4_KCRC4               */
    0,              /* R_PMA_HWA_ID_S4_CRC3                */
    0,              /* R_PMA_HWA_ID_S4_CRC2                */
    0,              /* R_PMA_HWA_ID_S4_CRC1                */
    0,              /* R_PMA_HWA_ID_S4_CRC0                */
    0,              /* R_PMA_HWA_ID_S4_SCMT                */
    0,              /* R_PMA_HWA_ID_S4_PFC0D3              */
    0,              /* R_PMA_HWA_ID_S4_PFC0D2              */
    0,              /* R_PMA_HWA_ID_S4_PFC0D1              */
    0,              /* R_PMA_HWA_ID_S4_WWDT9_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT8_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT7_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT6_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT5_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT4_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT3_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT2_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT1_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_WWDT0_RCLK          */
    0,              /* R_PMA_HWA_ID_S4_KCRC7               */
    0,              /* R_PMA_HWA_ID_S4_KCRC6               */
    0,              /* R_PMA_HWA_ID_S4_KCRC5               */
    0,              /* R_PMA_HWA_ID_S4_CKMMM               */
    0,              /* R_PMA_HWA_ID_S4_CKMPE0              */
    0,              /* R_PMA_HWA_ID_S4_CKMRT               */
    0,              /* R_PMA_HWA_ID_S4_CKM                 */
    0,              /* R_PMA_HWA_ID_S4_CKMHSC              */
    0,              /* R_PMA_HWA_ID_S4_CKMCR52             */
    0,              /* R_PMA_HWA_ID_S4_UFS                 */
    0,              /* R_PMA_HWA_ID_S4_SHIP_S2             */
    0,              /* R_PMA_HWA_ID_S4_SHIP_S1             */
    0,              /* R_PMA_HWA_ID_S4_ETHER_TSN_IF        */
    0               /* R_PMA_HWA_ID_S4_R_SWITCH2           */
};

#endif  /* R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_S4_H */
