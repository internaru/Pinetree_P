/****************************************************************************** 
 * Copyright (c) 2010-2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/** 
 * \file pip.h
 * 
 * \brief Internal high-level header file for PIP image pipe.
 * 
 **/

#ifndef PIP_H
#define PIP_H

// This is used to decide whether or not to include the pipsmart
//   test and images.  Turning it off reduces the memory footprint.
//   It should not be defined for released code.
//#define INCLUDE_PIPTEST

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \brief One-time initialization for the pip_test module.
 * 
 * \param none. 
 *
 * \retval error_type_t OK if successful, FAIL otherwise. 
 **/
error_type_t pip_test_init(void);

/** 
 * \brief Notify the top-level pip module that a stat cdma 
 * transfer is complete.
 * 
 * \param cdma_perfid Peripherial ID of the STAT CDMA channel
 * that completed. 
 * 
 * This function provides the means for the PIP STAT module to
 * notify the high-level PIP code that a STAT CDMA channel has
 * completed a transfer.  
 **/
void pip_notify_stat_cdma_done(uint32_t cdma_perfid);

/** 
 * \brief Notify the top-level pip module that a pipe access
 * stat cdma transfer is complete.
 * 
 * \param cdma_perfid Peripherial ID of the PA stat CDMA channel
 * that completed. 
 * 
 * This function provides the means for the PA stat module to
 * notify the high-level PIP code that a PA CDMA channel has
 * completed a transfer.  
 **/
void pip_notify_pa_stat_cdma_done(uint32_t cdma_perfid);


void pip_enable_pa_store(uint32_t mode);

uint32_t pip_pastore_rows(uint32_t *outbuf, uint32_t nlines, uint32_t width, uint32_t channels);

uint32_t pip_paload_rows(uint32_t *inbuf, uint32_t nlines, uint32_t width, uint32_t channels);

void pip_frac_scale_set(uint32_t value);


#ifdef __cplusplus
}
#endif
#endif

