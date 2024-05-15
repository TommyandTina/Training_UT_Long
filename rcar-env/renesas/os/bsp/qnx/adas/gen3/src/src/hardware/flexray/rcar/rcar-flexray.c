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

#include <sys/mman.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <atomic.h>
#include <sys/procmgr.h>
#include <hw/hwinfo_rcar.h>
#include "rcar-flexray.h"
#include <hw/config.h>

#define		RCAR_CONFIG_GPIO_FLEXRAY 1
#define		RCAR_V3H_FLEXRAY		 1
#define		RCAR_V3M_FLEXRAY		 2
#define		RCAR_V3U_FLEXRAY		 3

#define		RCAR_V3H_FLEXRAY_CLK	 (1<<5)
#define		RCAR_V3M_FLEXRAY_CLK	 (1<<5)
#define		RCAR_V3U_FLEXRAY_CLK	 (1<<13)

uint8_t product_id = 0;
uintptr_t rcar_cpg_vbase = 0;
uint32_t rcar_pfc_base = 0;
uint32_t rcar_cpg_base = 0;
uint32_t rcar_cpg_cpgwpr = RCAR_V3U_CPG_CPGWPR;
uint32_t rcar_pfc_size = 0;
uint32_t rcar_cpg_mstpcr = 0;
uint32_t rcar_cpg_mstpsr = 0;
uint32_t rcar_cpg_srcr = 0;
uint32_t rcar_cpg_srstclr = 0;
uint32_t rcar_gpio_pmmr = 0;
uint32_t rcar_gpio_pmer = 0;
uint32_t rcar_gpio_gpsr0 = 0;
uint32_t rcar_gpio_gpsr1 = 0;
uint32_t rcar_gpio_gpsr2 = 0;
uint32_t rcar_gpio_gpsr3 = 0;
uint32_t rcar_gpio_ipsr1 = 0;
uint32_t rcar_gpio_ipsr2 = 0;
uint32_t rcar_gpio_ipsr8 = 0;
uint32_t bitmask = RCAR_V3U_FLEXRAY_CLK;

struct fr_cluster_param cluster_param = {
	.gColdStartAttempts                = 31,
	.gdActionPointOffset               = 4,
	.gdCASRxLowMax                     = 99,
	.gdDynamicSlotIdlePhase            = 1,
	.gdMinislot                        = 63,
	.gdMinislotActionPointOffset       = 4,
	.gdStaticSlot                      = 300,
	//.gdSymbolWindow				   =
	.gdTSSTransmitter                  = 15,
	.gdWakeupSymbolRxIdle              = 59,
	.gdWakeupSymbolRxLow               = 55,
	.gdWakeupSymbolRxWindow            = 301,
	.gdWakeupSymbolTxIdle              = 180,
	.gdWakeupSymbolTxLow               = 60,
	.gListenNoise                      = 15,
	.gMacroPerCycle                    = 11805,
	.gMaxWithoutClockCorrectionFatal   = 2,
	.gMaxWithoutClockCorrectionPassive = 1,
	.gNumberOfMinislots                = 100,
	.gNumberOfStaticSlots              = 16,
	.gOffsetCorrectionStart            = 11121,
	.gPayloadLengthStatic              = 127,
	.gSyncNodeMax                      = 2,
	// Protocol related 
	.gdNIT                             = 685,
	.gdSampleClockPeriod               = 0,
	.gNetworkManagementVectorLength    = 12,
};
struct fr_node_param node_param = {
	// Protocol relevant
	.pAllowHaltDueToClock              = false,
	.pAllowPassiveToActive             = 31,
	.pChannels                         = FR_CHANNEL_A,
	.pdAcceptedStartupRange            = 1875,
	.pClusterDriftDamping              = 5,
	.pDecodingCorrection               = 72,
	.pDelayCompensationA               = 0,
	.pDelayCompensationB               = 0,
	.pdListenTimeOut                   = 802404,
	.pdMaxDrift                        = 1202,
	.pExternOffsetCorrection           = 0,
	.pExternRateCorrection             = 0,
	//.pKeySlotID;
	.pKeySlotUsedForStartup            = true,
	.pKeySlotUsedForSync               = true,
	.pLatestTx                         = 94,
	.pMacroInitialOffsetA              = 6,
	.pMacroInitialOffsetB              = 6,
	.pMicroInitialOffsetA              = 6,
	.pMicroInitialOffsetB              = 6,
	.pMicroPerCycle                    = 472200,  
	.pOffsetCorrectionOut              = 134,
	.pRateCorrectionOut                = 1202,
	.pSingleSlotEnabled                = false,
	.pWakeupChannel                    = FR_WAKEUP_CH_A,
	.pWakeupPattern                    = 2,
	//Protocol related 
	.pSamplesPerMicrotick              = 2,
};

/*
n = 16*300 + 0 + 100*63
n = 4800 + 6300 = 11100 
gOffsetCorrectionStart = 11100 + 21= 11121
NIT = 11120
gMacroPerCycle = 11805
gdNIT = 11805 - 11120 -1
*/
/*
	n = gNumberOfStaticSlots • gdStaticSlot + dynamic segment offset + gNumberOfMinislots • gdMinislot
	gdActionPointOffset <= gdMinislotActionPointOffset --> dynamic segment offset = 0

	n = 16*300 + 0 + 100*63
	n = 4800 + 6300
	n = 11100
	MT = gMacroPerCycle = 11805  11121-11100=21
	                           <------------->  
	|--------------------------|-------------|-------|
  MT|    Static/Dynamic Segment|Symbol Window|NIT    |
   0|                          |11100         |11120 |11804
               gOffsetCorrectionStart = 11121
	|<-------------------------------------->|	
							11805	
	|<---------------------------------------------->|
*/
/*	sample clock set to 80 MHz
	Baud rate=10 MBit/s
	gdSampleClockPeriod 	= 12.5ns
	pSamplesPerMicrotick	= 2
	pdMicrotick = pSamplesPerMicrotick*gdSampleClockPeriod
				= gdSampleClockPeriod=12.5*2=25ns (1 μT = 25 ns)
	gdBit 	= cSamplesPerBit * gdSampleClockPeriod
			= 8*12.5=100ns (must be identical for all nodes of the cluster)
	pMicroPerMacroNom(Number of microticks per nominal macrotick) 
	pMicroPerMacroNom[μT/MT]= gdMacrotick[μs/MT] / pdMicrotick[μs/μT]
							= pMicroPerCycle[μT] / gMacroPerCycle[MT]
							= 472200/11805 = 40					
	gdMacrotick = 25*40 = 1000ns = 1μs	
	
	if(gdActionPointOffset > gdMinislotActionPointOffset)                        )
		dynamic segment offset = gdActionPointOffset - gdMinislotActionPointOffset 
	else gdActionPointOffset <= gdMinislotActionPointOffset
		dynamic segment offset = 0
	gMacroPerCycle[MT] = gdStaticSlot[MT] * gNumberOfStaticSlots + adActionPointDifference[MT] +
		gdMinislot[MT] * gNumberOfMinislots + gdSymbolWindow[MT] + gdNIT[MT]
adActionPointDifference[MT] -->dynamic segment offset
if gdActionPointOffset <= gdMinislotActionPointOffset
	adActionPointDifference[MT] = 0 
if gNumberOfMinislots = 0
	adActionPointDifference[MT] = 0
else	
	adActionPointDifference = gdActionPointOffset - gdMinislotActionPointOffset		
*/

enum data_err_index {
	FR_EIDX,
	FR_IAE,
	FR_OAE,
	FR_FAE,
	FR_MAE,
	FR_AEA
};
enum msg_buf_err_index {
	FR_AMR,
	FR_ATBF1,
	FR_ATBF2,
	FR_FMBD,
	FR_MFMB,
	FR_FMB
};

fr_timer timer0_cfg = {
    FR_TIMER0, 		
    FR_TIMER_ABSOLUTE, 		
    FR_TIMER_CONTINUOUS, 	    
    8000, 				
    79	
};

fr_timer timer1_cfg = {
    FR_TIMER1, 		
    FR_TIMER_RELATIVE, 		
    FR_TIMER_SINGLE_SHOT, 	    
    8000, 				
    0
};

fr_timer timer2_cfg = {
    FR_TIMER2, 		
    FR_TIMER_ABSOLUTE, 		
    FR_TIMER_SINGLE_SHOT, 	    
    8000, 			
    79	
};

fr_timer* timers_cfg[]={
	&timer0_cfg,	
    //&timer1_cfg,   
    //&timer2_cfg,   
    NULL
};


fr_stopwatch_t stop_watch = {
	FR_STOWATCH_TRIGGER,
	FR_STOWATCH_CONTINUOUS,
	FR_STOWATCH_EDGE_RISING
};

/** 
 * @brief Get the state of operation of the Commmunication Controller Protocol Operation Control
 * @param regs: Pointer to FlexRay module registers.
 * @param pocState: The state of operation of the Protocol Operation Control.
 * @return None.
 */
void fr_get_pocs_state(fr_regs_t *regs,fr_poc_state *pocState)
{
    uint8_t pocs = FR_REG_BITS(regs->cc_stat.frccsv, FR_CCSV_POCS_L, FR_CCSV_POCS_H) & FR_CCSV_POCS_MASK ;

	switch (pocs){
		case 0x00:
			*pocState = FR_STATE_DEFAULT_CONFIG;
			break;
		case 0x01:
			*pocState = FR_STATE_READY;
			break;
		case 0x02:
			*pocState = FR_STATE_NORMAL_ACTIVE;
			break;
		case 0x03:
			*pocState = FR_STATE_NORMAL_PASSIVE;
			break;
		case 0x04:
			*pocState = FR_STATE_HALT;
			break;
		case 0x0f:
			*pocState = FR_STATE_CONFIG;
			break;
		case 0x10:
			*pocState = FR_STATE_WAKEUP_STANDBY;
			break;
		case 0x11:
			*pocState = FR_STATE_WAKEUP_LISTEN;
			break;
		case 0x12:
			*pocState = FR_STATE_WAKEUP_SEND;
			break;
		case 0x13:
			*pocState = FR_STATE_WAKEUP_DETECT;
			break;
		case 0x20:
			*pocState = FR_STATE_STARTUP_PREPARE;
			break;
		case 0x21:
			*pocState = FR_STATE_COLDSTART_LISTEN;
			break;
		case 0x22:
			*pocState = FR_STATE_COLDSTART_COLLISION_RESOLUTION;
			break;
		case 0x23:
			*pocState = FR_STATE_COLDSTART_CONSISTENCY_CHECK;
			break;
		case 0x24:
			*pocState = FR_STATE_COLDSTART_GAP;
			break;
		case 0x25:
			*pocState = FR_STATE_COLDSTART_JOIN;
			break;
		case 0x26:
			*pocState = FR_STATE_INTEGRATION_COLDSTART_CHECK;
			break;
		case 0x27:
			*pocState = FR_STATE_INTEGRATION_LISTEN;
			break;
		case 0x28:
			*pocState = FR_STATE_INTEGRATION_CONSISTENCY_CHECK;
			break;
		case 0x29:
			*pocState = FR_STATE_INITIALIZE_SCHEDULE;
			break;
		case 0x2A:
			*pocState = FR_STATE_ABORT_STARTUP;
			break;
		case 0x2B:
			*pocState = FR_STATE_STARTUP_SUCCESS;
			break;
		default:
			*pocState = FR_STATE_UNSPEC;
	}
	return ;
}

