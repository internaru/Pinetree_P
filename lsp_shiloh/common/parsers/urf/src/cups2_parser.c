/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file cups2_parser.c
 *
 * \brief Print parser for Cups raster 2.0 format
 *
 */
#include "string.h"

#include "lassert.h"
#include "logger.h"
#include "cp_strip.h"
#include "urf.h"
#include "urfio.h"

#define DBG_PRFX "cups2: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 23 )

static const char *cupsBE_header_string_const = "RaS2";
static const char *cupsLE_header_string_const = "2SaR";
const int32_t pwg_len = 9;
const char * pwg_token = "PwgRaster";
/**
 * cups 1.4 raster spec
 * http://www.cups.org/documentation.php/doc-1.4/spec-raster.html
 * The Printer Working Group
 * ipp subset of cups raster
 * wd-ippraster10-20110126.pdf
 */
typedef struct cups_ras_1_s
{
    char MediaClass[64];
    char MediaColor[64];
    char MediaType[64];
    char OutputType[64];
    uint32_t AdvanceDistance;
    uint32_t AdvanceMedia;
    uint32_t Collate;
    uint32_t CutMedia;
    uint32_t Duplex;
    uint32_t HWResolution[2];
    uint32_t ImagingBoundingBox[4];
    uint32_t InsertSheet;
    uint32_t Jog;
    uint32_t LeadingEdge;
    uint32_t Margins[2];
    uint32_t ManualFeed;
    uint32_t MediaPosition;
    uint32_t MediaWeight;
    uint32_t MirrorPrint;
    uint32_t NegativePrint;
    uint32_t NumCopies;
    uint32_t Orientation;
    uint32_t OutputFaceUp;
    uint32_t PageSize[2];
    uint32_t Separations;
    uint32_t TraySwitch;
    uint32_t Tumble;
    uint32_t cupsWidth;
    uint32_t cupsHeight;
    uint32_t cupsMediaType;
    uint32_t cupsBitsPerColor;
    uint32_t cupsBitsPerPixel;
    uint32_t cupsBytesPerLine;
    uint32_t cupsColorOrder;
    uint32_t cupsColorSpace;
    uint32_t cupsCompression;
    uint32_t cupsRowCount;
    uint32_t cupsRowFeed;
    uint32_t cupsRowStep;
} cups_ras_1_t;

#define CUPS_RASTER_2_SKIP (1796 - sizeof(cups_ras_1_t))
/*PWG raster Candidate Standard 5102.4-2012*/
typedef struct cups_pwg_1_s
{
    char PwgRaster[64];
    char MediaColor[64];
    char MediaType[64];
    char PrintContentOptimize[64];
    uint32_t Reserved1;
    uint32_t Reserved2;
    uint32_t Reserved3;
    uint32_t CutMedia;
    uint32_t Duplex;
    uint32_t HWResolution[2];
    uint32_t Reserved4[4];
    uint32_t InsertSheet;
    uint32_t Jog;
    uint32_t LeadingEdge;
    uint32_t Reserved5[2];
    uint32_t Reserved6;
    uint32_t MediaPosition;
    uint32_t MediaWeightMetric;
    uint32_t Reserved7;
    uint32_t Reserved8;
    uint32_t NumCopies;
    uint32_t Orientation;
    uint32_t Reserved9;
    uint32_t PageSize[2];
    uint32_t xSeparations;
    uint32_t xTraySwitch;
    uint32_t Tumble;
    uint32_t cupsWidth;
    uint32_t cupsHeight;
    uint32_t xcupsMediaType;
    uint32_t cupsBitsPerColor;
    uint32_t cupsBitsPerPixel;
    uint32_t cupsBytesPerLine;
    uint32_t cupsColorOrder;
    uint32_t cupsColorSpace;
    uint32_t xcupsCompression;
    uint32_t xcupsRowCount;
    uint32_t xcupsRowFeed;
    uint32_t xcupsRowStep;
    uint32_t TotalPageCount;
} cups_pwg_1_t;
#define PWG_RASTER_1_SKIP (1796 - sizeof(cups_pwg_1_t))
static void cups2_parser_register(void);
static void pwg1_parser_register(void);
int cups2_parser_detect(IOReg *pipe,  urf_page_t * urf_page);
int cups2_header_read(urf_bstream_t *bstream, urf_page_t *urf_page);
int pwg_header_read(urf_bstream_t *bstream, urf_page_t *urf_page);

static urf_parser_t cups2_parser_struct = 
    {
        cups2_parser_register,
        cups2_parser_detect,
        cups2_header_read,
        urf_parser_decompress,
        NULL
    };

static urf_parser_t pwg1_parser_struct = 
    {
        pwg1_parser_register,
        cups2_parser_detect,
        pwg_header_read,
        urf_parser_decompress,
        NULL
    };
static bool CupsBigEndian;
void cups2_isBigEndian(bool isBigEndian)
{
    CupsBigEndian = isBigEndian;
}
/**
 * @brief Registers the cups 2.0 parser with the connection
 *        manager.  All of the *_parser_register routines should
 *        look identical except the ParseCmpString & StringSize.
 */
static void cups2_parser_register(void)
{
    DBG_PRINTF_NOTICE("URF: Setup cups Parser\n");
    PARSE_SETUP Parser;
    
    memset((void*)&Parser,0,sizeof(PARSE_SETUP));   // Need to null this structure due to monochrome printers having function
                                                    // pointers is this structure that will be called if not NULL. paw
        
    Parser.ParserID = UNIRAST_MODULE_ID;
    Parser.ParseCmpString = (char*)cupsBE_header_string_const;//< this is only the big endian version 
                                                              //NEED TO DO: will need to add little endian version
    Parser.StringSize = 4;
    Parser.ParseString = "URF";
    Parser.ParseType = e_Print;     // On monochrome printers this is the wrong enum. However it will work
                                    //for now since the correct enum of e_Printing is the same value as e_Print. paw

    ParserRegister(&Parser);        // register the parser for cups.

    Parser.ParseCmpString = (char*)cupsLE_header_string_const;

    ParserRegister(&Parser);        // register the parser for cups.
}
//Currently not used
static void pwg1_parser_register(void)
{
    DBG_PRINTF_NOTICE("URF: Setup cups Parser\n");
    PARSE_SETUP Parser;
    
    memset((void*)&Parser,0,sizeof(PARSE_SETUP));   // Need to null this structure due to monochrome printers having function
                                                    // pointers is this structure that will be called if not NULL. paw
        
    Parser.ParserID = UNIRAST_MODULE_ID;
    Parser.ParseCmpString = (char*)cupsBE_header_string_const;//< this is only the big endian version 
                                                              //NEED TO DO: will need to add little endian version
    Parser.StringSize = 4;
    Parser.ParseString = "URF";
    Parser.ParseType = e_Print;     // On monochrome printers this is the wrong enum. However it will work
                                    //for now since the correct enum of e_Printing is the same value as e_Print. paw

    ParserRegister(&Parser);        // register the parser for cups.

    Parser.ParseCmpString = (char*)cupsLE_header_string_const;

    ParserRegister(&Parser);        // register the parser for cups.
}
/**
 * @brief Checks for the cups 2.0 job header.  If it finds it it
 *        advances the stream it not, it puts the stream back
 *        the way it found it.
 * @param pipe: input stream
 * @param urf_page: urf page data
 * 
 * @return int: 0 if no error: -1 if error.
 */
