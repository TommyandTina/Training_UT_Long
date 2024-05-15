#ifndef R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_V4H_H
#define R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_V4H_H

#include "r_pma_functionl_test_sequence.h"

st_expected_info_t expected_info =
{
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_1  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_OFF, R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_2  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_3  */
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_4  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* SEQUENCE_TEST_SCENARIO_5  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_OFF, R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_6  */
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_7  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_ENABLED,  /* SEQUENCE_TEST_SCENARIO_8  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_OFF, R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_9  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_10 */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_11 */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG        /* SEQUENCE_TEST_SCENARIO_12 */
};

st_expected_info_t expected_info_impdta =
{
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* Unused  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_2  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* Unused  */
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* Unused  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_CG,       /* Unused  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_OFF, R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* Unused  */
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* Unused  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_ENABLED,  /* Unused  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_OFF, R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* Unused  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* Unused  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* Unused  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG        /* Unused  */
};


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

st_expected_info_t expected_info_without_cpg =
{
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_1  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_OFF, R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_2  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_3  */
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_4  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_ENABLED,  /* SEQUENCE_TEST_SCENARIO_5  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_OFF, R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_6  */
        R_PMA_HWA_REQUESTED, R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_7  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_ENABLED,  /* SEQUENCE_TEST_SCENARIO_8  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_OFF, R_PMA_STATE_OFF, R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_9  */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_10 */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG,       /* SEQUENCE_TEST_SCENARIO_11 */
        R_PMA_HWA_RELEASED,  R_PMA_STATE_ON,  R_PMA_STATE_ON,  R_PMA_UNDEFINED_RESET, R_PMA_STATE_PG        /* SEQUENCE_TEST_SCENARIO_12 */
};

const st_expected_info_t * gp_expected_info_tbl_impdta = 
{
    &expected_info_impdta        /* R_PMA_HWA_ID_V4H_IMPDTA */
};

const st_expected_info_t * gp_expected_info_tbl[R_PMA_HWA_ID_V4H_NUM] = 
{
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMPPSC */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMPDMAC0 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMP1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMP0 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_SPMC */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMPCNN */
    &expected_info,              /* R_PMA_HWA_ID_V4H_ISP1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_ISP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANRT1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_DOF */
    &expected_info,              /* R_PMA_HWA_ID_V4H_SMPS0 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_SMPO0 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_RGX */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANVI1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANVI0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANVC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANVC0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANSP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANPV0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ADVFSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ADG */
    &expected_info,              /* R_PMA_HWA_ID_V4H_SPMI */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMPSLV */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMPDTA */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXIMP0_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXIMP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANIMP1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANIMP0 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMPDMAC1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMP3 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMP2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSM_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSD0_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSD0_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRS_AXSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRT_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRD_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRD_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRC_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRC_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRS_AXPV */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXPV */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANMM_AXMM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AVB2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AVB1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AVB0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ARMREG */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ARMM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APSP3_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APSP3_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APSP4_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APSP4_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APRT0_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APRT0_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANVIP2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANVIP1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANVIP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CSITOP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CSDBGPAP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CR0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CANFD */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVIP1_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVIP1_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVIP0_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVIP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVIFF */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVI1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVI1_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVI */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVC_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXTBH */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSTM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSTM_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRS_AXSP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSN */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXRS_AXSN */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXSM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_DSITXLINK1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_DSITXLINK0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_DOC2CH */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_DIS0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_DDR3_DDR2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_DDR1_DDR0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CSITOP1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_INTAP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_IMS1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_IMS0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_IMR2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_IMR1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_IMR0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_I2C5 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_I2C4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_I2C3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_I2C2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_I2C1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_I2C0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_HSCIF3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_HSCIF2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_HSCIF1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_HSCIF0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_FRAY00 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_FCPVD1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_FCPVD0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_FCPCS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RTDM1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RTDM0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RPC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PWM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PCIE10 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PCIE00 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_MSI5 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_MSI4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_MSI3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_MSI2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_MSI1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_MSI0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_IVCP1E */
    &expected_info,              /* R_PMA_HWA_ID_V4H_ISPCS1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_ISPCS0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_IRQC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_INTTP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_IPC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN01 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN00 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VCPL4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC7 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC6 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC5 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AES_ACC_WRAP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_TPU0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_TMU4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_TMU3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_TMU2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_TMU1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_TMU0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SYDM2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SYDM1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SECROM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SDHI0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SCIF4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SCIF3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SCIF1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SCIF0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RTDM3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RTDM2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VSPD1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VSPD0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN17 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN16 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN15 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN14 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN13 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN12 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN11 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN10 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN07 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN06 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN05 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN04 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN03 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_VIN02 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_UCMT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CMT3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CMT2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CMT1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CMT0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WDT1_WDT0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WCRC3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WCRC2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WCRC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WCRC0 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VSPX1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VSPX0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RGXC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RGXB */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SWDT1_SWDT0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RGXPVC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_RGXPVDE */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PCIE11 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PCIE01 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_INTAP11 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_FRAY01 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXVI11 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AULK4P */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AULK2P */
    &expected_info,              /* R_PMA_HWA_ID_V4H_FCPVX1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_FCPVX0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_KCRC4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_FSO */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CRC3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CRC2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CRC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CRC0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ADVFS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SCMT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC3D3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC3D2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC3D1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC2D3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC2D2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC2D1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC1D3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC1D2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC1D1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC0D3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC0D2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_PFC0D1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT9_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT8_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT7_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT6_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT5_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT4_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT3_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT2_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT1_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT0_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_DBE */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_MTI */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_KCRC7 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_KCRC6 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_KCRC5 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMMM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMIR */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMVIP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMVC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMVIO */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMPE0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKM3DG */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APVI4_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APVI4_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APVC1_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_APVC1_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMDSP_CKMIR */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMCNR_CKMIR */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMHSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CKMCR52 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_ANHC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXHC_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_AXHC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CSBRG_IR_A2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_CSBRG_IR_A3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_TSN */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMPSDMAC1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_IMPSDMAC0 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_CVE3 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_CVE2 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_CVE1 */
    &expected_info,              /* R_PMA_HWA_ID_V4H_CVE0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP1_REG */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VDSP1_CSB */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP0_SYSPO */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP0_APBDBG */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP0_OCEM */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP0_CORE */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP0_SYS */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP0_GLOBAL */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP0_REG */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VDSP0_CSB */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_DSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_FCPRC */
    &expected_info,              /* R_PMA_HWA_ID_V4H_PAPSDMA */
    &expected_info,              /* R_PMA_HWA_ID_V4H_HSM */
    &expected_info,              /* R_PMA_HWA_ID_V4H_PAPBUS */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VDSP3_BUS */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VDSP2_BUS */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VDSP1_BUS */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VDSP0_BUS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SSI */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4H_SSIU */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP3_SYSPO */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP3_APBDBG */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP3_OCEM */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP3_CORE */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP3_SYS */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP3_GLOBAL */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP3_REG */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VDSP3_CSB */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP2_SYSPO */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP2_APBDBG */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP2_OCEM */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP2_CORE */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP2_SYS */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP2_GLOBAL */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP2_REG */
    &expected_info,              /* R_PMA_HWA_ID_V4H_VDSP2_CSB */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP1_SYSPO */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP1_APBDBG */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP1_OCEM */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP1_CORE */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP1_SYS */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_VDSP1_GLOBAL */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR */
    &expected_info,              /* R_PMA_HWA_ID_V4H_REG_SPMI  */
    &expected_info,              /* R_PMA_HWA_ID_V4H_REG_SPMC  */
    &expected_info,              /* R_PMA_HWA_ID_V4H_DOF_FCPRM */
    &expected_info,              /* R_PMA_HWA_ID_V4H_DCM       */
    &expected_info,              /* R_PMA_HWA_ID_V4H_CBF       */
    &expected_info,              /* R_PMA_HWA_ID_V4H_SBF       */
    &expected_info,              /* R_PMA_HWA_ID_V4H_OSM       */
    &expected_info,              /* R_PMA_HWA_ID_V4H_ISM0      */
    &expected_info,              /* R_PMA_HWA_ID_V4H_ISM1      */
    &expected_info,              /* R_PMA_HWA_ID_V4H_FBS       */
    &expected_info,              /* R_PMA_HWA_ID_V4H_APB       */
    &expected_info,              /* R_PMA_HWA_ID_V4H_DUL       */
    &expected_info               /* R_PMA_HWA_ID_V4H_SPMC01    */
};

