# ============================================================================
# Copyright (c) 2009-2011 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS =-I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
ifdef HAVE_FREETYPE
INCFLAGS += -I$(BUILD_ROOT)/common/internal_page/src
endif

INSTALL_SRC = internal_page_config.h
INSTALL_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
OTHER_SRC_DIR = libs

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))


# specify source files to build
SOURCE  = internal_page_config.c
#SOURCE += internal_page_example.c
#SOURCE += diagnostics_page.c
#SOURCE += demopage.c
#SOURCE += calibration_page_example.c
SOURCE += report_manager.c
SOURCE += report_page.c
SOURCE += json_to_pcl.c
SOURCE += barcode_generator.c

#yajl lib
SOURCE += yajl.c
SOURCE += yajl_alloc.c
SOURCE += yajl_buf.c
SOURCE += yajl_encode.c
SOURCE += yajl_gen.c
SOURCE += yajl_lex.c
SOURCE += yajl_parser.c
SOURCE += yajl_version.c

#ctemplate lib
SOURCE += ctemplate.c

#tif lib
SOURCE += tif_lzw.c
SOURCE += tif_flush.c
#SOURCE += tif_acorn.c
SOURCE += tif_fax3sm.c
SOURCE += tif_unix.c
#SOURCE += tif_win32.c
SOURCE += tif_next.c
#SOURCE += tif_atari.c
SOURCE += tif_read.c
SOURCE += tif_dir.c
SOURCE += tif_close.c
SOURCE += tif_tile.c
SOURCE += tif_write.c
#SOURCE += mkspans.c
SOURCE += tif_aux.c
SOURCE += tif_predict.c
SOURCE += tif_codec.c
SOURCE += tif_dumpmode.c
SOURCE += tif_strip.c
SOURCE += tif_warning.c
SOURCE += tif_compress.c
SOURCE += tif_open.c
SOURCE += tif_jpeg.c
SOURCE += tif_fax3.c
SOURCE += tif_print.c
#SOURCE += tif_win3.c
#SOURCE += mkg3states.c
#SOURCE += tif_vms.c
SOURCE += tif_getimage.c
SOURCE += tif_dirwrite.c
#SOURCE += tif_msdos.c
SOURCE += tif_version.c
SOURCE += tif_dirinfo.c
#SOURCE += tif_pixarlog.c
SOURCE += tif_packbits.c
SOURCE += tif_swab.c
#SOURCE += tiff2pcl.c
SOURCE += tif_error.c
SOURCE += tif_thunder.c
SOURCE += tif_dirread.c
#SOURCE += tif_apple.c
#SOURCE += tif_zip.c
#SOURCE += mkversion.c
SOURCE += tif_color.c
SOURCE += tif_extension.c

REPORT_TEMPLATE = network.tmpl 
REPORT_TEMPLATE += system.tmpl
REPORT_TEMPLATE += service.tmpl
REPORT_TEMPLATE += txrxresultreport.tmpl
REPORT_TEMPLATE += tcrreport.tmpl
REPORT_TEMPLATE += errlog.tmpl
REPORT_TEMPLATE += t30logreport.tmpl
REPORT_TEMPLATE += memtxjoblist.tmpl
REPORT_TEMPLATE += tcrforfaxservice.tmpl
REPORT_TEMPLATE += addressbook.tmpl
REPORT_TEMPLATE += spamfaxlist.tmpl
REPORT_TEMPLATE += broadcastresultreport.tmpl
REPORT_TEMPLATE_PATH = config/templates
REPORT_TEMPLATE_TARGET = $(REPORT_TEMPLATE_PATH)/report_templates.h

vpath %.tmpl $(REPORT_TEMPLATE_PATH)
vpath %.temp $(REPORT_TEMPLATE_PATH)

INCFLAGS += -I$(REPORT_TEMPLATE_PATH)
INCFLAGS += -Ilibs

PREBUILD = $(REPORT_TEMPLATE_TARGET)
CLEAN_SRC = $(REPORT_TEMPLATE_TARGET)

.PHONY: internal_page report

include $(BUILD_ROOT)/stdtargets.mk

report: $(REPORT_TEMPLATE_TARGET)

$(REPORT_TEMPLATE_TARGET): $(REPORT_TEMPLATE:.tmpl=.temp)
	cat $^ > $@

%.temp: %.tmpl
	xxd -i $< $@

# override install rule with rule to run the 
# rules for the template header
install: $(PRODUCT_PATH)/internal_page/config/templates/report_template.h

$(PRODUCT_PATH)/internal_page/config/templates/report_template.h: report
	@echo --------------------------------------
	@echo Running Report template header builder
	@echo --------------------------------------
