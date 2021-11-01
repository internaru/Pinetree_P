/*
 *  $RCSfile: modemv8.c,v $
 * $Revision: 1.30 $
 *   $Author: miurat $
 *     $Date: 2012/01/06 22:42:28 $
 */
/************************************************************************
 *
 *      modemv8.c
 *
 *  V8 handling code of the modem device driver.
 *  This handles the V8 sequences during the initial called station
 *  detaction, calling station detection, or switch in polling modes.
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
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
#include "PHONEX.H"
#include "T30X.H"
#include "ECMX.H"
#include "ECM.H"
#include "HDLC.H"
#include "modem336.h"


#define MODEM_V8_INTERVAL       20    /* ms */
#define MODEM_CED_TONE_MASK     0x10  /* ATV25 */

extern OSC_MUTEX modem_dsp_mutex;

/*===========================================================================
*    V8EarlyFinish
*    Utility function to terminate the call progress tone detection loop
*    of a fax call if V8 negotiations are not detected
*===========================================================================*/
static UINT8 V8EarlyFinish( UINT8 result )
{
	TRACE_MODEM( "V8EarlyFinish %d\n", result );

	ModemControl->State = MODEM_IDLE;

	MDM_CLEARBIT( MODEM_09, ORG + DTR );
	ModemDisableToneDet( MODEM_FSK_TONE );

	if ( ( ModemControl->Status == NO_ERROR )
	  || ( ModemControl->Status == TIME_OUT ) )
	{
		ModemConfig( MODEM_V8_CONF );
		ModemConfig( MODEM_V21_CH2_CONF );
		MDM_SETBIT( MODEM_09, DATA );
	}

	return result;
}


