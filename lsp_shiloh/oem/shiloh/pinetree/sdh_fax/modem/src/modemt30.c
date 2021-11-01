/*
 *  $RCSfile: modemt30.c,v $
 * $Revision: 1.37 $
 *   $Author: miurat $
 *     $Date: 2012/02/29 02:35:30 $
 */
/************************************************************************
 *
 *      modemt30.c
 *
 *  Modem T30 driver.
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
#include "T30X.H"
#include "ECMX.H"
#include "ECM.H"
#include "HDLC.H"
#include "modem336.h"
#include "SDHFaxCommonHeader.h"
#include "fax_memAPI.h"
#include "faxapp_notify.h"
#include "../../image_backup/include/image_backup.h"

UINT16 cdbcount = 0;
UINT32 last_EOL_time;

#define MODEM_CED_TONE_MASK     0x40  /* TONEB for CED tone */

extern void prepare_rx_buffers(UINT16 buffers, BOOL ecm_mode);
extern JOBNODE_BUFFER *pBuffer_front[NUM_OF_BUFF];
extern CDB_BUFFER cdb_buffer[MAX_ECM_BLOCKS][MAX_ECM_FRAMES];
extern UINT16 frame_num, tx_blk_num;
#ifdef HAVE_IMAGELOG
extern BOOLEAN IsImageLogEnabled(char jobtype, int subJobType);
extern UINT32 create_unique_job_id( void );
extern void fsu_update_unique_job_id (UINT16 jnum, UINT32 jobId);
#endif
extern int T4_Decoding(UINT16 jnum, char *FileName, char *out_FileName, int codingtype, int width_type, int rx_resolution, UINT8 Direction);
extern UINT8 fsu_get_fax_forward (UINT16 jnum);
extern UINT8 T30RxCheckErrors(void);
extern UINT8 fsu_get_poll_receive (UINT16 jnum);
extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT8 fsu_get_width(UINT16 jnum);
extern UINT16 fsu_get_doc_id(UINT16 jnum);
extern int sw_jbig_encode_tx (int Page, char *FileName, UINT16 jnum, UINT8 Direction);
extern int sw_jbig_decode_tx(int Page, UINT16 jnum, char *FileName, UINT8 Direction);
extern int T4_encoding(int Page, char *FileName, int codingtype, int faxRes, int job_num_send, UINT8 Direction, int photo_image_resolution);
extern void fsu_set_prn_job_ready(UINT16 jnum);
extern void fsu_set_next_prn_num(UINT16 next_prn_num);
extern void Create_Job_Queue_File(UINT16 jnum, FAX_APP_JOB_TYPE_t FaxAppJobType, UINT16 DocId);
extern void faxapp_make_pdf_start (const char *Path, int jnum, int StartPageNo, int TotalPageNo, int image_width, int PageType, int Request, int Reason);
#ifdef HAVE_IMAGELOG
extern void fsu_update_remote_no_for_image_log (UINT8 type, UINT16 jnum);
#endif
extern int getSaveFaxFile(void);
extern int getgFaxPrintMode(void);
extern int fsu_image_validation_check (int res, int jnum);

extern int gFaxRxTotalPages;
extern UINT16 fsu_job_num_rcv;
extern int gFaxRxCurrentPage;
extern BOOLEAN isReadyToFaxPrint;
extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern BOOLEAN T4_end_of_page;
extern UINT8 *gpRxPageMemory;
extern UINT8 isFSURxImageProgessBegin;

UINT32 NonECM_total_size = 0;
UINT8 too_many_error = 0;

extern OSC_MUTEX modem_dsp_mutex;

