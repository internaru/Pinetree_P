/******************************************************************************
**                                                                           **
**                 Proprietary and Confidential Information                  **
**                                                                           **
**                 Copyright (c) Conexant, Inc. Unpublished                  **
**                                                                           **
** This source code and the algorithms implemented therein constitute        **
** confidential information and may compromise trade secrets of Conexant,    **
** Inc. or its associates, and any unauthorized use thereof is prohibited.   **
**                                                                           **
******************************************************************************/
#ifndef __FSHS_H__
# define __FSHS_H__

/******************************************************************************
*  FILE FORMAT:
*      1. ENUMERATIONS
*      2. DEFINES
*      3. STRUCTURES
*      4. ALLOCS
*      5. FUNCTIONS
******************************************************************************/

//#include "dc_type.h"
#include "tx_api.h"
#include "configmo.h"

/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/

enum
{
  FAX_REPORT_TX_CONFIRM,
  FAX_REPORT_ACTIVITY_LOG,
  FAX_REPORT_SETTINGS,
  FAX_REPORT_LOST_JOBS,
  FAX_REPORT_COVERSHEET   
};

/******************************************************************************
*  2. DEFINES
******************************************************************************/

#define PUMA_STYLE_FAX           // Use Puma Fax features
#define PUMA_SSF336              // Puma using Conexant SSF336 modem

#define FSU_STANDALONE           // Uncomment to use dbgmon only as U.I.
#define ENABLE_HOMOLOGATION_API  // Uncomment to enable the new homologation utility for DP3 code

#define MODEM_RESET_ON()         modem_spi_reset()
#define MODEM_RESET_OFF()        modem_spi_clear_reset()

#define DISCONNECT_LOCAL_PHONE() // (dcreg(RA_GP_OUTPUT0_J) &= ~0x01)  // NULL macro for Copperhead; no relay
#define CONNECT_LOCAL_PHONE()    // (dcreg(RA_GP_OUTPUT0_J) |=  0x01)  // NULL macro for Copperhead; no relay

UINT8 local_phone_off_hook (void);  // +sjb-6/29/07

#define LOCAL_PHONE_OFF_HOOK()   local_phone_off_hook ()

 #if defined(RTOS_THREADX)
   #include <ra_ua.h>
 #elif defined(RTOS_LINUX)
 #endif

  #define MODEM_UA_CONTROL              RA_UA0_CONTROL
  #define MODEM_UA_STATUS               RA_UA0_STATUS
  #define MODEM_UA_CONFIG               RA_UA0_CONFIG
  #define MODEM_UA_EMI_REC              RA_UA0_EMI_REC
  #define MODEM_UA_HBAUD_LO             RA_UA0_HBAUD_LO
  #define MODEM_UA_HBAUD_HI             RA_UA0_HBAUD_HI
  #define MODEM_UA_STATUS_FIFO          RA_UA0_STATUS_FIFO
  #define MODEM_UA_CONFIG_FIFO          RA_UA0_CONFIG_FIFO
  #define MODEM_UA_POINTERS_FIFO_EMI    RA_UA0_POINTERS_FIFO_EMI
  #define MODEM_UA_POINTERS_CPU_EMI     RA_UA0_POINTERS_CPU_EMI
  #define MODEM_UA_POINTERS_FIFO_REC    RA_UA0_POINTERS_FIFO_REC
  #define MODEM_UA_POINTERS_CPU_REC     RA_UA0_POINTERS_CPU_REC
  #define MODEM_UA_TEST                 RA_UA0_TEST
  #define MODEM_UA_ENABLE               RA_UA0_ENABLE

  #define MODEM_UA_BUFFERDATA_LO        RA_UA0_BUFFERDATA_LO
  #define MODEM_UA_BUFFERDATA_MIDL      RA_UA0_BUFFERDATA_MIDL
  #define MODEM_UA_BUFFERDATA_MIDH      RA_UA0_BUFFERDATA_MIDH
  #define MODEM_UA_BUFFERDATA_HI        RA_UA0_BUFFERDATA_HI
  #define MODEM_UA_UNIBUFFERDATA_LO     RA_UA0_UNIBUFFERDATA_LO
  #define MODEM_UA_UNIBUFFERDATA_MIDL   RA_UA0_UNIBUFFERDATA_MIDL
  #define MODEM_UA_UNIBUFFERDATA_MIDH   RA_UA0_UNIBUFFERDATA_MIDH
  #define MODEM_UA_UNIBUFFERDATA_HI     RA_UA0_UNIBUFFERDATA_HI
  #define MODEM_UA_TRANSMIT_DT_CONTROL  RA_UA0_TRANSMIT_DT_CONTROL
  #define MODEM_UA_RECEIVE_DT_CONTROL   RA_UA0_RECEIVE_DT_CONTROL
  #define MODEM_UA_CONFIGEXT            RA_UA0_CONFIGEXT
 #if 0 //(DC_PARTNUM == DC2250)
  #define MODEM_SPIDS_ACTIVE()          (regwr(RA_GP_OUTPUT0_I, dcreg(RA_GP_OUTPUT0_I) & ~0x08))
  #define MODEM_SPIDS_INACTIVE()        (regwr(RA_GP_OUTPUT0_I, dcreg(RA_GP_OUTPUT0_I) |  0x08))
  #define MODEM_RD_SPIDS()              (dcreg(RA_GP_OUTPUT0_I) & 0x08)
 #else
  #define MODEM_SPIDS_ACTIVE()          (regwr(RA_GP_OUTPUT0_N, dcreg(RA_GP_OUTPUT0_N) & ~0x01))
  #define MODEM_SPIDS_INACTIVE()        (regwr(RA_GP_OUTPUT0_N, dcreg(RA_GP_OUTPUT0_N) |  0x01))
  #define MODEM_RD_SPIDS()              (dcreg(RA_GP_OUTPUT0_N) & 0x01)
 #endif


