/*
 * ============================================================================
 * Copyright (c) 2007-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanalyzer.c
 *
 * \brief A debugging tool to log events and data to memory.
 *
 **/
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "scos.h"

#include "memAPI.h"
#include "lassert.h"       // XASSERT, atsystemhalt
#include "utils.h"         // str_mkint
#include "debug.h"
#ifndef __KERNEL__
#include "timer_api.h"
#endif
#ifdef HAVE_CMD
    #ifdef __KERNEL__
        #include "scos_cmd_proc.h"
    #else
        #include "cmd_proc_api.h"
    #endif
#endif
#include "scancore.h"      // DBG
#include "scantypes.h"
#include "scandbg.h"       // dbg1
#include "scanalyzer.h"
#include "scanlog.h"

/* Uncomment this to dump sa logs on assert */
//#define SA_DUMP_ON_ASSERT


/* Event Buffer Size:
 *   Number of t_EventHeader entries to be statically allocated.
 */
#define BUFFER_SIZE   2048*8

/**
 *  \brief Event table entry
 *
 *  Each logged event will fill out one of these data structures,
 **/
typedef struct EventHeader 
{
    uint16_t ID;             /**< Logged event ID                      */
    uint32_t timestamp;      /**< Event timestamp (32 bit 1us counter) */
    uint32_t data;           /**< Raw event data  (32 bits)            */
} t_EventHeader;


/* Event Buffer Global Variables:
 *  An array of event structures; a circular buffer with head and tail indexes
 */
static t_EventHeader EventBuffer[BUFFER_SIZE];
static uint32_t buffer_head;
static uint32_t buffer_tail;

/* Global event log pause state, allow enabled logs by default */
static t_PauseType sa_pauseflag = LOG_RUN;

/* Flag indicating if scanalyzer has been initialized */
static bool sa_init = false;

/* Configuration for the events that will be logged. One byte per event:
 *   BIT 0       the event enable bit
 *   BIT 1 - 7   unused
 */
static uint8_t event_config[NUM_LOG];

/* Configuration bit values */
#define ENABLE_BIT    0x01

/* davep 06-Dec-2012 ; porting to linux kernel */
#ifdef __KERNEL__
DEFINE_SPINLOCK(scanalyzer_lock);
#endif

/*
 *  Local (static) function prototypes
 */
#ifdef HAVE_CMD
static const char *scanalyzer_cmd_enable_desc;
static const char *scanalyzer_cmd_enable_usage;
static const char *scanalyzer_cmd_enable_notes;
static const char *scanalyzer_cmd_disable_desc;
static const char *scanalyzer_cmd_disable_usage;
static const char *scanalyzer_cmd_disable_notes;
static const char *scanalyzer_cmd_pause_desc;
static const char *scanalyzer_cmd_pause_usage;
static const char *scanalyzer_cmd_pause_notes;
static const char *scanalyzer_cmd_run_desc;
static const char *scanalyzer_cmd_run_usage;
static const char *scanalyzer_cmd_run_notes;
static const char *scanalyzer_cmd_dump_desc;
static const char *scanalyzer_cmd_dump_usage;
static const char *scanalyzer_cmd_dump_notes;
static const char *scanalyzer_cmd_clear_desc;
static const char *scanalyzer_cmd_clear_usage;
static const char *scanalyzer_cmd_clear_notes;
static const char *scanalyzer_cmd_stat_desc;
static const char *scanalyzer_cmd_stat_usage;
static const char *scanalyzer_cmd_stat_notes;

static int scanalyzer_cmd_enable_cb( int argc, char *argv[] );
static int scanalyzer_cmd_disable_cb( int argc, char *argv[] );
static int scanalyzer_cmd_pause_cb( int argc, char *argv[] );
static int scanalyzer_cmd_run_cb( int argc, char *argv[] );
static int scanalyzer_cmd_dump_cb( int argc, char *argv[] );
static int scanalyzer_cmd_clear_cb( int argc, char *argv[] );
static int scanalyzer_cmd_stat_cb( int argc, char *argv[] );
#endif


/**
 *  \brief (API) Initialize scanalyzer
 *
 *  API function: please see documentation in scanalyzer.h
 **/
