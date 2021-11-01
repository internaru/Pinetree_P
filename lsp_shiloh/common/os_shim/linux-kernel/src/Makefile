#
# **************************************************************************************
# *
# * Copyright (c) 2013 Marvell International, Ltd.
# *
# **************************************************************************************
# *
# * Marvell Commercial License Option
# *
# * If you received this File from Marvell as part of a proprietary software release,
# * the File is considered Marvell Proprietary and Confidential Information, and is
# * licensed to you under the terms of the applicable Commercial License.
# *
# **************************************************************************************
# *
# * Marvell GPL License Option
# *
# * If you received this File from Marvell as part of a Linux distribution, this File
# * is licensed to you in accordance with the terms and conditions of the General Public
# * License Version 2, June 1991 (the "GPL License").  You can redistribute it and/or
# * modify it under the terms of the GPL License; either version 2 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful, but WITHOUT ANY
# * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# * PARTICULAR PURPOSE.  See the GPL License for more details.
# *
# * You should have received a copy of the GNU General Public License along with this
# * program.  If not, see http://www.gnu.org/licenses/.
# *
# **************************************************************************************

# Makefile for Driver OS shim layer as linux kernel module. 
# Builds for both 6120 and 6170.

obj-m := dros.o

ARCH=arm
HOSTCC="/usr/bin/gcc"
CROSS_COMPILE="arm-marvell-linux-gnueabi-"

# start building the file list
#
dros-y := 

DROS_SRC=list.c mem_linux.c int_linux.c cpu_linux.c hw_linux.c lehmer.c \
           str_linux.c lassert.c dros_linux.c dros_kmod.c

#CFLAGS_MODULE+=-DHAVE_DBG_CALLBACK_CMDS

GRANITE=1
BUILDROOT=../../../..
MACROS =
KERNELDIR=$(PRJROOT)/linux-mrvl/

ifdef GEMSTONE
    HAZ_ASIC=ASIC_gemstone
    MACROS += $(HAZ_ASIC)
    MACROS += ASIC_REV=$(ASIC_REV_NUM)
    CFLAGS_ASIC=-I$(src)/$(BUILDROOT)/common/asic/gemstone/include \
		-I$(src)/$(BUILDROOT)/common/asic/gemstone/b0/include
endif

ifdef GRANITE      
    HAZ_ASIC=ASIC_granite
    ASIC_REV_C0_ID = 130
    MACROS += $(HAZ_ASIC)
    MACROS += ASIC_REV_C0=$(ASIC_REV_C0_ID)
    MACROS += ASIC_REV=$(ASIC_REV_C0_ID)
    CFLAGS_ASIC=-I$(src)/$(BUILDROOT)/common/asic/granite/include \
		-I$(src)/$(BUILDROOT)/common/asic/granite/c0/include
endif

ifndef HAZ_ASIC
    $(error no can haz ASIC?)
endif

#$(error $(SRC))

# convert all weird paths to something kbuild can find
dros-y += $(DROS_SRC:%.c=%.o)

#$(error $(PWD) $(dros-y))
linux_printer_path += $(shell grep CONFIG_EXTRA_DIR_NAME $(PRJROOT)/linux-mrvl/.config | cut -d= -f2 | cut -d\" -f2)

ccflags-y := $(CFLAGS_ASIC) \
	     -I$(src)/$(BUILDROOT)/common/os_shim/linux-kernel/include \
	     -I$(src)/$(BUILDROOT)/common/os_shim/include \
	     -I$(PRJROOT)/linux-mrvl/arch/arm/mach-gogin/$(linux_printer_path)/src

#$(error $(ccflags-y))

CFLAGS_MODULE += -DDEBUG $(foreach MACRO,$(MACROS),-D$(MACRO))

all:
	@echo kerneldir = $(KERNELDIR)
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

