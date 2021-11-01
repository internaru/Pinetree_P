#ifndef SCANAPP_UISCAN_H
#define SCANAPP_UISCAN_H

#include "../../../../../common/scan/include/asp.h"
#include "../../../../../common/scan/src/scanmargin.h"
#include "scanapp_margins.h"
#include "SysSharedMemory.h"
#include "ATypes.h"
#include "commonutils.h"

#ifdef EXTERN
#undef EXTERN
#endif

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN   4096
#endif
#ifndef PTHREAD_CANCELED
#define PTHREAD_CANCELED    0
#endif

#define SCANAPP_UISCAN_QUEUE_SIZE   20                   ///< Number of messages in the scanner queue
#define SCANAPP_UISCAN_STACK_SIZE   PTHREAD_STACK_MIN*2  ///< The size of the stack.

#define FAX_SEND_FLAG					0x01000000
#define	SET_FAX_JOB_NO(n)				((n<<8)&0x00ffff00)
#define	GET_FAX_JOB_NO(n)				((n>>8)&0x0000ffff)
#define	GET_SCAN_JOB_NO(n)				((n)&0x000000ff)

#define	UI_SCAN_DUAL_PAGE_NUM			2
#define	USE_DEBUG_FUNC					1
#define	USE_SDH_A3_MERGE_ALGORITHM		1
#define MAX_UISCAN_JOB_COUNT			100
#define MAX_OUTPUT_FILENAME				255
#define	MAX_SYSTEM_CMD_LENGTH			255
#define MAX_LEGAL_MERGE_RESOLUTION		300

#define	MAX_SCAN_ADF_WIDTH_HINCHES		MECH_SPEC_JUNIPER_ADF_MAX_WIDTH_HINCHES
#define	MAX_SCAN_FB_HEIGHT_HINCHES 1156 // mod.kks. 1162 -> 1156
#define DEFAULT_BORDER_MM			3	//3mm
#define DEFAULT_BORDER_LEFT_MM		DEFAULT_BORDER_MM
#define DEFAULT_BORDER_RIGTH_MM	DEFAULT_BORDER_MM
#define DEFAULT_BORDER_TOP_MM		DEFAULT_BORDER_MM
#define DEFAULT_BORDER_BOTTOM_MM	DEFAULT_BORDER_MM

#define	SDH_USE_SEND_EMAIL_FUNCTION_TEST		0
#define	SDH_USE_SEND_FTP_FUNCTION_TEST		0
#define	SDH_USE_SEND_SMB_FUNCTION_TEST		0
#define	SDH_USE_SEND_WEBDAV_FUNCTION_TEST		0
#define	SDH_USE_SET_FILE_INFO					1
#define SDH_USE_ERASE_BORDER					1
#define	SDH_USE_PC_SCAN_CANCEL_BY_UI			0

#define PDF_SOURCE_COMPRESSED_MEMORY			0
#define	PDF_SOURCE_COMPRESSED_FILE				1

#define SDH_SCAN_COLOR_GAMMA_VAL		22
#define SDH_SCAN_GRAY_GAMMA_VAL		19

#define	UISCAN_MINIMUM_MEMSIZE	(10 * 1024)	//10M

void debug_make_jpg_test(void);
void debug_make_tif_test(void);
void debug_make_pdf_test(void);

void scanapp_notify_adf_status(UINT32 eventReason, UINT32 eventStatus);
bool scanner_check_adf_paper(void);
int scanner_job_setupMergeJob_scanArea(int jobType);
int scanner_job_setupMergeJob_forStart(int jobType);
int scanner_job_setupJobManage(int save_filesystem_type);
int scanner_job_clear_app_resource(void);
int scanner_job_get_current_working_folder(char *working_folder_path);

void scanner_pc_scan_input(uint32_t action);
void scanner_ui_scan_input(uint32_t action);
int scanner_erase_image_folder(char *folderPath);
int scanner_erase_image_file(char *filePath);
int scanner_make_image_folder(char *folderPath, bool removemode);
void scanner_make_image_folder_using_system_cmd(char *folderPath);
int scanner_make_end_file(char *fileFullPath, char *file_name);
void make_output_filename(char outfilename[], int fileformat, int page_number, scan_data_type dtype, uint8_t page_side);

int scanner_send_result(unsigned int sendResultJobId, unsigned int errorCode);
int scanner_send_image_files(int destination);
int scanner_send_to_email(void);
int scanner_send_to_ftp(int requestType);
int scanner_send_to_ftp_request_from_fax( char * ftpID, char* ftpPassword, char* ftpAddress, int ftpPort, char *foler_path, int jobNum );
int scanner_send_to_smb(int requestType);
int scanner_send_to_webdav(int requestType);

void scanner_backup_jpegFilename(uint8_t page_side, char *outfilename);

void scanner_pdf_write_pageImage_spdf(uint8_t page_side);
void scanner_pdf_open_spdf(uint8_t page_side );
void scanner_pdf_close_spdf(uint8_t page_side );

