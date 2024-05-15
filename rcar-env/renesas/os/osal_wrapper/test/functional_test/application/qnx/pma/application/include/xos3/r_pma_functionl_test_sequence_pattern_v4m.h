#ifndef R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_V4M_H
#define R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_V4M_H

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
    &expected_info_impdta        /* R_PMA_HWA_ID_V4M_IMPDTA */
};

const st_expected_info_t * gp_expected_info_tbl[R_PMA_HWA_ID_V4M_NUM] = 
{
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMPPSC */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMPDMAC0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMP1 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMP0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_SPMC */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMPCNN */
    &expected_info,              /* R_PMA_HWA_ID_V4M_ISP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANRT1 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_DOF */
    &expected_info,              /* R_PMA_HWA_ID_V4M_SMPS0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_SMPO0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_RGX */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANVI1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANVI0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANVC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANVC0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANSP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANPV0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ADVFSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ADG */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SPMI */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_IMPSLV */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_IMPDTA */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXIMP0_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXIMP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANIMP1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANIMP0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMPDMAC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSM_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSD0_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSD0_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRS_AXSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRT_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRD_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRD_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRC_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRC_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRS_AXPV */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXPV */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANMM_AXMM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AVB2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AVB1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AVB0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ARMREG */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ARMM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APSP3_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APSP3_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APSP4_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APSP4_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APRT0_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APRT0_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANVIP2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANVIP1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANVIP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CSITOP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CSDBGPAP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CR0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CANFD */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVIP1_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVIP1_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVIP0_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVIP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVIFF */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVI1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVI1_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVI */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVC_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXTBH */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSTM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSTM_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRS_AXSP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSP0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSN */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXRS_AXSN */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXSM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_DSITXLINK0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_DOC2CH */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_DIS0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_DDR3_DDR2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_DDR1_DDR0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CSITOP1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_INTAP */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMS1 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMS0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMR1 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IMR0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_I2C5 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_I2C4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_I2C3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_I2C2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_I2C1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_I2C0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_HSCIF3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_HSCIF2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_HSCIF1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_HSCIF0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_FRAY00 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_FCPVD1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_FCPVD0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_RTDM1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_RTDM0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_RPC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PWM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PCIE10 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PCIE00 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_MSI5 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_MSI4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_MSI3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_MSI2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_MSI1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_MSI0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_IVCP1E */
    &expected_info,              /* R_PMA_HWA_ID_V4M_ISPCS0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_ISPCS1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_IRQC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_INTTP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_IPC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN01 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN00 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC7 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC6 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC5 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AES_ACC_WRAP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_TPU0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_TMU4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_TMU3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_TMU2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_TMU1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_TMU0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SYDM2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SYDM1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SECROM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SDHI0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SCIF4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SCIF3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SCIF1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SCIF0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VSPD0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN15 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN14 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN13 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN12 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN11 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN10 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN09 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN08 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN07 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN06 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN05 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN04 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN03 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VIN02 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_UCMT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_TSC4_TSC3_TSC2_TSC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CMT3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CMT2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CMT1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CMT0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WDT1_WDT0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WCRC3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WCRC2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WCRC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WCRC0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_VSPX0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_RGXC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_RGXB */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SWDT1_SWDT0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_RGXPVC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_RGXPVDE */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PCIE11 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PCIE01 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_INTAP11 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_FRAY01 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXVI11 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AULK4P */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AULK2P */
    &expected_info,              /* R_PMA_HWA_ID_V4M_FCPVX0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_KCRC4 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_FSO */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CRC3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CRC2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CRC1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CRC0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ADVFS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SCMT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC3D3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC3D2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC3D1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC2D3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC2D2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC2D1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC1D3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC1D2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC1D1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC0D3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC0D2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_PFC0D1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT9_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT8_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT7_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT6_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT5_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT4_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT3_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT2_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT1_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT0_RCLK */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT9_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT8_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT7_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT6_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT5_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT4_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT3_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT2_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT1_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_WWDT0_SASYNCRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_DBE */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_MTI */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_KCRC7 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_KCRC6 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_KCRC5 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMMM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMIR */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMVIP */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMVC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMVIO */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMPE0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMRT */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKM3DG */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APVI4_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APVI4_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APVC1_1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_APVC1_0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMDSP_CKMIR */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMCNR_CKMIR */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMHSC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CKMCR52 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_ANHC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXHC_AXRS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_AXHC */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CSBRG_IR_A2 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_CSBRG_IR_A3 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_TSN */
    &expected_info,              /* R_PMA_HWA_ID_V4M_CVE3 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_CVE2 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_CVE1 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_CVE0 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP1_REG */
    &expected_info,              /* R_PMA_HWA_ID_V4M_VDSP1_CSB */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP0_SYSPO */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP0_APBDBG */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP0_OCEM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP0_CORE */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP0_SYS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP0_GLOBAL */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP0_REG */
    &expected_info,              /* R_PMA_HWA_ID_V4M_VDSP0_CSB */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_FCPRC */
    &expected_info,              /* R_PMA_HWA_ID_V4M_PAPSDMA */
    &expected_info,              /* R_PMA_HWA_ID_V4M_HSM */
    &expected_info,              /* R_PMA_HWA_ID_V4M_PAPBUS */
    &expected_info,              /* R_PMA_HWA_ID_V4M_VDSP1_BUS */
    &expected_info,              /* R_PMA_HWA_ID_V4M_VDSP0_BUS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SSI */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_SSIU */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP1_SYSPO */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP1_APBDBG */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP1_OCEM */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP1_CORE */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP1_SYS */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP1_GLOBAL */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_REG_SPMI */
    &expected_info,              /* R_PMA_HWA_ID_V4M_REG_SPMC */
    &expected_info,              /* R_PMA_HWA_ID_V4M_DOF_FCPRM */
    &expected_info,              /* R_PMA_HWA_ID_V4M_DCM */
    &expected_info,              /* R_PMA_HWA_ID_V4M_CBF */
    &expected_info,              /* R_PMA_HWA_ID_V4M_SBF */
    &expected_info,              /* R_PMA_HWA_ID_V4M_OSM */
    &expected_info,              /* R_PMA_HWA_ID_V4M_ISM0 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_ISM1 */
    &expected_info,              /* R_PMA_HWA_ID_V4M_FBS */
    &expected_info,              /* R_PMA_HWA_ID_V4M_APB */
    &expected_info,              /* R_PMA_HWA_ID_V4M_DUL */
    &expected_info,              /* R_PMA_HWA_ID_V4M_SPMC01 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP1 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV1 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV2 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV3 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_DP0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_DP1 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_CNN0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP1 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_UMFL0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_SMPS0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_SMPO0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_PAP0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_ISP0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMR0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMR1 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMS0 */
    &expected_info_without_cpg,  /* R_PMA_HWA_ID_V4M_FBA_IMS1 */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET */
    &expected_info_without_sysc, /* R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET */
    &expected_info,              /* R_PMA_HWA_ID_V4M_NUM */
};

