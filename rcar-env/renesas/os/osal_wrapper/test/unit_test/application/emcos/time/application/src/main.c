
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include "stub.h"

e_osal_return_t time_test_all(int func_id);
e_osal_return_t time01_01_Initialize(void);
e_osal_return_t time01_02_Deinitialize(void);
e_osal_return_t time01_03_GetVersion(void);
e_osal_return_t time02_01_ThreadCreate(void);
e_osal_return_t time02_02_ThreadSleepUntilTimeStamp(void);
e_osal_return_t time02_03_ThreadSleepForTimePeriod(void);
e_osal_return_t time02_04_ThreadSelf(void);     // xOS2
e_osal_return_t time02_05_ThreadEqual(void);
e_osal_return_t time02_06_ThreadJoin(void);
e_osal_return_t time02_07_ThreadInitializeThreadConfigSt(void); // xOS2
e_osal_return_t time03_01_ThsyncMutexCreate(void);
e_osal_return_t time03_02_ThsyncMutexDestroy(void);
e_osal_return_t time03_03_ThsyncMutexLockUntilTimeStamp(void);
e_osal_return_t time03_04_ThsyncMutexLockForTimePeriod(void);
e_osal_return_t time03_05_ThsyncMutexTryLock(void);
e_osal_return_t time03_06_ThsyncMutexUnlock(void);
e_osal_return_t time03_07_ThsyncCondCreate(void);
e_osal_return_t time03_08_ThsyncCondDestroy(void);
e_osal_return_t time03_09_ThsyncCondWaitForTimePeriod(void);
e_osal_return_t time03_10_ThsyncCondWaitUntilTimeStamp(void);
e_osal_return_t time03_11_ThsyncCondBroadcast(void);
e_osal_return_t time03_12_ThsyncCondSignal(void);
e_osal_return_t time03_13_ThsyncMemoryBarrier(void);
e_osal_return_t time04_01_MqCreate(void);
e_osal_return_t time04_02_MqDelete(void);
e_osal_return_t time04_03_MqSendUntilTimeStamp(void);
e_osal_return_t time04_04_MqSendForTimePeriod(void);
e_osal_return_t time04_05_MqReceiveUntilTimeStamp(void);
e_osal_return_t time04_06_MqReceiveForTimePeriod(void);
e_osal_return_t time04_07_MqIsFull(void);
e_osal_return_t time04_08_MqIsEmpty(void);
e_osal_return_t time04_09_MqGetConfig(void);
e_osal_return_t time04_10_MqReset(void);        // xOS2
e_osal_return_t time04_11_MqInitializeMqConfigSt(void); // xOS2
e_osal_return_t time05_01_ClockTimeGetTimeStamp(void);
e_osal_return_t time05_02_ClockTimeCalculateTimeDifference(void);
e_osal_return_t time06_01_InterruptGetNumOfIrqChannels(void);
e_osal_return_t time06_02_InterruptRegisterIsr(void);
e_osal_return_t time06_03_InterruptUnregisterIsr(void);
e_osal_return_t time06_04_InterruptEnableIsr(void);
e_osal_return_t time06_05_InterruptDisableIsr(void);
e_osal_return_t time06_06_InterruptIsISRContext(void);
e_osal_return_t time06_07_InterruptSetIsrPriority(void);
e_osal_return_t time07_01_IoGetNumOfDevices(void);
e_osal_return_t time07_02_IoGetDeviceList(void);
e_osal_return_t time07_03_IoDeviceOpen(char *device);
e_osal_return_t time07_04_IoDeviceClose(void);
e_osal_return_t time07_05_IoRead8(void);
e_osal_return_t time07_06_IoRead16(void);
e_osal_return_t time07_07_IoRead32(void);
e_osal_return_t time07_08_IoRead64(void);
e_osal_return_t time07_09_IoWrite8(void);
e_osal_return_t time07_10_IoWrite16(void);
e_osal_return_t time07_11_IoWrite32(void);
e_osal_return_t time07_12_IoWrite64(void);
e_osal_return_t time07_13_IoBlockRead8(void);
e_osal_return_t time07_14_IoBlockRead16(void);
e_osal_return_t time07_15_IoBlockRead32(void);
e_osal_return_t time07_16_IoBlockRead64(void);
e_osal_return_t time07_17_IoBlockWrite8(void);
e_osal_return_t time07_18_IoBlockWrite16(void);
e_osal_return_t time07_19_IoBlockWrite32(void);
e_osal_return_t time07_20_IoBlockWrite64(void);
e_osal_return_t time07_21_IoGetDeviceAxiBusId(void);
e_osal_return_t time07_22_IoGetAxiBusIdFromDeviceName(void);
e_osal_return_t time07_23_IoGetAxiBusNameList(void);
e_osal_return_t time07_24_IoGetAxiBusName(void);
e_osal_return_t time07_25_IoGetAxiIdListForDeviceHdl(void);
e_osal_return_t time07_26_IoGetAxiIdListForDeviceName(void);
e_osal_return_t time07_27_IoGetAxiIdListForAxiBusName(void);
e_osal_return_t time07_28_IoGetNumOfAxiIdsForDeviceHdl(void);
e_osal_return_t time07_29_IoGetNumOfAxiIdsForDeviceName(void);
e_osal_return_t time07_30_IoGetNumOfAxiIdsForAxiBusName(void);
e_osal_return_t time08_01_PmSetRequiredState(void);
e_osal_return_t time08_02_PmGetRequiredState(void);
e_osal_return_t time08_03_PmWaitForState(void);
e_osal_return_t time08_04_PmGetState(void);
e_osal_return_t time08_05_PmGetLowestIntermediatePowerState(void);
e_osal_return_t time08_06_PmSetPolicy(void);
e_osal_return_t time08_07_PmGetPolicy(void);
e_osal_return_t time08_08_PmSetResetState(void);
e_osal_return_t time08_09_PmGetResetState(void);
e_osal_return_t time08_10_PmSetPostClock(void);
e_osal_return_t time09_01_MmngrOpen(void);
e_osal_return_t time09_02_MmngrClose(void);
e_osal_return_t time09_03_MmngrAlloc(void);
e_osal_return_t time09_04_MmngrDealloc(void);
e_osal_return_t time09_05_MmngrGetConfig(void);
e_osal_return_t time09_06_MmngrGetOsalMaxConfig(void);
e_osal_return_t time09_07_MmngrInitializeMmngrConfigSt(void);
e_osal_return_t time09_08_MmngrRegisterMonitor(void);
e_osal_return_t time09_09_MmngrGetCpuPtr(void);
e_osal_return_t time09_10_MmngrGetConstCpuPtr(void);
e_osal_return_t time09_11_MmngrFlush(void);
e_osal_return_t time09_12_MmngrInvalidate(void);
e_osal_return_t time09_13_MmngrGetHwAddr(void);
e_osal_return_t time09_14_MmngrGetSize(void);
e_osal_return_t time09_15_MmngrCheckBuffer(void);
e_osal_return_t time09_16_MmngrDebugMmngrPrint(void);
e_osal_return_t time09_17_MmngrDebugBufferPrint(void);
e_osal_return_t time09_18_MmngrInitializeMemoryManagerObj(void);
e_osal_return_t time09_19_MmngrInitializeMemoryBufferObj(void);

/* irq callback function */
void time_callback_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg);

/* mmgr callback function */
void time_callback_mmgr(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl);


void entry_point(void)
{
    /* entry point for WinAMS */
}

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    e_osal_return_t osal_ret;

    printf("Check API time\n");
#ifdef USE_STUB
    g_stub_start = true;
#endif
    osal_ret = time_test_all(101);  // time01_01_Initialize
    osal_ret = time_test_all(201);  // time02_01_ThreadCreate
    osal_ret = time_test_all(202);  // time02_02_ThreadSleepUntilTimeStamp
    osal_ret = time_test_all(203);  // time02_03_ThreadSleepForTimePeriod
    osal_ret = time_test_all(204);  // time02_04_ThreadSelf(void)     // xOS2
    osal_ret = time_test_all(205);  // time02_05_ThreadEqual
    osal_ret = time_test_all(206);  // time02_06_ThreadJoin
    osal_ret = time_test_all(207);  // time02_07_ThreadInitializeThreadConfigSt
    osal_ret = time_test_all(301);  // time03_01_ThsyncMutexCreate
    osal_ret = time_test_all(303);  // time03_03_ThsyncMutexLockUntilTimeStamp
    osal_ret = time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret = time_test_all(304);  // time03_04_ThsyncMutexLockForTimePeriod
    osal_ret = time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret = time_test_all(305);  // time03_05_ThsyncMutexTryLock
    osal_ret = time_test_all(307);  // time03_07_ThsyncCondCreate
    osal_ret = time_test_all(309);  // time03_09_ThsyncCondWaitForTimePeriod
    osal_ret = time_test_all(310);  // time03_10_ThsyncCondWaitUntilTimeStamp
    osal_ret = time_test_all(311);  // time03_11_ThsyncCondBroadcast
    osal_ret = time_test_all(312);  // time03_12_ThsyncCondSignal
    osal_ret = time_test_all(308);  // time03_08_ThsyncCondDestroy
    osal_ret = time_test_all(306);  // time03_06_ThsyncMutexUnlock
    osal_ret = time_test_all(302);  // time03_02_ThsyncMutexDestroy
    osal_ret = time_test_all(313);  // time03_13_ThsyncMemoryBarrier
    osal_ret = time_test_all(401);  // time04_01_MqCreate
    osal_ret = time_test_all(403);  // time04_03_MqSendUntilTimeStamp
    osal_ret = time_test_all(405);  // time04_05_MqReceiveUntilTimeStamp
    osal_ret = time_test_all(404);  // time04_04_MqSendForTimePeriod
    osal_ret = time_test_all(406);  // time04_06_MqReceiveForTimePeriod
    osal_ret = time_test_all(407);  // time04_07_MqIsFull
    osal_ret = time_test_all(408);  // time04_08_MqIsEmpty
    osal_ret = time_test_all(409);  // time04_09_MqGetConfig
    osal_ret = time_test_all(410);  // time04_10_MqReset
    osal_ret = time_test_all(411);  // time04_11_MqInitializeMqConfigSt
    osal_ret = time_test_all(402);  // time04_02_MqDelete
    osal_ret = time_test_all(501);  // time05_01_ClockTimeGetTimeStamp
    osal_ret = time_test_all(502);  // time05_02_ClockTimeCalculateTimeDifference
    osal_ret = time_test_all(701);  // time07_01_IoGetNumOfDevices
    osal_ret = time_test_all(702);  // time07_02_IoGetDeviceList
    osal_ret = time_test_all(703);  // time07_03_IoDeviceOpen(ims_00)
    osal_ret = time_test_all(705);  // time07_05_IoRead8
    osal_ret = time_test_all(706);  // time07_06_IoRead16
    osal_ret = time_test_all(707);  // time07_07_IoRead32
    osal_ret = time_test_all(708);  // time07_08_IoRead64
    osal_ret = time_test_all(709);  // time07_09_IoWrite8
    osal_ret = time_test_all(710);  // time07_10_IoWrite16
    osal_ret = time_test_all(711);  // time07_11_IoWrite32
    osal_ret = time_test_all(712);  // time07_12_IoWrite64
    osal_ret = time_test_all(713);  // time07_13_IoBlockRead8
    osal_ret = time_test_all(714);  // time07_14_IoBlockRead16
    osal_ret = time_test_all(715);  // time07_15_IoBlockRead32
    osal_ret = time_test_all(716);  // time07_16_IoBlockRead64
    osal_ret = time_test_all(717);  // time07_17_IoBlockWrite8
    osal_ret = time_test_all(718);  // time07_18_IoBlockWrite16
    osal_ret = time_test_all(719);  // time07_19_IoBlockWrite32
    osal_ret = time_test_all(720);  // time07_20_IoBlockWrite64
    osal_ret = time_test_all(721);  // time07_21_IoGetDeviceAxiBusId
    osal_ret = time_test_all(722);  // time07_22_IoGetAxiBusIdFromDeviceName
    osal_ret = time_test_all(723);  // time07_23_IoGetAxiBusNameList
    osal_ret = time_test_all(724);  // time07_24_IoGetAxiBusName
    osal_ret = time_test_all(726);  // time07_25_IoGetAxiIdListForDeviceHdl
    osal_ret = time_test_all(727);  // time07_26_IoGetAxiIdListForDeviceName
    osal_ret = time_test_all(728);  // time07_27_IoGetAxiIdListForAxiBusName
    osal_ret = time_test_all(729);  // time07_28_IoGetNumOfAxiIdsForDeviceHdl
    osal_ret = time_test_all(730);  // time07_29_IoGetNumOfAxiIdsForDeviceName
    osal_ret = time_test_all(731);  // time07_30_IoGetNumOfAxiIdsForAxiBusName
    osal_ret = time_test_all(601);  // time06_01_InterruptGetNumOfIrqChannels
    osal_ret = time_test_all(602);  // time06_02_InterruptRegisterIsr
    osal_ret = time_test_all(604);  // time06_04_InterruptEnableIsr
    osal_ret = time_test_all(605);  // time06_05_InterruptDisableIsr
    osal_ret = time_test_all(603);  // time06_03_InterruptUnregisterIsr
    osal_ret = time_test_all(606);  // time06_06_InterruptIsISRContext
    osal_ret = time_test_all(607);  // time06_07_InterruptSetIsrPriority
    osal_ret = time_test_all(801);  // time08_01_PmSetRequiredState
    osal_ret = time_test_all(802);  // time08_02_PmGetRequiredState
    osal_ret = time_test_all(803);  // time08_03_PmWaitForState
    osal_ret = time_test_all(804);  // time08_04_PmGetState
    osal_ret = time_test_all(805);  // time08_05_PmGetLowestIntermediatePowerState
    osal_ret = time_test_all(806);  // time08_06_PmSetPolicy
    osal_ret = time_test_all(807);  // time08_07_PmGetPolicy
    osal_ret = time_test_all(808);  // time08_08_PmSetResetState
    osal_ret = time_test_all(809);  // time08_09_PmGetResetState
    osal_ret = time_test_all(704);  // time07_04_IoDeviceClose
    osal_ret = time_test_all(725);  // time07_03_IoDeviceOpen(fbc_00)
    osal_ret = time_test_all(810);  // time08_10_PmSetPostClock
    osal_ret = time_test_all(901);  // time09_01_MmngrOpen
    osal_ret = time_test_all(903);  // time09_03_MmngrAlloc
    osal_ret = time_test_all(905);  // time09_05_MmngrGetConfig
    osal_ret = time_test_all(906);  // time09_06_MmngrGetOsalMaxConfig
    osal_ret = time_test_all(907);  // time09_07_MmngrInitializeMmngrConfigSt
    osal_ret = time_test_all(908);  // time09_08_MmngrRegisterMonitor
    osal_ret = time_test_all(909);  // time09_09_MmngrGetCpuPtr
    osal_ret = time_test_all(910);  // time09_10_MmngrGetConstCpuPtr
    osal_ret = time_test_all(911);  // time09_11_MmngrFlush
    osal_ret = time_test_all(912);  // time09_12_MmngrInvalidate
    osal_ret = time_test_all(913);  // time09_13_MmngrGetHwAddr
    osal_ret = time_test_all(914);  // time09_14_MmngrGetSize
    osal_ret = time_test_all(915);  // time09_15_MmngrCheckBuffer
    osal_ret = time_test_all(916);  // time09_16_MmngrDebugMmngrPrint
    osal_ret = time_test_all(917);  // time09_17_MmngrDebugBufferPrint
    osal_ret = time_test_all(904);  // time09_04_MmngrDealloc
    osal_ret = time_test_all(902);  // time09_02_MmngrClose
    osal_ret = time_test_all(704);  // time07_04_IoDeviceClose
    osal_ret = time_test_all(918);  // time09_18_MmngrInitializeMemoryManagerObj
    osal_ret = time_test_all(919);  // time09_19_MmngrInitializeMemoryBufferObj
    osal_ret = time_test_all(103);  // time01_03_GetVersion
    osal_ret = time_test_all(102);  // time01_02_Deinitialize
