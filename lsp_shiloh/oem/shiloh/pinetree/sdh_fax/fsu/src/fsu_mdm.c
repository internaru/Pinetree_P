/*
 *  $RCSfile: fsu_mdm.c,v $
 * $Revision: 1.15 $
 *   $Author: miurat $
 *     $Date: 2011/11/11 23:09:38 $
 */
/************************************************************************
 *
 *      fsu_mdm.c
 *
 *  Modem Task.
 *
 *  Copyright 2004 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "SDHFaxCommonHeader.h"
#include <stdio.h>
#include <unistd.h>
#include "dc_common_includes.h"

#include <sys/types.h>
#include <unistd.h>
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "MODEMX.H"
#include "ECMX.H"
#include "T30X.H"

#include "fshs.h"
#include "PHONEX.H"
#include "fsu.h"
#include "modem_buff.h"
#include "fxcb.h"
#include "t35_cty.h"
#include "fax_memAPI.h"
#include "faxapp_notify.h"


//.. declare variables
OSC_SEMAPHORE sem_modem_start;
OSC_MUTEX modem_dsp_mutex;
OSC_MUTEX mdm_spi_mutex;

static BOOL modem_task_stop = FALSE;
static BOOL modem_task_skip = FALSE;

static UINT8 gCountryCode = 0;

// cdb_buffer for fax tx
CDB_BUFFER cdb_buffer[MAX_ECM_BLOCKS][MAX_ECM_FRAMES];
CDB_BUFFER	*pcdb_buffer;

// jobnode_buffer for fax rx
JOBNODE_BUFFER job_buffer[NUM_OF_BUFF];		// JOBNODE BUFFER for rx data
JOBNODE_BUFFER *pBuffer_front[NUM_OF_BUFF];		// JOBNODE BUFFER for rx data

//OSC_SEMAPHORE *sem_ptr;
MODEM_JOB_PARAMETERS modem_params;
UINT16 frame_num, tx_blk_num, total_blk_num;
UINT8 *gpBufferFrontMemory = 0;

extern UINT8 isReadyToTransmit;
extern UINT32 gnSizeForTxDataBuffer;

extern UINT8 T30TxCheckErrors(void);
extern int faxapp_get_select_country_from_db(void);


/***********************************************************************
 *  Function: setup_buffer_front
 *
 *  Description:
 *		set JOBNODE_BUFFER pointer and set up it's components
 *
 *  Inputs:
 *		frame_cnt: buffer counter
 *
 *  Output:
 *		pointer of JOBNODE_BUFFER
 */
JOBNODE_BUFFER *setup_buffer_front(UINT8 frame_cnt)
{
	UINT32 temp;
	JOBNODE_BUFFER *pBuffOut = &job_buffer[frame_cnt];

	pBuffOut->size = 256*MAX_FRAME_IMP;
	pBuffOut->lastBuffer = pBuffOut->used = 0;
	
	temp = (UINT32)gpBufferFrontMemory;
	pBuffOut->data = (UINT8 *)(temp+(frame_cnt*pBuffOut->size));

	return(pBuffOut);
	
}


/***********************************************************************
 *  Function: prepare_rx_buffers
 *
 *  Description:
 *		prepare JOBNODE_BUFFERs for fax rx
 *
 *  Inputs:
 *		buffers: number of buffer - number of ecm buffers per block
 *		ecm_mode: 1 = ECM, 0 = non-ECM
 *
 *  Output:
 *		none
 */
void prepare_rx_buffers(UINT16 buffers, BOOL ecm_mode)
{
	UINT16 byte_per_frame = 256, i;

	for(i = 0; i < buffers; i++)
	{
		pBuffer_front[i] = setup_buffer_front(i);
		
		if(ecm_mode)
		{
			if(modem_params.coding_type == CODING_JPEG)
            {
				pBuffer_front[i]->userData = byte_per_frame*256;
            }
		}
		else
        {
			pBuffer_front[i]->userData = MAX_BYTE_NON_ECM_RX;	
        }
	}
}


/***********************************************************************
 *  Function: job_modem_init
 *
 *  Description:
 *
 *  Inputs:
 *		UINT8 direction: Tx or Rx
 *		BOOLEAN m_ecm: 1 = ECM, 0 = non-ECM
 *		UINT8 coding = Coding type
 *		UINT16 frame_size : number of byte per frame in case of ECM
 *
 *  Output:
 *		none
 */
