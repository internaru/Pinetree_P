/*
 *  $RCSfile: modem.c,v $
 * $Revision: 1.60 $
 *   $Author: miurat $
 *     $Date: 2012/08/21 22:15:57 $
 */
/************************************************************************
 *
 *      modem.c
 *
 *  Modem device driver.
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"
#include "SDHFaxCommonHeader.h"
#include "cxshell.h"
#include <math.h>
#include "sys/time.h"
#include "time.h"
#include "fsu.h"
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "ERROR.H"
#include "MODEMX.H"
#include "PHONEX.H"
#include "T30X.H"
#include "t30.h"
#include "ECMX.H"
#include "ECM.H"
#include "HDLC.H"
#include "modem336.h"
#include "download.inc"
#include "../../faxapp/include/faxapp_notify.h"
#include "spec.h"

#if USE_SDAA
#include "oem_cty.h"
#include "modemsdaa_c.inc"
#endif

#ifdef OEM_FPGA
#define PSP_AUX_IRQ_LEV     0x200
#endif

#define MODEM_RETRY_COUNT 5

static UINT8 no_access_flag = 0;
static UINT8 irq_output_type;
static UINT8 sleep_state;
static UINT32 inactive_time;
static UINT32 sleep_timer;

static UINT8 ctrl_reg0;
static UINT8 ctrl_reg1;
static UINT8 irq_imsr;
static UINT8 irq_ier;
static UINT8 burst_count_reg;
static UINT8 gpio_dir;
static UINT8 gpo_data;
static UINT8 gpo_drv1;
static UINT8 gpo_drv0;
static UINT8 irq_pol;
static UINT8 irq_type;

extern BOOLEAN gFaxNoAnswerFlag;
extern UINT8 gFaxModemResetComplete;
#if __SUPPORT_MANUAL_RECEIVE__
extern UINT8 isRinging;
#else
#endif
extern UINT16 fsu_job_num_rcv;
extern UINT16 gFaxDelayForPauseKey;
extern int isPhoneRingOn;

typedef enum ModemStateType
{
    MODEM_ACTIVE,       // 0, awaked
    MODEM_NORMAL_SLEEP, // 1, normal sleep mode
    MODEM_DEEP_SLEEP,   // 2, deep sleep mode
    MODEM_STOP,         // 3, stop mode
    MODEM_HOOK          // 4, hook control
} MODEM_STATE;


typedef enum ModemIRQType {
    MODEM_IRQ_TYPE_DISABLE = 0,
    MODEM_IRQ_TYPE_OPENDRAIN_LOW = 0x80,
    MODEM_IRQ_TYPE_ACTIVE_LOW = 0x90,
    MODEM_IRQ_TYPE_ACTIVE_HIGH = 0x98,

    MODEM_IRQ_TYPE_LAST
} MODEM_IRQ_TYPE_t;

extern void phone_init_after_wakeup( void );
extern void modem_spi_reset( void );
extern void phone_continue_task( void );
extern void phone_skip_task( void );
extern void modem_start_task( void );
extern void modem_continue_task( void );
extern void modem_skip_task( void );
extern void modem_stop_task( void );
extern void fsu_purge_doc_dir(UINT16 jnum);
extern void fsu_update_status(UINT16 jnum, UINT8 status);

#define MODEM_NO_OFF_MASK       0xE0
#define MODEM_DIAL_TONE_MASK    0x80  /* TONEA for BUSY tone */
#define MODEM_BUSY_TONE_MASK    0x80  /* TONEA for BUSY tone */
#define MODEM_CI_TONE_MASK      0x80  /* TONEA for CI tone */
#define MODEM_CNG_TONE_MASK     0x40  /* TONEB for CNG tone */
#define MODEM_CED_TONE_MASK     0x40  /* TONEB for CED tone */
#define MODEM_FSK_TONE_MASK     0x20  /* TONEC for FSK tone */
#define TONE_A  2
#define TONE_B  1
#define TONE_C  0


T_MODEM_CONTROL *ModemControl;

extern OSC_MUTEX modem_dsp_mutex;

extern char gcCID_for_RemoteFaxNum[FCFSendTableSize + 1];


static void modem_local_param_init( void )
{
	no_access_flag = 0;
	gpio_dir = 0;
	gpo_data = 0;

    if ( (IS_OAK2M == 1) || (IS_OAK2H == 1) )
    {
        irq_output_type = MODEM_IRQ_TYPE_ACTIVE_HIGH;
    }
    else
    {
        irq_output_type = MODEM_IRQ_TYPE_DISABLE;
    }

	sleep_state = MODEM_ACTIVE;
	inactive_time = 0;
	sleep_timer = ModemElapsedTime( 0 );
}


static void modem_delay_usec( int time )
{
    struct timeval tp;
    int lp;
    for( lp = 0; lp < time; lp++ )
    {
        gettimeofday ( &tp, NULL );
    }
}


static void ModemStoreGPO( void )
{
    ctrl_reg0 = MDM_READ( CTRL_REG0 );
    ctrl_reg1 = MDM_READ( CTRL_REG1 );
    irq_imsr  = MDM_READ( IRQ_IMSR );
    irq_ier   = MDM_READ( IRQ_IER );
    gpo_drv1  = MDM_READ( GPO_DRV1 );
    gpo_drv0  = MDM_READ( GPO_DRV0 );
    gpo_data  = MDM_READ( GPO_DATA );
    gpio_dir  = MDM_READ( GPIO_DIR );
    irq_pol   = MDM_READ( IRQ_POL );
    irq_type  = MDM_READ( IRQ_TYPE );
    burst_count_reg = MDM_READ( BURST_COUNT_REG );
}


static void ModemRestoreGPO( void )
{
    MDM_WRITE( CTRL_REG0, ctrl_reg0 );
    MDM_WRITE( CTRL_REG1, ctrl_reg1 );
    MDM_WRITE( IRQ_IMSR, irq_imsr );
    MDM_WRITE( IRQ_IER, irq_ier );
    MDM_WRITE( GPO_DRV1, gpo_drv1 );
    MDM_WRITE( GPO_DRV0, gpo_drv0 );
    MDM_WRITE( GPO_DATA, gpo_data );
    MDM_WRITE( GPIO_DIR, gpio_dir );
    MDM_WRITE( IRQ_POL, irq_pol );
    MDM_WRITE( IRQ_TYPE, irq_type );
    MDM_WRITE( BURST_COUNT_REG, burst_count_reg );
}


UINT16 ModemLineMonitor( BOOL start )
{
    static UINT32 _no_volt_msec;

    if ( start )
    {
        if ( ( no_access_flag == 0 ) && MDM_BITSET( CTRL_REG0, 0x04 ) )
        {
            if ( SSFLineVoltage() > 0 )
            {
                ModemTimerStart( &_no_volt_msec );
            }
            else if ( ModemTimerExpired( _no_volt_msec, 5000 ) )
            {
                TRACE_MODEM( " -- no line --\n" );
                ModemTimerStart( &_no_volt_msec );
                return 0;
            }
        }
    }
    else
    {
        ModemTimerStart( &_no_volt_msec );
    }

    return 1;
}


void ModemConnectedSpeed( UINT8 IndexToSpeed )
{
    UINT16 speed = 0;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    if ( ModemControl->V34Mode )
    {
        if ( MDM_BITSET( MODEM_08, RTS ) )
        {
            speed = V34SpeedTbl[ ModemDSPRead1( MODEM_V34_CURR_TX_SPEED ) - 1 ];
        }
        else
        {
            speed = V34SpeedTbl[ ModemDSPRead1( MODEM_V34_CURR_RX_SPEED ) - 1 ];
        }
    }
    else
    {
        if (T30Control == (T_T30_CONTROL *)NULL)
        {
            speed = V17SpeedTbl[ IndexToSpeed ];
        }
        else
        {
            speed = V17SpeedTbl[ T30Control->ModemSpeed ];
        }
    }

    OSC_Release_Mutex( &modem_dsp_mutex );

    TRACE_MODEM( " ( Connect %d bps )\n", speed );

    ModemControl->ConnectedSpeed = speed;
}


/*-------------------------------------------------------------------
    Name: ModemDetectPresence -  Detect pesence of modem hardware

    Input:  None
    Output: TRUE - Active, FALSE - Sleep
    Return: none

 *-------------------------------------------------------------------*/
BOOL ModemDetectPresence( void )
{
    if ( ModemControl->State == MODEM_SLEEP )
        return FALSE;
    else
        return TRUE;
}


void ModemSetIRQOutType( UINT8 type )
{
    irq_output_type &= ~type;
    irq_output_type |=  type;
}


BOOL ModemSleepStateChange( UINT8 sleep )
{
    UINT8 data;

    if ( ( ModemControl->State == MODEM_IDLE )
      && ( sleep_state == MODEM_ACTIVE )
      && ( sleep != MODEM_HOOK ) )
    {
        if ( ( SSFLineStatus() == LINE_INUSE )
          || ( SSFLineStatus() == LINE_OFFHOOK )
          || MDM_BITSET( MODEM_0F, RI ) )
            return FALSE;

        if ( sleep != MODEM_ACTIVE )
        {
            SSFDownload( DL_SLEEP );
            TRACE_MODEM( " downloaded %d\n", DL_SLEEP );
        }
    }

    switch ( sleep )
    {
        case MODEM_ACTIVE:

            if ( ( ModemControl->State != MODEM_IDLE )
              && ( sleep_state != MODEM_ACTIVE ) )
            {
                ModemReset( 0 );
                sleep_state = MODEM_ACTIVE;
                ModemControl->State = MODEM_IDLE;
                phone_init_after_wakeup();
            }

            return TRUE;

        case MODEM_NORMAL_SLEEP:

            if ( ( ModemControl->State == MODEM_IDLE )
              && ( sleep_state == MODEM_ACTIVE ) )
            {
                if ( irq_output_type != 0 )
                {
                    data = MDM_READ( CTRL_REG0 );
                    data &= ~0x18;
                    data |= ( irq_output_type & 0x18 );
                    MDM_WRITE( CTRL_REG0, data );
                    MDM_SETBIT( IRQ_IER, ENABLE_SSDIRQ );

                    if ( irq_output_type & 0x80 )
                    {
                        MDM_SETBIT( IRQ_IER, ENABLE_SSDIRQ );
                        MDM_SETBIT( MODEM_1F, NCIE );
                    }
                }

                MDM_SETBIT( MODEM_15, MSLEEP + RDWK + HWRWK );
                MDM_SETBIT( MODEM_1F, NEWC );

                OSC_Sleep( 1 );

                if ( MDM_BITSET( MODEM_1F, NEWC ) )
                {
                    sleep_state = MODEM_NORMAL_SLEEP;
                    ModemControl->State = MODEM_SLEEP;
                    return TRUE;
                }
                else
                {
                    ModemReset( 0 );
                    sleep_state = MODEM_ACTIVE;
                    ModemControl->State = MODEM_IDLE;
                    phone_init_after_wakeup();
                    return FALSE;
                }
            }
            else
                return FALSE;

        case MODEM_DEEP_SLEEP:

            if ( ( ModemControl->State == MODEM_IDLE )
              && ( sleep_state == MODEM_ACTIVE ) )
            {
                if ( irq_output_type != 0 )
                {
                    data = MDM_READ( CTRL_REG0 );
                    data &= ~0x18;
                    data |= ( irq_output_type & 0x18 );
                    MDM_WRITE( CTRL_REG0, data );
                    MDM_SETBIT( IRQ_IER, ENABLE_SSDIRQ );

                    if ( irq_output_type & 0x80 )
                    {
                        MDM_SETBIT( IRQ_IER, ENABLE_SSDIRQ );
                        MDM_SETBIT( MODEM_1F, NCIE );
                    }
                }

                MDM_SETBIT( MODEM_15, MSLEEP + RDWK );
                MDM_SETBIT( MODEM_1F, NEWC );

                OSC_Sleep( 1 );

                if ( MDM_BITSET( MODEM_1F, NEWC ) )
                {
                    sleep_state = MODEM_DEEP_SLEEP;
                    ModemControl->State = MODEM_SLEEP;
                    return TRUE;
                }
                else
                {
                    ModemReset( 0 );
                    sleep_state = MODEM_ACTIVE;
                    ModemControl->State = MODEM_IDLE;
                    phone_init_after_wakeup();
                    return FALSE;
                }
            }
            else
                return FALSE;

        case MODEM_STOP:

            if ( ( ModemControl->State == MODEM_IDLE )
              && ( sleep_state == MODEM_ACTIVE ) )
            {
                MDM_SETBIT( MODEM_15, MSLEEP + MSTOP );
                MDM_SETBIT( MODEM_1F, NEWC );

                OSC_Sleep( 1 );

                if ( MDM_BITSET( MODEM_1F, NEWC ) )
                {
                    sleep_state = MODEM_STOP;
                    ModemControl->State = MODEM_SLEEP;
                    return TRUE;
                }
                else
                {
                    ModemReset( 0 );
                    sleep_state = MODEM_ACTIVE;
                    ModemControl->State = MODEM_IDLE;
                    phone_init_after_wakeup();
                    return FALSE;
                }
            }
            else
                return FALSE;

        case MODEM_HOOK:

            ModemReset( 1 );
            sleep_state = MODEM_ACTIVE;
            ModemControl->State = MODEM_IDLE;
            return TRUE;

        default:
            break;
    }
    return FALSE;
}


void ModemWakeup( void )
{
    BOOL ret = FALSE;

    ret = ModemSleepStateChange( MODEM_ACTIVE );

    printf("\n(YHCHO) %s() - ret: %d\n", __FUNCTION__, ret);
}


void ModemSleep(UINT8 Mode)
{
    BOOL ret = FALSE;

    printf("\n(YHCHO) %s() - Mode: %d\n", __FUNCTION__, Mode);

    if (Mode == MODEM_NORMAL_SLEEP)
    {
        ret = ModemSleepStateChange( MODEM_NORMAL_SLEEP );
    }
    else
    {
        ret = ModemSleepStateChange( MODEM_DEEP_SLEEP );
    }

    printf("\n(YHCHO) %s() - ret: %d\n", __FUNCTION__, ret);
}


void ModemSleepTimerSetup( UINT32 time )
{
    inactive_time = time;
}


void ModemSleepTimerRestart( void )
{
    sleep_timer = ModemElapsedTime( 0 );
}


/*=======================================================================
*    ModemReset
*      Does a hardware reset on the modem.  Uses a software timer (YUK),
*      since the multitasker may not be running.
*
*      Input:          1: force, 0: normal
*      Output:         (none)
*=======================================================================*/
void ModemReset( UINT8 force )
{
    UINT8 count, limit, data;
    INT8  dl_type;
    BOOL  result;

//	TRACE_MODEMDEBUG( "ModemReset:  Start --->\n" );

    count = MODEM_RETRY_COUNT;
    no_access_flag = 1;

    while ( count )
    {
        if ( ( force != 0 )
          || ( sleep_state == MODEM_ACTIVE )
          || MDM_BITSET( MODEM_1F, NEWC ) )
        {
            modem_spi_reset();
        }
        else if ( irq_output_type != 0 )
        {
            data = MDM_READ( CTRL_REG0 );
            data &= ~0x18;
            MDM_WRITE( CTRL_REG0, data );
            MDM_CLEARBIT( IRQ_IER, ENABLE_SSDIRQ );
        }

        ModemRestoreGPO();

        limit = 20;

        while ( ( MDM_READ( MODEM_CONF ) & 0xF0 ) != 0xA0 )
        {
            OSC_Sleep( 10 );

            if ( --limit == 0 )
            {
                TRACE_ERROR( " ### Conf %02X ###", MDM_READ( MODEM_CONF ) );
                return ;
            }
        }

        limit = 20;

        while ( MDM_BITSET( MODEM_0C, SDBS ) )
        {
            OSC_Sleep( 10 );

            if ( --limit == 0 )
            {
                TRACE_ERROR( " ### SDSB %02X ###", MDM_READ( MODEM_CONF ) );
                return ;
            }
        }

        MDM_SETBIT( MODEM_1F, NEWC );

        limit = 20;

        do {
            OSC_Sleep( 10 );

            if ( --limit == 0 )
                break;
        } while ( MDM_BITSET( MODEM_1F, NEWC ) );

        if ( limit != 0 )
        {
            if ( ( force == 0 ) && ( sleep_state != MODEM_ACTIVE ) )
                dl_type = DL_SPX;
            else
                dl_type = DL_SPX + DL_CPX;

            result = SSFDownload( dl_type );

            if ( result )
            {
                ModemRestoreGPO();

                MDM_SETBIT( MODEM_1F, NEWC );

                limit = 20;

                do {
                    OSC_Sleep( 10 );

                    if ( --limit == 0 )
                        break;
                } while ( MDM_BITSET( MODEM_1F, NEWC ) );

                if ( limit != 0 )
                {
                    TRACE_MODEM( " downloaded\n" );
                    break;
                }
                else
                {
                    TRACE_MODEM( " retry NEWC after download\n" );
                }
            }
            else
            {
                TRACE_MODEM( " retry download\n" );
            }
        }
        else
        {
            TRACE_MODEM( " retry NEWC before download\n" );
        }
        count--;
    }

    if ( count == 0 )
    {
        TRACE_ERROR( "ModemReset: Failed End --->#####\n" );
    }
    else
    {
        ModemConfigGPIO( EX_RELAY_PORT, EX_RELAY_MASK ); // Configure I/O port for Exclusion Relay

#ifdef CX_DEBUG
        if ( _CxDebugLevel == 0xff )
        {
            MDM_WRITE( CTRL_REG0, 0x04 );
            //TRACE_ALWAYS( " XCLK,YCLK enable\n" );
        }
#endif

//		TRACE_MODEMDEBUG( "ModemReset: %d retry, End --->\n", MODEM_RETRY_COUNT - count );

        no_access_flag = 0;

        gFaxModemResetComplete = 1;
    }

    SSFInitMonitorParams( sleep_state == MODEM_ACTIVE );
    ModemSleepTimerRestart();
}


