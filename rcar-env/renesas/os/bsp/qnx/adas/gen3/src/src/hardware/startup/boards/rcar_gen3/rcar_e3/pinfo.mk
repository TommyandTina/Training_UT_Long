define PINFO
PINFO DESCRIPTION=R-Car E3 Cortex-A53 startup
endef
NAME=startup-rcar_e3
CCFLAGS += -DPMIC_ROHM_BD9571
LIBS += cpg-support sysc-support
