FREERTOS_SOURCE			:= $(VLIB_ROOT)/os/freertos/FreeRTOS/Source
RCAR_BSP_SOURCE			:= $(VLIB_ROOT)/os/freertos/FreeRTOS/RTOS/BSP
FREERTOS_PLUS_DEMO		:= $(VLIB_ROOT)/os/freertos/FreeRTOS-Plus/Demo/Common/FreeRTOS_Plus_CLI_Demos
FREERTOS_PLUS_SOURCE	:= $(VLIB_ROOT)/os/freertos/FreeRTOS-Plus/Source
DEMO_SOURCE				:= $(VLIB_ROOT)/os/freertos/FreeRTOS/Demo/R-Car_Gen3_CR7

#
# Public include paths
# TODO some of these paths are only needed to build freertos and so should be in the private section below
#

VLIB_INC	+= $(FREERTOS_SOURCE)/include
VLIB_INC	+= $(FREERTOS_SOURCE)/portable/GCC/ARM_CR7

# Demo (includes GIC, timer, printf)
VLIB_INC	+= $(DEMO_SOURCE)/common
VLIB_INC	+= $(DEMO_SOURCE)/CMSIS
VLIB_INC	+= $(VLIB_ROOT)/os/freertos/FreeRTOS/Demo/Common/include

# FreeRTOS-Plus (CLI, Trace)
VLIB_INC	+= $(FREERTOS_PLUS_DEMO)
VLIB_INC	+= $(FREERTOS_PLUS_SOURCE)/FreeRTOS-Plus-CLI
VLIB_INC	+= $(FREERTOS_PLUS_SOURCE)/FreeRTOS-Plus-Trace/Include
VLIB_INC	+= $(FREERTOS_PLUS_SOURCE)/FreeRTOS-Plus-Trace/config

ifneq ($(VLIB_APP),freertos.a)

# included by an application or another library

ifneq ($(suffix $(VLIB_APP)),.a)
#VLIB_LIBS      += $(VLIB_ROOT)/os/freertos/FreeRTOS/target/$(VLIB_BOARD)/freertos.a
VLIB_LIBS	+= $(VLIB_ROOT)/os/freertos/FreeRTOS/target/cr7_r-carx3/freertos.a

endif

else

# included by the library itself, so add all sources and include build rules

#
# Core FreeRTOS
#

VLIB_VPATH 	+= $(FREERTOS_SOURCE)
VLIB_VPATH 	+= $(FREERTOS_SOURCE)/portable/GCC/ARM_CR7
# In FreeRTOS, there are different implementations of pvPortMalloc in portable/MemMang,
# each with their own C file, you just build the one you want. Since VPATH can't
# cope with this, the portable dir contains a link to the C file we want to use.
VLIB_VPATH 	+= $(FREERTOS_SOURCE)/portable

# Demo (includes GIC, timer, printf)
VLIB_VPATH	+= $(DEMO_SOURCE)/common
VLIB_VPATH	+= $(DEMO_SOURCE)/CMSIS
VLIB_VPATH	+= $(DEMO_SOURCE)/Blinky

# FreeRTOS-Plus (CLI, Trace)
VLIB_VPATH	+= $(FREERTOS_PLUS_DEMO)
VLIB_VPATH	+= $(FREERTOS_PLUS_SOURCE)/FreeRTOS-Plus-CLI
VLIB_VPATH	+= $(FREERTOS_PLUS_SOURCE)/FreeRTOS-Plus-Trace


# include generic compiler
include $(VLIB_ROOT)/compiler/compiler.mk

endif
