/*
 *  $RCSfile: t30_rx.c,v $
 * $Revision: 1.54 $
 *   $Author: miurat $
 *     $Date: 2012/02/29 02:36:57 $
 */
/************************************************************************
 *
 *      t30_rx.c
 *
 *  T.30 Receive State Machine Functions
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
#include "MACROS.H"
#include "MODEMX.H"
#include "T30X.H"
#include "HDLC.H"
#include "ECMX.H"
#include "ECM.H"
#include "t30.h"
#include "fsu.h"
#include "../../faxapp/include/faxapp_notify.h"

#define RNR_SENT_CNT     15

// global variable

extern void CallModemReset( void );
extern void CIDShutdown( void );
extern void ECMCheckV34FallBack( void );
extern void ModemRxForceEndOfPage( UINT8 );
extern void ECMSendBlockOnError( void );
extern void fsu_rx_fax_cancel(UINT8 abort_condition);
extern void prepare_rx_buffers(UINT16 buffers, BOOL ecm_mode);
extern void fsu_delete_dir_on_file_system (const char *Path);
extern UINT8 poll_tx_job_ready (void);
#if __TX_REPORT_IMAGE_PRINT__
extern void tx_report_image_setting(UINT16 jnum, int type);
#endif

extern UINT8 ModemRxSwitchPCtoCC( void );
extern UINT8 fsu_get_total_page_no_in_mem (UINT16 DocId, UINT8 FileType);
extern UINT8 fsu_get_photo_mode(UINT16 jnum);
extern UINT8 start_fsu_rcv_to_mem(void);
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern boolean fsu_rcv_next_page(void);
extern void fsu_update_status (UINT16 jnum, UINT8 status);

extern BOOLEAN T4_end_of_page;
extern UINT16 fsu_job_num_send;
extern UINT16 fsu_job_num_rcv;
extern UINT32 last_EOL_time;
extern UINT32 gFaxT1Timeout;
extern UINT32 gFaxCEDDetectionTime;
extern UINT8 too_many_error;
extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
UINT8 RNRSentCount = 0;


/*===========================================================================
*
*    T30RxCheckCopyQuality
*
*      Checks the copy quality of the received page.  Use 11% as the
*      MCF-RTN threshold, because it's easy to check and satisfies the French
*      PTT requirements.
*
*      Total = Good + Bad
*      If Bad = Good/8, errors = (Good/8) / (Good + Good/8) = 11%
*
*      French PTT Requirements
*      =======================
*
*      T4 errors     Accepted response
*      ---------     -----------------
*        0             MCF
*        0-5%          MCF or RTP
*        5-15%         MCF or RTP or RTN
*        >15%          RTN
*
*      Inputs:         T30Control->LostCarrFlg = TRUE if carrier was lost during the page
*      Output:         return value = MCF if good, RTN if not
*                      T30Control->LostCarrFlg = 0
*
*===========================================================================*/
UINT8 T30RxCheckCopyQuality( void )
{
    UINT8 ret = 0;

    if ( !ModemControl->FoundRTC || too_many_error)
    {
        TRACE_FAX( "  send ( RTN )\n" );
        T30Control->LostCarrFlg = 0;
        ret = RTN;
    }
    else
    {
		set_fsu_state( FSU_STATE_RX_PHASE_D );
		TRACE_FAX( "  send ( MCF )\n" );
		ret = MCF;
    }

	ModemControl->FoundRTC = FALSE;
	ModemControl->EOLCount = 0;
	ModemControl->ZeroCount = 0;
	return ret;
}


/*===========================================================================
*
*    T30RxCheckErrors
*
*      Checks for printer, memory and MMR errors.
*
*      Input:          T30Control->RxToMemFlg = 1 if we're already RX'ing to memory
*                      t30_attribute.FaxOptions.IgnoreFileSystem = 1 if we're ignoring page memory
*                      T30Control->State = current state of the T.30
*                      PrtStatus = printer status
*      Output:         return value = 0 if no faults.  return value = 1 if not.
*                      T30Control->State = STATE_RX_PHASE_E if we have to quit
*
*===========================================================================*/
UINT8 T30RxCheckErrors( void )
{
	if ( T30Control->Status || ( ModemControl->Status & MODEM_ABORT ) )
		return 1;

	/* No printer related errors */
	return ( 0 );
}


/*===========================================================================
*
*    T30RxMakeDIS
*
*      Makes up the DIS/NSF frames
*
*      Input:          t30_attribute.FaxOptions contains various fax options
*      Output:         T30Control->LocalCap and LocalNSTD initialized
*
*===========================================================================*/
void T30RxMakeDIS( void )
{
	TRACE_FAX( "Entering T30RxMakeDIS().....\n" );

	/* Init local capabilities with defaults */
	memcpy( T30Control->LocalCap, &LocalDIS, LocalDISSize );

	if ( t30_attribute.ECMMode != ECM_NONE )
	{
		if ( t30_attribute.FaxRxStartSpeed > T30_SPEED_V17_14400 )
		{
			TRACE_FAX( "Set V8 capability....\n" );

			SETBIT( &T30Control->LocalCap[ 0 ], 0x20 );  /* set V8 capability */
		}
	}

	/* Set the starting speed */
	if ( t30_attribute.FaxRxStartSpeed > T30_SPEED_V17_14400 )
    {
		T30Control->LocalCap[ 1 ] |= DIS_SpeedBits[ T30_SPEED_V17_14400 ];
    }
	else
    {
		T30Control->LocalCap[ 1 ] |= DIS_SpeedBits[ t30_attribute.FaxRxStartSpeed ];
    }

	/* Check for TX-by-polling document */
    if ( ( ( t30_attribute.PollTxSource == TXPOLLFROM_MEMORY ) && poll_tx_job_ready() ) || ( ( t30_attribute.PollTxSource == TXPOLLFROM_SCAN ) && T30Control->ScnDocPresent ) )
	{
		SETBIT( &T30Control->LocalCap[ 1 ], 0x01 );

        if ( ( t30_attribute.PollTxRequireSubaddress ) && ( t30_attribute.PollLocalTxSubaddress[ 0 ] != 0 ) )
		{
			SETBIT( &T30Control->LocalCap[ 6 ], 0x01 );
		}

        if ( ( t30_attribute.PollTxRequirePassword ) && ( t30_attribute.PollLocalTxPassword[ 0 ] != 0 ) )
		{
			SETBIT( &T30Control->LocalCap[ 6 ], 0x02 );
		}
	}

	/* Set paper capabilities (A4, B4 or A3 paper) */
	switch ( t30_attribute.FaxRxImageWidth )
	{
		case WIDTH_A3:
			SETBIT( &T30Control->LocalCap[ 2 ], 0x0A );
			break;

		case WIDTH_B4:
			SETBIT( &T30Control->LocalCap[ 2 ], 0x05 );
			break;

		case WIDTH_A4:
		default:
			CLEARBIT( &T30Control->LocalCap[ 2 ], 0x03 );
			break;
	}

	/* Set length capabilities (unlimited) */
	if ( t30_attribute.FaxUnlimitLength )
	{
		SETBIT( &T30Control->LocalCap[ 2 ], 0x08 );
	}

	/* Set resolution capability */
	switch ( t30_attribute.FaxMinimumScanTime )
	{
		case 0:
			SETBIT( &T30Control->LocalCap[ 2 ], 0x70 );
			break;

		case 5:
			SETBIT( &T30Control->LocalCap[ 2 ], 0x10 );
			break;

		case 10:
			SETBIT( &T30Control->LocalCap[ 2 ], 0x20 );
			break;

		case 40:
			SETBIT( &T30Control->LocalCap[ 2 ], 0x40 );
			break;

		case 20:
		default:
			CLEARBIT( &T30Control->LocalCap[ 2 ], 0x70 );
			break;
	}


	/* Set resolution capability */
	switch ( t30_attribute.FaxRxResolution )
	{
		case RES_ULTRAFINE:
#ifdef RES_SUPERFINE
			SETBIT( &T30Control->LocalCap[ 5 ], 0x07 );
#else
			SETBIT( &T30Control->LocalCap[ 5 ], 0x06 );
#endif
			SETBIT( &T30Control->LocalCap[ 1 ], 0x40 );
			break;

		case RES_300:
#ifdef RES_SUPERFINE
			SETBIT( &T30Control->LocalCap[ 5 ], 0x03 );
#else
			SETBIT( &T30Control->LocalCap[ 5 ], 0x02 );
#endif
			SETBIT( &T30Control->LocalCap[ 1 ], 0x40 );
			break;

#ifdef RES_SUPERFINE
		case RES_SUPERFINE:
			SETBIT( &T30Control->LocalCap[ 5 ], 0x01 );
			SETBIT( &T30Control->LocalCap[ 1 ], 0x40 );
			break;
#endif
		case RES_FINE:
			SETBIT( &T30Control->LocalCap[ 1 ], 0x40 );
			break;

		case RES_STANDARD:
		default:
			CLEARBIT( &T30Control->LocalCap[ 5 ], 0x07 );
			CLEARBIT( &T30Control->LocalCap[ 1 ], 0x40 );
			break;
	}

	/* Set MR coding if not ignoring MR */
	if ( !t30_attribute.FaxOptions.IgnoreMR )
	{
		SETBIT( &T30Control->LocalCap[ 1 ], 0x80 );
	}

	/* Set ECM if not being ignored and memory is available */
	if ( ( t30_attribute.ECMMode != ECM_NONE )
	       /* For mode changes only if ECM was available earlier */
	  && ( !T30Control->PageNum || T30Control->ECMEnabled ) )
	{
		if ( ECMInitJob( t30_attribute.ECMMode ) == E_OK )
		{
			SETBIT( &T30Control->LocalCap[ 3 ], 0x04 );

			if ( t30_attribute.ECMMode == ECM_64 )
			{
				SETBIT( &T30Control->LocalCap[ 0 ], 0x40 );
			}

			/* Set MMR mode */
			if ( !t30_attribute.FaxOptions.IgnoreMMR )
			{
				SETBIT( &T30Control->LocalCap[ 3 ], 0x40 );
			}

			/* Set JPEG mode */
			if ( !t30_attribute.FaxOptions.IgnoreJPEG )
			{
				SETBIT( &T30Control->LocalCap[ 8 ], 0x08 );
			}

			/* Set Full Color mode */
			if ( !t30_attribute.FaxOptions.IgnoreFullColor )
			{
				SETBIT( &T30Control->LocalCap[ 8 ], 0x10 );
			}

			/* Set JBIG mode */
			if ( !t30_attribute.FaxOptions.IgnoreJBIG )
			{
				if ( !t30_attribute.FaxOptions.IgnoreJBIGL0 )
				{
					SETBIT( &T30Control->LocalCap[ 9 ], 0x60 );
				}
				else
					SETBIT( &T30Control->LocalCap[ 9 ], 0x20 );
			}
		}
	}

	/* Set North American paper capabilities if allowed */
	if ( t30_attribute.HandleNAPaperSizes )
		SETBIT( &T30Control->LocalCap[ 9 ], 0x18 );

	if ( ( t30_attribute.RxRequireSubaddress )
	  && ( t30_attribute.FaxLocalRxSubaddress[ 0 ] != 0 ) )
	{
		SETBIT( &T30Control->LocalCap[ 6 ], 0x01 );
	}

	if ( ( t30_attribute.RxRequirePassword )
	  && ( t30_attribute.FaxLocalRxPassword[ 0 ] != 0 ) )
	{
		SETBIT( &T30Control->LocalCap[ 6 ], 0x02 );
	}

	/* Copy DIS into NSF */
	if ( t30_attribute.FaxEnableNSF )
		memcpy( T30Control->LocalNSTDCap, T30Control->LocalCap, LocalDISSize );
}


