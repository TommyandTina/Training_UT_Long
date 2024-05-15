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
* File Name :    r_osal_os_power.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL OS IFs for Power
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <linux/renesas_uioctl.h>
#include "target/linux/r_osal_os_types.h"
#include <stdio.h> /* must be include under the r_osal_api.h */

/***********************************************************************************************************************
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
***********************************************************************************************************************/
/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_PmSetPostClock() when the enable clock.
 * @param[in,out]    handle    A handle of opened device.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
 * @retval           OSAL_RETURN_UNSUPPORTED_OPERATION
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_pm_post_clock_enable(osal_device_handle_t const handle);

/***********************************************************************************************************************
 * @brief            Sub procedure of R_OSAL_OS_PmSetPostClock() when the disable clock.
 * @param[in,out]    handle    A handle of opened device.
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_pm_post_clock_disbale(osal_device_handle_t const handle);

/***********************************************************************************************************************
 * @brief            Get the HW value with ioctl().
 * @param[in]        device_fd    File descriptor.
 * @param[in]        req          Request code.
 * @param[in]        p_value      A pointer to the parameter of ioctl
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
/* PRQA S 5209 1 # Used as the parameter of ioctl. */
OSAL_STATIC e_osal_return_t r_osal_os_pm_get_ioctl(int device_fd, unsigned long int req, int* p_value);

