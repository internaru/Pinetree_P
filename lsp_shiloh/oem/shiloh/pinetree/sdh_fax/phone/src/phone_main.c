/*
 *  $RCSfile: phone_main.c,v $
 * $Revision: 1.42 $
 *   $Author: miurat $
 *     $Date: 2012/08/21 22:15:31 $
 */
/************************************************************************
 *
 *      phone_main.c
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"
#include "../../CommonHeader/SDHFaxCommonHeader.h"
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "MODEMX.H"
#include "T30X.H"
#include "PHONEX.H"
#include "phone.h"
#include "fsu_api.h"
#include "faxapp_notify.h"

//.. declare variables
OSC_SEMAPHORE sem_phone_task;
OSC_SEMAPHORE sem_phone_start;

UINT32 gFaxCNGStartTime = 0;
UINT8 offHookDone = 0;

static BOOL   phone_task_stop;
static BOOL   phone_task_skip;

static UINT8 dial_number[ PHONE_DIAL_BYTES ];
static UINT8 phone_state;
static UINT8 answer_mode;
static UINT8 answer_rings;
static UINT8 dial_mode;
static UINT8 dial_tone;
static UINT8 busy_tone;
static UINT8 cng_tone;
static UINT8 fsk_tone;
static UINT8 ci_tone;
static UINT8 ans_tone;
static UINT8 saved_count;
static BOOL offhook = FALSE;
static BOOL sending = FALSE;

BOOLEAN gFaxNoAnswerFlag = FALSE;
UINT8 isPhoneBusy = 0;
UINT8 isPhoneNoAnswer = 0;
UINT8 isPhoneNoDial = 0;
UINT8 isPhoneAbort = 0;
BOOLEAN isDialCancel = FALSE;
#if __SUPPORT_MANUAL_TRANSMIT__ || __SUPPORT_MANUAL_RECEIVE__
UINT8 ExtPhoneDialBuffer[LEN_PHONE_NO + 1] = {0, };
UINT8 ExtPhoneDialIndex = 0;
#else
#endif

extern void ModemExclusionRelay( BOOL onoff );
extern void T30PhoneLineMonitor( void );
extern UINT16 ModemLineMonitor( BOOL start );
extern void ModemOnHook( UINT8 jcid );
extern void ModemOffHook( UINT8 jcid );
extern int faxapp_check_on_hook_button_pressed (void);
extern int faxapp_get_OnHookButtonPressed();
extern void faxapp_set_OnHookButtonPressed(int value);
extern void faxapp_activate_on_hook (void);


/*****************************************************************************
 * Function:    job_phone_start
 * Purpose:     Create a task and semaphore for phone
 * Inputs:		None
 * Outputs:		None
 *****************************************************************************/
#include <pthread.h>
#include "posix_ostools.h"
static void *job_phone_main (void *unused);
#define PHONE_MAIN_STACK_SIZE     (1024 * 10 * 3)
static unsigned char SDHFaxPhoneStack[PHONE_MAIN_STACK_SIZE];
static pthread_t sdhfaxphone_thd_id;
extern int isFSUPhoneReady;
extern UINT8 gRingDetect;

