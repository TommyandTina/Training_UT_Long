from os.path import normpath, join, exists, split
from os import listdir
from argparse import ArgumentParser
import re

FUNC_PROTOTYPE = 'function_prototype.txt'

class Function:
    def __init__(self, ReturnType, Name, Parameters, ParameterOrigin):
        self.ReturnType      = ReturnType
        self.Name            = Name
        self.Parameters      = Parameters
        self.ParameterOrigin = ParameterOrigin

class Parameter:
    def __init__(self, Type, Name):
        self.Type = Type
        self.Name = Name

class FileHandle:
    def __init__(self, ut_stub_c, ut_hook_c, ut_stub_h):
        self.ut_stub_c = ut_stub_c
        self.ut_hook_c = ut_hook_c
        self.ut_stub_h = ut_stub_h

def stub_ReadingFuncPrototype():
    if exists(FUNC_PROTOTYPE):
        with open(FUNC_PROTOTYPE, 'r') as File:
            FuncPrototypes = [Line.replace('\n', '') for Line in File]
            # return FuncPrototypes

        ReturnValue = []
        for Func in FuncPrototypes:
            Temp = re.search(r'^(.*?)\s+(\w+)(?=\()', Func)
            # Get return type of function and name of function
            ReturnType = Temp.group(1)
            Name = Temp.group(2)
            # Get all parameter of function
            Temp = re.search(r'(?<=\().*?(?=\))', Func)
            ParamOrigin = Temp.group(0)

            ListParam = []
            if ParamOrigin == 'void':
                ListParam.append(Parameter('void', 'void'))
                ReturnValue.append(
                    Function(ReturnType, Name, ListParam, ParamOrigin)
                )
            else:
                Params = ParamOrigin.split(',')
                for Param in Params:
                    TempParamType = re.search(r'^(const\s+)?\w+(\*)?', Param).group(0)
                    TempParamName = re.search(r'(const\s+)?\w+(\[.*?\])?$', Param).group(0)
                    TempParam = Parameter(TempParamType, TempParamName)
                    ListParam.append(TempParam)
                ReturnValue.append(
                    Function(ReturnType, Name, ListParam, ParamOrigin)
                )
            # print(Func)
            # print(' '*4, f'{ReturnType:20}', f'{Name:40}', f'{ParamOrigin}')
            # for Param in ListParam:
            #     print(' '*4, f'{Param.Type:20}', f'{Param.Name:40}')
        return tuple(ReturnValue)
    else:
        print(f'Error: File not found. {FUNC_PROTOTYPE}')
        print('Check, is stub_FindPrototype.py executed?')
        exit()

def stub_StubStruct(FuncData):
    FuncName = FuncData.Name
    ReturnType = FuncData.ReturnType
    ReturnType = re.sub(r'const\s', '', ReturnType)

    StubStruct = 'typedef struct {\n'

    ut_max_size_macro = f'UT_{FuncName}_MAX_SIZE'

    for Parameter in FuncData.Parameters:
        ParamNameOnly = Parameter.Name
        ParamNameOnly = ParamNameOnly.replace('const ', '')
        ParamNameOnly = re.sub(r'\[.*?\]', '', ParamNameOnly)
        ParamTypeOnly = Parameter.Type
        ParamTypeOnly = ParamTypeOnly.replace('const ', '')
        ParamTypeOnly = ParamTypeOnly.replace('*', '')
        # print(f'{Parameter.Type:50}', f'{ParamTypeOnly:50}')
        # print(f'{Parameter.Name:50}', f'{ParamNameOnly:50}')
        if Parameter.Name != 'void':
            if 'void' == ParamTypeOnly:
                ParamTypeOnly = 'uint32_t'
            ParamTypeRemovedConst = Parameter.Type.replace('const ', '')

            if '[]' in Parameter.Name:
                Temp = Parameter.Name.replace('[]', f'[{ut_max_size_macro}]')
                StubStruct += f'    {ParamTypeRemovedConst} {Temp};\n'
            else:
                StubStruct += f'    {ParamTypeRemovedConst} {ParamNameOnly}[{ut_max_size_macro}];\n'

            if '*' in Parameter.Type:
                ParamTypeRemovedConst = re.sub(r'\s*\*\s*', '', ParamTypeRemovedConst)
                if 'void' in Parameter.Type:
                    ParamTypeRemovedConst ='uint32_t'
                    StubStruct += f'    {ParamTypeRemovedConst} {ParamNameOnly}_in[{ut_max_size_macro}];\n'
                    StubStruct += f'    {ParamTypeRemovedConst} {ParamNameOnly}_out[{ut_max_size_macro}];\n'
                else:
                    StubStruct += f'    {ParamTypeRemovedConst} {ParamNameOnly}_in[{ut_max_size_macro}];\n'
                    StubStruct += f'    {ParamTypeRemovedConst} {ParamNameOnly}_out[{ut_max_size_macro}];\n'

    if (ReturnType == 'void'):
        StubStruct += ''
    else:
        StubStruct += f'    {ReturnType} ReturnValue[{ut_max_size_macro}];\n'

    StubStruct +=  '    uint32_t ut_index;\n'
    StubStruct += f'}} s_{FuncName};\n'

    GlobalDefine = f's_{FuncName} g_{FuncName};\n'

    GlobalDeclare = f'extern {GlobalDefine}'

    return StubStruct, GlobalDeclare, GlobalDefine

def stub_HookPointer(FuncData):
    FuncName = FuncData.Name
    ReturnType = FuncData.ReturnType
    ReturnType = re.sub(r'const\s', '', ReturnType)
    HookPointer        = f'typedef {ReturnType} (*f_hook_{FuncName})({FuncData.ParameterOrigin});\n'
    HookPointerDefine  = f'f_hook_{FuncName} g_hook_{FuncName};\n'
    HookPointerDeclare = f'extern {HookPointerDefine}'

    return HookPointer, HookPointerDeclare, HookPointerDefine

def stub_HookFunc(FuncData):
    FuncName = FuncData.Name
    ReturnType = FuncData.ReturnType
    ReturnType = re.sub(r'const\s', '', ReturnType)

    HookFuncDeclare = f'extern {ReturnType} ut_hook_{FuncName}({FuncData.ParameterOrigin});\n'

    HookFuncDefine = ''
    HookFuncDefine += f'{FuncData.ReturnType} ut_hook_{FuncName}({FuncData.ParameterOrigin})\n'
    HookFuncDefine +=  '{\n'
    HookFuncDefine += f'    s_{FuncName} *IO = &g_{FuncName};\n'

    if 'void' not in FuncData.ReturnType:
        HookFuncDefine += f'    {FuncData.ReturnType} ReturnValue;\n'

    InputParam = ''
    for Parameter in FuncData.Parameters:
        ParamNameOnly = Parameter.Name
        ParamNameOnly = ParamNameOnly.replace('const ', '')
        ParamNameOnly = re.sub(r'\[.*?\]', '', ParamNameOnly)
        ParamTypeOnly = Parameter.Type
        ParamTypeOnly = ParamTypeOnly.replace('const ', '')
        ParamTypeOnly = ParamTypeOnly.replace('*', '')
        if 'void' != ParamNameOnly:
            InputParam += f'{ParamNameOnly}, '

        if 'void' in Parameter.Type and '*' in Parameter.Type:
            HookFuncDefine += f'    IO->{ParamNameOnly}[IO->ut_index] = {ParamNameOnly};\n'
            HookFuncDefine += f'    if ({ParamNameOnly} != NULL)\n'
            HookFuncDefine +=  '    {\n'
            HookFuncDefine += f'        memcpy(IO->{ParamNameOnly}_in[IO->ut_index], {ParamNameOnly}, UT_MAX_SIZE*4);\n'
            HookFuncDefine +=  '    }\n'

        elif 'void' == Parameter.Type:
            continue

        elif '*' in Parameter.Type:
            HookFuncDefine += f'    IO->{ParamNameOnly}[IO->ut_index] = {ParamNameOnly};\n'
            HookFuncDefine += f'    if ({ParamNameOnly} != NULL)\n'
            HookFuncDefine +=  '    {\n'
            HookFuncDefine += f'        memcpy(&IO->{ParamNameOnly}_in[IO->ut_index], {ParamNameOnly}, sizeof(IO->{ParamNameOnly}_out[IO->ut_index]));\n'

            HookFuncDefine +=  '    }\n'
        elif '[' in Parameter.Name:
            HookFuncDefine += f'    memcpy(IO->{ParamNameOnly}[IO->ut_index], {ParamNameOnly}, sizeof({Parameter.Type}));\n'
        else:
            HookFuncDefine += f'    IO->{ParamNameOnly} = {ParamNameOnly};\n'
    InputParam = InputParam[:-2]

    CheckHook = f'(g_hook_{FuncName} == NULL) ?\n'
    RealFunc = f'{FuncName}({InputParam}) :\n'
    StubFunc = f'g_hook_{FuncName}({InputParam});\n'


    if 'void' not in FuncData.ReturnType:
        if 'const' in FuncData.ReturnType:
            CheckHook = f'({FuncData.ReturnType}){CheckHook}'
        CheckHook = f'ReturnValue = {CheckHook}'

    HookFuncDefine += f'    {CheckHook}'
    HookFuncDefine += f'        {RealFunc}'
    HookFuncDefine += f'        {StubFunc}'

    if 'void' not in FuncData.ReturnType:
        HookFuncDefine += f'    return ReturnValue;\n'
    HookFuncDefine +=  '}\n'

    return HookFuncDeclare, HookFuncDefine