/*===========================================================================
*
*    ModemInitJob
*
*    Initializes the fax modem. Also allocates Modem control structure
*    to be used by the T.30 and Phone applications
*
*   Input:  none
*   Output: Modem interrupts disabled
*           Parallel data modem enabled
*           T/2 equalizer enabled
*           Diagnostics 1 enabled at sample rate
*           Modem control structures allocated
*           returns 1 if OK, 0 if allocation failed
*
*===========================================================================*/
UINT8 ModemInitJob( void )
{
    UINT8 count;

    if ( OSC_Allocate_Zero_Memory( (OSC_MEMORY_POOL *)NULL, (void **)&ModemControl, sizeof(T_MODEM_CONTROL), TRUE ) != OSC_SUCCESS )
    {
        TRACE_ERROR( "ModemInitJob() failed.\n" );
        return ( 0 );
    }

    modem_local_param_init();

    memset( &SDAA_Data_Table, 0, sizeof( SDAADATA ) );

    /* try several times and declare error if all fails */
    for ( count = 0; count < MODEM_RETRY_COUNT; count++ )
    {
        sleep_state = MODEM_ACTIVE;
        ModemStoreGPO();
        ModemReset( 1 );   // Reset modem, Clear left over interrupts, etc

        if ( MDM_READ( MODEM_CONF ) == MODEM_V8_CONF )
        {
            ModemShowRAMVersion();
            break;
        }

        OSC_Sleep( 100 );
    }

    /* If modem not present, exit */
    if ( count >= MODEM_RETRY_COUNT )
    {
        TRACE_ERROR( "Modem not present ??\n?\n?\n?\n?\n?\n?\n?\n?\n?\n?\n?\n?\n?\n" );
        return ( 0 );
    }

    ModemClearIRQ();

    TRACE_MODEM( "ModemInitJob() done.\n" );

    activate_modem_job();

#ifdef USE_EXCLUSION_RELAY
    ModemConfigGPIO( EX_RELAY_PORT, EX_RELAY_MASK ); // Configure I/O port for Exclusion Relay
#endif

    ModemRxConfigRing();

    return ( 1 );
}


void ModemReinitJob( void )
{
    ModemClearIRQ();
}


void ModemInitVars( void )
{
    if ( ModemControl )
    {
        ModemControl->State  = MODEM_IDLE;
        ModemControl->Status = NO_ERROR;

        ModemControl->V8Answer  = FALSE;
        ModemControl->V8Polling = FALSE;
        ModemControl->V34Mode   = FALSE;
        ModemControl->HighSpeed = FALSE;
        ModemControl->FoundFlag = FALSE;
        ModemControl->SendCRP   = FALSE;
        ModemControl->FoundRTC  = FALSE;

        ModemControl->EOLType = CODING_MH;
        ModemControl->EOLCount  = 0;
        ModemControl->ZeroCount = 0;

        ModemControl->EQM   = 0;
        ModemControl->Level = 0;

        ModemControl->TCFErrorBytes = 0;
        ModemControl->TCFOKBytes    = 0;

        ModemControl->TxBlockCnt = 0;
        ModemControl->TxFrameCnt = 0;

        ModemControl->BytesPerFrame  = 0;
        ModemControl->IsrDataCounter = 0;

        ModemControl->CDBByteNum = 0;
        ModemControl->BufferQueueProcess = 0;
    }
}


/*=============================================================================
*
*    ModemClearIRQ
*
*
*   Clear all SPX interrupts (D1DIE,D2DIE,PIE) enable bits and
*   interrupt flags. Also clear all HDLC flags, but set FLAG bit.
*   This would allow the SPX to "resync/reset" internal modes for
*   the next next time when interrupt is enabled.
*
*   Input:      (none)
*
*   Output:     Turns off interrupt enables:
*             programmable interrupts
*
*           Resets flags:
*             OE, PE, FE, SYNCD
*             New status interrupt request
*
*=============================================================================*/
void ModemClearIRQ( void )
{
    /* Turn off all interrupt enable bits */
    MDM_CLEARBIT( MODEM_1F, NSIE );
    MDM_CLEARBIT( MODEM_1F, NCIE );
    MDM_CLEARBIT( MODEM_1E, TDBIE );
    MDM_CLEARBIT( MODEM_1E, RDBIE );

    /* Reset the HDLC status flags */
    MDM_CLEARBIT( MODEM_0A, ( UINT8 ) ~MODEM_HDLC_STATUS_MASK );

    /* Clear new status interrupt request flag */
    MDM_CLEARBIT( MODEM_1F, NSIA );
    modem_stop_task();
}


/*===========================================================================
*
*    ModemCompareAGCGain
*
*   Compares the modem AGC gain word to the maximum gain word, to
*   check signal levels.
*
*   Input:      (none)
*
*   Output:     Returns with 1 if the AGC gain word is less
*             than the max. gain word (signal above threshold)
*             0 if not.
*
*============================================================================*/
UINT8 ModemCompareAGCGain( void )
{
    if ( !ModemDetectPresence() )
        return FALSE;

    return TRUE;
}


/*===========================================================================
*
*   ModemConfig
*
*   Configures the modem
*
*   Input:      modem configuration code
*   Output:     Stores configuration in config. reg, turns on setup bit,
*             and waits for it to go away.
*
*============================================================================*/
void ModemConfig( UINT8 config )
{
    MDM_WRITE( MODEM_CONF, config );

    /* Wait until the new configuration gets loaded into the modem */
    MDM_SETBIT( MODEM_1F, NEWC );
    ModemBitOrBust( MODEM_1F, NEWC, 0 );

    TRACE_MODEM("Modem configured [%02X]\n", config );
}

/*============================================================================
*
*    ModemRxConfig
*
*   Configures modem receiver
*
*   Input:  X = modem speed table index
*
*   Index      Speed
*     0 V.21,   300
*     1 V.27,  2400
*     2 V.27,  4800
*     3 V.29,  7200
*     4 V.29,  9600
*     5 V.17,  7200
*     6 V.17,  9600
*     7 V.17, 12000
*     8 V.17, 14400
*     9 V.34,  2400
*    10 V.34,  4800
*    11 V.34,  7200
*    12 V.34,  9600
*    13 V.34, 12000
*    14 V.34, 14400
*    15 V.34, 16800
*    16 V.34, 19200
*    17 V.34, 21600
*    18 V.34, 24000
*    19 V.34, 26400
*    20 V.34, 28800
*    21 V.34, 31200
*    22 V.34, 33600
*
*   Output:     ModemRTSP = 0
*
*=============================================================================*/
void ModemRxConfig( UINT8 speed, UINT8 hdlc )
{
    UINT8 temp;

    TRACE_MODEM( "ModemRxConfig ( %s )\n", hdlc ? "HDLC" : "NORM" );

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    MDM_CLEARBIT( MODEM_09, DTMF + ORG + DTR );
    MDM_CLEARBIT( MODEM_08, ( ASYN + RTS ) );
    MDM_CLEARBIT( MODEM_06, RTDIS + CCRTN + HDLC );
    MDM_CLEARBIT( MODEM_05, FCCR + CEQS + VINIT + TXSQ + CEQ + TTDIS + STOFF );
    ModemBitOrBust( MODEM_0F, CTS, 0 );

    MDM_SETBIT( MODEM_09, DATA );
    MDM_SETBIT( MODEM_04, FIFOEN );

    /* Enable HDLC mode */
    if ( hdlc )
    {
        MDM_SETBIT( MODEM_06, HDLC );
        /* Reset the status flags */
        MDM_CLEARBIT( MODEM_0A, ( UINT8 ) ~MODEM_HDLC_STATUS_MASK );
        ModemControl->HDLCMode = TRUE;
    }
    else
    {
        ModemControl->HDLCMode = FALSE;
    }

    if ( speed == T30_SPEED_V21_300 )
    {
        /* Disable Extended FIFO */
        temp = ModemDSPRead1( MODEM_RX_EXTEND_FIFO_DIS );
        ModemDSPWrite1( MODEM_RX_EXTEND_FIFO_DIS, temp | 0x03, FALSE );
    }
    else
    {
        /* Enable Extended FIFO */
        temp = ModemDSPRead1( MODEM_RX_EXTEND_FIFO_DIS );
        ModemDSPWrite1( MODEM_RX_EXTEND_FIFO_DIS, temp & 0xfc, FALSE );
    }

    /* Trigger level set 1 byte */
    temp = ModemDSPRead1( MODEM_RX_FIFO );
    ModemDSPWrite1( MODEM_RX_FIFO, temp & 0x21, FALSE );

    /* Reset RX max gain */
    ModemSetMaxGain( ModemRxMaxGainTbl[ ModemControl->Options.RxMaxGain ] );

    /* Configure modem */
    ModemConfig( ModemConfigTbl[ speed ] );

    while ( MDM_BITSET( MODEM_0C, RXFNE ) )
        MDM_READ( MODEM_RBUFF );

    if ( speed != T30_SPEED_V21_300 )
        ModemConnectedSpeed(speed);

    OSC_Release_Mutex( &modem_dsp_mutex );
}

/* ===========================================================================
 *  ModemStopV34
 *
 *  Configure modem to stop sending signal
 *
 * =========================================================================== */
void ModemStopV34( void )
{
    MDM_CLEARBIT( MODEM_09, DTR );
    MDM_CLEARBIT( MODEM_08, RTS );
}


/*============================================================================
*
*    ModemRxConfigHDLC
*
*   Configures for HDLC reception, with RTS off.
*
*   Input:      speed = desired data mode (index into modem speed table)
*
*   Index      Speed
*     0 V.21,   300
*     1 V.27,  2400
*     2 V.27,  4800
*     3 V.29,  7200
*     4 V.29,  9600
*     5 V.17,  7200
*     6 V.17,  9600
*     7 V.17, 12000
*     8 V.17, 14400
*     9 V.34,  2400
*    10 V.34,  4800
*    11 V.34,  7200
*    12 V.34,  9600
*    13 V.34, 12000
*    14 V.34, 14400
*    15 V.34, 16800
*    16 V.34, 19200
*    17 V.34, 21600
*    18 V.34, 24000
*    19 V.34, 26400
*    20 V.34, 28800
*    21 V.34, 31200
*    22 V.34, 33600
*
*   Output:     Configures modem to desired speed and enables HDLC mode
*
*============================================================================*/
void ModemRxConfigHDLC( UINT8 speed )
{
    /* Configure the mode receiver */
    ModemRxConfig( speed, TRUE );
}

/*============================================================================
*
*    ModemRxConfigHiSpeed
*
*   Configures modem as high speed receiver, getting ready to receive either
*   1.5 seconds of 0's during TCF, or T.4 compressed image data.
*
*   Input:      speed = desired data mode (index into modem speed table)
*
*   Index      Speed
*     0 V.21,   300
*     1 V.27,  2400
*     2 V.27,  4800
*     3 V.29,  7200
*     4 V.29,  9600
*     5 V.17,  7200
*     6 V.17,  9600
*     7 V.17, 12000
*     8 V.17, 14400
*     9 V.34,  2400
*    10 V.34,  4800
*    11 V.34,  7200
*    12 V.34,  9600
*    13 V.34, 12000
*    14 V.34, 14400
*    15 V.34, 16800
*    16 V.34, 19200
*    17 V.34, 21600
*    18 V.34, 24000
*    19 V.34, 26400
*    20 V.34, 28800
*    21 V.34, 31200
*    22 V.34, 33600
*
*   Output:     Configures modem to desired speed and disables HDLC mode
*
*============================================================================*/
void ModemRxConfigHiSpeed( UINT8 speed )
{
    /* Configure the speed */
    ModemRxConfig( speed, FALSE );
}


void ModemTxSetV34SymbolRate( UINT16 rate )
{
    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    switch ( rate )
    {
        case 2400:

            ModemDSPWrite1( MODEM_V34_BMR, MODEM_V34_BMR_2400, FALSE );
            break;

        case 2800:
            ModemDSPWrite1( MODEM_V34_BMR, MODEM_V34_BMR_2800, FALSE );
            break;

        case 3000:
            ModemDSPWrite1( MODEM_V34_BMR, MODEM_V34_BMR_3000, FALSE );
            break;

        case 3200:
            ModemDSPWrite1( MODEM_V34_BMR, MODEM_V34_BMR_3200, FALSE );
            break;

        case 3429:
            ModemDSPWrite1( MODEM_V34_BMR, MODEM_V34_BMR_3429, FALSE );
            break;

        default:
            ModemDSPWrite1( MODEM_V34_BMR, 0x3F, FALSE );
            break;
    }

    OSC_Release_Mutex( &modem_dsp_mutex );
}


/*============================================================================
*
*    ModemTxSetLevel
*
*   Sets modem OUTPUT LEVEL.
*
*   Input:      TX attenuation in dB
*
*            -4.0 through -15.0 dBm: 0 to 40 = -4 dBm, 150 = -15.0 dBm
*
*   Output:     Modem OUTPUT LEVEL set
*
*============================================================================*/
void ModemTxSetLevel( UINT16 txlevel )
{
    float  fValue;
    UINT16 wValue = 0;
    UINT8  cConf;

    TRACE_MODEM( "TransmitLevel -%d.%ddBm\n", txlevel / 10, txlevel % 10 );

    cConf = MDM_READ( MODEM_CONF );

    if ( cConf == 0xAA )
    {
        cConf = V8_MOD;
    }
    else if ( ( cConf & 0xF0 ) == 0xA0 )
    {
        cConf = V21_MOD;
    }
    else if ( ( cConf & 0xF0 ) == 0xC0 )
    {
        cConf = V34_MOD;
    }
    else if ( ( cConf & 0xF0 ) == 0xB0 )
    {
        cConf = V17_MOD;
    }
    else if ( ( cConf & 0xF0 ) == 0x10 )
    {
        cConf = V29_MOD;
    }
    else if ( ( cConf & 0xF0 ) == 0x00 )
    {
        cConf = V27_MOD;
    }
    else if (  cConf == 0x81 )
    {
        cConf = TONE1100;
    }
    else
    {
        cConf = TONE2100;
    }

//	TRACE_MODEMDEBUG( " modulation %d\n", cConf );

    MDM_WRITE( MODEM_TLVL, 0x90 );

    if ( txlevel >= 10 )
        wValue = txlevel - 10;

    if ( ModemControl->Options.TxCompensation )
    {
        wValue += ( UINT16 ) TxCompLevel[ cConf ].Complex;

//		TRACE_MODEMDEBUG( " complex %d\n", TxCompLevel[ cConf ].Complex );
    }
    else
    {
        wValue += ( UINT16 ) TxCompLevel[ cConf ].Normal;

//		TRACE_MODEMDEBUG( " normal %d\n", TxCompLevel[ cConf ].Normal );
    }

//	TRACE_MODEMDEBUG( " power %d -> %d\n", txlevel, wValue );

    if ( wValue > 90 )
    {
        wValue -= 90;

        fValue = -( ( float ) wValue / 200 );
//		TRACE_MODEMDEBUG(" fValue %f\n", fValue);

        fValue = powf( ( float ) 10, fValue );
//		TRACE_MODEMDEBUG(" fValue %f\n", fValue);

        wValue = ( UINT16 ) ( fValue * 16384 );

    }
    else if ( wValue < 90 )
    {
        wValue = 90 - wValue;

        fValue = ( ( float ) wValue / 200 );
//		TRACE_MODEMDEBUG(" fValue %f\n", fValue);

        fValue = powf( ( float ) 10, fValue );
//		TRACE_MODEMDEBUG(" fValue %f\n", fValue);

        wValue = ( UINT16 ) ( fValue * 16384 );
    }
    else
    {
        wValue = 0x4000;
    }

    TRACE_MODEM(" wValue 0x%04X\n", wValue);
    ModemDSPWrite2( MODEM_TX_OUTPUT_GAIN_ALL_H, MODEM_TX_OUTPUT_GAIN_ALL_L, wValue );
}


