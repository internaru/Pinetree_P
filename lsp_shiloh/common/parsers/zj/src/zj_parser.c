/******************************************************************************
 * Copyright (c) 2006-2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file zj_parser.c
 *
 * \brief Scans the input zj stream and parses it.
 *
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "pthread.h"
#include "posix_ostools.h"
#include "dprintf.h"
#include "lassert.h"
#include "sys_init_api.h"
#include "agConnectMgr.h"
#include "ZjPlane.h"
#include "zj_rca.h"
#include "zj_parser_config.h"
#include "jbig.h"
#include "print_job_types.h"
#include "agMessage.h"
#include "memAPI.h"
#include "debug.h"
#include "logger.h"
#include "ioutils.h"
#include "print_job_api.h"
#include "agResourceMgr.h"
#include "SJMgr.h"

/*** local definitions ***/
#define DBGZJP_SOFT     (DBG_SOFT|DBG_OUTPUT)   // Activate this line for LOTS of debug output. **********
//#define DBGZJP_SOFT     0                       // Activate this line for HARDLY ANY debug output. **********
#define DBGZJP_ITEM     (DBG_LOUD|DBG_OUTPUT)   // Activate this line for LOTS of debug output re ZJ_ITEM. **********
//#define DBGZJP_ITEM     0                       // Activate this line for HARDLY ANY debug output re ZJ_ITEM. **********
#define DBGZJP_LOUD     (DBG_LOUD|DBG_OUTPUT)
#define DBGZJP_ERR      (DBG_ERROR|DBG_OUTPUT)  // Important error conditions that should always be flagged.

/* 2013.12.02		req. from version	 by Juny */
// first : ZJ released version, second : ZJ parser version, third : update version
/* history
 1.1.01		2013.12.12 distinguish GDI/PDL
 1.1.00		2013.12.02 set version
*/
char GDIPARSER_VERSION[]={"1.1.01"};
char* get_version_GDIParser()
{
   return (&GDIPARSER_VERSION[0]);
}

typedef struct plane_params_s
{
    uint32_t raster_x;
    uint32_t raster_y;
    uint32_t bpp;
    COLOR color;
    bool last_data;
    JBIG_BIH jbig_image_header;
}plane_params_t;

typedef struct bilevel_plane_node_s
{
    ATLISTENTRY node;
    plane_params_t plane_params;
    BigBuffer_t* buffer;
    uint32_t data_len;
}bilevel_plane_node_t;

typedef struct parse_message_wrapper_s
{
    ATLISTENTRY node;
    MESSAGE msg;
}parse_message_wrapper_t;

/**
 * \brief Enumeration of ZJ endian formats (big/little endian).
 */
typedef enum
{
    ZJ_BIG_ENDIAN = 0,
    ZJ_LITTLE_ENDIAN
} zj_endian_t;

static uint32_t  dpi_x, dpi_y;
static page_color_mode_t page_output_color_mode = e_Mono;

uint32_t  LASTPARSEERROR=0;

#ifdef PIXELCNT_BYHOST
static PIXELCNT pixels;
#endif // PIXELCNT_BYHOST

/*** default settings ***/

/*** external functions ***/

/*** external data ***/

/** local definitions **/

#define ZJI_QUALITY_DRAFT -3
#define ZJI_QUALITY_BEST -4

// Compare strings for both big and little endian data formats.
#define ZJ_CMP_STRING_BE_DATA "JZJZ"    ///< ZJ data sent in big endian format
#define ZJ_CMP_STRING_LE_DATA "ZJZJ"    ///< ZJ data sent in little endian format

/*** internal functions ***/
int32_t DoParsing(IOReg *, MODULE_ID );
void* zjparser_thread( void* unused );
static void sendQueuedBilevelPlanes(print_page_t* page, ATLISTENTRY *pList );
static error_type_t zj_set_job_endianness(const char* parse_compare_str);
static const char* DecodeZjBufType( ZJ_TYPE zjType );
static const char* ToString_ZJ_ITEM( uint16_t zjItem );
/*** public data ***/

/*** private data ***/

static mlimiter_t *mlimiter = 0;
static zj_parser_config_t *zjp_config = NULL;   // assigned during init

char ParseString[]="ZJS";
zj_endian_t zj_current_endian_format = ZJ_BIG_ENDIAN;  // default to big endian; will be set by job parse start

#define NUM_MESSAGES 10

static pthread_t    zjparser_thread_id;
static mqd_t        zjparser_msgq;

#define ZJSTACK_SIZE POSIX_MIN_STACK_SIZE // 6kB stack req'd for wireless and WSD
uint8_t  ZJStack[ZJSTACK_SIZE] ALIGN8;

ATLISTENTRY ParseQueue;         // a list of parse stuff to do next.

typedef struct _ZJ_COLORMAP_ENTRY
{
    PLANE zjPlaneColor;
    COLOR sysColor;
}ZJ_COLORMAP_ENTRY;

/* This is mapping table used to convert from the incoming zjStream color order
 * to our internal system color order.
 */ 
ZJ_COLORMAP_ENTRY zjColorMap[MAX_COLORS] = 
{
    {CYAN, COLOR_CYAN},
    {MAGENTA, COLOR_MAGENTA},
    {YELLOW, COLOR_YELLOW},
    {BLACK, COLOR_BLACK}
};

/*** public functions ***/


/*** public functions ***/

uint32_t agzjParserInit(void)
{
    PARSE_SETUP Parser;
    error_type_t etype;
    
    Parser.ParserID = ZJPARSER;

    etype = zj_parser_get_config((const zj_parser_config_t **)&zjp_config);
    REL_XASSERT(OK==etype, etype);
    REL_ASSERT(NULL != zjp_config);

    // register to support incoming data in big endian format
    Parser.ParseCmpString = ZJ_CMP_STRING_BE_DATA;
    Parser.StringSize = strlen(ZJ_CMP_STRING_BE_DATA);
    Parser.ParseString = ParseString;
    Parser.ParseType = e_Print;
    ParserRegister(&Parser);

    // register to support incoming data in little endian format
    Parser.ParseCmpString = ZJ_CMP_STRING_LE_DATA;
    Parser.StringSize = strlen(ZJ_CMP_STRING_LE_DATA);
    ParserRegister(&Parser);

    rm_register(e_ZJParser, ZJPARSER);

    ATInitList(&ParseQueue);
    posix_create_message_queue(&zjparser_msgq, "/ZJParseQ", NUM_MESSAGES, sizeof(MESSAGE));
    router_register_queue(ZJPARSER, zjparser_msgq);
    posix_create_thread(&zjparser_thread_id, zjparser_thread, 0, "zjparser", ZJStack, ZJSTACK_SIZE, POSIX_THR_PRI_NORMAL);
    
    return(0);
}

/*** private functions ***/

static inline uint32_t zj32_to_cpu(uint32_t value)
{
    return (zj_current_endian_format == ZJ_BIG_ENDIAN) ? be32_to_cpu(value) : le32_to_cpu(value);
}

static inline uint16_t zj16_to_cpu(uint16_t value)
{
    return (zj_current_endian_format == ZJ_BIG_ENDIAN) ? be16_to_cpu(value) : le16_to_cpu(value);
}

/*
 ** ConvertZjToSysColor
 *
 *  PARAMETERS: colorIn - incoming ZJ color
 *
 *  DESCRIPTION: Converts ZJ color to internal system color
 *
 *  RETURNS: COLOR - internal system color enum
 *
 */
COLOR ConvertZjToSysColor(PLANE colorIn)
{
    uint32_t i;
    for(i = 0; i < MAX_COLORS; i++)
    {
        if(zjColorMap[i].zjPlaneColor == colorIn)
        {
            return zjColorMap[i].sysColor;
        }
    }
    ASSERT(i < MAX_COLORS);
    // we should never get to here, but return something sane just in case.
    return COLOR_BLACK;
}

/*
 ** ScanPageOptions
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS:
 *
 */
uint32_t ScanPageOptions (print_page_t* page, plane_params_t plane_params[NUM_MARKING_COLOR_CHANNELS], unsigned char *Buffer, uint32_t Length, uint32_t NumParms)
{
    ZJ_ITEM_HEADER *Item;
    uint16_t tmp_item;
    uint32_t i;
    uint32_t raster_x = 0;
    uint32_t video_x = 0;

    if(Length == 0) return(0);         // if nothing to do return

    Item = (ZJ_ITEM_HEADER*)((uint32_t)Buffer);
    for(i = 0; i < NumParms; i++)
    {
        uint32_t tmpValueUINT32;

        switch(Item->Type)
        {
            case ZJIT_UINT32:
                {
                    tmpValueUINT32 = zj32_to_cpu(((ZJ_ITEM_UINT32*)Item)->value);
                    tmp_item =  zj16_to_cpu(Item->Item);

                    DPRINTF(DBGZJP_ITEM, ("ZJPARSE: ScanPageOptions Item='%s'=0x%X=%d  value=0x%X=%d\n",
                                           ToString_ZJ_ITEM(tmp_item), tmp_item, tmp_item, tmpValueUINT32, tmpValueUINT32 ));
                     
                    switch(tmp_item)
                    {
                        case ZJI_DMDUPLEX:    
                            print_page_set_duplex(page, (uint16_t)tmpValueUINT32);
                            break;

                        case ZJI_OUTBIN:    
                            print_page_set_output_dest(page, (trayenum_t)tmpValueUINT32);
                            DPRINTF(DBGZJP_SOFT, ("ZJPARSE: OUTBIN = %d\n",tmpValueUINT32));
                            break;

                        case ZJI_DMPAPER:  
                            print_page_set_media_size(page, (mediasize_t)tmpValueUINT32);
                            DPRINTF(DBGZJP_SOFT, ("ZJPARSE: PaperSize = %d\n", tmpValueUINT32));
                            break;

                        case ZJI_DMCOPIES: 
                            print_page_set_num_copies(page, tmpValueUINT32);
                            DPRINTF(DBGZJP_SOFT,("ZJI_DMCOPIES = %d\n", tmpValueUINT32));
                            break;

                        case ZJI_DMDEFAULTSOURCE: 
#if 0
//                            print_page_set_input_source(page, print_translate_input_tray(tmpValueUINT32));
#else	/* @@ tray value req. by LYJ 2013.06.26		by Juny */
                            switch (tmpValueUINT32)
                            	  {
                            	  case 7:	// Auto select
                            	   tmpValueUINT32 = TRAY_ANY;	//1;
                            	  	break;
                            	  case 4:	// Manual
                            	   tmpValueUINT32 = TRAY_1;	//1
                            	  	break;
                            	  case 1:	// Tray 1
                            	   tmpValueUINT32 = TRAY_2;
                            	  	break;
                            	  case 3:	// Tray 2
                            	   tmpValueUINT32 = TRAY_3;
                            	  	break;
                            	  case 2:	// Tray 3
                            	   tmpValueUINT32 = TRAY_4;
                            	  	break;
                            	  default:	// others
                            		DPRINTF(DBGZJP_SOFT,("ZJPARSE: input tray = %d(unknown)\n", tmpValueUINT32));
                            	   tmpValueUINT32 = 1;
                            	  	break;
                            	  }
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE:  GDI Tray is 0x%x\n", tmpValueUINT32));
                            print_page_set_input_source(page, tmpValueUINT32);
#endif
                            break;

                        case ZJI_DMMEDIATYPE:   
                            print_page_set_media_type(page, (mediatype_t)tmpValueUINT32);
                            break;

                        case ZJI_NBIE:
                            print_page_set_color_mode(page, (tmpValueUINT32 == 1) ? e_Mono : e_Color);
                            page_output_color_mode = (tmpValueUINT32 == 1) ? e_Mono : e_Color;
                            break;

                        case ZJI_RESOLUTION_X:
                            print_page_set_dpi_x(page, tmpValueUINT32);
                            dpi_x = tmpValueUINT32;
                            break;

                        case ZJI_RESOLUTION_Y:
                            print_page_set_dpi_y(page, tmpValueUINT32);
                            dpi_y = tmpValueUINT32;
                            break;

                        case ZJI_OFFSET_X:   
                            break;

                        case ZJI_OFFSET_Y:   
                            break;

                        case ZJI_RASTER_X:
                            print_page_set_raster_x(page, tmpValueUINT32);
                            plane_params[0].raster_x = tmpValueUINT32;
                            raster_x = tmpValueUINT32;
                            break;

                        case ZJI_RASTER_Y:   
                            print_page_set_image_height_lines(page, tmpValueUINT32);
                            print_page_add_zx_raster_y(page, tmpValueUINT32);
                            plane_params[0].raster_y = tmpValueUINT32;
                            break;

                        case ZJI_VIDEO_BPP:
                            print_page_set_output_bpp(page, tmpValueUINT32);
                            plane_params[0].bpp = tmpValueUINT32;
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE: Video BPP = %d %d \n", tmpValueUINT32, tmpValueUINT32 == 1 ? 600 : tmpValueUINT32 == 2 ? 1200 : 600));
                            break;

                        case ZJI_BITMAP_BPP:  
                            ASSERT(0);
//-jrs                            if(Page->VIDEO_BPP == 0)
//-jrs                                Page->VIDEO_BPP = tmpValueUINT32;
                            break;
                     
                        case ZJI_VIDEO_X:
                            print_page_set_video_x(page, tmpValueUINT32);
                            video_x = tmpValueUINT32;
                            break;

                        case ZJI_VIDEO_Y:
                            print_page_set_image_height_lines(page, tmpValueUINT32);
                            break;

                        case ZJI_BITMAP_PIXELS:
                            print_page_set_raster_x(page, tmpValueUINT32);
                            raster_x = tmpValueUINT32;
                            break;

                        case ZJI_BITMAP_LINES:
                            print_page_set_image_height_lines(page, tmpValueUINT32);
                            break;

                        case ZJI_PLANE:
                            //the code further down the line never actually used the color value
                            // in the page structure and I highly doubt that it is ever really sent so
                            // I eliminated it
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE: WARNING: plane color received as part of page data\n"));
//                            Page->PLANE_COLOR = ConvertZjToSysColor(((ZJ_ITEM_UINT32 *)Item)->value);
                            break;
#ifdef TODO  // how do we handle OEM specific tags in the Zeno parser?
                        case ZJI_RET:
                            Page->RET = tmpValueUINT32;
#endif
                            break;
                        case ZJI_TONER_SAVE:
//TODO                            Page->TONER_SAVE = tmpValueUINT32;
                            break;

                        case ZJI_BITMAP_STRIDE:
//-jrs                            print_page_set_raster_x_stride(page, tmpValueUINT32);
                            break;
                        case ZJI_BITMAP_TYPE:
//-jrs                            if(tmpValueUINT32 == ZJBT_RAW)
//-jrs                                print_page_set_data_format(page, true);
                            break;
                        case ZJI_END_PLANE:
                            DPRINTF(DBGZJP_LOUD, ("ZJPARSE: Ignore setting for ZJI_END_PLANE (deprecated)\n"));
                            //deleted 2008/08/08: Page->END_PLANE = ((ZJ_ITEM_UINT32 *)Item)->value;
                            break;
                        case ZJI_MEDIA_SIZE_X:
                            print_page_set_custom_media_size_x(page, tmpValueUINT32);
                            break;
                        case ZJI_MEDIA_SIZE_Y:
                            print_page_set_custom_media_size_y(page, tmpValueUINT32);
                            break;

                        case ZJI_MEDIA_SIZE_UNITS:
                            /*
                             * zjrca.h declares:
                             * - ZJI_MEDIA_SIZE_UNITS, // 1 == 1/1000 inch, 2 == 1/10 mm 
                             *
                             * Sample SDD.sdd uses:
                             * - [PaperSize]
                             * - 8=2970,4200,2 // A3; therefore, 2 = mm x 10.
                             * - 258=850,1300,4 // 8.5x13; therefore, 4 = in x 100.
                             *
                             * agHbParser.h declares:
                             * - #define TAGPAGE_CUSTOM_UNITS_INCHES 0   //inches
                             * - #define TAGPAGE_CUSTOM_UNITS_MM     1 //millimeters
                             *
                             */
                            switch ( tmpValueUINT32 )
                            {
                                case 0:  print_page_set_custom_media_units(page, PAPER_UNITS_INx100);  break;
                                case 1:  print_page_set_custom_media_units(page, PAPER_UNITS_INx1000); break;
                                case 2:  print_page_set_custom_media_units(page, PAPER_UNITS_MMx10);   break;
                                case 4:  print_page_set_custom_media_units(page, PAPER_UNITS_INx100);  break;
                                default: print_page_set_custom_media_units(page, PAPER_UNITS_MMx100);  break;
                            }
                            break;

                        case ZJI_ENCODING_DATA:
                            //- ENCODING DATA IS NOT PART OF PAGE OPTIONS 
                            ASSERT(Item->Item != ZJI_ENCODING_DATA);
                            break;

                        case ZJI_QUALITY:
                            {
                                PRINT_QUALITY pq;
                                if (ZJI_QUALITY_DRAFT == tmpValueUINT32)
                                {
                                    pq = PRINTQUALITY_DRAFT;
                                }
                                //else if (ZJI_QUALITY_NORMAL == tmpValueUINT32)   TODO: Get this definition
                                //{
                                //    Page->PrintQuality = PRINTQUALITY_NORMAL;
                                //}
                                else
                                {
                                    XASSERT(ZJI_QUALITY_BEST == tmpValueUINT32, tmpValueUINT32);
                                    pq = PRINTQUALITY_DRAFT;
                                }
                                DPRINTF(DBGZJP_ITEM, ("ZJPARSE: Print Quality = %d", pq));
                                print_page_set_print_quality(page, pq);
                            }
                            break;                            
                        case ZJI_BORDERLESS:
                            print_page_set_borderless(page, 1);
                            break;    
                        case ZJI_PRINTGRAY:
                            print_page_set_print_gray(page, 1);
                            print_page_set_color_mode(page, e_Mono);
                            break;
                        case ZJI_COLOR_FORMAT:
                            /* ignore */
                            break;
                        case ZJI_COLOR_SIZE:
                            print_page_set_num_input_colors(page, tmpValueUINT32);
                            break;
                        case ZJI_ZxBITS:
                            /* ignore */
                            break;
#ifdef PIXELCNT_BYHOST
                        case ZJI_SW_PIXELCOUNT_C:
                            pixels.pixelCntPerPage[CM_CYAN] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_M:
                            pixels.pixelCntPerPage[CM_MAGENTA] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_Y:
                            pixels.pixelCntPerPage[CM_YELLOW] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_K:
                            pixels.pixelCntPerPage[CM_BLACK] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_NW_C:
                         //   DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Got pixel nw c\n"));
                            pixels.nonWhitePixelCntPerPage[CM_CYAN] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_NW_M:
                         //   DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Got pixel nw M\n"));
                            pixels.nonWhitePixelCntPerPage[CM_MAGENTA] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_NW_Y:
                         //   DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Got pixel nw y\n"));
                            pixels.nonWhitePixelCntPerPage[CM_YELLOW] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_NW_K:
                         //   DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Got pixel nw k\n"));
                            pixels.nonWhitePixelCntPerPage[CM_BLACK] = tmpValueUINT32;
                            break;
#endif                        
                        default:
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE:  Error unknown  PAGE parameter 0x%x\n", Item->Item));
                            break;
                    }
                }
                break;
            default:
                /* Not handling any other item type now */
                DPRINTF(DBGZJP_ERR, ( "ZJPARSE: WARNING, unhandled PAGE item value type %d\n", Item->Type ));
                break;
        }
       uint32_t itemSize = zj32_to_cpu(Item->cbSize);
       Item = (ZJ_ITEM_HEADER*)((uint32_t)Item + itemSize);
    }
    
    // init the image info with the parsed data
    if (ZJ_DATA_FORMAT_ZX == zjp_config->data_format)
    {
        print_page_set_image_width_in_pixels(page, raster_x);
        print_page_add_zx_raster_x(page, raster_x);
    }
    else
    {
        print_page_set_image_width_in_pixels(page, video_x);
    }

    // In some cases (mono, color inline?) the plane specific options are not sent
    // so we store the page options into the planes as default values to be used
    // if plane options not sent.  We have already set the black plane options,
    // so here we replicate to the other channels.
    plane_params[0].color = COLOR_BLACK;
    for (i = 1; i < NUM_MARKING_COLOR_CHANNELS; i++)
    {
        plane_params[i] = plane_params[0];
        plane_params[i].color = i;
    }
    
    return(0);
}

