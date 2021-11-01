/**
 * \file scancore.h
 *
 * \brief "Core" header file for scan code.
 *
 * Anything completely general, completely applicable to all parts of the scan
 * code.  Useful memory macros, ASIC definiitions.
 *
 * This file is designed to be the first scan firmware #include in all the scan
 * source.
 *
 * Do NOT add #includes to this file (or any other header file) under threat of
 * a severe beating.  The one #include is specifically there to carefully
 * solve a few platform portability problems.
 *
 * ============================================================================
 * Copyright (c) 2006-2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef SCANCORE_H
#define SCANCORE_H

/* davep 13-Apr-2012 ; Converted all the scan code to C99 standard.  The sheer
 * amount of code requiring these headers necessitated the "no include in
 * header files" rules violation.
 */
#include <stdint.h>
#include <stdbool.h>

/* davep 09-Apr-2008 ; grab our compile-time feature list */
#include "scanconfig.h"

/********************************/
/********************************/

/*
 *  Scanning general purpose, convenience code.
 */

#if ICE_DMA_BYTE_ALIGN==16
    #define ICE_DMA_BYTE_ALIGN_MASK 0xfffffff0
#elif ICE_DMA_BYTE_ALIGN==32
    #define ICE_DMA_BYTE_ALIGN_MASK 0xffffffe0
#else
    /* must have ICE_DMA_BYTE_ALIGN in scanconfig-<asic>.h */
    #error invalid ICE_DMA_BYTE_ALIGN value; missing from scanconfig.h?
#endif

/* stupid human check ; if we're trying to do above 16-byte bursts and the ASIC
 * doesn't support it, raise an error
 */
#if ICE_DMA_BYTE_ALIGN != 16 && !defined HAVE_ICE_DMA_SET_BURST_SIZE 
    #error ICE DMA 32-byte bursts not supported on this platform
#endif

/* round up to DMA alignment */
#define ICE_DMA_ALIGN_ME(n)  ( ((n)+(ICE_DMA_BYTE_ALIGN-1)) & ~(ICE_DMA_BYTE_ALIGN-1))

/* davep 29-Mar-2011 ; verify dma aligned value */
#define ICE_DMA_IS_ALIGNED(n) (((n) & ~ICE_DMA_BYTE_ALIGN_MASK)==0)

/* initialize all DMA buffers to this value (when testing) */
//#define DMA_BUFFER_INIT  0xee

/* get/set/clear a certain bit */
#define GET_BIT(num,bitpos)  ( (num) & (1 << (bitpos)) )
#define SET_BIT(num,bitpos)  ( (num) |= (1 << (bitpos)) )
#define CLR_BIT(num,bitpos)  ( (num) &= ~(1 << (bitpos)) )

/* davep 16-Aug-2005 ; set/clear cache bypass bit */
/* davep 28-Oct-2007 ; added the ISNOTSET to make #def'ing the macros easier on
 * systems without the cache bypass stuff
 */
#ifdef HAVE_CACHE_BYPASS
#define CACHE_BYPASS_SET(ptr)   ((uint8_t *)((uint32_t)(ptr) | CACHE_BYPASS_OFFSET))
#define CACHE_BYPASS_CLEAR(ptr) ((uint8_t *)((uint32_t)(ptr) & ~CACHE_BYPASS_OFFSET))
#define CACHE_BYPASS_ISSET(ptr) ((uint32_t)(ptr) & CACHE_BYPASS_OFFSET)
#define CACHE_BYPASS_ISNOTSET(ptr) (!((uint32_t)(ptr) & CACHE_BYPASS_OFFSET))
#else

/* davep 03-May-2010 ; the CACHE_BYPASS_OFFSET symbol has disappeared from our
 * recent systems but scan needs it. Note I'm not checking (#ifndef) if it's
 * already defined. I want a warning/error if it's not the same as 0.
 */
#define CACHE_BYPASS_OFFSET 0

/* davep 8-Sep-2007 ; don't mess with pointers if we're running on our
 * host-based system
 */
#define CACHE_BYPASS_SET(ptr)   ((uint8_t *)(ptr))
#define CACHE_BYPASS_CLEAR(ptr) ((uint8_t *)(ptr))
#define CACHE_BYPASS_ISSET(ptr) (1)
#define CACHE_BYPASS_ISNOTSET(ptr) (1)
#endif

/* davep 11-May-2012 ; porting to Linux Kernel */
#ifndef __KERNEL__
typedef void * dma_addr_t;
#endif

/* an attempt to make something like C++ auto_ptr */
#define PTR_ASSIGN(dst,src)  do { (dst)=(src); (src)=NULL; } while(0)
/* davep 02-Jul-2008 ; use the new memory manager functions */
#define PTR_FREE(ptr)  do { memFree((ptr)); (ptr)=NULL; } while(0)

/* Determine whether a time count in the future has passed.
 *
 * Note that this must work with fixed sized integers (uint32_t)
 * to correctly handle clock rollover.
 */
#define TIMER_EXPIRED(now,start,delay)  ( (now)-(start) < (delay) ? 0:1)

#define FLOAT_TO_DECIMAL(f)   ( (int)( (f)*100 ) )

#ifdef ASP_ON_LITTLE_ENDIAN
    #define asp_htonl(x) cpu_to_be32(x)
    #define asp_htons(x) cpu_to_be16(x)
#else
    #define asp_htonl(x) x
    #define asp_htons(x) x
#endif

#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif

#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#endif

#ifndef UNUSED_ARG
#define UNUSED_ARG(a) do { (void)(a); } while(0);
#endif

/********************************/
/********************************/

/* 27-Mar-05 ; private messages, messages passed inside the Scan module itself
 * such as pie_interrupt() passing data to a task.  
 *
 * SMSG == Scan Message
 */
/* Note the public SMSG_xxx messages in scantypes.h start at SMSG_PUB_PAGE
 * which is 1000.
 */

#define SMSG_BASE  0

/* PIE messages */
#define SMSG_PIE_RDMA_DATA (SMSG_BASE + 1)
#define SMSG_PIE_WDMA_DATA (SMSG_BASE + 2)
#define SMSG_PIE_RDMA_INT  (SMSG_BASE + 3)
#define SMSG_PIE_WDMA_INT  (SMSG_BASE + 4)

/* PIE HalfPack DMA */
#define SMSG_PIEHP_RDMA_DATA (SMSG_BASE + 50)
#define SMSG_PIEHP_WDMA_DATA (SMSG_BASE + 51)

/* PIC messages */
#define SMSG_PIC_WDMA_DATA (SMSG_BASE + 100)
#define SMSG_PIC_ESD       (SMSG_BASE + 101)
#define SMSG_PIC_ESD_FLUSH (SMSG_BASE + 102)
#define SMSG_PIC_PSENSE    (SMSG_BASE + 103)

/* AFE messages */
#define SMSG_AFE_HW_FAILURE    (SMSG_BASE + 150)    /* AFE has hardware failure */

/* Scan messages */
#define SMSG_SCAN_AFEPCOMP (SMSG_BASE + 200)
#define SMSG_SCAN_SCMDCOMP (SMSG_BASE + 201)
#define SMSG_SCAN_SCMDEND  (SMSG_BASE + 202) /* scan command queue will end the scan at new expected row (for ADF) */
#define SMSG_SCAN_DEBUG    (SMSG_BASE + 203) /* debug/test message; params vary */
#define SMSG_SCAN_CMDQSTOP (SMSG_BASE + 204) /* scan command queue empty cause motor stop */
#define SMSG_SCAN_MECH_STATUS (SMSG_BASE + 205) /* scan mech changed state */
#define SMSG_SCAN_MECH_FAULT  (SMSG_BASE + 206) /* scan mech detected hw fault */
#define SMSG_SCAN_MECH_PWR_ON  (SMSG_BASE + 207) /* power up the scan hardware */
#define SMSG_SCAN_MECH_PWR_OFF (SMSG_BASE + 208) /* power down the scan hardware (if possible) */
#define SMSG_SCAN_ADF_PAPER_EVENT (SMSG_BASE + 209) /* ADF paper (paper present change, etc)*/
#define SMSG_SCAN_MECH_IOCTL (SMSG_BASE + 210) /* free form mech ioctl */
 
