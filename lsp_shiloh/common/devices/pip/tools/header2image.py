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
    print "Usage: 'python header2image.py <input header base name> <output filename>'\n"
    exit(1)

# Open the input file.
input_file_base_name = sys.argv[1]
input_filename = input_file_base_name + "_rgb32.h"
fin = open(input_filename, 'rb')

# Read past the first three lines of unused image header bytes.
image_header_unused_line_count = 0
while (3 != image_header_unused_line_count):
    line_in = fin.readline()  
    if ("" == line_in):
        print "Image header not detected in input file, exiting."
        exit(1)
    if ("0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00," == line_in.rstrip('\r\n')):
        image_header_unused_line_count += 1

# Parse the width and height from the image header.
line_in = fin.readline()
if ("" == line_in):
    print "Width and height not detected in input image header, exiting."
    exit(1)
width_and_height_byte_list = (line_in.rstrip(',\r\n')).split(', ')
if (8 != len(width_and_height_byte_list)):
    print "Unexpected input image header format, exiting."
    exit(1)
width_in_bytes = int(width_and_height_byte_list[3],16)*2**24 + int(width_and_height_byte_list[2],16)*2**16 + int(width_and_height_byte_list[1],16)*2**8 + int(width_and_height_byte_list[0],16)
pixels_per_line = width_in_bytes / 4
height = int(width_and_height_byte_list[7],16)*2**24 + int(width_and_height_byte_list[6],16)*2**16 + int(width_and_height_byte_list[5],16)*2**8 + int(width_and_height_byte_list[4],16)

print "width in bytes = ", width_in_bytes
print "height = ", height

# Loop through input image hex data and save it to a list of pixels.
image_bytes_read = 0
image_data = []
while ((width_in_bytes*height) != image_bytes_read):
    # Read a line of hex image data.
    line_in = fin.readline()  
    if ("" == line_in):
        print "Image data smaller than expected, exiting."
        exit(1)
    image_line_bytes = (line_in.rstrip(',\r\n')).split(', ')
    if (8 != len(image_line_bytes)):
        print "Unexpected input image data format, exiting."
        exit(1)
    image_bytes_read +=8

    # Parse the first pixel as a tuple and add it to the list.
    blue = int(image_line_bytes[1],16)
    green = int(image_line_bytes[2],16)
    red = int(image_line_bytes[3],16)
    pixel = (red, green, blue)
    image_data.append(pixel)

    # Parse the second pixel and add it to the list.
    blue = int(image_line_bytes[5],16)
    green = int(image_line_bytes[6],16)
    red = int(image_line_bytes[7],16)
    pixel = (red, green, blue)
    image_data.append(pixel)

# Create an Image, add the data do it, and save it to a file.
im = Image.new("RGB", (pixels_per_line,height))
im.putdata(image_data)
im.save(sys.argv[2])

fin.close()

