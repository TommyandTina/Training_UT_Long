/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation
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

pthread_mutex_t mutex;

capture_context_t capture_create_context(uint32_t flags)
{
    ThreadCtl(_NTO_TCTL_IO, 0);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);

    if((rcar_context = calloc(1, sizeof(rcar_context_t))) == NULL) {
        fprintf(stderr, "%s: calloc failed\n", __FUNCTION__);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    rcar_context->active_dev = 0;
    rcar_context->source_idx = 0;
    rcar_context->is_runing = 0;
    rcar_context->enable = 0;

    pthread_mutex_unlock(&mutex);
    return rcar_context;
}

void enable_hdmi_audio(capture_context_t context)
{
   rcar_context_t *p_soc = (rcar_context_t *)context;
   rcar_vin_t *vin = &p_soc->vin;

    if(vin->input_type & INPUT_FROM_HDMI) {
        if(vin->output_type == DISPLAY_TYPE_HDMI) {
            if(vin->screen_idx == DISPLAY_ID_HDMI0) {
                audio_setup(OUPUT_VIA_HDMI0, vin->audio_ssi_chan);
                audio_start();
            }
            else if(vin->screen_idx == DISPLAY_ID_HDMI1) {
                if(vin->audio_support == 2) {
                    audio_setup(OUPUT_VIA_HDMI1, vin->audio_ssi_chan);
                    audio_start();
                }
            }
        } else if((vin->output_type == DISPLAY_TYPE_COMPONENT_RGB) || (vin->output_type == DISPLAY_TYPE_OTHER)) {
            /* Setup audio HDMI0/HDMI1 using for LVDS/VGA output */
            audio_setup(OUPUT_VIA_HDMI0, vin->audio_ssi_chan);
            if(vin->audio_support == 2) {
                audio_setup(OUPUT_VIA_HDMI1, vin->audio_ssi_chan);
            }
            audio_start();
        }
    }
}

void disable_hdmi_audio(capture_context_t context)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;
    
    if((vin->audio_support) && ((vin->output_type== DISPLAY_TYPE_HDMI) || 
        (vin->output_type == DISPLAY_TYPE_COMPONENT_RGB) || (vin->output_type == DISPLAY_TYPE_OTHER))) {
            audio_stop();
            audio_deinit();
    }
}


int capture_create_buffers(capture_context_t context, uint32_t property)
{
    errno = ENOTSUP;
    return -1;
}

void capture_destroy_context(capture_context_t context)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    
    if(p_soc) {
        pthread_mutex_lock(&mutex);
        disable_hdmi_audio(context);
        rcar_vin_fini(&context->vin);
        free(p_soc);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
}

int capture_update(capture_context_t context, uint32_t flags)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;

    pthread_mutex_lock(&mutex);

    if((p_soc->enable) && (!p_soc->is_runing)) {
        if(rcar_vin_init(vin)) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }

        if(vin->audio_support) {
            /* Enable audio via HDMI */
            if(vin->input_type & INPUT_FROM_HDMI) {
                enable_hdmi_audio(context);
            }
        }

        p_soc->is_runing = 1;
    }
    else if((!p_soc->enable) && (p_soc->is_runing)) {
        disable_hdmi_audio(context);
        if(rcar_vin_fini(vin)) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
        p_soc->is_runing = 0;
        return 0;
    }

    if(rcar_vin_update(vin)) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    pthread_mutex_unlock(&mutex);
    
    return 0;	
}

int capture_get_frame(capture_context_t context, uint64_t timeout, uint32_t flags)
{	
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;
    int ret;
    
    pthread_mutex_lock(&mutex);

    if(p_soc->enable == 0) {
        pthread_mutex_unlock(&mutex);
        errno = ECANCELED;
        return -1;
    }
    ret = rcar_get_frame(vin, timeout, flags);

    pthread_mutex_unlock(&mutex);

    return ret;
}

int capture_release_frame(capture_context_t context, uint32_t idx)
{	
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;

    return rcar_release_frame(vin, idx);
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
        {
            return 1;
        }
    }

    return 0;
}

int capture_get_property_i(capture_context_t context, uint32_t prop, int32_t *value)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;

    switch(prop)
    {
            break;
        case CAPTURE_ENABLE:
            *value = p_soc->enable;
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
            p_soc->active_dev = value & 0xF;
            vin->screen_idx = (value >> 4) & 0xF;
            vin->board = (value >> 8) & 0xF;
            vin->decoder = (value >> 12) & 0xF;
            vin->output_type = (value >> 16) & 0xFFFF;
            break;
        case CAPTURE_PROPERTY_SRC_INDEX:
            p_soc->source_idx = value;
            //vin->csi2_ch = value;
            break;
        case CAPTURE_ENABLE:
            p_soc->enable = value;
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
            vin->frm_nbufs = value;
            break;
        case CAPTURE_PROPERTY_CSI2_DDR_CLOCK_MHZ:
            vin->csi_rate = value;
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
            for(int i = 0; i < RCAR_VIN_NUM; i++){
                vin->frm_bufs[i] = (void**)value + (i * 4);
            }
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


int capture_set_input_info(capture_context_t context, void *info_addr)
{
    rcar_context_t *p_soc = (rcar_context_t *)context;
    rcar_vin_t *vin = &p_soc->vin;
    input_attribute_t *info = (input_attribute_t *)info_addr;
    
    vin->vin_num        = info->vin_num;
    vin->vin_chan       = info->vin_chan;
    vin->csi2_chan      = info->csi_chan;
    vin->csi_lanes      = info->csi_lanes;
    vin->scale_support  = info->scale_support;
    vin->audio_support  = info->audio_support;
    vin->audio_ssi_chan = info->audio_ssi_chan;
    vin->input_type     = info->type;
            
    return 0;
}

