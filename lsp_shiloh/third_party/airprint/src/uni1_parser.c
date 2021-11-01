/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file uni1_parser.c
 *
 * \brief Print parser for Universal Raster Format, aka airprint
 *
 */
#include "string.h"
#include "urfio.h"
#include "lassert.h"
#include "logger.h"
#include "ColorPipe.h"
#include "ColorPipeTables.h"
#include "cp_strip.h"
#include "urf.h"
#include "debug.h"

#define DBG_PRFX "urf: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 23 )

static const char *unirast_header_string_const = "UNIRAST";

typedef enum 
{
    auto_printer_default_e = 0,
    draft = 3,
    normal = 4,
    best = 5
} unirast_printQuality_enum;

typedef enum
{
    UNIRAST_DUPLEX_MODE_AUTO = 0,
    UNIRAST_DUPLEX_MODE_SIMPLEX = 1,
    UNIRAST_DUPLEX_MODE_LANDSCAPE = 2,
    UNIRAST_DUPLEX_MODE_PORTRAIT = 3,
}unirast_duplex_mode_e;

// page header 
typedef struct unirast_page_s
{
    uint8_t bitsPerPixel; ///< 8, 24, 32, 48, 64 
    uint8_t colorSpace;   ///< 0 == luminace gamma=2.2, 1 == sRGB
    uint8_t duplex_Mode;  ///< auto, 1, short, long 
    uint8_t printQuality; 
    uint8_t mediaType;
    uint8_t inputSlot;
    uint8_t outputBin;
    uint8_t copies;
    uint8_t finishings[4];
    uint32_t width;
    uint32_t height;
    uint32_t resolution; // pixels per inch X and Y 
    uint32_t reserved[2];
} unirast_page_t;

static void uni1_parser_register(void);
int uni1_parser_detect(IOReg *pipe,  urf_page_t * urf_page);
int uni1_header_read(urf_bstream_t *bstream, urf_page_t *urf_page);
bool uni1_parser_128_method(color_pipe_t *cp, object_tag_t * object_tag, bool printable);

static urf_parser_t uni1_parser_struct = 
    {
        uni1_parser_register,
        uni1_parser_detect,
        uni1_header_read,
        urf_parser_decompress,
        uni1_parser_128_method
    };

/**
 * @brief Registers the unirast 1.0 parser with the connection
 *        manager.  All of the *_parser_register routines should
 *        look identical except the ParseCmpString & StringSize.
 */
static void uni1_parser_register(void)
{
    DBG_PRINTF_NOTICE("URF: Setup Unirast Parser\n");
    PARSE_SETUP Parser;
    
    memset((void*)&Parser,0,sizeof(PARSE_SETUP));   // Need to null this structure due to monochrome printers having function
                                                    // pointers is this structure that will be called if not NULL. paw
        
    Parser.ParserID = UNIRAST_MODULE_ID;
    Parser.ParseCmpString = (char*)unirast_header_string_const;
    Parser.StringSize = strlen(Parser.ParseCmpString);
    Parser.ParseString = "URF";
    Parser.ParseType = e_Print;     // On monochrome printers this is the wrong enum. However it will work
                                    //for now since the correct enum of e_Printing is the same value as e_Print. paw

    ParserRegister(&Parser);        // register the parser for unirast.
}
/**
 * @brief Checks for the unirast 1.0 job header.  If it finds it
 *        it advances the stream it not, it puts the stream back
 *        the way it found it.
 * @param pipe: input stream
 * @param urf_page: urf page data
 * 
 * @return int: 0 if no error: -1 if error.
 */
int uni1_parser_detect(IOReg *pipe,  urf_page_t * urf_page)
{
    const int32_t unirast_len = 12; // fileHeader "UNIRAST" numpages
    const int32_t unirast_str_len = 8;
    uint32_t buf[unirast_len / sizeof(uint32_t)];   // union as int as char 
    char *buffer = (char*) &buf[0]; // force alignment
    int32_t bytes_read;

    urf_page->curr_parser = NULL;
    bytes_read = pipe->ReadConnect(pipe, buffer, unirast_len, PARSEREAD_EXACT);
    if (bytes_read == unirast_len)
    {
        if (strncmp(buffer, unirast_header_string_const, unirast_str_len))
        {
            if (bytes_read > 0) 
            {
                pipe->pipe_read_rewind(pipe, buffer, bytes_read);
            }
            return 0;
        }
        urf_page->curr_parser = &uni1_parser_struct;
        DBG_PRINTF_NOTICE("---- urf Selected uni1 Job\n");
        urf_page->job_pages = be32_to_cpu( buf[2] );
        urf_page->job_pages = urf_page->job_pages == 0 ? 99999 : urf_page->job_pages; // unspecified # pages
        return 0;
    }
    else if (bytes_read > 0) 
    {
        pipe->pipe_read_rewind(pipe, buffer, bytes_read);
        return 0;
    }
    else
    {
        DBG_PRINTF_NOTICE("---- urf bad Job %d %x %x %x\n",bytes_read, buf[0], buf[1], buf[2]);
        return -1;
    }
}
/**
 * @brief Reads in the unirast 1.0 header and parses out the
 *        required fields.
 * @param bstream: input bstream
 * @param urf_page: urf page data.
 * 
 * @return int status 0 good: -1 error.
 */