/** 
 * @brief Send Controller Host Interface command to change the state of operation of the Commmunication Controller Protocol Operation Control.
 * @param regs: Pointer to FlexRay module registers.
 * @param cmd: Controller Host Interface Command Vector.
 * @return 0:success, -1:error
 */
int fr_change_poc_state(fr_regs_t *regs, fr_poc_cmd cmd)
{
	uint32_t stat = 0;
	int i = FR_WAIT_CYCLE;
	
	if(EBUSY==fr_check_state_of_bit(regs->cc_ctrl.frsucc1,FR_SUCC1_PBSY,FR_BIT_CLEAR)){
		fr_slogf(FR_SLOG_DEBUG1,"fr_check_state_of_bit: frsucc1=0x%x return EBUSY\n",in32(regs->cc_ctrl.frsucc1));
		return -1;
	}
	stat = in32(regs->cc_ctrl.frsucc1);
	stat &= ~FR_SUCC1_CMD_MASK;
	stat |= cmd;
	write_bits(regs->cc_ctrl.frsucc1, FR_SUCC1_CMD_L, FR_SUCC1_CMD_H, stat);
	
	if(EBUSY==fr_check_state_of_bit(regs->cc_ctrl.frsucc1,FR_SUCC1_PBSY,FR_BIT_CLEAR)){
		fr_slogf(FR_SLOG_DEBUG1,"fr_check_state_of_bit: frsucc1=0x%x return EBUSY\n",in32(regs->cc_ctrl.frsucc1));
		return -1;
	}
	
	for(i=0;i<FR_WAIT_CYCLE;i++){
		stat = in32(regs->cc_ctrl.frsucc1);
		if ((stat & FR_SUCC1_CMD_MASK)==cmd){
			break;
		}
		if ((stat & FR_SUCC1_CMD_MASK)==FR_CMD_NOT_ACCEPTED){
			fr_slogf(FR_SLOG_DEBUG1,"%s: cmd=%d, not accepted\n",__func__,cmd);
			return -1;
		}
		fr_delay(1);
	}
	
    return 0;
}

int get_machine_name(void)
{
    const char *options = NULL;
#define CS_MACHINE_LEN (32 + 1)
    char cs_machine_str[CS_MACHINE_LEN] = { 0 };

    if (options == NULL) {
        confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN);
        options = cs_machine_str;
    }
    product_id = 0;
	bitmask = RCAR_V3U_FLEXRAY_CLK;	
	rcar_pfc_base = RCAR_V3U_GPIO_DM0_BASE;
	rcar_cpg_base = RCAR_V3U_CPG_DM0_BASE;
	rcar_pfc_size = 0x10000;
	rcar_cpg_mstpcr = RCAR_V3U_CPG_MSTPCR5;
	rcar_cpg_mstpsr = RCAR_V3U_CPG_MSTPSR5;
	rcar_cpg_srcr = RCAR_V3U_CPG_SRCR5;
	rcar_cpg_srstclr = RCAR_V3U_CPG_SRSTCLR5;
	rcar_gpio_gpsr2 = RCAR_V3U_GPIO_GP2;
	rcar_gpio_gpsr3 = RCAR_V3U_GPIO_GP3;
	rcar_gpio_ipsr2 = 0;
	rcar_gpio_ipsr8 = 0;
	rcar_gpio_pmmr = RCAR_V3U_GPIO_PMMR;
	rcar_gpio_pmer = RCAR_V3U_GPIO_PMMER;
    if (options) {
        if ( strstr( options, "V3M") || strstr( options, "v3m") ) {
            product_id = RCAR_V3M_FLEXRAY;
			bitmask = RCAR_V3M_FLEXRAY_CLK;
			rcar_cpg_base = RCAR_GEN3_CPG_BASE;
			rcar_cpg_cpgwpr = RCAR_GEN3_CPG_CPGWPR;
			rcar_pfc_base = RCAR_GEN3_PFC_BASE;
			rcar_pfc_size = RCAR_GEN3_PFC_SIZE;
			rcar_cpg_mstpcr = RCAR_GEN3_CPG_SMSTPCR5;
			rcar_cpg_mstpsr = RCAR_GEN3_CPG_MSTPSR5;
			rcar_cpg_srcr = RCAR_GEN3_CPG_SRCR5;
			rcar_cpg_srstclr = RCAR_GEN3_CPG_SRSTCLR5;
			rcar_gpio_gpsr2 = RCAR_V3U_GPIO_GP2;
			rcar_gpio_gpsr3 = RCAR_V3U_GPIO_GP3;
			rcar_gpio_ipsr1 = RCAR_GEN3_PFC_IPSR1;
			rcar_gpio_pmmr = RCAR_V3U_GPIO_PMMR;
			rcar_gpio_pmer = RCAR_V3U_GPIO_PMMER;
        } else if ( strstr( options, "V3H") || strstr( options, "v3h") ) {
            product_id = RCAR_V3H_FLEXRAY;
			bitmask = RCAR_V3H_FLEXRAY_CLK;
			rcar_cpg_base = RCAR_GEN3_CPG_BASE;
			rcar_cpg_cpgwpr = RCAR_GEN3_CPG_CPGWPR;
			rcar_pfc_base = RCAR_GEN3_PFC_BASE;
			rcar_pfc_size = RCAR_GEN3_PFC_SIZE;
			rcar_cpg_mstpcr = RCAR_GEN3_CPG_SMSTPCR5;
			rcar_cpg_mstpsr = RCAR_GEN3_CPG_MSTPSR5;
			rcar_cpg_srcr = RCAR_GEN3_CPG_SRCR5;
			rcar_cpg_srstclr = RCAR_GEN3_CPG_SRSTCLR5;
			rcar_gpio_gpsr0 = RCAR_GEN3_PFC_GPSR0;
			rcar_gpio_gpsr1 = RCAR_GEN3_PFC_GPSR1;
			rcar_gpio_ipsr2 = RCAR_GEN3_PFC_IPSR2;
			rcar_gpio_ipsr8 = RCAR_GEN3_PFC_IPSR8;	
			rcar_gpio_pmmr = RCAR_V3U_GPIO_PMMR;
			rcar_gpio_pmer = RCAR_V3U_GPIO_PMMER;			
        } else if ( strstr( options, "V3U") || strstr( options, "v3u") ) {
            product_id = RCAR_V3U_FLEXRAY;
			bitmask = RCAR_V3U_FLEXRAY_CLK;
			rcar_cpg_base = RCAR_V3U_CPG_DM0_BASE;
			rcar_cpg_cpgwpr = RCAR_V3U_CPG_CPGWPR;
			rcar_pfc_base = RCAR_V3U_GPIO_DM0_BASE;
			rcar_pfc_size = 0x10000;
			rcar_cpg_mstpcr = RCAR_V3U_CPG_MSTPCR5;
			rcar_cpg_mstpsr = RCAR_V3U_CPG_MSTPSR5;
			rcar_cpg_srcr = RCAR_V3U_CPG_SRCR5;
			rcar_cpg_srstclr = RCAR_V3U_CPG_SRSTCLR5;
			rcar_gpio_gpsr2 = RCAR_V3U_GPIO_GP2;
			rcar_gpio_gpsr3 = RCAR_V3U_GPIO_GP3;
			rcar_gpio_pmmr = RCAR_V3U_GPIO_PMMR;
			rcar_gpio_pmer = RCAR_V3U_GPIO_PMMER;
        } else {
			fr_slogf(FR_SLOG_DEBUG1,"%s : Unknown SoC", __func__);
            return -ENOMEM;       
        }
    }   
    return EOK;
}

void cpg_config(uintptr_t base, int reg, uint32_t val)
{
    out32(base + rcar_cpg_cpgwpr, ~val);
    out32(base + reg, val);
}

void pfc_reg_config(uintptr_t base, uint32_t reg, uint32_t val)
{
    out32(base + rcar_gpio_pmer, 1);
    out32(base + rcar_gpio_pmmr, ~val);
    out32(base + reg, val);
}

void cpg_module_reset(uintptr_t   base)
{
    uint32_t tmp;
    
    int timeout = 1024;

	//Flexray
    /* Reset module */
    tmp  = in32(base + rcar_cpg_srcr);
    tmp |= bitmask;
    cpg_config(base,rcar_cpg_srcr,tmp);

    /* Wait for at least one cycle of the RCLK clock (32 kHz) */
    fr_delay(35);

    /* Release reset module */
    cpg_config(base, rcar_cpg_srstclr, bitmask);
    
    fr_delay(2);

    while(--timeout >= 0) {
        if(!(in32(base + rcar_cpg_srcr) & bitmask))
            break;
    }	
}

int fr_pfc_v3h_config(uintptr_t addr)
{
    uint32_t    tmp;
	
	/* GPSR0[20:19] = 0x3 -> DU_VSYNC/FXR_TXENB#,
	*                        DU_HSYNC/FXR_TXENA#
	*/
	tmp  = in32(addr + rcar_gpio_gpsr0);
    tmp |= 0x00180000;
    pfc_reg_config(addr,rcar_gpio_gpsr0, tmp);
	
	/*
	* IPSR2[15:12] = 4 -> FXR_TXENA
	* IPSR2[19:16] = 4 -> FXR_TXENB
	*/
	tmp  = in32(addr + rcar_gpio_ipsr2);
    tmp &= 0xFFF00FFF;
	tmp |= 0x00044000;
    pfc_reg_config(addr, rcar_gpio_ipsr2, tmp);
	/* GPSR1[24:21] = 0x1F ->RXDB_EXTFXR,
	*                        FXR_TXDB,
	*                        RXDA_EXTFXR,
	*                        FXR_TXDA
	*/
	tmp  = in32(addr + rcar_gpio_gpsr1);
    tmp |= 0x01E00000;
	pfc_reg_config(addr ,rcar_gpio_gpsr2, tmp);
	/* IPSR8[19:4] = 0x1111->RXDB_EXTFXR,
	*                        FXR_TXDB,
	*                        RXDA_EXTFXR,
	*                        FXR_TXDA
	*/
	tmp  = in32(addr + rcar_gpio_ipsr8);
	tmp &= ~(0x000FFFF0);
	tmp |= (0x00011110);
    pfc_reg_config(addr,rcar_gpio_ipsr8, tmp);
	return 0;
}

int fr_pfc_v3m_config(uintptr_t addr)
{
    uint32_t    tmp;
	
	/* GPSR0[16:15] = 0x3 -> DU_DB6/FXR_TXENB#,
	*                        DU_DB5/FXR_TXENA#
	*/
	tmp  = in32(addr + rcar_gpio_gpsr0);
    tmp |= 0x00011000;
    pfc_reg_config(addr,rcar_gpio_gpsr0, tmp);
	/*
	* IPSR1[31:28] = 4 -> FXR_TXENA
	*/
	tmp  = in32(addr + rcar_gpio_ipsr1);
    tmp &= 0x0FFFFFFF;
	tmp |= 0x40000000;
	/*
	* IPSR2[3:0] = 4 -> FXR_TXENB
	*/
	tmp  = in32(addr + rcar_gpio_ipsr2);
    tmp &= 0xFFFFFFFF;
	tmp |= 0x00000004;	
    pfc_reg_config(addr, rcar_gpio_ipsr1, tmp);
	/* GPSR1[24:21] = 0x1F ->RXDB_EXTFXR,
	*                        FXR_TXDB,
	*                        RXDA_EXTFXR,
	*                        FXR_TXDA
	*/
	tmp  = in32(addr + rcar_gpio_gpsr1);
    tmp |= 0x01E00000;
	pfc_reg_config(addr ,rcar_gpio_gpsr2, tmp);
	/* IPSR8[15:0] = 0x1111 ->RXDB_EXTFXR,
	*                        FXR_TXDB,
	*                        RXDA_EXTFXR,
	*                        FXR_TXDA
	*/
	tmp  = in32(addr + rcar_gpio_ipsr8);
	tmp &= ~(0x0000FFFF);
	tmp |= (0x00001111);
    pfc_reg_config(addr,rcar_gpio_ipsr8, tmp);
	return 0;
}

