include ../../../common.mk

# Override the linker file path, to point to the board specific file
LDF_    := $(patsubst %$(SECTION).lnk, %v4h_whitehawk/$(SECTION).lnk, $(LDF_))
LDF_qcc := $(patsubst %$(SECTION).lnk, %v4h_whitehawk/$(SECTION).lnk, $(LDF_qcc))
