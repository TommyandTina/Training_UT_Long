/**
 * @file synchronization_manager.c
 * @brief OSAL Synchronization Manager evaluation file.
 * @author K.Takagi
 * @date 2019/11/12
 */

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcar-xos/osal/r_osal.h"
#include "osal_test.h"
#include "osal_test_common.h"
#include "synchronization_manager.h"
#include <time.h>

/* prototype */
static void osal_test_R_OSAL_ThsyncMutexCreate();
static void osal_test_R_OSAL_ThsyncMutexDestroy();
static void osal_test_R_OSAL_ThsyncMutexLockForTimePeriod();
static void osal_test_R_OSAL_ThsyncMutexLockUntilTimeStamp();
static void osal_test_R_OSAL_ThsyncMutexTryLock();
static void osal_test_R_OSAL_ThsyncMutexUnlock();
static void osal_test_R_OSAL_ThsyncCondCreate();
static void osal_test_R_OSAL_ThsyncCondDestroy();
static void osal_test_R_OSAL_ThsyncCondWaitForTimePeriod();
static void osal_test_R_OSAL_ThsyncCondWaitUntilTimeStamp();
static void osal_test_R_OSAL_ThsyncCondBroadcast();
static void osal_test_R_OSAL_ThsyncCondSignal();

typedef struct st_arg_cond_abwait {
	st_osal_time_t time;
	osal_cond_handle_t handle;
	osal_mutex_handle_t mutex_handle;
} st_arg_cond_abwait_t;

typedef struct st_arg_cond_waitperiod {
	osal_milli_sec_t time;
	osal_cond_handle_t handle;
	osal_mutex_handle_t mutex_handle;
} st_arg_cond_waitperiod_t;

typedef struct st_arg_mutex_waitperiod {
	osal_milli_sec_t time;
	osal_mutex_handle_t mutex_handle;
} st_arg_mutex_waitperiod_t;

volatile osal_mutex_handle_t g_mutexlock_handle = NULL;
volatile osal_cond_handle_t g_cond_handle = NULL;
volatile osal_mutex_handle_t g_mutex_common = NULL;
volatile osal_cond_handle_t g_cond_common = NULL;

void osal_test_synchronization_manager_display_menu()
{

	printf("*** OSAL Thread Synchronization Manager           ***\n");
	printf("***  02_001: R_OSAL_ThsyncMutexCreate             ***\n");
	printf("***  02_002: R_OSAL_ThsyncMutexDestroy            ***\n");
	printf("***  02_003: R_OSAL_ThsyncMutexLockForTimePeriod  ***\n");
	printf("***  02_004: R_OSAL_ThsyncMutexLockUntilTimeStamp ***\n");
	printf("***  02_005: R_OSAL_ThsyncMutexTryLock            ***\n");
	printf("***  02_006: R_OSAL_ThsyncMutexUnlock             ***\n");
	printf("***  02_007: R_OSAL_ThsyncCondCreate              ***\n");
	printf("***  02_008: R_OSAL_ThsyncCondDestroy             ***\n");
	printf("***  02_009: R_OSAL_ThsyncCondWaitForTimePeriod   ***\n");
	printf("***  02_010: R_OSAL_ThsyncCondWaitUntilTimeStamp  ***\n");
	printf("***  02_011: R_OSAL_ThsyncCondBroadcast           ***\n");
	printf("***  02_012: R_OSAL_ThsyncCondSignal              ***\n");
	printf("***  02_100: synchronization manager module all exec ***\n\n");
}

void osal_test_synchronization_manager_menu(int child)
{

	int test_number = child;
	unsigned int test_count  = 1;
	unsigned int loop_count  = 0;

	/* Reset stub flags */
	//memset(&g_stub_flag, 0, sizeof(st_stub));

	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number = OSAL_TEST_THREADSYNCMUTEXCREATE;
		test_count  = OSAL_TEST_THREADSYNC_MAX;
	}
	
	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number)
		{
			case OSAL_TEST_THREADSYNCMUTEXCREATE:
				osal_test_R_OSAL_ThsyncMutexCreate();
				break;
			case OSAL_TEST_THREADSYNCMUTEXDESTROY:
				osal_test_R_OSAL_ThsyncMutexDestroy();
				break;
			case OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD:
				osal_test_R_OSAL_ThsyncMutexLockForTimePeriod();
				break;
			case OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP:
				osal_test_R_OSAL_ThsyncMutexLockUntilTimeStamp();
				break;
			case OSAL_TEST_THREADSYNCMUTEXTRYLOCK:
				osal_test_R_OSAL_ThsyncMutexTryLock();
				break;
			case OSAL_TEST_THREADSYNCMUTEXUNLOCK:
				osal_test_R_OSAL_ThsyncMutexUnlock();
				break;
			case OSAL_TEST_THREADSYNCCONDCREATE:
				osal_test_R_OSAL_ThsyncCondCreate();
				break;
			case OSAL_TEST_THREADSYNCCONDDESTROY:
				osal_test_R_OSAL_ThsyncCondDestroy();
				break;
			case OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD:
				osal_test_R_OSAL_ThsyncCondWaitForTimePeriod();
				break;
			case OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP:
				osal_test_R_OSAL_ThsyncCondWaitUntilTimeStamp();
				break;
			case OSAL_TEST_THREADSYNCCONDBROADCAST:
				osal_test_R_OSAL_ThsyncCondBroadcast();
				break;
			case OSAL_TEST_THREADSYNCCONDSIGNAL:
				osal_test_R_OSAL_ThsyncCondSignal();
				break;
			default:
				printf("That menu is not available.\n\n");
				break;
		}
		test_number++;
	}
}

e_osal_return_t osal_init_synchronization(void)
{
	e_osal_return_t ret = OSAL_RETURN_OK;
	if(false == init_flag)
	{
		ret = R_OSAL_Initialize();
		if(ret == OSAL_RETURN_OK)
		{
			init_flag = true;
		}

	}

    return ret;
}

e_osal_return_t osal_deinit_synchronization(void)
{
	e_osal_return_t ret = OSAL_RETURN_OK;
	if(true == init_flag)
	{
		ret = R_OSAL_Deinitialize();
		if(ret == OSAL_RETURN_OK)
		{
			init_flag = false;
		}
	}

    return ret;
}

/* test thread. */
int64_t osal_test_mutex_lock(void *user_arg)
{
	osal_mutex_handle_t 	mutex_handle;
	e_osal_return_t      	osal_ret;
	st_arg_mutex_waitperiod_t *mutex_arg;
	
	mutex_arg = (st_arg_mutex_waitperiod_t *)user_arg;
	mutex_handle = mutex_arg->mutex_handle;

	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, mutex_arg->time);
	if (OSAL_RETURN_OK == osal_ret)
	{
		/* wait until the main task calls R_OSAL_ThreadJoin() */
		R_OSAL_ThreadSleepForTimePeriod(5000);
		osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);
		if (OSAL_RETURN_OK == osal_ret)
		{
			osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
		}
		else
		{
			osal_ret = OSAL_RETURN_FAIL;
		}
	}
	else
	{
		osal_ret = OSAL_RETURN_FAIL;
	}

	return (int64_t)osal_ret;
}

/* test thread. use 02_011_001 */
int64_t osal_test_cond_waitperiod(void *user_arg)
{
	osal_mutex_handle_t 		mutex_handle;
	osal_cond_handle_t  		cond_handle;
	e_osal_return_t      		osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	/* cond wait */
	arg_cond 	= (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexTryLock(mutex_handle);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret    = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, arg_cond->time);
	TIME_MEASURE_STOP

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(3000);

	return (int64_t)osal_ret;
}

/* test thread. */
int64_t osal_test_cond_waitperiod_mutex_null(void *user_arg)
{
	osal_cond_handle_t  		cond_handle;
	e_osal_return_t      		osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	cond_handle = arg_cond->handle;

	/* cond wait */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, NULL, arg_cond->time);
	TIME_MEASURE_STOP

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread. */
int64_t osal_test_cond_waitperiod_mutex_unlock(void *user_arg)
{
	osal_mutex_handle_t 		mutex_handle;
	osal_cond_handle_t  		cond_handle;
	e_osal_return_t      		osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	/* cond wait */
	arg_cond 	= (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* cond wait */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, arg_cond->time);
	TIME_MEASURE_STOP

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread. */
int64_t osal_test_cond_abwait(void *user_arg)
{
	osal_mutex_handle_t 	mutex_handle;
	osal_cond_handle_t  	cond_handle;
	e_osal_return_t      	osal_ret;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	*user_setting;

	user_setting = (st_arg_cond_abwait_t *)user_arg;
	absolute_time = user_setting->time;
	cond_handle = user_setting->handle;
	mutex_handle = user_setting->mutex_handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	
	/* cond wait */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, &absolute_time);
	TIME_MEASURE_STOP

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread. */
int64_t osal_test_cond_abwait_mutex_null(void *user_arg)
{
	osal_cond_handle_t  	cond_handle;
	e_osal_return_t      	osal_ret;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	*user_setting;

	user_setting = (st_arg_cond_abwait_t *)user_arg;
	absolute_time = user_setting->time;
	cond_handle = user_setting->handle;

	/* cond wait */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, NULL, &absolute_time);
	TIME_MEASURE_STOP

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread. */
int64_t osal_test_cond_abwait_mutex_unlock(void *user_arg)
{
	osal_mutex_handle_t 	mutex_handle;
	osal_cond_handle_t  	cond_handle;
	e_osal_return_t      	osal_ret;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	*user_setting;

	user_setting = (st_arg_cond_abwait_t *)user_arg;
	absolute_time = user_setting->time;
	cond_handle = user_setting->handle;
	mutex_handle = user_setting->mutex_handle;

	/* cond wait */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, &absolute_time);
	TIME_MEASURE_STOP

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread. */
int64_t osal_test_cond_abwait_time_null(void *user_arg)
{
	osal_mutex_handle_t 	mutex_handle;
	osal_cond_handle_t  	cond_handle;
	e_osal_return_t      	osal_ret;
	st_arg_cond_abwait_t	*user_setting;

	user_setting = (st_arg_cond_abwait_t *)user_arg;
	cond_handle = user_setting->handle;
	mutex_handle = user_setting->mutex_handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	
	/* cond wait */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, NULL);
	TIME_MEASURE_STOP

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread */
int64_t osal_test_cond_subthread_wakeup_period(void *user_arg)
{
	
	osal_cond_handle_t 			cond_handle;
	e_osal_return_t     		osal_ret;
	osal_mutex_handle_t 		mutex_handle;
	st_arg_cond_waitperiod_t	*arg_cond;

	/* cond signal */
	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	cond_handle = arg_cond->handle;
	mutex_handle = arg_cond->mutex_handle;

	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 1000);
	osal_ret    = R_OSAL_ThsyncCondSignal(cond_handle);
	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the another thread calls R_OSAL_ThsyncCondTimedWaitXXX(). */
	R_OSAL_ThreadSleepForTimePeriod(2000);

	return (int64_t)osal_ret;
}