int fr_pfc_v3u_config(uintptr_t addr)
{
    uint32_t    tmp;
	
	tmp  = in32(addr + rcar_gpio_gpsr2 + RCAR_V3U_GPIO_GPSR);
    tmp &= ~0x001F0000;
    pfc_reg_config(addr + rcar_gpio_gpsr2,RCAR_V3U_GPIO_GPSR, tmp);
	
	/* IP2SR2[3:0]   = 0 -> FXR_TXDA
	* IP2SR2[7:4]   = 0 -> RXDA_EXTFXR
	* IP2SR2[11:8]  = 0 -> FXR_TXDB
	* IP2SR2[15:12] = 0 -> RXDB_EXTFXR
	* IP2SR2[19:16] = 0 -> CLK_EXTFXR
	*/
	tmp  = in32(addr + rcar_gpio_gpsr2 + RCAR_V3U_GPIO_IP2SR);
    tmp &= 0xFFF00000;
    pfc_reg_config(addr + rcar_gpio_gpsr2, RCAR_V3U_GPIO_IP2SR, tmp);
	/* GPSR2[20:16] = 0x1F -> CLK_EXTFXR,
	*                        RXDB_EXTFXR,
	*                        FXR_TXDB,
	*                        RXDA_EXTFXR,
	*                        FXR_TXDA
	*/
	tmp  = in32(addr + rcar_gpio_gpsr2 + RCAR_V3U_GPIO_GPSR);
    tmp |= 0x001F0000;
    pfc_reg_config(addr + rcar_gpio_gpsr2,RCAR_V3U_GPIO_GPSR, tmp);
	tmp  = in32(addr + rcar_gpio_gpsr3 + RCAR_V3U_GPIO_GPSR);
	tmp &= ~(0x5<<11);
    pfc_reg_config(addr + rcar_gpio_gpsr3,RCAR_V3U_GPIO_GPSR, tmp);
	/* 
	* IP1SR3[15:12] = 3 -> FXR_TXENA
	* IP1SR3[19:16] = 3 -> FXR_TXENB
	*/
	tmp  = in32(addr + rcar_gpio_gpsr3 + RCAR_V3U_GPIO_IP1SR);
    tmp &= 0xFFF00FFF;
	tmp |= (5<<12) | (5<<16);
    pfc_reg_config(addr + rcar_gpio_gpsr3, RCAR_V3U_GPIO_IP1SR, tmp);	
	
	/* GPSR3[12:11] = 0x3 -> CANFD5_RX-> FXR_TXENA
	*                      	CANFD5_TX-> FXR_TXENB
	*/
	tmp  = in32(addr + rcar_gpio_gpsr3 + RCAR_V3U_GPIO_GPSR);
	tmp |= (0x5<<11);
    pfc_reg_config(addr + rcar_gpio_gpsr3,RCAR_V3U_GPIO_GPSR, tmp);
	return 0;
}

void pfc_write32(uintptr_t addr, uint32_t data)
{
	out32(addr + rcar_gpio_pmmr, ~data);
	out32(addr, data);
}

int fr_enable_clock(bool enable, uintptr_t base)
{
    uint32_t    tmp;
	int i =0;
	
    tmp  = in32(base + rcar_cpg_mstpcr);
	if(enable) {
		tmp &= ~(bitmask);
	} else{
		tmp |= bitmask;
	}
    cpg_config(base, rcar_cpg_mstpcr, tmp);

    /* Check if change is effected */
    for (i = 10000; i > 0; --i) {
        tmp = in32(base + rcar_cpg_mstpsr);
		if(enable) {
			if (!(tmp & bitmask)) {
				break;
			}
		}else {
			if (tmp & bitmask) {
				break;
			}
		}
    }
	if(enable) {
		cpg_module_reset(base);
	}else{
		munmap_device_io(base,0x3000);
	}
    return 0;
}

void fr_clear_all_flags(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
	// Clear error interrupts
    out32(regs->intr.freir, FR_EIR_CLEAR_ALL);
	out32(regs->intr.frsir, FR_SIR_CLEAR_ALL);
	out32(regs->cc_stat.fracs, 0x00001F1F);  
}

int fr_irq_setup(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;

    // Interrupt assigned to FlexRay interrupt to line 0
	out32(regs->intr.freils,0);
    // Interrupt assigned to FlexRay interrupt to line 0
	out32(regs->intr.frsils, 0);
    // Enable error interrupts
    out32(regs->intr.freies, FR_EIR_ENABLE); 
    // Enable status interrupts
    out32(regs->intr.frsies, FR_FRSIES_ENABLE);  
    // FlexRay interrupt 0 and 1 enabled
    out32(regs->intr.frile, FR_ILE_EINT01);
	dev->iid = InterruptAttach(dev->irq, fr_isr, dev, sizeof(fr_dev_t), _NTO_INTR_FLAGS_TRK_MSK);
    return 0;
}

void fr_set_baud_rate(fr_dev_t *dev,fr_regs_t *regs)
{
	if (dev == NULL){
        fr_slogf(FR_SLOG_DEBUG1,"%s: Null input pointer", __func__);
        return ;
    }

	switch(dev->baud_rate){
		case 10:
			write_bits(regs->cc_ctrl.frprtc1, FR_PRTC1_BRP_L, FR_PRTC1_BRP_H, FR_BAUD_RATE_10);
			break;
		case 5:
			write_bits(regs->cc_ctrl.frprtc1, FR_PRTC1_BRP_L, FR_PRTC1_BRP_H, FR_BAUD_RATE_5);
			break;
		case 2:
			write_bits(regs->cc_ctrl.frprtc1, FR_PRTC1_BRP_L, FR_PRTC1_BRP_H, FR_BAUD_RATE_2_5);
			break;
		default:
			write_bits(regs->cc_ctrl.frprtc1, FR_PRTC1_BRP_L, FR_PRTC1_BRP_H, FR_BAUD_RATE_10);
			break;
	}
}

int fr_check_msg_buf(uint8_t nmsg, uint16_t dataLength)
{
	/* 53.3.13 Message RAM 
		- Header Partition:
			Each message buffer has a header section of four 32 bit words
		- Data Partition:
			Flexible storage of data sections with different length. Some maximum values are:
			30 message buffers with 254 byte data section each
			Or 56 message buffers with 128 byte data section each
			Or 128 message buffers with 48 byte data section each
			Restriction: header partition + data partition may not occupy more than 2048 32-bit words.
	*/
	//Check message buffers <= 128
	if(nmsg > FR_MAX_BUF){
		fr_slogf(FR_SLOG_DEBUG1,"%s: nmsg=%d > %d\n",__func__,(int)nmsg,(int)FR_MAX_BUF);
		return -1;
	}
	//Check header partition + data partition may not occupy more than 2048 32-bit words
	if((nmsg*FR_HEADER_LEN + dataLength) > (FR_MBF_MAX_MEM*4)){
		fr_slogf(FR_SLOG_DEBUG1,"header partition + data partition may not occupy more than 2048 32-bit words\n");
		return -1;
	}
	// Check data partition
	if(nmsg <= FR_MBF_NUM) {
		if(dataLength > FR_MAX_DATA1) {
			fr_slogf(FR_SLOG_DEBUG1,"%s: dataLength=%d > FR_MAX_DATA1=%d\n",__func__,(int)dataLength,(int)FR_MAX_DATA1);
			return -1;
		}else {
			return 0;
		}
	}else if((nmsg <= FR_MBF_56) &&  (nmsg > FR_MBF_NUM)){
		if(dataLength > FR_MAX_DATA2) {
			fr_slogf(FR_SLOG_DEBUG1,"%s: dataLength=%d > FR_MAX_DATA2=%d\n",__func__,(int)dataLength,(int)FR_MAX_DATA2);
			return -1;
		}else {
			return 0;
		}
	}else if(nmsg > FR_MBF_56){
		if(dataLength > FR_MAX_DATA3) {
			fr_slogf(FR_SLOG_DEBUG1,"%s: dataLength=%d > FR_MAX_DATA3=%d\n",__func__,(int)dataLength,(int)FR_MAX_DATA3);
			return -1;
		}else {
			return 0;
		}
	}
	return 0;
}

void fr_stopwatch_init(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
	
	out32(regs->timer.frstpw1,0);
	
	if(dev->intr_timer){
		set_bit(regs->timer.frstpw1,FR_STPW1_EINT0);
		set_bit(regs->timer.frstpw1,FR_STPW1_EINT1);
	}else{
		clear_bit(regs->timer.frstpw1,FR_STPW1_EINT0);
		clear_bit(regs->timer.frstpw1,FR_STPW1_EINT1);
	}
	if(stop_watch.edge==FR_STOWATCH_EDGE_RISING){
		set_bit(regs->timer.frstpw1,FR_STPW1_EDGE);
	}else{
		clear_bit(regs->timer.frstpw1,FR_STPW1_EDGE);
	}
	if(stop_watch.mode==FR_STOWATCH_CONTINUOUS){
		set_bit(regs->timer.frstpw1,FR_STPW1_SWMS);
	}else{
		clear_bit(regs->timer.frstpw1,FR_STPW1_SWMS);
	}
}

void fr_timers_irq_enable(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;

	if(dev->intr_timer) {
		set_bit(regs->intr.frsies, FR_SIES_SWEE);
		set_bit(regs->intr.frsies, FR_SIES_TI0E);
		set_bit(regs->intr.frsies, FR_SIES_TI1E);	
		set_bit(regs->ope.froc, FR_OC_T0IE);
		set_bit(regs->ope.froc, FR_OC_T1IE);

	}else{
		// Timer 2 and 0 interrupt disable
		clear_bit(regs->intr.frsies, FR_SIES_SWEE);
		clear_bit(regs->intr.frsies, FR_SIES_TI0E);
		clear_bit(regs->intr.frsies, FR_SIES_TI1E);
		clear_bit(regs->ope.froc, FR_OC_T0IE);
		clear_bit(regs->ope.froc, FR_OC_T1IE);
	}
}

