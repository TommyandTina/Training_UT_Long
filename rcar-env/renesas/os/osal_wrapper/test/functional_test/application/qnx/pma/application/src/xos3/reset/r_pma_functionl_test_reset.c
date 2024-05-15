#include "r_pma_functionl_test_fw.h"
#include "r_pma_functionl_test_sequence.h"
#if defined(RCAR_V4H)
#include "r_pma_functionl_test_reset_v4h.h"
#include "r_pma_functionl_test_id_tbl_v4h.h"
#elif defined(RCAR_V4M)
#include "r_pma_functionl_test_reset_v4h.h"
#include "r_pma_functionl_test_id_tbl_v4m.h"
#elif defined(RCAR_S4)
#include "r_pma_functionl_test_reset_v4h.h"
#include "r_pma_functionl_test_id_tbl_s4.h"
#endif
extern st_pma_handle_t g_pma_handle;
/**********************************************************************************
  Private type definition
**********************************************************************************/
typedef int (*p_test_reset_t)(e_pma_hwa_id_t hwaId, int test_num);
/**********************************************************************************
  Private global variable
**********************************************************************************/
int pma_reset_type_test_v4h(e_pma_hwa_id_t hwa_id,int test_num);
void pma_reset_type_test_s4(e_pma_hwa_id_t hwa_id, int test_num);
int pma_ng_reset_type_test_v4h(e_pma_hwa_id_t hwa_id, int test_num);

#if defined(RCAR_V4H)
e_pma_hwa_id_t    g_hwa_id_max_rst = R_PMA_HWA_ID_V4H_NUM;
int g_test_case_rst  = 8;
int g_ng_test_case_rst  = 20;
p_test_reset_t g_pma_test_reset_func = &pma_reset_type_test_v4h;
p_test_reset_t g_pma_ng_test_reset_func = &pma_ng_reset_type_test_v4h;
#elif defined(RCAR_V4M)
e_pma_hwa_id_t    g_hwa_id_max_rst = R_PMA_HWA_ID_V4M_NUM;
int g_test_case_rst  = 8;
int g_ng_test_case_rst  = 20;
p_test_reset_t g_pma_test_reset_func = &pma_reset_type_test_v4h;
p_test_reset_t g_pma_ng_test_reset_func = &pma_ng_reset_type_test_v4h;
#elif defined(RCAR_S4)
e_pma_hwa_id_t    g_hwa_id_max_rst = R_PMA_HWA_ID_S4_NUM;
int g_test_case_rst  = 8;
int g_ng_test_case_rst  = 20;
p_test_reset_t g_pma_test_reset_func = &pma_reset_type_test_s4;
p_test_reset_t g_pma_ng_test_reset_func = &pma_ng_reset_type_test_v4h;
#endif

/* The NG test item that the state does not change.
Static definition for the purpose of call a function only once to initialize the state*/
int callcount = 0;
int test_result = R_PMA_APPLIED_RESET;

#define CHILDREN_DEASSERT_ASSERT                     1
#define PARENT_DEASSERT_ASSERT                       2
#define GRAND_PARENT_DEASSERT_ASSERT                 3
#define NO_DEP_HWA_DEASSERT_ASSERT                   4
#define GRAND_PARENT_INITIALIZE_DEASSERT             5
#define PARENT_INITIALIZE_DEASSERT                   6
#define CHILDREN_INITIALIZE_DEASSERT                 7
#define NO_DEP_HWA_INITIALIZE_DEASSERT               8
#define ONLY_CHILDREN_NOT_CHANGE_ASSERT              9
#define CHILDREN_AND_PARENT_NOT_CHANGE_ASSERT_1     10
#define CHILDREN_AND_PARENT_NOT_CHANGE_ASSERT_2     11
#define CHILDREN_AND_PARENT_NOT_CHANGE_ASSERT_3     12
#define CHILDREN_AND_PARENT_NOT_CHANGE_DEASSERT_1   13
#define CHILDREN_AND_PARENT_NOT_CHANGE_DEASSERT_2   14
#define ONLY_PARENT_NOT_CHANGE_ASSERT_1             15
#define ONLY_PARENT_NOT_CHANGE_ASSERT_2             16
#define ONLY_PARENT_NOT_CHANGE_DEASSERT_1           17
#define ONLY_PARENT_NOT_CHANGE_DEASSERT_2           18
#define A2_ONLY_PARENT_NOT_CHANGE_ASSERT            19
#define A2_ONLY_PARENT_NOT_CHANGE_DEASSERT          20

void pma_deassert_assert(e_pma_hwa_id_t id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t     expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    st_pma_hwa_state_t    hwa_state;

    (void)R_PMA_GetHwaState(g_pma_handle, id, &hwa_state);    
    if (hwa_state.enReqState == R_PMA_HWA_RELEASED)
    {
        expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
    }
    else
    {
        expected_hwa_state.enReqState      = R_PMA_HWA_REQUESTED;
    }
    pma_ret = R_PMA_ApplyResetHwa(g_pma_handle, id);
    if (R_PMA_RESULT_OK == pma_ret)
    {
        expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
        expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
        expected_reset_req                 = R_PMA_APPLIED_RESET;
        pma_ret = R_PMA_WaitForState(g_pma_handle, id, &expected_hwa_state, expected_reset_req, 1, 100, 0);
        if (R_PMA_RESULT_OK != pma_ret)
        {
            printf("(line:%d) Wait For State Error(pma_ret=%d) \n", __LINE__, pma_ret);
        }
        pma_ret = R_PMA_ReleaseResetHwa(g_pma_handle, id);

        if (R_PMA_RESULT_OK == pma_ret)
        {
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_RELEASED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, id, &expected_hwa_state, expected_reset_req, 1, 100, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf("(line:%d) Wait For State Error(pma_ret=%d) \n", __LINE__, pma_ret);
            }
            pma_ret = R_PMA_ApplyResetHwa(g_pma_handle, id);

        }
    }

    if (R_PMA_RESULT_OK != pma_ret)
    {
        TEST_LOG("(hwa_id = %d) pma_deassert_assert() Failed (result=%d)\n\n", id, pma_ret);
    }
    else
    {
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_APPLIED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, id, &expected_hwa_state, expected_reset_req, 1, 100, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf("(line:%d) Wait For State Error(pma_ret=%d) \n", __LINE__, pma_ret);
            }
        TEST_LOG("--(HWAID = %3d)---------- OK : ApplyReset -> ReleaseReset -> ApplyReset\n", id);fflush(NULL);
    }

}
void pma_assert_deassert(e_pma_hwa_id_t id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t     expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    st_pma_hwa_state_t    hwa_state;

    (void)R_PMA_GetHwaState(g_pma_handle, id, &hwa_state);
    if (hwa_state.enReqState == R_PMA_HWA_RELEASED)
    {
        expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
    }
    else
    {
        expected_hwa_state.enReqState      = R_PMA_HWA_REQUESTED;
    }    
    pma_ret = R_PMA_ApplyResetHwa(g_pma_handle, id);
    if (R_PMA_RESULT_OK == pma_ret)
    {
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_APPLIED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, id, &expected_hwa_state, expected_reset_req, 1, 100, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf("(line:%d) Wait For State Error(pma_ret=%d) \n", __LINE__, pma_ret);
            }
        pma_ret = R_PMA_ReleaseResetHwa(g_pma_handle, id);
    }

    if (R_PMA_RESULT_OK != pma_ret)
    {
        TEST_LOG("(hwa_id = %d) pma_deassert_assert() Failed (result=%d)\n\n", id, pma_ret);
    }
    else
    {
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_RELEASED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, id, &expected_hwa_state, expected_reset_req, 1, 100, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf("(line:%d) Wait For State Error(pma_ret=%d) \n", __LINE__, pma_ret);
            }
        TEST_LOG("--(HWAID = %3d)---------- OK : ReleaseReset -> ApplyReset\n", id);fflush(NULL);
    }

}
void pma_assert(e_pma_hwa_id_t id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t     expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    st_pma_hwa_state_t    hwa_state;

    (void)R_PMA_GetHwaState(g_pma_handle, id, &hwa_state);
    if (hwa_state.enReqState == R_PMA_HWA_RELEASED)
    {
        expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
    }
    else
    {
        expected_hwa_state.enReqState      = R_PMA_HWA_REQUESTED;
    } 
    pma_ret = R_PMA_ApplyResetHwa(g_pma_handle, id);
    if (R_PMA_RESULT_OK != pma_ret)
    {
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_APPLIED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, id, &expected_hwa_state, expected_reset_req, 1, 100, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf("(line:%d) Wait For State Error(pma_ret=%d) \n", __LINE__, pma_ret);
            }
        TEST_LOG("(hwa_id = %d) pma_deassert_assert() Failed (result=%d)\n\n", id, pma_ret);
    }
    else
    {
        TEST_LOG("--(HWAID = %3d)---------- OK : ApplyReset\n", id);fflush(NULL);
    }

}

