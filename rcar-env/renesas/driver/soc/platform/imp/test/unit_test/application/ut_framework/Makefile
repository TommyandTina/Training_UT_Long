#    _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#   _/                                                          _/
#  _/     Makefile for Renesas Modules                         _/
# _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#   2017.01.12 Rev 0.0 Newly created by T.Ohashi
SHELL=cmd
# RMDIR=rm -rf
# RM=rm -rf
RMDIR=rmdir /S /Q
RM=del /S /Q

TEST_ENV=TEST_ENV_UT
OBJDIR	:= obj
DEPDIR	:= dep
DRVDIR  := imp
OBJS :=
DEPS :=

DEVICE  ?= v4m

# Convert to upper string
ifeq ($(DEVICE),v3m2)
UPPER_DEVICE := V3M2
endif

ifeq ($(DEVICE),v3h1)
UPPER_DEVICE := V3H1
endif

ifeq ($(DEVICE),v3h2)
UPPER_DEVICE := V3H2
endif

ifeq ($(DEVICE),v4h)
UPPER_DEVICE := V4H
endif

ifeq ($(DEVICE),v4h2)
UPPER_DEVICE := V4H2
endif

ifeq ($(DEVICE),v4m)
UPPER_DEVICE := V4M
endif

#-----------------------------------------------------------------
# Test source files
#-----------------------------------------------------------------
C_SRC    := $(wildcard ./*.c)
C_SRC    += $(wildcard ./base/imp_stub/*.c)
C_SRC    += $(wildcard ./base/util/*.c)
C_SRC	 += $(wildcard ./framework/*.c)
C_SRC	 += $(wildcard ./ut/*.c)

INCDIR   += \
	-Iinclude \
	-Ibase/include \
	-Ibase/imp/include \
	-Ibase/imp_stub \
	-Ibase/imp_stub/include \
	-Iut/imp/include \
	-I../../../../../../../../os/osal/include \
	-I../../../../../../../soc/platform/imp/include \
	-I../../../../../../../soc/platform/imp/include/rcar-xos/imp \
	-I../../../../../../../soc/platform/imp/src \
	-I../../../../../../../soc/platform/imp/src/target/$(DEVICE)
	
OBJS     += $(subst ./,./$(OBJDIR)/,$(C_SRC:.c=.c.o))
DEPS     += $(subst ./,./$(DEPDIR)/,$(C_SRC:.c=.c.d))

C_SRC1    += $(wildcard ./base/imp/r_impdrv_api/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_cmnctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_cmnctl_reg/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_cnnctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_dmactl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_dmasctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_dspctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_genctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_impctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_impsctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_ocvctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_osdep/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_pscctl/*.c)
C_SRC1    += $(wildcard ./base/imp/r_impdrv_udefctl/*.c)

C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_api/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_cmnctl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_cmnctl_reg/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_cnnctl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_dmactl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_dmasctl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_dspctl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_genctl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_impctl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_impsctl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_ocvctl/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_osdep/*.c)
C_SRC2	 += $(wildcard ./ut/imp/r_impdrv_pscctl/*.c)
C_SRC2     += $(wildcard ./ut/imp/r_impdrv_udefctl/*.c)

# filter out test source of some HW specific
# v3x
ifeq (, $(findstring v4h,$(DEVICE)))
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v4h1*),$(C_SRC1))
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v4h2*),$(C_SRC1))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v4h1*),$(C_SRC2))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v4h2*),$(C_SRC2))
endif
# v4h1
ifeq ($(DEVICE),v4h)
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v3x*),$(C_SRC1))
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v4h2*),$(C_SRC1))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v3x*),$(C_SRC2))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v4h2*),$(C_SRC2))
endif
# v4h2
ifeq ($(DEVICE),v4h2)
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v4h1*),$(C_SRC1))
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v3x*),$(C_SRC1))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v4h1*),$(C_SRC2))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v3x*),$(C_SRC2))
endif

# v4m
ifeq ($(DEVICE),v4m)
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v3x*),$(C_SRC1))
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v4h1*),$(C_SRC1))
C_SRC1	:= $(filter-out $(wildcard ./base/imp/**/*v4h2*),$(C_SRC1))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v3x*),$(C_SRC2))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v4h1*),$(C_SRC2))
C_SRC2	:= $(filter-out $(wildcard ./ut/imp/**/*v4h2*),$(C_SRC2))
endif


