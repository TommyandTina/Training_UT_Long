RM=rm -rf
CP=cp
MKDIR=mkdir -p

#CP2NAME	:= cp2
WANAME	:= osal_ut
WANAME_MCDC	:= osal_ut_mcdc
TARGET	:= ./$(WANAME)/$(WANAME_MCDC).axf
SRC		+= $(wildcard ./target_mcdc/linux/*.c)
SRC		+= $(wildcard ./target_mcdc/linux/common/*.c)
SRC		+= $(wildcard ./target_mcdc/linux/common/res_cfg/r_osal_res_cfg_memory.c)
SRC		+= $(wildcard ./target_mcdc/linux/common/res_cfg/dynamic/*.c)
SRC		+= $(wildcard ./target_mcdc/linux/common/dev_cfg/dynamic/*.c)
SRC		+= $(wildcard ./target_mcdc/linux/common/helper/*.c)
SRC		+= $(wildcard ./target_mcdc/linux/os_if/r_osal_os_*.c)
SRC		+= $(wildcard ./target_mcdc/linux/stub/*.c)
# include directories
INC_DIR	+= ./src/include
INC_DIR	+= ./target_mcdc/linux/
INC_DIR	+= ../../../../../../middleware/libraries/memory_allocator/include
INC_DIR	+= ../../../../../osal/include
INC_DIR	+= ../../../../../osal_configuration/include
INC_DIR	+= ../../../../../osal_wrapper/src/include
USER_INC := $(addprefix -I,$(INC_DIR))
USR_ISYS	+= $(addprefix -isystem,$(ISYS_DIR))
# command option
CC		:= armclang -mno-unaligned-access --target=aarch64-arm-none-eabi -mcpu=cortex-a76
# command option
CFLAGS	+= -O0
CFLAGS	+= -std=c99
CFLAGS	+= -Wextra
CFLAGS	+= -Wconversion
CFLAGS	+= -Wunused
CFLAGS	+= -Wuninitialized
CFLAGS	+= -gdwarf-3
CFLAGS	+= -DWINAMS_STUB
CFLAGS	+= -D_POSIX_C_SOURCE=200809L
CFLAGS	+= -D__noreturn__=
CFLAGS	+= -DRCAR_V4H
CFLAGS	+= -D__LITTLEENDIAN__
CFLAGS	+= -D__ARM__
CFLAGS	+= -D__LINUX__
#CFLAGS	+= -lrt
#CFLAGS	+= -lpthread
#-------------------------------------------------------------------------------
all: 
	$(CC) $(CFLAGS) $(USER_INC) $(USR_ISYS) $(SRC) -o $(TARGET)

.PHONY: all clean
