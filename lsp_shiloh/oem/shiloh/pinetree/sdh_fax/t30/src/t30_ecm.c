/*
 *  $RCSfile: t30_ecm.c,v $
 * $Revision: 1.62 $
 *   $Author: miurat $
 *     $Date: 2012/02/29 02:36:57 $
 */
/************************************************************************
 *
 *      t30_ecm.c
 *
 *  T.30 Error Correction Mode Functions
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "SDHFaxCommonHeader.h"
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
#include "unistd.h"
#include "faxapp_notify.h"
#include "../../image_backup/include/image_backup.h"


T_ECM_CONTROL *ECMControl = 0;
static UINT8 out_frame_num;

extern void HDLCTxWriteDataFromCDB( CDB_BUFFER *cdb );
extern void prepare_rx_buffers(UINT16 buffers, BOOL ecm_mode);
extern void de_activate_rcv_to_mem(void);
extern int sw_jbig_encode_tx (int Page, char *FileName, UINT16 jnum, UINT8 Direction);
extern int sw_jbig_decode_tx(int Page, UINT16 jnum, char *FileName, UINT8 Direction);
extern BOOLEAN fsu_check_manual_tx(void);
extern BOOLEAN fsu_rcv_next_page(void);
#ifdef HAVE_IMAGELOG
extern BOOLEAN IsImageLogEnabled(char jobtype, int subJobType);
extern UINT32 create_unique_job_id( void );
extern void fsu_update_unique_job_id (UINT16 jnum, UINT32 jobId);
#endif
extern int T4_Decoding(UINT16 jnum, char *FileName, char *out_FileName, int codingtype, int width_type, int rx_resolution, UINT8 Direction);
extern int T4_encoding(int Page, char *FileName, int codingtype, int faxRes, int job_num_send, UINT8 Direction, int photo_image_resolution);
extern UINT8 fsu_get_fax_forward (UINT16 jnum);
extern UINT8 fsu_get_poll_receive (UINT16 jnum);
extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT8 fsu_get_width(UINT16 jnum);
extern UINT16 fsu_get_doc_id(UINT16 jnum);
extern JOBNODE_BUFFER *pBuffer_front[NUM_OF_BUFF];
extern BOOLEAN T4_end_of_page;
extern void faxapp_set_event_flags(int Event);
extern void fsu_set_next_prn_num(UINT16 next_prn_num);
extern void Create_Job_Queue_File(UINT16 jnum, FAX_APP_JOB_TYPE_t FaxAppJobType, UINT16 DocId);
extern void faxapp_make_pdf_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType, int Request, int Reason);
#ifdef HAVE_IMAGELOG
extern void fsu_update_remote_no_for_image_log (UINT8 type, UINT16 jnum);
#endif
extern int fsu_image_validation_check (int res, int jnum);

void ECMRxProcessCTC( void );

#define TONE_A  2       // filter to detect FSK in case of V.27ter receiving

extern int getSaveFaxFile(void);
extern int getgFaxPrintMode(void);

extern TX_EVENT_FLAGS_GROUP fipp_rx_events_flags;
#define FLAG_RX_COMPLETE       (1 << 6)

extern UINT16 fsu_job_num_rcv;

extern int gFaxRxTotalPages;
extern UINT8 too_many_error;
extern UINT8 isFSURxImageProgessBegin;
//taeyoukim add.
extern int gFaxRxCurrentPage;
extern BOOLEAN isReadyToFaxPrint;
extern UINT8 RNRSentCount;
extern UINT8 RTNCount;
UINT8 *gpRxPageMemory = 0;	//add.bong.2012-09-04 8:04am
extern void fsu_set_prn_job_ready(UINT16 jnum);
extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
static UINT8 *pBuff = (UINT8 *)NULL;
extern UINT32 gFaxT5Timeout;
UINT32 total_size = 0;
#if __ONHOOK_TX_WAITING__
#include "../../faxapp/include/faxapp_scan.h"

extern UINT16 fsu_job_num_send;
extern UINT8 gFaxOnHookScanSource;
extern BOOLEAN fOnHookScanComplete;
extern UINT8 fsu_get_flag_manual_tx(UINT16 jnum);
extern UINT16 fsu_get_last_page_no_in_mem (UINT16 DocId, UINT8 FileType, UINT8 Mode);
#endif /* __ONHOOK_TX_WAITING__ */

/*===========================================================================
*
*    ECMInitJob
*
*      Initializes session for ECM mode
*
*      Input: frame size (ECM_64 = 64 bytes, ECM_256 = 256 bytes)
*      Output: 0 if successful, 1 if allocations failed
*
*===========================================================================*/
UINT8 ECMInitJob( UINT8 frame_size )
{
	UINT8 status;
	UINT16 bytes_per_frame;

	status = E_OK;
	bytes_per_frame = 256;
	out_frame_num = 0;
	T4_end_of_page = FALSE;

	TRACE_FAX( "ECMInitJob: frame_size = %d\n", bytes_per_frame );

	/* If frame size changed, reset the ECM job */

	if ( ECMControl && ( ECMControl->BytesPerFrame != bytes_per_frame ) )
	{
		ECMReturnCDBs();
		//T4TxRxDeallocateCDBs();
		OSC_Deallocate_Memory( ECMControl );
		ECMControl = 0;
	}

	/* Allocate the Control structure for ECM if not already allocated */
	if ( !ECMControl )
	{
		if ( OSC_Allocate_Zero_Memory( (OSC_MEMORY_POOL *)NULL, ( void ** ) & ECMControl, sizeof( T_ECM_CONTROL ), TRUE ) != E_OK )
		{
			TRACE_FAX( "ECMInitJob: Allocation Error T_ECM_CONTROL\n" );
			status = !E_OK;
		}

		/* Make sure the structure is zeroed */
		memset( ( void * ) ECMControl, 0, sizeof( T_ECM_CONTROL ) );

		/* Initialize T.30 FIF tables (ECM) with dynamic addresses */

		FIFSendTable[ PPStbl ] = ECMControl->PPS_FIF;
		FIFSendTable[ PPRtbl ] = ECMControl->PPR_FIF;
		FIFSendTable[ CTCtbl ] = T30Control->LocalCap;
		FIFSendTable[ EORtbl ] = ECMControl->PPS_FIF;

		FIFReceiveTable[ PPStbl ] = ECMControl->PPS_FIF;
		FIFReceiveTable[ PPRtbl ] = ECMControl->PPR_FIF;
		FIFReceiveTable[ CTCtbl ] = T30Control->RemoteCap;
		FIFReceiveTable[ EORtbl ] = ECMControl->PPS_FIF;

		/* Initialize control structure variables which aren't already zero */
		T30Control->ECMEnabled = 1;

		ECMControl->BytesPerFrame = bytes_per_frame;
		ModemControl->BytesPerFrame = bytes_per_frame;

		ECMControl->RxBlockNum = 0xFF;

		memset( ECMControl->PPR_FIF, 0xFF, 32 );
	}

	TRACE_FAX( "ECMInitJob() Frame Size %d\n", ECMControl->BytesPerFrame );

	return ( status );
}


/*===========================================================================
*
*    ECMFeedCDB
*
*   Places the CDB in the next location in the ECM frame queue in ascending
*   order.  The Modem ISR will process this CDB/frame as soon as it gets to it in
*   the queue
*
*   Input:      pointer to CDB control structure
*   Output:     0 - success
*               1 - failure
*
*===========================================================================*/
UINT8 ECMFeedCDB( CDB_BUFFER *cdb )
{
    UINT8 status = 0;
    UINT8 temp;

	/* If more blocks than we can handle, report error */

	if ( ECMControl->NetBlockCount >= MAX_ECM_BLOCKS )
	{
		status = 1;
	}
	else
	{
		/* Save the frame number for later MCB storage */
		temp = ECMControl->CDBFrameIn;

		/* Place the CDB in the next location in the ECM block */
		ECMControl->Block[ ECMControl->BlockBack ].Frame[ ECMControl->CDBFrameIn ] = cdb;

		/* If we fill up all the frames in the block or hit the end of page, */
		/* move on to the next block and increment the block count */

        if ( ( ECMControl->CDBFrameIn == ( MAX_ECM_FRAMES - 1 ) ) || ECMControl->Block[ ECMControl->BlockBack ].Frame[ ECMControl->CDBFrameIn ]->end_of_page )
		{
			ECMControl->BlockBack = ( ECMControl->BlockBack + 1 ) % MAX_ECM_BLOCKS;
			ECMControl->CDBFrameIn = 0;
			ECMControl->NetBlockCount++;
		}
		else
        {
			ECMControl->CDBFrameIn++;
        }
	}

	return ( status );
}

