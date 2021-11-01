/*
 * ============================================================================
 * Copyright (c) 2011,2013 Marvell International, Ltd. All Rights Reserved
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
#include <getopt.h>

#include "lassert.h"
#include "utils.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanlog.h"
#include "scanvars.h"
#include "aspscan/include/cmdline.h"
#include "asp.h"

/* version string displayed in the usage */
#define PROGRAM_VERSION  "1.0.0"

#define UNUSED_PARAMETER(x) (x)=(x)

///* must have -1 as the last resolution as end of list */
//uint32_t valid_resolution[] = { 75, 150, 300, 600, 1200, 2400, -1 };

struct keyvalue {
    const char *key;
    int keylen;
    uint32_t value;
};

const struct keyvalue valid_data_type[] = {
    /* (color and planar are the same thing) */
    { "mono",   4, SCAN_DATA_TYPE_MONO },
    { "gray",   4, SCAN_DATA_TYPE_MONO },
    { "color",  5, SCAN_DATA_TYPE_PLANE },
    { "planar", 6, SCAN_DATA_TYPE_PLANE },
    { "xrgb",   4, SCAN_DATA_TYPE_XRGB}, 
    { "rgbx",   4, SCAN_DATA_TYPE_RGBX}, 
    { "rgb",    3, SCAN_DATA_TYPE_RGB}, 
    { "bilevel", 7, SCAN_DATA_TYPE_BILEVEL },

    /* copy's data type is handled as a special case */
    { "copy",   4, -1 },

    { NULL , 0, -1 }
};

const struct keyvalue valid_copy_quality[] = {
//    { "text",  4, COPY_QUALITY_TEXT },
//    { "draft", 5, COPY_QUALITY_DRAFT },
//    { "mixed", 5, COPY_QUALITY_MIXED },
//    { "film",  4, COPY_QUALITY_FILM_PHOTO },
//    { "picture", 7, COPY_QUALITY_PICTURE },
//    { "color", 5, COPY_QUALITY_COLORCOPY },
//    { "faxtext", 7, COPY_QUALITY_FAX_TEXT },
//    { "faxmixed", 8, COPY_QUALITY_FAX_MIXED },
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

    return SCANERR_GENERIC_FAIL;
}


static scan_err_t parse_ulong( const char *str, int len, uint32_t *num )
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
        return SCANERR_INVALID_PARAM;
    }

    return 0;
}

static scan_err_t parse_fraction( const char *str, uint32_t *numer, uint32_t *denom )
{
    int retcode;
    int len;
    char *ptrlist[20];
    char *copy_of_str;

    /* str_split() is destructive so use a copy */
    copy_of_str = strdup( str );
    if( copy_of_str == NULL ) {
        dbg1( "out of memory\n" );
        return SCANERR_OUT_OF_MEMORY;
    }

    len = str_split( copy_of_str, strlen(copy_of_str), ptrlist, 20, "/" );
    
    /* need to find exactly ONE separator in there */
    if( len != 2 ) {
        dbg1( "Invalid fraction string \"%s\". Expect string like \"1/3\"\n.", str );
        goto fail;
    }

    retcode = parse_ulong( ptrlist[0], strlen(ptrlist[0]), numer );
    if( retcode != 0 ) {
        dbg1( "Invalid integer \"%s\" for numerator.\n", ptrlist[0] );
        goto fail;
    }

    retcode = parse_ulong( ptrlist[1], strlen(ptrlist[1]), denom );
    if( retcode != 0 ) {
        dbg1( "Invalid integer \"%s\" for denominator\n.", ptrlist[1] );
        goto fail;
    }

    return 0;

fail:
    free( copy_of_str );
    return SCANERR_INVALID_PARAM;
}

static int parse_resolution( const char *str, int len, uint32_t *num32 )
{
    int retcode;
//    int i;

    *num32 = -1;
    retcode = parse_ulong( str, len, num32 );
    if( retcode < 0 ) {
        dbg1( "Invalid integer \"%s\" for resolution.\n", str );
        return SCANERR_GENERIC_FAIL;
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
        dbg1( "Unknown or unsupported scan resolution \"%d\".\n", *num32 );
        return SCANERR_GENERIC_FAIL;
    }
#endif
    /* davep 19-Dec-05 ; we support all sorts of crazy resolutions now. Just
     * check for max and min 
     */
//    if( *num32<75 || *num32>2400 ) {
//        dbg1( "Unknown or unsupported scan resolution \"%d\".\n", *num32 );
//        return SCANERR_GENERIC_FAIL;
//    }

    return 0;
}

