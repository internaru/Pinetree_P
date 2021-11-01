# ============================================================================
# Copyright (c) 2004-2010 Marvell International, Ltd. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

# Include the .IGNORE target to tell make to continue past errors
#.IGNORE :

# If the target fails due to an error, delete the partial output
.DELETE_ON_ERROR:

buildconfig: 
	@echo -----------------------------------------------------
	@echo $(BLDCFGSTR)
	@echo -----------------------------------------------------

# The "flags" method of dependency checking compares a set of flags used for
# compilation (-I, -D, -U, -O, --xyz, etc.). The current flags are compared with
# the settings from the last compilation. If they differ then re-compilation is
# triggerred. The "files" method uses a list of makefiles as additional
# dependencies. This is faster (especially on systems such as Cygwin) but less
# reliable since compilation settings can change even when makefiles do not.
# The "none" method skips dependency checking altogether. This is, of course,
# the very quickest for single builds (such as build server builds) but is not
# a realistic method for developers to use.
 DEPENDENCY_METHOD ?= flags
#DEPENDENCY_METHOD ?= files
#DEPENDENCY_METHOD ?= none

_MAKEFILE_files = makefile $(addprefix $(BUILD_ROOT)/, $(MKFILES))
_SETTINGS_flags = $(TARGETDIR)/%.settings
_MAKEFILE_DEP = $(_MAKEFILE_$(DEPENDENCY_METHOD))
_SETTINGS_DEP = $(_SETTINGS_$(DEPENDENCY_METHOD))

# This variable should be a concatenation of ANYTHING that could change the
# outcome of compilation (C or C++), assembly. This includes -I, -D, and any
# options to the compiler, preprocessor, or assembler
_SETTINGS  = "I:$(INCFLAGS)"
_SETTINGS += "M:$(foreach MACRO,$(MACROS),-D$(MACRO))"
_SETTINGS += "P:$(INCPATH_MACROS)"
_SETTINGS += "A:$(ASPARAM)"
_SETTINGS += "CC:$(CCPARAM)"
_SETTINGS += "CPP:$(CPPPARAM)"
_SETTINGS += "XTRA:$(EXTRA_CFLAGS_$(*))"

.PHONY : update_settings_always
.PRECIOUS : $(TARGETDIR)/%.settings
$(TARGETDIR)/%.settings : update_settings_always
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@echo "$(_SETTINGS)" | $(CMP) -s $@ - || $(ECHO) "$(_SETTINGS)" > $@

# Create object files from .S files
$(TARGETDIR)/%.o: %.S $(_SETTINGS_DEP) $(_MAKEFILE_DEP)
	@echo Assembling $*:
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@$(CC) -E $(INCFLAGS) $(INCPATH_MACROS) $(CURDIR)/$< -o $(TARGETDIR)/$*.s
	@$(AS) $(ASPARAM) $(TARGETDIR)/$*.s -o ./$@
	@$(RM) $(TARGETDIR)/$*.s

# Create object files from .s files
$(TARGETDIR)/%.o: %.s $(_SETTINGS_DEP) $(_MAKEFILE_DEP)
	@echo Compiling $*:
	@$(RM) $(TARGETDIR)/$@ 2>/dev/null
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@$(AS) $(ASPARAM) $(CURDIR)/$< -o $@
#	@echo done.

# Create object files from .c files
$(TARGETDIR)/%.o: %.c $(_SETTINGS_DEP) $(_MAKEFILE_DEP)
	@echo Compiling $*:
	@$(RM) $(TARGETDIR)/$@ 2>/dev/null
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	$(COVCC) $(CCPARAM) -c $(INCFLAGS) $(foreach MACRO,$(MACROS),-D$(MACRO)) $(INCPATH_MACROS) $(CURDIR)/$< -o $@

# Create object files from .cpp (C++) files
$(TARGETDIR)/%.o: %.cpp $(_SETTINGS_DEP) $(_MAKEFILE_DEP)
	@echo Compiling $*:
	@$(RM) $(TARGETDIR)/$@ 2>/dev/null
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@$(COVCPP) $(CPPPARAM) -c $(INCFLAGS) $(foreach MACRO,$(MACROS),-D$(MACRO)) $(INCPATH_MACROS) $(CURDIR)/$< -o $@
#	@echo done.

# --------------------------------
# Dependency File Rule Definitions
# --------------------------------

$(TARGETDIR)/%.d: %.c $(_MAKEFILE_DEP)
	@echo Creating dependency for $*:
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@$(CC) $(DEPFLAGS) -MM -MP -MF $(TARGETDIR)/$*.d -MT $(TARGETDIR)/$*.o -MT $(TARGETDIR)/$*.d  \
		$(INCFLAGS) $(foreach MACRO,$(MACROS),-D$(MACRO)) $(INCPATH_MACROS) $(CURDIR)/$<

# Create dependency files for .S files
$(TARGETDIR)/%.d : %.S $(_MAKEFILE_DEP)
	@echo Creating dependency for $*:
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@$(CC) -MM -MP -MF $(TARGETDIR)/$*.d -MT $(TARGETDIR)/$*.o -MT $(TARGETDIR)/$*.d \
		$(INCFLAGS) $(foreach MACRO,$(MACROS),-D$(MACRO)) $(INCPATH_MACROS) $(CURDIR)/$< 

# Create dependency files for .cpp (C++) files
$(TARGETDIR)/%.d : %.cpp $(_MAKEFILE_DEP)
	@echo Creating dependency for $*:
	@test -d $(TARGETDIR) || $(MKPATH) $(TARGETDIR)
	@$(CPP) -MM -MP -MF $(TARGETDIR)/$*.d -MT $(TARGETDIR)/$*.o -MT $(TARGETDIR)/$*.d \
		$(INCFLAGS) $(foreach MACRO,$(MACROS),-D$(MACRO)) $(INCPATH_MACROS) $(CURDIR)/$< 

# --------------------------------
# Archive File Rule Definitions
# --------------------------------
$(MODULE_LIB_DIR)/%.a: makefile $(OBJECTS)
	@echo +++++++++++++++++++++++++++++++++++++++
	@echo Archiving $(MODULE_LIB_TARGET)...
	@test -d $(MODULE_LIB_DIR) || $(MKPATH) $(MODULE_LIB_DIR)
	@-$(RM) $@
	@$(AR) -rs \
               $(MODULE_LIB_TARGET) \
	       $(OBJECTS)
	@echo Done!
	@echo +++++++++++++++++++++++++++++++++++++++

# ---------------------------------------------------------------------------
# I N C L U D E  D E P E N D E N C Y  F I L E S 
# ---------------------------------------------------------------------------

# What object files could require dependency files?
_OBJFILE_LIST=$(OBJECT_FILES) $(LIB_OBJECT_FILES)

# Convert that list to have a directory path and a .d suffix
_DFILE_LIST=$(addprefix $(TARGETDIR)/,$(_OBJFILE_LIST:.o=.d))

# Filter _DFILE_LIST if DEPENDENCY_METHOD=none
_DFILES=$(if $(filter-out none,$(DEPENDENCY_METHOD)),$(_DFILE_LIST))

# Filter _DFILES if MAKECMDGOALS={clean, install, headers}
DFILES=$(if $(filter-out clean install headers,$(MAKECMDGOALS)),$(_DFILES))

.PHONY : __dummy__
__dummy__ :

build: $(DFILES) __dummy__

-include $(DFILES) __no_file_matches_this_name

