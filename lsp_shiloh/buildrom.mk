#******************************************************************************
#* Copyright (c) 2011-2012  Marvell International, Ltd. All Rights Reserved
#*
#*                         Marvell Confidential
#*****************************************************************************/

REAL_TOOLS_PATH = $(realpath $(TOOLS_PATH))
REAL_OEM_TOOLS_PATH = $(realpath $(OEM_TOOLS_PATH))
CONFIG_TABLE = $(REAL_OEM_TOOLS_PATH)/nv_image/$(NV_IMG).nvimg
CONFIG_INI   = $(REAL_OEM_TOOLS_PATH)/nv_image/$(NV_IMG).ini
# a config table version of 1 is the default since that is all we had until 
# recently (2011).  If your ASIC rev requires version 2 (or something else)
# then you should define the CONFIG_TABLE_VERSION_for_ASIC_REV_$(ASIC_REV)
# variable appropriately
CONFIG_TABLE_VERSION=$(firstword $(CONFIG_TABLE_VERSION_for_ASIC_REV_$(ASIC_REV)) 1)
# OEM should override at product makefile
ADDPID_USBID := 0x1286

MAKEIMAGE_BLKS ?= 256
MAKEIMAGE_BLKSIZE ?= 0x10000

ZBOOT_MK  = $(REAL_TOOLS_PATH)/makeimage/zboot/makefile.zboot
ZBOOT_ELF = $(REAL_TOOLS_PATH)/makeimage/zboot/output_$(ASIC_REV)/zboot.elf
MIN_ELF   = $(BUILD_ROOT)/$(MIN_PRODUCT)-release_stripped.elf
APP_ELF   = $(BUILD_ROOT)/$(APPTARGET)_nofiles_stripped.elf

APP_BIN   = $(BUILD_ROOT)/$(APPTARGET).application.bin
FULL_BIN  = $(BUILD_ROOT)/$(APPTARGET).manufacturing-one-time.bin
ZBOOT_BIN = $(BUILD_ROOT)/$(APPTARGET).zboot-upgrade.bin

BIN_FILES = $(APP_BIN) $(FULL_BIN) $(ZBOOT_BIN)

.PHONY : buildroms cleanroms alwaysbuild

buildroms : $(APP_BIN) $(FULL_BIN) $(ZBOOT_BIN)

alwaysbuild :

# We do NOT want to recursively clean the min product
cleanrom.$(MIN_PRODUCT)-release :
	@true

# We do want to clean the min product when we are asked to clean anything else 
cleanrom.% :
	@$(MAKE) -s PRODUCT=$(MIN_PRODUCT) TYPE=release clean

cleanroms : cleanrom.$(PRODUCT)-$(TYPE)
	@-$(RM) $(REAL_TOOLS_PATH)/makeimage/$(NV_IMG)/*.bin 
	@-$(RM) $(REAL_TOOLS_PATH)/nv_image/$(NV_IMG).nvimg
	@-$(MAKE) -sf $(ZBOOT_MK) clean

# Make sure zboot, for a particualr ASIC_REV, is up to date
$(ZBOOT_ELF) : alwaysbuild
	@echo "Building zboot"
	@$(MAKE) -sf $(ZBOOT_MK) ASIC_REV=$(ASIC_REV) CPU_TYPE=$(CPU_TYPE) ERROR_ON_WARNING=$(ERROR_ON_WARNING) ASIC=$(ASIC)

# Make sure min is up to date
$(MIN_ELF) : alwaysbuild
	@echo "Building min"
	@$(MAKE) -s PRODUCT=$(MIN_PRODUCT) TYPE=release $(MIN_PRODUCT)-release_stripped.elf

# Make sure the config table for this NV_IMG is up to date
$(CONFIG_TABLE): $(CONFIG_INI)
	@echo Creating $(NV_IMG) config table:
	@-cd $(REAL_TOOLS_PATH)/nv_image/ &&\
         perl ./nvimage.pl -r $(CONFIG_TABLE_VERSION) $(CONFIG_INI)

_exe := $(shell uname -a | grep -iq linux || echo .exe)
MAKEIMAGE_EXEC=$(REAL_TOOLS_PATH)/makeimage/makeimage$(_exe)

$(FULL_BIN) : $(CONFIG_TABLE) $(APP_ELF) $(ZBOOT_ELF)
	$(MAKEIMAGE_EXEC) -o $@ -blks $(MAKEIMAGE_BLKS) -blksize $(MAKEIMAGE_BLKSIZE) -c $(word 1,$+) -a $(word 2,$+) -z $(word 3,$+) -lzg 1 -full 1

$(ZBOOT_BIN) : $(CONFIG_TABLE) $(APP_ELF) $(ZBOOT_ELF)
	$(MAKEIMAGE_EXEC) -o $@ -blks $(MAKEIMAGE_BLKS) -blksize $(MAKEIMAGE_BLKSIZE) -c $(word 1,$+) -a $(word 2,$+) -z $(word 3,$+) -lzg 1

$(APP_BIN) : $(APP_ELF) $(BUILTIN_ROMFILES_DAT)
	$(MAKEIMAGE_EXEC) -o $@ -blks $(MAKEIMAGE_BLKS) -blksize $(MAKEIMAGE_BLKSIZE) -a $(word 1,$+) -f $(word 2,$+) -lzg 1
	@perl $(REAL_TOOLS_PATH)/addpid/addpid.pl -i FAILSAFE -u \"$(ADDPID_USBID)\" $@
