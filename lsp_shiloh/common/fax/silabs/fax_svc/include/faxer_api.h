/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

/**\file faxer_api.h
 *  The file defines the fax service module API's
 *
 */

#ifndef FAXER_H
#define FAXER_H

#include <stdint.h>
#include "fax_store_api.h"
#include "fax_tiff_io.h"
#include "date_time_api.h"


/** Max lengths of strings */
#define FAXER_MAX_TEL_DIGITS  (50)       /* max phone number length */
#define FAXER_MAX_PREFIX_SIZE (25)       /* max prefix length */
#define FAXER_MAX_HEADER_SIZE (25)
#define FAXER_MAX_SID_SIZE (20)

#define FAXER_STORAGE_DRIVE "/ramfax"

/**\brief Fax result code
 *
 *  These enums define various fax result codes which are stored as the status field 
 *  of the FAXER_ACTIVITY_ENTRY structure defined further below.
 *
 */
typedef enum
{
     /** This is the default status at a fax session creation.*/
    FAXER_ACT_STATUS_NULL = 0,

     /** The call failed for some reason.*/
    FAXER_ACT_STATUS_CALL_FAILED,

     /** There was no answer when calling.*/
    FAXER_ACT_STATUS_NO_ANSWER,

     /** Picked up the phone but did not have a dial tone.*/
    FAXER_ACT_STATUS_NO_DIALTONE,

     /** A busy signal was detected.*/
    FAXER_ACT_STATUS_LINE_BUSY,

     /** The call has completed.*/
    FAXER_ACT_STATUS_FAX_COMPLETE,

    /** The call completed with an RTN.*/
    FAXER_ACT_STATUS_COMPLETE_RTN,

     /** Sent at least 1 page of a fax.*/
    FAXER_ACT_STATUS_PARTIAL_FAX,

     /** Called the number but did not detect a fax.*/
    FAXER_ACT_STATUS_NO_FAX,

     /** There was some unspecified communication error.*/
    FAXER_ACT_STATUS_COMM_ERROR,

     /** There was a file system error.*/
    FAXER_ACT_STATUS_FS_ERROR,

    /** There is no more room in FS to store faxes.*/
    FAXER_ACT_STATUS_FS_FULL,

     /** The state is set to retry the fax that previously failed.*/
    FAXER_ACT_STATUS_RETRY,

     /** Received a fax that was to be blocked.*/
    FAXER_ACT_STATUS_BLOCKED,

     /** The fax was canceled after the call was started.*/
    FAXER_ACT_STATUS_CANCELED,

     /** The fax was canceled after the call was started.*/
    FAXER_ACT_STATUS_UNSCHEDULED,

}FAXER_ACT_STATUS;


/**\brief The various activity types for a fax.
 *
 */
typedef enum
{
    /** An undefined fax event. */
    FAXER_ACT_TYPE_NULL = 0,

    /** A transmit fax event. */
    FAXER_ACT_TYPE_SEND = 1,

    /** A fax receive event. */
    FAXER_ACT_TYPE_RECEIVE = 2,

    /** A polling receive fax event. */
    FAXER_ACT_TYPE_POLLING_RCV = 3,

} FAXER_ACT_TYPE;

/**\brief The fax log activity log data structure
 *
 */
typedef struct
{
    /** Fax Job ID*/
    uint32_t             job_id;

    /** Fax activity type code*/
    FAXER_ACT_TYPE       type;

    /** Fax event time
     *
     *  For a pending fax, this is scheduled time. For an ongoing or
     *  completed fax, this is fax start time time.
     *
     * */
    struct tm          time;

    /** The duration in seconds of this activity.*/
    uint16_t             duration;

    /**Fax activity status.   */
    FAXER_ACT_STATUS    status;

    /** Number of pages send or received in this fax. */
    uint16_t            page_cnt;

    /** CSI/TSI/CIG or remote phone number or caller ID number */
    char                remoteId[50 + 1];
    /** CID remote caller ID name */
    char                remoteName[50 + 1];

    /** The file name.*/
    char                fname[30 + 1];

    /** Pending actions.
     *
     *  Each bit in this flag represent a pending action associated
     *  with a fax job. Bit 0 is used by the fax module to mark an
     *  unsent outgoing fax or an unread incoming fax. Users can
     *  define their own pending actions by using bit 1 to bit 15 of
     *  this flag. Fax module will keep all the fax files (TIFF,
     *  trace) for a fax job if its pending action flag is not
     *  cleared.
     **/
    uint16_t            pending;

} FAXER_ACTIVITY_ENTRY;