/*===========================================================================
*
*    T30RxReadDCS
*
*      Reads the options from the received DCS (or CTC)
*
*      Input:          T30Control->RemoteCap = remote DCS FIF (or CTC FIF)
*      Output:         See below
*
*===========================================================================*/
void T30RxReadDCS( void )
{
	UINT8 index;
	UINT8 ecm_mode;

	TRACE_FAX( "T30RxReadDCS()\n" );
	TRACE_FAX( "[" );
	for ( index = 0; index < RemoteCapSize; index++ )
	{
		TRACE_FAX( " %02X", T30Control->RemoteCap[ index ] );
	}
	TRACE_FAX( " ]\n" );

	/* Set the modem speed */
	if ( !T30Control->V34Mode )
	{
		index = ( T30Control->RemoteCap[ 1 ] & 0x3C ) >> 2;
		T30Control->ModemSpeed = SpeedConvertTbl[ index ];
	}

	/* Set standard, fine, or superfine resolution */
	if ( BITSET( &T30Control->RemoteCap[ 5 ], 0x04 ) )
	{
		T30Control->T4Params.Rx.Res = RES_ULTRAFINE; // 400x400 or R16x15.6
	}
	else if ( BITSET( &T30Control->RemoteCap[ 5 ], 0x02 ) )
	{
		T30Control->T4Params.Rx.Res = RES_300;       // 300x300
	}
#ifdef RES_SUPERFINE
	else if ( BITSET( &T30Control->RemoteCap[ 5 ], 0x01 ) )
	{
		T30Control->T4Params.Rx.Res = RES_SUPERFINE; // 200x400 or R8x15.6
	}
#endif
	else if ( BITSET( &T30Control->RemoteCap[ 1 ], 0x40 ) )
	{
		T30Control->T4Params.Rx.Res = RES_FINE;      // 200x200 or R8x7.7
	}
	else
	{
		T30Control->T4Params.Rx.Res = RES_STANDARD;  // 200x100 or R8x3.85
	}

	/* Set MH or MR coding */
	if ( BITCLEAR( &T30Control->RemoteCap[ 1 ], 0x80 ) )
    {
		T30Control->T4Params.Rx.Coding = CODING_MH;
    }
	else
    {
		T30Control->T4Params.Rx.Coding = CODING_MR;
    }

	ModemControl->EOLType = T30Control->T4Params.Rx.Coding;

	/* Set A4, Letter, B4 and A3 width */

    if ( ( T30Control->RemoteCap[ 2 ] & 0x03 ) == 0x00 ) // A4
	{
		/* Check for North American papersizes */
		if ( t30_attribute.HandleNAPaperSizes )
		{
			if ( T30Control->RemoteCap[ 9 ] & 0x08 )
			{
				T30Control->T4Params.Rx.Width = WIDTH_A4;
				T30Control->T4Params.Rx.PageLen = LEN_LETTER;
				T30Control->T4Params.Rx.PaperType = PAPSIZE_LETTER;
			}
			else if ( T30Control->RemoteCap[ 9 ] & 0x10 )
			{
				T30Control->T4Params.Rx.Width = WIDTH_A4;
				T30Control->T4Params.Rx.PageLen = LEN_LEGAL;
				T30Control->T4Params.Rx.PaperType = PAPSIZE_LEGAL;
			}
		}
		else
		{
			T30Control->T4Params.Rx.Width = WIDTH_A4;
			T30Control->T4Params.Rx.PageLen = LEN_A4;
			T30Control->T4Params.Rx.PaperType = PAPSIZE_A4;
		}
	}
    else if ( ( T30Control->RemoteCap[ 2 ] & 0x03 ) == 0x01 ) // B4
	{
		if ( ( t30_attribute.FaxRxImageWidth == WIDTH_A3 )
		  || ( t30_attribute.FaxRxImageWidth == WIDTH_B4 ) )
		{
			T30Control->T4Params.Rx.Width = WIDTH_B4;
			T30Control->T4Params.Rx.PageLen = LEN_B4;
			T30Control->T4Params.Rx.PaperType = PAPSIZE_B4;
			TRACE_FAX(" B4 size\n" );
		}
	}
    else if ( ( T30Control->RemoteCap[ 2 ] & 0x03 ) == 0x02 ) // A3
	{
		if ( t30_attribute.FaxRxImageWidth == WIDTH_A3 )
		{
			T30Control->T4Params.Rx.Width = WIDTH_A3;
			T30Control->T4Params.Rx.PageLen = LEN_UNLTD;
			T30Control->T4Params.Rx.PaperType = PAPSIZE_A3;
			TRACE_FAX(" A3 size\n" );
		}
	}

	/* Make sure the remote is ECM capable */
	if ( BITSET( &T30Control->RemoteCap[ 3 ], 0x04 ) )
	{
		/* Set the frame size (64 or 256 bytes) */

		if ( BITSET( &T30Control->RemoteCap[ 3 ], 0x08 ) )
		{
			ecm_mode = ECM_64;
			ECMControl->BytesPerFrame = 64;
		}
		else
		{
			ecm_mode = ECM_256;
			ECMControl->BytesPerFrame = 256;
		}

		/* Attempt to set up the ECM job. This should have already */
		/* happened in T30RxMakeDIS().  But we might be going down to */
		/* 64 byte frames if that's all the remote wants */
		if ( ECMInitJob( ecm_mode ) == E_OK )
		{
			/* Set the ecm mode */
			T30Control->T4Params.Rx.ECMMode = ecm_mode;

			/* Set MMR/JPEG possibly */

			if ( BITSET( &T30Control->RemoteCap[ 8 ], 0x18 ) )
			{
				TRACE_FAX( " SETTING RX JPEG CODING\n" );
				T30Control->T4Params.Rx.Coding = CODING_JPEG;
			}
			else if ( BITSET( &T30Control->RemoteCap[9], 0x20 ) )
			{
				TRACE_FAX( " SETTING RX JBIG CODING\n" );
				T30Control->T4Params.Rx.Coding = CODING_JBIG;
			}
			else if ( BITSET( &T30Control->RemoteCap[ 3 ], 0x40 ) )
			{
                if ( ( T30Control->T4Params.Rx.Coding != CODING_JPEG ) || ( T30Control->T4Params.Rx.Coding != CODING_JBIG ) )
                {
					T30Control->T4Params.Rx.Coding = CODING_MMR;
                }
			}
        }
	}

	/* Release any memory allocated by ECMInitJob() previously */
	else
	{
		TRACE_FAX(" NonECM 0x%02X\n", T30Control->RemoteCap[ 3 ] );
		ECMShutdownJob();
	}

	TRACE_FAX(" T4Params.Rx.Res %d\n", T30Control->T4Params.Rx.Res);
	TRACE_FAX(" T4Params.Rx.Width %d\n", T30Control->T4Params.Rx.Width);
	TRACE_FAX(" T4Params.Rx.Coding %d\n", T30Control->T4Params.Rx.Coding);
}


