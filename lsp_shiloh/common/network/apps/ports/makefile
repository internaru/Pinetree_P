# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS =  -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) 
INCFLAGS += -I$(PRJROOT)/sysapps/include/
INSTALL_SRC = solution.h commonutils.h
INSTALL_SRC_DIR  = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
SOURCE += commonutils.c send-email.c ftpupload.c webdavupload.c smbupload.c solution.c emailvalidate.c urlencode.c
ifdef HAVE_IFAX
SOURCE += iFaxMailReceiver.c
ifdef HAVE_POP
SOURCE += pop.c
endif
ifdef HAVE_IMAP
SOURCE += imap.c
endif
endif
include $(BUILD_ROOT)/stdtargets.mk
