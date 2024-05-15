/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    expect_value_s4.h
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configurator of S4 Setting
***********************************************************************************************************************/

#include "expect_value_struct.h"

/* Table 3-39 Device Configuration value for Interrupt (For R-Car S4) */
interrupt_struct_t caiplite_wrapper_00_st_int_info[]={NULL};
interrupt_struct_t caiplite_00_st_int_info[]={933};
interrupt_struct_t caiplite_01_st_int_info[]={935};
interrupt_struct_t caiplite_02_st_int_info[]={937};
interrupt_struct_t caiplite_03_st_int_info[]={939};
interrupt_struct_t caiplite_04_st_int_info[]={941};
interrupt_struct_t caiplite_05_st_int_info[]={943};
interrupt_struct_t caiplite_06_st_int_info[]={945};
interrupt_struct_t caiplite_07_st_int_info[]={947};
interrupt_struct_t wcrc_caiplite_00_st_int_info[]={NULL};
interrupt_struct_t wcrc_caiplite_01_st_int_info[]={NULL};
interrupt_struct_t wcrc_caiplite_02_st_int_info[]={NULL};
interrupt_struct_t wcrc_caiplite_03_st_int_info[]={NULL};
interrupt_struct_t rtdmac_control0_00_st_int_info[]={30};
interrupt_struct_t rtdmac_control0_01_st_int_info[]={31};
interrupt_struct_t rtdmac_control0_02_st_int_info[]={32};
interrupt_struct_t rtdmac_control0_03_st_int_info[]={33};
interrupt_struct_t rtdmac_control0_04_st_int_info[]={34};
interrupt_struct_t rtdmac_control0_05_st_int_info[]={35};
interrupt_struct_t rtdmac_control0_06_st_int_info[]={36};
interrupt_struct_t rtdmac_control0_07_st_int_info[]={37};
interrupt_struct_t rtdmac_control0_08_st_int_info[]={38};
interrupt_struct_t rtdmac_control0_09_st_int_info[]={39};
interrupt_struct_t rtdmac_control0_10_st_int_info[]={40};
interrupt_struct_t rtdmac_control0_11_st_int_info[]={41};
interrupt_struct_t rtdmac_control0_12_st_int_info[]={42};
interrupt_struct_t rtdmac_control0_13_st_int_info[]={43};
interrupt_struct_t rtdmac_control0_14_st_int_info[]={44};
interrupt_struct_t rtdmac_control0_15_st_int_info[]={45};
interrupt_struct_t rtdmac_control1_00_st_int_info[]={48};
interrupt_struct_t rtdmac_control1_01_st_int_info[]={49};
interrupt_struct_t rtdmac_control1_02_st_int_info[]={50};
interrupt_struct_t rtdmac_control1_03_st_int_info[]={51};
interrupt_struct_t rtdmac_control1_04_st_int_info[]={52};
interrupt_struct_t rtdmac_control1_05_st_int_info[]={53};
interrupt_struct_t rtdmac_control1_06_st_int_info[]={54};
interrupt_struct_t rtdmac_control1_07_st_int_info[]={55};
interrupt_struct_t rtdmac_control1_08_st_int_info[]={56};
interrupt_struct_t rtdmac_control1_09_st_int_info[]={57};
interrupt_struct_t rtdmac_control1_10_st_int_info[]={58};
interrupt_struct_t rtdmac_control1_11_st_int_info[]={59};
interrupt_struct_t rtdmac_control1_12_st_int_info[]={60};
interrupt_struct_t rtdmac_control1_13_st_int_info[]={61};
interrupt_struct_t rtdmac_control1_14_st_int_info[]={62};
interrupt_struct_t rtdmac_control1_15_st_int_info[]={63};
interrupt_struct_t rtdmac_control2_00_st_int_info[]={66};
interrupt_struct_t rtdmac_control2_01_st_int_info[]={67};
interrupt_struct_t rtdmac_control2_02_st_int_info[]={68};
interrupt_struct_t rtdmac_control2_03_st_int_info[]={69};
interrupt_struct_t rtdmac_control2_04_st_int_info[]={70};
interrupt_struct_t rtdmac_control2_05_st_int_info[]={71};
interrupt_struct_t rtdmac_control2_06_st_int_info[]={72};
interrupt_struct_t rtdmac_control2_07_st_int_info[]={73};
interrupt_struct_t rtdmac_control2_08_st_int_info[]={74};
interrupt_struct_t rtdmac_control2_09_st_int_info[]={75};
interrupt_struct_t rtdmac_control2_10_st_int_info[]={76};
interrupt_struct_t rtdmac_control2_11_st_int_info[]={77};
interrupt_struct_t rtdmac_control2_12_st_int_info[]={78};
interrupt_struct_t rtdmac_control2_13_st_int_info[]={79};
interrupt_struct_t rtdmac_control2_14_st_int_info[]={80};
interrupt_struct_t rtdmac_control2_15_st_int_info[]={81};
interrupt_struct_t rtdmac_control3_00_st_int_info[]={84};
interrupt_struct_t rtdmac_control3_01_st_int_info[]={85};
interrupt_struct_t rtdmac_control3_02_st_int_info[]={86};
interrupt_struct_t rtdmac_control3_03_st_int_info[]={87};
interrupt_struct_t rtdmac_control3_04_st_int_info[]={88};
interrupt_struct_t rtdmac_control3_05_st_int_info[]={89};
interrupt_struct_t rtdmac_control3_06_st_int_info[]={90};
interrupt_struct_t rtdmac_control3_07_st_int_info[]={91};
interrupt_struct_t rtdmac_control3_08_st_int_info[]={92};
interrupt_struct_t rtdmac_control3_09_st_int_info[]={93};
interrupt_struct_t rtdmac_control3_10_st_int_info[]={94};
interrupt_struct_t rtdmac_control3_11_st_int_info[]={95};
interrupt_struct_t rtdmac_control3_12_st_int_info[]={96};
interrupt_struct_t rtdmac_control3_13_st_int_info[]={97};
interrupt_struct_t rtdmac_control3_14_st_int_info[]={98};
interrupt_struct_t rtdmac_control3_15_st_int_info[]={99};
interrupt_struct_t rtdmac_descriptor_00_st_int_info[]={NULL};
interrupt_struct_t rtdmac_descriptor_01_st_int_info[]={NULL};
interrupt_struct_t rtdmac_descriptor_02_st_int_info[]={NULL};
interrupt_struct_t rtdmac_descriptor_03_st_int_info[]={NULL};
interrupt_struct_t ships_00_st_int_info[]={948,949};
interrupt_struct_t ships_01_st_int_info[]={950,951};
interrupt_struct_t ships_02_st_int_info[]={952,953};
interrupt_struct_t iccom_reg_00_st_int_info[]={NULL};
interrupt_struct_t iccom_00_st_int_info[]={320};
interrupt_struct_t iccom_01_st_int_info[]={322};
interrupt_struct_t iccom_02_st_int_info[]={324};
interrupt_struct_t iccom_03_st_int_info[]={326};
interrupt_struct_t iccom_04_st_int_info[]={328};
interrupt_struct_t iccom_05_st_int_info[]={330};
interrupt_struct_t iccom_06_st_int_info[]={332};
interrupt_struct_t iccom_07_st_int_info[]={334};
interrupt_struct_t iccom_08_st_int_info[]={336};
interrupt_struct_t iccom_09_st_int_info[]={338};
interrupt_struct_t iccom_10_st_int_info[]={340};
interrupt_struct_t iccom_11_st_int_info[]={342};
interrupt_struct_t iccom_12_st_int_info[]={344};
interrupt_struct_t iccom_13_st_int_info[]={346};
interrupt_struct_t iccom_14_st_int_info[]={348};
interrupt_struct_t iccom_15_st_int_info[]={350};
interrupt_struct_t iccom_16_st_int_info[]={354};
interrupt_struct_t iccom_17_st_int_info[]={356};
interrupt_struct_t iccom_18_st_int_info[]={358};
interrupt_struct_t iccom_19_st_int_info[]={360};
interrupt_struct_t iccom_20_st_int_info[]={362};
interrupt_struct_t iccom_21_st_int_info[]={364};
interrupt_struct_t iccom_22_st_int_info[]={366};
interrupt_struct_t iccom_23_st_int_info[]={368};
interrupt_struct_t system_ram_00_st_int_info[]={NULL};
interrupt_struct_t icumx_cmd_00_st_int_info[]={192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211};
interrupt_struct_t gicd_00_st_int_info[]={NULL};
interrupt_struct_t fbc_00_st_int_info[]={740};
interrupt_struct_t fba_ca55d0_st_int_info[]={621};
interrupt_struct_t fba_ca55d1_st_int_info[]={624};
interrupt_struct_t fba_ca550_st_int_info[]={622};
interrupt_struct_t fba_ca551_st_int_info[]={623};
interrupt_struct_t fba_ca552_st_int_info[]={625};
interrupt_struct_t fba_ca553_st_int_info[]={626};
interrupt_struct_t rfso_00_st_int_info[]={218};
interrupt_struct_t rfso_01_st_int_info[]={219};
interrupt_struct_t rfso_02_st_int_info[]={220};
interrupt_struct_t rfso_03_st_int_info[]={221};
interrupt_struct_t rfso_04_st_int_info[]={222};
interrupt_struct_t rfso_05_st_int_info[]={223};
interrupt_struct_t rfso_06_st_int_info[]={224};
interrupt_struct_t rfso_07_st_int_info[]={225};
interrupt_struct_t rfso_08_st_int_info[]={226};
interrupt_struct_t rfso_09_st_int_info[]={227};
interrupt_struct_t rfso_10_st_int_info[]={228};