/*===========================================================================
*
*    T30RxTrainingCheck
*
*      Receives the training check, and sends CFR or FTT
*
*      Input:          T30Control->RemoteCap = Received DCS FIF
*                      T30Control->ModemSpeed = modem speed table index
*      Output:         (none)
*
*===========================================================================*/
void T30RxTrainingCheck( UINT8 send_fcf )
{
	UINT8 status;

	TRACE_FAX( "Entering T30RxTrainingCheck().....\n" );

	/* Always send CFR in V34 */
	if ( T30Control->V34Mode )
	{
		OSC_Sleep( T30_DELAY_TIME );  // give process slot for fsu
		status = 0;
	}
	else
	{
		/* Receive the TCF at the indicated speed. Status indicates good or bad. */
		status = ModemRxTCF( T30Control->ModemSpeed );
	}

	if ( send_fcf == DCN )
	{
		T30SendFCF( DCN );
		OSC_Sleep( 100 );  // give receive time for remote
		T30Control->State = STATE_RX_DISCONN;
		return ;
	}

	/* For a bad TCF, send FTT.  For a good TCF, send CFR and init the job */
	if ( status )
	{
		TRACE_FAX("T30RxTrainingCheck: Send ( FTT )\n");

		T30SendFCF( FTT );
		T30Control->State = STATE_RX_PHASE_B;
	}
	else
	{
		if ( T30Control->LastFCFSent != CFR )
		{
			// prepapre file and fipp when it is first page
			if(!T30Control->PageNum)
				set_fsu_state(FSU_STATE_MEM_RX_START);
		}
		TRACE_FAX("T30RxTrainingCheck: Send ( CFR )\n");

		T30SendFCF( CFR );
		T30Control->State = STATE_RX_PHASE_B_SIGNAL_REC;
	}
}


/*===========================================================================
*
*    T30RxSignalRec
*
*      High speed signal recognition and command input
*
*      Input:          t30_attribute.FaxVoiceRequest = TRUE if receiver has sent PIP/PIN and is
*                       waiting for PRI-Q.
*      Output:         T30Control->State = STATE_RX_GET_CMD if FSK detected
*                               = STATE_RX_PHASE_C if high speed detected
*                      T30Control->Status = TIME_OUT_ERROR if no signal detected
*
*===========================================================================*/
void T30RxSignalRec( void )
{
	UINT8 status;
	UINT8 field;
	UINT8 fDone = 0;
	UINT8 training = 0;
	UINT32 timeout;

	if ( !T30Control->V34Mode && ( T30Control->LastFCFSent == CFR ) )
	{
		TRACE_FAX( "T30RxSignalRec TimerB = 2000\n" );

		/* Wait 2 sec for TCF */
		T30_TIMER_START( TimerB, 2000 );
	}
	else
	{
		TRACE_FAX( "T30RxSignalRec TimerB = 30000\n" );

		/* Wait 30 sec for response to PIP/PIN [Note 1] */
		T30_TIMER_START( TimerB, 30000 );
	}

	while ( !fDone )
	{
		training = T30Control->ModemSpeed < T30_SPEED_V17_7200 || T30Control->ECMLongTrain;

		if ( T30Control->rcvd_post_msg == EOM )
		{
			timeout = 6000;
		}
		else
		{
			timeout = T2_TIMEOUT + 1000;
		}

		/* Do signal recognition */

		status = ModemRxHsSignalRec( T30Control->ModemSpeed, training,
			                         timeout, T30Control->ECMEnabled );

		switch ( status )
		{
			case HIGH_SPEED:

				TRACE_FAX( "T30RxSignalRec Detected HIGH SPEED\n" );

				T30Control->State = STATE_RX_PHASE_C;
				fDone = 1;
				break;

			case FSK:

				TRACE_FAX( "T30RxSignalRec Detected FSK\n" );

				if ( T30Control->LastFCFSent == CFR )
				{
					T30Control->State = STATE_RX_PHASE_B;
					T30Control->FirstCmdFlg = 1;
				}
				else
				{
					T30Control->State = STATE_RX_PHASE_D;
					T30Control->SignalRecFlg = 1;
				}

				fDone = 1;
				break;

			case TIME_OUT:

				TRACE_FAX( "T30RxSignalRec TIME_OUT\n" );

                if ( ( T30Control->State == STATE_RX_PHASE_B ) || ( T30Control->State == STATE_RX_PHASE_B_SIGNAL_REC ) )
				{
					if ( T30_TIMER_EXPIRE( TimerB ) )
					{
						TRACE_FAX( "T30RxSignalRec Abort in PhaseB\n" );
                        if (T30Control->State == STATE_RX_PHASE_B)
                        {
                            T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_6_1;
                        }
                        else
                        {
                            T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_6_2;
                        }
						fDone = 1;
					}
				}
				else
				{
					if ( T30Control->ECMEnabled )
					{
						field = ECMControl->PPS_FIF[ 0 ];
					}
					else
                    {
						field = T30Control->LastFCFRcvd;
                    }

					/* Check for EOM here */
					if ( ( field & 0xEE ) == EOM )
					{
						T30Control->State = STATE_RX_PHASE_B;
						fDone = 1;
					}
					else if ( ( field & 0xEE ) == EOP )
					{
						T30Control->State = STATE_RX_PHASE_E;
						T30RxProcessDCN();
						fDone = 1;
					}
					else if ( !t30_attribute.FaxVoiceRequest && T30_TIMER_EXPIRE( TimerB ) )
					{
						/* Time out if we didn't send a PIP/PIN (not waiting for PRI-Q) and we waited */
						/* 30 seconds for a response to PIP/PIN */
						TRACE_FAX( "T30RxSignalRec Abort in PhaseD\n" );
                        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_7;
						fDone = 1;
					}
				}
				break;

			case DISCONNECT:

				TRACE_FAX( "T30RxSignalRec DISCONNECT detected\n" );

				//T30Control->State = STATE_RX_PHASE_E;
				T30RxProcessDCN();
				fDone = 1;
				break;

			default:

				TRACE_FAX( "T30RxSignalRec Unknown status %d\n", status );

				if ( ModemControl->Status & MODEM_ABORT ) {
					fDone = 1;
				}

				//.. 12/07/10 THY check more in this case
				//fDone = 1;
				T30_TIMER_START( TimerB, 30000 );
				break;
		} /* switch */
	} /* while */

	TRACE_FAX( "T30RxSignalRec: Stop T30TimerB\n" );
}


/*===========================================================================
*
*    T30RxPhaseBDCommand
*
*      Inputs a command for the receiver during receive Phase B or D
*
*      Input:          (none)
*      Output:         Command received and stored
*
*===========================================================================*/
void T30RxPhaseBDCommand( void )
{
	UINT8 cmd, i = 0, retry;
	UINT8 fDone = 0;

	TRACE_FAX( "Entering T30RxPhaseBDCommand().....\n" );

	if ( T30Control->V34Mode )
	{
		retry = 3;  // Set retry counter

		while ( retry-- )
		{
			cmd = ModemRxSwitchPCtoCC();

			if ( cmd == NO_ERROR )
			{
				break;
			}
			else
			{
				if ( ( retry > 0 ) && ( cmd == TIME_OUT ) )
				{
					TRACE_FAX( "T30Rx TIME Out %d\n", 3 - retry );
					continue;
				}
				else
				{
					TRACE_FAX( "T30Rx CC timeout, disconnecting..\n" );
                    T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_1;
					return ;
				}
			}
		}
	}

#if 0
	if ( T30Control->LastFCFSent != RNR )
	{
		TRACE_FAX( " T30RxPhaseBDCommand reset rcvd_post_msg\n" );
		T30Control->rcvd_post_msg = 0xff;  // Clear post message
	}
#endif
	/* Start the T2 timer (default = 6.7 sec ) */
	T30_TIMER_START( TimerC, T2_TIMEOUT );

	if ( T30Control->ModemSpeed == T30_SPEED_V27_2400 )
	{
		//* Joe 06/16 work around for PPR RLSD drop out
		ModemRxTrainingDisable( FALSE );  // Enable training
	}

	T30Control->FirstCmdFlg = 1;
	retry = 3;  // Set retry counter

	while ( !fDone )
	{
		while ( 1 )
		{
			/* Look for the command */
			cmd = T30GetCommand();

			if ( cmd == T30Control->LastFCFSent )
			{
				/* Assume this is an echo and we'll get another command next */
				TRACE_FAX( " T30RxPhaseBD: echo ( %02X )\n", cmd );

				if ( cmd == MCF )
				{
					TRACE_FAX( "Exiting T30RxPhaseD by echo MCF\n" );
					T30Control->State = STATE_RX_PHASE_D_SIGNAL_REC;
					return ;
				}

				/* Restart the T2 timer */
				T30_TIMER_START( TimerC, T2_TIMEOUT );
			}
			else
				break;
		}

		TRACE_FAXDEBUG("T30Rx Got command ( %02X )\n", cmd);

		retry--;

		/* Fax error? */
		if ( T30Control->Status )
		{
			fDone = 1;
			break;
		}

		/* Process the command */
		switch ( cmd )
		{
			case FRAME_ERROR:

				if ( retry > 0 )
				{
					T30SendFCF( CRP );
					continue;
				}
				else
				{
					TRACE_FAX( "T30Rx frame error ---> Phase E\n" );
                    T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_8;
					fDone = 1;
				}
				break;

				 /* Fatal error in frame? */
			case DISCONNECT:

				TRACE_FAX( "T30Rx Fatal frame error, disconnecting..\n" );
                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_2;
				fDone = 1;
				break;

				 /* Did we get a command? */
			case NO_COMMAND:

				/* Do signal recognition again if got a bad command */

				if ( T30Control->SignalRecFlg )
				{
					TRACE_FAX( "T30Rx SignalRecFlag\n" );

					T30Control->State = STATE_RX_PHASE_D_SIGNAL_REC;
					fDone = 1;
				}
				else if ( retry > 0 )
				{
					TRACE_FAX( "T30Rx ?NC\n" );

					T30_TIMER_START( TimerC, T2_TIMEOUT );
					OSC_Sleep( AUTO_T4_TIMEOUT );
					continue;
				}
				else
				{
					TRACE_FAX( "T30Rx 3 retry Out ---> Phase E\n" );

                    T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_9;
					fDone = 1;
				}

				break;

			case TIME_OUT:

				if ( retry > 0 )
				{
					TRACE_FAX( "T30Rx TIME Out %d\n", i );

					T30_TIMER_START( TimerC, T2_TIMEOUT );
					OSC_Sleep( AUTO_T4_TIMEOUT );
					continue;
				}
				else
				{
					if ( ( T30Control->rcvd_post_msg == EOP )
					  && ( T30Control->LastFCFSent == MCF ) )
					{
						TRACE_FAX( "T30Rx TIME Out ---> Phase E\n" );

						fsu_rx_fax_cancel(0);
						T30Control->State = STATE_RX_PHASE_E;
						fDone = 1;
					}
					else
					{
						TRACE_FAX( "T30Rx TIME Out ---> Phase D\n" );

                        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_10;
						fDone = 1;
					}
				}

				break;

				/* Got a good command */
			default:

				fDone = 1;

				/* DIS might not be an echo. Disconnect for anybody's DCN. An echo of our response? */

				if ( t30_attribute.FaxOptions.IgnoreEcho
				  && ( cmd != DIS )
				  && ( cmd != DCN )
				  && ( cmd == T30Control->LastFCFSent ) )
				{
					/* Assume this is an echo and we'll get another command next */

					for ( i = 0; i < EchoTblSize; i++ )
					{
						if ( cmd == EchoTbl[ i ] )
						{
							T30Control->State = STATE_RX_PHASE_D;
							break;
						}
					}

					/* Not an echo so do signal recognition next */
					T30Control->State = STATE_RX_PHASE_D_SIGNAL_REC;
				}
				else
				{
					if ( ( cmd == DCN ) && ( T30Control->LastFCFSent == FTT ) )
					{
						fsu_rx_fax_cancel(0);
						T30Control->State = STATE_RX_PHASE_E;
					}
					else
					{
						/* Not an echo or DIS or DCN so process the command */
						T30RxPhaseBDProcessCommand();
					}
				}

				break;
		} /* switch */

		OSC_Sleep( T30_DELAY_TIME );

	}  /* while */

	T30Control->SignalRecFlg = 0;
}


/*===========================================================================
*
*    T30RxPhaseA
*
*      Call establishment
*
*      Input:          (none)
*      Output:         T30Control->State = STATE_RX_PHASE_B
*
*===========================================================================*/
void T30RxPhaseA( void )
{
	UINT8 speed, status;
	UINT8 poll;

	TRACE_FAX( "T30RxPhaseA().....\n" );

	speed = t30_attribute.FaxRxStartSpeed;

	if ( t30_attribute.ECMMode == ECM_NONE )
	{
		if ( speed > T30_SPEED_V17_14400 )
			speed = T30_SPEED_V17_14400;
	}

	ModemRxTrainingDisable( FALSE );   // Enable training

	T30Control->PPRSendCnt = 0;      // THY

    /* Should a document to be polled or be indicated ? */

	if ( ( ( t30_attribute.PollTxSource == TXPOLLFROM_MEMORY ) && poll_tx_job_ready() ) || ( ( t30_attribute.PollTxSource == TXPOLLFROM_SCAN ) && T30Control->ScnDocPresent ) )
	{
        poll = 1;
	}
	else
	{
        poll = 0;
	}
	
    TRACE_FAX("T30RxPhaseA() - speed: %d, poll: %d, gFaxCEDDetectionTime: %ld\n\n", speed, poll, gFaxCEDDetectionTime);

	/* Do billing delay */
    status = ModemRxBillingDelay( &speed, poll, gFaxCEDDetectionTime );
	
	TRACE_FAX("T30RxPhaseA() - status: %d\n", status);

	/* Check if there were problems with negotiations */
	if ( status == MODEM_FAX_PHASE_A_ABORT && !T30Control->Status )
	{
		TRACE_FAX( "T30RxPhaseA timeout\n" );
        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_14;
		return;
	}

    if ( ( status == MODEM_FAX_PHASE_A_V34_POLL ) || ( status == MODEM_FAX_PHASE_A_V34 ) || ( status == MODEM_FAX_PHASE_A_NON_V34 ) )
	{
		T30Control->TriedV8++;
	}

	T30Control->V34Mode = ModemControl->V34Mode;

	if ( T30Control->V34Mode )
	{
		T30Control->ModemSpeed = speed;
	}
	else if ( t30_attribute.FaxRxStartSpeed > T30_SPEED_V17_14400 )
	{
		T30Control->ModemSpeed = T30_SPEED_V17_14400;
		OSC_Sleep( 450 );  // T.30 Amd1
	}
	else
	{
		T30Control->ModemSpeed = t30_attribute.FaxRxStartSpeed;
	}

	TRACE_FAX( "T30RxPhaseA negotiation status %d, speed %d\n", status, speed );

	/* Enter Phase B if no printer or memory faults. */
	if ( !T30RxCheckErrors() )
	{
		T30Control->State = STATE_RX_PHASE_B;
	}
}


/*===========================================================================
*
*    T30RxPhaseB
*
*      Transmits CSI/DIS or CIG/DTC and looks for a response
*
*      Input:          t30_attribute.FaxRxPolling = TRUE if we are trying to receive by polling
*      Output:         T30Control->LastFCFRcvd = FCF of command if any received
*                      T30Control->Status = TIME_OUT_ERROR if T1 time-out occurs
*                               = BAD_RSP_ERROR if a fatal HDLC error occurs
*                      T30Control->State = STATE_RX_PROC_CMD if a response is found
*
*===========================================================================*/
void T30RxPhaseB( void )
{
	UINT8 rsp;
	UINT8 fDone = 0;
	UINT32 time;

	TRACE_FAX( " T30RxPhaseB().....\n" );

    if ( ( T30Control->LastFCFSent != CFR ) && ( T30Control->LastFCFSent != FTT ) )
	{
		T30Control->got_PWD = 0;
		T30Control->got_SID = 0;
		T30Control->got_SUB = 0;
		T30Control->got_SEP = 0;
		T30Control->got_PSA = 0;

		/* Make up the DIS frame */
		T30RxMakeDIS();

		/* Would have received the DIS in receive by polling first phase B */
		if ( !t30_attribute.FaxRxPolling || T30Control->PageNum )
		{
			T30Control->DISFlg = 0;

			/* Clear ECM, MMR, superfine, remote ID, and DIS received */
			memset( T30Control->RemoteCap, 0, sizeof( T30Control->RemoteCap ) );
			memset( t30_attribute.FaxRemoteID, 0, sizeof( t30_attribute.FaxRemoteID ) );
			memset( t30_attribute.FaxRemotePassword, 0, sizeof( t30_attribute.FaxRemotePassword ) );
			memset( t30_attribute.FaxRemoteSubaddress, 0, sizeof( t30_attribute.FaxRemoteSubaddress ) );
			memset( t30_attribute.PollRemotePassword, 0, sizeof( t30_attribute.PollRemotePassword ) );
			memset( t30_attribute.PollRemoteSubaddress, 0, sizeof( t30_attribute.PollRemoteSubaddress ) );
			memset( t30_attribute.FaxRemoteNSF, 0, sizeof( t30_attribute.FaxRemoteNSF ) );
			memset( t30_attribute.FaxRemoteNSS, 0, sizeof( t30_attribute.FaxRemoteNSS ) );
			memset( t30_attribute.FaxRemoteNSC, 0, sizeof( t30_attribute.FaxRemoteNSC ) );
		}
	}

	/* Set the T1 timer (default = 38 sec) */
    time = gFaxT1Timeout;

    if ( T30Control->V34Mode )
    {
		time -= AUTO_T4_TIMEOUT;
    }
	else
	{
		if ( T30Control->ManualMode )
        {
			time -= MANUAL_T4_TIMEOUT;
        }
		else
        {
			time -= AUTO_T4_TIMEOUT;
        }
	}

	T30_TIMER_START( TimerB, time );

	while ( !fDone )
	{
		/* Receive by polling? */

        if ( ( T30Control->LastFCFSent != CFR ) && ( T30Control->LastFCFSent != FTT ) )
		{
			if ( t30_attribute.FaxRxPolling )
			{
                if ( ( t30_attribute.PollRxRequirePassword ) && ( t30_attribute.PollLocalRxPassword[ 0 ] != 0 ) )
				{
					if ( BITSET( &T30Control->RemoteCap[ 6 ], 0x02 ) )
					{
						TRACE_FAX(" T30RxPhaseB: Send ( PWD )\n");

						FIFSendTable[ PWDtbl ] = t30_attribute.PollLocalRxPassword;
						T30SendFCF( PWD );
					}
				}

                if ( ( t30_attribute.PollRxRequireSubaddress ) && ( t30_attribute.PollLocalRxSubaddress[ 0 ] != 0 ) )
				{
					if ( BITSET( &T30Control->RemoteCap[ 5 ], 0x40 ) )
					{
						TRACE_FAX(" T30RxPhaseB: Send ( SEP )\n");

						FIFSendTable[ SEPtbl ] = t30_attribute.PollLocalRxSubaddress;
						T30SendFCF( SEP );
					}
				}

				if ( t30_attribute.FaxEnableNSF )
				{
					TRACE_FAX(" T30RxPhaseB: Send ( NSC )\n");

					T30SendFCF( NSC );
				}

				if ( FIFSendTable[ CIGtbl ][ 0 ] != 0 )
				{
					TRACE_FAX(" T30RxPhaseB: Send ( CIG )\n");

					T30SendFCF( CIG );
				}

				TRACE_FAX(" T30RxPhaseB: Send ( DTC )\n");

				T30SendFCF( DTC );
			}
			else
			{
				if ( t30_attribute.FaxEnableNSF )
				{
					TRACE_FAX(" T30RxPhaseB: Send ( NSF )\n");

					T30SendFCF( NSF );
				}

				if ( FIFSendTable[ CSItbl ][ 0 ] != 0 )
				{
					TRACE_FAX(" T30RxPhaseB: Send ( CSI )\n");

					T30SendFCF( CSI );
				}

				TRACE_FAX(" T30RxPhaseB: Send ( DIS )\n");

				T30SendFCF( DIS );
			}
		}

		/* Fax error? */
		if ( T30RxCheckErrors() )
		{
			fDone = 1;
			break;
		}

		/* Get the response */
		// Check CRC error or bad frame - when some test script sends DCS with CRC error
		// this program does not receive next DCS because of receive timming.
		// One possible solution will make program wait for next DCS without sending DIS.
		// As a same reason update program to retun DISCONNECT when it has CRC error in
		// HDLCRxResponse() in T30GetResponse().
		// TYH 8/27/01

		while ( 1 )
		{
			rsp = T30GetResponse();

			/* Fax error? */
			if ( T30RxCheckErrors() )
			{
				fDone = 1;
				break;
			}

			if ( rsp == T30Control->LastFCFSent )
			{
				/* Assume this is an echo and we'll get another command next */
				TRACE_FAX( " T30RxPhaseB: echo ( %02X )\n", rsp );

				if ( rsp == CFR )
				{
					TRACE_FAX( "Exiting T30RxPhaseB by echo CFR\n" );
					T30Control->State = STATE_RX_PHASE_B_SIGNAL_REC;
					return ;
				}
			}
			else
				break;
		}

		/* Fax error? */
		if ( T30RxCheckErrors() )
		{
			fDone = 1;
			break;
		}

		/* Time out? */
		if ( T30_TIMER_EXPIRE( TimerB ) )
		{
			TRACE_FAX( " T30RxPhaseB: Timeout. Send ( DCN )\n" );

            T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_11;
			T30SendFCF( DCN );
			fDone = 1;
			break;
		}

		TRACE_FAX( " T30RxPhaseB: Processing command %d\n", rsp  );

		/* Process the command */
		switch ( rsp )
		{
			case DISCONNECT:
				TRACE_FAX( " T30RxPhaseB: DISCONNECT\n" );    // THY
				break;

				/* CRC error ? */
			case FRAME_ERROR:
				TRACE_FAX( " T30RxPhaseB: CRC error detected\n" );

				/* Wait 3.5 sec to skip V.17 long TCF */
				OSC_Sleep( 3500 );

				if ( !T30_TIMER_EXPIRE( TimerB ) )
				{
					TRACE_FAX( " T30RxPhaseB: Resend DIS\n" );

					continue;
				}

				TRACE_FAX( " T30RxPhaseB: exit from while()\n" );

				fDone = 1;
				break;
				
			case CRP:
                TRACE_FAX( " T30RxPhaseB: got ( CRP )\n" );
				break;

				/* No command? */
			case TIME_OUT:
			case NO_RESPONSE:
                TRACE_FAX( " T30RxPhaseB: NO_RESPONSE/TIME_OUT\n" );
				break;

			case CI_TONE:

				/* If V34 mode has not been negotiated but is allowed then
				   check whether the remote wants to switch to V34 */
                if ( !T30Control->V34Mode && ( T30Control->TriedV8 < 3 ) && ( t30_attribute.FaxRxStartSpeed > T30_SPEED_V17_14400 ) )
				{
					ModemControl->V34Mode = TRUE;
					T30Control->V34Mode = TRUE;
					T30Control->TriedV8++;
					T30Control->ModemSpeed = t30_attribute.FaxRxStartSpeed;
					/* Should restart phase A in this case */
					T30Control->State = STATE_RX_PHASE_A;
					fDone = 1;

					TRACE_FAX( " V34 restart speed %d\n", T30Control->ModemSpeed );
				}
				break;

				/* Good command */
			default:

				if ( !T30RxCheckErrors() )
				{
					/* Process the response */
					T30RxPhaseBDProcessCommand();
				}

				fDone = 1;
				break;
		} /* switch */
	} /* while */

	TRACE_FAX( "Exiting T30RxPhaseB...\n" );
}


/*===========================================================================
*
*    T30RxPhaseC
*
*      Receives a page of a document
*
*      Input:
*      Output:
*
*===========================================================================*/
void T30RxPhaseC( void )
{
	UINT8 RTC_Detected = 0;
	UINT8 fsk_count, carrier_status = 0;
    UINT16 rx_speed = 0;
	UINT8 fDone;

    TRACE_FAX( "T30RxPhaseC().....\n");

    //TRACE_FAX("\n(YHCHO) %s() - T30Control->PageNum: %d\n", __FUNCTION__, T30Control->PageNum);

    //TRACE_FAX( "T30RxPhaseC()..... Page %d\n", T30Control->PageNum+1);

	/* If ECM mode, get an ECM block next */
	if ( T30Control->ECMEnabled )
	{
		TRACE_FAX( " ECM %d bps\n", ModemControl->ConnectedSpeed );

		T4_end_of_page = FALSE;
		T30Control->State = STATE_RX_PHASE_C_ECM;
		return;
	}

	TRACE_FAX( " NON-ECM %d bps\n", ModemControl->ConnectedSpeed );

	if(T30Control->PageNum)
	{
        if(fsu_rcv_next_page() == FALSE)
		{
            T30Control->Status = FAX_ERROR_NO_MEM_ERROR;
			return;
		}
	}

	T4_end_of_page = FALSE;

	T30Control->LastFCFSent   = 0xff;
	T30Control->rcvd_post_msg = 0xff;  // Clear post message

	prepare_rx_buffers(NUM_OF_BUFF, FALSE);			// prepare buffers for rx data

	/* Increment the page number */
	T30Control->PageNum++;

	/* Set modem tone detector to detect FSK flags */
	ModemSetupToneDet( MODEM_HISPEED_FSK_TONE );
	ModemSetupToneDet( MODEM_FSK_TONE );

   	ModemControl->State  = T4_RX_MODE;
 	ModemControl->Status = NO_ERROR;
 	T30Control->LostCarrFlg = 0;

	ModemControl->EOLCount = 0;
	ModemControl->ZeroCount = 0;
	ModemControl->FoundRTC = FALSE;

	/* Start EOL timeout for failsafe */
	last_EOL_time = ModemElapsedTime( 0 );

	ModemRxCarrierError( 0 );

	TRACE_FAX( "T30RxPhaseC: Start IRQ\n" );

	/* Enable receive data interrupts */
	ModemEnableRxInt();

	ModemControl->EQM = 0xffff;
	fsk_count = 0;
	fDone = 0;

	/* Monitor for various errors while the pipeline is running */
	while ( !fDone )
	{
		OSC_Sleep( T30_DELAY_TIME );

#ifdef OEM_FAX_REPORT
		if ( ( ModemControl->EQM != 0 ) && ( ModemControl->EQM != 0xffff ) )
		{
            rx_speed = ( ModemControl->ConnectedSpeed / 1000 );
			CxProtocolReport( IMG, &rx_speed, 1, RX_LOG );
			ModemControl->EQM = 0;  // stop to report again
		}
#endif

		/* Check abort or any errors */

		if ( T30RxCheckErrors() )
		{
			TRACE_ERROR("Exit T30Rx - T30RxCheckErrors()\n");

			ModemClearIRQ();
			ModemDisableToneDet( MODEM_HISPEED_FSK_TONE );
			ModemRxForceEndOfPage( 0 );
			ModemControl->State = MODEM_IDLE;
			ModemControl->Status |= MODEM_ABORT;
			T30Control->State = STATE_RX_PHASE_D;
            T30Control->Status = FAX_ERROR_BAD_RX_ERROR_1;
			return ;
		}

		/* Check no EOL */

		if ( ModemElapsedTime( last_EOL_time ) > 60000 )  // 60sec
		{
			TRACE_FAX( "T30RxPhaseC: EOL timeout\n" );

			ModemClearIRQ();
			ModemControl->State = MODEM_IDLE;
			HDLCRxWriteInitAllFrames();
			T30Control->ECMLongTrain = 0;
            T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_12;
			T30Control->State = STATE_RX_ABORT;
			fDone = 1;
			return ;
		}

		/* Check RTC status */

		if ( ModemControl->FoundRTC )
		{
            TRACE_FAX( "T30RxPhaseC: RTC Detected\n" );
			TRACE_FAX( "ModemControl->State: %d\n", ModemControl->State );

#ifdef OEM_FAX_REPORT
            CxProtocolReport( RTC, NULL, 0, RX_LOG );
			ModemControl->EQM = 0;  // stop to report again
#endif
			break;
		}

		/* Check for signal drop out or end of page */

		carrier_status = ModemRxCarrierError( 1 );

		if ( ( carrier_status != CARRIER_ON ) && ( fsk_count == 0 ) )  /* Bail if carrier lost */
		{
			TRACE_FAX( "T30RxPhaseC: Start dropout timer %d\n", carrier_status );

			/* Wait upto 6 seconds for carrier to comeback */
			T30_TIMER_START( TimerC, 6100 );

			do {

				OSC_Sleep( T30_DELAY_TIME );

				carrier_status = ModemRxCarrierError( 1 );

				if ( carrier_status == CARRIER_ON )
				{
					break;
				}
				else
				{
					if ( carrier_status != CARRIER_OFF )
					{
						/* restart 6 seconds carrier off */
						T30_TIMER_START( TimerC, 6100 );
					}
				}

				if ( ModemElapsedTime( last_EOL_time ) > 60000 )
				{
					break;
				}

				if ( ModemToneDetected( MODEM_HISPEED_FSK_TONE )
				  && ModemToneDetected( MODEM_FSK_TONE ) )
				{
                    if ( ++fsk_count > ( 6 /*200 / T30_DELAY_TIME*/ ) )
					{
						TRACE_FAX( "T30RxPhaseC: 1st FSK detected\n" );
						break;
					}
				}
				else
				{
					fsk_count = 0;
				}

				if ( T30_TIMER_EXPIRE( TimerC ) )
				{
					switch ( carrier_status )
					{
						case CARRIER_OFF:

							TRACE_FAX( "T30RxPhaseC: No Carrier\n" );

							ModemControl->State = MODEM_IDLE;
                            T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_13;
							T30Control->State = STATE_RX_ABORT;
							fDone = 1;
							return ;

						case EQM_BAD:

							TRACE_FAX( "T30RxPhaseC: Bad EQM Error\n" );
							break;

						case RLSD_OFF:

							TRACE_FAX( "T30RxPhaseC: Lost sync, RLSD off\n" );
							break;

						default:
							break;
					}

					T30_TIMER_START( TimerC, 6100 );
				}

				if ( T30RxCheckErrors() )
				{
					switch ( carrier_status )
					{
						case CARRIER_OFF:

							TRACE_FAX( "T30RxPhaseC: No Carrier\n" );
							break;

						case EQM_BAD:

							TRACE_FAX( "T30RxPhaseC: Bad EQM Error\n" );
							break;

						case RLSD_OFF:

							TRACE_FAX( "T30RxPhaseC: Lost sync, RLSD off\n" );
							break;

						default:
							TRACE_FAX( "T30RxPhaseC: Abort\n" );
							break;
					}

					ModemControl->State = MODEM_IDLE;
					if (carrier_status == CARRIER_OFF)
					{
						T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_15_1;
					}
					else if (carrier_status == EQM_BAD)
					{
						T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_15_2;
					}
					else if (carrier_status == RLSD_OFF)
					{
						T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_15_3;
					}
					else
					{
						T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_15_4;
					}
					T30Control->State = STATE_RX_ABORT;
					fDone = 1;
					return ;
				}

			} while ( carrier_status );

			TRACE_FAX( "T30RxPhaseC: Stop dropout timer %d\n", carrier_status );
		}

		/* Check FSK signal */

		if ( ModemToneDetected( MODEM_HISPEED_FSK_TONE )
		  && ModemToneDetected( MODEM_FSK_TONE ) )
		{
			if ( ++fsk_count > ( 10 /*300 / T30_DELAY_TIME*/ ) )
			{
				TRACE_FAX( "T30RxPhaseC: FSK detected\n" );

				T30Control->LostCarrFlg = 1;

				if ( ModemElapsedTime( last_EOL_time ) < 1000 )
				{
					ModemControl->FoundRTC = TRUE;
				}

				break;
			}
		}
		else
		{
			fsk_count = 0;
		}

	}  // while( !fDone )

	TRACE_FAX( "T30RxPhaseC: detect RTC or FSK or line drop\n" );

	ModemClearIRQ();
	ModemRxTrainingDisable( FALSE );
	ModemDisableToneDet( MODEM_HISPEED_FSK_TONE );
	ModemRxForceEndOfPage( RTC_Detected );
	ModemControl->State = MODEM_IDLE;
	T30Control->State = STATE_RX_PHASE_D;

	if ( T30Control->RxToMemFlg )
		T30Control->MemPageNum++;

	if ( !T30Control->Status )
	{
		//T4CodecQuery( T4CODEC_TOO_MANY_ERRORS, &too_many_error);
#if 0
		if (too_many_error)
		{
			TRACE_ERROR( "T30RxPhaseC(): Too many decompression errors: %d\n" , too_many_error);
			
			if(too_many_error == 1)
            {
				T30Control->Status = FAX_ERROR_BAD_RX_ERROR_2_1;
            }
            else if(too_many_error == 2)
            {
				T30Control->Status = FAX_ERROR_INVALID_HEIGHT_1;
            }
            else if(too_many_error == 3)
            {
				T30Control->Status = FAX_ERROR_JIBG_DECODE_ERROR_1;
			}
			else
			{
				//temp code
				T30Control->Status = FAX_ERROR_BAD_RX_ERROR_2_1;
			}
			
            too_many_error = 0;
		}
#endif
	}

	/* Get a command next */
	TRACE_FAX( "T30RxPhaseC ..... End of Phase C\n" );
}