void fr_timers_init(fr_regs_t *regs,fr_timer**ptimer)
{
	uint8_t i = 0; 
	i = 0; 
	fr_timer* pfrtimer = NULL;
	while (ptimer[i] != NULL){
		pfrtimer = ptimer[i];
		if(FR_TIMER_ABSOLUTE==pfrtimer->type){
			if (pfrtimer->id == FR_TIMER0){
				//Timer 0 halted
				clear_bit(regs->timer.frt0c,FR_FRTC_TRC);
				write_bits(regs->timer.frt0c,FR_FRTC_TCC_L,FR_FRTC_TCC_H,(pfrtimer->cycleCode & FR_TOCC_MASK));
				write_bits(regs->timer.frt0c,FR_FRTC_TMO_L,FR_FRTC_TMO_H,(pfrtimer->macrotickOffset & FR_TMO_MASK));
				// The TICCR reg. will be configured later
				if(pfrtimer->mode == FR_TIMER_CONTINUOUS){
					set_bit(regs->timer.frt0c,FR_FRTC_TMS);					
				}else{
					clear_bit(regs->timer.frt0c,FR_FRTC_TMS);
				}
			}else if(pfrtimer->id == FR_TIMER2){
				//Timer 1 halted
				clear_bit(regs->timer.frt2c,FR_FRTC_TRC);
				write_bits(regs->timer.frt2c,FR_FRTC_TCC_L,FR_FRTC_TCC_H,(pfrtimer->cycleCode & FR_TOCC_MASK));
				write_bits(regs->timer.frt2c,FR_FRTC_TMO_L,FR_FRTC_TMO_H,(pfrtimer->macrotickOffset>>16) & FR_TMO_MASK);
				// The TICCR reg. will be configured later
				if(pfrtimer->mode == FR_TIMER_CONTINUOUS){
					set_bit(regs->timer.frt2c,FR_FRTC_TMS);					
				}else{
					clear_bit(regs->timer.frt2c,FR_FRTC_TMS);
				}
			}
		}
		if(FR_TIMER_RELATIVE==pfrtimer->type){
			clear_bit(regs->timer.frt1c,FR_FRTC_TRC);
			write_bits(regs->timer.frt1c,FR_FRTC_TMC_L,FR_FRTC_TMC_H,(pfrtimer->macrotickOffset & FR_TMC_MASK));
			// The TICCR reg. will be configured later
			if(pfrtimer->mode == FR_TIMER_CONTINUOUS){
				set_bit(regs->timer.frt1c,FR_FRTC_TMS);					
			}else{
				clear_bit(regs->timer.frt1c,FR_FRTC_TMS);
			}
		}
		i++;
	}
}

int fr_start_timer(fr_dev_t *dev, uint32_t id) 
{
	fr_regs_t *regs = &dev->regs;
	fr_poc_state pocState = 0;
	
	fr_get_pocs_state(regs,&pocState);
	if((pocState != FR_STATE_NORMAL_ACTIVE) && (pocState != FR_STATE_NORMAL_PASSIVE)){
		return EIO;
	}
	if(id == FR_TIMER0){
		set_bit(regs->timer.frt0c,FR_FRTC_TRC);
	}
	if(id == FR_TIMER1){
		set_bit(regs->timer.frt1c,FR_FRTC_TRC);
	}
	if(id == FR_TIMER2){
		set_bit(regs->timer.frt2c,FR_FRTC_TRC);
	}
	
	dev->timer_start = true;
	
	return 0;
}

void fr_stop_timer(fr_dev_t *dev, uint32_t id) 
{
	fr_regs_t *regs = &dev->regs;
	if(id == FR_TIMER0){
		clear_bit(regs->timer.frt0c,FR_FRTC_TRC);
	}
	if(id == FR_TIMER1){
		clear_bit(regs->timer.frt1c,FR_FRTC_TRC);
	}
	if(id == FR_TIMER2){
		clear_bit(regs->timer.frt2c,FR_FRTC_TRC);
	}
	dev->timer_coid = -1;
	dev->timer_start = false;
}

void fr_start_stopwatch(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
	if(stop_watch.type==FR_STOWATCH_TRIGGER){
		set_bit(regs->timer.frstpw1,FR_STPW1_ESWT);
	}else{
		clear_bit(regs->timer.frstpw1,FR_STPW1_ESWT);
	}
	if(stop_watch.type==FR_STOWATCH_SOFTWARE){
		set_bit(regs->timer.frstpw1,FR_STPW1_SSWT);
	}else{
		clear_bit(regs->timer.frstpw1,FR_STPW1_SSWT);
	}
}

void fr_stop_stopwatch(fr_dev_t *dev) 
{
	fr_regs_t *regs = &dev->regs;
	
	clear_bit(regs->timer.frstpw1,FR_STPW1_ESWT);
	clear_bit(regs->timer.frstpw1,FR_STPW1_SSWT);
}

int fr_set_poc_ready(fr_regs_t *regs)
{
    // Wait POC busy
	if(EBUSY==fr_check_state_of_bit(regs->cc_ctrl.frsucc1,FR_SUCC1_PBSY,FR_BIT_CLEAR)) {
		fr_slogf(FR_SLOG_DEBUG1,"%s: Wait POC busy failed\n", __func__);
	}
	write_bits(regs->spec.frlck, FR_LCK_CLK_L, FR_LCK_CLK_H, FR_LCK_CLK_FIRST_WRITE);
    write_bits(regs->spec.frlck, FR_LCK_CLK_L, FR_LCK_CLK_H, FR_LCK_CLK_SECOND_WRITE);
 
	if(fr_change_poc_state(regs, FR_CMD_READY)) {
		return -1;
	}
	return 0;
}

int fr_wakeup_cluster(fr_regs_t *regs)
{
	if(fr_change_poc_state(regs, FR_CMD_WAKEUP)) {
		fr_slogf(FR_SLOG_DEBUG1,"%s failed", __func__);
		return -1;
	}
	return 0;
}

int fr_allow_cold_start(fr_regs_t *regs)
{
	/* For a coldstart node, the flags must be configured with FRSUCC1.TXSY = "1" and
		FRSUCC1.TXST = "1".
	*/
	if((FR_REG_BIT(regs->cc_ctrl.frsucc1, FR_SUCC1_TXSY) == FR_BIT_SET) && 
		(FR_REG_BIT(regs->cc_ctrl.frsucc1, FR_SUCC1_TXST) == FR_BIT_SET)){
			if(fr_change_poc_state(regs, FR_CMD_ALLOW_COLDSTART)) {
				fr_slogf(FR_SLOG_DEBUG1, "%s failed", __func__);
				return -1;
			}else {
				fr_slogf(FR_SLOG_DEBUG1, "%s done", __func__);
				return 0;
			}
	}
	return -1;
}
int fr_start_communication(fr_dev_t *dev) 
{
	uint8_t retry = 0;
	fr_regs_t *regs = &dev->regs;
	retry = 0;
	do{
		if(0==fr_wait_pocs_state(regs,FR_STATE_READY)){
			break;	
		}
		fr_delay(1);
	}while(retry++ < FR_WAIT_CYCLE);	
	
	if(fr_allow_cold_start(regs)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: send FR_CMD_ALLOW_COLDSTART failed\n", __func__);
		return -1;
	}
	
	retry = 0;
	do{
		if((in32(regs->cc_stat.frccsv) & (1<<FR_CCSV_CSI)) == 0x00000000){
			break;
		}
	}while(retry++ < FR_WAIT_CYCLE);
	if(retry >= FR_WAIT_CYCLE) {
		fr_slogf(FR_SLOG_DEBUG1,"%s: wait Cold starting of node enabled failed\n", __func__);
		return -1;
	}
	if(fr_change_poc_state(regs, FR_CMD_RUN)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: send CMD_RUN failed", __func__);
		return -1;
	}
	return 0;
}

/** 
 * @brief Wait FlexRay enter into NORMAL_ACTIVE state.
 * @param dev: Pointer to fr_dev_t structure.
 * @return 0: success, -1: error
 */
int fr_startup(fr_dev_t *dev)
{
	fr_poc_state pocState = 0;
	uint8_t retry = 0 ;
	fr_regs_t *regs = &dev->regs;
	
	if(fr_set_poc_ready(regs)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_READY failed", __func__);
	}else{
		fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_READY done", __func__);
	}
	if(fr_wait_pocs_state(regs, FR_STATE_READY)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: wait pocs state(FR_STATE_READY) failed", __func__);
		return -1;
	}else{
		fr_slogf(FR_SLOG_DEBUG1,"%s: wait pocs state(FR_STATE_READY) done", __func__);
	}
	
	for(;;) {
		fr_get_pocs_state(regs,&pocState);
		if(pocState != FR_STATE_WAKEUP_LISTEN){
			if(fr_change_poc_state(regs, FR_CMD_WAKEUP)){
				fr_slogf(FR_SLOG_DEBUG1,"%s: fr_change_poc_state: FR_CMD_WAKEUP failed\n", __func__);
			}
		}else {
			break;
		}
		/* Fix Me: Need 100ms or more? */
		fr_delay(1);
		if(retry++ >= FR_WAIT_CYCLE){
			fr_slogf(FR_SLOG_DEBUG1,"%s: timeout to wait state: FR_STATE_WAKEUP_LISTEN\n", __func__);
			return -1;
		}
	}
	//Go to POC state STARTUP
	if(fr_start_communication(dev)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: fr_start_communication failed\n",__func__);
		return -1;
	}else{
		fr_slogf(FR_SLOG_DEBUG3,"%s: fr_start_communication done\n",__func__);
	}
	retry = 0;
	do{
		fr_get_pocs_state(regs,&pocState);
		if(pocState==FR_STATE_NORMAL_ACTIVE){
			fr_slogf(FR_SLOG_DEBUG1,"pocState=FR_STATE_NORMAL_ACTIVE\n");
			break;
		}
		fr_delay(1);
	}while(1);		
	if(pocState==FR_STATE_HALT){
		fr_slogf(FR_SLOG_DEBUG1,"pocState=FR_STATE_HALT\n");
		return -1;
	}
	fprintf(stdout,"Startup completed successfully and Flexray entered NORMAL_ACTIVE state\n");
	return 0;
}

/** 
 * @brief Clear all interrupt.
 * @param regs: Pointer to FlexRay module registers.
 * @return None
 */
void fr_clear_interrupt(fr_regs_t *regs)
{
    // Enable error interrupts
    out32(regs->intr.freies, 0); 
    out32(regs->intr.freier, FR_EIER_CLEAR_ALL);
    // Enable status interrupts
    out32(regs->intr.frsies, 0); 
	out32(regs->intr.frsier, FR_SIER_CLEAR_ALL);
    // Enable both the interrupt lines
    out32(regs->intr.frile, 0);
}

/** 
 * @brief Clear message RAM.
 * @param regs: Pointer to FlexRay module registers.
 * @return 0:success, -1:error
 */
int fr_clear_ram(fr_regs_t *regs)
{
    uint8_t i = 0;

	if (FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) == FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Input transfer is enable", __func__);
		return -1;
	}
	if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) == FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Output transfer is enable", __func__);
		return -1;
	}
	for (i = 0; i < 4; i++) {
		
		if ((in32(regs->msg_buf_stat.frtxrq[i]) != 0) || (in32(regs->msg_buf_stat.frndat[i]) != 0)) {
			fr_slogf(FR_SLOG_DEBUG1,"%s: There are outstanding tranfers in the message handler", __func__);
			return -1;
		}
	}
	if(fr_change_poc_state(regs, FR_CMD_CLEAR_RAMS)) {
		fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_CLEAR_RAMS failed", __func__);
		return -1;
	}
	//wait until FRMHDS.CRAM = "0"
	fr_check_state_of_bit(regs->msg_buf_stat.frmhds, FR_MHDS_CRAM,FR_BIT_CLEAR);

    return 0;
}

/** 
 * @brief Initialize FlexRay.
 * @param regs: Pointer to fr_dev_t structure.
 * @return 0:success, -1:error
 */