static int parse_data_type( const char *str, int len, uint32_t *num32 ) 
{
    int retcode;

    retcode = find_key_value( valid_data_type, str, len, num32 );

    if( retcode < 0 ) {
        dbg1( "Unknown or unsupported scan data type \"%s\".\n", str );
    }
    return retcode;
}

static scan_err_t parse_area( const char *str, int len, struct scan_area **pparea )
{
    scan_err_t scerr, final_scerr;
    struct scan_area *area_ptr;
    int retcode;
    int i;
    char *ptrlist[20];
    char *copy_of_str;
    uint32_t num32[4];
    int lower_right_x, lower_right_y;

    UNUSED_PARAMETER(len);

    *pparea = NULL;

    final_scerr = SCANERR_INVALID_PARAM;

    area_ptr = (struct scan_area *)malloc(sizeof(struct scan_area));
    if( area_ptr==NULL ) {
        dbg1( "out of memory\n" );
        return SCANERR_OUT_OF_MEMORY;
    }

    /* str_split() is destructive so use a copy */
    copy_of_str = strdup( str );
    if( copy_of_str == NULL ) {
        dbg1( "out of memory\n" );
        free( area_ptr );
        return SCANERR_OUT_OF_MEMORY;
    }

    /* note the space in the token list passed to str_split() */
    retcode = str_split( copy_of_str, strlen(copy_of_str), ptrlist, 20, ", ");
    if( retcode != 4 ) {
        dbg1( "Invalid area \"%s\". Expect area with four integers like \"0,0,850,1100\"\n.",
                str );
        goto fail;
    }
    
    for( i=0 ; i<4 ; i++ ) {
        scerr = parse_ulong( ptrlist[i], strlen(ptrlist[i]), &num32[i] );
        if( scerr != SCANERR_NONE ) {
            dbg1( "Invalid integer \"%s\" in area\n.", ptrlist[i] );
            final_scerr = scerr;
            goto fail;
        }
    }

    /* davep 19-Apr-2013 ; scan_area is upper-left, width, height. Aspscan
     * (which I want to remain compatible with) is upper-left, lower-right. So
     * convert the aspscan-style area to scan_area
     */
    area_ptr->x = num32[0];
    area_ptr->y = num32[1];
    lower_right_x = num32[2];
    lower_right_y = num32[3];

    if( lower_right_x < area_ptr->x ) {
        final_scerr = SCANERR_INVALID_PARAM;
        goto fail;
    }
    if( lower_right_y < area_ptr->y ) {
        final_scerr = SCANERR_INVALID_PARAM;
        goto fail;
    }

    area_ptr->width = lower_right_x - area_ptr->x;
    area_ptr->height = lower_right_y - area_ptr->y;

    *pparea = area_ptr;

    free( copy_of_str );
    return SCANERR_NONE;
fail:
    free( area_ptr );
    free( copy_of_str );
    return final_scerr;
}

#if 0
static int parse_copy_quality( const char *str, int len, uint32_t *num32 ) 
{
    int retcode;

    retcode = find_key_value( valid_copy_quality, str, len, num32 );
    if( retcode < 0 ) {
        dbg1( "Unknown or unsupported copy quality type \"%s\".\n", str );
    }
    return retcode;
}
#endif

