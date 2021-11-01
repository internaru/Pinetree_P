# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================
 
#Include master environment
BUILD_ROOT = ../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I$(BUILD_ROOT)/common/scan/src\
           -I$(BUILD_ROOT)/common/scan/include\
           -I$(BUILD_ROOT)/common/scan/lsp/apps\
           -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)\
           -I$(BUILD_ROOT)/common/internal_page/include\
           -I$(BUILD_ROOT)/oem/shiloh/common/scan/hpdflib\
           -I$(BUILD_ROOT)/oem/shiloh/common/scan/libjpeg-turbo-1.1.0

# Paths to source folders other than the default "src"
VPATH=$(BUILD_ROOT)/common/scan/src:../config:$(BUILD_ROOT)/common/scan/lsp/apps

INSTALL_SRC = calgraph.h cal.h copyapp_api.h scancont.h scands.h scanhwerr.h\
	scanman.h scanplat.h calstats.h scantask.h scantypes.h scanvars.h\
	demo_scanapp.h scanmsg.h scancap.h

#INSTALL_SRC += scan_to_file_app_api.h
#INSTALL_SRC += scan2jpg_service_api.h

INSTALL_SRC_DIR = ../include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))
#MODULE_LIB_TARGET = libScan-core-$(TYPE).a
#MODULE_LIB_DIR = ./lib

#
#  Start of Scan specific makefile tweaks/tuning
#

# we'll add scan specific macros here
SCAN_MACROS=

# Define SCAN_HIQ to include host-side scan image quality (siqfile) support.
# A siqfile contains settings to manipulate the various settings, tables, etc,
# in the scan path (e.g., PIE LUTs). (HIQ == "Host Image Quality")
SCAN_HIQ=1

# Define SCAN_ICEFILE to include ability to send scan data from host (bypasses
# scan and pic; inject data into PIE)
#SCAN_ICEFILE=1

# Add fakescan, an interface to interact with scanman as if we were scanning
# real data.
#SCAN_FAKESCAN=1

# Uncomment if you don't want use scanman, instead put the fake scanman in.
# This prevents the full  scan subsystem from running at all.  
# This is for debugging - you get to do a fake copyjob or a fake scanjob
#SCAN_FAKESCANMAN=1

# Uncomment to enable the scanalyzer
#SCANALYZER=1

# uncomment to include my test code
#SCAN_TEST_CODE=1

# Uncoment to enable cal graphs.
#SCAN_CALGRAPH=1

# uncomment to add continuous scan test app (used with RFI testing)
# (requires scantasktest.c from the test code)
#SCAN_CONTINUOUS=1

# davep 23-Jul-2008 ; XXX temp to prevent agMessage.h from being included 
SCAN_MACROS+=__AGMESSAGE_H__

#SCAN_MACROS += HAVE_THREADX
#SCAN_MACROS += HAVE_TX_POSIX_2011

#  objects to build:
SCAN_SRC =  scanlog.c scanvars.c\
          lex.yy.c siqnames.c siqdata.c scanvarsiq.c scandbg.c\
          fraction.c scancap.c safeint.c grayline.c scanmech.c\
          safetylock.c scos.c pic_common.c ostools.c \
          mbuffer.c scands.c strtable.c scanapp.c scantask.c taskqueue.c\
          scantools.c scanpower-stub.c scanlib_appspace.c copyapp.c\
          copyappcmd.c copy_to_host.c scan_stubs.c\
          scanplat_linux.c scansen_appspace.c scanlib_msg.c\
          adfsensor_appspace.c scanmem.c notaheap.c\
          scanapp_uiscan.c outfile.c scanapp_dataprocess.c a3mergeLib.c a3mergeMain.c\
		  scanapp_margins.c rollereraseMain.c rollereraseLib.c\
		  scanapp_save_file.c make2pdf.c spdf_encrypt.c spdf_encryptdict.c

ifdef HAVE_IMAGELOG
   SCAN_SRC += scanapp_imagelog.c
endif

ifdef SCAN_FAKESCANMAN
	SCAN_SRC += fake_scanman.c
	SCAN_MACROS += HAVE_SCAN_FAKESCANMAN
else
   SCAN_SRC += scanman.c scanmanobs.c scanman_smirb.c
endif

# Uncomment to override the motor driver with the null driver (no motor motion)
#SCAN_MACROS+=NULL_MECH_DRIVER
SCAN_SRC += scanmech_null.c 

ifeq ($(ASIC), granite)
    # splat into brick wall if we don't know what we're compiling under
    $(if $(findstring $(ASIC_REV),z1z3b0c0),,$(error Unknown ASIC_REV $(ASIC_REV)))

    SCAN_MACROS += GRANITE_ASIC 
    INCFLAGS += -I$(BUILD_ROOT)/common/scan/src/asic/granite

    HAZ_ASIC=1
endif

# If this fails, you need to define an ASIC (as of Nov-2012 we only have
# gemstone or granite support)
ifndef HAZ_ASIC
    $(error no can haz ASIC?)
endif

ifdef HAVE_ACL
#    SCAN_SRC += scanACL.c
endif

# calgraph stuff
ifdef SCAN_CALGRAPH
    SCAN_SRC += calgraph.c calgrapp.c figure.c
    SCAN_MACROS += HAVE_SCAN_CALGAPH
else
#    SCAN_SRC += calgraph_stub.c
endif

# all current platforms are little endian
ifdef CPU_LITTLE_ENDIAN
    SCAN_MACROS += ASP_ON_LITTLE_ENDIAN
endif

ifdef SCAN_CONTINUOUS
    SCAN_SRC += scancont.c scantasktest.c
    SCAN_MACROS += HAVE_SCAN_CONTINUOUS
endif

ifdef SCAN_TEST_CODE
    SCAN_SRC+= scantest.c pictest.c pictest_pc.c scanlibtest.c \
    scantasktest.c pietest.c scandstest.c strtabletest.c\
	  scanvarstest.c icetesttest.c 
    SCAN_MACROS+=HAVE_SCAN_TEST_CODE

    SCAN_SRC += picdma_descrip_test.c
    SCAN_SRC += cisxtest.c

    ifeq ($(ASIC), gemstone)
        SCAN_SRC += pictest_3bdr.c
    endif

    # davep 24-May-2010 ; don't turn this on unless you've added a demo_scanapp
    # to ResourceMap.c ResourceMap.h agRouter.h agResourceMgr.h 
#    SCAN_SRC += demo_scanapp.c
#    SCAN_MACROS += HAVE_DEMO_SCANAPP
endif

ifdef HAVE_NETWORK
#    SCAN_SRC += scannet.c icenet.c pipenet.c
endif

#SCAN_SRC += scan_to_file_app.c
#SCAN_SRC += scan2jpg_service.c

ifdef SCAN_HIQ
    SCAN_SRC += lex.yy.c siqnames.c siqdata.c scanvarsiq.c
    ifdef HAVE_NETWORK
#        SCAN_SRC += siqnet.c
    endif
    SCAN_MACROS += HAVE_SCANHIQ_SUPPORT
endif

ifdef SCAN_ICEFILE
    SCAN_SRC += icefileapp.c
    SCAN_MACROS += HAVE_SCAN_ICEFILE_SUPPORT
endif

ifdef SCANALYZER
    SCAN_SRC += scanalyzer.c
    SCAN_MACROS += HAVE_SCANALYZER_SUPPORT
endif

ifdef SCAN_FAKESCAN
    SCAN_SRC += fakescan.c
    SCAN_MACROS+=HAVE_FAKESCAN_SUPPORT
endif

ifdef HAVE_COPY_TO_HOST
    SCAN_SRC += copy_to_host.c
endif

# davep 20-Sep-2010 ; xxx code merged into trunk, breaking a few things.
# Scan/Copy code needs to work under old regime as well. Eventually the new
# stuff will be merged backward to old code.
# See also scanman.c, copyapp.c
SCAN_MACROS += STRIP_INFO_2010
SCAN_MACROS += CURRENT_RESOURCE_2010

# 
# Configure drivers
#

# FIXME - remove this when SCAN_MECH is defined in oem/shiloh/build/project_shiloh_pinetree_mech.mk
SCAN_MECH=SCANMECH_LINUX_APP

# The top level make file defines the scan mech
ifeq ($(SCAN_MECH), SCANMECH_LINUX_APP)
    # userspace mech driver (stub functions to talk to kernel through sysfs)
    SCAN_SRC+=scanmech_app.c
    SCAN_MACROS += SCAN_MECH_LINUX_APP
    INCFLAGS += -I$(BUILD_ROOT)/oem/shiloh/common/scan/apps
else
    $(error You need some kind of scan mech)
endif

ifeq ($(findstring HAVE_SCANSEN_STUB,$(SCAN_MACROS)),HAVE_SCANSEN_STUB)
    SCAN_SRC+= scansen_stub.c afe_stub.c scanplat_pluto_gemstonez3.c
endif

# davep 27-Jun-2012; sick of having to modify so many projects when I add a new
# file so adding a core scan makefile
#SCANMAKE=$(wildcard $(BUILD_ROOT)/common/scan/src/scancore.mk)
ifneq "" "$(SCANMAKE)"
    include $(SCANMAKE)
endif

vpath %.c ../src

#
#  End of scan code hackery. Return to OBA build boilerplate
#

SOURCE = $(SCAN_SRC)

CONFIG_SOURCE = 

MACROS+=$(SCAN_MACROS)

include $(BUILD_ROOT)/stdtargets.mk
