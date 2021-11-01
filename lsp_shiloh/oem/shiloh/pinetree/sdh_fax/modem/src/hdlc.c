/*
 *  $RCSfile: hdlc.c,v $
 * $Revision: 1.43 $
 *   $Author: miurat $
 *     $Date: 2012/03/20 23:05:38 $
 */
/************************************************************************
 *
 *      hdlc.c
 *
 *  T.30 HDLC Frame Functions.
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
#include "../../faxapp/include/faxapp_notify.h"


T_HDLC_CONTROL *HDLCControl;


UINT8 HDLCAllocateMemory( void )
{
	/* Make sure the HDLC control structure is NULL */
	HDLCControl = ( T_HDLC_CONTROL * ) NULL;

	/* Allocate the Control structure for HDLC. */
	if ( OSC_Allocate_Zero_Memory( ( OSC_MEMORY_POOL * ) NULL, ( void ** ) &HDLCControl, sizeof( T_HDLC_CONTROL ), TRUE ) != E_OK )
	{
		TRACE_ERROR( "HDLCControl failed.\n" );
		return 1;
	}

	return 0;
}


void HDLCDeAllocateMemory( void )
{
	/* Deallocate and clear out the HDLC control structure */
	OSC_Deallocate_Memory( ( void * ) HDLCControl );

	HDLCControl = ( T_HDLC_CONTROL * ) NULL;
}


void HDLCTxWritePreviousFrameFinal( void )
{
	UINT8 prev;

	if ( HDLCControl->TxFrameBack == 0 )
		prev = MAX_HDLC_TX_FRAMES;
	else
		prev = HDLCControl->TxFrameBack - 1;
	HDLCControl->TxFrames[ prev ].final = 1;
}


void HDLCRxWriteInitFrame( void )
{
	T_HDLC_FRAME *frame = &HDLCControl->RxFrames[ HDLCControl->RxFrameBack ];

	memset( frame->info, 0, MAX_INFORMATION_FIELD );

	frame->addr = 0;
	frame->control = 0;
	frame->length = 0;
	frame->readbytenum = 0;
	frame->writebytenum = 0;
	frame->status = HDLC_NO_FRAME_YET;
	frame->fAddressField = 0;
	frame->fControlField = 0;
	frame->maxlength = MAX_INFORMATION_FIELD;
	frame->final = 0;
}


void HDLCTxWriteInitFrame( void )
{
	T_HDLC_FRAME *frame = &HDLCControl->TxFrames[ HDLCControl->TxFrameBack ];
	memset( frame->info, 0, MAX_INFORMATION_FIELD );

	frame->addr = 0xFF;
	frame->control = 0;
	frame->length = 2;  // for address and control
	frame->readbytenum = 0;
	frame->writebytenum = 0;
	frame->status = HDLC_NO_FRAME_YET;
	frame->fAddressField = 0;
	frame->fControlField = 0;
	frame->maxlength = MAX_INFORMATION_FIELD;
	frame->final = 0;
}


void HDLCTxWriteInitResendFrame( void )
{
	T_HDLC_FRAME *frame = &HDLCControl->TxFrames[ HDLCControl->TxFrameFront ];

	frame->readbytenum = 0;
	frame->fAddressField = 0;
	frame->fControlField = 0;
	frame->length = frame->writebytenum + 2;
}


void HDLCRxWriteInitAllFrames( void )
{
	UINT8 i;

	HDLCControl->RxFrameBack = 0;
	for ( i = 0; i < MAX_HDLC_RX_FRAMES; i++ )
	{
		HDLCRxWriteNextFrame();
		HDLCRxWriteInitFrame();
	}
	HDLCControl->RxFrameFront = HDLCControl->RxFrameBack = HDLCControl->RxFrameCount = 0;
}


void HDLCTxWriteInitAllFrames( void )
{
	UINT8 i;

	HDLCControl->TxFrameBack = 0;
	for ( i = 0; i < MAX_HDLC_TX_FRAMES; i++ )
	{
		HDLCTxWriteNextFrame();
		HDLCTxWriteInitFrame();
	}
	HDLCControl->TxFrameFront = HDLCControl->TxFrameBack = HDLCControl->TxFrameCount = 0;
}


UINT8 HDLCRxReadDataByte( void )
{
	/* Adjust the length */
	HDLCControl->RxFrames[ HDLCControl->RxFrameFront ].length--;

	/* Return the next byte */
	return ( HDLCControl->RxFrames[ HDLCControl->RxFrameFront ].info[ HDLCControl->RxFrames[ HDLCControl->RxFrameFront ].readbytenum++ ] );
}


void HDLCRxWriteDataByte( UINT8 data )
{
	T_HDLC_FRAME *frame = &HDLCControl->RxFrames[ HDLCControl->RxFrameBack ];

	/* This is an unauthorized, rule-violating,  hack. If the remote dropped */
	/*  the address field patch it with an artificial one. */
	if ( !frame->fAddressField && ( data == 0x03 || data == 0x13 ) )
	{
		frame->addr = 0xFF;
		frame->writebytenum = 0;
		frame->length = 0;
		frame->fAddressField = 1;
		frame->control = data;
		frame->fControlField = 1;
	}
	else if ( !frame->fAddressField || ( !frame->fControlField && data == 0xFF ) )
	{
		frame->addr = data;
		frame->writebytenum = 0;
		frame->length = 0;
		frame->fAddressField = 1;

#if 0
		TRACE_HDLC( "\n(%d)A[%x]", HDLCControl->RxFrameBack, data );
#endif

	}
	else if ( frame->fAddressField && !frame->fControlField )
	{
		frame->control = data;
		frame->writebytenum = 0;
		frame->length = 0;
		frame->fControlField = 1;

#if 0
		TRACE_HDLC( "C[%x]D", data );
#endif

	}
	else if ( frame->fAddressField
	       && frame->fControlField
	       && ( frame->length < MAX_INFORMATION_FIELD ) )
	{
		/* Adjust the length */
		frame->length++;

		/* Write the next byte to the frame buffer */
		frame->info[ frame->writebytenum++ ] = data;

#if 0
		if ( frame->writebytenum <= 3 )
			TRACE_HDLC( "[0x%x, 0x%x]", frame->info[ frame->writebytenum - 1 ],
			       frame->writebytenum - 1 );
		if(frame->writebytenum == 2)
			TRACE_HDLC(" rx frame_num %d\n", frame->info[ frame->writebytenum - 1 ]);
#endif

	}
}