/*===========================================================================
*
*    ModemCDBDone
*
*   Wakes up the cdb task to finish processing the cdb for non-ECM
*
*   Input:      none
*   Output:     none
*
*===========================================================================*/
void ModemCDBDone(UINT8 last_buffer)
{
    JOBNODE_BUFFER *cdb = (JOBNODE_BUFFER *)NULL;
	int i = 0;
    FILE *pFile = (FILE *)NULL;
    char PathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char TiffName[MAX_LEN_FILE_PATH_NAME] = {0, };
    int result = 0;
    UINT8 coding_type = 0;
    UINT8 rx_res = 0;   
    UINT16 DocId = 0;
    UINT16 jnum = fsu_job_num_rcv;
    UINT8 *pBuff = (UINT8 *)NULL;

	// in non-ECM lastbuffer will be always 0 and container->production_state will be updated by t4 when it detects RTC
	cdb = pBuffer_front[ModemControl->BufferQueueProcess];

    if (cdb == (JOBNODE_BUFFER *)NULL)
    {
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_CDB_DONE);
    }

	cdb->lastBuffer = last_buffer;

	TRACE_FAX("rx_data size %d(%d)\n", (int)cdb->used, ModemControl->BufferQueueProcess);	

    if( (last_buffer == JOBNODE_BUFFER_LAST) && (T30RxCheckErrors() == 0) )
	{
		//remained buffer write
		isFSURxImageProgessBegin = 0;
		gFaxRxTotalPages++;
		too_many_error = 0;
        DocId = fsu_get_doc_id(jnum);

        snprintf(PathName, sizeof(PathName), "%s%cDoc%02X%02X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, 0, DocId, PATH_SEPARATOR, gFaxRxTotalPages);
		snprintf(TiffName, sizeof(TiffName), "%s%cDoc%02X%02X%cfax%02X.pbm", pRootFolderNameRAM, PATH_SEPARATOR, 0, DocId, PATH_SEPARATOR, gFaxRxTotalPages);
        printf("\n(YHCHO) %s() - PathName: %s , NonECM_total_size = %ld\n", __FUNCTION__, PathName, NonECM_total_size);

        pFile = fopen(PathName, "wb");
        if (pFile == (FILE *)NULL)
        {
            printf("\n(YHCHO) %s() - Check Point [1]\n", __FUNCTION__);

            XASSERT(0,0);
        }
        else
        {
            //printf("\n(YHCHO) %s() - total_size: %d\n", __FUNCTION__, total_size);

			for(i = 0; i <= ModemControl->BufferQueueProcess; i++)
			{
				pBuff = (UINT8 *)pBuffer_front[i]->data;
				memcpy(gpRxPageMemory+NonECM_total_size, pBuff, pBuffer_front[i]->used);
				NonECM_total_size += pBuffer_front[i]->used;
			}

			result = fwrite(gpRxPageMemory, sizeof(UINT8), NonECM_total_size, pFile);
			printf("\n(YHCHO) %s() -(%d/%d) result = (%d/%ld)\n", __FUNCTION__,i,ModemControl->BufferQueueProcess,result,NonECM_total_size);
			if (result <= 0)
			{
				printf("\n(YHCHO) %s() - Check Point [2]\n", __FUNCTION__);
				XASSERT(0,0);
			}

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
		
		NonECM_total_size = 0;

        T30Query( IOCTL_RX_CODING_TYPE, &coding_type, sizeof( coding_type ) );
        T30Query( IOCTL_GET_COMM_RES_RX, &rx_res, sizeof( rx_res ) );
        
        printf("\n(YHCHO) %s() - coding: %d\n", __FUNCTION__, coding_type);

		if(coding_type != CODING_JBIG)
		{
			if(T4_Decoding(jnum, PathName, TiffName, coding_type, 0, rx_res, FAX_APP_JOB_DIRECTION_RECEIVE))
			{
				++ModemControl->BufferQueueProcess;
				pBuffer_front[ModemControl->BufferQueueProcess]->used = 0;
				too_many_error = 1;
				gFaxRxTotalPages--;
				unlink(PathName);
				isFSURxImageProgessBegin = 1;

				return;
			}

			if(!fsu_image_validation_check(rx_res, jnum))
			{
				++ModemControl->BufferQueueProcess;
				pBuffer_front[ModemControl->BufferQueueProcess]->used = 0;
				too_many_error = 2;
				gFaxRxTotalPages--;
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
				++ModemControl->BufferQueueProcess;
				pBuffer_front[ModemControl->BufferQueueProcess]->used = 0;
				too_many_error = 3;
				gFaxRxTotalPages--;
				unlink(PathName);
				isFSURxImageProgessBegin = 1;
				
				return;	
			}

			if(!fsu_image_validation_check(rx_res, jnum))
			{
				++ModemControl->BufferQueueProcess;
				pBuffer_front[ModemControl->BufferQueueProcess]->used = 0;
				too_many_error = 2;
				gFaxRxTotalPages--;
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

    } // if(last_buffer == JOBNODE_BUFFER_LAST)

	// when all allocated dmbs are consumed
	if(++ModemControl->BufferQueueProcess >= NUM_OF_BUFF)
	{	
		TRACE_FAX("Non-ECM buffer full NonECM_total_size = %ld!!\n",NonECM_total_size);

		if( !cdb->lastBuffer ) {
			for(i = 0; i < NUM_OF_BUFF; i++)
			{
				pBuff = (UINT8 *)pBuffer_front[i]->data;
				memcpy(gpRxPageMemory+NonECM_total_size, pBuff, pBuffer_front[i]->used);
				NonECM_total_size += pBuffer_front[i]->used;
			}

			ModemControl->BufferQueueProcess = 0;
			prepare_rx_buffers(NUM_OF_BUFF, FALSE);
		}
		else {
			ModemControl->BufferQueueProcess = 0;
		}
	}
	
	pBuffer_front[ModemControl->BufferQueueProcess]->used = 0;
}

//This will be called at the end of Rx Phase C when there is no data or error to fill RTC
/*===========================================================================
*
*    ModemRxForceEndOfPage
*
*   Called at the end of Rx Phase C
*
*   Input:      none
*   Output:     none
*
*===========================================================================*/
void ModemRxForceEndOfPage( UINT8 RTC_Detected )
{
    JOBNODE_BUFFER *cdb = (JOBNODE_BUFFER *)NULL;
    UINT8 coding_type = 0;
	UINT8 rtc_MH[ 9 ] = { 0x00, 0x08, 0x80, 0x00, 0x08, 0x80, 0x00, 0x08, 0x80 };
	UINT8 rtc_MR[ 10 ] = { 0x00, 0x18, 0x00, 0x03, 0x60, 0x00, 0x0C, 0x80, 0x01, 0x30 };
    UINT8 rtc_cnt = 0, i = 0;
	UINT8 *p_rtc_data = (UINT8 *)NULL;

	if ( !RTC_Detected )        // case is fax error
	{
		cdb = pBuffer_front[ ModemControl->BufferQueueProcess ];

		if ( cdb == 0 )
		{
			TRACE_ERROR( "### ModemRxForceEndOfPage: Error no cdb ###\n" );
            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_RX_PHASE_C_ENDOFPAGE);
		}

		TRACE_FAX("ModemRxForceEndOfPage: Insert RTC\n");

		T30Query( IOCTL_RX_CODING_TYPE, &coding_type, sizeof( coding_type ) );

		// Insert RTC
		if(coding_type == CODING_MH)
		{
			rtc_cnt = 9;
			p_rtc_data = rtc_MH;
		}
		else if(coding_type == CODING_MR)
		{
			rtc_cnt = 10;
			p_rtc_data = rtc_MR;
		}
		else  /* For MMR, the EOFB is always inserted.  Therefore
			   any separate RTCs are not needed. */
			return;


		for(i = 0; i< rtc_cnt; i++)
		{
			if(cdb->used == cdb->userData)
			{
				ModemCDBDone(JOBNODE_BUFFER_NOT_LAST);
				cdb = pBuffer_front[ ModemControl->BufferQueueProcess ];
			}
			*(cdb->data + cdb->used++) = *(p_rtc_data + i);
		}
	}
 	else  // RTC is detected, then we don't need the data
	{
		// wait until no signal

		while ( ModemControl->State != MODEM_IDLE )
		{
			if ( ModemControl->Status )
				break;

			OSC_Sleep( MODEM_DELAY_TIME );
		}

		// if RTC is detected then send blank buffer with setting lastBuffer to finish JobModem
		// get current buffer
		cdb = pBuffer_front[ ModemControl->BufferQueueProcess ];

		cdb->used = 0;      // no need the received data to have Modem Job release this buffer
	}

	// send buffer having RTC to T4 Jobnode
	ModemCDBDone(JOBNODE_BUFFER_LAST);
}

/*===========================================================================
*
*    ModemShutdownJob
*
*   Deallocates the Modem and HDLC control structures previously allocated
*   by ModemInitJob.
*
*   Input:      none
*   Output:     0 if successful, 1 if error detected
*
*===========================================================================*/
UINT8 ModemShutdownJob( void )
{
	UINT8 status;

	status = 1;

	/* Deallocate the Modem control structure */

	if ( ModemControl )
	{
		OSC_Deallocate_Memory( ModemControl );
		ModemControl = 0;
	}

	return ( status );
}


/*===========================================================================
*
*    ModemCheckRTC
*
*   Check RTC
*
*   Input:      data
*   Output:     none
*
*===========================================================================*/
void ModemCheckRTC( UINT8 in_data )
{
	UINT8 i, data;

	data = in_data;

	if ( data == 0 )
	{
		ModemControl->ZeroCount += 8;
	}
	else
	{
		for ( i = 0; i < 8; i++ )
		{
			if ( data & 0x01 )
			{
				if ( ModemControl->ZeroCount >= 11 )
				{
					last_EOL_time = ModemElapsedTime( 0 );
				}

				if ( ( ( ModemControl->EOLType == CODING_MR ) && ( data == 0x03 ) )
				  || ( ( ModemControl->EOLType == CODING_MH ) && ( data == 0x01 ) ) )
				{
					if ( ( ( ModemControl->EOLCount >  0 ) && ( ModemControl->ZeroCount == 11 ) )
					  || ( ( ModemControl->EOLCount == 0 ) && ( ModemControl->ZeroCount >= 11 ) ) )
					{
						ModemControl->EOLCount++;

                        //TRACE_IRQDEBUG( "EOL %d %d %d %d\n", ModemControl->EOLCount, ModemControl->ZeroCount, data, i );

						if ( ModemControl->EOLCount == CONSECUTIVE_EOL )
						{
							ModemControl->FoundRTC = TRUE;

							TRACE_IRQ( " ( RTC )\n" );
						}
					}

					if ( ModemControl->EOLType == CODING_MR )
					{
						data = data >> 1;
						i++;
					}
				}
				else if ( ModemControl->EOLCount < CONSECUTIVE_EOL )
				{
					ModemControl->EOLCount = 0;
				}

				ModemControl->ZeroCount = 0;
			}
			else
			{
				ModemControl->ZeroCount++;
			}

			data = data >> 1;
		}
	}
}

/*===========================================================================
*
*    ModemCDBWriteByte
*
*   Writes a byte of data to the next location in the empty CDB
*   If at the end of the CDB, make the notify callback
*
*   Input:      data
*   Output:     none
*
*===========================================================================*/
void ModemCDBWriteByte( UINT8 data )
{
    JOBNODE_BUFFER *pCdb = (JOBNODE_BUFFER *)NULL;

    pCdb = pBuffer_front[ ModemControl->BufferQueueProcess ];

    if (pCdb == (JOBNODE_BUFFER *)NULL)
    {
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_CDB_WRITE_1);
    }

	// non-ECM Rx mode
	// when it set lastBuffer, it will be one of three cases and don't need to get data.
	// 1. detect RTC 2. Error(line drop, printer/memory), 3. FSK detect without RTC
	if( pCdb->lastBuffer )
		return;

	/* If there is no CDB available or no data buffer, return */
    if ( !pCdb->data )
	{
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_CDB_WRITE_2);
	}

	*( pCdb->data + pCdb->used++ ) = data;

	/* If this is the end of the CDB, make the notify callback */
	//.. here only handle when buffer is full.
	// other case including lastBuffer will be handled in ModemCDBMarkEndOfPage()  8/18/06 THY
	if ( pCdb->used == pCdb->userData )
	{
		//TRACE_FAX( "pModemCDBWriteByte: Cdb full(cdb = %p, size = 0x%x)\n", pCdb, pCdb->used );
		//TRACE_FAX( "rxed data size = 0x%x \n", pCdb->used );
		ModemCDBDone(JOBNODE_BUFFER_NOT_LAST);
	}
}


