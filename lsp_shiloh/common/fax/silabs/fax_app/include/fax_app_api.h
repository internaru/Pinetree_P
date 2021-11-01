/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

/**\file fax_app_api.h
 *
 * \brief The file defines the fax app API's.  This app provides the ability to send and 
 *  receive faxes.  It also provides status via a standard status object.
 *
 * \note Fax app provides an interface to the UI to send/receive fax data.  The ifc is 
 * designed to minimize interaction from the UI.  All flow control and data handling
 * is performed by this application.  The UI simply launches jobs and watches status. 
 * 
 * All top level fax job related functionality is provided by this API including:
 * - Job Launch
 * - Job cancel
 * - Job status
 *
 * Major use cases 
 * - Send a color JPEG fax from the scanner, either flatbed or ADF. 
 * - Send a grayscale JPEG fax from the scanner, either flatbed or ADF. 
 * - Send a mono (MMR, MR, or MH) encoded image from the scanner, either flatbed or ADF.
 * - Receive and print a mono, grayscale, or color fax. 
 * - Reprint a mono fax only.  Reprint of color and grayscale is not supported.
 */
#ifndef __FAX_APP_H__
#define __FAX_APP_H__

#include <stdint.h>
#include "faxer_api.h"
#include "fax_app_status.h"
#include "fax_tiff_io.h" /* for resolution */
#include "paper_size_config.h"

/**
 * \brief Fax data source specification.  
*/
typedef enum
{
  FAX_SRC_SCANNER_AUTO, /*!< The scanner source, ADF or flatbed, is auto detected.  If 
                             media is detected in the ADF, that will be the source.  Else, 
                             the flatbed will be the source. */
//  FAX_SRC_SCANNER_ADF,  /* not supported at this time */
//  FAX_SRC_SCANNER_FLATBED, /* not supported at this time */

  FAX_SRC_FILE,         /*!< To specify a file for sending.  The file must be in the correct
                             format and obey all fax specific guidelines since it is not modified
                             before sending */ 

  FAX_SRC_EXTERNAL,     /*!< To specify an external source that will be generated and provided externaly, 
                             e.g. PC Fax data */
}  fax_app_src_t;

/**
 * \brief Fax data type specification 
*/
typedef enum
{
  FAX_TYPE_BW,    /*!< MMR. MR, or MH encoded mono image */
  FAX_TYPE_COLOR, /*!< JPEG color */
  FAX_TYPE_GRAY,  /*!< JPEG grayscale  */
} fax_app_type_t;

typedef enum
{
  FAX_INPUT_FORMAT_GRAY,
  FAX_INPUT_FORMAT_RGB,
  FAX_INPUT_FORMAT_XBGR,
}
fax_app_input_data_format_t;


/**
 * \brief Supported brightness levels
*/
#define FAX_APP_BRIGHTNESS_MIN      (1)
#define FAX_APP_BRIGHTNESS_STANDARD (5)
#define FAX_APP_BRIGHTNESS_MAX      (10)
typedef unsigned char fax_app_brightness_t;

/**
 * \brief Supported brightness levels
 *
 *   FAX_TYPE_STANDARD  
 *      Mono - 200x100 bi-level
 *      JPEG - 200x200
 *   FAX_TYPE_FINE      
 *      Mono - 200x200 bi-level
 *      JPEG - 200x200 
 *   FAX_TYPE_VERY_FINE - 
 *      MONO - 200x400 bi-level
 *      JPEG - 200x200
 *   FAX_TYPE_PHOTO 
 *      MONO - 300x300 half-tones
 *      JPEG - 300x300
 */
typedef FAX_TYPES fax_app_resolution_t;

/**
 * \brief MAX length of strings passed in
 */
#define FAX_APP_MAX_DIGITS_LENGTH  (50)
#define FAX_APP_MAX_FILE_NAME_LENGTH (50)

/**
 * \brief Job ticket containing settings
 */
typedef struct 
{
  char digits[FAX_APP_MAX_DIGITS_LENGTH];  /*!<Number to dial */
  fax_app_type_t type;                     /*!<Fax type, eg. BW or Color */
  fax_app_src_t  src;                      /*!<Fax data source, eg. Scanner or file */
  bool           scan_before_sending;      /*!<Scan the data before dialing, forced to false for 
                                               grayscale and color JPEG send (note that this limitation
                                               can easily be removed if desired) */
  fax_app_brightness_t brightness;         /*!<Brighness level, BRIGHTNESS_MIN to BRIGHTNESS_MAX */
  fax_app_resolution_t resolution;         /*!<Resolution to scan and send data with */
  char src_filename[FAX_APP_MAX_FILE_NAME_LENGTH]; /*!<File to send, only needed if src is file */
  //date_time_t time_to_send; /* 0 (or less than current) to send now */
} fax_app_ticket_t;

/**
 * \brief External page data information
 */
