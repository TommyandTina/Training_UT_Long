import os, shutil, re
print("=============================================================\n")
print("          Creating source for IMPDRV Unit Test         \n")
print("=============================================================\n")

path = '../renesas/driver/soc/platform/imp/'
# Location of source and destination
src_dir = path+'src'
dst_dir = path+'test/unit_test/application/ut_framework/src'

# getting all the files in the source directory to test folderr0
files = os.listdir(src_dir)
shutil.copytree(src_dir, dst_dir, dirs_exist_ok=True)

#File location need to change
dsp_file = [
        '/target/v4h/r_impdrv_dspctl.c',
        '/target/v4h2/r_impdrv_dspctl.c',
        # '/target/v4m/r_impdrv_dspctl.c'
        ]
cmnctl_reg_file = [
        '/target/v4h/r_impdrv_cmnctl_reg.c',
        '/target/v4h2/r_impdrv_cmnctl_reg.c',
        # '/target/v4m/r_impdrv_cmnctl_reg.c'
        ]
cmnctl_file = [
        '/target/v3h1/r_impdrv_cmnctl.c',
        '/target/v3h2/r_impdrv_cmnctl.c',
        '/target/v3m2/r_impdrv_cmnctl.c',
        '/target/v4h/r_impdrv_cmnctl.c',
        '/target/v4h2/r_impdrv_cmnctl.c',
        # '/target/v4m/r_impdrv_cmnctl.c'
        ]
#value need to replace
dsp_macro = [
        r'IMPDRV_VDSP_GPIO_WAIT_LOOP\s+\((.+)\)',
        r'IMPDRV_VDSP_P_PDTC_WAIT_LOOP\s+\((.+)\)',
        r'IMPDRV_VDSP_P_CCOCR_WAIT_LOOP\s+\((.+)\)',
        r'IMPDRV_VDSP_D_DDCL_WAIT_LOOP\s+\((.+)\)',
        r'l_loop = (100000U);'
        ]
cmnctl_reg_macro = [
        r'wait_loop = (0x800000U);'
        ]

#Function use to reduce the loop
def loop_reduce(list_file, list_macro):
    for file in list_file:
        new_file = []
        with open(dst_dir+file, 'rt') as f:
            # read a list of lines into data
            data = f.readlines()

        for line in data:
            for macro in list_macro:
                match = re.search(macro, line)
                if match != None:
                    line = line.replace(match.group(1), '5U')
            new_file.append(line)

        # and write everything back
        with open(dst_dir+file, 'wt') as f:
            f.writelines(new_file)

#Replace static resource
def static_replace(list_file):
    format =  r' +(IMPDRV_STATIC)'
    for file in list_file:
        new_file = []
        with open(dst_dir+file, 'rt') as f:
            # read a list of lines into data
            data = f.readlines()

        for line in data:
            match = re.search(format, line)
            if match != None:
                line = line.replace(match.group(1), 'static')
            new_file.append(line)

        # and write everything back
        with open(dst_dir+file, 'wt') as f:
            f.writelines(new_file)

#main exec
loop_reduce(dsp_file, dsp_macro)
loop_reduce(cmnctl_reg_file, cmnctl_reg_macro)
static_replace(cmnctl_file)

print("=============================================================\n")
print("          Done         \n")
print("=============================================================\n")