int64_t osal_test_cond_subthread_wakeup_ab(void *user_arg)
{
	
	osal_cond_handle_t 			cond_handle;
	e_osal_return_t     		osal_ret;
	osal_mutex_handle_t 		mutex_handle;
	st_arg_cond_abwait_t		*arg_cond;

	/* cond signal */
	arg_cond = (st_arg_cond_abwait_t *)user_arg;
	cond_handle = arg_cond->handle;
	mutex_handle = arg_cond->mutex_handle;

	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 1000);
	osal_ret    = R_OSAL_ThsyncCondSignal(cond_handle);
	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the another thread calls R_OSAL_ThsyncCondTimedWaitXXX(). */
	R_OSAL_ThreadSleepForTimePeriod(2000);

	return (int64_t)osal_ret;
}

/* test thread. */
int64_t osal_test_cond_destroy_abwait(void *user_arg)
{
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
	osal_cond_handle_t  	cond_handle;
	e_osal_return_t      	osal_ret;
	st_osal_time_t        	absolute_time;

	memset(&mutex_handle,  0x00, sizeof(osal_mutex_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	mutex_id   = 2;

	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);

	absolute_time.tv_sec += 2;

	/* mutex lock */
	R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	
	/* cond wait */
	cond_handle = (osal_cond_handle_t)user_arg;
	osal_ret    = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, &absolute_time);

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread */
int64_t osal_test_cond_destroy(void *user_arg)
{
	osal_cond_handle_t  cond_handle;
	e_osal_return_t      osal_ret;

	/* wait until the another thread calls R_OSAL_ThsyncCondTimedWaitXXX(). */
	R_OSAL_ThreadSleepForTimePeriod(1500);

	cond_handle = (osal_cond_handle_t)user_arg;
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	TIME_MEASURE_STOP

	return (int64_t)osal_ret;
}

/* test thread. */
int64_t osal_test_condboardcast_wait(void *user_arg)
{
	osal_mutex_handle_t 		mutex_handle;
	osal_cond_handle_t  		cond_handle;
	e_osal_return_t      		osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	
	/* cond wait */
	osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, arg_cond->time);

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(2000);

	return (int64_t)osal_ret;
}

int64_t osal_test_condboardcast_multi_wait(void *user_arg)
{
	osal_mutex_handle_t 		mutex_handle;
	osal_cond_handle_t  		cond_handle;
	e_osal_return_t      		osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 1000);
	
	/* cond wait */
	osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, arg_cond->time);

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	
	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(1000);

	return (int64_t)osal_ret;
}

/* test thread */
int64_t osal_test_condboardcast_wakeup(void *user_arg)
{
	
	osal_cond_handle_t cond_handle;
	osal_mutex_handle_t 		mutex_handle;
	e_osal_return_t     osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10000);

	/* cond signal */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);
	TIME_MEASURE_STOP

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the another thread calls R_OSAL_ThsyncCondTimedWaitXXX(). */
	R_OSAL_ThreadSleepForTimePeriod(1000);


	return (int64_t)osal_ret;
}
int64_t osal_test_condboardcast_multiwakeup(void *user_arg)
{
	
	osal_cond_handle_t cond_handle;
	osal_mutex_handle_t 		mutex_handle;
	e_osal_return_t     osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;


	/* wait until the another thread calls R_OSAL_ThsyncCondTimedWaitXXX(). */
	R_OSAL_ThreadSleepForTimePeriod(3000);

	/* mutex lock */
	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10000);

	/* cond signal */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);
	TIME_MEASURE_STOP

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	return (int64_t)osal_ret;
}


#if 0
/* test thread */
int64_t osal_test_condboardcast_wakeup_stub(void *user_arg)
{
	
	osal_cond_handle_t cond_handle;
	osal_mutex_handle_t 		mutex_handle;
	e_osal_return_t     osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10000);

	/* cond signal */
	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	return (int64_t)osal_ret;
}
#endif

/* test thread. */
int64_t osal_test_condsignal_wait(void *user_arg)
{
	osal_mutex_handle_t 		mutex_handle;
	osal_cond_handle_t  		cond_handle;
	e_osal_return_t      		osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	
	/* cond wait */
	osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, arg_cond->time);

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the main task calls R_OSAL_ThreadJoin() */
	R_OSAL_ThreadSleepForTimePeriod(2000);
	return (int64_t)osal_ret;
}

/* test thread */
int64_t osal_test_condsignal_wakeup(void *user_arg)
{
	
	osal_cond_handle_t cond_handle;
	osal_mutex_handle_t 		mutex_handle;
	e_osal_return_t     osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10000);

	/* cond signal */
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);
	TIME_MEASURE_STOP

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	/* wait until the another thread calls R_OSAL_ThsyncCondTimedWaitXXX(). */
	R_OSAL_ThreadSleepForTimePeriod(1000);
	return (int64_t)osal_ret;
}

#if 0
/* test thread */
int64_t osal_test_condsignal_wakeup_stub(void *user_arg)
{
	
	osal_cond_handle_t cond_handle;
	osal_mutex_handle_t 		mutex_handle;
	e_osal_return_t     osal_ret;
	st_arg_cond_waitperiod_t	*arg_cond;

	arg_cond = (st_arg_cond_waitperiod_t *)user_arg;
	mutex_handle = arg_cond->mutex_handle;
	cond_handle = arg_cond->handle;

	/* mutex lock */
	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10000);

	/* cond signal */
	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;

	R_OSAL_ThsyncMutexUnlock(mutex_handle);


/* ----- */
	/* mutex lock */
	R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 10000);

	/* cond signal */
	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;
	
	R_OSAL_ThsyncMutexUnlock(mutex_handle);
/*  ------  */



	/* wait until the another thread calls R_OSAL_ThsyncCondTimedWaitXXX(). */
	//R_OSAL_ThreadSleepForTimePeriod(1000);
	return (int64_t)osal_ret;
}
#endif

/* ------------------------02_001_001 [success]------------------------ */
static void Test02_001_001()
{
	osal_mutex_handle_t handle;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	osal_mutex_id_t 	mutex_id;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID001);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 1;
											   
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (handle != NULL))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE,
			OSAL_TEST_ID001);
		return;
	}																	   
}

/* ------------------------02_001_002 [success]------------------------ */
static void Test02_001_002()
{
	osal_mutex_handle_t handle;
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID002);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = OSAL_MUTEX_IMR_00;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (handle != NULL))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE,
			OSAL_TEST_ID002);
		return;
	}																   
}

/* ------------------------02_001_003 [success]------------------------ */
static void Test02_001_003()
{
	osal_mutex_handle_t handle;
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID003);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = OSAL_MUTEX_ICCOM_OPEN;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (handle != NULL))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE,
				OSAL_TEST_ID003);
			return;
		}
	}
}

/* ------------------------02_001_004 [OSAL_RETURN_PAR]------------------------ */
static void Test02_001_004()
{
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID004);
	mutex_id = 4;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------02_001_005 [OSAL_RETURN_ID]------------------------ */
static void Test02_001_005()
{
	osal_mutex_handle_t handle;
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID005);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));								   
	mutex_id = 9999;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID005, 
							test_ret, opt_info);											   
}

/* ------------------------02_001_006 [OSAL_RETURN_ID]------------------------ */
static void Test02_001_006()
{
	osal_mutex_handle_t handle;
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID006);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));								   
	mutex_id = OSAL_MUTEX_RESOURCE_NUM;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID006, 
							test_ret, opt_info);													   
}

/* ------------------------02_001_007 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_001_007()
{
	osal_mutex_handle_t handle, handle1;
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID007);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&handle1, 0x00, sizeof(osal_mutex_handle_t));								   
	mutex_id = 7;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE,
			OSAL_TEST_ID007);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle1);
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID007, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE,
			OSAL_TEST_ID007);
		return;
	}															   
}

/* ------------------------02_001_008 [OSAL_RETURN_STATE]------------------------ */
static void Test02_001_008()
{
	osal_mutex_handle_t handle;
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID008);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));							   
	mutex_id = 8;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID008, 
							test_ret, opt_info);														   
}

/* ------------------------02_001_009 [OSAL_RETURN_STATE]------------------------ */
static void Test02_001_009()
{
	osal_mutex_handle_t handle;
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID009);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));							   
	mutex_id = 9;

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE,OSAL_TEST_ID009);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID009, 
							test_ret, opt_info);														   
}

#if 0
/* ------------------------02_001_010 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_001_010()
{
	osal_mutex_handle_t handle;
	osal_mutex_id_t 	mutex_id;
	e_osal_return_t     osal_ret;
	e_osal_return_t     expect_data;
	char               	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 				test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXCREATE,
														   OSAL_TEST_ID010);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));							   
	mutex_id = 10;

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE,OSAL_TEST_ID010);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreTake = SEMAPHORETAKE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreTake = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THREADSYNCMUTEXCREATE,
							OSAL_TEST_ID010, 
							test_ret, opt_info);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE,OSAL_TEST_ID010);
		return;
	}
}
#endif

/* ------------------------02_002_001 [success]------------------------ */
static void Test02_002_001()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              	 	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXDESTROY,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 1;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXDESTROY,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

/* ------------------------02_002_002 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_002_002()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	char              	 	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXDESTROY,
														   OSAL_TEST_ID002);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexDestroy(NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXDESTROY,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------02_002_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_002_003()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              	 	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXDESTROY,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 3;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID003);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXDESTROY,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* ------------------------02_002_004 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_002_004()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              	 	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXDESTROY,
														   OSAL_TEST_ID004);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 4;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, 20);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXDESTROY,
							OSAL_TEST_ID004, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------02_002_005 [OSAL_RETURN_STATE]------------------------ */
static void Test02_002_005()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              	 	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXDESTROY,
														   OSAL_TEST_ID005);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 1;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID005);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXDESTROY,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
}

#if 0
/* ------------------------02_002_006 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_002_006()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              	 	opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXDESTROY,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 1;

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID006);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreTake = SEMAPHORETAKE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	TIME_MEASURE_STOP

	/* Set stub flag */
	g_stub_flag.SemaphoreTake = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXDESTROY,
							OSAL_TEST_ID006, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID006);
		return;
	}
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY,
			OSAL_TEST_ID006);
		return;
	}
}
#endif

