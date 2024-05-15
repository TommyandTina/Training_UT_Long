ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

define PINFO
PINFO DESCRIPTION=I2C protocol driver
endef

INSTALLDIR = sbin
include $(MKFILES_ROOT)/qmacros.mk

-include $(PROJECT_ROOT)/roots.mk

USEFILE=$(PROJECT_ROOT)/$(SECTION)/$(NAME).use
EXTRA_SRCVPATH = $(EXTRA_SRCVPATH_$(SECTION))
-include $(PROJECT_ROOT)/$(SECTION)/extra.mk

LIBS += drvrS

PRE_SRCVPATH = $(foreach var,$(filter a, $(VARIANTS)),$(CPU_ROOT)/$(subst $(space),.,$(patsubst a,dll,$(filter-out g, $(VARIANTS)))))




#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk

