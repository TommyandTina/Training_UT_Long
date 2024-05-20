
// /* impdrv_dspctl_set_state */
#define  impdrv_dspctl_set_state  impdrv_dspctl_set_state_(__LINE__)
#define  impdrv_dspctl_set_state_(line)  impdrv_dspctl_set_state__(line)
#define  impdrv_dspctl_set_state__(line)  impdrv_dspctl_set_state_ut_##line

#if !defined(RCAR_V4M)
#define  impdrv_dspctl_set_state_ut_1225  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_1569  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_1804  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_2237  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_2474  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_2551  impdrv_dspctl_set_state_stub
#endif
#if defined(RCAR_V4M)
#define  impdrv_dspctl_set_state_ut_1244  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_1606  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_1844  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_2277  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_2514  impdrv_dspctl_set_state
#define  impdrv_dspctl_set_state_ut_2591  impdrv_dspctl_set_state_stub
#endif
// /* impdrv_cmnctl_get_io_imptop */
#define  impdrv_cmnctl_get_io_imptop  impdrv_cmnctl_get_io_imptop_(__LINE__)
#define  impdrv_cmnctl_get_io_imptop_(line)  impdrv_cmnctl_get_io_imptop__(line)
#define  impdrv_cmnctl_get_io_imptop__(line)  impdrv_cmnctl_get_io_imptop_ut_##line
#if !defined(RCAR_V4M)
#define  impdrv_cmnctl_get_io_imptop_ut_3085   impdrv_cmnctl_get_io_imptop_stub
#endif
#if defined(RCAR_V4M)
#define  impdrv_cmnctl_get_io_imptop_ut_3123   impdrv_cmnctl_get_io_imptop_stub
#endif
/* impdrv_udefctl_udef_crc */
#define  impdrv_udefctl_udef_crc  impdrv_udefctl_udef_crc_(__LINE__)
#define  impdrv_udefctl_udef_crc_(line)  impdrv_udefctl_udef_crc__(line)
#define  impdrv_udefctl_udef_crc__(line)  impdrv_udefctl_udef_crc_ut_##line
#if defined(RCAR_V4M)
#define impdrv_udefctl_udef_crc_ut_4139  impdrv_udefctl_udef_crc_stub
#endif
#if !defined(RCAR_V4M)
#define impdrv_udefctl_udef_crc_ut_4101  impdrv_udefctl_udef_crc_stub
#endif
// #define  impdrv_udefctl_set_ecm   impdrv_udefctl_set_ecm_(__LINE__)
// #define  impdrv_udefctl_set_ecm_(line)  impdrv_udefctl_set_ecm__(line)
// #define  impdrv_udefctl_set_ecm__(line)  impdrv_udefctl_set_ecm_ut_##line

// #define impdrv_udefctl_set_ecm_ut_1672 impdrv_udefctl_set_ecm_stub
// #define impdrv_udefctl_set_ecm_ut_1765  impdrv_udefctl_set_ecm_stub
// #define impdrv_udefctl_set_ecm_ut_2214  impdrv_udefctl_set_ecm_stub

#include "r_impdrv_dspctl.c"

#undef impdrv_dspctl_set_state
#undef impdrv_cmnctl_get_io_imptop
