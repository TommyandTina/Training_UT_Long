/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems. All Rights Reserved.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software.   Free development
 * licenses are available for evaluation and non-commercial purposes.  For more
 * information vvspt http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others.  Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#ifndef VSP_BLIT_H_
#define VSP_BLIT_H_

#include <sys/iomsg.h>
#include <sys/neutrino.h>

#define VSP_BLIT_CHANNEL "vsp_blit_channel"
#define MSG_TYPE_REQUEST_VSP_BLIT (_IO_MAX + 3)

struct rcar3_vsp_blit_msg
{
        struct _pulse hdr;
	int src_id;
	int dst_id;
};

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/graphics/branches/release-2.1.x/hardware/wfd/rcar3/public/wfdqnx/rcar3_vsp_blit.h $ $Rev: 908390 $")
#endif