void job_modem_init(UINT8 direction, BOOLEAN m_ecm, UINT8 coding, UINT16 frame_size)
{
    UINT16 nego_frame_size = 0;

	if(m_ecm)
	{
		T30Query(IOCTL_BYTE_PER_FRAME, (UINT8 *)&nego_frame_size, sizeof(UINT8));
		nego_frame_size &= 0x00FF;
		
        if(nego_frame_size == 1)
        {
            nego_frame_size = 256;
        }
        else if(nego_frame_size == 2)
        {
            nego_frame_size = 64;
        }
        else
        {
            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_MODEM_INIT);
        }
	}

    if(direction) modem_params.ModemDirection = MODEM_RX;
	else modem_params.ModemDirection = MODEM_TX;
	    	
	modem_params.ecm_mode = m_ecm;
	modem_params.coding_type = coding;

	if(m_ecm) modem_params.wrap = nego_frame_size;
	else modem_params.wrap = frame_size;

    if(modem_params.ModemDirection == MODEM_TX)
	{
    	memset(&cdb_buffer[0][0], 0x00, sizeof(CDB_BUFFER)*MAX_ECM_BLOCKS*MAX_ECM_FRAMES);
	}

	frame_num = tx_blk_num = total_blk_num = 0;
}


/***********************************************************************
 *  Function: give_dmb_to_modem
 *
 *  Description:
 *    Give partial buffer pointer of t4/6 coded data in dmb to cdb 
 *    - cdb->data will have pointer of every 256 buffer area of dmb
 *	  - this will be called by t4 compressor when it compressed to a frame data
 *
 *  Inputs:
 *    JOBNODE_BUFFER *pBuff - job node buffer pointer 
 *
 */
void give_dmb_to_modem(JOBNODE_BUFFER *pBuff)
{
    CDB_BUFFER *cdb = (CDB_BUFFER *)NULL;
	
	cdb = &cdb_buffer[tx_blk_num][frame_num];

    if (cdb == (CDB_BUFFER *)NULL)
    {
        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_DMB_TO_MODEM);
    }

	cdb->data = (UINT8 *)pBuff->data;
	cdb->size = pBuff->used;
	cdb->end_of_page = pBuff->lastBuffer;

    if(modem_params.ecm_mode)
    {
		ECMFeedCDB(cdb);			// give a cdb to modem driver
    }

	frame_num++;

	if(cdb->end_of_page || frame_num == MAX_ECM_FRAMES)
	{
        if (frame_num == MAX_ECM_FRAMES)
        {
            isReadyToTransmit = 1;
        }

		frame_num = 0;
        tx_blk_num = (tx_blk_num + 1) % MAX_ECM_BLOCKS;
		total_blk_num++;
        printf("=====> give_dmb_to_modem: total_blk_num %d, \n", total_blk_num);

		if(cdb->end_of_page)
		{
            printf("==> give_dmb_to_modem: end of page\n");

            isReadyToTransmit = 1;

			return;
        }
	}
}


/*****************************************************************************
 * Function:    job_modem_start
 * Purpose:     Create a task and semaphore for simple codec for fax
 * Inputs:		None      
 * Outputs:		None
 *****************************************************************************/
#include <pthread.h>
#include "posix_ostools.h"
static void *job_modem_main (void *unused);
#define FSU_MDM_STACK_SIZE     (1024 * 10 * 3)
static unsigned char SDHFaxModemStack[FSU_MDM_STACK_SIZE];
static pthread_t sdhfaxmodem_thd_id;
extern int isFSUModemReady;

void job_modem_start(void)
{
    DC_RETURN_CODE result;
    int retcode = 0;

    while(1)
    {
        if (isFSUModemReady == 1)
            break;

        OSC_Sleep_Task (100);
    }

    gpBufferFrontMemory = (UINT8 *)MEM_MALLOC(TEMP_BUFFER_FRONT_MEM);
	if( gpBufferFrontMemory == 0 )
	{
        XASSERT(0,0);
	}

	result = OSC_Create_Semaphore(&sem_modem_start, "modem_start_semaphore", 0, 0);
	if(result == TX_NO_MEMORY)
	{
        printf("error creation sem_modem_start\n");
	}

	// create mutex for modem dsp control
	result = OSC_Create_Mutex(&modem_dsp_mutex, "modem_dsp_mutex", 0);
	if (result != DC_SUCCESS)
    {
        printf("create modem_dsp_mutex ERR [%#x]", result);
		return ;
    }

	// create mutex for spi	control
	result = OSC_Create_Mutex(&mdm_spi_mutex, "modem_spi_mutex", 0);
	if (result != DC_SUCCESS)
    {
        printf("tx_mutex_create() ERR [%#x]", result);
    }
	
    retcode = posix_create_thread(&sdhfaxmodem_thd_id, job_modem_main, 0, "tSDHFaxModem", SDHFaxModemStack, FSU_MDM_STACK_SIZE, POSIX_THR_PRI_NORMAL);
	XASSERT(retcode==0,0);
}


