/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file net_wsd_print_config.c
 *
 * \brief This file implements the public configuration API for
 * WSD Print.
 * 
 **/

//--------------------------------------
// Included files
//--------------------------------------
#include "net_wsd_print_config.h"
#include "print_status_codes.h"
#include "lassert.h"

//--------------------------------------
// Local Definitions and Types
//--------------------------------------
#define WSD_SIGNAL_IO_READY_THRESHOLD   1024
#define WSD_RECV_SIZE_MAX               16384
#define WSD_RECV_SIZE_HIGH_THRESHOLD    10240

typedef struct 
{
    mediatype_t type;
    char        *wsd_name;
} net_wsd_mediatype_mapping_t;

typedef struct 
{
    mediasize_t type;
    char        *wsd_name;
} net_wsd_mediasize_mapping_t;

typedef struct 
{
    COLOR   color;
    char    *wsd_name;
} net_wsd_color_mapping_t;

//--------------------------------------
// Global Definitions
//--------------------------------------

net_wsd_mediatype_mapping_t net_wsd_mediatypes [] =
{
    { MEDIATYPE_ANY,                "anySupportedType"          },
    { MEDIATYPE_PLAIN_PAPER,        "stationery",               },
    { MEDIATYPE_TRANSPARENCY,       "transparency"              },
    { MEDIATYPE_LIGHTPAPER,         "stationery-lightweight"    },
    { MEDIATYPE_CARDSTOCK,          "cardstock"                 },
    { MEDIATYPE_HEAVY1,             "stationery-heavyweight"    },
    { MEDIATYPE_LABEL,              "labels"                    },
    { MEDIATYPE_ENVELOPE,           "envelope"                  },
    { MEDIATYPE_PREMIUMPHOTO,       "photographic"              },
    { MEDIATYPE_LETTERHEAD,         "stationery-letterhead"     },
    { MEDIATYPE_PREPRINTED,         "stationery-preprinted"     },
    { MEDIATYPE_PREPUNCHED,         "stationery-prepunched"     },
    { MEDIATYPE_UNKNOWN,            "unknown"                   }
//    { MEDIATYPE_BOND,               "bond"                      },
//    { MEDIATYPE_ROUGHPAPER,         "rough"                     },
//    { MEDIATYPE_GLOSSY1,            "lightGlossy"               },
//    { MEDIATYPE_GLOSSY2,            "midweightGlossy"           },
//    { MEDIATYPE_GLOSSY3,            "heavyGlossy"               },
//    { MEDIATYPE_VELLUM,             "vellum"                    },
//    { MEDIATYPE_GLOSSFILM,          "toughPaper"                },
//    { MEDIATYPE_PREMIUMCOVER,       ""  },
//    { MEDIATYPE_ENVELOPE_10,        ""  },
//    { MEDIATYPE_ARCHIVE,            ""  },
//    { MEDIATYPE_PLAIN75_89G,        ""  },
//    { MEDIATYPE_MIDWEIGHT_96_110G,  ""  },
//    { MEDIATYPE_EXTRA_HEAVY,        ""  },
//    { MEDIATYPE_HEAVY_GLOSSY,       ""  },
//    { MEDIATYPE_EXTRA_HEAVY_GLOSSY, ""  },
//    { MEDIATYPE_CARD_GLOSSY,        ""  },
//    { MEDIATYPE_HEAVY_ENVELOPE,     ""  },
//    { MEDIATYPE_MIDWEIGHT_GLOSSY,   ""  },
//    { MEDIATYPE_HEAVY_ROUGH,        ""  },
//    { MEDIATYPE_COLORED,            ""  },
//    { MEDIATYPE_RECYCLED,           ""  },
};
#define NET_WSD_NUM_MEDIATYPES  ( sizeof( net_wsd_mediatypes ) / sizeof( net_wsd_mediatype_mapping_t ) )

net_wsd_mediasize_mapping_t net_wsd_mediasizes [] =
{
    { MEDIASIZE_LETTER,                 "na_letter_8.5x11in" },
    { MEDIASIZE_ANY,                    "any" },
    { MEDIASIZE_LEGAL,                  "na_legal_8.5x14in" },
    { MEDIASIZE_EXECUTIVE,              "na_executive_7.25x10.5in" },
    { MEDIASIZE_CUSTOM_85x13,           "na_foolscap_8.5x13in" },
    { MEDIASIZE_A4,                     "iso_a4_210x297mm" },
    { MEDIASIZE_A5,                     "iso_a5_148x210mm" },
    { MEDIASIZE_B5,                     "jis_b5_182x257mm" },
    { MEDIASIZE_CUSTOM_16K,             "roc_16k_7.75x10.75in" },
    { MEDIASIZE_ENV_10,                 "na_number-10_4.125x9.5in" },
    { MEDIASIZE_ENV_MONARCH,            "na_monarch_3.875x7.5in" },
    { MEDIASIZE_ENV_C5,                 "iso_c5_162x229mm" },
    { MEDIASIZE_ENV_DL,                 "iso_dl_110x220mm" },
    { MEDIASIZE_ENV_B5,                 "iso_b5_176x250mm" },
    { MEDIASIZE_A6,                     "iso_a6_105x148mm" },
    { MEDIASIZE_JAPANESE_POSTCARD,      "jpn_hagaki_100x148mm" },
    { MEDIASIZE_DBL_JAPANESE_POSTCARD,  "jpn_oufuku_148x200mm" },
    { MEDIASIZE_CUSTOM_16K_184_260,     "custom_16k_184x260mm" },
    { MEDIASIZE_CUSTOM_16K_195_270,     "custom_16k_195x270mm" },
    { MEDIASIZE_CUSTOM_4x6,             "na_index-4x6_4x6in" },
    { MEDIASIZE_CUSTOM_5x8,             "na_index-5x8_5x8in" },
    { MEDIASIZE_CUSTOM_10x15cm,         "custom_index-card_100x150mm" },
//    {MEDIASIZE_CUSTOM_POSTCARD_JIS,     "" },
//    {MEDIASIZE_CUSTOM_DBL_POSTCARD_JIS, "" },
//    { MEDIASIZE_ENV_PERSONAL,           "" },
};
#define NET_WSD_NUM_MEDIASIZES  ( sizeof( net_wsd_mediasizes ) / sizeof( net_wsd_mediasize_mapping_t ) )

net_wsd_color_mapping_t net_wsd_colors [] = 
{
    {   COLOR_BLACK,    "Black"     },
    {   COLOR_CYAN,     "Cyan"      },
    {   COLOR_YELLOW,   "Yellow"    },
    {   COLOR_MAGENTA,  "Magenta"   },
};
#define NET_WSD_NUM_COLORS  ( sizeof( net_wsd_colors ) / sizeof( net_wsd_color_mapping_t ) ) 

wsd_get_printer_status_condition_options_t wsd_get_printer_status_condition_options[ ] = 
{
    // Currently not handling - 
    // CoverOpen, 
    // InterlockOpen, 
    // InputTrayMediaSizeChange, 
    // InputTrayMediaTypeChange, 
    // InputTraySupplyLow
    // OutputTrayMissing
    // OutputTrayAlmostFull
    // FuserUnderTemperature
    // FuserOverTemperature
    // ConsumableLow
    // ConsumableEmpty
    // WasteReceptacleAlmostFull
    // WasteReceptacleFull
    { STATUS_PAPER_JAM_INPUT,   wprt__ComponentGroupBaseType__InputBin,     "None",     wprt__ConditionNameBaseType__Jam,                   wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_PAPER_JAM_TRAY1,   wprt__ComponentGroupBaseType__InputBin,     "Tray1",    wprt__ConditionNameBaseType__Jam,                   wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_PAPER_JAM_TRAY2,   wprt__ComponentGroupBaseType__InputBin,     "Tray2",    wprt__ConditionNameBaseType__Jam,                   wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_PAPER_JAM_TRAY3,   wprt__ComponentGroupBaseType__InputBin,     "Tray3",    wprt__ConditionNameBaseType__Jam,                   wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_PAPER_JAM_OUTPUT,  wprt__ComponentGroupBaseType__OutputBin,    "None",     wprt__ConditionNameBaseType__Jam,                   wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_DOOROPEN_FRONT,    wprt__ComponentGroupBaseType__CoverDoor,    "None",     wprt__ConditionNameBaseType__DoorOpen,              wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_DOOROPEN_FD,       wprt__ComponentGroupBaseType__CoverDoor,    "None",     wprt__ConditionNameBaseType__DoorOpen,              wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_TRAY1_OPEN,        wprt__ComponentGroupBaseType__InputBin,     "Tray1",    wprt__ConditionNameBaseType__InputTrayMissing,      wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_TRAY2_OPEN,        wprt__ComponentGroupBaseType__InputBin,     "Tray2",    wprt__ConditionNameBaseType__InputTrayMissing,      wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_PAPER_OUT_TRAY1,   wprt__ComponentGroupBaseType__InputBin,     "Tray1",    wprt__ConditionNameBaseType__InputTraySupplyEmpty,  wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_PAPER_OUT_TRAY2,   wprt__ComponentGroupBaseType__InputBin,     "Tray2",    wprt__ConditionNameBaseType__InputTraySupplyEmpty,  wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_PAPER_OUT_TRAY3,   wprt__ComponentGroupBaseType__InputBin,     "Tray3",    wprt__ConditionNameBaseType__InputTraySupplyEmpty,  wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_PAPER_OUT_TRAY4,   wprt__ComponentGroupBaseType__InputBin,     "Tray4",    wprt__ConditionNameBaseType__InputTraySupplyEmpty,  wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_INPUT_LOAD_TRAY_1, wprt__ComponentGroupBaseType__InputBin,     "Tray1",    wprt__ConditionNameBaseType__InputTraySupplyEmpty,  wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_INPUT_LOAD_TRAY_2, wprt__ComponentGroupBaseType__InputBin,     "Tray2",    wprt__ConditionNameBaseType__InputTraySupplyEmpty,  wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_INPUT_LOAD_TRAY_3, wprt__ComponentGroupBaseType__InputBin,     "Tray3",    wprt__ConditionNameBaseType__InputTraySupplyEmpty,  wprt__ConditionSeverityBaseType__Critical   },
    { STATUS_INPUT_LOAD_TRAY_4, wprt__ComponentGroupBaseType__InputBin,     "Tray4",    wprt__ConditionNameBaseType__InputTraySupplyEmpty,  wprt__ConditionSeverityBaseType__Critical   },
    //{ STATUS_BINFULL,           wprt__ComponentGroupBaseType__OutputBin,    "None",     wprt__ConditionNameBaseType__OutputTrayFull,        wprt__ConditionSeverityBaseType__Critical   },
};
#define WSD_NUM_STATUS_CONDITION_OPTIONS    ( sizeof( wsd_get_printer_status_condition_options ) / sizeof( wsd_get_printer_status_condition_options_t ) )


//--------------------------------------
// Local Function Declarations
//--------------------------------------


//--------------------------------------
// Local Function Definitions
//--------------------------------------
void net_wsd_print_config( net_wsd_print_config_t *config )
{
    config->signal_io_ready_threshold   = WSD_SIGNAL_IO_READY_THRESHOLD;
    config->max_recv_bytes_to_buffer    = WSD_RECV_SIZE_MAX;
    config->signal_to_recv_threshold    = WSD_RECV_SIZE_HIGH_THRESHOLD;
}

error_type_t net_wsd_map_mediatype( mediatype_t input, char **output)
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < NET_WSD_NUM_MEDIATYPES; i++ )
    {
        if ( net_wsd_mediatypes[ i ].type == input )
        {
            *output = net_wsd_mediatypes[ i ].wsd_name;
            return OK;
        }
    }

    return FAIL;
}

error_type_t net_wsd_map_mediasize( mediasize_t input, char **output)
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < NET_WSD_NUM_MEDIASIZES; i++ )
    {
        if ( net_wsd_mediasizes[ i ].type == input )
        {
            *output = net_wsd_mediasizes[ i ].wsd_name;
            return OK;
        }
    }

    return FAIL;
}

error_type_t net_wsd_map_color( COLOR color, char **output )
{
    uint32_t i;

    *output = NULL;
    for ( i = 0; i < NET_WSD_NUM_COLORS; i++ )
    {
        if ( net_wsd_colors[ i ].color == color )
        {
            *output = net_wsd_colors[ i ].wsd_name;
            return OK;
        }
    }

    return FAIL;
} 

error_type_t net_wsd_get_printer_status_condition_table( wsd_get_printer_status_condition_options_t **table, uint32_t *elements )
{
    *table = wsd_get_printer_status_condition_options;
    *elements = WSD_NUM_STATUS_CONDITION_OPTIONS;
    return OK;
}


