ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

define PINFO
PINFO DESCRIPTION=R-Car GEN3 and GEN4 SYSC support code
endef

include $(MKFILES_ROOT)/qmacros.mk

#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk

