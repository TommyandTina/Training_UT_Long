from os.path import normpath, join, exists, split
from argparse import ArgumentParser
import re

SAVE_LOCAL_FUNCTION = 'local_function.txt'
SAVE_STATIC_FUNCTION = 'static_function.txt'
SKIP = 'skip_function.txt'

STATIC_FUNC_PATTERN = r'^\w+((\s+\*?\s*)|(\s*\*?\s+))\w+\s+\w+\s*\('
LOCAL_FUNC_PATTERN = r'(=\s*|(\(\s*void\s*\)\s*)|^\s+|^\t+)\w+\s*\('
FUNC_PATTERN = r'^(const\s+)?\w+((\s+\*?\s*)|(\s*\*?\s+))\w+\s*\('
# Variable is assigned at stub_FindFunction()
FUNCS = []

def stub_FindFunction(SourcePath):
    global FUNCS
    with open(SourcePath, 'r') as SourceFile:
        SourceData = SourceFile.read()

    LineIndexs = []
    for m in re.finditer(r'.*\n', SourceData):
        LineIndexs.append(m.start())

    for m in re.finditer(r'^\S+\s+\S+\s*\(', SourceData, re.MULTILINE):
        Func = m.group(0)
        if '#' in Func:
            continue
        Func = re.sub(r'\s+', ' ', Func)
        Func = re.sub(r'\s*\(\s*', '', Func)
        FuncName = Func.split(' ')[1]
        for Line, LineIndex in enumerate(LineIndexs, start=1):
            if LineIndex == m.start():
                FUNCS.append(
                    {
                        'name': FuncName,
                        'line': [Line, ]
                    }
                )
    #Debug
    # print(f'Source: {SourcePath}')
    # print('Funcs found:')
    # for Func in FUNCS:
    #     print(' '*4, f'{Func["name"]:50}', f'{Func["line"]}')

def IsValidStaticFunc(Data):
    NotValids = (
        r'^(const)(\s+)',
    )

    for NotValid in NotValids:
        if re.search(NotValid, Data):
            return False
    return True

def IsValidLocalFunc(Data):
    NotValids = (
        r'if(\s*)\(',
        r'else(\s*)if(\s*)\(',
        r'while(\s*)\(',
        r'for(\s*)\(',
        r'switch(\s*)\(',
        r'#define',
        r'sizeof(\s*)\(',
        r'(c|C)allback(\s*)\(',
        r'return(\s*)\(',
    )

    for NotValid in NotValids:
        if re.search(NotValid, Data):
            return False
    return True

def IsSkip(Data):
    if exists(SKIP):
        with open(SKIP, 'r') as File:
            for SkipData in File:
                if SkipData.replace('\n', '') in Data:
                    return True
    return False

# Find static macro
def stub_FindStaticMacro(SourcePath: str):
    """
    Output is a macro which is defined for static
    Output type: str
    Output example:
        'IMR_STATIC'
    """
    Compile = re.compile(STATIC_FUNC_PATTERN)
    StaticMacro = ''

    with open(SourcePath, 'r') as SourceFile:
        for SourceLine in SourceFile:
            SourceLine = SourceLine.replace('\n', '')
            SourceLine = re.sub(r'\s+', ' ', SourceLine)
            SearchResultObj = Compile.search(SourceLine)
            if SearchResultObj:
                SearchResult = SearchResultObj.group(0)
                SearchResult = re.search(r'^\S+', SearchResult).group(0)
                StaticMacro = SearchResult
                break
    return StaticMacro

# Find static function
def stub_FindStaticFunction(SourcePath: str):
    """
    Output is an array of dictionary with 2 keys: name and line
    Output type: list
    Output example:
    [
        {
            'name': 'FunctionName1',
            'line': [9,21]
        },
        {
            'name': 'FunctionName2',
            'line': [10,31]
        }
    ]
    """
    Compile = re.compile(STATIC_FUNC_PATTERN)
    StaticFuncs = []

    with open(SourcePath, 'r') as SourceFile:
        for Line, Data in enumerate(SourceFile, start=1):
            Data = Data.replace('\n', '')
            Data = re.sub(r'\s+', ' ', Data)
            SearchResultObj = Compile.search(Data)
            if SearchResultObj:
                SearchResult = SearchResultObj.group(0)
                # Check and skip function in skip list
                if IsSkip(SearchResult):
                    continue
                # Check is valid static function
                if not IsValidStaticFunc(SearchResult):
                    continue
                SearchResult = re.sub(r'\s*\(', '', SearchResult)
                SearchResult = re.sub(r'^(\S+)(\s+)(\S+)(\s+)', '', SearchResult)

                FuncDuplicate = -1
                for Index, StaticFunc in enumerate(StaticFuncs):
                    if StaticFunc['name'] == SearchResult:
                        FuncDuplicate = Index
                        break

                if FuncDuplicate == -1:
                    StaticFuncs.append(
                        {
                            'name': SearchResult,
                            'line': [Line, ]
                        }
                    )
                else:
                    StaticFuncs[FuncDuplicate]['line'].append(Line)
    # Debug
    # print(f'Source: {SourcePath}')
    # print('StaticFuncs found:')
    # for StaticFunc in StaticFuncs:
    #     print(' '*4, f'{StaticFunc["name"]:50}', f'{StaticFunc["line"]}')
    return StaticFuncs