/* ICE Net messages */
#define SMSG_ICENET_RDATA (SMSG_BASE + 300) /* PIE data from TCP; send to read DMA */
#define SMSG_ICENET_RDONE (SMSG_BASE + 301) /* channel is done (socket closed) */
#define SMSG_ICENET_WDATA (SMSG_BASE + 302) /* returning data buffer sent to TCP */
#define SMSG_ICENET_WDONE (SMSG_BASE + 303) /* write channel socket closed */

/* Scan SC messages */
#define SMSG_SC_AFE_OFFSET_FAIL        (SMSG_BASE + 211) //kmg
#define SMSG_SC_AFE_GAIN_FAIL          (SMSG_BASE + 212) //kmg
#define SMSG_SC_AFE_SETTING_FAIL       (SMSG_BASE + 213) //kmg 
#define SMSG_SC_CIS_HOME_POSITION_FAIL (SMSG_BASE + 214) 
#define SMSG_SC_WHITE_STRIP_READ_FAIL  (SMSG_BASE + 215) //kmg
#define SMSG_SC_BLACK_STRIP_READ_FAIL  (SMSG_BASE + 216) //kmg
#define SMSG_SC_BAD_PIXEL_FAIL 		   (SMSG_BASE + 217) //kmg 
#define SMSG_SC_CIS_HOME_OFF_FAIL 	   (SMSG_BASE + 218) //kks
#define SMSG_SC_CIS_HOME_ON_FAIL       (SMSG_BASE + 219) //kks
#define SMSG_SC_BAD_PIXEL_FAIL_B 	   (SMSG_BASE + 220) //kmg 
#define SMSG_SC_PIC_OVF_FAIL		   (SMSG_BASE + 221) //lsh
#define SMSG_SC_CMD_QUEUE_OVR_FAIL	   (SMSG_BASE + 222) //lsh
#define SMSG_SC_CMD_COMPLETE_OVR_FAIL  (SMSG_BASE + 223) //lsh
#define SMSG_SC_CMD_MEM_MALLOC_FAIL	   (SMSG_BASE + 224) //lsh
#define SMSG_SC_PIPE_WAIT_TIMEOUT      (SMSG_BASE + 228) //ksb
#define SMSG_SC_SCAN_ASSERT_FAIL	   (SMSG_BASE + 240) //ksb

/* Copy messages */
#define SMSG_COPY_DEBUG_JOB (SMSG_BASE + 400) /* Start an internal (debug) copy job */

/* ICE Software Filter */
#define SMSG_ICE_FILTER_DATA (SMSG_BASE + 500)

/* ICEFILE, scan input data from USB/TCP/Whatever */
#define SMSG_ICEFILE_CANCEL         (SMSG_BASE+600) /* tell icefile to cancel */
#define SMSG_ICEFILE_RUN_INTERNAL   (SMSG_BASE+601) /* tell icefile to run with internal page */
                                                        
/* Calibration */
#define SMSG_CAL_RUN_CALIBRATION         (SMSG_BASE+700) /* Do a calibration */
#define SMSG_CAL_CALIBRATION_IN_PROGRESS (SMSG_BASE+701) /* informational message to scanman */
#define SMSG_CAL_CALIBRATION_DONE        (SMSG_BASE+702) /* informational message to scanman */

/* davep 31-Oct-2012 ; adding scanman messages */
#define SMSG_SCANMAN_SCAN_START (SMSG_BASE + 750) /* linux userspace -> kernel */

/* davep 08-Dec-2010 ; firmware margins (for when PIC hardware margins
 * won't work)
 */
#define SMSG_FW_READ_MARGIN_DATA (SMSG_BASE + 800) 
#define SMSG_FW_WRITE_MARGIN_DATA (SMSG_BASE + 801) 

/* davep 30_mar-2011 ; adding icetest */
#define SMSG_ICETEST_IDMA_DATA (SMSG_BASE + 900)

/* more messages as they arrive */
/* 
 * Do not exceed 1000! 1000 and above are used for the public messages (see
 * scantypes.h)
 */

/* davep 23-May-2013 ; adding sensor number to all the PIE multistrip DMA
 * INT messages. Used to support platforms with multiple scan sensors.
 */
