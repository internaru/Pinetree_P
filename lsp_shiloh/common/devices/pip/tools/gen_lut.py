#!/usr/bin/python
#******************************************************************************
# * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
# *
# *                         Marvell Confidential
#******************************************************************************


import sys

# Write out the file header
def write_header(f, name, bytes_per_entry):
    f.write("/******************************************************************************\n")
    f.write(" * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved\n")
    f.write(" *\n")
    f.write(" *                         Marvell Confidential\n")
    f.write(" ******************************************************************************/\n")
    f.write("\n")
    temp_str = "#ifndef {0}_H\n".format(name.upper())
    f.write(temp_str)
    temp_str = "#define {0}_H\n".format(name.upper())
    f.write(temp_str)
    f.write("\n")
    if 1 == bytes_per_entry:
        temp_str = "uint8_t {0}[] =\n".format(name)
    if 2 == bytes_per_entry:
        temp_str = "uint16_t {0}[] =\n".format(name)
    if 4 == bytes_per_entry:
        temp_str = "uint32_t {0}[] =\n".format(name)
    f.write(temp_str)
    f.write("{\n")
    return
    
# Write out the file footer
def write_footer(f):
    f.write("};\n")
    f.write("\n")
    f.write("#endif\n")

# Main function
def main():
    if len(sys.argv) != 5:
        print "Usage: 'python gen_lut.py <output file base name> <num entries> <entry value> <bytes_per_entry>'\n"
        exit(1)

    # Parse input args.
    output_file_base_name = sys.argv[1]
    num_entries = int(sys.argv[2])
    entry_value = int(sys.argv[3])
    bytes_per_entry = int(sys.argv[4])

    # Open the output file.
    header_filename = output_file_base_name + ".h"
    fout = open(header_filename, 'wb')

    # Write the output file header.
    write_header(fout, output_file_base_name, bytes_per_entry)

    # Write out the LUT entries.
    for entry in range(num_entries):
        if 1 == bytes_per_entry:
            str_out = "    0x%02x,\n" % entry_value
        if 2 == bytes_per_entry:
            str_out = "    0x%04x,\n" % entry_value
        if 4 == bytes_per_entry:
            str_out = "    0x%08x,\n" % entry_value
        fout.write( str_out )

    # Write the output file footer.
    write_footer(fout)

if __name__ == '__main__':
    main()
