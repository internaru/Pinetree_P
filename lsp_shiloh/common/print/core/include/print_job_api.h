/*
 *
 * =====================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * =====================================================================
 *
 */

/** 
 * \file print_job_api.h
 * 
 * \brief Interface for creating print jobs and sending pages.
 * 
 * The Print Job API is used to create print jobs and pages and send the
 * data associated with the pages to the print subsystem.  The API is
 * to be used once permission to create a print job has been granted by the
 * system job manager.  The system job manger does this by sending the
 * message MSG_ACKRECIPE to the parser or generator using the Print Job API.
 * 
 * Typical usage of this API is as shown in the following pseudo code.
 * 
 * {
 *     my_job = print_job_construct(...)
 *     <set desired job options; collation, duplex, etc.>
 *     print_job_start(my_job, job_dest_module)
 * 
 *     while(have_pages)
 *     {
 *         my_page = print_page_construct(my_job);
 *         <set desired page options; media_size, input source, etc.>
 * 
 *         print_page_start(my_page);
 * 
 *         for(each print plane)
 *         {
 *             print_plane_start(my_page, plane_color);
 *        
 *             while(have_print_plane_data)
 *             {
 *                 print_plane_add_data(my_page, plane_color, ...);
 *             }
 * 
 *             print_plane_end(my_page, plane_color);
 *         }
 * 
 *         print_page_end(my_page, NULL);
 *         print_page_free(my_page);
 *     }
 * 
 *     print_job_end(my_job);
 * }
 * 
 * Note that the following functions must be called for each page:
 * 
 *   print_page_set_dpi_x(...);
 *   print_page_set_dpi_y(...);
 *   print_page_set_raster_x(...);
 *   print_page_set_raster_y(...);
 *   print_page_set_video_x(...);
 *   print_page_set_video_y(...);
 *   print_page_set_output_bpp(...);
 * 
 * In addition, for color devices the following function must be called for each page:
 *   print_page_set_color_mode(...);
 *
 **/

#ifndef PRINT_JOB_API_H
#define PRINT_JOB_API_H

#include <stdint.h>
#include <stdbool.h>
#include "mlimiter.h"
#include "print_job_types.h"
#include "paper_size_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
// Print Job API object declarations


// structure describing a print_job object from the point of view of the print_job_api.
struct print_job_s
{
    JOB_DATA* job_data;         ///< pointer to the job_data object; sent to job_dest and ultimately to the print job manager at job_start time
    mlimiter_t* limiter;        ///< memory limiter used by the job
    MODULE_ID job_dest;         ///< module id of the module that will receive all job message send from this api; typically the print job manager
    bool job_started;           ///< flag indicating that the job has been started

    bool manual_duplex_started;   ///< Indicates that we have started printing the second side pages of a manual duplex job.
    uint32_t current_page_number; ///< The number of the current page being created.  Starts from 1.

    // shadow job_data settings; used to allow pages to inherit job default settings
    uint16_t duplex;            ///< duplex options
    bool duplex_rotate_backside;///< rotate the backside of duplex pages 
    bool collate;               ///< collation on/off
    uint32_t num_copies;        ///< number of copies of the job
    DOCTYPE doc_type;           ///< document type of the job
    trayenum_t DefaultInputSource;  ///< if no input tray is specified for the page use this tray
    uint32_t job_id;        ///< to keep track of SJM's job id
};

/// opaque type of the print job api's job object
typedef struct print_job_s print_job_t;


// structure describing a print_page object from the point of view of the print_job_api.
struct print_page_s
{
    PAGE_DATA *page_data;   ///< pointer to the page_data object; sent to job_dest and ultimately to the print job manager at page_start time
    print_job_t* my_job;    ///< pointer to the job object that this page is associated with
};

/// opaque type of the print job api's page object
typedef struct print_page_s print_page_t;



/////////////////////////////////////////////////////////////////////////////////////////////////
// 
// JOB specific portion of the API
// 
/////////////////////////////////////////////////////////////////////////////////////////////////

// Typical Usage:
//   print_job_construct(...)
//   set desired job options i.e. collation, duplex, etc
//   print_job_start(...)
//   send pages
//   print_job_end(...)

/** 
 * \brief Construct a new print job object and initialize the job with default settings.
 * 
 * \param limiter - Pointer to the memory limiter to be used by this job.
 * 
 * \param job_pipe - Pointer to the pipe used by this job.
 * 
 * \return print_job_t* a pointer to the newly constructed job object
 * 
 * This routine constructs a job object and initializes the job with default settings.
 * The job defaults set the job to a standard simplex non-collated single copy job.
 * When a job is complete it must be freed.  The job is freed with a call to any one of
 * print_job_end, print_job_abort or print_job_cancel.  If the job has not been started
 * the free should be done by calling print_job_cancel.
 * 
 **/
print_job_t* print_job_construct(mlimiter_t* limiter, IOReg* job_pipe);


/** 
 * \brief Notify the next component in a print job chain that a job is starting.
 * 
 * \param p_job - Pointer to the job returned by print_job_construct.
 * 
 * \param job_dest - Module ID of the job destination.  All messages regarding
 * this job will be sent to the specified destination.
 * 
 * This routine notifies the module specified in the job_dest paramater that the
 * job is starting.  Typically the job_dest parameter is the print job manager,
 * but it may specify an intermediate destination if further processing of
 * the data is required.
 * 
 * Note that all job related settings must be specified prior
 * to calling calling print_job_start.
 * 
 **/
void print_job_start(print_job_t* p_job, MODULE_ID job_dest);


/** 
 * \brief Notify the print job manager that the job generation has completed.
 * 
 * \param p_job - Pointer to the job to end.
 * 
 * \return NULL indicating that the job was freed
 * 
 * This routine should be called on all normal job terminations.  The
 * print job manager is notified that the job is complete from the stand point
 * of job data generation.  The print job api specific memory allocations are
 * freed.  
 * 
 * Note that calling this routine does not halt the job.  It indicates that
 * no more data will be sent on the job and that the job completed normally from
 * the data generation standpoint.  The pages sent during the job may still be
 * in the process of being printed and will be completed as normal.
 * 
 * \warning The print_job_t object specfied by the p_job parameter is freed
 * by this routine and is no longer valid.
 * 
 **/
print_job_t* print_job_end(print_job_t* p_job);


/** 
 * \brief Notify the print job manager to abort a job in progress.
 * 
 * \param p_job - Pointer to the job to be aborted.
 * 
 * This routine should be called on all abnormal job terminations.  The
 * print job manager is notified that the job is aborted from the stand point
 * of job data generation.  The print job api specific memory allocations are
 * freed.  
 * 
 * Note that calling this routine does not halt the job.  It indicates that
 * no more data will be sent on the job and that the job completed abnormally from
 * the data generation standpoint.  The pages sent during the job may still be
 * in the process of being printed.  Any pages that are valid will still be
 * printed, but partially constructed pages will be discarded.
 * 
 * This routine is typically used to abort a job in progress when bad data is
 * received.
 * 
 * \warning The print_job_t object specfied by the p_job parameter is freed
 * by this routine and is no longer valid.
 * 
 **/
print_job_t* print_job_abort(print_job_t* p_job);


/** 
 * \brief Cancel a job in progress.
 * 
 * \param p_job - Pointer to the job to be cancelled.
 * 
 * This routine frees resources allocated to the job when a job cancel
 * notifications is received.  This routine should be called only when 
 * a parser receives a MSG_CANCELJOB message.  The print job api specific 
 * memory allocations are * freed.  
 * 
 * \warning The print_job_t object specfied by the p_job parameter is freed
 * by this routine and is no longer valid.
 * 
 **/
print_job_t* print_job_cancel(print_job_t* p_job);


/** 
 * \brief Set the job's mode parameter.
 * 
 * \param p_job - pointer to the job
 * 
 * \param job_mode - The job mode to set.
 * 
 * Sets the mode for the job.  This is used to handle how pages cancels occur.
 * The default value is e_Normal.
 * 
 * The job mode may be one of e_Normal or e_KillAll.
 * 
 * e_Normal indicates that on page cancel processing to handle things like
 * paper out, paper jam the job manager holds the data and handles the entire
 * cancel/restart of the page.  This is the typical usage.
 * 
 * e_KillAll mode means that upon the occurrence of a page cancel the JobMgr 
 * will not hold all of the data and hence cannot recover the page without 
 * assistance.  So the JobMgr kills all of the pages and then sends the 
 * MSG_RESTART_PAGE msg to the job originator.  If using e_KillAll the 
 * source module ID must be set using print_job_set_source_module().
 * 
 **/
void print_job_set_job_mode(print_job_t* p_job, JOB_MODE job_mode);