UINT8 HDLCTxReadDataByte( void )
{
	T_HDLC_FRAME *frame = &HDLCControl->TxFrames[ HDLCControl->TxFrameFront ];

	if ( !frame->fAddressField )
	{
		frame->fAddressField = 1;
		frame->length--;
		return ( frame->addr );
	}
	else if ( !frame->fControlField )
	{
		frame->fControlField = 1;
		frame->length--;
		return ( frame->control );
	}
	else
	{
		UINT8 data = frame->info[ frame->readbytenum++ ];

		/* Adjust the length */
		frame->length--;

		/* And mark frame available if fully read */
		if ( !frame->length )
			HDLCControl->TxFrameCount--;

		/* Return the next byte */
		return ( data );
	}
}


void HDLCTxWriteDataByte( UINT8 data )
{
	/* Adjust the length */
	HDLCControl->TxFrames[ HDLCControl->TxFrameBack ].length++;

	/* Write the next byte to the frame buffer */
	HDLCControl->TxFrames[ HDLCControl->TxFrameBack ].info[ HDLCControl->TxFrames[ HDLCControl->TxFrameBack ].writebytenum++ ] = data;
}


UINT16 HDLCRxReadDataToBuff( UINT8 *cdb )
{
	T_HDLC_FRAME *frame = &HDLCControl->RxFrames[ HDLCControl->RxFrameFront ];
	UINT8 *src;
	UINT8 *dst;
	UINT16 size, size_return;

	/* Set up the pointers */
	src = &( frame->info[ frame->readbytenum ]);
	dst = cdb;
	size = frame->length;
	size_return = size;

	/* Mark HDLC frame read and the CDB written */
	frame->readbytenum += size;
	//cdb->size = size;
	frame->length = 0;

	/* And copy the data */
	memcpy( dst, src, size );

	return size_return;
}


void HDLCTxWriteDataFromCDB( CDB_BUFFER *cdb )
{
	T_HDLC_FRAME *frame = &HDLCControl->TxFrames[ HDLCControl->TxFrameBack ];
	UINT8 *src;
	UINT8 *dst;
	UINT16 size;

	/* Set up the pointers */
	dst = &( frame->info[ frame->writebytenum ] );
	src = ( UINT8 * ) cdb->data;
	size = cdb->size;

	/* Mark HDLC frame written */
	frame->writebytenum += size;
	frame->length += size;

	/* And copy the data */
	memcpy( dst, src, size );
}


void HDLCTxReadNextFrame( void )
{
	T_HDLC_FRAME *frame;

	HDLCControl->TxFrameFront = ( HDLCControl->TxFrameFront + 1 ) % MAX_HDLC_TX_FRAMES;
	frame = &HDLCControl->TxFrames[ HDLCControl->TxFrameFront ];
	frame->readbytenum = 0;
	frame->fAddressField = 0;
	frame->fControlField = 0;
}


void HDLCTxWriteNextFrame( void )
{
	HDLCControl->TxFrameBack = ( HDLCControl->TxFrameBack + 1 ) % MAX_HDLC_TX_FRAMES;
	HDLCControl->TxFrameCount++;
	HDLCTxWriteInitFrame();
}


void HDLCRxReadNextFrame( void )
{
	HDLCControl->RxFrameFront = ( HDLCControl->RxFrameFront + 1 ) % MAX_HDLC_RX_FRAMES;
	HDLCControl->RxFrameCount--;
	HDLCControl->RxFrames[ HDLCControl->RxFrameFront ].readbytenum = 0;
}


void HDLCRxReadPreviousFrame( void )
{
	if ( HDLCControl->RxFrameFront == 0 )
		HDLCControl->RxFrameFront = MAX_HDLC_RX_FRAMES;
	else
		HDLCControl->RxFrameFront--;

	HDLCControl->RxFrameCount++;
	HDLCControl->RxFrames[ HDLCControl->RxFrameFront ].readbytenum = 0;
}


void HDLCRxWriteNextFrame( void )
{
	T_HDLC_FRAME *frame = &HDLCControl->RxFrames[ HDLCControl->RxFrameBack ];

	if ( ( frame->length >= 2 ) && ( frame->writebytenum >= 2 ) )
	{
		/* Remove CRC */
		frame->length -= 2;
		frame->writebytenum -= 2;
	}

	HDLCControl->RxFrameBack = ( HDLCControl->RxFrameBack + 1 ) % MAX_HDLC_RX_FRAMES;
	HDLCControl->RxFrameCount++;
	HDLCRxWriteInitFrame();
}


UINT8 HDLCRxCurrentFramNum(void)
{
	return(HDLCControl->RxFrameBack);
}


/*===========================================================================
*
*    HDLCRxVerifyFlags
*
*  Verifies that HDLC flags are present for at least FlagVerifyTime
*
*  Input:      verify_time = amount of time to verify flags
*  Output:     return value = 1 if flags are on, 0 if not
*===========================================================================*/
UINT8 HDLCRxVerifyFlags( UINT16 verify_time )
{
	int ib;

	/* No HDLC flag detected? */
	if ( !ModemFlagSequence() )
		return ( 0 );

	/* Do not wait on a timer if ticks less than accuracy */
	MODEM_TIMER_START( TimerB, verify_time );
	ib = ( verify_time + ( verify_time / 20 ) ) / MODEM_DELAY_TIME;  // failsafe counter

	/* Timer expired? */
	while ( !MODEM_TIMER_EXPIRE( TimerB ) )
	{
		if ( --ib == 0 )
			break;

		OSC_Sleep( MODEM_DELAY_TIME );
	}

	/* No HDLC flag detected? */
	if ( !ModemFlagSequence() )
		return ( 0 );

	TRACE_HDLC( "+F+\n" );

	return ( 1 );
}