int fr_node_init(fr_dev_t *dev)
{
	fr_poc_state pocState;
	fr_regs_t *regs = &dev->regs;
		
	if (FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) == FR_BIT_SET){
		clear_bit(regs->data_xfer_ctrl.fritc, FR_ITC_ITE);
	}
	if (FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) == FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Input transfer is operating", __func__);
	}
	if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) == FR_BIT_SET){
		clear_bit(regs->data_xfer_ctrl.frotc, FR_OTC_OTE);
	}
	if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) == FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Output transfer is operating", __func__);
	}
	
	fr_clear_interrupt(regs);
	fr_get_pocs_state(regs,&pocState);

	if (pocState != FR_STATE_CONFIG){
		/* Set FlexRay to a state,
		 * 1. Send freeze command  
		 * 2. Wait for halt state  
		 * 3. Send config command
		 * 4. Wait for default config state
		 * 5. Send config command
		 * 6. Wait for config state
		*/		
		if(fr_change_poc_state(regs, FR_CMD_FREEZE)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_FREEZE failed", __func__);
			return -1;
		}else{
			fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_FREEZE done", __func__);
		}
		if(fr_wait_pocs_state(regs, FR_STATE_HALT)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: wait pocs state(STATE_HALT) failed", __func__);
			return -1;
		}else{
			fr_slogf(FR_SLOG_DEBUG1,"%s: wait pocs state(STATE_HALT) done", __func__);
		}
		if(fr_change_poc_state(regs, FR_CMD_CONFIG)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_CONFIG failed", __func__);
			return -1;
		}else{
			fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_CONFIG done", __func__);
		}
		if(fr_wait_pocs_state(regs, FR_STATE_DEFAULT_CONFIG)){
			fr_slogf(FR_SLOG_DEBUG1,"%s: wait pocs state(FR_STATE_DEFAULT_CONFIG) failed", __func__);
			return -1;
		}else{
			fr_slogf(FR_SLOG_DEBUG1,"%s: wait pocs state(FR_STATE_DEFAULT_CONFIG) done", __func__);
		}
		//wait until FRMHDS.CRAM = "0"
		if(fr_clear_ram(regs)) {
			fr_slogf(FR_SLOG_DEBUG1,"%s: wait for cleared CRAM flag failed\n",__func__);
			return -1;
		}else{
			fr_slogf(FR_SLOG_DEBUG1,"%s: fr_clear_ram done", __func__);
		}
		if(fr_change_poc_state(regs, FR_CMD_CONFIG)) {
			fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_CONFIG failed", __func__);
			return -1;
		}else{
			fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_CONFIG done", __func__);
		}
		if(fr_wait_pocs_state(regs, FR_STATE_CONFIG)){
			fr_slogf(FR_SLOG_DEBUG1, "%s: wait pocs state(FR_STATE_CONFIG) failed", __func__);
			return -1;
		}else{
			fr_slogf(FR_SLOG_DEBUG1,"%s: wait pocs state(FR_STATE_CONFIG) done", __func__);
		}
	}

	fr_timers_init(regs,(fr_timer**)(timers_cfg));
	fr_stopwatch_init(dev);
	if(strstr(dev->system_parameter,".conf")){
		dev->gPayloadLengthStatic = dev->cluster_param.gPayloadLengthStatic;
		dev->gNumberOfStaticSlots = dev->cluster_param.gPayloadLengthStatic;
		if(fr_set_cluster_param(regs,&dev->cluster_param)) {
			fr_slogf(FR_SLOG_DEBUG1,"%s:fr_set_cluster_param failed",__func__);
			return -1;
		}
		if(fr_set_node_param(regs,&dev->node_param)) {
			fr_slogf(FR_SLOG_DEBUG1,"%s:fr_set_node_param failed",__func__);
			return -1;
		}
	}else {
		//Use default config
		fr_slogf(FR_SLOG_DEBUG3,"%s:Use default config for cluster and node",__func__);
		dev->gPayloadLengthStatic = cluster_param.gPayloadLengthStatic;
		dev->gNumberOfStaticSlots = cluster_param.gPayloadLengthStatic;
		if(fr_set_cluster_param(regs,&cluster_param)) {
			fr_slogf(FR_SLOG_DEBUG1,"%s:fr_set_cluster_param failed",__func__);
			return -1;
		}
		if(fr_set_node_param(regs,&node_param)) {
			fr_slogf(FR_SLOG_DEBUG1,"%s:fr_set_node_param failed",__func__);
			return -1;
		}
		memcpy(&dev->cluster_param,&cluster_param,sizeof(fr_cluster_param_t));
		memcpy(&dev->node_param,&node_param,sizeof(fr_node_param_t));
	}
	
	fr_set_baud_rate(dev,regs);
    return 0;
}
uint16_t get_header_crc(fr_msg_ram_t *msg)
{	
	uint32_t vCrcInit = 0x1A;			// initialization vector of header CRC for both channels
	uint32_t vCrcPolynomial = 0x385;	// hexadecimal representation of the header CRC polynomial

	uint32_t vCrcReg = vCrcInit; 		// Initialize the CRC register
	uint32_t vNextBit, vCrcNext;
	uint32_t i = 0;
	uint32_t inputData = 0;
	
	if(msg->sync){
		inputData  = (1<< 19);
	}
	if(msg->startup){
		inputData  |= (1<< 18);
	}

	inputData |= ((msg->frameId & FR_FID_MAX) <<  7)|((msg->payloadLength/2)& FR_WRHS2_PLC_MAX);
	
	for(i=0; i<20; i++){
		// vNextBit is from MSB to LSB of input data
		vNextBit = ((inputData & 0x80000)>>19);
		inputData <<= 1;
		inputData &= 0xFFFFE;	// mask 20 bit

		// determine if the CRC polynomial has to be applied by taking
		// the exclusive OR of the most significant bit of the CRC register
		// and the next bit to be fed into the register
		vCrcNext = vNextBit ^ ((vCrcReg&0x400)>>10);

		// Shift the CRC register left by one bit
		vCrcReg <<= 1;
		vCrcReg &= 0x7FE;	// mask 11 bit

		// Apply the CRC polynomial if necessary
		if(vCrcNext){
			vCrcReg ^= vCrcPolynomial;
		}

		vNextBit >>= 1;
	}

	return (vCrcReg & 0x7FF);	
}
		
/** 
 * @brief Write data to registers FRWRDS.
 * @param dev: Pointer to struct fr_dev_t.
 * @param payloadLength: Length of data.
 * @param payload: Pointer to data.
 * @return none
 */
void fr_set_data_buffer(fr_dev_t *dev,uint8_t payloadLength, uint8_t *payload)
{
	fr_regs_t *regs = &dev->regs;
	uint32_t	data; 
	int i = 0;
	// Wait if transfer between IBF Shadow and Message RAM in progress
	fr_wait_inputbuffer_busy(dev);
	for(i=0; i<payloadLength; i+=4){
		data = 0x00000000;
		if((i+0)<payloadLength) data |= (uint32_t)payload[i+0] << 0;
		if((i+1)<payloadLength) data |= (uint32_t)payload[i+1] << 8;
		if((i+2)<payloadLength) data |= (uint32_t)payload[i+2] << 16;
		if((i+3)<payloadLength) data |= (uint32_t)payload[i+3] << 24;
		out32(regs->in_buf.frwrds[i>>2],data);
	}
}
/** 
 * @brief Setup header of message RAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @param msg: Pointer to struct fr_msg_ram_t.
 * @return 0:success, EINVAL, ETIMEDOUT :error
 */
int fr_set_header_message_buffer(fr_dev_t *dev,fr_msg_ram_t *msg)
{
	uint16_t  dataPointer = 0;	//Pointer to the data partition of the Message RAM.
	fr_regs_t *regs = &dev->regs;
	uint32_t frwrhs1 = 0;
	uint32_t frwrhs2 = 0;
	uint32_t frwrhs3 = 0;
	/*	53.2.13.1 (8)
		While FRITS.ITS=1, there should be no read or write access to the address area ERAY+h'0400 to ERAY+h'05FF
		and thereshould be no CLEAR_RAMS command applied to FRSUCC1.CMD register
	*/
	if (FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) == FR_BIT_SET){
		return EINVAL;
	}
	//Enables the receive / transmit interrupt for the corresponding message buffer
	if((msg->type==FR_MSGBUF_TYPE_RX)||(msg->type==FR_MSGBUF_TYPE_TX) ){
		frwrhs1 |= (1<<FR_WRHS1_MBI); 
	}
	//Configure transmission mode
	if (msg->mode==FR_SINGLE_MODE){
		frwrhs1 |= (1<<FR_WRHS1_TXM);
	} else {
		frwrhs1 &= ~(1<<FR_WRHS1_TXM);
	}

	// Configure direction
	if (msg->type==FR_MSGBUF_TYPE_TX){
		// Configure payload preamble indicator transmit
		if (msg->ppi){
			frwrhs1 |= (1<<FR_WRHS1_PPIT);
		} else {
			frwrhs1 &= ~(1<<FR_WRHS1_PPIT);
		}
		frwrhs1 |= (1<<FR_WRHS1_CFG);	
	}else{
		frwrhs1 &= ~(1<<FR_WRHS1_CFG);
	}			
	// Configure channel control/filter
	if(msg->channel==FR_CHANNEL_AB) { 	
		frwrhs1 |= (FR_CHANNEL_AB<<FR_WRHS1_CH_L);
	}else if(msg->channel==FR_CHANNEL_A){
		frwrhs1 |= (FR_CHANNEL_A<<FR_WRHS1_CH_L);
	}else if(msg->channel==FR_CHANNEL_B){
		frwrhs1 |= (FR_CHANNEL_B<<FR_WRHS1_CH_L);
	}else{
		frwrhs1 &= ~(FR_CHANNEL_AB<<FR_WRHS1_CH_L);
	}		
	// Configure cycle count filter
	frwrhs1 |= (msg->cycleCount<<FR_WRHS1_CYC_L);
	// Configure frame filter
	if ((msg->frameId <= dev->gNumberOfStaticSlots) && (msg->payloadLength > 2*dev->gPayloadLengthStatic)) {
		return EINVAL;
	}
	if (msg->frameId > FR_FID_MAX){
		msg->frameId = FR_FID_MAX;
	} 
	frwrhs1 |= (msg->frameId<<FR_WRHS1_FID_L);
	
	// Configure payload length of the message
	if (msg->payloadLength > FR_MAX_PAYLOAD){
		msg->payloadLength = FR_MAX_PAYLOAD;
	} 
	frwrhs2 |= (((msg->payloadLength + 1)/2)<<FR_WRHS2_PLC_L);
	
	if(msg->type==FR_MSGBUF_TYPE_TX){
		// Configure msg crc
		frwrhs2 |= (get_header_crc(msg)<<FR_WRHS2_CRC_L); 	
	}
	// Configure data pointer 
	dataPointer = (FR_MBF_MAX_MEM -1 ) - ((msg->payloadLength + 3)/4)*(msg->idx);
	if (dataPointer < (msg->idx + 3)*4 ){
		return EINVAL;
	}	
	// Wait if transfer between IBF Shadow and Message RAM in progress
	if(fr_wait_inputbuffer_busy(dev)){
		return ETIMEDOUT;
	}
	out32(regs->in_buf.frwrhs1,frwrhs1);
	out32(regs->in_buf.frwrhs2,frwrhs2);
	frwrhs3 = dataPointer & FR_WRHS3_DP_MASK;
	out32(regs->in_buf.frwrhs3, frwrhs3);
	
	return 0;
}

/** 
 * @brief Store to message RAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @param msg: Pointer to struct fr_msg_ram_t.
 * @return none
 */
