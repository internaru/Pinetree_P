/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include <string.h>
#include <dprintf.h>
#include "agRouter.h"
#include "ATypes.h"
#include "lassert.h"
#include "agConnectMgr.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "faxer_api.h"
#include "fax_tiff_io.h"
#include "fax_app_prv.h"

#include "paper_size_api.h"

#ifdef HAVE_SCAN_SUPPORT
#include "scantypes.h"
#include "scanvars.h"
#include "scanplat.h"
#include "scands.h"
#include "scanmsg.h"
#endif
#include "utils.h"


#include "fax_app_jpeg.h"
#include "fax_header.h"
#include "posix_ostools.h"

#define FAX_SCAN_RESOLUTION 300  // when we scan for mono we always do this resolution.
#if 0 /* scan currently is always passing back 300 dpi data */
#define FAX_COLOR_SCAN_RESOLUTION 200  
#else
#define FAX_JPEG_SCAN_RESOLUTION 300  
#endif
#define FAX_COLOR_SCAN_COMPONENTS 4 /* RGBX */

#define FAX_SCAN_WIDTH_A4 827
#define FAX_SCAN_HEIGHT_A4 1169

#define FAX_SCAN_WIDTH_LETTER 850
#define FAX_SCAN_HEIGHT_LETTER 1100

#define FAX_SCAN_SRC_SCALE 100 /* CURRENTLY SCAN HANGS IF WE CHANGE THIS...
                                  percent to scale source to leave room for header */

/* set to 1 to see all scan events on serial */
#define FAX_DEBUG_SCAN_EVENTS (0)


typedef struct
{
  void *context;
  fax_app_subjob_callback_t update_callback; 
  char *fname;
  fax_store_handle_t store;
  void *tiff_writer_handle;
  bool color;
  struct jpeg_compress_struct jpeg_comp_handle;
  fax_app_brightness_t brightness;
  fax_app_resolution_t resolution;
  uint16_t active_page;
  uint16_t pages_completed;
} _scan_job_info_t;
/* only one at this time */
_scan_job_info_t _scan_info;

/*----------------------------------------------------------*/
#if FAX_DEBUG_SCAN_EVENTS
#define _TS(_a) {_a, #_a}
static const char *msg_to_string(uint32_t msg)
{
  int i;
  typedef struct
  {
    AGMSG msg;
    char *str; 
  } msg_to_str_t;
  static const msg_to_str_t string_tbl[] = 
  {
    _TS(MSG_RESOURCES),
    _TS(MSG_FAXER_EVT),
    _TS(MSG_ACKRECIPE),
    _TS(MSG_NAKRECIPE),
    _TS(MSG_SCAN_SIZE),
    _TS(MSG_SCAN_PAGESTART),
    _TS(MSG_SCAN_PLANEDATA),
    _TS(MSG_SCAN_PAGEEND),
    _TS(MSG_SCAN_JOBSTART),
    _TS(MSG_SCAN_JOBEND),
  };

  for(i=0; i<ARRAY_SIZE(string_tbl); i++) 
  {
    if(string_tbl[i].msg == msg)
     break;
  }
  if(i<ARRAY_SIZE(string_tbl))
    return(string_tbl[i].str);
  else
    return("UNKNOWN_MSG");
}
#endif

/*----------------------------------------------------------*/
static void _update_status(fax_subjob_state_t state)
{
  fax_subjob_update_t update;
  /* clear it out */
  memset(&update, 0, sizeof(update));
  /* only context id and state are needed */
  update.context = _scan_info.context;
  update.state = state;
  update.active_page = _scan_info.active_page;
  update.pages_completed = _scan_info.pages_completed;
  (*_scan_info.update_callback)(&update);
}

#ifdef HAVE_SCAN_SUPPORT
/*----------------------------------------------------------*/
/* Check to see if we are faxing from the flatbed or ADF.   */
static bool _flatbed_scan(void)
{
  uint32_t flag = 0;
  scan_err_t scerr;

  scerr = scands_get_integer( "adf_paper_present", &flag );
  if(scerr != SCANERR_NONE)
  {
    flag = 0;  /* just in case the function modified flag */
  }

  return(!flag);
}

/*----------------------------------------------------------*/
/* setup the scan job */
static struct scanvars *_setup_scan(_scan_job_info_t *scan_info)
{
  int32_t             ResX, ResY;
  uint32_t            scan_width = 0xffffffff, scan_height;
  struct scanvars     *sv = NULL;
  uint32_t            scan_resolution;

  if(scan_info->store)
  {
    scan_width  = FAX_SCAN_WIDTH_LETTER;
    scan_height = FAX_SCAN_HEIGHT_LETTER; 
             
    /* hardcoded to 200x200 for now given that is the MUST support for color fax */
    scan_resolution = FAX_JPEG_SCAN_RESOLUTION;
    if(scan_info->resolution == FAX_TYPE_PHOTO)
    {
      ResX = 300;
      ResY = 300;
    }
    else 
    {
      ResX = 200;
      ResY = 200;
    }

    if(scan_info->color)
    {
      sv = scanplat_sv_color_scan_new();
      scanvar_set_pieout(sv, PIE_PIXEL_RGBX); 
    }
    else 
    {
      sv = scanplat_sv_mono_scan_new();
      scanvar_set_pieout(sv, PIE_PIXEL_MONO); 
    }
  }
  else
  {
    scan_width  = FAX_SCAN_WIDTH_LETTER;
    scan_height = FAX_SCAN_HEIGHT_LETTER; 

    /* setup tiff */
    scan_resolution = FAX_SCAN_RESOLUTION;
    scan_info->tiff_writer_handle = fax_tiff_writer_init(scan_info->fname, scan_info->resolution, FAX_PGSIZE_LETTER);
               
    if(!scan_info->tiff_writer_handle)
    {
      dbg_printf("Fail to obtain TIFF writer, Abort!\n");
      return(NULL);
    }
    if(fax_tiff_writer_start_new_page(scan_info->tiff_writer_handle))
    {
      dbg_printf("Start page failure, Abort!\n");
      return(NULL);
    }

    if(scan_info->resolution == FAX_TYPE_PHOTO)
    {
      sv = scanplat_sv_mixed_fax_new();
    }
    else 
    {
      sv = scanplat_sv_text_fax_new();
    }

    if(fax_tiff_writer_get_dimensions(scan_info->tiff_writer_handle, &ResX, &ResY))//,&scan_width, &scan_height))
    {
      dbg_printf("Failed to get tiff dimensions, Abort!\n");
      return(NULL);
    }
  }

  if(sv == NULL)
  {
    dbg_printf("Fail to get new scan var, Abort!\n");
    return(NULL);
  }

  /* Set scan variables */
  if(scan_info->brightness < 1 || scan_info->brightness > 11)
  {
    dbg_printf("Invalid brightness %d, use 5\n", scan_info->brightness);
    scan_info->brightness = 5;
  }
  dbg_printf("Brightness %d\n", scan_info->brightness);

  if(_flatbed_scan())
  {
    scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_FLATBED);
    dbg_printf("FAX SCAN: Fax from flatbed\n");
  }
  else
  {
    scanvar_set_document_source(sv, SCAN_DOCUMENT_SOURCE_ADF);
    dbg_printf("FAX SCAN: Fax from ADF\n");
  }
              
  scanvar_set_copy_brightness(sv, scan_info->brightness);
  scanvar_set_dpi(sv, scan_resolution);
#if 0 /* the numnbers in this scale call don't make sense */
                /*
                 * Ask scanner to convert its scanned image to A4 size (that's what what we calim in fax send),
                 * with correct resolutions.
                 *  Note I have to use /10 to scale down input values here otherwise scanvar_set_xyscale
                 * won't calculate, even thought it claims support integer type.  */
                scanvar_set_xyscale(sv, ResX*827/10,scan_resolution*scan_width/10, ResY*1169/10,scan_resolution*scan_height/10);
#else
  dbg_printf("scanver_set_xyscale: %d %d | %d %d\n", 
             (ResX*FAX_SCAN_SRC_SCALE)/100, scan_resolution, (ResY*FAX_SCAN_SRC_SCALE)/100, scan_resolution);
  scanvar_set_xyscale(sv, (ResX*FAX_SCAN_SRC_SCALE)/100, scan_resolution, (ResY*FAX_SCAN_SRC_SCALE)/100, scan_resolution);
#endif
  scanvar_set_area(sv, 0, 0, scan_width, scan_height);
  return(sv);
}

#endif


/*----------------------------------------------------------*/
static uint32_t _write_tiff(uint8_t *buf, int rows, int row_size)
{ 
  int res;
  
  res = fax_app_faxer_free_storage(rows*row_size); 
  if(res)
  {
    dbg_printf("FAXAPP: out of storage memory, sending cancel.\n");
    _update_status(FAX_SUBJOB_CANCELING);
    fax_app_scan_cancel(_scan_info.context);
  }
  else if((res = fax_tiff_writer_write(_scan_info.tiff_writer_handle, buf, rows, row_size)))
  {
    dbg_printf("FAXAPP: call to faxer_tiff_writer_write() failed, sending cancel.\n");
    _update_status(FAX_SUBJOB_CANCELING);
    fax_app_scan_cancel(_scan_info.context);
  }
  return(res);
}


/*----------------------------------------------------------*/
/*
 *  Fax application code for scanning.
 *
 */

void* fax_app_agent_scan(void* Input)
{
    mqd_t              *pQueue =(mqd_t *)Input;
    MESSAGE             job_msg;    // Warning -- this message is used by the job macros
    MESSAGE             msg;
    CURRENT_RESOURCE    *OurResource= NULL;
    int pix_width = 0; 
    int pix_height = 0;
    struct scanvars     *sv = NULL;
    int                 scan_bytes_per_row_padded = 1;
    int                 first_pagestart_seen = 0;
    void *header;
    uint32_t pixels_per_row_padded, pixels_per_row, total_rows, bits_per_pixel;
    uint16_t num_rows;
    uint8_t *planedata;
#ifdef HAVE_SCAN_SUPPORT
    scan_data_type dtype;
#endif
    BOOL last_buffer;

    while(1)
    {
        if(0 != posix_wait_for_message(*pQueue, (char*)&msg,  sizeof(MESSAGE), 
                                       POSIX_WAIT_FOREVER)) 
            continue;
#if FAX_DEBUG_SCAN_EVENTS
        dbg_printf("FAX SCAN: received event %s\n", msg_to_string(msg.msgType));
#endif
            
        switch(msg.msgType)
        {
            case MSG_FAXER_EVT:
                break;
            
            case MSG_RESOURCES:
                OurResource = (CURRENT_RESOURCE *) msg.param3;
                break;
                
            case MSG_ACKRECIPE:
                scan_bytes_per_row_padded = 1;
                first_pagestart_seen = 0;
               
 
#ifdef HAVE_SCAN_SUPPORT
                sv = _setup_scan(&_scan_info); 
#else
                sv = NULL;
#endif
                if(!sv)
                {
                  dbg_printf("FAXAPP: Could not setup for scan!\n");
                  _update_status(FAX_SUBJOB_CANCELING);
                  fax_app_scan_cancel(_scan_info.context);
                  break;
                }

                // Send a message to the scanner to start the job.
                msg.msgType = MSG_SCAN_JOBSTART;
                msg.param3  = sv;
                SYMsgSend( OurResource->Source, &msg );

                _update_status(FAX_SUBJOB_ACTIVE);
                break;
                
            case MSG_NAKRECIPE:
                _update_status(FAX_SUBJOB_CANCELED);
                dbg_printf("Got NAKRECEIPE! Abort scanning!\n");
                break;
                
            case MSG_SCAN_SIZE:
                /* davep 22-Apr-2011 ; change to new scan size message */
#ifdef HAVE_SCAN_SUPPORT
                msg_scan_size_decode( &msg, &pixels_per_row_padded, &pixels_per_row, &total_rows, &bits_per_pixel );
#else
                pixels_per_row_padded = 0;
                pixels_per_row = 0;
                total_rows = 0;
                bits_per_pixel = 0;
#endif
                /* poison the structure to catch someone using old fields */
                memset( &msg, 0xaa, sizeof(MESSAGE));

                if(_scan_info.store)
                {
                    pix_height = total_rows;
                    pix_width = pixels_per_row_padded;
                    int pix_per_row = pixels_per_row;
                  if(_scan_info.color)
                  {
                    scan_bytes_per_row_padded = pix_width * FAX_COLOR_SCAN_COMPONENTS;
                  }
                  else
                  {
                    scan_bytes_per_row_padded = pix_width;
                  }
                  dbg_printf("FAXAPP_SCAN_WIDTH = %d\n", pix_per_row);
                }
                else
                {
                  /* mono, pixels to bytes */
                  scan_bytes_per_row_padded = pixels_per_row_padded>>3;
                }
                break;
             
            case MSG_SCAN_PLANEDATA:
                /* davep 22-Apr-2011 ; change to new scan planedata message */
#ifdef HAVE_SCAN_SUPPORT
                msg_scan_planedata_decode( &msg, &num_rows, &dtype, &planedata, &last_buffer );
#else
                num_rows = 0;
                planedata = NULL;
                last_buffer = true;
#endif
                /* poison the structure to catch someone using old fields */
                memset( &msg, 0xaa, sizeof(MESSAGE));

                if(_scan_info.store)
                {
                  unsigned char * buffer = (unsigned char*)planedata;
                  int rows = num_rows;
                  
                  fax_app_jpeg_write(&_scan_info.jpeg_comp_handle, buffer, rows);
                }
                else
                {
                  /* write data to tiff */
                  _write_tiff((UINT8 *)planedata, num_rows, scan_bytes_per_row_padded);
                }
                /*
                 * Scan code allocated the buffer for us.  Now that we've dealt with it, we
                 * need to free it for them (they have no way to know we're done with it).
                 */
                MEM_FREE_AND_NULL(planedata);
                break;
            
            case MSG_CANCELJOB:
                END_JOB(OurResource, FAXER_SCAN_JOB_AGENT_ID);
                ACK_CANCEL(msg.param3, FAXER_SCAN_JOB_AGENT_ID);

                if(_scan_info.store)
                  fax_app_jpeg_destroy_compress(&_scan_info.jpeg_comp_handle);

                if(sv)
                {
#ifdef HAVE_SCAN_SUPPORT
                   scanvar_delete( &sv );
#endif
                   sv = NULL;
                }
                if(_scan_info.tiff_writer_handle)
                {
                    fax_tiff_writer_close(_scan_info.tiff_writer_handle);
                    _scan_info.tiff_writer_handle = NULL;
                }

                first_pagestart_seen = 0;

                _update_status(FAX_SUBJOB_CANCELED);
                break;


            case MSG_SCAN_PAGESTART:
                {
                bool update_status = 1;
                char number[FAXER_MAX_SID_SIZE+1];
                char name[FAXER_MAX_HEADER_SIZE+1];

                fax_app_get_setting(FAXER_SETTING_CHR_SID, number, sizeof(number)); 
                fax_app_get_setting(FAXER_SETTING_CHR_HEADER_NAME, name, sizeof(name)); 

                _scan_info.active_page++;
                if(_scan_info.store)
                {
                  uint8_t *header_buf;
                  int header_rows;
                  int res;
                  fax_app_input_data_format_t src_format;

                  res = (_scan_info.resolution == FAX_TYPE_PHOTO)?300:200;

                  if(_scan_info.color)
                    src_format = FAX_INPUT_FORMAT_XBGR;
                  else
                    src_format = FAX_INPUT_FORMAT_GRAY;

                  /* pix height and pix_width are set upon size event */
                  fax_app_jpeg_start_compress(&_scan_info.jpeg_comp_handle, src_format, 
                                              pix_width, pix_height,
                                              res, 
                                              _scan_info.store);
                  if(_scan_info.color)
                    header = fax_header_build(scan_bytes_per_row_padded, res, res, FAX_HEADER_TYPE_RGBX, 
                                              _scan_info.active_page, name, number, NULL, FAX_HEADER_FORMAT_DATE);
                  else
                    header = fax_header_build(scan_bytes_per_row_padded, res, res, FAX_HEADER_TYPE_GRAYSCALE, 
                                              _scan_info.active_page, name, number, NULL, FAX_HEADER_FORMAT_DATE);

                  /* write header to jpeg */
                  fax_header_get_data(header, &header_buf, &header_rows);
                  fax_app_jpeg_write(&_scan_info.jpeg_comp_handle, header_buf, header_rows);
                }
                else
                {
                  uint8_t *header_buf;
                  int header_rows;
                  int32_t res_x, res_y;
                  if (!first_pagestart_seen)
                  {
                    first_pagestart_seen = 1;
                  }
                  else
                  {
                    fax_tiff_writer_start_new_page(_scan_info.tiff_writer_handle);
                  }
                  fax_tiff_writer_get_dimensions(_scan_info.tiff_writer_handle, &res_x, &res_y);
                  header = fax_header_build(scan_bytes_per_row_padded, res_x, res_y, FAX_HEADER_TYPE_MONO,
                                            _scan_info.active_page, name, number, NULL, FAX_HEADER_FORMAT_DATE);
                  fax_header_get_data(header, &header_buf, &header_rows);
                  if(_write_tiff(header_buf, header_rows, scan_bytes_per_row_padded))
                  {
                    update_status = 0;
                  }
                }
                if(header) fax_header_destroy(header);

                if(update_status)
                {
                  _update_status(FAX_SUBJOB_ACTIVE);
                }
                }
                break;           

            case MSG_SCAN_PAGEEND:
                _scan_info.pages_completed++;
                //Finish current page
                if(_scan_info.store)
                {
                  int size;
                  size = fax_app_jpeg_finish_compress(&_scan_info.jpeg_comp_handle);
                  dbg_printf("JPEG created, size = %d\n", size);
                  fax_app_jpeg_destroy_compress(&_scan_info.jpeg_comp_handle);
                  if(size == 0)
                  {
                    dbg_printf("FAXAPP: out of storage memory, sending cancel.\n");
                    _update_status(FAX_SUBJOB_CANCELING);
                    fax_app_scan_cancel(_scan_info.context);
                  }
                }
                else
                {
                  if(!_write_tiff(NULL, 0, scan_bytes_per_row_padded))
                  {
                    fax_tiff_writer_end_page(_scan_info.tiff_writer_handle);
                  }
                }
                break;

            case MSG_SCAN_JOBEND:

#ifdef HAVE_SCAN_SUPPORT
                    // Tell the scanner that we are all done.
                    // Remember to set the cookie, i.e. the scanvars variable.
                    msg.msgType = MSG_SCAN_JOBEND;
                    msg.param3  = sv;
                    SYMsgSend( OurResource->Source, &msg );
#endif

                    //End of scan
                    END_JOB(OurResource, FAXER_SCAN_JOB_AGENT_ID);
#ifdef HAVE_SCAN_SUPPORT
                    scanvar_delete( &sv );
#endif
                    sv = NULL;
                    if (_scan_info.store)
                        _update_status(FAX_SUBJOB_COMPLETE);
                    else
                    {
                      int r = fax_tiff_writer_close(_scan_info.tiff_writer_handle);
                      if(r)
                      {
                        _update_status(FAX_SUBJOB_CANCELING);
                        fax_app_scan_cancel(_scan_info.context);
                      }
                      else
                        _update_status(FAX_SUBJOB_COMPLETE);
                      _scan_info.tiff_writer_handle = NULL;
                     }
                break;


            default:
                dbg_printf("Unknown msg! type=%d\n", msg.msgType);
                break;
        }
    }
    return 0;
}

/*----------------------------------------------------------*/
int fax_app_scan_start_bw(char *fname, fax_app_brightness_t brightness, fax_app_resolution_t resolution, void *context, 
                         fax_app_subjob_callback_t callback )
{
  MESSAGE             job_msg;  
  STARTRECIPE         *StartIt;
  _scan_info.update_callback = callback;  
  _scan_info.context = context;
  _scan_info.fname = fname;
  _scan_info.store = 0;
  _scan_info.color = 0;
  _scan_info.brightness = brightness;
  _scan_info.resolution = resolution;
  _scan_info.active_page = 0;
  _scan_info.pages_completed = 0;
  START_JOB(ejob_ScanFax, FAXER_SCAN_JOB_AGENT_ID, 0);
  return(0);
}


/*----------------------------------------------------------*/
int fax_app_scan_start_color(fax_store_handle_t store, fax_app_brightness_t brightness, fax_app_resolution_t resolution,
                             void *context, 
                             fax_app_subjob_callback_t callback )
{
  MESSAGE             job_msg;  
  STARTRECIPE         *StartIt;
  _scan_info.update_callback = callback;  
  _scan_info.context = context;
  _scan_info.fname = NULL;
  _scan_info.store = store;
  _scan_info.color = 1;
  _scan_info.brightness = brightness;
  _scan_info.resolution = resolution;
  _scan_info.active_page = 0;
  _scan_info.pages_completed = 0;
  START_JOB(ejob_ScanFax, FAXER_SCAN_JOB_AGENT_ID, 0);
  return(0);
}

/*----------------------------------------------------------*/
int fax_app_scan_start_gray(fax_store_handle_t store, fax_app_brightness_t brightness, fax_app_resolution_t resolution,
                            void *context, 
                            fax_app_subjob_callback_t callback )
{
  MESSAGE             job_msg;  
  STARTRECIPE         *StartIt;
  _scan_info.update_callback = callback;  
  _scan_info.context = context;
  _scan_info.fname = NULL;
  _scan_info.store = store;
  _scan_info.color = 0;
  _scan_info.brightness = brightness;
  _scan_info.resolution = resolution;
  _scan_info.active_page = 0;
  _scan_info.pages_completed = 0;
  START_JOB(ejob_ScanFax, FAXER_SCAN_JOB_AGENT_ID, 0);
  return(0);
}

int fax_app_scan_cancel(void *context)
{
  MESSAGE msg;    
 
  memset( &msg, 0, sizeof(msg) );
  msg.msgType = MSG_CANCELJOB;
  msg.param1 = SYS_REQUEST;
  msg.param2 = 0;
  msg.param3 = (void*) e_ScanToFax;

  SYMsgSend( SJMID, &msg );
  //CANCEL_JOB_CONTAINING_MODULE(e_ScanToFax, e_ScanToFax); 
  return(0);
}
