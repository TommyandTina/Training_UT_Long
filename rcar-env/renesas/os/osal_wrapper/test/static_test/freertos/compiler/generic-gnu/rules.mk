
#
# Add include pathes
#

VLIB_INCOPT     := $(foreach DIR,$(sort $(VLIB_INC)),-I$(DIR))


#
# Add defines
#

VLIB_DEFOPT 	:= $(foreach DEFINE,$(sort $(VLIB_DEFINE)),-D$(DEFINE))


#
# Define compiler, assembler and linker options
#

VLIB_CFLAGS     += $(VLIB_COMMONFLAGS) $(VLIB_INCOPT) $(VLIB_DEFOPT) 
VLIB_ASFLAGS    += $(VLIB_COMMONFLAGS) $(VLIB_INCOPT) $(VLIB_DEFOPT)

ifneq ($(VLIB_LDSCRIPT),)
VLIB_LDFLAGS	+= -T $(VLIB_LDSCRIPT)
endif

#
# Define compiler, assembler, linker and library build rules
#

VLIB_DEP_BUILD_RULE =

CC_BUILD      = $(VLIB_BUILD_LOG)$(CC) $(VLIB_CFLAGS) $< $(VLIB_DEP_BUILD_RULE)
CXX_BUILD     = $(VLIB_BUILD_LOG)$(CXX) $(VLIB_CFLAGS) $< $(VLIB_DEP_BUILD_RULE)
ASX_BUILD     = $(VLIB_BUILD_LOG)$(AS) $(VLIB_ASFLAGS) $< $(VLIB_DEP_BUILD_RULE)
LXX_BUILD     = $(VLIB_BUILD_LOG)$(LINK) $(VLIB_OBJS) -Wl,--start-group $(VLIB_LIBS) -Wl,--end-group -o $@ -Wl,-Map=$(basename $@).map $(VLIB_LDFLAGS); $(OBJCOPY) -O binary $@ $(basename $@).bin; $(OBJDUMP) -d $@ > $(basename $@).lst
ARXX_BUILD    = $(VLIB_BUILD_LOG)$(AR) -rc $@ $(VLIB_LIB_OBJS)

#
# Define rules for each file extension
#


%.$(VLIB_LIB_EXT) : $(VLIB_LIB_OBJS) $(VLIB_MAKE_FILES)
	@echo Creating library $(notdir $@)
	$(ARXX_BUILD)


%.$(VLIB_APP_EXT) :$(VLIB_OBJS) $(VLIB_MAKE_FILES) $(VLIB_LDSCRIPT) lib_update
	@echo Linking $(notdir $@)
	$(LXX_BUILD)
ifneq ($(VLIB_CREATE_SREC),)
	$(OBJCOPY) -O srec --srec-forceS3 $@ $(basename $@).srec
endif
%.$(VLIB_OBJ_EXT) : %.S $(VLIB_MAKE_FILES)
	@echo Assembling $(notdir $<)
	$(ASX_BUILD)

%.$(VLIB_OBJ_EXT) : %.s $(VLIB_MAKE_FILES)
	@echo Assembling $(notdir $<)
	$(ASX_BUILD)

%.$(VLIB_OBJ_EXT) : %.c $(VLIB_MAKE_FILES)
	@echo Compiling $(notdir $<)
	$(CC_BUILD)

%.$(VLIB_OBJ_EXT) : %.cc $(VLIB_MAKE_FILES)
	@echo Compiling $(notdir $<)
	$(CXX_BUILD)

%.$(VLIB_OBJ_EXT) : %.cpp $(VLIB_MAKE_FILES)
	@echo Compiling $(notdir $<)
	$(CXX_BUILD)

