/* WinAMS_SPMC.h(v6.3) */
#include "winAMS_SpmcDefine.h"

#define WinAMS_SPMC_BOOL int

#ifdef WINAMS_SPMC_U4_INT	/* v6.3 */
typedef unsigned int  WinAMS_SPMC_U4;
#else
typedef unsigned long WinAMS_SPMC_U4;
#endif
typedef unsigned char WinAMS_SPMC_U1;
typedef unsigned short WinAMS_SPMC_U2;

/* v3.5 */
#if WINAMS_SPMC_USR_DEF_TFUNCNAME
#ifdef WinAMS_SPMC_const_funcname
typedef const WinAMS_SPMC_BASE_TFUNCNAME * WinAMS_SPMC_TFUNCNAME;
#else
typedef WinAMS_SPMC_BASE_TFUNCNAME * WinAMS_SPMC_TFUNCNAME;
#endif /* WinAMS_SPMC_const_funcname */
#else
#ifdef WinAMS_SPMC_const_funcname
typedef const char * WinAMS_SPMC_TFUNCNAME;
#else
typedef char * WinAMS_SPMC_TFUNCNAME;
#endif /* WinAMS_SPMC_const_funcname */
#endif /* WINAMS_SPMC_USR_DEF_TFUNCNAME */

#ifdef __cplusplus
#define WinAMS_SPMC_bool bool
extern "C" {
#endif  /* __cplusplus */

WinAMS_SPMC_BOOL WinAMS_SPMC_Exp(WinAMS_SPMC_U2 expID,WinAMS_SPMC_BOOL exp);
WinAMS_SPMC_BOOL WinAMS_SPMC_Clr(WinAMS_SPMC_U2 expcnt);

/* v3.2 */
WinAMS_SPMC_BOOL WinAMS_SPMC_Res(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 resID,WinAMS_SPMC_BOOL res,WinAMS_SPMC_U2 expcnt,WinAMS_SPMC_U4 blkID);

WinAMS_SPMC_BOOL WinAMS_SPMC_C1(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 blkID);
/* v6.3 */
#if WinAMS_SPMC_Init_arg_void
void WinAMS_SPMC_Init(void);
#else
void WinAMS_SPMC_Init();
#endif

WinAMS_SPMC_BOOL WinAMS_SPMC_C0(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 line);

void WinAMS_SPMC_CALL(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 callID);

#ifdef __cplusplus
}

WinAMS_SPMC_bool WinAMS_SPMC_Exp_PP(WinAMS_SPMC_U2 expID,WinAMS_SPMC_bool exp);
WinAMS_SPMC_bool WinAMS_SPMC_Clr_PP(WinAMS_SPMC_U2 expcnt);

WinAMS_SPMC_bool WinAMS_SPMC_Res_PP(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 resID,WinAMS_SPMC_bool res,WinAMS_SPMC_U2 expcnt,WinAMS_SPMC_U4 blkID);

WinAMS_SPMC_bool WinAMS_SPMC_C1_PP(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 blkID);

WinAMS_SPMC_bool WinAMS_SPMC_C0_PP(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 line);
#endif  /* __cplusplus */

/* v3.2 */
extern volatile WinAMS_SPMC_BOOL	WinAMS_SPMC_resVal;

extern volatile WinAMS_SPMC_U1	WinAMS_SPMC;

