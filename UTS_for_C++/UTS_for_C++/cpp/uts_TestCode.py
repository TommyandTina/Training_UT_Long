##
# @file uts_TestCode.py
#
# @brief Below is brief information of Test code generation.
#
# @section Description
# This is a sub-module of Test Extraction module.
# It supports generating test code based on pattern info from Test data generation.
#
# @section Notes
# - None
#
# @section Restriction
# - None.
#
# @section History
# - 2020.07.06  Create Baseline
#
# Copyright(c) 2020 Renesas Electoronics Corporation

# Imports
import os
from os import path
import openpyxl
import re
from ast import literal_eval

# Global Constants
# Enable to show debug log during execution
DEBUG_LOG										= True

## Local and global data.
DATA_GLOBAL 									= 0
DATA_LOCAL										= 1

## Supported input formats.
FORMAT_INPUT_NORMAL								= 0
FORMAT_INPUT_POINTER							= 1
FORMAT_INPUT_POINTER_TO_POINTER					= 2
FORMAT_INPUT_ARRAY_OF_POINTER					= 3

## Supported output formats
FORMAT_OUTPUT_TYPE_NORMAL						= 0
FORMAT_OUTPUT_TYPE_POINTER 						= 1
FORMAT_OUTPUT_TYPE_POINTER_VALUE 				= 2
FORMAT_OUTPUT_TYPE_POINTER_ARRAY 				= 3
FORMAT_OUTPUT_TYPE_NOTCHANGE					= 4

## Supported output value formats
FORMAT_OUTPUT_VALUE_NORMAL 						= 0
FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM 				= 1
FORMAT_OUTPUT_VALUE_ARRAY_TWO_DIM 				= 2
FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM 			= 3

# List of key value to get data from test pattern dictionary
##############################################################
## global config from input test pattern
g_str_PATTERN_DATA_IN_LOCAL					= "local"
g_str_PATTERN_DATA_IN_GLOBAL				= "global"
## output check type
g_str_PATTERN_DATA_OUT_NOT_CHANGE			= "not_change"
g_str_PATTERN_DATA_OUT_VALUE				= "value"
g_str_PATTERN_DATA_OUT_POINTER				= "pointer"
g_str_PATTERN_DATA_OUT_POINTER_VALUE		= "pointer_value"
g_str_PATTERN_DATA_OUT_POINTER_ARRAY		= "pointer_array"

## list item type for output element
g_str_PATTERN_ITEM_PARAMETER				= "parameter"
g_str_PATTERN_ITEM_LOCAL_FUNC				= "local_func"
g_str_PATTERN_ITEM_GLOBAL_PARAMETER			= "global_parameter"
g_str_PATTERN_ITEM_RETURN_VALUE				= "return_value"
## global config for pattern info keys
## input element info
g_str_PATTERN_INPUT_PARAM_TYPE				= "input_param_type"
g_str_PATTERN_INPUT_PARAM_NAME				= "input_param_name"
g_str_PATTERN_INPUT_DATA_TYPE				= "input_data_type"
g_str_PATTERN_INPUT_ITEM_TYPE				= "input_item_type"
## output element info
g_str_PATTERN_OUTPUT_PARAM_TYPE				= "output_param_type"
g_str_PATTERN_OUTPUT_PARAM_NAME				= "output_param_name"
g_str_PATTERN_EXPECTED_PARAM_NAME			= "expected_param_name"
g_str_PATTERN_OUTPUT_DATA_TYPE				= "output_data_type"
g_str_PATTERN_OUTPUT_ITEM_TYPE				= "output_item_type"
## for getting string pointer name of expect output
g_str_PATTERN_OUTPUT_POINTER_NAME			= "output_pointer_names"
## name return value of stub is fixed as this name
g_str_PATTERN_STUB_RETURN_NAME				= "ReturnValue"
## variable name for function parameter structure
g_str_PATTERN_FUNC_PARAM_NAME				= "func_params"
# End of list
##############################################################

## List of global config from input config file
##############################################################
g_bool_config_enabled_import_data			= None
g_str_config_target_module					= None
g_dict_config_table_validator_by_name		= None
g_dict_config_table_validator_by_type		= None
g_dict_config_init_data						= None
g_list_config_pointer_type					= None
## macro of "null" and "not null" values
g_str_config_NOT_NULL						= None
g_str_config_NULL							= None
g_str_config_TEST_ADDR_NOT_NULL				= None
g_str_config_TEST_ADDR_NULL					= None
# End of list
##############################################################

# List of global config after handled input pattern data
##############################################################
## Config ouput test code when enabled validator
g_bool_enabled_validator				= True
g_str_checkbit_validation				= ""
g_str_checkbit_postcond					= ""
g_str_checkbit_indent					= ""
g_str_checkbit_open_brace				= ""
g_str_checkbit_close_brace				= ""
## General info
g_str_func_params						= ""
g_bool_input_element					= False
g_bool_input_params						= False
g_bool_output_return					= False
g_bool_output_element					= False
## list of TEST_ADDR_XXX macros and their strings
g_list_test_addr_macro					= []
g_list_test_addr_string					= []
# End of list
##############################################################

## List of global config for UT test environment
##############################################################
g_str_ENV_GLOBAL_VAR_PREFIX				= "g_uts_"
g_str_ENV_GLOBAL_POINTER_VAR_PREFIX		= "g_p_uts_"
g_str_ENV_VALIDATE_POINTER_MACRO		= "TEST_VALIDATOR_ADDR"
g_str_ENV_VALIDATE_MEMORY_MACRO			= "TEST_VALIDATOR_MEMORY"
g_str_ENV_VALIDATOR_TYPE				= "bool"
g_str_ENV_TEST_VALIDATOR_I32VALUE		= "TEST_VALIDATOR_I32VALUE"
g_str_ENV_VOID_TYPE						= "void"
# End of list
##############################################################

############################ Internal function   ###################################
## This function converts a string to TEST_ADDR_ABC macro.
# @param str_value		The input string need to be converted
# @return str_macro		A macro string
def testcode_convert_string_to_macro(str_value):
	# Latin alphabet or numbers.	A-Z, a-z, 0-9
	# Bit wise operators.			>>, <<, !, ~. &&, ||
	# Arithmetic operations			+, -, /, *, %
	# Additional C systax.			[, ], .
	# remove list character from the pattern
	str_value = re.sub(r'^{|}$', "", str_value)
	
	# check if string value is "don't care" value
	if str_value == '-':
		str_macro = '0'
	# check if string value is existing in the list
	elif str_value in g_list_test_addr_string:
		str_macro = g_list_test_addr_macro[g_list_test_addr_string.index(str_value)]
	else:
		# append string value to the list
		g_list_test_addr_string.append(str_value)

		# handle macro conversion
		str_temp_value = str_value.upper()
		str_temp_value = re.sub("]", "", str_temp_value)
		str_temp_value = re.sub("[^0-9a-zA-Z_]", "_", str_temp_value)
		str_macro = "TEST_ADDR_{}".format(str_temp_value)

		# check if the macro is existing in the list, update for a new macro
		if str_macro in g_list_test_addr_macro:
			idx = 1
			while (str_macro + str(idx)) in g_list_test_addr_macro:
				idx += 1
			str_macro = str_macro + str(idx)

		# append string macro value to the list
		g_list_test_addr_macro.append(str_macro)

	return str_macro

## This function evaluates an input string and returns a new object coresponding to expected type
# @param str_value		The input string need to be evaluated
# @param checked_type	Expected type of input str_value
# @return config_value	An object value with convert_type type
def testcode_convert_python_type(str_value, checked_type):
	# Check if value is correct Python Syntax
	try:
		config_value = literal_eval(str_value)
	# If not correct Python syntax, raise an Exception
	except:
		raise Exception("[E_UTS_CONF] Invalid configuration format: Value: {}. Expected syntax is {}.".format(str_value, str(checked_type)))

	# Check if config_value is same type of convert_type
	# Example list, dictionary, boolean,...
	if isinstance(config_value, checked_type):
		config_value = checked_type(config_value)
	# If not correct input type, raise Exception
	else:
		raise Exception("[E_UTS_CONF] Invalid configuration format: Value: {}. Expected input type is {}.".format(str_value, str(checked_type)))

	return config_value

## This function checks if parameter type is a pointer or not.
# Note that UTS tool does not support typedef double pointer type
# @param param_type		The param type string
# @return is_pointer	Check result in boolean
# If True, input param type is a pointer. If FaLse, input param type is not a pointer.
def testcode_check_pointer_type(param_type):
	# check param in list
	is_pointer = param_type in g_list_config_pointer_type
	return is_pointer

## This function checks if parameter name is a pointer or not.
# @param param_name		The param name string
# @return is_pointer	Check result in boolean
# If True, input param name is a pointer. If FaLse, input param name is not a pointer.
def testcode_check_pointer_name(param_name):
	# check "*" in param name: *param
	is_pointer = param_name.startswith("*")
	return is_pointer

## This function removes pointer sign (*) from a string.
# @param string		Input string
# @return string	String result after removing pointer sign
def testcode_remove_pointer(string):
	#newstr = re.sub(r'^\*+', '', string)
	obj_match = re.search(r'^\*', string)
	while obj_match != None:
		string = string.replace(obj_match.group(), "")
		obj_match = re.search(r'^\*', string)

	return string

## This function removes array info from a string.
# @param string		Input string
# @return string	String result after removing array info
def testcode_remove_array(string):
    # RegExp pattern explaination
	# \[ : Match an opening bracket for array
	# \d+: Match 1 or more digit character (0-9)
	# (\.{3}|\…): Match a dot "." OR three dot \... 
	# \d*: Match 0 or more digit character (0-9)
	# \]$: Match ending bracket for array at the end of string
	obj_match = re.search(r'\[(\d+(\.{3}|\…)\d+|\d*)\]$', string)
	while obj_match != None:
    	# string.rpartition is used for replace only last occurance
		string, _, _ = string.rpartition(obj_match.group())
		obj_match = re.search(r'\[(\d+(\.{3}|\…)\d+|\d*)\]$', string)

	return string

## This function gets array size of a string.
# @param string			Input string
# @return int_arr_num	Array size in integer type
def testcode_get_num_array(string):
	# get array size
	# arr		->		return 0
	# arr[3]	->		return 3
	# arr[3][4]	->		return 3
	# arr[3][4][5] ->	return 3
	int_arr_num = 0
	while True:
		obj_match = re.search(r'\[(\d+)\]$', string)
		if obj_match == None:
			break
		else:
			int_arr_num = int(obj_match.group(1))
			string = string.replace(obj_match.group(), "")

	return int_arr_num

## This function gets paramter name from a string.
# @param str_pattern		Input string pattern
# @return str_param_name	Parameter info from input string
# @return str_patern_param_name First match string pattern for raw param name
def testcode_get_param_name_from_string(str_pattern):
	# get parameter name from a string
	# example pattern for getting "test" string
	# "test->a",
	# "&test->a",
	# "(uint32 *)&test->a",
	# "test",
	# "test.a",
	# "test->state",
	# "(uint32_t)test->state",
	# "(RCvIMPRTT_RTTINFO_T *)(&test->RttInfo[0])->state[14]",
	# "(RCvIMPRTT_RTTINFO_T *)(&test)->state[14]",
	# "(RCvIMPRTT_RTTINFO_T *)(&((struct test *)&test)->RttInfo[0])->state[2]",
	# "(RCvIMPRTT_RTTINFO_T *)(&((struct test *)&test[3])->RttInfo[0])->state[2]",
	# "(RCvIMPRTT_RTTINFO_T *)(&((struct test *)&test[3][4])->RttInfo[0])->state[2]"
	str_param_name = ""
	str_patern_param_name = ""

	str_tmp = str_pattern.split("->")[0]
	str_tmp = str_tmp.split(".")[0]

	str_patern_param_name = str_tmp

	list_match = re.split(r'[\W]+', str_tmp)
	if list_match != None:
		# check if last string is array info
		list_match.reverse()
		for str_match in list_match:
			if str_match != '' and not str_match.isnumeric():
				str_param_name = str_match
				break

	return str_param_name, str_patern_param_name

