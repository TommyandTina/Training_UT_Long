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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <screen/screen.h>
#include <vcapture/capture.h>
#include <vcapture/capture-adv-ext.h>
#include <errno.h>
#include <vin.h>

#define MAX_BUF_DISPLAY   4
#define MAX_WINDOW_SCREEN 4
#define MAX_DEVICES       2
#define MAX9286_INFO "MAX9286"
#define MAX96712_INFO "MAX96712"
#define CAPTURE_TIMEOUT_NSECOND 100000000

screen_display_t *screen_displays = NULL;
screen_display_t screen_disp = NULL;
screen_buffer_t screen_buf[MAX_DEVICES][MAX_WINDOW_SCREEN][MAX_BUF_DISPLAY];
static int app_abort = 1;
int32_t screen_id = 0;
int32_t pipeline = -1;
int32_t device = -1;
int32_t window_width = -1, window_height = -1;
int32_t window_x = 0, window_y = 0;
int32_t view_width = -1, view_height = -1;
int32_t view_x = 0, view_y = 0;
int32_t dst_format = SCREEN_FORMAT_UYVY;
int32_t crop_height = -1,crop_width = -1;
int32_t crop_x = -1, crop_y = -1;
int32_t src_width = -1, src_height = -1;
int32_t dst_width = -1, dst_height = -1;
int32_t color_saturation = 4095;
int32_t color_hue = 4095;
int32_t brightness = 4095;
int32_t contrast = 4095;
int32_t cam_source = -1;
int32_t zorder = 0;
int32_t num_windows = 1;
int32_t frame_nbuffers = 0;
int32_t vin_nbufs = 0;
int32_t rect_buf[4] = { 0, 0, 0, 0 };
int32_t dst_stride;
int32_t ipmmu = 0;
int32_t gmsl = 1;
unsigned char *frame_buffers[MAX_DEVICES][MAX_WINDOW_SCREEN][MAX_BUF_DISPLAY];

screen_context_t screen_ctx = NULL;
screen_window_t screen_video[MAX_DEVICES][MAX_WINDOW_SCREEN];
capture_context_t capture_ctx[MAX_DEVICES] = { NULL, NULL };

void *signal_thread (void *arg)
{
    pthread_t t1, t2;
    sigset_t signals;
    int sig; 
    int result;
    t1 = pthread_self();
    pthread_setname_np(t1, "signal_thread");
    t2 = pthread_self();
    pthread_detach(t2);
    sigaddset (&signals,  SIGINT); 
    sigaddset (&signals,  SIGTERM);
    do
        result = sigwait (&signals, &sig);
    while ( result);
    app_abort = 0;

    return 0;
}

static int camera_getsubopt(char **optionp, char * const *tokens, char **valuep)
{
    char *p, *opt;
    int len, index;
    const char *token;

    *valuep = 0;

    for(opt = p = *optionp, len = 0; *p && *p != ' ' && *p != '\t'; p++, len++) 
    {
        if(*p == '=') 
        {
            for(*valuep = ++p; *p && *p != ' ' && *p != '\t'; p++) 
            {
                /* Nothing to do */
            }
            break;
        }
    }
    if(*p) 
    {
        *p++ = '\0';
    }

    for (index = 0; (token = *tokens++); index++) 
    {
        if(*token == *opt && !strncmp(token, opt, len) && token[len] == '\0') 
        {
            return index;
        }
    }
    *valuep = opt;
    return -1;
}

