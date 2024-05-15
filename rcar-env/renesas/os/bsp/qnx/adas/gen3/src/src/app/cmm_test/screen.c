/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <img/img.h>
#include "cmm_test.h"

int display = 1;
int screen_size[2];
int usage = SCREEN_USAGE_NATIVE;
int format = SCREEN_FORMAT_RGBX8888;

screen_context_t screen_ctx = NULL;
screen_display_t *screen_disps = NULL;
screen_display_t screen_disp = NULL;

static int draw_histogram(screen_window_t window, int *bufsz, int stride, void *pointer, uint32_t *histo)
{
    uint32_t width = bufsz[0];
    uint32_t height = bufsz[1];
    uint32_t stride_ = stride / 4;
    uint32_t *buffer = (uint32_t*)pointer;
    uint32_t *pdata, i, j, k;
    uint32_t bar_w = width / 128;
    uint32_t bar_h = 0;
    uint32_t max_histo;

    for(i = 0, pdata = buffer; i < height; i++, pdata += (stride_ - width)) {
        for(j = 0; j < width; j++, pdata++) {
            *pdata = 0x000000;
        }
    }

    max_histo = histo[0];

    for(i = 1; i < CM2_HGO_NUM; i++) {
        if(max_histo < histo[i]) {
            max_histo = histo[i];
        }
    }

    for(i = 0; i < CM2_HGO_NUM; i++) {
        bar_h = (histo[i] * (height / 2)) / max_histo;
        pdata = buffer + stride_ * (height / 2 - 1) + i * bar_w;
        for(j = 0; j < bar_h; j++) {
            pdata += 1;
            for(k = 1; k < bar_w ; k++) {
                *pdata = 0xFF0000; pdata++;
            }
            pdata -= (stride_ + bar_w);
        }
    }

    histo += CM2_HGO_NUM; max_histo = histo[0];

    for(i = 1; i < CM2_HGO_NUM; i++) {
        if(max_histo < histo[i]) {
            max_histo = histo[i];
        }
    }

    for(i = 0; i < CM2_HGO_NUM; i++) {
        bar_h = (histo[i] * (height / 2)) / max_histo;
        pdata = buffer + width / 2 + stride_ * (height / 2 - 1) + i * bar_w;
        for(j = 0; j < bar_h; j++) {
            pdata += 1;
            for(k = 1; k < bar_w ; k++) {
                *pdata = 0x00FF00; pdata++;
            }
            pdata -= (stride_ + bar_w);
        }
    }

    histo += CM2_HGO_NUM; max_histo = histo[0];

    for(i = 1; i < CM2_HGO_NUM; i++) {
        if(max_histo < histo[i]) {
            max_histo = histo[i];
        }
    }

    for(i = 0; i < CM2_HGO_NUM; i++) {
        bar_h = (histo[i] * (height / 2)) / max_histo;
        pdata = buffer + stride_ * (height - 1) + i * bar_w;
        for(j = 0; j < bar_h; j++) {
            pdata += 1;
            for(k = 1; k < bar_w ; k++) {
                *pdata = 0x0000FF; pdata++;
            }
            pdata -= (stride_ + bar_w);
        }
    }

    return 0;
}

static int decompr_setup(uintptr_t data, img_t *img, unsigned flags)
{
    screen_buffer_t screen_buf[2];
    screen_window_t screen_win = (screen_window_t)data;

    int image_size[2] = { img->w, img->h };
    int image_pos[2] = { 0, 0 };

    if((image_size[0] > screen_size[0]) || (image_size[1] > screen_size[1])) {
        fprintf(stderr, "Image size exceed monitor's native resolution\n");
        return IMG_ERR_INTR;
    }

    if(screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, image_size) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_SIZE) failed (%s)\n", strerror(errno));
        return IMG_ERR_INTR;
    }

    if(screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_POSITION, image_pos) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_POSITION) failed (%s)\n", strerror(errno));
        return IMG_ERR_INTR;
    }

    screen_create_window_buffers(screen_win, 1);

    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screen_buf);
    screen_get_buffer_property_pv(*screen_buf, SCREEN_PROPERTY_POINTER, (void **)&img->access.direct.data);
    screen_get_buffer_property_iv(*screen_buf, SCREEN_PROPERTY_STRIDE, (int *)&img->access.direct.stride);

    img->flags |= IMG_DIRECT;

    return IMG_ERR_OK;
}

static int decompr_image(screen_window_t screen_win, const char *img_path)
{
    int rc;
    img_t img = { 0 };
    img_decode_callouts_t callouts = { 0 };
    img_lib_t ilib = NULL;

    if((rc = img_lib_attach(&ilib)) != IMG_ERR_OK) {
        fprintf(stderr, "img_lib_attach() failed (err %d)\n", rc);
        return -1;
    }

    img.flags |= IMG_FORMAT;
    img.format = IMG_FMT_PKLE_XRGB8888;

    callouts.setup_f = decompr_setup;
    callouts.data = (uintptr_t)screen_win;

    if((rc = img_load_file(ilib, img_path, &callouts, &img)) != IMG_ERR_OK) {
        fprintf(stderr, "img_load_file() failed (err %d)\n", rc);
        return -1;
    }

    img_lib_detach(ilib);

    return (rc == IMG_ERR_OK) ? 0 : -1;
}