/*============================================================================
*
*    ModemTxConfig
*
*   Configures the modem transmitter.  Delays 60 msec to satisfy
*   CCITT delay between transmissions requirement.  Requirement is 75 +/- 20 msec.
*   Modem adds 20 msec to the start of V.29, and to the?End of V.29 and V.27.
*
*   Input:  ForceEPTOn = 1 to force EPT on
*           ForceEPTOff = 1 to force EPT off
*           Has no affect on V.21.  ForceEPTOn takes precedence if both are on.
*           speed = modem speed table index
*
*   Index      Speed
*     0 V.21,   300
*     1 V.27,  2400
*     2 V.27,  4800
*     3 V.29,  7200
*     4 V.29,  9600
*     5 V.17,  7200
*     6 V.17,  9600
*     7 V.17, 12000
*     8 V.17, 14400
*     9 V.34,  2400
*    10 V.34,  4800
*    11 V.34,  7200
*    12 V.34,  9600
*    13 V.34, 12000
*    14 V.34, 14400
*    15 V.34, 16800
*    16 V.34, 19200
*    17 V.34, 21600
*    18 V.34, 24000
*    19 V.34, 26400
*    20 V.34, 28800
*    21 V.34, 31200
*    22 V.34, 33600
*
*   Output:     EPT set according to the configuration code
*               RTS = 1
*
*=============================================================================*/
void ModemTxConfig( UINT8 speed, UINT8 hdlc )
{
    UINT8  temp;
    int ie;

    TRACE_MODEM( "ModemTxConfig ( %s )\n", hdlc ? "HDLC" : "NORM" );

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    MDM_CLEARBIT( MODEM_09, DTMF + ORG + DTR );
    MDM_CLEARBIT( MODEM_08, ( ASYN + V21S + RTRN + RTS ) );
    MDM_CLEARBIT( MODEM_06, RTDIS + CCRTN + HDLC );
    MDM_CLEARBIT( MODEM_05, FCCR + CEQS + VINIT + TXSQ + CEQ + TTDIS + STOFF );
    MDM_CLEARBIT( MODEM_03, EPT + SEPT );

    /* for V.34 transmit signal test */
    if ( speed >= T30_SPEED_V34_2400 )
    {
        MDM_SETBIT( MODEM_07, L3ACT );
        MDM_SETBIT( MODEM_15, EXL3 );
        MDM_SETBIT( MODEM_09, ORG );
        MDM_CLEARBIT( MODEM_15, EARC );
    }
    else
    {
        MDM_CLEARBIT( MODEM_07, L3ACT );
        MDM_CLEARBIT( MODEM_15, EXL3 );

        /* V.21 or V.29 or Force means we turn off echo protect. */
        if ( ( speed == T30_SPEED_V21_300 )
          || ( ( ( speed == T30_SPEED_V29_7200 )
              || ( speed == T30_SPEED_V29_9600 ) )
            && !ModemControl->Options.ForceEPTOn )
          || ModemControl->Options.ForceEPTOff )
        {
            MDM_CLEARBIT( MODEM_03, EPT );
        }
        else  /* Otherwise, turn echo protect on. */
        {
            MDM_SETBIT( MODEM_03, EPT );
        }
    }

    /* Setup for parallel data mode and disallow start-up sequence */
    /* Also enable transmitter FIFO */
    MDM_SETBIT( MODEM_08, TPDM );
    MDM_SETBIT( MODEM_09, DATA );
    MDM_SETBIT( MODEM_04, FIFOEN );

    if ( hdlc )
    {
        MDM_SETBIT( MODEM_06, HDLC );
        ModemDSPWrite1( MODEM_TX_ABORT_UNDERFLOW, 0x40, TRUE );  // Transmitter abort on underrun
        ModemControl->HDLCMode = TRUE;
    }
    else
    {
        ModemControl->HDLCMode = FALSE;
    }

    if ( speed == T30_SPEED_V21_300 )
    {
        /* Disable Extended FIFO */
        temp = ModemDSPRead1( MODEM_TX_EXTEND_FIFO_DIS );
        ModemDSPWrite1( MODEM_RX_EXTEND_FIFO_DIS, temp | 0x03, FALSE );
    }
    else
    {
        /* Enable Extended FIFO */
        temp = ModemDSPRead1( MODEM_TX_EXTEND_FIFO_DIS );
        ModemDSPWrite1( MODEM_RX_EXTEND_FIFO_DIS, temp & 0xFC, FALSE );
    }

    ModemTxSetLevel( ModemControl->Options.FaxTxLevel );

    TRACE_MODEM( "ModemTxConfig [ %02X ], speed %d\n", ModemConfigTbl[ speed ], speed );

    /* Configure the mode */
    ModemConfig( ModemConfigTbl[ speed ] );

    /* Reset the HDLC status flags, in case we're in HDLC mode */
    MDM_CLEARBIT( MODEM_0A, ( UINT8 ) ~MODEM_HDLC_STATUS_MASK );
    MDM_WRITE( MODEM_ABCODE, 0 );

    ie = 500 / MODEM_DELAY_TIME;  // 500 msec failsafe

    /* Wait until signal gap ready */
    while ( !MODEM_TIMER_EXPIRE( TimerEvent ) )
    {
        if ( --ie == 0)
            break;

        OSC_Sleep( MODEM_DELAY_TIME );
    }

    TRACE_MODEM( " ** ModemTxConfig Exit from TimerEvent %d\n", ie );

    /* for V.34 transmit signal test */
    if ( speed >= T30_SPEED_V34_2400 )
    {
        /* Turn on DTR */
        MDM_SETBIT( MODEM_09, DTR );
    }
    else
    {
        /* Turn on RTS */
        MDM_SETBIT( MODEM_08, RTS );
    }

    if ( !hdlc )
    {
        ModemDSPWrite1( MODEM_TX_FIFO_PRELOAD, 0x04, TRUE );

        while ( MDM_BITSET( MODEM_0D, TXFNF ) )
        {
            MDM_WRITE( MODEM_TBUFF, 0xFF );
        }
    }

    OSC_Release_Mutex( &modem_dsp_mutex );

    if ( speed != T30_SPEED_V21_300 )
        ModemConnectedSpeed(speed);
}


/*============================================================================
*
*    ModemTxConfigHDLC
*
*   Configures for HDLC transmission.
*
*   Input:      speed = desired data mode (index into modem speed table)
*
*   Index      Speed
*     0 V.21,   300
*     1 V.27,  2400
*     2 V.27,  4800
*     3 V.29,  7200
*     4 V.29,  9600
*     5 V.17,  7200
*     6 V.17,  9600
*     7 V.17, 12000
*     8 V.17, 14400
*     9 V.34,  2400
*    10 V.34,  4800
*    11 V.34,  7200
*    12 V.34,  9600
*    13 V.34, 12000
*    14 V.34, 14400
*    15 V.34, 16800
*    16 V.34, 19200
*    17 V.34, 21600
*    18 V.34, 24000
*    19 V.34, 26400
*    20 V.34, 28800
*    21 V.34, 31200
*    22 V.34, 33600
*
*   Output:     (none)
*
*============================================================================*/
void ModemTxConfigHDLC( UINT8 speed )
{
    /* Set up for HDLC mode, configure modem, and set flag count */
    ModemTxConfig( speed, TRUE );
}


/*============================================================================
*
*    ConfigTxHiSpeed
*
*   Configures modem as high speed transmitter
*
*   Input:      speed = desired data mode (index into modem speed table)
*
*   Index      Speed
*     0 V.21,   300
*     1 V.27,  2400
*     2 V.27,  4800
*     3 V.29,  7200
*     4 V.29,  9600
*     5 V.17,  7200
*     6 V.17,  9600
*     7 V.17, 12000
*     8 V.17, 14400
*     9 V.34,  2400
*    10 V.34,  4800
*    11 V.34,  7200
*    12 V.34,  9600
*    13 V.34, 12000
*    14 V.34, 14400
*    15 V.34, 16800
*    16 V.34, 19200
*    17 V.34, 21600
*    18 V.34, 24000
*    19 V.34, 26400
*    20 V.34, 28800
*    21 V.34, 31200
*    22 V.34, 33600
*
*   Output:     ModemHDLC = 0
*               EPT set according to the configuration code
*               RTS = 1
*
*============================================================================*/
void ModemTxConfigHiSpeed( UINT8 speed )
{
    ModemTxConfig( speed, FALSE );
}


/*============================================================================
*
*    ModemTxConfigVoice
*
*   Configures the modem for voice transmission, and
*   enables data interrupts.
*
*   Input:       1 for error correction, 0 for none
*                voice playback speed
*                TX volume (0 = max, 0x1f = min)
*   Output:     ModemControl->State = VOICE_TX_MODE
*               ModemControl->Status = NO_ERROR
*
*============================================================================*/
void ModemTxConfigVoice( UINT8 error_correction, UINT8 speed, UINT8 volume )
{}


/*============================================================================
*
*    ModemInitToneDet
*
*   Initializes tone detector
*
*   Input:  detector_number
*              2 = TONE_A
*              1 = TONE_B
*              0 = TONE_C
*           CoeffTable = address of coefficient table
*   Output:     Tone detector coefficients initialized
*
*============================================================================*/
void ModemInitToneDet( UINT8 detector_number, const UINT16 *CoeffTable )
{
    UINT8 CoeffNum;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    /* Enable tone detectors */
    MDM_SETBIT( MODEM_02, TDE );

    /* Write the tone coefficients to DSP */
    for ( CoeffNum = 0; CoeffNum < 14; CoeffNum++ )
        ModemDSPWrite3( FreqAccTable[ detector_number ][ CoeffNum ], CoeffTable[ CoeffNum ] );

    if ( detector_number == TONE_C )
    {
        MDM_SETBIT( MODEM_02, SQDIS );

        for ( CoeffNum = 0; CoeffNum < 10; CoeffNum++ )
            ModemDSPWrite3( FreqAccTable[ 3 ][ CoeffNum ], CoeffTable[ CoeffNum ] );
    }

    OSC_Release_Mutex( &modem_dsp_mutex );
}


/*============================================================================
*
*    ModemReadAGCGain
*
*   Reads the modem AGC gain word and compares
*
*   Input:      (none)
*   Output:     gain word
*
*=============================================================================*/
UINT16 ModemReadAGCGain( void )
{
    UINT16 retval;

    /* AGC gain word access code */
    retval = ModemDSPRead4( MODEM_AGC_GAIN );

    return ( retval );
}


/*============================================================================
*
*    ModemReadTxLevel
*
*   Reads the modem receive level
*
*   Input:      (none)
*   Output:     level
*
*=============================================================================*/
INT8 ModemReadTxLevel( void )
{
    return ( INT8 ) -( ModemControl->Options.FaxTxLevel / 10 );
}


/*============================================================================
*
*    ModemReadRxLevel
*
*   Reads the modem receive level
*
*   Input:      (none)
*   Output:     level
*
*=============================================================================*/
INT8 ModemReadRxLevel( void )
{
    INT8   level;
    UINT8  conf;
    UINT16 val, param;

    val = ModemDSPRead4( MODEM_AGC_GAIN );

    conf = MDM_READ( MODEM_CONF );

    switch ( conf )
    {
        case MODEM_V21_CH2_CONF:
        case MODEM_V23_75TX_1200RX_CONF:

            param = 55;  // for V.21, V.23HDX75RX
            break;

        case MODEM_V23_1200TX_75RX_CONF:

            param = 48;  // for V.23HDX1200RX
            break;

        default:

            if ( ( ( conf & 0xF0 ) == 0xC0 ) && ( MDM_READ( MODEM_RXHSKSTAT ) == 0x84 ) )
            {
                param = 48;  // for V.34(control)
            }
            else
            {
                param = 55;  // for V.34(primary),33,17,29,27ter
            }
            break;
    }

    level = ( INT8 )( ( ( val * 10 ) / 6827 ) - param );

    TRACE_MODEM( "ModemReadRxLevel: %ddBm\n", level );

    return ( level );
}


/*============================================================================
*
*    ModemReadEQM
*
*   Reads the modem Eye Quality Monitor
*
*   Input:      (none)
*   Output:     eqm
*
*=============================================================================*/
UINT16 ModemReadEQM( void )
{
    UINT16 retval;

    /* EQM access code */
    retval = ModemDSPRead4( MODEM_EQM );

    TRACE_MODEM( "ModemReadEQM: 0x%X\n", retval );

    return ( retval );
}


/*============================================================================
*
*    ModemSetDTMFOnTime
*
*   Sets the minimum DTMF on time
*
*   Input:      on time (msec)
*   Output:     Modem minimum DTMF on time modified
*
*=============================================================================*/
void ModemSetDTMFOnTime( INT16 on_time )
{
    UINT8 i;
    UINT16 Orig;
    UINT16 Sum;

    /* Convert to 10's of msec */
    on_time /= 10;

    /* Minimum time is 20 msec */
    Orig = Sum = on_time - 2;

    /* Requested time < minimum? */
    if ( on_time < 0 )
        return ;

    /* Now convert to samples */
    for ( i = 0; i < 94; i++ )
        Sum += Orig;

    /* Write to DSP */
    ModemDSPWrite3( MODEM_DTMF_MIN_ON_TIME, Sum );
}


/*============================================================================
*
*    ModemSetFlagCnt
*
*   Sets the number of flags between frames.
*
*   Input:      flag count
*   Output:     (none)
*
*============================================================================*/
void ModemSetFlagCnt( UINT16 flag_count )
{
    /* Write flag count to DSP */
    ModemDSPWrite1( MODEM_FLAGS2TX, flag_count, FALSE );
}


/*============================================================================
*
*    ModemSetMaxGain
*
*   Sets the modem max. gain word
*
*   Input:  gain word
*   Output:     MAXG written to modem
*
*=============================================================================*/
void ModemSetMaxGain( UINT16 gain )
{}


/*============================================================================
*
*    ModemSetNewMaxGain
*
*   Sets the modem max. gain word to 10 dB greater than current
*            gain.
*
*   Input:      DeltaMaxGain = Amount to subtract from gain word for new max
*             gain (default = 10 dB)
*           RxMaxGain = Default max gain (normally $0FC0 = 44 dB)
*
*   Output:     New MAXG written to modem
*
*=============================================================================*/
void ModemSetNewMaxGain( void )
{
    INT16 gain;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    gain = ModemReadAGCGain();

    gain -= DeltaMaxGain;

    if ( gain >= 0 && gain <= ModemControl->Options.RxMaxGain )
    {
        TRACE_MODEM( "ModemSetNewMaxGain: Setting max gain %d\n", gain );

        ModemSetMaxGain( ( UINT16 ) gain );
    }

    OSC_Release_Mutex( &modem_dsp_mutex );
}


/*============================================================================
*
*    ModemSetVoiceAGC
*
*   Sets voice AGC parameters
*
*   Input:   RecordSource - one of the following
*               TAM_LINE_RECORDING
*               TAM_LOCAL_RECORDING
*
*   Output:     (none)
*
*============================================================================*/
void ModemSetVoiceAGC( UINT8 RecordSource )
{}


/*============================================================================
*
*    ModemSetVoiceIntRate
*
*   Sets voice interrupt rate.  Number of voice sample times (8 KHz)
*   between interrupts in a burst transfer is n + 1.
*
*   Input:      (none)
*   Output:     (none)
*
*============================================================================*/
void ModemSetVoiceIntRate( void )
{}


/*============================================================================
*
*    ModemSetVoiceSpeed
*
*   Sets voice playback speed
*
*   Input:  playback speed:
*             0 - 50% slow
*             1 - Normal
*             2 - 33% fast
*             3 - 50% fast
*
*   Output:     (none)
*
*============================================================================*/
void ModemSetVoiceSpeed( UINT8 playback_speed )
{}


/*===========================================================================
*
*   ModemSetVoiceThreshold
*
*   Sets the voice turn-on and turn-off thresholds
*
*   Input:      VOXOnThresh = voice turn-on threshold
*           VOXOffThresh = voice turn-off threshold
*   Output:     (none)
*
*===========================================================================*/
void ModemSetVoiceThreshold( void )
{}


