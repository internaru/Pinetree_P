/*
 *  $RCSfile: t30_tx.c,v $
 * $Revision: 1.60 $
 *   $Author: miurat $
 *     $Date: 2011/12/17 02:33:36 $
 */
/************************************************************************
 *
 *      t30_tx.c
 *
 *  T.30 Transmit State Machine Functions
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
#include "ERROR.H"
#include "BUFFERS.H"
#include "MACROS.H"
#include "MODEMX.H"
#include "T30X.H"
#include "HDLC.H"
#include "ECMX.H"
#include "ECM.H"
#include "t30.h"
#include "fsu.h"
#include "../../faxapp/include/faxapp_scan.h"
#include <unistd.h>
#include "../../faxapp/include/faxapp_notify.h"

#if __ONHOOK_TX_WAITING__
extern BOOLEAN fOnHookScanComplete;
#endif /* __ONHOOK_TX_WAITING__ */

extern void ModemTxDataStop(void);
extern UINT16 ECMCheckV34FallBack( void );
extern void faxapp_set_event_flags(int Event);
extern void fsu_tx_image_process_begin (void);
extern void fsu_tx_update_page(void);
extern BOOLEAN fsu_check_1st_image_file_in_mem (UINT16 jnum, UINT8 FileType);
extern UINT8 fsu_get_resolution(UINT16 jnum);
extern UINT8 fsu_get_width(UINT16 jnum);
extern UINT8 fsu_get_total_pages(UINT16 jnum);
extern UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
extern UINT8 fsu_get_photo_mode (UINT16 jnum);
extern UINT8 fsu_request_get_total_page_no_from_Job_Queue_File (UINT16 DocId);
extern UINT16 fsu_get_last_page_no_in_mem (UINT16 DocId, UINT8 FileType, UINT8 Mode);
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern int fsu_get_scan_busy_flag (void);

extern UINT8 RTNCount;
extern UINT8 isReadyToTransmit;
extern UINT8 isTxImageProcessDone;
extern UINT8 gFaxOnHookScanSource;
extern UINT16 fsu_job_num_send;
extern UINT16 fsu_job_num_rcv;
extern UINT32 gFaxT1Timeout;
extern UINT32 last_EOL_time;
extern UINT32 gFaxCEDDetectionTime;
extern int gFaxAbnormalFlag;
extern char *pRootFolderName;

#if __MEASURE_PERFORMANCE__
extern void faxapp_get_job_start_time(int JobType);
extern void faxapp_get_job_end_time(int JobType);
extern char *faxapp_get_elapsed_time(int JobType);
#endif // #if __MEASURE_PERFORMANCE__


/*===========================================================================
*
*    T30TxCEDVerified
*
*      Query to inform others whether CED has been verified from the remote
*
*      Input:          (none)
*      Output:         TRUE (1) or FALSE (0)
*
*===========================================================================*/
UINT8 T30TxCEDVerified( void )
{
	if ( T30Control )
    {
		return ( T30Control->CalledStationDetected );
    }
	else
    {
		return ( 0 );
    }
}


/*===========================================================================
*
*    T30TxFallback
*
*      Tries to fall back in speed
*
*      Input:          T30Control->ModemSpeed = old modem speed table index
*                      T30Control->Try2400 = number of times to try 2400 bps
*      Output:         T30Control->ModemSpeed = new modem speed table index
*                      Returns 0 if we've tried 2400 bps and T30Control->Try2400 decrements to 0
*                      Returns 1 otherwise
*
*===========================================================================*/
UINT8 T30TxFallback( void )
{
	if ( ( T30Control->V34Mode ) && ( T30Control->State == STATE_TX_PHASE_D2 ) )
	{
		TRACE_FAX( "ECMControl->PPRCnt %d\n", ECMControl->PPRCnt );
#if 0 /* avoid infinite fallback loop */
		ECMCheckV34FallBack();
#else
		if( ModemV34FallSpeed() == 0 )
			return ( 0 );
#endif
		return ( 1 );
	}
	else
	{
		if ( T30Control->ModemSpeed == T30_SPEED_V27_2400 )
		{
			if ( T30Control->Try2400 > 0 )
			{
				T30Control->Try2400--;
				return ( 1 );
			}
			else
			{
				return ( 0 );
			}
		}
		else
		{
			/* Test whether the speed should be retried first */
			T30Control->ModemSpeedTryCnt++;

			if ( T30Control->ModemSpeedTryCnt >= t30_attribute.NoOfTriesOnFallback )
			{
				if ( T30Control->ModemSpeed == T30_SPEED_V17_7200 )
					T30Control->ModemSpeed = FallbackTbl[ T30Control->ModemSpeed - 2 ];  // skip V.29
				else
					T30Control->ModemSpeed = FallbackTbl[ T30Control->ModemSpeed ];
				T30Control->ModemSpeedTryCnt = 0;
			}

			/* when remote cap has only V.29 and send FTT, the fall back should be finished at 7200bps.*/
			if ( ( ( T30Control->RemoteCap[ 1 ] & 0x3C ) >> 2 ) == 0x01 )  // if V.29 only in remote cap.
			{
				if ( T30Control->ModemSpeed <= T30_SPEED_V27_4800 )   // if cuurent fallback speed is V.27
					return ( 0 );
			}

			TRACE_FAX( "T30TxFallback(): %d\n", T30Control->ModemSpeed );
		}
	}

	return ( 1 );
}


/*===========================================================================
*
*    T30TxSetDCSCoding
*
*      Sets the DCS coding scheme
*
*      Input:          FaxCxJob = compressor job number
*                      T30Control->RemoteCap+1 = remote machine's capabilities
*                      t30_attribute.FaxOptions says whether to ignore MR coding
*      Output:         T30Control->LocalCap+1 coding scheme bits set
*                      T4Coding,X set
*
*===========================================================================*/
void T30TxSetDCSCoding( void )
{
	/* If remote is not MR capable, set up for MH coding */

    if ( BITCLEAR( &T30Control->RemoteCap[ 1 ], 0x80 ) || t30_attribute.FaxOptions.IgnoreMR )
	{
		CLEARBIT( &T30Control->LocalCap[ 1 ], 0x80 );
		T30Control->T4Params.Tx.Coding = CODING_MH;
	}

	/* Otherwise set up for MR coding */
	else
	{
		SETBIT( &T30Control->LocalCap[ 1 ], 0x80 );
		T30Control->T4Params.Tx.Coding = CODING_MR;
	}

	TRACE_FAX( "T30TxSetDCSCoding(): %d\n", T30Control->T4Params.Tx.Coding );
}


/*===========================================================================
*
*    T30TxSetDCSECM
*
*      Sets the DCS ECM bit and checks for MMR mode
*
*      Input:          t30_attribute.FaxOptions has the "ignore ECM" and "ignore MMR" bits
*                      T30Control->RemoteCap+3 = remote machine's capabilities
*      Output:         T30Control->LocalCap+3 has ECM bit
*                      T30Control->LocalCap+2 set for 0 msec lines if ECM
*                      T4MinBytes = 0 if ECM
*                      ECMStatus LSB = 1 if ECM
*                      T4Coding = MMR if MMR mode
*
*===========================================================================*/
void T30TxSetDCSECM( void )
{
	UINT8 ecm_mode = ECM_NONE;

	/* Set up for ECM if remote can do it and not ignoring ECM */

    if ( ( t30_attribute.ECMMode != ECM_NONE ) && BITSET( &T30Control->RemoteCap[ 3 ], 0x04 ) )
	{
		/* Only set up for ECM mode if memory is available */

		if ( ECMInitJob( t30_attribute.ECMMode ) == E_OK )
			ecm_mode = t30_attribute.ECMMode;

		/* Otherwise check whether memory is available
		   for a lower frame size */
		else if ( ( t30_attribute.ECMMode != ECM_64 )
		       && ( ECMInitJob( ECM_64 ) == E_OK ) )
			ecm_mode = ECM_64;
	}

	if ( ecm_mode )
	{
		/* Set the DCS ECM mode bit */
		SETBIT( &T30Control->LocalCap[ 3 ], 0x04 );

		if ( ecm_mode == ECM_64 )
		{
			SETBIT( &T30Control->LocalCap[ 3 ], 0x08 );
			ECMControl->BytesPerFrame = 64;
		}
		else
		{
			ECMControl->BytesPerFrame = 256;
		}

		T30Control->T4Params.Tx.ECMMode = ecm_mode;

		/* Set to 0 msec min scan time */
		SETBIT( &T30Control->LocalCap[ 2 ], 0x70 );

		/* Set min line byte count */
		T30Control->T4Params.Tx.MinBytes = 0;

		/* JBIG coding */
        if ( !t30_attribute.FaxOptions.IgnoreJBIG && BITSET( &T30Control->RemoteCap[ 9 ], 0x20) )
		{
			CLEARBIT( &T30Control->LocalCap[ 1 ], 0x80 );
			CLEARBIT( &T30Control->LocalCap[ 3 ], 0x40 );
			CLEARBIT( &T30Control->LocalCap[ 8 ], 0x18 );

            if ( !t30_attribute.FaxOptions.IgnoreJBIGL0 && BITSET( &T30Control->RemoteCap[ 9 ], 0x40) )
			{
				SETBIT( &T30Control->LocalCap[ 9 ], 0x60 );
			}
			else
            {
				SETBIT( &T30Control->LocalCap[ 9 ], 0x20 );
            }

			T30Control->T4Params.Tx.Coding = CODING_JBIG;
		}
		/* If remote is color and JPEG capable AND we're set up to do a */
		/* color/grayscale scan, set up for JPEG coding */
        else if ( !t30_attribute.FaxOptions.IgnoreJPEG && BITSET( &T30Control->RemoteCap[ 8 ], 0x08 ) && ( ( T30Control->T4Params.Tx.ImageType == IMG_GRAYSCALE ) || ( T30Control->T4Params.Tx.ImageType == IMG_COLOR ) ) )
		{
			CLEARBIT( &T30Control->LocalCap[ 1 ], 0x80 );
			CLEARBIT( &T30Control->LocalCap[ 3 ], 0x40 );

            if ( ( T30Control->T4Params.Tx.ImageType == IMG_GRAYSCALE ) || t30_attribute.FaxOptions.IgnoreFullColor || BITCLEAR( &T30Control->RemoteCap[ 8 ], 0x10 ) )
			{
				SETBIT( &T30Control->LocalCap[ 8 ], 0x08 );
				T30Control->T4Params.Tx.Coding = CODING_JPEG_GRAYSCALE;
				TRACE_FAX("T30TxSetDCSECM: ========== >>> set CODING_JPEG_GRAYSCALE\n");
			}
			else if ( T30Control->T4Params.Tx.ImageType == IMG_COLOR )
			{
				SETBIT( &T30Control->LocalCap[ 8 ], 0x18 );
				T30Control->T4Params.Tx.Coding = CODING_JPEG_COLOR;
				TRACE_FAX("T30TxSetDCSECM: ========== >>> set CODING_JPEG_COLOR\n");
			}

			T30Control->T4Params.Tx.Res = RES_FINE;
		}
		/* Set up for MMR if remote can do it and not ignoring MMR */
        else if ( !t30_attribute.FaxOptions.IgnoreMMR && BITSET( &T30Control->RemoteCap[ 3 ], 0x40 ) )
		{
			CLEARBIT( &T30Control->LocalCap[ 1 ], 0x80 );
			SETBIT( &T30Control->LocalCap[ 3 ], 0x40 );
			T30Control->T4Params.Tx.Coding = CODING_MMR;
		}
	}

	if(!ecm_mode)
	{
		T30Control->ECMEnabled = 0;
		T30Control->T4Params.Tx.ECMMode = ECM_NONE;
	}

    TRACE_FAX( "T30TxSetDCSECM(): ecm %d coding %d\n", T30Control->T4Params.Tx.ECMMode, T30Control->T4Params.Tx.Coding );
}


