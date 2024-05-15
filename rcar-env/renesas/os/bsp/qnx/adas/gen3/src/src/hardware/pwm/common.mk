ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qmacros.mk

define PINFO
PINFO DESCRIPTION=Renesas R-Car PWM driver
endef

NAME=pwm-rcar

ifndef USEFILE
USEFILE=$(PROJECT_ROOT)/pwm.use
endif

EXTRA_SILENT_VARIANTS+=$(SECTION)
EXTRA_SRCVPATH = $(EXTRA_SRCVPATH_$(SECTION))

LIBS += rcar-cpgS

#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk


-include $(PROJECT_ROOT)/roots.mk