/*============================================================================
*
*    ModemTxCNG
*
*   Uses the modem to continuously transmit a CNG tone for a duration specified in DSP RAM
*           (default is 3 sec on, .5 sec off)
*       NOTE: Tone is stopped when NEWC is set or 0xFF is written to TBUFF
*   Input:   none
*
*   Output:     Modem configured to tone mode
*               CNG tone transmitted
*
*============================================================================*/
void ModemTxCNG( void )
{
    TRACE_MODEM( "ModemTxCNG()\n" );

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    /* Configure the modem for tone generation */
    MDM_WRITE( MODEM_CONF, MODEM_DIALING_CONF );
    MDM_CLEARBIT( MODEM_09, DATA | DTR );
    MDM_SETBIT( MODEM_09, DTMF | ORG );
    ModemTxSetLevel( ModemControl->Options.FaxTxLevel );

    MDM_SETBIT( MODEM_1F, NEWC );
    ModemBitOrBust( MODEM_1F, NEWC, 0 );

    /* Wait for transmit buffer to be empty */
    ModemBitOrBust( MODEM_1E, TDBE, 1 );

    /* Put CNG code in transmit buffer */
    MDM_WRITE( MODEM_TBUFF, 0x11 );

    OSC_Release_Mutex( &modem_dsp_mutex );
}


/*============================================================================
*
*    ModemTxCED
*
*   Uses the modem to transmit a CED tone
*   Input:   none
*
*   Output:     Modem configured to tone mode
*               CED tone transmitted
*
 *============================================================================*/
void ModemTxCED( void )
{
    TRACE_MODEM( "ModemTxCED()\n" );

    ModemTxTone( 2100, ModemControl->Options.FaxTxLevel );
}


/*============================================================================
*
*    ModemTxTone
*
*   Uses the modem to transmit single tone
*
*   Input:   Frequency(Hz),
*            Level(dBm)
*
*   Output:     Modem configured to tone mode
*               single tone transmitted
*
*============================================================================*/
void ModemTxTone( UINT16 freq, UINT8 level )
{
    float  fValue;
    UINT16 wValue;
    UINT8  cLevel;

    TRACE_MODEM( "ModemTxTone freq %dHz\n", freq );
    TRACE_MODEM( "      level -%d.%ddBm\n", level / 10, level % 10 );

    if ( !ModemDetectPresence() )
        return ;

    if ( freq == 0 )
    {
        MDM_CLEARBIT( MODEM_08, RTS );        /* Turn off the tone */
        return ;
    }

    MDM_CLEARBIT( MODEM_05, CEQ );

    ModemDSPWrite2( MODEM_TX_OUTPUT_GAIN_ALL_H, MODEM_TX_OUTPUT_GAIN_ALL_L, 0x4000 );
    cLevel = ( MDM_READ( MODEM_TLVL ) & 0x0F ) + ( 9 << 4 );
    MDM_WRITE( MODEM_TLVL, cLevel );

    /* If not already in tone mode, configure for tone mode */
    if ( MDM_READ( MODEM_CONF ) != MODEM_SINGLE_TONE_CONF )
    {
        /* Set up the modem for dial mode */
        ModemConfig( MODEM_SINGLE_TONE_CONF );
    }

    /* Set frequency */

    fValue = ( ( float ) freq / 0.109863 );

//	TRACE_MODEMDEBUG( "fF:%f\n", fValue );

    wValue = ( UINT16 ) fValue;

//	TRACE_MODEMDEBUG( "wF:0x%04X\n", wValue );

    ModemDSPWrite2( MODEM_DUAL_TONE1_FREQ_H, MODEM_DUAL_TONE1_FREQ_L, wValue );

    /* Set level */

    /* Fine tune */
    if ( SDAA_Data_Table.m_country.LineImpedance == COMPLEX )
        level -= 4 + ( 21 - ( freq / 100 ) );
    else
        level -= 7;

    if ( level > 90 )
    {
        wValue = level - 90;

//		TRACE_MODEMDEBUG(" adjust -%d\n", wValue );

        fValue = -( ( float ) wValue / 200 );
    }
    else
    {
        wValue = 90 - level;

//		TRACE_MODEMDEBUG(" adjust +%d\n", wValue );

        fValue = ( ( float ) wValue / 200 );
    }

//	TRACE_MODEMDEBUG( "fL:%f\n", fValue );

    fValue = powf( ( float ) 10, fValue );

//	TRACE_MODEMDEBUG( "fL:%f\n", fValue );

    wValue = ( UINT16 ) ( fValue * 8192 );

//	TRACE_MODEMDEBUG( "wL:0x%04X\n", wValue );

    ModemDSPWrite2( MODEM_DUAL_TONE1_POWER_H, MODEM_DUAL_TONE1_POWER_L, wValue );

    MDM_SETBIT( MODEM_08, RTS );        /* Turn on the tone */
}


/*============================================================================
*
*    ModemTxDualTone
*
*   Uses the modem to transmit dual tone
*
*   Input:   Frequency1(Hz), Level1(dBm)
*            Frequency2(Hz), Level2(dBm)
*
*   Output:     Modem configured to tone mode
*               single tone transmitted
*
*============================================================================*/
void ModemTxDualTone( UINT16 freq1, UINT8 level1, UINT16 freq2, UINT8 level2 )
{
    UINT8  cLevel;

    TRACE_MODEM( "ModemTxDualTone()\n" );

    if ( !ModemDetectPresence() )
        return ;

    if ( ( freq1 == 0 ) && ( freq2 == 0 ) )
    {
        MDM_CLEARBIT( MODEM_08, RTS );        /* Turn off the tone */
        return ;
    }

    MDM_CLEARBIT( MODEM_05, CEQ );

    ModemDSPWrite2( MODEM_TX_OUTPUT_GAIN_ALL_H, MODEM_TX_OUTPUT_GAIN_ALL_L, 0x4000 );
    cLevel = ( MDM_READ( MODEM_TLVL ) & 0x0F ) + ( 9 << 4 );
    MDM_WRITE( MODEM_TLVL, cLevel );

    /* If not already in tone mode, configure for tone mode */
    if ( MDM_READ( MODEM_CONF ) != MODEM_DUAL_TONE_CONF )
    {
        /* Set up the modem for dial mode */
        ModemConfig( MODEM_DUAL_TONE_CONF );
    }

    ModemTxSetDualToneFreq( freq1, freq2 );

    /* Fine tune */
    if ( SDAA_Data_Table.m_country.LineImpedance == COMPLEX )
    {
        level1 -= 4 + ( 21 - ( freq1 / 100 ) );
        level2 -= 4 + ( 21 - ( freq2 / 100 ) );
    }
    else
    {
        level1 -= 7;
        level2 -= 7;
    }

    ModemTxSetDualToneLevel( level1, level2 );

    MDM_SETBIT( MODEM_08, RTS );        /* Turn on the tone */
}


/*============================================================================
*
*    ModemTxSetDualToneFreq
*
*   Uses the modem to transmit dual tone
*
*   Input:   Frequency1(Hz), Frequency2(Hz)
*
*   Output:     set frequency
*
*============================================================================*/
void ModemTxSetDualToneFreq( UINT16 freq1, UINT16 freq2 )
{
    float  fValue1, fValue2;
    UINT16 wValue1, wValue2;

    TRACE_MODEM( "Tone1 %dHz\n", freq1 );
    TRACE_MODEM( "Tone2 %dHz\n", freq2 );

    fValue1 = ( ( float ) freq1 / 0.109863 );
    fValue2 = ( ( float ) freq2 / 0.109863 );

//	TRACE_MODEMDEBUG( "F1:%f F2%f\n", fValue1, fValue2 );

    wValue1 = ( UINT16 ) fValue1;
    wValue2 = ( UINT16 ) fValue2;

//	TRACE_MODEMDEBUG( "F1:0x%04X F2:0x%04X\n", wValue1, wValue2 );

    ModemDSPWrite2( MODEM_DUAL_TONE1_FREQ_H, MODEM_DUAL_TONE1_FREQ_L, wValue1 );
    ModemDSPWrite2( MODEM_DUAL_TONE2_FREQ_H, MODEM_DUAL_TONE2_FREQ_L, wValue2 );
}


/*============================================================================
*
*    ModemTxSetDualToneLevel
*
*   Uses the modem to transmit dual tone
*
*   Input:   Level1(dBm)
*            Level2(dBm)
*
*   Output:     set level
*
*============================================================================*/
void ModemTxSetDualToneLevel( UINT8 level1, UINT8 level2 )
{
    float  fValue1, fValue2;
    UINT16 wValue1, wValue2;

    TRACE_MODEM( "Tone1 -%d.%ddBm\n", level1 / 10, level1 % 10 );
    TRACE_MODEM( "Tone2 -%d.%ddBm\n", level2 / 10, level2 % 10 );

    if ( level1 > 90 )
    {
        wValue1 = level1 - 90;

//		TRACE_MODEMDEBUG(" adjust1 -%d\n", wValue1 );

        fValue1 = -( ( float ) wValue1 / 200 );
    }
    else
    {
        wValue1 = 90 - level1;

//		TRACE_MODEMDEBUG(" adjust1 +%d\n", wValue1 );

        fValue1 = ( ( float ) wValue1 / 200 );
    }

    if ( level2 > 90 )
    {
        wValue2 = level2 - 90;

//		TRACE_MODEMDEBUG(" adjust2 -%d\n", wValue2 );

        fValue2 = -( ( float ) wValue2 / 200 );
    }
    else
    {
        wValue2 = 90 - level2;

//		TRACE_MODEMDEBUG(" adjust2 +%d\n", wValue2 );

        fValue2 = ( ( float ) wValue2 / 200 );
    }

//	TRACE_MODEMDEBUG( "L1:%f L2:%f\n", fValue1, fValue2 );

    fValue1 = powf( ( float ) 10, fValue1 );
    fValue2 = powf( ( float ) 10, fValue2 );

//	TRACE_MODEMDEBUG( "L1:%f L2:%f\n", fValue1, fValue2 );

    wValue1 = ( UINT16 ) ( fValue1 * 8192 );
    wValue2 = ( UINT16 ) ( fValue2 * 8192 );

//	TRACE_MODEMDEBUG( "L1:0x%04X L2:0x%04X\n", wValue1, wValue2 );

    ModemDSPWrite2( MODEM_DUAL_TONE1_POWER_H, MODEM_DUAL_TONE1_POWER_L, wValue1 );
    ModemDSPWrite2( MODEM_DUAL_TONE2_POWER_H, MODEM_DUAL_TONE2_POWER_L, wValue2 );
}


/*============================================================================
*
*    ModemTxSetDTMFLevle
*
*   Set the DTMF level
*
*   Input:   DTMF digit, 0-9, 0x0a = *, 0x0b = #, 0x0c = A, - 0x0f = D
*
*   Output:     set level
*
*============================================================================*/
void ModemTxSetDTMFLevle( UINT8 digit, UINT8 lband_level, UINT8 hband_level )
{
    float  fValueL, fValueH;
    UINT16 wValueL, wValueH;

    TRACE_MODEM( "Low band -%d.%ddBm\n", lband_level / 10, lband_level % 10 );
    TRACE_MODEM( "Hi  band -%d.%ddBm\n", hband_level / 10, hband_level % 10 );

    if ( ModemControl->Options.DTMFCompensation )
    {
        lband_level += DTMFCompLevel[ digit ].lBand - 10;
        hband_level += DTMFCompLevel[ digit ].hBand - 20;
    }
    else
    {
        lband_level -= 12;
        hband_level -= 13;
    }

//	TRACE_MODEMDEBUG( "adjusted low %d, high %d\n", lband_level, hband_level );

    fValueL = -( ( float ) lband_level / 200 );
    fValueH = -( ( float ) hband_level / 200 );

//	TRACE_MODEMDEBUG( "L:%f H:%f\n", fValueL, fValueH );

    fValueL = powf( ( float ) 10, fValueL );
    fValueH = powf( ( float ) 10, fValueH );

//	TRACE_MODEMDEBUG( "L1:%f L2:%f\n", fValueL, fValueH );

    wValueL = ( UINT16 ) ( fValueL * 10143 );
    wValueH = ( UINT16 ) ( fValueH * 10143 );

//	TRACE_MODEMDEBUG( "L:0x%04X H:0x%04X\n", wValueL, wValueH );

    /* Write the output level to the modem */
    ModemDSPWrite2( MODEM_DTMF_LO_BAND_POWER_H, MODEM_DTMF_LO_BAND_POWER_L, wValueL );
    ModemDSPWrite2( MODEM_DTMF_HI_BAND_POWER_H, MODEM_DTMF_HI_BAND_POWER_L, wValueH );
}


/*============================================================================
*
*    ModemTxDTMFNumbers
*
*   Uses the modem to transmit a DTMF digits
*
*   Input:   DTMF digits pointer, 0-9, 0x0a = *, 0x0b = #, 0x0c = A, - 0x0f = D
*            time is 0 for contunuing, digit 0xff for stop
*
*   Output:     Modem configured to dialing mode
*               DTMF digit transmitted
*
*============================================================================*/
void ModemTxDTMFNumbers( UINT8 *digits_buffer, UINT8 on_time, UINT8 off_time, UINT8 lband_level, UINT8 hband_level )
{
    UINT8 *digits;
    UINT8 delay;

    if ( !ModemDetectPresence() )
        return ;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    /* If not already in tone mode, configure for tone mode */
    if ( MDM_READ( MODEM_CONF ) != MODEM_DIALING_CONF )
    {
        MDM_CLEARBIT( MODEM_05, CEQ );

        /* Set up the modem for dial mode */
        ModemConfig( MODEM_DIALING_CONF );

        MDM_SETBIT( MODEM_09, DTMF );

        /* Delay at least 4 ms per SSF336 designer's guide */
        OSC_Sleep( 4 );

        MDM_SETBIT( MODEM_1F, NEWC );
        ModemBitOrBust( MODEM_1F, NEWC, 0 );
    }

    digits = digits_buffer;

    while ( *digits != 0xff )
    {
        TRACE_MODEM( "ModemTxDTMFNumbers [ %X ]\n", *digits );

        if ( *digits == PHONE_DIAL_PAUSE_CHAR )
        {
            digits++;
            OSC_Sleep( gFaxDelayForPauseKey );
            continue;
        }

        /* Program the digit and wait for the modem to complete */
        ModemTxSetDTMFTime( on_time, off_time );
        ModemTxSetDTMFLevle( *digits, lband_level, hband_level );
        MDM_WRITE( MODEM_TBUFF, *digits );
        delay = ( on_time + off_time) / 10;
        delay *= 10;
        OSC_Sleep( delay );

        ModemBitOrBust( MODEM_1E, TDBE, 1 );

        digits++;
    }

    OSC_Release_Mutex( &modem_dsp_mutex );
}


/*============================================================================
*
*    ModemTxDTMFDigit
*
*   Uses the modem to transmit a DTMF digit
*
*   Input:   DTMF digit, 0-9, 0x0a = *, 0x0b = #, 0x0c = A,, 0x0f = D
*            time is 0 for contunuing, digit 0xff for stop
*
*   Output:     Modem configured to dialing mode
*               DTMF digit transmitted
*
*============================================================================*/
void ModemTxDTMFDigit( UINT8 digit, UINT16 on_time, UINT16 off_time, UINT8 lband_level, UINT8 hband_level )
{
    UINT16 wValue;

    TRACE_MODEM( "ModemTxDTMFDigit [ %X ]\n", digit );

    if ( !ModemDetectPresence() )
        return ;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    /* If not already in tone mode, configure for tone mode */
    if ( MDM_READ( MODEM_CONF ) != MODEM_DIALING_CONF )
    {
        MDM_CLEARBIT( MODEM_05, CEQ );

        /* Set up the modem for dial mode */
        ModemConfig( MODEM_DIALING_CONF );

        MDM_SETBIT( MODEM_09, DTMF );

        /* Delay at least 4 ms per SSF336 designer's guide */

        OSC_Sleep( 4 );

        MDM_SETBIT( MODEM_1F, NEWC );
        ModemBitOrBust( MODEM_1F, NEWC, 0 );
    }

    if ( on_time != 0 )
        wValue = ( on_time * 24 ) / 10;
    else
        wValue = 0;

    ModemDSPWrite2( MODEM_DTMF_TONE_DUR_H, MODEM_DTMF_TONE_DUR_L, wValue );
    ModemTxSetDTMFLevle( digit, lband_level, hband_level );

    /* Program the digit and wait for the modem to complete */
    MDM_WRITE( MODEM_TBUFF, digit );

    ModemBitOrBust( MODEM_1E, TDBE, 1 );

    OSC_Release_Mutex( &modem_dsp_mutex );
}


