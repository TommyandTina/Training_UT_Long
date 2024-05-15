/* WinAMS_SPMC.c(v6.3) */
#include "winAMS_Spmc.h"

#define BOOL WinAMS_SPMC_BOOL

typedef WinAMS_SPMC_U4 U4;
typedef WinAMS_SPMC_U1 U1;
typedef WinAMS_SPMC_U2 U2;

volatile U1	WinAMS_SPMC;
volatile U4	WinAMS_SPMC_bits[WINAMS_SPMC_MCDC_MAXCONDNEST][(WINAMS_SPMC_MCDC_MAXCONDCNT+1)/16];
volatile U2	WinAMS_SPMC_nest;
volatile U2	WinAMS_SPMC_expcnt;
volatile U4	WinAMS_SPMC_ID;
volatile U4	WinAMS_SPMC_line;
volatile U4	WinAMS_SPMC_Blk_ID;

/* v3.5 */
#if THREE_BYTE_POINTER_USE	/* v6.0.3 */
unsigned long WinAMS_SPMC_funcname;
#else	/* THREE_BYTE_POINTER_USE */

#if WINAMS_SPMC_USR_DEF_TFUNCNAME		/* v3.5 */
#if __COMPILER_FCC907__
#ifdef WinAMS_SPMC_const_funcname
volatile const WinAMS_SPMC_BASE_TFUNCNAME	*WinAMS_SPMC_funcname;
#else
volatile WinAMS_SPMC_BASE_TFUNCNAME	*WinAMS_SPMC_funcname;
#endif /* WinAMS_SPMC_const_funcname */
#else
#ifdef WinAMS_SPMC_const_funcname
WinAMS_SPMC_BASE_TFUNCNAME	const *volatile WinAMS_SPMC_funcname;
#else
WinAMS_SPMC_BASE_TFUNCNAME	*volatile WinAMS_SPMC_funcname;
#endif /* WinAMS_SPMC_const_funcname */
#endif /* __COMPILER_FCC907__ */
#else
#if __COMPILER_FCC907__
#ifdef WinAMS_SPMC_const_funcname
volatile const char	*WinAMS_SPMC_funcname;
#else
volatile char	*WinAMS_SPMC_funcname;
#endif /* WinAMS_SPMC_const_funcname */
#else
#ifdef WinAMS_SPMC_const_funcname
char	const *volatile WinAMS_SPMC_funcname;
#else
char	*volatile WinAMS_SPMC_funcname;
#endif /* WinAMS_SPMC_const_funcname */
#endif /* __COMPILER_FCC907__ */
#endif /* WINAMS_SPMC_USR_DEF_TFUNCNAME */

#endif /* THREE_BYTE_POINTER_USE */

volatile BOOL	WinAMS_SPMC_resVal;

volatile U2	WinAMS_SPMC_maxCondCnt;
volatile U2	WinAMS_SPMC_maxCondNest;
volatile U1	WinAMS_SPMC_Lock;
/* v6.3 */
#if WinAMS_SPMC_Init_arg_void
void (*volatile WinAMS_SPMC_Init_ptr)(void) = WinAMS_SPMC_Init;
#else
void (*volatile WinAMS_SPMC_Init_ptr)() = WinAMS_SPMC_Init;
#endif


/* v6.3 */
#if WinAMS_SPMC_Init_arg_void
void WinAMS_SPMC_Init(void)
#else
void WinAMS_SPMC_Init()
#endif
{
	WinAMS_SPMC_maxCondCnt = WINAMS_SPMC_MCDC_MAXCONDCNT;
	WinAMS_SPMC_maxCondNest = WINAMS_SPMC_MCDC_MAXCONDNEST;
	WinAMS_SPMC_Lock = 0;
	WinAMS_SPMC_nest = 0;
	WinAMS_SPMC_resVal = 0;
}

BOOL WinAMS_SPMC_Exp(U2 expID,BOOL exp)
{
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_DI();
#endif
	if (WinAMS_SPMC_Lock == 0) {
		U2 nest;
		WinAMS_SPMC_Lock = 1;
		nest = WinAMS_SPMC_nest-1;
		if (expID < WINAMS_SPMC_MCDC_MAXCONDCNT &&
		    nest < WINAMS_SPMC_MCDC_MAXCONDNEST) {
			U1 amsbit = exp ? 0x3 : 0x2;
			WinAMS_SPMC_bits[nest][expID>>4] |= (U4)amsbit << ((expID & 0xf) << 1);
		}
		WinAMS_SPMC_Lock = 0;
	}
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_EI();
#endif
	return exp;
}