/***********************************************************************************************************************
 * @brief            Set the HW value with ioctl().
 * @param[in]        device_fd    File descriptor.
 * @param[in]        req          Request code.
 * @param[in]        value        Parameter of ioctl
 * @return           #e_osal_return_t
 * @retval           OSAL_RETURN_OK
 * @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
/* PRQA S 5209 1 # Used as the parameter of ioctl. */
OSAL_STATIC e_osal_return_t r_osal_os_pm_set_ioctl(int device_fd, unsigned long int req, int value);

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_101
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_PmGetPowerState()
***********************************************************************************************************************/
/* PRQA S 5209 2 # Used as the parameter of ioctl. */
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_PmGetPowerState(osal_device_handle_t const handle, int* p_value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if ((NULL == handle) ||
        (NULL == p_value))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542 2 # UIO_PDRV_GET_PWR is used in accordance with the specification, verified OK. */
        /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_GET_PWR uses bit-shift, verified OK. */
        osal_ret = r_osal_os_pm_get_ioctl(handle->dev_fd, UIO_PDRV_GET_PWR, p_value);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_PmGetPowerState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_102
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_PmSetPowerState()
***********************************************************************************************************************/
/* PRQA S 5209 2 # Used as the parameter of ioctl. */
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_PmSetPowerState(osal_device_handle_t const handle, int value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542 2 # UIO_PDRV_SET_PWR is used in accordance with the specification, verified OK. */
        /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_SET_PWR uses bit-shift, verified OK. */
        osal_ret = r_osal_os_pm_set_ioctl(handle->dev_fd, UIO_PDRV_SET_PWR, value);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_PmSetPowerState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_103
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_PmGetClockState()
***********************************************************************************************************************/
/* PRQA S 5209 2 # Used as the parameter of ioctl. */
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_PmGetClockState(osal_device_handle_t const handle, int* p_value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if ((NULL == handle) ||
        (NULL == p_value))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542 2 # UIO_PDRV_GET_CLK is used in accordance with the specification, verified OK. */
        /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_GET_CLK uses bit-shift, verified OK. */
        osal_ret = r_osal_os_pm_get_ioctl(handle->dev_fd, UIO_PDRV_GET_CLK, p_value);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_PmGetClockState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_104
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_PmSetClockState()
***********************************************************************************************************************/
/* PRQA S 5209 2 # Used as the parameter of ioctl. */
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_PmSetClockState(osal_device_handle_t const handle, int value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542 2 # UIO_PDRV_SET_CLK is used in accordance with the specification, verified OK. */
        /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_SET_CLK uses bit-shift, verified OK. */
        osal_ret = r_osal_os_pm_set_ioctl(handle->dev_fd, UIO_PDRV_SET_CLK, value);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_PmSetClockState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_105
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_PmGetResetState()
***********************************************************************************************************************/
/* PRQA S 5209 2 # Used as the parameter of ioctl. */
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_PmGetResetState(osal_device_handle_t const handle, int* p_value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if ((NULL == handle) ||
        (NULL == p_value))
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542 2 # UIO_PDRV_GET_RESET is used in accordance with the specification, verified OK. */
        /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_GET_RESET uses bit-shift, verified OK. */
        osal_ret = r_osal_os_pm_get_ioctl(handle->dev_fd, UIO_PDRV_GET_RESET, p_value);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_PmGetResetState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_106
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_PmSetResetState()
***********************************************************************************************************************/
/* PRQA S 5209 2 # Used as the parameter of ioctl. */
/* PRQA S 3673 1 # Implemented in accordance with the requirement. */
e_osal_return_t R_OSAL_OS_PmSetResetState(osal_device_handle_t const handle, int value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 4542 2 # UIO_PDRV_SET_RESET is used in accordance with the specification, verified OK. */
        /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_SET_RESET uses bit-shift, verified OK. */
        osal_ret = r_osal_os_pm_set_ioctl(handle->dev_fd, UIO_PDRV_SET_RESET, value);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_PmSetResetState()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_026]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_107
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_OS_PmSetPostClock()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_OS_PmSetPostClock(osal_device_handle_t const handle, bool clock_enable, uint32_t clock_divider)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (NULL == handle)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((true == clock_enable) && (-1 == handle->dev_post_fd))
        {
            osal_ret = r_osal_os_pm_post_clock_enable(handle);
        }
        else if ((false == clock_enable) && (-1 != handle->dev_post_fd))
        {
            osal_ret = r_osal_os_pm_post_clock_disbale(handle);
        }
        else
        {
            /* Do nothing */
        }
    }

    if ((OSAL_RETURN_OK == osal_ret) && (true == clock_enable))
    {
        /* PRQA S 5209 3 # clock_divider is used as the parameter of ioctl. */
        /* PRQA S 4542 2 # UIO_PDRV_CLK_SET_DIV is used in accordance with the specification, verified OK. */
        /* PRQA S 4513,0499,4543,1841 1 # UIO_PDRV_CLK_SET_DIV uses bit-shift, verified OK. */
        osal_ret = r_osal_os_pm_set_ioctl(handle->dev_post_fd, UIO_PDRV_CLK_SET_DIV, (int)clock_divider);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_OS_PmSetPostClock()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_110
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_pm_post_clock_enable()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_pm_post_clock_enable(osal_device_handle_t const handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    char*           p_dev_post_id;

    /* PRQA S 0317 1 # Required cast on system, not modifiable. */
    p_dev_post_id = R_OSAL_OS_Malloc((size_t)(sizeof(char) * (size_t)PATH_MAX));
    if (NULL == p_dev_post_id)
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        linux_ret = snprintf(p_dev_post_id, PATH_MAX, "/dev/post_%s", handle->dev_id);
        if (linux_ret < 0)
        {
            osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* PRQA S 0339 1 # O_RDWR uses octal and is used in accordance with the specification of open(). */
        handle->dev_post_fd = R_OSAL_OS_Open(p_dev_post_id, O_RDWR);
        if (-1 == handle->dev_post_fd)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    /* PRQA S 0314 1 # The arguments must be (void *) but no side effects in this case. */
    R_OSAL_OS_Free((void *)p_dev_post_id);

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_pm_post_clock_enable()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_111
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_pm_post_clock_disbale()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_os_pm_post_clock_disbale(osal_device_handle_t const handle)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    linux_ret = flock(handle->dev_post_fd, LOCK_UN);
    if (0 != linux_ret)
    {
       osal_ret = OSAL_RETURN_FAIL;
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_OS_Close(handle->dev_post_fd);
        if (OSAL_RETURN_OK == osal_ret)
        {
            handle->dev_post_fd = -1;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_pm_post_clock_disbale()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_027]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_108
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_pm_get_ioctl()
***********************************************************************************************************************/
/* PRQA S 5209 1 # Used as the parameter of ioctl. */
OSAL_STATIC e_osal_return_t r_osal_os_pm_get_ioctl(int device_fd, unsigned long int req, int* p_value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;

    /* PRQA S 5209 4 # Used as the parameter of ioctl. */
    _Static_assert( (
        sizeof(*p_value)
        ) == (
        sizeof(int)
        ), "Structure must not have any padding");

    linux_ret = ioctl(device_fd, req, p_value);
    if (0 != linux_ret)
    {
        if (-EOPNOTSUPP == *p_value)
        {
            osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
        }
        else
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_pm_get_ioctl()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_028]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_LINUX_CD_CD_POW_109
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_os_pm_set_ioctl()
***********************************************************************************************************************/
/* PRQA S 5209 1 # Used as the parameter of ioctl. */
OSAL_STATIC e_osal_return_t r_osal_os_pm_set_ioctl(int device_fd, unsigned long int req, int value)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # Return values from linux must be int. Used according to the specifications. */
    int             linux_ret;
    /* PRQA S 5209 1 # Used as the parameter of ioctl. */
    int             local_value = value;

    /* PRQA S 5209 4 # Used as the parameter of ioctl. */
    _Static_assert( (
        sizeof(local_value)
        ) == (
        sizeof(int)
        ), "Structure must not have any padding");

    linux_ret = ioctl(device_fd, req, &local_value);
    if (0 == linux_ret)
    {
        /* Do nothing */
    }
    else if (-EOPNOTSUPP == linux_ret)
    {
        osal_ret = OSAL_RETURN_UNSUPPORTED_OPERATION;
    }
    else
    {
        osal_ret = OSAL_RETURN_FAIL;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_os_pm_set_ioctl()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_LINUX_UD_DD_POW_029]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */
