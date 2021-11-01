/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <libusb.h>
#include <getopt.h>

#include "err.h"
#include "log.h"
#include "str.h"
#include "asp.h"
#include "cmdline.h"
#include "xassert.h"
#include "prod.h"

/*
 * davep 15-Oct-2010 ; 1.9.0 added support for RGB packed mode. Still needs
 * work. Doesn't automatically byte-swap the incoming data.
 *
 * davep 17-Dec-2010 v1.9.2 - htonl() buffers of rgbx, xrgb, rgb
 *                      remove a debug message that was causing USB overflows       
 *
 * davep 23-Dec-2010 v1.9.3 - add a simple state machine in the ASP protocol
 *                      handler; used to catch firmware protocol errors
 *
 * davep 03-May-2011 v1.9.4 - add --htonl long opt to force yes/no of htonl
 *                      incoming data; switched to getopt_long()
 *
 * davep 17-Jun-2011 v1.9.5 - default resolution to 300
 *
 * davep 20-Jun-2011 v1.9.6 - write pnm header alongside data; add htons() for
 *                      16-bit pixels
 *
 * davep 20-Jul-2011 v1.9.7 - add document source (adf, flatbed, etc) to command line 
 *
 * davep 26-Jul-2011 v1.9.8 - add nocal and nopie
 *
 * davep 01-Sep-2011 v2.0.0 - whoa! version 2!? and fixed adf page number
 *                      filename problem
 *
 * davep 08-Sep-2011 v2.0.1 - add initial duplex adf support
 *
 * davep 24-Oct-2011 ; v2.0.5 - restore Brightness and Contrast to command line args
 *
 * davep 19-Dec-2011 ; v2.0.6 - adding document content via --content 
 *
 * (add version log here)
 */

/* version string displayed in the usage */
#define PROGRAM_VERSION  "2.0.6"

#define UNUSED_PARAMETER(x) (x)=(x)

#define INVALID_ENDPOINT  0xffff

///* must have -1 as the last resolution as end of list */
//uint32_t valid_resolution[] = { 75, 150, 300, 600, 1200, 2400, -1 };

struct keyvalue {
    const char *key;
    int keylen;
    uint32_t value;
};

const struct keyvalue valid_data_type[] = {
    /* (color and planar are the same thing) */
    { "mono",   4, SCAN_DATA_MONO },
    { "gray",   4, SCAN_DATA_MONO },
    { "color",  5, SCAN_DATA_PLANAR },
    { "planar", 6, SCAN_DATA_PLANAR },
    { "xrgb",   4, SCAN_DATA_XRGB_PACKED }, 
    { "rgbx",   4, SCAN_DATA_RGBX_PACKED }, 
    { "rgb",    3, SCAN_DATA_RGB_PACKED }, 
    { "bilevel", 7, SCAN_DATA_BILEVEL },

    /* copy's data type is handled as a special case */
    { "copy",   4, -1 },

    { NULL , 0, -1 }
};

const struct keyvalue valid_copy_quality[] = {
    { "text",  4, COPY_QUALITY_TEXT },
    { "draft", 5, COPY_QUALITY_DRAFT },
    { "mixed", 5, COPY_QUALITY_MIXED },
    { "film",  4, COPY_QUALITY_FILM_PHOTO },
    { "picture", 7, COPY_QUALITY_PICTURE },
    { "color", 5, COPY_QUALITY_COLORCOPY },
    { "faxtext", 7, COPY_QUALITY_FAX_TEXT },
    { "faxmixed", 8, COPY_QUALITY_FAX_MIXED },
    { NULL , 0, -1 }
};

void print_option( int idx, struct cmdline_option *opt )
{
    dbg1( "idx=%d key=%d num32=%d numer=%d denom=%d buf=%p buflen=%d\n", 
                idx, opt->key, opt->num32, opt->numerator, opt->denominator,
                opt->buf, opt->buflen );
}

void print_option_list( struct cmdline_option_list *option_list ) 
{
    int i;

    for( i=0 ; i<option_list->num_options ; i++ ) {
        print_option( i, &option_list->options[i] );
    }
}

static int find_key_value( const struct keyvalue kvlist[], 
                           const char *str, 
                           int len, 
                           uint32_t *num32 )
{
    int i;
    const struct keyvalue *kv;

    *num32 = -1;
    i = 0;

    /* linear search through our list until we find one we like */
    while( kvlist[i].key != NULL ) {
        kv = &kvlist[i];
        if( len==kv->keylen && str_match( str, kv->key, kv->keylen) ) {
            *num32 = kv->value;
            return 0;
        }
        i++;
    }

    return ERR_FAIL;
}


