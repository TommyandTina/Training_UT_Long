#include "r_pma_functionl_test_fw.h"
#include "r_pma_functionl_test_sequence.h"
#if defined(RCAR_V4H)
#include "r_pma_functionl_test_sequence_pattern_v4h.h"
#include "r_pma_functionl_test_id_tbl_v4h.h"
#elif defined(RCAR_V4M)
#include "r_pma_functionl_test_sequence_pattern_v4m.h"
#include "r_pma_functionl_test_id_tbl_v4m.h"
#elif defined(RCAR_S4)
#include "r_pma_functionl_test_sequence_pattern_s4.h"
#include "r_pma_functionl_test_id_tbl_s4.h"
#endif
/**********************************************************************************
  Private type definition
**********************************************************************************/
typedef int (*p_test_func_t)(e_pma_hwa_id_t hwa_id);

/**********************************************************************************
  Private function definition
**********************************************************************************/
static int execute_hwa_sequence(e_pma_hwa_id_t hwa_id);

static int SEQUENCE_TEST_SCENARIO_1(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_2(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_3(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_4(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_5(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_6(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_7(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_8(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_9(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_10(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_11(e_pma_hwa_id_t hwa_id);
static int SEQUENCE_TEST_SCENARIO_12(e_pma_hwa_id_t hwa_id);
/**********************************************************************************
  Private global variable
**********************************************************************************/
#if defined(RCAR_V4H)
    g_hwa_id_max = R_PMA_HWA_ID_V4H_NUM;
    g_hwa_depend_num = R_PMA_HW_DEP_HWA_DEPEND_V4H_NUM;
    uint64_t g_impslv_children = R_PMA_HW_DEP_HWA_BIT_V4H_CONCLUSION_SPMI;
    e_pma_hwa_id_t g_impslv_hwa_id = R_PMA_HWA_ID_V4H_IMPDTA;
#elif defined(RCAR_V4M)
    g_hwa_id_max = R_PMA_HWA_ID_V4M_NUM;
    g_hwa_depend_num = R_PMA_HW_DEP_HWA_DEPEND_V4M_NUM;
    uint64_t g_impslv_children = R_PMA_HW_DEP_HWA_BIT_V4M_CONCLUSION_SPMI;
    e_pma_hwa_id_t g_impslv_hwa_id = R_PMA_HWA_ID_V4M_IMPDTA;
#elif defined(RCAR_S4)
    g_hwa_id_max = R_PMA_HWA_ID_S4_NUM;
    g_hwa_depend_num = R_PMA_HW_DEP_HWA_DEPEND_V4H_NUM;
    uint64_t g_impslv_children = R_PMA_HW_DEP_HWA_BIT_V4H_CONCLUSION_SPMI;
    e_pma_hwa_id_t g_impslv_hwa_id = R_PMA_HWA_ID_V4H_IMPDTA;
#endif
extern st_pma_handle_t g_pma_handle;
const p_test_func_t gp_test_func[TEST_SCENARIO_NUM] = 
{
    &SEQUENCE_TEST_SCENARIO_1,
    &SEQUENCE_TEST_SCENARIO_2,
    &SEQUENCE_TEST_SCENARIO_3,
    &SEQUENCE_TEST_SCENARIO_4,
    &SEQUENCE_TEST_SCENARIO_5,
    &SEQUENCE_TEST_SCENARIO_6,
    &SEQUENCE_TEST_SCENARIO_7,
    &SEQUENCE_TEST_SCENARIO_8,
    &SEQUENCE_TEST_SCENARIO_9,
    &SEQUENCE_TEST_SCENARIO_10,
    &SEQUENCE_TEST_SCENARIO_11,
    &SEQUENCE_TEST_SCENARIO_12
};
#if defined(RCAR_V4H)
const e_pma_hwa_id_t g_test_hw_dep_hwa_bit_to_id_tbl[R_PMA_HW_DEP_HWA_DEPEND_V4H_NUM] = {
    R_PMA_HWA_ID_V4H_IMPCNN,
    R_PMA_HWA_ID_V4H_IMPSDMAC0,
    R_PMA_HWA_ID_V4H_IMPSDMAC1,
    R_PMA_HWA_ID_V4H_SPMC,
    R_PMA_HWA_ID_V4H_IMP0,
    R_PMA_HWA_ID_V4H_IMP1,
    R_PMA_HWA_ID_V4H_IMP2,
    R_PMA_HWA_ID_V4H_IMP3,
    R_PMA_HWA_ID_V4H_IMPDMAC0,
    R_PMA_HWA_ID_V4H_IMPDMAC1,
    R_PMA_HWA_ID_V4H_IMPPSC,
    R_PMA_HWA_ID_V4H_CVE0,
    R_PMA_HWA_ID_V4H_CVE1,
    R_PMA_HWA_ID_V4H_CVE2,
    R_PMA_HWA_ID_V4H_CVE3,
    R_PMA_HWA_ID_V4H_IMPDTA,
    R_PMA_HWA_ID_V4H_IMPSLV,
    R_PMA_HWA_ID_V4H_SPMI,
    R_PMA_HWA_ID_V4H_VDSP0_CSB,
    R_PMA_HWA_ID_V4H_VDSP0_BUS,
    R_PMA_HWA_ID_V4H_VDSP1_CSB,
    R_PMA_HWA_ID_V4H_VDSP1_BUS,
    R_PMA_HWA_ID_V4H_VDSP2_BUS,
    R_PMA_HWA_ID_V4H_VDSP2_CSB,
    R_PMA_HWA_ID_V4H_VDSP3_BUS,
    R_PMA_HWA_ID_V4H_VDSP3_CSB,
    R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR,
    R_PMA_HWA_ID_V4H_REG_SPMI,
    R_PMA_HWA_ID_V4H_REG_SPMC,
    R_PMA_HWA_ID_V4H_SPMC01
};
e_pma_hwa_id_t * g_lowest_group_tbl;

const e_pma_hwa_id_t lowest_group1[] = {
    R_PMA_HWA_ID_V4H_SPMI,
    R_PMA_HWA_ID_V4H_IMPSLV,
    R_PMA_HWA_ID_V4H_IMPDTA,
    R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR,
    R_PMA_HWA_ID_V4H_REG_SPMI,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group2[] = {
    R_PMA_HWA_ID_V4H_SPMC,
    R_PMA_HWA_ID_V4H_IMPSDMAC1,
    R_PMA_HWA_ID_V4H_IMPSDMAC0,
    R_PMA_HWA_ID_V4H_REG_SPMC,
    R_PMA_HWA_ID_V4H_SPMC01,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group3[] = {
    R_PMA_HWA_ID_V4H_IMP0,
    R_PMA_HWA_ID_V4H_IMP1,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group4[] = {
    R_PMA_HWA_ID_V4H_IMP2,
    R_PMA_HWA_ID_V4H_IMP3,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group5[] = {
    R_PMA_HWA_ID_V4H_IMPPSC,
    R_PMA_HWA_ID_V4H_IMPDMAC0,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group6[] = {
    R_PMA_HWA_ID_V4H_PAPSDMA,
    R_PMA_HWA_ID_V4H_HSM,
    R_PMA_HWA_ID_V4H_PAPBUS,
    R_PMA_HWA_ID_V4H_DCM,
    R_PMA_HWA_ID_V4H_CBF,
    R_PMA_HWA_ID_V4H_SBF,
    R_PMA_HWA_ID_V4H_OSM,
    R_PMA_HWA_ID_V4H_ISM0,
    R_PMA_HWA_ID_V4H_ISM1,
    R_PMA_HWA_ID_V4H_FBS,
    R_PMA_HWA_ID_V4H_APB,
    R_PMA_HWA_ID_V4H_DUL,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group7[] = {
R_PMA_HWA_ID_V4H_ISP1,
R_PMA_HWA_ID_V4H_ISPCS1,
R_PMA_HWA_ID_V4H_VSPX1,
R_PMA_HWA_ID_V4H_FCPVX1,
        R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group8[] = {
    R_PMA_HWA_ID_V4H_ISP0,
    R_PMA_HWA_ID_V4H_ISPCS0,
    R_PMA_HWA_ID_V4H_VSPX0,
    R_PMA_HWA_ID_V4H_FCPVX0,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group9[] = {
    R_PMA_HWA_ID_V4H_DOF,
    R_PMA_HWA_ID_V4H_DOF_FCPRM,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group10[] = {
    R_PMA_HWA_ID_V4H_SMPS0,
    R_PMA_HWA_ID_V4H_SMPO0,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group11[] = {
    R_PMA_HWA_ID_V4H_VDSP1_CSB,
    R_PMA_HWA_ID_V4H_VDSP1_BUS,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group12[] = {
    R_PMA_HWA_ID_V4H_VDSP3_BUS,
    R_PMA_HWA_ID_V4H_VDSP3_CSB,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group13[] = {
    R_PMA_HWA_ID_V4H_VDSP2_BUS,
    R_PMA_HWA_ID_V4H_VDSP2_CSB,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group14[] = {
    R_PMA_HWA_ID_V4H_VDSP0_CSB,
    R_PMA_HWA_ID_V4H_VDSP0_BUS,
        R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group15[] = {
    R_PMA_HWA_ID_V4H_HSM,
    R_PMA_HWA_ID_V4H_DCM,
    R_PMA_HWA_ID_V4H_CBF,
    R_PMA_HWA_ID_V4H_SBF,
    R_PMA_HWA_ID_V4H_OSM,
    R_PMA_HWA_ID_V4H_ISM0,
    R_PMA_HWA_ID_V4H_ISM1,
    R_PMA_HWA_ID_V4H_FBS,
    R_PMA_HWA_ID_V4H_APB,
    R_PMA_HWA_ID_V4H_DUL,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group16[] = {
    R_PMA_HWA_ID_V4H_SPMC,
    R_PMA_HWA_ID_V4H_REG_SPMC,
    R_PMA_HWA_ID_V4H_SPMC01,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group17[] = {
    R_PMA_HWA_ID_V4H_SPMI,
    R_PMA_HWA_ID_V4H_REG_SPMI,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group18[] = {
    R_PMA_HWA_ID_V4H_DOF,
    R_PMA_HWA_ID_V4H_DOF_FCPRM,
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group19[] = {
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group20[] = {
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group21[] = {
    R_PMA_HWA_ID_V4H_NUM
};

const e_pma_hwa_id_t lowest_group22[] = {
    R_PMA_HWA_ID_V4H_NUM
};

unsigned int lowest_group_num[] = {
    5,5,2,2,2,12,4,4,2,2,
    2,2,2,2,10,3,2,2,0,0,
    0,0,0
};

#else
const e_pma_hwa_id_t g_test_hw_dep_hwa_bit_to_id_tbl[R_PMA_HW_DEP_HWA_DEPEND_V4M_NUM] = {
    R_PMA_HWA_ID_V4M_IMPCNN,
    R_PMA_HWA_ID_V4M_SPMC,
    R_PMA_HWA_ID_V4M_IMP0,
    R_PMA_HWA_ID_V4M_IMP1,
    R_PMA_HWA_ID_V4M_IMPDMAC0,
    R_PMA_HWA_ID_V4M_IMPDMAC1,
    R_PMA_HWA_ID_V4M_IMPPSC,
    R_PMA_HWA_ID_V4M_CVE0,
    R_PMA_HWA_ID_V4M_CVE1,
    R_PMA_HWA_ID_V4M_CVE2,
    R_PMA_HWA_ID_V4M_CVE3,
    R_PMA_HWA_ID_V4M_IMPDTA,
    R_PMA_HWA_ID_V4M_IMPSLV,
    R_PMA_HWA_ID_V4M_SPMI,
    R_PMA_HWA_ID_V4M_VDSP0_CSB,
    R_PMA_HWA_ID_V4M_VDSP0_BUS,
    R_PMA_HWA_ID_V4M_VDSP1_CSB,
    R_PMA_HWA_ID_V4M_VDSP1_BUS,
    R_PMA_HWA_ID_V4M_REG_SPMI,
    R_PMA_HWA_ID_V4M_REG_SPMC,
    R_PMA_HWA_ID_V4M_SPMC01,
    R_PMA_HWA_ID_V4M_FBA_IMP0,
    R_PMA_HWA_ID_V4M_FBA_IMP1,
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV0,
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV1,
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV2,
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV3,
    R_PMA_HWA_ID_V4M_FBA_IMP_DP0,
    R_PMA_HWA_ID_V4M_FBA_IMP_DP1,
    R_PMA_HWA_ID_V4M_FBA_IMP_CNN0,
    R_PMA_HWA_ID_V4M_FBA_IMP_DSP0,
    R_PMA_HWA_ID_V4M_FBA_IMP_DSP1,
    R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0,
    R_PMA_HWA_ID_V4M_CVE3,
    R_PMA_HWA_ID_V4M_CVE2,
    R_PMA_HWA_ID_V4M_CVE1,
    R_PMA_HWA_ID_V4M_CVE0,
};
e_pma_hwa_id_t * g_lowest_group_tbl;

const e_pma_hwa_id_t lowest_group1[] = {
    R_PMA_HWA_ID_V4M_CVE0,
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group2[] = {
    R_PMA_HWA_ID_V4M_CVE1,
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV1,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group3[] = {
    R_PMA_HWA_ID_V4M_CVE2,
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV2,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group4[] = {
    R_PMA_HWA_ID_V4M_CVE3,
    R_PMA_HWA_ID_V4M_FBA_IMP_OCV3,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group5[] = {
    R_PMA_HWA_ID_V4M_IMR0,
    R_PMA_HWA_ID_V4M_FBA_IMR0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group6[] = {
    R_PMA_HWA_ID_V4M_IMR1,
    R_PMA_HWA_ID_V4M_FBA_IMR1,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group7[] = {
R_PMA_HWA_ID_V4M_IMS0,
    R_PMA_HWA_ID_V4M_FBA_IMS0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group8[] = {
    R_PMA_HWA_ID_V4M_IMS1,
    R_PMA_HWA_ID_V4M_FBA_IMS1,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group9[] = {
    R_PMA_HWA_ID_V4M_IMPCNN,
    R_PMA_HWA_ID_V4M_FBA_IMP_CNN0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group10[] = {
    R_PMA_HWA_ID_V4M_VDSP0_CSB,
    R_PMA_HWA_ID_V4M_VDSP0_BUS,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group11[] = {
    R_PMA_HWA_ID_V4M_VDSP1_CSB,
    R_PMA_HWA_ID_V4M_VDSP1_BUS,
    R_PMA_HWA_ID_V4M_NUM,
    
};

const e_pma_hwa_id_t lowest_group12[] = {
    R_PMA_HWA_ID_V4M_SMPS0,
    R_PMA_HWA_ID_V4M_SMPO0,
    R_PMA_HWA_ID_V4M_FBA_SMPS0,
    R_PMA_HWA_ID_V4M_FBA_SMPO0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group13[] = {
    R_PMA_HWA_ID_V4M_ISP0,
    R_PMA_HWA_ID_V4M_ISPCS0,
    R_PMA_HWA_ID_V4M_ISPCS1,
    R_PMA_HWA_ID_V4M_VSPX0,
    R_PMA_HWA_ID_V4M_FCPVX0,
    R_PMA_HWA_ID_V4M_FBA_ISP0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group14[] = {
    R_PMA_HWA_ID_V4M_IMP1,
    R_PMA_HWA_ID_V4M_IMP0,
    R_PMA_HWA_ID_V4M_FBA_IMP0,
    R_PMA_HWA_ID_V4M_FBA_IMP1,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group15[] = {
    R_PMA_HWA_ID_V4M_IMPDMAC1,
    R_PMA_HWA_ID_V4M_FBA_IMP_DP1,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group16[] = {
    R_PMA_HWA_ID_V4M_IMPPSC,
    R_PMA_HWA_ID_V4M_IMPDMAC0,
    R_PMA_HWA_ID_V4M_FBA_IMP_DP0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group17[] = {
    R_PMA_HWA_ID_V4M_PAPSDMA,
    R_PMA_HWA_ID_V4M_HSM,
    R_PMA_HWA_ID_V4M_PAPBUS,
    R_PMA_HWA_ID_V4M_DCM,
    R_PMA_HWA_ID_V4M_CBF,
    R_PMA_HWA_ID_V4M_SBF,R_PMA_HWA_ID_V4M_OSM,
    R_PMA_HWA_ID_V4M_ISM0,
    R_PMA_HWA_ID_V4M_ISM1,
    R_PMA_HWA_ID_V4M_FBS,
    R_PMA_HWA_ID_V4M_APB,R_PMA_HWA_ID_V4M_DUL,
    R_PMA_HWA_ID_V4M_FBA_PAP0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group18[] = {
    R_PMA_HWA_ID_V4M_SPMC,
    R_PMA_HWA_ID_V4M_REG_SPMC,
    R_PMA_HWA_ID_V4M_SPMC01,
    R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group19[] = {
    R_PMA_HWA_ID_V4M_SPMI,
    R_PMA_HWA_ID_V4M_REG_SPMI,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group20[] = {
    R_PMA_HWA_ID_V4M_HSM,
    R_PMA_HWA_ID_V4M_DCM,
    R_PMA_HWA_ID_V4M_CBF,
    R_PMA_HWA_ID_V4M_SBF,
    R_PMA_HWA_ID_V4M_OSM,
    
    R_PMA_HWA_ID_V4M_ISM0,
    R_PMA_HWA_ID_V4M_ISM1,
    R_PMA_HWA_ID_V4M_FBS,
    R_PMA_HWA_ID_V4M_APB,
    R_PMA_HWA_ID_V4M_DUL,
    R_PMA_HWA_ID_V4M_NUM,
};

const e_pma_hwa_id_t lowest_group21[] = {
    R_PMA_HWA_ID_V4M_SPMC,
    R_PMA_HWA_ID_V4M_REG_SPMC,
    R_PMA_HWA_ID_V4M_SPMC01,
    R_PMA_HWA_ID_V4M_NUM
};

const e_pma_hwa_id_t lowest_group22[] = {
    R_PMA_HWA_ID_V4M_DOF,
    R_PMA_HWA_ID_V4M_DOF_FCPRM,
    R_PMA_HWA_ID_V4M_FBA_UMFL0,
    R_PMA_HWA_ID_V4M_NUM,
};

unsigned int lowest_group_num[] = {
    2,2,2,2,2,2,2,2,2,2,
    2,4,6,4,2,3,13,4,2,10,
    3,3,0
};

#endif

#if defined(RCAR_V4H) || defined(RCAR_V4M)
int execute_lowest_test(void) {

    e_pma_power_state_t   lowest_power_state;
    st_pma_hwa_state_t    hwa_state;
    unsigned int group, hwa, num;
    e_pma_return_t        pma_ret;
    e_pma_hwa_id_t hwa_id;
    unsigned int fail_count = 0;
    st_pma_hwa_state_t     expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    TEST_LOG("------------------------- Lowest State test Start --------------------------\n\n");
    for (group = 0; group < 22 ; group++) {
        switch (group)
        {
        case 0:
            g_lowest_group_tbl = lowest_group1; 
            break;
        case 1:
            g_lowest_group_tbl = lowest_group2; 
            break;
        case 2:
            g_lowest_group_tbl = lowest_group3; 
            break;
        case 3:
            g_lowest_group_tbl = lowest_group4; 
            break;
        case 4:
            g_lowest_group_tbl = lowest_group5; 
            break;
        case 5:
            g_lowest_group_tbl = lowest_group6; 
            break;
        case 6:
            g_lowest_group_tbl = lowest_group7; 
            break;
        case 7:
            g_lowest_group_tbl = lowest_group8; 
            break;
        case 8:
            g_lowest_group_tbl = lowest_group9; 
            break;
        case 9:
            g_lowest_group_tbl = lowest_group10; 
            break;
        case 10:
            g_lowest_group_tbl = lowest_group11; 
            break;
        case 11:
            g_lowest_group_tbl = lowest_group12; 
            break;
        case 12:
            g_lowest_group_tbl = lowest_group13; 
            break;
        case 13:
            g_lowest_group_tbl = lowest_group14; 
            break;
        case 14:
            g_lowest_group_tbl = lowest_group15; 
            break;
        case 15:
            g_lowest_group_tbl = lowest_group16; 
            break;
        case 16:
            g_lowest_group_tbl = lowest_group17; 
            break;
        case 17:
            g_lowest_group_tbl = lowest_group18; 
            break;
        case 18:
            g_lowest_group_tbl = lowest_group19; 
            break;
        case 19:
            g_lowest_group_tbl = lowest_group20; 
            break;
        case 20:
            g_lowest_group_tbl = lowest_group21; 
            break;
        case 21:
            g_lowest_group_tbl = lowest_group22; 
            break;
        default:
            break;
        }

        num = lowest_group_num[group];
        TEST_LOG("target hwa group : %d \n\n", group+1);

        for (hwa = 0; hwa < num; hwa++)
        {
            hwa_id = g_lowest_group_tbl[hwa];
    /* set HP policy and Request Release  */
            pma_ret = R_PMA_GetLockHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" GetLockError  \n");
            }
            pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" SetError  \n");
            }
            pma_ret = R_PMA_RequestHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" ReqError  \n");
            }
            expected_hwa_state.enReqState      = R_PMA_HWA_REQUESTED;
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_UNDEFINED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" Wait For State ReqError (line:%d) (pma_ret=%d) \n", __LINE__, pma_ret);
            }
            pma_ret = R_PMA_ReleaseHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" RelError  \n");
            }
            expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_UNDEFINED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" Wait For State RelError (line:%d) (pma_ret=%d) \n", __LINE__, pma_ret);
            }
            pma_ret = R_PMA_UnlockHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" UnLockError  \n");
            }
        }
        for (hwa = 0; hwa < num; hwa++)
        {
    /* check status */
            hwa_id = g_lowest_group_tbl[hwa];
            (void)R_PMA_GetLockHwa(g_pma_handle, hwa_id);
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            (void)R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            (void)R_PMA_UnlockHwa(g_pma_handle,hwa_id);
            DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, lowest_power_state);
        }

        for (hwa = 0; hwa < num; hwa++)
        {
    /* change policy PG */
            hwa_id = g_lowest_group_tbl[hwa];
            pma_ret = R_PMA_GetLockHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" GetLockError  \n");
            }
            pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_PG_MODE);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" SetError  \n");
            }

            expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_STATE_OFF;
            expected_reset_req                 = R_PMA_UNDEFINED_RESET;
            /* Change all devices to PG state. This wait synchronizes register settings before R_PMA_UnlockHwa. */
            pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 5, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                /* Don't check the state here. Because In the case of shared domains, it will not change to PG unless everything becomes PG */
            }

            pma_ret = R_PMA_UnlockHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" UnLockError  \n");
            }
        }
        for (hwa = 0; hwa < num; hwa++)
        {
            /* check status */
            hwa_id = g_lowest_group_tbl[hwa];
            (void)R_PMA_GetLockHwa(g_pma_handle, hwa_id);

            expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_STATE_OFF;
            expected_reset_req                 = R_PMA_UNDEFINED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 5, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf(" Wait For State SetpolicyError (line:%d) (hwa_id:%u)(pma_ret=%d) \n", __LINE__, hwa_id, pma_ret);
                st_pma_hwa_state_t     act_hwa_state;
                e_pma_reset_req_t      act_reset_state;
                printf("expect enReqState(%u), enActualClkStat(%u), enActualPwrStat(%u) expected_reset_req(%u)\n", expected_hwa_state.enReqState, expected_hwa_state.enActualClkStat, expected_hwa_state.enActualPwrStat, expected_reset_req);
                R_PMA_GetHwaState(g_pma_handle, hwa_id, &act_hwa_state);
                R_PMA_GetReset(g_pma_handle, hwa_id, &act_reset_state);
                printf("actual enReqState(%u), enActualClkStat(%u), enActualPwrStat(%u), reset_req(%u)\n", act_hwa_state.enReqState, act_hwa_state.enActualClkStat, act_hwa_state.enActualPwrStat,act_reset_state);
            }

            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            (void)R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            (void)R_PMA_UnlockHwa(g_pma_handle,hwa_id);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
#if defined(RCAR_V4M)
            if ((lowest_power_state != R_PMA_STATE_PG) && (18 != group))
            {
                fail_count++;
                printf("Failed : lowest power state is not PG_STATE \r\n");
            }
            else if ((lowest_power_state != R_PMA_STATE_CG) && (18 == group))
            {
                fail_count++;
                printf("Failed : lowest power state is not CG_STATE (SPMI or REG_SPMI)\r\n");
            }
#else
            if (lowest_power_state != R_PMA_STATE_PG)
            {
                fail_count++;
                printf("Failed : lowest power state is not PG_STATE \r\n");
            }
#endif
        }
    }
    if (fail_count != 0)
    {
        return TEST_NG;
    }
    return TEST_OK;
}
#endif
/**********************************************************************************
  Function: pma_sequence_test
    - description:
       main routine of sequence test
**********************************************************************************/
int pma_sequence_test(void)
{
    int result = TEST_OK;
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t     expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_policy_t  hwa_policy;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t hwa_id;

    for (e_pma_hwa_id_t id = 0; g_pma_test_hwa_id[id] < g_hwa_id_max;id++)
    {
        /* Set PG Mode */
        pma_ret = R_PMA_GetLockHwa(g_pma_handle, g_pma_test_hwa_id[id]);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            pma_ret = R_PMA_GetHwaState(g_pma_handle, g_pma_test_hwa_id[id], &hwa_state);
            if (R_PMA_RESULT_OK !=pma_ret)
            {
                TEST_LOG("(hwa_id = %d) R_PMA_GetHwaState Failed\n", g_pma_test_hwa_id[id]);
            }
            pma_ret = R_PMA_GetPowerPolicy(g_pma_handle, g_pma_test_hwa_id[id], &hwa_policy);
            if (R_PMA_ERR_UNSUPPORTED_OPERATION == pma_ret)
            {
                TEST_LOG("(hwa_id = %d) R_PMA_ERR_UNSUPPORTED_OPERATION\n", g_pma_test_hwa_id[id]);
                pma_ret = R_PMA_UnlockHwa(g_pma_handle, g_pma_test_hwa_id[id]);
                if (R_PMA_RESULT_OK != pma_ret)
                {
                    TEST_LOG("(hwa_id = %d) R_PMA_UnlockHwa Failed\n", g_pma_test_hwa_id[id]);
                    exchange_ret_code_to_str(pma_ret);
                }
                continue;
            }
            DISPLAY_HWA_STATE_INFO(g_pma_test_hwa_id[id], hwa_state, NO_DISP_LOW_POW);
            DISPLAY_POWER_POLICY(g_pma_test_hwa_id[id], hwa_policy);

            /* If Power Policy is not PG, Set PG-MODE */
            if (R_PMA_PG_MODE != hwa_policy)
            {
                TEST_LOG("(hwa_id = %d) Change Power Policy to R_PMA_PG_MODE\n", g_pma_test_hwa_id[id]);
                (void)R_PMA_SetPowerPolicy(g_pma_handle, g_pma_test_hwa_id[id], R_PMA_PG_MODE);
                (void)R_PMA_ReleaseHwa(g_pma_handle, g_pma_test_hwa_id[id]);

                expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
                expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
                expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
                expected_reset_req                 = R_PMA_UNDEFINED_RESET;

                pma_ret = R_PMA_WaitForState(g_pma_handle, g_pma_test_hwa_id[id], &expected_hwa_state, expected_reset_req, 1, 1000, 0);
                if (R_PMA_RESULT_OK != pma_ret)
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, g_pma_test_hwa_id[id], &hwa_state);
                    DISPLAY_HWA_STATE_ERROR(g_pma_test_hwa_id[id], hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed\n", g_pma_test_hwa_id[id]);
                }
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_GetLockHwa Failed\n", g_pma_test_hwa_id[id]);
        }

        if (R_PMA_RESULT_OK == pma_ret)
        {
            pma_ret = R_PMA_UnlockHwa(g_pma_handle, g_pma_test_hwa_id[id]);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                TEST_LOG("(hwa_id = %d) R_PMA_UnlockHwa Failed\n", g_pma_test_hwa_id[id]);
                exchange_ret_code_to_str(pma_ret);
            }
        }
        if (R_PMA_RESULT_OK != pma_ret)
        {
            exchange_ret_code_to_str(pma_ret);
            result = TEST_NG;
        }
    }
    TEST_LOG("------------------------------------------------------ Set all HWA PG-Mode ------------------------------------------------------\n");

/** request -> release **/
    for (e_pma_hwa_id_t id = 0; g_pma_test_hwa_id[id] < g_hwa_id_max;id++)
    {
        hwa_id = g_pma_test_hwa_id[id];
        TEST_LOG("(hwa_id = %d)  (g_pma_handle = 0x%lx)  \n", hwa_id,g_pma_handle);
        /* Set PG Mode */
        pma_ret = R_PMA_GetLockHwa(g_pma_handle, hwa_id);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* R_PMA_RequestHwa (PG -> HP) */
            pma_ret = R_PMA_RequestHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                expected_hwa_state.enReqState      = R_PMA_HWA_REQUESTED;
                expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
                expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
                expected_reset_req                 = R_PMA_UNDEFINED_RESET;

                pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
                if (R_PMA_RESULT_OK != pma_ret)
                {
                    TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed (R_PMA_RequestHwa) \n", hwa_id);
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    DISPLAY_HWA_STATE_ERROR(hwa_id, hwa_state, NO_DISP_LOW_POW);
                }
                /* R_PMA_ReleaseHwa (HP -> PG) */
                pma_ret = R_PMA_ReleaseHwa(g_pma_handle, hwa_id);
                if (R_PMA_RESULT_OK == pma_ret)
                {
                    expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
                    expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
                    expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
                    expected_reset_req                 = R_PMA_UNDEFINED_RESET;

                    pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 2000, 0);
                    if (R_PMA_RESULT_OK != pma_ret)
                    {
                        TEST_LOG("(hwa_id = %d) R_PMA_WaitForState (R_PMA_ReleaseHwa) Failed\n", hwa_id);
                        (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                        DISPLAY_HWA_STATE_ERROR(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    }
                }
            }
            else if (R_PMA_ERR_UNSUPPORTED_OPERATION == pma_ret)
            {
                TEST_LOG("(hwa_id = %d) R_PMA_ERR_UNSUPPORTED_OPERATION\n", hwa_id);
                pma_ret = R_PMA_UnlockHwa(g_pma_handle, hwa_id);
                if (R_PMA_RESULT_OK != pma_ret)
                {
                    TEST_LOG("(hwa_id = %d) R_PMA_UnlockHwa Failed\n", hwa_id);
                    exchange_ret_code_to_str(pma_ret);
                }
                continue;
            }
            if (R_PMA_RESULT_OK == pma_ret)
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
                (void)R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
                DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, lowest_power_state);
                DISPLAY_POWER_POLICY_INFO(hwa_id, hwa_policy);
            }
            pma_ret = R_PMA_UnlockHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                TEST_LOG("(hwa_id = %d) R_PMA_UnlockHwa Failed\n", hwa_id);
                exchange_ret_code_to_str(pma_ret);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_GetLockHwa Failed\n", hwa_id);
        }
        if (R_PMA_RESULT_OK != pma_ret)
        {
            exchange_ret_code_to_str(pma_ret);
            result = TEST_NG;
        }
    }

   TEST_LOG("------------------------------------------------------ All HWA is PG-Mode ------------------------------------------------------\n");


    TEST_LOG("------------------------------------------------------ Start ------------------------------------------------------\n");

    for (e_pma_hwa_id_t id = 0; g_pma_test_hwa_id[id] < g_hwa_id_max;id++)
    {
        result = execute_hwa_sequence(g_pma_test_hwa_id[id]);
    }

    if(TEST_OK != result)
    {
        TEST_LOG("---------------------------------------------- Failed ----------------------------------------------\n\n");
    }
    else
    {
        TEST_LOG("---------------------------------------------- Success ----------------------------------------------\n\n");
    }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
    result = execute_lowest_test();

    if(TEST_OK != result)
    {
        TEST_LOG("------------------------------ Lowest State Failed -------------------------------\n\n");
    }
#endif
    return result;
}

/**********************************************************************************
  TEST: execute_hwa_sequence
    - description:
       Execute one of HWA's sequence
**********************************************************************************/
static int execute_hwa_sequence(e_pma_hwa_id_t hwa_id)
{
    int result = TEST_OK;
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_policy_t  hwa_policy;

    TEST_LOG("(hwa_id = %d) ---------------------------------------------- Start ----------------------------------------------\n", hwa_id);

    /* Prepare for test */
    pma_ret = R_PMA_GetLockHwa(g_pma_handle, hwa_id);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
        pma_ret = R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
        if (R_PMA_ERR_UNSUPPORTED_OPERATION == pma_ret)
        {
            TEST_LOG("(hwa_id = %d) R_PMA_ERR_UNSUPPORTED_OPERATION\n", hwa_id);
        }
        else
        {
            DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
            DISPLAY_POWER_POLICY_INFO(hwa_id, hwa_policy);

            /* If Power Policy is not PG, Set PG-MODE */
            if (R_PMA_PG_MODE != hwa_policy)
            {
                TEST_LOG("(hwa_id = %d) Change Power Policy to R_PMA_PG_MODE\n", hwa_id);
                (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_PG_MODE);
                (void)R_PMA_ReleaseHwa(g_pma_handle, hwa_id);

                expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
                expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
                expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
                expected_reset_req                 = R_PMA_UNDEFINED_RESET;

                pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
                if (R_PMA_RESULT_OK != pma_ret)
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    DISPLAY_HWA_STATE_ERROR(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed\n", hwa_id);
                }
            }
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_GetLockHwa Failed\n", hwa_id);
    }

    if (R_PMA_RESULT_OK == pma_ret)
    {
        for (int i = 0; i < TEST_SCENARIO_NUM; i++)
        {
            /* Execute Test */
            result = gp_test_func[i](hwa_id);

            if (TEST_OK != result)
            {
                break;
            }
        }
    }
    else if (R_PMA_ERR_UNSUPPORTED_OPERATION == pma_ret)
    {
    }
    else
    {
        exchange_ret_code_to_str(pma_ret);
        result = TEST_NG;
    }

    if (R_PMA_ERR_UNSUPPORTED_OPERATION == pma_ret)
    {
    }
    else
    {
        /* Clean up for test */
        pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_PG_MODE);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            pma_ret = R_PMA_ReleaseHwa(g_pma_handle, hwa_id);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
                expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
                expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
                expected_reset_req                 = R_PMA_UNDEFINED_RESET;

                pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
                if (R_PMA_RESULT_OK != pma_ret)
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    DISPLAY_HWA_STATE_ERROR(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed\n", hwa_id);
                }
            }
            else
            {
                TEST_LOG("(hwa_id = %d) R_PMA_ReleaseHwa Failed\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed\n", hwa_id);
        }

        if (R_PMA_RESULT_OK != pma_ret)
        {
            exchange_ret_code_to_str(pma_ret);
        }
    }

    pma_ret = R_PMA_UnlockHwa(g_pma_handle, hwa_id);
    if (R_PMA_RESULT_OK != pma_ret)
    {
        TEST_LOG("(hwa_id = %d) R_PMA_UnlockHwa Failed\n", hwa_id);
        exchange_ret_code_to_str(pma_ret);
    }

    if((TEST_OK != result) || (R_PMA_RESULT_OK != pma_ret))
    {
        TEST_LOG("(hwa_id = %d) ---------------------------------------------- Failed ----------------------------------------------\n\n", hwa_id);
    }
    else
    {
        TEST_LOG("(hwa_id = %d) ---------------------------------------------- Success ----------------------------------------------\n\n", hwa_id);
    }
    return result;
}

static int SEQUENCE_TEST_SCENARIO_1(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;
    e_pma_hwa_id_t        share_domain_hwa_id;
    st_pma_hwa_state_t    share_domain_hwa_state;
    st_pma_hwa_state_t    share_domain_expected_hwa_state;

/* TEST_SCENARIO_1 - (1) */
    /* R_PMA_RequestHwa (PG -> HP) */
    pma_ret = R_PMA_RequestHwa(g_pma_handle, hwa_id);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_1].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_1].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_1].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_1].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d, expected = %d)\n", hwa_id, lowest_power_state, gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_1].lowest_pow);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_1 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    TEST_LOG("(target_bit = 0x%08x) \n", g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent);
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].hwa_state.enActualPwrStat;
                        TEST_LOG("(parent_hwa_id = %d) ClkStat = %d  PwrStat = %d \n", parent_hwa_id, parent_expected_hwa_state.enActualClkStat , parent_expected_hwa_state.enActualPwrStat);
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            DISPLAY_HWA_STATE(parent_hwa_id, hwa_state, lowest_power_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed (lowest_power_state=%d, expected=%d\n", parent_hwa_id, lowest_power_state, gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_1 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                DISPLAY_HWA_STATE(hwa_id, children_hwa_state, lowest_power_state);
                                TEST_LOG("(children_hwa_id = %d)  children_pwa_state = %d children_expected_pwa_state = %d    children Power state Failed\n", children_hwa_id,children_hwa_state.enActualPwrStat ,children_expected_hwa_state.enActualPwrStat);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) (expected = %d) (hwa_state = %d) children Clock state Failed\n", children_hwa_id,children_expected_hwa_state.enActualClkStat,children_hwa_state.enActualClkStat);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                                if(g_impslv_hwa_id != children_hwa_id)
                                {
                                    if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                    {
                                        TEST_LOG("(children_hwa_id = %d) (Ғl = %d) (l = %d) lowest_power_state Failed\n", children_hwa_id,children_expected_hwa_state.enActualClkStat,lowest_power_state);
                                        (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                        return TEST_NG;
                                    }
                                }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                                else
                                {
                                    if (gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                    {
                                        TEST_LOG("(children_hwa_id = %d) (Ғl = %d) (l = %d) lowest_power_state Failed\n", children_hwa_id,children_expected_hwa_state.enActualClkStat,lowest_power_state);
                                        (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                        return TEST_NG;
                                    }
                                }
#endif
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed, g_pma_test_hw_dep_hwa_dependency_tbl[%d].children=0x%x, bit=0x%x \n", children_hwa_id, hwa_id, g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children, bit);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
/* TEST_SCENARIO_1 - (4) */ /* share domain check (Expected to be ON) */
            /* cpg */
            for (uint64_t id = 0; gp_share_cpg_info[hwa_id]->share_domain[id] != g_hwa_id_max ; id++)
            {
                share_domain_hwa_id = gp_share_cpg_info[hwa_id]->share_domain[id];
                if (hwa_id != share_domain_hwa_id)
                {
                    pma_ret = R_PMA_GetLockHwa(g_pma_handle, share_domain_hwa_id);
                    if (R_PMA_RESULT_OK == pma_ret)
                    {
                        (void)R_PMA_GetHwaState(g_pma_handle, share_domain_hwa_id, &share_domain_hwa_state);
                        /* (4)-a Sharing the same domain the HWA domain is also ON.(cpg) */
                        if (expected_hwa_state.enActualClkStat != share_domain_hwa_state.enActualClkStat)
                        {
                            TEST_LOG("(share_domain_hwa_id = %d) share domain cpg state Failed\n", share_domain_hwa_id);
                            (void)R_PMA_UnlockHwa(g_pma_handle, share_domain_hwa_id);
                            return TEST_NG;
                        }
                        pma_ret = R_PMA_UnlockHwa(g_pma_handle, share_domain_hwa_id);
                        if (R_PMA_RESULT_OK != pma_ret)
                        {
                            TEST_LOG("(share_domain_hwa_id = %d) R_PMA_UnlockHwa Failed\n", share_domain_hwa_id);
                        }
                    }
                    else
                    {
                        TEST_LOG("(share_domain_hwa_id = %d) R_PMA_GetLockHwa Failed\n", share_domain_hwa_id);
                    }
                }

            }
            /* sysc */
            for (uint64_t id = 0; gp_share_sysc_info[hwa_id]->share_domain[id] != g_hwa_id_max ; id++)
            {
                share_domain_hwa_id = gp_share_sysc_info[hwa_id]->share_domain[id];
                if (hwa_id != share_domain_hwa_id)
                {
                    pma_ret = R_PMA_GetLockHwa(g_pma_handle, share_domain_hwa_id);
                    if (R_PMA_RESULT_OK == pma_ret)
                    {
                        (void)R_PMA_GetHwaState(g_pma_handle, share_domain_hwa_id, &share_domain_hwa_state);
                        /* (4)-a Sharing the same domain the HWA domain is also ON.(sysc) */
                        if (expected_hwa_state.enActualPwrStat != share_domain_hwa_state.enActualPwrStat)
                        {
                            TEST_LOG("(share_domain_hwa_id = %d) share domain sysc state Failed\n", share_domain_hwa_id);
                            (void)R_PMA_UnlockHwa(g_pma_handle, share_domain_hwa_id);
                            return TEST_NG;
                        }
                        else
                        {
                            /* share domain OFF */
                            /* share domain HWA (PG -> HP) */
                            (void)R_PMA_SetPowerPolicy(g_pma_handle, share_domain_hwa_id, R_PMA_HP_MODE);
                            share_domain_expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
                            share_domain_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[share_domain_hwa_id]->state[TEST_SCENARIO_1].hwa_state.enActualClkStat;
                            share_domain_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[share_domain_hwa_id]->state[TEST_SCENARIO_1].hwa_state.enActualPwrStat;
                            expected_reset_req                 = gp_expected_info_tbl[share_domain_hwa_id]->state[TEST_SCENARIO_1].reset_req;
                            pma_ret = R_PMA_WaitForState(g_pma_handle, share_domain_hwa_id, &share_domain_expected_hwa_state, expected_reset_req, 1, 1000, 0);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* share domain HWA (HP -> PG) */
                                pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, share_domain_hwa_id, R_PMA_PG_MODE);
                                if (R_PMA_RESULT_OK == pma_ret)
                                {
                                    /* check test HWA poicy (Expected not to be OFF) */ /* The HW state does not turn OFF even if the HWA which shares the same domain is set to PG mode. */
                                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                                    if ((expected_hwa_state.enActualPwrStat != hwa_state.enActualPwrStat)
                                        || (expected_hwa_state.enActualClkStat != hwa_state.enActualClkStat))
                                    {
                                        TEST_LOG("(hwa_id = %d) Failed due to policy change in shared domain \n", hwa_id);
                                        (void)R_PMA_UnlockHwa(g_pma_handle, share_domain_hwa_id);
                                        return TEST_NG;
                                    }
                                    TEST_LOG("(hwa_id = %d) HWA sharing a domain check : Success\n",hwa_id);
                                }
                                else
                                {
                                    TEST_LOG("(share_domain_hwa_id = %d) R_PMA_SetPowerPolicy Failed\n", share_domain_hwa_id);
                                }
                            }
                            else
                            {
                                TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
                            }
                        }
                        pma_ret = R_PMA_UnlockHwa(g_pma_handle, share_domain_hwa_id);
                        if (R_PMA_RESULT_OK != pma_ret)
                        {
                            TEST_LOG("(share_domain_hwa_id = %d) R_PMA_UnlockHwa Failed\n", share_domain_hwa_id);
                        }
                    }
                    else
                    {
                        TEST_LOG("(share_domain_hwa_id = %d) R_PMA_GetLockHwa Failed\n", share_domain_hwa_id);
                    }
                }
            }
#endif
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_RequestHwa Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_2(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_2 - (1) */
    /* R_PMA_ReleaseHwa (HP -> PG) */
    pma_ret = R_PMA_ReleaseHwa(g_pma_handle, hwa_id);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_2].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_2].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_2 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_2 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        /* PG mode = TEST_SCENARIO_2 */
                        children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                        children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_ReleaseHwa Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_3(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_3 - (1) */
    /* R_PMA_SetPowerPolicy (PG -> CG) */
    pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_3].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_3].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_3].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_3].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_3].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_3 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_3].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_3].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_3 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_4(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_4 - (1) */
    /* R_PMA_RequestHwa (CG -> HP) */
    pma_ret = R_PMA_RequestHwa(g_pma_handle, hwa_id);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_4].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_4].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_4].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_4].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_4].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_4 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_4].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_4].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_4 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_5(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_5 - (1) */
    /* R_PMA_ReleaseHwa (HP -> CG) */
    pma_ret = R_PMA_ReleaseHwa(g_pma_handle, hwa_id);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_5].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_5].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_5].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_5].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_5].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_5 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_5].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_5].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_2 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_ReleaseHwa Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_6(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_6 - (1) */
    /* R_PMA_SetPowerPolicy (CG -> PG) */
    pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_PG_MODE);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_6].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_6].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_6].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_6].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_6].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_6 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_6].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_6].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_6 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        /* PG mode = TEST_SCENARIO_2 */
                        children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                        children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_7(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_7 - (1) */
    /* R_PMA_RequestHwa (PG -> HP) */
    pma_ret = R_PMA_RequestHwa(g_pma_handle, hwa_id);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_7].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_7].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_7].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_7].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_7].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_7 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_7].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_7].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_7 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_RequestHwa Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_8(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_8 - (1) */
    /* R_PMA_SetPowerPolicy (PG -> HP) */
    pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        /* R_PMA_ReleaseHwa (HP -> HP) */
        pma_ret = R_PMA_ReleaseHwa(g_pma_handle, hwa_id);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_8].hwa_state.enReqState;
            expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_8].hwa_state.enActualClkStat;
            expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_8].hwa_state.enActualPwrStat;
            expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_8].reset_req;

            pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                /* lowest power state */
                pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
                if (R_PMA_RESULT_OK == pma_ret)
                {
                    if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_8].lowest_pow != lowest_power_state)
                    {
                        TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                        return TEST_NG;
                    }
                }
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_8 - (2) */ /* parent check */
                if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
                {
                    for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                    {
                        if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                        {
                            parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                            parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_8].hwa_state.enActualClkStat;
                            parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_8].hwa_state.enActualPwrStat;
                            pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                                /* parent HWA error */
                                if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                                /* if test HWA is not VDSP */
                                if (gp_vdsp_info_flg[hwa_id] == 0)
                                {
                                    if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                    {
                                        TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                        (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                        return TEST_NG;
                                    }
                                }
                                /* lowest power state */
                                pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                                if (R_PMA_RESULT_OK == pma_ret)
                                {
                                    /* The parent's lowest power state is not updated */
                                    if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                    {
                                        TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                        (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                        return TEST_NG;
                                    }
                                }
                                pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                if (R_PMA_RESULT_OK != pma_ret)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                                }
                            }
                            else
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                            }
                        }
                    }
                    TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
                }
