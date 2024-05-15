/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2020, Renesas Electronics Corporation
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
#include "soc.h"

#define SOC_INFO    "VIN"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

capture_context_t capture_create_context(uint32_t flags)
{
    rcar_context_t* rcar_context;

    pthread_mutex_lock(&mutex);

    if((rcar_context = calloc(1, sizeof(rcar_context_t))) == NULL) {
        fprintf(stderr, "%s: calloc(rcar_context_t) failed\n", __FUNCTION__);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    rcar_context->active_dev = 0;
    rcar_context->source_idx = 0;
    rcar_context->is_running = 0;
    rcar_context->enable = 0;
    pthread_mutex_init(&rcar_context->mutex, NULL);

    pthread_mutex_unlock(&mutex);

    return rcar_context;
}

int capture_create_buffers(capture_context_t context, uint32_t property)
{
    errno = ENOTSUP;
    return -1;
}

void capture_destroy_context(capture_context_t context)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;

    pthread_mutex_lock(&mutex);

    if(p_soc) {
        if (p_soc->is_running) {
            rcar_vin_fini(&p_soc->vin);
        }
        pthread_mutex_destroy(&p_soc->mutex);
        free(p_soc);
    }

    pthread_mutex_unlock(&mutex);
}

static int capture_endisable(capture_context_t context, int enable)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;

    pthread_mutex_lock(&p_soc->mutex);

    if (enable && p_soc->enable) {
        if (!p_soc->is_running) {
            if(rcar_vin_init(vin)) {
                pthread_mutex_unlock(&p_soc->mutex);
                return -1;
            }
        } else {
            if(rcar_vin_update(vin)) {
                pthread_mutex_unlock(&p_soc->mutex);
                return -1;
            }
        }

        p_soc->is_running = 1;

    } else if (!enable && p_soc->enable) {
        if (p_soc->is_running) {
            if(rcar_vin_fini(vin)) {
                pthread_mutex_unlock(&p_soc->mutex);
                return -1;
            }
        }

        p_soc->is_running = 0;
        p_soc->enable = 0;
    }

    pthread_mutex_unlock(&p_soc->mutex);

    return 0;
}

int capture_update(capture_context_t context, uint32_t flags)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;

    pthread_mutex_lock(&p_soc->mutex);

    if (capture_endisable(p_soc, 1)) {
        pthread_mutex_unlock(&p_soc->mutex);
        return -1;
    }

    pthread_mutex_unlock(&p_soc->mutex);

    return 0;
}

int capture_get_frame(capture_context_t context, uint64_t timeout, uint32_t flags)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;
    int ret;

    if(p_soc->is_running == 0) {
        errno = ECANCELED;
        return -1;
    }

    pthread_mutex_lock(&p_soc->mutex);

    ret = rcar_get_frame(vin, timeout, p_soc->source_idx);

    pthread_mutex_unlock(&p_soc->mutex);

    return ret;
}

int capture_release_frame(capture_context_t context, uint32_t idx)
{	
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;

    return rcar_release_frame(vin, p_soc->source_idx, idx);
}

int capture_put_buffer(capture_context_t ctx, uint32_t idx, uint32_t flags)
{
    return 0;
}

int capture_is_property(capture_context_t context, uint32_t prop)
{
    switch(prop)
    {
        case CAPTURE_PROPERTY_DEVICE_INFO:
        case CAPTURE_PROPERTY_DEVICE:
        case CAPTURE_ENABLE:
        case CAPTURE_PROPERTY_SRC_WIDTH:
        case CAPTURE_PROPERTY_SRC_HEIGHT:
        case CAPTURE_PROPERTY_SRC_FORMAT:
        case CAPTURE_PROPERTY_NORM:
        case CAPTURE_PROPERTY_DST_WIDTH:
        case CAPTURE_PROPERTY_DST_HEIGHT:
        case CAPTURE_PROPERTY_DST_FORMAT:
        case CAPTURE_PROPERTY_DST_STRIDE:
        case CAPTURE_PROPERTY_CROP_X:
        case CAPTURE_PROPERTY_CROP_Y:
        case CAPTURE_PROPERTY_CROP_WIDTH:
        case CAPTURE_PROPERTY_CROP_HEIGHT:
        case CAPTURE_PROPERTY_FRAME_FLAGS:
        case CAPTURE_PROPERTY_FRAME_TIMESTAMP:
        case CAPTURE_PROPERTY_FRAME_NBUFFERS:
        case CAPTURE_PROPERTY_FRAME_BUFFERS:
        case CAPTURE_PROPERTY_CSI2_NUM_DATA_LANES:
        case CAPTURE_PROPERTY_NSOURCES:
        case CAPTURE_PROPERTY_BOARD_ID:
        case CAPTURE_PROPERTY_DECODER_ID:
        case CAPTURE_PROPERTY_IPMMU_SUPPORT:
        {
            return 1;
        }
    }

    return 0;
}

int capture_get_property_i(capture_context_t context, uint32_t prop, int32_t *value)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;

    switch(prop)
    {
        case CAPTURE_ENABLE:
            *value = p_soc->is_running;
            break;
        case CAPTURE_PROPERTY_FRAME_NBUFFERS:
            *value = vin->frm_nbufs[p_soc->source_idx];
            break;
        case CAPTURE_PROPERTY_NSOURCES:
            *value = vin->vin_num;
            break;
        default:
            errno = ENOTSUP;
            return -1;
    }

    return 0;
}