void pma_deassert(e_pma_hwa_id_t id)
{
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t     expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    st_pma_hwa_state_t    hwa_state;

    (void)R_PMA_GetHwaState(g_pma_handle, id, &hwa_state);
    if (hwa_state.enReqState == R_PMA_HWA_RELEASED)
    {
        expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
    }
    else
    {
        expected_hwa_state.enReqState      = R_PMA_HWA_REQUESTED;
    }
    pma_ret = R_PMA_ReleaseResetHwa(g_pma_handle, id);
    if (R_PMA_RESULT_OK != pma_ret)
    {
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_RELEASED_RESET;
            pma_ret = R_PMA_WaitForState(g_pma_handle, id, &expected_hwa_state, expected_reset_req, 1, 100, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                printf("(line:%d) Wait For State Error(pma_ret=%d) \n", __LINE__, pma_ret);
            }
        TEST_LOG("(hwa_id = %d) pma_deassert() Failed (result=%d)\n\n", id, pma_ret);
    }
    else
    {
        TEST_LOG("--(HWAID = %3d)---------- OK : Set ReleaseReset \n", id);fflush(NULL);
    }

}

void pma_ng_assert(e_pma_hwa_id_t id)
{
    e_pma_return_t        pma_ret;

    pma_ret = R_PMA_ApplyResetHwa(g_pma_handle, id);
    if (R_PMA_RESULT_OK != pma_ret)
    {
        TEST_LOG("(hwa_id = %d) pma_ng_assert() Failed (result=%d)\n", id, pma_ret);
    }
    else
    {
        TEST_LOG("--(HWAID = %3d)---------- OK : Set ApplyRese\n", id);fflush(NULL);
    }
}

void pma_ng_deassert(e_pma_hwa_id_t id)
{
    e_pma_return_t        pma_ret;

    pma_ret = R_PMA_ReleaseResetHwa(g_pma_handle, id);
    if (R_PMA_RESULT_OK != pma_ret)
    {
        TEST_LOG("(hwa_id = %d) pma_ng_deassert() Failed (result=%d)\n\n", id, pma_ret);
    }
    else
    {
        TEST_LOG("--(HWAID = %3d)---------- OK : Set ReleaseReset \n", id);fflush(NULL);
    }

}
int pma_reset_status_check(e_pma_hwa_id_t id, e_pma_reset_req_t *pRstStat)
{
    int result = TEST_OK;
    e_pma_return_t pma_ret;

    pma_ret = R_PMA_GetReset(g_pma_handle, id, pRstStat);
    if(R_PMA_RESULT_OK != pma_ret)
    {
        result = TEST_NG;
        TEST_LOG("(Return Value = %d) NG : Test results NG. \n", result);
        TEST_LOG("(pma_ret = %d) \n", pma_ret);
    }
    else if (test_result == *pRstStat)
    {
        TEST_LOG("(test_result = %d) \n", test_result);
        TEST_LOG("(Reset Status = %d) OK : Test results were matched. \n", *pRstStat);
    }
    else
    {
        result = TEST_NG;
        TEST_LOG("(test_result = %d) \n", test_result);
        TEST_LOG("(Reset Status = %d) NG : Test results were mismatched. \n", *pRstStat);
    }
    return result;
}

