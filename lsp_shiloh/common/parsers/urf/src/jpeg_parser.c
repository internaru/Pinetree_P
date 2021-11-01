/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file jpeg_parser.c
 *
 * \brief Print parser for JPEG
 *
 */
#include "string.h"
#include "agRouter.h"
#include "agConnectMgr.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "ResourceMap.h"
#include "SJMgr.h"
#include "sys_init_api.h"

#include "print_job_types.h"
#include "print_job_api.h"
#include "memAPI.h"
#include "lassert.h"
#include "logger.h"
#ifdef HAVE_LASER_COLOR_SUPPORT
#include "PrintJobInfo.h"
#endif

#include "cp_strip.h" 
#include "urf.h"
#include "urfio.h"
#include "jpeg_decompress.h"
#include "jpeg_parser.h"
#include "printvars_api.h"

#define DBG_PRFX "jpeg: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 23 )

static char *jpeg_header_string = "JPEG";


static void jpeg_parser_register(void);
int jpeg_parser_detect(IOReg *pipe,  urf_page_t * urf_page);
int jpeg_header_read(urf_bstream_t *bstream, urf_page_t *urf_page);
int jpeg_parser_decompress(urf_bstream_t *bstream, 
        urf_page_t *urf_page, print_page_t* p_page, 
        bool (*dummy) (color_pipe_t *cp, object_tag_t * object_tag, bool printable));

static urf_parser_t jpeg_parser_struct = 
    {
        jpeg_parser_register,
        jpeg_parser_detect,
        jpeg_header_read,
        jpeg_parser_decompress,
        NULL
    };

/**
 * @brief Registers the jpeg parser with the connection manager.
 *        All of the *_parser_register routines should look
 *        identical except the ParseCmpString & StringSize.
 */
static void jpeg_parser_register(void)
{
    DBG_PRINTF_NOTICE("URF: Setup jpeg Parser\n");
    PARSE_SETUP Parser;
    
    memset((void*)&Parser,0,sizeof(PARSE_SETUP));   // Need to null this structure due to monochrome printers having function
                                                    // pointers is this structure that will be called if not NULL. paw
        
    Parser.ParserID = UNIRAST_MODULE_ID;
    Parser.ParseCmpString = (char*)jpeg_header_string;
    Parser.StringSize = strlen(jpeg_header_string);
    Parser.ParseString = "URF";
    Parser.ParseType = e_Print;     // On monochrome printers this is the wrong enum. However it will work
                                    //for now since the correct enum of e_Printing is the same value as e_Print. paw

    ParserRegister(&Parser);        // register the parser for jpeg.
    Parser.ParseCmpString = (char*)jpeg_header_string;
    ParserRegister(&Parser);        // register the parser for jpeg.
}

/**
 * @brief Checks for the jpeg job header.  If it finds it it
 *        advances the stream if not, it puts the stream back
 *        the way it found it.
 * @param pipe: input stream
 * @param urf_page: urf page data
 * 
 * @return int: 0 if no error: -1 if error.
 */
int jpeg_parser_detect(IOReg *pipe,  urf_page_t * urf_page)
{
    const int32_t jpeg_len = 4; // fileHeader "JPEG" 
    uint32_t buf[jpeg_len / sizeof(uint32_t)];   // union as int as char 
    char *buffer = (char*) &buf[0]; // force alignment
    int32_t bytes_read;
    //jpeg2_isBigEndian(true);
    urf_page->curr_parser = NULL;
    bytes_read = pipe->ReadConnect(pipe, 
                 buffer, jpeg_len, PARSEREAD_EXACT);
    if (bytes_read == jpeg_len)
    {
        if (strncmp(buffer, jpeg_header_string, jpeg_len) != 0)
        {
            if (bytes_read > 0) 
            {
                pipe->pipe_read_rewind(pipe, buffer, bytes_read);
            }
            return 0;
        }
        urf_page->curr_parser = &jpeg_parser_struct;
        DBG_PRINTF_NOTICE("---- urf Selected jpeg Job\n");
        urf_page->job_pages = 99999; // unspecified # pages
        return 0;
    }
    else if (bytes_read > 0) 
    {
        pipe->pipe_read_rewind(pipe, buffer, bytes_read);
        return 0;
    }
    else
    {
        DBG_PRINTF_NOTICE("---- urf bad Job %d %x\n",bytes_read, buf[0]);
        return -1;
    }
}



/**
 * @brief Reads in the jpeg header and parses out the required
 *        fields.
 * @param bstream: input bstream
 * @param urf_page: urf page data.
 * 
 * @return int status 0 good: -1 error.
 */