#define DSP_2MS_ISR      // Service the DSP (Modem) as part of the timer ISR
#define FAX_PAGE_NUM_256 // Use 8-bit page numbers for Tx/Rx status

// ------------------------------------------------------- //
// Configuration for non-reference design fax applications //
// ------------------------------------------------------- //
// This configuration is used for a specific customer and, generally, should not
// be modified.  Use/modify the general purpose configuration.

  // *** Features *** //
  //#define MONITOR_PARALLEL_PHONES  // Use parallel phones to start Rx
  #define INDEPENDENT_ECM_SETTING  // Use ECM setting when Rcvr doesn't support V.34
  //#define REJECT_BAD_RX_PAGES      // Reject bad receive pages and send RTN
  #define RETRANSMIT_PAGE_FOR_RTN  // Resend rejected pages
  #define RETRY_FOR_COMM_ERROR     // Retry after communication errors
  #define BUSY_PROGRESSIVE_REDIAL  // +sjb-3/03/06: Progress delay time for busy/no answer
  #define ENCODE_MR_MMR            // Encode pages using MR or MMR
  #define DECODE_MR_MMR            // Receive and decode MR and MMR pages
  #define MULTIPLE_PASS_MODE       // Uncomment to use multiple PASS mode encoding
  #define FSU_ENABLE_CALLER_ID     // Process Caller I.D. information
  #ifdef FSU_ENABLE_CALLER_ID
    #define CALL_SCREENING           // Display caller I.D. info for blocking junk faxes
    #define CALLER_ID_BLOCKED      0xFE
    #define CALLER_ID_UNAVAILABLE  0xFF
    #define INDICATE_BLOCKED_AND_UNAVAILABLE_NUM  // Indicate blocked and unavailable numbers
  #endif
  #define CONVERT_FOR_SCAN_AFTER_DIAL // Down convert page parameters for scan after dial jobs
  //#define CONVERT_SCAN_BEFORE_DIAL_JOBS  // Down convert page parameters for scan before dial jobs
  //#define SPLIT_LONG_TX_PAGES            // Split long received pages into two pages +sjb-3/15/07

  #define USE_PROGRAMMABLE_PAUSE_TIME    // Allow pause time for dialing to be variable
  #ifdef USE_PROGRAMMABLE_PAUSE_TIME
    //#define PAUSE_TIME_MAX 99          // Range: 0 to 99
    //#define PULSE_20PPS  *** Fix bug in code!!!
  #endif
  #define LIMIT_ONE_RETRY_IF_NO_ANSWER   // Retry a maximum of one time if no answer

  #define ORIGINATE_POLLING
  #ifdef ORIGINATE_POLLING
    //#define MANUAL_ORIGINATE_POLLING
  #endif
  //#define ANSWER_POLLING  // Not implemented yet!
  //#define ORDERLY_TX_END_OF_CALL
  #define AUTO_ANS_DET_CNG
  //#define SET_TONE_PULSE_EACH_DIGIT
  //#define WAIT_FOR_SCAN_MEMORY
  //#define V17_FAX_ONLY  // *sjb-10/06/08  +sjb-12/20/07: Uncomment to use V.17/V.29/V.27 only, no V.34
  #define DP3_DSP_ANS_MODES  // +sjb-3/03/08: Enable answer mode operation for DP3 DSP

  // *** Modem/T.30 Configuration *** //
  #define DTS_COMPLIANCE           // Comply with DTS requirements (No complete yet)
  //#define USE_UNCOMPRESSED_MODE  // Not tested yet!!!
  #define SUPPORT_NA_LTR_LGL_SIZE  // Support North American letter and legal sizes
  //#define SUPPORT_V33_MODULATION   // Support V.33 modulation  -sjb-2/13/08

  // *** Parameters *** //
  #define BC_JOBS 1                         // Number of broadcast jobs
  #define ADF_SCAN_PAGE_LIMIT 35            // Maximum number of pages to scan using the ADF
  #define BC_DEST 10                        // Number of destinations per broadcast job
  #define CID_MAX 120                       // Length of Caller I.D. string supported
  #define FAX_NUM_SIZE 64                   // Max. size of phone numbers Range: 10 to 64 digits
  #define DIAL_BUF_SIZE FAX_NUM_SIZE + 10   // Size of dial buffer

  #ifdef DP3_DSP_ANS_MODES   // +sjb-3/03/08: Begin...
    #define NO_RING_TIMEOUT 12000             // Time to qualify ringing has stopped (ms) for DP3
  #else  // +sjb-3/03/08: End
    #define NO_RING_TIMEOUT 10000             // Time to qualify ringing has stopped (ms) for DP2
  #endif  // +sjb-3/03/08

	#define DELAY_BETWEEN_CALLS 15000         // Delay between calls (ms)
	#define OFF_HOOK_ALARM_TIME 2000          // Time to qualify local phone off-hook is an alarm (ms)
  #define MAX_TX_JOB_NUM 16000
  #define MIN_TX_JOB_NUM 8000
  #define MAX_RX_JOB_NUM 7999
  #define MIN_RX_JOB_NUM 1

  #define DIAL_NUM_MAX FAX_NUM_SIZE  // +sjb-5/2/05

  #define SENDER_NAME_MAX 24  // +sjb-6/9/05: Begin...
  #define SENDER_NUMBER_MAX 20
  #define FAX_FORWARD_NUM_MAX 64
  #define ANS_CODE_SIZE 16   // +sjb-6/9/05: End

  #define FAX_NAME_MAX 24      // Move to uihs.h!
  #define FAX_FORWARD_MAX 64   // Move to uihs.h!
  #define MAX_BCAST_NUM 5      // Move to uihs.h!
  #define FAX_NUMBER_MAX 20    // Move to uihs.h!

  // *** NVRAM Defines ***
