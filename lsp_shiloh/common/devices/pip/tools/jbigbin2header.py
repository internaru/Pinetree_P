#!/usr/bin/python
#******************************************************************************
# * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
# *
# *                         Marvell Confidential
#******************************************************************************


import sys
import os
import struct


# Write out the file header
def write_header(f, name):
    f.write("/******************************************************************************\n")
    f.write(" * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved\n")
    f.write(" *\n")
    f.write(" *                         Marvell Confidential\n")
    f.write(" ******************************************************************************/\n")
    f.write("\n")
    str_out = "#ifndef INC_%s_JBIG_INPUT_H\n" % name.upper()
    f.write(str_out)
    str_out = "#define INC_%s_JBIG_INPUT_H\n" % name.upper()
    f.write(str_out)
    f.write("\n")
    str_out = "uint8_t %s_jbig_input[] =\n" % name
    f.write(str_out)
    f.write("{\n")
    return

# Write out the file footer
def write_footer(f):
    f.write("};\n")
    f.write("\n")
    f.write("#endif\n")

# Print the jbig header and image dimensions to the output
def print_header(header):
    # Print the header bytes
    print "Header:",
    for byte in range(19):
        str_out = "%02x" % int(header[byte].encode("hex"), 16)     
        print str_out,
    str_out = "%02x" % int(header[19].encode("hex"), 16)     
    print str_out

    # Print Xd
    temp = header[4:8]
    Xd = (struct.unpack('>I', temp))[0]
    print "Xd: ", Xd

    # Print Yd
    temp = header[8:12]
    Yd = (struct.unpack('>I', temp))[0]
    print "Yd: ", Yd

    # Print L0
    temp = header[12:16]
    L0 = (struct.unpack('>I', temp))[0]
    print "L0: ", L0

# Main function
def main():
    if len(sys.argv) != 3:
        print "Usage: python jbigbin2header.py <input file> <output file>"
        sys.exit(1)

    # Open the file to read from
    fin = open(sys.argv[1], "rb")

    # Open the file to write to
    fout = open(sys.argv[2], "wb")

    write_header(fout, os.path.splitext(sys.argv[2])[0])

    header_in = fin.read(20)
    print_header(header_in)

    loopCounter = 0
    next_byte_in = fin.read(1)
    while 1:
        byte_in = next_byte_in 
        if not byte_in:
            break
        str_out = "0x%02x" % int(byte_in.encode("hex"), 16)     
        fout.write(str_out)

        next_byte_in = fin.read(1)
        if next_byte_in:
            fout.write(",")

        if (7 == loopCounter%8):
            fout.write("\n")
        else:
            fout.write(" ")

        loopCounter += 1

    if (0 != loopCounter%8):
        fout.write( "\n" )

    fout.write( "\n" )

    write_footer(fout)

    # Close opened files
    fin.close()
    fout.close()


if __name__ == '__main__':
    main()
