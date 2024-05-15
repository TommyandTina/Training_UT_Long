#include <stdio.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
//#include <aarch64/r-car-gen3.h>
#include <hw/inout.h>
//#include <hw/cpg.h>
//#include "../../lib/cpg/public/hw/cpg.h"
//#include <hw/sysc.h>
//#include "../../lib/sysc/public/hw/sysc.h"

#include "../../../../../../../../bsp/qnx/adas/gen3/src/src/hardware/startup/lib/public/aarch64/r-car-gen3.h"

#include "../../../../../../../src/include/target/common/pma/r_pma.h"

#define STR(var) #var


typedef struct
{
    unsigned int parent_policy;        /* Power Domain Register Number */
    unsigned int parent_state;          /* Parent Power Domain Register Number */
    unsigned int child_policy;        /* Power Domain Register Number */
    unsigned int child_state;          /* Parent Power Domain Register Number */
    unsigned int action;               /* Parent Power Domain Register Number */
    unsigned int parent_sysc;          /* Parent Power Domain Register Number */
    unsigned int parent_cpg;           /* Parent Power Domain Register Number */
    unsigned int child_sysc;           /* Parent Power Domain Register Number */
    unsigned int child_cpg;            /* Parent Power Domain Register Number */
    unsigned int result_parent_sysc;           /* Parent Power Domain Register Number */
    unsigned int result_parent_cpg;           /* Parent Power Domain Register Number */
    unsigned int result_child_sysc;           /* Parent Power Domain Register Number */
    unsigned int result_child_cpg;           /* Parent Power Domain Register Number */
    unsigned int do_flg;
} st_state_test_info_t;
#define HP      0
#define CG      1
#define PG      2
#define Release 0
#define Request 1

#include "test_pattern.c"

//typedef struct
//{
//    int PDR_NUM;                  /* Power Domain Register Number */
//    int Parent_PDR_NUM;           /* Parent Power Domain Register Number */
//    CPG_MOD_ID CPG_ID;            /* CPG Module ID */
//    char PD_Name[10];             /* Power Domain Name */
//} st_sysc_cpg_local_info_t;
//
//static const st_sysc_cpg_local_info_t R_PMA_PDRNUM_CPGID_Tbl_local[R_PMA_HWA_ID_NUM] =
//{
//    /*PDR_NUM, Parent PDR_NUM,  CPG_ID, PD_Name*/
//
//    /*IMP*/
//    {34, 43, IMP0, "A2IMP01"},          //    R_PMA_HWA_ID_IMP_CORE0,       /*!< IMP0 */
//    {34, 43, IMP1, "A2IMP01"},          //    R_PMA_HWA_ID_IMP_CORE1,       /*!< IMP1 */
//    {46, 43, IMP2, "A2IMP23"},          //    R_PMA_HWA_ID_IMP_CORE2,       /*!< IMP2 */
//    {46, 43, IMP3, "A2IMP23"},          //    R_PMA_HWA_ID_IMP_CORE3,       /*!< IMP3 */
//    {36, 43, OCV0, "A2CV0"},            //    R_PMA_HWA_ID_IMP_OCV_CORE0,   /*!< IMP-OCV0 */
//    {37, 43, OCV1, "A2CV1"},            //    R_PMA_HWA_ID_IMP_OCV_CORE1,   /*!< IMP-OCV1 */
//    {48, 43, OCV2, "A2CV2"},            //    R_PMA_HWA_ID_IMP_OCV_CORE2,   /*!< IMP-OCV2 */
//    {49, 43, OCV3, "A2CV3"},            //    R_PMA_HWA_ID_IMP_OCV_CORE3,   /*!< IMP-OCV3 */
//    {38, 43, OCV4, "A2CV4"},            //    R_PMA_HWA_ID_IMP_OCV_CORE4,   /*!< IMP-OCV4 */
//    {50, 43, OCV5, "A2CV5"},            //    R_PMA_HWA_ID_IMP_OCV_CORE5,   /*!< IMP-OCV5 */
//    {39, 43, OCV6, "A2CV6"},            //    R_PMA_HWA_ID_IMP_OCV_CORE6,   /*!< IMP-OCV6 */
//    {51, 43, OCV7, "A2CV7"},            //    R_PMA_HWA_ID_IMP_OCV_CORE7,   /*!< IMP-OCV7 */
//    {35, 43, IMPDMA0,"A2DP0"},          //    R_PMA_HWA_ID_IMP_DMAC0,       /*!< IMP DMA0 */
//    {47, 43, IMPDMA1, "A2DP1"},         //    R_PMA_HWA_ID_IMP_DMAC1,       /*!< IMP DMA1 */
//    {35, 43, IMPPSC0, "A2DP0"},         //    R_PMA_HWA_ID_IMP_PSC0,        /*!< IMP PSC0 */
//    {47, 43, IMPPSC1, "A2DP1"},         //    R_PMA_HWA_ID_IMP_PSC1,        /*!< IMP PSC1 */
//    {41, 42, IMPCNN0, "A1CNN0"},        //    R_PMA_HWA_ID_IMP_CNN0,        /*!< IMP CNN0 */
//    {44, 52, IMPCNN1, "A1CNN1"},        //    R_PMA_HWA_ID_IMP_CNN1,        /*!< IMP CNN1 */
//    {32, 40, IMPCNN2, "A1CNN2"},        //    R_PMA_HWA_ID_IMP_CNN2,        /*!< IMP CNN2 */
//    {43, -1, SPMI0, "A3IR"},            //    R_PMA_HWA_ID_IMP_SPMI0,       /*!< IMP RAM0 */
//    {43, -1, SPMI1, "A3IR"},            //    R_PMA_HWA_ID_IMP_SPMI1,       /*!< IMP RAM1 */
//    {42, 43, SPMC0, "A2CN0"},           //    R_PMA_HWA_ID_IMP_SPMC0,       /*!< IMP CNN RAM0 */
//    {52, 43, SPMC1, "A2CN1"},           //    R_PMA_HWA_ID_IMP_SPMC1,       /*!< IMP CNN RAM1 */
//    {40, 43, SPMC2, "A2CN2"},           //    R_PMA_HWA_ID_IMP_SPMC2,       /*!< IMP CNN RAM2 */
//
//    /*Vision IP*/
//    {57, -1, UMFL0, "A3VIP1"},          //    R_PMA_HWA_ID_VIP_UMFL0,       /*!< UMFL0  (DOF) */
//    {59, -1, UMFL1, "A3VIP3"},          //    R_PMA_HWA_ID_VIP_UMFL1,       /*!< UMFL1  (DOF) */
//    {56, -1, DISP0_0, "A3VIP0"},        //    R_PMA_HWA_ID_VIP_DISP0,       /*!< DISP0 (STV) */
//    {56, -1, DISP0_1, "A3VIP0"},        //    R_PMA_HWA_ID_VIP_DISP1,       /*!< DISP1 (STV) */
//    {58, -1, CLE0, "A3VIP2"},           //    R_PMA_HWA_ID_VIP_ACF0,        /*!< ACF0(CLE0) */
//    {58, -1, CLE1, "A3VIP2"},           //    R_PMA_HWA_ID_VIP_ACF1,        /*!< ACF1(CLE1) */
//    {58, -1, CLE2, "A3VIP2"},           //    R_PMA_HWA_ID_VIP_ACF2,        /*!< ACF2(CLE2) */
//    {58, -1, CLE3, "A3VIP2"},           //    R_PMA_HWA_ID_VIP_ACF3,        /*!< ACF3(CLE3) */
//
//    /*iVCPE*//* Always Power ON */
//    {-1, -1, IV1ES, "NONE"},            //    R_PMA_HWA_ID_VCP_IVCP1E,      /*!< iVCP1E */
//
//    /*ISP*/
//    {60, -1, ISP0, "A3ISP01"},          //    R_PMA_HWA_ID_ISP_CORE0,       /*!< ISP Channel 0 */
//    {60, -1, ISP1, "A3ISP01"},          //    R_PMA_HWA_ID_ISP_CORE1,       /*!< ISP Channel 1 */
//    {61, -1, ISP2, "A3ISP23"},          //    R_PMA_HWA_ID_ISP_CORE2,       /*!< ISP Channel 2 */
//    {61, -1, ISP3, "A3ISP23"},          //    R_PMA_HWA_ID_ISP_CORE3,       /*!< ISP Channel 3 */
//
//    /*IMR*//* Always Power ON */
//    {-1, -1, IMR0, "NONE"},             //    R_PMA_HWA_ID_IMR_LX4_CH0,     /*!< IMR-LX4 ch0 */
//    {-1, -1, IMR1, "NONE"},             //    R_PMA_HWA_ID_IMR_LX4_CH1,     /*!< IMR-LX4 ch1 */
//    {-1, -1, IMR2, "NONE"},             //    R_PMA_HWA_ID_IMR_LX4_CH2,     /*!< IMR-LX4 ch2 */
//    {-1, -1, IMR3, "NONE"},             //    R_PMA_HWA_ID_IMR_LX4_CH3,     /*!< IMR-LX4 ch3 */
//    {-1, -1, IMR4, "NONE"},             //    R_PMA_HWA_ID_IMR_LX4_CH4,     /*!< IMR-LX4 ch4 */
//    {-1, -1, IMR5, "NONE"},             //    R_PMA_HWA_ID_IMR_LX4_CH5,     /*!< IMR-LX4 ch5 */
//};