//  #define DISABLE_NVRAM_USAGE  // Comment out to use NVRAM  +sjb-12/17/08

  #define FAX_SPEED_DIAL_MAX 79
  #define FAX_GROUP_DIAL_MAX 20
  #define FAX_JUNK_FAX_MAX 50
  #define CODE_PAGE_DEFAULT 0xFFFF
  #define FAX_ACTIVITY_LOG_MAX 40
  #define FAX_PENDING_LOG_MAX 10

  #define TSI_STR_LEN   20
  #define CID_STR_LEN   30

  #define FSU_SPEED_DIAL_ENABLED
  //#define FSU_GROUP_DIAL_ENABLED
  //#define FSU_JUNK_FAX_ENABLED

    // Call logging stored in NVRAM
  #define FAX_CALL_LOG_MAX 30  // Number of calls to log
  #define FAX_LOG_FULL 30      // Threshold to print log
  //#define LOG_DIALED_NUM     // Uncomment to log number dialed
  //#define LOG_CID_NAME       // Uncomment to log Caller I.D. name
  //#define LOG_CID_NUMBER     // Uncomment to log Caller I.D. number

  // *** AUDIO Defines ***
  #define AUDIO_TONES_MAX 4
  //#define ENABLE_RING_SILENCE  // +sjb-2/05/08: Uncomment to allow ring audible to be silenced per call

// -------------------- //
// *** FAX PRINTING *** //
// -------------------- //

  #define MAX_PRINT_JOBS 8

  #if 1
    #define MAX_PRINT_PAGES 100
  #else
    #define MAX_PRINT_PAGES 10  // TEST!!!
  #endif

  #define ENABLE_FSU_PRINT_TEST

  #ifdef ENABLE_FSU_PRINT_TEST
    // #define ENABLE_TEST_SET_DONE
    UINT8 fsu_pmgr_test_print_done (void);

// +sjb-4/26/06: Begin addition...
    //#define CAPTURE_RECV_FAX

    //#ifdef CAPTURE_RECV_FAX  // Capture faxes to develop color tables, tonal response curves, settings, etc.
      //void init_capture_fax (void);
      //UINT8 capture_fax_page (FSU_PRINT_PAGE_t *ptr);
    //#endif
