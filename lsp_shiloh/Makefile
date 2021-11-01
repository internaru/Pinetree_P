# ============================================================================
# Copyright (c) 2006-2013 Marvell International, Ltd. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

# This is a recursive make system.  The entire project can
# be built at once using this make file, or individual modules can be made
# using their own make files.  However, in order to make an individual module
# the required header files from other modules must already exist in the
# .include folder.
#
# Example usage: `make -j8 MFLAGS=-w -f Makefile`
#
# Important make files:
# project.mk    - Defines which modules will get built
# buildenv.mk   - Build environment variables
# stdtargets.mk - A standard set of rules used to build modules
# buildrules.mk - Rules for bulding dependency and object files
# makefile      - A make file residing within each module to setup the
#                 sources and include paths required to build it.  Can
#                 be ran alone to build just that module or is ran
#                 recursively by this make file.

# Include master environment and project specific options
export BUILD_ROOT := $(CURDIR)
include project.mk

# Some users may want to rebuild when makefiles change instead of when make
# variables or compilation settings change. For these users this is a list of
# makefiles to watch for changes.
export MKFILES += project.mk
export MKFILES += oem/$(OEM_PARSED)/build/project_$(PRODUCT).mk
export MKFILES += buildenv.mk
export MKFILES += stdtargets.mk
export MKFILES += buildrules.mk
export MKFILES += Makefile

# Linker parameters, use a linker command file (--script), enable warnings for
# global variables with the same name (--warn-common), configure for endian
# target ($(ENDIAN_AS_PARAM)), and output a map file (-Map)
LDPARAM = -Wl,--warn-common $(ENDIAN_CC_PARAM) -Wl,-Map -Wl,$(TARGET:.elf=.map)
ifeq ($(TYPE), debug)
#allow the backtrace() library routine to provide full symbol info -jrs
LDPARAM += -rdynamic
endif

OBJECT_DIRS := $(foreach TARGET_DIR,$(LOCAL_TARGETS), $(addsuffix /$(TARGETDIR), $(TARGET_DIR)))

# Generate list of directories to invoke the install rule on.
# We always install both the local target dir headers and the library headers.
INSTALL_TARGETS = $(addsuffix -install, $(LOCAL_TARGETS))
INSTALL_TARGETS += $(addsuffix -install, $(LIB_TARGETS))

# Generate list of directories to clean.  Note that cleaning of libs is intentionally
# a special case.  Since libs rarely change and take a relatively long time to build
# we have special clean rules that allow us to clean the base files without needlessly
# cleaning things like to OS and network libraries.
CLEAN_TARGETS = $(addsuffix -clean, $(LOCAL_TARGETS))
LIB_CLEAN_TARGETS = $(addsuffix -clean, $(LIB_TARGETS))
CLEAND_TARGETS = $(addsuffix -cleand, $(LOCAL_TARGETS))

#MODIFIED_C := $(BUILD_ROOT)/libmodified_c.a
#STANDARD_C := $(STANDARD_LIBRARY_LOCATION)/libc.a

.PHONY: all buildconfig localsubdirs disasm clean cleand cleaning cleanrootinc cleanrootfiles cleanbase cleanlibs bin install installing help $(LOCAL_TARGETS) $(LIB_TARGETS) $(CLEAN_TARGETS) $(LIB_CLEAN_TARGETS) $(CLEAND_TARGETS) $(INSTALL_TARGETS) $(APPTARGET)

# Meaningful default targets are probably things like elfonly, buildroms, etc.
DEFAULT_MAKE_TGT ?= elfonly

all: $(DEFAULT_MAKE_TGT)

include $(BUILD_ROOT)/romfiles.mk
include $(BUILD_ROOT)/buildrom.mk

$(APPTARGET): $(APPTARGET)_stripped.elf

#$(MODIFIED_C): $(STANDARD_C)
#	@echo Deleting symbols from libc.a
#	$(CP) $(STANDARD_C) $(MODIFIED_C)
#	$(AR) d $(MODIFIED_C) lib_a-mallocr.o lib_a-reallocr.o

%_stripped.elf : %.elf
	@$(ELFSTRIP) -S --strip-unneeded -R.comment -o $@ $<

fw_version.o: install localsubdirs
	@echo ------------------------------------------
	@echo Creating fwversion:
	@perl $(TOOLS_PATH)/makefwversion/makefwversion.pl $(COMPUTERNAME) PROD=$(PRODUCT) $(TYPE) $(BUILD_ID)
	@$(CC) $(CCPARAM) -Wno-unused-but-set-variable -c -I $(GLOBAL_INCLUDE) $(foreach MACRO,$(MACROS),-D$(MACRO)) $(INCPATH_MACROS) ./fw_version.c -o ./fw_version.o
	@echo done.

define apptarget_rules
	@echo Linking $(basename $@):
	@$(LD) $(LDPARAM) -o $@ $(foreach d, $(OBJECT_DIRS), $(wildcard $(d)/*.o)) $(filter %.o,$^)\
	   $(foreach l,$(LIBS),-l $(l)) $(foreach l,$(LIBPATH),-L $(l))
	@echo done.
	@echo ------------------------------------------
endef

$(BUILD_ROOT)/$(APPTARGET)_nofiles.elf: fw_version.o $(NOFILES_ROMFILES_OBJ) $(MODIFIED_C)
	$(call apptarget_rules)

$(APPTARGET).elf: fw_version.o $(BUILTIN_ROMFILES_OBJ) $(MODIFIED_C)
	$(call apptarget_rules)

elfonly : $(APPTARGET)_stripped.elf
	@echo $< is ready.

$(INSTALL_TARGETS):
	@$(MAKE) -s -C $(subst -install,,$@) install

installcommonheader:
	@echo "Installing common headers..."
	@-cp $(BUILD_ROOT)/../../common_header/SYS_UI_msg_interface.h ./common/systemservice/include/
	@-cp $(BUILD_ROOT)/../../common_header/SysSharedMemory.h ./common/systemservice/include/

installing:
	@echo "Installing headers..."

install: installcommonheader installing $(INSTALL_TARGETS)

localsubdirs: $(LIB_TARGETS) $(LOCAL_TARGETS) 

$(LOCAL_TARGETS): install
	@if test -d $@; then $(MAKE) $(MFLAGS) -s -C $@ build; fi

$(LIB_TARGETS): install
	@if test -d $@; then $(MAKE) $(MFLAGS) -s -C $@ lib_no_headers; fi

# create binary file suitable for burning in the boot spi flash
bin: $(TARGET:.elf=_stripped.elf)
	@echo Generating bin from: $<
	$(OEM_TOOLS_PATH)/imagebuilder.pl -o $(TARGET:.elf=.bin) -c 0 $(OEM_TOOLS_PATH)/$(BOOTSPI_INI_FILE)

# create nand pkg file suitable for burning in the nand flash
pkg: $(TARGET:.elf=_stripped.elf)
	@echo Generating pkg from: $<
#	@$(OEM_TOOLS_PATH)/makenandrom.pl -i $(PRODUCTIDSTR) -u $(USBIDSTR) -n $(TARGET:.elf=.pkg) $<
	@$(OEM_TOOLS_PATH)/makenandrom.pl -i $(PRODUCTIDSTR) -n $(TARGET:.elf=.pkg) $<
	@echo PKG done.
	@echo ------------------------------------------

# create xdb BD file
xdb: $(APPTARGET).elf $(TARGET:.elf=_stripped.elf)
	@echo Generating XDB BD from: $<  
	@if test -s $(OEM_TOOLS_PATH)/dwarf2bd ; then \
	$(OEM_TOOLS_PATH)/dwarf2bd -nc $(APPTARGET).elf $(APPTARGET).elf.BD; \
	echo xdb BD done.  ; else \
	echo "**** missing dwarf2bd"; \
	echo "**** dwarf2bd is found in C:\\Program Files\\Marvell\\XDBx.y\\linux_debugging\\bin"; \
	echo "**** put dwarf2bd in your $(OEM_TOOLS_PATH)" ; fi
	@echo ------------------------------------------ 

# provide optional targets useful for specific projects
ifdef OEM_CUSTOM_TARGETS
include $(OEM_CUSTOM_TARGETS)
endif

disasm: 
	@echo Making mixed source/assembly listing.
	$(OBJDUMP) -SDx $(TARGET) >$(TARGET:.elf=.txt)


help:	
	@echo LOCAL TARGETS: $(LOCAL_TARGETS)
	@echo *   
	@echo BUILD ROOT IS: $(BUILD_ROOT)
	@echo MODULES_INC_PATH: $(MODULES_INC_PATH)
	@echo CCPARAM: $(CCPARAM)
	@echo ASPARAM: $(ASPARAM)
	@echo INCFLAGS: $(INCFLAGS)
	@echo MACROS: $(MACROS)
	@echo INCPATH_MACROS: $(INCPATH_MACROS)

cleaning:
	@echo Cleaning...

$(CLEAN_TARGETS):
	@$(MAKE) -s -C $(subst -clean,,$@) clean

$(LIB_CLEAN_TARGETS):
	@$(MAKE) -s -C $(subst -clean,,$@) clean

cleanrootinc:
	$(RM_R) $(GLOBAL_INCLUDE)

cleanrootfiles:
	$(RM) $(TARGET)
	$(RM) $(TARGET:.elf=.bin)
	$(RM) $(TARGET:.elf=.map)
	$(RM) $(APPTARGET)_stripped.elf
	$(RM) fw_version.c
	$(RM) fw_version.o
	$(RM) $(MODIFIED_C)
	@-$(RM_R) $(BUILD_ROOT)/$(APPTARGET).romfiles

# main clean rule.  cleans everything.
clean:  cleaning $(CLEAN_TARGETS) $(LIB_CLEAN_TARGETS) cleanrootfiles cleanrootinc
	@echo done.

# clean only object files.  Do not clean libraries.
cleanbase: cleaning $(CLEAN_TARGETS) cleanrootfiles cleanrootinc
	@echo done.

# clean only libraries.  do not clean object files. do not clean global include dir
cleanlibs: cleaning $(LIB_CLEAN_TARGETS) cleanrootfiles
	@echo done.

include $(BUILD_ROOT)/buildrules.mk

