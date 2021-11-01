#include "SDHFaxCommonHeader.h"
#include <stdio.h>
#include "db_api.h"
#include "../../faxapp/include/faxapp_notify.h"
#include "../../faxapp/include/faxapp_sc_code.h"


/***********************************************************************
 *  Function: faxapp_get_select_country_from_db()
 *
 *  Description:
 *    get the country code saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    country code
 *
 **********************************************************************/
int faxapp_get_select_country_from_db(void)
{
    int SelectCountry = 0;
    error_type_t db_retval = FAIL;

    SelectCountry = db_get_var_int("CONF_FAXSP", "SelectCountry", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> SelectCountry\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return SelectCountry;
}


/***********************************************************************
 *  Function: faxapp_get_rep_tx_rx_set()
 *
 *  Description:
 *    get the print option for Tx / Rx Result Report saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    RepTxRxSet 0: Always On
 *               1: On Error
 *               2: Tx Only
 *               3: Rx Only
 *               4: OFF
 *
 **********************************************************************/
int faxapp_get_rep_tx_rx_set(void)
{
    int RepTxRxSet = 0;
    error_type_t db_retval = FAIL;

    RepTxRxSet = db_get_var_int("CONF_FAXUP", "RepTxRxSet", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RepTxRxSet\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return RepTxRxSet;
}


/***********************************************************************
 *  Function: faxapp_get_rep_tcr_set()
 *
 *  Description:
 *    get the country code saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    RepTCRSet
 *
 **********************************************************************/
int faxapp_get_rep_tcr_set(void)
{
    int RepTCRSet = 0;
    error_type_t db_retval = FAIL;

    RepTCRSet = db_get_var_int("CONF_FAXUP", "RepTCRSet", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RepTCRSet\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return RepTCRSet;
}


/***********************************************************************
 *  Function: faxapp_get_print_media_size_tray_1()
 *
 *  Description:
 *    get the media size for tray 1 saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    Media Size
 *
 **********************************************************************/
int faxapp_get_print_media_size_tray_1 (void)
{
    int MediaSize = 0;
    error_type_t db_retval = FAIL;

    MediaSize = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_1", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_PRNT -> TrayConfig_mediaSize_1\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return MediaSize;
}



/***********************************************************************
 *  Function: faxapp_get_print_media_size_tray_2()
 *
 *  Description:
 *    get the media size for tray 2 saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    Media Size
 *
 **********************************************************************/
int faxapp_get_print_media_size_tray_2 (void)
{
    int MediaSize = 0;
    error_type_t db_retval = FAIL;

    MediaSize = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_2", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_PRNT -> TrayConfig_mediaSize_2\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return MediaSize;
}


/***********************************************************************
 *  Function: faxapp_get_print_media_size_tray_3()
 *
 *  Description:
 *    get the media size for tray 3 saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    Media Size
 *
 **********************************************************************/
int faxapp_get_print_media_size_tray_3 (void)
{
    int MediaSize = 0;
    error_type_t db_retval = FAIL;

    MediaSize = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_3", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_PRNT -> TrayConfig_mediaSize_3\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return MediaSize;
}


/***********************************************************************
 *  Function: faxapp_get_print_media_size_tray_0()
 *
 *  Description:
 *    get the media size for tray 0(MPT) saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    Media Size
 *
 **********************************************************************/
int faxapp_get_print_media_size_tray_0 (void)
{
    int MediaSize = 0;
    error_type_t db_retval = FAIL;

    MediaSize = db_get_var_int("CONF_PRNT", "TrayConfig_mediaSize_0", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_PRNT -> TrayConfig_mediaSize_0\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return MediaSize;
}


/***********************************************************************
 *  Function: faxapp_get_print_default_src_tray()
 *
 *  Description:
 *    get the default media size saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    Media Size
 *
 **********************************************************************/
int faxapp_get_print_default_src_tray (void)
{
    int defalut_tray = 1;
    error_type_t db_retval = FAIL;

    defalut_tray = db_get_var_int("CONF_PRNT", "Default_Src_Tray", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_PRNT -> Default_Src_Tray\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return defalut_tray;
}


/***********************************************************************
 *  Function: faxapp_get_data_count()
 *
 *  Description:
 *    get the last no of RECD_FAXLOG / RECD_FAXSPLOG in DB
 *
 *  Inputs:
 *
 *  Output:
 *    DataCount
 *
 **********************************************************************/
int faxapp_get_data_count(UINT8 Reason)
{
    int DataCount = 0;
    error_type_t db_retval = FAIL;

    if (Reason == FAX_REPORT_TCR)
    {
        DataCount = db_get_data_count("RECD_FAXLOG", NULL, &db_retval);

        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_get_data_count() Error... RECD_FAXLOG\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
    }
    else
    {
        DataCount = db_get_data_count("RECD_FAXSPLOG", NULL, &db_retval);

        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_get_data_count() Error... RECD_FAXSPLOG\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
    }

    return DataCount;
}


/***********************************************************************
 *  Function: faxapp_get_rep_broadcast_result_report()
 *
 *  Description:
 *    get the broadcast result report print option saved in DB
 *
 *  Inputs:
 *
 *  Output:
 *    RepBCResultReport
 *
 **********************************************************************/
int faxapp_get_rep_broadcast_result_report(void)
{
    int RepBCResultReport = 0;
    error_type_t db_retval = FAIL;

    RepBCResultReport = db_get_var_int("CONF_FAXUP", "RepBCResultReport", &db_retval);
    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_get_var_int() Error... CONF_FAXUP -> RepBCResultReport\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_READ_DB);
    }

    return RepBCResultReport;
}


/********************************************************************************
 *  Function: faxapp_delete_record_var()
 *
 *  Description:
 *    delete the oldest record of RECD_FAXLOG / RECD_FAXSPLOG if it has more than
 *    35 or 700 records, respectively
 *
 *  Inputs:
 *
 *  Output:
 *    DataCount
 *
 ********************************************************************************/
void faxapp_delete_record_var(UINT8 Reason)
{
    error_type_t db_retval = FAIL;

    if (Reason == FAX_REPORT_TCR)
    {
        db_retval = db_del_recd_var("RECD_FAXLOG");

        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_del_recd_var() Error... RECD_FAXLOG\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
    }
    else
    {
        db_retval = db_del_recd_var("RECD_FAXSPLOG");

        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_del_recd_var() Error... RECD_FAXSPLOG\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
    }
}


/***************************************************************************************
 *  Function: faxapp_get_last_record_no()
 *
 *  Description:
 *    get the last NO of RECD_FAXLO or RECD_FAXSPLOG depending on the Reason
 *
 *  Inputs:
 *    UINT8 Reason: determine which db table to get
 *
 *  Output:
 *    int RecordNo: the last record no
 *
 **************************************************************************************/
int faxapp_get_last_record_no (UINT8 Reason)
{
    error_type_t db_retval = FAIL;
    int LastRecordNo = 0;

    if (Reason == FAX_REPORT_TCR)
    {
        LastRecordNo = db_get_last_recd_no("RECD_FAXLOG", &db_retval);

        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_get_last_recd_no() Error... RECD_FAXLOG\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
    }
    else
    {
        LastRecordNo = db_get_last_recd_no("RECD_FAXSPLOG", &db_retval);

        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_get_last_recd_no() Error... RECD_FAXSPLOG\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
    }

    return LastRecordNo;
}


/*******************************************************************************************
 *  Function: faxapp_delete_all_records()
 *
 *  Description:
 *    delete all records of RECD_FAXLOG or RECD_FAXSPLOG depending on the Reason
 *
 *  Inputs:
 *    UINT8 Reason: determine which db table to purge
 *
 *  Output:
 *
 *******************************************************************************************/
void faxapp_delete_all_records (UINT8 Reason)
{
    error_type_t db_retval = FAIL;

    if (Reason == FAX_REPORT_TCR)
    {
        db_retval = db_del_recd_all("RECD_FAXLOG");

        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_del_recd_all() Error... RECD_FAXLOG\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
    }
    else
    {
        db_retval = db_del_recd_all("RECD_FAXSPLOG");

        if (db_retval == FAIL)
        {
            printf("\n(YHCHO) %s() - db_del_recd_all() Error... RECD_FAXSPLOG\n", __FUNCTION__);

            faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
        }
    }
}


/*******************************************************************************************
 *  Function: faxapp_update_recd_fax_image_backup()
 *
 *  Description:
 *    update RECD_FAXIMAGEBACKUP
 *
 *  Inputs:
 *    pointer to JOB_QUEUE_INFO_UI_t struct
 *
 *  Output:
 *
 *******************************************************************************************/
void faxapp_update_recd_fax_image_backup (JOB_QUEUE_INFO_UI_t *Info)
{
    error_type_t db_retval = FAIL;

    db_retval = db_set_recd_fax_image_backup(Info->ID, Info->JobType, Info->DialNo, Info->TimeToTx);

    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_set_recd_fax_image_backup() Error...\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
    }
}


/*******************************************************************************************
 *  Function: faxapp_update_recd_fax_job_log()
 *
 *  Description:
 *    update RECD_FAXJOBLOG
 *
 *  Inputs:
 *    pointer to INFO_FOR_FAX_JOB_LOG_t struct
 *
 *  Output:
 *
 *******************************************************************************************/
void faxapp_update_recd_fax_job_log (INFO_FOR_FAX_JOB_LOG_t *Info)
{
    error_type_t db_retval = FAIL;

    db_retval = db_set_recd_fax_job_log(Info->JobType, Info->UserId, Info->DateTime, Info->RemoteNo, Info->Result, Info->TotalPageNo, Info->DelayTime);

    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_set_recd_fax_job_log() Error...\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
    }
}


/*******************************************************************************************
 *  Function: faxapp_update_broadcast_result_report_info_to_db()
 *
 *  Description:
 *    update RECD_FAXBROADCASTRESULTREPORT
 *
 *  Inputs:
 *    pointer to FSU_INFO_BROADCAST_RESULT_REPORT_t struct
 *
 *  Output:
 *
 *******************************************************************************************/
void faxapp_update_broadcast_result_report_info_to_db (FSU_INFO_BROADCAST_RESULT_REPORT_t *Info)
{
    error_type_t db_retval = FAIL;

    db_retval = db_set_recd_fax_broadcast_log(Info->JobId, Info->RemoteNo, Info->StartTime, Info->Duration, Info->TotalPageNo, Info->SentPageNo, Info->Result);

    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_set_recd_fax_broadcast_log() Error...\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
    }
}


