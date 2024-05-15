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

#ifndef __CMM_TEST_H__
#define __CMM_TEST_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <screen/screen.h>
#include <hw/rcar_cmm.h>

#define CM2_LUT_NUM                    256
#define CM2_CLU_NUM                    17 * 17 * 17
#define CM2_HGO_NUM                    64

#define CM2_TEST_SET_LUT               (1)
#define CM2_TEST_SET_CLU               (2)
#define CM2_TEST_CONFIG_HGO            (3)
#define CM2_TEST_READ_HGO              (4)

#define CM2_COLOR_TONE_BASIC           '1'
#define CM2_COLOR_TONE_WARM            '2'
#define CM2_COLOR_TONE_COLD            '3'
#define CM2_COLOR_TONE_DRAMA           '4'
#define CM2_COLOR_TONE_VIBRANT         '5'
#define CM2_COLOR_TONE_TEAL_ORANGE1    '6'
#define CM2_COLOR_TONE_TEAL_ORAMGE2    '7'

typedef struct cmm_posix_mem {
    int fd;
    size_t size;
    uint32_t paddr;
    void *vaddr;
} cmm_posix_mem_t;

typedef struct cmm_test_opt {
    int display;
    int command;
    int hgo_ctrl;
    int hgo_mode;
    int hgo_rect[4];
    char image[PATH_MAX];
} cmm_test_opt_t;

int screen_init(int display);
int screen_fini(void);
int screen_load_image(screen_window_t *window, const char *image);
int screen_draw_histo(screen_window_t *window, uint32_t *histo);
int screen_flush(void);

int devctl_start_hgo(int fd);
int devctl_start_stop(int fd, uint8_t start);
int devctl_config(int fd, uint8_t start);
int devctl_wait_cmd(int fd);
int devctl_set_lut(int fd, cmm_data_addr_t *table);
int devctl_set_clu(int fd, cmm_data_addr_t *table);
int devctl_set_hgoinfo(int fd, cmm_hgo_info_t *cfg);
int devctl_get_hgo(int fd, cmm_data_addr_t *table);

#endif /* __CMM_TEST_H__ */
