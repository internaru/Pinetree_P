# ============================================================================
# Copyright (c) 2008-2013 Marvell International, Ltd. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

ifndef TYPE
export TYPE = debug
endif

export COMPILER = gcc

ifndef PRODUCT
$(error No Product Specified!!!)
endif

# substitute underscore for spaces, then grab the first word
# as the OEM name
UNDERSCORE_REPLACE = $(subst _, ,$(1))
# get a space into a variable
SPACE :=
SPACE +=
UNDERSCORE_RESTORE = $(subst $(SPACE),_,$(1))
# split the "PRODUCT" variable up in to space-separated words
PRODUCT_WORDS = $(call UNDERSCORE_REPLACE,$(PRODUCT))
# ATTENTION: OEM cannot contain underscores with this method!
OEM_PARSED = $(firstword $(PRODUCT_WORDS))
# grab the PRODUCT name now by grabbing all words after the substitution and parsing of OEM
TOTAL_WORDS = $(words $(PRODUCT_WORDS))
PRODUCT_PARSED = $(wordlist 2,$(TOTAL_WORDS),$(PRODUCT_WORDS))
PRODUCT_ACTUAL = $(call UNDERSCORE_RESTORE,$(PRODUCT_PARSED))
$(if $(PRODUCT_ACTUAL),,$(error no Product name found))

APPTARGET = $(PRODUCT)-$(TYPE)

ifdef HAVE_CODE_COVERAGE
  ifneq ($(HAVE_CODE_COVERAGE), 0)
    APPTARGET = $(PRODUCT)-$(TYPE)-$(HAVE_CODE_COVERAGE)
  endif
endif

TARGET = $(APPTARGET).elf

# product libraries first then system libraries
export LIBPATH = 
export LIBS =

# initialize the macros define (to be empty at first)
MACROS =

# add modules and common to the local targets so that they will autobuild
# ditto, regarding order independence and no need to initialize
LOCAL_TARGETS = 

#include the project specific configuration makefile
include $(BUILD_ROOT)/oem/$(OEM_PARSED)/build/project_$(PRODUCT).mk


#include the master build environment configuration
include $(BUILD_ROOT)/buildenv.mk

# check the version of the compiler
ifdef CC_EXP_VERSION
  ifneq ($(CC_FULL_VERSION),$(CC_EXP_VERSION))
    $(error compiler version error got $(CC_FULL_VERSION) expected $(CC_EXP_VERSION))
  endif
endif

export TARGETDIR = $(COMPILER)_$(TYPE)_$(PRODUCT)

#use single .include for all project or use a project based .include directory
ifndef GLOBAL_INCLUDE
#export GLOBAL_INCLUDE = .include
export GLOBAL_INCLUDE = .include_$(PRODUCT)
endif

# Product configuration directory.  This defines a product configuration directory
# that is used to customize the product.  It is a substitution for the generic "config"
# sub-directory.  It is used for product specfic customization.  It differs from the BOARD
# define in that the same product may support different "boards" during its life.
# Typically this value is used for upper level product customization as opposed to the 
# low level driver configuration performed by BOARD.
# The default value is set here if not already defined in the build/project_$(PRODUCT).mk
#ifndef PROD_CFG_DIR
#export PROD_CFG_DIR=config
#endif

#Modules directory
export MODULES_PATH = $(BUILD_ROOT)
export MODULES_INC_PATH = $(MODULES_PATH)/$(GLOBAL_INCLUDE)
export MODULES_LIB_PATH = $(MODULES_PATH)/.lib/$(TARGETDIR)
export COMMON_INC_PATH = $(MODULES_PATH)/include
export TOOLS_PATH := $(BUILD_ROOT)/tools

ifdef OEM_DIR
export OEM_PATH = $(BUILD_ROOT)/oem/$(OEM_DIR)
export PRODUCT_PATH = $(OEM_PATH)/$(PRODUCT_DIR)
export OEM_TOOLS_PATH := $(PRODUCT_PATH)/buildtools
endif

# Pass the board on to the compiler
ifdef BOARD
    MACROS += BOARD=$(BOARD)
    MACROS += BOARD_$(BOARD)
endif

ifdef FLOAT_ABI
    MACROS += FLOAT_ABI=$(FLOAT_ABI)
endif

# ------------------------------------------------------------------------------
# ASSUME JTAG
# Debug builds will assume that the user is employing a JTAG device to debug
# the produced elf files. This assumption may not be valid and may, therefore,
# be overridden. Release builds will assume that a JTAG device is not being
# employed. Once again, this may be overridden on a case by case basis.
# The JTAG assumption will control certain device behavior such as reset on
# assert or not.
# ------------------------------------------------------------------------------
ifeq ($(TYPE), release)
    ASSUME_JTAG ?= 0
else
    ASSUME_JTAG ?= 1
endif
MACROS += ASSUME_JTAG=$(ASSUME_JTAG)

# ------------------------------------------------------------------------------
# enable posix conversion
# ------------------------------------------------------------------------------
ifdef POSIX_CONVERSION_ENABLED
    MACROS += POSIX_CONVERSION_ENABLED
endif

# ------------------------------------------------------------------------------
# file system module 
# ------------------------------------------------------------------------------
# HAVE_FILESYSTEM is not just set to a 1 in the project specific makefile but is
# also set to the particular file system to use. 
ifdef HAVE_FILESYSTEM
$(if $(RAM_PARTITION_NAME),,$(error RAM_PARTITION_NAME must be set, most people choose: RAM_PARTITION_NAME = tmp))
MACROS += RAM_PARTITION_NAME=\"$(RAM_PARTITION_NAME)\"

ifndef BUILD_SDK_FILESYSTEM
LIB_TARGETS += third_party/filesystem
else
LOCAL_TARGETS += third_party/filesystem
endif
LIBPATH += third_party/filesystem/lib/$(TARGETDIR)
LIBS += $(HAVE_FILESYSTEM)
#LOCAL_TARGETS += third_party/filesystem
MACROS += HAVE_FILESYSTEM

# Determine the type of NAND partition that was defined and set the appropriate macro.
# Easy Install is the only partition.
ifeq ($(NAND_PARTITIONS), SI_only)
MACROS += HAVE_EI_PARTITION
# Easy Install, Fax, and Kinoma partitions are present.
else ifeq ($(NAND_PARTITIONS), SI_fax_Kinoma)
MACROS += HAVE_EI_PARTITION
MACROS += HAVE_FAX_PARTITION
MACROS += HAVE_KINOMA_PARTITION
# Easy Install and Kinoma partitions are present.
else ifeq ($(NAND_PARTITIONS), SI_Kinoma)
MACROS += HAVE_EI_PARTITION
MACROS += HAVE_KINOMA_PARTITION
# Fax and Kinoma partitions are present.
else ifeq ($(NAND_PARTITIONS), Fax_Kinoma)
MACROS += HAVE_FAX_PARTITION
MACROS += HAVE_KINOMA_PARTITION
# Kinoma is the only partition.
else ifeq ($(NAND_PARTITIONS), Kinoma_only)
MACROS += HAVE_KINOMA_PARTITION
# Fax is the only partition.
else ifeq ($(NAND_PARTITIONS), Fax_only)
MACROS += HAVE_FAX_PARTITION
# If no partition was defined default to SI only.
else
MACROS += HAVE_EI_PARTITION
endif

ifeq ($(HAVE_FILESYSTEM), blunk)
ifeq ($(FS_VERSION), 20112)
MACROS += BLUNK_VERSION_2011
endif
endif

endif # HAVE_FILESYSTEM

# ------------------------------------------------------------------------------
# UFS - unified file system
# ------------------------------------------------------------------------------
ifdef HAVE_UFS
LOCAL_TARGETS += common/ufs
MACROS += HAVE_UFS
endif

# ------------------------------------------------------------------------------
# DATAFS - raw data backend for UFS
# ------------------------------------------------------------------------------
ifdef HAVE_DATAFS
ifndef HAVE_UFS
$(error UFS must be included for DATAFS module)
endif
LOCAL_TARGETS += common/datafs
MACROS += HAVE_DATAFS
endif

# ------------------------------------------------------------------------------
# ROMFILES - The ability to map romfiles objects into the datafs
# ------------------------------------------------------------------------------
ifdef HAVE_ROMFILES
ifndef HAVE_DATAFS
$(error DATAFS must be included for ROMFILES module)
endif
LOCAL_TARGETS += common/romfiles
MACROS += HAVE_ROMFILES
endif

# ------------------------------------------------------------------------------
# fax module
# ------------------------------------------------------------------------------
ifeq ($(HAVE_FAX), silabs)
# Pull in Fax Silicon Labs Support
LOCAL_TARGETS += third_party/fax/silabs
LIBPATH += third_party/fax/silabs/lib
LIBS += Fax-$(TYPE) supc++

# Pull in the fax OEM demonstration applications
LOCAL_TARGETS += common/fax/silabs/demo

LOCAL_TARGETS += common/fax/silabs/fax_app

LOCAL_TARGETS += common/fax/silabs/fax_svc

LOCAL_TARGETS += common/fax/silabs/fax_store

LOCAL_TARGETS += common/fax/silabs/fax_pipe

LOCAL_TARGETS += common/fax/silabs/fax_reports

# Fax wants to have a phonebook
LOCAL_TARGETS += common/fax/silabs/fax_phbook

# Pull in the fax image utility
LOCAL_TARGETS += third_party/imgcnvrtr
LIBPATH += third_party/imgcnvrtr/lib
LIBS += imgcnvtrt_le_$(TYPE)

#LIB_TARGETS += third_party/tiff
#LIBPATH += third_party/tiff/lib/$(TARGETDIR)
#LIBS += tiff

MACROS += HAVE_FAX_SUPPORT=silabs
ifeq ($(HAVE_FAX_COLOR), 1)
MACROS += HAVE_FAX_COLOR_SUPPORT
endif
else
ifdef HAVE_FAX
    MACROS += HAVE_FAX_SUPPORT=1
    ifeq ($(HAVE_FAX_COLOR), 1)
      MACROS += HAVE_FAX_COLOR_SUPPORT
    endif
    LIB_TARGETS += third_party/tiff
    LIBPATH += third_party/tiff/lib/$(TARGETDIR)
    LIBS += tiff
endif
endif

#-------------------------------------------------------------------------------
#
# Common build directory definitions.  All directories residing in the
# common modules respository should be defined here, not in the product
# specific project_$(PRODUCT).mk file
#
#-------------------------------------------------------------------------------

## Begin Directory and -DMACRO section



ifdef PROCESSOR_ARM
    MACROS += PROCESSOR_ARM
endif

ifeq ($(CPU_TYPE), ARM_PJ1)
    MACROS += HAVE_CPU_ARM_PJ1
endif

