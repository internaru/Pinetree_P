/*
 *  $RCSfile: modemisr.c,v $
 * $Revision: 1.42 $
 *   $Author: miurat $
 *     $Date: 2012/03/20 23:05:38 $
 */
/************************************************************************
 *
 *      modemisr.c
 *
 *  SSF336 Interrupt Service Routines.
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
#include "HDLC.H"
#include "ECMX.H"
#include "ECM.H"
#include "modem336.h"
#include "faxapp_notify.h"


#ifdef SPI_MULTI_DATA

#define SPI_MULTI_TX_SIZE   8
#define SPI_MULTI_RX_SIZE   6

#define HDLC_STAT_MASK  0x74
#define HDLC_PE_SYNCD   0x44
#define HDLC_PE         0x40
#define HDLC_FE         0x20
#define HDLC_OE         0x10
#define HDLC_OE_SYNCD   0x14
#define HDLC_SYNCD      0x04
#define HDLC_GOOD_DATA  0x00

#else

#define HDLC_STAT_MASK  0x39
#define HDLC_PE_SYNCD   0x21
#define HDLC_PE         0x20
#define HDLC_FE         0x10
#define HDLC_OE         0x08
#define HDLC_OE_SYNCD   0x09
#define HDLC_SYNCD      0x01
#define HDLC_GOOD_DATA  0x00

#endif

static void ModemRxCidIsr( void );
UINT8 ModemV8StatusIsr( void );
void ModemIdleStatusIsr( void );

extern CDB_BUFFER cdb_buffer[MAX_ECM_BLOCKS][MAX_ECM_FRAMES];
extern void ModemCheckRTC( UINT8 in_data );

extern OSC_MUTEX modem_dsp_mutex;

/* Modem Interrupt Vector Tables */

/* New Configuration Interrupts */
void ( *ModemNewConfigIntTbl[] ) ( void ) = {
	ModemBadIsr,                /* IDLE    - shouldn't be here */
	ModemBadIsr,                /* HDLC RX - shouldn't be here */
	ModemBadIsr,                /* HDLC TX - shouldn't be here */
	ModemBadIsr,                /* T.4 RX  - shouldn't be here */
	ModemBadIsr,                /* T.4 TX  - shouldn't be here */
	ModemBadIsr,                /* Flags RX - shouldn't be here */
	ModemBadIsr,                /* CID RX - shouldn't be here */
	ModemBadIsr,                /* V8 V34 negotiations - shouldn't be here */
	ModemBadIsr,                /* Tx TCF */
	ModemBadIsr,                /* Rx TCF */
#if (TAM==Internal)
	ModemBadIsr,                /* Voice RX - shouldn't be here */
	ModemBadIsr                 /* Voice TX - shouldn't be here */
#endif
};

/* New Status interrupts */
void ( *ModemNewStatusIntTbl[] ) ( void ) = {
	ModemIdleStatusIsr,         /* IDLE - look for RI */
	ModemRxHDLCNewStatusIsr,    /* HDLC RX - new status */
	ModemTxHDLCNewStatusIsr,    /* HDLC TX - new status */
	ModemBadIsr,                /* T.4 RX  - shouldn't be here */
	ModemBadIsr,                /* T.4 TX  - shouldn't be here */
	ModemRxFlagsIsr,            /* Flags RX - Notify absense of HDLC flags */
	ModemBadIsr,                /* CID RX - shouldn't be here */
	ModemBadIsr,                /* V8 V34 negotiation status updates */
	ModemBadIsr,                /* Tx TCF */
	ModemBadIsr,                /* Rx TCF */
#if (TAM==Internal)
	ModemBadIsr,                /* Voice RX - shouldn't be here */
	ModemBadIsr                 /* Voice TX - shouldn't be here */
#endif
};

/* Transmit Data interrupts */
void ( *ModemTxDataIntTbl[] ) ( void ) = {
	ModemBadIsr,                /* IDLE    - shouldn't be here */
	ModemBadIsr,                /* HDLC RX - shouldn't be here */
	ModemTxHDLCIsr,             /* HDLC TX - transmit a HDLC frame byte */
	ModemBadIsr,                /* T.4 RX  - shouldn't be here */
	ModemTxT4Isr,               /* T.4 TX  - transmit an image data byte  */
	ModemBadIsr,                /* Flags RX - shouldn't be here */
	ModemBadIsr,                /* CID RX - receive a V.23 Caller Id. data byte */
	ModemBadIsr,                /* V8 V34 negotiations - shouldn't be here */
	ModemTxTCFIsr,              /* Tx TCF  */
	ModemBadIsr,                /* Rx TCF */
#if (TAM==Internal)
	ModemBadIsr,                /* Voice RX - shouldn't be here   */
	ModemTxVoiceIsr             /* Voice TX - play back a voice message */
#endif
};

/* Receive Data interrupts */
void ( *ModemRxDataIntTbl[] ) ( void ) = {
	ModemBadIsr,                /* IDLE    - shouldn't be here */
	ModemRxHDLCIsr,             /* HDLC RX - receive a HDLC frame byte */
	ModemBadIsr,                /* HDLC TX - shouldn't be here */
	ModemRxT4Isr,               /* T.4 RX  - receive an image data byte */
	ModemBadIsr,                /* T.4 TX  - shouldn't be here    */
	ModemBadIsr,                /* Flags RX - shouldn't be here */
	ModemRxCidIsr,              /* CID RX - receive a V.23 Caller Id. data byte */
	ModemBadIsr,                /* V8 V34 negotiations - shouldn't be here */
	ModemBadIsr,                /* Tx TCF */
	ModemRxTCFIsr,              /* Rx TCF */
#if (TAM==Internal)
	ModemRxVoiceIsr,            /* Voice RX - record a voice message  */
	ModemBadIsr                 /* Voice TX - shouldn't be here */
#endif
};

/*===========================================================================
*
*   ModemIsr
*
*   Modem IRQ interrupt service routine
*
*   Input:    ModemControl->State = Modem current state
*   Output:   (none)
*
*   NOTE:     This ISR expects the processor specific registers to be saved
          and restored elsewhere
*===========================================================================*/
void ModemIsr( void *isr_data )
{
	UINT8 reg1F, reg1E;
	UINT8 limit = 2;

	extern UINT8 spi_access;

	if ( ( ModemControl->State == MODEM_IDLE )
	  || ( ModemControl->Status & MODEM_ABORT ) )
	{
		return ;
	}

	if ( spi_access )
	{
//		TRACE_DEBUG( " ** skip ModemIsr **\n" );
		return ;
	}

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	while ( limit-- )
	{
		reg1F = MDM_READ( MODEM_1F );
		reg1E = MDM_READ( MODEM_1E );

		if ( ( reg1F & ( NSIE + NCIE ) ) || ( reg1E & ( TDBIE + RDBIE ) ) )
		{
//			TRACE_CONTINUE_IRQ( "ModemIsr 1F = 0x%02X, 1E = 0x%02X\n", reg1F, reg1E );

			/* Service new configuration interrupt */
			if ( ( reg1F & ( NCIA + NCIE ) ) == ( NCIA + NCIE ) )
			{
				( *ModemNewConfigIntTbl[ ModemControl->State ] )();
			}
			/* Service new status interrupt */
			else if ( ( reg1F & ( NSIA + NSIE ) ) == ( NSIA + NSIE ) )
			{
				( *ModemNewStatusIntTbl[ ModemControl->State ] )();
			}
#ifdef SPI_MULTI_DATA
  #if ( SPI_MULTI_TX_SIZE == 8 )
			else if ( ( reg1E & TDBIE ) && MDM_BITCLEAR( MODEM_01, TXHF ) )
			{
				( *ModemTxDataIntTbl[ ModemControl->State ] )();
			}
  #endif
#endif
			/* Service transmit data interrupt */
			else if ( ( reg1E & ( TDBIA + TDBIE ) ) == ( TDBIA + TDBIE ))
			{
				( *ModemTxDataIntTbl[ ModemControl->State ] )();
			}
			/* Service receive data interrupt */
			else if ( ( reg1E & ( RDBIA + RDBIE ) ) == ( RDBIA + RDBIE ) )
			{
				( *ModemRxDataIntTbl[ ModemControl->State ] )();
			}
			else if ( ( reg1E & RDBIE ) && MDM_BITSET( MODEM_0C, RXFNE ) )
			{
				( *ModemRxDataIntTbl[ ModemControl->State ] )();
			}
			/* Service V.8 state */
			else if ( ModemControl->State == MODEM_V8_MODE )
			{
				( *ModemNewStatusIntTbl[ ModemControl->State ] )();
				break;
			}
			else
			{
#ifdef DBG_IRQ_PRINTF
//				TRACE_CONTINUE_IRQ( "ModemIsr Warning! Extra interrupt\n" );
#endif
				break;
			}
		}
		else
			break;
	}

	OSC_Release_Mutex( &modem_dsp_mutex );

//	TRACE_DEBUG( "Ex\n" );
}



/*===========================================================================
*
*    ModemBadIsr
*
*   Called when an interrupt occurs when no interrupt should occur.
*   Disables interrupts and clears the interrupt flags.
*
*   Input:      (none)
*   Output:     All modem interrupts disabled, and interrupt and status
*           flags cleared.
*
*===========================================================================*/
void ModemBadIsr( void )
{
#ifdef DBG_IRQ_PRINTF
	TRACE_IRQ( "=B%d,%x,%x=", ModemControl->State, MDM_READ( MODEM_1F ), MDM_READ( MODEM_1E ) );
#endif

	ModemClearIRQ();
}


void ModemIdleStatusIsr( void )
{
	MDM_CLEARBIT( MODEM_1F, NEWS );
}
/*===========================================================================
*
*    ModemRxOff
*
*
*   Input:      (none)
*   Output:
*
*===========================================================================*/
void ModemRxOff( void )
{
#ifdef DBG_IRQ_PRINTF
	TRACE_IRQ( "ModemRxOff()\n" );
#endif

	ModemClearIRQ();
	ModemControl->State = MODEM_IDLE;
}

/*===========================================================================
*
*    ModemRxT4NewStatusIsr
*
*    Handles Rx Data status interrupts
*
*   Input:      (none)
*   Output:     NSIE = 0 - disables new status interrupts
*
*===========================================================================*/
void ModemRxHDLCNewStatusIsr( void )
{
	/* For primary to control channel transitions, mark the frame complete */
	/* and turn off new status interrupts */
	MDM_CLEARBIT( MODEM_1F, NSIE );

	if ( MDM_BITSET( MODEM_0C, PCOFF ) )
	{
		HDLCRxWriteStatus( HDLC_PC_TO_CC_TRANSITION );
		HDLCRxWriteNextFrame();
		ModemDSPWrite1( MODEM_NEWS_MASK_MEM_ACCESS, MODEM_MEM_ACCESS_DISABLE, TRUE );
		ModemDSPWrite1( MODEM_NEWS_MASK_0C, 0, FALSE );
	}
}

/*===========================================================================
*
*    ModemTxT4NewStatusIsr
*
*    Handles Tx Data status interrupts
*
*   Input:      (none)
*   Output:     NSIE = 0 - disables new status interrupts
*
*===========================================================================*/
void ModemTxHDLCNewStatusIsr( void )
{
	//$GGF: Beware!  Saw some evidence that we are getting flurry of interrupts
	// that don't get cleared.  I.e. an interrupt with FLAGS bit cleared?

	if ( MDM_BITSET( MODEM_0A, FLAGS ) )
	{
		/* Turn off new status and memory access interrupt */
		ModemDSPWrite1( MODEM_NEWS_MASK_MEM_ACCESS, MODEM_MEM_ACCESS_DISABLE, TRUE );
		ModemDSPWrite1( MODEM_NEWS_MASK_0A, 0, FALSE );
		MDM_CLEARBIT( MODEM_1F, NSIE );

		if ( ModemControl->HSFrameEndNotify )
		{
			/* Make the end of frame notification */
			//brd_que_func (ModemControl->HSFrameEndNotify);
			//brd_irq_gen ();
			ModemControl->HSFrameEndNotify();
		}
	}
}


/*===========================================================================
*
*    ModemRxFlagsIsr
*
*   Makes the flags done notify if the HDLC flags stop
*   and disables new status programmable interrupts.
*===========================================================================*/
void ModemRxFlagsIsr( void )
{
	/* Disable new status and DSP memory access (FLAGS) interrupts */
	MDM_CLEARBIT( MODEM_1F, NSIE );
	ModemDSPWrite1( MODEM_NEWS_MASK_MEM_ACCESS, MODEM_MEM_ACCESS_DISABLE, TRUE );
	ModemDSPWrite1( MODEM_NEWS_MASK_0A, 0, FALSE );
}


