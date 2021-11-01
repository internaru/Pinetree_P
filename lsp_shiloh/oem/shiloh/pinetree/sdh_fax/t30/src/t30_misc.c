/*
 *  $RCSfile: t30_misc.c,v $
 * $Revision: 1.30 $
 *   $Author: miurat $
 *     $Date: 2011/12/16 02:28:36 $
 */
/************************************************************************
 *
 *      t30_misc.c
 *
 *  T.30 Miscellaneous Functions (Transmit and Receive)
 *
 *  Copyright 2004 - 2011 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"
#include "cxshell.h"
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "ERROR.H"
#include "MODEMX.H"
#include "T30X.H"
#include "HDLC.H"
#include "ECMX.H"
#include "ECM.H"
#include "t30.h"

// declare structure
T_T30_CONTROL *T30Control = NULL;

/*===========================================================================
*
*    T30AlertLocalOp
*
*      Alerts the local operator to voice request, and checks for line
*      request
*
*      Input:          t30_attribute.FaxVoiceRequest = TRUE if we initiated
*      Output:         return = TRUE if 'Talk' key pressed, FALSE if not
*                      t30_attribute.FaxLineRequest = TRUE if 'Talk' key pressed, FALSE if not
*
*===========================================================================*/
UINT8 T30AlertLocalOp( void )
{
	/* Verify that voice requests are being handled */

	if ( t30_attribute.IgnoreVoiceRequest )
		return 0;

	TRACE_FAX( "T30AlertLocalOp(): Pick Up Phone\n" );

	/* If we didn't originate, wait 2 seconds.  Otherwise, wait 14 seconds */
	if ( t30_attribute.FaxVoiceRequest )
		T30Control->AlertCnt = 7;
	else
		T30Control->AlertCnt = 1;

	while ( T30Control->AlertCnt-- )
	{
		/* Beep the operator */

		/* Wait 2 seconds for line request. */
		T30_TIMER_START( TimerB, 2000 );

		while ( !T30_TIMER_EXPIRE( TimerB ) )
		{
			OSC_Sleep( T30_DELAY_TIME );

			/* Exit if line request from local operator */

			if ( t30_attribute.FaxLineRequest )
			{
				TRACE_FAX( "T30AlertLocalOp(): exiting due to line req\n" );
				return ( 1 );
			}
		}
	}

	TRACE_FAX( "T30AlertLocalOp(): exiting due to time out\n" );
	return ( 0 );
}


/*===========================================================================
*
*    T30CheckCommand
*
*      Receives T.30 commands.  This function will be called during modedHsSignalRec.
*      in V34 modes
*
*      Input:          (none)
*      Output:         return value = 1 ; EOM or MPS detected again
*                                   = 2 ; DCN detected
*                                   = 0 ; Skip command check
*
*===========================================================================*/
UINT8 T30CheckCommand( void )
{
	UINT8 cmd;

	TRACE_FAX( "T30CheckCommand: LastFCFRcvd %02X, LastFCFSent %02X\n", T30Control->LastFCFRcvd, T30Control->LastFCFSent );

	if ( ( ( T30Control->LastFCFRcvd & 0xEE ) == EOM )
	  || ( ( T30Control->LastFCFRcvd & 0xEE ) == MPS )
	  || ( T30Control->LastFCFSent == MCF ) )
	{
		cmd = T30GetCommand();

		if ( cmd == T30Control->LastFCFSent )
		{
			/* Assume this is an echo and we'll get another command next */
			TRACE_FAX( " echo ( %02X )\n", cmd );

			if ( cmd == MCF )
			{
				TRACE_FAX( "Exiting T30RxPhaseD by echo MCF\n" );
				return ( 0 );
			}

			/* Restart the T2 timer */
			T30_TIMER_START( TimerC, T2_TIMEOUT );
		}

		if ( ( ( cmd & 0xFE ) == PPS ) || ( ( cmd & 0xFE ) == MPS ) )
		{
			return ( 0 );
		}

		if ( ( cmd & 0xFE ) == EOM )
		{
			T30RxProcessEOM();

			TRACE_FAX( "T30CheckCommand: Got EOM or MPS again\n" );
			return ( 1 );
		}
		else if ( ( cmd & 0xFE ) == DCN )
			return ( 2 );
	}
	else if ( T30Control->LastFCFSent == CFR )
	{
		return ( 3 );
	}

	return ( 0 );
}


