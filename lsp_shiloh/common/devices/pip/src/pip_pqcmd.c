/******************************************************************************
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pip_pqcmd.c
 *
 * \brief Code for the 'pq' serial line command.  This allows changing various
 *        print and copy settings (PIP, engine, and scan). 
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "asic.h"
#include "pip_api.h"
#include "pip.h"
#include "pip_utils.h"

#include "pip_cfg.h"
#include "lassert.h"

#include "print_mode.h"
#include "print_mode_api.h"
#include "laser_engine_config_api.h"

#include "cmd_proc_api.h"

#define DBG_PRFX "PIP_PQ: "



static const char *pq_cmd_force_color_desc = "Select a color plane for RGB to CMYK to K printing";
static const char *pq_cmd_force_color_usage = "pq force_color [c | m | y | k ]";

static bool forced_color_enabled = true; // mono engine requires this
static COLOR forced_color = COLOR_BLACK;

static int pq_cmd_force_color_cb( int argc, char *argv[] )
{
    if ( 2 == argc ) {
        if (strcmp(argv[1], "c") == 0) {
            forced_color = COLOR_CYAN;
        } else if (strcmp(argv[1], "m") == 0) {
            forced_color = COLOR_MAGENTA;
        } else if (strcmp(argv[1], "y") == 0) {
            forced_color = COLOR_YELLOW;
        } else if (strcmp(argv[1], "k") == 0) {
            forced_color = COLOR_BLACK;
        } else {
            return CMD_USAGE_ERROR;
        }
        return CMD_OK;
    }
    return CMD_USAGE_ERROR;
}

COLOR next_forced_color( COLOR c ) 
{
    if ( forced_color_enabled ) {
        return forced_color;
    }
    return c; 
} 

// In generator_pip.c:
extern int genpip_force_mode(int value);
extern int genpip_get_mode(void);

static int find_pip_mode(int argc, char *argv[], int *argn)
{
    // Look for "mode #"
    if ( ((argc-*argn) >= 2) && (strcmp(argv[*argn], "mode") == 0) ) {
        int mode = atoi(argv[*argn+1]);
        *argn += 2;
        return mode;
    }
    return genpip_get_mode();
}


static uint32_t find_default_print_mode(void)
{
  PAGE_DATA page;
  print_mode_t mode;
  uint32_t pipe_configuration;

  page.page_output_color_mode = false;
  page.DocType = e_DOCUMENT;
  page.PrintQuality = PRINTQUALITY_NORMAL;
  
  page.print_mode = &mode;
  (void) print_mode_get(&page);
  pipe_configuration = print_mode_get_image_pipe_configuration(&mode);
  return pipe_configuration;
}


static uint32_t find_default_copy_mode(void)
{
  PAGE_DATA page;
  print_mode_t mode;
  uint32_t pipe_configuration;

  page.page_output_color_mode = false;
  page.DocType = e_COPYPAGE;
  // copyapp.c:
  //    Text, FaxText, FaxMixed => NORMAL
  //    Draft => DRAFT
  //    Film, Picture => BEST
  //    Mixed => BETTER
  page.PrintQuality = PRINTQUALITY_BEST;

  page.image_output_info.image_bpp = 8;
  page.image_output_info.image_dpi.x = 600;
  page.image_output_info.image_dpi.y = 600;
  page.image_output_info.image_width_in_pixels = page.image_output_info.image_dpi.x * 7;
  page.image_output_info.image_data_width_in_bytes = page.image_output_info.image_width_in_pixels * page.image_output_info.image_bpp / 8;

  page.print_mode = &mode;
  (void) print_mode_get(&page);
  pipe_configuration = print_mode_get_image_pipe_configuration(&mode);
  return pipe_configuration;
}


static const char *pq_cmd_modes_desc = "display the selectable pip modes";
static const char *pq_cmd_modes_usage = NULL;
static const char *pq_cmd_modes_notes = NULL;


static int         pq_cmd_modes_cb(int argc, char *argv[])
{
    int mode;
    int current_mode, default_print_mode, default_copy_mode;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    current_mode = genpip_get_mode();
    default_print_mode = find_default_print_mode();
    default_copy_mode = find_default_copy_mode();

    for (mode = 0; mode < IPC_LAST; mode++) {
      if (pip_is_mode_supported(mode)) {
        cmd_printf("%s Mode %2d:  %u-bpp   %-30s  %s\n",
                   (mode == current_mode)  ?  "*"  :  " ",
                   mode,
                   pip_mode_bpp_get(mode),
                   pip_mode_name_get(mode),
                   (mode == default_print_mode) ? "<- default print" :
                   (mode == default_copy_mode)  ? "<- default copy (film)"  : ""
                  );
      }
    }
    if (current_mode >= 0)
       cmd_printf("Mode is forced to %d '%s'\n", current_mode, pip_mode_name_get(current_mode));
    return CMD_OK;
}

static const char *pq_cmd_mode_desc = "get or set the pip mode";
static const char *pq_cmd_mode_usage = "[<mode>|default]";
static const char *pq_cmd_mode_notes = "if <mode> is not specified the current value will be displayed";


static int         pq_cmd_mode_cb(int argc, char *argv[])
{
    int mode;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (2 == argc)
    {
        if (strcmp(argv[1], "default") == 0) {
            genpip_force_mode(-1);
        } else {
            if ( genpip_force_mode(atoi(argv[1])) == 0) {
               cmd_printf("Mode %d is unsupported.  Use 'pq modes' to show supported modes.\n", atoi(argv[1]));
               return CMD_OK;
            }
        }
    }

    mode = genpip_get_mode();

    if (mode < 0) {
        cmd_printf( "pip mode is default\n");
    } else {
        cmd_printf( "pip mode is forced to %d  '%s'\n",
                    mode, pip_mode_name_get(mode) );
    }

    return CMD_OK;
}


#if ASIC_REV >= ASIC_REV_B0 && ASIC_gemstone

typedef struct pip_sharpen_config_entry_s {
  const char*     name;
  sharpen_cfg_t*  config;
} pip_sharpen_config_entry_t;

#ifndef PIP_SHARPEN_CONFIGURATIONS
  #define PIP_SHARPEN_CONFIGURATIONS \
   { "bypass",     &sharpen_bypass_cfg }, \
   { "generic",    &sharpen_generic_cfg }, \
   { "aggressive", &sharpen_aggressive_cfg }, \
   { "test",       &sharpen_test_cfg }
#endif

static pip_sharpen_config_entry_t pip_sharpen_configs[] = {PIP_SHARPEN_CONFIGURATIONS};
static sharpen_cfg_t* match_sharpen_name(const char* name) {
  size_t n, nconfigs;
  nconfigs = sizeof(pip_sharpen_configs) / sizeof(pip_sharpen_configs[0]);
  for (n = 0; n < nconfigs; n++) {
    if (strcmp(name, pip_sharpen_configs[n].name) == 0)
      return pip_sharpen_configs[n].config;
  }
  return NULL;
}
static const char* match_sharpen_config(const sharpen_cfg_t* config) {
  size_t n, nconfigs;
  nconfigs = sizeof(pip_sharpen_configs) / sizeof(pip_sharpen_configs[0]);
  for (n = 0; n < nconfigs; n++) {
    if (config == pip_sharpen_configs[n].config)
      return pip_sharpen_configs[n].name;
  }
  return NULL;
}
static void init_sharpen_config(sharpen_cfg_t* config) {
  ASSERT(config != 0);
  config->bypass = 0;
  config->fill_nReplicate = 0;
  config->ot_nib_sel = 0;
  config->image_width = 0;
  config->fill_val_Y = 0;
  config->fill_val_C = 0;
}

static const char *pq_cmd_sharpen_desc = "set the pip sharpen settings";
static const char *pq_cmd_sharpen_usage = "[mode #] [off|light|medium|aggressive|level <0-5>|<9 matrix values>]";
static const char *pq_cmd_sharpen_notes = NULL;
static int         pq_cmd_sharpen_cb(int argc, char *argv[])
{
    int mode;
    int argn = 1;
    static sharpen_cfg_t sharpen_custom_cmd_cfg;
    sharpen_cfg_t *cfg = 0;
    pip_cfg_t* pip_cfg = NULL;

    mode = find_pip_mode(argc, argv, &argn);

    if (mode < 0) {
        cmd_printf("Mode must be forced or specified.\n");
        return CMD_OK;
    }
    ASSERT( genpip_is_mode_supported(mode) );
    pip_cfg_reference_get(mode, &pip_cfg);

    // Change setting if requested
    if (argn < argc) {
      if (strcmp(argv[argn], "off") == 0) {
        pip_cfg->sharpen_cfg = &sharpen_bypass_cfg;
      } else if (match_sharpen_name(argv[argn]) != NULL) {
        pip_cfg->sharpen_cfg = match_sharpen_name(argv[argn]);
      } else if ( (strcmp(argv[argn], "level") == 0) && ((argc-argn) == 2) &&
                  (atoi(argv[argn+1]) == 0) ) {
        // "level 0" = bypass
        pip_cfg->sharpen_cfg = &sharpen_bypass_cfg;
      } else if ( (strcmp(argv[argn], "level") == 0) && ((argc-argn) == 2) &&
                  ((atoi(argv[argn+1]) >= 1) && (atoi(argv[argn+1]) <= 5)) ) {
        // Initialize with defaults
        init_sharpen_config(&sharpen_custom_cmd_cfg);
        cfg = &sharpen_custom_cmd_cfg;
        // -sqrt(800) / linear distance from center
        // arguably we want to tighten up the spot, e.g. distance^1.5
        cfg->Y_coeff[0][0]                      = -10;
        cfg->Y_coeff[0][1] = cfg->Y_coeff[1][0] = -13;
        cfg->Y_coeff[0][2] = cfg->Y_coeff[2][0] = -14;
        cfg->Y_coeff[1][1]                      = -20;
        cfg->Y_coeff[1][2] = cfg->Y_coeff[2][1] = -28;

        // 'light' is visually between level 1 and 2.
        // 'medium' is level 2.
        // 'aggressive' is visually between 3 and 4.
        switch (atoi(argv[argn+1])) {
          case 1: cfg->Y_coeff[2][2] = 900; break;
          case 2: cfg->Y_coeff[2][2] = 600; break;
          case 3: cfg->Y_coeff[2][2] = 500; break;   // almost to 'aggressive'
          case 4: cfg->Y_coeff[2][2] = 450; break;   // oversharpened
          case 5: cfg->Y_coeff[2][2] = 425; break;   // very oversharpened
          default: ASSERT(0);
        }

        // TODO: currently setting Y and C the same.  Decide how to parse both.
        //       e.g.  Y ... C ...
        memcpy(cfg->C_coeff, cfg->Y_coeff, sizeof(cfg->C_coeff));

      } else if ( (argc-argn) == 9 ) {
        // Load defaults for the structure and set.
        init_sharpen_config(&sharpen_custom_cmd_cfg);
        cfg = &sharpen_custom_cmd_cfg;

        // Read in 9 parameters
        cfg->Y_coeff[0][0] = atoi(argv[argn+0]);
        cfg->Y_coeff[0][1] = atoi(argv[argn+1]);
        cfg->Y_coeff[0][2] = atoi(argv[argn+2]);
        cfg->Y_coeff[1][0] = atoi(argv[argn+3]);
        cfg->Y_coeff[1][1] = atoi(argv[argn+4]);
        cfg->Y_coeff[1][2] = atoi(argv[argn+5]);
        cfg->Y_coeff[2][0] = atoi(argv[argn+6]);
        cfg->Y_coeff[2][1] = atoi(argv[argn+7]);
        cfg->Y_coeff[2][2] = atoi(argv[argn+8]);

        // Duplicate into C.
        memcpy(cfg->C_coeff, cfg->Y_coeff, sizeof(cfg->C_coeff));

      } else {
        return CMD_USAGE_ERROR;
      }

      // Separate this from reading the values.
      // Calculate shift values, verify all values, set.
      if (cfg != 0) {
        int coef_sum;

        // Calculate shift and shiftfactor based on description in DRS.
        coef_sum =
            4*cfg->Y_coeff[0][0] + 4*cfg->Y_coeff[0][1] + 2*cfg->Y_coeff[0][2]
          + 4*cfg->Y_coeff[1][0] + 4*cfg->Y_coeff[1][1] + 2*cfg->Y_coeff[1][2]
          + 2*cfg->Y_coeff[2][0] + 2*cfg->Y_coeff[2][1] +   cfg->Y_coeff[2][2];
        cfg->Y_shift       = 3 + (int) (log2f((float)coef_sum)-2.0001);
        cfg->Y_shiftfactor = (int) roundf(   exp2f(14 + cfg->Y_shift - 3)
                                           / (float)coef_sum );
        coef_sum =
            4*cfg->C_coeff[0][0] + 4*cfg->C_coeff[0][1] + 2*cfg->C_coeff[0][2]
          + 4*cfg->C_coeff[1][0] + 4*cfg->C_coeff[1][1] + 2*cfg->C_coeff[1][2]
          + 2*cfg->C_coeff[2][0] + 2*cfg->C_coeff[2][1] +   cfg->C_coeff[2][2];
        cfg->C_shift       = 3 + (int) (log2f((float)coef_sum)-2.0001);
        cfg->C_shiftfactor = (int) roundf(   exp2f(14 + cfg->C_shift - 3)
                                           / (float)coef_sum );

        // Verify this is valid.
        {
          int i, j;
          for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
              if ( ( ((i == 2) && (j == 2)) &&
                     (    (cfg->Y_coeff[i][j] < -1023)
                       || (cfg->C_coeff[i][j] < -1023)
                       || (cfg->Y_coeff[i][j] >  1023)
                       || (cfg->C_coeff[i][j] >  1023) ) ) ||
                   ( ((i != 2) || (j != 2)) &&
                     (    (cfg->Y_coeff[i][j] < -255)
                       || (cfg->C_coeff[i][j] < -255)
                       || (cfg->Y_coeff[i][j] >  255)
                       || (cfg->C_coeff[i][j] >  255) ) ) ) {
                 cmd_printf("coefficients out of range -- not set.\n");
                 return CMD_OK;
              }
          if (    (cfg->Y_shift > 15) || (cfg->C_shift > 15)
               || (cfg->Y_shiftfactor > 4095) || (cfg->C_shiftfactor > 4095) ) {
                 cmd_printf("shift values out of range -- not set.\n");
                 return CMD_OK;
          }
        }
        // Looks good.  Set.
        pip_cfg->sharpen_cfg = cfg;
      }
    }

    cfg = pip_cfg->sharpen_cfg;

    // Display current setting
    if (match_sharpen_config(cfg) != NULL) {
      cmd_printf("sharpen setting for mode %d is %s.\n", mode, match_sharpen_config(cfg));
    } else  {
      cmd_printf("sharpen setting for mode %d is custom:\n", mode);
      cmd_printf("Y:  %5d  %5d  %5d    C:  %5d  %5d  %5d\n",
                 cfg->Y_coeff[0][0], cfg->Y_coeff[0][1], cfg->Y_coeff[0][2],
                 cfg->C_coeff[0][0], cfg->C_coeff[0][1], cfg->C_coeff[0][2]);
      cmd_printf("    %5d  %5d  %5d        %5d  %5d  %5d\n",
                 cfg->Y_coeff[1][0], cfg->Y_coeff[1][1], cfg->Y_coeff[1][2],
                 cfg->C_coeff[1][0], cfg->C_coeff[1][1], cfg->C_coeff[1][2]);
      cmd_printf("    %5d  %5d  %5d        %5d  %5d  %5d\n",
                 cfg->Y_coeff[2][0], cfg->Y_coeff[2][1], cfg->Y_coeff[2][2],
                 cfg->C_coeff[2][0], cfg->C_coeff[2][1], cfg->C_coeff[2][2]);
      cmd_printf("    fill %02x shift %01x/%03x        fill %02x shift %01x/%03x\n",
                 cfg->fill_val_Y, cfg->Y_shift, cfg->Y_shiftfactor,
                 cfg->fill_val_C, cfg->C_shift, cfg->C_shiftfactor);
    }
    return CMD_OK;
}
#endif

// There are lots of technical issues with this, but I think it works more or
// less as intended.  The main problem is that we're applying gamma to the end
// result instead of to a reflectance curve earlier in the process.
#define N_GAMMA_LUTS 20
static const char *pq_cmd_gamma_desc = "adjust the pip PA LUT for a given gamma";
static const char *pq_cmd_gamma_usage = "[mode #] linear|<gamma> [+/-<shift>]";
static const char *pq_cmd_gamma_notes = "Higher numbers will be lighter.  Gamma must be 0.1-10.0.  Shift should be -1.0-1.0.  Examples:   'pq gamma linear' sets a linear LUT.   'pq gamma 1.0' sets the LUT back to the original.   'pq gamma 1.7' applies a gamma curve of 1.7, making the result lighter.   'pq gamma 0.7 -.1' applies a curve of 0.7 (darker) and a linear lightness.";
static int         pq_cmd_gamma_cb(int argc, char *argv[])
{
    int mode;
    int argn = 1;
    static uint32_t* orig_lut[N_GAMMA_LUTS];
    static uint32_t custom_lut[1023];
    double gamma = 1.0;
    double shift = 0.0;
    int i;
    pip_cfg_t* pip_cfg;

    mode = find_pip_mode(argc, argv, &argn);

    if (mode < 0) {
        cmd_printf("Mode must be forced or specified.\n");
        return CMD_OK;
    }
    ASSERT( pip_is_mode_supported(mode) );
    pip_cfg_reference_get(mode, &pip_cfg);

    // Change setting if requested
    if (argn < argc) {
      if (strcmp(argv[argn], "linear") == 0) {
        gamma = 0.0;
      } else {
        gamma = atof(argv[argn]);
        if ((gamma < 0.1) || (gamma > 10.0)) {
          cmd_printf("gamma value out of range -- not set.\n");
          return CMD_OK;
        }
        if ( ((argc-argn) == 2) &&
             ((argv[argn+1][0] == '+') || (argv[argn+1][0] == '-')) ) {
          shift = atof(argv[argn+1]);
        }
       }
    } else {
      // TODO: For now require a gamma
      return CMD_USAGE_ERROR;
    }
    ASSERT(mode < N_GAMMA_LUTS);

    // TODO: Just changing LUT4 for now
    // TODO: All modes are sharing a single custom LUT

    if (orig_lut[mode] == 0)
       orig_lut[mode] = (uint32_t*) pip_cfg->onedlut4;
    pip_cfg->onedlut4 = (uint8_t *) custom_lut;

    if (gamma == 0.0) {
      for (i = 0; i < 1023; i++) {
        if (i > 255) {
          custom_lut[i] = 0.0;
        } else {
          custom_lut[i] = (uint32_t) (1023.0 * (255.0-(float)i)/255.0 + 0.5);
        }
      }
      cmd_printf("LUT set to linear for mode %d '%s'\n",
                 mode, pip_mode_name_get(mode) );
    } else {
      for (i = 0; i < 1023; i++) {
        uint32_t* old_lut = orig_lut[mode];
        double fval = (double)old_lut[i] / 1023.0;
        // Apply the requested gamma
        fval = pow(fval, gamma);
        fval += shift;
        // Clip to 0.0 - 1.0.
        if (fval > 1.0)  fval = 1.0;
        if (fval < 0.0)  fval = 0.0;
        // Program the LUT
        custom_lut[i] = (uint32_t) (fval * 1023.0 + 0.5);
      }
      cmd_printf("LUT set to gamma %.2f + %.2f for mode %d '%s'\n",
                 gamma, shift, mode, pip_mode_name_get(mode) );
    }

    return CMD_OK;
}

/* Note this duplicates the functionality of the 'pcode' command */
static const char *pq_cmd_pcode_desc = "get or set the pcodes";
static const char *pq_cmd_pcode_usage = "[<bits> <code> ...]";
static const char *pq_cmd_pcode_notes = "bits must be 1, 2, or 4.\n  1 code is required for 1-bit, 3 codes for 2-bit, 15 codes for 4-bit.\n  Codes are specified as 0x.. where .. is a hex number from 00 to 3f.";
static int         pq_cmd_pcode_cb(int argc, char *argv[])
{
    int bits;
    uint32_t c1 = 0;
    uint32_t c2 = 0;
    uint32_t c3 = 0;
    uint32_t c4 = 0;

    if ( (argc < 1) || (argc == 2) )
      return CMD_USAGE_ERROR;

    if (argc > 1) {
      bits = atoi(argv[1]);
      if ( ((bits != 1) && (bits != 2) && (bits != 4)) ||
           ((bits == 1) && (argc != (2+ 1))) ||
           ((bits == 2) && (argc != (2+ 3))) ||
           ((bits == 4) && (argc != (2+15))) )
        return CMD_USAGE_ERROR;

      if (bits == 1) {
        ASSERT(argc == (2+1));
        c1 = strtol( argv[2], (char**)NULL, 16);
        if (c1 > 0x3f)
          return CMD_USAGE_ERROR;
        engine_set_video_pcode_1Bit(true, c1);
      } else if (bits == 2) {
        ASSERT(argc == (2+3));
        c1 = strtol( argv[2], (char**)NULL, 16);
        c2 = strtol( argv[3], (char**)NULL, 16);
        c3 = strtol( argv[4], (char**)NULL, 16);
        if ( (c1 > 0x3f) || (c2 > 0x3f) || (c3 > 0x3f) )
          return CMD_USAGE_ERROR;
        engine_set_video_pcode_2Bit(true, c1, c2, c3);
      } else {
        int i;
        uint32_t c;
        ASSERT(argc == (2+15));
        for (i = 1; i < 16; i++) {
          c = strtol( argv[2+i-1], (char**)NULL, 16);
          if (c > 0x3f)
            return CMD_USAGE_ERROR;
          switch (i) {
                    case 1: case  2: case  3:  c1 |= (c << (8*(i- 0)));  break;
            case 4: case 5: case  6: case  7:  c2 |= (c << (8*(i- 4)));  break;
            case 8: case 9: case 10: case 11:  c3 |= (c << (8*(i- 8)));  break;
            default:                           c4 |= (c << (8*(i-12)));  break;
          }
          engine_set_video_pcode_4Bit(true, c1, c2, c3, c4);
        }
      }
    }

    engine_get_video_pcode_1Bit(true, &c1);
    cmd_printf("1-bit pcodes: 00 %02x\n", c1 & 0x3f);

    engine_get_video_pcode_2Bit(true, &c1, &c2, &c3);
    cmd_printf("2-bit pcodes: 00 %02x %02x %02x\n", c1 & 0x3f, c2 & 0x3f, c3 & 0x3f);

    engine_get_video_pcode_4Bit(true, &c1, &c2, &c3, &c4);
    cmd_printf("4-bit pcodes: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
      (c1 >> 0) & 0x3f, (c1 >> 8) & 0x3f, (c1 >> 16) & 0x3f, (c1 >> 24) & 0x3f,
      (c2 >> 0) & 0x3f, (c2 >> 8) & 0x3f, (c2 >> 16) & 0x3f, (c2 >> 24) & 0x3f,
      (c3 >> 0) & 0x3f, (c3 >> 8) & 0x3f, (c3 >> 16) & 0x3f, (c3 >> 24) & 0x3f,
      (c4 >> 0) & 0x3f, (c4 >> 8) & 0x3f, (c4 >> 16) & 0x3f, (c4 >> 24) & 0x3f);

    return CMD_OK;
}


