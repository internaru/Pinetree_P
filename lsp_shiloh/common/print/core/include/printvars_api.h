/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                      Marvell Confidential
 * ============================================================================
 */

/** 
 * \file printvars_api.h
 * 
 * \brief Public interface for the print component specific variables.
 * 
 * This file contains the public interface for the print component specific variables.
 * 
 **/

#ifndef INC_PRINT_VARS_H
#define INC_PRINT_VARS_H

//--------------------------------------
// Included Files
//--------------------------------------

#include "ATypes.h"
#include "data_access_api.h"
#include "agprint.h"
#include "event_observer.h"
#include "error_types.h"

#ifdef HAVE_RTC
#include "rtc_api.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//bit mask defines for flags in size_type_tray_t struct
#define STT_PAPEROUT   0x00000001
#define STT_WRONGSIZE  0x00000002
#define STT_WRONGTYPE  0x00000004
//--------------------------------------
// API Type Definitions
//--------------------------------------

/**
 * \brief structure for holding tray info
 * used by the printmgr
 */
typedef struct
{
    mediasize_t  size;      ///< The media size needed in the tray
    mediatype_t  type;      ///< The media type needed in the tray
    trayenum_t   tray;        ///< The enum for the tray
    uint32_t     flags;     ///< flags as defined above.
}size_type_tray_t;


/**
 * \brief enum to specify the paper out actions
 */

typedef enum 
{
    PA_WAIT_FOREVER,    ///< When paperout wait for something to happen forever
    PA_OVERRIDE,        ///< When paperout if nothing happens after timeout, print anyway
    PA_CANCEL           ///< When paperout if nothing happens after timeout, cancel the job
} e_PaperActions;

/**
 * \brief paperout action structure
 * 2 paramaters, Action is what to do and is based on the enum above.
 * Timeout applies only to PA_OVERRIDE and PA_CANCEL  Time to wait before 
 * either printing anyway or canceling the job.
 */
typedef struct
{
    e_PaperActions Action;  ///< What to do when paper is out
    UINT32         Timeout; ///< How long to wait for something to happen.

}paperoutaction_t;


/** \brief Print System Log Entry Record
 * Useful for logging system print status events that may need to be recovered
 * over time, such as paper jam, marker low, etc.  */
typedef struct print_log_entry_s
{
    uint32_t page_count; /*!< The total number of pages the system has printed
                           over its lifetime */

    uint32_t code;       /*!< A code representing events such as paper jam,
                           marker low, etc. */

    #ifdef HAVE_RTC
	date_time_t time;      /*!< The time of the log */
    #endif

} print_log_entry_t;


//--------------------------------------
// API Function Declarations
//--------------------------------------

// FUNCTION NAME:  printVarInit
/** 
 * \brief Boot time initialization function for the print variables.
 * 
 * This function is called at system startup after the NVRAM driver and partition manager have been
 * initialized.  It sets all its variables to either default values or previously stored values from
 * NVRAM.
 * 
 * \param[in] initType type of initialization being done:  nvram_init_type_t enum value from dataAccAPI.h 
 * 
 **/ 
 
void printvar_init(void);
/**
 * \brief get and set the status of the print subsystem
 * \param[in] val The status to get or set
 */
error_type_t printvar_get_status(uint32_t *val);
error_type_t printvar_set_status(uint32_t *val);
/**
 * \brief get and set the online status of the print subsystem
 * \param[in] val The on-line status to set/get
 */
error_type_t printvar_get_online(uint32_t *val);
error_type_t printvar_set_online(uint32_t *val);

/**
 * \brief Get and set the current Job page count
 * During a print job this variable has the count of the number of pages that have been printed.
 * \param val The value of the count.
 */
error_type_t printvar_get_currentjob_pgcnt(uint16_t *val);
error_type_t printvar_set_currentjob_pgcnt(const uint16_t *val);

/**
 * \brief Get and set the current Job byte count. 
 * \param[in] val The number of bytes in a job.
 * \returns error_type_t 
 * \retval OK everything worked.
 * \retval FAIL did not work
 */
error_type_t printvar_get_currentjob_bytecount(uint32_t *val);
error_type_t printvar_set_currentjob_bytecount(const uint32_t *val);

/**
 * \brief get and set the doc type of the current job.  
 */

error_type_t printvar_get_doctype(uint32_t *val);
error_type_t printvar_set_doctype(const uint32_t *val);

/**
 * \brief Get and set the jam recovery option for the print subsystem
 * With Jamrecovery set to 0 if a jam occurs the page that jammed will not
 * be reprinted.  With Jamrecovery on the page will be reprinted.  
 * Jamrecovery should be turned off if the print jobs have very complex
 * pages that may not all fit into memory.  With Jamrecovery on the
 * entire page is retained in memory until the page is done when the
 * data is released.
 * \param val Set to 0 to turn off and 1 to turn on
 */
error_type_t printvar_get_jamrecovery(uint8_t *val);
error_type_t printvar_set_jamrecovery(const uint8_t *val);

//-------------------------
// JOBName
//-------------------------

#define F_STRING_LEN 40
/**
 * \brief Get and set the print job name.
 * PJL can name a given print job.  When a job starts and pjl has previously 
 * named a job this variable will hold that name.
 * \param val The job name string
 */

error_type_t printvar_get_jobname(char *val);
error_type_t printvar_set_jobname(const char *val);

/**
 * \brief Get and set the manual duplex flag.
 * If a job is manual duplexed this is set to 1 else
 * it is set to 0
 * \param[in] val The state of manual duplex.
 */
error_type_t printvar_get_manualduplex(uint32_t *val);
error_type_t printvar_set_manualduplex(const uint32_t *val);

/**
 * \brief This is set to the number of pages in the current job.
 * When a job starts this is set to the total number of pages in
 * the job.
 * \param[in] val The number of pages in a job.
 * \returns error_type_t 
 * \retval OK everything worked.
 * \retval FAIL did not work
 */
error_type_t printvar_get_pagesperjob(uint16_t *val);
error_type_t printvar_set_pagesperjob(const uint16_t *val);

/**
 * \brief Get and set the size_type_tray_t structure in the datastore.
 * See the structure def for size_type_tray_t to get info on what this does.
 * \param[in] val A pointer to the structure to fill or write.
 * \returns error_type_t
 * \retval OK everything worked.
 * \retval FAIL did not work
 */

error_type_t printvar_get_sizetypetray(size_type_tray_t *val);
error_type_t printvar_set_sizetypetray(const size_type_tray_t *val);

/**
 * \brief get and set the mediaconfig variable
 * mediacnfg holds the tray configurations for the printer.  The definiation of the
 * array is located in agprint.h.  The array holds the tray media sizes and types
 * plus the default settings for all trays.  This allows the modification of
 * those values.
 * \param val The mediacnfg_t value.
 * \returns error_type_t
 * \retval OK Everything worked
 * \retval FAIL bad
 */
error_type_t printvar_get_trayconfig(mediacnfg_t *val);
error_type_t printvar_set_trayconfig(const mediacnfg_t *val);

error_type_t printvar_set_trayconfig_get_from_DB(void);

/**
 * \brief get and set for the printer_setup_t structure.  
 * This structure is specified in the agprint.h header file.  
 * See that for the documentation.
 * \param setup The structure to save.
 * \returns error_type_t
 * \retval OK everything is ok
 * \retval FAIL everything is bad
 */

error_type_t printvar_get_printersetup(printer_setup_t *setup);
error_type_t printvar_set_printersetup(const printer_setup_t *setup);

/**
 * \brief Set the paper out action to 1 of 3 options shown above.
 * \param[in] val type of paperoutaction_t  
 * \return error_type_t
 * \retval OK worked
 * \retval FAIL didn't work
 */

error_type_t printvar_get_paperoutaction(paperoutaction_t *val);
error_type_t printvar_set_paperoutaction(paperoutaction_t *val);

