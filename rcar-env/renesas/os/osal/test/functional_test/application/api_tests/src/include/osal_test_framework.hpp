#ifndef osal_test_framework
#define osal_test_framework

#include "osal_test_framework_helper.hpp"
#include "rcar-xos/osal/r_osal.h"
#include <stdint.h>

/* Enum definition */
typedef enum compare_type
{
    EQUAL,
    STR_EQUAL,
    NOT_EQUAL,
    STR_NOT_EQUAL,
    TRUE,
    FALSE,
    GREATE,
    GREATE_OR_EQUAL,
    LESS,
    LESS_OR_EQUAL,
    FAIL_WITHOUT_EXPR,
    INVALID
} compare_type_t;

typedef enum fatal_type
{
    NON_FATAL,
    FATAL
} fatal_type_t;

/* Common definition */
#define DISALLOW_ASSIGN(type) \
void operator=(type const &) = delete
#define DISALLOW_COPY_AND_ASSIGN(type) \
type(type const &) = delete; \
DISALLOW_ASSIGN(type)
#define GENERATE_STRING(STRING) #STRING
#define MAKE_TEST_FIXTURE_NAME(test_suite, test_num) test_suite##_##test_num##_Test
#define FIRST_ARG_(N, ...) N
#define GENERATE_FIST_ARG(args) FIRST_ARG_ args
#define SEC_ARG_(N, M, ...) M
#define GENERATE_SECOND_ARG(args) SEC_ARG_ args
#define MAX_ELEMENT_IN_FILTER_OPTION 100
#define MAX_OF_NUM_PARAMETERIZED_TEST 100

/* Parent class of test cases using OSAL test framework.
* All class of new test cases must inherit this class.
*/
class OsalTestFrameWork
{
public:
    OsalTestFrameWork(){};
    virtual ~OsalTestFrameWork(){};
    virtual void test_body(){};
    virtual void SetUp(){};
    virtual void TearDown(){};
    static void SetUpTestSuite(){};
    static void TearDownTestSuite(){};
};

class OsalTestFrameWorkParameterizedTest : public OsalTestFrameWork
{
private:
    DISALLOW_COPY_AND_ASSIGN(OsalTestFrameWorkParameterizedTest);
public:
    struct parameterized_test_info
    {
        char test_suite_name[100];
        OsalFWHelper::pair<int, int> range;
        int num_of_loop;
    };

    size_t test_instantiation_count = 0;
    OsalFWHelper::multimap<const char*, parameterized_test_info> test_list;
    OsalTestFrameWorkParameterizedTest(){};
    ~OsalTestFrameWorkParameterizedTest(){};
    virtual void test_body(){};
    virtual void SetUp(){};
    virtual void TearDown(){};
    static void SetUpTestSuite(){};
    static void TearDownTestSuite(){};
    virtual int GetParam(void){};
    static OsalTestFrameWorkParameterizedTest & get(void)
    {
        static OsalTestFrameWorkParameterizedTest instance;
        return instance;
    }
};

// Concatenate strings
#define STR_CAT(A,B) STR_CAT_IMPL(A,B)
#define STR_CAT_IMPL(A,B) A##B

// Make test child fixture class for each test case
#define MAKE_CLASS_FIXTURE(test_suite, test_num, has_namespace) STR_CAT(MAKE_CLASS_FIXTURE_, has_namespace)(test_suite, test_num)
#define MAKE_CLASS_FIXTURE_0(test_suite, test_num) \
class MAKE_TEST_FIXTURE_NAME(test_suite, test_num) : public test_suite
#define MAKE_CLASS_FIXTURE_1(test_suite, test_num) \
namespace test_suite##_##test_num \
{ \
    class test_suite : public OsalTestFrameWork \
    { \
    public: \
        test_suite(){} \
        ~test_suite(){} \
    }; \
} \
class MAKE_TEST_FIXTURE_NAME(test_suite, test_num) : public test_suite##_##test_num::test_suite

// TEST_F_PRO definition
#define TEST_F_PRO(test_suite, test_num, has_namespace) \
MAKE_CLASS_FIXTURE(test_suite, test_num, has_namespace) \
{ \
public: \
    MAKE_TEST_FIXTURE_NAME(test_suite, test_num)(){} \
    DISALLOW_COPY_AND_ASSIGN(MAKE_TEST_FIXTURE_NAME(test_suite, test_num)); \
    virtual void test_body(); \
    void SetUp(){test_suite::SetUp();} \
    void TearDown(){test_suite::TearDown();} \
    static void SetUpTestSuite(){test_suite::SetUpTestSuite();} \
    static void TearDownTestSuite(){test_suite::TearDownTestSuite();} \
    static bool const test_info; \
}; \
bool const MAKE_TEST_FIXTURE_NAME(test_suite, test_num)::test_info = \
TestCaseInfo<OsalTestFrameWork>::get().RegisterTestInfo<MAKE_TEST_FIXTURE_NAME(test_suite, test_num)> \
(new MAKE_TEST_FIXTURE_NAME(test_suite, test_num)(), \
GENERATE_STRING(test_suite), GENERATE_STRING(test_num)); \
void MAKE_TEST_FIXTURE_NAME(test_suite, test_num)::test_body()

/* TEST_F definition */
#define TEST_F(test_suite, test_num) TEST_F_PRO(test_suite, test_num, 0)

/* TEST definition */
#define TEST(test_suite, test_num) TEST_F_PRO(test_suite, test_num, 1)

/* TEST_P definition */
#define TEST_P(test_suite, test_num) \
class MAKE_TEST_FIXTURE_NAME(test_suite, test_num) \
: public test_suite \
{ \
public: \
    MAKE_TEST_FIXTURE_NAME(test_suite, test_num)(const char* inst_name) \
    { \
        local_inst_name = inst_name; \
    } \
    DISALLOW_COPY_AND_ASSIGN(MAKE_TEST_FIXTURE_NAME(test_suite, test_num)); \
    virtual void test_body(); \
    void SetUp(){test_suite::SetUp(); Init();} \
    void TearDown(){test_suite::TearDown();} \
    static void SetUpTestSuite(){test_suite::SetUpTestSuite();} \
    static void TearDownTestSuite(){test_suite::TearDownTestSuite();} \
    int GetParam() \
    { \
        int value = 0; \
        if((min + 1) <= max) \
        { \
            value = min; \
            min++; \
        } \
        else \
        { \
            LOG_ERROR("Out of range \n"); \
        } \
        return value; \
    } \
    void Init() \
    { \
        for(auto i = OsalTestFrameWorkParameterizedTest::get().test_list.begin(); i != OsalTestFrameWorkParameterizedTest::get().test_list.end(); i++) \
        { \
            if(!strcmp(local_inst_name, i->first)) \
            { \
                min = i->second.range.first; \
                max = i->second.range.second; \
                break; \
            } \
        } \
    } \
    static bool register_parameterized_test() \
    { \
        for(auto i = OsalTestFrameWorkParameterizedTest::get().test_list.begin(); i != OsalTestFrameWorkParameterizedTest::get().test_list.end(); i++) \
        { \
            if(!strcmp(GENERATE_STRING(test_suite), i->second.test_suite_name)) \
            { \
                TestCaseInfo<OsalTestFrameWork>::get().RegisterTestInfoTestP<MAKE_TEST_FIXTURE_NAME(test_suite, test_num)> \
                (new MAKE_TEST_FIXTURE_NAME(test_suite, test_num)(i->first), \
                i->first, GENERATE_STRING(test_suite), GENERATE_STRING(test_num), \
                i->second.num_of_loop); \
            } \
        } \
        return true; \
    } \
    static bool const test_info; \
    int min; \
    int max; \
    const char* local_inst_name; \
}; \
bool const MAKE_TEST_FIXTURE_NAME(test_suite, test_num)::test_info = \
MAKE_TEST_FIXTURE_NAME(test_suite, test_num)::register_parameterized_test(); \
void MAKE_TEST_FIXTURE_NAME(test_suite, test_num)::test_body()

