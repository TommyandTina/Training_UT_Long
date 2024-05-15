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
#include <errno.h>
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
    if(check == 1) printf("%s : Power off \n\n",name);
    else if(check == 0) printf("%s : Power on \n\n",name);
    else printf("%s Power : Error \n\n",name);
}


void rcar_v3u_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car V3U\n");
    printf("----------------------------------------\n");
/*######################### ISP test ###################################*/
    
    printf("############## ISP0/1 Clock&Power test ##############\n");
    
    /* ###Disable CPG ISP0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_V3U_ISP0) || clkhwfuncs.mstp_disable(CPG_MSTP_V3U_ISP1)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");
    
    clk_check_print("ISP0",CPG_MSTP_V3U_ISP0);
    clk_check_print("ISP1",CPG_MSTP_V3U_ISP1);
    printf("\n");

    /* ###Power off ISP0/1 ### */
    if(syschwfuncs.power_off(60)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("ISP0/1",60);
    
    /* ###Power on ISP0/1 ### */
    if(syschwfuncs.power_on(60)) printf("power_on failed \n");
    else printf("power_on succeed\n");
    
    power_check_print("ISP0/1",60);
    
    /* ###Enable CPG ISP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_V3U_ISP0) || clkhwfuncs.mstp_enable(CPG_MSTP_V3U_ISP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
    
    clk_check_print("ISP0",CPG_MSTP_V3U_ISP0);
    clk_check_print("ISP1",CPG_MSTP_V3U_ISP1);
    printf("\n");    
        
/*######################### A3IR domain power test ###################################*/

    printf("############## IMP0/1 Clock&Power OFF test ##############\n");

    /* ### Disable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_V3U_IMP0) || clkhwfuncs.mstp_disable(CPG_MSTP_V3U_IMP1)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");

    /* ### Power off IMP0/1 ### */
    if(syschwfuncs.power_off(34)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("IMP0/1",34);
    
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
    
    
/*######################### IMP test ###################################*/
    /* ### Power on IMP0/1 ### */
    if(syschwfuncs.power_on(34)) printf("power_on failed \n");
    else printf("power_on succeed\n");
    
    power_check_print("IMP0/1",34);

    /* ### Enable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_V3U_IMP0) || clkhwfuncs.mstp_enable(CPG_MSTP_V3U_IMP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
        
    clk_check_print("IMP0",CPG_MSTP_V3U_IMP0);
    clk_check_print("IMP1",CPG_MSTP_V3U_IMP1);
    printf("\n");

}


void rcar_v3h_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car V3H\n");
    printf("----------------------------------------\n");

    printf("PDMODE = %d\n", syschwfuncs.read_pdmode());
/*######################### ISP test ###################################*/
    
    printf("############## ISP0/1 Clock&Power test ##############\n");
    
    /* ###Disable CPG ISP0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_ISP0) || clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_ISP1)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");
    
    clk_check_print("ISP0",CPG_MSTP_GEN3_ISP0);
    clk_check_print("ISP1",CPG_MSTP_GEN3_ISP1);
    printf("\n");
    
    /* ###Enable CPG ISP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_ISP0) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_ISP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
    
    clk_check_print("ISP0",CPG_MSTP_GEN3_ISP0);
    clk_check_print("ISP1",CPG_MSTP_GEN3_ISP1);
    printf("\n");    
        
/*######################### A2IR0 domain power test ###################################*/

    /* Before power on/off A2IR0 power domian, need to clear Module stop IMP0/1 core.
     * See Figure 9.4(c) of HW for more detail. 
     */

    printf("############## IMP0/1 Clock&Power OFF test ##############\n");

    /* ### Enable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP0) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");

    clk_check_print("IMP0",CPG_MSTP_GEN3_IMP0);
    clk_check_print("IMP1",CPG_MSTP_GEN3_IMP1);
    printf("\n");

    /* ###Power off A3IR ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_A3IR)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("A3IR",SYSC_PDR_GEN3_A3IR);

    /* ###Power off A2IR0 ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_A2IR0)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("A2IR0",SYSC_PDR_GEN3_A2IR0);

    printf("############## IMP0/1 Clock&Power ON test ##############\n");

    /* ###Power on A3IR ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_A3IR)) printf("power_on failed \n");
    else printf("power_on succeed \n");

    power_check_print("A3IR",SYSC_PDR_GEN3_A3IR);

    /* ###Power on A2IR0 ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_A2IR0)) printf("power_on failed \n");
    else printf("power_on succeed \n");
    
    power_check_print("A2IR0",SYSC_PDR_GEN3_A2IR0);

    /* ### Enable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP0) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
        
    clk_check_print("IMP0",CPG_MSTP_GEN3_IMP0);
    clk_check_print("IMP1",CPG_MSTP_GEN3_IMP1);
    printf("\n");    
}


void rcar_v3m_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car V3M\n");
    printf("----------------------------------------\n");
    
/*######################### ISP test ###################################*/
    
    printf("############## ISP Clock&Power test ##############\n");
    
    /* ###Disable CPG ISP core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_ISP0)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");
    
    clk_check_print("ISP",CPG_MSTP_GEN3_ISP0);
    printf("\n");
    
    /* ###Enable CPG ISP core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_ISP0)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
    
    clk_check_print("ISP0",CPG_MSTP_GEN3_ISP0);
    printf("\n");    
        
/*######################### A2IR0 domain power test ###################################*/

    /* Before power on/off A2IR0 power domian, need to clear Module stop IMP0/1 core.
     * See Figure 9.4(c) of HW for more detail. 
     */

    printf("############## IMP0/1 Clock&Power OFF test ##############\n");

    /* ### Disable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMP0) || clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMP1)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");

    clk_check_print("IMP0",CPG_MSTP_GEN3_IMP0);
    clk_check_print("IMP1",CPG_MSTP_GEN3_IMP1);
    printf("\n");

    /* ### Enable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP0) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");

    clk_check_print("IMP0",CPG_MSTP_GEN3_IMP0);
    clk_check_print("IMP1",CPG_MSTP_GEN3_IMP1);
    printf("\n");

    /* ###Power off A3IR ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_A3IR)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("A3IR",SYSC_PDR_GEN3_A3IR);

    /* ###Power off A2IR0 ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_A2IR0)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("A2IR0",SYSC_PDR_GEN3_A2IR0);

    printf("############## IMP0/1 Clock&Power ON test ##############\n");

    /* ###Power on A3IR ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_A3IR)) printf("power_on failed \n");
    else printf("power_on succeed \n");

    power_check_print("A3IR",SYSC_PDR_GEN3_A3IR);
    /* ###Power on A2IR0 ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_A2IR0)) printf("power_on failed \n");
    else printf("power_on succeed \n");
    
    power_check_print("A2IR0",SYSC_PDR_GEN3_A2IR0);

    /* ### Enable CPG IMP0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP0) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP1)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
        
    clk_check_print("IMP0",CPG_MSTP_GEN3_IMP0);
    clk_check_print("IMP1",CPG_MSTP_GEN3_IMP1);
    printf("\n");
}


void rcar_d3_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car D3\n");
    printf("----------------------------------------\n");

/*######################### IMR0 test ###################################*/
    
    printf("############## IMR0 Clock&Power test ##############\n");
    
    /* ###Disable CPG IMR0 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMRLX40)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    printf("\n");
    
    /* ###Enable CPG IMR0 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMRLX40)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    printf("\n");
}


void rcar_e3_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car E3\n");
    printf("----------------------------------------\n");
    
/*######################### IMR0 test ###################################*/
    
    printf("############## IMR0 Clock&Power test ##############\n");
    
    /* ###Disable CPG IMR0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMRLX40)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    printf("\n");
    
    /* ###Enable CPG IMR0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMRLX40)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    printf("\n");    
        
/*######################### A3VC/3DGA domain power test ###################################*/

    printf("############## A3VC/3DGA domain power test ##############\n");
    
    /* ###Power off A3VC ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_A3VC)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("A3VC", SYSC_PDR_GEN3_A3VC);

    /* ###Power off 3DGA ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_3DGA)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("3DGA", SYSC_PDR_GEN3_3DGA);

    /* ###Power on A3VC ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_A3VC)) printf("power_on failed \n");
    else printf("power_on succeed \n");

    power_check_print("A3VC", SYSC_PDR_GEN3_A3VC);
    /* ###Power on 3DGA ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_3DGA)) printf("power_on failed \n");
    else printf("power_on succeed \n");
    
    power_check_print("3DGA", SYSC_PDR_GEN3_3DGA);
}


void rcar_h3_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car H3\n");
    printf("----------------------------------------\n");

/*######################### IMR0/1 test ###################################*/
    
    printf("############## IMR0/1 Clock&Power test ##############\n");
    
    /* ###Disable CPG IMR0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMRLX40) || clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMRLX41)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    clk_check_print("IMR1", CPG_MSTP_GEN3_IMRLX41);
    printf("\n");
    
    /* ###Enable CPG IMR0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMRLX40) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMRLX41)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    clk_check_print("IMR1", CPG_MSTP_GEN3_IMRLX41);
    printf("\n");    
        
/*######################### A3IR/3DGA domain power test ###################################*/
#define CPG_MSTP_IMP_H3  CPG_MSTP_REG_BIT_TO_ID(8, 24)
    printf("############## IMP Clock&Power OFF test ##############\n");

    /* ### Enable CPG IMP core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_IMP_H3)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");

    clk_check_print("IMP", CPG_MSTP_IMP_H3);
    printf("\n");

    /* ###Power off A3IR ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_A3IR)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("A3IR", SYSC_PDR_GEN3_A3IR);

    /* ###Power off 3DGA ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_3DGA)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("3DGA", SYSC_PDR_GEN3_3DGA);

    printf("############## IMP Clock&Power ON test ##############\n");

    /* ###Power on A3IR ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_A3IR)) printf("power_on failed \n");
    else printf("power_on succeed \n");

    power_check_print("A3IR", SYSC_PDR_GEN3_A3IR);
    /* ###Power on 3DGA ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_3DGA)) printf("power_on failed \n");
    else printf("power_on succeed \n");
    
    power_check_print("3DGA", SYSC_PDR_GEN3_3DGA);

    /* ### Enable CPG IMP core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_IMP_H3)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
        
    clk_check_print("IMP", CPG_MSTP_IMP_H3);
    printf("\n");
}

void rcar_m3n_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car M3N\n");
    printf("----------------------------------------\n");

/*######################### IMR0/1 test ###################################*/
    
    printf("############## IMR0/1 Clock&Power test ##############\n");
    
    /* ###Disable CPG IMR0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMRLX40) || clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMRLX41)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    clk_check_print("IMR1", CPG_MSTP_GEN3_IMRLX41);
    printf("\n");
    
    /* ###Enable CPG IMR0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMRLX40) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMRLX41)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    clk_check_print("IMR1", CPG_MSTP_GEN3_IMRLX41);
    printf("\n");    
        
/*######################### A3VP/3DGA domain power test ###################################*/

    printf("############## A3VP/3DGA domain power test ##############\n");

    /* ###Power off A3VP ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_A3VP)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("A3VP", SYSC_PDR_GEN3_A3VP);

    /* ###Power off 3DGA ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_3DGA)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("3DGA", SYSC_PDR_GEN3_3DGA);

    /* ###Power on A3VP ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_A3VP)) printf("power_on failed \n");
    else printf("power_on succeed \n");

    power_check_print("A3VP", SYSC_PDR_GEN3_A3VP);
    /* ###Power on 3DGA ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_3DGA)) printf("power_on failed \n");
    else printf("power_on succeed \n");
    
    power_check_print("3DGA", SYSC_PDR_GEN3_3DGA);
    printf("\n");
}


void rcar_m3_test()
{
    printf("----------------------------------------\n");
    printf("     CPG & SYSC testing on R-Car M3W\n");
    printf("----------------------------------------\n");

/*######################### IMR0/1 test ###################################*/
    
    printf("############## IMR0/1 Clock&Power test ##############\n");
    
    /* ###Disable CPG IMR0/1 core clock ###*/
    if (clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMRLX40) || clkhwfuncs.mstp_disable(CPG_MSTP_GEN3_IMRLX41)) printf("mstp_disable failed \n");
    else printf("mstp_disable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    clk_check_print("IMR1", CPG_MSTP_GEN3_IMRLX41);
    printf("\n");
    
    /* ###Enable CPG IMR0/1 core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMRLX40) || clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMRLX41)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
    
    clk_check_print("IMR0", CPG_MSTP_GEN3_IMRLX40);
    clk_check_print("IMR1", CPG_MSTP_GEN3_IMRLX41);
    printf("\n");    
        
/*######################### A3IR/3DGA domain power test ###################################*/

    printf("############## IMP Clock&Power OFF test ##############\n");

    /* ### Enable CPG IMP core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP3)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");

    clk_check_print("IMP", CPG_MSTP_GEN3_IMP3);
    printf("\n");

    /* ###Power off A3IR ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_A3IR)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("A3IR", SYSC_PDR_GEN3_A3IR);

    /* ###Power off 3DGA ### */
    if(syschwfuncs.power_off(SYSC_PDR_GEN3_3DGA)) printf("power_off failed \n");
    else printf("power_off succeed \n");
    
    power_check_print("3DGA", SYSC_PDR_GEN3_3DGA);

    printf("############## IMP Clock&Power ON test ##############\n");
    
    /* ### Enable CPG IMP core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP3)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
        
    clk_check_print("IMP", CPG_MSTP_GEN3_IMP3);
    printf("\n");
    
    /* ###Power on A3IR ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_A3IR)) printf("power_on failed \n");
    else printf("power_on succeed \n");

    power_check_print("A3IR", SYSC_PDR_GEN3_A3IR);

    /* ###Power on 3DGA ### */
    if(syschwfuncs.power_on(SYSC_PDR_GEN3_3DGA)) printf("power_on failed \n");
    else printf("power_on succeed \n");
    
    power_check_print("3DGA", SYSC_PDR_GEN3_3DGA);
    
    /* ### Enable CPG IMP core clock ###*/
    if (clkhwfuncs.mstp_enable(CPG_MSTP_GEN3_IMP3)) printf("mstp_enable failed \n");
    else printf("mstp_enable succeed \n");
        
    clk_check_print("IMP", CPG_MSTP_GEN3_IMP3);
    printf("\n");
}


int main(int argc, char **argv)
{
    /* Get functions table from CPG lib */
    if (cpg_mgr_getfuncs(&clkhwfuncs, sizeof(cpg_mgr_funcs_t))){
        printf("cpg_mgr_getfuncs failed \n");
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
            if ( strstr( option, "H3")  || strstr( option, "h3") ) {
                rcar_h3_test();
            } else if ( strstr( option, "M3N") || strstr( option, "m3n") ) {
                rcar_m3n_test();
            } else if ( strstr( option, "M3")  || strstr( option, "m3") ) {
                rcar_m3_test();
            } else if ( strstr( option, "E3")  || strstr( option, "e3") ) {
                rcar_e3_test();
            } else if ( strstr( option, "D3")  || strstr( option, "d3") ) {
                rcar_d3_test();
            } else if ( strstr( option, "V3M") || strstr( option, "v3m") ) {
                rcar_v3m_test();
            } else if ( strstr( option, "V3H") || strstr( option, "v3h") ) {
                rcar_v3h_test();
            } else if ( strstr( option, "V3U") || strstr( option, "v3u") ) {
                rcar_v3u_test();
            } else {
                printf("Unknown SoC\n");
            }
        }
    }

    clkhwfuncs.deinit();
    syschwfuncs.deinit();

    return 0;
}

