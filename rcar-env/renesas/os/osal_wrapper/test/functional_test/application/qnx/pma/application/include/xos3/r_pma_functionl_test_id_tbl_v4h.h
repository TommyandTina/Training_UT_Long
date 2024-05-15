#ifndef R_PMA_FUNCTIONAL_TEST_ID_TBL_V4H_H
#define R_PMA_FUNCTIONAL_TEST_ID_TBL_V4H_H

#include "target/common/pma/hw/r_pma_hwa_id.h"
/*======================================================================================================================
Private global variables
======================================================================================================================*/

static e_pma_hwa_id_t g_pma_test_no_sysc_hwa_id[] = {
    /* V4H */
    R_PMA_HWA_ID_V4H_IMPPSC              ,
    R_PMA_HWA_ID_V4H_IMPDMAC0            ,
    R_PMA_HWA_ID_V4H_IMP1                ,
    R_PMA_HWA_ID_V4H_IMP0                ,
    R_PMA_HWA_ID_V4H_SPMC                ,
    R_PMA_HWA_ID_V4H_IMPSDMAC1           ,
    R_PMA_HWA_ID_V4H_IMPSDMAC0           ,
    R_PMA_HWA_ID_V4H_IMPCNN              ,
    R_PMA_HWA_ID_V4H_ISP1                ,
    R_PMA_HWA_ID_V4H_ISP0                ,
    R_PMA_HWA_ID_V4H_ANRT1               ,
    R_PMA_HWA_ID_V4H_DOF                 ,
    R_PMA_HWA_ID_V4H_SMPS0               ,
    R_PMA_HWA_ID_V4H_SMPO0               ,
    R_PMA_HWA_ID_V4H_RGX                 ,
    R_PMA_HWA_ID_V4H_ANVI1               ,
    R_PMA_HWA_ID_V4H_ANVI0               ,
    R_PMA_HWA_ID_V4H_ANVC1               ,
    R_PMA_HWA_ID_V4H_ANVC0               ,
    R_PMA_HWA_ID_V4H_ANSP0               ,
    R_PMA_HWA_ID_V4H_ANRT                ,
    R_PMA_HWA_ID_V4H_ANPV0               ,
    R_PMA_HWA_ID_V4H_ADVFSC              ,
    R_PMA_HWA_ID_V4H_ADG                 ,
    R_PMA_HWA_ID_V4H_IMPSLV              ,
    R_PMA_HWA_ID_V4H_IMPDTA              ,
    R_PMA_HWA_ID_V4H_AXIMP0_AXRS         ,
    R_PMA_HWA_ID_V4H_AXIMP0              ,
    R_PMA_HWA_ID_V4H_ANIMP1              ,
    R_PMA_HWA_ID_V4H_ANIMP0              ,
    R_PMA_HWA_ID_V4H_IMPDMAC1            ,
    R_PMA_HWA_ID_V4H_IMP3                ,
    R_PMA_HWA_ID_V4H_IMP2                ,
/*     R_PMA_HWA_ID_V4H_AXSM_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXSD0_1             , */
/*     R_PMA_HWA_ID_V4H_AXSD0_0             , */
/*     R_PMA_HWA_ID_V4H_AXSC                , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSC           , */
/*     R_PMA_HWA_ID_V4H_AXRT_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXRT                , */
/*     R_PMA_HWA_ID_V4H_AXRD_1              , */
/*     R_PMA_HWA_ID_V4H_AXRD_0              , */
/*     R_PMA_HWA_ID_V4H_AXRC_1              , */
/*     R_PMA_HWA_ID_V4H_AXRC_0              , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXPV           , */
/*     R_PMA_HWA_ID_V4H_AXPV                , */
/*     R_PMA_HWA_ID_V4H_ANMM_AXMM           , */
/*     R_PMA_HWA_ID_V4H_AVB2                , */
/*     R_PMA_HWA_ID_V4H_AVB1                , */
/*     R_PMA_HWA_ID_V4H_AVB0                , */
/*     R_PMA_HWA_ID_V4H_ARMREG              , */
/*     R_PMA_HWA_ID_V4H_ARMM                , */
/*     R_PMA_HWA_ID_V4H_APSP3_1             , */
/*     R_PMA_HWA_ID_V4H_APSP3_0             , */
/*     R_PMA_HWA_ID_V4H_APSP4_1             , */
/*     R_PMA_HWA_ID_V4H_APSP4_0             , */
/*     R_PMA_HWA_ID_V4H_APRT0_1             , */
/*     R_PMA_HWA_ID_V4H_APRT0_0             , */
/*     R_PMA_HWA_ID_V4H_ANVIP2              , */
/*     R_PMA_HWA_ID_V4H_ANVIP1              , */
/*     R_PMA_HWA_ID_V4H_ANVIP0              , */
    R_PMA_HWA_ID_V4H_CSITOP0             ,
/*     R_PMA_HWA_ID_V4H_CSDBGPAP            , */
/*     R_PMA_HWA_ID_V4H_CR0                 , */
/*     R_PMA_HWA_ID_V4H_CANFD               , */
/*     R_PMA_HWA_ID_V4H_AXVIP1_1            , */
/*     R_PMA_HWA_ID_V4H_AXVIP1_0            , */
/*     R_PMA_HWA_ID_V4H_AXVIP0_AXRS         , */
/*     R_PMA_HWA_ID_V4H_AXVIP0              , */
/*     R_PMA_HWA_ID_V4H_AXVIFF              , */
/*     R_PMA_HWA_ID_V4H_AXVI1               , */
/*     R_PMA_HWA_ID_V4H_AXVI1_AXRS          , */
/*     R_PMA_HWA_ID_V4H_AXVI                , */
/*     R_PMA_HWA_ID_V4H_AXVC_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXVC                , */
/*     R_PMA_HWA_ID_V4H_AXTBH               , */
/*     R_PMA_HWA_ID_V4H_AXSTM               , */
/*     R_PMA_HWA_ID_V4H_AXSTM_AXRS          , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSP0          , */
/*     R_PMA_HWA_ID_V4H_AXSP0               , */
/*     R_PMA_HWA_ID_V4H_AXSN                , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSN           , */
/*     R_PMA_HWA_ID_V4H_AXSM                , */
    R_PMA_HWA_ID_V4H_DSITXLINK1          ,
    R_PMA_HWA_ID_V4H_DSITXLINK0          ,
    R_PMA_HWA_ID_V4H_DOC2CH              ,
    R_PMA_HWA_ID_V4H_DIS0                ,
/*     R_PMA_HWA_ID_V4H_DDR3_DDR2           , */
/*     R_PMA_HWA_ID_V4H_DDR1_DDR0           , */
    R_PMA_HWA_ID_V4H_CSITOP1             ,
/*     R_PMA_HWA_ID_V4H_INTAP               , */
    R_PMA_HWA_ID_V4H_IMS1                ,
    R_PMA_HWA_ID_V4H_IMS0                ,
    R_PMA_HWA_ID_V4H_IMR2                ,
    R_PMA_HWA_ID_V4H_IMR1                ,
    R_PMA_HWA_ID_V4H_IMR0                ,
/*     R_PMA_HWA_ID_V4H_I2C5                , */
/*     R_PMA_HWA_ID_V4H_I2C4                , */
/*     R_PMA_HWA_ID_V4H_I2C3                , */
/*     R_PMA_HWA_ID_V4H_I2C2                , */
/*     R_PMA_HWA_ID_V4H_I2C1                , */
/*     R_PMA_HWA_ID_V4H_I2C0                , */
/*     R_PMA_HWA_ID_V4H_HSCIF3              , */
/*     R_PMA_HWA_ID_V4H_HSCIF2              , */
/*     R_PMA_HWA_ID_V4H_HSCIF1              , */
/*     R_PMA_HWA_ID_V4H_HSCIF0              , */
    R_PMA_HWA_ID_V4H_FRAY00              ,
    R_PMA_HWA_ID_V4H_FCPVD1              ,
    R_PMA_HWA_ID_V4H_FCPVD0              ,
    R_PMA_HWA_ID_V4H_FCPCS               ,
/*     R_PMA_HWA_ID_V4H_RTDM1               , */
/*     R_PMA_HWA_ID_V4H_RTDM0               , */
/*     R_PMA_HWA_ID_V4H_RPC                 , */
/*     R_PMA_HWA_ID_V4H_PWM                 , */
/*     R_PMA_HWA_ID_V4H_PCIE10              , */
/*     R_PMA_HWA_ID_V4H_PCIE00              , */
    R_PMA_HWA_ID_V4H_MSI5                ,
    R_PMA_HWA_ID_V4H_MSI4                ,
    R_PMA_HWA_ID_V4H_MSI3                ,
    R_PMA_HWA_ID_V4H_MSI2                ,
    R_PMA_HWA_ID_V4H_MSI1                ,
    R_PMA_HWA_ID_V4H_MSI0                ,
    R_PMA_HWA_ID_V4H_IVCP1E              ,
    R_PMA_HWA_ID_V4H_ISPCS1              ,
    R_PMA_HWA_ID_V4H_ISPCS0              ,
/*     R_PMA_HWA_ID_V4H_IRQC                , */
/*     R_PMA_HWA_ID_V4H_INTTP               , */
/*     R_PMA_HWA_ID_V4H_IPC                 , */
    R_PMA_HWA_ID_V4H_VIN01               ,
    R_PMA_HWA_ID_V4H_VIN00               ,
    R_PMA_HWA_ID_V4H_VCPL4               ,
    R_PMA_HWA_ID_V4H_AES_ACC7            ,
    R_PMA_HWA_ID_V4H_AES_ACC6            ,
    R_PMA_HWA_ID_V4H_AES_ACC5            ,
    R_PMA_HWA_ID_V4H_AES_ACC4            ,
    R_PMA_HWA_ID_V4H_AES_ACC3            ,
    R_PMA_HWA_ID_V4H_AES_ACC2            ,
    R_PMA_HWA_ID_V4H_AES_ACC1            ,
    R_PMA_HWA_ID_V4H_AES_ACC0            ,
    R_PMA_HWA_ID_V4H_AES_ACC_WRAP        ,
/*     R_PMA_HWA_ID_V4H_TPU0                , */
/*     R_PMA_HWA_ID_V4H_TMU4                , */
/*     R_PMA_HWA_ID_V4H_TMU3                , */
/*     R_PMA_HWA_ID_V4H_TMU2                , */
/*     R_PMA_HWA_ID_V4H_TMU1                , */
/*     R_PMA_HWA_ID_V4H_TMU0                , */
/*     R_PMA_HWA_ID_V4H_SYDM2               , */
/*     R_PMA_HWA_ID_V4H_SYDM1               , */
/*     R_PMA_HWA_ID_V4H_SECROM              , */
/*     R_PMA_HWA_ID_V4H_SDHI0               , */
/*     R_PMA_HWA_ID_V4H_SCIF4               , */
/*     R_PMA_HWA_ID_V4H_SCIF3               , */
/*     R_PMA_HWA_ID_V4H_SCIF1               , */
/*     R_PMA_HWA_ID_V4H_SCIF0               , */
/*     R_PMA_HWA_ID_V4H_RTDM3               , */
/*     R_PMA_HWA_ID_V4H_RTDM2               , */
    R_PMA_HWA_ID_V4H_VSPD1               ,
    R_PMA_HWA_ID_V4H_VSPD0               ,
    R_PMA_HWA_ID_V4H_VIN17               ,
    R_PMA_HWA_ID_V4H_VIN16               ,
    R_PMA_HWA_ID_V4H_VIN15               ,
    R_PMA_HWA_ID_V4H_VIN14               ,
    R_PMA_HWA_ID_V4H_VIN13               ,
    R_PMA_HWA_ID_V4H_VIN12               ,
    R_PMA_HWA_ID_V4H_VIN11               ,
    R_PMA_HWA_ID_V4H_VIN10               ,
    R_PMA_HWA_ID_V4H_VIN07               ,
    R_PMA_HWA_ID_V4H_VIN06               ,
    R_PMA_HWA_ID_V4H_VIN05               ,
    R_PMA_HWA_ID_V4H_VIN04               ,
    R_PMA_HWA_ID_V4H_VIN03               ,
    R_PMA_HWA_ID_V4H_VIN02               ,
/*     R_PMA_HWA_ID_V4H_UCMT                , */
/*     R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1 , */
/*     R_PMA_HWA_ID_V4H_PFC3                , */
/*     R_PMA_HWA_ID_V4H_PFC2                , */
/*     R_PMA_HWA_ID_V4H_PFC1                , */
/*     R_PMA_HWA_ID_V4H_PFC0                , */
/*     R_PMA_HWA_ID_V4H_CMT3                , */
/*     R_PMA_HWA_ID_V4H_CMT2                , */
/*     R_PMA_HWA_ID_V4H_CMT1                , */
/*     R_PMA_HWA_ID_V4H_CMT0                , */
/*     R_PMA_HWA_ID_V4H_WDT1_WDT0           , */
    R_PMA_HWA_ID_V4H_WCRC3               ,
    R_PMA_HWA_ID_V4H_WCRC2               ,
    R_PMA_HWA_ID_V4H_WCRC1               ,
    R_PMA_HWA_ID_V4H_WCRC0               ,
    R_PMA_HWA_ID_V4H_VSPX1               ,
    R_PMA_HWA_ID_V4H_VSPX0               ,
    R_PMA_HWA_ID_V4H_RGXC                ,
    R_PMA_HWA_ID_V4H_RGXB                ,
/*     R_PMA_HWA_ID_V4H_SWDT1_SWDT0         , */
    R_PMA_HWA_ID_V4H_RGXPVC              ,
    R_PMA_HWA_ID_V4H_RGXPVDE             ,
/*     R_PMA_HWA_ID_V4H_PCIE11              , */
/*     R_PMA_HWA_ID_V4H_PCIE01              , */
/*     R_PMA_HWA_ID_V4H_INTAP11             , */
    R_PMA_HWA_ID_V4H_FRAY01              ,
/*     R_PMA_HWA_ID_V4H_AXVI11              , */
/*     R_PMA_HWA_ID_V4H_AULK4P              , */
/*     R_PMA_HWA_ID_V4H_AULK2P              , */
    R_PMA_HWA_ID_V4H_FCPVX1              ,
    R_PMA_HWA_ID_V4H_FCPVX0              ,
/*     R_PMA_HWA_ID_V4H_KCRC4               , */
/*     R_PMA_HWA_ID_V4H_FSO                 , */
/*     R_PMA_HWA_ID_V4H_CRC3                , */
/*     R_PMA_HWA_ID_V4H_CRC2                , */
/*     R_PMA_HWA_ID_V4H_CRC1                , */
/*     R_PMA_HWA_ID_V4H_CRC0                , */
/*     R_PMA_HWA_ID_V4H_ADVFS               , */
/*     R_PMA_HWA_ID_V4H_SCMT                , */
/*     R_PMA_HWA_ID_V4H_PFC3D3              , */
/*     R_PMA_HWA_ID_V4H_PFC3D2              , */
/*     R_PMA_HWA_ID_V4H_PFC3D1              , */
/*     R_PMA_HWA_ID_V4H_PFC2D3              , */
/*     R_PMA_HWA_ID_V4H_PFC2D2              , */
/*     R_PMA_HWA_ID_V4H_PFC2D1              , */
/*     R_PMA_HWA_ID_V4H_PFC1D3              , */
/*     R_PMA_HWA_ID_V4H_PFC1D2              , */
/*     R_PMA_HWA_ID_V4H_PFC1D1              , */
/*     R_PMA_HWA_ID_V4H_PFC0D3              , */
/*     R_PMA_HWA_ID_V4H_PFC0D2              , */
/*     R_PMA_HWA_ID_V4H_PFC0D1              , */
/*     R_PMA_HWA_ID_V4H_WWDT9_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT8_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT7_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT6_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT5_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT4_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT3_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT2_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT1_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT0_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_DBE                 , */
/*     R_PMA_HWA_ID_V4H_MTI                 , */
/*     R_PMA_HWA_ID_V4H_KCRC7               , */
/*     R_PMA_HWA_ID_V4H_KCRC6               , */
/*     R_PMA_HWA_ID_V4H_KCRC5               , */
/*     R_PMA_HWA_ID_V4H_CKMMM               , */
/*     R_PMA_HWA_ID_V4H_CKMIR               , */
/*     R_PMA_HWA_ID_V4H_CKMVIP              , */
/*     R_PMA_HWA_ID_V4H_CKMVC               , */
/*     R_PMA_HWA_ID_V4H_CKMVIO              , */
/*     R_PMA_HWA_ID_V4H_CKMPE0              , */
/*     R_PMA_HWA_ID_V4H_CKMRT               , */
/*     R_PMA_HWA_ID_V4H_CKM3DG              , */
/*     R_PMA_HWA_ID_V4H_APVI4_1             , */
/*     R_PMA_HWA_ID_V4H_APVI4_0             , */
/*     R_PMA_HWA_ID_V4H_APVC1_1             , */
/*     R_PMA_HWA_ID_V4H_APVC1_0             , */
/*     R_PMA_HWA_ID_V4H_CKMDSP_CKMIR        , */
/*     R_PMA_HWA_ID_V4H_CKMCNR_CKMIR        , */
/*     R_PMA_HWA_ID_V4H_CKM                 , */
/*     R_PMA_HWA_ID_V4H_CKMHSC              , */
/*     R_PMA_HWA_ID_V4H_CKMCR52             , */
/*     R_PMA_HWA_ID_V4H_ANHC                , */
/*     R_PMA_HWA_ID_V4H_AXHC_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXHC                , */
/*     R_PMA_HWA_ID_V4H_CSBRG_IR_A2         , */
/*     R_PMA_HWA_ID_V4H_CSBRG_IR_A3         , */
/*     R_PMA_HWA_ID_V4H_TSN                 , */
    R_PMA_HWA_ID_V4H_CVE3                ,
    R_PMA_HWA_ID_V4H_CVE2                ,
    R_PMA_HWA_ID_V4H_CVE1                ,
    R_PMA_HWA_ID_V4H_CVE0                ,
    R_PMA_HWA_ID_V4H_VDSP1_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP1_REG           ,
    R_PMA_HWA_ID_V4H_VDSP0_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP0_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP0_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP0_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP0_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP0_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP0_REG           ,
    R_PMA_HWA_ID_V4H_VDSP0_CSB           ,
    R_PMA_HWA_ID_V4H_DSC                 ,
/*     R_PMA_HWA_ID_V4H_FCPRC               , */
    R_PMA_HWA_ID_V4H_PAPSDMA             ,
    R_PMA_HWA_ID_V4H_HSM                 ,
    R_PMA_HWA_ID_V4H_PAPBUS              ,
    R_PMA_HWA_ID_V4H_VDSP3_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP2_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP1_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP0_BUS           ,
    R_PMA_HWA_ID_V4H_SSI                 ,
/*     R_PMA_HWA_ID_V4H_SSIU                , */
    R_PMA_HWA_ID_V4H_VDSP3_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP3_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP3_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP3_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP3_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP3_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP3_REG           ,
    R_PMA_HWA_ID_V4H_VDSP3_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP2_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP2_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP2_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP2_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP2_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP2_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP2_REG           ,
    R_PMA_HWA_ID_V4H_VDSP2_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP1_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP1_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP1_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP1_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP1_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP1_GLOBAL        ,
/*     R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR      , */
    R_PMA_HWA_ID_V4H_REG_SPMC            ,
    R_PMA_HWA_ID_V4H_DOF_FCPRM           ,
    R_PMA_HWA_ID_V4H_DCM                 ,
    R_PMA_HWA_ID_V4H_CBF                 ,
    R_PMA_HWA_ID_V4H_SBF                 ,
    R_PMA_HWA_ID_V4H_OSM                 ,
    R_PMA_HWA_ID_V4H_ISM0                ,
    R_PMA_HWA_ID_V4H_ISM1                ,
    R_PMA_HWA_ID_V4H_FBS                 ,
    R_PMA_HWA_ID_V4H_APB                 ,
    R_PMA_HWA_ID_V4H_DUL                 ,
    R_PMA_HWA_ID_V4H_SPMC01              ,
    R_PMA_HWA_ID_V4H_SPMI                ,
    R_PMA_HWA_ID_V4H_REG_SPMI            ,
    R_PMA_HWA_ID_V4H_NUM
};