const uint32_t gp_vdsp_info_flg[R_PMA_HWA_ID_V4M_NUM] = 
{
    0,    /* R_PMA_HWA_ID_V4M_IMPPSC */
    0,    /* R_PMA_HWA_ID_V4M_IMPDMAC0 */
    0,    /* R_PMA_HWA_ID_V4M_IMP1 */
    0,    /* R_PMA_HWA_ID_V4M_IMP0 */
    0,    /* R_PMA_HWA_ID_V4M_SPMC */
    0,    /* R_PMA_HWA_ID_V4M_IMPCNN */
    0,    /* R_PMA_HWA_ID_V4M_ISP0 */
    0,    /* R_PMA_HWA_ID_V4M_ANRT1 */
    0,    /* R_PMA_HWA_ID_V4M_DOF */
    0,    /* R_PMA_HWA_ID_V4M_SMPS0 */
    0,    /* R_PMA_HWA_ID_V4M_SMPO0 */
    0,    /* R_PMA_HWA_ID_V4M_RGX */
    0,    /* R_PMA_HWA_ID_V4M_ANVI1 */
    0,    /* R_PMA_HWA_ID_V4M_ANVI0 */
    0,    /* R_PMA_HWA_ID_V4M_ANVC1 */
    0,    /* R_PMA_HWA_ID_V4M_ANVC0 */
    0,    /* R_PMA_HWA_ID_V4M_ANSP0 */
    0,    /* R_PMA_HWA_ID_V4M_ANRT */
    0,    /* R_PMA_HWA_ID_V4M_ANPV0 */
    0,    /* R_PMA_HWA_ID_V4M_ADVFSC */
    0,    /* R_PMA_HWA_ID_V4M_ADG */
    0,    /* R_PMA_HWA_ID_V4M_SPMI */
    0,    /* R_PMA_HWA_ID_V4M_IMPSLV */
    0,    /* R_PMA_HWA_ID_V4M_IMPDTA */
    0,    /* R_PMA_HWA_ID_V4M_AXIMP0_AXRS */
    0,    /* R_PMA_HWA_ID_V4M_AXIMP0 */
    0,    /* R_PMA_HWA_ID_V4M_ANIMP1 */
    0,    /* R_PMA_HWA_ID_V4M_ANIMP0 */
    0,    /* R_PMA_HWA_ID_V4M_IMPDMAC1 */
    0,    /* R_PMA_HWA_ID_V4M_AXSM_AXRS */
    0,    /* R_PMA_HWA_ID_V4M_AXSD0_1 */
    0,    /* R_PMA_HWA_ID_V4M_AXSD0_0 */
    0,    /* R_PMA_HWA_ID_V4M_AXSC */
    0,    /* R_PMA_HWA_ID_V4M_AXRS_AXSC */
    0,    /* R_PMA_HWA_ID_V4M_AXRT_AXRS */
    0,    /* R_PMA_HWA_ID_V4M_AXRT */
    0,    /* R_PMA_HWA_ID_V4M_AXRD_1 */
    0,    /* R_PMA_HWA_ID_V4M_AXRD_0 */
    0,    /* R_PMA_HWA_ID_V4M_AXRC_1 */
    0,    /* R_PMA_HWA_ID_V4M_AXRC_0 */
    0,    /* R_PMA_HWA_ID_V4M_AXRS_AXPV */
    0,    /* R_PMA_HWA_ID_V4M_AXPV */
    0,    /* R_PMA_HWA_ID_V4M_ANMM_AXMM */
    0,    /* R_PMA_HWA_ID_V4M_AVB2 */
    0,    /* R_PMA_HWA_ID_V4M_AVB1 */
    0,    /* R_PMA_HWA_ID_V4M_AVB0 */
    0,    /* R_PMA_HWA_ID_V4M_ARMREG */
    0,    /* R_PMA_HWA_ID_V4M_ARMM */
    0,    /* R_PMA_HWA_ID_V4M_APSP3_1 */
    0,    /* R_PMA_HWA_ID_V4M_APSP3_0 */
    0,    /* R_PMA_HWA_ID_V4M_APSP4_1 */
    0,    /* R_PMA_HWA_ID_V4M_APSP4_0 */
    0,    /* R_PMA_HWA_ID_V4M_APRT0_1 */
    0,    /* R_PMA_HWA_ID_V4M_APRT0_0 */
    0,    /* R_PMA_HWA_ID_V4M_ANVIP2 */
    0,    /* R_PMA_HWA_ID_V4M_ANVIP1 */
    0,    /* R_PMA_HWA_ID_V4M_ANVIP0 */
    0,    /* R_PMA_HWA_ID_V4M_CSITOP0 */
    0,    /* R_PMA_HWA_ID_V4M_CSDBGPAP */
    0,    /* R_PMA_HWA_ID_V4M_CR0 */
    0,    /* R_PMA_HWA_ID_V4M_CANFD */
    0,    /* R_PMA_HWA_ID_V4M_AXVIP1_1 */
    0,    /* R_PMA_HWA_ID_V4M_AXVIP1_0 */
    0,    /* R_PMA_HWA_ID_V4M_AXVIP0_AXRS */
    0,    /* R_PMA_HWA_ID_V4M_AXVIP0 */
    0,    /* R_PMA_HWA_ID_V4M_AXVIFF */
    0,    /* R_PMA_HWA_ID_V4M_AXVI1 */
    0,    /* R_PMA_HWA_ID_V4M_AXVI1_AXRS */
    0,    /* R_PMA_HWA_ID_V4M_AXVI */
    0,    /* R_PMA_HWA_ID_V4M_AXVC_AXRS */
    0,    /* R_PMA_HWA_ID_V4M_AXVC */
    0,    /* R_PMA_HWA_ID_V4M_AXTBH */
    0,    /* R_PMA_HWA_ID_V4M_AXSTM */
    0,    /* R_PMA_HWA_ID_V4M_AXSTM_AXRS */
    0,    /* R_PMA_HWA_ID_V4M_AXRS_AXSP0 */
    0,    /* R_PMA_HWA_ID_V4M_AXSP0 */
    0,    /* R_PMA_HWA_ID_V4M_AXSN */
    0,    /* R_PMA_HWA_ID_V4M_AXRS_AXSN */
    0,    /* R_PMA_HWA_ID_V4M_AXSM */
    0,    /* R_PMA_HWA_ID_V4M_DSITXLINK0 */
    0,    /* R_PMA_HWA_ID_V4M_DOC2CH */
    0,    /* R_PMA_HWA_ID_V4M_DIS0 */
    0,    /* R_PMA_HWA_ID_V4M_DDR3_DDR2 */
    0,    /* R_PMA_HWA_ID_V4M_DDR1_DDR0 */
    0,    /* R_PMA_HWA_ID_V4M_CSITOP1 */
    0,    /* R_PMA_HWA_ID_V4M_INTAP */
    0,    /* R_PMA_HWA_ID_V4M_IMS1 */
    0,    /* R_PMA_HWA_ID_V4M_IMS0 */
    0,    /* R_PMA_HWA_ID_V4M_IMR1 */
    0,    /* R_PMA_HWA_ID_V4M_IMR0 */
    0,    /* R_PMA_HWA_ID_V4M_I2C5 */
    0,    /* R_PMA_HWA_ID_V4M_I2C4 */
    0,    /* R_PMA_HWA_ID_V4M_I2C3 */
    0,    /* R_PMA_HWA_ID_V4M_I2C2 */
    0,    /* R_PMA_HWA_ID_V4M_I2C1 */
    0,    /* R_PMA_HWA_ID_V4M_I2C0 */
    0,    /* R_PMA_HWA_ID_V4M_HSCIF3 */
    0,    /* R_PMA_HWA_ID_V4M_HSCIF2 */
    0,    /* R_PMA_HWA_ID_V4M_HSCIF1 */
    0,    /* R_PMA_HWA_ID_V4M_HSCIF0 */
    0,    /* R_PMA_HWA_ID_V4M_FRAY00 */
    0,    /* R_PMA_HWA_ID_V4M_FCPVD1 */
    0,    /* R_PMA_HWA_ID_V4M_FCPVD0 */
    0,    /* R_PMA_HWA_ID_V4M_RTDM1 */
    0,    /* R_PMA_HWA_ID_V4M_RTDM0 */
    0,    /* R_PMA_HWA_ID_V4M_RPC */
    0,    /* R_PMA_HWA_ID_V4M_PWM */
    0,    /* R_PMA_HWA_ID_V4M_PCIE10 */
    0,    /* R_PMA_HWA_ID_V4M_PCIE00 */
    0,    /* R_PMA_HWA_ID_V4M_MSI5 */
    0,    /* R_PMA_HWA_ID_V4M_MSI4 */
    0,    /* R_PMA_HWA_ID_V4M_MSI3 */
    0,    /* R_PMA_HWA_ID_V4M_MSI2 */
    0,    /* R_PMA_HWA_ID_V4M_MSI1 */
    0,    /* R_PMA_HWA_ID_V4M_MSI0 */
    0,    /* R_PMA_HWA_ID_V4M_IVCP1E */
    0,    /* R_PMA_HWA_ID_V4M_ISPCS0 */
    0,    /* R_PMA_HWA_ID_V4M_ISPCS1 */
    0,    /* R_PMA_HWA_ID_V4M_IRQC */
    0,    /* R_PMA_HWA_ID_V4M_INTTP */
    0,    /* R_PMA_HWA_ID_V4M_IPC */
    0,    /* R_PMA_HWA_ID_V4M_VIN01 */
    0,    /* R_PMA_HWA_ID_V4M_VIN00 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC7 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC6 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC5 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC4 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC3 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC2 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC1 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC0 */
    0,    /* R_PMA_HWA_ID_V4M_AES_ACC_WRAP */
    0,    /* R_PMA_HWA_ID_V4M_TPU0 */
    0,    /* R_PMA_HWA_ID_V4M_TMU4 */
    0,    /* R_PMA_HWA_ID_V4M_TMU3 */
    0,    /* R_PMA_HWA_ID_V4M_TMU2 */
    0,    /* R_PMA_HWA_ID_V4M_TMU1 */
    0,    /* R_PMA_HWA_ID_V4M_TMU0 */
    0,    /* R_PMA_HWA_ID_V4M_SYDM2 */
    0,    /* R_PMA_HWA_ID_V4M_SYDM1 */
    0,    /* R_PMA_HWA_ID_V4M_SECROM */
    0,    /* R_PMA_HWA_ID_V4M_SDHI0 */
    0,    /* R_PMA_HWA_ID_V4M_SCIF4 */
    0,    /* R_PMA_HWA_ID_V4M_SCIF3 */
    0,    /* R_PMA_HWA_ID_V4M_SCIF1 */
    0,    /* R_PMA_HWA_ID_V4M_SCIF0 */
    0,    /* R_PMA_HWA_ID_V4M_VSPD0 */
    0,    /* R_PMA_HWA_ID_V4M_VIN15 */
    0,    /* R_PMA_HWA_ID_V4M_VIN14 */
    0,    /* R_PMA_HWA_ID_V4M_VIN13 */
    0,    /* R_PMA_HWA_ID_V4M_VIN12 */
    0,    /* R_PMA_HWA_ID_V4M_VIN11 */
    0,    /* R_PMA_HWA_ID_V4M_VIN10 */
    0,    /* R_PMA_HWA_ID_V4M_VIN09 */
    0,    /* R_PMA_HWA_ID_V4M_VIN08 */
    0,    /* R_PMA_HWA_ID_V4M_VIN07 */
    0,    /* R_PMA_HWA_ID_V4M_VIN06 */
    0,    /* R_PMA_HWA_ID_V4M_VIN05 */
    0,    /* R_PMA_HWA_ID_V4M_VIN04 */
    0,    /* R_PMA_HWA_ID_V4M_VIN03 */
    0,    /* R_PMA_HWA_ID_V4M_VIN02 */
    0,    /* R_PMA_HWA_ID_V4M_UCMT */
    0,    /* R_PMA_HWA_ID_V4M_TSC4_TSC3_TSC2_TSC1 */
    0,    /* R_PMA_HWA_ID_V4M_PFC3 */
    0,    /* R_PMA_HWA_ID_V4M_PFC2 */
    0,    /* R_PMA_HWA_ID_V4M_PFC1 */
    0,    /* R_PMA_HWA_ID_V4M_PFC0 */
    0,    /* R_PMA_HWA_ID_V4M_CMT3 */
    0,    /* R_PMA_HWA_ID_V4M_CMT2 */
    0,    /* R_PMA_HWA_ID_V4M_CMT1 */
    0,    /* R_PMA_HWA_ID_V4M_CMT0 */
    0,    /* R_PMA_HWA_ID_V4M_WDT1_WDT0 */
    0,    /* R_PMA_HWA_ID_V4M_WCRC3 */
    0,    /* R_PMA_HWA_ID_V4M_WCRC2 */
    0,    /* R_PMA_HWA_ID_V4M_WCRC1 */
    0,    /* R_PMA_HWA_ID_V4M_WCRC0 */
    0,    /* R_PMA_HWA_ID_V4M_VSPX0 */
    0,    /* R_PMA_HWA_ID_V4M_RGXC */
    0,    /* R_PMA_HWA_ID_V4M_RGXB */
    0,    /* R_PMA_HWA_ID_V4M_SWDT1_SWDT0 */
    0,    /* R_PMA_HWA_ID_V4M_RGXPVC */
    0,    /* R_PMA_HWA_ID_V4M_RGXPVDE */
    0,    /* R_PMA_HWA_ID_V4M_PCIE11 */
    0,    /* R_PMA_HWA_ID_V4M_PCIE01 */
    0,    /* R_PMA_HWA_ID_V4M_INTAP11 */
    0,    /* R_PMA_HWA_ID_V4M_FRAY01 */
    0,    /* R_PMA_HWA_ID_V4M_AXVI11 */
    0,    /* R_PMA_HWA_ID_V4M_AULK4P */
    0,    /* R_PMA_HWA_ID_V4M_AULK2P */
    0,    /* R_PMA_HWA_ID_V4M_FCPVX0 */
    0,    /* R_PMA_HWA_ID_V4M_KCRC4 */
    0,    /* R_PMA_HWA_ID_V4M_FSO */
    0,    /* R_PMA_HWA_ID_V4M_CRC3 */
    0,    /* R_PMA_HWA_ID_V4M_CRC2 */
    0,    /* R_PMA_HWA_ID_V4M_CRC1 */
    0,    /* R_PMA_HWA_ID_V4M_CRC0 */
    0,    /* R_PMA_HWA_ID_V4M_ADVFS */
    0,    /* R_PMA_HWA_ID_V4M_SCMT */
    0,    /* R_PMA_HWA_ID_V4M_PFC3D3 */
    0,    /* R_PMA_HWA_ID_V4M_PFC3D2 */
    0,    /* R_PMA_HWA_ID_V4M_PFC3D1 */
    0,    /* R_PMA_HWA_ID_V4M_PFC2D3 */
    0,    /* R_PMA_HWA_ID_V4M_PFC2D2 */
    0,    /* R_PMA_HWA_ID_V4M_PFC2D1 */
    0,    /* R_PMA_HWA_ID_V4M_PFC1D3 */
    0,    /* R_PMA_HWA_ID_V4M_PFC1D2 */
    0,    /* R_PMA_HWA_ID_V4M_PFC1D1 */
    0,    /* R_PMA_HWA_ID_V4M_PFC0D3 */
    0,    /* R_PMA_HWA_ID_V4M_PFC0D2 */
    0,    /* R_PMA_HWA_ID_V4M_PFC0D1 */
    0,    /* R_PMA_HWA_ID_V4M_WWDT9_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT8_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT7_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT6_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT5_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT4_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT3_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT2_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT1_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT0_RCLK */
    0,    /* R_PMA_HWA_ID_V4M_WWDT9_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT8_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT7_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT6_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT5_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT4_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT3_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT2_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT1_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_WWDT0_SASYNCRT */
    0,    /* R_PMA_HWA_ID_V4M_DBE */
    0,    /* R_PMA_HWA_ID_V4M_MTI */
    0,    /* R_PMA_HWA_ID_V4M_KCRC7 */
    0,    /* R_PMA_HWA_ID_V4M_KCRC6 */
    0,    /* R_PMA_HWA_ID_V4M_KCRC5 */
    0,    /* R_PMA_HWA_ID_V4M_CKMMM */
    0,    /* R_PMA_HWA_ID_V4M_CKMIR */
    0,    /* R_PMA_HWA_ID_V4M_CKMVIP */
    0,    /* R_PMA_HWA_ID_V4M_CKMVC */
    0,    /* R_PMA_HWA_ID_V4M_CKMVIO */
    0,    /* R_PMA_HWA_ID_V4M_CKMPE0 */
    0,    /* R_PMA_HWA_ID_V4M_CKMRT */
    0,    /* R_PMA_HWA_ID_V4M_CKM3DG */
    0,    /* R_PMA_HWA_ID_V4M_APVI4_1 */
    0,    /* R_PMA_HWA_ID_V4M_APVI4_0 */
    0,    /* R_PMA_HWA_ID_V4M_APVC1_1 */
    0,    /* R_PMA_HWA_ID_V4M_APVC1_0 */
    0,    /* R_PMA_HWA_ID_V4M_CKMDSP_CKMIR */
    0,    /* R_PMA_HWA_ID_V4M_CKMCNR_CKMIR */
    0,    /* R_PMA_HWA_ID_V4M_CKM */
    0,    /* R_PMA_HWA_ID_V4M_CKMHSC */
    0,    /* R_PMA_HWA_ID_V4M_CKMCR52 */
    0,    /* R_PMA_HWA_ID_V4M_ANHC */
    0,    /* R_PMA_HWA_ID_V4M_AXHC_AXRS */
    0,    /* R_PMA_HWA_ID_V4M_AXHC */
    0,    /* R_PMA_HWA_ID_V4M_CSBRG_IR_A2 */
    0,    /* R_PMA_HWA_ID_V4M_CSBRG_IR_A3 */
    0,    /* R_PMA_HWA_ID_V4M_TSN */
    0,    /* R_PMA_HWA_ID_V4M_CVE3 */
    0,    /* R_PMA_HWA_ID_V4M_CVE2 */
    0,    /* R_PMA_HWA_ID_V4M_CVE1 */
    0,    /* R_PMA_HWA_ID_V4M_CVE0 */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_REG */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_CSB */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_SYSPO */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_APBDBG */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_OCEM */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_CORE */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_SYS */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_GLOBAL */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_REG */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_CSB */
    0,    /* R_PMA_HWA_ID_V4M_FCPRC */
    0,    /* R_PMA_HWA_ID_V4M_PAPSDMA */
    0,    /* R_PMA_HWA_ID_V4M_HSM */
    0,    /* R_PMA_HWA_ID_V4M_PAPBUS */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_BUS */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_BUS */
    0,    /* R_PMA_HWA_ID_V4M_SSI */
    0,    /* R_PMA_HWA_ID_V4M_SSIU */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_SYSPO */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_APBDBG */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_OCEM */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_CORE */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_SYS */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_GLOBAL */
    0,    /* R_PMA_HWA_ID_V4M_REG_SPMI */
    0,    /* R_PMA_HWA_ID_V4M_REG_SPMC */
    0,    /* R_PMA_HWA_ID_V4M_DOF_FCPRM */
    0,    /* R_PMA_HWA_ID_V4M_DCM */
    0,    /* R_PMA_HWA_ID_V4M_CBF */
    0,    /* R_PMA_HWA_ID_V4M_SBF */
    0,    /* R_PMA_HWA_ID_V4M_OSM */
    0,    /* R_PMA_HWA_ID_V4M_ISM0 */
    0,    /* R_PMA_HWA_ID_V4M_ISM1 */
    0,    /* R_PMA_HWA_ID_V4M_FBS */
    0,    /* R_PMA_HWA_ID_V4M_APB */
    0,    /* R_PMA_HWA_ID_V4M_DUL */
    0,    /* R_PMA_HWA_ID_V4M_SPMC01 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP1 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV1 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV2 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV3 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DP0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DP1 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_CNN0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP1 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_UMFL0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_SMPS0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_SMPO0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_PAP0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_ISP0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMR0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMR1 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMS0 */
    0,    /* R_PMA_HWA_ID_V4M_FBA_IMS1 */
    1,    /* R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET */
    1,    /* R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET */
    0,    /* R_PMA_HWA_ID_V4M_NUM */
};

