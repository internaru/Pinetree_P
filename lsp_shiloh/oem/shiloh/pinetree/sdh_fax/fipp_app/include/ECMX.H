/*
 *  $RCSfile: ECMX.H,v $
 * $Revision: 1.4 $
 *   $Author: yooh1 $
 *     $Date: 2011/02/01 17:46:29 $
 */
/************************************************************************
 *
 *      ECMX.H
 *
 *  T.30 Error Correction Mode Definitions.
 *
 *  Copyright 2004 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef ECMX_H
#define ECMX_H

/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/

/******************************************************************************
*  2. DEFINES
******************************************************************************/
#define MAX_ECM_BLOCKS 			40//2
#define MAX_ECM_FRAMES 			256
#define MAX_ECM_BYTES_PER_FRAME 256

/******************************************************************************
*  3. STRUCTURES
******************************************************************************/
typedef struct 
{
	UINT8 	*data;
	UINT16 	size;
	BOOLEAN	end_of_page;	
} CDB_BUFFER;


/******************************************************************************
*  4. ALLOCS
******************************************************************************/


/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/
extern UINT8 ECMFeedCDB(CDB_BUFFER *cdb);
extern UINT8 ECMReturnCDBs(void);
extern UINT8 ECMGetBlockFront(void);
extern void ECMResetBlockFront(void);
extern UINT8 ECMTx_PPR_Transmit(void);
#endif // #ifndef ECMX_H
