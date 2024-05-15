
#
# Application's external API
#

VLIB_INC 	+= $(VLIB_ROOT)/app/taurus/lib

TAURUS_APP_BUILD=false

ifeq ($(VLIB_APP),taurus-$(VLIB_COMPILER)-$(VLIB_OS).a)
TAURUS_APP_BUILD=true
endif

ifeq ($(VLIB_APP),taurus.out)
TAURUS_APP_BUILD=true
endif

ifeq ($(TAURUS_APP_BUILD),false)

#
# included by an application or another library
#

ifneq ($(suffix $(VLIB_APP)),.a)
VLIB_LIBS	+= $(VLIB_ROOT)/app/taurus/lib/arm_gnu_$(VLIB_OS)_r-carx3/taurus-$(VLIB_COMPILER)-$(VLIB_OS).a
endif

else

#
# included by the library itself, so add all sources and include build rules
#

#
# Source code search path
#
VLIB_VPATH := $(VLIB_ROOT)/app/taurus/src
VLIB_INC   += $(VLIB_ROOT)/app/taurus/src
VLIB_VPATH += $(VLIB_ROOT)/app/taurus/src/comserv
VLIB_INC   += $(VLIB_ROOT)/app/taurus/src/comserv
VLIB_INC   += $(VLIB_ROOT)/app/taurus/src/perserv/lib
VLIB_INC   += $(VLIB_ROOT)/app/taurus/driver/include

ifneq ($(suffix $(VLIB_APP)),.a)
VLIB_VPATH	+= $(VLIB_ROOT)/app/taurus/src/standalone
endif

ifeq ($(TAURUS_VIRTDEV_ENABLE), yes)
VLIB_DEFINE += R_TAURUS_ENABLE_VIRTDEV
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/src/perserv/virtdevserv/src
VLIB_INC    += $(VLIB_ROOT)/app/taurus/src/perserv/virtdevserv/lib
VLIB_INC    += $(VLIB_ROOT)/app/taurus/driver/virtdev/lib
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/driver/virtdev/cio/src
endif # TAURUS_VIRTDEV_ENABLE

ifeq ($(TAURUS_ETHER_ENABLE), yes)
VLIB_DEFINE += R_TAURUS_ENABLE_ETHER
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/src/perserv/ethserv/src
VLIB_INC    += $(VLIB_ROOT)/app/taurus/src/perserv/ethserv/lib
VLIB_INC    += $(VLIB_ROOT)/app/taurus/driver/eth/lib
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/driver/eth/cio/src

VLIB_VPATH  += $(VLIB_ROOT)/app/cio/driver/r-car_ether/src
VLIB_INC    += $(VLIB_ROOT)/app/cio/driver/r-car_ether/src
VLIB_INC    += $(VLIB_ROOT)/app/cio/driver/r-car_ether/lib

# require lower drivers

#Ether Module
VLIB_INC    += $(VLIB_ROOT)/drivers/ether/lib
VLIB_INC    += $(VLIB_ROOT)/drivers/ether/src/modules_common/include
VLIB_VPATH  += $(VLIB_ROOT)/drivers/ether/src/modules_common/src
VLIB_INC    += $(VLIB_ROOT)/drivers/ether/src/modules_h3/include
VLIB_VPATH  += $(VLIB_ROOT)/drivers/ether/src/modules_h3/src

# Ether stub
VLIB_INC    += $(VLIB_ROOT)/drivers/ether/src/generic
VLIB_VPATH  += $(VLIB_ROOT)/drivers/ether/src/generic

endif # TAURUS_ETHER_ENABLE

ifeq ($(TAURUS_RVGC_ENABLE), yes)
VLIB_DEFINE += R_TAURUS_ENABLE_RVGC
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/src/perserv/rvgcserv/src
VLIB_INC    += $(VLIB_ROOT)/app/taurus/src/perserv/rvgcserv/lib
VLIB_INC    += $(VLIB_ROOT)/app/taurus/driver/rvgc/lib
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/driver/rvgc/src

#
# Window Manager
#
ifneq (,$(findstring USE_ROS,$(VLIB_DEFINE)))
 ifeq ($(VLIB_OS),ros)
  include $(VLIB_ROOT)/os/ros/um_roslib/um_roslib.mk
 else
  include $(VLIB_ROOT)/os/freertos/FreeRTOS/kernel.mk
 endif
 include $(VLIB_ROOT)/os/osal_wrapper/osallib_$(VLIB_OS).mk

 include $(VLIB_ROOT)/middleware/WindowMng/um_wmlib/um_wmlib.mk
 include $(VLIB_ROOT)/app/cio/um_ciolib/um_ciolib.mk
else
 include $(VLIB_ROOT)/middleware/WindowMng/um_wmlib/um_wmlib_bm.mk
 include $(VLIB_ROOT)/middleware/WindowMng/drv_wmlib/drv_wmlib_bm.mk
endif

endif # TAURUS_RVGC_ENABLE

ifeq ($(TAURUS_CAMERA_ENABLE), yes)
VLIB_DEFINE += R_TAURUS_ENABLE_CAMERA
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/src/perserv/cameraserv/src
VLIB_INC    += $(VLIB_ROOT)/app/taurus/src/perserv/cameraserv/lib
VLIB_INC    += $(VLIB_ROOT)/app/taurus/driver/camera/lib
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/driver/camera/src
endif # TAURUS_CAMERA_ENABLE

ifeq ($(TAURUS_CAN_ENABLE), yes)
VLIB_DEFINE += R_TAURUS_ENABLE_CAN

ifeq ($(TAURUS_CAN_USE_CANFD), yes)
VLIB_DEFINE += R_TAURUS_CAN_USE_CANFD
endif

VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/src/perserv/canserv/src
VLIB_INC    += $(VLIB_ROOT)/app/taurus/src/perserv/canserv/lib
VLIB_INC    += $(VLIB_ROOT)/app/taurus/driver/can/lib
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/driver/can/cio/src
endif # TAURUS_CAN_ENABLE

ifeq ($(TAURUS_VIDEO_ENABLE), yes)
VLIB_DEFINE += R_TAURUS_ENABLE_VIDEO
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/src/perserv/videoserv/src
VLIB_INC    += $(VLIB_ROOT)/app/taurus/src/perserv/videoserv/lib
VLIB_INC    += $(VLIB_ROOT)/app/taurus/driver/video/lib
VLIB_VPATH  += $(VLIB_ROOT)/app/taurus/driver/video/cio/src
endif # TAURUS_VIDEO_ENABLE

VLIB_INC   += $(VLIB_ROOT)/app/taurus/bridge/lib
VLIB_INC   += $(VLIB_ROOT)/app/taurus/platform/lib
VLIB_VPATH += $(VLIB_ROOT)/app/taurus/platform/$(RTE_PLATFORM)/src

VLIB_DEFINE += RESOURCE_TABLE_BASE=$(TAURUS_RES_TABLE_ADDR)

#
# include abstraction layers
#
#
include $(VLIB_ROOT)/app/taurus/comal/comal.mk

#
# include generic compiler
#
include $(VLIB_ROOT)/compiler/compiler.mk

endif

