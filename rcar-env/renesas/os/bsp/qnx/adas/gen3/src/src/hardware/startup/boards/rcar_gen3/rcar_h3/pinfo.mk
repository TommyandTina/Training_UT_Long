define PINFO
PINFO DESCRIPTION=R-Car H3 Cortex-A57/A53 startup
endef
NAME=startup-rcar_h3
CCFLAGS += -DPMIC_ROHM_BD9571
LIBS += cpg-support sysc-support
