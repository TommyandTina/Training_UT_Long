/********************************************************************************
    Atomic Library
     Copyright (C) 2016-2018 Renesas Electronics Corporation, all rights reserved.

    [File] validator.c

********************************************************************************/
#include <stdlib.h>
#include "testenv.h"
#include "base.h"
#include "validator.h"
#include "file.h"
#include "logger.h"
#include "drv_def.h"
#define ENUM_TO_STRING(a) {a, #a}

static uint64_t m_validator = 0xffffffffffffffff;

struct TestValidatePointerInfo test_validate_pointer_info[TEST_MACRO_MAX];

void TEST_init_validate_pointer_info(void)
{
    (void)memset(test_validate_pointer_info, 0, sizeof(test_validate_pointer_info));
}

bool Test_set_validate_pointer_info(uint32_t addr_value, char *addr_name)
{
    int i;
    bool ret = false;

    if (!!addr_name)
    {
        for (i = 0; i < ARRAY_COUNT(test_validate_pointer_info); i++)
        {
            /* check if the addr_value is existing in the list */
            if (test_validate_pointer_info[i].addr_value != addr_value)
            {
                /* find an available index to store the info */
                if (!test_validate_pointer_info[i].addr_value)
                {
                    (void)memcpy(test_validate_pointer_info[i].addr_name, addr_name, sizeof(test_validate_pointer_info[i].addr_name));
                    test_validate_pointer_info[i].addr_value = addr_value;
                    ret = true;
                    break;
                }
            }
            else
            {
                /* the addr_value is existing, return true */
                ret = true;
                break;
            }
        }
    }
    else
    {
        printf("Error! Invalid addr_name\n");
    }
    
    return ret;
}

static void TEST_int32_tToString(int32_t value, char *string)
{
    char name[32] = {0};
    sprintf(name, "0x%08X\0", value);
    strcpy(string, name);
}

static void TEST_int64_tToString(int64_t value, char *string)
{
    char name[32] = {0};
    sprintf(name, "0x%016lX\0", value);
    strcpy(string, name);
}

static void TEST_uint32_tToString(uint32_t value, char *string)
{
    char name[32] = {0};
    sprintf(name, "0x%08X\0", value);
    strcpy(string, name);
}

static void TEST_uint64_tToString(uint64_t value, char *string)
{
    char name[32] = {0};
    sprintf(name, "0x%016lX\0", value);
    strcpy(string, name);
}

static void TEST_boolToString(bool value, char *string)
{
    if (value)
    {
        strcpy(string, "true\0");
    }
    else
    {
        strcpy(string, "false\0");
    }
}

static void TEST_AddrToString(int32_t ret, char *string)
{
    int i;
    bool is_found = false;

    for (i = 0; i < ARRAY_COUNT(test_validate_pointer_info); i++)
    {
        if (ret == (int32_t)test_validate_pointer_info[i].addr_value)
        {
            strcpy(string, test_validate_pointer_info[i].addr_name);
            is_found = true;
            break;
        }
    }
    
    if (!is_found)
    {
        TEST_int32_tToString(ret, string);
    }
}

static void TEST_NameToString(const char *ret, char *string)
{
    char name[32] = {0};
    sprintf(name, "\"%s\"\0", ret);
    strcpy(string, name);
}

static void TEST_ValidateMemory(bool b, const void *output, const void *expected, const char *name)
{
    if (b)
    {
        VLOG("<OK> %s [output] %s(%d) <==> [expected] %s(%d)", name,
            "not change", output,
            "not change", expected);
    }
    else
    {
        ELOG("<NG> %s [output] %s(%d) <==> [expected] %s(%d)", name,
            "changed", output,
            "changed", expected);
    }
}

static void TEST_ValidateAddr(bool b, const int32_t output, const int32_t expected, const char *name)
{
    char str_output[64];
    char str_expected[64];

    (void)TEST_AddrToString(output, str_output);
    (void)TEST_AddrToString(expected, str_expected);

    if (b)
    {
        VLOG("<OK> %s [output] %s(%d) <==> [expected] %s(%d)", name,
            str_output, output,
            str_expected, expected);
    }
    else
    {
        ELOG("<NG> %s [output] %s(%d) <==> [expected] %s(%d)", name,
            str_output, output,
            str_expected, expected);
    }
}