OBJS1     += $(subst ./,./$(OBJDIR)/,$(C_SRC1:.c=.c.o))
DEPS1     += $(subst ./,./$(DEPDIR)/,$(C_SRC1:.c=.c.d))

OBJS2     += $(subst ./,./$(OBJDIR)/,$(C_SRC2:.c=.c.o))
DEPS2     += $(subst ./,./$(DEPDIR)/,$(C_SRC2:.c=.c.d))

#-----------------------------------------------------------------
# Test Target files
#-----------------------------------------------------------------
C_DRV_SRC  = $(abspath ../../../../src/r_impdrv_api.c)
C_DRV_SRC  += $(abspath ../../../../src/r_impdrv_osdep_mmngr.c)
# C_DRV_SRC  += $(abspath ../../../../src/target/$(DEVICE)/r_impdrv_udefctl.c)

DRV_OBJS  = $(subst $(abspath ../../../../src),./$(OBJDIR)/src,$(C_DRV_SRC:.c=.c.o))
DRV_DEPS  = $(subst $(abspath ../../../../src),./$(DEPDIR)/src,$(C_DRV_SRC:.c=.c.d))
INCDIR    += -I../../../../include
INCDIR    += -I../../../../src/include
OBJS      += $(DRV_OBJS)
DEPS      += $(DRV_DEPS)

#--------------------------------------------------------------
# For Environment Configurations
#--------------------------------------------------------------
TEST_CONFIGS  += -DTEST_ENV_UT
TEST_CONFIGS  += -DUPDATE_IMP_TOP
TEST_CONFIGS  += -DIMPDRV_BUILD_UNIT_TEST
TEST_CONFIGS  += -DRCAR_$(UPPER_DEVICE)
TEST_CONFIGS  += -DTEST_SOC=\"$(UPPER_DEVICE)\"
TEST_CONFIGS  += -DWINDOWS_OS
TEST_CONFIGS  += -DNDEBUG 
TEST_CONFIGS  += -DIMPDRV_DSPCRC_DETECT_ADDDEF
EXTRA_CFLAGS += -std=c99 -g -O0

CC := gcc
LD := g++

OUTPUT  = impdrv_ut.exe

#CANTPP_CMD = cppccd "--comp:x86-Win32-gcc4.6.2-bundled" --no_link
#CANTPP_LD_CMD = cppccd "--comp:x86-Win32-gcc4.6.2-bundled"
#CANTPP_TARGET_OPTS = "--parse:--line_directives" "--parse:-W2" "--sm:--call_seq_code" "--ci:--instr:func;call;stmt;decn;log;" "--analyse"

TEST_CONFIGS += -DWIN32 

all: $(OUTPUT)

$(OUTPUT) : $(OBJS) $(DEPLIB) base1.a base2.a
	$(CANTPP_LD_CMD) $(CANTPP_TARGET_OPTS) $(LD) $(LIBDIR) $(CONFIG_FLAG) $(RELINCLIB) $(ADDULIBPATH) $(OS_LIBS) $(ULIBS) $(RELLIB) -o $@ $(OBJS) base2.a base1.a $(LDFLAGS)

base1.a: $(OBJS1)
	ar rcs base1.a $(OBJS1)

base2.a: $(OBJS2)
	ar rcs base2.a $(OBJS2)
	
libs: base1.a base2.a

$(OBJDIR)/%.c.o: ./%.c
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst $(OBJDIR)/,$(DEPDIR)/,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -MF $(addprefix $(DEPDIR)/,$(subst .c,.c.d,$<)) -c -o $(subst ./$(OBJDIR)/,./,$@)  $<

./obj/base/imp_stub/r_impdrv_genctl_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_genctl_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_cmnctl_reg_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_cmnctl_reg_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_cmnctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_cmnctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_cnnctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_cnnctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_pscctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_pscctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_ocvctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_ocvctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_impctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_impctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<
	
./obj/base/imp_stub/r_impdrv_dmactl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_dmactl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<
	