## This function gets list of pointer from an parameter string
# @param str_param_name		Input string pattern
# @param str_addr			"&" if str_param_name is not a pointer, or "" if str_param_name is a pointer
# @return list_param_names	List of pointer string contains in input parameter string
def testcode_get_pointer_list_in_param_name(str_param_name, str_addr):
	# extract pointer variables to check null before doing "copy" command
	#ex.
	# "test->a",
	# "&test->a",
	# "(RCvIMPRTT_RTTINFO_T *)(&((struct test *)&func.test[3][4])->RttInfo[0])->state[2]"
	list_param_names = []
	str_param_name_tmp = str_param_name

	# append first pointer type
	if str_addr == "":
		list_param_names.append(str_param_name_tmp)

	# pattern to find a couple of brackets
	pattern = re.compile(r'\([^()]+\)')

	# loop for getting pointer list
	while True:
		# find the last match of "->" character
		int_idx = str_param_name_tmp.rfind("->")
		if int_idx == -1:
			# break the loop when no matching
			break
		str_param_name_tmp = str_param_name_tmp[:int_idx]
		str_chk_param_name = str_param_name_tmp

		# handle for complicated pattern (casting data types)
		if str_chk_param_name.endswith(")"):
			# get index of last ")" character
			int_close_idx = len(str_chk_param_name)
			str_chk_param_name_tmp = str_chk_param_name
			bool_checking = True
			while bool_checking:
				iter_match = pattern.finditer(str_chk_param_name_tmp)
				bool_checking = False
				# replace all matched brackets to find the open bracket
				for match in iter_match:
					bool_checking = True
					int_start = match.start()
					int_end = match.end()
					if int_end == int_close_idx:
						# return string in matched brackets
						str_chk_param_name = str_chk_param_name[int_start + 1 : int_end - 1]
						bool_checking = False
						break
					else:
						# replace all other brackets to find target brackets
						str_chk_param_name_tmp = str_chk_param_name_tmp[:int_start] + "_" + str_chk_param_name_tmp[int_start + 1 : int_end - 1] +\
							"_" + str_chk_param_name_tmp[int_end:]
		else:
			# handle to get the valid string to check pointer state
			# (uint32 *)a
			# (uint32_t *)(a
			str_chk_param_name_tmp = str_chk_param_name
			bool_checking = True
			while bool_checking:
				iter_match = pattern.finditer(str_chk_param_name_tmp)
				bool_checking = False
				for match in iter_match:
					bool_checking = True
					int_start = match.start()
					int_end = match.end()
					# replace all other brackets to find target brackets
					str_chk_param_name_tmp = str_chk_param_name_tmp[:int_start] + "_" + str_chk_param_name_tmp[int_start + 1 : int_end - 1] +\
						"_" + str_chk_param_name_tmp[int_end:]

			# if there remains an open bracket, update string for pointer name
			int_idx = str_chk_param_name_tmp.rfind("(")
			if int_idx != -1:
				str_chk_param_name = str_chk_param_name[int_idx + 1:]

		# do not need check an valid address
		if not str_chk_param_name.startswith("&"):
			list_param_names.append(str_chk_param_name)

	# reserve list for checking null pointer
	list_param_names.reverse()

	return list_param_names

## This function gets output memeber format for validation
# @param str_expected_msg_info		Message information for the validation
# @param str_member					Member parameter name
# @param str_member_type			Member parameter type
# @return str_marco_validator		Validator macro string
# @return str_addr					"&" if member is a value, "" is member is a pointer
# @return str_value					"" if member is a value, "*" is member is a pointer
# @return bool_is_arr				True if member is an array type
# @return str_arr_sub1				Array size if member is an array type
# @return str_member				Updated member parameter name for validation
def testcode_get_output_member_format(str_expected_msg_info, str_member, str_member_type):
	# get validator flag for member
	str_marco_validator = testcode_get_validator_flag(str_expected_msg_info, str_member_type, str_member)

	# handle member is pointer type
	if testcode_check_pointer_name(str_member) or testcode_check_pointer_type(str_member_type):
		str_addr = ""
		str_value = "*"
		# always use this macro type for memory validation
		str_marco_validator = g_str_ENV_VALIDATE_MEMORY_MACRO
	else:
		str_addr = "&"
		str_value = ""

	bool_is_arr = False
	# handle member is array type
	# only check value format for member array types or normal type
	_, flag_value_format, str_arr_sub1, _, str_var = testcode_check_output_formats(str_member, g_str_PATTERN_DATA_OUT_VALUE)

	if flag_value_format == FORMAT_OUTPUT_VALUE_ARRAY_TWO_DIM:
		str_member = testcode_remove_array(str_member)
		# always use this macro type for 2 demention validation
		str_marco_validator = g_str_ENV_VALIDATE_MEMORY_MACRO
	elif flag_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or flag_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
		bool_is_arr = True
		str_member = str_var
	else:
		pass
	
	return str_marco_validator, str_addr, str_value, bool_is_arr, str_arr_sub1, str_member

## This function extracts parameter info from an dictionary format
# @param dict_data		Param info in dictionary format
# @return str_param		Parameter name
# @return list_mem		List of parameter members
# @return list_type		List of parameter types coresponding to each members
def testcode_process_data_dictionary_format(dict_data):
	str_param = ""
	list_mem = []
	list_mem_type = []

	# format dictionary: {'param':['mem1' , 'mem2']}
	for key, list_value in dict_data.items():
		str_param = key
		for mem in list_value:
			if mem != '':
				match = re.search(r'(.+) (.+)$', mem)
				if match != None:
					list_mem_type.append(match.group(1))
					list_mem.append(match.group(2))
				else:
					raise Exception("[ERROR] Invalid format for member list ([\"param_type param_name\", \"param_type2 param_name2\"]), info:\n{}".format(dict_data))
		break

	return str_param, list_mem, list_mem_type

## This function classifies input formats.
# @param str_data_type		Data type string
# @param str_param_type		Parameter type string
# @param str_param_name		Parameter name string
# @return flag_type			Format flag for input param type
# @return flag_data			Format flag for input data type
def testcode_check_input_formats(str_data_type, str_param_type, str_param_name):
	flag_type = 0
	flag_data = 0
	
	if not testcode_check_pointer_name(str_param_name):
		flag_type = FORMAT_INPUT_NORMAL
		if testcode_check_pointer_type(str_param_type):
			flag_type = FORMAT_INPUT_POINTER
			if str_param_name.endswith("]"):
				flag_type = FORMAT_INPUT_ARRAY_OF_POINTER
	elif str_param_name.count("*") == 1 and str_param_name.endswith("]"):
		flag_type = FORMAT_INPUT_ARRAY_OF_POINTER
	elif str_param_name.count("*") == 1:
		flag_type = FORMAT_INPUT_POINTER
		if testcode_check_pointer_type(str_param_type):
			flag_type = FORMAT_INPUT_POINTER_TO_POINTER
	else:
		flag_type = FORMAT_INPUT_POINTER_TO_POINTER

	if str_data_type == g_str_PATTERN_DATA_IN_GLOBAL:
		flag_data = DATA_GLOBAL
	elif str_data_type == g_str_PATTERN_DATA_IN_LOCAL:
		flag_data = DATA_LOCAL
	else:
		raise Exception("[ERROR] Invalid input data type: {}\n".format(str_data_type))

	return flag_type, flag_data

## This function classifies output formats.
# @param str_output_param_name			Parameter name string
# @param str_data_type_output			Parameter type string
# @return flag_output_type_format		Format flag for output param type
# @return flag_output_value_format		Format flag for output data type
# @return str_arr_sub1 					Array size of 1st subscript
# @return str_arr_sub2 					Array size of 2nd subscript
# @return str_var 						Parameter name string
def testcode_check_output_formats(str_output_param_name, str_data_type_output):
	flag_output_type_format = 0
	flag_output_value_format = 0
	str_arr_sub1 = ""
	str_arr_sub2 = ""
	str_var = ""

	if str_data_type_output == g_str_PATTERN_DATA_OUT_POINTER:
		flag_output_type_format = FORMAT_OUTPUT_TYPE_POINTER
	elif str_data_type_output == g_str_PATTERN_DATA_OUT_POINTER_VALUE:
		flag_output_type_format = FORMAT_OUTPUT_TYPE_POINTER_VALUE
	elif str_data_type_output == g_str_PATTERN_DATA_OUT_POINTER_ARRAY:
		flag_output_type_format = FORMAT_OUTPUT_TYPE_POINTER_ARRAY
	elif str_data_type_output == g_str_PATTERN_DATA_OUT_NOT_CHANGE:
		flag_output_type_format = FORMAT_OUTPUT_TYPE_NOTCHANGE
	elif str_data_type_output == g_str_PATTERN_DATA_OUT_VALUE:
		flag_output_type_format = FORMAT_OUTPUT_TYPE_NORMAL
	else:
		raise Exception("[ERROR] Invalid output data type: {}".format(str_data_type_output))

	# check array formats
	if str_output_param_name.endswith("]"):
		# check array formats: test->data_10_[16]
		out_format = re.search(r'(.+)_(\d+)_\[(\d+)\]$', str_output_param_name)
		if out_format != None:
			flag_output_value_format = FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM
			str_arr_sub1 = out_format.group(2)
			str_arr_sub2 = out_format.group(3)
			str_var = out_format.group(1) + "[" + str_arr_sub1 + "]"
		else:
			# check array formats: test->data[10][16]
			out_format = re.search(r'(.+)\[(\d+)\]\[(\d+)\]$', str_output_param_name)
			if out_format != None:
				flag_output_value_format = FORMAT_OUTPUT_VALUE_ARRAY_TWO_DIM
				str_arr_sub1 = out_format.group(2)
				str_arr_sub2 = out_format.group(3)
				str_var = str_output_param_name
			else:
				# check array formats: test->data[16]
				out_format = re.search(r'(.+)\[(\d+)\]$', str_output_param_name)
				if out_format != None:
					flag_output_value_format = FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM
					str_arr_sub1 = out_format.group(2)
					str_var = out_format.group(1)
				else:
					raise Exception("[E_UTS_PCL] Un-supported output expected format for array: {}".format(str_output_param_name))
	else:
		flag_output_value_format = FORMAT_OUTPUT_VALUE_NORMAL
		str_var = str_output_param_name

	return flag_output_type_format, flag_output_value_format, str_arr_sub1, str_arr_sub2, str_var

## This function returns validator string based on parameter type or parameter name.
# @param func_name				Name of testing function
# @param str_validator_type		Parameter type
# @param str_validator_name		Parameter name
# @return marco_validator		Validator string. TEST_VALIDATOR_I32VALUE is returned if input param name and type are not configurated.
def testcode_get_validator_flag(func_name, str_validator_type, str_validator_name):
	# handle pamrameter name due to some complicated format
	#	Input name					: Hanle name
	#	1. index					: index
	#	2. driver_info				: driver_info
	#	3. test->index				: index
	#	4. test->pointer->CoreInfo	: CoreInfo
	#	5. test->data				: data
	# 	6. test->data[10][11].core 	: core
	# 	6. test->data[10][11] 		: data
	match = re.search(r'(.+)(\.|->)(.+)$', str_validator_name)
	if match != None:
		str_validator_name = match.group(3)
		str_validator_name = testcode_remove_array(testcode_remove_pointer(str_validator_name))

	# get raw parameter name for checking
	str_validator_name = testcode_remove_array(testcode_remove_pointer(str_validator_name))

	if str_validator_type in g_dict_config_table_validator_by_type:
		marco_validator = g_dict_config_table_validator_by_type[str_validator_type]
	elif str_validator_name in g_dict_config_table_validator_by_name:
		marco_validator = g_dict_config_table_validator_by_name[str_validator_name]
	else:
		marco_validator = g_str_ENV_TEST_VALIDATOR_I32VALUE
		print("[Warning] Validator default for \"{str_validator_name}\" is {TEST_VALIDATOR_I32VALUE}, function: {func_name}".format(str_validator_name = str_validator_name,\
			TEST_VALIDATOR_I32VALUE = g_str_ENV_TEST_VALIDATOR_I32VALUE, func_name = func_name))

	return marco_validator

## This function returns a group lines of code for setting local function info from test pattern to test environment.
# @param func_name		Local funciton name
# @param list_params	List of params needs to set to test environment
# @return str_format	Group lines of code
def testcode_set_local_function_in(func_name, list_params):
	str_format = ""
	str_tmp = ""
	for str_var in list_params:
		# remove pointer and array info
		str_var = testcode_remove_pointer(testcode_remove_array(str_var))
		# check name to set parameter out for stub function
		if str_var != g_str_PATTERN_STUB_RETURN_NAME:
			str_stub_var = str_var + "_out"
		else:
			str_stub_var = str_var

		str_tmp += "\t\tmemcpy(&g_{func}.{stub_var}, &inputs->{func}.{var}, sizeof(inputs->{func}.{var}));\n".format(func = func_name, var = str_var, stub_var = str_stub_var)

	if str_tmp != "":
		str_format = "\
	if (inputs->{func}.is_stub)\n\
	{{\n\
{str_tmp}\
		g_hook_{func} = ut_stub_{func};\n\
	}}\n".format(func = func_name, str_tmp = str_tmp)

	return str_format