const uint32_t gp_vdsp_info_flg[R_PMA_HWA_ID_V4H_NUM] = 
{
    0,              /* R_PMA_HWA_ID_V4H_IMPPSC */
    0,              /* R_PMA_HWA_ID_V4H_IMPDMAC0 */
    0,              /* R_PMA_HWA_ID_V4H_IMP1 */
    0,              /* R_PMA_HWA_ID_V4H_IMP0 */
    0,              /* R_PMA_HWA_ID_V4H_SPMC */
    0,              /* R_PMA_HWA_ID_V4H_IMPCNN */
    0,              /* R_PMA_HWA_ID_V4H_ISP1 */
    0,              /* R_PMA_HWA_ID_V4H_ISP0 */
    0,              /* R_PMA_HWA_ID_V4H_ANRT1 */
    0,              /* R_PMA_HWA_ID_V4H_DOF */
    0,              /* R_PMA_HWA_ID_V4H_SMPS0 */
    0,              /* R_PMA_HWA_ID_V4H_SMPO0 */
    0,              /* R_PMA_HWA_ID_V4H_RGX */
    0,              /* R_PMA_HWA_ID_V4H_ANVI1 */
    0,              /* R_PMA_HWA_ID_V4H_ANVI0 */
    0,              /* R_PMA_HWA_ID_V4H_ANVC1 */
    0,              /* R_PMA_HWA_ID_V4H_ANVC0 */
    0,              /* R_PMA_HWA_ID_V4H_ANSP0 */
    0,              /* R_PMA_HWA_ID_V4H_ANRT */
    0,              /* R_PMA_HWA_ID_V4H_ANPV0 */
    0,              /* R_PMA_HWA_ID_V4H_ADVFSC */
    0,              /* R_PMA_HWA_ID_V4H_ADG */
    0,              /* R_PMA_HWA_ID_V4H_SPMI */
    0,              /* R_PMA_HWA_ID_V4H_IMPSLV */
    0,              /* R_PMA_HWA_ID_V4H_IMPDTA */
    0,              /* R_PMA_HWA_ID_V4H_AXIMP0_AXRS */
    0,              /* R_PMA_HWA_ID_V4H_AXIMP0 */
    0,              /* R_PMA_HWA_ID_V4H_ANIMP1 */
    0,              /* R_PMA_HWA_ID_V4H_ANIMP0 */
    0,              /* R_PMA_HWA_ID_V4H_IMPDMAC1 */
    0,              /* R_PMA_HWA_ID_V4H_IMP3 */
    0,              /* R_PMA_HWA_ID_V4H_IMP2 */
    0,              /* R_PMA_HWA_ID_V4H_AXSM_AXRS */
    0,              /* R_PMA_HWA_ID_V4H_AXSD0_1 */
    0,              /* R_PMA_HWA_ID_V4H_AXSD0_0 */
    0,              /* R_PMA_HWA_ID_V4H_AXSC */
    0,              /* R_PMA_HWA_ID_V4H_AXRS_AXSC */
    0,              /* R_PMA_HWA_ID_V4H_AXRT_AXRS */
    0,              /* R_PMA_HWA_ID_V4H_AXRT */
    0,              /* R_PMA_HWA_ID_V4H_AXRD_1 */
    0,              /* R_PMA_HWA_ID_V4H_AXRD_0 */
    0,              /* R_PMA_HWA_ID_V4H_AXRC_1 */
    0,              /* R_PMA_HWA_ID_V4H_AXRC_0 */
    0,              /* R_PMA_HWA_ID_V4H_AXRS_AXPV */
    0,              /* R_PMA_HWA_ID_V4H_AXPV */
    0,              /* R_PMA_HWA_ID_V4H_ANMM_AXMM */
    0,              /* R_PMA_HWA_ID_V4H_AVB2 */
    0,              /* R_PMA_HWA_ID_V4H_AVB1 */
    0,              /* R_PMA_HWA_ID_V4H_AVB0 */
    0,              /* R_PMA_HWA_ID_V4H_ARMREG */
    0,              /* R_PMA_HWA_ID_V4H_ARMM */
    0,              /* R_PMA_HWA_ID_V4H_APSP3_1 */
    0,              /* R_PMA_HWA_ID_V4H_APSP3_0 */
    0,              /* R_PMA_HWA_ID_V4H_APSP4_1 */
    0,              /* R_PMA_HWA_ID_V4H_APSP4_0 */
    0,              /* R_PMA_HWA_ID_V4H_APRT0_1 */
    0,              /* R_PMA_HWA_ID_V4H_APRT0_0 */
    0,              /* R_PMA_HWA_ID_V4H_ANVIP2 */
    0,              /* R_PMA_HWA_ID_V4H_ANVIP1 */
    0,              /* R_PMA_HWA_ID_V4H_ANVIP0 */
    0,              /* R_PMA_HWA_ID_V4H_CSITOP0 */
    0,              /* R_PMA_HWA_ID_V4H_CSDBGPAP */
    0,              /* R_PMA_HWA_ID_V4H_CR0 */
    0,              /* R_PMA_HWA_ID_V4H_CANFD */
    0,              /* R_PMA_HWA_ID_V4H_AXVIP1_1 */
    0,              /* R_PMA_HWA_ID_V4H_AXVIP1_0 */
    0,              /* R_PMA_HWA_ID_V4H_AXVIP0_AXRS */
    0,              /* R_PMA_HWA_ID_V4H_AXVIP0 */
    0,              /* R_PMA_HWA_ID_V4H_AXVIFF */
    0,              /* R_PMA_HWA_ID_V4H_AXVI1 */
    0,              /* R_PMA_HWA_ID_V4H_AXVI1_AXRS */
    0,              /* R_PMA_HWA_ID_V4H_AXVI */
    0,              /* R_PMA_HWA_ID_V4H_AXVC_AXRS */
    0,              /* R_PMA_HWA_ID_V4H_AXVC */
    0,              /* R_PMA_HWA_ID_V4H_AXTBH */
    0,              /* R_PMA_HWA_ID_V4H_AXSTM */
    0,              /* R_PMA_HWA_ID_V4H_AXSTM_AXRS */
    0,              /* R_PMA_HWA_ID_V4H_AXRS_AXSP0 */
    0,              /* R_PMA_HWA_ID_V4H_AXSP0 */
    0,              /* R_PMA_HWA_ID_V4H_AXSN */
    0,              /* R_PMA_HWA_ID_V4H_AXRS_AXSN */
    0,              /* R_PMA_HWA_ID_V4H_AXSM */
    0,              /* R_PMA_HWA_ID_V4H_DSITXLINK1 */
    0,              /* R_PMA_HWA_ID_V4H_DSITXLINK0 */
    0,              /* R_PMA_HWA_ID_V4H_DOC2CH */
    0,              /* R_PMA_HWA_ID_V4H_DIS0 */
    0,              /* R_PMA_HWA_ID_V4H_DDR3_DDR2 */
    0,              /* R_PMA_HWA_ID_V4H_DDR1_DDR0 */
    0,              /* R_PMA_HWA_ID_V4H_CSITOP1 */
    0,              /* R_PMA_HWA_ID_V4H_INTAP */
    0,              /* R_PMA_HWA_ID_V4H_IMS1 */
    0,              /* R_PMA_HWA_ID_V4H_IMS0 */
    0,              /* R_PMA_HWA_ID_V4H_IMR2 */
    0,              /* R_PMA_HWA_ID_V4H_IMR1 */
    0,              /* R_PMA_HWA_ID_V4H_IMR0 */
    0,              /* R_PMA_HWA_ID_V4H_I2C5 */
    0,              /* R_PMA_HWA_ID_V4H_I2C4 */
    0,              /* R_PMA_HWA_ID_V4H_I2C3 */
    0,              /* R_PMA_HWA_ID_V4H_I2C2 */
    0,              /* R_PMA_HWA_ID_V4H_I2C1 */
    0,              /* R_PMA_HWA_ID_V4H_I2C0 */
    0,              /* R_PMA_HWA_ID_V4H_HSCIF3 */
    0,              /* R_PMA_HWA_ID_V4H_HSCIF2 */
    0,              /* R_PMA_HWA_ID_V4H_HSCIF1 */
    0,              /* R_PMA_HWA_ID_V4H_HSCIF0 */
    0,              /* R_PMA_HWA_ID_V4H_FRAY00 */
    0,              /* R_PMA_HWA_ID_V4H_FCPVD1 */
    0,              /* R_PMA_HWA_ID_V4H_FCPVD0 */
    0,              /* R_PMA_HWA_ID_V4H_FCPCS */
    0,              /* R_PMA_HWA_ID_V4H_RTDM1 */
    0,              /* R_PMA_HWA_ID_V4H_RTDM0 */
    0,              /* R_PMA_HWA_ID_V4H_RPC */
    0,              /* R_PMA_HWA_ID_V4H_PWM */
    0,              /* R_PMA_HWA_ID_V4H_PCIE10 */
    0,              /* R_PMA_HWA_ID_V4H_PCIE00 */
    0,              /* R_PMA_HWA_ID_V4H_MSI5 */
    0,              /* R_PMA_HWA_ID_V4H_MSI4 */
    0,              /* R_PMA_HWA_ID_V4H_MSI3 */
    0,              /* R_PMA_HWA_ID_V4H_MSI2 */
    0,              /* R_PMA_HWA_ID_V4H_MSI1 */
    0,              /* R_PMA_HWA_ID_V4H_MSI0 */
    0,              /* R_PMA_HWA_ID_V4H_IVCP1E */
    0,              /* R_PMA_HWA_ID_V4H_ISPCS1 */
    0,              /* R_PMA_HWA_ID_V4H_ISPCS0 */
    0,              /* R_PMA_HWA_ID_V4H_IRQC */
    0,              /* R_PMA_HWA_ID_V4H_INTTP */
    0,              /* R_PMA_HWA_ID_V4H_IPC */
    0,              /* R_PMA_HWA_ID_V4H_VIN01 */
    0,              /* R_PMA_HWA_ID_V4H_VIN00 */
    0,              /* R_PMA_HWA_ID_V4H_VCPL4 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC7 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC6 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC5 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC4 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC3 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC2 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC1 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC0 */
    0,              /* R_PMA_HWA_ID_V4H_AES_ACC_WRAP */
    0,              /* R_PMA_HWA_ID_V4H_TPU0 */
    0,              /* R_PMA_HWA_ID_V4H_TMU4 */
    0,              /* R_PMA_HWA_ID_V4H_TMU3 */
    0,              /* R_PMA_HWA_ID_V4H_TMU2 */
    0,              /* R_PMA_HWA_ID_V4H_TMU1 */
    0,              /* R_PMA_HWA_ID_V4H_TMU0 */
    0,              /* R_PMA_HWA_ID_V4H_SYDM2 */
    0,              /* R_PMA_HWA_ID_V4H_SYDM1 */
    0,              /* R_PMA_HWA_ID_V4H_SECROM */
    0,              /* R_PMA_HWA_ID_V4H_SDHI0 */
    0,              /* R_PMA_HWA_ID_V4H_SCIF4 */
    0,              /* R_PMA_HWA_ID_V4H_SCIF3 */
    0,              /* R_PMA_HWA_ID_V4H_SCIF1 */
    0,              /* R_PMA_HWA_ID_V4H_SCIF0 */
    0,              /* R_PMA_HWA_ID_V4H_RTDM3 */
    0,              /* R_PMA_HWA_ID_V4H_RTDM2 */
    0,              /* R_PMA_HWA_ID_V4H_VSPD1 */
    0,              /* R_PMA_HWA_ID_V4H_VSPD0 */
    0,              /* R_PMA_HWA_ID_V4H_VIN17 */
    0,              /* R_PMA_HWA_ID_V4H_VIN16 */
    0,              /* R_PMA_HWA_ID_V4H_VIN15 */
    0,              /* R_PMA_HWA_ID_V4H_VIN14 */
    0,              /* R_PMA_HWA_ID_V4H_VIN13 */
    0,              /* R_PMA_HWA_ID_V4H_VIN12 */
    0,              /* R_PMA_HWA_ID_V4H_VIN11 */
    0,              /* R_PMA_HWA_ID_V4H_VIN10 */
    0,              /* R_PMA_HWA_ID_V4H_VIN07 */
    0,              /* R_PMA_HWA_ID_V4H_VIN06 */
    0,              /* R_PMA_HWA_ID_V4H_VIN05 */
    0,              /* R_PMA_HWA_ID_V4H_VIN04 */
    0,              /* R_PMA_HWA_ID_V4H_VIN03 */
    0,              /* R_PMA_HWA_ID_V4H_VIN02 */
    0,              /* R_PMA_HWA_ID_V4H_UCMT */
    0,              /* R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1 */
    0,              /* R_PMA_HWA_ID_V4H_PFC3 */
    0,              /* R_PMA_HWA_ID_V4H_PFC2 */
    0,              /* R_PMA_HWA_ID_V4H_PFC1 */
    0,              /* R_PMA_HWA_ID_V4H_PFC0 */
    0,              /* R_PMA_HWA_ID_V4H_CMT3 */
    0,              /* R_PMA_HWA_ID_V4H_CMT2 */
    0,              /* R_PMA_HWA_ID_V4H_CMT1 */
    0,              /* R_PMA_HWA_ID_V4H_CMT0 */
    0,              /* R_PMA_HWA_ID_V4H_WDT1_WDT0 */
    0,              /* R_PMA_HWA_ID_V4H_WCRC3 */
    0,              /* R_PMA_HWA_ID_V4H_WCRC2 */
    0,              /* R_PMA_HWA_ID_V4H_WCRC1 */
    0,              /* R_PMA_HWA_ID_V4H_WCRC0 */
    0,              /* R_PMA_HWA_ID_V4H_VSPX1 */
    0,              /* R_PMA_HWA_ID_V4H_VSPX0 */
    0,              /* R_PMA_HWA_ID_V4H_RGXC */
    0,              /* R_PMA_HWA_ID_V4H_RGXB */
    0,              /* R_PMA_HWA_ID_V4H_SWDT1_SWDT0 */
    0,              /* R_PMA_HWA_ID_V4H_RGXPVC */
    0,              /* R_PMA_HWA_ID_V4H_RGXPVDE */
    0,              /* R_PMA_HWA_ID_V4H_PCIE11 */
    0,              /* R_PMA_HWA_ID_V4H_PCIE01 */
    0,              /* R_PMA_HWA_ID_V4H_INTAP11 */
    0,              /* R_PMA_HWA_ID_V4H_FRAY01 */
    0,              /* R_PMA_HWA_ID_V4H_AXVI11 */
    0,              /* R_PMA_HWA_ID_V4H_AULK4P */
    0,              /* R_PMA_HWA_ID_V4H_AULK2P */
    0,              /* R_PMA_HWA_ID_V4H_FCPVX1 */
    0,              /* R_PMA_HWA_ID_V4H_FCPVX0 */
    0,              /* R_PMA_HWA_ID_V4H_KCRC4 */
    0,              /* R_PMA_HWA_ID_V4H_FSO */
    0,              /* R_PMA_HWA_ID_V4H_CRC3 */
    0,              /* R_PMA_HWA_ID_V4H_CRC2 */
    0,              /* R_PMA_HWA_ID_V4H_CRC1 */
    0,              /* R_PMA_HWA_ID_V4H_CRC0 */
    0,              /* R_PMA_HWA_ID_V4H_ADVFS */
    0,              /* R_PMA_HWA_ID_V4H_SCMT */
    0,              /* R_PMA_HWA_ID_V4H_PFC3D3 */
    0,              /* R_PMA_HWA_ID_V4H_PFC3D2 */
    0,              /* R_PMA_HWA_ID_V4H_PFC3D1 */
    0,              /* R_PMA_HWA_ID_V4H_PFC2D3 */
    0,              /* R_PMA_HWA_ID_V4H_PFC2D2 */
    0,              /* R_PMA_HWA_ID_V4H_PFC2D1 */
    0,              /* R_PMA_HWA_ID_V4H_PFC1D3 */
    0,              /* R_PMA_HWA_ID_V4H_PFC1D2 */
    0,              /* R_PMA_HWA_ID_V4H_PFC1D1 */
    0,              /* R_PMA_HWA_ID_V4H_PFC0D3 */
    0,              /* R_PMA_HWA_ID_V4H_PFC0D2 */
    0,              /* R_PMA_HWA_ID_V4H_PFC0D1 */
    0,              /* R_PMA_HWA_ID_V4H_WWDT9_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT8_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT7_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT6_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT5_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT4_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT3_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT2_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT1_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT0_RCLK */
    0,              /* R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT */
    0,              /* R_PMA_HWA_ID_V4H_DBE */
    0,              /* R_PMA_HWA_ID_V4H_MTI */
    0,              /* R_PMA_HWA_ID_V4H_KCRC7 */
    0,              /* R_PMA_HWA_ID_V4H_KCRC6 */
    0,              /* R_PMA_HWA_ID_V4H_KCRC5 */
    0,              /* R_PMA_HWA_ID_V4H_CKMMM */
    0,              /* R_PMA_HWA_ID_V4H_CKMIR */
    0,              /* R_PMA_HWA_ID_V4H_CKMVIP */
    0,              /* R_PMA_HWA_ID_V4H_CKMVC */
    0,              /* R_PMA_HWA_ID_V4H_CKMVIO */
    0,              /* R_PMA_HWA_ID_V4H_CKMPE0 */
    0,              /* R_PMA_HWA_ID_V4H_CKMRT */
    0,              /* R_PMA_HWA_ID_V4H_CKM3DG */
    0,              /* R_PMA_HWA_ID_V4H_APVI4_1 */
    0,              /* R_PMA_HWA_ID_V4H_APVI4_0 */
    0,              /* R_PMA_HWA_ID_V4H_APVC1_1 */
    0,              /* R_PMA_HWA_ID_V4H_APVC1_0 */
    0,              /* R_PMA_HWA_ID_V4H_CKMDSP_CKMIR */
    0,              /* R_PMA_HWA_ID_V4H_CKMCNR_CKMIR */
    0,              /* R_PMA_HWA_ID_V4H_CKM */
    0,              /* R_PMA_HWA_ID_V4H_CKMHSC */
    0,              /* R_PMA_HWA_ID_V4H_CKMCR52 */
    0,              /* R_PMA_HWA_ID_V4H_ANHC */
    0,              /* R_PMA_HWA_ID_V4H_AXHC_AXRS */
    0,              /* R_PMA_HWA_ID_V4H_AXHC */
    0,              /* R_PMA_HWA_ID_V4H_CSBRG_IR_A2 */
    0,              /* R_PMA_HWA_ID_V4H_CSBRG_IR_A3 */
    0,              /* R_PMA_HWA_ID_V4H_TSN */
    0,              /* R_PMA_HWA_ID_V4H_IMPSDMAC1 */
    0,              /* R_PMA_HWA_ID_V4H_IMPSDMAC0 */
    0,              /* R_PMA_HWA_ID_V4H_CVE3 */
    0,              /* R_PMA_HWA_ID_V4H_CVE2 */
    0,              /* R_PMA_HWA_ID_V4H_CVE1 */
    0,              /* R_PMA_HWA_ID_V4H_CVE0 */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_REG */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_CSB */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_SYSPO */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_APBDBG */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_OCEM */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_CORE */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_SYS */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_GLOBAL */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_REG */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_CSB */
    0,              /* R_PMA_HWA_ID_V4H_DSC */
    0,              /* R_PMA_HWA_ID_V4H_FCPRC */
    0,              /* R_PMA_HWA_ID_V4H_PAPSDMA */
    0,              /* R_PMA_HWA_ID_V4H_HSM */
    0,              /* R_PMA_HWA_ID_V4H_PAPBUS */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_BUS */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_BUS */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_BUS */
    1,              /* R_PMA_HWA_ID_V4H_VDSP0_BUS */
    0,              /* R_PMA_HWA_ID_V4H_SSI */
    0,              /* R_PMA_HWA_ID_V4H_SSIU */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_SYSPO */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_APBDBG */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_OCEM */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_CORE */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_SYS */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_GLOBAL */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_REG */
    1,              /* R_PMA_HWA_ID_V4H_VDSP3_CSB */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_SYSPO */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_APBDBG */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_OCEM */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_CORE */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_SYS */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_GLOBAL */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_REG */
    1,              /* R_PMA_HWA_ID_V4H_VDSP2_CSB */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_SYSPO */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_APBDBG */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_OCEM */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_CORE */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_SYS */
    1,              /* R_PMA_HWA_ID_V4H_VDSP1_GLOBAL */
    0,              /* R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR */
    0,              /* R_PMA_HWA_ID_V4H_REG_SPMI  */
    0,              /* R_PMA_HWA_ID_V4H_REG_SPMC  */
    0,              /* R_PMA_HWA_ID_V4H_DOF_FCPRM */
    0,              /* R_PMA_HWA_ID_V4H_DCM       */
    0,              /* R_PMA_HWA_ID_V4H_CBF       */
    0,              /* R_PMA_HWA_ID_V4H_SBF       */
    0,              /* R_PMA_HWA_ID_V4H_OSM       */
    0,              /* R_PMA_HWA_ID_V4H_ISM0      */
    0,              /* R_PMA_HWA_ID_V4H_ISM1      */
    0,              /* R_PMA_HWA_ID_V4H_FBS       */
    0,              /* R_PMA_HWA_ID_V4H_APB       */
    0,              /* R_PMA_HWA_ID_V4H_DUL       */
    0               /* R_PMA_HWA_ID_V4H_SPMC01    */
};

