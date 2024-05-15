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

/* prototype */
void osal_test_result_output(int parent, int api_id, int test_id, int result, char *param);
uint64_t osal_test_get_expectdata(int parent, int api_id, int test_id);
char *osal_ret_to_str(e_osal_return_t ret_val);
e_osal_return_t io_open(char *device_type, int device_number, osal_device_handle_t *dev_handle);

#define TIME_DEBUG(...) {printf(__VA_ARGS__);}
#define sec_to_nsec (1000000000)
#define TIME_MEASURE_INIT struct timespec abstime_before, abstime_after; abstime_before.tv_sec = 0; abstime_before.tv_nsec = 0; abstime_after.tv_sec = 0; abstime_after.tv_nsec = 0;
#define TIME_MEASURE_START clock_gettime(CLOCK_REALTIME, &abstime_before);
#define TIME_MEASURE_STOP clock_gettime(CLOCK_REALTIME, &abstime_after);  uint64_t t = (abstime_after.tv_sec * sec_to_nsec + abstime_after.tv_nsec) - (abstime_before.tv_sec * sec_to_nsec + abstime_before.tv_nsec); TIME_DEBUG("Execution time: %ld ns\n", t);

#endif