/** 
 * \brief Set the job data type (copy or print).
 * 
 * \param p_job - pointer to the job
 * 
 * \param job_data_type - the data type of the job (copy or print)
 * 
 * The default setting is e_PrintJob.
 * 
 **/
// void print_job_set_job_data_type(print_job_t* p_job, JOB_DATA_TYPE job_data_type);


/** 
 * \brief Set the document type for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param doc_type - the type of document for this job
 * 
 * The default value is e_DOCUMENT.  Refer to DOCTYPE for details.
 * 
 **/
void print_job_set_doc_type(print_job_t* p_job, DOCTYPE doc_type);


/** 
 * \brief Set the source module id for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param src_mod_id - the module id of the module originating the job
 * 
 * Sets the module ID for the module originating the job.  This only needs
 * to be set if using the e_KillAll option in print_job_set_job_mode.
 * 
 **/
void print_job_set_source_module(print_job_t* p_job, MODULE_ID src_mod_id);


/** 
 * \brief Set the job collation setting.
 * 
 * \param p_job - pointer to the job
 * 
 * \param collate - true to turn on collation; false else
 * 
 * The default value is false.
 * 
 **/
void print_job_set_collation(print_job_t* p_job, bool collate);


/** 
 * \brief Sets the job duplex mode.
 * 
 * \param p_job - pointer to the job
 * 
 * \param duplex - the duplex mode for the job
 * 
 * The base default setting is DMDUP_SIMPLEX.  The setting
 * must be one of:
 *  DMDUP_SIMPLEX
 *  DMDUP_VERTICAL
 *  DMDUP_HORIZONTAL
 * 
 **/
void print_job_set_duplex(print_job_t* p_job, uint16_t duplex);


/** 
 * \brief Set the duplex backside rotation setting for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param rotate_backside - flag indicating if backsides should be rotated
 * 
 **/
void print_job_set_duplex_rotate_backside(print_job_t* p_job, bool rotate_backside);


/** 
 * \brief Set the number of copies of each page in the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param num_copies - the number of copies
 * 
 * Sets the number of copies of each page in the job.  This setting
 * will be inherited by each page.  The default value is 1.
 * 
 **/
void print_job_set_num_copies(print_job_t* p_job, uint32_t num_copies);


/** 
 * \brief Sets the request_media_detect flag 
 * 
 * \param p_job - pointer to the job
 * 
 * \param request_media_detect - whether or not to perform a 
 *        media detect
 * 
 * Informs the engine that this job requires media detection 
 * prior to any pages being produced 
 *  
 */ 
void print_job_set_request_media_detect(print_job_t* p_job, bool request_media_detect);


/** 
 * \brief Sets the default media type for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param mediatype - the desired media type
 * 
 * Sets the default media type for the job.  This setting is inherited 
 * by each page, but may be overridded on a per page basis.  
 * The default value is MEDIATYPE_ANY.
 * 
 * REVISIT: should this be removed from the API and done only at the page level
 * 
 **/
void print_job_set_media_type(print_job_t* p_job, mediatype_t mediatype);


/** 
 * \brief Sets the default media size for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param mediasize - the desired media size
 * 
 * Sets the default media size for the job.  This setting is inherited 
 * by each page, but may be overridded on a per page basis.  
 * The default value is MEDIASIZE_ANY.
 * 
 * REVISIT: should this be removed from the API and done only at the page level
 * 
 **/
void print_job_set_media_size(print_job_t* p_job, mediasize_t mediasize);

void print_job_set_copy_tray_setting_info(print_job_t* p_job, mediasize_t tray_0, mediasize_t tray_1, mediasize_t tray_2, mediasize_t tray_3);

/** 
 * \brief Sets the default media input source for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param source_tray - the desired input source
 * 
 * Sets the default media inptu source for the job.  This setting is inherited 
 * by each page, but may be overridded on a per page basis.  
 * The default value is MEDIASIZE_ANY.
 * 
 * REVISIT: should this be removed from the API and done only at the page level
 * 
 **/
void print_job_set_default_source(print_job_t* p_job, trayenum_t source_tray);


/** 
 * \brief Sets the darkness level for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param darknesslevel - the desired darkness level
 * 
 * Engine Darkness level range : 1 ~ 10 
 * Engine default darkenss level : 6
 * 
 **/
void print_job_set_darkness_level(print_job_t* p_job, uint32_t darknesslevel);


/** 
 * \brief Sets the toner save for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param tonersave - toner save ON / OFF
 * 
 * tonersave = 0	: 	OFF
 * tonersave = 1	: 	ON
 * 
 **/