/*============================================================================
*
*    HDLCRxCheckFinalFrame
*
*  Checks if we just received a final HDLC frame, and turns off
*  interrupts, if so.  Waits for closing flags to be complete if
*  final frame.
*
*  Input:      HDLC Frame buffer contains the frame received
*  Output:     Clears interrupts if this was a final frame, or if the byte
*              count = 0, which shouldn't happen if we got EOF.
*              HDLC byte count as output parameter
*              return = 0, if byte count = 0, which shouldn't happen
*               or HDLC control field, if byte count > 0
*
*============================================================================*/
UINT8 HDLCRxCheckFinalFrame( void )
{
	UINT8 retval = 0;

	/* length = 0 means no command, indicate error and disable interrupts */
	if ( !HDLCRxReadLength() )
	{
		retval = 0;
		ModemClearIRQ();

		TRACE_HDLC( "  HDLCRxCheckFinalFrame: Zero length for frame %d ctl %x adr %x. Advancing to next frame.\n",
		       HDLCControl->RxFrameFront, HDLCRxReadControl(), HDLCRxReadAddress() );

		HDLCRxReadInitFrame();
	}

	/* Not the final frame, return the control field */
	else
	{
		retval = HDLCRxReadControl();

		TRACE_HDLC( "  HDLCRxCheckFinalFrame: Control field 0x%02X for frame %d\n",
		       retval, HDLCControl->RxFrameFront );

		if ( !ModemControl->V34Mode && ( retval != 0x03 ) )
		{
			/* Wait for the flags to be complete. Then clear interrupts. */
			while ( ModemFlagSequence() )
			{
				OSC_Sleep( MODEM_DELAY_TIME );
			}

			ModemClearIRQ();
		}
	}

	return ( retval );
}

/*===========================================================================
*    HDLCRxControlFrame
*
*  Waits for a T.30 HDLC Control frame to be received
*
*  Input:      None
*  Output:     ModemEOF = 1 if end of frame detected
*===========================================================================*/
UINT8 HDLCRxControlFrame( void )
{
	UINT8 status, cnt;
	int ib;

	TRACE_HDLC( " HDLCRxControlFrame: Start IRQ\n" );

	ModemControl->FoundFlag = FALSE;

	/* Enable Rx Data available interrupts  */
	ModemEnableRxInt();

	/* Start a 3 second timer  */
	MODEM_TIMER_START( TimerB, 3000 );
	ib = 3100 / MODEM_DELAY_TIME;  // failsafe counter
	cnt = 0;

	while ( 1 )
	{
		status = HDLCRxReadStatus();

		switch ( status )
		{
			/* Check for end of frame */
			case HDLC_NO_ERROR:

				TRACE_HDLC( "  Got frame [ %02X ]\n", HDLCRxReadCommand() );
				ModemDisableRxInt();
				return ( status );

			/* Check for control channel init*/
	       case HDLC_PC_TO_CC_TRANSITION:

				TRACE_HDLC( "  Time out of CC transition %d\n", MODEM_TIMER_EXPIRE( TimerB ) );
				ModemDisableRxInt();
				return ( status );

			/* Bad frame, advance to next frame */
			case HDLC_FRAME_ERROR:
			case HDLC_CHECKSUM_ERROR:

				TRACE_HDLC( "  Checksum Error\n" );
				ModemDisableRxInt();
				return ( status );

			default:
				break;
		}

		if ( !ModemControl->V34Mode
		  && ( T30Control->TriedV8 < 3 )
		  && ( CheckToneDetection( TONE_DETECT_CI, PHONE_TASK_PERIODIC_TIME ) == TONE_DETECT_CI ) )
		{
			if ( ++cnt == 3 )
			{
				TRACE_HDLC( "  Detected CI\n" );
				return ( HDLC_CI_TONE );
			}
		}
		else if ( cnt > 0 )
		{
			cnt--;
		}

		if ( MODEM_TIMER_EXPIRE( TimerB )
		  || ( --ib == 0 ) )
		{
			TRACE_HDLC( "  Time Out\n" );
			ModemDisableRxInt();
			return ( HDLC_TIME_OUT );
		}

		if ( ModemControl->Status & MODEM_ABORT )
		{
			TRACE_HDLC( " - abort %d\n", ModemControl->Status );
			return ( HDLC_CARRIER_LOST_ERROR );
		}

		OSC_Sleep( MODEM_DELAY_TIME );
	}
}


/*============================================================================
*
*    HDLCTxControlFrame
*
*  Transmits a T.30 HDLC Control Frame
*
*  Input:    HDLC control field = 0x03 for non-final frame, 0x13 for final frame
*
*  Output:   Transmits frame.  If non-final, transmitter is left on,
*            sending HDLC flags.  Otherwise, the transmitter is
*            turned off.
*============================================================================*/
void HDLCTxControlFrame( void )
{
	UINT8 final_frame;
	int ib, ie;

	TRACE_HDLC( " HDLCTxControlFrame()\n" );

	if ( ModemControl->Status & MODEM_ABORT )
	{
		TRACE_HDLC( " - abort %d\n", ModemControl->Status );
		return ;
	}

	ModemClearIRQ();

	ModemControl->State = HDLC_TX_MODE;
	ModemControl->Status = NO_ERROR;

	/* Make it possible for the FIQ ISR to do notifies */
	/* NOTE: Might want to use flag interrupt if timing problems */
	/*       show up in V.34 mode */
	ModemControl->HSFrameEndNotify = 0;

	/* Point to the next transmit frame */
	HDLCTxReadNextFrame();

	/* Is this the final frame? */
	final_frame = ( HDLCTxReadControl() == 0x13 );

	/* V.34 Mode */
	if ( ModemControl->V34Mode )
	{
		MODEM_TIMER_START( TimerB, 3000 );
		ib = 3100 / MODEM_DELAY_TIME;  // failsafe counter

		/* Wait for control channel data mode */
		while ( MDM_READ( MODEM_RXHSKSTAT ) != CC_DATA_MODE )
		{
			OSC_Sleep( MODEM_DELAY_TIME );

			if ( ModemControl->Status & MODEM_ABORT )
			{
				TRACE_HDLC( " - V34 abort @ CC_DATA_MODE! %d\n", ModemControl->Status );
				return ;
			}

			if ( MODEM_TIMER_EXPIRE( TimerB )
			  || ( --ib == 0 ) )
			{
				TRACE_HDLC( " - V34 timeout @ CC_DATA_MODE! %d\n", ib );

				ModemControl->State = MODEM_IDLE;
				return ;
			}
		}

		/* Wait for CTS to be active */
		while ( MDM_BITCLEAR( MODEM_0F, CTS ) )
		{
			OSC_Sleep( MODEM_DELAY_TIME );

			if ( ModemControl->Status & MODEM_ABORT )
			{
				TRACE_HDLC( " - V34 abort @ CTS! %d\n", ModemControl->Status );
				return ;
			}

			if ( MODEM_TIMER_EXPIRE( TimerB )
			  || ( --ib == 0 ) )
			{
				TRACE_HDLC( " - V34 timeout @ CTS! %d\n", ib );

				ModemControl->State = MODEM_IDLE;
				return ;
			}
		}

		MODEM_TIMER_START( TimerEvent, 250 );
		ie = 260 / MODEM_DELAY_TIME;  // failsafe counter

		while ( !MODEM_TIMER_EXPIRE( TimerEvent ) )
		{
			if ( --ie == 0)
				break;

			OSC_Sleep( MODEM_DELAY_TIME );
		}

		TRACE_HDLC( " ** HDLCTxControlFrame Exit from TimerEvent %d\n", ie );
		TRACE_HDLC( " - V.34 mode\n" );
	}

	/* Non-V.34 mode */
	else
	{
		/* If not already transmitting */
		if ( MDM_BITCLEAR( MODEM_08, RTS ) || ( MDM_READ( MODEM_CONF) != MODEM_V21_CH2_CONF ) )
		{
			/* Configure as V.21 transmitter (RTS on) and send preamble */
			/* Wait 1 second for opening flags to be sent */

			ModemTxConfigHDLC( T30_SPEED_V21_300 );

			TRACE_HDLC( " - send preamble\n" );

			OSC_Sleep( 1000 );
		}
	}

	TRACE_HDLC( " - Start IRQ\n" );

	/* Enable interrupts */
	ModemEnableTxInt();

	MODEM_TIMER_START( TimerB, 3000 );
	ib = 3100 / MODEM_DELAY_TIME;  // failsafe counter

	/* Wait until last byte of frame */
	do
	{
		OSC_Sleep( MODEM_DELAY_TIME );

		if ( ModemControl->Status & MODEM_ABORT )
		{
			TRACE_HDLC( " - abort @ TEOF! %d\n", ModemControl->Status );
			return ;
		}

		/* Bail if V34 mode and not in control channel data mode */
		if ( ModemControl->V34Mode && ( MDM_READ( MODEM_RXHSKSTAT ) != CC_DATA_MODE ) )
		{
			TRACE_HDLC(" - bailing, Final frame %d\n", final_frame);
			return ;
		}

		if ( MODEM_TIMER_EXPIRE( TimerB )
		  || ( --ib == 0 ) )
		{
			TRACE_HDLC( " - Timeout @ TEOF! %d\n", ib );

			ModemControl->State = MODEM_IDLE;
			return ;
		}
	} while ( MDM_BITCLEAR( MODEM_11, TEOF ) && MDM_BITSET( MODEM_1E, TDBIE ) );

	TRACE_HDLC( " - wait tx fifo empty\n" );

	/* Wait until FIFO empty */
	do
	{
		OSC_Sleep( MODEM_DELAY_TIME );

		if ( ModemControl->Status & MODEM_ABORT )
		{
			TRACE_HDLC( " - abort @ TDBIE! %d\n", ModemControl->Status );
			return ;
		}

		if ( MODEM_TIMER_EXPIRE( TimerB )
		  || ( --ib == 0 ) )
		{
			TRACE_HDLC( " - Timeout @ TDBIE! %d\n", ib );

			ModemControl->State = MODEM_IDLE;
			return ;
		}
	} while ( MDM_BITSET( MODEM_1E, TDBIE ) && MDM_BITCLEAR( MODEM_0A, FLAGS ) );

	TRACE_HDLC( " - wait EOF\n" );

	/* Wait until end of frame */
	do
	{
		OSC_Sleep( MODEM_DELAY_TIME );

		if ( ModemControl->Status & MODEM_ABORT )
		{
			TRACE_HDLC( " - abort @ FLAGS! %d\n", ModemControl->Status );
			return ;
		}

		if ( MODEM_TIMER_EXPIRE( TimerB )
		  || ( --ib == 0 ) )
		{
			TRACE_HDLC( " - Timeout @ FLAGS! %d\n", ib );

			ModemControl->State = MODEM_IDLE;
			return ;
		}
	} while ( MDM_BITCLEAR( MODEM_0A, FLAGS ) );

	TRACE_HDLC( " - EOF done\n" );

	/* Turn RTS off if final frame */
	if ( final_frame )
	{
		/* Clear RTS and wait for the transmitter to shut off */
		if ( !ModemControl->V34Mode )
		{
			/* Wait until some closing flags get transmitted */
			MODEM_TIMER_START( TimerB, 100 );
			ib = 110 / MODEM_DELAY_TIME;  // failsafe counter

			do
			{
				OSC_Sleep( MODEM_DELAY_TIME );
			} while ( !MODEM_TIMER_EXPIRE( TimerB )
			       && ( --ib > 0 ) );

			TRACE_HDLC( " - Time expire for RTS OFF %d\n", ib );

			MDM_CLEARBIT( MODEM_08, RTS );
		}

		TRACE_HDLC( " - Sent final frame\n" );
	}

	ModemControl->State = MODEM_IDLE;
}