void job_phone_start( void )
{
    int retcode = 0;

    // create semaphore
    OSC_Create_Semaphore( &sem_phone_task, "phone_task_semaphore", 0, 0 );
    OSC_Create_Semaphore( &sem_phone_start, "phone_task_semaphore", 0, 0 );

    retcode = posix_create_thread(&sdhfaxphone_thd_id, job_phone_main, 0, "tSDHFaxPhone", SDHFaxPhoneStack, PHONE_MAIN_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    XASSERT(retcode == 0, retcode);
}


void phone_wait_semaphore( void )
{
    OSC_Take_Semaphore( &sem_phone_task, OSC_SUSPEND );
}


void phone_release_semaphore( void )
{
    OSC_Give_Semaphore( &sem_phone_task );
}


void activate_phone_job( void )
{
    OSC_Give_Semaphore( &sem_phone_start );
}


void phone_stop_task( void )
{
    phone_task_stop = TRUE;
}


void phone_skip_task( void )
{
    phone_task_skip = TRUE;
}


void phone_continue_task( void )
{
    phone_task_skip = FALSE;
}


/*****************************************************************************
 * Function:    job_phone_main
 * Purpose:     transfer image data to next job
 * Inputs:		None
 * Outputs:		None
 *****************************************************************************/
static void *job_phone_main (void *unused)
{
    isFSUPhoneReady = 1;

    OSC_Take_Semaphore( &sem_phone_start, OSC_SUSPEND );

    set_phone_state( PHONE_STATE_INIT );

    while ( 1 )
    {
        phone_task();

#if __OVERRUN_ERROR__
        OSC_Sleep_Task( 20 );
#else
        OSC_Sleep_Task( (PHONE_TASK_PERIODIC_TIME) );
#endif
    }

    return 0;
}


void phone_init_after_wakeup( void )
{
    Init_RingState( TRUE );
    SetUpToneDetection( TONE_DETECT_OFF, 1 );
    SetUpToneDetection( TONE_DETECT_CNG, 1 );
}


static void phone_onhook_for_ring( void )
{
    ModemExclusionRelay( FALSE );
    OSC_Sleep( 500 );
    ModemOnHook( 0 );
    Init_RingState( TRUE );
    T30Attribute(GET, FXA_ANSWERMODE, &answer_mode);
    T30Attribute(GET, FXA_AUTOANSWERRINGS, &answer_rings);
    set_phone_status( TONE_DETECT_OFF, TRUE );
    SetUpToneDetection( TONE_DETECT_OFF, 1 );
    SetUpToneDetection( TONE_DETECT_CNG, 1 );
    SetMaxCNGHalfCycle( 2 );
    SetUpDTMFDetection();
}


void phone_task( void )
{
    switch ( phone_state )
    {
        case PHONE_STATE_IDLE:
        {
            isDialCancel = FALSE;
            break;
        }

        case PHONE_STATE_INIT:
        {
            isDialCancel = FALSE;
            ModemReinitJob();
            phone_onhook_for_ring();
            set_phone_state( PHONE_STATE_RING_CHECK );

            OSC_Sleep_Task( 500 );

            if(faxapp_get_OnHookButtonPressed() != ON_HOOK_STATE_DISABLE)
            {
                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_ON_HOOK_DONE, 1, 0, 0, 0, EMPTY_JOB_NUM);
                faxapp_set_OnHookButtonPressed( ON_HOOK_STATE_DISABLE );
            }
            else
            {
                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_ON_HOOK_DONE, 0, 0, 0, 0, EMPTY_JOB_NUM);
            }
            offHookDone = 0;

            break;
        }

        case PHONE_STATE_ONHOOK:
        {
            ModemExclusionRelay( FALSE );
            OSC_Sleep_Task( 500 );
            ModemOnHook( 0 );
            CxPhoneReport( PHONE_ONHOOK, NULL );
            set_phone_state( PHONE_STATE_IDLE );
            break;
        }

        case PHONE_STATE_ONHOOK_FOR_RING:
        {
            OSC_Sleep_Task( 3000 );
            set_phone_state( PHONE_STATE_RING_CHECK );
            CxPhoneReport( FAX_END, NULL );

            if(faxapp_get_OnHookButtonPressed() != ON_HOOK_STATE_DISABLE)
            {
                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_ON_HOOK_DONE, 1, 0, 0, 0, EMPTY_JOB_NUM);
                faxapp_set_OnHookButtonPressed( ON_HOOK_STATE_DISABLE );
            }
            else
            {
                faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_ON_HOOK_DONE, 0, 0, 0, 0, EMPTY_JOB_NUM);
            }
            offHookDone = 0;

            break;
        }

        case PHONE_STATE_OFFHOOK:
        {
            ModemOffHook( 0 );
            CxPhoneReport( PHONE_OFFHOOK, NULL );
            OSC_Sleep_Task( 300 );
            ModemExclusionRelay( TRUE );
            ModemLineMonitor( FALSE );
            T30SpeakerControl( T30_SPEKER_CALL );
            set_phone_state( PHONE_STATE_NOLINE_CHECK );

			OSC_Sleep_Task( 500 );

			faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_OFF_HOOK_DONE, 0, 0, 0, 0, EMPTY_JOB_NUM);
			faxapp_set_OnHookButtonPressed( ON_HOOK_STATE_OFFHOOK );
			offHookDone = 1;

            break;
        }

        case PHONE_STATE_OFFHOOK_ANS:
        {
            ModemOffHook( 0 );
            CxPhoneReport( PHONE_OFFHOOK, NULL );
            OSC_Sleep_Task( 500 );
            ModemExclusionRelay( TRUE );
            OSC_Sleep_Task( 500 );
            T30SpeakerControl( T30_SPEKER_ANS );
            //OSC_Sleep_Task( 3000 );
            OSC_Sleep_Task( 1000 );
            ModemLineMonitor( FALSE );
            CxPhoneReport( RX_PHASE_A, NULL );
            set_fsu_state( FSU_STATE_RX_START );
            set_phone_state( PHONE_STATE_NOLINE_CHECK );
            break;
        }

        case PHONE_STATE_OFFHOOK_CALL:
        {
            ModemOffHook( 0 );
            CxPhoneReport( PHONE_OFFHOOK, NULL );
            OSC_Sleep_Task( 500 );
            ModemExclusionRelay( TRUE );
            T30SpeakerControl( T30_SPEKER_CALL );
            ModemLineMonitor( FALSE );
            set_phone_state( PHONE_STATE_DIAL_BUSY_TONE_SETUP );
            break;
        }

