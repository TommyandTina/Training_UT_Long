/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2017-2021 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : imp_demo.h
 * Version      : -
 * Product Name : impsample_dsp_link
 * Device(s)    : -
 * Description  : main.c's header
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History      : Version DD.MM.YYYY Description
 *              : v2.15.0 08.12.2021 New Created 
 *********************************************************************************************************************/
/*
=======================================================================================================================
 Includes   <System Includes> , "Project Includes"
=======================================================================================================================
*/
#include <stdio.h>
#include <string.h>
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/impfw/r_impfw.h"

/* Atomic Library */
#include "rcar-xos/atmlib/r_atmlib_prot.h"
#include "rcar-xos/atmlib/r_atmlib_types.h"

/*
=======================================================================================================================
 Public Macro definitions
=======================================================================================================================
*/
#ifndef IMPFWDEMO_H_
#define IMPFWDEMO_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* declare osal resorce id used by main.c */
#define    OSAL_MQ_IMP_FW_TOTASK                     (10ul)
#define    OSAL_MQ_IMP_FW_FROMTASK                   (11ul)
#define    OSAL_MUTEX_IMP_FW_STATE_LOCK              (12ul)
#define    OSAL_MUTEX_IMP_FW_ATTR_LOCK               (13ul)
#define    OSAL_MUTEX_IMP_FW_LOCK                    (14ul)
#define    OSAL_THREAD_IMP_FW                        (15ul)

#define    OSAL_MUTEX_IMP_DRV_LOCK                   (20ul)

#define    IMPDEMO_OK                                (0)
#define    IMPDEMO_NG                                (-1)

/*
=======================================================================================================================
 Public type definitions
=======================================================================================================================
*/
/* declare typedef */
typedef struct {
    osal_memory_buffer_handle_t    buffer;
    uint32_t                       buffersize;
    void                           * cpu_addr;
    uintptr_t                      hw_addr;
    uint32_t                       clsize;
} st_buffer_t;

typedef struct {
    uint32_t            loop_num;
    e_impfw_core_type_t core_type;
    uint32_t            core_num;
    uint32_t            pair_id;
    impfw_attr_handle_t attrhandle;
} st_usecore_info_t;

/*
=======================================================================================================================
 Public global variables
=======================================================================================================================
*/

extern osal_mq_id_t mq_id_for_callback[];
extern osal_mq_id_t mq_id_for_send_msg[];
extern osal_mq_id_t mq_id_for_receive_msg[]; 
extern osal_mutex_id_t mutex_id_for_core_state[];
extern osal_mutex_id_t mutex_id_for_attr_state[];
extern osal_mutex_id_t mutex_id_for_impfw_api[];
extern osal_mutex_id_t mutex_id_for_drv[];
extern osal_thread_id_t thread_id_for_app[];
extern osal_thread_id_t thread_id_for_impfw[];

/*
=======================================================================================================================
 Public global function
=======================================================================================================================
*/
/**********************************************************************************************************************
 * Function Name: fillmemory
 * Description  : fill memory
 * Arguments    : void * mem
 *                uint8_t data
 *                uint32_t size
 * Return Value : None
 *********************************************************************************************************************/
void fillmemory (void * mem, uint8_t data, uint32_t size);

/**********************************************************************************************************************
 * Function Name: outputmemory
 * Description  : output memory
 * Arguments    : void * mem
 *                uint32_t xsize
 *                uint32_t ysize
 *                uint32_t bytepp
 * Return Value : None
 *********************************************************************************************************************/
void outputmemory (void * mem, uint32_t xsize, uint32_t ysize, uint32_t bytepp);

/**********************************************************************************************************************
 * Function Name: mmngr_open
 * Description  : init osal mmngr
 * Arguments    : None
 * Return Value : e_osal_return_t
 *********************************************************************************************************************/
e_osal_return_t mmngr_open (void);

/**********************************************************************************************************************
 * Function Name: mmngr_close
 * Description  : quit osal mmngr
 * Arguments    : None
 * Return Value : e_osal_return_t
 *********************************************************************************************************************/
e_osal_return_t mmngr_close (void);

/**********************************************************************************************************************
 * Function Name: buffer_alloc
 * Description  : allocate buffer
 * Arguments    : st_buffer_t * buffer
 *              : uint32_t size
 * Return Value : e_osal_return_t
 *********************************************************************************************************************/
e_osal_return_t buffer_alloc (st_buffer_t * buffer, uint32_t size);

/**********************************************************************************************************************
 * Function Name: buffer_free
 * Description  : free buffer
 * Arguments    : st_buffer_t * buffer
 * Return Value : e_osal_return_t
 *********************************************************************************************************************/
e_osal_return_t buffer_free (st_buffer_t * buffer);

/**********************************************************************************************************************
 * Function Name: buffer_sync
 * Description  : cache flush and invalidate to sync memory
 * Arguments    : st_buffer_t * buffer
 * Return Value : e_osal_return_t
 *********************************************************************************************************************/
e_osal_return_t buffer_sync (st_buffer_t * buffer);

/**********************************************************************************************************************
 * Function Name: imp_demo_impfw_init
 * Description  : initialize impfw_ctrl_handle
 * Arguments    : impfw_ctrl_handle_t * p_impfwctl
 * Return Value : e_impfw_api_retcode_t
 *********************************************************************************************************************/
e_impfw_api_retcode_t imp_demo_impfw_init(impfw_ctrl_handle_t * p_impfwctl);

/**********************************************************************************************************************
 * Function Name: imp_demo_impfw_execute
 * Description  : initialize impfw_ctrl_handle
 * Arguments    : impfw_ctrl_handle_t p_impfwctl
 *              : st_impfw_core_info_t *p_core
 *              : impfw_attr_handle_t impfw_attr
 * Return Value : e_impfw_api_retcode_t
 *********************************************************************************************************************/
e_impfw_api_retcode_t imp_demo_impfw_execute(impfw_ctrl_handle_t impfwctl, st_impfw_core_info_t *p_core, impfw_attr_handle_t impfw_attr);

/**********************************************************************************************************************
 * Function Name: imp_demo_impfw_waitcallback
 * Description  : wait all callback during 5sec.
 * Arguments    : None
 * Return Value : e_impfw_api_retcode_t
 *********************************************************************************************************************/
e_osal_return_t imp_demo_impfw_waitcallback(void);

/**********************************************************************************************************************
 * Function Name: create_cl
 * Description  : Create CL for each Core.
 * Arguments    : st_usecore_info_t usecore_info
 * Return Value : R_ATMLIB_RETURN_VALUE
 *********************************************************************************************************************/
R_ATMLIB_RETURN_VALUE create_cl(st_impfw_core_info_t core_info, st_buffer_t cl_buffer);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* IMPFWDEMO_H_ */

/*
=======================================================================================================================
 End of file
=======================================================================================================================
*/