static void TEST_ValidateName(bool b, const char *output, const char *expected, const char *name)
{
    char str_output[64];
    char str_expected[64];

    (void)TEST_NameToString(output, str_output);
    (void)TEST_NameToString(expected, str_expected);

    if (b)
    {
        VLOG("<OK> %s [output] %s(\"%s\") <==> [expected] %s(\"%s\")", name,
            str_output, output,
            str_expected, expected);
    }
    else
    {
        ELOG("<NG> %s [output] %s(\"%s\") <==> [expected] %s(\"%s\")", name,
            str_output, output,
            str_expected, expected);
    }
}

// PRINT_VALIDATE_INFO_DECLARE(e_impdrv_api_retcode_t)
#define PRINT_VALIDATE_INFO_DECLARE(type) \
    static void TEST_Validate##type(bool result, type output, type expected, const char *name) \
    { \
        char str_output[64] = {0}; \
        char str_expected[64] = {0}; \
        (void)TEST_##type##ToString(output, str_output); \
        (void)TEST_##type##ToString(expected, str_expected); \
        if (result) \
        { \
            VLOG("<OK> %s [output] %s(%d) <==> [expected] %s(%d)", name, \
                    str_output, output, \
                    str_expected, expected \
                ); \
        } \
        else \
        { \
            ELOG("<NG> %s [output] %s(%d) <==> [expected] %s(%d)", name, \
                    str_output, output, \
                    str_expected, expected \
                ); \
        } \
    }

#define PRINT_VALIDATE_INFO_CALL(type, result, output, expected, name) \
    TEST_Validate##type(result, *(type *)output, *(type *)expected, name)


bool TEST_CheckBitValidator(bool validator[], uint32_t index)
{
    return validator[index];
}

struct ImpDrvVALUE_TO_STRING {
    int64_t ret;
    const char *name;
};

struct ImpDrvVALUE_TO_STRING_TABLE {
    uint64_t validator;
    struct ImpDrvVALUE_TO_STRING data;
};

static const struct ImpDrvVALUE_TO_STRING_TABLE m_impdrv_tableNames[] =
{
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_OK) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_CHECKFAIL) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_SEQSTATE) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_ARGNULL) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_PARAM) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_NOTSUPPORT) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_ATTRIBUTE) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_SYSTEMERROR) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_PMSYSTEMERROR) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_INSTANCE) },
    { TEST_VALIDATOR_IMPDRV_RETCODE, ENUM_TO_STRING(IMPDRV_EC_NG_DSP_HALT) },
    { TEST_VALIDATOR_IMPDRV_POLICY, ENUM_TO_STRING(IMPDRV_PM_POLICY_CG) },
    { TEST_VALIDATOR_IMPDRV_POLICY, ENUM_TO_STRING(IMPDRV_PM_POLICY_PG) },
    { TEST_VALIDATOR_IMPDRV_POLICY, ENUM_TO_STRING(IMPDRV_PM_POLICY_HP) },
    { TEST_VALIDATOR_IMPDRV_PARAM, ENUM_TO_STRING(IMPDRV_PARAM_OFF) },
    { TEST_VALIDATOR_IMPDRV_PARAM, ENUM_TO_STRING(IMPDRV_PARAM_ON) },
    { TEST_VALIDATOR_IMPDRV_STATE, ENUM_TO_STRING(IMPDRV_STATE_UNINIT) },
    { TEST_VALIDATOR_IMPDRV_STATE, ENUM_TO_STRING(IMPDRV_STATE_INIT) },
    { TEST_VALIDATOR_IMPDRV_STATE, ENUM_TO_STRING(IMPDRV_STATE_READY) },
    { TEST_VALIDATOR_IMPDRV_STATE, ENUM_TO_STRING(IMPDRV_STATE_EXEC) },
    { TEST_VALIDATOR_IMPDRV_STATE, ENUM_TO_STRING(IMPDRV_STATE_INT) },
    { TEST_VALIDATOR_VERSION_MAJOR, ENUM_TO_STRING(IMPDRV_VERSION_MAJOR) },
    { TEST_VALIDATOR_VERSION_MINOR, ENUM_TO_STRING(IMPDRV_VERSION_MINOR) },
    { TEST_VALIDATOR_VERSION_PATCH, ENUM_TO_STRING(IMPDRV_VERSION_PATCH) },
};

static void TEST_ImpValueToString(uint64_t validator, int64_t ret, char *string)
{
    for (int i = 0; i < ARRAY_COUNT(m_impdrv_tableNames); i++)
    {
        if (validator & m_impdrv_tableNames[i].validator)
        {
            if (ret == m_impdrv_tableNames[i].data.ret)
            {
                strcpy(string, m_impdrv_tableNames[i].data.name);
                return;
            }
            else
            {
                /* The validator is not register with table */
                continue;
            }
        }
    }
    TEST_int32_tToString((int32_t)ret, string);
}