/*===========================================================================
*    V8 Setup
*      Setsup V8 with the associated configurations
*      Input:  speed  - Initial speed to try for
*              originate - TRUE if setting up as the originator
*              wait - For originate, FALSE if the CM without ANSam bit should be set
*                     For receive, TRUE if the no ANSam until CI bit should be set
*      Output: None
*===========================================================================*/
static void V8Setup( UINT8 speed, BOOL originate, BOOL wait, BOOL tx_ci )
{
	UINT8 temp;

	TRACE_MODEM( "V8Setup: %s\n", originate ? "Originate" : "Answer"  );       // THY

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	temp = MDM_READ( MODEM_15 );
	temp &= ~EXL3;
	temp |= EARC;
	MDM_WRITE( MODEM_15, temp );

	temp = MDM_READ( MODEM_09 );
	if ( originate )
		temp |= ORG;
	else
		temp &= ~ORG;
	temp &= ~CC;
	temp |= DATA;
	MDM_WRITE( MODEM_09, temp );

	temp = MDM_READ( MODEM_08 );
	temp &= ~( ASYN + V21S + RTRN + RTS );
	temp |= TPDM;
	MDM_WRITE( MODEM_08, temp );

	temp = MDM_READ( MODEM_07 );
	temp &= ~( L3ACT + MHLD );
	MDM_WRITE( MODEM_07, temp );

	temp = MDM_READ( MODEM_06 );
	temp &= ~( RTDIS + CCRTN + WDSZ_8 );
	temp |= HDLC;
	MDM_WRITE( MODEM_06, temp );

	temp = MDM_READ( MODEM_05 );
	temp &= ~( FCCR + CEQS + VINIT + TXSQ + TTDIS + STOFF );
	temp |= CEQ;
	MDM_WRITE( MODEM_05, temp );

	temp = MDM_READ( MODEM_04 );
	temp &= ~( EQFZ + NRZIEN + TOD + STRN );
	temp |= FIFOEN;
	MDM_WRITE( MODEM_04, temp );

	/* Transmitter abort on underrun */
	ModemDSPWrite1( MODEM_TX_ABORT_UNDERFLOW, 0x40, TRUE );

	/* Enable Extended FIFO */
	temp = ModemDSPRead1( MODEM_RX_EXTEND_FIFO_DIS ) & ~0x03;
	ModemDSPWrite1( MODEM_RX_EXTEND_FIFO_DIS, temp, FALSE );

	/* Trigger level set 1 byte */
	temp = ModemDSPRead1( MODEM_RX_FIFO );
	ModemDSPWrite1( MODEM_RX_FIFO, temp & 0x21, FALSE );

	/* Set symmetric control channel rates */
	temp = ModemDSPRead1( MODEM_V34_CCR_ENABLE ) & ~0x60;
	ModemDSPWrite1( MODEM_V34_CCR_ENABLE, temp, FALSE );

	/* Select the preferred high speed connection rate */
	temp = 0xC0 | ( speed - T30_SPEED_V17_14400 );
	ModemDSPWrite1( MODEM_V34_HD_CONF, temp, FALSE );

	if ( speed == T30_SPEED_V34_2400 )
		ModemDSPWrite1( MODEM_V34_BMR, 0x03, FALSE );
	else
		ModemDSPWrite1( MODEM_V34_BMR, 0x3F, FALSE );

	/* Setup the control registers */

	/* Control 1 */
	if ( originate )
		temp = 0x88;
	else
	{
		temp = 0xA8;
		/* Also verify no TX of ANSam until CI detected bit is set correctly */
		if ( wait )
			temp |= 0x02;
	}

	ModemDSPWrite1( MODEM_V8_CONTROL_1, temp, FALSE );

	/* Control 2 - None since no RLSD in V8 is already set */
	ModemDSPWrite1( MODEM_V8_CONTROL_2, 0x08, FALSE );  // default

	/* Control 3 - None since V34 HDX is already set */
	ModemDSPWrite1( MODEM_V8_CONTROL_3, 0x80, FALSE );  // default

	/* Control 4 - V17, V29, and V27 support */
	ModemDSPWrite1( MODEM_V8_CONTROL_4, 0xC4, FALSE );  // default

	/* Control 5 - None since V21 support is already set */
	ModemDSPWrite1( MODEM_V8_CONTROL_5, 0x80, FALSE );  // default

	/* Control 6 - Check if CM without ANSam bit should be set */
	temp = ModemDSPRead1( MODEM_V8_CONTROL_6 ) & ~0x28;

	if ( originate )
	{
		if ( wait )
			temp |= 0x20;
		else
			temp |= 0x28;
	}
	else
	{
		if ( wait )
			temp |= 0x08;
		else
			temp |= 0x00;
	}

	ModemDSPWrite1( MODEM_V8_CONTROL_6, temp, FALSE );

	temp = ModemDSPRead1( 0x43E ) & ~0x10;
	ModemDSPWrite1( 0x43E, temp, FALSE );  // Disable V8b detector status update

	if ( tx_ci )
	{
		ModemTxSetLevel( ModemControl->Options.FaxTxLevel );
		ModemDSPWrite1( 0x413, 0x20, FALSE );  // adjust CI level
	}

	/* Configure in V8 mode */
	ModemConfig( MODEM_V8_CONF );

	ModemDSPWrite1( MODEM_RLSD_DROPOUT_TIMER_L, 0x08, FALSE );

	OSC_Release_Mutex( &modem_dsp_mutex );
}


