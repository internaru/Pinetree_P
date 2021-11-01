#!/usr/bin/perl -w

#
# ============================================================================
# Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
#
#                         Marvell Confidential
# ============================================================================
#

# this takes an input zpjl symbol file and creates a zPJL_sym.<c,h> files
# for those symbols.  Note that by adding new symbols to this file you are not
# generating the code to handle them.  Note that white space will be compressed
# for example if your symbol name is "hello world" the output will be "helloworld"
#
use strict;
use Compress::Zlib;
use Getopt::Std;
use POSIX;
use Switch;
use File::stat;


use constant VERSION => "0.0";
my $verbose = 0;
#
# trim the leading and trailing whitespace from a string
#
sub trim($)
{
   my $string = shift;
   $string =~ s/\s+$//;
   $string =~ s/^\s+//;
   return $string;
}
#
# help usage
#
sub usage
{
   print "$0 <filename> to execute this program\n";
   print "\tfilename is a list of pjl symbols to process.  see zPJL_sym.smg for examplen";
   print "\t-h\tprint the help file\n";
   print "\t-v\t verbose print out lots of stuff\n";
}
# start of the main routine.

# parse the arguments

    my %opts;
    my $retcode = getopts( 'o:hv', \%opts ) or usage();
    if( ! $retcode ) {
        die "Error! Incorrect command line. Try \"$0 -h\" for help.\n";
        exit 1;
    }

    if( exists $opts{'h'} ) {
        usage();
        exit 1;
    }

    # were we given the ini file?
    my @file_list = @ARGV;


    # verbose is optional but we want to return the global level if it wasn't passed
#    $verbose = 1;   # uncomment this to turn on verbose always
    if( defined $opts{'v'} ) {
        $verbose = 1;
    }


    open FH, "@file_list"  or die "Error! Could not open ".@file_list." for reading: $!\n";
    open FHOutC, ">zPJL_sym.c" or die "Error! Could not open zPJL_sym.c $!\n";
    open FHOutH, ">zPJL_sym.h" or die "Error! Could not open zPJL_sym.h $!\n";
    #
    # put in the copyright headers for the files we will make
    #
    print FHOutC "/*===================================================\n";
    print FHOutC "*\t(C) Copyright 2010   Marvell International Ltd.\n";
    print FHOutC "*\t\tAll Rights Reserved\n";
    print FHOutC "*\n";
    print FHOutC "*\t\tMarvell Confidential\n";
    print FHOutC "=====================================================\n*/\n";
    print FHOutC "const char*  __SYMBOL_TABLE__[] =\n{\n";  # start the array

    print FHOutH "/*===================================================\n";
    print FHOutH "*\t(C) Copyright 2010   Marvell International Ltd.\n";
    print FHOutH "*\t\tAll Rights Reserved\n";
    print FHOutH "*\n";
    print FHOutH "*\t\tMarvell Confidential\n";
    print FHOutH "=====================================================\n*/\n";
    print FHOutH "#ifndef __zPJL_sym_h__\n#define __zPJL_sym_h__\n\n";
    print FHOutH "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n"; # c++ headers

    my $count = 0;   # initialize the count to 0 used for header file counts
    my @unline = <FH>;

    my @linearray = sort( @unline);

    foreach (@linearray) # for each line in the file
    {
       my $line = trim($_);   # get rid of leading and trailing whitespace.
       my $len = length($line);

       if($len > 0)   # if we still have something left, go in here.
       {
          if((my $loc = index($line,"//"))>= 0) # see if this has a comment
          {
             print "found a comment $line\n" if $verbose;
             $line = substr($line, 0, $loc); # strip out everything after the comment.
          }
          $len = length($line);
          if($len > 0)  # if there is still something left go into here.
          {
             $line =~ s/\s//g;   # get rid of all the rest of the whitespace.
             $len = length($line);
             if($len > 0)  # see if there is something left.
             {
                print "Output string $line count = $count\n" if $verbose;
                # output the string to the c file.
                print FHOutC "\t\"$line\",\t\t/*\t$count */ \n";
                # some of the strings have a ~ which is turned into an _ in the .h file
                # not sure what this is used for.
                #
                $line =~ s/~/_/g;   # change ~ to _
                print FHOutH "#define IDS_$line\t\t\t$count\n";
                $count++;  # increment the count

             }
          }
       }

    }
    close FH;
    # now finish the files.
    print FHOutC "\t((void *)0L)\n};\n";
    print FHOutH "#define __DEFAULT_SYMBOL_TABLE_SIZE__\t$count\n\n";
    print FHOutH "#ifdef __cplusplus\n}\n#endif\n\n#endif\n";
    # close all the open file handles.
    close FHOutC;
    close FHOutH;
