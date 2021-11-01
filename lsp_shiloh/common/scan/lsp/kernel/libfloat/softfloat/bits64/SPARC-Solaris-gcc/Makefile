
PROCESSOR_DIR = ../../../processors
PROCESSOR_H = $(PROCESSOR_DIR)/SPARC-gcc.h
SOFTFLOAT_MACROS = ../softfloat-macros
CC = gcc
CINCLUDES = -I. -I.. -I-
CFLAGS = -O2
LINK = gcc
LFLAGS =
TIMESOFTFLOAT_EXE = timesoftfloat

ALL: softfloat.o $(TIMESOFTFLOAT_EXE)

milieu.h: $(PROCESSOR_H)
	touch milieu.h

softfloat.o: milieu.h softfloat.h softfloat-specialize $(SOFTFLOAT_MACROS) ../softfloat.c
	$(CC) $(CINCLUDES) $(CFLAGS) -c ../softfloat.c

timesoftfloat.o: milieu.h softfloat.h ../timesoftfloat.c
	$(CC) $(CINCLUDES) $(CFLAGS) -c ../timesoftfloat.c

$(TIMESOFTFLOAT_EXE): softfloat.o timesoftfloat.o
	$(LINK) $(LFLAGS) -o $(TIMESOFTFLOAT_EXE) softfloat.o timesoftfloat.o