## This function returns a group lines of code for setting parameter in and related global parameter in.
# @param flag_type				Input type format
# @param flag_data				Input value format
# @param str_parameter_in		String parameter name
# @param bool_func_param		If True, the function sets for input param. If False, the function sets for related global parameter in.
# @return str_format			Group lines of code
def testcode_set_common_parameter_in(flag_type, flag_data, str_parameter_in, bool_func_param):
	str_format = ""

	parameter_in_new = testcode_remove_pointer(testcode_remove_array(str_parameter_in))

	# check the condition of function param
	if bool_func_param:
		str_func_param = g_str_PATTERN_FUNC_PARAM_NAME + "."
	else:
		str_func_param = ""

	if flag_type == FORMAT_INPUT_NORMAL:
		str_format = "\tmemcpy(&{func_params}{name}, &inputs->{name}, sizeof({func_params}{name}));\n".format(name = parameter_in_new, func_params = str_func_param)

	elif flag_type == FORMAT_INPUT_POINTER and flag_data == DATA_LOCAL:
		str_format = "\tif (inputs->_{name} == {TEST_ADDR_NULL})\n\
	{{\n\
		{func_params}{name} = NULL;\n\
	}}\n\
	else\n\
	{{\n\
		{func_params}{name} = &inputs->{name};\n\
	}}\n".format(name = parameter_in_new, func_params = str_func_param, TEST_ADDR_NULL = g_str_config_TEST_ADDR_NULL)

	elif flag_type == FORMAT_INPUT_POINTER and flag_data == DATA_GLOBAL:
		str_format = "\tif (inputs->_{name} == {TEST_ADDR_NULL})\n\
	{{\n\
		{func_params}{name} = NULL;\n\
	}}\n\
	else\n\
	{{\n\
		{func_params}{name} = &{stub_var_prefix}{name};\n\
	}}\n".format(name = parameter_in_new, stub_var_prefix = g_str_ENV_GLOBAL_VAR_PREFIX, func_params = str_func_param, TEST_ADDR_NULL = g_str_config_TEST_ADDR_NULL)

	elif flag_type == FORMAT_INPUT_ARRAY_OF_POINTER and flag_data == DATA_LOCAL:
		array_num = testcode_get_num_array(str_parameter_in)
		str_format = "\tif (inputs->_{name} == {TEST_ADDR_NULL})\n\
	{{\n\
		{func_params}{name} = NULL;\n\
	}}\n\
	else\n\
	{{\n".format(name = parameter_in_new, func_params = str_func_param, TEST_ADDR_NULL = g_str_config_TEST_ADDR_NULL)

		# loop to set pointer to array of pointer
		for index in range(array_num):
			str_format += "\t\t{name}[{idx}] = &inputs->{name}[{idx}];\n".format(name = parameter_in_new, idx = str(index))
		str_format += "\t\t{func_params}{name} = {name};\n\
	}}\n".format(name = parameter_in_new, func_params = str_func_param)
	
	elif (flag_type == FORMAT_INPUT_ARRAY_OF_POINTER and flag_data == DATA_GLOBAL) or \
		(flag_type == FORMAT_INPUT_POINTER_TO_POINTER and flag_data == DATA_GLOBAL):
		str_format = "\tif (inputs->_{name} == {TEST_ADDR_NULL})\n\
	{{\n\
		{func_params}{name} = NULL;\n\
	}}\n\
	else\n\
	{{\n\
		{func_params}{name} = &{stub_pointer_var_prefix}{name};\n\
	}}\n".format(name = parameter_in_new, stub_pointer_var_prefix = g_str_ENV_GLOBAL_POINTER_VAR_PREFIX, func_params = str_func_param, TEST_ADDR_NULL = g_str_config_TEST_ADDR_NULL)

	elif (flag_type == FORMAT_INPUT_POINTER_TO_POINTER and flag_data == DATA_LOCAL):
		str_format = "\tif (inputs->_{name} == {TEST_ADDR_NULL})\n\
	{{\n\
		{func_params}{name} = NULL;\n\
	}}\n\
	else\n\
	{{\n\
		{name} = &inputs->{name};\n\
		{func_params}{name} = &{name};\n\
	}}\n".format(name = parameter_in_new, func_params = str_func_param, TEST_ADDR_NULL = g_str_config_TEST_ADDR_NULL)

	else:
		raise Exception("[ERROR] Invalid parameter input format: {}, {}".format(str_parameter_in, flag_type))

	return str_format

## This function returns a group lines of code for setting parameter in.
# @param str_data_type			Input value format in string
# @param str_param_type			Input type format in string
# @param str_param_name			String parameter name
# @return str_format			Group lines of code
def testcode_set_parameter_in(str_data_type, str_param_type, str_param_name):
	# get input param format
	flag_type, flag_data = testcode_check_input_formats(str_data_type, str_param_type, str_param_name)

	str_format = testcode_set_common_parameter_in(flag_type, flag_data, str_param_name, True)

	return str_format

## This function returns a group lines of code for setting related global parameter in.
# @param str_data_type			Input value format in string
# @param str_param_type			Input type format in string
# @param str_param_name			String parameter name
# @return str_format			Group lines of code
def testcode_set_global_parameter_in(str_data_type, str_param_type, str_param_name):
	# get input param format
	flag_type, flag_data = testcode_check_input_formats(str_data_type, str_param_type, str_param_name)

	str_format = testcode_set_common_parameter_in(flag_type, flag_data, str_param_name, False)

	return str_format

## This function returns a group lines of code for getting actual output values after executing target test function.
# @param func_name						Function name
# @param str_item_type					Item type
# @param bool_get_expected_addr			Check flag for calling function for getting expected address pointer.
# @param flag_output_type_format		Output type format
# @param flag_output_value_format		Output data format
# @param str_arr_sub1					1st subscript info of array
# @param str_arr_sub2					2nd subscript info of array
# @param list_member					Member list
# @param str_addr						"&" character in case of non-pointer type. "" character in case of pointer type
# @param str_param_name					Parameter name
# @param str_expected_param_name		Expected parameter name
# @return str_format					Group lines of code
def testcode_post_expected_common(func_name, str_item_type, bool_get_expected_addr, flag_output_type_format, flag_output_value_format, \
	str_arr_sub1, str_arr_sub2, list_member, str_addr, str_param_name, str_expected_param_name):
	str_format = ""

	if flag_output_value_format == FORMAT_OUTPUT_VALUE_NORMAL or flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or \
		flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_TWO_DIM or flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
		str_validate_index_inc = ""
		int_validate_index_inc = 0

		# in case of pointer type, there are a loop for array type to check pointer state
		# so the increase index will not count array info
		if flag_output_type_format == FORMAT_OUTPUT_TYPE_NORMAL:
			if flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM:
				int_validate_index_inc = int(str_arr_sub1)
			elif flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_TWO_DIM:
				pass
			elif flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
				int_validate_index_inc = int(str_arr_sub2)

		# increase validator index for memlist size
		# handle array of struct
		if int_validate_index_inc != 0 and len(list_member) != 0:
			int_validate_index_inc *= len(list_member)
		else:
			int_validate_index_inc += len(list_member)
		
		# increase validate index in case of enable check bit
		if int_validate_index_inc != 0 and g_str_checkbit_postcond != "":
			str_validate_index_inc += "\tindex += {num};\n".format(num = str(int_validate_index_inc))

	# the handle of pointer to array format is the same with pointer to value. It doesn't need the array info
	if (flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM) and \
		(flag_output_type_format != FORMAT_OUTPUT_TYPE_POINTER_ARRAY):
		str_array = "[j]"
		str_indent_array = "\t"
	else:
		str_array = ""
		str_indent_array = ""

	if str_item_type == g_str_PATTERN_ITEM_LOCAL_FUNC:
		str_indent_local_func = "\t"
		str_post_prefix = "_in"
	else:
		str_indent_local_func = ""
		str_post_prefix = ""

	list_param_names = testcode_get_pointer_list_in_param_name(str_param_name, str_addr)
	str_check_null = ""
	str_check_null2 = ""
	str_indent_checknull = ""

	# check array of pointer format
	if (flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_VALUE or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY) and \
			list_param_names != []:
		str_check_null2 = "\tif (" + list_param_names.pop(-1) + str_array + " != NULL)\n"

	for param in list_param_names:
		if str_check_null == "":
			str_check_null += "\tif (" + param + " != NULL"
		else:
			str_check_null += " && " + param + " != NULL"

	if str_check_null != "":
		str_check_null += ")\n"
		str_indent_checknull = "\t"

	if bool_get_expected_addr:
			str_get_expected_addr = "{indent_chk}{indent_ar}\tb &= TEST_GetAddr_{func_name}(&pattern->expected._{val_expected}{str_array}, {func_params});\n".format(\
			func_name = func_name, val_expected = str_expected_param_name, func_params = g_str_func_params, str_array = str_array, indent_ar = str_indent_array, indent_chk = str_indent_checknull)
	else:
		str_get_expected_addr = ""

	if flag_output_type_format == FORMAT_OUTPUT_TYPE_NORMAL:
		# Format for global variable is value:
		#	1. g_core
		#	2. g_impdev.index
		#	3. g_array[3].type
		str_format = "{indent_chk}\tmemcpy(&outputs.{val_expected}, {str_addr}{val_parameter_out}{str_post_prefix}, sizeof(outputs.{val_expected}));\n".format(val_expected = str_expected_param_name, \
	val_parameter_out = str_param_name, str_addr = str_addr, str_post_prefix = str_post_prefix, indent_chk = str_indent_checknull)

	elif flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_VALUE or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY:

		if flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER:
			if len(list_member) != 0:
				raise Exception("[ERROR] Invalid format pattern, pointer will not have member list: {}, {}".format(str_param_name, flag_output_type_format))
			str_set_value = ""
		else:
			#Format pointer:
			# add indent for increase index step
			if str_validate_index_inc != "":
				str_validate_index_inc = "{indent}{indent_ar}{index_inc}".format(index_inc = str_validate_index_inc, indent = g_str_checkbit_indent, indent_ar = str_indent_array)

			#	1. *g_impfwctl:['type' , 'data']
			str_set_value = "{indent_chk}{indent_ar}\tmemcpy(&outputs.{val_expected}{str_array} , {str_addr}{val_parameter_out}{str_post_prefix}{str_array}, sizeof(outputs.{val_expected}{str_array}));\n".format(val_expected = str_expected_param_name,\
				val_parameter_out = str_param_name, indent_ar = str_indent_array, str_addr = str_addr, str_post_prefix = str_post_prefix, indent_chk = str_indent_checknull, str_array = str_array)

			if str_check_null2 != "":
				str_set_value = "\
{indent_chk}{indent_ar}{str_check_null2}\
{indent_chk}{indent_ar}	{{\n\
	{str_set_value}\
{indent_chk}{indent_ar}	}}\n".format(str_set_value = str_set_value, str_check_null2 = str_check_null2, indent_chk = str_indent_checknull, indent_ar = str_indent_array)

		str_check_addr  ="\
{indent_chk}{indent_lf}{indent_ar}	if (pattern->expected._{val_expected}{str_array} == {TEST_ADDR_NULL} || pattern->expected._{val_expected}{str_array} == {TEST_ADDR_NOT_NULL})\n\
{indent_chk}{indent_lf}{indent_ar}	{{\n\
{indent_chk}{indent_lf}{indent_ar}		outputs._{val_expected}{str_array} = ({val_parameter_out}{str_array} == NULL) ? {TEST_ADDR_NULL} : {TEST_ADDR_NOT_NULL};\n\
{indent_chk}{indent_lf}{indent_ar}	}}\n\
{indent_chk}{indent_lf}{indent_ar}	else\n\
{indent_chk}{indent_lf}{indent_ar}	{{\n\
{indent_chk}{indent_lf}{indent_ar}		outputs._{val_expected}{str_array} = (uint32_t){val_parameter_out}{str_array};\n\
{indent_chk}{indent_lf}{indent_ar}	}}\n".format(val_expected= str_expected_param_name, val_parameter_out = str_param_name, indent_chk = str_indent_checknull,\
		TEST_ADDR_NULL = g_str_config_TEST_ADDR_NULL, TEST_ADDR_NOT_NULL = g_str_config_TEST_ADDR_NOT_NULL, str_array = str_array, indent_lf = str_indent_local_func, indent_ar = str_indent_array)

		if str_item_type == g_str_PATTERN_ITEM_LOCAL_FUNC:
			str_check_addr = "\
{indent_chk}{indent_ar}	if ((uint32_t){val_parameter_out}{str_array} != 0xCCCCCCCC)\n\
{indent_chk}{indent_ar}	{{\n\
{str_check_addr}\
{indent_chk}{indent_ar}	}}\n\
{indent_chk}{indent_ar}	else\n\
{indent_chk}{indent_ar}	{{\n\
{indent_chk}{indent_ar}		outputs._{val_expected}{str_array} = 0xCCCCCCCC;\n\
{indent_chk}{indent_ar}	}}\n".format(val_parameter_out = str_param_name, val_expected=str_expected_param_name, str_check_addr = str_check_addr, \
		indent_ar = str_indent_array, str_array = str_array, indent_chk = str_indent_checknull)

		str_check_addr = "\
{str_check_addr}\
{str_set_value}\
{str_get_expected_addr}".format(str_check_addr = str_check_addr, str_get_expected_addr = str_get_expected_addr, str_set_value = str_set_value,)

		# the handle of pointer to array format is the same with pointer to value. Copy output data normally
		if flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY or \
			flag_output_value_format == FORMAT_OUTPUT_VALUE_NORMAL or flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_TWO_DIM:
			#Format pointer:
			#	1. g_impdev.CbFunc
			#	2. *g_impfwctl
			str_format = str_check_addr

		elif flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
			#Format pointer:
			#	2. *g_impfwctl[2]
			str_format  +="\
{indent_chk}	for( int j = 0; j < ARRAY_COUNT(outputs._{val_expected}); j++ )\n\
{indent_chk}	{{\n\
{str_check_addr}\
{indent_chk}	}}\n".format(val_expected= str_expected_param_name, str_check_addr = str_check_addr, indent_chk = str_indent_checknull)

		else:
			raise Exception("[ERROR] Invalid output value format: {}, {}".format(str_param_name, flag_output_value_format))

	elif flag_output_type_format == FORMAT_OUTPUT_TYPE_NOTCHANGE:
		# Format for check point is not change, poiter type ONLY
		#	1. test
		#str_format = "{Post_CHECKBIT}{start_brace}".format(Post_CHECKBIT=g_str_checkbit_postcond, start_brace = g_str_checkbit_open_brace)
		str_format = "{indent_chk}\tmemcpy(&outputs.{val_expected} , {str_addr}{val_parameter_out}, sizeof(outputs.{val_expected}));\n".format(\
			val_expected = str_expected_param_name, val_parameter_out = str_param_name, str_addr=str_addr, indent_chk = str_indent_checknull)

	else:
		raise Exception("[ERROR] Invalid output type format: {}, {}".format(str_param_name, flag_output_type_format))

	if str_check_null != "":
		str_format = "{str_check_null}\
	{{\n\
{str_format}\
	}}\n".format(str_check_null = str_check_null, str_format = str_format)

	str_format += "\n"

	return str_format

