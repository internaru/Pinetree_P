# ============================================================================
# Copyright (c) 2009-2011 Marvell International, Ltd. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================
#
# romfiles.mk
#
# This makefile processes a list of tuples (2 or 3 tuples) called ROMFILES and
# produces makefile rules to produce and maintain a .romfile file for each
# module. The .romfile files will, under some conditions, be stored in the
# linked program's data segment and, under other circumstances, will be stored
# in the flash (or ROM) of the product and not statically linked to the program
#
# Each tuple in the ROMFILES list is formed as shown:
#
# <host file>:<target file>[:<options>]
#
# Note that options are optional (pun intended).
#
# A typical use may look like this:
#
# ROMFILES =
# ROMFILES += logo.jpg:graphics/logo.jpg
# ROMFILES += yellow_line.jpg:graphics/line.jpg
# ROMFILES += src/fw_cert.der:ssldata/fw_cert.der:gzip
# ROMFILES += config/fw_key.der:ssldata/fw_key.der:gzip
#
#   NOTE!!! - File names must match the regexp [a-zA-Z_][a-zA-Z0-9_./]*
#             They will be mangled into valid makefile variable names by
#             replacing .'s with a single _ and /'s by a double _ (__).
#
#   NOTE!!! - The makefile will search for <host file> by looking in the
#             top level directory or in the SRCDIR directory. This is easily
#             expanded. If the file is not in either of these places then the
#             pathname should be provided as part of the <host file>
#
# Later the make process will take all .romfile's from all components and make
# them available, either as compiled data for download or in another form for
# inclusion in the project.
#
# ROMFILES can, theoretically, have more than option. In this case all options
# are listed separated by commas with no space. The available options are:
#
#   gzip - gzip the data and transparently provided uncompressed data to the
#          accessor. Note that this adds CPU and RAM storage overhead.
#

ifneq ($(strip $(ROMFILES)),)

# This part of this file is used when ROMFILES is non-empty as part of module
# build process

# For a provided 2-tuple compute the key as a mangled form of the <target file>
# The mangling converts .'s to _'s and /'s to __'s
define ROMOBJ_KEY
$(subst /,__,$(subst .,_,$(word 2,$(subst :, ,$(1)))))
endef

ROMOBJ_KEYS = $(foreach pair,$(ROMFILES),$(call ROMOBJ_KEY,$(pair)))

# Find the tuple with key=$(1)
define ROMOBJ_TUPLE
$(foreach p,$(ROMFILES),$(if $(filter $(1),$(call ROMOBJ_KEY,$(p))),$(p)))
endef

# Find the value of the $(2)'th element in the tuple with key=$(1)
define ROMOBJ_LOOKUP
$(strip $(word $(2),$(subst :, ,$(strip $(call ROMOBJ_TUPLE,$(1))))))
endef

# Find a source file (in some directory) that matches filename $(1)
define ROMOBJ_FILE
$(firstword $(realpath $(wildcard $(1) $(SRCDIR)/$(1))) not.found[$(1)])
endef

# Find the host file name for the ROMOBJ with key of $(1)
define ROMOBJ_SRC
$(strip $(call ROMOBJ_LOOKUP,$(1),1))
endef

# Find the target file path/name for the ROMOBJ with key of $(1)
define ROMOBJ_TGT
$(strip $(call ROMOBJ_LOOKUP,$(1),2))
endef

# Find the options, if any, for the ROMOBJ with key of $(1)
define ROMOBJ_OPT
$(strip $(call ROMOBJ_LOOKUP,$(1),3))
endef

# Find a specific option ($1) in a comma delimited option list ($2)
COMMA := ,
SPACE := " "
define ROMOBJ_OPTION
$(filter $(1),$(subst $(COMMA),$(SPACE),$(2)))
endef

# Invoked with $(1)=key, $(2)=source file, $(3)=dest path, $(4)=options
define romfile_template

$(TARGETDIR)/$(1).romfile.gz : $(2)
	@command -v gzip >/dev/null || ( echo "gzip not found in PATH"; false )
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@cat $(2) | gzip - > $$@

$(TARGETDIR)/$(1).romfile : $(if $(call ROMOBJ_OPTION,gzip,$(4)), $(TARGETDIR)/$(1).romfile.gz,$(2)) $(BUILD_ROOT)/romfiles.mk
	@echo "Storing ($(notdir $(2)) --> $(3))"
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@(/bin/echo -ne "ROMOBJ: $(1)\n"; \
	  /bin/echo -ne "OPTION: $(4)\n"; \
	  /bin/echo -ne "SOURCE: $(2)\n"; \
	  /bin/echo -ne "LENGTH: `cat $$< | wc -c`\n"; \
	  /bin/echo -ne "TARGET: $(3)\n"; \
          cat $$<) > $$@
ROMOBJECTS += $(TARGETDIR)/$(1).romfile
endef

# Invoke romfile_template for each rom object
$(foreach obj,$(ROMOBJ_KEYS),$(eval $(call romfile_template,$(obj),$(call ROMOBJ_FILE,$(call ROMOBJ_SRC,$(obj))),$(call ROMOBJ_TGT,$(obj)),$(call ROMOBJ_OPT,$(obj)))))

endif



################################################################################
################################################################################
################################################################################


# This following part of this file is used when OBJECT_DIRS is non-empty as
# part of the top level Makefile process
ifneq ($(strip $(OBJECT_DIRS)),)

ROMFILES_DIR=$(BUILD_ROOT)/$(APPTARGET).romfiles

ROMOBJ_FILES = $(foreach d, $(OBJECT_DIRS), $(wildcard $(d)/*.romfile))

$(ROMFILES_DIR)/nofiles.dat : 
	@echo "Building $(notdir $@)..."
	@test -d $(dir $@) || $(MKPATH) $(dir $@)
	@echo -n "" > $@

$(ROMFILES_DIR)/builtin.dat : $(ROMFILES_DIR)/nofiles.dat localsubdirs
	@echo "Building $(notdir $@)..."
	@test -d $(dir $@) || $(MKPATH) $(dir $@)
	@cat $(ROMOBJ_FILES) $< > $@

.PRECIOUS : $(ROMFILES_DIR)/nofiles.c $(ROMFILES_DIR)/builtin.c

$(ROMFILES_DIR)/%.c : $(ROMFILES_DIR)/%.dat
	@echo "Building $(notdir $@)..."
	@test -d $(dir $@) || $(MKPATH) $(dir $@)
	@echo "const unsigned char builtin_romfiles[] = {"                   >$@
	@od -v -t x1 --width=16 < $< |\
          sed -e 's/^[^ ]*//' -e 's/ /0x/g' -e 's/0x../&,/g'                >>$@
	@echo "};"                                                          >>$@
	@echo "unsigned int builtin_romfiles_size=sizeof(builtin_romfiles);">>$@

$(ROMFILES_DIR)/%.o : $(ROMFILES_DIR)/%.c
	@echo "Building $(notdir $@)..."
	@test -d $(dir $@) || $(MKPATH) $(dir $@)
	@$(CC) $(CCPARAM) -c $< -o $@

BUILTIN_ROMFILES_DAT=$(ROMFILES_DIR)/builtin.dat
BUILTIN_ROMFILES_OBJ=$(ROMFILES_DIR)/builtin.o
NOFILES_ROMFILES_OBJ=$(ROMFILES_DIR)/nofiles.o

endif
