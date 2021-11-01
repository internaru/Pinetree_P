#!/usr/bin/perl

#
# ============================================================================
# Copyright (c) 2009-2012 Marvell International, Ltd. All Rights Reserved
#
#                         Marvell Confidential
# ============================================================================
#

use warnings;
use strict;
use Getopt::Long;

my $BLOBBER_VERSION = "20121112";

# Should the blob file output be big endian or little endian....
my $big_endian_blobs = 1;

my $verbose_output = 0;

#
# Perl script to read in & parse a string configuration file and create the desired
# output files for each supported language.
# Usage:
#  perl string_blobber.pl string_mgr_config.h > blobEnglish.c

#
# definitions
# 
use constant false => 0;
use constant true  => 1;

use constant LOADRES_OK   => 0;
use constant LOADRES_FAIL => 1;

use constant BLOB_C         => 0; #static char tables
use constant BLOB_BINARY_FW => 1; #binary compressable format for printer fw
use constant BLOB_PC_CPP    => 2; #binary format for PC

use constant LANGCAT_PC  => 0x00000001;    #language used on PC
use constant LANGCAT_FA  => 0x00000002;    #firmware language, all strings localized
use constant LANGCAT_FP  => 0x00000004;    #firmware language, only some strings supported

#
# file scope variables
# 
my $fatalError = false;
my $m_NumLanguages = 0;
my $m_NumTables = 0;
my $m_NumEnums = 0;
my @m_LangInfo;
my @m_TableInfo;
my @m_EnumInfo;

my $path;
my %argOptions = ();
#
# functions
# 

#
# Utility function to trim leading whitespace.
# 
sub TrimLeft {
   $_[0] =~ s/^\s+//; # Replace leading whitespace with nothing
}

#
# Utility function to trim trailing whitespace.
# 
sub TrimRight {
   $_[0] =~ s/\s+$//; # Replace trailing whitespace with nothing
}

#
# Function to read in & parse the configuration file and create the desired output
# files for each supported language.
# 
sub OnButtonGenerate {
   my $res = LOADRES_OK;
   my $index;

   # The configuration file has all the information we need to generate our localization
   # strings for the string manager. 
   #  
   print "Reading configuration file ...\n" if $verbose_output;
   $res = &LoadStrProduct;
   print "Done reading configuration file\n" if $verbose_output;

   if($res != LOADRES_OK) {
      $fatalError = true;
      print STDERR "Error reading configuration file\n";
      return;
   }
   else {
      print "Successful reading configuration file\n" if $verbose_output;
   }

   foreach $index (0..($m_NumLanguages-1)){
      print "\nProcessing: $m_LangInfo[$index]{langEnumName}\n" if $verbose_output;

      #see if we have enough info to localize this language
      if (!(&CanStartLocalizeLang($index)))
      {
         $fatalError = true;
         print STDERR "Can't localize not enough info.\n";
         next;
      }

      CleanUp( $index );

      print "Reading input files...\n" if $verbose_output;
      
      #read the strings from the localized file, if error skip to next language
      #English is the standard language, we don't update from the localization tag
      #file, instead we use the DEF: string from the config file as the string.
      #UpdateTagStrings handles this for us
      $res = &UPdateTagStrings($index);
      if ( $res != LOADRES_OK ) 
      {
         print "Failed to find/parse translation file: " . $m_LangInfo[$index]{translations} . "\n";
      }
      
      if($index == 0)
      {
         print "Generating localization files\n" if $verbose_output;
         &CreateLocalizationStringList();
      }
      
      print "Creating output files...\n" if $verbose_output;
      
      #go through and do string parsing before creating the blobs.
      #convert \n to 0x0A, \xNN to 0xNN etc
      &PreProcessStrings();
      
      #create static C tables that can be compiled into firmware
      $res = &CreateBlob(BLOB_C, $index);
      if ($res == LOADRES_OK) {
         print "Blob C file created\n" if $verbose_output;
      }
      
      $res = &CreateBlob(BLOB_BINARY_FW, $index);
      if($res == LOADRES_OK)
      {
          print "Blob FW file created\n" if $verbose_output;
      }
      print "Done processing: $m_LangInfo[$index]{langEnumName}\n" if $verbose_output;
   }
}


sub CleanUp
{
    my $langIndex = shift;
    my $i;

    #clean up all the data structures
    #go through enum struct and empty the enumString
    foreach $i (0..($m_NumEnums-1))
    {
        $m_EnumInfo[$i]{enumString} = undef;
        $m_EnumInfo[$i]{haveString} = false;
        $m_EnumInfo[$i]{foundTag}   = false;
        $m_EnumInfo[$i]{haveEnum}   = false;
    }
}


sub CreateLocalizationStringList
{
    my $filename;
    $filename = $path . "fwEnglishStrings.txt";
    open OUTPUT_FILE, '>', $filename or die "Open of writable file $filename failed: $!"; 

    foreach my $enumIndex (0..($m_NumEnums-1)) 
    {
        if ( ( $m_EnumInfo[$enumIndex]{haveTag} == true ) &&
             ( index( $m_EnumInfo[$enumIndex]{enumTag}, "LH" ) != -1 ) )
        {
            printf OUTPUT_FILE "%s     \"%s\"\n", $m_EnumInfo[$enumIndex]{enumTag}, $m_EnumInfo[$enumIndex]{enumString};
        }
    }


    close OUTPUT_FILE;
}