/*============================================================================
*
*    ModemTxSetDTMFTime
*
*   Set the DTMF time
*
*   Input:   on time(ms), off time(ms)
*
*   Output:     set time
*
*============================================================================*/
void ModemTxSetDTMFTime( UINT16 tone_on_time, UINT16 tone_off_time )
{
    UINT16 duration;

    if ( tone_on_time )
    {
        duration = ( tone_on_time * 24 ) / 10;
        ModemDSPWrite2( MODEM_DTMF_TONE_DUR_H, MODEM_DTMF_TONE_DUR_L, duration );
    }

    if ( tone_off_time )
    {
        duration = ( tone_off_time * 24 ) / 10;
        ModemDSPWrite2( MODEM_DTMF_INTERDIG_DELAY_H, MODEM_DTMF_INTERDIG_DELAY_L, duration );
    }
}


/*============================================================================
*    ModemConfigRxDTMFDigit
*   Sets up to receive DTMF digits
*============================================================================*/
void ModemRxConfigDTMFDigit ( BOOL setup_min_time, UINT8 min_time )
{
    TRACE_MODEM( "ModemRxConfigDTMFDigit.\n" );

    if ( !ModemDetectPresence() )
        return ;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    if ( setup_min_time )
        ModemSetDTMFOnTime( min_time );

    /* Write to DSP */
    ModemDSPWrite3( MODEM_DTMF_FREQ_DEV_LOW, 0x057A );
    ModemDSPWrite3( MODEM_DTMF_FREQ_DEV_HIGH, 0x07A4 );
    ModemDSPWrite3( MODEM_DTMF_TWIST_NEG, 0x1AD7 );
    ModemDSPWrite3( MODEM_DTMF_TWIST_POS, 0x0885 );

    MDM_CLEARBIT( MODEM_1B, DTMFD );

    OSC_Release_Mutex( &modem_dsp_mutex );
}


// This new one is compatible with the one in MODEM214 driver and phone driver. (2/8/00)
/*-------------------------------------------------------------------
    Name: ModemRxDTMFDigit - Determins if a DTMF digit has been received and
          returns it if so.

    Input: pDTMFDigit - buffer to store received DTMF digit
    Output: *pDTMFDigit - received DTMF digit in ASCII format
    Return: TRUE - received a DTMF digit, FALSE - no DTMF digit received
 *-------------------------------------------------------------------*/
#define MAX_RETRY   5
#define DTMFW   0x0F    /* DTMF Output Word */

BOOL ModemRxDTMFDigit( UINT8 *pDTMFDigit )
{
    const UINT8 DTMFDecodeDigit[] =
    {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', '*', '#',
        'A', 'B', 'C', 'D'
    };

    UINT8 i;

    if ( !ModemDetectPresence() )
        return FALSE;

    /* Has a DTMF digit been received ? */
    if ( MDM_BITCLEAR( MODEM_1B, DTMFD ) )
        return FALSE;

    i = MDM_READ( MODEM_DTMFW );
    *pDTMFDigit = DTMFDecodeDigit[ i & DTMFW ];

    TRACE_MODEM( "Detected DTFM Digit: %c, 0x%02X.\n",
           *pDTMFDigit, i );

    for ( i = 0; i < MAX_RETRY; i++ )
    {
        MDM_CLEARBIT( MODEM_1B, DTMFD );
        if ( MDM_BITCLEAR( MODEM_1B, DTMFD ) )
            break;
    }
    return TRUE;
}


/*============================================================================
*
*    ModemFlagSequence
*
*   Determines if flag sequence is being transmitted
*
*   Input:  none
*   Output: 1 if flags being transmitted, 0 if not
*
*============================================================================*/
UINT8 ModemFlagSequence( void )
{
    if ( !ModemFEDStatus() )
        return FALSE;

    while (1)
    {
        if ( MDM_BITSET( MODEM_0A, SYNCD ) )
        {
            return TRUE;
        }
        else if ( MDM_BITSET( MODEM_0C, RXFNE ) )
        {
            MDM_READ( MODEM_RBUFF );
        }
        else
        {
            return FALSE;
        }
    }
}


/*============================================================================
*
*    ModemFSKFlags
*
*   Determines if V.21 Channel2 flags (0x7E) are being received
*
*	Input:  none
*	Output: 1 if V.21 Ch2 flags have been detected, 0 if not
*
*============================================================================*/
UINT8 ModemFSKFlags( void )
{
    switch ( MDM_READ( MODEM_CONF ) )
    {
        case MODEM_V21_CH2_CONF:

            if ( ModemFEDStatus() )
                return ModemFlagSequence();
            else
                return FALSE;

        case MODEM_V17_14400_CONF:
        case MODEM_V17_12000_CONF:
        case MODEM_V17_9600_CONF :
        case MODEM_V17_7200_CONF :
        case MODEM_V29_9600_CONF :
        case MODEM_V29_7200_CONF :
        case MODEM_V29_4800_CONF :
        case MODEM_V27_4800_CONF :
        case MODEM_V27_2400_CONF :

            if ( ModemFEDStatus() )
                return ModemToneDetected( MODEM_HISPEED_FSK_TONE );
            else
                return FALSE;

        default:
            return ModemToneDetected( MODEM_FSK_TONE );
    }
}


/*============================================================================
*
*    ModemDetectTone
*
*   Determines if a tone has been detected
*
*   Input:  tone mask for
*               Dial Tone
*               Busy Tone
*               FSK Tone
*               CNG Tone
*               CED Tone
*   Output: non-zero if tone detected, 0 if not
*
*============================================================================*/
UINT8 ModemDetectTone( UINT8 on_mask, UINT8 off_mask )
{
    UINT8 tone;

    tone = MDM_READ( MODEM_0B ) & 0xE0;

    if ( tone & ~off_mask )
        return 0;
    else
        return ( tone & on_mask );
}


/*============================================================================
*
*    ModemRxCarrierError
*
*   Determines if carrier is present
*
*   Input:  0:start, other:measure
*   Output: 0:if good carrier, 1:if no carrier, 2:if bad EQM, 3:if RLSD off
*
*============================================================================*/
UINT8 ModemRxCarrierError( UINT8 ctrl )
{
    static UINT32 _no_energy_msec, _no_data_msec, _bad_eqm_msec, total_fed_off;
    static UINT8  ret, count;
    UINT8  rx_state, tx_state;

    if ( ctrl == 0 )
    {
        ModemTimerStart( &_no_energy_msec );
        ModemTimerStart( &_no_data_msec );
        ModemTimerStart( &_bad_eqm_msec );
        total_fed_off = 0;
        count = 0;
        ret = CARRIER_ON;
        return ret;
    }

    if ( MDM_BITSET( MODEM_0F, FED ) )
    {
        ModemTimerStart( &_no_energy_msec );

        if ( ModemControl->V34Mode )
        {
            ModemTimerStart( &_no_energy_msec );

            if ( ModemControl->IsrDataCounter != 0 )
            {
                ModemTimerStart( &_no_data_msec );
            }

            rx_state = MDM_READ( MODEM_RXHSKSTAT );
            tx_state = MDM_READ( MODEM_TXHSKSTAT );

            if ( rx_state == PC_RX_DATA_MODE )
            {
                if ( MDM_BITSET( MODEM_0B, EQMAT ) )
                {
                    MDM_CLEARBIT( MODEM_0B, EQMAT );
                    count++;

                    TRACE_MODEM( " EQMAT %d\n", count );

					if ( count == 17 /*500 / T30_DELAY_TIME*/ ) // 500ms
                    {
                        OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

                        ModemControl->EQM = ModemReadEQM();
                        ModemControl->Level = ModemReadRxLevel();

                        ModemDSPWrite5( 0x4802, 0x0000, 0x0002 );
                        ModemDSPWrite2( 0x274, 0x273, 0x0001 );

                        OSC_Release_Mutex( &modem_dsp_mutex );

                        MDM_SETBIT( MODEM_05, FCCR );

                        if ( HDLCRxReadFrameAvailable() == 0 )
                        {
                            ModemV34FallSpeed();
                        }

                        ret = EQM_BAD;
                    }
                    else
                    {
                        ret = CARRIER_ON;
                    }
                }
                else
                {
					if( count > 0 ) {
						count--;
					}
					if( ModemControl->IsrDataCounter != 0 ) {
						ret = CARRIER_ON;
					}
                }
            }
            else if ( ModemControl->HighSpeed )
            {
                if ( ModemTimerExpired( _no_data_msec, 1000 ) )  // 1000ms
                {
                    ModemTimerStart( &_no_data_msec );

                    if ( ( rx_state == CC_INIT ) || ( rx_state >= PC_INIT ) )
                    {
                        TRACE_MODEM( " Idle (%02X %02X)\n", tx_state, rx_state );

                        OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

                        ModemDSPWrite2( 0x274, 0x273, 0x0001 );

                        OSC_Release_Mutex( &modem_dsp_mutex );

                        MDM_SETBIT( MODEM_05, FCCR );
                        ret = EQM_BAD;
                    }
                    else if ( MDM_BITSET( MODEM_0C, PCOFF ) || ( rx_state > CC_INIT ) )
                    {
                        TRACE_MODEM( " PCOFF or (%02X %02X)\n", tx_state, rx_state );
                        ret = CARRIER_ON;
                    }
                    else
                    {
                        TRACE_MODEM( " (%02X %02X) %d\n", tx_state, rx_state, ret );
                        ret = CARRIER_ON;
                    }
                }
                else
                {
                    ret = CARRIER_ON;
                }
            }
        }
        else  // V.17 FED on
        {
            if ( !ModemControl->HDLCMode )
            {
                if ( ret == EQM_BAD )
                {
                    if ( ModemTimerExpired( _bad_eqm_msec, 60000 ) )  // 60sec failsafe
                    {
                        TRACE_MODEM( " # TIMEOUT bad_eqm_msec\n" );

                        ModemControl->Status |= MODEM_ABORT;
                        return EQM_BAD;
                    }
                }
            }

            if ( ret != CARRIER_ON )
            {
                ModemControl->EQM = ModemReadEQM();

                if ( ModemControl->EQM < 0x5000 )
                {
                    ret = CARRIER_ON;
                }
                else
                {
                    if ( ret != EQM_BAD )
                    {
                        if ( ret != CARRIER_OFF )
                        {
                            TRACE_MODEM( " * restart bad_eqm_msec %04X\n", ModemControl->EQM );

                            ModemTimerStart( &_bad_eqm_msec );
                        }
                    }

                    ret = EQM_BAD;
                }
            }
            else if ( MDM_BITSET( MODEM_0B, EQMAT ) )
            {
                if ( ret != EQM_BAD )
                {
                    TRACE_MODEM( " * restart bad_eqm_msec EQMAT\n" );

                    ModemTimerStart( &_bad_eqm_msec );
                }

                MDM_CLEARBIT( MODEM_0B, EQMAT );
                ret = EQM_BAD;
            }
            else if ( MDM_BITCLEAR( MODEM_0F, RLSD ) )
            {
                ret = RLSD_OFF;
            }
            else
            {
                ret = CARRIER_ON;
            }
        }
    }
    else  // FED off
    {
        if ( ModemControl->V34Mode )
        {
            if ( ModemTimerExpired( _no_energy_msec, 1000 ) )  // 1000ms
            {
                TRACE_MODEM( " V34 FED off %ldms\n", _no_energy_msec );

                if ( ModemControl->HighSpeed )
                {
                    rx_state = MDM_READ( MODEM_RXHSKSTAT );
                    tx_state = MDM_READ( MODEM_TXHSKSTAT );

                    TRACE_MODEM( " (%02X %02X)\n", tx_state, rx_state );

                    if ( rx_state != CC_INIT )
                    {
                        OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

                        ModemDSPWrite5( 0x4802, 0x0000, 0x0002 );
                        ModemDSPWrite2( 0x274, 0x273, 0x0001 );

                        OSC_Release_Mutex( &modem_dsp_mutex );

                        MDM_SETBIT( MODEM_05, FCCR );
                    }
                }

                ModemTimerStart( &_no_energy_msec );
            }
        }
        else  // V.17 FED off
        {
            if ( !ModemControl->HDLCMode )
            {
				if ( ++total_fed_off > ( 400 /*12000 / T30_DELAY_TIME*/ ) )  // 12sec failsafe
                {
                    TRACE_MODEM( " # TIMEOUT total_fed_off\n" );

                    ModemControl->Status |= MODEM_ABORT;
                    total_fed_off = 0;
                    return CARRIER_OFF;
                }

                if ( ret == CARRIER_ON )
                {
                    TRACE_MODEM( " * restart bad_eqm_msec\n" );

                    ModemTimerStart( &_bad_eqm_msec );
                }

                if ( ModemTimerExpired( _bad_eqm_msec, 60000 ) )  // 60sec failsafe
                {
                    TRACE_MODEM( " # TIMEOUT bad_eqm_msec\n" );

                    ModemControl->Status |= MODEM_ABORT;
                }

				if ( ( total_fed_off % ( ( 57600 /*100 * 14400 / T30_DELAY_TIME*/ ) / ModemControl->ConnectedSpeed ) ) == 0 )
                {
                    ModemCDBWriteErrorData();
                }
            }
        }

        ret = CARRIER_OFF;
    }

    return ret;
}


/*============================================================================
*
*    ModemRTSConfig
*
*   Turns the transmitter on or off
*
*   Input:  1 - turn on transmitter, 0 - turn off transmitter
*   Output: 1 if available, 0 if not
*
*============================================================================*/
void ModemRTSConfig( UINT8 mode )
{
    static UINT32 _delay_msec;
    int ie;

    /* Turn on the transmitter */
    if ( mode )
        MDM_SETBIT( MODEM_08, RTS );

    else if ( ModemControl->Status & MODEM_ABORT )
    {
        MDM_CLEARBIT( MODEM_08, RTS );
        MDM_CLEARBIT( MODEM_08, ORG | DTR );
        ModemControl->HighSpeed = FALSE;
        ModemControl->State = MODEM_IDLE;
    }
    /* Turn off the transmitter */
    else
    {
        if ( ModemControl->V34Mode )
            ModemTxSwitchPCtoCC();
        else
        {
            while ( 1 )
            {
                if ( ModemTxWaitForFIFOEmpty( 1 ) )
                {
                    ModemTimerStart( &_delay_msec );
                    ie = 130 / 10;  // failsafe counter

                    while ( !ModemTimerExpired( _delay_msec, 120 ) )
                    {
                        /* Wait for sending all data out */
                        OSC_Sleep( 10 );

                        if ( --ie == 0 )
                            break;
                    }

                    MDM_CLEARBIT( MODEM_08, RTS );
                    /* Set signal gap timer */
                    MODEM_TIMER_START( TimerEvent, 90 );

                    ModemTimerStart( &_delay_msec );
                    ie = 50 / 10;  // failsafe counter

                    while ( !ModemTimerExpired( _delay_msec, 40 ) )
                    {
                        /* Wait for turn off sequence */
                        OSC_Sleep( 10 );

                        if ( --ie == 0 )
                            break;
                    }

                    ModemControl->State = MODEM_IDLE;
                    break;
                }

                OSC_Sleep( MODEM_DELAY_TIME );
            }
        }

        ModemControl->HighSpeed = FALSE;
    }
}


/*============================================================================
*
*    ModemTxStopTestSignal
*
*   Turns transmit test signal off
*
*   Input:  none
*   Output: none
*
*============================================================================*/
void ModemTxStopTestSignal( void )
{
    MDM_WRITE( MODEM_TBUFF, 0xff );
    MDM_CLEARBIT( MODEM_09, ORG | DTR );
    MDM_CLEARBIT( MODEM_08, RTS );
    MDM_CLEARBIT( MODEM_07, L3ACT );
    MDM_CLEARBIT( MODEM_15, EXL3 );
}


/*============================================================================
*
*    ModemWaitRX
*
*   Waits for the previous transmission to finish.
*
*   Output: (none)
*
*============================================================================*/
void ModemWaitRX ( void )
{
    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    ModemBitOrBust( MODEM_0F, CTS, 0 );

    OSC_Release_Mutex( &modem_dsp_mutex );
}


/*============================================================================
  The following is for maintenance and testing code
*============================================================================*/