typedef void (*fax_app_add_page_cb)(uint8_t *data_ptr);

typedef struct
{
  uint8_t * data;         /*!<Raw page data.  8bpp gray for mono and grayscale, 24bpp RGB for color */
  uint32_t pix_width; 
  uint32_t pix_height; 
  fax_app_input_data_format_t pix_format;
  bool complete;          /*!<True if this is the last page of the doc, else false */
  fax_app_add_page_cb cb; /*!<Callback to enable freeing memory when we are done with the page */
} fax_app_ext_page_data_t;

/**
 * \brief Return values for the function calls below */
typedef enum
{
  FAX_APP_RES_SUCCESS = 0,
  FAX_APP_RES_ERROR,  /* generic error */
  FAX_APP_RES_INVALID_PARAMS,
  FAX_APP_RES_LINE_UNAVAILABLE,
  FAX_APP_RES_FAX_ACTIVE,
  FAX_APP_RES_NO_FAX_TO_RECEIVE,
  FAX_APP_RES_FILE_DOES_NOT_EXIST,
} fax_app_res_t;



/**\brief Send a fax.
 *
 * Global settings will be read and used upon start of job, no job queueing.  This call is non blocking.
 *
 * \param[in] fax_ticket pointer to the fax job parameters defined by fax_app_ticket_t structure. 
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_send_fax(fax_app_ticket_t *fax_ticket);

/**\brief Send a new page of data for EXTERNAL sourced jobs only 
 *
 * \param[in] data Page data information
 *
 * Note that the data must be RAW.  Mono data must be 8bpp gray, it will be
 * converted to 1bpp.
 */
fax_app_res_t fax_app_send_add_raw_page( fax_app_ext_page_data_t *data );

/**\brief Trigger the receive and print of a fax manualy.  
 *
 * This will trigger a recieve and print of the fax.  The phone could be off hook or on hook. 
 * In either case the device will attempt to receive a fax.  Auto receive is handled by 
 * the configuration settings.  This call is non blocking.
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_receive_fax(void); /* manual receive, auto is managed using the settings */

/**\brief Trigger reprint of a fax
 *
 * This will trigger the reprint of the fax with the job id sepcified.  This is the job id
 * that is stored in the log.  An error will return if the file does not exist within the log.
 *
 * \param[in] job_id The job id of the fax to reprint.  Note that the job id is contained
 *                   in the log data.
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_reprint_fax(uint32_t job_id);

/**\brief Get a list of reprintable faxes
 *
 * \param[in] list Memory where the list will be stored
 *
 * \param[in] max_items Max number of elements to copy to the list
 *
 * \param[in] num_items_ret Number of items returned
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_reprint_get_list(FAXER_ACTIVITY_ENTRY *list, uint32_t max_items, 
                                       uint32_t *num_items_ret );

/**\brief Check if last fax received was printed or not
 *
 * \param[in] printed True or false
 *
 * This could be used after powerup to check if the last fax received was printed.
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_check_if_last_fax_printed(bool *printed);


/**\brief Cancel a fax send or receive.
 *
 * This will cancel all active fax jobs.  This call is non blocking.
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_cancel(void);

/***************************************************************
* Test modes
***************************************************************/
/**\brief Disable printing of the fax job for testing
 *
 * This will disable printing.  It will also create a /debug 
 * directory where it will move all fax receive files to.
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_test_mode_no_print(bool on);

/**************************************************************
* Print app settings 
**************************************************************/
/**\brief Fax print media size.  This setting is not stored in non volatile
 *  storage.
 *
 * \param[in] setting setting to get
 * \param[in] size media size to print on
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_print_set_media_size(mediasize_t media_size);

/**\brief Set the fax print stamp (footer) to on or off.
 * This setting is not stored in non volatile storage.
 *
 * \param[in] stamp on flag 1=use stamp 0=don't use stamp.
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_print_set_stamp_faxes(bool on);

/**\brief Get the fax print stamp (footer) value.
 *
 * \param[in] none
 *
 * \return The bool value of the stamp flag
 */
bool fax_app_print_get_stamp_faxes(void);


/***************************************************************
* Non Job Specific Settings 
****************************************************************/

/**\brief Fax setting get
 *
 * \param[in] setting setting to get
 * \param[in] buf pointer to buf to hold value
 * \param[in] buf_size sizeof buf in bytes
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_get_setting(FAXER_SETTING setting, void *buf, int buf_size);

/**\brief Fax setting set
 *
 * \param[in] setting setting to set
 * \param[in] val set to this value
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_set_setting(FAXER_SETTING setting, void *val);

/**\brief Restore settings to factory defaults 
 *
 * \param[in] setting setting to get
 * \param[out] val to be set to value 
 *
 * \return Result code as defined by fax_app_res_t
 */
fax_app_res_t fax_app_restore_settings(void);

#endif /* __FAX_APP_API__ */

