from os.path import normpath, join, exists, split, isdir, isfile
from os import listdir
from argparse import ArgumentParser
import re

FUNC_PROTOTYPE = 'function_prototype.txt'
SAVE_LOCAL_FUNCTION = 'local_function.txt'
SAVE_STATIC_FUNCTION = 'static_function.txt'

def stub_SearchFile(ModuleDir, FileExtension):
    ListDirs = []
    DirNames = listdir(ModuleDir)
    for DirName in DirNames:
        Dir = join(ModuleDir, DirName)
        if isdir(Dir):
            ListDirs += stub_SearchFile(Dir, FileExtension)
        elif isfile(Dir) and FileExtension in DirName:
            ListDirs.append(Dir)
    return ListDirs

def stub_ReadingLocalFunc():
    if exists(SAVE_LOCAL_FUNCTION):
        with open(SAVE_LOCAL_FUNCTION, 'r') as File:
            LocalFuncs = [Line.replace('\n', '') for Line in File]
            return LocalFuncs
    else:
        print(f'Error: File not found. {SAVE_LOCAL_FUNCTION}')
        print('Check, is stub_LocalFuncToStub.py executed?')
        exit()

def stub_ReadingStaticFunc():
    if exists(SAVE_STATIC_FUNCTION):
        with open(SAVE_STATIC_FUNCTION, 'r') as File:
            StaticFuncs = [Line.replace('\n', '') for Line in File]
            return StaticFuncs
    else:
        print(f'Error: File not found. {SAVE_STATIC_FUNCTION}')
        print('Check, is stub_LocalFuncToStub.py executed?')
        exit()

def stub_FindFuncs(ModuleDirs):
    FuncPrototypes = []
    Funcs = []
    Funcs += stub_ReadingLocalFunc()
    Funcs += stub_ReadingStaticFunc()

    for ModuleDir in ModuleDirs:
        HeaderPaths = []
        HeaderPaths += stub_SearchFile(ModuleDir, '.h')
        HeaderPaths += stub_SearchFile(ModuleDir, '.c')
        
        for HeaderPath in HeaderPaths:
            with open(HeaderPath, 'r') as File:
                Data = File.read()
            
                Rets = re.finditer(r'^(extern)?(\w+\s+)?\w+(\s*\**\s*)\w+\s*\(.*?\)', Data, re.MULTILINE|re.DOTALL)
                for Ret in Rets:
                    Prototype = Ret.group(0)
                    Prototype = re.sub(r'\s+', ' ', Prototype)
                    Prototype = re.sub(r'\s*\(\s*', '(', Prototype)
                    Prototype = re.sub(r'\s*\)\s*', ')', Prototype)
                    Prototype = re.sub(r'\s*,\s*', ',', Prototype)
                    Prototype = re.sub(r'\s+(?=\*)', '', Prototype)
                    Prototype = re.sub(r'(?<=\*)\s*', ' ', Prototype)
                    Prototype = re.sub(r'extern\s+', '', Prototype)
                    Prototype = re.sub(r'^([A-Z_])+\s+', '', Prototype)
                    Prototype = re.sub(r'^static\s+', '', Prototype)
                    for Func in Funcs:
                        if f'{Func}(' in Prototype:
                            FuncPrototypes.append(Prototype)
                            Funcs.remove(Func)
    # Check if prototype are all foundFuncs
    for Func in Funcs:
        print(f'Error: Not found prototype of {Func}')
    return FuncPrototypes

# Main
if __name__ == '__main__':
    # Input argument parse
    parser = ArgumentParser()
    parser.add_argument('-i', type=str, help='Module dir', required=True)
    # parser.add_argument('-o', type=str, help='Output directory', required=False, default='./')
    args = parser.parse_args()
    ModuleDirs = normpath(args.i).split('"')
    # OutputDir = normpath(args.o)

    with open(FUNC_PROTOTYPE, 'w') as File:
        Funcs = stub_FindFuncs(ModuleDirs)
        for Func in Funcs:
            File.write(f'{Func}\n')