st_share_domain_info_t hwa_info_NONE =
{
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t cpg_info_DOF =
{
    R_PMA_HWA_ID_V4M_DOF, R_PMA_HWA_ID_V4M_DOF_FCPRM, R_PMA_HWA_ID_V4M_FBA_UMFL0,R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t cpg_info_SPMC =
{
    R_PMA_HWA_ID_V4M_SPMC, R_PMA_HWA_ID_V4M_REG_SPMC, R_PMA_HWA_ID_V4M_SPMC01, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t cpg_info_PAPTOP =
{
    R_PMA_HWA_ID_V4M_HSM, R_PMA_HWA_ID_V4M_DCM, R_PMA_HWA_ID_V4M_CBF, R_PMA_HWA_ID_V4M_SBF, R_PMA_HWA_ID_V4M_OSM, 
    R_PMA_HWA_ID_V4M_ISM0, R_PMA_HWA_ID_V4M_ISM1, R_PMA_HWA_ID_V4M_FBS, R_PMA_HWA_ID_V4M_APB, R_PMA_HWA_ID_V4M_DUL,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t cpg_info_SPMI =
{
    R_PMA_HWA_ID_V4M_SPMI, R_PMA_HWA_ID_V4M_REG_SPMI, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A2CN0 =
{
    R_PMA_HWA_ID_V4M_SPMC, R_PMA_HWA_ID_V4M_REG_SPMC, R_PMA_HWA_ID_V4M_SPMC01, R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A3DUL =
{
    R_PMA_HWA_ID_V4M_PAPSDMA, R_PMA_HWA_ID_V4M_HSM, R_PMA_HWA_ID_V4M_PAPBUS, R_PMA_HWA_ID_V4M_DCM, R_PMA_HWA_ID_V4M_CBF,
    R_PMA_HWA_ID_V4M_SBF,R_PMA_HWA_ID_V4M_OSM, R_PMA_HWA_ID_V4M_ISM0, R_PMA_HWA_ID_V4M_ISM1, R_PMA_HWA_ID_V4M_FBS,
    R_PMA_HWA_ID_V4M_APB,R_PMA_HWA_ID_V4M_DUL, R_PMA_HWA_ID_V4M_FBA_PAP0, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A2PSC =
{
    R_PMA_HWA_ID_V4M_IMPPSC, R_PMA_HWA_ID_V4M_IMPDMAC0, R_PMA_HWA_ID_V4M_FBA_IMP_DP0, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A2DMA =
{
    R_PMA_HWA_ID_V4M_IMPDMAC1, R_PMA_HWA_ID_V4M_FBA_IMP_DP1, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A2IMP01 =
{
    R_PMA_HWA_ID_V4M_IMP1, R_PMA_HWA_ID_V4M_IMP0, R_PMA_HWA_ID_V4M_FBA_IMP0, R_PMA_HWA_ID_V4M_FBA_IMP1, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A3ISP0 =
{
    R_PMA_HWA_ID_V4M_ISP0, R_PMA_HWA_ID_V4M_ISPCS0, R_PMA_HWA_ID_V4M_ISPCS1, R_PMA_HWA_ID_V4M_VSPX0, R_PMA_HWA_ID_V4M_FCPVX0,
    R_PMA_HWA_ID_V4M_FBA_ISP0, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A3VIP2 =
{
    R_PMA_HWA_ID_V4M_SMPS0, R_PMA_HWA_ID_V4M_SMPO0, R_PMA_HWA_ID_V4M_FBA_SMPS0, R_PMA_HWA_ID_V4M_FBA_SMPO0, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};
st_share_domain_info_t sysc_info_A3VIP0 =
{
    R_PMA_HWA_ID_V4M_DOF, R_PMA_HWA_ID_V4M_DOF_FCPRM, R_PMA_HWA_ID_V4M_FBA_UMFL0,R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A1DSP1 =
{
    R_PMA_HWA_ID_V4M_VDSP1_CSB, R_PMA_HWA_ID_V4M_VDSP1_BUS, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A1DSP0 =
{
    R_PMA_HWA_ID_V4M_VDSP0_CSB, R_PMA_HWA_ID_V4M_VDSP0_BUS, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A1CN0 =
{
    R_PMA_HWA_ID_V4M_IMPCNN, R_PMA_HWA_ID_V4M_FBA_IMP_CNN0, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A2CV0 =
{
    R_PMA_HWA_ID_V4M_CVE0, R_PMA_HWA_ID_V4M_FBA_IMP_OCV0, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A2CV1 =
{
    R_PMA_HWA_ID_V4M_CVE1, R_PMA_HWA_ID_V4M_FBA_IMP_OCV1, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A2CV2 =
{
    R_PMA_HWA_ID_V4M_CVE2, R_PMA_HWA_ID_V4M_FBA_IMP_OCV2, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A2CV3 =
{
    R_PMA_HWA_ID_V4M_CVE3, R_PMA_HWA_ID_V4M_FBA_IMP_OCV3, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A3IMR0 =
{
    R_PMA_HWA_ID_V4M_IMR0, R_PMA_HWA_ID_V4M_FBA_IMR0, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A3IMR1 =
{
    R_PMA_HWA_ID_V4M_IMR1, R_PMA_HWA_ID_V4M_FBA_IMR1, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A3IMR2 =
{
    R_PMA_HWA_ID_V4M_IMS0, R_PMA_HWA_ID_V4M_FBA_IMS0, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

st_share_domain_info_t sysc_info_A3IMR3 =
{
    R_PMA_HWA_ID_V4M_IMS1, R_PMA_HWA_ID_V4M_FBA_IMS1, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM,
    R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM , R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM, R_PMA_HWA_ID_V4M_NUM
};

const st_share_domain_info_t * gp_share_cpg_info[R_PMA_HWA_ID_V4M_NUM] = 
{
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMPPSC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMPDMAC0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMP1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMP0 */
    &cpg_info_SPMC,    /* R_PMA_HWA_ID_V4M_SPMC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMPCNN */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ISP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANRT1 */
    &cpg_info_DOF,     /* R_PMA_HWA_ID_V4M_DOF */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SMPS0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SMPO0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_RGX */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANVI1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANVI0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANVC1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANVC0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANSP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANPV0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ADVFSC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ADG */
    &cpg_info_SPMI,    /* R_PMA_HWA_ID_V4M_SPMI */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMPSLV */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMPDTA */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXIMP0_AXRS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXIMP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANIMP1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANIMP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMPDMAC1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSM_AXRS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSD0_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSD0_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRS_AXSC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRT_AXRS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRD_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRD_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRC_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRC_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRS_AXPV */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXPV */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANMM_AXMM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AVB2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AVB1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AVB0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ARMREG */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ARMM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APSP3_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APSP3_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APSP4_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APSP4_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APRT0_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APRT0_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANVIP2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANVIP1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANVIP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CSITOP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CSDBGPAP */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CR0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CANFD */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVIP1_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVIP1_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVIP0_AXRS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVIP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVIFF */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVI1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVI1_AXRS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVI */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVC_AXRS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXTBH */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSTM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSTM_AXRS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRS_AXSP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSN */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXRS_AXSN */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXSM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_DSITXLINK0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_DOC2CH */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_DIS0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_DDR3_DDR2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_DDR1_DDR0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CSITOP1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_INTAP */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMS1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMS0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMR1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IMR0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_I2C5 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_I2C4 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_I2C3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_I2C2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_I2C1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_I2C0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_HSCIF3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_HSCIF2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_HSCIF1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_HSCIF0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FRAY00 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FCPVD1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FCPVD0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_RTDM1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_RTDM0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_RPC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PWM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PCIE10 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PCIE00 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_MSI5 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_MSI4 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_MSI3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_MSI2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_MSI1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_MSI0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IVCP1E */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ISPCS0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ISPCS1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IRQC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_INTTP */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_IPC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN01 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN00 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC7 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC6 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC5 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC4 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AES_ACC_WRAP */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_TPU0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_TMU4 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_TMU3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_TMU2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_TMU1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_TMU0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SYDM2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SYDM1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SECROM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SDHI0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SCIF4 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SCIF3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SCIF1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SCIF0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VSPD0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN15 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN14 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN13 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN12 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN11 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN10 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN09 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN08 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN07 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN06 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN05 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN04 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN03 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VIN02 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_UCMT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_TSC4_TSC3_TSC2_TSC1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CMT3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CMT2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CMT1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CMT0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WDT1_WDT0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WCRC3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WCRC2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WCRC1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WCRC0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VSPX0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_RGXC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_RGXB */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SWDT1_SWDT0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_RGXPVC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_RGXPVDE */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PCIE11 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PCIE01 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_INTAP11 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FRAY01 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXVI11 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AULK4P */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AULK2P */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FCPVX0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_KCRC4 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FSO */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CRC3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CRC2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CRC1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CRC0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ADVFS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SCMT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC3D3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC3D2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC3D1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC2D3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC2D2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC2D1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC1D3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC1D2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC1D1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC0D3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC0D2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PFC0D1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT9_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT8_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT7_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT6_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT5_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT4_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT3_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT2_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT1_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT0_RCLK */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT9_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT8_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT7_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT6_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT5_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT4_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT3_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT2_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT1_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_WWDT0_SASYNCRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_DBE */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_MTI */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_KCRC7 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_KCRC6 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_KCRC5 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMMM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMIR */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMVIP */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMVC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMVIO */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMPE0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMRT */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKM3DG */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APVI4_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APVI4_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APVC1_1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_APVC1_0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMDSP_CKMIR */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMCNR_CKMIR */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMHSC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CKMCR52 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_ANHC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXHC_AXRS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_AXHC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CSBRG_IR_A2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CSBRG_IR_A3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_TSN */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CVE3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CVE2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CVE1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_CVE0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_REG */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_CSB */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_SYSPO */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_APBDBG */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_OCEM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_CORE */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_SYS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_GLOBAL */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_REG */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_CSB */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FCPRC */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PAPSDMA */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_HSM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_PAPBUS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_BUS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_BUS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SSI */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_SSIU */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_SYSPO */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_APBDBG */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_OCEM */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_CORE */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_SYS */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_GLOBAL */
    &cpg_info_SPMI,    /* R_PMA_HWA_ID_V4M_REG_SPMI */
    &cpg_info_SPMC,    /* R_PMA_HWA_ID_V4M_REG_SPMC */
    &cpg_info_DOF,     /* R_PMA_HWA_ID_V4M_DOF_FCPRM */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_DCM */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_CBF */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_SBF */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_OSM */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_ISM0 */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_ISM1 */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_FBS */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_APB */
    &cpg_info_PAPTOP,  /* R_PMA_HWA_ID_V4M_DUL */
    &cpg_info_SPMC,    /* R_PMA_HWA_ID_V4M_SPMC01 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV2 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV3 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DP1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_CNN0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_UMFL0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_SMPS0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_SMPO0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_PAP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_ISP0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMR0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMR1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMS0 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_FBA_IMS1 */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET */
    &hwa_info_NONE,    /* R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET */
};

const st_share_domain_info_t * gp_share_sysc_info[R_PMA_HWA_ID_V4M_NUM] = 
{
    &sysc_info_A2PSC,    /* R_PMA_HWA_ID_V4M_IMPPSC */
    &sysc_info_A2PSC,    /* R_PMA_HWA_ID_V4M_IMPDMAC0 */
    &sysc_info_A2IMP01,  /* R_PMA_HWA_ID_V4M_IMP1 */
    &sysc_info_A2IMP01,  /* R_PMA_HWA_ID_V4M_IMP0 */
    &sysc_info_A2CN0,    /* R_PMA_HWA_ID_V4M_SPMC */
    &sysc_info_A1CN0,    /* R_PMA_HWA_ID_V4M_IMPCNN */
    &sysc_info_A3ISP0,   /* R_PMA_HWA_ID_V4M_ISP0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANRT1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_DOF */
    &sysc_info_A3VIP2,   /* R_PMA_HWA_ID_V4M_SMPS0 */
    &sysc_info_A3VIP2,   /* R_PMA_HWA_ID_V4M_SMPO0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_RGX */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANVI1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANVI0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANVC1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANVC0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANSP0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANPV0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ADVFSC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ADG */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SPMI */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_IMPSLV */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_IMPDTA */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXIMP0_AXRS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXIMP0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANIMP1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANIMP0 */
    &sysc_info_A2DMA,    /* R_PMA_HWA_ID_V4M_IMPDMAC1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSM_AXRS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSD0_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSD0_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRS_AXSC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRT_AXRS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRD_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRD_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRC_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRC_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRS_AXPV */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXPV */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANMM_AXMM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AVB2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AVB1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AVB0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ARMREG */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ARMM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APSP3_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APSP3_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APSP4_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APSP4_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APRT0_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APRT0_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANVIP2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANVIP1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANVIP0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CSITOP0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CSDBGPAP */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CR0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CANFD */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVIP1_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVIP1_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVIP0_AXRS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVIP0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVIFF */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVI1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVI1_AXRS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVI */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVC_AXRS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXTBH */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSTM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSTM_AXRS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRS_AXSP0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSP0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSN */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXRS_AXSN */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXSM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_DSITXLINK0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_DOC2CH */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_DIS0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_DDR3_DDR2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_DDR1_DDR0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CSITOP1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_INTAP */
    &sysc_info_A3IMR3,   /* R_PMA_HWA_ID_V4M_IMS1 */
    &sysc_info_A3IMR2,   /* R_PMA_HWA_ID_V4M_IMS0 */
    &sysc_info_A3IMR1,   /* R_PMA_HWA_ID_V4M_IMR1 */
    &sysc_info_A3IMR0,   /* R_PMA_HWA_ID_V4M_IMR0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_I2C5 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_I2C4 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_I2C3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_I2C2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_I2C1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_I2C0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_HSCIF3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_HSCIF2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_HSCIF1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_HSCIF0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_FRAY00 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_FCPVD1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_FCPVD0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_RTDM1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_RTDM0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_RPC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PWM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PCIE10 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PCIE00 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_MSI5 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_MSI4 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_MSI3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_MSI2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_MSI1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_MSI0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_IVCP1E */
    &sysc_info_A3ISP0,   /* R_PMA_HWA_ID_V4M_ISPCS0 */
    &sysc_info_A3ISP0,   /* R_PMA_HWA_ID_V4M_ISPCS1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_IRQC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_INTTP */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_IPC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN01 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN00 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC7 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC6 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC5 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC4 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AES_ACC_WRAP */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_TPU0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_TMU4 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_TMU3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_TMU2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_TMU1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_TMU0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SYDM2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SYDM1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SECROM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SDHI0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SCIF4 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SCIF3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SCIF1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SCIF0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VSPD0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN15 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN14 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN13 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN12 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN11 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN10 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN09 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN08 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN07 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN06 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN05 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN04 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN03 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VIN02 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_UCMT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_TSC4_TSC3_TSC2_TSC1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CMT3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CMT2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CMT1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CMT0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WDT1_WDT0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WCRC3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WCRC2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WCRC1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WCRC0 */
    &sysc_info_A3ISP0,   /* R_PMA_HWA_ID_V4M_VSPX0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_RGXC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_RGXB */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SWDT1_SWDT0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_RGXPVC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_RGXPVDE */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PCIE11 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PCIE01 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_INTAP11 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_FRAY01 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXVI11 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AULK4P */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AULK2P */
    &sysc_info_A3ISP0,   /* R_PMA_HWA_ID_V4M_FCPVX0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_KCRC4 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_FSO */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CRC3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CRC2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CRC1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CRC0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ADVFS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SCMT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC3D3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC3D2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC3D1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC2D3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC2D2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC2D1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC1D3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC1D2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC1D1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC0D3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC0D2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_PFC0D1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT9_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT8_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT7_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT6_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT5_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT4_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT3_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT2_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT1_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT0_RCLK */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT9_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT8_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT7_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT6_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT5_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT4_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT3_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT2_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT1_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_WWDT0_SASYNCRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_DBE */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_MTI */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_KCRC7 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_KCRC6 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_KCRC5 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMMM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMIR */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMVIP */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMVC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMVIO */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMPE0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMRT */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKM3DG */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APVI4_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APVI4_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APVC1_1 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_APVC1_0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMDSP_CKMIR */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMCNR_CKMIR */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMHSC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CKMCR52 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_ANHC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXHC_AXRS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_AXHC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CSBRG_IR_A2 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_CSBRG_IR_A3 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_TSN */
    &sysc_info_A2CV3,    /* R_PMA_HWA_ID_V4M_CVE3 */
    &sysc_info_A2CV2,    /* R_PMA_HWA_ID_V4M_CVE2 */
    &sysc_info_A2CV1,    /* R_PMA_HWA_ID_V4M_CVE1 */
    &sysc_info_A2CV0,    /* R_PMA_HWA_ID_V4M_CVE0 */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP1_REG */
    &sysc_info_A1DSP1,   /* R_PMA_HWA_ID_V4M_VDSP1_CSB */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP0_SYSPO */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP0_APBDBG */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP0_OCEM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP0_CORE */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP0_SYS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP0_GLOBAL */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP0_REG */
    &sysc_info_A1DSP0,   /* R_PMA_HWA_ID_V4M_VDSP0_CSB */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_FCPRC */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_PAPSDMA */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_HSM */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_PAPBUS */
    &sysc_info_A1DSP1,   /* R_PMA_HWA_ID_V4M_VDSP1_BUS */
    &sysc_info_A1DSP0,   /* R_PMA_HWA_ID_V4M_VDSP0_BUS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SSI */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_SSIU */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP1_SYSPO */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP1_APBDBG */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP1_OCEM */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP1_CORE */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP1_SYS */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_VDSP1_GLOBAL */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_REG_SPMI */
    &sysc_info_A2CN0,    /* R_PMA_HWA_ID_V4M_REG_SPMC */
    &hwa_info_NONE,     /* R_PMA_HWA_ID_V4M_DOF_FCPRM */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_DCM */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_CBF */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_SBF */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_OSM */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_ISM0 */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_ISM1 */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_FBS */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_APB */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_DUL */
    &sysc_info_A2CN0,    /* R_PMA_HWA_ID_V4M_SPMC01 */
    &sysc_info_A2IMP01,  /* R_PMA_HWA_ID_V4M_FBA_IMP0 */
    &sysc_info_A2IMP01,  /* R_PMA_HWA_ID_V4M_FBA_IMP1 */
    &sysc_info_A2CV0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV0 */
    &sysc_info_A2CV1,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV1 */
    &sysc_info_A2CV2,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV2 */
    &sysc_info_A2CV3,    /* R_PMA_HWA_ID_V4M_FBA_IMP_OCV3 */
    &sysc_info_A2PSC,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DP0 */
    &sysc_info_A2DMA,    /* R_PMA_HWA_ID_V4M_FBA_IMP_DP1 */
    &sysc_info_A1CN0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_CNN0 */
    &sysc_info_A1DSP0,   /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP0 */
    &sysc_info_A1DSP1,   /* R_PMA_HWA_ID_V4M_FBA_IMP_DSP1 */
    &sysc_info_A2CN0,    /* R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0 */
    &sysc_info_A3VIP0,      /* R_PMA_HWA_ID_V4M_FBA_UMFL0 */
    &sysc_info_A3VIP2,   /* R_PMA_HWA_ID_V4M_FBA_SMPS0 */
    &sysc_info_A3VIP2,   /* R_PMA_HWA_ID_V4M_FBA_SMPO0 */
    &sysc_info_A3DUL,    /* R_PMA_HWA_ID_V4M_FBA_PAP0 */
    &sysc_info_A3ISP0,   /* R_PMA_HWA_ID_V4M_FBA_ISP0 */
    &sysc_info_A3IMR0,   /* R_PMA_HWA_ID_V4M_FBA_IMR0 */
    &sysc_info_A3IMR1,   /* R_PMA_HWA_ID_V4M_FBA_IMR1 */
    &sysc_info_A3IMR2,   /* R_PMA_HWA_ID_V4M_FBA_IMS0 */
    &sysc_info_A3IMR3,   /* R_PMA_HWA_ID_V4M_FBA_IMS1 */
    &hwa_info_NONE,      /* R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET */
    &hwa_info_NONE,      /* R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET */
};

#endif  /* R_PMA_FUNCTIONAL_TEST_SEQUENCE_PATTERN_V4M_H */
