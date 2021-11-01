#ifndef FWCXDXX_H
#define FWCXDXX_H

/*********************************************************************
 *
 * FwCxDxx.h
 * Driver interface for the firmware T4/T6 compression code.
 *
 * This should be treated as the equivalent of the hardware.  Thus
 * it is the duty of the module controlling FwCxDx to make sure that
 * the task is scheduled only once (using SchedTask function) and
 * that the inputs and the outputs are co-ordinated as well as that
 * the SchedTask, AbortJob, and SwitchJob functions are called at
 * the proper times only.
 * These routines make it possible to use the REDC firmware
 * compression / decompression routines similiar to the
 * hardware.  The main difference is that many of the
 * details mentioned in the T4/T6 hardware spec as needed
 * to be done by the firmware will also be done by the
 * REDC firmware.  The job handling details are still
 * done by the T4CxDx (T4 device driver) layer above this.
 * The following assumptions are made.
 * - On MH or MR decompression, T4CxDx will always select to
 *   search for the first EOL before expecting any output.
 * - On line errors on MR decompression, T4CxDx will always
 *   search for the next 1D EOL tag.  (This means that the
 *   data generated inbetween will have to be thrown away !).
 *
 * Copyright 1997  Rockwell Semiconductor Systems, Inc.,
 * 4311 Jamboree Road, Newport Beach  CA 92660  USA.
 * This file contains confidential and proprietary, unpublished source
 * code solely owned by the Rockwell Semiconductor Systems, Inc.
 * All rights reserved.
 *
 * Subject to change without notice.
 *
 ********************************************************************/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !!!!! It was decided not to use nested includes. So, !!!!!
  !!!!! TYPEDEFF.H, TYPEDEF.H, and Rtypedef.H          !!!!!
  !!!!! MUST be included before this.                  !!!!!
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


/***********************
  For optimization, the REDC tasks are combined with FwCxDx task.
  **********************/
#define FWCXDX_COMBINE_TASKS
#if defined(RTOS_THREADX)
#define BOOL boolean
#elif defined(RTOS_LINUX)	
#endif

/*
  These routines make it possible to use the REDC firmware
  compression / decompression routines similiar to the
  hardware.  The main difference is that many of the
  details mentioned in the T4/T6 hardware spec as needed
  to be done by the firmware will also be done by the
  REDC firmware.  The job handling details are still
  done by the T4CxDx (T4 device driver) layer above this.

  The following assumptions are made.
  - On MH or MR decompression, T4CxDx will always select to
    search for the first EOL before expecting any output.
  - On line errors on MR decompression, T4CxDx will always
    search for the next 1D EOL tag.  (This means that the
    data generated inbetween will have to be thrown away !).
  */


#define MAX_T4_JOBS 4

#define CONFIG_BIT_DECOMPRESSION 0x04
#define CONFIG_BIT_CODING_TYPE 0x03
#define CONTRL_BIT_MMR_FLUSH 0x01
#define CONTROL_BIT_BUSY 0x02
#define CONTROL_BIT_DECODE_OUT 0x10
#define STATUS_BIT_EOL 0x0080
#define STATUS_BIT_TAG 0x40
#define STATUS_BIT_NON_CONSECUTIVE_EOL 0x20
#define STATUS_BIT_LINE_ERROR 0x10
#define INTERRUPT_BIT_EOL 0x40
#define INTERRUPT_BIT_LINE_ERROR 0x08


#define DECOMPRESSION       1
#define COMPRESSION         0

#define ALLOC_BUFF_SIZE    256*20      // enough for 400 dpi
//#define CDB_SIZE_IN_BUFF      200     // desirable number of coded data in a Buffer

//.. define these things here for temp .. THY 11/18/08
/* Coding Schemes */
#define CODING_MH               0
#define CODING_MR               1
#define CODING_MMR              2
#define CODING_JPEG             3
#define CODING_JPEG_COLOR       3
#define CODING_JPEG_GRAYSCALE   4
#define CODING_JBIG             5


#define RES_STANDARD        0  // 200 x 100
#define RES_FINE            1  // 200 x 200
//#define RES_SUPERFINE       2  // 200 x 400 -> Not support
#define RES_300             3  // 300 x 300
#define RES_ULTRAFINE       4  // 400 x 400 -> we have not been supporting 400 dpi but we have to add this because your code support this.


/* K Factors */
#define KFACTOR_STANDARD    2
#define KFACTOR_FINE        4
#define KFACTOR_SFINE       8

#define MAX_NUM_QUEUE       20


/* return value of T4codec   */
#define T4CODEC_OK              0
#define T4CODEC_NO_DATA         1
#define T4CODEC_FATAL_ERROR     2
#define T4CODEC_NOT_CALLED      3