ifeq ($(CPU_TYPE), ARM_PJ4)
    MACROS += HAVE_CPU_ARM_PJ4
endif

ifeq ($(CPU_TYPE), ARM_R4)
    MACROS += HAVE_CPU_ARM_R4
endif

ifdef HAVE_SERENITY
    MACROS += HAVE_SERENITY
endif

ifdef HAVE_DCACHE
    MACROS += ENABLE_DCACHE
endif


#-------------------------------------------------------------------------------
# ASIC specific files.  Typically headers.
#-------------------------------------------------------------------------------
ifdef ASIC

include $(BUILD_ROOT)/asic_rev.mk

LOCAL_TARGETS += common/asic/$(ASIC)
LOCAL_TARGETS += common/asic/$(ASIC)/$(ASIC_REV)
ifneq ($(wildcard common/asic/$(ASIC)/$(ASIC_REV)/config),)
	LOCAL_TARGETS += common/asic/$(ASIC)/$(ASIC_REV)/config
endif

MACROS += ASIC_$(ASIC)
MACROS += ASIC_REV=$(ASIC_REV_NUM)

endif 

# ------------------------------------------------------------------------------
# ThreadX RTOS (moved to the top for dependancy reasons)
# ------------------------------------------------------------------------------
ifdef HAVE_RTOS
ifndef BUILD_SDK_RTOS
LIB_TARGETS += third_party/rtos/$(HAVE_RTOS)
else
LOCAL_TARGETS += third_party/rtos/$(HAVE_RTOS)
endif
LIBPATH += third_party/rtos/$(HAVE_RTOS)/lib/$(TARGETDIR)
MACROS += $(HAVE_RTOS)
LIBS += threadx
endif

# ------------------------------------------------------------------------------
# ThreadX Posix compatibility layer (moved to the top for dependancy reasons)
# ------------------------------------------------------------------------------
ifdef HAVE_POSIX
ifndef HAVE_RTOS
$(error POSIX support requires RTOS Base layer)
endif
ifndef BUILD_SDK_RTOS
LIB_TARGETS += third_party/rtos/$(HAVE_RTOS)/posix
else
LOCAL_TARGETS += third_party/rtos/$(HAVE_RTOS)/posix
endif
LIBPATH += third_party/rtos/$(HAVE_RTOS)/posix/lib/$(TARGETDIR)
LIBS += txposix
MACROS += HAVE_POSIX
endif

# ------------------------------------------------------------------------------
# os shim module for standard functions not included 
# in the toolchain (moved to the top for dependancy reasons)
# ------------------------------------------------------------------------------
ifdef HAVE_OS_SHIM
LOCAL_TARGETS += common/os_shim
MACROS += HAVE_OS_SHIM
endif

#-------------------------------------------------------------------------------
# utilities directory (moved to the top for dependancy reasons)
#-------------------------------------------------------------------------------
ifdef HAVE_UTILS
LOCAL_TARGETS += common/utils
MACROS += HAVE_UTILS
endif

#-------------------------------------------------------------------------------
# common consumables api
#-------------------------------------------------------------------------------
ifdef HAVE_COMMON_CONSUMABLES_API
LOCAL_TARGETS += common/consumables
MACROS += HAVE_COMMON_CONSUMABLES_API
endif

#-------------------------------------------------------------------------------
# oem consumables api
#-------------------------------------------------------------------------------
ifdef HAVE_OEM_CONSUMABLES_API
LOCAL_TARGETS += $(OEM_PATH)/$(HAVE_OEM_CONSUMABLES_API)/consumables
MACROS += HAVE_OEM_CONSUMABLES_API
endif

# ------------------------------------------------------------------------------
# date/time module
# ------------------------------------------------------------------------------
ifdef HAVE_DATE_TIME
LOCAL_TARGETS += common/date_time
endif


# ------------------------------------------------------------------------------
# data access module
# ------------------------------------------------------------------------------
ifdef HAVE_DATA_ACCESS
LOCAL_TARGETS += common/data_access
endif


# ------------------------------------------------------------------------------
# debug module
# ------------------------------------------------------------------------------
ifdef HAVE_DEBUG
LOCAL_TARGETS += common/debug

ifdef HAVE_MEMORY_LOG
MACROS += HAVE_MEMORY_LOG
endif
ifdef HAVE_DEBUG_STACK_SCAN
MACROS += HAVE_DEBUG_STACK_SCAN
endif
ifdef HAVE_DBG_PRINTF
MACROS += HAVE_DBG_PRINTF
endif
ifdef CPU_USAGE_PROFILER
MACROS += CPU_USAGE_PROFILER DISABLE_WFI
MACROS += PROFILE_SCALE_POWER=6
endif
ifdef HAVE_SINDOH_LOG_SYSTEM
MACROS += HAVE_SINDOH_LOG_SYSTEM
endif
endif

ifdef HAVE_DISABLE_WFI
MACROS += DISABLE_WFI
endif

# ------------------------------------------------------------------------------
# delay module
# ------------------------------------------------------------------------------
ifdef HAVE_DELAY
LOCAL_TARGETS += common/delay
MACROS += HAVE_DELAY
endif

# ------------------------------------------------------------------------------
# efuse module
# ------------------------------------------------------------------------------
ifdef HAVE_EFUSE
LOCAL_TARGETS += common/devices/efuse
MACROS += HAVE_EFUSE
endif

# ------------------------------------------------------------------------------
# http module 
# ------------------------------------------------------------------------------
ifdef HAVE_HTTP
LOCAL_TARGETS += common/http
MACROS += HAVE_HTTP
ifdef HAVE_HTTPS
  ifdef HAVE_SSL
    MACROS += HAVE_HTTPS
  else
    $(error SSL is required)
  endif
endif
endif


# ------------------------------------------------------------------------------
# http ews module 
# ------------------------------------------------------------------------------
ifdef HAVE_HTTP
  ifdef HAVE_DEBUG_OVER_HTTP
    ifndef HAVE_DEBUG
      LOCAL_TARGETS += common/debug
    endif
    MACROS += HAVE_DEBUG_OVER_HTTP
  endif
endif


# ------------------------------------------------------------------------------
# include module 
# ------------------------------------------------------------------------------
ifdef HAVE_INCLUDE
LOCAL_TARGETS += common/include
endif


# ------------------------------------------------------------------------------
# init module 
# ------------------------------------------------------------------------------
ifdef HAVE_INIT
LOCAL_TARGETS += common/init
endif


# ------------------------------------------------------------------------------
# jpeg module 
# ------------------------------------------------------------------------------
ifdef HAVE_JPEG_SW
#LOCAL_TARGETS += common/jpeg/
#LOCAL_TARGETS += common/jpeg/sw/epeg
#LOCAL_TARGETS += third_party/ijg
LOCAL_TARGETS += $(OEM_PATH)/common/scan/libjpeg-turbo-1.4.2
LIBPATH += $(OEM_PATH)/common/scan/libjpeg-turbo-1.4.2/lib
LIBS += turbojpeg
LIBS += jpeg
MACROS += HAVE_JPEG
MACROS += HAVE_JPEG_SW
endif

# ------------------------------------------------------------------------------
# json module
# ------------------------------------------------------------------------------
ifdef HAVE_JSON
LOCAL_TARGETS += common/json/
MACROS += HAVE_JSON
endif

ifdef HAVE_JSONRPC
#LOCAL_TARGETS += common/jsonrpc/
LOCAL_TARGETS += oem/shiloh/pinetree/jsonrpc/
MACROS += HAVE_JSONRPC
#LIBPATH += common/jsonrpc/libjsoncpp/
LIBPATH += oem/shiloh/pinetree/jsonrpc/libjsoncpp/
LIBS += json_marvell-4.6.4_libmt 
endif


# ------------------------------------------------------------------------------
# generators module 
# ------------------------------------------------------------------------------
ifdef HAVE_GENERATORS
LOCAL_TARGETS += common/generators
MACROS += HAVE_GENERATORS

  ifdef HAVE_GENERATOR_SKELETON
    MACROS += HAVE_GENERATOR_SKELETON
  endif

  ifdef HAVE_GENERATOR_PIP
    MACROS += HAVE_GENERATOR_PIP
  endif

  ifdef HAVE_GENERATOR_PIP_ADAPTOR
    MACROS += HAVE_GENERATOR_PIP_ADAPTOR
  endif

  ifdef HAVE_GENERATOR_PIPP_PIPR
    MACROS += HAVE_GENERATOR_PIPP_PIPR
  endif

  ifdef HAVE_GENERATOR_COMPRESS
    MACROS += HAVE_GENERATOR_COMPRESS
  endif

  ifdef HAVE_GENERATOR_DECOMPRESS
    MACROS += HAVE_GENERATOR_DECOMPRESS
  endif

  ifdef HAVE_GENERATOR_MIP
    MACROS += HAVE_GENERATOR_MIP
  endif

  ifdef HAVE_GENERATOR_WIFIPRINT
    MACROS += HAVE_GENERATOR_WIFIPRINT
  endif

  ifdef HAVE_GENERATOR_NUP
    MACROS += HAVE_GENERATOR_NUP
  endif

  ifdef HAVE_IMAGELOG
    MACROS += HAVE_IMAGELOG
  endif

  ifdef HAVE_SCANBOX
    MACROS += HAVE_SCANBOX
  endif

endif

# ------------------------------------------------------------------------------
# libgloss module 
# ------------------------------------------------------------------------------
ifdef HAVE_GLOSS
MACROS += HAVE_GLOSS
LOCAL_TARGETS += common/gloss
endif

# ------------------------------------------------------------------------------
# location module 
# ------------------------------------------------------------------------------
ifdef HAVE_LOCATION
MACROS += HAVE_LOCATION
LOCAL_TARGETS += common/location
endif

# ------------------------------------------------------------------------------
# memmgr module malloc required 
# ------------------------------------------------------------------------------
ifdef HAVE_MEMMGR
LOCAL_TARGETS += common/memmgr
endif

# ------------------------------------------------------------------------------
# network interface module 
# ------------------------------------------------------------------------------
ifdef HAVE_NETWORK
# set default stack version if none was specified
ifdef HAVE_RTOS
NETWORK_VERSION ?= v5_0_1_23
ifndef BUILD_SDK_NETWORK
LIB_TARGETS += third_party/treck-tcpip/$(NETWORK_VERSION)
else
LOCAL_TARGETS += third_party/treck-tcpip/$(NETWORK_VERSION)
endif
LIBPATH += third_party/treck-tcpip/$(NETWORK_VERSION)/lib/$(TARGETDIR)
LIBS += treck
endif

# rdj TODO temp
#LOCAL_TARGETS += third_party/uip

LOCAL_TARGETS += common/network/interface
LOCAL_TARGETS += common/network/conf
LOCAL_TARGETS += common/network/link

