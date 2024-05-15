
/*
 * $QNXLicenseC:
 * Copyright 2014, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include <stdio.h>
#include <string.h>
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>
#include <audio_driver.h>
#include "rcar_rsrc.h"
#include "rcar_support.h"

#define RCAR_SSI_RSRC_NAME "iossi"
#define RCAR_HDMI_RSRC_NAME "iohdmi"
#define RCAR_SRC_RSRC_NAME "iosrc"
#define RCAR_CMD_RSRC_NAME "iocmd"
#define RCAR_MLM_RSRC_NAME "iomlm"
#define RCAR_DTCPP_RSRC_NAME "iodtcpp"
#define RCAR_DTCPC_RSRC_NAME "iodtcpc"

typedef struct {
    char* rsrc_name;
    uint32_t min_rsrc_idx;
    uint32_t max_rsrc_idx;
} rsrc_info_t;

/* Masks of resources reserved by current process, updated upon success
 * of rsrcdbmgr_attach or rsrcdbmgr_detach calls
 * Note: it is not required to keep track of resources reserved per
 * rcar driver instance (in case io-audio mounts multiple instances
 * of the rcar driver), given that each instance keeps track which
 * resources it was able to reserve successfully;
 * A mutex is used to protect below masks that can be modified or
 * accessed by multiple rcar driver instances, along with the actual
 * resource reservation
 */
static uint32_t reserved_ssi_mask = 0;
static uint32_t reserved_src_mask = 0;
static uint32_t reserved_cmd_mask = 0;
static uint32_t reserved_hdmi_mask = 0;
static uint32_t reserved_dtcp_mask = 0;
static uint32_t reserved_mlm_mask = 0;
static ado_mutex_t rsrc_lock = PTHREAD_MUTEX_INITIALIZER;