/* error code */
#define E_OK            0     /* 0x00 No error */
#define E_EMPTY         1     /* 0x01 No object to return */
#define E_FULL          2     /* 0x02 Buffer full */
#define E_PARAM         3     /* 0x03 Parameter invalid */
#define E_PTRZ          4     /* 0x04 Zero address */
#define E_NOTFND        5     /* 0x05 Not Found */
#define E_NOTYET        6     /* 0x06 Not Yet Supported */

//#define MAX_FRAME_IMP 5           // maximum bumber of frames for T4codec to handle at one time
//#define NUM_OF_BUFF       (300/MAX_FRAME_IMP) // number of buffer to prepare for ECM rx
#define MAX_FRAME_IMP   1           // maximum bumber of frames for T4codec to handle at one time
#define NUM_OF_BUFF     (256/MAX_FRAME_IMP) // number of buffer to prepare for ECM rx

#define DEFAULT_STACK_BYTES     16*1024

//#define MAX_BYTE_NON_ECM_RX     256*3
#define MAX_BYTE_NON_ECM_RX     256
#define NUM_OF_BUFF_NON_ECM     2
#define MAX_NON_ECM_TXFRAME     256

#define CIRCB_BUFF_DENOM		4			

// last buffer
#define JOBNODE_BUFFER_LAST			1
#define JOBNODE_BUFFER_NOT_LAST		0

#if 0
static UINT16 res_width_table[3][5] = {
	{ 1728, 1728, 1728, 2592, 3456 },    /* A4 */
	{ 2048, 2048, 2048, 3072, 4096 },    /* B4 */
	{ 2432, 2432, 2432, 3648, 4864 }     /* A3 */
};
#endif

// define for Paper width
#define A4_WIDTH_STD		1728
#define A4_WIDTH_FINE		1728
#define A4_WIDTH_SFINE		1728
#define A4_WIDTH_300		2592
#define A4_WIDTH_400		3456

#define B4_WIDTH_STD		2048
#define B4_WIDTH_FINE		2048
#define B4_WIDTH_SFINE		2048
#define B4_WIDTH_300		3072
#define B4_WIDTH_400		4096

#define A3_WIDTH_STD		2432
#define A3_WIDTH_FINE		2432
#define A3_WIDTH_SFINE		2432
#define A3_WIDTH_300		3648
#define A3_WIDTH_400		4864

//.. t30/system abort
#define FIPP_T30_ERROR			1
#define FIPP_ABORT_ERROR		2


// T4Codec Query ..
enum
{
	T4CODEC_ACTIVATED,
	T4CODEC_DONE,
	T4CODEC_TOT_RXLINE,
	T4CODEC_BAD_RXLINE,
	T4CODEC_RTC_DETECTED,
	T4CODEC_TOO_MANY_ERRORS,

	T4CODEC_LAST
};


enum
{
	FSU_STATE_IDLE,                   // 0
	FSU_STATE_TX_INIT1,
	FSU_STATE_TX_INIT1A,
	FSU_STATE_TX_INIT2,
	FSU_STATE_TX_INIT2A,

	FSU_STATE_TX_ORG_POLL,            // 5
	FSU_STATE_TX_DIAL,
	FSU_STATE_TX_START,
	FSU_STATE_TX_PHASE_B,
	FSU_STATE_TX_POLL_PHASE_B,

	FSU_STATE_TX_PHASE_C_WAIT,        // 10
	FSU_STATE_TX_PHASE_C_PAGE_WAIT,
	FSU_STATE_TX_PHASE_C_SEND,
	FSU_STATE_TX_SEND_PPS,
	FSU_STATE_TX_PHASE_D,

	FSU_STATE_TX_PHASE_E,             // 15
	FSU_STATE_TX_ABORT,
	FSU_STATE_RX_START,
	FSU_STATE_RX_INIT2,
	FSU_STATE_RX_INIT2A,

	FSU_STATE_RX_INIT3,               // 20
	FSU_STATE_RX_CID_WAIT,
	FSU_STATE_RX_AUTO_ANS,
	FSU_STATE_ANS_WAIT_FOR_COMMAND,
	FSU_STATE_ANS_WAIT_FOR_OFFHOOK,

	FSU_STATE_ANS_LOOK_FOR_CNG,       // 25
	FSU_STATE_ANS_WAIT_FOR_ON_HOOK,
	FSU_STATE_RX_PHASE_B,
	FSU_STATE_RX_PHASE_C_WAIT_TX_POLL,
	FSU_STATE_RX_PHASE_C_WAIT_EOM,

	FSU_STATE_RX_PHASE_C_WAIT,        // 30
	FSU_STATE_RX_PHASE_C_RECV,
	FSU_STATE_RX_PHASE_D,
	FSU_STATE_RX_PHASE_E,
	FSU_STATE_OFF_HOOK_WAIT,

	FSU_STATE_OFF_HOOK,               // 35
	FSU_STATE_ON_HOOK,
	FSU_STATE_RX_ABORT,
	FSU_STATE_OFF_HOOK_ALARM,
	FSU_STATE_SET_MODEM_IDLE,