/**\brief Configurable faxer options
 *
 */
typedef enum 
{
    /** Fax subscriber ID (SID)\n
     *  This is the subscriber ID that the fax machine uses to
     *  identify itself during a fax call. It shall be the
     *  international telephone number including '+' character, the
     *  country code, area code and subscriber number. It shall
     *  consist of 20 numeric digits, with length less than 20.
     **/
    FAXER_SETTING_CHR_SID = 1,

    /** Specify the distinctive ring patterns as defined below:\n
     *  0: All ring types\n
     *  1: Single ring only\n
     *  2: Double ring only\n
     *  3: Triple ring only\n
     *  4: Double and triple ring\n
     *
     **/
    FAXER_SETTING_INT_RING_PATTERN,

    /** Set to enable fax forwarding\n
     *  User can  use FAXER_SETTING_CHR_FORWARD_NUMBER to set the fax forwarding number.
     **/
    FAXER_SETTING_SW_FORWARD,

    /** Forward telephone number\n
     * Use this set the fax forwarding number, the number is limited to UTF8 string with a length
     * less than 50.
     **/
    FAXER_SETTING_CHR_FORWARD_NUMBER,

    /** If set, re-dial at busy
     **/
    FAXER_SETTING_SW_BUSY_REDIAL,

    /** If set re-dial at no-answer
     **/
    FAXER_SETTING_SW_NOANS_REDIAL,

    /** Set to re-dial at communication error\n
     **/
    FAXER_SETTING_SW_COMMERR_REDIAL,

    /** ECM switch\n
     * Set this to enable ECM
     **/
    FAXER_SETTING_SW_ECM_MODE,

    /** Fax speed selection, choose from followings:\n
     *  0: Fast,   with V.34,V.17 and V.29 all enabled\n
     *  1: Medium, with V.17 and V.29 enabled, V.34 disabled\n
     *  2: Slow    With only V.29 enabled.\n
     **/
    FAXER_SETTING_INT_MAX_SPEED,

    /** Dialing mode, choose from followings:\n
     *  0: tone dialing mode\n
     *  1: pulse dialing mode\n
     **/
    FAXER_SETTING_INT_DIALING_MODE,

    /** Set to enable using prefix during number dialing\n
     *  Use FAXER_SETTING_CHR_PREFIX to set prefix string.
     **/
    FAXER_SETTING_SW_DIAL_PREFIX,

    /** Set to require dial tone detection before dialing.
     *
     **/
    FAXER_SETTING_SW_DETECT_DIAL_TONE,

    /** Number of rings to be detected before a call is answered\n
     * This is used in automatic fax answer mode.
     **/
    FAXER_SETTING_INT_RINGS_TO_ANSWER,

    /** Set to stamp incoming fax\n
     * Use FAXER_SETTING_CHR_HEADER_NAME to set string used for fax
     * stamping.
     **/
    FAXER_SETTING_SW_STAMP_FAX,

    /** Answer mode, choose from followings:\n
     *  0: Manual\n
     *  1: Automatic\n
     *  2: Fax/Tel mode
     *  3: TAM mode
     **/
    FAXER_SETTING_INT_ANSWER_MODE,

    /** Name string for fax stamping (length < 25)\n
     * This string is used to stamp incoming or outgoing fax, it should have a length
     * less than 25.
     **/
    FAXER_SETTING_CHR_HEADER_NAME,

    /** Prefix strings (length < 50)\n
     * This string is used as dialing prefix, it should have a length
     * less than 50.
     **/
    FAXER_SETTING_CHR_PREFIX,

    /** Retry with V.34 off switch \n
     *  Set this to turn off V.34 during a retry.\n
     *  If FAXER_SETTING_INT_MAX_SPEED is set to Fast(0), retry will run with Medium (1) setting.\n
     *  If FAXER_SETTING_INT_MAX_SPEED is set to Medium(1) or Slow (2), no effect.\n
     *
     **/
    FAXER_SETTING_SW_RETRY_V34_OFF,

    /** Audio volume setting \n
     *  0 - off
     *  1 - soft
     *  2 - medium
     *  3 - loud
     */
    FAXER_SETTING_SW_AUDIO_VOLUME,

    /** Fax storage size
     *  Storage size in bytes.
     **/
    FAXER_SETTING_INT_STORAGE, 

 
    /** Fax ext DTMF answer sequence */
    FAXER_SETTING_CHR_EXT_ANS_DTMF, 

    /** Country/Region setting
     *  Set to a FAXER_REGION value. 
     **/
    FAXER_SETTING_REGION,

}FAXER_SETTING;



