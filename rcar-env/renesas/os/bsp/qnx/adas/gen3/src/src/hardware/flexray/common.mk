ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

ifndef CLASS
CLASS=flexray
endif

ifndef NAME
NAME=$(CLASS)-$(SECTION)
endif


ifndef USEFILE
USEFILE=$(PROJECT_ROOT)/flexray.use
endif
EXTRA_SILENT_VARIANTS+=$(SECTION)
EXTRA_SRCVPATH = $(EXTRA_SRCVPATH_$(SECTION))

LIBS += drvr

define PINFO
PINFO DESCRIPTION=flexray driver for R-CarV3U
endef


include $(MKFILES_ROOT)/qmacros.mk
include $(SECTION_ROOT)/pinfo.mk

-include $(PROJECT_ROOT)/roots.mk


#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk
