/***********************************************************************************************************************
* Copyright [2022-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_os_dev_cfg_tbl_v4h.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car V4H
* Description :  Device Configuration file for R-Car V4H
***********************************************************************************************************************/

#ifndef OSAL_OS_DEV_CFG_V4H_H
#define OSAL_OS_DEV_CFG_V4H_H

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_014
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_int_ims
 * @brief osal device configuration : IMS interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ims_v4h[2][1] =
{
    {
        {579}
    },
    {
        {580}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_imr
 * @brief osal device configuration : IMR interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imr_v4h[3][1] =
{
    {
        {581}
    },
    {
        {582}
    },
    {
        {583}
    },
};

/*******************************************************************************************************************//**
 * @var gs_int_imp_top_v4h
 * @brief osal device configuration : IMP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imp_top_v4h[3] =
{
    {565}, {566}, {567}
};

/*******************************************************************************************************************//**
 * @var gs_int_vdsp_v4h
 * @brief osal device configuration : vdsp interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_vdsp_v4h[4][5] =
{
    {
        {478}, {479}, {480}, {481}, {482}
    },
    {
        {483}, {484}, {485}, {486}, {487}
    },
    {
        {488}, {489}, {490}, {491}, {492}
    },
    {
        {493}, {494}, {495}, {496}, {497}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_dof_v4h
 * @brief osal device configuration : DOF interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_dof_v4h[3][2] =
{
    {
        {861}, {862}
    },
    {
        {873}, {874}
    },
    {
        {871}, {872}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_gsx_v4h
 * @brief osal device configuration : gsx interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_gsx_v4h[1] =
{
    {464}
};

/*******************************************************************************************************************//**
 * @var gs_int_iccom_v4h
 * @brief osal device configuration : ICCOM interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_iccom_v4h[12][1] =
{
    {
        {123}
    },
    {
        {125}
    },
    {
        {127}
    },
    {
        {129}
    },
    {
        {131}
    },
    {
        {133}
    },
    {
        {135}
    },
    {
        {137}
    },
    {
        {916}
    },
    {
        {918}
    },
    {
        {920}
    },
    {
        {922}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_ivcp1e_v4h
 * @brief osal device configuration : IVCP1E interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ivcp1e_v4h[2] =
{
    {855}, {856}
};

/*******************************************************************************************************************//**
 * @var gs_int_vcp4
 * @brief osal device configuration : VCP4 interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_vcp4_v4h[2] =
{
    {858}, {859}
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_cisp_v4h
 * @brief osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_cisp_v4h[2][1] =
{
    {
        {475}
    },
    {
        {476}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_tisp_v4h
 * @brief osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_tisp_v4h[2][1] =
{
    {
        {473}
    },
    {
        {474}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_vspx_v4h
 * @brief osal device configuration : ISP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_isp_vspx_v4h[2][1] =
{
    {
        {556}
    },
    {
        {557}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_fbc_v4h
 * @brief osal device configuration : FBC interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbc_v4h[1] =
{
    {852}
};

/*******************************************************************************************************************//**
 * @var gs_int_fba_v4h
 * @brief osal device configuration : FBA interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fba_v4h[35][1] =
{
    {
        {695}
    },
    {
        {696}
    },
    {
        {691}
    },
    {
        {692}
    },
    {
        {693}
    },
    {
        {713}
    },
    {
        {714}
    },
    {
        {715}
    },
    {
        {716}
    },
    {
        {719}
    },
    {
        {720}
    },
    {
        {721}
    },
    {
        {722}
    },
    {
        {717}
    },
    {
        {718}
    },
    {
        {727}
    },
    {
        {738}
    },
    {
        {739}
    },
    {
        {740}
    },
    {
        {741}
    },
    {
        {744}
    },
    {
        {745}
    },
    {
        {746}
    },
    {
        {701}
    },
    {
        {703}
    },
    {
        {707}
    },
    {
        {699}
    },
    {
        {686}
    },
    {
        {687}
    },
    {
        {669}
    },
    {
        {670}
    },
    {
        {671}
    },
    {
        {672}
    },
    {
        {673}
    },
    {
        {674}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtt_v4h
 * @brief osal device configuration : RTT interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtt_v4h[11][1] =
{
    {
        {227}
    },
    {
        {228}
    },
    {
        {229}
    },
    {
        {230}
    },
    {
        {231}
    },
    {
        {232}
    },
    {
        {233}
    },
    {
        {234}
    },
    {
        {235}
    },
    {
        {236}
    },
    {
        {237}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_ipmmu_v4h
 * @brief osal device configuration : IPMMU interrupt
***********************************************************************************************************************/
    static st_osal_interrupt_info_t gs_int_ipmmu_v4h[2] =
{
    {210}, {211}
};
/*******************************************************************************************************************//**
 * @var gs_int_caiplite_v4h
 * @brief osal device configuration : caiplite interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_caiplite_v4h[8][1] =
{
    {
        {900}
    },
    {
        {902}
    },
    {
        {904}
    },
    {
        {906}
    },
    {
        {908}
    },
    {
        {910}
    },
    {
        {912}
    },
    {
        {914}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtdmac_control0_v4h
 * @brief osal device configuration : rtdmac_control interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtdmac_control0_v4h[16][1] =
{
    {
        { 10}
    },
    {
        { 11}
    },
    {
        { 12}
    },
    {
        { 13}
    },
    {
        { 14}
    },
    {
        { 15}
    },
    {
        { 16}
    },
    {
        { 17}
    },
    {
        { 18}
    },
    {
        { 19}
    },
    {
        { 20}
    },
    {
        { 21}
    },
    {
        { 22}
    },
    {
        { 23}
    },
    {
        { 24}
    },
    {
        { 25}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtdmac_control1_v4h
 * @brief osal device configuration : rtdmac_control interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtdmac_control1_v4h[16][1] =
{
    {
        { 28}
    },
    {
        { 29}
    },
    {
        { 30}
    },
    {
        { 31}
    },
    {
        { 32}
    },
    {
        { 33}
    },
    {
        { 34}
    },
    {
        { 35}
    },
    {
        { 36}
    },
    {
        { 37}
    },
    {
        { 38}
    },
    {
        { 39}
    },
    {
        { 40}
    },
    {
        { 41}
    },
    {
        { 42}
    },
    {
        { 43}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtdmac_control2_v4h
 * @brief osal device configuration : rtdmac_control interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtdmac_control2_v4h[16][1] =
{
    {
        { 46}
    },
    {
        { 47}
    },
    {
        { 48}
    },
    {
        { 49}
    },
    {
        { 50}
    },
    {
        { 51}
    },
    {
        { 52}
    },
    {
        { 53}
    },
    {
        { 54}
    },
    {
        { 55}
    },
    {
        { 56}
    },
    {
        { 57}
    },
    {
        { 58}
    },
    {
        { 59}
    },
    {
        { 60}
    },
    {
        { 61}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtdmac_control3_v4h
 * @brief osal device configuration : rtdmac_control interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtdmac_control3_v4h[16][1] =
{
    {
        { 64}
    },
    {
        { 65}
    },
    {
        { 66}
    },
    {
        { 67}
    },
    {
        { 68}
    },
    {
        { 69}
    },
    {
        { 70}
    },
    {
        { 71}
    },
    {
        { 72}
    },
    {
        { 73}
    },
    {
        { 74}
    },
    {
        { 75}
    },
    {
        { 76}
    },
    {
        { 77}
    },
    {
        { 78}
    },
    {
        { 79}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_vin_v4h
 * @brief osal device configuration : vin interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_vin_v4h[16][1] =
{
    {
        {529}
    },
    {
        {530}
    },
    {
        {531}
    },
    {
        {532}
    },
    {
        {533}
    },
    {
        {534}
    },
    {
        {535}
    },
    {
        {536}
    },
    {
        {537}
    },
    {
        {538}
    },
    {
        {539}
    },
    {
        {540}
    },
    {
        {541}
    },
    {
        {542}
    },
    {
        {543}
    },
    {
        {544}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_vsp_v4h
 * @brief osal device configuration : vsp interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_vsp_v4h[2][1] =
{
    {
        {546}
    },
    {
        {551}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_csi_v4h
 * @brief osal device configuration : csi interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_csi_v4h[2][1] =
{
    {
        {499}
    },
    {
        {500}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_du_v4h
 * @brief osal device configuration : du interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_du_v4h[2] =
{
    {523}, {524}
};

/*******************************************************************************************************************//**
 * @var gs_int_dsi_v4h
 * @brief osal device configuration : dsi interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_dsi_v4h[2][9] =
{
    {
        {502}, {503}, {504}, {505}, {506}, {507}, {508}, {509}, {510}
    },
    {
        {511}, {512}, {513}, {514}, {515}, {516}, {517}, {518}, {519}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_wwdt_v4h
 * @brief osal device configuration : wwdt interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_wwdt_v4h[10][2] =
{
    {
        {310}, {311}
    },
    {
        {312}, {313}
    },
    {
        {314}, {315}
    },
    {
        {316}, {317}
    },
    {
        {318}, {319}
    },
    {
        {320}, {321}
    },
    {
        {322}, {323}
    },
    {
        {324}, {325}
    },
    {
        {326}, {327}
    },
    {
        {328}, {329}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_doc_v4h
 * @brief osal device configuration : doc interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_doc_v4h[2] =
{
    {526}, {527}
};

/*******************************************************************************************************************//**
 * @var gs_int_dsc_v4h
 * @brief osal device configuration : dsc interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_dsc_v4h[1] =
{
    {559}
};

/*******************************************************************************************************************//**
 * @var gs_int_sdmac_v4h
 * osal device configuration : sdmac interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_sdmac_v4h[2] =
{
    {213}, {214}
};

/*******************************************************************************************************************//**
 * @var gs_int_fbs_v4h
 * @brief osal device configuration : fbs interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbs_v4h[1] =
{
    {223}
};

/*******************************************************************************************************************//**
 * @var gs_int_fbs_v4h
 * @brief osal device configuration : fbs interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_icumx_v4h[20] =
{
    {878},{879},{880},{881},{882},{883},{884},{885},{886},{887},
    {888},{889},{890},{891},{892},{893},{894},{895},{896},{897}
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_032]
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @var gs_utlb_vi0_vin_ch0_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi0_vin_ch0_v4h =
{
    1, { 0 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi0_vin_ch1_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vi0_vin_ch1_v4h =
{
    1, { 1 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip0_smpo_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vip0_smpo_v4h =
{
    1, { 1 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip0_smps_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vip0_smps_v4h =
{
    1, { 2 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip0_dof_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vip0_dof_v4h =
{
    1, { 3 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip0_dof_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vip0_wrap_v4h =
{
    3, { 1, 2, 3 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_fcpvd_ch0_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vi1_fcpvd_ch0_v4h =
{
    1, { 6 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_fcpvd_ch1_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vi1_fcpvd_ch1_v4h =
{
    1, { 7 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr0_ch0_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr0_ch0_v4h =
{
    1, { 8 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr1_ch0_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr1_ch0_v4h =
{
    1, { 9 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_ivcp1e_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vc_ivcp1e_v4h =
{
    1, { 14 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip1_pap_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vip1_pap_v4h =
{
    1, { 16 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_dsi_ch0_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vi1_dsi_ch0_v4h =
{
    1, { 18 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_fcpvx_ch0_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vi1_fcpvx_ch0_v4h =
{
    1, { 24 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_fcpvx_ch1_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vi1_fcpvx_ch1_v4h =
{
    1, { 25 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr2_ch0ch1_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr2_ch0ch1_v4h =
{
    2, { 0, 4 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr3_ch0ch1_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr3_ch0ch1_v4h =
{
    2, { 1, 5 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr4_ch0ch1_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr4_ch0ch1_v4h =
{
    2, { 2, 6 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi0_isp0_ch0ch1ch2ch3ch4_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vi0_isp0_ch0ch1ch2ch3ch4_v4h =
{
    5, { 10, 14, 18, 22, 26 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi0_isp1_ch0ch1ch2ch3ch4_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_vi0_isp1_ch0ch1ch2ch3ch4_v4h =
{
    5, { 11, 15, 19, 23, 27 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_ir_dspp_dspd_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_ir_dspp_dspd_v4h =
{
    2, { 13, 14 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_3dg_3dge_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_3dg_3dge_v4h =
{
    8, { 0, 1, 2, 3, 4, 5, 6, 7 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_00_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_00_v4h =
{
    1, { 0}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_01_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_01_v4h =
{
    1, {  1}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_02_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_02_v4h =
{
    1, {  2}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_03_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_03_v4h =
{
    1, {  3}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_04_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_04_v4h =
{
    1, {  4}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_05_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_05_v4h =
{
    1, {  5}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_06_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_06_v4h =
{
    1, {  6}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_07_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_07_v4h =
{
    1, {  7}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_08_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_08_v4h =
{
    1, {  8}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_09_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_09_v4h =
{
    1, {  9}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_10_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_10_v4h =
{
    1, { 10}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_11_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_11_v4h =
{
    1, { 11}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_12_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_12_v4h =
{
    1, { 12}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_13_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_13_v4h =
{
    1, { 13}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_14_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_14_v4h =
{
    1, { 14}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_15_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_15_v4h =
{
    1, { 15}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_00_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_00_v4h =
{
    1, { 16}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_01_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_01_v4h =
{
    1, { 17}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_02_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_02_v4h =
{
    1, { 18}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_03_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_03_v4h =
{
    1, { 19}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_04_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_04_v4h =
{
    1, { 20}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_05_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_05_v4h =
{
    1, { 21}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_06_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_06_v4h =
{
    1, { 22}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_07_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_07_v4h =
{
    1, { 23}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_08_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_08_v4h =
{
    1, { 24}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_09_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_09_v4h =
{
    1, { 25}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_10_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_10_v4h =
{
    1, { 26}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_11_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_11_v4h =
{
    1, { 27}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_12_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_12_v4h =
{
    1, { 28}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_13_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_13_v4h =
{
    1, { 29}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_14_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_14_v4h =
{
    1, { 30}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_15_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_15_v4h =
{
    1, { 31}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_ir_imp_ch0ch1_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_ir_imp_ch0ch1_v4h =
{
    16, { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_ir_imp_ch0_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_ir_imp_ch0_v4h =
{
    8, { 16, 17, 18, 19, 20, 21, 22, 23 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_ir_imp_ch1_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_ir_imp_ch1_v4h =
{
    8, { 24, 25, 26, 27, 28, 29, 30, 31 }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_00_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_00_v4h =
{
    1, { 32}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_01_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_01_v4h =
{
    1, { 33}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_02_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_02_v4h =
{
    1, { 34}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_03_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_03_v4h =
{
    1, { 35}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_04_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_04_v4h =
{
    1, { 36}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_05_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_05_v4h =
{
    1, { 37}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_06_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_06_v4h =
{
    1, { 38}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_07_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_07_v4h =
{
    1, { 39}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_08_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_08_v4h =
{
    1, { 40}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_09_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_09_v4h =
{
    1, { 41}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_10_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_10_v4h =
{
    1, { 42}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_11_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_11_v4h =
{
    1, { 43}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_12_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_12_v4h =
{
    1, { 44}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_13_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_13_v4h =
{
    1, { 45}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_14_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_14_v4h =
{
    1, { 46}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_15_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_15_v4h =
{
    1, { 47}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_00_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_00_v4h =
{
    1, { 48}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_01_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_01_v4h =
{
    1, { 49}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_02_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_02_v4h =
{
    1, { 50}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_03_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_03_v4h =
{
    1, { 51}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_04_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_04_v4h =
{
    1, { 52}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_05_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_05_v4h =
{
    1, { 53}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_06_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_06_v4h =
{
    1, { 54}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_07_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_07_v4h =
{
    1, { 55}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_08_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_08_v4h =
{
    1, { 56}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_09_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_09_v4h =
{
    1, { 57}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_10_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_10_v4h =
{
    1, { 58}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_11_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_11_v4h =
{
    1, { 59}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_12_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_12_v4h =
{
    1, { 60}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_13_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_13_v4h =
{
    1, { 61}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_14_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_14_v4h =
{
    1, { 62}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_15_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_15_v4h =
{
    1, { 63}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt0_icumx_v4h
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt0_icumx_v4h =
{
    2, { 1,2}
};

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_015
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_device_info_v4h
 * @brief osal device configuration : v4 device information structure
***********************************************************************************************************************/
/* PRQA S 3218 1 # Definition within a function reduces readability and maintainability, so no change. */
static const st_osal_device_info_t gs_osal_device_info_v4h[] =
{
    /* group name,   device unique ID,      device type,    physical address,    range,     interrupt,    number of interrupts, power manager id,               POSTCKCR,               AXI bus name,    assignment */
    /* IMR */
    {NULL,           "ims_00",                "ims",                0xFE860000U, 0x00010000U, gs_int_ims_v4h[0],           1U, R_PMA_HWA_ID_V4H_IMS0,         OSAL_PM_POSTCKCR_INVALID, "vc",          &gs_utlb_vc_imr0_ch0_v4h},
    {NULL,           "ims_01",                "ims",                0xFE870000U, 0x00010000U, gs_int_ims_v4h[1],           1U, R_PMA_HWA_ID_V4H_IMS1,         OSAL_PM_POSTCKCR_INVALID, "vc",          &gs_utlb_vc_imr1_ch0_v4h},
    {NULL,           "imr_00",                "imr",                0xFE880000U, 0x00010000U, gs_int_imr_v4h[0],           1U, R_PMA_HWA_ID_V4H_IMR0,         OSAL_PM_POSTCKCR_INVALID, "vc",          &gs_utlb_vc_imr2_ch0ch1_v4h},
    {NULL,           "imr_01",                "imr",                0xFE890000U, 0x00010000U, gs_int_imr_v4h[1],           1U, R_PMA_HWA_ID_V4H_IMR1,         OSAL_PM_POSTCKCR_INVALID, "vc",          &gs_utlb_vc_imr3_ch0ch1_v4h},
    {NULL,           "imr_02",                "imr",                0xFE8A0000U, 0x00010000U, gs_int_imr_v4h[2],           1U, R_PMA_HWA_ID_V4H_IMR2,         OSAL_PM_POSTCKCR_INVALID, "vc",          &gs_utlb_vc_imr4_ch0ch1_v4h},

     /* IMP */
    {NULL,           "imp_top_00",            "imp_top",            0xFF900000U, 0x00006000U, gs_int_imp_top_v4h,          3U, R_PMA_HWA_ID_V4H_IMPSLV,       OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0ch1_v4h},
    {NULL,           "imp_00",                "imp",                0xFFA00000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMP0,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_01",                "imp",                0xFFA20000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMP1,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_02",                "imp",                0xFFB00000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMP2,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_03",                "imp",                0xFFB20000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMP3,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_ocv_00",            "imp_ocv",            0xFFA40000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_V4H_CVE0,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_ocv_01",            "imp_ocv",            0xFFA50000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_V4H_CVE1,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_ocv_02",            "imp_ocv",            0xFFA60000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_V4H_CVE2,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_ocv_03",            "imp_ocv",            0xFFA70000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_V4H_CVE3,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_dma_00",            "imp_dma",            0xFFA80000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMPDMAC0,     OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_dma_01",            "imp_dma",            0xFFB80000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMPDMAC1,     OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_psc_00",            "imp_psc",            0xFFA84000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMPPSC,       OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_dta_00",            "imp_dta",            0xFFA88000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMPDTA,       OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch0_v4h},
    {NULL,           "imp_dma_slim_00",       "imp_dma_slim",       0xFFA90000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMPSDMAC0,    OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_dma_slim_01",       "imp_dma_slim",       0xFFA94000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMPSDMAC1,    OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_cnn_00",            "imp_cnn",            0xFFAA0000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IMPCNN,       OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_reg_spmi_00",       "imp_reg_spmi",       0xFFA8C000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_REG_SPMI,     OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_reg_spmc_00",       "imp_reg_spmc",       0xFFAB0000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_REG_SPMC,     OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_spmi_00",           "imp_spmi",           0xED300000U, 0x00100000U, NULL,                        0U, R_PMA_HWA_ID_V4H_SPMI,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_spmc_00",           "imp_spmc",           0xED400000U, 0x00200000U, NULL,                        0U, R_PMA_HWA_ID_V4H_SPMC,         OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},
    {NULL,           "imp_spmc_01",           "imp_spmc",           0xED600000U, 0x00080000U, NULL,                        0U, R_PMA_HWA_ID_V4H_SPMC01,       OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_imp_ch1_v4h},

    /* VDSP */
    {NULL,           "vdsp00_00",             "vdsp00",             0xF1400000U, 0x001A0000U, gs_int_vdsp_v4h[0],          5U, R_PMA_HWA_ID_V4H_VDSP0_CSB,    OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_dspp_dspd_v4h},
    {NULL,           "vdsp00_01",             "vdsp00",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP0_BUS,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp00_02",             "vdsp00",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP0_REG,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp00_03",             "vdsp00",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP0_GLOBAL, OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp00_04",             "vdsp00",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP0_SYS,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp00_05",             "vdsp00",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP0_OCEM,   OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp00_06",             "vdsp00",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP0_APBDBG, OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp00_07",             "vdsp00",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP0_SYSPO,  OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp01_00",             "vdsp01",             0xF1600000U, 0x001A0000U, gs_int_vdsp_v4h[1],          5U, R_PMA_HWA_ID_V4H_VDSP1_CSB,    OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_dspp_dspd_v4h},
    {NULL,           "vdsp01_01",             "vdsp01",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP1_BUS,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp01_02",             "vdsp01",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP1_REG,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp01_03",             "vdsp01",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP1_GLOBAL, OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp01_04",             "vdsp01",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP1_SYS,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp01_05",             "vdsp01",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP1_OCEM,   OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp01_06",             "vdsp01",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP1_APBDBG, OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp01_07",             "vdsp01",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP1_SYSPO,  OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp02_00",             "vdsp02",             0xF1800000U, 0x001A0000U, gs_int_vdsp_v4h[2],          5U, R_PMA_HWA_ID_V4H_VDSP2_CSB,    OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_dspp_dspd_v4h},
    {NULL,           "vdsp02_01",             "vdsp02",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP2_BUS,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp02_02",             "vdsp02",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP2_REG,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp02_03",             "vdsp02",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP2_GLOBAL, OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp02_04",             "vdsp02",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP2_SYS,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp02_05",             "vdsp02",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP2_OCEM,   OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp02_06",             "vdsp02",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP2_APBDBG, OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp02_07",             "vdsp02",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP2_SYSPO,  OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp03_00",             "vdsp03",             0xF1A00000U, 0x001A0000U, gs_int_vdsp_v4h[3],          5U, R_PMA_HWA_ID_V4H_VDSP3_CSB,    OSAL_PM_POSTCKCR_INVALID, "ir",          &gs_utlb_ir_dspp_dspd_v4h},
    {NULL,           "vdsp03_01",             "vdsp03",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP3_BUS,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp03_02",             "vdsp03",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP3_REG,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp03_03",             "vdsp03",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP3_GLOBAL, OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp03_04",             "vdsp03",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP3_SYS,    OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp03_05",             "vdsp03",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP3_OCEM,   OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp03_06",             "vdsp03",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP3_APBDBG, OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vdsp03_07",             "vdsp03",             0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_VDSP3_SYSPO,  OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* VISIONIP */
    {NULL,           "dof_00",                "dof",                0xE7A10000U, 0x00010000U, gs_int_dof_v4h[0],           2U, R_PMA_HWA_ID_V4H_DOF,          OSAL_PM_POST4CKCR,        "vip0",        &gs_utlb_vip0_dof_v4h},
    {NULL,           "smd_ps_00",             "smd_ps",             0xE7A20000U, 0x00010000U, gs_int_dof_v4h[1],           2U, R_PMA_HWA_ID_V4H_SMPS0,        OSAL_PM_POST4CKCR,        "vip0",        &gs_utlb_vip0_smps_v4h},
    {NULL,           "smd_post_00",           "smd_post",           0xE7A40000U, 0x00010000U, gs_int_dof_v4h[2],           2U, R_PMA_HWA_ID_V4H_SMPO0,        OSAL_PM_POST4CKCR,        "vip0",        &gs_utlb_vip0_smpo_v4h},
    {NULL,           "dof_fcprm_00",          "dof_fcprm",          0xE7AB1000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_DOF_FCPRM,    OSAL_PM_POST4CKCR,        "vip0",        &gs_utlb_vip0_dof_v4h},
    {NULL,           "vip_wrap_00",           "vip_wrap",           0xE7B3B000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST4CKCR,        "vip0",        &gs_utlb_vip0_wrap_v4h},

    /* GFX */
    {NULL,           "gsx_00",                "gsx",                0xFD000000U, 0x00800000U, gs_int_gsx_v4h,              1U, R_PMA_HWA_ID_V4H_RGX,          OSAL_PM_POSTCKCR_INVALID, "3dg",         &gs_utlb_3dg_3dge_v4h},

    /* ICCOM(for IO) */
    {NULL,           "iccom_reg_00",          "iccom_reg",          0xE6260000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* ICCOM(for Interrupt) */
    {NULL,           "iccom_00",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 0],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_01",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 1],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_02",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 2],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_03",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 3],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_04",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 4],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_05",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 5],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_06",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 6],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_07",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 7],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_08",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 8],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_09",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[ 9],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_10",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[10],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "iccom_11",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_v4h[11],        1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* ENCODER */
    {NULL,           "ivcp1e_00",             "ivcp1e",             0xFEA00000U, 0x00010000U, gs_int_ivcp1e_v4h,           2U, R_PMA_HWA_ID_V4H_IVCP1E,       OSAL_PM_POSTCKCR_INVALID, "vc",          &gs_utlb_vc_ivcp1e_v4h},
    {NULL,           "vcp4_00",               "vcp4",               0xFE910000U, 0x00010000U, gs_int_vcp4_v4h,             2U, R_PMA_HWA_ID_V4H_VCPL4,        OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "fcpc_00",               "fcpc",               0xFE90F000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_FCPCS,        OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* ISP */
    {NULL,           "cisp_00",               "cisp",               0xFEC00000U, 0x00100000U, gs_int_isp_cisp_v4h[0],      1U, R_PMA_HWA_ID_V4H_ISP0,         OSAL_PM_POSTCKCR_INVALID, "vi0",         &gs_utlb_vi0_isp0_ch0ch1ch2ch3ch4_v4h},
    {NULL,           "cisp_01",               "cisp",               0xFEE00000U, 0x00100000U, gs_int_isp_cisp_v4h[1],      1U, R_PMA_HWA_ID_V4H_ISP1,         OSAL_PM_POSTCKCR_INVALID, "vi0",         &gs_utlb_vi0_isp1_ch0ch1ch2ch3ch4_v4h},
    {NULL,           "tisp_00",               "tisp",               0xFED00000U, 0x00010000U, gs_int_isp_tisp_v4h[0],      1U, R_PMA_HWA_ID_V4H_ISPCS0,       OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "tisp_01",               "tisp",               0xFED20000U, 0x00010000U, gs_int_isp_tisp_v4h[1],      1U, R_PMA_HWA_ID_V4H_ISPCS1,       OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "vspx_00",               "vspx",               0xFEDD0000U, 0x00008000U, gs_int_isp_vspx_v4h[0],      1U, R_PMA_HWA_ID_V4H_VSPX0,        OSAL_PM_POSTCKCR_INVALID, "vi1",         &gs_utlb_vi1_fcpvx_ch0_v4h},
    {NULL,           "vspx_01",               "vspx",               0xFEDD8000U, 0x00008000U, gs_int_isp_vspx_v4h[1],      1U, R_PMA_HWA_ID_V4H_VSPX1,        OSAL_PM_POSTCKCR_INVALID, "vi1",         &gs_utlb_vi1_fcpvx_ch1_v4h},
    {NULL,           "fcpvx_00",              "fcpvx",              0xFEDB0000U, 0x00008000U, NULL,                        0U, R_PMA_HWA_ID_V4H_FCPVX0,       OSAL_PM_POSTCKCR_INVALID, "vi1",         &gs_utlb_vi1_fcpvx_ch0_v4h},
    {NULL,           "fcpvx_01",              "fcpvx",              0xFEDB8000U, 0x00008000U, NULL,                        0U, R_PMA_HWA_ID_V4H_FCPVX1,       OSAL_PM_POSTCKCR_INVALID, "vi1",         &gs_utlb_vi1_fcpvx_ch1_v4h},

    /* FieldBIST */
    {NULL,           "gicd_00",               "gicd",               0xF1000000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* FieldBIST */
    {NULL,           "fbc_00",                "fbc",                0xFF830000U, 0x00002000U, gs_int_fbc_v4h,              1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_ims0",              "fba",                0xFE605000U, 0x00001000U, gs_int_fba_v4h[0],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_ims1",              "fba",                0xFE606000U, 0x00001000U, gs_int_fba_v4h[1],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imr0",              "fba",                0xFE601000U, 0x00001000U, gs_int_fba_v4h[2],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imr1",              "fba",                0xFE602000U, 0x00001000U, gs_int_fba_v4h[3],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imr2",              "fba",                0xFE603000U, 0x00001000U, gs_int_fba_v4h[4],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp0",              "fba",                0xFF8C0000U, 0x00001000U, gs_int_fba_v4h[5],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp1",              "fba",                0xFF8C1000U, 0x00001000U, gs_int_fba_v4h[6],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp2",              "fba",                0xFF8C2000U, 0x00001000U, gs_int_fba_v4h[7],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp3",              "fba",                0xFF8C3000U, 0x00001000U, gs_int_fba_v4h[8],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv0",          "fba",                0xFF8C4000U, 0x00001000U, gs_int_fba_v4h[9],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv1",          "fba",                0xFF8C5000U, 0x00001000U, gs_int_fba_v4h[10],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv2",          "fba",                0xFF8C6000U, 0x00001000U, gs_int_fba_v4h[11],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_ocv3",          "fba",                0xFF8C7000U, 0x00001000U, gs_int_fba_v4h[12],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_dp0",           "fba",                0xFFF56000U, 0x00001000U, gs_int_fba_v4h[13],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_dp1",           "fba",                0xFFF57000U, 0x00001000U, gs_int_fba_v4h[14],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_cnn0",          "fba",                0xFFF50000U, 0x00001000U, gs_int_fba_v4h[15],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_dsp0",          "fba",                0xFFF64000U, 0x00001000U, gs_int_fba_v4h[16],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_dsp1",          "fba",                0xFFF65000U, 0x00001000U, gs_int_fba_v4h[17],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_dsp2",          "fba",                0xFFF66000U, 0x00001000U, gs_int_fba_v4h[18],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_dsp3",          "fba",                0xFFF67000U, 0x00001000U, gs_int_fba_v4h[19],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_cnram0",        "fba",                0xFFF55000U, 0x00001000U, gs_int_fba_v4h[20],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_slim0",         "fba",                0xFF8F4000U, 0x00001000U, gs_int_fba_v4h[21],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_imp_slim1",         "fba",                0xFF8F5000U, 0x00001000U, gs_int_fba_v4h[22],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_umfl0",             "fba",                0xE7B61000U, 0x00001000U, gs_int_fba_v4h[23],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST4CKCR,        "invalid",     NULL},
    {NULL,           "fba_smps0",             "fba",                0xE7B64000U, 0x00001000U, gs_int_fba_v4h[24],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST4CKCR,        "invalid",     NULL},
    {NULL,           "fba_smpo0",             "fba",                0xE7B63000U, 0x00001000U, gs_int_fba_v4h[25],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST4CKCR,        "invalid",     NULL},
    {NULL,           "fba_pap0",              "fba",                0xE7B62000U, 0x00001000U, gs_int_fba_v4h[26],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST4CKCR,        "invalid",     NULL},
    {NULL,           "fba_isp0",              "fba",                0xFEAC1000U, 0x00001000U, gs_int_fba_v4h[27],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_isp1",              "fba",                0xFEAC2000U, 0x00001000U, gs_int_fba_v4h[28],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR,         "invalid",     NULL},
    {NULL,           "fba_ca76d0",            "fba",                0xFF870000U, 0x00001000U, gs_int_fba_v4h[29],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca76d1",            "fba",                0xFF873000U, 0x00001000U, gs_int_fba_v4h[30],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca760",             "fba",                0xFF871000U, 0x00001000U, gs_int_fba_v4h[31],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca761",             "fba",                0xFF872000U, 0x00001000U, gs_int_fba_v4h[32],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca762",             "fba",                0xFF874000U, 0x00001000U, gs_int_fba_v4h[33],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",     NULL},
    {NULL,           "fba_ca763",             "fba",                0xFF875000U, 0x00001000U, gs_int_fba_v4h[34],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POST2CKCR,        "invalid",     NULL},

    /* RTT */
    {NULL,           "rfso_00",               "rfso",               0xFFE80000U, 0x00001000U, gs_int_rtt_v4h[0],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_01",               "rfso",               0xFFE81000U, 0x00001000U, gs_int_rtt_v4h[1],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_02",               "rfso",               0xFFE82000U, 0x00001000U, gs_int_rtt_v4h[2],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_03",               "rfso",               0xFFE83000U, 0x00001000U, gs_int_rtt_v4h[3],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_04",               "rfso",               0xFFE84000U, 0x00001000U, gs_int_rtt_v4h[4],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_05",               "rfso",               0xFFE85000U, 0x00001000U, gs_int_rtt_v4h[5],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_06",               "rfso",               0xFFE86000U, 0x00001000U, gs_int_rtt_v4h[6],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_07",               "rfso",               0xFFE87000U, 0x00001000U, gs_int_rtt_v4h[7],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_08",               "rfso",               0xFFE88000U, 0x00001000U, gs_int_rtt_v4h[8],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_09",               "rfso",               0xFFE89000U, 0x00001000U, gs_int_rtt_v4h[9],           1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "rfso_10",               "rfso",               0xFFE8A000U, 0x00001000U, gs_int_rtt_v4h[10],          1U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* IPMMU */
    {NULL,           "ipmmu_mm_00",           "ipmmu_mm",           0xEEFC0000U, 0x00020000U, gs_int_ipmmu_v4h,            2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vi0",       "ipmmu_cache",        0xEEE80000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vi1",       "ipmmu_cache",        0xEEEC0000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vc",        "ipmmu_cache",        0xEEDC0000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_ir",        "ipmmu_cache",        0xEED80000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_V4H_IPMMU_CACHE_IR,OSAL_PM_POSTCKCR_INVALID,"invalid",     NULL},
    {NULL,           "ipmmu_cache_rt0",       "ipmmu_cache",        0xEE480000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_rt1",       "ipmmu_cache",        0xEE4C0000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_ds0",       "ipmmu_cache",        0xEED00000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_hsc",       "ipmmu_cache",        0xEED40000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vip0",      "ipmmu_cache",        0xEEF00000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_vip1",      "ipmmu_cache",        0xEEF40000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},
    {NULL,           "ipmmu_cache_3dg",       "ipmmu_cache",        0xEEE00000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",     NULL},

    /* caiplite */
    {NULL,           "caiplite_wrapper_00",   "caiplite_wrapper",   0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_V4H_AES_ACC_WRAP, OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "caiplite_00",           "caiplite",           0xE7800000U, 0x00001000U, gs_int_caiplite_v4h[0],      1U, R_PMA_HWA_ID_V4H_AES_ACC0,     OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "caiplite_01",           "caiplite",           0xE7801000U, 0x00001000U, gs_int_caiplite_v4h[1],      1U, R_PMA_HWA_ID_V4H_AES_ACC1,     OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "caiplite_02",           "caiplite",           0xE7802000U, 0x00001000U, gs_int_caiplite_v4h[2],      1U, R_PMA_HWA_ID_V4H_AES_ACC2,     OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "caiplite_03",           "caiplite",           0xE7803000U, 0x00001000U, gs_int_caiplite_v4h[3],      1U, R_PMA_HWA_ID_V4H_AES_ACC3,     OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "caiplite_04",           "caiplite",           0xE7804000U, 0x00001000U, gs_int_caiplite_v4h[4],      1U, R_PMA_HWA_ID_V4H_AES_ACC4,     OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "caiplite_05",           "caiplite",           0xE7805000U, 0x00001000U, gs_int_caiplite_v4h[5],      1U, R_PMA_HWA_ID_V4H_AES_ACC5,     OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "caiplite_06",           "caiplite",           0xE7806000U, 0x00001000U, gs_int_caiplite_v4h[6],      1U, R_PMA_HWA_ID_V4H_AES_ACC6,     OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "caiplite_07",           "caiplite",           0xE7807000U, 0x00001000U, gs_int_caiplite_v4h[7],      1U, R_PMA_HWA_ID_V4H_AES_ACC7,     OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* caiplite */
    {NULL,           "wcrc_caiplite_00",      "wcrc_caiplite",      0xE7080000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_WCRC0,        OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wcrc_caiplite_01",      "wcrc_caiplite",      0xE7090000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_WCRC1,        OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wcrc_caiplite_02",      "wcrc_caiplite",      0xE70A0000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_WCRC2,        OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wcrc_caiplite_03",      "wcrc_caiplite",      0xE70B0000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_WCRC3,        OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* rtdmac_control0 */
    {NULL,           "rtdmac_control0_00",    "rtdmac_control0",    0xFFC10000U, 0x00001000U, gs_int_rtdmac_control0_v4h[0],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_00_v4h},
    {NULL,           "rtdmac_control0_01",    "rtdmac_control0",    0xFFC11000U, 0x00001000U, gs_int_rtdmac_control0_v4h[1],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_01_v4h},
    {NULL,           "rtdmac_control0_02",    "rtdmac_control0",    0xFFC12000U, 0x00001000U, gs_int_rtdmac_control0_v4h[2],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_02_v4h},
    {NULL,           "rtdmac_control0_03",    "rtdmac_control0",    0xFFC13000U, 0x00001000U, gs_int_rtdmac_control0_v4h[3],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_03_v4h},
    {NULL,           "rtdmac_control0_04",    "rtdmac_control0",    0xFFC14000U, 0x00001000U, gs_int_rtdmac_control0_v4h[4],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_04_v4h},
    {NULL,           "rtdmac_control0_05",    "rtdmac_control0",    0xFFC15000U, 0x00001000U, gs_int_rtdmac_control0_v4h[5],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_05_v4h},
    {NULL,           "rtdmac_control0_06",    "rtdmac_control0",    0xFFC16000U, 0x00001000U, gs_int_rtdmac_control0_v4h[6],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_06_v4h},
    {NULL,           "rtdmac_control0_07",    "rtdmac_control0",    0xFFC17000U, 0x00001000U, gs_int_rtdmac_control0_v4h[7],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_07_v4h},
    {NULL,           "rtdmac_control0_08",    "rtdmac_control0",    0xFFC18000U, 0x00001000U, gs_int_rtdmac_control0_v4h[8],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_08_v4h},
    {NULL,           "rtdmac_control0_09",    "rtdmac_control0",    0xFFC19000U, 0x00001000U, gs_int_rtdmac_control0_v4h[9],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_09_v4h},
    {NULL,           "rtdmac_control0_10",    "rtdmac_control0",    0xFFC1A000U, 0x00001000U, gs_int_rtdmac_control0_v4h[10], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_10_v4h},
    {NULL,           "rtdmac_control0_11",    "rtdmac_control0",    0xFFC1B000U, 0x00001000U, gs_int_rtdmac_control0_v4h[11], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_11_v4h},
    {NULL,           "rtdmac_control0_12",    "rtdmac_control0",    0xFFC1C000U, 0x00001000U, gs_int_rtdmac_control0_v4h[12], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_12_v4h},
    {NULL,           "rtdmac_control0_13",    "rtdmac_control0",    0xFFC1D000U, 0x00001000U, gs_int_rtdmac_control0_v4h[13], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_13_v4h},
    {NULL,           "rtdmac_control0_14",    "rtdmac_control0",    0xFFC1E000U, 0x00001000U, gs_int_rtdmac_control0_v4h[14], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_14_v4h},
    {NULL,           "rtdmac_control0_15",    "rtdmac_control0",    0xFFC1F000U, 0x00001000U, gs_int_rtdmac_control0_v4h[15], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac0_15_v4h},

    /* rtdmac_control1 */
    {NULL,           "rtdmac_control1_00",    "rtdmac_control1",    0xFFC20000U, 0x00001000U, gs_int_rtdmac_control1_v4h[0],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_00_v4h},
    {NULL,           "rtdmac_control1_01",    "rtdmac_control1",    0xFFC21000U, 0x00001000U, gs_int_rtdmac_control1_v4h[1],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_01_v4h},
    {NULL,           "rtdmac_control1_02",    "rtdmac_control1",    0xFFC22000U, 0x00001000U, gs_int_rtdmac_control1_v4h[2],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_02_v4h},
    {NULL,           "rtdmac_control1_03",    "rtdmac_control1",    0xFFC23000U, 0x00001000U, gs_int_rtdmac_control1_v4h[3],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_03_v4h},
    {NULL,           "rtdmac_control1_04",    "rtdmac_control1",    0xFFC24000U, 0x00001000U, gs_int_rtdmac_control1_v4h[4],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_04_v4h},
    {NULL,           "rtdmac_control1_05",    "rtdmac_control1",    0xFFC25000U, 0x00001000U, gs_int_rtdmac_control1_v4h[5],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_05_v4h},
    {NULL,           "rtdmac_control1_06",    "rtdmac_control1",    0xFFC26000U, 0x00001000U, gs_int_rtdmac_control1_v4h[6],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_06_v4h},
    {NULL,           "rtdmac_control1_07",    "rtdmac_control1",    0xFFC27000U, 0x00001000U, gs_int_rtdmac_control1_v4h[7],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_07_v4h},
    {NULL,           "rtdmac_control1_08",    "rtdmac_control1",    0xFFC28000U, 0x00001000U, gs_int_rtdmac_control1_v4h[8],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_08_v4h},
    {NULL,           "rtdmac_control1_09",    "rtdmac_control1",    0xFFC29000U, 0x00001000U, gs_int_rtdmac_control1_v4h[9],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_09_v4h},
    {NULL,           "rtdmac_control1_10",    "rtdmac_control1",    0xFFC2A000U, 0x00001000U, gs_int_rtdmac_control1_v4h[10], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_10_v4h},
    {NULL,           "rtdmac_control1_11",    "rtdmac_control1",    0xFFC2B000U, 0x00001000U, gs_int_rtdmac_control1_v4h[11], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_11_v4h},
    {NULL,           "rtdmac_control1_12",    "rtdmac_control1",    0xFFC2C000U, 0x00001000U, gs_int_rtdmac_control1_v4h[12], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_12_v4h},
    {NULL,           "rtdmac_control1_13",    "rtdmac_control1",    0xFFC2D000U, 0x00001000U, gs_int_rtdmac_control1_v4h[13], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_13_v4h},
    {NULL,           "rtdmac_control1_14",    "rtdmac_control1",    0xFFC2E000U, 0x00001000U, gs_int_rtdmac_control1_v4h[14], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_14_v4h},
    {NULL,           "rtdmac_control1_15",    "rtdmac_control1",    0xFFC2F000U, 0x00001000U, gs_int_rtdmac_control1_v4h[15], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac1_15_v4h},

    /* rtdmac_control2 */
    {NULL,           "rtdmac_control2_00",    "rtdmac_control2",    0xFFD70000U, 0x00001000U, gs_int_rtdmac_control2_v4h[0],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_00_v4h},
    {NULL,           "rtdmac_control2_01",    "rtdmac_control2",    0xFFD71000U, 0x00001000U, gs_int_rtdmac_control2_v4h[1],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_01_v4h},
    {NULL,           "rtdmac_control2_02",    "rtdmac_control2",    0xFFD72000U, 0x00001000U, gs_int_rtdmac_control2_v4h[2],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_02_v4h},
    {NULL,           "rtdmac_control2_03",    "rtdmac_control2",    0xFFD73000U, 0x00001000U, gs_int_rtdmac_control2_v4h[3],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_03_v4h},
    {NULL,           "rtdmac_control2_04",    "rtdmac_control2",    0xFFD74000U, 0x00001000U, gs_int_rtdmac_control2_v4h[4],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_04_v4h},
    {NULL,           "rtdmac_control2_05",    "rtdmac_control2",    0xFFD75000U, 0x00001000U, gs_int_rtdmac_control2_v4h[5],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_05_v4h},
    {NULL,           "rtdmac_control2_06",    "rtdmac_control2",    0xFFD76000U, 0x00001000U, gs_int_rtdmac_control2_v4h[6],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_06_v4h},
    {NULL,           "rtdmac_control2_07",    "rtdmac_control2",    0xFFD77000U, 0x00001000U, gs_int_rtdmac_control2_v4h[7],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_07_v4h},
    {NULL,           "rtdmac_control2_08",    "rtdmac_control2",    0xFFD78000U, 0x00001000U, gs_int_rtdmac_control2_v4h[8],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_08_v4h},
    {NULL,           "rtdmac_control2_09",    "rtdmac_control2",    0xFFD79000U, 0x00001000U, gs_int_rtdmac_control2_v4h[9],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_09_v4h},
    {NULL,           "rtdmac_control2_10",    "rtdmac_control2",    0xFFD7A000U, 0x00001000U, gs_int_rtdmac_control2_v4h[10], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_10_v4h},
    {NULL,           "rtdmac_control2_11",    "rtdmac_control2",    0xFFD7B000U, 0x00001000U, gs_int_rtdmac_control2_v4h[11], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_11_v4h},
    {NULL,           "rtdmac_control2_12",    "rtdmac_control2",    0xFFD7C000U, 0x00001000U, gs_int_rtdmac_control2_v4h[12], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_12_v4h},
    {NULL,           "rtdmac_control2_13",    "rtdmac_control2",    0xFFD7D000U, 0x00001000U, gs_int_rtdmac_control2_v4h[13], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_13_v4h},
    {NULL,           "rtdmac_control2_14",    "rtdmac_control2",    0xFFD7E000U, 0x00001000U, gs_int_rtdmac_control2_v4h[14], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_14_v4h},
    {NULL,           "rtdmac_control2_15",    "rtdmac_control2",    0xFFD7F000U, 0x00001000U, gs_int_rtdmac_control2_v4h[15], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac2_15_v4h},

    /* rtdmac_control3 */
    {NULL,           "rtdmac_control3_00",    "rtdmac_control3",    0xFFD80000U, 0x00001000U, gs_int_rtdmac_control3_v4h[0],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_00_v4h},
    {NULL,           "rtdmac_control3_01",    "rtdmac_control3",    0xFFD81000U, 0x00001000U, gs_int_rtdmac_control3_v4h[1],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_01_v4h},
    {NULL,           "rtdmac_control3_02",    "rtdmac_control3",    0xFFD82000U, 0x00001000U, gs_int_rtdmac_control3_v4h[2],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_02_v4h},
    {NULL,           "rtdmac_control3_03",    "rtdmac_control3",    0xFFD83000U, 0x00001000U, gs_int_rtdmac_control3_v4h[3],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_03_v4h},
    {NULL,           "rtdmac_control3_04",    "rtdmac_control3",    0xFFD84000U, 0x00001000U, gs_int_rtdmac_control3_v4h[4],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_04_v4h},
    {NULL,           "rtdmac_control3_05",    "rtdmac_control3",    0xFFD85000U, 0x00001000U, gs_int_rtdmac_control3_v4h[5],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_05_v4h},
    {NULL,           "rtdmac_control3_06",    "rtdmac_control3",    0xFFD86000U, 0x00001000U, gs_int_rtdmac_control3_v4h[6],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_06_v4h},
    {NULL,           "rtdmac_control3_07",    "rtdmac_control3",    0xFFD87000U, 0x00001000U, gs_int_rtdmac_control3_v4h[7],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_07_v4h},
    {NULL,           "rtdmac_control3_08",    "rtdmac_control3",    0xFFD88000U, 0x00001000U, gs_int_rtdmac_control3_v4h[8],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_08_v4h},
    {NULL,           "rtdmac_control3_09",    "rtdmac_control3",    0xFFD89000U, 0x00001000U, gs_int_rtdmac_control3_v4h[9],  1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_09_v4h},
    {NULL,           "rtdmac_control3_10",    "rtdmac_control3",    0xFFD8A000U, 0x00001000U, gs_int_rtdmac_control3_v4h[10], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_10_v4h},
    {NULL,           "rtdmac_control3_11",    "rtdmac_control3",    0xFFD8B000U, 0x00001000U, gs_int_rtdmac_control3_v4h[11], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_11_v4h},
    {NULL,           "rtdmac_control3_12",    "rtdmac_control3",    0xFFD8C000U, 0x00001000U, gs_int_rtdmac_control3_v4h[12], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_12_v4h},
    {NULL,           "rtdmac_control3_13",    "rtdmac_control3",    0xFFD8D000U, 0x00001000U, gs_int_rtdmac_control3_v4h[13], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_13_v4h},
    {NULL,           "rtdmac_control3_14",    "rtdmac_control3",    0xFFD8E000U, 0x00001000U, gs_int_rtdmac_control3_v4h[14], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_14_v4h},
    {NULL,           "rtdmac_control3_15",    "rtdmac_control3",    0xFFD8F000U, 0x00001000U, gs_int_rtdmac_control3_v4h[15], 1U, R_PMA_HWA_ID_NONE,          OSAL_PM_POSTCKCR_INVALID, "rt1",        &gs_utlb_rt1_rtdmac3_15_v4h},

    /* rtdmac_control */
    {NULL,           "rtdmac_descriptor0_00", "rtdmac_descriptor0", 0xFFD60000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_RTDM0,        OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "rtdmac_descriptor1_00", "rtdmac_descriptor1", 0xFFD61000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_RTDM1,        OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "rtdmac_descriptor2_00", "rtdmac_descriptor2", 0xFFD62000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_RTDM2,        OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "rtdmac_descriptor3_00", "rtdmac_descriptor3", 0xFFD63000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_RTDM3,        OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* vin */
    {NULL,           "vin_00",                "vin",                0xE6EF0000U, 0x00001000U, gs_int_vin_v4h[0],           1U, R_PMA_HWA_ID_V4H_VIN00,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {NULL,           "vin_01",                "vin",                0xE6EF1000U, 0x00001000U, gs_int_vin_v4h[1],           1U, R_PMA_HWA_ID_V4H_VIN01,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {NULL,           "vin_02",                "vin",                0xE6EF2000U, 0x00001000U, gs_int_vin_v4h[2],           1U, R_PMA_HWA_ID_V4H_VIN02,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {NULL,           "vin_03",                "vin",                0xE6EF3000U, 0x00001000U, gs_int_vin_v4h[3],           1U, R_PMA_HWA_ID_V4H_VIN03,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {NULL,           "vin_04",                "vin",                0xE6EF4000U, 0x00001000U, gs_int_vin_v4h[4],           1U, R_PMA_HWA_ID_V4H_VIN04,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {NULL,           "vin_05",                "vin",                0xE6EF5000U, 0x00001000U, gs_int_vin_v4h[5],           1U, R_PMA_HWA_ID_V4H_VIN05,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {NULL,           "vin_06",                "vin",                0xE6EF6000U, 0x00001000U, gs_int_vin_v4h[6],           1U, R_PMA_HWA_ID_V4H_VIN06,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {NULL,           "vin_07",                "vin",                0xE6EF7000U, 0x00001000U, gs_int_vin_v4h[7],           1U, R_PMA_HWA_ID_V4H_VIN07,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {NULL,           "vin_10",                "vin",                0xE6EF8000U, 0x00001000U, gs_int_vin_v4h[8],           1U, R_PMA_HWA_ID_V4H_VIN10,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {NULL,           "vin_11",                "vin",                0xE6EF9000U, 0x00001000U, gs_int_vin_v4h[9],           1U, R_PMA_HWA_ID_V4H_VIN11,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {NULL,           "vin_12",                "vin",                0xE6EFA000U, 0x00001000U, gs_int_vin_v4h[10],          1U, R_PMA_HWA_ID_V4H_VIN12,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {NULL,           "vin_13",                "vin",                0xE6EFB000U, 0x00001000U, gs_int_vin_v4h[11],          1U, R_PMA_HWA_ID_V4H_VIN13,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {NULL,           "vin_14",                "vin",                0xE6EFC000U, 0x00001000U, gs_int_vin_v4h[12],          1U, R_PMA_HWA_ID_V4H_VIN14,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {NULL,           "vin_15",                "vin",                0xE6EFD000U, 0x00001000U, gs_int_vin_v4h[13],          1U, R_PMA_HWA_ID_V4H_VIN15,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {NULL,           "vin_16",                "vin",                0xE6EFE000U, 0x00001000U, gs_int_vin_v4h[14],          1U, R_PMA_HWA_ID_V4H_VIN16,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {NULL,           "vin_17",                "vin",                0xE6EFF000U, 0x00001000U, gs_int_vin_v4h[15],          1U, R_PMA_HWA_ID_V4H_VIN17,        OSAL_PM_POSTCKCR_INVALID, "vi0",        &gs_utlb_vi0_vin_ch1_v4h},

    /* vsp */
    {NULL,           "vsp_00",                "vsp",                0xFEA20000U, 0x00008000U, gs_int_vsp_v4h[0],           1U, R_PMA_HWA_ID_V4H_VSPD0,        OSAL_PM_POSTCKCR_INVALID, "vi1",        &gs_utlb_vi1_fcpvd_ch0_v4h},
    {NULL,           "vsp_01",                "vsp",                0xFEA28000U, 0x00008000U, gs_int_vsp_v4h[1],           1U, R_PMA_HWA_ID_V4H_VSPD1,        OSAL_PM_POSTCKCR_INVALID, "vi1",        &gs_utlb_vi1_fcpvd_ch1_v4h},

    /* csi */
    {NULL,           "csi_00",                "csi",                0xFE500000U, 0x00040000U, gs_int_csi_v4h[0],           1U, R_PMA_HWA_ID_V4H_CSITOP0,      OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "csi_01",                "csi",                0xFE540000U, 0x00040000U, gs_int_csi_v4h[1],           1U, R_PMA_HWA_ID_V4H_CSITOP1,      OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* du */
    {NULL,           "du_00",                 "du",                 0xFEB00000U, 0x00040000U, gs_int_du_v4h,               2U, R_PMA_HWA_ID_V4H_DIS0,         OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* dsi */
    {NULL,           "dsi_00",                "dsi",                0xFED80000U, 0x00010000U, gs_int_dsi_v4h[0],           9U, R_PMA_HWA_ID_V4H_DSITXLINK0,   OSAL_PM_POSTCKCR_INVALID, "vi1",        &gs_utlb_vi1_dsi_ch0_v4h},
    {NULL,           "dsi_01",                "dsi",                0xFED90000U, 0x00010000U, gs_int_dsi_v4h[1],           9U, R_PMA_HWA_ID_V4H_DSITXLINK1,   OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* wwdt */
    {NULL,           "wwdt_00",               "wwdt",               0xFFC90000U, 0x00001000U, gs_int_wwdt_v4h[0],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_01",               "wwdt",               0xFFCA0000U, 0x00001000U, gs_int_wwdt_v4h[1],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_02",               "wwdt",               0xFFCB0000U, 0x00001000U, gs_int_wwdt_v4h[2],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_03",               "wwdt",               0xFFCC0000U, 0x00001000U, gs_int_wwdt_v4h[3],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_04",               "wwdt",               0xFFCF0000U, 0x00001000U, gs_int_wwdt_v4h[4],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_05",               "wwdt",               0xFFEF0000U, 0x00001000U, gs_int_wwdt_v4h[5],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_06",               "wwdt",               0xFFF10000U, 0x00001000U, gs_int_wwdt_v4h[6],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_07",               "wwdt",               0xFFF20000U, 0x00001000U, gs_int_wwdt_v4h[7],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_08",               "wwdt",               0xFFF30000U, 0x00001000U, gs_int_wwdt_v4h[8],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},
    {NULL,           "wwdt_09",               "wwdt",               0xFFF40000U, 0x00001000U, gs_int_wwdt_v4h[9],          2U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* doc */
    {NULL,           "doc_00",                "doc",                0xFEBA0000U, 0x00011000U, gs_int_doc_v4h,              2U, R_PMA_HWA_ID_V4H_DOC2CH,       OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* dsc */
    {NULL,           "dsc_00",                "dsc",                0xFEB8D000U, 0x00001000U, gs_int_dsc_v4h,              1U, R_PMA_HWA_ID_V4H_DSC,          OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* pap */
    {NULL,           "hsm_00",                "hsm",                0xFF000000U, 0x00080000U, NULL,                        0U, R_PMA_HWA_ID_V4H_HSM,          OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "dcm_00",                "dcm",                0xFF080000U, 0x00064000U, NULL,                        0U, R_PMA_HWA_ID_V4H_DCM,          OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "cbf_00",                "cbf",                0xFF0E8000U, 0x00003000U, NULL,                        0U, R_PMA_HWA_ID_V4H_CBF,          OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "sbf_00",                "sbf",                0xFF0EC000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_SBF,          OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "osm_00",                "osm",                0xFF0F0000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_V4H_OSM,          OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "ism_00",                "ism",                0xFF100000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_V4H_ISM0,         OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "ism_01",                "ism",                0xFF120000U, 0x00020000U, NULL,                        0U, R_PMA_HWA_ID_V4H_ISM1,         OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "sdmac_00",              "sdmac",              0xFF180000U, 0x00010000U, gs_int_sdmac_v4h,            2U, R_PMA_HWA_ID_V4H_PAPSDMA,      OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "fbs_00",                "fbs",                0xFF200000U, 0x00080000U, gs_int_fbs_v4h,              1U, R_PMA_HWA_ID_V4H_FBS,          OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "apb_00",                "apb",                0xFF290000U, 0x00004000U, NULL,                        0U, R_PMA_HWA_ID_V4H_APB,          OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "dul_00",                "dul",                0xFF298000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_V4H_DUL,          OSAL_PM_POSTCKCR_INVALID, "vip1",       &gs_utlb_vip1_pap_v4h},
    {NULL,           "ecm_00",                "ecm",                0xE6250000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* SYSTEM RAM */
    {NULL,           "system_ram_00",         "system_ram",         0xE6300000U, 0x00100000U, NULL,                        0U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "invalid",    NULL},

    /* ICUMX */
    {NULL,           "icumx_cmd_00",          "icumx_cmd",          0xE6610000U, 0x00008000U, gs_int_icumx_v4h,           20U, R_PMA_HWA_ID_NONE,             OSAL_PM_POSTCKCR_INVALID, "rt0",        &gs_utlb_rt0_icumx_v4h},

    {.id = OSAL_DEVICE_INVALID_ID}
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_031]
 * [Covers: XOS3_OSAL_QNX_UD_DD_NON_002]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

#endif /* OSAL_OS_DEV_CFG_V4H_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
