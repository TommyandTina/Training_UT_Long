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

#include "board.h"

#define SOC_LIB	            "libcapture-soc-rcar3.so"

static context_api_t  decoder_api[DECODER_NUMBER];
static context_api_t  rcarsoc_api[DECODER_NUMBER];
static rcar_context_t *decoder_context[DECODER_NUMBER] = { NULL, NULL };
static rcar_context_t *soc_context[DECODER_NUMBER] = { NULL, NULL };
static int open_count[DECODER_NUMBER] = { 0, 0 };
pthread_mutex_t rcar_mutex[DECODER_NUMBER] = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER };
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int load_lib(void* hdl, const char* pathname, int mode)
{
    context_api_t *hdl_api = (context_api_t*)hdl;

    hdl_api->hdl = dlopen(pathname, mode);
    if(hdl_api->hdl == NULL ){
        fprintf(stderr, "%s: DLL Error %s\n",__FUNCTION__, dlerror());
        return -1;
    }
    hdl_api->capture_create_context = dlsym(hdl_api->hdl, "capture_create_context");
    hdl_api->capture_destroy_context = dlsym(hdl_api->hdl, "capture_destroy_context");
    hdl_api->capture_is_property = dlsym(hdl_api->hdl, "capture_is_property");
    hdl_api->capture_update = dlsym(hdl_api->hdl, "capture_update");
    hdl_api->capture_set_property_i = dlsym(hdl_api->hdl, "capture_set_property_i");
    hdl_api->capture_set_property_p = dlsym(hdl_api->hdl, "capture_set_property_p");
    hdl_api->capture_get_property_i = dlsym(hdl_api->hdl, "capture_get_property_i");
    hdl_api->capture_get_property_p = dlsym(hdl_api->hdl, "capture_get_property_p");
    hdl_api->capture_get_frame = dlsym(hdl_api->hdl, "capture_get_frame");
    hdl_api->capture_release_frame = dlsym(hdl_api->hdl, "capture_release_frame");
    hdl_api->capture_create_buffers = dlsym(hdl_api->hdl, "capture_create_buffers");
    hdl_api->capture_put_buffer = dlsym(hdl_api->hdl, "capture_put_buffer");
    hdl_api->capture_init = dlsym(hdl_api->hdl, "capture_init");
    if (hdl_api->capture_create_context == 0	||
        hdl_api->capture_destroy_context == 0	||
        hdl_api->capture_is_property == 0 		||
        hdl_api->capture_update == 0	 		||
        hdl_api->capture_set_property_i == 0	||
        hdl_api->capture_set_property_p == 0 	||
        hdl_api->capture_get_property_i == 0 	||
        hdl_api->capture_get_property_p == 0 	||
        hdl_api->capture_get_frame == 0 		||
        hdl_api->capture_release_frame == 0 	||
        hdl_api->capture_create_buffers == 0 	||
        hdl_api->capture_put_buffer == 0) {
        fprintf(stderr, "%s: Couldn't find all the necessary library %s functions\n", __FUNCTION__, pathname);
        return -1;
        dlclose(hdl_api->hdl);
    }

    return 0;
}