static int create_window(screen_window_t *screen_win)
{
    if(screen_create_window(screen_win, screen_ctx) < 0) {
        fprintf(stderr, "screen_create_window() failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_set_window_property_pv(*screen_win, SCREEN_PROPERTY_DISPLAY, (void**)&screen_disp) < 0) {
        fprintf(stderr, "screen_set_window_property_pv(SCREEN_PROPERTY_DISPLAY) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_set_window_property_iv(*screen_win, SCREEN_PROPERTY_USAGE, &usage) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_USAGE) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_set_window_property_iv(*screen_win, SCREEN_PROPERTY_FORMAT, &format) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT) failed (%s)\n", strerror(errno));
        return -1;
    }

    return 0;
}

int screen_init(int display)
{
    int temp;
    int n_display;
 
    if(screen_create_context(&screen_ctx, SCREEN_APPLICATION_CONTEXT) < 0) {
        fprintf(stderr, "screen_create_context() failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_get_context_property_iv(screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &n_display) < 0) {
        fprintf(stderr, "screen_get_context_property_iv(SCREEN_PROPERTY_DISPLAY_COUNT) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(!(screen_disps = calloc(n_display, sizeof(screen_display_t)))) {
        fprintf(stderr, "calloc() failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_get_context_property_pv(screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)screen_disps) < 0) {
        fprintf(stderr, "screen_get_context_property_iv(SCREEN_PROPERTY_DISPLAYS) failed (%s)\n", strerror(errno));
        return -1;
    }

    for(int i = 0; i < n_display; i++) {
        if(screen_get_display_property_iv(screen_disps[i], SCREEN_PROPERTY_ID, &temp) < 0) {
            fprintf(stderr, "screen_get_display_property_iv(SCREEN_PROPERTY_ID) failed (%s)\n", strerror(errno));
            return -1;
        }
        if(temp == display) {
            screen_disp = screen_disps[i];
            break;
        }
    }

    if(screen_disp) {
        if(screen_get_display_property_iv(screen_disp, SCREEN_PROPERTY_SIZE, screen_size) < 0) {
            fprintf(stderr, "screen_get_display_property_iv(SCREEN_PROPERTY_SIZE) failed (%s)\n", strerror(errno));
            return -1;
        }
    } else {
        fprintf(stderr, "Invalid display id\n");
        return -1;
    }

    return 0;
}

int screen_fini(void)
{
    if(screen_ctx) {
        screen_destroy_context(screen_ctx);
    }

    if(screen_disps) {
        free(screen_disps);
    }

    return 0;
}

int screen_load_image(screen_window_t *screen_win, const char *image)
{
    if(create_window(screen_win) < 0) {
        return -1;
    }

    if(decompr_image(*screen_win, image) < 0) {
        return -1;
    }

    return 0;
}

int screen_draw_histo(screen_window_t *screen_win, uint32_t *histo)
{
    void *pointer;
    int bufsz[2], stride;
    screen_buffer_t screen_buf[2];

    if(create_window(screen_win) < 0) {
        return -1;
    }

    if(screen_set_window_property_iv(*screen_win, SCREEN_PROPERTY_SIZE, screen_size) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_SIZE) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_create_window_buffers(*screen_win, 1) < 0) {
        fprintf(stderr, "screen_create_window_buffers() failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_get_window_property_iv(*screen_win, SCREEN_PROPERTY_BUFFER_SIZE, bufsz) < 0) {
        fprintf(stderr, "screen_get_window_property_iv(SCREEN_PROPERTY_BUFFER_SIZE) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_get_window_property_pv(*screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&screen_buf) < 0) {
        fprintf(stderr, "screen_get_window_property_pv(SCREEN_PROPERTY_RENDER_BUFFERS) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_get_buffer_property_pv(*screen_buf, SCREEN_PROPERTY_POINTER, &pointer) < 0) {
        fprintf(stderr, "screen_get_buffer_property_pv(SCREEN_PROPERTY_POINTER) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_get_buffer_property_iv(*screen_buf, SCREEN_PROPERTY_STRIDE, &stride) < 0) {
        fprintf(stderr, "screen_get_buffer_property_iv(SCREEN_PROPERTY_STRIDE) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(draw_histogram(*screen_win, bufsz, stride, pointer, histo) < 0) {
        return -1;
    }

    return 0;
}

int screen_flush(void)
{
    return screen_flush_context(screen_ctx, 0);
}
