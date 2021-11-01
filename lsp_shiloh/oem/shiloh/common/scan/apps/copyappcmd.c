/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2008-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <string.h>
#include <stdlib.h>

// #include "tx_api.h"

#include "agRouter.h"
#include "ATypes.h"
#include "lassert.h"
#include "agJobUtils.h"
#include "utils.h"
#include "debug.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "data_access_api.h"
#include "error_types.h"
#include "response_subject.h"
#include "cmd_proc_api.h"
#include "scancore.h"
#include "scantypes.h"
#include "scanvars.h"
#include "scands.h"
#include "copyapp_api.h"
#include "copyapp.h"
#include "scanvars.h"
#include "print_job_api.h"
#include "printvars_api.h"
#include "copy_config.h"

#define CMD_FAIL "command failed"
#define CMD_PASS "ok"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"

static const char *copyapp_cmd_job_desc;
static const char *copyapp_cmd_job_usage;
static const char *copyapp_cmd_job_notes;
static int copyapp_cmd_job_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_cont_job_desc;
static const char *copyapp_cmd_cont_job_usage;
static const char *copyapp_cmd_cont_job_notes;
static int copyapp_cmd_cont_job_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_finish_desc;
static const char *copyapp_cmd_finish_usage;
static const char *copyapp_cmd_finish_notes;
static int copyapp_cmd_finish_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_set_quality_desc;
static const char *copyapp_cmd_set_quality_usage;
static const char *copyapp_cmd_set_quality_notes;
static int copyapp_cmd_set_quality_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_set_pquality_desc;
static const char *copyapp_cmd_set_pquality_usage;
static const char *copyapp_cmd_set_pquality_notes;
static int copyapp_cmd_set_pquality_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_resize_desc;
static const char *copyapp_cmd_resize_usage;
static const char *copyapp_cmd_resize_notes;
static int copyapp_cmd_resize_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_next_desc;
static const char *copyapp_cmd_next_usage;
static const char *copyapp_cmd_next_notes;
static int copyapp_cmd_next_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_cancel_desc;
static const char *copyapp_cmd_cancel_usage;
static const char *copyapp_cmd_cancel_notes;
static int copyapp_cmd_cancel_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_background_desc;
static const char *copyapp_cmd_background_usage;
static const char *copyapp_cmd_background_notes;
static int copyapp_cmd_background_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_brightness_desc;
static const char *copyapp_cmd_brightness_usage;
static const char *copyapp_cmd_brightness_notes;
static int copyapp_cmd_brightness_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_contrast_desc;
static const char *copyapp_cmd_contrast_usage;
static const char *copyapp_cmd_contrast_notes;
static int copyapp_cmd_contrast_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_sharpness_desc;
static const char *copyapp_cmd_sharpness_usage;
static const char *copyapp_cmd_sharpness_notes;
static int copyapp_cmd_sharpness_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_color_tint_desc;
static const char *copyapp_cmd_color_tint_usage;
static const char *copyapp_cmd_color_tint_notes;
static int copyapp_cmd_color_tint_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_collate_desc;
static const char *copyapp_cmd_collate_usage;
static const char *copyapp_cmd_collate_notes;
static int copyapp_cmd_collate_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_color_desc;
static const char *copyapp_cmd_color_usage;
static const char *copyapp_cmd_color_notes;
static int copyapp_cmd_color_cb(int argc, char *argv[]) ;

#ifdef HAVE_ID_COPY
static const char *copyapp_cmd_id_desc;
static const char *copyapp_cmd_id_usage;
static const char *copyapp_cmd_id_notes;
static int copyapp_cmd_id_cb(int argc, char *argv[]) ;
#endif

static const char *copyapp_cmd_numcopies_desc;
static const char *copyapp_cmd_numcopies_usage;
static const char *copyapp_cmd_numcopies_notes;
static int copyapp_cmd_numcopies_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_nvram_desc;
static const char *copyapp_cmd_nvram_usage;
static const char *copyapp_cmd_nvram_notes;
static int copyapp_cmd_nvram_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_papersize_desc;
static const char *copyapp_cmd_papersize_usage;
static const char *copyapp_cmd_papersize_notes;
static int copyapp_cmd_papersize_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_inputtray_desc;
static const char *copyapp_cmd_inputtray_usage;
static const char *copyapp_cmd_inputtray_notes;
static int copyapp_cmd_inputtray_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_orient_desc;
static const char *copyapp_cmd_orient_usage;
static const char *copyapp_cmd_orient_notes;
static int copyapp_cmd_orient_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_pduplex_desc;
static const char *copyapp_cmd_pduplex_usage;
static const char *copyapp_cmd_pduplex_notes;
static int copyapp_cmd_pduplex_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_sduplex_desc;
static const char *copyapp_cmd_sduplex_usage;
static const char *copyapp_cmd_sduplex_notes;
static int copyapp_cmd_sduplex_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_region_desc;
static const char *copyapp_cmd_region_usage;
static const char *copyapp_cmd_region_notes;
static int copyapp_cmd_region_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_log_desc;
static const char *copyapp_cmd_log_usage;
static const char *copyapp_cmd_log_notes;
static int copyapp_cmd_log_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_OOM_desc;
static const char *copyapp_cmd_OOM_usage;
static const char *copyapp_cmd_OOM_notes;
static int copyapp_cmd_OOM_cb(int argc, char *argv[]) ;