/* Parameterized Test range setting */
#define INSTANTIATE_TEST_CASE_P(instance_name, test_suite, ...) \
class MAKE_TEST_FIXTURE_NAME(test_suite, instance_name) \
: public test_suite \
{ \
public: \
    MAKE_TEST_FIXTURE_NAME(test_suite, instance_name)(){} \
    DISALLOW_COPY_AND_ASSIGN(MAKE_TEST_FIXTURE_NAME(test_suite, instance_name)); \
    static MAKE_TEST_FIXTURE_NAME(test_suite, instance_name) & get(void) \
    { \
        static MAKE_TEST_FIXTURE_NAME(test_suite, instance_name) instance; \
        return instance; \
    } \
    bool set_range(int min, int max) \
    { \
        OsalTestFrameWorkParameterizedTest::parameterized_test_info test_inst_info; \
        for(auto i = OsalTestFrameWorkParameterizedTest::get().test_list.begin(); i != OsalTestFrameWorkParameterizedTest::get().test_list.end(); i++) \
        { \
            if(!strcmp(GENERATE_STRING(instance_name), i->first)) \
            { \
                matched = true; \
                break; \
            } \
        } \
        if(false == matched) \
        { \
            strcpy(test_inst_info.test_suite_name, GENERATE_STRING(test_suite)); \
            test_inst_info.range.first = min; \
            test_inst_info.range.second = max; \
            test_inst_info.num_of_loop = max - min; \
            if (test_inst_info.num_of_loop <= 0) \
            { \
                LOG_ERROR("Invalid range setting \n"); \
            } \
            OsalTestFrameWorkParameterizedTest::get().test_list.insert({GENERATE_STRING(instance_name), test_inst_info}); \
            OsalTestFrameWorkParameterizedTest::get().test_instantiation_count++; \
            if(MAX_OF_NUM_PARAMETERIZED_TEST <= OsalTestFrameWorkParameterizedTest::get().test_instantiation_count) \
            { \
                LOG_ERROR("The number of test suite exceed the limitation\n"); \
            } \
        } \
        else \
        { \
            LOG_ERROR("Test suite has been existing\n"); \
        } \
        return true; \
    } \
    static bool const set_range_result; \
    bool matched = false; \
}; \
bool const MAKE_TEST_FIXTURE_NAME(test_suite, instance_name)::set_range_result = \
MAKE_TEST_FIXTURE_NAME(test_suite, instance_name)::get().set_range(GENERATE_FIST_ARG(__VA_ARGS__), GENERATE_SECOND_ARG(__VA_ARGS__)); \
int dummy_##instance_name

/* Assertion definition */
#define EXPECT_EQ(target, expected_value) expect_pro(do_compare(target, expected_value, EQUAL, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_NE(target, expected_value) expect_pro(do_compare(target, expected_value, NOT_EQUAL, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_TRUE(expr) expect_pro(do_compare(expr, true, TRUE, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_FALSE(expr) expect_pro(do_compare(expr, false, FALSE, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_GT(target, expected_value) expect_pro(do_compare(target, expected_value, GREATE, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_GE(target, expected_value) expect_pro(do_compare(target, expected_value, GREATE_OR_EQUAL, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_LE(target, expected_value) expect_pro(do_compare(target, expected_value, LESS_OR_EQUAL, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_LT(target, expected_value) expect_pro(do_compare(target, expected_value, LESS, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_STRNE(target, expected_value) expect_pro(do_compare_str(target, expected_value, STR_NOT_EQUAL, __FILE__, __LINE__), NON_FATAL)
#define EXPECT_STREQ(target, expected_value) expect_pro(do_compare_str(target, expected_value, STR_EQUAL, __FILE__, __LINE__), NON_FATAL)
#define ASSERT_EQ(target, expected_value) expect_pro(do_compare(target, expected_value, EQUAL, __FILE__, __LINE__), FATAL)
#define ASSERT_NE(target, expected_value) expect_pro(do_compare(target, expected_value, NOT_EQUAL, __FILE__, __LINE__), FATAL)
#define ASSERT_GE(target, expected_value) expect_pro(do_compare(target, expected_value, GREATE_OR_EQUAL, __FILE__, __LINE__), FATAL)
#define ASSERT_STRNE(target, expected_value) expect_pro(do_compare_str(target, expected_value, STR_NOT_EQUAL, __FILE__, __LINE__), FATAL)
#define ASSERT_TRUE(expr) expect_pro(do_compare(expr, true, TRUE, __FILE__, __LINE__), FATAL)
#define ASSERT_FALSE(expr) expect_pro(do_compare(expr, false, FALSE, __FILE__, __LINE__), FATAL)
#define ASSERT_GT(target, expected_value) expect_pro(do_compare(target, expected_value, GREATE, __FILE__, __LINE__), FATAL)
#define ASSERT_LE(target, expected_value) expect_pro(do_compare(target, expected_value, LESS_OR_EQUAL, __FILE__, __LINE__), FATAL)
#define ASSERT_LT(target, expected_value) expect_pro(do_compare(target, expected_value, LESS, __FILE__, __LINE__), FATAL)
#define ASSERT_STREQ(target, expected_value) expect_pro(do_compare_str(target, expected_value, STR_EQUAL, __FILE__, __LINE__), FATAL)
#define ADD_FAILURE() expect_pro(do_compare(false, true, FAIL_WITHOUT_EXPR, __FILE__, __LINE__), NON_FATAL)
#define SUCCEED() {}
#define ASSERT_THAT(target, matcher) \
for(auto i : target) \
{ \
    OsalTestFrameworkMatcher::matcher_info::get().target_list.push_back(i); \
} \
expect_matcher(target, matcher, __FILE__, __LINE__, FATAL)

