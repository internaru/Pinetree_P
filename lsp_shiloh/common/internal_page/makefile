# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


INSTALL_SRC = 
SOURCE =

ifdef HAVE_INTERNAL_PAGE_MGR
INSTALL_SRC += internal_page_api.h renderer.h 

#  objects to build:
SOURCE += agLinkedList.c
#SOURCE += blobber_font_renderer.c
#SOURCE += InternalPage.c
SOURCE += InternalPageMgr.c
#SOURCE += render_rotate.c
#SOURCE += renderer.c
#SOURCE += ufst_font_driver_ip.c
#SOURCE += ufstRenderer.c
ifdef HAVE_FREETYPE
SOURCE += freetype_renderer.c
endif

MACROS += RENDERER
endif # HAVE_INTERNAL_PAGE_MGR

ifdef HAVE_JPDL_INTERNAL_PAGES
INSTALL_SRC += jpdl_internal_page.h
SOURCE += jpdl_internal_page.c
endif # HAVE_JPDL_INTERNAL_PAGES

include $(BUILD_ROOT)/stdtargets.mk