/*===========================================================================
*
*    ModemCDBReadByte
*
*   Reads a byte of data from the next location in the full CDB
*
*   Input:      none
*   Output:     returns data value read from CDB
*
*===========================================================================*/
UINT8 ModemCDBReadByte( void )
{
    CDB_BUFFER *cdb = (CDB_BUFFER *)NULL;
	UINT8 retval;

    cdb = &cdb_buffer[ModemControl->TxBlockCnt][ModemControl->TxFrameCnt];    // get a cdb pointer from cdb_buffer

	retval = *(UINT8 *)( cdb->data + ModemControl->CDBByteNum++ );

	if ( ModemControl->CDBByteNum >= cdb->size )
	{
        TRACE_IRQ("sent %d bytes (%d) (%d)\n", cdb->size, ModemControl->TxFrameCnt, ModemControl->TxBlockCnt);

		last_EOL_time = ModemElapsedTime( 0 );

		ModemControl->CDBByteNum = 0;
		cdb->size = 0;

		if ( !cdb->end_of_page )
		{
			// if it is end of cdb_buffer
			if ( ++ModemControl->TxFrameCnt >= MAX_NON_ECM_TXFRAME)
			{
				ModemControl->TxFrameCnt = 0;
				ModemControl->TxBlockCnt = (ModemControl->TxBlockCnt + 1) % MAX_ECM_BLOCKS;
			}
		}
		else
		{
			TRACE_IRQ( "ModemCDBReadByte End of page\n" );

			ModemControl->State = MODEM_IDLE;
			ModemDisableTxInt();
		}
	}

	return ( retval );
}

/*===========================================================================
*
*    ModemCDBReadLength
*
*   Reads the length of the current CDB
*
*   Input:      none
*   Output:     returns length read from CDB
*
*===========================================================================*/
UINT16 ModemCDBReadLength( void )
{
	register CDB_BUFFER *cdb;

	cdb = &cdb_buffer[ModemControl->TxBlockCnt][ModemControl->TxFrameCnt];    // get a cdb pointer from cdb_buffer

	if ( !cdb->size )
	{
		if ( cdb->end_of_page )
			return ( 0xFFFF );
	}

	return ( cdb->size );
}

/*===========================================================================
*
*    ModemCDBMarkEndOfPage
*
*   Marks the current CDB in the modem CDB queue as an end-of-page CDB.
*   Used by the modem receive ISR when an abort condition is encountered
*
*   Input:      none
*   Output:     none
*
*===========================================================================*/
void ModemCDBMarkEndOfPage( void )
{

	if ( !pBuffer_front[ ModemControl->BufferQueueProcess ] )
		return;
	pBuffer_front[ ModemControl->BufferQueueProcess ]->lastBuffer = 1;
}

/*===========================================================================
*
*    ModemCDBWriteErrorData
*
*   Write error line data
*
*   Input:      none
*   Output:     none
*
*===========================================================================*/
void ModemCDBWriteErrorData( void )
{
	if ( !ModemControl->FoundRTC )
	{
		TRACE_FAX( " put error line data\n" );

		ModemCDBWriteByte( 0x0F );
		ModemCDBWriteByte( 0x07 );
		ModemCDBWriteByte( 0x40 );
	}
}

/*=============================================================================
*
*    ModemRxInitPage
*
*  Initializes non-HDLC data reception per page.  Configures the modem
*  to the indicated speed (if necessary), initializes pointers, waits for
*  the training sequence to be received, enables Rx data mode
*  interrupts, and returns.  If carrier is not detected within 5 seconds,
*  it returns without enabling interrupts
*
*  Input:      speed = desired data mode (index into modem speed table)
*              training = short or long train
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
*  Output:     ModemControl->State = T4_RX_MODE
*              RDBIE = 1
*============================================================================*/
void ModemRxInitPage( UINT8 speed, UINT8 training )
{
	TRACE_FAX( "ModemRxInitPage() speed %d, training %d\n", speed, training );

	ModemControl->State = T4_RX_MODE;
	ModemControl->Status = NO_ERROR;

	/* Reconfigure and check for short train if code is different or HDLC flag is on */

	if ( ( MDM_READ( MODEM_CONF ) != ModemConfigTbl[ speed ] ) || MDM_BITSET( MODEM_06, HDLC ) )
	{
		TRACE_FAX( "ModemRxInitPage() Reconfigure\n" );

		ModemRxConfigHiSpeed( speed );
	}

	/* Start a 5 second timer */
	MODEM_TIMER_START( TimerA, 5000 );

	while ( 1 )
	{
		OSC_Sleep( MODEM_DELAY_TIME );

		if ( MODEM_TIMER_EXPIRE( TimerA ) )      /* Timed out ? */
		{
			TRACE_FAX( "ModemRxInitPage() Eerror! Timed out.\n" );

			/* Indicate carrier not found */
			ModemControl->Status = TIME_OUT;
			ModemControl->State = MODEM_IDLE;
			break;
		}

		if ( MDM_BITSET( MODEM_0A, PNSUC ) && MDM_BITSET( MODEM_0F, RLSD ) )
		{
			TRACE_FAX( "ModemRxInitPage() Succeeded training\n" );

			MDM_SETBIT( MODEM_06, RTDIS );
			MDM_CLEARBIT( MODEM_0A, FLAGDT );

			TRACE_FAX( "ModemRxInitPage: Start IRQ\n" );

			/* Enable Rx Data interrupts */
			ModemEnableRxInt();
			break;
		}
		else
		{
			if ( MDM_BITSET( MODEM_0F, FED ) )
			{
				OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );
				ModemDSPWrite3( 0xABC, 0x4600 );
				ModemDSPWrite3( 0xABE, 0x1755 );
				OSC_Release_Mutex( &modem_dsp_mutex );
			}
		}
	}
}

/*============================================================================
*
*    ModemTxInitPage
*
*  Initializes non-HDLC data transmission per page.  Configures the modem
*  to the indicated speed, init's sending dummy data if necessary, sends
*  the training sequence, enables parallel data mode interrupts, and returns.
*
*  Input:      speed = desired data mode (index into modem speed table)
*              training = long or short train
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
*  Output:     ModemControl->State = T4_TX_MODE
*              TDBIE = 1
*============================================================================*/
void ModemTxInitPage( UINT8 speed, UINT8 training )
{
	ModemControl->State = T4_TX_MODE;
	ModemControl->Status = NO_ERROR;

	ModemControl->TxFrameCnt = 0;  // frame counter for non-ECM Tx
	ModemControl->TxBlockCnt = 0;
	ModemControl->CDBByteNum = 0;
	frame_num = tx_blk_num = 0;

	ModemSetTraining( training );

	MODEM_TIMER_START( TimerEvent, 250 );  // adjust silence time

	ModemTxConfigHiSpeed( speed );

	ModemControl->HighSpeed = TRUE;

   	OSC_Sleep(1000);

	TRACE_FAX( "ModemTxInitPage: Start IRQ\n" );

   	/* Enable Tx data interrupts */
	ModemEnableTxInt();
}


