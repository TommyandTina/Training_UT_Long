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

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/syspage.h>
#include <hw/rcar_cpg.h>
#include <hw/rcar_sysc.h>

#define CPG_CLK_MSTP    (CPG_CLK_UNKNOWN + 1)
static sysc_mgr_funcs_t syschwfuncs;
static int rcar_gen3 = 1;


#define SYSC_PDR_CA57_SCU    (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_CA57_SCU)
#define SYSC_PDR_CA53_SCU    (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_CA53_SCU)
#define SYSC_PDR_CR7         (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_CR7)
#define SYSC_PDR_A3IR        (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3IR: SYSC_PDR_GEN3_A3IR)
#define SYSC_PDR_A2IR0       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2IR0)
#define SYSC_PDR_A2IR1       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2IR1)
#define SYSC_PDR_A2IR2       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2IR2)
#define SYSC_PDR_A2IR3       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2IR3)
#define SYSC_PDR_A2IR4       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2IR4)
#define SYSC_PDR_A2IR5       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2IR5)
#define SYSC_PDR_A2SC0       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2SC0)
#define SYSC_PDR_A2SC1       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2SC1)
#define SYSC_PDR_A2SC2       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2SC2)
#define SYSC_PDR_A2SC3       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2SC3)
#define SYSC_PDR_A2SC4       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2SC4)
#define SYSC_PDR_A2DP0       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2DP0 : SYSC_PDR_GEN3_A2DP0)
#define SYSC_PDR_A2DP1       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2DP1 : SYSC_PDR_GEN3_A2DP1)
#define SYSC_PDR_A2DP        (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2DP)
#define SYSC_PDR_A2CN        (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2CN0)
#define SYSC_PDR_A3VIP0      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3VIP0 : SYSC_PDR_GEN3_A3VIP0)
#define SYSC_PDR_A3VIP1      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3VIP1 : SYSC_PDR_GEN3_A3VIP1)
#define SYSC_PDR_A3VIP2      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3VIP2 : SYSC_PDR_GEN3_A3VIP2)
#define SYSC_PDR_A3VP        (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A3VP)
#define SYSC_PDR_A3VC        (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A3VC)
#define SYSC_PDR_A2VC0       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2VC0)
#define SYSC_PDR_A2VC1       (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_A2VC1)
#define SYSC_PDR_3DGA        (rcar_gen3 == 0 ?  SYSC_PDR_V3U_3DGA : SYSC_PDR_GEN3_3DGA)
#define SYSC_PDR_3DGB        (rcar_gen3 == 0 ?  SYSC_PDR_V3U_3DGB : SYSC_PDR_GEN3_3DGB)
#define SYSC_PDR_3DGC        (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_3DGC)
#define SYSC_PDR_3DGD        (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_3DGD)
#define SYSC_PDR_3DGE        (rcar_gen3 == 0 ?  -1 : SYSC_PDR_GEN3_3DGE)
#define SYSC_PDR_A1E0D0C0    (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1E0D0C0 : -1)
#define SYSC_PDR_A1E0D0C1    (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1E0D0C1 : -1)
#define SYSC_PDR_A1E0D1C0    (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1E0D1C0 : -1)
#define SYSC_PDR_A1E0D1C1    (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1E0D1C1 : -1)
#define SYSC_PDR_A1E1D0C0    (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1E1D0C0 : -1)
#define SYSC_PDR_A1E1D0C1    (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1E1D0C1 : -1)
#define SYSC_PDR_A1E1D1C0    (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1E1D1C0 : -1)
#define SYSC_PDR_A1E1D1C1    (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1E1D1C1 : -1)
#define SYSC_PDR_A2E0D0      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2E0D0 : -1)
#define SYSC_PDR_A2E0D1      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2E0D1 : -1)
#define SYSC_PDR_A2E1D0      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2E1D0 : -1)
#define SYSC_PDR_A2E1D1      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2E1D1  : -1)
#define SYSC_PDR_A3E0        (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3E0  : -1)
#define SYSC_PDR_A3E1        (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3E1  : -1)
#define SYSC_PDR_A1CNN2      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1CNN2  : -1)
#define SYSC_PDR_A1DSP0      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1DSP0  : -1)
#define SYSC_PDR_A2IMP0      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2IMP0  : -1)
#define SYSC_PDR_A2CV0       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CV0  : -1)
#define SYSC_PDR_A2CV1       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CV1  : -1)
#define SYSC_PDR_A2CV4       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CV4  : -1)
#define SYSC_PDR_A2CV6       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CV6  : -1)
#define SYSC_PDR_A2CN2       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CN2  : -1)
#define SYSC_PDR_A1CNN0      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1CNN0  : -1)
#define SYSC_PDR_A2CN0       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CN0  : -1)
#define SYSC_PDR_A1CNN1      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1CNN1  : -1)
#define SYSC_PDR_A1DSP1      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A1DSP1  : -1)
#define SYSC_PDR_A2IMP2      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2IMP2  : -1)
#define SYSC_PDR_A2CV2       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CV2  : -1)
#define SYSC_PDR_A2CV3       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CV3  : -1)
#define SYSC_PDR_A2CV5       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CV5  : -1)
#define SYSC_PDR_A2CV7       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CV7  : -1)
#define SYSC_PDR_A2CN1       (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A2CN1  : -1)
#define SYSC_PDR_A3VIP3      (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3VIP3  : -1)
#define SYSC_PDR_A3ISP01     (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3ISP01  : -1)
#define SYSC_PDR_A3ISP23     (rcar_gen3 == 0 ?  SYSC_PDR_V3U_A3ISP23  : -1)



