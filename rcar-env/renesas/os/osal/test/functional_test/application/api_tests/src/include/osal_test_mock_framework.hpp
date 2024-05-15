#ifndef osal_test_mock_framework
#define osal_test_mock_framework

#include "osal_test_framework.hpp"
#include "osal_test_framework_helper.hpp"

// Generate head and tail of args list
#define GENERATE_HEAD(...) GENERATE_HEAD_INTERNAL(__VA_ARGS__)
#define GENERATE_HEAD_INTERNAL(_1, ...) _1
#define GENERATE_TAIL(...) GENERATE_TAIL_INTERNAL(__VA_ARGS__)
#define GENERATE_TAIL_INTERNAL(_1, ...) __VA_ARGS__

// remove parentheses
#define REMOVE_PARENS(...) __VA_ARGS__

// Max num of EXPECT_CALL
#define MAX_NUM_EXPECT_CALL 10

// A preprocessor comma counting
#define COMMA_COUNT(...) COMMA_COUNT_I(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define COMMA_COUNT_I(_9,_8,_7,_6,_5,_4,_3,_2,_1,X,...) X

// if-else statement
#define IF_ELSE_STATEMENT(cond, do_if_body, do_else_body) STR_CAT(IF_ELSE_STATEMENT_, cond)(do_if_body, do_else_body)
#define IF_ELSE_STATEMENT_1(do_if_body, do_else_body) do_if_body
#define IF_ELSE_STATEMENT_0(do_if_body, do_else_body) do_else_body

// define comma
#define COMMA(...) ,

// check args is empty or not
#define IS_ARGS_EMPTY(...) STR_CAT(IS_ARGS_EMPTY_, COMMA_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define IS_ARGS_EMPTY_0(...) \
IF_ELSE_STATEMENT(COMMA_COUNT(COMMA __VA_ARGS__()), 0, 1)
#define IS_ARGS_EMPTY_1(...) 1
#define IS_ARGS_EMPTY_2(...) 1
#define IS_ARGS_EMPTY_3(...) 1
#define IS_ARGS_EMPTY_4(...) 1
#define IS_ARGS_EMPTY_5(...) 1

// A preprocessor argument counting
#define COUNT_ARGS(...) \
IF_ELSE_STATEMENT(IS_ARGS_EMPTY(__VA_ARGS__), COUNT_ARGS_I(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1), 0)
#define COUNT_ARGS_I(_9,_8,_7,_6,_5,_4,_3,_2,_1,X,...) X

// add a comma
#define ADD_COMMA(index) STR_CAT(COMMA_, index)
#define COMMA_0
#define COMMA_1 ,
#define COMMA_2 ,
#define COMMA_3 ,
#define COMMA_4 ,
#define COMMA_5 ,

// increase index
#define INCREASE(index) STR_CAT(INDEX_, index)
#define INDEX_0 1
#define INDEX_1 2
#define INDEX_2 3
#define INDEX_3 4
#define INDEX_4 5
#define INDEX_5 6

// get argument
#define GET_ARGUMENT(prefix, i) STR_CAT(prefix, i)

#define MAKE_OWN_ARGS(func_type, ...) STR_CAT(MAKE_OWN_ARGS_, COUNT_ARGS __VA_ARGS__)(0, func_type, __VA_ARGS__)
#define MAKE_OWN_ARGS_PRO(index, func_type, ...) \
ADD_COMMA(index) \
typename OsalGetArgsType::OsalFunction<func_type(__VA_ARGS__)>::Arg::type mock_arg_##index
#define MAKE_OWN_ARGS_0(index, func_type, ...)
#define MAKE_OWN_ARGS_1(index, func_type, ...) \
MAKE_OWN_ARGS_PRO(index, func_type, REMOVE_PARENS __VA_ARGS__)
#define MAKE_OWN_ARGS_1_WITHOUT_PARENS(index, func_type, ...) \
MAKE_OWN_ARGS_PRO(index, func_type, __VA_ARGS__)
#define MAKE_OWN_ARGS_2(index, func_type, ...) \
MAKE_OWN_ARGS_PRO(index, func_type, GENERATE_HEAD __VA_ARGS__) \
MAKE_OWN_ARGS_1_WITHOUT_PARENS(INCREASE(index), func_type, GENERATE_TAIL __VA_ARGS__)
#define MAKE_OWN_ARGS_3(index, func_type, ...) \
MAKE_OWN_ARGS_PRO(index, func_type, GENERATE_HEAD __VA_ARGS__) \
MAKE_OWN_ARGS_2(INCREASE(index), func_type, (GENERATE_TAIL __VA_ARGS__))
#define MAKE_OWN_ARGS_4(index, func_type, ...) \
MAKE_OWN_ARGS_PRO(index, func_type, GENERATE_HEAD __VA_ARGS__) \
MAKE_OWN_ARGS_3(INCREASE(index), func_type, (GENERATE_TAIL __VA_ARGS__))
#define MAKE_OWN_ARGS_5(index, func_type, ...) \
MAKE_OWN_ARGS_PRO(index, func_type, GENERATE_HEAD __VA_ARGS__) \
MAKE_OWN_ARGS_4(INCREASE(index), func_type, (GENERATE_TAIL __VA_ARGS__))