int capture_get_property_p(capture_context_t context, uint32_t prop, void **value)
{
    switch(prop)
    {
        case CAPTURE_PROPERTY_DEVICE_INFO:
            strcpy((char*)(value), SOC_INFO);
            break;
        default:
            errno = ENOTSUP;
            return -1;
    }

    return 0;
}

int capture_set_property_i(capture_context_t context, uint32_t prop, int32_t value)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;
    decoder_info_t *dec = &vin->dec;

    switch(prop)
    {
        case CAPTURE_PROPERTY_DEVICE:
            p_soc->active_dev = value;
            break;
        case CAPTURE_PROPERTY_BOARD_ID:
            vin->board = value;
            break;
        case CAPTURE_PROPERTY_DECODER_ID:
            vin->decoder = value;
            break;
        case CAPTURE_PROPERTY_SRC_INDEX:
            p_soc->source_idx = value;
            vin->enable[p_soc->source_idx] = 1;
            break;
        case CAPTURE_ENABLE:
            p_soc->enable = value ? 1 : 0;
            break;
        case CAPTURE_PROPERTY_SRC_WIDTH:
            dec->sw = value;
            break;
        case CAPTURE_PROPERTY_SRC_HEIGHT:
            dec->sh = value;
            break;
        case CAPTURE_PROPERTY_FRAME_FLAGS:
            dec->interlace = value;
            break;
        case CAPTURE_PROPERTY_SRC_FORMAT:
            dec->format = value;
            break;
        case CAPTURE_PROPERTY_DST_WIDTH:
            dec->dw = value;
            dec->update |= SOC_SCALING_UPDATE;
            break;
        case CAPTURE_PROPERTY_DST_HEIGHT:
            dec->dh = value;
            dec->update |= SOC_SCALING_UPDATE;
            break;
        case CAPTURE_PROPERTY_DST_FORMAT:
            dec->dfmt = value;
            break;
        case CAPTURE_PROPERTY_DST_STRIDE:
            dec->dstride = value;
            break;
        case CAPTURE_PROPERTY_CROP_X:
            dec->cx = value;
            dec->update |= SOC_CROPPING_UPDATE;
            break;
        case CAPTURE_PROPERTY_CROP_Y:
            dec->cy = value;
            dec->update |= SOC_CROPPING_UPDATE;
            break;
        case CAPTURE_PROPERTY_CROP_WIDTH:
            dec->cw = value;
            dec->update |= SOC_CROPPING_UPDATE;
            break;
        case CAPTURE_PROPERTY_CROP_HEIGHT:
            dec->ch = value;
            dec->update |= SOC_CROPPING_UPDATE;
            break;
        case CAPTURE_PROPERTY_FRAME_NBUFFERS:
            vin->frm_nbufs[p_soc->source_idx] = value;
            break;
        case CAPTURE_PROPERTY_CSI2_DDR_CLOCK_MHZ:
            vin->csi_rate = value;
            break;
        case CAPTURE_PROPERTY_CSI2_PHY_MODE:
            vin->csi_phy_mode = value;
            break;
        case CAPTURE_PROPERTY_CSI2_NUM_DATA_LANES:
            if (value > vin->csi_lanes_max) {
                vin->csi_lanes = vin->csi_lanes_max;
            } else {
                vin->csi_lanes = value;
            }
            break;
        case CAPTURE_PROPERTY_IPMMU_SUPPORT:
            vin->ipmmu_support = value;
            break;
        default:
            errno = ENOTSUP;
            return -1;
    }

    return 0;
}

int capture_set_property_p(capture_context_t context, uint32_t prop, void *value)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;
    decoder_info_t *dec = &vin->dec;

    switch(prop)
    {
        case CAPTURE_PROPERTY_NORM:
            strcpy(dec->sfmt, (char*)value);
            break;
        case CAPTURE_PROPERTY_FRAME_BUFFERS:
            vin->frm_bufs[p_soc->source_idx] = (void **)value;
            break;
        case CAPTURE_PROPERTY_FRAME_TIMESTAMP:
        case CAPTURE_PROPERTY_FRAME_FLAGS:
            /* Not implemented yet */
            break;
        default:
            errno = ENOTSUP;
            return -1;
    }

    return 0;
}


int capture_init(capture_context_t context, int device, int board_id, void *info_addr)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;
    input_attribute_t *info = (input_attribute_t *)info_addr;

    p_soc->active_dev   = device;
    vin->board          = board_id;
    vin->vin_num        = info->vin_num;
    vin->vin_chan       = info->vin_chan;
    vin->csi_chan       = info->csi_chan;
    vin->csi_lanes      = info->csi_lanes;
    vin->csi_lanes_max  = info->csi_lanes;
    vin->csi_phy_mode   = info->csi_phy_mode;
    vin->scale_support  = info->scale_support;
    vin->input_type     = info->type;
    
    for(int i = 0 ; i < vin->vin_num ; i++)
        vin->enable[i] = 0;

    return 0;
}