static int parse_ulong( const char *str, int len, uint32_t *num )
{
    char *endptr;

    /* I'm requiring hex numbers to start with "0x". strtoul() treats the 0x as
     * optional which I find confusing when I screw up and type a bad number.
     */
    if( len>=3 && str[0]=='0' && (str[1]=='x' || str[1]=='X')) {
        /* try converting hex */
        *num = strtoul( str, &endptr, 16 );
    }
    else {
        /* convert decimal */
        *num = strtoul( str, &endptr, 10 );
    }

    if( *endptr != 0 ) {
        /* parse error */
        return ERR_FAIL;
    }

    return 0;
}

static int parse_fraction( const char *str, uint32_t *numer, uint32_t *denom )
{
    int retcode;
    int len;
    char *ptrlist[20];
    char *copy_of_str;

    /* str_split() is destructive so use a copy */
    copy_of_str = strdup( str );
    if( copy_of_str == NULL ) {
        errorlog( "out of memory\n" );
        return ERR_OUT_OF_MEMORY;
    }

    len = str_split( copy_of_str, "/", ptrlist, 20 );
    
    /* need to find exactly ONE separator in there */
    if( len != 2 ) {
        errorlog( "Invalid fraction string \"%s\". Expect string like \"1/3\"\n.", str );
        goto fail;
    }

    retcode = parse_ulong( ptrlist[0], strlen(ptrlist[0]), numer );
    if( retcode != 0 ) {
        errorlog( "Invalid integer \"%s\" for numerator.\n", ptrlist[0] );
        goto fail;
    }

    retcode = parse_ulong( ptrlist[1], strlen(ptrlist[1]), denom );
    if( retcode != 0 ) {
        errorlog( "Invalid integer \"%s\" for denominator\n.", ptrlist[1] );
        goto fail;
    }

    return 0;

fail:
    free( copy_of_str );
    return ERR_FAIL;
}

static int parse_resolution( const char *str, int len, uint32_t *num32 )
{
    int retcode;
//    int i;

    *num32 = -1;
    retcode = parse_ulong( str, len, num32 );
    if( retcode < 0 ) {
        errorlog( "Invalid integer \"%s\" for resolution.\n", str );
        return ERR_FAIL;
    }

    /* make sure we have a valid allowable value */
#if 0
    i = 0;
    while( valid_resolution[i] != -1 ) {
        if( *num32==valid_resolution[i] ) {
            break;
        }
        i++;
    }

    if( valid_resolution[i] == -1 ) {
        errorlog( "Unknown or unsupported scan resolution \"%d\".\n", *num32 );
        return ERR_FAIL;
    }
#endif
    /* davep 19-Dec-05 ; we support all sorts of crazy resolutions now. Just
     * check for max and min 
     */
//    if( *num32<75 || *num32>2400 ) {
//        errorlog( "Unknown or unsupported scan resolution \"%d\".\n", *num32 );
//        return ERR_FAIL;
//    }

    return 0;
}

static int parse_data_type( const char *str, int len, uint32_t *num32 ) 
{
    int retcode;

    retcode = find_key_value( valid_data_type, str, len, num32 );

    if( retcode < 0 ) {
        errorlog( "Unknown or unsupported scan data type \"%s\".\n", str );
    }
    return retcode;
}