/*===========================================================================
*
*    T30TxSetDCSLength
*
*      Sets DCS page length.  Uses the remote machine's length, and sets
*      the T4 length variable appropriately.
*
*      Input:          T30Control->RemoteCap+2 = remote machine's capabilities
*      Output:         T30Control->LocalCap+2 = remote machine's length, A4 width
*
*===========================================================================*/
void T30TxSetDCSLength( void )
{
	/* Assume A4 length, width - 1728 bits wide and whatever the remote can do */
	T30Control->LocalCap[ 2 ] = T30Control->RemoteCap[ 2 ] & 0x0C;

	if ( T30Control->LocalCap[ 2 ] )

		/* Not A4 length */
		if ( T30Control->RemoteCap[ 2 ] & 0x08 )
			T30Control->T4Params.Tx.PageLen = LEN_UNLTD;
		else
			T30Control->T4Params.Tx.PageLen = LEN_B4;
	else
		T30Control->T4Params.Tx.PageLen = LEN_A4;

#if 0
	TRACE_FAX( "T30TxSetDCSLength(): %d\n", T30Control->T4Params.Tx.PageLen );
#endif
}


/*===========================================================================
*
*    T30TxSetDCSRes
*
*      Sets DCS resolution
*
*      Input:          T4Res = current resolution
*                      T30Control->RemoteCap+1 = remote machine's capabilities
*      Output:         T4Res = resolution
*                      T30Control->LocalCap+1, +5 resolution bits set
*
*===========================================================================*/
void T30TxSetDCSRes( void )
{
	/* Save our resolution as the original resolution */
	T30Control->OrigRes = T30Control->T4Params.Tx.Res;

	/* Is local setting standard or
	   is standard the best the remote can do ? */

	/* If in ultra fine 400x400 mode and the remote can handle ultrafine */
    if ( ( T30Control->T4Params.Tx.Res == RES_ULTRAFINE ) && BITCLEAR( &T30Control->RemoteCap[ 5 ], 0x04 ) )
	{
		T30Control->T4Params.Tx.Res = RES_300;
	}

	/* If in 300x300 and the remote can handle 300x300 ? */
    if ( ( T30Control->T4Params.Tx.Res == RES_300 ) && BITCLEAR( &T30Control->RemoteCap[ 5 ], 0x02 ) )
#ifdef RES_SUPERFINE
	{
		T30Control->T4Params.Tx.Res = RES_SUPERFINE;
	}

	/* If in super fine mode and the remote can handle superfine */
    if ( ( T30Control->T4Params.Tx.Res == RES_SUPERFINE ) && BITCLEAR( &T30Control->RemoteCap[ 5 ], 0x01 ) )
#endif
	{
		T30Control->T4Params.Tx.Res = RES_FINE;
	}

	/* If in fine mode and the remote can handle fine */
    if ( ( T30Control->T4Params.Tx.Res == RES_FINE ) && BITCLEAR( &T30Control->RemoteCap[ 1 ], 0x40 ) )
	{
		T30Control->T4Params.Tx.Res = RES_STANDARD;
	}


	/* Now set the selected resolution */
	if ( T30Control->T4Params.Tx.Res == RES_ULTRAFINE )
	{
		SETBIT( &T30Control->LocalCap[ 5 ], 0x04 );
	}
	else if ( T30Control->T4Params.Tx.Res == RES_300 )
	{
		SETBIT( &T30Control->LocalCap[ 5 ], 0x0A );
	}
#ifdef RES_SUPERFINE
	else if ( T30Control->T4Params.Tx.Res == RES_SUPERFINE )
	{
		SETBIT( &T30Control->LocalCap[ 5 ], 0x01 );
	}
#endif
	else if ( T30Control->T4Params.Tx.Res == RES_FINE )
	{
		SETBIT( &T30Control->LocalCap[ 1 ], 0x40 );
	}

	TRACE_FAX( "T30TxSetDCSRes(): %d\n", T30Control->T4Params.Tx.Res );
}


/*===========================================================================
*
*    T30TxSetDCSScanTime
*
*      Sets DCS scan time
*
*      Input:          T30Control->RemoteCap+2 = remote machine's capabilities
*                      T4Res = image resolution
*                      T30Control->ModemSpeed = modem speed
*      Output:         T30Control->LocalCap+2 scan time bits set
*                      T4MinBytes = minimum number of bytes per line
*
*===========================================================================*/
void T30TxSetDCSScanTime( void )
{
	UINT8 index;

	/* Convert to a table index */
	index = ( T30Control->RemoteCap[ 2 ] & 0x70 ) >> 3;

	/* If we're in fine, superfine, or halftone, bump the index */

	if ( T30Control->T4Params.Tx.Res )
		index++;

	/* Set the extended field bit */
	T30Control->LocalCap[ 2 ] |= ( ScanTimeTbl[ index ] | 0x80 );

	/* Set the minimum number of bytes.  Mask out all but the minimum scan time. OR the speed back in. */
	/* Setup temporary value since this will be reset later as V34 requires ECM */
	if ( T30Control->V34Mode )
		index = T30_SPEED_V17_14400;
	else
		index = T30Control->ModemSpeed;

	T30Control->T4Params.Tx.MinBytes = T4MinBytesTbl[ ( T30Control->LocalCap[ 2 ] & 0x70 ) | index ];

#if 0
	TRACE_FAX( "T30TxSetDCSScanTime(): %d\n", T30Control->T4Params.Tx.MinBytes );
#endif
}


/*===========================================================================
*
*    T30TxSetDCSSpeed
*
*      Sets DCS speed bits
*
*      Input:          T30Control->RemoteCap+1 = remote machine's capabilities
*                      T30Control->ModemSpeed = our fastest speed
*      Output:         T30Control->LocalCap+1 speed bits set
*                      T30Control->ModemSpeed set
*
*===========================================================================*/
void T30TxSetDCSSpeed( void )
{

	UINT8 index;

#if 0
	TRACE_FAX( "T30TxSetDCSSpeed(): RemoteCap[1] %x\n",
	       T30Control->RemoteCap[ 1 ] );
#endif

	if ( T30Control->V34Mode )
	{
		/* Set the T.4 receiver bit */
		T30Control->LocalCap[ 1 ] = 0x02;
		return ;
	}

	/* See what speed the remote RX can do */
	index = ( T30Control->RemoteCap[ 1 ] & 0x3C ) >> 2;

	TRACE_FAX( "T30TxSetDCSSpeed(): index %d ModemSpeed %d\n",
	       index, T30Control->ModemSpeed );

	/* Use whichever speed is slower */
	if ( T30Control->ModemSpeed > SpeedIndexTbl[ index ] )
		T30Control->ModemSpeed = SpeedIndexTbl[ index ];

	/* Set the T.4 receiver bit */
	T30Control->LocalCap[ 1 ] = DCS_SpeedBits[ T30Control->ModemSpeed ] | 0x02;

#if 0
	TRACE_FAX( "T30TxSetDCSSpeed(): DCS[1] %x new ModemSpeed %d\n",
	       T30Control->LocalCap[ 1 ], T30Control->ModemSpeed );
#endif
}


