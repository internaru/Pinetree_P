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

obj-m := kscantask.o

ARCH=arm 
HOSTCC="/usr/bin/gcc" 
CROSS_COMPILE="arm-marvell-linux-gnueabi-"

# relative path to common (core) scan code
COMMON_SCAN=../../../../../common/scan

kscantask-y := 

SRC = scan.c scanif.c pic.c safeint.c icetest.c picdma_descrip.c \
                scands.c strtable.c ostools.c\
		cisx.c ddma.c icedma.c picpcdma_descrip.c scantools.c scancmdq.c \
		scanmech.c scandbg.c scanimg.c pie.c piedma.c piemsdma.c strippr.c\
		safetylock.c scanpipe.c scanlib.c soft_piehp.c fwmargin.c pager.c\
		icefilter.c pipecut.c scancap.c chipgap.c adfpath.c adfgapper.c \
		scanvars.c fraction.c scanlog.c scos_kernel.c scanmargin.c 

#INCFLAGS += -I$(../../../)/common/include\

# TODO these files are a work in progress
#SRC+=scantask.c taskqueue.c taskqueue_kernel.c  

#CFLAGS_MODULE+=-DNULL_MECH_DRIVER
SRC += scanmech_null.c 

SRC += scanpower-stub.c

# linux specific code
KERNEL_SRC=list.c mem_linux.c int_linux.c cpu_linux.c hw_linux.c lehmer.c \
            str_linux.c icetest_linux.c scanmem_linux.c \
            stubs_linux.c cal_linux.c scanman_linux.c float_linux.c \
            grayline_stub.c test_stubs.c scantask_linux.c

# Uncomment to enable the scanalyzer
#SCANALYZER=1

# uncomment to include my test code
SCAN_TEST_CODE=1

# Define SCAN_HIQ to include host-side scan image quality (siqfile) support.
# A siqfile contains settings to manipulate the various settings, tables, etc,
# in the scan path (e.g., PIE LUTs). (HIQ == "Host Image Quality")
SCAN_HIQ=1

# test code 
ifdef SCAN_TEST_CODE
    SRC += icetesttest.c pictest_common.c pictest_3bdr.c picdma_descrip_test.c \
	    cisxtest.c scantest.c pietest.c scanlibtest.c scanvarstest.c
    CFLAGS_MODULE+=-DHAVE_SCAN_TEST_CODE
endif

ifdef SCAN_HIQ
    SRC += lex.yy.c siqnames.c siqdata.c scanvarsiq.c
    CFLAGS_MODULE += -DHAVE_SCANHIQ_SUPPORT
endif

# optional utility code
#ifdef SCANALYZER
#    SRC += scanalyzer.c
#    CFLAGS_MODULE += -DHAVE_SCANALYZER_SUPPORT
#endif

#GEMSTONE=1
GRANITE=1

ifdef GEMSTONE
    HAZ_ASIC=1
    SRC += picbm2005.c scanif_mcfg2005.c scanif_led2005.c piedma2005.c
    CFLAGS_ASIC=-I$(KERNELDIR)/arch/arm/mach-mv31x0/include/asic/asic_B0 \
	        -I$(KERNELDIR)/arch/arm/mach-mv31x0/include/ \
                -I$(PWD)/asic/gemstoneB0
    kscantask-y += scanplat_linux.o scanplat_linux_gemstone.o

    # some stubs until have real hardware
    SRC += adfsensor_stub.c

    # Alum sensor using internal AFE
    kscantask-y += scansen_4729.o afe_internal_mas.o 

    KERNELDIR=$(PRJROOT)/linux-mrvl/
endif

ifdef GRANITE 
    HAZ_ASIC=1
    SRC += picbm2005.c scanif_mcfg.c scanif_led_2sen.c piedma2005.c
    CFLAGS_ASIC=-I$(KERNELDIR)/arch/arm/mach-mv61x0/include/asic/asic_gr_C0 \
   	        -I$(KERNELDIR)/arch/arm/mach-mv61x0/include/ \
                -I$(src)/$(COMMON_SCAN)/src/asic/granite 
    kscantask-y += scanplat_linux.o scanplat_linux_granite.o

    # some stubs until have real hardware
#    SRC += adfsensor_stub.c
    kscantask-y += adfsensor_pinetree.o scanmech_pinetree.o scanmot_pinetree.o
    CFLAGS_MODULE+=-DSCAN_MECH_SHILOH

    # sensor and afe stub
#    kscantask-y += scansen_stub.o afe_stub.o
    kscantask-y += scansen-target.o afe_wm8213.o

    KERNELDIR=$(PRJROOT)/linux-mrvl/
endif

ifndef HAZ_ASIC
    $(error no can haz ASIC?)
endif

# convert all weird paths to something kbuild can find
kscantask-y += $(SRC:%.c=$(COMMON_SCAN)/src/%.o)
kscantask-y += $(KERNEL_SRC:%.c=linux-kernel/src/%.o)

#$(error $(kscantask-y))

ccflags-y := $(CFLAGS_ASIC) \
	     -I$(src)/$(COMMON_SCAN)/include\
	     -I$(src)/$(COMMON_SCAN)/src\
	     -I$(src)/linux-kernel/include

CFLAGS_MODULE += -DDEBUG

# block agMessage.h from being included in core scan code (firewall)
CFLAGS_MODULE += -D__AGMESSAGE_H__

all:
	make -C $(KERNELDIR) M=$(PWD) modules

#clean:
#	make -C $(KERNELDIR) M=$(PWD) clean

clean:
	@echo $(kscantask-y)
	$(RM) $(kscantask-y)


backup:
	@echo $(kscantask-y)
	sh backup.sh $(kscantask-y:.o=.c)

