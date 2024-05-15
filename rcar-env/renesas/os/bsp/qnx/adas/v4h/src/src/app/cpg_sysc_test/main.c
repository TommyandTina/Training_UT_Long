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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syspage.h>
#include <hw/rcar_cpg.h>
#include <hw/rcar_sysc.h>

/* 
 * Enable clock supply for VSPD and FCPV in WFD driver, an issue found is to power 
 * ON/OFF ISP and IMP domain cannot work, WFD driver ver.1.2 fixed the issue.
 */

static cpg_mgr_funcs_t clkhwfuncs;
static sysc_mgr_funcs_t syschwfuncs;

void clk_check_print(char *name, int id);
void power_check_print(char *name, int pdr_num);

void clk_check_print(char *name, int id) {
    int check;

    check = clkhwfuncs.mstp_is_enabled(id);
    if(check == 1) printf("%s Clock : Supplied \n",name);
    else if(check == 0) printf("%s Clock : Not Supplied \n",name);
    else printf("%s Clock : Error \n",name);
}

void power_check_print(char *name, int pdr_num) {
    int check;

    check = syschwfuncs.power_is_off(pdr_num);
    if(check == 1) printf("%s : Power off \n",name);
    else if(check == 0) printf("%s : Power on \n",name);
    else printf("%s Power : Error \n\n",name);
}

void rcar_s4_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car S4\n");
    printf("----------------------------------------\n");
}

void rcar_v4h_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car V4H\n");
    printf("----------------------------------------\n");
/*######################### ISP test ###################################*/

    printf("############## ISP0/1 Clock&Power test ##############\n");

    /* ###Disable CPG ISP0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN4_ISP0) || clkhwfuncs.mstp_disable(CPG_MSTP_GEN4_ISP1)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");

    clk_check_print("ISP0",CPG_MSTP_GEN4_ISP0);
    clk_check_print("ISP1",CPG_MSTP_GEN4_ISP1);
    printf("\n");

    /* ###Power off ISP0/1 ### */
    if(syschwfuncs.power_off(60) || syschwfuncs.power_off(61)) printf("power_off failed \n");
    else printf("power_off succeed \n");

    power_check_print("ISP0",60);
    power_check_print("ISP1",61);

    /* ###Power on ISP0/1 ### */
    if(syschwfuncs.power_on(60) || syschwfuncs.power_on(61)) printf("power_on failed \n");
    else printf("power_on succeed\n");

    power_check_print("ISP0",60);
    power_check_print("ISP1",61);

    /* ###Enable CPG ISP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN4_ISP0) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN4_ISP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");

    clk_check_print("ISP0",CPG_MSTP_GEN4_ISP0);
    clk_check_print("ISP1",CPG_MSTP_GEN4_ISP1);
    printf("\n");    

/*######################### IMP test ###################################*/
    printf("############## IMP0/1 Clock&Power OFF test ##############\n");

    /* ### Disable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN4_IMP0) || clkhwfuncs.mstp_disable(CPG_MSTP_GEN4_IMP1)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");

    /* ### Power off IMP0/1 ### */
    if(syschwfuncs.power_off(34)) printf("power_off failed \n");
    else printf("power_off succeed \n");

    power_check_print("IMP0/1",34);

//    printf("############## IMP2/3 Clock&Power OFF test ##############\n");
//
//    /* ### Disable CPG IMP0/1 core clock ###*/
//    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN4_IMP2) || clkhwfuncs.mstp_disable(CPG_MSTP_GEN4_IMP3)) printf("mstp_disable failed \n");
//    else printf("mstp_disable succeed \n");
//
//    /* ### Power off IMP2/3 ### */
//    if(syschwfuncs.power_off(46)) printf("power_off failed \n");
//    else printf("power_off succeed \n");
//    
//    power_check_print("IMP2/3",46);

    printf("############## A3IR domain power off test ##############\n");

    /* ###Power off A3IR ### */
    if(syschwfuncs.power_off(43)) printf("power_off failed \n");
    else printf("power_off succeed \n");

    power_check_print("A3IR",43);

    printf("############## A3IR domain power ON test ##############\n");

    /* ###Power on A3IR ### */
    if(syschwfuncs.power_on(43)) printf("power_on failed \n");
    else printf("power_on succeed \n");

    power_check_print("A3IR",43);

    /* ### Power on IMP0/1 ### */
    if(syschwfuncs.power_on(34)) printf("power_on failed \n");
    else printf("power_on succeed\n");

    power_check_print("IMP0/1",34);

    /* ### Enable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN4_IMP0) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN4_IMP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");

    clk_check_print("IMP0",CPG_MSTP_GEN4_IMP0);
    clk_check_print("IMP1",CPG_MSTP_GEN4_IMP1);

//    /* ### Power on IMP2/3 ### */
//    if(syschwfuncs.power_on(46)) printf("power_on failed \n");
//    else printf("power_on succeed\n");
//    
//    power_check_print("IMP2/3",46);
//
//    /* ### Enable CPG IMP2/3 core clock ###*/
//    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN4_IMP2) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN4_IMP3)) printf("mstp_enable failed \n");
//    else printf("mstp_enable succeed \n");
//        
//    clk_check_print("IMP2",CPG_MSTP_GEN4_IMP2);
//    clk_check_print("IMP3",CPG_MSTP_GEN4_IMP3);

    printf("\n");
}

int main(int argc, char **argv)
{
    /* Get functions table from CPG lib */
    if (cpg_mgr_getfuncs(&clkhwfuncs, sizeof(cpg_mgr_funcs_t))){
        printf("clk_mgr_getfuncs failed \n");
        return -1;
    }

    /* Get functions table from SYSC lib */
    if (sysc_mgr_getfuncs(&syschwfuncs, sizeof(sysc_mgr_funcs_t))){
        printf("sysc_mgr_getfuncs failed \n");
        return -1;
    }

    /* sysc func init */
    if (syschwfuncs.init()){
        printf("sysc_init failed\n");
        return -1;
    }

    /* Open cpg_mstp instant and clock cpg init */
    if (clkhwfuncs.init(1)) {
        printf("mstp_init or clk_init failed \n");
        return -1;
    }

    {
#define CS_MACHINE_LEN (32 + 1)
        char cs_machine_str[CS_MACHINE_LEN] = { 0 };
        const char  *option = NULL;

        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        option = cs_machine_str;

        if (option) {
            if ( strstr( option, "S4") || strstr( option, "s4") ) {
                rcar_s4_test();
            } else if ( strstr( option, "V4H") || strstr( option, "v4h") ) {
                rcar_v4h_test();
            } else {
                printf("Unknown SoC\n");
            }
        }
    }

    clkhwfuncs.deinit();
    syschwfuncs.deinit();

    return 0;
}

