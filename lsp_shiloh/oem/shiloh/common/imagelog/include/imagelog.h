/**
 *
 * ============================================================================
 * Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file image_log.c
 * \brief the source code of image log.
 */
#ifndef _IMAGE_LOG_H
#define _IMAGE_LOG_H

#include <stdint.h>
#include "print_job_types.h"
#include "hpdf.h"
#include "hpdf_doc.h"

//#define IL_ORIGINAL
//#define IMAGE_LOG_TEST_FOLDER
#ifdef IMAGE_LOG_TEST_FOLDER
#undef IMAGE_LOG_PATH
#define IMAGE_LOG_PATH "/tmp/upload"
#define IMAGE_LOG_SD_MOUNT_PATH "/tmp"
#else
#define IMAGE_LOG_PATH "/mnt/sd/upload"
#define IMAGE_LOG_SD_MOUNT_PATH "/mnt/sd"
#endif

#define CONVERT_600DPI (HPDF_REAL)((HPDF_REAL)600.0/(HPDF_REAL)72.0)
#define CONVERT_300DPI (HPDF_REAL)((HPDF_REAL)300.0/(HPDF_REAL)72.0)

void IL_get_raw_data(PLANED *plane);
HPDF_STATUS IL_PDF_NewDoc();
HPDF_STATUS IL_PDF_NewPage(int width, int height, HPDF_REAL convert_dpi);
HPDF_STATUS IL_PDF_DrawImage(HPDF_BYTE* rawdata, int width, int height, HPDF_REAL offset, int bpp, HPDF_REAL convert_dpi);
HPDF_STATUS IL_PDF_SaveDoc(int page_number);

#define META_END_WITH_JOBID

int set_metafile_start_info(char _jobtype, int _subJobType, int _jobid, char* _username);
#ifdef META_END_WITH_JOBID
int set_metafile_end_info(char _jobtype, int _subJobType, char* _destination, int _copy, char* _filename, int _originalpages, int _jobid);
#else
int set_metafile_end_info(char _jobtype, int _subJobType, char* _destination, int _copy, char* _filename, int _originalpages, int _jobid);
#endif

int get_current_timestamp(char* timestamp);
char* get_il_filename(int _jobid);
int get_PrintImageLogEnabled(void);

uint32_t GetImageLogStatus();

/**
* \brief Is Image Log Enabled or not for a specific job type
*
* \param jobtype char C: Copy, F: Fax, P: Print, S: Scan
* \param subJobType int For FAX-> 1: Fax Transfer 2: Fax Receive
*
*/
bool IsImageLogEnabled(char jobtype, int subJobType);

/**
* \brief check Image Log memory status and take action to
* cancel current job or not
*
* \param jobtype char C: Copy, F: Fax, P: Print, S: Scan
* \param subJobType int For FAX-> 1: Fax Transfer 2: Fax Receive
*
* \return int 0: Memory Available (no job calcelled) 1: Memory Full (send job cancelled to SJM)
*
*/
int CheckImgLogMemDoCancelAct(char jobtype, int subJobType);


typedef enum {
	IMAGELOG_FAX_SUBJOB_TYPE_NONE = 0,
	IMAGELOG_FAX_SUBJOB_TYPE_TX = 1,
	IMAGELOG_FAX_SUBJOB_TYPE_RX = 2,
} IMAGELOG_FAX_SUBJOB_TYPE;

int ARIA_encode(char *in_file, int crypt_flag);
int ARIA_do_encode(char *in_file, int crypt_flag);
int ARIA_encode_path(char *in_file, char *out_file, int crypt_flag, int file_option);
int ARIA_core(char *in_file, char *out_file, int crypt_flag);


#define FAX_NUM_LENGTH 26

#ifdef IL_MAIN_C
ATLISTENTRY il_fax_broadcast_list;
#else
extern ATLISTENTRY il_fax_broadcast_list;
#endif

typedef struct fax_broadcast
{
    int _jobid;
    char fax_num[FAX_NUM_LENGTH];
    int fax_page;
    int fax_result; //0:
    ATLISTENTRY ListNode;
}FAX_BROADCAST;


#endif // _IMAGE_LOG_H