ifdef HAVE_NET_SERVICES
  LOCAL_TARGETS += common/network/services
  MACROS += HAVE_NET_SERVICES
endif

ifdef HAVE_RTOS
LOCAL_TARGETS += common/network/apps/net_app 
LOCAL_TARGETS += common/network/net_dispatch
endif

# OEM config
ifdef HAVE_NET_IFACE_OEM_CONFIG
    MACROS += HAVE_NET_IFACE_OEM_CONFIG
endif
ifdef HAVE_NET_LINK_OEM_CONFIG
    MACROS += HAVE_NET_LINK_OEM_CONFIG
endif

LOCAL_TARGETS += common/network
MACROS += HAVE_NETWORK

ifndef REMOVE_HAVE_IPV6
HAVE_IPV6 = 1
endif

ifdef HAVE_IPV6
   MACROS += HAVE_IPV6
endif

ifdef HAVE_RTOS
MACROS += TM_USE_CLIB
MACROS += NETWORK HAVE_SNMP_INFO NETWORK_GIGABIT
MACROS += HAVE_LPR
#TRECK specific defines
MACROS += _BSDTYPES_DEFINED _POSIX_SOURCE
select which phy we are using
MACROS += HAVE_ETH_RECV_CHKSUM_OFFLOAD TM_DEV_RECV_OFFLOAD
endif

MACROS += MRV_MODIFY


ifdef HAVE_NETIO
    LOCAL_TARGETS += common/network/apps/net_io
    MACROS += HAVE_NETIO
endif
ifdef HAVE_NET_RAW_IO
    LOCAL_TARGETS += common/network/apps/raw_io
    MACROS += HAVE_NET_RAW_IO
endif
ifdef HAVE_NETPRINT
    LOCAL_TARGETS += common/network/apps/print
    MACROS += HAVE_NETPRINT
endif
ifdef HAVE_NET_SCAN
    LOCAL_TARGETS += common/network/apps/scan
    MACROS += HAVE_NET_SCAN
endif
ifdef HAVE_NET_LPD
    LOCAL_TARGETS += common/network/apps/lpd
    MACROS += HAVE_NET_LPD
endif
ifdef HAVE_NBNS
    LOCAL_TARGETS += common/network/apps/nbns
    MACROS += HAVE_NBNS
endif
ifdef HAVE_SLP
    LOCAL_TARGETS += common/network/apps/slp
    MACROS += HAVE_SLP
endif
ifdef HAVE_SNTP
    LOCAL_TARGETS += common/network/apps/sntp
    MACROS += HAVE_SNTP
endif
ifdef HAVE_MDNS
    LOCAL_TARGETS += common/network/apps/mdns
    MACROS += HAVE_MDNS
endif
ifdef HAVE_SNMP
    LOCAL_TARGETS += common/network/apps/snmp
    MACROS += HAVE_SNMP HAVE_SNMP_INFO

    ifdef HAVE_SNMP_HOST_RESOURCES_MIB
	MACROS += HAVE_SNMP_HOST_RESOURCES_MIB
    endif

    ifdef HAVE_SNMP_PRINTER_MIB
	MACROS += HAVE_SNMP_PRINTER_MIB
    endif

    ifdef HAVE_SNMP_MIB_OEM_CONFIG
	MACROS += HAVE_SNMP_MIB_OEM_CONFIG
    endif
endif
# snmp as subagent implies third_party library provided
ifdef HAVE_SNMP_SUBAGENT

    LOCAL_TARGETS += third_party/net_snmp_subagent
    MACROS += HAVE_SNMP_SUBAGENT

    # sysapp's libs built as extension to kernel
    LIBPATH += $(PRJROOT)/sysapps/lib
    LIBPATH += $(PRJROOT)/newroot/lib

    LIBS += netsnmp
    LIBS += netsnmpagent
    LIBS += netsnmphelpers
    LIBS += netsnmpmibs
    LIBS += netsnmptrapd

endif
ifdef HAVE_TELNET
    LOCAL_TARGETS += common/network/apps/telnet
    MACROS += HAVE_TELNET
endif
ifdef HAVE_NET_CONSOLE
    LOCAL_TARGETS += common/network/apps/net_console
    MACROS += HAVE_NET_CONSOLE
endif
ifdef HAVE_LLTD
    LOCAL_TARGETS += third_party/lltd
    MACROS += HAVE_LLTD
    ifdef HAVE_DEBUG
        MACROS += USE_LLTD_CMD
    endif
endif
ifdef HAVE_RAW_SOCKET
    LOCAL_TARGETS += common/network/raw_socket
    MACROS += HAVE_RAW_SOCKET
endif

ifdef HAVE_SSL
    MACROS += HAVE_SSL

    LIBPATH += $(PRJROOT)/newroot/usr/local/ssl/lib
    LIBPATH += $(PRJROOT)/sysapps/lib
    LIBPATH += $(PRJROOT)/newroot/lib

    LIBS += ssl 
    LIBS += z
    LIBS += crypto
endif

ifdef HAVE_CURL
	ifndef HAVE_SSL
		$(error SSL library is required)
	endif
    LIBPATH += $(PRJROOT)/newroot/lib
    LIBPATH += $(PRJROOT)/sysapps/lib

    # better place?
    INCFLAGS += $(PRJROOT)/sysapps/include

    LIBS += curl
    LIBS += ssh2
    MACROS += HAVE_CURL
endif

ifdef HAVE_XMPP
	ifndef HAVE_SSL
		$(error SSL library is required)
	endif

    LIBPATH += $(PRJROOT)/sysapps/lib
    LIBPATH += $(PRJROOT)/newroot/lib

    LIBS += strophe
    LIBS += resolv

    MACROS += HAVE_XMPP
endif

ifdef HAVE_OAUTH
	ifndef HAVE_CURL
		$(error CURL library is required)
	endif

    LIB_TARGETS += third_party/liboauth
    LIBPATH += third_party/liboauth/lib/$(TARGETDIR)
    LIBS += oauth
    MACROS += HAVE_OAUTH
endif
ifdef HAVE_DNS
    LOCAL_TARGETS += common/network/apps/dns
    MACROS += HAVE_DNS
endif
ifdef HAVE_TTCP
    LOCAL_TARGETS += common/network/apps/ttcp
    MACROS += HAVE_TTCP
endif
ifdef HAVE_NET_ECHO
    MACROS += HAVE_NET_ECHO
    LOCAL_TARGETS += common/network/apps/net_echo
endif

ifdef HAVE_WSD
    ifndef HAVE_SSL
	$(error SSL library is required)
    endif

    LOCAL_TARGETS += common/network/apps/wsd 
    MACROS += WITH_NONAMESPACES 
    MACROS += DC_USER_CONFIG
    MACROS += HAVE_WSD

    LIBS += dcruntime

    ifdef HAVE_WSD_PRINT
	    MACROS += HAVE_WSD_PRINT
        LOCAL_TARGETS += common/network/apps/wsd/src/wsprint/gen
    endif
    ifdef HAVE_WSD_SCAN
	    MACROS += HAVE_WSD_SCAN
        LOCAL_TARGETS += common/network/apps/wsd/src
    endif

endif

#not sure why I have to put HAVE_SOAP_SCAN here after HAVE_WSD
#otherwise get some strange assert when scanning by my parser
ifdef HAVE_SOAP_SCAN
    ifdef HAVE_WSD_SCAN #still get some build relationship with wsd, fixme
        LOCAL_TARGETS += $(OEM_PATH)/common/scan/soapscan
        LOCAL_TARGETS += $(OEM_PATH)/common/scan/soapscan/hpscan/gen
        MACROS += HAVE_SOAP_SCAN
    endif
endif

ifdef HAVE_GCPP
	ifndef HAVE_XML
		$(error XML modules is required)
	endif

	ifndef HAVE_SSL
		$(error SSL library is required)
	endif

	ifndef HAVE_XMPP
		$(error XMPP library is required)
	endif

	ifndef HAVE_CURL
		$(error CURL library is required)
	endif

	LOCAL_TARGETS += common/network/apps/gcpp
	MACROS += HAVE_GCPP
endif

# ------------------------------------------------------------------------------
# Semantic model network
# ------------------------------------------------------------------------------

ifdef HAVE_SM_JOB
    LOCAL_TARGETS += common/network/apps/sm_job
    MACROS += HAVE_SM_JOB
endif

# obsolete -- TODO remove dependencies (e.g. GCPP)
ifdef HAVE_NET_SM
LOCAL_TARGETS += common/network/apps/net_sm
MACROS += HAVE_NET_SM
endif

# ------------------------------------------------------------------------------
# IPP server
# ------------------------------------------------------------------------------
ifdef HAVE_IPP

    ifndef HAVE_SM_JOB
        $(error SM_JOB is required)
    endif

LOCAL_TARGETS += common/network/apps/ipp
#LOCAL_TARGETS += common/network/apps/ipp/ipp_http
MACROS += HAVE_IPP
endif

# ------------------------------------------------------------------------------
# Apple AirPrint (proprietary third party)
# ------------------------------------------------------------------------------

ifdef HAVE_AIRPRINT
  LOCAL_TARGETS += third_party/airprint
  MACROS += HAVE_AIRPRINT
endif

# ------------------------------------------------------------------------------
# network Ethernet
# ------------------------------------------------------------------------------
ifdef HAVE_ETHERNET

ifndef HAVE_NETWORK
  $(error network module is required)
endif

MACROS += WIRED_NETWORK

# ethernet headers
MACROS += HAVE_ETHERNET
LOCAL_TARGETS += common/devices/eth/mac
LOCAL_TARGETS += common/devices/eth/phy
LOCAL_TARGETS += common/devices/eth

ifdef HAVE_SMTP
    LOCAL_TARGETS += common/network/apps/ports
    LIBS += curl
    LIBS += ssh2
    MACROS += HAVE_SMTP
    ifdef HAVE_IFAX
        MACROS += HAVE_IFAX
    endif
    ifdef HAVE_POP
        MACROS += HAVE_POP
    endif
    ifdef HAVE_IMAP
        MACROS += HAVE_IMAP
    endif
endif

endif # HAVE_ETHERNET

# ------------------------------------------------------------------------------
# network Wireless 
# ------------------------------------------------------------------------------
ifdef HAVE_WIRELESS

    MACROS += HAVE_WIRELESS

    # ensure that the wifi kernel modules are up-to-date
    LOCAL_TARGETS += third_party/devices/wifi

    # common wireless headers and support
    ifdef HAVE_WLAN

        MACROS += HAVE_WLAN
        ifdef HAVE_WIFI_SETTER
            MACROS += HAVE_WIFI_SETTER
        endif
        # wireless module 878x
        LOCAL_TARGETS += third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)
        LOCAL_TARGETS += common/network/apps/wifi

    endif
    ifdef HAVE_UAP

        MACROS += HAVE_UAP
        MACROS += UAP_SUPPORT

        LOCAL_TARGETS += common/network/apps/uap

    endif

    LIBS += crypto
    LIBPATH += $(PRJROOT)/sysapps/lib

