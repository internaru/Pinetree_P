/* 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef FINDCALSTRIP_H
#define FINDCALSTRIP_H

#ifdef FCS_DEBUG
#define MAX_PREFIX_FILENAME 64
void fcs_set_outfilename_prefix( const char *filename_base );
#endif

cal_err_t find_cal_strip( uint16_t *pixbuf, uint32_t dpi, uint32_t pixels_per_row, uint32_t num_rows,
                     uint32_t *starting_row, uint32_t *ending_row );

#endif