BOOL WinAMS_SPMC_Clr(U2 expcnt)
{
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_DI();
#endif
	if (WinAMS_SPMC_Lock == 0) {
		U2 nest;
		WinAMS_SPMC_Lock = 1;
		nest = WinAMS_SPMC_nest++;
		if (nest < WINAMS_SPMC_MCDC_MAXCONDNEST) {
			U2 i,n;
			n = (expcnt < WINAMS_SPMC_MCDC_MAXCONDCNT) ? expcnt : WINAMS_SPMC_MCDC_MAXCONDCNT;
			n = (n >> 4) + 1;
			for (i = 0 ; i < n ; i++)
				WinAMS_SPMC_bits[nest][i] = 0;
		}
		WinAMS_SPMC_Lock = 0;
	}
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_EI();
#endif
	return 0;
}

BOOL WinAMS_SPMC_Res(WinAMS_SPMC_TFUNCNAME funcname,U4 resID,BOOL res,U2 expcnt,U4 blkID)
{
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_DI();
#endif
	if (WinAMS_SPMC_Lock == 0) {
		U2 nest;
		WinAMS_SPMC_Lock = 1;
		nest = --WinAMS_SPMC_nest;
		if (expcnt < WINAMS_SPMC_MCDC_MAXCONDCNT+1 &&
		    nest < WINAMS_SPMC_MCDC_MAXCONDNEST) {
			U1 amsbit = res ? 0x3 : 0x2;
			WinAMS_SPMC_bits[nest][expcnt>>4] |= (U4)amsbit << ((expcnt & 0xf) << 1);
		}
		else if (nest == 0xffff) {
			WinAMS_SPMC_nest = 0; 
		}
#ifdef WinAMS_SPMC_CVT_FUNCNAME							/* v3.5 */
		WinAMS_SPMC_funcname = WinAMS_SPMC_CVT_FUNCNAME(funcname);	/* v3.5 */
#else
		WinAMS_SPMC_funcname = funcname;
#endif
		WinAMS_SPMC_expcnt = expcnt;
		WinAMS_SPMC_ID = resID;
		WinAMS_SPMC_Blk_ID = blkID;
		WinAMS_SPMC = 2;
		WinAMS_SPMC_Lock = 0;
	}
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_EI();
#endif
	return res;
}

BOOL WinAMS_SPMC_C1(WinAMS_SPMC_TFUNCNAME funcname,U4 blkID)
{
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_DI();
#endif
	if (WinAMS_SPMC_Lock == 0) {
		WinAMS_SPMC_Lock = 1;
#ifdef WinAMS_SPMC_CVT_FUNCNAME							/* v3.5 */
		WinAMS_SPMC_funcname = WinAMS_SPMC_CVT_FUNCNAME(funcname);	/* v3.5 */
#else
		WinAMS_SPMC_funcname = funcname;
#endif
		WinAMS_SPMC_Blk_ID = blkID;
		WinAMS_SPMC = 1;
		WinAMS_SPMC_Lock = 0;
	}
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_EI();
#endif
	return 0;
}


BOOL WinAMS_SPMC_C0(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 line)
{
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_DI();
#endif
	if (WinAMS_SPMC_Lock == 0) {
		WinAMS_SPMC_Lock = 1;
#ifdef WinAMS_SPMC_CVT_FUNCNAME							/* v3.5 */
		WinAMS_SPMC_funcname = WinAMS_SPMC_CVT_FUNCNAME(funcname);	/* v3.5 */
#else
		WinAMS_SPMC_funcname = funcname;
#endif
		WinAMS_SPMC_line = line;
		WinAMS_SPMC = 3;
		WinAMS_SPMC_Lock = 0;
	}
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_EI();
#endif
	return 0;
}

void WinAMS_SPMC_CALL(WinAMS_SPMC_TFUNCNAME funcname,WinAMS_SPMC_U4 callID)
{
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_DI();
#endif
	if (WinAMS_SPMC_Lock == 0) {
		WinAMS_SPMC_Lock = 1;
#ifdef WinAMS_SPMC_CVT_FUNCNAME							/* v3.5 */
		WinAMS_SPMC_funcname = WinAMS_SPMC_CVT_FUNCNAME(funcname);	/* v3.5 */
#else
		WinAMS_SPMC_funcname = funcname;
#endif
		WinAMS_SPMC_ID = callID;
		WinAMS_SPMC = 4;
		WinAMS_SPMC_Lock = 0;
	}
#if defined(WinAMS_SPMC_DI) && defined(WinAMS_SPMC_EI)
	WinAMS_SPMC_EI();
#endif
}
