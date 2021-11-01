/******************************************************************************
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file internal_page_parser.c
 *
 * \brief Print parser for internal Universal Raster Format.
 *
 */
#include "string.h"
#include "urfio.h"
#include "lassert.h"
#include "logger.h"
#include "cp_strip.h"
#include "urf.h"
#include "internal_urf_parser.h"


#define DBG_PRFX "urf: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 23 )

static const char *internal_urf_header_string_const = "InTl";

static void internal_urf_parser_register(void);
int internal_urf_parser_detect(IOReg *pipe,  urf_page_t * urf_page);
int internal_urf_header_read(urf_bstream_t *bstream, urf_page_t *urf_page);

static urf_parser_t internal_urf_parser_struct = 
    {
        internal_urf_parser_register,
        internal_urf_parser_detect,
        internal_urf_header_read,
        urf_parser_decompress,
        NULL
    };

/**
 * @brief Registers the internal page parser with the connection
 *        manager. All of the *_parser_register routines should
 *        look identical except the ParseCmpString & StringSize.
 */
static void internal_urf_parser_register(void)
{
    DBG_PRINTF_NOTICE("Setup int_urf Parser\n");
    PARSE_SETUP Parser;
    
    memset((void*)&Parser,0,sizeof(PARSE_SETUP));        
    Parser.ParserID = UNIRAST_MODULE_ID;
    Parser.ParseCmpString = (char*)internal_urf_header_string_const;
    Parser.StringSize = strlen(Parser.ParseCmpString);
    Parser.ParseString = "URF";
    Parser.ParseType = e_Print;

    ParserRegister(&Parser);        // register the parser for internal page data.
}
/**
 * @brief Checks for the internal page job header.  If it finds 
 *        it it advances the stream it not, it puts the stream
 *        back the way it found it.
 * @param pipe: input stream
 * @param urf_page: urf page data
 * 
 * @return int: 0 if no error: -1 if error.
 */
int internal_urf_parser_detect(IOReg *pipe,  urf_page_t * urf_page)
{
    const int32_t int_urf_str_len = strlen(internal_urf_header_string_const);
    const int32_t int_urf_len = int_urf_str_len + 4 + 32; // fileHeader compare string + 4 bytes for numpages + 32 bytes for doc type
    uint32_t buf[int_urf_len / sizeof(uint32_t)];   // union as int as char 
    char *buffer = (char*) &buf[0]; // force alignment
    char *name;
    int32_t bytes_read;

    urf_page->curr_parser = NULL;
    bytes_read = pipe->ReadConnect(pipe, buffer, int_urf_len, PARSEREAD_EXACT);
    if (bytes_read == int_urf_len)
    {
        if (strncmp(buffer, internal_urf_header_string_const, int_urf_str_len))
        {
            if (bytes_read > 0) 
            {
                pipe->pipe_read_rewind(pipe, buffer, bytes_read);
            }
            return 0;
        }
        urf_page->curr_parser = &internal_urf_parser_struct;
        DBG_PRINTF_NOTICE("---- urf Selected uni1 Job\n");
        urf_page->job_pages = be32_to_cpu( buf[1] );
        urf_page->job_pages = urf_page->job_pages == 0 ? 99999 : urf_page->job_pages; // unspecified # pages
        name = buffer + 8;
        if ( ( strncmp( name, "calibration page", strlen( "calibration page" ) ) == 0 ) ||
             ( strncmp( name, "density cal page", strlen( "density cal page" ) ) == 0 ) ||
             ( strncmp( name, "registration cal page", strlen( "registration cal page" ) ) == 0 ) )
        {
            urf_page->doc_type = e_CALPAGE;
        }
        else if ( strncmp( name, "cleaning page", strlen( "cleaning page" ) ) == 0 )
        {
            urf_page->doc_type = e_CLEANINGPAGE;
        }
        else
        {
            urf_page->doc_type = e_DOCUMENT;
        }

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
 * @brief Reads in the internal page header and parses out the
 *        required fields.
 * @param bstream: input bstream
 * @param urf_page: urf page data.
 * 
 * @return int status 0 good: -1 error.
 */
int internal_urf_header_read(urf_bstream_t *bstream, urf_page_t *urf_page)
{
    DBG_PRINTF_NOTICE("---- Reading int_urf header\n");
    internal_urf_page_t * int_urf_hdr_ptr;
    int32_t bytes_read;

    if (sizeof(internal_urf_page_t) != (bytes_read = urf_bstream_read(bstream,(uint8_t **) &int_urf_hdr_ptr, sizeof(internal_urf_page_t) ))) //stream to HW Res
    {
        DBG_PRINTF_NOTICE("---- urf read less than len %d bytes_read  %d\n", sizeof(internal_urf_page_t), bytes_read);
        return -1;
    }

    urf_page->bitsPerPixel = int_urf_hdr_ptr->bits_per_pixel; ///< 8, 24 
    urf_page->colorSpace = int_urf_hdr_ptr->color_space;   ///< 0 == luminace gamma=2.2, 1 == sRGB 
    memcpy(&urf_page->width, &int_urf_hdr_ptr->width, 4);
    urf_page->width = be32_to_cpu(urf_page->width);
    memcpy(&urf_page->height, &int_urf_hdr_ptr->height, 4);
    urf_page->height = be32_to_cpu(urf_page->height);
    memcpy(&urf_page->resolution, &int_urf_hdr_ptr->resolution, 4);
    urf_page->resolution = be32_to_cpu(urf_page->resolution);
    urf_page->copies = urf_page->copies;
    urf_page->UnderColorRemoval = true;
    return 0;
}
urf_parser_t * internal_urf_parser_init(void)
{
    return &internal_urf_parser_struct;
}