int pma_reset_type_test_v4h(e_pma_hwa_id_t hwa_id, int test_num)
{
    int result = TEST_OK;
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    e_pma_reset_req_t     pRstStat;
    e_pma_hwa_id_t        id;
    e_pma_power_policy_t  hwa_policy;

    /* Dependent HWA test */
    if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) || (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
    {
        if (((CHILDREN_DEASSERT_ASSERT == test_num) || (CHILDREN_INITIALIZE_DEASSERT == test_num))
            && ((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
            )
        {
            if(CHILDREN_DEASSERT_ASSERT == test_num)
            {
                if(callcount == 0)
                {
                    TEST_LOG("--------------- Test Prerequisite : all HP Mode and Release Reset ---------------\n");
                    callcount++;
                }
                pma_deassert_assert(hwa_id);
                result = pma_reset_status_check(hwa_id, &pRstStat);
            }
            else
            {
                if(callcount == 0)
                {
                    TEST_LOG("--------------- Test Prerequisite : A1(HP Mode and Reset) A2,A3(HP Mode and Release Reset)  ---------------\n");
                    callcount++;
                }
                pma_deassert(hwa_id);
                result = pma_reset_status_check(hwa_id, &pRstStat);
            }
        }

        else if (((PARENT_DEASSERT_ASSERT == test_num) || (PARENT_INITIALIZE_DEASSERT == test_num))
            && ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)))
        {
            if(PARENT_DEASSERT_ASSERT == test_num)
            {
                if(callcount == 0)
                {
                    TEST_LOG("--------------- Test Prerequisite : A1(HP Mode and Reset) A2,A3(HP Mode and Release Reset)  ---------------\n");
                    callcount++;
                }
                pma_deassert_assert(hwa_id);
                result = pma_reset_status_check(hwa_id, &pRstStat);
            }
            else
            {
                if(callcount == 0)
                {
                    TEST_LOG("--------------- Test Prerequisite : A1,A2(HP Mode and Reset) A3(HP Mode and Release Reset)  ---------------\n");
                    callcount++;
                }
                pma_deassert(hwa_id);
                result = pma_reset_status_check(hwa_id, &pRstStat);
            }
        }

        else if (((GRAND_PARENT_DEASSERT_ASSERT == test_num) || (GRAND_PARENT_INITIALIZE_DEASSERT == test_num))
            && ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
            )
        {
            if(GRAND_PARENT_DEASSERT_ASSERT == test_num)
            {
                if(callcount == 0)
                {
                    TEST_LOG("--------------- Test Prerequisite : A1,A2(HP Mode and Reset) A3(HP Mode and Release Reset)  ---------------\n");
                    callcount++;

                }
                pma_deassert_assert(hwa_id);
                result = pma_reset_status_check(hwa_id, &pRstStat);
            }
            else
            {
                if(callcount == 0)
                {
                    TEST_LOG("--------------- Test Prerequisite : all HP Mode and Reset ---------------\n");
                    callcount++;
                }
                test_result = R_PMA_RELEASED_RESET;
                pma_deassert(hwa_id);
                result = pma_reset_status_check(hwa_id, &pRstStat);
            }
        }
    }
    /*  No dependencies */
    else
    {
        if ((NO_DEP_HWA_DEASSERT_ASSERT == test_num) || (NO_DEP_HWA_INITIALIZE_DEASSERT == test_num))
        {
            if(NO_DEP_HWA_DEASSERT_ASSERT == test_num)
            {
                TEST_LOG("Before PG mode\n");
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE*/ 
                if (R_PMA_PG_MODE != hwa_policy)
                {
                    TEST_LOG("Chage PG mode\n");
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_PG_MODE);
                    delay(1);
                }
                if(callcount == 0)
                {
                    TEST_LOG("--------------- Test Prerequisite : R_PMA_SYSC_DOMAIN_ID_NONE(HP Mode and Release Reset)  ---------------\n");
                    callcount++;
                }
                TEST_LOG("After PG mode\n");
                test_result = R_PMA_RELEASED_RESET;
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                DISPLAY_HWA_STATE(hwa_id, hwa_state, NO_DISP_LOW_POW);
                TEST_LOG("(reset status = %d)\n", pRstStat);

                pma_assert_deassert(hwa_id);

                result = pma_reset_status_check(hwa_id, &pRstStat);
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                DISPLAY_HWA_STATE(hwa_id, hwa_state, NO_DISP_LOW_POW);
                TEST_LOG("(reset status = %d)\n", pRstStat);
            }
            else
            {
                if(callcount == 0)
                {
                    TEST_LOG("--------------- Test Prerequisite : R_PMA_SYSC_DOMAIN_ID_NONE(HP Mode and Reset)  ---------------\n");
                    callcount++;
                }
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                DISPLAY_HWA_STATE(hwa_id, hwa_state, NO_DISP_LOW_POW);
                TEST_LOG("(reset status = %d)\n", pRstStat);

                pma_deassert(hwa_id);

                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                DISPLAY_HWA_STATE(hwa_id, hwa_state, NO_DISP_LOW_POW);
                TEST_LOG("(reset status = %d)\n", pRstStat);
                result = pma_reset_status_check(hwa_id, &pRstStat);
            }
        }
    }
    return result;
}

void pma_reset_type_test_s4(e_pma_hwa_id_t hwa_id, int test_num)
{
    e_pma_return_t        pma_ret;

    if (R_PMA_HWA_ID_S4_SHIP_S0 == hwa_id || R_PMA_HWA_ID_S4_SHIP_S1 == hwa_id || R_PMA_HWA_ID_S4_SHIP_S2 == hwa_id)
    {
        TEST_LOG("--(HWAID = %3d)---------- OK : No Software Reset Register \n", hwa_id);fflush(NULL);
        pma_ret = R_PMA_RESULT_OK;
    }
    else
    {
        pma_ret = R_PMA_ApplyResetHwa(g_pma_handle, hwa_id);
        delay(5);
        if (R_PMA_RESULT_OK == pma_ret)
        {
            pma_ret = R_PMA_ReleaseResetHwa(g_pma_handle, hwa_id);
            delay(5);
            if (R_PMA_RESULT_OK == pma_ret)
            {
                TEST_LOG("--(HWAID = %3d)---------- OK : ApplyReset -> ReleaseReset\n", hwa_id);fflush(NULL);
            }
            else
            {
                TEST_LOG("(hwa_id = %d) R_PMA_ReleaseResetHwa() Failed (result=%d)\n\n", hwa_id, pma_ret);
            }
        }
        else
        {
            TEST_LOG("(hwa_id = %d) R_PMA_ApplyResetHwa() Failed (result=%d)\n\n", hwa_id, pma_ret);
        }
    }
}
void a2child_cg_release(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=5;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    /* A3 HPmodeÅ®A2 HPmodeÅ®A1 reset statusÅ®A2 CGmodeÅ®A3 CGmode */
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            hwa_id = g_pma_test_reset_hwa_id[id];
            if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))  && (HWA_change_count == 1))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 2))
            {
                pma_assert(hwa_id);
            }
        }
    }
}

void return_a2child_cg_reset(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=5;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    /* A3 release statusÅ®A2 HPmodeÅ®A2 release statusÅ®A2 CGmodeÅ®A3 CGmode*/
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            if(HWA_change_count != 3)
            {
                hwa_id = g_pma_test_reset_hwa_id[id];
                if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
                {
                    pma_deassert(hwa_id);
                }
                else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 2))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not HG, Set HP-MODE */
                    if (R_PMA_HP_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                        delay(1);
                    }
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 4))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
                else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent) && (HWA_change_count == 5))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
            }
            else
            {
                hwa_id = g_pma_test_reset_hwa_id_reverse[id];
                if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
                {
                    pma_deassert(hwa_id);
                }
            }
        }
    }
}

void a2child_cg_reset(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max = 5;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    /* A3 HPmodeÅ®A2 HPmodeÅ®A2 reset statusÅ®A2 CGmodeÅ®A3 reset status*/
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            hwa_id = g_pma_test_reset_hwa_id[id];
            if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 2))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 3))
            {
                pma_assert(hwa_id);
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 4))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 5))
            {
                pma_assert(hwa_id);
            }
        }
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            hwa_id = g_pma_test_reset_hwa_id[id];
            /* Prepare for test */

            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, NO_DISP_LOW_POW);
            TEST_LOG("(reset status = %d)\n", pRstStat);
        }
    }
}

void return_child_cg_release(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=6;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    /* A3 HPmodeÅ®A3 release statusÅ®A2 HPmodeÅ®A2 release statusÅ®A2 CGmodeÅ®A3 CGmode*/
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            if(HWA_change_count != 4)
            {
                hwa_id = g_pma_test_reset_hwa_id[id];
                if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not HP, Set HP-MODE */
                    if (R_PMA_HP_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                        delay(1);
                    }
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 2))
                {
                    pma_deassert(hwa_id);
                }
                else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 3))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not HG, Set HP-MODE */
                    if (R_PMA_HP_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                        delay(1);
                    }
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 5))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
                else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 6))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
            }
            else
            {
                hwa_id = g_pma_test_reset_hwa_id_reverse[id];
                if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
                {
                    pma_deassert(hwa_id);
                }
            }
        }
    }
}

