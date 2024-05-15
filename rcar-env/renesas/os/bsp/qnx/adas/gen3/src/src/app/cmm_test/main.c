/*
 * $QNXLicenseC:
 * Copyright 2021, QNX Software Systems.
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
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <devctl.h>
#include <getopt.h>
#include <pthread.h>
#include <sys/mman.h>

#include "cmm_test.h"

#include "lut_basic.h"
#include "lut_cold.h"
#include "lut_drama.h"
#include "lut_teal_orange1.h"
#include "lut_teal_orange2.h"
#include "lut_vibrant.h"
#include "lut_warm.h"

#include "clu_basic.h"
#include "clu_cold.h"
#include "clu_drama.h"
#include "clu_teal_orange1.h"
#include "clu_teal_orange2.h"
#include "clu_vibrant.h"
#include "clu_warm.h"

static char input;
static bool force_quit = false;

static cmm_test_opt_t option = {
    .display = 1,
    .command = CM2_TEST_SET_LUT,
    .hgo_ctrl = 0,
    .hgo_mode = 0,
    .hgo_rect = { 0, 0, 0, 0 }
};

void *show_window(void *arg)
{
    screen_buffer_t screen_buf[2];
    screen_window_t screen_win = *(screen_window_t*)arg;

    if(screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screen_buf) < 0) {
        fprintf(stderr, "screen_get_window_property_pv(SCREEN_PROPERTY_RENDER_BUFFERS) failed (%s)\n", strerror(errno));
        return NULL;
    }

    int rect[4] = { 0 };

    if(screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, &rect[2]) < 0) {
        fprintf(stderr, "screen_set_window_property_iv(SCREEN_PROPERTY_SIZE) failed (%s)\n", strerror(errno));
        return NULL;
    }

    if(screen_post_window(screen_win, *screen_buf, 1, rect, 0) < 0) {
        fprintf(stderr, "screen_post_window() failed (%s)\n", strerror(errno));
        return NULL;
    }

    while(force_quit) {
        delay(100);
    }

    return NULL;
}

int create_table(cmm_posix_mem_t *mem, size_t size)
{
    size = (size + __PAGESIZE - 1) & ~((size_t)__PAGESIZE - 1);

    if((mem->fd = posix_typed_mem_open("/memory/below4G", O_RDWR, POSIX_TYPED_MEM_ALLOCATE_CONTIG)) < 0) {
        fprintf(stderr, "posix_typed_mem_open() failed (%s)", strerror(errno));
        return -1;
    }

    if((mem->vaddr = mmap64(0, size, PROT_READ | PROT_WRITE | PROT_NOCACHE, MAP_SHARED, mem->fd, 0)) == MAP_FAILED) {
        fprintf(stderr, "mmap64() failed (%s)", strerror(errno));
        return -1;
    }

    if(posix_mem_offset64(mem->vaddr, size, (off64_t*)&mem->paddr, 0, 0) < 0) {
        fprintf(stderr, "posix_mem_offset64() failed (%s)", strerror(errno));
        return -1;
    }

    mem->size = size;

    return 0;
}

void destroy_table(cmm_posix_mem_t *mem)
{
    if(mem->vaddr) {
        munmap(mem->vaddr, mem->size);
    }

    if(mem->fd) {
        close(mem->fd);
    }
}

void load_tone(cmm_posix_mem_t *mem, const double filter[][3], size_t size)
{
    uint8_t r, g, b;
    uint32_t *ptr;

    ptr = (uint32_t*)mem->vaddr;

    for(int i = 0; i < size; i++)
    {
        r = (uint8_t)(filter[i][0] * 255);
        g = (uint8_t)(filter[i][1] * 255);
        b = (uint8_t)(filter[i][2] * 255);
 
        ptr[i] = (r << 16) | (g << 8) | (b << 0);
    }
}

int load_clu_tone(char tone, cmm_posix_mem_t *mem)
{
    switch(tone) {
        case CM2_COLOR_TONE_BASIC:
            load_tone(mem, color_clu_basic, CM2_CLU_NUM);
            break;
        case CM2_COLOR_TONE_WARM:
            load_tone(mem, color_clu_warm, CM2_CLU_NUM);
            break;
        case CM2_COLOR_TONE_COLD:
            load_tone(mem, color_clu_cold, CM2_CLU_NUM);
            break;
        case CM2_COLOR_TONE_VIBRANT:
            load_tone(mem, color_clu_vibrant, CM2_CLU_NUM);
            break;
        case CM2_COLOR_TONE_DRAMA:
            load_tone(mem, color_clu_drama, CM2_CLU_NUM);
            break;
        case CM2_COLOR_TONE_TEAL_ORANGE1:
            load_tone(mem, color_clu_teal_orange1, CM2_CLU_NUM);
            break;
        case CM2_COLOR_TONE_TEAL_ORAMGE2:
            load_tone(mem, color_clu_teal_orange2, CM2_CLU_NUM);
            break;
        default:
            break;
    }

    return 0;
}

int load_lut_tone(char tone, cmm_posix_mem_t *mem)
{
    switch(tone) {
        case CM2_COLOR_TONE_BASIC:
            load_tone(mem, color_lut_basic, CM2_LUT_NUM);
            break;
        case CM2_COLOR_TONE_WARM:
            load_tone(mem, color_lut_warm, CM2_LUT_NUM);
            break;
        case CM2_COLOR_TONE_COLD:
            load_tone(mem, color_lut_cold, CM2_LUT_NUM);
            break;
        case CM2_COLOR_TONE_VIBRANT:
            load_tone(mem, color_lut_vibrant, CM2_LUT_NUM);
            break;
        case CM2_COLOR_TONE_DRAMA:
            load_tone(mem, color_lut_drama, CM2_LUT_NUM);
            break;
        case CM2_COLOR_TONE_TEAL_ORANGE1:
            load_tone(mem, color_lut_teal_orange1, CM2_LUT_NUM);
            break;
        case CM2_COLOR_TONE_TEAL_ORAMGE2:
            load_tone(mem, color_lut_teal_orange2, CM2_LUT_NUM);
            break;
        default:
            break;
    }

    return 0;
}

void test_read_hgo(int fd)
{
    int image_vis = 0;
    int histo_vis = 0;

    pthread_t image_tid;
    pthread_t histo_tid;

    cmm_posix_mem_t tbl_mem = { 0 };
    cmm_data_addr_t tbl_hgo = { 0 };
    cmm_hgo_info_t  hgo_info = { 0 };

    screen_window_t image_win = NULL;
    screen_window_t histo_win = NULL;

    if(screen_init(option.display) < 0) {
        goto done;
    }

    if(screen_load_image(&image_win, option.image) < 0) {
        goto done;
    }

    fprintf(stdout, "Is the image shown on screen? (y/n): ");

    if(pthread_create(&image_tid, NULL, show_window, &image_win) < 0) {
        fprintf(stderr, "pthread_create() failed (%s)\n", strerror(errno));
        goto done;
    }

    input = fgetc(stdin); while(fgetc(stdin) != '\n');

    if(input != 'y') {
        fprintf(stderr, "Terminated the test\n");
        goto done;
    }

    if(create_table(&tbl_mem, CM2_HGO_NUM * 4 * 3) < 0) {
        goto done;
    }

    tbl_hgo.length = tbl_mem.size;
    tbl_hgo.data = tbl_mem.paddr;

    hgo_info.maxrgb = (option.hgo_mode >> 7) & 0x1;
    hgo_info.ofsb_b = (option.hgo_mode >> 4) & 0x1;
    hgo_info.ofsb_g = (option.hgo_mode >> 5) & 0x1;
    hgo_info.ofsb_r = (option.hgo_mode >> 6) & 0x1;
    hgo_info.hratio = (option.hgo_mode >> 2) & 0x3;
    hgo_info.vratio = (option.hgo_mode >> 0) & 0x3;
    hgo_info.hoffset = option.hgo_rect[0];
    hgo_info.voffset = option.hgo_rect[1];
    hgo_info.hsize = option.hgo_rect[2];
    hgo_info.vsize = option.hgo_rect[3];
    hgo_info.hists = option.hgo_ctrl;

    devctl_start_stop(fd, 0);
    devctl_set_hgoinfo(fd, &hgo_info);
    devctl_start_stop(fd, 1);

    devctl_start_hgo(fd);
    devctl_get_hgo(fd, &tbl_hgo);
    devctl_wait_cmd(fd);
    devctl_get_hgo(fd, &tbl_hgo);
    devctl_wait_cmd(fd);

    if(screen_draw_histo(&histo_win, (uint32_t*)tbl_mem.vaddr) < 0) {
        goto done;
    }

    if(pthread_create(&histo_tid, NULL, show_window, &histo_win) < 0) {
        fprintf(stderr, "pthread_create() failed (%s)\n", strerror(errno));
        goto done;
    }

    fprintf(stdout, "Enter a number to show image/histogram or 'q' to quit the test\n");
    fprintf(stdout, "  1. Show image\n");
    fprintf(stdout, "  2. Show color histogram\n");

    while(!force_quit) {
        fprintf(stdout, "Enter your input: ");
        switch(input = fgetc(stdin)) {
            case '1':
                image_vis = 1;
                histo_vis = 0;
                screen_set_window_property_iv(image_win, SCREEN_PROPERTY_VISIBLE, &image_vis);
                screen_set_window_property_iv(histo_win, SCREEN_PROPERTY_VISIBLE, &histo_vis);
                screen_flush();
                break;
            case '2':
                image_vis = 0;
                histo_vis = 1;
                screen_set_window_property_iv(image_win, SCREEN_PROPERTY_VISIBLE, &image_vis);
                screen_set_window_property_iv(histo_win, SCREEN_PROPERTY_VISIBLE, &histo_vis);
                screen_flush();
                break;
            case 'q':
                force_quit = true;
                break;
            default:
                break;
        }
        while(input != '\n') {
            input = fgetc(stdin);
        }
    }

    pthread_join(image_tid, NULL);
    pthread_join(histo_tid, NULL);

done:
    if(image_win) {
        screen_destroy_window(image_win);
    }
    if(histo_win) {
        screen_destroy_window(histo_win);
    }
    destroy_table(&tbl_mem);
    screen_fini();
}

void test_set_clu(int fd)
{
    pthread_t tid;
    
    cmm_posix_mem_t tbl_mem = { 0 };
    cmm_data_addr_t tbl_clu = { 0 };

    screen_window_t image_win = NULL;

    if(screen_init(option.display) < 0) {
        goto done;
    }

    if(screen_load_image(&image_win, option.image) < 0) {
        goto done;
    }

    if(create_table(&tbl_mem, CM2_CLU_NUM * 4) < 0) {
        goto done;
    }

    tbl_clu.length = tbl_mem.size;
    tbl_clu.data = tbl_mem.paddr;

    if(pthread_create(&tid, NULL, show_window, &image_win) < 0) {
        fprintf(stderr, "pthread_create() failed (%s)\n", strerror(errno));
        goto done;
    }

    fprintf(stdout, "Enter a number (1-7) to select color tone, or 'q' to quit the test\n");
    fprintf(stdout, "  1. Basic\n");
    fprintf(stdout, "  2. Warm\n");
    fprintf(stdout, "  3. Cold\n");
    fprintf(stdout, "  4. Drama\n");
    fprintf(stdout, "  5. Vibrant\n");
    fprintf(stdout, "  6. TealOrange1\n");
    fprintf(stdout, "  7. TealOrange2\n");

    while(!force_quit) {
        fprintf(stdout, "Enter your input: ");
        switch(input = fgetc(stdin)) {
            case '1' ... '7':
                load_clu_tone(input, &tbl_mem);
                devctl_set_clu(fd, &tbl_clu);
                devctl_wait_cmd(fd);
                devctl_set_clu(fd, &tbl_clu);
                devctl_wait_cmd(fd);
                break;
            case 'q':
                force_quit = true;
                break;
            default:
                break;
        }
        while(input != '\n') {
            input = fgetc(stdin);
        }
    }

    pthread_join(tid, NULL);

done:
    if(image_win) {
        screen_destroy_window(image_win);
    }
    destroy_table(&tbl_mem);
    screen_fini();
}

void test_set_lut(int fd)
{
    pthread_t tid;
    
    cmm_posix_mem_t tbl_mem = { 0 };
    cmm_data_addr_t tbl_lut = { 0 };

    screen_window_t image_win = NULL;

    if(screen_init(option.display) < 0) {
        goto done;
    }

    if(screen_load_image(&image_win, option.image) < 0) {
        goto done;
    }

    if(create_table(&tbl_mem, CM2_LUT_NUM * 4) < 0) {
        goto done;
    }

    tbl_lut.length = tbl_mem.size;
    tbl_lut.data = tbl_mem.paddr;

    if(pthread_create(&tid, NULL, show_window, &image_win) < 0) {
        fprintf(stderr, "pthread_create() failed (%s)\n", strerror(errno));
        goto done;
    }

    fprintf(stdout, "Enter a number (1-7) to select color tone, or 'q' to quit the test\n");
    fprintf(stdout, "  1. Basic\n");
    fprintf(stdout, "  2. Warm\n");
    fprintf(stdout, "  3. Cold\n");
    fprintf(stdout, "  4. Drama\n");
    fprintf(stdout, "  5. Vibrant\n");
    fprintf(stdout, "  6. TealOrange1\n");
    fprintf(stdout, "  7. TealOrange2\n");

    while(!force_quit) {
        fprintf(stdout, "Enter your input: ");
        switch(input = fgetc(stdin)) {
            case '1' ... '7':
                load_lut_tone(input, &tbl_mem);
                devctl_set_lut(fd, &tbl_lut);
                devctl_wait_cmd(fd);
                devctl_set_lut(fd, &tbl_lut);
                devctl_wait_cmd(fd);
                break;
            case 'q':
                force_quit = true;
                break;
            default:
                break;
        }
        while(input != '\n') {
            input = fgetc(stdin);
        }
    }

    pthread_join(tid, NULL);

done:
    if(image_win) {
        screen_destroy_window(image_win);
    }
    destroy_table(&tbl_mem);
    screen_fini();
}

void parse_test_cmd(const char *str)
{
    if(!strcmp(str, "set-lut")) {
        option.command = CM2_TEST_SET_LUT;
    } else if(!strcmp(str, "set-clu")) {
        option.command = CM2_TEST_SET_CLU;
    } else if(!strcmp(str, "read-hgo")) {
        option.command = CM2_TEST_READ_HGO;
    } else {
        fprintf(stderr, "Unknown command option\n");
        abort();
    }
}

void parse_hgo_ctrl(const char *str)
{
    if(!strcmp(str, "before-lut")) {
        option.hgo_ctrl = 1;
    } else if(!strcmp(str, "before-clu")) {
        option.hgo_ctrl = 0;
    } else {
        fprintf(stderr, "Unknown HGO ctrl option\n");
        abort();
    }
}

void parse_option(int argc, char *argv[])
{
    int opt, index;

    static struct option options[] = {
        { "display",       required_argument, 0,  'd' },
        { "command",       required_argument, 0,  'c' },
        { "image",         required_argument, 0,  'i' },
        { "hgo-ctrl",      required_argument, 0,  'p' },
        { "hgo-mode",      required_argument, 0,  'm' },
        { "hgo-rect",      required_argument, 0,  'r' },
        { NULL,            0,                 0,   0  }
    };

    while((opt = getopt_long(argc, argv,"d:c:i:p:m:r:", options, &index )) != -1) {
        switch (opt) {
            case 'd':
                option.display = atoi(optarg);
                break;
            case 'c':
                parse_test_cmd(optarg);
                break;
            case 'i':
                strcpy(option.image, optarg);
                break;
            case 'p':
                parse_hgo_ctrl(optarg);
                break;
            case 'm':
                option.hgo_mode = (strtol(optarg, NULL, 16) & 0xFF);
                break;
            case 'r':
                sscanf(optarg, "%d,%d,%d,%d", &option.hgo_rect[0], &option.hgo_rect[1],
                                              &option.hgo_rect[2], &option.hgo_rect[3]);
                break;
            default:
                fprintf(stderr, "Unknown option\n");
                abort();
        }
    }
}

int main(int argc, char *argv[])
{
    int fd = -1;

    parse_option(argc, argv);

    if((fd = open(CMM_DEV_NAME, O_RDWR)) < 0) {
        fprintf(stderr, "open() failed (%s)\n", strerror(errno));
        abort();
    }

    devctl_start_stop(fd, 1);

    switch(option.command) {
        case CM2_TEST_SET_LUT:
            test_set_lut(fd);
            break;
        case CM2_TEST_SET_CLU:
            test_set_clu(fd);
            break;
        case CM2_TEST_READ_HGO:
            test_read_hgo(fd);
            break;
        default:
            break;
    }

    devctl_start_stop(fd, 0);

    if(fd > 0) {
        close(fd);
    }

    return 0;
}
