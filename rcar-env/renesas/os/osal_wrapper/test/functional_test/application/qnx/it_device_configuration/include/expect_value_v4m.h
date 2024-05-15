/***********************************************************************************************************************
* Copyright [2023-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    expect_value_v4m.h
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configurator of V4M Setting
***********************************************************************************************************************/

#include "expect_value_struct.h"

/* Table 3-38 Device Configuration value for Interrupt (For R-Car V4M) */
interrupt_struct_t ims_00_st_int_info[]={579};
interrupt_struct_t ims_01_st_int_info[]={580};
interrupt_struct_t imr_00_st_int_info[]={581};
interrupt_struct_t imr_01_st_int_info[]={582};
interrupt_struct_t imp_top_00_st_int_info[]={565, 566, 567};
interrupt_struct_t imp_00_st_int_info[]={NULL};
interrupt_struct_t imp_01_st_int_info[]={NULL};
interrupt_struct_t imp_ocv_00_st_int_info[]={NULL};
interrupt_struct_t imp_ocv_01_st_int_info[]={NULL};
interrupt_struct_t imp_ocv_02_st_int_info[]={NULL};
interrupt_struct_t imp_ocv_03_st_int_info[]={NULL};
interrupt_struct_t imp_dma_00_st_int_info[]={NULL};
interrupt_struct_t imp_dma_01_st_int_info[]={NULL};
interrupt_struct_t imp_psc_00_st_int_info[]={NULL};
interrupt_struct_t imp_dta_00_st_int_info[]={NULL};
interrupt_struct_t imp_dma_slim_00_st_int_info[]={NULL};
interrupt_struct_t imp_dma_slim_01_st_int_info[]={NULL};
interrupt_struct_t imp_cnn_00_st_int_info[]={NULL};
interrupt_struct_t imp_reg_spmi_00_st_int_info[]={NULL};
interrupt_struct_t imp_reg_spmc_00_st_int_info[]={NULL};
interrupt_struct_t imp_spmi_00_st_int_info[]={NULL};
interrupt_struct_t imp_spmc_00_st_int_info[]={NULL};
interrupt_struct_t imp_spmc_01_st_int_info[]={NULL};
interrupt_struct_t vdsp00_00_st_int_info[]={478, 479, 480, 481, 482};
interrupt_struct_t vdsp00_01_st_int_info[]={NULL};
interrupt_struct_t vdsp00_02_st_int_info[]={NULL};
interrupt_struct_t vdsp00_03_st_int_info[]={NULL};
interrupt_struct_t vdsp00_04_st_int_info[]={NULL};
interrupt_struct_t vdsp00_05_st_int_info[]={NULL};
interrupt_struct_t vdsp00_06_st_int_info[]={NULL};
interrupt_struct_t vdsp00_07_st_int_info[]={NULL};
interrupt_struct_t vdsp00_08_st_int_info[]={NULL};
interrupt_struct_t vdsp01_00_st_int_info[]={483,484,485,486,487};
interrupt_struct_t vdsp01_01_st_int_info[]={NULL};
interrupt_struct_t vdsp01_02_st_int_info[]={NULL};
interrupt_struct_t vdsp01_03_st_int_info[]={NULL};
interrupt_struct_t vdsp01_04_st_int_info[]={NULL};
interrupt_struct_t vdsp01_05_st_int_info[]={NULL};
interrupt_struct_t vdsp01_06_st_int_info[]={NULL};
interrupt_struct_t vdsp01_07_st_int_info[]={NULL};
interrupt_struct_t vdsp01_08_st_int_info[]={NULL};
interrupt_struct_t dof_00_st_int_info[]={861,862};
interrupt_struct_t smd_ps_00_st_int_info[]={873, 874};
interrupt_struct_t smd_post_00_st_int_info[]={871, 872};
interrupt_struct_t dof_fcprm_00_st_int_info[]={NULL};
interrupt_struct_t vip_wrap_00_st_int_info[]={NULL};
interrupt_struct_t gsx_00_st_int_info[]={464};
interrupt_struct_t iccom_reg_00_st_int_info[]={NULL};
interrupt_struct_t iccom_00_st_int_info[]={123};
interrupt_struct_t iccom_01_st_int_info[]={125};
interrupt_struct_t iccom_02_st_int_info[]={127};
interrupt_struct_t iccom_03_st_int_info[]={129};
interrupt_struct_t iccom_04_st_int_info[]={131};
interrupt_struct_t iccom_05_st_int_info[]={133};
interrupt_struct_t iccom_06_st_int_info[]={135};
interrupt_struct_t iccom_07_st_int_info[]={137};
interrupt_struct_t iccom_08_st_int_info[]={916};
interrupt_struct_t iccom_09_st_int_info[]={918};
interrupt_struct_t iccom_10_st_int_info[]={920};
interrupt_struct_t iccom_11_st_int_info[]={922};
interrupt_struct_t ivcp1e_00_st_int_info[]={855,856};
interrupt_struct_t cisp_00_st_int_info[]={475};
interrupt_struct_t tisp_00_st_int_info[]={473};
interrupt_struct_t tisp_01_st_int_info[]={474};
interrupt_struct_t vspx_00_st_int_info[]={556};
interrupt_struct_t fcpvx_00_st_int_info[]={NULL};
interrupt_struct_t gicd_00_st_int_info[]={NULL};
interrupt_struct_t fbc_00_st_int_info[]={852};
interrupt_struct_t fba_ims0_st_int_info[]={695};
interrupt_struct_t fba_ims1_st_int_info[]={696};
interrupt_struct_t fba_imr0_st_int_info[]={691};
interrupt_struct_t fba_imr1_st_int_info[]={692};
interrupt_struct_t fba_imp0_st_int_info[]={713};
interrupt_struct_t fba_imp1_st_int_info[]={714};
interrupt_struct_t fba_imp2_st_int_info[]={715};
interrupt_struct_t fba_imp_ocv0_st_int_info[]={719};
interrupt_struct_t fba_imp_ocv1_st_int_info[]={720};
interrupt_struct_t fba_imp_ocv2_st_int_info[]={721};
interrupt_struct_t fba_imp_ocv3_st_int_info[]={722};
interrupt_struct_t fba_imp_dp0_st_int_info[]={717};
interrupt_struct_t fba_imp_dp1_st_int_info[]={718};
interrupt_struct_t fba_imp_cnn0_st_int_info[]={727};
interrupt_struct_t fba_imp_dsp0_st_int_info[]={738};
interrupt_struct_t fba_imp_dsp1_st_int_info[]={739};
interrupt_struct_t fba_imp_cnram0_st_int_info[]={744};
interrupt_struct_t fba_umfl0_st_int_info[]={701};
interrupt_struct_t fba_smps0_st_int_info[]={703};
interrupt_struct_t fba_smpo0_st_int_info[]={707};
interrupt_struct_t fba_pap0_st_int_info[]={699};
interrupt_struct_t fba_isp0_st_int_info[]={686};
interrupt_struct_t fba_ca76d0_st_int_info[]={669};
interrupt_struct_t fba_ca760_st_int_info[]={671};
interrupt_struct_t fba_ca761_st_int_info[]={672};
interrupt_struct_t fba_ca762_st_int_info[]={673};
interrupt_struct_t fba_ca763_st_int_info[]={674};
interrupt_struct_t rfso_00_st_int_info[]={227};
interrupt_struct_t rfso_01_st_int_info[]={228};
interrupt_struct_t rfso_02_st_int_info[]={229};
interrupt_struct_t rfso_03_st_int_info[]={230};
interrupt_struct_t rfso_04_st_int_info[]={231};
interrupt_struct_t rfso_05_st_int_info[]={232};
interrupt_struct_t rfso_06_st_int_info[]={233};
interrupt_struct_t rfso_07_st_int_info[]={234};
interrupt_struct_t rfso_08_st_int_info[]={235};
interrupt_struct_t rfso_09_st_int_info[]={236};
interrupt_struct_t rfso_10_st_int_info[]={237};
interrupt_struct_t ipmmu_mm_00_st_int_info[]={210,211};
interrupt_struct_t ipmmu_cache_vi0_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_vi1_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_vc_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_ir_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_rt0_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_rt1_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_ds0_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_hsc_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_vip0_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_vip1_st_int_info[]={NULL};
interrupt_struct_t ipmmu_cache_3dg_st_int_info[]={NULL};
interrupt_struct_t OSAL_DEVICE_INVALID_IDst_int_info[]={NULL};
interrupt_struct_t caiplite_wrapper_00_st_int_info[]={NULL};
interrupt_struct_t caiplite_00_st_int_info[]={900};
interrupt_struct_t caiplite_01_st_int_info[]={902};
interrupt_struct_t caiplite_02_st_int_info[]={904};
interrupt_struct_t caiplite_03_st_int_info[]={906};
interrupt_struct_t caiplite_04_st_int_info[]={908};
interrupt_struct_t caiplite_05_st_int_info[]={910};
interrupt_struct_t caiplite_06_st_int_info[]={912};
interrupt_struct_t caiplite_07_st_int_info[]={914};
interrupt_struct_t wcrc_caiplite_00_st_int_info[]={NULL};
interrupt_struct_t wcrc_caiplite_01_st_int_info[]={NULL};
interrupt_struct_t wcrc_caiplite_02_st_int_info[]={NULL};
interrupt_struct_t wcrc_caiplite_03_st_int_info[]={NULL};
interrupt_struct_t rtdmac_control0_00_st_int_info[]={10};
interrupt_struct_t rtdmac_control0_01_st_int_info[]={11};
interrupt_struct_t rtdmac_control0_02_st_int_info[]={12};
interrupt_struct_t rtdmac_control0_03_st_int_info[]={13};
interrupt_struct_t rtdmac_control0_04_st_int_info[]={14};
interrupt_struct_t rtdmac_control0_05_st_int_info[]={15};
interrupt_struct_t rtdmac_control0_06_st_int_info[]={16};
interrupt_struct_t rtdmac_control0_07_st_int_info[]={17};
interrupt_struct_t rtdmac_control0_08_st_int_info[]={18};
interrupt_struct_t rtdmac_control0_09_st_int_info[]={19};
interrupt_struct_t rtdmac_control0_10_st_int_info[]={20};
interrupt_struct_t rtdmac_control0_11_st_int_info[]={21};
interrupt_struct_t rtdmac_control0_12_st_int_info[]={22};
interrupt_struct_t rtdmac_control0_13_st_int_info[]={23};
interrupt_struct_t rtdmac_control0_14_st_int_info[]={24};
interrupt_struct_t rtdmac_control0_15_st_int_info[]={25};
interrupt_struct_t rtdmac_control1_00_st_int_info[]={28};
interrupt_struct_t rtdmac_control1_01_st_int_info[]={29};
interrupt_struct_t rtdmac_control1_02_st_int_info[]={30};
interrupt_struct_t rtdmac_control1_03_st_int_info[]={31};
interrupt_struct_t rtdmac_control1_04_st_int_info[]={32};
interrupt_struct_t rtdmac_control1_05_st_int_info[]={33};
interrupt_struct_t rtdmac_control1_06_st_int_info[]={34};
interrupt_struct_t rtdmac_control1_07_st_int_info[]={35};
interrupt_struct_t rtdmac_control1_08_st_int_info[]={36};
interrupt_struct_t rtdmac_control1_09_st_int_info[]={37};
interrupt_struct_t rtdmac_control1_10_st_int_info[]={38};
interrupt_struct_t rtdmac_control1_11_st_int_info[]={39};
interrupt_struct_t rtdmac_control1_12_st_int_info[]={40};
interrupt_struct_t rtdmac_control1_13_st_int_info[]={41};
interrupt_struct_t rtdmac_control1_14_st_int_info[]={42};
interrupt_struct_t rtdmac_control1_15_st_int_info[]={43};
interrupt_struct_t vin_00_st_int_info[]={529};
interrupt_struct_t vin_01_st_int_info[]={530};
interrupt_struct_t vin_02_st_int_info[]={531};
interrupt_struct_t vin_03_st_int_info[]={532};
interrupt_struct_t vin_04_st_int_info[]={533};
interrupt_struct_t vin_05_st_int_info[]={534};
interrupt_struct_t vin_06_st_int_info[]={535};
interrupt_struct_t vin_07_st_int_info[]={536};
interrupt_struct_t vin_10_st_int_info[]={537};
interrupt_struct_t vin_11_st_int_info[]={538};
interrupt_struct_t vin_12_st_int_info[]={539};
interrupt_struct_t vin_13_st_int_info[]={540};
interrupt_struct_t vin_14_st_int_info[]={541};
interrupt_struct_t vin_15_st_int_info[]={542};
interrupt_struct_t vin_16_st_int_info[]={543};
interrupt_struct_t vin_17_st_int_info[]={544};
interrupt_struct_t vsp_00_st_int_info[]={546};
interrupt_struct_t csi_00_st_int_info[]={499};
interrupt_struct_t csi_01_st_int_info[]={500};
interrupt_struct_t du_00_st_int_info[]={523, 524};
interrupt_struct_t dsi_00_st_int_info[]={502,503,504,505,506,507,508,509,510};
interrupt_struct_t wwdt_00_st_int_info[]={310,311};
interrupt_struct_t wwdt_01_st_int_info[]={312,313};
interrupt_struct_t wwdt_02_st_int_info[]={314,315};
interrupt_struct_t wwdt_03_st_int_info[]={316,317};
interrupt_struct_t wwdt_04_st_int_info[]={318,319};
interrupt_struct_t wwdt_05_st_int_info[]={320,321};
interrupt_struct_t wwdt_06_st_int_info[]={322,323};
interrupt_struct_t wwdt_07_st_int_info[]={324,325};
interrupt_struct_t wwdt_08_st_int_info[]={326,327};
interrupt_struct_t wwdt_09_st_int_info[]={328,329};
interrupt_struct_t doc_00_st_int_info[]={526,527};
interrupt_struct_t rtdmac_descriptor_00_st_int_info[]={NULL};
interrupt_struct_t rtdmac_descriptor_01_st_int_info[]={NULL};
interrupt_struct_t hsm_00_st_int_info[]={NULL};
interrupt_struct_t dcm_00_st_int_info[]={NULL};
interrupt_struct_t cbf_00_st_int_info[]={NULL};
interrupt_struct_t sbf_00_st_int_info[]={NULL};
interrupt_struct_t osm_00_st_int_info[]={NULL};
interrupt_struct_t ism_00_st_int_info[]={NULL};
interrupt_struct_t ism_01_st_int_info[]={NULL};
interrupt_struct_t sdmac_00_st_int_info[]={213,214};
interrupt_struct_t fbs_00_st_int_info[]={223};
interrupt_struct_t apb_00_st_int_info[]={NULL};
interrupt_struct_t dul_00_st_int_info[]={NULL};
interrupt_struct_t ecm_00_st_int_info[]={NULL};
interrupt_struct_t system_ram_00_st_int_info[]={NULL};
interrupt_struct_t icumx_cmd_00_st_int_info[]={878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897};