/**\brief Enums used to get various attributes of the fax session status.
 *
 */
typedef enum
{
    /** Non-zero ongoing fax job ID */
    FAXER_SESSION_INT_JOB_ID,

    /** Current ring count*/
    FAXER_SESSION_INT_RING_CNT,

    /** Line status*/
    FAXER_SESSION_INT_LINE_STATUS,

    /** Current page count*/
    FAXER_SESSION_INT_PAGE_CNT,

    /** Remote SID*/
    FAXER_SESSION_CHR_REMOTE_NUMBER,

    /** Current fax file name*/
    FAXER_SESSION_CHR_FAXFILE,
  
    /** JPEG enabled/supported */
    FAXER_SESSION_INT_JPEG_SUPPORTED,

    /** Color JPEG enabled/supported */
    FAXER_SESSION_INT_COLOR_SUPPORTED,

    /** Supported resolutions */
    FAXER_SESSION_INT_RESOLUTIONS,

}FAXER_SESSION_DATA;


/**\brief Various fax application callback events.
 *
 */
typedef enum 
{
    /** Non Event  */
    FAXER_APP_EVT_NULL = 0,

    /** Incoming Ring\n
     * This is sent when there is a new incoming ring. Each incoming
     *  ring will generate a new ring event.
     **/
    FAXER_APP_EVT_LIURING,

    /** Line is on-hooked\n
     *  This event is sent when the line is back to on-hook state.\n
     **/
    FAXER_APP_EVT_ONHOOKED,

    /** Fail to on-hook\n
     *  This event is sent when the line can not go back to the
     *  on-hook state.\n
     **/
    FAXER_APP_EVT_ONHOOKFAILURE,

    /** Line is Off-Hooked\n
     *  This event is sent when the line becomes off-hooked.
     **/
    FAXER_APP_EVT_OFFHOOKED,

    /**Fail to Off-hook\n
     *  This is sent when line device fails to off-hook
     */
    FAXER_APP_EVT_OFFHOOKFAILURE,

    /** Dial tone detected\n
     *  This event is sent when dial tone is detected after off
     *  hook.
     **/
    FAXER_APP_EVT_DIALTONE_DETECTED,

    /** Dial done\n
     * This event is sent when a dial string has been dialed out.
     **/
    FAXER_APP_EVT_DIALDONE,

    /** Fail to dial\n
     * This event is sent when a dial string can not be dialed out successfully
     */
    FAXER_APP_EVT_DIALFAILURE,

    /** No dial tone\n
     * This event is sent when there is no dial tone detected.
    **/
    FAXER_APP_EVT_NO_DIALTONE,

    /** remote answered\n
     * This event is sent when remote end has answered a fax/voice call.
     **/
    FAXER_APP_EVT_REM_ANSWERED,

    /** No answer\n
     * This event is sent when a no-answer is detected
     **/
    FAXER_APP_EVT_REM_NOANSWER,

    /** Detect busy\n
     *  This event is sent when a busy tone is detected from remote end.
     **/
    FAXER_APP_EVT_REM_BUSY,

    /** Incoming call answered\n
     * This event is sent when a incoming call has been answered.
     **/
    FAXER_APP_EVT_INCOMING_ANSWERED,

    /** Fax session is started, data holds job id \n
     *  This is sent when a fax session is started, it usually happens with the start of
     *  initial fax negotiation.
     **/
    FAXER_APP_EVT_SESSION_START,

    /** Fax session is ended, data holds FAX_FAILURE data result\n
     *  This is sent after DCN is sent/received, or a line is disconnected.
     **/
    FAXER_APP_EVT_SESSION_END,

    /** Waiting to retry fax send.
     * This is sent if the connection fails and we are trying to redial.
     **/
    FAXER_APP_EVT_WAITING_RETRY,

    /** Job has ended.
     *  Job completed and it is now safe to start a new job.
     **/
    FAXER_APP_EVT_JOB_END,

    /** Start to send/receive fax document, data holds job id\n
     *  This event is sent when fax negotiation has completed.
     **/
    FAXER_APP_EVT_DOC_START,

    /** End of fax document sending/receiving, data holds job id\n
     *  This event is sent when final page of fax document has been
     *  sent/received.
     **/
    FAXER_APP_EVT_DOC_END,

    /** Starting a new page send/receive, data holds the page number\n
     *  This event is sent at the beginning of a new page sending/receiving.
     **/
    FAXER_APP_EVT_PAGE_START,

    /** End of a fax page send /receive, data holds the page
     *  number.\n This event is sent at the end of a page
     *  sending/receiving.
     **/
    FAXER_APP_EVT_PAGE_END,

    /** Auto answer is on and has been triggered.  The app should respond\n
     *  by answering. 
     **/ 
    FAXER_APP_EVT_AUTO_ANSWER,
 
}FAXER_APP_EVT;