/* TEST_SCENARIO_8 - (3) */ /* children check */
                if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                    && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
                {
                    for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                    {
                        if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                        {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                            pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                                /* children HWA error */
                                if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                                /* if test HWA is not VDSP */
                                if (gp_vdsp_info_flg[children_hwa_id] == 0)
                                {
                                    if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                    {
                                        TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                        (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                        return TEST_NG;
                                    }
                                }
                                /* lowest power state */
                                pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                                if (R_PMA_RESULT_OK == pma_ret)
                                {
                                    if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                    {
                                        TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                        (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                        return TEST_NG;
                                    }
                                }
                                pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                if (R_PMA_RESULT_OK != pma_ret)
                                {
                                    TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                                }
                            }
                            else
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                            }
                        }
                    }
                    TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
                }
            }
            else
            {
                TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_ReleaseHwa Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_9(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_9 - (1) */
    /* R_PMA_SetPowerPolicy (HP -> PG) */
    pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_PG_MODE);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_9].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_9].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_9].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_9].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_9].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_9 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_9].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_9].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_9 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        /* PG mode = TEST_SCENARIO_2 */
                        children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                        children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_10(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_10 - (1) */
    /* R_PMA_SetPowerPolicy (PG -> HP) */
    pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_10].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_10].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_10].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_10].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_10].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_10 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_10].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_10].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_10 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_11(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_11 - (1) */
    /* R_PMA_SetPowerPolicy (HP -> CG) */
    pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_11].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_11].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_11].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_11].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_11].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_11 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_11].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_11].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_11 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}