static int parse_area( const char *str, int len, struct scan_dimensions **pparea )
{
    struct scan_dimensions *area_ptr;
    int retcode;
    int i;
    char *ptrlist[20];
    char *copy_of_str;
    uint32_t num32[4];

    UNUSED_PARAMETER(len);

    *pparea = NULL;

    area_ptr = (struct scan_dimensions *)malloc(sizeof(struct scan_dimensions));
    if( area_ptr==NULL ) {
        errorlog( "out of memory\n" );
        return ERR_OUT_OF_MEMORY;
    }

    /* str_split() is destructive so use a copy */
    copy_of_str = strdup( str );
    if( copy_of_str == NULL ) {
        errorlog( "out of memory\n" );
        free( area_ptr );
        return ERR_OUT_OF_MEMORY;
    }

    /* note the space in the token list passed to str_split() */
    retcode = str_split( copy_of_str, ", ", ptrlist, 20 );
    if( retcode != 4 ) {
        errorlog( "Invalid area \"%s\". Expect area with four integers like \"0,0,850,1100\"\n.",
                str );
        goto fail;
    }
    
    for( i=0 ; i<4 ; i++ ) {
        retcode = parse_ulong( ptrlist[i], strlen(ptrlist[i]), &num32[i] );
        if( retcode != 0 ) {
            errorlog( "Invalid integer \"%s\" in area\n.", ptrlist[i] );
            goto fail;
        }
    }

    /* My command line wants X0,Y0,X1,Y1 but ASP wants "top,left,bottom.right"
     * so need to do a conversion: top=y0, left=x0, bottom=y1, right=x1 
     */
    area_ptr->top = num32[1];
    area_ptr->left = num32[0];
    area_ptr->bottom = num32[3];
    area_ptr->right = num32[2];

    *pparea = area_ptr;

    free( copy_of_str );
    return 0;
fail:
    free( area_ptr );
    free( copy_of_str );
    return ERR_FAIL;
}

static int parse_endpoints( const char *str, int len, uint32_t *out, uint32_t *in )
{
    int retcode;
    int numStr;
    int i;
    char *ptrlist[20];
    char *copy_of_str;
    uint32_t num32[2];

    UNUSED_PARAMETER(len);

    *out     = INVALID_ENDPOINT;
    *in      = INVALID_ENDPOINT;
    num32[0] = INVALID_ENDPOINT;
    num32[1] = INVALID_ENDPOINT;

    /* str_split() is destructive so use a copy */
    copy_of_str = strdup( str );
    if( copy_of_str == NULL ) {
        errorlog( "out of memory\n" );
        return ERR_OUT_OF_MEMORY;
    }

    /* note the space in the token list passed to str_split() */
    numStr = str_split( copy_of_str, ", ", ptrlist, 20 );
    if( numStr != 1 && numStr != 2 ) {
        errorlog( "Invalid endpoints \"%s\". Expect one or two endpoints like \"0x2,0x82\"\n", str );
        goto fail;
    }
    
    for( i=0 ; i<numStr ; i++ ) {
        retcode = parse_ulong( ptrlist[i], strlen(ptrlist[i]), &num32[i] );
        if( retcode != 0 ) {
            errorlog( "Invalid integer \"%s\" in endpoint\n", ptrlist[i] );
            goto fail;
        }
    }

    /* Validate the in endpoint direction bit */
    if (num32[1] != INVALID_ENDPOINT)
    {
        if ((num32[1] & 0x80) == 0)
        {
            errorlog( "Invalid direction bit in in endpoint (bit 0x80 must be set)\n" );
            goto fail;
        }
    }

    /* Return parsed results */
    *out = num32[0];
    *in  = num32[1];

    free( copy_of_str );
    return 0;
fail:
    free( copy_of_str );
    return ERR_FAIL;
}

static int parse_copy_quality( const char *str, int len, uint32_t *num32 ) 
{
    int retcode;

    retcode = find_key_value( valid_copy_quality, str, len, num32 );
    if( retcode < 0 ) {
        errorlog( "Unknown or unsupported copy quality type \"%s\".\n", str );
    }
    return retcode;
}