/* namespace for assertion with the matcher. Currently, only support "Not(Each(Ge))" */
namespace OsalTestFrameworkMatcher
{
    class matcher_info
    {
    private:
        matcher_info(){}
        DISALLOW_COPY_AND_ASSIGN(matcher_info);
        ~matcher_info(){}
    public:
        static matcher_info & get(void)
        {
            static matcher_info instance;
            return instance;
        }
        OsalFWHelper::vector<int64_t> target_list;
        int atleast_value = 0;
    };

    inline bool Ge(int value)
    {
        bool ret = true;

        for(auto i : matcher_info::get().target_list)
        {
            if(i < value)
            {
                ret = false;
                break;
            }
        }

        return ret;
    }

    inline bool Each(bool matcher)
    {
        return matcher;
    }

    inline bool Not(bool matcher)
    {
        matcher_info::get().target_list.clear();
        return !matcher;
    }

    inline matcher_info& AtLeast(int value)
    {
        matcher_info::get().atleast_value = value;
        return matcher_info::get();
    }

    class AnyMatcher {};

    static AnyMatcher _ = {};
}

/* Assertion streaming class */
class AssertionStreaming
{
public:
    AssertionStreaming(bool expr_result, fatal_type_t type)
    {
        if(!expr_result)
        {
            expression = expr_result;
            if(FATAL == type)
            {
#if !defined(UNSUPPORT_TEST_FW_ASSERT)
                exit(1);
#endif
            }
        }
    }

    AssertionStreaming operator<< (const char* msg)
    {
        if(!expression)
        {
            LOG_INFO("%s\n", msg);
        }

        return *this;
    }

    AssertionStreaming operator<< (size_t msg)
    {
        if(!expression)
        {
            LOG_INFO("%d\n", msg);
        }

        return *this;
    }

    AssertionStreaming operator<< (const st_osal_mmngr_config_t & obj)
    {
        if(!expression)
        {
            LOG_INFO("[mode: %d, mem limit: %dKB, max # alloc: %d, max event monitors: %d]\n", \
                obj.mode, (obj.memory_limit / 1024), obj.max_allowed_allocations, obj.max_registered_monitors_cbs);
        }

        return *this;
    }

private:
    bool expression = true;
};

/* Test judgement class */
class TestJudgement
{
private:
    TestJudgement(){}
    DISALLOW_COPY_AND_ASSIGN(TestJudgement);
    ~TestJudgement(){}
public:
    struct failed_test_information
    {
        char inst_name[100];
        char test_suite[100];
        char test_num[100];
    };
    size_t num_of_failed_expr = 0;
    size_t num_of_failed_test = 0;
    size_t num_of_pass_test = 0;
    OsalFWHelper::vector<failed_test_information> failed_test_info;

    static TestJudgement & get(void)
    {
        static TestJudgement instance;
        return instance;
    }

    bool check_test_result(const char* test_suite, char* test_num)
    {
        bool ret = false;

        if(0 != num_of_failed_expr)
        {
            if(strcmp(test_num, "SetUpTestSuite") && strcmp(test_num, "TearDownTestSuite"))
            {
                LOG_INFO("[  FAILED  ] %s.%s\n", test_suite, test_num);
            }
            num_of_failed_expr = 0;
        }
        else
        {
            if(strcmp(test_num, "SetUpTestSuite") && strcmp(test_num, "TearDownTestSuite"))
            {
                LOG_INFO("[       OK ] %s.%s\n", test_suite, test_num);
            }
            ret = true;
        }

        return ret;
    }

    bool check_test_result(const char* inst_name, const char* test_suite, char* test_num)
    {
        bool ret = false;

        if(0 != num_of_failed_expr)
        {
            if(strcmp(test_num, "SetUpTestSuite") && strcmp(test_num, "TearDownTestSuite"))
            {
                LOG_INFO("[  FAILED  ] %s/%s.%s\n", inst_name, test_suite, test_num);
            }
            num_of_failed_expr = 0;
        }
        else
        {
            if(strcmp(test_num, "SetUpTestSuite") && strcmp(test_num, "TearDownTestSuite"))
            {
                LOG_INFO("[       OK ] %s/%s.%s\n", inst_name, test_suite, test_num);
            }
            ret = true;
        }

        return ret;
    }

    bool summarize_test_result(void)
    {
        bool ret = true;

        LOG_INFO("\n[==========] %d tests ran.\n", num_of_pass_test + num_of_failed_test);
        LOG_INFO("[  PASSED  ] %d tests.\n", num_of_pass_test);
        if(0 != num_of_failed_test)
        {
            LOG_INFO("[  FAILED  ] %d tests, listed below:\n", num_of_failed_test);
            for(auto i = failed_test_info.begin(); i != failed_test_info.end(); i++)
            {
                if(!strcmp(i->inst_name, ""))
                {
                    LOG_INFO("[  FAILED  ] %s.%s\n", i->test_suite, i->test_num);
                }
                else
                {
                    LOG_INFO("[  FAILED  ] %s/%s.%s\n", i->inst_name, i->test_suite, i->test_num);
                }
            }

            ret = false;
        }

        return ret;
    }
};

/* Test case information storing class */
template<class T>
class TestCaseInfo
{
private:
    DISALLOW_COPY_AND_ASSIGN(TestCaseInfo<T>);
public:
    TestCaseInfo(){}
    ~TestCaseInfo(){}

    struct tuple
    {
        char test_num[100];
        T* p_test_obj;
        void (*p_setup_test_suite)(); // SetUpTestSuite
        void (*p_teardown_test_suite)(); // TearDownTestSuite
        bool is_parameterized_test; // TEST_F or TEST_P
        int loop_num; // TEST_F: loop num is 1, TEST_P: loop num got from INSTANTIATE_TEST_CASE_P
        bool is_ignore;
        char inst_name[100];
    };

    bool use_filter_test_map;
    int filter_loop_num;

    /* map structure: test suite, test num, test object, SetUpTestSuite func, TearDownTestSuite func, TEST_F or TEST_P flag detection, loop number */
    OsalFWHelper::multimap<const char*, tuple> test_map;
    OsalFWHelper::multimap<const char*, tuple> test_map_filter;
    typename OsalFWHelper::multimap<const char*, tuple>::iterator current_test;

    static TestCaseInfo<T> & get(void)
    {
        static TestCaseInfo<T> instance;
        return instance;
    }

    template<class T2> bool RegisterTestInfo(T2* p_obj, const char* test_suite_name, const char* test_num_name)
    {
        tuple tmp_obj;
        strcpy(tmp_obj.test_num, test_num_name);
        strcpy(tmp_obj.inst_name, "");
        tmp_obj.p_test_obj = (T*)(p_obj);
        tmp_obj.p_setup_test_suite = &(T2::SetUpTestSuite);
        tmp_obj.p_teardown_test_suite = &(T2::TearDownTestSuite);
        tmp_obj.is_parameterized_test = false;
        tmp_obj.loop_num = 1;
        tmp_obj.is_ignore = false;
        test_map.insert({test_suite_name, tmp_obj});
        return true;
    }

