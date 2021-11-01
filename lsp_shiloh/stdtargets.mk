# ============================================================================
# Copyright (c) 2009-2011 Marvell International, Ltd. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================
#
# stdtargets.mk
#
# Represents a standard set of rules used to build modules that originate from
# the Unity repository.
# 
# Module makefiles should define the following variables and then include
# stdtargets.mk:
#
# NOTE: all the vars should be defined, even if no value is assigned.
#
# SOURCE =            # Local source files to be compiled or assembled
#                       (*.c, *.cpp, or *.S)
# CONFIG_SOURCE =     # Configuration and test files not to be built into
#                       a library given to a customer.
# INSTALL_SRC =       # Header files to be installed (i.e. copied)
# INSTALL_SRC_DIR =   # Include directory containing INSTALL_SRC files
# INSTALL_DEST_DIR =  # Destination directory where the files will be copied
# INCFLAGS =          # Paths of the locations of all required header files
#
# NOTE: The following are optional and not always required and need not
#       be defined.
#
# CONFIG_SRC =     # Config header files to be installed (i.e. copied)
# CONFIG_SRC_DIR = # Config include directory containing CONFIG_SRC files
# OTHER_SRC =      # Misc header files to be installed (i.e. copied)
# OTHER_SRC_DIR =  # Misc include directory containing OTHER_SRC files
# MACROS +=        # Macros to be passed to the compiler
# VPATH :=         # Paths to source folders other than the default "src"
# CLEAN_SRC =      # files to explicitly add to the clean rule.  Typically used to
                   # clean auto-generated source files.

################################################################
# PHONY TARGETS
# A makefile that includes stdtargets.mk must not define *commands* for the
# following phony targets (otherwise they will be overridden by commands for
# the standard targets defined later on):

.PHONY: all build build_base_rule lib install clean cleand clean_base_rule debug headers

all: headers install build

# END PHONY TARGETS
################################################################

###############################################################
# INSTALL_SOURCE
# Rules for installing INSTALL_SRC files (i.e. copied from INSTALL_SRC_DIR
# to INSTALL_DST_DIR).  Also copies CONFIG_SRC and OTHER_SRC header files
# if any have been defined.

ifneq ($(strip $(CONFIG_SRC)),)
CONFIG_FILES := $(foreach CONFIGFILE, $(CONFIG_SRC), $(INSTALL_DEST_DIR)/$(CONFIGFILE))

$(CONFIG_FILES): $(INSTALL_DEST_DIR)/%: $(CONFIG_SRC_DIR)/%
	@test -d $(INSTALL_DEST_DIR) || $(MKPATH) $(INSTALL_DEST_DIR)
	@$(CP) $< $(INSTALL_DEST_DIR)
endif

ifneq ($(strip $(OTHER_SRC)),)
OTHER_FILES := $(foreach OTHERFILE, $(OTHER_SRC), $(INSTALL_DEST_DIR)/$(OTHERFILE))

$(OTHER_FILES): $(INSTALL_DEST_DIR)/%: $(OTHER_SRC_DIR)/%
	@test -d $(INSTALL_DEST_DIR) || $(MKPATH) $(INSTALL_DEST_DIR)
	@$(CP) $< $(INSTALL_DEST_DIR)
endif

ifneq ($(strip $(INSTALL_SRC)),)
INSTALL_FILES := $(foreach INSTALLFILE, $(INSTALL_SRC), $(INSTALL_DEST_DIR)/$(INSTALLFILE))

$(INSTALL_FILES): $(INSTALL_DEST_DIR)/%: $(INSTALL_SRC_DIR)/%
	@test -d $(INSTALL_DEST_DIR) || $(MKPATH) $(INSTALL_DEST_DIR)
	@$(CP) $< $(INSTALL_DEST_DIR)
endif

install: $(INSTALL_FILES) $(CONFIG_FILES) $(OTHER_FILES)

# END INSTALL_SOURCE
###############################################################

include $(BUILD_ROOT)/romfiles.mk

###############################################################
# SOURCE
# Rules for managing local SOURCE files

#initialize
SOURCE_ALL_CHECK =

ifneq ($(strip $(SOURCE)),)
SOURCE_C = $(filter %.c, $(SOURCE))
SOURCE_CPP = $(filter %.cpp, $(SOURCE))
SOURCE_ASM = $(filter %.S, $(SOURCE))
OBJECT_FILES = $(SOURCE_C:.c=.o) $(SOURCE_CPP:.cpp=.o) $(SOURCE_ASM:.S=.o)
OBJECTS = $(foreach O_FILE, $(OBJECT_FILES), $(TARGETDIR)/$(O_FILE))
DFILES = $(foreach D_FILE, $(OBJECT_FILES:.o=.d), $(TARGETDIR)/$(D_FILE))

SOURCE_ALL_CHECK += $(SOURCE_C) $(SOURCE_CPP) $(SOURCE_ASM)
endif

