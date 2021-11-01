#!/usr/bin/python
#******************************************************************************
# * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
# *
# *                         Marvell Confidential
#******************************************************************************


import sys


# Array storing input (x-axis) sample points and corresponding output (y-axis) values. Linear interpolation
# is performed between sample points.
xsamp = [  0,  63, 127, 191, 255, 319, 383, 447, 511, 575, 639, 703, 767, 831, 895, 959, 1023, 1024]
ysamp = [560, 526, 490, 455, 420, 385, 350, 315, 280, 245, 210, 175, 140, 105,  70,  35,    0,    0]

##         16   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0 
#xsamp = [  0,  20,  68, 100, 148, 192, 232, 296, 356, 424, 484, 552, 616, 712, 812, 936, 1023, 1024]
##ysamp = [560, 549, 523, 505, 479, 455, 433, 397, 365, 328, 295, 258, 223, 170, 116,  48,    0,    0]
##ysamp = [560, 544, 518, 500, 474, 450, 428, 391, 358, 310, 288, 215, 190, 180, 140,  78,    0,    0]
#ysamp = [560, 544, 518, 500, 474, 450, 428, 391, 358, 310, 288, 215, 190, 175, 150,  80,    0,    0]   # -- Good


# Write out the file header
def write_header(f):
    f.write("/******************************************************************************\n")
    f.write(" * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved\n")
    f.write(" *\n")
    f.write(" *                         Marvell Confidential\n")
    f.write(" ******************************************************************************/\n")
    f.write("\n")
    f.write("#ifndef INC_INV1D_RAMP_CLIPPED_H\n")
    f.write("#define INC_INV1D_RAMP_CLIPPED_H\n")
    #f.write("#ifndef INC_1D_RAMP_CLIPPED_H\n")
    #f.write("#define INC_1D_RAMP_CLIPPED_H\n")
    f.write("\n")
    f.write("uint32_t PA_inv1d_ramp_clipped[] =\n")
    #f.write("uint32_t PA_1d_ramp_clipped[] =\n")
    f.write("{\n")
    return

def write_footer(f):
    f.write("};\n")
    f.write("\n")
    f.write("#endif\n")

# Calculate the rise for the given section
def calc_rise(section):
    rise = ysamp[section+1] - ysamp[section]
    return rise

# Calculate the run for the given section
def calc_run(section):
    run = xsamp[section+1] - xsamp[section]
    return run

# Calculate the yint for the given section
def calc_yint(section, rise, run):
    yint = ysamp[section] - (rise*xsamp[section])/run
    return yint

# Main function
def main():
    # Open the file to write to
    fout = open(sys.argv[1], "wb")

    write_header(fout)

    section = 0
    rise = calc_rise(section)
    run = calc_run(section)
    yint = ysamp[0]

    for in_val in range(1024):
        if in_val > xsamp[section+1]:
            section += 1
            rise = calc_rise(section)
            run = calc_run(section)
            yint = calc_yint(section, rise, run)

        out_val = ((rise*in_val)/run) + yint
        if out_val < 0:
            out_val = 0
#        out_val = 1023 - out_val
        print out_val
        str_out = "0x%08x,\n" % out_val
        fout.write( str_out )

    write_footer(fout)

if __name__ == '__main__':
    main()

