/***********************************************************************************************************************
* Copyright [2021-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    r_osal_dev_cfg_v4h.h
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car V4H
* Description :  OSAL device configuration file for R-Car V4H
***********************************************************************************************************************/
#ifndef OSAL_DEV_CFG_V4H_H
#define OSAL_DEV_CFG_V4H_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"
#include "target/emcos/dev_cfg/r_osal_dev_cfg_control.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Private_Defines_Static_Variables_DevConfig Device Condifurator Private static variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_int_ims
 * osal device configuration : IMS interrupt
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
 * osal device configuration : IMR interrupt
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
 * osal device configuration : IMP interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_imp_top_v4h[3] =
{
    {565}, {566}, {567}
};

/*******************************************************************************************************************//**
 * @var gs_int_vdsp_v4h
 * osal device configuration : vdsp interrupt
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
 * osal device configuration : DOF interrupt
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
 * @var gs_int_iccom_v4h
 * osal device configuration : ICCOM interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_iccom_v4h[12] =
{
    {123}, {125}, {127}, {129}, {131}, {133},
    {135}, {137}, {916}, {918}, {920}, {922}
};

/*******************************************************************************************************************//**
 * @var gs_int_ivcp1e_v4h
 * osal device configuration : IVCP1E interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ivcp1e_v4h[2] =
{
    {855}, {856}
};

/*******************************************************************************************************************//**
 * @var gs_int_vcp4
 * osal device configuration : VCP4 interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_vcp4_v4h[2] =
{
    {858}, {859}
};

/*******************************************************************************************************************//**
 * @var gs_int_isp_cisp_v4h
 * osal device configuration : ISP interrupt
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
 * osal device configuration : ISP interrupt
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
 * osal device configuration : ISP interrupt
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
 * osal device configuration : FBC interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbc_v4h[1] =
{
    {852}
};

/*******************************************************************************************************************//**
 * @var gs_int_fba_v4h
 * osal device configuration : FBA interrupt
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
 * osal device configuration : RTT interrupt
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
 * osal device configuration : IPMMU interrupt
***********************************************************************************************************************/
#if 0
    static st_osal_interrupt_info_t gs_int_ipmmu_v4h[1] =
{
    {21}
};
#endif
/*******************************************************************************************************************//**
 * @var gs_int_caiplite_v4h
 * osal device configuration : caiplite interrupt
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
 * @var gs_int_vin_v4h
 * osal device configuration : vin interrupt
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
 * osal device configuration : vsp interrupt
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
 * osal device configuration : csi interrupt
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
 * osal device configuration : du interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_du_v4h[2] =
{
    {523}, {524}
};

/*******************************************************************************************************************//**
 * @var gs_int_dsi_v4h
 * osal device configuration : dsi interrupt
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
 * osal device configuration : wwdt interrupt
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
 * osal device configuration : doc interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_doc_v4h[2] =
{
    {526}, {527}
};

/*******************************************************************************************************************//**
 * @var gs_int_dsc_v4h
 * osal device configuration : dsc interrupt
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
 * osal device configuration : fbs interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbs_v4h[1] =
{
    {223}
};

/*******************************************************************************************************************//**
 * @var gs_int_fbs_v4h
 * osal device configuration : fbs interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_icumx_v4h[20] =
{
    {878},{879},{880},{881},{882},{883},{884},{885},{886},{887},
    {888},{889},{890},{891},{892},{893},{894},{895},{896},{897}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi0_vin_ch0_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi0_vin_ch0_v4h =
{
    1, { 0,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi0_vin_ch1_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi0_vin_ch1_v4h =
{
    1, { 1,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip0_smpo_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vip0_smpo_v4h =
{
    1, { 1,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip0_smps_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vip0_smps_v4h =
{
    1, { 2,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip0_dof_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vip0_dof_v4h =
{
    1, { 3,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip0_dof_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vip0_wrap_v4h =
{
    3, { 1,                 2,                 3,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_fcpvd_ch0_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi1_fcpvd_ch0_v4h =
{
    1, { 6,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_fcpvd_ch1_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi1_fcpvd_ch1_v4h =
{
    1, { 7,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr0_ch0_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr0_ch0_v4h =
{
    1, { 8,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr1_ch0_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr1_ch0_v4h =
{
    1, { 9,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_ivcp1e_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vc_ivcp1e_v4h =
{
    1, { 14,                OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vip1_pap_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vip1_pap_v4h =
{
    1, { 16,                OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_dsi_ch0_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi1_dsi_ch0_v4h =
{
    1, { 18,                OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_fcpvx_ch0_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi1_fcpvx_ch0_v4h =
{
    1, { 24,                OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi1_fcpvx_ch1_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi1_fcpvx_ch1_v4h =
{
    1, { 25,                OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr2_ch0ch1_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr2_ch0ch1_v4h =
{
    2, { 0,                 4,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr3_ch0ch1_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr3_ch0ch1_v4h =
{
    2, { 1,                 5,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vc_imr4_ch0ch1_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vc_imr4_ch0ch1_v4h =
{
    2, { 2,                 6,                 OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi0_isp0_ch0ch1ch2ch3ch4_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi0_isp0_ch0ch1ch2ch3ch4_v4h =
{
    5, { 10,                14,                18,                22,                26,                OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_vi0_isp1_ch0ch1ch2ch3ch4_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_vi0_isp1_ch0ch1ch2ch3ch4_v4h =
{
    5, { 11,                15,                19,                23,                27,                OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_ir_dspp_dspd_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_ir_dspp_dspd_v4h =
{
    2, { 13,                14,                OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_ir_imp_ch0ch1_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_ir_imp_ch0ch1_v4h =
{
    16, { 16,                17,                18,                19,                20,                21,                22,                23,
          24,                25,                26,                27,                28,                29,                30,                31,
          OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
          OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
          OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
          OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
          OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
          OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_ir_imp_ch0_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_ir_imp_ch0_v4h =
{
    8, { 16,                17,                18,                19,                20,                21,                22,                23,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_utlb_ir_imp_ch1_v4h
 * osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_ir_imp_ch1_v4h =
{
    8, { 24,                25,                26,                27,                28,                29,                30,                31,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB,
         OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB, OSAL_INVALID_UTLB }
};

/*******************************************************************************************************************//**
 * @var gs_pdev_group_name_v4h
 * osal device configuration : pdev entry group name
***********************************************************************************************************************/
static const char * gs_pdev_group_name_v4h[] =
{
    "pdev_imr",
    "pdev_imp_core",
    "pdev_imp_spm",
    "pdev_vdsp",
    "pdev_visionip_core",
    "pdev_visionip_wrap",
    "pdev_iccom",
    "pdev_ivcp1e",
    "pdev_vcp4",
    "pdev_isp1",
    "pdev_isp2",
    "pdev_fbc",
    "pdev_fba01",
    "pdev_fba02",
    "pdev_fba03",
    "pdev_fba04",
    "pdev_fba05",
    "pdev_fba06",
    "pdev_fba07",
    "pdev_fba08",
    "pdev_fba09",
    "pdev_rfso",
    "pdev_caiplite",
    "pdev_wcrc",
    "pdev_vin",
    "pdev_vsp",
    "pdev_csi",
    "pdev_du",
    "pdev_dsi",
    "pdev_wwdt1",
    "pdev_wwdt2",
    "pdev_wwdt3",
    "pdev_wwdt4",
    "pdev_doc",
    "pdev_dsc",
    "pdev_pap",
    "pdev_sysram",
    "pdev_icumx",
};

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Private_Defines_Extern_Variables_DevConfig Device Condifurator Private extern variables
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_device_info_v4h
 * osal device configuration : device information structure
 * XOS3_OSAL_EMCOS_CD_CD_IO_040
 * [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_066]
 * [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_023]