uintptr_t sysc_vbase = (uintptr_t)MAP_FAILED;
extern e_pma_hwa_id_t hwa_idmax;
extern e_pma_hwa_id_t hwa_idmin;
extern e_pma_hwa_id_t hwa_idimpr;

//extern uintptr_t sysc_vbase;

#if defined(RCAR_V3M2)
#define GETSTAA3IR  if(R_PMA_HWA_ID_V3M_IMP_RAM!=i)R_PMA_GetLockHwa(PMAhandle, R_PMA_HWA_ID_V3M_IMP_RAM);\
                    getsta(PMAhandle,R_PMA_HWA_ID_V3M_IMP_RAM);\
                    if(R_PMA_HWA_ID_V3M_IMP_RAM!=i)R_PMA_UnlockHwa(PMAhandle, R_PMA_HWA_ID_V3M_IMP_RAM);
#elif (defined(RCAR_V3H2) || defined(RCAR_V3H1))
#define GETSTAA3IR  if(R_PMA_HWA_ID_IMP_RAM!=i)R_PMA_GetLockHwa(PMAhandle, R_PMA_HWA_ID_IMP_RAM);\
                    getsta(PMAhandle,R_PMA_HWA_ID_IMP_RAM);\
                    if(R_PMA_HWA_ID_IMP_RAM!=i)R_PMA_UnlockHwa(PMAhandle, R_PMA_HWA_ID_IMP_RAM);
