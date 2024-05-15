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

#define OSAL_FUNCTION_CALL(call, osal_ret)                                                              \
{                                                                                                       \
    PRINT(call);                                                                                        \
                                                                                                        \
    osal_ret = call;                                                                                    \
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

#define LOOP_MAX(ci)                    (1)
#define MEMORY_CONTINUOUS_CHECK_COUNT   (1)
#define MEMORY_CONTINUOUS_CHECK(check_array)                                                            \
{                                                                                                       \
    /* Do nothing */                                                                                    \
}                                                                                                       \

#endif /* OSAL_TEST_H */