/* utlbs */
/* Table 3-42 Device Configuration value for AXI bus control (For R-Car V4M) */
assignment_struct_t ims_00_st_utlb_info[]={8};
assignment_struct_t ims_01_st_utlb_info[]={9};
assignment_struct_t imr_00_st_utlb_info[]={0,4};
assignment_struct_t imr_01_st_utlb_info[]={1,5};
assignment_struct_t imp_top_00_st_utlb_info[]={16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
assignment_struct_t imp_00_st_utlb_info[]={16,17,18,19,20,21,22,23};
assignment_struct_t imp_01_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_ocv_00_st_utlb_info[]={16,17,18,19,20,21,22,23};
assignment_struct_t imp_ocv_01_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_ocv_02_st_utlb_info[]={16,17,18,19,20,21,22,23};
assignment_struct_t imp_ocv_03_st_utlb_info[]={16,17,18,19,20,21,22,23};
assignment_struct_t imp_dma_00_st_utlb_info[]={16,17,18,19,20,21,22,23};
assignment_struct_t imp_dma_01_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_psc_00_st_utlb_info[]={16,17,18,19,20,21,22,23};
assignment_struct_t imp_dta_00_st_utlb_info[]={16,17,18,19,20,21,22,23};
assignment_struct_t imp_dma_slim_00_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_dma_slim_01_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_cnn_00_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_reg_spmi_00_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_reg_spmc_00_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_spmi_00_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_spmc_00_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t imp_spmc_01_st_utlb_info[]={24,25,26,27,28,29,30,31};
assignment_struct_t vdsp00_00_st_utlb_info[]={13,14};
assignment_struct_t vdsp00_01_st_utlb_info[]={NULL};
assignment_struct_t vdsp00_02_st_utlb_info[]={NULL};
assignment_struct_t vdsp00_03_st_utlb_info[]={NULL};
assignment_struct_t vdsp00_04_st_utlb_info[]={NULL};
assignment_struct_t vdsp00_05_st_utlb_info[]={NULL};
assignment_struct_t vdsp00_06_st_utlb_info[]={NULL};
assignment_struct_t vdsp00_07_st_utlb_info[]={NULL};
assignment_struct_t vdsp00_08_st_utlb_info[]={NULL};
assignment_struct_t vdsp01_00_st_utlb_info[]={13,14};
assignment_struct_t vdsp01_01_st_utlb_info[]={NULL};
assignment_struct_t vdsp01_02_st_utlb_info[]={NULL};
assignment_struct_t vdsp01_03_st_utlb_info[]={NULL};
assignment_struct_t vdsp01_04_st_utlb_info[]={NULL};
assignment_struct_t vdsp01_05_st_utlb_info[]={NULL};
assignment_struct_t vdsp01_06_st_utlb_info[]={NULL};
assignment_struct_t vdsp01_07_st_utlb_info[]={NULL};
assignment_struct_t vdsp01_08_st_utlb_info[]={NULL};
assignment_struct_t dof_00_st_utlb_info[]={3};
assignment_struct_t smd_ps_00_st_utlb_info[]={2};
assignment_struct_t smd_post_00_st_utlb_info[]={1};
assignment_struct_t dof_fcprm_00_st_utlb_info[]={3};
assignment_struct_t vip_wrap_00_st_utlb_info[]={1,2,3};
assignment_struct_t gsx_00_st_utlb_info[]={0,1,2,3,4,5,6,7};
assignment_struct_t iccom_reg_00_st_utlb_info[]={NULL};
assignment_struct_t iccom_00_st_utlb_info[]={NULL};
assignment_struct_t iccom_01_st_utlb_info[]={NULL};
assignment_struct_t iccom_02_st_utlb_info[]={NULL};
assignment_struct_t iccom_03_st_utlb_info[]={NULL};
assignment_struct_t iccom_04_st_utlb_info[]={NULL};
assignment_struct_t iccom_05_st_utlb_info[]={NULL};
assignment_struct_t iccom_06_st_utlb_info[]={NULL};
assignment_struct_t iccom_07_st_utlb_info[]={NULL};
assignment_struct_t iccom_08_st_utlb_info[]={NULL};
assignment_struct_t iccom_09_st_utlb_info[]={NULL};
assignment_struct_t iccom_10_st_utlb_info[]={NULL};
assignment_struct_t iccom_11_st_utlb_info[]={NULL};
assignment_struct_t ivcp1e_00_st_utlb_info[]={14};
assignment_struct_t cisp_00_st_utlb_info[]={10,14,18,22,26};
assignment_struct_t tisp_00_st_utlb_info[]={NULL};
assignment_struct_t tisp_01_st_utlb_info[]={NULL};
assignment_struct_t vspx_00_st_utlb_info[]={24};
assignment_struct_t fcpvx_00_st_utlb_info[]={24};
assignment_struct_t gicd_00_st_utlb_info[]={NULL};
assignment_struct_t fbc_00_st_utlb_info[]={NULL};
assignment_struct_t fba_ims0_st_utlb_info[]={NULL};
assignment_struct_t fba_ims1_st_utlb_info[]={NULL};
assignment_struct_t fba_imr0_st_utlb_info[]={NULL};
assignment_struct_t fba_imr1_st_utlb_info[]={NULL};
assignment_struct_t fba_imp0_st_utlb_info[]={NULL};
assignment_struct_t fba_imp1_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_ocv0_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_ocv1_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_ocv2_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_ocv3_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_dp0_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_dp1_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_cnn0_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_dsp0_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_dsp1_st_utlb_info[]={NULL};
assignment_struct_t fba_imp_cnram0_st_utlb_info[]={NULL};
assignment_struct_t fba_umfl0_st_utlb_info[]={NULL};
assignment_struct_t fba_smps0_st_utlb_info[]={NULL};
assignment_struct_t fba_smpo0_st_utlb_info[]={NULL};
assignment_struct_t fba_pap0_st_utlb_info[]={NULL};
assignment_struct_t fba_isp0_st_utlb_info[]={NULL};
assignment_struct_t fba_ca76d0_st_utlb_info[]={NULL};
assignment_struct_t fba_ca760_st_utlb_info[]={NULL};
assignment_struct_t fba_ca761_st_utlb_info[]={NULL};
assignment_struct_t fba_ca762_st_utlb_info[]={NULL};
assignment_struct_t fba_ca763_st_utlb_info[]={NULL};
assignment_struct_t rfso_00_st_utlb_info[]={NULL};
assignment_struct_t rfso_01_st_utlb_info[]={NULL};
assignment_struct_t rfso_02_st_utlb_info[]={NULL};
assignment_struct_t rfso_03_st_utlb_info[]={NULL};
assignment_struct_t rfso_04_st_utlb_info[]={NULL};
assignment_struct_t rfso_05_st_utlb_info[]={NULL};
assignment_struct_t rfso_06_st_utlb_info[]={NULL};
assignment_struct_t rfso_07_st_utlb_info[]={NULL};
assignment_struct_t rfso_08_st_utlb_info[]={NULL};
assignment_struct_t rfso_09_st_utlb_info[]={NULL};
assignment_struct_t rfso_10_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_mm_00_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_vi0_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_vi1_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_vc_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_ir_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_rt0_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_rt1_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_ds0_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_hsc_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_vip0_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_vip1_st_utlb_info[]={NULL};
assignment_struct_t ipmmu_cache_3dg_st_utlb_info[]={NULL};
assignment_struct_t caiplite_wrapper_00_st_utlb_info[]={NULL};
assignment_struct_t caiplite_00_st_utlb_info[]={NULL};
assignment_struct_t caiplite_01_st_utlb_info[]={NULL};
assignment_struct_t caiplite_02_st_utlb_info[]={NULL};
assignment_struct_t caiplite_03_st_utlb_info[]={NULL};
assignment_struct_t caiplite_04_st_utlb_info[]={NULL};
assignment_struct_t caiplite_05_st_utlb_info[]={NULL};
assignment_struct_t caiplite_06_st_utlb_info[]={NULL};
assignment_struct_t caiplite_07_st_utlb_info[]={NULL};
assignment_struct_t wcrc_caiplite_00_st_utlb_info[]={NULL};
assignment_struct_t wcrc_caiplite_01_st_utlb_info[]={NULL};
assignment_struct_t wcrc_caiplite_02_st_utlb_info[]={NULL};
assignment_struct_t wcrc_caiplite_03_st_utlb_info[]={NULL};
assignment_struct_t rtdmac_control0_00_st_utlb_info[]={0};
assignment_struct_t rtdmac_control0_01_st_utlb_info[]={1};
assignment_struct_t rtdmac_control0_02_st_utlb_info[]={2};
assignment_struct_t rtdmac_control0_03_st_utlb_info[]={3};
assignment_struct_t rtdmac_control0_04_st_utlb_info[]={4};
assignment_struct_t rtdmac_control0_05_st_utlb_info[]={5};
assignment_struct_t rtdmac_control0_06_st_utlb_info[]={6};
assignment_struct_t rtdmac_control0_07_st_utlb_info[]={7};
assignment_struct_t rtdmac_control0_08_st_utlb_info[]={8};
assignment_struct_t rtdmac_control0_09_st_utlb_info[]={9};
assignment_struct_t rtdmac_control0_10_st_utlb_info[]={10};
assignment_struct_t rtdmac_control0_11_st_utlb_info[]={11};
assignment_struct_t rtdmac_control0_12_st_utlb_info[]={12};
assignment_struct_t rtdmac_control0_13_st_utlb_info[]={13};
assignment_struct_t rtdmac_control0_14_st_utlb_info[]={14};
assignment_struct_t rtdmac_control0_15_st_utlb_info[]={15};
assignment_struct_t rtdmac_control1_00_st_utlb_info[]={16};
assignment_struct_t rtdmac_control1_01_st_utlb_info[]={17};
assignment_struct_t rtdmac_control1_02_st_utlb_info[]={18};
assignment_struct_t rtdmac_control1_03_st_utlb_info[]={19};
assignment_struct_t rtdmac_control1_04_st_utlb_info[]={20};
assignment_struct_t rtdmac_control1_05_st_utlb_info[]={21};
assignment_struct_t rtdmac_control1_06_st_utlb_info[]={22};
assignment_struct_t rtdmac_control1_07_st_utlb_info[]={23};
assignment_struct_t rtdmac_control1_08_st_utlb_info[]={24};
assignment_struct_t rtdmac_control1_09_st_utlb_info[]={25};
assignment_struct_t rtdmac_control1_10_st_utlb_info[]={26};
assignment_struct_t rtdmac_control1_11_st_utlb_info[]={27};
assignment_struct_t rtdmac_control1_12_st_utlb_info[]={28};
assignment_struct_t rtdmac_control1_13_st_utlb_info[]={29};
assignment_struct_t rtdmac_control1_14_st_utlb_info[]={30};
assignment_struct_t rtdmac_control1_15_st_utlb_info[]={31};
assignment_struct_t vin_00_st_utlb_info[]={0};
assignment_struct_t vin_01_st_utlb_info[]={0};
assignment_struct_t vin_02_st_utlb_info[]={0};
assignment_struct_t vin_03_st_utlb_info[]={0};
assignment_struct_t vin_04_st_utlb_info[]={0};
assignment_struct_t vin_05_st_utlb_info[]={0};
assignment_struct_t vin_06_st_utlb_info[]={0};
assignment_struct_t vin_07_st_utlb_info[]={0};
assignment_struct_t vin_10_st_utlb_info[]={1};
assignment_struct_t vin_11_st_utlb_info[]={1};
assignment_struct_t vin_12_st_utlb_info[]={1};
assignment_struct_t vin_13_st_utlb_info[]={1};
assignment_struct_t vin_14_st_utlb_info[]={1};
assignment_struct_t vin_15_st_utlb_info[]={1};
assignment_struct_t vin_16_st_utlb_info[]={1};
assignment_struct_t vin_17_st_utlb_info[]={1};
assignment_struct_t vsp_00_st_utlb_info[]={6};
assignment_struct_t csi_00_st_utlb_info[]={NULL};
assignment_struct_t csi_01_st_utlb_info[]={NULL};
assignment_struct_t du_00_st_utlb_info[]={NULL};
assignment_struct_t dsi_00_st_utlb_info[]={18};
assignment_struct_t wwdt_00_st_utlb_info[]={NULL};
assignment_struct_t wwdt_01_st_utlb_info[]={NULL};
assignment_struct_t wwdt_02_st_utlb_info[]={NULL};
assignment_struct_t wwdt_03_st_utlb_info[]={NULL};
assignment_struct_t wwdt_04_st_utlb_info[]={NULL};
assignment_struct_t wwdt_05_st_utlb_info[]={NULL};
assignment_struct_t wwdt_06_st_utlb_info[]={NULL};
assignment_struct_t wwdt_07_st_utlb_info[]={NULL};
assignment_struct_t wwdt_08_st_utlb_info[]={NULL};
assignment_struct_t wwdt_09_st_utlb_info[]={NULL};
assignment_struct_t doc_00_st_utlb_info[]={NULL};
assignment_struct_t rtdmac_descriptor0_00_st_utlb_info[]={NULL};
assignment_struct_t rtdmac_descriptor1_00_st_utlb_info[]={NULL};
assignment_struct_t hsm_00_st_utlb_info[]={16};
assignment_struct_t dcm_00_st_utlb_info[]={16};
assignment_struct_t cbf_00_st_utlb_info[]={16};
assignment_struct_t sbf_00_st_utlb_info[]={16};
assignment_struct_t osm_00_st_utlb_info[]={16};
assignment_struct_t ism_00_st_utlb_info[]={16};
assignment_struct_t ism_01_st_utlb_info[]={16};
assignment_struct_t sdmac_00_st_utlb_info[]={16};
assignment_struct_t fbs_00_st_utlb_info[]={16};
assignment_struct_t apb_00_st_utlb_info[]={16};
assignment_struct_t dul_00_st_utlb_info[]={16};
assignment_struct_t ecm_00_st_utlb_info[]={NULL};
assignment_struct_t system_ram_00_st_utlb_info={NULL};
assignment_struct_t icumx_cmd_00_st_utlb_info[]={1,2};
assignment_struct_t OSAL_DEVICE_INVALID_ID_st_utlb_info[]={NULL};

/* Table 3-40 Device Configuration value for Power control (For R-Car V4M) */
/* PMA UM 1.2.2.2.4 e_pma_hwa_id_t(V4M) */
power_struct_t ims_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IMS0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ims_01_st_pm_info[]={R_PMA_HWA_ID_V4M_IMS1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imr_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IMR0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imr_01_st_pm_info[]={R_PMA_HWA_ID_V4M_IMR1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_top_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IMPSLV, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IMP0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_01_st_pm_info[]={R_PMA_HWA_ID_V4M_IMP1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_ocv_00_st_pm_info[]={R_PMA_HWA_ID_V4M_CVE0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_ocv_01_st_pm_info[]={R_PMA_HWA_ID_V4M_CVE1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_ocv_02_st_pm_info[]={R_PMA_HWA_ID_V4M_CVE2, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_ocv_03_st_pm_info[]={R_PMA_HWA_ID_V4M_CVE3, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_dma_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IMPDMAC0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_dma_01_st_pm_info[]={R_PMA_HWA_ID_V4M_IMPDMAC1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_psc_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IMPPSC, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_dta_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IMPDTA, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_dma_slim_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_dma_slim_01_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_cnn_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IMPCNN, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_reg_spmi_00_st_pm_info[]={R_PMA_HWA_ID_V4M_REG_SPMI, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_reg_spmc_00_st_pm_info[]={R_PMA_HWA_ID_V4M_REG_SPMC, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_spmi_00_st_pm_info[]={R_PMA_HWA_ID_V4M_SPMI, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_spmc_00_st_pm_info[]={R_PMA_HWA_ID_V4M_SPMC, OSAL_PM_POSTCKCR_INVALID};
power_struct_t imp_spmc_01_st_pm_info[]={R_PMA_HWA_ID_V4M_SPMC01, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_00_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_CSB, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_01_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_BUS, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_02_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_REG, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_03_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_GLOBAL, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_04_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_SYS, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_05_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_OCEM, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_06_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_APBDBG, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_07_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_SYSPO, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp00_08_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP0_BUS_RESET, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_00_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_CSB, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_01_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_BUS, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_02_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_REG, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_03_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_GLOBAL, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_04_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_SYS, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_05_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_OCEM, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_06_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_APBDBG, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_07_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_SYSPO, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vdsp01_08_st_pm_info[]={R_PMA_HWA_ID_V4M_VDSP1_BUS_RESET, OSAL_PM_POSTCKCR_INVALID};
power_struct_t dof_00_st_pm_info[]={R_PMA_HWA_ID_V4M_DOF, OSAL_PM_POST4CKCR};
power_struct_t smd_ps_00_st_pm_info[]={R_PMA_HWA_ID_V4M_SMPS0, OSAL_PM_POST4CKCR};
power_struct_t smd_post_00_st_pm_info[]={R_PMA_HWA_ID_V4M_SMPO0, OSAL_PM_POST4CKCR};
power_struct_t dof_fcprm_00_st_pm_info[]={R_PMA_HWA_ID_V4M_DOF_FCPRM, OSAL_PM_POST4CKCR};
power_struct_t vip_wrap_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST4CKCR};
power_struct_t gsx_00_st_pm_info[]={R_PMA_HWA_ID_V4M_RGX, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_reg_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_01_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_02_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_03_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_04_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_05_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_06_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_07_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_08_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_09_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_10_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_11_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ivcp1e_00_st_pm_info[]={R_PMA_HWA_ID_V4M_IVCP1E, OSAL_PM_POSTCKCR_INVALID};
power_struct_t cisp_00_st_pm_info[]={R_PMA_HWA_ID_V4M_ISP0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t tisp_00_st_pm_info[]={R_PMA_HWA_ID_V4M_ISPCS0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t tisp_01_st_pm_info[]={R_PMA_HWA_ID_V4M_ISPCS1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vspx_00_st_pm_info[]={R_PMA_HWA_ID_V4M_VSPX0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t fcpvx_00_st_pm_info[]={R_PMA_HWA_ID_V4M_FCPVX0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t gicd_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t fbc_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR};
power_struct_t fba_ims0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMS0, OSAL_PM_POSTCKCR};
power_struct_t fba_ims1_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMS1, OSAL_PM_POSTCKCR};
power_struct_t fba_imr0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMR0, OSAL_PM_POSTCKCR};
power_struct_t fba_imr1_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMR1, OSAL_PM_POSTCKCR};
power_struct_t fba_imp0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP0, OSAL_PM_POSTCKCR};
power_struct_t fba_imp1_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP1, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_ocv0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_OCV0, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_ocv1_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_OCV1, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_ocv2_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_OCV2, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_ocv3_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_OCV3, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_dp0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_DP0, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_dp1_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_DP1, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_cnn0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_CNN0, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_dsp0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_DSP0, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_dsp1_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_DSP1, OSAL_PM_POSTCKCR};
power_struct_t fba_imp_cnram0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_IMP_CNRAM0, OSAL_PM_POSTCKCR};
power_struct_t fba_umfl0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_UMFL0, OSAL_PM_POST4CKCR};
power_struct_t fba_smps0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_SMPS0, OSAL_PM_POST4CKCR};
power_struct_t fba_smpo0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_SMPO0, OSAL_PM_POST4CKCR};
power_struct_t fba_pap0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_PAP0, OSAL_PM_POST4CKCR};
power_struct_t fba_isp0_st_pm_info[]={R_PMA_HWA_ID_V4M_FBA_ISP0, OSAL_PM_POSTCKCR};
power_struct_t fba_ca76d0_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca760_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca761_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca762_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca763_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t rfso_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_01_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_02_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_03_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_04_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_05_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_06_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_07_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_08_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_09_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rfso_10_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_mm_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_vi0_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_vi1_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_vc_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_ir_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_rt0_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_rt1_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_ds0_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_hsc_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_vip0_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_vip1_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ipmmu_cache_3dg_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_wrapper_00_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC_WRAP, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_00_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_01_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_02_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC2, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_03_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC3, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_04_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC4, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_05_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC5, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_06_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC6, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_07_st_pm_info[]={R_PMA_HWA_ID_V4M_AES_ACC7, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wcrc_caiplite_00_st_pm_info[]={R_PMA_HWA_ID_V4M_WCRC0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wcrc_caiplite_01_st_pm_info[]={R_PMA_HWA_ID_V4M_WCRC1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wcrc_caiplite_02_st_pm_info[]={R_PMA_HWA_ID_V4M_WCRC2, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wcrc_caiplite_03_st_pm_info[]={R_PMA_HWA_ID_V4M_WCRC3, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_01_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_02_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_03_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_04_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_05_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_06_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_07_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_08_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_09_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_10_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_11_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_12_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_13_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_14_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control0_15_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_01_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_02_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_03_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_04_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_05_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_06_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_07_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_08_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_09_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_10_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_11_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_12_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_13_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_14_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control1_15_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_00_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN00, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_01_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN01, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_02_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN02, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_03_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN03, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_04_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN04, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_05_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN05, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_06_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN06, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_07_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN07, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_10_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN08, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_11_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN09, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_12_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN10, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_13_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN11, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_14_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN12, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_15_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN13, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_16_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN14, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vin_17_st_pm_info[]={R_PMA_HWA_ID_V4M_VIN15, OSAL_PM_POSTCKCR_INVALID};
power_struct_t vsp_00_st_pm_info[]={R_PMA_HWA_ID_V4M_VSPD0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t csi_00_st_pm_info[]={R_PMA_HWA_ID_V4M_CSITOP0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t csi_01_st_pm_info[]={R_PMA_HWA_ID_V4M_CSITOP1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t du_00_st_pm_info[]={R_PMA_HWA_ID_V4M_DIS0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t dsi_00_st_pm_info[]={R_PMA_HWA_ID_V4M_DSITXLINK0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_01_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_02_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_03_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_04_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_05_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_06_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_07_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_08_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wwdt_09_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t doc_00_st_pm_info[]={R_PMA_HWA_ID_V4M_DOC2CH, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_descriptor0_00_st_pm_info[]={R_PMA_HWA_ID_V4M_RTDM0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_descriptor1_00_st_pm_info[]={R_PMA_HWA_ID_V4M_RTDM1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t hsm_00_st_pm_info[]={R_PMA_HWA_ID_V4M_HSM, OSAL_PM_POSTCKCR_INVALID};
power_struct_t dcm_00_st_pm_info[]={R_PMA_HWA_ID_V4M_DCM, OSAL_PM_POSTCKCR_INVALID};
power_struct_t cbf_00_st_pm_info[]={R_PMA_HWA_ID_V4M_CBF, OSAL_PM_POSTCKCR_INVALID};
power_struct_t sbf_00_st_pm_info[]={R_PMA_HWA_ID_V4M_SBF, OSAL_PM_POSTCKCR_INVALID};
power_struct_t osm_00_st_pm_info[]={R_PMA_HWA_ID_V4M_OSM, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ism_00_st_pm_info[]={R_PMA_HWA_ID_V4M_ISM0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ism_01_st_pm_info[]={R_PMA_HWA_ID_V4M_ISM1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t sdmac_00_st_pm_info[]={R_PMA_HWA_ID_V4M_PAPSDMA, OSAL_PM_POSTCKCR_INVALID};
power_struct_t fbs_00_st_pm_info[]={R_PMA_HWA_ID_V4M_FBS, OSAL_PM_POSTCKCR_INVALID};
power_struct_t apb_00_st_pm_info[]={R_PMA_HWA_ID_V4M_APB, OSAL_PM_POSTCKCR_INVALID};
power_struct_t dul_00_st_pm_info[]={R_PMA_HWA_ID_V4M_DUL, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ecm_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t system_ram_00_st_pm_info={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t icumx_cmd_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};

/* Table 3-36 Device Configuration value for IO access (For R-Car V4M) */
struct expect_config test[] =
{
    {"ims_00","ims",0xFE860000U,0x00010000U,1U,sizeof(ims_00_st_int_info)/sizeof(interrupt_struct_t),&ims_00_st_int_info,"vc",sizeof(ims_00_st_utlb_info)/sizeof(assignment_struct_t),&ims_00_st_utlb_info,&ims_00_st_pm_info},
    {"ims_01","ims",0xFE870000U,0x00010000U,1U,sizeof(ims_01_st_int_info)/sizeof(interrupt_struct_t),&ims_01_st_int_info,"vc",sizeof(ims_01_st_utlb_info)/sizeof(assignment_struct_t),&ims_01_st_utlb_info,&ims_01_st_pm_info},
    {"imr_00","imr",0xFE880000U,0x00010000U,1U,sizeof(imr_00_st_int_info)/sizeof(interrupt_struct_t),&imr_00_st_int_info,"vc",sizeof(imr_00_st_utlb_info)/sizeof(assignment_struct_t),&imr_00_st_utlb_info,&imr_00_st_pm_info},
    {"imr_01","imr",0xFE890000U,0x00010000U,1U,sizeof(imr_01_st_int_info)/sizeof(interrupt_struct_t),&imr_01_st_int_info,"vc",sizeof(imr_01_st_utlb_info)/sizeof(assignment_struct_t),&imr_01_st_utlb_info,&imr_01_st_pm_info},
    {"imp_top_00","imp_top",0xFF900000U,0x00006000U,3U,sizeof(imp_top_00_st_int_info)/sizeof(interrupt_struct_t),&imp_top_00_st_int_info,"ir",sizeof(imp_top_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_top_00_st_utlb_info,&imp_top_00_st_pm_info},
    {"imp_00","imp",0xFFA00000U,0x00020000U,0U,sizeof(imp_00_st_int_info)/sizeof(interrupt_struct_t),&imp_00_st_int_info,"ir",sizeof(imp_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_00_st_utlb_info,&imp_00_st_pm_info},
    {"imp_01","imp",0xFFA20000U,0x00020000U,0U,sizeof(imp_01_st_int_info)/sizeof(interrupt_struct_t),&imp_01_st_int_info,"ir",sizeof(imp_01_st_utlb_info)/sizeof(assignment_struct_t),&imp_01_st_utlb_info,&imp_01_st_pm_info},
    {"imp_ocv_00","imp_ocv",0xFFA40000U,0x00010000U,0U,sizeof(imp_ocv_00_st_int_info)/sizeof(interrupt_struct_t),&imp_ocv_00_st_int_info,"ir",sizeof(imp_ocv_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_ocv_00_st_utlb_info,&imp_ocv_00_st_pm_info},
    {"imp_ocv_01","imp_ocv",0xFFA50000U,0x00010000U,0U,sizeof(imp_ocv_01_st_int_info)/sizeof(interrupt_struct_t),&imp_ocv_01_st_int_info,"ir",sizeof(imp_ocv_01_st_utlb_info)/sizeof(assignment_struct_t),&imp_ocv_01_st_utlb_info,&imp_ocv_01_st_pm_info},
    {"imp_ocv_02","imp_ocv",0xFFA60000U,0x00010000U,0U,sizeof(imp_ocv_02_st_int_info)/sizeof(interrupt_struct_t),&imp_ocv_02_st_int_info,"ir",sizeof(imp_ocv_02_st_utlb_info)/sizeof(assignment_struct_t),&imp_ocv_02_st_utlb_info,&imp_ocv_02_st_pm_info},
    {"imp_ocv_03","imp_ocv",0xFFA70000U,0x00010000U,0U,sizeof(imp_ocv_03_st_int_info)/sizeof(interrupt_struct_t),&imp_ocv_03_st_int_info,"ir",sizeof(imp_ocv_03_st_utlb_info)/sizeof(assignment_struct_t),&imp_ocv_03_st_utlb_info,&imp_ocv_03_st_pm_info},
    {"imp_dma_00","imp_dma",0xFFA80000U,0x00004000U,0U,sizeof(imp_dma_00_st_int_info)/sizeof(interrupt_struct_t),&imp_dma_00_st_int_info,"ir",sizeof(imp_dma_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_dma_00_st_utlb_info,&imp_dma_00_st_pm_info},
    {"imp_dma_01","imp_dma",0xFFB80000U,0x00004000U,0U,sizeof(imp_dma_01_st_int_info)/sizeof(interrupt_struct_t),&imp_dma_01_st_int_info,"ir",sizeof(imp_dma_01_st_utlb_info)/sizeof(assignment_struct_t),&imp_dma_01_st_utlb_info,&imp_dma_01_st_pm_info},
    {"imp_psc_00","imp_psc",0xFFA84000U,0x00004000U,0U,sizeof(imp_psc_00_st_int_info)/sizeof(interrupt_struct_t),&imp_psc_00_st_int_info,"ir",sizeof(imp_psc_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_psc_00_st_utlb_info,&imp_psc_00_st_pm_info},
    {"imp_dta_00","imp_dta",0xFFA88000U,0x00004000U,0U,sizeof(imp_dta_00_st_int_info)/sizeof(interrupt_struct_t),&imp_dta_00_st_int_info,"ir",sizeof(imp_dta_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_dta_00_st_utlb_info,&imp_dta_00_st_pm_info},
    {"imp_dma_slim_00","imp_dma_slim",0xFFA90000U,0x00004000U,0U,sizeof(imp_dma_slim_00_st_int_info)/sizeof(interrupt_struct_t),&imp_dma_slim_00_st_int_info,"ir",sizeof(imp_dma_slim_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_dma_slim_00_st_utlb_info,&imp_dma_slim_00_st_pm_info},
    {"imp_dma_slim_01","imp_dma_slim",0xFFA94000U,0x00004000U,0U,sizeof(imp_dma_slim_01_st_int_info)/sizeof(interrupt_struct_t),&imp_dma_slim_01_st_int_info,"ir",sizeof(imp_dma_slim_01_st_utlb_info)/sizeof(assignment_struct_t),&imp_dma_slim_01_st_utlb_info,&imp_dma_slim_01_st_pm_info},
    {"imp_cnn_00","imp_cnn",0xFFAA0000U,0x00010000U,0U,sizeof(imp_cnn_00_st_int_info)/sizeof(interrupt_struct_t),&imp_cnn_00_st_int_info,"ir",sizeof(imp_cnn_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_cnn_00_st_utlb_info,&imp_cnn_00_st_pm_info},
    {"imp_reg_spmi_00","imp_reg_spmi",0xFFA8C000U,0x00004000U,0U,sizeof(imp_reg_spmi_00_st_int_info)/sizeof(interrupt_struct_t),&imp_reg_spmi_00_st_int_info,"ir",sizeof(imp_reg_spmi_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_reg_spmi_00_st_utlb_info,&imp_reg_spmi_00_st_pm_info},
    {"imp_reg_spmc_00","imp_reg_spmc",0xFFAB0000U,0x00004000U,0U,sizeof(imp_reg_spmc_00_st_int_info)/sizeof(interrupt_struct_t),&imp_reg_spmc_00_st_int_info,"ir",sizeof(imp_reg_spmc_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_reg_spmc_00_st_utlb_info,&imp_reg_spmc_00_st_pm_info},
    {"imp_spmi_00","imp_spmi",0xED200000U,0x00100000U,0U,sizeof(imp_spmi_00_st_int_info)/sizeof(interrupt_struct_t),&imp_spmi_00_st_int_info,"ir",sizeof(imp_spmi_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_spmi_00_st_utlb_info,&imp_spmi_00_st_pm_info},
    {"imp_spmc_00","imp_spmc",0xED400000U,0x00200000U,0U,sizeof(imp_spmc_00_st_int_info)/sizeof(interrupt_struct_t),&imp_spmc_00_st_int_info,"ir",sizeof(imp_spmc_00_st_utlb_info)/sizeof(assignment_struct_t),&imp_spmc_00_st_utlb_info,&imp_spmc_00_st_pm_info},
    {"imp_spmc_01","imp_spmc",0xED600000U,0x00080000U,0U,sizeof(imp_spmc_01_st_int_info)/sizeof(interrupt_struct_t),&imp_spmc_01_st_int_info,"ir",sizeof(imp_spmc_01_st_utlb_info)/sizeof(assignment_struct_t),&imp_spmc_01_st_utlb_info,&imp_spmc_01_st_pm_info},
    {"vdsp00_00","vdsp00",0xF1400000U,0x001A0000U,5U,sizeof(vdsp00_00_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_00_st_int_info,"ir",sizeof(vdsp00_00_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_00_st_utlb_info,&vdsp00_00_st_pm_info},
    {"vdsp00_01","vdsp00",0x00000000U,0x00000000U,0U,sizeof(vdsp00_01_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_01_st_int_info,"invalid",sizeof(vdsp00_01_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_01_st_utlb_info,&vdsp00_01_st_pm_info},
    {"vdsp00_02","vdsp00",0x00000000U,0x00000000U,0U,sizeof(vdsp00_02_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_02_st_int_info,"invalid",sizeof(vdsp00_02_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_02_st_utlb_info,&vdsp00_02_st_pm_info},
    {"vdsp00_03","vdsp00",0x00000000U,0x00000000U,0U,sizeof(vdsp00_03_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_03_st_int_info,"invalid",sizeof(vdsp00_03_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_03_st_utlb_info,&vdsp00_03_st_pm_info},
    {"vdsp00_04","vdsp00",0x00000000U,0x00000000U,0U,sizeof(vdsp00_04_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_04_st_int_info,"invalid",sizeof(vdsp00_04_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_04_st_utlb_info,&vdsp00_04_st_pm_info},
    {"vdsp00_05","vdsp00",0x00000000U,0x00000000U,0U,sizeof(vdsp00_05_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_05_st_int_info,"invalid",sizeof(vdsp00_05_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_05_st_utlb_info,&vdsp00_05_st_pm_info},
    {"vdsp00_06","vdsp00",0x00000000U,0x00000000U,0U,sizeof(vdsp00_06_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_06_st_int_info,"invalid",sizeof(vdsp00_06_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_06_st_utlb_info,&vdsp00_06_st_pm_info},
    {"vdsp00_07","vdsp00",0x00000000U,0x00000000U,0U,sizeof(vdsp00_07_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_07_st_int_info,"invalid",sizeof(vdsp00_07_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_07_st_utlb_info,&vdsp00_07_st_pm_info},
    {"vdsp00_08","vdsp00",0x00000000U,0x00000000U,0U,sizeof(vdsp00_08_st_int_info)/sizeof(interrupt_struct_t),&vdsp00_08_st_int_info,"invalid",sizeof(vdsp00_08_st_utlb_info)/sizeof(assignment_struct_t),&vdsp00_08_st_utlb_info,&vdsp00_08_st_pm_info},
    {"vdsp01_00","vdsp01",0xF1600000U,0x001A0000U,5U,sizeof(vdsp01_00_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_00_st_int_info,"ir",sizeof(vdsp01_00_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_00_st_utlb_info,&vdsp01_00_st_pm_info},
    {"vdsp01_01","vdsp01",0x00000000U,0x00000000U,0U,sizeof(vdsp01_01_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_01_st_int_info,"invalid",sizeof(vdsp01_01_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_01_st_utlb_info,&vdsp01_01_st_pm_info},
    {"vdsp01_02","vdsp01",0x00000000U,0x00000000U,0U,sizeof(vdsp01_02_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_02_st_int_info,"invalid",sizeof(vdsp01_02_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_02_st_utlb_info,&vdsp01_02_st_pm_info},
    {"vdsp01_03","vdsp01",0x00000000U,0x00000000U,0U,sizeof(vdsp01_03_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_03_st_int_info,"invalid",sizeof(vdsp01_03_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_03_st_utlb_info,&vdsp01_03_st_pm_info},
    {"vdsp01_04","vdsp01",0x00000000U,0x00000000U,0U,sizeof(vdsp01_04_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_04_st_int_info,"invalid",sizeof(vdsp01_04_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_04_st_utlb_info,&vdsp01_04_st_pm_info},
    {"vdsp01_05","vdsp01",0x00000000U,0x00000000U,0U,sizeof(vdsp01_05_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_05_st_int_info,"invalid",sizeof(vdsp01_05_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_05_st_utlb_info,&vdsp01_05_st_pm_info},
    {"vdsp01_06","vdsp01",0x00000000U,0x00000000U,0U,sizeof(vdsp01_06_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_06_st_int_info,"invalid",sizeof(vdsp01_06_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_06_st_utlb_info,&vdsp01_06_st_pm_info},
    {"vdsp01_07","vdsp01",0x00000000U,0x00000000U,0U,sizeof(vdsp01_07_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_07_st_int_info,"invalid",sizeof(vdsp01_07_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_07_st_utlb_info,&vdsp01_07_st_pm_info},
    {"vdsp01_08","vdsp01",0x00000000U,0x00000000U,0U,sizeof(vdsp01_08_st_int_info)/sizeof(interrupt_struct_t),&vdsp01_08_st_int_info,"invalid",sizeof(vdsp01_08_st_utlb_info)/sizeof(assignment_struct_t),&vdsp01_08_st_utlb_info,&vdsp01_08_st_pm_info},
    {"dof_00","dof",0xE7A10000U,0x00010000U,2U,sizeof(dof_00_st_int_info)/sizeof(interrupt_struct_t),&dof_00_st_int_info,"vip0",sizeof(dof_00_st_utlb_info)/sizeof(assignment_struct_t),&dof_00_st_utlb_info,&dof_00_st_pm_info},
    {"smd_ps_00","smd_ps",0xE7A20000U,0x00010000U,2U,sizeof(smd_ps_00_st_int_info)/sizeof(interrupt_struct_t),&smd_ps_00_st_int_info,"vip0",sizeof(smd_ps_00_st_utlb_info)/sizeof(assignment_struct_t),&smd_ps_00_st_utlb_info,&smd_ps_00_st_pm_info},
    {"smd_post_00","smd_post",0xE7A40000U,0x00010000U,2U,sizeof(smd_post_00_st_int_info)/sizeof(interrupt_struct_t),&smd_post_00_st_int_info,"vip0",sizeof(smd_post_00_st_utlb_info)/sizeof(assignment_struct_t),&smd_post_00_st_utlb_info,&smd_post_00_st_pm_info},
    {"dof_fcprm_00","dof_fcprm",0xE7AB1000U,0x00001000U,0U,sizeof(dof_fcprm_00_st_int_info)/sizeof(interrupt_struct_t),&dof_fcprm_00_st_int_info,"vip0",sizeof(dof_fcprm_00_st_utlb_info)/sizeof(assignment_struct_t),&dof_fcprm_00_st_utlb_info,&dof_fcprm_00_st_pm_info},
    {"vip_wrap_00","vip_wrap",0xE7B3B000U,0x00001000U,0U,sizeof(vip_wrap_00_st_int_info)/sizeof(interrupt_struct_t),&vip_wrap_00_st_int_info,"vip0",sizeof(vip_wrap_00_st_utlb_info)/sizeof(assignment_struct_t),&vip_wrap_00_st_utlb_info,&vip_wrap_00_st_pm_info},
    {"gsx_00","gsx",0xFD000000U,0x00800000U,1U,sizeof(gsx_00_st_int_info)/sizeof(interrupt_struct_t),&gsx_00_st_int_info,"3dg",sizeof(gsx_00_st_utlb_info)/sizeof(assignment_struct_t),&gsx_00_st_utlb_info,&gsx_00_st_pm_info},
    {"iccom_reg_00","iccom_reg",0xE6260000U,0x00010000U,0U,sizeof(iccom_reg_00_st_int_info)/sizeof(interrupt_struct_t),&iccom_reg_00_st_int_info,"invalid",sizeof(iccom_reg_00_st_utlb_info)/sizeof(assignment_struct_t),&iccom_reg_00_st_utlb_info,&iccom_reg_00_st_pm_info},
    {"iccom_00","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_00_st_int_info)/sizeof(interrupt_struct_t),&iccom_00_st_int_info,"invalid",sizeof(iccom_00_st_utlb_info)/sizeof(assignment_struct_t),&iccom_00_st_utlb_info,&iccom_00_st_pm_info},
    {"iccom_01","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_01_st_int_info)/sizeof(interrupt_struct_t),&iccom_01_st_int_info,"invalid",sizeof(iccom_01_st_utlb_info)/sizeof(assignment_struct_t),&iccom_01_st_utlb_info,&iccom_01_st_pm_info},
    {"iccom_02","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_02_st_int_info)/sizeof(interrupt_struct_t),&iccom_02_st_int_info,"invalid",sizeof(iccom_02_st_utlb_info)/sizeof(assignment_struct_t),&iccom_02_st_utlb_info,&iccom_02_st_pm_info},
    {"iccom_03","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_03_st_int_info)/sizeof(interrupt_struct_t),&iccom_03_st_int_info,"invalid",sizeof(iccom_03_st_utlb_info)/sizeof(assignment_struct_t),&iccom_03_st_utlb_info,&iccom_03_st_pm_info},
    {"iccom_04","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_04_st_int_info)/sizeof(interrupt_struct_t),&iccom_04_st_int_info,"invalid",sizeof(iccom_04_st_utlb_info)/sizeof(assignment_struct_t),&iccom_04_st_utlb_info,&iccom_04_st_pm_info},
    {"iccom_05","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_05_st_int_info)/sizeof(interrupt_struct_t),&iccom_05_st_int_info,"invalid",sizeof(iccom_05_st_utlb_info)/sizeof(assignment_struct_t),&iccom_05_st_utlb_info,&iccom_05_st_pm_info},
    {"iccom_06","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_06_st_int_info)/sizeof(interrupt_struct_t),&iccom_06_st_int_info,"invalid",sizeof(iccom_06_st_utlb_info)/sizeof(assignment_struct_t),&iccom_06_st_utlb_info,&iccom_06_st_pm_info},
    {"iccom_07","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_07_st_int_info)/sizeof(interrupt_struct_t),&iccom_07_st_int_info,"invalid",sizeof(iccom_07_st_utlb_info)/sizeof(assignment_struct_t),&iccom_07_st_utlb_info,&iccom_07_st_pm_info},
    {"iccom_08","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_08_st_int_info)/sizeof(interrupt_struct_t),&iccom_08_st_int_info,"invalid",sizeof(iccom_08_st_utlb_info)/sizeof(assignment_struct_t),&iccom_08_st_utlb_info,&iccom_08_st_pm_info},
    {"iccom_09","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_09_st_int_info)/sizeof(interrupt_struct_t),&iccom_09_st_int_info,"invalid",sizeof(iccom_09_st_utlb_info)/sizeof(assignment_struct_t),&iccom_09_st_utlb_info,&iccom_09_st_pm_info},
    {"iccom_10","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_10_st_int_info)/sizeof(interrupt_struct_t),&iccom_10_st_int_info,"invalid",sizeof(iccom_10_st_utlb_info)/sizeof(assignment_struct_t),&iccom_10_st_utlb_info,&iccom_10_st_pm_info},
    {"iccom_11","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_11_st_int_info)/sizeof(interrupt_struct_t),&iccom_11_st_int_info,"invalid",sizeof(iccom_11_st_utlb_info)/sizeof(assignment_struct_t),&iccom_11_st_utlb_info,&iccom_11_st_pm_info},
    {"ivcp1e_00","ivcp1e",0xFEA00000U,0x00010000U,2U,sizeof(ivcp1e_00_st_int_info)/sizeof(interrupt_struct_t),&ivcp1e_00_st_int_info,"vc",sizeof(ivcp1e_00_st_utlb_info)/sizeof(assignment_struct_t),&ivcp1e_00_st_utlb_info,&ivcp1e_00_st_pm_info},
    {"cisp_00","cisp",0xFEC00000U,0x00100000U,1U,sizeof(cisp_00_st_int_info)/sizeof(interrupt_struct_t),&cisp_00_st_int_info,"vi0",sizeof(cisp_00_st_utlb_info)/sizeof(assignment_struct_t),&cisp_00_st_utlb_info,&cisp_00_st_pm_info},
    {"tisp_00","tisp",0xFED00000U,0x00010000U,1U,sizeof(tisp_00_st_int_info)/sizeof(interrupt_struct_t),&tisp_00_st_int_info,"invalid",sizeof(tisp_00_st_utlb_info)/sizeof(assignment_struct_t),&tisp_00_st_utlb_info,&tisp_00_st_pm_info},
    {"tisp_01","tisp",0xFED20000U,0x00010000U,1U,sizeof(tisp_01_st_int_info)/sizeof(interrupt_struct_t),&tisp_01_st_int_info,"invalid",sizeof(tisp_01_st_utlb_info)/sizeof(assignment_struct_t),&tisp_01_st_utlb_info,&tisp_01_st_pm_info},
    {"vspx_00","vspx",0xFEDD0000U,0x00008000U,1U,sizeof(vspx_00_st_int_info)/sizeof(interrupt_struct_t),&vspx_00_st_int_info,"vi1",sizeof(vspx_00_st_utlb_info)/sizeof(assignment_struct_t),&vspx_00_st_utlb_info,&vspx_00_st_pm_info},
    {"fcpvx_00","fcpvx",0xFEDB0000U,0x00008000U,0U,sizeof(fcpvx_00_st_int_info)/sizeof(interrupt_struct_t),&fcpvx_00_st_int_info,"vi1",sizeof(fcpvx_00_st_utlb_info)/sizeof(assignment_struct_t),&fcpvx_00_st_utlb_info,&fcpvx_00_st_pm_info},
    {"gicd_00","gicd",0xF1000000U,0x00010000U,0U,sizeof(gicd_00_st_int_info)/sizeof(interrupt_struct_t),&gicd_00_st_int_info,"invalid",sizeof(gicd_00_st_utlb_info)/sizeof(assignment_struct_t),&gicd_00_st_utlb_info,&gicd_00_st_pm_info},
    {"fbc_00","fbc",0xFF830000U,0x00002000U,1U,sizeof(fbc_00_st_int_info)/sizeof(interrupt_struct_t),&fbc_00_st_int_info,"invalid",sizeof(fbc_00_st_utlb_info)/sizeof(assignment_struct_t),&fbc_00_st_utlb_info,&fbc_00_st_pm_info},
    {"fba_ims0","fba",0xFE605000U,0x00001000U,1U,sizeof(fba_ims0_st_int_info)/sizeof(interrupt_struct_t),&fba_ims0_st_int_info,"invalid",sizeof(fba_ims0_st_utlb_info)/sizeof(assignment_struct_t),&fba_ims0_st_utlb_info,&fba_ims0_st_pm_info},
    {"fba_ims1","fba",0xFE606000U,0x00001000U,1U,sizeof(fba_ims1_st_int_info)/sizeof(interrupt_struct_t),&fba_ims1_st_int_info,"invalid",sizeof(fba_ims1_st_utlb_info)/sizeof(assignment_struct_t),&fba_ims1_st_utlb_info,&fba_ims1_st_pm_info},
    {"fba_imr0","fba",0xFE601000U,0x00001000U,1U,sizeof(fba_imr0_st_int_info)/sizeof(interrupt_struct_t),&fba_imr0_st_int_info,"invalid",sizeof(fba_imr0_st_utlb_info)/sizeof(assignment_struct_t),&fba_imr0_st_utlb_info,&fba_imr0_st_pm_info},
    {"fba_imr1","fba",0xFE602000U,0x00001000U,1U,sizeof(fba_imr1_st_int_info)/sizeof(interrupt_struct_t),&fba_imr1_st_int_info,"invalid",sizeof(fba_imr1_st_utlb_info)/sizeof(assignment_struct_t),&fba_imr1_st_utlb_info,&fba_imr1_st_pm_info},
    {"fba_imp0","fba",0xFF8C0000U,0x00001000U,1U,sizeof(fba_imp0_st_int_info)/sizeof(interrupt_struct_t),&fba_imp0_st_int_info,"invalid",sizeof(fba_imp0_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp0_st_utlb_info,&fba_imp0_st_pm_info},
    {"fba_imp1","fba",0xFF8C1000U,0x00001000U,1U,sizeof(fba_imp1_st_int_info)/sizeof(interrupt_struct_t),&fba_imp1_st_int_info,"invalid",sizeof(fba_imp1_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp1_st_utlb_info,&fba_imp1_st_pm_info},
    {"fba_imp_ocv0","fba",0xFF8C4000U,0x00001000U,1U,sizeof(fba_imp_ocv0_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_ocv0_st_int_info,"invalid",sizeof(fba_imp_ocv0_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_ocv0_st_utlb_info,&fba_imp_ocv0_st_pm_info},
    {"fba_imp_ocv1","fba",0xFF8C5000U,0x00001000U,1U,sizeof(fba_imp_ocv1_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_ocv1_st_int_info,"invalid",sizeof(fba_imp_ocv1_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_ocv1_st_utlb_info,&fba_imp_ocv1_st_pm_info},
    {"fba_imp_ocv2","fba",0xFF8C6000U,0x00001000U,1U,sizeof(fba_imp_ocv2_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_ocv2_st_int_info,"invalid",sizeof(fba_imp_ocv2_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_ocv2_st_utlb_info,&fba_imp_ocv2_st_pm_info},
    {"fba_imp_ocv3","fba",0xFF8C7000U,0x00001000U,1U,sizeof(fba_imp_ocv3_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_ocv3_st_int_info,"invalid",sizeof(fba_imp_ocv3_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_ocv3_st_utlb_info,&fba_imp_ocv3_st_pm_info},
    {"fba_imp_dp0","fba",0xFFF56000U,0x00001000U,1U,sizeof(fba_imp_dp0_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_dp0_st_int_info,"invalid",sizeof(fba_imp_dp0_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_dp0_st_utlb_info,&fba_imp_dp0_st_pm_info},
    {"fba_imp_dp1","fba",0xFFF57000U,0x00001000U,1U,sizeof(fba_imp_dp1_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_dp1_st_int_info,"invalid",sizeof(fba_imp_dp1_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_dp1_st_utlb_info,&fba_imp_dp1_st_pm_info},
    {"fba_imp_cnn0","fba",0xFFF50000U,0x00001000U,1U,sizeof(fba_imp_cnn0_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_cnn0_st_int_info,"invalid",sizeof(fba_imp_cnn0_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_cnn0_st_utlb_info,&fba_imp_cnn0_st_pm_info},
    {"fba_imp_dsp0","fba",0xFFF64000U,0x00001000U,1U,sizeof(fba_imp_dsp0_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_dsp0_st_int_info,"invalid",sizeof(fba_imp_dsp0_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_dsp0_st_utlb_info,&fba_imp_dsp0_st_pm_info},
    {"fba_imp_dsp1","fba",0xFFF65000U,0x00001000U,1U,sizeof(fba_imp_dsp1_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_dsp1_st_int_info,"invalid",sizeof(fba_imp_dsp1_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_dsp1_st_utlb_info,&fba_imp_dsp1_st_pm_info},
    {"fba_imp_cnram0","fba",0xFFF55000U,0x00001000U,1U,sizeof(fba_imp_cnram0_st_int_info)/sizeof(interrupt_struct_t),&fba_imp_cnram0_st_int_info,"invalid",sizeof(fba_imp_cnram0_st_utlb_info)/sizeof(assignment_struct_t),&fba_imp_cnram0_st_utlb_info,&fba_imp_cnram0_st_pm_info},
    {"fba_umfl0","fba",0xE7B61000U,0x00001000U,1U,sizeof(fba_umfl0_st_int_info)/sizeof(interrupt_struct_t),&fba_umfl0_st_int_info,"invalid",sizeof(fba_umfl0_st_utlb_info)/sizeof(assignment_struct_t),&fba_umfl0_st_utlb_info,&fba_umfl0_st_pm_info},
    {"fba_smps0","fba",0xE7B64000U,0x00001000U,1U,sizeof(fba_smps0_st_int_info)/sizeof(interrupt_struct_t),&fba_smps0_st_int_info,"invalid",sizeof(fba_smps0_st_utlb_info)/sizeof(assignment_struct_t),&fba_smps0_st_utlb_info,&fba_smps0_st_pm_info},
    {"fba_smpo0","fba",0xE7B63000U,0x00001000U,1U,sizeof(fba_smpo0_st_int_info)/sizeof(interrupt_struct_t),&fba_smpo0_st_int_info,"invalid",sizeof(fba_smpo0_st_utlb_info)/sizeof(assignment_struct_t),&fba_smpo0_st_utlb_info,&fba_smpo0_st_pm_info},
    {"fba_pap0","fba",0xE7B62000U,0x00001000U,1U,sizeof(fba_pap0_st_int_info)/sizeof(interrupt_struct_t),&fba_pap0_st_int_info,"invalid",sizeof(fba_pap0_st_utlb_info)/sizeof(assignment_struct_t),&fba_pap0_st_utlb_info,&fba_pap0_st_pm_info},
    {"fba_isp0","fba",0xFEAC1000U,0x00001000U,1U,sizeof(fba_isp0_st_int_info)/sizeof(interrupt_struct_t),&fba_isp0_st_int_info,"invalid",sizeof(fba_isp0_st_utlb_info)/sizeof(assignment_struct_t),&fba_isp0_st_utlb_info,&fba_isp0_st_pm_info},
    {"fba_ca76d0","fba",0xFF870000U,0x00001000U,1U,sizeof(fba_ca76d0_st_int_info)/sizeof(interrupt_struct_t),&fba_ca76d0_st_int_info,"invalid",sizeof(fba_ca76d0_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca76d0_st_utlb_info,&fba_ca76d0_st_pm_info},
    {"fba_ca760","fba",0xFF871000U,0x00001000U,1U,sizeof(fba_ca760_st_int_info)/sizeof(interrupt_struct_t),&fba_ca760_st_int_info,"invalid",sizeof(fba_ca760_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca760_st_utlb_info,&fba_ca760_st_pm_info},
    {"fba_ca761","fba",0xFF872000U,0x00001000U,1U,sizeof(fba_ca761_st_int_info)/sizeof(interrupt_struct_t),&fba_ca761_st_int_info,"invalid",sizeof(fba_ca761_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca761_st_utlb_info,&fba_ca761_st_pm_info},
    {"fba_ca762","fba",0xFF874000U,0x00001000U,1U,sizeof(fba_ca762_st_int_info)/sizeof(interrupt_struct_t),&fba_ca762_st_int_info,"invalid",sizeof(fba_ca762_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca762_st_utlb_info,&fba_ca762_st_pm_info},
    {"fba_ca763","fba",0xFF875000U,0x00001000U,1U,sizeof(fba_ca763_st_int_info)/sizeof(interrupt_struct_t),&fba_ca763_st_int_info,"invalid",sizeof(fba_ca763_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca763_st_utlb_info,&fba_ca763_st_pm_info},
    {"ipmmu_mm_00","ipmmu_mm",0xEEFC0000U,0x00020000U,0U,sizeof(ipmmu_mm_00_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_mm_00_st_int_info,"invalid",sizeof(ipmmu_mm_00_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_mm_00_st_utlb_info,&ipmmu_mm_00_st_pm_info},
    {"ipmmu_cache_vi0","ipmmu_cache",0xEEE80000U,0x00020000U,0U,sizeof(ipmmu_cache_vi0_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_vi0_st_int_info,"invalid",sizeof(ipmmu_cache_vi0_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_vi0_st_utlb_info,&ipmmu_cache_vi0_st_pm_info},
    {"ipmmu_cache_vi1","ipmmu_cache",0xEEEC0000U,0x00020000U,0U,sizeof(ipmmu_cache_vi1_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_vi1_st_int_info,"invalid",sizeof(ipmmu_cache_vi1_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_vi1_st_utlb_info,&ipmmu_cache_vi1_st_pm_info},
    {"ipmmu_cache_vc","ipmmu_cache",0xEEDC0000U,0x00020000U,0U,sizeof(ipmmu_cache_vc_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_vc_st_int_info,"invalid",sizeof(ipmmu_cache_vc_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_vc_st_utlb_info,&ipmmu_cache_vc_st_pm_info},
    {"ipmmu_cache_ir","ipmmu_cache",0xEED80000U,0x00020000U,0U,sizeof(ipmmu_cache_ir_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_ir_st_int_info,"invalid",sizeof(ipmmu_cache_ir_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_ir_st_utlb_info,&ipmmu_cache_ir_st_pm_info},
    {"ipmmu_cache_rt0","ipmmu_cache",0xEE480000U,0x00020000U,0U,sizeof(ipmmu_cache_rt0_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_rt0_st_int_info,"invalid",sizeof(ipmmu_cache_rt0_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_rt0_st_utlb_info,&ipmmu_cache_rt0_st_pm_info},
    {"ipmmu_cache_rt1","ipmmu_cache",0xEE4C0000U,0x00020000U,0U,sizeof(ipmmu_cache_rt1_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_rt1_st_int_info,"invalid",sizeof(ipmmu_cache_rt1_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_rt1_st_utlb_info,&ipmmu_cache_rt1_st_pm_info},
    {"ipmmu_cache_ds0","ipmmu_cache",0xEED00000U,0x00020000U,0U,sizeof(ipmmu_cache_ds0_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_ds0_st_int_info,"invalid",sizeof(ipmmu_cache_ds0_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_ds0_st_utlb_info,&ipmmu_cache_ds0_st_pm_info},
    {"ipmmu_cache_hsc","ipmmu_cache",0xEED40000U,0x00020000U,0U,sizeof(ipmmu_cache_hsc_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_hsc_st_int_info,"invalid",sizeof(ipmmu_cache_hsc_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_hsc_st_utlb_info,&ipmmu_cache_hsc_st_pm_info},
    {"ipmmu_cache_vip0","ipmmu_cache",0xEEF00000U,0x00020000U,0U,sizeof(ipmmu_cache_vip0_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_vip0_st_int_info,"invalid",sizeof(ipmmu_cache_vip0_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_vip0_st_utlb_info,&ipmmu_cache_vip0_st_pm_info},
    {"ipmmu_cache_vip1","ipmmu_cache",0xEEF40000U,0x00020000U,0U,sizeof(ipmmu_cache_vip1_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_vip1_st_int_info,"invalid",sizeof(ipmmu_cache_vip1_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_vip1_st_utlb_info,&ipmmu_cache_vip1_st_pm_info},
    {"ipmmu_cache_3dg","ipmmu_cache",0xEEE00000U,0x00020000U,0U,sizeof(ipmmu_cache_3dg_st_int_info)/sizeof(interrupt_struct_t),&ipmmu_cache_3dg_st_int_info,"invalid",sizeof(ipmmu_cache_3dg_st_utlb_info)/sizeof(assignment_struct_t),&ipmmu_cache_3dg_st_utlb_info,&ipmmu_cache_3dg_st_pm_info},
    {"rfso_00","rfso",0xFFE80000U,0x00001000U,1U,sizeof(rfso_00_st_int_info)/sizeof(interrupt_struct_t),&rfso_00_st_int_info,"invalid",sizeof(rfso_00_st_utlb_info)/sizeof(assignment_struct_t),&rfso_00_st_utlb_info,&rfso_00_st_pm_info},
    {"rfso_01","rfso",0xFFE81000U,0x00001000U,1U,sizeof(rfso_01_st_int_info)/sizeof(interrupt_struct_t),&rfso_01_st_int_info,"invalid",sizeof(rfso_01_st_utlb_info)/sizeof(assignment_struct_t),&rfso_01_st_utlb_info,&rfso_01_st_pm_info},
    {"rfso_02","rfso",0xFFE82000U,0x00001000U,1U,sizeof(rfso_02_st_int_info)/sizeof(interrupt_struct_t),&rfso_02_st_int_info,"invalid",sizeof(rfso_02_st_utlb_info)/sizeof(assignment_struct_t),&rfso_02_st_utlb_info,&rfso_02_st_pm_info},
    {"rfso_03","rfso",0xFFE83000U,0x00001000U,1U,sizeof(rfso_03_st_int_info)/sizeof(interrupt_struct_t),&rfso_03_st_int_info,"invalid",sizeof(rfso_03_st_utlb_info)/sizeof(assignment_struct_t),&rfso_03_st_utlb_info,&rfso_03_st_pm_info},
    {"rfso_04","rfso",0xFFE84000U,0x00001000U,1U,sizeof(rfso_04_st_int_info)/sizeof(interrupt_struct_t),&rfso_04_st_int_info,"invalid",sizeof(rfso_04_st_utlb_info)/sizeof(assignment_struct_t),&rfso_04_st_utlb_info,&rfso_04_st_pm_info},
    {"rfso_05","rfso",0xFFE85000U,0x00001000U,1U,sizeof(rfso_05_st_int_info)/sizeof(interrupt_struct_t),&rfso_05_st_int_info,"invalid",sizeof(rfso_05_st_utlb_info)/sizeof(assignment_struct_t),&rfso_05_st_utlb_info,&rfso_05_st_pm_info},
    {"rfso_06","rfso",0xFFE86000U,0x00001000U,1U,sizeof(rfso_06_st_int_info)/sizeof(interrupt_struct_t),&rfso_06_st_int_info,"invalid",sizeof(rfso_06_st_utlb_info)/sizeof(assignment_struct_t),&rfso_06_st_utlb_info,&rfso_06_st_pm_info},
    {"rfso_07","rfso",0xFFE87000U,0x00001000U,1U,sizeof(rfso_07_st_int_info)/sizeof(interrupt_struct_t),&rfso_07_st_int_info,"invalid",sizeof(rfso_07_st_utlb_info)/sizeof(assignment_struct_t),&rfso_07_st_utlb_info,&rfso_07_st_pm_info},
    {"rfso_08","rfso",0xFFE88000U,0x00001000U,1U,sizeof(rfso_08_st_int_info)/sizeof(interrupt_struct_t),&rfso_08_st_int_info,"invalid",sizeof(rfso_08_st_utlb_info)/sizeof(assignment_struct_t),&rfso_08_st_utlb_info,&rfso_08_st_pm_info},
    {"rfso_09","rfso",0xFFE89000U,0x00001000U,1U,sizeof(rfso_09_st_int_info)/sizeof(interrupt_struct_t),&rfso_09_st_int_info,"invalid",sizeof(rfso_09_st_utlb_info)/sizeof(assignment_struct_t),&rfso_09_st_utlb_info,&rfso_09_st_pm_info},
    {"rfso_10","rfso",0xFFE8A000U,0x00001000U,1U,sizeof(rfso_10_st_int_info)/sizeof(interrupt_struct_t),&rfso_10_st_int_info,"invalid",sizeof(rfso_10_st_utlb_info)/sizeof(assignment_struct_t),&rfso_10_st_utlb_info,&rfso_10_st_pm_info},
    {"caiplite_wrapper_00","caiplite_wrapper",0x00000000U,0x00000000U,0U,sizeof(caiplite_wrapper_00_st_int_info)/sizeof(interrupt_struct_t),&caiplite_wrapper_00_st_int_info,"invalid",sizeof(caiplite_wrapper_00_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_wrapper_00_st_utlb_info,&caiplite_wrapper_00_st_pm_info},
    {"caiplite_00","caiplite",0xE7800000U,0x00001000U,1U,sizeof(caiplite_00_st_int_info)/sizeof(interrupt_struct_t),&caiplite_00_st_int_info,"invalid",sizeof(caiplite_00_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_00_st_utlb_info,&caiplite_00_st_pm_info},
    {"caiplite_01","caiplite",0xE7801000U,0x00001000U,1U,sizeof(caiplite_01_st_int_info)/sizeof(interrupt_struct_t),&caiplite_01_st_int_info,"invalid",sizeof(caiplite_01_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_01_st_utlb_info,&caiplite_01_st_pm_info},
    {"caiplite_02","caiplite",0xE7802000U,0x00001000U,1U,sizeof(caiplite_02_st_int_info)/sizeof(interrupt_struct_t),&caiplite_02_st_int_info,"invalid",sizeof(caiplite_02_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_02_st_utlb_info,&caiplite_02_st_pm_info},
    {"caiplite_03","caiplite",0xE7803000U,0x00001000U,1U,sizeof(caiplite_03_st_int_info)/sizeof(interrupt_struct_t),&caiplite_03_st_int_info,"invalid",sizeof(caiplite_03_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_03_st_utlb_info,&caiplite_03_st_pm_info},
    {"caiplite_04","caiplite",0xE7804000U,0x00001000U,1U,sizeof(caiplite_04_st_int_info)/sizeof(interrupt_struct_t),&caiplite_04_st_int_info,"invalid",sizeof(caiplite_04_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_04_st_utlb_info,&caiplite_04_st_pm_info},
    {"caiplite_05","caiplite",0xE7805000U,0x00001000U,1U,sizeof(caiplite_05_st_int_info)/sizeof(interrupt_struct_t),&caiplite_05_st_int_info,"invalid",sizeof(caiplite_05_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_05_st_utlb_info,&caiplite_05_st_pm_info},
    {"caiplite_06","caiplite",0xE7806000U,0x00001000U,1U,sizeof(caiplite_06_st_int_info)/sizeof(interrupt_struct_t),&caiplite_06_st_int_info,"invalid",sizeof(caiplite_06_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_06_st_utlb_info,&caiplite_06_st_pm_info},
    {"caiplite_07","caiplite",0xE7807000U,0x00001000U,1U,sizeof(caiplite_07_st_int_info)/sizeof(interrupt_struct_t),&caiplite_07_st_int_info,"invalid",sizeof(caiplite_07_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_07_st_utlb_info,&caiplite_07_st_pm_info},
    {"wcrc_caiplite_00","wcrc_caiplite",0xE7080000U,0x00001000U,0U,sizeof(wcrc_caiplite_00_st_int_info)/sizeof(interrupt_struct_t),&wcrc_caiplite_00_st_int_info,"invalid",sizeof(wcrc_caiplite_00_st_utlb_info)/sizeof(assignment_struct_t),&wcrc_caiplite_00_st_utlb_info,&wcrc_caiplite_00_st_pm_info},
    {"wcrc_caiplite_01","wcrc_caiplite",0xE7090000U,0x00001000U,0U,sizeof(wcrc_caiplite_01_st_int_info)/sizeof(interrupt_struct_t),&wcrc_caiplite_01_st_int_info,"invalid",sizeof(wcrc_caiplite_01_st_utlb_info)/sizeof(assignment_struct_t),&wcrc_caiplite_01_st_utlb_info,&wcrc_caiplite_01_st_pm_info},
    {"wcrc_caiplite_02","wcrc_caiplite",0xE70A0000U,0x00001000U,0U,sizeof(wcrc_caiplite_02_st_int_info)/sizeof(interrupt_struct_t),&wcrc_caiplite_02_st_int_info,"invalid",sizeof(wcrc_caiplite_02_st_utlb_info)/sizeof(assignment_struct_t),&wcrc_caiplite_02_st_utlb_info,&wcrc_caiplite_02_st_pm_info},
    {"wcrc_caiplite_03","wcrc_caiplite",0xE70B0000U,0x00001000U,0U,sizeof(wcrc_caiplite_03_st_int_info)/sizeof(interrupt_struct_t),&wcrc_caiplite_03_st_int_info,"invalid",sizeof(wcrc_caiplite_03_st_utlb_info)/sizeof(assignment_struct_t),&wcrc_caiplite_03_st_utlb_info,&wcrc_caiplite_03_st_pm_info},
    {"rtdmac_control0_00","rtdmac_control0",0xFFC10000U,0x00001000U,1U,sizeof(rtdmac_control0_00_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_00_st_int_info,"rt1",sizeof(rtdmac_control0_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_00_st_utlb_info,&rtdmac_control0_00_st_pm_info},
    {"rtdmac_control0_01","rtdmac_control0",0xFFC11000U,0x00001000U,1U,sizeof(rtdmac_control0_01_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_01_st_int_info,"rt1",sizeof(rtdmac_control0_01_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_01_st_utlb_info,&rtdmac_control0_01_st_pm_info},
    {"rtdmac_control0_02","rtdmac_control0",0xFFC12000U,0x00001000U,1U,sizeof(rtdmac_control0_02_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_02_st_int_info,"rt1",sizeof(rtdmac_control0_02_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_02_st_utlb_info,&rtdmac_control0_02_st_pm_info},
    {"rtdmac_control0_03","rtdmac_control0",0xFFC13000U,0x00001000U,1U,sizeof(rtdmac_control0_03_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_03_st_int_info,"rt1",sizeof(rtdmac_control0_03_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_03_st_utlb_info,&rtdmac_control0_03_st_pm_info},
    {"rtdmac_control0_04","rtdmac_control0",0xFFC14000U,0x00001000U,1U,sizeof(rtdmac_control0_04_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_04_st_int_info,"rt1",sizeof(rtdmac_control0_04_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_04_st_utlb_info,&rtdmac_control0_04_st_pm_info},
    {"rtdmac_control0_05","rtdmac_control0",0xFFC15000U,0x00001000U,1U,sizeof(rtdmac_control0_05_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_05_st_int_info,"rt1",sizeof(rtdmac_control0_05_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_05_st_utlb_info,&rtdmac_control0_05_st_pm_info},
    {"rtdmac_control0_06","rtdmac_control0",0xFFC16000U,0x00001000U,1U,sizeof(rtdmac_control0_06_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_06_st_int_info,"rt1",sizeof(rtdmac_control0_06_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_06_st_utlb_info,&rtdmac_control0_06_st_pm_info},
    {"rtdmac_control0_07","rtdmac_control0",0xFFC17000U,0x00001000U,1U,sizeof(rtdmac_control0_07_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_07_st_int_info,"rt1",sizeof(rtdmac_control0_07_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_07_st_utlb_info,&rtdmac_control0_07_st_pm_info},
    {"rtdmac_control0_08","rtdmac_control0",0xFFC18000U,0x00001000U,1U,sizeof(rtdmac_control0_08_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_08_st_int_info,"rt1",sizeof(rtdmac_control0_08_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_08_st_utlb_info,&rtdmac_control0_08_st_pm_info},
    {"rtdmac_control0_09","rtdmac_control0",0xFFC19000U,0x00001000U,1U,sizeof(rtdmac_control0_09_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_09_st_int_info,"rt1",sizeof(rtdmac_control0_09_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_09_st_utlb_info,&rtdmac_control0_09_st_pm_info},
    {"rtdmac_control0_10","rtdmac_control0",0xFFC1A000U,0x00001000U,1U,sizeof(rtdmac_control0_10_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_10_st_int_info,"rt1",sizeof(rtdmac_control0_10_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_10_st_utlb_info,&rtdmac_control0_10_st_pm_info},
    {"rtdmac_control0_11","rtdmac_control0",0xFFC1B000U,0x00001000U,1U,sizeof(rtdmac_control0_11_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_11_st_int_info,"rt1",sizeof(rtdmac_control0_11_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_11_st_utlb_info,&rtdmac_control0_11_st_pm_info},
    {"rtdmac_control0_12","rtdmac_control0",0xFFC1C000U,0x00001000U,1U,sizeof(rtdmac_control0_12_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_12_st_int_info,"rt1",sizeof(rtdmac_control0_12_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_12_st_utlb_info,&rtdmac_control0_12_st_pm_info},
    {"rtdmac_control0_13","rtdmac_control0",0xFFC1D000U,0x00001000U,1U,sizeof(rtdmac_control0_13_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_13_st_int_info,"rt1",sizeof(rtdmac_control0_13_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_13_st_utlb_info,&rtdmac_control0_13_st_pm_info},
    {"rtdmac_control0_14","rtdmac_control0",0xFFC1E000U,0x00001000U,1U,sizeof(rtdmac_control0_14_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_14_st_int_info,"rt1",sizeof(rtdmac_control0_14_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_14_st_utlb_info,&rtdmac_control0_14_st_pm_info},
    {"rtdmac_control0_15","rtdmac_control0",0xFFC1F000U,0x00001000U,1U,sizeof(rtdmac_control0_15_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control0_15_st_int_info,"rt1",sizeof(rtdmac_control0_15_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control0_15_st_utlb_info,&rtdmac_control0_15_st_pm_info},
    {"rtdmac_control1_00","rtdmac_control1",0xFFC20000U,0x00001000U,1U,sizeof(rtdmac_control1_00_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_00_st_int_info,"rt1",sizeof(rtdmac_control1_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_00_st_utlb_info,&rtdmac_control1_00_st_pm_info},
    {"rtdmac_control1_01","rtdmac_control1",0xFFC21000U,0x00001000U,1U,sizeof(rtdmac_control1_01_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_01_st_int_info,"rt1",sizeof(rtdmac_control1_01_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_01_st_utlb_info,&rtdmac_control1_01_st_pm_info},
    {"rtdmac_control1_02","rtdmac_control1",0xFFC22000U,0x00001000U,1U,sizeof(rtdmac_control1_02_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_02_st_int_info,"rt1",sizeof(rtdmac_control1_02_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_02_st_utlb_info,&rtdmac_control1_02_st_pm_info},
    {"rtdmac_control1_03","rtdmac_control1",0xFFC23000U,0x00001000U,1U,sizeof(rtdmac_control1_03_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_03_st_int_info,"rt1",sizeof(rtdmac_control1_03_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_03_st_utlb_info,&rtdmac_control1_03_st_pm_info},
    {"rtdmac_control1_04","rtdmac_control1",0xFFC24000U,0x00001000U,1U,sizeof(rtdmac_control1_04_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_04_st_int_info,"rt1",sizeof(rtdmac_control1_04_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_04_st_utlb_info,&rtdmac_control1_04_st_pm_info},
    {"rtdmac_control1_05","rtdmac_control1",0xFFC25000U,0x00001000U,1U,sizeof(rtdmac_control1_05_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_05_st_int_info,"rt1",sizeof(rtdmac_control1_05_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_05_st_utlb_info,&rtdmac_control1_05_st_pm_info},
    {"rtdmac_control1_06","rtdmac_control1",0xFFC26000U,0x00001000U,1U,sizeof(rtdmac_control1_06_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_06_st_int_info,"rt1",sizeof(rtdmac_control1_06_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_06_st_utlb_info,&rtdmac_control1_06_st_pm_info},
    {"rtdmac_control1_07","rtdmac_control1",0xFFC27000U,0x00001000U,1U,sizeof(rtdmac_control1_07_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_07_st_int_info,"rt1",sizeof(rtdmac_control1_07_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_07_st_utlb_info,&rtdmac_control1_07_st_pm_info},
    {"rtdmac_control1_08","rtdmac_control1",0xFFC28000U,0x00001000U,1U,sizeof(rtdmac_control1_08_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_08_st_int_info,"rt1",sizeof(rtdmac_control1_08_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_08_st_utlb_info,&rtdmac_control1_08_st_pm_info},
    {"rtdmac_control1_09","rtdmac_control1",0xFFC29000U,0x00001000U,1U,sizeof(rtdmac_control1_09_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_09_st_int_info,"rt1",sizeof(rtdmac_control1_09_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_09_st_utlb_info,&rtdmac_control1_09_st_pm_info},
    {"rtdmac_control1_10","rtdmac_control1",0xFFC2A000U,0x00001000U,1U,sizeof(rtdmac_control1_10_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_10_st_int_info,"rt1",sizeof(rtdmac_control1_10_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_10_st_utlb_info,&rtdmac_control1_10_st_pm_info},
    {"rtdmac_control1_11","rtdmac_control1",0xFFC2B000U,0x00001000U,1U,sizeof(rtdmac_control1_11_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_11_st_int_info,"rt1",sizeof(rtdmac_control1_11_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_11_st_utlb_info,&rtdmac_control1_11_st_pm_info},
    {"rtdmac_control1_12","rtdmac_control1",0xFFC2C000U,0x00001000U,1U,sizeof(rtdmac_control1_12_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_12_st_int_info,"rt1",sizeof(rtdmac_control1_12_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_12_st_utlb_info,&rtdmac_control1_12_st_pm_info},
    {"rtdmac_control1_13","rtdmac_control1",0xFFC2D000U,0x00001000U,1U,sizeof(rtdmac_control1_13_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_13_st_int_info,"rt1",sizeof(rtdmac_control1_13_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_13_st_utlb_info,&rtdmac_control1_13_st_pm_info},
    {"rtdmac_control1_14","rtdmac_control1",0xFFC2E000U,0x00001000U,1U,sizeof(rtdmac_control1_14_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_14_st_int_info,"rt1",sizeof(rtdmac_control1_14_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_14_st_utlb_info,&rtdmac_control1_14_st_pm_info},
    {"rtdmac_control1_15","rtdmac_control1",0xFFC2F000U,0x00001000U,1U,sizeof(rtdmac_control1_15_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control1_15_st_int_info,"rt1",sizeof(rtdmac_control1_15_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control1_15_st_utlb_info,&rtdmac_control1_15_st_pm_info},
    {"vin_00","vin",0xE6EF0000U,0x00001000U,1U,sizeof(vin_00_st_int_info)/sizeof(interrupt_struct_t),&vin_00_st_int_info,"vi0",sizeof(vin_00_st_utlb_info)/sizeof(assignment_struct_t),&vin_00_st_utlb_info,&vin_00_st_pm_info},
    {"vin_01","vin",0xE6EF1000U,0x00001000U,1U,sizeof(vin_01_st_int_info)/sizeof(interrupt_struct_t),&vin_01_st_int_info,"vi0",sizeof(vin_01_st_utlb_info)/sizeof(assignment_struct_t),&vin_01_st_utlb_info,&vin_01_st_pm_info},
    {"vin_02","vin",0xE6EF2000U,0x00001000U,1U,sizeof(vin_02_st_int_info)/sizeof(interrupt_struct_t),&vin_02_st_int_info,"vi0",sizeof(vin_02_st_utlb_info)/sizeof(assignment_struct_t),&vin_02_st_utlb_info,&vin_02_st_pm_info},
    {"vin_03","vin",0xE6EF3000U,0x00001000U,1U,sizeof(vin_03_st_int_info)/sizeof(interrupt_struct_t),&vin_03_st_int_info,"vi0",sizeof(vin_03_st_utlb_info)/sizeof(assignment_struct_t),&vin_03_st_utlb_info,&vin_03_st_pm_info},
    {"vin_04","vin",0xE6EF4000U,0x00001000U,1U,sizeof(vin_04_st_int_info)/sizeof(interrupt_struct_t),&vin_04_st_int_info,"vi0",sizeof(vin_04_st_utlb_info)/sizeof(assignment_struct_t),&vin_04_st_utlb_info,&vin_04_st_pm_info},
    {"vin_05","vin",0xE6EF5000U,0x00001000U,1U,sizeof(vin_05_st_int_info)/sizeof(interrupt_struct_t),&vin_05_st_int_info,"vi0",sizeof(vin_05_st_utlb_info)/sizeof(assignment_struct_t),&vin_05_st_utlb_info,&vin_05_st_pm_info},
    {"vin_06","vin",0xE6EF6000U,0x00001000U,1U,sizeof(vin_06_st_int_info)/sizeof(interrupt_struct_t),&vin_06_st_int_info,"vi0",sizeof(vin_06_st_utlb_info)/sizeof(assignment_struct_t),&vin_06_st_utlb_info,&vin_06_st_pm_info},
    {"vin_07","vin",0xE6EF7000U,0x00001000U,1U,sizeof(vin_07_st_int_info)/sizeof(interrupt_struct_t),&vin_07_st_int_info,"vi0",sizeof(vin_07_st_utlb_info)/sizeof(assignment_struct_t),&vin_07_st_utlb_info,&vin_07_st_pm_info},
    {"vin_10","vin",0xE6EF8000U,0x00001000U,1U,sizeof(vin_10_st_int_info)/sizeof(interrupt_struct_t),&vin_10_st_int_info,"vi0",sizeof(vin_10_st_utlb_info)/sizeof(assignment_struct_t),&vin_10_st_utlb_info,&vin_10_st_pm_info},
    {"vin_11","vin",0xE6EF9000U,0x00001000U,1U,sizeof(vin_11_st_int_info)/sizeof(interrupt_struct_t),&vin_11_st_int_info,"vi0",sizeof(vin_11_st_utlb_info)/sizeof(assignment_struct_t),&vin_11_st_utlb_info,&vin_11_st_pm_info},
    {"vin_12","vin",0xE6EFA000U,0x00001000U,1U,sizeof(vin_12_st_int_info)/sizeof(interrupt_struct_t),&vin_12_st_int_info,"vi0",sizeof(vin_12_st_utlb_info)/sizeof(assignment_struct_t),&vin_12_st_utlb_info,&vin_12_st_pm_info},
    {"vin_13","vin",0xE6EFB000U,0x00001000U,1U,sizeof(vin_13_st_int_info)/sizeof(interrupt_struct_t),&vin_13_st_int_info,"vi0",sizeof(vin_13_st_utlb_info)/sizeof(assignment_struct_t),&vin_13_st_utlb_info,&vin_13_st_pm_info},
    {"vin_14","vin",0xE6EFC000U,0x00001000U,1U,sizeof(vin_14_st_int_info)/sizeof(interrupt_struct_t),&vin_14_st_int_info,"vi0",sizeof(vin_14_st_utlb_info)/sizeof(assignment_struct_t),&vin_14_st_utlb_info,&vin_14_st_pm_info},
    {"vin_15","vin",0xE6EFD000U,0x00001000U,1U,sizeof(vin_15_st_int_info)/sizeof(interrupt_struct_t),&vin_15_st_int_info,"vi0",sizeof(vin_15_st_utlb_info)/sizeof(assignment_struct_t),&vin_15_st_utlb_info,&vin_15_st_pm_info},
    {"vin_16","vin",0xE6EFE000U,0x00001000U,1U,sizeof(vin_16_st_int_info)/sizeof(interrupt_struct_t),&vin_16_st_int_info,"vi0",sizeof(vin_16_st_utlb_info)/sizeof(assignment_struct_t),&vin_16_st_utlb_info,&vin_16_st_pm_info},
    {"vin_17","vin",0xE6EFF000U,0x00001000U,1U,sizeof(vin_17_st_int_info)/sizeof(interrupt_struct_t),&vin_17_st_int_info,"vi0",sizeof(vin_17_st_utlb_info)/sizeof(assignment_struct_t),&vin_17_st_utlb_info,&vin_17_st_pm_info},
    {"vsp_00","vsp",0xFEA20000U,0x00008000U,1U,sizeof(vsp_00_st_int_info)/sizeof(interrupt_struct_t),&vsp_00_st_int_info,"vi1",sizeof(vsp_00_st_utlb_info)/sizeof(assignment_struct_t),&vsp_00_st_utlb_info,&vsp_00_st_pm_info},
    {"csi_00","csi",0xFE500000U,0x00040000U,1U,sizeof(csi_00_st_int_info)/sizeof(interrupt_struct_t),&csi_00_st_int_info,"invalid",sizeof(csi_00_st_utlb_info)/sizeof(assignment_struct_t),csi_00_st_utlb_info,csi_00_st_pm_info},
    {"csi_01","csi",0xFE540000U,0x00040000U,1U,sizeof(csi_01_st_int_info)/sizeof(interrupt_struct_t),&csi_01_st_int_info,"invalid",sizeof(csi_01_st_utlb_info)/sizeof(assignment_struct_t),csi_01_st_utlb_info,csi_01_st_pm_info},
    {"du_00","du",0xFEB00000U,0x00040000U,2U,sizeof(du_00_st_int_info)/sizeof(interrupt_struct_t),&du_00_st_int_info,"invalid",sizeof(du_00_st_utlb_info)/sizeof(assignment_struct_t),&du_00_st_utlb_info,&du_00_st_pm_info},
    {"dsi_00","dsi",0xFED80000U,0x00010000U,9U,sizeof(dsi_00_st_int_info)/sizeof(interrupt_struct_t),&dsi_00_st_int_info,"vi1",sizeof(dsi_00_st_utlb_info)/sizeof(assignment_struct_t),&dsi_00_st_utlb_info,&dsi_00_st_pm_info},
    {"wwdt_00","wwdt",0xFFC90000U,0x00001000U,2U,sizeof(wwdt_00_st_int_info)/sizeof(interrupt_struct_t),&wwdt_00_st_int_info,"invalid",sizeof(wwdt_00_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_00_st_utlb_info,&wwdt_00_st_pm_info},
    {"wwdt_01","wwdt",0xFFCA0000U,0x00001000U,2U,sizeof(wwdt_01_st_int_info)/sizeof(interrupt_struct_t),&wwdt_01_st_int_info,"invalid",sizeof(wwdt_01_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_01_st_utlb_info,&wwdt_01_st_pm_info},
    {"wwdt_02","wwdt",0xFFCB0000U,0x00001000U,2U,sizeof(wwdt_02_st_int_info)/sizeof(interrupt_struct_t),&wwdt_02_st_int_info,"invalid",sizeof(wwdt_02_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_02_st_utlb_info,&wwdt_02_st_pm_info},
    {"wwdt_03","wwdt",0xFFCC0000U,0x00001000U,2U,sizeof(wwdt_03_st_int_info)/sizeof(interrupt_struct_t),&wwdt_03_st_int_info,"invalid",sizeof(wwdt_03_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_03_st_utlb_info,&wwdt_03_st_pm_info},
    {"wwdt_04","wwdt",0xFFCF0000U,0x00001000U,2U,sizeof(wwdt_04_st_int_info)/sizeof(interrupt_struct_t),&wwdt_04_st_int_info,"invalid",sizeof(wwdt_04_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_04_st_utlb_info,&wwdt_04_st_pm_info},
    {"wwdt_05","wwdt",0xFFEF0000U,0x00001000U,2U,sizeof(wwdt_05_st_int_info)/sizeof(interrupt_struct_t),&wwdt_05_st_int_info,"invalid",sizeof(wwdt_05_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_05_st_utlb_info,&wwdt_05_st_pm_info},
    {"wwdt_06","wwdt",0xFFF10000U,0x00001000U,2U,sizeof(wwdt_06_st_int_info)/sizeof(interrupt_struct_t),&wwdt_06_st_int_info,"invalid",sizeof(wwdt_06_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_06_st_utlb_info,&wwdt_06_st_pm_info},
    {"wwdt_07","wwdt",0xFFF20000U,0x00001000U,2U,sizeof(wwdt_07_st_int_info)/sizeof(interrupt_struct_t),&wwdt_07_st_int_info,"invalid",sizeof(wwdt_07_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_07_st_utlb_info,&wwdt_07_st_pm_info},
    {"wwdt_08","wwdt",0xFFF30000U,0x00001000U,2U,sizeof(wwdt_08_st_int_info)/sizeof(interrupt_struct_t),&wwdt_08_st_int_info,"invalid",sizeof(wwdt_08_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_08_st_utlb_info,&wwdt_08_st_pm_info},
    {"wwdt_09","wwdt",0xFFF40000U,0x00001000U,2U,sizeof(wwdt_09_st_int_info)/sizeof(interrupt_struct_t),&wwdt_09_st_int_info,"invalid",sizeof(wwdt_09_st_utlb_info)/sizeof(assignment_struct_t),&wwdt_09_st_utlb_info,&wwdt_09_st_pm_info},
    {"doc_00","doc",0xFEBA0000U,0x00011000U,2U,sizeof(doc_00_st_int_info)/sizeof(interrupt_struct_t),&doc_00_st_int_info,"invalid",sizeof(doc_00_st_utlb_info)/sizeof(assignment_struct_t),&doc_00_st_utlb_info,&doc_00_st_pm_info},
    {"rtdmac_descriptor0_00","rtdmac_descriptor0",0xFFD60000U,0x00001000U,0U,sizeof(rtdmac_descriptor_00_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_descriptor_00_st_int_info,"invalid",sizeof(rtdmac_descriptor0_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_descriptor0_00_st_utlb_info,&rtdmac_descriptor0_00_st_pm_info},
    {"rtdmac_descriptor1_00","rtdmac_descriptor1",0xFFD61000U,0x00001000U,0U,sizeof(rtdmac_descriptor_01_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_descriptor_01_st_int_info,"invalid",sizeof(rtdmac_descriptor1_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_descriptor1_00_st_utlb_info,&rtdmac_descriptor1_00_st_pm_info},
    {"hsm_00","hsm",0xFF000000U,0x00080000U,0U,sizeof(hsm_00_st_int_info)/sizeof(interrupt_struct_t),&hsm_00_st_int_info,"vip1",sizeof(hsm_00_st_utlb_info)/sizeof(assignment_struct_t),&hsm_00_st_utlb_info,&hsm_00_st_pm_info},
    {"dcm_00","dcm",0xFF080000U,0x00064000U,0U,sizeof(dcm_00_st_int_info)/sizeof(interrupt_struct_t),&dcm_00_st_int_info,"vip1",sizeof(dcm_00_st_utlb_info)/sizeof(assignment_struct_t),&dcm_00_st_utlb_info,&dcm_00_st_pm_info},
    {"cbf_00","cbf",0xFF0E8000U,0x00003000U,0U,sizeof(cbf_00_st_int_info)/sizeof(interrupt_struct_t),&cbf_00_st_int_info,"vip1",sizeof(cbf_00_st_utlb_info)/sizeof(assignment_struct_t),&cbf_00_st_utlb_info,&cbf_00_st_pm_info},
    {"sbf_00","sbf",0xFF0EC000U,0x00004000U,0U,sizeof(sbf_00_st_int_info)/sizeof(interrupt_struct_t),&sbf_00_st_int_info,"vip1",sizeof(sbf_00_st_utlb_info)/sizeof(assignment_struct_t),&sbf_00_st_utlb_info,&sbf_00_st_pm_info},
    {"osm_00","osm",0xFF0F0000U,0x00010000U,0U,sizeof(osm_00_st_int_info)/sizeof(interrupt_struct_t),&osm_00_st_int_info,"vip1",sizeof(osm_00_st_utlb_info)/sizeof(assignment_struct_t),&osm_00_st_utlb_info,&osm_00_st_pm_info},
    {"ism_00","ism",0xFF100000U,0x00020000U,0U,sizeof(ism_00_st_int_info)/sizeof(interrupt_struct_t),&ism_00_st_int_info,"vip1",sizeof(ism_00_st_utlb_info)/sizeof(assignment_struct_t),&ism_00_st_utlb_info,&ism_00_st_pm_info},
    {"ism_01","ism",0xFF120000U,0x00020000U,0U,sizeof(ism_01_st_int_info)/sizeof(interrupt_struct_t),&ism_01_st_int_info,"vip1",sizeof(ism_01_st_utlb_info)/sizeof(assignment_struct_t),&ism_01_st_utlb_info,&ism_01_st_pm_info},
    {"sdmac_00","sdmac",0xFF180000U,0x00010000U,2U,sizeof(sdmac_00_st_int_info)/sizeof(interrupt_struct_t),&sdmac_00_st_int_info,"vip1",sizeof(sdmac_00_st_utlb_info)/sizeof(assignment_struct_t),&sdmac_00_st_utlb_info,&sdmac_00_st_pm_info},
    {"fbs_00","fbs",0xFF200000U,0x00080000U,1U,sizeof(fbs_00_st_int_info)/sizeof(interrupt_struct_t),&fbs_00_st_int_info,"vip1",sizeof(fbs_00_st_utlb_info)/sizeof(assignment_struct_t),&fbs_00_st_utlb_info,&fbs_00_st_pm_info},
    {"apb_00","apb",0xFF290000U,0x00004000U,0U,sizeof(apb_00_st_int_info)/sizeof(interrupt_struct_t),&apb_00_st_int_info,"vip1",sizeof(apb_00_st_utlb_info)/sizeof(assignment_struct_t),&apb_00_st_utlb_info,&apb_00_st_pm_info},
    {"dul_00","dul",0xFF298000U,0x00001000U,0U,sizeof(dul_00_st_int_info)/sizeof(interrupt_struct_t),&dul_00_st_int_info,"vip1",sizeof(dul_00_st_utlb_info)/sizeof(assignment_struct_t),&dul_00_st_utlb_info,&dul_00_st_pm_info},
    {"ecm_00","ecm",0xE6250000U,0x00001000U,0U,sizeof(ecm_00_st_int_info)/sizeof(interrupt_struct_t),&ecm_00_st_int_info,"invalid",sizeof(ecm_00_st_utlb_info)/sizeof(assignment_struct_t),&ecm_00_st_utlb_info,&ecm_00_st_pm_info},
    {"system_ram_00","system_ram",0xE6300000U,0x00100000U,0U,sizeof(system_ram_00_st_int_info)/sizeof(interrupt_struct_t),&system_ram_00_st_int_info,"invalid",sizeof(system_ram_00_st_utlb_info)/sizeof(assignment_struct_t),&system_ram_00_st_utlb_info,&system_ram_00_st_pm_info},
    {"icumx_cmd_00","icumx_cmd",0xE6610000U,0x00008000U,20U,sizeof(icumx_cmd_00_st_int_info)/sizeof(interrupt_struct_t),&icumx_cmd_00_st_int_info,"rt0",sizeof(icumx_cmd_00_st_utlb_info)/sizeof(assignment_struct_t),&icumx_cmd_00_st_utlb_info,&icumx_cmd_00_st_pm_info},
    {NULL,NULL,0x00000000U,0x00000000U,0U,0U,NULL,"invalid",0U,NULL,NULL},
};
