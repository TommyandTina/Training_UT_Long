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
* File Name :    r_osal_os_dev_cfg_tbl_s4.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car S4
* Description :  OSAL Device Configuration file for R-Car S4
***********************************************************************************************************************/

#ifndef OSAL_OS_DEV_CFG_S4_H
#define OSAL_OS_DEV_CFG_S4_H

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_006
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_int_caiplite_s4
 * @brief osal device configuration : caiplite interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_caiplite_s4[8][1] =
{
    {
        {933}
    },
    {
        {935}
    },
    {
        {937}
    },
    {
        {939}
    },
    {
        {941}
    },
    {
        {943}
    },
    {
        {945}
    },
    {
        {947}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtdmac_control0_s4
 * @brief osal device configuration : rtdmac_control interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtdmac_control0_s4[16][1] =
{
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
    },
    {
        { 44}
    },
    {
        { 45}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtdmac_control1_s4
 * @brief osal device configuration : rtdmac_control interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtdmac_control1_s4[16][1] =
{
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
    },
    {
        { 62}
    },
    {
        { 63}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtdmac_control2_s4
 * @brief osal device configuration : rtdmac_control interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtdmac_control2_s4[16][1] =
{
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
    },
    {
        { 80}
    },
    {
        { 81}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_rtdmac_control3_s4
 * @brief osal device configuration : rtdmac_control interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtdmac_control3_s4[16][1] =
{
    {
        { 84}
    },
    {
        { 85}
    },
    {
        { 86}
    },
    {
        { 87}
    },
    {
        { 88}
    },
    {
        { 89}
    },
    {
        { 90}
    },
    {
        { 91}
    },
    {
        { 92}
    },
    {
        { 93}
    },
    {
        { 94}
    },
    {
        { 95}
    },
    {
        { 96}
    },
    {
        { 97}
    },
    {
        { 98}
    },
    {
        { 99}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_ships_s4
 * @brief osal device configuration : ships interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_ships_s4[3][2] =
{
    {
        {948}, {949}
    },
    {
        {950}, {951}
    },
    {
        {952}, {953}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_iccom_s4
 * @brief osal device configuration : iccom interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_iccom_s4[24][1] =
{
    {
        {320}
    },
    {
        {322}
    },
    {
        {324}
    },
    {
        {326}
    },
    {
        {328}
    },
    {
        {330}
    },
    {
        {332}
    },
    {
        {334}
    },
    {
        {336}
    },
    {
        {338}
    },
    {
        {340}
    },
    {
        {342}
    },
    {
        {344}
    },
    {
        {346}
    },
    {
        {348}
    },
    {
        {350}
    },
    {
        {354}
    },
    {
        {356}
    },
    {
        {358}
    },
    {
        {360}
    },
    {
        {362}
    },
    {
        {364}
    },
    {
        {366}
    },
    {
        {368}
    }
};

/*******************************************************************************************************************//**
 * @var gs_int_icumx_s4
 * @brief osal device configuration : icumx interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_icumx_s4[20] =
{
    {192},{193},{194},{195},{196},{197},{198},{199},{200},{201},
    {202},{203},{204},{205},{206},{207},{208},{209},{210},{211},
};

/*******************************************************************************************************************//**
 * @var gs_int_fbc_s4
 * @brief osal device configuration : FBC interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fbc_s4[1] =
{
    {740}
};

/*******************************************************************************************************************//**
 * @var gs_int_fba_s4
 * @brief osal device configuration : FBA interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_fba_s4[6][1] =
{
    {
        {621}
    },
    {
        {624}
    },
    {
        {622}
    },
    {
        {623}
    },
    {
        {625}
    },
    {
        {626}
    },
};

/*******************************************************************************************************************//**
 * @var gs_int_rtt_s4
 * @brief osal device configuration : RTT interrupt
***********************************************************************************************************************/
static st_osal_interrupt_info_t gs_int_rtt_s4[11][1] =
{
    {
        {218}
    },
    {
        {219}
    },
    {
        {220}
    },
    {
        {221}
    },
    {
        {222}
    },
    {
        {223}
    },
    {
        {224}
    },
    {
        {225}
    },
    {
        {226}
    },
    {
        {227}
    },
    {
        {228}
    }
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_032]
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_00_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_00_s4 =
{
    1, { 0}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_01_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_01_s4 =
{
    1, {  1}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_02_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_02_s4 =
{
    1, {  2}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_03_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_03_s4 =
{
    1, {  3}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_04_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_04_s4 =
{
    1, {  4}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_05_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_05_s4 =
{
    1, {  5}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_06_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_06_s4 =
{
    1, {  6}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_07_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_07_s4 =
{
    1, {  7}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_08_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_08_s4 =
{
    1, {  8}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_09_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_09_s4 =
{
    1, {  9}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_10_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_10_s4 =
{
    1, { 10}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_11_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_11_s4 =
{
    1, { 11}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_12_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_12_s4 =
{
    1, { 12}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_13_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_13_s4 =
{
    1, { 13}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_14_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_14_s4 =
{
    1, { 14}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac0_15_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac0_15_s4 =
{
    1, { 15}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_00_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_00_s4 =
{
    1, { 16}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_01_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_01_s4 =
{
    1, { 17}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_02_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_02_s4 =
{
    1, { 18}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_03_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_03_s4 =
{
    1, { 19}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_04_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_04_s4 =
{
    1, { 20}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_05_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_05_s4 =
{
    1, { 21}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_06_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_06_s4 =
{
    1, { 22}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_07_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_07_s4 =
{
    1, { 23}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_08_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_08_s4 =
{
    1, { 24}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_09_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_09_s4 =
{
    1, { 25}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_10_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_10_s4 =
{
    1, { 26}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_11_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_11_s4 =
{
    1, { 27}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_12_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_12_s4 =
{
    1, { 28}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_13_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_13_s4 =
{
    1, { 29}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_14_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_14_s4 =
{
    1, { 30}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac1_15_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac1_15_s4 =
{
    1, { 31}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_00_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_00_s4 =
{
    1, { 32}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_01_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_01_s4 =
{
    1, { 33}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_02_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_02_s4 =
{
    1, { 34}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_03_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_03_s4 =
{
    1, { 35}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_04_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_04_s4 =
{
    1, { 36}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_05_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_05_s4 =
{
    1, { 37}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_06_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_06_s4 =
{
    1, { 38}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_07_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_07_s4 =
{
    1, { 39}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_08_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_08_s4 =
{
    1, { 40}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_09_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_09_s4 =
{
    1, { 41}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_10_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_10_s4 =
{
    1, { 42}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_11_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_11_s4 =
{
    1, { 43}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_12_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_12_s4 =
{
    1, { 44}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_13_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_13_s4 =
{
    1, { 45}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_14_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_14_s4 =
{
    1, { 46}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac2_15_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac2_15_s4 =
{
    1, { 47}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_00_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_00_s4 =
{
    1, { 48}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_01_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_01_s4 =
{
    1, { 49}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_02_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_02_s4 =
{
    1, { 50}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_03_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_03_s4 =
{
    1, { 51}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_04_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_04_s4 =
{
    1, { 52}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_05_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_05_s4 =
{
    1, { 53}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_06_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_06_s4 =
{
    1, { 54}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_07_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_07_s4 =
{
    1, { 55}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_08_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_08_s4 =
{
    1, { 56}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_09_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_09_s4 =
{
    1, { 57}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_10_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_10_s4 =
{
    1, { 58}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_11_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_11_s4 =
{
    1, { 59}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_12_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_12_s4 =
{
    1, { 60}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_13_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_13_s4 =
{
    1, { 61}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_14_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_14_s4 =
{
    1, { 62}
};

/*******************************************************************************************************************//**
 * @var gs_utlb_rt1_rtdmac3_15_s4
 * @brief osal device configuration : uTLB number
***********************************************************************************************************************/
/* PRQA S 0686 3 # There is no problem because the array and effective size are held as structures and used together. */
static st_osal_utlb_assignment_info_t gs_utlb_rt1_rtdmac3_15_s4 =
{
    1, { 63}
};

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_007
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_device_info_s4
 * @brief osal device configuration : s4 device information structure
***********************************************************************************************************************/
/* PRQA S 3218 1 # Definition within a function reduces readability and maintainability, so no change. */
static const st_osal_device_info_t gs_osal_device_info_s4[] =
{
    /* group name,   device unique ID,      device type,    physical address,    range,     interrupt,    number of interrupts, power manager id,               POSTCKCR,               AXI bus name,    assignment */
    /* caiplite */
    {NULL,            "caiplite_00",           "caiplite",           0xE7800000U, 0x00001000U, gs_int_caiplite_s4[0],       1U, R_PMA_HWA_ID_S4_AES_ACC0,          OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "caiplite_01",           "caiplite",           0xE7801000U, 0x00001000U, gs_int_caiplite_s4[1],       1U, R_PMA_HWA_ID_S4_AES_ACC1,          OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "caiplite_02",           "caiplite",           0xE7802000U, 0x00001000U, gs_int_caiplite_s4[2],       1U, R_PMA_HWA_ID_S4_AES_ACC2,          OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "caiplite_03",           "caiplite",           0xE7803000U, 0x00001000U, gs_int_caiplite_s4[3],       1U, R_PMA_HWA_ID_S4_AES_ACC3,          OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "caiplite_04",           "caiplite",           0xE7804000U, 0x00001000U, gs_int_caiplite_s4[4],       1U, R_PMA_HWA_ID_S4_AES_ACC4,          OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "caiplite_05",           "caiplite",           0xE7805000U, 0x00001000U, gs_int_caiplite_s4[5],       1U, R_PMA_HWA_ID_S4_AES_ACC5,          OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "caiplite_06",           "caiplite",           0xE7806000U, 0x00001000U, gs_int_caiplite_s4[6],       1U, R_PMA_HWA_ID_S4_AES_ACC6,          OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "caiplite_07",           "caiplite",           0xE7807000U, 0x00001000U, gs_int_caiplite_s4[7],       1U, R_PMA_HWA_ID_S4_AES_ACC7,          OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "caiplite_wrapper_00",   "caiplite_wrapper",   0x00000000U, 0x00000000U, NULL,                        0U, R_PMA_HWA_ID_S4_AES_ACC_WRAPPER,   OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* caiplite */
    {NULL,            "wcrc_caiplite_00",      "wcrc_caiplite",      0xE7080000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_S4_WCRC0,             OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "wcrc_caiplite_01",      "wcrc_caiplite",      0xE7090000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_S4_WCRC1,             OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "wcrc_caiplite_02",      "wcrc_caiplite",      0xE70A0000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_S4_WCRC2,             OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "wcrc_caiplite_03",      "wcrc_caiplite",      0xE70B0000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_S4_WCRC3,             OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* rtdmac_control0 */
    {NULL,            "rtdmac_control0_00",    "rtdmac_control0",    0xFFC10000U, 0x00001000U, gs_int_rtdmac_control0_s4[0],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_00_s4},
    {NULL,            "rtdmac_control0_01",    "rtdmac_control0",    0xFFC11000U, 0x00001000U, gs_int_rtdmac_control0_s4[1],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_01_s4},
    {NULL,            "rtdmac_control0_02",    "rtdmac_control0",    0xFFC12000U, 0x00001000U, gs_int_rtdmac_control0_s4[2],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_02_s4},
    {NULL,            "rtdmac_control0_03",    "rtdmac_control0",    0xFFC13000U, 0x00001000U, gs_int_rtdmac_control0_s4[3],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_03_s4},
    {NULL,            "rtdmac_control0_04",    "rtdmac_control0",    0xFFC14000U, 0x00001000U, gs_int_rtdmac_control0_s4[4],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_04_s4},
    {NULL,            "rtdmac_control0_05",    "rtdmac_control0",    0xFFC15000U, 0x00001000U, gs_int_rtdmac_control0_s4[5],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_05_s4},
    {NULL,            "rtdmac_control0_06",    "rtdmac_control0",    0xFFC16000U, 0x00001000U, gs_int_rtdmac_control0_s4[6],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_06_s4},
    {NULL,            "rtdmac_control0_07",    "rtdmac_control0",    0xFFC17000U, 0x00001000U, gs_int_rtdmac_control0_s4[7],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_07_s4},
    {NULL,            "rtdmac_control0_08",    "rtdmac_control0",    0xFFC18000U, 0x00001000U, gs_int_rtdmac_control0_s4[8],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_08_s4},
    {NULL,            "rtdmac_control0_09",    "rtdmac_control0",    0xFFC19000U, 0x00001000U, gs_int_rtdmac_control0_s4[9],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_09_s4},
    {NULL,            "rtdmac_control0_10",    "rtdmac_control0",    0xFFC1A000U, 0x00001000U, gs_int_rtdmac_control0_s4[10], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_10_s4},
    {NULL,            "rtdmac_control0_11",    "rtdmac_control0",    0xFFC1B000U, 0x00001000U, gs_int_rtdmac_control0_s4[11], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_11_s4},
    {NULL,            "rtdmac_control0_12",    "rtdmac_control0",    0xFFC1C000U, 0x00001000U, gs_int_rtdmac_control0_s4[12], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_12_s4},
    {NULL,            "rtdmac_control0_13",    "rtdmac_control0",    0xFFC1D000U, 0x00001000U, gs_int_rtdmac_control0_s4[13], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_13_s4},
    {NULL,            "rtdmac_control0_14",    "rtdmac_control0",    0xFFC1E000U, 0x00001000U, gs_int_rtdmac_control0_s4[14], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_14_s4},
    {NULL,            "rtdmac_control0_15",    "rtdmac_control0",    0xFFC1F000U, 0x00001000U, gs_int_rtdmac_control0_s4[15], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac0_15_s4},

    /* rtdmac_control1 */
    {NULL,            "rtdmac_control1_00",    "rtdmac_control1",    0xFFC20000U, 0x00001000U, gs_int_rtdmac_control1_s4[0],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_00_s4},
    {NULL,            "rtdmac_control1_01",    "rtdmac_control1",    0xFFC21000U, 0x00001000U, gs_int_rtdmac_control1_s4[1],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_01_s4},
    {NULL,            "rtdmac_control1_02",    "rtdmac_control1",    0xFFC22000U, 0x00001000U, gs_int_rtdmac_control1_s4[2],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_02_s4},
    {NULL,            "rtdmac_control1_03",    "rtdmac_control1",    0xFFC23000U, 0x00001000U, gs_int_rtdmac_control1_s4[3],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_03_s4},
    {NULL,            "rtdmac_control1_04",    "rtdmac_control1",    0xFFC24000U, 0x00001000U, gs_int_rtdmac_control1_s4[4],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_04_s4},
    {NULL,            "rtdmac_control1_05",    "rtdmac_control1",    0xFFC25000U, 0x00001000U, gs_int_rtdmac_control1_s4[5],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_05_s4},
    {NULL,            "rtdmac_control1_06",    "rtdmac_control1",    0xFFC26000U, 0x00001000U, gs_int_rtdmac_control1_s4[6],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_06_s4},
    {NULL,            "rtdmac_control1_07",    "rtdmac_control1",    0xFFC27000U, 0x00001000U, gs_int_rtdmac_control1_s4[7],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_07_s4},
    {NULL,            "rtdmac_control1_08",    "rtdmac_control1",    0xFFC28000U, 0x00001000U, gs_int_rtdmac_control1_s4[8],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_08_s4},
    {NULL,            "rtdmac_control1_09",    "rtdmac_control1",    0xFFC29000U, 0x00001000U, gs_int_rtdmac_control1_s4[9],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_09_s4},
    {NULL,            "rtdmac_control1_10",    "rtdmac_control1",    0xFFC2A000U, 0x00001000U, gs_int_rtdmac_control1_s4[10], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_10_s4},
    {NULL,            "rtdmac_control1_11",    "rtdmac_control1",    0xFFC2B000U, 0x00001000U, gs_int_rtdmac_control1_s4[11], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_11_s4},
    {NULL,            "rtdmac_control1_12",    "rtdmac_control1",    0xFFC2C000U, 0x00001000U, gs_int_rtdmac_control1_s4[12], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_12_s4},
    {NULL,            "rtdmac_control1_13",    "rtdmac_control1",    0xFFC2D000U, 0x00001000U, gs_int_rtdmac_control1_s4[13], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_13_s4},
    {NULL,            "rtdmac_control1_14",    "rtdmac_control1",    0xFFC2E000U, 0x00001000U, gs_int_rtdmac_control1_s4[14], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_14_s4},
    {NULL,            "rtdmac_control1_15",    "rtdmac_control1",    0xFFC2F000U, 0x00001000U, gs_int_rtdmac_control1_s4[15], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac1_15_s4},

    /* rtdmac_control2 */
    {NULL,            "rtdmac_control2_00",    "rtdmac_control2",    0xFFD70000U, 0x00001000U, gs_int_rtdmac_control2_s4[0],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_00_s4},
    {NULL,            "rtdmac_control2_01",    "rtdmac_control2",    0xFFD71000U, 0x00001000U, gs_int_rtdmac_control2_s4[1],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_01_s4},
    {NULL,            "rtdmac_control2_02",    "rtdmac_control2",    0xFFD72000U, 0x00001000U, gs_int_rtdmac_control2_s4[2],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_02_s4},
    {NULL,            "rtdmac_control2_03",    "rtdmac_control2",    0xFFD73000U, 0x00001000U, gs_int_rtdmac_control2_s4[3],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_03_s4},
    {NULL,            "rtdmac_control2_04",    "rtdmac_control2",    0xFFD74000U, 0x00001000U, gs_int_rtdmac_control2_s4[4],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_04_s4},
    {NULL,            "rtdmac_control2_05",    "rtdmac_control2",    0xFFD75000U, 0x00001000U, gs_int_rtdmac_control2_s4[5],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_05_s4},
    {NULL,            "rtdmac_control2_06",    "rtdmac_control2",    0xFFD76000U, 0x00001000U, gs_int_rtdmac_control2_s4[6],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_06_s4},
    {NULL,            "rtdmac_control2_07",    "rtdmac_control2",    0xFFD77000U, 0x00001000U, gs_int_rtdmac_control2_s4[7],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_07_s4},
    {NULL,            "rtdmac_control2_08",    "rtdmac_control2",    0xFFD78000U, 0x00001000U, gs_int_rtdmac_control2_s4[8],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_08_s4},
    {NULL,            "rtdmac_control2_09",    "rtdmac_control2",    0xFFD79000U, 0x00001000U, gs_int_rtdmac_control2_s4[9],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_09_s4},
    {NULL,            "rtdmac_control2_10",    "rtdmac_control2",    0xFFD7A000U, 0x00001000U, gs_int_rtdmac_control2_s4[10], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_10_s4},
    {NULL,            "rtdmac_control2_11",    "rtdmac_control2",    0xFFD7B000U, 0x00001000U, gs_int_rtdmac_control2_s4[11], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_11_s4},
    {NULL,            "rtdmac_control2_12",    "rtdmac_control2",    0xFFD7C000U, 0x00001000U, gs_int_rtdmac_control2_s4[12], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_12_s4},
    {NULL,            "rtdmac_control2_13",    "rtdmac_control2",    0xFFD7D000U, 0x00001000U, gs_int_rtdmac_control2_s4[13], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_13_s4},
    {NULL,            "rtdmac_control2_14",    "rtdmac_control2",    0xFFD7E000U, 0x00001000U, gs_int_rtdmac_control2_s4[14], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_14_s4},
    {NULL,            "rtdmac_control2_15",    "rtdmac_control2",    0xFFD7F000U, 0x00001000U, gs_int_rtdmac_control2_s4[15], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac2_15_s4},

    /* rtdmac_control3 */
    {NULL,            "rtdmac_control3_00",    "rtdmac_control3",    0xFFD80000U, 0x00001000U, gs_int_rtdmac_control3_s4[0],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_00_s4},
    {NULL,            "rtdmac_control3_01",    "rtdmac_control3",    0xFFD81000U, 0x00001000U, gs_int_rtdmac_control3_s4[1],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_01_s4},
    {NULL,            "rtdmac_control3_02",    "rtdmac_control3",    0xFFD82000U, 0x00001000U, gs_int_rtdmac_control3_s4[2],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_02_s4},
    {NULL,            "rtdmac_control3_03",    "rtdmac_control3",    0xFFD83000U, 0x00001000U, gs_int_rtdmac_control3_s4[3],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_03_s4},
    {NULL,            "rtdmac_control3_04",    "rtdmac_control3",    0xFFD84000U, 0x00001000U, gs_int_rtdmac_control3_s4[4],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_04_s4},
    {NULL,            "rtdmac_control3_05",    "rtdmac_control3",    0xFFD85000U, 0x00001000U, gs_int_rtdmac_control3_s4[5],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_05_s4},
    {NULL,            "rtdmac_control3_06",    "rtdmac_control3",    0xFFD86000U, 0x00001000U, gs_int_rtdmac_control3_s4[6],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_06_s4},
    {NULL,            "rtdmac_control3_07",    "rtdmac_control3",    0xFFD87000U, 0x00001000U, gs_int_rtdmac_control3_s4[7],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_07_s4},
    {NULL,            "rtdmac_control3_08",    "rtdmac_control3",    0xFFD88000U, 0x00001000U, gs_int_rtdmac_control3_s4[8],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_08_s4},
    {NULL,            "rtdmac_control3_09",    "rtdmac_control3",    0xFFD89000U, 0x00001000U, gs_int_rtdmac_control3_s4[9],  1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_09_s4},
    {NULL,            "rtdmac_control3_10",    "rtdmac_control3",    0xFFD8A000U, 0x00001000U, gs_int_rtdmac_control3_s4[10], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_10_s4},
    {NULL,            "rtdmac_control3_11",    "rtdmac_control3",    0xFFD8B000U, 0x00001000U, gs_int_rtdmac_control3_s4[11], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_11_s4},
    {NULL,            "rtdmac_control3_12",    "rtdmac_control3",    0xFFD8C000U, 0x00001000U, gs_int_rtdmac_control3_s4[12], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_12_s4},
    {NULL,            "rtdmac_control3_13",    "rtdmac_control3",    0xFFD8D000U, 0x00001000U, gs_int_rtdmac_control3_s4[13], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_13_s4},
    {NULL,            "rtdmac_control3_14",    "rtdmac_control3",    0xFFD8E000U, 0x00001000U, gs_int_rtdmac_control3_s4[14], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_14_s4},
    {NULL,            "rtdmac_control3_15",    "rtdmac_control3",    0xFFD8F000U, 0x00001000U, gs_int_rtdmac_control3_s4[15], 1U, R_PMA_HWA_ID_NONE,               OSAL_PM_POSTCKCR_INVALID,  "rt1",        &gs_utlb_rt1_rtdmac3_15_s4},

    /* rtdmac_control */
    {NULL,            "rtdmac_descriptor0_00", "rtdmac_descriptor0", 0xFFD60000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_S4_RTDM0,             OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rtdmac_descriptor1_00", "rtdmac_descriptor1", 0xFFD61000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_S4_RTDM1,             OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rtdmac_descriptor2_00", "rtdmac_descriptor2", 0xFFD62000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_S4_RTDM2,             OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rtdmac_descriptor3_00", "rtdmac_descriptor3", 0xFFD63000U, 0x00001000U, NULL,                        0U, R_PMA_HWA_ID_S4_RTDM3,             OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* ships */
    {NULL,            "ships_00",              "ships",              0xE7810000U, 0x00010000U, gs_int_ships_s4[0],          2U, R_PMA_HWA_ID_S4_SHIP_S0,           OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "ships_01",              "ships",              0xE7820000U, 0x00010000U, gs_int_ships_s4[1],          2U, R_PMA_HWA_ID_S4_SHIP_S1,           OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "ships_02",              "ships",              0xE7830000U, 0x00010000U, gs_int_ships_s4[2],          2U, R_PMA_HWA_ID_S4_SHIP_S2,           OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* ICCOM(for IO) */
    {NULL,            "iccom_reg_00",          "iccom_reg",          0xE6260000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* ICCOM(for Interrupt) */
    {NULL,            "iccom_00",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[0],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_01",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[1],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_02",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[2],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_03",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[3],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_04",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[4],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_05",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[5],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_06",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[6],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_07",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[7],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_08",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[8],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_09",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[9],          1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_10",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[10],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_11",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[11],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_12",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[12],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_13",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[13],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_14",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[14],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_15",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[15],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_16",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[16],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_17",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[17],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_18",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[18],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_19",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[19],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_20",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[20],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_21",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[21],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_22",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[22],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "iccom_23",              "iccom",              0x00000000U, 0x00000000U, gs_int_iccom_s4[23],         1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* SYSRAM */
    {NULL,            "system_ram_00",         "system_ram",         0xE6300000U, 0x00060000U, NULL,                        0U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* ICUMFW */
    {NULL,            "icumx_cmd_00",          "icumx_cmd",          0xE6610000U, 0x00008000U, gs_int_icumx_s4,            20U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* FieldBIST */
    {NULL,            "gicd_00",                "gicd",              0xF1000000U, 0x00010000U, NULL,                        0U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    /* FieldBIST */
    {NULL,            "fbc_00",                 "fbc",               0xFF830000U, 0x00002000U, gs_int_fbc_s4,               1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POST2CKCR,         "invalid",    NULL},
    {NULL,            "fba_ca55d0",             "fba",               0xFF870000U, 0x00001000U, gs_int_fba_s4[0],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POST2CKCR,         "invalid",    NULL},
    {NULL,            "fba_ca55d1",             "fba",               0xFF871000U, 0x00001000U, gs_int_fba_s4[1],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POST2CKCR,         "invalid",    NULL},
    {NULL,            "fba_ca550",              "fba",               0xFF872000U, 0x00001000U, gs_int_fba_s4[2],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POST2CKCR,         "invalid",    NULL},
    {NULL,            "fba_ca551",              "fba",               0xFF873000U, 0x00001000U, gs_int_fba_s4[3],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POST2CKCR,         "invalid",    NULL},
    {NULL,            "fba_ca552",              "fba",               0xFF874000U, 0x00001000U, gs_int_fba_s4[4],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POST2CKCR,         "invalid",    NULL},
    {NULL,            "fba_ca553",              "fba",               0xFF875000U, 0x00001000U, gs_int_fba_s4[5],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POST2CKCR,         "invalid",    NULL},

    /* RTT */
    {NULL,            "rfso_00",               "rfso",               0xFFE80000U, 0x00001000U, gs_int_rtt_s4[0],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_01",               "rfso",               0xFFE81000U, 0x00001000U, gs_int_rtt_s4[1],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_02",               "rfso",               0xFFE82000U, 0x00001000U, gs_int_rtt_s4[2],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_03",               "rfso",               0xFFE83000U, 0x00001000U, gs_int_rtt_s4[3],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_04",               "rfso",               0xFFE84000U, 0x00001000U, gs_int_rtt_s4[4],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_05",               "rfso",               0xFFE85000U, 0x00001000U, gs_int_rtt_s4[5],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_06",               "rfso",               0xFFE86000U, 0x00001000U, gs_int_rtt_s4[6],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_07",               "rfso",               0xFFE87000U, 0x00001000U, gs_int_rtt_s4[7],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_08",               "rfso",               0xFFE88000U, 0x00001000U, gs_int_rtt_s4[8],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_09",               "rfso",               0xFFE89000U, 0x00001000U, gs_int_rtt_s4[9],            1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},
    {NULL,            "rfso_10",               "rfso",               0xFFE8A000U, 0x00001000U, gs_int_rtt_s4[10],           1U, R_PMA_HWA_ID_NONE,                 OSAL_PM_POSTCKCR_INVALID,  "invalid",    NULL},

    {.id = OSAL_DEVICE_INVALID_ID}
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_031]
 * [Covers: XOS3_OSAL_QNX_UD_DD_NON_002]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

#endif /* OSAL_OS_DEV_CFG_S4_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