// The next four structures FAXER_SETTINGS, FAXER_CONTEXT, faxer_modem_state_t
// and FAXER_DATA_TYPE came from the prv_faxer.h and were moved here to aid in
// stubbing out the fax_svr code.
typedef struct
{
    int             settings;  
} FAXER_SETTINGS;

typedef struct
{
    int             context;  
} FAXER_CONTEXT;

typedef enum
{ 
  MODEM_IDLE = 0,
  MODEM_ACTIVE,
  MODEM_INITIALIZING,
} faxer_modem_state_t; 

typedef enum {
    FAXER_DATA_TYPE_UNKNOWN,
    FAXER_DATA_TYPE_MONO,
    FAXER_DATA_TYPE_COLOR,
    FAXER_DATA_TYPE_GRAY,
}FAXER_DATA_TYPE;

/**\brief Fax service module startup
 *
 *  This is the function that the fax application uses to initialize the fax service module.
 *
 *  \param[in] app_cb     Application call-back
 *
 *
 * \return fax service module handle, NULL if fax service fail to start
 *
 * To start the fax service, the user needs to call this function at system power up, with a callback 
 * function that will notify the calling application when a fax event occurs. This function will return 
 * a fax service module handle that should be used for all fax service calls.
 *
 */
void  *faxer_service_start(void (*app_cb)(uint16_t evt, void *data));

/**\brief Schedule a fax send
 *
 * \param[in] handle       Fax service module handle
 * \param[in] schdule_time Time to send
 * \param[in] dialString   Number to dial, NULL send over voice call
 * \param[in] res          Desired resolution (max) to use for sending this fax job.  
 * \param[in] fname        Tiff file name
 * \param[out] job_id      To hold job id for newly created fax job
 *
 *
 *  If the dial string is empty, the fax send can be either a monitor dialing call if the line is still idle, or a
 *  voice-to-fax call if the line is currently being used for voice call.
 *
 *  The input string 'dialString' is limited to 50 character length.
 *  The input string 'fname' is limited to 30 character length.
 *   *
 *  To send a fax immediately, make schedule_time earlier than current time.
 */
uint32_t faxer_schedule_send(void *handle, struct tm *schdule_time, char *dialString, 
                             FAX_TYPES res, char *fname, uint32_t *job_id);


/**\brief Create a new send job using the same file from an existing fax send job
 *
 * \param[in]  handle         Fax service module handle
 * \param[in]  lead_job_id    Id to an existing fax send job, considered as the lead job of a group
 * \param[out] job_id         Storage holds the id newly created fax send job
 *
 * \return completion code
 *
 * User can use this function to create a group sending job that shares the same fax file by adding a
 * new fax phone number to an existing send fax job.
 *
 * The input 'number' is limited to 50 character length.
 */
uint32_t faxer_group_send_add(void *handle, uint32_t lead_job_id, char *number, uint32_t *job_id);

/**\brief Start a color or grayscale jpeg fax send session
 *
 * \param[in] handle       Fax service module handle
 * \param[in] dialString   Number to dial, NULL send over voice call
 * \param[in] res          Desired resolution (max) to send with
 * \param[in] store        The store object that holds the document
 * \param[out] job_id      To hold job id for newly created fax job
 *
 *  The input string 'dialString' is limited to 50 character length.
*   
 */
uint32_t faxer_start_jpeg_send(void *handle, char *dialString, bool color, FAX_TYPES res,
                               fax_store_handle_t store, uint32_t *job_id);

