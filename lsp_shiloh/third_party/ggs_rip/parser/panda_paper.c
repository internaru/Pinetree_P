/* Copyright (C) 2011-2013 Global Graphics Software Ltd. All rights reserved.
*
* This example is provided on an "as is" basis and without
* warranty of any kind. Global Graphics Software Ltd. does not
* warrant or make any representations regarding the use or results
* of use of this example.
*
* Portions Copyright (c) 2011-2013 Marvell International, Ltd. All Rights Reserved
*/

#include <stdint.h>
#include "paper_size_config.h"
#include "paper_types_config.h"
#include "string_mgr_api.h"
#include "panda_api.h"
#include "paper_size_api.h"
#include "paper_types_api.h"


/*! \brief Stores paper information.
*/
typedef struct {
    /* PDL definitions */
    int             mediasize;           /**< enum key mediasize_t */	
    int             nTopUnprintable;     /**< Unprintable distance at top (microinches). */
    int             nBottomUnprintable;  /**< Unprintable distance at bottom (microinches). */
    int             nLeftUnprintable;    /**< Unprintable distance on left (microinches). */
    int             nRightUnprintable;   /**< Unprintable distance on right (microinches). */
    int             nTopLogicalPage;     /**< Physical to logical page distance at top (microinches). */
    int             nBottomLogicalPage;  /**< Physical to logical page distance at bottom (microinches). */
    int             nLeftLogicalPage;    /**< Physical to logical page distance on left (microinches). */
    int             nRightLogicalPage;   /**< Physical to logical page distance on right (microinches). */
    EMS_ePCL5size   ePCL5size;           /**< PCL 5 Paper size ID. */
    EMS_ePCLXLsize  ePCLXLsize;          /**< PCL XL Paper size ID. */
    char            szPSName[32];        /**< Postscript name. */
    char            szXLName[32];        /**< XL Paper size name . */
} engine_media_sizes_pdl_ext_t;

/* Paper Size Name, Size (mm), Size (inch), PCL5e, PCL6, PostScript, Note, paper_size_config.h */
/* A4 210.0  297.0  8.27  11.69  26 2 [595 842] default(1) MEDIASIZE_A4 */
/* B5 182.0  257.0  7.17  10.12  45 11 [516 729]  MEDIASIZE_B5 */
/* A5_SEF 148.0  210.0  5.83  8.27  25 16 [420 595] Equal to A5 MEDIASIZE_A5 */
/* A5_LEF 210.0  148.0  8.27  5.83  ? ? ? Only GDI MEDIASIZE_A5_ROTATED */
/* B6 128.0  182.0  5.04  7.17  20 18 [363 516]  MEDIASIZE_B6_JIS */
/* A6 105.0  148.0  4.13  5.83  2001 17 [298 420]  MEDIASIZE_A6 */
/* Legal 215.9  355.6  8.50  14.00  3 1 [612 1008]  MEDIASIZE_LEGAL */
/* Letter 215.9  279.4  8.50  11.00  2 0 [612 792] default(2) MEDIASIZE_LETTER */
/* Statement 139.7  215.9  5.50  8.50  2008 30 [396 612]  MEDIASIZE_STATEMENT */
/* Exective 184.2  266.7  7.25  10.50  1 3 [522 756]  MEDIASIZE_EXECUTIVE */
/* US Folio 215.9  330.2  8.50  13.00  2012 32 [595 936]  MEDIASIZE_FOLIO */
/* Eng Quatro 203.2  254.0  8.00  10.00  2013 36 [576 720]  english quatro or 8x10 photo paper */
/* Government Legal 209.6  355.6  8.25  14.00  2031 34 [595 1006]      non-standard name to size mapping  */
/* Government Letter 203.2  266.7  8.00  10.50  2032 37 [576 756] non-standard name to size mapping */
/* Postcard 100 x 148 100.0  148.0  3.94  5.83  71 14 [283 420]  MEDIASIZE_JAPANESE_POSTCARD */
/* Postcard 148 x 200 148.0  200.0  5.83  7.87  72 15 [566 420]  MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED */
/* Postcard 200 x 148 200.0  148.0  4.125  9.50  ? ? ? Only GDI MEDIASIZE_DBL_JAPANESE_POSTCARD */
/* Com9 Env 98.4  225.4  3.875  8.875  2023 38 [279 640]  MEDIASIZE_ENV_9 */
/* Com10 Env 104.8  241.3  4.125  9.50  81 6 [297 684]  MEDIASIZE_ENV_10 */
/* Monarch Env 98.4  190.5  3.875  7.50  80 7 [279 540]  MEDIASIZE_ENV_MONARCH */
/* C5 Env 162.0  229.0  6.38  9.02  91 8 [459 649]  MEDIASIZE_ENV_C5 */
/* C6 Env 114.0  162.0  4.49  6.38  2022 35 [323 459]  MEDIASIZE_ENV_C6 */
/* DL Env 110.0  220.0  4.33  8.66  90 9 [312 624]  MEDIASIZE_ENV_DL */