#if __SUPPORT_MANUAL_TRANSMIT__
        case PHONE_STATE_OFFHOOK_CALL_MANUAL:
        {
            ModemOffHook( 1 );
            CxPhoneReport( PHONE_OFFHOOK, NULL );
            OSC_Sleep_Task( 500 );
            ModemExclusionRelay( TRUE );
            T30SpeakerControl( T30_SPEKER_CALL );
            ModemLineMonitor( FALSE );
            set_phone_state( PHONE_STATE_ANS_FSK_BUSY_TONE_SETUP );
            break;
        }
#else
#endif

        case PHONE_STATE_DIAL_BUSY_TONE_SETUP:
        {
            SetUpToneDetection( TONE_DETECT_DIAL_BUSY, 1 );
            set_phone_state( PHONE_STATE_DIAL_BUSY_TONE_CHECK );
            break;
        }

        case PHONE_STATE_DIAL_BUSY_TONE_CHECK:
        {
            if ( PDP.BlindDial )
            {
                OSC_Sleep_Task( PTP.BlindDialDelay );
                set_phone_state( PHONE_STATE_DIAL_TONE_DETECTED );
            }
            else
            {
                UINT8 ret = CheckToneDetection( TONE_DETECT_DIAL_BUSY, PHONE_TASK_PERIODIC_TIME );

                switch ( ret )
                {
                    case TONE_DETECT_DIAL:
                    {
                        set_phone_state( PHONE_STATE_DIAL_TONE_DETECTED );
                        break;
                    }

                    case TONE_DETECT_BUSY:
                    {
                        set_phone_state( PHONE_STATE_BUSY_TONE_DETECTED );
                        break;
                    }

                    case TONE_DETECT_ABORT:
                    {
                        isPhoneNoDial++;
                        CxPhoneReport( PHONE_NODIAL, NULL );
                        set_phone_state( PHONE_STATE_ABORT );
                        break;
                    }

                    default:
                        break;
                }
            }
            break;
        }

        case PHONE_STATE_DIAL_TONE_DETECTED:
        {
            set_phone_status( TONE_DETECT_DIAL, TRUE );
            CxPhoneReport( PHONE_DIAL, dial_number );
            phone_auto_dial( dial_number, dial_mode );
            OSC_Sleep_Task( 1000 );
            set_phone_state( PHONE_STATE_ANS_FSK_BUSY_TONE_SETUP );
            break;
        }

        case PHONE_STATE_DIAL_DIGIT:
        {
            CxPhoneReport( PHONE_DIAL, dial_number );
            phone_auto_dial( dial_number, dial_mode );
            set_phone_state( PHONE_STATE_IDLE );
            break;
        }

        case PHONE_STATE_ANS_FSK_BUSY_TONE_SETUP:
        {
            SetMaxBusyToneTime( 1 );
            SetUpToneDetection( TONE_DETECT_ANS_FSK_BUSY, 1 );
            ModemTxCNG();
            CxPhoneReport( PHONE_CNG, NULL );
            gFaxCNGStartTime = ospl_time_get();

            if ( PDP.BlindDial ) {
                OSC_Sleep_Task( PTP.BlindDialDelay );
            }
            ModemLineMonitor( FALSE );
            set_phone_state( PHONE_STATE_ANS_FSK_BUSY_TONE_CHECK );
            break;
        }

        case PHONE_STATE_ANS_FSK_BUSY_TONE_CHECK:
        {
            UINT8 ret = CheckToneDetection( TONE_DETECT_ANS_FSK_BUSY, PHONE_TASK_PERIODIC_TIME );

            switch ( ret )
            {
                case TONE_DETECT_ANS:
                {
                	OSC_Sleep_Task( 150 );		/* To fix V8 nego timeout */
                    set_phone_state( PHONE_STATE_ANS_TONE_DETECTED );
                    break;
                }

                case TONE_DETECT_FSK:
                {
                    set_phone_state( PHONE_STATE_FSK_TONE_DETECTED );
                    break;
                }

                case TONE_DETECT_BUSY:
                {
                    set_phone_state( PHONE_STATE_BUSY_TONE_DETECTED );
                    break;
                }

                case TONE_DETECT_ABORT:
                {
                    isPhoneNoAnswer++;
                    CxPhoneReport( PHONE_NOANSWER, NULL );
                    set_phone_state( PHONE_STATE_ABORT );
                    break;
                }

                default:
                    break;
            }

            if ( ModemLineMonitor( TRUE ) == 0 )
            {
                isPhoneAbort++;
                set_phone_state( PHONE_STATE_ABORT );
            }
            break;
        }

        case PHONE_STATE_BUSY_TONE_DETECTED:
        {
            isPhoneBusy++;
            fsu_tx_fax_cancel();
            CxPhoneReport( PHONE_BUSY, NULL );
            set_phone_status( TONE_DETECT_BUSY, TRUE );
            SetUpToneDetection( TONE_DETECT_OFF, 1 );
            ModemOnHook( 0 );
            set_fsu_state( FSU_STATE_TX_ABORT );
            set_phone_state( PHONE_STATE_IDLE );
            break;
        }

        case PHONE_STATE_ANS_TONE_DETECTED:
        {
            CxPhoneReport( PHONE_DETECTEDFAX, NULL );
            set_phone_status( TONE_DETECT_ANS, TRUE );
            SetUpToneDetection( TONE_DETECT_OFF, 1 );
            set_fsu_state( FSU_STATE_TX_START );
            set_phone_state( PHONE_STATE_NOLINE_CHECK );
            CxPhoneReport( TX_PHASE_A, NULL );
            break;
        }

        case PHONE_STATE_FSK_TONE_DETECTED:
        {
            CxPhoneReport( PHONE_DETECTEDFAX, NULL );
            set_phone_status( TONE_DETECT_FSK, TRUE );
            SetUpToneDetection( TONE_DETECT_OFF, 1 );
            set_fsu_state( FSU_STATE_TX_START );
            set_phone_state( PHONE_STATE_NOLINE_CHECK );
            CxPhoneReport( TX_PHASE_A, NULL );
            break;
        }

        case PHONE_STATE_RING_CHECK:
        {
            INT8 value = 0;
            if(gRingDetect)
            {
				value = Detect_Ring_CID( PHONE_TASK_PERIODIC_TIME );
            }
            LINE_STATUS status = SSFLineInUse();

            if ( value == 0 )
            {
                saved_count = 0;
            }
            else if ( value < 0 )
            {
                gFaxNoAnswerFlag = TRUE;

                Init_RingState( TRUE );
            }
            else
            {
                if ( ( value == 1 ) && ( saved_count == 0 ) )  // one time update
                {
                    T30Attribute(GET, FXA_ANSWERMODE, &answer_mode);
                    T30Attribute(GET, FXA_AUTOANSWERRINGS, &answer_rings);
                }

                saved_count = value;

                switch ( answer_mode )
                {
                    case AUTO_ANSWER:
                    {
                        if ( ( answer_rings > 0 ) && ( answer_rings == value ) )
                        {
                            saved_count = 0;
                            Init_RingState( FALSE );
                            set_phone_state( PHONE_STATE_OFFHOOK_ANS );
                            OSC_Sleep_Task( 1000 );
                        }
                        break;
                    }

                    case MANUAL_ANSWER:
                    default:
                        break;
                }
            }

            switch ( status )
            {
                case LINE_INUSE:
                {
                    SetUpToneDetection( TONE_DETECT_CNG, 1 );
                    SetMaxCNGHalfCycle( 2 );
                    set_phone_state( PHONE_STATE_LINEINUSE_CHECK );
                    CxPhoneReport( PHONE_LINEINUSE, NULL );
                    break;
                }

                case LINE_ONHOOK:
                case LINE_NOLINE:
                case LINE_OFFHOOK:
                default:
                    break;
            }
            break;
        }

        case PHONE_STATE_CID_START:
        {
            CIDType1Start();
            set_phone_state( PHONE_STATE_RING_CHECK );
            break;
        }

        case PHONE_STATE_LINEINUSE_CHECK:
        {
            LINE_STATUS status = SSFLineInUse();

            switch ( status )
            {
                case LINE_ONHOOK:
                {
                    set_phone_state( PHONE_STATE_RING_CHECK );
                    CxPhoneReport( PHONE_LINENOTUSE, NULL );
                    break;
                }

                case LINE_INUSE:
                {
                    UINT8 ret = CheckToneDetection( TONE_DETECT_CNG, 30 /*PHONE_TASK_PERIODIC_TIME*/ );

                    if ( (answer_mode == AUTO_ANSWER) && (ret == TONE_DETECT_CNG) )
                    {
						if(gRingDetect)
						{
							set_phone_state( PHONE_STATE_OFFHOOK_ANS );
						}
                    }
                    else
                    {
                        UINT8 DTMF = CheckDTMFDetection();

                        if ( DTMF )
                        {
                            TRACE_PHONE( "DTMF[ %c ]\n", DTMF );

#if __SUPPORT_MANUAL_TRANSMIT__
                            if (ExtPhoneDialIndex == 0)
                            {
                                memset(ExtPhoneDialBuffer, 0x00, sizeof(ExtPhoneDialBuffer));
                            }
                            else if ( ExtPhoneDialIndex == LEN_PHONE_NO - 1 )
                            {
								char tmp1 = ExtPhoneDialBuffer[ExtPhoneDialIndex - 2];
								char tmp2 = ExtPhoneDialBuffer[ExtPhoneDialIndex - 1];
								memset(ExtPhoneDialBuffer, 0x00, sizeof(ExtPhoneDialBuffer));
								ExtPhoneDialIndex = 0;
								ExtPhoneDialBuffer[ExtPhoneDialIndex++] = tmp1;
								ExtPhoneDialBuffer[ExtPhoneDialIndex++] = tmp2;
								ExtPhoneDialBuffer[ExtPhoneDialIndex] = 0;
                            }

                            ExtPhoneDialBuffer[ExtPhoneDialIndex] = DTMF;
                            ExtPhoneDialIndex++;
                            ExtPhoneDialBuffer[ExtPhoneDialIndex] = 0;

                            printf("\n(YHCHO) %s() - ExtPhoneDialBuffer: %s\n", __FUNCTION__, ExtPhoneDialBuffer);
#else
#endif
                        }
                    }
                    break;
                }

                case LINE_NOLINE:
                {
                    set_phone_state( PHONE_STATE_RING_CHECK );
                    CxPhoneReport( PHONE_NOLINE, NULL );
                    break;
                }

                case LINE_OFFHOOK:
                default:
                    break;
            }
            break;
        }

        case PHONE_STATE_NOLINE_CHECK:
        {
            LINE_STATUS status = SSFLineInUse();

            switch ( status )
            {
                case LINE_ONHOOK:
                {
                    set_phone_state( PHONE_STATE_RING_CHECK );
                    CxPhoneReport( PHONE_LINENOTUSE, NULL );
                    break;
                }

                case LINE_INUSE:
                {
                    UINT8 ret = CheckToneDetection( TONE_DETECT_CNG, 30 /*PHONE_TASK_PERIODIC_TIME*/ );

                    if ( ret == TONE_DETECT_CNG )
                    {
                        set_phone_state( PHONE_STATE_OFFHOOK_ANS );
                    }
                    break;
                }

                case LINE_OFFHOOK:
                {
                    T30PhoneLineMonitor();
                    break;
                }

                case LINE_NOLINE:
                default:
                    break;
            }
            break;
        }

        case PHONE_STATE_ABORT:
        {
            fsu_tx_fax_cancel();
            CxPhoneReport( PHONE_ABORT, NULL );
            SetUpToneDetection( TONE_DETECT_OFF, 1 );
            ModemOnHook( 0 );
            Init_RingState( TRUE );
            T30Attribute(GET, FXA_ANSWERMODE, &answer_mode);
            T30Attribute(GET, FXA_AUTOANSWERRINGS, &answer_rings);
            set_fsu_state( FSU_STATE_TX_ABORT );
            set_phone_state( PHONE_STATE_IDLE);
            break;
        }

        default:
            break;
    }
}


