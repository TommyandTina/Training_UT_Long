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
* File Name :    osal_sample_memory_common.h
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header file of Sample application to use OSAL Memory API
***********************************************************************************************************************/
#ifndef OSAL_SAMPLE_DEVICE_COMMON_H_
#define OSAL_SAMPLE_DEVICE_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"

#ifndef OSAL_SAMPLE_NO_LOGS
#define OSAL_SAMPLE_INFO(...) {printf("%s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#define OSAL_SAMPLE_ERR(...)  {printf("error: %s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#else
#define OSAL_SAMPLE_INFO(...) {}
#define OSAL_SAMPLE_ERR(...)  {}
#endif

/* Alloc Size */
#define ALLOC_SIZE 64U /* 64 byte */

/* Alignment */
#define ALLOC_ALIGN 64

/* Number of allocate buffers */
#define NUM_OF_BUFFER 3
/* memory regions definition */
#define SAMPLE_NUM_OF_MEMORY_REGION 2U
#define SAMPLE_VALUE_REGION_INDEX_DEFAULT 0U

#endif /* End of File */