endif # HAVE_WIRELESS

endif # HAVE_NETWORK

# ------------------------------------------------------------------------------
# nvram module
# ------------------------------------------------------------------------------
ifdef HAVE_NVRAM
LOCAL_TARGETS += common/nvram

MACROS += HAVE_NVRAM

ifdef HAVE_NVRAM_FILE
MACROS += HAVE_NVRAM_FILE
endif 

ifdef HAVE_STATIC_NV
MACROS += HAVE_STATIC_NV
endif 

endif

# ------------------------------------------------------------------------------
# CIFS server
# ------------------------------------------------------------------------------
ifdef HAVE_CIFS
LOCAL_TARGETS += third_party/cifs
LOCAL_TARGETS += common/network/apps/cifs
MACROS += HAVE_CIFS
endif


# ------------------------------------------------------------------------------
# oid module
# ------------------------------------------------------------------------------
ifdef HAVE_OID
LOCAL_TARGETS += common/oid
MACROS += HAVE_OID
endif


# ------------------------------------------------------------------------------
# paper module
# ------------------------------------------------------------------------------
ifdef HAVE_PAPER
LOCAL_TARGETS += common/paper
MACROS += HAVE_PAPER
endif


# ------------------------------------------------------------------------------
# parsers module
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# acl 
# ------------------------------------------------------------------------------
ifdef HAVE_ACL
LOCAL_TARGETS += common/parsers/acl
MACROS += HAVE_ACL
endif

# ------------------------------------------------------------------------------
# pjl 
# ------------------------------------------------------------------------------
ifdef HAVE_PJL
LOCAL_TARGETS += common/parsers/zpjl
MACROS += HAVE_PJL_SUPPORT
endif

ifdef HAVE_USB_HOST_PARSER
  MACROS += HAVE_USB_HOST_PARSER
  LOCAL_TARGETS += common/parsers/print_to_usb_host
  MACROS += HAVE_PASSTHRU
endif

# ------------------------------------------------------------------------------
# urf 
# ------------------------------------------------------------------------------
ifdef HAVE_URF
HAVE_PARSER_COMMOM = 1
ifdef HAVE_LASER_SW_COLOR_PIPE
#HAVE_JBIGHW = 1
endif
ifdef HAVE_LASER_HW_STRIP_COLOR_PIPE
#HAVE_JBIGHW = 1
endif
HAVE_COLOR_PIPE = 1
LOCAL_TARGETS += common/parsers/urf
MACROS += HAVE_URF_SUPPORT
HAVE_PARSER_INTERNAL_URF = 1
HAVE_PARSER_CUPS20 = 1
ifdef HAVE_PARSER_CUPS20
HAVE_PARSER_CUPS30 = 1
endif
ifdef HAVE_PARSER_UNI10
  # UNIRAST 1.0 is a 3rd party proprietary AirPrint parser.
  ifdef HAVE_AIRPRINT
    MACROS += HAVE_PARSER_UNI10
  endif
endif
ifdef HAVE_PARSER_JPEG
HAVE_JPEG_PARSER = 1
endif
endif

# ------------------------------------------------------------------------------
# common 
# ------------------------------------------------------------------------------
ifdef HAVE_PARSER_COMMOM
LOCAL_TARGETS += common/parsers/common
endif

# ------------------------------------------------------------------------------
# xml
# ------------------------------------------------------------------------------
ifdef HAVE_XML
LOCAL_TARGETS += third_party/expat_2_0_1
MACROS += HAVE_XML_SUPPORT
endif


# ------------------------------------------------------------------------------
# zj
# ------------------------------------------------------------------------------
ifdef HAVE_ZJ
LOCAL_TARGETS += common/parsers/zj
MACROS += HAVE_ZJS_SUPPORT
endif

# ------------------------------------------------------------------------------
# pf
# @@ PC fax	2013.08.28	by Juny
# ------------------------------------------------------------------------------
ifdef HAVE_PJL
LOCAL_TARGETS += common/parsers/pf
MACROS += HAVE_ZPF_SUPPORT
endif

ifdef HAVE_PJL
LOCAL_TARGETS += common/parsers/pcfax
MACROS += HAVE_ZPCFAX_SUPPORT
endif

#ifdef HAVE_USB_HOST_PARSER
#  MACROS += HAVE_USB_HOST_PARSER
#  LOCAL_TARGETS += common/parsers/print_to_usb_host
#  MACROS += HAVE_PASSTHRU
#endif

# ------------------------------------------------------------------------------
# Global Graphics Harlequin RIP
# ------------------------------------------------------------------------------
ifdef HAVE_GGSRIP
LOCAL_TARGETS += third_party/ggs_rip/parser
MACROS += HAVE_GGS_RIP_SUPPORT
ifdef HAVE_GGSRIP_FREETYPE
LIBPATH += third_party/ggs_rip/lib-freetype
LIBS += corelib 
LIBS += freetype
LIBS += pms
LIBS += oil
LIBS += pdfspool
LIBS += skin
LIBS += swfolderlib
endif
ifdef HAVE_GGSRIP_FONTFUSION
LIBPATH += third_party/ggs_rip/lib-ff
LIBS += corelib 
LIBS += ff
LIBS += pms
LIBS += oil
LIBS += pdfspool
LIBS += skin
LIBS += swfolderlib
endif
ifdef HAVE_GGSRIP_UFST
LIBPATH += third_party/ggs_rip/lib-ufst
LIBS += corelib 
LIBS += ufst5
LIBS += pms
LIBS += oil
LIBS += pdfspool
LIBS += skin
LIBS += swfolderlib
endif
LIBS += ssl     # needed w/trimmed corelib
LIBS += crypto  # needed w/trimmed corelib
LIBS += z       # needed w/trimmed corelib
endif

# ------------------------------------------------------------------------------
# image edits
# ------------------------------------------------------------------------------
ifdef HAVE_IMAGE_EDITS
LOCAL_TARGETS += common/imaging/image_edits
MACROS += HAVE_IMAGE_EDITS
endif

# ------------------------------------------------------------------------------
# Contrast overlay
# ------------------------------------------------------------------------------
ifdef HAVE_CONTRAST_OVERLAY
LOCAL_TARGETS += common/imaging/contrast_overlay
MACROS += HAVE_CONTRAST_OVERLAY
endif

# ------------------------------------------------------------------------------
# XYScale
# ------------------------------------------------------------------------------
ifdef HAVE_XYSCALE
LOCAL_TARGETS += common/imaging/xyscale
MACROS += HAVE_XYSCALE
endif

# ------------------------------------------------------------------------------
# RGB2LABI conversions (integer based conversions from RGB to/from Fax LAB)
# ------------------------------------------------------------------------------
ifdef HAVE_RGB2LABI
LOCAL_TARGETS += common/imaging/rgb2labi
MACROS += HAVE_RGB2LABI
endif

# ------------------------------------------------------------------------------
# PPM utilities
# ------------------------------------------------------------------------------
ifdef HAVE_PPM
LOCAL_TARGETS += common/imaging/ppm
MACROS += HAVE_PPM
endif

# ------------------------------------------------------------------------------
# PGM utilities
# ------------------------------------------------------------------------------
ifdef HAVE_PGM
LOCAL_TARGETS += common/imaging/pgm
MACROS += HAVE_PGM
endif

# ------------------------------------------------------------------------------
# Gray 2 BW conversion (error diffuse grayscale images to 1 bit B&W images)
# ------------------------------------------------------------------------------
ifdef HAVE_GRAY2BW
LOCAL_TARGETS += common/imaging/gray2bw
MACROS += HAVE_GRAY2BW
endif

# ------------------------------------------------------------------------------
# photo module
# ------------------------------------------------------------------------------
ifdef HAVE_PHOTO
LOCAL_TARGETS += common/photo
MACROS += HAVE_PHOTO
endif

# ------------------------------------------------------------------------------
# image_drawer module
# ------------------------------------------------------------------------------
ifdef HAVE_IMAGE_DRAWER
ifndef HAVE_UFS
$(error UFS must be included for IMAGE_DRAWER module)
endif
LOCAL_TARGETS += common/imaging/image_drawer
LOCAL_TARGETS += common/imaging/drawmgr
MACROS += HAVE_IMAGE_DRAWER
endif

# ------------------------------------------------------------------------------
# image_drawer's PPM backend
# ------------------------------------------------------------------------------
ifdef HAVE_IMAGE_DRAWER_BACKEND_PPM
ifndef HAVE_IMAGE_DRAWER
$(error IMAGE_DRAWER must be included for IMAGE_DRAWER_PPM module)
endif
LOCAL_TARGETS += common/imaging/image_drawer/ppm_backend
MACROS += HAVE_IMAGE_DRAWER_BACKEND_PPM 
endif

# ------------------------------------------------------------------------------
# image_drawer's JPEG_SW backend
# ------------------------------------------------------------------------------
ifdef HAVE_IMAGE_DRAWER_BACKEND_JPEG_SW
ifndef HAVE_IMAGE_DRAWER
$(error IMAGE_DRAWER must be included for IMAGE_DRAWER_JPEG_SW module)
endif
LOCAL_TARGETS += common/imaging/image_drawer/jpeg_sw_backend
MACROS += HAVE_IMAGE_DRAWER_BACKEND_JPEG_SW 
endif

# ------------------------------------------------------------------------------
# image_drawer's JPEG wrapped PDF backend
# ------------------------------------------------------------------------------
ifdef HAVE_IMAGE_DRAWER_BACKEND_PDF
ifndef HAVE_IMAGE_DRAWER
$(error IMAGE_DRAWER must be included for IMAGE_DRAWER_PDF module)
endif
ifndef HAVE_IMAGE_DRAWER_BACKEND_JPEG_SW
$(error JPEG_SW_BACKEND must be included for IMAGE_DRAWER_PDF module)
endif
LOCAL_TARGETS += common/imaging/image_drawer/pdf_backend
MACROS += HAVE_IMAGE_DRAWER_BACKEND_PDF 
endif

# ------------------------------------------------------------------------------
# image_drawer's PNG backend
# ------------------------------------------------------------------------------
ifdef HAVE_IMAGE_DRAWER_BACKEND_PNG
ifndef HAVE_IMAGE_DRAWER
$(error IMAGE_DRAWER must be included for IMAGE_DRAWER_PNG module)
endif
LOCAL_TARGETS += common/imaging/image_drawer/png_backend
MACROS += HAVE_IMAGE_DRAWER_BACKEND_PNG 
endif

