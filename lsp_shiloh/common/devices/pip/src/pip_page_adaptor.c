/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "pip_page_adaptor.h"
#include "pip_api.h"
#include "dma_buffer.h"


#if 1 // if we want logger printfs
#include "logger.h"
#define DBG_PRFX "page_adaptor: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT(4)
#endif


void pip_recv_page( struct pip_page_adaptor *ppa, void *page_blob )
{
    if ( ppa && ppa->recv_page )
	(ppa->recv_page)( ppa, page_blob );
    DBG_ASSERT(ppa && ppa->recv_page);
}
static void pip_recv_page_fn( struct pip_page_adaptor *ppa, 
			      void *page_blob )
{
    uint32_t width_pix_out;

    ppa->strip_in_cnt = 0;
    ppa->strip_out_cnt = 0;
    ppa->strip_in_height = 0;
    ppa->strip_out_height = 0;

    ppa->attach_from_page( ppa, page_blob );

    pip_prepare_page( from_page_pip_mode( ppa ),
		      ppa->scale_x,
		      ppa->scale_y,
		      ppa->pix_width,
		      ppa->pix_width,
		      ppa->page_height,
		      1, true, false, false, 
		      &width_pix_out
	);

    
    ppa->pip_send_page( ppa );
}

static void add_strip_fn( struct pip_page_adaptor *page, uint32_t height, bool end_of_image )
{
    if ( page && page->from_page_pip_mode )
    {
	page->strip_in_cnt ++;

	page->strip_in_height += height;
	if (end_of_image) 
	    page->out_page_height = page->strip_in_height;
    }
}

bool from_page_at_output_eoi( struct pip_page_adaptor *page, uint32_t output_strip_height )
{
    bool ret = false;

    page->strip_out_cnt ++;
    page->strip_out_height += output_strip_height;
    ret = page->out_page_height && (page->strip_out_height >= page->strip_in_height);
    if (page->out_page_height)
	DBG_PRINTF_WARNING("%s: outpageY %d outstripY %d stripheight %d EOI %d incnt %d outcnt %d\n", 
			   __func__, page->out_page_height, page->strip_out_height,  page->strip_in_height,
			   ret,
			   page->strip_in_cnt,
			   page->strip_out_cnt
	    );
    return ret;
}

uint32_t from_page_pip_mode( struct pip_page_adaptor *ppa )
{
    if ( ppa && ppa->from_page_pip_mode )
	return ppa->from_page_pip_mode( ppa );
    DBG_ASSERT( ppa && ppa->from_page_pip_mode );
    return 101;
}

void from_page_close( struct pip_page_adaptor *ppa )
{
    pip_close_page(); // do base class pip close 
    if ( ppa ) { 
	if ( ppa->page_close ) {
	    ppa->page_close( ppa ); // do derived class page close
	}
    }
    DBG_ASSERT( ppa );
}

struct pip_page_adaptor *construct_pip_page_adaptor( 
    struct pip_page_adaptor *reuse_if_not_null )
{
    struct pip_page_adaptor *ppa = 0;

    if (reuse_if_not_null) 
	ppa = reuse_if_not_null;
    else
	ppa = (struct pip_page_adaptor *) MEM_MALLOC( sizeof( struct pip_page_adaptor ) );
    if (!ppa) 
	return 0;
    memset( ppa, 0, sizeof( struct pip_page_adaptor ) ); // NULL all the function pointers need override.

    ppa->recv_page = pip_recv_page_fn;  ///< base class should do the right thing.
    ppa->add_strip = add_strip_fn;      ///< base class should do the right thing.

    /// defaults ...
    ppa->scale_x = 1;
    ppa->scale_y = 1;
    ppa->pix_width = 8.5 * 600;
    ppa->page_height = 11 * 600;


    return (struct pip_page_adaptor *) ppa;
}