/*===========================================================================
*
*    T30RxPhaseE
*
*      Cuts the recording paper
*
*      Input:          (none)
*      Output:         T30Control->State = STATE_RX_DISCONN
*
*===========================================================================*/
void T30RxPhaseE( void )
{
	TRACE_FAX( "T30RxPhaseE().....\n" );

	if ( T30Control->ECMEnabled && T30Control->Status )
		ECMSendBlockOnError();

	/* Send a disconnect message if ending with an error
	   if none have been exchanged yet */
	if ( T30Control->Status
	  && ( T30Control->LastFCFSent != DCN )
	  && ( ( T30Control->LastFCFRcvd & 0xEE ) != DCN ) )
	{
		T30SendFCF ( DCN );
	}

	ModemControl->State = MODEM_IDLE;
	T30Control->State = STATE_RX_DISCONN;

	TRACE_FAX( "T30RxPhaseE() Exit\n" );
}


/*===========================================================================
*
*    T30RxProcessBadCommand
*
*      Processes a bad command during receive Phase B or D or any other situation
*      which would require disconnecting
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessBadCommand( void )
{
	TRACE_FAX( "T30Rx: Bad Command\n" );

	T30SendFCF( DCN );

	T30Control->State  = STATE_RX_DISCONN;
    T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_3;
}


/*===========================================================================
*
*    T30RxProcessDCN
*
*      Processes a DCN received during receive Phase B or D
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessDCN( void )
{
	TRACE_FAX( "T30Rx: ( DCN )\n" );

	TRACE_FAX(" State %d\n", T30Control->State );
	TRACE_FAX(" LastFCFSent %02X\n", T30Control->LastFCFSent );
	TRACE_FAX(" rcvd_post_msg %02X\n", T30Control->rcvd_post_msg );

	if ( (T30Control->LastFCFSent != MCF  && T30Control->Status == 0 ) ||
		 (T30Control->State == STATE_RX_PHASE_D_SIGNAL_REC) ||
		 ((T30Control->rcvd_post_msg == NUL || T30Control->rcvd_post_msg == EOM || T30Control->rcvd_post_msg == MPS)
		    && T30Control->Status == 0))
	{
        if (T30Control->LastFCFSent != MCF && T30Control->Status == 0)
        {
            if ( (RNRSentCount >= RNR_SENT_CNT) && (T4_end_of_page == FALSE) )
            {
                T30Control->Status = FAX_ERROR_RR_RNR; // Maybe RR-RNR problem... but not exact expression...
            }
            else if( (T30Control->rcvd_post_msg == EOP) && ((T30Control->LastFCFSent == RTN) || (T30Control->LastFCFSent == RTP)) )
            {
            	T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_4_6;
            }
            else
            {
                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_4_1;
            }
        }
        else if (T30Control->State == STATE_RX_PHASE_D_SIGNAL_REC)
        {
            T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_4_2;
        }
        else if (T30Control->rcvd_post_msg == NUL && T30Control->Status == 0)
        {
            T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_4_3;
        }
        else if (T30Control->rcvd_post_msg == EOM && T30Control->Status == 0)
        {
            T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_4_4;
        }
        else if (T30Control->rcvd_post_msg == MPS && T30Control->Status == 0)
        {
            T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_4_5;
        }
    }

	T30Control->State = STATE_RX_DISCONN;
}


/*===========================================================================
*
*    T30RxProcessDCS
*
*      Processes a DCS received during receive Phase B
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessDCS( void )
{
    UINT8 i = 0;
    UINT8 ret = 0;

	TRACE_FAX( "T30Rx: ( DCS )\n" );

	SSFStopOffHookMonitor();

	if ( t30_attribute.RxRequirePassword )
	{
		if ( BITSET( &T30Control->RemoteCap[ 6 ], 0x02 ) )
		{
			if ( memcmp( t30_attribute.FaxLocalRxPassword, t30_attribute.FaxRemotePassword, sizeof( t30_attribute.FaxLocalRxPassword ) ) != 0 )
			{
				TRACE_FAX(" T30RxProcessDCS: Invalid password\n" );
				TRACE_FAX( " Local  [" );
                for ( i = 0; i < StandardFIFSize; i++ )
				{
					TRACE_FAX( " %c", t30_attribute.FaxLocalRxPassword[ i ] );
				}
				TRACE_FAX( " ]\n" );
				TRACE_FAX( " Remote [" );
                for ( i = 0; i < StandardFIFSize; i++ )
				{
					TRACE_FAX( " %c", t30_attribute.FaxRemotePassword[ i ] );
				}
				TRACE_FAX( " ]\n" );

#ifdef OEM_FAX_REPORT
				ModemControl->Level = 0;
				ModemControl->EQM   = 0xffff;
                CxProtocolReport( PWD_ERR, NULL, 0, RX_LOG );
#endif
                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_5;
			}
			else
			{
				TRACE_FAX(" T30RxProcessDCS: Match password\n" );
			}
		}
		else
		{
			TRACE_FAX(" T30RxProcessDCS: No password\n" );

#ifdef OEM_FAX_REPORT
			ModemControl->Level = 0;
			ModemControl->EQM   = 0xffff;
            CxProtocolReport( PWD_ERR, NULL, 0, RX_LOG );
#endif

            T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_6;
		}
	}

	if ( t30_attribute.RxRequireSubaddress )
	{
		if ( BITSET( &T30Control->RemoteCap[ 6 ], 0x01 ) )
		{
			if ( memcmp( t30_attribute.FaxLocalRxSubaddress, t30_attribute.FaxRemoteSubaddress, sizeof( t30_attribute.FaxLocalRxSubaddress ) ) != 0 )
			{
				TRACE_FAX(" T30RxProcessDCS: Invalid subaddress\n" );
				TRACE_FAX( " Local  [" );
                for ( i = 0; i < StandardFIFSize; i++ )
				{
					TRACE_FAX( " %c", t30_attribute.FaxLocalRxSubaddress[ i ] );
				}
				TRACE_FAX( " ]\n" );
				TRACE_FAX( " Remote [" );
                for ( i = 0; i < StandardFIFSize; i++ )
				{
					TRACE_FAX( " %c", t30_attribute.FaxRemoteSubaddress[ i ] );
				}
				TRACE_FAX( " ]\n" );

#ifdef OEM_FAX_REPORT
				ModemControl->Level = 0;
				ModemControl->EQM   = 0xffff;
                CxProtocolReport( SUB_ERR, NULL, 0, RX_LOG );
#endif

                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_15;
			}
			else
			{
				TRACE_FAX(" T30RxProcessDCS: Match subaddress\n" );
			}
		}
		else
		{
			TRACE_FAX(" T30RxProcessDCS: No subaddress\n" );

#ifdef OEM_FAX_REPORT
			ModemControl->Level = 0;
			ModemControl->EQM   = 0xffff;
            CxProtocolReport( SUB_ERR, NULL, 0, RX_LOG );
#endif

            T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_16;
		}
	}

	if( !T30Control->DocCreateFlag && T30Control->Status == 0)
	{
        ret = start_fsu_rcv_to_mem();

        if (ret == 0)
        {
            T30Control->Status = FAX_ERROR_NO_ERROR;
        }
        else if ( (ret == 1) || (ret == 2) )
        {
            T30Control->Status = FAX_ERROR_NO_MEM_ERROR;
        }
        else if (ret == 3)
        {
            T30Control->Status = FAX_ERROR_JOB_QUEUE_FULL_ERROR;
        }
        else if (ret == 4)
        {
            T30Control->Status = FAX_ERROR_RECEIVE_SPAM_FAX;
        }

		T30Control->DocCreateFlag = 1;
	}

	if ( T30Control->Status == 0 )
	{
		/* Find the first byte in the DCS that indicates there
		   are no more extended bytes available */
		for ( i = 2; i < RemoteCapSize; ++i )
			if ( !( T30Control->RemoteCap[ i ] & 0x80 ) )
				break;

		/* And zero out all the following bytes of DCS */
		for ( ++i; i < RemoteCapSize; ++i )
			T30Control->RemoteCap[ i ] = 0;

		T30RxReadDCS();
	}

	if ( T30Control->Status )
		i = DCN;
	else
		i = CFR;

	T30RxTrainingCheck( i );
}