/**
 * \brief Holds the default paper size
 * When the printer starts this is set to the default paper size for a given
 * region.  Usually is either A4 or letter.
 * \param val The default paper size
 */
error_type_t printvar_get_default_papersize(mediasize_t *val);
error_type_t printvar_set_default_papersize(const mediasize_t *val);


/**
 * \brief Holds the default paper type
 * When the job starts this is set to the default paper type. 
 * \param val The default paper type
 */
error_type_t printvar_get_default_papertype(mediatype_t *val);
error_type_t printvar_set_default_papertype(const mediatype_t val);


/** 
 * \brief Get function for a a log entry.
 * 
 * This function returns the print log entry for the index specified
 * 
 * \param[out] index The print log index entry to return.  The API
 * printGetVarMaxLogEntries() is used to get the maximum number of
 * log entries.
 *
 * \param[out] val pointer to destination for data to be returned
 * 
 * \return error_type_t
 * \retval OK
 * \retval DATA_ACC_ERR_NULL_PTR if incoming pointer is NULL
 * \retval DATA_ACC_ERR_INVALID_VALUE if the index parameter is invalid 
 * 
 **/ 
error_type_t printvar_get_log_entry( uint32_t index, print_log_entry_t * val );


/** 
 * \brief Get maximum number of log entries.
 * 
 * The index indicates an array size, it is 0-based.  
 * To access the last log index, use the data index returned - 1
 * 
 * \param[out] index Pointer to the array size.
 * 
 * \return error_type_t
 * \retval OK
 * \retval DATA_ACC_ERR_NULL_PTR if incoming pointer is NULL
 * 
 **/ 
error_type_t printvar_get_max_logentries( uint32_t * index );


/** 
 * \brief Add a log entry to the print module's log.
 * 
 * \param val The event to log into the print log, this is the "code" 
 * member of the print_log_entry_t.  Note that the page count and other
 * members (if there are any will be filled automatically)
 *
 * \return error_type_t
 * \retval OK
 * \retval DATA_ACC_ERR_STORAGE_DEV_FAILURE if there is a problem with the permanent storage device
 * 
 **/ 
error_type_t printvar_set_logentry( uint32_t val );

trayenum_t get_mismatch_tray(void);
void set_mismatch_tray(trayenum_t tray);

mediasize_t get_mismatch_size(void);
void set_mismatch_size(mediasize_t size);
/** 
 * \brief Get current toner count.
 * 
 * \return uint32_t
 *
 **/
 
int* GetTonerJobByModes(void);
int* GetTonerPrintedByCoverage(void);
int GetFreeMotorOnCount(void);

#if 0
// notification structures
#define printVars_ID 0x44

#define printVars_num_pages_ID 0x45 ///< The unique id for the numpages object.
#define printVars_num_bytes_ID 0x46 ///< The unique id for the numbytes object.

/**
 * \brief the next 3 calls are used for nofification of numpages change.  
 * This happens when the print of a page is complete.  This notifies that
 * that page complete is out of the printer.
 */
/**
 * \brief Attach an observer to the numpages object
 * \param[in] o a valid observer structure pointer
 */
void printvars_numpages_attachobserver(Observer_t *o);
/**
 * \brief Detach a previously attached observer to the numpages object.
 * \param[in] o Pointer to the observer to detach.
 */
void printvars_numpages_detachobserver(Observer_t *o);
/**
 * \brief When notified that a numpages event has occurred, use this to get the number of pages
 * \param[in] s The subject of the object.
 * \returns uint32_t
 * \retval The number of the page that has printed.
 */
uint32_t printvars_get_numpages(Subject_t *s);

#endif

/* Temporary Placeholders for host-print.html */
#define EWS_PRINT_INPUT_TYPE_FACTORY_DEFAULT 1
#define EWS_PRINT_MEDIA_TYPE_FACTORY_DEFAULT 1
#define EWS_PRINT_MEDIA_SIZE_FACTORY_DEFAULT 1

#ifdef __cplusplus
}
#endif

#endif // INC_PRINT_VARS_H