/* ------------------------02_003_001 [Success]------------------------ */
static void Test02_003_001()
{
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	osal_milli_sec_t    		time;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
														   OSAL_TEST_ID001);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));

	mutex_id = 1;
	time     = 1000;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
				OSAL_TEST_ID001);
			return;
		}
	}

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_003_002 [Success]------------------------ */
static void Test02_003_002()
{
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	osal_milli_sec_t    		time;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
														   OSAL_TEST_ID002);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));

	mutex_id = 2;
	time     = 0;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
				OSAL_TEST_ID002);
			return;
		}
	}

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------02_003_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_003_003()
{
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	osal_milli_sec_t    		time;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
														   OSAL_TEST_ID003);
	time = 10;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(NULL, time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* ------------------------02_003_004 [OSAL_RETURN_TIME]------------------------ */
static void Test02_003_004()
{
	e_osal_return_t      		osal_ret, osal_ret1;
	e_osal_return_t      		expect_data;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	st_osal_time_t        		after_time, previous_time;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	osal_nano_sec_t				diff_time;
	st_osal_thread_config_t		config;
	osal_thread_handle_t 		thread_handle;
	st_arg_mutex_waitperiod_t	arg_mutex;
	int64_t						return_value;


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
														   OSAL_TEST_ID004);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&after_time, 0x00, sizeof(st_osal_time_t));
	memset(&previous_time, 0x00, sizeof(st_osal_time_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&thread_handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_mutex, 0x00, sizeof(st_arg_mutex_waitperiod_t));

	mutex_id = 5;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID004);
		return;
	}

	arg_mutex.mutex_handle 	= handle;
	arg_mutex.time			= 10000;
	config.func       = osal_test_mutex_lock;
	config.userarg    = (void *)&arg_mutex;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_003_004";
	config.stack_size = 1024;
	
	osal_ret = R_OSAL_ThreadCreate(&config, 1, &thread_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID004);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(2000);
	
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &previous_time);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, 10);
	TIME_MEASURE_STOP
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &after_time);
	R_OSAL_ClockTimeCalculateTimeDifference(&after_time, &previous_time, &diff_time);

	osal_ret1 = R_OSAL_ThreadJoin(thread_handle, &return_value);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, OSAL_TEST_ID004);
		return;
	}
	
	if ((osal_ret == expect_data) && (diff_time >= 10000000) && ((e_osal_return_t)return_value == OSAL_RETURN_OK))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------02_003_005 [OSAL_RETURN_PAR]------------------------ */
static void Test02_003_005()
{
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	int    						time;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
														   OSAL_TEST_ID005);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));

	mutex_id = 1;
	time     = -10;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID005);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------02_003_006 [OSAL_RETURN_STATE]------------------------ */
static void Test02_003_006()
{
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	osal_milli_sec_t    		time;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
														   OSAL_TEST_ID006);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));

	mutex_id = 6;
	time     = 10;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, OSAL_TEST_ID006);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
}

#if 0
void Test02_003_007_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
														   OSAL_TEST_ID007);	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(g_mutexlock_handle, 1000);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
							OSAL_TEST_ID007, 
							test_ret, opt_info);
	
	R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_common, 10000);
	R_OSAL_ThsyncCondSignal(g_cond_common);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

/* ------------------------02_003_007 [OSAL_RETURN_OK]------------------------ */
static void Test02_003_007()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, OSAL_TEST_ID007);
		return;
	}

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(0, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_00, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}

	g_mutex_common = mutex_handle;
	g_cond_common = cond_handle;

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 7;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}

	g_mutexlock_handle = handle;
	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_003_007_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, OSAL_TEST_ID007);
	}

	R_OSAL_ThsyncMutexTryLock(g_mutex_common);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_common, g_mutex_common, 10000);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

void Test02_003_008_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
														   OSAL_TEST_ID008);	
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(g_mutexlock_handle, 1000);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID008);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(g_mutexlock_handle, 1000);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
							OSAL_TEST_ID008, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_common, 10000);
	R_OSAL_ThsyncCondSignal(g_cond_common);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

/* ------------------------02_003_008 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_003_008()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(1, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_01, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID008);
		return;
	}

	g_mutex_common = mutex_handle;
	g_cond_common = cond_handle;

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 8;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD,
			OSAL_TEST_ID008);
		return;
	}

	g_mutexlock_handle = handle;
	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_003_008_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, OSAL_TEST_ID008);
	}

	R_OSAL_ThsyncMutexTryLock(g_mutex_common);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_common, g_mutex_common, 10000);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_deinit_synchronization() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, OSAL_TEST_ID008);
		return;
	}
}
#endif
/* ------------------------02_004_001 [success]------------------------ */
static void Test02_004_001()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	st_osal_time_t        	absolute_time;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID001);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	mutex_id   = 1;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_nsec += 2000000;
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 2;
		absolute_time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
				OSAL_TEST_ID002);
			return;
		}
	}	

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_004_002 [success]------------------------ */
static void Test02_004_002()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	st_osal_time_t        	absolute_time;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID002);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	mutex_id   = 2;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	if(absolute_time.tv_nsec + 1000000 <= 999999999)
	{
		absolute_time.tv_nsec += 1000000;
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 2;
		absolute_time.tv_nsec = 1000000 - (999999999 - absolute_time.tv_nsec);
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
				OSAL_TEST_ID002);
			return;
		}
	}	

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------02_004_003 [success]------------------------ */
static void Test02_004_003()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	st_osal_time_t        	absolute_time;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID003);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	mutex_id   = 3;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	absolute_time.tv_nsec = 999999999;
	absolute_time.tv_sec = absolute_time.tv_sec + 1;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
				OSAL_TEST_ID002);
			return;
		}
	}	

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------02_004_004 [success]------------------------ */
static void Test02_004_004()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	st_osal_time_t        	absolute_time;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID004);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	mutex_id   = 4;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}


	if(absolute_time.tv_nsec + 999999 <= 999999999)
	{
		absolute_time.tv_nsec += 999999;
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 2;
		absolute_time.tv_nsec = 999999 - (999999999 - absolute_time.tv_nsec);
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID004, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
				OSAL_TEST_ID002);
			return;
		}
	}	

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------02_004_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_004_005()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	st_osal_time_t        	absolute_time;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID005);

	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID005);
		return;
	}

	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_nsec += 2000000;
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 2;
		absolute_time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(NULL, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
}

/* ------------------------02_004_006 [OSAL_RETURN_PAR]------------------------ */
static void Test02_004_006()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID006);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id   = 6;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID006);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(handle, NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID006, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID006);
		return;
	}
}



/* ------------------------02_004_007 [OSAL_RETURN_PAR]------------------------ */
static void Test02_004_007()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	st_osal_time_t        	absolute_time;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID007);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	mutex_id   = 8;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}

	absolute_time.tv_nsec = 1000000000;
	absolute_time.tv_sec = absolute_time.tv_sec + 1;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID007, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------02_004_008 [OSAL_RETURN_TIME]------------------------ */
static void Test02_004_008()
{
	e_osal_return_t      	osal_ret, osal_ret1;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	st_osal_time_t        	absolute_time, previous_time;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;
	osal_nano_sec_t			diff_time;
	st_osal_thread_config_t		config;
	osal_thread_handle_t 		thread_handle;
	st_arg_mutex_waitperiod_t	arg_mutex;
	int64_t						return_value;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID008);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&previous_time, 0x00, sizeof(st_osal_time_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&thread_handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_mutex, 0x00, sizeof(st_arg_mutex_waitperiod_t));

	mutex_id   = 10;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	arg_mutex.mutex_handle 	= handle;
	arg_mutex.time			= 10;
	config.func       = osal_test_mutex_lock;
	config.userarg    = (void *)&arg_mutex;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_004_008";
	config.stack_size = 1024;
	
	osal_ret = R_OSAL_ThreadCreate(&config, 1, &thread_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(2000);
	
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	previous_time.tv_sec =  absolute_time.tv_sec;
	previous_time.tv_nsec = absolute_time.tv_nsec;
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_nsec += 2000000;
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 2;
		absolute_time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(handle, &absolute_time);
	TIME_MEASURE_STOP

	osal_ret1 = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_PRECISION, &absolute_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	osal_ret1 = R_OSAL_ClockTimeCalculateTimeDifference(&absolute_time, &previous_time, &diff_time);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeCalculateTimeDifference() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	osal_ret1 = R_OSAL_ThreadJoin(thread_handle, &return_value);
	if (osal_ret1 != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_THREAD, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, OSAL_TEST_ID004);
		return;
	}

	if ((osal_ret == expect_data) && (diff_time >= 1002000000) && ((e_osal_return_t)return_value == OSAL_RETURN_OK))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID008, 
							test_ret, opt_info);
}

/* ------------------------02_004_009 [OSAL_RETURN_STATE]------------------------ */
static void Test02_004_009()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	st_osal_time_t        	absolute_time;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID009);

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	mutex_id   = 11;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ClockTimeGetTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_nsec += 2000000;
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 2;
		absolute_time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}

	
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, OSAL_TEST_ID009);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID009, 
							test_ret, opt_info);
}

#if 0
void Test02_004_010_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	st_osal_time_t        		absolute_time;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID010);
	
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_nsec += 2000000;
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
		absolute_time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(g_mutexlock_handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID010, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_common, 10000);
	R_OSAL_ThsyncCondSignal(g_cond_common);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

/* ------------------------02_004_010 [OSAL_RETURN_OK]------------------------ */
static void Test02_004_010 ()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, OSAL_TEST_ID010);
		return;
	}

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(0, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_00, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID010);
		return;
	}

	g_mutex_common = mutex_handle;
	g_cond_common = cond_handle;

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 12;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID010);
		return;
	}

	g_mutexlock_handle = handle;
	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_004_010_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, OSAL_TEST_ID010);
	}

	R_OSAL_ThsyncMutexTryLock(g_mutex_common);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_common, g_mutex_common, 10000);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

void Test02_004_011_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	st_osal_time_t        		absolute_time;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
														   OSAL_TEST_ID011);
	
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	osal_ret = R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_nsec += 2000000;
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
		absolute_time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(g_mutexlock_handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockUntilTimeStamp() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, OSAL_TEST_ID011);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(g_mutexlock_handle, &absolute_time);
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
							OSAL_TEST_ID011, 
							test_ret, opt_info);
	
	R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_common, 10000);
	R_OSAL_ThsyncCondSignal(g_cond_common);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