/*===========================================================================
*
*    T30RxProcessDIS
*
*      Processes a DIS received during receive Phase B
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessDIS( void )
{
	TRACE_FAX( "T30Rx: ( DIS )\n" );

	/* If this is the second DIS, treat it as a DTC */
	if ( T30Control->DISFlg )
		T30RxProcessDTC();
	else
	{
		UINT8 i, match = 1;

		for ( i = 0; i < LocalDISSize; ++i )
			if ( T30Control->LocalCap[ i ] != T30Control->RemoteCap[ i ] )
				match = 0;

		T30Control->DISFlg = 1;

		if ( !t30_attribute.FaxOptions.IgnoreEcho && !match )
			T30RxProcessDTC();

		/* Assume this is an echo and wait for more closing flags, etc. */
		else
		{
			OSC_Sleep( 500 );
			T30Control->State = STATE_RX_PHASE_D;
		}
	}
}


/*===========================================================================
*
*    T30RxProcessDTC
*
*      Processes a DTC received during receive Phase B
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessDTC( void )
{
    UINT8 index = 0;

    TRACE_FAX( "T30Rx: ( DTC )\n" );

    if ( t30_attribute.PollTxRequirePassword )
    {
        if ( BITSET( &T30Control->RemoteCap[ 6 ], 0x02 ) && T30Control->got_PWD )
        {
            if ( memcmp( t30_attribute.PollLocalTxPassword, t30_attribute.PollRemotePassword, sizeof( t30_attribute.PollLocalTxPassword ) ) != 0 )
            {
                TRACE_FAX(" T30RxProcessDTC: Invalid password\n" );
                TRACE_FAX( " Local  [" );

                for ( index = 0; index < StandardFIFSize; index++ )
                {
                    TRACE_FAX( " %c", t30_attribute.PollLocalTxPassword[ index ] );
                }

                TRACE_FAX( " ]\n" );
                TRACE_FAX( " Remote [" );

                for ( index = 0; index < StandardFIFSize; index++ )
                {
                    TRACE_FAX( " %c", t30_attribute.PollRemotePassword[ index ] );
                }

                TRACE_FAX( " ]\n" );

                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_17;

                return ;
            }
            else
            {
                TRACE_FAX(" T30RxProcessDTC: Match password\n" );
            }
        }
        else
        {
            TRACE_FAX(" T30RxProcessDTC: No password\n" );

            T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_18;

            return ;
        }
    }

    if ( t30_attribute.PollTxRequireSubaddress )
    {
        if ( ( BITSET( &T30Control->RemoteCap[ 5 ], 0x80 ) && T30Control->got_SEP ) || ( BITSET( &T30Control->RemoteCap[ 4 ], 0x04 ) && T30Control->got_PSA ) )
        {
            if ( memcmp( t30_attribute.PollLocalTxSubaddress, t30_attribute.PollRemoteSubaddress, sizeof( t30_attribute.PollLocalTxSubaddress ) ) != 0 )
            {
                TRACE_FAX(" T30RxProcessDTC: Invalid subaddress\n" );
                TRACE_FAX( " Local  [" );

                for ( index = 0; index < StandardFIFSize; index++ )
                {
                    TRACE_FAX( " %c", t30_attribute.PollLocalTxSubaddress[ index ] );
                }

                TRACE_FAX( " ]\n" );
                TRACE_FAX( " Remote [" );

                for ( index = 0; index < StandardFIFSize; index++ )
                {
                    TRACE_FAX( " %c", t30_attribute.PollRemoteSubaddress[ index ] );
                }

                TRACE_FAX( " ]\n" );

                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_19;

                return ;
            }
            else
            {
                TRACE_FAX(" T30RxProcessDTC: Match subaddress\n" );
            }
        }
        else
        {
            TRACE_FAX(" T30RxProcessDTC: No subaddress\n" );

            T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_20;

            return ;
        }
    }

    /* First check for possible documents in memory for polling */
    if ( t30_attribute.PollTxSource == TXPOLLFROM_MEMORY )
    {
        UINT16 jnum = fsu_job_num_send;

        if(poll_tx_job_ready() == 1)
        {
            char DocDirPathNAND[MAX_LEN_FILE_PATH_NAME] = {0, };
            char DocDirPathRAM[MAX_LEN_FILE_PATH_NAME] = {0, };
            UINT16 DocId = 0;

            T30Control->TxFromMemFlg = 1;

            T30Control->T4Params.Tx.DocID = fsu_get_doc_id(jnum);

            if (fsu_get_photo_mode(jnum) == 1)
            {
                T30Control->T4Params.Tx.PagesInDoc = fsu_get_total_page_no_in_mem(T30Control->T4Params.Tx.DocID, FAX_FILE_TYPE_PHOTO);
            }
            else
            {
                T30Control->T4Params.Tx.PagesInDoc = fsu_get_total_page_no_in_mem(T30Control->T4Params.Tx.DocID, FAX_FILE_TYPE_JBG);
            }

            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_POLL_TX_START, 0, 0, 0, 0, jnum);

            OSC_Sleep(200); // To give enough time for UIManager

            faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_CLEAR_STATUS_BAR, 0, 0, 0, 0, fsu_job_num_rcv);

            DocId = fsu_get_doc_id(fsu_job_num_rcv);

            snprintf(DocDirPathNAND, sizeof(DocDirPathNAND), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, DocId);

            snprintf(DocDirPathRAM, sizeof(DocDirPathRAM), "%s%cDoc%04X", pRootFolderNameRAM, PATH_SEPARATOR, DocId);

            fsu_delete_dir_on_file_system(DocDirPathNAND);

            fsu_delete_dir_on_file_system(DocDirPathRAM);

            fsu_update_status(fsu_job_num_rcv, FSU_STATE_NULL);

				tx_report_image_setting(jnum, 1);
        }
    }

    /* So that DCN will be sent properly in Tx Phase E */
    T30Control->CalledStationDetected = 1;

    /* Generate coverpage if necessary */
    T30Control->T4Params.Tx.CoverPage = t30_attribute.CoverPage;

    /* Set up for turn around transmit */
    T30Control->TxPollFlg = 1;

    /* Wait for receive to be done */
    while ( T30Control->PagesDone < T30Control->PageNum )
    {
        /* Exit if printer faults or memory errors */

        if ( T30RxCheckErrors() )
            break;

        OSC_Sleep( 100 );
    }

    T30Control->Direction = T30_TRANSMIT;
}


