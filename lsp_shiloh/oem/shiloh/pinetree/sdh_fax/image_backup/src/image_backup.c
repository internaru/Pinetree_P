#include "SDHFaxCommonHeader.h"
#include "image_backup.h"
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include "lassert.h"
#include <math.h>
#include "../../fax_memmgr/include/fax_memAPI.h"


JOB_QUEUE_INFO_t FSUJobQueueInfo = { };

static char DelayTimeInJobQueueFile[MAX_LEN_DATE_TIME] = {0, };
static char DelayTimeInJobQueueFileReportMgr[MAX_LEN_DATE_TIME_YEAR + 1] = {0, };
static char CurrentTimeInJobQueueFileReportMgr[6] = {0, };
extern char JobQueueDelayTime[MAX_LEN_FILE_PATH_NAME];
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
static char FileNameInJobQueueFile[MAX_LEN_USER_ID + 1] = {0, };
#endif

extern char *pRootFolderName;
extern char *pRootFolderNameRAM;
extern char *pCurrentDirName;
extern char *pPreviousDirName;
extern UINT8 isSDCardEnable;
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
extern char gFaxUserId[MAX_LEN_USER_ID + 1];
#endif
extern int fax_scan_page_count;
#ifdef HAVE_IMAGELOG
extern int ARIA_encode(char *in_file, int crypt_flag);
#endif
extern UINT8 getImageBackup(void);

