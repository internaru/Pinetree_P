/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#include <stdint.h>

#include "print_mode.h"
#include "logger.h"
#include "lassert.h"
#include "debug.h"
#include "pip_api.h"
#include "print_mode_api.h"

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define DBG_PRFX "printmode: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 19 )
#undef DBG_LVL

/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */

static void print_mode_get_copy_page( PAGE_DATA *page, print_mode_t *mode );
static void print_mode_get_print_page( PAGE_DATA *page, print_mode_t *mode );
static void print_mode_get_zjs_print_page( PAGE_DATA *page, print_mode_t *mode);
static void print_mode_get_fax_page( PAGE_DATA *page, print_mode_t *mode );
void print_mode_display( print_mode_t *mode );
const char* to_string_print_mode_name( print_mode_name_t name );

uint32_t print_mode_get_sizeof()
{
	DBG_PRINTF_NOTICE("%s  = %d\n", __func__, sizeof(print_mode_t));
    return sizeof(print_mode_t);
}
uint32_t print_mode_get_input_bpp( print_mode_t * print_mode )
{
    ASSERT(print_mode != NULL);
    return print_mode->image_input_info.image_bpp;
}

uint32_t print_mode_get_output_bpp(  print_mode_t * print_mode )
{
    ASSERT(print_mode != NULL);
    return print_mode->image_output_info.image_bpp;
}

uint32_t print_mode_get_image_pipe_configuration(print_mode_t * print_mode )
{
    return print_mode->image_pipe_configuration;
}

uint32_t print_mode_get_image_width_in_pixels( print_mode_t *print_mode )
{
    return print_mode->image_output_info.image_width_in_pixels;
}

uint32_t print_mode_get_halftone( print_mode_t *print_mode )
{
    return print_mode->halftone;
}

print_mode_t *print_mode_get( PAGE_DATA *page)
{
    print_mode_t* mode = page->print_mode;
    DBG_PRINTF_DEBUG("\n********** %s ENTRY **********\n", __func__);


    DBG_PRINTF_NOTICE("Document type: %s\n", to_string_document_type( page->DocType ));

    /*
     *  Note that PIP may change the selected print mode.  Therefore any
     *  video adjustments have to wait until then.
     *
     *  Also be aware that this will be called by both job and the generator.
     *  Some of the input variables aren't set when called by job, so the
     *  recorded input image information is questionable.  Also be aware if
     *  adjusting any page settings that this is getting called multiple times
     *  per page.
     */

    // Record input image information
    if (page->print_mode->image_input_info_init == false)
    {
        DBG_PRINTF_NOTICE("RECORDING INPUT IMAGE INFO %d\n", page->image_output_info.image_bpp);
        page->print_mode->image_input_info = page->image_output_info; /// WTF input and output can vary!
        page->print_mode->image_input_info_init = true;
    }

    // Set default halftone to threshold, modify as necessary.
    page->print_mode->halftone = THRESHOLD_HALFTONE;

    // Set initial output_resolution based on the page.  In theory we will set
    // this later to the appropriate resolution for the print mode.
    mode->image_output_info.image_dpi = page->image_output_info.image_dpi;

    // set name and image_pip_configuration based on doc type and quality
    mode->name = PRINT_DRAFT;
    mode->image_pipe_configuration = IPC_UNUSED;

    // This is mono only.  What exactly does num_color_planes indicate?  The
    // number of output planes?  Is this different than data derived from the
    // image_pipe_configuration data?  What do we do if those don't match?
    mode->num_color_planes = 1;

    mode->color = (page->page_output_color_mode == e_Color);
    DBG_PRINTF_NOTICE("PAGE is %s\n", mode->color ? "COLOR" : "MONOCHROME");

    // We still need to set:
    //    mode->output_bpp

    // These functions will set:
    //     mode->name
    //     mode->image_pipe_configuration
    switch (page->DocType)
    {
	case e_SYSTEMREPORT:
	case e_NETWORKREPORT:
	case e_SPREPORT:
	case e_FAXTXRXREPORT:
	case e_FAXTCR:
	case e_MEMTXJOBLIST:
	case e_T30LOGREPORT:
	case e_TCRFORFAXSERVICE:
	case e_SPAMFAXLIST:
	case e_BROADCASTRESULTREPORT:
		 // SDH Report print job
		 print_mode_get_print_page( page, page->print_mode );
		 break;
    case e_DOCUMENT:
        // Standard print job
        // Threshold halftone
        print_mode_get_print_page( page, mode );
        break;

    case e_ZJS_DOCUMENT:
        // ZJS host based print job
        print_mode_get_zjs_print_page( page, page->print_mode );
        print_mode_get_print_page( page, page->print_mode );
        break;

    case e_COPYPAGE:
        print_mode_get_copy_page( page, mode );
        break;

    case e_FAX:
        print_mode_get_fax_page( page, mode );
        break;

    case e_DEMOPAGE:
    case e_CONFIGPAGE:
    case e_MENUPAGE:
    case e_SUPPLYPAGE:
        break;

    case e_ERRORREPORT:
    case e_SCANCALPAGE:

    case e_CALPAGE:
    case e_ALIGNMENTPAGE:
    case e_CLEANINGPAGE:
    case e_PQPAGE:
    case e_WIRELESSTEST:
    case e_DIAGNOSTIC:
    case e_PHOTOLAYOUT:
    case e_DDPAGE:
    default:
        ASSERT(0);
        break;
    }
        DBG_PRINTF_NOTICE("PAGE is MONOCHROME\n");

    // Set output_bpp from the mode
    mode->image_output_info.image_bpp = pip_mode_bpp_get(mode->image_pipe_configuration);
    DBG_PRINTF_NOTICE("PAGE VIDEO bpp: %d, print mode bpp: %d\n", page->image_output_info.image_bpp, mode->image_output_info.image_bpp);

    // TODO:  Set page->print_mode->image_info

#if 0
    // Set various page data appropriately.

    //data_width_in_bytes = (mode->output_bpp * page->RASTER_X) / (page->VIDEO_BPP/8);
    image_width_in_pixels = page->RASTER_X / page->VIDEO_BPP;
    image_width_in_pixels *= mode->output_resolution.x / page->RESOLUTION_X;
    page->VIDEO_BPP = mode->output_bpp;
    page->RASTER_X = image_width_in_pixels * mode->output_bpp;
    page->VIDEO_X = image_width_in_pixels;
    //data_width_in_bytes = (image_width_in_pixels * (image_bpp/8);
    page->RESOLUTION_X = mode->output_resolution.x;
#endif

    DBG_PRINTF_NOTICE("    PAGE resolution X: %d, Y: %d\n", page->image_output_info.image_dpi.x, page->image_output_info.image_dpi.y);
    DBG_PRINTF_NOTICE("    PAGE width in pixels: %d, height: %d\n", page->image_output_info.image_width_in_pixels, page->image_output_info.image_height_in_lines);
    DBG_PRINTF_NOTICE("    PAGE width in bytes: %d\n", page->image_output_info.image_data_width_in_bytes);
    DBG_PRINTF_NOTICE("    PAGE document type %s\n", to_string_document_type(page->DocType) );
    DBG_PRINTF_NOTICE("    PAGE print quality %s\n", to_string_print_quality(page->PrintQuality) );


    DBG_PRINTF_NOTICE("********** %s EXIT **********\n\n", __func__);

    print_mode_display( mode );

    return mode;
}

