# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

# Makefile for scantask as linux kernel module. 
# Builds for both 6120 and 6170.
# davep 17-Jul-2012
#
# Split code into two trees: oem and common. Build is out of oem tree.
# davep 29-Aug-2012
#

obj-m := kscantask.o kscanman.o qsort.o scansb.o

ARCH=arm 
HOSTCC="/usr/bin/gcc" 
CROSS_COMPILE="arm-marvell-linux-gnueabi-"

# relative path to common (core) scan code
COMMON_SCAN=../../../../../common/scan

BUILDROOT=../../../../..

# relative path to DrOS code; we need to copy the Module.symvers file from
# this directory as we depend on its exports.  We copy to allow the kernel
# make file to find the symbols as describe in the kernel's 
# Documentation/kbuild/modules.txt section 6.3 Symbols From Another External Module
# The method we follow is "Use an extra Module.symvers file".
DROS_PATH=$(BUILDROOT)/common/os_shim/linux-kernel/src

# Add fakescan, an interface to interact with scanman as if we were scanning
# real data.
SCAN_FAKESCAN=1

# Uncomment to enable the scanalyzer
SCANALYZER=1

# uncomment to include my test code
SCAN_TEST_CODE=1

# Define SCAN_HIQ to include host-side scan image quality (siqfile) support.
# A siqfile contains settings to manipulate the various settings, tables, etc,
# in the scan path (e.g., PIE LUTs). (HIQ == "Host Image Quality")
SCAN_HIQ=1

# start building the file list
kscantask-y := 
kscanman-y :=

SRC = scan.c scanif.c pic.c safeint.c icetest.c picdma_descrip.c \
                scands.c strtable.c ostools.c\
		cisx.c ddma.c icedma.c picpcdma_descrip.c scantools.c scancmdq.c \
		scanmech.c scandbg.c scanimg.c pie.c piedma.c piemsdma.c strippr.c\
		safetylock.c scanpipe.c scanlib.c soft_piehp.c fwmargin.c \
		icefilter.c pipecut.c scancap.c chipgap.c adfpath.c adfgapper.c \
		scanvars.c fraction.c scanlog.c scanmargin.c \
                scantask.c taskqueue.c cmdline.c pic_common.c scanlib_msg.c\
                piecs.c notaheap.c piemux.c stripdma.c pipemap.c scos_kernel.c

CAL=cal_common.c cal_digital_correction.c cal_gardenia.c \
    cal_analog_H.c calcaplist.c findcalstrip.c
SRC+=$(CAL)

#CFLAGS_MODULE+=-DNULL_MECH_DRIVER
SRC += scanmech_null.c 

SRC += scanpower-stub.c

# test code 
ifdef SCAN_TEST_CODE
    SRC += icetesttest.c pictest_common.c pictest_3bdr.c picdma_descrip_test.c \
	    cisxtest.c scantest.c pietest.c scanlibtest.c scanvarstest.c\
            pictest.c pictest_pc.c stripdmatest.c
    CFLAGS_MODULE+=-DHAVE_SCAN_TEST_CODE
endif

ifdef SCAN_HIQ
    SRC += lex.yy.c siqnames.c siqdata.c scanvarsiq.c
    CFLAGS_MODULE += -DHAVE_SCANHIQ_SUPPORT
endif

HAVE_NETWORK=1
ifdef HAVE_NETWORK
    CFLAGS_MODULE += -DHAVE_NETWORK
    KERNEL_SRC += scannet.c pipenet.c siqnet.c
endif

# davep 12-Sep-2012 ; we have a command line now!
CFLAGS_MODULE+=-DHAVE_DBG_CALLBACK_CMDS
SRC+=scos_cmd_proc.c

ifdef SCAN_FAKESCAN
    SRC += fakescan.c
    CFLAGS_MODULE+=-DHAVE_FAKESCAN_SUPPORT
endif

# optional utility code
ifdef SCANALYZER
    SRC += scanalyzer.c
    CFLAGS_MODULE += -DHAVE_SCANALYZER_SUPPORT
    # scanalyzer.c wants HAVE_CMD
    CFLAGS_MODULE += -DHAVE_CMD
endif

GRANITE=1

SCANMECH_SRC=

ifdef GRANITE 
    HAZ_ASIC=1
    KERNELDIR=$(PRJROOT)/linux-mrvl/
    SRC += picbm2005.c scanif_mcfg.c scanif_led_2sen.c piedma2005.c
    CFLAGS_ASIC=-I$(KERNELDIR)/arch/arm/mach-gogin/include/asic/asic_gr_C0 \
	       -I$(KERNELDIR)/arch/arm/mach-gogin/include/ \
		    -I$(src)/$(COMMON_SCAN)/src/asic/granite 
    SCANMECH_SRC += scanplat_linux.o scanplat_linux_granite.o

    # some stubs until have real hardware
#    SRC += adfsensor_stub.c
    SCANMECH_SRC += adfsensor_pinetree.o

    SCANMECH_SRC += scanmech_pinetree.o scanmot_pinetree_cdma.o
    CFLAGS_MODULE+=-DSCAN_MECH_SHILOH

    # sensor and afe stub
#    SRC += scansen_stub.c afe_stub.c
    SCANMECH_SRC += scansen-target.o afe_wm8213.o

    # tell scan code we are little endian
    CFLAGS_MODULE+=-DASP_ON_LITTLE_ENDIAN
endif

#$(error $(SRC))

ifndef HAZ_ASIC
    $(error no can haz ASIC?)
endif

SCANTASK_KERNEL_SRC=scantask_linux.c scanmem_linux.c list.c mem_linux.c\
    str_linux.c lassert.c lehmer.c test_stubs.c str_linux.c\
    cpu_linux.c scannet.c pipenet.c icetest_linux.c hw_linux.c float_linux.c\
    stubs_linux.c

# convert all weird paths to something kbuild can find
kscantask-y += $(SRC:%.c=$(COMMON_SCAN)/src/%.o)
kscantask-y += $(SCANTASK_KERNEL_SRC:%.c=$(COMMON_SCAN)/lsp/kernel/src/%.o)
kscantask-y += libfloat.a
kscantask-y += $(SCANMECH_SRC:%.c=$(COMMON_SCAN)/src/%.o)

# linux specific code 
SCANMAN_KERNEL_SRC=scanman_linux.c mem_linux.c str_linux.c stubs_linux.c lassert.c 

kscanman-y += $(SCANMAN_KERNEL_SRC:%.c=$(COMMON_SCAN)/lsp/kernel/src/%.o)
kscanman-y += $(COMMON_SCAN)/src/scanlog.o
kscanman-y += $(COMMON_SCAN)/src/notaheap.o
kscanman-y += $(COMMON_SCAN)/src/scos_kernel.o

#$(error $(kscantask-y))

# Quicksort. Required by calibration.
qsort-y := $(COMMON_SCAN)/lsp/kernel/src/qsort.o

# scansb is GPL module for calling GPL-only kernel functions. It's a bit of an
# odd collection of code.
scansb-y := scanplat_sb.o
scansb-y += $(COMMON_SCAN)/lsp/kernel/src/lassert.o

linux_printer_path += $(shell grep CONFIG_EXTRA_DIR_NAME $(PRJROOT)/linux-mrvl/.config | cut -d= -f2 | cut -d\" -f2)

ccflags-y := $(CFLAGS_ASIC) \
	     -I$(src)/$(COMMON_SCAN)/include\
	     -I$(src)/$(COMMON_SCAN)/src\
	     -I$(src)/$(BUILDROOT)/common/os_shim/linux-kernel/include\
	     -I$(src)/$(BUILDROOT)/common/os_shim/include\
	     -I$(src)/$(COMMON_SCAN)/lsp/kernel/include\
	     -I$(PRJROOT)/linux-mrvl/arch/arm/mach-gogin/m2m\
	     -I$(PRJROOT)/linux-mrvl/arch/arm/mach-gogin/$(linux_printer_path)/src

CFLAGS_MODULE += -DDEBUG
CFLAGS_MODULE += -DHAVE_SCANPLAT_SCLKPER
CFLAGS_MODULE += -DHAVE_SCANLEDEXP_CAL
CFLAGS_MODULE += -DHAVE_PINETREE_FORM_FACTOR

# block agMessage.h from being included in core scan code (firewall)
CFLAGS_MODULE += -D__AGMESSAGE_H__

all: libfloat.a
        # this is an obnoxious workaround to the problem of Chimera's cmd_proc
        # being shared between both the kernel kscantask and userspace code.
#	@$(RM) $(COMMON_SCAN)/chimera/src/cmd_proc.o
	cp $(DROS_PATH)/Module.symvers .
	make -C $(KERNELDIR) M=$(PWD) modules

libfloat.a: 
	make -C $(COMMON_SCAN)/lsp/kernel/libfloat clean all
	cp $(COMMON_SCAN)/lsp/kernel/libfloat/libfloat.a .

clean:
	$(RM) $(kscantask-y) libfloat.a
	make -C $(COMMON_SCAN)/lsp/kernel/libfloat clean
	make -C $(KERNELDIR) M=$(PWD) clean

mkdist:
	@python mkdist.py $(kscantask-y:.o=.c) $(kscanman-y:.o=.c) $(qsort-y:.o=.c) $(scansb-y:.o=.c)

