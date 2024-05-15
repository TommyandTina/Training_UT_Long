ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk

define PINFO
PINFO DESCRIPTION=MEMORY STRESS TEST
endef

NAME := out32
USEFILE = $(PROJECT_ROOT)/Usemsg
INSTALLDIR = sbin

include $(MKFILES_ROOT)/qtargets.mk
LIBS += drvr smmu
