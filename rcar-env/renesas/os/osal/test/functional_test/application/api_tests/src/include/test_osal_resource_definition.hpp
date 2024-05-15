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
* File Name :    test_osal_resource_definition.hpp
* Version :      3.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header file used in OSAL API Functional Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     10.02.2022    Create new
 *           3.0.0     23.03.2022    Add new macro NEGATIVE_TIME
 *           3.1.0     04.04.2022    Add new macro SYNC_MQ_TIME
 *                                   Update description comment for TEST_MQ_COND_ID
 *                                   Add new macro SYNC_MQ_ID
 *           3.2.0     24.06.2022    Add new macro TEST_NUM_OF_MEMORY_REGION and TEST_MEMORY_REGION_DEFAULT
 *           3.3.0     28.07.2022    Add new macro for memory testing in multi-threads
 *           3.4.0     04.01.2022    Add new marco MUTEX_HANDLE_INITIALIZED_VALUE
 *                                   Add new marco COND_HANDLE_INITIALIZED_VALUE
 *                                   Add new marco THREAD_HANDLE_INITIALIZED_VALUE
 *                                   Add new marco MQ_HANDLE_INITIALIZED_VALUE
 *           3.5.0     10.04.2023    Add new marco TEST_MMNGR_NOT_OPEN_EXPECT
 *                                   Add new marco TEST_MEDIAN_EXPECT
 *           3.6.0     11.09.2023    Add definition of V4M
***********************************************************************************************************************/

/**
 * @page Test_Resource_Defined_value Defined OSAL resource value used in OSAL API Functional Test
 * @section Resource_Defined_value OSAL resource defined value
 * The defined OSAL resource value used in each test cases. These values can be changed.
 * @snippet test_osal_resource_definition.hpp OSAL_Resource_Defined_value_In_Test
 */
// [OSAL_Resource_Defined_value_In_Test]
#define THREAD_STACK_SIZE                   2*1024*1024 // 2MB
#define MAX_NSEC                            999999999
#define SEC_TO_NSEC                         1000000000
#define MSEC_TO_NSEC                        1000000
#define NEGATIVE_TIME                       -100
#define SYNC_MQ_TIME                        1000
#define ISR_MQ_MSG_CONTENT                  0x1111
#define ISR_MQ_NUM_OF_MSG                   1
#define ISR_MQ_MSG_SIZE                     4
#define ISR_MUTEX_ID                        0xf009U /* OSAL_MUTEX_OSAL_09 */
#define ISR_COND_ID                         0xf009U /* OSAL_COND_OSAL_09 */
#define ISR_MQ_ID                           0xf009U /* OSAL_MQ_OSAL_09 {1,4} */
#define INT_SYNC_MQ_ID                      0xf000U + 10U /* OSAL_MQ_OSAL_10 {1,4} */
#define ISR_THREAD_ID_NO1                   0xf000U /* OSAL_THREAD_OSAL_00   */
#define ISR_THREAD_ID_NO2                   0xf001U /* OSAL_THREAD_OSAL_01   */
#define ISR_THREAD_ID_NO3                   0xf002U /* OSAL_THREAD_OSAL_02   */
#define TEST_THREAD_ID1                     0xf000U /* OSAL_THREAD_OSAL_00   */
#define TEST_THREAD_ID2                     0xf001U /* OSAL_THREAD_OSAL_01   */
#define TEST_THREAD_ID3                     0xf002U /* OSAL_THREAD_OSAL_02   */
#define TEST_THREAD_ID4                     0xf003U /* OSAL_THREAD_OSAL_03   */
#define TEST_MUTEX_ID1                      0xf000U /* OSAL_MUTEX_OSAL_01    */
#define TEST_MUTEX_ID2                      0xf001U /* OSAL_MUTEX_OSAL_02    */
#define TEST_MUTEX_ID3                      0xf002U /* OSAL_MUTEX_OSAL_03    */
#define TEST_COND_ID1                       0xf000U /* OSAL_COND_OSAL_00     */
#define TEST_COND_ID2                       0xf001U /* OSAL_COND_OSAL_01     */
#define TEST_COND_ID3                       0xf002U /* OSAL_COND_OSAL_02     */
#define TEST_MQ_ID1                         0xf000U /* OSAL_MQ_OSAL_00 {2,4} */
#define TEST_MQ_ID2                         0xf001U /* OSAL_MQ_OSAL_01 {1,4} */
#define TEST_MQ_ID3                         0xf002U /* OSAL_MQ_OSAL_02 {1,4} */
#define TEST_MQ_COMMON_ID                   0xf003U /* OSAL_MQ_OSAL_03 {10,4} */
#define TEST_MQ_COND_ID                     0xf004U /* OSAL_MQ_OSAL_04 {1,4} */
#define SYNC_MQ_ID                          0xf003U /* OSAL_MQ_OSAL_03 {10, 4} */
#define TEST_MEMORY_REGION_DEFAULT          0U
#define MUTEX_HANDLE_INITIALIZED_VALUE      0xaa
#define COND_HANDLE_INITIALIZED_VALUE       0xaa
#define THREAD_HANDLE_INITIALIZED_VALUE     0xaa
#define MQ_HANDLE_INITIALIZED_VALUE         0xaa

#if defined(TEST_SIL)
#define TEST_NUM_OF_MEMORY_REGION           5U
#define TEST_REGION_IDX_OUT_RANGE_EXPECT    OSAL_RETURN_OK
#define TEST_MMNGR_NOT_OPEN_EXPECT          OSAL_RETURN_STATE
#else
#define TEST_NUM_OF_MEMORY_REGION           2U
#define TEST_REGION_IDX_OUT_RANGE_EXPECT    OSAL_RETURN_PAR
#define TEST_MMNGR_NOT_OPEN_EXPECT          OSAL_RETURN_HANDLE
#endif
#define TEST_NUM_OF_MULTI_BUFFERS           6U
#define TEST_MULTI_THREADS_BUFFER_SIZE      64U
#define TEST_MULTI_THREADS_BUFFER_ALIGN     64U
#define TEST_KNOWN_DATA                     0xAAU
#define TEST_SYNC_MQ_MSG                    0xFFFFU

#if defined(TEST_WINDOWS)
#define TEST_MEDIAN_EXPECT                  (21000000U + 100000U) // 21msec + 100usec
#elif defined(TEST_ZEPHYR)
#define TEST_MEDIAN_EXPECT                  (2000000U + 100000U) // 2msec + 100usec
#else
#define TEST_MEDIAN_EXPECT                  (1000000U + 100000U) // 1msec + 100usec
#endif
// [OSAL_Resource_Defined_value_In_Test]