## This function returns a group lines of code for getting local function output.
# @param func_name					Function name
# @param str_param_name				String parameter name
# @param dict_expected_info			Output expected info
# @param str_data_type_info			String output data type
# @param bool_get_expected_addr		Check flag for calling function for getting expected address pointer
# @return str_format				Group lines of code
def testcode_post_expected_local_func(func_name, str_param_name, dict_expected_info, str_data_type_info, bool_get_expected_addr):
	# get common info
	str_expected_param_name, list_member, list_member_type = testcode_process_data_dictionary_format(dict_expected_info)
	flag_output_type_format, flag_output_value_format, str_arr_sub1, str_arr_sub2, str_var= testcode_check_output_formats(str_expected_param_name, str_data_type_info)
	
	# get local function name and checking parameter info
	str_local_func_name = str_param_name
	str_local_param_name = testcode_remove_pointer(str_var)

	# handle parameter string for local function output
	# format expected FunctionName_ParamName
	str_expected_param_name = str_local_func_name + "_" + str_local_param_name
	# format target output g_FunctionName.ParamName
	str_param_name = "g_" + str_local_func_name + "." + str_local_param_name
	# format target output g_FunctionName.ParamName_in which is an pointer value
	str_local_func_param_in = "g_" + str_local_func_name + "." + str_local_param_name + "_in"

	return testcode_post_expected_common(func_name, g_str_PATTERN_ITEM_LOCAL_FUNC, bool_get_expected_addr, flag_output_type_format, flag_output_value_format, \
		str_arr_sub1, str_arr_sub2, list_member, "&", str_param_name, str_expected_param_name)

## This function returns a group lines of code for getting parameter output.
# @param func_name							Function name
# @param list_parameter_in					List of parameter input
# @param raw_list_unsupported_validation 	List of parameter inputs which unsupported validation
# @param str_param_name						String parameter name
# @param dict_expected_info					Output expected info
# @param str_data_type_info					String output data type
# @param bool_get_expected_addr				Check flag for calling function for getting expected address pointer
# @return str_format						Group lines of code
def testcode_post_expected_parameter_out(func_name, list_parameter_in, raw_list_unsupported_validation, str_param_name, dict_expected_info, str_data_type_info, bool_get_expected_addr):
	# get common info
	str_expected_param_name, list_member, list_member_type = testcode_process_data_dictionary_format(dict_expected_info)
	flag_output_type_format, flag_output_value_format, str_arr_sub1, str_arr_sub2, str_var= testcode_check_output_formats(str_param_name, str_data_type_info)

	# handle pointer for output parameter
	# expected check type is non-pointer type, but actual type for function param is always pointer type
	# check if output parameter is in input parameter list
	str_var = testcode_remove_pointer(str_var)
	str_raw_param_in, str_patern_param_name = testcode_get_param_name_from_string(str_var)

	if str_raw_param_in in list_parameter_in:
		# raise exception here if it's validating an unsupported format
		if str_raw_param_in in raw_list_unsupported_validation:
			raise Exception("[E_UTS_PCL] Unsupported validating output parameter for an input with double pointer or array of pointer types: {}".format(str_param_name))
		# parameter input is pointer ONLY when cheking output parameter
		if str_raw_param_in == testcode_remove_pointer(testcode_remove_array(str_param_name)) or\
			flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_VALUE or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY:
			str_addr = ""
		else:
			str_addr = "&"
	else:
		raise Exception("[E_UTS_PCL] Invalid name for Parameter out: {}".format(str_param_name))

	# handle parameter string for function param dereference
	# test->state 												-> func_params.test->state
	# (uint32_t)test->state										-> (uint32_t)func_params.test->state
	# (RCvIMPRTT_RTTINFO_T *)(&rcvrttctl->RttInfo[0])->state[14]-> (RCvIMPRTT_RTTINFO_T *)(&func_params.rcvrttctl->RttInfo[0])->state[14]
	# "(RCvIMPRTT_RTTINFO_T *)(&((struct test *)&test[3][4])->RttInfo[0])->state[2] -> (RCvIMPRTT_RTTINFO_T *)(&((struct test *)&func_params.test[3][4])->RttInfo[0])->state[2]"
	int_lst_occur_idx = str_patern_param_name.rfind(str_raw_param_in)
	str_param_name = str_var[:int_lst_occur_idx] + g_str_PATTERN_FUNC_PARAM_NAME + "." + str_var[int_lst_occur_idx:]

	return testcode_post_expected_common(func_name, g_str_PATTERN_ITEM_PARAMETER, bool_get_expected_addr, flag_output_type_format, flag_output_value_format, \
		str_arr_sub1, str_arr_sub2, list_member, str_addr, str_param_name, str_expected_param_name)

## This function returns a group lines of code for getting related global parameter output.
# @param func_name					Function name
# @param str_param_name				String parameter name
# @param dict_expected_info			Output expected info
# @param str_data_type_info			String output data type
# @param bool_get_expected_addr		Check flag for calling function for getting expected address pointer
# @return str_format				Group lines of code
def testcode_post_expected_global_out(func_name, str_param_name, dict_expected_info, str_data_type_info, bool_get_expected_addr):
	# get common info
	str_expected_param_name, list_member, list_member_type = testcode_process_data_dictionary_format(dict_expected_info)
	flag_output_type_format, flag_output_value_format, str_arr_sub1, str_arr_sub2, str_var = testcode_check_output_formats(str_param_name, str_data_type_info)

	# check pointer format for append address character
	if flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_VALUE or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY:
		str_addr = ""
	else:
		str_addr = "&"
	
	# handle string parameter name, remove pointer and array characters
	str_param_name = testcode_remove_pointer(str_var)

	return testcode_post_expected_common(func_name, g_str_PATTERN_ITEM_GLOBAL_PARAMETER, bool_get_expected_addr, flag_output_type_format, flag_output_value_format, \
		str_arr_sub1, str_arr_sub2, list_member, str_addr, str_param_name, str_expected_param_name)