uint32_t print_mode_get_bpp(print_mode_t *print_mode)
{
    return print_mode->image_output_info.image_bpp;
}

uint32_t print_mode_get_image_pipe_cfg( print_mode_t *print_mode )
{
    return print_mode->image_pipe_configuration;
}

static void print_mode_get_copy_page( PAGE_DATA *page, print_mode_t *mode )
{
    DBG_PRINTF_NOTICE("%s\n", to_string_print_quality(page->PrintQuality) );

    switch (page->PrintQuality)
    {
    case PRINTQUALITY_DRAFT:
        mode->image_pipe_configuration = IPC_COPY_MONO8_DETAIL;
        mode->name = COPY_DRAFT;
        break;

    case PRINTQUALITY_NORMAL:	// Text, Mixed Copy Mode
    case PRINTQUALITY_BETTER:
        mode->image_pipe_configuration = IPC_COPY_MONO8_DETAIL;
        mode->name = COPY_MIXED;
        break;

    case PRINTQUALITY_BEST:		// Flim Copy Mode
        //mode->image_pipe_configuration = IPC_COPY_MONO8_DETAIL;
        mode->image_pipe_configuration = IPC_COPY_MONO8_FILM_DETAIL;	// mod.lsh.for SDH tuning
        mode->image_output_info.image_bpp = 2;
        mode->name = COPY_PHOTO;
        break;

    default:
        ASSERT(0);
        break;
    }
    // threshold for now ... lsptodo.
    //mode->image_pipe_configuration =  IPC_PRINT_MONO8_DETAIL;
}