# ------------------------------------------------------------------------------
# image_drawer's ZXRAW backend
# ------------------------------------------------------------------------------
ifdef HAVE_IMAGE_DRAWER_BACKEND_ZXRAW
ifndef HAVE_IMAGE_DRAWER
$(error IMAGE_DRAWER must be included for IMAGE_DRAWER_ZXRAW module)
endif
LOCAL_TARGETS += common/imaging/image_drawer/zx_raw_backend
MACROS += HAVE_IMAGE_DRAWER_BACKEND_ZXRAW
endif

# ------------------------------------------------------------------------------
# image_drawer's FREETYPE backend
# ------------------------------------------------------------------------------
ifdef HAVE_IMAGE_DRAWER_BACKEND_FREETYPE
ifndef HAVE_IMAGE_DRAWER
$(error IMAGE_DRAWER must be included for IMAGE_DRAWER_FREETYPE module)
endif
LOCAL_TARGETS += common/imaging/image_drawer/freetype_backend
MACROS += HAVE_IMAGE_DRAWER_BACKEND_FREETYPE
endif

# ------------------------------------------------------------------------------
# platform module
# ------------------------------------------------------------------------------
ifdef HAVE_PLATFORM
LOCAL_TARGETS += common/platform
MACROS += HAVE_PLATFORM
endif

# ------------------------------------------------------------------------------
# sdh_fax module
# ------------------------------------------------------------------------------
ifdef HAVE_SDHFAX
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/cty
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/faxapp
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/fipp_app
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/fsu
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/jbig
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/modem
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/phone
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/t30
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/thread_man
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/tiff
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/image_backup
LOCAL_TARGETS += oem/shiloh/pinetree/sdh_fax/fax_memmgr
MACROS += HAVE_SDHFAX
endif


# ------------------------------------------------------------------------------
# media manager
# ------------------------------------------------------------------------------
ifdef HAVE_MEDIA_MANAGER
LOCAL_TARGETS += common/media_manager
MACROS += HAVE_MEDIA_MANAGER
endif

#-------------------------------------------------------------------------------
# print module 
#-------------------------------------------------------------------------------

ifdef HAVE_PRINT_SUPPORT
HAVE_PRINT_CORE = 1
LOCAL_TARGETS += common/print/mode
MACROS += HAVE_PRINT_SUPPORT
ifeq ($(PRINT_MECH_TYPE), color)
  MACROS += COLOR_ENG # for laser print 
endif
endif

ifdef HAVE_PRINT_CORE
LOCAL_TARGETS += common/print/core
endif

ifdef HAVE_AUTOMATIC_DUPLEXER_SUPPORT
MACROS += HAVE_AUTOMATIC_DUPLEXER_SUPPORT
endif

#-------------------------------------------------------------------------------
# video application module 
#-------------------------------------------------------------------------------
ifdef HAVE_VIDEO_APP
LOCAL_TARGETS += common/print/video_laser

# video application depends on the video driver
HAVE_VIDEO_DRIVER=1
MACROS += VIDEO_APP
endif


# ------------------------------------------------------------------------------
# ColorPipe
# ------------------------------------------------------------------------------
ifdef HAVE_COLOR_PIPE
LOCAL_TARGETS += common/print/colorpipe
endif

# ------------------------------------------------------------------------------
# statistics 
# ------------------------------------------------------------------------------
ifdef HAVE_STATISTICS
LOCAL_TARGETS += common/statistics
MACROS += HAVE_STATISTICS
endif


#-------------------------------------------------------------------------------
# string mgr module 
#-------------------------------------------------------------------------------
ifdef HAVE_STRING_MGR
LOCAL_TARGETS += common/string_mgr/version_$(HAVE_STRING_MGR)
MACROS += HAVE_STRING_MGR=$(HAVE_STRING_MGR)
endif


#-------------------------------------------------------------------------------
# system module 
#-------------------------------------------------------------------------------
ifdef HAVE_SYSTEM
LOCAL_TARGETS += common/system
MACROS += HAVE_SYSTEM
endif

# ------------------------------------------------------------------------------
# Power manager
# ------------------------------------------------------------------------------
ifdef HAVE_POWER_MGR
LOCAL_TARGETS += common/pwr_mgr
MACROS += HAVE_POWER_MGR

ifdef HAVE_ASIC_POWER_DRIVER
LOCAL_TARGETS += common/devices/power/$(ASIC)
MACROS += HAVE_ASIC_POWER_DRIVER
endif
endif

# ------------------------------------------------------------------------------
# Zlib compression library
# ------------------------------------------------------------------------------
ifdef HAVE_ZLIB
HAVE_ZLIB = common/zlib/1.2.5
LOCAL_TARGETS += $(HAVE_ZLIB)
MACROS += HAVE_ZLIB
  ifdef HAVE_ZLIB_APPS
    #Note that this depends on the gloss os shim support 
    # HAVE_OS_SHIM and HAVE_OS_SHIM_UTIME must be defined for z_apps support
    LOCAL_TARGETS += common/z_apps
    MACROS += HAVE_ZLIB_APPS
  endif
endif

#-------------------------------------------------------------------------------
# unit test module 
#-------------------------------------------------------------------------------
ifdef HAVE_UNIT_TEST
LOCAL_TARGETS += common/unit_test
MACROS += HAVE_UNIT_TEST
endif

# ------------------------------------------------------------------------------
#  scan module
# ------------------------------------------------------------------------------
ifdef HAVE_SCAN_SUPPORT
#LOCAL_TARGETS += $(OEM_PATH)/common/scan/config
LOCAL_TARGETS += $(OEM_PATH)/common/scan/apps
ifndef HAVE_RTOS
LOCAL_TARGETS += common/scan/lsp/apps
endif
#LIBPATH += $(OEM_PATH)/common/scan/config
LIBPATH += $(OEM_PATH)/common/scan/apps
MACROS += HAVE_SCAN_SUPPORT
# MACROS += HAVE_SCAN_CONTINUOUS
# Pass the scan mech on to the compiler
MACROS += SCAN_MECH_$(SCAN_MECH)
endif

# ------------------------------------------------------------------------------
#  copy module
# ------------------------------------------------------------------------------
ifdef HAVE_COPY_SUPPORT
MACROS += HAVE_COPY_SUPPORT
endif

ifdef HAVE_COPY_CONTINUOUS
MACROS += HAVE_COPY_CONTINUOUS
endif

ifdef HAVE_COPY_TO_HOST
MACROS += HAVE_COPY_TO_HOST
endif

ifdef HAVE_ID_COPY
MACROS += HAVE_ID_COPY
endif

# ------------------------------------------------------------------------------
# Internal page manager
# ------------------------------------------------------------------------------
ifdef HAVE_INTERNAL_PAGE_MGR
LOCAL_TARGETS += common/internal_page
MACROS += HAVE_INTERNAL_PAGE_MGR
MACROS += RENDERER
endif

ifdef HAVE_JPDL_INTERNAL_PAGES
 ifndef HAVE_INTERNAL_PAGE_MGR
  LOCAL_TARGETS += common/internal_page
 endif # HAVE_INTERNAL_PAGE_MGR
 MACROS += HAVE_JPDL_INTERNAL_PAGES
endif # HAVE_JPDL_INTERNAL_PAGES


# ------------------------------------------------------------------------------
# devices directory
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# HiPS PLL device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_HIPS_PLL
  LOCAL_TARGETS += common/devices/hips_pll
  MACROS += HAVE_HIPS_PLL
endif

# ------------------------------------------------------------------------------
# axi bus device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_AXI_BUS
    LOCAL_TARGETS += common/devices/axi_bus
    MACROS += HAVE_AXI_BUS
endif

# ------------------------------------------------------------------------------
# bootspi device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_BOOTSPI
LOCAL_TARGETS += common/devices/bootspi/$(HAVE_BOOTSPI)
MACROS += HAVE_BOOTSPI
endif


# ------------------------------------------------------------------------------
# cdma device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_CDMA
LOCAL_TARGETS += common/devices/cdma/$(HAVE_CDMA)
MACROS += HAVE_CDMA
endif

# ------------------------------------------------------------------------------
# crypto device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_CRYPTO
LOCAL_TARGETS += common/devices/crypto
MACROS += HAVE_CRYPTO
endif

# ------------------------------------------------------------------------------
# mipi_dsi device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_MIPI_DSI
LOCAL_TARGETS += common/devices/mipi_dsi
MACROS += HAVE_MIPI_DSI
endif

# ------------------------------------------------------------------------------
# cpu device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_CPU
LOCAL_TARGETS += common/devices/cpu/$(HAVE_CPU)
    ifdef HAVE_WMMX
    MACROS += HAVE_WMMX
    endif
endif


# ------------------------------------------------------------------------------
#  frame buffer
# ------------------------------------------------------------------------------
    ifdef HAVE_FRAME_BUFFER
    LOCAL_TARGETS += common/devices/frame_buffer/core/$(HAVE_FRAME_BUFFER)
    MACROS += HAVE_FRAME_BUFFER
    endif

    ifdef HAVE_DISPLAY
    MACROS += HAVE_DISPLAY
    MACROS += $(HAVE_DISPLAY)
    LOCAL_TARGETS += common/devices/frame_buffer/display/$(HAVE_DISPLAY)
    endif

# ------------------------------------------------------------------------------
#  control panel 
# ------------------------------------------------------------------------------
ifeq ($(HAVE_CONTROL_PANEL),touch_control_panel)
LOCAL_TARGETS += common/devices/control_panel/$(HAVE_CONTROL_PANEL)
MACROS += HAVE_CONTROL_PANEL
endif 

# ------------------------------------------------------------------------------
#  marvell control panel 
# ------------------------------------------------------------------------------
ifeq ($(HAVE_CONTROL_PANEL),marvell_control_panel)
LOCAL_TARGETS += common/devices/control_panel/$(HAVE_CONTROL_PANEL)
MACROS += HAVE_CONTROL_PANEL
MACROS += HAVE_MARVELL_CONTROL_PANEL
endif 

# ------------------------------------------------------------------------------
#  UIO device driver library
# ------------------------------------------------------------------------------
ifdef HAVE_UIO_LIB
    LOCAL_TARGETS += common/devices/uio_lib
    MACROS += HAVE_UIO_LIB
endif

# ------------------------------------------------------------------------------
# gpio device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_GPIO
    LOCAL_TARGETS += common/devices/gpio/$(HAVE_GPIO)
    MACROS += HAVE_GPIO
endif

# ------------------------------------------------------------------------------
# switched_pwr device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_SWITCHED_PWR
    LOCAL_TARGETS += common/devices/switched_pwr
    MACROS += HAVE_SWITCHED_PWR