uint32_t ScanJobOptions (print_job_t* job, unsigned char *Buffer, uint32_t Length, uint32_t NumParms)
{
    ZJ_ITEM_HEADER *Item;
    uint32_t i;
    uint16_t tmp_item;

    if(Length == 0) return(0);         // if nothing to do return

    Item = (ZJ_ITEM_HEADER*)((uint32_t)Buffer);
    for(i = 0; i < NumParms; i++)
    {
        uint32_t tmpValueUINT32;

        switch(Item->Type)
        {
            case ZJIT_UINT32:
                {
                    tmpValueUINT32 = zj32_to_cpu(((ZJ_ITEM_UINT32*)Item)->value);
                    tmp_item = zj16_to_cpu(Item->Item);

                    DPRINTF(DBGZJP_ITEM, ("ZJPARSE: ScanJobOptions Item='%s'=0x%X=%d  value=0x%X=%d\n",
                                          ToString_ZJ_ITEM(tmp_item), tmp_item, tmp_item, tmpValueUINT32, tmpValueUINT32 ));
 
                    switch(tmp_item)
                    {
#ifdef SDH_TONERMODE		/* 2013.10.28		by Juny */
					        case ZJI_TONERSAVE:
//                            DPRINTF(DBGZJP_ERR, ("ZJPARSE: diff margin = set always\n"));
//                            print_job_set_parser_info(job, 1);
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE: TONER SAVE = %d %x\n", tmpValueUINT32, ZJI_TONERSAVE ));
                            print_job_set_toner_save(job, (bool)tmpValueUINT32);
                            break;
					        case ZJI_TONERDARKNESS:
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE: diff margin = set always\n"));
                            print_job_set_parser_info(job, 1);
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE: TONER DARKNESS = %d %x\n", tmpValueUINT32, ZJI_TONERDARKNESS ));
                            print_job_set_darkness_level(job, tmpValueUINT32);
                            break;
#endif
                        case ZJI_PAGECOUNT:      
                            // parser currently always sends page count as 0; we ignore and count
                            // real pages in the job manager.
                            break;

                        case ZJI_DMCOLLATE:      
                            print_job_set_collation(job, (bool)tmpValueUINT32);      
                            DPRINTF(DBGZJP_LOUD, ("ZJPARSE: COLLATE = %d\n", tmpValueUINT32 ));
                            break;

                        case ZJI_DMDUPLEX:    
                            print_job_set_duplex(job, (uint16_t) tmpValueUINT32);
                            DPRINTF(DBGZJP_LOUD, ("ZJPARSE: DUPLEX = %d\n", tmpValueUINT32 ));
                            break;

                        case ZJI_DMPAPER:  
                            print_job_set_media_size(job, (mediasize_t)tmpValueUINT32);
                            DPRINTF(DBGZJP_SOFT, ("ZJPARSE: JobPaperSize = %d\n", (mediasize_t)tmpValueUINT32 ));
                            break;

                        case ZJI_DMCOPIES: 
                            DPRINTF(DBGZJP_SOFT,("ZJI_DMCOPIES = %d\n", tmpValueUINT32));
                            print_job_set_num_copies(job, tmpValueUINT32);
                            break;
                        case ZJI_DMDEFAULTSOURCE: 
                            print_job_set_default_source(job, print_translate_input_tray(tmpValueUINT32));
                            break;

                        case ZJI_DMMEDIATYPE:   
                            // remap UNKNOWN to ANY
                            print_job_set_media_type(job, (tmpValueUINT32 == MEDIATYPE_UNKNOWN) ? MEDIATYPE_ANY : tmpValueUINT32);
                            break;
                        default:
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE:  Error unknown JOB parameter 0x%x\n", Item->Item));
                            break;
                    }
                }
                break;

            default:
                /* Not handling any other item type now */
                DPRINTF(DBGZJP_ERR, ( "ZJPARSE: WARNING, unhandled JOB item value type %d\n", Item->Type ));
                break;
        }
        uint32_t itemSize = zj32_to_cpu(Item->cbSize);
        Item = (ZJ_ITEM_HEADER*)((uint32_t)Item + itemSize);
    }
    return(0);
}

COLOR ScanPlaneOptions (plane_params_t plane_params_out[NUM_MARKING_COLOR_CHANNELS], unsigned char *Buffer, uint32_t Length, uint32_t NumParms)
{
    ZJ_ITEM_HEADER *Item;
    uint32_t i;
    uint16_t tmp_item;

    JBIG_BIH jbigImageHeader;
    bool fGotBIH = false;
    bool fGotBPP = false;
    bool raster_x_valid = false; 
    bool raster_y_valid = false; 
    bool color_valid = false;
    uint32_t raster_x = 0;
    uint32_t raster_y = 0;
    uint32_t bpp = 0;
    COLOR color = COLOR_BLACK;
    bool last_data = false;

    ASSERT(Length > 0);

    Item = (ZJ_ITEM_HEADER*)((uint32_t)Buffer);
    for(i = 0; i < NumParms; i++)
    {
        uint32_t tmpValueUINT32;
        switch(Item->Type)
        {
            case ZJIT_UINT32:
                {
                    tmpValueUINT32 = zj32_to_cpu(((ZJ_ITEM_UINT32*)Item)->value);
                    tmp_item = zj16_to_cpu(Item->Item);
                    DPRINTF(DBGZJP_ITEM, ("ZJPARSE: ScanPlaneOptions Item='%s'=0x%X=%d  value=0x%X=%d\n",
                                          ToString_ZJ_ITEM(tmp_item), tmp_item, tmp_item, tmpValueUINT32, tmpValueUINT32 ));

                    switch(tmp_item)
                    {
                        case ZJI_RESOLUTION_X:
                            ASSERT(0); // I don't think resolution is used on the plane level
                            break;

                        case ZJI_RESOLUTION_Y:
                            ASSERT(0); // I don't think resolution is used on the plane level
                            break;

                        case ZJI_RASTER_X:   
                            raster_x = tmpValueUINT32;
                            raster_x_valid = true;
                            break;

                        case ZJI_RASTER_Y:   
                            raster_y = tmpValueUINT32;
                            raster_y_valid = true;
                            break;

                        case ZJI_VIDEO_BPP:
                            bpp = tmpValueUINT32;
                            fGotBPP = TRUE;
                            break;

                        case ZJI_BITMAP_BPP:  
                            if(bpp == 0)
                            {
                                bpp = tmpValueUINT32;
                                fGotBPP = TRUE;
                            }
                            break; 
                         
                        case ZJI_BITMAP_PIXELS:
                            raster_x = tmpValueUINT32;
                            raster_x_valid = true;
                            break;

                        case ZJI_BITMAP_LINES:
                            raster_y = tmpValueUINT32;
                            raster_y_valid = true;
                            break;

                        case ZJI_PLANE:
                            color = ConvertZjToSysColor( tmpValueUINT32 );
                            color_valid = true;
                            // agprintf("plane color: %d\n", Plane->PLANE_COLOR);
                            break;
                         
                        case ZJI_BITMAP_STRIDE:
                            ASSERT(0);
                            break;

                        case ZJI_BITMAP_TYPE:
                            if(tmpValueUINT32 == ZJBT_RAW)
                            {
                                ASSERT(0);
                            }
                            break;

                        case ZJI_END_PLANE:
                            last_data = tmpValueUINT32;
                            break;
                         
                        case ZJI_ENCODING_DATA:
                            XASSERT(Item->cbSize == sizeof(JBIG_BIH), Item->cbSize);
                            jbigImageHeader = *(JBIG_BIH*)(((ZJ_ITEM_BYTELUT*)Item)->lut);
                            fGotBIH = TRUE;
                            break;

#ifdef PIXELCNT_BYHOST
                        case ZJI_SW_PIXELCOUNT_C:
                            pixels.pixelCntPerPage[CM_CYAN] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_M:
                            pixels.pixelCntPerPage[CM_MAGENTA] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_Y:
                            pixels.pixelCntPerPage[CM_YELLOW] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_K:
                            pixels.pixelCntPerPage[CM_BLACK] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_NW_C:
                         //   DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Got pixel nw c\n"));
                            pixels.nonWhitePixelCntPerPage[CM_CYAN] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_NW_M:
                         //   DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Got pixel nw M\n"));
                            pixels.nonWhitePixelCntPerPage[CM_MAGENTA] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_NW_Y:
                         //   DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Got pixel nw y\n"));
                            pixels.nonWhitePixelCntPerPage[CM_YELLOW] = tmpValueUINT32;
                            break;
                        case ZJI_SW_PIXELCOUNT_NW_K:
                         //   DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Got pixel nw k\n"));
                            pixels.nonWhitePixelCntPerPage[CM_BLACK] = tmpValueUINT32;
                            break;
#endif                        
                        default:
                            DPRINTF(DBGZJP_ERR, ("ZJPARSE:  Error PLANE unknown parameter %d\n", Item->Item));
                            break;
                    }
                }
                break;

            case ZJIT_BYTELUT:
                tmpValueUINT32 = zj32_to_cpu(((ZJ_ITEM_BYTELUT*)Item)->cbSize);
                DPRINTF(DBGZJP_ITEM, ("ZJPARSE: ScanPlaneOptions Item='%s'=0x%X=%d  cbSize=0x%X=%d\n",
                                      ToString_ZJ_ITEM(Item->Item), Item->Item, Item->Item,
                                      tmpValueUINT32, tmpValueUINT32 ));
                switch(Item->Item)
                {
                    case ZJI_ENCODING_DATA:
                    XASSERT((((ZJ_ITEM_BYTELUT*)Item)->cbSize) == sizeof(JBIG_BIH), (tmpValueUINT32));
                    jbigImageHeader = *(JBIG_BIH*)(((ZJ_ITEM_BYTELUT*)Item)->lut);
                    fGotBIH = TRUE;
                        break;
                }
                break;
           
            default:
                /* Not handling any other item type now */
                DPRINTF(DBGZJP_ERR, ( "ZJPARSE: WARNING, unhandled PLANE item value type %d\n", Item->Type ));
                break;
        }
        uint32_t itemSize = zj32_to_cpu(Item->cbSize);
        Item = (ZJ_ITEM_HEADER*)((uint32_t)Item + itemSize);
    }

    // we have now parsed all of the options so we should have the plane color
    // so now we can store the BIH and BPP to the proper location
    if(fGotBIH)
    {
        DPRINTF(DBGZJP_SOFT, ("ZJPARSE: Storing BIH for plane %d\n", color));
        plane_params_out[color].jbig_image_header = jbigImageHeader;
    }
    if(fGotBPP)
    {
        plane_params_out[color].bpp = bpp;
    }

    if (raster_x_valid)
    {
        plane_params_out[color].raster_x = raster_x;
    }
    if (raster_y_valid)
    {
        plane_params_out[color].raster_y = raster_y;
    }
    if (color_valid)
    {
        plane_params_out[color].color = color;
    }
    plane_params_out[color].last_data = last_data;

    return color;
}

/**
 * \brief Test parser compare string and set job endianness as indicated. 
 *  
 * \param parse_compare_str String from incoming job data to compare. 
 *  
 * \return OK if compare string was matched; else FAIL.
 */  
static error_type_t zj_set_job_endianness(const char* parse_compare_str)
{
    error_type_t status = OK;
    if (0 == strncmp(parse_compare_str, ZJ_CMP_STRING_BE_DATA, strlen(ZJ_CMP_STRING_BE_DATA)))
    {
        zj_current_endian_format = ZJ_BIG_ENDIAN;
    }
    else if (0 == strncmp(parse_compare_str, ZJ_CMP_STRING_LE_DATA, strlen(ZJ_CMP_STRING_BE_DATA)))
    {
        zj_current_endian_format = ZJ_LITTLE_ENDIAN;
    }
    else
    {
        status = FAIL;
        ASSERT(0);  // unsupported compare string
    }
    DPRINTF(DBGZJP_SOFT, ("ZJPARSE: %s: %s ENDIAN\n", __func__, ((zj_current_endian_format == ZJ_BIG_ENDIAN) ? "BIG" : "LITTLE")));
    return status;
}

// FUNCTION NAME: zjparser_thread
/**
 * \brief Parse a Zeno ZjStream print job.
 *
 *  This function is called by the system parser when a Parse string is located.  
 *  The data in the buffer is parsed and then more data is read until it is finished,
 *  when this function will return to waiting for another job to parse.
 *
 *         2008/10/24 - Restructured this "outer loop" to handle a
 *         Cancel Job message. This closed a window whereby such a message was
 *         lost if it arrived between certain other messages. Also restructured
 *         handling of the OurResource and Pipe pointers to plug other possible
 *         windows. The whole system should be redesigned away from the current
 *         message-based scheme!
 */
void* zjparser_thread( void* unused)
{
    MESSAGE     nextMsg;
    CURRENT_RESOURCE*   OurResource = NULL;
    IOReg*              Pipe = NULL;
    bool Parsing = FALSE;
    parse_message_wrapper_t* parse_msg;
    ATLISTENTRY* node;

    SysWaitForInit ();

    if (mlimiter == 0)
    {
        mlimiter = mlimiter_by_name("print");
    }

    DPRINTF(DBGZJP_SOFT, ("ZJPARSE: initialize\n"));

    while (1)
    {
        if ((node = ATRemoveHeadList(&ParseQueue)) != NULL)
        {
            parse_msg = CONTAINING_RECORD(node, parse_message_wrapper_t, node);
            nextMsg = parse_msg->msg;
            MEM_FREE_AND_NULL(parse_msg);
        }
        else    
        {
            mq_receive(zjparser_msgq, (char*)&nextMsg, sizeof(MESSAGE), 0);
        }
        DPRINTF(DBGZJP_SOFT, ("ZJPARSE: agzjParser: got message '%s'=0x%X=%d; param3=0x%08X\n",
                              DecodeSysMsgEnum(nextMsg.msgType), nextMsg.msgType,
                              nextMsg.msgType, nextMsg.param3 ));
        switch (nextMsg.msgType)
        {
            case MSG_START_PARSE:
                {
                    //
                    // If we are curently parsing, save the message till we are done.
                    //
                    if (Parsing)
                    {
                        parse_msg = (parse_message_wrapper_t*)MEM_MALLOC(sizeof(parse_message_wrapper_t));
                        ASSERT(parse_msg != NULL);
                        if (parse_msg)
                        {
                            parse_msg->msg = nextMsg;
                            ATInsertTailList(&ParseQueue, &parse_msg->node);
                        }
                    }
                    else
                    {
                        STARTRECIPE *StartIt;

                        StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
                        ASSERT(StartIt != NULL);

                        DPRINTF(DBGZJP_ERR, (" @@ ZJ START PARSE MSG\n"));	// 2013.12.06		by Juny
                        // we have a connection, get the resources and start.
                        //
                        Pipe = (IOReg *) nextMsg.param3;        // save the pipe info.
                        nextMsg.msgType = MSG_STARTRECIPE;
                        if (ZJ_DATA_FORMAT_JBIG == zjp_config->data_format)
                        {
                            StartIt->Job = ejob_zjs_host_print;
                        }
                        else
                        {
                            XASSERT(ZJ_DATA_FORMAT_ZX == zjp_config->data_format, zjp_config->data_format);
                            StartIt->Job = ejob_PrintIO;
                        }
 
                        StartIt->AdditionalResource = e_ZJParser;
                        StartIt->Pipe = Pipe;
                        StartIt->Wait = e_WaitForever;
                        nextMsg.param3 = StartIt;       // freed by sjm

                        SYMsgSend(SJMID, &nextMsg);         // send the message.
                        DPRINTF(DBGZJP_ERR, (" @@ ZJ START PARSE MSG to SJM\n"));	// 2013.12.06		by Juny
                        Parsing = TRUE;
                    }
                    break;
                }

            case MSG_RESOURCES:
                if ( Parsing )
                {
                    OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ RESOURCE MSG\n"));	// 2013.12.06		by Juny
                    DPRINTF(DBGZJP_LOUD, ("ZJPARSE: Resource %#x\n",OurResource));
                }
                else
                {
                    nextMsg.msgType = MSG_FREERECIPE;
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ RESOURCE MSG to SJM\n"));	// 2013.12.06		by Juny
                    SYMsgSend( SJMID, &nextMsg );
                }
                break;

            case MSG_ACKRECIPE:
                {
                    int32_t ParseRet;

                    // Update status to indicate that a print job is being received.
                    // This is necessary to cause the "processing" status message
                    // to be displayed.
//                    statusmgr_post_status_event(ZJPARSER, STATUS_INFO_PRINTING);

                    // We have the resources, start the job.
                    mlimiter_start( mlimiter_by_name("print"));
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ ACKRECIPE MSG Doparsing\n"));	// 2013.12.06		by Juny

                    ParseRet = DoParsing(Pipe, OurResource->Destination);

                    mlimiter_stop( mlimiter_by_name("print"));

                    DPRINTF(DBGZJP_ERR, (" @@ ZJ end Doparsing\n"));	// 2013.12.06		by Juny
                    DPRINTF(DBGZJP_LOUD, ("ZJPARSE: Parse Done: free recipe: %#x, pipe: %#x\n", OurResource, Pipe));

                    // finished parsing, free the resources.
                    Parsing = FALSE;
                    nextMsg.msgType = MSG_FREERECIPE;
                    nextMsg.param3 = OurResource;
                    SYMsgSend(SJMID, &nextMsg);
                    OurResource = NULL;

                    // Tell the connection manager the results of the parse.
                    nextMsg.param1 = ParseRet;
                    nextMsg.msgType = MSG_END_PARSE;
                    nextMsg.param3 = Pipe;          // The pipe that we are done parsing.
                    SYMsgSend(CMPARSER, &nextMsg);      // send the response.
                    Pipe = NULL;
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ END PARSE to CM\n"));	// 2013.12.06		by Juny
                    DPRINTF((DBG_OUTPUT|DBG_LOUD),("agzparser: Done Parsing\n"));
                    break;      // done
                }
        case MSG_CANCELJOB:
            DPRINTF(DBGZJP_ERR, (" @@ ZJ CANCEL param1=%d=%s param2=%d=%s param3=0x%08X\n",
                                  nextMsg.param1, ToString_DOACTION((DOACTION)nextMsg.param1),
                                  nextMsg.param2, ToString_MODULE_ID((MODULE_ID)nextMsg.param2),
                                  nextMsg.param3 ));	// 2013.12.06		by Juny
            DPRINTF(DBGZJP_LOUD, ("ZJPARSE: agzjParser: got MSG_CANCELJOB param1=%d=%s param2=%d=%s param3=0x%08X\n",
                                  nextMsg.param1, ToString_DOACTION((DOACTION)nextMsg.param1),
                                  nextMsg.param2, ToString_MODULE_ID((MODULE_ID)nextMsg.param2),
                                  nextMsg.param3 ));
            if ( nextMsg.param1 == SYS_START )
            {
                MESSAGE outMsg;

                Parsing = FALSE;

                if ( OurResource )
                {
                    outMsg.msgType = MSG_FREERECIPE;
                    outMsg.param1 = 0;
                    outMsg.param2 = 0;
                    outMsg.param3 = OurResource;
                    SYMsgSend( SJMID, &outMsg );
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ FREERECIPE MSG to SJM\n"));	// 2013.12.06		by Juny
                    OurResource = NULL;
                }

                if ( Pipe )
                {
                    outMsg.msgType = MSG_END_PARSE;
                    outMsg.param1 = (uint32_t)PARSER_ERROR;
                    outMsg.param2 = 0;
                    outMsg.param3 = Pipe;
                    SYMsgSend( CMPARSER, &outMsg );
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ PARSE ERROR MSG to CM\n"));	// 2013.12.06		by Juny
                    Pipe = NULL;
                }
                mlimiter_stop( mlimiter_by_name("print"));

                nextMsg.param1 = SYS_ACK;
                nextMsg.param2 = ZJPARSER;
                SYMsgSend( SJMID, &nextMsg ); // param3 preserved.
            }
            // else // Ignore SYS_ACK coming back to us.
            break;

        default:
            DPRINTF(DBGZJP_ERR, (" @@ ZJ ignore MSG\n"));	// 2013.12.06		by Juny
//            DPRINTF(DBGZJP_SOFT, ("ZJPARSE: agzjParser: message ignored!\n" ));
            break;
        } // end switch ( nextMsg.msgType )
    } // end while (1)

    return 0;
} // end agzjParser()