#else
#define GETSTAA3IR
#endif
void getsta(st_pma_handle_t handle, e_pma_hwa_id_t  hwaId)
{
    st_pma_hwa_state_t state;
    e_pma_return_t pma_ret = R_PMA_GetHwaState(handle, hwaId, &state);
    char* st;
    char* pwr;
    char* clk;

    if (state.enReqState == R_PMA_HWA_RELEASED) {
        st = "R_PMA_HWA_RELEASED ";
    }
    else if (state.enReqState == R_PMA_HWA_REQUESTED) {
        st = "R_PMA_HWA_REQUESTED";
    }
    else {
        st = "?";
    }

    if (state.enActualPwrStat == R_PMA_STATE_OFF) {
        pwr = "R_PMA_STATE_OFF";
    }
    else if (state.enActualPwrStat == R_PMA_STATE_ON) {
        pwr = "R_PMA_STATE_ON ";
    }
    else {
        pwr = "?";
    }

    if (state.enActualClkStat == R_PMA_STATE_OFF) {
        clk = "R_PMA_STATE_OFF";
    }
    else if (state.enActualClkStat == R_PMA_STATE_ON) {
        clk = "R_PMA_STATE_ON ";
    }
    else {
        clk = "?";
    }

    if(R_PMA_RESULT_OK == pma_ret)
    {
//        printf("HWID %02d:R_PMA_GetHwaState OK\n",hwaId);
//        printf("       enReqState      :%d\n",state.enReqState);
//        printf("       enActualPwrStat :%d\n",state.enActualPwrStat);
//        printf("       enActualClkStat :%d\n",state.enActualClkStat);
        printf("          enReqState: %s enActualPwrStat: %s enActualClkStat: %s\n", st, pwr , clk);
    }
    else
    {
        printf("HWID %02d:R_PMA_GetHwaState NG\n",hwaId);
    }
    fflush(NULL);
}
void getrst(st_pma_handle_t handle, e_pma_hwa_id_t  hwaId)
{
    e_pma_reset_req_t state;
    e_pma_return_t pma_ret = R_PMA_GetReset(handle, hwaId, &state);
    char* st;

    if (state == R_PMA_RELEASED_RESET) {
        st = "R_PMA_RELEASED_RESET";
    }
    else if (state == R_PMA_APPLIED_RESET) {
        st = "R_PMA_APPLIED_RESET ";
    }
    else {
        st = "?";
    }

    if(R_PMA_RESULT_OK == pma_ret)
    {
//        printf("HWID %02d:R_PMA_GetReset OK\n",hwaId);
        printf("          enRstStat:%s\n",st);
    }
    else
    {
        printf("HWID %02d:R_PMA_GetReset NG\n",hwaId);
    }
    fflush(NULL);
}
void getlps(st_pma_handle_t handle, e_pma_hwa_id_t  hwaId)
{
    e_pma_power_state_t state;
    e_pma_return_t pma_ret = R_PMA_GetLowestPowerState(handle, hwaId, &state);
    char* st;

    if (state == R_PMA_STATE_PG) {
        st = "R_PMA_STATE_PG     ";
    }
    else if (state == R_PMA_STATE_CG) {
        st = "R_PMA_STATE_CG     ";
    }
    else if (state == R_PMA_STATE_ENABLED) {
        st = "R_PMA_STATE_ENABLED";
    }
    else {
        st = "?";
    }


    if(R_PMA_RESULT_OK == pma_ret)
    {
//        printf("HWID %02d:R_PMA_GetLowestPowerState OK\n",hwaId);
//        printf("       enLowestPwrState:%d\n",state);
        printf("          enLowestPwrState:%s\n",st);
    }
    else
    {
        printf("HWID %02d:R_PMA_GetLowestPowerState NG\n",hwaId);
    }
    fflush(NULL);
}
void getpol(st_pma_handle_t handle, e_pma_hwa_id_t  hwaId)
{
    e_pma_power_policy_t state;
    e_pma_return_t pma_ret = R_PMA_GetPowerPolicy(handle, hwaId, &state);
    char* mode;
 
    if (state == R_PMA_PG_MODE) {
        mode = "R_PMA_PG_MODE";
    }
    else if (state == R_PMA_CG_MODE)
    {
        mode = "R_PMA_CG_MODE";
    }
    else if (state == R_PMA_HP_MODE)
    {
        mode = "R_PMA_HP_MODE";
    }
    else {
        mode = "?";
    }


    if(R_PMA_RESULT_OK == pma_ret)
    {
//        printf("HWID %02d:R_PMA_GetPowerPolicy OK\n",hwaId);
        printf("          enPwrPolicy     :%s\n", mode);
    }
    else
    {
        printf("HWID %02d:R_PMA_GetPowerPolicy NG\n",hwaId);
    }
    fflush(NULL);
}
uintptr_t cpg_vbase = (uintptr_t)MAP_FAILED;
#define RCAR_GEN3_CPG_SCMSTPCR0          0xB20       // Secure Module Stop Control Register 0
#define RCAR_GEN3_CPG_SCMSTPCR1          0xB24       // Secure Module Stop Control Register 1
#define RCAR_GEN3_CPG_SCMSTPCR2          0xB28       // Secure Module Stop Control Register 2
#define RCAR_GEN3_CPG_SCMSTPCR3          0xB2C       // Secure Module Stop Control Register 3
#define RCAR_GEN3_CPG_SCMSTPCR4          0xB20       // Secure Module Stop Control Register 4
#define RCAR_GEN3_CPG_SCMSTPCR5          0xB34       // Secure Module Stop Control Register 5
#define RCAR_GEN3_CPG_SCMSTPCR6          0xB38       // Secure Module Stop Control Register 6
#define RCAR_GEN3_CPG_SCMSTPCR7          0xB3C       // Secure Module Stop Control Register 7
#define RCAR_GEN3_CPG_SCMSTPCR8          0xB40       // Secure Module Stop Control Register 8
#define RCAR_GEN3_CPG_SCMSTPCR9          0xB44       // Secure Module Stop Control Register 9
#define RCAR_GEN3_CPG_SCMSTPCR10         0xB48       // Secure Module Stop Control Register 10
#define RCAR_GEN3_CPG_SCMSTPCR11         0xB4C       // Secure Module Stop Control Register 11

#if defined(RCAR_V3M2)
    e_pma_hwa_id_t parent_hwa = R_PMA_HWA_ID_V3M_IMP_TOP;
    e_pma_hwa_id_t child_hwa = R_PMA_HWA_ID_V3M_IMP_CORE0;
    #define HWA_NUM    11
    e_pma_hwa_id_t hwa_list[HWA_NUM] = {
    R_PMA_HWA_ID_V3M_IMP_TOP,
    R_PMA_HWA_ID_V3M_IMP_CORE0,
    R_PMA_HWA_ID_V3M_IMP_CORE2,
    R_PMA_HWA_ID_V3M_IMP_OCV_CORE0,
    R_PMA_HWA_ID_V3M_IMP_OCV_CORE1,
    R_PMA_HWA_ID_V3M_IMP_DMA,
    R_PMA_HWA_ID_V3M_IMP_CNN,
    R_PMA_HWA_ID_V3M_IMP_CORE1,
    R_PMA_HWA_ID_V3M_IMP_CORE3,
    R_PMA_HWA_ID_V3M_IMP_PSC,
    R_PMA_HWA_ID_V3M_IMP_SPMI
    };

#elif defined(RCAR_V3H2)
    e_pma_hwa_id_t parent_hwa = R_PMA_HWA_ID_V3H2_IMP_TOP;
    e_pma_hwa_id_t child_hwa = R_PMA_HWA_ID_V3H2_IMP_CORE0;
    #define HWA_NUM    18
    e_pma_hwa_id_t hwa_list[HWA_NUM] = {
    R_PMA_HWA_ID_V3H2_IMP_TOP,
    R_PMA_HWA_ID_V3H2_IMP_CORE0,
    R_PMA_HWA_ID_V3H2_IMP_CORE1,
    R_PMA_HWA_ID_V3H2_IMP_CORE2,
    R_PMA_HWA_ID_V3H2_IMP_CORE3,
    R_PMA_HWA_ID_V3H2_IMP_CORE4,
    R_PMA_HWA_ID_V3H2_SIMP_CORE,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE0,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE1,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE2,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE3,
    R_PMA_HWA_ID_V3H2_IMP_OCV_CORE4,
    R_PMA_HWA_ID_V3H2_IMP_DMAC0,
    R_PMA_HWA_ID_V3H2_IMP_DMAC1,
    R_PMA_HWA_ID_V3H2_IMP_PSC,
    R_PMA_HWA_ID_V3H2_SIMP_DMA,
    R_PMA_HWA_ID_V3H2_IMP_CNN,
    R_PMA_HWA_ID_V3H2_IMP_SPMI
    };