/* ------------------------02_004_011 [OSAL_RETURN_OK]------------------------ */
static void Test02_004_011()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(1, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_01, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID011);
		return;
	}

	g_mutex_common = mutex_handle;
	g_cond_common = cond_handle;

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 13;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP,
			OSAL_TEST_ID011);
		return;
	}

	g_mutexlock_handle = handle;
	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_004_011_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, OSAL_TEST_ID011);
	}

	R_OSAL_ThsyncMutexTryLock(g_mutex_common);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_common, g_mutex_common, 10000);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, OSAL_TEST_ID011);
		return;
	}
}
#endif

/* ------------------------02_005_001 [Success]------------------------ */
static void Test02_005_001()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
														   OSAL_TEST_ID001);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 1;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexTryLock(handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	if(osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
				OSAL_TEST_ID001);
			return;
		}
	}

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_005_002 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_005_002()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
														   OSAL_TEST_ID002);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexTryLock(NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------02_005_003 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_005_003()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
														   OSAL_TEST_ID003);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 3;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, 20);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID003);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexTryLock(handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------02_005_004 [OSAL_RETURN_STATE]------------------------ */
static void Test02_005_004()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
														   OSAL_TEST_ID004);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 4;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncDeinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexTryLock(handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
							OSAL_TEST_ID004, 
							test_ret, opt_info);

							
}

#if 0
void Test02_005_005_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
														   OSAL_TEST_ID005);	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexTryLock(g_mutexlock_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
							OSAL_TEST_ID005, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_common, 10000);
	R_OSAL_ThsyncCondSignal(g_cond_common);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

/* ------------------------Test02_005_005 [OSAL_RETURN_OK]------------------------ */
static void Test02_005_005()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, OSAL_TEST_ID005);
		return;
	}

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(0, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_00, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID005);
		return;
	}

	g_mutex_common = mutex_handle;
	g_cond_common = cond_handle;

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 5;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID005);
		return;
	}

	g_mutexlock_handle = handle;
	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_005_005_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, OSAL_TEST_ID005);
	}

	R_OSAL_ThsyncMutexTryLock(g_mutex_common);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_common, g_mutex_common, 10000);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

void Test02_005_006_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
														   OSAL_TEST_ID006);	
	osal_ret = R_OSAL_ThsyncMutexTryLock(g_mutexlock_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexTryLock() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThsyncMutexTryLock(g_mutexlock_handle);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
							OSAL_TEST_ID006, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_common, 10000);
	R_OSAL_ThsyncCondSignal(g_cond_common);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

/* ------------------------Test02_005_006 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_005_006()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(1, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_01, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID006);
		return;
	}

	g_mutex_common = mutex_handle;
	g_cond_common = cond_handle;

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 6;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID006);
		return;
	}

	g_mutexlock_handle = handle;
	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_005_006_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, OSAL_TEST_ID006);
	}

	R_OSAL_ThsyncMutexTryLock(g_mutex_common);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_common, g_mutex_common, 10000);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);

   osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_deinit_synchronization() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, OSAL_TEST_ID006);
		return;
	}

}
#endif

#if 0
/* ------------------------02_005_007 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_005_007()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	/****************************/
	/* Pre-process				*/
	/****************************/
	/* Init device */
	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncInitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, OSAL_TEST_ID007);
		return;
	}

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
														   OSAL_TEST_ID007);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 7;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
			OSAL_TEST_ID007);
		return;
	}

	g_stub_flag.SemaphoreTake = SEMAPHORETAKE_pdFAIL;
	/****************************/
	/* Test execution			*/
	/****************************/
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexTryLock(handle);
	TIME_MEASURE_STOP

	g_stub_flag.SemaphoreTake = 0;
	/****************************/
	/* Check result				*/
	/****************************/
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXTRYLOCK,
							OSAL_TEST_ID007, 
							test_ret, opt_info);
	
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncDeinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, OSAL_TEST_ID007);
		return;
	}
}
#endif

/* ------------------------02_006_001 [Success]------------------------ */
static void Test02_006_001()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXUNLOCK,
														   OSAL_TEST_ID001);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 1;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, 20);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXUNLOCK,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_006_002 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_006_002()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXUNLOCK,
														   OSAL_TEST_ID002);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexUnlock(NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXUNLOCK,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------02_006_003 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_006_003()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXUNLOCK,
														   OSAL_TEST_ID003);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 3;


	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, 20);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID003);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXUNLOCK,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------02_006_004 [OSAL_RETURN_STATE]------------------------ */
static void Test02_006_004()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXUNLOCK,
														   OSAL_TEST_ID004);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 4;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncDeinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, OSAL_TEST_ID004);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXUNLOCK,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

#if 0
void Test02_006_005_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXUNLOCK,
														   OSAL_TEST_ID005);	
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutexlock_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXUNLOCK,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
	
	R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_common, 10000);
	R_OSAL_ThsyncCondSignal(g_cond_common);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

/* ------------------------Test02_006_005 [OSAL_RETURN_OK]------------------------ */
static void Test02_006_005()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, OSAL_TEST_ID005);
		return;
	}

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(0, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_00, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID005);
		return;
	}

	g_mutex_common = mutex_handle;
	g_cond_common = cond_handle;

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 5;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID005);
		return;
	}
	g_mutexlock_handle = handle;

	osal_ret = R_OSAL_ThsyncMutexTryLock(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_test_R_OSAL_ThsyncMutexTryLock() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID005);
		return;
	}
	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_006_005_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, OSAL_TEST_ID005);
	}

	R_OSAL_ThsyncMutexTryLock(g_mutex_common);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_common, g_mutex_common, 10000);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

void Test02_006_006_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXUNLOCK,
														   OSAL_TEST_ID006);	
	osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutexlock_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexUnlock() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, OSAL_TEST_ID006);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThsyncMutexUnlock(g_mutexlock_handle);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXUNLOCK,
							OSAL_TEST_ID006, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexLockForTimePeriod(g_mutex_common, 10000);
	R_OSAL_ThsyncCondSignal(g_cond_common);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);
}

/* ------------------------Test02_006_006 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_006_006()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t 			cond_handle;
	osal_mutex_handle_t 		mutex_handle;

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	osal_ret = R_OSAL_ThsyncMutexCreate(1, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_01, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID006);
		return;
	}

	g_mutex_common = mutex_handle;
	g_cond_common = cond_handle;

	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 6;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID006);
		return;
	}

	g_mutexlock_handle = handle;

	osal_ret = R_OSAL_ThsyncMutexTryLock(handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_test_R_OSAL_ThsyncMutexTryLock() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID005);
		return;
	}
	Period.Sec = 2;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_006_006_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, OSAL_TEST_ID006);
	}

	R_OSAL_ThsyncMutexTryLock(g_mutex_common);
	R_OSAL_ThsyncCondWaitForTimePeriod(g_cond_common, g_mutex_common, 10000);
	R_OSAL_ThsyncMutexUnlock(g_mutex_common);

   osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_deinit_synchronization() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, OSAL_TEST_ID006);
		return;
	}

}
#endif

#if 0
/* ------------------------02_006_007 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_006_007()
{
	e_osal_return_t      	osal_ret;
	e_osal_return_t      	expect_data;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	handle;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCMUTEXUNLOCK,
														   OSAL_TEST_ID007);
	memset(&handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id    = 1;

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID007);
		return;
	}
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(handle, 20);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK,
			OSAL_TEST_ID007);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncMutexUnlock(handle);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;
	
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCMUTEXUNLOCK,
							OSAL_TEST_ID007, 
							test_ret, opt_info);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncDeinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, OSAL_TEST_ID007);
		return;
	}
}
#endif

/* ------------------------02_007_001 [Success]------------------------ */
static void Test02_007_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID001);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_01;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (cond_handle != NULL))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_007_002 [Success]------------------------ */
static void Test02_007_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID002);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_00;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (cond_handle != NULL))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE,
			OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------02_007_003 [Success]------------------------ */
static void Test02_007_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID003);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_09;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	TIME_MEASURE_STOP
	if ((osal_ret == expect_data) && (cond_handle != NULL))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	if (osal_ret == OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE,
				OSAL_TEST_ID003);
			return;
		}
	}
}

/* ------------------------02_007_004 [OSAL_RETURN_PAR]------------------------ */
static void Test02_007_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID004);
	cond_id     = OSAL_COND_RESERVE_04;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------02_007_005 [OSAL_RETURN_ID]------------------------ */
static void Test02_007_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID005);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = 9999;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
}

/* ------------------------02_007_006 [OSAL_RETURN_ID]------------------------ */
static void Test02_007_006()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID006);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESOURCE_NUM;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
}

/* ------------------------02_007_007 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_007_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle, cond_handle1;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID007);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&cond_handle1, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_05;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE,
			OSAL_TEST_ID007);
		test_ret = OSAL_TEST_NG;
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle1);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID007, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE,
			OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------02_007_008 [OSAL_RETURN_STATE]------------------------ */
static void Test02_007_008()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID008);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_06;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID008, 
							test_ret, opt_info);
}

/* ------------------------02_007_009 [OSAL_RETURN_STATE]------------------------ */
static void Test02_007_009()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID009);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_07;

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE,OSAL_TEST_ID009);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID009, 
							test_ret, opt_info);
}

#if 0
/* ------------------------02_007_010 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_007_010()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
    osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDCREATE,
														   OSAL_TEST_ID010);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_08;

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE,OSAL_TEST_ID010);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDCREATE,
							OSAL_TEST_ID010, 
							test_ret, opt_info);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE,OSAL_TEST_ID010);
		return;
	}
}
#endif

/* ------------------------02_008_001 [Success]------------------------ */
static void Test02_008_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
	osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret    = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDDESTROY,
														   OSAL_TEST_ID001);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_00;


	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDDESTROY,
							OSAL_TEST_ID001, 
							test_ret, opt_info);
}

