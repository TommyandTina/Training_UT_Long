
VLIB_ROOT		:= $(firstword $(shell pwd | sed -re '{s/[Vv]-?[Ll][Ii][Bb][-,a-z,A-Z,0-9]*/& /;}'))

include $(VLIB_ROOT)/compiler/arm-gnu/setup.mk

VLIB_CPU            	:= cortex-r7
VLIB_OS			:= freertos
VLIB_APP            	= taurus-$(VLIB_COMPILER)-$(VLIB_OS).a
VLIB_BOARD		:= cr7_r-carx3
VLIB_DEBUG_BUILD	:= yes
VLIB_BUILD_LOG		:= on

VLIB_CFLAGS		+= -O3

# Temporary workaround. Needed to select the implementation of logging
# function in vlib/apps/common/debug
VLIB_DEFINE   += USE_ROS

# List of virtual peripheral to enable
TAURUS_VIRTUALDEV_ENABLE := no
TAURUS_RVGC_ENABLE       := yes
TAURUS_CAMERA_ENABLE     := no
TAURUS_CAN_ENABLE        := yes
TAURUS_CAN_USE_CANFD     := yes
TAURUS_ETHER_ENABLE      := no
TAURUS_VIDEO_ENABLE      := no
RTE_COMAL_TARGET 		= rpmsg
RTE_PLATFORM            = ros_cr7_r-carx3

#
# Guest OS (linux, integrity)
#
GUEST_OS = linux
#GUEST_OS = integrity

ifeq ($(GUEST_OS), linux)
	VLIB_CFLAGS += -DLINUX_GUEST
	TAURUS_RES_TABLE_ADDR   = 0x70400000
else ifeq ($(GUEST_OS), integrity)
	VLIB_CFLAGS += -DINTEGRITY_GUEST
	TAURUS_RES_TABLE_ADDR   = 0x80000000
else
	$(error Type of guest OS not supported!)
endif


include $(VLIB_ROOT)/app/taurus/taurus.mk


