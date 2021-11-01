/*
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file paper_size_api.h 
 *
 * \brief This file contains the paper size public API.
 *
 */

#ifndef PAPERSIZE_API_H
#define PAPERSIZE_API_H

#include <stdint.h>
#include <stdbool.h>
#include "paper_size_config.h"        ///< this includes the mediasize_t 
#include "string_mgr_api.h"
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
   \brief Paper size units used when specifying a custom media size.
*/ 
typedef enum  
{
    PAPER_UNITS_600DPI,   ///< 600 dots per inch
    PAPER_UNITS_1200DPI,  ///< 1200 dots per inch
    PAPER_UNITS_MM,       ///< milimeter
    PAPER_UNITS_MMx10,    ///< tenths of a millimeter.
    PAPER_UNITS_MMx100,   ///< hundredths of a millimeter.
    PAPER_UNITS_MMx1000,  ///< thousandths of a millimeter = micrometers, abbreviated um, one-millionth of a meter.
    PAPER_UNITS_INCH,     ///< inches
    PAPER_UNITS_INx100,   ///< hundredths of an inch.
    PAPER_UNITS_INx1000,  ///< thousandths of an inch.
    PAPER_UNITS_INx10000, ///< ten-thousandths of an inch.
    PAPER_UNITS_72DPI,    ///< 72dpi postscript points
} paper_size_units_t;

#define MMx100_PER_HALF_INCH    1270
#define MMx100_PER_INCH         2540

/**
 * \brief Macro for converting measurements from millimeters x 100 to pixels at
 *        the given resolution in dots per inch. Input, output, and intermediate
 *        calculations all are unsigned integers. The result is rounded.
 *
 * \param mmx100 (unsigned int) millimeters x 100 measurement to be converted to
 *        dots at the given resolution.
 * \param dpi (unsigned int) Resolution, in dots per inch. Usually 600 or 1200.
 *
 *  Value returned is an unsigned integer, and is rounded.
 */
#define MMx100_TO_DOTS(mmx100, dpi) (((unsigned int)(mmx100)*(unsigned int)(dpi) + MMx100_PER_HALF_INCH)/MMx100_PER_INCH)

/**
  *
  * \brief Initialize any paper size internals
  *
  */
void paper_size_init( void );


/**
 * \brief Get the size of a given paper in mm.
 * This takes the paper size enum, looks up that paper, and returns the height and
 * and width in hundredth of a millimeter units  1/100mm
 * \param[in] media_size The media size specified in papersizeconfig.h
 * \param[out] width The width of the media
 * \param[out] height The height of the media
 * \return error_type_t standard error type return
 * \retval OK Paper was found and width and height are correct.
 * \retval FAIL Paper type was not found.
 */
error_type_t paper_get_media_size(mediasize_t media_size, uint32_t *width, uint32_t *height);


/**
 * \brief Get the enum definition of paper size given the x and 
 * y dimensions. 
 * \param[in] width The width of the media
 * \param[in] height The height of the media 
 * \param[in] units  The unit of measurement 
 * \param[out] media_size The media size specified in 
 *       papersizeconfig.h
 * \return error_type_t standard error type return
 * \retval OK Paper was found and width and height are correct.
 * \retval FAIL Paper size match to the given dimensions was not
 *         found.
 */
error_type_t paper_get_media_size_enum( float width, float height, paper_size_units_t units, mediasize_t * media_size);

/**
 * \brief Returns the number of papers we support in the system.
 * \returns Number of papers supported by the system
 */
uint32_t paper_get_num_media_sizes(void);

/** 
 * \brief Returns the pjl code that corresponds to the media size given.
 * This Finds the media size and returns the pjl code that represents that media.
 * \returns uint16_t 
 * \retval The pjl code corresponding to MediaSize
 */
uint16_t paper_get_pjl_code(mediasize_t media_Size);

/** 
 * \brief Returns id string for this paper.
 * This id string is used by pjl, internal pages, etc.
 * \param[in] media_size The enum that specifies the paper size.
 * \returns char *
 * \retval NULL, no id string found
 * \retval pointer A pointer to the id string
 */
string_mgr_code_t paper_get_id_string_token(mediasize_t media_size);

/**
 * \brief Get an next entry in the paper size table. 
 * Given an index into the table, this returns the mediasize_t
 * entry for that entry in the table.  This is thread safe so
 * will work no matter how many people call it.
 * \param[in] index Index into the table
 * \returns mediasize_t
 * \retval The media size
 * \retval -1 End of the table 
 */
mediasize_t paper_get_entry(uint32_t index);

bool paper_engine_supports_mediasize( mediasize_t mediaSize );

const char * paper_get_pjl_name(mediasize_t mediaSize);

typedef struct paper_common_pjl_table_s
{
	mediasize_t mediasize;
	uint16_t pjl_code;
	char *pjl_name;
} paper_common_pjl_table_t;

#ifdef __cplusplus
}
#endif

#endif