/************************************************************************
*  Function: check_job_queue_validation
*
*  Description:
*    return the valid number of remaining jobs in /DocMan/DocXXXX/ and
*    retrieve the job queue info. from Job Queue File (JobQueueXXXX.info)
*
*  Inputs:
*    none
*
*  Output:
*    valid number of remaining jobs
************************************************************************/
int check_job_queue_validation (void)
{
    UINT8 Len = 0;
    UINT8 jbgFileNo = 0;
    UINT8 photoFileNo = 0;
    UINT8 StartPageNo = 0;
    UINT8 TotalPageNo = 0;
    UINT16 IndexToJobNo = 0;
    UINT16 IndexToDocId = 0;
    int IndexToPageNo = 0;
    DIR *pDir = (DIR *)NULL;
    struct dirent *JobQueueInfoFile = NULL;
    char DocIdPathName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char jbgFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char photoFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueueFullFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    int JobQueueFile = 0;
    int ValidJobCnt = 0;

    // Step 1. Search for all the /DocMan/DocXXXX...
    for (IndexToDocId = 0; IndexToDocId < MAX_NUM_DOC_ID; IndexToDocId++)
    {
        snprintf(DocIdPathName, sizeof(DocIdPathName), "%s%cDoc%04X", pRootFolderName, PATH_SEPARATOR, IndexToDocId);

        pDir = opendir(DocIdPathName);

        // Step 2. Check to see whether the /DocMan/DocXXXX exists...
        if (pDir == (DIR *)NULL)
        {
            ; // No Action Here...
        }
        else
        {
            snprintf(JobQueueFileName, sizeof(JobQueueFileName), "JobQueue%04X.info", IndexToDocId);

            // Step 3. Save all file names to JbgFileName except for '.' or '..'
            while ( (JobQueueInfoFile = readdir(pDir)) != NULL )
            {
                // Step 3-1. Don't store '.' or '..' to JbgFileName
                if (  (strcmp (JobQueueInfoFile->d_name, pCurrentDirName) == 0) || (strcmp (JobQueueInfoFile->d_name, pPreviousDirName) == 0) )
                {
                    ; // No Action Here...
                }
                // Step 3-2. Check to see whether the JobQueueXXXX.info file exists...
                else if ( (strcmp (JobQueueInfoFile->d_name, JobQueueFileName) == 0) )
                {
                    JobQueueFile++;
                }
                // Step 3-3. Count faxXX.jbg or faxXX.photo
                else
                {
                    Len = strlen(JobQueueInfoFile->d_name);

                    if (Len == 9) // faxXX.jbg
                    {
                        jbgFileNo++;
                    }
                    else // faxXX.photo
                    {
                        photoFileNo++;
                    }
                }
            }

            // Step 4. Close the /DocMan/DocXXXX
            closedir(pDir);

            // Step 5. If JobQueueXXXX.info file exists...
            if (JobQueueFile != 0)
            {
                if (jbgFileNo != 0)
                {
                    UINT8 ret = 0;

                    // Step 6. Open JobQueueXXXX.info file and get the job queue info...
                    FSUJobQueueInfo.DocId[IndexToJobNo] = IndexToDocId;

                    get_job_queue_from_Job_Queue_File(IndexToJobNo);

                    /* Check for the validation of .jbg file(s) only in case of mem to print... */

                    if (FSUJobQueueInfo.JobType[IndexToJobNo] == IMAGE_BACKUP_JOB_TYPE_RCV_DONE)
                    {
                        if(getImageBackup() == 2) /*ImageBackup off*/
                        {
                            // Step 1. Purge .jbg files in NAND

                            StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_JBG);

                            TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_JBG);

                            for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                            {
                                snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                                fsu_delete_file_on_file_system(jbgFileName);
                            }

                            // Step 2. Purge .photo files in NAND if exists

                            StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                            TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                            for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                            {
                                snprintf(photoFileName, sizeof(photoFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                                if (access(photoFileName, F_OK) == 0)
                                {
                                    fsu_delete_file_on_file_system(photoFileName);
                                }
                            }

                            // Step 3. Delete Job Queue File in NAND

                            snprintf(JobQueueFullFileName, sizeof(JobQueueFullFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToDocId);

                            fsu_delete_file_on_file_system(JobQueueFullFileName);

                            // Step 4. Delete DocXXX Directory

                            fsu_delete_dir_on_file_system(DocIdPathName);

                            ret = 0;
                        }
                        else
                        {
                            ret = fsu_check_image_file_validation_receive(FSUJobQueueInfo.DocId[IndexToJobNo], FSUJobQueueInfo.Resolution[IndexToJobNo], FSUJobQueueInfo.ImageWidth[IndexToJobNo]);
                        }

                        if (ret == 1)
                        {
                            printf("\n(YHCHO) %s() - Image Backup Check Point [1] - Valid Case...\n", __FUNCTION__);

                            ValidJobCnt++;
                            IndexToJobNo++;
                        }
                        else
                        {
                            printf("\n(YHCHO) %s() - Image Backup Check Point [2] - Abnormal Case...\n", __FUNCTION__);
                        }
                    }
                    else
                    {
                        if(getImageBackup() != 0) /*ImageBackup all*/
                        {
                            ret = 0;
                        }
                        else
                        {
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
                            if(FSUJobQueueInfo.Permission[IndexToJobNo] == SOLUTION_PERMISSION_STATE_DISABLE)
                            {
                                ret = 0;
                            }
                                else
                            {
                                ret = fsu_check_image_file_validation_transmit(FSUJobQueueInfo.DocId[IndexToJobNo]);
                            }
#else
                            ret = fsu_check_image_file_validation_transmit(FSUJobQueueInfo.DocId[IndexToJobNo]);
#endif
                        }

                        if (ret == 1)
                        {
                            printf("\n(YHCHO) %s() - Image Backup Check Point [3] - Valid Case...\n", __FUNCTION__);

                            ValidJobCnt++;
                            IndexToJobNo++;
                        }
                        else
                        {
                            printf("\n(YHCHO) %s() - Image Backup Check Point [4] - Abnormal Case...\n", __FUNCTION__);

                            // Step 1. Purge .jbg files in NAND

                            StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_JBG);

                            TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_JBG);

                            for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                            {
                                snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                                fsu_delete_file_on_file_system(jbgFileName);
                            }

                            // Step 2. Purge .photo files in NAND if exists

                            StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                            TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                            for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                            {
                                snprintf(photoFileName, sizeof(photoFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                                if (access(photoFileName, F_OK) == 0)
                                {
                                    fsu_delete_file_on_file_system(photoFileName);
                                }
                            }

                            // Step 3. Delete Job Queue File in NAND

                            snprintf(JobQueueFullFileName, sizeof(JobQueueFullFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToDocId);

                            fsu_delete_file_on_file_system(JobQueueFullFileName);

                            // Step 4. Delete DocXXX Directory

                            fsu_delete_dir_on_file_system(DocIdPathName);
                        }
                    }
                }
                else
                {
                    if (photoFileNo != 0)
                    {
                        printf("\n(YHCHO) %s() - Image Backup Check Point [5] - Abnormal Case...\n", __FUNCTION__);

                        // Step 1. Purge .photo files in NAND if exists

                        TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                        StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                        for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                        {
                            snprintf(photoFileName, sizeof(photoFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                            fsu_delete_file_on_file_system(photoFileName);
                        }

                        // Step 2. Delete Job Queue File in NAND

                        snprintf(JobQueueFullFileName, sizeof(JobQueueFullFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToDocId);

                        fsu_delete_file_on_file_system(JobQueueFullFileName);

                        // Step 3. Delete DocXXX Directory

                        fsu_delete_dir_on_file_system(DocIdPathName);
                    }
                    else
                    {
                        printf("\n(YHCHO) %s() - Image Backup Check Point [6] - Abnormal Case...\n", __FUNCTION__);

                        // Step 1. Delete Job Queue File in NAND

                        snprintf(JobQueueFullFileName, sizeof(JobQueueFullFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToDocId);

                        fsu_delete_file_on_file_system(JobQueueFullFileName);

                        // Step 2. Delete DocXXX Directory

                        fsu_delete_dir_on_file_system(DocIdPathName);
                    }
                }
            }
            else
            {
                if (jbgFileNo != 0)
                {
                    if (photoFileNo != 0)
                    {
                        printf("\n(YHCHO) %s() - Image Backup Check Point [7] - Abnormal Case...\n", __FUNCTION__);

                        // Step 1. Purge .jbg files in NAND

                        StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_JBG);

                        TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_JBG);

                        for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                        {
                            snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                            fsu_delete_file_on_file_system(jbgFileName);
                        }

                        // Step 2. Purge .photo files in NAND if exists

                        TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                        StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                        for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                        {
                            snprintf(photoFileName, sizeof(photoFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                            fsu_delete_file_on_file_system(photoFileName);
                        }

                        // Step 3. Delete DocXXX Directory

                        fsu_delete_dir_on_file_system(DocIdPathName);
                    }
                    else
                    {
                        printf("\n(YHCHO) %s() - Image Backup Check Point [8] - Abnormal Case...\n", __FUNCTION__);

                        // Step 1. Purge .jbg files in NAND

                        StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_JBG);

                        TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_JBG);

                        for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                        {
                            snprintf(jbgFileName, sizeof(jbgFileName), "%s%cDoc%04X%cfax%02X.jbg", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                            fsu_delete_file_on_file_system(jbgFileName);
                        }

                        // Step 2. Delete DocXXX Directory

                        fsu_delete_dir_on_file_system(DocIdPathName);
                    }
                }
                else
                {
                    if (photoFileNo != 0)
                    {
                        printf("\n(YHCHO) %s() - Image Backup Check Point [9] - Abnormal Case...\n", __FUNCTION__);

                        // Step 1. Purge .photo files in NAND if exists

                        TotalPageNo = fsu_get_total_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                        StartPageNo = fsu_get_start_page_no_in_mem(IndexToDocId, FAX_FILE_TYPE_PHOTO);

                        for (IndexToPageNo = StartPageNo; IndexToPageNo < StartPageNo + TotalPageNo; IndexToPageNo++)
                        {
                            snprintf(photoFileName, sizeof(photoFileName), "%s%cDoc%04X%cfax%02X.photo", pRootFolderName, PATH_SEPARATOR, IndexToDocId, PATH_SEPARATOR, IndexToPageNo);

                            fsu_delete_file_on_file_system(photoFileName);
                        }

                        // Step 2. Delete DocXXX Directory

                        fsu_delete_dir_on_file_system(DocIdPathName);
                    }
                    else
                    {
                        printf("\n(YHCHO) %s() - Image Backup Check Point [10] - Abnormal Case...\n", __FUNCTION__);

                        // Step 1. Delete DocXXX Directory

                        fsu_delete_dir_on_file_system(DocIdPathName);
                    }
                }
            }

            jbgFileNo = 0;
            photoFileNo = 0;
            JobQueueFile = 0;
        } // if (pDir == (DIR *)NULL)
    } // for (IndexToDocId = 0; IndexToDocId < MAX_NUM_DOC_ID; IndexToDocId++)

    return ValidJobCnt;
}


void get_job_queue_from_Job_Queue_File (UINT16 jnum)
{
    UINT16 DocId = 0;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    int len = 13;
    char *pJobType =       "JobType      ";
    char *pDialNo =        "DialNo       ";
    char *pTimeToTx =      "TimeToTx     ";
    char *pResolution =    "Resolution   ";
    char *pImageWidth =    "ImageWidth   ";
    char *pImageHeight =   "ImageHeight  ";
    char *pPhotoMode =     "PhotoMode    ";
    char *pOriginalDocId = "OriginalDocId";
    char *pRetransmit    = "Retransmit   ";
    char *pPageInfo      = "PageInfo.    ";
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    char *pFileName      = "FileName     ";
    char *pPermission    = "Permission   ";
    char *pUniqueJobId   = "UinqueJobId  ";
    char *pScanInfo      = "ScanInfo     ";
#endif
    DocId = FSUJobQueueInfo.DocId[jnum];

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return ;
    }
    else
    {
        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, len);

            if (strcmp (Temp, pJobType) == 0)     // Step 2-1-1. Get JobType
            {
                FSUJobQueueInfo.JobType[jnum] = get_job_type_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pDialNo) == 0)     // Step 2-1-2. Get Dial No.
            {
                if ( FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING_POLL )
                {
                    ; // No Action Here...
                }
                else
                {
                    get_dial_no_from_Job_Queue_File(jnum, JobQueue);
                }
            }
            else if (strcmp (Temp, pTimeToTx) == 0)     // Step 2-1-3. Get Time To Tx
            {
                if ( (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_RCV_DONE) 
					|| (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING_POLL) )
                {
                    ; // No Action Here...
                }
                else
                {
                    if (FSUJobQueueInfo.JobType[jnum] == IMAGE_BACKUP_JOB_TYPE_TX_PENDING_DELAY)
                    {
                        FSUJobQueueInfo.TimeDiff[jnum] = get_time_diff_from_Job_Queue_File(JobQueue);
                    }
                    else
                    {
                        FSUJobQueueInfo.TimeDiff[jnum] = TRANSMIT_TIME_RIGHT_NOW; // Transmit Now
                    }
                }
            }
            else if (strcmp (Temp, pResolution) == 0)     // Step 2-1-4. Get Resolution
            {
                FSUJobQueueInfo.Resolution[jnum] = get_resolution_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pImageWidth) == 0)     // Step 2-1-5. Get ImageWidth
            {
                FSUJobQueueInfo.ImageWidth[jnum] = get_image_width_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pImageHeight) == 0)     // Step 2-1-6. Get ImageHeight
            {
                FSUJobQueueInfo.ImageHeight[jnum] = get_image_height_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pPhotoMode) == 0)     // Step 2-1-7. Get PhotoMode
            {
                FSUJobQueueInfo.PhotoMode[jnum] = get_photo_mode_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pOriginalDocId) == 0)     // Step 2-1-8. Get OriginalDocId
            {
                FSUJobQueueInfo.OriginalDocId[jnum] = get_original_id_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pRetransmit) == 0)     // Step 2-1-9. Get Retransmit
            {
                FSUJobQueueInfo.Retransmit[jnum] = get_retransmit_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pPageInfo) == 0)     // Step 2-1-10. Page Info.
            {
                FSUJobQueueInfo.CurrentPageNo[jnum] = get_current_page_no_from_Job_Queue_File(JobQueue);

                FSUJobQueueInfo.TotalPageNo[jnum] = get_total_page_no_from_Job_Queue_File(JobQueue);
            }
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
            else if (strcmp (Temp, pFileName) == 0)     // Step 2-1-10. Page Info.
            {
                get_file_name_from_Job_Queue_File(jnum, JobQueue);
            }
            else if (strcmp (Temp, pPermission) == 0)     // Step 2-1-10. Page Info.
            {
                FSUJobQueueInfo.Permission[jnum] = get_permission_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pUniqueJobId) == 0)     // Step 2-1-10. Page Info.
            {
                FSUJobQueueInfo.UniqueJobId[jnum] = get_unique_job_id_from_Job_Queue_File(JobQueue);
            }
            else if (strcmp (Temp, pScanInfo) == 0)     // Step 2-1-10. Scan Info.
            {
                get_scan_info_from_Job_Queue_File(&FSUJobQueueInfo.ScanInfo[jnum], JobQueue);
            }
