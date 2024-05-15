/**
 * @file osal_test_common.h
 * @brief OSAL test application common module header file.
 * @author K.Takagi
 * @date 2019/11/12
 */

#ifndef OSAL_TEST_COMMON_H
#define OSAL_TEST_COMMON_H

/* define */
#define OSAL_TEST_OPT_INFO_BUFFER	(150)
#define OSAL_TEST_RET_NUM			(15)
#define OSAL_TEST_EXPECT_STR_BUF	(100)

#define TASKNOTIFY_pdFAIL          (1)
#define TASKNOTIFYWAIT_pdFAIL      (1)
#define TASKCREATE_MEMORY          (1)
#define SEMAPHORETAKE_pdFAIL       (1)
#define SEMAPHOREGIVE_pdFAIL       (1)
#define QUEUERESET_pdFAIL          (1)
#define QUEUEAVAILABLE_pdFAIL      (1)
#define drvISR_FAIL                (1)
#define drvISR_BUS_INTERFACE_FAULT (2)
#define R_PMA_NG                   (1)
#define R_PMA_BUS_IF_CHK_NG        (2)
#define R_PMA_RESET_CHK_NG         (3)
#define R_PMA_RESET_TOGGLE_CHK_NG  (4)
#define R_OSAL_ERR_NULL_CONTROL    (1)
#define R_OSAL_ERR_OTHER_CONTROL   (2)
#define R_OSAL_ERR_INFO_DEVICE     (3)

typedef struct
{
    uint32_t TaskNotify;
    uint32_t TaskNotifyWait;
    uint32_t TaskCreate;
    uint32_t SemaphoreTake;
    uint32_t SemaphoreGive;
    uint32_t QueueReset;
    uint32_t QueueAvailable;
    uint32_t drvISR_SetupSPI;
    uint32_t drvISR_RemoveSPI;
    uint32_t drvISR_EnableSPI;
    uint32_t drvISR_DisableSPI;
    uint32_t R_PMA_RequestHwa;
    uint32_t R_PMA_ReleaseHwa;
    uint32_t R_PMA_GetHwaState;
    uint32_t R_PMA_GetReset;
    uint32_t R_PMA_SetPowerPolicy;
    uint32_t R_PMA_GetPowerPolicy;
    uint32_t R_PMA_ApplyResetHwa;
    uint32_t R_PMA_ReleaseResetHwa;
    uint32_t R_PMA_Init;
    uint32_t R_PMA_Quit;
    uint32_t R_PMA_UnlockHwa;
    uint32_t R_OSAL_RCFG_GetNumOfThread;
    uint32_t R_OSAL_RCFG_GetThreadInfo;
    uint32_t R_OSAL_RCFG_GetMutexInfo;
    uint32_t R_OSAL_RCFG_GetCondInfo;
    uint32_t R_OSAL_RCFG_GetMqInfo;
    uint32_t R_OSAL_DCFG_GetDeviceInfo;
    uint32_t R_OSAL_RCFG_GetInterruptThreadInfo;
}st_stub;

extern st_stub g_stub_flag;

/* prototype */
void osal_test_result_output(int parent, int api_id, int test_id, int result, char *param);
uint64_t osal_test_get_expectdata(OSAL_TEST_PARENT_ID parent, OSAL_TEST_API_ID api_id, OSAL_TEST_ID test_id);
char *osal_policy_ret_to_str(e_osal_pm_policy_t ret_val);

#ifdef TARGET_LINUX
void osal_test_initialize_serialport();
void osal_test_get_testid(char *test_number, unsigned int input_count);
#endif

e_osal_return_t osal_init(void);
e_osal_return_t osal_deinit(void);
e_osal_return_t interrupt_generation(void);
char *osal_ret_to_str(e_osal_return_t ret_val);
char *osal_reset_state_ret_to_str(e_osal_pm_reset_state_t ret_val);

//#if TIME_DEBUG_ON
//#define TIME_DEBUG(...) {printf(__VA_ARGS__);}
//#else
//#define TIME_DEBUG(...)
//#endif

#define TIME_DEBUG(...) {printf(__VA_ARGS__);}
#define sec_to_nsec (1000000000)
#define TIME_MEASURE_INIT struct timespec abstime_before, abstime_after; abstime_before.tv_sec = 0; abstime_before.tv_nsec = 0; abstime_after.tv_sec = 0; abstime_after.tv_nsec = 0;
#define TIME_MEASURE_START clock_gettime(CLOCK_REALTIME, &abstime_before);
#define TIME_MEASURE_STOP clock_gettime(CLOCK_REALTIME, &abstime_after);  int64_t t = (abstime_after.tv_sec * sec_to_nsec + abstime_after.tv_nsec) - (abstime_before.tv_sec * sec_to_nsec + abstime_before.tv_nsec); TIME_DEBUG("Execution time: %ld ns\n", t);

#if IT_DEBUG_ON
#define IT_DEBUG(...) {printf("%s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#else
#define IT_DEBUG(...)
#endif

#if 0
#define    OSAL_THREAD_ICCOM_MGR_TASK                (0x1000U)
#define    OSAL_THREAD_ICCOM_TIMER_TASK              (1U)
#define    OSAL_THREAD_ICCOM_RCV_TASK0               (2U)
#define    OSAL_THREAD_ICCOM_RCV_TASK1               (3U)
#define    OSAL_THREAD_ICCOM_RCV_TASK2               (4U)
#define    OSAL_THREAD_ICCOM_RCV_TASK3               (5U)
#define    OSAL_THREAD_ICCOM_RCV_TASK4               (6U)
#define    OSAL_THREAD_ICCOM_RCV_TASK5               (7U)
#define    OSAL_THREAD_ICCOM_RCV_TASK6               (8U)
#define    OSAL_THREAD_ICCOM_RCV_TASK7               (9U)
#define    OSAL_THREAD_ICCOM_READ_TASK0              (10U)
#define    OSAL_THREAD_ICCOM_READ_TASK1              (11U)
#define    OSAL_THREAD_ICCOM_READ_TASK2              (12U)
#define    OSAL_THREAD_ICCOM_READ_TASK3              (13U)
#define    OSAL_THREAD_ICCOM_READ_TASK4              (14U)
#define    OSAL_THREAD_ICCOM_READ_TASK5              (15U)
#define    OSAL_THREAD_ICCOM_READ_TASK6              (16U)
#define    OSAL_THREAD_ICCOM_READ_TASK7              (17U)
#define    OSAL_THREAD_ICCOM_SEND_TASK0              (18U)
#define    OSAL_THREAD_ICCOM_SEND_TASK1              (19U)
#define    OSAL_THREAD_ICCOM_SEND_TASK2              (20U)
#define    OSAL_THREAD_ICCOM_SEND_TASK3              (21U)
#define    OSAL_THREAD_ICCOM_SEND_TASK4              (22U)
#define    OSAL_THREAD_ICCOM_SEND_TASK5              (23U)
#define    OSAL_THREAD_ICCOM_SEND_TASK6              (24U)
#define    OSAL_THREAD_ICCOM_SEND_TASK7              (25U)
#define    OSAL_THREAD_IMP_RTT_DRV                   (26U)
#define    OSAL_THREAD_IMP_DRV                       (27U)

#define    OSAL_THREAD_RESERVE_00                    (1000U)
#define    OSAL_THREAD_RESERVE_01                    (1001U)
#define    OSAL_THREAD_RESERVE_02                    (1002U)
#define    OSAL_THREAD_RESERVE_03                    (1003U)
#define    OSAL_THREAD_RESERVE_04                    (1004U)
#define    OSAL_THREAD_RESERVE_05                    (1005U)
#define    OSAL_THREAD_RESERVE_06                    (1006U)
#define    OSAL_THREAD_RESERVE_07                    (1007U)
#define    OSAL_THREAD_RESERVE_08                    (1008U)
#define    OSAL_THREAD_RESERVE_09                    (1009U)

#define    OSAL_THREAD_RESOURCE_NUM                  (38U)

/**@} Thread_IDs*/

/**
 * @addtogroup Threadsync_mutex_IDs
 * @{
 */
#define    OSAL_MUTEX_IMR_00                         (0U)
#define    OSAL_MUTEX_IMR_01                         (1U)
#define    OSAL_MUTEX_IMR_02                         (2U)
#define    OSAL_MUTEX_IMR_03                         (3U)
#define    OSAL_MUTEX_IMR_04                         (4U)
#define    OSAL_MUTEX_IMR_05                         (5U)
#define    OSAL_MUTEX_DOF0_00                        (6U)
#define    OSAL_MUTEX_DOF0_01                        (7U)
#define    OSAL_MUTEX_STV0_00                        (8U)
#define    OSAL_MUTEX_STV0_01                        (9U)
#define    OSAL_MUTEX_ACF0_00                        (10U)
#define    OSAL_MUTEX_ACF0_01                        (11U)
#define    OSAL_MUTEX_ACF1_00                        (12U)
#define    OSAL_MUTEX_ACF1_01                        (13U)
#define    OSAL_MUTEX_ACF2_00                        (14U)
#define    OSAL_MUTEX_ACF2_01                        (15U)
#define    OSAL_MUTEX_ACF3_00                        (16U)
#define    OSAL_MUTEX_ACF3_01                        (17U)
#define    OSAL_MUTEX_ACF4_00                        (18U)
#define    OSAL_MUTEX_ACF4_01                        (19U)
#define    OSAL_MUTEX_VIPRTT_00                      (20U)
#define    OSAL_MUTEX_VIPRTT_01                      (21U)
#define    OSAL_MUTEX_VIPRTT_02                      (22U)
#define    OSAL_MUTEX_VIPRTT_03                      (23U)
#define    OSAL_MUTEX_ICCOM_WRITE_00                 (24U)
#define    OSAL_MUTEX_ICCOM_WRITE_01                 (25U)
#define    OSAL_MUTEX_ICCOM_WRITE_02                 (26U)
#define    OSAL_MUTEX_ICCOM_WRITE_03                 (27U)
#define    OSAL_MUTEX_ICCOM_WRITE_04                 (28U)
#define    OSAL_MUTEX_ICCOM_WRITE_05                 (29U)
#define    OSAL_MUTEX_ICCOM_WRITE_06                 (30U)
#define    OSAL_MUTEX_ICCOM_WRITE_07                 (31U)
#define    OSAL_MUTEX_ICCOM_READ_00                  (32U)
#define    OSAL_MUTEX_ICCOM_READ_01                  (33U)
#define    OSAL_MUTEX_ICCOM_READ_02                  (34U)
#define    OSAL_MUTEX_ICCOM_READ_03                  (35U)
#define    OSAL_MUTEX_ICCOM_READ_04                  (36U)
#define    OSAL_MUTEX_ICCOM_READ_05                  (37U)
#define    OSAL_MUTEX_ICCOM_READ_06                  (38U)
#define    OSAL_MUTEX_ICCOM_READ_07                  (39U)
#define    OSAL_MUTEX_ICCOM_TIMER                    (40U)
#define    OSAL_MUTEX_IMP_FW_LOCK                    (41U)
#define    OSAL_MUTEX_IMP_RTT_LOCK                   (42U)
#define    OSAL_MUTEX_IMP_DRV_LOCK                   (43U)
#define    OSAL_MUTEX_ENCODER_00                     (44U)
#define    OSAL_MUTEX_ENCODER_01                     (45U)
#define    OSAL_MUTEX_ENCODER_02                     (46U)
#define    OSAL_MUTEX_CISP_00                        (47U)
#define    OSAL_MUTEX_CISP_01                        (48U)
#define    OSAL_MUTEX_TISP_00                        (49U)
#define    OSAL_MUTEX_TISP_01                        (50U)
#define    OSAL_MUTEX_ENCODER_APP_00                 (51U)
#define    OSAL_MUTEX_ENCODER_APP_01                 (52U)
#define    OSAL_MUTEX_ICCOM_OPEN                     (53U)