void fr_store_to_message_ram(fr_dev_t *dev,fr_msg_ram_t *msg)
{
	fr_regs_t *regs = &dev->regs;
	uint32_t fribcm = 0;
	
	
	// Update header section
	if(msg->cmdMask & FR_CMD_MASK_HEADER){
		fribcm |= (1<<FR_IBCM_LHSH);		
	} else {
		fribcm &= ~(1<<FR_IBCM_LHSH);
	}
	if(msg->cmdMask & FR_CMD_MASK_DATA){
		fribcm |= (1<<FR_IBCM_LDSH);		
	} else {
		fribcm &= ~(1<<FR_IBCM_LDSH);
	}
	if(msg->cmdMask & FR_CMD_MASK_REQUEST){
		fribcm |= (1<<FR_IBCM_STXRH);
	} else {
		fribcm &= ~(1<<FR_IBCM_STXRH);
	}

	// Write command mask
	out32(regs->in_buf.fribcm,fribcm);
	// Demand data transfer to target message buffer: write FRIBCR.IBRH
	out32(regs->in_buf.fribcr,msg->idx);
	return ;
}
/** 
 * @brief Setup message RAM.
 * @param dev: Pointer to struct fr_dev_t.
 * @param cfg: Pointer to struct fr_msg_buffer_control_t.
 * @return 0: success, -1: error
 */
int fr_config_msg_ram(fr_dev_t *dev,fr_msg_buffer_control_t *cfg)
{
	uint8_t numberMessage = 0;
	fr_regs_t *regs = &dev->regs;
	fr_poc_state pocState;
	
	fr_get_pocs_state(regs,&pocState);

	if (pocState != FR_STATE_CONFIG){
		if(fr_change_poc_state(regs,FR_CMD_CONFIG)) {
			fr_slogf(FR_SLOG_DEBUG1,"%s: Send cmd=FR_CMD_CONFIG failed at line %d", __func__,__LINE__);
			return -1;
		}
	}
	if(fr_clear_ram(regs)) {
		fr_slogf(FR_SLOG_DEBUG1,"%s: fr_clear_ram failed", __func__);
		return -1;
	}
	/*	53.2.13.2 (11)
		While FROTS.OTS=1, the user should not change the ERAY message RAM configuration by writing to the FRMRC
		register.
	*/
	if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) == FR_BIT_SET){
		fr_slogf(FR_SLOG_DEBUG1,"%s: Output transfer is operating", __func__);
		return -1;
	}
	if(cfg->type & FR_MSGBUF_UNLOCK){	
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_SEC_L, FR_MRC_SEC_H,0);
	}else if(cfg->type & FR_MSGBUF_LOCK_FIFO_STATIC){	
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_SEC_L, FR_MRC_SEC_H,1);
	}else if(cfg->type & FR_MSGBUF_LOCK){	
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_SEC_L, FR_MRC_SEC_H,2);
	}else if(cfg->type & FR_MSGBUF_LOCK_STATIC_TX){	
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_SEC_L, FR_MRC_SEC_H,3);
	}else{
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_SEC_L, FR_MRC_SEC_H,0);
	}
	numberMessage = cfg->staticBuffers + cfg->dynamicBuffers + cfg->fifoBuffers;
	/*	Last Configured Buffer
	*	0…127: Number of message buffers is FRMRF.LCB + 1
	*	>=128: No message buffer configured
	*/
	dev->msg_lcb = numberMessage -1;
	write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_LCB_L, FR_MRC_LCB_H,dev->msg_lcb);
	
	/* First Buffer of FIFO
	 * 0 = All message buffers assigned to the FIFO
	 * 1…127 = Message buffers from FRMRC.FFB to FRMRC.LCB assigned to the FIFO
	 * 128 = No message buffer assigned to the FIFO
	 * FRMRC.FFB=cfg->nMsgBuffer-cfg->dynamicBuffers-cfg->staticBuffers
	*/
	if(0==cfg->fifoBuffers){
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_FFB_L,FR_MRC_FFB_H,FR_MAX_BUF);	
	} else if(FR_MAX_BUF==cfg->fifoBuffers){
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_FFB_L,FR_MRC_FFB_H,0);	
	} else {
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_FFB_L,FR_MRC_FFB_H,(numberMessage -1) - (cfg->fifoBuffers -1));
	}
	/* First Dynamic Buffer
	* FRMRC.FDB=cfg->staticBuffers
	*/
	if(0==cfg->dynamicBuffers){
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_FDB_L, FR_MRC_FDB_H,FR_MAX_BUF);
	} else {
		write_bits(regs->msg_buf_ctrl.frmrc, FR_MRC_FDB_L, FR_MRC_FDB_H,cfg->staticBuffers);
	}
	
	//Receive FIFO is configured
	if ((cfg->fifoBuffers != 0) || (cfg->fifoBuffers != FR_MAX_BUF)){
		/* Channel Filter
		 *00 = receive on both channels
		 *01 = receive only on channel B
		 *10 = receive only on channel A
		 *11 = no reception
		*/
		write_bits(regs->msg_buf_ctrl.frfrf, FR_FRF_CH_L, FR_FRF_CH_H,cfg->channelFilter);
		/* Set Frame ID Filter
		 * FRFRF.FID=cfg->frameIdRejected
		*/
		write_bits(regs->msg_buf_ctrl.frfrf, FR_FRF_FID_L, FR_FRF_FID_H,cfg->frameIdRejected);
		/* Set Cycle Counter Filter
		 * FRFRF.CYF=cfg->cycleCountFiler
		*/
		write_bits(regs->msg_buf_ctrl.frfrf, FR_FRF_CYF_L, FR_FRF_CYF_H,cfg->cycleCount);	
		/* where FIFO is used in static segment or is not used	
		 * FRFRF.RSS=1	
		*/
		if(cfg->type & FR_MSGCTL_NO_STATIC){
			set_bit(regs->msg_buf_ctrl.frfrf, FR_FRF_RSS);
		} else {
			clear_bit(regs->msg_buf_ctrl.frfrf, FR_FRF_RSS);
		}
		/* Set reject null frames
		 * FRFRF.RNF=1
		*/		
		if(cfg->type & FR_MSGCTL_NO_NULL_FRAME){
			set_bit(regs->msg_buf_ctrl.frfrf, FR_FRF_RNF);
		} else {
			clear_bit(regs->msg_buf_ctrl.frfrf, FR_FRF_RNF);
		}
		/* Set Frame ID Filter
		 * FRFRFM.MFID=cfg->frameIdFilter
		*/
		write_bits(regs->msg_buf_ctrl.frfrfm, FR_FRFM_MFID_L, FR_FRFM_MFID_H,cfg->frameIdFilter);	
		/* Set FIFO Critical Level
		 * FRFCL.CL=cfg->fifoThreshold
		*/
		write_bits(regs->msg_buf_ctrl.frfcl, FR_FCL_CL_L, FR_FCL_CL_H,cfg->fifoThreshold);        
	}
	
	return 0;
}

/** 
 * @brief Call function fr_config_msg_ram to setup message RAM.
 * @param ctp: Pointer to struct resmgr_context_t.
 * @param msg: Pointer to struct io_devctl_t.
 * @param ocb: Pointer to struct iofunc_ocb_t.
 * @return 0:success, !=0 :error
 */
int rcar_get_error(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
    fr_dev_t *dev = (fr_dev_t *)ocb->attr;
    fr_devctl_error_t *pmsg = NULL;

    if (msg->i.nbytes < sizeof(*pmsg)){
        fr_slogf(FR_SLOG_DEBUG1, "%s: FR_DEVCTL_ERROR failed",__func__);
        return EINVAL;
    }
    pmsg = (fr_devctl_error_t*)_DEVCTL_DATA(msg->i);
	pmsg->drvr1 = dev->fr_err.drvr1;
	pmsg->drvr2 = dev->fr_err.drvr2;
	pmsg->drvr2 = dev->fr_err.drvr2;
	pmsg->drvr4 = dev->fr_err.drvr4;
	
	msg->o.nbytes = sizeof(fr_devctl_error_t);
    return(_RESMGR_PTR(ctp, &msg->o, sizeof(msg->o) + msg->o.nbytes));
}

/** 
 * @brief FlexRay Module enable.
 * @param dev: Pointer to struct fr_dev_t.
 * @return 0:success, EINVAL:error
 */

int fr_enable_module(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
	/*	53.3.1.1 FlexRay Module enable.
		Figure 53.3 FlexRay enable flow. 
	*/	

	// Set FROC.OEP to 0	
    clear_bit(regs->ope.froc, FR_OC_OEP);
	//Set FROC.OE to 1
    set_bit(regs->ope.froc, FR_OC_OE);

    // Wait until FROS.OS = 1
	if(EBUSY==fr_check_state_of_bit(regs->ope.fros, FR_OS_OS,FR_BIT_SET)) {
		fr_slogf(FR_SLOG_DEBUG1,"%s wait for FROS.OS=1 failed\n",__func__);
	}
	//Set FROC.OEP to 1
    set_bit(regs->ope.froc, FR_OC_OEP);

    // Wait until FRSUCC1.PBSY = 0
	if(EBUSY==fr_check_state_of_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_PBSY,FR_BIT_CLEAR)){
		fr_slogf(FR_SLOG_DEBUG1,"%s wait for FRSUCC1.PBSY = 0 failed\n",__func__);
	}

    return 0;
}

void fr_disable_module(fr_dev_t *dev)
{
	fr_regs_t *regs = &dev->regs;
	fr_stop_timer(dev,FR_TIMER0);
	fr_stop_timer(dev,FR_TIMER1);
	fr_stop_timer(dev,FR_TIMER2);
	fr_stop_stopwatch(dev);
    // Suspend FlexRay communication
    // Suspend output transfer if it was activated
    if (FR_REG_BIT(regs->data_xfer_stat.frots, FR_OTS_OTS) != FR_BIT_CLEAR){
        clear_bit(regs->data_xfer_ctrl.frotc, FR_OTC_OTE);
		fr_check_state_of_bit(regs->data_xfer_stat.frots, FR_OTS_OTS,FR_BIT_CLEAR); 
    }
    // Suspend input transfer if it was activated
    if (FR_REG_BIT(regs->data_xfer_stat.frits, FR_ITS_ITS) != FR_BIT_CLEAR){
        clear_bit(regs->data_xfer_ctrl.fritc, FR_ITC_ITE);
		fr_check_state_of_bit(regs->data_xfer_stat.frits, FR_ITS_ITS,FR_BIT_CLEAR); 
    }
    clear_bit(regs->ope.froc, FR_OC_OEP);
    clear_bit(regs->ope.froc, FR_OC_OE);

    // Wait for the module to be in the operation state
	fr_check_state_of_bit(regs->ope.fros, FR_OS_OS,FR_BIT_CLEAR);
    set_bit(regs->ope.froc, FR_OC_OEP);
}

int fr_query_hwi_device(fr_dev_t *dev)
{
	int status = EOK;
    unsigned hwi_off = 0;
    hwi_tag *tag = NULL;
   
	hwi_off = hwi_find_device(RCAR_HWI_FLEXRAY,0);
	if(hwi_off != HWI_NULL_OFF){
		tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
		if(tag) {
			dev->phybase = tag->location.base;
			dev->len = tag->location.len;
		}else{
			status = ENOSYS;
		}
		tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, 0);
		if(tag){
			dev->irq = tag->irq.vector;
		}else{
			status = ENOSYS;
		}
		if( (dev->phybase==0) ||(dev->len==0) ||(dev->irq==0)){
			status = ENOSYS;
		}
	}else{
		status = ENOSYS;
	}
	return status;
}