/*===========================================================================
*
*    ModemRxHDLCIsr
*
*   Receives HDLC frame data
*
*   Input:      HDLC frame bytes in modem RXFIFO
*   Output:     RXFIFO contents written to HDLC buffer
*
*===========================================================================*/
void ModemRxHDLCIsr( void )
{
#ifdef SPI_MULTI_DATA
    UINT8 spi_data[ SPI_MULTI_RX_SIZE ] = {0, 0, 0, 0, 0, 0};
    UINT8 spi_cnt = 0;
    UINT8 burst_cnt = 0;
#endif

    UINT8 status = 0;
    UINT8 rx_data = 0;
	static UINT8 saved_data[ 4 ];
	int overrun_cnt = 0;

#ifdef SPI_MULTI_DATA

	while ( 1 )
	{
		BOOL no_data = FALSE;
		BOOL fOverrunError = FALSE;

		if ( T30Control->State == STATE_RX_PHASE_C_ECM )
		{
			burst_cnt = SPI_MULTI_RX_SIZE;
			spi_cnt = 0;
			mdm_spi_multi_read( 0x81, spi_data, burst_cnt );
		}
		else
		{
			burst_cnt = 2;
			spi_cnt = 0;
			mdm_spi_multi_read( 0x80, spi_data, burst_cnt );
		}

		for ( spi_cnt = 0; spi_cnt < burst_cnt; spi_cnt += 2 )
		{
			if ( spi_data[ spi_cnt ] & 0x80 )
			{
				status  = spi_data[ spi_cnt ] & HDLC_STAT_MASK;
				rx_data = spi_data[ spi_cnt + 1 ];

#else

	while ( MDM_BITSET( MODEM_0C, RXFNE ) )
	{
		status  = MDM_READ(MODEM_0A) & HDLC_STAT_MASK;
		rx_data = MDM_READ(MODEM_RBUFF);

#endif

		switch ( status )
		{
			case HDLC_GOOD_DATA:

				if ( ModemControl->IsrDataCounter == 0 )
				{
					if ( rx_data == 0xFF )
					{
						saved_data[ 0 ] = rx_data;
						HDLCRxWriteDataByte( rx_data );
						ModemControl->IsrDataCounter++;
					}
				}
				else if ( ModemControl->IsrDataCounter < ( ModemControl->BytesPerFrame + 6 ) )
				{
					if ( ModemControl->IsrDataCounter == 1 )
					{
						saved_data[ 1 ] = rx_data;
					}
					else if ( ModemControl->IsrDataCounter == 2 )
					{
						saved_data[ 2 ] = rx_data;
					}
					else if ( ModemControl->IsrDataCounter == 3 )
					{
						saved_data[ 3 ] = rx_data;
					}
					HDLCRxWriteDataByte( rx_data );
					ModemControl->IsrDataCounter++;
				}
				else
				{
					HDLCRxWriteInitFrame();
					ModemControl->IsrDataCounter = 0;
#ifdef DBG_IRQ_PRINTF
					TRACE_IRQ( "Over size frame\n" );
#endif
				}
				break;

			case HDLC_SYNCD:

				if ( ModemControl->IsrDataCounter == 0 )
				{
#ifdef DBG_IRQ_PRINTF
					TRACE_IRQ( "HDLC flag %d\n", ModemControl->IsrDataCounter );
#endif
					break;
				}
				else if ( ModemControl->IsrDataCounter > 4 )
				{
					if ( saved_data[ 1 ] & 0x10 )
						HDLCRxWriteFinalFrame();
					HDLCRxWriteStatus( HDLC_NO_ERROR );
					HDLCRxWriteNextFrame();
#if 0 // Eliminate unnecessary debug logs...
#ifdef DBG_IRQ_PRINTF
					if ( saved_data[ 2 ] == 0x06 )
					{
						TRACE_IRQDEBUG( " FCF[%02X] frame%d %dbyte\n", saved_data[ 2 ], saved_data[ 3 ], ModemControl->IsrDataCounter );
					}
					else if ( saved_data[ 2 ] == 0x86 )
					{
						TRACE_IRQDEBUG( " FCF[%02X] %dbyte\n", saved_data[ 2 ], ModemControl->IsrDataCounter );
					}
					else
					{
						TRACE_IRQ( " FCF[%02X] %dbyte\n", saved_data[ 2 ], ModemControl->IsrDataCounter );
					}
#endif
#endif // #if 0
				}
				else
				{
					HDLCRxWriteInitFrame();
#ifdef DBG_IRQ_PRINTF
					TRACE_IRQ( "Short frame %d\n", ModemControl->IsrDataCounter );
#endif
				}

				if ( ( T30Control->State == STATE_RX_PHASE_B )
				  || ( T30Control->State == STATE_RX_PHASE_B_SIGNAL_REC )
				  || ( T30Control->State == STATE_TX_PHASE_B ) )
				{
#ifdef DBG_IRQ_PRINTF
					TRACE_IRQ( "  %02X\n", rx_data );
#endif
					ModemDisableRxInt();
				}

				saved_data[ 0 ] = 0;
				saved_data[ 1 ] = 0;
				saved_data[ 2 ] = 0;
				saved_data[ 3 ] = 0;
				ModemControl->IsrDataCounter = 0;
				ModemControl->FoundFlag = TRUE;
				ModemControl->SendCRP = FALSE;
				break;

			case HDLC_PE:
			case HDLC_PE_SYNCD:

				if ( ModemControl->IsrDataCounter == 0 )
				{
#ifdef DBG_IRQ_PRINTF
					TRACE_IRQ( "HDLC flag CRC %02X\n", status );
#endif
					break;
				}
				else if ( ModemControl->IsrDataCounter > 4 )
				{
					HDLCRxWriteStatus( HDLC_CHECKSUM_ERROR );
					HDLCRxWriteNextFrame();
#ifdef DBG_IRQ_PRINTF
					if ( ( saved_data[ 0 ] == 0xFF )
					  && ( ( saved_data[ 1 ] & ~0x10 ) == 0x03 ) )
					{
						TRACE_IRQ( "CRC Error %d byte\n", ModemControl->IsrDataCounter );
						TRACE_IRQ( "[" );
						{
							UINT16 i;
							for ( i = 0; i < 4; i++ )
							{
								TRACE_IRQ( " %02X", saved_data[ i ] );
							}
						}
						TRACE_IRQ( " ... ]\n" );
					}
#endif
					if ( T30Control->State == STATE_RX_PHASE_D )
					{
						if ( ( saved_data[ 0 ] == 0xFF )
						  && ( saved_data[ 1 ] == 0x13 ) )
						{
#ifdef DBG_IRQ_PRINTF
							TRACE_IRQ( " Request CRP for %02X\n", saved_data[ 2 ] );
#endif
							ModemControl->SendCRP = TRUE;
						}
					}
				}
				else
				{
					ModemControl->SendCRP = FALSE;
					HDLCRxWriteInitFrame();
#ifdef DBG_IRQ_PRINTF
					TRACE_IRQ( "Short CRC Error\n" );
#endif
				}

				if ( ( T30Control->State == STATE_RX_PHASE_B )
				  || ( T30Control->State == STATE_RX_PHASE_B_SIGNAL_REC )
				  || ( T30Control->State == STATE_TX_PHASE_B ) )
				{
#ifdef DBG_IRQ_PRINTF
					TRACE_IRQDEBUG( "  %02X\n", rx_data );
#endif
					ModemDisableRxInt();
				}

				saved_data[ 0 ] = 0;
				saved_data[ 1 ] = 0;
				saved_data[ 2 ] = 0;
				saved_data[ 3 ] = 0;
				ModemControl->IsrDataCounter = 0;
				ModemControl->FoundFlag = FALSE;
				break;

			case HDLC_OE:
			case HDLC_OE_SYNCD:
				overrun_cnt = 0;
#ifdef SPI_MULTI_DATA
				fOverrunError = TRUE;
#endif
				while ( MDM_BITSET( MODEM_0C, RXFNE ) ) {
					MDM_READ( MODEM_RBUFF );
					overrun_cnt++;
				}

				MDM_CLEARBIT( MODEM_0A, OE );
				HDLCRxWriteInitFrame();

				saved_data[ 0 ] = 0;
				saved_data[ 1 ] = 0;
				saved_data[ 2 ] = 0;
				saved_data[ 3 ] = 0;
				ModemControl->IsrDataCounter = 0;
				ModemControl->FoundFlag = FALSE;
//#ifdef DBG_IRQ_PRINTF
				TRACE_IRQ( "Overrun Error %d,%02d,%02X,%02X\n", overrun_cnt, spi_cnt, spi_data[ spi_cnt ], spi_data[ spi_cnt + 1 ] );
//#endif
				break;

			case HDLC_FE:

				if ( ModemControl->IsrDataCounter > 4 )
				{
					HDLCRxWriteStatus( HDLC_FRAME_ERROR );
					HDLCRxWriteNextFrame();
#ifdef DBG_IRQ_PRINTF
					TRACE_IRQ( "ABORT frame %d byte\n", ModemControl->IsrDataCounter );
					TRACE_IRQ( "[" );
					{
						UINT16 i;
						for ( i = 0; i < 4; i++ )
						{
							TRACE_IRQ( " %02X", saved_data[ i ] );
						}
					}
					TRACE_IRQ( " ... ]\n" );
#endif
				}
				else
					HDLCRxWriteInitFrame();

				saved_data[ 0 ] = 0;
				saved_data[ 1 ] = 0;
				saved_data[ 2 ] = 0;
				saved_data[ 3 ] = 0;
				ModemControl->IsrDataCounter = 0;
				ModemControl->FoundFlag = FALSE;
				break;

			default:

#ifdef DBG_IRQ_PRINTF
				TRACE_IRQDEBUG( "RxIsr unknown %02X\n", status );
#endif
				break;

		} // end of switch ()

		if ( ModemControl->FoundFlag
		  && ( ModemControl->EQM == 0xffff ) )
		{
			ModemControl->FoundFlag = FALSE;
			ModemControl->EQM   = ModemReadEQM();
			ModemControl->Level = ModemReadRxLevel();
		}

#ifdef SPI_MULTI_DATA
				if( fOverrunError == TRUE ) {
					fOverrunError = FALSE;
					break;
				}
			}
			else
			{
				no_data = TRUE;
			}

			if ( burst_cnt == 2 )
			{
				no_data = TRUE;
				break;
			}
		} // end of for ()

		if ( no_data )
			break;
#endif
	}
}


/*===========================================================================
*
*    ModemTxHDLCIsr
*
*   Transmits HDLC frame data (FIFO Enabled).  When the last byte has been
*   sent it clears the TEOF flag.
*
*   Input:      HDLC buffer contains data to transmit and byte count
*   Output:     TEOF = 0 after the last byte has been read by the modem
*
*===========================================================================*/
extern void ECMEndFrame( void );

void ModemTxHDLCIsr( void )
{
#ifdef SPI_MULTI_DATA
	UINT8 spi_data[ SPI_MULTI_TX_SIZE ];
	UINT8 spi_cnt;
#endif

	if ( MDM_READ( MODEM_ABCODE ) == 0x61 )   // Abort?
	{
		MDM_WRITE( MODEM_ABCODE, 0 );
		HDLCTxWriteInitResendFrame();

#ifdef DBG_IRQ_PRINTF
		TRACE_IRQ( " ABORT frame by underrun\n" );
#endif
	}

	if ( HDLCTxReadLength() > 1 )  /* More data left in the HDLC buffer */
	{

#ifdef SPI_MULTI_DATA

		while ( !ModemControl->Status && MDM_BITCLEAR( MODEM_01, TXHF ) )
		{
			if ( HDLCTxReadLength() >= SPI_MULTI_TX_SIZE )
			{
				for ( spi_cnt = 0; spi_cnt < SPI_MULTI_TX_SIZE; spi_cnt++ )
				{
					spi_data[ spi_cnt ] = HDLCTxReadDataByte();

					if ( HDLCTxReadLength() == 1 )  /* last data in the HDLC buffer */
					{
						spi_cnt++;
						break;
					}
				}

				mdm_spi_multi_write( 0x80 | ( MODEM_TBUFF << 1 ), spi_data, spi_cnt );
			}
			else
			{
				while ( MDM_BITSET( MODEM_0D, TXFNF ) && ( HDLCTxReadLength() > 1 ) )
					MDM_WRITE( MODEM_TBUFF, HDLCTxReadDataByte() );
			}

			if ( HDLCTxReadLength() == 1 )  /* last data in the HDLC buffer */
				break;
		}
	}

	if ( ( HDLCTxReadLength() == 1 ) && MDM_BITSET( MODEM_0D, TXFNF ) )

#else

		/* Keep writing data to the modem until the transmit buffer is full */
		/* or we're down to the last byte */
		while ( MDM_BITSET( MODEM_0D, TXFNF ) && ( HDLCTxReadLength() > 1 ) )
			MDM_WRITE( MODEM_TBUFF, HDLCTxReadDataByte() );

	}
	else if (HDLCTxReadLength() == 1)  /* Transmitted all but the last byte of data in the HDLC buffer */

#endif

	{
		/* Turn on the TEOF flag */
		MDM_SETBIT( MODEM_11, TEOF );

		/* Give the last byte to the modem */
		MDM_WRITE( MODEM_TBUFF, HDLCTxReadDataByte() );

		/* Turn off TEOF, and disable interrupts */
		MDM_CLEARBIT( MODEM_11, TEOF );

		ModemDisableTxInt();

		ECMEndFrame();
	}
	else if ( ( ModemControl->Status & MODEM_ABORT ) )
	{
		ModemBadIsr();
	}
}