/*===========================================================================
*
*    T30GetCommand
*
*      Receives T.30 commands.  Stores the FIF's, if necessary.  Keeps
*                       going until it receives a final frame, or an error.
*
*      Input:          (none)
*      Output:         return value = HDLC FCF ( > $0F) if no error, or
*                                  = error code ( < $10)
*
*===========================================================================*/
UINT8 T30GetCommand( void )
{
	UINT8 command;
	UINT8 fcf;

	TRACE_FAX( "Entering T30GetCommand().....\n" );

	if ( !T30Control->V34Mode )
	{
		OSC_Sleep( T30_DELAY_TIME );
	}

	while ( 1 )
	{
		/* Look for the first or subsequent command */
		command = HDLCRxCommand( T30Control->FirstCmdFlg, &( T30Control->TimerC ) );

		/* Check for error (byte count = 0?) */

		if ( !command
		  || ( ModemControl->Status == TIME_OUT )
		  || ( ModemControl->Status == NO_COMMAND )
		  || ( ModemControl->Status == FRAME_ERROR )
		  || ( ModemControl->Status == CI_TONE ) )
		{
			if ( ModemControl->Status == NO_COMMAND )
			{
				T30Control->FirstCmdFlg = 0;
				continue;
			}

			if ( ModemControl->Status == FRAME_ERROR )
				T30Control->FirstCmdFlg = 0;
			return ( ModemControl->Status );
		}

		/* Store the frame info, if necessary */
		fcf = T30StoreFrame();

		/* Was there an acceptable error in the frame ? */
		if ( !fcf )
		{
			return ( ModemControl->Status );
		}
		else
		{
			T30Control->FirstCmdFlg = 0;
		}

		/* Was this a final frame? */
		if ( command == 0x13 )
		{
			return ( fcf );
		}
	}
}

/*===========================================================================
*
*    T30GetResponse
*
*      Receives T.30 responses.  Stores the FIF's, if necessary.  Keeps
*                       going until it receives a final frame, or an error.
*
*      Input:          (none)
*      Output:         return = HDLC FCF ( > $0F) if no error, or
*                                  = error code ( < $10)
*
*===========================================================================*/
UINT8 T30GetResponse( void )
{
	UINT8 response;
	UINT8 fcf;
	UINT8 status;
	UINT16 timeout;

	TRACE_FAXDEBUG( "Entering T30GetResponse().....\n" );

	/* Clear out all the frames */
	HDLCRxWriteInitAllFrames();

	T30Control->FirstCmdFlg = 1;

	/* Get frames (good or bad) */
	while ( 1 )
	{
		/* Start either an auto or manual timer */

		if ( T30Control->ManualMode )
			timeout = MANUAL_T4_TIMEOUT;
		else
			timeout = AUTO_T4_TIMEOUT;

		/* Look for the first response */
		if ( !T30Control->V34Mode
		  && ( T30Control->TriedV8 < 3 )
		  && t30_attribute.FaxTxStartSpeed > T30_SPEED_V17_14400 )
		{
			status = MODEM_CI_TONE;
		}
		else
			status = HDLC_NO_FRAME_YET;

		while ( 1 )
		{
			/* Get frames.  If frames are bad, we will go back to getting the first command */

			response = HDLCRxResponse( T30Control->FirstCmdFlg, &status, timeout, NULL, NULL );

			/* Check for error (byte count = 0?) */

			if ( !response )
			{
				TRACE_FAX( "T30GetResponse No response %d, %d\n", status, ModemControl->Status );

				/* Echo bit not on or T4 timer expired? */
				if ( ( status == HDLC_NO_CARRIER_ERROR )
				  || ( status == HDLC_TIME_OUT ) )
				{
					return ( ModemControl->Status );
				}
				else if ( !ModemControl->V34Mode
			           && ( T30Control->TriedV8 < 3 )
				       && ( status == HDLC_CI_TONE ) )
				{
					ModemControl->Status = CI_TONE;
					return ( CI_TONE );
				}
				else if ( status == HDLC_CHECKSUM_ERROR )
				{
					T30Control->FirstCmdFlg = 0;
					break;
				}
				else
				{
					/* Ignore bad frames if T4 time not expired */
					break;
				}
			}

			/* Store the frame info, if necessary */
			fcf = T30StoreFrame();

			/* Was there an acceptable error in the frame ? */
			if ( !fcf )
				return ( ModemControl->Status );
			else
				T30Control->FirstCmdFlg = 0;

			/* Was this a final frame? */
			if ( response == 0x13 )
			{
#if 0
				/* CRP received? */

				if ( fcf == CRP )
					return ( NO_RESPONSE );
				else
#endif
					return ( fcf );
			}

			/* If not final frame, get another response */
			status = HDLC_NO_FRAME_YET;
		}

		OSC_Sleep( T30_DELAY_TIME );
	}
}


