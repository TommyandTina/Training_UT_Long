/***********************************************************************************************************************
* Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* Version :      0.0.1
* Description  : Internal function for PM API
***********************************************************************************************************************/

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <drv/sysc/sysc.h>
#include <drv/cpg/cpg_driver.h>

#include "target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h"
/*======================================================================================================================
Private global variables
======================================================================================================================*/
int sysc_fd;
int cpg_fd;
static e_cpg_domain_id_t cpg_idmax;
static e_sysc_domain_id_t sysc_idmax;

const uint32_t * g_pma_bsp_wrap_sysc_id_tbl;
const uint32_t * g_pma_bsp_wrap_cpg_id_tbl;
const uint32_t * g_pma_bsp_wrap_post_clk_id_tbl;
const uint32_t * g_pma_bsp_wrap_cpg_div0_id_tbl;
const uint32_t * g_pma_bsp_wrap_cpg_div2_id_tbl;
const uint32_t * g_pma_bsp_wrap_cpg_div3_id_tbl;
const uint32_t * g_pma_bsp_wrap_cpg_div4_id_tbl;

/* PRQA S 3684 2 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const uint32_t g_pma_bsp_wrap_sysc_id_tbl_v4h[];
extern const uint32_t g_pma_bsp_wrap_cpg_id_tbl_v4h[];
extern const uint32_t g_pma_bsp_wrap_post_clk_id_tbl_v4h[R_PMA_MNG_CLK_POSTCKCR];
extern const uint32_t g_pma_bsp_wrap_cpg_div0_id_tbl_v4h[(R_PMA_MNG_CLK_DIVIDER-1)];
extern const uint32_t g_pma_bsp_wrap_cpg_div2_id_tbl_v4h[(R_PMA_MNG_CLK_DIVIDER-1)];
extern const uint32_t g_pma_bsp_wrap_cpg_div3_id_tbl_v4h[(R_PMA_MNG_CLK_DIVIDER-1)];
extern const uint32_t g_pma_bsp_wrap_cpg_div4_id_tbl_v4h[(R_PMA_MNG_CLK_DIVIDER-1)];

/* PRQA S 3684 2 # Do not specify because the size of the object to be exposed to the user might change.
                   Has a terminating symbol at the end of an object because it is not referenced beyond the size of the object. */
