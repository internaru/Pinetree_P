all:  testsf  ftest 
# testemu testsfpic ftestemu ftestpic

CC=arm-marvell-linux-gnueabi-gcc -L../ -O0 -freg-struct-return -D__LIBFLOAT__ -D__ARM_EABI__ -msoft-float -lfloat -lc

# -Bstatic -l../float
#CC=gcc --verbose  -c -S

#testemu: floattest.c
#	gcc -O2 floattest.c -o testemu 

testsf: floattest.c
	$(CC) -static floattest.c -o floattest.exe 
	
	#../libfloat.a
	
#	gcc -O2 -static floattest.c -msoft-float -o testsf /usr/lib/libfloat.a

#testsfpic: floattest.c
#	gcc -O2 floattest.c -msoft-float -o testsfpic /usr/lib/libfloat.so

#ftestemu: ftest.c
#	gcc -O2 -static ftest.c -o ftestemu

ftest: ftest.c
	$(CC) -static ftest.c  -o ftest.exe 
	
	#../libfloat.a
	
	#../libfloat.a
#	gcc -O2 -static ftest.c -msoft-float -o ftest /usr/lib/libfloat.a

#ftestpic: ftest.c
#	gcc -O2 ftest.c -msoft-float -o ftestpic /usr/lib/libfloat.so

clean: 
	rm -f testemu
	rm -f testsf
	rm -f testsfpic
	rm -f ftest
	rm -f ftestemu
	rm -f ftestpic
	rm -f floattest.exe 
	rm -f ftest.exe 
	rm -f *~