void child_cg_release(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=7;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
        e_pma_reset_req_t     pRstStat;

    /* A3 HPmodeÅ®A2 HPmodeÅ®A2 reset statusÅ®A3 reset statusÅ®A2 CGmodeÅ®A3 CGmode*/
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {

            if(HWA_change_count != 2)
            {
                hwa_id = g_pma_test_reset_hwa_id[id];
                if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not HP, Set HP-MODE */
                    if (R_PMA_HP_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                        delay(1);
                    }
                }
                else if (HWA_change_count == 3)
                {

                }
                else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 4))
                {
                    pma_assert(hwa_id);
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 5))
                {
                    pma_assert(hwa_id);
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 6))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 7))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
            }
            else 
            {
                hwa_id = g_pma_test_reset_hwa_id_reverse[id];
                if((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 2))
                {
                        (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                        (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
                    /* If Power Policy is not HP, Set HP-MODE */
                    if (R_PMA_HP_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                        delay(1);
                    }
                }
            }
        }
    }
}

void return_all_cg_reset(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=7;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    /* A3 HPmodeÅ®A3 release statusÅ®A2 HPmodeÅ®A2 release statusÅ®A1 release statusÅ®A2 CGmodeÅ®A3 CGmode*/
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            if(HWA_change_count !=4 )
            {
                hwa_id = g_pma_test_reset_hwa_id[id];
                if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not HP, Set HP-MODE */
                    if (R_PMA_HP_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                        delay(1);
                    }
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 2))
                {
                    pma_deassert(hwa_id);
                }
                else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 3))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not HP, Set HP-MODE */
                    if (R_PMA_HP_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                        delay(1);
                    }
                }
                else if ((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 5))
                {
                    pma_deassert(hwa_id);
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 6))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
                else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))  && (HWA_change_count == 7))
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
            }
            hwa_id = g_pma_test_reset_hwa_id_reverse[id];
            if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 4))
            {
                pma_deassert(hwa_id);
            }
        }
    }
}

void all_cg_reset(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=7;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    /* A3 HPmodeÅ®A2 HPmodeÅ®A1 reset statusÅ®A2 reset statusÅ®A3 reset statusÅ®A2 CGmodeÅ®A3 CGmode*/
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            hwa_id = g_pma_test_reset_hwa_id[id];
            if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 2))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if (((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 3))
            {
                pma_assert(hwa_id);

            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 4))
            {
                pma_assert(hwa_id);
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 5))
            {
                pma_assert(hwa_id);

            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 6))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
        else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 7))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
        }
    }
}

void return_child_cg_reset(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max = 5;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;

    /* A3 HPmodeÅ®A2 HPmodeÅ®A1 release reset statusÅ®A2 CGmodeÅ®A3 CGmode */
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            hwa_id = g_pma_test_reset_hwa_id[id];
            if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 2))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not PG, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if ((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 3))
            {
                pma_deassert(hwa_id);
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 4))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 5))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
        }
    }
}

void child_cg_reset(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=5;

    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    /* A3 HPmodeÅ®A2 HPmodeÅ®A1 reset statusÅ®A2 CGmodeÅ®A3 CGmode */
    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            hwa_id = g_pma_test_reset_hwa_id[id];
            if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 2))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if ((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 3))
            {
                pma_assert(hwa_id);
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 4))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))&& (HWA_change_count == 5))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not PG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
        }
    }
}

void all_hp_release(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=9;

    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max;HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {

            hwa_id = g_pma_test_reset_hwa_id[id];
            if (((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 2))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 3))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 4))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                pma_ret = R_PMA_ReleaseResetHwa(g_pma_handle,hwa_id);
                delay(1);
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 5))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not HP, Set HP-MODE */
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 6))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                pma_ret = R_PMA_ReleaseResetHwa(g_pma_handle,hwa_id);
                delay(1);
            }
            else if ((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 7))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
            else if ((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 8))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                pma_ret = R_PMA_ReleaseResetHwa(g_pma_handle,hwa_id);
                delay(1);
            }
            else if ((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)&& (HWA_change_count == 9))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                if (R_PMA_HP_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
                    delay(1);
                }
            }
        }
    }
}
void all_cg_release(void)
{
    int result = TEST_OK;
    e_pma_hwa_id_t        HWA_change_count_max=4;

    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_hwa_id_t        HWA_change_count;
    e_pma_reset_req_t     pRstStat;

    for (HWA_change_count = 1; HWA_change_count <= HWA_change_count_max; HWA_change_count++)
    {
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            hwa_id = g_pma_test_reset_hwa_id[id];
            if (((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 1))
            {
#if defined(RCAR_V4H)
                if (R_PMA_HWA_ID_V4H_VDSP1_REG == hwa_id || R_PMA_HWA_ID_V4H_VDSP1_CSB == hwa_id || R_PMA_HWA_ID_V4H_VDSP0_SYSPO == hwa_id || R_PMA_HWA_ID_V4H_VDSP0_APBDBG == hwa_id || R_PMA_HWA_ID_V4H_VDSP0_OCEM == hwa_id || 
                    R_PMA_HWA_ID_V4H_VDSP0_CORE == hwa_id || R_PMA_HWA_ID_V4H_VDSP0_SYS == hwa_id || R_PMA_HWA_ID_V4H_VDSP0_GLOBAL == hwa_id || R_PMA_HWA_ID_V4H_VDSP0_REG == hwa_id || R_PMA_HWA_ID_V4H_VDSP0_CSB == hwa_id || 
                    R_PMA_HWA_ID_V4H_VDSP3_BUS == hwa_id || R_PMA_HWA_ID_V4H_VDSP2_BUS == hwa_id || R_PMA_HWA_ID_V4H_VDSP1_BUS == hwa_id || R_PMA_HWA_ID_V4H_VDSP0_BUS == hwa_id || 
                    R_PMA_HWA_ID_V4H_VDSP3_SYSPO == hwa_id || R_PMA_HWA_ID_V4H_VDSP3_APBDBG == hwa_id || R_PMA_HWA_ID_V4H_VDSP3_OCEM == hwa_id || R_PMA_HWA_ID_V4H_VDSP3_CORE == hwa_id || 
                    R_PMA_HWA_ID_V4H_VDSP3_SYS == hwa_id || R_PMA_HWA_ID_V4H_VDSP3_GLOBAL == hwa_id || R_PMA_HWA_ID_V4H_VDSP3_REG == hwa_id ||  R_PMA_HWA_ID_V4H_VDSP3_CSB == hwa_id || R_PMA_HWA_ID_V4H_VDSP2_SYSPO == hwa_id || 
                    R_PMA_HWA_ID_V4H_VDSP2_APBDBG == hwa_id || R_PMA_HWA_ID_V4H_VDSP2_OCEM == hwa_id || R_PMA_HWA_ID_V4H_VDSP2_CORE == hwa_id || R_PMA_HWA_ID_V4H_VDSP2_SYS == hwa_id || 
                    R_PMA_HWA_ID_V4H_VDSP2_GLOBAL == hwa_id || R_PMA_HWA_ID_V4H_VDSP2_REG == hwa_id || R_PMA_HWA_ID_V4H_VDSP2_CSB == hwa_id || R_PMA_HWA_ID_V4H_VDSP1_SYSPO == hwa_id || R_PMA_HWA_ID_V4H_VDSP1_APBDBG == hwa_id || 
                    R_PMA_HWA_ID_V4H_VDSP1_OCEM == hwa_id || R_PMA_HWA_ID_V4H_VDSP1_CORE == hwa_id || R_PMA_HWA_ID_V4H_VDSP1_SYS == hwa_id || R_PMA_HWA_ID_V4H_VDSP1_GLOBAL == hwa_id)
#elif defined(RCAR_V4M)
                if (R_PMA_HWA_ID_V4M_VDSP1_REG == hwa_id || R_PMA_HWA_ID_V4M_VDSP1_CSB == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_SYSPO == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_APBDBG == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_OCEM == hwa_id ||
                    R_PMA_HWA_ID_V4M_VDSP0_CORE == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_SYS == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_GLOBAL == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_REG == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_CSB == hwa_id ||
                    R_PMA_HWA_ID_V4M_VDSP1_BUS == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_BUS == hwa_id || R_PMA_HWA_ID_V4M_VDSP1_SYSPO == hwa_id || R_PMA_HWA_ID_V4M_VDSP1_APBDBG == hwa_id || R_PMA_HWA_ID_V4M_VDSP1_OCEM == hwa_id ||
                    R_PMA_HWA_ID_V4M_VDSP1_CORE == hwa_id || R_PMA_HWA_ID_V4M_VDSP1_SYS == hwa_id || R_PMA_HWA_ID_V4M_VDSP1_GLOBAL == hwa_id || R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET == hwa_id || R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET == hwa_id)
#elif defined(RCAR_S4)
                if (0)
#endif
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
                    /* If Power Policy is not PG, Set CG-MODE */
                    if (R_PMA_PG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_PG_MODE);
                        delay(1);
                    }
                }
                else
                {
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
                    /* If Power Policy is not CG, Set CG-MODE */
                    if (R_PMA_CG_MODE != hwa_policy)
                    {
                        (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                        delay(1);
                    }
                }
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))&& (HWA_change_count == 2))
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);
                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
            else if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) && (HWA_change_count == 3) )
            {
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

                /* If Power Policy is not CG, Set CG-MODE */
                if (R_PMA_CG_MODE != hwa_policy)
                {
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_CG_MODE);
                    delay(1);
                }
            }
        }
    }
}
int pma_ng_a2child_release(e_pma_hwa_id_t hwa_id, int test_num)
{
    int result = TEST_OK;
    e_pma_reset_req_t     pRstStat;
    st_pma_hwa_state_t    hwa_state;
    e_pma_hwa_id_t        id;
    test_result = R_PMA_RELEASED_RESET;

    if(callcount == 0)
    {
        TEST_LOG("--------------- Test Prerequisite : A2(CG Mode and Release Reset) A3(HP Mode and Reset) ---------------\n");
        a2child_cg_release();
        callcount++;
    }
    pma_ng_assert(hwa_id);

    result = pma_reset_status_check(hwa_id, &pRstStat);
    return result;
}

int pma_ng_a2child_reset(e_pma_hwa_id_t hwa_id, int test_num)
{
    int result = TEST_OK;
    e_pma_reset_req_t     pRstStat;
    st_pma_hwa_state_t    hwa_state;
    e_pma_hwa_id_t        id;
    test_result = R_PMA_RELEASED_RESET;

    if(callcount == 0)
    {
        TEST_LOG("--------------- Test Prerequisite : A2(CG Mode and Release Reset) A3(HP Mode and Reset) ---------------\n");
        a2child_cg_reset();
        callcount++;
    }
    pma_ng_deassert(hwa_id);

    result = pma_reset_status_check(hwa_id, &pRstStat);
    return result;
}

int pma_ng_parent_reset(e_pma_hwa_id_t hwa_id, int test_num)
{
    int result = TEST_OK;
    e_pma_reset_req_t     pRstStat;
    st_pma_hwa_state_t    hwa_state;
    e_pma_hwa_id_t        id;
    test_result = R_PMA_RELEASED_RESET;

    if(ONLY_PARENT_NOT_CHANGE_ASSERT_1 == test_num)
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : All CG Mode and Release Reset ---------------\n");
            callcount++;
        }
        pma_ng_assert(hwa_id);
    }
    else
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : A1(CG Mode and Release Reset) A2,A3(CG Mode and Reset)  ---------------\n");
            child_cg_release();
            pma_ng_assert(hwa_id);
            callcount++;
        }
        else
        {
            pma_ng_assert(hwa_id);
        }
    }
    result = pma_reset_status_check(hwa_id, &pRstStat);
    return result;
}

int pma_ng_parent_release_reset(e_pma_hwa_id_t hwa_id, int test_num)
{
    int result = TEST_OK;
    e_pma_reset_req_t     pRstStat;
    e_pma_hwa_id_t        id;
    st_pma_hwa_state_t    hwa_state;

    test_result = R_PMA_APPLIED_RESET;

    if (ONLY_PARENT_NOT_CHANGE_DEASSERT_1 == test_num)
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : All CG Mode and Reset  ---------------\n");
            all_cg_reset();
            pma_ng_deassert(hwa_id);
            callcount++;
        }
        else
        {
            pma_ng_deassert(hwa_id);
        }
    }
    else
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : A1(CG Mode and Release Reset) A2,A3(CG Mode and Reset)  ---------------\n");
            child_cg_reset();
            pma_ng_deassert(hwa_id);
            callcount++;
        }
        else
        {
            pma_ng_deassert(hwa_id);
        }
    }
    result = pma_reset_status_check(hwa_id, &pRstStat);
    return result;
}

int pma_ng_child_and_parent_reset(e_pma_hwa_id_t hwa_id, int test_num)
{
    int result = TEST_OK;
    e_pma_reset_req_t     pRstStat;

    if(CHILDREN_AND_PARENT_NOT_CHANGE_ASSERT_1 == test_num)
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : all HP Mode and Release Reset ---------------\n");
            callcount++;
        }
        pma_ng_assert(hwa_id);
    }
    else if (CHILDREN_AND_PARENT_NOT_CHANGE_ASSERT_2 == test_num)
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : all CG Mode and Release Reset ---------------\nRst");
            all_cg_release();
            pma_ng_assert(hwa_id);
            callcount++;
        }
        else
        {
            pma_ng_assert(hwa_id);
        }
    }
    else
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : A1(CG Mode and Reset) A2,A3(CG Mode and Release Reset)  ---------------\n");
            child_cg_reset();
            pma_ng_assert(hwa_id);
            callcount++;
        }
        else
        {
            pma_ng_assert(hwa_id);
        }
    }
    result = pma_reset_status_check(hwa_id, &pRstStat);
    return result;
}

int pma_ng_child_and_parent_release_reset(e_pma_hwa_id_t hwa_id, int test_num)
{
    int result = TEST_OK;
    e_pma_reset_req_t     pRstStat;

    if (CHILDREN_AND_PARENT_NOT_CHANGE_DEASSERT_1 == test_num)
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : All CG Mode and Reset  ---------------\n");
            all_cg_reset();
            test_result = R_PMA_APPLIED_RESET;
            pma_ng_deassert(hwa_id);
            callcount++;
        }
        else
        {
            pma_ng_deassert(hwa_id);
        }
    }
    else
    {
        if(callcount == 0)
        {
            TEST_LOG("--------------- Test Prerequisite : A1(CG Mode and Release Reset) A2,A3(CG Mode and Reset)  ---------------\n");

            child_cg_release();
            pma_ng_deassert(hwa_id);
            callcount++;
        }
        else
        {
            pma_ng_deassert(hwa_id);
        }
    }
    result = pma_reset_status_check(hwa_id, &pRstStat);
    return result;
}

int pma_ng_reset_type_test_v4h(e_pma_hwa_id_t hwa_id, int test_num)
{
    int result = TEST_OK;
    e_pma_return_t    pma_ret;
    e_pma_reset_req_t pRstStat;

    /* Dependent HWA test */
    if (((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
        && (ONLY_CHILDREN_NOT_CHANGE_ASSERT == test_num))
    {
        test_result = R_PMA_RELEASED_RESET;
        TEST_LOG("--------------- Test Prerequisite : all HP Mode and Release Reset ---------------\nRst");
        pma_ng_assert(hwa_id);
        result = pma_reset_status_check(hwa_id, &pRstStat);
    }
    else if(((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) 
        && ((CHILDREN_AND_PARENT_NOT_CHANGE_ASSERT_1 == test_num) || (CHILDREN_AND_PARENT_NOT_CHANGE_ASSERT_2 == test_num) || (CHILDREN_AND_PARENT_NOT_CHANGE_ASSERT_3 == test_num)))
    {
        result = pma_ng_child_and_parent_reset(hwa_id,test_num);
    }
    else if(((0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent)) 
        && ((CHILDREN_AND_PARENT_NOT_CHANGE_DEASSERT_1 == test_num) || (CHILDREN_AND_PARENT_NOT_CHANGE_DEASSERT_2 == test_num)))
    {
        result = pma_ng_child_and_parent_release_reset(hwa_id,test_num);
    }
    else if(((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
        && ((ONLY_PARENT_NOT_CHANGE_ASSERT_1 == test_num) || (ONLY_PARENT_NOT_CHANGE_ASSERT_2 == test_num)))
    {
#if defined(RCAR_V4H)
        if (R_PMA_HWA_ID_V4H_VDSP1_REG != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_CSB != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_SYSPO != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_APBDBG != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_OCEM != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP0_CORE != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_SYS != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_GLOBAL != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_REG != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_CSB != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP3_BUS != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_BUS != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_BUS != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_BUS != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP3_SYSPO != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_APBDBG != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_OCEM != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_CORE != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP3_SYS != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_GLOBAL != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_REG != hwa_id &&  R_PMA_HWA_ID_V4H_VDSP3_CSB != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_SYSPO != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP2_APBDBG != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_OCEM != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_CORE != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_SYS != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP2_GLOBAL != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_REG != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_CSB != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_SYSPO != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_APBDBG != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP1_OCEM != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_CORE != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_SYS != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_GLOBAL != hwa_id && R_PMA_HWA_ID_V4H_IMPDMAC1 != hwa_id &&
            R_PMA_HWA_ID_V4H_IMP3 != hwa_id && R_PMA_HWA_ID_V4H_IMP2 != hwa_id && R_PMA_HWA_ID_V4H_CVE3 != hwa_id && R_PMA_HWA_ID_V4H_CVE2 != hwa_id && R_PMA_HWA_ID_V4H_CVE1 != hwa_id && R_PMA_HWA_ID_V4H_CVE0 != hwa_id ) 
#elif defined(RCAR_V4M)
        if (R_PMA_HWA_ID_V4M_VDSP1_REG != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_CSB != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_SYSPO != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_APBDBG != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_OCEM != hwa_id &&
            R_PMA_HWA_ID_V4M_VDSP0_CORE != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_SYS != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_GLOBAL != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_REG != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_CSB != hwa_id &&
            R_PMA_HWA_ID_V4M_VDSP1_BUS != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_BUS != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_SYSPO != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_APBDBG != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_OCEM != hwa_id &&
            R_PMA_HWA_ID_V4M_VDSP1_CORE != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_SYS != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_GLOBAL != hwa_id && R_PMA_HWA_ID_V4M_IMPDMAC1 != hwa_id &&
            R_PMA_HWA_ID_V4M_CVE3 != hwa_id && R_PMA_HWA_ID_V4M_CVE2 != hwa_id && R_PMA_HWA_ID_V4M_CVE1 != hwa_id && R_PMA_HWA_ID_V4M_CVE0 != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET != hwa_id)
#elif defined(RCAR_S4)
        if (0)
#endif
        {
            result = pma_ng_parent_reset(hwa_id,test_num);
        }
    }
    else if(((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
        && ((ONLY_PARENT_NOT_CHANGE_DEASSERT_1 == test_num) || (ONLY_PARENT_NOT_CHANGE_DEASSERT_2 == test_num)))
    {
#if defined(RCAR_V4H)
        if (R_PMA_HWA_ID_V4H_VDSP1_REG != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_CSB != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_SYSPO != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_APBDBG != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_OCEM != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP0_CORE != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_SYS != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_GLOBAL != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_REG != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_CSB != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP3_BUS != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_BUS != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_BUS != hwa_id && R_PMA_HWA_ID_V4H_VDSP0_BUS != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP3_SYSPO != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_APBDBG != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_OCEM != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_CORE != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP3_SYS != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_GLOBAL != hwa_id && R_PMA_HWA_ID_V4H_VDSP3_REG != hwa_id &&  R_PMA_HWA_ID_V4H_VDSP3_CSB != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_SYSPO != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP2_APBDBG != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_OCEM != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_CORE != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_SYS != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP2_GLOBAL != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_REG != hwa_id && R_PMA_HWA_ID_V4H_VDSP2_CSB != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_SYSPO != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_APBDBG != hwa_id && 
            R_PMA_HWA_ID_V4H_VDSP1_OCEM != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_CORE != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_SYS != hwa_id && R_PMA_HWA_ID_V4H_VDSP1_GLOBAL != hwa_id && R_PMA_HWA_ID_V4H_IMPDMAC1 != hwa_id &&
            R_PMA_HWA_ID_V4H_IMP3 != hwa_id && R_PMA_HWA_ID_V4H_IMP2 != hwa_id && R_PMA_HWA_ID_V4H_CVE3 != hwa_id && R_PMA_HWA_ID_V4H_CVE2 != hwa_id && R_PMA_HWA_ID_V4H_CVE1 != hwa_id && R_PMA_HWA_ID_V4H_CVE0 != hwa_id ) 
#elif defined(RCAR_V4M)
        if (R_PMA_HWA_ID_V4M_VDSP1_REG != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_CSB != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_SYSPO != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_APBDBG != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_OCEM != hwa_id &&
            R_PMA_HWA_ID_V4M_VDSP0_CORE != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_SYS != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_GLOBAL != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_REG != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_CSB != hwa_id &&
            R_PMA_HWA_ID_V4M_VDSP1_BUS != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_BUS != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_SYSPO != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_APBDBG != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_OCEM != hwa_id &&
            R_PMA_HWA_ID_V4M_VDSP1_CORE != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_SYS != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_GLOBAL != hwa_id && R_PMA_HWA_ID_V4M_IMPDMAC1 != hwa_id &&
            R_PMA_HWA_ID_V4M_CVE3 != hwa_id && R_PMA_HWA_ID_V4M_CVE2 != hwa_id && R_PMA_HWA_ID_V4M_CVE1 != hwa_id && R_PMA_HWA_ID_V4M_CVE0 != hwa_id && R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET != hwa_id && R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET != hwa_id)
#elif defined(RCAR_S4)
        if (0)
#endif
        {
            result = pma_ng_parent_release_reset(hwa_id,test_num);
        }
    }
    else if(((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
        && (A2_ONLY_PARENT_NOT_CHANGE_ASSERT == test_num))
    {
#if defined(RCAR_V4H)
        if ( R_PMA_HWA_ID_V4H_IMPDMAC1 == hwa_id || R_PMA_HWA_ID_V4H_IMP3 == hwa_id || R_PMA_HWA_ID_V4H_IMP2 == hwa_id || R_PMA_HWA_ID_V4H_CVE3 == hwa_id || R_PMA_HWA_ID_V4H_CVE2 == hwa_id || R_PMA_HWA_ID_V4H_CVE1 == hwa_id || R_PMA_HWA_ID_V4H_CVE0 == hwa_id )
#elif defined(RCAR_V4M)
        if ( R_PMA_HWA_ID_V4M_IMPDMAC1 == hwa_id || R_PMA_HWA_ID_V4M_CVE3 == hwa_id || R_PMA_HWA_ID_V4M_CVE2 == hwa_id || R_PMA_HWA_ID_V4M_CVE1 == hwa_id || R_PMA_HWA_ID_V4M_CVE0 == hwa_id )
#elif defined(RCAR_S4)
        if (0)
#endif
        {
            result = pma_ng_a2child_reset(hwa_id,test_num);
        }
    }
    else if(((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U != g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
        && (A2_ONLY_PARENT_NOT_CHANGE_DEASSERT == test_num))
    {
#if defined(RCAR_V4H)
        if ( R_PMA_HWA_ID_V4H_IMPDMAC1 == hwa_id || R_PMA_HWA_ID_V4H_IMP3 == hwa_id || R_PMA_HWA_ID_V4H_IMP2 == hwa_id || R_PMA_HWA_ID_V4H_CVE3 == hwa_id || R_PMA_HWA_ID_V4H_CVE2 == hwa_id || R_PMA_HWA_ID_V4H_CVE1 == hwa_id || R_PMA_HWA_ID_V4H_CVE0 == hwa_id )
#elif defined(RCAR_V4M)
        if ( R_PMA_HWA_ID_V4M_IMPDMAC1 == hwa_id || R_PMA_HWA_ID_V4M_CVE3 == hwa_id || R_PMA_HWA_ID_V4M_CVE2 == hwa_id || R_PMA_HWA_ID_V4M_CVE1 == hwa_id || R_PMA_HWA_ID_V4M_CVE0 == hwa_id )
#elif defined(RCAR_S4)
        if (0)
#endif
        {
            result = pma_ng_a2child_release(hwa_id,test_num);
        }
    }
    return result;
}

/**********************************************************************************
  Function: pma_sequence_test
    - description:
       main routine of sequence test
**********************************************************************************/
int pma_reset_test(void)
{
    int result = TEST_OK;
    e_pma_return_t        pma_ret;
    st_pma_hwa_state_t    hwa_state;
    e_pma_power_policy_t  hwa_policy;
    st_pma_hwa_state_t    expected_hwa_state;
    e_pma_reset_req_t     expected_reset_req;
    e_pma_hwa_id_t        hwa_id;
    e_pma_hwa_id_t        id;
    e_pma_reset_req_t     pRstStat;
    for (id = 0; g_pma_test_no_sysc_hwa_id[id] < g_hwa_id_max_rst;id++)
    {
        hwa_id = g_pma_test_no_sysc_hwa_id[id];
        pma_ret = R_PMA_GetLockHwa(g_pma_handle, hwa_id);
        if (R_PMA_RESULT_OK != pma_ret)
        {
            TEST_LOG("(hwa_id = %d) R_PMA_lockHwa Failed\n", hwa_id);
            exchange_ret_code_to_str(pma_ret);
            return 0;
        }
    }
    TEST_LOG("------------------------------------------------------ Start ------------------------------------------------------\n");
    for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
    {
        hwa_id = g_pma_test_reset_hwa_id[id];
        /* Prepare for test */
        (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
        (void)R_PMA_GetPowerPolicy(g_pma_handle, hwa_id, &hwa_policy);

        /* If Power Policy is not HP, Set HP-MODE */
        if (R_PMA_HP_MODE != hwa_policy)
        {
            TEST_LOG("(hwa_id = %d) Change Power Policy to HP MODE\n", hwa_id);
            TEST_LOG("(g_pma_handle = %d) \n", g_pma_handle);
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            DISPLAY_HWA_STATE(hwa_id, hwa_state, NO_DISP_LOW_POW);
            delay(5);
            (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_HP_MODE);
            (void)R_PMA_ReleaseHwa(g_pma_handle, hwa_id);

            expected_hwa_state.enReqState      = R_PMA_HWA_RELEASED;
            expected_hwa_state.enActualClkStat = R_PMA_UNDEFINED_STATE;
            expected_hwa_state.enActualPwrStat = R_PMA_UNDEFINED_STATE;
            expected_reset_req                 = R_PMA_UNDEFINED_RESET;

            pma_ret = R_PMA_WaitForState(g_pma_handle, hwa_id, &expected_hwa_state, expected_reset_req, 1, 100, 0);
            if (R_PMA_RESULT_OK != pma_ret)
            {
                TEST_LOG("(hwa_id = %d) R_PMA_WaitForState not OK\n", hwa_id);
            }
        }
    }
    all_hp_release();
    /* All HWA is HP-MODE &  Release reset */
    /* reset type test */
    for (id = 0; g_pma_test_no_sysc_hwa_id[id] < g_hwa_id_max_rst; id++)
    {
        if ((0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].children) && (0U == g_pma_test_hw_dep_hwa_dependency_tbl[hwa_id].parent))
        {
                    TEST_LOG("Chage PG mode\n");
                    (void)R_PMA_SetPowerPolicy(g_pma_handle, hwa_id, R_PMA_PG_MODE);
                    delay(1);
        }
    }

    TEST_LOG("---------------------------------------------- reset type test start ----------------------------------------------\n\n");
    for (int test_num = 1; test_num <= g_test_case_rst ; test_num++)
    {
        callcount = 0;
        TEST_LOG("--(test number = %d)\n\n", test_num);
        if((test_num != 4) && (test_num != 6) && (test_num != 8))
        {
            for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst; id++)
            {
                hwa_id = g_pma_test_reset_hwa_id[id];
                result = g_pma_test_reset_func(hwa_id, test_num);

                if(result == TEST_NG)
                {
                    TEST_LOG("--(HWAID = %d)---------- Test Failed\n", hwa_id);
                    break;
                }
            }
        }
        else if((test_num == 4) || (test_num == 8))
        {
            for (id = 0; g_pma_test_no_sysc_hwa_id[id] < g_hwa_id_max_rst; id++)
            {
                hwa_id = g_pma_test_no_sysc_hwa_id[id];
                result =g_pma_test_reset_func(hwa_id, test_num);

                if(result == TEST_NG)
                {
                    TEST_LOG("--(HWAID = %d)---------- Test Failed\n", hwa_id);
                    break;
                }
            }
        }
        else
        {
            for (id = 0; g_pma_test_reset_hwa_id_reverse[id] < g_hwa_id_max_rst; id++)
            {
                hwa_id = g_pma_test_reset_hwa_id_reverse[id];
                result =g_pma_test_reset_func(hwa_id, test_num);

                if(result == TEST_NG)
                {
                    TEST_LOG_ERROR("--(HWAID = %d)---------- Test Failed\n", hwa_id);
                    break;
                }
            }
        }
        TEST_LOG("\n--------------- Testnum = %d Finish :  ---------------\n",test_num);
        for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
        {
            hwa_id = g_pma_test_reset_hwa_id[id];
            (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
            (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
            DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);

        }
        if(result == TEST_NG)
        {
            TEST_LOG_ERROR("--(HWAID = %d)---------- Test Failed\n", hwa_id);
            break;
        }
    }
#if defined(RCAR_V4H) || defined(RCAR_V4M)
    if(result != TEST_NG)
    {
        /* All HWA is HP-MODE &  Release reset */
        for (int test_num = 9; test_num <= g_ng_test_case_rst ; test_num++)
        {
            callcount = 0;
            if(CHILDREN_AND_PARENT_NOT_CHANGE_DEASSERT_1 == test_num)
            {
                TEST_LOG("--------------- Change All CG Release Reset mode  ---------------\n");
                return_child_cg_reset();
                TEST_LOG("--------------- Change All CG Release Reset Mode Finish  ---------------\n");
                for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
                {
                    hwa_id = g_pma_test_reset_hwa_id[id];

                    /* Prepare for test */
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                    DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG_INFO("(reset status = %d)\n", pRstStat);
                    delay(1);
                }
            }
            else if(CHILDREN_AND_PARENT_NOT_CHANGE_DEASSERT_2 == test_num)
            {
                TEST_LOG("--------------- Change All CG Release Reset mode  ---------------\n");
                return_all_cg_reset();
                TEST_LOG("--------------- Change All CG Release Reset Mode Finish  ---------------\n");
                for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
                {
                    hwa_id = g_pma_test_reset_hwa_id[id];
                    /* Prepare for test */
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                    DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG_INFO("(reset status = %d)\n", pRstStat);
                    delay(1);
                }
            }
            else if(ONLY_PARENT_NOT_CHANGE_ASSERT_1 == test_num)
            {
                TEST_LOG("--------------- Change All CG Release Reset mode  ---------------\n");
                return_child_cg_release();
                TEST_LOG("--------------- Change All CG Release Reset Mode Finish  ---------------\n");
                for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
                {
                    hwa_id = g_pma_test_reset_hwa_id[id];
                    /* Prepare for test */
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                    DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG_INFO("(reset status = %d)\n", pRstStat);
                }
            }
            else if(ONLY_PARENT_NOT_CHANGE_DEASSERT_1 == test_num)
            {
                TEST_LOG("--------------- Change All CG Release Reset mode  ---------------\n");
                return_child_cg_release();
                TEST_LOG("--------------- Change All CG Release Reset Mode Finish  ---------------\n");
                for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
                {
                    hwa_id = g_pma_test_reset_hwa_id[id];
                    /* Prepare for test */
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                    DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG_INFO("(reset status = %d)\n", pRstStat);
                }
            }
            else if(ONLY_PARENT_NOT_CHANGE_DEASSERT_2 == test_num)
            {
                TEST_LOG("--------------- Change All CG Release Reset mode  ---------------\n");
                return_all_cg_reset();
                TEST_LOG("--------------- Change All CG Release Reset Mode Finish  ---------------\n");
                for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
                {
                    hwa_id = g_pma_test_reset_hwa_id[id];
                    /* Prepare for test */
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                    DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG_INFO("(reset status = %d)\n", pRstStat);
                }
            }
            else if(A2_ONLY_PARENT_NOT_CHANGE_ASSERT == test_num)
            {
                TEST_LOG("--------------- Change All CG Release Reset mode  ---------------\n");
                return_child_cg_reset();
                TEST_LOG("--------------- Change All CG Release Reset Mode Finish  ---------------\n");
                for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
                {
                    hwa_id = g_pma_test_reset_hwa_id[id];
                    /* Prepare for test */
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                    DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG_INFO("(reset status = %d)\n", pRstStat);
                }
            }
            else if(A2_ONLY_PARENT_NOT_CHANGE_DEASSERT == test_num)
            {
                TEST_LOG("--------------- Change All CG Release Reset mode  ---------------\n");
                return_a2child_cg_reset();
                TEST_LOG("--------------- Change All CG Release Reset Mode Finish  ---------------\n");
                for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
                {
                    hwa_id = g_pma_test_reset_hwa_id[id];
                    /* Prepare for test */
                    (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                    (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                    DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
                    TEST_LOG_INFO("(reset status = %d)\n", pRstStat);
                }
            }
            else
            {
                /* invalid test_num  */
            }

            TEST_LOG("\n--(test number = %d)\n\n", test_num);
            for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst; id++)
            {
                result = g_pma_ng_test_reset_func(g_pma_test_reset_hwa_id[id], test_num);

                if(result == TEST_NG)
                {
                    TEST_LOG_ERROR("--(HWAID = %d)---------- Test Failed\n", hwa_id);
                    break;
                }
            }
            if(result == TEST_NG)
            {
                TEST_LOG_ERROR("--(HWAID = %d)---------- Test Failed\n", hwa_id);
                break;
            }
            TEST_LOG("--------------- Testnum = %d Finish :  ---------------\n", test_num);
            for (id = 0; g_pma_test_reset_hwa_id[id] < g_hwa_id_max_rst;id++)
            {
                hwa_id = g_pma_test_reset_hwa_id[id];
                (void)R_PMA_GetHwaState(g_pma_handle, hwa_id, &hwa_state);
                (void)R_PMA_GetReset(g_pma_handle, hwa_id, &pRstStat);
                DISPLAY_HWA_STATE_INFO(hwa_id, hwa_state, NO_DISP_LOW_POW);
                TEST_LOG_INFO("(reset status = %d)\n", pRstStat);
            }
        }
    }
#endif
    if(TEST_OK != result)
    {
        TEST_LOG("---------------------------------------------- Failed ----------------------------------------------\n\n");
    }
    else
    {
        TEST_LOG("---------------------------------------------- Success ----------------------------------------------\n\n");
    }
    for (id = 0; g_pma_test_no_sysc_hwa_id[id] < g_hwa_id_max_rst;id++)
    {
        hwa_id = g_pma_test_no_sysc_hwa_id[id];
        pma_ret = R_PMA_UnlockHwa(g_pma_handle, hwa_id);
        if (R_PMA_RESULT_OK != pma_ret)
        {
            TEST_LOG("(hwa_id = %d) R_PMA_UnlockHwa Failed\n", hwa_id);
            exchange_ret_code_to_str(pma_ret);
            return 0;
        }
    }
    return result;
}