/*============================================================================
* ModemTxFinishInitPage
* Finish initializing the modem transmission now that the
* first CDB data (not dummy data) to be sent is ready.
*=============================================================================*/
void ModemTxFinishInitPage( void )
{
	TRACE_FAX( "ModemTxFinishInitPage: Start IRQ\n" );

	/* Enable Tx data interrupts */
	ModemEnableTxInt();
}

/*============================================================================
* ModemTxDummyDataTimeOut
* Called on a timeout on wait for actual CDB data and hence
* finish sending the dummy data and abort the modem.
*=============================================================================*/
void ModemTxDataStop( void )
{
	/* Disable interrupts and abort */
	MDM_CLEARBIT( MODEM_08, RTS );

	ModemControl->State = MODEM_IDLE;
}


/*============================================================================
*
*    ModemRxTCFDone
*
*  Completes the training check sequence(TCF)
*
*  Input:      none
*
*  Output:     Returns with 0 if no error, 1 if there is an error
*              TCFErrorBytes = number of errors.  $7FFF means no carrier.
*============================================================================*/
UINT8 ModemRxTCFDone( void )
{
	UINT8 status = 1;
	extern const UINT16 EQM_limit_tbl[ 9 ];

	ModemControl->State = MODEM_IDLE;

	if ( ( ModemControl->TCFErrorBytes != 0xFFFF )
	  && ( ( ModemControl->TCFOKBytes / ModemControl->TCFErrorBytes ) > ModemControl->TCFThreshold ) )
	{
		if ( ModemControl->EQM < EQM_limit_tbl[ T30Control->ModemSpeed ] )
			status = 0;
	}

	TRACE_FAX( "ModemRxTCFDone: OK %d, Error %d\n", ModemControl->TCFOKBytes, ModemControl->TCFErrorBytes );
	if ( status )
		TRACE_FAX( " TCF error\n" );

	ModemDisableRxInt();

	ModemControl->State = MODEM_IDLE;

	MODEM_TIMER_START( TimerEvent, 50 );
	OSC_Sleep( MODEM_DELAY_TIME );

	return ( status );
}


static const UINT16 tcf_byte_tbl[ 9 ] = { 0, 300, 600, 900, 1200, 900, 1200, 1500, 1800 };
/*============================================================================
*
*    ModemRxTCF
*
*  Receives the training check sequence (TCF)
*
*  Input:      speed = modem configuration code
*              ModemControl->TCFThreshold = FTT error threshold
*  Output:     Returns with the 0 if no error, 1 if there is an
*            error.
*          TCFErrorBytes = number of errors.  $7FFF means no carrier.
*============================================================================*/
UINT8 ModemRxTCF( UINT8 speed )
{
	UINT8 data;
	UINT8 count = 0;

	/* Do not receive TCF in V.34 mode */

	if ( ModemControl->V34Mode )
		return ( 0 );

	ModemClearIRQ();

	ModemControl->State = T4_RX_MODE;

	ModemControl->Status = NO_ERROR;

	TRACE_FAX( "ModemRxTCF: Entered ModemRxTCF...\n" );

	/* Turn off short train bit and configure as high speed receiver */
	MDM_CLEARBIT( MODEM_04, EQFZ | STRN );

	TRACE_FAX( "ModemRxTCF: speed = %d\n", speed );

	ModemRxConfigHiSpeed( speed );

	/* Wait up to 4 seconds for carrier detect */
	MODEM_TIMER_START( TimerA, 4000 );

	while ( 1 )
	{
		OSC_Sleep( MODEM_DELAY_TIME );

		/* Failed training */
		if ( MODEM_TIMER_EXPIRE( TimerA ) )
		{
			TRACE_FAX( "ModemRxTCF: Failed training. Timer expired\n" );

			ModemControl->TCFErrorBytes = 0xFFFF;
			return ( ModemRxTCFDone() );
		}

		/* Carrier detected, continue */
		if ( MDM_BITSET( MODEM_0A, PNSUC ) && MDM_BITSET( MODEM_0F, RLSD ) )
		{
			TRACE_FAX( "ModemRxTCF: Succeeded training\n" );

			OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );
			ModemDSPWrite1( 0x270, 0x05, FALSE );
			OSC_Release_Mutex( &modem_dsp_mutex );
			ModemControl->TCFOKBytes = 0;
			ModemControl->TCFErrorBytes = 1;  // for divide 0 error

#ifdef OEM_FAX_REPORT
			ModemControl->Level = ModemReadRxLevel();
			ModemControl->EQM   = ModemReadEQM();
#endif
			break;
		}
		else
		{
			if ( MDM_BITSET( MODEM_0F, FED ) )
			{
				OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );
				ModemDSPWrite3( 0xABC, 0x4A00 );
				ModemDSPWrite3( 0xABE, 0x2000 );
				OSC_Release_Mutex( &modem_dsp_mutex );
			}
		}
	}

	/* Make sure to clear any garbage out of the data buffer */

	while ( MDM_BITSET( MODEM_0C, RXFNE ) )
		data = MDM_READ( MODEM_RBUFF );

	/* Look at data for 1.3 sec */
	MODEM_TIMER_START( TimerA, 1300 );

	while ( 1 )
	{
		OSC_Sleep( MODEM_DELAY_TIME );

		/* Bad TCF data*/
		if ( MODEM_TIMER_EXPIRE( TimerA ) )
		{
			//TRACE_FAX( "ModemRxTCF: Bad TCF data %d. TimerA expired\n" );

			ModemControl->TCFErrorBytes = 0xFFFF;
			return ( ModemRxTCFDone() );
		}

		/* Bad TCF - Carrier Gone */
		if ( MDM_BITCLEAR( MODEM_0F, RLSD ) )
		{
			TRACE_FAX( "ModemRxTCF: Carrier gone.\n" );

			ModemControl->TCFErrorBytes = 0xFFFF;
			return ( ModemRxTCFDone() );
		}

		while ( MDM_BITSET( MODEM_0C, RXFNE ) )
		{
			if ( MDM_READ( MODEM_RBUFF ) == 0 )
				ModemControl->TCFOKBytes++;
			else
				ModemControl->TCFErrorBytes++;
		}

		if ( ModemControl->TCFOKBytes >= 10 )
		{
			break;
		}
	}

	// THY for interrupt method to receive TCF data
	ModemControl->State = MODEM_RX_TCF;

	TRACE_FAX( "ModemRxTCF: Start IRQ\n" );

	ModemEnableRxInt();

	while ( 1 )
	{
		OSC_Sleep( MODEM_DELAY_TIME );

		/* Bad TCF - Carrier Gone */
		if ( MODEM_TIMER_EXPIRE( TimerA ) || MDM_BITCLEAR( MODEM_0F, RLSD ) )
		{
			if ( ( count == 0 ) || !MODEM_TIMER_EXPIRE( TimerA ) )
			{
				TRACE_ERROR( "ModemRxTCF: ERROR carrier gone before valid TCF time\n" );

				ModemControl->TCFErrorBytes = 0xFFFF;
			}

			return ( ModemRxTCFDone() );
		}

		if ( ( count == 0 ) && ( ModemControl->TCFOKBytes > tcf_byte_tbl[ speed ] ) )
		{
			count++;

			if ( speed >= T30_SPEED_V17_7200 )
			{
				TRACE_FAX( "ModemRxTCF: Set short train\n" );

				MDM_SETBIT( MODEM_04, STRN );  // Set short train
			}

#ifdef OEM_FAX_REPORT
			ModemControl->Level = ModemReadRxLevel();
			ModemControl->EQM   = ModemReadEQM();
            CxProtocolReport( TCF, NULL, 0, RX_LOG );
#endif
		}
	}
}