int map_regs(fr_dev_t *dev)
{
    uint32_t    i;
	fr_regs_t *regs = &dev->regs;
    // Initialize base address
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: ThreadCtl failed\n",__func__);
		return 0;
    }
	
	if ((dev->fr_reg_base = mmap_device_io(dev->len, dev->phybase)) == MAP_DEVICE_FAILED){
	   return EINVAL;
	}

    /* Operation */
    regs->ope.froc = dev->fr_reg_base + FROC;
    regs->ope.fros = dev->fr_reg_base + FROS;
    
    /* Special */
    regs->spec.frlck = dev->fr_reg_base + FRLCK;

    /* Interrupt */
    regs->intr.freir  = dev->fr_reg_base + FREIR;
    regs->intr.frsir  = dev->fr_reg_base + FRSIR;
    regs->intr.freils = dev->fr_reg_base + FREILS;
    regs->intr.frsils = dev->fr_reg_base + FRSILS;
    regs->intr.freies = dev->fr_reg_base + FREIES;
    regs->intr.freier = dev->fr_reg_base + FREIER;
    regs->intr.frsies = dev->fr_reg_base + FRSIES;
    regs->intr.frsier = dev->fr_reg_base + FRSIER;
    regs->intr.frile  = dev->fr_reg_base + FRILE;

    /* Timer */
    regs->timer.frt0c = dev->fr_reg_base + FRT0C;
    regs->timer.frt1c = dev->fr_reg_base + FRT1C;
    regs->timer.frt2c = dev->fr_reg_base + FRT2C;
    regs->timer.frstpw1 = dev->fr_reg_base + FRSTPW1;
    regs->timer.frstpw2 = dev->fr_reg_base + FRSTPW2;

    /* CC Control */
    regs->cc_ctrl.frsucc1 = dev->fr_reg_base + FRSUCC1;
    regs->cc_ctrl.frsucc2 = dev->fr_reg_base + FRSUCC2;
    regs->cc_ctrl.frsucc3 = dev->fr_reg_base + FRSUCC3;
    regs->cc_ctrl.frnemc = dev->fr_reg_base + FRNEMC;
    regs->cc_ctrl.frprtc1 = dev->fr_reg_base + FRPRTC1;
    regs->cc_ctrl.frprtc2 = dev->fr_reg_base + FRPRTC2;
    regs->cc_ctrl.frmhdc = dev->fr_reg_base + FRMHDC;
    regs->cc_ctrl.frgtuc1 = dev->fr_reg_base + FRGTUC1;
    regs->cc_ctrl.frgtuc2 = dev->fr_reg_base + FRGTUC2;
    regs->cc_ctrl.frgtuc3 = dev->fr_reg_base + FRGTUC3;
    regs->cc_ctrl.frgtuc4 = dev->fr_reg_base + FRGTUC4;
    regs->cc_ctrl.frgtuc5 = dev->fr_reg_base + FRGTUC5;
    regs->cc_ctrl.frgtuc6 = dev->fr_reg_base + FRGTUC6;
    regs->cc_ctrl.frgtuc7 = dev->fr_reg_base + FRGTUC7;
    regs->cc_ctrl.frgtuc8 = dev->fr_reg_base + FRGTUC8;
    regs->cc_ctrl.frgtuc9 = dev->fr_reg_base + FRGTUC9;
    regs->cc_ctrl.frgtuc10 = dev->fr_reg_base + FRGTUC10;
    regs->cc_ctrl.frgtuc11 = dev->fr_reg_base + FRGTUC11;
    /* CC Status */
    regs->cc_stat.frccsv = dev->fr_reg_base + FRCCSV;
    regs->cc_stat.frccev = dev->fr_reg_base + FRCCEV;
    regs->cc_stat.frscv = dev->fr_reg_base + FRSCV;
    regs->cc_stat.frmtccv = dev->fr_reg_base + FRMTCCV;
    regs->cc_stat.frrcv = dev->fr_reg_base + FRRCV;
    regs->cc_stat.frocv = dev->fr_reg_base + FROCV;
    regs->cc_stat.frsfs = dev->fr_reg_base + FRSFS;
    regs->cc_stat.frswnit = dev->fr_reg_base + FRSWNIT;
    regs->cc_stat.fracs = dev->fr_reg_base + FRACS;
    regs->cc_stat.fresid1 = dev->fr_reg_base + FRESID(1);
    regs->cc_stat.fresid2 = dev->fr_reg_base + FRESID(2);
    regs->cc_stat.fresid3 = dev->fr_reg_base + FRESID(3);
    regs->cc_stat.fresid4 = dev->fr_reg_base + FRESID(4);
    regs->cc_stat.fresid5 = dev->fr_reg_base + FRESID(5);
    regs->cc_stat.fresid6 = dev->fr_reg_base + FRESID(6);
    regs->cc_stat.fresid7 = dev->fr_reg_base + FRESID(7);
    regs->cc_stat.fresid8 = dev->fr_reg_base + FRESID(8);
    regs->cc_stat.fresid9 = dev->fr_reg_base + FRESID(9);
    regs->cc_stat.fresid10 = dev->fr_reg_base + FRESID(10);
    regs->cc_stat.fresid11 = dev->fr_reg_base + FRESID(11);
    regs->cc_stat.fresid12 = dev->fr_reg_base + FRESID(12);
    regs->cc_stat.fresid13 = dev->fr_reg_base + FRESID(13);
    regs->cc_stat.fresid14 = dev->fr_reg_base + FRESID(14);
    regs->cc_stat.fresid15 = dev->fr_reg_base + FRESID(15);
    regs->cc_stat.frosid1 = dev->fr_reg_base + FROSID(1);
    regs->cc_stat.frosid2 = dev->fr_reg_base + FROSID(2);
    regs->cc_stat.frosid3 = dev->fr_reg_base + FROSID(3);
    regs->cc_stat.frosid4 = dev->fr_reg_base + FROSID(4);
    regs->cc_stat.frosid5 = dev->fr_reg_base + FROSID(5);
    regs->cc_stat.frosid6 = dev->fr_reg_base + FROSID(6);
    regs->cc_stat.frosid7 = dev->fr_reg_base + FROSID(7);
    regs->cc_stat.frosid8 = dev->fr_reg_base + FROSID(8);
    regs->cc_stat.frosid9 = dev->fr_reg_base + FROSID(9);
    regs->cc_stat.frosid10 = dev->fr_reg_base + FROSID(10);
    regs->cc_stat.frosid11 = dev->fr_reg_base + FROSID(11);
    regs->cc_stat.frosid12 = dev->fr_reg_base + FROSID(12);
    regs->cc_stat.frosid13 = dev->fr_reg_base + FROSID(13);
    regs->cc_stat.frosid14 = dev->fr_reg_base + FROSID(14);
    regs->cc_stat.frosid15 = dev->fr_reg_base + FROSID(15);
    regs->cc_stat.frnmv1 = dev->fr_reg_base + FRNMV(1);
    regs->cc_stat.frnmv2 = dev->fr_reg_base + FRNMV(2);
    regs->cc_stat.frnmv3 = dev->fr_reg_base + FRNMV(3);
    /* Message buffer control */
    regs->msg_buf_ctrl.frmrc = dev->fr_reg_base + FRMRC;
    regs->msg_buf_ctrl.frfrf = dev->fr_reg_base + FRFRF;
    regs->msg_buf_ctrl.frfrfm = dev->fr_reg_base + FRFRFM;
    regs->msg_buf_ctrl.frfcl = dev->fr_reg_base + FRFCL;
    /* Message buffer status */
    regs->msg_buf_stat.frmhds = dev->fr_reg_base + FRMHDS;
    regs->msg_buf_stat.frldts = dev->fr_reg_base + FRLDTS;
    regs->msg_buf_stat.frfsr = dev->fr_reg_base + FRFSR;
    regs->msg_buf_stat.frmhdf = dev->fr_reg_base + FRMHDF;
    regs->msg_buf_stat.frtxrq[0] = dev->fr_reg_base + FRTXRQ(0);
    regs->msg_buf_stat.frtxrq[1] = dev->fr_reg_base + FRTXRQ(1);
    regs->msg_buf_stat.frtxrq[2] = dev->fr_reg_base + FRTXRQ(2);
    regs->msg_buf_stat.frtxrq[3] = dev->fr_reg_base + FRTXRQ(3);
    regs->msg_buf_stat.frndat[0] = dev->fr_reg_base + FRNDAT(0);
    regs->msg_buf_stat.frndat[1] = dev->fr_reg_base + FRNDAT(1);
    regs->msg_buf_stat.frndat[2] = dev->fr_reg_base + FRNDAT(2);
    regs->msg_buf_stat.frndat[3] = dev->fr_reg_base + FRNDAT(3);
    regs->msg_buf_stat.frmbsc[0] = dev->fr_reg_base + FRMBSC(0);
    regs->msg_buf_stat.frmbsc[1] = dev->fr_reg_base + FRMBSC(1);
    regs->msg_buf_stat.frmbsc[2] = dev->fr_reg_base + FRMBSC(2);
    regs->msg_buf_stat.frmbsc[3] = dev->fr_reg_base + FRMBSC(3);	
    /* Input Buffer */
    for (i = 0; i < FR_MAX_DATA_INDEX; i++){
        regs->in_buf.frwrds[i] = dev->fr_reg_base + FRWRDS(i + 1);
    }
    regs->in_buf.frwrhs1 = dev->fr_reg_base + FRWRHS1;
    regs->in_buf.frwrhs2 = dev->fr_reg_base + FRWRHS2;
    regs->in_buf.frwrhs3 = dev->fr_reg_base + FRWRHS3;
    regs->in_buf.fribcm = dev->fr_reg_base + FRIBCM;
    regs->in_buf.fribcr = dev->fr_reg_base + FRIBCR;
    
    /* Output Buffer */
    for (i = 0; i < FR_MAX_DATA_INDEX; i++){
        regs->out_buf.frrdds[i] = dev->fr_reg_base + FRRDDS(i + 1);
    }
    regs->out_buf.frrdhs1 = dev->fr_reg_base + FRRDHS1;
    regs->out_buf.frrdhs2 = dev->fr_reg_base + FRRDHS2;
    regs->out_buf.frrdhs3 = dev->fr_reg_base + FRRDHS3;
    regs->out_buf.frmbs = dev->fr_reg_base + FRMBS;
    regs->out_buf.frobcm = dev->fr_reg_base + FROBCM;
    regs->out_buf.frobcr = dev->fr_reg_base + FROBCR;
    
    /* Data transfer control */
    regs->data_xfer_ctrl.fritc = dev->fr_reg_base + FRITC;
    regs->data_xfer_ctrl.frotc = dev->fr_reg_base + FROTC;
    regs->data_xfer_ctrl.friba = dev->fr_reg_base + FRIBA;
    regs->data_xfer_ctrl.frfba = dev->fr_reg_base + FRFBA;
    regs->data_xfer_ctrl.froba = dev->fr_reg_base + FROBA;
    regs->data_xfer_ctrl.friqc = dev->fr_reg_base + FRIQC;
    regs->data_xfer_ctrl.fruir = dev->fr_reg_base + FRUIR;
    regs->data_xfer_ctrl.fruor = dev->fr_reg_base + FRUOR;

    /* Data transfer status */
    regs->data_xfer_stat.frits = dev->fr_reg_base + FRITS;
    regs->data_xfer_stat.frots = dev->fr_reg_base + FROTS;
    regs->data_xfer_stat.fraes = dev->fr_reg_base + FRAES;
    regs->data_xfer_stat.fraea = dev->fr_reg_base + FRAEA;

    regs->data_xfer_stat.frda[0] = dev->fr_reg_base + FRDA(0);
    regs->data_xfer_stat.frda[1] = dev->fr_reg_base + FRDA(1);
    regs->data_xfer_stat.frda[2] = dev->fr_reg_base + FRDA(2);
    regs->data_xfer_stat.frda[3] = dev->fr_reg_base + FRDA(3);
   
    return 0;
}

