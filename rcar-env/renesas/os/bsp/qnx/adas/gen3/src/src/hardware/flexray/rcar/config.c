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
#include "rcar-flexray.h"
#include "flexray-regs.h"

const char *FR_PATHFILE="/etc/system/";

#define fr_config_check(p, name, min, max) \
	do { \
		if (p->name < min || p->name > max) { \
			fr_slogf(FR_SLOG_DEBUG1,"%s: Parameter out of range: p->name=%d > max=%d", __func__,p->name,max);	\
			break; \
		} \
		} while (0) 

int fr_get_conf(fr_dev_t *dev)
{
	char buf[LINE_MAX];
	char *opt = NULL;
	char *c = NULL;
	char *str_name = NULL;
	char *str_value = NULL;
	int lineno = 0;
	FILE *fd = NULL;
	const char *flexray_path = NULL;

	if(NULL==strstr(dev->system_parameter,".conf")){
		
		flexray_path = getenv("FLEXRAY_PATH");
		if (!flexray_path) {
			fr_slogf(FR_SLOG_DEBUG3,"%s:FLEXRAY_PATH environment variable not found. Use file name %s",__func__,FR_PATHFILE);
			flexray_path=FR_PATHFILE;
		}
		memset(dev->system_parameter,0,PATH_MAX);
		snprintf(dev->system_parameter, sizeof(dev->system_parameter), "%sflexray-node.conf", flexray_path);
	}
	fd = fopen(dev->system_parameter, "r");
	if (NULL==fd){
		fr_slogf(FR_SLOG_DEBUG3,"%s:Can't locate %s file",__func__,dev->system_parameter);
		memset(dev->system_parameter,0,PATH_MAX);
		return -1;
	}
	while (fgets(buf, sizeof (buf), fd) != NULL) {
		lineno++;
		c = buf;
		while ((*c == ' ') || (*c == '\t'))
			c++;
		if ((*c == '\015') || (*c== '\032') || (*c == '\0') || (*c == '\n') || (*c == '#'))
			continue;
		opt = c;
		while ((*c == '\015') || (*c== '\032') || ((*c != '\0') && (*c != '\n') && (*c != '#')))
			c++;
		*c = '\0';
		//Cluster parameter
		if ((str_name = strstr (opt, "gColdStartAttempts"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gColdStartAttempts = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdActionPointOffset"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdActionPointOffset = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdCASRxLowMax"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdCASRxLowMax = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdDynamicSlotIdlePhase"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdDynamicSlotIdlePhase = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdMinislot"))){
			str_value = strstr (opt, "gdMinislotActionPointOffset");
			if(!str_value){
				str_value = str_name;
				if ((str_name = strstr (str_value, "="))) {
					dev->cluster_param.gdMinislot = atoi(str_name + 1);
				}
			}
		}
		if ((str_name = strstr (opt, "gdMinislotActionPointOffset"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdMinislotActionPointOffset = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdStaticSlot"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdStaticSlot = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdTSSTransmitter"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdTSSTransmitter = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdWakeupSymbolRxIdle"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdWakeupSymbolRxIdle = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdWakeupSymbolRxLow"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdWakeupSymbolRxLow = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdWakeupSymbolRxWindow"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdWakeupSymbolRxWindow = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdWakeupSymbolTxIdle"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdWakeupSymbolTxIdle = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdWakeupSymbolTxLow"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdWakeupSymbolTxLow = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gListenNoise"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gListenNoise = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gMacroPerCycle"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gMacroPerCycle = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gMaxWithoutClockCorrectionFatal"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gMaxWithoutClockCorrectionFatal = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gMaxWithoutClockCorrectionPassive"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gMaxWithoutClockCorrectionPassive = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gNumberOfMinislots"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gNumberOfMinislots = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gNumberOfStaticSlots"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gNumberOfStaticSlots = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gOffsetCorrectionStart"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gOffsetCorrectionStart = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gPayloadLengthStatic"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gPayloadLengthStatic = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gSyncNodeMax"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gSyncNodeMax = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdNIT"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdNIT = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gdSampleClockPeriod"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gdSampleClockPeriod = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gNetworkManagementVectorLength"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gNetworkManagementVectorLength = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "gNetworkManagementVectorLength"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->cluster_param.gNetworkManagementVectorLength = atoi(str_name + 1);
			}
		}
		//Node parameter
		if ((str_name = strstr (opt, "pAllowHaltDueToClock"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pAllowHaltDueToClock = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pAllowPassiveToActive"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pAllowPassiveToActive = atoi(str_name + 1);
			}
		}	
		if ((str_name = strstr (opt, "pChannels"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pChannels = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pdAcceptedStartupRange"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pdAcceptedStartupRange = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pClusterDriftDamping"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pClusterDriftDamping = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pDecodingCorrection"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pDecodingCorrection = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pDelayCompensationA"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pDelayCompensationA = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pDelayCompensationB"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pDelayCompensationB = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pdListenTimeOut"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pdListenTimeOut = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pdMaxDrift"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pdMaxDrift = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pExternOffsetCorrection"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pExternOffsetCorrection = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pExternRateCorrection"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pExternRateCorrection = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pKeySlotUsedForStartup"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pKeySlotUsedForStartup = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pKeySlotUsedForSync"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pKeySlotUsedForSync = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pLatestTx"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pLatestTx = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pMacroInitialOffsetA"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pMacroInitialOffsetA = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pMacroInitialOffsetB"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pMacroInitialOffsetB = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pMicroInitialOffsetA"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pMicroInitialOffsetA = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pMicroInitialOffsetB"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pMicroInitialOffsetB = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pMicroPerCycle"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pMicroPerCycle = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pOffsetCorrectionOut"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pOffsetCorrectionOut = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pRateCorrectionOut"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pRateCorrectionOut = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pSingleSlotEnabled"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pSingleSlotEnabled = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pWakeupChannel"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pWakeupChannel = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pWakeupPattern"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pWakeupPattern = atoi(str_name + 1);
			}
		}
		if ((str_name = strstr (opt, "pSamplesPerMicrotick"))){
			str_value = str_name;
			if ((str_name = strstr (str_value, "="))) {
				dev->node_param.pSamplesPerMicrotick = atoi(str_name + 1);
			}
		}	
	}
	fclose(fd);
	return 0;
}

/** 
 * @brief Validate parametes of cluster.
 * @param p: Pointer to struct fr_cluster_param.
 * @return 0:success, EINVAL:error
 */
int fr_validate_cluster_param(struct fr_cluster_param *p)
{
	fr_config_check(p, gColdStartAttempts, 2, 31);
	fr_config_check(p, gdActionPointOffset, 1, 63);
	fr_config_check(p, gdCASRxLowMax, 67, 99);
	fr_config_check(p, gdDynamicSlotIdlePhase, 0, 2);
	fr_config_check(p, gdMinislot, 2, 63);
	fr_config_check(p, gdMinislotActionPointOffset, 1, 31);
	fr_config_check(p, gdStaticSlot, 10, 659);
	fr_config_check(p, gdTSSTransmitter, 3, 15);
	fr_config_check(p, gdWakeupSymbolRxLow, 11, 59);
	fr_config_check(p, gdWakeupSymbolRxIdle, 14, 59);
	fr_config_check(p, gdWakeupSymbolRxWindow, 76, 301);
	fr_config_check(p, gdWakeupSymbolTxIdle, 45, 180);
	fr_config_check(p, gdWakeupSymbolTxLow, 15, 60);
	fr_config_check(p, gListenNoise, 2, 16);
	fr_config_check(p, gMacroPerCycle, 10, 16000);
	fr_config_check(p, gMaxWithoutClockCorrectionFatal, 1, 15);
	fr_config_check(p, gMaxWithoutClockCorrectionPassive, 1, 15);
	fr_config_check(p, gNumberOfMinislots, 0, 7986);
	fr_config_check(p, gNumberOfStaticSlots, 2, 1023);
	fr_config_check(p, gOffsetCorrectionStart, 9, 15999);
	fr_config_check(p, gPayloadLengthStatic, 0, 127);
	fr_config_check(p, gSyncNodeMax, 2, 15);
	fr_config_check(p, gdNIT, 2, 805);
	//fr_config_check(p, gClusterDriftDamping, 0, 5);
	fr_config_check(p, gNetworkManagementVectorLength, 0, 12);
	//fr_config_check(p, gdMaxInitializationError, 0, 117);
	//fr_config_check(p, gdMinPropagationDelay, 0,10);
	return 0;
}
/** 
 * @brief Validate parametes of node.
 * @param p: Pointer to struct fr_node_param.
 * @p@return 0:success, EINVAL:error
 */
int fr_validate_node_param(struct fr_node_param *p)
{
	fr_config_check(p, pAllowPassiveToActive, 0, 31);
	fr_config_check(p, pChannels, 0, 3);
	fr_config_check(p, pdAcceptedStartupRange, 0, 1875);
	fr_config_check(p, pClusterDriftDamping, 0, 20);
	fr_config_check(p, pDecodingCorrection, 14, 143);
	fr_config_check(p, pdListenTimeOut, 1284, 1283846);
	fr_config_check(p, pdMaxDrift, 2, 1923);
	fr_config_check(p, pExternOffsetCorrection, 0, 7);
	fr_config_check(p, pExternRateCorrection, 0, 28);
	//fr_config_check(p, pKeySlotID, 0, 1023);
	fr_config_check(p, pLatestTx, 0, 7981);
	fr_config_check(p, pMacroInitialOffsetA, 2, 68);
	fr_config_check(p, pMacroInitialOffsetB, 2, 68);
	fr_config_check(p, pMicroInitialOffsetA, 0, 239);
	fr_config_check(p, pMicroInitialOffsetB, 0, 239);
	fr_config_check(p, pMicroPerCycle, 640, 640000);
	fr_config_check(p, pOffsetCorrectionOut, 13, 15567);
	fr_config_check(p, pRateCorrectionOut, 2, 1923);
	fr_config_check(p, pWakeupChannel, 0, 3);
	fr_config_check(p, pWakeupPattern, 2, 63);
	return 0;
}

int fr_set_cluster_param(fr_regs_t *regs, struct fr_cluster_param *p)
{
	if( (regs == NULL) || (p == NULL) ) {
        fr_slogf(FR_SLOG_DEBUG1,"%s: Null input pointer", __func__);
        return -1;
    }
	
	if(fr_validate_cluster_param(p)) {
		fr_slogf(FR_SLOG_DEBUG1,"%s: Validate cluster parameter failed", __func__);
		//return -1;
	}

	write_bits(regs->cc_ctrl.frsucc1, FR_SUCC1_CSA_L, FR_SUCC1_CSA_H, p->gColdStartAttempts);
	write_bits(regs->cc_ctrl.frgtuc9, FR_GTUC9_APO_L, FR_GTUC9_APO_H, p->gdActionPointOffset);
	/* 	53.2.5.5 FRPRTC1 [h’0090]
		Notes: Bit[10]=CASM[6] is fixed to 1, therefore bit[10] is “Read Only” bit: user cannot write this bit.
		Valid values are 67 to 99 bit times.
	*/
	write_bits(regs->cc_ctrl.frprtc1, FR_PRTC1_CASM_L, FR_PRTC1_CASM_H, p->gdCASRxLowMax);
	write_bits(regs->cc_ctrl.frgtuc9, FR_GTUC9_DSI_L, FR_GTUC9_DSI_H, p->gdDynamicSlotIdlePhase);
	write_bits(regs->cc_ctrl.frgtuc8, FR_GTUC8_MSL_L, FR_GTUC8_MSL_H, p->gdMinislot);
	write_bits(regs->cc_ctrl.frgtuc9, FR_GTUC9_MAPO_L, FR_GTUC9_MAPO_H, p->gdMinislotActionPointOffset);
	write_bits(regs->cc_ctrl.frgtuc7, FR_GTUC7_SSL_L, FR_GTUC7_SSL_H, p->gdStaticSlot);
	write_bits(regs->cc_ctrl.frprtc1, FR_PRTC1_TSST_L, FR_PRTC1_TSST_H, p->gdTSSTransmitter);
	write_bits(regs->cc_ctrl.frprtc2, FR_PRTC2_RXI_L, FR_PRTC2_RXI_H, p->gdWakeupSymbolRxIdle);
	write_bits(regs->cc_ctrl.frprtc2, FR_PRTC2_RXL_L, FR_PRTC2_RXL_H, p->gdWakeupSymbolRxLow);
	write_bits(regs->cc_ctrl.frprtc1, FR_PRTC1_RXW_L, FR_PRTC1_RXW_H, p->gdWakeupSymbolRxWindow);
	write_bits(regs->cc_ctrl.frprtc2, FR_PRTC2_TXI_L, FR_PRTC2_TXI_H, p->gdWakeupSymbolTxIdle );
	write_bits(regs->cc_ctrl.frprtc2, FR_PRTC2_TXL_L, FR_PRTC2_TXL_H, p->gdWakeupSymbolTxLow);
	write_bits(regs->cc_ctrl.frsucc2, FR_SUCC2_LTN_L, FR_SUCC2_LTN_H, p->gListenNoise - 1);
	write_bits(regs->cc_ctrl.frgtuc2, FR_GTUC2_MPC_L, FR_GTUC2_MPC_H, p->gMacroPerCycle);
	write_bits(regs->cc_ctrl.frsucc3, FR_SUCC3_WCF_L, FR_SUCC3_WCF_H, p->gMaxWithoutClockCorrectionFatal );
	write_bits(regs->cc_ctrl.frsucc3, FR_SUCC3_WCP_L, FR_SUCC3_WCP_H, p->gMaxWithoutClockCorrectionPassive);
	write_bits(regs->cc_ctrl.frgtuc8, FR_GTUC8_NMS_L, FR_GTUC8_NMS_H, p->gNumberOfMinislots);
	write_bits(regs->cc_ctrl.frgtuc7, FR_GTUC7_NSS_L, FR_GTUC7_NSS_H, p->gNumberOfStaticSlots);
	write_bits(regs->cc_ctrl.frgtuc4, FR_GTUC4_OCS_L, FR_GTUC4_OCS_H, p->gOffsetCorrectionStart - 1);
	write_bits(regs->cc_ctrl.frmhdc, FR_MHDC_SFDL_L, FR_MHDC_SFDL_H, p->gPayloadLengthStatic);
	write_bits(regs->cc_ctrl.frgtuc2, FR_GTUC2_SNM_L, FR_GTUC2_SNM_H, p->gSyncNodeMax);
	write_bits(regs->cc_ctrl.frgtuc4, FR_GTUC4_NIT_L, FR_GTUC4_NIT_H,p->gMacroPerCycle - p->gdNIT -1);
	write_bits(regs->cc_ctrl.frnemc, FR_NEMC_NML_L, FR_NEMC_NML_H, p->gNetworkManagementVectorLength);
	
	return 0;
}

int fr_set_node_param(fr_regs_t *regs, struct fr_node_param *p)
{
	if( (regs == NULL) || (p == NULL) ) {
        fr_slogf(FR_SLOG_DEBUG1,"%s: Null input pointer", __func__);
        return EINVAL;
    }
	if(fr_validate_node_param(p)) {
		fr_slogf(FR_SLOG_DEBUG1,"%s: Validate node parameter failed", __func__);
		//return EINVAL;
	}

    if (p->pAllowHaltDueToClock == true) {
        set_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_HCSE);
    } else {
        clear_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_HCSE);
    }
	write_bits(regs->cc_ctrl.frsucc1, FR_SUCC1_PTA_L, FR_SUCC1_PTA_H, p->pAllowPassiveToActive);
	if(FR_CHANNEL_AB==p->pChannels) {
		write_bits(regs->cc_ctrl.frsucc1, FR_SUCC1_CCHA,FR_SUCC1_CCHB,p->pChannels);
	}else if(FR_CHANNEL_A==p->pChannels) {
		set_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_CCHA);
	}else  if(FR_CHANNEL_B==p->pChannels) {
		set_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_CCHB);
	}
	write_bits(regs->cc_ctrl.frgtuc6, FR_GTUC6_ASR_L, FR_GTUC6_ASR_H, p->pdAcceptedStartupRange);
	write_bits(regs->cc_ctrl.frgtuc5, FR_GTUC5_CDD_L, FR_GTUC5_CDD_H, p->pClusterDriftDamping);
	write_bits(regs->cc_ctrl.frgtuc5, FR_GTUC5_DEC_L, FR_GTUC5_DEC_H, p->pDecodingCorrection);
	write_bits(regs->cc_ctrl.frgtuc5, FR_GTUC5_DCA_L, FR_GTUC5_DCA_H, p->pDelayCompensationA);
	write_bits(regs->cc_ctrl.frgtuc5, FR_GTUC5_DCB_L, FR_GTUC5_DCB_H, p->pDelayCompensationB);
	write_bits(regs->cc_ctrl.frsucc2, FR_SUCC2_LT_L, FR_SUCC2_LT_H, p->pdListenTimeOut );
	write_bits(regs->cc_ctrl.frgtuc6, FR_GTUC6_MOD_L, FR_GTUC6_MOD_H, p->pdMaxDrift);
	write_bits(regs->cc_ctrl.frgtuc11, FR_GTUC11_EOC_L, FR_GTUC11_EOC_H, p->pExternOffsetCorrection);
	write_bits(regs->cc_ctrl.frgtuc11, FR_GTUC11_ERC_L, FR_GTUC11_ERC_H, p->pExternRateCorrection);
    if (p->pKeySlotUsedForStartup == true) {
        set_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_TXST);
    } else {
        clear_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_TXST);
    }
    if (p->pKeySlotUsedForSync == true) {
        set_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_TXSY);
    } else {
        clear_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_TXSY);
    }
	write_bits(regs->cc_ctrl.frmhdc, FR_MHDC_SLT_L, FR_MHDC_SLT_H, p->pLatestTx);
	write_bits(regs->cc_ctrl.frgtuc3, FR_GTUC3_MIOA_L, FR_GTUC3_MIOA_H, p->pMacroInitialOffsetA);
	write_bits(regs->cc_ctrl.frgtuc3, FR_GTUC3_MIOB_L, FR_GTUC3_MIOB_H, p->pMacroInitialOffsetB);
	write_bits(regs->cc_ctrl.frgtuc3, FR_GTUC3_UIOA_L, FR_GTUC3_UIOA_H, p->pMicroInitialOffsetA);
	write_bits(regs->cc_ctrl.frgtuc3, FR_GTUC3_UIOB_L, FR_GTUC3_UIOB_H, p->pMicroInitialOffsetB);
	write_bits(regs->cc_ctrl.frgtuc1, FR_GTUC1_UT_L, FR_GTUC1_UT_H, p->pMicroPerCycle);
	write_bits(regs->cc_ctrl.frgtuc10, FR_GTUC10_MOC_L, FR_GTUC10_MOC_H, p->pOffsetCorrectionOut);
	write_bits(regs->cc_ctrl.frgtuc10, FR_GTUC10_MRC_L, FR_GTUC10_MRC_H, p->pRateCorrectionOut );
	if (p->pSingleSlotEnabled == true) {
        set_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_TSM);
    } else {
        clear_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_TSM);
    }
    if (p->pWakeupChannel == FR_WAKEUP_CH_A) {
        clear_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_WUCS);
    } else {
        set_bit(regs->cc_ctrl.frsucc1, FR_SUCC1_WUCS);
    }
	write_bits(regs->cc_ctrl.frprtc1, FR_PRTC1_RWP_L, FR_PRTC1_RWP_H, p->pWakeupPattern);
	return 0;
}