// Check the value of all parameters
#define DO_CHECK_PARAMETER(obj, has_const, i, ...) STR_CAT(DO_CHECK_PARAM_CONST_, has_const)(i, obj, __VA_ARGS__)
#define DO_CHECK_PARAM_CONST_0(i, obj, ...) DO_CHECK_PARAM_ARGS(i, obj, __VA_ARGS__)
#define DO_CHECK_PARAM_CONST_1(i, obj, ...) \
DO_CHECK_PARAM_ARGS(i, const_cast<decltype(obj)&>(obj), __VA_ARGS__)
#define DO_CHECK_PARAM_ARGS(i, obj, ...) STR_CAT(DO_CHECK_PARAM_ARG_, i)(i, obj, __VA_ARGS__)
#define DO_CHECK_PARAM_PRO(i, obj, ...) \
for (size_t count = 0; count < obj.index; count++) \
{ \
    if(false == obj.p_base_obj[count]->GET_ARGUMENT(is_ignore_arg_, i)) \
    { \
        if(GET_ARGUMENT(mock_arg_, i) == obj.p_base_obj[count]->GET_ARGUMENT(mock_expect_arg_, i)) \
        { \
            obj.p_base_obj[count]->num_of_matched_parameter++; \
            __VA_ARGS__ = true; \
        } \
    } \
    else \
    { \
        obj.p_base_obj[count]->num_of_matched_parameter++; \
        __VA_ARGS__ = true; \
    } \
}
#define DO_CHECK_PARAM_ARG_0(i, obj, ...) \
DO_CHECK_PARAM_PRO(i, obj, __VA_ARGS__)
#define DO_CHECK_PARAM_ARG_1(i, obj, ...) \
DO_CHECK_PARAM_PRO(i, obj, __VA_ARGS__)
#define DO_CHECK_PARAM_ARG_2(i, obj, ...) \
DO_CHECK_PARAM_PRO(i, obj, __VA_ARGS__)
#define DO_CHECK_PARAM_ARG_3(i, obj, ...) \
DO_CHECK_PARAM_PRO(i, obj, __VA_ARGS__)
#define DO_CHECK_PARAM_ARG_4(i, obj, ...) \
DO_CHECK_PARAM_PRO(i, obj, __VA_ARGS__)

#define DO_CHECK_MATCHED_PARAM(obj, has_const, expected_num) STR_CAT(DO_CHECK_MATCHED_PARAM_CONST_, has_const)(obj, expected_num)
#define DO_CHECK_MATCHED_PARAM_CONST_0(obj, expected_num) DO_CHECK_MATCHED_PARAM_PRO(obj, expected_num)
#define DO_CHECK_MATCHED_PARAM_CONST_1(obj, expected_num) \
DO_CHECK_MATCHED_PARAM_PRO(const_cast<decltype(obj)&>(obj), expected_num)

#define DO_CHECK_MATCHED_PARAM_PRO(obj, expected_num) \
for (size_t j = 0; j < obj.index; j++) \
{ \
    if(expected_num == obj.p_base_obj[j]->num_of_matched_parameter) \
    { \
        obj.p_base_obj[j]->actual_exec_times++; \
    } \
    obj.p_base_obj[j]->num_of_matched_parameter = 0; \
}

