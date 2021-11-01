/**
 * ============================================================================
 * Copyright (c) 2011  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* davep 23-Jul-2008 ; added -D__AGMESSAGE_H_ to makefile to prevent agMessage.h
 * from being included in the scan code.  But we need agMessage.h in this file.
 */
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lassert.h"
//#include "tx_api.h"
#include <pthread.h>
#include <posix_ostools.h>
#include "dprintf.h"
#include "debug.h"
#include "ioutils.h"
#include "logger.h"
#include "ATypes.h"
#include "agRouter.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "sys_init_api.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "ResourceMap.h"
#include "print_job_types.h"
//#include "print_job_types.h"
#include "acl_api.h"
#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "response_subject.h"
#include "copyapp_api.h"


//#define COPY_TO_HOST_DEBUG
#ifdef COPY_TO_HOST_DEBUG
#define cth_dbg2 dbg2
#else
#define cth_dbg2(...)
#endif

#define ACLCMD_READ_COPY_JOB 0x16ff
#define NUM_CTH_MESSAGES 10
#define CTH_STACK_SIZE POSIX_MIN_STACK_SIZE

#define CTH_HEADER_COOKIE  0x788ABEFD

/* these fields are used in sending image data to host */
#define CTH_HEADER_NULL   0  /* uninitialized/null - not used */
#define CTH_HEADER_START_JOB  1
#define CTH_HEADER_START_PAGE 2
#define CTH_HEADER_START_SHEET 3
#define CTH_HEADER_DATA 4
#define CTH_HEADER_END_SHEET 5
#define CTH_HEADER_END_PAGE 6
#define CTH_HEADER_END_JOB  7
#define CTH_HEADER_CANCEL 8

/* in cmd field in ACL acl_cth_xxx messages */
#define CTH_CMD_START          0x00     /* start the copy */
#define CTH_CMD_SET_RESIZE     0x01     /* set the resize */
#define CTH_CMD_USER_INPUT     0x02     /* fake user input */
#define CTH_CMD_SET_QUALITY    0x03     /* set the copy quality */
#define CTH_CMD_SET_NUM_COPIES 0x04     /* set the numcopies */
#define CTH_CMD_SET_BRIGHTNESS 0x05     /* set the copy light/dark */
#define CTH_CMD_IDCOPY_START   0x06     /* set the ID copy */
#define CTH_CMD_IDCOPY_NEXT    0x07     /* copy next ID doc */
#define CTH_CMD_IDCOPY_FINISH  0x08     /* finish the ID copy */
#define CTH_CMD_SET_TINT       0x09     /* set the color balance (tint) */
#define CTH_CMD_SET_COLORMODE  0x0A     /* set mode to color or mono */
#define CTH_CMD_SET_CONTRAST   0x0B     /* set the contrast */
#define CTH_CMD_SET_SHARP      0x0C     /* set the sharpness */
#define CTH_CMD_SET_BACKGROUND 0x0D     /* set background (white clip level) */
#define CTH_CMD_SET_ORIENT     0x0E     /* set orientation (landscape/portrait) */
#define CTH_CMD_SET_PAPERSIZE  0x0F     /* set papersize */
#define CTH_CMD_GET_CONFIG     0x10     /* get current copy settings */
#define CTH_CMD_SET_DEFAULTS   0x11     /* set factory default copy settings */

/* acl_cth_resize.resize_type Not using the copyapp_api.h values so we can
 * hopefully keep copytohost product independent; different products could have
 * non-intersecting sets of resize settings.
 */
#define CTH_RESIZE_TYPE_NULL 0  /* uninitialized/null - not used */
#define CTH_RESIZE_TYPE_ORIGINAL 1  /* 100% (i.e., no scaling) */
#define CTH_RESIZE_TYPE_4UP 2
#define CTH_RESIZE_TYPE_2UP 3
#define CTH_RESIZE_TYPE_A4_TO_LETTER 4
#define CTH_RESIZE_TYPE_LETTER_TO_A4 5
#define CTH_RESIZE_TYPE_FULL 6
#define CTH_RESIZE_TYPE_CUSTOM 7
#define CTH_RESIZE_TYPE_LGL_TO_LETTER 9
#define CTH_RESIZE_TYPE_LGL_TO_A4 10