extern const uint32_t g_pma_bsp_wrap_sysc_id_tbl_s4[];
extern const uint32_t g_pma_bsp_wrap_cpg_id_tbl_s4[];
extern const uint32_t g_pma_bsp_wrap_post_clk_id_tbl_s4[R_PMA_MNG_CLK_POSTCKCR];
extern const uint32_t g_pma_bsp_wrap_cpg_div0_id_tbl_s4[(R_PMA_MNG_CLK_DIVIDER-1)];
extern const uint32_t g_pma_bsp_wrap_cpg_div2_id_tbl_s4[(R_PMA_MNG_CLK_DIVIDER-1)];
extern const uint32_t g_pma_bsp_wrap_cpg_div3_id_tbl_s4[(R_PMA_MNG_CLK_DIVIDER-1)];
extern const uint32_t g_pma_bsp_wrap_cpg_div4_id_tbl_s4[(R_PMA_MNG_CLK_DIVIDER-1)];
/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
static e_result_t R_PMA_BSP_WRAP_Exchange_ErrCode(int32_t errNo);

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_Init()
* XOS3_OSAL_NOS_CD_CD_POW_164
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_Init(uint64_t pma_soc_type)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;
    const char cpg_path[]  = "/dev/cpg";
    const char sysc_path[] = "/dev/sysc";

    if (R_PMA_DEVICE_SOC_TYPE_V4H == pma_soc_type)
    {
        cpg_idmax  = R_PMA_CPG_DOMAIN_ID_V4H_NUM;
        sysc_idmax = R_PMA_SYSC_DOMAIN_ID_V4H_NUM;
        g_pma_bsp_wrap_sysc_id_tbl     = g_pma_bsp_wrap_sysc_id_tbl_v4h;
        g_pma_bsp_wrap_cpg_id_tbl      = g_pma_bsp_wrap_cpg_id_tbl_v4h;
        g_pma_bsp_wrap_post_clk_id_tbl = g_pma_bsp_wrap_post_clk_id_tbl_v4h;
        g_pma_bsp_wrap_cpg_div0_id_tbl = g_pma_bsp_wrap_cpg_div0_id_tbl_v4h;
        g_pma_bsp_wrap_cpg_div2_id_tbl = g_pma_bsp_wrap_cpg_div2_id_tbl_v4h;
        g_pma_bsp_wrap_cpg_div3_id_tbl = g_pma_bsp_wrap_cpg_div3_id_tbl_v4h;
        g_pma_bsp_wrap_cpg_div4_id_tbl = g_pma_bsp_wrap_cpg_div4_id_tbl_v4h;
    }
    else if (R_PMA_DEVICE_SOC_TYPE_S4 == pma_soc_type)
    {
        cpg_idmax  = R_PMA_CPG_DOMAIN_ID_S4_NUM;
        sysc_idmax = R_PMA_SYSC_DOMAIN_ID_S4_NUM;
        g_pma_bsp_wrap_sysc_id_tbl     = g_pma_bsp_wrap_sysc_id_tbl_s4;
        g_pma_bsp_wrap_cpg_id_tbl      = g_pma_bsp_wrap_cpg_id_tbl_s4;
        g_pma_bsp_wrap_post_clk_id_tbl = g_pma_bsp_wrap_post_clk_id_tbl_s4;
        g_pma_bsp_wrap_cpg_div0_id_tbl = g_pma_bsp_wrap_cpg_div0_id_tbl_s4;
        g_pma_bsp_wrap_cpg_div2_id_tbl = g_pma_bsp_wrap_cpg_div2_id_tbl_s4;
        g_pma_bsp_wrap_cpg_div3_id_tbl = g_pma_bsp_wrap_cpg_div3_id_tbl_s4;
        g_pma_bsp_wrap_cpg_div4_id_tbl = g_pma_bsp_wrap_cpg_div4_id_tbl_s4;
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    if (R_PMA_OK == result)
    {
        drv_ret = sysc_initialize();
        if (0 <= drv_ret)
        {
            drv_ret = open(sysc_path, O_RDWR);
            if (0 <= drv_ret)
            {
                sysc_fd = drv_ret;
                drv_ret = 0;
            }
        }
        if (0 <= drv_ret)
        {
            drv_ret = cpgdrv_initialize();
            if (0 == drv_ret)
            {
                drv_ret = open(cpg_path, O_RDWR);
                if (0 <= drv_ret)
                {
                    cpg_fd  = drv_ret;
                    drv_ret = 0;
                }
                if (-1 == drv_ret)
                {
                    /** POSIX error code */
                    drv_ret = errno;
                }
            }
        }
        
        if (0 != drv_ret)
        {
            result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
        }
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_Quit()
* XOS3_OSAL_NOS_CD_CD_POW_165
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_Quit(void)
{
    e_result_t result = R_PMA_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int drv_ret;

    drv_ret = close(cpg_fd);
    if (0 == drv_ret)
    {
        drv_ret = cpgdrv_finalize();
        if (0 == drv_ret)
        {
            drv_ret = close(sysc_fd);
            if (0 == drv_ret)
            {
                drv_ret = sysc_finalize();
            }
            else
            {
                /** POSIX error code */
                drv_ret = errno;
            }
        }
    }
    
    if (0 != drv_ret)
    {
        result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_Quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetCpgON()
* XOS3_OSAL_NOS_CD_CD_POW_166
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetCpgON(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    ST_CPGDRV_FRQ_REQ req;
    
    if (cpg_idmax > modId)
    {
        req.i32_req_clk = (int32_t)g_pma_bsp_wrap_cpg_id_tbl[modId];
        
        drv_ret = ioctl(cpg_fd, CPGDRV_MSSR_ON, &req);
        if (-1 == drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetCpgON()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetCpgOFF()
* XOS3_OSAL_NOS_CD_CD_POW_167
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetCpgOFF(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    ST_CPGDRV_FRQ_REQ req;
    
    if (cpg_idmax > modId)
    {
        req.i32_req_clk = (int32_t)g_pma_bsp_wrap_cpg_id_tbl[modId];
        
        drv_ret = ioctl(cpg_fd, CPGDRV_MSSR_OFF, &req);
        if (-1 == drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetCpgOFF()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_GetCpgState()
* XOS3_OSAL_NOS_CD_CD_POW_168
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_GetCpgState(e_cpg_domain_id_t modId, e_pma_clock_onoff_t *modState)
{
    e_result_t result = R_PMA_BSP_ECPG;
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_GetCpgState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetSyscON()
* XOS3_OSAL_NOS_CD_CD_POW_169
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetSyscON(e_sysc_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    dcmd_pd_on_t      *set_req;
    dcmd_pd_sts_get_t *get_req;
    
    if (sysc_idmax > modId)
    {
        
        set_req->power_domain = g_pma_bsp_wrap_sysc_id_tbl[modId]; 
        //set_req->ret_func = X;
        drv_ret = ioctl(sysc_fd, SYSC_DCMD_PD_ON, set_req);
        if (0 == drv_ret)
        {
            get_req->pd_array[0] = g_pma_bsp_wrap_sysc_id_tbl[modId];
            drv_ret = ioctl(sysc_fd, SYSC_DCMD_PD_STATUS_GET, get_req);
            if (0 == drv_ret)
            {
                if (R_PMA_SYSC_POWER_ON != (e_pma_power_onoff_t)get_req->result)
                {
                    /** The change is not as expected */
                    result = R_PMA_NG;
                }
            }
            else
            {
                /** POSIX error code */
                drv_ret = errno;
            }
        }
        else
        {
            /** POSIX error code */
            drv_ret = errno;
        }
        
        if (R_PMA_OK = result)
        {
            result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetSyscON()
***********************************************************************************************************************/

/**********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetSyscOFF()
* XOS3_OSAL_NOS_CD_CD_POW_170
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetSyscOFF(e_sysc_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    dcmd_pd_off_t     *set_req;
    dcmd_pd_sts_get_t *get_req;
    
    if (sysc_idmax > modId)
    {
        
        set_req->power_domain = g_pma_bsp_wrap_sysc_id_tbl[modId];
        //set_req->ret_func = X;
        drv_ret = ioctl(sysc_fd, SYSC_DCMD_PD_OFF, set_req);
        if (0 == drv_ret)
        {
            get_req->pd_array[0] = g_pma_bsp_wrap_sysc_id_tbl[modId];
            drv_ret = ioctl(sysc_fd, SYSC_DCMD_PD_STATUS_GET, get_req);
            if (0 == drv_ret)
            {
                if (R_PMA_SYSC_POWER_OFF != (e_pma_power_onoff_t)get_req->result)
                {
                    /** The change is not as expected */
                    result = R_PMA_NG;
                }
            }
            else
            {
                /** POSIX error code */
                drv_ret = errno;
            }
        }
        else
        {
            /** POSIX error code */
            drv_ret = errno;
        }
        
        if (R_PMA_OK == result)
        {
            result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetSyscOFF()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_GetSyscState()
* XOS3_OSAL_NOS_CD_CD_POW_171
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_GetSyscState(e_sysc_domain_id_t modId, e_pma_power_onoff_t *modState)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    dcmd_pd_sts_get_t *get_req;
    
    if (sysc_idmax > modId)
    {
        get_req->pd_array[0] = g_pma_bsp_wrap_sysc_id_tbl[modId];
        drv_ret = ioctl(sysc_fd, SYSC_DCMD_PD_STATUS_GET, get_req);
        if (0 == drv_ret)
        {
            if (R_PMA_SYSC_POWER_OFF == (e_pma_power_onoff_t)get_req->result)
            {
                *modState = R_PMA_SYSC_POWER_OFF;
            }
            else if (R_PMA_SYSC_POWER_ON == (e_pma_power_onoff_t)get_req->result)
            {
                *modState = R_PMA_SYSC_POWER_ON;
            }
            else
            {
                /** BSP Failure */
                result = R_PMA_NG;
            }
        }
        else
        {
            /** POSIX error code */
            drv_ret = errno;
            result = R_PMA_BSP_WRAP_Exchange_ErrCode(drv_ret);
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_GetSyscState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_AssertReset()
* XOS3_OSAL_NOS_CD_CD_POW_172
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_AssertReset(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    ST_CPGDRV_FRQ_REQ req;
    
    if (cpg_idmax > modId)
    {
        req.i32_req_clk = (int32_t)g_pma_bsp_wrap_cpg_id_tbl[modId];
        drv_ret = ioctl(cpg_fd, CPGDRV_MSSR_SOFTRST_ON, &req);
        if (-1 == drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_AssertReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_DeassertReset()
* XOS3_OSAL_NOS_CD_CD_POW_173
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_DeassertReset(e_cpg_domain_id_t modId)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    ST_CPGDRV_FRQ_REQ req;
    
    if (cpg_idmax > modId)
    {
        req.i32_req_clk = (int32_t)g_pma_bsp_wrap_cpg_id_tbl[modId];
        drv_ret = ioctl(cpg_fd, CPGDRV_MSSR_SOFTRST_OFF, &req);
        if (-1 == drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_NG;
        }
    }
    else
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_DeassertReset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_SetPostDivider()
* XOS3_OSAL_NOS_CD_CD_POW_174
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_SetPostDivider(uint32_t chanIdx, uint32_t const * pClockDivider)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    ST_CPGDRV_FRQ_REQ req;
    req.i32_req_clk_on  = CPGDRV_ON;
    req.i32_req_clk     = (int32_t)g_pma_bsp_wrap_post_clk_id_tbl[chanIdx];
    
    switch(req.i32_req_clk)
    {
    case 0:
        req.i64_req_frq = (int64_t)g_pma_bsp_wrap_cpg_div0_id_tbl[*pClockDivider];
        break;
    case 2:
        req.i64_req_frq = (int64_t)g_pma_bsp_wrap_cpg_div2_id_tbl[*pClockDivider];
        break;
    case 3:
        req.i64_req_frq = (int64_t)g_pma_bsp_wrap_cpg_div3_id_tbl[*pClockDivider];
        break;
    case 4:
        req.i64_req_frq = (int64_t)g_pma_bsp_wrap_cpg_div4_id_tbl[*pClockDivider];
        break;
    default:
        result = R_PMA_INVALID_ARGUMENT;
        break;
    }
    
    if (R_PMA_OK == result)
    {
        if (R_PMA_BSP_WRAP_UNDEFINED_ID == req.i64_req_frq)
        {
            result = R_PMA_INVALID_ARGUMENT;
        }
        else
        {
            drv_ret = ioctl(cpg_fd, CPGDRV_FRQ_CLK_DIV, &req);
            if (-1 == drv_ret)
            {
                /** BSP Failure */
                result = R_PMA_NG;
            }
        }
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_SetPostDivider()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_EnablePostClock()
* XOS3_OSAL_NOS_CD_CD_POW_175
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_EnablePostClock(uint32_t chanIdx)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    ST_CPGDRV_FRQ_REQ req;
    
    
    req.i32_req_clk = (int32_t)g_pma_bsp_wrap_post_clk_id_tbl[chanIdx];
    if (R_PMA_BSP_WRAP_UNDEFINED_ID == req.i32_req_clk)
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    else
    {
        ret = ioctl(fd, CPGDRV_FRQ_CLK_ON, &req);
        if (-1 == drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_NG;
        }
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_EnablePostClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_DisablePostClock()
* XOS3_OSAL_NOS_CD_CD_POW_176
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
e_result_t R_PMA_BSP_WRAP_DisablePostClock(uint32_t chanIdx)
{
    e_result_t result = R_PMA_OK;
    int drv_ret;
    ST_CPGDRV_FRQ_REQ req;
    
    req.i32_req_clk = (int32_t)g_pma_bsp_wrap_post_clk_id_tbl[chanIdx];
    if (R_PMA_BSP_WRAP_UNDEFINED_ID == req.i32_req_clk)
    {
        result = R_PMA_INVALID_ARGUMENT;
    }
    else
    {
        ret = ioctl(fd, CPGDRV_FRQ_CLK_OFF, &req);
        if (-1 == drv_ret)
        {
            /** BSP Failure */
            result = R_PMA_NG;
        }
    }
    
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_DisablePostClock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_PMA_BSP_WRAP_Exchange_ErrCode()
* XOS3_OSAL_NOS_CD_CD_POW_177
* [Covers: XOS3_OSAL_NOS_UD_DD_POW_102]
***********************************************************************************************************************/
static e_result_t R_PMA_BSP_WRAP_Exchange_ErrCode(int32_t errNo)
{
    e_result_t result;
    int32_t liErr = errNo;

    if (0 > liErr)
    {
        liErr = -(liErr);
    }

    switch(liErr)
    {
    case 0:
        result = R_PMA_OK;
        break;
    case EINVAL:
        result = R_PMA_BSP_EINVAL;
        break;
    case ETIMEDOUT:
        result = R_PMA_BSP_ETIMEDOUT;
        break;
    case EIO:
        result = R_PMA_BSP_EIO;
        break;
    default:
        result = R_PMA_NG;
        break;
    }
    return result;
}
/***********************************************************************************************************************
* End of function R_PMA_BSP_WRAP_Exchange_ErrCode()
***********************************************************************************************************************/
