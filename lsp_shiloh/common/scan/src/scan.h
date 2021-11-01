/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *****************************************************************************/

#ifndef SCAN_H
#define SCAN_H

struct scan_interrupt_stats {
    unsigned int total;              /* incremented every scan_interrupt() */
    unsigned int cmdQueueOverrun;
    unsigned int cmdCompleteOverrun;
    unsigned int afeComplete;        /* AFE R/W completed */
    unsigned int cmdComplete;        /* Scan Command completed */
    unsigned int cmdq_empties;
    unsigned int motor_stops;
};

typedef int (*scan_cmdq_isr_t)( void );
scan_cmdq_isr_t scan_set_cmdq_interrupt( scan_cmdq_isr_t isr_function );

/* davep 12-Nov-2012 ; add method to hook the scan block's external interrupt
 * source (usually a digital AFE). Originally I just grabbed the ISR but now
 * the interrupts have to be SMP safe I'm adding enable/disable as well.
 */
struct scan_external_interrupt {
    /* returns non-zero if interrupt was previously enabled */
    uint32_t (*disable)(void);

    /* unconditionally enables interrupt */
    void (*enable)(void);

    /* handler when scan_interrupt() gets external interrupt */
    void (*isr)(void);
};

scan_err_t scan_set_external_interrupt( struct scan_external_interrupt *scan_ext_int );

#ifdef SCANHWERR_H
ScannerHWError scan_get_hwerr( void );
void scan_set_hwerr( ScannerHWError scanner_hw_err );
#endif

scan_err_t scan_onetime_init(void); // one time cold init

struct scan_interrupt_stats * scan_get_interrupt_stats( void );
void scan_interrupt_stats_dump( void );
void scan_interrupt_stats_clear( void );

uint32_t scan_interrupt_disable( void );
void scan_interrupt_enable( void );

#endif // SCAN_H

