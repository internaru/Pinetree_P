/******************************************************************************
 * Copyright (c) 2006-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * ============================================================================
 * Copyright (c) 2004, 2005 Avago Technologies, Inc. All Rights Reserved
 *                      
 *                         Avago Confidential
 * ============================================================================
 **/

#ifndef SCANVARSIQ_H
#define SCANVARSIQ_H

void scanvar_siq_merge( struct scanvars *sv );
void scanvar_siq_print( const struct scanvars *sv );
bool scanvar_siq_has_key( struct scanvars *sv, int key );

#ifdef SIQDATA_H
void scanvar_siq_merge_data( const struct siqdata *sd, struct scanvars *sv );
scan_err_t scanvar_siq_merge_all_user_settings( const struct siqdata_table *siqdata, 
                                                struct scanvars *sv );
#endif

#endif