/*******************************************************************************************
 *  Function: faxapp_delete_all_records_recd_faximagebackup()
 *
 *  Description:
 *    To purge RECD_FAXIMAGEBACKUP
 *
 *  Inputs:
 *
 *  Output:
 *
 *******************************************************************************************/
void faxapp_delete_all_records_recd_faximagebackup (void)
{
    error_type_t db_retval = FAIL;

    db_retval = db_del_recd_all("RECD_FAXIMAGEBACKUP");

    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_del_recd_all() Error... RECD_FAXIMAGEBACKUP\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
    }
}


/*******************************************************************************************
 *  Function: faxapp_delete_all_records_recd_faxbroadcastresultreport()
 *
 *  Description:
 *    To purge RECD_FAXBROADCASTRESULTREPORT
 *
 *  Inputs:
 *
 *  Output:
 *
 *******************************************************************************************/
void faxapp_delete_all_records_recd_faxbroadcastresultreport (void)
{
    error_type_t db_retval = FAIL;

    db_retval = db_del_recd_all("RECD_FAXBROADCASTRESULTREPORT");

    if (db_retval == FAIL)
    {
        printf("\n(YHCHO) %s() - db_del_recd_all() Error... RECD_FAXBROADCASTRESULTREPORT\n", __FUNCTION__);

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);
    }
}