#define SMSG_PIEMS_INT_GET_SENSOR_NUM(n)  ((n)&0xff)
#define SMSG_PIEMS_INT_SET_SENSOR_NUM(n)  ((n)&0xff)

/* davep 24-Apr-2009 ; when we moved to the BSDK ~July, 2008, I created
 * scan_msg_t and started using the msgType field. But I failed to update the
 * documentation below so I'm updating the docs now.
 */

/* 
 * PIE read/write Data message is sent from pie_dma_interrupt(). The message
 * contains the DMA channel's received data in a pie_dma_buffer structure as
 * well the total number of bytes the channel has received so far. 
 *
 * msgType:  SMSG_PIE_RDMA_DATA, SMSG_PIE_WDMA_DATA 
 *
 * param1:  reserved
 *
 * param2:  0 or ICE_DMA_LAST_BUFFER if this is the last buffer (see icedma.c) 
 *
 * param3:  pointer to pie_dma_buffer from DMA complete
 * 
 * Sent from PIE read/write DMA isr to move completed data from interrupt to a
 * thread. 'param2' will be true if this is the last buffer from the channel.
 * The thread can watch for the param2 to know the channel's data is done.
 *
 * msgType:  SMSG_PIC_WDMA_DATA 
 * Same as SMSG_PIE_WDMA_DATA but for PIC
 *
 * 25-Apr-05 XXX; I think SMSG_PIE_RNET_DATA will fit in here but I need to do
 * some testing to make sure. Stay tuned.
 *
 * davep 23-May-2013 ; adding sensor_num to param1 to help support platforms
 * with multiple scan sensors ("nsensor" systems). The sensor_num will be used
 * especially to map pic/pie dma channels back and forth.
 */

/* 13-June-05 davep ; added the XXX_INT messages to PIE DMA. Instead of
 * handling the complex Multi Strip DMA in interrupt context, communicate the
 * interrupt to a thread via a message. The thread will handle the buffer
 * dance.
 *
 * msgType: SMSG_PIE_[RW]DMA_INT
 * param1: unused
 * param2: channel number
 * param3: reserved
 */

/* davep 06-Sep-2005 ; added SMSG_SCAN_SIZE */
/** \def SMSG_SCAN_SIZE 
 *  Message containing the size of the scanned area. Derived by the Scan Worker
 *  by taking the requested scan size and adjusting it up, down, left, right,
 *  and sideways for DMA. Necessary to tell a higher layer the description of
 *  the actual data.
 *
 * \li msgType : SMSG_SCAN_SIZE
 * \li param1: pixels per row padded
 * \li param2: pixels per row
 * \li param3: total rows expected
 */

/* davep 17-Nov-2005 ; added SMSG_SCAN_CMDQSTOP */
/** \def SMSG_SCAN_CMDQSTOP
 *
 * A scan is carved up into chunks, each chunk a certain number of lines long.
 * Commands for a certain number of lines are queued into the scan command
 * register as memory comes available.  During the scan, scan_interrupt() will
 * push in new commands. To stop the scan when memory runs low, a new command
 * won't be pushed into the command register and the motor will be "gently"
 * stopped. The SMSG_SCAN_CMDQSTOP tells the thread the last capture line as
 * best as we can tell and also the quad count to restart.
 *
 * \li msgType : SMSG_SCAN_CMDQSTOP
 * \li param1: unused
 * \li param2: unused
 * \li param3: unused
 */

/* davep 24-Apr-2009 ; adding documentation on SMSG_SCAN_MECH_STATUS. The
 * message was originally created by Theron Luttman.
 */
/** \def SMSG_SCAN_MECH_STATUS
 *
 * The mech status has changed. The Scantask must call the scanmech to
 * check/update the status.
 *
 * Originally created to handle transitioning scanmech responsibility from
 * interrupt space to threadspace.  See smech_set_status() and
 * scan_cmdq_mech_status_change().
 *
 * \li msgType: SMSG_SCAN_MECH_STATUS
 * \li param1: scan mech status (t_ScanMechStatus)
 * \li param2: unused
 * \li param3: unused
 */

