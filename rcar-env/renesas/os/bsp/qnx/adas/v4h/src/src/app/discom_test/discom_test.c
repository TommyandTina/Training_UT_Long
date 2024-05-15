/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <devctl.h>
#include <getopt.h>
#include <img/img.h>
#include <screen/screen.h>
#include <dcmd_discom.h>

int fd = -1;
int quit = 0;
int display = 1;
int n_display = 0;
int pipeline = -1;
int uif_rect[4] = { 0 };

int usage = SCREEN_USAGE_NATIVE;
int format = SCREEN_FORMAT_RGBX8888;

int screen_size[2] = { 0 };
int image_size[2] = { 0 };

char dev_name[PATH_MAX] = { 0 };
char image_path[PATH_MAX] = { 0 };

struct crc32_info {
    void *pbuf;
    size_t stride;
    uint32_t expect;
    uint32_t actual;
} crc32;

screen_context_t screen_ctx = NULL;
screen_display_t *screen_disps = NULL;
screen_display_t screen_disp = NULL;
screen_window_t screen_win = NULL;
screen_buffer_t screen_buf[2];

dc_data_packet_t discom = { 0 };

extern void xz_crc32_init(void);
extern uint32_t xz_crc32(const uint8_t *buf, size_t size, uint32_t crc);

void signal_handler(int signum)
{
    switch(signum) {
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGKILL:
        case SIGSEGV:
            quit = 1;
            break;
        default:
            break;
    }
}

uint32_t calculate_crc(void)
{
    void* p_roi;
    uint32_t crc = 0;
    uint32_t img_rect[4] = { 0 };

    memcpy(img_rect, uif_rect, 4 * sizeof(int));

    p_roi = crc32.pbuf + (img_rect[0] * 4);
    p_roi += crc32.stride * img_rect[1];

    for(uint32_t row = 0; row < img_rect[3]; row++) {
        crc = xz_crc32(p_roi, img_rect[2] * 4, crc);
        p_roi += crc32.stride;
    }

    return crc;
}

int display_image(void)
{
    int dirty[4] = { 0 };

    if(screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screen_buf) < 0) {
        fprintf(stderr, "screen_get_window_property_pv(SCREEN_PROPERTY_RENDER_BUFFERS) failed (%s)\n", strerror(errno));
        return -1;
    }

    dirty[2] = image_size[0];
    dirty[3] = image_size[1];

    if(screen_post_window(screen_win, *screen_buf, 1, dirty, 0) < 0) {
        fprintf(stderr, "screen_post_window() failed (%s)\n", strerror(errno));
        return -1;
    }

    return 0;
}

int decompr_setup(uintptr_t data, img_t *img, unsigned flags)
{
    screen_window_t screen_win = (screen_window_t)data;

    image_size[0] = img->w;
    image_size[1] = img->h;

    if((image_size[0] > screen_size[0]) || (image_size[1] > screen_size[1])) {
        fprintf(stderr, "Image size exceed monitor's native resolution\n");
        return IMG_ERR_INTR;
    }

    if(screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, image_size) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_SIZE) failed (%s)\n", strerror(errno));
        return IMG_ERR_INTR;
    }

    screen_create_window_buffers(screen_win, 1);

    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screen_buf);
    screen_get_buffer_property_pv(*screen_buf, SCREEN_PROPERTY_POINTER, (void **)&img->access.direct.data);
    screen_get_buffer_property_iv(*screen_buf, SCREEN_PROPERTY_STRIDE, (int *)&img->access.direct.stride);

    crc32.pbuf = img->access.direct.data;
    crc32.stride = img->access.direct.stride;

    img->flags |= IMG_DIRECT;

    return IMG_ERR_OK;
}

int decompr_image(void)
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

    if((rc = img_load_file(ilib, image_path, &callouts, &img)) != IMG_ERR_OK) {
        fprintf(stderr, "img_load_file() failed (err %d)\n", rc);
        return -1;
    }

    img_lib_detach(ilib);

    return (rc == IMG_ERR_OK) ? 0 : -1;
}

int create_window(void)
{
    if(screen_create_window(&screen_win, screen_ctx) < 0) {
        fprintf(stderr, "screen_create_window() failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_set_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY, (void**)&screen_disp) < 0) {
        fprintf(stderr, "screen_set_window_property_pv(SCREEN_PROPERTY_DISPLAY) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_USAGE) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &format) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT) failed (%s)\n", strerror(errno));
        return -1;
    }

    if(screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_PIPELINE, &pipeline) < 0) {
		fprintf(stderr, "screen_get_window_property_iv(SCREEN_PROPERTY_PIPELINE) failed (%s)\n", strerror(errno));
		return -1;
	}

    return 0;
}

int verify_option(void)
{
    if(strlen(image_path) == 0) {
        fprintf(stderr, "Please specify image path\n");
        return -1;
    }

    if(access(image_path, F_OK) < 0) {
        fprintf(stderr, "Image file does not exist\n");
        return -1;
    }

    return 0;
}

