/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include "pip_page_adaptor.h"
#include "pip_PAGE_page_adaptor.h"

#include "logger.h"
#include "memAPI.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "pip_api.h"
#include "print_mode_api.h"

#define DBG_PRFX "page_adaptor: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT(4)

static void pip_PAGE_attach_page( struct pip_page_adaptor *page, void *from_page_blob )
{
    struct pip_PAGE_page_adaptor *ppa = (struct pip_PAGE_page_adaptor * )page;

    ppa->page = (PAGE_DATA*)from_page_blob;

    ppa->scale_x = ppa->page->image_output_info.image_dpi.x == 300 ? 2 : 1;
    ppa->scale_y = ppa->page->image_output_info.image_dpi.y == 300 ? 2 : 1;
    ppa->pix_width = ppa->page->image_output_info.image_width_in_pixels;    
    ppa->out_page_height = ppa->page_height = ppa->page->image_output_info.image_height_in_lines;
}

static void pip_PAGE_send_page( struct pip_page_adaptor *page_adaptor )
{
    MESSAGE msg;

    struct pip_PAGE_page_adaptor *ppa = (struct pip_PAGE_page_adaptor * )page_adaptor;
    PAGE_DATA *page = ppa->page;

    // Adjust output page settings are forward the information to next in the pipeline.

    uint32_t padded_image_width_in_pixels = 
	pip_padded_image_width( page->image_output_info.image_width_in_pixels, 
				ppa->scale_x );
    page->image_output_info.image_width_in_pixels = padded_image_width_in_pixels;
    page->image_output_info.image_bpp = pip_mode_bpp_get(from_page_pip_mode( page_adaptor ));
    page->image_output_info.image_data_width_in_bytes = 
	(padded_image_width_in_pixels * page->image_output_info.image_bpp) / 8; 
    page->image_output_info.image_height_in_lines *= ppa->scale_y;
    page->image_output_info.image_dpi.x *= ppa->scale_x;
    page->image_output_info.image_dpi.y *= ppa->scale_y;
    page->nup_image_params.image_width  *= ppa->scale_x;
    page->nup_image_params.image_height *= ppa->scale_y;
    page->video_justification_mode = pip_mode_justification_get(from_page_pip_mode( page_adaptor ));

    DBG_PRINTF_INFO("%s: page: bpp = %d width_in_pixels = %d, data_width_in_bytes = %d, height_in_lines = %d, ResX = %d, ResY = %d\n", 
                   __func__,
                   page->image_output_info.image_bpp,
                   page->image_output_info.image_width_in_pixels,
                   page->image_output_info.image_data_width_in_bytes,
                   page->image_output_info.image_height_in_lines,
                   page->image_output_info.image_dpi.x,
                   page->image_output_info.image_dpi.y); 

    msg.msgType = MSG_JOBPAGESTART;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = ppa->page;
    SYMsgSend( ppa->destination, &msg );
}

static void pip_PAGE_close_page( struct pip_page_adaptor *page )
{
    struct pip_PAGE_page_adaptor *ppa = (struct pip_PAGE_page_adaptor * )page;
    if (ppa)
	ppa->page = 0; // referenced memory not owned
}

static uint32_t pip_PAGE_pip_mode( struct pip_page_adaptor *page )
{
    struct pip_PAGE_page_adaptor *ppa = (struct pip_PAGE_page_adaptor * )page;
    if ( ppa && ppa->page ) {
	ppa->page->print_mode = print_mode_get( ppa->page );
	ppa->pip_mode = print_mode_get_image_pipe_configuration(ppa->page->print_mode);
    }
    return ppa->pip_mode;
}


struct pip_page_adaptor *construct_pip_PAGE_page_adaptor( 
    struct pip_PAGE_page_adaptor *reuse_if_not_null )
{
    struct pip_PAGE_page_adaptor *ppa = 0;

    if (reuse_if_not_null) 
	ppa = reuse_if_not_null;
    else
	ppa = (struct pip_PAGE_page_adaptor *) MEM_MALLOC( sizeof( struct pip_PAGE_page_adaptor ) );
    if (!ppa) 
	return 0;
    ppa = (struct pip_PAGE_page_adaptor*) construct_pip_page_adaptor( (struct pip_page_adaptor*)ppa );

    ppa->attach_from_page = pip_PAGE_attach_page;
    ppa->pip_send_page = pip_PAGE_send_page;
    ppa->page_close = pip_PAGE_close_page;

    ppa->from_page_pip_mode = pip_PAGE_pip_mode;

    ppa->pip_mode = 1;
    ppa->first_strip = false;
    ppa->page = 0;
    ppa->destination = 0xdeadbeef;

    return (struct pip_page_adaptor *) ppa;
}
