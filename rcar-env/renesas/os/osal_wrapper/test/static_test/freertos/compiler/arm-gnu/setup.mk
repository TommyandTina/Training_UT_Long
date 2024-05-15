include $(VLIB_ROOT)/compiler/setup.mk

VLIB_CROSS_COMPILE  := arm-none-eabi-
VLIB_COMPILER 		:= arm-gnu

# Following variable was used to disable FPU support for FreeRTOS targets
# as long as the FreeRTOS BSP was instable. It works, because only FreeRTOS
# projects include vlib/compiler/arm-gnu/setup.mk while R-OS targets do
# set VLIB_CROSS_COMPILE and VLIB_COMPILER
#
# After updating the FreeRTOS BSP, we could use the FPU again and the
# variable below is set to 'no' instead of 'yes'
#
VLIB_FREERTOS_DISABLE_FPU := no