def stub_StubFunc(FuncData):
    FuncName = FuncData.Name
    ReturnType = FuncData.ReturnType
    ReturnType = re.sub(r'const\s', '', ReturnType)

    StubFuncDeclare = f'extern {FuncData.ReturnType} ut_stub_{FuncName}({FuncData.ParameterOrigin});\n'
    StubFuncDefine = ''
    StubFuncDefine += f'{FuncData.ReturnType} ut_stub_{FuncName}({FuncData.ParameterOrigin})\n{{\n'
    StubFuncDefine += f'    s_{FuncName} *IO = &g_{FuncName};\n'
    if 'void' not in FuncData.ReturnType:
        StubFuncDefine += f'    {FuncData.ReturnType} ReturnValue;\n'
        if 'const' in FuncData.ReturnType:
            StubFuncDefine += f'    ReturnValue = ({FuncData.ReturnType})IO->ReturnValue[IO->ut_index];\n'
        else:
            StubFuncDefine += f'    ReturnValue = IO->ReturnValue[IO->ut_index];\n'

    for Parameter in FuncData.Parameters:
        ParamNameOnly = Parameter.Name
        ParamNameOnly = ParamNameOnly.replace('const ', '')
        ParamNameOnly = re.sub(r'\[.*?\]', '', ParamNameOnly)
        ParamTypeOnly = Parameter.Type
        ParamTypeOnly = ParamTypeOnly.replace('const ', '')
        ParamTypeOnly = ParamTypeOnly.replace('*', '')

        if 'const' not in Parameter.Type and '*' in Parameter.Type:
            StubFuncDefine += f'    if ({ParamNameOnly} != NULL)\n'
            StubFuncDefine +=  '    {\n'
            StubFuncDefine += f'        memcpy({ParamNameOnly}, &IO->{ParamNameOnly}_out[IO->ut_index], sizeof(IO->{ParamNameOnly}_out[IO->ut_index]));\n'
            StubFuncDefine +=  '    }\n'

    StubFuncDefine += f'    IO->ut_index++;\n'
    if 'void' not in FuncData.ReturnType:
        StubFuncDefine += f'    return ReturnValue;\n'
    StubFuncDefine +=  '}\n'

    return StubFuncDeclare, StubFuncDefine

def Output(File, FuncPrototypes):
    def ExtractFuncData(Funcs):
        for Func in Funcs:
            TempStubStruct, TempStubStructDeclare, TempStubStructDefine = stub_StubStruct(Func)
            TempHookPointer, TempHookPointerDeclare, TempHookPointerDefine = stub_HookPointer(Func)
            TempStubFuncDeclare, TempStubFuncDefine = stub_StubFunc(Func)
            TempHookFuncDeclare, TempHookFuncDefine = stub_HookFunc(Func)

            StubStruct.append(TempStubStruct)
            StubStructDeclare.append(TempStubStructDeclare)
            StubStructDefine.append(TempStubStructDefine)
            HookPointer.append(TempHookPointer)
            HookPointerDeclare.append(TempHookPointerDeclare)
            HookPointerDefine.append(TempHookPointerDefine)
            StubFuncDeclare.append(TempStubFuncDeclare)
            StubFuncDefine.append(TempStubFuncDefine)
            HookFuncDeclare.append(TempHookFuncDeclare)
            HookFuncDefine.append(TempHookFuncDefine)

    StubStruct         = []
    StubStructDeclare  = []
    StubStructDefine   = []
    HookPointer        = []
    HookPointerDeclare = []
    HookPointerDefine  = []
    StubFuncDeclare    = []
    StubFuncDefine     = []
    HookFuncDeclare    = []
    HookFuncDefine     = []

    ExtractFuncData(FuncPrototypes)

    for result in StubStruct:
        File.ut_stub_h.write(result + '\n')
    for result in StubStructDeclare:
        File.ut_stub_h.write(result)
    File.ut_stub_h.write('\n')
    for result in StubStructDefine:
        File.ut_hook_c.write(result)
    File.ut_hook_c.write('\n')
    for result in HookPointer:
        File.ut_stub_h.write(result)
    File.ut_stub_h.write('\n')
    for result in HookPointerDeclare:
        File.ut_stub_h.write(result)
    File.ut_stub_h.write('\n')
    for result in HookPointerDefine:
        File.ut_hook_c.write(result)
    File.ut_hook_c.write('\n')
    for result in StubFuncDeclare:
        File.ut_stub_h.write(result)
    File.ut_stub_h.write('\n')
    for result in StubFuncDefine:
        File.ut_stub_c.write(result + '\n')
    for result in HookFuncDeclare:
        File.ut_stub_h.write(result)
    File.ut_stub_h.write('\n')
    for result in HookFuncDefine:
        File.ut_hook_c.write(result + '\n')

