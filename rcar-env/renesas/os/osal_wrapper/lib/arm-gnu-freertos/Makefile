
VLIB_ROOT := $(firstword $(shell pwd | sed -re '{s/[Vv]-?[Ll][Ii][Bb][-,a-z,A-Z,0-9]*/& /;}'))

include $(VLIB_ROOT)/compiler/arm-gnu/setup.mk

VLIB_CPU            := cortex-r7
VLIB_OS				:= freertos
VLIB_APP      		 = osallib-$(VLIB_COMPILER)-$(VLIB_OS).a
VLIB_DEBUG_BUILD	:= yes
VLIB_BUILD_LOG		:= on

VLIB_CFLAGS			+= -std=gnu99
VLIB_CFLAGS 		+= -O3

include $(VLIB_ROOT)/os/osal_wrapper/osallib_$(VLIB_OS).mk