capture_context_t capture_create_context(uint32_t flags)
{
    rcar_context_t *rcar_context;
    int i;

    pthread_mutex_lock(&mutex);

    if((rcar_context = calloc(1, sizeof(*rcar_context))) == NULL) {
        fprintf(stderr, "%s: calloc failed\n", __FUNCTION__);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    rcar_context->n_devices = DECODER_NUMBER;

    for (i = 0; i < rcar_context->n_devices; i++) {
        if (!rcarsoc_api[i].hdl) {
            if (load_lib((void*)&rcarsoc_api[i], SOC_LIB, 0) == -1 ) {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
        }

        if (!decoder_api[i].hdl) {
            if (load_lib((void*)&decoder_api[i], decoder_info[i].dec_lib, 0) == -1) {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
        }

        if (!soc_context[i]) {
            if ((soc_context[i] = (rcar_context_t *)rcarsoc_api[i].capture_create_context(flags)) == NULL) {
                fprintf(stderr, "%s: rcarsoc_api.capture_create_context\n", __FUNCTION__);
                goto fail;
            }
        }

        if (!decoder_context[i]) {
            if ((decoder_context[i] = (rcar_context_t *)decoder_api[i].capture_create_context(flags)) == NULL) {
                fprintf(stderr, "%s: decoder_api.capture_create_context\n", __FUNCTION__);
                goto fail;
            }
        }
    }

    rcar_context->active_dev = -1;
    rcar_context->source_idx = 0;
    rcar_context->enable     = 0;
    rcar_context->is_running = 0;
    rcar_context->mutex = &rcar_mutex[0];
    pthread_mutex_unlock(&mutex);

    return rcar_context;

fail:
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int capture_create_buffers(capture_context_t context, uint32_t property)
{
    rcar_context_t *p_context = (rcar_context_t*)context;
    capture_context_t s_context = (capture_context_t)p_context->soc_context;
    context_api_t *soc_api = (context_api_t *)p_context->soc_api;

    return soc_api->capture_create_buffers(s_context, property);
}

void capture_destroy_context(capture_context_t context)
{
    rcar_context_t *p_context = (rcar_context_t*)context;
    capture_context_t d_context;
    capture_context_t s_context;
    context_api_t *soc_api;
    context_api_t *dec_api;

    pthread_mutex_lock(p_context->mutex);

    if (--open_count[p_context->active_dev] <= 0) {
        if(p_context) {
            d_context = (capture_context_t)p_context->dec_context;
            s_context = (capture_context_t)p_context->soc_context;
            soc_api = (context_api_t *)p_context->soc_api;
            dec_api = (context_api_t *)p_context->dec_api;

            if(soc_api->hdl && s_context) {
                soc_api->capture_destroy_context(s_context);
            }

            if(decoder_api->hdl && d_context) {
                dec_api->capture_destroy_context(d_context);

                dlclose(dec_api->hdl);
            }

            if(soc_api->hdl) {
                dlclose(soc_api->hdl);
            }

            p_context->dec_context = NULL;
            p_context->soc_context = NULL;
            p_context->soc_api = NULL;
            p_context->dec_api = NULL;
            free(p_context);
        }
    }

    pthread_mutex_lock(p_context->mutex);
}

int capture_update(capture_context_t context, uint32_t flags)
{
    rcar_context_t *p_context;
    capture_context_t d_context;
    capture_context_t s_context;
    context_api_t *soc_api;
    context_api_t *dec_api;
    int dec_idx, src_idx;
    uint32_t input_type;
    int  srcw;
    int  srch;
    int  interlace = 0;
    int  csi_rate;
    uint32_t fmt;
    char *std;
    int gmsl_support_idx;

    p_context = (rcar_context_t*)context;
    dec_idx = p_context->active_dev;
    src_idx = 0;

    d_context = (capture_context_t)p_context->dec_context;
    s_context = (capture_context_t)p_context->soc_context;
    soc_api = (context_api_t *)p_context->soc_api;
    dec_api = (context_api_t *)p_context->dec_api;

    pthread_mutex_lock(p_context->mutex);

    /* Setup decoder */
    if(dec_api->capture_update(d_context, flags)) {
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }

    /* Tell SoC about input video */
    if(dec_api->capture_get_property_p(d_context, CAPTURE_PROPERTY_CURRENT_NORM, (void**)&std)) {
        fprintf(stderr, "%s: Get video standard failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }
    if(soc_api->capture_set_property_p(s_context, CAPTURE_PROPERTY_NORM, (void*)std)) {
        fprintf(stderr, "%s: Set video standard failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }	
    if(dec_api->capture_get_property_i(d_context, CAPTURE_PROPERTY_CSI2_DDR_CLOCK_MHZ, &csi_rate)) {
        fprintf(stderr, "%s: Get csi bus width failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }

    if(soc_api->capture_set_property_i(s_context, CAPTURE_PROPERTY_CSI2_DDR_CLOCK_MHZ, csi_rate)) {
        fprintf(stderr, "%s: Set source format failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }

    if(dec_api->capture_get_property_i(d_context, CAPTURE_PROPERTY_SRC_WIDTH, &srcw)) {
        fprintf(stderr, "%s: Get source width size failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }
    if(soc_api->capture_set_property_i(s_context, CAPTURE_PROPERTY_SRC_WIDTH, srcw)) {
        fprintf(stderr, "%s: Set source width size failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }
    if(dec_api->capture_get_property_i(d_context, CAPTURE_PROPERTY_SRC_HEIGHT, &srch)) {
        fprintf(stderr, "%s: Get source height size failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }
    if(soc_api->capture_set_property_i(s_context, CAPTURE_PROPERTY_SRC_HEIGHT, srch)) {
        fprintf(stderr, "%s: Set source height size failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }

    if(dec_api->capture_get_property_i(d_context, CAPTURE_PROPERTY_FRAME_FLAGS, &interlace)) {
        fprintf(stderr, "%s: Get source format failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }
    if(soc_api->capture_set_property_i(s_context, CAPTURE_PROPERTY_FRAME_FLAGS, interlace)) {
        fprintf(stderr, "%s: Set source format failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }
    if (dec_api->capture_get_property_i(d_context, CAPTURE_PROPERTY_GMSL_MODE, &gmsl_support_idx)) {
        fprintf(stderr, "%s: Get GMSL mode failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }

    fmt = (decoder_info[dec_idx].dec_cfg[src_idx].yuv_input << 31) |
          (decoder_info[dec_idx].dec_cfg[src_idx].vi_format[gmsl_support_idx]);

    input_type = decoder_info[dec_idx].dec_cfg[src_idx].type;

    if(input_type & INPUT_FROM_HDMI) {
        if(!interlace) {
            fmt |= RCAR_VIN_MC_IM_ODD_EVEN;
        } else {
            fmt |= RCAR_VIN_MC_IM_FULL;
        }
    } else if(input_type & INPUT_FROM_CVBS) {
        fmt |= RCAR_VIN_MC_IM_FULL;
    } else {
        fmt |= RCAR_VIN_MC_IM_ODD_EVEN;
    }

    if(soc_api->capture_set_property_i(s_context, CAPTURE_PROPERTY_SRC_FORMAT, fmt)) {
        fprintf(stderr, "%s: Set source format failed\n", __FUNCTION__);
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }

    if (decoder_info[dec_idx].dec_cfg[src_idx].csi_phy_mode != 0) {
        if(soc_api->capture_set_property_i(s_context, CAPTURE_PROPERTY_CSI2_PHY_MODE, decoder_info[dec_idx].dec_cfg[src_idx].csi_phy_mode)) {
            fprintf(stderr, "%s: Set CSI PHY mode failed\n", __FUNCTION__);
            pthread_mutex_unlock(p_context->mutex);
            return -1;
        }
    }

    /* Enable VIN */
    if(soc_api->capture_update(s_context, flags)) {
        return -1;
    }

    /* Enable decoder */
    if(dec_api->capture_update(d_context, flags)) {
        return -1;
    }

    /* Update VIN */
    if(soc_api->capture_update(s_context, flags)) {
        return -1;
    }

    p_context->is_running = 1;

    pthread_mutex_unlock(p_context->mutex);

    return 0;
}

int capture_get_frame(capture_context_t context, uint64_t timeout, uint32_t flags)
{
    rcar_context_t *p_context = (rcar_context_t *)context;
    capture_context_t s_context = (capture_context_t)p_context->soc_context;
    capture_context_t d_context = (capture_context_t)p_context->dec_context;
    context_api_t *soc_api = (context_api_t *)p_context->soc_api;
    context_api_t *dec_api = (context_api_t *)p_context->dec_api;
    int ret = 0;

    pthread_mutex_lock(p_context->mutex);

    if (dec_api->capture_get_frame(d_context, timeout, flags))
        ret = soc_api->capture_get_frame(s_context, timeout, flags);
    else
        ret = -1;

    pthread_mutex_unlock(p_context->mutex);

    return ret;
}

int capture_release_frame(capture_context_t context, uint32_t idx)
{
    rcar_context_t *p_context = (rcar_context_t*)context;
    capture_context_t s_context = (capture_context_t)p_context->soc_context;
    context_api_t *soc_api = (context_api_t *)p_context->soc_api;

    return soc_api->capture_release_frame(s_context, idx);
}

int capture_put_buffer(capture_context_t ctx, uint32_t idx, uint32_t flags)
{
    rcar_context_t *p_context = (rcar_context_t *)ctx;
    capture_context_t s_context = (capture_context_t)p_context->soc_context;
    context_api_t *soc_api = (context_api_t *)p_context->soc_api;
    int ret  = 0;
    pthread_mutex_lock(p_context->mutex);
    ret = soc_api->capture_put_buffer(s_context, idx, flags);
    pthread_mutex_unlock(p_context->mutex);
    return ret;
}

int capture_is_property(capture_context_t context, uint32_t prop)
{
    rcar_context_t *p_context;
    capture_context_t d_context;
    capture_context_t s_context;
    context_api_t *soc_api;
    context_api_t *dec_api;
    int ret = 0;

    p_context = (rcar_context_t*)context;
    d_context = (capture_context_t)p_context->dec_context;
    s_context = (capture_context_t)p_context->soc_context;
    soc_api = (context_api_t *)p_context->soc_api;
    dec_api = (context_api_t *)p_context->dec_api;

    pthread_mutex_lock(p_context->mutex);

    if(dec_api->capture_is_property(d_context, prop)) {
        ret = 1;
    } else if(soc_api->capture_is_property(s_context, prop)) {
        ret = 1;
    }

    pthread_mutex_unlock(p_context->mutex);

    return ret;
}

int capture_get_property_i(capture_context_t context, uint32_t prop, int32_t *value)
{
    rcar_context_t *p_context;
    capture_context_t d_context;
    capture_context_t s_context;
    context_api_t *soc_api;
    context_api_t *dec_api;
    int decoder_val = 0;
    int soc_val = 0;

    p_context = (rcar_context_t*)context;     
    d_context = (capture_context_t)p_context->dec_context;
    s_context = (capture_context_t)p_context->soc_context;
    soc_api = (context_api_t *)p_context->soc_api;
    dec_api = (context_api_t *)p_context->dec_api;

    if(value == NULL || p_context->active_dev == -1) {
        errno = EINVAL;
        return -1;
    }

    pthread_mutex_lock(p_context->mutex);

    switch(prop) {
        case CAPTURE_PROPERTY_NDEVICES:
            *value = p_context->n_devices;
            break;
        case CAPTURE_ENABLE:
            if((dec_api->capture_get_property_i(d_context, prop, &decoder_val)) ||
                soc_api->capture_get_property_i(s_context, prop, &soc_val)) {
                    errno = ENOTSUP;
                    pthread_mutex_unlock(p_context->mutex);
                    return -1;
                }
            *value = decoder_val & soc_val;
            break;
        default:
            if(dec_api->capture_get_property_i(d_context, prop, &decoder_val)) {
                if(soc_api->capture_get_property_i(s_context, prop, &soc_val)) {
                    errno = ENOTSUP;
                    pthread_mutex_unlock(p_context->mutex);
                    return -1;
                }
                *value = soc_val;
                break;
            }
            *value = decoder_val;
            break;
    }

    pthread_mutex_unlock(p_context->mutex);

    return 0;
}

int capture_get_property_p(capture_context_t context, uint32_t prop, void **value)
{
    rcar_context_t *p_context;
    capture_context_t d_context;
    capture_context_t s_context;
    context_api_t *soc_api;
    context_api_t *dec_api;
    char decoder_info[20];

    p_context = (rcar_context_t*)context;
    d_context = (capture_context_t)p_context->dec_context;
    s_context = (capture_context_t)p_context->soc_context;
    soc_api = (context_api_t *)p_context->soc_api;
    dec_api = (context_api_t *)p_context->dec_api;

    if(value == NULL || p_context->active_dev == -1) {
        errno = EINVAL;
        return -1;
    }

    pthread_mutex_lock(p_context->mutex);

    if(prop == CAPTURE_PROPERTY_DEVICE_INFO) {
        if(*value == NULL) {
            *value = calloc(1, 20);
        }
        if(dec_api->capture_get_property_p(d_context, prop, (void**)&decoder_info)) {
            errno = ENOTSUP;
            pthread_mutex_unlock(p_context->mutex);
            return -1;
        }
        snprintf((char*)(*value), 20, "%s", decoder_info);
    }
    else {
        if(dec_api->capture_get_property_p(d_context, prop, value)) {
            if(soc_api->capture_get_property_p(s_context, prop, value)) {
                errno = ENOTSUP;
                pthread_mutex_unlock(p_context->mutex);
                return -1;
            }
        }
    }
    pthread_mutex_unlock(p_context->mutex);
    return 0;
}

int capture_set_property_i( capture_context_t context, uint32_t prop, int32_t value)
{
    rcar_context_t *p_context;
    capture_context_t d_context;
    capture_context_t s_context;
    context_api_t *soc_api;
    context_api_t *dec_api;
    int index;

    p_context = (rcar_context_t*)context;
    d_context = (capture_context_t)p_context->dec_context;
    s_context = (capture_context_t)p_context->soc_context;
    soc_api = (context_api_t *)p_context->soc_api;
    dec_api = (context_api_t *)p_context->dec_api;

    if(prop != CAPTURE_PROPERTY_DEVICE && p_context->active_dev == -1) {
        errno = EINVAL;
        return -1;
    }

    pthread_mutex_lock(p_context->mutex);

    switch(prop) {
    case CAPTURE_PROPERTY_DEVICE:
        if (value == p_context->active_dev) {
            pthread_mutex_unlock(p_context->mutex);
            return 0;
        }
        if (value < p_context->n_devices) {
            if (d_context && s_context && p_context->is_running) {
                fprintf(stderr, "%s: This device is enabled already, cannot change device to %d\n", __FUNCTION__, value);
                pthread_mutex_unlock(p_context->mutex);
                return -1;
            }
            pthread_mutex_unlock(p_context->mutex);
            ++open_count[value];
            p_context->active_dev = value;
            index = p_context->active_dev;
            p_context->mutex = &rcar_mutex[index];
            pthread_mutex_lock(p_context->mutex);
            p_context->dec_context = decoder_context[index];
            p_context->soc_context = soc_context[index];
            p_context->dec_api = &decoder_api[index];
            p_context->soc_api = &rcarsoc_api[index];
            d_context = (capture_context_t)p_context->dec_context;
            s_context = (capture_context_t)p_context->soc_context;
            soc_api = (context_api_t *)p_context->soc_api;
            dec_api = (context_api_t *)p_context->dec_api;
            soc_api->capture_init(p_context->soc_context, index, BOARD_ID, &decoder_info[index].dec_cfg[0]);
            dec_api->capture_init(p_context->dec_context, index, BOARD_ID, &decoder_info[index].dec_cfg[0]);
            if (soc_api->capture_set_property_i(s_context, CAPTURE_PROPERTY_DECODER_ID, decoder_info[index].id)) {
               errno = ENOTSUP;
               pthread_mutex_unlock(p_context->mutex);
               return -1;
            }
            if (dec_api->capture_set_property_i(d_context, CAPTURE_PROPERTY_DEVICE, index)) {
                errno = ENOTSUP;
                pthread_mutex_unlock(p_context->mutex);
                return -1;
            }
        } else {
            fprintf(stderr, "%s: No support decoder device %d\n", __FUNCTION__, value);
            errno = EINVAL;
            pthread_mutex_unlock(p_context->mutex);
            return -1;
        }
        break;
    case CAPTURE_PROPERTY_SRC_INDEX:
        if (soc_api->capture_set_property_i(s_context, prop, value) ||
            dec_api->capture_set_property_i(d_context, prop, value)) {
            errno = ENOTSUP;
            pthread_mutex_unlock(p_context->mutex);
            return -1;
        }
        break;
    case CAPTURE_PROPERTY_DST_WIDTH:
        if (!soc_api->capture_set_property_i(s_context, prop, value)) {
            if(dec_api->capture_set_property_i(d_context, prop, value)) {
                errno = ENOTSUP;
                pthread_mutex_unlock(p_context->mutex);
                return -1;
            }
        }
        break;
    case CAPTURE_ENABLE:
        if ( dec_api->capture_set_property_i(d_context, prop, value) ||
             soc_api->capture_set_property_i(s_context, prop, value)) {
            errno = ENOTSUP;
            pthread_mutex_unlock(p_context->mutex);
            return -1;
        }
        p_context->enable = value ? 1 : 0;
        break;
    case CAPTURE_PROPERTY_IPMMU_SUPPORT:
        if (soc_api->capture_set_property_i(s_context, prop, value)) {
            errno = ENOTSUP;
            pthread_mutex_unlock(p_context->mutex);
            return -1;
        }
        break;
    case CAPTURE_PROPERTY_GMSL_MODE:
        if (dec_api->capture_set_property_i(d_context, prop, value)) {
            errno = ENOTSUP;
            pthread_mutex_unlock(p_context->mutex);
            return -1;
        }
        break;
    default:
        if (dec_api->capture_set_property_i(d_context, prop, value) &&
            soc_api->capture_set_property_i(s_context, prop, value)) {
            errno = ENOTSUP;
            pthread_mutex_unlock(p_context->mutex);
            return -1;
        }
        break;
    }

    pthread_mutex_unlock(p_context->mutex);

    return 0;
}

int capture_set_property_p( capture_context_t context, uint32_t prop, void *value )
{
    rcar_context_t *p_context;
    capture_context_t d_context;
    capture_context_t s_context;
    context_api_t *soc_api;
    context_api_t *dec_api;

    p_context = (rcar_context_t*)context;
    d_context = (capture_context_t)p_context->dec_context;
    s_context = (capture_context_t)p_context->soc_context;
    soc_api = (context_api_t *)p_context->soc_api;
    dec_api = (context_api_t *)p_context->dec_api;

    if(p_context->active_dev == -1) {
        errno = EINVAL;
        return -1;
    }

    pthread_mutex_lock(p_context->mutex);

    if (dec_api->capture_set_property_p(d_context, prop, value) &&
        soc_api->capture_set_property_p(s_context, prop, value)) {
        errno = ENOTSUP;
        pthread_mutex_unlock(p_context->mutex);
        return -1;
    }

    pthread_mutex_unlock(p_context->mutex);

    return 0;
}
