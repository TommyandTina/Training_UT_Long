VLIB_ROOT := $(firstword $(shell pwd | sed -re '{s/[Vv]-?[Ll][Ii][Bb][-,a-z,A-Z,0-9]*/& /;}'))

include $(VLIB_ROOT)/compiler/arm-gnu/setup.mk

VLIB_APP      		 = freertos.a
VLIB_CPU           	:= cortex-r7
VLIB_BOARD         	:= cr7_r-carx3
VLIB_OS				:= freertos
VLIB_DEBUG_BUILD	:= yes
VLIB_BUILD_LOG		:= on
VLIB_CFLAGS			+= -O3  -std=gnu99

# Replicate the functionality from config.mk
VLIB_DEFINE         += RCAR_SALVATOR_XS=0
VLIB_DEFINE         += RCAR_EBISU=1
# TODO is there a better way to do this?
VLIB_DEFINE         += RCAR_BOARD=1

ifeq ($(ETHER_APP_BUILD), true)
  VLIB_DEFINE         += ETHER_ENABLE=1
else
  VLIB_DEFINE         += ETHER_ENABLE=0
endif

include $(VLIB_ROOT)/os/freertos/FreeRTOS/kernel.mk
