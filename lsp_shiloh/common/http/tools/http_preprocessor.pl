#!perl
use diagnostics;
no warnings 'uninitialized';
use strict;

use File::Find;
use File::Spec;
use File::Path;
use File::Copy;
use Cwd;
use Getopt::Long;

my %macros;
my %enums;
my %oids;
my %strings;
my %files_to_process;
my $num_files = 0;
my $abs_rom_input_directory;
my $offset = 0;
my $path_length = 0;
my $version = "20120911";

# declare variables that are used by the http_preprocessor_config.pl script
# to perform it's initialization
our $product_path = '';
our $common_path = '';

# declare variables initialized in the http_preprocessor_config.pl script
# declared as our so that the script can access them with strict mode
our $verbose_output;
our $verbose_warnings;
our $input_directory;
our $strings_file;
our @oids_files;
our $http_files_header;
our $output_directory;
our $file_name;
our $ssi_tag;
our $strip_white_space;
our @macro_files;
our @enum_files;
our @binary_file_extensions;
our $generate_name_to_value_mapping_files;

$generate_name_to_value_mapping_files = 0;

## process cmdline options
my %argOptions = ();
GetOptions (\%argOptions, 'verbose', "common_path=s" => \$common_path, "product_path=s" => \$product_path);

## verify that required command line options were provided
die "common_path not specified" unless $common_path;
die "product_pth not specified" unless $product_path;

print "common_path = $common_path\n" if $argOptions{verbose};
print "product_path = $product_path\n" if $argOptions{verbose};

# include the configuration script
require "$product_path/http/config/http_preprocessor_config.pl";

# if verbose output not explicitly enabled in config file apply the
# command line verpose option
$verbose_output = $argOptions{verbose} unless $verbose_output;


&parse_args;
&print_config_vars;
&parse_oids;
&parse_strings;
&parse_macros;
&parse_enums;
&build_file_hash;

# Create absolute path - then create abs output path
my $abs_input_directory = File::Spec->rel2abs( $input_directory, cwd() ) ;
$path_length = length( cwd() ) + 1;
my $abs_output_directory = $abs_input_directory;
$abs_output_directory =~ s/tmpin/tmpout/;

# Remove old information
rmtree( $abs_output_directory );

# Process data
find ( \&preprocess_data_file, $abs_input_directory );

# Generate Rom files now
&generate_rom_files;

