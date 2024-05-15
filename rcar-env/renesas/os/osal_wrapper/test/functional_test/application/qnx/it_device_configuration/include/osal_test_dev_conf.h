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
* File Name :    osal_test_dev_conf.h
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  main of check about OSAL Device Configuration
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           0.1.0     10.02.2023    Create New
 *           0.2.0     11.10.2023    Add definition for V4M
***********************************************************************************************************************/
#ifndef OSAL_TEST_DEV_CONF_H
#define OSAL_TEST_DEV_CONF_H

#define    OSAL_DEVICE_INVALID_ID    (NULL)

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcar-xos/osal/r_osal.h"
#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"
#if defined(RCAR_V4H)
#include "target/qnx/r_osal_os_dev_cfg_tbl_v4h.h"
#elif defined(RCAR_V4M)
#include "target/qnx/r_osal_os_dev_cfg_tbl_v4m.h"
#elif defined(RCAR_S4)
#include "target/qnx/r_osal_os_dev_cfg_tbl_s4.h"
#endif

#ifndef OSAL_CHECK_NO_LOGS
#define OSAL_CHECK_INFO(...) {printf("%s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#define OSAL_CHECK_ERR(...)  {printf("error: %s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#else
#define OSAL_SAMPLE_INFO(...) {}
#define OSAL_SAMPLE_ERR(...)  {}
#endif

int simple_dev_conf_test(void);

#endif  /* OSAL_TEST_H */