endif

# ------------------------------------------------------------------------------
# gpio_led device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_GPIO_LED
    LOCAL_TARGETS += common/devices/gpio_led
    MACROS += HAVE_GPIO_LED
endif

# ------------------------------------------------------------------------------
# hwconfig device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_HWCONFIG
LOCAL_TARGETS += common/devices/hwconfig
endif


# ------------------------------------------------------------------------------
# i2c device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_I2C
LOCAL_TARGETS += common/devices/i2c/$(HAVE_I2C)
MACROS += HAVE_I2C

# ------------------------------------------------------------------------------
# i2c - eeprom device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_EEPROM
LOCAL_TARGETS += common/devices/eeprom
MACROS += HAVE_EEPROM
endif

endif #HAVE_I2C


# ------------------------------------------------------------------------------
#  interrupt device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_INTERRUPT
LOCAL_TARGETS += common/devices/interrupt/$(HAVE_INTERRUPT)
endif

# ------------------------------------------------------------------------------
# jbig device driver
# ------------------------------------------------------------------------------
ifdef HAVE_JBIG
ifdef HAVE_RTOS
LOCAL_TARGETS += common/devices/jbig
else 
LOCAL_TARGETS += common/devices/jbig_uio
endif
MACROS += HAVE_JBIG
endif

# ------------------------------------------------------------------------------
#  NAND device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_NAND

LOCAL_TARGETS += common/devices/nand
MACROS += HAVE_NAND

ifdef HAVE_NAND_PLUS
LOCAL_TARGETS += common/devices/nand_plus
MACROS += HAVE_NAND_PLUS
endif #HAVE_NAND_DIRECT

ifdef HAVE_NAND_NV
MACROS += HAVE_NAND_NV
endif #HAVE_NAND_NV

# Determine the NAND size and set the appropriate macro. If no size is set
# default to a 1 Gb part.
ifeq ($(NAND_SIZE), 2Gb_part)
MACROS += HAVE_2Gb_NAND
else ifeq ($(NAND_SIZE), 1Gb_part)
MACROS += HAVE_1Gb_NAND
else
MACROS += HAVE_1Gb_NAND
endif

endif #HAVE_NAND

# ------------------------------------------------------------------------------
# One Wire Interface device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_ONE_WIRE
LOCAL_TARGETS += common/devices/one_wire/$(HAVE_ONE_WIRE)
MACROS += HAVE_ONE_WIRE
endif

# ------------------------------------------------------------------------------
#  pad config driver 
# ------------------------------------------------------------------------------
ifdef HAVE_PAD_CONFIG
LOCAL_TARGETS += common/devices/pad_config
endif

# ------------------------------------------------------------------------------
#  PWM device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_PWM
LOCAL_TARGETS += common/devices/pwm
MACROS += HAVE_PWM
endif


# ------------------------------------------------------------------------------
#  RTC device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_RTC
LOCAL_TARGETS += common/devices/rtc/$(HAVE_RTC)
MACROS += HAVE_RTC
endif


# ------------------------------------------------------------------------------
# timer device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_TIMER
LOCAL_TARGETS += common/devices/timer/$(HAVE_TIMER)
MACROS += HAVE_TIMER
endif


# ------------------------------------------------------------------------------
# uart device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_UART
LOCAL_TARGETS += common/devices/uart/$(HAVE_UART)
MACROS += HAVE_UART
ifeq ($(HAVE_UART), dwapb)
MACROS += HAVE_DWAPB_UART
else ifeq ($(HAVE_UART), mrvl_pxa920)
MACROS += HAVE_MRVL_PXA920_UART
else
$(error unsupported UART driver)
endif
endif


# ------------------------------------------------------------------------------
# usb phy device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_USB_PHY
LOCAL_TARGETS += common/devices/usb/phy/$(HAVE_USB_PHY)
MACROS += HAVE_USB_PHY
MACROS += HAVE_USB_PHY_$(HAVE_USB_PHY)
endif


# ------------------------------------------------------------------------------
# usb device - device driver
# ------------------------------------------------------------------------------
ifdef HAVE_USB_DEVICE
LOCAL_TARGETS += common/devices/usb/device/chipidea
MACROS += HAVE_USB_DEVICE

ifdef HAVE_PRINT_SUPPORT
MACROS += HAVE_USB_PRINTER_DEVICE
endif

ifdef HAVE_SCAN_SUPPORT
MACROS += HAVE_USB_SCAN_DEVICE
endif

ifeq ($(HAVE_USB_MASS_STORAGE_DEVICE), 1)
MACROS += HAVE_USB_MASS_STORAGE_DEVICE
MACROS += MS_CD_ROM
    
ifdef HAVE_EASY_INSTALL
  MACROS += HAVE_EASY_INSTALL
endif

endif

ifdef HAVE_ACL
# TODO NEED TO UDPATE usbaclcmds.c 
#MACROS += HAVE_DEBUG_USB_ACL
endif

ifdef HAVE_USB_CMD_DEVICE
MACROS += HAVE_USB_CMD_DEVICE
LOCAL_TARGETS += common/cmd/usb
endif

endif

# ------------------------------------------------------------------------------
# usb device linux gadget driver interface
# ------------------------------------------------------------------------------
ifdef HAVE_USB_GADGET
MACROS += HAVE_USB_GADGET

ifdef HAVE_PRINT_SUPPORT
MACROS += HAVE_USB_PRINTER_GADGET
endif

ifdef HAVE_SCAN_SUPPORT
MACROS += HAVE_USB_SCAN_GADGET
endif

endif


# ------------------------------------------------------------------------------
# usb host - device driver
# ------------------------------------------------------------------------------
ifdef HAVE_USB_HOST

#LOCAL_TARGETS += third_party/devices/usb/host/stack

USB_CLASS_DRIVERS :=

# NOTICE:
# the order below here matters, and should match the order
# in the makefile that builds the USB Host stack libraries

ifeq ($(HAVE_USB_MASS_STORAGE), 1)
MACROS += HAVE_USB_MASS_STORAGE
USB_CLASS_DRIVERS:=$(USB_CLASS_DRIVERS)_msc
LOCAL_TARGETS += third_party/devices/usb/host/class_apps/mass_storage_app
endif

ifeq ($(HAVE_USB_WLAN), 1)
    HAVE_WLAN=1
	MACROS += HAVE_USB_WLAN HAVE_WLAN WIRELESS HAVE_WIRELESS
	MACROS += STA_SUPPORT UAP_SUPPORT HAVE_UAP REASSOCIATION
	MACROS += HAVE_DHCPD
ifeq ($(HAVE_WIFI_CERT_TEST), 1)
	MACROS += HAVE_WIFI_CERT_TEST
endif

ifeq ($(HAVE_USB_HOST_PRINTER_CLASS), 1)
MACROS += HAVE_USB_HOST_PRINTER_CLASS
USB_CLASS_DRIVERS:=$(USB_CLASS_DRIVERS)_pcd
LOCAL_TARGETS += third_party/devices/usb/host/class_apps/printer_class_app
endif

ifeq ($(HAVE_HOTSPOT_SUPPORT), 1)
	MACROS += HAVE_HOTSPOT_SUPPORT
endif

ifeq ($(HAVE_WIFI_DIRECT_SUPPORT), 1)
	MACROS += WIFI_DIRECT_SUPPORT WFD_SUPPORT
endif

    LOCAL_TARGETS += common/network/apps/dhcpd

	ifndef WIRELESS_MODULE_TYPE
		$(error wireless module type is required)
	endif
	USB_CLASS_DRIVERS:=$(USB_CLASS_DRIVERS)_wlan$(WIRELESS_MODULE_TYPE)
	LOCAL_TARGETS += common/network/link/wlan
	LOCAL_TARGETS += third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)
	LOCAL_TARGETS += third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)/mlan
	LOCAL_TARGETS += third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)/mthreadx
	LOCAL_TARGETS += third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)/wps
	LOCAL_TARGETS += third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)/supplicant
ifeq ($(HAVE_WIFI_CERT_TEST), 1)
	LOCAL_TARGETS += third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)/sigma
endif	
	LOCAL_TARGETS += third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)/mlaninterface
endif


ifeq ($(HAVE_USB_ECHO_CLASS), 1)
  MACROS += HAVE_USB_ECHO_CLASS
 # LOCAL_TARGETS += third_party/devices/usb/host/class_apps/echo_class_app
endif

ifeq ($(HAVE_USB_HOST_EWS_CLASS), 1)
  MACROS += HAVE_USB_HOST_EWS_CLASS
  USB_CLASS_DRIVERS:=$(USB_CLASS_DRIVERS)_ews
  LOCAL_TARGETS += third_party/devices/usb/host/class_apps/ews_class_app
endif

LIBPATH += oem/$(OEM_DIR)/$(PRODUCT_DIR)/libs
LIBS += usb_host_ontime_arm_le$(USB_CLASS_DRIVERS)_$(TYPE)
MACROS += HAVE_USB_HOST

endif


# ------------------------------------------------------------------------------
# sd device
# ------------------------------------------------------------------------------
ifdef HAVE_SD
LOCAL_TARGETS += common/devices/sd/$(HAVE_SD)
MACROS += HAVE_SD
endif

# ------------------------------------------------------------------------------
# Display image pipe - software to manage hw color and hw raster monochrome pipe 
# ------------------------------------------------------------------------------
#ifdef HAVE_DISP_IMAGE_PIPE
#LOCAL_TARGETS += common/devices/disp_image_pipe
#MACROS += HAVE_DISP_IMAGE_PIPE
#endif

#ifdef HAVE_DISP_IMAGE_PIPE_EVENTS
#MACROS += HAVE_DISP_IMAGE_PIPE_EVENTS
#endif

ifdef HAVE_VIDEO_APP
MACROS += HAVE_VIDEO_APP
endif


# ------------------------------------------------------------------------------
# image pipe output DMA 
# ------------------------------------------------------------------------------
ifdef HAVE_ODMA
LOCAL_TARGETS += common/devices/odma
MACROS += HAVE_ODMA
endif

