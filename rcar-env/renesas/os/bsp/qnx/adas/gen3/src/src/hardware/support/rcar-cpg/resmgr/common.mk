ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

define PINFO
PINFO DESCRIPTION=R-Car CPG Resource Manager
endef

LDFLAGS += -Wl,-E

LIBS += drvr slog2

INSTALLDIR=sbin

USEFILE=$(PROJECT_ROOT)/cpg.use
NAME=rcar-cpg-mgr


#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk
