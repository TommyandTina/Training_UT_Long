#ifndef R_PMA_FUNCTIONAL_TEST_SEQUENCE_H
#define R_PMA_FUNCTIONAL_TEST_SEQUENCE_H

/**********************************************************************************
  Public macro definition
**********************************************************************************/
#define TEST_SCENARIO_1   (0)
#define TEST_SCENARIO_2   (1)
#define TEST_SCENARIO_3   (2)
#define TEST_SCENARIO_4   (3)
#define TEST_SCENARIO_5   (4)
#define TEST_SCENARIO_6   (5)
#define TEST_SCENARIO_7   (6)
#define TEST_SCENARIO_8   (7)
#define TEST_SCENARIO_9   (8)
#define TEST_SCENARIO_10  (9)
#define TEST_SCENARIO_11  (10)
#define TEST_SCENARIO_12  (11)
#define TEST_SCENARIO_NUM (12)

/**********************************************************************************
  Public type definition
**********************************************************************************/
typedef struct
{
    st_pma_hwa_state_t  hwa_state;   /* Expected HWA status of each test */
    e_pma_reset_req_t   reset_req;   /* Expected HWA's reset status of each test */
    e_pma_power_state_t lowest_pow;  /* Expected HWA lowest power status of each test */
} st_state_t;

typedef struct
{
    st_state_t state[TEST_SCENARIO_NUM];
} st_expected_info_t;

typedef struct
{
    e_pma_hwa_id_t share_domain[15];
} st_share_domain_info_t;
/**********************************************************************************
  Function: pma_sequence_test
    - description:
       main routine of sequence test
**********************************************************************************/
int pma_sequence_test(void);

#endif  /* R_PMA_FUNCTIONAL_TEST_SEQUENCE_H */