static const char *copyapp_cmd_resolution_desc;
static const char *copyapp_cmd_resolution_usage;
static const char *copyapp_cmd_resolution_notes;
static int copyapp_cmd_resolution_cb(int argc, char *argv[]);

 
/* command registration happens here */
void CopyAppCmdInit( void )
{
    int retcode;
    retcode = cmd_register_cmd( "copy", NULL, NULL, NULL, NULL, NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "job", copyapp_cmd_job_desc,
         copyapp_cmd_job_usage, copyapp_cmd_job_notes, copyapp_cmd_job_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "background",
	 copyapp_cmd_background_desc, copyapp_cmd_background_usage, 
	  copyapp_cmd_background_notes, copyapp_cmd_background_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "brightness",
          copyapp_cmd_brightness_desc, copyapp_cmd_brightness_usage, 
	  copyapp_cmd_brightness_notes, copyapp_cmd_brightness_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "cancel", 
	copyapp_cmd_cancel_desc, copyapp_cmd_cancel_usage,
        copyapp_cmd_cancel_notes, copyapp_cmd_cancel_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "finish", 
	 copyapp_cmd_finish_desc, copyapp_cmd_finish_usage,
	 copyapp_cmd_finish_notes, copyapp_cmd_finish_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "collate",
	 copyapp_cmd_collate_desc, copyapp_cmd_collate_usage,
	 copyapp_cmd_collate_notes, copyapp_cmd_collate_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "color", 
	 copyapp_cmd_color_desc, copyapp_cmd_color_usage,
	 copyapp_cmd_color_notes, copyapp_cmd_color_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "continuous",
	 copyapp_cmd_cont_job_desc, copyapp_cmd_cont_job_usage, 
         copyapp_cmd_cont_job_notes, copyapp_cmd_cont_job_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "contrast",
	 copyapp_cmd_contrast_desc, copyapp_cmd_contrast_usage, 
	  copyapp_cmd_contrast_notes, copyapp_cmd_contrast_cb );
    XASSERT( retcode==CMD_OK, retcode );
    #ifdef HAVE_ID_COPY
    retcode = cmd_register_subcmd( "copy", "id", copyapp_cmd_id_desc,
         copyapp_cmd_id_usage, copyapp_cmd_id_notes, copyapp_cmd_id_cb );
    XASSERT( retcode==CMD_OK, retcode );
    #endif
    retcode = cmd_register_subcmd( "copy", "inputtray",
	 copyapp_cmd_inputtray_desc, copyapp_cmd_inputtray_usage, 
	 copyapp_cmd_inputtray_notes, copyapp_cmd_inputtray_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "next",
	 copyapp_cmd_next_desc, copyapp_cmd_next_usage,
	 copyapp_cmd_next_notes, copyapp_cmd_next_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "numcopies",
	 copyapp_cmd_numcopies_desc, copyapp_cmd_numcopies_usage, 
	 copyapp_cmd_numcopies_notes, copyapp_cmd_numcopies_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "nvram", 
	copyapp_cmd_nvram_desc, copyapp_cmd_nvram_usage,
	 copyapp_cmd_nvram_notes, copyapp_cmd_nvram_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "orient", 
	copyapp_cmd_orient_desc, copyapp_cmd_orient_usage,
        copyapp_cmd_orient_notes, copyapp_cmd_orient_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "papersize",
	 copyapp_cmd_papersize_desc, copyapp_cmd_papersize_usage, 
	  copyapp_cmd_papersize_notes, copyapp_cmd_papersize_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "pduplex",
	 copyapp_cmd_pduplex_desc, copyapp_cmd_pduplex_usage,
	 copyapp_cmd_pduplex_notes, copyapp_cmd_pduplex_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "printqual",
	 copyapp_cmd_set_pquality_desc, copyapp_cmd_set_pquality_usage, 
	  copyapp_cmd_set_pquality_notes, copyapp_cmd_set_pquality_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "quality",
	 copyapp_cmd_set_quality_desc, copyapp_cmd_set_quality_usage,
	  copyapp_cmd_set_quality_notes, copyapp_cmd_set_quality_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "resize", 
	 copyapp_cmd_resize_desc, copyapp_cmd_resize_usage,
         copyapp_cmd_resize_notes, copyapp_cmd_resize_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "sduplex",
	 copyapp_cmd_sduplex_desc, copyapp_cmd_sduplex_usage,
         copyapp_cmd_sduplex_notes, copyapp_cmd_sduplex_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "sharpness",
	  copyapp_cmd_sharpness_desc, copyapp_cmd_sharpness_usage, 
	  copyapp_cmd_sharpness_notes, copyapp_cmd_sharpness_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "tint",
         copyapp_cmd_color_tint_desc, copyapp_cmd_color_tint_usage, 
	  copyapp_cmd_color_tint_notes, copyapp_cmd_color_tint_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "region", 
	copyapp_cmd_region_desc, copyapp_cmd_region_usage,
        copyapp_cmd_region_notes, copyapp_cmd_region_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "log", copyapp_cmd_log_desc,
	 copyapp_cmd_log_usage, copyapp_cmd_log_notes, copyapp_cmd_log_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "oom", copyapp_cmd_OOM_desc,
         copyapp_cmd_OOM_usage, copyapp_cmd_OOM_notes, copyapp_cmd_OOM_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "copy", "resolution", copyapp_cmd_resolution_desc,
         copyapp_cmd_resolution_usage, copyapp_cmd_resolution_notes, copyapp_cmd_resolution_cb);
    XASSERT( retcode==CMD_OK, retcode );
}

