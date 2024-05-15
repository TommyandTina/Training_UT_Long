ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk

define PINFO
PINFO DESCRIPTION=Renesas R-CAR Gen3 and Gen4 Thermal driver
endef

USEFILE=$(PROJECT_ROOT)/thermal.use

LIBS += drvr

EXTRA_INCVPATH+= $(EXTRA_INCVPATH_$(SECTION)) \
             $(PRODUCT_ROOT)/../../install/usr/include


#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk


-include $(PROJECT_ROOT)/roots.mk
