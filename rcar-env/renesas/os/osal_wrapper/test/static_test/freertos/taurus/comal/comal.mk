
#
# Set default configuration, if not configured
#

ifeq ($(RTE_COMAL_TARGET),)
RTE_COMAL_TARGET = smem
endif

#
# Include and source path
#

VLIB_INC   += $(VLIB_ROOT)/app/taurus/comal/lib
VLIB_VPATH += $(VLIB_ROOT)/app/taurus/comal/src/$(RTE_COMAL_TARGET)