#endif
        }

        printf("\n(YHCHO) %s() - FSUJobQueueInfo.DocId[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.DocId[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.JobType[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.JobType[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.DialNo[%d]: %s", __FUNCTION__, jnum, FSUJobQueueInfo.DialNo[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.TimeDiff[%d]: %ld", __FUNCTION__, jnum, FSUJobQueueInfo.TimeDiff[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.Resolution[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.Resolution[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.ImageWidth[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.ImageWidth[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.ImageHeight[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.ImageHeight[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.PhotoMode[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.PhotoMode[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.OriginalDocId[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.OriginalDocId[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.Retransmit[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.Retransmit[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.CurrentPageNo[%d]: %d", __FUNCTION__, jnum, FSUJobQueueInfo.CurrentPageNo[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.TotalPageNo[%d]: %d\n", __FUNCTION__, jnum, FSUJobQueueInfo.TotalPageNo[jnum]);
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.FileName[%d]: %s", __FUNCTION__, jnum, FSUJobQueueInfo.FileName[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.Permission[%d]: %d\n", __FUNCTION__, jnum, FSUJobQueueInfo.Permission[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.UniqueJobId[%d]: %ld\n", __FUNCTION__, jnum, FSUJobQueueInfo.UniqueJobId[jnum]);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.paperSize[%d]: %d\n", __FUNCTION__, jnum, FSUJobQueueInfo.ScanInfo[jnum].paperSize);
        printf("\n(YHCHO) %s() - FSUJobQueueInfo.duplex[%d]: %d\n", __FUNCTION__, jnum, FSUJobQueueInfo.ScanInfo[jnum].duplex);
#endif
        // Step 3. Close JobQueueXXXX.info..
        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }
}


int get_job_type_from_Job_Queue_File(char *JobQueue)
{
    int job_type = 0;
    int len = 0;

    len = strlen(JobQueue);

    job_type = JobQueue[len-2] - '0';

    return job_type;
}


int get_no_of_recipient_from_Job_Queue_File(char *JobQueue)
{
    int no_of_recipient = 0;
    int len = 0;
    int temp_val1 = 0;
    int temp_val2 = 0;

    len = strlen(JobQueue);

    temp_val1 = fsu_convert_char_to_int(JobQueue[len-3]);
    temp_val2 = JobQueue[len-2] - '0';

    no_of_recipient = temp_val1 * 10 + temp_val2;

    //printf("\n(YHCHO) %s() - no_of_recipient: %d\n", __FUNCTION__, no_of_recipient);

    return no_of_recipient;
}


void get_dial_no_from_Job_Queue_File(UINT16 jnum, char *JobQueue)
{
    int IndexToDialNo = 0;
    int IndexToJobQueue = 0;
    int len = 0;

    len = strlen(JobQueue);

    for (IndexToDialNo = 15; IndexToDialNo < (len - 1); IndexToDialNo++)
    {
        FSUJobQueueInfo.DialNo[jnum][IndexToJobQueue] = JobQueue[IndexToDialNo];

        IndexToJobQueue++;
    }

    FSUJobQueueInfo.DialNo[jnum][IndexToJobQueue] = 0;
}


long int get_time_diff_from_Job_Queue_File(char *JobQueue)
{
    UINT8 Len = 0;
    char *pJobQueueTime = (char *)NULL;
    long int DiffInSec = 0;
    time_t t;
    struct tm *pt;
    typedef struct {
        int Year;
        int Month;
        int Day;
        int Hour;
        int Min;
    } DELAY_TIME_t;

    UINT8 Days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    UINT8 isLeapYear = 0;

    DELAY_TIME_t LocalTime = { };
    DELAY_TIME_t JobQueueStartTime = { };
    DELAY_TIME_t Diff = { };
    DELAY_TIME_t CarryField = { };

    Len = strlen(JobQueue);

    pJobQueueTime = (char *)MEM_MALLOC(Len + 1);

    if (pJobQueueTime == (char *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }

    if (memcpy(pJobQueueTime, JobQueue, Len + 1) == (void *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }

    time(&t);

    pt = localtime(&t);

    if (pt == NULL)
    {
        LocalTime.Year = 1900;
        LocalTime.Month = 1;
        LocalTime.Day = 1;
        LocalTime.Hour = 0;
        LocalTime.Min = 0;
    }
    else
    {
        LocalTime.Year = pt->tm_year + 1900;
        LocalTime.Month = pt->tm_mon + 1;
        LocalTime.Day = pt->tm_mday;
        LocalTime.Hour = pt->tm_hour;
        LocalTime.Min = pt->tm_min;
    }

    JobQueueStartTime.Year = fsu_convert_char_to_int(pJobQueueTime[15]) * pow(10, 3) + fsu_convert_char_to_int(pJobQueueTime[16]) * pow(10, 2) + fsu_convert_char_to_int(pJobQueueTime[17]) * pow(10, 1) + fsu_convert_char_to_int(pJobQueueTime[18]) * pow(10, 0);
    JobQueueStartTime.Month = fsu_convert_char_to_int(pJobQueueTime[20]) * pow(10, 1) + fsu_convert_char_to_int(pJobQueueTime[21]) * pow(10, 0);
    JobQueueStartTime.Day = fsu_convert_char_to_int(pJobQueueTime[23]) * pow(10, 1) + fsu_convert_char_to_int(pJobQueueTime[24]) * pow(10, 0);
    JobQueueStartTime.Hour = fsu_convert_char_to_int(pJobQueueTime[26]) * pow(10, 1) + fsu_convert_char_to_int(pJobQueueTime[27]) * pow(10, 0);
    JobQueueStartTime.Min = fsu_convert_char_to_int(pJobQueueTime[29]) * pow(10, 1) + fsu_convert_char_to_int(pJobQueueTime[30]) * pow(10, 0);

    MEM_FREE_AND_NULL(pJobQueueTime);

    if ( (LocalTime.Year % 4 == 0) && ((LocalTime.Year % 100 != 0) || (LocalTime.Year % 400 == 0)) )
    {
        isLeapYear = 1;
    }
    else
    {
        isLeapYear = 0;
    }

    //printf("\n(YHCHO) isLeapYear: %d\n", isLeapYear);

    if (isLeapYear == 1)
    {
        Days[2] = 29;
    }
    else
    {
        Days[2] = 28; // fail-safe
    }

    //printf("\n(YHCHO) -     Start Time: %04d.%02d.%02d %02d:%02d", JobQueueStartTime.Year, JobQueueStartTime.Month, JobQueueStartTime.Day, JobQueueStartTime.Hour, JobQueueStartTime.Min);
    //printf("\n(YHCHO) - LocalTime Time: %04d.%02d.%02d %02d:%02d\n", LocalTime.Year, LocalTime.Month, LocalTime.Day, LocalTime.Hour, LocalTime.Min);

    if (JobQueueStartTime.Min - LocalTime.Min >= 0)
    {
        Diff.Min = JobQueueStartTime.Min - LocalTime.Min;
    }
    else
    {
        Diff.Min = 60 + JobQueueStartTime.Min - LocalTime.Min;

        CarryField.Hour++;
    }

    if (JobQueueStartTime.Hour - LocalTime.Hour - CarryField.Hour >= 0)
    {
        Diff.Hour = JobQueueStartTime.Hour - LocalTime.Hour - CarryField.Hour;
    }
    else
    {
        Diff.Hour = 24 + JobQueueStartTime.Hour - LocalTime.Hour - CarryField.Hour;

        CarryField.Day++;
    }

    if (JobQueueStartTime.Day - LocalTime.Day - CarryField.Day >= 0)
    {
        Diff.Day = JobQueueStartTime.Day - LocalTime.Day - CarryField.Day;
    }
    else
    {
        Diff.Day = Days[LocalTime.Month] + JobQueueStartTime.Day - LocalTime.Day - CarryField.Day;

        CarryField.Month++;
    }

    if (JobQueueStartTime.Month - LocalTime.Month - CarryField.Month >= 0)
    {
        Diff.Month = JobQueueStartTime.Month - LocalTime.Month - CarryField.Month;
    }
    else
    {
        Diff.Month = 12 + JobQueueStartTime.Month - LocalTime.Month - CarryField.Month;

        CarryField.Year++;
    }

    Diff.Year = JobQueueStartTime.Year - LocalTime.Year - CarryField.Year;

    printf("\n(YHCHO) Diff.Year: %d", Diff.Year);
    printf("\n(YHCHO) Diff.Month: %d", Diff.Month);
    printf("\n(YHCHO) Diff.Day: %d", Diff.Day);
    printf("\n(YHCHO) Diff.Hour: %d", Diff.Hour);
    printf("\n(YHCHO) Diff.Min: %d\n", Diff.Min);

    DiffInSec = fsu_calculate_time_diff_in_sec(Diff.Year, Diff.Month, Diff.Day, Diff.Hour, Diff.Min);

    printf("\n(YHCHO) %s() - DiffInSec: %ld\n", __FUNCTION__, DiffInSec);

    return DiffInSec;
}


UINT8 get_resolution_from_Job_Queue_File(char *JobQueue)
{
    UINT8 ret_val = 0;
    int len = 0;

    len = strlen(JobQueue);

    ret_val = JobQueue[len-2] - '0';

    return ret_val;
}


UINT16 get_image_width_from_Job_Queue_File(char *JobQueue)
{
    unsigned short image_width = 0;
    int First = 0;
    int Second = 0;
    int Third = 0;
    int Fourth = 0;
    int len = 0;

    len = strlen(JobQueue);

    First = fsu_convert_char_to_int(JobQueue[len-5]);

    Second = fsu_convert_char_to_int(JobQueue[len-4]);

    Third = fsu_convert_char_to_int(JobQueue[len-3]);

    Fourth = fsu_convert_char_to_int(JobQueue[len-2]);

    image_width = First * 1000 + Second * 100 + Third * 10 + Fourth;

    return image_width;
}


UINT16 get_image_height_from_Job_Queue_File(char *JobQueue)
{
    unsigned short image_height = 0;
    int First = 0;
    int Second = 0;
    int Third = 0;
    int Fourth = 0;
    int len = 0;

    len = strlen(JobQueue);

    First = fsu_convert_char_to_int(JobQueue[len-5]);

    Second = fsu_convert_char_to_int(JobQueue[len-4]);

    Third = fsu_convert_char_to_int(JobQueue[len-3]);

    Fourth = fsu_convert_char_to_int(JobQueue[len-2]);

    image_height = First * 1000 + Second * 100 + Third * 10 + Fourth;

    return image_height;
}


UINT8 get_photo_mode_from_Job_Queue_File(char *JobQueue)
{
    int ret_val = 0;
    int len = 0;

    len = strlen(JobQueue);

    ret_val = JobQueue[len-2] - '0';

    return ret_val;
}


UINT16 get_original_id_from_Job_Queue_File(char *JobQueue)
{
    UINT16 original_doc_id = 0;
    int First = 0;
    int Second = 0;
    int Third = 0;
    int Fourth = 0;
    int len = 0;

    len = strlen(JobQueue);

    First = fsu_convert_char_to_int(JobQueue[len-5]);

    Second = fsu_convert_char_to_int(JobQueue[len-4]);

    Third = fsu_convert_char_to_int(JobQueue[len-3]);

    Fourth = fsu_convert_char_to_int(JobQueue[len-2]);

    original_doc_id = First * pow(16, 3) + Second * pow(16, 2) + Third * pow(16, 1) + Fourth * pow(16, 0);

    return original_doc_id;
}


UINT8 get_retransmit_from_Job_Queue_File(char *JobQueue)
{
    int retransmit = 0;
    int len = 0;

    len = strlen(JobQueue);

    retransmit = JobQueue[len-2] - '0';

    return retransmit;
}


UINT16 get_current_page_no_from_Job_Queue_File(char *JobQueue)
{
    UINT16 current_page_no = 0;
    UINT8 First = 0;
    UINT8 Second = 0;
    UINT8 Third = 0;
    int len = 0;

    len = strlen(JobQueue);

    First = fsu_convert_char_to_int(JobQueue[len-8]);

    Second = fsu_convert_char_to_int(JobQueue[len-7]);

    Third = fsu_convert_char_to_int(JobQueue[len-6]);

    current_page_no = First * 100 + Second * 10 + Third;

    return current_page_no;
}


UINT16 get_total_page_no_from_Job_Queue_File(char *JobQueue)
{
    UINT16 total_page_no = 0;
    UINT8 First = 0;
    UINT8 Second = 0;
    UINT8 Third = 0;
    int len = 0;

    len = strlen(JobQueue);

    First = fsu_convert_char_to_int(JobQueue[len-4]);

    Second = fsu_convert_char_to_int(JobQueue[len-3]);

    Third = fsu_convert_char_to_int(JobQueue[len-2]);

    total_page_no = First * 100 + Second * 10 + Third;

    return total_page_no;
}


UINT16 fsu_request_get_original_doc_id_from_Job_Queue_File (UINT16 DocId)
{
    UINT8 Len = 0;
    UINT8 StringLength = 13;
    UINT16 OriginalDocId = 0;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char *pOriginalDocId = "OriginalDocId";

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        UINT8 First = 0;
        UINT8 Second = 0;
        UINT8 Third = 0;
        UINT8 Fourth = 0;

        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, StringLength);

            if (strcmp (Temp, pOriginalDocId) == 0)
            {
                Len = strlen(JobQueue);

                First = fsu_convert_char_to_int(JobQueue[Len-5]);

                Second = fsu_convert_char_to_int(JobQueue[Len-4]);

                Third = fsu_convert_char_to_int(JobQueue[Len-3]);

                Fourth = fsu_convert_char_to_int(JobQueue[Len-2]);

                OriginalDocId = First * pow(16, 3) + Second * pow(16, 2) + Third * pow(16, 1) + Fourth * pow(16, 0);

                break;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }

    return OriginalDocId;
}


UINT8 fsu_request_get_retransmit_from_Job_Queue_File (UINT16 DocId)
{
    UINT8 Len = 0;
    UINT8 StringLength = 13;
    UINT8 Retransmit = 0;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char *pRetransmit = "Retransmit   ";

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif

    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, StringLength);

            if (strcmp (Temp, pRetransmit) == 0)
            {
                Len = strlen(JobQueue);

                Retransmit = fsu_convert_char_to_int(JobQueue[Len-2]);

                break;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }

    return Retransmit;
}


UINT8 fsu_requset_get_current_page_no_from_Job_Queue_File (UINT16 jnum)
{
    UINT8 Len = 0;
    UINT8 StringLength = 13;
    UINT8 CurrentPageNo = 0;
    UINT16 DocId = 0;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char *pPageInfo = "PageInfo.    ";

    DocId = fsu_get_doc_id(jnum);

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, StringLength);

            if (strcmp (Temp, pPageInfo) == 0)
            {
                UINT8 First = 0;
                UINT8 Second = 0;
                UINT8 Third = 0;

                Len = strlen(JobQueue);

                First = fsu_convert_char_to_int(JobQueue[Len-8]);

                Second = fsu_convert_char_to_int(JobQueue[Len-7]);

                Third = fsu_convert_char_to_int(JobQueue[Len-6]);

                CurrentPageNo = First * 100 + Second * 10 + Third;

                break;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }

    return CurrentPageNo;
}


