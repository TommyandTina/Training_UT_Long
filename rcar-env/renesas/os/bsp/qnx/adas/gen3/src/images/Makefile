ROOT_DIR := $(notdir $(CURDIR))
ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

HOST_MKIFS   := mkifs
HOST_OBJCOPY := ntoaarch64-objcopy

SUFFIXES := .build .ifs .bin


.PHONY: all clean h3 h3sk m3 m3sk e3 d3 m3n v3h v3m v3msk v3u

all: h3 h3sk m3 m3sk e3 d3 m3n v3h v3m v3msk v3u

h3: ifs-rcar_h3
h3sk: ifs-rcar_h3sk
m3: ifs-rcar_m3
m3sk: ifs-rcar_m3sk
e3: ifs-rcar_e3
d3: ifs-rcar_d3
m3n: ifs-rcar_m3n
v3h: ifs-rcar_v3h
v3m: ifs-rcar_v3m
v3msk: ifs-rcar_v3msk
v3u: ifs-rcar_v3u

ifs-%: %.build
	$(HOST_MKIFS) -vvv -r../install $(MKIFSFLAGS) $^ $@.bin

clean:
	$(RM_HOST) *.bin