/*===========================================================================
*
*    ECMCDBDone
*
*   Informs the T4 services that the modem has finished processing the
*   current CDB.
*
*   Input:      by_request flag indicates if by external request
*               block_number indicates which block is being processed
*               frame_number indicates which frame is being processed
*   Output:     none
*
*===========================================================================*/
void ECMCDBDone( UINT8 by_request, UINT8 block_number, UINT16 frame_number )
{
	/* Notify the T4 services that the CDB is complete */
	/* and mark the CDB null in the ECM table */

	if ( ECMControl->Block[ block_number ].Frame[ frame_number ] )
	{
		/* Mark the CDB null */
		ECMControl->Block[ block_number ].Frame[ frame_number ] = NULL;
	}
}


/*===========================================================================
*
*    ECMReturnCDBs
*
*   Returns all CDBs in the ECM CDB queue
*
*   Input:      none
*   Output:     none
*
*===========================================================================*/
UINT8 ECMReturnCDBs( void )
{
	UINT8 block;
    UINT16 frame;

	/* Loop through each of the CDB elements and invoke the notify function */

	for ( block = 0; block < MAX_ECM_BLOCKS; block++ )
	{
		for ( frame = 0; frame < MAX_ECM_FRAMES; frame++ )
        {
			ECMCDBDone( 1, block, frame );
        }
	}

	/* Reinitialize counters */
	ECMControl->NetBlockCount = 0;

	ECMControl->BlockFront = ECMControl->BlockBack = ECMControl->CDBFrameIn = 0;

	TRACE_FAX( "ECMReturnCDBs()\n" );

	return ( 0 );
}


/*===========================================================================
*
*    ECMBlockDone
*
*      Passes all CDBs in the block to T4 for processing
*      Also advances the block pointer
*
*      Input: none
*      Output:
*
*===========================================================================*/
void ECMBlockDone( void )
{
	TRACE_FAX( "ECMBlockDone: Return Here !!!\n" );

	/* Point to the next block and decrement the block count */
	ECMControl->BlockFront = ( ECMControl->BlockFront + 1 ) % MAX_ECM_BLOCKS;

	/* Decrement the block counter */

	if ( ECMControl->NetBlockCount )
		ECMControl->NetBlockCount--;
}


UINT8 ECMGetBlockFront(void)
{
	return out_frame_num;			//ECMControl->BlockFront;
}


void ECMResetBlockFront(void)
{
	out_frame_num = 0;
}


UINT8 ECMTx_PPR_Transmit(void)
{
	return ECMControl->PPRCnt;
}


/*===========================================================================
*
*    ECMShutdownJob
*
*   Deallocates the ECM control structure and returns CDBs
*
*   Input:      none
*   Output:     0 if successful, 1 if error detected
*
*===========================================================================*/
UINT8 ECMShutdownJob( void )
{
    UINT8 status;

	status = 0;

	if ( ECMControl )
	{
		ECMReturnCDBs();
		OSC_Deallocate_Memory( ECMControl );
		T30Control->ECMEnabled = 0;
		T30Control->ECMLongTrain = 0;
		T30Control->T4Params.Rx.ECMMode = ECM_NONE;
		ECMControl = 0;
	}
	else
    {
		status = 1;
    }

	TRACE_FAX( "ECMShutdownJob() status %d\n", status );

	return ( status );
}


/*===========================================================================
*
*    ECMAbortJob
*
*   Tells ECM to abort any operations currently in progress
*
*   Input:      none
*   Output:     none
*
*===========================================================================*/
UINT8 ECMAbortJob( void )
{
	return ( 0 );
}


/*===========================================================================
*
*    ECMQuery
*
*   Provides information to the caller regarding modem operating parameters
*
*   Input:      none
*   Output:     none
*
*===========================================================================*/
void ECMQuery( UINT8 query_type )
{
}


/*===========================================================================
*
*    ECMCDBCheckEndOfPage
*
*    Checks if the current CDB is the end of page
*
*   Input:      none
*   Output:     1 = end of page in CDB detected
*               0 = not at end of page
*===========================================================================*/
UINT8 ECMCDBCheckEndOfPage( UINT8 frame_number )
{
	if ( !ECMControl->Block[ ECMControl->BlockFront ].Frame[ frame_number ] )
    {
		return ( 0 );
    }
	else
    {
		return ( ECMControl->Block[ ECMControl->BlockFront ].Frame[ frame_number ]->end_of_page );
    }
}


/*===========================================================================
*
*    ECMNextBlock
*
*      Prepares for the next ECM block
*
*      Input:
*      Output:         See below
*
*===========================================================================*/
void ECMNextBlock( void )
{
	TRACE_FAX( "Entering ECMNextBlock().....\n" );

	/* Clear the block counter on the LCD */

	/* Since the block is complete, pass the CDBs to the T4 service */
	ECMBlockDone();

	/* Inititalize PPR FIF to "all bad" */
	memset( ECMControl->PPR_FIF, 0xFF, 32 );

	/* Initialize counters */
	ECMControl->PPRCnt = 0;
	out_frame_num = 0;
	ECMControl->RxTotalFrameCnt = 0;
}


/*===========================================================================
*
*    ECMTxSetPRIQ
*
*      Sets PRIQ bit during ECM transmit mode
*      Input:          (none)
*      Output:         (none)
*
*===========================================================================*/
void ECMTxSetPRIQ( void )
{
	ECMControl->PPS_FIF[ 0 ] |= 0x10;
}


/*===========================================================================
*
*    ECMCheckPPR
*
*      Checks the PPR FIF to see if a frame is good or bad.  Returns
*      with the isolated bit in the return value
*
*      Input:          frame number
*      Output:         return value = 0 if good, non-0 if bad - Contains the bit
*                        of the PPR FIF in it's correct position.  Other bits AND'ed out
*
*===========================================================================*/
UINT8 ECMCheckPPR( UINT8 frame_number )
{
	UINT8 byte_number;
	UINT8 bit_mask;

	byte_number = frame_number / 8;
	bit_mask = 1 << ( frame_number % 8 );

	return ( ECMControl->PPR_FIF[ byte_number ] & bit_mask );
}


/*===========================================================================
*
*    ECMRxCheckCopyQuality
*
*      Checks the receiver's PPR for errors
*
*      Input:          RxGoodFrameCnt = number of good frames we received
*                      PPS_FIF[3] = number of frames TX sent
*      Output:         PPRCnt = TRUE if there were errors
*
*===========================================================================*/
void ECMRxCheckCopyQuality( void )
{
	UINT8 ppr = 0;
	int i;

	TRACE_FAX( "Entering ECMRxCheckCopyQuality().....\n" );

	/* If frame counts don't match, indicate an error */
	if ( ( ECMControl->RxGoodFrameCnt - 1 ) != ECMControl->PPS_FIF[ 3 ] )
	{
		ppr = 1;

		TRACE_FAX( "Frame count mismatch ACTUAL(%d) PPS(%d)\n",
		           ( ECMControl->RxGoodFrameCnt - 1 ), ECMControl->PPS_FIF[ 3 ] );
	}
	else
	{
		/* Loop through each of the frames, if any errors, set the PPRCnt */

		for ( i = 0; i < MAX_ECM_FRAMES ; i++ )
		{
			if ( 0 != ECMCheckPPR( i ) )
			{
				TRACE_FAX( "Found bad frame (%d)\n", i );

				ppr = 1;
				break;
			}

			/* Exit if we're at the last frame */
			if ( i == ECMControl->RxTotalFrameCnt )
			{
				TRACE_FAX( "All frames good.\n" );

				ppr = 0;
				ECMControl->PPRCnt = 0;
				break;
			}
		}
	}

	if ( ppr != 0 )
		ECMControl->PPRCnt++;
}


/*===========================================================================
*
*    ECMEndFrame
*
*      Wakes up the waiting T30 task at the end of a ECM frame.
*
*===========================================================================*/
void ECMEndFrame( void )
{
//	TRACE_FAXDEBUG( "ECMEndFrame\n" );

	if(ECMControl)
		ECMControl->FrameDone = TRUE;
}


/*===========================================================================
*
*    ECMRxPrintBlock
*
*      Start printing an ECM block
*
*      Input:          PPRCnt = nonzero if there were errors
*                      PPR_FIF = error flags
*      Output:         (none)
*
*===========================================================================*/
void ECMRxPrintBlock( void )
{
	UINT8 frame, i;
    FILE *pFile = (FILE *)NULL;
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char TiffName[MAX_LEN_FILE_PATH_NAME] = {0, };
    int result = 0;
    UINT8 coding_type = 0;
    UINT8 rx_res = 0;
    UINT16 DocId = 0;
    UINT16 jnum = fsu_job_num_rcv;

    //printf("\n(YHCHO) %s() , total_size = %d\n", __FUNCTION__, total_size);

	if(T30RxCheckErrors())
		return;

	ECMControl->RxBlockNum = ECMControl->PPS_FIF[ 2 ];

	for ( frame = 0; ; frame++ )
	{
		if(frame == 0 && ECMControl->RxBlockNum == 0)
		{
			pBuff = (UINT8 *)pBuffer_front[0]->data;
			T30Control->total_jbig_rcvd_line = 0;

			for(i = 0 ; i < 4; i++)
            {
                T30Control->total_jbig_rcvd_line += *(pBuff + 8 + i) << (3 - i)*8;
            }

			TRACE_FAX( "ECMRxPrintBlock: total jbig line = %ld\n", T30Control->total_jbig_rcvd_line );
		}

		// RX_ERROR_PRINT .. prepare printing data in error .. Fill any bad frames with 0's
		if ( ECMCheckPPR( frame ) )
		{
		    pBuff = (UINT8 *)pBuffer_front[frame]->data;
			ECMControl->RxFrameSize[frame] = ECMControl->BytesPerFrame;
			memset(pBuff, 0, ECMControl->BytesPerFrame);        // fill 0 in case of error
		}

		pBuff = (UINT8 *)pBuffer_front[frame]->data;
        memcpy(gpRxPageMemory+total_size, pBuff, ECMControl->RxFrameSize[frame]);
        //printf("\n(YHCHO) %s() - memcpy(%x, %x, %d, %d)\n", __FUNCTION__, gpRxPageMemory, pBuff, total_size, ECMControl->RxFrameSize[frame]);

		pBuffer_front[frame]->used = ECMControl->RxFrameSize[frame];

		total_size += ECMControl->RxFrameSize[frame];

		if ( frame == ECMControl->RxTotalFrameCnt )
		{
			if ( ( ECMControl->PPS_FIF[ 0 ] & 0xEE ) || T30Control->Status )
            {
				pBuffer_front[frame]->lastBuffer = TRUE;
            }

			TRACE_FAX( "ECMRxPrintBlock: total size = 0x%lx, frame = %d\n", total_size, frame );
			break;
		}

		if ( frame == ( MAX_ECM_FRAMES - 1 ) )
			break;
	}

	if( pBuffer_front[frame]->lastBuffer )
	{
		isFSURxImageProgessBegin = 0;
        gFaxRxTotalPages++;
        too_many_error = 0;
        DocId = fsu_get_doc_id(jnum);

        snprintf(PathName, sizeof(PathName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, gFaxRxTotalPages);
        snprintf(TiffName, sizeof(TiffName), "%s%cDoc%04X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, DocId, PATH_SEPARATOR, gFaxRxTotalPages);

        printf("\n(YHCHO) %s() - PathName: %s\n", __FUNCTION__, PathName);

        pFile = fopen(PathName, "wb");
        if (pFile == (FILE *)NULL)
        {
            printf("\n(YHCHO) %s() - Check Point [1]\n", __FUNCTION__);

            XASSERT(0,0);
        }
        else
        {
            printf("\n(YHCHO) %s() - total_size: %ld\n", __FUNCTION__, total_size);

            result = fwrite(gpRxPageMemory, sizeof(UINT8), total_size, pFile);

            memset(pBuff, 0, ECMControl->BytesPerFrame);
            total_size = 0;

            printf("\n(YHCHO) %s() - result: %d\n", __FUNCTION__, result);

            if (result <= 0)
            {
                printf("\n(YHCHO) %s() - Check Point [2]\n", __FUNCTION__);

                XASSERT(0,0);
            }
            else
            {
                if (fflush(pFile) != 0)
                {
                    XASSERT(0,0);
                }

                if (fsync(pFile->_fileno) == -1)
                {
                    XASSERT(0,0);
                }

                if (fclose(pFile) != 0)
                {
                    XASSERT(0,0);
                }
            }
        }

        T30Query( IOCTL_RX_CODING_TYPE, &coding_type, sizeof( coding_type ) );
        T30Query( IOCTL_GET_COMM_RES_RX, &rx_res, sizeof( rx_res ) );
        
        printf("\n(YHCHO) %s() - coding: %d\n", __FUNCTION__, coding_type);

		if(coding_type != CODING_JBIG)
		{
			if(T4_Decoding(jnum, PathName, TiffName, coding_type, 0, rx_res, FAX_APP_JOB_DIRECTION_RECEIVE))
			{
				gFaxRxTotalPages--;
				too_many_error = 1;
				unlink(PathName);
				isFSURxImageProgessBegin = 1;
				
				return;
			}

			if(!fsu_image_validation_check(rx_res, jnum))
			{
				gFaxRxTotalPages--;
				too_many_error = 2;
				unlink(PathName);
				unlink(TiffName);
				isFSURxImageProgessBegin = 1;

				return;
			}

			printf("\n(YHCHO) %s() - T4 Decoding Complete: %s\n", __FUNCTION__, TiffName);
			T4_encoding(gFaxRxTotalPages, TiffName, CODING_MMR , rx_res, jnum, FAX_APP_JOB_DIRECTION_RECEIVE, FAX_APP_PHOTO_MODE_IMAGE_RES_300DPI);
			printf("\n(YHCHO) %s() - T4 Encode End\n",__FUNCTION__);

			T4_end_of_page = TRUE;
		}
		else
		{
			if(sw_jbig_decode_tx(gFaxRxTotalPages, jnum, PathName, FAX_APP_JOB_DIRECTION_RECEIVE))
			{
				gFaxRxTotalPages--;
				too_many_error = 3;
				unlink(PathName);
				isFSURxImageProgessBegin = 1;

				return;
			}

			if(!fsu_image_validation_check(rx_res, jnum))
			{
				gFaxRxTotalPages--;
				too_many_error = 2;
				unlink(PathName);
				unlink(TiffName);
				isFSURxImageProgessBegin = 1;

				return;
			}

			T4_encoding(gFaxRxTotalPages, TiffName, CODING_MMR , rx_res, jnum, FAX_APP_JOB_DIRECTION_RECEIVE, FAX_APP_PHOTO_MODE_IMAGE_RES_300DPI);
			printf("\n(YHCHO) %s() - T4 Encode End\n",__FUNCTION__);
			T4_end_of_page = TRUE;
		}

#ifdef HAVE_IMAGELOG
        // Save the remote no. once and make .pdf file for image log
        if ( ( (IsImageLogEnabled('F', IMAGE_LOG_JOB_TYPE_TX) == TRUE) && (fsu_get_fax_forward(fsu_job_num_rcv) == 1) ) || ( (IsImageLogEnabled('F', IMAGE_LOG_JOB_TYPE_RX) == TRUE) && (getSaveFaxFile() == 1) ) )
        {
            if (gFaxRxTotalPages == 1)
            {
				UINT32 jobId = create_unique_job_id();
				fsu_update_unique_job_id(fsu_job_num_rcv, jobId);
				
                if ( (IsImageLogEnabled('F', IMAGE_LOG_JOB_TYPE_TX) == TRUE) && (fsu_get_fax_forward(fsu_job_num_rcv) == 1) )
                {
                    fsu_update_remote_no_for_image_log(IMAGE_LOG_JOB_TYPE_TX, fsu_job_num_rcv);
                }
                else
                {
                    fsu_update_remote_no_for_image_log(IMAGE_LOG_JOB_TYPE_RX, fsu_job_num_rcv);
                }
            }

            faxapp_make_pdf_start(NULL, fsu_job_num_rcv, gFaxRxTotalPages, 1, fsu_get_width(fsu_job_num_rcv), SAVE_FILE_PAGE_TYPE_MULTI_PAGE, REQUEST_SOURCE_FAX, FAXAPP_MAKE_PDF_REASON_IMAGE_LOG);
        }
#endif

		if (gFaxRxTotalPages == 1)
		{
			if (fsu_get_poll_receive(jnum) == 0)
			{
				Create_Job_Queue_File(jnum, FAX_APP_JOB_TYPE_RECEIVE, DocId);
			}
			else
			{
				Update_Job_Queue_File(jnum, IMAGE_BACKUP_REASON_UPDATE_RESOLUTION, fsu_get_resolution(jnum));
			}
		}

		gFaxRxCurrentPage++;

		if(fsu_get_fax_forward(jnum) == DISABLE)
		{
            if ( (getgFaxPrintMode() == FAX_PRINT_MODE_PRINT_EVERY_PAGE) && (getSaveFaxFile() == DISABLE) )
            {
                if(gFaxRxCurrentPage == 1)
                {
                    fsu_set_prn_job_ready(jnum);
                    fsu_set_next_prn_num(jnum);
                }
            }
        }

        isFSURxImageProgessBegin = 1;
	}

	/* Store the page and block numbers */
	ECMControl->RxPageNum = ECMControl->PPS_FIF[ 1 ];

	ECMControl->RxBlockNum = ECMControl->PPS_FIF[ 2 ];

    TRACE_FAX( "ECMRxPrintBlock block %d page %d\n", ECMControl->RxBlockNum, ECMControl->RxPageNum );

	/* Pass the CDBs in the block on to the next stage in the pipeline */
	/* and move on to the next block */

	ECMNextBlock();
}


/*===========================================================================
*
*    ECMRxPrintRemainingBlocks
*
*      Prints the remaining ECM blocks for the current page.
*      Used when a voice request breaks into the line
*
*      Input:          PPS_FIF = PPS FIF received
*                      ECMControl->RxPageNum = last ECM page number printed
*                      ECMControl->RxBlockNum = last ECM block number printed
*      Output:         (none)
*
*===========================================================================*/
void ECMRxPrintRemainingBlocks( void )
{
	UINT16 status = 0;

	TRACE_FAX( "Entering ECMRxPrintRemainingBlocks()...\n" );

	/* First verify that we have started the data and that
	   the user did not select to abort.
	   Also cannot continue on memory errors */
    if ( ( T30Control->Status == FAX_ERROR_USER_ABORT_ERROR )
	  || !ECMControl
	  || !ECMControl->RxTotalFrameCnt )
		return ;

	status = T30Control->Status;

	T30Control->Status = 0;

	/* Wait until done */
	while ( 1 )
	{
		OSC_Sleep( T30_DELAY_TIME );

		/* If any errors, exit */

		if ( T30RxCheckErrors() )
			break;
	}

	/* Restore the saved status */
	if ( !T30Control->Status || ( ( T30Control->Status == 1 ) && status ) )
		T30Control->Status = status;

	/* If receiving to memory, increment the page number */
	if ( T30Control->RxToMemFlg )
		T30Control->MemPageNum++;
}


/* -------------------------------------------------------------------
   After received 3 times PPR and request to fall back to lower speed
   on V.34 mode
----------------------------------------------------------------------*/
UINT16 ECMCheckV34FallBack( void )
{
	UINT16 fallback_speed;

	fallback_speed = ModemV34FallSpeed();

	if ( fallback_speed != 0 )
	{
		TRACE_FAX( " fallback speed %d bps\n", fallback_speed );
	}

	return fallback_speed;
}


/*===========================================================================
*
*    ECMRxBlock
*
*      Receives a block of ECM image data.
*
*      Input:          (none)
*      Outputs:        (none)
*
*===========================================================================*/
void ECMRxBlock( void )
{
	UINT8 training;

	TRACE_FAX( "ECMRxBlock()\n" );
    TRACE_FAX( "ECMRxBlock()..... Page %d\n", T30Control->PageNum + 1);

#if 0
	if ( ( T30Control->LastFCFSent != PPR )
	  && ( T30Control->LastFCFSent != CTR )
	  && ( T30Control->LastFCFSent != ERR ) )
	{
		prepare_rx_buffers(NUM_OF_BUFF, TRUE);
	}
#else
		prepare_rx_buffers(NUM_OF_BUFF, TRUE);
#endif

	T30Control->LastFCFSent   = 0xff;
	T30Control->rcvd_post_msg = 0xff;

	/* Start the receiver */
	training = ( T30Control->ModemSpeed < T30_SPEED_V17_7200 ) || T30Control->ECMLongTrain;

	HDLCRxDataFrame( T30Control->ModemSpeed, 0, training, ECMEndFrame, NULL );

	/* If opening flags were found, receive the frames */

	if ( ModemControl->State )
	{
		ECMRxFrames();
	}
	else
	{
		/* Get a command next. Assume we won't do long train in V.17 */
		T30Control->ECMLongTrain = 0;
		T30Control->State = STATE_RX_PHASE_D;
	}
}


/*===========================================================================
*
*    ECMRxFrames
*
*      Receives ECM image data frames
*
*      Input:          BytesPerFrame = number of bytes per frame ( 0 = 256)
*      Output:         (none)
*
*===========================================================================*/
void ECMRxFrames( void )
{
	UINT8 data;
	UINT8 fDone;
    UINT16 rx_speed = 0;
	UINT8 rcp_count;
	UINT8 fsk_count;
	UINT8 carrier_status;
	UINT8 report;
	UINT32 timeout;

	/* Reset the ECM error flag and frame counter */
	ECMControl->RxGoodFrameCnt = 0;
	rcp_count = 0;
	fsk_count = 0;
	fDone = 0;
	report = 1;

	ModemControl->EQM = 0xffff;  // request to read EQM

	/* Start Block limit timer */
	timeout = ECM_BLOCK_LIMIT_TIMEOUT;
	T30_TIMER_START( TimerD, timeout );

	ModemRxCarrierError( 0 );

	TRACE_FAX( "ECMRxFrames: Start IRQ\n" );
	TRACE_FAX( " set timeout %ldsec\n", timeout/1000 );

	ModemEnableRxInt();

	/* Loop until we have received all the frames or have aborted */
	while ( !fDone )
	{
		/* Wait on a frame or 20 msec timer. */
		OSC_Sleep( T30_DELAY_TIME );

#ifdef OEM_FAX_REPORT
		if ( ( report == 1 ) && ( ModemControl->EQM != 0xffff ) )
		{
            rx_speed = ( ModemControl->ConnectedSpeed / 1000 );
			CxProtocolReport( IMG, &rx_speed, 1, RX_LOG );
			report = 0;  // stop to report again
		}
#endif
		/*********************************/
		/*  Process received data frame  */
		/*********************************/

		/* Check for an ECM Frame ready */

		if ( HDLCRxReadStatus() == HDLC_NO_ERROR )
		{
			do {

				/* Verify frame status, length, and header */

				if ( ECMRxVerifyFrameLength() || ECMRxVerifyFrameHeader() )
				{
					HDLCRxReadNextFrame();
				}
				else if ( T30Control->Status )
				{
					T30Control->State = STATE_RX_ABORT;
					fDone = 1;
					break;
				}
				else
				{
					/* Reset the data byte pointer so we're pointing to the first byte */
					HDLCRxReadInitFrame();

					/* Read the frame type */
					data = HDLCRxReadDataByte();

					switch ( data )
					{
						case FCD:

							/* Store the frame and move on to the next frame */

							if ( !ECMRxStoreFCDFrame() )
							{
							}

							HDLCRxReadNextFrame();
							break;

						case RCP:
#ifdef OEM_FAX_REPORT
							if ( rcp_count == 0 )
							{
                                CxProtocolReport( RCP, NULL, 0, RX_LOG );
							}
#endif
							TRACE_FAX( " ( RCP )\n" );

							/* Get 3 RCP frames, then get a command */
							rcp_count++;
							HDLCRxReadNextFrame();

							if ( rcp_count == MAX_RCP_FRAMES )
							{
								TRACE_FAX( " 3 RCPs\n" );

								ModemClearIRQ();
								HDLCRxWriteInitAllFrames();
								T30Control->ECMLongTrain = 0;
								T30Control->State = STATE_RX_PHASE_D;
								fDone = 1;
							}

							/* Up to 4.5 seconds wait for frame allowed */
							T30_TIMER_START( TimerA, 4500 );
							break;

						default:

							TRACE_FAX( "Bad or unknown frame received\n" );

							HDLCRxReadNextFrame();
							break;

					} /* switch */
				} /* else */

			} while ( HDLCRxReadStatus() == HDLC_NO_ERROR );

			if ( fDone )
			{
				break;
			}
		}
		else if ( ( HDLCRxReadStatus() == HDLC_FRAME_ERROR )
		       || ( HDLCRxReadStatus() == HDLC_CHECKSUM_ERROR ) )
		{
			TRACE_FAXDEBUG( "ECMRxFrame: Frame Error %d\n", T30Control->Status );

			if ( T30Control->Status )
			{
				T30Control->State = STATE_RX_ABORT;
				fDone = 1;
				break;
			}
			else
			{
				HDLCRxReadNextFrame();
			}
		}

		/* Check for signal drop out or end of page */

		carrier_status = ModemRxCarrierError( 1 );

		if ( ( carrier_status != CARRIER_ON ) && ( fsk_count == 0 ) )
		{
			TRACE_FAX( "ECMRxFrame: Start dropout timer %d\n", carrier_status );

			if ( rcp_count > 0 )
			{
				TRACE_FAX( " Found RCP\n" );

				ModemClearIRQ();
				HDLCRxWriteInitAllFrames();
				T30Control->ECMLongTrain = 0;
				T30Control->State = STATE_RX_PHASE_D;
				fDone = 1;
				break;
			}

			/* Wait upto 6 seconds for carrier to comeback */
			T30_TIMER_START( TimerC, 6100 );

			do {

				OSC_Sleep( T30_DELAY_TIME );

				carrier_status = ModemRxCarrierError( 1 );

				if ( carrier_status == CARRIER_ON )
				{
					break;
				}
				else if ( carrier_status != CARRIER_OFF )
				{
					/* restart 6 seconds carrier off */
					T30_TIMER_START( TimerC, 6100 );
				}

                if ( !T30Control->V34Mode && ModemToneDetected( MODEM_HISPEED_FSK_TONE ) && ModemToneDetected( MODEM_FSK_TONE ) )
				{
					if ( ++fsk_count > ( 6 /*200 / T30_DELAY_TIME*/ ) )  // 200ms
					{
						TRACE_FAX( "ECMRxFrames: 1st FSK detected\n" );
						break;
					}
				}
				else
				{
					fsk_count = 0;
				}

				if ( T30_TIMER_EXPIRE( TimerC ) )
				{
					TRACE_FAX( "ECMRxFrame: No Carrier\n" );

					ModemClearIRQ();
					ModemControl->State = MODEM_IDLE;
					HDLCRxWriteInitAllFrames();
					T30Control->ECMLongTrain = 0;
                    T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_1;
					T30Control->State = STATE_RX_ABORT;
					fDone = 1;
					break;
				}

				if ( T30_TIMER_EXPIRE( TimerD ) )
				{
					break;
				}

				if ( T30Control->Status )
				{
					TRACE_FAX( "ECMRxFrame: Error/Abort\n" );

					ModemClearIRQ();
					T30Control->State = STATE_RX_ABORT;
					fDone = 1;
					break;
				}

			} while ( carrier_status );

			if ( fDone )
			{
				break;
			}
			else
			{
				TRACE_FAX( "ECMRxFrame: Stop dropout timer %d\n", carrier_status );
			}
		}

		/* Check abort or any errors */

		if ( T30RxCheckErrors() )
		{
			TRACE_FAX( "ECMRxFrame: T30 Rx Error %d\n", T30Control->Status );

			ModemClearIRQ();
			T30Control->State = STATE_RX_DISCONN;
			fDone = 1;
			break;
		}

		/* Check ECM block time */

		if ( T30_TIMER_EXPIRE( TimerD ) )
		{
			TRACE_FAX( "ECMRxFrame: TimerD timeout\n" );

			ModemClearIRQ();
			ModemControl->State = MODEM_IDLE;
			HDLCRxWriteInitAllFrames();
			T30Control->ECMLongTrain = 0;
            T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_2;
			T30Control->State = STATE_RX_ABORT;
			fDone = 1;
			break;
		}

		if ( T30Control->V34Mode )
		{
			/* Check CC mode */

			if ( ModemV34CheckCC() )
			{
				TRACE_FAX( "ECMRxFrame: Out of PC mode\n" );

				ModemClearIRQ();
				ModemControl->State = MODEM_IDLE;
				HDLCRxWriteInitAllFrames();
				T30Control->ECMLongTrain = 0;
				T30Control->State = STATE_RX_PHASE_D;
				fDone = 1;
				break;
			}
		}
		else  // V.17
		{
			/* Check FSK signal */

            if ( ModemToneDetected( MODEM_HISPEED_FSK_TONE ) && ModemToneDetected( MODEM_FSK_TONE ) )
			{
				if ( ++fsk_count > ( 10 /*300 / T30_DELAY_TIME*/ ) )  // 300ms
				{
					TRACE_FAX( "ECMRxFrames: FSK detected\n" );

					ModemClearIRQ();
					HDLCRxWriteInitAllFrames();
					T30Control->ECMLongTrain = 0;
					T30Control->State = STATE_RX_PHASE_D;
					fDone = 1;
					break;
				}
			}
			else
			{
				fsk_count = 0;
			}
		}
	} /* while */

	TRACE_FAX( "ECMRxFrames: STOP next state %d\n", T30Control->State );

	ModemDisableToneDet( MODEM_HISPEED_FSK_TONE );
}

/*===========================================================================
*
*    ECMRxVerifyFrameLength
*
*      Verifies frame length
*
*      Input:          current frame
*      Output:         return 1 if the frame is too short, 0 otherwise
*
*===========================================================================*/
UINT8 ECMRxVerifyFrameLength( void )
{
	if ( !HDLCRxReadLength() )
	{
		TRACE_FAX( "ECM zero frame length\n" );
		return ( 1 );
	}
	else
		return ( 0 );
}


/*===========================================================================
*
*    ECMRxVerifyFrameHeader
*
*      Verifies the frame header
*
*      Input:          current frame
*      Output:         return value = 1 if error
*                      return value = 0 if no error
*
*===========================================================================*/
UINT8 ECMRxVerifyFrameHeader( void )
{
	/* Bad address field */

	if ( HDLCRxReadAddress() != 0xFF )
	{
		TRACE_FAX( "ECM bad address.\n" );
		return ( 1 );
	}

	/* Bad control field */
	if ( HDLCRxReadControl() != 0x03 )
	{
		TRACE_FAX( "ECM bad control field.\n" );
		return ( 1 );
	}

	return ( 0 );
}


/*===========================================================================
*
*    ECMRxStoreFCDFrame
*
*      Transfers image data from the HDLC buffer to the ECM buffers
*
*      Input:          FrameByteCtr = how many bytes left in the frame
*      Output:         0 if no error, 1 if there is
*                      RxGoodFrameCnt = number of frames - 1 received in this block
*
*===========================================================================*/
UINT8 ECMRxStoreFCDFrame( void )
{
	UINT8 frame_number;
	UINT8 bit_mask;
	UINT8 *pBuff;

	/* Make sure there's data left */

	if ( HDLCRxReadLength() < 2 )
	{
		TRACE_FAX( "ECM Frame size < 2.\n" );
		return ( 1 );
	}

	if(!pBuffer_front[0])
		TRACE_FAX("=====> pBuffer_front is NULL !!!\n");

	if(!pBuffer_front[0]->data)
		TRACE_FAX("=====> pBuffer_front->data is NULL !!!\n");

	ECMControl->NetBlockCount = 1;          // indicate there is frame

	/* Read the frame number */
	frame_number = HDLCRxReadDataByte();

	if (pBuffer_front[frame_number])
	{
		// get a writing pointer in a pBuff
		pBuff = (UINT8 *)pBuffer_front[frame_number]->data;

		/* Bump the total frame count if this frame is higher than any earlier ones */
		if ( frame_number > ECMControl->RxTotalFrameCnt )
			ECMControl->RxTotalFrameCnt = frame_number;

		//fcd_length = HDLCRxReadDataToBuff(pBuff);
		ECMControl->RxFrameSize[ frame_number ] = HDLCRxReadDataToBuff( pBuff );    // used to calculate toal length in pBuff
		pBuffer_front[frame_number]->used = ECMControl->RxFrameSize[ frame_number ];

		/* Clear the PPR_FIF bit since this is a good frame */
		bit_mask = 1 << ( frame_number % 8 );

		CLEARBIT( ( UINT8 * ) & ( ECMControl->PPR_FIF[ frame_number / 8 ] ), bit_mask );

		/* Increment the number of good frames received for this block */
		ECMControl->RxGoodFrameCnt++;

		TRACE_FAX( "%3d, ", frame_number);
		if(!(frame_number % 10) && frame_number)
			TRACE_FAX( "\n ");

		return ( 0 );
	}
	else
	{
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_ERROR_TRANSFERS_IMAGE_DATA_FROM_THE_HDLC_BUFFER);

		return ( 1 );
	}
}


/*===========================================================================
*
*    ECMRxStartPrint
*
*      Starts printing if not already printing during receive Phase D ECM mode
*      Otherwise sends RNR to receiver (if not printing the last block)
*           indicating that we are busy.
*
*      Input:          none
*      Output:         return value = 0 if we weren't printing
*                      return value = 1 if we were printing and not the last block
*
*===========================================================================*/
UINT8 ECMRxStartPrint( void )
{
	TRACE_FAX( "ECMRxStartPrint NetBlockCount %d\n", ECMControl->NetBlockCount );

	/* If we just sent RNR then we're responding to RR in which case */
	/* the pipeline is already running so don't start it up again */

    if ( ( T30Control->LastFCFSent == RNR ) || ( T30Control->LastFCFSent == MCF ) )
	{
		return ( E_OK );
	}
	else
	{
		ECMRxPrintBlock();

		/* Initialize in case next block has to be handled without the PPS
		   due to line being dropped */

		ECMControl->PPS_FIF[ 2 ]++;

		// if cmd is not PPS_NULL

		if ( ECMControl->PPS_FIF[ 0 ] & 0xEE )
		{
			/* Last block for the page was handled */
			ECMControl->PPS_FIF[ 1 ]++;   // page counter
			ECMControl->PPS_FIF[ 2 ] = 0;  // block counter
		}
	}

	TRACE_FAX( "ECMRxStartPrint return E_OK\n" );
	return ( E_OK );
}


/*===========================================================================
*
*    ECMRxEndOfBlockResponse
*
*      Responds to the transmitter during ECM Phase D End of Block
*      Used when PPS or RR is received
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void ECMRxEndOfBlockResponse( void )
{
	static UINT8 last_post_message;

	TRACE_FAX( "Entering ECMRxEndOfBlockResponse().....\n" );

	/* If we are already printing and not the last block, we send RNR to receiver and exit */

	if ( ECMRxStartPrint() == E_OK )
	{
		T30Control->PPRSendCnt = 0;     // THY

		if (too_many_error)
		{
			TRACE_ERROR( "ECMRxEndOfBlockResponse(): Too many decompression errors %d\n", too_many_error);

			if(too_many_error == 1)
            {
				T30Control->Status = FAX_ERROR_BAD_RX_ERROR_2_2;
            }
            else if(too_many_error == 2)
            {
				T30Control->Status = FAX_ERROR_INVALID_HEIGHT_2;
            }
            else
            {
				T30Control->Status = FAX_ERROR_JIBG_DECODE_ERROR_2;
			}
            too_many_error = 0;

            return;
		}

		if ( ECMControl->PPS_FIF[ 0 ] == NUL )
		{
			/* PPS_NULL */
			TRACE_FAX( "ECMRxEndOfBlockResponse: PPS_NULL Send ( MCF )\n" );

			T30SendFCF( MCF );
		}
		else if ( ECMControl->PPS_FIF[ 0 ] & 0x10 )
		{
			/* PPS-PRIQ */
			TRACE_FAX( "ECMRxEndOfBlockResponse: proceed PRIQ\n" );

			ECMRxProcessPPS_PRIQ();
		}
		else if ( t30_attribute.FaxVoiceRequest
		       && !t30_attribute.IgnoreVoiceRequest )
		{
			/* Voice request from local operator */
			TRACE_FAX( "ECMRxEndOfBlockResponse: Send ( PIP )\n" );

			T30SendFCF( PIP );
		}
		else
		{
            UINT8 cnt = 10;

			/* Otherwise, just send MCF or RNR */
			/* AKW - Potentially, an InitPage could be done while the pipeline */
			/* is running with data for the previous page - watch out for this. */

			if ( ( T30Control->LastFCFRcvd & 0xEE ) != RR )
			{
				T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xEE;
				last_post_message = T30Control->rcvd_post_msg;
			}

            while ( 1 )
			{
                if ( (cnt == 0) || (T4_end_of_page == TRUE) )
                {
					break;
                }

                cnt--;

                OSC_Sleep( 200 );
			}

            if ( T4_end_of_page == FALSE )
			{
				TRACE_FAX( "ECMRxEndOfBlockResponse: Send ( RNR ) for %02X\n", last_post_message );

				T30SendFCF( RNR );

                RNRSentCount++;

				T30Control->State = STATE_RX_PHASE_D;
			}
			else
			{
				if ( last_post_message == MPS )
				{
					if ( T30Control->LastFCFSent != MCF )
					{
						TRACE_FAX( "==============> call fsu_rcv_next_page() !!!!!\n" );

                        if(fsu_rcv_next_page() == FALSE)
						{
                            T30Control->Status = FAX_ERROR_NO_MEM_ERROR;
							return;
						}
					}
				}

				// when T4 completes decoding
				TRACE_FAX( "ECMRxEndOfBlockResponse: Send ( MCF ) for %02X\n", last_post_message );

				T30SendFCF( MCF );

                RNRSentCount = 0;

				// 12/22/10 to add prn job on fsu_task 	 2010_LAST
				//set_fsu_state( FSU_STATE_RX_PHASE_D );

				if ( last_post_message == EOP )
				{
					T30Control->State = STATE_RX_PHASE_D;

                    de_activate_rcv_to_mem();
				}
				else if ( last_post_message == EOM )
				{
					if ( T30Control->V34Mode )
                    {
						T30Control->State = STATE_RX_PHASE_B;
                    }
					else
                    {
						T30Control->State = STATE_RX_PHASE_D_SIGNAL_REC;
                    }
				}

				last_post_message = 0xff;
			}
		}
	}
}