#elif defined(RCAR_V3H1)
    e_pma_hwa_id_t parent_hwa = R_PMA_HWA_ID_V3H1_IMP_TOP;
    e_pma_hwa_id_t child_hwa = R_PMA_HWA_ID_V3H1_IMP_CORE0;
    #define HWA_NUM    18
    e_pma_hwa_id_t hwa_list[HWA_NUM] = {
    R_PMA_HWA_ID_V3H1_IMP_TOP,
    R_PMA_HWA_ID_V3H1_IMP_CORE0,
    R_PMA_HWA_ID_V3H1_IMP_CORE1,
    R_PMA_HWA_ID_V3H1_IMP_CORE2,
    R_PMA_HWA_ID_V3H1_IMP_CORE3,
    R_PMA_HWA_ID_V3H1_IMP_CORE4,
    R_PMA_HWA_ID_V3H1_SIMP_CORE,
    R_PMA_HWA_ID_V3H1_IMP_OCV_CORE0,
    R_PMA_HWA_ID_V3H1_IMP_OCV_CORE1,
    R_PMA_HWA_ID_V3H1_IMP_OCV_CORE2,
    R_PMA_HWA_ID_V3H1_IMP_OCV_CORE3,
    R_PMA_HWA_ID_V3H1_IMP_OCV_CORE4,
    R_PMA_HWA_ID_V3H1_IMP_DMAC0,
    R_PMA_HWA_ID_V3H1_IMP_DMAC1,
    R_PMA_HWA_ID_V3H1_IMP_PSC0,
    R_PMA_HWA_ID_V3H1_IMP_PSC1,
    R_PMA_HWA_ID_V3H1_IMP_CNN,
    R_PMA_HWA_ID_V3H1_IMP_SPMI
    };

#else
    printf("Not supported SoC\n");
    e_pma_hwa_id_t parent_hwa = R_PMA_HWA_ID_IMP_RAM;
    e_pma_hwa_id_t child_hwa = R_PMA_HWA_ID_IMP_CORE0;
    #define HWA_NUM    1
    e_pma_hwa_id_t hwa_list[HWA_NUM] = {
        R_PMA_HWA_ID_IMP_RAM;
    };
#endif

typedef struct
{
    uintptr_t MSTPSR;    /* MSTPSR offset address */
    uintptr_t SMSTPCR;   /* SMSTPCR offset address */
    uintptr_t SRCR;      /* SRCR offset address */
    uintptr_t SRSTCLR;   /* SRSTCLR offset address */
    uintptr_t RMSTPCR;
    uintptr_t SCMSTPCR;
    uint32_t  BitNum;    /* CPG register's bit number */
    uint32_t  RegNum;    /* MSTP register number */
} st_cpg_mstp_reg_info_t;