/* UNPRINTABLE is unprintable area of the engine in micro-inches. e.g. 1/6 inch... 166667  
   This controls the amount of raster data blanked from each side (before stride length padding).
   Top, Bottom, Left, Right
*/
#define UNPRINTABLE  166667, 166667, 166667, 166667  /* 1/6 inch all sides */

/* PCL Logical page figure is defined by the PCL spec and should not be need to changed
   It is expressed as pixels at 300dpi - e.g for 60PX = (60/300 *1000000) table values are in micro-inches
   This sets the amount of white raster data before raster trimming.
   Top, Bottom, Left, Right
 */
#define LOGICAL_PCL_PAGE     0, 0, 250000, 250000
#define LOGICAL_PCL_PAGE59PX 0, 0, 196667, 196667
#define LOGICAL_PCL_PAGE60PX 0, 0, 200000, 200000
#define LOGICAL_PCL_PAGE71PX 0, 0, 236667, 236667
#define LOGICAL_PCL_PAGE75PX 0, 0, 250000, 250000

/* SDK media id,           unprintable t,b,l,r, pcl logical t,b,l,r */
/*                         all pdls,            pcl */
panda_paper_pdl_logical_table_t panda_paper_pdl_logical_table[] = {
{ MEDIASIZE_A4,            UNPRINTABLE, LOGICAL_PCL_PAGE71PX }, 
{ MEDIASIZE_B5,            UNPRINTABLE, LOGICAL_PCL_PAGE59PX }, 
{ MEDIASIZE_A5,            UNPRINTABLE, LOGICAL_PCL_PAGE59PX }, 
{ MEDIASIZE_B6_JIS,        UNPRINTABLE, LOGICAL_PCL_PAGE59PX }, 
{ MEDIASIZE_A6,            UNPRINTABLE, LOGICAL_PCL_PAGE59PX }, 
{ MEDIASIZE_LEGAL,         UNPRINTABLE, LOGICAL_PCL_PAGE60PX }, 
{ MEDIASIZE_LETTER,        UNPRINTABLE, LOGICAL_PCL_PAGE60PX }, 
{ MEDIASIZE_STATEMENT,     UNPRINTABLE, LOGICAL_PCL_PAGE60PX }, 
{ MEDIASIZE_EXECUTIVE,     UNPRINTABLE, LOGICAL_PCL_PAGE75PX }, 
{ MEDIASIZE_CUSTOM_85x13,  UNPRINTABLE, LOGICAL_PCL_PAGE75PX }, 
{ MEDIASIZE_CUSTOM_8x10,   UNPRINTABLE, LOGICAL_PCL_PAGE75PX }, 
{ MEDIASIZE_CUSTOM_825x14, UNPRINTABLE, LOGICAL_PCL_PAGE75PX }, 
{ MEDIASIZE_CUSTOM_8x105,  UNPRINTABLE, LOGICAL_PCL_PAGE75PX }, 
{ MEDIASIZE_JAPANESE_POSTCARD,             UNPRINTABLE, LOGICAL_PCL_PAGE71PX }, 
{ MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED, UNPRINTABLE, LOGICAL_PCL_PAGE71PX }, 
{ MEDIASIZE_ENV_9,         UNPRINTABLE, LOGICAL_PCL_PAGE71PX },
{ MEDIASIZE_ENV_10,        UNPRINTABLE, LOGICAL_PCL_PAGE71PX },
{ MEDIASIZE_ENV_MONARCH,   UNPRINTABLE, LOGICAL_PCL_PAGE71PX },
{ MEDIASIZE_ENV_C5,        UNPRINTABLE, LOGICAL_PCL_PAGE71PX },
{ MEDIASIZE_ENV_C6,        UNPRINTABLE, LOGICAL_PCL_PAGE71PX },
{ MEDIASIZE_ENV_DL,        UNPRINTABLE, LOGICAL_PCL_PAGE71PX },
{ MEDIASIZE_CUSTOM_USER,   UNPRINTABLE, LOGICAL_PCL_PAGE71PX } 
}; 

