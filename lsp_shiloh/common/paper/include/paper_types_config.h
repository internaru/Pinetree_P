/*
 * ========================================================= 
 * (c) Copyright 2009-2010 Marvell International Ltd. 
 *
 *               Marvell Confidential
 * ==========================================================
 */
/**
 * \file paper_types_config.h
 *
 * \brief Paper types
 *
 */

#ifndef PAPERTYPES_CONFIG_H
#define PAPERTYPES_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

//paper types
typedef enum
{
//these match defines from wingdi.h
    MEDIATYPE_ANY          = 0,     //first three are defined in wingdi
    MEDIATYPE_PLAIN_PAPER  = 1,     ///< Plain 75-95g
                                    ///< Plain 75-110g
                                    ///< Plain 75-125g
    MEDIATYPE_TRANSPARENCY = 2,     ///< Color Transparancy

    //user defined types   
    MEDIATYPE_BOND         = 257,   ///< Bond     
    MEDIATYPE_CARDSTOCK    = 258,   
    MEDIATYPE_ENVELOPE     = 259,   ///< Envelope
    MEDIATYPE_HEAVY1       = 260,   ///< Heavy 111-125g
    MEDIATYPE_LABEL        = 261,   ///< Labels
    MEDIATYPE_ROUGHPAPER   = 262,   ///< Rough   
    MEDIATYPE_THINPAPER    = 263,  
    MEDIATYPE_LIGHTPAPER   = 264,   ///< Light 60-74g
    MEDIATYPE_GLOSSY1      = 268,   ///< light glossy  
    MEDIATYPE_GLOSSY2      = 269,   ///< glossy  
    MEDIATYPE_GLOSSY3      = 270,   ///< heavy glossy  
    MEDIATYPE_VELLUM       = 273,
    MEDIATYPE_GLOSSFILM    = 276,   ///< Tough Paper
    MEDIATYPE_PREMIUMCOVER = 277,   ///< premium cover  200g/m2
    MEDIATYPE_PREMIUMPHOTO = 278,
    MEDIATYPE_ENVELOPE_10  = 279,
    MEDIATYPE_ARCHIVE      = 280,
    MEDIATYPE_PLAIN75_89G                   = 281,  ///< dstokes
    MEDIATYPE_MIDWEIGHT_96_110G             = 282,  ///< Mid-Weight 96-110g
    MEDIATYPE_EXTRA_HEAVY                   = 283,  ///< Extra Heavy 126-175g
    MEDIATYPE_HEAVY_GLOSSY                  = 284,  ///< Heavy Glossy 111-125g
    MEDIATYPE_EXTRA_HEAVY_GLOSSY            = 285,  ///< Extra-Heavy Glossy 126-175g
    MEDIATYPE_CARD_GLOSSY                   = 286,  ///< Card Glossy 176-220g
    MEDIATYPE_HEAVY_ENVELOPE                = 287,  ///< Heavy Envelope
    MEDIATYPE_MIDWEIGHT_GLOSSY              = 288,  ///< Mid-Weight Glossy 96-110g
    MEDIATYPE_HEAVY_ROUGH                   = 289,  ///< Heavy Rough
    MEDIATYPE_PRESENTATION          = 300,
    MEDIATYPE_LIGHT_MATTE_COATED    = 301,
    MEDIATYPE_MEDIUM_MATTE_COATED   = 302,
    MEDIATYPE_HEAVY_MATTE_INKJET    = 303,
    MEDIATYPE_HEAVY_GLOSSY_INKJET   = 304,
    MEDIATYPE_RECYCLED_INKJET       = 305,
    MEDIATYPE_HP_ECOSMART_LITE      = 306,
    MEDIATYPE_HP_LASERJET_90        = 307,
    MEDIATYPE_HP_COLOR_LASER_105    = 308,
    MEDIATYPE_HP_PREMIUM_120        = 309,
    MEDIATYPE_HP_MATTE_160          = 310,
    MEDIATYPE_HP_MATTE_COVER_200    = 311,
    MEDIATYPE_HP_MATTE_PHOTO_200    = 312,
    MEDIATYPE_HP_GLOSSY_130         = 313,
    MEDIATYPE_HP_GLOSSY_160         = 314,
    MEDIATYPE_HP_TRIFOLD_GLOSSY     = 315,
    MEDIATYPE_HP_GLOSS_PHOTO_220    = 316,
    MEDIATYPE_HP_INTERMEDIATE       = 317,   // 85-95g
    MEDIATYPE_HP_GLOSS_220          = 318,
    MEDIATYPE_HP_GLOSS_220_FAST     = 319,

    MEDIATYPE_COLORED      = 512,   ///< Colored
    MEDIATYPE_LETTERHEAD   = 513,   ///< Letterhead
    MEDIATYPE_PREPRINTED   = 514,   ///< Preprinted
    MEDIATYPE_PREPUNCHED   = 515,   ///< Prepunched
    MEDIATYPE_RECYCLED     = 516,   ///< Recycled
    MEDIATYPE_UNKNOWN      = 600    ///< same as 'ANY'
}mediatype_t;

typedef struct
{
	int Weight;
    int Texture;
}paperfeature_t;

typedef struct
{
    paperfeature_t PlainPaper;
    paperfeature_t TransParency;
    paperfeature_t PaperLabel;
    paperfeature_t VinylLabel;
    paperfeature_t CardStock;
    paperfeature_t Envelope;
    paperfeature_t PostCard;
    paperfeature_t Recycled;
}papertype_t;


#ifdef __cplusplus
}
#endif

#endif
