/*
 * ============================================================================
 * Copyright (c) 2013   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include "dbg_pnm.h"

void __attribute__((weak)) dbg_dump_stripdata_to_ppm_file( int pagecnt, PLANED *plane, int planecount )
{
}           

void __attribute__((weak)) dbg_dump_stripdata_to_pgm_file( int pagecnt, PLANED *plane, int planecount )
{
}
           
void  __attribute__((weak)) dbg_dump_stripdata_to_pbm_file( int pagecnt, int color, PLANED *plane, int planecount )
{
}

int  __attribute__((weak)) dbg_dump_mono_filename_cb( int argc, char *argv[] )
{
	return 0;
}
int  __attribute__((weak)) dbg_dump_gray_filename_cb( int argc, char *argv[] )
{
	return 0;
}
int  __attribute__((weak)) dbg_dump_color_filename_cb( int argc, char *argv[] )
{
	return 0;
}
void  __attribute__((weak)) dbg_dump_page_stripdata_to_pbm_file( PAGE_DATA *page )
{
}

int __attribute__((weak)) dbg_validate_page( PAGE_DATA *page )
{
    return 0;
}