void usage()
{
    printf("Syntax:                                                       \n");
    printf("    sysc_ctrl dom=<domain_name> -e -d                         \n");
    printf("                                                              \n");
    printf("    <domain_name>: Supported power domain name                \n");
    printf("    (Check power domain that your R-Car platform is supported)\n");
    if (rcar_gen3) {
    printf("        ca57_scu  for    CA57_SCU                             \n");
    printf("        ca53_scu  for    CA53_SCU                             \n");
    printf("        cr7       for    CR7                                  \n");
    printf("        a3ir      for    A3IR                                 \n");
    printf("        a2ir0     for    A2IR0                                \n");
    printf("        a2ir1     for    A2IR1                                \n");
    printf("        a2ir2     for    A2IR2                                \n");
    printf("        a2ir3     for    A2IR3                                \n");
    printf("        a2ir4     for    A2IR4                                \n");
    printf("        a2ir5     for    A2IR5                                \n");
    printf("        a2sc0     for    A2SC0                                \n");
    printf("        a2sc1     for    A2SC1                                \n");
    printf("        a2sc2     for    A2SC2                                \n");
    printf("        a2sc3     for    A2SC3                                \n");
    printf("        a2sc4     for    A2SC4                                \n");
    printf("        a2dp0     for    A2DP0                                \n");
    printf("        a2dp1     for    A2DP1                                \n");
    printf("        a2dp      for    A2DP                                 \n");
    printf("        a2cn      for    A2CN                                 \n");
    printf("        a3vip0    for    A3VIP0                               \n");
    printf("        a3vip1    for    A3VIP1                               \n");
    printf("        a3vip2    for    A3VIP2                               \n");
    printf("        a3vp      for    A3VP                                 \n");
    printf("        a3vc      for    A3VC                                 \n");
    printf("        a2vc0     for    A2VC0                                \n");
    printf("        a2vc1     for    A2VC1                                \n");
    printf("        3dga      for    3DGA                                 \n");
    printf("        3dgb      for    3DGB                                 \n");
    printf("        3dgc      for    3DGC                                 \n");
    printf("        3dgd      for    3DGD                                 \n");
    printf("        3dge      for    3DGE                                 \n");
    } else {
    printf("        a1e0d0c0  for    A1E0D0C0                             \n");
    printf("        a1e0d0c1  for    A1E0D0C1                             \n");
    printf("        a1e0d1c0  for    A1E0D1C0                             \n");
    printf("        a1e0d1c1  for    A1E0D1C1                             \n");
    printf("        a1e1d0c0  for    A1E1D0C0                             \n");
    printf("        a1e1d0c1  for    A1E1D0C1                             \n");
    printf("        a1e1d1c0  for    A1E1D1C0                             \n");
    printf("        a1e1d1c1  for    A1E1D1C1                             \n");
    printf("        a2e0d0    for    A2E0D0                               \n");
    printf("        a2e0d1    for    A2E0D1                               \n");
    printf("        a2e1d0    for    A2E1D0                               \n");
    printf("        a2e1d1    for    A2E1D1                               \n");
    printf("        a3e0      for    A3E0                                 \n");
    printf("        a3e1      for    A3E1                                 \n");
    printf("        a1cnn2    for    A1CNN2                               \n");
    printf("        a1dsp0    for    A1DSP0                               \n");
    printf("        a2imp0    for    A2IMP0                               \n");
    printf("        a2cv0     for    A2CV0                                \n");
    printf("        a2cv1     for    A2CV1                                \n");
    printf("        a2cv4     for    A2CV4                                \n");
    printf("        a2cv6     for    A2CV6                                \n");
    printf("        a2cn2     for    A2CN2                                \n");
    printf("        a1cnn0    for    A1CNN0                               \n");
    printf("        a2cn0     for    A2CN0                                \n");
    printf("        a1cnn1    for    A1CNN1                               \n");
    printf("        a1dsp1    for    A1DSP1                               \n");
    printf("        a2imp2    for    A2IMP2                               \n");
    printf("        a2cv2     for    A2CV2                                \n");
    printf("        a2cv3     for    A2CV3                                \n");
    printf("        a2cv5     for    A2CV5                                \n");
    printf("        a2cv7     for    A2CV7                                \n");
    printf("        a2cn1     for    A2CN1                                \n");
    printf("        a3vip3    for    A3VIP3                               \n");
    printf("        a3isp01   for    A3ISP01                              \n");
    printf("        a3isp23   for    A3ISP23                              \n");
    }
    printf("    -e: power on domain                                       \n");
    printf("    -d: power off domain                                      \n");
}