/*===========================================================================
*
*    HDLCRxDataFrame
*
*  Initializes HDLC data reception for ECM mode.  Configures the modem
*  to the indicated speed (if necessary), and waits up to 5 seconds for
*  an opening HDLC flag
*
*  Input:  speed = desired data mode (index into modem speed table)
*
*  Index      Speed
*    0 V.21,   300
*    1 V.27,  2400
*    2 V.27,  4800
*    3 V.29,  7200
*    4 V.29,  9600
*    5 V.17,  7200
*    6 V.17,  9600
*    7 V.17, 12000
*    8 V.17, 14400

*  Output: ModemControl->State = HDLC_RX_MODE
*          RDBIE = 1
*          NSIE = 1 (Enable interrupts on SYNCD, FE, and PE)
*          If no opening flag detected within 5 seconds:
*            ModemControl->Status = end-of-frame conditions:
*
*              Bit  Description
*              ---  -----------
*               7   1 = abort command received
*               6-3 Not used
*               2   1 = end-of-frame detected
*               1   1 = CRC error detected
*               0   1 = loss of carrier detected
*============================================================================*/
void HDLCRxDataFrame( UINT8 speed, UINT8 restart, UINT8 training,
                      void ( *frame_end_notify ) ( void ), void ( *bad_frame_notify ) ( void ) )
{
	BOOL fsk = FALSE;
	int ia, ib = 0;

	if ( ModemControl->Status & MODEM_ABORT )
	{
		TRACE_HDLC( " HDLCRxDataFrame abort %d\n", ModemControl->Status );
		return ;
	}

	ModemControl->State = HDLC_RX_MODE;
	ModemControl->Status = NO_ERROR;
	ModemControl->IsrDataCounter = 0;

	/* If we're restarting clear out the frame */
	if ( restart )
	{
		HDLCRxWriteInitFrame();
	}

	/* For non-V.34 mode, configure modem */
	if ( !ModemControl->V34Mode )
	{
		/* Set modem tone detector to detect FSK flags */
		ModemSetupToneDet( MODEM_HISPEED_FSK_TONE );
		ModemSetupToneDet( MODEM_FSK_TONE );

		/* Start a 6.5 second timer */
		MODEM_TIMER_START( TimerA, 6500 );
		ia = 6600 / MODEM_DELAY_TIME;  // failsafe counter

		while ( 1 )
		{
			OSC_Sleep( MODEM_DELAY_TIME );

			if ( ModemControl->Status & MODEM_ABORT )
			{
				TRACE_HDLC( " HDLCRxDataFrame abort @ FED! %d\n", ModemControl->Status );
				return ;
			}

			if ( MODEM_TIMER_EXPIRE( TimerA )
			  || ( --ia == 0 ) )      /* Timed out ? */
			{
				TRACE_FAX( "ModemRxInitPage() No energy! Timed out.\n" );

				/* Indicate carrier not found */
				ModemControl->Status = TIME_OUT;
				ModemControl->State = MODEM_IDLE;
				ModemControl->HighSpeed = FALSE;
				return ;
			}

			if ( MDM_BITSET( MODEM_0F, FED ) )
			{
				ModemDSPWrite3( 0xABC, 0x4600 );
				ModemDSPWrite3( 0xABE, 0x1755 );
				break;
			}
		}

		while ( 1 )
		{
			OSC_Sleep( MODEM_DELAY_TIME );

			/* We've found HDLC flags.  Enable interrupts on parallel data */
			if ( HDLCRxVerifyFlags( ECM_FLAG_VERIFY_TIME ) )
			{
				break;
			}

			/* FSK tone detected? */

			if ( MDM_BITSET( MODEM_0A, FLAGDT ) )
			{
				/* Is this the start of the detection ? */

				if ( !fsk )
				{
					fsk = TRUE;
					/* Verify FSK for 150ms */
					MODEM_TIMER_START( TimerB, 150 );
					ib = 160 / MODEM_DELAY_TIME;  // failsafe counter
				}

				/* Detected for long enough ? */
				else if ( MODEM_TIMER_EXPIRE( TimerB )
				       || ( --ib == 0 ) )
				{
					ModemControl->HighSpeed = FALSE;
					break;
				}
			}

			/* FSK tone had been detected earlier but not on now */
			else if ( fsk )
			{
				fsk = FALSE;
			}

			/* Check for timeout */
			if ( MODEM_TIMER_EXPIRE( TimerA )
			  || ( --ia == 0 ) )      /* Timed out ? */
			{
				ModemControl->Status |= MODEM_ABORT;
				ModemControl->State  = MODEM_IDLE;
				ModemControl->HighSpeed = FALSE;
				break;
			}

			/* Check for abort */
			if ( ModemControl->Status & MODEM_ABORT )
			{
				ModemControl->State = MODEM_IDLE;
				ModemControl->HighSpeed = FALSE;
				break;
			}
		}
	}
	else
	{
		MDM_CLEARBIT( MODEM_0B, EQMAT );
		/* Clear out the RX FIFO */
		ModemDSPWrite1( MODEM_REINIT_EXTEND_FIFO, 0x80, TRUE );
		ModemDSPWrite1( MODEM_RX_FIFO_CLEAR, 0x08, TRUE );

		while ( MDM_BITSET( MODEM_0C, RXFNE ) )
			MDM_READ( MODEM_RBUFF );
	}
}


/*===========================================================================
*
*    HDLCTxDataFrame
*
*  Initializes HDLC data transmission for ECM mode.  Configures the modem
*  to the indicated speed, sends the training sequence, enables parallel
*  data mode interrupts, and returns.
*
*  Input:  speed = desired data mode (index into modem speed table)
*          training = short or long train
*          first_frame = flag indicating whether modem configuring is needed
*
*  Index      Speed
*    0 V.21,   300
*    1 V.27,  2400
*    2 V.27,  4800
*    3 V.29,  7200
*    4 V.29,  9600
*    5 V.17,  7200
*    6 V.17,  9600
*    7 V.17, 12000
*    8 V.17, 14400
*
*  Output: ModemControl->State = T4_TX_MODE
*          RTS = 1
*          TDBIE = 1
*============================================================================*/
void HDLCTxDataFrame( UINT8 speed, UINT8 training, UINT8 first_frame,
                      void ( *frame_end_notify ) ( void ) )
{
	ModemControl->State = HDLC_TX_MODE;
	ModemControl->Status = NO_ERROR;

	/* Set the notify functions */
	ModemControl->HSFrameEndNotify = frame_end_notify;

	/* Point to the next frame for HDLC buffer -> modem */
	HDLCTxReadNextFrame();

//	TRACE_HDLCDEBUG( "HDLCTxDataFrame: Start IRQ\n" );

	ModemEnableTxInt();
}