void activate_modem_job(void)
{
	OSC_Give_Semaphore(&sem_modem_start);
}


/*****************************************************************************
 * Function:    modem_task_main
 * Purpose:     transfer image data to next job
 * Inputs:		None      
 * Outputs:		None
 *****************************************************************************/
static void *job_modem_main (void *unused)
{
   	while(1)
	{
		OSC_Take_Semaphore(&sem_modem_start, TX_WAIT_FOREVER);

		modem_task_stop = FALSE;
		modem_task_skip = FALSE;

		while ( !modem_task_stop )
		{
			if ( !modem_task_skip )
            {
				ModemIsr((void *)NULL);
            }

			usleep( (MODEM_TASK_PERIODIC_TIME)*1000 );
		}
	}

    /* should never get here! */
    //assert(0);

    return 0;
}


void modem_start_task( void )
{
	OSC_Give_Semaphore(&sem_modem_start);
}


void modem_stop_task( void )
{
	modem_task_stop = TRUE;
}


void modem_skip_task( void )
{
	modem_task_skip = TRUE;
}


void modem_continue_task( void )
{
	modem_task_skip = FALSE;
}


/*****************************************************************************
 * Function:    modem_startup_process
 * Purpose:     Initialize modem job process
 * Inputs:		UINT8      
 * Outputs:		None
 *****************************************************************************/
void modem_startup_process (UINT8 *status)
{
    int CountryCode = 0;

    // *** Modem code initializtion ***
    CxModemOpen( (void *)NULL, status );  // should set pointer of callback function

    CountryCode = faxapp_get_select_country_from_db();

	gCountryCode = (UINT8) CountryCode;

    CxSelectCountry( (UINT8) CountryCode );

	CxSetCallBackHost( (void *)NULL );
}


UINT8 getCountryDateType(void)
{
	UINT8 datetype = FAX_COUNTRY_DATE_TYPE_YMD;

    switch (gCountryCode)
    {
        case T35_UNITED_STATES:
            datetype = FAX_COUNTRY_DATE_TYPE_MDY;
            break;

        case T35_KOREA:
        case T35_CHINA:
            datetype = FAX_COUNTRY_DATE_TYPE_YMD;
            break;

        default:
            datetype = FAX_COUNTRY_DATE_TYPE_DMY;
            break;
    }

	return datetype;
}


/*****************************************************************************
 * Function:    set_modem_idle
 * Purpose:     
 * Inputs:		None      
 * Outputs:		None
 *****************************************************************************/
void set_modem_idle (void)
{
//  char cmd[80];

//  dc_log_printf("+ set_modem_idle\n");
  //local_phone_relay (RLY_CLOSED); // Restore local phone connection
  caller_id_name[0] = 0;          // Indicate name not received
  caller_id_num[0] = 0;           // Indicate number not received

#ifdef ENABLE_RING_SILENCE   // +sjb-2/05/08: Begin...
  ring_audible_silence (FALSE);   // Don't silence ring audible
#endif  // +sjb-2/05/08: End
  //.. 7/21/09 THY
  //fsu_log_end_call ();  // +sjb-1/12/06: End call log, if active
//  dc_log_printf("- set_modem_idle\n");

}


/*****************************************************************************
 * Function:    monitor_modem_idle
 * Purpose:     State machine to put the modem into a known idle state
 * Inputs:		None      
 * Outputs:		None
 *****************************************************************************/
void monitor_modem_idle (void)
{
	UINT8 modem_initialized;

//	dc_log_string ("DSP H/W Reset...\n");  // TEST!!!

	modem_startup_process (&modem_initialized);

	if (!modem_initialized)  // Failed to initialize DSP?
	{
//	  dc_log_string ("*** DSP H/W Failure! ***\n");  // TEST!!!
	}
	else  // DSP came up okay
	{
//	  dc_log_string ("DSP H/W Okay\n");  // TEST!!!
	  set_modem_idle ();
	}
}


/*****************************************************************************
 * Function:    local_phone_off_hook
 * Purpose:     Check the status of external phones (in-line and parallel)
 * Inputs:		None      
 * Outputs:		// FALSE: Local phone is on-hook
 *				// TRUE:  Local phone is off-hook
 *****************************************************************************/
UINT8 local_phone_off_hook (void)
{
    return (CxModemLineStatus ());
}