// complete table of all media sizes 
panda_paper_pdl_name_table_t panda_paper_pdl_name_table[] = {
// mediatype         pcl id   pxl id         postscript      pjlname
{ MEDIASIZE_A4,          26,       2,        "/A4",          "(A4)"          },
{ MEDIASIZE_B5,          45,      11,        "/B5",          "(B5)"          },  // todo wrong string names? JIS?
{ MEDIASIZE_A5,          25,      16,        "/A5",          "(A5)"          },
/* A5_R gdi only */
{ MEDIASIZE_B6_JIS,      20,      18,        "/B6",          "(B6)"          },  // todo wrong string names? JIS?
{ MEDIASIZE_A6,        2001,      17,        "/A6",          "(A6)"          },
{ MEDIASIZE_LEGAL,        3,       1,        "/Legal",       "(LEGAL)"       },
{ MEDIASIZE_LETTER,       2,       0,        "/Letter",      "(LETTER)"      },
{ MEDIASIZE_STATEMENT, 2008,      30,        "/Statement",   "(STATEMENT)"   },
{ MEDIASIZE_EXECUTIVE,    1,       3,        "/Executive",   "(EXEC)"        },
{ MEDIASIZE_CUSTOM_85x13,   2012, 32,        "/825X13",      "(825X13)"      },  // todo: alias for gov legal 
{ MEDIASIZE_CUSTOM_8x10,    2013, 36,        "/EngQuatro",   "(ENGQUATRO)"   },
{ MEDIASIZE_CUSTOM_825x14,  2031, 34,        "/G_LG",        "(G_LG)"        },  
{ MEDIASIZE_CUSTOM_8x105,   2032, 37,        "/G_LT",        "(G_LT)"        },
{ MEDIASIZE_JAPANESE_POSTCARD,             71, 14, "/JpnPostcard",    "(JPNPOSTCARD)" },    
{ MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED, 72, 15, "/JpnDblPostcard", "(JPNDBLPOSTCARD)" },
/* Postcard 200 x 148  GDI Only */
{ MEDIASIZE_ENV_9,     2023,      38,        "/COM9_ENV",    "(COM9_ENV)"    },
{ MEDIASIZE_ENV_10,      81,       6,        "/COM10_ENV",   "(COM10_ENV)"   },
{ MEDIASIZE_ENV_MONARCH, 80,       7,        "/MONARCH_ENV", "(MONARCH_ENV)" },
{ MEDIASIZE_ENV_C5,      91,       8,        "/C5",          "(C5)"          },
{ MEDIASIZE_ENV_C6,    2022,      35,        "/C6",          "(C6)"          },
{ MEDIASIZE_ENV_DL,      90,       9,        "/DL",          "(DL)"          },
{ MEDIASIZE_CUSTOM_USER, 101,     -1,        "/Custom",      "(CUSTOMSIZE)"  },
};


#if 1

