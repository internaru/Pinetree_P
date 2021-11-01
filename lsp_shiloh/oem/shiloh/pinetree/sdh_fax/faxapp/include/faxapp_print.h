
#ifndef __FAXAPP_PRINT__
#define __FAXAPP_PRINT__


#include "SDHFaxCommonHeader.h"


#define _DBG(fmt, ...) dbg_msg("FAXAPP", fmt, ## _VA_ARGS_ )

#define LINES_IN_STRIP      64 ///< use 64 lines per strip for printing.
#define MONO_PRINT_RES      300
#define JPEG_PRINT_MAX_RES  300 /* can change to 200 for some images */
#define STRIP_LINES_FOR_FAX_RX_HDR_200 40  ///< The number of lines for the fax receive header 200 DPI
#define STRIP_LINES_FOR_FAX_RX_HDR_300 60  ///< The number of lines for the fax receive header 300 DPI
#define HEADER_NAME_LEN      ((FAXER_ALPHA_NUMERIC_MAX_STRING_LENGTH * FAXER_UTF8_BYTES_PER_CHAR) + 1)
#define CNV_RESOLUTION FAX_RES_300x300  ///< This is the resolution we want from rdr
#define MAX_NUM_STRIPS     80 //((300*17)/LINES_IN_STRIP)
#define COLOR_PRINT_COMPS (3) /* RGB */
#define GRAY_PRINT_COMPS  (1)

/* Page status...defined here since tiff lib doesn't have return types defined.  */
#define SUCCESS     0
#define PAGE_BREAK  1
#define DOC_END     2
#define PAGE_ERROR  4

/* Errors returned when retrieving data */
#define NP_ERROR      (-1)
#define NP_END_OF_DOC (1)
#define NP_MORE_DATA  (0)

/* internal messages */
//#define FAXAPP_MSG_NEXT_PAGE   (MSG_LAST_MSG +1)
//#define FAXAPP_MSG_CANCEL_DONE (MSG_LAST_MSG +2)

#define FAXPRINTTASK_QUEUE_SIZE     50

#define FAXAPP_PRINT_STACK_SIZE (MAX((1024 * 10 * 3), POSIX_MIN_STACK_SIZE))

#define PRINT_TASK_CONTROL_TOKEN 0x456723AB

#define START_JOB(job_type, module, addRsc) do{\
                     StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));\
                     XASSERT(StartIt != NULL,0);\
                     job_msg.msgType = MSG_STARTRECIPE;\
                     StartIt->Job = (job_type);\
                     StartIt->AdditionalResource = addRsc;\
                     StartIt->Pipe = NULL;\
                     StartIt->Wait = e_NoWait;\
                     StartIt->SendingModule = (module);\
                     job_msg.param3 = StartIt;\
                     SYMsgSend(SJMID, &job_msg);}while(0)

#define START_JOB_PIPE(job_type, module, addRsc, wait, pipe) do{\
                     StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));\
                     XASSERT(StartIt != NULL,0);\
                     job_msg.msgType = MSG_STARTRECIPE;\
                     StartIt->Job = (job_type);\
                     StartIt->AdditionalResource = addRsc;\
                     StartIt->Pipe = pipe;\
                     StartIt->Wait = wait;\
                     StartIt->SendingModule = (module);\
                     job_msg.param3 = StartIt;\
                     SYMsgSend(SJMID, &job_msg);}while(0)

#define END_JOB(resource, module)   do{\
                     job_msg.msgType = MSG_FREERECIPE;\
                     job_msg.param1 = 0;\
                     job_msg.param3 = (resource);\
                     SYMsgSend(SJMID, &job_msg);\
                     resource = NULL; }while(0)

#define CANCEL_JOB(resource, module) do{\
                     job_msg.msgType = MSG_CANCELJOB;\
                     job_msg.param1 = SYS_REQUSER;\
                     job_msg.param2 = 0;\
                     job_msg.param3 = 0;\
                     SYMsgSend( SJMID, &job_msg);}while(0)

#define ACK_CANCEL(resource, module) do{\
                     job_msg.msgType = MSG_CANCELJOB;\
                     job_msg.param1 = SYS_ACK;\
                     job_msg.param2 = module;\
                     job_msg.param3 = resource;\
                     SYMsgSend( SJMID, &job_msg);}while(0)


//
// Enum.
//
typedef enum {
  FAX_HEADER_TYPE_MONO,
  FAX_HEADER_TYPE_GRAYSCALE,
  FAX_HEADER_TYPE_RGB,
  FAX_HEADER_TYPE_RGBX,

} fax_header_type_t;

typedef enum {
  FAX_APP_RES_SUCCESS = 0,
  FAX_APP_RES_ERROR,  /* generic error */
  FAX_APP_RES_INVALID_PARAMS,
  FAX_APP_RES_LINE_UNAVAILABLE,
  FAX_APP_RES_FAX_ACTIVE,
  FAX_APP_RES_NO_FAX_TO_RECEIVE,
  FAX_APP_RES_FILE_DOES_NOT_EXIST,

} fax_app_res_t;

typedef enum {
    /** U.S. Letter Size*/
    FAX_PGSIZE_LETTER = 1,

    /** A4 size paper*/
    FAX_PGSIZE_A4 = 2,

    /** Legal */
    FAX_PGSIZE_LEGAL = 3,

} FAX_PAGE_SIZE;


typedef struct {
    bool incomplete;
    bool done;
    uint8_t page_status;
    uint32_t pageLineCnt;
    uint32_t output_byte_width;
    uint32_t width;
    uint32_t height;
    bool color;
    fax_header_type_t type;
} active_page_t;


typedef struct {
    unsigned int token;
    uint16_t scale_page;
    void *reader;
    int reader_pages;
    int max_pixels_per_row;
    int max_lines_per_page;
    int strip_index;
    JOBINFO job_info;
    mediasize_t DefMediaSize;
    CURRENT_RESOURCE *resource;
    unsigned int cur_page_num;
    bool stamp_faxes;
//    struct jpeg_decompress_struct jpeg_info;
    mlimiter_t *limiter;
    /* stored here to handle active pages that may have not completed due to
     lack of memory */
    active_page_t active_page;
    bool canceling;
    int pages_printed; /* used by reader task (print) */
} PRINT_TASK_CONTROL;


/* filled out and passed to print thread to initiate a print job */
typedef struct {
    bool in_use;
    void *context;
    uint32_t job_id;
//    fax_app_subjob_callback_t update_callback;
    void *pjob_handle;
    char *fname;
//    fax_store_handle_t store; /* for jpeg */
    PRINT_TASK_CONTROL pCtrl;
    int pages_to_print; /* number of pages sent */
    bool data_complete; /* all pages have been sent, only set by client */
} _print_job_info_t;


//
// Function Declaration
//
void FaxPrintJobMgrInit( void );
void FaxPrintJobResult(int result_fax_print, int printed_pages);
void faxapp_fax_print_start_job( void );
void faxapp_cancel_job( void );
void faxapp_send_data_to_printer (CURRENT_RESOURCE *job_resources, PAGEINFO *page_info);
int faxapp_print_cancel(void);
int faxapp_print_subsystem_cancel(void);


extern UINT8 fsu_get_resolution (UINT16 jnum);
extern UINT32 fsu_get_image_height_pixel (UINT16 jnum);
extern UINT16 fsu_get_doc_id(UINT16 jnum);
extern void de_activate_mem_to_prn (void);
#endif // #ifndef __FAXAPP_PRINT__

