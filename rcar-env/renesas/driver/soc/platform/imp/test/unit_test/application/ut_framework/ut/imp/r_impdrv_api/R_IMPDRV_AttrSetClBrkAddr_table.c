#include "testenv.h"

TEST_CASE_F(UT, R_IMPDRV_AttrSetClBrkAddr, V4H2, R_IMPDRV_AttrSetClBrkAddr_0001) { EXPECT_EQ( true, TEST_R_IMPDRV_AttrSetClBrkAddr("PCL", 1)); }

struct TestCase UT_R_IMPDRV_AttrSetClBrkAddr_All_Tests[] = {
	TEST_CASE_T(UT, R_IMPDRV_AttrSetClBrkAddr, V4H2, R_IMPDRV_AttrSetClBrkAddr_0001, R_IMPDRV_AttrSetClBrkAddr_0001),
	TEST_CASE_END
};