static void print_mode_get_print_page( PAGE_DATA *page, print_mode_t *mode )
{
    DBG_PRINTF_NOTICE("%s\n", to_string_print_quality(page->PrintQuality) );

    // REVISIT DAB Need to accomodate the case of RGB input data rendered on a
    // mono printer. Should printer capabilities be known?

    if (page->page_output_color_mode) {
       mode->image_pipe_configuration = IPC_UNUSED;
       return;
    }

    

    switch (page->PrintQuality)
    {
    case PRINTQUALITY_DRAFT:
	    mode->image_pipe_configuration = page->page_output_color_mode ? 
		    IPC_PRINT_COLOR24_1BPP : 
	    page->input_color_space == e_subtractive_gray ? 
		    IPC_PRINT_MONO8_DETAIL : 
	    page->input_color_space == e_additive_gray ? 
		    IPC_PRINT_ADDITIVE_GRAY8_DETAIL : IPC_PRINT_RGB24_1BPP;
	    mode->name = PRINT_DRAFT;
        break;

    case PRINTQUALITY_NORMAL:
    case PRINTQUALITY_BETTER:
	    mode->image_pipe_configuration = page->page_output_color_mode ? 
		    IPC_PRINT_COLOR24_1BPP : 
	    page->input_color_space == e_subtractive_gray ? 
		    IPC_PRINT_MONO8_DETAIL : 
	    page->input_color_space == e_additive_gray ? 
		    IPC_PRINT_ADDITIVE_GRAY8_DETAIL : IPC_PRINT_RGB24_1BPP;
	    mode->name = PRINT_NORMAL;
        break;

    case PRINTQUALITY_BEST:
	    mode->image_pipe_configuration = page->page_output_color_mode ? 
		    IPC_PRINT_COLOR24_1BPP : 
	    page->input_color_space == e_subtractive_gray ? 
		    IPC_PRINT_MONO8_SMOOTH : 
	    page->input_color_space == e_additive_gray ? 
		    IPC_PRINT_ADDITIVE_GRAY8_SMOOTH : IPC_PRINT_RGB24_1BPP;
	    
	    mode->name = PRINT_PHOTO;
        break;
  
    default:
        ASSERT(0);
        break;
    }
}

static void print_mode_get_zjs_print_page( PAGE_DATA *page, print_mode_t *mode )
{

    DBG_PRINTF_NOTICE("%s\n", to_string_print_quality(page->PrintQuality) );
    DBG_PRINTF_NOTICE("     video bpp = %d\n", page->image_output_info.image_bpp );

    
    mode->image_output_info.image_dpi.x = 600;
    mode->image_output_info.image_dpi.y = 600;
    mode->image_input_info.image_bpp = mode->image_output_info.image_bpp = page->image_output_info.image_bpp;


    switch (page->PrintQuality)
    {
    case PRINTQUALITY_DRAFT:
    case PRINTQUALITY_NORMAL:
    case PRINTQUALITY_BETTER:
    case PRINTQUALITY_BEST:
        
        // Nothing to see here: pay attention to the video bpp (see above)!
        //mode->input_bpp = 2;
        //mode->output_bpp = 2;
        //page->VIDEO_BPP = mode->output_bpp;

        break;

    default:
        ASSERT(0);
        break;
    }

    
    DBG_PRINTF_NOTICE("%s print mode is %d\n", __func__, mode->image_pipe_configuration);
}

static void print_mode_get_fax_page( PAGE_DATA *page, print_mode_t *mode )
{
    //mode->image_input_info.image_bpp = 1;   // BUGBUG: why set input info here?
    //mode->image_output_info.image_bpp = 1;
    //mode->image_output_info.image_dpi.x = 300;
    //mode->image_output_info.image_dpi.y = 300;
    //mode->image_input_info.image_bpp = mode->image_output_info.image_bpp = page->image_output_info.image_bpp;
//taeyoukim add.
    mode->image_pipe_configuration = IPC_PRINT_MONO8_DETAIL;
    mode->name = PRINT_NORMAL;
}


// Debug functions

void print_mode_display( print_mode_t *mode )
{
    DBG_PRINTF_NOTICE("%s ENTRY \n", __func__);

    DBG_PRINTF_NOTICE("output bpp: %d\n", mode->image_output_info.image_bpp);
    DBG_PRINTF_NOTICE("pipe configuration %d\n", mode->image_pipe_configuration);
    DBG_PRINTF_NOTICE("color %d\n", mode->color);
    DBG_PRINTF_NOTICE("num color planes %d\n", mode->num_color_planes);
    DBG_PRINTF_NOTICE("output resolution x: %d, y: %d \n", mode->image_output_info.image_dpi.x,  mode->image_output_info.image_dpi.y);
    DBG_PRINTF_NOTICE("name %s\n", to_string_print_mode_name(mode->name));

}


const char* to_string_print_mode_name( print_mode_name_t name )
{
    switch ( name )
    {
        CASE_ENUM_RETURN_STRING( PRINT_DRAFT );
        CASE_ENUM_RETURN_STRING( PRINT_NORMAL );
        CASE_ENUM_RETURN_STRING( PRINT_PHOTO );
        CASE_ENUM_RETURN_STRING( PRINT_FAX );
        CASE_ENUM_RETURN_STRING( COPY_TEXT );
        CASE_ENUM_RETURN_STRING( COPY_DRAFT );
        CASE_ENUM_RETURN_STRING( COPY_MIXED );
        CASE_ENUM_RETURN_STRING( COPY_PHOTO );
        DEFAULT_CASE_ENUM_RETURN_STRING( PLANE_TYPE );
    }

    return "NOT SUPPORTED";
}
