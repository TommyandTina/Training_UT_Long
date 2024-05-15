
#
# Source code search path
#

VLIB_INC 	+= $(VLIB_ROOT)/os/osal/include
VLIB_INC 	+= $(VLIB_ROOT)/os/osal_dev_cfg/include
VLIB_INC 	+= $(VLIB_ROOT)/os/osal_dev_cfg/src/target/freertos/include/config/$(VLIB_OS)
VLIB_INC 	+= $(VLIB_ROOT)/os/osal_dev_cfg/src/target/freertos/include
VLIB_INC 	+= $(VLIB_ROOT)/middleware/libraries/memory_allocator/include

VLIB_INC 	+= $(VLIB_ROOT)/os/osal_usr_cfg/include
VLIB_INC 	+= $(VLIB_ROOT)/os/osal_usr_cfg/src/target/freertos/include
VLIB_INC 	+= $(VLIB_ROOT)/os/osal/include/rcar-xos/osal_wrapper/include/target/$(VLIB_OS)
VLIB_INC 	+= $(VLIB_ROOT)/os/osal/include/rcar-xos/osal_wrapper/include/

VLIB_INC 	+= $(VLIB_ROOT)/os/osal_wrapper/src/include

VLIB_INC 	+= $(VLIB_ROOT)/drivers/cpg/lib
VLIB_INC 	+= $(VLIB_ROOT)/drivers/sysc/lib
VLIB_INC 	+= $(VLIB_ROOT)/drivers/io/lib
VLIB_INC 	+= $(VLIB_ROOT)/drivers/delay/lib
VLIB_INC        += $(VLIB_ROOT)/drivers/print/lib

$(warning check 1:$(VLIB_APP),osallib-$(VLIB_COMPILER)-$(VLIB_OS).a)
ifneq ($(VLIB_APP),osallib-$(VLIB_COMPILER)-$(VLIB_OS).a)

#
# included by an application or another library
#

ifneq ($(suffix $(VLIB_APP)),.a)
VLIB_LIBS	+= $(VLIB_ROOT)/os/osal_wrapper/lib/$(VLIB_COMPILER)-$(VLIB_OS)/osallib-$(VLIB_COMPILER)-$(VLIB_OS).a
endif

else

#
# included by the library itself, so add all sources and include build rules
#

VLIB_VPATH 	+= $(VLIB_ROOT)/os/osal_dev_cfg/src/target/freertos
VLIB_VPATH 	+= $(VLIB_ROOT)/os/osal_usr_cfg/src/target/freertos
VLIB_VPATH 	+= $(VLIB_ROOT)/os/osal_wrapper/src/target/$(VLIB_OS)
VLIB_VPATH 	+= $(VLIB_ROOT)/middleware/libraries/memory_allocator/src
VLIB_VPATH 	+= $(VLIB_ROOT)/drivers/delay/src
VLIB_VPATH 	+= $(VLIB_ROOT)/drivers/cpg/src
VLIB_VPATH 	+= $(VLIB_ROOT)/drivers/sysc/src
VLIB_VPATH 	+= $(VLIB_ROOT)/os/osal_wrapper/src/target/freertos/pma
VLIB_VPATH      += $(VLIB_ROOT)/drivers/print/src

# Driver PRR - used by IRQ code to determine if VSPD2 is used
VLIB_INC 	+= $(VLIB_ROOT)/drivers/prr/lib

# Definition
#VLIB_DEFINE     += configSTACK_DEPTH_TYPE=uint64_t
#VLIB_DEFINE     += USE_INTERRUPT_THREAD

$(warning check :VPATH $(VLIB_VPATH))
$(warning check :INC   $(VLIB_INC))

include $(VLIB_ROOT)/app/cio/um_ciolib/um_ciolib.mk

ifeq ($(VLIB_OS),ros)
 include $(VLIB_ROOT)/os/ros/um_roslib/um_roslib.mk
else
 include $(VLIB_ROOT)/os/freertos/FreeRTOS/kernel.mk
endif

include $(VLIB_ROOT)/compiler/compiler.mk

endif