static int screen_parse_argument(char **options)
{
    char *value, *cp;
    int opt;

    static char *opts[] = {
                /*  0 */ "-format",      /* Color Formats */
                /*  1 */ "-device",      /*  */
                /*  2 */ "-size",        /*  */
                /*  3 */ "-display",     /*  */
                /*  4 */ "-ssize",       /*  */
                /*  5 */ "-csize",       /*  */
                /*  6 */ "-bsize",       /*  */
                /*  7 */ "-pos",         /*  */
                /*  8 */ "-cpos",        /*  */
                /*  9 */ "-spos",        /*  */
                /* 10 */ "-pipeline",    /*  */
                /* 11 */ "-brightness",  /*  */
                /* 12 */ "-contrast",    /*  */
                /* 13 */ "-hue",         /*  */
                /* 14 */ "-saturation",  /*  */
                /* 15 */ "-zorder",      /*  */
                /* 16 */ "-source",      /*  */
                /* 17 */ "-ipmmu",       /*  */
                /* 18 */ "-gmsl",        /*  */
                         NULL
                };

    while (options && *options != NULL) {
        if((opt = camera_getsubopt(options++, opts, &value)) == -1) { }
        switch (opt) 
        {
            case 0:
                if((!strcmp((const char *)value, "rgba8888")) ||
                   (!strcmp((const char *)value, "yuv10"))) 
                {
                    dst_format = SCREEN_FORMAT_RGBA8888;
                }
                else if(!strcmp((const char *)value, "rgb565")) 
                {
                    dst_format = SCREEN_FORMAT_RGB565;
                }
                else if(!strcmp((const char *)value, "rgba5551")) 
                {
                    dst_format = SCREEN_FORMAT_RGBA5551;
                }
                else if(!strcmp((const char *)value, "uyvy")) 
                {
                    dst_format = SCREEN_FORMAT_UYVY;
                }
                else 
                {
                    printf("rearview-camera: invalid format %s\n", value);
                }
                break;
            case 1:
                device = strtoul(value, NULL, 0);               
                break;
            case 2:
                window_width = strtoul(value, NULL, 0);
                if((cp = strchr(value, 'x'))) 
                {
                    value = cp+1;
                    window_height = strtoul(value, NULL, 0);
                }
                break;
            case 3:
                screen_id = strtoul(value, NULL, 0);              
                break;
            case 4:
                view_width = strtoul(value, NULL, 0);
                if((cp = strchr(value, 'x'))) 
                {
                    value = cp+1;
                    view_height = strtoul(value, NULL, 0);
                }
                break;
            case 5:
                crop_width = strtoul(value, NULL, 0);
                if((cp = strchr(value, 'x'))) 
                {
                    value = cp+1;
                    crop_height = strtoul(value, NULL, 0);
                }
                break;
            case 6:
                dst_width = strtoul(value, NULL, 0);
                if((cp = strchr(value, 'x'))) 
                {
                    value = cp+1;
                    dst_height = strtoul(value, NULL, 0);
                }
                break;
            case 7:
                window_x = strtoul(value, NULL, 0);
                if((cp = strchr(value, 'x'))) 
                {
                    value = cp+1;
                    window_y = strtoul(value, NULL, 0);
                }
                break;
            case 8:
                crop_x = strtoul(value, NULL, 0);
                if((cp = strchr(value, 'x'))) 
                {
                    value = cp+1;
                    crop_y = strtoul(value, NULL, 0);
                }
                break;
            case 9:
                view_x = strtoul(value, NULL, 0);
                if((cp = strchr(value, 'x'))) 
                {
                    value = cp+1;
                    view_y = strtoul(value, NULL, 0);
                }
                break;
            case 10:
                pipeline = strtoul(value, NULL, 0);
                break;
            case 11:
                brightness = strtoul(value, NULL, 0);
                break;
            case 12:
                contrast = strtoul(value, NULL, 0);
                break;
            case 13:
                color_hue = strtoul(value, NULL, 0);
                break;
            case 14:
                color_saturation = strtoul(value, NULL, 0);
                break;
            case 15:
                zorder = strtoul(value, NULL, 0);
                break;
            case 16:
                cam_source = strtoul(value, NULL, 0);
                break;
            case 17:
                if(!strcmp((const char *)value, "on") || !strcmp((const char *)value, "1")) {
                    ipmmu = 1;
                }
                break;
            case 18:
                gmsl = strtoul(value, NULL, 0);
                break;
            default:
                break;
        }
    }
    
    if(dst_width == -1 || dst_height == -1) 
    {
        printf("error: buffer is not specified!\n");
        return EIO;
    }

    return (EOK);
}