# ------------------------------------------------------------------------------
# PIP - print image pipe
# ------------------------------------------------------------------------------
ifdef HAVE_PIP
LOCAL_TARGETS += common/devices/pip
LOCAL_TARGETS += common/devices/pip/ahs
LOCAL_TARGETS += common/devices/pip/cp
LOCAL_TARGETS += common/devices/pip/cr
LOCAL_TARGETS += common/devices/pip/csc
LOCAL_TARGETS += common/devices/pip/davbuf
LOCAL_TARGETS += common/devices/pip/fhs
LOCAL_TARGETS += common/devices/pip/idma
LOCAL_TARGETS += common/devices/pip/kmod
LOCAL_TARGETS += common/devices/pip/mip_test
LOCAL_TARGETS += common/devices/pip/nga
LOCAL_TARGETS += common/devices/pip/pip_stats
LOCAL_TARGETS += common/devices/pip/pipe_access
LOCAL_TARGETS += common/devices/pip/raster_odma
LOCAL_TARGETS += common/devices/pip/sccsc
LOCAL_TARGETS += common/devices/pip/serp
LOCAL_TARGETS += common/devices/pip/tp
LOCAL_TARGETS += common/devices/pip/ued
LOCAL_TARGETS += common/devices/pip/th
LOCAL_TARGETS += common/devices/pip/yipi
LOCAL_TARGETS += common/devices/pip/zx_test
LOCAL_TARGETS += common/devices/pip/color_reg_enhance
ifeq ($(ASIC_REV), b0)
LOCAL_TARGETS += common/devices/pip/ctwin
LOCAL_TARGETS += common/devices/pip/sharpen
LOCAL_TARGETS += common/devices/pip/tems
endif
MACROS += HAVE_PIP 
endif

ifdef HAVE_ZXDRAWER
LOCAL_TARGETS += common/devices/zxdrawer
MACROS += HAVE_ZXDRAWER
endif

# ------------------------------------------------------------------------------
# PIPP - print image pipe input pogo
# ------------------------------------------------------------------------------
ifdef HAVE_PIPP
LOCAL_TARGETS += common/devices/pipp
MACROS += HAVE_PIPP 

HAVE_TEXT_MERGE=1
HAVE_COLOR_REG_ENHANCE=1
HAVE_COLOR_REPLACE=1
HAVE_COLOR_SPACE_CONVERSION=1
HAVE_CONTONE_POGO=1
HAVE_OBJECT_TYPE_POGO=1
HAVE_DEPOGO=1
HAVE_UDMA=1
HAVE_PIPP_IDMA=1
HAVE_PIPP_ODMA=1
endif

# ------------------------------------------------------------------------------
# PIPP ODMA - print image pipe POGO output DMA driver
# ------------------------------------------------------------------------------
ifdef HAVE_PIPP_ODMA
LOCAL_TARGETS += common/devices/pipp/pipp_odma
endif

# ------------------------------------------------------------------------------
# PIPP IDMA - print image pipe POGO input DMA driver
# ------------------------------------------------------------------------------
ifdef HAVE_PIPP_ODMA
LOCAL_TARGETS += common/devices/pipp/pipp_idma
endif

# ------------------------------------------------------------------------------
# PIPP SC_CSC - print image pipe pogo subcube color space conversion
# ------------------------------------------------------------------------------
ifdef HAVE_COLOR_SPACE_CONVERSION
LOCAL_TARGETS += common/devices/pipp/sc_csc
endif

# ------------------------------------------------------------------------------
# TEXT MERGE - print image pipe object type and contone data merge
# ------------------------------------------------------------------------------
ifdef HAVE_TEXT_MERGE
LOCAL_TARGETS += common/devices/pipp/text_merge
endif

# ------------------------------------------------------------------------------
# CONTONE POGO - print image pipe contone POGO 
# ------------------------------------------------------------------------------
ifdef HAVE_CONTONE_POGO
LOCAL_TARGETS += common/devices/pipp/contone_pogo
endif

# ------------------------------------------------------------------------------
# OBJECT TYPE POGO - print image pipe object type POGO 
# ------------------------------------------------------------------------------
ifdef HAVE_OBJECT_TYPE_POGO
LOCAL_TARGETS += common/devices/pipp/object_type_pogo
endif

# ------------------------------------------------------------------------------
# COLOR REPLACE - print image pipe data merge, color replace, object mapper
# ------------------------------------------------------------------------------
ifdef HAVE_COLOR_REPLACE
LOCAL_TARGETS += common/devices/pipp/color_replace
endif

# ------------------------------------------------------------------------------
# COLOR REGISTRATION ENHANCEMENT - print image pipe color trapping block
# ------------------------------------------------------------------------------
ifdef HAVE_COLOR_REG_ENHANCE
LOCAL_TARGETS += common/devices/pipp/color_reg_enhance
endif

# ------------------------------------------------------------------------------
# DEPOGO - print image pipe de-pogo
# ------------------------------------------------------------------------------
ifdef HAVE_DEPOGO
LOCAL_TARGETS += common/devices/pipp/depogo
endif

# ------------------------------------------------------------------------------
# END PIPP  - print image pipe input pogo
# ------------------------------------------------------------------------------



# ------------------------------------------------------------------------------
# PIPR - print image pipe raster 
# ------------------------------------------------------------------------------
ifdef HAVE_PIPR
LOCAL_TARGETS += common/devices/pipr
MACROS += HAVE_PIPR

HAVE_UDMA=1
HAVE_PIPR_IDMA=1
HAVE_PIPR_ODMA=1
HAVE_PIPR_CONTONE_PAD=1
HAVE_TEXT_PAD=1
HAVE_TH_HALFTONE=1
HAVE_PIPR_MIP=1
HAVE_UED=1
HAVE_FHS=1
HAVE_YIPI=1
HAVE_EDGER=1
HAVE_TH_HALFTONE=1
HAVE_ACCUM=1
HAVE_ONE_D_LUT=1
endif

# ------------------------------------------------------------------------------
# monochrome image pipe block
# ------------------------------------------------------------------------------
ifdef HAVE_PIPR_MIP
LOCAL_TARGETS += common/devices/pipr/mip
LOCAL_TARGETS += common/devices/pipr/mip/$(ASIC)

MACROS += HAVE_MIP
endif

# ------------------------------------------------------------------------------
# PIPR ODMA - print image pipe raster output DMA driver
# ------------------------------------------------------------------------------
ifdef HAVE_PIPR_ODMA
LOCAL_TARGETS += common/devices/pipr/pipr_odma
endif

# ------------------------------------------------------------------------------
# PIPR IDMA - print image pipe raster input DMA driver
# ------------------------------------------------------------------------------
ifdef HAVE_PIPR_IDMA
LOCAL_TARGETS += common/devices/pipr/pipr_idma
endif

# ------------------------------------------------------------------------------
# PIPR contone pad
# ------------------------------------------------------------------------------
ifdef HAVE_PIPR_CONTONE_PAD
LOCAL_TARGETS += common/devices/contone_pad
endif

# ------------------------------------------------------------------------------
# PIPR text pad
# ------------------------------------------------------------------------------
ifdef HAVE_TEXT_PAD
LOCAL_TARGETS += common/devices/pipr/text_pad
MACROS += HAVE_TEXT_PAD
endif

# ------------------------------------------------------------------------------
# PIPR Universal Error Diffusion
# ------------------------------------------------------------------------------
ifdef HAVE_UED
LOCAL_TARGETS += common/devices/pipr/ued
MACROS += HAVE_UED
endif

# ------------------------------------------------------------------------------
# PIPR Fractional Horizontal Scale
# ------------------------------------------------------------------------------
ifdef HAVE_FHS
LOCAL_TARGETS += common/devices/pipr/fhs
MACROS += HAVE_FHS
endif

# ------------------------------------------------------------------------------
# PIPR Yellow Insertion
# ------------------------------------------------------------------------------
ifdef HAVE_YIPI
LOCAL_TARGETS += common/devices/pipr/yipi
MACROS += HAVE_YIPI
endif

# ------------------------------------------------------------------------------
# PIPR Edger
# ------------------------------------------------------------------------------
ifdef HAVE_EDGER
LOCAL_TARGETS += common/devices/pipr/edger
MACROS += HAVE_EDGER
endif

# ------------------------------------------------------------------------------
# PIPR Threshold Halftone
# ------------------------------------------------------------------------------
ifdef HAVE_TH_HALFTONE
LOCAL_TARGETS += common/devices/pipr/th_halftone
MACROS += HAVE_TH_HALFTONE
endif

# ------------------------------------------------------------------------------
# PIPR Pixel Accumulator
# ------------------------------------------------------------------------------
ifdef HAVE_ACCUM
LOCAL_TARGETS += common/devices/pipr/accum
MACROS += HAVE_ACCUM
endif

# ------------------------------------------------------------------------------
# One Dimensional LUT
# ------------------------------------------------------------------------------
ifdef HAVE_ONE_D_LUT
LOCAL_TARGETS += common/devices/pipr/one_d_lut
MACROS += HAVE_ONE_D_LUT
endif


# ------------------------------------------------------------------------------
# END PIPR - print image pipe raster 
# ------------------------------------------------------------------------------



# ------------------------------------------------------------------------------
# UDMA - universal DMA reference hardware
# ------------------------------------------------------------------------------
ifdef HAVE_UDMA
LOCAL_TARGETS += common/devices/udma
endif


# ------------------------------------------------------------------------------
# video block 
# ------------------------------------------------------------------------------
ifdef HAVE_VIDEO_DRIVER
ifdef HAVE_RTOS
LOCAL_TARGETS += common/devices/video
else
LOCAL_TARGETS += common/devices/linux_laser_video_jbig_ko
endif
endif

# ------------------------------------------------------------------------------
# tcl module
# ------------------------------------------------------------------------------
ifdef HAVE_TCL
LOCAL_TARGETS += third_party/tcl
MACROS += HAVE_TCL
endif

# ------------------------------------------------------------------------------
# print engine block 
# ------------------------------------------------------------------------------

ifdef PRINT_MECH

# bring in needed common engine api code.  handling of the specific mech
# is done in oem.mk and the specific engine's makefile
LOCAL_TARGETS += common/print/engine
MACROS += HAVE_PRINT_MECH

else ifdef HAVE_ENGINE_STUB

LOCAL_TARGETS += common/print/engine
MACROS += HAVE_ENGINE_STUB

endif


# ------------------------------------------------------------------------------
# spi device
# ------------------------------------------------------------------------------
ifdef HAVE_SPI
LOCAL_TARGETS += common/devices/spi/$(HAVE_SPI)
MACROS += HAVE_SPI
endif

# ------------------------------------------------------------------------------
# dec devices 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC
LOCAL_TARGETS += common/devices/dec
MACROS += HAVE_DEC
endif

# ------------------------------------------------------------------------------
# dec Sensor device 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC_SENSOR
LOCAL_TARGETS += common/devices/dec_sensor
MACROS += HAVE_DEC_SENSOR
endif

# ------------------------------------------------------------------------------
# dec ADC device 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC_ADC
LOCAL_TARGETS += common/devices/dec_adc
MACROS += HAVE_DEC_ADC
endif

# ------------------------------------------------------------------------------
# dec fuser device 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC_FUSER
LOCAL_TARGETS += common/devices/dec_fuser
MACROS += HAVE_DEC_FUSER
endif