    template<class T2> bool RegisterTestInfoTestP(T2* p_obj, const char* test_instantiation_name, const char* test_suite_name, const char* test_num_name, int count_loop)
    {
        tuple tmp_obj;
        strcpy(tmp_obj.test_num, test_num_name);
        strcpy(tmp_obj.inst_name, test_instantiation_name);
        tmp_obj.p_test_obj = (T*)(p_obj);
        tmp_obj.p_setup_test_suite = &(T2::SetUpTestSuite);
        tmp_obj.p_teardown_test_suite = &(T2::TearDownTestSuite);
        tmp_obj.is_parameterized_test = true;
        tmp_obj.loop_num = count_loop;
        tmp_obj.is_ignore = false;
        test_map.insert({test_suite_name, tmp_obj});
        return true;
    }

    void delete_objs()
    {
        for (auto itr = test_map.begin(); itr != test_map.end(); ++itr)
        {
            delete itr->second.p_test_obj;
        }
        test_map.clear();
        test_map_filter.clear();
    }

    void run_test(OsalFWHelper::multimap<const char*, tuple> map)
    {
        bool is_setup_testsuite = false;
        char index_str[10];
        bool result;

        for (auto itr = map.begin(); itr != map.end(); ++itr)
        {
            current_test = itr;
            if(is_setup_testsuite != true)
            {
                LOG_INFO("\n[----------] Run tests from %s\n", itr->first);
                (itr->second.p_setup_test_suite)();
                is_setup_testsuite = true;
                TestJudgement::get().check_test_result(itr->first, "SetUpTestSuite");
            }

            if(strstr(itr->second.test_num, "DISABLED_") != NULL || strstr(itr->first, "DISABLED_") != NULL)
            {
                itr->second.is_ignore = true;
            }

            if(false == itr->second.is_ignore)
            {
                char tmp[100];
                strcpy(tmp, itr->second.test_num);
                for(auto i = 0; i < itr->second.loop_num; i++)
                {
                    if(itr->second.is_parameterized_test == true)
                    {
                        strcat(itr->second.test_num, "/");
                        sprintf(index_str, "%d", i);
                        strcat(itr->second.test_num, index_str);
                        strncpy(index_str, "\0", sizeof(index_str));
                    }

                    itr->second.p_test_obj->SetUp();
                    if(itr->second.is_parameterized_test == true)
                    {
                        LOG_INFO("[ RUN      ] %s/%s.%s\n", itr->second.inst_name, itr->first, itr->second.test_num);
                    }
                    else
                    {
                        LOG_INFO("[ RUN      ] %s.%s\n", itr->first, itr->second.test_num);
                    }
                    itr->second.p_test_obj->test_body();
                    itr->second.p_test_obj->TearDown();
                    if(itr->second.is_parameterized_test == true)
                    {
                        result = TestJudgement::get().check_test_result(itr->second.inst_name, itr->first, itr->second.test_num);
                    }
                    else
                    {
                        result = TestJudgement::get().check_test_result(itr->first, itr->second.test_num);
                    }
                    if(true != result)
                    {
                        TestJudgement::get().num_of_failed_test++;
                        TestJudgement::failed_test_information test_info;
                        strcpy(test_info.inst_name, itr->second.inst_name);
                        strcpy(test_info.test_suite, itr->first);
                        strcpy(test_info.test_num, itr->second.test_num);
                        TestJudgement::get().failed_test_info.push_back(test_info);
                    }
                    else
                    {
                        TestJudgement::get().num_of_pass_test++;
                    }

                    if(itr->second.is_parameterized_test == true)
                    {
                        strcpy(itr->second.test_num, tmp);
                    }
                }
            }

            if(map.upper_bound(itr->first) == OsalFWHelper::next(itr, 1))
            {
                (itr->second.p_teardown_test_suite)();
                is_setup_testsuite = false;
                TestJudgement::get().check_test_result(itr->first, "TearDownTestSuite");
            }
        }
    }

    void exec_tests()
    {
        bool is_setup_testsuite = false;

        for(auto i = 0; i < TestCaseInfo<OsalTestFrameWork>::get().filter_loop_num; i++)
        {
           if(true == use_filter_test_map)
            {
                run_test(test_map_filter);
            }
            else
            {
                run_test(test_map);
            }
        }
    }

    const char* current_test_name()
    {
        return current_test->second.test_num;
    }
};

/* Assertion streaming object creation function */
inline AssertionStreaming expect_pro(bool expr_result, fatal_type_t type)
{
    return AssertionStreaming(expr_result, type);
}

template<class T> inline AssertionStreaming expect_matcher(T target, bool result_matcher, const char* file, int line, fatal_type_t type)
{
    if(!result_matcher)
    {
        LOG_INFO("%s:(%d): Failure\n", file, line);
    }
    return AssertionStreaming(result_matcher, type);
}