/**\brief Set the data for a send if not passed in when starting the call.  
 *
 *  The data can be color or mono at this point.  This can often be the case for color sends when
 *  the app wants to know if the receiver supports color or not.  
 *
 * \param[in] handle      Fax service module handle
 * \param[in] jpeg        JPEG data?
 * \param[in] data        JPEG store or mono file containing data to send
 * \param[in] pages       Total pages already contained in document, any additional 
 *                        page completion should be notifed by calling add_page
 * \param[in] complete    Is the document complete 
 *
 *  The input string 'dialString' is limited to 50 character length. 
 *
 *  Note that the function will return an error if attempting to send color data when a mono job was 
 *  started.  An error will also return an error if the
 */
uint32_t faxer_send_set_data(void *handle, uint32_t job_id, bool jpeg, void *data, 
                             uint16_t pages, bool data_complete);

/**\brief Inform the fax application that a new page has added to an existing pending fax send job.
 *
 * \param[in]  handle      fax service module handle
 * \param[in]  job_id      Id of send job obtained from faxer_send_schedule
 * \param[in]  EOP         Set to indicate the page is the last page
 *
 * \return completion code
 *
 * If a new bw fax sending job is scheduled with tiff_incomplete flag set or color job is started, 
 * user should call this function after a new page is added to the TIFF file or fax store to signal 
 * the fax service module that a new page is present. The user can use the EOP flag to complete the job.
 */
uint32_t faxer_send_add_page(void *handle, uint32_t job_id, bool EOP);

/**\brief Start receiving a fax
 *
 * \param[in] handle      Fax service module handle
 * \param[in] color_store Fax store object to store the color data in, if this is a color job
 * \param[out] job_id  Buffer holds the newly received fax's job id
 *
 * \return completion code
 *
 * User can use this to either answer an incoming call, or switch from a voice call to fax receiving
 * if the line is currently being used  for a voice call.
 *
 * User can use the returned job_id to get the received fax file at the end of a fax call.
 */
uint32_t faxer_start_receive(void *handle, fax_store_handle_t color_store, uint32_t *job_id);


/**\brief Start polling receive
 *
 * \param[in] handle     Fax service module handle
 * \param[in] dialString Fax number to poll fax from
 * \param[out] job_id    Buffer holds the new receiving job id
 *
 * \return Completion code
 *
 * User can use the returned job_id to get the received fax file at end of the fax call.
 *
 * The input 'dialString' is limited to 50 character length.
 */
uint32_t faxer_start_polling(void *handle, char *dialString, uint32_t *job_id);

/**\brief Cancel an the ongoing fax
 *
 * \param[in] handle Fax service module handle
 *
 * \return Completion code
 *
 * User should use this the terminate/cancel an ongoing fax. The fax will be terminted and line
 * will go back to idle.
 */
uint32_t faxer_terminate_fax(void *handle);


/**\brief Offhook to place a phone call
 *
 * \param[in] handle Fax service module handle
 *
 *  \return Completion code
 */
uint32_t faxer_place_call(void *handle);


/**\brief Dial digits
 *
 *  \param[in] handle     Fax service module handle
 *  \param[in] digits     String of digits to dial
 *
 *  \return Completion code
 *
 *  Up to 50 digits can be placed in the digits string in a single call of this function. User should register
 *  for FAX_APP_DIALDONE event for successful
 */
uint32_t faxer_dial_digits(void *handle, char *digits);

/**\brief On-hook and finish a voice call
 *
 * \param[in] handle fax service module handle
 *
 * \return Completion code
 *
 * This can only be used to end a voice all.
 */
uint32_t faxer_end_call(void *handle);

/**\brief Answer a incoming voice call
 *
 * \param[in] handle fax service module handle
 *
 * \return Completion code
 */
uint32_t faxer_answer_call(void *handle);

/**\brief Access pending action on a fax job
 *
 *\param[in] handle   Fax service module handle
 *\param[in] job_id   Fax job id
 *\param[in,out] act  Pointer to buffer that holds bit values to write and the ones being read out
 *\param[in] act_mask Bit mask for action bits to be accessed
 *
 * \return Completion code
 *
 * Fax service routinely cleans its system to remove completed fax jobs, so user can use this to mark a certain
 * fax job as finished or still pending further actions. To read the pending action flag of a fax job,
 * set act_mask = 0 when calling this function, the current pending action bits will be returned in act buffer.
 *
 * System uses a 16-bit action flag to track pending actions on a fax job. The bit 0 is defined to be 'pending fax' bit,
 * it can only be set by fax service module, but is clearable by user. For a send/poll receive job, a set pending fax bit
 * indicates the fax is pending to be sent, for a received fax, it indicates a received fax is pending for user's
 * acknowledgment. When user clears this bit, for a sending/polling job, it effectively cancels a pending send job.
 * For a received fax, it would indicates user has read/printed/uploaded the fax file and the fax service
 * is free to clean the fax files associated with the job whenever it wants to do so.
 *
 * User are free to set/clear all other 15 bits (bit1-15) of a fax job's pending action flag.
 *
 * The pending action flag of a fax job can not be modified if the fax job is an ongoing fax.
 *
 */