/* ------------------------02_008_002 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_008_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret    = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDDESTROY,
														   OSAL_TEST_ID002);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondDestroy(NULL);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDDESTROY,
							OSAL_TEST_ID002, 
							test_ret, opt_info);
}

/* ------------------------02_008_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_008_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
	osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret    = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDDESTROY,
														   OSAL_TEST_ID003);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_00;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID003);
		test_ret = OSAL_TEST_NG;	
	}
	else
	{
		TIME_MEASURE_INIT
		TIME_MEASURE_START
		osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
		TIME_MEASURE_STOP
		if (osal_ret == expect_data)
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDDESTROY,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

/* ------------------------02_008_004 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_008_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
	osal_cond_id_t     		cond_id;
	int64_t					return_value;
	st_osal_thread_config_t config1;
	st_osal_thread_config_t config2;
	osal_thread_handle_t 	handle1;
	osal_thread_handle_t 	handle2;
	osal_thread_id_t     	thread_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret    = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDDESTROY,
														   OSAL_TEST_ID004);	

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));

	cond_id  = OSAL_COND_RESERVE_00;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID004);
		return;
	}

	config1.func       = osal_test_cond_destroy_abwait;
	config1.userarg    = (void *)cond_handle;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_008_004_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_cond_destroy;
	config2.userarg    = (void *)cond_handle;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_008_004_02";
	config2.stack_size = 1024;

	thread_id = 0;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID004);
		return;
	}

	thread_id = 1;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID004);
		return;
	}

	if (return_value == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)return_value));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDDESTROY,
							OSAL_TEST_ID004, 
							test_ret, opt_info);

	if(return_value != OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncCondSignal() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
				OSAL_TEST_ID004);
			return;
		}

		osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
				OSAL_TEST_ID004);
			return;
		}
	}
}

/* ------------------------02_008_005 [OSAL_RETURN_BUSY]------------------------ */
static void Test02_008_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
	osal_cond_id_t     		cond_id;
	int64_t					return_value;
	st_osal_thread_config_t config1;
	st_osal_thread_config_t config2;
	osal_thread_handle_t 	handle1;
	osal_thread_handle_t 	handle2;
	osal_thread_id_t     	thread_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret    = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDDESTROY,
														   OSAL_TEST_ID005);	

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));

	cond_id  = OSAL_COND_RESERVE_00;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID005);
		return;
	}

	config1.func       = osal_test_cond_destroy_abwait;
	config1.userarg    = (void *)cond_handle;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_008_005_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_cond_destroy;
	config2.userarg    = (void *)cond_handle;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_008_005_02";
	config2.stack_size = 1024;

	thread_id = 2;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID005);
		return;
	}

	thread_id = 3;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &return_value);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID005);
		return;
	}

	if (return_value == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)return_value));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDDESTROY,
							OSAL_TEST_ID005, 
							test_ret, opt_info);

	if(return_value != OSAL_RETURN_OK)
	{
		osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncCondSignal() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
				OSAL_TEST_ID005);
			return;
		}

		osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
		if (osal_ret != OSAL_RETURN_OK)
		{
			OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
				OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
				OSAL_TEST_ID005);
			return;
		}
	}
}

/* ------------------------02_008_006 [OSAL_RETURN_STATE]------------------------ */
static void Test02_008_006()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
	osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret    = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDDESTROY,
														   OSAL_TEST_ID006);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_00;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY, OSAL_TEST_ID006);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDDESTROY,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
}

#if 0
/* ------------------------02_008_007 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_008_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	osal_cond_handle_t 		cond_handle;
	osal_cond_id_t     		cond_id;
	char              		opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int  					test_ret    = OSAL_TEST_OK;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDDESTROY,
														   OSAL_TEST_ID007);
	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	cond_id     = OSAL_COND_RESERVE_00;

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY, OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY,
			OSAL_TEST_ID007);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDDESTROY,
							OSAL_TEST_ID007, 
							test_ret, opt_info);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY, OSAL_TEST_ID007);
		return;
	}
}
#endif

/* ------------------------02_009_001 [Success]------------------------ */
static void Test02_009_001()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	int64_t       				thread_ret  = 0;
	st_osal_thread_config_t 	config1;
	st_osal_thread_config_t 	config2;
	osal_thread_handle_t 		handle1;
	osal_thread_handle_t 		handle2;
	osal_thread_id_t     		thread_id;
	osal_cond_handle_t  		cond_handle;	
	osal_cond_id_t      		cond_id;
	st_arg_cond_waitperiod_t 	arg_cond;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID001);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 1;
	cond_id  = OSAL_COND_RESERVE_00;
	
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}

	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle	   = cond_handle;
	arg_cond.time	   = 10000;

	config1.func       = osal_test_cond_waitperiod;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_009_001_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_cond_subthread_wakeup_period;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_009_001_02";
	config2.stack_size = 1024;

	thread_id = 0;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);
	thread_id = 1;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_009_002 [OSAL_RETURN_TIME]------------------------ */
static void Test02_009_002()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	int64_t       				thread_ret  = 0;
	st_osal_thread_config_t 	config1;
	osal_thread_handle_t 		handle1;
	osal_thread_id_t     		thread_id;
	osal_cond_handle_t  		cond_handle;	
	osal_cond_id_t      		cond_id;
	st_arg_cond_waitperiod_t 	arg_cond;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID002);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 2;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}

	arg_cond.handle	   = cond_handle;
	arg_cond.time	   = 0;
	arg_cond.mutex_handle = mutex_handle;

	config1.func       = osal_test_cond_waitperiod;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_009_002_01";
	config1.stack_size = 1024;

	thread_id = 2;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	R_OSAL_ThsyncCondSignal(cond_handle);
	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------02_009_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_009_003()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	int64_t       				thread_ret  = 0;
	st_osal_thread_config_t 	config;
	osal_thread_handle_t 		handle;
	osal_thread_id_t     		thread_id;
	st_arg_cond_waitperiod_t 	arg_cond;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID003);

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 3;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID002);
		return;
	}

	arg_cond.handle	   = NULL;
	arg_cond.time	   = 10;
	arg_cond.mutex_handle = mutex_handle;

	config.func       = osal_test_cond_waitperiod;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_009_003";
	config.stack_size = 1024;

	thread_id = 3;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID003);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------02_009_004 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_009_004()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	int64_t       				thread_ret  = 0;
	st_osal_thread_config_t 	config;
	osal_thread_handle_t 		handle;
	osal_cond_handle_t  		cond_handle;	
	osal_cond_id_t      		cond_id;
	osal_thread_id_t     		thread_id;
	st_arg_cond_waitperiod_t 	arg_cond;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID004);

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	cond_id = OSAL_COND_RESERVE_00;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID004);
		return;
	}

	arg_cond.handle	   = cond_handle;
	arg_cond.time	   = 10;

	config.func       = osal_test_cond_waitperiod_mutex_null;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_009_004";
	config.stack_size = 1024;

	thread_id = 4;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID004);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID004, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------02_009_005 [OSAL_RETURN_PAR]------------------------ */
static void Test02_009_005()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	int64_t       				thread_ret  = 0;
	st_osal_thread_config_t 	config1;
	osal_thread_handle_t 		handle1;
	osal_thread_id_t     		thread_id;
	osal_cond_handle_t  		cond_handle;	
	osal_cond_id_t      		cond_id;
	st_arg_cond_waitperiod_t 	arg_cond;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID005);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 5;
	cond_id  = OSAL_COND_RESERVE_00;
	
	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID005);
		return;
	}

	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle	   = cond_handle;
	arg_cond.time	   = -10;

	config1.func       = osal_test_cond_waitperiod;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_009_001_01";
	config1.stack_size = 1024;

	thread_id = 5;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID005);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID005, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID005);
		return;
	}
}

/* ------------------------02_009_006 [OSAL_RETURN_TIME]------------------------ */
static void Test02_009_006()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	int64_t       				thread_ret  = 0;
	st_osal_thread_config_t 	config;
	osal_thread_handle_t 		handle;
	osal_cond_handle_t  		cond_handle;	
	osal_cond_id_t      		cond_id;
	osal_thread_id_t     		thread_id;
	st_arg_cond_waitperiod_t 	arg_cond;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID006);

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id = OSAL_COND_RESERVE_00;
	mutex_id = 6;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID006);
		return;
	}
	arg_cond.handle	   = cond_handle;
	arg_cond.time	   = 2;
	arg_cond.mutex_handle = mutex_handle;

	config.func       = osal_test_cond_waitperiod;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_009_006";
	config.stack_size = 1024;

	thread_id = 7;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID006);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID006, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	R_OSAL_ThsyncCondSignal(cond_handle);
	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------02_009_007 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_009_007() /* Add July 11, 2020 */
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	int64_t       				thread_ret  = 0;
	st_osal_thread_config_t 	config1;
	osal_thread_handle_t 		handle1;
	osal_thread_id_t     		thread_id;
	osal_cond_handle_t  		cond_handle;
	osal_cond_id_t      		cond_id;
	st_arg_cond_waitperiod_t 	arg_cond;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION,
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID007);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 1;
	cond_id  = 1000;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}

	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle	   = cond_handle;
	arg_cond.time	   = 1000;

	config1.func       = osal_test_cond_waitperiod_mutex_unlock;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_009_007_01";
	config1.stack_size = 1024;

	thread_id = 0;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID007,
							test_ret, opt_info);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID007);
		return;
	}
}


/* ------------------------02_009_008 [OSAL_RETURN_STATE]------------------------ */
static void Test02_009_008()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	osal_cond_handle_t  		cond_handle;
	osal_cond_id_t      		cond_id;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID008);

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id = OSAL_COND_RESERVE_00;
	mutex_id = 7;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID008);
		return;
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID008);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, 10);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID008,
							test_ret, opt_info);
}

#if 0
/* ------------------------02_009_009 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_009_009()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data = OSAL_RETURN_OK;
	int  						test_ret    = OSAL_TEST_OK;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	osal_cond_handle_t  		cond_handle;
	osal_cond_id_t      		cond_id;
    char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];


	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
														   OSAL_TEST_ID009);

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id = OSAL_COND_RESERVE_00;
	mutex_id = 8;

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID009);
		return;
	}
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 20);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID009);
		return;
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret  = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, 10);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;
	
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
							OSAL_TEST_ID009,
							test_ret, opt_info);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID009);
		return;
	}
}
#endif

/* ------------------------02_010_001 [Success]------------------------ */
static void Test02_010_001()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config1;
	st_osal_thread_config_t config2;
	osal_thread_id_t     	thread_id;
	osal_thread_handle_t 	handle1;
	osal_thread_handle_t 	handle2;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 1;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID001);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	arg_cond.handle    		   	= cond_handle;
	arg_cond.mutex_handle		= mutex_handle;
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 1;
		arg_cond.time.tv_nsec = absolute_time.tv_nsec + 2000000;
	}
	else
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 2;
		arg_cond.time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}

	config1.func       = osal_test_cond_abwait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_010_001_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_cond_subthread_wakeup_ab;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_010_001_02";
	config2.stack_size = 1024;

	thread_id = 0;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);
	thread_id = 1;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_010_002 [Success]------------------------ */
