/*impdrv_pscctl_get_inten_val*/
#define impdrv_pscctl_get_inten_val impdrv_pscctl_get_inten_val_(__LINE__)
#define impdrv_pscctl_get_inten_val_(line) impdrv_pscctl_get_inten_val__(line)
#define impdrv_pscctl_get_inten_val__(line) impdrv_pscctl_get_inten_val_ut_##line
/* V3H1 */
// #define impdrv_pscctl_get_inten_val_ut_223 impdrv_pscctl_get_inten_val
// #define impdrv_pscctl_get_inten_val_ut_1019 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1141 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1260 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1656 impdrv_pscctl_get_inten_val
/* V3H2 */
// #define impdrv_pscctl_get_inten_val_ut_228 impdrv_pscctl_get_inten_val
// #define impdrv_pscctl_get_inten_val_ut_982 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1141 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1260 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1656 impdrv_pscctl_get_inten_val

/* V3M2 */
// #define impdrv_pscctl_get_inten_val_ut_223 impdrv_pscctl_get_inten_val
// #define impdrv_pscctl_get_inten_val_ut_976 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1135 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1254 impdrv_pscctl_get_inten_val_stub
// #define impdrv_pscctl_get_inten_val_ut_1650 impdrv_pscctl_get_inten_val
/* V4H, V4H2 */
#define impdrv_pscctl_get_inten_val_ut_270 impdrv_pscctl_get_inten_val
#define impdrv_pscctl_get_inten_val_ut_1197 impdrv_pscctl_get_inten_val_stub
#define impdrv_pscctl_get_inten_val_ut_1356 impdrv_pscctl_get_inten_val_stub
#define impdrv_pscctl_get_inten_val_ut_1482 impdrv_pscctl_get_inten_val_stub // stub of resume
#define impdrv_pscctl_get_inten_val_ut_1940 impdrv_pscctl_get_inten_val
// 
/*impdrv_pscctl_set_state*/
#define  impdrv_pscctl_set_state  impdrv_pscctl_set_state_(__LINE__)
#define  impdrv_pscctl_set_state_(line)  impdrv_pscctl_set_state__(line)
#define  impdrv_pscctl_set_state__(line)  impdrv_pscctl_set_state_ut_##line
/* V3H2, V3H1 */
// #define  impdrv_pscctl_set_state_ut_370  impdrv_pscctl_set_state
// #define  impdrv_pscctl_set_state_ut_552  impdrv_pscctl_set_state
// #define  impdrv_pscctl_set_state_ut_605  impdrv_pscctl_set_state
// #define  impdrv_pscctl_set_state_ut_1019  impdrv_pscctl_set_state_stub
// #define  impdrv_pscctl_set_state_ut_1048  impdrv_pscctl_set_state
// #define  impdrv_pscctl_set_state_ut_1172  impdrv_pscctl_set_state_stub
// #define  impdrv_pscctl_set_state_ut_1243  impdrv_pscctl_set_state_stub
// #define  impdrv_pscctl_set_state_ut_1288  impdrv_pscctl_set_state
/* V3M2 */
// #define  impdrv_pscctl_set_state_ut_365  impdrv_pscctl_set_state
// #define  impdrv_pscctl_set_state_ut_547  impdrv_pscctl_set_state
// #define  impdrv_pscctl_set_state_ut_600  impdrv_pscctl_set_state
// #define  impdrv_pscctl_set_state_ut_1013  impdrv_pscctl_set_state_stub
// #define  impdrv_pscctl_set_state_ut_1042  impdrv_pscctl_set_state
// #define  impdrv_pscctl_set_state_ut_1166  impdrv_pscctl_set_state_stub
// #define  impdrv_pscctl_set_state_ut_1237  impdrv_pscctl_set_state_stub
// #define  impdrv_pscctl_set_state_ut_1282  impdrv_pscctl_set_state
/* V4H, V4H2 */
#define  impdrv_pscctl_set_state_ut_412  impdrv_pscctl_set_state
#define  impdrv_pscctl_set_state_ut_594  impdrv_pscctl_set_state
#define  impdrv_pscctl_set_state_ut_647  impdrv_pscctl_set_state
#define  impdrv_pscctl_set_state_ut_1234  impdrv_pscctl_set_state_stub
#define  impdrv_pscctl_set_state_ut_1263  impdrv_pscctl_set_state
#define  impdrv_pscctl_set_state_ut_1387  impdrv_pscctl_set_state_stub
#define  impdrv_pscctl_set_state_ut_1458  impdrv_pscctl_set_state_stub//stub of resume
#define  impdrv_pscctl_set_state_ut_1510  impdrv_pscctl_set_state

#include "r_impdrv_pscctl.c"

#undef impdrv_pscctl_get_inten_val
#undef impdrv_pscctl_set_state