/*=============================================================================
*
*    ModemRxHsSignalRec
*
*  High speed signal recognition
*
*  - Configures to high speed receiver
*  - Starts a 6.7 second T.30 T2 timer.
*
*  - Waits for either the 'PN success' flag or 'FSK 7E' flag, or T2 time-out
*
*  Input:  T2_TIMEOUT = T2 time-out period
*          speed = desired data mode (index into modem speed table)
*          training = short or long train
*          timeout = Time to check for ONLY the PN success flag initially
*          index =  Speed
*           0 V.21,   300
*           1 V.27,  2400
*           2 V.27,  4800
*           3 V.29,  7200
*           4 V.29,  9600
*           5 V.17,  7200
*           6 V.17,  9600
*           7 V.17, 12000
*           8 V.17, 14400
*
*  Output:   return value:
*              TIME_OUT if no signal within 6.7 seconds
*              FSK if no PNSUC detected within 220 msec of signal present
*              HIGH_SPEED if PNSUC detected
*            ModemControl->Status = retval if TIME_OUT
*=============================================================================*/
UINT8 ModemRxHsSignalRec( UINT8 speed, UINT8 training, UINT32 timeout, UINT8 ecm )
{
	UINT8 retval, v34state;
	BOOL fsk = FALSE;

	/* Non V.34 mode */

	if ( !ModemControl->V34Mode )
	{
		TRACE_FAX( "Entering ModemRxHsSignalRec: V17\n" );

		ModemControl->State = T4_RX_MODE;
		ModemControl->Status = NO_ERROR;
		ModemControl->BufferQueueProcess = 0;

		HDLCRxWriteInitAllFrames();

		/* Configure the modem */
		ModemSetTraining( training );

		if ( ecm )
			ModemRxConfigHDLC( speed );
		else
			ModemRxConfigHiSpeed( speed );

		/* Start T.30 T2 timer, waiting for either high speed training sequence (PNSUC) or */
		/* FSK command (FLAGDT).  Default time-out = 7.7 */
		MODEM_TIMER_START( TimerA, timeout );

		/* Wait until fast energy detected */
		while ( MDM_BITCLEAR( MODEM_0F, FED ) )
		{
			OSC_Sleep( MODEM_DELAY_TIME );

			if ( MODEM_TIMER_EXPIRE( TimerA ) )
			{
				TRACE_FAX( "ModemRxHsSignalRec(): FED Expired\n" );

				ModemControl->Status = TIME_OUT;
				return ( TIME_OUT );
			}
		}

		/* Continue checking for PNSUC bit unless the timer expires */

		while ( 1 )
		{
			OSC_Sleep( MODEM_DELAY_TIME );

			/* High speed signal detected? */

			if ( MDM_BITSET( MODEM_0A, PNSUC ) && MDM_BITSET( MODEM_0F, RLSD ) )
			{
				TRACE_FAX( "ModemRxHsSignalRec: Succeeded training\n" );

				MDM_SETBIT( MODEM_06, RTDIS );
				MDM_CLEARBIT( MODEM_0A, FLAGDT );

				/* Set back short train after CTC-CTR for V.17 */
				if ( speed >= T30_SPEED_V17_7200 )
					ModemSetTraining( 0 );

				ModemControl->State = MODEM_IDLE;
				ModemControl->HighSpeed = TRUE;

				retval = HIGH_SPEED;
				break;
			}
			else
			{
				if ( MDM_BITSET( MODEM_0F, FED ) )
				{
					OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );
					ModemDSPWrite3( 0xABC, 0x4600 );
					ModemDSPWrite3( 0xABE, 0x1755 );
					OSC_Release_Mutex( &modem_dsp_mutex );
				}
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
				}
				else if ( MODEM_TIMER_EXPIRE( TimerB ) )  /* Detected for long enough ? */
				{
					retval = FSK;
					break;
				}
			}
			else if ( fsk )  /* FSK tone had been detected earlier but not on now */
			{
				fsk = FALSE;
			}

			/* Timeout? */
			if ( MODEM_TIMER_EXPIRE( TimerA ) )
			{
				TRACE_FAX( "ModemRxHsSignalRec: V.17 TimerA expired\n" );

				retval = TIME_OUT;
				break;
			}
			
			if ( ModemControl->Status & MODEM_ABORT )
			{
				retval = MODEM_ABORT;
				break;
			}
		}  // while(1)
	}

	/* V.34 mode */
	else
	{
		TRACE_FAX( "Entering ModemRxHsSignalRec: V34\n" );

		timeout = ECM_BLOCK_LIMIT_TIMEOUT;
		MODEM_TIMER_START( TimerA, timeout );

		ModemRxWaitForPC( 0 );

		/* Wait for transition to rx data mode */

		while ( 1 )
		{
			OSC_Sleep( MODEM_DELAY_TIME );

			/* Check for primary channel */
			v34state = ModemRxWaitForPC( 1 );

			if ( v34state == PC_RX_DATA_MODE )
			{
				TRACE_FAX( "ModemRxHsSignalRec: V.34 High speed\n" );

				ModemControl->HighSpeed = TRUE;
				retval = HIGH_SPEED;
				break;
			}
			else if ( MODEM_TIMER_EXPIRE( TimerA ) )  /* Timeout means go back to phase B */
			{
				TRACE_FAX( "ModemRxHsSignalRec: V.34 TimerA expired\n" );

				retval = TIME_OUT;
				break;

			}
			else if ( v34state == CC_DATA_MODE )  /* Tx retransmit EOM or MPS ? */
			{
				retval = FSK;
				break;
			}
			
			if ( ModemControl->Status & MODEM_ABORT )
			{
				retval = MODEM_ABORT;
				break;
			}
		}
	}

	ModemControl->State = MODEM_IDLE;
	return ( retval );

}


