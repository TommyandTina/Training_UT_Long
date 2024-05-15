ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include ../../../prodroot_pkt.mk

LIBS = drvrS netdrvrS

NAME = devnp-$(PROJECT)

USEFILE=$(PROJECT_ROOT)/Usemsg

define PINFO
PINFO DESCRIPTION=Renesas R-Car EtherAVB Network driver
endef

ifneq ($(QNX_ALLOW_WERROR), 0)
CCFLAGS += -Werror
endif

#CCFLAGS += -DUSE_IPMMU
LIBS += rcar-ipmmu

#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk


