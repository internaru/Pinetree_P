/*
 *  $RCSfile: modem_buff.h,v $
 * $Revision: 1.6 $
 *   $Author: yooh1 $
 *     $Date: 2010/08/06 21:44:34 $
 */
/************************************************************************
 *
 *      modem_buff.h
 *
 *  Modem Task Header File.
 *
 *  Copyright 2004 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef __MDM_MAIN_H__
#define __MDM_MAIN_H__

/******************************************************************************
*  FILE FORMAT:
*      1. ENUMERATIONS
*      2. DEFINES
*      3. STRUCTURES
*      4. ALLOCS
*      5. FUNCTIONS
******************************************************************************/


/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/


/******************************************************************************
*  2. DEFINES
******************************************************************************/
#define MODEM_RX	1
#define MODEM_TX	2


/******************************************************************************
*  3. STRUCTURES
******************************************************************************/
typedef struct
{
	UINT8	curService;				// service name that this node is belong to
	UINT8 	ModemDirection;         /* 2 = TX, 1 = RX */
	UINT16 	wrap;					// bytes for a frame
	BOOLEAN	ecm_mode;
	UINT8	coding_type;			// coding type of this fax session
} MODEM_JOB_PARAMETERS;


/******************************************************************************
*  4. ALLOCS
******************************************************************************/


/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/
void job_modem_start(void);
void job_modem_init(UINT8 direction, BOOLEAN m_ecm, UINT8 coding, UINT16 frame_size);

extern void *CxModemOpen( void *homolofunc, UINT8 *status );
extern void CxSelectCountry( UINT8 nCountryCode );
extern void set_phone_state(UINT8 new_status);
extern void CxSetCallBackHost( void *callback );
extern UINT8 CxModemLineStatus( void );
#endif			// __MDM_MAIN_H__