/*===========================================================================
*
*    ECMRxProcessPPS
*
*      Processes a PPS received during receive Phase D ECM mode
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void ECMRxProcessPPS( void )
{
    TRACE_FAX( "ECM Rx ( PPS - %02X )", ECMControl->PPS_FIF[ 0 ] & 0xFE );
    TRACE_FAX( "ECMControl->RxPageNum: %d\n", ECMControl->RxPageNum );

	T30Control->LastFCFRcvd = ECMControl->PPS_FIF[ 0 ];
	T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xEE;

	/* Repeated command */
    if ( ( ECMControl->PPS_FIF[ 1 ] == ECMControl->RxPageNum ) && ( ECMControl->PPS_FIF[ 2 ] == ECMControl->RxBlockNum ) )
	{
		/* Check for PRI bit set and handle accordingly */

		if ( ECMControl->PPS_FIF[ 0 ] & 0x10 )
		{
			ECMRxProcessPPS_PRIQ();
		}
		else
		{
			if ( T30Control->LastFCFSent == RNR )
			{
				ECMRxEndOfBlockResponse();
			}
			else
			{
				/* Otherwise, just repeat the response */
				T30SendFCF( T30Control->LastFCFSent );

				if ( T30Control->LastFCFSent == MCF )
				{
					if ( T30Control->rcvd_post_msg == EOP )
					{
						T30Control->State = STATE_RX_PHASE_D;
					}
					else if ( T30Control->rcvd_post_msg == EOM )
					{
						if ( T30Control->V34Mode )
                        {
							T30Control->State = STATE_RX_PHASE_B;
                        }
						else
                        {
							T30Control->State = STATE_RX_PHASE_D_SIGNAL_REC;
                        }
					}
				}
			}
		}
	}
	else
	{
		/* No repeated command */

		T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xEE;

		/* Check for ECM errors and send PPR if so */

		ECMRxCheckCopyQuality();

		if ( ECMControl->PPRCnt )
		{
			T30SendFCF( PPR );
			T30Control->PPRSendCnt++;
		}
		else
		{
			/* Otherwise, decode and process the response */

			ECMRxEndOfBlockResponse();
		}
	}
}


/*===========================================================================
*
*    ECMRxProcessRR
*
*      Processes a RR received during receive Phase D ECM mode
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void ECMRxProcessRR( void )
{
	TRACE_FAX( "ECM Rx ( RR )\n" );

	/* Allow some time for the processing and then check */
	OSC_Sleep( 400 );

	/* Respond to the transmitter with RNR, MCF or PIP */
	ECMRxEndOfBlockResponse();
}


/*===========================================================================
*
*    ECMRxProcessCTC
*
*      Processes a CTC received during receive Phase D ECM mode
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void ECMRxProcessCTC( void )
{
	TRACE_FAX( "ECM Rx ( CTC )\n" );

	/* For Non-V.34 mode read the CTC FIF */
	if ( !T30Control->V34Mode )
		T30RxReadDCS();

	/* Respond with CTR */
	T30SendFCF( CTR );

	/* Do long train next, if V.17 */
	T30Control->ECMLongTrain = 1;
}


/*===========================================================================
*
*    ECMRxProcessEOR
*
*      Processes a EOR received during receive Phase D ECM mode
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void ECMRxProcessEOR( void )
{
	TRACE_FAX( "ECM Rx ( EOR - %02X )\n", ECMControl->PPS_FIF[ 0 ] & 0xFE );

	T30Control->LastFCFRcvd = ECMControl->PPS_FIF[ 0 ];
	T30Control->rcvd_post_msg = T30Control->LastFCFRcvd & 0xEE;

	/* If we got EOR after ERR, it's a repeat */
	if ( T30Control->LastFCFSent == ERR )
	{
		T30SendFCF( T30Control->LastFCFSent );
	}
	else if ( T30Control->LastFCFSent == RNR )
	{
		ECMRxEndOfBlockResponse();
	}
	else
	{
		if ( ECMControl->PPS_FIF[ 0 ] & 0x10 )
		{
			ECMRxProcessPPS_PRIQ();
		}
		else if ( t30_attribute.FaxVoiceRequest
		       && !t30_attribute.IgnoreVoiceRequest )
		{
			/* Voice request from local operator */
			TRACE_FAX( " Send ( PIP )\n" );

			T30SendFCF( PIP );
		}
		else
		{
			switch ( T30Control->rcvd_post_msg )
			{
				case 0:  // NULL

					TRACE_FAX( " Send ( ERR )\n" );

					ECMControl->PPRCnt = 0;
					T30SendFCF( ERR );
					break;

#if 0  // not supported yet
				case MPS:
				case EOP:
				case EOM:

					if ( ECMRxStartPrint() == E_OK )
					{
						TRACE_FAX( " Send ( RNR )\n" );

						T30SendFCF( RNR );
					}
					else
					{
						ECMControl->PPRCnt = 0;
						T30SendFCF( ERR );
					}
					break;
#endif

				default:

					T30Control->State = STATE_RX_PHASE_E;
                    T30Control->Status = FAX_ERROR_BAD_RX_ERROR_4;
					break;
			}
		}
	}
}