UINT8 fsu_request_get_total_page_no_from_Job_Queue_File (UINT16 DocId)
{
    UINT8 First = 0;
    UINT8 Second = 0;
    UINT8 Third = 0;
    UINT8 Len = 0;
    UINT8 StringLength = 13;
    UINT8 TotalPageNo = 0;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char *pPagInfo = "PageInfo.    ";

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, StringLength);

            if (strcmp (Temp, pPagInfo) == 0)
            {
                Len = strlen(JobQueue);

                Third = fsu_convert_char_to_int(JobQueue[Len-4]);

                Second = fsu_convert_char_to_int(JobQueue[Len-3]);

                First = fsu_convert_char_to_int(JobQueue[Len-2]);

                TotalPageNo = Third * 100 + Second * 10 + First;

                break;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }

    return TotalPageNo;
}


char *fsu_request_get_time_to_tx_from_Job_Queue_File (UINT16 DocId)
{
    UINT8 Len = 0;
    UINT8 StringLength = 13;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char TimeToTx[12] = {0, }; // e.g.) 05.20 14:25
    char *pTimeToTx = "TimeToTx     ";

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, StringLength);

            if (strcmp (Temp, pTimeToTx) == 0)
            {
                Len = strlen(JobQueue);

                TimeToTx[0] = JobQueue[Len - 12];
                TimeToTx[1] = JobQueue[Len - 11];
                TimeToTx[2] = '-';
                TimeToTx[3] = JobQueue[Len - 9];
                TimeToTx[4] = JobQueue[Len - 8];
                TimeToTx[5] = JobQueue[Len - 7];
                TimeToTx[6] = JobQueue[Len - 6];
                TimeToTx[7] = JobQueue[Len - 5];
                TimeToTx[8] = JobQueue[Len - 4];
                TimeToTx[9] = JobQueue[Len - 3];
                TimeToTx[10] = JobQueue[Len - 2];
                TimeToTx[11] = 0;

                break;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }

    memset(DelayTimeInJobQueueFile, 0x00, sizeof(DelayTimeInJobQueueFile));

    snprintf(DelayTimeInJobQueueFile, sizeof(DelayTimeInJobQueueFile), "%s", TimeToTx);

    return DelayTimeInJobQueueFile;
}


