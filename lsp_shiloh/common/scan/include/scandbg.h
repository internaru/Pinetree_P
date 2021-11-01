/**
 * \file scandbg.h
 *
 * \brief Scan firmware debug macros, functions, etc.
 *
 * ============================================================================
 * Copyright (c) 2005-2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANDBG_H
#define SCANDBG_H


/* icedma.c */
/* 7-Apr-05; added ICE_DMA_LOAD_LOG; it's a global since it's needed in a
 * structure used across several files 
 */
//#define ICE_DMA_LOAD_LOG

/* A multi-level debug message macro. Normal level would be 1 but set to >1 to
 * increase the number of scanner subsystem debug messages. Created this so the
 * scan subsystem can have lots of debugging but still won't irritate other
 * developers.
 */
#ifdef DEBUG 
    #define SDBGLEVEL  2
#else
    #define SDBGLEVEL  0
#endif

/* disable until I have a chance to clean warnings from all our source */
//void scanlog( int priority, const char *format, ... ) __attribute__((format(printf, 2, 3)));
//void scanlog1( const char *format, ... ) __attribute__((format(printf, 1, 2)));
//void scanlog2( const char *format, ... ) __attribute__((format(printf, 1, 2)));
//void scanlog3( const char *format, ... ) __attribute__((format(printf, 1, 2)));
void scanlog( int priority, const char *format, ... );
void scanlog1( const char *format, ... );
void scanlog2( const char *format, ... );
void scanlog3( const char *format, ... );
void scanlog_hex_dump( unsigned char *ptr, int size );

#ifdef USE_VSCANLOG
void vscanlog1( const char *format, va_list ap );
void vscanlog2( const char *format, va_list ap );
#endif

/* printf shorthand */
#define dbg1(...) scanlog1( __VA_ARGS__ )
#define dbg2(...) scanlog2( __VA_ARGS__ )
#define dbg3(...) scanlog3( __VA_ARGS__ )

#if SDBGLEVEL <= 0 
    #undef dbg1
    #define dbg1(...)
#endif

#if SDBGLEVEL <= 1 
    #undef dbg2
    #define dbg2(...)
#endif

#if SDBGLEVEL <= 2 
    #undef dbg3
    #define dbg3(...)
#endif

/* debug messages; scan_task() interprets; put into msg.param1 */
#define SCAN_DBGMSG_DUMP_AFE   1  /* read and dump all AFE registers */
#define SCAN_DBGMSG_DUMP_SCAN  2  /* read and dump all Scan block registers */
#define SCAN_DBGMSG_PIC        3  /* debug message to PIC block module (see SCAN_DBGMSG_PIC_xxx below) */
#define SCAN_DBGMSG_NOTCH      4  /* debug message to mech code to run notchfind */
#define SCAN_DBGMSG_IP         5  /* set destination IP for network data dump */
/* 6 available */ 
#define SCAN_DBGMSG_REPOST     7  /* debug message to mech code to rerun POST (to debug hardware) */
#define SCAN_DBGMSG_SCAN       8  /* call scan test code (see SCAN_DBGMSG_SCAN_xxx below) */
#define SCAN_DBGMSG_PLAT_TEST  9  /* run a platform specific test in scanplat.c */

/* user defined debug messages start here (see scandbg_get_next_user_msgid()) */
#define SCAN_DBGMSG_USER    65536

/* Start module specific debug messages at 1000.
 *
 * Sub-debug messages; put into msg.param2 
 *
 * The naming seems
 * a bit weird but think of a namespace:
 *  scan.debug.scan
 *              test sensor
 *              finger test
 *              scanlib 
 *              (etc)
 *  scan.debug.pic
 *  scan.debug.pie
 */

/* PIC specific tests */
#define SCAN_DBGMSG_PIC_DUMP     1100  /* dump PIC registers */

/* Scan specific tests (some scan block, some scan subsystem).  */
#define SCAN_DBGMSG_TEST_SENSOR      3101  /* run scantest.c */
#define SCAN_DBGMSG_TEST_FINGER_TEST 3102  /* do "finger test" ; starts sensor and RUNS FOREVER! Blocks scantask! */
#define SCAN_DBGMSG_TEST_SCANLIB     3103  /* run scanlibtest.c */
#define SCAN_DBGMSG_TEST_SCANDS      3104  /* run scandstest.c */
#define SCAN_DBGMSG_TEST_TASK        3105  /* run scantasktest.c */
#define SCAN_DBGMSG_TEST_PIC         3106  /* run PIC test code */
#define SCAN_DBGMSG_TEST_SCANVARS    3107  /* run scanvartest.c */
#define SCAN_DBGMSG_TEST_CISX        3108  /* run cisxtest.c */
#define SCAN_DBGMSG_TEST_ICETEST     3109  /* run icetesttest.c */
#define SCAN_DBGMSG_TEST_PIE         3110  /* run PIE test code */
#define SCAN_DBGMSG_TEST_AFE         3111  /* run afe_test() */
/* TODO add more later, as needed */

struct scan_reg {
    const char *name;
    volatile uint32_t *addr;
};

/* 
 * Function Prototypes. 
 */

/* scanplat.c */
void scanplat_run_test(uint32_t param2, void *param3);


/* scandbg.c */
typedef scan_err_t (*scandbg_callback_fn)( uint32_t param1, uint32_t param2, void *param3 );