st_share_domain_info_t hwa_info_NONE =
{
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t cpg_info_DOF =
{
    R_PMA_HWA_ID_V4H_DOF, R_PMA_HWA_ID_V4H_DOF_FCPRM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t cpg_info_SPMC =
{
    R_PMA_HWA_ID_V4H_SPMC, R_PMA_HWA_ID_V4H_REG_SPMC, R_PMA_HWA_ID_V4H_SPMC01, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t cpg_info_PAPTOP =
{
    R_PMA_HWA_ID_V4H_HSM, R_PMA_HWA_ID_V4H_DCM, R_PMA_HWA_ID_V4H_SBF, R_PMA_HWA_ID_V4H_OSM, R_PMA_HWA_ID_V4H_ISM0, 
    R_PMA_HWA_ID_V4H_ISM1, R_PMA_HWA_ID_V4H_FBS, R_PMA_HWA_ID_V4H_APB, R_PMA_HWA_ID_V4H_DUL, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t cpg_info_SPMI =
{
    R_PMA_HWA_ID_V4H_SPMI, R_PMA_HWA_ID_V4H_REG_SPMI, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A2CN0 =
{
    R_PMA_HWA_ID_V4H_SPMC, R_PMA_HWA_ID_V4H_REG_SPMC, R_PMA_HWA_ID_V4H_SPMC01, R_PMA_HWA_ID_V4H_IMPSDMAC1, R_PMA_HWA_ID_V4H_IMPSDMAC0,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A3DUL =
{
    R_PMA_HWA_ID_V4H_HSM, R_PMA_HWA_ID_V4H_DCM, R_PMA_HWA_ID_V4H_SBF, R_PMA_HWA_ID_V4H_OSM, R_PMA_HWA_ID_V4H_ISM0,
    R_PMA_HWA_ID_V4H_ISM1, R_PMA_HWA_ID_V4H_FBS, R_PMA_HWA_ID_V4H_APB, R_PMA_HWA_ID_V4H_DUL, R_PMA_HWA_ID_V4H_PAPSDMA,
    R_PMA_HWA_ID_V4H_PAPBUS, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A2PSC =
{
    R_PMA_HWA_ID_V4H_IMPPSC, R_PMA_HWA_ID_V4H_IMPDMAC0, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A2IMP01 =
{
    R_PMA_HWA_ID_V4H_IMP1, R_PMA_HWA_ID_V4H_IMP0, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A3ISP1 =
{
    R_PMA_HWA_ID_V4H_ISP1, R_PMA_HWA_ID_V4H_ISPCS1, R_PMA_HWA_ID_V4H_VSPX1, R_PMA_HWA_ID_V4H_FCPVX1, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A3ISP0 =
{
    R_PMA_HWA_ID_V4H_ISP0, R_PMA_HWA_ID_V4H_ISPCS0, R_PMA_HWA_ID_V4H_VSPX0, R_PMA_HWA_ID_V4H_FCPVX0, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A3VIP2 =
{
    R_PMA_HWA_ID_V4H_SMPS0, R_PMA_HWA_ID_V4H_SMPO0, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A3IR =
{
    R_PMA_HWA_ID_V4H_SPMI, R_PMA_HWA_ID_V4H_IMPSLV, R_PMA_HWA_ID_V4H_IMPDTA, R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR, R_PMA_HWA_ID_V4H_REG_SPMI,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A2IMP23 =
{
    R_PMA_HWA_ID_V4H_IMP3, R_PMA_HWA_ID_V4H_IMP2, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A1DSP1 =
{
    R_PMA_HWA_ID_V4H_VDSP1_BUS, R_PMA_HWA_ID_V4H_VDSP1_CSB, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A1DSP3 =
{
    R_PMA_HWA_ID_V4H_VDSP3_BUS, R_PMA_HWA_ID_V4H_VDSP3_CSB, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A1DSP2 =
{
    R_PMA_HWA_ID_V4H_VDSP2_BUS, R_PMA_HWA_ID_V4H_VDSP2_CSB, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

st_share_domain_info_t sysc_info_A1DSP0 =
{
    R_PMA_HWA_ID_V4H_VDSP0_CSB, R_PMA_HWA_ID_V4H_VDSP0_BUS, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM,
    R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM , R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM, R_PMA_HWA_ID_V4H_NUM
};

const st_share_domain_info_t * gp_share_cpg_info[R_PMA_HWA_ID_V4H_NUM] = 
{
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMPPSC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMPDMAC0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMP1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMP0 */
    &cpg_info_SPMC,                     /* R_PMA_HWA_ID_V4H_SPMC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMPCNN */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ISP1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ISP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANRT1 */
    &cpg_info_DOF,                      /* R_PMA_HWA_ID_V4H_DOF */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SMPS0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SMPO0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RGX */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANVI1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANVI0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANVC1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANVC0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANSP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANPV0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ADVFSC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ADG */
    &cpg_info_SPMI,                     /* R_PMA_HWA_ID_V4H_SPMI */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMPSLV */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMPDTA */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXIMP0_AXRS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXIMP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANIMP1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANIMP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMPDMAC1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMP3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMP2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSM_AXRS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSD0_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSD0_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRS_AXSC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRT_AXRS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRD_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRD_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRC_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRC_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRS_AXPV */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXPV */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANMM_AXMM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AVB2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AVB1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AVB0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ARMREG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ARMM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APSP3_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APSP3_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APSP4_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APSP4_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APRT0_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APRT0_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANVIP2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANVIP1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANVIP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CSITOP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CSDBGPAP */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CR0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CANFD */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVIP1_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVIP1_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVIP0_AXRS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVIP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVIFF */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVI1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVI1_AXRS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVI */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVC_AXRS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXTBH */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSTM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSTM_AXRS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRS_AXSP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSP0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSN */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXRS_AXSN */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXSM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_DSITXLINK1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_DSITXLINK0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_DOC2CH */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_DIS0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_DDR3_DDR2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_DDR1_DDR0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CSITOP1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_INTAP */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMS1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMS0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMR2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMR1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMR0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_I2C5 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_I2C4 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_I2C3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_I2C2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_I2C1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_I2C0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_HSCIF3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_HSCIF2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_HSCIF1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_HSCIF0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FRAY00 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FCPVD1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FCPVD0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FCPCS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RTDM1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RTDM0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RPC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PWM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PCIE10 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PCIE00 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_MSI5 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_MSI4 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_MSI3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_MSI2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_MSI1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_MSI0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IVCP1E */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ISPCS1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ISPCS0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IRQC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_INTTP */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IPC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN01 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN00 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VCPL4 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC7 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC6 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC5 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC4 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AES_ACC_WRAP */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_TPU0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_TMU4 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_TMU3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_TMU2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_TMU1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_TMU0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SYDM2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SYDM1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SECROM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SDHI0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SCIF4 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SCIF3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SCIF1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SCIF0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RTDM3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RTDM2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VSPD1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VSPD0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN17 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN16 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN15 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN14 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN13 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN12 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN11 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN10 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN07 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN06 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN05 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN04 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN03 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VIN02 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_UCMT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CMT3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CMT2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CMT1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CMT0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WDT1_WDT0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WCRC3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WCRC2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WCRC1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WCRC0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VSPX1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VSPX0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RGXC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RGXB */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SWDT1_SWDT0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RGXPVC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_RGXPVDE */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PCIE11 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PCIE01 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_INTAP11 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FRAY01 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXVI11 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AULK4P */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AULK2P */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FCPVX1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FCPVX0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_KCRC4 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FSO */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CRC3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CRC2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CRC1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CRC0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ADVFS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SCMT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC3D3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC3D2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC3D1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC2D3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC2D2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC2D1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC1D3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC1D2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC1D1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC0D3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC0D2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PFC0D1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT9_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT8_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT7_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT6_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT5_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT4_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT3_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT2_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT1_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT0_RCLK */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_DBE */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_MTI */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_KCRC7 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_KCRC6 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_KCRC5 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMMM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMIR */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMVIP */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMVC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMVIO */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMPE0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMRT */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKM3DG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APVI4_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APVI4_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APVC1_1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_APVC1_0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMDSP_CKMIR */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMCNR_CKMIR */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMHSC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CKMCR52 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_ANHC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXHC_AXRS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_AXHC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CSBRG_IR_A2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CSBRG_IR_A3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_TSN */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMPSDMAC1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IMPSDMAC0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CVE3 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CVE2 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CVE1 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_CVE0 */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_REG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_CSB */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_SYSPO */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_APBDBG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_OCEM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_CORE */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_SYS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_GLOBAL */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_REG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_CSB */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_DSC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_FCPRC */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PAPSDMA */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_HSM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_PAPBUS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_BUS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_BUS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_BUS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP0_BUS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SSI */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_SSIU */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_SYSPO */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_APBDBG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_OCEM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_CORE */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_SYS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_GLOBAL */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_REG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP3_CSB */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_SYSPO */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_APBDBG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_OCEM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_CORE */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_SYS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_GLOBAL */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_REG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP2_CSB */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_SYSPO */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_APBDBG */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_OCEM */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_CORE */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_SYS */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_VDSP1_GLOBAL */
    &hwa_info_NONE,                     /* R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR */
    &cpg_info_SPMI,                     /* R_PMA_H  WA_ID_V4H_REG_SPMI  */
    &cpg_info_SPMC,                     /* R_PMA_H  WA_ID_V4H_REG_SPMC  */
    &cpg_info_DOF,                      /* R_PMA_H  WA_ID_V4H_DOF_FCPRM */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_DCM       */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_CBF       */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_SBF       */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_OSM       */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_ISM0      */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_ISM1      */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_FBS       */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_APB       */
    &cpg_info_PAPTOP,                   /* R_PMA_H  WA_ID_V4H_DUL       */
    &cpg_info_SPMC,                     /* R_PMA_H  WA_ID_V4H_SPMC01    */
};

const st_share_domain_info_t * gp_share_sysc_info[R_PMA_HWA_ID_V4H_NUM] = 
{
    &sysc_info_A2PSC,                      /* R_PMA_HWA_ID_V4H_IMPPSC */
    &sysc_info_A2PSC,                      /* R_PMA_HWA_ID_V4H_IMPDMAC0 */
    &sysc_info_A2IMP01,                    /* R_PMA_HWA_ID_V4H_IMP1 */
    &sysc_info_A2IMP01,                    /* R_PMA_HWA_ID_V4H_IMP0 */
    &sysc_info_A2CN0,                      /* R_PMA_HWA_ID_V4H_SPMC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IMPCNN */
    &sysc_info_A3ISP1,                     /* R_PMA_HWA_ID_V4H_ISP1 */
    &sysc_info_A3ISP0,                     /* R_PMA_HWA_ID_V4H_ISP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANRT1 */
    &cpg_info_DOF,                         /* R_PMA_HWA_ID_V4H_DOF */
    &sysc_info_A3VIP2,                     /* R_PMA_HWA_ID_V4H_SMPS0 */
    &sysc_info_A3VIP2,                     /* R_PMA_HWA_ID_V4H_SMPO0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RGX */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANVI1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANVI0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANVC1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANVC0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANSP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANPV0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ADVFSC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ADG */
    &sysc_info_A3IR,                       /* R_PMA_HWA_ID_V4H_SPMI */
    &sysc_info_A3IR,                       /* R_PMA_HWA_ID_V4H_IMPSLV */
    &sysc_info_A3IR,                       /* R_PMA_HWA_ID_V4H_IMPDTA */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXIMP0_AXRS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXIMP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANIMP1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANIMP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IMPDMAC1 */
    &sysc_info_A2IMP23,                    /* R_PMA_HWA_ID_V4H_IMP3 */
    &sysc_info_A2IMP23,                    /* R_PMA_HWA_ID_V4H_IMP2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSM_AXRS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSD0_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSD0_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRS_AXSC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRT_AXRS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRD_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRD_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRC_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRC_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRS_AXPV */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXPV */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANMM_AXMM */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AVB2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AVB1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AVB0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ARMREG */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ARMM */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APSP3_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APSP3_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APSP4_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APSP4_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APRT0_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APRT0_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANVIP2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANVIP1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANVIP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CSITOP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CSDBGPAP */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CR0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CANFD */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVIP1_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVIP1_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVIP0_AXRS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVIP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVIFF */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVI1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVI1_AXRS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVI */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVC_AXRS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXTBH */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSTM */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSTM_AXRS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRS_AXSP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSP0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSN */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXRS_AXSN */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXSM */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_DSITXLINK1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_DSITXLINK0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_DOC2CH */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_DIS0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_DDR3_DDR2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_DDR1_DDR0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CSITOP1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_INTAP */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IMS1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IMS0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IMR2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IMR1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IMR0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_I2C5 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_I2C4 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_I2C3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_I2C2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_I2C1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_I2C0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_HSCIF3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_HSCIF2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_HSCIF1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_HSCIF0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_FRAY00 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_FCPVD1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_FCPVD0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_FCPCS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RTDM1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RTDM0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RPC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PWM */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PCIE10 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PCIE00 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_MSI5 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_MSI4 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_MSI3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_MSI2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_MSI1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_MSI0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IVCP1E */
    &sysc_info_A3ISP1,                     /* R_PMA_HWA_ID_V4H_ISPCS1 */
    &sysc_info_A3ISP0,                     /* R_PMA_HWA_ID_V4H_ISPCS0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IRQC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_INTTP */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_IPC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN01 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN00 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VCPL4 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC7 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC6 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC5 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC4 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AES_ACC_WRAP */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_TPU0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_TMU4 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_TMU3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_TMU2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_TMU1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_TMU0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SYDM2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SYDM1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SECROM */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SDHI0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SCIF4 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SCIF3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SCIF1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SCIF0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RTDM3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RTDM2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VSPD1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VSPD0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN17 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN16 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN15 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN14 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN13 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN12 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN11 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN10 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN07 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN06 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN05 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN04 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN03 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_VIN02 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_UCMT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CMT3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CMT2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CMT1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CMT0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WDT1_WDT0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WCRC3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WCRC2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WCRC1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WCRC0 */
    &sysc_info_A3ISP1,                     /* R_PMA_HWA_ID_V4H_VSPX1 */
    &sysc_info_A3ISP0,                     /* R_PMA_HWA_ID_V4H_VSPX0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RGXC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RGXB */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SWDT1_SWDT0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RGXPVC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_RGXPVDE */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PCIE11 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PCIE01 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_INTAP11 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_FRAY01 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXVI11 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AULK4P */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AULK2P */
    &sysc_info_A3ISP1,                     /* R_PMA_HWA_ID_V4H_FCPVX1 */
    &sysc_info_A3ISP0,                     /* R_PMA_HWA_ID_V4H_FCPVX0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_KCRC4 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_FSO */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CRC3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CRC2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CRC1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CRC0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ADVFS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SCMT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC3D3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC3D2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC3D1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC2D3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC2D2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC2D1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC1D3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC1D2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC1D1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC0D3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC0D2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_PFC0D1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT9_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT8_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT7_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT6_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT5_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT4_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT3_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT2_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT1_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT0_RCLK */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_DBE */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_MTI */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_KCRC7 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_KCRC6 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_KCRC5 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMMM */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMIR */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMVIP */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMVC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMVIO */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMPE0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMRT */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKM3DG */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APVI4_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APVI4_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APVC1_1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_APVC1_0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMDSP_CKMIR */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMCNR_CKMIR */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKM */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMHSC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CKMCR52 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_ANHC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXHC_AXRS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_AXHC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CSBRG_IR_A2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CSBRG_IR_A3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_TSN */
    &sysc_info_A2CN0,                      /* R_PMA_HWA_ID_V4H_IMPSDMAC1 */
    &sysc_info_A2CN0,                      /* R_PMA_HWA_ID_V4H_IMPSDMAC0 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CVE3 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CVE2 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CVE1 */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_CVE0 */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_REG */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_CSB */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_SYSPO */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_APBDBG */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_OCEM */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_CORE */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_SYS */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_GLOBAL */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_REG */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_CSB */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_DSC */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_FCPRC */
    &sysc_info_A3DUL,                      /* R_PMA_HWA_ID_V4H_PAPSDMA */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_HSM */
    &sysc_info_A3DUL,                      /* R_PMA_HWA_ID_V4H_PAPBUS */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_BUS */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_BUS */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_BUS */
    &sysc_info_A1DSP0,                     /* R_PMA_HWA_ID_V4H_VDSP0_BUS */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SSI */
    &hwa_info_NONE,                        /* R_PMA_HWA_ID_V4H_SSIU */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_SYSPO */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_APBDBG */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_OCEM */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_CORE */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_SYS */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_GLOBAL */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_REG */
    &sysc_info_A1DSP3,                     /* R_PMA_HWA_ID_V4H_VDSP3_CSB */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_SYSPO */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_APBDBG */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_OCEM */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_CORE */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_SYS */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_GLOBAL */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_REG */
    &sysc_info_A1DSP2,                     /* R_PMA_HWA_ID_V4H_VDSP2_CSB */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_SYSPO */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_APBDBG */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_OCEM */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_CORE */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_SYS */
    &sysc_info_A1DSP1,                     /* R_PMA_HWA_ID_V4H_VDSP1_GLOBAL */
    &sysc_info_A3IR,                       /* R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR */
    &sysc_info_A3IR,                       /* R_PMA_H  WA_ID_V4H_REG_SPMI  */
    &sysc_info_A2CN0,                      /* R_PMA_H  WA_ID_V4H_REG_SPMC  */
    &cpg_info_DOF,                         /* R_PMA_H  WA_ID_V4H_DOF_FCPRM */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_DCM       */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_CBF       */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_SBF       */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_OSM       */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_ISM0      */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_ISM1      */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_FBS       */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_APB       */
    &sysc_info_A3DUL,                      /* R_PMA_H  WA_ID_V4H_DUL       */
    &sysc_info_A2CN0                       /* R_PMA_H  WA_ID_V4H_SPMC01    */
};

#endif  /* R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_V4H_H */