#ifdef USE_STUB
    g_stub_start = false;
#endif
    return 0;
}

/* The first ID of the configuration table */
#define OSAL_THREAD_CREATE_ID   (0x3000U)   /* OSAL_THREAD_ICCOM_DEMO_TASK */
#define OSAL_MUTEX_CREATE_ID    (0x1000U)   /* OSAL_MUTEX_IMR_00 */
#define OSAL_COND_CREATE_ID     (0x7000U)   /* OSAL_COND_CPURTT_00 */
#define OSAL_MQ_CREATE_ID       (0x1000U)   /* OSAL_MQ_IMR_MSG_00 */

static bool is_initialized = false;
static bool is_thread_created = false;
static bool is_mutex_created = false;
static bool is_cond_created = false;
static bool is_mq_created = false;
static bool is_io_numget = false;
static bool is_io_devlistget = false;
static bool is_io_opened = false;
osal_thread_handle_t    th_handle = NULL;
osal_mutex_handle_t     mutex_handle = NULL;
osal_cond_handle_t      cond_handle = NULL;
osal_mq_handle_t        mq_handle = NULL;
st_osal_mq_config_t     mq_config = {1, 4};   // {max_num_msg, msg_size}
osal_device_handle_t    dev_handle = NULL;
osal_memory_manager_handle_t    mmgr_handle = NULL;
osal_memory_buffer_handle_t     buf_handle = NULL;

e_osal_return_t time_test_all(int func_id)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
#ifndef WINAMS_STUB
#define sec_to_nsec (1000000000)
    struct timespec abstime_before;
    struct timespec abstime_after;

    /* initialize global variable for stub */
    init_stub_value();

    /* get current time */
    clock_gettime(CLOCK_REALTIME, &abstime_before);
#endif /* WINAMS_STUB */

    switch (func_id)
    {
        case 101:  osal_ret = time01_01_Initialize(); break;
        case 102:  osal_ret = time01_02_Deinitialize(); break;
        case 103:  osal_ret = time01_03_GetVersion(); break;
        case 201:  osal_ret = time02_01_ThreadCreate(); break;
        case 202:  osal_ret = time02_02_ThreadSleepUntilTimeStamp(); break;
        case 203:  osal_ret = time02_03_ThreadSleepForTimePeriod(); break;
        case 204:  osal_ret = time02_04_ThreadSelf(); break;
        case 205:  osal_ret = time02_05_ThreadEqual(); break;
        case 206:  osal_ret = time02_06_ThreadJoin(); break;
        case 207:  osal_ret = time02_07_ThreadInitializeThreadConfigSt(); break;
        case 301:  osal_ret = time03_01_ThsyncMutexCreate(); break;
        case 302:  osal_ret = time03_02_ThsyncMutexDestroy(); break;
        case 303:  osal_ret = time03_03_ThsyncMutexLockUntilTimeStamp(); break;
        case 304:  osal_ret = time03_04_ThsyncMutexLockForTimePeriod(); break;
        case 305:  osal_ret = time03_05_ThsyncMutexTryLock(); break;
        case 306:  osal_ret = time03_06_ThsyncMutexUnlock(); break;
        case 307:  osal_ret = time03_07_ThsyncCondCreate(); break;
        case 308:  osal_ret = time03_08_ThsyncCondDestroy(); break;
        case 309:  osal_ret = time03_09_ThsyncCondWaitForTimePeriod(); break;
        case 310:  osal_ret = time03_10_ThsyncCondWaitUntilTimeStamp(); break;
        case 311:  osal_ret = time03_11_ThsyncCondBroadcast(); break;
        case 312:  osal_ret = time03_12_ThsyncCondSignal(); break;
        case 313:  osal_ret = time03_13_ThsyncMemoryBarrier(); break;
        case 401:  osal_ret = time04_01_MqCreate(); break;
        case 402:  osal_ret = time04_02_MqDelete(); break;
        case 403:  osal_ret = time04_03_MqSendUntilTimeStamp(); break;
        case 404:  osal_ret = time04_04_MqSendForTimePeriod(); break;
        case 405:  osal_ret = time04_05_MqReceiveUntilTimeStamp(); break;
        case 406:  osal_ret = time04_06_MqReceiveForTimePeriod(); break;
        case 407:  osal_ret = time04_07_MqIsFull(); break;
        case 408:  osal_ret = time04_08_MqIsEmpty(); break;
        case 409:  osal_ret = time04_09_MqGetConfig(); break;
        case 410:  osal_ret = time04_10_MqReset(); break;
        case 411:  osal_ret = time04_11_MqInitializeMqConfigSt(); break;
        case 501:  osal_ret = time05_01_ClockTimeGetTimeStamp(); break;
        case 502:  osal_ret = time05_02_ClockTimeCalculateTimeDifference(); break;
        case 601:  osal_ret = time06_01_InterruptGetNumOfIrqChannels(); break;
        case 602:  osal_ret = time06_02_InterruptRegisterIsr(); break;
        case 603:  osal_ret = time06_03_InterruptUnregisterIsr(); break;
        case 604:  osal_ret = time06_04_InterruptEnableIsr(); break;
        case 605:  osal_ret = time06_05_InterruptDisableIsr(); break;
        case 606:  osal_ret = time06_06_InterruptIsISRContext(); break;
        case 607:  osal_ret = time06_07_InterruptSetIsrPriority(); break;
        case 701:  osal_ret = time07_01_IoGetNumOfDevices(); break;
        case 702:  osal_ret = time07_02_IoGetDeviceList(); break;
        case 703:  osal_ret = time07_03_IoDeviceOpen("ims_00"); break;
        case 704:  osal_ret = time07_04_IoDeviceClose(); break;
        case 705:  osal_ret = time07_05_IoRead8(); break;
        case 706:  osal_ret = time07_06_IoRead16(); break;
        case 707:  osal_ret = time07_07_IoRead32(); break;
        case 708:  osal_ret = time07_08_IoRead64(); break;
        case 709:  osal_ret = time07_09_IoWrite8(); break;
        case 710:  osal_ret = time07_10_IoWrite16(); break;
        case 711:  osal_ret = time07_11_IoWrite32(); break;
        case 712:  osal_ret = time07_12_IoWrite64(); break;
        case 713:  osal_ret = time07_13_IoBlockRead8(); break;
        case 714:  osal_ret = time07_14_IoBlockRead16(); break;
        case 715:  osal_ret = time07_15_IoBlockRead32(); break;
        case 716:  osal_ret = time07_16_IoBlockRead64(); break;
        case 717:  osal_ret = time07_17_IoBlockWrite8(); break;
        case 718:  osal_ret = time07_18_IoBlockWrite16(); break;
        case 719:  osal_ret = time07_19_IoBlockWrite32(); break;
        case 720:  osal_ret = time07_20_IoBlockWrite64(); break;
        case 721:  osal_ret = time07_21_IoGetDeviceAxiBusId(); break;
        case 722:  osal_ret = time07_22_IoGetAxiBusIdFromDeviceName(); break;
        case 723:  osal_ret = time07_23_IoGetAxiBusNameList(); break;
        case 724:  osal_ret = time07_24_IoGetAxiBusName(); break;
        case 725:  osal_ret = time07_03_IoDeviceOpen("fbc_00"); break;
        case 726:  osal_ret = time07_25_IoGetAxiIdListForDeviceHdl(); break;
        case 727:  osal_ret = time07_26_IoGetAxiIdListForDeviceName(); break;
        case 728:  osal_ret = time07_27_IoGetAxiIdListForAxiBusName(); break;
        case 729:  osal_ret = time07_28_IoGetNumOfAxiIdsForDeviceHdl(); break;
        case 730:  osal_ret = time07_29_IoGetNumOfAxiIdsForDeviceName(); break;
        case 731:  osal_ret = time07_30_IoGetNumOfAxiIdsForAxiBusName(); break;
        case 801:  osal_ret = time08_01_PmSetRequiredState(); break;
        case 802:  osal_ret = time08_02_PmGetRequiredState(); break;
        case 803:  osal_ret = time08_03_PmWaitForState(); break;
        case 804:  osal_ret = time08_04_PmGetState(); break;
        case 805:  osal_ret = time08_05_PmGetLowestIntermediatePowerState(); break;
        case 806:  osal_ret = time08_06_PmSetPolicy(); break;
        case 807:  osal_ret = time08_07_PmGetPolicy(); break;
        case 808:  osal_ret = time08_08_PmSetResetState(); break;
        case 809:  osal_ret = time08_09_PmGetResetState(); break;
        case 810:  osal_ret = time08_10_PmSetPostClock(); break;
        case 901:  osal_ret = time09_01_MmngrOpen(); break;
        case 902:  osal_ret = time09_02_MmngrClose(); break;
        case 903:  osal_ret = time09_03_MmngrAlloc(); break;
        case 904:  osal_ret = time09_04_MmngrDealloc(); break;
        case 905:  osal_ret = time09_05_MmngrGetConfig(); break;
        case 906:  osal_ret = time09_06_MmngrGetOsalMaxConfig(); break;
        case 907:  osal_ret = time09_07_MmngrInitializeMmngrConfigSt(); break;
        case 908:  osal_ret = time09_08_MmngrRegisterMonitor(); break;
        case 909:  osal_ret = time09_09_MmngrGetCpuPtr(); break;
        case 910:  osal_ret = time09_10_MmngrGetConstCpuPtr(); break;
        case 911:  osal_ret = time09_11_MmngrFlush(); break;
        case 912:  osal_ret = time09_12_MmngrInvalidate(); break;
        case 913:  osal_ret = time09_13_MmngrGetHwAddr(); break;
        case 914:  osal_ret = time09_14_MmngrGetSize(); break;
        case 915:  osal_ret = time09_15_MmngrCheckBuffer(); break;
        case 916:  osal_ret = time09_16_MmngrDebugMmngrPrint(); break;
        case 917:  osal_ret = time09_17_MmngrDebugBufferPrint(); break;
        case 918:  osal_ret = time09_18_MmngrInitializeMemoryManagerObj(); break;
        case 919:  osal_ret = time09_19_MmngrInitializeMemoryBufferObj(); break;
        default:   osal_ret = OSAL_RETURN_PAR; break;
    }

