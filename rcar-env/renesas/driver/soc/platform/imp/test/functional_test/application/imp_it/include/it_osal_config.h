#ifndef __IT_OSAL_CONFIG_H__
#define __IT_OSAL_CONFIG_H__

/* Config OSAL resource for test environment */
#define RESORCE_ID_IMP      0x5000U

/* OSAL message queue resource IDs */
#define IT_OSAL_MQ_ID_CHK_STAT		(1000U)
#define IT_OSAL_MQ_ID_APPEND		(RESORCE_ID_IMP   + 13U)
#define IT_OSAL_MQ_ID_CBFUNC		(RESORCE_ID_IMP   + 13U)
#define IT_OSAL_MQ_ID_CORE_EXEC1	(RESORCE_ID_IMP   + 13U)
#define IT_OSAL_MQ_ID_CORE_EXEC2	(RESORCE_ID_IMP   + 14U)
#define IT_OSAL_MQ_ID_CORE_EXEC3	(RESORCE_ID_IMP   + 15U)
#define IT_OSAL_MQ_ID_CORE_EXEC4	(RESORCE_ID_IMP   + 16U)
#define IT_OSAL_MQ_ID_CORE_EXEC5	(RESORCE_ID_IMP   + 17U)
#define IT_OSAL_MQ_ID_CORE_EXEC6	(RESORCE_ID_IMP   + 18U)
#define IT_OSAL_MQ_ID_EXEC_CB		(RESORCE_ID_IMP   + 19U)
#define IT_OSAL_MQ_ID_SET_IRQ_GROUP (RESORCE_ID_IMP   + 19U)
#define IT_OSAL_MQ_ID_RESUME_CL		(RESORCE_ID_IMP   + 19U)
#define IT_OSAL_MQ_ID_RTT_APPEND	(RESORCE_ID_IMP   + 19U)
#define IT_OSAL_MQ_ID_RTT_TRANS		(RESORCE_ID_IMP   + 19U)

#define IT_OSAL_THREAD_IMPDRV_APP_TASK00              (RESORCE_ID_IMP   +  9U)
#define IT_OSAL_THREAD_IMPDRV_APP_TASK01              (RESORCE_ID_IMP   + 10U)
#define IT_OSAL_THREAD_IMPDRV_APP_TASK02              (RESORCE_ID_IMP   + 11U)
#define IT_OSAL_THREAD_IMPDRV_APP_TASK03              (RESORCE_ID_IMP   + 12U)
#define IT_OSAL_THREAD_IMPDRV_APP_TASK04              (RESORCE_ID_IMP   + 13U)
#define IT_OSAL_THREAD_IMPDRV_APP_TASK05              (RESORCE_ID_IMP   + 14U)
#define IT_OSAL_THREAD_IMPDRV_APP_TASK06              (RESORCE_ID_IMP   + 15U)

#define OSAL_MUTEX_IMP_DRV_LOCK                   (RESORCE_ID_IMP   +  2U)
#define OSAL_MQ_IMP_RTT_DRVCB_00                  (RESORCE_ID_IMP   +  2U)

#endif
