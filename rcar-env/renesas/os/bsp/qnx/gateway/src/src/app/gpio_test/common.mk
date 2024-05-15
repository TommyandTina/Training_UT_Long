ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk

define PINFO
PINFO DESCRIPTION=GPIO
endef

USEFILE=$(PROJECT_ROOT)/Usemsg
INSTALLDIR = sbin

include $(MKFILES_ROOT)/qtargets.mk


INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../install
USE_INSTALL_ROOT=1