/*===========================================================================
*    V8Answer
*      Sends ANSam and then monitors either for DIS, CM, or a timeout
*      Input:  speed  - Pointer to initial speed to try for
*              poll   - TRUE if turn around polling can be supported
*              postDIS - TRUE if this is the procedure after sending DIS
*                        in non-V34 mode and need to wait to detect another CI
*              timeout - carrier time-out period (msec)
*      Output: If V34 has not been selected then,
*                   FAX_PHASE_A_CED if this is a post DIS check where
*                                   FSK flags are being detected
*                   FAX_PHASE_A_CED_POLL if initial polling selected
*                   FAX_PHASE_A_TIMEOUT otherwise
*              Else if V34 has been selected then,
*                   FAX_PHASE_A_V34 if turn around polling not selected
*                   FAX_PHASE_A_V34_POLL if being polled
*                   *speed will contain the speed selected
*===========================================================================*/
UINT8 V8Answer( UINT8 *speed, BOOL poll, BOOL postDIS, UINT16 timeout )
{
	UINT16 rx_speed;
	UINT8 temp, status;
	int ib, ie;

	TRACE_MODEM( "V8Answer\n" );

	ModemControl->Status = NO_ERROR;
	ModemControl->V8Answer = TRUE;
	ModemControl->V8Polling = poll;

	/* Make sure ANSam transmission does not start */
	MDM_CLEARBIT( MODEM_09, DTR );

	/* Set up for V8 as an answerer */
	V8Setup( *speed, FALSE, postDIS, FALSE );

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	/* Should indication be sent that initial polling is allowed ? */
	if ( poll )
	{
		ModemDSPWrite1( MODEM_V8_AUTO_IPE, 0x40, TRUE );
		ModemDSPWrite1( MODEM_V8_CALL_FUNCTIONS, 0x30, FALSE );
	}
	else
	{
		/* Disable initial polling */
		temp = ModemDSPRead1( MODEM_V8_AUTO_IPE );
		ModemDSPWrite1( MODEM_V8_AUTO_IPE, temp & ~0x40, FALSE );
		ModemDSPWrite1( MODEM_V8_CALL_FUNCTIONS, 0x10, FALSE );
	}

	OSC_Release_Mutex( &modem_dsp_mutex );

	/* Start sending ANSam if enabled */
	MDM_SETBIT( MODEM_09, DTR );

#ifdef OEM_FAX_REPORT
	ModemControl->Level = ModemReadTxLevel();
	ModemControl->EQM   = 0xffff;
    CxProtocolReport( ANSam, NULL, 0, TX_LOG );
#endif

	/* Start a 3.8 second timer */
	MODEM_TIMER_START( TimerB, 3800 );
	ib = 4000 / MODEM_V8_INTERVAL;

	/* Loop until timeout or CM detected */
	while ( 1 )
	{
		/* Has CM been detected yet ? */
		temp = MDM_READ( MODEM_RXHSKSTAT );

		if ( temp >= 0x02 )
			break;

		/* Or is this a timeout ? */
		if ( MODEM_TIMER_EXPIRE( TimerB )
		  || ModemControl->Status
		  || ( --ib == 0 ) )
		{
			MDM_CLEARBIT( MODEM_09, DTR );
			/* Set signal gap timer */
			MODEM_TIMER_START( TimerEvent, 70 );

			TRACE_MODEM( "V8Answer: CM timeout!! %d, %d\n", ib, ModemControl->Status );

			return V8EarlyFinish( MODEM_FAX_PHASE_A_TIMEOUT );
		}

		OSC_Sleep( MODEM_V8_INTERVAL );
	}

	TRACE_MODEM( "V8Answer: CM detected\n" );

#ifdef OEM_FAX_REPORT
	ModemControl->Level = ModemReadRxLevel();
	ModemControl->EQM   = 0xffff;
    CxProtocolReport( V8CM, NULL, 0, RX_LOG );
#endif

	T30SpeakerControl( T30_SPEKER_V8_OFF );

	ModemClearIRQ();
	ModemControl->State = MODEM_V8_MODE;

	/* Start a T1 timeout timer */
	MODEM_TIMER_START( TimerB, timeout );
	ib = ( timeout + ( timeout / 20 ) ) / MODEM_V8_INTERVAL;
	MODEM_TIMER_START( TimerEvent, 7000 );
	ie = 7100 / MODEM_V8_INTERVAL;

	/* Now wait for either a timeout or the end of negotiations */
	while ( ModemControl->State == MODEM_V8_MODE )
	{
		if ( ( MODEM_TIMER_EXPIRE( TimerB ) )
		  || ( MODEM_TIMER_EXPIRE( TimerEvent ) )
		  || ModemControl->Status
		  || ( --ib == 0 )
		  || ( --ie == 0 ) )
		{
			MDM_CLEARBIT( MODEM_09, DTR );

			TRACE_MODEM( "V8Answer: timeout!! %d, %d\n", ib, ie );

			return V8EarlyFinish( MODEM_FAX_PHASE_A_TIMEOUT );
		}

		if ( ModemV8StatusIsr() )
		{
			ie = 7100 / MODEM_V8_INTERVAL;
		}

		OSC_Sleep( MODEM_V8_INTERVAL );
	}

	/* Time for initial flag */
	MODEM_TIMER_START( TimerEvent, 250 );

	/* Check for initial polling cases */
	temp = MDM_READ( MODEM_TXHSKSTAT );
	status = MODEM_FAX_PHASE_A_ABORT;

	if ( poll && MDM_BITSET( MODEM_09, ORG ) )
	{
		if ( temp == CC_DATA_MODE )
			status = MODEM_FAX_PHASE_A_V34_POLL;
		else if ( ( temp == 0x05 ) && ( MDM_READ( MODEM_CONF ) == MODEM_V21_CH2_CONF ) )
			status = MODEM_FAX_PHASE_A_CED_POLL;
		else if ( MDM_READ( MODEM_RXHSKSTAT ) == CC_DATA_MODE )
			status = MODEM_FAX_PHASE_A_V34_POLL;
	}

	/* Has the status been determined yet ? */
	if ( status != MODEM_FAX_PHASE_A_ABORT )
		;
	else if ( ( temp >= 0x02 ) && ( MDM_READ( MODEM_CONF ) == MODEM_V21_CH2_CONF ) )
		status = MODEM_FAX_PHASE_A_NON_V34;
	else if ( temp == CC_DATA_MODE )
		status = MODEM_FAX_PHASE_A_V34;
	else if ( MDM_READ( MODEM_RXHSKSTAT ) == CC_DATA_MODE )
		status = MODEM_FAX_PHASE_A_V34;
	else if ( ModemControl->Status == TIME_OUT )
		status = MODEM_FAX_PHASE_A_TIMEOUT;

	/* Was V34 mode selected ? */
	if ( ( status == MODEM_FAX_PHASE_A_V34 )
	  || ( status == MODEM_FAX_PHASE_A_V34_POLL ) )
	{
		ModemControl->V34Mode = TRUE;

		OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

		/* what speed was negotiated ? */
		rx_speed = V34SpeedTbl[ MDM_READ( MODEM_CONF ) - MODEM_V34_2400_CONF ];
		ModemControl->ConnectedSpeed = rx_speed;
		*speed = rx_speed + 1 + T30_SPEED_V17_14400;

		TRACE_DEBUG( "\nspeeds %x %x   JM %x %x %x\n",
		              ModemSpeedTbl[ ( MDM_READ( 0x0E ) & 0x1F ) - 0x03 ], rx_speed,
		              ModemDSPRead1( 0x32F ), ModemDSPRead1( 0x330 ), ModemDSPRead1( 0x331 ) );
		TRACE_MODEM( " ( Connect %d bps )\n", ModemControl->ConnectedSpeed );

		OSC_Release_Mutex( &modem_dsp_mutex );
	}
	else
	{
		V8EarlyFinish( status );
	}

	TRACE_MODEM("V8Answer: end\n");

	ModemControl->State = MODEM_IDLE;
	MDM_CLEARBIT( MODEM_1F, NSIE );

	return status;
}