## This function returns a group lines of code for getting actual output for parameter out, local function, related global parameter.
# @param str_param_name				String parameter name
# @param str_expected_param_name	Output expected parameter name
# @param str_expected_msg_info		Expected message info
# @param list_member				List of members
# @param list_member_type			List of member typs coresponding to each members
# @param flag_output_type_format	Output type format
# @param flag_output_value_format	Output data format
# @param str_marco_validator		Validator macro string
# @return str_format				Group lines of code
def testcode_validate_expected_common(str_param_name, str_expected_param_name, str_expected_msg_info, list_member, list_member_type, flag_output_type_format, flag_output_value_format, str_marco_validator):
	str_format = ""

	if flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_VALUE or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY:
		if flag_output_value_format == FORMAT_OUTPUT_VALUE_NORMAL or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY:
			str_format += "{Vali_CHECKBIT}{start_brace}\
{indent}	b &= TEST_ValidateOutput(&outputs->_{val_expected}, &expected->_{val_expected}, sizeof(outputs->_{val_expected}), \"{msg_info}\", {validator});\n{end_brace}\n".format(Vali_CHECKBIT = g_str_checkbit_validation, \
		val_expected= str_expected_param_name, msg_info=str_expected_msg_info, validator = g_str_ENV_VALIDATE_POINTER_MACRO, start_brace = g_str_checkbit_open_brace, end_brace=g_str_checkbit_close_brace, indent= g_str_checkbit_indent)

		elif flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM:
			# get array size
			int_array_size = len(str(testcode_get_num_array(str_param_name)))

			# get size of message info (Number 5 includes 2 for '[', ']' characters, 1 end string charecter, and 2 reserved character)
			int_msg_info_len = len(str_expected_msg_info) + int_array_size + 5

			str_format  +="\
	for( int j = 0; j < ARRAY_COUNT(outputs->_{val_expected}); j++ )\n\
	{{\n\
{indent}{Vali_CHECKBIT}{indent}{start_brace}\
{indent}		char msg_info[{size}] = {{0}};\n\
{indent}		(void)snprintf(msg_info, sizeof(msg_info), \"%s[%d]\", \"{msg_info}\", j);\n\
{indent}		b &= TEST_ValidateOutput(&outputs->_{val_expected}[j], &expected->_{val_expected}[j], sizeof(outputs->_{val_expected}[j]), msg_info, {validator});\n\
{indent}{end_brace}\
	}}\n\n".format(Vali_CHECKBIT =g_str_checkbit_validation, val_expected= str_expected_param_name, msg_info=str_expected_msg_info, validator = g_str_ENV_VALIDATE_POINTER_MACRO, \
		start_brace = g_str_checkbit_open_brace, end_brace=g_str_checkbit_close_brace, indent = g_str_checkbit_indent, size = int_msg_info_len)
	
		else:
			raise Exception("[E_UTS_PCL] Un-supported validating this format with flag: {}, {}".format(str_expected_param_name, flag_output_type_format))

	elif flag_output_type_format == FORMAT_OUTPUT_TYPE_NOTCHANGE:
		# always use this macro type for memory validation
		str_marco_validator = g_str_ENV_VALIDATE_MEMORY_MACRO

		str_format += "{Vali_CHECKBIT}{start_brace}\
{indent}	b &= TEST_ValidateOutput(&outputs->{val_expected}, &expected->{val_expected}, sizeof(outputs->{val_expected}), \"{msg_info}\", {validator});\n{end_brace}\n".format(Vali_CHECKBIT = g_str_checkbit_validation, \
		val_expected= str_expected_param_name, msg_info=str_expected_msg_info, validator = str_marco_validator,start_brace = g_str_checkbit_open_brace, end_brace=g_str_checkbit_close_brace, indent = g_str_checkbit_indent)

	elif flag_output_type_format == FORMAT_OUTPUT_TYPE_NORMAL:
		pass
	else:
		raise Exception("[ERROR] Invalid output type format: {}, {}".format(str_expected_param_name, flag_output_type_format))

	# there are no additional validate value for pointer type and not change
	if flag_output_type_format != FORMAT_OUTPUT_TYPE_POINTER and flag_output_type_format != FORMAT_OUTPUT_TYPE_NOTCHANGE:
		if flag_output_value_format == FORMAT_OUTPUT_VALUE_NORMAL or flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_TWO_DIM:
			# check if there are no member in list
			if len(list_member) == 0:
				# Format for normal value:
				#	1. index
				#	2. test->index
				#	3. test->pointer->type
				#	4. test->data[10][11]
				# 	5. test->data[10][11].type
				# check if the validation is for value of pointer
				if flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_VALUE:
					# *index	->	index
					# g_func.*index -> g_func.index
					str_expected_msg_info = testcode_remove_pointer(str_expected_msg_info)
					# for local function naming rule
					str_expected_msg_info = str_expected_msg_info.replace(".*", ".")

				str_format += "{Vali_CHECKBIT}{start_brace}\
{indent}	b &= TEST_ValidateOutput(&outputs->{val_expected}, &expected->{val_expected}, sizeof(outputs->{val_expected}), \"{msg_info}\", {validator});\n{end_brace}\n".format(Vali_CHECKBIT = g_str_checkbit_validation, \
	val_expected= str_expected_param_name, msg_info=str_expected_msg_info, validator = str_marco_validator, start_brace = g_str_checkbit_open_brace,end_brace=g_str_checkbit_close_brace, indent = g_str_checkbit_indent)

			else:
				# Format for normal value with member list:
				#	1. index 					: ['ch', 'type']
				#	2. driver_info 				: ['coreType', 'coreNum']
				#	3. test->index 				: ['ch', 'type']
				#	4. test->pointer->CoreInfo 	: ['CoreType', 'CoreNum']
				#	5. test->data[10][11] 		: ['type' , 'addr']
				# 	6. test->data[10][11].core 	: ['coreType', 'coreNum']
				# 	6. test->data[10][11].core 	: ['coreType', 'coreNum[5]']
				for idx in range(len(list_member)):
					str_member = list_member[idx]
					str_member_type = list_member_type[idx]

					# get member info based on the format
					str_marco_validator, str_addr, str_value, bool_is_arr, str_arr_size, str_member = testcode_get_output_member_format(str_expected_msg_info, str_member, str_member_type)

					# append member list to message info
					msg_info = str_expected_msg_info + "." + str_member

					if not bool_is_arr:
						str_format += "{Vali_CHECKBIT}{start_brace}\
{indent}	b &= TEST_ValidateOutput({str_addr}outputs->{val_expected}.{member}, {str_addr}expected->{val_expected}.{member}, sizeof({str_value}outputs->{val_expected}.{member}), \"{msg_info}\", {validator});\n{end_brace}\n".format(Vali_CHECKBIT = g_str_checkbit_validation, \
		val_expected=str_expected_param_name, msg_info=msg_info, validator=str_marco_validator, member=str_member, start_brace = g_str_checkbit_open_brace, \
		end_brace=g_str_checkbit_close_brace, indent=g_str_checkbit_indent, str_addr=str_addr, str_value = str_value)
					# handle array format
					else:
						# get size of message info (Number 5 includes 2 for '[', ']' characters, 1 end string charecter, and 2 reserved character)
						int_msg_info_len = len(msg_info) + len(str(str_arr_size)) + 5

						str_format  +="\
	for( int j = 0; j < ARRAY_COUNT(outputs->{val_expected}.{member}); j++ )\n\
	{{\n\
{indent}{Vali_CHECKBIT}{indent}{start_brace}\
{indent}		char msg_info[{size}] = {{0}};\n\
{indent}		(void)snprintf(msg_info, sizeof(msg_info), \"%s[%d]\", \"{msg_info}\", j);\n\
{indent}		b &= TEST_ValidateOutput({str_addr}outputs->{val_expected}.{member}[j], {str_addr}expected->{val_expected}.{member}[j], sizeof({str_value}outputs->{val_expected}.{member}[j]), msg_info, {validator});\n\
{indent}{end_brace}\
	}}\n\n".format(Vali_CHECKBIT =g_str_checkbit_validation, val_expected= str_expected_param_name, msg_info=msg_info, validator = str_marco_validator, \
		start_brace = g_str_checkbit_open_brace, end_brace=g_str_checkbit_close_brace, indent = g_str_checkbit_indent, size = int_msg_info_len, \
			member=testcode_remove_pointer(str_member), str_addr=str_addr, str_value = str_value)

		elif flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
			# get array size
			int_array_size = len(str(testcode_get_num_array(str_param_name)))

			# check if there are no member in list
			if len(list_member) == 0:
				#Format array one dimensional:
				#	1. for test->data[20]

				# get size of message info (Number 5 includes 2 for '[', ']' characters, 1 end string charecter, and 2 reserved character)
				int_msg_info_len = len(str_expected_msg_info) + int_array_size + 5

				str_format  +="\
	for( int j = 0; j < ARRAY_COUNT(outputs->{val_expected}); j++ )\n\
	{{\n\
{indent}{Vali_CHECKBIT}{indent}{start_brace}\
{indent}		char msg_info[{size}] = {{0}};\n\
{indent}		(void)snprintf(msg_info, sizeof(msg_info), \"%s[%d]\", \"{msg_info}\", j);\n\
{indent}		b &= TEST_ValidateOutput(&outputs->{val_expected}[j], &expected->{val_expected}[j], sizeof(outputs->{val_expected}[j]), msg_info, {validator});\n\
{indent}{end_brace}\
	}}\n\n".format(Vali_CHECKBIT =g_str_checkbit_validation, val_expected= str_expected_param_name, msg_info=str_expected_msg_info, validator = str_marco_validator, \
	start_brace = g_str_checkbit_open_brace, end_brace=g_str_checkbit_close_brace, indent = g_str_checkbit_indent, size = int_msg_info_len)

			else:
				#Format array one dimensional with member list:
				#	1. for test->data[20] 		: ['type' , 'addr']
				str_format  +="\tfor( int j = 0; j < ARRAY_COUNT(outputs->{val_expected}); j++ )\n\t{{\n".format(val_expected= str_expected_param_name)
				str_msg_info_var = ""
				str_msg_info_name = ""
				str_msg_info_cat = ""
				str_format_tmp = ""

				for idx in range(len(list_member)):
					str_member = list_member[idx]
					str_member_type = list_member_type[idx]

					# get member info based on the format
					str_marco_validator, str_addr, str_value, bool_is_arr, str_arr_size, str_member = testcode_get_output_member_format(str_expected_msg_info, str_member, str_member_type)

					# handle member list to message info
					msg_info = str_expected_msg_info

					# get size of message info (Number 5 includes 2 for '[', ']' characters, 1 '.' character, 1 end string charecter, and 1 reserved character)
					int_msg_info_len = len(str_expected_msg_info) + len(str_member) + int_array_size + 5
					# add array size of member if member is an array "[num]"
					if bool_is_arr:
						int_msg_info_len += len(str_arr_size) + 2

					# get name of message info variable being to concate
					str_msg_info_name = "msg_info{str_mem}".format(str_mem = idx)

					str_msg_info_var += "\
		char {str_msg_info_name}[{size}] = {{0}};\n".format(size = int_msg_info_len, indent = g_str_checkbit_indent, str_msg_info_name = str_msg_info_name)

					# create string cat code
					if not bool_is_arr:
						str_msg_info_cat = "\
		(void)snprintf({str_msg_info_name}, sizeof({str_msg_info_name}), \"%s[%d].%s\", \"{msg_info}\", j, \"{str_mem}\");\n".format(indent = g_str_checkbit_indent, \
		str_mem = str_member, msg_info = msg_info, str_msg_info_name = str_msg_info_name)
					else:
						str_msg_info_cat = "\
			(void)snprintf({str_msg_info_name}, sizeof({str_msg_info_name}), \"%s[%d].%s[%d]\", \"{msg_info}\", j, \"{str_mem}\", i);\n".format(indent = g_str_checkbit_indent, \
		str_mem = str_member, msg_info = msg_info, str_msg_info_name = str_msg_info_name)

					if not bool_is_arr:
						str_format_tmp  +="\
{indent}{Vali_CHECKBIT}{indent}{start_brace}\
{indent}{str_msg_info_cat}\
{indent}		b &= TEST_ValidateOutput({str_addr}outputs->{val_expected}[j].{str_mem}, {str_addr}expected->{val_expected}[j].{str_mem}, sizeof({str_value}outputs->{val_expected}[j].{str_mem}), {str_msg_info_name}, {validator});\n\
{indent}{end_brace}".format(Vali_CHECKBIT =g_str_checkbit_validation, val_expected= str_expected_param_name, msg_info=msg_info, validator = str_marco_validator, start_brace = g_str_checkbit_open_brace, \
end_brace=g_str_checkbit_close_brace, indent = g_str_checkbit_indent, size = int_msg_info_len, str_addr = str_addr, str_msg_info_name = str_msg_info_name, \
	str_mem = testcode_remove_pointer(str_member), str_value = str_value, str_msg_info_cat = str_msg_info_cat)
					else:
						str_format_tmp  +="\
		for( int i = 0; i < ARRAY_COUNT(outputs->{val_expected}[j].{str_mem}); i++ )\n\
		{{\n\
{indent}{indent}{Vali_CHECKBIT}{indent}{indent}{start_brace}\
{indent}{str_msg_info_cat}\
{indent}			b &= TEST_ValidateOutput({str_addr}outputs->{val_expected}[j].{str_mem}[i], {str_addr}expected->{val_expected}[j].{str_mem}[i], sizeof({str_value}outputs->{val_expected}[j].{str_mem}[i]), {str_msg_info_name}, {validator});\n\
{indent}{indent}{end_brace}\
		}}\n".format(Vali_CHECKBIT =g_str_checkbit_validation, val_expected= str_expected_param_name, msg_info=msg_info, validator = str_marco_validator, start_brace = g_str_checkbit_open_brace, \
end_brace=g_str_checkbit_close_brace, indent = g_str_checkbit_indent, size = int_msg_info_len, str_addr = str_addr, str_msg_info_name = str_msg_info_name, \
	str_mem = str_member, str_value = str_value, str_msg_info_cat = str_msg_info_cat)

				# append info for str_format
				str_format += str_msg_info_var + "\n" + str_format_tmp

				# end for loop
				str_format  +="\t}\n\n"

	return str_format

## This function returns a group lines of code for validating parameter output.
# @param func_name				Function name
# @param str_param_type			Parameter type string
# @param str_param_name			Parameter name string
# @param dict_expected_info		Ouput expected info in dictionary format
# @param str_data_type_info		Data type format string
# @return str_format			Group lines of code
def testcode_validate_expected_parameter_out(func_name, str_param_type, str_param_name, dict_expected_info, str_data_type_info):
	# get general information for validation format
	str_expected_param_name, list_member, list_member_type = testcode_process_data_dictionary_format(dict_expected_info)
	flag_output_type_format, flag_output_value_format, str_arr_sub1, str_arr_sub2, str_var = testcode_check_output_formats(str_param_name, str_data_type_info)

	str_marco_validator = testcode_get_validator_flag(func_name, str_param_type, str_var)

	# change name of message info in case of 1 dimemsion array
	if flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
		str_expected_msg_info = str_var
	else:
		# get expected message info
		str_expected_msg_info  = str_param_name

	# remove pointer from param name
	str_expected_param_name = testcode_remove_pointer(testcode_remove_array(str_expected_param_name))

	str_format = testcode_validate_expected_common(str_param_name, str_expected_param_name, str_expected_msg_info, list_member, list_member_type, \
		flag_output_type_format, flag_output_value_format, str_marco_validator)

	return str_format

## This function returns a group lines of code for validating releted global parameter output.
# @param func_name				Function name
# @param str_param_type			Parameter type string
# @param str_param_name			Parameter name string
# @param dict_expected_info		Ouput expected info in dictionary format
# @param str_data_type_info		Data type format string
# @return str_format			Group lines of code
def testcode_validate_expected_global_out(func_name, str_param_type, str_param_name, dict_expected_info, str_data_type_info):
	# get general information for validation format
	str_expected_param_name, list_member, list_member_type = testcode_process_data_dictionary_format(dict_expected_info)
	flag_output_type_format, flag_output_value_format, str_arr_sub1, str_arr_sub2, str_var = testcode_check_output_formats(str_param_name, str_data_type_info)

	str_marco_validator = testcode_get_validator_flag(func_name, str_param_type, str_var)

	# change name of message info in case of 1 dimemsion array
	if flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
		str_expected_msg_info = str_var
	else:
		# get expected message info
		str_expected_msg_info  = str_param_name

	# remove pointer from param name
	str_expected_param_name = testcode_remove_pointer(testcode_remove_array(str_expected_param_name))

	str_format = testcode_validate_expected_common(str_param_name, str_expected_param_name, str_expected_msg_info, list_member, list_member_type, \
		flag_output_type_format, flag_output_value_format, str_marco_validator)

	return str_format

## This function returns a group lines of code for validating local function output.
# @param func_name				Function name
# @param str_param_type			Parameter type string
# @param str_param_name			Parameter name string
# @param dict_expected_info		Ouput expected info in dictionary format
# @param str_data_type_info		Data type format string
# @return str_format			Group lines of code
def testcode_validate_expected_local_func(func_name, str_param_type, str_param_name, dict_expected_info, str_data_type_info):
	# get general information for validation format
	str_expected_param_name, list_member, list_member_type = testcode_process_data_dictionary_format(dict_expected_info)
	flag_output_type_format, flag_output_value_format, str_arr_sub1, str_arr_sub2, str_var = testcode_check_output_formats(str_expected_param_name, str_data_type_info)

	str_marco_validator = testcode_get_validator_flag(func_name, str_param_type, str_expected_param_name)

	# change name of message info in case of 1 dimemsion array
	if flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
		str_expected_msg_info = str_param_name + "." + str_var
	else:
		# get expected message info
		str_expected_msg_info  = str_param_name + "." + str_expected_param_name

	# get local function name and checking parameter info
	str_local_func_name = str_param_name
	str_local_param_name = testcode_remove_pointer(testcode_remove_array(str_expected_param_name))

	# handle parameter string for local function output
	# format expected FunctionName_ParamName
	str_expected_param_name = str_local_func_name + "_" + str_local_param_name

	str_format = testcode_validate_expected_common(str_param_name, str_expected_param_name, str_expected_msg_info, list_member, list_member_type, \
		flag_output_type_format, flag_output_value_format, str_marco_validator)

	return str_format

