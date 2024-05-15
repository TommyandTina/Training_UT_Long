#include <stdio.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
//#include <aarch64/r-car-gen3.h>
//#include <hw/inout.h>
//#include <hw/cpg.h>
//#include "../../lib/cpg/public/hw/cpg.h"
//#include <hw/sysc.h>
//#include "../../lib/sysc/public/hw/sysc.h"


#include "../../../../../../../src/include/target/common/pma/r_pma.h"

#define STR(var) #var

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
//extern uintptr_t sysc_vbase;

#if defined(RCAR_V3M2)
#define GETSTAA3IR  if(R_PMA_HWA_ID_V3M_IMP_RAM!=i)R_PMA_GetLockHwa(PMAhandle, R_PMA_HWA_ID_V3M_IMP_RAM);\
                    getsta(PMAhandle,R_PMA_HWA_ID_V3M_IMP_RAM);\
                    if(R_PMA_HWA_ID_V3M_IMP_RAM!=i)R_PMA_UnlockHwa(PMAhandle, R_PMA_HWA_ID_V3M_IMP_RAM);
#elif defined(RCAR_V3H2)
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

int main(void)
{
    //e_pma_return_t result;

    //uint32_t    sysc_reg_value = 0;
    //uint32_t    cpg_reg_value  = 0;
    //uint32_t    bit_no;
    //uint32_t    reg_no;

    //uintptr_t cpg_vbase = (uintptr_t)MAP_FAILED;


    //  .まずは全ドメインのOFFを確認
    //  PDRSRxx ⇒ 0x00000001       ※bit1のみ1

    //  .このタイミング（PMA_init前≒driverのinit前）ではdriverもメモリマッピングしていないため
    //  .独自にレジスタをマッピングする
//    cpg_vbase = (uintptr_t)mmap_device_io(RCAR_V3U_CPG_SIZE, RCAR_V3U_CPG_DM0_BASE);
//    if (cpg_vbase  == (uintptr_t)MAP_FAILED) {
//        printf("CPG - %s : Unable to map CPG registers 0x%x\n", __func__, RCAR_V3U_CPG_DM0_BASE);
//        return 0;
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

    unsigned int loop, count;
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
#if 0
    R_OSAL_Initialize();
#else
    osal_clock_time_initialize();
    osal_thread_initialize();
    osal_threadsync_initialize();
    osal_mq_initialize();
    R_OSAL_DCFG_Init();
#endif
//    OsalPMAhandle = R_OSAL_Internal_GetPmaHandle();
//    PMAhandle = *OsalPMAhandle;

for (loop = 0;loop < 4;loop++) {

    if (loop == 0) {
        pma_policy = R_PMA_CG_MODE;
        printf("1st CG_MODE TEST\n");

    }
    else if (loop == 1) {
        pma_policy = R_PMA_PG_MODE;
        printf("PG_MODE TEST\n");
    }
    else if (loop == 2) {
        pma_policy = R_PMA_CG_MODE;
        printf("CG_MODE TEST\n");
    }
    else {
        pma_policy = R_PMA_HP_MODE;
        printf("HP MODE TEST\n");
    }
    
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

#if defined(RCAR_V3H2)
    for(e_pma_hwa_id_t i = R_PMA_HWA_ID_IMP_CORE0 ; i <  R_PMA_HWA_ID_NUM ; i++)            //ALL
#elif defined(RCAR_V3M2)
    for(e_pma_hwa_id_t i = R_PMA_HWA_ID_V3M_IMP_CORE01 ; i <  R_PMA_HWA_ID_V3M_NUM ; i++)            //ALL
#endif
    //for(e_pma_hwa_id_t i = R_PMA_HWA_ID_IMP_DMAC0 ; i <=  R_PMA_HWA_ID_IMP_PSC1 ; i++)      //A2DP0-1
    //for(e_pma_hwa_id_t i = R_PMA_HWA_ID_VIP_UMFL0 ; i <=  R_PMA_HWA_ID_VIP_ACF3 ; i++)      //A3VIP0-3
    //for(e_pma_hwa_id_t i = R_PMA_HWA_ID_ISP_CORE0 ; i <=  R_PMA_HWA_ID_ISP_CORE3 ; i++)     //A3ISP0-3
    {
        // 起動直後の電源・クロック状態をチェック
//        printf("\n*** HWID %02d POWER DOMAIN NAME %s ***\n", i, R_PMA_PDRNUM_CPGID_Tbl_local[i].PD_Name);
//        if( -1 != R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM)
//        {
//            //sysc_reg_value = in32(sysc_vbase + RCAR_V3U_SYSC_PDRSR(R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM));
//            printf("HWID %02d:PDRSR%02d:value 0x%02X      [Power %s]\n", i, R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM, sysc_reg_value, (sysc_reg_value & 0x10)?"ON":"OFF");
//        }
//        else    printf("HWID %02d no PDR Skip!!\n",i);
//        fflush(NULL);
//        reg_no = R_PMA_PDRNUM_CPGID_Tbl_local[i].CPG_ID / 100;
//        bit_no = R_PMA_PDRNUM_CPGID_Tbl_local[i].CPG_ID % 100;
//        //cpg_reg_value = (in32(cpg_vbase + RCAR_V3U_CPG_MSTPSR0 + reg_no*4) >> bit_no) & 0x1;
//        printf("HWID %02d:MSTPSR%02d(bit%02d) value %d [clock %s]\n",i , reg_no, bit_no, cpg_reg_value,cpg_reg_value?"OFF":"ON");
//        fflush(NULL);
        printf("HWID %02d ----------------------------------------------------------\n",i);

        pma_ret = R_PMA_GetLockHwa(PMAhandle, i);
        switch (pma_ret)
        {
            case R_PMA_RESULT_OK:
//                printf("HWID %02d:R_PMA_GetLockHwa OK\n",i);
                printf("          R_PMA_GetLockHwa OK\n",i);
                break;
            case R_PMA_ERR_RESULT_NG:
                printf("HWID %02d:R_PMA_GetLockHwa R_PMA_ERR_RESULT_NG\n",i);
                break;
            case R_PMA_ERR_INVALID_ARGUMENT:
                printf("HWID %02d:R_PMA_GetLockHwa R_PMA_ERR_INVALID_ARGUMENT\n",i);
                break;
            case R_PMA_ERR_NO_VACANT_HANDLE:
                printf("HWID %02d:R_PMA_GetLockHwa R_PMA_ERR_NO_VACANT_HANDLE\n",i);
                break;
//            case R_PMA_ERR_RESET_TOGGLE_CHK_NG:
//                printf("HWID %02d:R_PMA_GetLockHwa R_PMA_ERR_RESET_TOGGLE_CHK_NG\n",i);
//                break;
            default:
                printf("HWID %02d:R_PMA_GetLockHwa ERROR_FAIL(%d)\n",i,pma_ret);
                break;
        }
        fflush(NULL);

        if ((R_PMA_RESULT_OK == pma_ret) || (R_PMA_ERR_ALREADY_IN_USE == pma_ret))
        {
            getpol(PMAhandle, i);
            pma_ret = R_PMA_SetPowerPolicy(PMAhandle, i, pma_policy);
            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
//                    printf("HWID %02d:R_PMA_SetPowerPolicy OK\n",i);
                    printf("          R_PMA_SetPowerPolicy OK\n",i);
                    break;
                case R_PMA_ERR_INVALID_ARGUMENT:
                    printf("HWID %02d:R_PMA_SetPowerPolicy R_PMA_ERR_INVALID_ARGUMENT\n",i);
                    break;
                default:
                    printf("HWID %02d:R_PMA_SetPowerPolicy ERROR_FAIL(%d)\n",i,pma_ret);
                    break;
            }
            delay(500);
            getpol(PMAhandle, i);
            fflush(NULL);
        }
        if (loop == 0) {
            pma_ret = R_PMA_SetPowerPolicy(PMAhandle, i, R_PMA_HP_MODE);
            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
//                    printf("HWID %02d:R_PMA_SetPowerPolicy OK\n",i);
                    printf("          R_PMA_SetPowerPolicy OK\n",i);
                    break;
                case R_PMA_ERR_INVALID_ARGUMENT:
                    printf("HWID %02d:R_PMA_SetPowerPolicy R_PMA_ERR_INVALID_ARGUMENT\n",i);
                    break;
                default:
                    printf("HWID %02d:R_PMA_SetPowerPolicy ERROR_FAIL(%d)\n",i,pma_ret);
                    break;
            }
            count = 0;
            while  (count < 10) {

                delay(100);
                pma_ret = R_PMA_GetHwaState(PMAhandle, i, &state);
                if (pma_ret != R_PMA_RESULT_OK ) {
                    printf("R_PMA_GetHwaState error");
                }
                else {
                    if ((state.enActualPwrStat == R_PMA_STATE_ON) && (state.enActualClkStat == R_PMA_STATE_ON)) {
                        printf("changed\n");
                        count+=10;
                    }
                }
                count++;
            }
            getsta(PMAhandle, i);
            GETSTAA3IR;


        }
        if (R_PMA_RESULT_OK == pma_ret)
        {
            getsta(PMAhandle, i);GETSTAA3IR;
            pma_ret = R_PMA_RequestHwa(PMAhandle, i);
            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
//                    printf("HWID %02d:R_PMA_RequestHwa OK\n",i);
                    printf("          R_PMA_RequestHwa OK\n",i);
//                    //cpg_reg_value = (in32(cpg_vbase + RCAR_V3U_CPG_SRCR0 + reg_no*4) >> bit_no) & 0x1;
//                    printf("HWID %02d:SRCR%02d  (bit%02d) value %d [reset %s]\n",i , reg_no, bit_no, cpg_reg_value, cpg_reg_value?"APPLIED":"RELEASED");
//                    //cpg_reg_value = (in32(cpg_vbase + RCAR_V3U_CPG_MSTPSR0 + reg_no*4) >> bit_no) & 0x1;
//                    printf("HWID %02d:MSTPSR%02d(bit%02d) value %d [clock %s]\n",i , reg_no, bit_no, cpg_reg_value,cpg_reg_value?"OFF":"ON");
                    fflush(NULL);
                    break;
                case R_PMA_ERR_INVALID_ARGUMENT:
                    printf("HWID %02d:R_PMA_RequestHwa R_PMA_ERR_INVALID_ARGUMENT\n",i);
                    break;
//                case R_PMA_ERR_BUS_IF_CHK_NG:
//                    printf("HWID %02d:R_PMA_RequestHwa R_PMA_ERR_BUS_IF_CHK_NG\n",i);
//                    break;
                case R_PMA_ERR_INVALID_HANDLE:
                    printf("HWID %02d:R_PMA_RequestHwa R_PMA_ERR_INVALID_HANDLE\n",i);
                    break;
                case R_PMA_ERR_RESULT_NG:
                    printf("HWID %02d:R_PMA_RequestHwa R_PMA_ERR_RESULT_NG\n",i);
                    break;
                case R_PMA_ERR_TIMEOUT:
                    printf("HWID %02d:R_PMA_RequestHwa R_PMA_ERR_TIMEOUT\n",i);
                    break;
                default:
                    printf("HWID %02d:R_PMA_RequestHwa ERROR_FAIL(%d)\n",i,pma_ret);
                    break;
            }
            count = 0;
            while  (count < 10) {

                delay(500);
                pma_ret = R_PMA_GetHwaState(PMAhandle, i, &state);
                if (pma_ret != R_PMA_RESULT_OK ) {
                    printf("R_PMA_GetHwaState error");
                }
                else {
                    if ((state.enActualPwrStat == R_PMA_STATE_ON) && (state.enActualClkStat == R_PMA_STATE_ON)) {
                        printf("changed\n");
                        count+=10;
                    } 
                }
                count++;
            } 
            getsta(PMAhandle, i);GETSTAA3IR;
            fflush(NULL);

        }

        if (R_PMA_RESULT_OK == pma_ret)
        {
            getrst(PMAhandle, i);
            pma_ret = R_PMA_ApplyResetHwa(PMAhandle, i);
            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
//                    printf("HWID %02d:R_PMA_ApplyResetHwa OK\n",i);
                    printf("          R_PMA_ApplyResetHwa OK\n",i);
                    //cpg_reg_value = (in32(cpg_vbase + RCAR_V3U_CPG_SRCR0 + reg_no*4) >> bit_no) & 0x1;
                    //printf("HWID %02d:SRCR%02d  (bit%02d) value %d [reset %s]\n",i , reg_no, bit_no, cpg_reg_value, cpg_reg_value?"APPLIED":"RELEASED");
                    fflush(NULL);
                    break;
                case R_PMA_ERR_INVALID_ARGUMENT:
                    printf("HWID %02d:R_PMA_ApplyResetHwa R_PMA_ERR_INVALID_ARGUMENT\n",i);
                    break;
                default:
                    printf("HWID %02d:R_PMA_ApplyResetHwa ERROR_FAIL(%d)\n",i,pma_ret);
                    break;
            }
            count = 0;
            while  (count < 10) {
                
                delay(200);
                pma_ret = R_PMA_GetReset(PMAhandle, i, &reset_state);
                if (pma_ret != R_PMA_RESULT_OK ) {
                    printf("R_PMA_GetReset error");
                }
                else {
                    if (reset_state == R_PMA_APPLIED_RESET) {
                        printf("changed\n");
                        count+=10;
                    } 
                }
                count++;
            } 
            getsta(PMAhandle, i);GETSTAA3IR;
            getrst(PMAhandle, i);
            fflush(NULL);
        }
        if (R_PMA_RESULT_OK == pma_ret)
        {
            pma_ret = R_PMA_ReleaseResetHwa(PMAhandle, i);
            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
//                    printf("HWID %02d:R_PMA_ReleaseResetHwa OK\n",i);
                    printf("          R_PMA_ReleaseResetHwa OK\n",i);
                    //cpg_reg_value = (in32(cpg_vbase + RCAR_V3U_CPG_SRCR0 + reg_no*4) >> bit_no) & 0x1;
                    //printf("HWID %02d:SRCR%02d  (bit%02d) value %d [reset %s]\n",i , reg_no, bit_no, cpg_reg_value, cpg_reg_value?"APPLIED":"RELEASED");
                    fflush(NULL);
                    break;
                case R_PMA_ERR_INVALID_ARGUMENT:
                    printf("HWID %02d:R_PMA_ReleaseResetHwa R_PMA_ERR_INVALID_ARGUMENT\n",i);
                    break;
                default:
                    printf("HWID %02d:R_PMA_ReleaseResetHwa ERROR_FAIL(%d)\n",i,pma_ret);
                    break;
            }
            count = 0;
            while  (count < 10) {

                delay(200);
                pma_ret = R_PMA_GetReset(PMAhandle, i, &reset_state);
                if (pma_ret != R_PMA_RESULT_OK ) {
                    printf("R_PMA_GetReset error");
                }
                else {
                    if (reset_state == R_PMA_RELEASED_RESET) {
                        printf("changed\n");
                        count+=10;
                    }
                }
                count++;
            }
            getsta(PMAhandle, i);GETSTAA3IR;
            getrst(PMAhandle, i);
            fflush(NULL);
        }
        if (R_PMA_RESULT_OK == pma_ret)
        {
            pma_ret = R_PMA_ReleaseHwa(PMAhandle, i);
            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
//                    printf("HWID %02d:R_PMA_ReleaseHwa OK\n",i);
                    printf("          R_PMA_ReleaseHwa OK\n",i);
                    //cpg_reg_value = (in32(cpg_vbase + RCAR_V3U_CPG_MSTPSR0 + reg_no*4) >> bit_no) & 0x1;
//                    printf("HWID %02d:MSTPSR%02d(bit%02d) value %d [clock %s]\n",i , reg_no, bit_no, cpg_reg_value,cpg_reg_value?"OFF":"ON");
//                    if( -1 != R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM)
//                    {
//                        //sysc_reg_value = in32(sysc_vbase + RCAR_V3U_SYSC_PDRSR(R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM));
//                        printf("HWID %02d:PDRSR%02d:value 0x%02X      [Power %s]\n", i, R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM, sysc_reg_value, (sysc_reg_value & 0x10)?"ON":"OFF");
//                    }
//                    else    printf("HWID %02d no PDR Skip!!\n",i);
                    fflush(NULL);
                    break;
                case R_PMA_ERR_INVALID_ARGUMENT:
                    printf("HWID %02d:R_PMA_ReleaseHwa R_PMA_ERR_INVALID_ARGUMENT\n",i);fflush(NULL);
                    break;
                default:
                    printf("HWID %02d:R_PMA_ReleaseHwa ERROR_FAIL(%d)\n",i,pma_ret);fflush(NULL);
                    break;
            }
            delay(1000);
            getsta(PMAhandle, i);GETSTAA3IR;
            getlps(PMAhandle, i);
        }

        if (R_PMA_RESULT_OK == pma_ret)
        {
            pma_ret = R_PMA_UnlockHwa(PMAhandle, i);
            switch (pma_ret)
            {
                case R_PMA_RESULT_OK:
                    printf("HWID %02d:R_PMA_UnlockHwa OK\n",i);fflush(NULL);
                    break;
                case R_PMA_ERR_INVALID_ARGUMENT:
                    printf("HWID %02d:R_PMA_UnlockHwa R_PMA_ERR_INVALID_ARGUMENT\n",i);fflush(NULL);
                    break;
                default:
                    printf("HWID %02d:R_PMA_UnlockHwa ERROR_FAIL(%d)\n",i,pma_ret);
                    break;
            }
        }

    }
    delay(1000);
    if (R_PMA_RESULT_OK == pma_ret)
    {
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
    }
    delay(2000);
//    for(e_pma_hwa_id_t i = R_PMA_HWA_ID_IMP_CORE0 ; i <  R_PMA_HWA_ID_NUM ; i++)
//    {
//        if( -1 != R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM)
//        {
//            //sysc_reg_value = in32(sysc_vbase + RCAR_V3U_SYSC_PDRSR(R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM));
//            printf("HWID %02d:PDRSR%02d:value 0x%02X      [Power %s]\n", i, R_PMA_PDRNUM_CPGID_Tbl_local[i].PDR_NUM, sysc_reg_value, (sysc_reg_value & 0x10)?"ON":"OFF");
//        }
//        else
//        {
//            printf("HWID %02d no PDR Skip!!\n",i);
//        }
//        fflush(NULL);
    
//    }
}
    printf("test end\n");
    return 0;
}
