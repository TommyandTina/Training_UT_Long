/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
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

#include "rswitch.h"
#include <sys/rsrcdbmgr.h>
#include <sys/rsrcdbmsg.h>

#define NAME_SIZE  50

static void rswitch2_rsrcdb_to_rsrcdb_name(char* const buf, const int buf_size, const rsrcdb_type_t type)
{
    if (type == RSRCDB_TYPE_GWCA)
        snprintf(buf, buf_size, "gwca_channels");
    else if (type == RSRCDB_TYPE_ETHA)
        snprintf(buf, buf_size, "etha_channels");
    else
        snprintf(buf, buf_size, "gwca_queues");
}

int rswitch2_rsrcdb_create_resources()
{
    rsrc_alloc_t rsrc_req;
    int gwca_rsrc_ret = EOK;
    int etha_rsrc_ret = EOK;
    int queue_rsrc_ret = EOK;
    char name[NAME_SIZE];

    /* Add the GWCA Channels to the resources database */
    memset(&rsrc_req, 0, sizeof(rsrc_req));

    rswitch2_rsrcdb_to_rsrcdb_name(name, NAME_SIZE, RSRCDB_TYPE_GWCA);

    gwca_rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -2, name, 0);
    if (gwca_rsrc_ret < 0) {
        gwca_rsrc_ret = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Failed querying %s resources", name);
        return gwca_rsrc_ret;
    } else if (gwca_rsrc_ret == 0) {
        rsrc_req.start = 0;
        rsrc_req.end = RSWITCH2_NUM_GWCA * RSWITCH2_TSN_PORT_MAX - 1; // Each GWCA can share with 'RSWITCH2_TSN_PORT_MAX' ports
        rsrc_req.flags = RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name = name;

        gwca_rsrc_ret = rsrcdbmgr_create(&rsrc_req, 1);
        if( gwca_rsrc_ret != EOK ) {
            gwca_rsrc_ret = errno;
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Failed creating %s resources: err %d", name, gwca_rsrc_ret );
            return gwca_rsrc_ret;
        }
    } else {
        gwca_rsrc_ret = EALREADY;
    }

    /* Add the ETHA Channels to the resources database */
    memset(&rsrc_req, 0, sizeof(rsrc_req));

    rswitch2_rsrcdb_to_rsrcdb_name(name, NAME_SIZE, RSRCDB_TYPE_ETHA);

    etha_rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -2, name, 0);
    if (etha_rsrc_ret < 0) {
        etha_rsrc_ret = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Failed querying %s resources", name);
        return etha_rsrc_ret;
    } else if (etha_rsrc_ret == 0) {
        rsrc_req.start = 0;
        rsrc_req.end = RSWITCH2_NUM_ETHA - 1; // 2 ETHA: ETHA0, ETHA1 and ETHA2
        rsrc_req.flags = RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name = name;

        etha_rsrc_ret = rsrcdbmgr_create(&rsrc_req, 1);
        if( etha_rsrc_ret != EOK ) {
            etha_rsrc_ret = errno;
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Failed creating %s resources: err %d", name, gwca_rsrc_ret );
            return etha_rsrc_ret;
        }
    } else {
        etha_rsrc_ret = EALREADY;
    }

    /* Add the Descriptors chain to the resources database */
    memset (&rsrc_req, 0, sizeof(rsrc_req));

    rswitch2_rsrcdb_to_rsrcdb_name(name, NAME_SIZE, RSRCDB_TYPE_QUEUE);

    queue_rsrc_ret = rsrcdbmgr_query_name (NULL, 0, 0, -2, name, 0);
    if (queue_rsrc_ret < 0) {
        queue_rsrc_ret = errno;
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Failed querying %s resources", name);
        return queue_rsrc_ret;
    } else if (queue_rsrc_ret == 0) {
        rsrc_req.start  = 0;
        rsrc_req.end    = RSWITCH2_NUM_GWCA_CHAIN * 2 - 1; // 128*2 Descriptor chains of each GWCA0 and GWCA1
        rsrc_req.flags  = RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_NOREMOVE;
        rsrc_req.name   = name;

        queue_rsrc_ret = rsrcdbmgr_create(&rsrc_req, 1);
        if (queue_rsrc_ret != EOK) {
            queue_rsrc_ret = errno;
            slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: Failed creating resources %s: err %d", name, queue_rsrc_ret);
            return queue_rsrc_ret;
        }
    } else {
        queue_rsrc_ret = EALREADY;
    }

    if ((gwca_rsrc_ret == EALREADY) && (queue_rsrc_ret == EALREADY) && (etha_rsrc_ret == EALREADY)) {
        return EALREADY;
    }

    return EOK;
}