static int screen_context_setup(screen_context_t *screen_ctx)
{
    int32_t screen_val = 0;
    int32_t count = 0;
    int32_t val = 0;
    int i = 0;

    if(screen_create_context(screen_ctx, SCREEN_WINDOW_MANAGER_CONTEXT))
    {
        printf("screen_create_context() failed\n");
        return EIO;
    }

    /* Get the number of supported displays with this context. */
    if(screen_get_context_property_iv(*screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &count))
    {
        printf("screen_get_context_property_iv(SCREEN_PROPERTY_DISPLAY_COUNT) failed\n");
        return EIO;
    }
    if(count > 0)
    {
        screen_displays = calloc(count, sizeof(screen_display_t));
        if(!screen_displays)
        {
            printf("Could not allocate memory for display list\n");
            return EIO;
        }
        if(screen_get_context_property_pv(*screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)screen_displays))
        {
            printf("screen_get_context_property_pv(SCREEN_PROPERTY_DISPLAYS) failed\n");
            return EIO;
        }
        if(screen_id == 0)
        {
            screen_disp = screen_displays[0];
        }/* Otherwise, determine which display has been requested for the screen shot. */
        else 
        {
            for (i = 0; i < count; i++) 
            {
                screen_get_display_property_iv(screen_displays[i], SCREEN_PROPERTY_ID, &val);
                if(val == screen_id) 
                {
                    screen_disp = screen_displays[i];
                    break;
                }
            }
        }
    }
    if(!screen_disp) 
    {
        printf("No displays\n");
        return EIO;
    }

    screen_get_display_property_iv(screen_disp, SCREEN_PROPERTY_TYPE, &screen_val);

    return EOK;
}