void scanner_tiff_set_field(uint8_t add_page_side);
void scanner_tiff_close(uint8_t page_side);

void scanner_jpeg_set_field(uint8_t page_side, FILE *outfile);
void scanner_jpeg_close(uint8_t page_side);
void scanner_jpeg_close_for_pdf(uint8_t page_side);

int scanner_service_update_margins(void);
int scanner_service_get_fb_margins(int dpi, scan_cmode_t cmode, struct scan_flatbed_margins *fb_margins, int32_t user_x, int32_t user_y, int32_t user_width, int32_t user_height);
int scanner_service_get_adf_margins(int dpi, scan_cmode_t cmode, struct scan_adf_margins *adf_margins, int32_t user_width, int32_t user_height);
int scanner_service_set_fb_margins(int dpi, int colorMode, int left, int top, int right, int bottom);
int scanner_service_set_adf_margins(int dpi, int colorMode, int first_tof,  int tof, int bof, int left, int tof_backside, int bof_backside);
int scanner_service_config_chagned(void);
int scanner_service_get_fb_wait_time(void);
int scanner_service_get_filesystem_usage(char *path);
int scanner_service_get_job_id(void);
int scanner_service_set_variable_with_current_time(char *variable);
int scanner_set_file_write_error(int errorType);
int scanner_clear_state_variable(void);
int scanner_imagelog_start(uint8_t page_side);

extern char *GetCurerntAuthUser(void);

typedef enum {
    UI_FILE_TIFF = 0,				//TIFF Single Page (divided into each files)
    UI_FILE_PDF,					//PDF Single Page (divided into each files)
    UI_FILE_JPEG,
    UI_FILE_SECURITY_PDF,
    UI_FILE_TIFF_MULTI_PAGE,	//TIFF Multi Page (into one file)
    UI_FILE_PDF_MULTI_PAGE,		//PDF Multi Page (into one file)
    UI_FILE_PGM,    				//For debug
    UI_FILE_LAST
} UI_FILE_FORMAT;

typedef enum {
	UI_FILE_PAGE_SINGLE = 0,
	UI_FILE_PAGE_MULTI,
	UI_FILE_PAGE_LAST	
} UI_FILE_PAGE_TYPE;

typedef enum {
    UI_FILE_DEST_USBTHUMB = 0,
    UI_FILE_DEST_EMAIL,
    UI_FILE_DEST_FTP,
    UI_FILE_DEST_SMB,
	UI_FILE_DEST_WEBDAV,
    UI_FILE_DEST_LAST
} UI_FILE_DESTINATION;

typedef enum {
    UI_SCANTYPE_COLOR = 0,
    UI_SCANTYPE_GRAY,
    UI_SCANTYPE_BW_TEXT,
    UI_SCANTYPE_BW_PHOTO,
    UI_SCANTYPE_LAST
} UI_SCAN_TYPE;

typedef enum {
	UI_SCAN_SOURCE_AUTO = 0,
	UI_SCAN_SOURCE_FB,
	UI_SCAN_SOURCE_ADF,
	UI_SCAN_SOURCE_ADF_DUPLEX_TOPTOP,
	UI_SCAN_SOURCE_ADF_DUPLEX_TOPBOTTOM
} UI_SCAN_SOURCE;

#define JPEG_LIB_QUALITY_SET_VAL_LOW		30
#define JPEG_LIB_QUALITY_SET_VAL_MID		60
#define JPEG_LIB_QUALITY_SET_VAL_HIGH		90

typedef enum {
	UI_SCAN_JPEG_QUALITY_LOW = 0,
	UI_SCAN_JPEG_QUALITY_MID,
	UI_SCAN_JPEG_QUALITY_HIGH
} UI_SCAN_JPEG_QUALITY;

typedef enum {
	UI_SCAN_MERGEJOB_NONE = 0,
	UI_SCAN_MERGEJOB_A3,
	UI_SCAN_MERGEJOB_11X17,
	UI_SCAN_MERGEJOB_LEGAL,
	UI_SCAN_MERGEJOB_LEGAL_2ND_PAGE
} UI_SCAN_ACTION_JOB_TYPE;

typedef enum {
    UI_SCAN_APP_IDLE					= 0,    ///< Scan is idle.
    UI_SCAN_APP_LOCKING         		= 1,    ///< We are in the process of getting the resources.
    UI_SCAN_APP_READY_TO_SCAN   	= 2,    ///< We have the resources and are ready to scan
    UI_SCAN_APP_SCANNING        		= 3,    ///< We are currently scanning.
    UI_SCAN_APP_WAIT_FOR_RELEASE	= 4,    ///< We are releasing the scan
    UI_SCAN_APP_CANCEL				= 5,    ///< In the process of canceling the scan.
    UI_SCAN_APP_TOCANCEL        		= 6,    ///< Timeout cancel
    UI_SCAN_APP_CANCELED        		= 7     ///< When the job is cancelled yet not released.
} UI_SCAN_APP_STATE;