#
# Function to read in & parse the configuration file.
# 
sub LoadStrProduct {
   my $STRPROD_START = 0;
   my $STRPROD_DOLANG = 1;
   my $STRPROD_DOTABLE = 2;
   my $STRPROD_DOENUM = 3;
   my $STRPROD_DOSIZES = 4;
   my $STRPROD_DONE = 5;

   my $state;
   my $nxtLine;
   my $index;
   my $lineNum = 0;
   my $res = LOADRES_OK;
   my $done;

   $state = $STRPROD_START;

   my $filename;
   my $done_with_translations;
   $filename = $path . "string_mgr_config.h";
   open DATA_FILE, $filename or die "Open of $filename failed: $!";

   $done_with_translations = false;

   while (!$done && ($nxtLine = <DATA_FILE>)) {
      $lineNum++;
      chomp($nxtLine);
      TrimLeft($nxtLine);
      TrimRight($nxtLine);

      if (length($nxtLine) == 0)
      {
         #skip empty lines
      }
      elsif ($state == $STRPROD_START) {
         if ($nxtLine =~ /#BEGIN/) {
            if ($nxtLine =~ /LANGUAGES/) {
               #print STDERR "Entering state STRPROD_DOLANG\n";
               $state = $STRPROD_DOLANG;
            }
            elsif ($nxtLine =~ /TABLES/) {
               #print STDERR "Entering state STRPROD_DOTABLE\n";
               $state = $STRPROD_DOTABLE;
            }
            elsif ($nxtLine =~ /ENUMS/) {
               #print STDERR "Entering state STRPROD_DOENUM\n";
               $state = $STRPROD_DOENUM;
            }
            elsif ($nxtLine =~ /TABLE_SIZES/) {
               #print STDERR "Entering state STRPROD_DOSIZES\n";
               $state = $STRPROD_DOSIZES;
            }
         }
      }
      elsif ($state == $STRPROD_DOLANG) {
         if ($nxtLine =~ /#END/) {
            #print STDERR "Entering state STRPROD_START\n";
            $state = $STRPROD_START;
         }
         else {
            #for valid (uncommented) lang enums it should start at the beginning of the line
            if ( ($nxtLine =~ /^LANG_/) && ($done_with_translations == false ) )
            {
               my ($numIndex, $endIndex, $eqIndex );

               #Here are some of the permutations we might see
               #1. LANG_ENGLISH = 0,  
               #2. LANG_FINNISH, 
               #3. LANG_NUM_PRINTER_LANG = LANG_END_PRINTER_LANG,                                    
               #4. LANG_ARABIC = LANG_NUM_PRINTER_LANG, 
               #5. LANG_END_ALL_LOCAL,                                                               
               #6. LANG_CZECH= LANG_END_ALL_LOCAL,  
               # 
               # we want to dump #3 and #5
               # and keep the rest

               #dump enums with NUM in them
               $numIndex = $nxtLine =~ /LANG_NUM_/;
               $endIndex = $nxtLine =~ /LANG_END_/;
               $eqIndex = $nxtLine =~ /=/;

               if ($numIndex && $eqIndex && $endIndex) {
                  #skip #3
                  $done_with_translations = true;
               }
               elsif ($endIndex && !$eqIndex) {
                  #skip #5
               }
               else {
                  my $langCategory;
                  my $langEnumName;
                  my $langStringName;
                  my $langAbrev;
                  my $translations;

                  # Find the ENUM name
                  my $index = index( $nxtLine, ',' );
                  my $index2 = index( $nxtLine, '=' );
                  if ( $index2 != -1 )
                  {
                     $index = $index2;
                  }
                  $langEnumName = substr( $nxtLine, 0, $index );
                  
                  # Find the comment
                  my @comments = qw ( /*!< /* // );
                  my $tmp_str = '';
                  foreach my $comment ( @comments ) 
                  {
                     $index = index ( $nxtLine, $comment );
                     if ( $index != -1 ) 
                     {  
                        $tmp_str = substr( $nxtLine, $index + length( $comment ) );
                        last;
                     }
                  }
                  if ( $tmp_str eq '' ) 
                  {
                     print STDERR "Enumerated languge is missing translation information in the line: $nxtLine - Skipping language...\n";
                     next;
                  }
                  
                  my @values = split( " ", $tmp_str );
                  if ( $#values < 4 ) 
                  {
                     print STDERR "Not enough information in line: '$nxtLine' Found " . scalar (@values) . " entries - Skipping line...\n";
                     next;
                  }
                  $langStringName = $values[0];
                  $langAbrev      = $values[1];
                  $translations   = $values[3];
                  if ( $values[2] eq 'FA')
                  {
                     $langCategory = LANGCAT_FA;
                  }
                  elsif ( $values[2] eq 'FP' )
                  {
                     $langCategory = LANGCAT_FP;
                  }
                  elsif ( $values[2] eq 'PC' )
                  {
                     $langCategory = LANGCAT_PC;
                  }
                  else
                  {
                     print STDERR "Invalid language category $values[2] - skipping language...\n";
                     next;
                  }
                  
                  # Save the values into array
                  push @m_LangInfo, { langCategories => $langCategory,
                                      langEnumName   => $langEnumName,
                                      langStringName => $langStringName,
                                      langAbrev      => $langAbrev,
                                      translations   => $translations,
                                    };
                  $m_NumLanguages++;
                  
               }
            }
         }
      }
      elsif ($state == $STRPROD_DOTABLE) {
         if ($nxtLine =~ /#END/) {
            #print STDERR "Entering state STRPROD_START\n";
            $state = $STRPROD_START;
         }
         else {
            if ($nxtLine =~ "STRING_") {
               &GetTableInfo($nxtLine, $m_NumTables);
               $m_NumTables++;
            }
         }
      }
      elsif ($state == $STRPROD_DOENUM) {
         if ($nxtLine =~ /#END/) {
            #print STDERR "Entering state STRPROD_START\n";
            $state = $STRPROD_START;
         }
         else {
             #look for the beginning of the enum
             #we should find uncommented enums at the beginning of the line
             #We want to skip lines like this: //STRING_BLAH
             if ($nxtLine =~ /^STRING_/)
             {
                if ($nxtLine =~ /_END/) {
                   #we're done with this table
                }
                else {
                   push @m_EnumInfo, {
                      #These vars are constant and are read one time from the config file
                      enumVal       => undef, #decimal value of enum
                      tableIndex    => undef, #index of table this enum belongs to
                      haveTag       => undef, #enum has a localization tag associated with it
                      isOSFA        => undef, #One size fits all, the english version of string used for all languages
                      enumName      => undef, #enum name
                      enumTag       => undef, #localization tag for this enum
                      defaultString => undef, #string to use for english

                      #these vars are cleared and reused for each language
                      foundTag      => undef, #true if found matching localization tag in the localization file
                      haveString    => undef, #a valid string value found in tag file or in strlist file
                      enumString    => undef, #current string value from localization or strlist file   
                   };

                   my $curEnumVal;

                   if ($m_NumEnums == 0) {
                      $curEnumVal = 0;
                   }
                   else {
                      $curEnumVal = $m_EnumInfo[$m_NumEnums-1]{enumVal};
                   }

                   $res = &GetEnumInfo($nxtLine, $m_NumEnums, $curEnumVal);
                   if ($res != LOADRES_OK) {
                       print STDERR "ERROR on line: $lineNum\n";
                       $done = true;
                   }

                   #get the enums table abrev
                   #find the table and increment the number of strings
                   my $abrev;
                   if (!($m_EnumInfo[$m_NumEnums]{enumName} =~ /STRING_/)) {
                      die "ERROR: Encountered bad enum: $m_EnumInfo[$m_NumEnums]{enumName}\n";
                   }

                   $abrev = $';
                   if (!($abrev =~ /_/)) {
                      die "ERROR: Encountered bad enum: $m_EnumInfo[$m_NumEnums]{enumName}\n";
                   }

                   $abrev = $`;
                   $index = &FindTableByAbrev($abrev);
                   if ($index < 0) {
                      die "ERROR: Unable to find table abbreviation: $abrev\n";
                   }

                   $m_TableInfo[$index]{tableSize}++;

                   $m_EnumInfo[$m_NumEnums]{tableIndex} = $index;

                   $m_NumEnums++;
                }
             }
             else
             {
                 #Didn't find the beginning of the enum, look to see
                 #if this is a continuation line for very long strings
                 #EX:  STRING_BUBBA    // DEF: "big long"
                 #                     // DEF: "string that"
                 #                     // DEF: "goes forever"
                 if ($nxtLine =~ /DEF:/) {
                    #grab the string and add it to the previous enums
                    #string
                    my $tmpOut;
                    my $tmp = $';
                    TrimLeft($tmp);

                    $res = &GetString($tmp, $tmpOut);

                    if ($res == LOADRES_OK) {
                       $m_EnumInfo[$m_NumEnums-1]{defaultString} = $m_EnumInfo[$m_NumEnums-1]{defaultString}.$tmpOut;
                    }
                    else
                    {
                        printf STDERR "ERROR: multiline DEF string for enum: %s missing closing quote!\n", $m_EnumInfo[$m_NumEnums-1]{enumName};
                        print STDERR "ERROR on line: $lineNum\n";
                        $res = LOADRES_FAIL;
                        $done = true;
                    }
                 }
             }
         }
      }
      elsif ($state == $STRPROD_DOSIZES) {
         if ($nxtLine =~ /#END/) {
            #print STDERR "Entering state STRPROD_DONE\n";
            $state = $STRPROD_DONE;
         }
         else{
            if ($nxtLine =~ /STRING_/) {
               &GetTableSizeDefines($nxtLine);
            }
         }
      }
      elsif ($state == $STRPROD_DONE) {
         $done = true;
      }
   }

   close DATA_FILE;
   print "Processed $lineNum lines in configuration file\n" if $verbose_output;
   return $res;
}

#
# Function to update the enum info array for the current language.
# 
sub UPdateTagStrings {
   my $langIndex = $_[0];
   my $enumIndex;
   my $filename;
   my $nxtLine;

    #for english we use the default string as our final tag string

    if ($langIndex == 0) {
       foreach $enumIndex (0..($m_NumEnums-1)) {
          $m_EnumInfo[$enumIndex]{enumString} = $m_EnumInfo[$enumIndex]{defaultString};
          $m_EnumInfo[$enumIndex]{foundTag} = true;
          $m_EnumInfo[$enumIndex]{haveString} = true;
       }
       return LOADRES_OK;
    }

    # TODO - add support for multiple translation files
    print "Opening translation file: $m_LangInfo[$langIndex]{translations}\n" if $verbose_output;
    $filename = $path . "localization/" . $m_LangInfo[$langIndex]{translations};
    open DATA_FILE, $filename or return LOADRES_FAIL;
    while ($nxtLine = <DATA_FILE>) 
    {
       my $enum_tag;
       my $translation;

       chomp($nxtLine);
       TrimLeft($nxtLine);
       TrimRight($nxtLine);

       #skip empty lines
       next if (length($nxtLine) == 0);
       
       # extract LH number 
       my $index = index( $nxtLine, " " );
       next if $index == -1;
       $enum_tag = substr( $nxtLine, 0, $index );

       # extract string
       $index = index( $nxtLine, "\"" );
       my $end_index = rindex( $nxtLine, "\"" );
       next if $index == -1 || $end_index == -1;
       $translation = substr( $nxtLine, $index + 1, $end_index - $index - 1 );

       # Replace enum string with new string from translation
       my $found = 0;
       foreach $enumIndex (0..($m_NumEnums-1)) {
          if ( $m_EnumInfo[$enumIndex]{enumTag} eq $enum_tag ) 
          {
             $m_EnumInfo[$enumIndex]{enumString} = $translation;
             $m_EnumInfo[$enumIndex]{foundTag} = true;
             $m_EnumInfo[$enumIndex]{haveString} = true;
             $found = 1;
             last;
          }
       }
       #print "Failed to find $enum_tag\n" if $found == 0;

    }
    close DATA_FILE;
    return LOADRES_OK;
}

#
# Function to create the requested blob type for the requested language.
# 
sub CreateBlob {
   my $blobType = $_[0];
   my $langIndex = $_[1];
   my $filename;

   #Create blob file
   #
   #Format:
   #  numtables
   #  table 1 offset 
   #  table 2 offset 
   #  table 3 offset 
   #  numstrings table1 <= (numtables+1)*4
   # str11 offset   
   #  str12 offset   
   #  str13 offset   
   #  numstrings table2  <= table1 offset + (numstr table1 + 1)*4
   #  str21 offset
   #  str22 offset
   #  str23 offset
   #  numstrings table3  <= table2 offset + (numstr table2 + 1)*4
   #  str31 offset
   #  str32 offset
   #  str33 offset
   #  str11          <= table3 offset + (numstr table3 + 1)*4
   #  str12          <= str11 offset + str11 len + 1
   #  str13          <= str12 offset + str12 len + 1
   #  str21          <= str13 offset + str13 len + 1
   #  str22 
   #  str23 
   #  str31 
   #  str32 
   #  str33 

   my $res = LOADRES_OK;
   my $offset;
   my $outVal;
   my $numEntries;
   my $numStrings;
   my $iter;
   my @tableOffset;
   my @includeTable;
   my @strOffset;
   my @strLength;
   my $strIndex;
   my $result = LOADRES_OK;
   my $numIncTables = 0;
   my $tableIndex = 0;
   my $lastString;
   my $output_filename;

   $output_filename = $path . "blob" . $m_LangInfo[$langIndex]{langStringName};
   if ($blobType == BLOB_C) 
   {
       $output_filename .= ".c" ;
   }
   elsif ( $blobType == BLOB_BINARY_FW)
   {
       $output_filename .= ".blb" ;
   }
   elsif ( $blobType == BLOB_PC_CPP )
   {
       $output_filename = "PCBlob.cpp";
       # No support yet for PC files...
       return LOADRES_FAIL;
   }

    open OUTPUT_FILE, '>', $output_filename or die "Open of writable file $output_filename failed: $!"; 
    if ($res == LOADRES_OK)
    {
        #figure out which tables to include in this output file
        #The tables have to be contiguous. This means that for some
        #outputs we will generate all the tables and for some outputs
        #we will skip one or more at the bottom of the tables (never in the middle)
        #This simplifies our logic below: as soon as we find a non included table
        #we are done
        foreach $iter (0..($m_NumTables-1)) {
           push @includeTable, undef;

           #include all tables for pc
           if ($blobType == BLOB_PC_CPP) {
              $includeTable[$iter] = true;
              $numIncTables++;
           }
           #include FA and FP tables for firmware
           else {
              if ($m_TableInfo[$iter]{tableCategory} & (LANGCAT_FA | LANGCAT_FP)) {
                 $includeTable[$iter] = true;
                 $numIncTables++;
              }
              else {
                 $includeTable[$iter] = false;
              }
           }
        }

        #calculate all the table offsets
        $offset = ($numIncTables+1)*4;

        foreach $iter (0..($numIncTables-1)) {
           push @tableOffset, $offset;
           $numEntries = $m_TableInfo[$iter]{tableSize};
           $offset += ($numEntries+1) * 4;
        }

        #calculate all the string offsets for each table
        $strIndex = 0;
        foreach $iter (0..($numIncTables-1)) {
           foreach $numStrings (0..($m_TableInfo[$iter]{tableSize}-1))
           {
              push @strOffset, undef;
              push @strLength, undef;
              $strOffset[$strIndex] = $offset;
              if ( defined ( $m_EnumInfo[$strIndex]{enumString} ) == 0 )
              {
                  $strLength[$strIndex] = 1;
              }
              else
              {
                  $strLength[$strIndex] = length($m_EnumInfo[$strIndex]{enumString}) + 1;
              }
              $offset += $strLength[$strIndex];
              $strIndex++;
           }
        }

        if ($blobType == BLOB_C) {
           print OUTPUT_FILE "/* \n";
           print OUTPUT_FILE " *\n";
           print OUTPUT_FILE " * ============================================================================\n";
           print OUTPUT_FILE " * Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved\n";
           print OUTPUT_FILE " *\n";
           print OUTPUT_FILE " *                         Marvell Confidential\n";
           print OUTPUT_FILE " * ============================================================================\n";
           print OUTPUT_FILE " *\n";
           print OUTPUT_FILE " */\n\n";

           printf OUTPUT_FILE "unsigned char Blob%sStrings[] __attribute__ ((aligned(4))) = \n{\n", $m_LangInfo[$langIndex]{langStringName};
        }

        $outVal = $numIncTables;
        
        if (($blobType == BLOB_C) || ($blobType == BLOB_PC_CPP)) 
        {
           &BlobCIntOut(*OUTPUT_FILE, $outVal);
        }
        else
        {
           &BlobIntOut( *OUTPUT_FILE, $outVal );
        }

        #output all the table offsets
        foreach $iter (0..($numIncTables-1)) {
           $outVal = $tableOffset[$iter];
           if (($blobType == BLOB_C) || ($blobType == BLOB_PC_CPP)) {
              &BlobCIntOut(*OUTPUT_FILE, $outVal);
           }
           else
           {
              &BlobIntOut(*OUTPUT_FILE, $outVal);
           }
        }

        $strIndex = 0;
        #now output all the string offsets for each table
        foreach $iter (0..($numIncTables-1)) {
           $outVal = $m_TableInfo[$iter]{tableSize};
           if (($blobType == BLOB_C) || ($blobType == BLOB_PC_CPP)) {
              &BlobCIntOut(*OUTPUT_FILE, $outVal);
           }
           else
           {
              &BlobIntOut(*OUTPUT_FILE, $outVal);
           }


           foreach $numStrings (0..($m_TableInfo[$iter]{tableSize}-1)) {
              $outVal = $strOffset[$strIndex];
              if (($blobType == BLOB_C) || ($blobType == BLOB_PC_CPP)) {
                 &BlobCIntOut(*OUTPUT_FILE, $outVal);
              }
              else
              {
                 &BlobIntOut(*OUTPUT_FILE, $outVal);
              }

              $strIndex++;
           }
        }

        #now output all the strings
        $lastString = false;
        foreach $iter (0..($m_NumEnums-1)) {
           #detect last string we are going to output, it is either the last
           #iter or if the table of the next string is not included
           if ($iter < ($m_NumEnums-1)) {
              $tableIndex = $m_EnumInfo[$iter+1]{tableIndex};
              if (!($includeTable[$tableIndex])) {
                 $lastString = true;
              }
           }
           else {
              $lastString = true;
           }

           #see if the table for this string is included
           #if not we are done
           $tableIndex = $m_EnumInfo[$iter]{tableIndex};
           if ($includeTable[$tableIndex] == true)
           {
              if (($blobType == BLOB_C) || ($blobType == BLOB_PC_CPP)) {
                 &BlobCStrOut(*OUTPUT_FILE, $m_EnumInfo[$iter]{enumString}, $strLength[$iter], $lastString);
              }
              else
              {
                 &BlobStrOut(*OUTPUT_FILE, $m_EnumInfo[$iter]{enumString}, $strLength[$iter], $lastString);
              }

           }
           else {
              last;
           }
        }

        if (($blobType == BLOB_C) || ($blobType == BLOB_PC_CPP)) {
           print OUTPUT_FILE "};\n\n";
        }
    }
    else {
       $fatalError = true;
       print STDERR "Could not open blob output file.\n";
       $result = LOADRES_FAIL;
    }
    close OUTPUT_FILE;

    return $result;
}

#
# Function to output an integer in 'C' blob format.
# 
sub BlobCIntOut {
    my $filehandle = shift;
    my $val = shift;

   # '0xaa', '0xbb', '0xcc' ,'0xdd',
   my @ch;

   push @ch, ($val & 0x000000FF);
   push @ch, (($val >> 8) & 0x000000FF);
   push @ch, (($val >> 16) & 0x000000FF);
   push @ch, (($val >> 24) & 0x000000FF);

   printf $filehandle "    0x%02X, 0x%02X, 0x%02X ,0x%02X,\n", $ch[3], $ch[2], $ch[1], $ch[0];
   return LOADRES_OK;
}

#
# Function to output an integer in blb format
#
sub BlobIntOut
{
    my $filehandle = shift;
    my $val = shift;

    if ( $big_endian_blobs == 1 )
    {
        print $filehandle pack( "N", $val );
    }
    else
    {
        print $filehandle pack( "V", $val );
    }
    return LOADRES_OK;
}

#
# Function to output a string in 'C' blob format.
# 
sub BlobCStrOut {
   my $filehandle = shift;
   my $str = shift;
   my $len = shift;
   my $last = shift;

   # 'a', 'b', 'c', 'd',
   my $i;
   my ($out, $tmp);
   my $c;

   $out = "    ";

   foreach $i (0..($len-2)) {
      $c = ord($str);
      $str =~ s/^[\d\D]//;

      #put out the hex value for non printable characters and for the single quote char
      if (($c < 0x20) || ($c > 0x7F) || ($c == 0x27)) {
         $tmp = sprintf "0x%02X, ", $c;
      }
      else
      {
         $tmp = sprintf "'%c', ", $c;
      }
      $out = $out.$tmp;
   }
   if ( defined( $str ) == 0 )
   {
       $tmp = "0x00";
   }
   else
   {
       $tmp = sprintf "0x%02X", ord($str);
   }
   $out = $out.$tmp;
   if (!$last) {
      $out = $out.",";
   }
   $out = $out."\n";
   print $filehandle "$out";

   return LOADRES_OK;
}

#
# Function to output string for blb format
#
sub BlobStrOut
{
    my $filehandle = shift;
    my $str = shift;
    my $len = shift;
    my $last = shift;

    if ( defined( $str ) == 0 )
    {
        print $filehandle pack( "Z*", "" );
    }
    else
    {
        print $filehandle pack ( "Z*", $str );
    }
    return LOADRES_OK;
}


#
# Utility function to verify we have enough information (language name and abbreviation)
# to localize the requested language.
sub CanStartLocalizeLang {
   my $index = $_[0];

   #see if we have enough info to start localizing.
   #the language entry must have:  //langname  abrev
   if (length($m_LangInfo[$index]{langStringName}) == 0) {
      return false;
   }

   if (length($m_LangInfo[$index]{langAbrev}) == 0) {
      return false;
   }

   return true;
}

#
# Function to parse the table info section of the config file.
# 
sub GetTableInfo {
   my $line = $_[0];
   my $tableNum = $_[1];

   #typical enum list:
   #typedef enum
   #{
   #    STRING_STATUS_TABLE        = 0x00000000,   //STAT   FA
   #    STRING_GENERIC_TABLE       = 0x01000000,   //GEN    FA 
   #    STRING_CTRL_PANEL_TABLE    = 0x02000000,   //CP     FA 
   #    STRING_EWS_TABLE           = 0x03000000,   //EWS    FA  
   #    STRING_REPORT_PAGE_TABLE   = 0x04000000,   //RPT    FA 
   #} STRMGR_TABLES;
   #      #define STRING_STATUS_TABLE        0x00000000    //STAT

   my $argCnt = 1;
   my $tmp1 = $line;
   my $tmp2;

   push @m_TableInfo, { tableCategory => 0,
                        tableOffset   => 0,    #base enum offset for this table
                        tableSize     => 0,    #number of enums in table
                        tableName     => "",   #define for the table name
                        tableAbrev    => "",   #abrev used as part of string enum
                        tableSizeName => "",   #the define for the size
   };

   $m_TableInfo[$tableNum]{tableOffset} = 0;  
   $m_TableInfo[$tableNum]{tableSize} = 0;    
   $m_TableInfo[$tableNum]{tableCategory} = 0;

   while (length($tmp1) > 0) {
      if ($tmp1 =~ / /)
      {
          $tmp2 = $`;
          $tmp1 = $';
      }
      else
      {
          $tmp2 = $tmp1;
          $tmp1 = "";
      }

      &TrimLeft($tmp2);
      &TrimRight($tmp2);

      if ($argCnt == 1) {
         #table name
         $m_TableInfo[$tableNum]{tableName} = $tmp2;
         $argCnt++;
      }
      elsif ($argCnt == 2) {
         #this is the '=', just toss it
         $argCnt++;
      }
      elsif ($argCnt == 3) {
         #table offset, need to convert this to decimal
         if ($tmp2 =~ /(0x|0X)\d+/) {
            $m_TableInfo[$tableNum]{tableOffset} = hex $&;
         }

         #also get rid of // before last arg
         if ($tmp1 =~ m{(///<)|(//)}) {
            $tmp1 = $';
         }
         $argCnt++;
      }
      elsif ($argCnt == 4) {
         $m_TableInfo[$tableNum]{tableAbrev} = $tmp2;
         $argCnt++;
      }
      elsif ($argCnt == 5) {
         if ($tmp2 =~ /FA/) {
             $m_TableInfo[$tableNum]{tableCategory} |= LANGCAT_FA;
         }
         if ($tmp2 =~ /FP/) {
             $m_TableInfo[$tableNum]{tableCategory} |= LANGCAT_FP;
         }
      }

      &TrimLeft($tmp1);
      &TrimRight($tmp1);
   }
}

#
# Function to parse an individual string's enum from the the config file.
# 
sub GetEnumInfo {
   my $line       = $_[0];
   my $enumIndex  = $_[1];
   my $curEnumVal = $_[2];

   #Some enums we may see:
   #  STRING_STAT_NONE = STRING_STATUS_TABLE, 
   #  STRING_STAT_READY,                        //m19928  DEF:"ready or not"

   my ($tmp1, $tmp2, $tmp3);
   my $argCnt = 1;
   my $res = LOADRES_OK;

   $tmp1 = $line;

   $m_EnumInfo[$enumIndex]{haveTag} = undef;
   $m_EnumInfo[$enumIndex]{foundTag} = undef;
   $m_EnumInfo[$enumIndex]{haveString} = undef;
   $m_EnumInfo[$enumIndex]{haveEnum} = undef;
   $m_EnumInfo[$enumIndex]{isOSFA} = undef;

   while (length($tmp1) > 0)
   {
      if ($argCnt == 1) {
         if ($tmp1 =~ /,/) {
            #grab the enum name
            $tmp2 = $`;

            #if we find an equal sign we need to convert the
            #value after the equal into an offset
            if ($tmp2 =~ /=/) {
               my $tableIndex;

               #grab the value after the =
               $tmp3 = $';

               #keep the enum name
               $tmp2 = $`;

               &TrimLeft($tmp3);
               &TrimRight($tmp3);

               $tableIndex = &FindTableByName($tmp3);
               if ($tableIndex < 0) {
                  die "Cannot find table $tmp3 for entry $line\n"
               }

               $m_EnumInfo[$enumIndex]{enumVal} = $m_TableInfo[$tableIndex]{tableOffset};
            }
            else {
               #else set the enum to 1 more than the previous
               $m_EnumInfo[$enumIndex]{enumVal} = $curEnumVal + 1;
            }
         }
         else {
            $line =~ / /;
            $tmp2 = $`;
         }
         &TrimLeft($tmp2);
         &TrimRight($tmp2);
         $m_EnumInfo[$enumIndex]{enumName} = $tmp2;

         #grab remainder of string
         $tmp1 = $';
         &TrimLeft($tmp1);
         &TrimRight($tmp1);
         $argCnt++;            
      }
      elsif ($argCnt == 2) {
         #see if have optional tag and default string
         #they follow comment

         #find and remove comments
         if (!($tmp1 =~ m{(///<)|(//)})) {
             #no comments found so no tag or DEF: 
         }
         else {
            #strip the comments and remove leading white space
            $tmp1 = $';
            TrimLeft($tmp1);
            TrimRight($tmp1);

            #after comment we can have these permutations:
            # 1.  tag !!!Now this is invalid, we require all strings to have a DEF
            # 2.  tag DEF:"str"
            # 3.  DEF:"str"
            # Put the tag in tmp1 and DEF: in tmp2

            if ($tmp1 =~ /DEF:/) {
               #have DEF:,  see if is 2 or 3
               if (length($`) == 0) {
                   #case 3 set tmp2 to to the value after DEF:
                   $tmp2 = $';

                   #tmp one has nothing more of interest in it
                   $tmp1 = "";
               }
               else {
                  #case 2 set tmp2 to the value after DEF:
                  $tmp2 = $';

                  #keep the tag
                  $tmp1 = $`;
                  &TrimLeft($tmp1);
                  &TrimRight($tmp1);
               }

               &TrimLeft($tmp2);
               &TrimRight($tmp2);

               #process the string (makes sure it is valid with quotes etc)
               $res = &GetString($tmp2, $tmp3);
               if ($res == LOADRES_OK) {
                  $tmp2 = $tmp3;
               }
               else {
                  $fatalError = true;
                  printf STDERR "ERROR: DEF string for enum %s missing quote!\n", $m_EnumInfo[$enumIndex]{enumName};
                  $res = LOADRES_FAIL;
               }
            }
            else
            {
               #case 1 tag only: missing the now required default string
               $tmp2 = "";
               &TrimLeft($tmp1);
               &TrimRight($tmp1);

               $fatalError = true;
               printf STDERR "ERROR: DEF string missing for enum: %s!\n", $m_EnumInfo[$enumIndex]{enumName};
               $res = LOADRES_FAIL;
            }

            $m_EnumInfo[$enumIndex]{enumTag} = $tmp1;
            if (length($tmp1) > 0) {
               $m_EnumInfo[$enumIndex]{haveTag} = true;

               if (($m_EnumInfo[$enumIndex]{enumTag} eq "OSFA")||($m_EnumInfo[$enumIndex]{enumTag} eq "OSFAFN")) 
               {
                   $m_EnumInfo[$enumIndex]{isOSFA} = true;
               }
            }

            $m_EnumInfo[$enumIndex]{defaultString} = $tmp2;
            $argCnt++;
            $tmp1 = "";
         }
      }
   }
   return $res;
}

#
# Function to parse the table size defines from the config file.
# 
sub GetTableSizeDefines {
   my $line = $_[0];

    # string format we are looing for:
    #     #define STRING_MAX_STAT  (STRING_STAT_END - STRING_STATUS_TABLE)
    # We only want the value right after the define
    my $tabIndex;
    my ($tmp, $tabAbrev);

    $line =~ /#define/;
    $tmp = $';  
    &TrimLeft($tmp);
    &TrimRight($tmp);

    $tmp =~ / /;
    $tmp = $`;
    &TrimLeft($tmp);
    &TrimRight($tmp);

    #now find the table this goes in
    $tmp =~ /MAX_/;
    $tabAbrev = $';
    &TrimLeft($tabAbrev);
    &TrimRight($tabAbrev);

    $tabIndex = &FindTableByAbrev($tabAbrev);

    $m_TableInfo[$tabIndex]{tableSizeName} = $tmp;
}

#
# Function to use the table abbreviation to lookup the associated table index.
# 
sub FindTableByAbrev {
   my $abrev = $_[0];

   my $tableIndex = -1;
   my $i;

   foreach $i (0..($m_NumTables-1)) {
      if ($m_TableInfo[$i]{tableAbrev} eq $abrev)
      {
         $tableIndex = $i;
         return $tableIndex;
      }
   }

   return $tableIndex;
}

#
# Function to use the table name to lookup the associated table index.
# 
sub FindTableByName {
   my $name = $_[0];

   my $tableIndex = -1;

   foreach my $i (0..($m_NumTables-1)) {
      if ($m_TableInfo[$i]{tableName} eq $name) {
          $tableIndex = $i;
          return $tableIndex;
      }
   }

   return $tableIndex;
}

#
# Function to replace the supported escaped ('\') characters with the desired content.
# 
sub PreProcessStrings {
   my ($i, $j);
   my $index;
   my $length;
   my @chIn = (undef, undef, undef, undef);
   my $chOut;
   my $tmpI;
   my @enumStr;

   #process all of the string lines
   for ($i=0; $i<$m_NumEnums; $i++)
   {
      my @buff;
      if ( defined($m_EnumInfo[$i]{enumString} ) == 0 )
      {
          next;
      }
      $length = length($m_EnumInfo[$i]{enumString});
      @enumStr = unpack('C*', $m_EnumInfo[$i]{enumString});
      $index = 0;

      for ($j = 0; $j < $length; $j++)
      {
         $chIn[0] = $enumStr[$j];

         #look for quoted chars and translate
         if ($chIn[0] == ord('\\'))
         {
            $chIn[1] = $enumStr[$j+1];
            if ($chIn[1] == ord('n')) {
               #put new line in for \n
               push @buff, 0x0A;
               $j++;
            }
            elsif ($chIn[1] == ord('"')) {
               #put unquoted quote in for quoted quote
               push @buff, ord('"');
               $j++;
            }
            elsif ($chIn[1] == ord('x')) {
               #convert the two digits after the x to decimal
               $chIn[2] = $enumStr[$j+2];
               $chIn[3] = $enumStr[$j+3];
               for ($tmpI = 2; $tmpI < 4; $tmpI++)
               {
                  if (($chIn[$tmpI] >= 0x30) && ($chIn[$tmpI] <= 0x39)) {
                     $chIn[$tmpI] -= 0x30;
                  }
                  elsif (($chIn[$tmpI] >= 0x41) && ($chIn[$tmpI] <= 0x46)) {
                     $chIn[$tmpI] -= 0x37;
                  }
                  elsif (($chIn[$tmpI] >= 0x61) && ($chIn[$tmpI] <= 0x66)) {
                     $chIn[$tmpI] -= 0x57;
                  }
               }

               $chOut = $chIn[2] << 4;
               $chOut += $chIn[3];
               push @buff, $chOut;
               $j+=3;
            }
            else {
               push @buff, 0x0A;
               $j++;
            }
         }
         else
         {
            push @buff, $chIn[0];
         }
      }

      # overwrite the previous string with the new string
      # NOTE: pack with a 'C*' template adds a null terminator 
      $m_EnumInfo[$i]{enumString} = pack('C*', @buff);
   }
}

#
# Function to convert a double-quoted string with the desired character sequence.
# 
sub GetString {
   my $inString  = $_[0];
   my $outString;

   #Processes a quoted string and gets the string characters
   #looks for quoted quote chars in the string and handles

   my $res = LOADRES_OK;

   #find initial quote
   $outString = "";

   if (!($inString =~ /"/)) {
      $_[1] = $outString;
      return LOADRES_FAIL;
   }

   #start after the leading quote (dump the quote)
   $inString = $';

   #keep any quoted quotes
   while ($inString =~ /\\"/) {
      #copy everything upto and including the \"
      $outString = $outString.$`.$&;

      #keep everything after the \"  
      $inString = $';
   }

   #copy the rest of the string
   #find final quote
   if ($inString =~ /"/) {
      $outString = $outString.$`;
   }
   else {
      $_[1] = $outString;
      return LOADRES_FAIL;
   }

   $_[1] = $outString;
   return $res;
}

#
# main program
# 
print "Starting string_blobber execution - $BLOBBER_VERSION\n" if $verbose_output;

#Read path information from command line options
GetOptions (\%argOptions, 'verbose', "path=s" => \$path);
if ( $path eq '') 
{
   print "Failed to provide path to string_mgr_config.h file\n";
   exit 1;
}

# Convert to forward slashes and append trailing slash if missing
$path =~ s|\\|/|g;
if ( substr( $path, -1, 1) ne '/' ) 
{
   $path .= '/';
}

# Start Generating string files
&OnButtonGenerate;
        
if (!$fatalError) {
   print "Finished string_blobber execution, succeeded!\n" if $verbose_output;
   0;
}
else {
   print STDERR "Finished string_blobber execution, failed!\n";
   1;
}

