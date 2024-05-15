/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation
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

#include "decoder.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char* norm_none = CAPTURE_NORM_NONE;

capture_context_t capture_create_context(uint32_t flags)
{
    rcar_context_t* rcar_context;

    pthread_mutex_lock(&mutex);

    if((rcar_context = calloc(1, sizeof(*rcar_context))) == NULL) {
        fprintf(stderr, "%s: calloc failed\n", __FUNCTION__);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    rcar_context->enable = 0;
    rcar_context->is_running = 0;
    rcar_context->source_idx = 0;
    rcar_context->video.update = 0;
    pthread_mutex_init(&rcar_context->mutex, NULL);

    pthread_mutex_unlock(&mutex);

    return rcar_context;
}

void capture_destroy_context( capture_context_t context )
{
    rcar_context_t *p_decoder = (rcar_context_t *)context;
    video_info_t* video = &p_decoder->video;
    int channel = p_decoder->source_idx;

    pthread_mutex_lock(&mutex);

    if(p_decoder) {
        DECODER_FINI(video, channel);
        free(p_decoder);
    }

    pthread_mutex_unlock(&mutex);
}

int capture_init(capture_context_t ctx, int device, int board_id, void *info_addr)
{
    rcar_context_t *p_decoder = (rcar_context_t *)ctx;
    video_info_t *video = &p_decoder->video;
    input_attribute_t *info = (input_attribute_t *)info_addr;

    pthread_mutex_lock(&p_decoder->mutex);

    video->csi_ch        = info->csi_chan;
    video->csi_lanes     = info->csi_lanes;
    video->csi_lanes_max = info->csi_lanes;
    video->csi_phy_mode  = info->csi_phy_mode;
    video->active_dev    = device;
    video->gmsl_mode     = 1;
    p_decoder->board_id  = board_id;

    if(DECODER_INIT(&p_decoder->video, device, board_id)) {
        pthread_mutex_unlock(&p_decoder->mutex);
        return -1;
    }

    pthread_mutex_unlock(&p_decoder->mutex);

    return 0;
}

static int capture_endisable(capture_context_t ctx, int enable)
{
    rcar_context_t *p_decoder = (rcar_context_t *)ctx;
    video_info_t *video = &p_decoder->video;

    pthread_mutex_lock(&p_decoder->mutex);

    if (enable && p_decoder->enable) {
        if (!p_decoder->is_running) {
            if(DECODER_UPDATE(video, p_decoder->source_idx)) {
                pthread_mutex_unlock(&p_decoder->mutex);
                return -1;
            }
        } else {
            if(DECODER_ENABLE(video, p_decoder->source_idx, 1)) {
                pthread_mutex_unlock(&p_decoder->mutex);
                return -1;
            }
        }

        p_decoder->is_running = 1;

    } else if (!enable && p_decoder->enable) {
        if (p_decoder->is_running) {
            if(DECODER_DISABLE(video, p_decoder->source_idx)) {
                pthread_mutex_unlock(&p_decoder->mutex);
                return -1;
            }
        }

        p_decoder->is_running = 0;
    }

    pthread_mutex_unlock(&p_decoder->mutex);

    return 0;
}

int capture_create_buffers(capture_context_t context, uint32_t property)
{
    errno = ENOTSUP;

    return -1;
}

int capture_update(capture_context_t context, uint32_t flags)
{
    rcar_context_t *p_decoder = (rcar_context_t *)context;

    pthread_mutex_lock(&p_decoder->mutex);

    if(capture_endisable(p_decoder, 1)) {
        pthread_mutex_unlock(&p_decoder->mutex);
        return -1;
    }

    pthread_mutex_unlock(&p_decoder->mutex);

    return 0;
}

int capture_get_frame( capture_context_t context, uint64_t timeout, uint32_t flags )
{
    rcar_context_t *p_decoder = (rcar_context_t *)context;
    video_info_t* video = &p_decoder->video;
    int channel = p_decoder->source_idx;
    int ret = 0;

    if(p_decoder->is_running == 0) {
        errno = ECANCELED;
        return -1;
    }
    pthread_mutex_lock(&p_decoder->mutex);

    ret = DECODER_CHECK(video, channel);

    pthread_mutex_unlock(&p_decoder->mutex);

    return ret;
}

int capture_release_frame( capture_context_t context, uint32_t idx )
{
    errno = ENOTSUP;

    return -1;
}

int capture_put_buffer( capture_context_t ctx, uint32_t idx, uint32_t flags )
{
    errno = ENOTSUP;

    return -1;
}

int capture_is_property(capture_context_t context, uint32_t prop)
{
    switch(prop)
    {
        case CAPTURE_ENABLE:
        case CAPTURE_PROPERTY_SRC_INDEX:
        case CAPTURE_PROPERTY_DST_WIDTH:
        case CAPTURE_PROPERTY_CONTRAST:
        case CAPTURE_PROPERTY_BRIGHTNESS:
        case CAPTURE_PROPERTY_SATURATION:
        case CAPTURE_PROPERTY_HUE:
        case CAPTURE_PROPERTY_DEVICE_INFO:
        case CAPTURE_PROPERTY_CURRENT_NORM:
        case CAPTURE_PROPERTY_SRC_WIDTH:
        case CAPTURE_PROPERTY_SRC_HEIGHT:
        case CAPTURE_PROPERTY_SRC_FORMAT:
        case CAPTURE_PROPERTY_DEVICE:
        case CAPTURE_PROPERTY_BOARD_ID:
        case CAPTURE_PROPERTY_CSI2_NUM_DATA_LANES:
        case CAPTURE_PROPERTY_GMSL_MODE:
        {
            return 1;
        }
    }

    return 0;
}

int capture_get_property_i(capture_context_t context, uint32_t prop, int32_t* value)
{
    rcar_context_t *p_decoder = (rcar_context_t *)context;
    video_info_t* video = &p_decoder->video;

    pthread_mutex_lock(&p_decoder->mutex);

    switch(prop)
    {
        case CAPTURE_PROPERTY_SRC_INDEX:
            *value = p_decoder->source_idx;
            break;
        case CAPTURE_ENABLE:
            *value = p_decoder->enable;
            break;
        case CAPTURE_PROPERTY_CONTRAST:
            *value = (int32_t)video->con;
            break;
        case CAPTURE_PROPERTY_HUE:
            *value = (int32_t)video->hue;
            break;
        case CAPTURE_PROPERTY_BRIGHTNESS:
            *value = (int32_t)video->bri;
            break;
        case CAPTURE_PROPERTY_SATURATION:
            *value = (int32_t)video->sat;
            break;
        case CAPTURE_PROPERTY_SRC_WIDTH:
            *value = video->width;
            break;
        case CAPTURE_PROPERTY_SRC_HEIGHT:
            *value = video->height;
            break;
        case CAPTURE_PROPERTY_FRAME_FLAGS:
            *value = video->interlace;
            break;
        case CAPTURE_PROPERTY_CSI2_DDR_CLOCK_MHZ:
            *value = video->csi_speed;
            break;
        case CAPTURE_PROPERTY_CSI2_NUM_DATA_LANES:
            *value = video->csi_lanes;
            break;
        case CAPTURE_PROPERTY_GMSL_MODE:
            if(video->gmsl_mode == 1)  
                *value = 0;
            else  
                *value = 1;

            break;
        default:
            errno = ENOTSUP;
            pthread_mutex_unlock(&p_decoder->mutex);
            return -1;
    }

    pthread_mutex_unlock(&p_decoder->mutex);

    return 0;
}

int capture_get_property_p(capture_context_t context, uint32_t prop, void **value)
{
    int tmp;
    rcar_context_t *p_decoder = (rcar_context_t *)context;
    video_info_t *video = &p_decoder->video;

    pthread_mutex_lock(&p_decoder->mutex);

    switch(prop) {
        case CAPTURE_PROPERTY_CURRENT_NORM:
            tmp = video->signal;
            if(tmp == 0) {
                *value = (void*)norm_none;
            }
            else {
                *value = (void*)video->format;
            }
            break;
        case CAPTURE_PROPERTY_DEVICE_INFO:
            strcpy((char*)(value), DECODER_INFO);
            break;
        default:
            errno = ENOTSUP;
            pthread_mutex_unlock(&p_decoder->mutex);
            return -1;
    }

    pthread_mutex_unlock(&p_decoder->mutex);

    return 0;
}

int capture_set_property_i(capture_context_t context, uint32_t prop, int32_t value)
{
    rcar_context_t *p_decoder = (rcar_context_t *)context;
    video_info_t *video = &p_decoder->video;

    pthread_mutex_lock(&p_decoder->mutex);

    switch(prop) {
        case CAPTURE_PROPERTY_BOARD_ID:
            p_decoder->board_id = value;
            break;
        case CAPTURE_PROPERTY_DEVICE:
            p_decoder->active_dev = value;
            video->active_dev = value;
            break;
        case CAPTURE_PROPERTY_SRC_INDEX:
            p_decoder->source_idx = value;
            video->source_idx = value;
            video->csi_ch = value;
            break;
        case CAPTURE_PROPERTY_CSI2_NUM_DATA_LANES:
            video->csi_lanes = value;
            break;
        case CAPTURE_PROPERTY_CSI2_PHY_MODE:
            video->csi_phy_mode = value;
            break;
        case CAPTURE_ENABLE:
            p_decoder->enable = value ? 1 : 0;
            break;
        case CAPTURE_PROPERTY_DST_WIDTH:
            if(value >= 1280)
                video->source_std = VIN_INPUT_STD_HDTV;
            else if(value == 720)
                video->source_std = VIN_INPUT_STD_SDTV;
            else
                video->source_std = VIN_INPUT_STD_VGA;
            break;
        case CAPTURE_PROPERTY_CONTRAST:
            video->con = value;
            video->update |= DECODER_COLOR_CON_UPDATE;
            break;
        case CAPTURE_PROPERTY_HUE:
            video->hue = value;
            video->update |= DECODER_COLOR_HUE_UPDATE;
            break;
        case CAPTURE_PROPERTY_BRIGHTNESS:
            video->bri = value;
            video->update |= DECODER_COLOR_BRI_UPDATE;
            break;
        case CAPTURE_PROPERTY_SATURATION:
            video->sat = value;
            video->update |= DECODER_COLOR_SAT_UPDATE;
            break;
        case CAPTURE_PROPERTY_GMSL_MODE:
            video->gmsl_mode = value;
            break;
        default:
            errno = ENOTSUP;
            pthread_mutex_unlock(&p_decoder->mutex);
            return -1;
    }

    pthread_mutex_unlock(&p_decoder->mutex);

    return 0;
}

int capture_set_property_p(capture_context_t context, uint32_t prop, void *value)
{
    errno = ENOTSUP;

    return -1;
}