## This function returns a group lines of code for validating return value.
# @param func_name				Function name
# @param str_param_type			Parameter type string
# @param str_param_name			Parameter name string
# @param dict_expected_info		Ouput expected info in dictionary format
# @param str_data_type_info		Data type format string
# @return str_format			Group lines of code
def testcode_validate_return_value(func_name, str_param_type, str_param_name, dict_expected_info, str_data_type_info):
	# get general information for validation format
	str_expected_param_name, list_member, list_member_type = testcode_process_data_dictionary_format(dict_expected_info)
	flag_output_type_format, flag_output_value_format, str_arr_sub1, str_arr_sub2, str_var = testcode_check_output_formats(str_param_name, str_data_type_info)

	str_marco_validator = testcode_get_validator_flag(func_name, str_param_type, str_var)

	# change name of message info in case of 1 dimemsion array
	if flag_output_value_format == FORMAT_OUTPUT_VALUE_ARRAY_ONE_DIM or flag_output_value_format == FORMAT_OUTPUT_VALUE_DASH_ARRAY_ONE_DIM:
		str_expected_msg_info = str_var
	else:
		# get expected message info
		str_expected_msg_info  = str_param_name

	# init string format
	str_format = ""

	# check pointer type
	if testcode_check_pointer_name(str_param_name) or testcode_check_pointer_type(str_param_type):
		new_value_return = "_" + testcode_remove_array(testcode_remove_pointer(str_param_name))

		str_format = "\tb &= TEST_ValidateOutput(&outputs->{val_return}, &expected->{val_return}, sizeof(outputs->{val_return}), \"{msg_info}\", {validator});\n\n".format(val_return=new_value_return, \
			msg_info = str_expected_msg_info, validator = str_marco_validator)
	# check if the return is void type
	elif str_param_type == g_str_ENV_VOID_TYPE:
		pass
	# check if the return is a NON-STRUCT type
	elif len(list_member) == 0:
		str_format = "\tb &= TEST_ValidateOutput(&outputs->{val_return}, &expected->{val_return}, sizeof(outputs->{val_return}), \"{msg_info}\", {validator});\n\n".format(val_return=str_param_name, \
			msg_info = str_expected_msg_info, validator = str_marco_validator)

	if len(list_member) != 0 or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_VALUE or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY:
		# remove pointer from param name
		str_expected_param_name = testcode_remove_pointer(testcode_remove_array(str_expected_param_name))
		# change to validate value only
		flag_output_type_format = FORMAT_OUTPUT_TYPE_NORMAL
		# get expected message info
		str_expected_msg_info  = str_expected_param_name

		str_format += testcode_validate_expected_common(str_param_name, str_expected_param_name, str_expected_msg_info, list_member, list_member_type, \
			flag_output_type_format, flag_output_value_format, str_marco_validator)

	return str_format

## This function writes a group lines of code to output file for including necessary header files.
# @param func_name		Function name
# @param fo				Opened file object
# @return None
def testcode_Header(func_name, fo, namespace_value):
		str_format = "#include \"base.h\"\n"
		str_format += "#include \"util.h\"\n"
		str_format += "#include \"patterns/{}_PCL.h\"\n\n".format(func_name)
		if namespace_value != None:
			str_format += "using namespace {};\n".format(namespace_value)
		fo.write(str_format)

## This function checks an parameter whether in a parameter list or not.
# @param param				Checking parameter name
# @param list_params		Input parameter name list
# @param list_type_params	Input parameter type list
# @param list_data_types	Input data type list
# @param list_item_type		Input item type list
# @return None
def testcode_check_param_in_param_list(param, list_params, list_type_params, list_data_types, list_item_type):
	bool_matched = False
	flag_type = 0
	flag_data = 0

	for i in range(len(list_params)):
		# check if param in target param list
		if list_item_type[i] == g_str_PATTERN_ITEM_PARAMETER or list_item_type[i] == g_str_PATTERN_ITEM_GLOBAL_PARAMETER:
			if param == testcode_remove_pointer(testcode_remove_array(list_params[i])):
				bool_matched = True
				# get input param format
				flag_type, flag_data = testcode_check_input_formats(list_data_types[i], list_type_params[i], list_params[i])
				# end loop when param was found
				break

	return bool_matched, flag_type, flag_data

## This function writes a function to get output expected address.
# @param func_name					Function name
# @param fo							Opened file object
# @param dict_pattern_info			Pattern info
# @return bool_get_expected_addr	Flag to check geting expected address function is generated or not
def testcode_get_expected_addr(func_name, fo, dict_pattern_info):
	# get input element info
	list_input_param_name		= dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_NAME)
	list_input_item_type		= dict_pattern_info.get(g_str_PATTERN_INPUT_ITEM_TYPE)
	# get input element info
	list_ouput_pointer_names	= dict_pattern_info.get(g_str_PATTERN_OUTPUT_POINTER_NAME)
	bool_get_expected_addr = False
	list_raw_parameter_in = []

	# get raw parameter in
	for i in range(len(list_input_item_type)):
		if list_input_item_type[i] == g_str_PATTERN_ITEM_PARAMETER:
			list_raw_parameter_in.append(testcode_remove_pointer(testcode_remove_array(list_input_param_name[i])))

	# check if the testing requires getting output pointer string for validation
	if len(list_ouput_pointer_names) != 0:
		str_format = "static bool TEST_GetAddr_{func_name}(uint32_t *flag, struct {func_name}_params *{func_params})\n{{\n".format(func_name = func_name, func_params = g_str_PATTERN_FUNC_PARAM_NAME)
		str_format += "\tbool ret = true;\n\n"
		str_format += "\tswitch (*flag)\n\t{\n"
		# handle information of output pointer names
		for str_pointer in list_ouput_pointer_names:
			# convert to TEST_ADDR_ABC macro
			str_flag = testcode_convert_string_to_macro(str_pointer)

			# set validate string for TEST_ADD_NULL and TEST_ADDR_NOT_NULL macros
			if str_flag == g_str_config_TEST_ADDR_NOT_NULL:
				str_format += "\tcase {str_flag}:\n\
		ret = Test_set_validate_pointer_info((uint32_t){str_flag}, \"{str_pointer}\\0\");\n\
		break;\n".format(str_flag = str_flag, str_pointer = g_str_config_NOT_NULL)

			elif str_flag == g_str_config_TEST_ADDR_NULL:
				str_format += "\tcase {str_flag}:\n\
		ret = Test_set_validate_pointer_info((uint32_t){str_flag}, \"{str_pointer}\\0\");\n\
		break;\n".format(str_flag = str_flag, str_pointer = g_str_config_NULL)

			# set validate string for other macros
			else:
				str_msg = str_pointer
				str_tmp, str_patern_param_name = testcode_get_param_name_from_string(str_pointer)
				if str_tmp in list_raw_parameter_in:
					# find last occurence index of input parameter name
					int_lst_occur_idx = str_patern_param_name.rfind(str_tmp)
					# append "func_params->" string into output pointer name
					str_pointer = str_pointer[:int_lst_occur_idx] + g_str_PATTERN_FUNC_PARAM_NAME + "->" + str_pointer[int_lst_occur_idx:]

				str_format += "\tcase {str_flag}:\n\
		*flag = (uint32_t){str_pointer};\n\
		ret = Test_set_validate_pointer_info((uint32_t){str_pointer}, \"{str_pointer2}\\0\");\n\
		break;\n".format(str_flag = str_flag, str_pointer = str_pointer, str_pointer2 = str_msg.replace("\"", "\\\""))

		str_format += "\tdefault:\n\t\tbreak;\n\t}\n\n"
		str_format += "\treturn ret;\n}\n\n"
		fo.write(str_format)
		bool_get_expected_addr = True

	return bool_get_expected_addr

## This function writes a function to output file for additional initializing input data (semi-auto feature).
# @param func_name					Function name
# @param fo							Opened file object
# @param dict_pattern_info			Pattern info
# @return bool_additional_init		Flag to check additional initializing data function is generated or not
def testcode_Initialize_data(func_name, fo, dict_pattern_info):
	str_format = ""
	bool_additional_init = False
	list_input_param_name		= dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_NAME)
	list_input_param_type		= dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_TYPE)
	list_input_data_type		= dict_pattern_info.get(g_str_PATTERN_INPUT_DATA_TYPE)
	list_input_item_type		= dict_pattern_info.get(g_str_PATTERN_INPUT_ITEM_TYPE)

	bool_matched = True
	str_initialize = "static bool TEST_Initialize_{func_name}(int32_t no)\n{{\n\tbool b = true;\n".format(func_name=func_name)
	str_initialize += "\tstruct {func_name}_input *inputs = &{func_name}_PCL[no - 1].input;\n\n".format(func_name=func_name)

	for handle_str, handle_info in g_dict_config_init_data.items():
		str_func_name = handle_info[0]		# get function name from the list info
		list_params = handle_info[1:]		# get function params from the list info
		# convert to TEST_ADDR_ABC macro
		handle_flag = testcode_convert_string_to_macro(handle_str)
		str_input_params = ""
		str_name = ""
		idx = 0

		for params in list_params:
			# handle main pointer for additional initialization
			if idx == 0:
				for param in params:
					# save the matched name
					str_name = param
					# check input parameter and related global parameter list
					bool_matched, flag_type, flag_data = testcode_check_param_in_param_list(param, list_input_param_name, list_input_param_type, list_input_data_type, list_input_item_type)
					if bool_matched:
						break

				if bool_matched:
					if flag_data == DATA_LOCAL:
						str_input_params += "&inputs->{}".format(param)
					else:
						if flag_type == FORMAT_INPUT_POINTER_TO_POINTER or flag_type == FORMAT_INPUT_ARRAY_OF_POINTER:
							str_stub_prefix = g_str_ENV_GLOBAL_POINTER_VAR_PREFIX
						else:
							str_stub_prefix = g_str_ENV_GLOBAL_VAR_PREFIX

						str_input_params += "&{stub_prefix}{param}".format(stub_prefix = str_stub_prefix, param = param)
				else:
					# continue next parameter
					break
			else:
				# pattern format for param: [defaul value, param name], [0, index], [NULL, core]
				default_value = params[0]
				# save the matched name
				param = params[1]

				# check pointer param type for external function
				if default_value == "NULL":
					str_pointer = "&"
				else:
					str_pointer = ""

				# check input parameter list with config data
				bool_matched, flag_type, flag_data = testcode_check_param_in_param_list(param, list_input_param_name, list_input_param_type, list_input_data_type, list_input_item_type)

				if bool_matched:
					if flag_data == DATA_LOCAL:
						str_input_params += ", {str_pointer}inputs->{param}".format(str_pointer = str_pointer, param = param)
					else:
						if flag_type == FORMAT_INPUT_POINTER_TO_POINTER or flag_type == FORMAT_INPUT_ARRAY_OF_POINTER:
							str_stub_prefix = g_str_ENV_GLOBAL_POINTER_VAR_PREFIX
							#str_pointer = ""
						else:
							str_stub_prefix = g_str_ENV_GLOBAL_VAR_PREFIX

						str_input_params += ", {str_pointer}{stub_prefix}{param}".format(str_pointer = str_pointer, stub_prefix = str_stub_prefix, param = param)
				else:
					str_input_params += ", {}".format(default_value)

			# increase index for next parameter
			idx += 1

		# set format for initilizing code
		if str_input_params != "":
			str_format += "\tif (inputs->_{name} == {marco})\n\t{{\n\t\tb &= {func}({parameter});\n\t}}\n".format(name = str_name, marco = handle_flag, func = str_func_name, parameter= str_input_params)

	# complete code and write to output file
	if str_format != "":
		str_initialize += str_format
		str_initialize += "\n\treturn b;\n}\n\n"
		fo.write(str_initialize)
		bool_additional_init = True

	return bool_additional_init


## This function writes a group lines of code to output file for declaring entry test function.
# @param func_name					Function name
# @param fo							Opened file object
# @param dict_pattern_info			Pattern info
# @return None
def testcode_Maintest(func_name, fo, dict_pattern_info):
	# format string
	str_format = "bool TEST_{}(const char *category, int32_t no)\n{{\n\tbool b = true;\n".format(func_name)
	str_format += "\tstruct TestParams *params = TEST_CreateParam(\"{func_name}\", category, no);\n".format(func_name = func_name)

	if g_bool_input_element or g_bool_input_params or g_bool_output_element or g_bool_output_return:
		str_format += "\tstruct TEST_{func_name}_Pattern *pattern = &{func_name}_PCL[no - 1];\n".format(func_name = func_name)

	if g_bool_input_element or g_bool_input_params:
		str_format += "\tstruct {func_name}_input	*inputs = &pattern->input;\n".format(func_name = func_name)

	if g_bool_input_params:
		str_format += "\tstruct {func_name}_params	{func_params};\n".format(func_name = func_name, func_params = g_str_PATTERN_FUNC_PARAM_NAME)

	if g_bool_output_element or g_bool_output_return:
		str_format += "\tstruct {func_name}_expect	outputs;\n".format(func_name = func_name)

	fo.write(str_format)

