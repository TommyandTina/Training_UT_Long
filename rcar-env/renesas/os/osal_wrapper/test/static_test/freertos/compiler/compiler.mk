
#
# Include Compiler configuration 
#

include $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/arch.mk


#
# compiler specific header files
#

VLIB_INC 		+= $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/lib


#
# Defines
#

VLIB_DEFINE		+= ARCH_$(subst -,_,$(VLIB_COMPILER))
VLIB_DEFINE		+= CPU_$(subst -,_,$(VLIB_CPU))

#
# Define make default target
#

.PHONY : all

all:	$(VLIB_APP) $(VLIB_OBJS) $(VLIB_LIBS)


#
# Generic rules
#

VPATH 		+= $(VLIB_VPATH)

VLIB_SRC_EXT	:= c cc cpp $(VLIB_AS_EXT)

VLIB_SRCS	+= $(foreach EXT, $(VLIB_SRC_EXT),$(wildcard $(foreach DIR,$(VLIB_VPATH),$(DIR)/*.$(EXT))))
VLIB_DEPS	+= $(foreach NAME,$(VLIB_SRCS),$(basename $(notdir $(NAME))).d)
# make sure to not to add any application objects to prebuild libs 
ifneq ($(suffix $(VLIB_APP)),.a)
VLIB_OBJS	+= $(foreach NAME,$(VLIB_SRCS),$(basename $(notdir $(NAME))).$(VLIB_OBJ_EXT))
else
VLIB_LIB_OBJS	+= $(foreach NAME,$(VLIB_SRCS),$(basename $(notdir $(NAME))).$(VLIB_OBJ_EXT))
endif

VLIB_MAKE_FILES	+= $(patsubst %.d,,$(MAKEFILE_LIST))


VLIB_WINPATH 	:= $(VLIB_ROOT)
VLIB_PATH2CC	= $(1)


ifeq ($(VLIB_BUILD_LOG), on)
VLIB_BUILD_LOG := 
else
VLIB_BUILD_LOG := @
endif


$(VLIB_OBJS) : $(VLIB_MAKE_FILES)


.PHONY : lib_update

lib_update:
	@echo Updating libs
	@for i in $(VLIB_LIBS); do \
	  if [ -f $$(dirname $$i)/Makefile ]; then \
	    make -C $$(dirname $$i); \
	  fi \
	done


.PHONY : proper

proper : clean
	@echo Proper cleaning: removing libs
	@for i in $(VLIB_LIBS); do \
	  if [ -f $$(dirname $$i)/Makefile ]; then \
	    make -C $$(dirname $$i) clean; \
	  fi \
	done


#
# Info for debugging the make process
#

.PHONY : clean

clean :
	rm -f *.$(VLIB_APP_EXT) *.$(VLIB_LIB_EXT) *.$(VLIB_OBJ_EXT) *.bin *.bak *.*.bak *.lst *.lcp *.inf *.rec *.srec *.dba *.dbo *.dla *.dle *.dnm *.mono *.mono.ael *.map *.dep *.d *.i *.si *~ *.ti *.ii
	rm -rf ./ship
	rm -f makeship.sh


.PHONY : list

list:
	@echo VLIB variables
	@echo VLIB_ROOT=$(VLIB_ROOT)
	@echo VLIB_DIRNAME=$(VLIB_DIRNAME)
	@echo VLIB_OS_ROOT=$(VLIB_OS_ROOT)
	@echo VLIB_APP_ROOT=$(VLIB_APP_ROOT)
	@echo VLIB_APP=$(VLIB_APP)
	@echo VLIB_COMPILER=$(VLIB_COMPILER)
	@echo VLIB_CROSS_COMPILE=$(VLIB_CROSS_COMPILE)
	@echo VLIB_CPU=$(VLIB_CPU)
	@echo VLIB_BOARD=$(VLIB_BOARD)
	@echo VLIB_LDSCRIPT=$(VLIB_LDSCRIPT)
	@echo VLIB_DEBUG_BUILD=$(VLIB_DEBUG_BUILD)
	@echo VPATH=$(VPATH)
	@echo VLIB_VPATH=$(VLIB_VPATH)
	@echo VLIB_OBJS=$(VLIB_OBJS)
	@echo VLIB_LIBS=$(VLIB_LIBS)
	@echo VLIB_LIB_OBJS=$(VLIB_LIB_OBJS)
	@echo VLIB_INC=$(VLIB_INC)
	@echo VLIB_DEFINE=$(VLIB_DEFINE)
	@echo VLIB_DEFOPT=$(VLIB_DEFOPT)
	@echo VLIB_COMMONFLAGS=$(VLIB_COMMONFLAGS)
	@echo VLIB_CFLAGS=$(VLIB_CFLAGS)
	@echo VLIB_ASFLAGS=$(VLIB_ASFLAGS)
	@echo VLIB_LDFLAGS=$(VLIB_LDFLAGS)
	@echo VLIB_MAKE_FILES=$(VLIB_MAKE_FILES)
	@echo VLIB_BUILD_LOG=$(VLIB_BUILD_LOG)
	@echo Ship Variables:
	@echo SHIP_TARGET_DIR=$(SHIP_TARGET_DIR)
	@echo SHIP_MAKE_FILES_FULL=$(SHIP_MAKE_FILES_FULL)
	@echo SHIP_MAKEFILE_DIRS=$(SHIP_MAKEFILE_DIRS)
	@echo SHIP_LF_DIR=$(SHIP_LF_DIR)
	@echo SHIP_LF_FNAMES=$(SHIP_LF_FNAMES)
	@echo SHIP_LIBS_DIRS=$(SHIP_LIBS_DIRS)
	@echo Testing:
	@echo VLIB_TEST=$(VLIB_TEST)


#
# Ensure no object is treated as intermediate file
#

.PRECIOUS: %.$(VLIB_OBJ_EXT) %.$(VLIB_APP_EXT) %.$(VLIB_LIB_EXT) 


#
# Include Compiler specific rules
#

include $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/rules.mk

#
# Include shipping rules
#

include $(VLIB_ROOT)/compiler/ship.mk

#
# Dependency lists
#

# Only include DEPS if the target is not proper or clean
# This helps if a dependency is broken clean and proper can still be called

ifeq (,$(filter $(MAKECMDGOALS),proper clean))
-include $(VLIB_DEPS)
endif