/*===========================================================================
*
*    ECMRxProcessPPS_PRIQ
*
*      Processes a PPS_PRIQ received during receive Phase D ECM mode
*
*      Input:          none
*      Output:         none
*
*===========================================================================*/
void ECMRxProcessPPS_PRIQ( void )
{
	UINT8 status;

	TRACE_FAX( "ECM Rx ( PPS_PRIQ )\n" );

	T30Control->PRIQCnt--;

	/* Alert local operator and check for a line request */
	status = T30AlertLocalOp();

	/* Quit if we initiated or line request or both */
	if ( status || t30_attribute.FaxVoiceRequest )
	{

		/* Don't send PIP if we didn't get a line request */

		if ( t30_attribute.FaxLineRequest )
		{
			/* Send PIP to transmitter and print the remining blocks in the page */
			T30SendFCF( PIP );
			ECMRxPrintRemainingBlocks();
		}

		T30Control->State = STATE_RX_PHASE_E;
	}
	else
	{
		/* If 3 PRI-Qs or if ignoring voice requests,
		   send MCF and do signal recognition */

		if ( !T30Control->PRIQCnt || t30_attribute.IgnoreVoiceRequest )
		{
			T30SendFCF( MCF );
			T30Control->State = STATE_RX_PHASE_D_SIGNAL_REC;
		}

		/* Otherwise, get another command */
		else
			T30Control->State = STATE_RX_PHASE_D;
	}
}