#ifndef WINAMS_STUB
    /* print elapsed time */
    clock_gettime(CLOCK_REALTIME, &abstime_after);
    uint64_t    elapsed_time = ((abstime_after.tv_sec * sec_to_nsec) + abstime_after.tv_nsec) - ((abstime_before.tv_sec * sec_to_nsec) + abstime_before.tv_nsec) - get_stub_time();
    printf("\n\nfunc-id=\t%d\tresult=\t%4d\tExecution time:\t%ld\t (ns)\n",
        func_id, osal_ret, elapsed_time);
#ifdef USE_STUB   /* Call count */
    /* print pthread_create call count */
    printf("pthread_create call count    :\t%ld\n", g_pthread_create_count);

    /* print pthread_mutex_init call count */
    printf("pthread_mutex_init call count:\t%ld\n", g_pthread_mutex_init_count);

    /* print pthread_cond_init call count */
    printf("pthread_cond_init call count :\t%ld\n", g_pthread_cond_init_count);

    /* print malloc call count */
    printf("malloc call count            :\t%ld\n", g_malloc_count);

    /* print realloc call count */
    printf("realloc call count           :\t%ld\n", g_realloc_count);

    /* print calloc call count */
    printf("calloc call count            :\t%ld\n", g_calloc_count);

    /* print timer_create call count */
    printf("timer_create call count      :\t%ld\n", g_timer_create_count);
#endif  /* Call count */
#endif  /* WINAMS_STUB */

    return osal_ret;
}

e_osal_return_t time01_01_Initialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if(is_initialized == false)
    {
        osal_ret = R_OSAL_Initialize();
        
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_initialized = true;
        }
    }
    return osal_ret;
}

e_osal_return_t time01_02_Deinitialize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_Deinitialize();
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_initialized = false;
    }

    return osal_ret;
}

e_osal_return_t time01_03_GetVersion(void)
{
    st_osal_version_t* version;

    version = R_OSAL_GetVersion();

    return OSAL_RETURN_OK;
}

/* irq callback function */
void time_dummy_func(void)
{
    return;
}

e_osal_return_t time02_01_ThreadCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_thread_config_t config;

    if (is_thread_created == false)
    {
        config.func       = time_dummy_func;
        config.userarg    = NULL;
        config.priority   = OSAL_THREAD_PRIORITY_TYPE10;
        config.task_name  = "time_Thread";
        config.stack_size = 0x1000;

        osal_ret = R_OSAL_ThreadCreate(&config, OSAL_THREAD_CREATE_ID, &th_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_thread_created = true;
        }
    }
    
    return osal_ret;
}

e_osal_return_t time02_02_ThreadSleepUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t      absolute_time;

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThreadSleepUntilTimeStamp(&absolute_time);

    return osal_ret;
}

e_osal_return_t time02_03_ThreadSleepForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThreadSleepForTimePeriod(2500);

    return osal_ret;
}

e_osal_return_t time02_04_ThreadSelf(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_thread_handle_t handle;

    osal_ret = R_OSAL_ThreadSelf(&handle);

    return osal_ret;
}

e_osal_return_t time02_05_ThreadEqual(void)
{
    e_osal_return_t         osal_ret = OSAL_RETURN_OK;
    bool                    result   = false;
    osal_thread_handle_t    th_handle2;

    th_handle2 = th_handle;
    osal_ret = R_OSAL_ThreadEqual(th_handle, th_handle2, &result);
    return osal_ret;
}

e_osal_return_t time02_06_ThreadJoin(void)
{
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    int64_t         return_value = 0;

    osal_ret = R_OSAL_ThreadJoin(th_handle, &return_value);

    return osal_ret;
}

e_osal_return_t time02_07_ThreadInitializeThreadConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_thread_config_t config;

    osal_ret = R_OSAL_ThreadInitializeThreadConfigSt(&config);

    return osal_ret;
}

e_osal_return_t time03_01_ThsyncMutexCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_mutex_created == false)
    {
        osal_ret = R_OSAL_ThsyncMutexCreate(OSAL_MUTEX_CREATE_ID, &mutex_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_mutex_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time03_02_ThsyncMutexDestroy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexDestroy(mutex_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_mutex_created = false;
    }

    return osal_ret;
}

e_osal_return_t time03_03_ThsyncMutexLockUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_ThsyncMutexLockUntilTimeStamp(mutex_handle, &absolute_time);

    return osal_ret;
}

e_osal_return_t time03_04_ThsyncMutexLockForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexLockForTimePeriod(mutex_handle, 2500);

    return osal_ret;
}

e_osal_return_t time03_05_ThsyncMutexTryLock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexTryLock(mutex_handle);

    return osal_ret;
}

e_osal_return_t time03_06_ThsyncMutexUnlock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMutexUnlock(mutex_handle);

    return osal_ret;
}

e_osal_return_t time03_07_ThsyncCondCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_cond_created == false)
    {
        osal_ret = R_OSAL_ThsyncCondCreate(OSAL_COND_CREATE_ID, &cond_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_cond_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time03_08_ThsyncCondDestroy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondDestroy(cond_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_cond_created = false;
    }

    return osal_ret;
}

e_osal_return_t time03_09_ThsyncCondWaitForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondWaitForTimePeriod(cond_handle, mutex_handle, 2500);
    
    return osal_ret;
}

