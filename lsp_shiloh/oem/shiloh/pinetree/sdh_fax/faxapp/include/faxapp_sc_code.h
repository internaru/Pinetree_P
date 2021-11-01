#ifndef __FAXAPP_SC_CODE__
#define __FAXAPP_SC_CODE__

typedef enum {
	FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_RESET = 500,
	FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_WRITE = 501,
	FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_MULTI_WRITE = 502,
	FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_OPEN_1 = 503,
	FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_OPEN_2 = 504,
	FAXAPP_SC_CODE_FAIL_TO_MODEM_SPI_INIT = 505,
	FAXAPP_SC_CODE_FAIL_TO_FAX_TOTAL_PAGE_SETTING = 506,
	FAXAPP_SC_CODE_FAIL_TO_FAX_RETRY_SETTING = 507,
	FAXAPP_SC_CODE_INVALID_FAX_TYPE = 508,
	FAXAPP_SC_CODE_FAX_PRINT_RESULT_ERROR = 509,
	FAXAPP_SC_CODE_FAIL_TO_MODEM_INIT = 510,
	FAXAPP_SC_CODE_FAIL_TO_DMB_TO_MODEM = 511,	
	FAXAPP_SC_CODE_FAIL_TO_JBIG_DECODE_1 = 512,
	FAXAPP_SC_CODE_FAIL_TO_JBIG_DECODE_2 = 513,
	FAXAPP_SC_CODE_FAIL_TO_TRANSMIT_T4_IMAGE_DATA = 514,
	FAXAPP_SC_CODE_FAIL_TO_CDB_DONE = 515,
	FAXAPP_SC_CODE_FAIL_TO_RX_PHASE_C_ENDOFPAGE = 516,
	FAXAPP_SC_CODE_FAIL_TO_CDB_WRITE_1 = 517,
	FAXAPP_SC_CODE_FAIL_TO_CDB_WRITE_2 = 518,
	FAXAPP_SC_CODE_FAIL_TO_CDB_READ = 519,
	FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_1 = 520,
	FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_2 = 521,
	FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_3 = 522,
	FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_4 = 523,
	FAXAPP_SC_CODE_FAIL_TO_SEND_IMAGE_RESCALING_5 = 524,
	//FAXAPP_SC_CODE_FAIL_TO_T4_DECODING = 525,
	FAXAPP_SC_CODE_T4_ENCODING_ERROR = 526,
	FAXAPP_SC_CODE_INVALID_SCAN_SOURCE = 527,
	FAXAPP_SC_CODE_FAIL_TO_T4_ENCODING = 528,
	FAXAPP_SC_CODE_INVALID_TX_CODING_TYPE = 529,
	FAXAPP_SC_CODE_INVALID_RX_TOTAL_PAGE_1 = 530,
	FAXAPP_SC_CODE_INVALID_RX_TOTAL_PAGE_2 = 531,
	FAXAPP_SC_CODE_FAIL_TO_CREATE_SHARED_MEMORY = 532,
	FAXAPP_SC_CODE_SHARED_MEMORY_ERROR = 533,
	FAXAPP_SC_CODE_ERROR_TRANSFERS_IMAGE_DATA_FROM_THE_HDLC_BUFFER = 534,
	FAXAPP_SC_CODE_ERROR_TRANSMITS_ONE_FCD_FRAME_OF_IMAGE_DATA = 535,
    FAXAPP_SC_CODE_FAIL_TO_READ_DB = 536,
    FAXAPP_SC_CODE_FAIL_TO_WRITE_DB = 537,
    FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB = 538,
    FAXAPP_SC_CODE_NO_VALID_DIAL_NO = 539,
    FAXAPP_SC_CODE_FAIL_TO_PRINT_PAGE_SETTING = 540,
    FAXAPP_SC_CODE_FAIL_TO_PRINT_IMAGE_SETTING = 541,

    FAXAPP_SC_CODE_LAST
} FAXAPP_SC_CODE_t;

#endif // #ifndef __FAXAPP_SC_CODE__