void print_job_set_toner_save(print_job_t* p_job, uint32_t tonersave);


/** 
 * \brief Sets the image resolution for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param imageresolution - set image resolution
 * 
 * imageresolution = 600	: 	600dpi
 * imageresolution = 1200	:	1200dpi
 * 
 **/
void print_job_set_image_resolution(print_job_t* p_job, uint32_t imageresolution);


/** 
 * \brief Sets the parser information for the job.
 * 
 * \param p_job - pointer to the job
 * 
 * \param parser - parser information (GDI, PS, PCL...)
 * 
 * parser = 0	: 	Not GDI
 * parser = 1 	:	GDI
 * (at this moment, only GDI job have different image margin)
 **/
void print_job_set_parser_info(print_job_t* p_job, uint8_t parser);

typedef enum
{
	PRINTJOB,
	COPYJOB,
	FAXJOB
}JOBMODE;

/** 
 * \brief Sets the job information 
 * 
 * \param p_job - pointer to the job
 * 
 * \param mode - job mode information 
 * 
 * mode = 0	 	: 	Print
 * mode = 1 	:	Copy
 * mode = 2		: 	Fax
 **/
void print_job_set_job_info(print_job_t* p_job, JOBMODE mode);

/////////////////////////////////////////////////////////////////////////////////////////////////
// 
// PAGE specific portion of the API
// 
/////////////////////////////////////////////////////////////////////////////////////////////////

// Typical Usage:
//   print_page_construct(...)
//   set desired page options i.e. media_size, input source, etc
//   print_page_start(...)
//   send plane data
//   print_page_end(...)
//   print_page_free(...)

/** 
 * \brief Construct a new print page object and initialize with default and inherited settings.
 * 
 * \param p_job - pointer to the job associated with this page.
 * 
 * \return print_page_t* a pointer to the newly constructed page object
 * 
 * This routine constructs a page object and initializes the page with default settings and
 * with settings inherited from the job.  When a page is complete it must be freed.  
 * The page is freed with a call to print_page_free.
 * 
 **/
print_page_t* print_page_construct(print_job_t* p_job);


/** 
 * \brief Notify the next component in a print job chain that a page is starting.
 * 
 * \param page - pointer to the page
 * 
 * This routine notifies the module specified in the job_dest parameter of 
 * print_job_start(print_job_t* p_job, MODULE_ID job_dest) that the job
 * is starting.  Typically the job_dest parameter is the print job manager,
 * but it may specify an intermediate destination if further processing of
 * the data is required.
 * 
 * Note that all page related settings must be specified prior
 * to calling calling print_page_start.
 * 
 **/
void print_page_start(print_page_t* page);

int print_page_start_would_block();


/** 
 * \brief Notify the print job manager that the page generation has completed.
 * 
 * \param page - pointer to the page
 * 
 * \param pix_counts - pointer to the pixel counts sent by the software driver
 * or generated by the parser firmware if any.  May be NULL if no counts are
 * available.
 * 
 * This routine should be called on all normal page terminations.  The
 * print job manager is notified that the page is complete from the stand point
 * of page data generation.  
 * 
 * Once this routine has been called the page object should be free with a call
 * to print_page_free.
 * 
 * Note that on some platforms passing a NULL pointer for the pix_counts parameter
 * will cause hardware based pixel counting to be used automatically.
 * 
 * \warning This routine should not be called in the case of job/page abort.
 * Instead call only print_page_free to free the page memory.
 * 
 **/
void print_page_end(print_page_t* page, PIXELCNT* sw_pix_counts);


/** 
 * \brief Free the page object.
 * 
 * \param page - pointer to the page
 * 
 * This routine frees the page object create by print_page_construct.
 * 
 **/
void print_page_free(print_page_t* page);


/** 
 * \brief Set the collation settings for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param collate - true to turn on collation; false else
 * 
 **/
void print_page_set_collation(print_page_t* page, bool collate);


/** 
 * \brief Set the duplex settings for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param duplex - the duplex mode for the page
 * 
 * The default setting is DMDUP_SIMPLEX.  The setting
 * must be one of:
 *  DMDUP_SIMPLEX
 *  DMDUP_VERTICAL
 *  DMDUP_HORIZONTAL
 * 
 * REVISIT: does this routine make sense or should we just
 * inherit from job always?
 **/