## This function writes a group lines of code to output file for setting input values before testing.
# @param func_name					Function name
# @param fo							Opened file object
# @param dict_pattern_info			Pattern info
# @param bool_additional_init		Flag to check additional initializing data function was generated or not
# @return None
def testcode_Precondition(func_name, fo, dict_pattern_info, bool_additional_init):
	# list of input info
	list_input_param_type 				= dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_TYPE)
	list_input_param_name				= dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_NAME)
	list_input_data_type				= dict_pattern_info.get(g_str_PATTERN_INPUT_DATA_TYPE)
	list_input_item_type				= dict_pattern_info.get(g_str_PATTERN_INPUT_ITEM_TYPE)

	# list of info for output validation
	list_output_param_name 				= dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_NAME)
	list_output_param_type 				= dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_TYPE)
	list_output_data_type 				= dict_pattern_info.get(g_str_PATTERN_OUTPUT_DATA_TYPE)
	list_output_item_type 				= dict_pattern_info.get(g_str_PATTERN_OUTPUT_ITEM_TYPE)

	flag_type = 0
	flag_data = 0
	str_format = ""

	# declare ReturnValue in case function return a pointer
	for idx in range(len(list_output_param_name)):
		if list_output_item_type[idx] == g_str_PATTERN_ITEM_RETURN_VALUE:
			if testcode_check_pointer_name(list_output_param_name[idx]) or testcode_check_pointer_type(list_output_param_type[idx]):
					str_format += "\t{} {} = NULL;\n".format(list_output_param_type[idx].replace("const", "").strip(), testcode_remove_array(list_output_param_name[idx]))
					# only one return value for each function call
					break

	# declare variable to handle array of pointer or double pointer input paramters
	for idx in range(len(list_input_param_name)):
		# check item parameter type
		if list_input_item_type[idx] == g_str_PATTERN_ITEM_PARAMETER:
			flag_type, flag_data = testcode_check_input_formats(list_input_data_type[idx], list_input_param_type[idx], list_input_param_name[idx])
			if flag_type == FORMAT_INPUT_ARRAY_OF_POINTER and flag_data ==  DATA_LOCAL:
				str_arr_size = testcode_get_num_array(list_input_param_name[idx])
				str_format += "\t{} {}[{}] = {{0}};\n".format(list_input_param_type[idx], testcode_remove_array(list_input_param_name[idx]), str_arr_size)
			elif flag_type == FORMAT_INPUT_POINTER_TO_POINTER and flag_data ==  DATA_LOCAL:
				# remove an "*" character from input parameter name
				str_param_name = list_input_param_name[idx].replace("*", "", 1)
				str_format += "\t{} {} = NULL;\n".format(list_input_param_type[idx], str_param_name)

	#declare stub flag
	# get list of local function name
	int_stub_cnt = 0
	str_is_stub = "false"
	for idx in range(len(list_input_param_name)):
		# check item local function type
		if list_input_item_type[idx] == g_str_PATTERN_ITEM_LOCAL_FUNC:
			str_is_stub = "is_stub"

			# the pattern for local function info is a dictionary, ex: {'read_register_function':[Return value, Value]}
			for stub_func_name, param_list in list_input_param_name[idx].items():
				stub_func_name = testcode_remove_pointer(stub_func_name)
				if int_stub_cnt == 0:
					str_format += "\tbool {is_stub} = !inputs->{func_name}.is_stub".format(func_name = stub_func_name, is_stub = str_is_stub)
					int_stub_cnt = 1
				else:
					str_format += " | !inputs->{func_name}.is_stub".format(func_name = stub_func_name)

	# append ";" character for stub flag variable
	if str_is_stub != "false":
		str_format += ";\n"

	#Call Init stub
	str_format += "\n\tut_init_{target_module}({is_stub});\n".format(target_module = g_str_config_target_module, is_stub = str_is_stub)

	# write all string to file
	fo.write(str_format)

	# reset string for next setting values
	str_format = ""

	# Call additional handle for precondition testing
	if bool_additional_init:
		str_format += "\tb &= TEST_Initialize_{}(no);\n\n".format(func_name)

	for idx in range(len(list_input_param_name)):
		# check item parameter type
		if list_input_item_type[idx] == g_str_PATTERN_ITEM_PARAMETER:
			str_format += testcode_set_parameter_in(list_input_data_type[idx], list_input_param_type[idx], list_input_param_name[idx])
		# check item global parameter type
		elif list_input_item_type[idx] == g_str_PATTERN_ITEM_GLOBAL_PARAMETER:
			str_format += testcode_set_global_parameter_in(list_input_data_type[idx], list_input_param_type[idx], list_input_param_name[idx])
		# check item local function type
		elif list_input_item_type[idx] == g_str_PATTERN_ITEM_LOCAL_FUNC:
			for func_name, param_list in list_input_param_name[idx].items():
				str_format += testcode_set_local_function_in(func_name, param_list)
		else:
			raise Exception("[ERROR] Invalide input item type: {}".format(list_input_item_type[idx]))

	for idx in range(len(list_output_data_type)):
		if list_output_data_type[idx] == g_str_PATTERN_DATA_OUT_NOT_CHANGE:
			str_format += "\n"
			# store value for "not change" validation of "related global parameter", this case related global parameter is non-pointer type ONLY
			if list_output_item_type[idx] == g_str_PATTERN_ITEM_GLOBAL_PARAMETER:
				str_format += "\tmemcpy(&pattern->expected.{val}, &{val}, sizeof(pattern->expected.{val}));\n".format(val=list_output_param_name[idx])
			# store value for "not change" validation of "parameter out", this case function parameter is a pointer type
			elif list_output_item_type[idx] == g_str_PATTERN_ITEM_PARAMETER:
				str_format += "\tif ({func_params}.{val} != NULL)\n\
	{{\n\
		memcpy(&pattern->expected.{val}, {func_params}.{val}, sizeof(pattern->expected.{val}));\n\
	}}\n".format(val=list_output_param_name[idx], func_params = g_str_PATTERN_FUNC_PARAM_NAME)
			else:
				raise Exception("[E_UTS_PCL] Un-supported {} for param {}".format(g_str_PATTERN_DATA_OUT_NOT_CHANGE, list_output_param_name[idx]))

	if str_format != "":
		str_format = "\n" + str_format

	# write all string to file
	fo.write(str_format)

## This function writes a group lines of code to output file for executing test function.
# @param func_name					Function name
# @param fo							Opened file object
# @param dict_pattern_info			Pattern info
# @param bool_get_expected_addr		Flag to check getting address pointer function was generated or not
# @return None
def testcode_Execution(work_sheet, func_name, fo, dict_pattern_info, bool_get_expected_addr, str_src_folder, method_flag):
	list_output_param_type   = dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_TYPE)
	list_output_param_name   = dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_NAME)
	list_output_item_type    = dict_pattern_info.get(g_str_PATTERN_OUTPUT_ITEM_TYPE)
	list_output_data_type    = dict_pattern_info.get(g_str_PATTERN_OUTPUT_DATA_TYPE)
	list_expected_param_name = dict_pattern_info.get(g_str_PATTERN_EXPECTED_PARAM_NAME)

	list_input_param_name = dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_NAME)
	list_input_item_type  = dict_pattern_info.get(g_str_PATTERN_INPUT_ITEM_TYPE)

	str_set_pointer_state = None
	str_set_pointer_value = None

	str_param_type = ""
	str_param_name = ""
	dict_expected_info = None
	str_data_type_info = ""
	
	# get return name and return type
	for i in range(len(list_output_item_type)):
		if list_output_item_type[i] == g_str_PATTERN_ITEM_RETURN_VALUE:
			str_param_type = list_output_param_type[i]
			str_param_name = list_output_param_name[i]
			dict_expected_info = list_expected_param_name[i]
			str_data_type_info = list_output_data_type[i]
			break

	# check the valid info of Return Value
	if dict_expected_info == None:
		raise Exception("[E_UTS_PCL] PCL {} must contain Return column for executing test".format(work_sheet))

	# get common info
	str_expected_param_name, list_member, list_member_type = testcode_process_data_dictionary_format(dict_expected_info)
	flag_output_type_format, _, _, _, _ = testcode_check_output_formats(str_param_name, str_data_type_info)

	#code test execution is return a pointer
	if testcode_check_pointer_name(str_param_name) or testcode_check_pointer_type(str_param_type):
		str_param_name = testcode_remove_array(testcode_remove_pointer(str_param_name))

		if bool_get_expected_addr:
			str_get_expected_addr = "\tb &= TEST_GetAddr_{work_sheet}(&pattern->expected._{val_expected}, {func_params});\n".format(work_sheet = work_sheet, \
			val_expected = str_param_name, func_params = g_str_func_params)
		else:
			str_get_expected_addr = ""

		str_format =  "\n\t{val_expected} = {func_name}(".format(val_expected = str_param_name, func_name = func_name)

		str_set_pointer_state = "\tif (pattern->expected._{val_expected} == {TEST_ADDR_NULL} || pattern->expected._{val_expected} == {TEST_ADDR_NOT_NULL})\n\
	{{\n\
		outputs._{val_expected} = ({val_expected} == NULL) ? {TEST_ADDR_NULL} : {TEST_ADDR_NOT_NULL};\n\
	}}\n\
	else\n\
	{{\n\
		outputs._{val_expected} = (uint32_t){val_expected};\n\
	}}\n{str_get_expected_addr}".format(val_expected=str_param_name, str_get_expected_addr = str_get_expected_addr, \
		TEST_ADDR_NULL = g_str_config_TEST_ADDR_NULL, TEST_ADDR_NOT_NULL = g_str_config_TEST_ADDR_NOT_NULL)

		if flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_VALUE or flag_output_type_format == FORMAT_OUTPUT_TYPE_POINTER_ARRAY:
			str_set_pointer_value = "\tif ({val_expected} != NULL)\n\
	{{\n\
		memcpy(&outputs.{val_expected}, {val_expected}, sizeof(outputs.{val_expected}));\n\
	}}\n".format(val_expected = str_param_name)

	# code test execution for void function
	elif str_param_type == g_str_ENV_VOID_TYPE:
		str_format = "\n\t{}(".format(func_name)
	
	# code test execution for non-void function
	else:
		if method_flag == "True":
			str_format =  "\n\toutputs.{} = TEST_{}->{}(".format(str_param_name, str_src_folder, func_name)
		else:
			str_format =  "\n\toutputs.{} = {}(".format(str_param_name, func_name)
	
	str_comma = ""
	for i in range(len(list_input_item_type)):
		if list_input_item_type[i] == g_str_PATTERN_ITEM_PARAMETER:
			param = g_str_PATTERN_FUNC_PARAM_NAME + "." + testcode_remove_pointer(testcode_remove_array(list_input_param_name[i]))
			str_format = str_format + str_comma + param
			str_comma = ", "

	str_format = str_format + ");\n"

	if str_set_pointer_state != None:
		str_format += str_set_pointer_state

	if str_set_pointer_value != None:
		str_format += str_set_pointer_value

	str_format += "\n"

	# write to ouptut file
	fo.write(str_format)

## This function writes a group lines of code to output file for getting actual value after executing test function.
# @param func_name					Function name
# @param fo							Opened file object
# @param dict_pattern_info			Pattern info
# @param bool_get_expected_addr		Flag to check getting address pointer function was generated or not
# @return None
def testcode_Postcondition(func_name, fo, dict_pattern_info, bool_get_expected_addr):
	# list of input info
	list_input_param_name		= dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_NAME)
	list_input_item_type		= dict_pattern_info.get(g_str_PATTERN_INPUT_ITEM_TYPE)
	list_input_param_type 		= dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_TYPE)
	list_input_data_type		= dict_pattern_info.get(g_str_PATTERN_INPUT_DATA_TYPE)

	# list of info for output validation
	list_output_param_type 		= dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_TYPE)
	list_output_param_name 		= dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_NAME)
	list_output_data_type 		= dict_pattern_info.get(g_str_PATTERN_OUTPUT_DATA_TYPE)
	list_output_item_type 		= dict_pattern_info.get(g_str_PATTERN_OUTPUT_ITEM_TYPE)
	list_expected_param_name	= dict_pattern_info.get(g_str_PATTERN_EXPECTED_PARAM_NAME)

	# remove all pointer and array character in input parameters
	raw_list_parameter_in = []
	raw_list_unsupported_validation = []
	for i in range(len(list_input_item_type)):
		if list_input_item_type[i] == g_str_PATTERN_ITEM_PARAMETER:
			# raw parameter is no pointer sign and array info
			str_raw_param_in = testcode_remove_pointer(testcode_remove_array(list_input_param_name[i]))
			raw_list_parameter_in.append(str_raw_param_in)
			# get input param format
			flag_type, _ = testcode_check_input_formats(list_input_data_type[i], list_input_param_type[i], list_input_param_name[i])
			if flag_type == FORMAT_INPUT_ARRAY_OF_POINTER or flag_type == FORMAT_INPUT_POINTER_TO_POINTER:
				raw_list_unsupported_validation.append(str_raw_param_in)

	str_format = ""

	for idx in range(len(list_expected_param_name)):
		# process Parameter output
		if list_output_item_type[idx] == g_str_PATTERN_ITEM_PARAMETER:
			str_format += testcode_post_expected_parameter_out(func_name, raw_list_parameter_in, raw_list_unsupported_validation, list_output_param_name[idx], \
																list_expected_param_name[idx], list_output_data_type[idx], bool_get_expected_addr)
		# process Related global parameter output
		elif list_output_item_type[idx] == g_str_PATTERN_ITEM_GLOBAL_PARAMETER:
			str_format += testcode_post_expected_global_out(func_name, list_output_param_name[idx], list_expected_param_name[idx],list_output_data_type[idx], bool_get_expected_addr)
		# process Local function output
		elif list_output_item_type[idx] == g_str_PATTERN_ITEM_LOCAL_FUNC:
			str_format += testcode_post_expected_local_func(func_name, list_output_param_name[idx], list_expected_param_name[idx], list_output_data_type[idx], bool_get_expected_addr)
		# process return value is handled in testcode_Execution
		elif list_output_item_type[idx] == g_str_PATTERN_ITEM_RETURN_VALUE:
			pass
		else:
			raise Exception("[ERROR] Invalid output item format {} in function {}".format(list_output_item_type[idx], func_name))

	fo.write(str_format)