int cups2_parser_detect(IOReg *pipe,  urf_page_t * urf_page)
{
    const int32_t cups_len = 4; // fileHeader "cups" numpages
    uint32_t buf[3/*cups_len / sizeof(uint32_t)*/];   // union as int as char 
    char *buffer = (char*) &buf[0]; // force alignment
    int32_t bytes_read;
    cups2_isBigEndian(true);
    urf_page->curr_parser = NULL;
    bytes_read = pipe->ReadConnect(pipe, buffer, cups_len, PARSEREAD_EXACT);
    if (bytes_read == cups_len)
    {
        if (strncmp(buffer, cupsBE_header_string_const, cups_len))
        {
            if (strncmp(buffer, cupsLE_header_string_const, cups_len))
            {
                if (bytes_read > 0) 
                {
                    pipe->pipe_read_rewind(pipe, buffer, bytes_read);
                }
                return 0;
            }
            cups2_isBigEndian(false);
            
        }
        if (CupsBigEndian) 
        {
            bytes_read = pipe->ReadConnect(pipe, buffer, pwg_len, PARSEREAD_EXACT);
            DBG_PRINTF_NOTICE("---- token %8s\n",buffer);
            if (!strncmp(buffer, pwg_token, pwg_len))
            {
                DBG_PRINTF_NOTICE("---- urf Selected pwg 1 Job\n");
                urf_page->curr_parser = &pwg1_parser_struct;
                //urf_page->curr_parser = &cups2_parser_struct;
            }
            else
            {
                DBG_PRINTF_NOTICE("---- urf Selected be cups 2 Job\n");
                urf_page->curr_parser = &cups2_parser_struct;
            }
            pipe->pipe_read_rewind(pipe, buffer, bytes_read);
        }
        else
        {
            DBG_PRINTF_NOTICE("---- urf Selected le cups 2 Job\n");
            urf_page->curr_parser = &cups2_parser_struct;
        }

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
 * @brief Reads in the cups 2.0 header and parses out the
 *        required fields.
 * @param bstream: input bstream
 * @param urf_page: urf page data.
 * 
 * @return int status 0 good: -1 error.
 */
int logsomebytes = 0;
int cups2_header_read(urf_bstream_t *bstream, urf_page_t *urf_page)
{
        DBG_PRINTF_NOTICE("---- Reading Cups2 header\n");
        cups_ras_1_t * cups_hdr1_ptr;
        uint8_t *cptr;
        uint32_t tmp_uint32;
        int32_t bytes_read;

        if (sizeof(cups_ras_1_t) != (bytes_read = urf_bstream_read(bstream,(uint8_t **) &cups_hdr1_ptr, sizeof(cups_ras_1_t) ))) //stream to HW Res
        {
            DBG_PRINTF_NOTICE("---- urf read less than len %d bytes_read  %d\n", sizeof(cups_ras_1_t), bytes_read);
            return -1;
        }
        logsomebytes++;
        DBG_MEMLOG_ERR("urf:Header Read\n");
        memcpy(&urf_page->resolution, &cups_hdr1_ptr->HWResolution[0], 4);
        urf_page->resolution = CupsBigEndian ? be32_to_cpu (urf_page->resolution):le32_to_cpu (urf_page->resolution);
        DBG_PRINTF_NOTICE("---- Reading Cups2 header HWResolution %x\n", urf_page->resolution);
        memcpy(&urf_page->width, &cups_hdr1_ptr->cupsWidth, 4);
        urf_page->width = CupsBigEndian ? be32_to_cpu(urf_page->width):le32_to_cpu(urf_page->width);
        DBG_PRINTF_NOTICE("---- Reading Cups2 header cupsWidth %x\n", urf_page->width);
        memcpy(&urf_page->height, &cups_hdr1_ptr->cupsHeight, 4);
        urf_page->height = CupsBigEndian ? be32_to_cpu(urf_page->height):le32_to_cpu(urf_page->height);
        memcpy(&tmp_uint32, &cups_hdr1_ptr->cupsBitsPerPixel, 4);
        DBG_PRINTF_NOTICE("---- Reading Cups2 header cupsBitsPerPixel %x\n", tmp_uint32);
        urf_page->bitsPerPixel = (uint8_t) (CupsBigEndian ? be32_to_cpu(tmp_uint32):le32_to_cpu(tmp_uint32));
        memcpy(&tmp_uint32, &cups_hdr1_ptr->cupsColorSpace, 4);
        DBG_PRINTF_NOTICE("---- Reading Cups2 header cupsColorSpace %x\n", tmp_uint32);
        tmp_uint32 = CupsBigEndian ? be32_to_cpu(tmp_uint32):le32_to_cpu(tmp_uint32);
        if (tmp_uint32 == 18) //sGrey
        {
            urf_page->colorSpace = 0;
        }
        if (tmp_uint32 == 0) //sGrey
        {
            urf_page->colorSpace = 0;
        }
        else if (tmp_uint32 == 19) //sRGB 
        {
            urf_page->colorSpace = 1;
        }
        else if (tmp_uint32 == 1) //device color, assume sRGB 
        {
            urf_page->colorSpace = 1;
        }
        else
        {
            return -1; //unsupported color space color space
        }       

        memcpy(&tmp_uint32, &cups_hdr1_ptr->Duplex, 4);
        DBG_PRINTF_NOTICE("---- Reading Cups2 header Duplex %x\n", tmp_uint32);
        tmp_uint32 = CupsBigEndian ? be32_to_cpu(tmp_uint32):le32_to_cpu(tmp_uint32);
        urf_page->duplex_mode = (tmp_uint32 == 1) ? DMDUP_VERTICAL : DMDUP_SIMPLEX;

        memcpy(&tmp_uint32, &cups_hdr1_ptr->Tumble, 4);
        DBG_PRINTF_NOTICE("---- Reading Cups2 header Tumble %x\n", tmp_uint32);
        tmp_uint32 = CupsBigEndian ? be32_to_cpu(tmp_uint32):le32_to_cpu(tmp_uint32);
        urf_page->rotate_backside = (tmp_uint32 == 1) ? true : false;

        if (CUPS_RASTER_2_SKIP != (bytes_read = urf_bstream_read(bstream, &cptr, CUPS_RASTER_2_SKIP )))//eat rest of header
        {
            DBG_PRINTF_NOTICE("---- urf read less than len %d bytes_read  %d\n", CUPS_RASTER_2_SKIP, bytes_read);
            return -1;
        }
        urf_page->UnderColorRemoval = true;
        urf_page->copies = 0;
        return 0; //good CUPS header
}
/**
 * @brief Reads in the pwg 1.0 header and parses out the
 *        required fields.
 * @param bstream: input bstream
 * @param urf_page: urf page data.
 * 
 * @return int status 0 good: -1 error.
 */
int pwg_header_read(urf_bstream_t *bstream, urf_page_t *urf_page)
{
        DBG_PRINTF_NOTICE("---- Reading pwg 1 header\n");
        cups_pwg_1_t * cups_hdr1_ptr;
        uint8_t *cptr;
        uint32_t tmp_uint32;
        int32_t bytes_read;

        if (sizeof(cups_pwg_1_t) != (bytes_read = urf_bstream_read(bstream,(uint8_t **) &cups_hdr1_ptr, sizeof(cups_pwg_1_t) ))) //stream to HW Res
        {
            DBG_PRINTF_ERR("---- urf read less than len %d bytes_read  %d\n", sizeof(cups_pwg_1_t), bytes_read);
            goto pwg_header_read_error;
            return -1;
        }
        if (strncmp(cups_hdr1_ptr->PwgRaster, pwg_token, pwg_len))
        {
           DBG_PRINTF_ERR("---- token %s\n", cups_hdr1_ptr->PwgRaster);
           goto pwg_header_read_error; 
        }
        memcpy(&urf_page->resolution, &cups_hdr1_ptr->HWResolution[0], 4);
        urf_page->resolution = CupsBigEndian ? be32_to_cpu (urf_page->resolution):le32_to_cpu (urf_page->resolution);
        DBG_PRINTF_NOTICE("---- Reading PWG1 header HWResolution %x\n", urf_page->resolution);
        memcpy(&urf_page->width, &cups_hdr1_ptr->cupsWidth, 4);
        urf_page->width = CupsBigEndian ? be32_to_cpu(urf_page->width):le32_to_cpu(urf_page->width);
        DBG_PRINTF_NOTICE("---- Reading PWG1 header cupsWidth %x\n", urf_page->width);
        memcpy(&urf_page->height, &cups_hdr1_ptr->cupsHeight, 4);
        urf_page->height = CupsBigEndian ? be32_to_cpu(urf_page->height):le32_to_cpu(urf_page->height);
        memcpy(&tmp_uint32, &cups_hdr1_ptr->cupsBitsPerPixel, 4);
        DBG_PRINTF_NOTICE("---- Reading PWG1 header cupsBitsPerPixel %x\n", tmp_uint32);
        urf_page->bitsPerPixel = (uint8_t) (CupsBigEndian ? be32_to_cpu(tmp_uint32):le32_to_cpu(tmp_uint32));
        memcpy(&tmp_uint32, &cups_hdr1_ptr->TotalPageCount, 4);
        tmp_uint32 = CupsBigEndian ? be32_to_cpu(tmp_uint32):le32_to_cpu(tmp_uint32);
        DBG_PRINTF_NOTICE("---- Reading PWG1 header TotalPageCount %x\n", tmp_uint32);
        if (tmp_uint32 > 0) 
        {
            urf_page->job_pages = tmp_uint32;
        }
        memcpy(&tmp_uint32, &cups_hdr1_ptr->cupsColorSpace, 4);
        tmp_uint32 =  be32_to_cpu(tmp_uint32);
        DBG_PRINTF_NOTICE("---- Reading Cups2 header cupsColorSpace %x\n", tmp_uint32);
        
        if (tmp_uint32 == 18) //sGrey
        {
            urf_page->colorSpace = 0;
        }
        if (tmp_uint32 == 0) //sGrey
        {
            urf_page->colorSpace = 0;
        }
        else if (tmp_uint32 == 19) //sRGB 
        {
            urf_page->colorSpace = 1;
        }
        else if (tmp_uint32 == 1) //device color, assume sRGB 
        {
            urf_page->colorSpace = 1;
        }
        else
        {
            goto pwg_header_read_error;
            return -1; //unsupported color space color space
        }
        if (PWG_RASTER_1_SKIP != (bytes_read = urf_bstream_read(bstream, &cptr, PWG_RASTER_1_SKIP )))//eat rest of header
        {
            DBG_PRINTF_NOTICE("---- urf read less than len %d bytes_read  %d\n", PWG_RASTER_1_SKIP, bytes_read);
            return -1;
        }
        urf_page->print_quality = PRINTQUALITY_NORMAL;
        urf_page->UnderColorRemoval = false;
        urf_page->copies = 0;
        return 0; //good CUPS header
        pwg_header_read_error:
        if (bytes_read > 0) 
        {   
            bstream->have += bytes_read;
            bstream->cur -= bytes_read;
        }
        return -1;
}
urf_parser_t * cups2_parser_init(void)
{
    return &cups2_parser_struct;
}



