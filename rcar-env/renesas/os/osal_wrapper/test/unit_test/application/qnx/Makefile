RM=rm -rf
CP=cp
MKDIR=mkdir -p

#CP2NAME    := cp2
WANAME  := osal_ut
WANAME_MCDC := osal_ut_mcdc
TARGET  := ./$(WANAME)/$(WANAME_MCDC).axf
SRC     += $(wildcard ./target_mcdc/qnx/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/bsp_wrapper/qnx/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/bsp_wrapper/qnx/tbl/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/hw/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/hw/tbl/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/manager/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/statemachine/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/dev_cfg/static/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/helper/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/res_cfg/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/res_cfg/static/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/os_ifs/r_osal_os_*.c)
# SRC       += $(wildcard ./target_mcdc/qnx/os_ifs/dev_cfg/*.c)
SRC     += $(wildcard ./target_mcdc/qnx/os_ifs/usr_cfg/*.c)
# include directories
#INC_DIR    := ./$(CP2NAME)
INC_DIR += ./src/include
INC_DIR += ./target_mcdc/qnx/
INC_DIR += ../../../../../../middleware/libraries/memory_allocator/include
INC_DIR += ../../../../../osal/include
INC_DIR += ../../../../../osal_wrapper/src/include
INC_DIR += ../../../../../osal_configuration/include
USER_INC := $(addprefix -I,$(INC_DIR))
USR_ISYS    += $(addprefix -isystem,$(ISYS_DIR))
# command option
CC      := armclang -mno-unaligned-access -march=armv8.2-a --target=aarch64-arm-none-eabi -mcpu=cortex-a76
# command option
CFLAGS  += -O0
CFLAGS  += -std=c99
CFLAGS  += -nostdinc
#CFLAGS += -nostdlib
#CFLAGS += -pedantic-errors
#CFLAGS += -Werror
#CFLAGS += -D_POSIX_C_SOURCE=200809L
#CFLAGS += -DQNX_WA_DISABLE_PDMODE
#CFLAGS += -fstack-protector-strong
CFLAGS  += -Wall
CFLAGS  += -Wextra
CFLAGS  += -Wconversion
CFLAGS  += -Wunused
CFLAGS  += -Wuninitialized
CFLAGS  += -gdwarf-3
CFLAGS  += -DWINAMS_STUB
CFLAGS  += -DRCAR_V4H
CFLAGS  += -D__LITTLEENDIAN__
CFLAGS  += -D__QNXNTO__
CFLAGS  += -D_LARGEFILE64_SOURCE
CFLAGS  += -D_QNX_SOURCE
CFLAGS  += -fno-builtin
CFLAGS  += -fno-common
CFLAGS  += -DFORTIFYSOURCE=2
#-------------------------------------------------------------------------------
all:
	$(CC) $(CFLAGS) $(USER_INC) $(USR_ISYS) $(SRC) -o $(TARGET)

.PHONY: all clean
