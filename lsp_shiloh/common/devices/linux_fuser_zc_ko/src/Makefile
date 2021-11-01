
obj-m := fuser_zc_dev.o

ARCH=arm 
HOSTCC="/usr/bin/gcc" 
CROSS_COMPILE="arm-marvell-linux-gnueabi-"

BUILDROOT=../../../..

# relative path to DrOS code; we need to copy the Module.symvers file from
# this directory as we depend on its exports.  We copy to allow the kernel
# make file to find the symbols as describe in the kernel's 
# Documentation/kbuild/modules.txt section 6.3 Symbols From Another External Module
# The method we follow is "Use an extra Module.symvers file".
DROS_PATH=$(BUILDROOT)/common/os_shim/linux-kernel/src

# start building the file list
#
#laservideo-y := 
#laser_video_chardev-y :=

# linux specific code 
#KERNEL_SRC=list.c mem_linux.c int_linux.c cpu_linux.c hw_linux.c lehmer.c \
#           str_linux.c lassert.c dros_linux.c

CFLAGS_MODULE+=-DHAVE_DBG_CALLBACK_CMDS

GRANITE=1
MACROS =

ifdef GEMSTONE
    HAZ_ASIC=ASIC_gemstone
    MACROS += $(HAZ_ASIC)
    MACROS += ASIC_REV=$(ASIC_REV_NUM)
    KERNELDIR=$(PRJROOT)/linux-mrvl/
    CFLAGS_ASIC=-I$(src)/$(BUILDROOT)/common/asic/gemstone/include \
		-I$(src)/$(BUILDROOT)/common/asic/gemstone/b0/include
endif

ifdef GRANITE      
    HAZ_ASIC=ASIC_granite
    ASIC_REV_C0_ID = 130
    MACROS += $(HAZ_ASIC)
    MACROS += ASIC_REV_C0=$(ASIC_REV_C0_ID)
    MACROS += ASIC_REV=$(ASIC_REV_C0_ID)
    KERNELDIR=$(PRJROOT)/linux-mrvl/
    CFLAGS_ASIC=-I$(src)/$(BUILDROOT)/common/asic/granite/include \
		-I$(src)/$(BUILDROOT)/common/asic/granite/c0/include
endif

ifndef HAZ_ASIC
    $(error no can haz ASIC?)
endif

#$(error $(SRC))

# convert all weird paths to something kbuild can find
#laservideo-y += $(KERNEL_SRC:%.c=$(BUILDROOT)/common/os_shim/linux-kernel/src/%.o)
#laservideo-y += $(LASERVIDEO_SRC:%.c=%.o)
#laser_video_chardev-y += $(LASERVIDEO_CHARDEV_SRC:%.c=%.o)

#$(error $(PWD) $(laservideo-y))
linux_printer_path += $(shell grep CONFIG_EXTRA_DIR_NAME $(PRJROOT)/linux-mrvl/.config | cut -d= -f2 | cut -d\" -f2)

ccflags-y := $(CFLAGS_ASIC) \
	     -I$(src)/$(BUILDROOT)/common/os_shim/linux-kernel/include \
	     -I$(src)/$(BUILDROOT)/common/os_shim/include \
	     -I$(src)/$(BUILDROOT)/common/include \
	     -I$(src)/$(BUILDROOT)/common/devices/linux_fuser_zc_ko/include \
	     -I$(src)/$(BUILDROOT)/common/devices/linux_fuser_zc_ko/src \
	     -I$(src)/$(BUILDROOT)/common \
	     -I$(PRJROOT)/linux-mrvl/arch/arm/mach-gogin/$(linux_printer_path)/src

#$(error $(ccflags-y))

CFLAGS_MODULE += -DDEBUG $(foreach MACRO,$(MACROS),-D$(MACRO))

all:
	@echo kerneldir = $(KERNELDIR)
	cp $(DROS_PATH)/Module.symvers .
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

