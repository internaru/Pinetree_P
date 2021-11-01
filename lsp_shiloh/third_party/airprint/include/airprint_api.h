/*
 * ============================================================================
 * (C) Copyright 2014   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** @file airprint_api.h 
 *
 * @brief Contains API definitions for the proprietary Apple AirPrint specification, which
 * is an extension of the PWG IPP specification. 
 * 
 * NOTE: Release of this source file to SDK customers is restricted.
 */

#ifndef AIRPRINT_API_H
#define AIRPRINT_API_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Represents the maximum string length of the string 
 *        describing the print scaling.
 */
#define AIRPRINT_SCALING_STR_LEN 16

/**
 * @name print scaling
 *  
 * Query the corresponding smjob_get_supported_XX() routine to determine the subset of 
 * values supported by the device. 
 */
typedef char airprint_scaling_t;

/**                                                 
 * @brief auto
 */                                                 
#define AIRPRINT_SCALING_AUTO       "auto"

/**                                                 
 * @brief auto-fit
 */                                                 
#define AIRPRINT_SCALING_AUTO_FIT   "auto-fit"

/**                                                 
 * @brief fill
 */                                                 
#define AIRPRINT_SCALING_FILL       "fill"

/**                                                 
 * @brief fit
 */                                                 
#define AIRPRINT_SCALING_FIT        "fit"

/**                                                 
 * @brief none
 */                                                 
#define AIRPRINT_SCALING_NONE       "none"

//@}


/**
 * @brief Represents the maximum string length of the string 
 *        describing the print content optimize.
 */
#define AIRPRINT_CONTENT_OPTIMIZE_STR_LEN 32


/**
 * @name print content optimize
 *  
 * Query the corresponding smjob_get_supported_XX() routine to determine the subset of 
 * values supported by the device. 
 */
typedef char airprint_content_optimize_t;

/**                                                 
 * @brief auto
 */                                                 
#define AIRPRINT_CONTENT_OPTIMIZE_AUTO              "auto"

/**                                                 
 * @brief graphics
 */                                                 
#define AIRPRINT_CONTENT_OPTIMIZE_GRAPHICS          "graphics"

/**                                                 
 * @brief photo
 */                                                 
#define AIRPRINT_CONTENT_OPTIMIZE_PHOTO             "photo"

/**                                                 
 * @brief text
 */                                                 
#define AIRPRINT_CONTENT_OPTIMIZE_TEXT              "text"

/**                                                 
 * @brief text-and-graphics
 */                                                 
#define AIRPRINT_CONTENT_OPTIMIZE_TEXT_GRAPHICS     "text-and-graphics"

//@}



// AirPrint-specific smjob extensions
typedef struct
{
    /** 
     * @brief print scaling to use when printing a document
     */
    airprint_scaling_t       scaling[AIRPRINT_SCALING_STR_LEN];

} airprint_doc_processing_t;


/**
 * @brief Initialize the AirPrint module.
 *
 * @retval SYS_OK Initialization was successful.
 * 
 * @retval SYS_FAIL Initialization failed. 
 *  
 * This routine should be called exactly once following a system startup.
 */
void airprint_init(void);

#endif // AIRPRINT_API_H