/*===========================================================================
*
*    T30TxSetDCSWidth
*
*      Sets DCS page width.  If remote can do B4 width, checks
*      to see whether we have a A4 or B4 document to send, and sets the DCS
*      bit appropriately.
*
*      Input:          T30Control->LocalCap+2 set for A4 width
*                      T30Control->RemoteCap+2 = remote machines capabilities
*      Output:         T30Control->LocalCap+2 width bits set
*                      T4PageWidth set
*
*===========================================================================*/
void T30TxSetDCSWidth( void )
{
    /* Save a copy for Phase D. */
	T30Control->OrigWidth = T30Control->T4Params.Tx.Width;

	/* See if remote can do A3 width */
	if ( T30Control->T4Params.Tx.Width == WIDTH_A3 )
	{
		if ( ( T30Control->RemoteCap[ 2 ] & 0x03 ) != 0x02 )
		{
			//if not .. send with A4
            T30Control->T4Params.Tx.Width = WIDTH_A4;
		}
	}
	/* See if remote can do B4 width */
    else if ( T30Control->T4Params.Tx.Width == WIDTH_B4 )
	{
        if ( (( T30Control->RemoteCap[ 2 ] & 0x03 ) == 0x00) || ((T30Control->RemoteCap[ 2 ] & 0x03 ) == 0x03) )
		{
			//if not .. send with A4
			T30Control->T4Params.Tx.Width = WIDTH_A4;
		}
	}

    printf("\n(YHCHO) %s() - T30Control->T4Params.Tx.Width: %d\n", __FUNCTION__, T30Control->T4Params.Tx.Width);

	/* Set to A4 paper by default */
	CLEARBIT( &T30Control->LocalCap[ 2 ], 0x03 );

	T30Control->T4Params.Tx.PaperType = PAPSIZE_A4;

	/* And check if it should be A3 */
	if ( T30Control->T4Params.Tx.Width == WIDTH_A3 )
	{
		/* If so, set the DCS bits to (0,1) */
		SETBIT( &T30Control->LocalCap[ 2 ], 2 );
		CLEARBIT( &T30Control->LocalCap[ 2 ], 1 );
		T30Control->T4Params.Tx.PaperType = PAPSIZE_A3;
	}
	/* And check if it should be B4 */
    else if ( T30Control->T4Params.Tx.Width == WIDTH_B4 )
	{
		/* If so, set the DCS bits to (1,0) */
		SETBIT( &T30Control->LocalCap[ 2 ], 1 );
		CLEARBIT( &T30Control->LocalCap[ 2 ], 2 );
		T30Control->T4Params.Tx.PaperType = PAPSIZE_B4;
	}
}


/*===========================================================================
*
*    T30TxMakeDCS
*
*      Makes the DCS FIF according to the received DIS or DTC
*
*      Input:          T30Control->RemoteCap = received DIS or DTC FIF
*                      T30Control->ModemSpeed = max. transmitter speed
*                      T30Control->NSTDFlg = TRUE if Fax Engine NSF received
*      Output:         T30Control->LocalCap = DCS to send, if standard mode
*                      LocalNSTD = NSS to send, if non-standard mode
*                      T30Control->ModemSpeed = modem speed table index
*                      T4PageWidth = page width
*                      T4PageLen = page length
*                      T4Res = fax resolution
*                      T4MinBytes = min. number of bytes per line to transmit, if NSTD
*
*===========================================================================*/
void T30TxMakeDCS( void )
{
    UINT8 res = 0;
    UINT8 i = 0;
    UINT16 jnum = fsu_job_num_send;

    printf("\n(YHCHO) %s\n", __FUNCTION__);

	T30Control->LocalCap[ 0 ] = 0;

	for ( i = 3; i < LocalDISSize; ++i )
		T30Control->LocalCap[ i ] = 0x80;

	T30Control->LocalCap[ LocalDISSize - 1 ] = 0;

    if ( T30Control->TxFromMemFlg )
	{
        /* Resolution */
        res = fsu_get_resolution(jnum);

        switch ( res )
        {
            case 4:
                T30Control->T4Params.Tx.Res = RES_ULTRAFINE;
                break;

            case 3:
                T30Control->T4Params.Tx.Res = RES_300;
                break;

            case 1:
                T30Control->T4Params.Tx.Res = RES_FINE;
                break;

            case 0:
            default:
                T30Control->T4Params.Tx.Res = RES_STANDARD;
                break;
        }

        /* And the width */
        T30Control->T4Params.Tx.Width = fsu_get_width(jnum);

        //TRACE_FAX( "T30TxMakeDCS T30Control->T4Params.Tx.Res: %d, T30Control->T4Params.Tx.Width: %d\n", T30Control->T4Params.Tx.Res, T30Control->T4Params.Tx.Width );
	}

	/* Otherwise, use the user settings */
	else
	{
        T30Control->T4Params.Tx.Width = t30_attribute.FaxTxImageWidth;
		T30Control->T4Params.Tx.Res = t30_attribute.FaxTxResolution;

        //TRACE_FAX( "T30TxMakeDCS Scan Res %d, Width %d\n", T30Control->T4Params.Tx.Res, T30Control->T4Params.Tx.Width );
	}

	/* Set the transmission speed */
	T30TxSetDCSSpeed();

	/* Set the resolution */
	T30TxSetDCSRes();

	/* Set the coding scheme */
	T30TxSetDCSCoding();

	/* Set the page length */
	T30TxSetDCSLength();

	/* Set the page width */
	T30TxSetDCSWidth();

	/* Check for North American paper type handling and
	   change setup as necessary */
	if ( ( T30Control->T4Params.Tx.Width == WIDTH_A4 )
	  && t30_attribute.HandleNAPaperSizes
	  && T30Control->RemoteCap[ 9 ] & 0x18 )
	{
		UINT8 paper_type;
		UINT16 pagelen;

		if ( T30Control->T4Params.Tx.SendingCoverPage || T30Control->TxFromMemFlg )
			;
		else
			/* Length not known */
			pagelen = ( UINT16 ) - 1;

		/* NNN - Find out paper type since this was not saved
		   as part of the file */
		paper_type = PAPSIZE_A4;

		res = T30Control->T4Params.Tx.Res;

		if ( ( ( res == RES_STANDARD )  && ( pagelen >= 1038 ) && ( pagelen <= 1078 ) )
		  || ( ( res == RES_FINE )      && ( pagelen >= 2076 ) && ( pagelen <= 2156 ) )
#ifdef RES_SUPERFINE
		  || ( ( res == RES_SUPERFINE ) && ( pagelen >= 4152 ) && ( pagelen <= 4301 ) )
#endif
		  || ( ( res == RES_300 )       && ( pagelen >= 3090 ) && ( pagelen <= 3300 ) ) )
			paper_type = PAPSIZE_LETTER;

		if ( ( ( res == RES_STANDARD )  && ( pagelen >= 1332 ) && ( pagelen <= 1372 ) )
		  || ( ( res == RES_FINE )      && ( pagelen >= 2664 ) && ( pagelen <= 2744 ) )
#ifdef RES_SUPERFINE
		  || ( ( res == RES_SUPERFINE ) && ( pagelen >= 5314 ) && ( pagelen <= 5474 ) )
#endif
		  || ( ( res == RES_300 )       && ( pagelen >= 3990 ) && ( pagelen <= 4200 ) ) )
			paper_type = PAPSIZE_LEGAL;

		/* Set capability if required */
		if ( paper_type != PAPSIZE_A4 )
		{
			if ( ( T30Control->RemoteCap[ 9 ] & 0x08 )
			  && ( paper_type == PAPSIZE_LETTER ) )
			{
				T30Control->LocalCap[ 9 ] |= 0x08;
				T30Control->T4Params.Tx.PageLen = LEN_LETTER;
				T30Control->T4Params.Tx.PaperType = PAPSIZE_LETTER;
			}
			else if ( ( T30Control->RemoteCap[ 9 ] & 0x10 )
			       && ( paper_type == PAPSIZE_LEGAL ) )
			{
				T30Control->LocalCap[ 9 ] |= 0x10;
				T30Control->T4Params.Tx.PageLen = LEN_LEGAL;
				T30Control->T4Params.Tx.PaperType = PAPSIZE_LEGAL;
			}
		}
	}

	/* Password */
	if ( ( t30_attribute.TxRequirePassword )
	  && ( t30_attribute.FaxLocalTxPassword[ 0 ] != 0 ) )
	{
		if ( BITSET( &T30Control->RemoteCap[ 6 ], 0x02 ) )
		{
			FIFSendTable[ SIDtbl ] = t30_attribute.FaxLocalTxPassword;
			SETBIT( &T30Control->LocalCap[ 6 ], 0x02 );
		}
	}

	/* Subaddress */
	if ( ( t30_attribute.TxRequireSubaddress )
	  && ( t30_attribute.FaxLocalTxSubaddress[ 0 ] != 0 ) )
	{
		if ( BITSET( &T30Control->RemoteCap[ 6 ], 0x01 ) )
		{
			FIFSendTable[ SUBtbl ] = t30_attribute.FaxLocalTxSubaddress;
			SETBIT( &T30Control->LocalCap[ 6 ], 0x01 );
		}
	}

	/* Set the scan time */
	T30TxSetDCSScanTime();

	/* Set the ECM and MMR bits */
	T30TxSetDCSECM();

	/* Copy DCS into NSS */
	if ( t30_attribute.FaxEnableNSF )
		memcpy( T30Control->LocalNSTDCap, T30Control->LocalCap, LocalDISSize );
}


