ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)
include $(MKFILES_ROOT)/qmacros.mk

NAME := spi-rcar
USEFILE=$(PROJECT_ROOT)/$(NAME).use
EXTRA_SILENT_VARIANTS+=$(SECTION)
LIBS = drvr
LIBS += dma-rcar-sysdmac drvr smmu

define PINFO
PINFO DESCRIPTION=
endef

include $(PROJECT_ROOT)/pinfo.mk
LDVFLAG_dll= -L.

#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk

-include $(PROJECT_ROOT)/roots.mk
