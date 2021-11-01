/*
 * ============================================================================
 * Copyright (c) 2010-2011 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <string.h>

#include "laser_engine_config_api.h"
#include "agprint.h" 
#include "lassert.h"
#include "dprintf.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

uint32_t engine_get_default_side_margin_in_subpixels( PAGE_DATA *page,
                                              uint32_t channel_number,
                                              uint32_t image_width_in_pixels,
                                              resolution_t page_x_resolution )
{
    uint32_t margin;
    uint32_t width;
    laser_engine_video_config_t * video_config;

    engine_get_video_config( &video_config );

    // This is a center feed engine, so the side margin is calculated by 
    // subtracting half the image width in pixels from the center of the paper
    // path.
    
    // Adjust the image width in pixels to the engine resolution
    width = image_width_in_pixels * video_config->print_engine_native_resolution / page_x_resolution;
    width *= video_config->sub_pixel_denominator;
    margin = video_config->side_margin_reference_in_sub_pixels[channel_number] - width/2;

	dbg_printf("VIDEO FREQUENCY : %d %d\n", video_config->pixel_freq_in_hz[0], image_width_in_pixels);

    // dbg_printf("width %d, center %d, margin %d\n", width,video_config->side_margin_reference_in_sub_pixels[channel_number], margin);

    margin = margin * page_x_resolution / video_config->print_engine_native_resolution;

    if (page->DocType == e_CALPAGE)   
    {
        margin = video_config->density_cal_side_margin;  
    }

    return margin;
}

uint32_t engine_get_default_top_margin_in_lines( PAGE_DATA *page, COLOR process_color)
{
    uint32_t top_margin;

    laser_engine_video_config_t * video_config;

    engine_get_video_config( &video_config );

    if (page->DocType == e_CALPAGE)
    {
        top_margin = video_config->density_cal_top_margin;
        // dbg_printf("CAL PAGE TOP MARGIN %d\n", top_margin);
    }
    else 
    {
        top_margin = video_config->top_of_page_margin_in_lines[process_color];
    }

    return top_margin;
}


// The way the pcode registers are set are typically what one wants -- the
// pulse codes are used for center, late, early, and split justification.
// What these functions do not allow is setting non-standard justification
// settings.  For instance, outputing a center-code even when a
// early/late/split code is requested, or setting different pulse codes for
// each justification.  These uses are very unusual and are not supported
// through this interface.

// Given an array base and four 32-bit words, program the entries.
static void set_pcode_array( uint32_t* pCodePtr, uint32_t one, uint32_t two, uint32_t three, uint32_t four)
{
   ASSERT(pCodePtr != NULL);

   // Verify input values don't have justification bits set
   XASSERT( (one   & 0xC0C0C0C0) == 0, one);
   XASSERT( (two   & 0xC0C0C0C0) == 0, two);
   XASSERT( (three & 0xC0C0C0C0) == 0, three);

   pCodePtr[ 0] = one;
   pCodePtr[ 1] = two;
   pCodePtr[ 2] = three;
   pCodePtr[ 3] = four;
   pCodePtr[ 4] = one   | 0x40404040;
   pCodePtr[ 5] = two   | 0x40404040;
   pCodePtr[ 6] = three | 0x40404040;
   pCodePtr[ 7] = four  | 0x40404040;
   pCodePtr[ 8] = one   | 0x80808080;
   pCodePtr[ 9] = two   | 0x80808080;
   pCodePtr[10] = three | 0x80808080;
   pCodePtr[11] = four  | 0x80808080;
   pCodePtr[12] = one   | 0xC0C0C0C0;
   pCodePtr[13] = two   | 0xC0C0C0C0;
   pCodePtr[14] = three | 0xC0C0C0C0;
   pCodePtr[15] = four  | 0xC0C0C0C0;
}

// Example use:  engine_set_video_pcode_4Bit( 0x0C080400, 0x1C181410, 0x2C282420, 0x3F383430 )
void engine_set_video_pcode_4Bit(bool threshold_halftone,
                                 uint32_t one,
                                 uint32_t two,
                                 uint32_t three,
                                 uint32_t four)
{
    laser_engine_video_config_t *config;
    uint32_t *pCodePtr;
    engine_get_video_config(&config);


    if (threshold_halftone)
    {
        pCodePtr = (uint32_t *)config->pulse_code_4_bit_array;
        if (pCodePtr != NULL)
        {
            set_pcode_array(pCodePtr, one, two, three, four);
            video_eng_pcode_config(pCodePtr, NUM_PCODE_REGS, PCODE_INDEX_4BIT);
        }
    } else
    {
        pCodePtr = (uint32_t *)config->ed_pulse_code_4_bit_array;
        if (pCodePtr != NULL)
        {
            set_pcode_array(pCodePtr, one, two, three, four);
            video_eng_pcode_config(pCodePtr, NUM_PCODE_REGS, PCODE_INDEX_ED4BIT);
        }
    }
}

// Example use:  engine_set_video_pcode_2Bit( 0x0A, 0x14, 0x1E )
void engine_set_video_pcode_2Bit(bool threshold_halftone,
                                 uint32_t one,
                                 uint32_t two,
                                 uint32_t three)
{
    laser_engine_video_config_t *config;
    engine_get_video_config(&config);

    // Verify the codes are in range
    XASSERT(one   <= 0x3F, one);
    XASSERT(two   <= 0x3F, two);
    XASSERT(three <= 0x3F, three);

    // Expand to fill the words
    one   |= (one   << 24) | (one   << 16) | (one   << 8);
    two   |= (two   << 24) | (two   << 16) | (two   << 8);
    three |= (three << 24) | (three << 16) | (three << 8);

    uint32_t *pCodePtr;

    if (threshold_halftone)
    {
        pCodePtr = (uint32_t *)config->pulse_code_2_bit_array;
        if (pCodePtr != NULL)
        {
            set_pcode_array(pCodePtr, 0x00, one, two, three);
            video_eng_pcode_config(pCodePtr, NUM_PCODE_REGS, PCODE_INDEX_2BIT);
        }
    } else
    {
        pCodePtr = (uint32_t *)config->ed_pulse_code_2_bit_array;
        if (pCodePtr != NULL)
        {
            set_pcode_array(pCodePtr, 0x00, one, two, three);
            video_eng_pcode_config(pCodePtr, NUM_PCODE_REGS, PCODE_INDEX_ED2BIT);
        }
    }
}

// Example use:  engine_set_video_pcode_1Bit( 0x1E )
void engine_set_video_pcode_1Bit(bool threshold_halftone, uint32_t one)
{
    laser_engine_video_config_t *config;
    engine_get_video_config(&config);

    XASSERT(one <= 0x3F, one);
    one |= (one << 24) | (one << 16) | (one << 8);

    uint32_t *pCodePtr;
    if (threshold_halftone)
    {
        pCodePtr = (uint32_t *)config->pulse_code_1_bit_array;

        if (pCodePtr != NULL)
        {
            set_pcode_array(pCodePtr, 0x00, 0x00, one, one);
            video_eng_pcode_config(pCodePtr, NUM_PCODE_REGS, PCODE_INDEX_1BIT);
        }
    } else
    {
        pCodePtr = (uint32_t *)config->ed_pulse_code_1_bit_array;
        if (pCodePtr != NULL)
        {
            set_pcode_array(pCodePtr, 0x00, 0x00, one, one);
            video_eng_pcode_config(pCodePtr, NUM_PCODE_REGS, PCODE_INDEX_ED1BIT);
        }
    }
}


void engine_get_video_pcode_4Bit(bool threshold_halftone,
                                 uint32_t *one,
                                 uint32_t *two,
                                 uint32_t *three,
                                 uint32_t *four)
{
    laser_engine_video_config_t *config;
    engine_get_video_config(&config);

    uint32_t *pCodePtr;

    if  (threshold_halftone == true)
    {
        pCodePtr = (uint32_t *)config->pulse_code_4_bit_array;
    } else
    {

        pCodePtr = (uint32_t *)config->ed_pulse_code_4_bit_array;
    }

    *one = pCodePtr[0];
    *two = pCodePtr[1];
    *three = pCodePtr[2];
    *four = pCodePtr[3];
}

void engine_get_video_pcode_2Bit(bool threshold_halftone,
                                 uint32_t *one,
                                 uint32_t *two,
                                 uint32_t *three)
{
    laser_engine_video_config_t *config;
    engine_get_video_config(&config);

    uint32_t *pCodePtr;

    if (threshold_halftone)
    {
        pCodePtr = (uint32_t *)config->pulse_code_2_bit_array;
    } else
    {
        pCodePtr = (uint32_t *)config->ed_pulse_code_2_bit_array;
    }
    *one = pCodePtr[1];
    *two = pCodePtr[2];
    *three = pCodePtr[3]; 
}

void engine_get_video_pcode_1Bit( bool threshold_halftone,
                                  uint32_t * one )
{
    laser_engine_video_config_t * config;
    engine_get_video_config(&config);

    uint32_t *pCodePtr;

	if (threshold_halftone)
    {
        pCodePtr = (uint32_t *)config->pulse_code_1_bit_array;
    }
    else
    {
        pCodePtr = (uint32_t *)config->ed_pulse_code_1_bit_array;
    }

    *one = pCodePtr[2];
}


#ifdef HAVE_CMD
// Allow choosing between threshold and error diffusion halftone. Default to 
// threshold.
static bool threshold_halftone = true;

static char *mip_pcode_cmd_set_desc = "set pcode values";
static char *mip_pcode_cmd_set_usage = "< 1 | 2 | 4 > 6bit hex values ( 0 - 3f ) 2 values required for 1bpp, 4 values required for 2bpp and 16 values required for 4bpp";
static char *mip_pcode_cmd_set_notes = "The pixel input values can be either 1,2 or 4 bits. The resulting pulse code output values are a 6 bits enter hex values between 0 and 3F for each pcode";

static int mip_tecm_set_pcode( int argc, char *argv[] )
{
    int retValue = CMD_OK;
    uint32_t one = 0;
    uint32_t two = 0;
    uint32_t three = 0;
    uint32_t four = 0;
    int i = 0;

    if ((argc < 2))
    {
        return CMD_USAGE_ERROR;
    }
    
    if( atoi(argv[1]) == 2 )
    {

        if( argc == 6)
        {
            // Read the three pulse code values (implied 0x00 for the first)
            one   = strtol( argv[3], (char**)NULL, 16);
            two   = strtol( argv[4], (char**)NULL, 16);
            three = strtol( argv[5], (char**)NULL, 16);
            // Verify they are in range.
            if ( (one > 0x3f) || (two > 0x3f) || (three > 0x3f) ) {
               cmd_printf("ERROR: parameters out of range 0x00 - 0x3F\n");
               return CMD_USAGE_ERROR;
            }
            engine_set_video_pcode_2Bit(threshold_halftone, one, two, three);
            
        }
        else
        {
            cmd_printf("ERROR: Not enough parameters\n");
            retValue = CMD_USAGE_ERROR;
        }
        
    }
    else if( atoi(argv[1]) == 4 )
    {

        // pcode set <bpp>  0  4  8  c 10 14 18 1c 20 24 28 2c 30 34 38 3f
        if( argc == 18 )
        {

            cmd_printf("setting 4bpp pcodes to " );
            // load the first 32 bits with first 4 entries
            for( i = 2; i <6; i++)
            {
                cmd_printf("%s ",argv[i]);
                one += (strtol(argv[i],(char**)NULL,16) << ((i-2)*8));
            }

            // load the second 32 bits with next 4 entries
            for( i = 6; i <10; i++)
            {
                cmd_printf("%s ",argv[i]);
                two |= (strtol(argv[i],(char**)NULL,16) << ((i-6)*8));
            }

            // load the third 32 bits with next 4 entries
            for( i = 10; i <14; i++)
            {
                cmd_printf("%s ",argv[i]);
                three |= (strtol(argv[i],(char**)NULL,16) << ((i-10)*8));
            }

            // load the fourth 32 bits with next 4 entries
            for( i = 14; i <18; i++)
            {
                cmd_printf("%s ",argv[i]);
                four |= (strtol(argv[i],(char**)NULL,16) << ((i-14)*8));

            }

            engine_set_video_pcode_4Bit( threshold_halftone, one, two, three, four );
            cmd_printf("\n");
        }
        else
        {
            cmd_printf("ERROR: Not enough parameters\n");
            retValue = CMD_USAGE_ERROR;

        }
    }
    else if( atoi(argv[1]) == 1 )
    {
        cmd_printf("setting 1bpp pcode");
        engine_set_video_pcode_1Bit( threshold_halftone, strtol( argv[2],(char**)NULL,16) );
    }
    else
    {
        retValue = CMD_USAGE_ERROR;
    }

    return retValue;
}

static char *mip_pcode_cmd_show_desc = "show pcode values";
static char *mip_pcode_cmd_show_usage = "< 1 | 2 | 4 > ";

static int mip_tecm_get_pcode( int argc, char *argv[] )
{
    int retValue = CMD_OK;
    int i, d;
    uint32_t one, two, three, four;

    if ((argc != 2))
    {
        return CMD_USAGE_ERROR;
    }

    if( atoi(argv[1]) == 2 )
    {
        engine_get_video_pcode_2Bit( threshold_halftone, &one, &two, &three);
        cmd_printf("2bpp pcodes = 0 %x %x %x\n", (unsigned int)one & 0x3f,(unsigned int)two & 0x3f,(unsigned int)three & 0x3f);
    }
    else if( atoi(argv[1]) == 4 )
    {
        
        engine_get_video_pcode_4Bit( threshold_halftone, &one, &two, &three, &four );
        cmd_printf("4bpp pcodes = " );
        for( i = 0; i < 4; i++)
        {
            uint32_t temp;
            if( i == 0)
            {
                temp = one;
            }
            else if( i == 1 )
            {
                temp = two;
            }
            else if( i == 2 )
            {
                temp = three;
            }
            else if( i == 3 )
            {
                temp = four;
            }
            for(d = 0; d < 4; d++)
            {

                cmd_printf("%3x", (unsigned int)temp & 0x3f);
                temp = temp >> 8;
            }
        }
        cmd_printf("\n");
        
    }
    else if( atoi(argv[1]) == 1 )
    {
        engine_get_video_pcode_1Bit( threshold_halftone, &one );
        cmd_printf("1bpp pcodes = %x\n", (unsigned int)one & 0x3f );
    }
    else
    {
        retValue = CMD_USAGE_ERROR;
    }

    return retValue;
}


static char *mip_pcode_cmd_ht_desc = "set pcode ht";
static char *mip_pcode_cmd_ht_usage = "<th | ed>";

static int mip_tecm_set_pcode_ht( int argc, char *argv[] )
{
    int ret_value = CMD_OK;
    
    if ( argc == 2)
    {
        if ( strncmp( argv[1], "ed", 2) == 0 )
        {
            cmd_printf("Set pulse code entry for error diffusion halftone\n");
            threshold_halftone = false;
        }
        else if ( strncmp( argv[1], "th", 2) == 0)
        {
            cmd_printf("Set pulse code entry for threshold halftone\n");
            threshold_halftone = true;
        }
        else
        {
             return CMD_USAGE_ERROR;
        }
        // cmd_printf("num parms %d, %s %s %s \n", argc, argv[0], argv[1], argv[2]);
    }
    else
    {
        cmd_printf("ERROR: Not enough parameters\n");
        ret_value = CMD_USAGE_ERROR;
    }

    return ret_value;
}

void engine_setup_pcode_debug_commands( void )
{

    int cmd_res;
    
    /* register logger commands */

    cmd_res = cmd_register_cmd( "pcode",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    XASSERT( cmd_res == CMD_OK, cmd_res );

    cmd_res = cmd_register_subcmd( "pcode",
                                   "set",
                                   mip_pcode_cmd_set_desc,
                                   mip_pcode_cmd_set_usage,
                                   mip_pcode_cmd_set_notes,
                                   mip_tecm_set_pcode );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    
    cmd_res = cmd_register_subcmd( "pcode",
                                   "show",
                                   mip_pcode_cmd_show_desc,
                                   mip_pcode_cmd_show_usage,
                                   NULL,
                                   mip_tecm_get_pcode );
    XASSERT( cmd_res == CMD_OK, cmd_res );

    cmd_res = cmd_register_subcmd( "pcode",
                                   "ht",
                                   mip_pcode_cmd_ht_desc,
                                   mip_pcode_cmd_ht_usage,
                                   NULL,
                                   mip_tecm_set_pcode_ht );
    XASSERT( cmd_res == CMD_OK, cmd_res );

}

#endif
