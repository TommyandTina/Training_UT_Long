/* example interrupt control */
/*
#define WinAMS_SPMC_DI	_WinAMS_SPMC_DI()
inline void _WinAMS_SPMC_DI()
{
	__asm("	di\n");
}

#define WinAMS_SPMC_EI	_WinAMS_SPMC_EI()
inline void _WinAMS_SPMC_EI()
{
	__asm("	ei\n");
}
*/

/* inport interrupt control */
#define WINAMS_SPMC_U4_INT
#define WinAMS_SPMC_Init_arg_void 1


/* The maximum nuber of the condition to measure coverage */
#define WINAMS_SPMC_MCDC_MAXCONDCNT	127	/* 255 or 511 can expand it */

/* The number if the greatest nests measuring coverage */
#define WINAMS_SPMC_MCDC_MAXCONDNEST	32	/* 64 or 128 can expand it */

#if defined(__S12Z__) || defined(__HCS12__)	/* v6.0.3 */
#define THREE_BYTE_POINTER_USE 1
#else	/* defined(__S12__) */
#define THREE_BYTE_POINTER_USE 0
#endif /* defined(__S12__) */

/* A user defines the type of the string of the function name */
#if THREE_BYTE_POINTER_USE	/* v6.0.3 */
#define WINAMS_SPMC_USR_DEF_TFUNCNAME 1		/* 0:not define, 1:define */
#else	/* THREE_BYTE_POINTER_USE */
#define WINAMS_SPMC_USR_DEF_TFUNCNAME 0		/* 0:not define, 1:define */
#endif	/* THREE_BYTE_POINTER_USE */
#if WINAMS_SPMC_USR_DEF_TFUNCNAME
/* example */
#define WinAMS_SPMC_BASE_TFUNCNAME char			/* base type=char */
/* #define WinAMS_SPMC_BASE_TFUNCNAME signed char */	/* base type=signed char */
/* #define WinAMS_SPMC_BASE_TFUNCNAME unsigned char */	/* base type=unsigned char */
/* #define WinAMS_SPMC_BASE_TFUNCNAME char __far */	/* base type=char __far */
#if THREE_BYTE_POINTER_USE	/* v6.0.3 */
#define WinAMS_SPMC_CVT_FUNCNAME(fname)	((unsigned long)(fname))		/* funcname pointer convert */
#else /* THREE_BYTE_POINTER_USE */
#define WinAMS_SPMC_CVT_FUNCNAME(fname)	(fname)		/* funcname pointer convert */
#endif /* THREE_BYTE_POINTER_USE */
/* #define WinAMS_SPMC_CVT_FUNCNAME(fname)	((WinAMS_SPMC_TFUNCNAME)((unsigned long)(fname) | 0xff0000)) */		/* funcname pointer convert */
#endif /* WINAMS_SPMC_USR_DEF_TFUNCNAME */
#ifndef WinAMS_SPMC_Init_arg_void
#define WinAMS_SPMC_Init_arg_void 0 /* 0: WinAMS_SPMC_Init()   1: WinAMS_SPMC_Init(void) v6.3 */
#endif
