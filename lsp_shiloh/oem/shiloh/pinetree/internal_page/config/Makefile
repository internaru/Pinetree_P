TEST_LIB_DIR=../libs

SOURCE = ${TEST_LIB_DIR}/tif_lzw.c
SOURCE += ${TEST_LIB_DIR}/tif_flush.c
#SOURCE += ${TEST_LIB_DIR}/tif_acorn.c
SOURCE += ${TEST_LIB_DIR}/tif_fax3sm.c
SOURCE += ${TEST_LIB_DIR}/tif_unix.c
#SOURCE += ${TEST_LIB_DIR}/tif_win32.c
SOURCE += ${TEST_LIB_DIR}/tif_next.c
#SOURCE += ${TEST_LIB_DIR}/tif_atari.c
SOURCE += ${TEST_LIB_DIR}/tif_read.c
SOURCE += ${TEST_LIB_DIR}/tif_dir.c
SOURCE += ${TEST_LIB_DIR}/tif_close.c
SOURCE += ${TEST_LIB_DIR}/tif_tile.c
SOURCE += ${TEST_LIB_DIR}/tif_write.c
#SOURCE += ${TEST_LIB_DIR}/mkspans.c
SOURCE += ${TEST_LIB_DIR}/tif_aux.c
SOURCE += ${TEST_LIB_DIR}/tif_predict.c
SOURCE += ${TEST_LIB_DIR}/tif_codec.c
SOURCE += ${TEST_LIB_DIR}/tif_dumpmode.c
SOURCE += ${TEST_LIB_DIR}/tif_strip.c
SOURCE += ${TEST_LIB_DIR}/tif_warning.c
SOURCE += ${TEST_LIB_DIR}/tif_compress.c
SOURCE += ${TEST_LIB_DIR}/tif_open.c
SOURCE += ${TEST_LIB_DIR}/tif_jpeg.c
SOURCE += ${TEST_LIB_DIR}/tif_fax3.c
SOURCE += ${TEST_LIB_DIR}/tif_print.c
#SOURCE += ${TEST_LIB_DIR}/tif_win3.c
#SOURCE += ${TEST_LIB_DIR}/mkg3states.c
#SOURCE += ${TEST_LIB_DIR}/tif_vms.c
SOURCE += ${TEST_LIB_DIR}/tif_getimage.c
SOURCE += ${TEST_LIB_DIR}/tif_dirwrite.c
#SOURCE += ${TEST_LIB_DIR}/tif_msdos.c
SOURCE += ${TEST_LIB_DIR}/tif_version.c
SOURCE += ${TEST_LIB_DIR}/tif_dirinfo.c
SOURCE += ${TEST_LIB_DIR}/tif_pixarlog.c
SOURCE += ${TEST_LIB_DIR}/tif_packbits.c
SOURCE += ${TEST_LIB_DIR}/tif_swab.c
SOURCE += ${TEST_LIB_DIR}/tiff2pcl.c
SOURCE += ${TEST_LIB_DIR}/tif_error.c
SOURCE += ${TEST_LIB_DIR}/tif_thunder.c
SOURCE += ${TEST_LIB_DIR}/tif_dirread.c
#SOURCE += ${TEST_LIB_DIR}/tif_apple.c
SOURCE += ${TEST_LIB_DIR}/tif_zip.c
#SOURCE += ${TEST_LIB_DIR}/mkversion.c

SOURCE += ${TEST_LIB_DIR}/yajl_alloc.c
SOURCE += ${TEST_LIB_DIR}/yajl_buf.c
SOURCE += ${TEST_LIB_DIR}/yajl.c
SOURCE += ${TEST_LIB_DIR}/yajl_encode.c
SOURCE += ${TEST_LIB_DIR}/yajl_gen.c
SOURCE += ${TEST_LIB_DIR}/yajl_lex.c
SOURCE += ${TEST_LIB_DIR}/yajl_parser.c
SOURCE += ${TEST_LIB_DIR}/yajl_version.c
SOURCE += ${TEST_LIB_DIR}/ctemplate.c

all:
	xxd -i test.tmpl > test_templates.h
	gcc -I${TEST_LIB_DIR} -o test test.c json_to_pcl.c report_page.c ${SOURCE} -lm
	./test
	cp test.pcl ~/

clean:
	rm *.o