int check_display(void)
{
    int temp;

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

    return screen_disp ? 0 : -1;
}

int main(int argc, char *argv[])
{
    int rc, opt, index;

    static struct option options[] = {
        { "display",   required_argument, 0,  'd' },
        { "image",     required_argument, 0,  'i' },
        { "rect",      required_argument, 0,  'r' },
        { NULL,        0,                 0,   0  }
    };

    while((opt = getopt_long(argc, argv,"d:i:r:", options, &index )) != -1) {
        switch (opt) {
             case 'd':
                display = strtol(optarg, NULL, 10);
                break;
             case 'i':
                strcpy(image_path, optarg);
                break;
             case 'r':
                sscanf(optarg, "%d,%d,%d,%d", &uif_rect[0], &uif_rect[1], &uif_rect[2], &uif_rect[3]);
                break;
             default:
                fprintf(stdout, "Unknown option\n");
                return 0;
        }
    }

    signal(SIGINT,  signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGKILL, signal_handler);
    signal(SIGSEGV, signal_handler);

    if(screen_create_context(&screen_ctx, SCREEN_APPLICATION_CONTEXT) < 0) {
        fprintf(stderr, "screen_create_context() failed (%s)\n", strerror(errno));
        return -1;
    }

    xz_crc32_init();

    if((check_display() < 0) ||
       (verify_option() < 0) ||
       (create_window() < 0) ||
       (decompr_image() < 0) ||
       (display_image() < 0)) {
        goto cleanup;
    }

    snprintf(dev_name, PATH_MAX - 1, DEV_NAME"%d", display);

    if((fd = open(dev_name, O_RDWR)) < 0) {
        fprintf(stderr, "open(%s) failed (%s)\n", dev_name, strerror(errno));
        goto cleanup;
    }

    if(!uif_rect[2] && !uif_rect[3]) {
        uif_rect[2] = image_size[0];
        uif_rect[3] = image_size[1];
    }

    discom.pipe = pipeline;
    discom.src.hor_off = uif_rect[0];
    discom.src.ver_off = uif_rect[1];
    discom.src.hor_size = uif_rect[2];
    discom.src.ver_size = uif_rect[3];

    if((rc = devctl(fd, DCMD_DISCOM_ATTACH, &discom, sizeof(dc_data_packet_t), NULL)) != EOK) {
        fprintf(stderr, "DCMD_DISCOM_ATTACH() failed (%s)\n", strerror(rc));
        goto cleanup;
    }

    screen_wait_vsync(screen_disp);

    if((rc = devctl(fd, DCMD_DISCOM_SOURCE_RECTANGLE, &discom, sizeof(dc_data_packet_t), NULL)) != EOK) {
        fprintf(stderr, "DCMD_DISCOM_SOURCE_RECTANGLE() failed (%s)\n", strerror(rc));
        goto detach;
    }

    if((rc = devctl(fd, DCMD_DISCOM_START, &discom, sizeof(dc_data_packet_t), NULL)) != EOK) {
        fprintf(stderr, "DCMD_DISCOM_START() failed (%s)\n", strerror(rc));
        goto detach;
    }

    screen_wait_vsync(screen_disp);

    if((rc = devctl(fd, DCMD_DISCOM_STOP, &discom, sizeof(dc_data_packet_t), NULL)) != EOK) {
        fprintf(stderr, "DCMD_DISCOM_STOP() failed (%s)\n", strerror(rc));
        goto detach;
    }

    screen_wait_vsync(screen_disp);

    if((rc = devctl(fd, DCMD_DISCOM_GET_CURRENT_CRC, &discom, sizeof(dc_data_packet_t), NULL)) != EOK) {
        fprintf(stderr, "DCMD_DISCOM_GET_CURRENT_CRC() failed (%s)\n", strerror(rc));
        goto detach;
    }

    crc32.actual = discom.cur_crc;
    crc32.expect = calculate_crc();

    fprintf(stdout, "Expect: 0x%08x\n", crc32.expect);
    fprintf(stdout, "Actual: 0x%08x\n", crc32.actual);

    if(crc32.actual != crc32.expect) {
        fprintf(stdout, "Result:\033[1;31m Failed \033[0;0m\n");
    } else {
        fprintf(stdout, "Result:\033[1;32m Passed \033[0;0m\n");
    }

    fprintf(stdout, "Press Ctrl-C to terminate\n");

    while(!quit) {
        delay(100);
    }

detach:
    if((rc = devctl(fd, DCMD_DISCOM_DETACH, &discom, sizeof(dc_data_packet_t), NULL)) != EOK) {
        fprintf(stderr, "DCMD_DISCOM_DETACH() failed (%s)\n", strerror(rc));
    }

cleanup:
    if(screen_ctx) {
        screen_destroy_context(screen_ctx);
    }
    if(screen_win) {
        screen_destroy_window(screen_win);
    }
    if(screen_disps) {
        free(screen_disps);
    }
    if(fd > 0) {
        close(fd);
    }
    return 0;
}
