ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)
include $(MKFILES_ROOT)/qmacros.mk

NAME = spi-slave-control
EXTRA_INCVPATH+=$(PROJECT_ROOT)/../../../install/usr/include

USEFILE=../../Usemsg

define PINFO
PINFO DESCRIPTION=
endef

include $(PROJECT_ROOT)/pinfo.mk

include $(MKFILES_ROOT)/qtargets.mk

CCFLAGS += -g
INSTALLDIR = sbin
INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../install
USE_INSTALL_ROOT=1