void usage( const char *progname )
{
    errorlog( "%s %s - %s\n", progname, PROGRAM_VERSION, __DATE__ );
    errorlog( "Capture scan data from Marvell-based USB scanners.\n" );
    errorlog( "usage: \n" );

    errorlog( "  -h ; this help text\n" );
    errorlog( "  -d num ; set log level to 'num' (higher numbers mean more messages)\n" );
    errorlog( "  -r resolution ; valid values are in range [75, 2400] (default 300)\n" );
    errorlog( "  -t data_type  ; valid values are rgb, mono, color, planar, xrgb, rgbx, copy, bilevel\n" );
    errorlog( "                (planar is another name for color)\n" );
    errorlog( "                (the copy option captures copy path data)\n" );
    errorlog( "                (bilevel is 8-bpp mono dithered to 0 or 255)\n" );

    errorlog( "  -b bpp ; bits per pixel (8 or 16; default=8)\n" );
    errorlog( "  -x xscale-numerator/xscale-denominator ; e.g. 1/2\n" );
    errorlog( "  -y yscale-numerator/yscale-denominator\n" );

    errorlog( "  -o outputfile\n" );
    errorlog( "     for rgb data_type, will create outputfile.rgb\n" );
    errorlog( "     for xrgb data_type, will create outputfile.xrgb\n" );
    errorlog( "     for rgbx data_type, will create outputfile.rgbx\n" );
    errorlog( "     for color data_type, will create outputfile.r,outputfile.g,outputfile.b\n" );
    errorlog( "     for mono data_type, will create outputfile.gray\n" );

    errorlog( "  -a x0,y0,x1,y1\n" );
    errorlog( "     specify area to scan; values are in 1/100ths of an inch.\n");
    errorlog( "     x0,y0 is upper-left and x1,y1 is lower right\n" );
    errorlog( "     for example, -a 0,0,850,1100 will scan a full 8.5\"x11\" sheet.\n" );
    errorlog( "                  -a 100,100,300,300 will scan a 200x200 hinches area at 100,100.\n" );
    errorlog( "     note either there can be no spaces between the numbers or\n" );
    errorlog( "        surround the numbers with quotes; e.g., -a \"0, 0, 850, 1100\"\n" );

    errorlog( "  -z fileid\n" );
    errorlog( "     write final image x,y size to file id (great for scripts)\n" );
    errorlog( "     for example, -z 6 will write stats to file handle 6\n" );

    errorlog( "  -C contrast\n" );
    errorlog( "     valid values are [0,200] where 100 is no contrast change\n" );
    errorlog( "     number is a float [0.0,2.0] * 100. Value divided by 100 in firmware.\n" );
    errorlog( "     <100 reduces contrast, 100 is no change, >100 increases contrast.\n" );

    errorlog( "  -B brightness\n" );
    errorlog( "     valid values are [0,200] where 100 is no brightness change\n" );
    errorlog( "     number is a float [0.0,2.0] * 100. Value divided by 100 in firmware.\n" );
    errorlog( "     <100 reduces brightness, 100 is no change, >100 increases brightness.\n" );

    errorlog( "  -G gamma\n" );
    errorlog( "     valid values are 10..100; value is divided by 10 in the firmware\n" );
    errorlog( "     for example, -G 20 is gamma of 2.0\n" );

    errorlog( "  -S smooth/sharpen\n" );
    errorlog( "     valid values are [0,200] where 100 is no sharpen/smooth change\n" );
    errorlog( "     number is a float [0.0,2.0] * 100. Value divided by 100 in firmware.\n" );
    errorlog( "     <100 smooths, 100 is no change, >100 sharpens\n" );

    errorlog( "  -q quality\n" );
    errorlog( "     set copy quality (only valid for copy-to-host)\n" );
    errorlog( "     valid values are: text, draft, mixed, film, color, faxtext, faxmixed\n" );
    errorlog( "     if this option is not present, firmware will use the control panel's copy quality\n" );

    errorlog( "  -T seconds\n" );
    errorlog( "     set device timeout to \"seconds\"\n" );

    errorlog( "  -v num\n" );
    errorlog( "     USB Vendor ID (default is %#06x)\n", SCANNER_VENDOR_ID );
    errorlog( "  -p num\n" );
    errorlog( "     USB Product ID (default is %#06x)\n", SCANNER_PRODUCT_ID );
    errorlog( "  -i num\n" );
    errorlog( "     USB Interface (default is %d)\n", SCAN_INTERFACE );
    errorlog( "  -e out[,in]\n" );
    errorlog( "     USB Out,In Endpoints (default is 0x%x,0x%x)\n", SCAN_OUT_ENDPOINT, SCAN_IN_ENDPOINT );
    errorlog( "     the out endpoint must always be provided, the in endpoint is optional.\n" );
    errorlog( "     note direction is with respect to the host: out is host to device (usb write)\n" );

    errorlog( "  -c ; do cal-to-host instead of scan\n" );
    errorlog( "     Perform a \"cal-to-host\" instead of a real scan. The data returned\n" );
    errorlog( "     will be the data gathered during the calibration instead of scan data.\n" ); 

    errorlog( "  -s serialnum\n" );
    errorlog( "     decide between devices with the same vid and pid using this USB serial number\n" );

    errorlog( "  -n ; add page numbers to scan files (useful with ADFs); default is no page number\n" );

    errorlog( "  --docsrc [auto|flatbed|adf|adfduplex] ; force document source\n" );
    errorlog( "     default is to allow the firmware select best document source\n" );

    errorlog( "  --nocal ; disable calibration for this scan\n" );
    errorlog( "  --nopie ; disable all image enhancements for this scan\n" );

    errorlog( "  --content [auto|photo|mixed|text] ; original page content\n" );
    errorlog( "     default is to allow the firmware select best settings (auto)\n" );
}

