SELF_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
MOD_DIR  := $(realpath $(SELF_DIR)/../..)
CC=arm-marvell-linux-gnueabi-gcc
export CC

FT_DIR=$(SELF_DIR)/freetype-2.4.4
INC_DIR=$(MOD_DIR)/.include_$(PRODUCT)
SRC_DIR=$(SELF_DIR)
CONFIG_ARGS=--host=arm-marvell-eabi 

all:
	cd $(FT_DIR) &&\
	./configure $(CONFIG_ARGS) CPPFLAGS="-I$(INC_DIR) -I$(SRC_DIR)" &&\
	cp $(SELF_DIR)/ftoption.h $(FT_DIR)/include/freetype/config &&\
	make &&\
	cp $(FT_DIR)/objs/.libs/libfreetype.a $(SELF_DIR)/lib &&\
	make distclean