***********************************************************************************************************************/
/* PRQA S 3218 1 # Definition within a function reduces readability and maintainability, so no change. */
static const st_osal_device_info_t gs_osal_device_info_v4h[] =
{
    /* pdev group,         device unique ID,   device type,    physical address,    range,     interrupt,    number of interrupts, power manager id,       initial policy,         POSTCKCR,                  AXI bus name,    assignment */
    /* IMR */
    {"pdev_imr",           "ims_00",           "ims",                0xFE860000U, 0x00010000U, gs_int_ims_v4h[0],           1U,    R_PMA_HWA_ID_V4H_000,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vc",         &gs_utlb_vc_imr0_ch0_v4h},
    {"pdev_imr",           "ims_01",           "ims",                0xFE870000U, 0x00010000U, gs_int_ims_v4h[1],           1U,    R_PMA_HWA_ID_V4H_001,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vc",         &gs_utlb_vc_imr1_ch0_v4h},
    {"pdev_imr",           "imr_00",           "imr",                0xFE880000U, 0x00010000U, gs_int_imr_v4h[0],           1U,    R_PMA_HWA_ID_V4H_002,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vc",         &gs_utlb_vc_imr2_ch0ch1_v4h},
    {"pdev_imr",           "imr_01",           "imr",                0xFE890000U, 0x00010000U, gs_int_imr_v4h[1],           1U,    R_PMA_HWA_ID_V4H_003,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vc",         &gs_utlb_vc_imr3_ch0ch1_v4h},
    {"pdev_imr",           "imr_02",           "imr",                0xFE8A0000U, 0x00010000U, gs_int_imr_v4h[2],           1U,    R_PMA_HWA_ID_V4H_004,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vc",         &gs_utlb_vc_imr4_ch0ch1_v4h},

    /* IMP */
    {"pdev_imp_core",      "imp_top_00",       "imp_top",            0xFF900000U, 0x00006000U, gs_int_imp_top_v4h,          3U,    R_PMA_HWA_ID_V4H_005,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0ch1_v4h},
    {"pdev_imp_core",      "imp_00",           "imp",                0xFFA00000U, 0x00020000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_006,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_01",           "imp",                0xFFA20000U, 0x00020000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_007,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_core",      "imp_02",           "imp",                0xFFB00000U, 0x00020000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_008,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_03",           "imp",                0xFFB20000U, 0x00020000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_009,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_ocv_00",       "imp_ocv",            0xFFA40000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_010,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_ocv_01",       "imp_ocv",            0xFFA50000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_011,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_core",      "imp_ocv_02",       "imp_ocv",            0xFFA60000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_012,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_ocv_03",       "imp_ocv",            0xFFA70000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_013,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_dma_00",       "imp_dma",            0xFFA80000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_014,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_dma_01",       "imp_dma",            0xFFB80000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_015,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_core",      "imp_psc_00",       "imp_psc",            0xFFA84000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_016,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_dta_00",       "imp_dta",            0xFFA88000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_017,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch0_v4h},
    {"pdev_imp_core",      "imp_dma_slim_00",  "imp_dma_slim",       0xFFA90000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_018,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_core",      "imp_dma_slim_01",  "imp_dma_slim",       0xFFA94000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_019,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_core",      "imp_cnn_00",       "imp_cnn",            0xFFAA0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_020,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_core",      "imp_reg_spmi_00",  "imp_reg_spmi",       0xFFA8C000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_021,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_core",      "imp_reg_spmc_00",  "imp_reg_spmc",       0xFFAB0000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_022,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_spm",       "imp_spmi_00",      "imp_spmi",           0xED300000U, 0x00100000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_023,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_imp_spm",       "imp_spmc_00",      "imp_spmc",           0xED400000U, 0x00200000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_024,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_imp_ch1_v4h},
    {"pdev_vdsp",          "vdsp00_00",        "vdsp00",             0xF1400000U, 0x001A0000U, gs_int_vdsp_v4h[0],          5U,    R_PMA_HWA_ID_V4H_025,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp00_01",        "vdsp00",             0xF15A0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_026,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp00_02",        "vdsp00",             0xF15B0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_027,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp00_03",        "vdsp00",             0xF15C0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_028,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp00_04",        "vdsp00",             0x00000000U, 0x00000000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_107,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp01_00",        "vdsp01",             0xF1600000U, 0x001A0000U, gs_int_vdsp_v4h[1],          5U,    R_PMA_HWA_ID_V4H_029,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp01_01",        "vdsp01",             0xF17A0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_030,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp01_02",        "vdsp01",             0xF17B0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_031,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp01_03",        "vdsp01",             0xF17C0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_032,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp01_04",        "vdsp01",             0x00000000U, 0x00000000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_108,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp02_00",        "vdsp02",             0xF1800000U, 0x001A0000U, gs_int_vdsp_v4h[2],          5U,    R_PMA_HWA_ID_V4H_033,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp02_01",        "vdsp02",             0xF19A0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_034,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp02_02",        "vdsp02",             0xF19B0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_035,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp02_03",        "vdsp02",             0xF19C0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_036,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp02_04",        "vdsp02",             0x00000000U, 0x00000000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_109,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp03_00",        "vdsp03",             0xF1A00000U, 0x001A0000U, gs_int_vdsp_v4h[3],          5U,    R_PMA_HWA_ID_V4H_037,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp03_01",        "vdsp03",             0xF1BA0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_038,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp03_02",        "vdsp03",             0xF1BB0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_039,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp03_03",        "vdsp03",             0xF1BC0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_040,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},
    {"pdev_vdsp",          "vdsp03_04",        "vdsp03",             0x00000000U, 0x00000000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_110,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "ir",         &gs_utlb_ir_dspp_dspd_v4h},

    /* VISIONIP */
    {"pdev_visionip_core", "dof_00",           "dof",                0xE7A10000U, 0x00010000U, gs_int_dof_v4h[0],           2U,    R_PMA_HWA_ID_VIP_DOF,         OSAL_PM_POLICY_CG,      OSAL_PM_POST4CKCR,            "vip0",       &gs_utlb_vip0_dof_v4h},
    {"pdev_visionip_core", "smd_ps_00",        "smd_ps" ,            0xE7A20000U, 0x00010000U, gs_int_dof_v4h[1],           2U,    R_PMA_HWA_ID_VIP_DOF,         OSAL_PM_POLICY_CG,      OSAL_PM_POST4CKCR,            "vip0",       &gs_utlb_vip0_smps_v4h},
    {"pdev_visionip_core", "smd_post_00",      "smd_post",           0xE7A40000U, 0x00010000U, gs_int_dof_v4h[2],           2U,    R_PMA_HWA_ID_VIP_DOF,         OSAL_PM_POLICY_CG,      OSAL_PM_POST4CKCR,            "vip0",       &gs_utlb_vip0_smpo_v4h},
    {"pdev_visionip_core", "dof_fcprm_00",     "dof_fcprm",          0xE7AB1000U, 0x00001000U, NULL,                        0U,    R_PMA_HWA_ID_VIP_DOF,         OSAL_PM_POLICY_CG,      OSAL_PM_POST4CKCR,            "vip0",       &gs_utlb_vip0_dof_v4h},
    {"pdev_visionip_wrap", "vip_wrap_00",      "vip_wrap",           0xE7B3B000U, 0x00001000U, NULL,                        0U,    R_PMA_HWA_ID_VIP_DOF,         OSAL_PM_POLICY_CG,      OSAL_PM_POST4CKCR,            "vip0",       &gs_utlb_vip0_wrap_v4h},

    /* ICCOM */
    {"pdev_iccom",         "iccom_00",         "iccom",              0xE6260000U, 0x00010000U, gs_int_iccom_v4h,           12U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* ENCODER */
    {"pdev_ivcp1e",        "ivcp1e_00",        "ivcp1e",             0xFEA00000U, 0x00010000U, gs_int_ivcp1e_v4h,           2U,    R_PMA_HWA_ID_V4H_045,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vc0",        &gs_utlb_vc_ivcp1e_v4h},
    {"pdev_vcp4",          "vcp4_00",          "vcp4",               0xFE910000U, 0x00010000U, gs_int_vcp4_v4h,             2U,    R_PMA_HWA_ID_V4H_046,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_vcp4",          "fcpc_00",          "fcpc",               0xFE90F000U, 0x00001000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_047,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* ISP */
    {"pdev_isp1",          "cisp_00",          "cisp",               0xFEC00000U, 0x00100000U, gs_int_isp_cisp_v4h[0],      1U,    R_PMA_HWA_ID_V4H_047,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_isp0_ch0ch1ch2ch3ch4_v4h},
    {"pdev_isp2",          "cisp_01",          "cisp",               0xFEE00000U, 0x00100000U, gs_int_isp_cisp_v4h[1],      1U,    R_PMA_HWA_ID_V4H_048,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_isp1_ch0ch1ch2ch3ch4_v4h},
    {"pdev_isp1",          "tisp_00",          "tisp",               0xFED00000U, 0x00010000U, gs_int_isp_tisp_v4h[0],      1U,    R_PMA_HWA_ID_V4H_049,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_isp1",          "tisp_01",          "tisp",               0xFED20000U, 0x00010000U, gs_int_isp_tisp_v4h[1],      1U,    R_PMA_HWA_ID_V4H_050,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_isp2",          "vspx_00",          "vspx",               0xFEDD0000U, 0x00008000U, gs_int_isp_vspx_v4h[0],      1U,    R_PMA_HWA_ID_V4H_051,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi1",        &gs_utlb_vi1_fcpvx_ch0_v4h},
    {"pdev_isp2",          "vspx_01",          "vspx",               0xFEDD8000U, 0x00008000U, gs_int_isp_vspx_v4h[1],      1U,    R_PMA_HWA_ID_V4H_052,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi1",        &gs_utlb_vi1_fcpvx_ch1_v4h},
    {"pdev_isp2",          "fcpvx_00",         "fcpvx",              0xFEDB0000U, 0x00008000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_105,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi1",        &gs_utlb_vi1_fcpvx_ch0_v4h},
    {"pdev_isp2",          "fcpvx_01",         "fcpvx",              0xFEDB8000U, 0x00008000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_106,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi1",        &gs_utlb_vi1_fcpvx_ch1_v4h},

    /* FieldBIST */
    {"pdev_fbc",           "fbc_00",           "fbc",                0xFF830000U, 0x00002000U, gs_int_fbc_v4h,              1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba02",         "fba_ims0",         "fba",                0xFE605000U, 0x00001000U, gs_int_fba_v4h[0],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba02",         "fba_ims1",         "fba",                0xFE606000U, 0x00001000U, gs_int_fba_v4h[1],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba02",         "fba_imr0",         "fba",                0xFE601000U, 0x00001000U, gs_int_fba_v4h[2],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba02",         "fba_imr1",         "fba",                0xFE602000U, 0x00001000U, gs_int_fba_v4h[3],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba02",         "fba_imr2",         "fba",                0xFE603000U, 0x00001000U, gs_int_fba_v4h[4],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba05",         "fba_imp0",         "fba",                0xFF8C0000U, 0x00001000U, gs_int_fba_v4h[5],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba05",         "fba_imp1",         "fba",                0xFF8C1000U, 0x00001000U, gs_int_fba_v4h[6],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba05",         "fba_imp2",         "fba",                0xFF8C2000U, 0x00001000U, gs_int_fba_v4h[7],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba05",         "fba_imp3",         "fba",                0xFF8C3000U, 0x00001000U, gs_int_fba_v4h[8],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba05",         "fba_imp_ocv0",     "fba",                0xFF8C4000U, 0x00001000U, gs_int_fba_v4h[9],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba05",         "fba_imp_ocv1",     "fba",                0xFF8C5000U, 0x00001000U, gs_int_fba_v4h[10],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba05",         "fba_imp_ocv2",     "fba",                0xFF8C6000U, 0x00001000U, gs_int_fba_v4h[11],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba05",         "fba_imp_ocv3",     "fba",                0xFF8C7000U, 0x00001000U, gs_int_fba_v4h[12],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba07",         "fba_imp_dp0",      "fba",                0xFFF56000U, 0x00001000U, gs_int_fba_v4h[13],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba07",         "fba_imp_dp1",      "fba",                0xFFF57000U, 0x00001000U, gs_int_fba_v4h[14],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba06",         "fba_imp_cnn0",     "fba",                0xFFF50000U, 0x00001000U, gs_int_fba_v4h[15],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba08",         "fba_imp_dsp0",     "fba",                0xFFF64000U, 0x00001000U, gs_int_fba_v4h[16],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba08",         "fba_imp_dsp1",     "fba",                0xFFF65000U, 0x00001000U, gs_int_fba_v4h[17],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba08",         "fba_imp_dsp2",     "fba",                0xFFF66000U, 0x00001000U, gs_int_fba_v4h[18],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba08",         "fba_imp_dsp3",     "fba",                0xFFF67000U, 0x00001000U, gs_int_fba_v4h[19],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba07",         "fba_imp_cnram0",   "fba",                0xFFF55000U, 0x00001000U, gs_int_fba_v4h[20],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba09",         "fba_imp_slim0",    "fba",                0xFF8F4000U, 0x00001000U, gs_int_fba_v4h[21],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba09",         "fba_imp_slim1",    "fba",                0xFF8F5000U, 0x00001000U, gs_int_fba_v4h[22],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba01",         "fba_umfl0",        "fba",                0xE7B61000U, 0x00001000U, gs_int_fba_v4h[23],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST4CKCR,            "invalid",    NULL},
    {"pdev_fba01",         "fba_smps0",        "fba",                0xE7B64000U, 0x00001000U, gs_int_fba_v4h[24],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST4CKCR,            "invalid",    NULL},
    {"pdev_fba01",         "fba_smpo0",        "fba",                0xE7B63000U, 0x00001000U, gs_int_fba_v4h[25],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST4CKCR,            "invalid",    NULL},
    {"pdev_fba01",         "fba_pap0",         "fba",                0xE7B62000U, 0x00001000U, gs_int_fba_v4h[26],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST4CKCR,            "invalid",    NULL},
    {"pdev_fba03",         "fba_isp0",         "fba",                0xFEAC1000U, 0x00001000U, gs_int_fba_v4h[27],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba03",         "fba_isp1",         "fba",                0xFEAC2000U, 0x00001000U, gs_int_fba_v4h[28],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR,             "invalid",    NULL},
    {"pdev_fba04",         "fba_ca76d0",       "fba",                0xFF870000U, 0x00001000U, gs_int_fba_v4h[29],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST2CKCR,            "invalid",    NULL},
    {"pdev_fba04",         "fba_ca76d1",       "fba",                0xFF873000U, 0x00001000U, gs_int_fba_v4h[30],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST2CKCR,            "invalid",    NULL},
    {"pdev_fba04",         "fba_ca760",        "fba",                0xFF871000U, 0x00001000U, gs_int_fba_v4h[31],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST2CKCR,            "invalid",    NULL},
    {"pdev_fba04",         "fba_ca761",        "fba",                0xFF872000U, 0x00001000U, gs_int_fba_v4h[32],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST2CKCR,            "invalid",    NULL},
    {"pdev_fba04",         "fba_ca762",        "fba",                0xFF874000U, 0x00001000U, gs_int_fba_v4h[33],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST2CKCR,            "invalid",    NULL},
    {"pdev_fba04",         "fba_ca763",        "fba",                0xFF875000U, 0x00001000U, gs_int_fba_v4h[34],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POST2CKCR,            "invalid",    NULL},

    /* RTT */
    {"pdev_rfso",          "rfso_00",          "rfso",               0xFFE80000U, 0x00001000U, gs_int_rtt_v4h[0],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_01",          "rfso",               0xFFE81000U, 0x00001000U, gs_int_rtt_v4h[1],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_02",          "rfso",               0xFFE82000U, 0x00001000U, gs_int_rtt_v4h[2],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_03",          "rfso",               0xFFE83000U, 0x00001000U, gs_int_rtt_v4h[3],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_04",          "rfso",               0xFFE84000U, 0x00001000U, gs_int_rtt_v4h[4],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_05",          "rfso",               0xFFE85000U, 0x00001000U, gs_int_rtt_v4h[5],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_06",          "rfso",               0xFFE86000U, 0x00001000U, gs_int_rtt_v4h[6],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_07",          "rfso",               0xFFE87000U, 0x00001000U, gs_int_rtt_v4h[7],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_08",          "rfso",               0xFFE88000U, 0x00001000U, gs_int_rtt_v4h[8],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_09",          "rfso",               0xFFE89000U, 0x00001000U, gs_int_rtt_v4h[9],           1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_rfso",          "rfso_10",          "rfso",               0xFFE8A000U, 0x00001000U, gs_int_rtt_v4h[10],          1U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* caiplite */
    {"pdev_caiplite",      "caiplite_wrapper_00", "caiplite_wrapper",0x00000000U, 0x00000000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_111,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_caiplite",      "caiplite_00",      "caiplite",           0xE7800000U, 0x00001000U, gs_int_caiplite_v4h[0],      1U,    R_PMA_HWA_ID_V4H_053,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_caiplite",      "caiplite_01",      "caiplite",           0xE7801000U, 0x00001000U, gs_int_caiplite_v4h[1],      1U,    R_PMA_HWA_ID_V4H_054,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_caiplite",      "caiplite_02",      "caiplite",           0xE7802000U, 0x00001000U, gs_int_caiplite_v4h[2],      1U,    R_PMA_HWA_ID_V4H_055,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_caiplite",      "caiplite_03",      "caiplite",           0xE7803000U, 0x00001000U, gs_int_caiplite_v4h[3],      1U,    R_PMA_HWA_ID_V4H_056,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_caiplite",      "caiplite_04",      "caiplite",           0xE7804000U, 0x00001000U, gs_int_caiplite_v4h[4],      1U,    R_PMA_HWA_ID_V4H_057,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_caiplite",      "caiplite_05",      "caiplite",           0xE7805000U, 0x00001000U, gs_int_caiplite_v4h[5],      1U,    R_PMA_HWA_ID_V4H_058,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_caiplite",      "caiplite_06",      "caiplite",           0xE7806000U, 0x00001000U, gs_int_caiplite_v4h[6],      1U,    R_PMA_HWA_ID_V4H_059,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_caiplite",      "caiplite_07",      "caiplite",           0xE7807000U, 0x00001000U, gs_int_caiplite_v4h[7],      1U,    R_PMA_HWA_ID_V4H_060,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* wcrc_caiplite */
    {"pdev_wcrc",          "wcrc_caiplite_00", "wcrc_caiplite",      0xE7080000U, 0x00001000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_061,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wcrc",          "wcrc_caiplite_01", "wcrc_caiplite",      0xE7090000U, 0x00001000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_062,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wcrc",          "wcrc_caiplite_02", "wcrc_caiplite",      0xE70A0000U, 0x00001000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_063,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wcrc",          "wcrc_caiplite_03", "wcrc_caiplite",      0xE70B0000U, 0x00001000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_064,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* vin */
    {"pdev_vin",           "vin_00",           "vin",                0xE6EF0000U, 0x00001000U, gs_int_vin_v4h[0],           1U,    R_PMA_HWA_ID_V4H_069,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {"pdev_vin",           "vin_01",           "vin",                0xE6EF1000U, 0x00001000U, gs_int_vin_v4h[1],           1U,    R_PMA_HWA_ID_V4H_070,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {"pdev_vin",           "vin_02",           "vin",                0xE6EF2000U, 0x00001000U, gs_int_vin_v4h[2],           1U,    R_PMA_HWA_ID_V4H_071,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {"pdev_vin",           "vin_03",           "vin",                0xE6EF3000U, 0x00001000U, gs_int_vin_v4h[3],           1U,    R_PMA_HWA_ID_V4H_072,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {"pdev_vin",           "vin_04",           "vin",                0xE6EF4000U, 0x00001000U, gs_int_vin_v4h[4],           1U,    R_PMA_HWA_ID_V4H_073,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {"pdev_vin",           "vin_05",           "vin",                0xE6EF5000U, 0x00001000U, gs_int_vin_v4h[5],           1U,    R_PMA_HWA_ID_V4H_074,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {"pdev_vin",           "vin_06",           "vin",                0xE6EF6000U, 0x00001000U, gs_int_vin_v4h[6],           1U,    R_PMA_HWA_ID_V4H_075,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {"pdev_vin",           "vin_07",           "vin",                0xE6EF7000U, 0x00001000U, gs_int_vin_v4h[7],           1U,    R_PMA_HWA_ID_V4H_076,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch0_v4h},
    {"pdev_vin",           "vin_10",           "vin",                0xE6EF8000U, 0x00001000U, gs_int_vin_v4h[8],           1U,    R_PMA_HWA_ID_V4H_077,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {"pdev_vin",           "vin_11",           "vin",                0xE6EF9000U, 0x00001000U, gs_int_vin_v4h[9],           1U,    R_PMA_HWA_ID_V4H_078,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {"pdev_vin",           "vin_12",           "vin",                0xE6EFA000U, 0x00001000U, gs_int_vin_v4h[10],          1U,    R_PMA_HWA_ID_V4H_079,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {"pdev_vin",           "vin_13",           "vin",                0xE6EFB000U, 0x00001000U, gs_int_vin_v4h[11],          1U,    R_PMA_HWA_ID_V4H_080,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {"pdev_vin",           "vin_14",           "vin",                0xE6EFC000U, 0x00001000U, gs_int_vin_v4h[12],          1U,    R_PMA_HWA_ID_V4H_081,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {"pdev_vin",           "vin_15",           "vin",                0xE6EFD000U, 0x00001000U, gs_int_vin_v4h[13],          1U,    R_PMA_HWA_ID_V4H_082,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {"pdev_vin",           "vin_16",           "vin",                0xE6EFE000U, 0x00001000U, gs_int_vin_v4h[14],          1U,    R_PMA_HWA_ID_V4H_083,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch1_v4h},
    {"pdev_vin",           "vin_17",           "vin",                0xE6EFF000U, 0x00001000U, gs_int_vin_v4h[15],          1U,    R_PMA_HWA_ID_V4H_084,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi0",        &gs_utlb_vi0_vin_ch1_v4h},

    /* vsp */
    {"pdev_vsp",           "vsp_00",           "vsp",                0xFEA20000U, 0x00008000U, gs_int_vsp_v4h[0],           1U,    R_PMA_HWA_ID_V4H_085,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi1",        &gs_utlb_vi1_fcpvd_ch0_v4h},
    {"pdev_vsp",           "vsp_01",           "vsp",                0xFEA28000U, 0x00008000U, gs_int_vsp_v4h[1],           1U,    R_PMA_HWA_ID_V4H_086,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi1",        &gs_utlb_vi1_fcpvd_ch1_v4h},

    /* csi */
    {"pdev_csi",           "csi_00",           "csi",                0xFE500000U, 0x00040000U, gs_int_csi_v4h[0],           1U,    R_PMA_HWA_ID_V4H_087,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_csi",           "csi_01",           "csi",                0xFE540000U, 0x00040000U, gs_int_csi_v4h[1],           1U,    R_PMA_HWA_ID_V4H_088,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* du */
    {"pdev_du",            "du_00",            "du",                 0xFEB00000U, 0x00040000U, gs_int_du_v4h,               2U,    R_PMA_HWA_ID_V4H_089,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* dsi */
    {"pdev_dsi",           "dsi_00",           "dsi",                0xFED80000U, 0x00010000U, gs_int_dsi_v4h[0],           9U,    R_PMA_HWA_ID_V4H_090,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vi1",        &gs_utlb_vi1_dsi_ch0_v4h},
    {"pdev_dsi",           "dsi_01",           "dsi",                0xFED90000U, 0x00010000U, gs_int_dsi_v4h[1],           9U,    R_PMA_HWA_ID_V4H_091,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* wwdt */
    {"pdev_wwdt1",          "wwdt_00",          "wwdt",               0xFFC90000U, 0x00001000U, gs_int_wwdt_v4h[0],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt1",          "wwdt_01",          "wwdt",               0xFFCA0000U, 0x00001000U, gs_int_wwdt_v4h[1],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt1",          "wwdt_02",          "wwdt",               0xFFCB0000U, 0x00001000U, gs_int_wwdt_v4h[2],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt1",          "wwdt_03",          "wwdt",               0xFFCC0000U, 0x00001000U, gs_int_wwdt_v4h[3],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt2",          "wwdt_04",          "wwdt",               0xFFCF0000U, 0x00001000U, gs_int_wwdt_v4h[4],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt3",          "wwdt_05",          "wwdt",               0xFFEF0000U, 0x00001000U, gs_int_wwdt_v4h[5],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt4",          "wwdt_06",          "wwdt",               0xFFF10000U, 0x00001000U, gs_int_wwdt_v4h[6],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt4",          "wwdt_07",          "wwdt",               0xFFF20000U, 0x00001000U, gs_int_wwdt_v4h[7],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt4",          "wwdt_08",          "wwdt",               0xFFF30000U, 0x00001000U, gs_int_wwdt_v4h[8],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},
    {"pdev_wwdt4",          "wwdt_09",          "wwdt",               0xFFF40000U, 0x00001000U, gs_int_wwdt_v4h[9],          2U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* doc */
    {"pdev_doc",           "doc_00",           "doc",                0xFEBA0000U, 0x00011000U, gs_int_doc_v4h,              2U,    R_PMA_HWA_ID_V4H_092,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* dsc */
    {"pdev_dsc",           "dsc_00",           "dsc",                0xFEB8D000U, 0x00001000U, gs_int_dsc_v4h,              1U,    R_PMA_HWA_ID_V4H_093,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* pap */
    {"pdev_pap",           "hsm_00",           "hsm",                0xFF000000U, 0x00080000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_093,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "dcm_00",           "dcm",                0xFF080000U, 0x00064000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_094,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "cbf_00",           "cbf",                0xFF0E8000U, 0x00003000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_095,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "sbf_00",           "sbf",                0xFF0EC000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_096,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "osm_00",           "osm",                0xFF0F0000U, 0x00010000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_097,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "ism_00",           "ism",                0xFF100000U, 0x00020000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_098,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "ism_01",           "ism",                0xFF120000U, 0x00020000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_099,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "sdmac_00",         "sdmac",              0xFF180000U, 0x00010000U, gs_int_sdmac_v4h,            2U,    R_PMA_HWA_ID_V4H_100,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "fbs_00",           "fbs",                0xFF200000U, 0x00080000U, gs_int_fbs_v4h,              1U,    R_PMA_HWA_ID_V4H_101,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "apb_00",           "apb",                0xFF290000U, 0x00004000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_102,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},
    {"pdev_pap",           "dul_00",           "dul",                0xFF298000U, 0x00001000U, NULL,                        0U,    R_PMA_HWA_ID_V4H_103,         OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "vip1",       &gs_utlb_vip1_pap_v4h},

    /* SYSTEM RAM */
    {"pdev_sysram",        "system_ram_00",    "system_ram",         0xE6300000U, 0x00100000U, NULL,                        0U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_INVALID, OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    /* SYSTEM RAM */
    {"pdev_icumx",         "icumx_cmd_00",     "icumx_cmd",          0xE6610000U, 0x00008000U, gs_int_icumx_v4h,           20U,    R_PMA_HWA_ID_NONE,            OSAL_PM_POLICY_CG,      OSAL_PM_POSTCKCR_INVALID,     "invalid",    NULL},

    {.id = OSAL_DEVICE_INVALID_ID}
};

/** @} */

#endif /* OSAL_DEV_CFG_V4H_H */