/*===========================================================================
*
*    ECMTxBlock
*
*      Transmit a block of image data in ECM mode.
*
*      Input:          PPR_FIF = 32 byte status on which frames need to be TX'd.
*                       Assumes 256 frame blocks.  A '1' means to TX, '0' means skip.
*                       LS bit of LS byte corresponds to frame 0.
*      Output:          RxGoodFrameCnt = number of frames - 1 sent in this block
*
*===========================================================================*/
void ECMTxBlock( void )
{
	UINT8 frame;
	UINT8 last_frame = 0;
	UINT8 training;
	UINT32 timeout;

	TRACE_FAX( "Entering ECMTxBlock().....\n" );

	/* If this is a new page and we're not retrying, init for the page */
	if ( !ECMControl->TxBlockNum && !ECMControl->PPRCnt )
	{
		T30TxInitPage();
	}

	/* Initialize the frame counter. (Count is always - 1) */
	ECMControl->TxTotalFrameCnt = 0xFF;

	/* Set the first frame flag */
	ECMControl->TxFirstFrame = 1;

	/* Clear out all of the frames */
	HDLCTxWriteInitAllFrames();

	/* Start the transmission of flags while waiting for
	   data to become available */
	training = T30Control->ModemSpeed < T30_SPEED_V17_7200 || T30Control->ECMLongTrain;

	HDLCTxInitialFlags( T30Control->ModemSpeed, training );

	if ( ECMControl->PPRCnt )
		OSC_Sleep( 300 );  // add flag time in case of PPR

	ECMControl->TxFirstFrame = 0;

	if ( T30TxCheckErrors() )
    {
    	ModemRTSConfig( 0 );
		return ;
    }

	/* Start Block limit timer */
	timeout = ECM_BLOCK_LIMIT_TIMEOUT;
	T30_TIMER_START( TimerD, timeout );

	/* For the first pass, wait on MCBs */

	frame = 0;		//add.bong.2012-09-05 8:04pm

	if ( !ECMControl->PPRCnt )
	{
		while ( !last_frame )
		{
			/* Check for any errors */
			if ( T30TxCheckErrors() )
			{
				TRACE_FAX("ECMTxBlock: Queue Error or Abort Key\n");
				break;
			}

			if ( T30_TIMER_EXPIRE( TimerD ) )
			{
                T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_4;
				TRACE_FAX( "ECM Block Timeout Error\n" );
				break;
			}

			/* Were there any errors/timeout/abort without the frame ready ? */

			if ( T30Control->Status )
			{
				TRACE_FAX( "ECMTxBlock: Abort Key\n" );
				break;
			}

			/* Final frame is either last frame of the block or end of page */
			ECMControl->TxEndOfPage = ECMCDBCheckEndOfPage( frame );

			last_frame = ( frame == ( MAX_ECM_FRAMES - 1 ) ) || ECMControl->TxEndOfPage;

			ECMTxFCDFrame( frame );
			ECMControl->TxTotalFrameCnt++;
			frame++;	//add.bong.2012-09-07 1:49pm
		}	// while()
	}

	/* We're retrying, so just go through all the frames */
	else
	{
		for ( frame = 0; !last_frame; frame++ )
		{
			/* Check for abort */

			if ( T30TxCheckErrors() )
				break;

			/* Final frame is either last frame of the block or end of page */
			ECMControl->TxEndOfPage = ECMCDBCheckEndOfPage( frame );

			last_frame = ( frame == ( MAX_ECM_FRAMES - 1 ) ) || ECMControl->TxEndOfPage;

			/* Don't send the frame if the PPR bit is good (i.e., it was received */
			/* by the remote correctly) */
			if ( ECMCheckPPR( frame ) )
			{
				ECMTxFCDFrame( frame );
				ECMControl->TxTotalFrameCnt++;
			}
		}	// for()
	}		// if ( !ECMControl->PPRCnt )

	if ( T30Control->Status )
    {
    	ModemRTSConfig( 0 );
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
		/* Now send the 3 RCPs to mark the end of the block */
		ECMTxRCPFrames();
		/* Turn off the transmitter and prepare for the next state */
		HDLCTxWriteInitAllFrames();
		ModemRTSConfig( 0 );
		T30Control->State = STATE_TX_PHASE_D;
		T30Control->ECMLongTrain = 0;
	}
}


/*===========================================================================
*
*    ECMTxFCDFrame
*
*      Transmits one FCD frame of image data in ECM mode.
*
*      Input:          frame number = frame number to transmit
*                      T30Control->ModemSpeed = modem speed table index
*      Output:         ECM frame written to HDLC buffer
*
*===========================================================================*/
void ECMTxFCDFrame( UINT8 frame_number )
{
    UINT8 training = 0;
    CDB_BUFFER *cdb = (CDB_BUFFER *)NULL;

	/* Point to the next frame */
	HDLCTxWriteNextFrame();

	/* Store the control field (non-final frame), FCD, and frame number in the HDLC buffer */
	HDLCTxWriteControl( 0x03 );
	HDLCTxWriteDataByte( FCD );
	HDLCTxWriteDataByte( frame_number );

	cdb = ECMControl->Block[ ECMControl->BlockFront ].Frame[ frame_number ];

    if (cdb == (CDB_BUFFER *)NULL)
    {
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_ERROR_TRANSMITS_ONE_FCD_FRAME_OF_IMAGE_DATA);
    }

	out_frame_num = frame_number;

    TRACE_FAX( " %d ", frame_number );

    if(!(frame_number % 10))
    {
        TRACE_FAX("\n");
    }

	/* Store the data in the HDLC buffer */
	HDLCTxWriteDataFromCDB( cdb );

	ECMControl->FrameDone = FALSE;

	/* Start the transmitter */
	training = T30Control->ModemSpeed < T30_SPEED_V17_7200 || T30Control->ECMLongTrain;

	T30_TIMER_START( TimerB, 4500 );

	HDLCTxDataFrame( T30Control->ModemSpeed, training, ECMControl->TxFirstFrame,
	                 ECMEndFrame );

	/* Clear the first frame flag */
	ECMControl->TxFirstFrame = 0;

	/* Wait for the end of frame */
	while ( !ECMControl->FrameDone )
	{
		OSC_Sleep( T30_DELAY_TIME );
		/* Check for any errors */
		if ( T30TxCheckErrors()
		  || T30_TIMER_EXPIRE( TimerB ) )
		{
			ModemClearIRQ();
			TRACE_FAX( "ECMTxFCDFrame: abort detected!!\n" );
			break;
		}
	}
}


/*===========================================================================
*
*    ECMTxRCPFrames
*
*      Transmits 3 RCP frames at the end of the block
*
*      Input:          T30Control->ModemSpeed = modem speed table index
*      Output:         ECM frame written to HDLC buffer
*
*===========================================================================*/
void ECMTxRCPFrames( void )
{
	UINT8 i;
	UINT8 training;

	/* Send each of the RCP frames. */

	for ( i = 0; ( i < MAX_RCP_FRAMES ) && !T30Control->Status; i++ )
	{
		/* Point to the next frame */
		HDLCTxWriteNextFrame();

		/* Store the control field, and RCP in the HDLC buffer */
		HDLCTxWriteControl( 0x03 );
		HDLCTxWriteDataByte( RCP );

#ifdef OEM_FAX_REPORT
		if ( i == 0 )
		{
            CxProtocolReport( RCP, NULL, 0, TX_LOG );
		}
#endif

		TRACE_FAX( " ( RCP )\n" );

		/* If this is the 3rd RCP frame, mark it as final frame. */
		if ( i == ( MAX_RCP_FRAMES - 1 ) )
			HDLCTxWriteFinalFrame();

		ECMControl->FrameDone = FALSE;

		/* Start the transmitter */
		training = T30Control->ModemSpeed < T30_SPEED_V17_7200 || T30Control->ECMLongTrain;

		HDLCTxDataFrame( T30Control->ModemSpeed, training, ECMControl->TxFirstFrame,
		                 ECMEndFrame );

		/* Clear the first frame flag */
		ECMControl->TxFirstFrame = 0;

		T30_TIMER_START( TimerB, 4500 );

		/* Wait for the end of frame */
		while ( !ECMControl->FrameDone )
		{
			OSC_Sleep( T30_DELAY_TIME );

			/* Check for any errors other than user abort */
			if ( ( T30TxCheckErrors()
                && T30Control->Status != FAX_ERROR_USER_ABORT_ERROR )
			  || T30_TIMER_EXPIRE( TimerB ) )
			{
				TRACE_FAX( "ECMTxRCPFrames: abort detected!!\n" );
				ModemClearIRQ();
				break;
			}
		}
	}

	if ( !ModemControl->V34Mode )
	{
		OSC_Sleep( 20 );
	}
}


