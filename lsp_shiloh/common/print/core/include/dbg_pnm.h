/*
 * ============================================================================
 * Copyright (c) 2013   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include "print_job_types.h"

/// overrideable functions that do nothing by default, and output
/// pnm files representing the data in a plane.
///

/// write a ppm RGB 24 bit color raster file.
///
void dbg_dump_stripdata_to_ppm_file( int pagecnt, PLANED *plane, int planecount );

/// write a pgm 8 bit gray raster file.
///
void dbg_dump_stripdata_to_pgm_file( int pagecnt, PLANED *plane, int planecount );

/// write a pbm 1 bit mono raster file.
///
/// Caveat: if used for 2 or 4 bit data the data will need to be post processed.
///
void dbg_dump_stripdata_to_pbm_file( int pagecnt, int color, PLANED *plane, int planecount );

/// Set the path/file_name to enable output via the CMD interface default is off.
/// 
int dbg_dump_stripdata_filename_cb( int argc, char *argv[] );
int dbg_dump_mono_filename_cb( int argc, char *argv[] );
int dbg_dump_gray_filename_cb( int argc, char *argv[] );
int dbg_dump_color_filename_cb( int argc, char *argv[] );

/// walks page list dumping a mono page  
void dbg_dump_page_stripdata_to_pbm_file( PAGE_DATA *page );