int main(int argc, char **argv)
{
    char * opt;
    char dom_name[16];
    int dom_id = 255;
    int sysc_ena = -1;
    int i = 1;
    int ret = 0;

    {
#define CS_MACHINE_LEN (32 + 1)
        char cs_machine_str[CS_MACHINE_LEN] = { 0 };
        const char  *option = NULL;

        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        option = cs_machine_str;

        if (option) {
            if ( strstr( option, "V3U") || strstr( option, "v3u") ||
                        strstr( option, "S4") || strstr( option, "s4") ) {
                rcar_gen3 = 0;
            }
        }
    }

    while (i < argc) {
        opt = argv[i++];
        for (int j = 0; j < strlen(opt); j++) {
            opt[j] = tolower(opt[j]);
        }
        if (strstr(opt,"dom=")) {
            if (rcar_gen3) {
                if (strstr(opt + 4, "ca57_scu")) {
                    dom_id = SYSC_PDR_CA57_SCU;
                    strcpy(dom_name, "CA57_SCU");
                } else if (strstr(opt + 4, "ca53_scu")) {
                    dom_id = SYSC_PDR_CA53_SCU;
                    strcpy(dom_name, "CA53_SCU");
                } else if (strstr(opt + 4, "cr7")) {
                    dom_id = SYSC_PDR_CR7;
                    strcpy(dom_name, "CR7");
                } else if (strstr(opt + 4, "a3ir")) {
                    dom_id = SYSC_PDR_A3IR;
                    strcpy(dom_name, "A3IR");
                } else if (strstr(opt + 4, "a2ir0")) {
                    dom_id = SYSC_PDR_A2IR0;
                    strcpy(dom_name, "A2IR0");
                } else if (strstr(opt + 4, "a2ir1")) {
                    dom_id = SYSC_PDR_A2IR1;
                    strcpy(dom_name, "A2IR1");
                } else if (strstr(opt + 4, "a2ir2")) {
                    dom_id = SYSC_PDR_A2IR2;
                    strcpy(dom_name, "A2IR2");
                } else if (strstr(opt + 4, "a2ir3")) {
                    dom_id = SYSC_PDR_A2IR3;
                    strcpy(dom_name, "A2IR3");
                } else if (strstr(opt + 4, "a2ir4")) {
                    dom_id = SYSC_PDR_A2IR4;
                    strcpy(dom_name, "A2IR4");
                } else if (strstr(opt + 4, "a2ir5")) {
                    dom_id = SYSC_PDR_A2IR5;
                    strcpy(dom_name, "A2IR5");
                } else if (strstr(opt + 4, "a2sc0")) {
                    dom_id = SYSC_PDR_A2SC0;
                    strcpy(dom_name, "A2SC0");
                } else if (strstr(opt + 4, "a2sc1")) {
                    dom_id = SYSC_PDR_A2SC1;
                    strcpy(dom_name, "A2SC1");
                } else if (strstr(opt + 4, "a2sc2")) {
                    dom_id = SYSC_PDR_A2SC2;
                    strcpy(dom_name, "A2SC2");
                } else if (strstr(opt + 4, "a2sc3")) {
                    dom_id = SYSC_PDR_A2SC3;
                    strcpy(dom_name, "A2SC3");
                } else if (strstr(opt + 4, "a2sc4")) {
                    dom_id = SYSC_PDR_A2SC4;
                    strcpy(dom_name, "A2SC4");
                } else if (strstr(opt + 4, "a2dp0")) {
                    dom_id = SYSC_PDR_A2DP0;
                    strcpy(dom_name, "A2DP0");
                } else if (strstr(opt + 4, "a2dp1")) {
                    dom_id = SYSC_PDR_A2DP1;
                    strcpy(dom_name, "A2DP1");
                } else if (strstr(opt + 4, "a2dp")) {
                    dom_id = SYSC_PDR_A2DP;
                    strcpy(dom_name, "A2DP");
                } else if (strstr(opt + 4, "a2cn")) {
                    dom_id = SYSC_PDR_A2CN;
                    strcpy(dom_name, "A2CN");
                } else if (strstr(opt + 4, "a3vip0")) {
                    dom_id = SYSC_PDR_A3VIP0;
                    strcpy(dom_name, "A3VIP0");
                } else if (strstr(opt + 4, "a3vip1")) {
                    dom_id = SYSC_PDR_A3VIP1;
                    strcpy(dom_name, "A3VIP1");
                } else if (strstr(opt + 4, "a3vip2")) {
                    dom_id = SYSC_PDR_A3VIP2;
                    strcpy(dom_name, "A3VIP2");
                } else if (strstr(opt + 4, "a3vp")) {
                    dom_id = SYSC_PDR_A3VP;
                    strcpy(dom_name, "A3VP");
                } else if (strstr(opt + 4, "a3vc")) {
                    dom_id = SYSC_PDR_A3VC;
                    strcpy(dom_name, "A3VC");
                } else if (strstr(opt + 4, "a2vc0")) {
                    dom_id = SYSC_PDR_A2VC0;
                    strcpy(dom_name, "A2VC0");
                } else if (strstr(opt + 4, "a2vc1")) {
                    dom_id = SYSC_PDR_A2VC1;
                    strcpy(dom_name, "A2VC1");
                } else if (strstr(opt + 4, "3dga")) {
                    dom_id = SYSC_PDR_3DGA;
                    strcpy(dom_name, "3DGA");
                } else if (strstr(opt + 4, "3dgb")) {
                    dom_id = SYSC_PDR_3DGB;
                    strcpy(dom_name, "3DGB");
                } else if (strstr(opt + 4, "3dgc")) {
                    dom_id = SYSC_PDR_3DGC;
                    strcpy(dom_name, "3DGC");
                } else if (strstr(opt + 4, "3dgd")) {
                    dom_id = SYSC_PDR_3DGD;
                    strcpy(dom_name, "3DGD");
                } else if (strstr(opt + 4, "3dge")) {
                    dom_id = SYSC_PDR_3DGE;
                    strcpy(dom_name, "3DGE");
                }
            } else {
                if  (strstr(opt + 4, "a1e0d0c0")) {
                    dom_id = SYSC_PDR_A1E0D0C0;
                    strcpy(dom_name, "A1E0D0C0");
                } else if  (strstr(opt + 4, "a1e0d0c1")) {
                    dom_id = SYSC_PDR_A1E0D0C1;
                    strcpy(dom_name, "A1E0D0C1");
                } else if  (strstr(opt + 4, "a1e0d1c0")) {
                    dom_id = SYSC_PDR_A1E0D1C0;
                    strcpy(dom_name, "A1E0D1C0");
                } else if  (strstr(opt + 4, "a1e0d1c1")) {
                    dom_id = SYSC_PDR_A1E0D1C1;
                    strcpy(dom_name, "A1E0D1C1");
                } else if  (strstr(opt + 4, "a1e1d0c0")) {
                    dom_id = SYSC_PDR_A1E1D0C0;
                    strcpy(dom_name, "A1E1D0C0");
                } else if  (strstr(opt + 4, "a1e1d0c1")) {
                    dom_id = SYSC_PDR_A1E1D0C1;
                    strcpy(dom_name, "A1E1D0C1");
                } else if  (strstr(opt + 4, "a1e1d1c0")) {
                    dom_id = SYSC_PDR_A1E1D1C0;
                    strcpy(dom_name, "A1E1D1C0");
                } else if  (strstr(opt + 4, "a1e1d1c1")) {
                    dom_id = SYSC_PDR_A1E1D1C1;
                    strcpy(dom_name, "A1E1D1C1");
                } else if  (strstr(opt + 4, "a2e0d0")) {
                    dom_id = SYSC_PDR_A2E0D0;
                    strcpy(dom_name, "A2E0D0");
                } else if  (strstr(opt + 4, "a2e0d1")) {
                    dom_id = SYSC_PDR_A2E0D1;
                    strcpy(dom_name, "A2E0D1");
                } else if  (strstr(opt + 4, "a2e1d0")) {
                    dom_id = SYSC_PDR_A2E1D0;
                    strcpy(dom_name, "A2E1D0");
                } else if  (strstr(opt + 4, "a2e1d1")) {
                    dom_id = SYSC_PDR_A2E1D1;
                    strcpy(dom_name, "A2E1D1");
                } else if  (strstr(opt + 4, "a3e0")) {
                    dom_id = SYSC_PDR_A3E0;
                    strcpy(dom_name, "A3E0");
                } else if  (strstr(opt + 4, "a3e1")) {
                    dom_id = SYSC_PDR_A3E1;
                    strcpy(dom_name, "A3E1");
                } else if  (strstr(opt + 4, "a1cnn2")) {
                    dom_id = SYSC_PDR_A1CNN2;
                    strcpy(dom_name, "A1CNN2");
                } else if  (strstr(opt + 4, "a1dsp0")) {
                    dom_id = SYSC_PDR_A1DSP0;
                    strcpy(dom_name, "A1DSP0");
                } else if  (strstr(opt + 4, "a2imp0")) {
                    dom_id = SYSC_PDR_A2IMP0;
                    strcpy(dom_name, "A2IMP0");
                } else if  (strstr(opt + 4, "a2dp0")) {
                    dom_id = SYSC_PDR_A2DP0;
                    strcpy(dom_name, "A2DP0");
                } else if  (strstr(opt + 4, "a2cv0")) {
                    dom_id = SYSC_PDR_A2CV0;
                    strcpy(dom_name, "A2CV0");
                } else if  (strstr(opt + 4, "a2cv1")) {
                    dom_id = SYSC_PDR_A2CV1;
                    strcpy(dom_name, "A2CV1");
                } else if  (strstr(opt + 4, "a2cv4")) {
                    dom_id = SYSC_PDR_A2CV4;
                    strcpy(dom_name, "A2CV4");
                } else if  (strstr(opt + 4, "a2cv6")) {
                    dom_id = SYSC_PDR_A2CV6;
                    strcpy(dom_name, "A2CV6");
                } else if  (strstr(opt + 4, "a2cn2")) {
                    dom_id = SYSC_PDR_A2CN2;
                    strcpy(dom_name, "A2CN2");
                } else if  (strstr(opt + 4, "a1cnn0")) {
                    dom_id = SYSC_PDR_A1CNN0;
                    strcpy(dom_name, "A1CNN0");
                } else if  (strstr(opt + 4, "a2cn0")) {
                    dom_id = SYSC_PDR_A2CN0;
                    strcpy(dom_name, "A2CN0");
                } else if  (strstr(opt + 4, "a3ir")) {
                    dom_id = SYSC_PDR_A3IR;
                    strcpy(dom_name, "A3IR");
                } else if  (strstr(opt + 4, "a1cnn1")) {
                    dom_id = SYSC_PDR_A1CNN1;
                    strcpy(dom_name, "A1CNN1");
                } else if  (strstr(opt + 4, "a1dsp1")) {
                    dom_id = SYSC_PDR_A1DSP1;
                    strcpy(dom_name, "A1DSP1");
                } else if  (strstr(opt + 4, "a2imp2")) {
                    dom_id = SYSC_PDR_A2IMP2;
                    strcpy(dom_name, "A2IMP2");
                } else if  (strstr(opt + 4, "a2cv2")) {
                    dom_id = SYSC_PDR_A2CV2;
                    strcpy(dom_name, "A2CV2");
                } else if  (strstr(opt + 4, "a2cv3")) {
                    dom_id = SYSC_PDR_A2CV3;
                    strcpy(dom_name, "A2CV3");
                } else if  (strstr(opt + 4, "a2cv5")) {
                    dom_id = SYSC_PDR_A2CV5;
                    strcpy(dom_name, "A2CV5");
                } else if  (strstr(opt + 4, "a2cv7")) {
                    dom_id = SYSC_PDR_A2CV7;
                    strcpy(dom_name, "A2CV7");
                } else if  (strstr(opt + 4, "a2cn1")) {
                    dom_id = SYSC_PDR_A2CN1;
                    strcpy(dom_name, "A2CN1");
                } else if  (strstr(opt + 4, "a3vip3")) {
                    dom_id = SYSC_PDR_A3VIP3;
                    strcpy(dom_name, "A3VIP3");
                } else if  (strstr(opt + 4, "a3isp01")) {
                    dom_id = SYSC_PDR_A3ISP01;
                    strcpy(dom_name, "A3ISP01");
                } else if  (strstr(opt + 4, "a3isp23")) {
                    dom_id = SYSC_PDR_A3ISP23;
                    strcpy(dom_name, "A3ISP23");
                }
            }
        } else if (strstr(opt,"-e")) {
            sysc_ena = 1;
        } else if (strstr(opt,"-d")) {
            sysc_ena = 0;
        } else {
            printf("Unknown option %s\n", opt);
            return -1;
        }
    }

    if (dom_id == 255) {
        usage();
        return -1;
    }
    else if (dom_id == -1) {
        printf("Power domain %s is not supported\n", dom_name);
        return -1;
    }
    /* Get functions table from CPG lib */
    if (sysc_mgr_getfuncs(&syschwfuncs, sizeof(sysc_mgr_funcs_t))){
        printf("sysc_mgr_getfuncs failed \n");
        return -1;
    }

    /* Open an instant and clock cpg init */
    if (syschwfuncs.init()) {
        printf("sysc_init failed \n");
        return -1;
    }
    if ( sysc_ena == -1) { 
        /* Read clock info */
        printf("Power domain %s info:\n", dom_name);
        ret = syschwfuncs.power_is_off(dom_id);
        if (ret == 1)
            printf("\tStatus  : OFF\n");
        else if(ret == 0)
            printf("\tStatus  : ON\n");
        else if(ret > 0)
            printf("\tStatus  : Unknown\n");
        else
            printf("\tStatus  : No support (ret=%d)\n", ret);
        ret = syschwfuncs.read_pdmode();
        if (ret == -1)
            printf("\tPDMODE  : No support (ret=%d)\n", ret);
        else
            printf("\tPDMODE = %d\n", ret);
    } else {
        if ( sysc_ena == 0 ) {
            /* Power off */
            if (syschwfuncs.power_off(dom_id)) {
                printf("Cannot power off domain %s\n", dom_name);
            } else {
                printf("Power off domain %s successfully\n", dom_name);
            }
        } else if ( sysc_ena == 1 ) {
            /* Power on */
            if (syschwfuncs.power_on(dom_id)) {
                printf("Cannot power on domain %s\n", dom_name);
            } else {
                printf("Power on domain %s successfully\n", dom_name);
            }
        }
    }

    syschwfuncs.deinit();

    return 0;
}