/*===========================================================================
*
*    ModemRxT4Isr
*
*   Reads T4 coded data from the modem RBUFFER. Disables interrupts and sets
*   the Modem state variable to IDLE when the flag goes false.  Also terminates
*   if abort command received.
*
*   Input:    MSB of ModemControl->Status = 1 if an abort command has been received
*   Output:   ModemControl->State = MODEM_IDLE
*             RDBIE = 0 (receive data interrupts disabled)
*
*===========================================================================*/
void ModemRxT4Isr( void )
{
#ifdef SPI_MULTI_DATA
    UINT8 spi_data[ SPI_MULTI_RX_SIZE ] = {0, 0, 0, 0, 0, 0};
    UINT8 spi_cnt = 0;
    BOOL no_data = FALSE;
#endif

	UINT8 data[ 256 ];
    UINT8 byte_count = 0, i = 0;

	byte_count = 0;

	/* Clear any overrun errors */
	if ( MDM_BITSET( MODEM_0A, OE ) )
	{
//#ifdef DBG_IRQ_PRINTF
		TRACE_IRQ( "Overrun Error\n" );
//#endif
		while ( MDM_BITSET( MODEM_0C, RXFNE ) )
			MDM_READ( MODEM_RBUFF );
		MDM_CLEARBIT( MODEM_0A, OE );
		return ;
	}

#ifdef SPI_MULTI_DATA

	while ( 1 )
	{
		no_data = FALSE;

		if ( byte_count < 253 )
		{
			mdm_spi_multi_read( 0x81, spi_data, SPI_MULTI_RX_SIZE );

			for ( spi_cnt = 0; spi_cnt < SPI_MULTI_RX_SIZE; spi_cnt += 2 )
			{
				if ( spi_data[ spi_cnt ] & 0x80 )
					data[ byte_count++ ] = spi_data[ spi_cnt + 1 ];
				else
					no_data = TRUE;
			}

			if ( ( byte_count > 0 )
			  && ( ModemControl->EQM == 0xffff ) )
			{
				ModemControl->EQM   = ModemReadEQM();
				ModemControl->Level = ModemReadRxLevel();
			}
		}
		else
			break;

		if ( no_data )
			break;
	}

#else

	/* Empty the modem RXFIFO contents */
	while ( MDM_BITSET( MODEM_0C, RXFNE ) )
	{
		if ( byte_count < 255 )
			data[ byte_count++ ] = MDM_READ( MODEM_RBUFF );
		else
			break;
	}

#endif

	/* If abort, disable interrupts and mark end of page in the CDB */
	if ( ModemControl->Status & MODEM_ABORT )
	{
		ModemDisableRxInt();
		ModemControl->State = MODEM_IDLE;
		ModemCDBMarkEndOfPage();
	}
	else if ( MDM_BITCLEAR( MODEM_0F, RLSD ) )
	{
		ModemCDBWriteErrorData();
	}
#ifdef SPI_MULTI_DATA	//add.bong.2012-09-07 12:03pm
	if ( MDM_BITSET( MODEM_0B, EQMAT ) )
		no_data = 0;
	else
		no_data = 1;

	/* Write the RXFIFO contents to the CDB */
	for ( i = 0; i < byte_count; i++ )
	{
		if ( no_data )
			ModemCheckRTC( data[ i ] );

		ModemCDBWriteByte( data[ i ] );
	}
#else //add.bong.
	/* Write the RXFIFO contents to the CDB */
	for ( i = 0; i < byte_count; i++ )
	{
		ModemCDBWriteByte( data[ i ] );
	}
#endif //add.bong.
}