## This function writes a group lines of code to output file for executing validating function.
# @param func_name					Function name
# @param fo							Opened file object
# @param dict_pattern_info			Pattern info
# @return None
def testcode_ValidateResult(func_name, fo, dict_pattern_info):
	str_format = ""

	if g_bool_output_element:
		str_validate = "pattern->validator"
	else:
		str_validate = "NULL"

	if g_bool_output_element or g_bool_output_return:
		if g_bool_enabled_validator:
			str_format	 += "\tb &= TEST_Validate_{func_name}(&outputs, &pattern->expected, {str_validate});\n\n".format(func_name = func_name, str_validate = str_validate)
		else:
			str_format	 += "\tb &= TEST_Validate_{func_name}(&outputs, &pattern->expected);\n\n".format(func_name = func_name)

	str_format	+= "\tTEST_ValidateResult(b, params);\n\n"
	str_format	+= "\tut_deinit_{}();\n\n".format(g_str_config_target_module)
	str_format	+= "\tTEST_DestroyParam(params);\n\n"
	str_format 	+= "\treturn b;\n}\n"

	# write to file
	fo.write(str_format)
	
## This function gets a group lines of code for validating outputs and expectes.
# @param func_name					Function name
# @param dict_pattern_info			Pattern info
# @return str_format				Group lines of code
def testcode_Validate_expected_value(func_name, dict_pattern_info):
	# list of info for output validation
	list_output_param_type 		= dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_TYPE)
	list_output_param_name 		= dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_NAME)
	list_expected_param_name	= dict_pattern_info.get(g_str_PATTERN_EXPECTED_PARAM_NAME)
	list_output_data_type 		= dict_pattern_info.get(g_str_PATTERN_OUTPUT_DATA_TYPE)
	list_output_item_type 		= dict_pattern_info.get(g_str_PATTERN_OUTPUT_ITEM_TYPE)

	str_format = ""
	
	for idx in range(len(list_expected_param_name)):
		# process Parameter output
		if list_output_item_type[idx] == g_str_PATTERN_ITEM_PARAMETER:
			str_format += testcode_validate_expected_parameter_out(func_name, list_output_param_type[idx],  list_output_param_name[idx], list_expected_param_name[idx], list_output_data_type[idx])
		# process Related global parameter output
		elif list_output_item_type[idx] == g_str_PATTERN_ITEM_GLOBAL_PARAMETER:
			str_format += testcode_validate_expected_global_out(func_name, list_output_param_type[idx],  list_output_param_name[idx], list_expected_param_name[idx], list_output_data_type[idx])
		# process Local function output
		elif list_output_item_type[idx] == g_str_PATTERN_ITEM_LOCAL_FUNC:
			str_format += testcode_validate_expected_local_func(func_name, list_output_param_type[idx],  list_output_param_name[idx], list_expected_param_name[idx], list_output_data_type[idx])
		# process return value
		elif list_output_item_type[idx] == g_str_PATTERN_ITEM_RETURN_VALUE:
			str_format += testcode_validate_return_value(func_name, list_output_param_type[idx],  list_output_param_name[idx], list_expected_param_name[idx], list_output_data_type[idx])
		else:
			raise Exception("[ERROR] Invalid output item format {} in function {}".format(list_output_item_type[idx], func_name))

	return str_format

## This function writes validation function to output file.
# @param func_name					Function name
# @param fo							Opened file object
# @param dict_pattern_info			Pattern info
# @return None
def testcode_Validate(func_name, fo, dict_pattern_info):
	declare_var = ""

	if g_bool_enabled_validator:
		declare_func = "static bool TEST_Validate_{func_name}(struct {func_name}_expect *outputs, struct {func_name}_expect *expected, {validator_type} validator[])\n".format(func_name=func_name,\
			validator_type = g_str_ENV_VALIDATOR_TYPE)
		if g_bool_output_element:
			declare_var = "\tuint64_t index = 0;\n\n"
	else:
		declare_func = "static bool TEST_Validate_{func_name}(struct {func_name}_expect *outputs, struct {func_name}_expect *expected)\n".format(func_name=func_name)

	str_validation	= testcode_Validate_expected_value(func_name, dict_pattern_info)

	# check if there are some things need to validate
	if str_validation != "":
		str_format 	 = declare_func
		str_format 	 += "{\n"
		str_format	 += "\tbool b = true;\n"
		str_format	 += declare_var
		str_format	 += str_validation
		end_func_validate = "\treturn b;\n}\n\n"
		str_format 		 += end_func_validate

		# write to source file
		fo.write(str_format)

## This function writes test code based on input pattern info.
# @param str_path				Path to output source code
# @param str_func_name			Function name
# @param dict_pattern_info		Pattern info
# @param obj_config				Configuration info
# @return None
def uts_tsc_write_test_code(str_path, str_worksheet ,str_func_name, dict_pattern_info, obj_config, src_language, str_src_folder, method_flag, namespace_value):
	print("#### [Test Code] Start: {}".format(str_worksheet))
	# Try to open test code file
	try:
			fo = open(str_path + "\\{}{}".format(str_worksheet,src_language), mode="w", encoding="utf-8")
	except:
		raise Exception("[E_UTS_FILE] Could not open test code file: {}".format(str_path + "\\{}.c".format(str_worksheet)))

	if DEBUG_LOG:
		print("Function name: \n", str_worksheet)
		print("input_item_type: \n", dict_pattern_info.get(g_str_PATTERN_INPUT_ITEM_TYPE))
		print("input_param_type:\n", dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_TYPE))
		print("input_param_name:\n", dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_NAME))
		print("input_data_type: \n", dict_pattern_info.get(g_str_PATTERN_INPUT_DATA_TYPE))

		print("output_item_type:\n", dict_pattern_info.get(g_str_PATTERN_OUTPUT_ITEM_TYPE))
		print("output_param_type:\n", dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_TYPE))
		print("output_param_name:\n", dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_NAME))
		print("expected_param_name: \n", dict_pattern_info.get(g_str_PATTERN_EXPECTED_PARAM_NAME))
		print("output_data_type: \n", dict_pattern_info.get(g_str_PATTERN_OUTPUT_DATA_TYPE))
		print("output_pointer_names: \n", dict_pattern_info.get(g_str_PATTERN_OUTPUT_POINTER_NAME))

	global g_bool_enabled_validator
	global g_bool_config_enabled_import_data
	global g_str_config_target_module
	global g_dict_config_table_validator_by_name
	global g_dict_config_table_validator_by_type
	global g_dict_config_init_data
	global g_list_config_pointer_type
	global g_str_config_NOT_NULL
	global g_str_config_NULL
	global g_str_config_TEST_ADDR_NOT_NULL
	global g_str_config_TEST_ADDR_NULL

	global g_str_checkbit_validation
	global g_str_checkbit_postcond
	global g_str_checkbit_open_brace
	global g_str_checkbit_close_brace
	global g_str_checkbit_indent

	global g_str_func_params
	global g_bool_input_element
	global g_bool_input_params
	global g_bool_output_return
	global g_bool_output_element

	# get data from configuration file
	g_bool_config_enabled_import_data		= testcode_convert_python_type(obj_config['OUTPUT_GENERATION_CONTROL']['EnableImportData'], bool)
	g_str_config_target_module 				= testcode_convert_python_type(obj_config['TARGET_OF_TESTING']['TargetModule'], str)
	g_dict_config_table_validator_by_name	= testcode_convert_python_type(obj_config['VALIDATOR']['TABLE_VALIDATOR_BY_NAME'], dict)
	g_dict_config_table_validator_by_type	= testcode_convert_python_type(obj_config['VALIDATOR']['TABLE_VALIDATOR_BY_TYPE'], dict)
	g_list_config_pointer_type 				= testcode_convert_python_type(obj_config['DATA_TYPE']['ListPointerType'], list)
	g_dict_config_init_data					= testcode_convert_python_type(obj_config['OUTPUT_GENERATION_CONTROL']['INIT_ADDR_DATA'], dict)
	g_str_config_NOT_NULL					= testcode_convert_python_type(obj_config['SPECIAL_STRING']['NOT_NULL_VALUE'], str)
	g_str_config_NULL						= testcode_convert_python_type(obj_config['SPECIAL_STRING']['NULL_VALUE'], str)
	g_str_config_TEST_ADDR_NOT_NULL			= testcode_convert_string_to_macro(g_str_config_NOT_NULL)
	g_str_config_TEST_ADDR_NULL				= testcode_convert_string_to_macro(g_str_config_NULL)

	# add check bit condition and code indentation
	if g_bool_enabled_validator:
		g_str_checkbit_validation = "\tif (TEST_CheckBitValidator(validator, index++))\n"
		g_str_checkbit_postcond = "\tif (TEST_CheckBitValidator(pattern->validator, index++))\n"
		g_str_checkbit_open_brace = "\t{\n"
		g_str_checkbit_close_brace = "\t}\n"
		g_str_checkbit_indent = "\t"
	else:
		g_str_checkbit_validation = ""
		g_str_checkbit_postcond = ""
		g_str_checkbit_open_brace = ""
		g_str_checkbit_close_brace = ""
		g_str_checkbit_indent = ""

	# get pattern info
	list_input_param_type = dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_TYPE)
	list_input_param_name = dict_pattern_info.get(g_str_PATTERN_INPUT_PARAM_NAME)
	list_input_data_type = dict_pattern_info.get(g_str_PATTERN_INPUT_DATA_TYPE)
	list_input_item_type = dict_pattern_info.get(g_str_PATTERN_INPUT_ITEM_TYPE)

	list_output_param_name = dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_NAME)
	list_output_param_type = dict_pattern_info.get(g_str_PATTERN_OUTPUT_PARAM_TYPE)
	list_expected_param_name = dict_pattern_info.get(g_str_PATTERN_EXPECTED_PARAM_NAME)
	list_output_data_type = dict_pattern_info.get(g_str_PATTERN_OUTPUT_DATA_TYPE)
	list_output_item_type = dict_pattern_info.get(g_str_PATTERN_OUTPUT_ITEM_TYPE)

	# check the consistent info lengths of output
	if not(len(list_input_param_type) == len(list_input_param_name) == len(list_input_data_type) == len(list_input_item_type)):
		raise Exception("[ERROR] List are not equal:\ninput_param_type = {}\ninput_param_name = {}\ninput_data_type = {}\ninput_item_type = {}".format(list_input_param_type, \
			list_input_param_name, list_input_data_type, list_input_item_type))

	# check the consistent info lengths of output
	if not(len(list_output_param_type) == len(list_output_param_name) == len(list_expected_param_name) == len(list_output_data_type) == len(list_output_item_type)):
		raise Exception("[ERROR] List are not equal:\noutput_param_type = {}\noutput_param_name = {}\nexpected_param_name = {}\noutput_data_type = {}\noutput_item_type = {}".format(list_output_param_type, \
			list_output_param_name, list_expected_param_name, list_output_data_type, list_output_item_type))

	# initialized global config for general function attribute
	g_str_func_params = "NULL"
	g_bool_input_params = False
	g_bool_input_element = False
	g_bool_output_return = False
	g_bool_output_element = False

	# get basic info of test pattern
	if g_str_PATTERN_ITEM_PARAMETER in list_input_item_type:
		g_str_func_params = "&" + g_str_PATTERN_FUNC_PARAM_NAME
		g_bool_input_params = True

	if (g_str_PATTERN_ITEM_GLOBAL_PARAMETER in list_input_item_type) or (g_str_PATTERN_ITEM_LOCAL_FUNC in list_input_item_type):
		g_bool_input_element = True

	for i in range(len(list_output_item_type)):
		if list_output_item_type[i] == g_str_PATTERN_ITEM_RETURN_VALUE:
			# get member list of return value
			_, list_member, _ = testcode_process_data_dictionary_format(list_expected_param_name[i])

			# check the output element
			if len(list_member) != 0 or list_output_data_type[i] == g_str_PATTERN_DATA_OUT_POINTER_VALUE or list_output_data_type[i] == g_str_PATTERN_DATA_OUT_POINTER_ARRAY:
				g_bool_output_element = True

			# code test execution is return a pointer
			if testcode_check_pointer_name(list_output_param_name[i]) or testcode_check_pointer_type(list_output_param_type[i]) or \
				list_output_param_type[i] != g_str_ENV_VOID_TYPE:
				g_bool_output_return = True
		else:
			g_bool_output_element = True

	testcode_Header(str_worksheet, fo, namespace_value)

	bool_get_expected_addr = testcode_get_expected_addr(str_worksheet, fo, dict_pattern_info)

	if g_bool_config_enabled_import_data:
		bool_additional_init_data = testcode_Initialize_data(str_worksheet, fo, dict_pattern_info)
	else:
		bool_additional_init_data = False

	testcode_Validate(str_worksheet, fo, dict_pattern_info)

	testcode_Maintest(str_worksheet, fo, dict_pattern_info)
	
	testcode_Precondition(str_worksheet, fo, dict_pattern_info, bool_additional_init_data)

	testcode_Execution(str_worksheet, str_func_name, fo, dict_pattern_info, bool_get_expected_addr, str_src_folder, method_flag)

	testcode_Postcondition(str_worksheet, fo, dict_pattern_info, bool_get_expected_addr)

	testcode_ValidateResult(str_worksheet, fo, dict_pattern_info)

	fo.close()
	print("#### [Test Code] End: {}".format(str_worksheet))