// store the value of passing arguments in EXPECT_CALL
#define STORE_EXPECT_VALUE(obj, index, ...) STR_CAT(STORE_EXPECT_VALUE_, index)(index, obj, __VA_ARGS__)
#define STORE_EXPECT_VALUE_PRO(index, obj, ...) \
obj->GET_ARGUMENT(mock_expect_arg_, index) = __VA_ARGS__;
#define STORE_EXPECT_VALUE_0(index, obj, ...) \
STORE_EXPECT_VALUE_PRO(index, obj, __VA_ARGS__)
#define STORE_EXPECT_VALUE_1(index, obj, ...) \
STORE_EXPECT_VALUE_PRO(index, obj, __VA_ARGS__)
#define STORE_EXPECT_VALUE_2(index, obj, ...) \
STORE_EXPECT_VALUE_PRO(index, obj, __VA_ARGS__)
#define STORE_EXPECT_VALUE_3(index, obj, ...) \
STORE_EXPECT_VALUE_PRO(index, obj, __VA_ARGS__)
#define STORE_EXPECT_VALUE_4(index, obj, ...) \
STORE_EXPECT_VALUE_PRO(index, obj, __VA_ARGS__)


// Get the value of passing arguments in EXPECT_CALL
#define GET_VALUE_FROM_OBJ(obj, index, ...) STR_CAT(GET_VALUE_FROM_OBJ_, COUNT_ARGS __VA_ARGS__)(obj, index)
#define GET_VALUE_FROM_OBJ_0(...) 0
#define GET_VALUE_FROM_OBJ_1(obj, index) \
obj.GET_ARGUMENT(mock_expect_arg_, index)
#define GET_VALUE_FROM_OBJ_2(obj, index) \
obj.GET_ARGUMENT(mock_expect_arg_, index)
#define GET_VALUE_FROM_OBJ_3(obj, index) \
obj.GET_ARGUMENT(mock_expect_arg_, index)
#define GET_VALUE_FROM_OBJ_4(obj, index) \
obj.GET_ARGUMENT(mock_expect_arg_, index)
#define GET_VALUE_FROM_OBJ_5(obj, index) \
obj.GET_ARGUMENT(mock_expect_arg_, index)

// Create the container to store the value of passing arguments in EXPECT_CALL
#define MAKE_SEPARATED_ARGS(func_type, ...) STR_CAT(MAKE_ARG_, COUNT_ARGS __VA_ARGS__)(0, func_type, __VA_ARGS__)
#define MAKE_ARG_PRO(index, func_type, ...) \
typename OsalGetArgsType::OsalFunction<func_type(__VA_ARGS__)>::Arg::type mock_expect_arg_##index;
#define MAKE_ARG_0(index, func_type, ...)
#define MAKE_ARG_1(index, func_type, ...) \
MAKE_ARG_PRO(index, func_type, REMOVE_PARENS __VA_ARGS__)
#define MAKE_ARG_1_WITHOUT_PARENS(index, func_type, ...) \
MAKE_ARG_PRO(index, func_type, __VA_ARGS__)
#define MAKE_ARG_2(index, func_type, ...) \
MAKE_ARG_PRO(index, func_type, GENERATE_HEAD __VA_ARGS__) \
MAKE_ARG_1_WITHOUT_PARENS(INCREASE(index), func_type, GENERATE_TAIL __VA_ARGS__)
#define MAKE_ARG_3(index, func_type, ...) \
MAKE_ARG_PRO(index, func_type, GENERATE_HEAD __VA_ARGS__) \
MAKE_ARG_2(INCREASE(index), func_type, (GENERATE_TAIL __VA_ARGS__))
#define MAKE_ARG_4(index, func_type, ...) \
MAKE_ARG_PRO(index, func_type, GENERATE_HEAD __VA_ARGS__) \
MAKE_ARG_3(INCREASE(index), func_type, (GENERATE_TAIL __VA_ARGS__))
#define MAKE_ARG_5(index, func_type, ...) \
MAKE_ARG_PRO(index, func_type, GENERATE_HEAD __VA_ARGS__) \
MAKE_ARG_4(INCREASE(index), func_type, (GENERATE_TAIL __VA_ARGS__))


#define IS_CONST(spec) STR_CAT(FUNC_HAS_CONST_, spec)
#define FUNC_HAS_CONST_const ,