uint32_t cmd_result(uint32_t result)
{
    switch(result) 
    {
        case CMD_OK:
            cmd_append_result(CMD_PASS);
            break;
        case CMD_ERROR:
        case CMD_USAGE_ERROR:
            cmd_append_result(CMD_FAIL);
            break;
    }
    return result;
}

static const char *copyapp_cmd_job_desc = "Start the copy job";
static const char *copyapp_cmd_job_usage = NULL;
static const char *copyapp_cmd_job_notes = NULL;
static int copyapp_cmd_job_cb(int argc, char *argv[]) 
{ 
    if( 1!= argc) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    copyapp_start_job();

    cmd_printf("copy job started \n");

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_cont_job_desc = "";
static const char *copyapp_cmd_cont_job_usage = "start|stop [time in minutes]\n"
	"   Start or Stop continuous copy.\n"
	"   A continuous copy will run until stopped or time has expired.";
static const char *copyapp_cmd_cont_job_notes =  "";
static int copyapp_cmd_cont_job_cb(int argc, char *argv[])
{
    extern uint32_t g_contscan_time;

    if( argc == 2 ) 
    {
        if( str_match( argv[1], "start", 5 ) ) 
        {
            cmd_printf( "starting continuous copy...\n" );
	    g_contscan_time =0;  // run forever
            copy_continuous_start( );
        }
        else if( str_match( argv[1], "stop", 4 ) ) 
        {
            cmd_printf( "stoping continuous copy...\n" );
            copy_continuous_stop( );
        }
	else 
	{
	    return cmd_result(CMD_USAGE_ERROR);
	}
    }
    else if( argc == 3 ) 
    {
	if( str_mkint( argv[2], &g_contscan_time ) != 0 ) 
	{
            cmd_printf("bad time integer \"%s\"\n", argv[2]);
	    g_contscan_time =0;
	    return cmd_result(CMD_USAGE_ERROR);
	}
	cmd_printf("  continuous copy will be automatically stopped in %d minutes.\n", (int)g_contscan_time );
	copy_continuous_start( );
    }
    else 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    return cmd_result(CMD_OK);
}


static const char *copyapp_cmd_set_quality_desc = "set quality for the copy job";
static const char *copyapp_cmd_set_quality_usage = "draft|film|picture|mixed|text|faxtext|faxmixed";
static const char *copyapp_cmd_set_quality_notes = "";
static int copyapp_cmd_set_quality_cb(int argc, char *argv[])
{
    e_COPYQUALITY quality;
    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_match( argv[1], "text", 4 ) ) 
    {
        cmd_printf( "set copy quality to text\n" );
        quality = e_TEXT;
    }
    else if( str_match( argv[1], "draft", 5 ) ) 
    {
        cmd_printf( "set copy quality to draft\n" );
        quality = e_DRAFT;
    }
    else if( str_match( argv[1], "mixed", 5 ) ) 
    {
        cmd_printf( "set copy quality to mixed\n" );
        quality = e_MIXED;
    }
    else if( str_match( argv[1], "film", 4 ) ) 
    {
        cmd_printf( "set copy quality to film\n" );
        quality = e_FILM_PHOTO;
    }
    else if( str_match( argv[1], "picture", 7 ) ) 
    {
        cmd_printf( "set copy quality to picture\n" );
        quality = e_PICTURE;
    }
    else if( str_match( argv[1], "faxtext", 7 ) ) 
    {
        cmd_printf( "set copy quality to faxtext\n" );
        quality = e_FAX_TEXT;
    }
    else if( str_match( argv[1], "faxmixed", 8 ) ) 
    {
        cmd_printf( "set copy quality to faxmixed\n" );
        quality = e_FAX_MIXED;
    }
    else 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    copyapp_set_fp_qualitymode( quality );
    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_set_pquality_desc = "set print quality for the copy job";
static const char *copyapp_cmd_set_pquality_usage = "draft|normal|better|best";
static const char *copyapp_cmd_set_pquality_notes = "";
static int copyapp_cmd_set_pquality_cb(int argc, char *argv[])
{
    PRINT_QUALITY quality;
    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_match( argv[1], "draft", 5 ) ) 
    {
        cmd_printf( "set print quality to draft\n" );
        quality = PRINTQUALITY_DRAFT;
    }
    else if( str_match( argv[1], "normal", 6 ) ) 
    {
        cmd_printf( "set print quality to normal\n" );
        quality = PRINTQUALITY_NORMAL;
    }
    else if( str_match( argv[1], "better", 6 ) ) 
    {
        cmd_printf( "set print quality to better\n" );
        quality = PRINTQUALITY_BETTER;
    }
    else if( str_match( argv[1], "best", 4 ) ) 
    {
        cmd_printf( "set print quality to best\n" );
        quality = PRINTQUALITY_BEST;
    }
    else 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    copyapp_set_fp_print_quality( quality );
    return cmd_result(CMD_OK);
}


