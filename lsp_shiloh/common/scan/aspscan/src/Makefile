# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
#
#                         Marvell Confidential
# ============================================================================

#CC=cgcc
CC=gcc
CFLAGS+=-g -Wall -Wextra -I../include -I/usr/include/libusb-1.0
CPPFLAGS+=-g -Wall -Wextra -I../include -I/usr/include/libusb-1.0
LDFLAGS=-g -L/usr/lib 

SOURCES=aspscan.c log.c xassert.c hdump.c cmdline.c str.c asp.c

EXE=aspscan rgb2lab xrgbtorgb

all : $(EXE)

aspscan : aspscan.o xassert.o log.o hdump.o cmdline.o str.o usbtools.o asp.o \
          aspusb.o
	$(CC) $(LDFLAGS) -o $@ $^ -lusb-1.0

# davep 12-Apr-07 ; convert raw RGB to XYZ to Lab
rgb2lab : rgb2lab.o
	$(CC) $(CFLAGS) -o $@ $< -lm
	
rgb2lab.o : rgb2lab.c

# davep 17-Apr-06 ; test program to generate tcns lut
tcns : tcns.o log.o xassert.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

xrgbtorgb : xrgbtorgb.o

xrgbtorgb.o : xrgbtorgb.c 

clean:
	rm -f $(EXE) *.o *.d