	FSU_STATE_TEST_MODE,              // 40
	FSU_STATE_DIRECT_TO_FAX_MODE,
	FSU_STATE_TX_PHASE_C_WAIT_EOM,

	FSU_STATE_TIME_UPDATE,
	FSU_STATE_WAIT,                   // 44
	FSU_STATE_TX_GOT_MCF,             // 45
	FSU_STATE_MEM_RX_START,		  	  // 46
	FSU_STATE_MEM_RX_START_MPS,
	FSU_STATE_RX_MANUAL_ANS,
	FSU_STATE_TX_DIAL_MANUAL,

    FSU_STATE_TX_START_MANUAL,

	FSU_STATE_NOT_INITIALIZED         // Keep this last
};


// DrvT30 IO Message
enum
{
	IOCTL_T30FAX_MESSAGE,
	IOCTL_T30FAX_WAIT_RXNEGO,
	IOCTL_T30FAX_WAIT_TXNEGO,
	IOCTL_T30Rx_GET_POST_MSG,
	IOCTL_T30FAX_GET_ATTRIBUTE,
	IOCTL_T30FAX_SET_ATTRIBUTE,			// 5
	IOCTL_CHECK_RCVDFAXQ,
	IOCTL_END_OF_PAGE,          // indicate T4 have done with last buffer
	IOCTL_ECM_MODE_RX,
	IOCTL_ECM_MODE_TX,
	IOCTL_BYTE_PER_FRAME,				// 10
	IOCTL_RX_CODING_TYPE,
	IOCTL_TX_CODING_TYPE,
	IOCTL_MAX_BYTES_PER_LINE,
	IOCTL_MIN_BYTES_PER_LINE,
	IOCTL_SET_T4TX_PARAMS,				// 15
	IOCTL_SET_T4RX_PARAMS,
	IOCTL_GET_TX_FAX_RES,
	IOCTL_GET_RX_FAX_RES,
	IOCTL_T30SET_ABORT,
	IOCTL_T30GET_STATUS,				// 20
	IOCTL_GET_COMM_RES_RX,
	IOCTL_GET_COMM_RES_TX,
	IOCTL_GET_COMM_WIDTH_RX,
	IOCTL_GET_COMM_WIDTH_TX,

	IOCTL_T30FAX_LAST
};


typedef signed char FIX8;
typedef unsigned char UFIX8;
typedef signed short FIX16;
typedef unsigned short UFIX16;
typedef signed long FIX32;
typedef unsigned long UFIX32;
typedef unsigned int ERCODE;        /* Error code */


typedef struct _JOBNODE_BUFFER
{
	UFIX32          size;               ///< size of buffer in bytes
	UFIX32          used;               ///< number of bytes that are used
	UFIX32          userData;           ///< user-defined data
	UFIX32          lastBuffer;         ///< TRUE if last buffer for job
	UFIX32          refCount;           ///< free when refCount is zero
	UFIX32          flags;              ///< misc flags
	struct _JOBNODE_BUFFER *next;
	volatile UFIX8  *data;              /// should be volatile
} JOBNODE_BUFFER;


extern void activate_scan_to_mem(void);
extern void activate_mem_to_send(void);
extern void activate_rcv_to_mem(void);
extern void activate_mem_to_prn(void);
extern void job_modem_start(void);
extern void job_t30_start(void);
extern void activate_modem_job(void);
extern void give_dmb_to_modem(JOBNODE_BUFFER *pBuff);
extern int T30Query(UINT8 request, UINT8 *pBuff, UINT32 size);
extern BOOLEAN fsu_get_tti(void);
void fsu_gen_fax_header(char *PBMFileName, UINT8 faxRes, UINT8 fax_image_width_type);

extern UFIX16 Fw_T4Config;                        /* Same as T4Config register */
extern UFIX16 Fw_T4Control;                       /* Same as T4Control register */
extern UFIX16 Fw_T4Status;                        /* Same as T4Status register */
extern UFIX16 Fw_T4Bytes;                         /* Same as T4Bytes register */
extern UFIX16 Fw_T4IntMask;                       /* Same as T4IntMask register
                                                     - for Error interrupts ????*/
extern UFIX32 Fw_BmpPtr;
extern UFIX32 Fw_CodePtr;
extern UFIX16 Fw_CodeSiz;


/* Registers that don't have an equivalent in hardware */
extern UFIX16 Fw_MinLine;                        /* Minimum line length in Bytes
                                                    for compressed data */
extern UFIX16 Fw_nRTCEOL;                        /* Number of EOLs in RTC */
extern UFIX16 Fw_KFactor;                        /* K Factor, if applicable */
extern UFIX16 Fw_LastLine;                       /* Non-zero for last Bitmap line on */

extern UINT16 bytes_per_line;
extern UINT16 stride_per_line;

#endif