// +sjb-4/26/06: End

  #endif

  // *** For bringing up FSU without a printer ***

  #define DUMP_RX_PRINT_JOB    // Un-comment to have fsu_call.c dump receive jobs

  //#define DISABLE_FSU_PRINTER_LOCK
  //#define DISABLE_FSU_MEDIA_DETECT
  //#define DISABLE_FSU_FOOTERS
  //#define DISABLE_FSU_PRINT

  #define DISABLE_FSU_SPLIT_PAGE

  // Print scale factors to print at 1200/600/300 dpi; convert fax res to print res.
  // NOTE: 200x100 dpi & R8x3.85 l/mm are printed as 600x1200 dpi.
  //       All others at 1200x1200 dpi.

  #define FRAC_PRN_SCALE    // Define for fraction print scaling to one decimal place

  #ifdef FRAC_PRN_SCALE
    #define FAX_PRN_SCALE_R8        30         // Approx 200 dpi * 30 / 10 = 600 dpi
    #define FAX_PRN_SCALE_R16       30         // Approx 400 dpi * 30 / 10 = 1200 dpi
    #define FAX_PRN_SCALE_3_85_L_MM 120        // Approx 100 dpi * 12 / 10 = 1200 dpi
    #define FAX_PRN_SCALE_7_7_L_MM  60         // Approx 200 dpi * 60 / 10 = 1200 dpi
    #define FAX_PRN_SCALE_15_4_L_MM 30         // Approx 400 dpi * 30 / 10 = 1200 dpi

    #define FAX_PRN_SCALE_100_VDPI  120        // 100 dpi * 120 / 10 = 1200 dpi
    #define FAX_PRN_SCALE_200_HDPI  30         // 200 dpi * 30 / 10 = 600 dpi
    #define FAX_PRN_SCALE_200_VDPI  60
    #define FAX_PRN_SCALE_300_HDPI  40
    #define FAX_PRN_SCALE_300_VDPI  40
    #define FAX_PRN_SCALE_400_HDPI  30
    #define FAX_PRN_SCALE_400_VDPI  30

    #define FAX_PRN_SCALE_DIVISOR 10000

  #else
    #if 1  // +sjb-12/13/06: Use new shingling modes -- Begin addition...
      #define FAX_PRN_SCALE_R8        3        // Approx 200 dpi
      #define FAX_PRN_SCALE_R16       3        // Approx 400 dpi
      #define FAX_PRN_SCALE_3_85_L_MM 12       // Approx 100 dpi
      #define FAX_PRN_SCALE_7_7_L_MM  6        // Approx 200 dpi
      #define FAX_PRN_SCALE_15_4_L_MM 3        // Approx 400 dpi

      #define FAX_PRN_SCALE_100_VDPI  12       // 100 dpi -> 1200 dpi
      #define FAX_PRN_SCALE_200_HDPI  3        // 200 dpi -> 600 dpi
      #define FAX_PRN_SCALE_200_VDPI  6        // 200 dpi -> 1200 dpi
      #define FAX_PRN_SCALE_300_HDPI  4        // 300 dpi -> 1200 dpi
      #define FAX_PRN_SCALE_300_VDPI  4
      #define FAX_PRN_SCALE_400_HDPI  3        // 400 dpi * 3 = 1200 dpi
      #define FAX_PRN_SCALE_400_VDPI  3

      #define FAX_PRN_SCALE_DIVISOR 1000

    #else  // +sjb-12/13/060: End addition.
      // Note: Scale fax res to print res of 1200x1200 dpi or 1200x600 dpi
      #define FAX_PRN_SCALE_R8        6
      #define FAX_PRN_SCALE_R16       3
      #define FAX_PRN_SCALE_3_85_L_MM 12
      //#define FAX_PRN_SCALE_3_85_L_MM 6
      #define FAX_PRN_SCALE_7_7_L_MM  6
      #define FAX_PRN_SCALE_15_4_L_MM 3

      #define FAX_PRN_SCALE_100_DPI   12        // 100 dpi * 12 = 1200 dpi
      //#define FAX_PRN_SCALE_100_DPI   6        // 100 dpi * 6 = 600 dpi
      #define FAX_PRN_SCALE_200_DPI   6
      #define FAX_PRN_SCALE_300_DPI   4
      #define FAX_PRN_SCALE_400_DPI   3

      #define FAX_PRN_SCALE_DIVISOR 1000

    #endif  // +sjb-12/13/06
  #endif

  // *** FOOTER *** //
  // Lines at fax res for 8 pt (.111") tall footer
  // 1 point = 1/72nd of an inch
  // 165 lines at 1200 approx. scales to 8 pt characters