char *report_manager_request_get_time_to_tx_from_Job_Queue_File (UINT16 DocId)
{
    UINT8 Country = getCountryDateType();
    UINT8 Len = 0;
    UINT8 StringLength = 13;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char TimeToTx[MAX_LEN_DATE_TIME_YEAR + 1] = {0, }; // e.g.) 2015.05.20 14:25
    char *pTimeToTx = "TimeToTx     ";

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, StringLength);

            if (strcmp (Temp, pTimeToTx) == 0)
            {
                Len = strlen(JobQueue);

                switch (Country)
                {
                    case FAX_COUNTRY_DATE_TYPE_YMD:
                    {
                        TimeToTx[0] = JobQueue[Len - 17]; // 2
                        TimeToTx[1] = JobQueue[Len - 16]; // 0
                        TimeToTx[2] = JobQueue[Len - 15]; // 1
                        TimeToTx[3] = JobQueue[Len - 14]; // 5
                        TimeToTx[4] = '-';
                        TimeToTx[5] = JobQueue[Len - 12]; // 0
                        TimeToTx[6] = JobQueue[Len - 11]; // 5
                        TimeToTx[7] = '-';
                        TimeToTx[8] = JobQueue[Len - 9]; // 2
                        TimeToTx[9] = JobQueue[Len - 8]; // 0
                        TimeToTx[10] = JobQueue[Len - 7]; // white space
                        TimeToTx[11] = JobQueue[Len - 6]; // 1
                        TimeToTx[12] = JobQueue[Len - 5]; // 4
                        TimeToTx[13] = JobQueue[Len - 4]; // :
                        TimeToTx[14] = JobQueue[Len - 3]; // 2
                        TimeToTx[15] = JobQueue[Len - 2]; // 5
                        TimeToTx[16] = 0;

                        break;
                    }

                    case FAX_COUNTRY_DATE_TYPE_MDY:
                    {
                        TimeToTx[0] = JobQueue[Len - 12]; // 0
                        TimeToTx[1] = JobQueue[Len - 11]; // 5
                        TimeToTx[2] = '-';
                        TimeToTx[3] = JobQueue[Len - 9]; // 2
                        TimeToTx[4] = JobQueue[Len - 8]; // 0
                        TimeToTx[5] = '-';
                        TimeToTx[6] = JobQueue[Len - 17]; // 2
                        TimeToTx[7] = JobQueue[Len - 16]; // 0
                        TimeToTx[8] = JobQueue[Len - 15]; // 1
                        TimeToTx[9] = JobQueue[Len - 14]; // 5
                        TimeToTx[10] = JobQueue[Len - 7]; // white space
                        TimeToTx[11] = JobQueue[Len - 6]; // 1
                        TimeToTx[12] = JobQueue[Len - 5]; // 4
                        TimeToTx[13] = JobQueue[Len - 4]; // :
                        TimeToTx[14] = JobQueue[Len - 3]; // 2
                        TimeToTx[15] = JobQueue[Len - 2]; // 5
                        TimeToTx[16] = 0;

                        break;
                    }

                    case FAX_COUNTRY_DATE_TYPE_DMY:
                    {
                        TimeToTx[0] = JobQueue[Len - 9]; // 2
                        TimeToTx[1] = JobQueue[Len - 8]; // 0
                        TimeToTx[2] = '-';
                        TimeToTx[3] = JobQueue[Len - 12]; // 0
                        TimeToTx[4] = JobQueue[Len - 11]; // 5
                        TimeToTx[5] = '-';
                        TimeToTx[6] = JobQueue[Len - 17]; // 2
                        TimeToTx[7] = JobQueue[Len - 16]; // 0
                        TimeToTx[8] = JobQueue[Len - 15]; // 1
                        TimeToTx[9] = JobQueue[Len - 14]; // 5
                        TimeToTx[10] = JobQueue[Len - 7]; // white space
                        TimeToTx[11] = JobQueue[Len - 6]; // 1
                        TimeToTx[12] = JobQueue[Len - 5]; // 4
                        TimeToTx[13] = JobQueue[Len - 4]; // :
                        TimeToTx[14] = JobQueue[Len - 3]; // 2
                        TimeToTx[15] = JobQueue[Len - 2]; // 5
                        TimeToTx[16] = 0;

                        break;
                    }

                    default:
                        break;
                }

                break;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }

    memset(DelayTimeInJobQueueFileReportMgr, 0x00, sizeof(DelayTimeInJobQueueFileReportMgr));

    snprintf(DelayTimeInJobQueueFileReportMgr, sizeof(DelayTimeInJobQueueFileReportMgr), "%s", TimeToTx);

    return DelayTimeInJobQueueFileReportMgr;
}


char *report_manager_request_get_current_time_from_Job_Queue_File (UINT16 DocId)
{
    UINT8 StringLength = 13;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char Time[6] = {0, }; // e.g.) 14:25
    char *pCurrentTime = "CurrentTime  ";

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, StringLength);

            if (strcmp (Temp, pCurrentTime) == 0)
            {
                Time[0] = JobQueue[26]; // 1
                Time[1] = JobQueue[27]; // 4
                Time[2] = JobQueue[28]; // :
                Time[3] = JobQueue[29]; // 2
                Time[4] = JobQueue[30]; // 5
                Time[5] = 0;

                break;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }

    memset(CurrentTimeInJobQueueFileReportMgr, 0x00, sizeof(CurrentTimeInJobQueueFileReportMgr));

    snprintf(CurrentTimeInJobQueueFileReportMgr, sizeof(CurrentTimeInJobQueueFileReportMgr), "%s", Time);

    return CurrentTimeInJobQueueFileReportMgr;
}


#if __HANA_FINANCIAL_GROUP_REQ_014_5__
char *fsu_request_get_file_name_from_Job_Queue_File (UINT16 DocId)
{
    UINT8 Len = 0;
    UINT8 IndexToFileName = 0;
    UINT8 StringLength = 13;
    FILE *pFile = (FILE *)NULL;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char FileName[MAX_LEN_USER_ID + 1] = {0, };
    char *pFileName = "FileName     ";

    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    // Step 1. Open JobQueueXXXX.info..
    pFile = fopen(JobQueueFileName, "r");

    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return 0;
    }
    else
    {
        // Step 2. Get job queue info. from JobQueueXXXX.info..
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            // Step 2-1. Get the first 13 characters to distinguish each field...
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, StringLength);

            if (strcmp (Temp, pFileName) == 0)
            {
                Len = strlen(JobQueue);

                printf("\n(YHCHO) %s() - Len: %d\n", __func__, Len);

                for (IndexToFileName = 0; IndexToFileName < Len - 14; Len++)
                {
                    printf("\n(YHCHO) %s() - JobQueue[%d]: %c\n", __func__, IndexToFileName, JobQueue[IndexToFileName]);

                    FileName[IndexToFileName] = JobQueue[Len - 14 - IndexToFileName];
                }

                FileName[IndexToFileName] = 0;
            }
        }

        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }

    memset(FileNameInJobQueueFile, 0x00, sizeof(FileNameInJobQueueFile));

    snprintf(FileNameInJobQueueFile, sizeof(FileNameInJobQueueFile), "%s", FileName);

    return FileNameInJobQueueFile;
}


void get_file_name_from_Job_Queue_File(UINT16 jnum, char *JobQueue)
{
    int IndexToFileName = 0;
    int IndexToJobQueue = 0;
    int len = 0;

    len = strlen(JobQueue);

    for (IndexToFileName = 15; IndexToFileName < (len - 1); IndexToFileName++)
    {
        FSUJobQueueInfo.FileName[jnum][IndexToJobQueue] = JobQueue[IndexToFileName];

        IndexToJobQueue++;
    }

    FSUJobQueueInfo.FileName[jnum][IndexToJobQueue] = 0;
}


