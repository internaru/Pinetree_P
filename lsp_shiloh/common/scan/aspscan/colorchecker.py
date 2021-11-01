#!/usr/bin/python
#
# ============================================================================
# Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
# 
#                         Marvell Confidential
# ============================================================================
# 


# ColorChecker chart values as a python script for instant access
# xyY values via back of chart
# davep 23-May-2007

# color is part of my pytools directory
import color

# x y Y values for samples
chart = [
    { "xyY": ( 0.400, 0.350, 10.1 ), "name": "dark skin" },
    { "xyY": ( 0.377, 0.345, 35.8 ), "name": "light skin" },
    { "xyY": ( 0.247, 0.251, 19.3 ), "name": "blue sky" },
    { "xyY": ( 0.337, 0.422, 13.3 ), "name": "foliage" },
    { "xyY": ( 0.265, 0.240, 24.3 ), "name": "blue flower" },
    { "xyY": ( 0.261, 0.343, 43.1 ), "name": "bluish green" },
    { "xyY": ( 0.506, 0.407, 30.1 ), "name": "orange" },
    { "xyY": ( 0.211, 0.175, 12.0 ), "name": "purplish blue" },
    { "xyY": ( 0.453, 0.306, 19.8 ), "name": "moderate red" },
    { "xyY": ( 0.285, 0.202,  6.6 ), "name": "purple" },
    { "xyY": ( 0.380, 0.489, 44.3 ), "name": "yellow green" },
    { "xyY": ( 0.473, 0.438, 43.1 ), "name": "orange yellow" },
    { "xyY": ( 0.187, 0.129,  6.1 ), "name": "blue" },
    { "xyY": ( 0.305, 0.478, 23.4 ), "name": "green" },
    { "xyY": ( 0.539, 0.313, 12.0 ), "name": "red" },
    { "xyY": ( 0.448, 0.470, 59.1 ), "name": "yellow" },
    { "xyY": ( 0.364, 0.233, 19.8 ), "name": "magenta" },
    { "xyY": ( 0.196, 0.252, 19.8 ), "name": "cyan" },
    { "xyY": ( 0.310, 0.316, 90.0 ), "name": "white" },
    { "xyY": ( 0.310, 0.316, 59.1 ), "name": "neutral 8" },
    { "xyY": ( 0.310, 0.316, 36.2 ), "name": "neutral 6.5" },
    { "xyY": ( 0.310, 0.316, 19.8 ), "name": "neutral 5" },
    { "xyY": ( 0.310, 0.316,  9.0 ), "name": "neutral 3.5" },
    { "xyY": ( 0.310, 0.316,  3.1 ), "name": "black" },
]

class ColorChecker :
    def __init__( self ) :
        self.chart = chart

        # make XYZ first so we can convert XYZ to the others
        self.add_XYZ()

        # calculate the RGB, Lab, and YCbCr values from our XYZ
        self.add_RGB()
        self.add_Lab()

        # can only convert to YCC from RGB so be sure RGB is done first
        self.add_YCC()

    def add_XYZ( self ) :
        for c in self.chart :
            c["XYZ"] = color.xyY_to_XYZ( c["xyY"] )

    def add_RGB( self ) :
        for c in self.chart :
            c["RGB"] = color.XYZ_to_RGB( c["XYZ"] )

    def add_Lab( self ) :
        for c in self.chart :
            c["Lab"] = color.XYZ_to_Lab( c["XYZ"] )

    def add_YCC( self ) :
        for c in self.chart :
            c["YCC"] = color.RGB_to_YCC( c["RGB"] )

def main() :
    colorchecker = ColorChecker()    
    for c in colorchecker.chart :
#        print c["name"], c["RGB"]
        print c["name"], c["Lab"]

if __name__ == '__main__' :
    main()