scan_err_t scandbg_onetime_init( void );
void scan_debug_dumpregs( struct scan_reg regs[], int num_regs );
void scan_do_debug_msg( uint32_t param1, uint32_t param2, void *param3 );
uint32_t scandbg_get_next_user_msgid( void );
scan_err_t scandbg_register_user_debug( uint32_t id, scandbg_callback_fn fn );

/* pic.c */
int pic_debug_msg( uint32_t param2, void *param3 );

#ifdef HAVE_NETWORK
/* scannet.c - dump scan results over network */
char *ipstr( uint32_t s_addr, char buf[], int bufsize );
int mkip( char *str, uint32_t *ip );
void report_net_err( const char *msg, int sock, const char *fct );
void scan_debug_net_close( void );
void scan_debug_set_ip( uint32_t destip );
uint32_t scan_debug_get_ip( void );
void scan_debug_clear_ip( void );
int scan_debug_net_open( uint32_t destip, uint8_t channels[], int num_channels );
int scan_debug_net_send( uint8_t channel, uint8_t *ptr, uint32_t len );
bool scan_debug_net_is_open( uint8_t channel );
#else
/* Replace functions with success codes; exception is net_is_open which should
 * report the network is NOT open.
 */
static inline int scan_debug_net_open( uint32_t destip, uint8_t channels[], int num_channels ) { return -1; }
static inline void scan_debug_net_close(void) {}
static inline int scan_debug_net_send( uint8_t channel, uint8_t *ptr, uint32_t len ) { return -1; }
static inline int scan_debug_net_is_open(void) { return false; }
static inline int scan_debug_get_ip(void) { return -1; }
static inline void scan_debug_set_ip(uint32_t destip) {}
static inline int mkip( char *str, uint32_t *ip ) { return -1;}
#endif

/* davep 29-Jan-2007 ; Note: these flags are for testing/debugging the scan
 * subsystem ONLY. During normal run-time, they should all be disabled (zero).
 */
#define SCAN_CONFIG_ALL_DISABLED 0
#define SCAN_CONFIG_NO_XMARGINS  (1<<0)
#define SCAN_CONFIG_NOPC         (1<<1)
#define SCAN_CONFIG_NOPRNU       (1<<2)
#define SCAN_CONFIG_NODSNU       (1<<3)
#define SCAN_CONFIG_NOQUAD       (1<<4)
#define SCAN_CONFIG_SCANMAN      (1<<5) 
#define SCAN_CONFIG_PIPENET      (1<<6) 
#define SCAN_CONFIG_NOCAL        (1<<7) 
#define SCAN_CONFIG_ICEFILE      (1<<8) 
#define SCAN_CONFIG_COPY_TO_HOST (1<<9) 
#define SCAN_CONFIG_NOPIE        (1<<10) 
#define SCAN_CONFIG_NOLEDS       (1<<11) 
#define SCAN_CONFIG_CAL_ONLY     (1<<12)
#define SCAN_CONFIG_NOBPR        (1<<13)
#define SCAN_CONFIG_NOJOBNOTCH   (1<<14)
#define SCAN_CONFIG_SCAN_FROM_HOME (1<<15)
#define SCAN_CONFIG_FAKESCAN     (1<<16)
#define SCAN_CONFIG_NO_CISX      (1<<17)
#define SCAN_CONFIG_NO_CALCACHE_TIMEOUT (1<<18)
#define SCAN_CONFIG_NO_ESD       (1<<19)
#define SCAN_CONFIG_NO_PSENSE    (1<<20)
#define SCAN_CONFIG_FORCE_PP     (1<<21)
#define SCAN_CONFIG_PP_POSITION  (1<<22)
#define SCAN_CONFIG_PIPE_DRAIN   (1<<23)
#define SCAN_CONFIG_PR_TEST      (1<<24)
#define SCAN_CONFIG_CAFFEINE     (1<<25)

/* do NOT use these set config functions casually! very dangerous! secret
 * backdoor use only! 
 */
uint32_t scan_get_config( void );
void scan_set_config( uint32_t flags );
void scan_set_config_pipenet(bool pipenet);
void scan_set_config_nocal(bool enable);
void scan_set_config_icefile(bool enable);
void scan_set_config_copy_to_host( bool enable );
void scan_set_config_nopie( bool enable );
void scan_set_config_noleds( bool enable );
void scan_set_config_cal_only( bool enable );
void scan_set_config_nobpr( bool enable );
void scan_set_config_nojobnotch( bool enable );
void scan_set_config_scan_from_home( bool enable );
void scan_set_config_fakescan( bool enable );
void scan_set_config_no_calcache_timeout( bool enable );
void scan_set_config_no_pc( bool enable );
void scan_set_config_no_prnu( bool enable );
void scan_set_config_no_dsnu( bool enable );
void scan_set_config_no_quad(bool enable);
void scan_set_config_force_pp( bool enable );
void scan_set_config_pp_position( bool pp_enable );
void scan_set_config_pipe_drain( bool enable );
void scan_set_config_pr_test( bool enable );
void scan_set_config_no_xmargins( bool enable );
void scan_set_config_no_esd( bool enable );
void scan_set_config_no_psense( bool enable );
void scan_set_config_no_cisx( bool enable );
#endif /* SCANDBG_H */