static e_pma_hwa_id_t g_pma_test_hwa_id[] = {
    /* V4H */
    R_PMA_HWA_ID_V4H_IMPPSC              ,
    R_PMA_HWA_ID_V4H_IMPDMAC0            ,
    R_PMA_HWA_ID_V4H_IMP1                ,
    R_PMA_HWA_ID_V4H_IMP0                ,
    R_PMA_HWA_ID_V4H_IMPSDMAC1           ,
    R_PMA_HWA_ID_V4H_IMPSDMAC0           ,
    R_PMA_HWA_ID_V4H_SPMC                ,
    R_PMA_HWA_ID_V4H_IMPCNN              ,
    R_PMA_HWA_ID_V4H_ISP1                ,
    R_PMA_HWA_ID_V4H_ISP0                ,
/*     R_PMA_HWA_ID_V4H_ANRT1               , */
    R_PMA_HWA_ID_V4H_DOF                 ,
    R_PMA_HWA_ID_V4H_SMPS0               ,
    R_PMA_HWA_ID_V4H_SMPO0               ,
    R_PMA_HWA_ID_V4H_RGX                 ,
/*     R_PMA_HWA_ID_V4H_ANVI1               , */
/*     R_PMA_HWA_ID_V4H_ANVI0               , */
/*     R_PMA_HWA_ID_V4H_ANVC1               , */
/*     R_PMA_HWA_ID_V4H_ANVC0               , */
/*     R_PMA_HWA_ID_V4H_ANSP0               , */
/*     R_PMA_HWA_ID_V4H_ANRT                , */
/*     R_PMA_HWA_ID_V4H_ANPV0               , */
/*     R_PMA_HWA_ID_V4H_ADVFSC              , */
/*     R_PMA_HWA_ID_V4H_ADG                 , */
    R_PMA_HWA_ID_V4H_IMPSLV              ,
    R_PMA_HWA_ID_V4H_IMPDTA              ,
/*     R_PMA_HWA_ID_V4H_AXIMP0_AXRS         , */
/*     R_PMA_HWA_ID_V4H_AXIMP0              , */
/*     R_PMA_HWA_ID_V4H_ANIMP1              , */
/*     R_PMA_HWA_ID_V4H_ANIMP0              , */
    R_PMA_HWA_ID_V4H_IMPDMAC1            ,
    R_PMA_HWA_ID_V4H_IMP3                ,
    R_PMA_HWA_ID_V4H_IMP2                ,
/*     R_PMA_HWA_ID_V4H_AXSM_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXSD0_1             , */
/*     R_PMA_HWA_ID_V4H_AXSD0_0             , */
/*     R_PMA_HWA_ID_V4H_AXSC                , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSC           , */
/*     R_PMA_HWA_ID_V4H_AXRT_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXRT                , */
/*     R_PMA_HWA_ID_V4H_AXRD_1              , */
/*     R_PMA_HWA_ID_V4H_AXRD_0              , */
/*     R_PMA_HWA_ID_V4H_AXRC_1              , */
/*     R_PMA_HWA_ID_V4H_AXRC_0              , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXPV           , */
/*     R_PMA_HWA_ID_V4H_AXPV                , */
/*     R_PMA_HWA_ID_V4H_ANMM_AXMM           , */
/*     R_PMA_HWA_ID_V4H_AVB2                , */
/*     R_PMA_HWA_ID_V4H_AVB1                , */
/*     R_PMA_HWA_ID_V4H_AVB0                , */
/*     R_PMA_HWA_ID_V4H_ARMREG              , */
/*     R_PMA_HWA_ID_V4H_ARMM                , */
/*     R_PMA_HWA_ID_V4H_APSP3_1             , */
/*     R_PMA_HWA_ID_V4H_APSP3_0             , */
/*     R_PMA_HWA_ID_V4H_APSP4_1             , */
/*     R_PMA_HWA_ID_V4H_APSP4_0             , */
/*     R_PMA_HWA_ID_V4H_APRT0_1             , */
/*     R_PMA_HWA_ID_V4H_APRT0_0             , */
/*     R_PMA_HWA_ID_V4H_ANVIP2              , */
/*     R_PMA_HWA_ID_V4H_ANVIP1              , */
/*     R_PMA_HWA_ID_V4H_ANVIP0              , */
    R_PMA_HWA_ID_V4H_CSITOP0             ,
/*     R_PMA_HWA_ID_V4H_CSDBGPAP            , */
/*     R_PMA_HWA_ID_V4H_CR0                 , */
/*     R_PMA_HWA_ID_V4H_CANFD               , */
/*     R_PMA_HWA_ID_V4H_AXVIP1_1            , */
/*     R_PMA_HWA_ID_V4H_AXVIP1_0            , */
/*     R_PMA_HWA_ID_V4H_AXVIP0_AXRS         , */
/*     R_PMA_HWA_ID_V4H_AXVIP0              , */
/*     R_PMA_HWA_ID_V4H_AXVIFF              , */
/*     R_PMA_HWA_ID_V4H_AXVI1               , */
/*     R_PMA_HWA_ID_V4H_AXVI1_AXRS          , */
/*     R_PMA_HWA_ID_V4H_AXVI                , */
/*     R_PMA_HWA_ID_V4H_AXVC_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXVC                , */
/*     R_PMA_HWA_ID_V4H_AXTBH               , */
/*     R_PMA_HWA_ID_V4H_AXSTM               , */
/*     R_PMA_HWA_ID_V4H_AXSTM_AXRS          , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSP0          , */
/*     R_PMA_HWA_ID_V4H_AXSP0               , */
/*     R_PMA_HWA_ID_V4H_AXSN                , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSN           , */
/*     R_PMA_HWA_ID_V4H_AXSM                , */
    R_PMA_HWA_ID_V4H_DSITXLINK1          ,
    R_PMA_HWA_ID_V4H_DSITXLINK0          ,
    R_PMA_HWA_ID_V4H_DOC2CH              ,
    R_PMA_HWA_ID_V4H_DIS0                ,
/*     R_PMA_HWA_ID_V4H_DDR3_DDR2           , */
/*     R_PMA_HWA_ID_V4H_DDR1_DDR0           , */
    R_PMA_HWA_ID_V4H_CSITOP1             ,
/*     R_PMA_HWA_ID_V4H_INTAP               , */
    R_PMA_HWA_ID_V4H_IMS1                ,
    R_PMA_HWA_ID_V4H_IMS0                ,
    R_PMA_HWA_ID_V4H_IMR2                ,
    R_PMA_HWA_ID_V4H_IMR1                ,
    R_PMA_HWA_ID_V4H_IMR0                ,
/*     R_PMA_HWA_ID_V4H_I2C5                , */
/*     R_PMA_HWA_ID_V4H_I2C4                , */
/*     R_PMA_HWA_ID_V4H_I2C3                , */
/*     R_PMA_HWA_ID_V4H_I2C2                , */
/*     R_PMA_HWA_ID_V4H_I2C1                , */
/*     R_PMA_HWA_ID_V4H_I2C0                , */
/*     R_PMA_HWA_ID_V4H_HSCIF3              , */
/*     R_PMA_HWA_ID_V4H_HSCIF2              , */
/*     R_PMA_HWA_ID_V4H_HSCIF1              , */
/*     R_PMA_HWA_ID_V4H_HSCIF0              , */
/*     R_PMA_HWA_ID_V4H_FRAY00              , */
/*     R_PMA_HWA_ID_V4H_FCPVD1              , */
/*     R_PMA_HWA_ID_V4H_FCPVD0              , */
    R_PMA_HWA_ID_V4H_FCPCS               ,
/*     R_PMA_HWA_ID_V4H_RTDM1               , */
/*     R_PMA_HWA_ID_V4H_RTDM0               , */
/*     R_PMA_HWA_ID_V4H_RPC                 , */
/*     R_PMA_HWA_ID_V4H_PWM                 , */
/*     R_PMA_HWA_ID_V4H_PCIE10              , */
/*     R_PMA_HWA_ID_V4H_PCIE00              , */
/*     R_PMA_HWA_ID_V4H_MSI5                , */
/*     R_PMA_HWA_ID_V4H_MSI4                , */
/*     R_PMA_HWA_ID_V4H_MSI3                , */
/*     R_PMA_HWA_ID_V4H_MSI2                , */
/*     R_PMA_HWA_ID_V4H_MSI1                , */
/*     R_PMA_HWA_ID_V4H_MSI0                , */
    R_PMA_HWA_ID_V4H_IVCP1E              ,
    R_PMA_HWA_ID_V4H_ISPCS1              ,
    R_PMA_HWA_ID_V4H_ISPCS0              ,
/*     R_PMA_HWA_ID_V4H_IRQC                , */
/*     R_PMA_HWA_ID_V4H_INTTP               , */
/*     R_PMA_HWA_ID_V4H_IPC                 , */
    R_PMA_HWA_ID_V4H_VIN01               ,
    R_PMA_HWA_ID_V4H_VIN00               ,
    R_PMA_HWA_ID_V4H_VCPL4               ,
    R_PMA_HWA_ID_V4H_AES_ACC7            ,
    R_PMA_HWA_ID_V4H_AES_ACC6            ,
    R_PMA_HWA_ID_V4H_AES_ACC5            ,
    R_PMA_HWA_ID_V4H_AES_ACC4            ,
    R_PMA_HWA_ID_V4H_AES_ACC3            ,
    R_PMA_HWA_ID_V4H_AES_ACC2            ,
    R_PMA_HWA_ID_V4H_AES_ACC1            ,
    R_PMA_HWA_ID_V4H_AES_ACC0            ,
    R_PMA_HWA_ID_V4H_AES_ACC_WRAP        ,
/*     R_PMA_HWA_ID_V4H_TPU0                , */
/*     R_PMA_HWA_ID_V4H_TMU4                , */
/*     R_PMA_HWA_ID_V4H_TMU3                , */
/*     R_PMA_HWA_ID_V4H_TMU2                , */
/*     R_PMA_HWA_ID_V4H_TMU1                , */
/*     R_PMA_HWA_ID_V4H_TMU0                , */
/*     R_PMA_HWA_ID_V4H_SYDM2               , */
/*     R_PMA_HWA_ID_V4H_SYDM1               , */
/*     R_PMA_HWA_ID_V4H_SECROM              , */
/*     R_PMA_HWA_ID_V4H_SDHI0               , */
/*     R_PMA_HWA_ID_V4H_SCIF4               , */
/*     R_PMA_HWA_ID_V4H_SCIF3               , */
/*     R_PMA_HWA_ID_V4H_SCIF1               , */
/*     R_PMA_HWA_ID_V4H_SCIF0               , */
/*     R_PMA_HWA_ID_V4H_RTDM3               , */
/*     R_PMA_HWA_ID_V4H_RTDM2               , */
    R_PMA_HWA_ID_V4H_VSPD1               ,
    R_PMA_HWA_ID_V4H_VSPD0               ,
    R_PMA_HWA_ID_V4H_VIN17               ,
    R_PMA_HWA_ID_V4H_VIN16               ,
    R_PMA_HWA_ID_V4H_VIN15               ,
    R_PMA_HWA_ID_V4H_VIN14               ,
    R_PMA_HWA_ID_V4H_VIN13               ,
    R_PMA_HWA_ID_V4H_VIN12               ,
    R_PMA_HWA_ID_V4H_VIN11               ,
    R_PMA_HWA_ID_V4H_VIN10               ,
    R_PMA_HWA_ID_V4H_VIN07               ,
    R_PMA_HWA_ID_V4H_VIN06               ,
    R_PMA_HWA_ID_V4H_VIN05               ,
    R_PMA_HWA_ID_V4H_VIN04               ,
    R_PMA_HWA_ID_V4H_VIN03               ,
    R_PMA_HWA_ID_V4H_VIN02               ,
/*     R_PMA_HWA_ID_V4H_UCMT                , */
/*     R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1 , */
/*     R_PMA_HWA_ID_V4H_PFC3                , */
/*     R_PMA_HWA_ID_V4H_PFC2                , */
/*     R_PMA_HWA_ID_V4H_PFC1                , */
/*     R_PMA_HWA_ID_V4H_PFC0                , */
/*     R_PMA_HWA_ID_V4H_CMT3                , */
/*     R_PMA_HWA_ID_V4H_CMT2                , */
/*     R_PMA_HWA_ID_V4H_CMT1                , */
/*     R_PMA_HWA_ID_V4H_CMT0                , */
/*     R_PMA_HWA_ID_V4H_WDT1_WDT0           , */
    R_PMA_HWA_ID_V4H_WCRC3               ,
    R_PMA_HWA_ID_V4H_WCRC2               ,
    R_PMA_HWA_ID_V4H_WCRC1               ,
    R_PMA_HWA_ID_V4H_WCRC0               ,
    R_PMA_HWA_ID_V4H_VSPX1               ,
    R_PMA_HWA_ID_V4H_VSPX0               ,
/*     R_PMA_HWA_ID_V4H_RGXC                , */
/*     R_PMA_HWA_ID_V4H_RGXB                , */
/*     R_PMA_HWA_ID_V4H_SWDT1_SWDT0         , */
/*     R_PMA_HWA_ID_V4H_RGXPVC              , */
/*     R_PMA_HWA_ID_V4H_RGXPVDE             , */
/*     R_PMA_HWA_ID_V4H_PCIE11              , */
/*     R_PMA_HWA_ID_V4H_PCIE01              , */
/*     R_PMA_HWA_ID_V4H_INTAP11             , */
/*     R_PMA_HWA_ID_V4H_FRAY01              , */
/*     R_PMA_HWA_ID_V4H_AXVI11              , */
/*     R_PMA_HWA_ID_V4H_AULK4P              , */
/*     R_PMA_HWA_ID_V4H_AULK2P              , */
    R_PMA_HWA_ID_V4H_FCPVX1              ,
    R_PMA_HWA_ID_V4H_FCPVX0              ,
/*     R_PMA_HWA_ID_V4H_KCRC4               , */
/*     R_PMA_HWA_ID_V4H_FSO                 , */
/*     R_PMA_HWA_ID_V4H_CRC3                , */
/*     R_PMA_HWA_ID_V4H_CRC2                , */
/*     R_PMA_HWA_ID_V4H_CRC1                , */
/*     R_PMA_HWA_ID_V4H_CRC0                , */
/*     R_PMA_HWA_ID_V4H_ADVFS               , */
/*     R_PMA_HWA_ID_V4H_SCMT                , */
/*     R_PMA_HWA_ID_V4H_PFC3D3              , */
/*     R_PMA_HWA_ID_V4H_PFC3D2              , */
/*     R_PMA_HWA_ID_V4H_PFC3D1              , */
/*     R_PMA_HWA_ID_V4H_PFC2D3              , */
/*     R_PMA_HWA_ID_V4H_PFC2D2              , */
/*     R_PMA_HWA_ID_V4H_PFC2D1              , */
/*     R_PMA_HWA_ID_V4H_PFC1D3              , */
/*     R_PMA_HWA_ID_V4H_PFC1D2              , */
/*     R_PMA_HWA_ID_V4H_PFC1D1              , */
/*     R_PMA_HWA_ID_V4H_PFC0D3              , */
/*     R_PMA_HWA_ID_V4H_PFC0D2              , */
/*     R_PMA_HWA_ID_V4H_PFC0D1              , */
/*     R_PMA_HWA_ID_V4H_WWDT9_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT8_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT7_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT6_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT5_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT4_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT3_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT2_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT1_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT0_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_DBE                 , */
/*     R_PMA_HWA_ID_V4H_MTI                 , */
/*     R_PMA_HWA_ID_V4H_KCRC7               , */
/*     R_PMA_HWA_ID_V4H_KCRC6               , */
/*     R_PMA_HWA_ID_V4H_KCRC5               , */
/*     R_PMA_HWA_ID_V4H_CKMMM               , */
/*     R_PMA_HWA_ID_V4H_CKMIR               , */
/*     R_PMA_HWA_ID_V4H_CKMVIP              , */
/*     R_PMA_HWA_ID_V4H_CKMVC               , */
/*     R_PMA_HWA_ID_V4H_CKMVIO              , */
/*     R_PMA_HWA_ID_V4H_CKMPE0              , */
/*     R_PMA_HWA_ID_V4H_CKMRT               , */
/*     R_PMA_HWA_ID_V4H_CKM3DG              , */
/*     R_PMA_HWA_ID_V4H_APVI4_1             , */
/*     R_PMA_HWA_ID_V4H_APVI4_0             , */
/*     R_PMA_HWA_ID_V4H_APVC1_1             , */
/*     R_PMA_HWA_ID_V4H_APVC1_0             , */
/*     R_PMA_HWA_ID_V4H_CKMDSP_CKMIR        , */
/*     R_PMA_HWA_ID_V4H_CKMCNR_CKMIR        , */
/*     R_PMA_HWA_ID_V4H_CKM                 , */
/*     R_PMA_HWA_ID_V4H_CKMHSC              , */
/*     R_PMA_HWA_ID_V4H_CKMCR52             , */
/*     R_PMA_HWA_ID_V4H_ANHC                , */
/*     R_PMA_HWA_ID_V4H_AXHC_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXHC                , */
/*     R_PMA_HWA_ID_V4H_CSBRG_IR_A2         , */
/*     R_PMA_HWA_ID_V4H_CSBRG_IR_A3         , */
/*     R_PMA_HWA_ID_V4H_TSN                 , */
    R_PMA_HWA_ID_V4H_CVE3                ,
    R_PMA_HWA_ID_V4H_CVE2                ,
    R_PMA_HWA_ID_V4H_CVE1                ,
    R_PMA_HWA_ID_V4H_CVE0                ,
    R_PMA_HWA_ID_V4H_VDSP1_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP1_REG           ,
    R_PMA_HWA_ID_V4H_VDSP0_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP0_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP0_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP0_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP0_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP0_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP0_REG           ,
    R_PMA_HWA_ID_V4H_VDSP0_CSB           ,
    R_PMA_HWA_ID_V4H_DSC                 ,
/*     R_PMA_HWA_ID_V4H_FCPRC               , */
    R_PMA_HWA_ID_V4H_PAPSDMA             ,
    R_PMA_HWA_ID_V4H_HSM                 ,
    R_PMA_HWA_ID_V4H_PAPBUS              ,
    R_PMA_HWA_ID_V4H_VDSP3_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP2_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP1_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP0_BUS           ,
    R_PMA_HWA_ID_V4H_SSI                 ,
/*     R_PMA_HWA_ID_V4H_SSIU                , */
    R_PMA_HWA_ID_V4H_VDSP3_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP3_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP3_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP3_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP3_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP3_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP3_REG           ,
    R_PMA_HWA_ID_V4H_VDSP3_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP2_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP2_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP2_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP2_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP2_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP2_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP2_REG           ,
    R_PMA_HWA_ID_V4H_VDSP2_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP1_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP1_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP1_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP1_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP1_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP1_GLOBAL        ,
    R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR      ,
    R_PMA_HWA_ID_V4H_REG_SPMC            ,
    R_PMA_HWA_ID_V4H_DOF_FCPRM           ,
    R_PMA_HWA_ID_V4H_DCM                 ,
    R_PMA_HWA_ID_V4H_CBF                 ,
    R_PMA_HWA_ID_V4H_SBF                 ,
    R_PMA_HWA_ID_V4H_OSM                 ,
    R_PMA_HWA_ID_V4H_ISM0                ,
    R_PMA_HWA_ID_V4H_ISM1                ,
    R_PMA_HWA_ID_V4H_FBS                 ,
    R_PMA_HWA_ID_V4H_APB                 ,
    R_PMA_HWA_ID_V4H_DUL                 ,
    R_PMA_HWA_ID_V4H_SPMC01              ,
    R_PMA_HWA_ID_V4H_SPMI                ,
    R_PMA_HWA_ID_V4H_REG_SPMI            ,
    R_PMA_HWA_ID_V4H_NUM
};

