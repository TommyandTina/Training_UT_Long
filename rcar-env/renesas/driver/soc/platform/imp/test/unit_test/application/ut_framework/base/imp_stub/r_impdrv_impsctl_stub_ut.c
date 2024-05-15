// #if defined(RCAR_V3H2) | defined(RCAR_V3H1)

// /* impdrv_impsctl_get_inten_val */
// #define impdrv_impsctl_get_inten_val impdrv_impsctl_get_inten_val_(__LINE__)
// #define impdrv_impsctl_get_inten_val_(line) impdrv_impsctl_get_inten_val__(line)
// #define impdrv_impsctl_get_inten_val__(line) impdrv_impsctl_get_inten_val_ut_##line
// /* V3H2 */
// #define impdrv_impsctl_get_inten_val_ut_563 impdrv_impsctl_get_inten_val
// #define impdrv_impsctl_get_inten_val_ut_1446 impdrv_impsctl_get_inten_val_stub
// #define impdrv_impsctl_get_inten_val_ut_1613 impdrv_impsctl_get_inten_val_stub
// #define impdrv_impsctl_get_inten_val_ut_2266 impdrv_impsctl_get_inten_val
// /* V3H1 */
// #define impdrv_impsctl_get_inten_val_ut_583 impdrv_impsctl_get_inten_val
// #define impdrv_impsctl_get_inten_val_ut_1426 impdrv_impsctl_get_inten_val_stub
// #define impdrv_impsctl_get_inten_val_ut_1593 impdrv_impsctl_get_inten_val_stub
// #define impdrv_impsctl_get_inten_val_ut_2188 impdrv_impsctl_get_inten_val

// /* impdrv_impsctl_set_state */
// #define  impdrv_impsctl_set_state  impdrv_impsctl_set_state_(__LINE__)
// #define  impdrv_impsctl_set_state_(line)  impdrv_impsctl_set_state__(line)
// #define  impdrv_impsctl_set_state__(line)  impdrv_impsctl_set_state_ut_##line
// /* V3H2 */
// #define  impdrv_impsctl_set_state_ut_811  impdrv_impsctl_set_state
// #define  impdrv_impsctl_set_state_ut_1006  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1059  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1471  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1058  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1501  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1595  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1628  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_2399  impdrv_impsctl_set_state_stub
// /* V3H1 */
// #define  impdrv_impsctl_set_state_ut_791  impdrv_impsctl_set_state
// #define  impdrv_impsctl_set_state_ut_986  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1039  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1451  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1481  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1575  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_1608  impdrv_impsctl_set_state_stub
// #define  impdrv_impsctl_set_state_ut_2321  impdrv_impsctl_set_state_stub

// /* impdrv_osdep_memory_data_set */
// #define  impdrv_osdep_memory_data_set  impdrv_osdep_memory_data_set_(__LINE__)
// #define  impdrv_osdep_memory_data_set_(line)  impdrv_osdep_memory_data_set__(line)
// #define  impdrv_osdep_memory_data_set__(line)  impdrv_osdep_memory_data_set_ut_##line
// /* V3H2 */
// #define  impdrv_osdep_memory_data_set_ut_1839  impdrv_osdep_memory_data_set_stub
// /* V3H1 */
// #define  impdrv_osdep_memory_data_set_ut_1819  impdrv_osdep_memory_data_set_stub

// #include "r_impdrv_impsctl.c"

#undef impdrv_impsctl_get_inten_val
#undef impdrv_impsctl_set_state
#undef impdrv_osdep_memory_data_set

// #endif /* defined(RCAR_V3H2) | defined(RCAR_V3H1) */