#define    OSAL_MUTEX_RESERVE_00                     (1000U)
#define    OSAL_MUTEX_RESERVE_01                     (1001U)
#define    OSAL_MUTEX_RESERVE_02                     (1002U)
#define    OSAL_MUTEX_RESERVE_03                     (1003U)
#define    OSAL_MUTEX_RESERVE_04                     (1004U)
#define    OSAL_MUTEX_RESERVE_05                     (1005U)
#define    OSAL_MUTEX_RESERVE_06                     (1006U)
#define    OSAL_MUTEX_RESERVE_07                     (1007U)
#define    OSAL_MUTEX_RESERVE_08                     (1008U)
#define    OSAL_MUTEX_RESERVE_09                     (1009U)

#define    OSAL_MUTEX_RESOURCE_NUM                   (64U)

/**@} Threadsync_mutex__IDs*/

/**
 * @addtogroup Threadsync_cond_IDs
 * @{
 */
#define    OSAL_COND_RESERVE_00                      (1000U)
#define    OSAL_COND_RESERVE_01                      (1001U)
#define    OSAL_COND_RESERVE_02                      (1002U)
#define    OSAL_COND_RESERVE_03                      (1003U)
#define    OSAL_COND_RESERVE_04                      (1004U)
#define    OSAL_COND_RESERVE_05                      (1005U)
#define    OSAL_COND_RESERVE_06                      (1006U)
#define    OSAL_COND_RESERVE_07                      (1007U)
#define    OSAL_COND_RESERVE_08                      (1008U)
#define    OSAL_COND_RESERVE_09                      (1009U)

#define    OSAL_COND_RESOURCE_NUM                    (40U)

/**@} Threadsync_cond_IDs*/

/**
 * @addtogroup Message_Queue_IDs
 * @{
 */
