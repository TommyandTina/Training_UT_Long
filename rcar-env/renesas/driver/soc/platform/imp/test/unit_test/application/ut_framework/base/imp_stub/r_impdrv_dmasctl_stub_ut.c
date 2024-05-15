#if defined(RCAR_V3H2) || defined(RCAR_V4H) || defined(RCAR_V4H2)

// /*impdrv_dmasctl_get_inten_val*/
#define impdrv_dmasctl_get_inten_val impdrv_dmasctl_get_inten_val_(__LINE__)
#define impdrv_dmasctl_get_inten_val_(line) impdrv_dmasctl_get_inten_val__(line)
#define impdrv_dmasctl_get_inten_val__(line) impdrv_dmasctl_get_inten_val_ut_##line

#define  impdrv_dmasctl_set_state  impdrv_dmasctl_set_state_(__LINE__)
#define  impdrv_dmasctl_set_state_(line)  impdrv_dmasctl_set_state__(line)
#define  impdrv_dmasctl_set_state__(line)  impdrv_dmasctl_set_state_ut_##line

#define impdrv_osdep_pow_on_imp impdrv_osdep_pow_on_imp_(__LINE__)
#define impdrv_osdep_pow_on_imp_(line) impdrv_osdep_pow_on_imp__(line)
#define impdrv_osdep_pow_on_imp__(line) impdrv_osdep_pow_on_imp_ut_##line

#define  impdrv_dmasctl_osal_core_num  impdrv_dmasctl_osal_core_num_(__LINE__)
#define  impdrv_dmasctl_osal_core_num_(line)  impdrv_dmasctl_osal_core_num__(line)
#define  impdrv_dmasctl_osal_core_num__(line)  impdrv_dmasctl_osal_core_num_ut_##line

#if defined(RCAR_V3H2) 
#define impdrv_dmasctl_get_inten_val_ut_383 impdrv_dmasctl_get_inten_val
#define impdrv_dmasctl_get_inten_val_ut_1323 impdrv_dmasctl_get_inten_val_stub
#define impdrv_dmasctl_get_inten_val_ut_1498 impdrv_dmasctl_get_inten_val_stub
#define impdrv_dmasctl_get_inten_val_ut_1641 impdrv_dmasctl_get_inten_val_stub
#define impdrv_dmasctl_get_inten_val_ut_2485 impdrv_dmasctl_get_inten_val

#define  impdrv_dmasctl_set_state_ut_526  impdrv_dmasctl_set_state
#define  impdrv_dmasctl_set_state_ut_800  impdrv_dmasctl_set_state
#define  impdrv_dmasctl_set_state_ut_873  impdrv_dmasctl_set_state
#define  impdrv_dmasctl_set_state_ut_1365  impdrv_dmasctl_set_state_stub
#define  impdrv_dmasctl_set_state_ut_1398  impdrv_dmasctl_set_state
#define  impdrv_dmasctl_set_state_ut_1534  impdrv_dmasctl_set_state_stub
#define  impdrv_dmasctl_set_state_ut_1613  impdrv_dmasctl_set_state_stub
#define  impdrv_dmasctl_set_state_ut_1672  impdrv_dmasctl_set_state

#define impdrv_osdep_pow_on_imp_ut_612 impdrv_osdep_pow_on_imp
#define impdrv_osdep_pow_on_imp_ut_773 impdrv_osdep_pow_on_imp_stub
#endif

#if defined(RCAR_V4H) || defined(RCAR_V4H2)
#define impdrv_dmasctl_get_inten_val_ut_438 impdrv_dmasctl_get_inten_val
#define impdrv_dmasctl_get_inten_val_ut_1509 impdrv_dmasctl_get_inten_val_stub
#define impdrv_dmasctl_get_inten_val_ut_1684 impdrv_dmasctl_get_inten_val_stub
#define impdrv_dmasctl_get_inten_val_ut_1833 impdrv_dmasctl_get_inten_val_stub //function for resume exe
#define impdrv_dmasctl_get_inten_val_ut_2699 impdrv_dmasctl_get_inten_val

#define  impdrv_dmasctl_set_state_ut_581  impdrv_dmasctl_set_state
#define  impdrv_dmasctl_set_state_ut_856  impdrv_dmasctl_set_state
#define  impdrv_dmasctl_set_state_ut_929  impdrv_dmasctl_set_state
#define  impdrv_dmasctl_set_state_ut_1551 impdrv_dmasctl_set_state_stub
#define  impdrv_dmasctl_set_state_ut_1584 impdrv_dmasctl_set_state
#define  impdrv_dmasctl_set_state_ut_1720 impdrv_dmasctl_set_state_stub
#define  impdrv_dmasctl_set_state_ut_1799 impdrv_dmasctl_set_state_stub //function for resume exe
#define  impdrv_dmasctl_set_state_ut_1864 impdrv_dmasctl_set_state

#define  impdrv_dmasctl_osal_core_num_ut_297  impdrv_dmasctl_osal_core_num
#define  impdrv_dmasctl_osal_core_num_ut_663  impdrv_dmasctl_osal_core_num_stub
#define  impdrv_dmasctl_osal_core_num_ut_825  impdrv_dmasctl_osal_core_num_stub
#define  impdrv_dmasctl_osal_core_num_ut_965  impdrv_dmasctl_osal_core_num_stub
#define  impdrv_dmasctl_osal_core_num_ut_1944 impdrv_dmasctl_osal_core_num_stub
#define  impdrv_dmasctl_osal_core_num_ut_2030 impdrv_dmasctl_osal_core_num_stub
#define  impdrv_dmasctl_osal_core_num_ut_2114 impdrv_dmasctl_osal_core_num_stub
#define  impdrv_dmasctl_osal_core_num_ut_2214 impdrv_dmasctl_osal_core_num

#define impdrv_osdep_pow_on_imp_ut_667 impdrv_osdep_pow_on_imp
#define impdrv_osdep_pow_on_imp_ut_829 impdrv_osdep_pow_on_imp_stub
#endif


// /* impdrv_osdep_pow_on_imp */
// /* V3H2 */
// /* V4H, V4H2 */

#include "r_impdrv_dmasctl.c"

#undef impdrv_dmasctl_get_inten_val
#undef impdrv_dmasctl_set_state
#undef impdrv_dmasctl_osal_core_num
#undef impdrv_osdep_pow_on_imp

#endif /* RCAR_V3H2 */