#define MOCK_METHOD(...) STR_CAT(MOCK_METHOD_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)
#define MOCK_METHOD_3(func_type, func, args) MOCK_METHOD_CHECK_CONST_0(func_type, func, args, ())
#define MOCK_METHOD_4(func_type, func, args, spec) STR_CAT(MOCK_METHOD_CHECK_CONST_, COMMA_COUNT(IS_CONST spec))(func_type, func, args, spec)
#define MOCK_METHOD_CHECK_CONST_0(func_type, func, args, spec) MOCK_METHOD_PRO_CHECK_ARGS(func_type, func, args, 0, REMOVE_PARENS spec)
#define MOCK_METHOD_CHECK_CONST_1(func_type, func, args, spec) MOCK_METHOD_PRO_CHECK_ARGS(func_type, func, args, 1, REMOVE_PARENS spec)
#define MOCK_METHOD_PRO_CHECK_ARGS(func_type, func, args, has_const, ...) STR_CAT(MOCK_METHOD_PRO_, COUNT_ARGS args)(func_type, func, args, has_const, __VA_ARGS__)
#define MOCK_METHOD_PRO_0(func_type, func, args, has_const, ...) \
class mock_##func \
{ \
public: \
    mock_##func(){} \
    ~mock_##func(){} \
    struct OsalMockMethod { \
        OsalMockMethod(){} \
        ~OsalMockMethod() \
        { \
            if(false == is_atleast) \
            { \
                if(expect_exec_times != actual_exec_times) \
                { \
                    LOG_INFO("Failure, Expected exection time is %d, but the actual execution time is %d\n", expect_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
            else \
            { \
                if(expect_atleast_exec_times > actual_exec_times) \
                { \
                    LOG_INFO("Failure, Atleast expected exection time is %d, but the actual execution time is %d\n", expect_atleast_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
        } \
        int expect_exec_times = 0; \
        int expect_atleast_exec_times = 0; \
        int actual_exec_times = 0; \
        bool is_atleast = false; \
    }; \
    mock_##func& operator()() \
    { \
        p_base_obj[index] = OsalFWHelper::shared_ptr<OsalMockMethod>(new OsalMockMethod()); \
        p_base_obj[index]->expect_exec_times = 1; \
        index++; \
        return *this; \
    } \
    template<class T> void Times(const T& value) \
    { \
        p_base_obj[index - 1]->expect_exec_times = value; \
    } \
    void Times(OsalTestFrameworkMatcher::matcher_info& obj) \
    { \
        p_base_obj[index - 1]->expect_atleast_exec_times = obj.atleast_value; \
        p_base_obj[index - 1]->is_atleast = true; \
    } \
    size_t index = 0; \
    OsalFWHelper::shared_ptr<OsalMockMethod> p_base_obj[MAX_NUM_EXPECT_CALL]; \
}; \
mock_##func object_##func; \
func_type func(MAKE_OWN_ARGS(func_type, args)) __VA_ARGS__ \
{ \
    LOG_INFO("Callback function (%s) was called \n", __func__); \
    for(auto i = 0; i < object_##func.index; i++) \
    { \
        const_cast<decltype(object_##func)&>(object_##func).p_base_obj[i]->actual_exec_times++; \
    } \
    return (func_type)0; \
}

#define MOCK_METHOD_PRO_1(func_type, func, args, has_const, ...) \
class mock_##func \
{ \
public: \
    mock_##func(){} \
    ~mock_##func(){} \
    struct OsalMockMethod { \
        OsalMockMethod(){} \
        ~OsalMockMethod() \
        { \
            if(false == is_atleast) \
            { \
                if(expect_exec_times != actual_exec_times) \
                { \
                    LOG_INFO("Failure, Expected exection time is %d, but the actual execution time is %d\n", expect_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
            else \
            { \
                if(expect_atleast_exec_times > actual_exec_times) \
                { \
                    LOG_INFO("Failure, Atleast expected exection time is %d, but the actual execution time is %d\n", expect_atleast_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
        } \
        template<class T> void store_expected_arg0(T value) \
        { \
            STORE_EXPECT_VALUE(this, 0, value); \
        } \
        void store_expected_arg0(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_0 = true; \
        } \
        int expect_exec_times = 0; \
        int expect_atleast_exec_times = 0; \
        int actual_exec_times = 0; \
        bool is_atleast = false; \
        int num_of_matched_parameter = 0; \
        MAKE_SEPARATED_ARGS(func_type, args); \
        bool is_ignore_arg_0 = false; \
    }; \
    template<class T0> mock_##func& operator()(const T0 &arg0) \
    { \
        OsalFWHelper::shared_ptr<OsalMockMethod> tmp(new OsalMockMethod()); \
        p_base_obj[index] = tmp; \
        p_base_obj[index]->store_expected_arg0(arg0); \
        p_base_obj[index]->expect_exec_times = 1; \
        index++; \
        return *this; \
    } \
    template<class T> void Times(const T& value) \
    { \
        p_base_obj[index - 1]->expect_exec_times = value; \
    } \
    void Times(OsalTestFrameworkMatcher::matcher_info& obj) \
    { \
        p_base_obj[index - 1]->expect_atleast_exec_times = obj.atleast_value; \
        p_base_obj[index - 1]->is_atleast = true; \
    } \
    size_t index = 0; \
    OsalFWHelper::shared_ptr<OsalMockMethod> p_base_obj[MAX_NUM_EXPECT_CALL]; \
}; \
mock_##func object_##func; \
func_type func(MAKE_OWN_ARGS(func_type, args)) __VA_ARGS__ \
{ \
    bool is_called = true; \
    bool ret = false; \
    DO_CHECK_PARAMETER(object_##func, has_const, 0, ret); \
    DO_CHECK_MATCHED_PARAM(object_##func, has_const, 1); \
    if(true == ret) \
    { \
        LOG_INFO("Callback function (%s) was called \n", __func__); \
    } \
    return (func_type)0; \
}

#define MOCK_METHOD_PRO_2(func_type, func, args, has_const, ...) \
class mock_##func \
{ \
public: \
    mock_##func(){} \
    ~mock_##func(){} \
    struct OsalMockMethod { \
        OsalMockMethod(){} \
        ~OsalMockMethod() \
        { \
            if(false == is_atleast) \
            { \
                if(expect_exec_times != actual_exec_times) \
                { \
                    LOG_INFO("Failure, Expected exection time is %d, but the actual execution time is %d\n", expect_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
            else \
            { \
                if(expect_atleast_exec_times > actual_exec_times) \
                { \
                    LOG_INFO("Failure, Atleast expected exection time is %d, but the actual execution time is %d\n", expect_atleast_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
        } \
        template<class T> void store_expected_arg0(T value) \
        { \
            STORE_EXPECT_VALUE(this, 0, value); \
        } \
        void store_expected_arg0(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_0 = true; \
        } \
        template<class T> void store_expected_arg1(T value) \
        { \
            STORE_EXPECT_VALUE(this, 1, value); \
        } \
        void store_expected_arg1(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_1 = true; \
        } \
        int expect_exec_times = 0; \
        int expect_atleast_exec_times = 0; \
        int actual_exec_times = 0; \
        bool is_atleast = false; \
        int num_of_matched_parameter = 0; \
        MAKE_SEPARATED_ARGS(func_type, args); \
        bool is_ignore_arg_0 = false; \
        bool is_ignore_arg_1 = false; \
    }; \
    template<class T0, class T1> mock_##func& operator()(const T0 &arg0, const T1 &arg1) \
    { \
        OsalFWHelper::shared_ptr<OsalMockMethod> tmp(new OsalMockMethod()); \
        p_base_obj[index] = tmp; \
        p_base_obj[index]->store_expected_arg0(arg0); \
        p_base_obj[index]->store_expected_arg1(arg1); \
        p_base_obj[index]->expect_exec_times = 1; \
        index++; \
        return *this; \
    } \
    template<class T> void Times(const T& value) \
    { \
        p_base_obj[index - 1]->expect_exec_times = value; \
    } \
    void Times(OsalTestFrameworkMatcher::matcher_info& obj) \
    { \
        p_base_obj[index - 1]->expect_atleast_exec_times = obj.atleast_value; \
        p_base_obj[index - 1]->is_atleast = true; \
    } \
    size_t index = 0; \
    OsalFWHelper::shared_ptr<OsalMockMethod> p_base_obj[MAX_NUM_EXPECT_CALL]; \
}; \
mock_##func object_##func; \
func_type func(MAKE_OWN_ARGS(func_type, args)) __VA_ARGS__ \
{ \
    bool is_called = true; \
    bool ret[2] = {false, false}; \
    DO_CHECK_PARAMETER(object_##func, has_const, 0, ret[0]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 1, ret[1]); \
    DO_CHECK_MATCHED_PARAM(object_##func, has_const, 2); \
    for(auto i = 0; i < 2; i++) \
    { \
        if(true != ret[i]) \
        { \
            is_called = false; \
            break; \
        } \
    } \
    if(true == is_called) \
    { \
        LOG_INFO("Callback function (%s) was called \n", __func__); \
    } \
    return (func_type)0; \
}

#define MOCK_METHOD_PRO_3(func_type, func, args, has_const, ...) \
class mock_##func \
{ \
public: \
    mock_##func(){} \
    ~mock_##func(){} \
    struct OsalMockMethod { \
        OsalMockMethod(){} \
        ~OsalMockMethod() \
        { \
            if(false == is_atleast) \
            { \
                if(expect_exec_times != actual_exec_times) \
                { \
                    LOG_INFO("Failure, Expected exection time is %d, but the actual execution time is %d\n", expect_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
            else \
            { \
                if(expect_atleast_exec_times > actual_exec_times) \
                { \
                    LOG_INFO("Failure, Atleast expected exection time is %d, but the actual execution time is %d\n", expect_atleast_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
        } \
        template<class T> void store_expected_arg0(T value) \
        { \
            STORE_EXPECT_VALUE(this, 0, value); \
        } \
        void store_expected_arg0(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_0 = true; \
        } \
        template<class T> void store_expected_arg1(T value) \
        { \
            STORE_EXPECT_VALUE(this, 1, value); \
        } \
        void store_expected_arg1(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_1 = true; \
        } \
        template<class T> void store_expected_arg2(T value) \
        { \
            STORE_EXPECT_VALUE(this, 2, value); \
        } \
        void store_expected_arg2(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_2 = true; \
        } \
        int expect_exec_times = 0; \
        int expect_atleast_exec_times = 0; \
        int actual_exec_times = 0; \
        bool is_atleast = false; \
        int num_of_matched_parameter = 0; \
        MAKE_SEPARATED_ARGS(func_type, args); \
        bool is_ignore_arg_0 = false; \
        bool is_ignore_arg_1 = false; \
        bool is_ignore_arg_2 = false; \
    }; \
    template<class T0, class T1, class T2> mock_##func& operator()(const T0 &arg0, const T1 &arg1, const T2 &arg2) \
    { \
        OsalFWHelper::shared_ptr<OsalMockMethod> tmp(new OsalMockMethod()); \
        p_base_obj[index] = tmp; \
        p_base_obj[index]->store_expected_arg0(arg0); \
        p_base_obj[index]->store_expected_arg1(arg1); \
        p_base_obj[index]->store_expected_arg2(arg2); \
        p_base_obj[index]->expect_exec_times = 1; \
        index++; \
        return *this; \
    } \
    template<class T> void Times(const T& value) \
    { \
        p_base_obj[index - 1]->expect_exec_times = value; \
    } \
    void Times(OsalTestFrameworkMatcher::matcher_info& obj) \
    { \
        p_base_obj[index - 1]->expect_atleast_exec_times = obj.atleast_value; \
        p_base_obj[index - 1]->is_atleast = true; \
    } \
    size_t index = 0; \
    OsalFWHelper::shared_ptr<OsalMockMethod> p_base_obj[MAX_NUM_EXPECT_CALL]; \
}; \
mock_##func object_##func; \
func_type func(MAKE_OWN_ARGS(func_type, args)) __VA_ARGS__ \
{ \
    bool is_called = true; \
    bool ret[3] = {false, false, false}; \
    DO_CHECK_PARAMETER(object_##func, has_const, 0, ret[0]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 1, ret[1]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 2, ret[2]); \
    DO_CHECK_MATCHED_PARAM(object_##func, has_const, 3); \
    for(auto i = 0; i < 3; i++) \
    { \
        if(true != ret[i]) \
        { \
            is_called = false; \
            break; \
        } \
    } \
    if(true == is_called) \
    { \
        LOG_INFO("Callback function (%s) was called \n", __func__); \
    } \
    return (func_type)0; \
}

#define MOCK_METHOD_PRO_4(func_type, func, args, has_const, ...) \
class mock_##func \
{ \
public: \
    mock_##func(){} \
    ~mock_##func(){} \
    struct OsalMockMethod { \
        OsalMockMethod(){} \
        ~OsalMockMethod() \
        { \
            if(false == is_atleast) \
            { \
                if(expect_exec_times != actual_exec_times) \
                { \
                    LOG_INFO("Failure, Expected exection time is %d, but the actual execution time is %d\n", expect_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
            else \
            { \
                if(expect_atleast_exec_times > actual_exec_times) \
                { \
                    LOG_INFO("Failure, Atleast expected exection time is %d, but the actual execution time is %d\n", expect_atleast_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
        } \
        template<class T> void store_expected_arg0(T value) \
        { \
            STORE_EXPECT_VALUE(this, 0, value); \
        } \
        void store_expected_arg0(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_0 = true; \
        } \
        template<class T> void store_expected_arg1(T value) \
        { \
            STORE_EXPECT_VALUE(this, 1, value); \
        } \
        void store_expected_arg1(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_1 = true; \
        } \
        template<class T> void store_expected_arg2(T value) \
        { \
            STORE_EXPECT_VALUE(this, 2, value); \
        } \
        void store_expected_arg2(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_2 = true; \
        } \
        template<class T> void store_expected_arg3(T value) \
        { \
            STORE_EXPECT_VALUE(this, 3, value); \
        } \
        void store_expected_arg3(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_3 = true; \
        } \
        int expect_exec_times = 0; \
        int expect_atleast_exec_times = 0; \
        int actual_exec_times = 0; \
        bool is_atleast = false; \
        int num_of_matched_parameter = 0; \
        MAKE_SEPARATED_ARGS(func_type, args); \
        bool is_ignore_arg_0 = false; \
        bool is_ignore_arg_1 = false; \
        bool is_ignore_arg_2 = false; \
        bool is_ignore_arg_3 = false; \
    }; \
    template<class T0, class T1, class T2, class T3> mock_##func& operator()(const T0 &arg0, const T1 &arg1, const T2 &arg2, const T3 &arg3) \
    { \
        OsalFWHelper::shared_ptr<OsalMockMethod> tmp(new OsalMockMethod()); \
        p_base_obj[index] = tmp; \
        p_base_obj[index]->store_expected_arg0(arg0); \
        p_base_obj[index]->store_expected_arg1(arg1); \
        p_base_obj[index]->store_expected_arg2(arg2); \
        p_base_obj[index]->store_expected_arg3(arg3); \
        p_base_obj[index]->expect_exec_times = 1; \
        index++; \
        return *this; \
    } \
    template<class T> void Times(const T& value) \
    { \
        p_base_obj[index - 1]->expect_exec_times = value; \
    } \
    void Times(OsalTestFrameworkMatcher::matcher_info& obj) \
    { \
        p_base_obj[index - 1]->expect_atleast_exec_times = obj.atleast_value; \
        p_base_obj[index - 1]->is_atleast = true; \
    } \
    size_t index = 0; \
    OsalFWHelper::shared_ptr<OsalMockMethod> p_base_obj[MAX_NUM_EXPECT_CALL]; \
}; \
mock_##func object_##func; \
func_type func(MAKE_OWN_ARGS(func_type, args)) __VA_ARGS__ \
{ \
    bool is_called = true; \
    bool ret[4] = {false, false, false, false}; \
    DO_CHECK_PARAMETER(object_##func, has_const, 0, ret[0]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 1, ret[1]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 2, ret[2]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 3, ret[3]); \
    DO_CHECK_MATCHED_PARAM(object_##func, has_const, 4); \
    for(auto i = 0; i < 4; i++) \
    { \
        if(true != ret[i]) \
        { \
            is_called = false; \
            break; \
        } \
    } \
    if(true == is_called) \
    { \
        LOG_INFO("Callback function (%s) was called \n", __func__); \
    } \
    return (func_type)0; \
}

#define MOCK_METHOD_PRO_5(func_type, func, args, has_const, ...) \
class mock_##func \
{ \
public: \
    mock_##func(){} \
    ~mock_##func(){} \
    struct OsalMockMethod { \
        OsalMockMethod(){} \
        ~OsalMockMethod() \
        { \
            if(false == is_atleast) \
            { \
                if(expect_exec_times != actual_exec_times) \
                { \
                    LOG_INFO("Failure, Expected exection time is %d, but the actual execution time is %d\n", expect_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
            else \
            { \
                if(expect_atleast_exec_times > actual_exec_times) \
                { \
                    LOG_INFO("Failure, Atleast expected exection time is %d, but the actual execution time is %d\n", expect_atleast_exec_times, actual_exec_times); \
                    ADD_FAILURE(); \
                } \
            } \
        } \
        template<class T> void store_expected_arg0(T value) \
        { \
            STORE_EXPECT_VALUE(this, 0, value); \
        } \
        void store_expected_arg0(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_0 = true; \
        } \
        template<class T> void store_expected_arg1(T value) \
        { \
            STORE_EXPECT_VALUE(this, 1, value); \
        } \
        void store_expected_arg1(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_1 = true; \
        } \
        template<class T> void store_expected_arg2(T value) \
        { \
            STORE_EXPECT_VALUE(this, 2, value); \
        } \
        void store_expected_arg2(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_2 = true; \
        } \
        template<class T> void store_expected_arg3(T value) \
        { \
            STORE_EXPECT_VALUE(this, 3, value); \
        } \
        void store_expected_arg3(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_3 = true; \
        } \
        template<class T> void store_expected_arg4(T value) \
        { \
            STORE_EXPECT_VALUE(this, 4, value); \
        } \
        void store_expected_arg4(OsalTestFrameworkMatcher::AnyMatcher value) \
        { \
            is_ignore_arg_4 = true; \
        } \
        int expect_exec_times = 0; \
        int expect_atleast_exec_times = 0; \
        int actual_exec_times = 0; \
        bool is_atleast = false; \
        int num_of_matched_parameter = 0; \
        MAKE_SEPARATED_ARGS(func_type, args); \
        bool is_ignore_arg_0 = false; \
        bool is_ignore_arg_1 = false; \
        bool is_ignore_arg_2 = false; \
        bool is_ignore_arg_3 = false; \
        bool is_ignore_arg_4 = false; \
    }; \
    template<class T0, class T1, class T2, class T3, class T4> mock_##func& operator()(const T0 &arg0, const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4) \
    { \
        OsalFWHelper::shared_ptr<OsalMockMethod> tmp(new OsalMockMethod()); \
        p_base_obj[index] = tmp; \
        p_base_obj[index]->store_expected_arg0(arg0); \
        p_base_obj[index]->store_expected_arg1(arg1); \
        p_base_obj[index]->store_expected_arg2(arg2); \
        p_base_obj[index]->store_expected_arg3(arg3); \
        p_base_obj[index]->store_expected_arg4(arg4); \
        p_base_obj[index]->expect_exec_times = 1; \
        index++; \
        return *this; \
    } \
    template<class T> void Times(const T& value) \
    { \
        p_base_obj[index - 1]->expect_exec_times = value; \
    } \
    void Times(OsalTestFrameworkMatcher::matcher_info& obj) \
    { \
        p_base_obj[index - 1]->expect_atleast_exec_times = obj.atleast_value; \
        p_base_obj[index - 1]->is_atleast = true; \
    } \
    size_t index = 0; \
    OsalFWHelper::shared_ptr<OsalMockMethod> p_base_obj[MAX_NUM_EXPECT_CALL]; \
}; \
mock_##func object_##func; \
func_type func(MAKE_OWN_ARGS(func_type, args)) __VA_ARGS__ \
{ \
    bool is_called = true; \
    bool ret[5] = {false, false, false, false, false}; \
    DO_CHECK_PARAMETER(object_##func, has_const, 0, ret[0]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 1, ret[1]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 2, ret[2]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 3, ret[3]); \
    DO_CHECK_PARAMETER(object_##func, has_const, 4, ret[4]); \
    DO_CHECK_MATCHED_PARAM(object_##func, has_const, 5); \
    for(auto i = 0; i < 5; i++) \
    { \
        if(true != ret[i]) \
        { \
            is_called = false; \
            break; \
        } \
    } \
    if(true == is_called) \
    { \
        LOG_INFO("Callback function (%s) was called \n", __func__); \
    } \
    return (func_type)0; \
}

#define EXPECT_CALL(mock_obj, func) expect_call_pro(mock_obj.object_##func)

// get and parse the type of input arguments
namespace OsalGetArgsType
{
    template <typename T>
    struct GetArgType {using type = T;};

    template <typename... T>
    struct GetArgTypeHelper : GetArgType<T>... {};

    // primary template
    template <typename T>
    struct OsalFunction;

    // partial specialization which specializes for function type:
    // R is the return value of function
    // Args... is the parameter pack including function's arguments.
    template <typename R, typename... Args>
    struct OsalFunction<R(Args...)>
    {
        using Arg = GetArgTypeHelper<Args...>;
    };
}

template<class U> inline U& expect_call_pro(U& this_args)
{
    return this_args;
}

#endif