int rswitch2_rsrcdb_attach_resources(rswitch2_dev_t *rswitch2, const rsrcdb_type_t type, const int length)
{
    char name[NAME_SIZE];
    rsrc_request_t  req = { 0 };

    rswitch2_rsrcdb_to_rsrcdb_name(name, NAME_SIZE, type);

    req.flags  = RSRCDBMGR_FLAG_NAME | RSRCDBMGR_FLAG_RANGE;
    req.name   = name;

    if (type == RSRCDB_TYPE_GWCA) {
        /* Attach GWCA0 (index 0, 1, 2) or GWCA1 (index 3, 4, 5) resource */
        req.start  = rswitch2->gwca.index * RSWITCH2_TSN_PORT_MAX;
        req.end    = req.start + RSWITCH2_TSN_PORT_MAX;
        req.length = 1;
    } else if (type == RSRCDB_TYPE_ETHA) {
        /* Attach ETHA0, ETHA1 or ETHA2 resource */
        req.start  = req.end = rswitch2->etha.index;
        req.length = 1;
    } else { 
        /* Attach Descriptor chains of GWCA0 or GWCA1 resource
         * - CWCA0 : from 1 to 127
         * - GWCA1 : from 128 to 2*128-1 */
        req.start  = (rswitch2->gwca.index * RSWITCH2_NUM_GWCA_CHAIN);
        req.end    = (rswitch2->gwca.index + 1) * RSWITCH2_NUM_GWCA_CHAIN - 1;
        req.length = length;
        req.flags |= RSRCDBMGR_FLAG_RANGE;
    }

    if (rsrcdbmgr_attach(&req, 1) == -1) {
        slogf(_SLOGC_NETWORK, _SLOG_ERROR, "rswitch2: rsrcdbmgr_attach %s failed: %s", name, strerror(errno));
        return -1;
    }
    return (req.start - (type == RSRCDB_TYPE_QUEUE ? rswitch2->gwca.index * RSWITCH2_NUM_GWCA_CHAIN : 0));
}

int rswitch2_rsrcdb_detach_resources(rswitch2_dev_t *rswitch2, const rsrcdb_type_t type, const int start, const int length)
{
    char name[NAME_SIZE];
    rsrc_request_t  req = { 0 };

    rswitch2_rsrcdb_to_rsrcdb_name(name, NAME_SIZE, type);

    if (type == RSRCDB_TYPE_GWCA) {
        /* Release GWCA resource */
        req.length = 1;
        req.start  = rswitch2->gwca.rsrc_idx;
        req.flags  = RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = name;
        rsrcdbmgr_detach(&req, 1);
    } else if (type == RSRCDB_TYPE_ETHA) {
        /* Release ETHA resource */
        req.length = 1;
        req.start  = rswitch2->etha.index;
        req.flags  = RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = name;
        rsrcdbmgr_detach(&req, 1);
    } else {
        /* release GWCA Descriptor chain resource */
        req.length = length;
        req.start  = (rswitch2->gwca.index * RSWITCH2_NUM_GWCA_CHAIN) + start;
        req.end    = (rswitch2->gwca.index * RSWITCH2_NUM_GWCA_CHAIN) + start + length;
        req.flags  = RSRCDBMGR_FLAG_RANGE | RSRCDBMGR_FLAG_NAME;
        req.name   = name;
        rsrcdbmgr_detach(&req, 1);
    }

    return EOK;
}

int rswitch2_rsrcdb_check_parallel_mode(rswitch2_dev_t *rswitch2)
{
    /*
     * Checking if this is first time RSwitch HW is started.
     * Purpose is to reset and make common setting for the first using.
     */
    char name[NAME_SIZE];
    rsrc_alloc_t  list[RSWITCH2_NUM_GWCA] = { { 0 }, { 0 } };
    int           size = 0, count = 0, start = 0;

    if (rswitch2->parallel_mode)
        return rswitch2->parallel_mode;

    rswitch2_rsrcdb_to_rsrcdb_name(name, NAME_SIZE, RSRCDB_TYPE_GWCA);

    while (1) {
        if ((count = (rsrcdbmgr_query_name(list, RSWITCH2_NUM_GWCA, start, 0, name, 0))) <= 0) {
            break;
        }

        count = min(RSWITCH2_NUM_GWCA, count);
        for (int i = 0; i < count; i++) {
            size += list[i].end - list[i].start + 1;
        }

        start += count;
    }

    return (size == (RSWITCH2_NUM_GWCA * RSWITCH2_TSN_PORT_MAX) ? 0 : 1);
}

int rswitch2_rsrcdb_check_gwca_used(rswitch2_dev_t *rswitch2)
{
    /*
     * Checking if a GWCA (specified by rswitch2->gwca.index) is used already.
     */
    char name[NAME_SIZE];
    rsrc_alloc_t  list[RSWITCH2_NUM_GWCA] = { { .start = 0 }, { .start = RSWITCH2_TSN_PORT_MAX } };
    int           count = 0, start = 0;

    rswitch2_rsrcdb_to_rsrcdb_name(name, NAME_SIZE, RSRCDB_TYPE_GWCA);

    while (1) {
        if ((count = (rsrcdbmgr_query_name(list, RSWITCH2_NUM_GWCA, start, 0, name, 0))) <= 0) {
            break;
        }
        count = min(RSWITCH2_NUM_GWCA, count);
        start += list[count-1].end + 1;
    }

    return ((list[rswitch2->gwca.index].start % RSWITCH2_TSN_PORT_MAX) ? 1 : 0);
}