/*===========================================================================
*
*    ECMTxSetPPS
*
*      Prepares the PPS for the remote receiver
*
*      Input:   fRetransmit : 1 if retransmitting, 0 if not
*      Output:
*
*===========================================================================*/
void ECMTxSetPPS( UINT8 fRetransmit )
{
	TRACE_FAX( "ECMTxSetPPS(): %d\n", fRetransmit );

	/* If this is the first time the block was sent (i.e, not retransmitting) */
	/* prepare all fields in the ECM control structure */
	if ( !fRetransmit )
	{
		/* Store the page and block number */
		ECMControl->PPS_FIF[ 1 ] = T30Control->PageNum - 1;
		ECMControl->PPS_FIF[ 2 ] = ECMControl->TxBlockNum;

		/* Not at end of page */

		if ( !ECMControl->TxEndOfPage )
		{
			/* Bump the block number and prepare to send PPS-NULL */
			ECMControl->TxBlockNum++;
			ECMControl->PPS_FIF[ 0 ] = 0;
			T30Control->PostMsgCmd = 3;

			TRACE_FAX( "Not at the end of page - sending PPS_NULL\n" );
		}

		/* T4Tx at end of page */
		else
		{
			TRACE_FAX( "At end of page - sending PPS_MPS/EOP\n" );

			ECMControl->TxBlockNum = 0;
			ECMControl->TxEndOfPage = 0;
			T30Control->MemPageNum++;

			/* OR in the 'DIS received' bit (always for ECM) */
			ECMControl->PPS_FIF[ 0 ] = T30Control->FCFToSend | 0x01;
		}
	}

	/* Set the frame counter and FCF to send */
	ECMControl->PPS_FIF[ 3 ] = ECMControl->TxTotalFrameCnt;

	T30Control->FCFToSend = PPS;
}


/*===========================================================================
*
*    ECMTxProcessPPR
*
*      Responds to a PPR from the receiver
*
*      Input:          PPRCnt = number of times we've received PPR for this block
*      Output:         next state of T.30
*
*===========================================================================*/
T_T30_STATES ECMTxProcessPPR( void )
{
	UINT8 frame, last_frame = 0, no_error_frame;

	TRACE_FAX( "ECMTx: Retransmit frames\n" );

	/* Transmit to HP570 under noise conditions resulted in receiving
	   a PPR indicating no wrong frames.  Handle these problems as a
	   case to stop retrying */
	last_frame = 0;

	no_error_frame = 1;

	for ( frame = 0; !last_frame; frame++ )
	{
		last_frame = ( frame == ( MAX_ECM_FRAMES - 1 ) )
		             || ECMCDBCheckEndOfPage( frame );

		if ( ECMCheckPPR( frame ) )
			no_error_frame = 0;
	}

	if ( no_error_frame )
	{
		/* Send EOR to the remote */
		T30Control->FCFToSend = EOR;
		return ( STATE_TX_PHASE_D2 );
	}

	ECMControl->PPRCnt++;

	/* Try falling back and resending */
	if ( ModemControl->V34Mode )
	{
		if ( ( ECMControl->PPRCnt % 3 ) == 0 )
		{
			if ( !T30TxFallback() )
			{
				/* Send EOR to the remote */
				T30Control->FCFToSend = EOR;
				return ( STATE_TX_PHASE_D2 );
			}
		}
	}
	else
	{
		if ( ( ECMControl->PPRCnt % PPR_FOR_FALLBACK ) == 0 )
		{
			if ( !T30TxFallback() )
			{
				/* Send EOR to the remote */
				T30Control->FCFToSend = EOR;
			}
			else
			{
				T30TxMakeDCS();
				T30Control->FCFToSend = CTC;
				T30Control->ECMLongTrain = 1;
			}

			return ( STATE_TX_PHASE_D2 );
		}
	}

	OSC_Sleep( 20 );
	return ( STATE_TX_PHASE_C_ECM );
}


/*===========================================================================
*
*    ECMTxProcessRNR
*
*      Sends "Receiver Ready" in response to "Receiver Not Ready"
*
*      Input:          (none)
*      Output:         T30Control->LastFCFSent = RR if we've received RNR before
*                      Starts the T5 timer the first time through
*
*===========================================================================*/
T_T30_STATES ECMTxProcessRNR( void )
{
	/* If this is the first RNR, start the T5 timer */
	if ( T30Control->LastFCFSent != RR )
	{
        T30_TIMER_START( TimerB, gFaxT5Timeout );
	}

	/* If we didn't time out, send RR back to the remote */
	if ( T30_TIMER_EXPIRE( TimerB ) )
	{
		TRACE_FAX( "ECMTx: Send DCN by T5 time out\n" );

        T30Control->Status = FAX_ERROR_TIME_OUT_ERROR_5;
		return ( STATE_TX_PHASE_E );
	}
	else
	{
		TRACE_FAX( "ECMTx: Send RR\n" );

		T30Control->FCFToSend = RR;
		return ( STATE_TX_PHASE_D2 );
	}
}


/*===========================================================================
*
*    ECMTxPhaseDProcessResponse
*
*      Processes a response received during trasnmit Phase D ECM Mode
*
*      Input:          response from remote receiver
*                      response index into next state table
*                      T30Control->PostMsgCmd: 0 = EOP, 1 = MPS, 2 = EOM, 3 = NULL
*      Output:         T.30 next state
*
*===========================================================================*/
T_T30_STATES ECMTxPhaseDProcessResponse( UINT8 response, UINT8 table_index )
{
	T_T30_STATES next_state;

	TRACE_FAX( "ECMTxPhaseDProcessResponse([ %02X ], %d) Entering.....\n", response, table_index );

	/* Set the next state */
	switch ( response )
	{
		case PPR:
        {
			TRACE_FAX( " Got PPR\n" );

			next_state = ECMTxProcessPPR();
			break;
        }

		case RNR:
        {
			TRACE_FAX( " Got RNR\n" );

			next_state = ECMTxProcessRNR();
			break;
        }

        /* The block is finished if we receive MCF or ERR */
        /* so move on to the next block */

		case MCF:
        {
			TRACE_FAX( " Got MCF\n" );

            RTNCount = 0;

			if(T30Control->PostMsgCmd != 3)				// PPS_NULL = 3
			{
                TRACE_FAX("=======>ECMTxPhaseDProcessResponse: send event FAXAPP_T30_EVENT_TX_COMPLETE\n");

                faxapp_set_event_flags(FAXAPP_T30_EVENT_TX_COMPLETE);

				set_fsu_state(FSU_STATE_TX_GOT_MCF);		// release dmb buffer in fipp
			}

			if ( ( T30Control->PostMsgCmd == 0 ) || ( T30Control->PostMsgCmd == 2 ) )
			{
				next_state = TxPhaseDNextState[ table_index ][ T30Control->PostMsgCmd ];
				break;
			}

			ECMNextBlock();
			next_state = TxPhaseDNextState[ table_index ][ T30Control->PostMsgCmd ];
			break;
        }

		case ERR:
        {
			TRACE_FAX( " Got ERR\n" );

			ECMNextBlock();
			next_state = TxPhaseDNextState[ table_index ][ T30Control->PostMsgCmd ];
            T30Control->Status = FAX_ERROR_RECEIVE_ERR_RESPONSE;
			break;
        }

		default:
        {
			next_state = TxPhaseDNextState[ table_index ][ T30Control->PostMsgCmd ];
			break;
        }
	}

	TRACE_FAX( "ECMTxPhaseDProcessResponse() next_state %d\n", next_state );

	return ( next_state );
}


/*===========================================================================
*
*    ECMSendBlockOnError
*
*      Check ECM block to transfer to Modem Job and send the block if not
*
*      Input:   None
*
*      Output:  None
*
*===========================================================================*/
void ECMSendBlockOnError( void )
{
	if ( ECMControl->NetBlockCount )
		ECMRxPrintBlock();
}