static int SEQUENCE_TEST_SCENARIO_12(e_pma_hwa_id_t hwa_id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_power_state_t   lowest_power_state;
    e_pma_hwa_id_t        parent_hwa_id;
    e_pma_hwa_id_t        children_hwa_id;
    st_pma_hwa_state_t    parent_hwa_state;
    st_pma_hwa_state_t    parent_expected_hwa_state;
    st_pma_hwa_state_t    children_hwa_state;
    st_pma_hwa_state_t    children_expected_hwa_state;

/* TEST_SCENARIO_12 - (1) */
    /* R_PMA_SetPowerPolicy (CG -> HP) */
    pma_ret = R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enReqState      = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_12].hwa_state.enReqState;
        expected_hwa_state.enActualClkStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_12].hwa_state.enActualClkStat;
        expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_12].hwa_state.enActualPwrStat;
        expected_reset_req                 = gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_12].reset_req;

        pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 1000, 0);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            /* lowest power state */
            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, hwa_id, &lowest_power_state);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                if (gp_expected_info_tbl[hwa_id]->state[TEST_SCENARIO_12].lowest_pow != lowest_power_state)
                {
                    TEST_LOG("(hwa_id = %d) Lowest power state Failed (lowest_power_state=%d)\n", hwa_id, lowest_power_state);
                    return TEST_NG;
                }
            }
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, lowest_power_state);
/* TEST_SCENARIO_12 - (2) */ /* parent check */
            if (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent & (uint64_t)(1ULL << bit)))
                    {
                        parent_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        parent_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_12].hwa_state.enActualClkStat;
                        parent_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_12].hwa_state.enActualPwrStat;
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, parent_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, parent_hwa_id, &parent_hwa_state);
                            /* parent HWA error */
                            if (parent_expected_hwa_state.enActualPwrStat != parent_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(parent_hwa_id = %d) Parent Power state Failed\n", parent_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[hwa_id] == 0)
                            {
                                if (parent_expected_hwa_state.enActualClkStat != parent_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Parent Clock state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, parent_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                /* The parent's lowest power state is not updated */
                                if (gp_expected_info_tbl[parent_hwa_id]->state[TEST_SCENARIO_1].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(parent_hwa_id = %d) Lowest power state Failed\n", parent_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, parent_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(parent_hwa_id = %d) R_PMA_UnlockHwa Failed\n", parent_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(parent_hwa_id = %d) R_PMA_GetLockHwa Failed\n", parent_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) parent HWA check : Success\n", hwa_id);
            }
/* TEST_SCENARIO_12 - (3) */ /* children check */
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children)
                && (g_impslv_children != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children))/* If the test HWA is IMPSLV, the convenience child domain of the implementation is SPMI, but there is no CPG dependency */
            {
                for (uint64_t bit = 0; bit < g_hwa_depend_num; bit++)
                {
                    if (0U != (g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children & (uint64_t)(1ULL << bit)))
                    {
                        children_hwa_id = g_test_hw_dep_hwa_bit_to_id_tbl[bit];
                        if(g_impslv_hwa_id != children_hwa_id)
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
                        else
                        {
                            /* PG mode = TEST_SCENARIO_2 */
                            children_expected_hwa_state.enActualClkStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualClkStat;
                            children_expected_hwa_state.enActualPwrStat = gp_expected_info_tbl_impdta->state[TEST_SCENARIO_2].hwa_state.enActualPwrStat;
                        }
#endif
                        pma_ret = R_PMA_GetLockHwa(g_pma_handle, children_hwa_id);
                        if (R_PMA_RESULT_OK == pma_ret)
                        {
                            (void)R_PMA_GetHwaState(g_pma_handle, children_hwa_id, &children_hwa_state);
                            /* children HWA error */
                            if (children_expected_hwa_state.enActualPwrStat != children_hwa_state.enActualPwrStat)
                            {
                                TEST_LOG("(children_hwa_id = %d) children Power state Failed\n", children_hwa_id);
                                (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                return TEST_NG;
                            }
                            /* if test HWA is not VDSP */
                            if (gp_vdsp_info_flg[children_hwa_id] == 0)
                            {
                                if (children_expected_hwa_state.enActualClkStat != children_hwa_state.enActualClkStat)
                                {
                                    TEST_LOG("(children_hwa_id = %d) children Clock state Failed\n", children_hwa_id);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            /* lowest power state */
                            pma_ret = R_PMA_GetLowestPowerState(g_pma_handle, children_hwa_id, &lowest_power_state);
                            if (R_PMA_RESULT_OK == pma_ret)
                            {
                                if (gp_expected_info_tbl[children_hwa_id]->state[TEST_SCENARIO_2].lowest_pow != lowest_power_state)
                                {
                                    TEST_LOG("(children_hwa_id = %d) Lowest power state Failed  (lowest_power_state=%d)\n", children_hwa_id, lowest_power_state);
                                    (void)R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                                    return TEST_NG;
                                }
                            }
                            pma_ret = R_PMA_UnlockHwa(g_pma_handle, children_hwa_id);
                            if (R_PMA_RESULT_OK != pma_ret)
                            {
                                TEST_LOG("(children_hwa_id = %d) R_PMA_UnlockHwa Failed\n", children_hwa_id);
                            }
                        }
                        else
                        {
                            TEST_LOG("(children_hwa_id = %d) R_PMA_GetLockHwa Failed\n", children_hwa_id);
                        }
                    }
                }
                TEST_LOG("(hwa_id = %d) children HWA check : Success\n", hwa_id);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_WaitForState Failed(%d)\n", hwa_id, pma_ret);
        }
    }
    else
    {
        TEST_LOG("(hwa_id = %d) R_PMA_SetPowerPolicy Failed(%d)\n", hwa_id, pma_ret);
    }

    if(R_PMA_RESULT_OK != pma_ret)
    {
        exchange_ret_code_to_str(pma_ret);
        TEST_LOG("(hwa_id = %d) Failed\n", hwa_id);
        return TEST_NG;
    }
    else
    {
        TEST_LOG("(hwa_id = %d) Success\n", hwa_id);
        return TEST_OK;
    }
}
