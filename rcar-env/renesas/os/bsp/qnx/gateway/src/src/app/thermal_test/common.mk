ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk

define PINFO
PINFO DESCRIPTION=THERMAL TEST
endef

NAME := thermal_test
USEFILE = $(PROJECT_ROOT)/Usemsg
INSTALLDIR = sbin

include $(MKFILES_ROOT)/qtargets.mk

EXTRA_INCVPATH+= $(EXTRA_INCVPATH_$(SECTION)) \
	         $(PRODUCT_ROOT)/../../install/usr/include
INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../install
USE_INSTALL_ROOT=1			 
