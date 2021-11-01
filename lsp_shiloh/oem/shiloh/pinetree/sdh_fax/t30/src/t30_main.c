/*
 *  $RCSfile: t30_main.c,v $
 * $Revision: 1.55 $
 *   $Author: miurat $
 *     $Date: 2012/01/13 03:10:03 $
 */
/************************************************************************
 *
 *      t30_main.c
 *
 *  T.30 Mains Functions (Transmit and Receive)
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/
#include "fax_memAPI.h"
#include "SDHFaxCommonHeader.h"
#include "dc_common_includes.h"
#include "cxshell.h"
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "ERROR.H"
#include "MODEMX.H"
#include "PHONEX.H"
#include "ECMX.H"
#include "T30X.H"
#include "ECM.H"
#include "t30.h"
#include "fxcb.h"
#include "fsu_api.h"
#include "oem_cty.h"

//.. prototype
OSC_SEMAPHORE sem_t30_start;

// .. extern
extern void t30_init_params( CtryPrmsStruct *Ctry );
extern void ECMSendBlockOnError( void );
extern void HDLCDeAllocateMemory( void );
extern void CxSelectCountry( UINT8 nCountryCode );
extern UINT8 HDLCAllocateMemory(void);
extern void fsu_set_curr_func_tx(void);
extern void fsu_set_curr_func_rx(void);
extern void faxapp_set_event_flags(int Event);

#if V34_SUPPORT
extern void ModemStopV34( void );
#endif

int gFaxEndOfFax = 1;
T_T30_MSG t30_msg;
BOOLEAN T4_end_of_page = FALSE;
extern UINT8 *gpRxPageMemory;	//add.bong.2012-09-04 8:04am
extern UINT16 gFaxT30Result;
extern UINT16 fsu_get_doc_id (UINT16 jnum);
extern UINT16 fsu_job_num_send;
extern UINT16 fsu_job_num_rcv;
extern int gFaxAbnormalFlag;
extern UINT8 isTxImageProcessDone;


UINT32 t30_jbig_rcvd_line(void)
{
    if(!T30Control)
        return 0;
    if(T30Control->total_jbig_rcvd_line == 0xffffffff)
        T30Control->total_jbig_rcvd_line = 0;

    return T30Control->total_jbig_rcvd_line;
}


/*****************************************************************************
 * Function:    job_t30_start
 * Purpose:     Create a task and semaphore for fax job
 * Inputs:		None
 * Outputs:		None
 *****************************************************************************/
#include <pthread.h>
#include "posix_ostools.h"
static void *job_t30_main (void *unused);
#define T30_MAIN_STACK_SIZE      (1024 * 10 * 10)
static unsigned char SDHFaxT30Stack[T30_MAIN_STACK_SIZE];
static pthread_t sdhfaxt30_thd_id;
extern int isFSUT30Ready;