e_osal_return_t time03_10_ThsyncCondWaitUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;
    
    osal_ret = R_OSAL_ThsyncCondWaitUntilTimeStamp(cond_handle, mutex_handle, &absolute_time);

    return osal_ret;
}

e_osal_return_t time03_11_ThsyncCondBroadcast(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondBroadcast(cond_handle);

    return osal_ret;
}

e_osal_return_t time03_12_ThsyncCondSignal(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncCondSignal(cond_handle);

    return osal_ret;
}

e_osal_return_t time03_13_ThsyncMemoryBarrier(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_ThsyncMemoryBarrier();

    return osal_ret;
}

e_osal_return_t time04_01_MqCreate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_mq_created == false)
    {
        osal_ret = R_OSAL_MqCreate(&mq_config, OSAL_MQ_CREATE_ID, &mq_handle);
        if(osal_ret == OSAL_RETURN_OK)
        {
            is_mq_created = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time04_02_MqDelete(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MqDelete(mq_handle);
    if(osal_ret == OSAL_RETURN_OK)
    {
        is_mq_created = false;
    }

    return osal_ret;
}

e_osal_return_t time04_03_MqSendUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;
    char            send_data[50] = {0};

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;
    osal_ret = R_OSAL_MqSendUntilTimeStamp(mq_handle, &absolute_time, send_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t time04_04_MqSendForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            send_data[50] = {0};

    osal_ret = R_OSAL_MqSendForTimePeriod(mq_handle, 100, send_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t time04_05_MqReceiveUntilTimeStamp(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time;
    char            recv_data[50];

    absolute_time.tv_sec  = 1;
    absolute_time.tv_nsec = 2000;

    osal_ret = R_OSAL_MqReceiveUntilTimeStamp(mq_handle, &absolute_time, recv_data, mq_config.msg_size);
    
    return osal_ret;
}

e_osal_return_t time04_06_MqReceiveForTimePeriod(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            recv_data[50];

    osal_ret = R_OSAL_MqReceiveForTimePeriod(mq_handle, 100, recv_data, mq_config.msg_size);

    return osal_ret;
}

e_osal_return_t time04_07_MqIsFull(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result   = false;

    osal_ret = R_OSAL_MqIsFull(mq_handle, &result);

    return osal_ret;
}

e_osal_return_t time04_08_MqIsEmpty(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result   = false;

    osal_ret = R_OSAL_MqIsEmpty(mq_handle, &result);

    return osal_ret;
}

e_osal_return_t time04_09_MqGetConfig(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t config;

    osal_ret = R_OSAL_MqGetConfig(mq_handle, &config);

    return osal_ret;
}

e_osal_return_t time04_10_MqReset(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MqReset(mq_handle);

    return osal_ret;
}

e_osal_return_t time04_11_MqInitializeMqConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mq_config_t config;

    osal_ret = R_OSAL_MqInitializeMqConfigSt(&config);

    return osal_ret;
}

e_osal_return_t time05_01_ClockTimeGetTimeStamp(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    e_osal_clock_type_t clock_type = OSAL_CLOCK_TYPE_HIGH_RESOLUTION;
    st_osal_time_t      absolute_time;

    osal_ret = R_OSAL_ClockTimeGetTimeStamp(clock_type, &absolute_time);

    return osal_ret;
}

e_osal_return_t time05_02_ClockTimeCalculateTimeDifference(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_time_t  absolute_time1;
    st_osal_time_t  absolute_time2;
    osal_nano_sec_t different_time;

    absolute_time1.tv_sec = 1;
    absolute_time1.tv_nsec = 2000;
    absolute_time2.tv_sec = 2;
    absolute_time2.tv_nsec = 1000;

    osal_ret = R_OSAL_ClockTimeCalculateTimeDifference(&absolute_time2, &absolute_time1, &different_time);

    return osal_ret;
}

e_osal_return_t time06_01_InterruptGetNumOfIrqChannels(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          num_channels;

    osal_ret = R_OSAL_InterruptGetNumOfIrqChannels(dev_handle, &num_channels);

    return osal_ret;
}

/* irq callback function */
void time_callback_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
    return;
}

e_osal_return_t time06_02_InterruptRegisterIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t        arg_data = 0;

    osal_ret = R_OSAL_InterruptRegisterIsr(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE10,
                                           (p_osal_isr_func_t)time_callback_func, (void*)&arg_data);

    return osal_ret;
}

e_osal_return_t time06_03_InterruptUnregisterIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptUnregisterIsr(dev_handle, 0, (p_osal_isr_func_t)time_callback_func);

    return osal_ret;
}

e_osal_return_t time06_04_InterruptEnableIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptEnableIsr(dev_handle, 0);

    return osal_ret;
}

/* Note: Must be called just time06_04_InterruptEnableIsr */
e_osal_return_t time06_05_InterruptDisableIsr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptDisableIsr(dev_handle, 0);

    return osal_ret;
}

e_osal_return_t time06_06_InterruptIsISRContext(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool            result;

    osal_ret = R_OSAL_InterruptIsISRContext(&result);

    return osal_ret;
}

e_osal_return_t time06_07_InterruptSetIsrPriority(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_InterruptSetIsrPriority(dev_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE13);

    return osal_ret;
}

