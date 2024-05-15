/******************************************************************************
     IMP Sample Code (How to use IMP Framework with IMP Driver)
     Copyright (c) 2017-2019 Renesas Electronics Corporation.

    [File] impDemo.h
******************************************************************************/
#ifndef IMPFWDEMO_H_
#define IMPFWDEMO_H_

#include <stdio.h>
#include <string.h>

//#include "rcv_memdrv.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* sub */
void FillMemory( void *mem, uint8_t data, uint32_t size );
void FillMemory_demo( void *mem, uint8_t *data, uint32_t size);
void OutputMemory( void *mem, uint32_t xsize, uint32_t ysize, uint32_t bytepp );
void Output_check( void *mem, uint32_t *data, uint32_t size, uint32_t bytepp );
int ImpDemo_test(uint8_t test_case);
	
#define    OSAL_THREAD_IMP_RTT_DRV                   (26U)
#define    OSAL_THREAD_IMP_DRV                       (27U)

#define    OSAL_MUTEX_IMP_FW_LOCK                    (41U)
#define    OSAL_MUTEX_IMP_RTT_LOCK                   (42U)
#define    OSAL_MUTEX_IMP_DRV_LOCK                   (43U)
#define    OSAL_MQ_IMP_RTT_TASK_REQ                  (86U)
#define    OSAL_MQ_IMP_RTT_TASK_ACK                  (87U)
#define    OSAL_MQ_IMP_RTT_DRVCB_00                  (88U)
#define    OSAL_MQ_IMP_RTT_EXECCB                    (89U)
#define    OSAL_MQ_IMP_DRV_TASK_CBFUNC               (90U)
#define    OSAL_MQ_IMP_DRV_TASK_DELTASK              (91U)

#ifdef __cplusplus
}
#endif
#endif /* IMPFWDEMO_H_ */