void job_t30_start( void )
{
    int retcode = 0;

    gpRxPageMemory = (UINT8 *)MEM_MALLOC(TEMP_RX_PAGE_MEM);
    if( gpRxPageMemory == 0 )
    {
        XASSERT(0,0);
    }

    // create semaphore
    OSC_Create_Semaphore(&sem_t30_start, "sem_t30_start", 0, 0);

    retcode = posix_create_thread(&sdhfaxt30_thd_id, job_t30_main, 0, "tSDHFaxT30", SDHFaxT30Stack, T30_MAIN_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    XASSERT( retcode==0, retcode );

}


/////////////////////////////////////////////////////////////////////
// Function name  : T30Query
//
// Return type    : int             (TRUE if successful)
// Argument       : UINT8 request   (unique request ID)
// Argument       : UINT8 *pBuff    (ptr to parameter in/out data)
// Argument       : UINT32 size     (sizeof of buffer)
//
// Description    : Driver specific I/O request method.
//
/////////////////////////////////////////////////////////////////////
int T30Query( UINT8 request, UINT8 *pBuff, UINT32 size )
{
    UINT16 val;
    int fResult = FALSE;

    if ( pBuff == NULL )
    {
        TRACE_ERROR( "### Error T30Query pBuff ###\n" );
        return fResult;
    }

    if ( !( request < IOCTL_T30FAX_LAST ) )
    {
        TRACE_ERROR( "### Error T30Query request ###\n" );
        return fResult;
    }

    switch ( request )
    {
        case IOCTL_T30FAX_MESSAGE:

            if ( !T30Message( *pBuff ) )
            {
                fResult = TRUE;
            }
            else
            {
                fResult = FALSE;
            }
            break;

#if 0 // Nout used any more..
        case IOCTL_T30FAX_WAIT_RXNEGO:

            OSC_Take_Semaphore(&m_SemaphorePhaseBDone, OSC_SUSPEND);
            *pBuff = T30RxModeQuery();
            fResult = TRUE;
            break;

        case IOCTL_T30FAX_WAIT_TXNEGO:

            OSC_Take_Semaphore(&m_SemaphorePhaseBDone, OSC_SUSPEND);
            *pBuff = T30Control->Status;
            fResult = TRUE;
            break;
#endif

        case IOCTL_T30Rx_GET_POST_MSG:

            fResult = T30RxCheckMultiPage( pBuff );
            break;

#if 0 // Nout used any more..
        case IOCTL_T30SET_ABORT:

            if ( T30Control )
            {
                T30Control->Status = *pBuff;
            }
            break;
#endif

#if 0 // Nout used any more..
        case IOCTL_T30GET_STATUS:

            if ( T30Control )
            {
                * pBuff = T30Control->Status;
            }
            break;
#endif

        case IOCTL_CHECK_RCVDFAXQ:
            break;

        case IOCTL_END_OF_PAGE:
            T4_end_of_page = TRUE;             // indicate T4 is done
            break;

        case IOCTL_ECM_MODE_RX:
            *pBuff = T30Control->T4Params.Rx.ECMMode;
            break;

        case IOCTL_ECM_MODE_TX:
            *pBuff = T30Control->T4Params.Tx.ECMMode;
            break;


        case IOCTL_BYTE_PER_FRAME:
            if ( ECMControl->BytesPerFrame == 256 )
            {
                *pBuff = ECM_256;
            }
            else
            {
                *pBuff = ECM_64;
            }
            break;

        case IOCTL_RX_CODING_TYPE:
            *pBuff = T30Control->T4Params.Rx.Coding;
            break;

        case IOCTL_TX_CODING_TYPE:
            *pBuff = T30Control->T4Params.Tx.Coding;
            break;

        case IOCTL_MAX_BYTES_PER_LINE:

            if ( T30Control->ModemSpeed > T30_SPEED_V17_14400 )
            {
                val = T30_T4MaxBytesTbl[ T30_SPEED_V17_14400 + 1 + T30Control->ModemSpeed ];
            }
            else
            {
                val = T30_T4MaxBytesTbl[ ( T30Control->ModemSpeed << 1 ) | T30Control->ECMEnabled ];
            }

            *pBuff = val;
            *( pBuff + 1 ) = ( val >> 8 );
            break;

        case IOCTL_MIN_BYTES_PER_LINE:
            *pBuff = T30Control->T4Params.Tx.MinBytes;
            break;

        case IOCTL_GET_COMM_RES_RX:
            *pBuff = T30Control->T4Params.Rx.Res;
            break;

        case IOCTL_GET_COMM_RES_TX:
            *pBuff = T30Control->T4Params.Tx.Res;
            break;

        case IOCTL_SET_T4RX_PARAMS:
            break;

        case IOCTL_SET_T4TX_PARAMS:
            break;

        case IOCTL_GET_TX_FAX_RES:
            T30Attribute( GET, FXA_TXRESOLUTION, pBuff );
            break;

        case IOCTL_GET_RX_FAX_RES:
            T30Attribute( GET, FXA_RXRESOLUTION, pBuff );
            break;

        case IOCTL_GET_COMM_WIDTH_RX:
            *pBuff = T30Control->T4Params.Rx.Width;
            break;

        case IOCTL_GET_COMM_WIDTH_TX:
            *pBuff = T30Control->T4Params.Tx.Width;
            break;

        default:
            break;
    }

    TRACE_FAX( "T30Query %d %d %ld\n", request, *pBuff, size );
    return fResult;
}


/*****************************************************************************
 * Function:    job_t30_main
 * Purpose:     t30 protocol state machine
 * Inputs:		None
 * Outputs:		None
 *****************************************************************************/
static void *job_t30_main(void *unused)
{
    static T_T30_STATES _prev_t30_state;
    UINT8 FaxSessionDone;

    isFSUT30Ready = 1;

    while ( TRUE )
    {
        OSC_Take_Semaphore( &sem_t30_start, OSC_SUSPEND );
        TRACE_FAX("job_t30_main start\n");

        /* Make sure initialization is successful before entering the T.30 state machine */

        if ( T30InitJob() != E_OK )
        {
            /* Report initialization error to app manager */;
            continue;
        }

        _prev_t30_state = STATE_IDLE;
        FaxSessionDone = 0;

        while ( !FaxSessionDone )
        {
            if ( _prev_t30_state != T30Control->State )
            {
                TRACE_FAX( "T.30 state (%d) -> (%d)\n", _prev_t30_state, T30Control->State );
                _prev_t30_state = T30Control->State;
                T30SpeakerControl( T30_SPEKER_OFF );
            }

            gFaxT30Result = T30Control->Status;

            /* Go directly to Phase E if any errors were detected */

            if ( T30Control->Status )
            {
                if ( ( T30Control->State != STATE_RX_DISCONN )
                  && ( T30Control->State != STATE_TX_DISCONN ) )
                {
                    // in case of t.30 protocol error
                    if (T30Control->Status != FAX_ERROR_NO_MEM_ERROR )
                    {
                        if ( (gFaxAbnormalFlag != FAX_ABNORMAL_FLAG_SCAN_JAM) 
							&& (gFaxAbnormalFlag != FAX_ABNORMAL_FLAG_SCAN_CANCEL) 
							&& (gFaxAbnormalFlag != FAX_ABNORMAL_FLAG_TX_CANCEL) 
							&& (gFaxAbnormalFlag != FAX_ABNORMAL_FLAG_RX_CANCEL) )
                        {
                            if (T30Control->Direction == T30_RECEIVE)
                            {
                                Fax_API_CancelFax(FALSE, fsu_job_num_rcv);
                            }
                            else
                            {
                                Fax_API_CancelFax(FALSE, fsu_job_num_send);
                            }
                        }
                    }

                    if ( T30Control->Direction == T30_RECEIVE )
                    {
                        if ( (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_1) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_2) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_3) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_1) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_2) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_3) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_4) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_5) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_6) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_5) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_6) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_7) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_1) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_2) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_3) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_9) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_1) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_2) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_3) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_4) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_5) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_6) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_11) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_12) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_13) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_14) )
                        {
                            T30Control->State = STATE_RX_DISCONN;
                        }
                        else if ( T30Control->State != STATE_RX_ABORT )
                        {
                            if ( T30Control->State == STATE_RX_PHASE_A )
                            {
                                T30Control->State = STATE_RX_ABORT;
                            }
                            else
                            {
                                T30Control->State = STATE_RX_PHASE_E;
                            }
                        }
                    }
                    else
                    {
                        if ( (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_1) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_2) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_3) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_1) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_2) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_3) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_4) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_5) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_6) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_5) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_6) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_7) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_1) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_2) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_3) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_9) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_1) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_2) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_3) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_4) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_5) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_6) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_11) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_12) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_13) ||
                             (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_14) )
                        {
                            T30Control->State = STATE_TX_DISCONN;
                        }
                        else if ( T30Control->State != STATE_TX_ABORT )
                        {
                            if ( T30Control->State == STATE_TX_PHASE_A )
                            {
                                T30Control->State = STATE_TX_ABORT;
                            }
                            else
                            {
                                T30Control->State = STATE_TX_PHASE_E;
                            }
                        }
                    }
                }
                // in case of STATE_RX_DISCONN or STATE_TX_DISCONN
                else if ( (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_1) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_2) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_3) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_1) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_2) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_3) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_4) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_5) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_4_6) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_5) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_6) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_7) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_1) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_2) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_8_3) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_9) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_1) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_2) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_3) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_4) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_5) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_10_6) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_11) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_12) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_13) ||
                          (T30Control->Status == FAX_ERROR_BAD_RSP_ERROR_14) )
                {
                    if (T30Control->Direction == T30_RECEIVE)
                    {
                        Fax_API_CancelFax(FALSE, fsu_job_num_rcv);
                    }
                    else
                    {
                        Fax_API_CancelFax(FALSE, fsu_job_num_send);
                    }
                }

                if ( _prev_t30_state != T30Control->State )
                {
                    TRACE_FAX( "T.30 state (%d) -> (%d)\n", _prev_t30_state, T30Control->State );
                    _prev_t30_state = T30Control->State;
                }

                TRACE_ERROR( "T30Task():Error, Status = %d\n", T30Control->Status );
            }

            switch ( T30Control->State )
            {
                case STATE_IDLE:
                    TRACE_FAX( "T.30 Idle\n" );
                    break;

                    /* Receiver states */
                case STATE_RX_PHASE_A:
                    T30RxPhaseA();
                    break;

                case STATE_RX_PHASE_B:
                    fsu_set_curr_func_rx();			// do this for rx poll
                    T30RxPhaseB();
                    break;

                case STATE_RX_PHASE_B_SIGNAL_REC:
                    T30RxSignalRec();
                    break;

                case STATE_RX_PHASE_C:
                    T30RxPhaseC();
                    break;

                case STATE_RX_PHASE_C_ECM:
                    ECMRxBlock();
                    break;

                case STATE_RX_PHASE_D_SIGNAL_REC:
                    T30RxSignalRec();
                    break;

                case STATE_RX_PHASE_D:
                    T30RxPhaseBDCommand();
                    break;

                case STATE_RX_PHASE_E:
                    T30RxPhaseE();
                    break;

                case STATE_RX_DISCONN:
                    TRACE_FAX( "T.30 RX Disconnect\n" );
                    T30ShutdownJob();
                    ModemReinitJob();
                    set_fsu_state( FSU_STATE_RX_PHASE_E );  // inform fsu_task
                    faxapp_set_event_flags(FAXAPP_T30_EVENT_RX_COMPLETE);
                    FaxSessionDone = 1;
                    break;

                case STATE_RX_ABORT:
                    TRACE_FAX( "T.30 RX Abort\n" );
                    T30ShutdownJob();
                    ModemReinitJob();
                    set_fsu_state( FSU_STATE_RX_ABORT );  // inform fsu_task
                    FaxSessionDone = 1;
                    break;

                    /* Transmitter States */

                case STATE_TX_PHASE_A:
                    T30TxPhaseA();
                    break;

                case STATE_TX_PHASE_B:
                    T30TxPhaseB();
                    set_fsu_state( FSU_STATE_TX_PHASE_B );
                    break;

                case STATE_TX_SET_MODE:
                    fsu_set_curr_func_tx();			// do this for tx poll
                    T30TxSetMode();
                    break;

                case STATE_TX_PHASE_C:
                    set_fsu_state( FSU_STATE_TX_PHASE_C_WAIT );
                    T30TxPhaseC();
                    break;

                case STATE_TX_PHASE_C_ECM:
                    ECMTxBlock();
                    break;

                case STATE_TX_PHASE_D:
                    T30TxPhaseD();
                    break;

                case STATE_TX_PHASE_D2:
                    set_fsu_state( FSU_STATE_TX_PHASE_D );
                    T30TxPhaseD2();
                    break;

                case STATE_TX_PHASE_E:
                    T30TxPhaseE();
                    break;

                case STATE_TX_DISCONN:
                    TRACE_FAX( "T.30 TX Disconnect\n" );
                    T30ShutdownJob();
                    ModemReinitJob();
                    set_fsu_state( FSU_STATE_TX_PHASE_E );  // inform fsu_task
                    FaxSessionDone = 1;
                    break;

                case STATE_TX_VOICE_REQ:
                    T30TxVoiceReq();
                    break;

                case STATE_TX_ABORT:
                    TRACE_FAX( "T.30 TX Abort\n" );
                    T30ShutdownJob();
                    ModemReinitJob();
                    set_fsu_state( FSU_STATE_TX_ABORT );  // inform fsu_task
                    FaxSessionDone = 1;
                    break;

                case STATE_HOST_ABORT:
                    TRACE_FAX( "T.30 Host Abort\n" );
                    T30ShutdownJob();
                    ModemReinitJob();
                    FaxSessionDone = 1;
                    set_phone_state( PHONE_STATE_INIT );
                    break;

                default:
                    TRACE_FAX( "T.30 unkown state %d\n", T30Control->State );
                    break;
            } /* switch */
        } /* while (!FaxSessionDone) */

        TRACE_FAX( "T.30 FaxSessionDone %d\n", FaxSessionDone );

    } /* while ( TRUE )*/

    return 0;
}