int rcar_fr_deinit(fr_dev_t *dev)
{
	fr_input_table_t *input_table = &dev->input_info;
	fr_output_table_t *output_table = &dev->output_info;
    // Disable the module
    fr_disable_module(dev);
	if(rcar_cpg_vbase){
		fr_enable_clock(false,rcar_cpg_vbase);
	}
    // Detach interrupts
    if (dev->iid != -1){
        InterruptDetach(dev->iid);
	}
	
	// unmap memory
	if(input_table->ita_vaddr){
		munmap((void*)input_table->ita_vaddr, input_table->table_size);
	}
	if(output_table->ota_vaddr){
		munmap((void*)output_table->ota_vaddr, output_table->ota_size);
	}
	if(output_table->ff_vaddr){
		munmap((void*)output_table->ff_vaddr, output_table->ff_size);
	}
    munmap_device_io(dev->fr_reg_base, RCAR_GEN3_FLEXRAY_SIZE);
    
    return 0;
}

void fr_config_msg_buf(fr_msg_tx_t *pmsg,fr_msg_ram_t msgr[], uint8_t numberMessage)
{
	int i = 0;
	
	for(i=0;i<numberMessage;i++){			
		pmsg->nmsg[i].cmdMask 			= msgr[i].cmdMask;
		pmsg->nmsg[i].idx 				= msgr[i].idx;
		pmsg->nmsg[i].frameId 			= msgr[i].frameId;
		pmsg->nmsg[i].cycleCount 		= msgr[i].cycleCount;
		pmsg->nmsg[i].channel 			= msgr[i].channel;
		pmsg->nmsg[i].type 				= msgr[i].type;	
		pmsg->nmsg[i].mode 				= msgr[i].mode;
		pmsg->nmsg[i].ppi 				= msgr[i].ppi;
		pmsg->nmsg[i].sync 				= msgr[i].sync;
		pmsg->nmsg[i].startup 			= msgr[i].startup;
		pmsg->nmsg[i].payloadLength		= msgr[i].payloadLength;		
	}
	pmsg->numberMessage = numberMessage;
}

void fr_config_msg_rx(fr_dev_t *dev,fr_msg_ram_t msgr[], uint8_t numberMessage)
{
	int i = 0;
	for(i=0;i<numberMessage;i++){	
		if((dev->msg_cfg.nmsg[i].type != FR_MSGBUF_TYPE_TX) && ( (!dev->msg_cfg.nmsg[i].sync) || (!dev->msg_cfg.nmsg[i].startup))){
			dev->msg_cfg.nmsg[i].cmdMask = FR_CMD_MASK_HEADER|FR_CMD_MASK_DATA|FR_CMD_MASK_REQUEST;
		}		
	}
}

int fr_init_msg_buf(fr_dev_t *dev)
{
	int status = 0;
	int i = 0;
	uint8_t idx = 0;
	uint32_t ipadx = 0;
	fr_input_table_t *pt = &dev->input_info;
	fr_regs_t *regs = &dev->regs;

	if(FR_NODE1==dev->node){
		if(dev->msg_ctl.dynamicBuffers==(FR_MBF_NUM - FR_MBF_NUM_STATIC)){
			fr_config_msg_buf(&dev->msg_cfg,fr_cfg11,FR_MBF_NUM);
		}else{
			fr_config_msg_buf(&dev->msg_cfg,fr_cfg21,FR_MBF_NUM);			
		}
	}else{
		if(dev->msg_ctl.dynamicBuffers==(FR_MBF_NUM - FR_MBF_NUM_STATIC)){
			fr_config_msg_buf(&dev->msg_cfg,fr_cfg12,FR_MBF_NUM);
		}else{
			fr_config_msg_buf(&dev->msg_cfg,fr_cfg22,FR_MBF_NUM);
		}
	}
	
	if(dev->mode){
		for (i = 0; i < dev->msg_cfg.numberMessage; i++) {
			status = fr_set_header_message_buffer(dev,&dev->msg_cfg.nmsg[i]);
			if(status) {
				fr_slogf(FR_SLOG_DEBUG1,"%s: fr_set_header_message_buffer failed\n",__func__);
				return -1;
			}
			fr_store_to_message_ram(dev,&dev->msg_cfg.nmsg[i]);
			// Wait if transfer between IBF Shadow and Message RAM in progress
			if(fr_wait_inputbuffer_busy(dev)){
				fr_slogf(FR_SLOG_DEBUG1,"%s: Wait transfer between IBF Shadow and Message RAM timeout", __func__);
				return -1;
			}
		}
	}else{
		if(fr_config_input(dev,&dev->msg_cfg)){
			return -1;
		}
		if(fr_set_output_fifo_size(dev)){
			return -1;
		}
		if(fr_config_output(dev)){
			return -1;
		}
		fr_allocate_output_memory(dev,&dev->msg_cfg);
		fr_config_fifo(dev,&dev->msg_cfg);
		if(fr_uram_output_enable(dev)){
			return -1;
		}
		fr_select_output_mode(dev,false);
		fr_enable_irq_output_uram(dev);	
		for (i = 0; i < dev->msg_cfg.numberMessage; i++) {
			ipadx = FR_REG_BITS(regs->data_xfer_stat.frits,FR_ITS_IPIDX_L,FR_ITS_IPIDX_H);
			idx = dev->msg_cfg.nmsg[i].idx;
			status = fr_set_header_uram(dev,&pt->pheader[idx],&dev->msg_cfg.nmsg[i]);
			if(status) {
				fr_slogf(FR_SLOG_DEBUG1,"%s: fr_set_header_uram failed\n",__func__);
				return -1;
			}
			if(fr_store_to_uram(dev,&pt->pentry[ipadx],&dev->msg_cfg.nmsg[i])){
				fr_slogf(FR_SLOG_DEBUG1,"%s: fr_store_to_uram timeout\n",__func__);
				return -1;
			}
			// Wait if transfer between IBF Shadow and Message RAM in progress
			if(fr_wait_inputbuffer_busy(dev)){
				fr_slogf(FR_SLOG_DEBUG1,"%s: Wait transfer between IBF Shadow and Message RAM timeout", __func__);
				return -1;
			}
		}
	}
	return 0;
}
int rcar_flexray_init(fr_dev_t *dev)
{
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: ThreadCtl failed\n",__func__);
		return -1;
    }
	get_machine_name();
	if(fr_query_hwi_device(dev)){
		dev->phybase = RCAR_GEN3_FLEXRAY_BASE;
		dev->len = RCAR_GEN3_FLEXRAY_SIZE;
		if(product_id == RCAR_V3U_FLEXRAY){
			dev->irq = RCAR_V3U_INTCSYS_FRAY;
		}else{
			dev->irq = RCAR_GEN3_INTCSYS_FRAY;
		}
	}
#ifdef 	RCAR_CONFIG_GPIO_FLEXRAY
	uintptr_t rcar_gpr = 0;
	if ((rcar_gpr = mmap_device_io(rcar_pfc_size, rcar_pfc_base)) == MAP_DEVICE_FAILED){
        return EINVAL;
	}
	if ((rcar_cpg_vbase = mmap_device_io(0x3000, rcar_cpg_base)) == MAP_DEVICE_FAILED){
        return EINVAL;
	}
	// Enable clock
	fr_enable_clock(true,rcar_cpg_vbase);
	if(product_id == RCAR_V3U_FLEXRAY){
		fr_pfc_v3u_config(rcar_gpr);
	}else if(product_id == RCAR_V3H_FLEXRAY){
		fr_pfc_v3h_config(rcar_gpr);
	}
	else if(product_id == RCAR_V3M_FLEXRAY){
		fr_pfc_v3m_config(rcar_gpr);
	}else{
		fr_pfc_v3u_config(rcar_gpr);
	}
	munmap_device_io(rcar_gpr,rcar_pfc_size);
#endif
    if (map_regs(dev) == EINVAL){
        fr_slogf(FR_SLOG_DEBUG1,"%s: Unable to map registers", __func__);
        return -1;
    }
		// Enable the module
	if(fr_enable_module(dev)) {
		return -1;
	}
	dev->priority = FR_PRIORITY;
		
	fr_clear_all_flags(dev);
	// Configure protocol
	if (fr_node_init(dev)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: FlexRay init failed", __func__);
		return -1;
	}
	if(fr_config_msg_ram(dev,&dev->msg_ctl)){
		return -1;
	}  	
	if(fr_init_msg_buf(dev)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: FlexRay init for message buffer failed", __func__);
		return -1;
	}
	// Initialize interrupts
	fr_irq_setup(dev);
	fr_timers_irq_enable(dev);
	if(fr_startup(dev)){
		fr_slogf(FR_SLOG_DEBUG1,"%s: FlexRay startup failed", __func__);
		return -1;
	}
	if(FR_NODE1==dev->node){
		if(dev->msg_ctl.dynamicBuffers==(FR_MBF_NUM - FR_MBF_NUM_STATIC)){
			fr_config_msg_rx(dev,fr_cfg11,FR_MBF_NUM);
		}else{
			fr_config_msg_rx(dev,fr_cfg21,FR_MBF_NUM);			
		}
	}else{
		if(dev->msg_ctl.dynamicBuffers==(FR_MBF_NUM - FR_MBF_NUM_STATIC)){
			fr_config_msg_rx(dev,fr_cfg12,FR_MBF_NUM);
		}else{
			fr_config_msg_rx(dev,fr_cfg22,FR_MBF_NUM);
		}
	}	
	return 0;
}

int rcar_start_timer(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	uint32_t *data;
	uint32_t timer_id = 1;
	int status = 0;
	data = _DEVCTL_DATA(msg->i);
	timer_id = *data;
	status = fr_start_timer(dev,timer_id);
	fr_start_stopwatch(dev);
	
	memset(&msg->o, 0, sizeof(msg->o));
	msg->o.ret_val = status;
	msg->o.nbytes = 0;
	return(_RESMGR_PTR(ctp, &msg->o, sizeof(msg->o)));
}

int rcar_stop_timer(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	uint32_t *data;
	uint32_t timer_id = 1;
	
	data = _DEVCTL_DATA(msg->i);
	timer_id = *data;
	fr_stop_timer(dev,timer_id);
	fr_stop_stopwatch(dev);
	memset(&msg->o, 0, sizeof(msg->o));
	msg->o.ret_val = EOK;
	msg->o.nbytes = 0;
	return(_RESMGR_PTR(ctp, &msg->o, sizeof(msg->o)));
}

int rcar_get_stop_watch_value(resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb)
{
	fr_dev_t *dev = (fr_dev_t *)ocb->attr;
	fr_stopwatch_value_t *rx_data;
	int     nbytes = 0;
	
	rx_data = (fr_stopwatch_value_t *)_DEVCTL_DATA(msg->i);
	
	nbytes = sizeof(fr_stopwatch_value_t);
	
	rx_data->smtv = (uint32_t)dev->smtv;
	rx_data->sccv = (uint32_t)dev->sccv;
	rx_data->sscvb = (uint32_t)dev->sscvb;
	rx_data->sscva = (uint32_t)dev->sscva;
	
	memset(&msg->o, 0, sizeof(msg->o));
	msg->o.ret_val = EOK;
	msg->o.nbytes = nbytes;
	
	return(_RESMGR_PTR(ctp, &msg->o, sizeof(msg->o)+ nbytes));
}