static void Test02_010_002()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config1;
	st_osal_thread_config_t config2;
	osal_thread_id_t     	thread_id;
	osal_thread_handle_t 	handle1;
	osal_thread_handle_t 	handle2;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 2;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID002);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	arg_cond.handle    		   	= cond_handle;
	arg_cond.mutex_handle    	= mutex_handle;
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 1000000 <= 999999999)
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 1;
		arg_cond.time.tv_nsec = absolute_time.tv_nsec + 1000000;
	}
	else
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 2;
		arg_cond.time.tv_nsec = 1000000 - (999999999 - absolute_time.tv_nsec);
	}

	config1.func       = osal_test_cond_abwait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_010_002_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_cond_subthread_wakeup_ab;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_010_002_02";
	config2.stack_size = 1024;

	thread_id = 2;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);
	thread_id = 3;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------02_010_003 [Success]------------------------ */
static void Test02_010_003()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config1;
	st_osal_thread_config_t config2;
	osal_thread_id_t     	thread_id;
	osal_thread_handle_t 	handle1;
	osal_thread_handle_t 	handle2;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 3;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID003);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	arg_cond.handle    		   	= cond_handle;
	arg_cond.mutex_handle    	= mutex_handle;

	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	arg_cond.time.tv_sec	   = absolute_time.tv_sec + 1;
	arg_cond.time.tv_nsec	   = 999999999;

	config1.func       = osal_test_cond_abwait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_010_003_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_cond_subthread_wakeup_ab;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_010_003_02";
	config2.stack_size = 1024;

	thread_id = 4;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);
	thread_id = 5;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------02_010_004 [Success]------------------------ */
static void Test02_010_004()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config1;
	st_osal_thread_config_t config2;
	osal_thread_id_t     	thread_id;
	osal_thread_handle_t 	handle1;
	osal_thread_handle_t 	handle2;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 4;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID004);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}

	arg_cond.handle    		   	= cond_handle;
	arg_cond.mutex_handle    	= mutex_handle;
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	arg_cond.time.tv_sec	   = absolute_time.tv_sec + 1;
	arg_cond.time.tv_nsec	   = absolute_time.tv_nsec + 999999;

	config1.func       = osal_test_cond_abwait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_010_004_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_cond_subthread_wakeup_ab;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_010_004_02";
	config2.stack_size = 1024;

	thread_id = 6;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);
	thread_id = 7;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID004);
		return;
	}
}

/* ------------------------02_010_005 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_010_005()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config;
	osal_thread_id_t     	thread_id;
	osal_thread_handle_t 	handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 5;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID005);

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID005);
		return;
	}

	arg_cond.handle    		   	= NULL;
	arg_cond.mutex_handle    	= mutex_handle;
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 1;
		arg_cond.time.tv_nsec = absolute_time.tv_nsec + 2000000;
	}
	else
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 2;
		arg_cond.time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}

	config.func       = osal_test_cond_abwait;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_010_005";
	config.stack_size = 1024;

	thread_id = 8;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID005);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID005, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID005);
		return;
	}			
}

/* ------------------------02_010_006 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_010_006()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config;
	osal_thread_id_t     	thread_id;
	osal_thread_handle_t 	handle;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	cond_id  = OSAL_COND_RESERVE_00;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID006);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID006);
		return;
	}

	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 1;
		arg_cond.time.tv_nsec = absolute_time.tv_nsec + 2000000;
	}
	else
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 2;
		arg_cond.time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}
	arg_cond.handle    		   = cond_handle;

	config.func       = osal_test_cond_abwait_mutex_null;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_010_006";
	config.stack_size = 1024;

	thread_id = 9;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID006);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID006, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID006);
		return;
	}
}

/* ------------------------02_010_007 [OSAL_RETURN_PAR]------------------------ */
static void Test02_010_007()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config;
	osal_thread_id_t     	thread_id;
	osal_thread_handle_t 	handle;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 7;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID007);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}

	arg_cond.handle    = cond_handle;
	arg_cond.mutex_handle = mutex_handle;

	config.func       = osal_test_cond_abwait_time_null;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_010_007";
	config.stack_size = 1024;

	thread_id = 10;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID007, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID007);
		return;
	}
}

/* ------------------------02_010_008 [OSAL_RETURN_PAR]------------------------ */
static void Test02_010_008()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config;
	osal_thread_id_t     	thread_id;
	osal_cond_id_t      	cond_id;
	osal_thread_handle_t 	handle;
	osal_cond_handle_t		cond_handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id = OSAL_COND_RESERVE_00;
	mutex_id = 9;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID008);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	arg_cond.handle    		   	= cond_handle;
	arg_cond.mutex_handle		= mutex_handle;
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	arg_cond.time.tv_sec	   = absolute_time.tv_sec + 1;
	arg_cond.time.tv_nsec	   = 1000000000;

	config.func       = osal_test_cond_abwait;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_010_009";
	config.stack_size = 1024;

	thread_id = 11;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID008, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}
	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID008);
		return;
	}
}

/* ------------------------02_010_009 [OSAL_RETURN_TIME]------------------------ */
static void Test02_010_009()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config;
	osal_thread_id_t     	thread_id;
	osal_cond_id_t      	cond_id;
	osal_thread_handle_t 	handle;
	osal_cond_handle_t		cond_handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id = OSAL_COND_RESERVE_00;
	mutex_id = 11;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID009);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	arg_cond.handle    		    = cond_handle;
	arg_cond.mutex_handle		= mutex_handle;
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	arg_cond.time.tv_sec	   = absolute_time.tv_sec + 1;
	arg_cond.time.tv_nsec	   = absolute_time.tv_nsec + 2000000;

	config.func       = osal_test_cond_abwait;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_010_009";
	config.stack_size = 1024;

	thread_id = 11;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID009, 
							test_ret, opt_info);

	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	R_OSAL_ThsyncCondSignal(cond_handle);
	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}
}

/* ------------------------02_010_006 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_010_010() /* Add July 11, 2020 */
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	int64_t       			thread_ret  = 0;
	st_osal_thread_config_t config;
	osal_thread_id_t     	thread_id;
	osal_thread_handle_t 	handle;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_osal_time_t        	absolute_time;
	st_arg_cond_abwait_t	arg_cond;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&config, 0x00, sizeof(st_osal_thread_config_t));
	memset(&handle, 0x00, sizeof(osal_thread_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_abwait_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 1;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID010);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	arg_cond.handle    		    = cond_handle;
	arg_cond.mutex_handle		= mutex_handle;
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 1;
		arg_cond.time.tv_nsec = absolute_time.tv_nsec + 2000000;
	}
	else
	{
		arg_cond.time.tv_sec = absolute_time.tv_sec + 2;
		arg_cond.time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}
	arg_cond.handle    		   = cond_handle;

	config.func       = osal_test_cond_abwait_mutex_unlock;
	config.userarg    = (void *)&arg_cond;
	config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config.task_name  = "task02_010_010";
	config.stack_size = 1024;

	thread_id = 9;
	osal_ret  = R_OSAL_ThreadCreate(&config, thread_id, &handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID010);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID010);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID010,
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID009);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID010);
		return;
	}
}

/* ------------------------02_010_011 [OSAL_RETURN_STATE]------------------------ */
static void Test02_010_011()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_osal_time_t        	absolute_time;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = 1000;
	mutex_id = 1;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION,
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID011);

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID011);
		return;
	}

	R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID011);
		return;
	}

	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
		absolute_time.tv_nsec = absolute_time.tv_nsec + 2000000;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 2;
		absolute_time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, OSAL_TEST_ID011);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, &absolute_time);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION,
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID011,
							test_ret, opt_info);
}

#if 0
/* ------------------------02_010_012 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_010_012()
{
	e_osal_return_t 		osal_ret;
	e_osal_return_t 		expect_data;
	int  					test_ret = OSAL_TEST_OK;
	osal_cond_id_t      	cond_id;
	osal_cond_handle_t  	cond_handle;
	st_osal_time_t        	absolute_time;
	osal_mutex_id_t     	mutex_id;
	osal_mutex_handle_t 	mutex_handle;
    char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	memset(&cond_handle, 0x00, sizeof(osal_cond_handle_t));
	memset(&absolute_time, 0x00, sizeof(st_osal_time_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 1;

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
														   OSAL_TEST_ID012);

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, OSAL_TEST_ID012);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID012);
		return;
	}
	R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID012);
		return;
	}
	osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 20);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexLockForTimePeriod() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
			OSAL_TEST_ID012);
		return;
	}
	R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &absolute_time);
	if(absolute_time.tv_nsec + 2000000 <= 999999999)
	{
		absolute_time.tv_sec = absolute_time.tv_sec;
		absolute_time.tv_nsec = absolute_time.tv_nsec + 2000000;
	}
	else
	{
		absolute_time.tv_sec = absolute_time.tv_sec + 1;
		absolute_time.tv_nsec = 2000000 - (999999999 - absolute_time.tv_nsec);
	}

	/* Set stub flag */
	g_stub_flag.SemaphoreGive = SEMAPHOREGIVE_pdFAIL;

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, &absolute_time);
	TIME_MEASURE_STOP

	/* Clear stub flag */
	g_stub_flag.SemaphoreGive = 0;
	
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP,
							OSAL_TEST_ID012,
							test_ret, opt_info);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, OSAL_TEST_ID012);
		return;
	}
}
#endif

/* ------------------------02_011_001 [Success]------------------------ */
static void Test02_011_001()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	int  						test_ret = OSAL_TEST_OK;
	osal_cond_id_t      		cond_id;
	osal_cond_handle_t  		cond_handle;
	st_osal_thread_config_t 	config1;
	st_osal_thread_config_t 	config2;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle1;
	osal_thread_handle_t 		handle2;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	int64_t						thread_ret = 0;
	int64_t						thread_ret1 = 0;
	st_arg_cond_waitperiod_t 	arg_cond;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDBROADCAST,
														   OSAL_TEST_ID001);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 1;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID001);
		return;
	}
	
	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle = cond_handle;
	arg_cond.time = 2000;

	config1.func       = osal_test_condboardcast_wait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_011_001_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_condboardcast_wakeup;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_011_001_02";
	config2.stack_size = 1024;

	thread_id = 1;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID001);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);
	thread_id = 2;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID011);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDBROADCAST,
							OSAL_TEST_ID001, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_011_002 [Success]------------------------ */