/*===========================================================================
*
*    T30TxSetMode
*
*      T.30 Transmit Phase B pre-message procedure Part 2
*      Performs the SET MODE function.  Sends TSI/DCS/TCF, and gets
*      the called station's response.
*
*      Input:          LocalID =  TSI FIF
*                      T30Control->LocalCap = DCS FIF
*                      LocalNSTD = NSS FIF
*                      T30Control->NSTDFlg = TRUE if Fax Engine NSF received
*      Output:         T30Control->ModemSpeed = speed we get a CFR at, if any
*                      If we get a CFR, T30Control->State = STATE_TX_PHASE_C
*                      Else, T30Control->Status = BAD_RSP_ERROR
*
*===========================================================================*/
void T30TxSetMode( void )
{
	UINT8 fDone = 0;
	UINT8 rsp;
	UINT8 i;
    UINT8 isT30ErrorDetected = 0;

    printf("\n(YHCHO) %s()\n", __FUNCTION__);

	/* Find the first byte in the DIS that indicates there
	   are no more extended bytes available */

	for ( i = 2; i < RemoteCapSize; ++i )
		if ( !( T30Control->RemoteCap[ i ] & 0x80 ) )
			break;

	T30Control->DIS_size = i + 1;

	/* And zero out all the following bytes of DIS */
	for ( ++i; i < RemoteCapSize; ++i )
		T30Control->RemoteCap[ i ] = 0;

	TRACE_FAXDEBUG(
	    "T30Tx DIS: %x %x %x %x %x %x %x %x %x %x %x %x\n",
	    T30Control->RemoteCap[ 0 ],
	    T30Control->RemoteCap[ 1 ],
	    T30Control->RemoteCap[ 2 ],
	    T30Control->RemoteCap[ 3 ],
	    T30Control->RemoteCap[ 4 ],
	    T30Control->RemoteCap[ 5 ],
	    T30Control->RemoteCap[ 6 ],
	    T30Control->RemoteCap[ 7 ],
	    T30Control->RemoteCap[ 8 ],
	    T30Control->RemoteCap[ 9 ],
	    T30Control->RemoteCap[ 10 ],
	    T30Control->RemoteCap[ 11 ] );

	T30Control->FrameTxCnt = T30Control->FrameRepeat;

	while ( !fDone )
	{
		/* Save the previous DIS speed */
		T30Control->LastDIS = T30Control->RemoteCap[ 1 ];

		T30TxMakeDCS();

		/* Error in setting up ? */

		if ( T30Control->Status )
		{
			TRACE_ERROR( "=====> T30TxSetMode: T30Control->Status 0x%x\n", T30Control->Status );
			break;
		}

		/* Do not send more than the maximum received DIS size */
		if ( T30Control->DIS_size < LocalDISSize && T30Control->DIS_size > 2 )
			T30Control->LocalCap[ T30Control->DIS_size - 1 ] &= 0x7f;

		TRACE_FAXDEBUG(
		    "T30Tx DCS: %x %x %x %x %x %x %x %x %x %x\n",
		    T30Control->LocalCap[ 0 ],
		    T30Control->LocalCap[ 1 ],
		    T30Control->LocalCap[ 2 ],
		    T30Control->LocalCap[ 3 ],
		    T30Control->LocalCap[ 4 ],
		    T30Control->LocalCap[ 5 ],
		    T30Control->LocalCap[ 6 ],
		    T30Control->LocalCap[ 7 ],
		    T30Control->LocalCap[ 8 ],
		    T30Control->LocalCap[ 9 ] );

        if ( ( t30_attribute.TxRequirePassword ) && ( t30_attribute.FaxLocalTxPassword[ 0 ] != 0 ) )
		{
			if ( BITSET( &T30Control->LocalCap[ 6 ], 0x02 ) )
			{
				TRACE_FAX( "T30Tx Send ( SID )\n" );

				FIFSendTable[ SIDtbl ]  = t30_attribute.FaxLocalTxPassword;
				T30SendFCF( SID );
			}
		}

        if ( ( t30_attribute.TxRequireSubaddress ) && ( t30_attribute.FaxLocalTxSubaddress[ 0 ] != 0 ) )
		{
			if ( BITSET( &T30Control->LocalCap[ 6 ], 0x01 ) )
			{
				TRACE_FAX( "T30Tx Send ( SUB )\n" );

				FIFSendTable[ SUBtbl ]  = t30_attribute.FaxLocalTxSubaddress;
				T30SendFCF( SUB );
			}
		}

		if ( FIFSendTable[ TSItbl ][ 0 ] != 0 )
		{
			TRACE_FAX( "T30Tx Send ( TSI )\n" );

			T30SendFCF( TSI );
		}

		/* Send NSS or DCS depending upon whether or not non-standard mode */
		if ( T30Control->NSTDFlg )
		{
			TRACE_FAX( "T30Tx Send ( NSS )\n" );

			T30SendFCF( NSS );
		}
		else
		{
            UINT16 jnum = fsu_job_num_send;

			TRACE_FAX( "T30Tx Send ( DCS )\n" );

			T30SendFCF( DCS );

			if(T30Control->MemPageNum == 1)
			{
				if (fsu_get_flag_manual_tx(jnum) == 1)
				{
					while (TRUE)
					{
						if (fsu_get_photo_mode(jnum) == 1)
						{
							if (fsu_check_1st_image_file_in_mem(jnum, FAX_FILE_TYPE_JBG) == TRUE)
							{
								break;
							}
							else
							{
								printf("\n(YHCHO) %s() - Wait for the fax01.photo & fax01.jbg to be made...\n", __FUNCTION__);
							}
						}
						else
						{
							if (fsu_check_1st_image_file_in_mem(jnum, FAX_FILE_TYPE_JBG) == TRUE)
							{
								break;
							}
							else
							{
								printf("\n(YHCHO) %s() - Wait for the fax01.jbg to be made...\n", __FUNCTION__);
							}
						}

						if (T30TxCheckErrors() == 1)
						{
							isT30ErrorDetected = 1;

							break;
						}

						OSC_Sleep( 100 );
					}
				}
			}

            if ( (isTxImageProcessDone == 0) && (T30Control->MemPageNum == 1) )
            {
                fsu_tx_image_process_begin();
            }
		}

        if (isT30ErrorDetected == 1)
        {
            fDone = 1;
        }

		SSFStopOffHookMonitor();

		TRACE_FAX( "T30Tx Speed index = %d\n", T30Control->ModemSpeed );

		/* Send the training check sequence */
		ModemTxTCF( T30Control->ModemSpeed );

		T30Control->FrameTxCnt--;

		while ( 1 )
		{
			/* Get the response */
			rsp = T30GetResponse();

			if ( rsp == T30Control->LastFCFSent )
			{
				/* Assume this is an echo and we'll get another command next */
				TRACE_FAX( " T30Tx  echo ( %02X )\n", rsp );
			}
			else
            {
				break;
            }
		}

		/* Fax error? */
		if ( T30Control->Status )
			fDone = 1;

		/* Process the command */
		switch ( rsp )
		{
			case FRAME_ERROR:

				TRACE_FAX( "T30Tx Frame error\n" );

				if ( !T30Control->FrameTxCnt )
				{
                    T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_7;
					fDone = 1;
				}
				break;

				/* No response? */
			case DISCONNECT:         //THY
			case NO_RESPONSE:
			case TIME_OUT:

				TRACE_FAX( "T30Tx No response\n" );

				/* Did we try 2 or 2 times? */
                if ( !T30Control->FrameTxCnt )
				{
                    if (rsp == DISCONNECT)
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_8_1;
                    }
                    else if (rsp == NO_RESPONSE)
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_8_2;
                    }
                    else
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_8_3;
                    }
					fDone = 1;
				}
				break;

			case CFR:

				TRACE_FAX( "T30Tx Got ( CFR )\n" );

                if (T30Control->MemPageNum == 1)
                {
                    activate_mem_to_send();
                }

				T30Control->State = STATE_TX_PHASE_C;
				fDone = 1;
				break;

			case FTT:

				TRACE_FAX( "T30Tx Got ( FTT )\n" );

				T30Control->FrameTxCnt = T30Control->FrameRepeat;

				/* If fallen all the way back, disconnect */
				if ( !T30TxFallback() )
				{
                    T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_9;
					fDone = 1;
				}
				break;

			case DIS:
			case DTC:
				/* Handle frames sent in wrong order */
			case CSI:
			case CIG:
			case NSF:
			case NSC:

				TRACE_FAX( "T30Tx Got ( DIS/DTC )\n" );

				/* See if speed capabilities changed for the French */
				if ( T30Control->RemoteCap[ 1 ] != T30Control->LastDIS )
					T30Control->FrameTxCnt = T30Control->FrameRepeat;

				T30Control->Try2400 = REPEAT_2400;

				if ( !T30Control->FrameTxCnt )
				{
                    if (rsp == DIS)
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_10_1;
                    }
                    else if (rsp == DTC)
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_10_2;
                    }
                    else if (rsp == CSI)
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_10_3;
                    }
                    else if (rsp == CIG)
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_10_4;
                    }
                    else if (rsp == NSF)
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_10_5;
                    }
                    else
                    {
                        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_10_6;
                    }
					fDone = 1;
				}
				break;

			case DCN:

				TRACE_FAX( "T30Tx Got ( DCN )\n" );

				T30Control->State = STATE_TX_DISCONN;
                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_11;
				fDone = 1;
				break;

				/* Fatal error or invalid command? */
				// case DISCONNECT: // move to NO_RESPONSE case THY
			default:

				TRACE_FAX( "T30Tx Fatal error or invalid command\n" );

                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_12;
				fDone = 1;
				break;
		} /* switch */

	} /* while */

	T30Control->release_semaphore = FALSE;
}


/*===========================================================================
*
*    T30TxInitPage
*
*      Initializes the fax job for the page and starts up the pipeline
*
*      Input:          (none)
*      Output:         (none)
*
*===========================================================================*/
void T30TxInitPage( void )
{
    UINT8 training = 0;

	/* Increment the page number */
	T30Control->PageNum++;

    TRACE_FAX("%s() - T30Control->PageNum: %d\n", __FUNCTION__, T30Control->PageNum);

	if ( !T30Control->ECMEnabled )
	{
		training = T30Control->ModemSpeed < T30_SPEED_V17_7200 || T30Control->ECMLongTrain;
		ModemTxInitPage( T30Control->ModemSpeed, training );
	}

	/* Customer wants the first page definition not the last page one */
	/* Store the information for the first page as necessary
	   or for the second page if the first page was a cover page */
    if ( ( T30Control->PageNum == 1 ) || ( ( T30Control->PageNum == 2 ) && T30Control->T4Params.Tx.CoverPage ) )
	{
		/* Assume controlx.h definitions and doclogx.h definitions are same
		   and that LOG_TYPE_FLAG_RES == 0x30 */
		T30Control->LogTypeInitialValue |= ( T30Control->T4Params.Tx.Res << 4 );
	}
}


/*===========================================================================
*
*    T30TxPhaseA
*
*      T.30 Phase A call establishment procedure
*
*      Input:          T30Control->TxFromMemFlg = non-zero if we're TX'ing from memory
*      Output:         T30Control->Status = unchanged if successful
*                        = TIME_OUT_ERROR if time out
*                        = BUSY_TONE_ERROR if busy tone
*                      T30Control->State = STATE_TX_PHASE_B
*
*===========================================================================*/
void T30TxPhaseA( void )
{
	UINT8 status, speed;
    UINT16 jnum = fsu_job_num_send;

    TRACE_FAX( "T30TxPhaseA().....\n" );

    speed = t30_attribute.FaxTxStartSpeed;

	if ( t30_attribute.ECMMode == ECM_NONE )
	{
		if ( speed > T30_SPEED_V17_14400 )
			speed = T30_SPEED_V17_14400;
	}

	/* Now try to detect the called station */
    status = ModemTxDetCalledStation( gFaxCEDDetectionTime,
	                                  T30TxCheckErrors,
	                                  &speed, t30_attribute.FaxRxPolling );

	TRACE_FAXDEBUG( "negotiation status %d speed %d\n", status, speed );

	switch ( status )
	{
		case MODEM_FAX_PHASE_A_CED:

			TRACE_FAX( "T30TxDetCalledStation(): Called station Detected(1)\n" );

			T30Control->CalledStationDetected = 1;
			T30Control->TriedV8 = 0;
			T30Control->V34Mode = FALSE;
			break;

		case MODEM_FAX_PHASE_A_NON_V34:

			T30Control->TriedV8++;
			T30Control->CalledStationDetected = 1;
			break;

		case MODEM_FAX_PHASE_A_V34:

			T30Control->TriedV8++;
			T30Control->V34Mode = TRUE;
			T30Control->CalledStationDetected = 1;
			break;

		case MODEM_FAX_PHASE_A_TIMEOUT:

			if ( !T30Control->Status )
                T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_16;
			break;

		case MODEM_FAX_PHASE_A_BUSY:

			T30Control->BusyToneDetected = 1;
			if ( !T30Control->Status )
                T30Control->Status = FAX_ERROR_BUSY_TONE_ERROR;
			break;

		case MODEM_FAX_PHASE_A_ABORT:

			if ( !T30Control->Status )
                T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_17;
			break;

		default:
			break;
	}

	/* Select the fax speed */

	if ( T30Control->V34Mode )
	{
		T30Control->ModemSpeed = speed;
	}
	else if ( t30_attribute.FaxTxStartSpeed > T30_SPEED_V17_14400 )
	{
		T30Control->ModemSpeed = T30_SPEED_V17_14400;
	}
	else
	{
		T30Control->ModemSpeed = t30_attribute.FaxTxStartSpeed;
	}

	if ( !T30Control->Status && !t30_attribute.FaxRxPolling )
	{
		if ( T30Control->TxFromMemFlg )
		{
            if (fsu_get_flag_manual_tx(jnum) == 1)
            {
                T30Control->T4Params.Tx.PagesInDoc = 0xFF;
            }
            else
            {
                if (fsu_get_broadcast(jnum) == 1)
                {
                    UINT16 DocId = fsu_get_doc_id(jnum);

                    UINT16 OriginalDocId = fsu_get_original_doc_id(jnum);

                    if (DocId != OriginalDocId)
                    {
                        T30Control->T4Params.Tx.PagesInDoc = fsu_get_total_pages(jnum);
                    }
                    else
                    {
                        T30Control->T4Params.Tx.PagesInDoc = fsu_request_get_total_page_no_from_Job_Queue_File(DocId);
                    }
                }
                else
                {
                    T30Control->T4Params.Tx.PagesInDoc = fsu_get_total_pages(jnum);
                }
            }

            printf("\n(YHCHO) %s() - T30Control->T4Params.Tx.PagesInDoc: %d\n", __FUNCTION__, T30Control->T4Params.Tx.PagesInDoc);

            T30Control->T4Params.Tx.BufferedTx = FALSE;
		}
	}

	/* Enter Phase B */
	T30Control->State = STATE_TX_PHASE_B;
}