static int screen_video_setup(screen_context_t screen_ctx, screen_window_t *screen_video, int dev, int num_devs)
{
    int32_t usage;
    int32_t window_size[2];
    int32_t rect[4] = { 0, 0, 0, 0};
    int32_t dst_size[2];
    int32_t window_pos[8];
    int32_t nbufs = MAX_BUF_DISPLAY;
    int32_t view_pos[2];
    int32_t view_size[2];
    int     i, j;
    int     tran = SCREEN_TRANSPARENCY_NONE;

    for(i = 0; i < num_windows; i++)
    {
        if(screen_create_window(&screen_video[i], screen_ctx))
        {
            printf("screen_create_window() failed\n");
            return EIO;
        }
    }

    for(i = 0; i < num_windows; i++)
    {
        if( screen_set_window_property_iv(screen_video[i], SCREEN_PROPERTY_TRANSPARENCY, &tran) )
        {
            perror("screen_set_window_property_iv(SCREEN_PROPERTY_TRANSPARENCY)");
        }
        if(screen_set_window_property_pv(screen_video[i], SCREEN_PROPERTY_DISPLAY, (void**)&screen_disp))
        {
            printf("screen_set_window_property_ptr(SCREEN_PROPERTY_DISPLAY) failed\n");
            return EIO;
        }
        usage = SCREEN_USAGE_CAPTURE | SCREEN_USAGE_VIDEO | SCREEN_USAGE_WRITE | SCREEN_USAGE_READ;
        if(pipeline != -1)
        {
            usage = SCREEN_USAGE_CAPTURE | SCREEN_USAGE_OVERLAY | SCREEN_USAGE_VIDEO | SCREEN_USAGE_WRITE;
        }
        if(screen_set_window_property_iv(screen_video[i], SCREEN_PROPERTY_USAGE, &usage))
        {
            printf("screen_set_window_property_iv(SCREEN_PROPERTY_USAGE) failed\n");
            return EIO;
        }

        if(screen_set_window_property_iv(screen_video[i], SCREEN_PROPERTY_FORMAT, &dst_format))
        {
            printf("screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT) failed\n");
            return EIO;
        }

    }

    if((window_width == -1) || (window_height == -1))
    {
        if(screen_get_window_property_iv(screen_video[0], SCREEN_PROPERTY_SIZE, window_size))
        {
            printf("screen_get_window_property_iv(SCREEN_PROPERTY_SIZE) failed\n");
            return EIO;
        }
        window_width = window_size[0];
        window_height = window_size[1];
    }
    else
    {
        window_size[0] = window_width;
        window_size[1] = window_height;
    }

    switch (num_devs * num_windows)
    {
        case 8:
            rect[0] = window_width / 4;
            rect[1] = window_height / 2;
            if (dev == 0) {
                window_pos[0] = 0;
                window_pos[1] = 0;
                window_pos[2] = window_width / 4;
                window_pos[3] = 0;
                window_pos[4] = window_width / 2;
                window_pos[5] = window_height / 2;
                window_pos[6] = 3 * window_width / 4;
                window_pos[7] = 0;
            } else {
                window_pos[0] = 0;
                window_pos[1] = window_height / 2;
                window_pos[2] = window_width / 4;
                window_pos[3] = window_height / 2;
                window_pos[4] = window_width / 2;
                window_pos[5] = window_height / 2;
                window_pos[6] = 3 * window_width / 4;
                window_pos[7] = window_height / 2;
            }
            break;
        case 4 :
            rect[0] = window_width / 2;
            rect[1] = window_height / 2;
            window_pos[0] = 0;
            window_pos[1] = 0;
            window_pos[2] = window_width / 2;
            window_pos[3] = 0;
            window_pos[4] = 0;
            window_pos[5] = window_height / 2;
            window_pos[6] = window_width / 2;
            window_pos[7] = window_height / 2;
            break;
        case 2 :
            rect[0] = window_width / 2;
            rect[1] = window_height;
            if (dev == 0) {
                window_pos[0] = 0;
                window_pos[1] = 0;
            } else {
                window_pos[0] = window_width / 2;
                window_pos[1] = 0;
            }
            break;
        case 1:
            rect[0] = window_width;
            rect[1] = window_height;
            window_pos[0] = 0;
            window_pos[1] = 0;
            break;
        default:
            printf("Unsupport (num_devs * num_windows) = %d\n", num_devs * num_windows);
            return EIO;
    }

    if((dst_width != 0) || (dst_height != 0))
    {
        dst_size[0] = dst_width;
        dst_size[1] = dst_height;
        rect_buf[2] = dst_width;
        rect_buf[3] = dst_height;

        for(i = 0; i < num_windows ; i++)
        {
            if(screen_set_window_property_iv(screen_video[i], SCREEN_PROPERTY_BUFFER_SIZE, dst_size)){
                printf("screen_set_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE) failed\n");
                return EIO;
            }
        }
    }

    for(i = 0; i < num_windows; i++)
    {
        if(screen_set_window_property_iv(screen_video[i], SCREEN_PROPERTY_SIZE, rect))
        {
            printf("screen_set_window_property_iv(SCREEN_PROPERTY_SIZE) failed\n");
            return EIO;
        }
    }

    for (int i = 0; i < num_windows; i++) 
    {
        if(screen_set_window_property_iv(screen_video[i], SCREEN_PROPERTY_POSITION, window_pos + 2 * i))
        {
            printf("screen_set_window_property_iv(SCREEN_PROPERTY_POSITION) for screen_video[%d] failed\n", i);
            return EIO;
        }
    }

    if(pipeline != -1)
    { 
        for(i = 0; i < num_windows; i++)
        {
            if(screen_set_window_property_iv(screen_video[i], SCREEN_PROPERTY_PIPELINE, &pipeline))
            {
                printf("screen_set_window_property_iv(SCREEN_PROPERTY_PIPELINE)failed\n");
                return EIO;
            }
        }
    }

    if(zorder)
    { 
        if(screen_set_window_property_iv(screen_video[0], SCREEN_PROPERTY_ZORDER, &zorder))
        {
            printf("screen_set_window_property_iv(SCREEN_PROPERTY_ZORDER)failed\n");
            return EIO;
        }
    }

    screen_set_window_property_cv(screen_video[0], SCREEN_PROPERTY_ID_STRING, 15, "rearview-camera");

    for(i = 0; i < num_windows; i++)
    {
        if(screen_create_window_buffers(screen_video[i], nbufs))
        {
            printf("screen_create_window_buffers with screen_video failed.\n");
            return EIO;
        }
    }

    if(screen_get_window_property_iv(screen_video[0], SCREEN_PROPERTY_RENDER_BUFFER_COUNT, &frame_nbuffers))
    {
        printf("screen_get_window_property_iv(SCREEN_PROPERTY_RENDER_BUFFER_COUNT) failed\n");
        return EIO;
    }

    for(i = 0; i < num_windows; i++)
    {
        if(screen_get_window_property_pv(screen_video[i], SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screen_buf[dev][i]))
        {
            printf("screen_get_window_property_pv(SCREEN_PROPERTY_RENDER_BUFFERS) failed\n");
            return EIO;
        }
    }


    for(i = 0; i < num_windows; i++)
    {
        for(j = 0; j < frame_nbuffers; ++j)
        {
            if(screen_get_buffer_property_pv(screen_buf[dev][i][j], SCREEN_PROPERTY_POINTER, (void**)&frame_buffers[dev][i][j]))
            {
                printf("screen_get_window_property_pv(SCREEN_PROPERTY_POINTER) failed\n");
                return EIO;
            }
        }
    }

    if(screen_get_buffer_property_iv(screen_buf[dev][0][0], SCREEN_PROPERTY_STRIDE, &dst_stride))
    {
        printf("screen_get_buffer_property_iv(SCREEN_PROPERTY_STRIDE) failed\n");
        return EIO;
    }

    if((view_width == -1) || (view_height == -1) || 
       (view_width + view_x > dst_width) ||
       (view_height + view_y > dst_height)) 
    {
        view_width = dst_width - view_x;
        view_height = dst_height - view_y;
    }

    view_size[0] = view_width;
    view_size[1] = view_height;

    for(i = 0; i < num_windows; i++)
    {
        if(screen_set_window_property_iv(screen_video[i], SCREEN_PROPERTY_SOURCE_SIZE, view_size))
        {
            printf("screen_set_window_property_iv(SCREEN_PROPERTY_SOURCE_SIZE) failed\n");
            return EIO;
        }
    }

    if((view_x !=0) || (view_y != 0))
    {
        view_pos[0] = view_x;
        view_pos[1] = view_y;
        if(screen_set_window_property_iv(screen_video[0], SCREEN_PROPERTY_SOURCE_POSITION, view_pos))
        {
            printf("screen_set_window_property_iv(SCREEN_PROPERTY_SOURCE_POSITION) failed\n");
            return EIO;
        }
    }

    return EOK;
}