int rcar_create_resources()
{
    static rsrc_info_t rsrc_info[6];
    static rsrc_alloc_t rsrc_req[6];
    uint32_t rsrc_idx = 0;
    uint32_t rsrc_num = 0;
    uint32_t rsrc_req_idx = 0;
    int rsrc_ret = EOK;

    if( rcar_ssi_get_supported_range( &rsrc_info[rsrc_idx].min_rsrc_idx, &rsrc_info[rsrc_idx].max_rsrc_idx ) == EOK ) {
        rsrc_info[rsrc_idx++].rsrc_name = RCAR_SSI_RSRC_NAME;
    }
    if( rcar_hdmi_get_supported_range( &rsrc_info[rsrc_idx].min_rsrc_idx, &rsrc_info[rsrc_idx].max_rsrc_idx ) == EOK ) {
        rsrc_info[rsrc_idx++].rsrc_name = RCAR_HDMI_RSRC_NAME;
    }
    if( rcar_src_get_supported_range( &rsrc_info[rsrc_idx].min_rsrc_idx, &rsrc_info[rsrc_idx].max_rsrc_idx ) == EOK ) {
        rsrc_info[rsrc_idx++].rsrc_name = RCAR_SRC_RSRC_NAME;
    }
    if( rcar_cmd_get_supported_range( &rsrc_info[rsrc_idx].min_rsrc_idx, &rsrc_info[rsrc_idx].max_rsrc_idx ) == EOK ) {
        rsrc_info[rsrc_idx++].rsrc_name = RCAR_CMD_RSRC_NAME;
    }
    if( rcar_mlm_get_supported_range( &rsrc_info[rsrc_idx].min_rsrc_idx, &rsrc_info[rsrc_idx].max_rsrc_idx ) == EOK ) {
        rsrc_info[rsrc_idx++].rsrc_name = RCAR_MLM_RSRC_NAME;
    }
    if( rcar_dtcp_get_supported_range( &rsrc_info[rsrc_idx].min_rsrc_idx, &rsrc_info[rsrc_idx].max_rsrc_idx ) == EOK ) {
        rsrc_info[rsrc_idx++].rsrc_name = RCAR_DTCPP_RSRC_NAME;
    }
    if( rcar_dtcp_get_supported_range( &rsrc_info[rsrc_idx].min_rsrc_idx, &rsrc_info[rsrc_idx].max_rsrc_idx ) == EOK ) {
        rsrc_info[rsrc_idx++].rsrc_name = RCAR_DTCPC_RSRC_NAME;
    }

    rsrc_num = rsrc_idx;

    memset(&rsrc_req, 0, sizeof(rsrc_req));

    ado_mutex_lock (&rsrc_lock);

    for( rsrc_idx = 0, rsrc_req_idx = 0; rsrc_idx < rsrc_num; rsrc_idx++ ) {
        rsrc_ret = rsrcdbmgr_query_name( NULL, 0, 0, -1, rsrc_info[rsrc_idx].rsrc_name, RSRCDBMGR_IO_PORT );

        if( rsrc_ret < 0 ) {
            ado_mutex_unlock (&rsrc_lock);
            ado_error_fmt( "Failed querying %s resources", rsrc_info[rsrc_idx].rsrc_name );
            return rsrc_ret;
        }
        if( rsrc_ret > 0 ) {
            if( (uint32_t)rsrc_ret < rsrc_info[rsrc_idx].max_rsrc_idx - rsrc_info[rsrc_idx].min_rsrc_idx + 1 ) {
                ado_error_fmt( "%s resources already created, expected %d, found %d",
                           rsrc_info[rsrc_idx].rsrc_name,
                           rsrc_info[rsrc_idx].max_rsrc_idx - rsrc_info[rsrc_idx].min_rsrc_idx + 1,
                           rsrc_ret );
            } else {
                ado_error_fmt( "%s resources already created", rsrc_info[rsrc_idx].rsrc_name );
            }
            continue;
        }

        ado_debug (DB_LVL_DRIVER, "Adding %d resources of type %s to the database",
                   rsrc_info[rsrc_idx].max_rsrc_idx - rsrc_info[rsrc_idx].min_rsrc_idx + 1,
                   rsrc_info[rsrc_idx].rsrc_name);
        rsrc_req[rsrc_req_idx].start = rsrc_info[rsrc_idx].min_rsrc_idx;
        rsrc_req[rsrc_req_idx].end = rsrc_info[rsrc_idx].max_rsrc_idx;
        rsrc_req[rsrc_req_idx].flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req[rsrc_req_idx++].name = rsrc_info[rsrc_idx].rsrc_name;
    }

    if( rsrc_req_idx > 0 ) {
        ado_debug (DB_LVL_DRIVER, "Creating %d types of resources", rsrc_req_idx);
        rsrc_ret = rsrcdbmgr_create( &rsrc_req[0], rsrc_req_idx );

        if( rsrc_ret != EOK ) {
            ado_error_fmt( "Failed creating resources: err %d", rsrc_ret );
        }
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* reserve specified SSI channel range */
int rcar_reserve_ssi( uint32_t min_ssi_channel, uint32_t max_ssi_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx, i;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_ssi_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }

    if( min_ssi_channel < min_rsrc_idx || min_ssi_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }

    if( max_ssi_channel < min_rsrc_idx || max_ssi_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }

    rsrc_req.name = RCAR_SSI_RSRC_NAME;
    rsrc_req.length = max_ssi_channel - min_ssi_channel + 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = min_ssi_channel;
    rsrc_req.end = max_ssi_channel;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_attach( &rsrc_req, 1 );

    if( rsrc_ret != EOK ) {
        ado_error_fmt( "Failed reserving SSI resources %d-%d: err %d",
                       min_ssi_channel, max_ssi_channel, rsrc_ret );
    } else {
        for (i = min_ssi_channel; i <= max_ssi_channel; i++) {
            reserved_ssi_mask |= (1<<i);
        }
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* release specified SSI channel range */
int rcar_release_ssi( uint32_t min_ssi_channel, uint32_t max_ssi_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx, i;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_ssi_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }

    if( min_ssi_channel < min_rsrc_idx || min_ssi_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }

    if( max_ssi_channel < min_rsrc_idx || max_ssi_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }

    ado_mutex_lock (&rsrc_lock);

    for (i = min_ssi_channel; i <= max_ssi_channel; i++) {
        if (!(reserved_ssi_mask & (1<<i))) {
            ado_mutex_unlock (&rsrc_lock);
            ado_error_fmt( "Can't release SSI resource %d not reserved by this process", i );
            return EINVAL;
        }
    }

    rsrc_req.name = RCAR_SSI_RSRC_NAME;
    rsrc_req.length = max_ssi_channel - min_ssi_channel + 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = min_ssi_channel;
    rsrc_req.end = max_ssi_channel;

    rsrc_ret = rsrcdbmgr_detach( &rsrc_req, 1 );
    if( rsrc_ret != EOK ) {
        ado_error_fmt( "Failed releasing SSI resources %d-%d: err %d",
                       min_ssi_channel, max_ssi_channel, rsrc_ret );
    } else {
        for (i = min_ssi_channel; i <= max_ssi_channel; i++) {
            reserved_ssi_mask &= ~(1<<i);
        }
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* reserve specified HDMI channel range */
int rcar_reserve_hdmi( uint32_t min_hdmi_channel, uint32_t max_hdmi_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx, i;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_hdmi_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }

    if( min_hdmi_channel < min_rsrc_idx || min_hdmi_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }

    if( max_hdmi_channel < min_rsrc_idx || max_hdmi_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }

    rsrc_req.name = RCAR_HDMI_RSRC_NAME;
    rsrc_req.length = max_hdmi_channel - min_hdmi_channel + 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = min_hdmi_channel;
    rsrc_req.end = max_hdmi_channel;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_attach( &rsrc_req, 1 );

    if( rsrc_ret != EOK ) {
        ado_error_fmt( "Failed reserving HDMI resources %d-%d: err %d",
                       min_hdmi_channel, max_hdmi_channel, rsrc_ret );
    } else {
        for (i = min_hdmi_channel; i <= max_hdmi_channel; i++) {
            reserved_hdmi_mask |= (1<<i);
        }
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* release specified HDMI channel range */
int rcar_release_hdmi( uint32_t min_hdmi_channel, uint32_t max_hdmi_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx, i;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_hdmi_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }

    if( min_hdmi_channel < min_rsrc_idx || min_hdmi_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }

    if( max_hdmi_channel < min_rsrc_idx || max_hdmi_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }

    ado_mutex_lock (&rsrc_lock);

    for (i = min_hdmi_channel; i <= max_hdmi_channel; i++) {
        if (!(reserved_hdmi_mask & (1<<i))) {
            ado_mutex_unlock (&rsrc_lock);
            ado_error_fmt( "Can't release HDMI resource %d not reserved by this process", i );
            return EINVAL;
        }
    }

    rsrc_req.name = RCAR_HDMI_RSRC_NAME;
    rsrc_req.length = max_hdmi_channel - min_hdmi_channel + 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = min_hdmi_channel;
    rsrc_req.end = max_hdmi_channel;

    rsrc_ret = rsrcdbmgr_detach( &rsrc_req, 1 );
    if( rsrc_ret != EOK ) {
        ado_error_fmt( "Failed releasing HDMI resources %d-%d: err %d",
                       min_hdmi_channel, max_hdmi_channel, rsrc_ret );
    } else {
        for (i = min_hdmi_channel; i <= max_hdmi_channel; i++) {
            reserved_hdmi_mask &= ~(1<<i);
        }
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* reserve SRC with specified features */
int rcar_reserve_src( uint32_t multichannel, uint32_t highsound, uint32_t is_inline, uint32_t* src_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( src_channel == NULL ) {
        return EINVAL;
    }
    if( rcar_src_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }

    ado_debug(DB_LVL_DRIVER, "Reserve SRC: multichannel=%d highsound=%d inline=%d",
              multichannel, highsound, is_inline);

    ado_mutex_lock (&rsrc_lock);

    while( min_rsrc_idx <= max_rsrc_idx ) {
        rsrc_req.name = RCAR_SRC_RSRC_NAME;
        rsrc_req.length = 1;
        rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
        rsrc_req.start = min_rsrc_idx;
        rsrc_req.end = max_rsrc_idx;

        rsrc_ret = rsrcdbmgr_attach( &rsrc_req, 1 );

        if( rsrc_ret != EOK ) {
            ado_error_fmt( "Failed reserving 1 SRC resource in range %d-%d: err %d",
                           min_rsrc_idx, max_rsrc_idx, rsrc_ret );
            break;
        }

        *src_channel = rsrc_req.start;
        ado_debug(DB_LVL_DRIVER, "Found SRC: %d", *src_channel);

        if( ( multichannel && !rcar_src_multichan_supported( *src_channel ) ) ||
            ( highsound && !rcar_src_highsound_supported( *src_channel ) ) ||
            ( is_inline && !rcar_src_inline_supported( *src_channel ) ) ) {
            rsrc_req.end = rsrc_req.start;
            rsrcdbmgr_detach( &rsrc_req, 1 );

            ado_error_fmt( "SRC %d doesn't satisfy the search criteria, continue looking", *src_channel );
            min_rsrc_idx = *src_channel + 1;
            rsrc_ret = EAGAIN;
            continue;
        }

        reserved_src_mask |= (1<<*src_channel);

        break;
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* release specified SRC */
int rcar_release_src( uint32_t src_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_src_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }
    if( src_channel < min_rsrc_idx || src_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }
	if( !(reserved_src_mask & (1<<src_channel)) ) {
        ado_error_fmt( "Can't release SRC resource %d not reserved by this process", src_channel );
        return EINVAL;
    }
    rsrc_req.name = RCAR_SRC_RSRC_NAME;
    rsrc_req.length = 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = src_channel;
    rsrc_req.end = src_channel;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_detach( &rsrc_req, 1 );
    if( rsrc_ret != EOK ) {
        ado_error_fmt( "Failed releasing SRC resource %d: err %d", src_channel, rsrc_ret );
    } else {
        reserved_src_mask &= ~(1<<src_channel);
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* reserve CMD */
int rcar_reserve_cmd( uint32_t* cmd_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( cmd_channel == NULL ) {
        return EINVAL;
    }
    if( rcar_cmd_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }

    ado_debug(DB_LVL_DRIVER, "Reserve a CMD");

    rsrc_req.name = RCAR_CMD_RSRC_NAME;
    rsrc_req.length = 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = min_rsrc_idx;
    rsrc_req.end = max_rsrc_idx;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_attach( &rsrc_req, 1 );

    if( rsrc_ret == EOK ) {
        *cmd_channel = rsrc_req.start;
        reserved_cmd_mask |= (1<<*cmd_channel);
    } else {
        ado_error_fmt( "Failed reserving 1 CMD resource in range %d-%d: err %d",
                       min_rsrc_idx, max_rsrc_idx, rsrc_ret );
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* release specified CMD */
int rcar_release_cmd( uint32_t cmd_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_cmd_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }
    if( cmd_channel < min_rsrc_idx || cmd_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }
	if( !(reserved_cmd_mask & (1<<cmd_channel)) ) {
        ado_error_fmt( "Can't release CMD resource %d not reserved by this process", cmd_channel );
        return EINVAL;
    }

    rsrc_req.name = RCAR_CMD_RSRC_NAME;
    rsrc_req.length = 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = cmd_channel;
    rsrc_req.end = cmd_channel;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_detach( &rsrc_req, 1 );
    if( rsrc_ret != EOK ) {
        ado_error_fmt( "Failed releasing CMD resource %d: err %d", cmd_channel, rsrc_ret );
    } else {
        reserved_cmd_mask &= ~(1<<cmd_channel);
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}


/* reserve MLM */
int rcar_reserve_mlm( uint32_t* mlm_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_mlm_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }

    rsrc_req.name = RCAR_MLM_RSRC_NAME;
    rsrc_req.length = 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = min_rsrc_idx;
    rsrc_req.end = max_rsrc_idx;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_attach( &rsrc_req, 1 );

    if( rsrc_ret == EOK ) {
        *mlm_channel = rsrc_req.start;
        reserved_mlm_mask |= (1<<*mlm_channel);
    } else {
        ado_error_fmt( "Failed reserving 1 MLM resource in range %d-%d: err %d",
                       min_rsrc_idx, max_rsrc_idx, rsrc_ret );
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* release specified MLM */
int rcar_release_mlm( uint32_t mlm_channel )
{
    uint32_t min_rsrc_idx, max_rsrc_idx;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_mlm_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }
    if( mlm_channel < min_rsrc_idx || mlm_channel > max_rsrc_idx ) {
        return ENOTSUP;
    }
	if( !(reserved_mlm_mask & (1<<mlm_channel)) ) {
        ado_error_fmt( "Can't release MLM resource %d not reserved by this process", mlm_channel );
        return EINVAL;
    }
    rsrc_req.name = RCAR_MLM_RSRC_NAME;
    rsrc_req.length = 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = mlm_channel;
    rsrc_req.end = mlm_channel;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_detach( &rsrc_req, 1 );
    if( rsrc_ret != EOK ) {
        ado_error_fmt( "Failed releasing MLM resource %d: err %d", mlm_channel, rsrc_ret );
    } else {
        reserved_mlm_mask &= ~(1<<mlm_channel);
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* reserve DTCP */
int rcar_reserve_dtcp( uint32_t* dtcp )
{
    uint32_t min_rsrc_idx, max_rsrc_idx;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_dtcp_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }

    rsrc_req.name = RCAR_DTCPP_RSRC_NAME;
    rsrc_req.length = 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = min_rsrc_idx;
    rsrc_req.end = max_rsrc_idx;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_attach( &rsrc_req, 1 );

    if( rsrc_ret == EOK ) {
        *dtcp = rsrc_req.start;
        reserved_dtcp_mask |= (1<<*dtcp);
    } else {
        ado_error_fmt( "Failed reserving 1 DTCP resource in range %d-%d: err %d",
                       min_rsrc_idx, max_rsrc_idx, rsrc_ret );
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}

/* release specified DTCP */
int rcar_release_dtcp( uint32_t dtcp )
{
    uint32_t min_rsrc_idx, max_rsrc_idx;
    rsrc_request_t rsrc_req;
    int rsrc_ret;

    if( rcar_dtcp_get_supported_range( &min_rsrc_idx, &max_rsrc_idx ) != EOK ) {
        return ENOTSUP;
    }
    if( dtcp < min_rsrc_idx || dtcp > max_rsrc_idx ) {
        return ENOTSUP;
    }
    if( !(reserved_dtcp_mask & (1<<dtcp)) ) {
        ado_error_fmt( "Can't release DTCP resource %d not reserved by this process", dtcp );
        return EINVAL;
    }
    rsrc_req.name = RCAR_DTCPP_RSRC_NAME;
    rsrc_req.length = 1;
    rsrc_req.flags = RSRCDBMGR_IO_PORT|RSRCDBMGR_FLAG_NAME|RSRCDBMGR_FLAG_RANGE;
    rsrc_req.start = dtcp;
    rsrc_req.end = dtcp;

    ado_mutex_lock (&rsrc_lock);

    rsrc_ret = rsrcdbmgr_detach( &rsrc_req, 1 );
    if( rsrc_ret != EOK ) {
        ado_error_fmt( "Failed releasing DTCP resource %d: err %d", dtcp, rsrc_ret );
    } else {
        reserved_dtcp_mask &= ~(1<<dtcp);
    }

    ado_mutex_unlock (&rsrc_lock);

    return rsrc_ret;
}



#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/rcar_rsrc.c $ $Rev: 869292 $")
#endif