/* show expected value and the actual value function */
inline void show_expect_actual(const e_osal_return_t &actual, const e_osal_return_t &expected, compare_type_t type)
{
    char actual_str[100];
    char expected_str[100];
    switch (actual)
    {
    case OSAL_RETURN_OK:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_OK));
        break;
    case OSAL_RETURN_PAR:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_PAR));
        break;
    case OSAL_RETURN_MEM:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_MEM));
        break;
    case OSAL_RETURN_DEV:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_DEV));
        break;
    case OSAL_RETURN_TIME:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_TIME));
        break;
    case OSAL_RETURN_CONF:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_CONF));
        break;
    case OSAL_RETURN_FAIL:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_FAIL));
        break;
    case OSAL_RETURN_IO:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_IO));
        break;
    case OSAL_RETURN_BUSY:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_BUSY));
        break;
    case OSAL_RETURN_ID:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_ID));
        break;
    case OSAL_RETURN_HANDLE:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_HANDLE));
        break;
    case OSAL_RETURN_STATE:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_STATE));
        break;
    case OSAL_RETURN_PROHIBITED:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_PROHIBITED));
        break;
    case OSAL_RETURN_UNSUPPORTED_OPERATION:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_UNSUPPORTED_OPERATION));
        break;
    case OSAL_RETURN_HANDLE_NOT_SUPPORTED:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_HANDLE_NOT_SUPPORTED));
        break;
    case OSAL_RETURN_UNKNOWN_NAME:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_UNKNOWN_NAME));
        break;
    case OSAL_RETURN_CONF_UNSUPPORTED:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_CONF_UNSUPPORTED));
        break;
    case OSAL_RETURN_OK_RQST:
        strcpy(actual_str, GENERATE_STRING(OSAL_RETURN_OK_RQST));
        break;
    default:
        LOG_ERROR("Invalid OSAL return code\n");
        break;
    }

    switch (expected)
    {
    case OSAL_RETURN_OK:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_OK));
        break;
    case OSAL_RETURN_PAR:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_PAR));
        break;
    case OSAL_RETURN_MEM:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_MEM));
        break;
    case OSAL_RETURN_DEV:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_DEV));
        break;
    case OSAL_RETURN_TIME:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_TIME));
        break;
    case OSAL_RETURN_CONF:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_CONF));
        break;
    case OSAL_RETURN_FAIL:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_FAIL));
        break;
    case OSAL_RETURN_IO:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_IO));
        break;
    case OSAL_RETURN_BUSY:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_BUSY));
        break;
    case OSAL_RETURN_ID:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_ID));
        break;
    case OSAL_RETURN_HANDLE:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_HANDLE));
        break;
    case OSAL_RETURN_STATE:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_STATE));
        break;
    case OSAL_RETURN_PROHIBITED:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_PROHIBITED));
        break;
    case OSAL_RETURN_UNSUPPORTED_OPERATION:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_UNSUPPORTED_OPERATION));
        break;
    case OSAL_RETURN_HANDLE_NOT_SUPPORTED:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_HANDLE_NOT_SUPPORTED));
        break;
    case OSAL_RETURN_UNKNOWN_NAME:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_UNKNOWN_NAME));
        break;
    case OSAL_RETURN_CONF_UNSUPPORTED:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_CONF_UNSUPPORTED));
        break;
    case OSAL_RETURN_OK_RQST:
        strcpy(expected_str, GENERATE_STRING(OSAL_RETURN_OK_RQST));
        break;
    default:
        LOG_ERROR("Invalid OSAL return code\n");
        break;
    }

    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%s) is not equal to the expected value (%s)\n", actual_str, expected_str);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%s) is not different from the expected value (%s)\n", actual_str, expected_str);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

inline void show_expect_actual(const e_osal_pm_policy& actual, const e_osal_pm_policy& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%d) is not equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%d) is not different from the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%d) is not greater than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not greater than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%d) is not less than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not less than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

inline void show_expect_actual(const e_osal_mmngr_allocator_mode& actual, const e_osal_mmngr_allocator_mode& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%d) is not equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%d) is not different from the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%d) is not greater than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not greater than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%d) is not less than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not less than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

inline void show_expect_actual(const e_osal_pm_reset_state& actual, const e_osal_pm_reset_state& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%d) is not equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%d) is not different from the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%d) is not greater than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not greater than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%d) is not less than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not less than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

inline void show_expect_actual(const e_osal_pm_required_state_t& actual, const e_osal_pm_required_state_t& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%d) is not equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%d) is not different from the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%d) is not greater than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not greater than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%d) is not less than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not less than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

inline void show_expect_actual(const e_osal_pm_state_t& actual, const e_osal_pm_state_t& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%d) is not equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%d) is not different from the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%d) is not greater than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not greater than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%d) is not less than the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not less than or equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

inline void show_expect_actual(const st_osal_time_t& actual, const st_osal_time_t& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%lld x %lld) is not equal to the expected value (%lld x %lld)\n", (long long)actual.tv_sec, actual.tv_nsec, (long long)expected.tv_sec, expected.tv_nsec);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%lld x %lld) is not different from the expected value (%lld x %lld)\n", (long long)actual.tv_sec, actual.tv_nsec, (long long)expected.tv_sec, expected.tv_nsec);
        break;
    case GREATE:
        LOG_INFO("The actual value (%lld x %lld) is not greater than the expected value (%lld x %lld)\n", (long long)actual.tv_sec, actual.tv_nsec, (long long)expected.tv_sec, expected.tv_nsec);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%lld x %lld) is not greater than or equal to the expected value (%d)\n", (long long)actual.tv_sec, actual.tv_nsec, (long long)expected.tv_sec, expected.tv_nsec);
        break;
    case LESS:
        LOG_INFO("The actual value (%lld x %lld) is not less than the expected value (%lld x %lld)\n", (long long)actual.tv_sec, actual.tv_nsec, (long long)expected.tv_sec, expected.tv_nsec);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%lld x %lld) is not less than or equal to the expected value (%lld x %lld)\n", (long long)actual.tv_sec, actual.tv_nsec, (long long)expected.tv_sec, expected.tv_nsec);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

#if defined(TEST_ZEPHYR) // on other OSs "uintptr_t" is the same as uint64_t except zephyr
template<class T> inline void show_expect_actual(const uintptr_t& actual, const T& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%x) is not equal to the expected value (%x)\n", actual, (uintptr_t)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%x) is not different from the expected value (%x)\n", actual, (uintptr_t)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%x) is not greater than the expected value (%x)\n", actual, (uintptr_t)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%x) is not greater than or equal to the expected value (%x)\n", actual, (uintptr_t)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%x) is not less than the expected value (%x)\n", actual, (uintptr_t)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%x) is not less than or equal to the expected value (%x)\n", actual, (uintptr_t)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}
#endif

