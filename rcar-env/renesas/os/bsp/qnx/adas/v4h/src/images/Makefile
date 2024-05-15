ROOT_DIR := $(notdir $(CURDIR))
ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

HOST_MKIFS   := mkifs
HOST_OBJCOPY := ntoaarch64-objcopy

SUFFIXES := .build .ifs .bin

IPL_VARIANTS=ipl-rcar_gen4-v4h_whitehawk
IPL_PATH=../install/aarch64le/boot/sys

.PHONY: clean v4h 

all: ipl v4h

v4h: ifs-rcar_v4h ifs-rcar_v4h-ipmmu
ipl: $(IPL_VARIANTS:%=%.srec) $(IPL_VARIANTS:%=%.bin)

ifs-%: %.build
	$(HOST_MKIFS) -vvv -r../install $(MKIFSFLAGS) $^ $@.bin
	$(HOST_OBJCOPY) --image-base=0x50100000 -Ibinary -Osrec --srec-forceS3 --change-addresses=0x50100000 $@.bin $@.srec

%.srec : $(IPL_PATH)/%
	$(HOST_OBJCOPY) -Ielf64-littleaarch64 -Osrec --srec-forceS3 $< $@

%.bin : $(IPL_PATH)/%
	$(HOST_OBJCOPY) -Ielf64-littleaarch64 -Obinary $< $@

clean:
	$(RM_HOST) *.bin *.srec