void usage( const char *progname )
{
    dbg1( "%s %s - %s\n", progname, PROGRAM_VERSION, __DATE__ );
    dbg1( "Capture scan data from Marvell-based Linux firmware scanners.\n" );
    dbg1( "usage: \n" );

    dbg1( "  -h ; this help text\n" );
    dbg1( "  -d num ; set log level to 'num' (higher numbers mean more messages)\n" );
    dbg1( "  -r resolution ; valid values are in range [75, 2400] (default 300)\n" );
    dbg1( "  -t data_type  ; valid values are rgb, mono, color, planar, xrgb, rgbx, copy\n" );
    dbg1( "                (planar is another name for color)\n" );
    dbg1( "                (the copy option captures copy path data)\n" );

    dbg1( "  -b bpp ; bits per pixel (8 or 16; default=8)\n" );
    dbg1( "  -x xscale-numerator/xscale-denominator ; e.g. 1/2\n" );
    dbg1( "  -y yscale-numerator/yscale-denominator\n" );

    dbg1( "  -o outputfile\n" );
    dbg1( "     for rgb data_type, will create outputfile.ppm\n" );
//    dbg1( "     for xrgb data_type, will create outputfile.xrgb\n" );
//    dbg1( "     for rgbx data_type, will create outputfile.rgbx\n" );
//    dbg1( "     for color data_type, will create outputfile.r,outputfile.g,outputfile.b\n" );
    dbg1( "     for mono data_type, will create outputfile.pgm\n" );

    dbg1( "  -a x0,y0,x1,y1\n" );
    dbg1( "     specify area to scan; values are in 1/100ths of an inch.\n");
    dbg1( "     x0,y0 is upper-left and x1,y1 is lower right\n" );
    dbg1( "     for example, -a 0,0,850,1100 will scan a full 8.5\"x11\" sheet.\n" );
    dbg1( "                  -a 100,100,300,300 will scan a 200x200 hinches area at 100,100.\n" );
    dbg1( "     note either there can be no spaces between the numbers or\n" );
    dbg1( "        surround the numbers with quotes; e.g., -a \"0, 0, 850, 1100\"\n" );

    dbg1( "  -C contrast\n" );
    dbg1( "     valid values are [0,200] where 100 is no contrast change\n" );
    dbg1( "     number is a float [0.0,2.0] * 100. Value divided by 100 in firmware.\n" );
    dbg1( "     <100 reduces contrast, 100 is no change, >100 increases contrast.\n" );

    dbg1( "  -B brightness\n" );
    dbg1( "     valid values are [0,200] where 100 is no brightness change\n" );
    dbg1( "     number is a float [0.0,2.0] * 100. Value divided by 100 in firmware.\n" );
    dbg1( "     <100 reduces brightness, 100 is no change, >100 increases brightness.\n" );

    dbg1( "  -G gamma\n" );
    dbg1( "     valid values are 10..100; value is divided by 10 in the firmware\n" );
    dbg1( "     for example, -G 20 is gamma of 2.0\n" );

    dbg1( "  -S smooth/sharpen\n" );
    dbg1( "     valid values are [0,200] where 100 is no sharpen/smooth change\n" );
    dbg1( "     number is a float [0.0,2.0] * 100. Value divided by 100 in firmware.\n" );
    dbg1( "     <100 smooths, 100 is no change, >100 sharpens\n" );

//    dbg1( "  -q quality\n" );
//    dbg1( "     set copy quality (only valid for copy-to-host)\n" );
//    dbg1( "     valid values are: text, draft, mixed, film, color, faxtext, faxmixed\n" );
//    dbg1( "     if this option is not present, firmware will use the control panel's copy quality\n" );

//    dbg1( "  -T seconds\n" );
//    dbg1( "     set device timeout to \"seconds\"\n" );

    dbg1( "  -c ; do cal-to-host instead of scan\n" );
    dbg1( "     Perform a \"cal-to-host\" instead of a real scan. The data returned\n" );
    dbg1( "     will be the data gathered during the calibration instead of scan data.\n" ); 

    dbg1( "  -n ; add page numbers to scan files (useful with ADFs); default is no page number\n" );

    dbg1( "  --docsrc [auto|flatbed|adf|adfduplex] ; force document source\n" );
    dbg1( "     default is to allow the firmware select best document source\n" );

    dbg1( "  --nocal ; disable calibration for this scan\n" );
    dbg1( "  --nopie ; disable all image enhancements for this scan\n" );

    dbg1( "  --dump-siqfile ; dump the scanvar's siqfile and exit\n" );
    dbg1( "  --siqfile [filename] ; parse a siqfile and merge with instantiated scanvar\n" );

//    dbg1( "  --content [auto|photo|mixed|text] ; original page content\n" );
//    dbg1( "     default is to allow the firmware select best settings (auto)\n" );
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

#if 0
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
        opt->num32 = SCAN_DATA_TYPE_XRGB;

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
#endif

static scan_err_t parse_long_option( struct cmdline_option_list *option_list, 
                              const char *long_opt_name, 
                              const char *long_opt_value )
{
    scan_err_t final_scerr;
    struct cmdline_option *opt;

//    dbg2( "%s %s %s\n", __FUNCTION__, long_opt_name, long_opt_value );
    
    final_scerr = SCANERR_NONE;
    opt = &option_list->options[ option_list->num_options ];

    if( str_match( long_opt_name, "htonl", 5 ) ) {
        if( !long_opt_value ) {
            dbg1( "htonl option needs an argument\n" );
            final_scerr = SCANERR_INVALID_PARAM;
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
            dbg1( "htonl argument needs \"yes\" or \"no\"\n" );
            final_scerr = SCANERR_INVALID_PARAM;
        }
    }
    else if( str_match( long_opt_name, "htons", 5 ) ) {
        if( !long_opt_value ) {
            dbg1( "htons option needs an argument\n" );
            final_scerr = SCANERR_INVALID_PARAM;
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
            dbg1( "htons argument needs \"yes\" or \"no\"\n" );
            final_scerr = SCANERR_INVALID_PARAM;
        }
    }
    else if( str_match( long_opt_name, "docsrc", 6 ) ) {
        /* davep 02-Apr-2013 ; --docsrc in runscan is handled differently than
         * in aspscan. In aspscan, the parameter turns into an ASP #def
         * SCAN_JOB_SETTINGS_DOC_SRC_xxx (asp.h). Here, since we're eventually
         * writing these settings into a scanvar, the option is a
         * scan_document_source_t (scantypes.h). The command line option will be the
         * same so users won't know the difference.
         */
        if( !long_opt_value ) {
            dbg1( "docsrc option needs an argument\n" );
            final_scerr = SCANERR_INVALID_PARAM;
            goto leave;
        }
        if( str_match( long_opt_value, "auto", 4 ) ) {
            /* TODO query the ADF paper present state */
            final_scerr = SCANERR_NOT_IMPLEMENTED;
            goto leave;
        }
        else if( str_match( long_opt_value, "flatbed", 7 ) ) {
            opt->key = OPT_DOCUMENT_SOURCE;
            opt->num32 = (uint32_t)SCAN_DOCUMENT_SOURCE_FLATBED;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "adf", 3 ) ) {
            opt->key = OPT_DOCUMENT_SOURCE;
            opt->num32 = (uint32_t)SCAN_DOCUMENT_SOURCE_ADF;
            option_list->num_options++;
        }
        else if( str_match( long_opt_value, "adfduplex", 9 ) ) {
            opt->key = OPT_DOCUMENT_SOURCE;
            opt->num32 = (uint32_t)SCAN_DOCUMENT_SOURCE_ADF_DUPLEX;
            option_list->num_options++;
        }
        else {
            dbg1( "docsrc argument must be one of: auto, flatbed, adf, adfduplex\n" );
            final_scerr = SCANERR_INVALID_PARAM;
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
    else if( str_match( long_opt_name, "dump-siqfile", 12 ) ) {
        opt->key = OPT_DUMP_SIQFILE;
        opt->num32 = 1;
        option_list->num_options++;
    }
    else if( str_match( long_opt_name, "siqfile", 7 ) ) {
        opt->key = OPT_IMPORT_SIQFILE;
        opt->buf = strdup( optarg );
        if( opt->buf == NULL ) {
            final_scerr = SCANERR_OUT_OF_MEMORY;
            dbg1( "out of memory for filename\n" );
            goto leave;
        }
        option_list->num_options++;
    }
#if 0
    else if( str_match( long_opt_name, "content", 7 ) ) {
        if( !long_opt_value ) {
            dbg1( "content option needs an argument\n" );
            final_scerr = SCANERR_INVALID_PARAM;
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
            dbg1( "content argument must be one of: auto, photo, mixed, text\n" );
            final_scerr = SCANERR_INVALID_PARAM;
        }
    }
#endif

leave:
    return final_scerr;
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
    scan_err_t scerr, final_scerr;
    int c;
    uint32_t num32, numer, denom;
    int found_outputfile, found_datatype;
    struct scan_area *area_ptr;
    static struct option long_options[] = {
        { "htonl", 1, 0, 0 },
        { "htons", 1, 0, 0 },
        { "docsrc", 1, 0, 0 },
        { "nocal", 0, 0, 0 },
        { "nopie", 0, 0, 0 },
        { "content", 1, 0, 0 },
        { "dump-siqfile", 0, 0, 0 },
        { "siqfile", 1, 0, 0 },
        { 0, 0, 0, 0 },
    };
    int long_index;
    struct cmdline_option *opt;

    final_scerr = SCANERR_INVALID_PARAM;

    memset( option_list, 0, sizeof(option_list) );

    if( argc==1 ) {
        usage( argv[0] );
        return SCANERR_GENERIC_FAIL;
    }

    /* require a few parameters to be set */
    found_outputfile = 0;
    found_datatype = 0;

    while( 1 ) {
        c = getopt_long( argc, argv, 
                        "hd:r:t:x:y:o:a:z:C:G:B:S:cnb:", 
                        long_options, &long_index );
        if( c==-1 ) {
            break;
        }

        /* if we're still parsing options by the time we've filling up the
         * array, bail out with an error 
         */
        if( option_list->num_options >= MAX_CMDLINE_OPTIONS ) {
            dbg1( "internal error; too many options\n" );
            goto fail;
        }

        opt = &option_list->options[ option_list->num_options ];

        switch( c ) {
            case 0 : 
                scerr = parse_long_option( option_list, long_options[long_index].name, optarg );
                if( scerr != SCANERR_NONE ) {
                    /* parse_long_option() logs error */
                    final_scerr = scerr;
                    goto fail;
                }
                break;

            case 'h' :
                usage( argv[0] );
                goto fail;
                break;

            case 'd' : 
                scerr = parse_ulong( optarg, strlen(optarg), &num32 );
                if( scerr != SCANERR_NONE  ) {
                    dbg1( "Invalid integer \"%s\" for log level.\n", optarg );
                    final_scerr = scerr;
                    goto fail;
                }
                scanlog_set_level( num32 );
                break;

            case 'a' :
                scerr = parse_area( optarg, strlen(optarg), &area_ptr );
                if( scerr < 0 ) {
                    /* parse_area() logs error */
                    final_scerr = scerr;
                    goto fail;
                }
                opt->key = OPT_AREA;
                opt->buf = (void *)area_ptr;
                opt->buflen = sizeof(struct scan_area);
                option_list->num_options += 1;
                break;

            case 'b' :
                scerr = parse_ulong( optarg, strlen(optarg), &num32 );
                if( scerr != SCANERR_NONE ) {
                    dbg1( "Invalid integer \"%s\" for bpp.\n", optarg );
                    final_scerr = scerr;
                    goto fail;
                }
                if( num32 != 8 && num32 != 16 ) {
                    dbg1( "Only 8 and 16-bpp are supported.\n" );
                    final_scerr = SCANERR_INVALID_PARAM;;
                    goto fail;
                }
                
                push_num_option( option_list, OPT_BPP, num32 );
                break;

            case 'r' :
                scerr = parse_resolution( optarg, strlen(optarg), &num32 );
                if( scerr != SCANERR_NONE ) {
                    /* parse_resolution() logs error */
                    final_scerr = scerr;
                    goto fail;
                }
                push_num_option( option_list, OPT_RESOLUTION, num32 );
                break;

            case 't' :

                scerr = parse_data_type( optarg, strlen(optarg), &num32 );
                if( scerr != SCANERR_NONE ) {
                    /* parse_type() logs error */
                    final_scerr = scerr;
                    goto fail;
                }

                /* make sure we have enough space for two options */
                if( option_list->num_options+2 >= MAX_CMDLINE_OPTIONS ) {
                    dbg1( "internal error; too many options\n" );
                    final_scerr = SCANERR_OUT_OF_MEMORY;
                    goto fail;
                }

                found_datatype = 1;

                /* 13-Sep-06 davep ; adding copy-to-host (scanning through the
                 * copy path in the firmware; e.g., receive the halfpack data)
                 */
#if 0
                if( str_match( optarg, "copy", 4 ) ) {
                    push_num_option( option_list, OPT_SCAN_TYPE, SCAN_TYPE_SCAN_THRU_COPY );

                    /* the actual data type we get back will depend on what
                     * sort of copy we do; e.g., a film quality copy will
                     * return 4bpp HalfPack data which I'll treat as mono and
                     * save to a .gray file but a colorcopy will return xRGB
                     * which should be saved to a .xrgb file.
                     */
                    push_num_option( option_list, OPT_DATA_TYPE, SCAN_DATA_TYPE_MONO );

                    /* stop here ; rest of this case is for regular
                     * scan-to-host 
                     */
                    break;
                }
#endif

                /* at this point, we have a regular color or mono scan */

                push_num_option( option_list, OPT_DATA_TYPE, num32 );

                /* davep 22-Jul-2013 ; adding bilevel scanning (mono with
                 * dithering)
                 */
                if( num32==SCAN_DATA_TYPE_MONO || num32==SCAN_DATA_TYPE_BILEVEL ) {
                    push_num_option( option_list, OPT_SCAN_TYPE, SCAN_CMODE_MONO );
                }
                else {
                    push_num_option( option_list, OPT_SCAN_TYPE, SCAN_CMODE_COLOR );
                }
                break;

            case 'x' :
                scerr = parse_fraction( optarg, &numer, &denom );
                if( scerr != SCANERR_NONE ) {
                    dbg1( "Invalid fraction value.\n" );
                    final_scerr = scerr;
                    goto fail;
                }
                opt->key = OPT_X_SCALE;
                opt->numerator = numer;
                opt->denominator = denom;
                option_list->num_options += 1;
                break;

            case 'y' :
                scerr = parse_fraction( optarg, &numer, &denom );
                if( scerr != SCANERR_NONE ) {
                    dbg1( "Invalid fraction value.\n" );
                    final_scerr = scerr;
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
                    dbg1( "out of memory for filename\n" );
                    final_scerr = SCANERR_OUT_OF_MEMORY;
                    goto fail;
                }
                opt->buflen = strlen( opt->buf );

                /* because I'm appending things to the end of the filename, enforce a
                 * shorter filename than the system max
                 */
                if( opt->buflen > MAX_OUTPUT_FILENAME ) {
                    dbg1( "filename too long; max is %d\n", MAX_OUTPUT_FILENAME );
                    goto fail;
                }
    
                option_list->num_options += 1;
                found_outputfile = 1;
                break;

            case 'C' :
                scerr = parse_ulong( optarg, strlen(optarg), &num32 );
                if( scerr != SCANERR_NONE ) {
                    /* parse_ulong() logs error */
                    final_scerr = scerr;
                    goto fail;
                }
                push_num_option( option_list, OPT_CONTRAST, num32 );
                break;

            case 'B' :
                scerr = parse_ulong( optarg, strlen(optarg), &num32 );
                if( scerr != SCANERR_NONE ) {
                    /* parse_ulong() logs error */
                    final_scerr = scerr;
                    goto fail;
                }
                push_num_option( option_list, OPT_BRIGHTNESS, num32 );
                break;

            case 'G' :
                /* Gamma */
                scerr = parse_ulong( optarg, strlen(optarg), &num32 );
                if( scerr != SCANERR_NONE ) {
                    /* parse_ulong() logs error */
                    final_scerr = scerr;
                    goto fail;
                }
                if( num32<10 || num32>100 ) {
                    dbg1( "gamma must be in range [10,100]\n" );
                    goto fail;
                }
                push_num_option( option_list, OPT_GAMMA, num32 );
                break;

            case 'S' :
                /* smooth/sharpen */
                scerr = parse_ulong( optarg, strlen(optarg), &num32 );
                if( scerr != SCANERR_NONE ) {
                    /* parse_ulong() logs error */
                    final_scerr = scerr;
                    goto fail;
                }
                push_num_option( option_list, OPT_SHARPNESS, num32 );
                break;

            case 'c' :
                /* no command line options */
                push_num_option( option_list, OPT_CAL_TO_HOST, 1 );
                break;
            
            case 'n' :
                /* boolean flag to add page numbers to the incoming scan files */
                push_num_option( option_list, OPT_PAGE_NUMBERS, 1 );
                break;

            default:
//                dbg1( "Unknown command line option \"%s\".\n", optarg);
                dbg1( "Unknown command line option.\n" );
                dbg1( "Run with \"-h\" to get usage.\n" );
                goto fail;
        }
    }

    /* check for a few required fields */
    if( !found_outputfile ) {
        /* davep 26-Mar-2013 ; don't require an output file if we're doing a
         * siqfile dump (the -o option not required since we're not writing a
         * file)
         */
        opt = lookup_option( option_list, OPT_DUMP_SIQFILE );
        if( !opt ) {
            dbg1( "Please specify a destination file for the data "
                      "with the \"-o\" option.\n" );
            goto fail;
        }
    }

    if( !found_datatype ) {
        dbg1( "Please specify a data type for the scan with the \"-t\" option.\n" );
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
//    maybe_tweak_copy_settings( option_list );

//    print_option_list( option_list );

    return 0;

fail: 
    /* free any memory we might have allocated in the options list */
    free_options( option_list );
    return final_scerr;
}