typedef enum {
	SEND_REQUEST_NORMAL 			= 0,
	SEND_REQUEST_CONNECT_TEST	= 1,
	SEND_REQUEST_FAX_AUTH_JOB	= 2,
} UI_SCAN_SEND_REQUEST_TYPE;

typedef enum {
	SCAN_MERGE_IOCTL_PARAM_A3_TO_BACK = 0,
	SCAN_MERGE_IOCTL_PARAM_A3_EJECT = 1,
	SCAN_MERGE_IOCTL_PARAM_DLT_TO_BACK_LITTLE = 2,
	SCAN_MERGE_IOCTL_PARAM_DLT_TO_BACK = 3,
	SCAN_MERGE_IOCTL_PARAM_DLT_EJECT = 4,
} SCAN_MERGE_IOCTL_PARAM;

typedef enum {
	SCAN_PDF_IMAGE_SOURCE_FROM_JPEG_FILE = 0,
	SCAN_PDF_IMAGE_SOURCE_FROM_JPEG_MEMORY = 1,
	SCAN_PDF_IMAGE_SOURCE_FROM_8BPP_RAW_MEMORY = 2,
} SCAN_PDF_SOUCE_IMAGE_TYPE;

typedef enum {
	SCAN_SAVE_FILESYSTEM_TYPE_NONE = 0,
	SCAN_SAVE_FILESYSTEM_TYPE_USE_RAMDISK = 1,
	SCAN_SAVE_FILESYSTEM_TYPE_USE_THUMBDRIVE = 2,	
} SCAN_SAVE_FILESYSTEM_TYPE;

struct uiscan_members {
    uint32_t pixels_per_row_padded;
    uint32_t pixels_per_row;
    uint32_t total_rows;
    uint32_t bits_per_pixel;

    /* calculated from the above fields */
    uint32_t bytes_per_row;
    scan_cmode_t cmode;
    uint32_t gtotal;
};

struct uiscan_stats_t {
    /* count incoming messages */
    uint32_t sizes;
    uint32_t scan_starts;
    uint32_t scan_ends;
    uint32_t page_starts;
    uint32_t page_ends;
    uint32_t page_datas;
    uint32_t readies;

    /* total data rows received (across all channels) */
    uint32_t current_rows_index[2];
    uint8_t file_write_error;
};

typedef struct
{
	uint32_t uiScanJobNum;
	char uiScanJobTempFolderName[MAX_OUTPUT_FILENAME];
	uint8_t	uiScanSendJobType;
	uint32_t	systemJobID;
} UISCAN_JOB_MANAGER;

typedef struct scanOnePageBuf_s {
    uint8_t *buf;
    uint32_t bufsize_bytes;
    uint32_t write_index;
} scanOnePageBuf_t;

typedef struct SCAN_BORDER_PARAMETERS_t {
	int32_t userInputLeft;
	int32_t userInputRight;
	int32_t userInputTop;
	int32_t userInputBottom;
	int32_t eraseLeftPixel;
	int32_t eraseRightPixel;
	int32_t eraseTopPixel;
	int32_t eraseBottomPixel;	
} SCAN_BORDER_PARAMETERS;

typedef struct
{
	int32_t Gamma;
	int32_t Brightness;
	int32_t Contrast;
	int32_t Resolution;
	XY_SCALE_TYPE XYScale;
	uint32_t Sharp;
	/* davep 21-Nov-2011 ; deprecated 'Smooth' because redundant with 'Sharp' */
	uint32_t deprecated1;
	uint32_t BitsPerPixel;
	int32_t reserved1;
	int32_t reserved2;
	int32_t reserved3;
	uint32_t flags;			///< see SCAN_JOB_FLAGS_xxx
	uint32_t DataType;		///< see enum scan_data_type
	SCAN_DIMENSIONS scan_window;
	SCAN_DIMENSIONS ScannableArea;
	uint32_t ScanType;		///< see enum SCAN_TYPE

	//SINDOH Add Parameter.
	UI_SCAN_ACTION_JOB_TYPE	scanActionType;
	UI_FILE_FORMAT 	fileFormat;
	UI_FILE_PAGE_TYPE		pageType;
	uint8_t			jpegQuality;
	uint8_t			scanToDestination;
	uint16_t			sizecode;
	uint32_t			input_horizontal_adfOffset;
	uint8_t			scanSource;
	SCAN_BORDER_PARAMETERS eraseBorderParam;
	stShmSaveFile 	saveToFileInfo;
	stShmSendEmail 	sendToEmailInfo;
	stShmSendServer 	sendToServerInfo;
	bool				isSecurityDocument;
	bool						isPersonalScanJob;
} UISCAN_JOB_SETTINGS;

#endif // SCANAPP_UISCAN_H