/*============================================================================
*
*    ModemTxTCF
*
*  Sends the TCF.  Configures the modem to high speed transmit mode
*  and sends 1.5 seconds of 0's.
*
*  Input:      speed = desired data mode (index into modem speed table)
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
*  Output:     (none)
*
*============================================================================*/
void ModemTxTCF( UINT8 speed )
{
	int i;

	/* Do not send TCF in V.34 mode */

	if ( ModemControl->V34Mode )
		return ;

	/* Turn off short train, reset RTS, clear HDLC, and configure modem */
	ModemClearIRQ();

	MDM_CLEARBIT( MODEM_04, STRN );

	MDM_CLEARBIT( MODEM_08, RTS );

	MDM_CLEARBIT( MODEM_06, HDLC );

	MDM_SETBIT( MODEM_08, TPDM );

	MDM_SETBIT( MODEM_09, DATA );

	MDM_SETBIT( MODEM_04, FIFOEN );

	if ( ModemControl->Options.ForceEPTOff
	  || ( !ModemControl->Options.ForceEPTOn
	    && ( ( speed == T30_SPEED_V29_9600 )
	      || ( speed == T30_SPEED_V29_7200 ) ) ) )
	{
		/* OFF echo protect */
		MDM_CLEARBIT( MODEM_03, EPT );
		MODEM_TIMER_START( TimerEvent, 70 );
		i = 80;
	}
	else
	{
		/* ON echo protect */
		MDM_SETBIT( MODEM_03, EPT );
		MODEM_TIMER_START( TimerEvent, 90 );
		i = 100;
	}

	ModemConfig( ModemConfigTbl[ speed ] );

	while ( !MODEM_TIMER_EXPIRE( TimerEvent ) )
	{
		if ( --i == 0 )
			break;

		OSC_Sleep( MODEM_DELAY_TIME );
	}

	/* TCF Sending */

	ModemControl->State = MODEM_TX_TCF;

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );
	/* Set up the modem DSP to automatically transmit 0's for 1.5 sec */
	ModemDSPWrite1( MODEM_TCF_FXN_ENB, MODEM_TCF_FXN_ENB_BIT, TRUE );
	OSC_Release_Mutex( &modem_dsp_mutex );

	/* Turn on RTS */
	MDM_SETBIT( MODEM_08, RTS );

	TRACE_FAX( "ModemTxTCF: Sending ( TCF )\n" );

#ifdef OEM_FAX_REPORT
	ModemControl->Level = ModemReadTxLevel();
	ModemControl->EQM   = 0xffff;
    CxProtocolReport( TCF, NULL, 0, TX_LOG );
#endif

	/* Wait for modem to finish the TCF and reset the RTS bit */
	OSC_Sleep( 1500 );

	MODEM_TIMER_START( TimerEvent, 1700 );

	while ( MDM_BITSET( MODEM_08, RTS ) )
	{
		OSC_Sleep( MODEM_DELAY_TIME );

		// Fail safe
		if ( MODEM_TIMER_EXPIRE( TimerEvent ) )
		{
			UINT8 data;

			TRACE_FAX( " Timeout ( TCF )\n" );

			OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );
			data = ModemDSPRead1( 0x6cd );
			ModemDSPWrite1( 0x6cd, data & 0x7F, FALSE );
			data = ModemDSPRead1( 0x6D7 );
			ModemDSPWrite1( 0x6D7, data & 0x7F, FALSE );
			data = ModemDSPRead1( 0x89 );
			ModemDSPWrite1( 0x89, data & 0xFB, FALSE );
			OSC_Release_Mutex( &modem_dsp_mutex );
			MDM_CLEARBIT( MODEM_08, RTS );
			break;
		}
	}

	ModemControl->State = MODEM_IDLE;
}