int jpeg_header_read(urf_bstream_t *urf_bstream, urf_page_t *urf_page)
{
    DBG_PRINTF_NOTICE(" Reading jpeg header\n");
    jpeg_header_t *jpeg_hdr_ptr;
    int32_t bytes_read;
    uint32_t header_size;

    //dbg_printf("%s\n", __FUNCTION__);

    header_size = sizeof(jpeg_header_t);
    bytes_read = urf_bstream_read(urf_bstream,(uint8_t **)&jpeg_hdr_ptr, header_size);
    if (header_size != bytes_read)
    {
        DBG_PRINTF_ERR("---- jpeg header read less than len %d bytes_read  %d\n", 
			  header_size, bytes_read);
        return -1;
    }

    // initialize urf page values from jpeg header values
    if((PRINT_QUALITY)jpeg_hdr_ptr->PrintQuality > PRINTQUALITY_DRAFT)
    {
	urf_page->print_quality = PRINTQUALITY_BEST;
    }
    else
    {
	urf_page->print_quality = (PRINT_QUALITY)jpeg_hdr_ptr->PrintQuality;
    }

    urf_page->colorSpace = jpeg_hdr_ptr->ColorSpace;
    if(urf_page->colorSpace > 2)
    {
	urf_page->colorSpace = 2;
    }

    urf_page->scale_image = jpeg_hdr_ptr->ScaleToFit;
    urf_page->resolution = jpeg_hdr_ptr->ImageResolution;
    if(urf_page->resolution == 0)
    {
	urf_page->resolution = 600;
    }
    // for now ignore the requested dpi and always use 600
    urf_page->resolution = 600;

    if((orientation_t)jpeg_hdr_ptr->Orientation < end_of_orientation_list)
    {
        urf_page->orientation = (orientation_t)jpeg_hdr_ptr->Orientation;
    }
    else
    {
	urf_page->orientation = e_auto;
    }

    urf_page->copies = jpeg_hdr_ptr->NumCopies;
    if(urf_page->copies > 100)
    {
	urf_page->copies = 1;
    }

    // if pagesize == 0 then use the printer default media size
    // also sanity test to make sure the width <= 8.5" and height <= 17"
    if(jpeg_hdr_ptr->PageSize[0] == 0 || jpeg_hdr_ptr->PageSize[1] == 0 ||
       jpeg_hdr_ptr->PageSize[0] > 850 || jpeg_hdr_ptr->PageSize[1] > 1700)
    {
	uint32_t media_widthmm=0;
	uint32_t media_heightmm=0;
	mediasize_t mediasize;

	// determine the default paper dimensions
	printvar_get_default_papersize(&mediasize);
	paper_get_media_size(mediasize, &media_widthmm, &media_heightmm);

	urf_page->width = (media_widthmm * urf_page->resolution) / 2540;
	urf_page->height = (media_heightmm * urf_page->resolution) / 2540;
    }
    else
    {
	urf_page->width = (jpeg_hdr_ptr->PageSize[0] * urf_page->resolution) / 100;
	urf_page->height = (jpeg_hdr_ptr->PageSize[1] * urf_page->resolution) / 100;
    }

    jpeg_decoder_open(urf_bstream, urf_page);

    //dbg_printf("----------jpeg header\n");
    //dbg_printf("  -># copies: %d\n", urf_page->copies);
    //dbg_printf("  ->orientation: %d\n", urf_page->orientation);
    //dbg_printf("  ->scale to fit: %d\n", urf_page->scale_image);
    //dbg_printf("  ->resolution: %d\n", urf_page->resolution);
    //dbg_printf("  ->page width: %d\n", urf_page->width);
    //dbg_printf("  ->page height: %d\n", urf_page->height);
    //dbg_printf("  ->print qual: %d\n", urf_page->print_quality);
    //dbg_printf("  ->color space: %d\n", urf_page->colorSpace);
    //dbg_printf("  ->jpeg hdr pagesize[0]: 0x%x\n", jpeg_hdr_ptr->PageSize[0]);
    //dbg_printf("  ->jpeg hdr pagesize[1]: 0x%x\n", jpeg_hdr_ptr->PageSize[1]);


    urf_page->this_is_jpeg_parser = true;
    // colorSpace 2 is auto
    if(urf_page->colorSpace == 2 && urf_page->bitsPerPixel > BITS_PER_BYTE)
    {
	urf_page->colorSpace = 1;
    }

    // if only 8 bits/pixel all we can do is grayscale
    // so override any user request for color
    if(urf_page->bitsPerPixel == BITS_PER_BYTE)
    {
	urf_page->colorSpace = 0;
    }

    urf_page->job_pages = 1;

    if(urf_page->height % MIN_STRIP_HEIGHT_MULTIPLE != 0)
    {
	// TODO: rather than truncate image pad with white lines
	urf_page->height -= urf_page->height % MIN_STRIP_HEIGHT_MULTIPLE;
    }

    //dbg_printf("-------------w:%d h:%d bbp:%d\n", 
	   //urf_page->width, urf_page->height, urf_page->bitsPerPixel);

    return 0;
}

urf_parser_t * jpeg_parser_init(void)
{
    return &jpeg_parser_struct;
}


int jpeg_parser_decompress(urf_bstream_t *bstream, 
           urf_page_t *urf_page,  print_page_t* p_page,
           bool (*dummy) (color_pipe_t *cp, 
			  object_tag_t * object_tag, bool printable))
{
    DBG_PRINTF_NOTICE("jpeg_parser_decompress\n");
   int stat = jpeg_decompress(bstream, urf_page, p_page);
   jpeg_decoder_close(bstream);
   return stat;
}