typedef struct panda_mediatype_pdl_name_table_s {
    int nMediaTypeId;
    char szPcl5Name[32]; 
    char szPclXLName[32]; 
    char szPSName[32]; 
} panda_mediatype_pdl_name_table_t;

// complete table of all media sizes 
// \todo Define/write structure panda_mediatype_pdl_name_table_t
panda_mediatype_pdl_name_table_t panda_mediatype_pdl_name_table[] = {
// id                     PCL5e,          PCL6,            PostScript      //PJL command
{ MEDIATYPE_ANY,          "Autoselect",   "Autoselect",    "Autoselect"},   //@PJL SET MEDIATYPE=AUTOSELECT
{ MEDIATYPE_PLAIN_PAPER,  "Plain",        "Plain",         "Plain"},        //@PJL SET MEDIATYPE=PLAIN
{ MEDIATYPE_HEAVY1,       "Thick",        "Thick",         "Thick"},        //@PJL SET MEDIATYPE=THICK
{ MEDIATYPE_EXTRA_HEAVY,  "Very Thick",   "Very Thick",    "Very Thick"},   //@PJL SET MEDIATYPE=VERYTHICK
{ MEDIATYPE_LIGHTPAPER,   "Thin",         "Thin",          "Thin"},         //@PJL SET MEDIATYPE=THIN
{ MEDIATYPE_TRANSPARENCY, "Transparency", "Transparency",  "Transparency"}, //@PJL SET MEDIATYPE=TRANSPARENCY
{ MEDIATYPE_LABEL,        "Paper Label",  "Paper Label",   "Paper Label"},  //@PJL SET MEDIATYPE=PAPERLABEL
{ MEDIATYPE_HEAVY_GLOSSY, "Vinyl Label",  "Vinyl Label",   "Vinyl Label"},  //@PJL SET MEDIATYPE=VINYLLABEL
{ MEDIATYPE_ENVELOPE,     "Envelope",     "Envelope",      "Envelope"},     //@PJL SET MEDIATYPE=ENVELOPE
{ MEDIATYPE_CARDSTOCK,    "Cardstock",    "Cardstock",     "Cardstock"},    //@PJL SET MEDIATYPE=CARDSTOCK
{ MEDIATYPE_CARD_GLOSSY,  "Postcard",     "Postcard",      "Postcard"},     //@PJL SET MEDIATYPE=POSTCARD
{ MEDIATYPE_RECYCLED,     "Recycled",     "Recycled",      "Recycled"},     //@PJL SET MEDIATYPE=RECYCLED
};
#endif
#if 0
// complete table of all tray enums 
// \todo Define/write structure panda_tray_pdl_enum_table_t
// \todo What are the actual values?
panda_tray_pdl_enum_table_t panda_tray_pdl_enum_table[] = {
// id                     PCL5e, PCL6,  PostScript
{ MEDIATYPE_TRAY1,        1,     1,     1},
{ MEDIATYPE_TRAY2,        2,     2,     2},
{ MEDIATYPE_TRAY3,        3,     3,     3},
{ MEDIATYPE_TRAY4,        4,     4,     4},
};
#endif

const panda_paper_pdl_logical_table_t * paper_map_mediaSize_to_panda_pdl_logical( int mediaSize )
{
	int i;
	int num_elements = sizeof(panda_paper_pdl_logical_table) / sizeof(panda_paper_pdl_logical_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( panda_paper_pdl_logical_table[i].nMediaSizeId == mediaSize ) {
			return &panda_paper_pdl_logical_table[i];
		}
	}
	return 0; // not found.
}

const panda_paper_pdl_logical_table_t * paper_get_panda_pdl_logical( int mediaSize )
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_map_mediaSize_to_panda_pdl_logical( mediaSize );
	}
	return 0;
}

EMS_ePCL5size paper_map_mediaSize_to_EMS_ePCL5size( int mediaSize )
{
	int i;
	int num_elements = sizeof(panda_paper_pdl_name_table) / sizeof(panda_paper_pdl_name_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( panda_paper_pdl_name_table[i].nMediaSizeId == mediaSize ) {
			return panda_paper_pdl_name_table[i].nPCL5PaperSizeID;
		}
	}
	return 0; // not found.
}