/*===========================================================================
*
*    T30TxPhaseB
*
*      T.30 Transmit Phase B pre-message procedure Part 1
*      Receives the DIS/DTC from the called fax station
*
*      Input:          ManualMode = TRUE if in manual dial mode, FALSE if not
*      Output:         T30Control->State = STATE_TX_SET_MODE if DIS received
*                      T30Control->Status = BAD_RSP_ERROR if no DIS received
*
*===========================================================================*/
void T30TxPhaseB( void )
{
	UINT8 fDone = 0;
	UINT8 cmd;
	UINT8 i;
	static UINT8 NSF_CSI_Receive;

	TRACE_FAX( "Entering T30TxPhaseB().....\n" );

	T30Control->DISFlg = 0;
	T30Control->got_PWD = 0;
	T30Control->got_SID = 0;
	T30Control->got_SUB = 0;
	T30Control->got_SEP = 0;
	T30Control->got_PSA = 0;

	/* Clear out remote DIS capabilities */

	memset( T30Control->RemoteCap, 0, sizeof( T30Control->RemoteCap ) );
	memset( t30_attribute.FaxRemoteID, 0, sizeof( t30_attribute.FaxRemoteID ) );
	memset( t30_attribute.FaxRemotePassword, 0, sizeof( t30_attribute.FaxRemotePassword ) );
	memset( t30_attribute.FaxRemoteSubaddress, 0, sizeof( t30_attribute.FaxRemoteSubaddress ) );
	memset( t30_attribute.PollRemotePassword, 0, sizeof( t30_attribute.PollRemotePassword ) );
	memset( t30_attribute.PollRemoteSubaddress, 0, sizeof( t30_attribute.PollRemoteSubaddress ) );
	memset( t30_attribute.FaxRemoteNSF, 0, sizeof( t30_attribute.FaxRemoteNSF ) );
	memset( t30_attribute.FaxRemoteNSS, 0, sizeof( t30_attribute.FaxRemoteNSS ) );
	memset( t30_attribute.FaxRemoteNSC, 0, sizeof( t30_attribute.FaxRemoteNSC ) );

	/* Start the T1 timer (default = 38 sec) */
    T30_TIMER_START( TimerB, gFaxT1Timeout );

	/* Now wait for DIS */
	T30Control->FirstCmdFlg = 1;

	/* When remote machine sends DIS command with wrong order we can not receive first
	   command, DIS, because of checking V.8 signal time. So using this flag we have to try
	   to receive next command for getting DIS FIF correctly.
	   With a same reason if someone one want to get NSF, this method should be applied for NSF.
	   9/27/01 THY */
	NSF_CSI_Receive = 1;

	while ( !fDone )
	{
		while ( 1 )
		{
			/* Look for the command */
			cmd = T30GetCommand();

			/* Fax error? */
			if ( T30RxCheckErrors() )
			{
				fDone = 1;
				break;
			}

			if ( cmd == T30Control->LastFCFSent )
			{
				/* Assume this is an echo and we'll get another command next */
				TRACE_FAX( " T30TxPhaseB: echo ( %02X )\n", cmd );
				/* Restart the T2 timer */
				T30_TIMER_START( TimerC, T2_TIMEOUT );
			}
			else
				break;
		}

		TRACE_FAX("T30TxPhaseB: COMMAND %02X\n", cmd );

		/* Fax error? */
		if ( T30RxCheckErrors() )
		{
			fDone = 1;
			break;
		}

		/* Process the command */
		switch ( cmd )
		{
				/* Did we get a command? */
			case FRAME_ERROR:
			case NO_COMMAND:
			case DISCONNECT:
			case TIME_OUT:

				/* Check for timeout */
				TRACE_FAX("T30TxPhaseB: NO_COMMAND\n");

				if ( T30_TIMER_EXPIRE( TimerB ) )
				{
                    if (cmd == FRAME_ERROR)
                    {
                        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_18_1;
                    }
                    else if (cmd == NO_COMMAND)
                    {
                        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_18_2;
                    }
                    else if (cmd == DISCONNECT)
                    {
                        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_18_3;
                    }
                    else
                    {
                        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_18_4;
                    }
					fDone = 1;
				}
				else if ( cmd == DISCONNECT )
				{
					T30Control->FirstCmdFlg = 1;
				}

				break;

				//case DIS: move to below of next case: THY
				/* Handle frames sent in wrong order */
			case CSI:
			case NSF:

				TRACE_FAX("T30TxPhaseB: Got ( CSI/NSF )\n");

				if ( NSF_CSI_Receive )
				{
					NSF_CSI_Receive = 0;
					break;              // receive command again
				}

			case DIS:

				TRACE_FAX( "T30TxPhaseB: Got ( DIS )\n" );
				TRACE_FAX( "[" );
				for ( i = 0; i < RemoteCapSize; i++ )
				{
					TRACE_FAX( " %02X", T30Control->RemoteCap[ i ] );
				}
				TRACE_FAX( " ]\n" );

				/* Haven't already received a DIS */
				if ( !T30Control->DISFlg )
				{
					T30Control->DISFlg = 1;

					/* If ignoring echo, go back and wait for the second DIS */

					if ( t30_attribute.FaxOptions.IgnoreEcho )
					{
						OSC_Sleep( 500 );
						break;
					}
				}

				/* If it's not turn-around polling, just go on to Set Mode */
				if ( !t30_attribute.FaxRxPolling )
				{
					if ( ( t30_attribute.TxRequirePassword )
					  && ( t30_attribute.FaxLocalTxPassword[ 0 ] != 0 ) )
					{
						if ( BITCLEAR( &T30Control->RemoteCap[ 6 ], 0x02 ) )
						{
							TRACE_FAX( "T30TxPhaseB(): Remote doesn't support password\n" );
#ifdef OEM_FAX_REPORT
							ModemControl->Level = 0;
							ModemControl->EQM   = 0xffff;
                            CxProtocolReport( PWD_ERR, NULL, 0, RX_LOG );
#endif
                            T30Control->Status = FAX_ERROR_REMOTE_DOES_NOT_SUPPORT_PW;
							fDone = 1;
							break;
						}
					}

					if ( ( t30_attribute.TxRequireSubaddress )
					  && ( t30_attribute.FaxLocalTxSubaddress[ 0 ] != 0 ) )
					{
						if ( BITCLEAR( &T30Control->RemoteCap[ 6 ], 0x01 ) )
						{
							TRACE_FAX( "T30TxPhaseB(): Remote doesn't support subaddress\n" );
#ifdef OEM_FAX_REPORT
							ModemControl->Level = 0;
							ModemControl->EQM   = 0xffff;
                            CxProtocolReport( SUB_ERR, NULL, 0, RX_LOG );
#endif
							fDone = 1;
							break;
						}
					}

					TRACE_FAX( "T30TxPhaseB: Switching to Set Mode\n" );

					T30Control->State = STATE_TX_SET_MODE;

					fDone = 1;
				}
				else  // Rx polling
				{
					TRACE_FAX( "T30TxPhaseB: Switching to Rx\n" );

					/* If the called station does not have a document, disconnect */
					if ( BITCLEAR( &T30Control->RemoteCap[ 1 ], 0x01 ) )
					{
						TRACE_FAX( "T30TxPhaseB: Send ( DCN ) No document\n" );

						T30SendFCF( DCN );
						T30Control->State = STATE_RX_ABORT;
                        T30Control->Status = FAX_ERROR_REMOTE_DOES_NOT_HAVE_DOCUMENT;
					}
					else
					{
						T30Control->State = STATE_RX_PHASE_B;
						T30Control->Direction = T30_RECEIVE;

                        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_POLL_RX_START, 0, 0, 0, 0, fsu_job_num_rcv);
					}

					fDone = 1;
				}

				break;

			default:

				TRACE_FAX( "T30TxPhaseB(): Illegal response %x\n", cmd );

				/* Illegal response, report error */
                T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_13;

				fDone = 1;

				break;
		} /* switch */

	} /* while */

	TRACE_FAX( "Exiting T30TxPhaseB().....\n" );
}


/*===========================================================================
*
*    T30TxPhaseC
*
*      Transmits a page of the document
*
*      Input:          T30Control->PageNum = current page number
*      Output:         (none)
*
*===========================================================================*/
void T30TxPhaseC( void )
{
    UINT8 isTimerEExpired = 0;
    UINT8 isT30CheckError = 0;
#ifdef OEM_FAX_REPORT
    UINT16 tx_speed = 0;
#endif
    UINT16 TimeToExpire = 0;

    TRACE_FAX( "T30TxPhaseC().....\n" );

	if(T30Control->PostMsgCmd != 3)
	{

#if __MEASURE_PERFORMANCE__
        faxapp_get_job_start_time(T30_TRANSMIT_BOTTLE_NECK);
#endif // #if __MEASURE_PERFORMANCE__

        if (T30Control->MemPageNum >= 2)
        {
            activate_mem_to_send();
        }
	}

    TRACE_FAX( " T30TxPhaseC: T30Control->MemPageNum = %d, T30Control->T4Params.Tx.PagesInDoc = %d\n", T30Control->MemPageNum, T30Control->T4Params.Tx.PagesInDoc );

    TimeToExpire = TX_PHASE_C_TIMEOUT; // 10s

    MODEM_TIMER_START( TimerE, TimeToExpire );

    while (TRUE)
    {
        if (isReadyToTransmit == 1)
        {
            break;
        }
        else
        {
            if (MODEM_TIMER_EXPIRE( TimerE ) == 1)
            {
                printf("\n(YHCHO) %s() - 10S's TimerE Expired in Tx Phase C...\n", __FUNCTION__);

                isTimerEExpired = 1;

                break;
            }
            else
            {
                if ( (T30TxCheckErrors() == 1) || (gFaxAbnormalFlag == FAX_ABNORMAL_FLAG_TX_CANCEL) ) // Comm. Error or Tx Cancel
                {
                    isT30CheckError = 1;

                    printf("\n(YHCHO) %s() - 'Comm. Error' or 'Tx Cancel' Detected While Waiting For The Modem Buffer To Be Filled...\n", __FUNCTION__);

                    break;
                }
                else
                {
                    printf("\n(YHCHO) %s() - Wait Here...\n", __FUNCTION__);
                }
            }
        }

        OSC_Sleep( 100 );
    }

    isTxImageProcessDone = 0;

    if (isT30CheckError == 1)
    {
        return;
    }

    if (isTimerEExpired == 1)
    {
        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_TX_PHASE_C;

        return;
    }

	/* ECM mode */
	if ( T30Control->ECMEnabled )
	{

#if __MEASURE_PERFORMANCE__
        faxapp_get_job_end_time(T30_TRANSMIT_BOTTLE_NECK);

        printf("\n(YHCHO) %s() - Elapsed Time <MEM TO SEND BOTTLE NECK>: %s\n", __FUNCTION__, faxapp_get_elapsed_time(T30_TRANSMIT_BOTTLE_NECK));
#endif // #if __MEASURE_PERFORMANCE__

		T30Control->State = STATE_TX_PHASE_C_ECM;

		return ;
	}

	/* Non-ECM mode */
	else
	{
        TRACE_FAX( " T30TxPhaseC: Non-ECM %d ......\n", ModemControl->ConnectedSpeed );

        /* Make sure there isn't any error */
		if ( T30Control->Status )
			return ;

		/* Initialize for the page */
		T30TxInitPage();

		last_EOL_time = ModemElapsedTime( 0 );  // last tx data time

#ifdef OEM_FAX_REPORT
        tx_speed = (ModemControl->ConnectedSpeed / 1000);
		ModemControl->Level = ModemReadTxLevel();
		ModemControl->EQM   = 0xffff;
		CxProtocolReport( IMG, &tx_speed, 1, TX_LOG );
#endif

		/* Wait for the end of page.  This occurs when there is no more data being sent on the line. */
		/* The modem driver keeps track of this. */
		while ( ModemControl->State )
		{
			OSC_Sleep( T30_DELAY_TIME );

			/* Check for any errors */

			if ( T30Control->Status )
				break;

			if ( ModemElapsedTime( last_EOL_time ) > 30000 )
			{
				TRACE_ERROR( " T30TxPhaseC: 30 sec time out\n" );

                T30Control->Status = FAX_ERROR_BAD_TX_ERROR_4;
				break;
            }
		}

		if ( T30Control->Status )
    	{
			ModemTxDataStop();

			switch ( T30Control->Status )
			{
                case FAX_ERROR_USER_ABORT_ERROR:
                //case FAX_ERROR_BAD_TX_ERROR_1:
                //case FAX_ERROR_BAD_TX_ERROR_2:
                //case FAX_ERROR_BAD_TX_ERROR_3:
                case FAX_ERROR_BAD_TX_ERROR_4:

					OSC_Sleep(100);
					T30SendFCF( DCN );
					T30Control->State = STATE_TX_PHASE_E;
					break;

                case FAX_ERROR_PHONE_LINE_ERROR:

					T30Control->State = STATE_TX_PHASE_E;
					break;

				default:

					T30Control->State = STATE_TX_PHASE_D;
					break;
			}
    	}
		else
		{
#if __ONHOOK_TX_WAITING__
			if( fsu_get_flag_manual_tx(fsu_job_num_send) == 1 )
			{
				TRACE_FAX( "ECMTxBlock : manual tx \n" );
				
				if( gFaxOnHookScanSource == FAX_SCAN_SOURCE_FB )
				{
					UINT16 last_page_no = 0;
					int cnt = 200;
					
					TRACE_FAX( "ECMTxBlock : flatbed \n" );
					
					while(1)
					{
						if( fOnHookScanComplete == TRUE )
						{
							TRACE_FAX( "ECMTxBlock : scan complete \n" );
							break;
						}
						if( cnt < 0 )
						{
							TRACE_FAX( "ECMTxBlock : count overflow \n" );
							break;
						}
						if( T30TxCheckErrors() )
						{
							TRACE_FAX( "ECMTxBlock : Abort Key while waiting FB scan \n" );
							break;
						}
						last_page_no = fsu_get_last_page_no_in_mem(fsu_get_doc_id(fsu_job_num_send), FAX_FILE_TYPE_JBG, 1);
						TRACE_FAX( "ECMTxBlock : check page(%d,%d) \n", last_page_no, T30Control->MemPageNum );
						if( last_page_no > T30Control->MemPageNum )
						{
							break;
						}
						cnt--;
						OSC_Sleep(200);
					}
					TRACE_FAX( "ECMTxBlock : finish waiting(%d)\n", cnt );
				}
			}
#endif /* __ONHOOK_TX_WAITING__ */
	    	// waiting for last byte sent
			OSC_Sleep(600);		/* 600ms for 128bytes FIFO, 300ms for 64bytes FIFO */
			ModemTxDataStop();

			T30Control->State = STATE_TX_PHASE_D;
		}
	}
}