./obj/base/imp_stub/r_impdrv_osdep_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_osdep_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/src/r_impdrv_api.c.o: $(abspath ../../../../src/r_impdrv_api.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

# ./obj/src/r_impdrv_osdep.c.o: $(abspath ../../../../src/r_impdrv_osdep.c)
# 	$(eval ODIR=$(dir $@))
# 	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
# 	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/src/r_impdrv_osdep_mmngr.c.o: $(abspath ../../../../src/r_impdrv_osdep_mmngr.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<


./obj/src/target/$(DEVICE)/r_impdrv_cmnctl_reg.c.o: $(abspath ../../../../src/target/$(DEVICE)/r_impdrv_cmnctl_reg.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

ifeq ($(DEVICE), v3h2)
./obj/base/imp_stub/r_impdrv_impsctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_impsctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_dmasctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_dmasctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<
endif

ifeq ($(DEVICE), v3h1)
./obj/base/imp_stub/r_impdrv_impsctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_impsctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<
endif

ifeq ($(DEVICE), v4h)
./obj/base/imp_stub/r_impdrv_dmasctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_dmasctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_dspctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_dspctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

# ./obj/src/target/$(DEVICE)/r_impdrv_dspctl.c.o: $(abspath ../../../../src/target/$(DEVICE)/r_impdrv_dspctl.c)
# 	$(eval ODIR=$(dir $@))
# 	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
# 	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<
endif

ifeq ($(DEVICE), v4h2)
./obj/base/imp_stub/r_impdrv_dmasctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_dmasctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_dspctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_dspctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

# ./obj/src/target/$(DEVICE)/r_impdrv_dspctl.c.o: $(abspath ../../../../src/target/$(DEVICE)/r_impdrv_dspctl.c)
# 	$(eval ODIR=$(dir $@))
# 	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
# 	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

# ./obj/src/target/$(DEVICE)/r_impdrv_udefctl.c.o: $(abspath ../../../../src/target/$(DEVICE)/r_impdrv_udefctl.c)
# 	$(eval ODIR=$(dir $@))
# 	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
# 	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_udefctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_udefctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<
endif

ifeq ($(DEVICE), v4m)
./obj/base/imp_stub/r_impdrv_dmasctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_dmasctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_dspctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_dspctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

# ./obj/src/target/$(DEVICE)/r_impdrv_dspctl.c.o: $(abspath ../../../../src/target/$(DEVICE)/r_impdrv_dspctl.c)
# 	$(eval ODIR=$(dir $@))
# 	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
# 	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

# ./obj/src/target/$(DEVICE)/r_impdrv_udefctl.c.o: $(abspath ../../../../src/target/$(DEVICE)/r_impdrv_udefctl.c)
# 	$(eval ODIR=$(dir $@))
# 	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
# 	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
# 	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<

./obj/base/imp_stub/r_impdrv_udefctl_stub_ut.c.o: $(abspath ./base/imp_stub/r_impdrv_udefctl_stub_ut.c)
	$(eval ODIR=$(dir $@))
	$(eval DDIR=$(subst ./$(OBJDIR)/src/obj,./$(OBJDIR)/src/dep,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(ODIR)) (mkdir $(subst /,\\,$(ODIR)))
	@if NOT EXIST $(subst /,\\,$(DDIR)) (mkdir $(subst /,\\,$(DDIR)))
	$(CANTPP_CMD) $(CANTPP_TARGET_OPTS) $(CC) $(INCDIR) $(RELINCE) $(C_CONFIG) $(DEBUG_CFLAGS) $(EXTRA_CFLAGS) $(DEF) -D$(TEST_ENV) $(TEST_CONFIGS) -MD -c -o $@ $<
endif

-include $(DEPS)

###note: clean for linux 
# clean:
# 	$(RM) $(OBJDIR) $(DRVDIR)/obj $(DRVDIR)/dep $(DRVDIR)/obj_$(DEVICE) $(DRVDIR)/dep_$(DEVICE) $(DEPDIR) $(OUTPUT) *.csi *.cov *.exe *.ctr *.ctg *.a

###note: clean for window 
clean:
	-$(RMDIR) $(OBJDIR)
	-$(RMDIR) $(DRVDIR)/obj
	-$(RMDIR) $(DRVDIR)/dep
	-$(RMDIR) $(DRVDIR)/obj_$(DEVICE)
	-$(RMDIR) $(DRVDIR)/dep_$(DEVICE)
	-$(RMDIR) $(DEPDIR)
	-$(RM) $(OUTPUT) *.csi *.cov *.exe *.ctr *.ctg *.a

.PHONY: all clean miniclean

ALL_TESTS := 

NUMBER_OF_TESTS := 0