UINT8 get_permission_from_Job_Queue_File(char *JobQueue)
{
    int permission = 0;
    int len = 0;

    len = strlen(JobQueue);

    permission = JobQueue[len-2] - '0';

    return permission;
}

UINT16 get_unique_job_id_from_Job_Queue_File(char *JobQueue)
{
    UINT16 unique_job_id = 0;
    UINT8 First = 0;
    UINT8 Second = 0;
    UINT8 Third = 0;
    UINT8 Fourth = 0;
    UINT8 Fifth = 0;
    int len = 0;

    len = strlen(JobQueue);

    First = fsu_convert_char_to_int(JobQueue[len-6]);

    Second = fsu_convert_char_to_int(JobQueue[len-5]);

    Third = fsu_convert_char_to_int(JobQueue[len-4]);
    
    Fourth = fsu_convert_char_to_int(JobQueue[len-3]);
    
    Fifth = fsu_convert_char_to_int(JobQueue[len-2]);

    unique_job_id = First * 10000 + Second * 1000 + Third * 100 + Fourth * 10 + Fifth;

    return unique_job_id;
}


void get_scan_info_from_Job_Queue_File(stIC_JobDoneInfo *scanInfo, char *JobQueue)
{
    UINT16 temp = 0;
    UINT8 First = 0;
    UINT8 Second = 0;
    UINT8 Third = 0;
    int len = 0;

    len = strlen(JobQueue);

	//paperSize
    First = fsu_convert_char_to_int(JobQueue[len-5]);
    Second = fsu_convert_char_to_int(JobQueue[len-4]);
    Third = fsu_convert_char_to_int(JobQueue[len-3]);
    temp = First * 100 + Second * 10 + Third;
    scanInfo->paperSize = temp;

	//duplex
    First = fsu_convert_char_to_int(JobQueue[len-2]);
    temp = First;
    scanInfo->duplex = temp;
}
#endif