/*============================================================================
*
*    ModemTxSetPermanentData
*
*  Initializes non-HDLC data transmission.  Configures the modem
*  to the indicated speed, init's sending data, sends
*  the training sequence, and returns.
*
*  Input:      configure = TRUE if need to reconfigure
*              data = The byte to send repeatedly
*              speed = desired data mode (index into modem speed table)
*              training = long or short train
*
*  Index     Speed
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
void ModemTxSetPermanentData( UINT8 configure, UINT8 data,
                              UINT8 speed, UINT8 training )
{
    if ( configure )
    {
        ModemControl->Status = NO_ERROR;

        ModemSetTraining( training );
        ModemTxConfigHiSpeed( speed );

        ModemClearIRQ();
        ModemControl->State = T4_TX_MODE;
    }
    /* Set up modem with the first byte of data. */
    MDM_WRITE( MODEM_TBUFF, data );
}


/*============================================================================
*
*    ModemTxGetOutputLevel and ModemTxSetOutputLevel
*
*  Get and set the current output level of the channels in the Modem tx.
*  Input sel :  1 = tone 1,  2 = tone 2,  else = primary channel
*============================================================================*/
UINT16 ModemTxGetOutputLevel( UINT8 sel )
{
    /* Obtain channel to get output level for */
    if ( sel == 2 )
    {
        return ModemDSPRead2( MODEM_DUAL_TONE2_POWER_H, MODEM_DUAL_TONE2_POWER_L );
    }
    else
    {
        return ModemDSPRead2( MODEM_DUAL_TONE1_POWER_H, MODEM_DUAL_TONE1_POWER_L );
    }
}


void ModemTxSetOutputLevel( UINT8 sel, UINT16 level )
{
    /* Obtain channel to get output level for */
    if ( sel == 2 )
    {
        ModemDSPWrite2( MODEM_DUAL_TONE2_POWER_H, MODEM_DUAL_TONE2_POWER_L, level );
    }
    else
    {
        ModemDSPWrite2( MODEM_DUAL_TONE1_POWER_H, MODEM_DUAL_TONE1_POWER_L, level );
    }
}


/*-------------------------------------------------------------------
    Name: ModemHighSampleRate - To check if the modem configuration would
          use V.8 and V.34 (9600 Hz), ADPCM/Voice (8000Hz) or others (7200Hz).
    Input:  none
    Output: none
    Return: BOOL (2: 9600Hz, 1: 8000Hz, 0: 7200Hz)
 *-------------------------------------------------------------------*/
UINT8 ModemHighSampleRate( void )
{
    UINT8 conf;

    conf = MDM_READ( MODEM_12 );

    if ( ( 0xAA == conf )
      || ( 0xC0 == ( conf & 0xF0 ) ) )
    {
        return ( 2 );
    }
    else if ( 0xAC == conf )
    {
        return ( 1 );
    }
    else
    {
        return ( 0 );
    }
}


/*-------------------------------------------------------------------
    Name: ModemRxCIDSetup - Set up modem for CID reception.
    Input:  none
    Output: none
    Return: none
 *-------------------------------------------------------------------*/
void ModemRxCIDSetup( void )
{
    UINT8 cdata;

    if ( !ModemDetectPresence() )
        return ;

    ModemControl->CDBByteNum = 0;           /* byte counter */
    ModemControl->Status = NO_ERROR;
    ModemClearIRQ();

    // These are the default value, but still do it to make sure
    MDM_CLEARBIT( MODEM_08, RTS );          /* Turn off RTS before touch ASYN and HDLC */
    MDM_CLEARBIT( MODEM_09, DTMF+ORG+DTR ); /* Answer Mode */
    MDM_CLEARBIT( MODEM_06, HDLC );         /* Turn off HDLC */
    MDM_SETBIT( MODEM_06, WDSZ_8 );         /* 8-bit/character, set before ASYN */
    MDM_SETBIT( MODEM_08, ASYN+TPDM );      /* Asynchronous mode, Parallel Tx Mode */
    MDM_SETBIT( MODEM_09, NV25+DATA );      /* No V.25 answer tone */

    MDM_SETBIT( MODEM_11, HDXEN );          /* V.23 Half Duplex Enable */

    cdata = ModemDSPRead1( MODEM_RX_FIFO ); /* Trigger level set 1 byte */
    ModemDSPWrite1( MODEM_RX_FIFO, cdata & 0x21, FALSE );

    ModemConfig( MODEM_V23_1200TX_75RX_CONF );

    if ( !( ModemDSPRead1( 0x720 ) & 0x40 ) )
    {
        cdata = SSFReadLSD( 0x09 );

        if ( ( cdata & 0x60 ) != 0x60 )
        {
            SSFWriteLSD( 0x09, cdata | 0x60 );
            TRACE_MODEM( "ModemRxCIDSetup LSD09 = %02X -> %02X\n", cdata, cdata | 0x60 );
        }
    }

    MDM_SETBIT( MODEM_09, DTR);

    TRACE_MODEM( "ModemRxCIDSetup: Start IRQ\n" );

    ModemControl->State = CID_RX_MODE;      /* Set Interrupt Vector Index */
    ModemEnableRxInt();
}


/*-------------------------------------------------------------------
    Name: ModemRxCIDShutdown - Shut down modem setup for CID reception.
    Input:  none
    Output: none
    Return: none
 *-------------------------------------------------------------------*/
void ModemRxCIDShutdown( void )
{
    if ( !ModemDetectPresence() )
        return ;

    MDM_CLEARBIT( MODEM_09, DTR);
    MDM_CLEARBIT( MODEM_06, WDSZ_8 );    /* 8-bit/character, set before ASYN */
    MDM_CLEARBIT( MODEM_08, ASYN+TPDM ); /* Asynchronous mode, Parallel Tx Mode */
    MDM_CLEARBIT( MODEM_09, NV25+DATA ); /* No V.25 answer tone */
    MDM_CLEARBIT( MODEM_11, HDXEN );     /* V.23 Half Duplex Enable */
    ModemDisableRxInt();

    ModemControl->Status = NO_ERROR;
    ModemControl->State = MODEM_IDLE;    /* Set Interrupt Vector Index */

    TRACE_MODEM( "ModemRxCIDShutdown.\n" );
}


/*-------------------------------------------------------------------
    Name: ModemDetectRing - Determines if ring burst is detected by modem.
    Input:  None
    Output: None
    Return: TRUE if ring detected, FALSE if not
    Note: This function should only be called by DAADetectRing().
          Other applications should call DAADetectRing().
 *-------------------------------------------------------------------*/
BOOL ModemDetectRing( void )
{
    BOOL ri;

    if ( !ModemDetectPresence() )
    {
        if ( sleep_state & 0x03 )
        {
            if ( MDM_BITCLEAR( MODEM_1F, NEWC ) )
            {
                ModemReset( 0 );
                sleep_state = MODEM_ACTIVE;
                ModemControl->State = MODEM_IDLE;
                phone_init_after_wakeup();
                ModemSleepTimerRestart();
            }
            else
            {
                ModemSleepTimerRestart();
                return FALSE;
            }
        }
        else
        {
            ModemSleepTimerRestart();
            return FALSE;
        }
    }

    if ( ( SSFLineStatus() == LINE_ONHOOK )
      || ( SSFLineStatus() == LINE_NOLINE ) )
    {
        ri = MDM_BITSET( MODEM_0F, RI );

        if ( ri )
        {
            ModemSleepTimerRestart();
        }
        else
        {
            if ( ( inactive_time >= 10000 )
              && ( inactive_time < ModemElapsedTime( sleep_timer ) ) )
            {
                ModemSleepStateChange( MODEM_DEEP_SLEEP );
                ModemSleepTimerRestart();
            }
        }
        return ri;
    }
    else
    {
        return FALSE;
    }
}


void ModemOffHook( UINT8 jcid )
{
    TRACE_MODEM( " Off Hook\n" );

#if 0
    if ( jcid )
    {
        SSFOffHook();
        OSC_Sleep( 100 );
        SSFStopOffHookMonitor();
    }
#endif
    if ( ModemSleepStateChange( MODEM_HOOK ) )
    {
        SSFOffHook();
#if __SUPPORT_MANUAL_TRANSMIT__
        if( jcid )
        {
            MDM_CLEARBIT( MODEM_05, CEQ );
            ModemConfig( MODEM_DIALING_CONF );
            MDM_SETBIT( MODEM_09, DTMF );
            OSC_Sleep( 4 );
            MDM_SETBIT( MODEM_1F, NEWC );
            ModemBitOrBust( MODEM_1F, NEWC, 0 );
        }
#endif
    }
}


void ModemOnHook( UINT8 jcid )
{
    TRACE_MODEM( " On Hook\n" );

    if ( jcid )
    {
        SSFStartOffHookMonitor();
        SSFOnHook();
    }
    else if ( ModemSleepStateChange( MODEM_HOOK ) )
    {
        SSFOnHook();
    }
}


/*-------------------------------------------------------------------
    Name: ModemTxPulseDigit - Modem pulse dial control
    Input:  digit 0-9, pulse_mode, make_time, break_time mSec
    Output: None
    Return: Number of dialed bytes
 *-------------------------------------------------------------------*/
void ModemTxPulseDigit( UINT8 digit, UINT8 pulse_mode, UINT8 make_time, UINT8 break_time )
{
    if ( !ModemDetectPresence() )
        return ;

    SSFPulseSetup();

    TRACE_MODEM( "[ ModemTxPulseNumbers %X ]\n", digit );

    SSFPulseDial( digit, pulse_mode, make_time, break_time );

    SSFPulseCleanup();
}


/*-------------------------------------------------------------------
    Name: ModemTxPulseNumbers - Modem pulse dial control
    Input:  digit 0-9, pulse_mode, make_time, break_time mSec
    Output: None
    Return: Number of dialed bytes
 *-------------------------------------------------------------------*/
UINT8 ModemTxPulseNumbers( UINT8 *digits_buffer, UINT8 pulse_mode, UINT8 make_time, UINT8 break_time, UINT16 inter_delay )
{
    UINT8 *digits, i = 0;

    if ( !ModemDetectPresence() )
        return 0xff;

    digits = digits_buffer;

    SSFPulseSetup();

    while ( *digits != 0xff )
    {
        TRACE_MODEM( "[ ModemTxPulseNumbers %X ]\n", *digits );

        if ( *digits == PHONE_DIAL_PULSE_TO_DTMF_CHAR )
        {
            digits++;
            i++;
            break;
        }

        if ( *digits == PHONE_DIAL_PAUSE_CHAR )
        {
            digits++;
            i++;
            OSC_Sleep( PHONE_DIAL_PAUSE_TIME );
            continue;
        }

        SSFPulseDial( *digits, pulse_mode, make_time, break_time );

        digits++;
        i++;

        OSC_Sleep( inter_delay );
    }

    SSFPulseCleanup();

    return i;
}


/*-------------------------------------------------------------------
    Name: ModemFEDStatus - Read modem FED (Fast Energy Detector) bit(s)
    Input: None
    Output: None
    Return: 0: FED not detected, else: FED detected
 *-------------------------------------------------------------------*/
UINT8 ModemFEDStatus( void )
{
    if ( !ModemDetectPresence() )
        return FALSE;

    return MDM_BITSET( MODEM_0F, FED );
}


/*-------------------------------------------------------------------
    Name: ModemVOXStatus - Read modem VOX (Voice Detected) bit
    Input: None
    Output: None
    Return: 0: VOX not detected, else: VOX detected
    Note: This function is designed for FM214, not for SSF336.
          But, we need a stub in SSF336.
 *-------------------------------------------------------------------*/
UINT8 ModemVOXStatus( void )        /* Not Supported */
{
    //  if (!ModemDetectPresence())
    //      return FALSE;
    return FALSE;
}


/*-------------------------------------------------------------------
    Name: ModemToneDetected - Read specified modem tone detector bit.
          This function provides the same interface for SSF336.
    Input: tone - MODEM_TONE_SELECT specified in ModemX.H
           MODEM_TONE_TONE specifies the tone to detect
           MODEM_DIAL_TONE use TONEA
           MODEM_BUSY_TONE use TONEA
           MODEM_CI_TONE   use TONEA
           MODEM_CNG_TONE  use TONEB
           MODEM_FSK_TONE  use TONEC
    Output: None
    Return: FALSE: tone not detected, else: tone detected
 *-------------------------------------------------------------------*/
BOOL ModemToneDetected( MODEM_TONE_SELECT tone )
{
    if ( !ModemDetectPresence() )
        return FALSE;

    switch ( tone )
    {
        case MODEM_DIAL_TONE:
            return ModemDetectTone( MODEM_DIAL_TONE_MASK, MODEM_NO_OFF_MASK );

        case MODEM_BUSY_TONE:
            return ModemDetectTone( MODEM_BUSY_TONE_MASK, MODEM_NO_OFF_MASK );

        case MODEM_CI_TONE:
            return ModemDetectTone( MODEM_CI_TONE_MASK, MODEM_NO_OFF_MASK );

        case MODEM_CNG_TONE:
            return ModemDetectTone( MODEM_CNG_TONE_MASK, MODEM_CNG_TONE_MASK );

        case MODEM_FSK_TONE:
            return ModemDetectTone( MODEM_FSK_TONE_MASK, MODEM_FSK_TONE_MASK );

        case MODEM_ANS_TONE:
            if ( MDM_BITSET( MODEM_09, DATA ) )
                return ModemDetectTone( MODEM_CED_TONE_MASK, MODEM_NO_OFF_MASK );
            else
                return ( ( MDM_READ( MODEM_0B ) & 0xF0 ) == ( TONEB | ATV25 ) ) ? TRUE : FALSE;

        case MODEM_HISPEED_FSK_TONE:
            return MDM_BITSET( MODEM_0A, FLAGDT ) ? TRUE : FALSE;

        default:
            /* This is a quick implementation and it only work for this special case. */
            return ModemDetectTone( ( UINT8 ) ( 0x80 >> tone ), MODEM_NO_OFF_MASK ) ? TRUE : FALSE;
    }
}

/*-------------------------------------------------------------------
    Name: ModemSetupToneDet - Set tone detection filters to detect different
          tone frequency.  Base on the current modem configuration, different
          sampling rate requires different tone detection coefficient tables.
          This function provides the same interface for SSF336.
          It is tested backward compatible with LM-1 code.
    Input: tone - MODEM_TONE_SELECT specified in ModemX.H
           MODEM_TONE_SELECT specifies the tone to detect.
           MODEM_DIAL_TONE use TONEA
           MODEM_BUSY_TONE use TONEA
           MODEM_CI_TONE   use TONEA
           MODEM_CNG_TONE  use TONEB
           MODEM_FSK_TONE  use TONEC
    Output: None
    Return: E_OK,
            E_PARAM - Input data out of range
 *-------------------------------------------------------------------*/
ERCODE ModemSetupToneDet( MODEM_TONE_SELECT tone )
{
    UINT8 rate;
    ERCODE ret = E_OK;

    if ( !ModemDetectPresence() )
        return E_NOTYET;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    rate = ModemHighSampleRate();

    TRACE_MODEM( "ModemSetupToneDet tone %d, rate %d\n", tone, rate );

    /* Set tone detection filters to detect different tone frequency */
    /* Busy/Dial/Congestion Tone detection coefficient tables */
    switch ( rate )    /* Modem Sampling rate */
    {
        case 0:  // 7200Hz

            switch ( tone )
            {
                case MODEM_DIAL_TONE:
                    ModemInitToneDet( TONE_A, CoeffTblHighBCD );
                    break;

                case MODEM_BUSY_TONE:
                    ModemInitToneDet( TONE_A, CoeffTblHighBCD );
                    break;

                case MODEM_CI_TONE:
                    ModemInitToneDet( TONE_A, CoeffTbl980 );
                    break;

                case MODEM_CNG_TONE:
                    ModemInitToneDet( TONE_A, CoeffTbl950 );
                    ModemInitToneDet( TONE_B, CoeffTbl1100 );
                    ModemInitToneDet( TONE_C, CoeffTbl1250 );
                    break;

                case MODEM_ANS_TONE:
                    ModemInitToneDet( TONE_B, CoeffTbl2100 );
                    break;

                case MODEM_FSK_TONE:
                    ModemInitToneDet( TONE_C, CoeffTbl1650 );
                    break;

                case MODEM_HISPEED_FSK_TONE:
                    break;

                default:
                    ret = E_PARAM;
                    break;
            }
            break;

        case 1:  // 8000Hz

            switch ( tone )
            {
                case MODEM_DIAL_TONE:
                    ModemInitToneDet( TONE_A, Tone8000CoeffTblLowBCD );
                    break;

                case MODEM_BUSY_TONE:
                    ModemInitToneDet( TONE_A, Tone8000CoeffTblHighBCD );
                    break;

                case MODEM_CNG_TONE:
                    ModemInitToneDet( TONE_B, Tone8000CoeffTbl1100 );
                    break;

                case MODEM_HISPEED_FSK_TONE:
                    break;

                default:
                    ret = E_PARAM;
                    break;
            }
            break;

        case 2:  // 9600Hz

            switch ( tone )
            {
                case MODEM_DIAL_TONE:
                    ModemInitToneDet( TONE_A, Tone9600CoeffTblHighBCD );
                    break;

                case MODEM_BUSY_TONE:
                    ModemInitToneDet( TONE_A, Tone9600CoeffTblHighBCD );
                    break;

                case MODEM_CI_TONE:
                    ModemInitToneDet( TONE_A, Tone9600CoeffTbl980 );
                    break;

                case MODEM_CNG_TONE:
                    ModemInitToneDet( TONE_B, Tone9600CoeffTbl1100 );
                    break;

                case MODEM_FSK_TONE:
                    ModemInitToneDet( TONE_C, Tone9600CoeffTbl1650 );
                    break;

                case MODEM_HISPEED_FSK_TONE:
                    break;

                default:
                    ret = E_PARAM;
                    break;
            }
            break;

        default:
            ret = E_PARAM;
            break;
    }

    OSC_Release_Mutex( &modem_dsp_mutex );

    return ret;
}