/////////////////////////////////////////////////////////////////////
// Function name  : T30Message
//
// Return type    : UINT8 - 0 if successful, 1 if operation invalid
//
// Argument       : UINT8
//
// Description    : T.30 activation function
//
/////////////////////////////////////////////////////////////////////
ERCODE T30Message( UINT8 message )
{
    ERCODE status = E_OK;
    BOOL bRestart = FALSE;

    t30_msg.Polling = 0;
    t30_msg.Memory = 0;
    t30_msg.Manual = 0;
    t30_msg.TxAllRcvdFaxes = FALSE;

    switch ( message )
    {
        case FXM_CALL_AUTO:
            /* CALL_AUTO is valid only if no fax session is running */
            //TRACE_FAX( "T30Message: FXM_CALL_AUTO %x\n", T30Control );

            if ( !T30Control )
            {
                t30_msg.Direction = T30_TRANSMIT;
                bRestart = TRUE;
            }
            else
                status = E_EMPTY;
            break;

        case FXM_CALL_MANUAL:
            /* CALL_MANUAL is valid only if no fax session is running */
            //TRACE_FAX( "T30Message: FXM_CALL_MANUAL %x\n", T30Control );

            if ( !T30Control )
            {
                t30_msg.Direction = T30_TRANSMIT;
                t30_msg.Manual = 1;
                bRestart = TRUE;
            }
            else
                status = E_EMPTY;
            break;

        case FXM_ANSWER:
            /* ANSWER is valid only if no fax session is running */
            //TRACE_FAX( "T30Message: FXM_ANSWER %x\n", T30Control );

            if ( !T30Control )
            {
                TRACE_FAX( "T30Message: t30_msg.Direction = T30_RECEIVE\n" );
                t30_msg.Direction = T30_RECEIVE;
                bRestart = TRUE;
            }
            else
                status = E_EMPTY;
            break;

        case FXM_POLL_AUTO:
            /* POLL_AUTO is valid only if no fax session is running */
            //TRACE_FAX( "T30Message: FXM_POLL_AUTO %x\n", T30Control );

            if ( !T30Control )
            {
                t30_msg.Direction = T30_TRANSMIT;
                t30_msg.Polling = 1;
                bRestart = TRUE;
            }
            else
                status = E_EMPTY;
            break;

        case FXM_POLL_MANUAL:
            /* POLL_MANUAL is valid only if no fax session is running */
            //TRACE_FAX( "T30Message: FXM_POLL_MANUAL %x\n", T30Control );

            if ( !T30Control )
            {
                t30_msg.Direction = T30_TRANSMIT;
                t30_msg.Polling = 1;
                t30_msg.Manual = 1;
                bRestart = TRUE;
            }
            else
                status = E_EMPTY;
            break;

        case FXM_ABORT:
            /* ABORT is valid only if a fax session is running */
            //TRACE_FAX( "T30Message: FXM_ABORT %x\n", T30Control );

            if ( T30Control )
            {
                T30StopFax();
            }
            else
                status = E_EMPTY;
            break;

        case FXM_MEMTX:
            /* MEMTX valid only if no fax session is running */
            //TRACE_FAX( "T30Message: FXM_MEMTX %x\n", T30Control );

            if ( !T30Control )
            {
                t30_msg.Direction = T30_TRANSMIT;
                t30_msg.Memory = 1;
                bRestart = TRUE;
            }
            else
                status = E_EMPTY;
            break;

        case FXM_TX_ALL_RCVD_FAXES:

            //TRACE_FAX( "T30Message: FXM_TX_ALL_RCVD_FAXES %x\n", T30Control );

            if ( !T30Control )
            {
                t30_msg.Direction = T30_TRANSMIT;
                t30_msg.Memory = 1;
                t30_msg.TxAllRcvdFaxes = TRUE;
                bRestart = TRUE;
            }
            else
                status = E_EMPTY;
            break;

        default:
            status = E_PARAM;
            break;
    }

    TRACE_FAX( "T30Message() message %d status %d\n", message, status );

    if ( bRestart )
    {
        OSC_Release_Semaphore( &sem_t30_start );		// activate job_t30_main()
    }

    return ( status );
}



