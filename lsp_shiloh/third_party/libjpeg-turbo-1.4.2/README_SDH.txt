1. configure
  $./configure --host=arm-linux CC=arm-marvell-linux-gnueabi-gcc  --with-jpeg8

2. make
  $make

etc...
1. bench test example
  $tjbench testout_tile.ppm 95 -rgb -quiet -tile -benchtime 0.01 >/dev/null 2>&1