#if defined(RCAR_V3M2)
static const st_cpg_mstp_reg_info_t R_PMA_CPG_GstRegTbl[] =
{
    /* MSTPSR,                SMSTPCR,                 SRCR,                 SRSTCLR,                 RMSTPCR,                SCMSTPCR,               BitNum */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 27u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP0        */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 26u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP1        */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 29u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_OCV0    */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 28u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_OCV1    */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 25u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP2        */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 24u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP3        */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 30u, 8u  }   /* R_PMA_CPG_DOMAIN_ID_IMP_RAM     */
};
#elif (defined(RCAR_V3H2) || defined(RCAR_V3H1))
static const st_cpg_mstp_reg_info_t R_PMA_CPG_GstRegTbl[] =
{
    /* MSTPSR,                SMSTPCR,                 SRCR,                 SRSTCLR,                 RMSTPCR,                SCMSTPCR,               BitNum */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 27u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP0        */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 26u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP1        */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 25u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP2        */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 24u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP3        */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5, 21u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_IMP4        */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5,  0u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_SIMP        */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 29u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_OCV0    */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 28u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_OCV1    */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5, 31u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_OCV2    */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5, 29u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_OCV3    */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5, 28u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_OCV4    */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5, 27u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_DMA0    */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5, 26u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_DMA1    */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5, 25u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_PSC0    */
    { RCAR_GEN3_CPG_MSTPSR5,  RCAR_GEN3_CPG_SMSTPCR5,  RCAR_GEN3_CPG_SRCR5,  RCAR_GEN3_CPG_SRSTCLR5,  RCAR_GEN3_CPG_RMSTPCR5, RCAR_GEN3_CPG_SCMSTPCR5, 24u, 5u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_PSC1    */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 31u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_CNN     */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 30u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMP_RAM     */
    { RCAR_GEN3_CPG_MSTPSR1,  RCAR_GEN3_CPG_SMSTPCR1,  RCAR_GEN3_CPG_SRCR1,  RCAR_GEN3_CPG_SRSTCLR1,  RCAR_GEN3_CPG_RMSTPCR1, RCAR_GEN3_CPG_SCMSTPCR1,  2u, 1u  },  /* R_PMA_CPG_DOMAIN_ID_VIP_DOF     */
    { RCAR_GEN3_CPG_MSTPSR1,  RCAR_GEN3_CPG_SMSTPCR1,  RCAR_GEN3_CPG_SRCR1,  RCAR_GEN3_CPG_SRSTCLR1,  RCAR_GEN3_CPG_RMSTPCR1, RCAR_GEN3_CPG_SCMSTPCR1,  1u, 1u  },  /* R_PMA_CPG_DOMAIN_ID_VIP_STV     */
    { RCAR_GEN3_CPG_MSTPSR10, RCAR_GEN3_CPG_SMSTPCR10, RCAR_GEN3_CPG_SRCR10, RCAR_GEN3_CPG_SRSTCLR10, RCAR_GEN3_CPG_RMSTPCR10,RCAR_GEN3_CPG_SCMSTPCR10, 4u, 10u },  /* R_PMA_CPG_DOMAIN_ID_VIP_ACF0    */
    { RCAR_GEN3_CPG_MSTPSR10, RCAR_GEN3_CPG_SMSTPCR10, RCAR_GEN3_CPG_SRCR10, RCAR_GEN3_CPG_SRSTCLR10, RCAR_GEN3_CPG_RMSTPCR10,RCAR_GEN3_CPG_SCMSTPCR10, 3u, 10u },  /* R_PMA_CPG_DOMAIN_ID_VIP_ACF1    */
    { RCAR_GEN3_CPG_MSTPSR10, RCAR_GEN3_CPG_SMSTPCR10, RCAR_GEN3_CPG_SRCR10, RCAR_GEN3_CPG_SRSTCLR10, RCAR_GEN3_CPG_RMSTPCR10,RCAR_GEN3_CPG_SCMSTPCR10, 2u, 10u },  /* R_PMA_CPG_DOMAIN_ID_VIP_ACF2    */
    { RCAR_GEN3_CPG_MSTPSR10, RCAR_GEN3_CPG_SMSTPCR10, RCAR_GEN3_CPG_SRCR10, RCAR_GEN3_CPG_SRSTCLR10, RCAR_GEN3_CPG_RMSTPCR10,RCAR_GEN3_CPG_SCMSTPCR10, 1u, 10u },  /* R_PMA_CPG_DOMAIN_ID_VIP_ACF3    */
    { RCAR_GEN3_CPG_MSTPSR10, RCAR_GEN3_CPG_SMSTPCR10, RCAR_GEN3_CPG_SRCR10, RCAR_GEN3_CPG_SRSTCLR10, RCAR_GEN3_CPG_RMSTPCR10,RCAR_GEN3_CPG_SCMSTPCR10, 0u, 10u },  /* R_PMA_CPG_DOMAIN_ID_VIP_ACF4    */
    { RCAR_GEN3_CPG_MSTPSR1,  RCAR_GEN3_CPG_SMSTPCR1,  RCAR_GEN3_CPG_SRCR1,  RCAR_GEN3_CPG_SRSTCLR1,  RCAR_GEN3_CPG_RMSTPCR1, RCAR_GEN3_CPG_SCMSTPCR1, 27u, 1u  },  /* R_PMA_CPG_DOMAIN_ID_VCP_IVCP1E  */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 17u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_ISP_CH0     */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 14u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_ISP_CH1     */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 23u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH0 */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 22u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH1 */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 21u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH2 */
    { RCAR_GEN3_CPG_MSTPSR8,  RCAR_GEN3_CPG_SMSTPCR8,  RCAR_GEN3_CPG_SRCR8,  RCAR_GEN3_CPG_SRSTCLR8,  RCAR_GEN3_CPG_RMSTPCR8, RCAR_GEN3_CPG_SCMSTPCR8, 20u, 8u  },  /* R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH3 */
    { RCAR_GEN3_CPG_MSTPSR7,  RCAR_GEN3_CPG_SMSTPCR7,  RCAR_GEN3_CPG_SRCR7,  RCAR_GEN3_CPG_SRSTCLR7,  RCAR_GEN3_CPG_RMSTPCR7, RCAR_GEN3_CPG_SCMSTPCR7,  7u, 7u  },  /* R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH4 */
    { RCAR_GEN3_CPG_MSTPSR7,  RCAR_GEN3_CPG_SMSTPCR7,  RCAR_GEN3_CPG_SRCR7,  RCAR_GEN3_CPG_SRSTCLR7,  RCAR_GEN3_CPG_RMSTPCR7, RCAR_GEN3_CPG_SCMSTPCR7,  6u, 7u  },  /* R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH5 */
    { RCAR_GEN3_CPG_MSTPSR7,  RCAR_GEN3_CPG_SMSTPCR7,  RCAR_GEN3_CPG_SRCR7,  RCAR_GEN3_CPG_SRSTCLR7,  RCAR_GEN3_CPG_RMSTPCR7, RCAR_GEN3_CPG_SCMSTPCR7,  6u, 7u  }   /* R_PMA_CPG_DOMAIN_ID_IMR_LX4_CH5 */
};
#endif
void printmstpcr(e_pma_hwa_id_t  i)
{
    uint32_t    reg_valuer  = 0;
    uint32_t    reg_values  = 0;
    uint32_t    reg_valuesc = 0;
    uint32_t    bit_no;
    uint32_t    reg_no;
    uint32_t    offset;

    if((uintptr_t)MAP_FAILED == cpg_vbase)
    {
        printf("no mapping cpg.\n");
        return;
    }

    bit_no = R_PMA_CPG_GstRegTbl[i].BitNum;
    reg_no = R_PMA_CPG_GstRegTbl[i].RegNum;
    offset = R_PMA_CPG_GstRegTbl[i].RMSTPCR;
    reg_valuer  = (in32(cpg_vbase + offset) >> bit_no) & 0x1;

    offset = R_PMA_CPG_GstRegTbl[i].SMSTPCR;
    reg_values  = (in32(cpg_vbase + offset) >> bit_no) & 0x1;

    offset = R_PMA_CPG_GstRegTbl[i].MSTPSR;
    reg_valuesc = (in32(cpg_vbase + offset) >> bit_no) & 0x1;
    printf("HWID %02d:MSTPCR(R:S:SR)%02d(bit%02d) value %d:%d:%d\n",i , reg_no, bit_no, reg_valuer,reg_values,reg_valuesc);
    fflush(NULL);
    if(parent_hwa>=i)
    {
        uint32_t  reg_valuesa = 0;
        offset = R_PMA_CPG_GstRegTbl[i].SCMSTPCR;
        reg_valuesc = (in32(cpg_vbase + offset) >> bit_no) & 0x1;

        offset =  0xC40;
        reg_valuesa = (in32(cpg_vbase + offset) >> bit_no) & 0x1;
        printf("HWID %02d:MSTPCR(R:S:SC:SA)%02d(bit%02d) value %d:%d:%d:%d\n",i , reg_no, bit_no, reg_valuer,reg_values,reg_valuesc,reg_valuesa);
    }

}
void writemstpcr(e_pma_hwa_id_t  i)
{
    uint32_t    reg_valuer  = 0;
    uint32_t    reg_values  = 0;
    uint32_t    reg_valuesc = 0;
    uint32_t    bit_no;
    uint32_t    reg_no;
    uint32_t    offset;

    if((uintptr_t)MAP_FAILED == cpg_vbase)
    {
        printf("no mapping cpg.\n");
        return;
    }

    bit_no = R_PMA_CPG_GstRegTbl[i].BitNum;
    reg_no = R_PMA_CPG_GstRegTbl[i].RegNum;
    offset = R_PMA_CPG_GstRegTbl[i].RMSTPCR;
    reg_valuer  =  in32(cpg_vbase + offset);
    reg_valuer  |= 0x1 << bit_no;
    out32(cpg_vbase + offset,reg_valuer);

    offset = R_PMA_CPG_GstRegTbl[i].SMSTPCR;
    reg_values  =  in32(cpg_vbase + offset);
    reg_values  |= 0x1 << bit_no;
    out32(cpg_vbase + offset,reg_values);

//    offset = R_PMA_CPG_GstRegTbl[i].SCMSTPCR;
//    reg_valuesc =  in32(cpg_vbase + offset);
//    reg_valuesc |= 0x1 << bit_no;
//    out32(cpg_vbase + offset,reg_valuesc);

    //printf("HWID %02d:MSTPCR(R:S:SC)%02d(bit%02d) value 0x%08x:0x%08x:0x%08x\n",i , reg_no, bit_no, reg_valuer,reg_values,reg_valuesc);
    fflush(NULL);
}

int main(void)
{
    //e_pma_return_t result;

    //uint32_t    sysc_reg_value = 0;
    //uint32_t    cpg_reg_value  = 0;
    //uint32_t    bit_no;
    //uint32_t    reg_no;



    //  .まずは全ドメインのOFFを確認
    //  PDRSRxx ⇒ 0x00000001       ※bit1のみ1

    //  .このタイミング（PMA_init前≒driverのinit前）ではdriverもメモリマッピングしていないため
    //  .独自にレジスタをマッピングする
//    cpg_vbase = (uintptr_t)mmap_device_io(RCAR_V3U_CPG_SIZE, RCAR_V3U_CPG_DM0_BASE);
//    if (cpg_vbase  == (uintptr_t)MAP_FAILED) {
//        printf("CPG - %s : Unable to map CPG registers 0x%x\n", __func__, RCAR_V3U_CPG_DM0_BASE);
//        return 0;
//    }
//
//    for(e_pma_hwa_id_t hwaid = hwa_idmin ; hwaid <  hwa_idmax ; hwaid++)
//    {
//        printmstpcr(hwaid);
//    }
//    sysc_vbase = (uintptr_t)mmap_device_io(RCAR_V3U_SYSC_SIZE, RCAR_V3U_SYSC_DM0_BASE);
//    if (sysc_vbase  == (uintptr_t)MAP_FAILED) {
//        printf("SYSC : Unable to map SYSC registers 0x%x\n", RCAR_V3U_SYSC_DM0_BASE);
//        return 0;
//    }

//    for(e_pma_hwa_id_t i = R_PMA_HWA_ID_IMP_CORE0 ; i <  R_PMA_HWA_ID_NUM ; i++)
//    {
//        if( -1 != R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM)
//        {
//            sysc_reg_value = in32(sysc_vbase + RCAR_V3U_SYSC_PDRSR(R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM));
//            printf("HWID %02d:PDRSR%02d:value 0x%02X      [Power %s]\n", i, R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM, sysc_reg_value, (sysc_reg_value & 0x10)?"ON":"OFF");
//        }
//        else
//        {
//            printf("HWID %02d no PDR Skip!!\n",i);
//        }
//        fflush(NULL);
//    
//    }

    // 参照用アドレスは自分で管理する。

    unsigned int loop, count, i;
    e_pma_return_t pma_ret;
    st_pma_handle_t PMAhandle;
    st_pma_handle_t *OsalPMAhandle;
    st_pma_config_t pma_config;
    e_pma_power_policy_t pma_policy;

    st_pma_hwa_state_t state;
    e_pma_reset_req_t reset_state;

    ///pma_config.polling_max_num = 10;
    ///pma_config.loop_num_for_rclk_cycle = 40;
    ///pma_config.timeout_ms = 10; //10msec を想定（暫定です）
    ///pma_config.task_priority = 1;
    pma_config.task_priority = 10;
    pma_config.timeout_ms = 500;
    pma_config.polling_max_num = 10;
    pma_config.loop_num_for_rclk_cycle = 3;

    e_pma_hwa_req_t parent_state;
    e_pma_hwa_req_t child_state;

#if 0
    uint32_t    reg_mstpcr8 = 0;

//#if defined(RCAR_V3M2)
    cpg_vbase = (uintptr_t)mmap_device_io(RCAR_V3U_CPG_SIZE, RCAR_V3U_CPG_DM0_BASE);
    if (cpg_vbase  == (uintptr_t)MAP_FAILED) {
        printf("CPG - %s : Unable to map CPG registers 0x%x\n", __func__, RCAR_V3U_CPG_DM0_BASE);
        return 0;
    }

    /* read register */
    reg_mstpcr8 = in32(cpg_vbase + RCAR_GEN3_CPG_RMSTPCR8);
    if((reg_mstpcr8 & 0x7ff00000 ) == 0x7ff00000 )
    {
        printf("RMSTPCR initial value OK!!\n");
    }
    else
    {
        printf("RMSTPCR initial value NG!!\n");

        printf("Set initial reg value\n");
        for(e_pma_hwa_id_t hwaid = child_hwa ; hwaid <=  parent_hwa ; hwaid++)
        {
            printf("id = %d\n",hwaid);
            printmstpcr(hwaid);
            writemstpcr(hwaid);
            printmstpcr(hwaid);
        }
    }
    munmap_device_io(cpg_vbase, RCAR_V3U_CPG_SIZE);
#endif
#if 1
    // Required for mmap
    // #include <hw/inout.h>
    // Required for register address
    // #include "r-car-gen3.h"
    uint32_t    reg_mstpcr8 = 0;
    uint32_t    reg_val = 0;

    cpg_vbase = (uintptr_t)mmap_device_io(RCAR_GEN3_CPG_SIZE, RCAR_GEN3_CPG_BASE);
    if (cpg_vbase  == (uintptr_t)MAP_FAILED) {
        printf("CPG - %s : Unable to map CPG registers 0x%x\n", __func__, RCAR_V3U_CPG_DM0_BASE);
        return 0;
    }

    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR0);
    printf("SRCR0  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR1);
    printf("SRCR1  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR2);
    printf("SRCR2  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR3);
    printf("SRCR3  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR4);
    printf("SRCR4  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR5);
    printf("SRCR5  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR6);
    printf("SRCR6  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR7);
    printf("SRCR7  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR8);
    printf("SRCR8  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR9);
    printf("SRCR9  : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR10);
    printf("SRCR10 : 0x%8x\n", reg_val);
    reg_val = in32(cpg_vbase + RCAR_GEN3_CPG_SRCR11);
    printf("SRCR11 : 0x%8x\n", reg_val);

    /* read register */
    reg_mstpcr8 = in32(cpg_vbase + RCAR_GEN3_CPG_RMSTPCR8);
    if((reg_mstpcr8 & 0x7ff00000 ) == 0x7ff00000 )
    {
        printf("RMSTPCR initial value OK!!\n");
    }
    else
    {
        printf("RMSTPCR initial value NG!!\n");

        printf("Set initial RMSTPCR8 value\n");
        out32(cpg_vbase + RCAR_GEN3_CPG_RMSTPCR8, reg_mstpcr8 | 0x7ff00000);
    }
    munmap_device_io(cpg_vbase, RCAR_GEN3_CPG_SIZE);

#endif

/* (0) OSAL initialize */
#if 0
    R_OSAL_Initialize();
#else
    R_OSAL_ClockTimeInitialize();
    R_OSAL_ThreadInitialize();
    R_OSAL_ThsyncDeinitialize();
    R_OSAL_MqInitialize();
#endif
//    OsalPMAhandle = R_OSAL_Internal_GetPmaHandle();
//    PMAhandle = *OsalPMAhandle;
    count = 0;

for (loop = 0;loop < 1;loop++) {

    
    pma_ret = R_PMA_Init(&PMAhandle, &pma_config);
    switch (pma_ret)
    {
        case R_PMA_RESULT_OK:
            printf("R_PMA_Init R_PMA_RESULT_OK\n");
            break;
        case R_PMA_ERR_RESULT_NG:
            printf("R_PMA_Init R_PMA_ERR_RESULT_NG\n");
            break;
        case R_PMA_ERR_INVALID_ARGUMENT:
            printf("R_PMA_Init R_PMA_ERR_INVALID_ARGUMENT\n");
            break;
        case R_PMA_ERR_NO_VACANT_HANDLE:
            printf("R_PMA_Init ERROR_STATE\n");
            break;
//        case R_PMA_ERR_RESET_TOGGLE_CHK_NG:
//            printf("R_PMA_Init ERROR_DEV\n");
            break;
        default:
            printf("R_PMA_Init ERROR_FAIL\n");
            break;
    }
    fflush(NULL);
    delay(100);
    
#if 0
#if defined(RCAR_V3H2)
    for(e_pma_hwa_id_t i = R_PMA_HWA_ID_IMP_CORE0 ; i <  R_PMA_HWA_ID_NUM ; i++)            //ALL
#elif defined(RCAR_V3M2)
    for(e_pma_hwa_id_t i = R_PMA_HWA_ID_V3M_IMP_CORE01 ; i <  R_PMA_HWA_ID_V3M_NUM ; i++)            //ALL
#endif
#endif
    /* HWA setting */
        for (int num = 0;num < HWA_NUM;num++) {
            e_pma_hwa_id_t target_hwa;
            target_hwa = hwa_list[num];

            (void)R_PMA_GetLockHwa(PMAhandle, target_hwa);
            (void)R_PMA_SetPowerPolicy(PMAhandle, target_hwa, R_PMA_PG_MODE);
            (void)R_PMA_RequestHwa(PMAhandle, target_hwa);
            (void)R_PMA_ReleaseHwa(PMAhandle, target_hwa);
            delay(2);
            (void)R_PMA_UnlockHwa(PMAhandle, target_hwa);
        }
        delay(5);        

        pma_ret = R_PMA_GetLockHwa(PMAhandle, parent_hwa);
        if (pma_ret != R_PMA_RESULT_OK) {
                printf("parent lock error\n",i);
        }

        pma_ret = R_PMA_GetLockHwa(PMAhandle, child_hwa);
        if (pma_ret != R_PMA_RESULT_OK) {
                printf("child lock error\n",i);        
        }
/*****************initial state check ******************************/
        pma_ret = R_PMA_GetHwaState(PMAhandle, parent_hwa, &state);
        if (pma_ret != R_PMA_RESULT_OK ) {
             printf("R_PMA_GetHwaState error\n");
        }
        else {
           if (state.enActualPwrStat == R_PMA_STATE_ON) {
               printf("parent SYSC ON   ");
           }
           else {
               printf("parent SYSC OFF  ");
           }  
           if (state.enActualClkStat == R_PMA_STATE_ON) {
                                printf(" CPG ON\n");
           }
           else {
                                printf(" CPG OFF\n");
           }
        }
        pma_ret = R_PMA_GetHwaState(PMAhandle, child_hwa, &state);
        if (pma_ret != R_PMA_RESULT_OK ) {
             printf("R_PMA_GetHwaState error\n");
        }
        else {
           if (state.enActualPwrStat == R_PMA_STATE_ON) {
               printf("child  SYSC ON   ");
           }
           else {
               printf("child  SYSC OFF  ");
           }
           if (state.enActualClkStat == R_PMA_STATE_ON) {
                                printf(" CPG ON\n");
           }
           else {
                                printf(" CPG OFF\n");
           }
        }

/**********************************************************/
         printf("Test Start ----- \n");        
for (i = 0;i < 360;i++) {
        
        printf("Test No: %d \n",i+1);        
  if (test_senario[i].do_flg == 1) {

/************** state and policy setting *************************/
//            getpol(PMAhandle, i);
        pma_ret = R_PMA_GetHwaState(PMAhandle, parent_hwa, &state);
        if (pma_ret != R_PMA_RESULT_OK ) {
             printf("R_PMA_GetHwaState error\n");
        }
        parent_state = state.enReqState;
        delay(100);
        pma_ret = R_PMA_GetHwaState(PMAhandle, child_hwa, &state);
        if (pma_ret != R_PMA_RESULT_OK ) {
             printf("R_PMA_GetHwaState error\n");
        }
        child_state = state.enReqState;
//typedef enum
//{
//    R_PMA_HWA_RELEASED,           /*!< HWA is not ready. */
//    R_PMA_HWA_REQUESTED           /*!< HWA is ready. */
//} e_pma_hwa_req_t;

        if (test_senario[i].parent_policy == CG) {
            printf("parent policy : CG",i);        
            pma_policy = R_PMA_CG_MODE;
        }
        else if (test_senario[i].parent_policy == PG) {
            printf("parent policy : PG",i);        
            pma_policy = R_PMA_PG_MODE;

        }
        else if (test_senario[i].parent_policy == HP) {
            printf("parent policy : HP",i);        
            pma_policy = R_PMA_HP_MODE;

        }
        else {
            pma_policy = R_PMA_CG_MODE;
            printf("error parent policy\n",i);        
        }

        pma_ret = R_PMA_SetPowerPolicy(PMAhandle, parent_hwa, pma_policy);

        if (pma_ret != R_PMA_RESULT_OK) {
                printf("parent set policy error\n",i);
        }
        delay(100);

        if (test_senario[i].child_policy == CG) {
            printf("        child policy : CG\n",i);        
            pma_policy = R_PMA_CG_MODE;
        }
        else if (test_senario[i].child_policy == PG) {
            printf("        child policy : PG\n",i);        
            pma_policy = R_PMA_PG_MODE;
        }
        else if (test_senario[i].child_policy == HP) {
            printf("        child policy : HP\n",i);        
            pma_policy = R_PMA_HP_MODE;
        }
        else {
            pma_policy = R_PMA_CG_MODE;
            printf("error child policy\n",i);
        }
        delay(100);
        pma_ret = R_PMA_SetPowerPolicy(PMAhandle, child_hwa, pma_policy);

        if (pma_ret != R_PMA_RESULT_OK) {
                printf("child set policy error\n",i);
        }
        delay(100);
            //getpol(PMAhandle, i);
        if (test_senario[i].parent_state == Release) {
            printf("parent state : Release",i);        
            if (parent_state == R_PMA_HWA_REQUESTED) {
                pma_ret = R_PMA_ReleaseHwa(PMAhandle, parent_hwa);
            }
        }
        else if (test_senario[i].parent_state == Request) {
            printf("parent state : Request",i);        
            if (parent_state == R_PMA_HWA_RELEASED) {
                pma_ret = R_PMA_RequestHwa(PMAhandle, parent_hwa);
            }
        }
        else {
            printf("error parent state param\n",i);        
            pma_ret = R_PMA_ReleaseHwa(PMAhandle, parent_hwa);
        }

        if (pma_ret != R_PMA_RESULT_OK) {
                printf("parent release/request error\n",i);
        }
        delay(100);

        if (test_senario[i].child_state == Release) {
            printf("    child state : Release\n",i);        
            if (child_state == R_PMA_HWA_REQUESTED) {
                pma_ret = R_PMA_ReleaseHwa(PMAhandle, child_hwa);
            }
        }
        else if (test_senario[i].child_state == Request) {
            printf("    child state : Request\n",i);        
            if (child_state == R_PMA_HWA_RELEASED) {
                pma_ret = R_PMA_RequestHwa(PMAhandle, child_hwa);
            }
        }
        else {
            printf("error child state param\n",i);
            pma_ret = R_PMA_ReleaseHwa(PMAhandle, child_hwa);
        }

        if (pma_ret != R_PMA_RESULT_OK) {
                printf("child release/request error\n",i);
        }
        delay(100);

 //           getsta(PMAhandle, i);GETSTAA3IR;
/********************* Action ******************************/
        switch (test_senario[i].action) {
            case 1:
                printf("parent request\n",i);
                pma_ret = R_PMA_RequestHwa(PMAhandle, parent_hwa);
                break;
            case 2:
                printf("parent release\n",i);
                pma_ret = R_PMA_ReleaseHwa(PMAhandle, parent_hwa);
                break;
            case 3:
                printf("child request\n",i);
                pma_ret = R_PMA_RequestHwa(PMAhandle, child_hwa);
                break;
            case 4:
                printf("child release\n",i);
                pma_ret = R_PMA_ReleaseHwa(PMAhandle, child_hwa);
                break;
            case 5:
                printf("parent policy PG\n",i);
                pma_ret = R_PMA_SetPowerPolicy(PMAhandle, parent_hwa, R_PMA_PG_MODE);
                break;
            case 6:
                printf("parent policy CG\n",i);
                pma_ret = R_PMA_SetPowerPolicy(PMAhandle, parent_hwa, R_PMA_CG_MODE);
                break;
            case 7:
                printf("parent policy HP\n",i);
                pma_ret = R_PMA_SetPowerPolicy(PMAhandle, parent_hwa, R_PMA_HP_MODE);
                break;
            case 8:
                printf("child policy PG\n",i);
                pma_ret = R_PMA_SetPowerPolicy(PMAhandle, child_hwa, R_PMA_PG_MODE);
                break;
            case 9:
                printf("child policy CG\n",i);
                pma_ret = R_PMA_SetPowerPolicy(PMAhandle, child_hwa, R_PMA_CG_MODE);
                break;
            case 10:
                printf("child policy HP\n",i);
                pma_ret = R_PMA_SetPowerPolicy(PMAhandle, child_hwa, R_PMA_HP_MODE);
                break;
            default:
                printf("action no error\n",i);
                break;
        }
        if (pma_ret != R_PMA_RESULT_OK) {
            printf("Action error \n",i);
        }

        delay(100);

/****************** check result ***********************/
        pma_ret = R_PMA_GetHwaState(PMAhandle, parent_hwa, &state);
        if (pma_ret != R_PMA_RESULT_OK ) {
             printf("R_PMA_GetHwaState error\n");
        }
        else {
            if (test_senario[i].result_parent_sysc) {
                if (state.enActualPwrStat == R_PMA_STATE_ON) {
                    printf("parent sysc ON :OK\n");
                }
                else {
                    count++;
                    printf("parent sysc OFF:NG expect ON\n");
                }
            }
            else {
                /*expect OFF*/
                if (state.enActualPwrStat == R_PMA_STATE_OFF) {
                    printf("parent sysc OFF:OK\n");
                }
                else {
                    count++;
                    printf("parent sysc ON :NG expect OFF\n");
                }
            }

            if (test_senario[i].result_parent_cpg) {
                if (state.enActualClkStat == R_PMA_STATE_ON) {
                    printf("parent CPG  ON :OK\n");
                }
                else {
                    count++;
                    printf("parent CPG  OFF:NG expect ON\n");
                }
            }
            else {
                /*expect OFF*/
                if (state.enActualClkStat == R_PMA_STATE_OFF) {
                    printf("parent CPG  OFF:OK\n");
                }
                else {
                    count++;
                    printf("parent CPG  ON :NG expect OFF\n");
                }
            }
        }
        delay(100);
        pma_ret = R_PMA_GetHwaState(PMAhandle, child_hwa, &state);
        if (pma_ret != R_PMA_RESULT_OK ) {
             printf("R_PMA_GetHwaState error\n");
        }
        else {
           if (test_senario[i].result_child_sysc) {
                if (state.enActualPwrStat == R_PMA_STATE_ON) {
                    printf("child  sysc ON :OK\n");
                }
                else {
                    count++;
                    printf("child  sysc OFF:NG expect ON\n");
                }
            }
            else {
                /*expect OFF*/
                if (state.enActualPwrStat == R_PMA_STATE_OFF) {
                    printf("child  sysc OFF:OK\n");
                }
                else {
                    count++;
                    printf("child  sysc ON :NG expect OFF\n");
                }
            }        

            if (test_senario[i].result_child_cpg) {
                if (state.enActualClkStat == R_PMA_STATE_ON) {
                    printf("child  CPG  ON :OK\n");
                }
                else {
                    count++;
                    printf("child  CPG  OFF:NG expect ON\n");
                }
            }
            else {
                /*expect OFF*/
                if (state.enActualClkStat == R_PMA_STATE_OFF) {
                    printf("child  CPG  OFF:OK\n");
                }
                else {
                    count++;
                    printf("child  CPG  ON :NG expect OFF\n");
                }
            }

        }
  }
  delay(100);
}/*fpr (i)*/

    delay(500);
    /*PMA Quit*/
    printf("R_PMA_Quit\n");fflush(NULL);
    pma_ret = R_PMA_Quit(PMAhandle);
        switch (pma_ret)
        {
            case R_PMA_RESULT_OK:
                printf("R_PMA_Quit OK\n");fflush(NULL);
                break;
            case R_PMA_ERR_INVALID_ARGUMENT:
                printf("R_PMA_Quit R_PMA_ERR_INVALID_ARGUMENT\n");fflush(NULL);
                break;
            default:
                printf("R_PMA_Quit ERROR_FAIL(%d)\n",pma_ret);fflush(NULL);
                break;
        }
    
    printf("Test Error count ( %d )\n",count);
}/* loop */

    printf("Test End ----- \n");

    unsigned int test_pass = 0;
    unsigned int test_fail = 0;
    unsigned int test_skip = 0;
    if (0 != count)
    {
        test_fail++;
    }
    else
    {
        test_pass++;
    }
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, test_skip);

    return 0;
}