void Create_Job_Queue_File(UINT16 jnum, FAX_APP_JOB_TYPE_t FaxAppJobType, UINT16 DocId)
{
    FILE *pFile = (FILE *)NULL;
    int Resolution = 0;
    IMAGE_BACKUP_JOB_TYPE_t ModJobType = IMAGE_BACKUP_JOB_TYPE_TX_PENDING;
    UINT8 IndexToDialNo = 0;
    UINT8 NoOfDialNo = 0;
    UINT8 PhotoMode = 0;
    UINT8 Retransmit = 0;
    UINT16 ImageWidth = 0;
    UINT16 ImageHeight = 0;
    UINT16 OriginalDocId = 0;
    UINT16 CurrentPageNo = 0;
    UINT16 TotalPageNo= 0;
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowJobType[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowDialNo[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowTimeToTx[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowResolution[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowImageWidth[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowImageHeight[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowPhotoMode[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowOriginalDocId[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowRetransmit[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowPageInfo[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowCurrentTime[MAX_LEN_FILE_PATH_NAME] = {0, };
    char TmpCurrentTime[MAX_LEN_FILE_PATH_NAME] = {0, };
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
    char RowFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowPermission[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowUniqueJobId[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowScanInfo[MAX_LEN_FILE_PATH_NAME] = {0, };
#endif
    char RowPadding[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowPaddingDialNo[MAX_LEN_FILE_PATH_NAME] = {0, };
    struct timeval CurrentTime;
    struct tm *ptm;

    // Step 1. Create Job Queue File for the first time
    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, DocId, PATH_SEPARATOR, DocId);

    pFile = fopen(JobQueueFileName, "wt");     // This is the first time that the Job Queue File is being created... (SCAN TO MEM or RECEIVE TO MEM)
    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return ;
    }
    else
    {
        // Step 2. Get Job Queue Information from Job Queue, and so on.
        ModJobType = Convert_Faxapp_Job_Type_To_Image_Backup_Job_Type(FaxAppJobType);

        // Step 2-1. JobType
        snprintf(RowJobType, sizeof(RowJobType), "JobType      : %d\n", ModJobType); // Add 6 spaces after 'JobType'

        printf("(YHCHO) %s", RowJobType);

        fputs(RowJobType, pFile);

        // Step 2-2. DialNo
        if ( FaxAppJobType == FAX_APP_JOB_TYPE_POLL_TRANSMIT )
        {
            snprintf(RowDialNo, sizeof(RowDialNo), "DialNo       : %c", 0x20); // Add 7 spaces after 'DialNo'

            for (IndexToDialNo = 0; IndexToDialNo < (LEN_PHONE_NO - 1); IndexToDialNo++)
            {
                snprintf(RowPadding, sizeof(RowPadding), "%c", 0x20);

                strcat(RowDialNo, RowPadding);
            }

            snprintf(RowPaddingDialNo, sizeof(RowPaddingDialNo), "\n");

            strcat(RowDialNo, RowPaddingDialNo);
        }
        else
        {
            snprintf(RowDialNo, sizeof(RowDialNo), "DialNo       : %s", fsu_get_dial_no(jnum)); // Add 7 spaces after 'DialNo'

            NoOfDialNo = strlen((char *)fsu_get_dial_no(jnum));

            //printf("\n(YHCHO) %s() - NoOfDialNo: %d\n", __FUNCTION__, NoOfDialNo);

            for (IndexToDialNo = 0; IndexToDialNo < (LEN_PHONE_NO - NoOfDialNo); IndexToDialNo++)
            {
                snprintf(RowPadding, sizeof(RowPadding), "%c", 0x20);

                strcat(RowDialNo, RowPadding);
            }

            snprintf(RowPaddingDialNo, sizeof(RowPaddingDialNo), "\n");

            strcat(RowDialNo, RowPaddingDialNo);
        }

        printf("(YHCHO) %s", RowDialNo);

        fputs(RowDialNo, pFile);

        // Step 2-3. TimeToTx
        if ( FaxAppJobType == FAX_APP_JOB_TYPE_DELAY )
        {
            snprintf(RowTimeToTx, sizeof(RowTimeToTx), "TimeToTx     : %s\n", JobQueueDelayTime);
        }
        else
        {
            snprintf(RowTimeToTx, sizeof(RowTimeToTx), "TimeToTx     : %c\n", 0x20);
        }

        printf("(YHCHO) %s", RowTimeToTx);

        fputs(RowTimeToTx, pFile);

        // Step 2-4. Resolution
        if (FaxAppJobType == FAX_APP_JOB_TYPE_POLL_RECEIVE)
        {
            Resolution = 9; // meaningless value only for poll receive
        }
        else
        {
            Resolution = fsu_get_resolution(jnum);
        }

        snprintf(RowResolution, sizeof(RowResolution), "Resolution   : %d\n", Resolution); // Add 3 spaces after 'Resolution'

        printf("(YHCHO) %s", RowResolution);

        fputs(RowResolution, pFile);

        // Step 2-5. Image Width
        ImageWidth = fsu_get_image_width_pixel(jnum);

        snprintf(RowImageWidth, sizeof(RowImageWidth), "ImageWidth   : %04d\n", ImageWidth); // Add 3 spaces after 'ImageWidth'

        printf("(YHCHO) %s", RowImageWidth);

        fputs(RowImageWidth, pFile);

        // Step 2-6. Image Height
        ImageHeight = fsu_get_image_height_pixel(jnum);

        snprintf(RowImageHeight, sizeof(RowImageHeight), "ImageHeight  : %04d\n", ImageHeight); // Add 2 spaces after 'ImageHeight'

        printf("(YHCHO) %s", RowImageHeight);

        fputs(RowImageHeight, pFile);

        // Step 2-7. Image Type
        PhotoMode = fsu_get_photo_mode(jnum);

        snprintf(RowPhotoMode, sizeof(RowPhotoMode), "PhotoMode    : %d\n", PhotoMode); // Add 4 spaces after 'PhotoMode'

        printf("(YHCHO) %s", RowPhotoMode);

        fputs(RowPhotoMode, pFile);

        // Step 2-8. Original Doc Id
        OriginalDocId = fsu_get_original_doc_id(jnum);

        snprintf(RowOriginalDocId, sizeof(RowOriginalDocId), "OriginalDocId: %04X\n", OriginalDocId); // Add 0 spaces after 'OriginalDocId'

        printf("(YHCHO) %s", RowOriginalDocId);

        fputs(RowOriginalDocId, pFile);

        // Step 2-9. Retransmit
        if ( ((FaxAppJobType == FAX_APP_JOB_TYPE_BROADCAST) && (DocId == OriginalDocId)) || (FaxAppJobType == FAX_APP_JOB_TYPE_POLL_RECEIVE) || (FaxAppJobType == FAX_APP_JOB_TYPE_POLL_TRANSMIT) )
        {
            Retransmit = FSU_RETRANSMIT_NEED_RETRANSMIT;
        }
        else
        {
            Retransmit = FSU_RETRANSMIT_NO_NEED_RETRANSMIT;
        }

        snprintf(RowRetransmit, sizeof(RowRetransmit), "Retransmit   : %d\n", Retransmit); // Add 3 spaces after 'Retransmit'

        printf("(YHCHO) %s", RowRetransmit);

        fputs(RowRetransmit, pFile);

        // Step 2-10. Page Info.
        if ( (FaxAppJobType == FAX_APP_JOB_TYPE_BROADCAST) && (DocId == OriginalDocId) )
        {
            CurrentPageNo = 0;

            TotalPageNo = fsu_get_total_pages(jnum);
        }
        else
        {
            TotalPageNo = CurrentPageNo = MAX_NO_IMAGE_FILES;
        }

        snprintf(RowPageInfo, sizeof(RowPageInfo), "PageInfo.    : %03d/%03d\n", CurrentPageNo, TotalPageNo); // Add 4 spaces after 'PageInfo.'

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        printf("(YHCHO) %s", RowPageInfo);
#else
        printf("(YHCHO) %s\n", RowPageInfo);
#endif

        fputs(RowPageInfo, pFile);

        // Step 2-11. CurrentTime
        gettimeofday(&CurrentTime, NULL);

        ptm = localtime(&CurrentTime.tv_sec);

        if(ptm == NULL)
        {
            snprintf(TmpCurrentTime, sizeof(TmpCurrentTime), "%04d.%02d.%02d %02d:%02d", 1900, 1, 1, 0, 0);
        }
        else
        {
            snprintf(TmpCurrentTime, sizeof(TmpCurrentTime), "%04d.%02d.%02d %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
        }

        snprintf(RowCurrentTime, sizeof(RowCurrentTime), "CurrentTime  : %s\n", TmpCurrentTime); // Add 2 spaces after 'CurrentTime'

        printf("(YHCHO) %s\n", RowCurrentTime);

        fputs(RowCurrentTime, pFile);

#if __HANA_FINANCIAL_GROUP_REQ_014_5__
        // Step 2-11. File Name
        snprintf(RowFileName, sizeof(RowFileName), "FileName     : %s\n", fsu_get_file_name_tx(jnum)); // Add 5 spaces after 'FileName'

        printf("(YHCHO) %s\n", RowFileName);
        
        fputs(RowFileName, pFile);

        // Step 2-12. Send Permission
        snprintf(RowPermission, sizeof(RowPermission), "Permission   : %d\n", fsu_get_permission(jnum)); // Add 3 spaces after 'Permission'

        printf("(YHCHO) %s", RowPermission);
        
        fputs(RowPermission, pFile);

        // Step 2-13. Unique Job Id
        snprintf(RowUniqueJobId, sizeof(RowUniqueJobId), "UinqueJobId  : %05ld\n", fsu_get_unique_job_id(jnum)); // Add 2 spaces after 'UniqueJobId'

        printf("(YHCHO) %s", RowUniqueJobId);

        fputs(RowUniqueJobId, pFile);

        // Step 2-13. Unique Job Id
        snprintf(RowScanInfo, sizeof(RowScanInfo), "ScanInfo     : %03d%01d\n",
				fsu_get_ScanInfo_paperSize(jnum),
				fsu_get_ScanInfo_duplex(jnum)); // Add 2 spaces after 'ScanInfo'

        printf("(YHCHO) %s", RowScanInfo);

        fputs(RowScanInfo, pFile);
#endif

        // Step 3. flush
        if (fflush(pFile) != 0)
        {
            XASSERT(0,0);
        }

        // Step 4. fsync
        if (fsync(pFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        // Step 5. close
        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }
}


void Update_Job_Queue_File(IMAGE_BACKUP_REASON_t Reason, UINT16 jnum, UINT16 Param)
{
    switch (Reason)
    {
        case IMAGE_BACKUP_REASON_UPDATE_JOB_TYPE:
            Update_Job_Type(jnum, Param);
            break;

        case IMAGE_BACKUP_REASON_UPDATE_DIAL_NO:
            Update_Dial_No(jnum);
            break;

        case IMAGE_BACKUP_REASON_UPDATE_RETRANSMIT:
            Update_Retransmit(jnum, Param);
            break;

        case IMAGE_BACKUP_REASON_UPDATE_PAGE_INFO:
            Update_Page_Info(jnum, Param);
            break;

        case IMAGE_BACKUP_REASON_UPDATE_RESOLUTION:
            Update_Resolution(jnum, Param);
            break;
            
        case IMAGE_BACKUP_REASON_UPDATE_PERMISSION:
            Update_Permission(jnum, Param);
            break;

        default:
            break;
    }
}


void Update_Job_Type(UINT16 jnum, UINT8 JobType)
{
    FILE *pFile = (FILE *)NULL;
    int FilePosition = 0;
    UINT8 IndexToString = 0;
    UINT8 Length = 0;
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowJobType[MAX_LEN_FILE_PATH_NAME] = {0, };
    char *pJobType = "JobType";

    // Step 1. Open Job Queue File
    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, fsu_get_doc_id(jnum));
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    pFile = fopen(JobQueueFileName, "r+b");
    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return ;
    }
    else
    {
        // Step 2. Look for the Job Type (JobType) Field
        Length = strlen(pJobType);

        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            for (IndexToString = 0; IndexToString < (Length + 1); IndexToString++)
            {
                if ( IndexToString == Length )
                {
                    Temp[IndexToString] = '\0';
                    break;
                }
                else
                {
                    Temp[IndexToString] = JobQueue[IndexToString];
                }
            }

            if (strcmp (Temp, pJobType) == 0)
            {
                FilePosition = ftell(pFile);

                if (fseek (pFile, -17, SEEK_CUR) != 0)
                {
                    XASSERT(0,0);
                }

                snprintf(RowJobType, sizeof(RowJobType), "JobType      : %d\n", JobType); // Add 6 spaces after 'JobType'

                fputs(RowJobType, pFile);

                break;
            }
        }

        // Step 3. flush
        if (fflush(pFile) != 0)
        {
            XASSERT(0,0);
        }

        // Step 4. fsync
        if (fsync(pFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        // Step 5. close
        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }
}


void Update_Dial_No(UINT16 jnum)
{
    FILE *pFile = (FILE *)NULL;
    int FilePosition = 0;
    UINT8 IndexToString = 0;
    UINT8 Length = 0;
    UINT8 NoOfDialNo = 0;
    UINT8 IndexToDialNo = 0;
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowDialNo[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowPadding[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowPaddingDialNo[MAX_LEN_FILE_PATH_NAME] = {0, };
    char *pDialNo = "DialNo";

    // Step 1. Open Job Queue File
    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, fsu_get_doc_id(jnum));
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    pFile = fopen(JobQueueFileName, "r+b");
    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return ;
    }
    else
    {
        // Step 2. Look for the Dial No. (DialNo) Field
        Length = strlen(pDialNo);

        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            for (IndexToString = 0; IndexToString < (Length + 1); IndexToString++)
            {
                if ( IndexToString == Length )
                {
                    Temp[IndexToString] = '\0';
                    break;
                }
                else
                {
                    Temp[IndexToString] = JobQueue[IndexToString];
                }
            }

            if (strcmp (Temp, pDialNo) == 0)
            {
                // Step 3. Write the Dial No. to Job Queue File

                FilePosition = ftell(pFile);

                if (fseek (pFile, -(17 + LEN_PHONE_NO - 1), SEEK_CUR) != 0)
                {
                    XASSERT(0,0);
                }

                // Get the forward no. from txq.job[jnum].fax_num using fsu_get_dial_no..
                snprintf(RowDialNo, sizeof(RowDialNo), "DialNo       : %s", fsu_get_dial_no(jnum)); // Add 7 spaces after 'DialNo'

                // Add spaces..
                NoOfDialNo = strlen((char *)fsu_get_dial_no(jnum));

                for (IndexToDialNo = 0; IndexToDialNo < (LEN_PHONE_NO - NoOfDialNo); IndexToDialNo++)
                {
                    snprintf(RowPadding, sizeof(RowPadding), "%c", 0x20);

                    strcat(RowDialNo, RowPadding);
                }

                snprintf(RowPaddingDialNo, sizeof(RowPaddingDialNo), "\n");

                strcat(RowDialNo, RowPaddingDialNo);

                fputs(RowDialNo, pFile);

                break;
            }
        }

        // Step 4. flush
        if (fflush(pFile) != 0)
        {
            XASSERT(0,0);
        }

        // Step 5. fsync
        if (fsync(pFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        // Step 6. close
        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG 
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }
}


void Update_Retransmit(UINT16 jnum, UINT8 Retransmit)
{
    FILE *pFile = (FILE *)NULL;
    int len = 13;
    int FilePosition = 0;
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowRetransmit[MAX_LEN_FILE_PATH_NAME] = {0, };
    char *pRetransmit = "Retransmit   ";

    // Step 1. Open Job Queue File
    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, fsu_get_doc_id(jnum));
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    pFile = fopen(JobQueueFileName, "r+b");
    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return ;
    }
    else
    {
        // Step 2. Look for the Retransmit (Retransmit) Field
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, len);

            if (strcmp (Temp, pRetransmit) == 0)
            {
                FilePosition = ftell(pFile);

                if (fseek (pFile, -17, SEEK_CUR) != 0)
                {
                    XASSERT(0,0);
                }

                snprintf(RowRetransmit, sizeof(RowRetransmit), "Retransmit   : %d\n", Retransmit); // Add 3 spaces after 'Retransmit'

                fputs(RowRetransmit, pFile);

                break;
            }
        }

        // Step 3. flush
        if (fflush(pFile) != 0)
        {
            XASSERT(0,0);
        }

        // Step 4. fsync
        if (fsync(pFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        // Step 5. close
        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
        if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }
}


void Update_Page_Info(UINT16 jnum, UINT16 CurrentPageNo)
{
    FILE *pFile = (FILE *)NULL;
    int len = 13;
    int FilePosition = 0;
    int TotalPageNo = 0;
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowPageInfo[MAX_LEN_FILE_PATH_NAME] = {0, };
    char *pPageInfo = "PageInfo.    ";

    // Step 1. Open Job Queue File
    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, fsu_get_doc_id(jnum));
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    pFile = fopen(JobQueueFileName, "r+b");
    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return ;
    }
    else
    {
        // Step 2. Look for the Page Info. (PageInfo.) Field
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, len);

            if (strcmp (Temp, pPageInfo) == 0)
            {
                FilePosition = ftell(pFile);

                if (fseek (pFile, -23, SEEK_CUR) != 0)
                {
                    XASSERT(0,0);
                }

                if (fsu_get_photo_mode(jnum) == 1)
                {
                    TotalPageNo = fsu_get_total_page_no_in_mem (fsu_get_doc_id(jnum), FAX_FILE_TYPE_PHOTO);
                }
                else
                {
                    TotalPageNo = fsu_get_total_page_no_in_mem (fsu_get_doc_id(jnum), FAX_FILE_TYPE_JBG);
                }

                snprintf(RowPageInfo, sizeof(RowPageInfo), "PageInfo.    : %03d/%03d\n", CurrentPageNo, (TotalPageNo - CurrentPageNo)); // Add 3 spaces after 'PageInfo.'

                fputs(RowPageInfo, pFile);

                break;
            }
        }

        // Step 3. flush
        if (fflush(pFile) != 0)
        {
            XASSERT(0,0);
        }

        // Step 4. fsync
        if (fsync(pFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        // Step 5. close
        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }
}


void Update_Resolution(UINT16 jnum, UINT8 Resolution)
{
    FILE *pFile = (FILE *)NULL;
    int len = 13;
    int FilePosition = 0;
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowResolution[MAX_LEN_FILE_PATH_NAME] = {0, };
    char *pResolution = "Resolution   ";

    // Step 1. Open Job Queue File
    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, fsu_get_doc_id(jnum));
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    pFile = fopen(JobQueueFileName, "r+b");
    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return ;
    }
    else
    {
        // Step 2. Look for the Resolution (Resolution) Field
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, len);

            if (strcmp (Temp, pResolution) == 0)
            {
                FilePosition = ftell(pFile);

                if (fseek (pFile, -17, SEEK_CUR) != 0)
                {
                    XASSERT(0,0);
                }

                snprintf(RowResolution, sizeof(RowResolution), "Resolution   : %d\n", Resolution); // Add 3 spaces after 'Resolution'

                fputs(RowResolution, pFile);

                break;
            }
        }

        // Step 3. flush
        if (fflush(pFile) != 0)
        {
            XASSERT(0,0);
        }

        // Step 4. fsync
        if (fsync(pFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        // Step 5. close
        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }
}


void Update_Permission(UINT16 jnum, UINT8 Permission)
{
    FILE *pFile = (FILE *)NULL;
    int len = 13;
    int FilePosition = 0;
    char JobQueue[1024] = {0, };
    char Temp[14] = {0, };
    char JobQueueFileName[MAX_LEN_FILE_PATH_NAME] = {0, };
    char RowPermission[MAX_LEN_FILE_PATH_NAME] = {0, };
    char *pPermission = "Permission   ";

    // Step 1. Open Job Queue File
    snprintf(JobQueueFileName, sizeof(JobQueueFileName), "%s%cDoc%04X%cJobQueue%04X.info", pRootFolderName, PATH_SEPARATOR, fsu_get_doc_id(jnum), PATH_SEPARATOR, fsu_get_doc_id(jnum));
#ifdef HAVE_IMAGELOG
	if(isSDCardEnable)
		ARIA_encode(JobQueueFileName, 1);
#endif
    pFile = fopen(JobQueueFileName, "r+b");
    if (pFile == (FILE *)NULL)
    {
        XASSERT(0,0);

        return ;
    }
    else
    {
        // Step 2. Look for the Resolution (Resolution) Field
        while (fgets(JobQueue, 1024, pFile) != NULL)
        {
            memset(Temp, 0x00, sizeof(Temp));

            memcpy(Temp, JobQueue, len);

            if (strcmp (Temp, pPermission) == 0)
            {
                FilePosition = ftell(pFile);

                if (fseek (pFile, -17, SEEK_CUR) != 0)
                {
                    XASSERT(0,0);
                }

                snprintf(RowPermission, sizeof(RowPermission), "Permission   : %d\n", Permission); // Add 3 spaces after 'Permission'

                fputs(RowPermission, pFile);

                break;
            }
        }

        // Step 3. flush
        if (fflush(pFile) != 0)
        {
            XASSERT(0,0);
        }

        // Step 4. fsync
        if (fsync(pFile->_fileno) == -1)
        {
            XASSERT(0,0);
        }

        // Step 5. close
        if (fclose(pFile) != 0)
        {
            XASSERT(0,0);
        }
#ifdef HAVE_IMAGELOG
		if(isSDCardEnable)
			ARIA_encode(JobQueueFileName, 0);
#endif
    }
}


IMAGE_BACKUP_JOB_TYPE_t Convert_Faxapp_Job_Type_To_Image_Backup_Job_Type(FAX_APP_JOB_TYPE_t JobType)
{
    UINT8 ImageBackupJobType = IMAGE_BACKUP_JOB_TYPE_TX_PENDING;

    switch (JobType)
    {
        case FAX_APP_JOB_TYPE_TX_IMMEDIATE:
        case FAX_APP_JOB_TYPE_BROADCAST:
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
		case FAX_APP_JOB_TYPE_FAX_SOLUTION:
#endif
            ImageBackupJobType = IMAGE_BACKUP_JOB_TYPE_TX_PENDING;
            break;

        case FAX_APP_JOB_TYPE_DELAY:
            ImageBackupJobType = IMAGE_BACKUP_JOB_TYPE_TX_PENDING_DELAY;
            break;

        case FAX_APP_JOB_TYPE_POLL_TRANSMIT:
            ImageBackupJobType = IMAGE_BACKUP_JOB_TYPE_TX_PENDING_POLL;
            break;

        case FAX_APP_JOB_TYPE_RECEIVE:
            ImageBackupJobType = IMAGE_BACKUP_JOB_TYPE_RCV_DONE;
            break;

        case FAX_APP_JOB_TYPE_POLL_RECEIVE:
            ImageBackupJobType = IMAGE_BACKUP_JOB_TYPE_POLL_RECEIVE;
            break;

        default:
            break;
    }

    return ImageBackupJobType;

}

