/*
 * Copyright (c) 2020, 2023, BlackBerry Limited. All rights reserved.
 * Copyright (c) 2022, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/syspage.h>
#include <hw/rcar_cpg.h>

#define CPG_CLK_MSTP    (CPG_CLK_UNKNOWN + 1)
static cpg_mgr_funcs_t cpghwfuncs;

typedef enum {
    OPT_INIT=-1,
    CLOCK_DIS=0,
    CLOCK_EN,
    RESET_MOD_MSTP_REG,
    RESET_MOD_MSTP_REG_FLOW_A,
    RESET_MOD_MSTP_REG_FLOW_B,
    RESET_MOD_MSTP_REG_FLOW_C,
    ASSERT_RESET_MOD_MSTP_REG,
    ASSERT_RESET_MOD_MSTP_REG_FLOW_A,
    ASSERT_RESET_MOD_MSTP_REG_FLOW_B,
    ASSERT_RESET_MOD_MSTP_REG_FLOW_C,
    DEASSERT_RESET_MOD_MSTP_REG,
} option_t;

#ifndef VARIANT_pm
static void usage(void)
{
    printf("Syntax:                                                                                              \n");
    printf("    cpg_ctrl mod=<module_name> idx=<module_chan_index>  clk=<clock_value> div=<divider_value> -e -d  \n");
    printf("    or                                                                                               \n");
    printf("    cpg_ctrl mod=mstp idx=<reg_index>  bit=<bit_idx> -e -d -r -A -B -C                               \n");
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
    printf("        rsw2  for RSW2                                                                               \n");
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
    printf("    -ra: Reset module by MSTP register follow flow A                                                 \n");
    printf("    -rb: Reset module by MSTP register follow flow B                                                 \n");
    printf("    -rc: Reset module by MSTP register follow flow C                                                 \n");
    printf("    -aa: Assert a reset to a module by MSTP register follow flow A                                   \n");
    printf("    -ab: Assert a reset to a module by MSTP register follow flow B                                   \n");
    printf("    -ac: Assert a reset to a module by MSTP register follow flow C                                   \n");
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
#endif // VARIANT_pm

int main(int argc, char **argv)
{
    char * opt, * end;
    char mod_name[16];
    int mod_id = CPG_CLK_UNKNOWN;
    int chan_idx = 0;
    uint32_t clk_val = 0;
    int clk_div = -1;
    option_t test_opt = OPT_INIT;
    int bit_idx = -1;
    int i = 1;
    int ret = 0;
    #ifdef VARIANT_pm
    uint32_t freq_mhz     = 0;
    uint32_t freq_opp     = UINT32_MAX;
    uint32_t freq_rng     = 0;
    int      freq_getset  = -1;
    #endif

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
            } else if (strstr(opt + 4,"rsw2")) {
                mod_id = CPG_CLK_RSW2;
                strcpy(mod_name, "RSW2");
            } else if (strstr(opt + 4,"mstp")) {
                mod_id = CPG_CLK_MSTP;
                strcpy(mod_name, "MSTP");
            }
        } else if (strstr(opt,"idx=")) {
            chan_idx = (int) atol(opt + 4);
        } else if (strstr(opt,"clk=")) {
            opt += 4;
            clk_val = (uint32_t) strtoul(opt, &end, 10);
            if(opt != end) {
                switch(*end) {
                case 'g':
                case 'G':
                    clk_val *= 1000;
                    /* fall through */
                case 'm':
                case 'M':
                    clk_val *= 1000;
                    /* fall through */
                case 'k':
                case 'K':
                    clk_val *= 1000;
                    break;
                default:
                    break;
                }
            }
        } else if (strstr(opt,"div=")) {
            clk_div = (int) atol(opt + 4);
        } else if (strstr(opt,"bit=")) {
            bit_idx = (int) atol(opt + 4);
        } else if (strstr(opt,"-e")) {
            test_opt = CLOCK_EN;
        } else if (strstr(opt,"-d")) {
            test_opt = CLOCK_DIS;
        } else if (strstr(opt,"-ra")) {
            test_opt = RESET_MOD_MSTP_REG_FLOW_A;
        } else if (strstr(opt,"-rb")) {
            test_opt = RESET_MOD_MSTP_REG_FLOW_B;
        } else if (strstr(opt,"-rc")) {
            test_opt = RESET_MOD_MSTP_REG_FLOW_C;
        } else if (strstr(opt,"-aa")) {
            test_opt = ASSERT_RESET_MOD_MSTP_REG_FLOW_A;
        } else if (strstr(opt,"-ab")) {
            test_opt = ASSERT_RESET_MOD_MSTP_REG_FLOW_B;
        } else if (strstr(opt,"-ac")) {
            test_opt = ASSERT_RESET_MOD_MSTP_REG_FLOW_C;
        } else if (strstr(opt,"-r")) {
            test_opt = RESET_MOD_MSTP_REG;
        } else if (strstr(opt,"-a")) {
            test_opt = ASSERT_RESET_MOD_MSTP_REG;
        } else if (strstr(opt,"-b")) {
            test_opt = DEASSERT_RESET_MOD_MSTP_REG;
        #ifdef VARIANT_pm
        } else if (strstr(opt, "freq=")) {
             opt += 5;
             freq_mhz = (uint32_t) strtoul(opt, &end, 10);
             if (0 == freq_mhz) {
                 freq_getset = 0;
             } else {
                 freq_getset = 1;
             }
        } else if (strstr(opt, "freq_opp=")) {
             opt += 9;
             freq_opp = (uint32_t) strtoul(opt, &end, 10);
        } else if (strstr(opt, "freq_rng=")) {
             opt += 9;
             freq_rng = (uint32_t) strtoul(opt, &end, 10);
        #endif // VARIANT_pm
        } else {
            printf("Unknown option %s\n", opt);
            return -1;
        }
    }

    if (mod_id == CPG_CLK_UNKNOWN) {
        #ifdef VARIANT_pm
        printf("Refer to use cpg_ctrl_pm for more information\n");
        #else
        usage();
        #endif
        return -1;
    }
    /* Get functions table from CPG lib */
    if (cpg_mgr_getfuncs(&cpghwfuncs, (int)sizeof(cpg_mgr_funcs_t))){
        printf("cpg_mgr_getfuncs failed \n");
        return -1;
    }

    /* Open an instant and clock cpg init */
    if (cpghwfuncs.init(1)) {
        printf("clk_init failed \n");
        return -1;
    }
    if (mod_id != CPG_CLK_MSTP) {
        #ifdef VARIANT_pm
        if (UINT32_MAX > freq_opp) {
            uint32_t opp = UINT32_MAX;
            /* get Operating Performance Point */
            if (CPG_RET_OK != cpghwfuncs.clk_freq_get_opp(mod_id, chan_idx, freq_opp, freq_rng, &opp)) {
                printf("%s (cluster %d): Unsupported Operating Performance Point: freq %u (range +/- %u)\n", mod_name, chan_idx, freq_opp, freq_rng);
            } else {
                printf("%s (cluster %d) opp would be %u for freq %u (range +/- %u)\n", mod_name, chan_idx, opp, freq_opp, freq_rng);
            }
        } else if (0 == freq_getset) {
            /* get freq */
            if (CPG_RET_OK != cpghwfuncs.clk_freq_get(mod_id, chan_idx, &freq_mhz)) {
                printf("Cannot get freq\n");
            } else {
                printf("Get %s (cluster %d) freq = %u MHz\n", mod_name, chan_idx, freq_mhz);
            }
        } else if (1 == freq_getset) {
            /* set freq */
            if (CPG_RET_OK != cpghwfuncs.clk_freq_set_opp(mod_id, chan_idx, &freq_mhz)) {
                printf("Cannot set %s%d freq to %u\n", mod_name, chan_idx, freq_mhz);
            } else {
                printf("Set %s (cluster %d)freq to %u MHz\n", mod_name, chan_idx, freq_mhz);
            }
        } else
        #endif // VARIANT_pm
        if ( (clk_val== 0) && (test_opt == OPT_INIT) && (clk_div == -1)) {
            /* Read clock info */
            printf("%s%d clock info:\n", mod_name, chan_idx);
            ret = cpghwfuncs.clk_is_enabled(mod_id,chan_idx);
            if (ret == 1) {
                printf("\tStatus  : Enabled\n");
            } else if(ret == 0) {
                printf("\tStatus  : Disabled\n");
            } else if(ret > 0) {
                printf("\tStatus  : Unknown\n");
            } else {
                printf("\tStatus  : No support (ret=%d)\n", ret);
            }

            if (!(ret=cpghwfuncs.clk_rate_get(mod_id,chan_idx, &clk_val))) {
                printf("\tClock   : %0.4fMHz\n", (float)((float)clk_val / 1000000.0));
            } else {
                printf("\tClock   : No support (ret=%d)\n", ret);
            }

            if (!(ret=cpghwfuncs.clk_divider_get(mod_id,chan_idx, (uint32_t*)&clk_div))) {
                printf("\tDivider : %d(0x%x)\n", clk_div, clk_div);
            } else {
                printf("\tDivider : No support (ret=%d)\n", ret);
            }
        } else {
            if ( clk_val != 0 ) {
                /* Change clock */
                if (cpghwfuncs.clk_rate_set(mod_id,chan_idx, &clk_val)) {
                    printf("Cannot set %s%d clock\n", mod_name, chan_idx);
                } else {
                    printf("Set %s%d clock to %0.4fMHz successfully\n", mod_name, chan_idx, (float)((float)clk_val / 1000000.0));
                }
            } else if (clk_div != -1) {
                /* Change clock by change divider: expected clock = input_parent_clock(800MHz)/(clk_div + 1) */
                if (cpghwfuncs.clk_divider_set(mod_id, chan_idx, (uint32_t*)&clk_div)) {
                    printf("Cannot set %s%d clock divider\n", mod_name, chan_idx);
                } else {
                    printf("Set %s%d clock divider to %d(0x%x) successfully\n", mod_name, chan_idx, clk_div, clk_div);
                }
            }
            /* Enable clock */
            if ( test_opt == CLOCK_EN ) {
                if (cpghwfuncs.clk_enable(mod_id,chan_idx)) {
                    printf("Cannot enable %s%d clock\n", mod_name, chan_idx);
                } else {
                    /* Check clock enabled or not */
                    if (cpghwfuncs.clk_is_enabled(mod_id,chan_idx) == 1) {
                        printf("Enable %s%d clock successfully\n", mod_name, chan_idx);
                    } else {
                        printf("Cannot enable %s%d clock\n", mod_name, chan_idx);
                    }
                }
            }
            /* Disable clock */
            if ( test_opt == CLOCK_DIS ) {
                if (cpghwfuncs.clk_disable(mod_id,chan_idx)) {
                    printf("Cannot Disable %s%d clock\n", mod_name, chan_idx);
                } else {
                    /* Check clock disabled or not */
                    if (cpghwfuncs.clk_is_enabled(mod_id,chan_idx) == 0) {
                        printf("Disable %s%d clock successfully\n", mod_name, chan_idx);
                    } else {
                        printf("Cannot disable %s%d clock\n", mod_name, chan_idx);
                    }
                }
            }
        }
    } else {
        const uint32_t mstp_id = CPG_MSTP_REG_BIT_TO_ID(chan_idx, bit_idx);
        if (test_opt == OPT_INIT) {
            /* Read clock info */
            printf("%s-%d:%d info:\n", mod_name, chan_idx, bit_idx);
            ret = cpghwfuncs.mstp_is_enabled(mstp_id);
            if (ret == 1) {
                printf("\tStatus  : Enabled\n");
            } else if(ret == 0) {
                printf("\tStatus  : Disabled\n");
            } else if(ret >= 0) {
                printf("\tStatus  : Unknown\n");
            } else {
                printf("\tStatus  : No support (ret=%d)\n", ret);
            }

            ret = cpghwfuncs.mstp_reset_state(mstp_id);
            if (ret == 1) {
                printf("\tReset state  : Reset\n");
            } else if(ret == 0) {
                printf("\tReset state  : Release\n");
            } else if(ret > 0) {
                printf("\tReset state  : Unknown\n");
            } else {
                printf("\tReset state  : No support (ret=%d)\n", ret);
            }
        } else {
            if ( test_opt == CLOCK_DIS ) {
                /* Diable clock */
                if (cpghwfuncs.mstp_disable(mstp_id)) {
                    printf("Cannot Disable %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Disable %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == CLOCK_EN ) {
                /* Enable clock */
                if (cpghwfuncs.mstp_enable(mstp_id)) {
                    printf("Cannot enable %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Enable %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == RESET_MOD_MSTP_REG ) {
                /* Reset */
                if (cpghwfuncs.mstp_reset(mstp_id)) {
                    printf("Cannot reset %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Reset %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == ASSERT_RESET_MOD_MSTP_REG ) {
                /* Assert reset */
                if (cpghwfuncs.mstp_assert(mstp_id)) {
                    printf("Cannot aseert reset %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Assert reset %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == DEASSERT_RESET_MOD_MSTP_REG ) {
                /* De-assert reset */
                if (cpghwfuncs.mstp_deassert(mstp_id)) {
                    printf("Cannot deaseert reset %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Deassert reset %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == RESET_MOD_MSTP_REG_FLOW_A ) {
                /* Reset sequence A */
                if (cpghwfuncs.mstp_reset_gen4_a(mstp_id)) {
                    printf("Cannot reset (Type A) %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Reset (Type A) %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == RESET_MOD_MSTP_REG_FLOW_B ) {
                /* Reset sequence B */
                if (cpghwfuncs.mstp_reset_gen4_b(mstp_id)) {
                    printf("Cannot reset (Type B) %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Reset (Type B) %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == RESET_MOD_MSTP_REG_FLOW_C ) {
                /* Reset sequence C */
                if (cpghwfuncs.mstp_reset_gen4_c(mstp_id)) {
                    printf("Cannot reset (Type C) %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Reset (Type C) %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }

            } else if ( test_opt == ASSERT_RESET_MOD_MSTP_REG_FLOW_A ) {
                /* Assert Reset sequence A */
                if (cpghwfuncs.mstp_assert_gen4_a(mstp_id)) {
                    printf("Cannot assert reset (Type A) %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Assert reset (Type A) %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == ASSERT_RESET_MOD_MSTP_REG_FLOW_B ) {
                /* Assert Reset sequence B */
                if (cpghwfuncs.mstp_assert_gen4_b(mstp_id)) {
                    printf("Cannot assert reset (Type B) %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Assert reset (Type B) %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            } else if ( test_opt == ASSERT_RESET_MOD_MSTP_REG_FLOW_C ) {
                /* Assert Reset sequence C */
                if (cpghwfuncs.mstp_assert_gen4_c(mstp_id)) {
                    printf("Cannot assert reset (Type C) %s-%d:%d\n", mod_name, chan_idx, bit_idx);
                } else {
                    printf("Assert reset (Type C) %s-%d:%d successfully\n", mod_name, chan_idx, bit_idx);
                }
            }
        }
    }

    cpghwfuncs.deinit();

    return 0;
}