static inline PLANE_TYPE zj_map_color_to_plane_type(COLOR color)
{
    static PLANE_TYPE plane_type_map[] = {PLANAR_K, PLANAR_C, PLANAR_Y, PLANAR_M};
    // map declaration above relies on the order enforced by this assert
    ASSERT(COLOR_BLACK == 0 && COLOR_CYAN == 1 && COLOR_YELLOW == 2 && COLOR_MAGENTA == 3);

    return plane_type_map[color];
}

static unsigned char gZJBuffer[512] ALIGN16;    
int32_t DoParsing(IOReg *Pipe, MODULE_ID Destination)
{
    int32_t bytesRead;
    uint32_t Length, DataLength, NumParam; //, BlkType;
    ZJ_TYPE BlkType; // use the ENUM!!!
    uint8_t done=0, JobStart=0;
    unsigned char *TagBuffer=NULL;
    void *DataBuffer=NULL;
    print_job_t* zj_current_job = NULL;
    print_page_t* page = NULL;
    
    BigBuffer_t *big_buffer;
    ATLISTENTRY bilevelPlaneList;
    bool ZxStartReceived = FALSE;
    bool startManualDuplex=FALSE;
    COLOR curPlaneColor = COLOR_BLACK; // default to black to make mono handling easier
    plane_params_t plane_params[NUM_MARKING_COLOR_CHANNELS];    // TODO: -jrs should be max marking color channels?

    // Init bilevel plane list
    ATInitList(&bilevelPlaneList);

    memset(plane_params, 0, sizeof(plane_params));

    page_output_color_mode = e_Mono;

    //
    // When we get here the header that was checked to get here was put back in the channel.
    // Read out the header and discard it.  We know it is there.
    //
    DPRINTF(DBGZJP_SOFT, ("ZJPARSE: start parse\n"));

    Length = strlen(ZJ_CMP_STRING_BE_DATA);     // read in the header and discard

    bytesRead = Pipe->ReadConnect(Pipe, gZJBuffer, Length, PARSEREAD_EXACT);

    // if we had an error return, we return success until we start parsing a job
    //
    if((uint32_t)bytesRead != Length)
    {
        if(bytesRead > 0)
            Pipe->RewindData (Pipe, gZJBuffer, bytesRead);            // put the data back into the pipe.
        return PARSER_SUCCESS;
    }

    gZJBuffer[Length] = 0;  // null terminate
    if(OK != zj_set_job_endianness((char *)gZJBuffer))
    {
        Pipe->RewindData (Pipe, gZJBuffer, bytesRead);            // put the data back into the pipe.
        goto Fail;
    }

    //
    // Read and verify the first header;
    //
    Length = sizeof(ZJ_HEADER);

    bytesRead = Pipe->ReadConnect (Pipe, gZJBuffer, Length, PARSEREAD_EXACT);
    if( bytesRead < (int32_t)Length)
    {
        // we have an error, rewind the data.
        if(bytesRead > 0)
            Pipe->RewindData (Pipe, gZJBuffer, bytesRead);            // put the data back into the pipe.
        goto Fail;
    }
    if(((ZJ_HEADER *)gZJBuffer)->Signature !=  ZJRCA_ITEM_SIGNATURE)
    {
        DPRINTF(DBGZJP_ERR, ("ZJPARSE: Parser signature doesn't match.\n"));
        Pipe->RewindData (Pipe, gZJBuffer, Length);
        goto Fail;
    }
    DPRINTF(DBGZJP_SOFT, ("ZJPARSE: start job\n"));
    //
    // Now parse the data stream.
    //
    while(!done)
    {
        uint32_t TagLength;       // how much data is contained in the tags.
        MESSAGE nextMsg;

        //
        // See if there is a message waiting for me.  Handle it if so.
        // Note 0 timeout.  Will return immediately
        //
        int msg_status = posix_wait_for_sys_message( zjparser_msgq, &nextMsg, 0 );
        if ( msg_status == 0 )
        {
            DPRINTF(DBGZJP_SOFT, ("ZJPARSE: DoParsing: got message '%s'=0x%X=%d; param3=0x%08X\n",
                                  DecodeSysMsgEnum(nextMsg.msgType), nextMsg.msgType,
                                  nextMsg.msgType, nextMsg.param3 ));
            switch(nextMsg.msgType)
            {
                case MSG_CANCELPAGE:
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ CANCELPAGE\n"));	// 2013.12.06		by Juny
                    ASSERT( "No Parser should ever get MSG_CANCELPAGE!" == 0 );
                    // fall through...

                case MSG_CANCELJOB: 
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ CANCELJOB param1=%d=%s param2=%d=%s param3=0x%08X\n",
                                          nextMsg.param1, ToString_DOACTION((DOACTION)nextMsg.param1),
                                          nextMsg.param2, ToString_MODULE_ID((MODULE_ID)nextMsg.param2),
                                          nextMsg.param3 ));// 2013.12.06		by Juny
                    DPRINTF(DBGZJP_LOUD, ("ZJPARSE: DoParsing: got MSG_CANCELJOB param1=%d=%s param2=%d=%s param3=0x%08X\n",
                                          nextMsg.param1, ToString_DOACTION((DOACTION)nextMsg.param1),
                                          nextMsg.param2, ToString_MODULE_ID((MODULE_ID)nextMsg.param2),
                                          nextMsg.param3 ));
                    zj_current_job = print_job_cancel(zj_current_job);
                    nextMsg.param1 = SYS_ACK;
                    nextMsg.param2 = ZJPARSER;
                    SYMsgSend(SJMID, &nextMsg); // acknowledge the cancel.
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ call cancelAPI CANCEL ACK to SJM\n"));	// 2013.12.06		by Juny

                    JobStart = 0; // Prevents "Fail:" code from sending its own MSG_JOBABORT
                    goto Fail;

            default:
                DPRINTF(DBGZJP_ERR, (" @@ ZJ ASSER\n"));	// 2013.12.06		by Juny
                DPRINTF(DBGZJP_SOFT, ("ZJPARSE: DoParsing: ASSERT!\n" ));
                ASSERT(0);  // we should not be getting any other messages.
            }
        }

        Length = sizeof(ZJ_HEADER);

        // figure the length of all the data we need for this block.
        //
        DataLength = zj32_to_cpu(((ZJ_HEADER *) gZJBuffer)->cbSize) - sizeof(ZJ_HEADER);
        TagLength = zj16_to_cpu(((ZJ_HEADER *) gZJBuffer)->wParam);        // adjust the length
        DataLength -= TagLength;
        big_buffer = 0;
        TagBuffer = NULL;
        DataBuffer = NULL;
        if(TagLength)
        {
            // dprintf("------------ taglength %d\n", TagLength);
            TagBuffer = (unsigned char *)MEM_MALLOC_ALIGN_LIMITED(mlimiter, TagLength, cpu_get_dcache_line_size());
        }

        BlkType = zj32_to_cpu(((ZJ_HEADER *) gZJBuffer)->Type);          // get the type from header
        if(DataLength)
        {
            if (BlkType == ZJT_JBIG_BID ||
                BlkType == ZJT_BITMAP || 
                BlkType == ZJT_OPAQUE_DATA ||
                BlkType == ZJT_ZX_DATA)
            {
                // Get compressed JBIG memory.  Factory here would allow configuration elsewhere, tbd.
                big_buffer = BigBuffer_Malloc(mlimiter, DataLength);
                // dprintf("------------ big_buffer alloc %x %d\n", big_buffer->data, big_buffer->datalen);
            }
            else
            {
                // dprintf("------------ datalength %d\n", DataLength);
                DataBuffer = (unsigned char *) MEM_MALLOC_LIMITED(mlimiter, DataLength);
            }
        }

        // If the TagLength is non-zero, we have options we need to parse.  Read them in here.
        //    
        if(TagLength > 0)
        {
            bytesRead = Pipe->ReadConnect (Pipe, TagBuffer, TagLength, PARSEREAD_EXACT);
            if(bytesRead != (int32_t)TagLength)
            {
                // Failed, put the data back and get out of here.
                if(bytesRead > 0)
                {
                    Pipe->RewindData (Pipe, TagBuffer, bytesRead);
                }
                goto Fail;
            }
        }
        // 
        // If DataLength > 0 we have data with this header.  Read the data in here.
        //
        if(DataLength > 0)
        {
            void *read_dest = big_buffer ? big_buffer->data : DataBuffer;

            bytesRead = Pipe->ReadConnect(Pipe, read_dest, DataLength, PARSEREAD_EXACT);
            if(bytesRead != (int32_t)DataLength)
            {
                //
                // We had an error, put the data back into the pipe and return.
                //
                Pipe->RewindData(Pipe, TagBuffer, TagLength);
                if(bytesRead > 0)
                {
                    Pipe->RewindData(Pipe, read_dest, bytesRead);
                }
                DPRINTF(DBGZJP_ERR, ("ZJPARSE: Unable to read data buffer, DataLength=%d, bytesRead=%d\n", DataLength, bytesRead));
                
                goto Fail;
            }
        }
        NumParam = zj32_to_cpu(((ZJ_HEADER *)gZJBuffer)->dwParam);      // get some data from header

        //
        // Now read the next header and insure that it is correct.
        //
        if(BlkType != ZJT_END_DOC)   // only do this if another header exists.
        {
            // 
            bytesRead = Pipe->ReadConnect (Pipe, gZJBuffer, Length, PARSEREAD_EXACT);
            if( bytesRead < (int32_t)Length)
            {
                // we have an error, rewind the data.
                if(bytesRead > 0)
                {
                    Pipe->RewindData (Pipe, gZJBuffer, bytesRead);
                }
                Pipe->RewindData (Pipe, DataBuffer, DataLength);
                Pipe->RewindData (Pipe, TagBuffer, TagLength);
                DPRINTF(DBGZJP_ERR, ("ZJPARSE: Unable to read next header %#x\n",Length));
                goto Fail;
            }

            //
            // Check that header is correct.
            //
            if(((ZJ_HEADER *)gZJBuffer)->Signature !=  ZJRCA_ITEM_SIGNATURE)
            {
                DPRINTF(DBGZJP_ERR, ("ZJPARSE: Parser signature doesn't match. D %#x T %#x\n",DataBuffer, TagBuffer));
                DPRINTF(DBGZJP_ERR, ("\tsig should be: %#x, is %#x\n", ZJRCA_ITEM_SIGNATURE, ((ZJ_HEADER *)gZJBuffer)->Signature));
                // assert was removed because this is an expected case.  
                // When a cancel is done on the PC the driver/LM send a large buffer of 0's to
                // forced this case, so don't rewind, just bail out and let abort
                // handling clean up
                goto Fail;
            }
        }

        //
        // Header is correct, continue parsing.
        //
        DPRINTF(DBGZJP_SOFT, ("ZJPARSE: DoParsing BlkType='%s'=0x%X=%d\n",
                              DecodeZjBufType(BlkType), BlkType, BlkType ));
        switch(BlkType)
        {
            case ZJT_START_DOC:
                {
                    uint32_t i;
                    UNUSED_VAR(i);

                    /*  Create new job object */
                    ASSERT(zj_current_job == NULL);
                    if (zj_current_job != NULL)
                    {
                        // if we are already in a job something "really bad" has happened on
                        // the sending side.  Attempt to end current job and start next.
                        // this really should never happen, but...
                        print_job_end(zj_current_job);
                        zj_current_job = NULL;
                    }
                    zj_current_job = print_job_construct(mlimiter, Pipe);

#ifdef PIXELCNT_BYHOST
                    memset(&pixels, 0, sizeof(PIXELCNT));
#endif // ifdef PIXELCNT_BYHOST

                    if(ScanJobOptions(zj_current_job, TagBuffer, TagLength, NumParam))
                    {
                        DPRINTF(DBGZJP_ERR, (" @@ ZJ ASSER\n"));	// 2013.12.06		by Juny
                        zj_current_job = print_job_cancel(zj_current_job);
                        ASSERT(zj_current_job == NULL);
                        goto Fail;
                    }
				       DPRINTF(DBGZJP_ERR, (" @@ Job Start\n"));	// 2013.11.07		by Juny
				    print_job_set_job_info(zj_current_job, PRINTJOB);
                    print_job_start(zj_current_job, Destination);

                    JobStart = 1;                                       // we have send something to the job manager.
                    break; 
                }

            case ZJT_START_PAGE:
                {
#ifdef HAVE_PRINT_SUPPORT
                    // default current plane to black to handle mono pages that
                    // don't specify plane color.  Will be overridden if we get
                    // a ZJT_START_PLANE
                    curPlaneColor = COLOR_BLACK;

                    // Initialize a page object
                    //
                    ASSERT(page == NULL);
                    page = print_page_construct(zj_current_job);
                    ASSERT(page != NULL);


                    if(startManualDuplex==TRUE)
                    {
                        print_page_set_manual_duplex_second_side(page);
                    }

                    if(ScanPageOptions(page, plane_params, TagBuffer, TagLength, NumParam))
                    {
                        print_page_free(page);
                        page = NULL;
                        goto Fail;
                    }

                    // Init a list for bilevel planes that need to be saved until the Zx layer appears
                    ASSERT(ATIsListEmpty(&bilevelPlaneList));
                    ATInitList(&bilevelPlaneList);  
                    ZxStartReceived = FALSE;   
                    ASSERT(NULL != zjp_config);     
                                   
                    if (ZJ_DATA_FORMAT_JBIG == zjp_config->data_format)
                    {
                        DPRINTF(DBGZJP_ERR, (" @@ Page Start\n"));	// 2013.11.07		by Juny
                        print_page_start(page);
                    }
                    else
                    {
                        XASSERT(ZJ_DATA_FORMAT_ZX == zjp_config->data_format, zjp_config->data_format);
                        // Wait until we get the Zx layer info to send the page to the job manager
                    }
#endif // HAVE_PRINT_SUPPORT
                    break;          
                }

                /* Bi-Level Image Header. Simply attach the header data to the
                 * message and send it on to the job manager. We'll let the job
                 * manager sort out what to do with the header information.
                 */
            case ZJT_JBIG_BIH:   
                {
                    // JBIG header. Check to see if "tag length" is non-zero and 
                    // read in options if it is.
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ JBIG_Bi-IH\n"));	// 2013.12.06		by Juny

                    if(DataLength == 0)
                    {
                        goto Fail;
                    }
#if NUM_MARKING_COLOR_CHANNELS == 1
                    ASSERT(curPlaneColor == COLOR_BLACK);
#endif
                    // store for later use in the BID
                    plane_params[curPlaneColor].jbig_image_header = *(JBIG_BIH*)DataBuffer;
                    // on monochrome JBIG pages we do not receive plane start/end notifications
                    // from the host driver so for mono pages we send the plane start
                    // when we receive the jbig header notification.
                    if(page_output_color_mode == e_Mono)
                    {
                        print_plane_start(page, curPlaneColor);
                        DataBuffer = NULL;
                    }
                    DPRINTF(DBGZJP_SOFT, ("ZJPARSE: ZJT_JBIG_BIH DataLength = %d\n", DataLength));
                    break;
                }

                /* Bi-Level Image Data. Configure this in a similar fashion to 
                 * "plane data" below. This is the case of monochrome data.
                 */
            case ZJT_JBIG_BID:   // BiLevel Image Data
                {
                    compression_info_t compression_info; 

                    DPRINTF(DBGZJP_ERR, (" @@ ZJ JBIG_Bi-ID\n"));	// 2013.12.06		by Juny
                    DPRINTF(DBGZJP_LOUD, ("ZJPARSE: BID block: Color %d Len %d\n", curPlaneColor, DataLength));
                    ASSERT( DataLength > 0 );

                    ASSERT(plane_params[curPlaneColor].bpp != 0);
                    plane_params[curPlaneColor].bpp = (plane_params[curPlaneColor].bpp == 0) ? 1 : plane_params[curPlaneColor].bpp;

                    compression_info.comp_type = COMPTYPE_JBIG;
                    compression_info.comp_details.jbig.Xd =  zj32_to_cpu(plane_params[curPlaneColor].jbig_image_header.Xd);
                    compression_info.comp_details.jbig.Yd =  zj32_to_cpu(plane_params[curPlaneColor].jbig_image_header.Yd);
                    compression_info.comp_details.jbig.L0 =  zj32_to_cpu(plane_params[curPlaneColor].jbig_image_header.L0);
                    compression_info.comp_details.jbig.option_flags = plane_params[curPlaneColor].jbig_image_header.Options;

                    print_plane_add_data(page, curPlaneColor, big_buffer, DataLength,
                                         zj_map_color_to_plane_type(curPlaneColor),
                                         plane_params[curPlaneColor].last_data,
                                         dpi_x, dpi_y,
                                         (plane_params[curPlaneColor].raster_x / plane_params[curPlaneColor].bpp),
                                         compression_info.comp_details.jbig.Yd,
                                         plane_params[curPlaneColor].bpp,
                                         true,
                                         &compression_info);
				       DPRINTF(DBGZJP_SOFT, ("ZJPARSE: JBIG BID = %d\n", plane_params[curPlaneColor].bpp));	// 2013.10.31		by Juny
                    DPRINTF(DBGZJP_SOFT, ("ZJPARSE: ZJT_JBIG_BID %#x\n", DataBuffer)); 
                    DataBuffer = NULL;
                    break;
                }

            case ZJT_END_JBIG:
                {
                    // on monochrome JBIG pages we do not receive plane start/end notifications
                    // from the host driver so for mono pages we send the plane end
                    // when we receive the jbig end notification.
                    if(page_output_color_mode == e_Mono)
                    {
                        print_plane_end(page, curPlaneColor);
                        DataBuffer = NULL;
                    }

                    DPRINTF(DBGZJP_ERR, (" @@ ZJ END JBIG\n"));	// 2013.12.06		by Juny
                    DPRINTF(DBGZJP_SOFT, ("ZJPARSE: ZJT_END_JBIG: %d\n", DataLength));
                    break;
                }

            case ZJT_BITMAP:
                {
                    COLOR plane_color;
                    compression_info_t compression_info; 

                    plane_color = ScanPlaneOptions(plane_params, TagBuffer, TagLength, NumParam);

                    if(DataLength > 0)
                    {
                        ASSERT(plane_params[plane_color].bpp != 0);
                        plane_params[plane_color].bpp = (plane_params[plane_color].bpp == 0) ? 1 : plane_params[plane_color].bpp;

                        ASSERT(NULL != zjp_config);                    
                        if (ZJ_DATA_FORMAT_ZX == zjp_config->data_format)
                        {
                            if (TRUE == ZxStartReceived)
                            {
                                ASSERT(0);
                                // TODO: this is untested.  not sure if it is correct
                                print_plane_add_data(page, COLOR_CYAN, big_buffer, DataLength,
                                                     ZX_STREAM,
                                                     plane_params[plane_color].last_data,
                                                     dpi_x, dpi_y,
                                                     plane_params[plane_color].raster_x,
                                                     plane_params[plane_color].raster_y,
                                                     plane_params[plane_color].bpp,
                                                     true,
                                                     NULL);
                            }
                            else
                            {
                                bilevel_plane_node_t* plane_node = MEM_MALLOC_LIMITED(mlimiter, sizeof(plane_node));
                                ASSERT(plane_node);
                                if (plane_node)
                                {
                                    plane_node->buffer = big_buffer;
                                    plane_node->data_len = DataLength;
                                    plane_node->plane_params = plane_params[plane_color];
                                    ATInsertTailList(&bilevelPlaneList, &plane_node->node);
                                }
                            }
                        }
                        else
                        {
                            XASSERT((ZJ_DATA_FORMAT_JBIG==zjp_config->data_format), zjp_config->data_format);

                            compression_info.comp_type = COMPTYPE_JBIG;
                            compression_info.comp_details.jbig.Xd =  zj32_to_cpu(plane_params[plane_color].jbig_image_header.Xd);
                            compression_info.comp_details.jbig.Yd =  zj32_to_cpu(plane_params[plane_color].jbig_image_header.Yd);
                            compression_info.comp_details.jbig.L0 =  zj32_to_cpu(plane_params[plane_color].jbig_image_header.L0);
                            compression_info.comp_details.jbig.option_flags = plane_params[plane_color].jbig_image_header.Options;

                            print_plane_add_data(page, plane_color, big_buffer, DataLength, 
                                                 zj_map_color_to_plane_type(plane_color),
                                                 plane_params[plane_color].last_data,
                                                 dpi_x, dpi_y,
                                                 (plane_params[plane_color].raster_x / plane_params[plane_color].bpp),
                                                 compression_info.comp_details.jbig.Yd,
                                                 plane_params[plane_color].bpp,
                                                 true,
                                                 &compression_info);
                        }

                        DataBuffer = NULL;
                    }
                    break;
                }
            case ZJT_START_PLANE:
                {
                    DPRINTF(DBGZJP_LOUD, ("ZJPARSE: start PLANE\n"));
                    curPlaneColor = ScanPlaneOptions(plane_params, TagBuffer, TagLength, NumParam);
                    print_plane_start(page, curPlaneColor);
                    DPRINTF(DBGZJP_LOUD, ("ZJPARSE: PLANE color %d\n", curPlaneColor));
                    break;
                }

            case ZJT_END_PLANE:
                {
                    COLOR plane_color;

                    plane_color = ScanPlaneOptions(plane_params, TagBuffer, TagLength, NumParam);
                    ASSERT(curPlaneColor == curPlaneColor);
                    print_plane_end(page, plane_color);
                    break;
                }
            case ZJT_ZX_START:
                {   
                    // We send off the page data struct after we gather the params for the Zx layer
                    ZJ_ITEM_HEADER *Item;
                    uint32_t i, BufPtr = 0;                           

                    DPRINTF(DBGZJP_ERR, (" @@ ZJ ZX START\n"));	// 2013.12.06		by Juny                
                    DPRINTF(DBGZJP_SOFT, ("ZJT_ZX_START received.\n"));
              
                    Item = (ZJ_ITEM_HEADER*)((uint32_t)TagBuffer + BufPtr);

                    if ( 0 == NumParam )
                    {
                        // most params should be set from parsing the page information
                        // set dpi here just in case
                        print_page_add_zx_res_x(page, dpi_x);
                        print_page_add_zx_res_y(page, dpi_y);
                    }
                    else
                    {
                        for(i = 0; i < NumParam; i++)
                        {
                            switch(Item->Type)
                            {
                                case ZJIT_UINT32:
                                    {
                                        switch(Item->Item)
                                        {
                                            case ZJI_RASTER_X:   
                                                print_page_add_zx_raster_x(page, ((ZJ_ITEM_UINT32 *)Item)->value);
                                                break;
                                            case ZJI_RASTER_Y:   
                                                print_page_add_zx_raster_y(page, ((ZJ_ITEM_UINT32 *)Item)->value);
                                                break;
                                            case ZJI_RESOLUTION_X:
                                                print_page_add_zx_res_x(page, ((ZJ_ITEM_UINT32 *)Item)->value);
                                                break;
                                            case ZJI_RESOLUTION_Y:
                                                print_page_add_zx_res_y(page, ((ZJ_ITEM_UINT32 *)Item)->value);
                                                break;
                                            case ZJI_COLOR_FORMAT:
                                                break;
                                            case ZJI_COLOR_SIZE:
                                                print_page_add_zx_color_size(page, ((ZJ_ITEM_UINT32 *)Item)->value);
                                                break;
                                            case ZJI_ZxBITS:
                                                break;
                                            default:
                                                DPRINTF(DBGZJP_LOUD, ( "ZJP: WARNING, unhandled Zx item value %d\n", Item->Item ));
                                                break;
                                        }
                                    }
                            }
                            Item = (ZJ_ITEM_HEADER*)(((uint32_t)Item) + Item->cbSize);
                        }
                    }

                    // Make sure bpp is consistent with zx_params.color_size_in_bytes.
//-jrsTODO                    Page->image_output_info.image_bpp = 8 * Page->zx_params.color_size_in_bytes;

                    /* JBIG ZJ hosts send down RASTER_X as the width in _bits_ of the image. On the other hand, Zx Stream
                     * hosts send it as the width in _pixels_. To make everything consistent downstream, we will convert
                     * to the JBIG host standard here.
                     */
//-jrsTODO                    Page->image_output_info.image_data_width_in_bytes *= Page->image_output_info.image_bpp;

                    // Send the page to the job manager now that we have the Zx layer info
				       DPRINTF(DBGZJP_ERR, (" @@ ZX Page Start\n"));	// not act 2013.11.07		by Juny
                    print_page_start(page);
                    
                    // Send any queued bilevel planes
                    sendQueuedBilevelPlanes(page, &bilevelPlaneList );
                    ZxStartReceived = TRUE;
                    break;
                }
            case ZJT_ZX_DATA:
                {
                    ASSERT(0 != DataLength);

                    ASSERT(0);
                    // TODO: this is untested.  not sure if it is correct
                    //NumParam is cbSize from the header for this block; it
                    //contains the actual number of valid zx block bytes
                    print_plane_add_data(page, COLOR_BLACK, big_buffer, NumParam,
                                         ZX_STREAM,
                                         0,
                                         dpi_x, dpi_y,
                                         plane_params[COLOR_BLACK].raster_x,
                                         plane_params[COLOR_BLACK].raster_y,
                                         plane_params[COLOR_BLACK].bpp,
                                         true,
                                         NULL);
                    DataBuffer = NULL;
                    break;
                }
            case ZJT_ZX_END:
                {
                    // no more zx blocks, so send job plane end to job manager
                    print_plane_end(page, COLOR_BLACK);
                    break;
                }
            case ZJT_END_PAGE:
                {
                    DPRINTF(DBGZJP_ERR, (" @@ Page End\n"));	// 2013.12.06		by Juny
#ifdef PIXELCNT_BYHOST
                    {
                        PIXELCNT *pix_counts;
                        ScanPlaneOptions (&plane_params, TagBuffer, TagLength, NumParam)
                        pix_counts = (PIXELCNT *) MEM_MALLOC_LIMITED(mlimiter, sizeof(PIXELCNT));
                        ASSERT(pix_counts != NULL);

                        // We should have our pixel values.  Update the page header.
                        memcpy(pix_counts, &pixels, sizeof(PIXELCNT));
                    }
                    print_page_end(page, pix_counts);
#else
                    print_page_end(page, NULL);
#endif
                    page = NULL;
                    break;
                }

            case ZJT_END_DOC:
                {
                    DPRINTF(DBGZJP_ERR, (" @@ ZJ End Job (END DOC)\n"));	// 2013.12.06		by Juny
                    print_job_end(zj_current_job);
                    zj_current_job = NULL;
                    done = 1;                      // all done, get out of here.
                    break;
                }

            case ZJT_PAUSE:
                {
                    startManualDuplex = TRUE;
                    DPRINTF(DBGZJP_SOFT, ("ZJPARSE: start manual duplex\n"));

                    ScanPlaneOptions(plane_params, TagBuffer, TagLength, NumParam);
                    break;
                }

            default:
                {
                    //
                    // This is an unknown record, throw it out.
                    //
                    DPRINTF(DBGZJP_ERR, ("ZJPARSE: unknown block type %#x\n", BlkType));
                    break;
                }
        } // end switch ( BlkType )
        // 
        // Free the tag buffer if it is allocated.
        //
        if(TagLength > 0)
        {
            MEM_FREE_AND_NULL(TagBuffer);
        }
        TagBuffer = NULL;    

        // Free the data buffer if it was not passed along
        if(DataBuffer != NULL)
        {
            MEM_FREE_AND_NULL(DataBuffer);
            DataBuffer = NULL;
        }
    } // end while ( !done )

    //if we get here we succeeded in printing the job
    return PARSER_SUCCESS;

    Fail:
    {
        DPRINTF(DBGZJP_ERR, ("ZJPARSE: Error DataBuffer %#x\n",DataBuffer));
        LASTPARSEERROR = 1;     // this job has aborted.
        
        if(TagBuffer != NULL)
        {
            MEM_FREE_AND_NULL(TagBuffer);
        }

        if(DataBuffer != NULL)
        {
            MEM_FREE_AND_NULL(DataBuffer);
        }
        //
        // if we have received a job start then we need to send a MSG_JOBABORT to the job manager. This
        // will either cancel the job or if in the case where there is another job in the queue ahead of
        // us it will print any pages we have already sent. Refer to bug #14595.
        //
        if(JobStart)
        {
            DPRINTF(DBGZJP_ERR, (" @@ ZJ Fail: abort to job_dest\n"));	// 2013.12.06		by Juny
            print_job_abort(zj_current_job);
        }
        // fast uel  flush

        dbg_printf(" ---------------------------------------------- pre uel flush\n");
        int cnt = 0;
        while (0 < (bytesRead = Pipe->ReadConnect(Pipe, gZJBuffer, 512, PARSEREAD_SOME)))
//        while (512 == (bytesRead = Pipe->ReadConnect(Pipe, gZJBuffer, 512, PARSEREAD_SOME)))
        {
            int i;
            cnt += bytesRead;
            // dprintf(" - ------------ read cnt %d %d\n", cnt, bytesRead); 
#if 1		/* add new checking of ZJT_END_DOC */
            for(i = 0; bytesRead > 15 && i < bytesRead; i++) {
                if (
                    (gZJBuffer[i] == 0x1b &&
                    gZJBuffer[i+1] == '%' &&
                    gZJBuffer[i+2] == '-' &&
                    gZJBuffer[i+3] == '1' &&
                    gZJBuffer[i+4] == '2' &&
                    gZJBuffer[i+5] == '3' &&
                    gZJBuffer[i+6] == '4' &&
                    gZJBuffer[i+7] == '5' &&
                    gZJBuffer[i+8] == 'X' 
                    ) || (gZJBuffer[i] == 0x4a &&
                    gZJBuffer[i+1] == 0x5a &&
                    gZJBuffer[i+2] == 0x4a &&
                    gZJBuffer[i+3] == 0x5a 
                    )) 
                {
                    // Found UEL put data back and exit.
                    Pipe->RewindData (Pipe, gZJBuffer+i, bytesRead-i);
//                    Pipe->RewindData (Pipe, gZJBuffer, bytesRead);		//origine
                    dbg_printf(" - ------------ found uel on parser cancel read %d i %d cnt %d\n", 
                            bytesRead, i, cnt);
                    return(PARSER_ERROR);
                }
/* 2013.12.06 16:00 ON */
              else if (
                    gZJBuffer[i] == 0x00 &&
                    gZJBuffer[i+1] == 0x00 &&
                    gZJBuffer[i+2] == 0x00 &&
                    gZJBuffer[i+3] == 0x10 &&
                    gZJBuffer[i+4] == 0x00 &&
                    gZJBuffer[i+5] == 0x00 &&
                    gZJBuffer[i+6] == 0x00 &&
                    gZJBuffer[i+7] == 0x01 &&
                    gZJBuffer[i+8] == 0x00 && 
                    gZJBuffer[i+9] == 0x00 &&
                    gZJBuffer[i+10] == 0x00 &&
                    gZJBuffer[i+11] == 0x00 &&
                    gZJBuffer[i+12] == 0x00 &&
                    gZJBuffer[i+13] == 0x00 &&
                    gZJBuffer[i+14] == 0x5a &&
                    gZJBuffer[i+15] == 0x5a
                    ) 
                {
                    // found ZJT_END_DOC for cancel, put data back and exit.
                    Pipe->RewindData (Pipe, gZJBuffer+16+i, bytesRead-16-i);
                    dbg_printf(" - ------------ found ZJT parser cancel read %d i %d cnt %d\n", 
                            bytesRead, i, cnt);
                    return(PARSER_ERROR);
                }

            }
#else
            for(i = 0; bytesRead > 8 && i < bytesRead; i++) {
                if (
                    (gZJBuffer[i] == 0x1b &&
                    gZJBuffer[i+1] == '%' &&
                    gZJBuffer[i+2] == '-' &&
                    gZJBuffer[i+3] == '1' &&
                    gZJBuffer[i+4] == '2' &&
                    gZJBuffer[i+5] == '3' &&
                    gZJBuffer[i+6] == '4' &&
                    gZJBuffer[i+7] == '5' &&
                    gZJBuffer[i+8] == 'X' 
                    ) || (gZJBuffer[i] == 0x4a &&
                    gZJBuffer[i+1] == 0x5a &&
                    gZJBuffer[i+2] == 0x4a &&
                    gZJBuffer[i+3] == 0x5a 
                    )) 
                {
                    // Found UEL put data back and exit.
                    Pipe->RewindData (Pipe, gZJBuffer+i, bytesRead-i);
                    dbg_printf(" - ------------ found uel on parser cancel read %d i %d cnt %d\n", 
                            bytesRead, i, cnt);
                    return(PARSER_ERROR);
                }
            }
#endif
        }
        dbg_printf("-- ------------------------------------- post flush %d\n", cnt);
//        dbg_printf(" - ------------ maybe end and ignore, cancel read %d \n", bytesRead);
//		  Pipe->RewindData (Pipe, gZJBuffer, 0);	// new

        return(PARSER_ERROR);
    }
} // end DoParsing()

static void sendQueuedBilevelPlanes( print_page_t* page, ATLISTENTRY *pList )
{
    ATLISTENTRY* node;
    bilevel_plane_node_t* plane;

    while(NULL != (node = ATRemoveHeadList(pList)))
    {
        plane = CONTAINING_RECORD(node, bilevel_plane_node_t, node);
        ASSERT(plane != NULL);

        // TODO: this is untested.  not sure if it is correct
        print_plane_add_data(page, COLOR_CYAN, plane->buffer, plane->data_len,
                             ZX_STREAM,
                             plane->plane_params.last_data,
                             dpi_x, dpi_y,
                             plane->plane_params.raster_x,
                             plane->plane_params.raster_y,
                             plane->plane_params.bpp,
                             true,
                             NULL);
        MEM_FREE_AND_NULL(plane);
    }
}


//------------------------------------------------------------------------------
static const char* DecodeZjBufType( ZJ_TYPE zjType )
{
#if defined( HAVE_DBG_PRINTF )
    switch ( zjType )
    {
    CASE_ENUM_RETURN_STRING( ZJT_START_DOC );
    CASE_ENUM_RETURN_STRING( ZJT_END_DOC );
    CASE_ENUM_RETURN_STRING( ZJT_START_PAGE );
    CASE_ENUM_RETURN_STRING( ZJT_END_PAGE );
    CASE_ENUM_RETURN_STRING( ZJT_JBIG_BIH );
    CASE_ENUM_RETURN_STRING( ZJT_JBIG_BID );
    CASE_ENUM_RETURN_STRING( ZJT_END_JBIG );
    CASE_ENUM_RETURN_STRING( ZJT_SIGNATURE );
    CASE_ENUM_RETURN_STRING( ZJT_RAW_IMAGE );
    CASE_ENUM_RETURN_STRING( ZJT_START_PLANE );
    CASE_ENUM_RETURN_STRING( ZJT_END_PLANE );
    CASE_ENUM_RETURN_STRING( ZJT_PAUSE );
    CASE_ENUM_RETURN_STRING( ZJT_BITMAP );
    CASE_ENUM_RETURN_STRING( ZJT_ZX_START );
    CASE_ENUM_RETURN_STRING( ZJT_ZX_DATA );
    CASE_ENUM_RETURN_STRING( ZJT_ZX_END );
    DEFAULT_CASE_ENUM_RETURN_STRING( ZJ_TYPE );
    } // end switch ( zjType )
#endif // defined( HAVE_DBG_PRINTF )
    return "bug";
} // end DecodeZjBufType()