template<class T> inline void show_expect_actual(void* actual, const T& expected, compare_type_t type)
{
    bool has_null_ptr = false;
    char actual_str[100];
    char expected_str[100];

    if(NULL == actual)
    {
        strcpy(actual_str, "NULL");
        has_null_ptr = true;
    }

    if(NULL == (void*)expected)
    {
        strcpy(expected_str, "NULL");
        has_null_ptr = true;
    }

    if(true == has_null_ptr)
    {
        if(NULL == actual && NULL == (void*)expected)
        {
            switch (type)
            {
            case EQUAL:
                LOG_INFO("The actual value (%s) is not equal to the expected value (%s)\n", actual_str, expected_str);
                break;
            case NOT_EQUAL:
                LOG_INFO("The actual value (%s) is not different from the expected value (%s)\n", actual_str, expected_str);
                break;
            case GREATE:
                LOG_INFO("The actual value (%s) is not greater than the expected value (%s)\n", actual_str, expected_str);
                break;
            case GREATE_OR_EQUAL:
                LOG_INFO("The actual value (%s) is not greater than or equal to the expected value (%s)\n", actual_str, expected_str);
                break;
            case LESS:
                LOG_INFO("The actual value (%s) is not less than the expected value (%s)\n", actual_str, expected_str);
                break;
            case LESS_OR_EQUAL:
                LOG_INFO("The actual value (%s) is not less than or equal to the expected value (%s)\n", actual_str, expected_str);
                break;
            default:
                LOG_INFO("Comparison type has not been supported yet\n");
                break;
            }
        }
        else if(NULL == actual)
        {
            switch (type)
            {
            case EQUAL:
                LOG_INFO("The actual value (%s) is not equal to the expected value (%p)\n", actual_str, (void*)expected);
                break;
            case NOT_EQUAL:
                LOG_INFO("The actual value (%s) is not different from the expected value (%p)\n", actual_str, (void*)expected);
                break;
            case GREATE:
                LOG_INFO("The actual value (%s) is not greater than the expected value (%p)\n", actual_str, (void*)expected);
                break;
            case GREATE_OR_EQUAL:
                LOG_INFO("The actual value (%s) is not greater than or equal to the expected value (%p)\n", actual_str, (void*)expected);
                break;
            case LESS:
                LOG_INFO("The actual value (%s) is not less than the expected value (%p)\n", actual_str, (void*)expected);
                break;
            case LESS_OR_EQUAL:
                LOG_INFO("The actual value (%s) is not less than or equal to the expected value (%p)\n", actual_str, (void*)expected);
                break;
            default:
                LOG_INFO("Comparison type has not been supported yet\n");
                break;
            }
        }
        else
        {
            switch (type)
            {
            case EQUAL:
                LOG_INFO("The actual value (%p) is not equal to the expected value (%s)\n", actual, expected_str);
                break;
            case NOT_EQUAL:
                LOG_INFO("The actual value (%p) is not different from the expected value (%s)\n", actual, expected_str);
                break;
            case GREATE:
                LOG_INFO("The actual value (%p) is not greater than the expected value (%s)\n", actual, expected_str);
                break;
            case GREATE_OR_EQUAL:
                LOG_INFO("The actual value (%p) is not greater than or equal to the expected value (%s)\n", actual, expected_str);
                break;
            case LESS:
                LOG_INFO("The actual value (%p) is not less than the expected value (%s)\n", actual, expected_str);
                break;
            case LESS_OR_EQUAL:
                LOG_INFO("The actual value (%p) is not less than or equal to the expected value (%s)\n", actual, expected_str);
                break;
            default:
                LOG_INFO("Comparison type has not been supported yet\n");
                break;
            }
        }
    }
    else
    {
        switch (type)
        {
        case EQUAL:
            LOG_INFO("The actual value (%p) is not equal to the expected value (%p)\n", actual, (void*)expected);
            break;
        case NOT_EQUAL:
            LOG_INFO("The actual value (%p) is not different from the expected value (%p)\n", actual, (void*)expected);
            break;
        case GREATE:
            LOG_INFO("The actual value (%p) is not greater than the expected value (%p)\n", actual, (void*)expected);
            break;
        case GREATE_OR_EQUAL:
            LOG_INFO("The actual value (%p) is not greater than or equal to the expected value (%p)\n", actual, (void*)expected);
            break;
        case LESS:
            LOG_INFO("The actual value (%p) is not less than the expected value (%p)\n", actual, (void*)expected);
            break;
        case LESS_OR_EQUAL:
            LOG_INFO("The actual value (%p) is not less than or equal to the expected value (%p)\n", actual, (void*)expected);
            break;
        default:
            LOG_INFO("Comparison type has not been supported yet\n");
            break;
        }
    }
}

template<class T> inline void show_expect_actual(const char* actual, const T& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%s) is not equal to the expected value (%s)\n", actual, (char*)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%s) is not different from the expected value (%s)\n", actual, (char*)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%s) is not greater than the expected value (%s)\n", actual, (char*)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%s) is not greater than or equal to the expected value (%s)\n", actual, (char*)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%s) is not less than the expected value (%s)\n", actual, (char*)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%s) is not less than or equal to the expected value (%s)\n", actual, (char*)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

template<class T> inline void show_expect_actual(const bool& actual, const T& expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%d) is not equal to the expected value (%d)\n", (int)actual, (int)expected);
        break;
    case TRUE:
        LOG_INFO("The actual value (%d) is not TRUE\n", (int)actual);
        break;
    case FALSE:
        LOG_INFO("The actual value (%d) is not FALSE\n", (int)actual);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%d) is not different from the expected value (%d)\n", (int)actual, (int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

template<class T> inline void show_expect_actual(volatile int &actual, const T &expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%d) is not equal to the expected value (%d)\n", actual, (int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%d) is not different from the expected value (%d)\n", actual, (int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%d) is not greater than the expected value (%d)\n", actual, (int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not greater than or equal to the expected value (%d)\n", actual, (int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%d) is not less than the expected value (%d)\n", actual, (int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not less than or equal to the expected value (%d)\n", actual, (int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

template<class T> inline void show_expect_actual(const unsigned int &actual, const T &expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%u) is not equal to the expected value (%u)\n", actual, (unsigned int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%u) is not different from the expected value (%u)\n", actual, (unsigned int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%u) is not greater than the expected value (%u)\n", actual, (unsigned int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%u) is not greater than or equal to the expected value (%u)\n", actual, (unsigned int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%u) is not less than the expected value (%u)\n", actual, (unsigned int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%u) is not less than or equal to the expected value (%u)\n", actual, (unsigned int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

template<class T> inline void show_expect_actual(const unsigned char &actual, const T &expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%u) is not equal to the expected value (%u)\n", actual, (unsigned char)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%u) is not different from the expected value (%u)\n", actual, (unsigned char)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%u) is not greater than the expected value (%u)\n", actual, (unsigned char)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%u) is not greater than or equal to the expected value (%u)\n", actual, (unsigned char)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%u) is not less than the expected value (%u)\n", actual, (unsigned char)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%u) is not less than or equal to the expected value (%u)\n", actual, (unsigned char)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

template<class T> inline void show_expect_actual(const uint64_t &actual, const T &expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%llu) is not equal to the expected value (%llu)\n", (unsigned long long)actual, (unsigned long long)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%llu) is not different from the expected value (%llu)\n", (unsigned long long)actual, (unsigned long long)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%llu) is not greater than the expected value (%llu)\n", (unsigned long long)actual, (unsigned long long)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%llu) is not greater than or equal to the expected value (%llu)\n", (unsigned long long)actual, (unsigned long long)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%llu) is not less than the expected value (%llu)\n", (unsigned long long)actual, (unsigned long long)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%llu) is not less than or equal to the expected value (%llu)\n", (unsigned long long)actual, (unsigned long long)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