static const char *copyapp_cmd_resize_desc = "";
static const char *copyapp_cmd_resize_usage = "1up|2up|4up|a4-ltr|ltr-a4|lgl-ltr|lgl-a4|full|<scale factor>\n"
	"   Set the size for subsequent copies ('scale factor' is interpreted as %).\n"
	"    2up is used to place 2 scanned documents on one printed page.\n"
	"      Use copy orient to set the proper orientation for all input documents.\n"
	"      Use copy next to begin scanning subsequent documents.\n"
	"    4up is used to place 4 scanned documents on one printed page.\n"
	"      Use copy orient to set the proper orientation for all input documents.\n"
	"      Use copy next to begin scanning subsequent documents.";
static const char *copyapp_cmd_resize_notes = "";
static int copyapp_cmd_resize_cb(int argc, char *argv[])
{

    e_COPYRESIZE resize_var;
    uint32_t custom_resize;
    
    cmd_printf( "%s\n", __FUNCTION__);
    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    custom_resize = 100;

    /* parse page scaling (e.g., A4->Letter, etc) */
    if( str_match( argv[1], "1up", 3 ) ) 
    {
        resize_var = e_ORG_100PCNT;
        cmd_printf( "copy 1 up Letter\n" );
    }
    else if( str_match( argv[1], "2up", 3 ) ) 
    {
        resize_var = e_TWO_PAGES_PER_SHEET;
        cmd_printf( "copy 2 up\n" );
    }
    else if( str_match( argv[1], "4up", 3 ) ) 
    {
        resize_var = e_FOUR_PAGES_PER_SHEET;
        cmd_printf( "copy 4 up\n" );
    }
    else if( str_match( argv[1], "a4-ltr", 6 ) ) 
    {
        resize_var = e_A4_2_LTR_94PCNT;
        cmd_printf( "copy A4 to Letter (94%%)\n" );
    }
    else if( str_match( argv[1], "ltr-a4", 6 ) ) 
    {
        resize_var = e_LTR_2_A4_97PCNT;
        cmd_printf( "copy Letter to A4 (97%%)\n" );
    }
    else if( str_match( argv[1], "lgl-ltr", 7 ) ) 
    {
        resize_var = e_LEGAL_2_LTR_78PCNT;
        cmd_printf( "copy Legal to Letter (78%%)\n" );
    }
    else if( str_match( argv[1], "lgl-a4", 6 ) ) 
    {
        resize_var = e_LEGAL_2_A4_83PCNT;
        cmd_printf( "copy Legal to A4 (83%%)\n" );
    }
    else if( str_match( argv[1], "full", 4 ) ) 
    {
        resize_var = e_FULL_PAGE_91PCNT;
        cmd_printf( "copy full page (91%%)\n" );
    }
    else 
    {
        /* assume it's a scale factor */
        if( str_mkint( argv[1], &custom_resize ) != 0 ) 
        {
            cmd_printf("bad scale integer \"%s\"\n", argv[1]  );
            return cmd_result(CMD_USAGE_ERROR);
        }
        resize_var = e_CUSTOM_PCNT;            
        cmd_printf( "copy resize set to %u\n", custom_resize );
    }

    copyapp_set_fp_resize( resize_var, custom_resize );
    return cmd_result(CMD_OK);
}