/*===========================================================================
*
*    T30SendFCF
*
*      Sends FCF
*
*      Input:          FCF to send
*                      T30Control->DISFlg = 1 if DIS received, 0 if not
*      Output:         T30Control->LastFCFSent = FCF sent
*
*===========================================================================*/
void T30SendFCF( UINT8 fcf )
{
	UINT8 i, j, size;

#ifdef OEM_FAX_REPORT
    UINT16 report[ 256 ];
#endif

	if ( (!T30Control->V34Mode) && ( (fcf == CFR) || (fcf == FTT) || (fcf == MPS) || (fcf == EOP)) )
	{
		OSC_Sleep(75);     // Add approximately 75ms' delay before sending CFR or MPS/EOP when not V.34 -> See T.30 Rec. of NOTE 4 of 5. Binary coded signalling procedure.
		if( (fcf == CFR) || (fcf == FTT) ) {
			OSC_Sleep(500);
		}
	}

	if ( fcf != CRP )
		T30Control->LastFCFSent = fcf;

	/* Point to the next frame */
	HDLCTxWriteNextFrame();

	/* Find the command in the table */
	for ( i = 0; i < FCFSendTableSize; i++ )
		if ( FCFSendTable[ i ] == fcf )
			break;

	/* Anything not in the table is final frame, no FIF */
	if ( i == FCFSendTableSize )
		i = 0;

	/* Write the control field to the buffer */
	HDLCTxWriteControl( CtrlTbl[ i ] );

	/* Check for FCF's we shouldn't OR the DIS bit into */
	if ( ( ( fcf == DTC )
	    || ( fcf == CIG )
	    || ( fcf == NSC )
	    || ( fcf == PWD )
	    || ( fcf == SEP )
	    || ( fcf == PSA )
	    || ( fcf == RCP ) ) )
		HDLCTxWriteDataByte( fcf );
	else
		HDLCTxWriteDataByte( fcf | T30Control->DISFlg );

	size = ByteCntTbl[ i ];

	if ( fcf == DCS )
	{
		size = T30Control->DIS_size;
		
		if (size > 10)
		{
			size = 10; // To avoid comm. problem betwen SDH's fax machine and Konica Minolta Magicolor 2490MF.
		}
	}

	TRACE_FAX( "Tx raw FCF [ %02X", fcf | T30Control->DISFlg );

#ifdef OEM_FAX_REPORT
	report[ 0 ] = fcf | T30Control->DISFlg;
#endif

	/* If there is more data, fill it in */
	if ( size )
	{
		/* Point to the table entry */
		T30Control->FIF_PTR = ( UINT8 * ) ( FIFSendTable[ i ] );

		/* Write each byte of the FIF to the HDLC Frame buffer */

		for ( j = 0; j < size; j++ )
		{
			TRACE_FAX( " %02X", T30Control->FIF_PTR[ j ] );

#ifdef OEM_FAX_REPORT
			report[ j + 1 ] = T30Control->FIF_PTR[ j ];
#endif
			HDLCTxWriteDataByte( T30Control->FIF_PTR[ j ] );
		}
	}

	TRACE_FAX( " ]\n" );

#ifdef OEM_FAX_REPORT
	ModemControl->Level = ModemReadTxLevel();
	ModemControl->EQM   = 0xffff;
	CxProtocolReport( fcf, report, size + 1, TX_LOG );
#endif

	/* Send the frame */
	HDLCTxControlFrame();
}


