/*
 *  $RCSfile: ATTRIB.H,v $
 * $Revision: 1.3 $
 *   $Author: miurat $
 *     $Date: 2009/09/25 18:45:38 $
 */
/************************************************************************
 *
 *      ATTRIB.H
 *
 *  This header file defines the ATTRIB type.
 *
 *  Copyright 1998 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef __ATTRIB_H__
#define __ATTRIB_H__

#define GET                 0
#define SET                 1
#define INIT                2

#define CPA                 0
#define CPA_END             0xFF
#define DCA                 0x100
#define DCA_END             0x1FF
#define DPA                 0x200
#define DPA_END             0x2FF
#define DSA                 0x300
#define DSA_END             0x3FF
#define FXA                 0x400
#define FXA_END             0x4FF
#define LCA                 0x500
#define LCA_END             0x5FF
#define PCA                 0x600
#define PCA_END             0x67F
#define PHA                 0x680
#define PHA_END             0x6FF
#define RTA                 0x700
#define RTA_END             0x7FF
#define SCA                 0x800
#define SCA_END             0x8FF
#define SMA                 0x900
#define SMA_END             0x9FF
#define HSA                 0xA00   // DFP-Host Scan
#define HSA_END             0xAFF
#define VCA                 0xB00
#define VCA_END             0xBFF
#define VPA                 0xC00
#define VPA_END             0xCFF
#define ATA                 0xD00
#define ATA_END             0xDFF
#define RGA                 0xE00
#define RGA_END             0xEFF
#define IMA                 0xF00   // Ink Manager
#define IMA_END             0xFFF
#define AMA                 0x1000  // Answering machine
#define AMA_END             0x10FF
#define MNA                 0x1100
#define MNA_END             0x11FF
#define RCA                 0x1200
#define RCA_END             0x12FF
#define HPA                 0x1300
#define HPA_END             0x13FF
#define PGA                 0x1400  /* Pager attributes */
#define PGA_END             0x147F
#define MDA                 0x1500  /* Modem setup attributes */
#define MDA_END             0x15FF
#ifdef INTERNET_FAX
#define T37A                0x1700 /* T37 attributes */
#define T37A_END            0x17FF
#endif

typedef enum {

	PGA_FAX_PAGING = PGA,   /* Pager attributes */
	PGA_PAGING_NO,
	PGA_PAGING_PIN,
	PGA_TASKSTATUS,
	PGA_VOICE_PAGING,

	RCA_ACTIVATE = RCA,     /* Remote Activate */
	RCA_ACTIVATECODE,       /* Remote Activate Code */
	RCA_ACTIVATE_ENABLE,        /* Remote Activation on or off */
	RCA_ACCESSCODE,         /* Remote Access Code */
	RCA_DEACTIVATE,         /* Remote Deactivate */
	RCA_DEACTIVATECODE,     /* Remote Deactivate Code */
	RCA_REMOTECONTROL,      /* Remote Control */
	RCA_REMOTEMAINTENANCE,  /* Remote Maintenance */
	RCA_FACTORYMAINTENANCE, /* Factory Line Remote Maintenance */
	RCA_TASKSTATUS,         /* Remote control Task Status */
	RCA_DTMF_PATHTHROUGH,   /* DTMF detect path through mode. */
	RCA_DTMF_DATA,          /* Remote control Task detected DTMF data */
	RCA_TAMSTATUS,          /* Tam status */

	// Modem setup attributes:
	MDA_RI_RDMAXP = MDA,    /* Maximum Samples per Ring Frequency Period: UINT16 */
	MDA_RI_RDMINP           /* Minimum Samples per Ring Frequency Period: UINT16 */

} ATTRIB;

#endif // __ATTRIB_H__
