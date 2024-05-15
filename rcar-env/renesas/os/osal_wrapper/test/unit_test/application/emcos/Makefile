RM=rm -rf
CP=cp
MKDIR=mkdir -p

#CP2NAME	:= cp2
WANAME	:= osal_ut
WANAME_MCDC	:= osal_ut_mcdc
TARGET	:= ./$(WANAME)/$(WANAME_MCDC).axf
SRC		+= $(wildcard ./target_mcdc/osal_ut_sstm/*.c)
SRC		+= $(wildcard ./target_mcdc/src/*.c)
SRC		+= $(wildcard ./target_mcdc/emcos/*.c)
SRC		+= $(wildcard ./target_mcdc/emcos/dev_cfg/*.c)
SRC		+= $(wildcard ./target_mcdc/emcos/usr_cfg/*.c)
#SRC		+= $(wildcard ./target_mcdc/common/pma_dummy/*.c)
# include directories
#INC_DIR	:= ./$(CP2NAME)
INC_DIR	+= ./src/include
INC_DIR	+= ./target_mcdc/emcos/
INC_DIR	+= ../../../../../../middleware/libraries/memory_allocator/include
INC_DIR	+= ../../../../../../middleware/libraries/memory_allocator/src/include
INC_DIR	+= ../../../../../osal/include
INC_DIR	+= ../../../../../osal_wrapper/src/include
INC_DIR	+= ../../../../../osal_configuration/include
ISYS_DIR	:= C:/Renesas/poky/3.1.3/sysroots/aarch64-poky-linux/usr/include
ISYS_DIR	+= C:/Renesas/poky/3.1.3/sysroots/x86_64-pokysdk-mingw32/usr/lib/aarch64-poky-linux/gcc/aarch64-poky-linux/9.3.0/include
ISYS_DIR	+= C:/Renesas/poky/3.1.3/sysroots/x86_64-pokysdk-mingw32/usr/lib/aarch64-poky-linux/gcc/aarch64-poky-linux/9.3.0/include-fixed

USER_INC := $(addprefix -I,$(INC_DIR))
USR_ISYS	+= $(addprefix -isystem,$(ISYS_DIR))
# command option
CC		:= armclang -mno-unaligned-access -march=armv8-a --target=aarch64-arm-none-eabi -mcpu=cortex-a53
# command option
CFLAGS	+= -O0
CFLAGS	+= -std=c99
#CFLAGS	+= -fPIC
#CFLAGS	+= -nostdlib
#CFLAGS	+= -nostdlibinc
#CFLAGS	+= -pedantic-errors
#CFLAGS	+= -Werror
#CFLAGS	+= -Wall
CFLAGS	+= -Wextra
CFLAGS	+= -Wconversion
CFLAGS	+= -Wunused
CFLAGS	+= -Wuninitialized
CFLAGS	+= -gdwarf-3
CFLAGS	+= -DWINAMS_STUB
CFLAGS	+= -D_POSIX_C_SOURCE=200809L
CFLAGS	+= -DRCAR_V4H
CFLAGS	+= -D__LITTLEENDIAN__
CFLAGS	+= -D__ARM__
CFLAGS	+= -D__QNXNTO__
#CFLAGS	+= -lrt
#CFLAGS	+= -lpthread
#-------------------------------------------------------------------------------
all: 
	$(CC) $(CFLAGS) $(USER_INC) $(USR_ISYS) $(SRC) -o $(TARGET)

.PHONY: all clean