static int capture_context_setup(screen_context_t screen_ctx, screen_window_t *screen_video, capture_context_t *ctx, int dev, int num_devs)
{
    int32_t source_type = -1;
    int32_t thread_priority = -1;
    int32_t frame_times[MAX_BUF_DISPLAY], frame_flags[MAX_BUF_DISPLAY];
    int num_sources, source;

    *ctx = capture_create_context(0);
    if(!(*ctx))
    {
        printf("capture_create_context failed \n");
        return EIO;
    }

    if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_DEVICE, device)) 
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_DEVICE) failed\n");
        return EIO;
    }
    if (cam_source == -1)
    {
        if(capture_get_property_i(*ctx, CAPTURE_PROPERTY_NSOURCES, &num_sources))
        {
            printf("capture_get_property_i(CAPTURE_PROPERTY_NSOURCES) failed\n");
            return EIO;
        }

        if (num_sources <= 0) 
        {
            printf("No support device %d with %d input sources\n", device, num_sources);
            return EIO;
        }
    }
    else
    {
        num_sources = 1;
    }

    num_windows = num_sources;

    if(screen_video_setup(screen_ctx, screen_video, dev, num_devs) != EOK) 
    {
        return EIO;
    }

    if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_IPMMU_SUPPORT, ipmmu)) 
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_IPMMU_SUPPORT) failed\n");
        //return EIO;
    }

    if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_GMSL_MODE, gmsl)) 
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_GMSL_MODE) failed\n");
        //return EIO;
    }

    if (cam_source == -1)
    {
        for (source = 0; source < num_sources; source++)
        {
            if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_SRC_INDEX, source))
            {
                printf("capture_set_property_i(CAPTURE_PROPERTY_SRC_INDEX) failed\n");
                return EIO;
            }
            if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_FRAME_NBUFFERS, frame_nbuffers))
            {
                printf("capture_set_property_i(CAPTURE_PROPERTY_FRAME_NBUFFERS) failed\n");
                return EIO;
            }
            if(capture_set_property_p(*ctx, CAPTURE_PROPERTY_FRAME_BUFFERS, frame_buffers[dev][source]))
            {
                printf("capture_set_property_p(CAPTURE_PROPERTY_FRAME_BUFFERS) failed\n");
                return EIO;
            }
        }

        source = 0;
    }
    else
    {
        source = cam_source;
        if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_SRC_INDEX, source))
        {
            printf("capture_set_property_i(CAPTURE_PROPERTY_SRC_INDEX) failed\n");
            return EIO;
        }
        if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_FRAME_NBUFFERS, frame_nbuffers))
        {
            printf("capture_set_property_i(CAPTURE_PROPERTY_FRAME_NBUFFERS) failed\n");
            return EIO;
        }
        if(capture_set_property_p(*ctx, CAPTURE_PROPERTY_FRAME_BUFFERS, frame_buffers[dev][0]))
        {
            printf("capture_set_property_p(CAPTURE_PROPERTY_FRAME_BUFFERS) failed\n");
            return EIO;
        }
    }

    if(source_type >= 0 && capture_is_property(*ctx, CAPTURE_PROPERTY_ADV_SRC_TYPE) && capture_set_property_i(*ctx, CAPTURE_PROPERTY_ADV_SRC_TYPE, source_type))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_ADV_SRC_TYPE) failed\n");
        return EIO;
    }
    if (capture_set_property_i(*ctx, CAPTURE_PROPERTY_SRC_INDEX, source))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_SRC_INDEX) failed\n");
        return EIO;
    }
    if(color_saturation != 4095 && capture_is_property(*ctx, CAPTURE_PROPERTY_SATURATION) && capture_set_property_i(*ctx, CAPTURE_PROPERTY_SATURATION, color_saturation))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_SATURATION failed\n");
        return EIO;
    }
    if(brightness != 4095 && capture_is_property(*ctx, CAPTURE_PROPERTY_BRIGHTNESS) && capture_set_property_i(*ctx, CAPTURE_PROPERTY_BRIGHTNESS, brightness))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_BRIGHTNESS) failed\n");
        return EIO;
    }
    if(contrast != 4095 && capture_is_property(*ctx, CAPTURE_PROPERTY_CONTRAST) && capture_set_property_i(*ctx, CAPTURE_PROPERTY_CONTRAST, contrast))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_CONTRAST) failed\n");
        return EIO;
    }
    if(color_hue != 4095 && capture_set_property_i(*ctx, CAPTURE_PROPERTY_HUE, color_hue))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_HUE) failed\n");
        return EIO;
    }
    if(capture_is_property(*ctx, CAPTURE_PROPERTY_DST_STRIDE) && capture_set_property_i(*ctx, CAPTURE_PROPERTY_DST_STRIDE, dst_stride))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_DST_STRIDE) failed\n");
        return EIO;
    }
    if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_DST_FORMAT, dst_format))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_DST_FORMAT) failed\n");
        return EIO;
    }
    if(capture_is_property(*ctx, CAPTURE_PROPERTY_SRC_WIDTH) && 
       capture_is_property(*ctx, CAPTURE_PROPERTY_SRC_HEIGHT) && 
                        (src_width != -1) && (src_height != -1))
    {
        if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_SRC_WIDTH, src_width))
        {
            printf("capture_set_property_i(CAPTURE_PROPERTY_SRC_WIDTH) failed\n");
            return EIO;
        }
        if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_SRC_HEIGHT, src_height))
        {
            printf("capture_set_property_i(CAPTURE_PROPERTY_SRC_HEIGHT) failed\n");
            return EIO;
        }
    }
    if(capture_is_property(*ctx, CAPTURE_PROPERTY_CROP_WIDTH) &&
       capture_is_property(*ctx, CAPTURE_PROPERTY_CROP_HEIGHT) &&
       capture_is_property(*ctx, CAPTURE_PROPERTY_CROP_X) &&
       capture_is_property(*ctx, CAPTURE_PROPERTY_CROP_Y))
    {
        if((crop_width != -1) && (crop_height != -1))
        {
            if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_CROP_WIDTH, crop_width))
            {
                printf("capture_set_property_i(CAPTURE_PROPERTY_CROP_WIDTH) failed\n");
                return EIO;
            }
            if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_CROP_HEIGHT, crop_height))
            {
                printf("capture_set_property_i(CAPTURE_PROPERTY_CROP_HEIGHT) failed\n");
                return EIO;
            }
        
        }
        if((crop_x != -1) && (crop_y != -1))
        {
            if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_CROP_X, crop_x))
            {
                printf("capture_set_property_i(CAPTURE_PROPERTY_CROP_X) failed\n");
                return EIO;
            }
            if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_CROP_Y, crop_y))
            {
                printf("capture_set_property_i(CAPTURE_PROPERTY_CROP_Y) failed\n");
                return EIO;
            }
        }
    }
    if(capture_is_property(*ctx, CAPTURE_PROPERTY_DST_WIDTH) && capture_is_property(*ctx, CAPTURE_PROPERTY_DST_HEIGHT))
    {
        if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_DST_WIDTH, dst_width))
        {
            printf("capture_set_property_i(CAPTURE_PROPERTY_DST_WIDTH) failed\n");
            return EIO;
        }
        if(capture_set_property_i(*ctx, CAPTURE_PROPERTY_DST_HEIGHT, dst_height))
        {
            printf("capture_set_property_i(CAPTURE_PROPERTY_DST_HEIGHT) failed\n");
            return EIO;
        }
    }

    if((thread_priority != -1) && capture_set_property_i(*ctx, CAPTURE_PROPERTY_THREAD_PRIORITY, thread_priority))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_THREAD_PRIORITY) failed\n");
        return EIO;
    }
    memset(frame_flags, 0, MAX_BUF_DISPLAY*sizeof(int32_t));
    memset(frame_times, 0, MAX_BUF_DISPLAY*sizeof(int32_t));
    capture_set_property_p(*ctx, CAPTURE_PROPERTY_FRAME_TIMESTAMP, frame_times);
    if(capture_is_property(*ctx, CAPTURE_PROPERTY_FRAME_FLAGS) && capture_set_property_p(*ctx, CAPTURE_PROPERTY_FRAME_FLAGS, frame_flags))
    {
        printf("capture_set_property_i(CAPTURE_PROPERTY_FRAME_FLAGS failed\n");
        return EIO;
    }

    capture_set_property_i(*ctx, CAPTURE_ENABLE, 1);

    if(capture_update(*ctx, 0) == -1)
    {
        printf("capture_update (device=%d) failed\n", device);
        return EIO;
    }

    return EOK;
}

