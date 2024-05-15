ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

define PINFO
PINFO DESCRIPTION=SYSC Control
endef

NAME=sysc_ctrl
USEFILE = $(PROJECT_ROOT)/Usemsg

LIBS += rcar-syscS drvr cache

INSTALLDIR = sbin


#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk
