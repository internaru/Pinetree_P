#!/bin/sh
#modified at 2015/01/13

STREAM_NAME=kara-s_ctl.app
SAVE_DIR=~/coverity/$STREAM_NAME
rm -rf $SAVE_DIR
echo Analysis data will be saved at $SAVE_DIR

export PATH=$PATH:/opt/cov-analysis-linux-7.0.1/bin
cov-configure --comptype gcc --compiler /opt/armcc.mv61x0/bin/arm-marvell-linux-gnueabi-gcc
cov-configure --compiler /opt/armcc.mv61x0/bin/arm-marvell-linux-gnueabi-gcc 
cov-configure -co arm-marvell-linux-gnueabi-gcc -- -mapcs-frame -mlittle-endian -mmarvell-div -mcpu=marvell-fv7 -mfloat-abi=hard

#-- --sysroot /home2/kyungbal/SDK/03-20-2011/output/staging/

echo Building....
make PRODUCT=shiloh_pinetree_mech TYPE=debug clean
cov-build --preprocess-next --dir $SAVE_DIR make PRODUCT=shiloh_pinetree_mech TYPE=debug

echo Analyzing
cov-analyze --aggressiveness-level medium --all --enable-callgraph-metrics -j 4 --dir $SAVE_DIR

echo Committing
echo SAVE_DIR: $SAVE_DIR
echo STREAM_NAME: $STREAM_NAME
cov-commit-defects --dir $SAVE_DIR --host 10.15.1.49 --stream $STREAM_NAME --user kyungbal --password dlrudqkf --cva

echo All Done