e_osal_return_t time07_01_IoGetNumOfDevices(void)
{
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    char            *device_type = "ims";
    size_t          numOfDevices;

    if (is_io_numget == false)
    {
        osal_ret = R_OSAL_IoGetNumOfDevices(device_type, &numOfDevices);
        if (osal_ret == OSAL_RETURN_OK)
        {
            is_io_numget = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time07_02_IoGetDeviceList(void)
{
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    char            *device_type = "ims";
    char            list[80]     = {0};
    size_t          numOfLists;

    if (is_io_devlistget == false)
    {
        osal_ret  = R_OSAL_IoGetDeviceList(device_type, list, sizeof(list), &numOfLists);
        if (osal_ret == OSAL_RETURN_OK)
        {
            is_io_devlistget = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time07_03_IoDeviceOpen(char *device)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    if (is_io_opened == false)
    {
        osal_ret = R_OSAL_IoDeviceOpen(device, &dev_handle);
        if (osal_ret == OSAL_RETURN_OK)
        {
            is_io_opened = true;
        }
    }

    return osal_ret;
}

e_osal_return_t time07_04_IoDeviceClose(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoDeviceClose(dev_handle);
    if (osal_ret == OSAL_RETURN_OK)
    {
        is_io_numget = false;
        is_io_devlistget = false;
        is_io_opened = false;
    }

    return osal_ret;
}

e_osal_return_t time07_05_IoRead8(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint8_t         read_data = 0;

    osal_ret = R_OSAL_IoRead8(dev_handle, 0x0030, &read_data);

    return osal_ret;
}

e_osal_return_t time07_06_IoRead16(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint16_t        read_data = 0;

    osal_ret = R_OSAL_IoRead16(dev_handle, 0x0030, &read_data);

    return osal_ret;
}

e_osal_return_t time07_07_IoRead32(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint32_t        read_data = 0;

    osal_ret = R_OSAL_IoRead32(dev_handle, 0x0030, &read_data);

    return osal_ret;
}

e_osal_return_t time07_08_IoRead64(void)
{
    e_osal_return_t osal_ret  = OSAL_RETURN_OK;
    uint64_t        read_data = 0;

    osal_ret = R_OSAL_IoRead64(dev_handle, 0x0030, &read_data);
    
    return osal_ret;
}

e_osal_return_t time07_09_IoWrite8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite8(dev_handle, 0x0030, 0xFF);

    return osal_ret;
}

e_osal_return_t time07_10_IoWrite16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite16(dev_handle, 0x0030, 0x00FF);
    
    return osal_ret;
}

e_osal_return_t time07_11_IoWrite32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite32(dev_handle, 0x0030, 0x000000FF);

    return osal_ret;
}

e_osal_return_t time07_12_IoWrite64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_IoWrite64(dev_handle, 0x0030, 0x00000000000000FF);

    return osal_ret;
}

e_osal_return_t time07_13_IoBlockRead8(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint8_t         read_data[4];

    osal_ret = R_OSAL_IoBlockRead8(dev_handle, 0x0030, read_data, sizeof(read_data));

    return osal_ret;
}

e_osal_return_t time07_14_IoBlockRead16(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint16_t        read_data[4];

    osal_ret = R_OSAL_IoBlockRead16(dev_handle, 0x0030, read_data, sizeof(read_data));

    return osal_ret;
}

e_osal_return_t time07_15_IoBlockRead32(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t        read_data[4];

    osal_ret = R_OSAL_IoBlockRead32(dev_handle, 0x0030, read_data, sizeof(read_data));

    return osal_ret;
}

e_osal_return_t time07_16_IoBlockRead64(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint64_t                read_data[4];

    osal_ret = R_OSAL_IoBlockRead64(dev_handle, 0x0030, read_data, sizeof(read_data));
    
    return osal_ret;
}

e_osal_return_t time07_17_IoBlockWrite8(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint8_t         write_data[2] = {0x0F, 0xFF};

    osal_ret = R_OSAL_IoBlockWrite8(dev_handle, 0x0030, write_data, sizeof(write_data));

    return osal_ret;
}

e_osal_return_t time07_18_IoBlockWrite16(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint16_t        write_data[2] = {0x000F, 0x00FF};

    osal_ret = R_OSAL_IoBlockWrite16(dev_handle, 0x0030, write_data, sizeof(write_data));

    return osal_ret;
}

e_osal_return_t time07_19_IoBlockWrite32(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint32_t        write_data[2] = {0x0000000F, 0x000000FF};

    osal_ret = R_OSAL_IoBlockWrite32(dev_handle, 0, write_data, sizeof(write_data));

    return osal_ret;
}

e_osal_return_t time07_20_IoBlockWrite64(void)
{
    e_osal_return_t osal_ret      = OSAL_RETURN_OK;
    uint64_t        write_data[2] = {0x000000000000000F, 0x00000000000000FF};

    osal_ret = R_OSAL_IoBlockWrite64(dev_handle, 0x0030, write_data, sizeof(write_data));
    
    return osal_ret;
}

e_osal_return_t time07_21_IoGetDeviceAxiBusId(void)
{
    e_osal_return_t   osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id;

    osal_ret = R_OSAL_IoGetDeviceAxiBusId(dev_handle, &axi_id);

    return osal_ret;
}

e_osal_return_t time07_22_IoGetAxiBusIdFromDeviceName(void)
{
    e_osal_return_t   osal_ret = OSAL_RETURN_OK;
    char              *device  = "ims_00";
    osal_axi_bus_id_t axi_id;

    osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName(device, &axi_id);

    return osal_ret;
}

e_osal_return_t time07_23_IoGetAxiBusNameList(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            list[80] = {0};
    size_t          num_of_byte;

    osal_ret = R_OSAL_IoGetAxiBusNameList(list, sizeof(list), &num_of_byte);

    return osal_ret;
}

e_osal_return_t time07_24_IoGetAxiBusName(void)
{
    e_osal_return_t     osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t   axi_id   = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;
    char                name[200];
    size_t              num_of_byte;

    osal_ret = R_OSAL_IoGetAxiBusName(axi_id, name, sizeof(name), &num_of_byte);

    return osal_ret;
}

e_osal_return_t time07_25_IoGetAxiIdListForDeviceHdl(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t array[10];
    size_t  array_size = 10;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForDeviceHdl(dev_handle, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_26_IoGetAxiIdListForDeviceName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            *device  = "ims_00";
    osal_axi_bus_id_t array[10];
    size_t  array_size = 10;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForDeviceName(device, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_27_IoGetAxiIdListForAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char axi_bus_name[] = "vc_00_04";
    osal_axi_bus_id_t array[10];
    size_t  array_size = 10;
    size_t  num_of_written_axi_ids[10];

    osal_ret = R_OSAL_IoGetAxiIdListForAxiBusName(axi_bus_name, &array, array_size, &num_of_written_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_28_IoGetNumOfAxiIdsForDeviceHdl(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(dev_handle, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_29_IoGetNumOfAxiIdsForDeviceName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char            *device  = "imr_00";
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForDeviceName(device, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t time07_30_IoGetNumOfAxiIdsForAxiBusName(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char axi_bus_name[] = "vc_00_04";
    size_t num_of_axi_ids;

    osal_ret = R_OSAL_IoGetNumOfAxiIdsForAxiBusName(axi_bus_name, &num_of_axi_ids);

    return osal_ret;
}

e_osal_return_t time08_01_PmSetRequiredState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmSetRequiredState(dev_handle, OSAL_PM_REQUIRED_STATE_RELEASED, true);

    return osal_ret;
}

e_osal_return_t time08_02_PmGetRequiredState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_required_state_t req_state;

    osal_ret = R_OSAL_PmGetRequiredState(dev_handle, &req_state);

    return osal_ret;
}

e_osal_return_t time08_03_PmWaitForState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmWaitForState(dev_handle, OSAL_PM_STATE_CG, 1000);

    return osal_ret;
}

e_osal_return_t time08_04_PmGetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t pm_state;

    osal_ret = R_OSAL_PmGetState(dev_handle, &pm_state);

    return osal_ret;
}

e_osal_return_t time08_05_PmGetLowestIntermediatePowerState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_state_t pm_state;

    osal_ret = R_OSAL_PmGetLowestIntermediatePowerState(dev_handle, &pm_state);

    return osal_ret;
}

e_osal_return_t time08_06_PmSetPolicy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmSetPolicy(dev_handle, OSAL_PM_POLICY_CG, true);

    return osal_ret;
}

e_osal_return_t time08_07_PmGetPolicy(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_policy_t pm_policy;

    osal_ret = R_OSAL_PmGetPolicy(dev_handle, &pm_policy);

    return osal_ret;
}

e_osal_return_t time08_08_PmSetResetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_PmSetResetState(dev_handle, OSAL_PM_RESET_STATE_RELEASED);

    return osal_ret;
}

e_osal_return_t time08_09_PmGetResetState(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    e_osal_pm_reset_state_t reset_state;

    osal_ret = R_OSAL_PmGetResetState(dev_handle, &reset_state);

    return osal_ret;
}

e_osal_return_t time08_10_PmSetPostClock(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    bool     clock_enable = false;
    uint32_t clock_driver = 1;

    osal_ret = R_OSAL_PmSetPostClock(dev_handle, clock_enable, clock_driver);

    return osal_ret;
}

e_osal_return_t time09_01_MmngrOpen(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    if (mmgr_handle == NULL)
    {
        config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
        config.memory_limit                = 4096;
        config.max_allowed_allocations     = 16;
        config.max_registered_monitors_cbs = 100;

        osal_ret = R_OSAL_MmngrOpen(&config, &mmgr_handle);
    }

    return osal_ret;
}

e_osal_return_t time09_02_MmngrClose(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrClose(mmgr_handle);
    mmgr_handle = NULL;

    return osal_ret;
}
    
e_osal_return_t time09_03_MmngrAlloc(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t size      = 0x80;
    size_t alignment = 0x40;

    if (buf_handle == NULL)
    {
        osal_ret = R_OSAL_MmngrAlloc(mmgr_handle, size, alignment, &buf_handle);
    }

    return osal_ret;
}

e_osal_return_t time09_04_MmngrDealloc(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDealloc(mmgr_handle, buf_handle);
    buf_handle = NULL;

    return osal_ret;
}

e_osal_return_t time09_05_MmngrGetConfig(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED; 
    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrGetConfig(mmgr_handle, &config);

    return osal_ret;
}

e_osal_return_t time09_06_MmngrGetOsalMaxConfig(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED; 
    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&config);

    return osal_ret;
}