int __attribute__((weak)) main_pip_grayfile_adaptor( int argc, char *argv[] )
{
    cmd_printf("not included in link %s\n", __FUNCTION__);
    return 0;
}


error_type_t pip_pqcmd_init(void)
{
    int retcode;

    // Register cmds
    retcode = cmd_register_cmd( "pq",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    ASSERT( CMD_OK == retcode );


    retcode = cmd_register_subcmd( "pq",
                                   "file_adaptor",
				   "file_adaptor //directory//", 
				   "run pip with input from directory of pgm files",
				   NULL,
                                   main_pip_grayfile_adaptor );
    ASSERT( CMD_OK == retcode );


    retcode = cmd_register_subcmd( "pq",
                                   "force_color",
                                   pq_cmd_force_color_desc,
                                   pq_cmd_force_color_usage,
                                   NULL,
                                   pq_cmd_force_color_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pq",
                                   "modes",
                                   pq_cmd_modes_desc,
                                   pq_cmd_modes_usage,
                                   pq_cmd_modes_notes,
                                   pq_cmd_modes_cb );
    ASSERT( CMD_OK == retcode );
    retcode = cmd_register_subcmd( "pq",
                                   "mode",
                                   pq_cmd_mode_desc,
                                   pq_cmd_mode_usage,
                                   pq_cmd_mode_notes,
                                   pq_cmd_mode_cb );
    ASSERT( CMD_OK == retcode );
#if ASIC_REV >= ASIC_REV_B0 && ASIC_gemstone
    retcode = cmd_register_subcmd( "pq",
                                   "sharpen",
                                   pq_cmd_sharpen_desc,
                                   pq_cmd_sharpen_usage,
                                   pq_cmd_sharpen_notes,
                                   pq_cmd_sharpen_cb );
    ASSERT( CMD_OK == retcode );
#endif
    retcode = cmd_register_subcmd( "pq",
                                   "gamma",
                                   pq_cmd_gamma_desc,
                                   pq_cmd_gamma_usage,
                                   pq_cmd_gamma_notes,
                                   pq_cmd_gamma_cb );
    ASSERT( CMD_OK == retcode );
    retcode = cmd_register_subcmd( "pq",
                                   "pcode",
                                   pq_cmd_pcode_desc,
                                   pq_cmd_pcode_usage,
                                   pq_cmd_pcode_notes,
                                   pq_cmd_pcode_cb );
    ASSERT( CMD_OK == retcode );

    return OK;
}
