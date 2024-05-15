ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk
-include $(PROJECT_ROOT)/roots.mk
#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../install
   USE_INSTALL_ROOT=1
##############################################################

LIBS += drvr

define PINFO
PINFO DESCRIPTION=PWM sample code
endef

USEFILE = $(PROJECT_ROOT)/Usemsg

#########################################################
# Make sure the build environment is configured properly
#########################################################
include $(MKFILES_ROOT)/qtargets.mk
##########################################################
# Install our binaries/headers into the CPP shared folder
##########################################################

CCFLAGS+= -Wall