/* acl_cth_quality.quality Not using the copyapp_api.h values so we can
 * hopefully keep copytohost product independent; different products could have
 * non-intersecting sets of quality settings.
 */
#define CTH_COPY_QUALITY_NULL 0 /* uninintialized/null - not used */
#define CTH_COPY_QUALITY_DRAFT 1
#define CTH_COPY_QUALITY_TEXT 2
#define CTH_COPY_QUALITY_MIXED 3
#define CTH_COPY_QUALITY_FILM 4
#define CTH_COPY_QUALITY_PICTURE 5
#define CTH_COPY_QUALITY_FAX_TEXT 6
#define CTH_COPY_QUALITY_FAX_MIXED 7

#pragma pack(1)

/* cth_header used in sending the data stream back to host */
typedef struct 
{
    uint32_t cookie;
    uint32_t msg;       /* CTH_HEADER_xxx */
    uint32_t reserved1; /* for future expansion */
    uint32_t reserved2; /* for future expansion */
    uint32_t seqnum;    /* good for tracking usb weirdness */
} cth_header;

typedef struct
{
    uint32_t pixels_per_line;   ///< Number of pixels /line
    uint32_t num_lines;     ///< Number of lines in this data block
    uint16_t bpp;            ///< The number of bits/pixel
    uint16_t last;           ///< set to 1 on the final strip
    char pad[32];
} print_data_info_t;

/* acl_cth_xxx are the payloads of the ACL CTH packet */
struct acl_cth 
{
    uint8_t cmd;
    uint8_t arg; /* misc; depends on cmd */
    uint8_t payload[10];
};

struct acl_cth_resize
{
    uint8_t cmd;
    uint8_t resize_type;
    uint16_t custom_percent;
    uint8_t padding[8];
};

struct acl_cth_user_input
{
    uint8_t cmd;
    /* nothing yet; could be key to hit, etc */
    uint8_t padding[11];
};

struct acl_cth_quality
{
    uint8_t cmd;
    uint8_t quality; /* draft, text, mixed, etc */
    uint8_t ccmode;  /* mono or color */
    uint8_t payload[9];
};

struct acl_cth_tint
{
    uint8_t cmd;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t padding[8];
};

#pragma pack()

//
// The align 8 below is because some of the function we could call use UINT64's.
//
static uint8_t CTHStack[CTH_STACK_SIZE] __attribute__ ((aligned (8)));
static mqd_t CTHParseQ;
//static MESSAGE CTHMess[NUM_CTH_MESSAGES];
static pthread_t CTHParser;
static IOReg *myPipe=NULL;
static uint8_t s_copy_to_host_job_active=0;

static error_type_t AclReadCopyJob(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void * cmd_data);
static void acl_turn_on_image_dump(IOReg *Pipe);

void copyapp_init_settings( CopyConf *conf );

static void swap_print_data( print_data_info_t *pd )
{
    pd->pixels_per_line = asp_htonl( pd->pixels_per_line );
    pd->num_lines = asp_htonl( pd->num_lines );
    pd->bpp = asp_htons( pd->bpp );
    pd->last = asp_htons( pd->last );
}

static void swap_cth_header( cth_header *hdr )
{
    hdr->cookie = asp_htonl( hdr->cookie );
    hdr->msg = asp_htonl( hdr->msg );
    hdr->reserved1 = asp_htonl( hdr->reserved1 );
    hdr->reserved2 = asp_htonl( hdr->reserved2 );
    hdr->seqnum = asp_htonl( hdr->seqnum );
}

static void swap_acl_cth_resize( struct acl_cth_resize *cth_resize )
{
    cth_resize->custom_percent = asp_htons( cth_resize->custom_percent );
}

static void cth_header_init( cth_header *hdr )
{
    static uint32_t seqnum=0;

    memset( hdr, 0, sizeof(cth_header) );
    hdr->cookie = CTH_HEADER_COOKIE;
    hdr->seqnum = ++seqnum;
}

static scan_err_t send_header( IOReg *pipe, uint32_t msg )
{
    cth_header *hdr;
    int retcode;

    cth_dbg2( "%s msg=%d\n", __FUNCTION__, msg );

    hdr = MEM_MALLOC_ALIGN( sizeof(cth_header), cpu_get_dcache_line_size() );
    if( hdr==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    cth_header_init( hdr );
    hdr->msg = msg;

    swap_cth_header( hdr );

    retcode = pipe->WriteConnect( pipe, (void *)hdr, sizeof(cth_header) );

    return SCANERR_NONE;
}
//
// \brief Print to host parser entry
//
static void* CTHParserEntry(void* input)
{
    MESSAGE nextMsg;   
    PLANED *plane;
    CURRENT_RESOURCE  *OurResource = 0;
    print_data_info_t *print_data;
    char *empty_data;
    char fillchar;
    PAGE_DATA  *page=0;
    UINT posix_retcode;
    UINT32 datalen;
    scan_err_t scerr;

    SysWaitForInit ();
    register_acl_cmd(ACLCMD_READ_COPY_JOB, AclReadCopyJob, 0);
    dbg2("CTHParseEntry thread now running...\n");

    while (1)
    {
        posix_retcode = posix_wait_for_message(CTHParseQ, (char*)&nextMsg,  sizeof(MESSAGE), 
                                       POSIX_WAIT_FOREVER);     // get a message
        XASSERT( posix_retcode==0, posix_retcode );

        cth_dbg2( "%s msgType=%#x %#x %#x %#x\n", __FUNCTION__, nextMsg.msgType, 
                    nextMsg.param1, nextMsg.param2, nextMsg.param3 );

        switch (nextMsg.msgType)
        {
            case MSG_RESOURCES:
                {
                    dbg2("%s MSG_RESOURCES\n", __FUNCTION__);
                    OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
                    dbg2("Srce ModID:%d, Dest ModID:%d\n", OurResource->Source, OurResource->Destination);
                    s_copy_to_host_job_active = 1;
                    page = 0;
                    break;
                }
            case MSG_JOBSTART:
                {
                    dbg2("%s MSG_JOBSTART\n", __FUNCTION__);
//                    MESSAGE msg;
//                    msg.msgType = MSG_ENGINE_MEDIA_DETECTED;
//                    SYMsgSend(OurResource->Source, &msg);
                    ASSERT(myPipe != NULL);
                    scerr = send_header( myPipe, CTH_HEADER_START_JOB );

                    if(OurResource && OurResource->Destination)
                        SYMsgSend(OurResource->Destination, &nextMsg);
                    break;
                }
            case MSG_JOBPAGESTART:
                {   
                    dbg2("%s MSG_JOBPAGESTART\n", __FUNCTION__);
                    scerr = send_header( myPipe, CTH_HEADER_START_PAGE );
                    page = (PAGE_DATA*)nextMsg.param3;
                    if(OurResource && OurResource->Destination)
                        SYMsgSend(OurResource->Destination, &nextMsg);
                    else
                        MEM_FREE_AND_NULL(nextMsg.param3);
                    break;

                }
            case MSG_JOBPAGEEND:
                {   
                    dbg2("%s MSG_JOBPAGEEND\n", __FUNCTION__);

                    scerr = send_header( myPipe, CTH_HEADER_END_PAGE );

                    if(OurResource && OurResource->Destination)
                        SYMsgSend(OurResource->Destination, &nextMsg);
                    break;
                }
            case MSG_JOBSHEETSTART:
                {   
                    dbg2("%s MSG_JOBSHEETSTART\n", __FUNCTION__);

                    scerr = send_header( myPipe, CTH_HEADER_START_SHEET );
                    if(OurResource && OurResource->Destination)
                        SYMsgSend(OurResource->Destination, &nextMsg);
                    break;
                }
            case MSG_JOBEND:                
                {   
                    dbg2("%s MSG_JOBEND\n", __FUNCTION__);
                    scerr = send_header( myPipe, CTH_HEADER_END_JOB );
                    if(OurResource && OurResource->Destination)
                        SYMsgSend(OurResource->Destination, &nextMsg);
                    nextMsg.msgType = MSG_FREERECIPE;
                    nextMsg.param3 = OurResource;
                    SYMsgSend (SJMID, &nextMsg);
                    myPipe = NULL;      // turn this off if it was on.
                    s_copy_to_host_job_active = 0;
                    break;
                }
            case MSG_JOBPLANEDATA:
                {
                    cth_dbg2("%s MSG_JOBPLANEDATA\n", __FUNCTION__);
                    plane = (PLANED *)nextMsg.param3;
                    // if I need to send back to host do that here.
                    if(myPipe != NULL)
                    {
                        cth_dbg2("plane: image_width_pixels=%d, image_width_bytes=%d, image_height_lines, bpp=%d\n",
                                    plane->image_info.image_width_in_pixels, plane->image_info.image_data_width_in_bytes, 
                                    plane->image_info.image_height_in_lines, plane->image_info.image_bpp );

                        // write out the header.
                        ASSERT(page != 0);
                        XASSERT(page->image_output_info.image_width_in_pixels > 0, page->image_output_info.image_width_in_pixels );
                        XASSERT(plane->image_info.image_bpp > 0, plane->image_info.image_bpp );

                        scerr = send_header( myPipe, CTH_HEADER_DATA );
                        //
                        // Build a print_data_info structure and fill it in.
                        //
                        print_data = MEM_MALLOC_ALIGN(sizeof(print_data_info_t),
                                                        cpu_get_dcache_line_size());
                        ASSERT(print_data != NULL);
                        memset(print_data, 0, sizeof(print_data_info_t));

                        print_data->num_lines = plane->image_info.image_height_in_lines;
                        print_data->pixels_per_line = plane->image_info.image_width_in_pixels;
                        print_data->bpp = plane->image_info.image_bpp;
                        print_data->last = plane->LastData;
  
                        datalen = print_data->pixels_per_line * 
                              print_data->num_lines * print_data->bpp / 8;

                        /* swap protocol fields to network byte order */
                        swap_print_data( print_data );

                        cth_dbg2("plane: image_width_pixels=%d, image_width_bytes=%d, image_height_lines=%d, bpp=%d, dl=%d\n",
                                    plane->image_info.image_width_in_pixels, plane->image_info.image_data_width_in_bytes, 
                                    plane->image_info.image_height_in_lines, plane->image_info.image_bpp, datalen );

                        // write out the header.
                        myPipe->WriteConnect(myPipe, print_data, sizeof(print_data_info_t));
                        if(plane->big_buffer == NULL)   // empty strip
                        {
                            empty_data = MEM_MALLOC_ALIGN(plane->DataLength,
                                                          cpu_get_dcache_line_size());
                            ASSERT(empty_data);
                            fillchar = 0x80;
                            
                            memset(empty_data, fillchar, plane->DataLength);

                            // send the empty strip to the host.
                            myPipe->WriteConnect(myPipe, empty_data, datalen);
                        } 
                        else
                        {
                            // write out the data.
                           myPipe->WriteConnect(myPipe, 
                                    plane->big_buffer->data, datalen);
                        }
                    }

                    if(OurResource && OurResource->Destination != INVALID_MODULE_ID)
                    {
                        SYMsgSend(OurResource->Destination, &nextMsg);
                    }
                    else 
                    {
                        // Nothing to send, just free everything.
                        if(plane->big_buffer != NULL)
                        {
                            if(!myPipe)
                            {
//                                cth_dbg2("Free data\n");
                                MEM_FREE_AND_NULL(plane->big_buffer->data);
                             }
//                             cth_dbg2("Free big_buffer\n");
                             MEM_FREE_AND_NULL(plane->big_buffer);
                        }
//                        cth_dbg2("Free param3\n");
                        MEM_FREE_AND_NULL(nextMsg.param3);
//                        cth_dbg2("Free done\n");
                    }
                    break;
                }
                default:
                 dbg2("%s unknown MSG: %d\n", __FUNCTION__, nextMsg.msgType);
                //DPRINTF(DBG_LOUD|DBG_OUTPUT,("don't understand\n");
                break;
        }
    }
    return 0;
}

uint8_t copy_to_host_job_active()
{
    return s_copy_to_host_job_active;
}

//
// \brief called by the acl cmd to establish a pipe for sending data to the host.
//

static void acl_turn_on_image_dump(IOReg *Pipe)
{
    myPipe = Pipe;      // turn on the image dump next time.
}

static error_type_t start_copy_job( void )
{
    STARTRECIPE *StartIt;
    MESSAGE msg;
    error_type_t err;

    err = 0;

    // initiate a copy
    StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
    ASSERT(StartIt != NULL);

    // we have a connection, get the resources and start.
    //
    memset( &msg, 0, sizeof(MESSAGE) );
    msg.msgType = MSG_STARTRECIPE;
    StartIt->Job = ejob_CopyToHost;
    StartIt->AdditionalResource = 0;
    StartIt->Pipe = 0;
    StartIt->Wait = e_WaitForever;
    msg.param3 = StartIt;       // freed by sjm
  
    // send a message to the System Job Manager telling it to 
    // kick off a copy
    //
    err = SYMsgSend(SJMID, &msg);         // send the message.
    XASSERT( err==OK, err );

    return err;
}

static error_type_t parse_cth_copy_quality( struct acl_cth_quality *cth_quality )
{
    e_COPYQUALITY new_quality=e_MIXED;
    copy_cmode_t new_ccmode=COPY_CMODE_MONO;

    if( cth_quality->ccmode != COPY_CMODE_MONO && cth_quality->ccmode != COPY_CMODE_COLOR ) {
        dbg2( "%s bad copy cmode=%d\n", __FUNCTION__, cth_quality->ccmode );
        return FAIL;
    }

    new_ccmode = (copy_cmode_t)cth_quality->ccmode;

    switch( cth_quality->quality ) {
        case CTH_COPY_QUALITY_DRAFT :
            new_quality = e_DRAFT;
            break;

        case CTH_COPY_QUALITY_TEXT :
            new_quality = e_TEXT;
            break;

        case CTH_COPY_QUALITY_MIXED :
            new_quality = e_MIXED;
            break;

        case CTH_COPY_QUALITY_FILM :
            new_quality = e_FILM_PHOTO;
            break;

        case CTH_COPY_QUALITY_PICTURE :
            new_quality = e_PICTURE;
            break;

        case CTH_COPY_QUALITY_FAX_TEXT :
            new_quality = e_FAX_TEXT;
            break;

        case CTH_COPY_QUALITY_FAX_MIXED :
            new_quality = e_FAX_MIXED;
            break;

        default:
            dbg2( "%s unknown cth quality=%d\n", __FUNCTION__, cth_quality->quality );
            return FAIL;
    }

    dbg2( "%s quality=%d\n", __FUNCTION__, new_quality );

    if( new_quality < e_COPYQUALITY_MAX ) 
    {
        copyapp_set_fp_qualitymode( new_quality );
        copyapp_set_fp_colormode( new_ccmode );

        dbg2( "%s %d %d %d\n", __FUNCTION__, __LINE__, new_quality, new_ccmode );

    }

    return 0;
}

static error_type_t parse_cth_num_copies( struct acl_cth *cth_cmd )
{
    int new_num_copies=1;

    new_num_copies = cth_cmd->arg;

    dbg2( "%s %d\n", __FUNCTION__, new_num_copies );

    if( new_num_copies >= 1 && new_num_copies <= 99 ) {
        copyapp_set_fp_num_copies( new_num_copies );
    }

    return 0;
}

static error_type_t parse_cth_contrast( struct acl_cth *cth_cmd )
{
    int new_contrast=6;

    new_contrast = cth_cmd->arg;

    dbg2( "%s contrast=%d\n", __FUNCTION__, new_contrast );

    if( new_contrast<COPY_CONTRAST_MIN || new_contrast>COPY_CONTRAST_MAX ) {
        return FAIL;
    }

    copyapp_set_fp_contrast( new_contrast );

    return 0;
}

static error_type_t parse_cth_colormode( struct acl_cth *cth_cmd )
{
    int new_colormode=6;

    new_colormode = cth_cmd->arg;

    dbg2( "%s colormode=%d\n", __FUNCTION__, new_colormode );

    if( new_colormode<1 || new_colormode>2 ) {
        return FAIL;
    }

    // new_colormode == 1 => Mono
    // new_colormode == 2 => Color
    copyapp_set_fp_colormode( new_colormode );

    return 0;
}

static error_type_t parse_cth_orientation( struct acl_cth *cth_cmd )
{
    int new_orientation=6;

    new_orientation = cth_cmd->arg;

    dbg2( "%s nup orientation=%d\n", __FUNCTION__, new_orientation );

    if( new_orientation<0 || new_orientation>1 ) {
        return FAIL;
    }

    // new_orientation == 0 => Portrait
    // new_orientation == 1 => Landscape
    copyapp_set_fp_orientation( new_orientation );

    return 0;
}

static error_type_t parse_cth_background( struct acl_cth *cth_cmd )
{
    int new_background=6;

    new_background = cth_cmd->arg;

    dbg2( "%s background=%d\n", __FUNCTION__, new_background );

    if( new_background<COPY_BACKGROUND_MIN || new_background>COPY_BACKGROUND_MAX ) {
        return FAIL;
    }

    copyapp_set_fp_background( new_background );

    return 0;
}

static error_type_t parse_cth_sharpness( struct acl_cth *cth_cmd )
{
    int new_sharpness=6;

    new_sharpness = cth_cmd->arg;

    dbg2( "%s sharpness=%d\n", __FUNCTION__, new_sharpness );

    if( new_sharpness<COPY_SHARP_MIN || new_sharpness>COPY_SHARP_MAX ) {
        return FAIL;
    }

    copyapp_set_fp_sharpness( new_sharpness );

    return 0;
}

static error_type_t parse_cth_brightness( struct acl_cth *cth_cmd )
{
    int new_brightness=6;

    new_brightness = cth_cmd->arg;

    dbg2( "%s bright=%d\n", __FUNCTION__, new_brightness );

    if( new_brightness<COPY_BRIGHTNESS_MIN || new_brightness>COPY_BRIGHTNESS_MAX ) {
        return FAIL;
    }

    copyapp_set_fp_brightness( new_brightness );

    return 0;
}

static error_type_t parse_cth_copy_resize( struct acl_cth_resize *cmd_resize )
{
    UINT32 custom_resize=100;
    e_COPYRESIZE resize_var;

    swap_acl_cth_resize( cmd_resize );

    dbg2( "%s resize=%d\n", cmd_resize->resize_type );

    // resize if requested
    //  resize options:
    //      e_ORG_100PCNT,
    //      e_A4_2_LTR_94PCNT,
    //      e_LTR_2_A4_97PCNT,
    //      e_FULL_PAGE_91PCNT,
    //      e_TWO_PAGES_PER_SHEET, 
    //      e_FOUR_PAGES_PER_SHEET,
    //      e_CUSTOM_PCNT,
    //      e_NoResize

    resize_var = e_ORG_100PCNT;

    switch( cmd_resize->resize_type ) {
        case CTH_RESIZE_TYPE_ORIGINAL :
            resize_var = e_ORG_100PCNT;
            break;

        case CTH_RESIZE_TYPE_4UP :
            resize_var = e_FOUR_PAGES_PER_SHEET;
            break;

        case CTH_RESIZE_TYPE_2UP :
            resize_var = e_TWO_PAGES_PER_SHEET;
            break;

        case CTH_RESIZE_TYPE_A4_TO_LETTER :
            resize_var = e_A4_2_LTR_94PCNT;
            break;

        case CTH_RESIZE_TYPE_LETTER_TO_A4 :
            resize_var = e_LTR_2_A4_97PCNT;
            break;

        case CTH_RESIZE_TYPE_FULL : 
            resize_var = e_FULL_PAGE_91PCNT;
            break;

        case CTH_RESIZE_TYPE_LGL_TO_LETTER:
            resize_var = e_LEGAL_2_LTR_78PCNT;
            break;

        case CTH_RESIZE_TYPE_LGL_TO_A4:
            resize_var = e_LEGAL_2_A4_83PCNT;
            break;

        case CTH_RESIZE_TYPE_CUSTOM :
            resize_var = e_CUSTOM_PCNT;
            custom_resize = cmd_resize->custom_percent; 
            break;
    }

    dbg2( "%s resize=%d custom=%d\n", __FUNCTION__, resize_var, custom_resize );
    copyapp_set_fp_resize( resize_var, custom_resize );

    return 0;
}

static error_type_t parse_cth_tint( struct acl_cth_tint *cth_tint )
{
    int r=0, g=0, b=0;

    r = cth_tint->r;
    g = cth_tint->g;
    b = cth_tint->b;

    dbg2( "%s r:%d g:%d b:%d\n", __FUNCTION__, r,g,b );

    if( r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255 ) 
    {
        copyapp_set_fp_color_balance( r, g, b );
    }

    return 0;
}

static error_type_t parse_cth_papersize( struct acl_cth *cth_cmd )
{
    mediasize_t pagesize = (mediasize_t)cth_cmd->arg;

    CopyConf conf;
    memset( &conf, 0, sizeof(conf) );
    copyapp_get_copy_config(&conf);
    conf.printPaperSize = pagesize;
    copyapp_set_copy_config(&conf);    

    return 0;
}

static error_type_t parse_cth_setdefaults( struct acl_cth *cth_cmd )
{
    CopyConf conf;
    copyapp_init_settings(&conf);
    copyapp_set_copy_config(&conf);    

    return 0;
}

static error_type_t parse_cth_get_config( struct acl_cth *cth_cmd )
{
    CopyConf *conf;
    conf = MEM_MALLOC_ALIGN( sizeof(CopyConf), cpu_get_dcache_line_size() );
    dbg2( "%s \n", __FUNCTION__ );

    copyapp_get_copy_config(conf);
    myPipe->WriteConnect(myPipe, conf, sizeof(CopyConf));

    return 0;
}

static error_type_t user_input( struct acl_cth_user_input *cmd_input )
{
    MESSAGE msg;
    error_type_t err;

    memset( &msg, 0, sizeof(MESSAGE) );
    msg.msgType = MSG_USER_INPUT;

    msg.param1 = PARAM_USER_OK;
    /* rest of params unused */
    msg.param2 = 0;
    msg.param3 = 0;

    err = SYMsgSend( COPYAPPID, &msg );
    XASSERT( err==OK, err );

    return err;
}

static error_type_t parse_cth_message( uint8_t msgbuf[12] )
{
    error_type_t err;
    struct acl_cth *cth;

    cth= (struct acl_cth *)msgbuf;
    dbg2( "%s cmd=%#x\n", __FUNCTION__, cth->cmd );

    switch( cth->cmd ) {
        case CTH_CMD_START :
            err = start_copy_job();
            break;

        case CTH_CMD_SET_RESIZE : 
            err = parse_cth_copy_resize( (struct acl_cth_resize *)msgbuf );
            break;

        case CTH_CMD_USER_INPUT :
            err = user_input( (struct acl_cth_user_input *)msgbuf );
            break;

        case CTH_CMD_SET_QUALITY :
            err = parse_cth_copy_quality( (struct acl_cth_quality *)msgbuf );
            break;

        case CTH_CMD_SET_NUM_COPIES :
            err = parse_cth_num_copies( (struct acl_cth *)msgbuf );
            break;

        case CTH_CMD_SET_BRIGHTNESS :
            err = parse_cth_brightness( (struct acl_cth *)msgbuf );
            break;

        case CTH_CMD_SET_TINT :
            err = parse_cth_tint( (struct acl_cth_tint *)msgbuf );
            break;

        case CTH_CMD_SET_COLORMODE :
            err = parse_cth_colormode( (struct acl_cth *)msgbuf );
            break;

        case CTH_CMD_SET_CONTRAST :
            err = parse_cth_contrast( (struct acl_cth *)msgbuf );
            break;

        case CTH_CMD_SET_SHARP :
            err = parse_cth_sharpness( (struct acl_cth *)msgbuf );
            break;

        case CTH_CMD_SET_BACKGROUND :
            err = parse_cth_background( (struct acl_cth *)msgbuf );
            break;

        case CTH_CMD_SET_ORIENT :
            err = parse_cth_orientation( (struct acl_cth *)msgbuf );
            break;
            
        case CTH_CMD_SET_PAPERSIZE :
            err = parse_cth_papersize( (struct acl_cth *)msgbuf );
            break;
            
        case CTH_CMD_SET_DEFAULTS:
            err = parse_cth_setdefaults( (struct acl_cth *)msgbuf );
            break;
            
        case CTH_CMD_GET_CONFIG :
            err = parse_cth_get_config( (struct acl_cth *)msgbuf );
            break;
            
        #ifdef HAVE_ID_COPY
        case CTH_CMD_IDCOPY_START :
            copyapp_id_copy_start_job( );
            break;

        case CTH_CMD_IDCOPY_NEXT :
            copyapp_id_copy_input( 0 );
            break;

        case CTH_CMD_IDCOPY_FINISH :
            copyapp_id_copy_input( 1 );
            break;
        #endif

        default : 
            /* wha? */
            dbg2( "%s unknown/unhandled cmd=%#x\n", __FUNCTION__, cth->cmd );
            err = FAIL;
            break;
    }

    dbg2( "%s err=%d\n", __FUNCTION__, err );

    return err;
}


// \brief 
// 
// \param[in] Pipe  IO Pipe for reporting results to the host
// 
// \param[in] AclCmd The command block that caused this to be called.
// 
// \param reserved Save for later
// 
// \retval int32_t 0 success, 1 failure.
// 
//
static error_type_t AclReadCopyJob(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void * cmd_data)
{
    error_type_t err;
    acl_response_base_struct_t *Buffer;

    dbg2("%s\n", __FUNCTION__);

    Buffer = (acl_response_base_struct_t *) construct_acl_response_buffer(ACLCMD_READ_COPY_JOB);
    // construct_acl_response_buffer should never return NULL
    ASSERT( NULL != Buffer );
    acl_turn_on_image_dump(Pipe);
    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);

    scanlog_hex_dump( (unsigned char *)AclCmd, 16 );

    err = parse_cth_message( AclCmd->temp );

    return err;
}

void cth_init(void)
{
    uint32_t status;

    /* sanity check our ACL protocol message structures */
    XASSERT( sizeof(struct acl_cth)==12, sizeof(struct acl_cth) );
    XASSERT( sizeof(struct acl_cth_resize)==12, sizeof(struct acl_cth_resize) );
    XASSERT( sizeof(struct acl_cth_user_input)==12, sizeof(struct acl_cth_user_input) );
    XASSERT( sizeof(struct acl_cth_quality)==12, sizeof(struct acl_cth_quality) );

    /* create the queue */
    posix_create_message_queue( &CTHParseQ, "/CTH", NUM_CTH_MESSAGES, sizeof(MESSAGE));   

    // Register resource
    rm_register(e_CopyToHostParser, COPY_TO_HOST_ID);    
    router_register_queue(COPY_TO_HOST_ID, CTHParseQ);

    status = posix_create_thread( &CTHParser, &CTHParserEntry, 0, "CTHPar", CTHStack,
                     CTH_STACK_SIZE, POSIX_THR_PRI_NORMAL );
    XASSERT(status==0, status);
}