def stub_InitEnv(FuncPrototypes, ModuleName):
    Data = ''
    Data += f'void ut_init_{ModuleName}(bool enable_real)\n'
    Data += '{\n'
    
    for LocalFunc in FuncPrototypes:
        Data += f'    memset(&g_{LocalFunc.Name}, 0, sizeof(s_{LocalFunc.Name}));\n'
    
    for LocalFunc in FuncPrototypes:
        Data += f'    g_hook_{LocalFunc.Name} = NULL;\n'

    Data += '}\n\n'
    return Data

def stub_DeinitEnv(FuncPrototypes, ModuleName):
    Data = ''
    Data += f'void ut_deinit_{ModuleName}(bool enable_real)\n'
    Data += '{\n'
    Data += '}\n\n'
    return Data

# Main
if __name__ == "__main__":
    # Input argument parse
    parser = ArgumentParser()
    parser.add_argument('-o', type=str, help='Output directory', required=True)
    parser.add_argument('-module', type=str, help='Module name', required=False, default='')
    args = parser.parse_args()
    OutputDir = normpath(args.o)
    ModuleName = args.module.lower()
    ModuleName_ = f'{ModuleName}_'
    ModuleNameUpper = ModuleName.upper()
    ModuleNameUpper_ = f'{ModuleNameUpper}_'

    # Reading function prototypes
    FuncPrototypes = stub_ReadingFuncPrototype()

    # Create file handle
    OutputPath = join(OutputDir, f'ut_{ModuleName_}stub.c')
    ut_stub_c = open(OutputPath, 'w')
    OutputPath = join(OutputDir, 'include')
    OutputPath = join(OutputPath, f'ut_{ModuleName_}stub.h')
    ut_stub_h = open(OutputPath, 'w')
    OutputPath = join(OutputDir, f'ut_{ModuleName_}hook.c')
    ut_hook_c = open(OutputPath, 'w')

    FileObj = FileHandle(ut_stub_c, ut_hook_c, ut_stub_h)
    FileObj.ut_stub_h.write(f'#ifndef _UT_{ModuleNameUpper_}STUB_H\n')
    FileObj.ut_stub_h.write(f'#define _UT_{ModuleNameUpper_}STUB_H\n\n')
    FileObj.ut_stub_h.write( '#include "base.h"\n\n')
    FileObj.ut_stub_h.write(f'#define UT_MAX_SIZE (10)\n')
    for Func in FuncPrototypes:
        FileObj.ut_stub_h.write(f'#define UT_{Func.Name}_MAX_SIZE (100)\n')
    FileObj.ut_stub_h.write(f'\n')

    FileObj.ut_stub_c.write(f'#include "ut_{ModuleName_}stub.h"\n\n')
    FileObj.ut_hook_c.write(f'#include "ut_{ModuleName_}stub.h"\n\n')

    FileObj.ut_hook_c.write('#ifdef WIN32\n')
    for Func in FuncPrototypes:
        FileObj.ut_hook_c.write(f'#define {Func.Name}(...) (0)\n')
    FileObj.ut_hook_c.write('#endif\n')
    Output(FileObj, FuncPrototypes)

    InitEnv = stub_InitEnv(FuncPrototypes, ModuleName)
    DeinitEnv = stub_DeinitEnv(FuncPrototypes, ModuleName)
    FileObj.ut_hook_c.write(InitEnv)
    FileObj.ut_hook_c.write(DeinitEnv)
    FileObj.ut_stub_h.write(f'extern void ut_init_stub();\n')
    FileObj.ut_stub_h.write(f'#endif /* _UT_STUB_{ModuleNameUpper_}_H */\n')