static e_pma_hwa_id_t g_pma_test_reset_hwa_id[] = {
    /* V4H */
    R_PMA_HWA_ID_V4H_IMPPSC              ,
    R_PMA_HWA_ID_V4H_IMPDMAC0            ,
    R_PMA_HWA_ID_V4H_IMP1                ,
    R_PMA_HWA_ID_V4H_IMP0                ,
    R_PMA_HWA_ID_V4H_IMPSDMAC1           ,
    R_PMA_HWA_ID_V4H_IMPSDMAC0           ,
    R_PMA_HWA_ID_V4H_SPMC                ,
    R_PMA_HWA_ID_V4H_IMPCNN              ,
    R_PMA_HWA_ID_V4H_ISP1                ,
    R_PMA_HWA_ID_V4H_ISP0                ,
/*     R_PMA_HWA_ID_V4H_ANRT1               , */
    R_PMA_HWA_ID_V4H_DOF                 ,
    R_PMA_HWA_ID_V4H_SMPS0               ,
    R_PMA_HWA_ID_V4H_SMPO0               ,
    R_PMA_HWA_ID_V4H_RGX                 ,
/*     R_PMA_HWA_ID_V4H_ANVI1               , */
/*     R_PMA_HWA_ID_V4H_ANVI0               , */
/*     R_PMA_HWA_ID_V4H_ANVC1               , */
/*     R_PMA_HWA_ID_V4H_ANVC0               , */
/*     R_PMA_HWA_ID_V4H_ANSP0               , */
/*     R_PMA_HWA_ID_V4H_ANRT                , */
/*     R_PMA_HWA_ID_V4H_ANPV0               , */
/*     R_PMA_HWA_ID_V4H_ADVFSC              , */
/*     R_PMA_HWA_ID_V4H_ADG                 , */
    R_PMA_HWA_ID_V4H_IMPSLV              ,
    R_PMA_HWA_ID_V4H_IMPDTA              ,
/*     R_PMA_HWA_ID_V4H_AXIMP0_AXRS         , */
/*     R_PMA_HWA_ID_V4H_AXIMP0              , */
/*     R_PMA_HWA_ID_V4H_ANIMP1              , */
/*     R_PMA_HWA_ID_V4H_ANIMP0              , */
    R_PMA_HWA_ID_V4H_IMPDMAC1            ,
    R_PMA_HWA_ID_V4H_IMP3                ,
    R_PMA_HWA_ID_V4H_IMP2                ,
/*     R_PMA_HWA_ID_V4H_AXSM_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXSD0_1             , */
/*     R_PMA_HWA_ID_V4H_AXSD0_0             , */
/*     R_PMA_HWA_ID_V4H_AXSC                , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSC           , */
/*     R_PMA_HWA_ID_V4H_AXRT_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXRT                , */
/*     R_PMA_HWA_ID_V4H_AXRD_1              , */
/*     R_PMA_HWA_ID_V4H_AXRD_0              , */
/*     R_PMA_HWA_ID_V4H_AXRC_1              , */
/*     R_PMA_HWA_ID_V4H_AXRC_0              , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXPV           , */
/*     R_PMA_HWA_ID_V4H_AXPV                , */
/*     R_PMA_HWA_ID_V4H_ANMM_AXMM           , */
/*     R_PMA_HWA_ID_V4H_AVB2                , */
/*     R_PMA_HWA_ID_V4H_AVB1                , */
/*     R_PMA_HWA_ID_V4H_AVB0                , */
/*     R_PMA_HWA_ID_V4H_ARMREG              , */
/*     R_PMA_HWA_ID_V4H_ARMM                , */
/*     R_PMA_HWA_ID_V4H_APSP3_1             , */
/*     R_PMA_HWA_ID_V4H_APSP3_0             , */
/*     R_PMA_HWA_ID_V4H_APSP4_1             , */
/*     R_PMA_HWA_ID_V4H_APSP4_0             , */
/*     R_PMA_HWA_ID_V4H_APRT0_1             , */
/*     R_PMA_HWA_ID_V4H_APRT0_0             , */
/*     R_PMA_HWA_ID_V4H_ANVIP2              , */
/*     R_PMA_HWA_ID_V4H_ANVIP1              , */
/*     R_PMA_HWA_ID_V4H_ANVIP0              , */
    R_PMA_HWA_ID_V4H_CSITOP0             ,
/*     R_PMA_HWA_ID_V4H_CSDBGPAP            , */
/*     R_PMA_HWA_ID_V4H_CR0                 , */
/*     R_PMA_HWA_ID_V4H_CANFD               , */
/*     R_PMA_HWA_ID_V4H_AXVIP1_1            , */
/*     R_PMA_HWA_ID_V4H_AXVIP1_0            , */
/*     R_PMA_HWA_ID_V4H_AXVIP0_AXRS         , */
/*     R_PMA_HWA_ID_V4H_AXVIP0              , */
/*     R_PMA_HWA_ID_V4H_AXVIFF              , */
/*     R_PMA_HWA_ID_V4H_AXVI1               , */
/*     R_PMA_HWA_ID_V4H_AXVI1_AXRS          , */
/*     R_PMA_HWA_ID_V4H_AXVI                , */
/*     R_PMA_HWA_ID_V4H_AXVC_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXVC                , */
/*     R_PMA_HWA_ID_V4H_AXTBH               , */
/*     R_PMA_HWA_ID_V4H_AXSTM               , */
/*     R_PMA_HWA_ID_V4H_AXSTM_AXRS          , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSP0          , */
/*     R_PMA_HWA_ID_V4H_AXSP0               , */
/*     R_PMA_HWA_ID_V4H_AXSN                , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSN           , */
/*     R_PMA_HWA_ID_V4H_AXSM                , */
    R_PMA_HWA_ID_V4H_DSITXLINK1          ,
    R_PMA_HWA_ID_V4H_DSITXLINK0          ,
    R_PMA_HWA_ID_V4H_DOC2CH              ,
    R_PMA_HWA_ID_V4H_DIS0                ,
/*     R_PMA_HWA_ID_V4H_DDR3_DDR2           , */
/*     R_PMA_HWA_ID_V4H_DDR1_DDR0           , */
    R_PMA_HWA_ID_V4H_CSITOP1             ,
/*     R_PMA_HWA_ID_V4H_INTAP               , */
    R_PMA_HWA_ID_V4H_IMS1                ,
    R_PMA_HWA_ID_V4H_IMS0                ,
    R_PMA_HWA_ID_V4H_IMR2                ,
    R_PMA_HWA_ID_V4H_IMR1                ,
    R_PMA_HWA_ID_V4H_IMR0                ,
/*     R_PMA_HWA_ID_V4H_I2C5                , */
/*     R_PMA_HWA_ID_V4H_I2C4                , */
/*     R_PMA_HWA_ID_V4H_I2C3                , */
/*     R_PMA_HWA_ID_V4H_I2C2                , */
/*     R_PMA_HWA_ID_V4H_I2C1                , */
/*     R_PMA_HWA_ID_V4H_I2C0                , */
/*     R_PMA_HWA_ID_V4H_HSCIF3              , */
/*     R_PMA_HWA_ID_V4H_HSCIF2              , */
/*     R_PMA_HWA_ID_V4H_HSCIF1              , */
/*     R_PMA_HWA_ID_V4H_HSCIF0              , */
/*     R_PMA_HWA_ID_V4H_FRAY00              , */
/*     R_PMA_HWA_ID_V4H_FCPVD1              , */
/*     R_PMA_HWA_ID_V4H_FCPVD0              , */
    R_PMA_HWA_ID_V4H_FCPCS               ,
/*     R_PMA_HWA_ID_V4H_RTDM1               , */
/*     R_PMA_HWA_ID_V4H_RTDM0               , */
/*     R_PMA_HWA_ID_V4H_RPC                 , */
/*     R_PMA_HWA_ID_V4H_PWM                 , */
/*     R_PMA_HWA_ID_V4H_PCIE10              , */
/*     R_PMA_HWA_ID_V4H_PCIE00              , */
/*     R_PMA_HWA_ID_V4H_MSI5                , */
/*     R_PMA_HWA_ID_V4H_MSI4                , */
/*     R_PMA_HWA_ID_V4H_MSI3                , */
/*     R_PMA_HWA_ID_V4H_MSI2                , */
/*     R_PMA_HWA_ID_V4H_MSI1                , */
/*     R_PMA_HWA_ID_V4H_MSI0                , */
    R_PMA_HWA_ID_V4H_IVCP1E              ,
    R_PMA_HWA_ID_V4H_ISPCS1              ,
    R_PMA_HWA_ID_V4H_ISPCS0              ,
/*     R_PMA_HWA_ID_V4H_IRQC                , */
/*     R_PMA_HWA_ID_V4H_INTTP               , */
/*     R_PMA_HWA_ID_V4H_IPC                 , */
    R_PMA_HWA_ID_V4H_VIN01               ,
    R_PMA_HWA_ID_V4H_VIN00               ,
    R_PMA_HWA_ID_V4H_VCPL4               ,
    R_PMA_HWA_ID_V4H_AES_ACC7            ,
    R_PMA_HWA_ID_V4H_AES_ACC6            ,
    R_PMA_HWA_ID_V4H_AES_ACC5            ,
    R_PMA_HWA_ID_V4H_AES_ACC4            ,
    R_PMA_HWA_ID_V4H_AES_ACC3            ,
    R_PMA_HWA_ID_V4H_AES_ACC2            ,
    R_PMA_HWA_ID_V4H_AES_ACC1            ,
    R_PMA_HWA_ID_V4H_AES_ACC0            ,
    R_PMA_HWA_ID_V4H_AES_ACC_WRAP        ,
/*     R_PMA_HWA_ID_V4H_TPU0                , */
/*     R_PMA_HWA_ID_V4H_TMU4                , */
/*     R_PMA_HWA_ID_V4H_TMU3                , */
/*     R_PMA_HWA_ID_V4H_TMU2                , */
/*     R_PMA_HWA_ID_V4H_TMU1                , */
/*     R_PMA_HWA_ID_V4H_TMU0                , */
/*     R_PMA_HWA_ID_V4H_SYDM2               , */
/*     R_PMA_HWA_ID_V4H_SYDM1               , */
/*     R_PMA_HWA_ID_V4H_SECROM              , */
/*     R_PMA_HWA_ID_V4H_SDHI0               , */
/*     R_PMA_HWA_ID_V4H_SCIF4               , */
/*     R_PMA_HWA_ID_V4H_SCIF3               , */
/*     R_PMA_HWA_ID_V4H_SCIF1               , */
/*     R_PMA_HWA_ID_V4H_SCIF0               , */
/*     R_PMA_HWA_ID_V4H_RTDM3               , */
/*     R_PMA_HWA_ID_V4H_RTDM2               , */
    R_PMA_HWA_ID_V4H_VSPD1               ,
    R_PMA_HWA_ID_V4H_VSPD0               ,
    R_PMA_HWA_ID_V4H_VIN17               ,
    R_PMA_HWA_ID_V4H_VIN16               ,
    R_PMA_HWA_ID_V4H_VIN15               ,
    R_PMA_HWA_ID_V4H_VIN14               ,
    R_PMA_HWA_ID_V4H_VIN13               ,
    R_PMA_HWA_ID_V4H_VIN12               ,
    R_PMA_HWA_ID_V4H_VIN11               ,
    R_PMA_HWA_ID_V4H_VIN10               ,
    R_PMA_HWA_ID_V4H_VIN07               ,
    R_PMA_HWA_ID_V4H_VIN06               ,
    R_PMA_HWA_ID_V4H_VIN05               ,
    R_PMA_HWA_ID_V4H_VIN04               ,
    R_PMA_HWA_ID_V4H_VIN03               ,
    R_PMA_HWA_ID_V4H_VIN02               ,
/*     R_PMA_HWA_ID_V4H_UCMT                , */
/*     R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1 , */
/*     R_PMA_HWA_ID_V4H_PFC3                , */
/*     R_PMA_HWA_ID_V4H_PFC2                , */
/*     R_PMA_HWA_ID_V4H_PFC1                , */
/*     R_PMA_HWA_ID_V4H_PFC0                , */
/*     R_PMA_HWA_ID_V4H_CMT3                , */
/*     R_PMA_HWA_ID_V4H_CMT2                , */
/*     R_PMA_HWA_ID_V4H_CMT1                , */
/*     R_PMA_HWA_ID_V4H_CMT0                , */
/*     R_PMA_HWA_ID_V4H_WDT1_WDT0           , */
    R_PMA_HWA_ID_V4H_WCRC3               ,
    R_PMA_HWA_ID_V4H_WCRC2               ,
    R_PMA_HWA_ID_V4H_WCRC1               ,
    R_PMA_HWA_ID_V4H_WCRC0               ,
    R_PMA_HWA_ID_V4H_VSPX1               ,
    R_PMA_HWA_ID_V4H_VSPX0               ,
/*     R_PMA_HWA_ID_V4H_RGXC                , */
/*     R_PMA_HWA_ID_V4H_RGXB                , */
/*     R_PMA_HWA_ID_V4H_SWDT1_SWDT0         , */
/*     R_PMA_HWA_ID_V4H_RGXPVC              , */
/*     R_PMA_HWA_ID_V4H_RGXPVDE             , */
/*     R_PMA_HWA_ID_V4H_PCIE11              , */
/*     R_PMA_HWA_ID_V4H_PCIE01              , */
/*     R_PMA_HWA_ID_V4H_INTAP11             , */
/*     R_PMA_HWA_ID_V4H_FRAY01              , */
/*     R_PMA_HWA_ID_V4H_AXVI11              , */
/*     R_PMA_HWA_ID_V4H_AULK4P              , */
/*     R_PMA_HWA_ID_V4H_AULK2P              , */
    R_PMA_HWA_ID_V4H_FCPVX1              ,
    R_PMA_HWA_ID_V4H_FCPVX0              ,
/*     R_PMA_HWA_ID_V4H_KCRC4               , */
/*     R_PMA_HWA_ID_V4H_FSO                 , */
/*     R_PMA_HWA_ID_V4H_CRC3                , */
/*     R_PMA_HWA_ID_V4H_CRC2                , */
/*     R_PMA_HWA_ID_V4H_CRC1                , */
/*     R_PMA_HWA_ID_V4H_CRC0                , */
/*     R_PMA_HWA_ID_V4H_ADVFS               , */
/*     R_PMA_HWA_ID_V4H_SCMT                , */
/*     R_PMA_HWA_ID_V4H_PFC3D3              , */
/*     R_PMA_HWA_ID_V4H_PFC3D2              , */
/*     R_PMA_HWA_ID_V4H_PFC3D1              , */
/*     R_PMA_HWA_ID_V4H_PFC2D3              , */
/*     R_PMA_HWA_ID_V4H_PFC2D2              , */
/*     R_PMA_HWA_ID_V4H_PFC2D1              , */
/*     R_PMA_HWA_ID_V4H_PFC1D3              , */
/*     R_PMA_HWA_ID_V4H_PFC1D2              , */
/*     R_PMA_HWA_ID_V4H_PFC1D1              , */
/*     R_PMA_HWA_ID_V4H_PFC0D3              , */
/*     R_PMA_HWA_ID_V4H_PFC0D2              , */
/*     R_PMA_HWA_ID_V4H_PFC0D1              , */
/*     R_PMA_HWA_ID_V4H_WWDT9_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT8_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT7_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT6_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT5_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT4_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT3_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT2_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT1_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT0_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_DBE                 , */
/*     R_PMA_HWA_ID_V4H_MTI                 , */
/*     R_PMA_HWA_ID_V4H_KCRC7               , */
/*     R_PMA_HWA_ID_V4H_KCRC6               , */
/*     R_PMA_HWA_ID_V4H_KCRC5               , */
/*     R_PMA_HWA_ID_V4H_CKMMM               , */
/*     R_PMA_HWA_ID_V4H_CKMIR               , */
/*     R_PMA_HWA_ID_V4H_CKMVIP              , */
/*     R_PMA_HWA_ID_V4H_CKMVC               , */
/*     R_PMA_HWA_ID_V4H_CKMVIO              , */
/*     R_PMA_HWA_ID_V4H_CKMPE0              , */
/*     R_PMA_HWA_ID_V4H_CKMRT               , */
/*     R_PMA_HWA_ID_V4H_CKM3DG              , */
/*     R_PMA_HWA_ID_V4H_APVI4_1             , */
/*     R_PMA_HWA_ID_V4H_APVI4_0             , */
/*     R_PMA_HWA_ID_V4H_APVC1_1             , */
/*     R_PMA_HWA_ID_V4H_APVC1_0             , */
/*     R_PMA_HWA_ID_V4H_CKMDSP_CKMIR        , */
/*     R_PMA_HWA_ID_V4H_CKMCNR_CKMIR        , */
/*     R_PMA_HWA_ID_V4H_CKM                 , */
/*     R_PMA_HWA_ID_V4H_CKMHSC              , */
/*     R_PMA_HWA_ID_V4H_CKMCR52             , */
/*     R_PMA_HWA_ID_V4H_ANHC                , */
/*     R_PMA_HWA_ID_V4H_AXHC_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXHC                , */
/*     R_PMA_HWA_ID_V4H_CSBRG_IR_A2         , */
/*     R_PMA_HWA_ID_V4H_CSBRG_IR_A3         , */
/*     R_PMA_HWA_ID_V4H_TSN                 , */
    R_PMA_HWA_ID_V4H_CVE3                ,
    R_PMA_HWA_ID_V4H_CVE2                ,
    R_PMA_HWA_ID_V4H_CVE1                ,
    R_PMA_HWA_ID_V4H_CVE0                ,
    R_PMA_HWA_ID_V4H_VDSP1_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP1_REG           ,
    R_PMA_HWA_ID_V4H_VDSP0_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP0_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP0_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP0_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP0_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP0_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP0_REG           ,
    R_PMA_HWA_ID_V4H_VDSP0_CSB           ,
    R_PMA_HWA_ID_V4H_DSC                 ,
/*     R_PMA_HWA_ID_V4H_FCPRC               , */
    R_PMA_HWA_ID_V4H_PAPSDMA             ,
    R_PMA_HWA_ID_V4H_HSM                 ,
    R_PMA_HWA_ID_V4H_PAPBUS              ,
    R_PMA_HWA_ID_V4H_VDSP3_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP2_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP1_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP0_BUS           ,
    R_PMA_HWA_ID_V4H_SSI                 ,
/*     R_PMA_HWA_ID_V4H_SSIU                , */
    R_PMA_HWA_ID_V4H_VDSP3_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP3_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP3_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP3_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP3_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP3_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP3_REG           ,
    R_PMA_HWA_ID_V4H_VDSP3_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP2_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP2_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP2_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP2_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP2_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP2_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP2_REG           ,
    R_PMA_HWA_ID_V4H_VDSP2_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP1_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP1_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP1_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP1_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP1_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP1_GLOBAL        ,
/*     R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR      , */
    R_PMA_HWA_ID_V4H_REG_SPMC            ,
    R_PMA_HWA_ID_V4H_DOF_FCPRM           ,
    R_PMA_HWA_ID_V4H_DCM                 ,
    R_PMA_HWA_ID_V4H_CBF                 ,
    R_PMA_HWA_ID_V4H_SBF                 ,
    R_PMA_HWA_ID_V4H_OSM                 ,
    R_PMA_HWA_ID_V4H_ISM0                ,
    R_PMA_HWA_ID_V4H_ISM1                ,
    R_PMA_HWA_ID_V4H_FBS                 ,
    R_PMA_HWA_ID_V4H_APB                 ,
    R_PMA_HWA_ID_V4H_DUL                 ,
    R_PMA_HWA_ID_V4H_SPMC01              ,
    R_PMA_HWA_ID_V4H_SPMI                ,
    R_PMA_HWA_ID_V4H_REG_SPMI            ,
    R_PMA_HWA_ID_V4H_NUM
};