/*===========================================================================
*    V8Originate
*      Sends CNG or CI and monitors either for AnsAM, CED, V.21 FSK 7E flags,
*      or a timeout
*      Input:  speed  - Pointer to initial speed to try for
*              poll   - TRUE if turn around polling is to be requested
*              postDIS - TRUE if this is the procedure is due to receiving
*                        a DIS with the V8 capability set in manual/delayed
*                        start cases (Ie in non-V34 mode)
*              timeout - carrier time-out period (msec)
*              abort_fn - Pointer to a function to call to check for aborts
*      Output: If line is busy then FAX_PHASE_A_TIMEOUT,
*              Else if the processing is aborted then FAX_PHASE_A_ABORT
*              Else if V34 negotiations selected fallback to V17,V29, or V27,
*                   FAX_PHASE_A_NON_V34
*              Else if V34 has not been selected then,
*                   FAX_PHASE_A_CED if CED or FSK flags are being detected
*              Else if V34 has been selected then,
*                   FAX_PHASE_A_V34 if turn around polling not done
*                   FAX_PHASE_A_V34_POLL if being polled
*                   *speed will contain the speed selected
*===========================================================================*/
UINT8 V8Originate( UINT8 *speed, BOOL poll, BOOL sendCI, UINT16 timeout, UINT8 ( *abort_fn ) ( void ) )
{
	UINT16 tx_speed;
	UINT8 temp, status;
	int ib, ie;

	TRACE_MODEM( "V8Originate\n" );

	if ( get_tone_status( TONE_DETECT_FSK ) )
	{
		set_phone_status( TONE_DETECT_FSK, FALSE );
		return V8EarlyFinish( MODEM_FAX_PHASE_A_CED );
	}

	ModemControl->Status = NO_ERROR;
	ModemControl->V8Answer = FALSE;
	ModemControl->V8Polling = poll;

	/* Set the carrier timeout timer */
	MODEM_TIMER_START( TimerB, timeout );

	ib = ( timeout + ( timeout / 20 ) ) / MODEM_V8_INTERVAL;

	/* Stop transmission while setting up */
	MDM_CLEARBIT( MODEM_09, DTR );

	/* ANS tones are being detected.  Discriminate between CED and ANSam */
	/* Setup for V8 handshake if necessary */

	V8Setup( *speed, TRUE, TRUE, sendCI );

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	/* Set up the call functions allowed */

	if ( poll )
	{
		ModemDSPWrite1( MODEM_V8_CALL_FUNCTIONS, 0x20, FALSE );
		ModemDSPWrite1( MODEM_V8_AUTO_IPE, 0x40, TRUE );
		temp = ModemDSPRead1( MODEM_V8_CONTROL_1 );
		temp |= 0xA0;
		ModemDSPWrite1( MODEM_V8_CONTROL_1, temp, FALSE );

		TRACE_MODEM( "V80Originate: Set Rx Call Function\n" );
	}
	else
	{
		ModemDSPWrite1( MODEM_V8_CALL_FUNCTIONS, 0x10, FALSE );

		TRACE_MODEM( "V80Originate: Set Tx Call Function\n" );
	}

	if ( sendCI )
	{
		/* Setup to transmit CI */
		ModemDSPWrite1( MODEM_V8_CONTROL_1, 0x04, TRUE );
	}

	OSC_Release_Mutex( &modem_dsp_mutex );

	ModemSetupToneDet( MODEM_FSK_TONE ); // set filter A to 1650Hz FSK carrier

	MDM_SETBIT( MODEM_09, DTR );

	temp = 0;

	/* And wait to see whether ANSam is detected ? */
	while ( 1 )
	{
		OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

		status = ModemDSPRead1( MODEM_V8_STATUS_1 );

		OSC_Release_Mutex( &modem_dsp_mutex );

		if ( status & 0x20 )
		{
			/* ANS or ANSam has been detected.  Now wait till end */
			TRACE_MODEM( "V8Originate:  ANS or ANSam detected\n" );

			MODEM_TIMER_START( TimerEvent, 3000 );
			ie = 3150 / MODEM_V8_INTERVAL;
			break;
		}

		if ( ModemToneDetected( MODEM_FSK_TONE ) )
		{
			temp++;

//			TRACE_MODEMDEBUG( "V8Originate:  FSK detected %d\n", temp );

			if ( temp == 10 )
			{
				TRACE_MODEM( "V8Originate: accepted FSK\n" );

				return V8EarlyFinish( MODEM_FAX_PHASE_A_CED );
			}
		}
		else if ( temp > 0 )
		{
			temp--;
		}

		if ( MODEM_TIMER_EXPIRE( TimerB )
		  || ModemControl->Status
		  || ( --ib == 0 ) )
		{
			return V8EarlyFinish( MODEM_FAX_PHASE_A_ABORT );
		}

		/* Check for abort or for scanner pullin paper jam */
		if ( abort_fn && abort_fn() )
		{
			return V8EarlyFinish( MODEM_FAX_PHASE_A_ABORT );
		}

		OSC_Sleep( MODEM_V8_INTERVAL );
	}

	temp = 0;

	/* And wait to see whether ANSam is detected ? */
	while ( 1 )
	{
		OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

		status = ModemDSPRead1( MODEM_V8_STATUS_2 );

		OSC_Release_Mutex( &modem_dsp_mutex );

		if ( status & 0x08 )
		{
			/* ANSam has been detected.  Now wait till end */
			TRACE_MODEM( "V8Originate:  ANSam detected\n" );

#ifdef OEM_FAX_REPORT
			ModemControl->Level = ModemReadRxLevel();
			ModemControl->EQM   = 0xffff;
            CxProtocolReport( ANSam, NULL, 0, RX_LOG );
#endif
			break;
		}

		if ( ModemToneDetected( MODEM_FSK_TONE ) )
		{
			temp++;

//			TRACE_MODEMDEBUG( "V8Originate:  FSK detected %d\n", temp );

			if ( temp == 10 )
			{
				TRACE_MODEM( "V8Originate: accepted FSK\n" );

				return V8EarlyFinish( MODEM_FAX_PHASE_A_CED );
			}
		}
		else if ( temp > 0 )
		{
			temp--;
		}

		if ( MDM_READ( MODEM_RXHSKSTAT ) >= 0x02 )
		{
			/* confirm remote does response with V.8 */
			TRACE_MODEM( " V8Originate: Got JM\n" );

#ifdef OEM_FAX_REPORT
			ModemControl->Level = ModemReadRxLevel();
			ModemControl->EQM   = 0xffff;
            CxProtocolReport( V8JM, NULL, 0, RX_LOG );
#endif
			break;
		}

		if ( MODEM_TIMER_EXPIRE( TimerEvent )
		  || ( --ie == 0 ) )
		{

#ifdef OEM_FAX_REPORT
			ModemControl->Level = ModemReadRxLevel();
			ModemControl->EQM   = 0xffff;
            CxProtocolReport( CED, NULL, 0, RX_LOG );
#endif
			return V8EarlyFinish( MODEM_FAX_PHASE_A_CED );
		}

		if ( ModemControl->Status )
		{
			return V8EarlyFinish( MODEM_FAX_PHASE_A_ABORT );
		}

		/* Check for abort or for scanner pullin paper jam */
		if ( abort_fn && abort_fn() )
		{
			return V8EarlyFinish( MODEM_FAX_PHASE_A_ABORT );
		}

		OSC_Sleep( MODEM_V8_INTERVAL );
	}

	ModemDisableToneDet( MODEM_FSK_TONE );
#if 1 //add.bong.2011-12-23 3:59pm
	MODEM_TIMER_START( TimerB, 28000 );
	ib = ( 28000 + ( 28000 / 20 ) ) / MODEM_V8_INTERVAL;
#endif //add.bong.
	MODEM_TIMER_START( TimerEvent, 7000 );

	ie = 7100 / MODEM_V8_INTERVAL;

	ModemControl->State = MODEM_V8_MODE;

	T30SpeakerControl( T30_SPEKER_V8_OFF );

	do {

		OSC_Sleep( MODEM_V8_INTERVAL );

#if 1	//mod.bong.2011-12-23 3:59pm
		if ( MODEM_TIMER_EXPIRE( TimerB )
		  || ModemControl->Status
		  || ( --ib == 0 ) )
		{
			TRACE_MODEM( "V8Originate: Bong timeout!! %d\n", ib );

			MDM_CLEARBIT( MODEM_09, DTR );

//			return V8EarlyFinish( MODEM_FAX_PHASE_A_ABORT );
			return V8EarlyFinish( MODEM_FAX_PHASE_A_CED );	//mod.bong.2011-12-23 4:00pm
		}
#endif

		if ( MODEM_TIMER_EXPIRE( TimerEvent )
		  || ModemControl->Status
		  || ( --ie == 0 ) )
		{
			TRACE_MODEM( "V8Originate: early timeout!! %d\n", ie );

			MDM_CLEARBIT( MODEM_09, DTR );

			return V8EarlyFinish( MODEM_FAX_PHASE_A_CED );
		}

		if ( ModemV8StatusIsr() )
		{
			ie = 7100 / MODEM_V8_INTERVAL;
		}

	} while ( ModemControl->State == MODEM_V8_MODE );

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	/* Disable interrupts */
	ModemDSPWrite1( MODEM_NEWS_MASK_16, 0, FALSE );
	ModemDSPWrite1( MODEM_NEWS_MASK_17, 0, FALSE );
	ModemDSPWrite1( MODEM_NEWS_MASK_MEM_ACCESS, MODEM_MEM_ACCESS_DISABLE, TRUE );

	OSC_Release_Mutex( &modem_dsp_mutex );

	/* What was decision based on the handshake */

	/* Check for initial polling cases */
	temp = MDM_READ( MODEM_TXHSKSTAT );
	status = MODEM_FAX_PHASE_A_ABORT;

	if ( poll )
	{
		if ( ( temp == 0x02 ) && ( MDM_READ( MODEM_CONF ) == MODEM_V21_CH2_CONF ) )
		{
			V8EarlyFinish( MODEM_FAX_PHASE_A_CED );
		}
		else if ( ( temp == CC_DATA_MODE ) || ( MDM_READ( MODEM_RXHSKSTAT ) == CC_DATA_MODE ) )
		{
			status = MODEM_FAX_PHASE_A_V34;
		}
	}

	/* Has the status been determined yet ? */
	if ( status != MODEM_FAX_PHASE_A_ABORT )
	{
		;
	}
	else if ( ( temp == 0x05 ) && ( MDM_READ( MODEM_CONF ) == MODEM_V21_CH2_CONF ) )
	{
		/* Was a non V34 mode negotiated ? */
		status = MODEM_FAX_PHASE_A_NON_V34;
	}
	else if ( temp == CC_DATA_MODE )
	{
		status = MODEM_FAX_PHASE_A_V34;
	}

	/* Was V34 mode selected ? */
	if ( ( status == MODEM_FAX_PHASE_A_V34 )
	  || ( status == MODEM_FAX_PHASE_A_V34_POLL ) )
	{
		ModemControl->V34Mode = TRUE;

		OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

		/* what speed was negotiated ? */
		tx_speed = ModemSpeedTbl[ ( MDM_READ( 0x0E ) & 0x1F ) - 0x03 ];
		ModemControl->ConnectedSpeed = tx_speed;
		*speed = tx_speed + 1 + T30_SPEED_V17_14400;

		TRACE_DEBUG( "\nspeeds %x %x   JM %x %x %x\n",
		              tx_speed,
		              V34SpeedTbl[ MDM_READ( MODEM_CONF ) - MODEM_V34_2400_CONF ],
		              ModemDSPRead1( 0x32F ),
		              ModemDSPRead1( 0x330 ),
		              ModemDSPRead1( 0x331 ) );
		TRACE_MODEM( " ( Connect %d bps )\n", ModemControl->ConnectedSpeed );

		OSC_Release_Mutex( &modem_dsp_mutex );

#ifdef OEM_FAX_REPORT
        CxProtocolReport( V34CC, NULL, 0, RX_LOG );
#endif

	}

	TRACE_MODEM( "V8Originate: end\n" );

	ModemControl->State = MODEM_IDLE;
	MDM_CLEARBIT( MODEM_1F, NSIE );

	return status;
}


void V8Tx_ANSam( void )
{
	V8Setup( T30_SPEED_V34_33600, FALSE, FALSE, FALSE );
	/* Start sending ANSam */
	MDM_SETBIT( MODEM_09, DTR );
}


void V8Tx_CI( void )
{
	V8Setup( T30_SPEED_V34_33600, TRUE, FALSE, TRUE );
	/* Setup to transmit CI */
	ModemDSPWrite1( MODEM_V8_CONTROL_1, 0x04, TRUE );
	/* Start sending CI */
	MDM_SETBIT( MODEM_09, DTR );
}


