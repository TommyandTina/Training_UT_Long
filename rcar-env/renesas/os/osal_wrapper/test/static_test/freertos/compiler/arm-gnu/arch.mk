
ifneq ($(VLIB_FREERTOS_DISABLE_FPU),yes)
VLIB_COMMONFLAGS	+= -mfloat-abi=hard -mfpu=vfpv3-d16
VLIB_LDFLAGS 	        += -mfloat-abi=hard
endif

include $(VLIB_ROOT)/compiler/generic-gnu/arch.mk