static const char *copyapp_cmd_next_desc = "Use to scan subsequent documents when executing a multi-scan copy such as 2up or 4up.";
static const char *copyapp_cmd_next_usage = NULL;
static const char *copyapp_cmd_next_notes = NULL;
static int copyapp_cmd_next_cb(int argc, char *argv[]) 
{
    copyapp_copy_input( e_COPY_NEXT );
    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_finish_desc = "Use to finish a 2up or 4up multi-scan copy pre,maturly.";
static const char *copyapp_cmd_finish_usage = NULL;
static const char *copyapp_cmd_finish_notes = NULL;
static int copyapp_cmd_finish_cb(int argc, char *argv[]) 
{
    copyapp_copy_input( e_COPY_FINISH );
    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_cancel_desc = "Cancel the copy Job";
static const char *copyapp_cmd_cancel_usage = NULL;
static const char *copyapp_cmd_cancel_notes = NULL;
static int copyapp_cmd_cancel_cb(int argc, char *argv[]) 
{
    copyapp_copy_input(e_COPY_CANCEL);
    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_brightness_desc = "change copy brightness";
static const char *copyapp_cmd_brightness_usage = "<brightness integer>";
static const char *copyapp_cmd_brightness_notes = "brightness level is an integer (check whether its in valid range)";
static int copyapp_cmd_brightness_cb(int argc, char *argv[]) 
{
    uint32_t brightness;
    
    cmd_printf( "%s\n", __FUNCTION__);
    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[1], &brightness ) != 0 ) 
    {
        cmd_printf("bad brightness integer \"%s\"\n", argv[1] ) ;
        return cmd_result(CMD_USAGE_ERROR);
    }

    /* brightness values are limited to 1..11 */
    if( brightness < COPY_CONTRAST_MIN || brightness > COPY_CONTRAST_MAX ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    cmd_printf( "copy brightness set to %u\n", brightness );
    copyapp_set_fp_brightness( brightness );

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_background_desc = "change copy background";
static const char *copyapp_cmd_background_usage = "<background integer>";
static const char *copyapp_cmd_background_notes = "background level is an integer (check whether its in valid range)";
static int copyapp_cmd_background_cb(int argc, char *argv[]) 
{
    uint32_t background;
    
    cmd_printf( "%s\n", __FUNCTION__);
    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[1], &background ) != 0 ) 
    {
        cmd_printf("bad background integer \"%s\"\n", argv[1] ) ;
        return cmd_result(CMD_USAGE_ERROR);
    }

    /* background values are limited to 1..11 */
//    if( background <  COPY_BACKGROUND_MIN || background >  COPY_BACKGROUND_MAX )
    if( background >  COPY_BACKGROUND_MAX )
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    cmd_printf( "copy background set to %u\n", background );
    copyapp_set_fp_background( background );

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_contrast_desc = "change copy contrast";
static const char *copyapp_cmd_contrast_usage = "<contrast integer>";
static const char *copyapp_cmd_contrast_notes = "contrast level is an integer (check whether its in valid range)";
static int copyapp_cmd_contrast_cb(int argc, char *argv[]) 
{
    uint32_t contrast;
    
    cmd_printf( "%s\n", __FUNCTION__);
    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[1], &contrast ) != 0 ) 
    {
        cmd_printf("bad contrast integer \"%s\"\n", argv[1] ) ;
        return cmd_result(CMD_USAGE_ERROR);
    }

    /* contrast values are limited to 1..11 */
    if( contrast < COPY_CONTRAST_MIN || contrast > COPY_CONTRAST_MAX ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    cmd_printf( "copy contrast set to %u\n", contrast );
    copyapp_set_fp_contrast( contrast );

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_sharpness_desc = "change copy sharpness";
static const char *copyapp_cmd_sharpness_usage = "<sharpess integer>";
static const char *copyapp_cmd_sharpness_notes = "sharpness level is an integer (check whether its in valid range)";
static int copyapp_cmd_sharpness_cb(int argc, char *argv[]) 
{
    uint32_t sharpness;
    
    cmd_printf( "%s\n", __FUNCTION__);
    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[1], &sharpness ) != 0 ) 
    {
        cmd_printf("bad sharpness integer \"%s\"\n", argv[1] ) ;
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( sharpness < COPY_SHARP_MIN || sharpness > COPY_SHARP_MAX ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    cmd_printf( "copy sharpness set to %u\n", sharpness );
    copyapp_set_fp_sharpness( sharpness );

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_color_tint_desc = "adjust copy color balance";
static const char *copyapp_cmd_color_tint_usage = "<r adj> <g adj> <b adj>";
static const char *copyapp_cmd_color_tint_notes = " r, g, and b adj increase(+) or reduce(-) color";
static int copyapp_cmd_color_tint_cb(int argc, char *argv[]) 
{
    uint32_t r, g, b;
    
    cmd_printf( "%s\n", __FUNCTION__);
    if( 4 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[1], &r ) != 0 ) 
    {
        cmd_printf("bad 'r' integer \"%s\"\n", argv[1]);
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( r < COPY_COLORADJUST_MIN || r > COPY_COLORADJUST_MAX )
    {
        cmd_printf("'r' value out of range %d..%d\n", 
		     COPY_COLORADJUST_MIN, COPY_COLORADJUST_MAX);
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[2], &g ) != 0 ) 
    {
        cmd_printf("bad 'g' integer \"%s\"\n", argv[2]);
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( g < COPY_COLORADJUST_MIN || g > COPY_COLORADJUST_MAX )
    {
        cmd_printf("'g' value out of range %d..%d\n", 
		     COPY_COLORADJUST_MIN, COPY_COLORADJUST_MAX);
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[3], &b ) != 0 ) 
    {
        cmd_printf("bad 'b' integer \"%s\"\n", argv[3]) ;
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( b < COPY_COLORADJUST_MIN || b > COPY_COLORADJUST_MAX )
    {
        cmd_printf("'b' value out of range %d..%d\n", 
		     COPY_COLORADJUST_MIN, COPY_COLORADJUST_MAX);
        return cmd_result(CMD_USAGE_ERROR);
    }

    cmd_printf( "adjust copy color by r:%u g:%u b:%u\n", r, g, b );
    copyapp_set_fp_color_balance( r, g, b ); 

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_collate_desc = "enable/disable copy collation";
static const char *copyapp_cmd_collate_usage = "on|off";
static const char *copyapp_cmd_collate_notes = NULL;
static int copyapp_cmd_collate_cb(int argc, char *argv[]) 
{
    if( 2 != argc) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( str_match( argv[1], "on", 2 ) ) 
    {
        cmd_printf( "enable copy collation\n" );
        copyapp_set_fp_collation( TRUE );
    }
    else if( str_match( argv[1], "off", 3 ) ) 
    {
        cmd_printf( "disable copy collation\n" );
        copyapp_set_fp_collation( FALSE );
    }
    else 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_numcopies_desc = "used to specify number of copies";
static const char *copyapp_cmd_numcopies_usage = "<no of copies>";
static const char *copyapp_cmd_numcopies_notes = "Set the number of output documents per one input document.";
static int copyapp_cmd_numcopies_cb(int argc, char *argv[]) 
{
    uint32_t num_copies;

    if( 2 != argc) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[1], &num_copies ) != 0 ) 
    {
        cmd_printf("bad integer \"%s\" for num copies\n", argv[1] );
        return cmd_result(CMD_USAGE_ERROR);
    }

    copyapp_set_fp_num_copies( num_copies );
    cmd_printf( "num copies set to *%u\n", num_copies );
    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_pduplex_desc = "enable or disable printing duplex";
static const char *copyapp_cmd_pduplex_usage = "on|off";
static const char *copyapp_cmd_pduplex_notes = NULL;
static int copyapp_cmd_pduplex_cb(int argc, char *argv[]) 
{

    if( 2 != argc) {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_match( argv[1], "on", 2 ) ) {
        cmd_printf( "enable print auto duplexing\n" );
        copyapp_set_fp_print_duplex( TRUE );
    }
    else if( str_match( argv[1], "off", 3 ) ) {
        cmd_printf( "disable print auto duplexing\n" );
        copyapp_set_fp_print_duplex( FALSE );
    }
    else 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_sduplex_desc = "enable or disable scan duplex";
static const char *copyapp_cmd_sduplex_usage = "on|off";
static const char *copyapp_cmd_sduplex_notes = NULL;
static int copyapp_cmd_sduplex_cb(int argc, char *argv[]) 
{

    if( 2 !=argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_match( argv[1], "on", 2 ) ) 
    {
        cmd_printf( "enable scan auto duplexing\n" );
        copyapp_set_fp_scan_duplex( TRUE );
    }
    else if( str_match( argv[1], "off", 3 ) ) 
    {
        cmd_printf( "disable scan auto duplexing\n" );
        copyapp_set_fp_scan_duplex( FALSE );
    }
    else 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_color_desc = "";
static const char *copyapp_cmd_color_usage = "yes|no\n"
	"    Set color/mono for subsequent copies. yes=color, no=mono";
static const char *copyapp_cmd_color_notes = NULL;
static int copyapp_cmd_color_cb(int argc, char *argv[]) 
{
    copy_cmode_t colormode=COPY_CMODE_MONO;

    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( str_match( argv[1], "yes", 3 ) ) 
    {
	#ifdef COLOR_ENG
        cmd_printf( "turn on color copy\n" );
	colormode = COPY_CMODE_COLOR;
	#else
	colormode = COPY_CMODE_MONO;
	cmd_printf("Cannot set color on a mono engine.  Forcing to mono.\n");
	#endif
    }
    else if( str_match( argv[1], "no", 2 ) ) 
    {
        cmd_printf( "turn off color copy (use monochrome)\n" );
	colormode = COPY_CMODE_MONO;
    }
    else
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    copyapp_set_fp_colormode( colormode );

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_orient_desc = "set copy image orientation";
static const char *copyapp_cmd_orient_usage = "landscape|portrait\n"
	         "   Set the orientatin for all subsequent input documents.";
static const char *copyapp_cmd_orient_notes = NULL;
static int copyapp_cmd_orient_cb(int argc, char *argv[]) 
{
    e_ORIENTATION orientation;
    
    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_match( argv[1], "portrait", 8 ) ) 
    {
        cmd_printf( "copy n-up in portrait\n" );
        orientation = e_PORTRAIT;
    }
    else if( str_match( argv[1], "landscape", 9 ) ) 
    {
        cmd_printf( "copy n-up in landscape\n" );
        orientation = e_LANDSCAPE;
    }
    else 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    copyapp_set_fp_orientation( orientation );

    return cmd_result(CMD_OK);
}


static const char *copyapp_cmd_papersize_desc = "set copy paper size";
static const char *copyapp_cmd_papersize_usage = "any|A4|letter|legal\n"; static const char *copyapp_cmd_papersize_notes = NULL;
static int copyapp_cmd_papersize_cb(int argc, char *argv[]) 
{
    mediasize_t pagesize=0;

    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if (str_match(argv[1], "A4", 2))
    {
        pagesize = MEDIASIZE_A4;
    }
    else if (str_match(argv[1], "letter", 6))
    {
        pagesize = MEDIASIZE_LETTER;
    }
    else if (str_match(argv[1], "legal", 5))
    {
        pagesize = MEDIASIZE_LEGAL;
    }
    else if (str_match(argv[1], "any", 3))
    {
        pagesize = MEDIASIZE_ANY;
    }
    else
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    cmd_printf("Setting paper size to %d\n", pagesize); 
    printvar_set_default_papersize(&pagesize);
    CopyConf copy_config;
    memset( &copy_config, 0, sizeof(copy_config) );
    copyapp_get_copy_config(&copy_config);
    copy_config.printPaperSize = pagesize;
    copy_config.scanPaperSize = pagesize;
    copyapp_set_copy_config(&copy_config);    

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_inputtray_desc = "set input tray for copy";
static const char *copyapp_cmd_inputtray_usage = "<tray number>\n";
static const char *copyapp_cmd_inputtray_notes = NULL;
static int copyapp_cmd_inputtray_cb(int argc, char *argv[]) 
{
    // just ignore the request for now, there's only one tray
    cmd_printf("Setting input tray to 1\n");
    copyapp_set_fp_input_tray( TRAY_ANY );

    return cmd_result(CMD_OK);
}

#ifdef HAVE_ID_COPY
static const char *copyapp_cmd_id_desc = "start copy id job";
static const char *copyapp_cmd_id_usage = "cancel|finish|job|next\n"
	"         job = start the ID copy job, scan first ID from flatbed.\n"
	"         next = scan 'next' ID from flatbed.\n"
	"         finish = combine all the ID copies into one page and print.\n"
	"         cancel = stop copy ID job.";
static const char *copyapp_cmd_id_notes = NULL;
static int copyapp_cmd_id_cb(int argc, char *argv[]) 
{
    uint32_t copyid_action;
    if( argc != 2 ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( str_match( argv[1], "job", 3 ) ) 
    {
		copyapp_id_copy_start_job();
        return cmd_result(CMD_OK);
    }
    else if( str_match( argv[1], "next", 4 ) ) 
    {
		copyid_action = e_COPY_NEXT;
    }
    else if( str_match( argv[1], "cancel", 6 ) ) 
    {
		copyid_action = e_COPY_CANCEL;
    }
    else if( str_match( argv[1], "finish", 6 ) ) 
    {
		copyid_action = e_COPY_FINISH;
    }
    else
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    //if(! idCopyEnable)
    if(! is_idcopy_enabled())
    {
		cmd_printf("Invalid ID copy request.  ID copy is not running\n");
        return cmd_result(CMD_USAGE_ERROR);
    }
    copyapp_id_copy_input(copyid_action);
    return cmd_result(CMD_OK);
}
#endif

static const char *copyapp_cmd_region_desc = "Set rectangle scan region for copy";
static const char *copyapp_cmd_region_usage = "x y width height";
static const char *copyapp_cmd_region_notes = NULL;
static int copyapp_cmd_region_cb(int argc, char *argv[]) 
{
    CopyConf copy_config;
    uint32_t x, y, width, height;

    if( 5 !=argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[1], &x ) != 0) 
    {
        cmd_printf("bad X integer \"%s\"\n", argv[1] ) ;
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( str_mkint( argv[2], &y ) != 0 ) 
    {
        cmd_printf("bad Y integer \"%s\"\n", argv[2] ) ;
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( str_mkint( argv[3], &width ) != 0 ) 
    {
        cmd_printf("bad width integer \"%s\"\n", argv[3] ) ;
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( str_mkint( argv[4], &height ) != 0 ) 
    {
        cmd_printf("bad height integer \"%s\"\n", argv[4] ) ;
        return cmd_result(CMD_USAGE_ERROR);
    }

    copyapp_set_fp_resize( e_CUSTOM_REGION, 100 );

    copyapp_get_copy_config(&copy_config);
    copy_config.region.x = x;
    copy_config.region.y = y;
    copy_config.region.width = width;
    copy_config.region.height = height;
    copyapp_set_copy_config(&copy_config);

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_nvram_desc = "display the current copy attributes saved in nvram";
static const char *copyapp_cmd_nvram_usage = "[reset]";
static const char *copyapp_cmd_nvram_notes = NULL;
static int copyapp_cmd_nvram_cb(int argc, char *argv[]) 
{
    if( 1 == argc ) 
    {
	print_copy_config_settings(true); // true implies read from nvram
        return cmd_result(CMD_OK);
    }
    else if( 2 == argc ) 
    {
	if( str_match( argv[1], "reset", 5 ) ) 
	{
	    CopyConf conf;
            copyapp_init_settings( &conf );
            copyapp_set_copy_config( &conf );
	    cmd_printf("Set copy settings to factory defaults.\n");

            return cmd_result(CMD_OK);
	}
    }
    return cmd_result(CMD_USAGE_ERROR);

}

static const char *copyapp_cmd_log_desc = "set the copy log level";
static const char *copyapp_cmd_log_usage = "<log level>\n";
static const char *copyapp_cmd_log_notes = "<log level> options are 0,1,2,3. Verbosity increases as log level increases";
static int copyapp_cmd_log_cb(int argc, char *argv[]) 
{
    uint32_t n;

    if( 2 != argc ) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    if( str_mkint( argv[1], &n ) != 0 )
    {
        return cmd_result(CMD_USAGE_ERROR);
    }
    
    cmd_printf("Set copy log level to %d.\n", (int)n);
    set_copylog_level(n);

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_OOM_desc = "tell the print job manager about copy going out of memory";
static const char *copyapp_cmd_OOM_usage = NULL;
static const char *copyapp_cmd_OOM_notes = NULL;
static int copyapp_cmd_OOM_cb(int argc, char *argv[]) 
{
    cmd_printf( "OOM! out of memory\n" );
    copyapp_OOM( );

    return cmd_result(CMD_OK);
}

static const char *copyapp_cmd_resolution_desc = "used to specify copy resolution";
static const char *copyapp_cmd_resolution_usage = "<resolution in dpi (300|600)>";
static const char *copyapp_cmd_resolution_notes = "Set the copy resolution.";
static int copyapp_cmd_resolution_cb(int argc, char *argv[]) 
{
    uint32_t dpi;

    if( 2 != argc) 
    {
        return cmd_result(CMD_USAGE_ERROR);
    }

    if( str_mkint( argv[1], &dpi ) != 0 ) 
    {
        cmd_printf("bad integer \"%s\" for num copies\n", argv[1] );
        return cmd_result(CMD_USAGE_ERROR);
    }

    if (dpi == 300 || dpi == 600)
    {
        copyapp_set_fp_resolution( dpi );
        cmd_printf( "resolution set to %d\n", dpi );
    }
    else
    {
        cmd_printf("invalid scan resolution %d\n", dpi );
        return cmd_result(CMD_USAGE_ERROR);
    }
    return cmd_result(CMD_OK);
}

#endif //HAVE_CMD
