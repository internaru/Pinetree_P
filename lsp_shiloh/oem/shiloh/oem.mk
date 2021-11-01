# ------------------------------------------------------------------------------
# OEM specific makefile.
# This makefile defines the directories built in the OEM subtree.
# It is included by the project.mk file.
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# always - the following directories are always built
# ------------------------------------------------------------------------------
#LOCAL_TARGETS += $(PRODUCT_PATH)/hardware/devices
ifdef BOARD
LOCAL_TARGETS += $(PRODUCT_PATH)/hardware/board/$(BOARD)
endif

# ------------------------------------------------------------------------------
# acl 
# ------------------------------------------------------------------------------
ifdef HAVE_ACL
LOCAL_TARGETS += $(OEM_PATH)/common/parsers/acl
endif

# ------------------------------------------------------------------------------
# acl 
# ------------------------------------------------------------------------------
ifdef HAVE_DEBUG
LOCAL_TARGETS += $(PRODUCT_PATH)/debug
endif

# ------------------------------------------------------------------------------
# http module
# ------------------------------------------------------------------------------
ifdef HAVE_HTTP
LOCAL_TARGETS += $(PRODUCT_PATH)/http
endif

# ------------------------------------------------------------------------------
# Internal page manager
# ------------------------------------------------------------------------------
ifdef HAVE_INTERNAL_PAGE_MGR
LOCAL_TARGETS += $(PRODUCT_PATH)/internal_page
endif

# ------------------------------------------------------------------------------
# memmgr module malloc required 
# ------------------------------------------------------------------------------
ifdef HAVE_MEMMGR
LOCAL_TARGETS += $(PRODUCT_PATH)/memmgr
endif

# ------------------------------------------------------------------------------
# network interface
# ------------------------------------------------------------------------------
ifdef HAVE_NETWORK

LOCAL_TARGETS += $(OEM_PATH)/common/network/interface

ifdef HAVE_SLP
    LOCAL_TARGETS += $(OEM_PATH)/common/network/apps/slp
endif

ifdef HAVE_SNMP
    LOCAL_TARGETS += $(OEM_PATH)/common/network/apps/snmp
endif

ifdef HAVE_GCPP
    LOCAL_TARGETS += $(OEM_PATH)/common/network/apps/gcpp
endif

endif

# ------------------------------------------------------------------------------
# nvram module
# ------------------------------------------------------------------------------
ifdef HAVE_NVRAM
LOCAL_TARGETS += $(PRODUCT_PATH)/nvram

ifdef HAVE_NVRAM_FILE
LIBPATH += oem/$(OEM_DIR)/$(PRODUCT_DIR)/libs
LIBS += sqlite3
endif

endif

# ------------------------------------------------------------------------------
# paper module
# ------------------------------------------------------------------------------
ifdef HAVE_PAPER
LOCAL_TARGETS += $(PRODUCT_PATH)/paper
endif

# ------------------------------------------------------------------------------
# pjl 
# ------------------------------------------------------------------------------
ifdef HAVE_PJL
LOCAL_TARGETS += $(OEM_PATH)/common/parsers/zpjl
endif

# ------------------------------------------------------------------------------
# zj
# ------------------------------------------------------------------------------
ifdef HAVE_ZJ
LOCAL_TARGETS += $(OEM_PATH)/common/parsers/zj
endif

# ------------------------------------------------------------------------------
# platform module
# ------------------------------------------------------------------------------
ifdef HAVE_PLATFORM
LOCAL_TARGETS += $(PRODUCT_PATH)/platform
endif

# ------------------------------------------------------------------------------
# print engine block 
# ------------------------------------------------------------------------------
ifdef PRINT_MECH
LOCAL_TARGETS += $(OEM_PATH)/common/print_mech
endif

#-------------------------------------------------------------------------------
# print module 
#-------------------------------------------------------------------------------
ifdef HAVE_PRINT_SUPPORT
LOCAL_TARGETS += $(OEM_PATH)/common/print/core
LOCAL_TARGETS += $(OEM_PATH)/common/print/mode
endif

# ------------------------------------------------------------------------------
# scan
# ------------------------------------------------------------------------------
ifdef HAVE_SCAN_SUPPORT
LOCAL_TARGETS += $(OEM_PATH)/common/scan
endif

# ------------------------------------------------------------------------------
# statistics 
# ------------------------------------------------------------------------------
ifdef HAVE_STATISTICS
LOCAL_TARGETS += $(OEM_PATH)/common/statistics
endif

#-------------------------------------------------------------------------------
# string mgr module 
#-------------------------------------------------------------------------------
ifdef HAVE_STRING_MGR
LOCAL_TARGETS += $(PRODUCT_PATH)/string_mgr
endif

#-------------------------------------------------------------------------------
# system module 
#-------------------------------------------------------------------------------
ifdef HAVE_SYSTEM
LOCAL_TARGETS += $(PRODUCT_PATH)/system
endif

#-------------------------------------------------------------------------------
# color pipe
#-------------------------------------------------------------------------------
ifdef HAVE_PIPP
LOCAL_TARGETS += $(PRODUCT_PATH)/color_pipe
endif

#-------------------------------------------------------------------------------
# mono pipe
#-------------------------------------------------------------------------------
ifdef HAVE_PIPR
LOCAL_TARGETS += $(PRODUCT_PATH)/mono_pipe
endif

#-------------------------------------------------------------------------------
# pip 
#-------------------------------------------------------------------------------
ifdef HAVE_PIP
LOCAL_TARGETS += $(OEM_PATH)/common/print/pip
endif

# ------------------------------------------------------------------------------
# oid module
# ------------------------------------------------------------------------------
ifdef HAVE_OID
LOCAL_TARGETS += $(OEM_PATH)/common/oid
endif

ifdef HAVE_DCMOTOR_OL_OEM
LOCAL_TARGETS += $(OEM_PATH)/common/devices/dcmotor_ol_oem
MACROS += HAVE_DCMOTOR
endif

#-------------------------------------------------------------------------------
# imagelog module
#-------------------------------------------------------------------------------
ifdef HAVE_IMAGELOG
LOCAL_TARGETS += $(OEM_PATH)/common/imagelog
endif

#-------------------------------------------------------------------------------
# scanbox module
#-------------------------------------------------------------------------------
ifdef HAVE_SCANBOX
LOCAL_TARGETS += $(OEM_PATH)/common/scanbox
endif
