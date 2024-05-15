
<!-- TOC -->

- [1. License](#1-license)
- [2. Reference Module Platform](#2-reference-module-platform)
  - [2.1. Description](#21-description)
  - [2.2. CMake file structure](#22-cmake-file-structure)
    - [2.2.1. Main CMake User Section](#221-main-cmake-user-section)
    - [2.2.2. Main CMake Optional Section](#222-main-cmake-optional-section)
    - [2.2.3. Main CMake Expert Section](#223-main-cmake-expert-section)
- [How to use module.cmake](#how-to-use-modulecmake)
  - [2.3. Directory structure](#23-directory-structure)
    - [2.3.1. Content](#231-content)
    - [2.3.2. Readme](#232-readme)
  - [2.4. Coding Standards](#24-coding-standards)s

<!-- /TOC -->

---

# 1. License

Copyright [2020-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.

The contents of this repository (the "contents") are proprietary and confidential to Renesas Electronics Corporation and/or its licensors ("Renesas") and subject to statutory and contractual protections.
Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may be subject to different terms.

---

# 2. Reference Module Platform

## 2.1. Description
This is the reference OS independent module template. It shall enable you to start developing a OS independent module with the xOS platform.

TODO add more details

## 2.2. CMake file structure

The application CMake file consists on one main CMake file: CMakeLists.txt

This file contains 3 sections:
1. User Section
2. Optional Section
3. Expert Section

### 2.2.1. Main CMake User Section
The User section contains the basic things you need to set to get started:

```cmake
#----------------------------------------------------------------------------------------------------------------------
# Set module information
#----------------------------------------------------------------------------------------------------------------------
set(module_name        "refefence_module_platform" )
set(module_version     "1.0.0")

# Additinal information (product number, current quality, target quality)
set(product_number      RTM8RC7798ZPPMAB00JPF0E)
set(current_quality     Beta)       # Current Quality level: Sample, Reference, Alpha, Beta, QM-MP, ASILx-MP
set(target_quality      ASIL-B)     # Target Quality level:  Sample, Reference, QM-MP, ASILx-MP

#----------------------------------------------------------------------------------------------------------------------
# Build option
#----------------------------------------------------------------------------------------------------------------------

# SoC dependent or not (TRUE or FALSE)
set(soc_dependent       FALSE)

# Sets the modules linkage type. Allowed values are [<empty>, STATIC, INTERFACE, SHARED, MODULE]
set(module_linkage      STATIC)

#----------------------------------------------------------------------------------------------------------------------
# Common Source/Header configuration
#----------------------------------------------------------------------------------------------------------------------

# Source files
set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common_source.c
    # Note if you select build type "INTERFACE" this variable must be empty or not set
)
```

Fill this section with all the files you require.

**Note:** variable name must be the same as the template.

Allowable variables:

| Variable | Description| 
|--        |--|
|module_name  |Module Name|
|module_version|Module Version|
|product_number|Product Number  |
|current_quality|Current Quality: Sample, Reference, Alpha, Beta, QM, ASIL |
|target_quality|Target Quality: Sample, Reference, QM, ASIL|
|soc_dependent|SoC dependent or not (TRUE or FALSE)|
|module_linkage|Kodule linkage type. Allowed values are [empty, STATIC, INTERFACE, SHARED, MODULE]|
|source|Source files|
|interface_source| Interface source files that always needs to be compiled with the application and not during artifact generation. Example: UDF sources|
|public_header|Public Header that are exported and can be used by application or other modules|
|private_header|Private Header files that are not exported and cannot be used by other components or modules. |
|external_public_include_dir| An external (3rd Party) include directory. This information is exported with the modules|
|external_private_include_dir| An private external include directory required to build this module (that are not exported and cannot be used by other components or modules)|
|compile_def|Compile Definitions|
|private_compile_def|Privation Compile Definitions|
|link_lib|Library Dependencies|
|private_link_lib|Private Library Dependencies|
|link_dir|Link Directories|
|private_link_dir|Private Link Directories|


### 2.2.2. Main CMake Optional Section

This section is for "semi" experts. Here you can implement/specify anything special the module may require.
Example:
* SoC Specific files

Variable naming rule of this section: 

Any variable in this section, you may need add a suffix (lower letters) to the variable defined in the [common section](#221-main-cmake-user-section)

Suffix can be target SoC, Env or combination of them.

    E.g. Specific sources for HIL Env:            source_hil
    E.g. Specific sources for V3H1 SoC:           source_v3h1

### 2.2.3. Main CMake Expert Section

This section is for experts or our buildsystem maintainers. Do not change something in this section unless it is really required and you do now what to do. In general no modification of this code should be required by a normal user.
```cmake
#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_module()
```

# How to use module.cmake

This file helps to identify which SoCs and OS's are supported by the application/module. 

RCar-Environment will check this file to enable this application/module for building.

Allowable variables:

| Variable    | Description| 
|      --     |     --     |
|SUPPORTED_SOC|specifies which SoCs are supported: [any, V3H1, V3M2, future SoC]|
|SUPPORTED_OS |specifies which OSs are supported: [any, freeRTOS, linux, future OS]|
|SUPPORTED_XIL|specifies which environments are supported: [any, HIL, XIL]|

*E.g:*
```cmake
set(SUPPORTED_SOC   any)
set(SUPPORTED_OS    any)
set(SUPPORTED_XIL   HIL)
```

## 2.3. Directory structure

### 2.3.1. Content


The OS dependent reference module template directory contains 4 main subdirectories:

 - doc

   Contains SW documentation: release_note, api_manual, user_manual

 - include
 
   The include subdirectory contains a subdirectory called “rcar-xos”. The r-car-xos subdirectory contains a directory that has the same name as the SW module. The “MODULE_NAME” directory shall contain the public include files (e.g. header files). This structure is done to allow the following include statements in source code files:

    #include “r-car-xos/<_modulename_>/<_public_headerfile_>.h

 - src
 
   Contains src code files for the OS dependent SW component. 

 - test
   
   - functional_test
   
     Functional tests typically test the functionality of the SW component as given by the requirements. Typically, the developer writes specific test applications for functional tests.
     For such test applications the REFERENCE_TEST_APPLICATION template directory structure shall be used.
   
   - unit_test
   
     Unit tests typically focus on the coverage of the API of the SW component under test. Sometimes these tests are done with specific tools. This directory should contain tool configuration files or test applications.
     For test applications the REFERENCE_TEST_APPLICATION template directory structure shall be used.
   
   - static_test
   
     Static tests typically focus on the compliance to standards (e.g. MISRA standard or coding style guides etc.) of the SW component under test. Usually these tests are done with specific tools.
     This directory should contain tool configuration files or test applications.
     For such test applications the REFERENCE_TEST_APPLICATION template directory structure shall be used

   - sample_app: 
  
     Sample app for this module.  Sample applications are lifted from the restriction to be MISRA complaint.
     The REFERENCE_APPLICATION template directory structure shall be used.

### 2.3.2. Readme

Every module should contain a minimal readme in a certain format. This example readme shows the structure and basic contents. Of course this needs to match your module.

## 2.4. Coding Standards

Any Renesas Software Component must follow the same common coding standard: See GSCE + EST coding standard for the details.
