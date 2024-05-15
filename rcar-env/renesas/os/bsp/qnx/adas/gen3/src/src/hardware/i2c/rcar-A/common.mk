ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk
-include $(PROJECT_ROOT)/roots.mk

define PINFO
PINFO DESCRIPTION=RCAR I2C protocol driver
endef

NAME := i2c-$(NAME)
USEFILE = $(PROJECT_ROOT)/Usemsg
INSTALLDIR = sbin
LIBS += i2c-master drvr rcar-cpgS


#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk

