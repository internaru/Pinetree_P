#include ../../../common.mk

all: md5.o sha1.o

clean:
	rm *.o

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