static void Test02_011_002()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	int  						test_ret = OSAL_TEST_OK;
	osal_cond_id_t      		cond_id;
	osal_cond_handle_t  		cond_handle;
	st_osal_thread_config_t 	config1;
	st_osal_thread_config_t 	config2;
	st_osal_thread_config_t 	config3;
	st_osal_thread_config_t 	config4;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle1;
	osal_thread_handle_t 		handle2;
	osal_thread_handle_t 		handle3;
	osal_thread_handle_t 		handle4;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	st_arg_cond_waitperiod_t 	arg_cond;
	int64_t						thread_ret1 = 0;
	int64_t						thread_ret2 = 0;
	int64_t						thread_ret3 = 0;
	int64_t						thread_ret4 = 0;
	char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDBROADCAST,
														   OSAL_TEST_ID002);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle3, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle4, 0x00, sizeof(osal_thread_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config3, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config4, 0x00, sizeof(st_osal_thread_config_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	cond_id  = OSAL_COND_RESERVE_01;
	mutex_id = 2;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle = cond_handle;
	arg_cond.time = 10000;

	config1.func       = osal_test_condboardcast_multi_wait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_011_002_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_condboardcast_multi_wait;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_011_002_02";
	config2.stack_size = 1024;

	config3.func       = osal_test_condboardcast_multi_wait;
	config3.userarg    = (void *)&arg_cond;
	config3.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config3.task_name  = "task02_011_002_03";
	config3.stack_size = 1024;

	config4.func       = osal_test_condboardcast_multiwakeup;
	config4.userarg    = (void *)&arg_cond;
	config4.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config4.task_name  = "task02_011_002_04";
	config4.stack_size = 1024;

	thread_id = 3;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);

	thread_id = 4;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);

	thread_id = 5;
	osal_ret  = R_OSAL_ThreadCreate(&config3, thread_id, &handle3);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);
	thread_id = 6;
	osal_ret  = R_OSAL_ThreadCreate(&config4, thread_id, &handle4);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle4, &thread_ret4);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle3, &thread_ret3);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}

	if ((thread_ret4 == expect_data) && (thread_ret1 == OSAL_RETURN_OK) && (thread_ret2 == OSAL_RETURN_OK) && (thread_ret3 == OSAL_RETURN_OK))
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret4));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDBROADCAST,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	osal_ret =  R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}
	
	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------02_011_003 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_011_003()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	int  						test_ret = OSAL_TEST_OK;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDBROADCAST,
														   OSAL_TEST_ID003);

	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 3;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID003);
		return;
	}
	
	R_OSAL_ThsyncMutexTryLock(mutex_handle);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondBroadcast(NULL);
	TIME_MEASURE_STOP

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDBROADCAST,
							OSAL_TEST_ID003, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID003);
		return;
	}
}

/* ------------------------02_011_004 [OSAL_RETURN_STATE]------------------------ */
static void Test02_011_004()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	osal_cond_id_t      		cond_id;
	osal_cond_handle_t  		cond_handle;
	int  						test_ret = OSAL_TEST_OK;
	char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDBROADCAST,
														   OSAL_TEST_ID004);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	cond_id = OSAL_COND_RESERVE_00;
	
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncDeinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDBROADCAST,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

#if 0
void Test02_011_005_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDBROADCAST,
														   OSAL_TEST_ID005);	
	R_OSAL_ThsyncMutexTryLock(g_mutexlock_handle);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondBroadcast(g_cond_handle);
	TIME_MEASURE_STOP
	R_OSAL_ThsyncMutexUnlock(g_mutexlock_handle);

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDBROADCAST,
							OSAL_TEST_ID005, 
							test_ret, opt_info);
}

/* ------------------------Test02_011_005 [OSAL_RETURN_OK]------------------------ */
static void Test02_011_005()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_handle_t 		mutex_handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t			cond_handle;

	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(5, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_05, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID005);
		return;
	}

	g_mutexlock_handle = mutex_handle;
	g_cond_handle = cond_handle;

	Period.Sec = 1;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_011_005_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, OSAL_TEST_ID005);
	}

	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, 10000);
	R_OSAL_ThsyncMutexUnlock(mutex_handle);

   osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_deinit_synchronization() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, OSAL_TEST_ID005);
		return;
	}
}
#endif

#if 0
/* ------------------------02_011_006 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_011_006()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	int  						test_ret = OSAL_TEST_OK;
	osal_cond_id_t      		cond_id;
	osal_cond_handle_t  		cond_handle;
	st_osal_thread_config_t 	config1;
	st_osal_thread_config_t 	config2;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle1;
	osal_thread_handle_t 		handle2;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	int64_t						thread_ret = 0;
	int64_t						thread_ret1 = 0;
	st_arg_cond_waitperiod_t 	arg_cond;
	char               		opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDBROADCAST,
														   OSAL_TEST_ID006);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 1;

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID006);
		return;
	}
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID006);
		return;
	}
	
	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle = cond_handle;
	arg_cond.time = 2000;
	config1.func       = osal_test_condboardcast_wait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_011_001_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_condboardcast_wakeup_stub;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_011_001_02";
	config2.stack_size = 1024;

	thread_id = 1;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID006);
		return;
	}

	thread_id = 2;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID006);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID011);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDBROADCAST,
							OSAL_TEST_ID006, 
							test_ret, opt_info);
	
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST,
			OSAL_TEST_ID006);
		return;
	}
}
#endif

/* ------------------------02_012_001 [Success]------------------------ */
static void Test02_012_001() /* Modify July 11, 2020 */
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	int  						test_ret = OSAL_TEST_OK;
	osal_cond_id_t      		cond_id;
	osal_cond_handle_t  		cond_handle;
	st_osal_thread_config_t 	config1;
	st_osal_thread_config_t 	config2;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle1;
	osal_thread_handle_t 		handle2;
	osal_thread_handle_t 		handle3;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	int64_t						thread_ret = 0;
	int64_t						thread_ret1 = 0;
	int64_t						thread_ret2 = 0;
	st_arg_cond_waitperiod_t 	arg_cond;
	char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDSIGNAL,
														   OSAL_TEST_ID001);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 1;

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}
	
	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle = cond_handle;
	arg_cond.time = 2000;
	config1.func       = osal_test_condsignal_wait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_012_001_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_condsignal_wakeup;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_012_001_02";
	config2.stack_size = 1024;

	thread_id = 1;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}

	R_OSAL_ThreadSleepForTimePeriod(100);
	thread_id = 2;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}

	thread_id = 3;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle3);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle3, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID011);
		return;
	}

	if (thread_ret == expect_data)
	{
		if (((thread_ret1 == OSAL_RETURN_OK) && (thread_ret2 == OSAL_RETURN_TIME)) ||
		    ((thread_ret1 == OSAL_RETURN_TIME) && (thread_ret2 == OSAL_RETURN_OK)))
		{
			test_ret = OSAL_TEST_OK;
		}
		else
		{
			test_ret = OSAL_TEST_NG;
		}
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDSIGNAL,
							OSAL_TEST_ID001, 
							test_ret, opt_info);


	osal_ret  = R_OSAL_ThsyncCondBroadcast(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondBroadcast() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
	    printf("%d\n", osal_ret);
		OSAL_TEST_ERR("R_OSAL_ThsyncCondDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}
}

/* ------------------------02_012_002 [OSAL_RETURN_HANDLE]------------------------ */
static void Test02_012_002()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	int  						test_ret = OSAL_TEST_OK;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDSIGNAL,
														   OSAL_TEST_ID002);

	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	mutex_id = 3;

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID002);
		return;
	}
	
	R_OSAL_ThsyncMutexTryLock(mutex_handle);

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondSignal(NULL);
	TIME_MEASURE_STOP

	R_OSAL_ThsyncMutexUnlock(mutex_handle);

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDSIGNAL,
							OSAL_TEST_ID002, 
							test_ret, opt_info);

	osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexDestroy() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID002);
		return;
	}
}

/* ------------------------02_012_003 [OSAL_RETURN_STATE]------------------------ */
static void Test02_012_003()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	osal_cond_id_t      		cond_id;
	osal_cond_handle_t  		cond_handle;
	int  						test_ret = OSAL_TEST_OK;
	char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDSIGNAL,
														   OSAL_TEST_ID003);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	cond_id = OSAL_COND_RESERVE_00;
	
	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
	    printf("%d\n", osal_ret);
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID003);
		return;
	}

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncDeinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID001);
		return;
	}

	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);
	TIME_MEASURE_STOP
	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDSIGNAL,
							OSAL_TEST_ID003, 
							test_ret, opt_info);
}

#if 0
void Test02_012_0Test02_012_004_CallbackFunc04_CallbackFunc(UINT8 Channel, UINT16 Result, drvTMU_IntType_t IntType, drvTMU_CbInfo_t *CbInfo)
{
	(void)Channel;
	(void)Result;
	(void)IntType;
	(void)CbInfo;
	e_osal_return_t      		osal_ret;
	e_osal_return_t      		expect_data;
	char              			opt_info[OSAL_TEST_OPT_INFO_BUFFER];
	int 						test_ret = OSAL_TEST_OK;
	
	
	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDSIGNAL,
														   OSAL_TEST_ID004);	
	R_OSAL_ThsyncMutexTryLock(g_mutexlock_handle);
	TIME_MEASURE_INIT
	TIME_MEASURE_START
	osal_ret = R_OSAL_ThsyncCondSignal(g_cond_handle);
	TIME_MEASURE_STOP
	R_OSAL_ThsyncMutexUnlock(g_mutexlock_handle);

	if (osal_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}
	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str(osal_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDSIGNAL,
							OSAL_TEST_ID004, 
							test_ret, opt_info);
}

/* ------------------------Test02_012_004 [OSAL_RETURN_OK]------------------------ */
static void Test02_012_004()
{
	e_osal_return_t      		osal_ret;
	osal_mutex_handle_t 		mutex_handle;
	UINT16 						Result;
	drvTMU_Time_t 				Period;
	osal_cond_handle_t			cond_handle;

	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));

   osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_deinit_synchronization() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(1004, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID004);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_RESERVE_04, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID004);
		return;
	}

	g_mutexlock_handle = mutex_handle;
	g_cond_handle = cond_handle;

	Period.Sec = 1;
	Period.Usec = 0;

	Result = drvTMU_Start(0, TMU_MODE_ONE_SHOT, &Period, TMU_ICP_OFF, Test02_012_004_CallbackFunc, TMU_EDGE_RISE);
	if(Result)
	{
		OSAL_TEST_ERR("drvTMU_Start() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, OSAL_TEST_ID004);
	}

	R_OSAL_ThsyncMutexTryLock(mutex_handle);
	osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, 10000);
	R_OSAL_ThsyncMutexUnlock(mutex_handle);

   osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("osal_deinit_synchronization() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, OSAL_TEST_ID004);
		return;
	}
}
#endif