#if 0   // +sjb-2/02/06: Default font
  #define FAX_FOOTER_PRINT_LINES 165                  // Number of lines @1200 dpi to print an 8-pt footer
  #define FAX_FOOTER_HORZ_SCALE(x) (x * 2463 / 1000)  // Factor to scale horizonatlly for 8-pt height

  #define FAX_FOOTER_HEIGHT_3_85_L_MM  14   // Lines at 3.85 l/mm for 165 lines at 1200 dpi (14 * 12)
  #define FAX_FOOTER_HEIGHT_7_7_L_MM   28   // 28 lines @ 7.7 l/mm * 6 = apporx. 165 lines @ 1200 dpi
  #define FAX_FOOTER_HEIGHT_15_4_L_MM  55

  #define FAX_FOOTER_HEIGHT_100_DPI   14    // Lines at 100 dpi for 165 lines at 1200 dpi
  #define FAX_FOOTER_HEIGHT_200_DPI   28    // 28 lines @ 200dpi * 6 = approx. 165 lines @ 1200 dpi
  #define FAX_FOOTER_HEIGHT_300_DPI   42
  #define FAX_FOOTER_HEIGHT_400_DPI   55

#else  // +sjb-2/02/06: Begin addition...
  // Kodak 8 pt font char is on 24x48 pixels at 300 dpi

  #define FAX_FOOTER_PRINT_LINES 192        // Number of lines @1200 dpi to print an 8-pt footer (48 * 4)
  //#define FAX_FOOTER_HORZ_SCALE(x) (x * 4000 / 1000)  // Factor to scale horizonatlly for 8-pt height (300 dpi * 4)

  #define FAX_FOOTER_HEIGHT_3_85_L_MM  16   // Lines at 3.85 l/mm for 192 lines at 1200 dpi (14 * 12)
  #define FAX_FOOTER_HEIGHT_7_7_L_MM   32   // 28 lines @ 7.7 l/mm * 6 = apporx. 192 lines @ 1200 dpi
  #define FAX_FOOTER_HEIGHT_15_4_L_MM  64

  #define FAX_FOOTER_HEIGHT_100_DPI   16    // Lines at 100 dpi for 192 lines at 1200 dpi
  #define FAX_FOOTER_HEIGHT_200_DPI   32    // 28 lines @ 200dpi * 6 = approx. 192 lines @ 1200 dpi
  #define FAX_FOOTER_HEIGHT_300_DPI   48
  #define FAX_FOOTER_HEIGHT_400_DPI   64

#endif // +sjb-2/02/06: End addition..

  #define ENABLE_FAX_RECEIVE_REJECTION

  // *** REPORTS *** //

  #define FSU_REPORT_HORZ_PSCALE 1   // *sjb: set these!!!
  #define FSU_REPORT_VERT_PSCALE 1
  #define FS_USE_REPORT_BANDS
  //#define FS_USE_REPORT_HEADER     // Not used!
  //#define FS_USE_REPORT_FOOTER     // Not used!


//  #define SAVE_BMP_TO_FILE           // to test text rendering, for linux only
//  #define TEST_DMB_ALLOCATE        // to see how DMB allocate gives index and base address for the block

/******************************************************************************
*  3. STRUCTURES
******************************************************************************/

  // Move to uihs.h!

  #define ACTIVITY_LOG_STR_LEN 32

  typedef struct
  {
    char name[ACTIVITY_LOG_STR_LEN];
    char number[ACTIVITY_LOG_STR_LEN];
    char status[ACTIVITY_LOG_STR_LEN];
  } ACTIVITY_LOG_t;

  typedef union
  {
    UINT16 d;
    struct
    {
      UINT8 busy;
      UINT8 index;
    } b;

  } LOG_INDEX_t;

  typedef struct
  {
    UINT16 total_fax_sent;
    UINT16 total_fax_receive;
    UINT16 total_fax_jobs;

    UINT16 total_pages_sent;
    UINT16 total_pages_receive;

    UINT16 total_fax_lost;
    UINT16 total_not_sent;
    UINT16 total_not_printed;

    LOG_INDEX_t activity_idx; // UINT16 Size
    LOG_INDEX_t pending_idx;  // UINT16 Size

  } FAX_STATISTICS;

/******************************************************************************
*  4. ALLOCS
******************************************************************************/
extern TX_MUTEX dc_chip_mutex;

/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/

#endif  /* __FSHS_H__ */