struct cmdline_option * lookup_option( struct cmdline_option_list *option_list, int key )
{
    int i;

    for( i=0 ; i<option_list->num_options ; i++ ) {
        if( option_list->options[i].key == key ) {
            return &option_list->options[i];
        }
    }

    return NULL;
}

void free_options( struct cmdline_option_list *option_list )
{
    int i;

    for( i=0 ; i<option_list->num_options ; i++ ) {
        if( option_list->options[i].buf != NULL ) {
            free( option_list->options[i].buf );
            option_list->options[i].buf = NULL;
            option_list->options[i].buflen = 0;
        }
    }
}

static void maybe_tweak_copy_settings( struct cmdline_option_list *option_list )
{
    struct cmdline_option *opt;

    opt = lookup_option( option_list, OPT_SCAN_TYPE );
    
    /* if we don't have that option or we're not doing a copy-to-host, bail 
     * now
     */
    if( !opt || opt->num32 != SCAN_TYPE_SCAN_THRU_COPY ) {
        /* not doing a copy, nothing to do */
        return;
    }

    opt = lookup_option( option_list, OPT_COPY_QUALITY );
    if( opt && opt->num32 == COPY_QUALITY_COLORCOPY ) {
        /* set the output type to xRGB */
        opt = lookup_option( option_list, OPT_DATA_TYPE );

        /* the way I'm parsing the command line automatically adds a
         * OPT_DATA_TYPE option so if I don't find it, something has gone wrong
         * somewhere.
         */
        ASSERT( opt!=NULL );
        opt->num32 = SCAN_DATA_XRGB_PACKED;

        /* NOTE! There is still a potential to get the filename wrong. The
         * quality option is, well, optional. The firmware defaults to the
         * control panel settings if no quality is sent in the job settings. As
         * of this writing (14-Sep-06), the firmware's default is one of the
         * mono HalfPack settings and our default in parse_args() matches
         * that. If the firmware's default changes to something else, lacking
         * any better information, we'll save the data to the .gray extension.
         *
         * TODO a better way to name the files would be to wait for the data to
         * come back from the firmware then, using the
         * scan_data_header.data_type field, open the appropriate files.  
         */
    }
}