template<class T> inline void show_expect_actual(const uint16_t &actual, const T &expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%u) is not equal to the expected value (%u)\n", (unsigned int)actual, (unsigned int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%u) is not different from the expected value (%u)\n", (unsigned int)actual, (unsigned int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%u) is not greater than the expected value (%u)\n", (unsigned int)actual, (unsigned int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%u) is not greater than or equal to the expected value (%u)\n", (unsigned int)actual, (unsigned int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%u) is not less than the expected value (%u)\n", (unsigned int)actual, (unsigned int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%u) is not less than or equal to the expected value (%u)\n", (unsigned int)actual, (unsigned int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

template<class T> inline void show_expect_actual(int &actual, const T &expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%d) is not equal to the expected value (%d)\n", actual, (int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%d) is not different from the expected value (%d)\n", actual, (int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%d) is not greater than the expected value (%d)\n", actual, (int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not greater than or equal to the expected value (%d)\n", actual, (int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%d) is not less than the expected value (%d)\n", actual, (int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%d) is not less than or equal to the expected value (%d)\n", actual, (int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

template<class T> inline void show_expect_actual(const int64_t &actual, const T &expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%lld) is not equal to the expected value (%lld)\n", (long long)actual, (long long)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%lld) is not different from the expected value (%lld)\n", (long long)actual, (long long)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%lld) is not greater than the expected value (%lld)\n", (long long)actual, (long long)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%lld) is not greater than or equal to the expected value (%lld)\n", (long long)actual, (long long)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%lld) is not less than the expected value (%lld)\n", (long long)actual, (long long)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%lld) is not less than or equal to the expected value (%lld)\n", (long long)actual, (long long)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}

#if !defined(TEST_ZEPHYR) && !defined(TEST_WINDOWS)// on zephyr and windows "unsigned long long int" is the same as uint64_t
template<class T> inline void show_expect_actual(const unsigned long long int &actual, const T &expected, compare_type_t type)
{
    switch (type)
    {
    case EQUAL:
        LOG_INFO("The actual value (%llu) is not equal to the expected value (%llu)\n", actual, (unsigned long long int)expected);
        break;
    case NOT_EQUAL:
        LOG_INFO("The actual value (%llu) is not different from the expected value (%llu)\n", actual, (unsigned long long int)expected);
        break;
    case GREATE:
        LOG_INFO("The actual value (%llu) is not greater than the expected value (%llu)\n", actual, (unsigned long long int)expected);
        break;
    case GREATE_OR_EQUAL:
        LOG_INFO("The actual value (%llu) is not greater than or equal to the expected value (%llu)\n", actual, (unsigned long long int)expected);
        break;
    case LESS:
        LOG_INFO("The actual value (%llu) is not less than the expected value (%llu)\n", actual, (unsigned long long int)expected);
        break;
    case LESS_OR_EQUAL:
        LOG_INFO("The actual value (%llu) is not less than or equal to the expected value (%llu)\n", actual, (unsigned long long int)expected);
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        break;
    }
}
#endif

/* String comparison function */
inline bool do_compare_str(const char* target, const char* expected_value, compare_type_t type, const char* file, int line)
{
    bool ret = true;

    switch (type)
    {
    case STR_EQUAL:
        if(NULL != expected_value && NULL != target)
        {
            if(strcmp(target, expected_value))
            {
                ret = false;
            }
        }
        else /* expected_value is NULL */
        {
            if(target != expected_value)
            {
                ret = false;
            }
        }
        break;
    case STR_NOT_EQUAL:
        if(NULL != expected_value && NULL != target)
        {
            if(!strcmp(target, expected_value))
            {
                ret = false;
            }
        }
        else /* expected_value is NULL */
        {
            if(target == expected_value)
            {
                ret = false;
            }
        }
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        type = INVALID;
        ret = false;
        break;
    }

    if((!ret) && (INVALID != type))
    {
        LOG_INFO("%s:(%d): Failure\n", file, line);
        TestJudgement::get().num_of_failed_expr++;
        char expected_value_str[100];
        char target_str[100];
        if(NULL == expected_value)
        {
            strcpy(expected_value_str, "NULL");
        }
        else
        {
            strcpy(expected_value_str, expected_value);
        }

        if(NULL == target)
        {
            strcpy(target_str, "NULL");
        }
        else
        {
            strcpy(target_str, target);
        }
        show_expect_actual(target_str, expected_value_str, type);
    }

    return ret;
}

/* Comparison function */
template<class T, class U> inline bool do_compare(const T &target, const U &expected_value, compare_type_t type, const char* file, int line)
{
    bool ret = true;
    T actual_value = target;

    switch (type)
    {
    case EQUAL:
    case TRUE:
    case FALSE:
        if(!(actual_value == (T)expected_value))
        {
            ret = false;
        }
        break;
    case NOT_EQUAL:
        if(actual_value == (T)expected_value)
        {
            ret = false;
        }
        break;
    case GREATE:
        if(!(actual_value > (T)expected_value))
        {
            ret = false;
        }
        break;
    case GREATE_OR_EQUAL:
        if(!(actual_value >= (T)expected_value))
        {
            ret = false;
        }
        break;
    case LESS:
        if(!(actual_value < (T)expected_value))
        {
            ret = false;
        }
        break;
    case LESS_OR_EQUAL:
        if(!(actual_value <= (T)expected_value))
        {
            ret = false;
        }
        break;
    case FAIL_WITHOUT_EXPR:
        ret = false;
        break;
    default:
        LOG_INFO("Comparison type has not been supported yet\n");
        type = INVALID;
        ret = false;
        break;
    }

    if((!ret) && (INVALID != type))
    {
        LOG_INFO("%s:(%d): Failure\n", file, line);
        TestJudgement::get().num_of_failed_expr++;
        switch (type)
        {
        case FAIL_WITHOUT_EXPR:
            break;
        default:
            show_expect_actual(actual_value, expected_value, type);
            break;
        }
    }

    return ret;
}

/* Function is to execute all test cases */
inline int RUN_ALL_TESTS()
{
    int ret = 0;

    TestCaseInfo<OsalTestFrameWork>::get().exec_tests();
    TestCaseInfo<OsalTestFrameWork>::get().delete_objs();
    if(!TestJudgement::get().summarize_test_result())
    {
        ret = -1;
    }

    return ret;
}

inline void create_filter_test_map_test_case(char* test_suite, char* test_num, bool is_ignore)
{
    bool found = false;

    for(auto i = TestCaseInfo<OsalTestFrameWork>::get().test_map.begin(); i != TestCaseInfo<OsalTestFrameWork>::get().test_map.end(); i++)
    {
        if(!strcmp(test_suite, i->first) && !strcmp(test_num, i->second.test_num))
        {
            i->second.is_ignore = is_ignore;
            if(true == is_ignore && true == TestCaseInfo<OsalTestFrameWork>::get().use_filter_test_map) // check the ignore test already inserted to the test_map_filter or not
            {
                for(auto j = TestCaseInfo<OsalTestFrameWork>::get().test_map_filter.begin(); j != TestCaseInfo<OsalTestFrameWork>::get().test_map_filter.end(); j++)
                {
                    if(!strcmp(test_suite, j->first) && !strcmp(test_num, j->second.test_num))
                    {
                        j->second.is_ignore = is_ignore;
                        found = true;
                    }
                }
            }

            if(true != found)
            {
                TestCaseInfo<OsalTestFrameWork>::get().test_map_filter.insert({(i->first), (i->second)});
            }
        }
    }
}