# ------------------------------------------------------------------------------
# ZeroCross block 
# ------------------------------------------------------------------------------
ifdef HAVE_FUSER_ZC_DRIVER
# LOCAL_TARGETS += common/devices/linux_fuser_zc_ko
endif

# ------------------------------------------------------------------------------
# ------------------------------------------------------------------------------
# dec laser device 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC_LASER
LOCAL_TARGETS += common/devices/dec_laser
MACROS += HAVE_DEC_LASER
endif

# ------------------------------------------------------------------------------
# dcmotor device 
# ------------------------------------------------------------------------------
ifdef HAVE_DCMOTOR
LOCAL_TARGETS += common/devices/dcmotor
MACROS += HAVE_DCMOTOR
endif

# ------------------------------------------------------------------------------
# dcmotor device - open loop version
# ------------------------------------------------------------------------------
ifdef HAVE_DCMOTOR_OL
LOCAL_TARGETS += common/devices/dcmotor_ol
MACROS += HAVE_DCMOTOR
endif

# ------------------------------------------------------------------------------
# stepper device 
# ------------------------------------------------------------------------------
ifdef HAVE_STEPPER
LOCAL_TARGETS += common/devices/stepper
MACROS += HAVE_STEPPER
endif

# ------------------------------------------------------------------------------
# jpeg hardware assist 
# ------------------------------------------------------------------------------
ifdef HAVE_JPEG_HW_ASSIST
  LOCAL_TARGETS += common/devices/jpeg
  MACROS += HAVE_JPEG_HW_ASSIST
endif

ifdef HAVE_AUDIO
  LOCAL_TARGETS += common/devices/audio
  LOCAL_TARGETS += common/audio_mgr
  MACROS += HAVE_AUDIO_SUPPORT
endif

ifdef HAVE_CODE_COVERAGE
  ifneq ($(HAVE_CODE_COVERAGE), 0)
    LIB_TARGETS += third_party/code_coverage/$(HAVE_CODE_COVERAGE)
    LIBPATH += third_party/code_coverage/$(HAVE_CODE_COVERAGE)/lib/$(TARGETDIR)
    LIBS += $(HAVE_CODE_COVERAGE)
    MACROS += HAVE_CODE_COVERAGE
  endif
endif

ifdef HAVE_UFST_FONT_SUPPORT
ifndef HAVE_KINOMA
  LOCAL_TARGETS += $(OEM_PATH)/common/font_sys
  LIB_TARGETS += third_party/font/ufst5_0
  LIBPATH += third_party/font/ufst5_0/rts/lib
  LIBS += if tt
  MACROS += HAVE_UFST_FONT_SUPPORT
 
  FONT_TARGETS = FONT_TRUETYPE_SUPPORT
  #FONT_TARGETS = FONT_INTELLFONT_SUPPORT
  #FONT_TARGETS = FONT_MICROTYPE_SUPPORT
  #FONT_TARGETS = FONT_POSTSCRIPT_SUPPORT
else
  LIB_TARGETS += third_party/font/ufst6_1
  LIBPATH += third_party/font/ufst6_1/rts/lib
  LIBS += if tt fco  
endif
endif

# ------------------------------------------------------------------------------
# Cmd Module (requires tcl and strfmt)
# ------------------------------------------------------------------------------
ifdef HAVE_CMD
ifndef HAVE_STRFMT
$(error strfmt must be included for cmd module)
endif
MACROS += HAVE_CMD
LOCAL_TARGETS += common/cmd
LOCAL_TARGETS += common/cmd/interp
ifdef HAVE_SYSTEM
LOCAL_TARGETS += common/parsers/cmd
endif
ifdef HAVE_CMD_VARS
MACROS += HAVE_CMD_VARS
endif
endif

# ------------------------------------------------------------------------------
# Strfmt Module
# ------------------------------------------------------------------------------
ifdef HAVE_STRFMT
LOCAL_TARGETS += common/strfmt
MACROS += HAVE_STRFMT
endif

# ------------------------------------------------------------------------------
# Freetype
# ------------------------------------------------------------------------------
ifdef HAVE_FREETYPE
LOCAL_TARGETS += common/imaging/freetype
LIBPATH += third_party/freetype2/lib
LIBS += freetype
MACROS += HAVE_FREETYPE
endif

# ------------------------------------------------------------------------------
# JPG2PDF (wrap JPG files into PDF files)
# ------------------------------------------------------------------------------
ifdef HAVE_JPG2PDF
ifndef HAVE_UFS
$(error UFS must be included for JPG2PDF module)
endif
ifndef HAVE_STRFMT
$(error STRFMT must be included for JPG2PDF module)
endif
LOCAL_TARGETS += common/imaging/jpg2pdf
MACROS += HAVE_JPG2PDF
endif

# ------------------------------------------------------------------------------
# EMAIL (send email with attached file)
# ------------------------------------------------------------------------------
ifdef HAVE_EMAIL
ifndef HAVE_UFS
$(error UFS must be included for EMAIL module)
endif
ifndef HAVE_STRFMT
$(error STRFMT must be included for EMAIL module)
endif
LOCAL_TARGETS += common/email
MACROS += HAVE_EMAIL
endif

# ------------------------------------------------------------------------------
# EMAIL (send email with attached file)
# ------------------------------------------------------------------------------
ifdef HAVE_EMAIL_APPS
ifndef HAVE_EMAIL
$(error EMAIL must be included for EMAIL Applications module)
endif
LOCAL_TARGETS += common/email/apps/common_email_apps
LOCAL_TARGETS += common/email/apps/file_to_email
ifdef HAVE_SCAN_SUPPORT
LOCAL_TARGETS += common/email/apps/scan_to_email
endif
LOCAL_TARGETS += common/email/apps/jpg2pdf_to_email
MACROS += HAVE_EMAIL_APPS
endif

# ------------------------------------------------------------------------------
# LIBPNG
# ------------------------------------------------------------------------------
ifdef HAVE_LIBPNG
ifndef HAVE_UFS
$(error UFS must be included for PNG module)
endif
LOCAL_TARGETS += common/imaging/libpng
LIBPATH += third_party/libpng/lib
LIBS += png
MACROS += HAVE_LIBPNG
endif


# ------------------------------------------------------------------------------
# Kinoma
# ------------------------------------------------------------------------------
ifdef HAVE_KINOMA
  LOCAL_TARGETS += third_party/kinoma/build/unity
  LIBPATH += third_party/kinoma
  LIBS += kinoma_$(TYPE)
  MACROS += HAVE_KINOMA
  ifdef INITIALIZE_KINOMA_FILE_SYSTEM
    export INITIALIZE_KINOMA_FILE_SYSTEM
  endif
  ifdef COMPRESS_KINOMA_FILE_SYSTEM
    export COMPRESS_KINOMA_FILE_SYSTEM
  endif
  ifdef BUILD_KINOMA_FILE_SYSTEM
    export BUILD_KINOMA_FILE_SYSTEM
  endif
  ifdef KINOMA_MANIFEST_FILE
    export KINOMA_MANIFEST_FILE
  endif
endif


# ------------------------------------------------------------------------------
# Zx Renderer
# ------------------------------------------------------------------------------
ifdef HAVE_ZXRENDERER
ifndef HAVE_UFS
$(error UFS must be included for Zx Renderer module)
endif
LOCAL_TARGETS += common/renderers/zx
LOCAL_TARGETS += common/renderers/zx/zsri
LOCAL_TARGETS += common/renderers/zx/SSPR
LOCAL_TARGETS += common/renderers/zx/ztag
LOCAL_TARGETS += common/renderers/zx/zwutils
LOCAL_TARGETS += common/renderers/zx/zjpeg
MACROS += HAVE_ZXRENDERER
ifdef HAVE_ZXR_TEST
MACROS += HAVE_ZXR_TEST
endif
endif

# ------------------------------------------------------------------------------+}
# CRI PA800 Consummable Protection System (CPS)
# ------------------------------------------------------------------------------+}
ifdef HAVE_PA800_CPS

LOCAL_TARGETS += third_party/pa800_cps

LIBPATH += $PRJROOT/newroot/libs
LIBS += PrinterHostInterface

#LIBS += Callback
#LIBS += eccSignatures
#LIBS += Hal
#LIBS += halArc
#LIBS += halI2COem
#LIBS += halI2CData
#LIBS += Manager
#LIBS += MsgLoggerLite
#LIBS += Osal
#LIBS += Primitives
#LIBS += PrinterHostInterface
#LIBS += SecurePrng
#LIBS += SwAuthUtilVer-a.ccf-arc
#LIBS += tomcrypt
#LIBS += tommath
#LIBS += Utils
#LIBS += VcfContCert
#LIBS += Verifier
MACROS += HAVE_PA800_CPS

# include the test serial commands
ifdef HAVE_PA800_CPS_CMD 
LOCAL_TARGETS += third_party/pa800_cps/test_cmd
MACROS += HAVE_PA800_CPS_CMD
endif

endif

# ------------------------------------------------------------------------------
# Build OEM specific modules after main build for dependency reasons
# ------------------------------------------------------------------------------
ifdef OEM_PATH
include $(OEM_PATH)/oem.mk
endif

export MACROS
export LOCAL_TARGETS

# Add standard libraries from buildenv.mk to the *end* of the library path 
# (filesystem needs to override some function calls that are also defined in libc)
LIBS += $(STD_LIBS)

ifdef HAVE_CPLUSPLUS
STD_LIBS += stdc++
endif

# ------------------------------------------------------------------------------
# services module: System Service Layer
# ------------------------------------------------------------------------------
ifdef HAVE_SYSTEM_SERVICE_LAYER
LOCAL_TARGETS += common/systemservice
MACROS += HAVE_SYSTEM_SERVICE_LAYER
#LIBPATH +=
#LIBS +=
endif

# ------------------------------------------------------------------------------
# Tiny debugger 
# ------------------------------------------------------------------------------
ifdef HAVE_TINY_DEBUGGER
LOCAL_TARGETS += common/tinydebugger
MACROS += HAVE_TINY_DEBUGGER
endif

# -----------------------------------------------------------------------------
# Card Reader 
# -----------------------------------------------------------------------------
ifdef HAVE_CARD_READER
LOCAL_TARGETS += common/cardreader
MACROS += HAVE_CARD_READER
endif
ifdef HAVE_PAPERCUT
MACROS += HAVE_PAPERCUT
endif

LOCAL_TARGETS += $(OEM_PATH)/common/network/manager

# ------------------------------------------------------------------------------
# PDF module: Scan Image to PDF Service 
# ------------------------------------------------------------------------------
ifdef HAVE_HPDF_LIB
LOCAL_TARGETS += $(OEM_PATH)/common/scan/hpdflib
MACROS += HAVE_HPDF_LIB
#LIBPATH +=
#LIBS +=
endif
