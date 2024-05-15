/***********************************************************************************************************************
* File Name :    main.c
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Test program for OSAL Linux IT
***********************************************************************************************************************/
#ifndef OSAL_TEST_H
#define OSAL_TEST_H

#define PRINT(str)              printf("=====START:" #str "\n")
#define INTERVAL_TIME           (1 * 1000 * 1000)
#define MEMORY_USE_AMOUNT(info) (info.hblkhd + info.uordblks)

#define OSAL_FUNCTION_CALL(call, osal_ret)                                                              \
{                                                                                                       \
    struct mallinfo api_mem[2];                                                                         \
                                                                                                        \
    PRINT(call);                                                                                        \
                                                                                                        \
    api_mem[0] = mallinfo();                                                                            \
    osal_ret = call;                                                                                    \
    api_mem[1] = mallinfo();                                                                            \
                                                                                                        \
    printf("Before %s: %d\n", #call, MEMORY_USE_AMOUNT(api_mem[0]));                                    \
    printf("After  %s: %d\n", #call, MEMORY_USE_AMOUNT(api_mem[1]));                                    \
    printf("Use in %s: %d\n", #call, MEMORY_USE_AMOUNT(api_mem[1]) - MEMORY_USE_AMOUNT(api_mem[0]));    \
                                                                                                        \
    if (OSAL_RETURN_OK == osal_ret)                                                                     \
    {                                                                                                   \
        test_pass++;                                                                                    \
    }                                                                                                   \
    else                                                                                                \
    {                                                                                                   \
        test_fail++;                                                                                    \
        printf("%s(%d) osal_ret=%d\n", __func__, __LINE__, osal_ret);                                   \
    }                                                                                                   \
                                                                                                        \
    usleep(INTERVAL_TIME);                                                                              \
    printf("======END=====\n");                                                                         \
}                                                                                                       \

#define LOOP_MAX(ci)                    ((0 == ci) ? 50 : 1)
#define MEMORY_CONTINUOUS_CHECK_COUNT   (5)
#define MEMORY_CONTINUOUS_CHECK(check_array)                                                            \
{                                                                                                       \
    uint64_t cnt;                                                                                       \
                                                                                                        \
    for (cnt = 0; cnt < sizeof(check_array) / sizeof(check_array[0]); cnt++)                            \
    {                                                                                                   \
        if (MEMORY_USE_AMOUNT(check_array[0]) != MEMORY_USE_AMOUNT(check_array[cnt]))                   \
        {                                                                                               \
            break;                                                                                      \
        }                                                                                               \
    }                                                                                                   \
                                                                                                        \
    if (sizeof(check_array) / sizeof(check_array[0]) <= cnt)                                            \
    {                                                                                                   \
        loop_cnt++;                                                                                     \
        break;                                                                                          \
    }                                                                                                   \
}                                                                                                       \

#endif /* OSAL_TEST_H */