/////////////////////////////////////////////////////////////////////
// Function name  : T30Attribute
//
// Return type    : UINT8 - 0 if successful, 1 if operation invalid
//
// Argument       : SET_ATTRIBUTE
//                  GET_ATTRIBUTE
//                  attrib_id - ID of the specific attibute
//                  *msg - pointer to memory where the attribute
//                         will be read or written
//
//
// Description    : T.30 attribute function for public use
//
/////////////////////////////////////////////////////////////////////
ERCODE T30Attribute( UINT8 set_get, T30_ATTRIB attrib_id, UINT8 *msg )
{
    UINT8 i, temp;
    ERCODE retval = E_OK;

    /* Set Attribute */

    if ( set_get == SET )
    {
        switch ( attrib_id )
        {
            case FXA_INIT_ATTRIBUTE:
                break;

            case FXA_COUNTRY_CODE:
                t30_attribute.CountryCode = *msg;
                CxSelectCountry( *msg );
                break;

            case FXA_DIALMODE:
                t30_attribute.FaxDialMode = *msg;
                break;

            case FXA_ANSWERMODE:
                t30_attribute.FaxAnswerMode = *msg;
                break;

#if __SUPPORT_MANUAL_RECEIVE__
            case FXA_MANUALRECEIVE:
                t30_attribute.ManualReceive = *msg;
                break;

            case FXA_RECEIVESOURCE:
                t30_attribute.FaxReceiveSource = *msg;
                break;
#else
#endif

            case FXA_AUTOANSWERRINGS:
                t30_attribute.FaxAutoAnswerRings = *msg;
                break;

            case FXA_ATTENUATION:
                t30_attribute.FaxAttenuation = *msg;

                if ( ModemControl )
                    ModemControl->Options.FaxTxLevel = *msg;
                break;

            case FXA_BUFFERED_DELAFTERTX:
                t30_attribute.Buffered_DelAfterTx = *msg;
                break;

            case FXA_BUFFEREDPRINT:
                t30_attribute.BufferedPrinting = *msg;
                break;

            case FXA_CEDDETECT_TIMEOUT:
                t30_attribute.CEDDetectionTimeout = *( ( UINT32 * ) msg );
                break;

            case FXA_COVERPAGE:
                t30_attribute.CoverPage = *msg;
                break;

            case FXA_COVERPAGE_ID:
                t30_attribute.CoverPageID = *msg;
                break;

            case FXA_DELAYEDTXDOCID:
                t30_attribute.FaxDelayedTxDocID = *( ( UINT16 * ) msg );
                break;

            case FXA_ECMMODE:
                t30_attribute.ECMMode = *msg;
                break;

            case FXA_EQUALIZER:
                t30_attribute.FaxEqualizer = *msg;

                if ( ModemControl )
                    ModemControl->Options.FaxEqzr = *msg;
                break;

            case FXA_FALLBACK_NOOFTRIES:
                t30_attribute.NoOfTriesOnFallback = *msg;
                break;

            case FXA_FAXFORWARD:
                t30_attribute.FaxForward = *msg;
                break;

            case FXA_FAXFORWARD_NO:
                for ( i = 0; i < LEN_PHONE_NO + 1; i++ )
                {
                    t30_attribute.FaxForwardNo[ i ] = *( msg + i );

                    if ( *( msg + i ) == 0 )
                        break;
                }
                break;

            case FXA_FAXLOGGING:
                t30_attribute.FaxLogging = *msg;
                break;

            case FXA_IGNOREECHO:
                t30_attribute.FaxOptions.IgnoreEcho = *msg;
                break;

            case FXA_IGNOREECM:
                t30_attribute.FaxOptions.IgnoreECM = *msg;
                break;

            case FXA_IGNOREMMR:
                t30_attribute.FaxOptions.IgnoreMMR = *msg;
                break;

            case FXA_IGNOREMR:
                t30_attribute.FaxOptions.IgnoreMR = *msg;
                break;

            case FXA_IGNOREJBIG:
                t30_attribute.FaxOptions.IgnoreJBIG = *msg;
                break;

            case FXA_IGNOREJBIGL0:
                t30_attribute.FaxOptions.IgnoreJBIGL0 = *msg;
                break;

            case FXA_IGNOREJPEG:
                t30_attribute.FaxOptions.IgnoreJPEG = *msg;
                break;

            case FXA_IGNOREFULLCOLOR:
                t30_attribute.FaxOptions.IgnoreFullColor = *msg;
                break;

            case FXA_IGNOREPAGEMEMORY:
                t30_attribute.FaxOptions.IgnoreFileSystem = *msg;
                break;

            case FXA_IGNORE_VOICEREQ:
                t30_attribute.IgnoreVoiceRequest = *msg;
                break;

            case FXA_IMGPROC:
                t30_attribute.TxImageType = *msg;
                break;

            case FXA_INTLMODE:
                t30_attribute.FaxOptions.International = *msg;
                break;

            case FXA_LINEREQ:
                if ( T30Control )
                {
                    if ( t30_attribute.IgnoreVoiceRequest )
                        retval = E_NOTYET;
                    else if ( T30Control->PRIQCnt != 3 )
                    {
                        t30_attribute.FaxLineRequest = *msg;
                    }
                    else
                    {
                        t30_attribute.FaxVoiceRequest = *msg;
                    }
                }
                break;

            case FXA_LOCALID:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxLocalID[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_REMOTEID:

                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxRemoteID[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_LOGTYPE_INIT:
                t30_attribute.LogTypeInitialValue = *( ( UINT32 * ) msg );
                break;

            case FXA_LOG_ERRORRESULT:
                t30_attribute.LogErrorResult = *msg;
                break;

            case FXA_LOG_PRINT:
                t30_attribute.PrintLastLog = *msg;
                break;

            case FXA_LOG_REMOTENUMBER:
#if 0 // Not used any more..
                t30_attribute.RemoteNumber = *( ( PHONE_NUMBER * ) msg );
#endif
                break;

            case FXA_TXRESOLUTION:
                t30_attribute.FaxTxResolution = *msg;
                break;

            case FXA_RXRESOLUTION:
                t30_attribute.FaxRxResolution = *msg;
                break;

            case FXA_RXMAXGAIN:
                t30_attribute.RxMaxGain = *msg;
                if ( ModemControl )
                    ModemControl->Options.RxMaxGain = *msg;
                break;

            case FXA_FORCERXTOMEM:
                t30_attribute.ForceRxToMem = *msg;
                break;

            case FXA_FORCERXTOMEM_COLOR:
                t30_attribute.ForceRxToMem_Color = *msg;
                break;

            case FXA_SPEAKER_CALLMODE:
                t30_attribute.SpeakerCallMode = *msg;
                break;

            case FXA_SPEAKER_ANSWERMODE:
                t30_attribute.SpeakerAnswerMode = *msg;
                break;

            case FXA_SPEAKERVOLUME:
                t30_attribute.SpeakerVolume = *msg;
                break;

            case FXA_TXSTARTSPEED:
                t30_attribute.FaxTxStartSpeed = *msg;
                break;

            case FXA_RXSTARTSPEED:
                t30_attribute.FaxRxStartSpeed = *msg;
                break;

            case FXA_T1TIMEOUT:
                t30_attribute.T1Timeout = *( ( UINT32 * ) msg );
                break;

            case FXA_T5TIMEOUT:
                t30_attribute.T5Timeout = *( ( UINT32 * ) msg );
                break;

            case FXA_TCFERRORS:
                ModemControl->TCFThreshold = *( ( UINT16 * ) msg );
                break;

            case FXA_TEXTHEADER:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxTextHeader[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_V29EPT:
                t30_attribute.FaxOptions.V29EPT = *msg;

                if ( ModemControl )
                    ModemControl->Options.ForceEPTOn = t30_attribute.FaxOptions.V29EPT;
                break;

            case FXA_TXREQUIREPASSWORD:
                t30_attribute.TxRequirePassword = *msg;
                break;

            case FXA_RXREQUIREPASSWORD:
                t30_attribute.RxRequirePassword = *msg;
                break;

            case FXA_TXPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxLocalTxPassword[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_RXPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxLocalRxPassword[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_REMOTEPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxRemotePassword[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_TXREQUIRESUBADDRESS:
                t30_attribute.TxRequireSubaddress = *msg;
                break;

            case FXA_RXREQUIRESUBADDRESS:
                t30_attribute.RxRequireSubaddress = *msg;
                break;

            case FXA_TXSUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxLocalTxSubaddress[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_RXSUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxLocalRxSubaddress[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_REMOTESUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxRemoteSubaddress[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_POLLTXMODE:
                t30_attribute.PollTxSource = *msg;
                break;

            case FXA_POLLRXMODE:
                t30_attribute.FaxRxPolling = *msg;
                break;

            case FXA_POLLTXREQUIREPASSWORD:
                t30_attribute.PollTxRequirePassword = *msg;
                break;

            case FXA_POLLRXREQUIREPASSWORD:
                t30_attribute.PollRxRequirePassword = *msg;
                break;

            case FXA_POLLTXPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.PollLocalTxPassword[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_POLLRXPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.PollLocalRxPassword[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_POLLREMOTEPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.PollRemotePassword[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_POLLTXREQUIRESUBADDRESS:
                t30_attribute.PollTxRequireSubaddress = *msg;
                break;

            case FXA_POLLRXREQUIRESUBADDRESS:
                t30_attribute.PollRxRequireSubaddress = *msg;
                break;

            case FXA_POLLTXSUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.PollLocalTxSubaddress[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_POLLRXSUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.PollLocalRxSubaddress[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_POLLREMOTESUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.PollRemoteSubaddress[ i ] = *( msg + StandardFIFSize - 1 - i );
                break;

            case FXA_ENABLENSF:
                t30_attribute.FaxEnableNSF = *msg;
                break;

            case FXA_LOCALNSF:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxLocalNSF[ i ] = *( msg + i );
                break;

            case FXA_LOCALNSC:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxLocalNSC[ i ] = *( msg + i );
                break;

            case FXA_LOCALNSS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxLocalNSS[ i ] = *( msg + i );
                break;

            case FXA_REMOTENSF:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxRemoteNSF[ i ] = *( msg + i );
                break;

            case FXA_REMOTENSC:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxRemoteNSC[ i ] = *( msg + i );
                break;

            case FXA_REMOTENSS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    t30_attribute.FaxRemoteNSS[ i ] = *( msg + i );
                break;

            case FXA_TXIMAGEWIDTH:
                t30_attribute.FaxTxImageWidth = *msg;
                break;

            case FXA_RXIMAGEWIDTH:
                t30_attribute.FaxRxImageWidth = *msg;
                break;

            case FXA_TXDATASOURCE:
                t30_attribute.TxImageSource = *msg;
                break;

            case FXA_RETRY_NUMBER:
                t30_attribute.fax_retry_number = *msg;
                break;

            case FXA_RETRY_INTERVAL:
                t30_attribute.fax_retry_interval = *msg;
                break;

            case FXA_BRIGHTNESS:
                t30_attribute.fax_brightness = *msg;
                break;

            case FXA_CONTRAST:
                t30_attribute.fax_contrast = *msg;
                break;

            case FXA_TTI_ENABLE:
                t30_attribute.fax_enable_tti = *msg;
                break;

            case FXA_SPEAKERV8OFF:
                t30_attribute.SpeakerOffV8 = *msg;
                break;

            case FXA_RXV34ABORTBADEQM:
                t30_attribute.RxV34AbortBadEQM = *msg;
                break;

            default:
                retval = E_NOTYET;
                break;
        }
    }

    /* Get Attribute */
    else
    {
        switch ( attrib_id )
        {
            case FXA_COUNTRY_CODE:
                *msg = t30_attribute.CountryCode;
                break;

            case FXA_DIALMODE:
                *msg = t30_attribute.FaxDialMode;
                break;

            case FXA_ANSWERMODE:
                *msg = t30_attribute.FaxAnswerMode;
                break;

#if __SUPPORT_MANUAL_RECEIVE__
            case FXA_MANUALRECEIVE:
                *msg = t30_attribute.ManualReceive;
                break;

            case FXA_RECEIVESOURCE:
                *msg = t30_attribute.FaxReceiveSource;
                break;
#else
#endif

            case FXA_AUTOANSWERRINGS:
                *msg = t30_attribute.FaxAutoAnswerRings;
                break;

            case FXA_ATTENUATION:
                *msg = t30_attribute.FaxAttenuation;
                break;

            case FXA_BUFFERED_DELAFTERTX:
                *msg = t30_attribute.Buffered_DelAfterTx;
                break;

            case FXA_BUFFEREDPRINT:
                *msg = t30_attribute.BufferedPrinting;
                break;

            case FXA_CEDDETECT_TIMEOUT:
                *( ( UINT32 * ) msg ) = t30_attribute.CEDDetectionTimeout;
                break;

            case FXA_COVERPAGE:
                *msg = t30_attribute.CoverPage;
                break;

            case FXA_COVERPAGE_ID:
                *msg = t30_attribute.CoverPageID;
                break;

            case FXA_DELAYEDTXDOCID:
                *( ( UINT16 * ) msg ) = t30_attribute.FaxDelayedTxDocID;
                break;

            case FXA_DOCID:

                if ( !T30Control )
                    *( ( UINT16 * ) msg ) = ( UINT16 ) - 1;
                else if ( T30Control->Direction == T30_TRANSMIT )
                    *( ( UINT16 * ) msg ) = T30Control->T4Params.Tx.DocID;
                else
                    *( ( UINT16 * ) msg ) = T30Control->T4Params.Rx.DocID;
                break;

            case FXA_ECMMODE:
                *msg = t30_attribute.ECMMode;
                break;

            case FXA_EQUALIZER:
                *msg = t30_attribute.FaxEqualizer;
                break;

            case FXA_FALLBACK_NOOFTRIES:
                *msg = t30_attribute.NoOfTriesOnFallback;
                break;

            case FXA_FAXLOGGING:
                *msg = t30_attribute.FaxLogging;
                break;

            case FXA_FAXFORWARD:
                *msg = t30_attribute.FaxForward;
                break;

            case FXA_FAXFORWARD_NO:
                for ( i = 0; i < LEN_PHONE_NO + 1; i++ )
                {
                    *( msg + i ) = t30_attribute.FaxForwardNo[ i ];

                    if ( t30_attribute.FaxForwardNo[ i ] == 0 )
                        break;
                }
                break;

            case FXA_IGNOREECHO:
                *msg = t30_attribute.FaxOptions.IgnoreEcho;
                break;

            case FXA_IGNOREECM:
                *msg = t30_attribute.FaxOptions.IgnoreECM;
                break;

            case FXA_IGNOREMMR:
                *msg = t30_attribute.FaxOptions.IgnoreMMR;
                break;

            case FXA_IGNOREMR:
                *msg = t30_attribute.FaxOptions.IgnoreMR;
                break;

            case FXA_IGNOREJBIG:
                *msg = t30_attribute.FaxOptions.IgnoreJBIG;
                break;

            case FXA_IGNOREJBIGL0:
                *msg = t30_attribute.FaxOptions.IgnoreJBIGL0;
                break;

            case FXA_IGNOREJPEG:
                *msg = t30_attribute.FaxOptions.IgnoreJPEG;
                break;

            case FXA_IGNOREFULLCOLOR:
                *msg = t30_attribute.FaxOptions.IgnoreFullColor;
                break;

            case FXA_IGNOREPAGEMEMORY:
                *msg = t30_attribute.FaxOptions.IgnoreFileSystem;
                break;

            case FXA_IGNORE_VOICEREQ:
                *msg = t30_attribute.IgnoreVoiceRequest;
                break;

            case FXA_IMGPROC:
                if ( !T30Control )
                    *msg = t30_attribute.TxImageType;
                else
                {
                    if ( T30Control->Direction == T30_TRANSMIT )
                        temp = T30Control->T4Params.Tx.Coding;
                    else
                        temp = T30Control->T4Params.Rx.Coding;
                    *msg = temp;
                }
                break;

            case FXA_INTLMODE:
                *msg = t30_attribute.FaxOptions.International;
                break;

            case FXA_LASTTXLOG:
#if 0 // Not used any more..
                if ( !msg )
                {
                    retval = E_PARAM;
                    break;
                }

                /* Copy the log */
                {
                    LOG *src, *dst;
                    src = &( t30_attribute.LastTxLog.log );
                    dst = &( ( ( T30_LAST_LOG * ) msg )->log );
                    dst->Duration   = src->Duration;
                    dst->Type       = src->Type;
                    dst->PageCount  = src->PageCount;
                    dst->Result     = src->Result;
                    dst->Diagnostic = src->Diagnostic;

                    for ( i = 0; i < MAX_ID_SIZE; ++i )
                        dst->ConnectedId[ i ] = src->ConnectedId[ i ];

                    dst->Remote = src->Remote;
                }
                /* Copy the starting time */
                ( ( T30_LAST_LOG * ) msg )->date_time = t30_attribute.LastTxLog.date_time;

                /* And copy the error pages information */
                {
                    UINT32 *src, *dst;
                    src = &( t30_attribute.LastTxLog.errorpage[ 0 ] );
                    dst = &( ( ( T30_LAST_LOG * ) msg )->errorpage[ 0 ] );

                    for ( i = 0; i < 8; ++i, ++src, ++dst )
                        *dst = *src;
                }
#endif // #if 0
                break;

            case FXA_LINEREQ:
                *msg = t30_attribute.FaxLineRequest;
                break;

            case FXA_LOCALID:

                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxLocalID[ i ];
                break;

            case FXA_LOG_ERRORRESULT:
                *msg = t30_attribute.LogErrorResult;
                break;

            case FXA_LOG_PRINT:
                *msg = t30_attribute.PrintLastLog;
                break;

            case FXA_LOG_REMOTENUMBER:
#if 0 // Not used any more..
                *( ( PHONE_NUMBER * ) msg ) = t30_attribute.RemoteNumber;
#endif // #if 0 // Not used any more..
                break;

            case FXA_REMOTEID:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxRemoteID[ i ];
                break;

            case FXA_TXRESOLUTION:
                if ( !T30Control )
                    *msg = t30_attribute.FaxTxResolution;
                else
                    *msg = T30Control->T4Params.Tx.Res;
                break;

            case FXA_RXRESOLUTION:
                if ( !T30Control )
                    *msg = t30_attribute.FaxRxResolution;
                else
                    *msg = T30Control->T4Params.Rx.Res;
                break;

            case FXA_RXMAXGAIN:
                *msg = t30_attribute.RxMaxGain;
                break;

            case FXA_SPEAKER_CALLMODE:
                *msg = t30_attribute.SpeakerCallMode;
                break;

            case FXA_SPEAKER_ANSWERMODE:
                *msg = t30_attribute.SpeakerAnswerMode;
                break;

            case FXA_SPEAKERVOLUME:
                *msg = t30_attribute.SpeakerVolume;
                break;

            case FXA_FORCERXTOMEM:
                *msg = t30_attribute.ForceRxToMem;
                break;

            case FXA_FORCERXTOMEM_COLOR:
                *msg = t30_attribute.ForceRxToMem_Color;
                break;

            case FXA_TXSTARTSPEED:
                *msg = t30_attribute.FaxTxStartSpeed;
                break;

            case FXA_RXSTARTSPEED:
                *msg = t30_attribute.FaxRxStartSpeed;
                break;

            case FXA_T1TIMEOUT:
                *( ( UINT32 * ) msg ) = t30_attribute.T1Timeout;
                break;

            case FXA_T5TIMEOUT:
                *( ( UINT32 * ) msg ) = t30_attribute.T5Timeout;
                break;

            case FXA_TCFERRORS:
                *( ( UINT16 * ) msg ) = ModemControl->TCFThreshold;
                break;

            case FXA_TEXTHEADER:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxTextHeader[ i ];
                break;

            case FXA_V29EPT:
                *msg = t30_attribute.FaxOptions.V29EPT;
                break;

            case FXA_VOICEREQ:
                break;

            case FXA_PAGENO:

                if ( T30Control
                  && ( ( T30Control->Direction == T30_RECEIVE
                      && !T30Control->ECMEnabled )
                    || !T30Control->PageNum ) )
                    *msg = T30Control->PageNum + 1;
                else if ( T30Control )
                    *msg = T30Control->PageNum;
                else
                    *msg = 0;
                break;

            case FXA_TXREQUIREPASSWORD:
                *msg = t30_attribute.TxRequirePassword;
                break;

            case FXA_RXREQUIREPASSWORD:
                *msg = t30_attribute.RxRequirePassword;
                break;

            case FXA_TXPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxLocalTxPassword[ i ];
                break;

            case FXA_RXPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxLocalRxPassword[ i ];
                break;

            case FXA_REMOTEPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxRemotePassword[ i ];
                break;

            case FXA_TXREQUIRESUBADDRESS:
                *msg = t30_attribute.TxRequireSubaddress;
                break;

            case FXA_RXREQUIRESUBADDRESS:
                *msg = t30_attribute.RxRequireSubaddress;
                break;

            case FXA_TXSUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxLocalTxSubaddress[ i ];
                break;

            case FXA_RXSUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxLocalRxSubaddress[ i ];
                break;

            case FXA_REMOTESUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.FaxRemoteSubaddress[ i ];
                break;

            case FXA_POLLTXMODE:
                *msg = t30_attribute.PollTxSource;
                break;

            case FXA_POLLRXMODE:
                *msg = t30_attribute.FaxRxPolling;
                break;

            case FXA_POLLTXREQUIREPASSWORD:
                *msg = t30_attribute.PollTxRequirePassword;
                break;

            case FXA_POLLRXREQUIREPASSWORD:
                *msg = t30_attribute.PollRxRequirePassword;
                break;

            case FXA_POLLTXPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.PollLocalTxPassword[ i ];
                break;

            case FXA_POLLRXPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.PollLocalRxPassword[ i ];
                break;

            case FXA_POLLREMOTEPASSWORD:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.PollRemotePassword[ i ];
                break;

            case FXA_POLLTXREQUIRESUBADDRESS:
                *msg = t30_attribute.PollTxRequireSubaddress;
                break;

            case FXA_POLLRXREQUIRESUBADDRESS:
                *msg = t30_attribute.PollRxRequireSubaddress;
                break;

            case FXA_POLLTXSUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.PollLocalTxSubaddress[ i ];
                break;

            case FXA_POLLRXSUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.PollLocalRxSubaddress[ i ];
                break;

            case FXA_POLLREMOTESUBADDRESS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + StandardFIFSize - 1 - i ) = t30_attribute.PollRemoteSubaddress[ i ];
                break;

            case FXA_ENABLENSF:
                *msg = t30_attribute.FaxEnableNSF;
                break;

            case FXA_LOCALNSF:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + i ) = t30_attribute.FaxLocalNSF[ i ];
                break;

            case FXA_LOCALNSC:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + i ) = t30_attribute.FaxLocalNSC[ i ];
                break;

            case FXA_LOCALNSS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + i ) = t30_attribute.FaxLocalNSS[ i ];
                break;

            case FXA_REMOTENSF:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + i ) = t30_attribute.FaxRemoteNSF[ i ];
                break;

            case FXA_REMOTENSC:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + i ) = t30_attribute.FaxRemoteNSC[ i ];
                break;

            case FXA_REMOTENSS:
                for ( i = 0; i < StandardFIFSize; i++ )
                    *( msg + i ) = t30_attribute.FaxRemoteNSS[ i ];
                break;

            case FXA_TXIMAGEWIDTH:
                *msg = t30_attribute.FaxTxImageWidth;
                break;

            case FXA_RXIMAGEWIDTH:
                *msg = t30_attribute.FaxRxImageWidth;
                break;

            case FXA_TXDATASOURCE:
                *msg = t30_attribute.TxImageSource;
                break;

            case FXA_RETRY_NUMBER:
                *msg = t30_attribute.fax_retry_number;
                break;

            case FXA_RETRY_INTERVAL:
                *msg = t30_attribute.fax_retry_interval;
                break;

            case FXA_BRIGHTNESS:
                *msg = t30_attribute.fax_brightness;
                break;

            case FXA_CONTRAST:
                *msg = t30_attribute.fax_contrast;
                break;

            case FXA_TTI_ENABLE:
                *msg = t30_attribute.fax_enable_tti;
                break;

            case FXA_SPEAKERV8OFF:
                *msg = t30_attribute.SpeakerOffV8;
                break;

            case FXA_RXV34ABORTBADEQM:
                *msg = t30_attribute.RxV34AbortBadEQM;
                break;

            default:
                *msg = 0;
                retval = E_NOTYET;
                break;
        }
    }

    return ( retval );
}


/////////////////////////////////////////////////////////////////////
// Function name  : T30InitJob
//
// Return type    : ERCODE
//
// Argument       : Index
//
// Description    : Initializes the T.30 for a fax session
//
/////////////////////////////////////////////////////////////////////

ERCODE T30InitJob( void )
{
    ERCODE status = E_OK;
    UINT8 i;

    TRACE_FAX( "T30InitJob\n" );

    for ( i = 0; i < FCFReceiveTableSize; i++ )
        FIFReceiveTable[ i ] = 0;

    for ( i = 0; i < FCFSendTableSize; i++ )
        FIFSendTable[ i ] = 0;

    if ( HDLCAllocateMemory() )
    {
        return ( E_EMPTY );
    }

    /* Make sure the T30 control structure is NULL */
    T30Control = ( T_T30_CONTROL* ) NULL;

    /* Allocate the control structure for T.30 */
    if ( OSC_Allocate_Zero_Memory( (OSC_MEMORY_POOL *)NULL, ( void ** ) &T30Control, sizeof( T_T30_CONTROL ),  TRUE ) != E_OK )
    {
        TRACE_ERROR( "T30Control failed.\n" );
        return ( E_EMPTY );
    }

    T30Control->rcvd_post_msg = 0xFF;       // no post message received

    /* Initialize T.30 FIF tables with dynamic addresses */

    FIFSendTable[ NSFtbl ]  = t30_attribute.FaxLocalNSF;
    FIFSendTable[ NSCtbl ]  = t30_attribute.FaxLocalNSC;
    FIFSendTable[ NSStbl ]  = t30_attribute.FaxLocalNSS;
    FIFSendTable[ SIDtbl ]  = t30_attribute.FaxLocalTxPassword;
    FIFSendTable[ SUBtbl ]  = t30_attribute.FaxLocalTxSubaddress;
    FIFSendTable[ PWDtbl ]  = t30_attribute.PollLocalRxPassword;
    FIFSendTable[ SEPtbl ]  = t30_attribute.PollLocalRxSubaddress;
    FIFSendTable[ CSItbl ]  = t30_attribute.FaxLocalID;
    FIFSendTable[ TSItbl ]  = t30_attribute.FaxLocalID;
    FIFSendTable[ CIGtbl ]  = t30_attribute.FaxLocalID;
    FIFSendTable[ DIStbl ]  = T30Control->LocalCap;
    FIFSendTable[ DCStbl ]  = T30Control->LocalCap;
    FIFSendTable[ DTCtbl ]  = T30Control->LocalCap;

    FIFReceiveTable[ NSFtbl ] = T30Control->RemoteCap;
    FIFReceiveTable[ NSCtbl ] = T30Control->RemoteCap;
    FIFReceiveTable[ NSStbl ] = T30Control->RemoteCap;
    FIFReceiveTable[ SIDtbl ] = t30_attribute.FaxRemotePassword;
    FIFReceiveTable[ SUBtbl ] = t30_attribute.FaxRemoteSubaddress;
    FIFReceiveTable[ PWDtbl ] = t30_attribute.PollRemotePassword;
    FIFReceiveTable[ SEPtbl ] = t30_attribute.PollRemoteSubaddress;
    FIFReceiveTable[ CSItbl ] = t30_attribute.FaxRemoteID;
    FIFReceiveTable[ TSItbl ] = t30_attribute.FaxRemoteID;
    FIFReceiveTable[ CIGtbl ] = t30_attribute.FaxRemoteID;
    FIFReceiveTable[ DIStbl ] = T30Control->RemoteCap;
    FIFReceiveTable[ DCStbl ] = T30Control->RemoteCap;
    FIFReceiveTable[ DTCtbl ] = T30Control->RemoteCap;

    /* Init flags and variables */
    T30InitVars();

    T30Control->LogTypeInitialValue    = t30_attribute.LogTypeInitialValue;
    T30Control->TxAllRcvdFaxes         = t30_msg.TxAllRcvdFaxes;
    T30Control->T4Params.Rx.FaxStorage = FALSE;
    T30Control->T4Params.Rx.BufferedRx = t30_attribute.BufferedPrinting
                                         || !T30Control->T4Params.Rx.FaxStorage;

    ModemInitVars();

    if ( ModemControl )
    {
        ModemControl->Options.FaxTxLevel = t30_attribute.FaxAttenuation;
        ModemControl->Options.FaxEqzr    = t30_attribute.FaxEqualizer;
        ModemControl->Options.RxMaxGain  = t30_attribute.RxMaxGain;
        ModemControl->Options.ForceEPTOn = t30_attribute.FaxOptions.V29EPT;
    }

    /* Save the original resolution */
    T30Control->OrigRes = t30_attribute.FaxTxResolution;
    T30Control->T4Params.Tx.ImageType = t30_attribute.TxImageType;

    if ( t30_attribute.FaxOptions.International )
    {
        T30Control->FrameRepeat = 2;
    }
    else
    {
        T30Control->FrameRepeat = 3;
    }

    /* Init. the NSF/NSS/NSC frame country & maker codes */
    if ( t30_attribute.FaxEnableNSF
      && ( FIFSendTable[ NSFtbl ][ 0 ] == 0 )
      && ( FIFSendTable[ NSFtbl ][ 1 ] == 0 ) )
    {
        FIFSendTable[ NSFtbl ][ 0 ] = t30_attribute.CountryCode;

        for ( i = 0; i < NSTDHeaderSize; i++ )
        {
            FIFSendTable[ NSFtbl ][ i + 1 ] = NSTDHeader[ i ];
        }
    }

    /* Set up the starting parameters */
    T30Control->Direction = t30_msg.Direction;

    if ( t30_msg.Direction == T30_TRANSMIT )
    {
        T30Control->State = STATE_TX_PHASE_A;

		//TX global variable init
		isTxImageProcessDone = 0;

        if ( t30_msg.Polling )
        {
            t30_attribute.FaxRxPolling = 1;
        }

        T30Control->TxFromMemFlg = 1;
        T30Control->T4Params.Tx.DocID = fsu_get_doc_id(fsu_job_num_send);

        if ( t30_msg.Manual )
            T30Control->ManualMode = 1;
    }
    else
    {
        T30Control->State = STATE_RX_PHASE_A;
    }

    TRACE_FAX( "T30InitJob(): status %d\n", status );

    return ( status );
}



/////////////////////////////////////////////////////////////////////
// Function name  : T30ShutdownJob
//
// Return type    : none
//
// Argument       : none
//
// Description    : Terminates fax TX or RX
//
/////////////////////////////////////////////////////////////////////
void T30ShutdownJob( void )
{
    TRACE_FAX( "T30ShutdownJob: Enter\n" );

    ModemControl->State = MODEM_IDLE;

#if V34_SUPPORT
    /* to stop sending dummy signal */
    if ( T30Control->V34Mode )        // THY
        ModemStopV34();
#endif

    gFaxEndOfFax = T30Control->end_of_fax = 1;

#ifdef OEM_FAX_REPORT
    CxProtocolReport( T30_END, &T30Control->Status, 1, T30Control->Direction );
#endif

    T30Control->State = STATE_IDLE;		//add.bong.2011-12-19 9:33am

    /* Restore one-shot attributes */
    t30_attribute.FaxRxPolling = 0;

    /* Deallocate the ECM control structure if necessary */
    ECMShutdownJob();

    /* Make sure modem interrupts are disabled */
    ModemClearIRQ();

    /* Deallocate and clear out the T.30 control structure */
    OSC_Deallocate_Memory( ( void * ) T30Control );

    T30Control = ( T_T30_CONTROL * ) NULL;

    HDLCDeAllocateMemory();

    TRACE_FAX( "T30ShutdownJob: END\n" );
}


/////////////////////////////////////////////////////////////////////
// Function name  : T30InitVars
//
// Return type    : none
//
// Argument       : none
//
// Description    : Initializes fax variables for TX or RX
//
/////////////////////////////////////////////////////////////////////
void T30InitVars( void )
{
    TRACE_FAX( "T30InitVars\n" );

    T30Control->TriedV8 = 0;
    T30Control->PageNum = 0;
    T30Control->MemPageNum = 1;
    T30Control->FirstCmdFlg = 1;
    T30Control->end_of_fax = gFaxEndOfFax = 0;
    T30Control->PRIQCnt = 3;
    T30Control->ModemSpeed = t30_attribute.FaxTxStartSpeed;

    t30_attribute.FaxLineRequest = 0;
    t30_attribute.FaxVoiceRequest = 0;
}


void T30SpeakerControl( UINT8 control )
{
    UINT8 volume = 0;

    Fax_API_T30Attribute(GET, FXA_SPEAKERVOLUME, &volume);

    TRACE_FAX( "T30SpeakerControl() control %d, volume %d\n", control, volume );
    TRACE_FAX( "                   call %d, answer %d\n", t30_attribute.SpeakerCallMode, t30_attribute.SpeakerAnswerMode );

    switch ( control )
    {
        case T30_SPEKER_OFF:

            if ( t30_attribute.SpeakerCallMode != ALWAYS_OFF )
            {
                if ( t30_attribute.SpeakerCallMode == T30Control->State )
                {
                    ModemSetSpeakerVolume( 0 );
                }
            }

            if ( t30_attribute.SpeakerAnswerMode != ALWAYS_OFF )
            {
                if ( t30_attribute.SpeakerAnswerMode == T30Control->State )
                {
                    ModemSetSpeakerVolume( 0 );
                }
            }
            break;

        case T30_SPEKER_CALL:

            if ( t30_attribute.SpeakerCallMode != ALWAYS_OFF )
            {
                ModemSetSpeakerVolume( volume );
            }
            break;

        case T30_SPEKER_ANS:

            if ( t30_attribute.SpeakerAnswerMode != ALWAYS_OFF )
            {
                ModemSetSpeakerVolume( volume );
            }
            break;

        case T30_SPEKER_MANUAL_ON:

            ModemSetSpeakerVolume( volume );
            break;

        case T30_SPEKER_MANUAL_OFF:

            ModemSetSpeakerVolume( 0 );
            break;

        case T30_SPEKER_V8_OFF:

            if ( t30_attribute.SpeakerOffV8 )
            {
                if ( t30_attribute.SpeakerCallMode == STATE_TX_PHASE_B )
                {
                    ModemSetSpeakerVolume( 0 );
                }

                if ( t30_attribute.SpeakerAnswerMode == STATE_RX_PHASE_B )
                {
                    ModemSetSpeakerVolume( 0 );
                }
            }
            break;

        default:
            break;
    }

}


void T30PhoneLineMonitor( void )
{
    extern UINT16 ModemLineMonitor( BOOL start );

    if ( ( T30Control != 0 ) && ( T30Control->State != STATE_IDLE ) )
    {
        if ( ModemLineMonitor( TRUE ) == 0 )
        {
            TRACE_FAX( "T.30 No Line\n" );

            if ( T30Control->Direction == T30_RECEIVE )
            {
                T30Control->State = STATE_RX_ABORT;
            }
            else
            {
                T30Control->State = STATE_TX_ABORT;
            }

            T30Control->Status = FAX_ERROR_PHONE_LINE_ERROR;
            ModemControl->Status |= MODEM_ABORT;
        }
    }
    else
    {
        ModemLineMonitor( FALSE );  // Reset timer
    }
}


void fsu_notify_no_mem_error_ram (UINT16 Error)
{
    TRACE_FAX( "No Mem Error RAM...\n" );

    if (T30Control != (T_T30_CONTROL *)NULL)
    {
        T30Control->Status = Error;
    }
}