void scanalyzer_init() 
{
    int retcode;

    /* Once is enough */
    XASSERT( sa_init==false, sa_init );

    /* Init the head/tail buffer pointers */
    buffer_head = 0;
    buffer_tail = 0;

    retcode = 0;

    /* Register 'sa' command line command for scanalyzer */
#ifdef HAVE_CMD
    retcode = cmd_register_cmd( "sa",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "sa",
                                   "enable",
                                   scanalyzer_cmd_enable_desc,
                                   scanalyzer_cmd_enable_usage,
                                   scanalyzer_cmd_enable_notes,
                                   scanalyzer_cmd_enable_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "sa",
                                   "disable",
                                   scanalyzer_cmd_disable_desc,
                                   scanalyzer_cmd_disable_usage,
                                   scanalyzer_cmd_disable_notes,
                                   scanalyzer_cmd_disable_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "sa",
                                   "pause",
                                   scanalyzer_cmd_pause_desc,
                                   scanalyzer_cmd_pause_usage,
                                   scanalyzer_cmd_pause_notes,
                                   scanalyzer_cmd_pause_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "sa",
                                   "run",
                                   scanalyzer_cmd_run_desc,
                                   scanalyzer_cmd_run_usage,
                                   scanalyzer_cmd_run_notes,
                                   scanalyzer_cmd_run_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "sa",
                                   "dump",
                                   scanalyzer_cmd_dump_desc,
                                   scanalyzer_cmd_dump_usage,
                                   scanalyzer_cmd_dump_notes,
                                   scanalyzer_cmd_dump_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "sa",
                                   "clear",
                                   scanalyzer_cmd_clear_desc,
                                   scanalyzer_cmd_clear_usage,
                                   scanalyzer_cmd_clear_notes,
                                   scanalyzer_cmd_clear_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "sa",
                                   "stat",
                                   scanalyzer_cmd_stat_desc,
                                   scanalyzer_cmd_stat_usage,
                                   scanalyzer_cmd_stat_notes,
                                   scanalyzer_cmd_stat_cb );
    XASSERT( retcode==CMD_OK, retcode );
#endif

    /* Clear all the event flags (disables all the event logs) */
    memset(event_config, 0, sizeof(event_config));

    /* Allow enabled logs by default (which is none right now) */
    sa_pauseflag = LOG_RUN;

    /* This function registers the dump function to be executed when we assert */
#ifdef SA_DUMP_ON_ASSERT
    atassert( scanalyzer_dump );
#endif

    /* Ready to log events */
    sa_init = true;
    dbg1("Scanalyzer installed, buffer entries=%d\n", BUFFER_SIZE);
}   


/**
 *  \brief (API) Enable a scanalyzer event
 *
 *  API function: please see documentation in scanalyzer.h
 **/
void scanalyzer_enable_log( uint32_t log_id) 
{
    /* The scanalyzer needs to be initialized first */
    XASSERT( sa_init==true, sa_init );
    XASSERT( log_id < NUM_LOG, log_id );

    /* Set the enable bit for the specified event */
    event_config[log_id] |= ENABLE_BIT;
}


/**
 *  \brief (API) Disable a scanalyzer event
 *
 *  API function: please see documentation in scanalyzer.h
 **/
void scanalyzer_disable_log( uint32_t log_id) 
{
    /* The scanalyzer needs to be initialized first */
    XASSERT( sa_init==true, sa_init );
    XASSERT( log_id < NUM_LOG, log_id );

    /* Clear the enable bit for the specified event */
    event_config[log_id] &= ~ENABLE_BIT;
}


/**
 *  \brief (API) Disable all scanalyzer events
 *
 *  API function: please see documentation in scanalyzer.h
 **/
void scanalyzer_disable_all() 
{
    int i;

    /* The scanalyzer needs to be initialized first */
    XASSERT( sa_init==true, sa_init );

    for (i=0; i < NUM_LOG; i++) 
    {
        event_config[i] &= ~ENABLE_BIT;
    }
}


/**
 *  \brief (API) Log a scanalyzer event
 *
 *  API function: please see documentation in scanalyzer.h
 **/

#ifdef __KERNEL__
void scanalyzer_log( uint32_t log_id, uint32_t data) 
{
    unsigned long flags;
    struct timespec tv;

    /* THIS CAN BE CALLED IN AN INTERRUPT CONTEXT */
//    TX_INTERRUPT_SAVE_AREA

    /* The scanalyzer needs to be initialized first */
    XASSERT( sa_init==true, sa_init );
    XASSERT( log_id < NUM_LOG, log_id );

    /* Don't log anything if in paused state */
    if (sa_pauseflag == LOG_PAUSE_ALL)
    {
        return;
    }

    /* Check to see if event is enabled */
    if (event_config[log_id] & ENABLE_BIT)
    {
        /* Disable all interrupts */
//        TX_DISABLE;
        spin_lock_irqsave( &scanalyzer_lock, flags );

        /* Store event in buffer */
        EventBuffer[buffer_tail].ID = log_id;
        getnstimeofday(&tv);
        EventBuffer[buffer_tail].timestamp = tv.tv_nsec;
//        EventBuffer[buffer_tail].timestamp = jiffies;
        EventBuffer[buffer_tail].data = data;

        /* Move tail and check to see if we overrun the buffer */
        if (++buffer_tail == BUFFER_SIZE) {
            buffer_tail = 0;
        }

        /* If the tail has reached the head, need to move the head */
        if (buffer_tail == buffer_head) {
            if(++buffer_head == BUFFER_SIZE) {
                buffer_head = 0;
            }
        }

        /* enable inerrupts */
//        TX_RESTORE;
        spin_unlock_irqrestore( &scanalyzer_lock, flags );
    }
}
#else
void scanalyzer_log( uint32_t log_id, uint32_t data) 
{
    /* THIS CAN BE CALLED IN AN INTERRUPT CONTEXT */
    TX_INTERRUPT_SAVE_AREA

    /* The scanalyzer needs to be initialized first */
    XASSERT( sa_init==true, sa_init );
    XASSERT( log_id < NUM_LOG, log_id );

    /* Don't log anything if in paused state */
    if (sa_pauseflag == LOG_PAUSE_ALL)
    {
        return;
    }

    /* Check to see if event is enabled */
    if (event_config[log_id] & ENABLE_BIT)
    {
        /* Disable all interrupts */
        TX_DISABLE;

        /* Store event in buffer */
        EventBuffer[buffer_tail].ID = log_id;
        EventBuffer[buffer_tail].timestamp = timer_get_time_usec();
        EventBuffer[buffer_tail].data = data;

        /* Move tail and check to see if we overrun the buffer */
        if (++buffer_tail == BUFFER_SIZE) {
            buffer_tail = 0;
        }

        /* If the tail has reached the head, need to move the head */
        if (buffer_tail == buffer_head) {
            if(++buffer_head == BUFFER_SIZE) {
                buffer_head = 0;
            }
        }

        /* enable inerrupts */
        TX_RESTORE;
    }
}
#endif

/**
 *  \brief (API) Clear the scanalyzer logs
 *
 *  API function: please see documentation in scanalyzer.h
 **/
void scanalyzer_clear_log(void) 
{
    /* The scanalyzer needs to be initialized first */
    XASSERT( sa_init==true, sa_init );

    /* Don't clear if in paused state, someone is trying to save a
     * logged event state
     */
    if (sa_pauseflag == LOG_PAUSE_ALL)
    {
        dbg1("(SA logs not cleared, in paused state)\n");
        return;
    }

    /* Flush it */
    buffer_head = buffer_tail = 0;
}


/**
 *  \brief (API) Dump the scanalyzer logs
 *
 *  API function: please see documentation in scanalyzer.h
 **/
void scanalyzer_dump() 
{
    uint32_t i;

    /* The scanalyzer needs to be initialized first */
    XASSERT( sa_init==true, sa_init );

    /* NOTE: should be using dbg_printf to get sa messages out in release
     * builds 
     */
    dbg1( "Scanalyzer Dump: %d, %d\n", sizeof(EventBuffer[0]), buffer_tail );

    /* Make sure head and tail are valid */
    if (!(buffer_head < BUFFER_SIZE && buffer_tail < BUFFER_SIZE)) 
    {
        return;
    }

    /* Don't print if there is no logged data */
    if (buffer_head == 0 && buffer_tail == 0) 
    {
        return;
    }

    /* Start at the head and write the log data until tail is reached */
    i = buffer_head;
    while (EventBuffer[i].ID != LOG_NULL) 
    {
        dbg1( "%d,%lu,%ld\n", EventBuffer[i].ID, EventBuffer[i].timestamp, EventBuffer[i].data );

        if (++i == BUFFER_SIZE) {
            i = 0;    /* wrap around */
        }
    
        if (i == buffer_tail) {
            break;    /* tail reached, break */
        }
    }
}


/**
 *  \brief (API) Set the logs pause state
 *
 *  API function: please see documentation in scanalyzer.h
 **/
void scanalyzer_pause_log(t_PauseType pause)
{
    /* The scanalyzer needs to be initialized first */
    XASSERT( sa_init==true, sa_init );

    switch(pause)
    {
        case LOG_PAUSE_ALL:
            sa_pauseflag = LOG_PAUSE_ALL;
            break;
            
        case LOG_RUN:
            sa_pauseflag = LOG_RUN;
            break;
            
        default:
            XASSERT(0, pause);
            break;
    }
}


#ifdef HAVE_CMD