/*===========================================================================
*
*    T30StopFax
*
*      Stops a fax transaction
*
*      Input:          T30Control->State = current T.30 state
*                      T30Control->Status = current error code, if any
*      Output:         MSB of T30Control->Status = 1, if there wasn't an error code
*                        already
*                      MSB of ModemControl->Status = 1 if we're receiving
*                      MSB of T4Status = 1 if we're transmitting
*
*===========================================================================*/
void T30StopFax( void )
{
	TRACE_FAX( "T30StopFax()\n" );

	T30Control->State  = STATE_HOST_ABORT;
    T30Control->Status = FAX_ERROR_USER_ABORT_ERROR;

	ModemControl->Status |= MODEM_ABORT;
}


/*===========================================================================
*
*    T30StoreFrame
*
*      Stores HDLC frame information field, if necessary
*
*      Input:          HDLC_BUFFER = HDLC frame
*      Output:         return value = FCF received
*                      Frame FIF stored in appropriate location
*                      T30Control->LastFCFRcvd = FCF received
*
*===========================================================================*/
UINT8 T30StoreFrame( void )
{
	UINT8 data, fcf;
	UINT8 i;

#ifdef OEM_FAX_REPORT
    UINT16 report[ 256 ];
#endif

	/* Reset the data byte pointer so we're pointing to the first byte */
	HDLCRxReadInitFrame();

	/* Get the HDLC address field. If not correct, set up for next frame and exit */

	if ( HDLCRxReadAddress() != 0xFF )
	{
		T30Control->LastFCFRcvd = 0xFF;
		HDLCRxReadNextFrame();

		TRACE_FAX( "T30StoreFrame(): Bad address field\n" );

		return ( T30Control->LastFCFRcvd );
	}

	/* Get the HDLC control field. If not correct, exit */
	if ( ( HDLCRxReadControl() & 0xEF ) != 0x03 )
	{
		T30Control->LastFCFRcvd = 0xFF;
		HDLCRxReadNextFrame();

		TRACE_FAX( "T30StoreFrame(): Bad control field\n" );

		return ( T30Control->LastFCFRcvd );
	}

	/* Get the FCF */
	data = HDLCRxReadDataByte();

	/* See if we can strip out the 'DIS received bit */
	if ( ( data == CIG )
	  || ( data == DTC )
	  || ( data == NSC )
	  || ( data == PWD )
	  || ( data == SEP )
	  || ( data == PSA ) )
		fcf = data;
	else
		fcf = data & 0xFE;

	/* Try to find the FCF in the FCF table */
	for ( i = FCFReceiveTableSize; i > 0; i-- )
		if ( FCFReceiveTable[ i ] == fcf )
			break;

	TRACE_FAX( "T30StoreFrame(): FCF ( %02X ), i = %d\n", fcf, i );

	/* If frame is too short, get rid of it */
	if ( HDLCRxReadLength() < ( UINT16 ) MinSizeTbl[ i ] )
	{
		HDLCRxReadNextFrame();

		TRACE_FAX( "T30StoreFrame(): Frame too short\n" );

		ModemControl->Status = DISCONNECT;
		return ( 0 );
	}	

	TRACE_FAX( "Rx raw FCF [ %02X", data );

#ifdef OEM_FAX_REPORT
	report[ 0 ] = data;
#endif

	/* Store the maximum size and FIF pointer */
	T30Control->MaxFIFLen = MaxSizeTbl[ i ];

	T30Control->FIF_PTR = ( UINT8 * ) ( FIFReceiveTable[ i ] );

	/* Now store the FIF */
	i = 0;

	while ( HDLCRxReadLength() )
	{
		/* If we read all we can, move on to the next frame */

		if ( i == T30Control->MaxFIFLen )
			break;

		T30Control->FIF_PTR[ i++ ] = HDLCRxReadDataByte();

		TRACE_FAX( " %02X", T30Control->FIF_PTR[ i - 1 ] );

#ifdef OEM_FAX_REPORT
		report[ i ] = T30Control->FIF_PTR[ i - 1 ];
#endif

	}

	TRACE_FAX( " ]\n" );

#ifdef OEM_FAX_REPORT
	CxProtocolReport( fcf, report, i + 1, RX_LOG );
#endif

	if ( fcf == CRP )
	{
		return fcf;
	}
	
	T30Control->LastFCFRcvd = fcf;

	/* Check for NSF, NSS, or NSC */
	if ( t30_attribute.FaxEnableNSF
	  && ( ( fcf & 0xFC ) == 0x20 ) )
	{
		T30Control->NSTDFlg = 0;

		for ( i = 0; i < NSTDHeaderSize; i++ )
		{
			if ( T30Control->FIF_PTR[ i ] != T30Control->LocalNSTD[ i ] )
			{
				TRACE_FAX( "T30StoreFrame(): Frame not non-standard %x\n", T30Control->FIF_PTR[ i ] );

				HDLCRxReadNextFrame();
				T30Control->LastFCFRcvd = fcf;

				return ( T30Control->LastFCFRcvd );
			}
		}

		T30Control->NSTDFlg = 1;
	}

	/* Notify status if remote ID is known */
	data = T30Control->LastFCFRcvd;

	if ( ( data == CIG ) || ( data == CSI ) || ( data == TSI ) )
	{
		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.FaxRemoteID[ i ] = T30Control->FIF_PTR[ i ];
		}
	}
	else if ( data == SID )
	{
		T30Control->got_SID = SID;

		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.FaxRemotePassword[ i ] = T30Control->FIF_PTR[ i ];
		}
	}
	else if ( data == SUB )
	{
		T30Control->got_SUB = SUB;

		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.FaxRemoteSubaddress[ i ] = T30Control->FIF_PTR[ i ];
		}
	}
	else if ( data == PWD )
	{
		T30Control->got_PWD = PWD;

		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.PollRemotePassword[ i ] = T30Control->FIF_PTR[ i ];
		}
	}
	else if ( data == SEP )
	{
		T30Control->got_SEP = SEP;

		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.PollRemoteSubaddress[ i ] = T30Control->FIF_PTR[ i ];
		}
	}
	else if ( data == PSA )
	{
		T30Control->got_PSA = PSA;

		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.PollRemoteSubaddress[ i ] = T30Control->FIF_PTR[ i ];
		}
	}
	else if ( data == NSF )
	{
		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.FaxRemoteNSF[ i ] = T30Control->FIF_PTR[ i ];
		}
	}
	else if ( data == NSC )
	{
		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.FaxRemoteNSC[ i ] = T30Control->FIF_PTR[ i ];
		}
	}
	else if ( data == NSS )
	{
		for ( i = 0; i < 20; i++ )
		{
			t30_attribute.FaxRemoteNSS[ i ] = T30Control->FIF_PTR[ i ];
		}
	}

	HDLCRxReadNextFrame();
	return ( T30Control->LastFCFRcvd );
}


void t30_timer_start( void *tmr, UINT32 msec )
{
	T_T30_TIMER *sTim;

	sTim = ( T_T30_TIMER * )tmr;
	sTim->start_time = tx_time_get();
	sTim->interval = msec;
}


BOOL t30_timer_expire( void *tmr )
{
	T_T30_TIMER *sTim;
	UINT32 curr_time;

	sTim = ( T_T30_TIMER * )tmr;
	curr_time = tx_time_get();

	if ( curr_time >= sTim->start_time )
	{
		if ( ( curr_time - sTim->start_time ) >= sTim->interval )
		{
			return TRUE;
		}
	}
	else
	{
		if ( ( curr_time + ~sTim->start_time + 1 ) >= sTim->interval )
		{
			return TRUE;
		}
	}

	return FALSE;
}