/*===========================================================================
*
*    ModemTxT4Isr
*
*   Writes T4 image data to transmit data (TBUFFER) register.
*   Turns off RTS, disables interrupts, and sets
*   the T.30 mode variable to idle when it runs out of data.
*
*   Input:      CDB contains data to transmit
*   Output:     ModemControl->State = MODEM_IDLE
*               RTS = 0 (request to send off)
*               TDBIE = 0 (transmit data interrupts disabled)
*
*===========================================================================*/
void ModemTxT4Isr( void )
{
    CDB_BUFFER *cdb = (CDB_BUFFER *)NULL;
#ifdef SPI_MULTI_DATA
    UINT8 spi_data[ SPI_MULTI_TX_SIZE ] = {0, };
#endif
	UINT8 temp = 0;
	UINT16 len, data_cnt;

	if ( _CxDebugLevel == ( DEBUG_IRQ | DEBUG_DEBUG ) )
	{
		if ( MDM_BITSET( MODEM_1E, TDBE ) )
		{
			UINT8 in, out;

			in  = ModemDSPRead1( MODEM_TX_FIFO_IN_POINTER );
			out = ModemDSPRead1( MODEM_TX_FIFO_OUT_POINTER );
			if ( in == out )
				TRACE_IRQ( "Mu\n" );
		}
	}

	cdb = &cdb_buffer[ModemControl->TxBlockCnt][ModemControl->TxFrameCnt];    // get a cdb pointer from cdb_buffer

    if (cdb == (CDB_BUFFER *)NULL)
    {
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_TRANSMIT_T4_IMAGE_DATA);
    }

	if ( !cdb->size )
	{
        //TRACE_IRQ( "Bu\n" );
        TRACE_IRQ( "Bu - ModemControl->TxBlockCnt: %d, ModemControl->TxFrameCnt: %d\n", ModemControl->TxBlockCnt, ModemControl->TxFrameCnt );
        OSC_Sleep(1);

		return;
	}

	/* If aborted, disable interrupts, turn off RTS, and return the CDB */
	data_cnt = 0;

	if ( ModemControl->Status & MODEM_ABORT )
	{
		ModemDisableTxInt();
		MDM_CLEARBIT( MODEM_08, RTS );
		ModemControl->State = MODEM_IDLE;
	}
	else
	{

#ifdef SPI_MULTI_DATA

		while ( !ModemControl->Status && MDM_BITCLEAR( MODEM_01, TXHF ) )
		{
			len = ModemCDBReadLength();

			if ( ( len == 0 ) || ( len == 0xFFFF ) )
			{
				break;
			}

			if ( len >= SPI_MULTI_TX_SIZE )
			{
				for ( data_cnt = 0; data_cnt < SPI_MULTI_TX_SIZE; data_cnt++ )
					spi_data[ data_cnt ] = ModemCDBReadByte();

				mdm_spi_multi_write( 0x80 | ( MODEM_TBUFF << 1 ), spi_data, data_cnt );
			}
			else
			{
				data_cnt++;
				temp = ModemCDBReadByte();
				MDM_WRITE( MODEM_TBUFF, temp );
			}

#ifdef DBG_IRQ_PRINTF
			TRACE_IRQDEBUG("%d, ", data_cnt);
#endif
		}

#else

		while ( !ModemControl->Status && MDM_BITSET( MODEM_0D, TXFNF ) )
		{
			data_cnt++;
			temp = ModemCDBReadByte();
			MDM_WRITE( MODEM_TBUFF, temp );
		}

#endif

#if 0 // Eliminate unnecessary debug logs...
#ifdef DBG_IRQ_PRINTF
		if ( data_cnt > 0 )
		{
			len = ModemCDBReadLength();

			if ( ( len == 0 ) || ( len == 0xFFFF ) )
			{
				TRACE_IRQDEBUG("%02X:%d, ", temp, data_cnt);
			}
#ifndef SPI_MULTI_DATA
			else
			{
				TRACE_IRQDEBUG("%d, ", data_cnt);
			}
#endif
		}
#endif
#endif // #if 0
	}
}


/*==========================================================================
*
* ModemTxTCFIsr
*
* Load 0x00 on TxBuffer     THY
*===========================================================================*/
void ModemTxTCFIsr( void )
{
#ifdef SPI_MULTI_DATA

	UINT8 spi_data[ SPI_MULTI_TX_SIZE ] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	while ( !ModemControl->Status && MDM_BITCLEAR( MODEM_01, TXHF ) )
	{
		mdm_spi_multi_write( 0x80 | ( MODEM_TBUFF << 1 ), spi_data, SPI_MULTI_TX_SIZE );
	}

#else

	while ( MDM_BITSET( MODEM_0D, TXFNF ) )
		MDM_WRITE( MODEM_TBUFF, 0x00 );

#endif
}