#if 0
/* ------------------------02_012_005 [OSAL_RETURN_FAIL]------------------------ */
static void Test02_012_005()
{
	e_osal_return_t 			osal_ret;
	e_osal_return_t 			expect_data;
	int  						test_ret = OSAL_TEST_OK;
	osal_cond_id_t      		cond_id;
	osal_cond_handle_t  		cond_handle;
	st_osal_thread_config_t 	config1;
	st_osal_thread_config_t 	config2;
	osal_thread_id_t     		thread_id;
	osal_thread_handle_t 		handle1;
	osal_thread_handle_t 		handle2;
	osal_mutex_id_t     		mutex_id;
	osal_mutex_handle_t 		mutex_handle;
	int64_t						thread_ret = 0;
	int64_t						thread_ret1 = 0;
	st_arg_cond_waitperiod_t 	arg_cond;
	char               			opt_info[OSAL_TEST_OPT_INFO_BUFFER];

	expect_data = (e_osal_return_t) osal_test_get_expectdata(OSAL_TEST_SYNCHRONIZATION, 
														   OSAL_TEST_THREADSYNCCONDSIGNAL,
														   OSAL_TEST_ID005);

	memset(&cond_handle,  0x00, sizeof(osal_cond_handle_t));
	memset(&handle1, 0x00, sizeof(osal_thread_handle_t));
	memset(&handle2, 0x00, sizeof(osal_thread_handle_t));
	memset(&config1, 0x00, sizeof(st_osal_thread_config_t));
	memset(&config2, 0x00, sizeof(st_osal_thread_config_t));
	memset(&mutex_handle, 0x00, sizeof(osal_mutex_handle_t));
	memset(&arg_cond, 0x00, sizeof(st_arg_cond_waitperiod_t));
	cond_id  = OSAL_COND_RESERVE_00;
	mutex_id = 1;

	osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncCondCreate(cond_id, &cond_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncCondCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThsyncMutexCreate(mutex_id, &mutex_handle);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncMutexCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID005);
		return;
	}
	
	arg_cond.mutex_handle = mutex_handle;
	arg_cond.handle = cond_handle;
	arg_cond.time = 2000;
	config1.func       = osal_test_condsignal_wait;
	config1.userarg    = (void *)&arg_cond;
	config1.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config1.task_name  = "task02_012_001_01";
	config1.stack_size = 1024;

	config2.func       = osal_test_condsignal_wakeup_stub;
	config2.userarg    = (void *)&arg_cond;
	config2.priority   = OSAL_THREAD_PRIORITY_TYPE10;
	config2.task_name  = "task02_012_001_02";
	config2.stack_size = 1024;

	thread_id = 1;
	osal_ret  = R_OSAL_ThreadCreate(&config1, thread_id, &handle1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID005);
		return;
	}

	thread_id = 2;
	osal_ret  = R_OSAL_ThreadCreate(&config2, thread_id, &handle2);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadCreate() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID005);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle2, &thread_ret);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID011);
		return;
	}

	osal_ret = R_OSAL_ThreadJoin(handle1, &thread_ret1);
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThreadJoin() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID011);
		return;
	}

	if (thread_ret == expect_data)
	{
		test_ret = OSAL_TEST_OK;
	}
	else
	{
		test_ret = OSAL_TEST_NG;
	}

	OSAL_TEST_MAKE_INFO(opt_info, osal_ret_to_str(expect_data), osal_ret_to_str((e_osal_return_t)thread_ret));

	osal_test_result_output(OSAL_TEST_SYNCHRONIZATION, 
							OSAL_TEST_THREADSYNCCONDSIGNAL,
							OSAL_TEST_ID005, 
							test_ret, opt_info);

	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL,
			OSAL_TEST_ID005);
		return;
	}
}
#endif

/* 02_001: R_OSAL_ThsyncMutexCreate */
static void osal_test_R_OSAL_ThsyncMutexCreate()
{

	e_osal_return_t      	osal_ret;
/* ------------------------Initialize------------------------ */
	Test02_001_008();
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXCREATE, 0);
		return;
	}

	Test02_001_001();
	Test02_001_002();
	Test02_001_003();
	Test02_001_004();
//	Test02_001_005();
//	Test02_001_006();
	Test02_001_007();
	Test02_001_009();
	//Test02_001_010();
}

/* 02_002: R_OSAL_ThsyncMutexDestroy */
static void osal_test_R_OSAL_ThsyncMutexDestroy()
{
	e_osal_return_t      osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncInitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXDESTROY, 0);
		return;
	}

	Test02_002_001();
	Test02_002_002();
	Test02_002_003();
	Test02_002_004();
	Test02_002_005();
	//Test02_002_006();
}

/* 02_003: R_OSAL_ThsyncMutexLockForTimePeriod */
static void osal_test_R_OSAL_ThsyncMutexLockForTimePeriod()
{

	e_osal_return_t      osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, 0);
		return;
	}

	/*if (drvTMU_Initialize())
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, 0);
		return;
	}*/

	Test02_003_001();
	Test02_003_002();
	Test02_003_003();
	Test02_003_004();
	Test02_003_005();
	Test02_003_006();
	//Test02_003_007();
	//Test02_003_008();

	/*if (drvTMU_Deinitialize())
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKFORTIMEPERIOD, 0);
		return;
	}*/
}

/* 02_004: R_OSAL_ThsyncMutexLockUntilTimeStamp */
static void osal_test_R_OSAL_ThsyncMutexLockUntilTimeStamp()
{

	e_osal_return_t      osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncInitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, 0);
		return;
	}

	/*if (drvTMU_Initialize())
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, 0);
		return;
	}*/

	Test02_004_001();
	Test02_004_002();
	Test02_004_003();
	Test02_004_004();
	Test02_004_005();
	Test02_004_006();
	Test02_004_007();
	Test02_004_008();
	Test02_004_009();
	//Test02_004_010();
	//Test02_004_011();

	/*if (drvTMU_Deinitialize())
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCMUTEXLOCKUNTILTIMESTAMP, 0);
		return;
	}*/
}

/* 02_005: R_OSAL_ThsyncMutexTryLock */
static void osal_test_R_OSAL_ThsyncMutexTryLock()
{

	e_osal_return_t      osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncInitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, 0);
		return;
	}

	/*if (drvTMU_Initialize())
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, 0);
		return;
	}*/

	Test02_005_001();
	Test02_005_002();
	Test02_005_003();
	Test02_005_004();
	//Test02_005_005();
	//Test02_005_006();
	//Test02_005_007();

	/*if (drvTMU_Deinitialize())
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXTRYLOCK, 0);
		return;
	}*/
}

/* 02_006: R_OSAL_ThsyncMutexUnlock */
static void osal_test_R_OSAL_ThsyncMutexUnlock()
{
	e_osal_return_t      osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncInitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, 0);
		return;
	}

	/*if (drvTMU_Initialize())
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, 0);
		return;
	}*/

	Test02_006_001();
	Test02_006_002();
	Test02_006_003();
	Test02_006_004();
	//Test02_006_005();
	//Test02_006_006();
	//Test02_006_007();

	/*if (drvTMU_Deinitialize())
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCMUTEXUNLOCK, 0);
		return;
	}*/
}

/* 02_007: R_OSAL_ThsyncCondCreate */
static void osal_test_R_OSAL_ThsyncCondCreate()
{
	e_osal_return_t osal_ret;

	Test02_007_008();
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDCREATE, 0);
		return;
	}
	Test02_007_001();
	Test02_007_002();
	Test02_007_003();
	Test02_007_004();
//	Test02_007_005();
//	Test02_007_006();
	Test02_007_007();
	Test02_007_009();
	//Test02_007_010();
}

/* 02_008: R_OSAL_ThsyncCondDestroy */
static void osal_test_R_OSAL_ThsyncCondDestroy()
{
	e_osal_return_t osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDDESTROY, 0);
		return;
	}

	Test02_008_001();
	Test02_008_002();
	Test02_008_003();
	Test02_008_004();
	Test02_008_005();
	Test02_008_006();
	//Test02_008_007();
}

/* 02_009: R_OSAL_ThsyncCondWaitForTimePeriod */
static void osal_test_R_OSAL_ThsyncCondWaitForTimePeriod()
{
	e_osal_return_t osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}

	Test02_009_001();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
#endif
	Test02_009_002();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
#endif
	Test02_009_003();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
#endif
	Test02_009_004();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
#endif
	Test02_009_005();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
#endif
	Test02_009_006();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
#endif
#if 0 /* Test02_009_007 comment out */
	Test02_009_007(); /* Add July 11, 2020 */
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD,
			OSAL_TEST_ID008);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITFORTIMEPERIOD, 0);
		return;
	}
#endif
	Test02_009_008();
	//Test02_009_009();
}

/* 02_010: R_OSAL_ThsyncCondWaitUntilTimeStamp */
static void osal_test_R_OSAL_ThsyncCondWaitUntilTimeStamp()
{
	e_osal_return_t osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}

	Test02_010_001();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_002();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_003();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_004();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_005();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_006();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_007();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_008();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_009();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
#if 0 /* comment out Test02_010_010 */
	Test02_010_010(); /* Add July 11, 2020 */
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THSYNCCONDWAITUNTILTIMESTAMP, 0);
		return;
	}
#endif
	Test02_010_011();
	//Test02_010_012();
}

/* 02_011: R_OSAL_ThsyncCondBroadcast */
static void osal_test_R_OSAL_ThsyncCondBroadcast()
{
	e_osal_return_t osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_ThsyncInitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}

	/*if (drvTMU_Initialize())
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}*/

	Test02_011_001();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}
#endif
	Test02_011_002();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}
#endif
	Test02_011_003();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}
#endif
	Test02_011_004();
	//Test02_011_005();
	//Test02_011_006();

	/*if (drvTMU_Deinitialize())
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDBROADCAST, 0);
		return;
	}*/
}

/* 02_012: R_OSAL_ThsyncCondSignal */
static void osal_test_R_OSAL_ThsyncCondSignal()
{
	e_osal_return_t osal_ret;

    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, 0);
		return;
	}

	/*if (drvTMU_Initialize())
	{
		OSAL_TEST_ERR("drvTMU_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, 0);
		return;
	}*/

	Test02_012_001();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, 0);
		return;
	}
#endif
	Test02_012_002();
#if 0
	osal_ret = osal_deinit_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, 0);
		return;
	}
    osal_ret = osal_init_synchronization();
	if (osal_ret != OSAL_RETURN_OK)
	{
		OSAL_TEST_ERR("R_OSAL_Initialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, 0);
		return;
	}
#endif
	Test02_012_003();
	//Test02_012_004();
	//Test02_012_005();

	/*if (drvTMU_Deinitialize())
	{
		OSAL_TEST_ERR("drvTMU_Deinitialize() error.",
			OSAL_TEST_SYNCHRONIZATION, OSAL_TEST_THREADSYNCCONDSIGNAL, 0);
		return;
	}*/
}