/*=======================================================================
*    ModemDisableToneDet
*       Disables tone detectors
*
*      Input:          tone:  Tone detection to be disabled
*      Output:         (none)
*=======================================================================*/
void ModemDisableToneDet( MODEM_TONE_SELECT tone )
{
    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    if ( tone == MODEM_CI_TONE )
    {
        /* Reset default values */
        ModemDSPWrite3( 0xAB8, 0x0880 );
        ModemDSPWrite3( 0xBB8, 0x0580 );
    }

    MDM_CLEARBIT( MODEM_02, TDE );

    OSC_Release_Mutex( &modem_dsp_mutex );
}


/*-------------------------------------------------------------------
    Name: ModemRxConfigDTMF - Set Up modem for DTMF detection
          If the caller already in certain mode (e.g. Voice Codec Mode) that
          is able to detect DTMF, no mode change will take place.  Otherwise,
          ModemRxConfigDTMF() will set to proper mode for DTMF detection.

    Input: None
    Output:
    Return: None
 *-------------------------------------------------------------------*/
void ModemRxConfigDTMF( void )
{
    UINT8 conf;

    if ( !ModemDetectPresence() )
        return ;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    conf = MDM_READ( MODEM_CONF );

    switch ( conf )
    {
        case MODEM_DTMF_RX_CONF:
            break;

        case MODEM_V8_CONF:

            /* DTMF Receiver in V.8 Modes (Bits 15/14,12) */
            ModemDSPWrite5( 0x480C, 0x0000, 0xC000 );
            ModemDSPWrite5( 0x480E, 0x0000, 0x1000 );
            break;

        case MODEM_V21_CONF:
        case MODEM_V21_CH2_CONF:

            /* DTMF Receiver in FSK Modes (Bits 15/14,12) */
            ModemDSPWrite5( 0x480C, 0x0000, 0x8000 );
            ModemDSPWrite5( 0x480E, 0x0000, 0x1000 );
            break;

        case MODEM_V23_1200TX_75RX_CONF:
        case MODEM_V23_75TX_1200RX_CONF:
        default:

            ModemDSPWrite5( 0x480C, 0x0000, 0x8000 );
            ModemDSPWrite5( 0x480E, 0x0000, 0x1000 );
            break;
    }

    MDM_CLEARBIT( MODEM_1B, DTMFD );

    OSC_Release_Mutex( &modem_dsp_mutex );

    TRACE_MODEM( "ModemRxConfigDTMF conf %02X\n", conf );
}


/*-------------------------------------------------------------------
    Name: ModemRxConfigRing - Set Up modem for Ring detection
    Input: None
    Output:
    Return: None
    Note: RI bit is valid only when DATA bit is reset and is
          not applicable in tone modes.
 *-------------------------------------------------------------------*/
void ModemRxConfigRing( void )
{
    TRACE_MODEM( "ModemRxConfigRing()\n" );

    if ( !ModemDetectPresence() )
        return ;

    OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

    ModemClearIRQ();

    // These are the default value, but still do it to make sure
    MDM_CLEARBIT( MODEM_08, RTS );          /* Turn off RTS before touch ASYN and HDLC */
    MDM_CLEARBIT( MODEM_09, DTMF+ORG+DTR ); /* Answer Mode */
    MDM_CLEARBIT( MODEM_06, HDLC );         /* Turn off HDLC */
    MDM_SETBIT( MODEM_06, WDSZ_8 );         /* 8-bit/character, set before ASYN */
    MDM_SETBIT( MODEM_08, ASYN+TPDM);       /* Asynchronous mode, Parallel Tx Mode */
    MDM_SETBIT( MODEM_09, NV25+DATA);       /* No V.25 answer tone */
    MDM_SETBIT( MODEM_11, HDXEN);           /* V.23 Half Duplex Enable */

    ModemConfig( MODEM_V23_1200TX_75RX_CONF );

    MDM_CLEARBIT( MODEM_0F, RI );     /* Clear RI, sometimes it locks up */
    ModemDSPWrite1( MODEM_NEWS_MASK_0F, 0x08, FALSE );       // Enable RI IRQ
    MDM_SETBIT( MODEM_1F, NSIE );     // enable NEWS IRQ

    OSC_Release_Mutex( &modem_dsp_mutex );

    if (gFaxNoAnswerFlag == TRUE)
    {
        //UINT16 jnum = fsu_job_num_rcv;

        faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_RECEIVING_FAIL, 0, 0, 0, 0, EMPTY_JOB_NUM);

        gFaxNoAnswerFlag = FALSE;

        isPhoneRingOn = 0;

#if __SUPPORT_MANUAL_RECEIVE__
        isRinging = 0;
#else
#endif

        memset(gcCID_for_RemoteFaxNum, 0, sizeof(gcCID_for_RemoteFaxNum));

        //fsu_purge_doc_dir(jnum);

        //fsu_update_status(jnum, FSU_STATE_NULL);
    }

    TRACE_MODEM( "ModemRxConfigRing() Start IRQ\n" );
}


/*-------------------------------------------------------------------
    Name: ModemSetSpeakerVolume -  hardware/physical audio volume control driver.

    Input:  UINT8 Volume - 0: Min., 7: Max.
    Output: Modify Modem scratch pad and LSD control register 0.
    Return: none

    Note: This function should only be used by DAA module or hardware test
          program, and not by other modules.
          Other modules should use firmware/logical audio volume control
          driver, such as SoundSetVolume(),  or phone message to control it.
          This one should not call any time delay function.
 *-------------------------------------------------------------------*/
void ModemSetSpeakerVolume( UINT8 Volume )
{
    if ( !ModemDetectPresence() )
        return ;

    SSFSpeakerVolume( Volume );
}


/*-------------------------------------------------------------------
    Name: ModemEnableRxInt
    Enable Rx Interrupt

    Input: None
    Output: None
 *-------------------------------------------------------------------*/
void ModemEnableRxInt( void )
{
    modem_start_task();
    MDM_SETBIT( MODEM_1E, RDBIE );
}


void ModemDisableRxInt( void )
{
    MDM_CLEARBIT( MODEM_1E, RDBIE );
    modem_stop_task();
}


void ModemEnableTxInt( void )
{
    modem_start_task();
    MDM_SETBIT( MODEM_1E, TDBIE );
}


void ModemDisableTxInt( void )
{
    MDM_CLEARBIT( MODEM_1E, TDBIE );
    modem_stop_task();
}


/*=======================================================================
*    ModemV34FallSpeed
*       Fall back the modem speed when it is needed
*
*      Input:          (none)
*      Output:         fallback speed
*=======================================================================*/
UINT16 ModemV34FallSpeed( void )
{
#if 1 /* avoid infinite fallback loop */
	UINT8 conf, next;
	UINT8 ConnectedBaudIndex;
	UINT16 speed ;

	speed = ModemControl->ConnectedSpeed;
	conf = MDM_READ( MODEM_CONF );

	ConnectedBaudIndex = ModemDSPRead1( 0x2E3 ) + 3;

	if ( ConnectedBaudIndex > 3 )
		ConnectedBaudIndex--;

	if ( ConnectedBaudIndex == 3 )
	{
		next = MODEM_V34_2400_CONF;
	}
	else
	{
		next = MODEM_V34_4800_CONF;
	}

	if ( MDM_BITSET( MODEM_09, ORG ) )
	{
		if ( ModemControl->ConnectedSpeed
		  == V34SpeedTbl[ conf - MODEM_V34_2400_CONF ] )
		{
			if ( conf > next )
			{
				MDM_WRITE( MODEM_CONF, --conf );
				TRACE_MODEM( " %02X %d\n", conf, V34SpeedTbl[ conf - MODEM_V34_2400_CONF ] );
				speed = V34SpeedTbl[ conf - MODEM_V34_2400_CONF ];
			}
			else
				speed = 0;
		}
	}
	else
	{
		if ( ModemControl->ConnectedSpeed
		  == V34SpeedTbl[ conf - MODEM_V34_2400_CONF ] )
		{
			if ( conf > next )
			{
				MDM_WRITE( MODEM_CONF, --conf );
				TRACE_MODEM( " %02X %d\n", conf, V34SpeedTbl[ conf - MODEM_V34_2400_CONF ] );
				speed = V34SpeedTbl[ conf - MODEM_V34_2400_CONF ];
			}
		}
	}

	return speed;
#else
	UINT8 conf = 0;

	conf = MDM_READ( MODEM_CONF );

	if ( ModemControl->ConnectedSpeed == V34SpeedTbl[ conf - MODEM_V34_2400_CONF ] )
	{
		if ( conf > MODEM_V34_2400_CONF )
		{
			MDM_WRITE( MODEM_CONF, --conf );

			TRACE_MODEM( "next speed conf [ %02X ]\n", conf );
			TRACE_MODEM( " %d -> %d bps\n", ModemControl->ConnectedSpeed, V34SpeedTbl[ conf - MODEM_V34_2400_CONF ] );

			return V34SpeedTbl[ conf - MODEM_V34_2400_CONF ];
		}
	}

	TRACE_MODEM( "no change speed\n" );
	return 0;
#endif
}


/*=======================================================================
*    ModemV34CheckCC
*       Check if modem is in control channel mode
*
*      Input:          (none)
*      Output:         TRUE if yes otherwise FALSE
*=======================================================================*/
UINT8 ModemV34CheckCC( void )
{
    if ( MDM_READ( MODEM_RXHSKSTAT ) == CC_DATA_MODE )
        return TRUE;
    else
        return FALSE;
}


void ModemConfigGPIO( UINT8 port, UINT8 out )
{
    UINT8 cdata, cmask;

    if ( port == 5 )
    {
        cdata = SSFReadLSD( 0x0F ) & ~0x02;

        if ( out )
            cdata |= 0x02;
        else
            cdata &= ~0x02;

        SSFWriteLSD( 0x0F, cdata );

        ModemStoreGPO();
    }
    else if ( port < 5 )
    {
        cdata = MDM_READ( GPIO_DIR );

        cmask = 1 << port;

        if ( out )
            cdata |= cmask;
        else
            cdata &= ~cmask;

        MDM_WRITE( GPIO_DIR, cdata );
        ModemStoreGPO();

    }
}


void ModemSetGPIO( UINT8 port, UINT8 out )
{
    UINT8 cdata, cmask;

    if ( port == 5 )
    {
        cdata = SSFReadLSD( 0x0F ) & ~0x01;

        if ( out )
            cdata |= 0x01;
        else
            cdata &= ~0x01;

        SSFWriteLSD( 0x0F, cdata );
        ModemStoreGPO();
    }
    else if ( port < 5 )
    {
        cdata = MDM_READ( GPO_DATA );
        cmask = 1 << port;

        if ( out )
            cdata |= cmask;
        else
            cdata &= ~cmask;

        MDM_WRITE( GPO_DATA, cdata );
        ModemStoreGPO();
    }
}


UINT8 ModemGetGPIO( UINT8 port )
{
    UINT8 value = 0;

    if ( port == 5 )
    {
        value = SSFReadLSD( 0x0F ) & 0x04;
    }
    else if ( port < 5 )
    {
        value = MDM_READ( GPI_DATA ) & ( 1 << port );
    }

    return value;
}


void ModemExclusionRelay( BOOL onoff )
{
    if ( onoff )
        ModemSetGPIO( EX_RELAY_PORT, ModemGetGPIO( EX_RELAY_PORT ) |  EX_RELAY_MASK );
    else
        ModemSetGPIO( EX_RELAY_PORT, ModemGetGPIO( EX_RELAY_PORT ) & ~EX_RELAY_MASK );
}


void ModemEnableToneInterrupt( MODEM_TONE_SELECT tone, BOOL enable )
{
    UINT8 InterruptMask = 0;

    switch ( tone )
    {
        case MODEM_DIAL_TONE:
            InterruptMask = TONEA;
            break;
        case MODEM_CNG_TONE:
            InterruptMask = TONEB;
            break;
        case MODEM_BUSY_TONE:
            InterruptMask = TONEC;
            break;
        default:
            TRACE_ERROR( "### ModemEnableToneInterrupt() unknown tone %d ###\n", tone );
            break;
    }

    if ( enable )
    {
        /* Enable interrupts on TONE detection */
        ModemDSPWrite1( MODEM_NEWS_MASK_MEM_ACCESS,
                        ModemDSPRead1( MODEM_NEWS_MASK_MEM_ACCESS ) & ~MODEM_MEM_ACCESS_DISABLE, FALSE );

        TRACE_MODEM( "Enable: MODEM_NEWS_MASK_0B(Before) = 0x%x\n", ModemDSPRead1( MODEM_NEWS_MASK_0B ) );

        ModemDSPWrite1( MODEM_NEWS_MASK_0B, InterruptMask, TRUE );

        TRACE_MODEM( "Enable: MODEM_NEWS_MASK_0B(After) = 0x%x\n", ModemDSPRead1( MODEM_NEWS_MASK_0B ) );

        MDM_SETBIT( MODEM_1F, NSIE );     // enable NEWS IRQ

        TRACE_MODEM( "ModemEnableToneInterrupt() Start IRQ\n" );
    }
    else
    {
        /* Disable interrupts on TONE detection */
        /*
        TRACE_MODEM("Disable: MODEM_NEWS_MASK_0B(Before) = 0x%x\n", ModemDSPRead1(MODEM_NEWS_MASK_0B));
        ModemDSPWrite1(MODEM_NEWS_MASK_0B, 0, FALSE);
        TRACE_MODEM("Disable: MODEM_NEWS_MASK_0B(After) = 0x%x\n", ModemDSPRead1(MODEM_NEWS_MASK_0B));
        ModemDSPWrite1(MODEM_MEM_ACCESS_DISABLE,0,FALSE);
        */
    }

}


UINT8 ModemTxWaitForFIFOEmpty( UINT8 efifo )
{
    UINT8 in, out;

    if ( efifo )
    {
        OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

        in = ModemDSPRead1( MODEM_TX_FIFO_IN_POINTER );
        out = ModemDSPRead1( MODEM_TX_FIFO_OUT_POINTER );

        OSC_Release_Mutex( &modem_dsp_mutex );

        TRACE_DEBUG( "ModemTxWaitForFIFOEmpty: in %02X, out %02X\n", in, out );
        return ( in == out );
    }
    else
        return ( MDM_BITSET( MODEM_1E, TDBE ) );
}


void ModemTxSwitchPCtoCC( void )
{
    int ie;

    if ( ModemControl->Status & MODEM_ABORT )
    {
        MDM_CLEARBIT( MODEM_08, RTS );
        MDM_CLEARBIT( MODEM_08, ORG | DTR );
        ModemControl->State = MODEM_IDLE;
        return ;
    }

    TRACE_MODEM( "ModemTxSwitchPCtoCC\n" );

    while ( !( ModemControl->Status & MODEM_ABORT ) )
    {
        if ( ModemTxWaitForFIFOEmpty( 1 ) )
        {
            /* Wait for sending all data out */
            ie = 20 * 33600 / ModemControl->ConnectedSpeed;
            OSC_Sleep( ie );
            MDM_CLEARBIT( MODEM_08, RTS );

            MODEM_TIMER_START( TimerEvent, 6000 );
            ie = 6100 / MODEM_DELAY_TIME;  // failsafe counter

            ModemTxWaitForCC( 0 );

            while ( !( ModemControl->Status & MODEM_ABORT ) )
            {
                OSC_Sleep( MODEM_DELAY_TIME );

                if ( ModemTxWaitForCC( 1 ) == CC_DATA_MODE )
                {
                    /* Delay for initial flag */
                    TRACE_MODEM("=========================> modem is in CC mode \n");
                    MODEM_TIMER_START( TimerEvent, 250 );
                    break;
                }

                if ( MODEM_TIMER_EXPIRE( TimerEvent )
                  || ( --ie == 0 ) )
                    break;
            }

            TRACE_MODEM("=========================> modem State MODEM_IDLE ... %d\n", ie);
            ModemControl->State = MODEM_IDLE;
            break;
        }

        OSC_Sleep( MODEM_DELAY_TIME );
    }
}


