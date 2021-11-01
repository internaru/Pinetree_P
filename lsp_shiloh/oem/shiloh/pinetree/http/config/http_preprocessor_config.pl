#!perl

#
# ============================================================================
# Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
#
#                         Marvell Confidential
# ============================================================================
#

use vars qw(
                $verbose_output $verbose_warnings $input_directory $strings_file
                $oids_file $http_files_header $output_directory $file_name
                $ssi_tag $strip_white_space @macro_files @enum_files
                @binary_file_extensions
);

$verbose_output   = 0;
$verbose_warnings = 1;

# Paths will be used from the http/tools directory - so make any relative paths
# relative to that directory not the http/config directory!

# The input path must contain the name tmpin
# tmpin will be replaced with tmpout to create the name for the output directory
$input_directory    = "$product_path/http/config/tmpin";
$strings_file       = "$product_path/string_mgr/config/string_mgr_config.h";
$http_files_header  = "$product_path/http/config/http_file_mappings.h.preproced";
$output_directory   = "$product_path/http/config/";
$file_name          = "http_rom";
$ssi_tag            = "ssi";
$strip_white_space  = 1; 

@oids_files         = ( 
                       "$common_path/oid/include/oid_api.h",
                       );

@macro_files = (
               "$product_path/platform/config/platform.h", 
               "$product_path/http/config/http_config.h", 
               "$common_path/http/src/include/html.h",
               "$product_path/http/config/http_midas_translation_enums.h",
#               "$common_path/print/core/include/printvars_api.h",
               );

@enum_files  =  ( 
                );

# Don't try and pre-process these binary files!
@binary_file_extensions = qw/ jpg bmp png jpeg gif /;