void print_page_set_duplex(print_page_t* page, uint16_t duplex_opts);


/** 
 * \brief Specifies that this page is on the second side of a manual
 * duplex page.  MUST be called for every second side page.
 * 
 * \param page - pointer to the page
 * 
 * This routine must be called to mark every second side page.  The
 * first time this routine is called in each job context it will
 * automatically set a flag to cause a print stall to allow the
 * user to move the first side paper stack from the output bin to the
 * input tray.
 * 
 **/
void print_page_set_manual_duplex_second_side(print_page_t* page);


/** 
 * \brief Sets the input source for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param in_tray - the input source tray for the page
 * 
 * Default is TRAY_ANY.
 * 
 **/
void print_page_set_input_source(print_page_t* page, trayenum_t in_tray);


/** 
 * \brief Sets the output destination for the page
 * 
 * \param page - pointer to the page
 * 
 * \param out_tray - the output destination for the page
 * 
 * The default is TRAY_ANY.
 * 
 **/
void print_page_set_output_dest(print_page_t* page, trayenum_t out_tray);


/** 
 * \brief Sets the media size for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param mediasize - media size of the page
 * 
 * The default value is MEDIASIZE_ANY.
 * 
 **/
void print_page_set_media_size(print_page_t* page, mediasize_t mediasize);

void print_page_set_media_default_size(print_page_t* page, uint8_t value);

/** 
 * \brief Sets the media type for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param mediatype - media type of the page
 * 
 * The default value is MEDIATYPE_ANY.
 * 
 **/
void print_page_set_media_type(print_page_t* page, mediatype_t mediatype);


/** 
 * \brief Sets how many copies of the page will be printed.
 * 
 * \param page - pointer to the page
 * 
 * \param num_copies - the number of copies
 * 
 * The default value is inherited from the job num copies setting.
 * 
 **/
void print_page_set_num_copies(print_page_t* page, uint32_t num_copies);


/** 
 * \brief Sets whether the page is color or monochrome.
 * 
 * \param page - pointer to the page
 * 
 * \param page_color - e_Mono or e_Color
 * 
 * The default value is e_Mono.
 * 
 **/
void print_page_set_color_mode(print_page_t* page, page_color_mode_t page_color);

/** 
 * \brief Sets the input colorspace, additive gray, subtractive gray, RGB.
 * 
 * \param page - pointer to the page
 * 
 * \param input_color_space_t  
 * 
 * The default value is e_sRGB or e_subtractive_grey depending on mono/color input.
 * 
 **/
void print_page_set_colorspace(print_page_t* page, input_color_space_t colorspace);

/** 
 * \brief Sets the number of input colors in print data
 * 
 * \param page - pointer to the page
 * 
 * \param num_input_colors - the number of input colors
 * 
 * The default value is 1.
 * 
 **/
void print_page_set_num_input_colors(print_page_t* page, uint16_t num_input_colors);

/** 
 * \brief Sets the print quality of this page. The available print quality
 * settings exist in the the PRINT_QUALITY enum specific to each product.
 * Common values include draft, normal, best, etc.
 * 
 * \param page - pointer to the page
 * 
 * \param print_quality - desired print quality setting
 * 
 **/
void print_page_set_print_quality(print_page_t* page, PRINT_QUALITY print_quality);

/** 
 * \brief Set the page output resolution in dpi in the x direction.
 * 
 * \param page - pointer to the page
 * 
 * \param dpi_x - output dpi in the x direction (300, 600, etc.)
 * 
 * There is no default value.  This value MUST be specified for each page.
 * 
 **/
void print_page_set_dpi_x(print_page_t* page, uint32_t dpi_x);


/** 
 * \brief Set the page output resolution in dpi in the y direction.
 * 
 * \param page - pointer to the page
 * 
 * \param dpi_y - output dpi in the y direction (300, 600, etc.)
 * 
 * There is no default value.  This value MUST be specified for each page.
 * 
 **/
void print_page_set_dpi_y(print_page_t* page, uint32_t dpi_y);

/** 
 * \brief Sets the page's raster_x parameter
 * 
 * \param page - pointer to the page
 * 
 * \param x_bits - width in bits of the image.
 * 
 * This routine is used to specify the image width of the page input data in bits.
 * (e.g. line_width_in_pixels * output_bits_per_pixel).  Note that the (laser?) data
 * input width must be padded such that it is an even multiple of 16 bytes.  This
 * parameter specifies the padded width.
 * 
 * There is no default value.  This value MUST be set for every (laser?) page.
 * 
 * REVIST: give this function a more meaningful name?
 * 
 **/
