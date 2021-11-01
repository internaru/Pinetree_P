/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file cups3_parser.c
 *
 * \brief Print parser for Cups raster 3.0 format
 *
 */
#include "string.h"

#include "lassert.h"
#include "logger.h"
#include "cp_strip.h"
#include "urf.h"
#include "urfio.h"

#define DBG_PRFX "cups3: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 23 )

static const char *cups3BE_header_string_const = "RaS3";
static const char *cups3LE_header_string_const = "3SaR";


static void cups3_parser_register(void);
int cups3_parser_detect(IOReg *pipe,  urf_page_t * urf_page);
int cups2_header_read(urf_bstream_t *bstream, urf_page_t *urf_page);
void cups2_isBigEndian(bool isBigEndian);
static urf_parser_t cups3_parser_struct = 
    {
        cups3_parser_register,
        cups3_parser_detect,
        cups2_header_read,
        urf_parser_uncompressed_data,
        NULL
    };


/**
 * @brief Registers the cups 3.0 parser with the connection
 *        manager.  All of the *_parser_register routines should
 *        look identical except the ParseCmpString & StringSize.
 */
static void cups3_parser_register(void)
{
    DBG_PRINTF_NOTICE("URF: Setup cups Parser\n");
    PARSE_SETUP Parser;
    
    memset((void*)&Parser,0,sizeof(PARSE_SETUP));   // Need to null this structure due to monochrome printers having function
                                                    // pointers is this structure that will be called if not NULL. paw
        
    Parser.ParserID = UNIRAST_MODULE_ID;
    Parser.ParseCmpString = (char*)cups3BE_header_string_const;//< this is only the big endian version 
                                                              //NEED TO DO: will need to add little endian version
    Parser.StringSize = 4;
    Parser.ParseString = "URF";
    Parser.ParseType = e_Print;     // On monochrome printers this is the wrong enum. However it will work
                                    //for now since the correct enum of e_Printing is the same value as e_Print. paw

    ParserRegister(&Parser);        // register the parser for cups.

    Parser.ParseCmpString = (char*)cups3LE_header_string_const;

    ParserRegister(&Parser);        // register the parser for cups.
}

/**
 * @brief Checks for the cups 3.0 job header.  If it finds it it
 *        advances the stream it not, it puts the stream back
 *        the way it found it.
 * @param pipe: input stream
 * @param urf_page: urf page data
 * 
 * @return int: 0 if no error: -1 if error.
 */
int cups3_parser_detect(IOReg *pipe,  urf_page_t * urf_page)
{
    const int32_t cups_len = 4; // fileHeader "cups" numpages
    uint32_t buf[cups_len / sizeof(uint32_t)];   // union as int as char 
    char *buffer = (char*) &buf[0]; // force alignment
    int32_t bytes_read;
    cups2_isBigEndian(true);
    urf_page->curr_parser = NULL;
    bytes_read = pipe->ReadConnect(pipe, buffer, cups_len, PARSEREAD_EXACT);
    if (bytes_read == cups_len)
    {
        if (strncmp(buffer, cups3BE_header_string_const, cups_len))
        {
            if (strncmp(buffer, cups3LE_header_string_const, cups_len))
            {
                if (bytes_read > 0) 
                {
                    pipe->pipe_read_rewind(pipe, buffer, bytes_read);
                }
                return 0;
            }
            cups2_isBigEndian(false);
        }
        urf_page->curr_parser = &cups3_parser_struct;
        DBG_PRINTF_NOTICE("---- urf Selected cups 3 Job\n");
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

urf_parser_t * cups3_parser_init(void)
{
    return &cups3_parser_struct;
}