static UINT8 phone_convert_number( UINT8 data )
{
    UINT8 num;

    switch ( data )
    {
        case 'P':
        case 'p':
            num = 'P';  // pulse dial
            break;

        case 'T':
        case 't':
            num = 'T';  // DTMF dial
            break;

        case ';':
            num = ';';  // blind dial
            break;

        case ',':
            num = ',';  // pause
            break;

        case '*':
            num = 0x0a;
            break;

        case '#':
            num = 0x0b;
            break;

        case 'A':
        case 'a':
            num = 0x0c;
            break;

        case 'B':
        case 'b':
            num = 0x0d;
            break;

        case 'C':
        case 'c':
            num = 0x0e;
            break;

        case 'D':
        case 'd':
            num = 0x0f;
            break;

        default:

            if ( ( data >= '0' ) && ( data <= '9' ) )
                num = data - '0';
            else
            {
                TRACE_ERROR( "phone number error %c\n", data );
                num = 0xff;
            }
            break;
    }

    return num;
}


void set_phone_number( UINT8 *dial_num, UINT8 mode )
{
    UINT8 i = 0;

    while ( ( *dial_num != 0 ) && ( i < PHONE_DIAL_BYTES ) )
    {
        dial_number[ i++ ] = phone_convert_number( *dial_num++ );

        if ( dial_number[ i - 1 ] == ';' )
        {
            PDP.BlindDial = TRUE;
            i--;
        }
    }

    dial_number[ i ] = 0xff;
    dial_mode        = mode;
    TRACE_PHONE( "set_phone_number %X, mode %d\n", dial_number[0], dial_mode );
}