/*============================================================================
*
*    ModemSetTraining
*
*  Sets training mode (short or long)
*
*  Input:      1 - long train
*              0 - short train
*
*  Output:     Controls modem short train
*
*============================================================================*/
void ModemSetTraining( UINT8 mode )
{
	TRACE_FAX( "ModemSetTraining %s\n", mode ? "long" : "short" );

	if ( mode )
		MDM_CLEARBIT( MODEM_04, EQFZ | STRN );
	else
		MDM_SETBIT( MODEM_04, STRN );

}

/*============================================================================
*
*    ModemRxTrainingDisable
*
*  Disable or enable the receiver training
*
*  Input:      1 - Disable training
*              0 - Enable training
*
*  Output:     Controls Rx training capability
*
*============================================================================*/
void ModemRxTrainingDisable( BOOL mode )
{
	if ( mode )
		MDM_SETBIT( MODEM_06, RTDIS );
	else
		MDM_CLEARBIT( MODEM_06, RTDIS );
}


/*===========================================================================
*
*    ModemRxBillingDelay
*
*      Waits for 2.15 seconds, and for CNG to go away
*
*      Input:          *speed = Initial speed to try connection at
*                      poll = TRUE if initial polling is allowed
*      Output:         Returns the following
*                      << look at V8Answer in modemv8.c if initial speed is a V34 speed >>
*                        0 if CNG no longer detected, non-zero otherwise
*
*===========================================================================*/
UINT8 ModemRxBillingDelay( UINT8 *speed, BOOL poll, UINT16 timeout )
{
	UINT8 status;

	TRACE_FAX( "ModemRxBillingDelay speed %d, poll %d, timeout %d\n", *speed, poll, timeout );

	ModemControl->V34Mode = FALSE;

	/* If V34 speeds are allowed try to do V8 */

	if ( *speed > T30_SPEED_V17_14400 )
	{
		return V8Answer( speed, poll, FALSE, timeout );
	}

	/* Set tone detectors to detect CNG flags  */
	ModemSetupToneDet( MODEM_CNG_TONE );

	/* Start a 1 second timer */
	MODEM_TIMER_START( TimerB, 1000 );

	/* Loop until either CNG no longer detected or timeout */
	while ( ModemToneDetected( MODEM_CNG_TONE ) && !MODEM_TIMER_EXPIRE( TimerB ) )
	{
		OSC_Sleep( 20 );
	}

	status = ModemToneDetected( MODEM_CNG_TONE );

	TRACE_FAX( "Tone detected: 0x%X.\n", status );

	ModemDisableToneDet( MODEM_CNG_TONE );

	/* Transmit answer tone */
	ModemTxCED();

#ifdef OEM_FAX_REPORT
	ModemControl->Level = ModemReadTxLevel();
	ModemControl->EQM   = 0xffff;
    CxProtocolReport( CED, NULL, 0, TX_LOG );
#endif

	/* Wait 3 seconds */
	OSC_Sleep( 3000 );

	/* Stop Transmitting */
	MDM_CLEARBIT( MODEM_08, RTS );

	MODEM_TIMER_START( TimerEvent, 50 );

	return status;
}

/*============================================================================
*    ModemTxDetCalledStation
*
*      Detects called fax station
*
*      Input:          timeout = carrier time-out period (msec)
*                      abort_fn = Pointer to a function to call to check for aborts
*                      *speed = Initial speed to try connection at
*                      poll = TRUE if initial polling is to be attempted
*      Output:         Returns the following:
*                      << look at V8Originate in modemv8.c if initial speed is a V34 speed >>
*                        FAX_PHASE_A_TIMEOUT
*                        FAX_PHASE_A_BUSY
*                        FAX_PHASE_A_CED
*                        FAX_PHASE_A_ABORT
*============================================================================*/
UINT8 ModemTxDetCalledStation( UINT16 timeout, UINT8 ( *abort_fn ) ( void ),
                               UINT8 *speed, BOOL poll )
{
	TRACE_FAX( "ModemTxDetCalledStation speed %d, poll %d, timeout %d\n", *speed, poll, timeout );

	ModemControl->V34Mode = FALSE;

	/* If V34 speeds are allowed try to do V8 */

	if ( *speed > T30_SPEED_V17_14400 )
	{
		return V8Originate( speed, poll, FALSE, timeout, abort_fn );
	}
	else
	{
		ModemSetupToneDet( MODEM_ANS_TONE );  /* CED signal */
		ModemSetupToneDet( MODEM_FSK_TONE );  /* V.21 signal */
	}

	ModemControl->State = HDLC_RX_MODE;

	ModemControl->Status = NO_ERROR;

	ModemClearIRQ();

	/* Set the carrier timeout timer */
	MODEM_TIMER_START( TimerB, timeout );

	/* Continue sending CNG tone until done */
	while ( 1 )
	{
		/* Need 100 msec interval to coordinate with phone */
		/* so that busy tone can be detected properly */
		OSC_Sleep( 100 );

		/* Check for abort or for scanner pullin paper jam */

		if ( abort_fn && abort_fn() )
		{
			ModemControl->State = MODEM_IDLE;
			return ( MODEM_FAX_PHASE_A_ABORT );
		}

		/* Check for carrier timeout */
		if ( MODEM_TIMER_EXPIRE( TimerB ) )
		{
			ModemControl->Status = NO_ERROR;
			ModemControl->State = MODEM_IDLE;
			return ( MODEM_FAX_PHASE_A_TIMEOUT );
		}

		/* CED or HDLC flag found? */

		if ( ModemToneDetected( MODEM_ANS_TONE ) || ModemToneDetected( MODEM_FSK_TONE ) )
		{
#ifdef OEM_FAX_REPORT
			ModemControl->Level = ModemReadRxLevel();
			ModemControl->EQM   = 0xffff;
            CxProtocolReport( CED, NULL, 0, RX_LOG );
#endif
			ModemControl->State = MODEM_IDLE;
			return ( MODEM_FAX_PHASE_A_CED );
		}
	} /* while sending CNG tones */
}

