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
static cpg_mgr_funcs_t  clkhwfuncs;

void usage()
{
    printf("Syntax:                                                                                              \n");
    printf("    cpg_ctrl mod=<module_name> idx=<module_chan_index>  clk=<clock_value> div=<divider_value> -e -d  \n");
    printf("    or                                                                                               \n");
    printf("    cpg_ctrl mod=mstp idx=<reg_index>  bit=<bit_idx> -e -d -r                                        \n");
    printf("                                                                                                     \n");
    printf("    <module_name>: Module name                                                                       \n");
    printf("        hdmi  for HDMI                                                                               \n");
    printf("        canfd for CANFD                                                                              \n");
    printf("        msiof for MSIOF                                                                              \n");
    printf("        csi2  for CSI2                                                                               \n");
    printf("        dsi   for DSI                                                                                \n");
    printf("        ipc   for IPC                                                                                \n");
    printf("        sdhi  for SDHIn                                                                              \n");
    printf("        rpc   for RPC (flash)                                                                        \n");
    printf("        pll   for PLL                                                                                \n");
    printf("        cpu   for CA53/CA57 CPU                                                                      \n");
    printf("        post  for POST                                                                               \n");
    printf("        mstp  for Modules by accessing MSTP registers                                                \n");
    printf("    <module_chan_index>: Channel index                                                               \n");
    printf("        0, 1, 2, 3 for SDHI0,1,2,3                                                                   \n");
    printf("        0, 1, 2, 3, 4, 5, 20, 21, 30, 31 for PLL0,1,2,3,4,5,20,21,30,31                              \n");
    printf("        0, 2, 3, 4 for POST,2,3,4                                                                    \n");
    printf("    <clock_value>: Expected clock value                                                              \n");
    printf("    <divider_value>: clock divider value need to set to clock register                               \n");
    printf("    <reg_idx>: Index of MSTP register                                                                \n");
    printf("    <bit_idx>: Bit index from 0 to 31 in MSTP register                                               \n");
    printf("    -e: Enable clock                                                                                 \n");
    printf("    -d: Disable clock                                                                                \n");
    printf("    -r: Reset module by MSTP register                                                                \n");
    printf("    -a: Assert a reset to a module by MSTP register                                                  \n");
    printf("    -b: Deassert a reset to a module by MSTP register                                                \n");
    printf("    To get clock info, don't use clk, -e and -d options                                              \n");
    printf("Example:                                                                                             \n");
    printf("    - Change POST clock to 66,66MHz                                                                  \n");
    printf("    cpg_ctrl mod=post idx=0 clk=66666666                                                             \n");
    printf("                                                                                                     \n");
    printf("    - Change POST clock to 66,66MHz and enable clock                                                 \n");
    printf("    cpg_ctrl mod=post idx=0 clk=66666666 -e                                                          \n");
    printf("                                                                                                     \n");
    printf("    - Disbale POST clock                                                                             \n");
    printf("    cpg_ctrl mod=post idx=0 -d                                                                       \n");
    printf("                                                                                                     \n");
    printf("    - Get POST clock info                                                                            \n");
    printf("    cpg_ctrl mod=post idx=0                                                                          \n");
    printf("                                                                                                     \n");
    printf("Note : <clock_value> and <divider_value> should not input concurrently                               \n");
}

int main(int argc, char **argv)
{
    char * opt, * end;
    char mod_name[16];
    int mod_id = CPG_CLK_UNKNOWN;
    int chan_idx = 0;
    uint32_t clk_val = 0;
    int clk_div = -1;
    int clk_ena = -1;
    int bit_idx = -1;
    int i = 1;
    int ret = 0;

    while (i < argc) {
        opt = argv[i++];
        for (int j = 0; j < strlen(opt); j++) {
            opt[j] = tolower(opt[j]);
        }
        if (strstr(opt,"mod=")) {
            if (strstr(opt + 4,"hdmi")) {
                mod_id = CPG_CLK_HDMI;
                strcpy(mod_name, "HDMI");
            } else if (strstr(opt + 4,"canfd")) {
                mod_id = CPG_CLK_CANFD;
                strcpy(mod_name, "CANFD");
            } else if (strstr(opt + 4,"msiof")) {
                mod_id = CPG_CLK_MSIOF;
                strcpy(mod_name, "MSIOF");
            } else if (strstr(opt + 4,"csi")) {
                mod_id = CPG_CLK_CSI2;
                strcpy(mod_name, "CSI2");
            } else if (strstr(opt + 4,"dsi")) {
                mod_id = CPG_CLK_DSI;
                strcpy(mod_name, "DSI");
            } else if (strstr(opt + 4,"ipc")) {
                mod_id = CPG_CLK_IPC;
                strcpy(mod_name, "IPC");
            } else if (strstr(opt + 4,"sdhi")) {
                mod_id = CPG_CLK_SDHI;
                strcpy(mod_name, "SDHI");
            } else if (strstr(opt + 4,"pll")) {
                mod_id = CPG_CLK_PLL;
                strcpy(mod_name, "PLL");
            } else if (strstr(opt + 4,"cpu")) {
                mod_id = CPG_CLK_CPU;
                strcpy(mod_name, "CPU");
            } else if (strstr(opt + 4,"post")) {
                mod_id = CPG_CLK_POST;
                strcpy(mod_name, "POST");
            } else if (strstr(opt + 4,"mstp")) {
                mod_id = CPG_CLK_MSTP;
                strcpy(mod_name, "MSTP");
            }
        } else if (strstr(opt,"idx=")) {
            chan_idx = atol(opt + 4);
        } else if (strstr(opt,"clk=")) {
            opt += 4;
            clk_val = strtoul(opt, &end, 10);
            if(opt != end) {
                switch(*end) {
                case 'g': case 'G':
                    clk_val *= 1000;
                    /* fall through */
                case 'm': case 'M':
                    clk_val *= 1000;
                    /* fall through */
                case 'k': case 'K':
                    clk_val *= 1000;
                    break;
                default:
                    break;
                }
            }
        } else if (strstr(opt,"div=")) {
            clk_div = atoll(opt + 4);
        } else if (strstr(opt,"bit=")) {
            bit_idx = atoll(opt + 4);
        } else if (strstr(opt,"-e")) {
            clk_ena = 1;
        } else if (strstr(opt,"-d")) {
            clk_ena = 0;
        } else if (strstr(opt,"-r")) {
            clk_ena = 2;
        } else if (strstr(opt,"-a")) {
            clk_ena = 6;
        } else if (strstr(opt,"-b")) {
            clk_ena = 7;
        } else {
            printf("Unknown option %s\n", opt);
            return -1;
        }
    }

    if (mod_id == CPG_CLK_UNKNOWN) {
        usage();
        return -1;
    }
    /* Get functions table from CPG lib */
    if (cpg_mgr_getfuncs(&clkhwfuncs, sizeof(cpg_mgr_funcs_t))){
        printf("cpg_mgr_getfuncs failed \n");
        return -1;
    }

    /* Open an instant and clock cpg init */
    if (clkhwfuncs.init(1)) {
        printf("clk_init failed \n");
        return -1;
    }
    if (mod_id != CPG_CLK_MSTP) {
        if ( clk_val== 0 && clk_ena == -1 && clk_div == -1) { 
            /* Read clock info */
            printf("%s%d clock info:\n", mod_name, chan_idx);
            ret = clkhwfuncs.clk_is_enabled(mod_id,chan_idx);
            if (ret == 1)
                printf("\tStatus  : Enabled\n");
            else if(ret == 0)
                printf("\tStatus  : Disabled\n");
            else if(ret > 0)
                printf("\tStatus  : Unknown\n");
            else
                printf("\tStatus  : No support (ret=%d)\n", ret);

            if (!(ret=clkhwfuncs.clk_rate_get(mod_id,chan_idx, &clk_val)))
                printf("\tClock   : %0.4fMHz\n", (float)((float)clk_val / 1000000.0));
            else
                printf("\tClock   : No support (ret=%d)\n", ret);

            if (!(ret=clkhwfuncs.clk_divider_get(mod_id,chan_idx, (uint32_t*)&clk_div)))
                printf("\tDivider : %d(0x%x)\n", clk_div, clk_div);
            else
                printf("\tDivider : No support (ret=%d)\n", ret);

        } else {
            if ( clk_val != 0 ) {
                /* Change clock */
                if (clkhwfuncs.clk_rate_set(mod_id,chan_idx, &clk_val)) {
                    printf("Cannot set %s%d clock\n", mod_name, chan_idx);
                } else {
                    printf("Set %s%d clock to %0.4fMHz successfully\n", mod_name, chan_idx, (float)((float)clk_val / 1000000.0));
                }
            } else if ( clk_div != -1 ) {
                /* Change clock by change divider: expected clock = input_parent_clock(800MHz)/(clk_div + 1) */
                if (clkhwfuncs.clk_divider_set(mod_id, chan_idx, (uint32_t*)&clk_div)) {
                    printf("Cannot set %s%d clock divider\n", mod_name, chan_idx);
                } else {
                    printf("Set %s%d clock divider to %d(0x%x) successfully\n", mod_name, chan_idx, clk_div, clk_div);
                }
            }
            /* Enable clock */
            if ( clk_ena == 1 ) {
                if (clkhwfuncs.clk_enable(mod_id,chan_idx)) {
                    printf("Cannot enable %s%d clock\n", mod_name, chan_idx);
                } else {
                    /* Check clock enabled or not */
                    if (clkhwfuncs.clk_is_enabled(mod_id,chan_idx) == 1) {
                        printf("Enable %s%d clock successfully\n", mod_name, chan_idx);
                    } else {
                        printf("Cannot enable %s%d clock\n", mod_name, chan_idx);
                    }
                }
            }
            /* Disable clock */
            if ( clk_ena == 0 ) {
                if (clkhwfuncs.clk_disable(mod_id,chan_idx)) {
                    printf("Cannot Disable %s%d clock\n", mod_name, chan_idx);
                } else {
                    /* Check clock disabled or not */
                    if (!clkhwfuncs.clk_is_enabled(mod_id,chan_idx)) {
                        printf("Disable %s%d clock successfully\n", mod_name, chan_idx);
                    } else {
                        printf("Cannot disable %s%d clock\n", mod_name, chan_idx);
                    }
                }
            }
        }
    } else {
        uint32_t mstp_id = CPG_MSTP_REG_BIT_TO_ID(chan_idx, bit_idx);
        if ( clk_ena == -1) { 
            /* Read clock info */
            printf("%s-%d:%d info:\n", mod_name, chan_idx, bit_idx);
            ret = clkhwfuncs.mstp_is_enabled(mstp_id);
            if (ret == 1)
                printf("\tClock status : Enabled\n");
            else if(ret == 0)
                printf("\tClock status : Disabled\n");
            else if(ret > 0)
                printf("\tClock status : Unknown\n");
            else
                printf("\tClock status : No support (ret=%d)\n", ret);

            ret = clkhwfuncs.mstp_reset_state(mstp_id);
            if (ret == 1)
                printf("\tReset state  : Reset\n");
            else if(ret == 0)
                printf("\tReset state  : Release\n");
            else if(ret > 0)
                printf("\tReset state  : Unknown\n");
            else
                printf("\tReset state  : No support (ret=%d)\n", ret);
        } else {
            if ( clk_ena == 0 ) {
                /* Diable clock */
                if (clkhwfuncs.mstp_disable(mstp_id)) {
                    printf("Cannot Disable %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Disable %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( clk_ena == 1 ) {
                /* Enable clock */
                if (clkhwfuncs.mstp_enable(mstp_id)) {
                    printf("Cannot enable %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Enable %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( clk_ena == 2 ) {
                /* Reset */
                if (clkhwfuncs.mstp_reset(mstp_id)) {
                    printf("Cannot reset %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Reset %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( clk_ena == 6 ) {
                /* Assert reset */
                if (clkhwfuncs.mstp_assert(mstp_id)) {
                    printf("Cannot aseert reset %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Assert reset %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( clk_ena == 7 ) {
                /* De-assert reset */
                if (clkhwfuncs.mstp_deassert(mstp_id)) {
                    printf("Cannot deaseert reset %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Deassert reset %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            }
        }
    }

    clkhwfuncs.deinit();

    return 0;
}