static int parse_long_option( struct cmdline_option_list *option_list, 
                              const char *long_opt_name, 
                              const char *long_opt_value )
{
    int final_retcode;
    struct cmdline_option *opt;

//    dbg2( "%s %s %s\n", __FUNCTION__, long_opt_name, long_opt_value );
    
    final_retcode = 0;
    opt = &option_list->options[ option_list->num_options ];

    if( str_match( long_opt_name, "htonl", 5 ) ) {
        if( !long_opt_value ) {
            errorlog( "htonl option needs an argument\n" );
            final_retcode = -1;
            goto leave;
        }
    
        if( str_match( long_opt_value, "no", 2 ) || str_match( long_opt_value, "0", 1 ) ) {
            opt->key = OPT_FORCE_HTONL_DATA;
            opt->num32 = 0;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "yes", 3 ) || str_match( long_opt_value, "1", 1 ) ) {
            opt->key = OPT_FORCE_HTONL_DATA;
            opt->num32 = 1;
            option_list->num_options++;
        }
        else {
            errorlog( "htonl argument needs \"yes\" or \"no\"\n" );
            final_retcode = -1;
        }
    }
    else if( str_match( long_opt_name, "htons", 5 ) ) {
        if( !long_opt_value ) {
            errorlog( "htons option needs an argument\n" );
            final_retcode = -1;
            goto leave;
        }
    
        if( str_match( long_opt_value, "no", 2 ) || str_match( long_opt_value, "0", 1 ) ) {
            opt->key = OPT_FORCE_HTONS_DATA;
            opt->num32 = 0;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "yes", 3 ) || str_match( long_opt_value, "1", 1 ) ) {
            opt->key = OPT_FORCE_HTONS_DATA;
            opt->num32 = 1;
            option_list->num_options++;
        }
        else {
            errorlog( "htons argument needs \"yes\" or \"no\"\n" );
            final_retcode = -1;
        }
    }
    else if( str_match( long_opt_name, "docsrc", 6 ) ) {
        if( !long_opt_value ) {
            errorlog( "docsrc option needs an argument\n" );
            final_retcode = -1;
            goto leave;
        }
        if( str_match( long_opt_value, "auto", 4 ) ) {
            opt->key = OPT_DOCUMENT_SOURCE;
            opt->num32 = SCAN_JOB_SETTINGS_DOC_SRC_AUTO;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "flatbed", 7 ) ) {
            opt->key = OPT_DOCUMENT_SOURCE;
            opt->num32 = SCAN_JOB_SETTINGS_DOC_SRC_FLATBED;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "adf", 3 ) ) {
            opt->key = OPT_DOCUMENT_SOURCE;
            opt->num32 = SCAN_JOB_SETTINGS_DOC_SRC_ADF;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "adfduplex", 9 ) ) {
            opt->key = OPT_DOCUMENT_SOURCE;
            opt->num32 = SCAN_JOB_SETTINGS_DOC_SRC_ADF_DUPLEX;
            option_list->num_options++;
        }
        else {
            errorlog( "docsrc argument must be one of: auto, flatbed, adf, adfduplex\n" );
            final_retcode = -1;
        }
    }
    else if( str_match( long_opt_name, "nocal", 5 ) ) {
        opt->key = OPT_NO_CAL;
        opt->num32 = 1;
        option_list->num_options++;
    }
    else if( str_match( long_opt_name, "nopie", 5 ) ) {
        opt->key = OPT_NO_PIE;
        opt->num32 = 1;
        option_list->num_options++;
    }
    else if( str_match( long_opt_name, "content", 7 ) ) {
        if( !long_opt_value ) {
            errorlog( "content option needs an argument\n" );
            final_retcode = -1;
            goto leave;
        }
        if( str_match( long_opt_value, "auto", 4 ) ) {
            opt->key = OPT_ORIGINAL_CONTENT;
            opt->num32 = SCAN_JOB_FLAGS_ORIGINAL_CONTENT_AUTO;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "photo", 4 ) ) {
            opt->key = OPT_ORIGINAL_CONTENT;
            opt->num32 = SCAN_JOB_FLAGS_ORIGINAL_CONTENT_PHOTO;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "mixed", 5 ) ) {
            opt->key = OPT_ORIGINAL_CONTENT;
            opt->num32 = SCAN_JOB_FLAGS_ORIGINAL_CONTENT_MIXED;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "text", 4 ) ) {
            opt->key = OPT_ORIGINAL_CONTENT;
            opt->num32 = SCAN_JOB_FLAGS_ORIGINAL_CONTENT_TEXT;
            option_list->num_options++;
        }
        else {
            errorlog( "content argument must be one of: auto, photo, mixed, text\n" );
            final_retcode = -1;
        }
    }


leave:
    return final_retcode;
}

static void push_num_option( struct cmdline_option_list *option_list,
                             int key, uint32_t value )
{
    struct cmdline_option *opt;

    opt = &option_list->options[ option_list->num_options ];

    opt->key = key;
    opt->num32 = value;

    option_list->num_options++;
}