uint32_t faxer_pending_action(void *handle, uint32_t job_id, uint16_t *act, uint16_t act_mask);

/**
 *\brief  Use this to obtain completed fax jobs
 *
 *  \param[in] handle   Fax service module handle
 *  \param[in] type     Type of fax activity to search
 *  \param[in] act      Bits of action to match
 *  \param[in] act_mask Bit mask for pending action
 *  \param index_set    Buffer to hold index of fax job that match setting
 *  \param size         Maximum number of found entries can be returned
 *
 *
 *  \return  Count of fax log that match search criteria
 *
 *  To get only number of fax jobs that matches search condition, leave index_set empty.
 *
 */
uint32_t faxer_find_fax(void *handle, FAXER_ACT_TYPE type_to_match, uint16_t tag_to_match,
                        uint32_t *index_set, uint16_t size);

/**\brief Clear files associated with a fax job
 *
 * \param[in] handle  Fax service module handle
 * \param[in] job_id  Job id of fax to be cleared
 * \param[in] force   Clear the fax even with pending flag(s) still on
 *
 * \return Completion code
 *
 * When a fax job is cleared, its fax log is not removed, only the TIFF file and other trace files related to it
 * are removed. Normally, a fax job can not  be cleared unless there are no pending actions with the job, user can
 * override this by having force flag set.
 **/
uint32_t faxer_clear_fax(void *handle, uint32_t job_id, bool force);

/**\brief Checks storage usage, attempts to reclaim storage if needed.
 *
 * \param[in] handle  Fax service module handle
 * \param[in] bytes_needed Bytes needed for storage.
 *
 * \return Completion code
 *
 * Checks the storage of the fax files.  If bytes needed is available, nothing happens and
 * 0 is returned.  If bytes needed is not available, the last fax job file found is deleted.
 * If, after removing all files, space is still not available an error is returned.
 **/
uint32_t faxer_free_storage(void *handle, int bytes_needed);

/**\brief Get activity log
 *
 * \param[in]  handle  Fax service module handle
 * \param[in]  job_id  Fax job id
 * \param[out] entry   Buffer to hold returned job info
 *
 * \return Completion code
 */
uint32_t faxer_get_log(void *handle, uint32_t job_id, FAXER_ACTIVITY_ENTRY *entry);


/**\brief Retrieve fax session status info
 *
 * \param[in]  handle Fax service module handle
 * \param[in]  id     Id of session data to get
 * \param[out] buf    Buffer to hold obtained session data
 *
 * \return Completion code
 */
uint32_t faxer_get_session_data(void *handle, FAXER_SESSION_DATA id, void *buf);


/**\brief Get a fax configuration setting.
 * 
 * \param[in]  handle  Fax service module handle
 * \param[in]  setting Fax setting enum
 * \param[in]  buf     Buffer to store returned configuration setting value
 * \param[in]  buf_size Size of buf in bytes
 * 
 * \return Completion code
 * 
 */
uint32_t faxer_config_get_setting(void *handle, FAXER_SETTING setting, void *buf, int buf_size);

/**\brief Set a fax configuration setting
 * 
 * \param[in] handle Fax service module handle
 *
 * \param[in] setting  Fax setting enum
 * 
 * \param[in] val      Buffer that holds the new setting value \n
 *              
 * \return Completion code
 **/
uint32_t faxer_config_set_setting(void *handle, FAXER_SETTING setting, void *val);

/**\brief Restore fax settings to their default values
 *
 * \param[in] handle fax service module handle
 * 
 * \return Completion code
 *
 * User calls this function to restore the fax system to its default country-specific settings, a 
 * power-cycle is usually required after this function is called.
 */
uint32_t faxer_config_restore(void *handle);


#endif
