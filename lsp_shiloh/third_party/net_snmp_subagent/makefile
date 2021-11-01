# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = net_snmp_subagent_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS  = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# net-snmp sysapp headers in $(SYSAPP_INC) convenience dir
INCFLAGS += -I${PRJROOT}/sysapps/include

# sysapp libs in $(TGT_FS)/lib
LSP_LIB   = -L${PRJROOT}/newroot/lib
LDFLAGS  += $(LSP_LIB)
LDPARAM  += $(LSP_LIB)

# specify source files to build
SOURCE  = net_snmp_subagent_main.c
SOURCE += printer_mib_translation.c
SOURCE += printer_mib_validation.c
SOURCE += prtGeneralTable.c
SOURCE += prtInputTable.c
SOURCE += prtMarkerTable.c
SOURCE += prtMarkerSuppliesTable.c
SOURCE += prtMarkerColorantTable.c
SOURCE += prtMediaPathTable.c
SOURCE += prtInterpreterTable.c
SOURCE += sdhEngineTable.c
SOURCE += hrDeviceTable.c
SOURCE += hrPrinterTable.c
SOURCE += portMonitorTable.c
SOURCE += systemSystem.c
SOURCE += sdhEngGeneral.c
SOURCE += sdhEngStatus.c
#SOURCE += sdhEngineTable_old.c
SOURCE += sdhEngOld.c
include $(BUILD_ROOT)/stdtargets.mk