/* utlbs */
/* Table 3-43 Device Configuration value for AXI bus control (For R-Car S4) */
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
assignment_struct_t rtdmac_control2_00_st_utlb_info[]={32};
assignment_struct_t rtdmac_control2_01_st_utlb_info[]={33};
assignment_struct_t rtdmac_control2_02_st_utlb_info[]={34};
assignment_struct_t rtdmac_control2_03_st_utlb_info[]={35};
assignment_struct_t rtdmac_control2_04_st_utlb_info[]={36};
assignment_struct_t rtdmac_control2_05_st_utlb_info[]={37};
assignment_struct_t rtdmac_control2_06_st_utlb_info[]={38};
assignment_struct_t rtdmac_control2_07_st_utlb_info[]={39};
assignment_struct_t rtdmac_control2_08_st_utlb_info[]={40};
assignment_struct_t rtdmac_control2_09_st_utlb_info[]={41};
assignment_struct_t rtdmac_control2_10_st_utlb_info[]={42};
assignment_struct_t rtdmac_control2_11_st_utlb_info[]={43};
assignment_struct_t rtdmac_control2_12_st_utlb_info[]={44};
assignment_struct_t rtdmac_control2_13_st_utlb_info[]={45};
assignment_struct_t rtdmac_control2_14_st_utlb_info[]={46};
assignment_struct_t rtdmac_control2_15_st_utlb_info[]={47};
assignment_struct_t rtdmac_control3_00_st_utlb_info[]={48};
assignment_struct_t rtdmac_control3_01_st_utlb_info[]={49};
assignment_struct_t rtdmac_control3_02_st_utlb_info[]={50};
assignment_struct_t rtdmac_control3_03_st_utlb_info[]={51};
assignment_struct_t rtdmac_control3_04_st_utlb_info[]={52};
assignment_struct_t rtdmac_control3_05_st_utlb_info[]={53};
assignment_struct_t rtdmac_control3_06_st_utlb_info[]={54};
assignment_struct_t rtdmac_control3_07_st_utlb_info[]={55};
assignment_struct_t rtdmac_control3_08_st_utlb_info[]={56};
assignment_struct_t rtdmac_control3_09_st_utlb_info[]={57};
assignment_struct_t rtdmac_control3_10_st_utlb_info[]={58};
assignment_struct_t rtdmac_control3_11_st_utlb_info[]={59};
assignment_struct_t rtdmac_control3_12_st_utlb_info[]={60};
assignment_struct_t rtdmac_control3_13_st_utlb_info[]={61};
assignment_struct_t rtdmac_control3_14_st_utlb_info[]={62};
assignment_struct_t rtdmac_control3_15_st_utlb_info[]={63};
assignment_struct_t rtdmac_descriptor0_00_st_utlb_info[]={NULL};
assignment_struct_t rtdmac_descriptor1_00_st_utlb_info[]={NULL};
assignment_struct_t rtdmac_descriptor2_00_st_utlb_info[]={NULL};
assignment_struct_t rtdmac_descriptor3_00_st_utlb_info[]={NULL};
assignment_struct_t ships_00_st_utlb_info[]={NULL};
assignment_struct_t ships_01_st_utlb_info[]={NULL};
assignment_struct_t ships_02_st_utlb_info[]={NULL};
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
assignment_struct_t iccom_12_st_utlb_info[]={NULL};
assignment_struct_t iccom_13_st_utlb_info[]={NULL};
assignment_struct_t iccom_14_st_utlb_info[]={NULL};
assignment_struct_t iccom_15_st_utlb_info[]={NULL};
assignment_struct_t iccom_16_st_utlb_info[]={NULL};
assignment_struct_t iccom_17_st_utlb_info[]={NULL};
assignment_struct_t iccom_18_st_utlb_info[]={NULL};
assignment_struct_t iccom_19_st_utlb_info[]={NULL};
assignment_struct_t iccom_20_st_utlb_info[]={NULL};
assignment_struct_t iccom_21_st_utlb_info[]={NULL};
assignment_struct_t iccom_22_st_utlb_info[]={NULL};
assignment_struct_t iccom_23_st_utlb_info[]={NULL};
assignment_struct_t system_ram_00_st_utlb_info[]={NULL};
assignment_struct_t gicd_00_st_utlb_info[]={NULL};
assignment_struct_t icumx_cmd_00_st_utlb_info[]={1};
assignment_struct_t fbc_00_st_utlb_info={};
assignment_struct_t fba_ca55d0_st_utlb_info={};
assignment_struct_t fba_ca55d1_st_utlb_info={};
assignment_struct_t fba_ca550_st_utlb_info={};
assignment_struct_t fba_ca551_st_utlb_info={};
assignment_struct_t fba_ca552_st_utlb_info={};
assignment_struct_t fba_ca553_st_utlb_info={};
assignment_struct_t rfso_00_st_utlb_info={};
assignment_struct_t rfso_01_st_utlb_info={};
assignment_struct_t rfso_02_st_utlb_info={};
assignment_struct_t rfso_03_st_utlb_info={};
assignment_struct_t rfso_04_st_utlb_info={};
assignment_struct_t rfso_05_st_utlb_info={};
assignment_struct_t rfso_06_st_utlb_info={};
assignment_struct_t rfso_07_st_utlb_info={};
assignment_struct_t rfso_08_st_utlb_info={};
assignment_struct_t rfso_09_st_utlb_info={};
assignment_struct_t rfso_10_st_utlb_info={};

