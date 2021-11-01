#!/usr/bin/python
#
#****************************************************************************** 
# * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
# *
# *                         Marvell Confidential
#******************************************************************************/


# Write out the file header
def write_header(f):
    f.write("/******************************************************************************\n");
    f.write("* Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved\n");
    f.write("*\n");
    f.write("*                         Marvell Confidential\n");
    f.write("******************************************************************************/\n");
    f.write("\n");
    f.write("#ifndef SCCSC_4CH_8BIT_COLORMAP_H\n");
    f.write("#define SCCSC_4CH_8BIT_COLORMAP_H\n");
    f.write("\n");
    f.write("#include <stdint.h>\n");
    f.write("\n");
    f.write("\n// **** Array color order is C M Y K****\n\n");
    f.write("#ifdef __cplusplus\n");
    f.write("extern \"C\" {\n");
    f.write("#endif\n");
    f.write("\n");
    f.write("uint32_t sccsc_4ch_8bit_colormap[] =\n");
    f.write("{\n");
    return

# Write out the file header
def write_footer(f):
    f.write("};\n");
    f.write("\n");
    f.write("#ifdef __cplusplus\n");
    f.write("}\n");
    f.write("#endif\n");
    f.write("\n");
    f.write("#endif    // SCCSC_4CH_8BIT_COLORMAP_H\n"); 
    f.write("\n");
    return

# Main function
def main():
    # Open the file to write to
    fout = open("sccsc_4ch_8bit_colormap.h", "wb")
    
    write_header(fout)

    for i in range(17):
        for j in range(17):
            for k in range(17):
                red = i / 16.0
                green = j / 16.0
                blue = k / 16.0
            
                cyan = 1.0 - red
                magenta = 1.0 - green
                yellow = 1.0 - blue
            
                black = min(cyan, magenta, yellow)
            
                if (black < 1.0):
                    cyan = (cyan-black) / (1.0-black)
                    magenta = (magenta-black) / (1.0-black)
                    yellow = (yellow-black) / (1.0-black)
                else:
                    cyan = 0
                    magenta = 0
                    yellow = 0

                cval = int(cyan*255.0)
                mval = int(magenta*255.0)
                yval = int(yellow*255.0)
                kval = int(black*255.0)

                out32 = 0xffffffff & ((cval<<24) |
                                      (mval<<16) |
                                      (yval<<8) |
                                      (kval<<0))
                out_val = "    0x%08x, // R 0x%02x, G 0x%02x, B 0x%02x\n" % (out32, int(red*255), int(green*255), int(blue*255))
                fout.write(out_val)

    write_footer(fout)
    fout.close()
    return

if __name__ == '__main__':
    main()