void print_page_set_raster_x(print_page_t* page, uint32_t x_bits);


/** 
 * \brief Sets the output bits per pixel for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param output_bpp - typically 1, 2, 4
 * 
 * There is no default value.  This value must be specified for every page.
 *
 **/
void print_page_set_output_bpp(print_page_t* page, uint32_t output_bpp);

/** 
 * \brief Sets the page's image width in pixels.
 * 
 * \param page - pointer to the page
 * 
 * \param x_pixels - width of the original image in pixels
 * 
 * This routine is used to specify the width of the original 
 * image given by the driver before any clipping or padding has 
 * been done to the image line 
 * 
 * There is no default value.  This value MUST be set for every (laser?) page.
 * 
 * 
 **/
void print_page_set_image_width_in_pixels(print_page_t* page, uint32_t x_pixels);

/** 
 * \brief Sets the page's x dimension in pixels.
 * 
 * \param page - pointer to the page
 * 
 * \param x_pixels - width of the printable area in pixels
 * 
 * This routine is used to specify the width of the printable area of the 
 * page input data in pixels.  This is the actual width of the image.  It does not
 * include any alignment padding.  This width is used to center the image on the
 * page.
 * 
 * There is no default value.  This value MUST be set for every (laser?) page.
 * 
 * REVIST: give this function a more meaningful name?
 * 
 **/
void print_page_set_video_x(print_page_t* page, uint32_t x_pixels);


/** 
 * \brief Set the page's video y parameter (image height in lines).
 * 
 * \param page - pointer to the page
 * 
 * \param y_pixels - total height of the page data in pixels.
 * 
 * This routine specifies the page image dimension in pixels in y direction.
 * Typically this is the same as the number of lines on the page.
 * 
 * There is no default value.  This value MUST be set for every page.
 * 
 * REVIST: this is the same as RASTER_Y.  rename and combine?
 *
 **/
void print_page_set_image_height_lines(print_page_t* page, uint32_t y_pixels);


/** 
 * \brief Specify the x dimension of a custom media type for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param xdim - the x dimension of the page.
 * 
 * This is one of the routines used to specify a custom media type.
 * When setting a custom media type it must be fully specified using
 * the following routines:
 *   print_page_set_custom_media_size_x()
 *   print_page_set_custom_media_size_y()
 *   print_page_set_custom_media_units()
 * 
 * The units used in setting the x and y dimension are as specified by
 * the call to print_page_set_custom_media_units().
 * 
 **/
void print_page_set_custom_media_size_x(print_page_t* page, uint32_t xdim);


/** 
 * \brief Specify the y dimension of a custom media type for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param ydim - the y dimension of the page.
 * 
 * This is one of the routines used to specify a custom media type.
 * When setting a custom media type it must be fully specified using
 * the following routines:
 *   print_page_set_custom_media_size_x()
 *   print_page_set_custom_media_size_y()
 *   print_page_set_custom_media_units()
 * 
 * The units used in setting the x and y dimension are as specified by
 * the call to print_page_set_custom_media_units().
 * 
 **/
void print_page_set_custom_media_size_y(print_page_t* page, uint32_t ydim);


/** 
 * \brief Specify the units for the x and y dimension of a 
 * custom media type for the page.
 * 
 * \param page - pointer to the page
 * 
 * \param media_units - units for the custom media 
 * size (e.g. hundredths of an inch, tenths of mm, etc). 
 * 
 * This is one of the routines used to specify a custom media type.
 * When setting a custom media type it must be fully specified using
 * the following routines:
 *   print_page_set_custom_media_size_x()
 *   print_page_set_custom_media_size_y()
 *   print_page_set_custom_media_units()
 * 
 **/
void print_page_set_custom_media_units(print_page_t* page, paper_size_units_t media_units);


/** 
 * \brief Set the contrast value used during image enhancement
 * 
 * \param page - pointer to the page
 * 
 * \param contrast - contrast value  range = [0,200]
 * 
 * This routine specifies the amount of contrast adjustment used
 * during image enhancement.  0 implies low contrast, 200 
 * implies high contrast.  Default value is 100, no contrast 
 * adjustment is made. 
 * 
 **/
void print_page_set_contrast(print_page_t* page, uint32_t contrast);