static void * camera_thread(void *data)
{
    int index, i;
    int dev = *((int *)data);
    int v_width = -1, v_height = -1;
    int source;

    capture_get_property_i(capture_ctx[dev], CAPTURE_PROPERTY_SRC_WIDTH, &v_width);
    capture_get_property_i(capture_ctx[dev], CAPTURE_PROPERTY_SRC_HEIGHT, &v_height);

    if ( (v_width != dst_width - view_x) || (v_height != dst_height - view_y))
        printf("Adjusting the source viewport size from %d x %d to %d x %d\n",
            v_width, v_height, dst_width - view_x, dst_height - view_y);

    while(app_abort)
    {
        for (i = 0; i < num_windows; i++)
        {
            source = (cam_source == -1 ? 0 : cam_source) + i;
            if (!capture_set_property_i(capture_ctx[dev], CAPTURE_PROPERTY_SRC_INDEX, source))
            {
                if (((index = capture_get_frame(capture_ctx[dev], 100000000, CAPTURE_FLAG_LATEST_FRAME))) != -1)
                {
                    screen_post_window(screen_video[dev][i], screen_buf[dev][i][index], 1, rect_buf , SCREEN_WAIT_IDLE);
                    capture_release_frame(capture_ctx[dev], index);
                }
            }
        }
    }

    return NULL;
}