//------------------------------------------------------------------------------
static const char* ToString_ZJ_ITEM( uint16_t zjItem )
{
#if defined( HAVE_DBG_PRINTF )
    switch ( zjItem )
    {
    CASE_ENUM_RETURN_STRING( ZJI_PAGECOUNT );
    CASE_ENUM_RETURN_STRING( ZJI_DMCOLLATE );
    CASE_ENUM_RETURN_STRING( ZJI_DMDUPLEX );
    CASE_ENUM_RETURN_STRING( ZJI_DMPAPER );
    CASE_ENUM_RETURN_STRING( ZJI_DMCOPIES );
    CASE_ENUM_RETURN_STRING( ZJI_DMDEFAULTSOURCE );
    CASE_ENUM_RETURN_STRING( ZJI_DMMEDIATYPE );
    CASE_ENUM_RETURN_STRING( ZJI_NBIE );
    CASE_ENUM_RETURN_STRING( ZJI_RESOLUTION_X );
    CASE_ENUM_RETURN_STRING( ZJI_RESOLUTION_Y );
    CASE_ENUM_RETURN_STRING( ZJI_OFFSET_X );
    CASE_ENUM_RETURN_STRING( ZJI_OFFSET_Y );
    CASE_ENUM_RETURN_STRING( ZJI_RASTER_X );
    CASE_ENUM_RETURN_STRING( ZJI_RASTER_Y );
    CASE_ENUM_RETURN_STRING( ZJI_COLLATE );
    CASE_ENUM_RETURN_STRING( ZJI_QUANTITY );
    CASE_ENUM_RETURN_STRING( ZJI_VIDEO_BPP );
    CASE_ENUM_RETURN_STRING( ZJI_VIDEO_X );
    CASE_ENUM_RETURN_STRING( ZJI_VIDEO_Y );
    CASE_ENUM_RETURN_STRING( ZJI_INTERLACE );
    CASE_ENUM_RETURN_STRING( ZJI_PLANE );
    CASE_ENUM_RETURN_STRING( ZJI_PALETTE );
    CASE_ENUM_RETURN_STRING( ZJI_RET );
    CASE_ENUM_RETURN_STRING( ZJI_TONER_SAVE );
    CASE_ENUM_RETURN_STRING( ZJI_MEDIA_SIZE_X );
    CASE_ENUM_RETURN_STRING( ZJI_MEDIA_SIZE_Y );
    CASE_ENUM_RETURN_STRING( ZJI_MEDIA_SIZE_UNITS );
    CASE_ENUM_RETURN_STRING( ZJI_CHROMATIC );
    CASE_ENUM_RETURN_STRING( ZJI_PAD );
    CASE_ENUM_RETURN_STRING( ZJI_PROMPT );
    CASE_ENUM_RETURN_STRING( ZJI_BITMAP_TYPE );
    CASE_ENUM_RETURN_STRING( ZJI_ENCODING_DATA );
    CASE_ENUM_RETURN_STRING( ZJI_END_PLANE );
    CASE_ENUM_RETURN_STRING( ZJI_BITMAP_PIXELS );
    CASE_ENUM_RETURN_STRING( ZJI_BITMAP_LINES );
    CASE_ENUM_RETURN_STRING( ZJI_BITMAP_BPP );
    CASE_ENUM_RETURN_STRING( ZJI_BITMAP_STRIDE );
    CASE_ENUM_RETURN_STRING( ZJI_COLOR_FORMAT );
    CASE_ENUM_RETURN_STRING( ZJI_COLOR_COMPONENT_SIZE );
    CASE_ENUM_RETURN_STRING( ZJI_COLOR_SIZE );
    CASE_ENUM_RETURN_STRING( ZJI_ZxBITS );
    CASE_ENUM_RETURN_STRING( ZJI_OUTBIN );
    /* for instrumentation */
    CASE_ENUM_RETURN_STRING( ZJI_TIMESTAMP );
    /* 0x8000-0x80FF : Item tags for OEM specific features. */
    /* 0x8100-0x81FF : Item tags for OEM specific features. */
    /* 0x8200-0x82FF : Item tags for the next OEM specific features. */
    CASE_ENUM_RETURN_STRING( ZJI_SW_PIXELCOUNT_C );
    CASE_ENUM_RETURN_STRING( ZJI_SW_PIXELCOUNT_M );
    CASE_ENUM_RETURN_STRING( ZJI_SW_PIXELCOUNT_Y );
    CASE_ENUM_RETURN_STRING( ZJI_SW_PIXELCOUNT_K );
    CASE_ENUM_RETURN_STRING( ZJI_SW_PIXELCOUNT_NW_C );
    CASE_ENUM_RETURN_STRING( ZJI_SW_PIXELCOUNT_NW_M );
    CASE_ENUM_RETURN_STRING( ZJI_SW_PIXELCOUNT_NW_Y );
    CASE_ENUM_RETURN_STRING( ZJI_SW_PIXELCOUNT_NW_K );
    DEFAULT_CASE_ENUM_RETURN_STRING( ZJ_ITEM );
    } // end switch ( zjItem )
#endif // defined( HAVE_DBG_PRINTF )
    return "bug";
} // end ToString_ZJ_ITEM()