/*===========================================================================
*    HDLCTxInitialFlags
*
*  Initializes HDLC data transmission for ECM mode.  Configures the modem
*  to the indicated speed, sends the training sequence, and returns.  This
*  sequence starts off the sending of flags by the modem.
*
*  Input:  speed = desired data mode (index into modem speed table)
*          training = short or long train
*============================================================================*/
void HDLCTxInitialFlags( UINT8 speed, UINT8 training )
{
    int ib = 0;
    UINT16 tx_speed = 0;

	/* Non-V.34 mode */
	if ( !ModemControl->V34Mode )
	{
		ModemControl->State = HDLC_TX_MODE;
		ModemControl->Status = NO_ERROR;
		ModemControl->HSFrameEndNotify = 0;

		ModemClearIRQ();
		/* Give time for receiver */
		OSC_Sleep( 200 );
		MODEM_TIMER_START( TimerEvent, 50 );

		ModemSetTraining( training );
		ModemTxConfigHDLC( speed );

		/* Wait for modem to be ready */
		MODEM_TIMER_START( TimerB, 2000 );
		ib = 2100 / 100;

		while ( MDM_BITCLEAR( MODEM_0F, CTS ) && !MODEM_TIMER_EXPIRE( TimerB ) )
		{
			if ( --ib == 0 )
				break;

			OSC_Sleep( 100 );
		}

		TRACE_HDLC( "HDLCTxInitialFlags: %s %d\n", MODEM_TIMER_EXPIRE( TimerB ) ? "Timeout" : "CTS ON", ib );
	}

	/* V.34 mode */
	else
	{
		MDM_SETBIT( MODEM_08, RTS );

		/* Wait for modem to be ready */
		MODEM_TIMER_START( TimerB, 2000 );
		ib = 2100 / MODEM_DELAY_TIME;  // failsafe counter

		while ( ( MDM_READ( MODEM_TXHSKSTAT ) != PC_TX_DATA_MODE ) && !MODEM_TIMER_EXPIRE( TimerB ) )
		{
			if ( --ib == 0 )
				break;

			OSC_Sleep( MODEM_DELAY_TIME );
		}

		TRACE_HDLC( "HDLCTxInitialFlags: %s %d\n", MODEM_TIMER_EXPIRE( TimerB ) ? "Timeout" : "Start PC", ib );
	}

    ModemConnectedSpeed(speed);

	ModemControl->HighSpeed = TRUE;

#ifdef OEM_FAX_REPORT
    tx_speed = (ModemControl->ConnectedSpeed / 1000);

	ModemControl->Level = ModemReadTxLevel();
	ModemControl->EQM   = 0xffff;
	CxProtocolReport( IMG, &tx_speed, 1, TX_LOG );
#endif

	/* Allow enough initial flags */
	OSC_Sleep( 200 );
}


/*============================================================================
*
*    HDLCRxCommandNone
*
*  Returns no command found status to caller at end of HDLCRxCommand sequence
*=============================================================================*/
UINT8 HDLCRxCommandNone( void )
{
	ModemControl->State = MODEM_IDLE;
	ModemClearIRQ();
	MODEM_TIMER_START( TimerEvent, 50 );
	return ( 0 );
}


/*============================================================================
*
*    HDLCRxCommand
*
*  Receives a T.30 HDLC command.  Does the COMMAND RECEIVED block
*  of the T.30 flow chart.   For finding the first T.30 command
*  the first_command parameter should be set to TRUE.  If
*  the return value indicates that the frame was a good, non-final frame
*  the first_command parameter should be set to FALSE.  Modem interrupts will
*  still be enabled, so it is important to continue to call HDLCRxCommand
*  until the final frame is received or an error is detected.
*
*  Input:   T2_TIMEOUT = T2 time-out period
*           first_command: TRUE if finding the first T30 command
*  Output:  If a command is received:
*              return value = byte count > 0
*              control_field = HDLC control field
*              HDLC Frame buffer contains the frame
*
*           If no command is received:
*              return value = byte count = 0
*              ModemControl->Status = error code:
*                NO_COMMAND - No opening flag, or no closing flag
*                DISCONNECT - FCS error and signal energy does not go away
*
*          T.30 T2 timer is reset to 6.7 seconds when an opening flag
*            is detected.
*============================================================================*/
UINT8 HDLCRxCommand( UINT8 first_command, void *T2_timer )
{
	UINT8 status;
	int ia;

	TRACE_HDLC( "Enter HDLCRxCommand %d\n", first_command );

	if ( first_command )
		HDLCRxWriteInitAllFrames();

	if ( first_command && !ModemControl->V34Mode )
	{
		/* Debounce start of preamble */
		HDLCControl->VfyFlags = 1;

		if ( MDM_READ( MODEM_CONF ) != MODEM_V21_CH2_CONF )
		{
			/* Clear IRQs and configure as HDLC Receiver */
			ModemClearIRQ();
			MDM_CLEARBIT( MODEM_09, DTMF + ORG + DTR );
			ModemRxConfigHDLC( T30_SPEED_V21_300 );
		}
	}
	else  /* Don't debounce flags */
	{
		HDLCControl->VfyFlags = 0;
		MDM_CLEARBIT( MODEM_0D, PCOFF );
	}

	/* Wait up to T2 time for flags */
	if ( !ModemControl->V34Mode )
	{
		MODEM_TIMER_START( TimerA, T2_TIMEOUT );
		ia = ( T2_TIMEOUT + ( T2_TIMEOUT / 20 ) ) / MODEM_DELAY_TIME;  // failsafe counter

		while ( first_command && !ModemFlagSequence() )
		{
			if ( ModemControl->Status & MODEM_ABORT )
			{
				TRACE_HDLC( " HDLCRxCommand abort %d\n", ModemControl->Status );
				return 0;
			}

			if ( MODEM_TIMER_EXPIRE( TimerA )
			  || ( --ia == 0 ) )
			{
				TRACE_HDLC( " HDLCRxCommand timeout %d\n", ia );
				ModemControl->Status = TIME_OUT;
				HDLCRxCommandNone();
				return 0;
			}

			if ( ( HDLCRxReadStatus() == HDLC_FRAME_ERROR )
		      || ( HDLCRxReadStatus() == HDLC_CHECKSUM_ERROR ) )
			{
				TRACE_HDLC( " HDLCRxCommand unkown CRC error\n" );
				HDLCRxWriteInitFrame();
#if 0
				HDLCControl->VfyFlags = 1;    /* Verify 100 msec of flags */
				HDLCRxCommandNone();
				ModemControl->Status = FRAME_ERROR;
				return 0;
#endif
			}

			OSC_Sleep( MODEM_DELAY_TIME );
		}

		/* Verify 100 msec of flags if this is the first frame */
		if ( HDLCControl->VfyFlags && !HDLCRxVerifyFlags( T30_FLAG_VERIFY_TIME ) )
		{
			TRACE_HDLC( " HDLCRxCommand not enough flag\n" );
			ModemControl->Status = NO_COMMAND;
			HDLCRxCommandNone();
			return 0;
		}

		/* Reset T2 Timer (default = 6.7 msec) */
		t30_timer_start( (void *)T2_timer, T2_TIMEOUT);

	}

	ModemControl->State = HDLC_RX_MODE;
	ModemControl->Status = NO_ERROR;
	ModemControl->BytesPerFrame = 256;

	if ( first_command )
		ModemControl->IsrDataCounter = 0;

	/* Prepare to receive the frame */
	status = HDLCRxControlFrame();

	//MODEM_TIMER_START( TimerEvent, 50 );

	/* Report the results */
	switch ( status )
	{
			/* Good Frame */
		case HDLC_NO_ERROR:

			/* Reset T2 Timer (default = 6.7 msec) */

			t30_timer_start( (void *)T2_timer, T2_TIMEOUT);

			ModemControl->Status = NO_ERROR;
			return ( HDLCRxCheckFinalFrame() );

			/* V.34 mode: PC to CC transition */
		case HDLC_PC_TO_CC_TRANSITION:

			return HDLC_PC_TO_CC_TRANSITION;

			/* Bad or Ugly Frame */
		case HDLC_FRAME_ERROR:
		case HDLC_CHECKSUM_ERROR:

			if ( T30Control->State == STATE_RX_PHASE_D )
			{
				if ( ModemControl->SendCRP )
				{
					TRACE_HDLC( " HDLCRxCommand Frame error\n" );

					t30_timer_start( (void *)T2_timer, T2_TIMEOUT);

					ModemControl->SendCRP = FALSE;
					ModemControl->Status = FRAME_ERROR;
				}
				else
				{
					TRACE_HDLC( " HDLCRxCommand Unknown CRC error\n" );
					ModemControl->Status = NO_COMMAND;
					HDLCRxReadNextFrame();
				}
			}
			else if ( HDLCRxReadFinalFrame() )
			{
				TRACE_HDLC( " HDLCRxCommand Frame error\n" );
				/* Reset T2 Timer (default = 6.7 msec) */
				t30_timer_start( (void *)T2_timer, T2_TIMEOUT);

				ModemControl->Status = FRAME_ERROR;
			}
			else
			{
				TRACE_HDLC( " HDLCRxCommand Unknown CRC error\n" );
				ModemControl->Status = NO_COMMAND;
				HDLCRxReadNextFrame();
			}
			break;

		case HDLC_TIME_OUT:

			TRACE_HDLC( " HDLCRxCommand Time out\n" );
			ModemControl->Status = TIME_OUT;
			break;

		case HDLC_CI_TONE:

			TRACE_HDLC( " HDLCRxCommand Detected CI tone\n" );
			ModemControl->Status = CI_TONE;
			break;

		case HDLC_NO_FRAME_YET:
		default:

			ModemControl->Status = NO_RESPONSE;
			HDLCRxReadNextFrame();
			break;
	}

	return ( HDLCRxCommandNone() );
}