/* davep 24-Apr-2009 ; adding SMSG_SCAN_MECH_FAULT (w/ BurtP) */
/** \def SMSG_SCAN_MECH_FAULT
 *
 * The mech driver has detected a hardware error.  This is a Bad Thing!
 *
 * Originally created to handle passing DC motor errors from interrupt context
 * to threadspace.
 *
 * \li msgType: SMSG_SCAN_MECH_FAULT
 * \li param1: opaque; passed from mech driver interrupt to mech driver (scantask) thread
 * \li param2: opaque; passed from mech driver interrupt to mech driver (scantask) thread
 * \li param3: opaque; passed from mech driver interrupt to mech driver (scantask) thread
 */

/* davep 08-Aug-2009 ; moving mech power messages from scantypes to internal
 * header 
 */
/** \def SMSG_SCAN_MECH_PWR_ON
 *
 * The mech driver should power on.
 *
 * See also scantask_power_on() and scantask_power_off().
 *
 * \li msgType: SMSG_SCAN_MECH_PWR_ON
 * \li param1: reserved
 * \li param2: reserved
 * \li param3: reserved
 */
/** \def SMSG_SCAN_MECH_PWR_OFF
 *
 * The mech driver should power off.
 *
 * See also scantask_power_on() and scantask_power_off().
 *
 * \li msgType: SMSG_SCAN_MECH_PWR_OFF
 * \li param1: reserved
 * \li param2: reserved
 * \li param3: reserved
 */

/* davep 26-Aug-2009 ; adding adf paper event */
/** \def SMSG_SCAN_ADF_PAPER_EVENT 
 *
 * \li msgType: SMSG_SCAN_ADF_PAPER_EVENT
 * \li param1: scan_adf_paper_event_t
 * \li param2: value (depending on event)
 * \li param3: reserved
 *
 * Something happened to paper in the ADF. For example, paper present asserted,
 * top-of-form switch asserted, yadda yadda yadda.
 *
 * The value field depends on the event.  For example, the paper present event
 * could send value==1 for paper present, value==0 for paper not present.
 *
 * Originally created as a message because sensors are usually GPIO devices
 * handled with interrupts.
 */

/* davep 16-Mar-2006 ; adding ICEFILE */
/** \def SMSG_ICEFILE_READ_BUFFER
 *
 * ICEFILE allows us to feed known good image data into the scanpipe without
 * using Scan or PIC.  This message is used to send a full buffer from the
 * ICEFILE thread to ScanTask to be injected into the scan pipe. For example,
 * ICEFILE reads a chunk of image data from USB then messages the full buffer
 * to ScanTask to be Read DMA'd into PIE. 
 *
 * \li param1: SMSG_ICEFILE_READ_BUFFER
 * \li param2: 0 (not last buffer); ICE_DMA_LAST_BUFFER (this is the last * buffer) 
 * \li param3: pointer to icebuf containing read data
 */

/* davep 17-Mar-2006 */
/** \def SMSG_ICEFILE_READ_ADD_READY
 *
 * ICEFILE allows us to feed known good image data into the scanpipe without
 * using Scan or PIC.  This message is used to pass an icebuf back from
 * ScanTask to the ICEFILE thread. The ICEFILE thread will message buffers to
 * ScanTask via SMSG_ICEFILE_READ_BUFFER ScanTask will feed the data into
 * the scanpipe then message the buffer back to the ICEFILE thread with this
 * message.
 *
 * \li msgType: SMSG_ICEFILE_READ_ADD_READY
 * \li param1: not used
 * \li param2: channel number
 * \li param3: pointer to icebuf containing read data
 */

/* davep 17-Mar-2006 */
/** \def SMSG_ICEFILE_IMAGE_INFO 
 *
 * ICEFILE has an interesting chicken-and-egg problem. ICEFILE runs in a
 * separate thread from scantask (in order to support USB and TCP and whatever
 * other transport we want) but ICEFILE needs to know how large to make its
 * icebuf data before sending to scantask, information only scantask knows (via
 * all the convulted calculations in scanpipe and scanlib). 
 *  
 * \li msgType : SMSG_ICEFILE_IMAGE_INFO
 * \li param1 : not used
 * \li param2 : sizeof(struct icefile_image_info)
 * \li param3 : pointer to allocated icefile_image_info (will be freed by
 *              icefile)
 */

#endif /* SCANCORE_H */

