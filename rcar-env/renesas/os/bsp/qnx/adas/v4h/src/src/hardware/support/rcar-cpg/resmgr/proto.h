/*
 * $QNXLicenseC:
 * Copyright 2020, 2023 BlackBerry Limited.
 * Copyright 2020, Renesas Electronics Corporation.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */


#ifndef _IO_H_INCLUDED
#define _IO_H_INCLUDED

#include <inttypes.h>
#include <sys/slog.h>
#include <aarch64/r-car.h>
#include <hw/rcar_cpg_support.h>

/* -v for verbose operation */
extern int verbose;

int rcar_cpg_mstp_control(const cpg_support_funcs_t* const cpg_funcs, cpg_mstpctl_t* const mstpctl, int *status);
int rcar_cpg_clk_control(const cpg_support_funcs_t* const cpg_funcs, cpg_clkctl_t* const clkctl, int *status);

int cpg_slogf(int vlevel, const char *fmt, ...);

#endif /* _IO_H_INCLUDED */