ifneq ($(strip $(CONFIG_SOURCE)),)
LIB_SOURCE_C = $(filter %.c, $(CONFIG_SOURCE))
LIB_OBJECT_FILES = $(LIB_SOURCE_C:.c=.o)
LIB_OBJECTS = $(foreach O_FILE, $(LIB_OBJECT_FILES), $(TARGETDIR)/$(O_FILE))

SOURCE_ALL_CHECK += $(LIB_SOURCE_C)
endif

ifneq ($(strip $(SOURCE_ALL_CHECK) $(ROMOBJECTS)),)
vpath %.c $(SRCDIR)
vpath %.cpp $(SRCDIR)
vpath %.S $(SRCDIR)

ifdef CONFIG_SRC_DIR
vpath %.c $(CONFIG_SRC_DIR)
vpath %.cpp $(CONFIG_SRC_DIR)
vpath %.S $(CONFIG_SRC_DIR)
endif

ifdef OTHER_SRC_DIR
vpath %.c $(OTHER_SRC_DIR)
vpath %.cpp $(OTHER_SRC_DIR)
vpath %.S $(OTHER_SRC_DIR)
endif

vpath %.c $(CFGDIR)
vpath %.c $(TESTDIR)
vpath %.o $(TARGETDIR)
vpath %.d $(TARGETDIR)

export INCPATH_MACROS := $(foreach MACRO,$(MACROS),-D$(MACRO))

build_base_rule: $(OBJECTS) $(LIB_OBJECTS) $(ROMOBJECTS)

#the build_base_rule is a level of abstraction that allows sub-make files
# to override the build rule, but still invoke the master build functionality
# for example as sub-make might do:
# build: build_base_rule
#     @echo PERFORMING EXTRA BUILD STEPS
#     cp file_x ../filerepo
#     etc...
build: build_base_rule

headers:
	@cd $(BUILD_ROOT) && $(MAKE) install

clean_base_rule:
	@-$(RM_R) $(TARGETDIR)
	@$(RM) $(foreach INCLUDE_FILE, $(INSTALL_SRC), $(INSTALL_DEST_DIR)/$(INCLUDE_FILE))
	@if test -d $(CONFIG_SRC_DIR); then $(RM) $(foreach CONFIG_FILE, $(CONFIG_SRC), $(INSTALL_DEST_DIR)/$(CONFIG_FILE)); fi
	@if test -d $(OTHER_SRC_DIR); then $(RM) $(foreach OTHER_FILE, $(OTHER_SRC), $(INSTALL_DEST_DIR)/$(OTHER_FILE)); fi
	@-$(RM) $(foreach CLEAN_FILE, $(CLEAN_SRC), $(CLEAN_FILE))
	if test -d $(MODULE_LIB_DIR); then $(RM_R) $(MODULE_LIB_DIR); fi

#the clean_base_rule is a level of abstraction that allows sub-make files
# to override the build rule, but still invoke the master build functionality
# for example as sub-make might do:
# clean: clean_base_rule
#     @echo PERFORMING EXTRA CLEAN STEPS
#     @-$(RM) *.cmd
clean: clean_base_rule

cleand:
	@-$(RM) $(TARGETDIR)/*.d

debug:
	@echo --- Debug Info --- 
	@echo CCPARAM= $(CCPARAM)
	@echo OBJECTS=$(OBJECTS)
	@echo LIB_OBJECTS=$(LIB_OBJECTS)
	@echo INCFLAGS= $(INCFLAGS)
	@echo VPATH=$(VPATH)
	@echo SOURCE= $(SOURCE)
	@echo CONFIG_SOURCE= $(CONFIG_SOURCE)
	@echo TARGETDIR= $(TARGETDIR)
	@echo INSTALL_SRC_DIR = $(INSTALL_SRC_DIR)
	@echo INSTALL_SRC = $(INSTALL_SRC)
	@echo CONFIG_SRC_DIR = $(CONFIG_SRC_DIR)
	@echo CONFIG_SRC = $(CONFIG_SRC)
	@echo OTHER_SRC_DIR = $(OTHER_SRC_DIR)
	@echo OTHER_SRC = $(OTHER_SRC)
	@echo OTHER_FILES = $(OTHER_FILES)

lib: build headers $(MODULE_LIB_TARGET)

#rule to build without causing a reinvokation of the master makefile to copy
# headers.  When doing a library only build we need to force update of the
# headers as in the lib: rule, but when building the libs as part of a full
# system build there should be no need to force an update of the headers.
lib_no_headers: build $(MODULE_LIB_TARGET)

make_lib: build headers
	@echo +++++++++++++++++++++++++++++++++++++++
	@echo Archiving $(MODULE_LIB_TARGET)...
	@test -d $(MODULE_LIB_DIR) || $(MKPATH) $(MODULE_LIB_DIR)
	@-$(RM) -f $(MODULE_LIB_TARGET)
	@$(AR) $(ARPARAM) \
               $(MODULE_LIB_TARGET) \
	       $(OBJECTS)
	@echo Done!
	@echo +++++++++++++++++++++++++++++++++++++++endif

endif

# END SOURCE
###############################################################

include $(BUILD_ROOT)/buildrules.mk