int uni1_header_read(urf_bstream_t *bstream, urf_page_t *urf_page)
{
    DBG_PRINTF_NOTICE("---- Reading uni1 header\n");
    unirast_page_t * unirast_hdr_ptr;
    int32_t bytes_read;

    if (sizeof(unirast_page_t) != (bytes_read = urf_bstream_read(bstream,(uint8_t **) &unirast_hdr_ptr, sizeof(unirast_page_t) ))) //stream to HW Res
    {
        DBG_PRINTF_NOTICE("---- urf read less than len %d bytes_read  %d\n", sizeof(unirast_page_t), bytes_read);
        return -1;
    }
    urf_page->bitsPerPixel = unirast_hdr_ptr->bitsPerPixel; ///< 8, 24, 32, 48, 64 
    urf_page->colorSpace = unirast_hdr_ptr->colorSpace;   ///< 0 == luminace gamma=2.2, 1 == sRGB 

    // Sanity check to try and catch short page --> losing next page header
    if ( ( ( urf_page->bitsPerPixel != 8 ) && 
           ( urf_page->bitsPerPixel != 16 ) && 
           ( urf_page->bitsPerPixel != 24 ) && 
           ( urf_page->bitsPerPixel != 32 ) && 
           ( urf_page->bitsPerPixel != 64) )
         ||
         ( ( urf_page->colorSpace != 0 ) && 
           ( urf_page->colorSpace != 1 ) ) )
    {
        DBG_PRINTF_ERR( "Invalid BPP (%d) or CS (%d) - abort airprint parsing...\n", urf_page->bitsPerPixel, urf_page->colorSpace );
        return URF_ABORT_PARSE;
    }

    switch ((unirast_duplex_mode_e)unirast_hdr_ptr->duplex_Mode)
    {
        case UNIRAST_DUPLEX_MODE_AUTO:
            urf_page->duplex_specified = false;
            break;
        case UNIRAST_DUPLEX_MODE_SIMPLEX:
            urf_page->duplex_specified = true;
            urf_page->duplex_mode = DMDUP_SIMPLEX;
            urf_page->rotate_backside = false;
            break;
        case UNIRAST_DUPLEX_MODE_LANDSCAPE:
            urf_page->duplex_specified = true;
            urf_page->duplex_mode = DMDUP_HORIZONTAL;
            urf_page->rotate_backside = true;
            break;
        case UNIRAST_DUPLEX_MODE_PORTRAIT:
            urf_page->duplex_specified = true;
            urf_page->duplex_mode = DMDUP_VERTICAL;
            urf_page->rotate_backside = true;
            break;
        default:
            DBG_PRINTF_ERR( "Invalid duplex mode (%d) - abort airprint parsing...\n", unirast_hdr_ptr->duplex_Mode);
            return URF_ABORT_PARSE;
    }

    memcpy(&urf_page->width, &unirast_hdr_ptr->width, 4);
    urf_page->width = be32_to_cpu(urf_page->width);
    memcpy(&urf_page->height, &unirast_hdr_ptr->height, 4);
    urf_page->height = be32_to_cpu(urf_page->height);
    memcpy(&urf_page->resolution, &unirast_hdr_ptr->resolution, 4);
    urf_page->resolution = be32_to_cpu(urf_page->resolution);

    // DBG_ASSERT(urf_page->resolution <= 1200);
    if (urf_page->resolution > 1200 || urf_page->height > 20000 || urf_page->bitsPerPixel > 64 || 
#ifdef COLOR_ENG
        urf_page->colorSpace > 1 ) 
#else // !COLOR_ENG
// BUGFIX: AirPrint 1.3 APVT fails if we convert color jobs to mono. Instead the test expects us
// to abort the job and report "doc format not supported"
        urf_page->colorSpace > 0 ) 
#endif // COLOR_ENG
    {
	    hex_dump_named( unirast_hdr_ptr, sizeof(unirast_page_t), (char*)__FUNCTION__ );
	    return URF_ABORT_PARSE;
    }
    switch ( unirast_hdr_ptr->printQuality )
    {
        case draft:
            urf_page->print_quality = PRINTQUALITY_DRAFT;
            urf_page->UnderColorRemoval = true;
            break;

        case best:
            urf_page->print_quality = PRINTQUALITY_BEST;
            urf_page->UnderColorRemoval = false;
            break;

        case normal:
        default:
            urf_page->print_quality = PRINTQUALITY_NORMAL;
            urf_page->UnderColorRemoval = false;
    }

    urf_page->copies = unirast_hdr_ptr->copies;
    return 0;
}
/**
 * @brief white to EOL
 * @param cp:ptr to color pipe
 * @param object_tag:ptr to OT
 */
bool uni1_parser_128_method(color_pipe_t *cp, object_tag_t * object_tag, bool printable)
{
    const uint8_t white[3] = {0xff, 0xff, 0xff};
    if (printable) 
    {
        color_pipe_process_repeated_pixels( cp, 1, object_tag, (uint8_t*) &white[0] ); // const cast
    }
    return true;
    
}
urf_parser_t * uni1_parser_init(void)
{
    return &uni1_parser_struct;
}