static int camera_create_thread(void *data)
{
    pthread_attr_t      pattr;
    struct sched_param  param;
    pthread_t thread_id;
    int ret = 0;

    pthread_attr_init(&pattr);
    pthread_attr_setschedpolicy(&pattr, SCHED_RR);
    param.sched_priority = 21;
    pthread_attr_setschedparam(&pattr, &param);
    pthread_attr_setinheritsched(&pattr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED);

    ret = pthread_create(&thread_id, &pattr, (void *)camera_thread, data);
    if (ret) {
        fprintf(stdout,"could not start thread\n");
        return -1;
    }

    return thread_id;
}

int main(int argc, char **argv)
{
    pthread_attr_t attr;
    sigset_t signals;
    int32_t thread_id[MAX_DEVICES] = { -1, -1 };
    int32_t dev;

    pthread_attr_init (&attr); 
    sigfillset (&signals);
    pthread_sigmask (SIG_BLOCK, &signals, NULL);
    pthread_create (NULL, &attr, signal_thread, NULL);

    if(screen_parse_argument(&argv[1]) != EOK) 
    {
        exit(0);
    }

    if(screen_context_setup(&screen_ctx) != EOK) 
    {
        goto screen_failed;
    }

    if (device == -1) 
    {
        for (dev = 0; dev < MAX_DEVICES; dev++)
        {
            device = dev;
            if (capture_context_setup(screen_ctx, screen_video[dev], &capture_ctx[dev], dev, MAX_DEVICES) != EOK)
            {
                goto screen_failed;
            }
        }
        for (dev= 0; dev < MAX_DEVICES; dev++)
        {
            thread_id[dev] = camera_create_thread(&dev);
            if (thread_id[dev] == -1)
            {
                goto screen_failed;
            }
        }
    }
    else
    {
        if (capture_context_setup(screen_ctx, screen_video[0], &capture_ctx[0], 0, 1) != EOK)
        {
            goto screen_failed;
        }

        dev = 0;
        thread_id[dev] = camera_create_thread(&dev);
        if (thread_id[dev] == -1)
        {
            goto screen_failed;
        }
    }

    while (app_abort) {
        delay(1);
    }

screen_failed:
    if(screen_displays != NULL)
    {
        free(screen_displays);
    }
    for(int j = 0; j < MAX_DEVICES; j++)
    {
        if(capture_ctx[j] != NULL)
        {
            capture_destroy_context(capture_ctx[j]);
        }

        for(int i = 0; i < num_windows; i++)
        {
            if(screen_video[j][i] != NULL)
            {
                screen_destroy_window(screen_video[j][i]);
            }
        }

        if (thread_id[j] != -1)
        {
            pthread_join(thread_id[j], NULL);
        }
    }

    if(screen_ctx != NULL) 
    {   
        screen_destroy_context(screen_ctx); 
    }

    return 1;
}