# Find local function
def stub_FindLocalFunction(SourcePath: str):
    """
    Output is an array of dictionary with 2 keys: name and line
    Output type: list
    Output example:
    [
        {
            'name': 'FunctionName3',
            'line': [10, 44, 66]
        },
        {
            'name': 'FunctionName4',
            'line': [99, 101]
        }
    ]
    """
    Compile = re.compile(LOCAL_FUNC_PATTERN)
    LocalFuncs = []

    with open(SourcePath, 'r') as SourceFile:
        for Line, Data in enumerate(SourceFile, start=1):
            Data = Data.replace('\n', '')
            Data = re.sub(r'\s+', ' ', Data)
            SearchResultObj = Compile.search(Data)
            if SearchResultObj:
                SearchResult = SearchResultObj.group(0)
                # Check and skip function in skip list
                if IsSkip(SearchResult):
                    continue
                # Check is valid local function
                if not IsValidLocalFunc(SearchResult):
                    continue

                SearchResult = re.sub(r'^\s*', '', SearchResult)
                SearchResult = re.sub(r'\(\s*void\s*\)\s?', '', SearchResult)
                SearchResult = re.sub(r'=\s*', '', SearchResult)
                SearchResult = re.sub(r'\s*\((.*)', '', SearchResult)
                SearchResult = re.sub(r'\t*', '', SearchResult)

                if SearchResult == '':
                    continue

                if SearchResult.count('(') > 0:
                    continue

                if SearchResult.count(')') > 0:
                    continue

                FuncDuplicate = -1
                for Index, StaticFunc in enumerate(LocalFuncs):
                    if StaticFunc['name'] == SearchResult:
                        FuncDuplicate = Index
                        break

                if FuncDuplicate == -1:
                    LocalFuncs.append(
                        {
                            'name': SearchResult,
                            'line': [Line, ]
                        }
                    )
                else:
                    LocalFuncs[FuncDuplicate]['line'].append(Line)
    # Debug
    # print(f'Source: {SourcePath}')
    # print('LocalFuncs found:')
    # for LocalFunc in LocalFuncs:
    #     print(' '*4, f'{LocalFunc["name"]:50}', f'{LocalFunc["line"]}')
    return LocalFuncs

def stub_DefineStaticMacro(StaticMacro):
    if StaticMacro != '':
        return f'#define {StaticMacro}\n\n'
    return ''

def stub_Define(StaticFuncs, LocalFuncs, DefineStaticToStub):
    Define = ''
    # Get maximum len of local function
    LenMax = 0
    for LocalFunc in LocalFuncs:
        Name = LocalFunc['name']
        Len = len(Name)
        LenMax = Len if LenMax < Len else LenMax

    for LocalFunc in LocalFuncs:
        Name = LocalFunc['name']
        LocalLines = LocalFunc['line']
        IsStatic = False
        StaticLines = []
        # Check, is this static function
        for StaticFunc in StaticFuncs:
            if Name == StaticFunc['name']:
                IsStatic = True
                StaticLines = StaticFunc['line']
        for Func in FUNCS:
            if Name in Func['name']:
                IsStatic = True
                StaticLines = Func['line']
        if IsStatic and DefineStaticToStub:
            StaticLenMax = len(Name) + 9
            Left = f'{Name}'
            Define += f'#define {Left:{StaticLenMax}} {Name}_(__LINE__)\n'
            Left = f'{Name}_(line)'
            Define += f'#define {Left:{StaticLenMax}} {Name}__(line)\n'
            Left = f'{Name}__(line)'
            Define += f'#define {Left:{StaticLenMax}} {Name}_ut_##line\n'
            for StaticLine in StaticLines:
                Left = f'{Name}_ut_{StaticLine}'
                Define += f'#define {Left:{StaticLenMax}} {Name}\n'
            for LocalLine in LocalLines:
                Left = f'{Name}_ut_{LocalLine}'
                Define += f'#define {Left:{StaticLenMax}} ut_hook_{Name}\n'
        else:
            Define += f'#define {Name:{LenMax}} ut_hook_{Name}\n'
    Define += '\n'
    return Define

