RM=rm -rf
CP=cp
MKDIR=mkdir -p

#CP2NAME	:= cp2
WANAME	:= osal_ut
WANAME_MCDC	:= osal_ut_mcdc
TARGET	:= ./$(WANAME)/$(WANAME_MCDC).axf
SRC		+= $(wildcard ./target_mcdc/zephyr/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/bsp_wrapper/zephyr/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/bsp_wrapper/zephyr/tbl/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/hw/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/hw/tbl/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/manager/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/statemachine/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/dev_cfg/static/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/helper/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/res_cfg/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/res_cfg/static/*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/os_ifs/r_osal_os_*.c)
SRC		+= $(wildcard ./target_mcdc/zephyr/os_ifs/usr_cfg/*.c)

# include directories
INC_DIR	+= ./src/include
INC_DIR	+= ./src/include/zephyrproject/zephyr
INC_DIR	+= ./src/include/zephyrproject/zephyr/include
INC_DIR	+= ./src/include/zephyr-sdk-0.14.2/arm-zephyr-eabi/lib/gcc/arm-zephyr-eabi/10.3.0/include
INC_DIR	+= ./src/include/zephyr-sdk-0.14.2/arm-zephyr-eabi/lib/gcc/arm-zephyr-eabi/10.3.0//include-fixed
INC_DIR	+= ./src/include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include
INC_DIR	+= ./src/include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/include
INC_DIR	+= ./src/include/zephyrproject/zephyr/include/generated
INC_DIR	+= ./src/include/zephyrproject/zephyr/soc/arm/renesas_rcar/gen4
INC_DIR	+= ./src/include/zephyrproject/modules/hal/cmsis/CMSIS/Core_R/Include
INC_DIR	+= ./src/include/zephyrproject/zephyr/include/generated/syscalls
INC_DIR	+= ./target_mcdc/zephyr/
INC_DIR	+= ../../../../../../middleware/libraries/memory_allocator/include
INC_DIR	+= ../../../../../osal/include
INC_DIR	+= ../../../../../osal_wrapper/src/include
INC_DIR	+= ../../../../../osal_configuration/include
USER_INC := $(addprefix -I,$(INC_DIR))
USR_ISYS	+= $(addprefix -isystem,$(ISYS_DIR))

# command option
CC		:= armclang -mno-unaligned-access --target=arm-arm-none-eab -mcpu=cortex-r52

# command option
CFLAGS	+= -O0
CFLAGS	+= -std=c99
CFLAGS	+= -Wextra
CFLAGS	+= -Wconversion
#CFLAGS	+= -Wunused
CFLAGS	+= -Wuninitialized
CFLAGS	+= -gdwarf-3
CFLAGS	+= -DWINAMS_STUB
CFLAGS	+= -DRCAR_V4H
CFLAGS	+= -D__LITTLEENDIAN__
CFLAGS	+= -D__ARM__
CFLAGS	+= -D__ZEPHYR__
CFLAGS	+= -DGCC_VERSION=0
CFLAGS	+= -D__inline__=
CFLAGS	+= -D_POSIX_SOURCE=
#CFLAGS	+= -lrt
#CFLAGS	+= -lpthread
#-------------------------------------------------------------------------------
all: 
	$(CC) $(CFLAGS) $(USER_INC) $(USR_ISYS) $(SRC) -o $(TARGET)

.PHONY: all clean