static void scanalyzer_cmd_dump_args( int argc, char *argv[] )
{
    int i;
    /* act like scan log 2, only we're using cmd_printf */
    if ( 2 > scanlog_get_level() )
    {
        return;
    }

    /* Echo command args back */
    for( i=0 ; i<argc ; i++ ) 
    {
        cmd_printf( "%d %s\n", i, argv[i] );
    }
}

static const char *scanalyzer_cmd_enable_desc = "enable log n";
static const char *scanalyzer_cmd_enable_usage = "<n>";
static const char *scanalyzer_cmd_enable_notes = "Add multiple values to enable multiple logs";
static int scanalyzer_cmd_enable_cb( int argc, char *argv[] )
{
    uint32_t num32;
    uint32_t i;
    scanalyzer_cmd_dump_args( argc, argv );
    if ( argc < 2 )
    {
        return CMD_USAGE_ERROR;
    }

    for(i = 1; i < argc; i++)
    {
        if( str_mkint( argv[i], &num32 ) != 0 )
        {
            return CMD_USAGE_ERROR;
        }
        else
        {
            SCANALYZER_ENABLE_LOG(num32);
            cmd_printf("SA log %" PRIu32 " enabled\n", num32);
        }
    }

    return CMD_OK;
}

static const char *scanalyzer_cmd_disable_desc = "disable log [n|all]";
static const char *scanalyzer_cmd_disable_usage = "<n>";
static const char *scanalyzer_cmd_disable_notes = NULL;
static int scanalyzer_cmd_disable_cb( int argc, char *argv[] )
{
    uint32_t num32;
    scanalyzer_cmd_dump_args( argc, argv );
    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        if( strncmp( argv[1], "all", 8 ) == 0 )
        {
            SCANALYZER_DISABLE_ALL();
            cmd_printf("All SA logs disabled\n");
            return CMD_OK;
        }
        return CMD_USAGE_ERROR;
    }
    SCANALYZER_DISABLE_LOG(num32);
    cmd_printf("SA log %" PRIu32 " disabled\n", num32);

    return CMD_OK;
}

static const char *scanalyzer_cmd_pause_desc = "pause logging";
static const char *scanalyzer_cmd_pause_usage = NULL;
static const char *scanalyzer_cmd_pause_notes = NULL;
static int scanalyzer_cmd_pause_cb( int argc, char *argv[] )
{
    scanalyzer_cmd_dump_args( argc, argv );
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    SCANALYZER_PAUSE(LOG_PAUSE_ALL);
    cmd_printf("SA log paused\n");

    return CMD_OK;
}

static const char *scanalyzer_cmd_run_desc = "start logging";
static const char *scanalyzer_cmd_run_usage = NULL;
static const char *scanalyzer_cmd_run_notes = NULL;
static int scanalyzer_cmd_run_cb( int argc, char *argv[] )
{
    scanalyzer_cmd_dump_args( argc, argv );
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    SCANALYZER_PAUSE(LOG_RUN);
    cmd_printf("SA log running\n");

    return CMD_OK;
}

static const char *scanalyzer_cmd_dump_desc = "dump logs to serial port";
static const char *scanalyzer_cmd_dump_usage = NULL;
static const char *scanalyzer_cmd_dump_notes = NULL;
static int scanalyzer_cmd_dump_cb( int argc, char *argv[] )
{
    scanalyzer_cmd_dump_args( argc, argv );
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    SCANALYZER_DUMP();

    return CMD_OK;
}

static const char *scanalyzer_cmd_clear_desc = "clear all log data";
static const char *scanalyzer_cmd_clear_usage = NULL;
static const char *scanalyzer_cmd_clear_notes = NULL;
static int scanalyzer_cmd_clear_cb( int argc, char *argv[] )
{
    scanalyzer_cmd_dump_args( argc, argv );
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    cmd_printf("SA log clear\n");
    SCANALYZER_CLEAR_LOG();

    return CMD_OK;
}

static const char *scanalyzer_cmd_stat_desc = "show run/pause state";
static const char *scanalyzer_cmd_stat_usage = NULL;
static const char *scanalyzer_cmd_stat_notes = NULL;
static int scanalyzer_cmd_stat_cb( int argc, char *argv[] )
{
    scanalyzer_cmd_dump_args( argc, argv );
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    switch(sa_pauseflag)
    {
        case LOG_PAUSE_ALL:
            cmd_printf("SA log PAUSED\n");
            break;

        case LOG_RUN:
            cmd_printf("SA log RUNNING\n");
            break;

        default:
            XASSERT(0, sa_pauseflag);
            break;
    }

    return CMD_OK;
}


#endif /* HAVE_CMD */