int parse_args( int argc, char *argv[], struct cmdline_option_list *option_list )
{
    int retcode;
    int c;
    uint32_t num32, numer, denom, data_type;
    uint32_t out, in;
    int found_outputfile, found_datatype;
    struct scan_dimensions *area_ptr;
    static struct option long_options[] = {
        { "htonl", 1, 0, 0 },
        { "htons", 1, 0, 0 },
        { "docsrc", 1, 0, 0 },
        { "nocal", 0, 0, 0 },
        { "nopie", 0, 0, 0 },
        { "content", 1, 0, 0 },
        { 0, 0, 0, 0 },
    };
    int long_index;
    struct cmdline_option *opt;

    memset( option_list, 0, sizeof(option_list) );

    if( argc==1 ) {
        usage( argv[0] );
        return ERR_FAIL;
    }

    /* require a few parameters to be set */
    found_outputfile = 0;
    found_datatype = 0;

    while( 1 ) {
        c = getopt_long( argc, argv, 
                        "hd:r:t:x:y:o:a:z:C:G:B:S:q:T:v:p:cs:ni:e:b:", 
                        long_options, &long_index );
        if( c==-1 ) {
            break;
        }

        /* if we're still parsing options by the time we've filling up the
         * array, bail out with an error 
         */
        if( option_list->num_options >= MAX_CMDLINE_OPTIONS ) {
            errorlog( "internal error; too many options\n" );
            goto fail;
        }

        opt = &option_list->options[ option_list->num_options ];

        switch( c ) {
            case 0 : 
                retcode = parse_long_option( option_list, long_options[long_index].name, optarg );
                if( retcode != 0 ) {
                    /* parse_long_option() logs error */
                    goto fail;
                }
                break;

            case 'h' :
                usage( argv[0] );
                goto fail;
                break;

            case 'd' : 
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode != 0 ) {
                    errorlog( "Invalid integer \"%s\" for log level.\n", optarg );
                    goto fail;
                }
                log_set_level( num32 );
                break;

            case 'a' :
                retcode = parse_area( optarg, strlen(optarg), &area_ptr );
                if( retcode < 0 ) {
                    /* parse_area() logs error */
                    goto fail;
                }
                opt->key = OPT_AREA;
                opt->buf = (void *)area_ptr;
                opt->buflen = 
                        sizeof(struct scan_dimensions);
                option_list->num_options += 1;
                break;

            case 'b' :
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode != 0 ) {
                    errorlog( "Invalid integer \"%s\" for bpp.\n", optarg );
                    goto fail;
                }
                if( num32 != 8 && num32 != 16 ) {
                    errorlog( "Only 8 and 16-bpp are supported.\n" );
                    goto fail;
                }
                
                push_num_option( option_list, OPT_BPP, num32 );
                break;

            case 'r' :
                retcode = parse_resolution( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_resolution() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_RESOLUTION, num32 );
                break;

            case 't' :

                retcode = parse_data_type( optarg, strlen(optarg), &data_type );
                if( retcode < 0 ) {
                    /* parse_type() logs error */
                    goto fail;
                }

                /* make sure we have enough space for two options */
                if( option_list->num_options+2 >= MAX_CMDLINE_OPTIONS ) {
                    errorlog( "internal error; too many options\n" );
                    goto fail;
                }

                found_datatype = 1;

                /* 13-Sep-06 davep ; adding copy-to-host (scanning through the
                 * copy path in the firmware; e.g., receive the halfpack data)
                 */
                if( str_match( optarg, "copy", 4 ) ) {
                    push_num_option( option_list, OPT_SCAN_TYPE, SCAN_TYPE_SCAN_THRU_COPY );

                    /* the actual data type we get back will depend on what
                     * sort of copy we do; e.g., a film quality copy will
                     * return 4bpp HalfPack data which I'll treat as mono and
                     * save to a .gray file but a colorcopy will return xRGB
                     * which should be saved to a .xrgb file.
                     */
                    push_num_option( option_list, OPT_DATA_TYPE, SCAN_DATA_MONO );

                    /* stop here ; rest of this case is for regular
                     * scan-to-host 
                     */
                    break;
                }

                /* at this point, we have a regular color or mono scan */

                push_num_option( option_list, OPT_DATA_TYPE, data_type );

                /* davep 19-Jul-2013 ; adding bilevel (dithered 8-bpp mono) */
                if( data_type==SCAN_DATA_MONO || data_type==SCAN_DATA_BILEVEL ) {
                    push_num_option( option_list, OPT_SCAN_TYPE, SCAN_TYPE_MONO );
                }
                else {
                    push_num_option( option_list, OPT_SCAN_TYPE, SCAN_TYPE_COLOR );
                }
                break;

            case 'x' :
                retcode = parse_fraction( optarg, &numer, &denom );
                if( retcode < 0 ) {
                    errorlog( "Invalid fraction value.\n" );
                    goto fail;
                }
                opt->key = OPT_X_SCALE;
                opt->numerator = numer;
                opt->denominator = denom;
                option_list->num_options += 1;
                break;

            case 'y' :
                retcode = parse_fraction( optarg, &numer, &denom );
                if( retcode < 0 ) {
                    errorlog( "Invalid fraction value.\n" );
                    goto fail;
                }
                opt->key = OPT_Y_SCALE;
                opt->numerator = numer;
                opt->denominator = denom;
                option_list->num_options += 1;
                break;

            case 'o' :
                /* output filename */
                opt->key = OPT_OUTFILENAME;
                opt->buf = strdup( optarg );
                if( opt->buf == NULL ) {
                    errorlog( "out of memory for filename\n" );
                    goto fail;
                }
                opt->buflen = strlen( opt->buf );

                /* because I'm appending things to the end of the filename, enforce a
                 * shorter filename than the system max
                 */
                if( opt->buflen > MAX_OUTPUT_FILENAME ) {
                    errorlog( "filename too long; max is %d\n", MAX_OUTPUT_FILENAME );
                    goto fail;
                }
    
                option_list->num_options += 1;
                found_outputfile = 1;
                break;

            case 'z' :
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_ulong() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_STAT_HANDLE, num32 );
                break;


            case 'C' :
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_ulong() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_CONTRAST, num32 );
                break;

            case 'B' :
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_ulong() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_BRIGHTNESS, num32 );
                break;

            case 'G' :
                /* Gamma */
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_ulong() logs error */
                    goto fail;
                }
                if( num32<10 || num32>100 ) {
                    errorlog( "gamma must be in range [10,100]\n" );
                    goto fail;
                }
                push_num_option( option_list, OPT_GAMMA, num32 );
                break;

            case 'S' :
                /* smooth/sharpen */
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_ulong() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_SHARPNESS, num32 );
                break;

            case 'q' :
                /* Copy Quality */
                retcode = parse_copy_quality( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_copy_quality() logs error */
                    goto fail;
                }
                XASSERT( num32 <= COPY_QUALITY_LAST, num32 );
                push_num_option( option_list, OPT_COPY_QUALITY, num32 );
                break;

            case 'T' :
                /* Timeout */
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_resolution() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_SCAN_TIMEOUT, num32 );
                break;

            case 'v' :
                /* USB Vendor ID (vid) */
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_resolution() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_USB_VID, num32 );
                break;

            case 'p' :
                /* USB Product ID (pid) */
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_resolution() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_USB_PID, num32 );
                break;
                
            case 'i' :
                /* scan's USB interface */
                retcode = parse_ulong( optarg, strlen(optarg), &num32 );
                if( retcode < 0 ) {
                    /* parse_ulong() logs error */
                    goto fail;
                }
                push_num_option( option_list, OPT_USB_INTERFACE, num32 );
                break;

            case 'e' :
                /* scan's USB endpoints */
                retcode = parse_endpoints( optarg, strlen(optarg), &out, &in );
                if( retcode < 0 ) {
                    /* parse_endpoints() logs error */
                    goto fail;
                }
                /* Kind of faking it here: using two OPT's for something that really
                 * comes in as one option.  out will always be set, in is optional. 
                 */
                push_num_option( option_list, OPT_USB_OUT_ENDPOINT, out );
                if (in != INVALID_ENDPOINT)
                {
                    push_num_option( option_list, OPT_USB_IN_ENDPOINT, in );
                }
                break;

            case 'c' :
                /* no command line options */
                push_num_option( option_list, OPT_CAL_TO_HOST, 1 );
                break;
            
            case 's' :
                /* USB Serial Number */
                opt->key = OPT_SERIAL_NUM;
                opt->buf = strdup( optarg );
                if( opt->buf == NULL ) {
                    errorlog( "out of memory for usb serial number\n" );
                    goto fail;
                }
                opt->buflen = strlen( opt->buf );
                option_list->num_options += 1;
                break;

            case 'n' :
                /* boolean flag to add page numbers to the incoming scan files */
                push_num_option( option_list, OPT_PAGE_NUMBERS, 1 );
                break;

            default:
//                errorlog( "Unknown command line option \"%s\".\n", optarg);
                errorlog( "Unknown command line option.\n" );
                errorlog( "Run with \"-h\" to get usage.\n" );
                goto fail;
        }
    }

    /* check for a few required fields */
    if( !found_outputfile ) {
        errorlog( "Please specify a destination file for the data "
                  "with the \"-o\" option.\n" );
        goto fail;
    }

    if( !found_datatype ) {
        errorlog( "Please specify a data type for the scan"
                  "with the \"-t\" option.\n" );
        goto fail;
    }

    /* davep 17-Jun-2011 ; default to 300 dpi if not specified */
    opt = lookup_option( option_list, OPT_RESOLUTION );
    if( opt==NULL ) {
        push_num_option( option_list, OPT_RESOLUTION, 300 );
    }

    /* When we're doing copy-to-host, there are a few settings that interlock.
     * Need to change those here. For example, if we're doing a copy-to-host
     * with quality "text", the output data type will be mono. But if we're
     * doing, a copy-to-host of "color", the output data type will be mono. 
     */ 
    maybe_tweak_copy_settings( option_list );

    print_option_list( option_list );

    return 0;

fail: 
    /* free any memory we might have allocated in the options list */
    free_options( option_list );
    return ERR_FAIL;
}