void set_phone_state( UINT8 new_state )
{
    if ( phone_state != new_state )
    {
        TRACE_PHONE( "set_phone_state(%d->%d)\n", phone_state, new_state );
        if(isDialCancel == TRUE)
        {
            phone_state = PHONE_STATE_INIT;
        }
        else
        {
            phone_state = new_state;
        }

        if ( phone_state == PHONE_STATE_ONHOOK_FOR_RING )
        {
            phone_onhook_for_ring();
        }
    }
}


UINT8 get_phone_state(void)
{
    return(phone_state);
}


void set_phone_status( UINT8 info, UINT8 value )
{
    TRACE_PHONE( "set_phone_status info %d, value %d\n", info, value );

    switch ( info )
    {
        case TONE_DETECT_DIAL:

            dial_tone = value;
            break;

        case TONE_DETECT_BUSY:

            busy_tone = value;
            break;

        case TONE_DETECT_CNG:

            cng_tone = value;
            break;

        case TONE_DETECT_FSK:

            fsk_tone = value;
            break;

        case TONE_DETECT_CI:

            ci_tone = value;
            break;

        case TONE_DETECT_ANS:

            ans_tone = value;
            break;

        case TONE_DETECT_OFF:

            dial_tone = 0;
            busy_tone = 0;
            cng_tone  = 0;
            fsk_tone  = 0;
            ci_tone   = 0;
            ans_tone  = 0;
            break;

        default:
            break;
    }
}


