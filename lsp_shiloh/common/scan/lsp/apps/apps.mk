# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# Makefile for embedded Linux userspace scan apps. These apps are specific to
# only the scan code and are used for test/debug.
#
 
#CC=gcc
CC=arm-marvell-linux-gnueabi-gcc

# relative path to the top of the entire source tree
TREE_TOP=../../../..

# relative path to common (core) scan code
COMMON_SCAN=$(TREE_TOP)/common/scan
COMMON_UTILS=$(TREE_TOP)/common/utils

KERNELDIR=$(PRJROOT)/linux-mrvl/

CFLAGS_ASIC=-I$(KERNELDIR)/arch/arm/mach-gogin/include/asic/asic_gr_C0 \
            -I$(KERNELDIR)/arch/arm/mach-gogin/include/ \
            -I$(COMMON_SCAN)/src/asic/granite 

CFLAGS=-I.\
       -I$(COMMON_SCAN)/include \
       -I$(COMMON_SCAN)/src \
       -I$(COMMON_SCAN)/chimera/include \
       -I$(COMMON_UTILS)/include \
       -I$(COMMON_SCAN) \
       $(CFLAGS_ASIC)

CFLAGS+=-DLINUX -DDEBUG -DHAVE_SCANHIQ_SUPPORT -DNULL_MECH_DRIVER

EXE=runscan scancmd

all : $(EXE)

runscan : scanlog.o minprintf.o memory.o debug.o scanvars.o\
          lex.yy.o siqnames.o siqdata.o scanvarsiq.o\
          fraction.o scancap.o safeint.o grayline.o runscan.o\
          safetylock.o str.o scos.o pic_common.o ostools.o \
          mbuffer.o scanplat_linux.o cmdline.o outfile.o scands.o \
          strtable.o
	@$(CC) $(CFLAGS) -o $@ $^ -lm -lrt -lpthread

scancmd : scancmd.o 
	@$(CC) $(CFLAGS) -g -Wall -o $@ $^ -lm -lrt -lpthread

runscan.o : runscan.c
	@$(CC) $(CFLAGS) -g -Wall -c -o $@ $<

outfile.o : outfile.c
	@$(CC) $(CFLAGS) -g -Wall -c -o $@ $<

scancmd.o : scancmd.c
	@$(CC) $(CFLAGS) -D__USE_GNU -g -Wall -c -o $@ $<

str.o : $(COMMON_UTILS)/src/str.c
	@$(CC) -I $(COMMON_UTILS)/include $(CFLAGS) -g -Wall -c -o $@ $<

minprintf.o : $(COMMON_SCAN)/chimera/src/minprintf.c
	@$(CC) $(CFLAGS) -g -Wall -c -o $@ $<

memory.o : $(COMMON_SCAN)/chimera/src/memory.c
	@$(CC) $(CFLAGS) -g -Wall -c -o $@ $<

debug.o : $(COMMON_SCAN)/chimera/src/debug.c
	@$(CC) $(CFLAGS) -g -Wall -c -o $@ $<

scanplat_linux.o : scanplat_linux.c
	@$(CC) -DNULL_MECH_DRIVER $(CFLAGS) -g -Wall -c -o $@ $<

cmdline.o : cmdline.c
	@$(CC) $(CFLAGS) -g -Wall -c -o $@ $<

# if all else fails, look in common code
%.o : $(COMMON_SCAN)/src/%.c
	@$(CC) $(CFLAGS) -g -Wall -c -o $@ $<

clean:
	$(RM) $(EXE) *.o