/** 
 * \brief Set the brightness value used during image enhancement
 * 
 * \param page - pointer to the page
 * 
 * \param contrast - brightness value  range = [0,200]
 * 
 * This routine specifies the amount of brightness adjustment 
 * used during image enhancement.  0 implies low contrast, 200 
 * implies high contrast.  Default value is 100, no contrast 
 * adjustment is made. 
 * 
 **/
void print_page_set_brightness(print_page_t* page, uint32_t brightness);
void print_page_set_app(print_page_t* page, uint8_t kindofapp);
/**
 * \brief Get the print mode object for the current page data
 *  
 * \param page - pointer to the page 
 *  
 * \param print_mode - Output parameter with page's print mode object
 *  
*/
void print_page_get_print_mode(print_page_t *page, print_mode_t **print_mode);
void print_page_set_oneup_rotation_params(print_page_t* page, ROTATE_DIR rotation);
void print_page_set_sindoh_rotation_params(print_page_t* page, ROTATE_DIR rotation, uint32_t h, uint32_t w);
/**
 * \brief Set the nup print parameters 
 *  
 * \param page - pointer to the page 
 *  
 * \param num_params - parameters needed to perform nup copy
 *  
*/
void print_page_set_nup_params(print_page_t* page, nup_image_params_t *nup_params);

/**
 * @brief zxstream functions
 * @param page
 * @param color_size
 */
void print_page_add_zx_color_size(print_page_t* page, uint32_t color_size);

void print_page_add_zx_raster_x(print_page_t* page, uint32_t raster_x);

void print_page_add_zx_raster_y(print_page_t* page, uint32_t raster_y);

void print_page_add_zx_res_x(print_page_t* page, uint32_t res_x);

void print_page_add_zx_res_y(print_page_t* page, uint32_t res_y);

/**
 * @brief CUPS raster functions
 * @param page
 */
void print_page_add_imaging_bbox_int(print_page_t* page, uint32_t imaging_bbox[4]);

void print_page_add_imaging_bbox_float(print_page_t* page, float imaging_bbox[4]);

void print_page_add_page_size_int(print_page_t* page, uint32_t page_size[2]);

void print_page_add_page_size_float(print_page_t* page, float page_size[2]);

void print_page_add_color_space(print_page_t* page, uint32_t color_space);

void print_page_add_compression(print_page_t* page, uint32_t compression);

void print_page_add_marker_type(print_page_t* page, char marker_type[64]);

void print_page_add_rendering_intent(print_page_t* page, char rendering_intent[64]);

void print_page_add_page_size_name(print_page_t* page, char page_size_name[64]);

void print_page_add_anti_bleed_control(print_page_t* page, uint32_t anti_bleed_control);

void print_page_add_source_image_lightness(print_page_t* page, uint32_t source_image_lightness);

void print_page_add_advance_dot_placement(print_page_t* page, uint32_t advance_dot_placement);

void print_page_add_duplex_dry_time(print_page_t* page, uint32_t duplex_dry_time); 

void print_page_add_top_of_form_detect_disable(print_page_t* page, uint32_t top_of_form_detect_disable);

/** 
 * \brief Specify the borderless mode setting for this page.
 * 
 * \param page - pointer to the page
 * 
 * \param borderless - the borderless setting for this page. 
 * 
 **/
void print_page_set_borderless(print_page_t* page, uint32_t borderless);

/**
 * \brief Specify that page should be printed in grayscale.
 *  
 * \param page - pointer to the page 
 *  
 * \param gray - print gray if 1.
 *  
*/
void print_page_set_print_gray(print_page_t* page, uint32_t gray);

/////////////////////////////////////////////////////////////////////////////////////////////////
// 
// PLANE specific portion of the API
// 
/////////////////////////////////////////////////////////////////////////////////////////////////

// Typical Usage:
//   print_plane_start(...)
//   while plane data remaining print_plane_add_data(...)
//   print_plane_end(...)

/** 
 * \brief Notify the next component in a print job chain that a plane is starting.
 * 
 * \param page - pointer to the page
 * 
 * \param plane_color - the color of the plane being started
 * 
 * This routine notifies the module specified in the job_dest parameter of 
 * print_job_start(print_job_t* p_job, MODULE_ID job_dest) that a plane of the
 * specified color is starting.  Typically the job_dest parameter is the print 
 * job manager, but it may specify an intermediate destination if further processing of
 * the data is required.
 * 
 * \warning print_page_start() MUST be called before calling this routine.
 * 
 **/
