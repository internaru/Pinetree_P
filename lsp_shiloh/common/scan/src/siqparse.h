/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Avago Technologies, Inc. All Rights Reserved
 *                      
 *                         Avago Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/

#ifndef SIQPARSE_H
#define SIQPARSE_H

/* error codes returned by functions */
#define SIQERR_OUT_OF_MEM -1
#define SIQERR_INVALID    -2
#define SIQERR_TOO_LONG   -3
#define SIQERR_PARSE_FAIL -4
#define SIQERR_EOF        -5  
#define SIQERR_UNKNOWN_NAME -6

/* siqnames.c ; put in a separate file so I don't have to siqparse.l every time
 * we want to add a new name
 */
extern const struct siqdata_name g_siq_names[];

void siqparse_set_buffer( char *buffer, int buflen );
void siqparse_delete_buffer( void );
int siqparse_parse_file( struct siqdata_table *siqdata );
void siqparse_siqdata_free( struct siqdata *sd );

#endif

