#ifndef _VALIDATOR_H_
#define _VALIDATOR_H_
#include "testenv.h"
#include "base.h"
#include "params.h"

#define TEST_VALIDATOR_BOOLVALUE                (0x0000000000000001ULL)
#define TEST_VALIDATOR_I32VALUE                 (0x0000000000000002ULL)
#define TEST_VALIDATOR_I64VALUE                 (0x0000000000000004ULL)
#define TEST_VALIDATOR_U32VALUE                 (0x0000000000000008ULL)
#define TEST_VALIDATOR_U64VALUE                 (0x0000000000000010ULL)
#define TEST_VALIDATOR_IMPDRV_RETCODE           (0x0000000000000020ULL)
#define TEST_VALIDATOR_MEMORY                   (0x0000000000000040ULL)
#define TEST_VALIDATOR_STATE                    (0x0000000000000080ULL)
#define TEST_ADDR_NOT_CHANGE                    (0x0000000000000100ULL)
#define TEST_VALIDATOR_ADDR                     (0x0000000000000200ULL)
#define TEST_VALIDATOR_STRING                   (0x0000000000000400ULL)
#define TEST_VALIDATOR_IMPDRV_POLICY            (0x0000000000000800ULL)
#define TEST_VALIDATOR_IMPDRV_PARAM             (0x0000000000001000ULL)
#define TEST_VALIDATOR_IMPDRV_STATE             (0x0000000000002000ULL)
#define TEST_VALIDATOR_VERSION                  (0x0000000000004000ULL)
#define TEST_VALIDATOR_VERSION_MAJOR            (0x0000000000008000ULL)
#define TEST_VALIDATOR_VERSION_MINOR            (0x0000000000010000ULL)
#define TEST_VALIDATOR_VERSION_PATCH            (0x0000000000020000ULL)

#define CHECK_BIT(validator, bit)	(((validator >> bit) & 0x1) != 0)

struct TestValidatePointerInfo
{
    uint32_t addr_value;
    char addr_name[100];
};

bool TEST_CheckBitValidator(bool validator[], uint32_t index);
bool TEST_ValidateOutput(const void *output, const void *expected, uint32_t size, const char *info, uint64_t validator);

void TEST_init_validate_pointer_info(void);
bool Test_set_validate_pointer_info(uint32_t addr_value, char *addr_name);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void TEST_SetSkipValidator(uint64_t skipValidator);

void TEST_ValidateResult(bool b, struct TestParams *params);

#ifdef __cplusplus
}
#endif

#endif
