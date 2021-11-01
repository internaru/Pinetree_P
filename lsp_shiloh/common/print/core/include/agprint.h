/*
 *
 * ============================================================================
 * Copyright (c) 2006-2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file agprint.h
 *
 * \brief
 *
 */

#ifndef _PRINT_H_
#define _PRINT_H_

#include "platform.h"
#include <stdint.h>
#include <stdbool.h>
#include "paper_size_api.h"
#include "paper_types_config.h"
#include "cppenumops.h"

// NOTE:
// Do not use extern "C" here, as C++ is used in this file.

/**
 * \brief Rotation values, in 90-degree increments, progressing
 *        CounterClockWise (CCW) in order.
 */
typedef enum
{
    e_NoRotate,
    e_Rotate90,
    e_Rotate180,
    e_Rotate270,
} ROTATE_DIR;



/** Input sources */
typedef enum
{
    INPUT_UNKNOWN = -1,
    INPUT_FIRST = 1,
    INPUT_UPPER = INPUT_FIRST,
    INPUT_ONLYONE = INPUT_FIRST,
    INPUT_LOWER,
    INPUT_MIDDLE,
    INPUT_MANUAL,
    INPUT_ENVELOPE,
    INPUT_ENVMANUAL,
    INPUT_AUTO,
    INPUT_TRACTOR,
    INPUT_SMALLFMT,
    INPUT_LARGEFMT,
    INPUT_LARGECAPACITY,
    INPUT_CASSETTE,
    INPUT_DUPLEXER,
    INPUT_FORMSOURCE,
    INPUT_LAST = INPUT_FORMSOURCE,
}INPUTTYPE;

/** Output destinations - CA can't find any output defined in wingdi.h
 * for now all our devices only have a single output so won't worry about it
 */
typedef enum
{
    OUTPUT_UNKNOWN = -1,
    OUTPUT_AUTO = 0,
    OUTPUT_FACEDOWNTRAY,
    OUTPUT_FDTRAY = OUTPUT_FACEDOWNTRAY, /*!< FaceDown output tray/bin: the default *** OUTPUT_FDTRAY DEPRECATED *** */
    OUTPUT_FACEUPTRAY,
    OUTPUT_DUPLEXER,  /*!< Duplexer unit */
}OUTPUTTYPE;


/* Generic defines for colors. the engine and video need to make sure and
 * convert from generic to actual color indices  when interacting with the 
 * print engine.
 */

typedef enum
{
    COLOR_BLACK = 0,
    COLOR_CYAN,
    COLOR_YELLOW,
    COLOR_MAGENTA,
    COLOR_FIRST = COLOR_BLACK,
    COLOR_LAST = COLOR_MAGENTA
} COLOR;

DECLARE_ENUM_OPERATORS(COLOR);

// MAX_COLORS is a define instead of an enum inside COLOR because many switch
// cases are done on colors and making MAX_COLORS part of the enum causes lots
// of unhandled case warnings in the compiler. -jrs
#define MAX_COLORS 4

typedef enum
{
    e_Mono = 0,
    e_Color = 1,
} page_color_mode_t;

typedef enum
{
    PAGE_ORIENT_PORTRAIT = 0,
    PAGE_ORIENT_LANDSCAPE
} page_orientation_t;

/** User configured media size and type for each tray
 * saved in nvram */
typedef struct
{
    mediatype_t mediaType[NUM_INPUTS];  ///< Use TRAYENUM enum to index
    mediasize_t mediaSize[NUM_INPUTS];
    mediatype_t DefmediaType;  
    mediasize_t DefmediaSize;
    papertype_t paperType;
    int defaultSourceTray;
    int traySettingPriority;
    int mediaPrintDarkness;
    int trayMatchOptionA4LT;
    int tonerSaveMode;
}mediacnfg_t;

/** Tray properties to customize the tray for a platform
 *is a priority manual feed tray (always print from it if paper present)
 */
#define INPUTPROPERTY_PRIORITY 0x00000001
/** Tray is an all-purpose tray. This means is is both a cassette and a manual feed
 * tray. The inputType should be set to INPUT_MANUAL. The print manager will treat
 * it as a manual feed tray if a page requests the manual tray else it will treat
 * it as a cassette
 */
#define INPUTPROPERTY_ALLPURPOSE    0x00000002 

/** Any printer that has a manual feed tray that is single feed should set this
 * property for that particula input tray.  This will later be used to determine
 * if a delay is needed between sheet feeds to prevent paper jams on these single
 * sheet input trays.
 */