#define    OSAL_MQ_IMR_MSG_00                        (0U)
#define    OSAL_MQ_IMR_MSG_01                        (1U)
#define    OSAL_MQ_IMR_MSG_02                        (2U)
#define    OSAL_MQ_IMR_MSG_03                        (3U)
#define    OSAL_MQ_IMR_MSG_04                        (4U)
#define    OSAL_MQ_IMR_MSG_05                        (5U)
#define    OSAL_MQ_IMR_RTT_MSG_00                    (6U)
#define    OSAL_MQ_IMR_RTT_MSG_01                    (7U)
#define    OSAL_MQ_IMR_RTT_MSG_02                    (8U)
#define    OSAL_MQ_IMR_RTT_MSG_03                    (9U)
#define    OSAL_MQ_IMR_RTT_MSG_04                    (10U)
#define    OSAL_MQ_IMR_RTT_MSG_05                    (11U)
#define    OSAL_MQ_ICCOM_MGR_TASK                    (12U)
#define    OSAL_MQ_ICCOM_REQ_OPEN                    (13U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_00          (14U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_01          (15U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_02          (16U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_03          (17U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_04          (18U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_05          (19U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_06          (20U)
#define    OSAL_MQ_ICCOM_SEND_DATA_UPPER_07          (21U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_00         (22U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_01         (23U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_02         (24U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_03         (25U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_04         (26U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_05         (27U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_06         (28U)
#define    OSAL_MQ_ICCOM_SEND_DATA_BOTTOM_07         (29U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_00             (30U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_01             (31U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_02             (32U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_03             (33U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_04             (34U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_05             (35U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_06             (36U)
#define    OSAL_MQ_ICCOM_RECEIVE_TASK_07             (37U)
#define    OSAL_MQ_ICCOM_SEND_TASK_00                (38U)
#define    OSAL_MQ_ICCOM_SEND_TASK_01                (39U)
#define    OSAL_MQ_ICCOM_SEND_TASK_02                (40U)
#define    OSAL_MQ_ICCOM_SEND_TASK_03                (41U)
#define    OSAL_MQ_ICCOM_SEND_TASK_04                (42U)
#define    OSAL_MQ_ICCOM_SEND_TASK_05                (43U)
#define    OSAL_MQ_ICCOM_SEND_TASK_06                (44U)
#define    OSAL_MQ_ICCOM_SEND_TASK_07                (45U)
#define    OSAL_MQ_ICCOM_READ_TASK_00                (46U)
#define    OSAL_MQ_ICCOM_READ_TASK_01                (47U)
#define    OSAL_MQ_ICCOM_READ_TASK_02                (48U)
#define    OSAL_MQ_ICCOM_READ_TASK_03                (49U)
#define    OSAL_MQ_ICCOM_READ_TASK_04                (50U)
#define    OSAL_MQ_ICCOM_READ_TASK_05                (51U)
#define    OSAL_MQ_ICCOM_READ_TASK_06                (52U)
#define    OSAL_MQ_ICCOM_READ_TASK_07                (53U)
#define    OSAL_MQ_ICCOM_SEND_DATA_00                (54U)
#define    OSAL_MQ_ICCOM_SEND_DATA_01                (55U)
#define    OSAL_MQ_ICCOM_SEND_DATA_02                (56U)
#define    OSAL_MQ_ICCOM_SEND_DATA_03                (57U)
#define    OSAL_MQ_ICCOM_SEND_DATA_04                (58U)
#define    OSAL_MQ_ICCOM_SEND_DATA_05                (59U)
#define    OSAL_MQ_ICCOM_SEND_DATA_06                (60U)
#define    OSAL_MQ_ICCOM_SEND_DATA_07                (61U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_00             (62U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_01             (63U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_02             (64U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_03             (65U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_04             (66U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_05             (67U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_06             (68U)
#define    OSAL_MQ_ICCOM_RECEIVE_DATA_07             (69U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_00       (70U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_01       (71U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_02       (72U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_03       (73U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_04       (74U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_05       (75U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_06       (76U)
#define    OSAL_MQ_ICCOM_EVENT_SEND_MSG_ACK_07       (77U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_00    (78U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_01    (79U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_02    (80U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_03    (81U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_04    (82U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_05    (83U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_06    (84U)
#define    OSAL_MQ_ICCOM_EVENT_RECEIVE_MSG_ACK_07    (85U)
#define    OSAL_MQ_IMP_RTT_TASK_REQ                  (86U)
#define    OSAL_MQ_IMP_RTT_TASK_ACK                  (87U)
#define    OSAL_MQ_IMP_RTT_DRVCB_00                  (88U)
#define    OSAL_MQ_IMP_RTT_EXECCB                    (89U)
#define    OSAL_MQ_IMP_DRV_TASK_CBFUNC               (90U)
#define    OSAL_MQ_IMP_DRV_TASK_DELTASK              (91U)

#define    OSAL_MQ_RESERVE_00                        (1000U)
#define    OSAL_MQ_RESERVE_01                        (1001U)
#define    OSAL_MQ_RESERVE_02                        (1002U)
#define    OSAL_MQ_RESERVE_03                        (1003U)
#define    OSAL_MQ_RESERVE_04                        (1004U)
#define    OSAL_MQ_RESERVE_05                        (1005U)
#define    OSAL_MQ_RESERVE_06                        (1006U)
#define    OSAL_MQ_RESERVE_07                        (1007U)
#define    OSAL_MQ_RESERVE_08                        (1008U)
#define    OSAL_MQ_RESERVE_09                        (1009U)

#define    OSAL_MQ_RESOURCE_NUM                      (102U)

#endif

#define    OSAL_THREAD_RESOURCE_NUM                  (0xfffeU)
#define    OSAL_MUTEX_RESOURCE_NUM                   (0xfffeU)
#define    OSAL_COND_RESOURCE_NUM                    (0xfffeU)
#define    OSAL_MQ_RESOURCE_NUM                      (0xfffeU)

#endif

