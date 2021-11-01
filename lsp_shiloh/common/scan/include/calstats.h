/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

#ifndef CALSTATS_H
#define CALSTATS_H

/**
 * \struct CalibrationStatTag
 *
 * \brief Data structure for the calibration statistics. 
 * Used for ACL and cimation (manufacturing).
 */

typedef struct 
{
    /* number of pixels in this calibration */
    uint32_t pixels_per_row;

    uint32_t color_type;
    uint32_t max;
    uint32_t pos_max;
    uint32_t min;
    uint32_t pos_min;
    uint32_t maxPtoP;
    uint32_t pos_maxPtoP;
    uint32_t mean;
    uint32_t num_band1;
    uint32_t num_band2;
    uint32_t num_band3;
    uint32_t numLeft;
    uint32_t numRight;
    uint32_t numBad;
    uint32_t numClipped;

    /* bad pixel replacement ; how many pixels total were replaced */
    uint32_t pixels_replaced;

    /* bad pixel replacmenet ; how many clumps of 3 consecutive pixels were
     * replaced
     */
    uint32_t num_replaced_clusters;

    /* davep 26-Sep-2012 ; add creation time. When non-zero, we know this entry
     * is valid. Will also serve as an age.
     */
    uint32_t creation_time;

} CalibrationStats;

void calstats_datastore_invalidate( void );
bool calstats_datastore_is_valid( void );

scan_err_t calstats_get_red( CalibrationStats *calstats );
scan_err_t calstats_get_green( CalibrationStats *calstats );
scan_err_t calstats_get_blue( CalibrationStats *calstats );

scan_err_t calstats_set_red( CalibrationStats *calstats );
scan_err_t calstats_set_green( CalibrationStats *calstats );
scan_err_t calstats_set_blue( CalibrationStats *calstats );

#endif 

