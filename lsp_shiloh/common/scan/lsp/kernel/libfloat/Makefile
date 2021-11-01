# Makefile for the Linux soft-float library

CC=arm-marvell-linux-gnueabi-gcc -O2 -freg-struct-return -fomit-frame-pointer -D__LIBFLOAT__ -D__ARM_EABI__ -mfloat-abi=softfp
#CC=gcc -g -O2 -freg-struct-return -D__LIBFLOAT__
AR=arm-marvell-linux-gnueabi-ar

all: libfloat.a

#mq libfloat.so

libfloat.a: softfloat.o fplib_glue.o
	rm -f libfloat.a
	$(AR) cq libfloat.a softfloat.o fplib_glue.o

libfloat.so: softfloat.os fplib_glue.os
	rm -f libfloat.so
	$(CC) -shared softfloat.os fplib_glue.os -o libfloat.so
#	gcc -shared softfloat.os fplib_glue.os -o libfloat.so

softfloat.o: softfloat/bits64/softfloat.c
	$(CC) -c -o softfloat.o -Isoftfloat/bits64/ARM-gcc softfloat/bits64/softfloat.c

fplib_glue.o: fplib_glue.S
	$(CC) -c -o fplib_glue.o fplib_glue.S

softfloat.os: softfloat/bits64/softfloat.c
	$(CC) -fpic -c -o softfloat.os -Isoftfloat/bits64/ARM-gcc softfloat/bits64/softfloat.c

fplib_glue.os: fplib_glue.S
	$(CC) -fpic -c -o fplib_glue.os fplib_glue.S

install: libfloat.a libfloat.so
	cp -a libfloat.a /usr/lib
	cp -a libfloat.so /usr/lib
	ldconfig

clean: 
	rm -f *.o
	rm -f *.os
	rm -f libfloat.a
	rm -f libfloat.so
	rm -f *~
