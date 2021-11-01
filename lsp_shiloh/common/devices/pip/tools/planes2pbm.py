#!/usr/bin/python
#******************************************************************************
# * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
# *
# *                         Marvell Confidential
#******************************************************************************

# This script converts multiple planes of raw data into viewable pbm files. To
# do this, it adds a basic header to the beginning of each plane, and saves the
# result with the .pbm extension. When opened with a pbm viewer, the data in the
# images will be interpreted as 1bpp.


import sys
import re

num_planes = 6

if len(sys.argv) != 2:
    print "Usage: python planes2pbm.py <plane prefix>"
    sys.exit(1)

# Open the .info file and parse Width (row_stride) and Height (height).
file_name = sys.argv[1] + ".info"
print "fin_info: ", file_name
fin_info = open(file_name, "rb")
field = fin_info.readline()
r = re.match( 'Width: (\d+)', field )
if (r):
    row_stride = r.group(1)
else:
    print "Could not find the Width param in the .info file."
field = fin_info.readline()
r = re.match( 'Height: (\d+)', field )
if (r):
    height = r.group(1)
else:
    print "Could not find the Height param in the .info file."
fin_info.close()

# Process each channel of input data.
for n in range(0, num_planes):
    # Open the input file
    file_name = sys.argv[1] + "_" + str(n) + ".bin"
    print "fin_data: ", file_name
    fin_data = open(file_name, "rb")
    data_in = fin_data.read()
    fin_data.close()

    # Open the output file
    file_name = sys.argv[1] + "_" + str(n) + ".pbm"
    print "fout_pbm: ", file_name
    fout_pbm = open(file_name, "wb")

    # Write out the header
    header = "P4\n{0}\n{1}\n.".format(int(row_stride)*8, height)
    fout_pbm.write(header)

    # Write out the data
    fout_pbm.write(data_in)

    # Close the output file
    fout_pbm.close()