#define INPUTPROPERTY_SINGLE_FEED     0x00000004

/** Do to the fact that different models of printers consider different trays for
 * Manual duplex we will use this property bit to signify which tray should be used
 * for manual duplexing.
 */
#define INPUTPROPERTY_MANUAL_DUPLEX   0x00000008

/** Point to the phyical tray for this input tray. this is used for trays
 * than can be both manual feed and a cassette.
 */
#define INPUTPROPERTY_PHYSICAL_TRAY_MASK    0x000F0000
#define INPUTPROPERTY_PHYSICAL_TRAY_1       0x00010000
#define INPUTPROPERTY_PHYSICAL_TRAY_2       0x00020000
#define INPUTPROPERTY_PHYSICAL_TRAY_3       0x00040000
#define INPUTPROPERTY_PHYSICAL_TRAY_4       0x00080000
#define INPUTPROPERTY_MULTIPURPOSE          0x00100000  // \todo mdb made this up

/** Current state of each tray, stored in data store */
typedef struct
{
    INPUTTYPE   inputType;     ///< Cassette, manual, duplexer
    uint32_t    properties;    ///< TBD
    bool        paperPresent;  ///< Is there paper in the tray
    bool        inputPresent;  ///< Is the input currently present (for optional cassettes)
    uint32_t    numSizes;
    uint32_t    numTypes;
    mediasize_t supportedSizes[NUM_MEDIA_SIZES];  ///< Supported media sizes
    mediatype_t supportedTypes[NUM_MEDIA_TYPES];  ///< Supported media types

}INPUTTRAY;

typedef struct
{
    OUTPUTTYPE  outputType;
    uint32_t    properties;     //TBD
    uint32_t    maxSheets;      ///< Maximum capacity, in sheets of media.
    bool        isFull;           ///< Is the output currently full, if there is such a sensor.
    bool        outputPresent;    ///<is the output currently present (for duplex)
}OUTPUTTRAY;

/** The tray enum is used internally for indexing into our print structure
 * the parsers have to convert from the driver supplied INPUTTYPE to these.
 */
typedef enum
{
    TRAY_1 = 0,	// MPT
    TRAY_2,		// Default Tray1
    TRAY_3,		// Default Tray2
    TRAY_4,		// Tray3 (Bank, Option1)
    TRAY_5,		// Tray4 (Bank, Option2)
    TRAY_6,
    TRAY_7,
    TRAY_8,
    TRAY_ANY,
    TRAY_NONE
}trayenum_t;
DECLARE_ENUM_OPERATORS(trayenum_t);

// TODO: this really isn't a good location for the pixel count defines
// need consider moving to a opaque structure with accessors in consumables code
// do this for now, but need to fix.
///> The type of action that caused the pixel counts to occur.
typedef enum pixel_count_type_s
{
    PIXCOUNT_MONO_PAGE,     // a monochrome page was printed
    PIXCOUNT_COLOR_PAGE,    // a color page was printed
    PIXCOUNT_TONER_PURGE    // a toner purge occurred
} pixel_count_type_t;

// PixelCount values
typedef struct
{
    uint32_t pixelCntPerPage[NUM_MARKING_COLOR_CHANNELS]; // total pixel weight per page
    uint32_t nonWhitePixelCntPerPage[NUM_MARKING_COLOR_CHANNELS]; // total unmarked pixels per page
    uint32_t totalPagePixels;       // total available pixels in page, page pixel size
    pixel_count_type_t   type;      ///< color, mono or purge.
} PIXELCNT;


/** Printer Setup Option defines - used in options field of printer_setup_t*/
#define PRINTER_AUTO_CASCADE_ENABLED    0x00000001  ///< If set PrintMgr will look for a new tray if the given tray won't work.
#define PRINTER_HAS_FRONTPANEL          0x00000002
#define PRINTER_NO_PAPER_SENSOR         0x00000004  ///< Set this bit if printer doesn't have input tray paper present sensors.
#define PRINTER_HAS_BIN_FULL_SENSOR     0x00000008  ///< Refers to output bin(s).


/*
 * Changing this structure will require a re-compile of jobmgr and printmgr.
 */
#define MAX_MEDIA_TRAYS 5
typedef struct
{
    uint32_t options;                    ///< customization bits - auto tray select etc
    INPUTTRAY inputTrays[MAX_MEDIA_TRAYS];
    OUTPUTTRAY outputTrays[MAX_MEDIA_TRAYS];
}printer_setup_t;


#endif