/*============================================================================
*
*    HDLCRxResponse
*
*  Receives a T.30 HDLC response.  Does the RESPONSE RECEIVED block
*  of the T.30 flow chart.
*
*  Input:  timeout - timeout value for carrier timeout
*          flag_notify - notification function that flags have been detected
*          abort_notify - notification function that the command is being aborted
*  Output: If a response is received:
*            return value = Control Feild ( 0x03 or 0x13 )
*            HDLC Frame buffer contains the frame
*
*          If detect V.8 CI
*            return = HDLC_CI_TONE
*
*          If V.34 PC to CC transition
*            return = HDLC_PC_TO_CC_TRANSITION
*
*          If no response is received:
*            return value = 0
*            ModemControl->Status = error_code
*              NO_RESPONSE - No opening flag, or no closing flag
*              DISCONNECT - FCS error and signal energy does not go away
*
*          status:
*               HDLC_NO_ERROR
*               HDLC_NO_CARRIER_ERROR
*               HDLC_FRAME_ERROR
*               HDLC_CHECKSUM_ERROR
*               HDLC_TIME_OUT
*               HDLC_NO_FRAME_YET
*
*============================================================================*/
UINT8 HDLCRxResponse( UINT8 first_command, UINT8 *status, UINT16 timeout,
                      void ( *flag_notify ) ( void ), void ( *abort_notify ) ( void ) )
{
	UINT8 result, cnt;
	int ia;

	TRACE_HDLC( "Enter HDLCRxResponse %d\n", first_command );

	/* For first command, debounce flags */
	if ( first_command && !ModemControl->V34Mode )
	{
		ModemClearIRQ();
		ModemRxConfigHDLC( T30_SPEED_V21_300 );
		if ( *status == MODEM_CI_TONE )
		{
			/* Set tone detectors to detect CI */
			SetUpToneDetection( TONE_DETECT_CI, 3500 );
		}
		HDLCControl->VfyFlags = 1;    /* Verify 100 msec of flags */
		HDLCRxWriteInitFrame();
	}
	else
	{
		HDLCControl->VfyFlags = 0;   /* Don't verify flags */
		MDM_CLEARBIT( MODEM_0D, PCOFF );
	}

	/* Start up the timer supplied by caller */
	MODEM_TIMER_START( TimerA, timeout );
	ia = ( timeout + ( timeout / 20 ) ) / MODEM_DELAY_TIME;  // failsafe counter

	/* Loop until HDLC flags are found or the timer expires */
	if ( !ModemControl->V34Mode )
	{
		while ( 1 )
		{
			cnt = 0;

			if ( ModemControl->Status & MODEM_ABORT )
			{
				TRACE_HDLC( " HDLCRxResponse abort %d\n", ModemControl->Status );

				*status = HDLC_NO_CARRIER_ERROR;
				return ( 0 );
			}

			while ( first_command && !ModemFlagSequence() )
			{
				if ( ModemControl->Status & MODEM_ABORT )
				{
					TRACE_HDLC( " HDLCRxResponse abort %d\n", ModemControl->Status );

					*status = HDLC_NO_CARRIER_ERROR;
					return ( 0 );
				}

				if ( MODEM_TIMER_EXPIRE( TimerA )
				  || ( --ia == 0 ) )
				{
					TRACE_HDLC( " HDLCRxResponse T4 Timeout\n" );

					ModemDisableToneDet( MODEM_CI_TONE );
					ModemControl->Status = NO_RESPONSE;
					ModemControl->State  = MODEM_IDLE;
					ModemClearIRQ();
					*status = HDLC_NO_CARRIER_ERROR;
					return ( 0 );
				}
				else if ( ( T30Control->TriedV8 < 3 )
				       && ( CheckToneDetection( TONE_DETECT_CI, PHONE_TASK_PERIODIC_TIME ) == TONE_DETECT_CI ) )
				{
					if ( ++cnt == 3 )
					{
						TRACE_HDLC( " HDLCRxResponse Detected CI\n" );

						ModemDisableToneDet( MODEM_CI_TONE );
						ModemControl->Status = CI_TONE;
						ModemControl->State  = MODEM_IDLE;
						ModemClearIRQ();
						*status = HDLC_CI_TONE;
						return ( 0 );
					}
				}
				else if ( cnt > 0 )
				{
					cnt = 0;
				}

				OSC_Sleep( MODEM_DELAY_TIME );
			}

			/* Verify 100 msec of flags if this is the first frame */
			if ( HDLCControl->VfyFlags && !HDLCRxVerifyFlags( T30_FLAG_VERIFY_TIME ) )
			{
				OSC_Sleep( T30_FLAG_VERIFY_TIME );

				if ( ModemFlagSequence() )
					break;
			}
			else
				break;
		}
	}

	/* Notify whoever is interested that flags have been detected */
	if ( flag_notify )
		( *flag_notify )();

	ModemControl->State = HDLC_RX_MODE;
	ModemControl->Status = NO_ERROR;
	ModemControl->BytesPerFrame = 256;

	if ( first_command )
		ModemControl->IsrDataCounter = 0;

	/* Receive the frame */
	result = HDLCRxControlFrame();

	if ( result == HDLC_CI_TONE )
	{
		ModemDisableToneDet( MODEM_CI_TONE );
		ModemControl->Status = CI_TONE;
		ModemControl->State  = MODEM_IDLE;
		ModemClearIRQ();
		*status = HDLC_NO_FRAME_YET;
		MODEM_TIMER_START( TimerEvent, 50 );
		return ( MODEM_CI_TONE );
	}

	ModemDisableToneDet( MODEM_CI_TONE );

	MODEM_TIMER_START( TimerEvent, 50 );

	/* Notify whoever is interested if an abort condition has been detected */
	if ( abort_notify )
		( *abort_notify )();

	/* Report the results */
	switch ( result )
	{
			/* Good Frame */
		case HDLC_NO_ERROR:

			TRACE_HDLC( " HDLCRxResponse Good frame\n" );

			ModemControl->Status = NO_ERROR;
			*status = HDLC_NO_ERROR;
			return ( HDLCRxCheckFinalFrame() );

			/* Bad Frame */
		case HDLC_FRAME_ERROR:
		case HDLC_CHECKSUM_ERROR:

			if ( T30Control->State == STATE_TX_PHASE_D2 )
			{
				if ( ModemControl->SendCRP )
				{
					TRACE_HDLC( " HDLCRxResponse Frame error\n" );
					ModemControl->SendCRP = FALSE;
					ModemControl->Status = FRAME_ERROR;
					*status = HDLC_CHECKSUM_ERROR;
					HDLCRxReadNextFrame();
				}
				else
				{
					TRACE_HDLC( " HDLCRxResponse Unknown CRC error\n" );
					ModemControl->Status = NO_COMMAND;
					*status = HDLC_CHECKSUM_ERROR;
					HDLCRxReadNextFrame();
				}
			}
			else if ( HDLCRxReadFinalFrame() )
			{
				TRACE_HDLC( " HDLCRxResponse Frame error\n" );
				ModemControl->Status = FRAME_ERROR;
				*status = HDLC_CHECKSUM_ERROR;
				HDLCRxReadNextFrame();
			}
			else
			{
				TRACE_HDLC( " HDLCRxResponse Unknown CRC error\n" );
				ModemControl->Status = NO_COMMAND;
				*status = HDLC_CHECKSUM_ERROR;
				HDLCRxReadNextFrame();
			}

			return ( HDLCRxCommandNone() );

			/* V.34 mode: PC to CC transitions */
		case HDLC_PC_TO_CC_TRANSITION:

			return ( HDLC_PC_TO_CC_TRANSITION );

		case HDLC_TIME_OUT:

			TRACE_HDLC( " HDLCRxResponse Time out\n" );

			ModemControl->Status = TIME_OUT;
			*status = HDLC_TIME_OUT;
			return ( HDLCRxCommandNone() );

			/* Ugly Frame (timeout or signal dropout) */
		case HDLC_NO_FRAME_YET:
		default:

			TRACE_HDLC( " HDLCRxResponse No carrier\n" );

			ModemControl->Status = NO_RESPONSE;
			*status = HDLC_NO_CARRIER_ERROR;
			return ( HDLCRxCommandNone() );
	}
}

