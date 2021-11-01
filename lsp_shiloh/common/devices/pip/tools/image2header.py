#!/usr/bin/python
#******************************************************************************
# * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
# *
# *                         Marvell Confidential
#******************************************************************************


import os.path
import sys
from PIL import Image

if len(sys.argv) != 3:
    print "Usage: 'python image2header.py <input image> <output file base name>'\n"
    exit(1)

# Open the output file.
im = Image.open(sys.argv[1])
output_file_base_name = sys.argv[2]
header_filename = output_file_base_name + "_rgb32.h"
f = open(header_filename, 'w')

# Write the copyright statement, header guards, and array declaration.
f.write("/******************************************************************************\n")
f.write(" * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved\n")
f.write(" *\n")
f.write(" *                         Marvell Confidential\n")
f.write(" ******************************************************************************/\n")
f.write("\n")
temp_str = "#ifndef {0}_RGB32_H\n".format(output_file_base_name.upper())
f.write(temp_str)
temp_str = "#define {0}_RGB32_H\n".format(output_file_base_name.upper())
f.write(temp_str)
f.write("\n")
temp_str = "unsigned char __attribute__ ((aligned(512))) {0}rgb32_bin[] = {{\n".format(output_file_base_name)
f.write(temp_str)
f.write("#ifdef INCLUDE_PIPTEST\n")

# Open the input image and get the data.
seq = list(im.getdata())
pixels_written = 0
pixels_written_to_line = 0

# Write out the image header.
f.write("0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n")
f.write("0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n")
f.write("0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n")

# Write the width in bytes and height (little-endian format).
width = "%08x" % (im.size[0]*4)
height = "%08x" % im.size[1]
temp_str = "0x{0}, 0x{1}, 0x{2}, 0x{3}, 0x{4}, 0x{5}, 0x{6}, 0x{7},\n".format(
           width[6:], width[4:6], width[2:4], width[:2], height[6:], height[4:6], height[2:4], height[:2])
f.write(temp_str)

# Write all of the pixels to the output file. 
for pixel in seq:
    blue = "0x%02x" % pixel[2]
    green = "0x%02x" % pixel[1]
    red = "0x%02x" % pixel[0]

    # Write out an xBGR pixel
    pixel_string = "0x00, {0}, {1}, {2}".format(blue, green, red)
    f.write(pixel_string)

    pixels_written += 1
    if pixels_written < len(seq):
        f.write(",")

    pixels_written_to_line += 1
    if pixels_written_to_line==2 or pixels_written==len(seq):
        f.write("\n");
        pixels_written_to_line = 0
    else:
        f.write(" ")

f.write("#endif\n")
f.write("};\n")
f.write("\n")
f.write("#endif\n")
f.write("\n")
f.close()