UINT8 ModemTxWaitForCC( UINT8 ctrl )
{
    UINT8  rx_state, tx_state;
    static UINT8 _prev_rx_state, _prev_tx_state;
    static int _time_count1;

    if ( ctrl == 0 )
    {
        TRACE_MODEM( "ModemTxWaitForCC Start\n" );

        _time_count1 = 0;
        _prev_rx_state = 0;
        _prev_tx_state = 0;
        return 0;
    }

    _time_count1++;

    rx_state = MDM_READ( MODEM_RXHSKSTAT );
    tx_state = MDM_READ( MODEM_TXHSKSTAT );

    if ( ( _prev_rx_state != rx_state ) )
        TRACE_MODEM( " rx (%02X -> %02X)\n", _prev_rx_state, rx_state );
    if ( ( _prev_tx_state != tx_state ) )
        TRACE_MODEM( " tx (%02X -> %02X)\n", _prev_tx_state, tx_state );

#ifdef OEM_FAX_REPORT
    if ( _prev_rx_state != rx_state )
    {
        ModemControl->Level = 0; //ModemReadTxLevel();
        ModemControl->EQM   = 0xffff;

        switch ( rx_state )
        {
            case 0x87:

                CxProtocolReport( V34C1, NULL, 0, TX_LOG );
                break;

            case CC_DATA_MODE:

                CxProtocolReport( V34CC, NULL, 0, TX_LOG );
                break;

            default:
                break;
        }
    }
#endif

    _prev_rx_state = rx_state;
    _prev_tx_state = tx_state;

    if ( ( rx_state == CC_DATA_MODE ) && ( tx_state == CC_DATA_MODE ) )
    {
        TRACE_MODEM( "ModemTxWaitForCC Swiched to CC\n" );

        ModemControl->HighSpeed = FALSE;
        return rx_state;
    }
    else if ( ( ( rx_state < CC_DATA_MODE ) && ( rx_state > CC_INIT ) )
           && ( ( tx_state < CC_DATA_MODE ) && ( tx_state > CC_INIT ) ) )
    {
#if 0
        if ( _time_count1 == ( 3000 / MODEM_DELAY_TIME ) )
        {
            MDM_SETBIT( MODEM_05, CCRTN );

            TRACE_MODEM( " CC Retrain (%02X %02X)\n", tx_state, rx_state );
        }
#endif
        ModemControl->HighSpeed = FALSE;
        return rx_state;
    }

    return 0;
}


UINT8 ModemRxSwitchPCtoCC( void )
{
    int ie;

    TRACE_MODEM( "ModemRxSwitchPCtoCC\n" );

    if ( ModemControl->Status & MODEM_ABORT )
    {
        MDM_CLEARBIT( MODEM_08, RTS );
        MDM_CLEARBIT( MODEM_08, ORG | DTR );
        ModemControl->State = MODEM_IDLE;
        return DISCONNECT;
    }

    MODEM_TIMER_START( TimerEvent, 6000 );
    ie = 6100 / MODEM_DELAY_TIME;  // failsafe counter

    if ( ModemRxWaitForCC( 0 ) == CC_DATA_MODE )
    {
        return 0;
    }

    while ( !( ModemControl->Status & MODEM_ABORT ) )
    {
        OSC_Sleep( MODEM_DELAY_TIME );

        if ( ModemRxWaitForCC( 1 ) == CC_DATA_MODE )
        {
            return 0;
        }

        if ( MODEM_TIMER_EXPIRE( TimerEvent )
          || ( --ie == 0 ) )
            return TIME_OUT;
    }

    ModemControl->State = MODEM_IDLE;
    return DISCONNECT;
}


UINT8 ModemRxWaitForCC( UINT8 ctrl )
{
    UINT8  rx_state, tx_state;
    static UINT8 _prev_rx_state, _prev_tx_state;
    static int _time_count2;

    if ( ctrl == 0 )
    {
        TRACE_MODEM( "ModemRxWaitForCC Start\n" );

        _time_count2 = 0;
        _prev_rx_state = 0;
        _prev_tx_state = 0;

        rx_state = MDM_READ( MODEM_RXHSKSTAT );

        if ( rx_state == CC_DATA_MODE )
            return rx_state;
        else
            return 0;
    }

    _time_count2++;

    rx_state = MDM_READ( MODEM_RXHSKSTAT );
    tx_state = MDM_READ( MODEM_TXHSKSTAT );

    if ( _prev_rx_state != rx_state )
        TRACE_MODEM( " rx (%02X -> %02X)\n", _prev_rx_state, rx_state );
    if ( _prev_tx_state != tx_state )
        TRACE_MODEM( " tx (%02X -> %02X)\n", _prev_tx_state, tx_state );

#ifdef OEM_FAX_REPORT
    if ( _prev_rx_state != rx_state )
    {
        switch ( rx_state )
        {
            case 0x87:

                ModemControl->Level = 0; //ModemReadRxLevel();
                ModemControl->EQM   = 0xffff;
                CxProtocolReport( V34C1, NULL, 0, RX_LOG );
                break;

            case CC_DATA_MODE:

                ModemControl->Level = 0; //ModemReadRxLevel();
                ModemControl->EQM   = 0xffff; //ModemReadEQM();
                CxProtocolReport( V34CC, NULL, 0, RX_LOG );
                break;

            default:
                break;
        }
    }
#endif

    _prev_rx_state = rx_state;
    _prev_tx_state = tx_state;

    if ( ( rx_state == CC_DATA_MODE ) && ( tx_state == CC_DATA_MODE ) )
    {
        TRACE_MODEM( "ModemRxWaitForCC Swiched to CC\n" );

        ModemControl->HighSpeed = FALSE;
        return rx_state;
    }
    else if ( ( rx_state < CC_DATA_MODE ) && ( rx_state > CC_INIT ) )
    {
        ModemControl->HighSpeed = FALSE;
        return rx_state;
    }

    return 0;
}


UINT8 ModemRxWaitForPC( UINT8 ctrl )
{
    UINT8  rx_state;
    static UINT8 _prev_rx_state;
    static int _time_count3;

    if ( ctrl == 0 )
    {
        TRACE_MODEM( "ModemRxWaitForPC Start\n" );

        _time_count3 = 0;
        _prev_rx_state = 0;
        return 0;
    }

    _time_count3++;

    rx_state = MDM_READ( MODEM_RXHSKSTAT );

    if ( _prev_rx_state != rx_state )
        TRACE_MODEM( " (%02X -> %02X)\n", _prev_rx_state, rx_state );

#ifdef OEM_FAX_REPORT
    if ( _prev_rx_state != rx_state )
    {
        switch ( rx_state )
        {
            case 0xA1:

                ModemControl->Level = 0; //ModemReadRxLevel();
                ModemControl->EQM   = 0xffff;
                CxProtocolReport( V34P1, NULL, 0, RX_LOG );
                break;

            case PC_RX_DATA_MODE:

                ModemControl->Level = 0; //ModemReadRxLevel();
                ModemControl->EQM   = 0xffff; //ModemReadEQM();
                CxProtocolReport( V34PC, NULL, 0, RX_LOG );
                break;

            default:
                break;
        }
    }
#endif

    _prev_rx_state = rx_state;

    if ( rx_state == PC_RX_DATA_MODE )
    {
        TRACE_MODEM( "ModemRxWaitForPC Switched to PC\n" );

        ModemConnectedSpeed(0);

        ModemControl->HighSpeed = TRUE;
        return rx_state;
    }
    else if ( rx_state >= 0xA1 )
    {
        return rx_state;
    }
    else if ( rx_state == CC_DATA_MODE )
    {
        if ( _time_count3 > ( 7000 / MODEM_DELAY_TIME ) )
        {
            TRACE_MODEM( "ModemRxWaitForPC still in CC !!!\n" );

            return rx_state;
        }
        return 0;
    }

    if ( ( ( rx_state == CC_INIT ) || ( rx_state == PC_INIT ) )
      && ( ( _time_count3 % ( 1000 / MODEM_DELAY_TIME ) ) == 0 ) )
    {
        ModemDSPWrite2( 0x274, 0x273, 0x0001 );

        TRACE_DEBUG( "ModemRxWaitForPC S,/S workaround\n" );
    }

    return 0;
}


BOOL ModemBitOrBust( UINT8 reg, UINT8 bit, BOOL change )
{
    UINT8 cnt = 0;

    while ( 1 )
    {
        modem_delay_usec( 100 );

        if ( ++cnt < 255 )
        {
            if ( change )
            {
                if ( MDM_BITSET( reg, bit ) )
                {
                    return FALSE;
                }
            }
            else
            {
                if ( MDM_BITCLEAR( reg, bit ) )
                {
                    return FALSE;
                }
            }

            if ( reg != 0x1D )
            {
                if ( ( cnt > 2 ) && ( cnt % 2 == 1 ) )
                {
                    modem_skip_task();
                    phone_skip_task();
                    OSC_Sleep( 1 );
                    phone_continue_task();
                    modem_continue_task();
                }
            }
        }
        else
        {
            TRACE_ERROR( "ModemBitOrBust Reg%02X Bit%02X overtime ###\n", reg, bit );
            return TRUE;
        }
    }
}


void ModemShowRAMVersion( void )
{
    TRACE_MODEM( "CPX %X, SPX %lX\n",
                 ModemDSPRead6( 0x1003, 0x1004 ),
                 ModemDSPRead7( 0xE003, 0xE002 ) );
}


//===================================================================
//  Modem I/O Access
//===================================================================
extern UINT8 modem_spi_read( UINT8 addr );
extern void  modem_spi_write( UINT8 addr, UINT8 data );
extern void  modem_spi_multi_read( UINT8 addr, UINT8 *data, UINT8 cnt );
extern void  modem_spi_multi_write( UINT8 addr, UINT8 *data, UINT8 cnt );
extern OSC_MUTEX mdm_spi_mutex;

UINT8 spi_access   = 0;
static UINT8 single_read  = 0;
static UINT8 single_write = 0;
static UINT8 multi_read   = 0;
static UINT8 multi_write  = 0;

static UINT8 mdm_spi_read( UINT8 addr )
{
    UINT8 val;

    if ( spi_access )
    {
        //TRACE_DEBUG(" ### *********************************** ###\n");
        //TRACE_DEBUG(" ### *     mdm_spi_read wait mutex     * ###\n");
        //TRACE_DEBUG(" ### *     sr:%d sw:%d mr:%d mw:%d         * ###\n", single_read, single_write, multi_read, multi_write );
        //TRACE_DEBUG(" ### *********************************** ###\n");
    }
    else
    {
        spi_access = 1;
    }

    OSC_Obtain_Mutex( &mdm_spi_mutex, OSC_SUSPEND );

    single_read = 1;

    val = modem_spi_read( addr );

    single_read = 0;

    OSC_Release_Mutex( &mdm_spi_mutex );

    spi_access = 0;

    return ( val );
}

static void mdm_spi_write( UINT8 addr, UINT8 data )
{
    if ( spi_access )
    {
        //TRACE_DEBUG(" ### ************************************ ###\n");
        //TRACE_DEBUG(" ### *     mdm_spi_write wait mutex     * ###\n");
        //TRACE_DEBUG(" ### *     sr:%d sw:%d mr:%d mw:%d          * ###\n", single_read, single_write, multi_read, multi_write );
        //TRACE_DEBUG(" ### ************************************ ###\n");
    }
    else
    {
        spi_access = 1;
    }

    OSC_Obtain_Mutex( &mdm_spi_mutex, OSC_SUSPEND );

    single_write = 1;

    modem_spi_write( addr, data );

    single_write = 0;

    OSC_Release_Mutex( &mdm_spi_mutex );

    spi_access = 0;
}


#ifdef SPI_MULTI_DATA
void mdm_spi_multi_read( UINT8 addr, UINT8 *data, UINT8 cnt )
{
    if ( spi_access )
    {
        //TRACE_DEBUG(" ### ***************************************** ###\n");
        //TRACE_DEBUG(" ### *     mdm_spi_multi_read wait mutex     * ###\n");
        //TRACE_DEBUG(" ### *     sr:%d sw:%d mr:%d mw:%d               * ###\n", single_read, single_write, multi_read, multi_write );
        //TRACE_DEBUG(" ### ***************************************** ###\n");
    }
    else
    {
        spi_access = 1;
    }

    OSC_Obtain_Mutex( &mdm_spi_mutex, OSC_SUSPEND );

    multi_read = 1;

    modem_spi_multi_read( addr, data, cnt );

    multi_read = 0;

    OSC_Release_Mutex( &mdm_spi_mutex );

    spi_access = 0;
}


void mdm_spi_multi_write( UINT8 addr, UINT8 *data, UINT8 cnt )
{
    if ( spi_access )
    {
        //TRACE_DEBUG(" ### ****************************************** ###\n");
        //TRACE_DEBUG(" ### *     mdm_spi_multi_write wait mutex     * ###\n");
        //TRACE_DEBUG(" ### *     sr:%d sw:%d mr:%d mw:%d                * ###\n", single_read, single_write, multi_read, multi_write );
        //TRACE_DEBUG(" ### ****************************************** ###\n");
    }
    else
    {
        spi_access = 1;
    }

    OSC_Obtain_Mutex( &mdm_spi_mutex, OSC_SUSPEND );

    multi_write = 1;

    modem_spi_multi_write( addr, data, cnt );

    OSC_Release_Mutex( &mdm_spi_mutex );

    multi_write = 0;

    spi_access = 0;
}
#endif


UINT8 ModemIoReadByte( UINT8 addr )
{

    return mdm_spi_read( addr );
}


void ModemIoWriteByte( UINT8 addr, UINT8 byte )
{

    mdm_spi_write( addr, byte );
}


BOOL ModemIoBitSet( UINT8 addr, UINT8 bits )
{
    UINT8 data;

    data = mdm_spi_read( addr );
    return ( data & bits );
}


BOOL ModemIoBitClear( UINT8 addr, UINT8 bits )
{
    UINT8 data;

    data = mdm_spi_read( addr );
    return !( data & bits );
}


void ModemIoSetBit( UINT8 addr, UINT8 bits )
{
    UINT8 data;

    data = mdm_spi_read( addr );
    data = data | bits;
    mdm_spi_write( addr, data );
}


void ModemIoClearBit( UINT8 addr, UINT8 bits )
{
    UINT8 data;

    data = mdm_spi_read( addr );
    data = data & ~bits;
    mdm_spi_write( addr, data );
}


void ModemTimerStart( UINT32 *timer )
{
    *timer = tx_time_get();
}


/***********************************************************************
 *  Function: ModemTimerExpired
 *
 *  Description:
 *		check timer is expired
 * Input:
 *		start_time: value of start time, interval
 *	Output:
 *		TRUE: expired, FALSE: not yet
 */
BOOL ModemTimerExpired( UINT32 start_time, UINT32 interval )
{
    UINT32 curr_time;

    curr_time = tx_time_get();

    if ( curr_time >= start_time )
    {
        if ( ( curr_time - start_time ) >= interval )
            return TRUE;
    }
    else
    {
        if ( ( curr_time + ~start_time + 1 ) >= interval )
            return TRUE;
    }

    return FALSE;
}


void modem_timer_start( void* tmr, UINT32 msec )
{
    T_MODEM_TIMER* sTim;

    sTim = ( T_MODEM_TIMER * )tmr;
    sTim->start_time = tx_time_get();
    sTim->interval = msec;
}


BOOL modem_timer_expire( void* tmr )
{
    T_MODEM_TIMER* sTim;
    UINT32 curr_time;

    sTim = ( T_MODEM_TIMER * )tmr;
    curr_time = tx_time_get();

    if ( curr_time >= sTim->start_time )
    {
        if ( ( curr_time - sTim->start_time ) >= sTim->interval )
        {
            return TRUE;
        }
    }
    else
    {
        if ( ( curr_time + ~sTim->start_time + 1 ) >= sTim->interval )
        {
            return TRUE;
        }
    }

    return FALSE;
}

UINT32 ModemElapsedTime( UINT32 start_time )
{
    UINT32 curr_time;

    curr_time = tx_time_get();

    if ( start_time )
    {
        if ( curr_time < start_time )
        {
            curr_time += ~start_time;
        }
        else
        {
            curr_time -= start_time;
        }
    }

    return curr_time;
}

