/*
 * ============================================================================
 * Copyright (c) 2013   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include "dbg_pnm.h"

#include <stdio.h>
#include <stdlib.h>
#include "dma_buffer.h"
#include "cmd_proc_api.h"
#include "lassert.h"
#include "logger.h"
#include "jbig.h"
#include "ioutils.h"

/// implementation is dependant on having a file system 

#define DBG_PRFX "dbgpnm: "
#define LOGGER_SUBMOD LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD


static char *dump_path = 0; 
static bool dump_color = false;
static bool dump_gray = false;
static bool dump_mono = false;

void dbg_dump_stripdata_to_ppm_file( int pagecnt, PLANED *plane, int planecount )
{
	char path[200];
	FILE *fp;

	if ( dump_path && dump_color ) {
		ASSERT( plane );
		ASSERT( plane->image_info.image_bpp == 24 );

		sprintf(path, "%s_p_%02d_s_%03d.ppm", 
			dump_path, pagecnt, planecount);
		
		if ( !(fp = fopen(path, "w")) ) {
			return;
		}
		BigBuffer_t *output_buffer = plane->big_buffer;

		dma_buffer_map_single(output_buffer, DMA_TO_DEVICE);
		dma_buffer_unmap_single(output_buffer, DMA_TO_DEVICE);
		void *data = dma_buffer_mmap_forcpu(output_buffer);
 	
		sprintf(path, "P6 %d %d 255 \n",  
			plane->image_info.image_data_width_in_bytes / 3, // rbg byte pixels.
			plane->image_info.image_height_in_lines );
		fwrite(path, 1, strlen(path)+1, fp); 
		fwrite(data, 1, plane->big_buffer->datalen, fp);
		fclose(fp);		
		dma_buffer_unmmap_forcpu( output_buffer );

	}
}

void dbg_dump_stripdata_to_pgm_file( int pagecnt, PLANED *plane, int planecount )
{
	char path[200];
	FILE *fp;

	if ( dump_path && dump_gray ) {
		ASSERT( plane );
		ASSERT( plane->image_info.image_bpp == 8 );

		sprintf(path, "%s_p_%02d_s_%03d.pgm", 
			dump_path, pagecnt, planecount);
		
		if ( !(fp = fopen(path, "w"))) {
			return;
		}
		BigBuffer_t *output_buffer = plane->big_buffer;

		dma_buffer_map_single(output_buffer, DMA_TO_DEVICE);
		dma_buffer_unmap_single(output_buffer, DMA_TO_DEVICE);
		void *data = dma_buffer_mmap_forcpu(output_buffer);
 	
		sprintf(path, "P5 %d %d 255 \n",  
			plane->image_info.image_data_width_in_bytes,
			plane->image_info.image_height_in_lines );
		fwrite(path, strlen(path)+1, 1, fp); 
		fwrite(data, plane->big_buffer->datalen, 1, fp);
		fclose(fp);
		dma_buffer_unmmap_forcpu( output_buffer );

	}
}

void dbg_dump_stripdata_to_pbm_file( int pagecnt, int color, PLANED *plane, int planecount )
{
    char path[200];
    FILE *fp;
    JBIG_BIH jbig_header = {0};

    if ( dump_path && dump_mono ) {
	ASSERT( plane  );
	ASSERT( plane->image_info.image_bpp < 8 ); // 2 and 4 bit will need post processing.
	if ( plane->image_info.image_bpp != 1) {
	    DBG_PRINTF_ERR("pbm output with bit per pixel of %d\n", plane->image_info.image_bpp);
	}
        if (plane->compression_info.comp_type == COMPTYPE_JBIG)
        {
            sprintf(path, "%s_p_%02d_c_%1d_s_%03d.jbg", dump_path, pagecnt, color, planecount);
        }
        else
        {
            sprintf(path, "%s_p_%02d_c_%1d_s_%03d.pbm", dump_path, pagecnt, color, planecount);
        }
	if ( !(fp = fopen(path, "w")) ) {
	    return;
	}
	BigBuffer_t *output_buffer = plane->big_buffer;
	
	dma_buffer_map_single(output_buffer, DMA_TO_DEVICE);
	dma_buffer_unmap_single(output_buffer, DMA_TO_DEVICE);
	void *data = dma_buffer_mmap_forcpu(output_buffer);
 	
        if (plane->compression_info.comp_type == COMPTYPE_JBIG)
        {
            jbig_header.Xd = cpu_to_be32(plane->compression_info.comp_details.jbig.Xd);
            jbig_header.Yd = cpu_to_be32(plane->compression_info.comp_details.jbig.Yd);
            jbig_header.L0 = cpu_to_be32(plane->compression_info.comp_details.jbig.L0);
            jbig_header.Options = plane->compression_info.comp_details.jbig.option_flags;
            jbig_header.P = 1;
            jbig_header.Mx = 16;
            jbig_header.Order = 3;
            jbig_header.Options |= 0x5c;

            fwrite(&jbig_header, sizeof(JBIG_BIH), 1, fp);
        }
        else
        {
            sprintf(path, "P4 %d %d \n",  
		    plane->image_info.image_data_width_in_bytes * 8,
		    plane->image_info.image_height_in_lines );
            fwrite(path, strlen(path)+1, 1, fp); 
        }
	fwrite(data, plane->big_buffer->datalen, 1, fp);
	fclose(fp);
	dma_buffer_unmmap_forcpu( output_buffer );
	
    }
}

void dbg_dump_page_stripdata_to_pbm_file( PAGE_DATA *page )
{
	if ( !dump_path || !dump_mono ) {
		return;
	}
	PAGE_DATA *current_page = page;
	PLANED* plane;
	int color, stripcnt;
	ATLISTENTRY* node;
	static int pagecnt = 0;
	PAGE_DATA *inpage = current_page;
	
	// Need to decrement UseCount so that the memory
	// for this page will be freed
	
	ASSERT( current_page && current_page->sheet );
        DBG_PRINTF_ERR( " sheet ready %d\n", current_page->sheet->sheetReady );
	DBG_PRINTF_ERR( " stub paper %d tray %d mediatype %d\n", 
			current_page->PAPER, 
			current_page->DEFAULTSOURCE,
			current_page->mediaType );
	
	if ( current_page->sheet->sheetReady ) { // then delete sheet else wait for complete sheet before printing
		if (current_page->sheet->BackPage) {
			pagecnt++;
			current_page = inpage->sheet->BackPage;
			for (color = 0; color < NUM_MARKING_COLOR_CHANNELS; color++) {
				for ( stripcnt = 0, node = MListHead(&current_page->planeDataPendingPrintList[color]) ;
				      NULL != node ;
				      stripcnt++, node = MListNext(&current_page->planeDataPendingPrintList[color], node)) {
					plane = CONTAINING_RECORD(node, PLANED, listNode);
					// plane->UseCount--;
					DBG_PRINTF_ERR( " page %d color %d plane %d page %p front %p back %p duplex %x\n",
							pagecnt, color, stripcnt, 
							current_page, current_page->sheet->FrontPage, 
							current_page->sheet->BackPage, current_page->DUPLEX );
					dbg_dump_stripdata_to_pbm_file( pagecnt, color, plane, stripcnt );
				}
			}
		}
		current_page = inpage->sheet->FrontPage;
		if (current_page) { // have front page.
			pagecnt++;
			for (color = 0; color < NUM_MARKING_COLOR_CHANNELS; color++) {
				for ( stripcnt = 0, node = MListHead(&current_page->planeDataPendingPrintList[color]) ;
				      NULL != node ;
				      stripcnt++, node = MListNext(&current_page->planeDataPendingPrintList[color], node)) {
					plane = CONTAINING_RECORD(node, PLANED, listNode);
					// plane->UseCount--;
					DBG_PRINTF_ERR( " page %d color %d plane %d page %p front %p back %p duplex %x \n",
							pagecnt, color, stripcnt, 
							current_page, current_page->sheet->FrontPage, 
							current_page->sheet->BackPage, current_page->DUPLEX );
					dbg_dump_stripdata_to_pbm_file( pagecnt, color, plane, stripcnt );
				}
			}
		}
	}			
}

int dbg_validate_page( PAGE_DATA *page )
{
	XASSERT(IS_PAGE_DATA(page), (uint32_t)page->page_data_cookie);
	DBG_PRINTF_ERR( " %s page %p \n", __FUNCTION__, page );
	extern void PageList_Print(SHEET_DATA* pSheetData);
	PageList_Print(page->sheet);
	return 0;
}


int dbg_dump_stripdata_filename_cb( int argc, char *argv[] )
{
#ifdef HAVE_CMD
	static char dump_path_buffer[128] = "/data/dump/page";
	
	dump_mono = !dump_mono;

	dump_path = dump_path_buffer;
	
#endif
	return 0;
}

int dbg_dump_mono_filename_cb( int argc, char *argv[] )
{
#ifdef HAVE_CMD
	static char dump_path_buffer[128] = "/data/dump/page";
	
	dump_mono = !dump_mono;
	dump_path = dump_path_buffer;
	
#endif
	return 0;
}

int dbg_dump_gray_filename_cb( int argc, char *argv[] )
{
#ifdef HAVE_CMD
	static char dump_path_buffer[128] = "/data/dump/page";
	
	dump_gray = !dump_gray;
	dump_path = dump_path_buffer;
	
#endif
	return 0;
}

int dbg_dump_color_filename_cb( int argc, char *argv[] )
{
#ifdef HAVE_CMD
	static char dump_path_buffer[128] = "/data/dump/page";
	
	dump_color = !dump_color;
	dump_path = dump_path_buffer;
	
#endif
	return 0;
}