e_osal_return_t time09_07_MmngrInitializeMmngrConfigSt(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    st_osal_mmngr_config_t config;

    config.mode                        = OSAL_MMNGR_ALLOC_MODE_STACK;
    config.memory_limit                = OSAL_MEMORY_MANAGER_CFG_UNLIMITED;
    config.max_allowed_allocations     = OSAL_MEMORY_MANAGER_CFG_UNLIMITED; 
    config.max_registered_monitors_cbs = 100;

    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(&config);

    return osal_ret;
}

void time_callback_mmgr(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
    return;
}

e_osal_return_t time09_08_MmngrRegisterMonitor(void)
{
    e_osal_return_t      osal_ret        = OSAL_RETURN_OK;
    e_osal_mmngr_event_t monitored_event = OSAL_MMNGR_EVENT_ALLOCATE;
    void* user_arg;

    osal_ret = R_OSAL_MmngrRegisterMonitor(mmgr_handle, monitored_event, time_callback_mmgr, &user_arg);

    return osal_ret;
}

e_osal_return_t time09_09_MmngrGetCpuPtr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    void            *cpu_ptr;

    osal_ret = R_OSAL_MmngrGetCpuPtr(buf_handle, &cpu_ptr);

    return osal_ret;
}

e_osal_return_t time09_10_MmngrGetConstCpuPtr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    const void      *cpu_ptr;

    osal_ret = R_OSAL_MmngrGetConstCpuPtr(buf_handle, &cpu_ptr);

    return osal_ret;
}

e_osal_return_t time09_11_MmngrFlush(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          offset   = 0x00;
    size_t          size     = 0x40;

    osal_ret = R_OSAL_MmngrFlush(buf_handle, offset, size);

    return osal_ret;
}

e_osal_return_t time09_12_MmngrInvalidate(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          offset   = 0x00;
    size_t          size     = 0x40;

    osal_ret = R_OSAL_MmngrInvalidate(buf_handle, offset, size);

    return osal_ret;
}

e_osal_return_t time09_13_MmngrGetHwAddr(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_axi_bus_id_t axi_id = OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY;
    uintptr_t hw_addr;

    osal_ret = R_OSAL_MmngrGetHwAddr(buf_handle, axi_id, &hw_addr);

    return osal_ret;
}

e_osal_return_t time09_14_MmngrGetSize(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          size;

    osal_ret = R_OSAL_MmngrGetSize(buf_handle, &size);

    return osal_ret;
}

e_osal_return_t time09_15_MmngrCheckBuffer(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrCheckBuffer(buf_handle);

    return osal_ret;
}

e_osal_return_t time09_16_MmngrDebugMmngrPrint(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDebugMmngrPrint(mmgr_handle, stdout);

    return osal_ret;
}

e_osal_return_t time09_17_MmngrDebugBufferPrint(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;

    osal_ret = R_OSAL_MmngrDebugBufferPrint(buf_handle, stdout);

    return osal_ret;
}

e_osal_return_t time09_18_MmngrInitializeMemoryManagerObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_manager_t tmp_manager;

    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&tmp_manager);

    return osal_ret;
}

e_osal_return_t time09_19_MmngrInitializeMemoryBufferObj(void)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_memory_buffer_t tmp_buffer;

    osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&tmp_buffer);

    return osal_ret;
}