EMS_ePCL5size paper_get_EMS_ePCL5size( int mediaSize)
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_map_mediaSize_to_EMS_ePCL5size( mediaSize );
	}
	return 0;
}

EMS_ePCLXLsize paper_map_mediaSize_to_EMS_ePCLXLsize( int mediaSize )
{
	int i;
	int num_elements = sizeof(panda_paper_pdl_name_table) / sizeof(panda_paper_pdl_name_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( panda_paper_pdl_name_table[i].nMediaSizeId == mediaSize ) {
			return panda_paper_pdl_name_table[i].nPCLXLPaperSizeID;
		}
	}
	return 0; // not found.
}

EMS_ePCLXLsize paper_get_EMS_ePCLXLsize( int mediaSize)
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_map_mediaSize_to_EMS_ePCLXLsize( mediaSize );
	}
	return 0;
}




const panda_paper_pdl_name_table_t * paper_map_mediaSize_to_panda_pdl_name( int mediaSize )
{
	int i;
	int num_elements = sizeof(panda_paper_pdl_name_table) / sizeof(panda_paper_pdl_name_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( panda_paper_pdl_name_table[i].nMediaSizeId == mediaSize ) {
			return &panda_paper_pdl_name_table[i];
		}
	}
	return 0; // not found.
}

const panda_paper_pdl_name_table_t * paper_get_panda_pdl_name( int mediaSize )
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_map_mediaSize_to_panda_pdl_name( mediaSize );
	}
	return 0;
}


const char * paper_map_mediaSize_to_szPSName( int mediaSize )
{
	int i;
	int num_elements = sizeof(panda_paper_pdl_name_table) / sizeof(panda_paper_pdl_name_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( panda_paper_pdl_name_table[i].nMediaSizeId == mediaSize ) {
			return panda_paper_pdl_name_table[i].szPSName;
		}
	}
	return 0; // not found.
}

const char * paper_get_szPSName( int mediaSize )
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_map_mediaSize_to_szPSName( mediaSize );
	}
	return 0;
}

const char * paper_map_mediaSize_to_szXLName( int mediaSize )
{
	int i;
	int num_elements = sizeof(panda_paper_pdl_name_table) / sizeof(panda_paper_pdl_name_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( panda_paper_pdl_name_table[i].nMediaSizeId == mediaSize ) {
			return panda_paper_pdl_name_table[i].szXLName;
		}
	}
	return 0; // not found.
}

const char * paper_get_szXLName( int mediaSize )
{
	if ( paper_engine_supports_mediasize( mediaSize ) ) {
		return paper_map_mediaSize_to_szXLName( mediaSize );
	}
	return 0;
}

// \todo
#if 1

const char * paper_map_mediaType_to_szXLName( int mediaType )
{
	int i;
// Marvel Bug : panda_mediatype_pdl_name_table			2014.04.09	by Juny
	int num_elements = sizeof(panda_paper_pdl_name_table) / sizeof(panda_paper_pdl_name_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( panda_mediatype_pdl_name_table[i].nMediaTypeId == mediaType ) {
			return panda_mediatype_pdl_name_table[i].szPclXLName;
		}
	}
/*
	int num_elements = sizeof(panda_mediatype_pdl_name_table) / sizeof(panda_mediatype_pdl_name_table_t);
	for (i = 0; i < num_elements; i++ ) {
		if ( panda_mediatype_pdl_name_table[i].nMediaTypeId == mediaType ) {
			return panda_mediatype_pdl_name_table[i].szXLName;
		}
	}
*/
	return 0; // not found.
}

const char * mediatype_get_szXLName( int mediaType )
{
	if ( paper_engine_supports_mediatype( mediaType ) ) {
		return paper_map_mediaType_to_szXLName( mediaType );
	}
	return 0;
}
#endif