/*===========================================================================
*
*    T30TxPhaseD
*
*      Determines the post-message command
*
*      Input:          T4Res,X = current fax resolution
*                      T30Control->OrigRes = fax resolution when we started this page of the doc.
*                      T30Control->TxFromMemFlg = non-zero if we're TX'ing from memory
*                      T30Control->FaxDocHandle = document pointer if from memory
*                      T30Control->PageNum + 1 = number of the next page to TX
*                      PPRCnt = number of PPR's transmitted
*      Output:         T30Control->PostMsgCmd: 0 = EOP, 1 = MPS, 2 = EOM, 3 = NULL
*                      T30Control->FCFToSend = Actual FCF to send
*
*===========================================================================*/
void T30TxPhaseD( void )
{
	UINT8 PostMsgTable[ 4 ] = {EOP, MPS, EOM, 0 /*NULL*/};

	TRACE_FAX( "T30TxPhaseD().....\n" );

	/* Set the next state */
	T30Control->State = STATE_TX_PHASE_D2;

	/* Change only PPS frame count if retransmitting in ECM */
	if ( T30Control->ECMEnabled && ECMControl->PPRCnt )
	{
		ECMTxSetPPS( 1 );
		return ;
	}

	/* Do not go through following code when not at end of page */
	else if ( T30Control->ECMEnabled && !ECMControl->TxEndOfPage )
	{
		T30Control->PostMsgCmd = 3;
		ECMTxSetPPS( 0 );
		return ;
	}

	/* Assume EOP (0 = EOP) */
	T30Control->PostMsgCmd = 0;

	/* Transmitting from memory */
	if ( T30Control->TxFromMemFlg )
	{
        // if it is manual tx, check total page here again since both scan and transmit occurs concurrently..
        if(fsu_get_flag_manual_tx(fsu_job_num_send) == 1)
		{
            if (gFaxOnHookScanSource == FAX_SCAN_SOURCE_ADF)
            {
                if (fsu_get_scan_busy_flag() == 1)
                {
                    T30Control->T4Params.Tx.PagesInDoc = 0xFF; // Assume.. since scan is not finished..
                }
                else
                {
                    if (fsu_get_photo_mode(fsu_job_num_send) == 1)
                    {
                        T30Control->T4Params.Tx.PagesInDoc = fsu_get_last_page_no_in_mem(fsu_get_doc_id(fsu_job_num_send), FAX_FILE_TYPE_PHOTO, 1);
                    }
                    else
                    {
                        T30Control->T4Params.Tx.PagesInDoc = fsu_get_last_page_no_in_mem(fsu_get_doc_id(fsu_job_num_send), FAX_FILE_TYPE_JBG, 1);
                    }
                }
            }
            else
            {
                UINT8 PageNoToTransmit = T30Control->MemPageNum;
                char FileName[MAX_LEN_FILE_PATH_NAME] = {0, };
                char Extension[6] = {0, };

                PageNoToTransmit++;

                if (fsu_get_photo_mode(fsu_job_num_send) == 1)
                {
                    snprintf(Extension, sizeof(Extension), "photo");

                    snprintf(FileName, sizeof(FileName), "%s%cDoc%04X%cfax%02X.%s", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(fsu_job_num_send), PATH_SEPARATOR, PageNoToTransmit, Extension);
                }
                else
                {
                    snprintf(Extension, sizeof(Extension), "jbg");

                    snprintf(FileName, sizeof(FileName), "%s%cDoc%04X%cfax%02X.%s", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(fsu_job_num_send), PATH_SEPARATOR, PageNoToTransmit, Extension);
                }

                printf("\n(YHCHO) %s() - FileName: %s\n", __FUNCTION__, FileName);

                if (access(FileName, F_OK) == 0)
                {
                    printf("\n(YHCHO) %s() - There Are More Pages..\n", __FUNCTION__);

                    T30Control->T4Params.Tx.PagesInDoc = PageNoToTransmit;
                }
                else
                {
                    printf("\n(YHCHO) %s() - There Is No More Page..\n", __FUNCTION__);

                    T30Control->T4Params.Tx.PagesInDoc = PageNoToTransmit - 1;
                }
            }
		}

        TRACE_FAX( " T30TxPhaseD: MemPageNum = %d, PagesInDoc = %d\n", T30Control->MemPageNum, T30Control->T4Params.Tx.PagesInDoc );

		/* Check for more documents if done with current document */
		if ( T30Control->MemPageNum >= T30Control->T4Params.Tx.PagesInDoc )
		{
            ; // No Action Here..
		}
	  	/* Check for a mode change if more pages in document. */
	  	else if ( T30Control->MemPageNum < T30Control->T4Params.Tx.PagesInDoc )
        {
	  		T30Control->PostMsgCmd = T30TxCheckModeChange();

            isReadyToTransmit = 0;
        }
    }

	/* Transmitting from the scanner */
	else
	{
		T30Control->PostMsgCmd = 0;
	}

	/* Now look up the command to be sent to the remote */
	T30Control->FCFToSend = PostMsgTable[ T30Control->PostMsgCmd ];

	/* Set the PRI-Q bit if there was a local voice request */
	if ( ( T30Control->FCFToSend == EOP ) && t30_attribute.FaxVoiceRequest )
		T30Control->FCFToSend |= 0x10;

	if ( T30Control->ECMEnabled )
    {
		ECMTxSetPPS( 0 );
    }
	else
	{
		T30Control->MemPageNum++;
	}
}


/*===========================================================================
*
*    T30TxCheckModeChange
*
*      Checks for a change in page width of resolution
*
*      Input:          T30Control->PostMsgCmd = 0 (EOP)
*                      FaxCxJob = Cx job number
*                      T30Control->FaxDocHandle = doc handle (if TX from mem)
*                      T30Control->OrigRes = previous page resolution
*                      T30Control->OrigWidth = previous page width
*      Output:
*                      return value = 1 (MPS) or 2 (EOM)
*
*===========================================================================*/
UINT8 T30TxCheckModeChange( void )
{
	TRACE_FAX( "Entering T30TxCheckModeChange()\n" );
	return ( 1 );			// MFP
}