/*==========================================================================
*
* ModemRxTCFIsr
*
* Receive TCF data and check error
*
* Output: ModemControl->TCFErrorBytes: number of error bytes on TCF
*
*===========================================================================*/
void ModemRxTCFIsr( void )
{
#ifdef SPI_MULTI_DATA

    UINT8 spi_data[ SPI_MULTI_RX_SIZE ] = {0, 0, 0, 0, 0, 0};
    UINT8 spi_cnt = 0;
    BOOL no_data = FALSE;

	while ( 1 )
	{
		no_data = FALSE;

		mdm_spi_multi_read( 0x81, spi_data, SPI_MULTI_RX_SIZE );

		for ( spi_cnt = 0; spi_cnt < SPI_MULTI_RX_SIZE; spi_cnt += 2 )
		{
			if ( spi_data[ spi_cnt ] & 0x80 )
			{
				if ( spi_data[ spi_cnt + 1 ] == 0 )
					ModemControl->TCFOKBytes++;
				else
					ModemControl->TCFErrorBytes++;
			}
			else
				no_data = TRUE;
		}

		if ( no_data )
			break;
	}

#else

    UINT8 data = 0;

	while ( MDM_BITSET( MODEM_0C, RXFNE ) )
	{
		data = MDM_READ( MODEM_RBUFF );

		if ( data == 0 )
        {
			ModemControl->TCFOKBytes++;
        }
		else
        {
			ModemControl->TCFErrorBytes++;
        }
	}

#endif

}


/*===========================================================================
*
*    ModemRxCidIsr
*
*   receive V.23 Caller Id. data bytes.
*
*===========================================================================*/
static void ModemRxCidIsr( void )
{
#ifdef SPI_MULTI_DATA
    UINT8 spi_data[ SPI_MULTI_RX_SIZE ] = {0, 0, 0, 0, 0, 0};
    UINT8 spi_cnt = 0;
    BOOL no_data = 0;
#endif

    UINT8 data = 0;

	if ( MDM_BITSET( MODEM_0F, RI ) )
	{
		return ;
	}

	/* Clear any overrun errors */
	if ( MDM_BITSET( MODEM_0A, OE ) )
	{
//#ifdef DBG_IRQ_PRINTF
		TRACE_IRQ( "Overrun Error\n" );
//#endif
		while ( MDM_BITSET( MODEM_0C, RXFNE ) )
			MDM_READ( MODEM_RBUFF );

		MDM_CLEARBIT( MODEM_0A, OE );
		return ;
	}

#ifdef SPI_MULTI_DATA

	while ( 1 )
	{
		no_data = FALSE;

		mdm_spi_multi_read( 0x81, spi_data, SPI_MULTI_RX_SIZE );

		for ( spi_cnt = 0; spi_cnt < SPI_MULTI_RX_SIZE; spi_cnt += 2 )
		{
			if ( spi_data[ spi_cnt ] & 0x80 )
			{
				data = spi_data[ spi_cnt + 1 ];

#ifdef DBG_IRQ_PRINTF
				TRACE_IRQDEBUG( "%02X ", data );
#endif
				if ( CIDRxData( data ) )
					return;
			}
			else
				no_data = TRUE;
		}

		if ( no_data )
			break;
	}

#else

	/* Empty the modem RXFIFO contents */
	while ( MDM_BITSET( MODEM_0C, RXFNE ) )
	{
		data = MDM_READ( MODEM_RBUFF );

#ifdef DBG_IRQ_PRINTF
		TRACE_IRQDEBUG( "%02X ", data );
#endif

		if ( CIDRxData( data ) )
			return;
	}

#endif

	/* If abort, disable interrupts, */
	/* not receiving data any more and */
	/* indicate loss of carrier. */
	if ( ModemControl->Status & MODEM_ABORT )
	{
		ModemRxCIDShutdown();
	}
}