void print_plane_start(print_page_t* page, COLOR plane_color);


/** 
 * \brief Notify the next component in a print job chain that a plane is complete.
 * 
 * \param page - pointer to the page
 * 
 * \param plane_color - the color of the plane being ended.
 * 
 * This routine notifies the module specified in the job_dest parameter of 
 * print_job_start(print_job_t* p_job, MODULE_ID job_dest) that a plane of the
 * specified color is endiing.
 * 
 * \warning print_plane_start() MUST be called before calling this routine.
 * 
 **/
void print_plane_end(print_page_t* page, COLOR plane_color);


/** 
 * \brief Add a data buffer to a print plane.
 * 
 * \param page - pointer to the page
 * 
 * \param plane_color - the color plane of the print data
 * 
 * \param data - a big buffer descriptor specifying the plane data;
 * ownership of this buffer transfers to the print subsystem which will free
 * it after consuming the data.
 * 
 * \param data_len_in_bytes - length of the data buffer in bytes
 * 
 * \param strip_type - specifies the data compression type of the plane
 * 
 * \param last_strip - MUST set to true if this is the last strip of the plane;
 * else set to false.
 * REVISIT: do we really need print_plane_end since we have this?
 * 
 * \param resolution_x - x resolution of the data in dpi
 * REVISIT: is this really needed or inherit from page?
 * 
 * 
 * \param resolution_y - y resolution of the data in dpi
 * REVISIT: is this really needed or inherit from page?
 * 
 * \param strip_width - full width of the strip in pixels including the alignment padding if any
 * REVISIT: is this really needed or inherit from page?
 * This may be needed for ink.  Need to check when merge to unity.  Discuss w/ Todd & Danny
 * 
 * \param strip_height - height of the strip in pixels (same as lines) 
 *  
 * \param bits_per_pixel - number of bits contained on one pixel 
 *  
 * \param left_to_right - data direction, true if left to right across the page 
 * 
 * \param comp_info - descriptor containing information describing the compression type (if any)
 * used on the strip.  The descriptor is a union.  The contents will vary depending on the type
 * of compression being used.  The compression data provided must match the type specified by
 * the strip_type paramater.  This parameter is ignored and may be NULL if RAW data is specified
 * for the strip_type.
 * 
 * Adds a strip of data to the print_plane specified by plane_color.  Typically a user would create
 * a full plane of data with a calling sequence similar to the following pseudo code:
 * 
 *   print_plane_start(...);
 *   for(i = 0; i < num_strips; i++)
 *   {
 *       print_plane_add_data(...);
 *   }
 *   print_plane_end(...);
 * 
 **/
void print_plane_add_data( print_page_t* page, 
                           COLOR plane_color, 
                           BigBuffer_t* data, 
                           uint32_t data_len_in_bytes, 
                           PLANE_TYPE strip_type,
                           bool last_strip, 
                           uint32_t resolution_x, 
                           uint32_t resolution_y, 
                           uint32_t strip_width_in_pixels, 
                           uint32_t strip_height_in_lines,
                           uint32_t bits_per_pixel,
                           bool left_to_right, 
                           compression_info_t* comp_info);
/**
 *  \brief construct only without sending the plane to the default front of the print pipe,
 *         used when the destination to send the plane to is in the middle of the pipe.
 **/
PLANED *print_construct_plane( mlimiter_t *limiter, 
                                    COLOR plane_color, 
                                    BigBuffer_t* data, 
                                    uint32_t data_len_in_bytes, 
                                    PLANE_TYPE strip_type,
                                    bool last_strip, 
                                    uint32_t resolution_x, 
                                    uint32_t resolution_y, 
                                    uint32_t strip_width_in_pixels, 
                                    uint32_t strip_height,
                                    uint32_t bits_per_pixel,
                                    bool left_to_right,
                                    compression_info_t* comp_info);

/////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Utility routines
// 
/////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Called by parsers to convert the driver supplied input tray value
 *        to our own internal representation.
*/
trayenum_t print_translate_input_tray(INPUTTYPE input);

/**
 * \brief Called by parsers to convert the driver supplied output tray value
 *        to our own internal representation.
 */
trayenum_t print_translate_output_tray(OUTPUTTYPE argOUTPUTTYPE);

void print_job_set_duplex_and_binding( print_job_t *job, uint32_t duplex, uint32_t binding ) ;

#ifdef __cplusplus
}
#endif

#endif //PRINT_JOB_API_H