/*===========================================================================
*
*    T30TxPhaseD2
*
*      Sends the post message command and waits for a response
*
*      Input:          T30Control->FCFToSend = Actual FCF to send
*                      T30Control->PostMsgCmd: 0 = EOP, 1 = MPS, 2 = EOM, 3 = NULL
*                      T30Control->FrameRepeat = number of times we should send the frame
*      Output:         T30Control->Status and T30Control->State set appropriately
*
*===========================================================================*/
void T30TxPhaseD2( void )
{
	UINT8 rsp;
	UINT8 i;

	TRACE_FAX( "T30TxPhaseD2().....\n" );

	/* Try 2 or 3 times */
	T30Control->FrameTxCnt = T30Control->FrameRepeat;

	/* Wait for modem in RX mode */
	if ( !T30Control->V34Mode )
		ModemWaitRX();

	/* Send the FCF up to 3 times */
	while ( 1 )
	{
		if ( T30Control->Status )           // check abnormal condition
        {
			return ;
        }

		if ( T30Control->LastFCFSent != CRP )
		{
			TRACE_FAX("====>T30Control->FCFToSend: 0x%x\n", T30Control->FCFToSend);

			/* Send the FCF and wait for a response */
			T30SendFCF( T30Control->FCFToSend );

			if ( T30Control->Status )           // check abnormal condition
            {
				return ;
            }
		}

		while ( 1 )
		{
			/* Get the response */
			rsp = T30GetResponse();

			if ( rsp == T30Control->LastFCFSent )
			{
				/* Assume this is an echo and we'll get another command next */
				TRACE_FAX( " T30RxPhaseD2: echo ( %02X )\n", rsp );
			}
			else
            {
				break;
            }
		}

		/* If sent 3 times with no response, proceed to Phase E */
        if ( ( rsp == NO_RESPONSE ) || ( rsp == FRAME_ERROR ) || ( rsp == DISCONNECT ) || ( rsp == TIME_OUT ) )
		{
			if ( --T30Control->FrameTxCnt == 0 )
			{
				T30Control->State = STATE_TX_PHASE_E;
                if (rsp == NO_RESPONSE)
                {
                    T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_19_1;
                }
                else if (rsp == FRAME_ERROR)
                {
                    T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_19_2;
                }
                else if (rsp == DISCONNECT)
                {
                    T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_19_3;
                }
                else
                {
                    T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_19_4;
                }
				return ;
			}
		}
		else
        {
			break;
        }

		OSC_Sleep(100);
	}

	/* Look up the index of the response */
	for ( i = 0; i < TxPhaseDTableSize; i++ )
	{
		if ( TxPhaseDTable[ i ] == rsp )
        {
			break;
        }
	}

	/* If bad FCF, exit with error */
	if ( i == TxPhaseDTableSize )
	{
        T30Control->Status = FAX_ERROR_BAD_RSP_ERROR_14;

        if ( rsp == DCN )
        {
			T30Control->State = STATE_TX_DISCONN;
        }
		else
        {
			T30Control->State = STATE_TX_PHASE_E;
        }

		return ;
	}

	if((T30Control->PostMsgCmd != 3 && rsp == MCF && T30Control->ECMEnabled) || (!T30Control->ECMEnabled && (rsp != CRP) && (rsp != RTN)))
	{
        UINT16 jnum = fsu_job_num_send;

		if(!T30Control->ECMEnabled)
		{
            faxapp_set_event_flags(FAXAPP_T30_EVENT_TX_COMPLETE);
		}

		RTNCount = 0;
        fsu_tx_update_page();

        if (fsu_get_flag_manual_tx(jnum) == 1)
        {
            printf("\n(YHCHO) %s() - gFaxOnHookScanSource: %d\n", __FUNCTION__, gFaxOnHookScanSource);

            if (gFaxOnHookScanSource == FAX_SCAN_SOURCE_ADF)
            {
                if (fsu_get_scan_busy_flag() == 1)
                {
                    if (isTxImageProcessDone == 0)
                    {
                        fsu_tx_image_process_begin();
                    }
                }
                else
                {
                    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };

                    snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum));

                    if (access(DocIdPathName, F_OK) == 0)
                    {
                        printf("\n(YHCHO) %s() - There Are More Pages To Transmit By ADF...\n", __FUNCTION__);

                        if (isTxImageProcessDone == 0)
                        {
                            fsu_tx_image_process_begin();
                        }
                    }
                    else
                    {
                        printf("\n(YHCHO) %s() - There Is No More Page To Transmit By ADF...\n", __FUNCTION__);

                        // No Action
                    }
                }
            }
            else
            {
                char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };

                snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum));

                if (access(DocIdPathName, F_OK) == 0)
                {
                    printf("\n(YHCHO) %s() - There Are More Pages To Transmit By FB...\n", __FUNCTION__);

                    if (isTxImageProcessDone == 0)
                    {
                        fsu_tx_image_process_begin();
                    }
                }
                else
                {
                    printf("\n(YHCHO) %s() - There Is No More Page To Transmit By FB...\n", __FUNCTION__);

                    // No Action
                }
            }
        }
        else
        {
            if (fsu_get_total_pages(jnum) == 0)
            {
                // No Action
            }
            else
            {
                if (isTxImageProcessDone == 0)
                {
                    fsu_tx_image_process_begin();
                }
            }
        }
	}

	/* Otherwise, process the FCF and set the next state */
	if ( T30Control->ECMEnabled )
    {
		T30Control->State = ECMTxPhaseDProcessResponse( rsp, i );
    }
	else
    {
		T30Control->State = T30TxPhaseDProcessResponse( rsp, i );
    }
}


/*===========================================================================
*
*    T30TxPhaseDProcessResponse
*
*      Processes a response received during trasnmit Phase D non-ECM Mode
*
*      Input:          response from remote receiver
*                      response index into next state table
*      Output:         T.30 next state
*
*===========================================================================*/
T_T30_STATES T30TxPhaseDProcessResponse( UINT8 response, UINT8 table_index )
{
	T_T30_STATES next_state;

	/* Set the next state */

	switch ( response )
	{
		case RTN:
			next_state = T30TxProcessRTN( table_index );
            T30Control->MemPageNum--;
            RTNCount++;
			break;

		default:
			next_state = TxPhaseDNextState[ table_index ][ T30Control->PostMsgCmd ];
			break;
	}

	return ( next_state );
}


/*===========================================================================
*
*    T30TxPhaseE
*
*      Ejects the document and sends DCN
*
*      Input:          T30Control->TxFromMemFlg = non-zero if we're TX'ing from memory
*                      t30_attribute.FaxLineRequest = TRUE if we're quitting after a voice request
*      Output:         T30Control->State = STATE_TX_DISCONN
*
*===========================================================================*/
void T30TxPhaseE( void )
{
	TRACE_FAX( "T30TxPhaseE().....\n" );

	/* Send DCN only if we didn't just do a voice request */
	/* and didn't detect busy tone */
	if ( !t30_attribute.FaxLineRequest && !T30Control->BusyToneDetected && T30Control->CalledStationDetected )
		T30SendFCF( DCN );

	// give modem a chance to send all data
	OSC_Sleep( 1000 );

	ModemControl->State = MODEM_IDLE;
	T30Control->State = STATE_TX_DISCONN;

	TRACE_FAX( "T30TxPhaseE() End\n" );
}


/*===========================================================================
*
*    T30TxProcessRTN
*
*      Determines what to do when RTN received
*
*      Input:          T30Control->PostMsgCmd: 0 = EOP, 1 = MPS, 2 = EOM, 3 = NULL
*                      response index into next state table
*      Output:         next state of T.30
*
*===========================================================================*/
T_T30_STATES T30TxProcessRTN ( UINT8 table_index )
{
	TRACE_FAX( "T30Tx(): RTN\n" );

	if ( !T30TxFallback() )
	{
        T30Control->Status = FAX_ERROR_BAD_RX_ERROR_3;
		return ( STATE_TX_PHASE_E );
	}
	else
    {
		return ( TxPhaseDNextState[ table_index ][ T30Control->PostMsgCmd ] );
    }
}


/*===========================================================================
*
*    T30TxVoiceReq
*
*      Alerts the local operator that a PIP/PIN has been received.
*      Waits for a line request.
*
*      Input:          T30Control->PostMsgCmd = index of post-message command sent
*                      T30Control->LastFCFSent = post-message command sent
*                      T30Control->LastFCFRcvd = post-message command received
*      Output:         T30Control->State & T30Control->Status set appropriately
*
*===========================================================================*/
void T30TxVoiceReq( void )
{
	UINT8 PIP_bit_set;

	TRACE_FAX( "Entering T30TxVoiceReq().....\n" );

	t30_attribute.FaxVoiceRequest = 1;

	T30Control->PRIQCnt--;

	/* Line request.  Set the PRI-Q bit (ECM or non-ECM mode) and quit */
	if ( T30AlertLocalOp() )
	{

		if ( T30Control->ECMEnabled )
		{
			ECMControl->PPS_FIF[ 0 ] |= 0x10;
			T30SendFCF( PPS );
		}
		else
			T30SendFCF( T30Control->LastFCFSent | 0x10 );

		T30Control->State = STATE_TX_PHASE_E;
	}

	/* No line request. */
	else
	{
		t30_attribute.FaxVoiceRequest = 0;
		T30Control->PRIQCnt++;

		/* Force a disconnect if already at 2400 bps */

		if ( ( ( T30Control->LastFCFRcvd & 0xFE ) == PIN )
		  && !T30TxFallback() )
		{
			T30Control->PostMsgCmd = 0;
		}

		/* Look up the error code and next state */
		PIP_bit_set = ( T30Control->LastFCFRcvd & 0x08 ) ? 1 : 0;

		T30Control->Status = VReqErrorCode[ 2 * T30Control->PostMsgCmd + PIP_bit_set ];

		T30Control->State = VReqNextState[ 2 * T30Control->PostMsgCmd + PIP_bit_set ];
	}
}


/*===========================================================================
*
*    T30TxCheckErrors
*
*      Checks for scanner and memory errors.  Note that in Phase C any detected
*      error will stop the transmission and throw away any available data.
*
*      Input:          T30Control->TxFromMemFlg or ->PollFromMemFlg = 1
*                         if transmitting from memory
*                      ScnStatus = scanner status
*      Output:         return value = 0 if no faults, non-zero otherwise
*                      T30Control->Status non-zero if we have to quit
*
*===========================================================================*/
UINT8 T30TxCheckErrors( void )
{
	/* Already might have an error ? */

	if ( T30Control->Status )
	{
		TRACE_ERROR( "T30TxCheckErrors(): Existing Error %d\n", T30Control->Status );

		return 1;
	}

	return 0;
}

