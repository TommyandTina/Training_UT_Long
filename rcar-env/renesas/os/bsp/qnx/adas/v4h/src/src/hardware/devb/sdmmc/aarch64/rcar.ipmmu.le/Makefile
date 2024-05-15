include ../../extra.mk
include ../../../common.mk

ifneq ("$(strip $(foreach dir,$(INCVPATH),$(wildcard $(dir)/hw/rcar_cpg.h)))","")
$(info Building with R-Car CPG Resource Manager Support)
LIBS += rcar-cpg rcar-ipmmu
endif
