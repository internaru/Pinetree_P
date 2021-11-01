#!/usr/bin/perl -w

#/*
# * ============================================================================
# * Copyright (c) 2008   Marvell International, Ltd. All Rights Reserved
# *
# *                         Marvell Confidential
# * ============================================================================
# */

# duplicate makefwversion.exe (from CreateDate.cpp) in Perl
# davep 19-Mar-2007
# dbodily 6-may-08
# added code to add the build info string.  This assumes perforce in on your machine, you
# have logged in using "p4 login" command, you have set your client by the "p4 set P4CLIENT=<your workspace>"
# command.  

use strict;
use POSIX;

my $debug=0;
my $parse = 'code';
my $Posix_Start_Year = 1900; #  +1900 because year starts at 100

#
# extract the change number from the string that is passed.
# the 2nd field of the string is the number.  So look for
# a space and then the 2nd space and return that value.
#
sub extractChangeNum
{
   #
   # now extract the change number from the string.
   my $val=shift(@_);
   my $start = index($val, ' ')+1;  # Find the first space in the strin
   $val = substr($val, $start, length($val));   # get the string from first space on.
   # get the string from beginning to the first space.
   my $changelist = substr($val, 0,index($val, ' ')); #now I have the # in change list
   return $changelist;  # return that number.
}
#
# Find the change list number and return it.
#
sub getChangelistFromWorkspace 
{
   my $failval = 'NA';
   my $PathPwd = `pwd`;
   # trim off end of string
   $PathPwd = substr($PathPwd,0,length($PathPwd)-1);

   my $client = `p4 info` || return $failval;    # the command to execute.
   my $changelist = 0;

   print "client First: $client\n" if $debug;
   my $settings = `p4 set`;
   print "setting $settings\n" if $debug;
   print "client First: $client\n" if $debug;
   #  Now extract the workspace info from the p4 info command.
   $client = substr($client, index($client, 'Client name: ')+length('Client name: '),
                    index($client, 'Client host') - index($client, 'Client name:') - length('Client name: ')-2);
   print "client Parse: $client\n" if $debug;
   my $path = `pwd`;    # get the directory to find the changelist number
   # Assume that from firmware on we have the same directory naming as perforce.
   # temp after this contains the path from firmare on.
   my $temp = substr($path, index($path, $parse), length($path)); # get rid of the extra stuff at the start.
   # now build the command, we will look at the bootcode directoy and then
   # look at the include and take the biggest number for the changelist.
   # First do the bootcode directory
   #
   my $Have_changes = `p4 changes -m1 //...#have`; # the command to execute
   print "Have_Changes $Have_changes\n" if $debug;   
   my $Havechangelist = extractChangeNum($Have_changes);  # get the changelist number
   #Next line availaboe for debugging
   print "Havechangelist $Havechangelist\n" if $debug;   
   #put the have changelist.
   $changelist = $Havechangelist;
   #$changelist .= ","; #appending ,
   # $changelist .= $Depotchangelist;  # appending depot change list number
   # print "changelsit $changelist\n" if $debug;   
   return $changelist;
}
# lifted straight from CreateDate.cpp then tweaked to remove old PVCS path from
# $Header
my $header=' 
/*******************************************************************************
 * Copyright (c) %s  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 * \file fw_version.c
 *
 */
// BEWARE!  The FIRMWARE_VERSION_STRING MUST remain 8 characters long!

char FIRMWARE_VERSION_STRING[]={"%s"};
char BUILD_INFO_STRING[]={"%s"};

char * Get_Firmware_Version_String(void)
{
   return (&FIRMWARE_VERSION_STRING[0]);
}
char * Get_Build_Info_String(void)
{
   return (&BUILD_INFO_STRING[0]);
}

%s
';

my $header2='
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#include "lassert.h"

static char *fw_version_cmd_desc = "Display the version information of the system";
static char *fw_version_cmd_usage = NULL;
static char *fw_version_cmd_notes = NULL;
static int fw_version_cmd_cb( int argc, char *argv[] )
{
    char *version_string;
    char *build_info_string;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    version_string = Get_Firmware_Version_String();
    build_info_string = Get_Build_Info_String();

    if ( ( NULL == build_info_string ) || ( NULL == version_string ) )
    {
        cmd_printf("error reading fw string\n");
        return CMD_ERROR;
    }

    cmd_printf("FW VERSION: %s\n", version_string);
    cmd_printf("FW BUILD STRING: %s\n", build_info_string);

    return CMD_OK;
}
#endif

void fw_version_init( void )
{
#ifdef HAVE_CMD
    int cmd_res = -1;

    cmd_res = cmd_register_cmd( "version",
                                "fwdate", /* legacy support */
                                fw_version_cmd_desc,
                                fw_version_cmd_usage,
                                fw_version_cmd_notes,
                                fw_version_cmd_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = CMD_OK;
#endif
}

/*
 * End of fw_version.c
 */
';
my $version_info_str="-1";
my($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) =
                                                    localtime();
# +1900 because year starts at 100; +1 because month starts at 0
my $fwversion = sprintf( "%04d%02d%02d", $year+$Posix_Start_Year, $mon+1, $mday );
$fwversion .= "$version_info_str";
print "fw version $fwversion\n" if $debug;

my $build_info_string="";
# build a  string like:
# "TIME=Mon Mar 19 13:16:40 2007 dangerousradish -- PROD=MY_PRODUCT CFG=GCC_DEBUG"

my $time_str = POSIX::strftime( "%a %b %d %H:%M:%S %Y", $sec, $min, $hour, $mday, $mon, $year );
$build_info_string .= "TIME=$time_str";

for my $arg ( @ARGV ) {
 #   print "arg $arg\n" if $debug;

# the following comment block lifted straight from CreateDate.cpp
#        // Generally this program is called from a makefile
#        // and passed the contents of the MAKEFLAGS variable.
#        // It appears that xt-make is defining both RM_R and RM
#        // on the command line and hence this unwanted text
#        // gets into MAKEFLAGS.  So the following statement
#        // spots these interlopers and removes them.  The operator
#        // can invoke make with arbitrary text on the command
#        // line as long as that arbitrary text remains legal to
#        // make.  About the only way to accomplish this is to
#        // make the arbitrary text look like legal variable definitions,
#        // such as is shown below:
#        //    xt-make CFG=GCC_RELEASE PROD=FIR special=build=for=tuesday

    next if( $arg =~ /^RM/ ); 

    # Change lone backslashes to escaped backslashes (e.g., c:\src\build
    # changed to c:\\src\\build) ; leave already escaped strings alone
    # by turning \\ into \ then back into \\.
    print "arg ", $arg, " -> " if $debug;
    $arg =~ s/\\\\/\\/g;
    $arg =~ s/\\/\\\\/g;
    print "arg ", $arg,"\n" if $debug;

    $build_info_string .= " " . $arg;
}
#$build_info_string .= ";CHGLST=";
#$build_info_string .= getChangelistFromWorkspace;
my $outputc = sprintf( $header, $year+$Posix_Start_Year, $fwversion, $build_info_string, $header2 );
print $outputc if $debug;

open( FH, ">fw_version.c" ) || die "could not open fw_version.c for writing: $!";
print FH "$outputc\n";
close( FH );
#
# end of makefwversion.pl
#