UINT8 get_tone_status( UINT8 info )
{
    UINT8 value = 0;

    switch ( info )
    {
        case TONE_DETECT_DIAL:

            value = dial_tone;
            break;

        case TONE_DETECT_BUSY:

            value = busy_tone;
            break;

        case TONE_DETECT_CNG:

            value = cng_tone;
            break;

        case TONE_DETECT_FSK:

            value = fsk_tone;
            break;

        case TONE_DETECT_CI:

            value = ci_tone;
            break;

        case TONE_DETECT_ANS:

            value = ans_tone;
            break;

        default:
            break;
    }

    TRACE_PHONE( "get_tone_status info %d, value %d)\n", info, value );

    return value;
}


LINE_STATUS get_line_status( void )
{
    return SSFLineStatus();
}


void phone_send_test_signal( MODEM_TX_TEST_INDEX mode )
{
    if ( !sending )
    {
        if ( !offhook )
        {
            set_phone_state( PHONE_STATE_IDLE );
            ModemOffHook( 0 );
            OSC_Sleep_Task( 1000 );
            ModemExclusionRelay( TRUE );
            T30SpeakerControl( T30_SPEKER_MANUAL_ON );
            OSC_Sleep( 1000 );
            offhook = TRUE;
        }

        MODEM_TIMER_START( TimerEvent, 50 );

        sending = TRUE;

        switch ( mode )
        {
            case DTMF_0:

                phone_dial_digit( 0, 2 );
                break;

            case DTMF_1:

                phone_dial_digit( 1, 2 );
                break;

            case DTMF_2:

                phone_dial_digit( 2, 2 );
                break;

            case DTMF_3:

                phone_dial_digit( 3, 2 );
                break;

            case DTMF_4:

                phone_dial_digit( 4, 2 );
                break;

            case DTMF_5:

                phone_dial_digit( 5, 2 );
                break;

            case DTMF_6:

                phone_dial_digit( 6, 2 );
                break;

            case DTMF_7:

                phone_dial_digit( 7, 2 );
                break;

            case DTMF_8:

                phone_dial_digit( 8, 2 );
                break;

            case DTMF_9:

                phone_dial_digit( 9, 2 );
                break;

            case DTMF_STAR:

                phone_dial_digit( 0x0a, 2 );
                break;

            case DTMF_POUND:

                phone_dial_digit( 0x0b, 2 );
                break;

            case DTMF_A:

                phone_dial_digit( 0x0c, 2 );
                break;

            case DTMF_B:

                phone_dial_digit( 0x0d, 2 );
                break;

            case DTMF_C:

                phone_dial_digit( 0x0e, 2 );
                break;

            case DTMF_D:

                phone_dial_digit( 0x0f, 2 );
                break;

            case OFF_HOOK:

                sending = FALSE;
                offhook = TRUE;
                break;

            case CNG_1100HZ:

                ModemTxCNG();
                break;

            case CED_2100HZ:

                ModemTxCED();
                break;

            case ANSAM_2100HZ:

                V8Tx_ANSam();
                break;

            case V8_CI:

                V8Tx_CI();
                break;

            case V21_CH2:

                ModemTxConfigHDLC( T30_SPEED_V21_300 );
                break;

            case V27TER_2400:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V27_2400, TRUE );
                break;

            case V27TER_4800:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V27_4800, TRUE );
                break;

            case V29_7200:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V29_7200, TRUE );
                break;

            case V29_9600:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V29_9600, TRUE );
                break;

            case V17_7200_LONG:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V17_7200, TRUE );
                break;

            case V17_7200_SHORT:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V17_7200, FALSE );
                break;

            case V17_9600_LONG:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V17_9600, TRUE );
                break;

            case V17_9600_SHORT:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V17_9600, FALSE );
                break;

            case V17_12000_LONG:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V17_12000, TRUE );
                break;

            case V17_12000_SHORT:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V17_12000, FALSE );
                break;

            case V17_14400_LONG:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V17_14400, TRUE );
                break;

            case V17_14400_SHORT:

                ModemTxSetPermanentData( TRUE, 0, T30_SPEED_V17_14400, FALSE );
                break;

            case V34_2400:

                ModemTxSetV34SymbolRate( 2400 );
                ModemTxConfigHDLC( T30_SPEED_V34_2400 );
                break;

            case V34_4800:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_4800 );
                break;

            case V34_7200:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_7200 );
                break;

            case V34_9600:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_9600 );
                break;

            case V34_12000:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_12000 );
                break;

            case V34_14400:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_14400 );
                break;

            case V34_16800:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_16800 );
                break;

            case V34_19200:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_19200 );
                break;

            case V34_21600:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_21600 );
                break;

            case V34_24000:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_24000 );
                break;

            case V34_26400:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_26400 );
                break;

            case V34_28800:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_28800 );
                break;

            case V34_31200:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_31200 );
                break;

            case V34_33600:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_33600 );
                break;

            case V34_2400_BAUD:

                ModemTxSetV34SymbolRate( 2400 );
                ModemTxConfigHDLC( T30_SPEED_V34_21600 );
                break;

            case V34_2800_BAUD:

                ModemTxSetV34SymbolRate( 2800 );
                ModemTxConfigHDLC( T30_SPEED_V34_26400 );
                break;

            case V34_3000_BAUD:

                ModemTxSetV34SymbolRate( 3000 );
                ModemTxConfigHDLC( T30_SPEED_V34_28800 );
                break;

            case V34_3200_BAUD:

                ModemTxSetV34SymbolRate( 3200 );
                ModemTxConfigHDLC( T30_SPEED_V34_31200 );
                break;

            case V34_3429_BAUD:

                ModemTxSetV34SymbolRate( 3429 );
                ModemTxConfigHDLC( T30_SPEED_V34_33600 );
                break;

            case MODEM_TX_TEST_STOP:

                sending = FALSE;

                if ( offhook )
                {
                    T30SpeakerControl( T30_SPEKER_MANUAL_OFF );
                    ModemExclusionRelay( FALSE );
                    OSC_Sleep_Task( 1000 );
                    ModemOnHook( 0 );
                    OSC_Sleep( 1000 );
                    set_phone_state( PHONE_STATE_RING_CHECK );
                    offhook = FALSE;
                }
                break;

            default:
                break;
        }
    }
    else
    {
        if ( mode == MODEM_TX_TEST_STOP )
        {
            ModemTxStopTestSignal();
            sending = FALSE;

            if ( offhook )
            {
                T30SpeakerControl( T30_SPEKER_MANUAL_OFF );
                ModemExclusionRelay( FALSE );
                OSC_Sleep_Task( 1000 );
                ModemOnHook( 0 );
                set_phone_state( PHONE_STATE_RING_CHECK );
                offhook = FALSE;
            }
            OSC_Sleep( 1000 );
        }
    }
}


void CxPhoneReport( UINT8 type, UINT8 *buffer )
{
    FAX_EVENT sFaxEvent;

    sFaxEvent.type = ( FAX_EVENT_STATE ) type;
    sFaxEvent.dir  = ( FAX_DIRECTION ) 0;
    sFaxEvent.fcf  = 0;
    sFaxEvent.size = 0;
    sFaxEvent.data = (UINT16 *)buffer;
    fax_events( &sFaxEvent );
}