static void TEST_ValidateImpValue(uint64_t validator, bool result, int64_t output, int64_t expected, const char *name)
{
    char str_output[64] = {0};
    char str_expected[64] = {0};

    (void)TEST_ImpValueToString(validator, output, str_output);
    (void)TEST_ImpValueToString(validator, expected, str_expected);

    if (result)
    {
        VLOG("<OK> %s [output] %s(%lld) <==> [expected] %s(%lld)", name,
            str_output, output,
            str_expected, expected);
    }
    else
    {
        ELOG("<NG> %s [output] %s(%lld) <==> [expected] %s(%lld)", name,
            str_output, output,
            str_expected, expected);
    }
}

PRINT_VALIDATE_INFO_DECLARE(bool)
PRINT_VALIDATE_INFO_DECLARE(int32_t)
PRINT_VALIDATE_INFO_DECLARE(int64_t)
PRINT_VALIDATE_INFO_DECLARE(uint32_t)
PRINT_VALIDATE_INFO_DECLARE(uint64_t)

bool TEST_ValidateOutput(const void *output, const void *expected, uint32_t size, const char *info, uint64_t validator)
{
    bool b = false;
    int64_t int_output = 0;
    int64_t int_expected = 0;

    if (!!(validator & TEST_VALIDATOR_STRING))
    {
        if (!output || !expected)
        {
            ELOG("%s", "Error !! Invalid validation, output and expected must not be null !!");
        }
        else if ((*(int32_t *)output   == 0xCC) ||
                 (*(int32_t *)expected == 0XCC))
        {
            if ((int32_t)output == (int32_t)expected)
            {
                b = true;
            }
            (void)TEST_Validateint32_t(b, (int32_t)output, (int32_t)expected, info);
        }
        else
        {
            if (strcmp((char *)output, (char *)expected) == 0)
            {
                b = true;
            }
            (void)TEST_ValidateName(b, (char *)output, (char *)expected, info);
        }
        return b;
    }

    if (size <= 1)
    {
        int_output = *(int8_t *)output;
        int_expected = *(int8_t *)expected;
    }
    else if (size <= 4)
    {
        int_output = *(int32_t *)output;
        int_expected = *(int32_t *)expected;
    }
    else
    {
        int_output = *(int64_t *)output;
        int_expected = *(int64_t *)expected;
    }

    if (!output || !expected)
    {
        ELOG("%s", "Error !! Invalid validation, output and expected must not be null !!");
    }
    else
    {
        if (memcmp(output, expected, size) == 0)
        {
            b = true;
        }

        do {
            if (validator & TEST_VALIDATOR_BOOLVALUE)
            {
                PRINT_VALIDATE_INFO_CALL(bool, b, output, expected, info);
            }
            else if (validator & TEST_VALIDATOR_I32VALUE)
            {
                PRINT_VALIDATE_INFO_CALL(int32_t, b, output, expected, info);
            }
            else if (validator & TEST_VALIDATOR_I64VALUE)
            {
                PRINT_VALIDATE_INFO_CALL(int64_t, b, output, expected, info);
            }
            else if (validator & TEST_VALIDATOR_U32VALUE)
            {
                PRINT_VALIDATE_INFO_CALL(uint32_t, b, output, expected, info);
            }
            else if (validator & TEST_VALIDATOR_U64VALUE)
            {
                PRINT_VALIDATE_INFO_CALL(uint64_t, b, output, expected, info);
            }
            else if (validator & TEST_VALIDATOR_MEMORY)
            {
                TEST_ValidateMemory(b, output, expected, info);
            }
            else if (validator & TEST_VALIDATOR_ADDR)
            {
                TEST_ValidateAddr(b, *(int32_t *)output, *(int32_t *)expected, info);
            }
            else
            {
                TEST_ValidateImpValue(validator, b, int_output, int_expected, info);
            }
        } while(0);
    }

    return b;
}

void TEST_SetSkipValidator(uint64_t skipValidator)
{
    m_validator &= ~skipValidator;
    printf("--skip-validator=%lx ===> validator: %lx\n", skipValidator, m_validator);
}

void TEST_ValidateResult(bool b, struct TestParams *params) {
    char *resCd[2];
    char resNG[3]="NG\0";
    char resOK[3]="OK\0";
    resCd[0]=&resNG[0];
    resCd[1]=&resOK[0];

    VLOG("[ RESULT   ] %s-%04d: %s", params->func, params->no, resCd[b]);

}