def stub_IncludeSource(SourceName):
    return f'#include "{SourceName}"\n\n'

def stub_Undef(StaticFuncs, LocalFuncs, DefineStaticToStub):
    Undef = ''
    for LocalFunc in LocalFuncs:
        Name = LocalFunc['name']
        # Check, is this static function
        IsStatic = False
        for StaticFunc in StaticFuncs:
            if Name == StaticFunc['name']:
                IsStatic = True
        if IsStatic and DefineStaticToStub:
            Undef += f'#undef {Name}\n'
        else:
            Undef += f'#undef {Name}\n'
    return Undef

# Main
if __name__ == '__main__':
    # Input argument parse
    parser = ArgumentParser()
    parser.add_argument('-i', type=str, help='Source path', required=True)
    parser.add_argument('-o', type=str, help='Output directory', required=False, default='./')
    parser.add_argument('-static', type=str, help='Define static function to stub', required=False, default='False')
    parser.add_argument('-module', type=str, help='Module name', required=False, default='')
    args = parser.parse_args()
    SourcePath = normpath(args.i)
    OutputDir = normpath(args.o)
    ModuleName = args.module.lower() if args.module == '' else (args.module + '_').lower()
    DefineStaticToStub = True if args.static.lower() == 'true' else False
    SourceName = split(SourcePath)[1]

    # Read data from Source code
    stub_FindFunction(SourcePath)
    StaticFuncs = stub_FindStaticFunction(SourcePath)
    LocalFuncs = stub_FindLocalFunction(SourcePath)
    StaticMacro = stub_FindStaticMacro(SourcePath)

    # Output data: ut_SourceName.c
    OutputPath = join(OutputDir, f'ut_{SourceName}')
    with open(OutputPath, 'w') as OutputFile:
        OutputFile.write(f'#include "ut_{ModuleName}stub.h"\n\n')
        OutputFile.write(stub_DefineStaticMacro(StaticMacro))
        OutputFile.write(stub_Define(StaticFuncs, LocalFuncs, DefineStaticToStub))
        OutputFile.write(stub_IncludeSource(SourceName))
        OutputFile.write(stub_Undef(StaticFuncs, LocalFuncs, DefineStaticToStub))

    # Output local function in used to stub
    if exists(SAVE_LOCAL_FUNCTION):
        with open(SAVE_LOCAL_FUNCTION, 'r+') as File:
            # Check if local function is exist in file
            FileLocalFuncs = [Line[:-1] for Line in File]
            for LocalFunc in LocalFuncs:
                LocalFuncName = LocalFunc['name']
                # Check if function name is static or not
                IsStatic = False
                for StaticFunc in StaticFuncs:
                    StaticFuncName = StaticFunc['name']
                    if StaticFuncName == LocalFuncName:
                        IsStatic = True
                if LocalFuncName not in FileLocalFuncs and not IsStatic:
                    File.write(f'{LocalFuncName}\n')
    else:
        with open(SAVE_LOCAL_FUNCTION, 'w') as File:
            for LocalFunc in LocalFuncs:
                LocalFuncName = LocalFunc['name']
                # Check if function name is static or not
                IsStatic = False
                for StaticFunc in StaticFuncs:
                    StaticFuncName = StaticFunc['name']
                    if StaticFuncName == LocalFuncName:
                        IsStatic = True
                # If not static function then write
                if not IsStatic:
                    File.write(f'{LocalFuncName}\n')

    # Output static function in used to stub
    if exists(SAVE_STATIC_FUNCTION):
        with open(SAVE_STATIC_FUNCTION, 'r+') as File:
            # Check if local function is exist in file
            FileStaticFuncs = [Line[:-1] for Line in File]
            for StaticFunc in StaticFuncs:
                StaticFuncName = StaticFunc['name']
                if StaticFuncName not in FileStaticFuncs:
                    File.write(f'{StaticFuncName}\n')
    else:
        with open(SAVE_STATIC_FUNCTION, 'w') as File:
            for StaticFunc in StaticFuncs:
                StaticFuncName = StaticFunc['name']
                File.write(f'{StaticFuncName}\n')