/*===========================================================================
*
*    T30RxProcessNSC
*
*      Processes a NSC received during receive Phase B
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessNSC( void )
{
	TRACE_FAX( "T30Rx: ( NSC )\n" );

	if ( T30Control->NSTDFlg )
		T30RxProcessDTC();
	else
		T30RxProcessBadCommand();
}


/*===========================================================================
*
*    T30RxProcessNSS
*
*      Processes a NSS received during receive Phase B
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessNSS( void )
{
	TRACE_FAX( "T30Rx: ( NSS )\n" );

	if ( T30Control->NSTDFlg )
		T30RxProcessDCS();
	else
		T30RxProcessBadCommand();
}


/*===========================================================================
*
*    T30RxProcessMPS
*
*      Processes a MPS received during receive Phase D
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessMPS( void )
{
	UINT8 fcf;

	TRACE_FAX( "T30Rx: ( MPS )\n" );

	if ( ( T30Control->LastFCFSent == MCF )
	  || ( T30Control->LastFCFSent == RTN ) )
	{
		fcf = T30Control->LastFCFSent;
	}
	else
	{
		/* Don't form feed if receiving to memory */
		if ( !T30Control->RxToMemFlg )
			;

		/* Form feed ? */

		/* Check the copy quality - returns MCF or RTN */
		fcf = T30RxCheckCopyQuality();

		/* If voice request, Convert to PIP or PIN */
		if ( t30_attribute.FaxVoiceRequest
		  && !t30_attribute.IgnoreVoiceRequest )
		{
			fcf = ( fcf | 0x20 ) & 0xBF;
		}
	}

	T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xEE;
	T30SendFCF( fcf );
}


/*===========================================================================
*
*    T30RxProcessEOM
*
*      Processes a EOM received during receive Phase D
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessEOM( void )
{
	UINT8 fcf;

	TRACE_FAX( "T30Rx: ( EOM )\n" );

	if ( ( T30Control->LastFCFSent == MCF )
	  || ( T30Control->LastFCFSent == RTN ) )
	{
		fcf = T30Control->LastFCFSent;
	}
	else
	{
		/* Check the copy quality - returns MCF or RTN */
		fcf = T30RxCheckCopyQuality();

		/* If voice request, Convert to PIP or PIN */
		if ( t30_attribute.FaxVoiceRequest
		  && !t30_attribute.IgnoreVoiceRequest )
		{
			fcf = ( fcf | 0x20 ) & 0xBF;
		}
	}

	T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xEE;
	T30SendFCF( fcf );
}


/*===========================================================================
*
*    T30RxProcessEOP
*
*      Processes a EOP received during receive Phase D
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessEOP( void )
{
	UINT8 fcf;

	TRACE_FAX( "T30Rx: ( EOP )\n" );

	if ( ( T30Control->LastFCFSent == MCF )
	  || ( T30Control->LastFCFSent == RTP )
	  || ( T30Control->LastFCFSent == RTN )
	  || ( ( T30Control->LastFCFSent != 0xFF )
	    && ( T30Control->LastFCFSent &  0x20 ) ) )
	{
		fcf = T30Control->LastFCFSent;
	}
	else
	{
		if ( ECMControl )
		{
			fcf = MCF;
		}
		else
		{
			/* Check the copy quality - returns MCF or RTN */
			fcf = T30RxCheckCopyQuality();
		}

		/* If voice request, Convert to PIP or PIN */
		if ( t30_attribute.FaxVoiceRequest
		  && !t30_attribute.IgnoreVoiceRequest )
		{
			fcf = ( fcf | 0x20 ) & 0xBF;
		}
	}

	T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xEE;
	T30SendFCF( fcf );
}


/*===========================================================================
*
*    T30RxProcessPRIQ
*
*      Processes a PRIQ received during receive Phase D
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessPRIQ( void )
{
	UINT8 fcf;

	TRACE_FAX( "T30Rx: ( PRIQ )\n" );

	T30Control->PRIQCnt--;

	T30AlertLocalOp();

	/* Quit if we initiated or line request or both */
	if ( t30_attribute.FaxLineRequest || t30_attribute.FaxVoiceRequest )
	{
		TRACE_FAX( "T30Rx: PRIQ linereq or voice req\n" );

		if ( t30_attribute.FaxLineRequest )
		{
			/* Send MCF or RTN to transmitter */
			fcf = T30RxCheckCopyQuality();
			fcf = ( fcf | 0x20 ) & 0xBF;
			T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xEE;
			T30SendFCF( fcf );
		}

		T30Control->State = STATE_RX_PHASE_E;
	}
	else
	{
		TRACE_FAX( "T30Rx: PRIQ no linereq or voice req, PRIQCnt %d\n",
		       T30Control->PRIQCnt );

		/* Check for 3 PRI-Q's or ignoring voice requests */
		if ( !T30Control->PRIQCnt || t30_attribute.IgnoreVoiceRequest )
		{
			/* Strip out the PRI-Q bit, initialize PRI-Q counter, and process like a non-PRI-Q */
			T30Control->PRIQCnt = 3;
			T30Control->State = STATE_RX_PHASE_D_SIGNAL_REC;

			switch ( T30Control->LastFCFRcvd & 0xEE )
			{
				case EOM :
					T30RxProcessEOM();
					break;
				case MPS :
					T30RxProcessMPS();
					break;
				case EOP :
					T30RxProcessEOP();
					break;
				default:
					T30RxProcessBadCommand();
					break;
			}
		}
	}
}


/*===========================================================================
*
*    T30RxProcessPWD
*
*      Processes a PWD received during receive Phase B
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void T30RxProcessPWD( void )
{
	TRACE_FAX( "T30Rx: ( PWD )\n" );

#if 0
	if ( T30Control->ECMEnabled )
		ECMRxPrintRemainingBlocks();

#endif
}


void T30RxProcessSID( void )
{
	TRACE_FAX( "T30Rx: ( SID )\n" );
}


void T30RxProcessSUB( void )
{
	TRACE_FAX( "T30Rx: ( SUB )\n" );
}


void T30RxProcessSEP( void )
{
	TRACE_FAX( "T30Rx: ( SEP )\n" );
}


/*===========================================================================
*
*    T30RxPhaseBDProcessCommand
*
*      Processes a received T.30 command during receive Phase B or D
*
*      Input:          T30Control->LastFCFRcvd = FCF of command received
*                      T30Control->LastFCFSent = last FCF we sent
*      Output:         T30Control->State = STATE_RX_SIGNAL_REC if MPS, EOM, EOP, or DCS received
*                      T30Control->Status = BAD_RSP_ERROR if anything else received
*                      T30Control->TxPollFlg = TRUE if we're going to TX by polling
*
*===========================================================================*/
void T30RxPhaseBDProcessCommand( void )
{
	UINT8 i;

	/* Determine the next state, then process the command */
	/* because the next state may change during command processing */

	for ( i = 0; i < RxCmdTableSize; i++ )
		if ( T30Control->LastFCFRcvd == RxCmdTable[ i ].command )
			break;

	// in case of PPR save FCF2 for getting MPS/EOM/EOP
	if ( ( T30Control->LastFCFRcvd & 0xFE ) == PPS )
	{
		T30Control->LastFCFRcvd = ECMControl->PPS_FIF[ 0 ];
		T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xFE;
		TRACE_FAX( "T30RxPhaseBDProcessCommand: PPS-%02X\n", T30Control->LastFCFRcvd );
	}
	else
	{
		TRACE_FAX( "T30RxPhaseBDProcessCommand: FCF = 0x%02X\n", T30Control->LastFCFRcvd );

		if ( ( T30Control->LastFCFRcvd & 0xFE ) == DCN )
		{
			T30Control->State = STATE_RX_PHASE_E;
			CxProtocolReport( DCN, &T30Control->Status, 1, RX_LOG );
		}
	}

	/* Anything not in the table is a bad command */
	if ( i == RxCmdTableSize )
		i = 0;

	T30Control->State = RxCmdTable[ i ].next_state;

	( *RxCmdTable[ i ].fxn ) ();
}


/*===========================================================================
*
*    T30RxCheckMultiPage
*      Check received post message to see there is another page to receive.
*      In case of EOM, wait until DCS processing is finished to know the
*      resolution changed.
*
*
*      Input:       None
*      Output:      TRUE: multi page
*                   FALSE: no furhter page to receive
*
*===========================================================================*/
BOOL T30RxCheckMultiPage( UINT8 *post_msg )
{
	BOOL ret_val;

	TRACE_FAX( "T30RxCheckMultiPage: 0x%x\n", T30Control->rcvd_post_msg );

	if ( !T30Control || T30Control->Status )    // fax module already finished or error, then it is the last
		return FALSE;

	/* wait until correct post message is received */

	while ( T30Control->rcvd_post_msg == 0xff )
	{
		if ( !T30Control || T30Control->Status )    // fax module already finished or error, then it is the last
			return FALSE;

		OSC_Sleep( 100 );
	}

	switch ( T30Control->rcvd_post_msg )
	{
		case EOM:

			TRACE_FAX( " T30RxCheckMultiPage: Received EOM .. %02X\n", T30Control->rcvd_post_msg );

			*post_msg = EOM;
			ret_val = TRUE;
			break;

		case MPS:

			TRACE_FAX( " T30RxCheckMultiPage: Received MPS .. %02X\n", T30Control->rcvd_post_msg );

			*post_msg = MPS;
			ret_val = TRUE;
			break;

		case EOP:

			TRACE_FAX( " T30RxCheckMultiPage: Received EOP .. %02X\n", T30Control->rcvd_post_msg );

			*post_msg = EOP;
			ret_val = FALSE;
			break;

		case NUL:        // PPS_NULL: this case doesn't reach here THY 8/28/06

			TRACE_FAX( " T30RxCheckMultiPage: Received PPS_NULL .. %02X\n", T30Control->rcvd_post_msg );

			*post_msg = 0xFE;           // PPS_NULL
			ret_val = TRUE;
			break;

		default:

			TRACE_FAX( " T30RxCheckMultiPage: Unknown .. %02X\n", T30Control->rcvd_post_msg );

			*post_msg = 0xFF;
			ret_val = FALSE;
			break;
	}

	T30Control->rcvd_post_msg = 0xff;
	return ( ret_val );
}