inline void create_filter_test_map_test_suite(char* test_suite, bool is_ignore)
{
    bool found = false;

    for(auto i = TestCaseInfo<OsalTestFrameWork>::get().test_map.begin(); i != TestCaseInfo<OsalTestFrameWork>::get().test_map.end(); i++)
    {
        if(!strcmp(test_suite, i->first))
        {
            i->second.is_ignore = is_ignore;
            if(true == is_ignore && true == TestCaseInfo<OsalTestFrameWork>::get().use_filter_test_map) // check the ignore test already inserted to the test_map_filter or not
            {
                for(auto j = TestCaseInfo<OsalTestFrameWork>::get().test_map_filter.begin(); j != TestCaseInfo<OsalTestFrameWork>::get().test_map_filter.end(); j++)
                {
                    if(!strcmp(test_suite, j->first))
                    {
                        j->second.is_ignore = is_ignore;
                        found = true;
                    }
                }
            }

            if(true != found)
            {
                TestCaseInfo<OsalTestFrameWork>::get().test_map_filter.insert({(i->first), (i->second)});
            }
        }
    }
}

inline void parse_running_and_ignore_list(char** option_list, size_t num_of_options)
{
    char* test_suite;
    char* test_num;
    bool is_ignore = false;
    size_t count_ignore_test = 0;

    for(auto i = 0; i < num_of_options; i++)
    {
        if(NULL != strstr(option_list[i], "-"))
        {
            count_ignore_test++;
        }
    }

    if(count_ignore_test == num_of_options)
    {
        TestCaseInfo<OsalTestFrameWork>::get().use_filter_test_map = false; // filter option only has ignore test. Use test_map instead of test_map_filter
    }
    else
    {
        TestCaseInfo<OsalTestFrameWork>::get().use_filter_test_map = true; // filter option has info used for running specific test cases. Use test_map_filter
    }

    for(auto i = 0; i < num_of_options; i++)
    {
        if(NULL != strstr(option_list[i], ".")) // single test case. Ex: --test_filter=test_suite.test_name
        {
            char* p_dot = strstr(option_list[i], ".");
            test_num = p_dot + 1; // plus to remove '.'
            *p_dot = '\0';
            if(NULL != strstr(option_list[i], "-"))
            {
                test_suite = option_list[i] + 1;
                is_ignore = true;
            }
            else
            {
                test_suite = option_list[i];
            }
            create_filter_test_map_test_case(test_suite, test_num, is_ignore);
        }

        if(NULL != strstr(option_list[i], "*")) // single test suite. Ex: --test_filter=test_suite1*
        {
            char* p_star = strstr(option_list[i], "*");
            *p_star = '\0';
            if(NULL != strstr(option_list[i], "-"))
            {
                test_suite = option_list[i] + 1;
                is_ignore = true;
            }
            else
            {
                test_suite = option_list[i];
            }
            create_filter_test_map_test_suite(test_suite, is_ignore);
        }
    }
}

inline void parse_filter_options(char* filter_option)
{
    char* option_list[MAX_ELEMENT_IN_FILTER_OPTION];
    char* begin;
    char* end;
    size_t option_count = 0;
    size_t filter_size = strlen(filter_option);

    // get the first option
    begin = strstr(filter_option, "=") + 1;
    if(NULL != strstr(filter_option, ":"))
    {
        end = strstr(filter_option, ":");
    }
    else
    {
        end = begin + strlen(begin);
    }
    option_list[0] = (char*)malloc(end - begin + 1);
    strncpy(option_list[0], begin, end - begin);
    option_list[0][end - begin] = '\0';
    option_count++;

    // get the remaining option if it's available
    for(auto i = 0; i < filter_size; i++)
    {
        if(':' == filter_option[i])
        {
            begin = &filter_option[i] + 1;
            if(NULL != strstr(begin, ":"))
            {
                end = strstr(begin, ":");
            }
            else
            {
                end = begin + strlen(begin);
            }
            option_list[option_count] = (char*)malloc(end - begin + 1);
            strncpy(option_list[option_count], begin, end - begin);
            option_list[option_count][end - begin] = '\0';
            option_count++;
        }
    }

    parse_running_and_ignore_list(option_list, option_count);
    for(auto i = 0; i < option_count; i++)
    {
        if(NULL != option_list[i])
        {
            free(option_list[i]);
        }
    }
}

inline void parse_loop_time(char* filter_option)
{
    char* loop_num_str = strstr(filter_option, "=") + 1;
    TestCaseInfo<OsalTestFrameWork>::get().filter_loop_num = atoi(loop_num_str);
    if (0 == TestCaseInfo<OsalTestFrameWork>::get().filter_loop_num)
    {
        LOG_ERROR("Invalid loop number\n");
    }
}

/* Support format:
* --test_filter=test_suite.test_name
* --test_filter=test_suite.test_name:test_suite1.test_name1
* --test_filter=test_suite*
* --test_filter=test_suite*:test_suite1*
* --test_filter=test_suite*:test_suite1.test_case1
*/
inline int InitOSALTestFramework(int* argc, char** argv)
{
    int local_arg_num = *argc;
    char* filter_option;
    TestCaseInfo<OsalTestFrameWork>::get().use_filter_test_map = false;
    TestCaseInfo<OsalTestFrameWork>::get().filter_loop_num = 1;

    if(local_arg_num > 1)
    {
        if(local_arg_num > 3)
        {
            LOG_ERROR("Invalid option. OSAL Test Framework only supports 2 kind of options:\n");
            LOG_ERROR("--test_filter: To run specific test cases or test suites. To ignore specific test cases or test suites\n");
            LOG_ERROR("--test_repeat: To repeat all (or selected) test cases or test suite in a program many times\n");
            return -1;
        }
        else
        {
            for(auto i = 1; i < local_arg_num; i++)
            {
                if(NULL != strstr(argv[i], "--test_filter"))
                {
                    filter_option = argv[i];
                    if(NULL == strstr(filter_option, "="))
                    {
                        LOG_ERROR("Invalid filter option\n");
                        return -1;
                    }
                    parse_filter_options(filter_option);
                }
                else if(NULL != strstr(argv[i], "--test_repeat"))
                {
                    filter_option = argv[i];
                    if(NULL == strstr(filter_option, "="))
                    {
                        LOG_ERROR("Invalid filter option\n");
                        return -1;
                    }
                    parse_loop_time(filter_option);
                }
                else
                {
                    LOG_ERROR("Invalid option. Current OSAL Test Framework has not supported it yet\n");
                    LOG_ERROR("OSAL Test Framework only supports 2 kind of options:\n");
                    LOG_ERROR("--test_filter: To run specific test cases or test suites. To ignore specific test cases or test suites\n");
                    LOG_ERROR("--test_repeat: To repeat all (or selected) test cases or test suite in a program many times\n");
                    return -1;
                }
            }
        }
    }

    return 0;
}
#endif
