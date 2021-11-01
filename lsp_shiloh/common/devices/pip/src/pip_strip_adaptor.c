/******************************************************************************
 * Copyright (c) 2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "lassert.h"
#include "memAPI.h"
#include "dma_buffer.h"
#include "pip_strip_adaptor.h"
#include "pip_api.h"
#include "logger.h"

#define DBG_PRFX "strip_adaptor: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_GENERATORS | LOGGER_SUBMODULE_BIT(4)

void dbg_dump_from_strip( struct pip_strip_adaptor *psa )
{
    if ( psa && psa->dbg_dump_from_strip_data )
	psa->dbg_dump_from_strip_data( psa );
}
void dbg_dump_to_strip( struct pip_strip_adaptor *psa )
{
    if ( psa && psa->dbg_dump_to_strip_data )
	psa->dbg_dump_to_strip_data( psa );
}
struct pip_cfg *pip_cfg_from_strip( struct pip_strip_adaptor *psa )
{
    if ( psa && psa->get_pip_cfg )
	return psa->get_pip_cfg( psa );
    return 0;
}
static BigBuffer_t * from_strip_get_bigbuffer( struct pip_strip_adaptor *psa )
{
    if ( psa && psa->get_bb_in )
	return psa->get_bb_in( psa );
    return 0;
}
void send_strip( struct pip_strip_adaptor *psa, BigBuffer_t* output_buffer[6] )
{
    if (psa && psa->pip_send_strip )
	psa->pip_send_strip( psa, output_buffer );
}
void free_in_strip( struct pip_strip_adaptor *psa )
{
    if ( psa && psa->free_in_strip )
	psa->free_in_strip( psa );
}
void pip_new_page( struct pip_strip_adaptor *from_strip )
{
    if ( from_strip ) {
	from_strip->is_start_of_page = true;
	if ( from_strip->new_page ) {
	    from_strip->new_page( from_strip );
	}
    }
}
// not needed in base class
// static void pip_new_page_fn( struct pip_strip_adaptor *from_strip ) {}

void pip_recv_strip( struct pip_page_adaptor *from_page, 
		     struct pip_strip_adaptor *from_strip, 
		     void *from_strip_blob )
{
    if ( from_strip && from_strip->recv_strip ) {
	from_strip->recv_strip( from_page, from_strip, from_strip_blob);
	from_strip->is_start_of_page = false;
    }
}

static void pip_recv_strip_fn( struct pip_page_adaptor *from_page, 
			       struct pip_strip_adaptor *from_strip, 
			       void *from_strip_blob )
{
    int have_output; 
    BigBuffer_t* output_buffer[6] = {0,0,0,0,0,0};

    from_strip->attach_from_strip( from_strip, from_strip_blob );
    /// output intent bpp 
    from_strip->output_bpp = pip_mode_bpp_get(from_page_pip_mode( from_page ));

    dbg_dump_from_strip( from_strip );

    from_page->add_strip( from_page, from_strip->lines_in * from_strip->scale_y, from_strip->is_end_of_page );

    bool input_end_of_page = from_strip->is_end_of_page;
    bool output_end_of_page = false; 
    BigBuffer_t *bb_in = from_strip_get_bigbuffer( from_strip );

    do 
    {
	have_output = pip_process_strip(
	    from_page_pip_mode( from_page ),
	    bb_in,
	    from_strip->width_pix_in,
	    from_strip->lines_in,
	    from_strip->out_strip_height,
	    from_strip->output_bpp,
	    from_page->out_page_height,
	    from_strip->scale_x,
	    from_strip->scale_y,
	    from_strip->is_start_of_page,
	    input_end_of_page, // from_strip->is_end_of_page,
	    output_buffer
	);
        if (have_output) 
        {
            // modify to_strip height, bpp, scale,
            // set end_of_image if its the last strip on the page.
	    from_strip->out_strip_height = have_output;  
	    DBG_ASSERT( have_output <= 128 );  // clamping output height
	    output_end_of_page = from_page_at_output_eoi( from_page, from_strip->out_strip_height );
	    from_strip->is_end_of_page = output_end_of_page;  
            send_strip( from_strip, output_buffer );
	    bb_in = 0;
	    // clamped output height means we might have 1 input strip and 2 output strips here at end of page.
	}
	DBG_PRINTF_DEBUG("%s: out EOI %d in EOI %d lines %d \n", 
			 __func__, output_end_of_page, input_end_of_page, have_output );
    } while ( have_output && !output_end_of_page );
    free_in_strip(from_strip); 
}
struct pip_strip_adaptor *construct_pip_strip_adaptor( 
    struct pip_strip_adaptor *reuse_if_not_null )
{
    struct pip_strip_adaptor *psa = 0;

    if (reuse_if_not_null) 
	psa = reuse_if_not_null;
    else 
	psa = (struct pip_strip_adaptor *) MEM_MALLOC( sizeof( struct pip_strip_adaptor ) );
    if (!psa) 
	return 0;
    memset( psa, 0, sizeof( struct pip_strip_adaptor ) );

    psa->recv_strip = pip_recv_strip_fn; // will ask derived call to override the rest.

    return (struct pip_strip_adaptor *) psa;
}    