/* Table 3-41 Device Configuration value for Power control (For R-Car S4) */
/* PMA UM 1.2.2.2.5 e_pma_hwa_id_t(S4) */
power_struct_t caiplite_wrapper_00_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC_WRAPPER, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_00_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_01_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_02_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC2, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_03_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC3, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_04_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC4, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_05_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC5, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_06_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC6, OSAL_PM_POSTCKCR_INVALID};
power_struct_t caiplite_07_st_pm_info[]={R_PMA_HWA_ID_S4_AES_ACC7, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wcrc_caiplite_00_st_pm_info[]={R_PMA_HWA_ID_S4_WCRC0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wcrc_caiplite_01_st_pm_info[]={R_PMA_HWA_ID_S4_WCRC1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wcrc_caiplite_02_st_pm_info[]={R_PMA_HWA_ID_S4_WCRC2, OSAL_PM_POSTCKCR_INVALID};
power_struct_t wcrc_caiplite_03_st_pm_info[]={R_PMA_HWA_ID_S4_WCRC3, OSAL_PM_POSTCKCR_INVALID};
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
power_struct_t rtdmac_control2_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_01_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_02_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_03_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_04_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_05_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_06_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_07_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_08_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_09_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_10_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_11_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_12_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_13_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_14_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control2_15_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_01_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_02_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_03_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_04_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_05_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_06_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_07_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_08_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_09_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_10_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_11_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_12_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_13_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_14_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_control3_15_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_descriptor0_00_st_pm_info[]={R_PMA_HWA_ID_S4_RTDM0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_descriptor1_00_st_pm_info[]={R_PMA_HWA_ID_S4_RTDM1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_descriptor2_00_st_pm_info[]={R_PMA_HWA_ID_S4_RTDM2, OSAL_PM_POSTCKCR_INVALID};
power_struct_t rtdmac_descriptor3_00_st_pm_info[]={R_PMA_HWA_ID_S4_RTDM3, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ships_00_st_pm_info[]={R_PMA_HWA_ID_S4_SHIP_S0, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ships_01_st_pm_info[]={R_PMA_HWA_ID_S4_SHIP_S1, OSAL_PM_POSTCKCR_INVALID};
power_struct_t ships_02_st_pm_info[]={R_PMA_HWA_ID_S4_SHIP_S2, OSAL_PM_POSTCKCR_INVALID};
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
power_struct_t iccom_12_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_13_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_14_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_15_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_16_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_17_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_18_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_19_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_20_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_21_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_22_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t iccom_23_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t system_ram_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t icumx_cmd_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t gicd_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POSTCKCR_INVALID};
power_struct_t fbc_00_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca55d0_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca55d1_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca550_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca551_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca552_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
power_struct_t fba_ca553_st_pm_info[]={R_PMA_HWA_ID_NONE, OSAL_PM_POST2CKCR};
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

/* Table 3-37 Device Configuration value for IO access (For R-Car S4) */
struct expect_config test[] =
{
    {"caiplite_wrapper_00","caiplite_wrapper",0x00000000U,0x00000000U,0U,sizeof(caiplite_wrapper_00_st_int_info)/sizeof(interrupt_struct_t),&caiplite_wrapper_00_st_int_info,"invalid",sizeof(caiplite_wrapper_00_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_wrapper_00_st_utlb_info, &caiplite_wrapper_00_st_pm_info},
    {"caiplite_00","caiplite",0xE7800000U,0x00001000U,1U,sizeof(caiplite_00_st_int_info)/sizeof(interrupt_struct_t),&caiplite_00_st_int_info,"invalid",sizeof(caiplite_00_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_00_st_utlb_info, &caiplite_00_st_pm_info},
    {"caiplite_01","caiplite",0xE7801000U,0x00001000U,1U,sizeof(caiplite_01_st_int_info)/sizeof(interrupt_struct_t),&caiplite_01_st_int_info,"invalid",sizeof(caiplite_01_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_01_st_utlb_info, &caiplite_01_st_pm_info},
    {"caiplite_02","caiplite",0xE7802000U,0x00001000U,1U,sizeof(caiplite_02_st_int_info)/sizeof(interrupt_struct_t),&caiplite_02_st_int_info,"invalid",sizeof(caiplite_02_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_02_st_utlb_info, &caiplite_02_st_pm_info},
    {"caiplite_03","caiplite",0xE7803000U,0x00001000U,1U,sizeof(caiplite_03_st_int_info)/sizeof(interrupt_struct_t),&caiplite_03_st_int_info,"invalid",sizeof(caiplite_03_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_03_st_utlb_info, &caiplite_03_st_pm_info},
    {"caiplite_04","caiplite",0xE7804000U,0x00001000U,1U,sizeof(caiplite_04_st_int_info)/sizeof(interrupt_struct_t),&caiplite_04_st_int_info,"invalid",sizeof(caiplite_04_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_04_st_utlb_info, &caiplite_04_st_pm_info},
    {"caiplite_05","caiplite",0xE7805000U,0x00001000U,1U,sizeof(caiplite_05_st_int_info)/sizeof(interrupt_struct_t),&caiplite_05_st_int_info,"invalid",sizeof(caiplite_05_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_05_st_utlb_info, &caiplite_05_st_pm_info},
    {"caiplite_06","caiplite",0xE7806000U,0x00001000U,1U,sizeof(caiplite_06_st_int_info)/sizeof(interrupt_struct_t),&caiplite_06_st_int_info,"invalid",sizeof(caiplite_06_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_06_st_utlb_info, &caiplite_06_st_pm_info},
    {"caiplite_07","caiplite",0xE7807000U,0x00001000U,1U,sizeof(caiplite_07_st_int_info)/sizeof(interrupt_struct_t),&caiplite_07_st_int_info,"invalid",sizeof(caiplite_07_st_utlb_info)/sizeof(assignment_struct_t),&caiplite_07_st_utlb_info, &caiplite_07_st_pm_info},
    {"wcrc_caiplite_00","wcrc_caiplite",0xE7080000U,0x00001000U,0U,sizeof(wcrc_caiplite_00_st_int_info)/sizeof(interrupt_struct_t),&wcrc_caiplite_00_st_int_info,"invalid",sizeof(wcrc_caiplite_00_st_utlb_info)/sizeof(assignment_struct_t),&wcrc_caiplite_00_st_utlb_info, &wcrc_caiplite_00_st_pm_info},
    {"wcrc_caiplite_01","wcrc_caiplite",0xE7090000U,0x00001000U,0U,sizeof(wcrc_caiplite_01_st_int_info)/sizeof(interrupt_struct_t),&wcrc_caiplite_01_st_int_info,"invalid",sizeof(wcrc_caiplite_01_st_utlb_info)/sizeof(assignment_struct_t),&wcrc_caiplite_01_st_utlb_info, &wcrc_caiplite_01_st_pm_info},
    {"wcrc_caiplite_02","wcrc_caiplite",0xE70A0000U,0x00001000U,0U,sizeof(wcrc_caiplite_02_st_int_info)/sizeof(interrupt_struct_t),&wcrc_caiplite_02_st_int_info,"invalid",sizeof(wcrc_caiplite_02_st_utlb_info)/sizeof(assignment_struct_t),&wcrc_caiplite_02_st_utlb_info, &wcrc_caiplite_02_st_pm_info},
    {"wcrc_caiplite_03","wcrc_caiplite",0xE70B0000U,0x00001000U,0U,sizeof(wcrc_caiplite_03_st_int_info)/sizeof(interrupt_struct_t),&wcrc_caiplite_03_st_int_info,"invalid",sizeof(wcrc_caiplite_03_st_utlb_info)/sizeof(assignment_struct_t),&wcrc_caiplite_03_st_utlb_info, &wcrc_caiplite_03_st_pm_info},
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
    {"rtdmac_control2_00","rtdmac_control2",0xFFD70000U,0x00001000U,1U,sizeof(rtdmac_control2_00_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_00_st_int_info,"rt1",sizeof(rtdmac_control2_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_00_st_utlb_info,&rtdmac_control2_00_st_pm_info},
    {"rtdmac_control2_01","rtdmac_control2",0xFFD71000U,0x00001000U,1U,sizeof(rtdmac_control2_01_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_01_st_int_info,"rt1",sizeof(rtdmac_control2_01_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_01_st_utlb_info,&rtdmac_control2_01_st_pm_info},
    {"rtdmac_control2_02","rtdmac_control2",0xFFD72000U,0x00001000U,1U,sizeof(rtdmac_control2_02_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_02_st_int_info,"rt1",sizeof(rtdmac_control2_02_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_02_st_utlb_info,&rtdmac_control2_02_st_pm_info},
    {"rtdmac_control2_03","rtdmac_control2",0xFFD73000U,0x00001000U,1U,sizeof(rtdmac_control2_03_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_03_st_int_info,"rt1",sizeof(rtdmac_control2_03_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_03_st_utlb_info,&rtdmac_control2_03_st_pm_info},
    {"rtdmac_control2_04","rtdmac_control2",0xFFD74000U,0x00001000U,1U,sizeof(rtdmac_control2_04_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_04_st_int_info,"rt1",sizeof(rtdmac_control2_04_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_04_st_utlb_info,&rtdmac_control2_04_st_pm_info},
    {"rtdmac_control2_05","rtdmac_control2",0xFFD75000U,0x00001000U,1U,sizeof(rtdmac_control2_05_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_05_st_int_info,"rt1",sizeof(rtdmac_control2_05_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_05_st_utlb_info,&rtdmac_control2_05_st_pm_info},
    {"rtdmac_control2_06","rtdmac_control2",0xFFD76000U,0x00001000U,1U,sizeof(rtdmac_control2_06_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_06_st_int_info,"rt1",sizeof(rtdmac_control2_06_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_06_st_utlb_info,&rtdmac_control2_06_st_pm_info},
    {"rtdmac_control2_07","rtdmac_control2",0xFFD77000U,0x00001000U,1U,sizeof(rtdmac_control2_07_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_07_st_int_info,"rt1",sizeof(rtdmac_control2_07_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_07_st_utlb_info,&rtdmac_control2_07_st_pm_info},
    {"rtdmac_control2_08","rtdmac_control2",0xFFD78000U,0x00001000U,1U,sizeof(rtdmac_control2_08_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_08_st_int_info,"rt1",sizeof(rtdmac_control2_08_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_08_st_utlb_info,&rtdmac_control2_08_st_pm_info},
    {"rtdmac_control2_09","rtdmac_control2",0xFFD79000U,0x00001000U,1U,sizeof(rtdmac_control2_09_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_09_st_int_info,"rt1",sizeof(rtdmac_control2_09_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_09_st_utlb_info,&rtdmac_control2_09_st_pm_info},
    {"rtdmac_control2_10","rtdmac_control2",0xFFD7A000U,0x00001000U,1U,sizeof(rtdmac_control2_10_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_10_st_int_info,"rt1",sizeof(rtdmac_control2_10_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_10_st_utlb_info,&rtdmac_control2_10_st_pm_info},
    {"rtdmac_control2_11","rtdmac_control2",0xFFD7B000U,0x00001000U,1U,sizeof(rtdmac_control2_11_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_11_st_int_info,"rt1",sizeof(rtdmac_control2_11_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_11_st_utlb_info,&rtdmac_control2_11_st_pm_info},
    {"rtdmac_control2_12","rtdmac_control2",0xFFD7C000U,0x00001000U,1U,sizeof(rtdmac_control2_12_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_12_st_int_info,"rt1",sizeof(rtdmac_control2_12_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_12_st_utlb_info,&rtdmac_control2_12_st_pm_info},
    {"rtdmac_control2_13","rtdmac_control2",0xFFD7D000U,0x00001000U,1U,sizeof(rtdmac_control2_13_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_13_st_int_info,"rt1",sizeof(rtdmac_control2_13_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_13_st_utlb_info,&rtdmac_control2_13_st_pm_info},
    {"rtdmac_control2_14","rtdmac_control2",0xFFD7E000U,0x00001000U,1U,sizeof(rtdmac_control2_14_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_14_st_int_info,"rt1",sizeof(rtdmac_control2_14_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_14_st_utlb_info,&rtdmac_control2_14_st_pm_info},
    {"rtdmac_control2_15","rtdmac_control2",0xFFD7F000U,0x00001000U,1U,sizeof(rtdmac_control2_15_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control2_15_st_int_info,"rt1",sizeof(rtdmac_control2_15_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control2_15_st_utlb_info,&rtdmac_control2_15_st_pm_info},
    {"rtdmac_control3_00","rtdmac_control3",0xFFD80000U,0x00001000U,1U,sizeof(rtdmac_control3_00_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_00_st_int_info,"rt1",sizeof(rtdmac_control3_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_00_st_utlb_info,&rtdmac_control3_00_st_pm_info},
    {"rtdmac_control3_01","rtdmac_control3",0xFFD81000U,0x00001000U,1U,sizeof(rtdmac_control3_01_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_01_st_int_info,"rt1",sizeof(rtdmac_control3_01_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_01_st_utlb_info,&rtdmac_control3_01_st_pm_info},
    {"rtdmac_control3_02","rtdmac_control3",0xFFD82000U,0x00001000U,1U,sizeof(rtdmac_control3_02_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_02_st_int_info,"rt1",sizeof(rtdmac_control3_02_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_02_st_utlb_info,&rtdmac_control3_02_st_pm_info},
    {"rtdmac_control3_03","rtdmac_control3",0xFFD83000U,0x00001000U,1U,sizeof(rtdmac_control3_03_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_03_st_int_info,"rt1",sizeof(rtdmac_control3_03_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_03_st_utlb_info,&rtdmac_control3_03_st_pm_info},
    {"rtdmac_control3_04","rtdmac_control3",0xFFD84000U,0x00001000U,1U,sizeof(rtdmac_control3_04_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_04_st_int_info,"rt1",sizeof(rtdmac_control3_04_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_04_st_utlb_info,&rtdmac_control3_04_st_pm_info},
    {"rtdmac_control3_05","rtdmac_control3",0xFFD85000U,0x00001000U,1U,sizeof(rtdmac_control3_05_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_05_st_int_info,"rt1",sizeof(rtdmac_control3_05_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_05_st_utlb_info,&rtdmac_control3_05_st_pm_info},
    {"rtdmac_control3_06","rtdmac_control3",0xFFD86000U,0x00001000U,1U,sizeof(rtdmac_control3_06_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_06_st_int_info,"rt1",sizeof(rtdmac_control3_06_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_06_st_utlb_info,&rtdmac_control3_06_st_pm_info},
    {"rtdmac_control3_07","rtdmac_control3",0xFFD87000U,0x00001000U,1U,sizeof(rtdmac_control3_07_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_07_st_int_info,"rt1",sizeof(rtdmac_control3_07_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_07_st_utlb_info,&rtdmac_control3_07_st_pm_info},
    {"rtdmac_control3_08","rtdmac_control3",0xFFD88000U,0x00001000U,1U,sizeof(rtdmac_control3_08_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_08_st_int_info,"rt1",sizeof(rtdmac_control3_08_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_08_st_utlb_info,&rtdmac_control3_08_st_pm_info},
    {"rtdmac_control3_09","rtdmac_control3",0xFFD89000U,0x00001000U,1U,sizeof(rtdmac_control3_09_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_09_st_int_info,"rt1",sizeof(rtdmac_control3_09_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_09_st_utlb_info,&rtdmac_control3_09_st_pm_info},
    {"rtdmac_control3_10","rtdmac_control3",0xFFD8A000U,0x00001000U,1U,sizeof(rtdmac_control3_10_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_10_st_int_info,"rt1",sizeof(rtdmac_control3_10_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_10_st_utlb_info,&rtdmac_control3_10_st_pm_info},
    {"rtdmac_control3_11","rtdmac_control3",0xFFD8B000U,0x00001000U,1U,sizeof(rtdmac_control3_11_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_11_st_int_info,"rt1",sizeof(rtdmac_control3_11_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_11_st_utlb_info,&rtdmac_control3_11_st_pm_info},
    {"rtdmac_control3_12","rtdmac_control3",0xFFD8C000U,0x00001000U,1U,sizeof(rtdmac_control3_12_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_12_st_int_info,"rt1",sizeof(rtdmac_control3_12_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_12_st_utlb_info,&rtdmac_control3_12_st_pm_info},
    {"rtdmac_control3_13","rtdmac_control3",0xFFD8D000U,0x00001000U,1U,sizeof(rtdmac_control3_13_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_13_st_int_info,"rt1",sizeof(rtdmac_control3_13_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_13_st_utlb_info,&rtdmac_control3_13_st_pm_info},
    {"rtdmac_control3_14","rtdmac_control3",0xFFD8E000U,0x00001000U,1U,sizeof(rtdmac_control3_14_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_14_st_int_info,"rt1",sizeof(rtdmac_control3_14_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_14_st_utlb_info,&rtdmac_control3_14_st_pm_info},
    {"rtdmac_control3_15","rtdmac_control3",0xFFD8F000U,0x00001000U,1U,sizeof(rtdmac_control3_15_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_control3_15_st_int_info,"rt1",sizeof(rtdmac_control3_15_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_control3_15_st_utlb_info,&rtdmac_control3_15_st_pm_info},
    {"rtdmac_descriptor0_00","rtdmac_descriptor0",0xFFD60000U,0x00001000U,0U,sizeof(rtdmac_descriptor_00_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_descriptor_00_st_int_info,"invalid",sizeof(rtdmac_descriptor0_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_descriptor0_00_st_utlb_info,&rtdmac_descriptor0_00_st_pm_info},
    {"rtdmac_descriptor1_00","rtdmac_descriptor1",0xFFD61000U,0x00001000U,0U,sizeof(rtdmac_descriptor_01_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_descriptor_01_st_int_info,"invalid",sizeof(rtdmac_descriptor1_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_descriptor1_00_st_utlb_info,&rtdmac_descriptor1_00_st_pm_info},
    {"rtdmac_descriptor2_00","rtdmac_descriptor2",0xFFD62000U,0x00001000U,0U,sizeof(rtdmac_descriptor_02_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_descriptor_02_st_int_info,"invalid",sizeof(rtdmac_descriptor2_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_descriptor2_00_st_utlb_info,&rtdmac_descriptor2_00_st_pm_info},
    {"rtdmac_descriptor3_00","rtdmac_descriptor3",0xFFD63000U,0x00001000U,0U,sizeof(rtdmac_descriptor_03_st_int_info)/sizeof(interrupt_struct_t),&rtdmac_descriptor_03_st_int_info,"invalid",sizeof(rtdmac_descriptor3_00_st_utlb_info)/sizeof(assignment_struct_t),&rtdmac_descriptor3_00_st_utlb_info,&rtdmac_descriptor3_00_st_pm_info},
    {"ships_00","ships",0xE7810000U,0x00010000U,2U,sizeof(ships_00_st_int_info)/sizeof(interrupt_struct_t),&ships_00_st_int_info,"invalid",sizeof(ships_00_st_utlb_info)/sizeof(assignment_struct_t),&ships_00_st_utlb_info,&ships_00_st_pm_info},
    {"ships_01","ships",0xE7820000U,0x00010000U,2U,sizeof(ships_01_st_int_info)/sizeof(interrupt_struct_t),&ships_01_st_int_info,"invalid",sizeof(ships_01_st_utlb_info)/sizeof(assignment_struct_t),&ships_01_st_utlb_info,&ships_01_st_pm_info},
    {"ships_02","ships",0xE7830000U,0x00010000U,2U,sizeof(ships_02_st_int_info)/sizeof(interrupt_struct_t),&ships_02_st_int_info,"invalid",sizeof(ships_02_st_utlb_info)/sizeof(assignment_struct_t),&ships_02_st_utlb_info,&ships_02_st_pm_info},
    {"iccom_reg_00","iccom_reg",0xE6260000U,0x00010000U,0U,sizeof(iccom_reg_00_st_int_info)/sizeof(interrupt_struct_t),&iccom_reg_00_st_int_info,"invalid",sizeof(iccom_reg_00_st_utlb_info)/sizeof(assignment_struct_t),&iccom_reg_00_st_utlb_info,&iccom_reg_00_st_pm_info},
    {"iccom_00","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_00_st_int_info)/sizeof(interrupt_struct_t),&iccom_00_st_int_info,"invalid",sizeof(iccom_00_st_int_info)/sizeof(assignment_struct_t),&iccom_00_st_utlb_info,&iccom_00_st_pm_info},
    {"iccom_01","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_01_st_int_info)/sizeof(interrupt_struct_t),&iccom_01_st_int_info,"invalid",sizeof(iccom_01_st_int_info)/sizeof(assignment_struct_t),&iccom_01_st_utlb_info,&iccom_01_st_pm_info},
    {"iccom_02","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_02_st_int_info)/sizeof(interrupt_struct_t),&iccom_02_st_int_info,"invalid",sizeof(iccom_02_st_int_info)/sizeof(assignment_struct_t),&iccom_02_st_utlb_info,&iccom_02_st_pm_info},
    {"iccom_03","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_03_st_int_info)/sizeof(interrupt_struct_t),&iccom_03_st_int_info,"invalid",sizeof(iccom_03_st_utlb_info)/sizeof(assignment_struct_t),&iccom_03_st_utlb_info,&iccom_03_st_pm_info},
    {"iccom_04","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_04_st_int_info)/sizeof(interrupt_struct_t),&iccom_04_st_int_info,"invalid",sizeof(iccom_04_st_utlb_info)/sizeof(assignment_struct_t),&iccom_04_st_utlb_info,&iccom_04_st_pm_info},
    {"iccom_05","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_05_st_int_info)/sizeof(interrupt_struct_t),&iccom_05_st_int_info,"invalid",sizeof(iccom_05_st_utlb_info)/sizeof(assignment_struct_t),&iccom_05_st_utlb_info,&iccom_05_st_pm_info},
    {"iccom_06","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_06_st_int_info)/sizeof(interrupt_struct_t),&iccom_06_st_int_info,"invalid",sizeof(iccom_06_st_utlb_info)/sizeof(assignment_struct_t),&iccom_06_st_utlb_info,&iccom_06_st_pm_info},
    {"iccom_07","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_07_st_int_info)/sizeof(interrupt_struct_t),&iccom_07_st_int_info,"invalid",sizeof(iccom_07_st_utlb_info)/sizeof(assignment_struct_t),&iccom_07_st_utlb_info,&iccom_07_st_pm_info},
    {"iccom_08","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_08_st_int_info)/sizeof(interrupt_struct_t),&iccom_08_st_int_info,"invalid",sizeof(iccom_08_st_utlb_info)/sizeof(assignment_struct_t),&iccom_08_st_utlb_info,&iccom_08_st_pm_info},
    {"iccom_09","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_09_st_int_info)/sizeof(interrupt_struct_t),&iccom_09_st_int_info,"invalid",sizeof(iccom_09_st_utlb_info)/sizeof(assignment_struct_t),&iccom_09_st_utlb_info,&iccom_09_st_pm_info},
    {"iccom_10","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_10_st_int_info)/sizeof(interrupt_struct_t),&iccom_10_st_int_info,"invalid",sizeof(iccom_10_st_utlb_info)/sizeof(assignment_struct_t),&iccom_10_st_utlb_info,&iccom_10_st_pm_info},
    {"iccom_11","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_11_st_int_info)/sizeof(interrupt_struct_t),&iccom_11_st_int_info,"invalid",sizeof(iccom_11_st_utlb_info)/sizeof(assignment_struct_t),&iccom_11_st_utlb_info,&iccom_11_st_pm_info},
    {"iccom_12","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_12_st_int_info)/sizeof(interrupt_struct_t),&iccom_12_st_int_info,"invalid",sizeof(iccom_12_st_utlb_info)/sizeof(assignment_struct_t),&iccom_12_st_utlb_info,&iccom_12_st_pm_info},
    {"iccom_13","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_13_st_int_info)/sizeof(interrupt_struct_t),&iccom_13_st_int_info,"invalid",sizeof(iccom_13_st_utlb_info)/sizeof(assignment_struct_t),&iccom_13_st_utlb_info,&iccom_13_st_pm_info},
    {"iccom_14","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_14_st_int_info)/sizeof(interrupt_struct_t),&iccom_14_st_int_info,"invalid",sizeof(iccom_14_st_utlb_info)/sizeof(assignment_struct_t),&iccom_14_st_utlb_info,&iccom_14_st_pm_info},
    {"iccom_15","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_15_st_int_info)/sizeof(interrupt_struct_t),&iccom_15_st_int_info,"invalid",sizeof(iccom_15_st_utlb_info)/sizeof(assignment_struct_t),&iccom_15_st_utlb_info,&iccom_15_st_pm_info},
    {"iccom_16","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_16_st_int_info)/sizeof(interrupt_struct_t),&iccom_16_st_int_info,"invalid",sizeof(iccom_16_st_utlb_info)/sizeof(assignment_struct_t),&iccom_16_st_utlb_info,&iccom_16_st_pm_info},
    {"iccom_17","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_17_st_int_info)/sizeof(interrupt_struct_t),&iccom_17_st_int_info,"invalid",sizeof(iccom_17_st_utlb_info)/sizeof(assignment_struct_t),&iccom_17_st_utlb_info,&iccom_17_st_pm_info},
    {"iccom_18","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_18_st_int_info)/sizeof(interrupt_struct_t),&iccom_18_st_int_info,"invalid",sizeof(iccom_18_st_utlb_info)/sizeof(assignment_struct_t),&iccom_18_st_utlb_info,&iccom_18_st_pm_info},
    {"iccom_19","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_19_st_int_info)/sizeof(interrupt_struct_t),&iccom_19_st_int_info,"invalid",sizeof(iccom_19_st_utlb_info)/sizeof(assignment_struct_t),&iccom_19_st_utlb_info,&iccom_19_st_pm_info},
    {"iccom_20","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_20_st_int_info)/sizeof(interrupt_struct_t),&iccom_20_st_int_info,"invalid",sizeof(iccom_20_st_utlb_info)/sizeof(assignment_struct_t),&iccom_20_st_utlb_info,&iccom_20_st_pm_info},
    {"iccom_21","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_21_st_int_info)/sizeof(interrupt_struct_t),&iccom_21_st_int_info,"invalid",sizeof(iccom_21_st_utlb_info)/sizeof(assignment_struct_t),&iccom_21_st_utlb_info,&iccom_21_st_pm_info},
    {"iccom_22","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_22_st_int_info)/sizeof(interrupt_struct_t),&iccom_22_st_int_info,"invalid",sizeof(iccom_22_st_utlb_info)/sizeof(assignment_struct_t),&iccom_22_st_utlb_info,&iccom_22_st_pm_info},
    {"iccom_23","iccom",0x00000000U,0x00000000U,1U,sizeof(iccom_23_st_int_info)/sizeof(interrupt_struct_t),&iccom_23_st_int_info,"invalid",sizeof(iccom_23_st_utlb_info)/sizeof(assignment_struct_t),&iccom_23_st_utlb_info,&iccom_23_st_pm_info},
    {"system_ram_00","system_ram",0xE6300000U,0x00060000U,0U,sizeof(system_ram_00_st_int_info)/sizeof(interrupt_struct_t),&system_ram_00_st_int_info,"invalid",sizeof(system_ram_00_st_utlb_info)/sizeof(assignment_struct_t),&system_ram_00_st_utlb_info,&system_ram_00_st_pm_info},
    {"icumx_cmd_00","icumx_cmd",0xE6610000U,0x00008000U,20U,sizeof(icumx_cmd_00_st_int_info)/sizeof(interrupt_struct_t),&icumx_cmd_00_st_int_info,"invalid",sizeof(icumx_cmd_00_st_utlb_info)/sizeof(assignment_struct_t),&icumx_cmd_00_st_utlb_info,&icumx_cmd_00_st_pm_info},
    {"gicd_00","gicd",0xF1000000U,0x00010000U,0U,sizeof(gicd_00_st_int_info)/sizeof(interrupt_struct_t),&gicd_00_st_int_info,"invalid",sizeof(gicd_00_st_utlb_info)/sizeof(assignment_struct_t),&gicd_00_st_utlb_info,&gicd_00_st_pm_info},
    {"fbc_00","fbc",0xFF830000,0x00002000U,1U,sizeof(fbc_00_st_int_info)/sizeof(interrupt_struct_t),&fbc_00_st_int_info,"invalid",sizeof(fbc_00_st_utlb_info)/sizeof(assignment_struct_t),&fbc_00_st_utlb_info,&fbc_00_st_pm_info},
    {"fba_ca55d0","fba",0xFF870000,0x00001000U,1U,sizeof(fba_ca55d0_st_int_info)/sizeof(interrupt_struct_t),&fba_ca55d0_st_int_info,"invalid",sizeof(fba_ca55d0_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca55d0_st_utlb_info,&fba_ca55d0_st_pm_info},
    {"fba_ca55d1","fba",0xFF871000,0x00001000U,1U,sizeof(fba_ca55d1_st_int_info)/sizeof(interrupt_struct_t),&fba_ca55d1_st_int_info,"invalid",sizeof(fba_ca55d1_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca55d1_st_utlb_info,&fba_ca55d1_st_pm_info},
    {"fba_ca550","fba",0xFF872000,0x00001000U,1U,sizeof(fba_ca550_st_int_info)/sizeof(interrupt_struct_t),&fba_ca550_st_int_info,"invalid",sizeof(fba_ca550_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca550_st_utlb_info,&fba_ca550_st_pm_info},
    {"fba_ca551","fba",0xFF873000,0x00001000U,1U,sizeof(fba_ca551_st_int_info)/sizeof(interrupt_struct_t),&fba_ca551_st_int_info,"invalid",sizeof(fba_ca551_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca551_st_utlb_info,&fba_ca551_st_pm_info},
    {"fba_ca552","fba",0xFF874000,0x00001000U,1U,sizeof(fba_ca552_st_int_info)/sizeof(interrupt_struct_t),&fba_ca552_st_int_info,"invalid",sizeof(fba_ca552_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca552_st_utlb_info,&fba_ca552_st_pm_info},
    {"fba_ca553","fba",0xFF875000,0x00001000U,1U,sizeof(fba_ca553_st_int_info)/sizeof(interrupt_struct_t),&fba_ca553_st_int_info,"invalid",sizeof(fba_ca553_st_utlb_info)/sizeof(assignment_struct_t),&fba_ca553_st_utlb_info,&fba_ca553_st_pm_info},
    {"rfso_00","rfso",0xFFE80000,0x00001000U,1U,sizeof(rfso_00_st_int_info)/sizeof(interrupt_struct_t),&rfso_00_st_int_info,"invalid",sizeof(rfso_00_st_utlb_info)/sizeof(assignment_struct_t),&rfso_00_st_utlb_info,&rfso_00_st_pm_info},
    {"rfso_01","rfso",0xFFE81000,0x00001000U,1U,sizeof(rfso_01_st_int_info)/sizeof(interrupt_struct_t),&rfso_01_st_int_info,"invalid",sizeof(rfso_01_st_utlb_info)/sizeof(assignment_struct_t),&rfso_01_st_utlb_info,&rfso_01_st_pm_info},
    {"rfso_02","rfso",0xFFE82000,0x00001000U,1U,sizeof(rfso_02_st_int_info)/sizeof(interrupt_struct_t),&rfso_02_st_int_info,"invalid",sizeof(rfso_02_st_utlb_info)/sizeof(assignment_struct_t),&rfso_02_st_utlb_info,&rfso_02_st_pm_info},
    {"rfso_03","rfso",0xFFE83000,0x00001000U,1U,sizeof(rfso_03_st_int_info)/sizeof(interrupt_struct_t),&rfso_03_st_int_info,"invalid",sizeof(rfso_03_st_utlb_info)/sizeof(assignment_struct_t),&rfso_03_st_utlb_info,&rfso_03_st_pm_info},
    {"rfso_04","rfso",0xFFE84000,0x00001000U,1U,sizeof(rfso_04_st_int_info)/sizeof(interrupt_struct_t),&rfso_04_st_int_info,"invalid",sizeof(rfso_04_st_utlb_info)/sizeof(assignment_struct_t),&rfso_04_st_utlb_info,&rfso_04_st_pm_info},
    {"rfso_05","rfso",0xFFE85000,0x00001000U,1U,sizeof(rfso_05_st_int_info)/sizeof(interrupt_struct_t),&rfso_05_st_int_info,"invalid",sizeof(rfso_05_st_utlb_info)/sizeof(assignment_struct_t),&rfso_05_st_utlb_info,&rfso_05_st_pm_info},
    {"rfso_06","rfso",0xFFE86000,0x00001000U,1U,sizeof(rfso_06_st_int_info)/sizeof(interrupt_struct_t),&rfso_06_st_int_info,"invalid",sizeof(rfso_06_st_utlb_info)/sizeof(assignment_struct_t),&rfso_06_st_utlb_info,&rfso_06_st_pm_info},
    {"rfso_07","rfso",0xFFE87000,0x00001000U,1U,sizeof(rfso_07_st_int_info)/sizeof(interrupt_struct_t),&rfso_07_st_int_info,"invalid",sizeof(rfso_07_st_utlb_info)/sizeof(assignment_struct_t),&rfso_07_st_utlb_info,&rfso_07_st_pm_info},
    {"rfso_08","rfso",0xFFE88000,0x00001000U,1U,sizeof(rfso_08_st_int_info)/sizeof(interrupt_struct_t),&rfso_08_st_int_info,"invalid",sizeof(rfso_08_st_utlb_info)/sizeof(assignment_struct_t),&rfso_08_st_utlb_info,&rfso_08_st_pm_info},
    {"rfso_09","rfso",0xFFE89000,0x00001000U,1U,sizeof(rfso_09_st_int_info)/sizeof(interrupt_struct_t),&rfso_09_st_int_info,"invalid",sizeof(rfso_09_st_utlb_info)/sizeof(assignment_struct_t),&rfso_09_st_utlb_info,&rfso_09_st_pm_info},
    {"rfso_10","rfso",0xFFE8A000,0x00001000U,1U,sizeof(rfso_10_st_int_info)/sizeof(interrupt_struct_t),&rfso_10_st_int_info,"invalid",sizeof(rfso_10_st_utlb_info)/sizeof(assignment_struct_t),&rfso_10_st_utlb_info,&rfso_10_st_pm_info},
    {NULL,NULL,0x00000000U,0x00000000U,0U,0U,NULL,"invalid",0U,NULL,NULL},

};