static e_pma_hwa_id_t g_pma_test_reset_hwa_id_reverse[] = {
    /* V4H */
    R_PMA_HWA_ID_V4H_SPMI                ,
    R_PMA_HWA_ID_V4H_REG_SPMI            ,
    R_PMA_HWA_ID_V4H_IMPPSC              ,
    R_PMA_HWA_ID_V4H_IMPDMAC0            ,
    R_PMA_HWA_ID_V4H_IMP1                ,
    R_PMA_HWA_ID_V4H_IMP0                ,
    R_PMA_HWA_ID_V4H_SPMC                ,
    R_PMA_HWA_ID_V4H_IMPCNN              ,
    R_PMA_HWA_ID_V4H_ISP1                ,
    R_PMA_HWA_ID_V4H_ISP0                ,
/*     R_PMA_HWA_ID_V4H_ANRT1               , */
    R_PMA_HWA_ID_V4H_DOF                 ,
    R_PMA_HWA_ID_V4H_SMPS0               ,
    R_PMA_HWA_ID_V4H_SMPO0               ,
    R_PMA_HWA_ID_V4H_RGX                 ,
/*     R_PMA_HWA_ID_V4H_ANVI1               , */
/*     R_PMA_HWA_ID_V4H_ANVI0               , */
/*     R_PMA_HWA_ID_V4H_ANVC1               , */
/*     R_PMA_HWA_ID_V4H_ANVC0               , */
/*     R_PMA_HWA_ID_V4H_ANSP0               , */
/*     R_PMA_HWA_ID_V4H_ANRT                , */
/*     R_PMA_HWA_ID_V4H_ANPV0               , */
/*     R_PMA_HWA_ID_V4H_ADVFSC              , */
/*     R_PMA_HWA_ID_V4H_ADG                 , */
    R_PMA_HWA_ID_V4H_IMPSLV              ,
    R_PMA_HWA_ID_V4H_IMPDTA              ,
/*     R_PMA_HWA_ID_V4H_AXIMP0_AXRS         , */
/*     R_PMA_HWA_ID_V4H_AXIMP0              , */
/*     R_PMA_HWA_ID_V4H_ANIMP1              , */
/*     R_PMA_HWA_ID_V4H_ANIMP0              , */
    R_PMA_HWA_ID_V4H_IMPDMAC1            ,
    R_PMA_HWA_ID_V4H_IMP3                ,
    R_PMA_HWA_ID_V4H_IMP2                ,
/*     R_PMA_HWA_ID_V4H_AXSM_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXSD0_1             , */
/*     R_PMA_HWA_ID_V4H_AXSD0_0             , */
/*     R_PMA_HWA_ID_V4H_AXSC                , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSC           , */
/*     R_PMA_HWA_ID_V4H_AXRT_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXRT                , */
/*     R_PMA_HWA_ID_V4H_AXRD_1              , */
/*     R_PMA_HWA_ID_V4H_AXRD_0              , */
/*     R_PMA_HWA_ID_V4H_AXRC_1              , */
/*     R_PMA_HWA_ID_V4H_AXRC_0              , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXPV           , */
/*     R_PMA_HWA_ID_V4H_AXPV                , */
/*     R_PMA_HWA_ID_V4H_ANMM_AXMM           , */
/*     R_PMA_HWA_ID_V4H_AVB2                , */
/*     R_PMA_HWA_ID_V4H_AVB1                , */
/*     R_PMA_HWA_ID_V4H_AVB0                , */
/*     R_PMA_HWA_ID_V4H_ARMREG              , */
/*     R_PMA_HWA_ID_V4H_ARMM                , */
/*     R_PMA_HWA_ID_V4H_APSP3_1             , */
/*     R_PMA_HWA_ID_V4H_APSP3_0             , */
/*     R_PMA_HWA_ID_V4H_APSP4_1             , */
/*     R_PMA_HWA_ID_V4H_APSP4_0             , */
/*     R_PMA_HWA_ID_V4H_APRT0_1             , */
/*     R_PMA_HWA_ID_V4H_APRT0_0             , */
/*     R_PMA_HWA_ID_V4H_ANVIP2              , */
/*     R_PMA_HWA_ID_V4H_ANVIP1              , */
/*     R_PMA_HWA_ID_V4H_ANVIP0              , */
    R_PMA_HWA_ID_V4H_CSITOP0             ,
/*     R_PMA_HWA_ID_V4H_CSDBGPAP            , */
/*     R_PMA_HWA_ID_V4H_CR0                 , */
/*     R_PMA_HWA_ID_V4H_CANFD               , */
/*     R_PMA_HWA_ID_V4H_AXVIP1_1            , */
/*     R_PMA_HWA_ID_V4H_AXVIP1_0            , */
/*     R_PMA_HWA_ID_V4H_AXVIP0_AXRS         , */
/*     R_PMA_HWA_ID_V4H_AXVIP0              , */
/*     R_PMA_HWA_ID_V4H_AXVIFF              , */
/*     R_PMA_HWA_ID_V4H_AXVI1               , */
/*     R_PMA_HWA_ID_V4H_AXVI1_AXRS          , */
/*     R_PMA_HWA_ID_V4H_AXVI                , */
/*     R_PMA_HWA_ID_V4H_AXVC_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXVC                , */
/*     R_PMA_HWA_ID_V4H_AXTBH               , */
/*     R_PMA_HWA_ID_V4H_AXSTM               , */
/*     R_PMA_HWA_ID_V4H_AXSTM_AXRS          , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSP0          , */
/*     R_PMA_HWA_ID_V4H_AXSP0               , */
/*     R_PMA_HWA_ID_V4H_AXSN                , */
/*     R_PMA_HWA_ID_V4H_AXRS_AXSN           , */
/*     R_PMA_HWA_ID_V4H_AXSM                , */
    R_PMA_HWA_ID_V4H_DSITXLINK1          ,
    R_PMA_HWA_ID_V4H_DSITXLINK0          ,
    R_PMA_HWA_ID_V4H_DOC2CH              ,
    R_PMA_HWA_ID_V4H_DIS0                ,
/*     R_PMA_HWA_ID_V4H_DDR3_DDR2           , */
/*     R_PMA_HWA_ID_V4H_DDR1_DDR0           , */
    R_PMA_HWA_ID_V4H_CSITOP1             ,
/*     R_PMA_HWA_ID_V4H_INTAP               , */
    R_PMA_HWA_ID_V4H_IMS1                ,
    R_PMA_HWA_ID_V4H_IMS0                ,
    R_PMA_HWA_ID_V4H_IMR2                ,
    R_PMA_HWA_ID_V4H_IMR1                ,
    R_PMA_HWA_ID_V4H_IMR0                ,
/*     R_PMA_HWA_ID_V4H_I2C5                , */
/*     R_PMA_HWA_ID_V4H_I2C4                , */
/*     R_PMA_HWA_ID_V4H_I2C3                , */
/*     R_PMA_HWA_ID_V4H_I2C2                , */
/*     R_PMA_HWA_ID_V4H_I2C1                , */
/*     R_PMA_HWA_ID_V4H_I2C0                , */
/*     R_PMA_HWA_ID_V4H_HSCIF3              , */
/*     R_PMA_HWA_ID_V4H_HSCIF2              , */
/*     R_PMA_HWA_ID_V4H_HSCIF1              , */
/*     R_PMA_HWA_ID_V4H_HSCIF0              , */
/*     R_PMA_HWA_ID_V4H_FRAY00              , */
/*     R_PMA_HWA_ID_V4H_FCPVD1              , */
/*     R_PMA_HWA_ID_V4H_FCPVD0              , */
    R_PMA_HWA_ID_V4H_FCPCS               ,
/*     R_PMA_HWA_ID_V4H_RTDM1               , */
/*     R_PMA_HWA_ID_V4H_RTDM0               , */
/*     R_PMA_HWA_ID_V4H_RPC                 , */
/*     R_PMA_HWA_ID_V4H_PWM                 , */
/*     R_PMA_HWA_ID_V4H_PCIE10              , */
/*     R_PMA_HWA_ID_V4H_PCIE00              , */
/*     R_PMA_HWA_ID_V4H_MSI5                , */
/*     R_PMA_HWA_ID_V4H_MSI4                , */
/*     R_PMA_HWA_ID_V4H_MSI3                , */
/*     R_PMA_HWA_ID_V4H_MSI2                , */
/*     R_PMA_HWA_ID_V4H_MSI1                , */
/*     R_PMA_HWA_ID_V4H_MSI0                , */
    R_PMA_HWA_ID_V4H_IVCP1E              ,
    R_PMA_HWA_ID_V4H_ISPCS1              ,
    R_PMA_HWA_ID_V4H_ISPCS0              ,
/*     R_PMA_HWA_ID_V4H_IRQC                , */
/*     R_PMA_HWA_ID_V4H_INTTP               , */
/*     R_PMA_HWA_ID_V4H_IPC                 , */
    R_PMA_HWA_ID_V4H_VIN01               ,
    R_PMA_HWA_ID_V4H_VIN00               ,
    R_PMA_HWA_ID_V4H_VCPL4               ,
    R_PMA_HWA_ID_V4H_AES_ACC7            ,
    R_PMA_HWA_ID_V4H_AES_ACC6            ,
    R_PMA_HWA_ID_V4H_AES_ACC5            ,
    R_PMA_HWA_ID_V4H_AES_ACC4            ,
    R_PMA_HWA_ID_V4H_AES_ACC3            ,
    R_PMA_HWA_ID_V4H_AES_ACC2            ,
    R_PMA_HWA_ID_V4H_AES_ACC1            ,
    R_PMA_HWA_ID_V4H_AES_ACC0            ,
    R_PMA_HWA_ID_V4H_AES_ACC_WRAP        ,
/*     R_PMA_HWA_ID_V4H_TPU0                , */
/*     R_PMA_HWA_ID_V4H_TMU4                , */
/*     R_PMA_HWA_ID_V4H_TMU3                , */
/*     R_PMA_HWA_ID_V4H_TMU2                , */
/*     R_PMA_HWA_ID_V4H_TMU1                , */
/*     R_PMA_HWA_ID_V4H_TMU0                , */
/*     R_PMA_HWA_ID_V4H_SYDM2               , */
/*     R_PMA_HWA_ID_V4H_SYDM1               , */
/*     R_PMA_HWA_ID_V4H_SECROM              , */
/*     R_PMA_HWA_ID_V4H_SDHI0               , */
/*     R_PMA_HWA_ID_V4H_SCIF4               , */
/*     R_PMA_HWA_ID_V4H_SCIF3               , */
/*     R_PMA_HWA_ID_V4H_SCIF1               , */
/*     R_PMA_HWA_ID_V4H_SCIF0               , */
/*     R_PMA_HWA_ID_V4H_RTDM3               , */
/*     R_PMA_HWA_ID_V4H_RTDM2               , */
    R_PMA_HWA_ID_V4H_VSPD1               ,
    R_PMA_HWA_ID_V4H_VSPD0               ,
    R_PMA_HWA_ID_V4H_VIN17               ,
    R_PMA_HWA_ID_V4H_VIN16               ,
    R_PMA_HWA_ID_V4H_VIN15               ,
    R_PMA_HWA_ID_V4H_VIN14               ,
    R_PMA_HWA_ID_V4H_VIN13               ,
    R_PMA_HWA_ID_V4H_VIN12               ,
    R_PMA_HWA_ID_V4H_VIN11               ,
    R_PMA_HWA_ID_V4H_VIN10               ,
    R_PMA_HWA_ID_V4H_VIN07               ,
    R_PMA_HWA_ID_V4H_VIN06               ,
    R_PMA_HWA_ID_V4H_VIN05               ,
    R_PMA_HWA_ID_V4H_VIN04               ,
    R_PMA_HWA_ID_V4H_VIN03               ,
    R_PMA_HWA_ID_V4H_VIN02               ,
/*     R_PMA_HWA_ID_V4H_UCMT                , */
/*     R_PMA_HWA_ID_V4H_TSC4_TSC3_TSC2_TSC1 , */
/*     R_PMA_HWA_ID_V4H_PFC3                , */
/*     R_PMA_HWA_ID_V4H_PFC2                , */
/*     R_PMA_HWA_ID_V4H_PFC1                , */
/*     R_PMA_HWA_ID_V4H_PFC0                , */
/*     R_PMA_HWA_ID_V4H_CMT3                , */
/*     R_PMA_HWA_ID_V4H_CMT2                , */
/*     R_PMA_HWA_ID_V4H_CMT1                , */
/*     R_PMA_HWA_ID_V4H_CMT0                , */
/*     R_PMA_HWA_ID_V4H_WDT1_WDT0           , */
    R_PMA_HWA_ID_V4H_WCRC3               ,
    R_PMA_HWA_ID_V4H_WCRC2               ,
    R_PMA_HWA_ID_V4H_WCRC1               ,
    R_PMA_HWA_ID_V4H_WCRC0               ,
    R_PMA_HWA_ID_V4H_VSPX1               ,
    R_PMA_HWA_ID_V4H_VSPX0               ,
/*     R_PMA_HWA_ID_V4H_RGXC                , */
/*     R_PMA_HWA_ID_V4H_RGXB                , */
/*     R_PMA_HWA_ID_V4H_SWDT1_SWDT0         , */
/*     R_PMA_HWA_ID_V4H_RGXPVC              , */
/*     R_PMA_HWA_ID_V4H_RGXPVDE             , */
/*     R_PMA_HWA_ID_V4H_PCIE11              , */
/*     R_PMA_HWA_ID_V4H_PCIE01              , */
/*     R_PMA_HWA_ID_V4H_INTAP11             , */
/*     R_PMA_HWA_ID_V4H_FRAY01              , */
/*     R_PMA_HWA_ID_V4H_AXVI11              , */
/*     R_PMA_HWA_ID_V4H_AULK4P              , */
/*     R_PMA_HWA_ID_V4H_AULK2P              , */
    R_PMA_HWA_ID_V4H_FCPVX1              ,
    R_PMA_HWA_ID_V4H_FCPVX0              ,
/*     R_PMA_HWA_ID_V4H_KCRC4               , */
/*     R_PMA_HWA_ID_V4H_FSO                 , */
/*     R_PMA_HWA_ID_V4H_CRC3                , */
/*     R_PMA_HWA_ID_V4H_CRC2                , */
/*     R_PMA_HWA_ID_V4H_CRC1                , */
/*     R_PMA_HWA_ID_V4H_CRC0                , */
/*     R_PMA_HWA_ID_V4H_ADVFS               , */
/*     R_PMA_HWA_ID_V4H_SCMT                , */
/*     R_PMA_HWA_ID_V4H_PFC3D3              , */
/*     R_PMA_HWA_ID_V4H_PFC3D2              , */
/*     R_PMA_HWA_ID_V4H_PFC3D1              , */
/*     R_PMA_HWA_ID_V4H_PFC2D3              , */
/*     R_PMA_HWA_ID_V4H_PFC2D2              , */
/*     R_PMA_HWA_ID_V4H_PFC2D1              , */
/*     R_PMA_HWA_ID_V4H_PFC1D3              , */
/*     R_PMA_HWA_ID_V4H_PFC1D2              , */
/*     R_PMA_HWA_ID_V4H_PFC1D1              , */
/*     R_PMA_HWA_ID_V4H_PFC0D3              , */
/*     R_PMA_HWA_ID_V4H_PFC0D2              , */
/*     R_PMA_HWA_ID_V4H_PFC0D1              , */
/*     R_PMA_HWA_ID_V4H_WWDT9_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT8_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT7_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT6_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT5_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT4_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT3_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT2_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT1_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT0_RCLK          , */
/*     R_PMA_HWA_ID_V4H_WWDT9_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT8_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT7_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT6_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT5_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT4_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT3_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT2_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT1_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_WWDT0_SASYNCRT      , */
/*     R_PMA_HWA_ID_V4H_DBE                 , */
/*     R_PMA_HWA_ID_V4H_MTI                 , */
/*     R_PMA_HWA_ID_V4H_KCRC7               , */
/*     R_PMA_HWA_ID_V4H_KCRC6               , */
/*     R_PMA_HWA_ID_V4H_KCRC5               , */
/*     R_PMA_HWA_ID_V4H_CKMMM               , */
/*     R_PMA_HWA_ID_V4H_CKMIR               , */
/*     R_PMA_HWA_ID_V4H_CKMVIP              , */
/*     R_PMA_HWA_ID_V4H_CKMVC               , */
/*     R_PMA_HWA_ID_V4H_CKMVIO              , */
/*     R_PMA_HWA_ID_V4H_CKMPE0              , */
/*     R_PMA_HWA_ID_V4H_CKMRT               , */
/*     R_PMA_HWA_ID_V4H_CKM3DG              , */
/*     R_PMA_HWA_ID_V4H_APVI4_1             , */
/*     R_PMA_HWA_ID_V4H_APVI4_0             , */
/*     R_PMA_HWA_ID_V4H_APVC1_1             , */
/*     R_PMA_HWA_ID_V4H_APVC1_0             , */
/*     R_PMA_HWA_ID_V4H_CKMDSP_CKMIR        , */
/*     R_PMA_HWA_ID_V4H_CKMCNR_CKMIR        , */
/*     R_PMA_HWA_ID_V4H_CKM                 , */
/*     R_PMA_HWA_ID_V4H_CKMHSC              , */
/*     R_PMA_HWA_ID_V4H_CKMCR52             , */
/*     R_PMA_HWA_ID_V4H_ANHC                , */
/*     R_PMA_HWA_ID_V4H_AXHC_AXRS           , */
/*     R_PMA_HWA_ID_V4H_AXHC                , */
/*     R_PMA_HWA_ID_V4H_CSBRG_IR_A2         , */
/*     R_PMA_HWA_ID_V4H_CSBRG_IR_A3         , */
/*     R_PMA_HWA_ID_V4H_TSN                 , */
    R_PMA_HWA_ID_V4H_CVE3                ,
    R_PMA_HWA_ID_V4H_CVE2                ,
    R_PMA_HWA_ID_V4H_CVE1                ,
    R_PMA_HWA_ID_V4H_CVE0                ,
    R_PMA_HWA_ID_V4H_VDSP1_REG           ,
    R_PMA_HWA_ID_V4H_VDSP1_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP0_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP0_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP0_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP0_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP0_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP0_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP0_REG           ,
    R_PMA_HWA_ID_V4H_VDSP0_CSB           ,
    R_PMA_HWA_ID_V4H_DSC                 ,
/*     R_PMA_HWA_ID_V4H_FCPRC               , */
    R_PMA_HWA_ID_V4H_PAPSDMA             ,
    R_PMA_HWA_ID_V4H_HSM                 ,
    R_PMA_HWA_ID_V4H_PAPBUS              ,
    R_PMA_HWA_ID_V4H_VDSP3_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP2_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP1_BUS           ,
    R_PMA_HWA_ID_V4H_VDSP0_BUS           ,
    R_PMA_HWA_ID_V4H_SSI                 ,
/*     R_PMA_HWA_ID_V4H_SSIU                , */
    R_PMA_HWA_ID_V4H_VDSP3_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP3_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP3_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP3_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP3_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP3_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP3_REG           ,
    R_PMA_HWA_ID_V4H_VDSP3_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP2_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP2_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP2_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP2_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP2_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP2_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP2_REG           ,
    R_PMA_HWA_ID_V4H_VDSP2_CSB           ,
    R_PMA_HWA_ID_V4H_VDSP1_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP1_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP1_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP1_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP1_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP1_GLOBAL        ,
    R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR      ,
    R_PMA_HWA_ID_V4H_REG_SPMC            ,
    R_PMA_HWA_ID_V4H_DOF_FCPRM           ,
    R_PMA_HWA_ID_V4H_DCM                 ,
    R_PMA_HWA_ID_V4H_CBF                 ,
    R_PMA_HWA_ID_V4H_SBF                 ,
    R_PMA_HWA_ID_V4H_OSM                 ,
    R_PMA_HWA_ID_V4H_ISM0                ,
    R_PMA_HWA_ID_V4H_ISM1                ,
    R_PMA_HWA_ID_V4H_FBS                 ,
    R_PMA_HWA_ID_V4H_APB                 ,
    R_PMA_HWA_ID_V4H_DUL                 ,
    R_PMA_HWA_ID_V4H_SPMC01              ,
    R_PMA_HWA_ID_V4H_IMPSDMAC1           ,
    R_PMA_HWA_ID_V4H_IMPSDMAC0           ,
    R_PMA_HWA_ID_V4H_NUM
};
static e_pma_hwa_id_t g_pma_vsdp_no_dep_test_hwa_id[] = {
    /* V4H */
    R_PMA_HWA_ID_V4H_VDSP1_REG           ,
    R_PMA_HWA_ID_V4H_VDSP0_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP0_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP0_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP0_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP0_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP0_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP0_REG           ,
    R_PMA_HWA_ID_V4H_VDSP3_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP3_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP3_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP3_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP3_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP3_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP3_REG           ,
    R_PMA_HWA_ID_V4H_VDSP2_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP2_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP2_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP2_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP2_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP2_GLOBAL        ,
    R_PMA_HWA_ID_V4H_VDSP2_REG           ,
    R_PMA_HWA_ID_V4H_VDSP1_SYSPO         ,
    R_PMA_HWA_ID_V4H_VDSP1_APBDBG        ,
    R_PMA_HWA_ID_V4H_VDSP1_OCEM          ,
    R_PMA_HWA_ID_V4H_VDSP1_CORE          ,
    R_PMA_HWA_ID_V4H_VDSP1_SYS           ,
    R_PMA_HWA_ID_V4H_VDSP1_GLOBAL        ,

};
#endif  /* R_PMA_FUNCTIONAL_TEST_ID_TBL_V4H_H */