/*===========================================================================
*    ModemV8StatusIsr
*
*   This interrupt monitors the V8/V34 handshake status bits and
*   readies the task when T30 handshaking can start
*===========================================================================*/
UINT8 ModemV8StatusIsr( void )
{
	UINT8 tx_state, rx_state, conf, org, ret;
	//UINT16 wPrecoder;
	BOOL done = FALSE;
	//static BOOL bV34Precoder;
	static UINT8 _tx_state, _rx_state;

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	/* Clear interrupt and obtain current status values */
	MDM_CLEARBIT( MODEM_1F, NEWS );
	tx_state = MDM_READ( MODEM_TXHSKSTAT );
	rx_state = MDM_READ( MODEM_RXHSKSTAT );
	conf = MDM_READ( MODEM_CONF );
	org  = MDM_BITSET( MODEM_09, ORG );

	if ( ( _tx_state != tx_state ) || ( _rx_state != rx_state ) )
	{
#ifdef DBG_IRQ_PRINTF
		TRACE_IRQ( " (%02X %02X %02X)\n", tx_state, rx_state, conf );
#endif
//		if ( ( tx_state > 0x23 ) && ( rx_state > 0x23 ) )
		{
			MODEM_TIMER_START( TimerEvent, 7000 );
		}
		ret = rx_state;
	}
	else
	{
		ret = 0;
	}

	/* Should care about transmitter state only if originator
	   or answerer allowing initial polling */
	if ( !ModemControl->V8Answer || ModemControl->V8Polling )
	{
		/* Originate cases */
		if ( org )
		{
			if ( ( ( tx_state == CC_DATA_MODE ) && ( rx_state == CC_DATA_MODE ) )
			  || ( ( tx_state == 0x05 ) && ( conf == MODEM_V21_CH2_CONF ) ) )
				done = TRUE;
		}
		else
		{
			switch ( tx_state )
			{
				case 0x02:

					if ( conf == MODEM_V21_CH2_CONF )
					{
						MDM_CLEARBIT( MODEM_09, DTR );
						done = TRUE;
					}
					break;

				case CC_DATA_MODE:

					done = TRUE;
					break;

				default:
					break;
			}
		}

#ifdef OEM_FAX_REPORT
		if ( _tx_state != tx_state )
		{
			ModemControl->Level = ModemReadTxLevel();
			ModemControl->EQM   = 0xffff;

			switch ( tx_state )
			{
				case 0x04:

                    CxProtocolReport( V8CJ, NULL, 0, TX_LOG );
					break;

				case 0x20:

                    CxProtocolReport( V34P2, NULL, 0, TX_LOG );
					break;

				case 0x44:

                    CxProtocolReport( V34P3, NULL, 0, TX_LOG );
					break;

				case 0x81:

                    CxProtocolReport( V34C1, NULL, 0, TX_LOG );
					break;

				case 0x84:

                    CxProtocolReport( V34CC, NULL, 0, TX_LOG );
                    break;

				default:
					break;
			}
		}
#endif
	}

	/* Should care about receiver state only if answerer
	   or orginator wanting to poll */
	if ( ModemControl->V8Answer || ModemControl->V8Polling )
	{
#if 0  //test
		if ( rx_state < 0x47 )
		{
			bV34Precoder = TRUE;
		}
		else if ( bV34Precoder && ( rx_state == 0x47 ) )
		{
			/* Precoder workaround for Agere modem */

			wPrecoder = ModemDSPRead4( MODEM_PHASE3_PRECORER );

			TRACE_MODEM( "Precoder 0x%04X\n", wPrecoder );

			if ( wPrecoder >= 0x0200 )
			{
				ModemDSPWrite3( MODEM_PHASE3_PRECORER, 0x0100 );
				bV34Precoder = FALSE;

				TRACE_MODEM( "  set to 0x0100\n" );
			}
		}
#endif
		if ( ( tx_state == CC_DATA_MODE ) && ( rx_state == CC_DATA_MODE ) )
		{
			done = TRUE;
		}
		else if ( ( rx_state == 0x03 ) && ( conf == MODEM_V21_CH2_CONF ) )
		{
			done = TRUE;
		}
		else if ( ( _tx_state == 0x40 ) && ( tx_state == 0x20 ) )
		{
			ModemControl->Status = TIME_OUT;
			done = TRUE;
		}


#ifdef OEM_FAX_REPORT
		if ( _rx_state != rx_state )
		{
			ModemControl->Level = -55;
			ModemControl->EQM   = 0xffff;

			switch ( rx_state )
			{
				case 0x03:

					ModemControl->Level = ModemReadRxLevel();
                    CxProtocolReport( V8CJ, NULL, 0, RX_LOG );
					break;

				case 0x26:

					ModemControl->Level = ModemReadRxLevel();
                    CxProtocolReport( V34P2, NULL, 0, RX_LOG );
					break;

				case 0x41:

					ModemControl->Level = ModemReadRxLevel();
                    CxProtocolReport( V34P3, NULL, 0, RX_LOG );
					break;

				case 0x81:

					ModemControl->Level = ModemReadRxLevel();
                    CxProtocolReport( V34C1, NULL, 0, RX_LOG );
					break;

				case 0x84:

					ModemControl->Level = ModemReadRxLevel();
					ModemControl->EQM   = ModemReadEQM();
                    CxProtocolReport( V34CC, NULL, 0, RX_LOG );
					break;

				default:
					break;
			}
		}
#endif
	}

	/* If done, disable future interrupts and notify the task */
	if ( done )
	{
#ifdef DBG_IRQ_PRINTF
		TRACE_IRQ( "ISR done (%02X %02X %02X)\n", tx_state, rx_state, conf );
#endif

		/* Disable IRQ and reset modem state */
		ModemControl->State = MODEM_IDLE;
		MDM_CLEARBIT( MODEM_1F, NSIE );
		ModemDSPWrite1( MODEM_NEWS_MASK_16, 0, FALSE );
		ModemDSPWrite1( MODEM_NEWS_MASK_17, 0, FALSE );
		ModemDSPWrite1( MODEM_NEWS_MASK_MEM_ACCESS, MODEM_MEM_ACCESS_DISABLE, TRUE );
	}

	_tx_state = tx_state;
	_rx_state = rx_state;

	OSC_Release_Mutex( &modem_dsp_mutex );

	return ret;
}


/*===========================================================================
*
*    ModemRxVoiceIsr
*
*   Reads data from the modem parallel data register and writes it to
*   voice memory.  Checks for out of memory error and abort command.
*
*   Input:      VceMsgHandle = voice messag queue index
*               MSB of ModemControl->Status = 1 to abort
*
*   Output:     Message byte count incremented
*               When abort command received:
*               RDBIE = 0 (parallel data interrupts disabled)
*               ModemControl->Status = 1 if out of memory
*               ModemControl->State = MODEM_IDLE when done
*
*
*===========================================================================*/
void ModemRxVoiceIsr( void )
{}

/*===========================================================================
*
*    ModemTxVoiceIsr
*
*   Reads data from voice memory and writes it to the modem parallel data
*   register.  Checks for abort command.
*
*   Input:  MSB of ModemControl->Status = 1 to abort
*
*   Output:  When abort command received or end of message:
*             ModemDCDEN = 0 (voice decoder disabled)
*             ModemIE2 = 0 (parallel data interrupts disabled)
*
*
*           ModemControl->State = MODEM_IDLE when done
*
*===========================================================================*/
void ModemTxVoiceIsr( void )
{}

