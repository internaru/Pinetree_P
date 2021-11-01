#
# Copyright (c) 2013  Marvell International, Ltd. All Rights Reserved
#                         Marvell Confidential
#
# Author: Stefan Kemper
# 
# This ruby script is a tool to assist in computing reasonable inlut and ucell values for the
# Marvell threshold block.
#
#
# todo: 
# 1) currently assumes 2bit, make a 1bit variant.

# offset: computation 
# solve this to find offset assuming 512 as start and 5 bits of random stocastic and 2 bpp
# (inlut - (offset * 32))/32 = 4
# (512 - 4*32) /32 = 12  
# solve this to find offset assuming 512 as start and 5 bits of random stocastic and 1 bpp
# (inlut - (offset * 32))/32 = 8
# (512 - 8*32) /32 = 8  



## options ##
verbose = false
print_usefull = false
bpp = 2

## end options ##


# copy the halliday micro cell definition from design here: 
# assumes 1..(num_columns*num_rows) numbering of cells.


if bpp == 2 then
  width = 6
  height = 3
  ucell_orig = [ 
                0x00000001, 0x00000003, 0x0000000B, 0x0000000f, 0x00000012, 0x00f10006,
                0x00000002, 0x00000004, 0x0000000c, 0x00000010, 0x00000011, 0x00f10008,
                0x1000000d, 0x1000000e, 0x01f10009, 0x01000005, 0x01000007, 0x1000000a,
               ]
else
  width = 8
  height = 4
  ucell_orig = [
                0x00000010, 0x00000011, 0x0000001B, 0x00000017, 0x00000012, 0x00F1000E, 0x0000000B, 0x00000007, 
                0x10000014, 0x01F1000D, 0x01000005, 0x01000009, 0x0100000F, 0x10000013, 0x10000015, 0x10000019, 
                0x00000016, 0x00F1000C, 0x00000001, 0x00000003, 0x00000006, 0x0000001A, 0x0000001D, 0x0000001F,
                0x0000001C, 0x00F10008, 0x00000002, 0x00000004, 0x0000000A, 0x00000018, 0x0000001E, 0x00000020, 
               ]
end

num = width * height    # num pixels in halftone halliday matrix 
num_concatbits = 5 # 32 levels of stocastic, 
first_inlut = 512 # must be above 256 to allow threshold_low stocatic in black region.

offset = bpp == 1 ? 0 : 12  # ((first inlut level 256 or 512) - 4**num_concatbits) / 4**num_concat_bits
min_scale = ((15.0*2**num_concatbits + 2**num_concatbits*offset) - 4*2**num_concatbits) / 2**num_concatbits - 12
print "min_scale " + min_scale.to_s + "\n" 
scale = bpp == 1 ? 1 : 16  # 11 from line above works but 16 gives nice round power of 2 numbers.
# not doing stochastic in white regions works best if we can hold a single dot.
threshold_lo_offset = 0
# This is used to choose a minimum hole size which will be 
# randomly/stochastically dithered in dark regions
threshold_high_offset = num-1 # use all values 
if bpp == 1 then 
  threshold_high_offset = 0x19# 19 is a very visible hole 1bit 106lpi #  
else
  threshold_high_offset = 7 # 
end 
## end input config ## 

# linear would be used if no pixels where grouped.
# 
linear_offset = Array.new(num) { |i| offset + scale*i }
print "\n linear_offset :"  # evenly spaced 
print linear_offset 

# no grouping 
scale_offset = linear_offset 
# hand edited allows pixel grouping...
# scale_offset = [12, 28, 44, 60, 76, 92, 108, 124, 140, 156, 172, 188, 204, 220, 236, 252, 268, 284]
# scale_offset = [12, 13, 14, 15, 76, 92, 108, 124, 140, 156, 172, 188, 204, 220, 236, 252, 268, 269]
#                 --------------
# these pixels grow ^^^^^^^^ together  so all will get 1/3 value before any gets a 2/3 value.
# scale of 16 will allow upto 4 pixels to be grouped.

# the original ucell table will now have a scale and offset applyed to the what was a seqential ordering.
print "\n scale_offset :"
print scale_offset

# last white pixel and last black pixel
threshold_lo = ( 2**num_concatbits * ( 4 + scale_offset[threshold_lo_offset] )) + (2**num_concatbits -1)
print bpp
pixel = 8 / bpp 
print pixel
threshold_high = pixel * 2**num_concatbits + 2**num_concatbits * scale_offset[threshold_high_offset] 
print "\n threshold lo " + threshold_lo.to_s + " threshold high " + threshold_high.to_s + "\n"

# compute the inlut value with for 1/3 2/3 and 3/3 on pixel values.  1100 top 2 bits for 2bit 1000 top bit for 1 bit.
# each ucell_data pixel will have 3 on values: 0100 1000 1100
# these are shifted over to allow stochastic for 5 bits of random:  1100xxxxx 
# then these are offset by the scale_offset this same scale_offset is applied to the ucell_data 
# in the end we will be comparing the ucell_data pixel value against the inlut value. 
inlut = []
if bpp == 2 then 
  (0..(num -1)).each { |i| 
    inlut <<  ( 4 * 2**num_concatbits + 2**num_concatbits * scale_offset[i] )
    inlut <<  ( 8 * 2**num_concatbits + 2**num_concatbits * scale_offset[i] )
    inlut <<  (12 * 2**num_concatbits + 2**num_concatbits * scale_offset[i] )
  }
else # bpp == 1
  (0..(num -1)).each { |i| 
    inlut <<  ( 8 * 2**num_concatbits + 2**num_concatbits * scale_offset[i] )
  }
end

inlut = inlut.sort # sort is needed if pixels are grouped

low = inlut
no_noise = Array.new(low.size) {|i| low[i] + 2**num_concatbits - 1 }

print "\n begin inlut values [" + inlut.size.to_s + "]:\n"
print inlut

print "\n no stocastic noise pure halftones:\n"
print no_noise



# all possilble different inlut values 0..2**stochastic_bits added to each inlut starting value.
# so we have a linear region with a flat no change inbetween 
all = []
low[0..-1].each { |v| (0..31).each { |val| all << v + val } }

# trim the "usefull" since we will not be able to print partial on pixels in isolation
# or worse a white hole in a black region will fill in.
usefull = [] 
all.each {|v| usefull << v  if v >= threshold_lo && v < threshold_high }


# verbose dump. of all useful inlut values 
if print_usefull then  
  print "\n usefull inlut values : + [" + usefull.length.to_s + "]\n"
  print usefull
end


if verbose then 
  # dumping usefull in groups of 256 for measurement of tone values.
  print "\n usefull_1 = [ "
  usefull[0..(256-2)].each { |v| printf( "%4d, ", v )}
  print " 0xffff, ] \n"
  print "\n usefull_2 = [ "
  usefull[(256-1)..(256*2-2)].each { |v| printf( "%4d, ", v )}
  print " 0xffff, ] \n"
  print "\n usefull_3 = [ "
  usefull[(256*2-1)..(256*3-2)].each { |v| printf( "%4d, ", v )}
  print " 0xffff, ] \n"
  # print "\n usefull_4 = [ "
  # usefull[(256*3-1)..(256*4-2)].each { |v| printf( "%4d, ", v )}
  # print " 0xffff, ] \n"

  #print "\n usefull_5 = [ "
  #usefull[(256*4-1)..(256*5-2)].each { |v| printf( "%4d, ", v )}
  # print " 0xffff, ] \n"
  # print "\n usefull_6 = [ "
  # usefull[(256*6-1)..(256*7-2)].each { |v| printf( "%4d, ", v )}
  # print " 0xffff, ] \n"
end


# build an example lut_data 
# actual table should use measured density values from paper to 
# choose halftone values from the usefull array. This is a swag first pass.
range = threshold_high - threshold_lo
skip = range / 254
skip = (usefull.length / 254.0 ).round
end_fill = 254 - (usefull.length / skip) 
print "skip " + skip.to_s + " end_fill " + end_fill.to_s + "\n" if verbose

table_idx_usefull = Array.new(256) do |i| 
  if i <= 1 then 
    i
  elsif i > 1 && i < 256 - end_fill then
    (i -1) * skip 
  else
    0xfe00 + ((i-(256-end_fill)) * 0x1ff / end_fill) / 2  # /2 lets try not going so dark
  end
end
# print "\n table_idx_usefull :"
# print table_idx_usefull 

table_new = Array.new(256) { |i| 
  table_idx_usefull[i] < usefull.length ? usefull[table_idx_usefull[i]] : table_idx_usefull[i] 
 }
table_new[255] = 0xffff

# print out example table to map 0..255 gray to halftone values.
print "\n new threshold table configuration data :\n .lut_data = {\n    0, "
(1..255).each { |i| 
  printf("%5d, ", table_new[i] )
  print "\n" if (i % 16) == 15
}
print " },\n"

# offset and scale the pixels to match the lut data.
ucell = Array.new(num) { |i| 
  if bpp == 2 then
    0xffff0000 & ucell_orig[i] | scale_offset[ (ucell_orig[i] & 0xff) - 1] 
  else
    ucell_orig[i]
  end
}
print "\n .ucell_data = {\n"
ucell.each_index {|i| 
  printf(" 0x%08x,", ucell[i])
  print "\n" if (i % width) == (width - 1) 
 }
print " },\n"

print " .thresh_hi = " + threshold_high.to_s + ",\n"
print " .thresh_lo = " + (table_new[1] -1).to_s + ",\n"