# Update input files by replace names with oid/string/macro/enum values
sub preprocess_data_file
{
    my $my_path;
    my $in_comment = 0;
    my $file_found = 0;
    my $end;
    my $start;
    my $first;
    my $last;

    return if $_ =~ /^\./;
    return if -d $_;

    while ( my ( $key, $value ) = each %files_to_process )
    {
        $file_found = 1 if ( $key =~ m/$_/ );
    }
    return if not $file_found;
 
    my $output_name = $File::Find::name;
    my $output_dir = $File::Find::dir;
    $output_name =~ s/tmpin/tmpout/;
    $output_dir =~ s/tmpin/tmpout/;
    mkpath( $output_dir );
    
    my $ext = ( $_ =~ m/([^.]+)$/)[0];
    foreach my $extension ( @binary_file_extensions )
    {
        if ( $ext eq $extension )
        {
            print "Copying binary file: " . substr( $output_name, $path_length ). " \n" if ( $verbose_output );
            copy( $_, $output_name ) or die "Copy of $_ to $output_name failed: $!";
            return;
        }
    }

    open DATA_FILE, $_ or die "Open of $_ failed: $!";
    open OUTPUT_FILE, '>', $output_name or die "Open of writable file $output_name failed: $!"; 
    print "Processing file: " . substr( $output_name, $path_length ). "\n" if ( $verbose_output );
    while ( <DATA_FILE> )
    {
       # Split so that only 1 ssi tag exists per string
       my @sub_lines = split /(<!--#$ssi_tag)/, $_;
       my $prev = "";
       foreach ( @sub_lines ) 
       {
          # Recombine to add back ssi prefix that was lost during split
          if ( $_ eq "<!--#$ssi_tag" )
          {
             $prev = $_;
             next;
          }
          elsif ( $prev ne '' )
          {
             $_ = $prev . $_;
             $prev = '';
          }

           # Replace COMMENT
           if ( $in_comment )
           {
               $end = index( $_, "-->" );
               if ( $end != -1 )
               {
                   $_ = substr( $_, $end + length( "-->" ) );
                   $in_comment = 0;
               }
               else
               {
                   next;
               }
           }
           
           # Look for comments "<!--COMMENT -->
           $start = index( $_, "<!--COMMENT" );
           if ( $start != -1 )
           {
               $end = index( $_, "-->", $start );
               if ( ( $end != -1 ) and ( $end > $start ) )
               {
                   #comment is all in this line - cut it out of this line
                   $first = substr( $_, 0, $start );
                   $last = substr( $_, $end + length( "-->") );
                   $_ = $first . $last;
               }
               else
               {
                   # Comments spans lines - cut out after comment and set flag
                   $in_comment = 1;
                   $_ = substr( $_, 0, $start );
               }
           }
           
           if ( $strip_white_space )
           {
               # Remove whitespace from start of line
               s/^(\s)+([^\s]+)/$2/;
               
               # Remove blank lines
               if ( m/^(\s)*$/ )
               {
                   next;
               }
           }
           
           # Replace macros
           if ( m/<!--#$ssi_tag(?:\s)+IDm(?:\s)+([^\s]+)(?:\s)*-->/ )
           {
               if ( defined( $macros{ $1 } ) )
               {
                   s/<!--#$ssi_tag(?:\s)+IDm(?:\s)+([^\s]+)(?:\s)*-->/$macros{$1}/g;
               }
               elsif( $verbose_warnings )
               {
                   print "MACRO: $1 not defined but used in: $output_name\n";
               }
           }
   
           # Replace enums
           if ( m/<!--#$ssi_tag(?:\s)+IDe(?:\s)+([^\s]+)(?:\s)*-->/ )
           {
               if ( defined( $enums{ $1 } ) )
               {
                   s/<!--#$ssi_tag(?:\s)+IDe(?:\s)+([^\s]+)(?:\s)*-->/$enums{$1}/g;
               }
               elsif( $verbose_warnings )
               {
                   print "ENUM: $1 not defined but used in: $output_name\n";
               }
           }
           
           # Replace oids                                                   Possible _x              possible index       
           #                                              tag        possible r |      ws    oid name     |           ws    
           #                                               |             |      |      |        |         |           | 
           if( ( my ( $resolve, $oid, $idx ) ) = m/<!--#$ssi_tag(?:\s)+ID(r)?(?:_.)?(?:\s)+([^\.\s]+)(?:\.)?([^\s]+)?(?:\s)*-->/ )
           {
               if ( $idx ne "" ) 
               {
                  if ( defined( $enums{ $idx } ) )
                  {
                     $idx = $enums{ $idx };
                  }
                  elsif ( defined $macros{ $idx } )
                  {
                     $idx = $macros{ $idx };
                  }
               }
   
               if ( defined( $oids{ $oid } ) )
               {
                  if ( $resolve eq "" )
                  {
                     s/<!--#$ssi_tag(?:\s)+ID(r)?(_.)?(?:\s)+([^\.\s]+)(\.)?([^\s]+)?(?:\s)*-->/ID$1$2 $oids{ $3 }$4$idx/g;
                  }
                  else
                  {
                     #                        1    2             3       4      5
                     s/<!--#$ssi_tag(?:\s)+ID(r)?(_.)?(?:\s)+([^\.\s]+)(\.)?([^\s]+)?(?:\s)*-->/<!--#$ssi_tag ID$1$2 $oids{ $3 }$4$idx -->/g;
                  }
               }
               else
               {
                  if ( $verbose_warnings )
                  {
                     print "OID: $oid not defined but used in: $output_name\n";
                  }
               }
           }
   
           #Replace strings
           if ( ( my $strenum) = m/<!--#$ssi_tag(?:\s)+IDs(?:_.)?(?:\s)+([^\s]+)(?:\s)*-->/ )
           {
               if ( defined( $strings{ $strenum } ) )
               {
                   s/<!--#$ssi_tag(?:\s)+IDs(_.)?(?:\s)+([^\s]+)(?:\s)*-->/<!--#$ssi_tag IDs$1 $strings{ $2 } -->/g;
               }
               else
               {  
                  if ( $verbose_warnings )
                  {
                     print "STRING: $strenum not defined but used in: $output_name\n";
                  }
                  s/<!--#$ssi_tag(?:\s)+IDs(_.)?(?:\s)+$strenum(?:\s)*-->//g;
               }
           }
   
           # Replace IF statements                                    Possible _x    Comparison-expression         possible index    [oid/string/string enum]
           #                                               tag             |     ws       |      ws      oid name      |            ws      |     ws                                       
           #                                                |              |     |        |      |          |          |            |       |     |                           
           if ( my ( $oid, $index, $param2 ) = m/<!--#$ssi_tag(?:\s)+IF(?:_.)?(?:\s)+(?:[^\s]+)(?:\s)+([^\.\s]+)(?:\.)?([^\s]+)?(?:\s)+([^\s]+)?(?:\s)*-->/ )
           {
               if ( defined( $oids{ $oid } ) )
               {
                   $oid = $oids{ $oid };
               }
               elsif ( $verbose_warnings )
               {
                   print "OID: $oid not defined but used in: $output_name\n";
               }
               
               if ( $index ne "" ) 
               {
                  if ( defined( $enums{ $index } ) )
                  {
                     $index = $enums{ $index };
                  }
                  elsif ( defined $macros{ $index } )
                  {
                     $index = $macros{ $index };
                  }
               }

               if ( $param2 ne "" ) 
               {
                  if ( defined( $oids{ $param2 } ) )
                  {
                      $param2 = $oids{ $param2 };
                  }
                  elsif ( defined( $strings{ $param2 } ) )
                  {
                      $param2 = $strings{ $param2 };
                  }
                  elsif ( defined( $macros{ $param2 } ) )
                  {
                      $param2 = $macros{ $param2 };
                  }
                  elsif ( defined( $enums{ $param2 } ) )
                  {
                     $param2 = $enums{ $param2 };
                  }
               }

               #                        _x          compare          oid     .      index    oid/string/string enum
               #                        1            2                3      4      5               6
               s/<!--#$ssi_tag(?:\s)+IF(_.)?(?:\s)+([^\s]+)(?:\s)+([^\.\s]+)(\.)?([^\s]+)?(?:\s)+([^\s]+)?(?:\s)*-->/<!--#$ssi_tag IF$1 $2 $oid$4$index $param2 -->/;
           }
   

           # Replace ELSEIF statements                                    Possible _x    Comparison-expression         possible index    [oid/string/string enum]
           #                                               tag                 |     ws       |      ws      oid name      |            ws      |     ws                                       
           #                                                |                  |     |        |      |          |          |            |       |     |                           
           if ( my ( $oid, $index, $param2 ) = m/<!--#$ssi_tag(?:\s)+ELSEIF(?:_.)?(?:\s)+(?:[^\s]+)(?:\s)+([^\.\s]+)(?:\.)?([^\s]+)?(?:\s)+([^\s]+)?(?:\s)*-->/ )
           {
               if ( defined( $oids{ $oid } ) )
               {
                   $oid = $oids{ $oid };
               }
               elsif ( $verbose_warnings )
               {
                   print "OID: $oid not defined but used in: $output_name\n";
               }

               if ( $index ne "" ) 
               {
                  if ( defined( $enums{ $index } ) )
                  {
                     $index = $enums{ $index };
                  }
                  elsif ( defined $macros{ $index } )
                  {
                     $index = $macros{ $index };
                  }
               }

               if ( $param2 ne "") 
               {
                  if ( defined( $oids{ $param2 } ) )
                  {
                      $param2 = $oids{ $param2 };
                  }
                  elsif ( defined( $strings{ $param2 } ) )
                  {
                      $param2 = $strings{ $param2 };
                  }
                  elsif ( defined( $macros{ $param2 } ) )
                  {
                      $param2 = $macros{ $param2 };
                  }
                  elsif ( defined( $enums{ $param2 } ) )
                  {
                     $param2 = $enums{ $param2 };
                  }
               }

               #                            _x          compare          oid     .      index    oid/string/string enum
               #                            1            2                3      4      5               6
               s/<!--#$ssi_tag(?:\s)+ELSEIF(_.)?(?:\s)+([^\s]+)(?:\s)+([^\.\s]+)(\.)?([^\s]+)?(?:\s)+([^\s]+)?(?:\s)*-->/<!--#$ssi_tag ELSEIF$1 $2 $oid$4$index $param2 -->/;
           }


           # Replace IDc statements                                            Possible _x    Comparison-expression         possible index    [oid/string/string enum]
           #                                                                  tag   |     ws        |      ws    oid name      |            ws      |        Possible index   ws   output string                                    
           #                                                                   |    |     |         |      |        |          |            |       |             |            |       |                      
           if ( my ( $oid, $index, $param2, $index2 ) = m/<!--#$ssi_tag(?:\s)+IDc(?:_.)?(?:\s)+(?:[^\s]+)(?:\s)+([^\.\s]+)(?:\.)?([^\s]+)?(?:\s)+([^\.\s]+)(?:\.)?([^\s]+)?(?:\s)+([^\s]+)(?:\s)*-->/ )
           {
               if ( defined( $oids{ $oid } ) )
               {
                   $oid = $oids{ $oid };
               }
               elsif ( $verbose_warnings )
               {
                   print "OID: $oid not defined but used in: $output_name\n";
               }
               
               if ( $index ne "" ) 
               {
                  if ( defined( $enums{ $index } ) )
                  {
                     $index = $enums{ $index };
                  }
                  elsif ( defined $macros{ $index } )
                  {
                     $index = $macros{ $index };
                  }
               }

               if ( defined( $oids{ $param2 } ) )
               {
                   $param2 = $oids{ $param2 };
               }
               elsif ( defined( $strings{ $param2 } ) )
               {
                   $param2 = $strings{ $param2 };
               }
               elsif ( defined( $macros{ $param2 } ) )
               {
                   $param2 = $macros{ $param2 };
               }
               elsif ( defined( $enums{ $param2 } ) )
               {
                  $param2 = $enums{ $param2 };
               }

               if ( $index2 ne "" ) 
               {
                  if ( defined( $enums{ $index2 } ) )
                  {
                     $index2 = $enums{ $index2 };
                  }
                  elsif ( defined $macros{ $index2 } )
                  {
                     $index2 = $macros{ $index2 };
                  }
                  

               }

                

               #                         _x           compare        oid      .       index        oid/string  .    index         text
               #                          1            2              3       4       5                 6     7     8              9
               s/<!--#$ssi_tag(?:\s)+IDc(_.)?(?:\s)+([^\s]+)(?:\s)+([^\.\s]+)(\.)?([^\s]+)?(?:\s)+([^\.\s]+)(\.)?([^\s]+)?(?:\s)+([^\s]+)(?:\s)*-->/<!--#$ssi_tag IDc$1 $2 $oid$4$index $param2$7$index2 $9 -->/;
           }
           printf OUTPUT_FILE "%s", $_;
        }
    }

    close OUTPUT_FILE;    
    close DATA_FILE;
}

sub generate_mapping_file
{
   my ( $name, %hash  ) = @_;
   my $key;

   print "Start generating $output_directory$name" . "_mapping.c\n" if $verbose_output;
   open OUTPUT_C, ">$output_directory$name" . "_mapping.c" or die "Cannot open $output_directory$file_name.c for output - $!";
   &print_copyright( *OUTPUT_C );
   printf OUTPUT_C "//Create array to map $name to strings\n";
   printf OUTPUT_C "\n";
   printf OUTPUT_C "#include <stdint.h>\n";
   printf OUTPUT_C "\n";
   printf OUTPUT_C "typedef struct $name" . "_to_str_s\n";
   printf OUTPUT_C "{\n";
   printf OUTPUT_C "   char        *str;\n";
   printf OUTPUT_C "   uint32_t    val;\n";
   printf OUTPUT_C "} $name" . "_to_str_t;\n";
   printf OUTPUT_C "\n";
   printf OUTPUT_C "$name" . "_to_str_t $name" . "_to_string_value_map[] = \n";
   printf OUTPUT_C "{\n";
   foreach $key (keys %hash)
   {
      printf OUTPUT_C "    { \"$key\", $hash{$key}";
      # special case to prevent compiler warning
      if ( $hash{$key} >= 2147483648 ) 
      {
         printf OUTPUT_C "LL";
      }
      printf OUTPUT_C " },\n";
   }  
   printf OUTPUT_C "};\n";

   printf OUTPUT_C "const uint32_t num_$name = " . keys(%hash) . ";\n";
   printf OUTPUT_C "\n";
   close OUTPUT_C;
   print "Finished generating $output_directory$name" . "_mapping.c\n" if $verbose_output;
}

#Populate oid hash
sub parse_oids
{
   my %oid_defines;
   foreach ( @oids_files )
   {
       print "Parsing oids: $_\n" if ( $verbose_output );
       open DATA_FILE, $_ or die "Open of oid file $_ failed: $!";
       
       # Have to make 2 passes at the file - first find the defines used in the oid make calls
       # Then calculate the oid values based off previously discovered defines
       while ( <DATA_FILE> )
       {
           my ( $key, $value );
           #First #define's ( #define test (uint8_t)3)    
           $oid_defines{ $key } = $value if ( ( $key, $value ) = /^(?:#define)(?:\s)+([^\s]+)(?:\s)+\((?:\s)*uint8_t(?:\s)*\)(?:\s)*([^\s]+)/);
       }
       seek(DATA_FILE, 0, 0);
       while ( <DATA_FILE> )
       {
           my ( $key, $value );
           #Now look for #define OID_MEMORY_TOTAL_SIZE_IN_MBYTES      oid_make(OID_MODULE_MEMORY, 0, 0, 0)
           if ( my ( $key, $param1, $param2, $param3, $param4 ) = /^(?:#define)(?:\s)+([^\s]+)(?:\s)+oid_make(?:\s)*\((?:\s)*([^\s,]+)(?:\s)*,(?:\s)*([^\s,]+)(?:\s)*,(?:\s)*([^\s,]+)(?:\s)*,(?:\s)*([^\s,]+)(?:\s)*\)/ )
           {
               # Map any none numeric values
               $param1 = $oid_defines{ $param1 } if ( defined( $oid_defines{ $param1 } ) );
               $param2 = $oid_defines{ $param2 } if ( defined( $oid_defines{ $param2 } ) );
               $param3 = $oid_defines{ $param3 } if ( defined( $oid_defines{ $param3 } ) );
               $param4 = $oid_defines{ $param4 } if  ( defined( $oid_defines{ $param4 } ) );
               
               # Check the ranges
               if ( ( $param1 < 0 ) || ( $param1 > 255 ) ||
                    ( $param2 < 0 ) || ( $param2 > 255 ) ||
                    ( $param3 < 0 ) || ( $param3 > 255 ) ||
                    ( $param4 < 0 ) || ( $param4 > 255 ) )
               {
                   die "Parameters for oid $key are out of range (0-255): $param1, $param2, $param3, $param4\n";
               }
               
               # Create the oid value and save in hash
               $oids{ $key } = ( $param1 << 24 ) | ( $param2 << 16 ) | ( $param3 << 8 ) | $param4;
           }
       }
       close DATA_FILE;
   }
   if ( $generate_name_to_value_mapping_files != 0 )
   {
      generate_mapping_file( "oid_defines", %oids );
   }
}


# Populate String Hash
sub parse_strings
{
    my $cur_enum_value = 0;
    my $in_table = 0;
    my $in_enums = 0;
    my %string_tables;
    
    print "Parsing strings: $strings_file\n" if ( $verbose_output );
    
    open DATAFILE, $strings_file or die "Open of strings file $strings_file failed: $!\n";
    
    while ( <DATAFILE> )
    {
        $in_table = 1 if( m/#BEGIN(?:\s)+TABLES/ );
        $in_table = 0 if( m/#END(?:\s)+TABLES/ );
        $in_enums = 1 if( m/#BEGIN(?:\s)+ENUMS/ );
        $in_enums = 0 if( m/#END(?:\s)+ENUMS/ );
        
        if ( $in_table )
        {
            if ( my ( $key, $value ) = m/STRING_([^\s]+)(?:\s*)=(?:\s*)([0-9x]+)/ )
            {
                $string_tables{ "STRING_" . $key } = hex( $value );
            }
        }
        
        if ( $in_enums )
        {
            # Look for "STRING_TEST," case
            if ( my ( $key ) = m/STRING_([^\s]+)(?:\s)*,/ )
            {
                $strings{ "STRING_" . $key } = $cur_enum_value++;
            }
            
            #Look for "STRING_TEST = STRING_STATUS_TABLE," case
            if ( my ( $key, $value ) = m/STRING_([^\s]+)(?:\s)*=(?:\s*)([^\s]+)(?:\s)*,/ )
            {
                $string_tables{ "STRING_" . $key } = $string_tables{ $value };
                $strings{ "STRING_" . $key } = $string_tables{ $value };
                $cur_enum_value = $string_tables{ $value } + 1;
            }
        }
    }
    close DATAFILE;

    if ( $generate_name_to_value_mapping_files != 0 )
    {
       generate_mapping_file( "strings", %strings );
    }
}

# Populate Macro Hash
sub parse_macros
{    
    foreach ( @macro_files )
    {
        print "Parsing macros file: $_\n" if ( $verbose_output );
        open DATA_FILE, $_ or die "Open of macro file $_ failed: $!";
        
        while ( <DATA_FILE> )
        {
            my ( $key, $value );
            # First look for quoted #define's (#define test "Some text")
            if ( ( $key, $value ) = /^(?:#define)(?:\s)+([^\s]+)(?:\s)+"([^"]+)"/)
    #        if ( my ( $key, $value ) = m{^(?:#define)   #Lines starts with #define
    #                                     (?:\s)+        #whitespace - don't save value
    #                                     ([^\s]+)       #The define key
    #                                     (?:\s)+        #whitespace - don't save value
    #                                     (?:")?         #Possible quote mark to ignore - don't save value
    #                                     ([^\s"])+      #The define value
    #                                    }x )        
            {
                $macros{ $key } = $value;
            }
            # Next look for other defines with values (#define test 3)
            elsif ( ( $key, $value ) = /^(?:#define)(?:\s)+([^\s]+)(?:\s)+([^\s]+)/)
            {
               if ( $value =~ /^(0x)([0-9a-fA-F]+)$/ )
               {
                   $value = hex( $value );
               }
               $macros{ $key } = $value;
            }
        }
        close DATA_FILE;
    }
}

# Populate Enum Hash
sub parse_enums
{
    foreach my $enum_file ( @enum_files )
    {
        print "Parsing enums file: $enum_file\n" if ( $verbose_output );
        open DATA_FILE, $enum_file or die "Open of enum file $enum_file failed: $!";
        my $in_enum = 0;
        my $cur_value = 0;
        my $temp_var = join '', <DATA_FILE>;

        # Strip out all comments
        $temp_var =~ s#/\*.*?\*/##sg;    # cleans out /* */ comments even across multiple lines
        $temp_var =~ s#//.+##g;          # Removes everything after // on the line

        my @lines = split /\n/, $temp_var;
        
        foreach ( @lines )
        {
            my ( $key, $value );

            # First look for quoted #define's (#define test "Some text")
            if ( /(typedef)?(?:\s)*enum/ )
            {
                # Don't want all the strings in the enums hash 
                # they are parsed separately for the strings hash
                if ( /string_mgr_code_e/ ) 
                {
                   next;
                }

                # Sometimes enums are just typedefs with no values - skip these
                # For example - typedef enum string_mgr_code_e string_mgr_code_t;
                if ( /;/ ) 
                {
                   next;
                }
                $in_enum = 1;
                $cur_value = 0;
                next;
            }
            
            if ( $in_enum )
            {
                #look for } to end enum
                if ( /}/ )
                {
                    $in_enum = 0;
                    next;
                }
                
                next if ( /#endif/);
                
                if ( /#ifdef/ )
                {
                    if ( $verbose_warnings )
                    {
                        print "Script does not handle preprocessor directives inside of enum - $_\n";
                        print "Possible incorrect enum values calculated for this file ($enum_file)\n\n";
                    }
                    next;
                }

                # Add value to hash
                # Possible formats (with and without final comma)
                # ENUM,
                # ENUM = VALUE,
                # ENUM = OTHER_ENUM_VALUE,
                # ENUM = OTHER_ENUM_VALUE + VALUE,    ---- NOT SUPPORTED
                if ( ($key, $value) = /([^\s{]+)[\s]*=[\s]*([^\s,]+)[\s]*/)
                {
                    if ( $value !~ /^\[+-]?\d+$/ )
                    {
                        if ( exists $enums{ $value } )
                        {
                            $value = $enums{ $value };
                        }
                        elsif ( $value =~ /^(0([xX]))([0-9a-fA-F]+)$/ )
                        {
                            $value = lc( $value );
                            $value = hex( $value );
                        }
                    }

                    $enums{ $key } = $value;
                    $cur_value = $value + 1;                    
                }
                elsif ( ($key) = /([^\s,{]+)[\s]*/)
                {
                    $enums{ $key } = $cur_value++;
                }
            }
        }
        close DATA_FILE;
    }

    if ( $generate_name_to_value_mapping_files != 0 )
    {
       generate_mapping_file( "enums", %enums );
    }
}

# Read in preprocessed file that specifies which files to include and make a hash of these files
sub build_file_hash
{
    my $in_struct = 0;
    
    open DATA_FILE, $http_files_header or die "Open of $http_files_header failed: $!";
    my $temp_var = join '', <DATA_FILE>;

    # Strip out all comments
    s#/\*.+?\*/##gms;   # cleans out /* */ comments even across multiple lines
    s#//.+##g;          # Removes everything after // on the line

    my @lines = split /\n/, $temp_var;
    foreach ( @lines )
    {
        if ( $in_struct )
        {
            $in_struct = 0 if /};/; #Look for end of structure;
            $files_to_process{ $1 } = 1 if ( /{[\s]*"([^\s"]+)"[\s]*,[\s]*"/ );
        }
        $in_struct = 1 if ( m/http_filename_mappings_t[\s]+http_file_mappings/ );
    }
}

# List of tags that are currently parsed
sub info()
{
    print "Tags that are parsed:\n";
    print "\t<!--#$ssi_tag ID[_?] [oid].[index] -->\n";
    print "\t<!--#$ssi_tag IDr[_?] [oid] -->\n";
    print "\t<!--#$ssi_tag IDm [macro] -->\n";
    print "\t<!--#$ssi_tag IDs [string] -->\n";
    print "\t<!--#$ssi_tag IF [comparison] [oid].[index] [oid/string/string enum] -->\n";
    print "\t<!--#$ssi_tag ELSEIF [comparison] [oid].[index] [oid/string/string enum] -->\n";
    print "\t<!--#$ssi_tag IDc [comparison] [oid] [oid/string/string enum] [string] -->\n";
    print "\t<!--#$ssi_tag IDe [enum] -->\n";
    print "\t<!--COMMENT -->\n";
    print "\n";
}

# Version of this script file
sub version()
{
    print "Script Version: $version\n";
}

# Create C array of all http files to be compiled into FW
sub generate_rom_files
{
    print "Generating HTTP rom file image\n" if $verbose_output;
    
    open OUTPUT_H, ">$output_directory$file_name.h" or die "Cannot open $output_directory$file_name.h for output - $!";
    open OUTPUT_C, ">$output_directory$file_name.c" or die "Cannot open $output_directory$file_name.c for output - $!";
    
    &print_copyright( *OUTPUT_H );
    &print_copyright( *OUTPUT_C );
    
    printf OUTPUT_H "#ifndef \U$file_name\E_H\n";   # Force filename to be in capital letters
    printf OUTPUT_H "#define \U$file_name\E_H\n\n";
    
    printf OUTPUT_H "#include <stdint.h>\n\n";
    
    printf OUTPUT_H "#ifdef __cplusplus\n";
    printf OUTPUT_H "extern \"C\" {\n";
    printf OUTPUT_H "#endif\n\n";
    
    printf OUTPUT_H "typedef struct HTTPROMFILE\n\n";
    printf OUTPUT_H "{\n";
    printf OUTPUT_H "    uint32_t      page_offset;\n";
    printf OUTPUT_H "    uint32_t      length;\n";
    printf OUTPUT_H "    const char *  file_name;\n";
    printf OUTPUT_H "} HTTP_ROM_FILE;\n\n";
    
    printf OUTPUT_H "extern const unsigned char http_rom_page[];\n\n";
    
    printf OUTPUT_H "static const HTTP_ROM_FILE http_rom_file_map[ ] =\n";
    printf OUTPUT_H "/* page_offset,     length,      file_name      */\n";
    printf OUTPUT_H "{\n";
    printf OUTPUT_H "\n";
    
    printf OUTPUT_C "\n#include \"$file_name.h\"\n";
    printf OUTPUT_C "const unsigned char http_rom_page[] = \n";
    printf OUTPUT_C "{\n";
    
    find ( \&rombld_data_file, $abs_output_directory );
    
    printf OUTPUT_H "};\n\n";
    
    printf OUTPUT_H "static const uint32_t http_rom_files = $num_files;\n\n";
    
    printf OUTPUT_H "#ifdef __cplusplus\n";
    printf OUTPUT_H "}\n";
    printf OUTPUT_H "#endif\n\n";
    
    printf OUTPUT_H "#endif // \U$file_name\E_H\n";
    
    printf OUTPUT_C "};\n";
    
    close OUTPUT_H;
    close OUTPUT_C;
    print "Finished generating \n" if $verbose_output;
}

# Copywrite information for this script and its generated output
sub print_copyright
{
    my $filehandle = shift;
    printf $filehandle "/* \n";
    printf $filehandle " *\n";
    printf $filehandle " * ============================================================================\n";
    printf $filehandle " * Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved\n";
    printf $filehandle " *\n";
    printf $filehandle " *                         Marvell Confidential\n";
    printf $filehandle " * ============================================================================\n";
    printf $filehandle " *\n";
    printf $filehandle " */\n\n";
}

# Add individual page to rom file
sub rombld_data_file
{
    my $my_path;
    my $last_char_ws = 0;
    my $cur_char_ws = 0;
    my $count = 0;
    
    return if $_ =~ /^\./;
    return if -d $File::Find::name;
    
    # We only care about the path after the input directory
    $my_path = substr( $File::Find::name, length ( $abs_output_directory ) );    
    
    printf OUTPUT_C "    //**** $my_path ****\n  ";
    open DATA_FILE, $File::Find::name or die "Open of $_ failed: $!";
    binmode DATA_FILE;
    my ( $buf, $data, $n, $line_count );

    my $ext = ( $File::Find::name =~ m/([^.]+)$/)[0];
    my $image_file = 0;
    foreach my $extension ( @binary_file_extensions )
    {
        $image_file = 1 if ( $ext eq $extension );
    }

    while ( ( $n = read DATA_FILE, $data, 1 ) != 0 )
    {
        if ( $data eq " " or
             $data eq "\t" or
             $data eq "\r" or
             $data eq "\n" )
        {
            $cur_char_ws = 1;
        }
        else
        {
            $cur_char_ws = 0;
        }
        
        $count++;
        
        printf OUTPUT_C "%4d,", ord( $data );
        $line_count++;
        if ( $line_count == 16 )
        {
            printf OUTPUT_C "\n  ";
            $line_count = 0;
        }
        $last_char_ws = $cur_char_ws;
    }
    printf OUTPUT_C "\n\n";
    
    printf OUTPUT_H "{ %8d,\t%8d,\t\"%s\" },\n", $offset, $count, $my_path;
    
    close DATA_FILE;
    
    $offset += $count;
    $num_files++;        
}

# Parse command line arguments
sub parse_args
{
    foreach my $argnum ( 0 .. $#ARGV )
    {
        if ( $ARGV[ $argnum ] eq "-v" or
                $ARGV[ $argnum ] eq "-version" )
        {
            &print_copyright( *STDOUT );
            &version;
            exit;
        }
        elsif ( $ARGV[ $argnum ] eq "-d" or
                $ARGV[ $argnum ] eq "-debug" )
        {
            $argnum++;
            if ( $argnum > $#ARGV )
            {
                print "Missing debug level\n";
                exit;
            }
            if ( $ARGV[ $argnum ]  == 0 )
            {
                $verbose_output   = 0;
                $verbose_warnings = 0;
            }
            elsif ( $ARGV[ $argnum ] == 1 )
            {
                $verbose_output   = 0;
                $verbose_warnings = 1;
            }
            elsif ( $ARGV[ $argnum ] == 2 )
            {
                $verbose_output   = 1;
                $verbose_warnings = 1;
            }
            else
            {
                print "Invalid debug level\n";
                exit;
            }
            last;
        }
        else
        {
            &print_copyright( *STDOUT );
            &info;
            print "Supported command line options: \n";
            print "\t-h:       Help\n";
            print "\t-d [0-2]: Debug Level 0-2\n";
            print "\t-v:       Script Version\n";
            print "\n";
            &version;
            exit;
        }   
    }
}

# Display the configuration values
sub print_config_vars
{
    if ( $verbose_output == 1 )
    {
        print "Enums:\n" ;
        foreach (@enum_files)
        {
            print "\t$_\n";
        }
        print "Macros:\n";
        foreach (@macro_files)
        {
            print "\t$_\n";
        }
        print "Input Directory:   $input_directory\n";
        print "Output Directory:  $output_directory\n";
        print "Output Filename:   $file_name\n";
        print "Strings File:      $strings_file\n";
        print "Oids Files:\n";
        foreach(@oids_files)
        {
           print "\t$_\n";
        }
        print "Http Files header: $http_files_header\n";
        print "Strip whitespace:  $strip_white_space\n";
        print "SSI Tag:           $ssi_tag\n";
        
        print "Binary File Extensions: \n";
        foreach ( @binary_file_extensions )
        {
            print "\t$_\n";
        }
    }
}

