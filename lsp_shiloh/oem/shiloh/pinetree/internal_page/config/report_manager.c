#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ctemplate.h"
#include "deviceid.h"
#include "string_mgr_api.h"
#include "printvars_api.h"

#include "oid_api.h"
#ifdef HAVE_NETWORK
//#include "net_iface_vars.h"
//#include "net_eth_vars.h"
#endif

#include "file_to_pipe_api.h"
#include "internal_page_api.h"

#include "report_manager.h"
#include "report_page.h"
#include "json_to_pcl.h"
#include "json_common.h"
#include "db_api.h"
#include "memAPI.h"
#include "logger.h"
#include "spec.h"
#include "fwVersion.h"
#include "net_api.h"

#include "report_templates.h"

#include <sys/time.h>
#include "../../sdh_fax/faxapp/include/faxapp_sc_code.h"
#include "../../sdh_fax/faxapp/include/faxapp_notify.h"
#include "../../sdh_fax/CommonHeader/SDHFaxCommonHeader.h"

#define DBG_PRFX "REPORT: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_INTERNAL | LOGGER_SUBMODULE_BIT(1)

#define SDH_ERR DBG_PRINTF_SDH_ERR
#define SDH_DEBUG DBG_PRINTF_SDH_DEBUG

/*
 *******************************************************************
/// Changed History
 *******************************************************************
 * YOU MUST KEEP BELOW FORMAT OF SENTENCE !!! 
 * [yyyy.mm.dd]: [version]: [author]: [description]
 * 
 * 2013.11.29: 0.1.0: jcshin: add version information.
 * 2013.12.02: 0.1.1: jcshin: add add_tli_header() function.
 * 2013.12.02: 0.1.2: jcshin: move get_version_FaxApp() function to fwVersion.h
 * 2013.12.06: 0.1.3: jcshin: add item about toner/drum, CPU clock. move the context position.
 * 2013.12.06: 0.1.4: jcshin: add UIManager version information.
 * 2013.12.07: 0.1.5: jcshin: apply toner infos.
 * 2013.12.07: 0.1.6: jcshin: adjust main motor value.
 * 2013.12.13: 0.1.7: jcshin: change CPU Clock speed(Dual -> Quad).
 * 2013.12.16: 0.1.8: jcshin: change CPU Clock speed(Quad -> Quad-core).
 * 2013.12.16: 0.1.9: jcshin: change pcl tray value.
 * 2013.12.16: 0.1.10: jcshin: get toner count/reamin/tli/serial, tray version info. from DB.
 * 2013.12.16: 0.1.11: jcshin: decode tray version.
 * 2013.12.17: 0.1.12: jcshin: add drum count in sp report.
 * 2013.12.18: 0.1.13: jcshin: change the motor value in sp report.
 * 2013.12.18: 0.1.14: jcshin: add toner count in sp report.
 * 2013.12.19: 0.1.15: jcshin: change the format of system report.
 * 2013.12.19: 0.1.16: jcshin: -1 minutes -> intelligent minutes.
 * 2014.01.10: 0.1.17: jcshin: add the toner history info.
 * 2014.01.10: 1.0.0: jcshin: change the 102, 103 jam message.
 * 2014.02.24: 1.0.1: jcshin: remove print timeout item.
 * 2014.02.25: 1.1.0: jcshin: add fax version information in the system report.
 * 2014.03.03: 1.2.0: jcshin: apply new CI.
 * 2014.03.03: 1.2.1: jcshin: change the name of fax client.
 * 2014.03.12: 1.2.2: jcshin: change the report source tray.
 * 2014.03.14: 1.2.3: jcshin: add error code information.
 * 2014.03.14: 1.2.4: jcshin: add copy/scan/fax page count.
 * 2014.04.07: 1.2.5: jcshin: add letter size default case.(for NA)
 * 2014.04.08: 1.2.6: jcshin: hide copy/scan/fax count on KARA-S.
 * 2014.04.10: 1.2.7: jcshin: add scan page count.
 * 2014.04.02: 1.3.0: jcshin: add copy/scan/fax setting information.
 * 2014.04.14: 1.3.1: jcshin: add darkness information and modify someting.
 * 2014.04.14: 1.3.2: jcshin: add GUI and Browser version(only P-S).
 * 2014.04.14: 1.3.3: jcshin: fix some bugs.
 * 2014.04.14: 1.3.4: jcshin: add scan page count information in system report.
 * 2014.04.17: 1.3.5: jcshin: add address book page.
 * 2014.04.21: 1.3.6: jcshin: change print darkeness value.
 * 2014.04.21: 1.3.7: jcshin: remove ftp/smb/webdav information.
 * 2014.04.21: 1.3.8: jcshin: address book page link up to UIManager.
 * 2014.04.22: 1.3.9: jcshin: modify address book page.
 * 2014.04.23: 1.3.10: jcshin: add fax prefix number information.
 * 2014.04.24: 1.3.11: jcshin: add kernel version information.
 * 2014.04.29: 1.3.12: jcshin: change the range of print darkness in pinetree.
 * 2014.04.29: 1.3.13: jcshin: change the value to a number related to the density in pinetree-s.
 * 2014.04.29: 1.3.14: jcshin: adjust copy's darkness value in pinetree-s.
 * 2014.05.05: 1.3.15: jcshin: add function for DB string information.
 * 2014.05.05: 1.3.16: jcshin: move the position of fax information.
 * 2014.05.05: 1.3.17: jcshin: modify system report item position.
 * 2014.05.06: 1.4.0: jcshin: change the font size of addressbook page & increase IPM stack size(64KB->1MB).
 * 2014.05.06: 1.4.1: jcshin: move the position of 'no' in addressbook page.
 * 2014.05.06: 1.4.2: jcshin: change the scan fileformat text.
 * 2014.05.13: 1.4.3: jcshin: add error code information.
 * 2014.05.15: 1.4.4: jcshin: modify wrong reference.
 * 2014.06.16: 1.4.5: jcshin: add the function for displaying DBVERSION.
 * 2014.07.10: 1.5.0: jcshin: add barcode format.
 * 2014.07.10: 1.5.1: jcshin: add toner chart.
 * 2014.07.11: 1.5.2: jcshin: add mac value barcode.
 * 2014.07.14: 1.5.3: jcshin: change drum/toner chart threshold(30%->70%).
 * 2014.07.15: 1.5.4: jcshin: change chart format.
 * 2014.07.23: 1.5.5: jcshin: add print/copy/scan margin information.
 * 2014.08.04: 1.5.6: jcshin: add date/time information.
 * 2014.08.04: 1.5.7: jcshin: add date/time information(modified).
 * 2014.08.27: 1.6.0: jcshin: change the system report format.
 * 2014.08.28: 1.6.1: jcshin: add the 'Recycled paper' type.
 * 2014.09.12: 1.6.2: jcshin: add the 'Recycled' Paper Type(Texture, Weight).
 * 2014.11.10: 1.7.0: jcshin: remove Tray3 in ABBA2/OAK2.
 * 2014.11.21: 1.7.1: jcshin: remove WIFI information in China TLI.
 * 2014.11.26: 1.7.2: jcshin: remove Tray3 in PINETREE-L.
 * 2014.11.28: 1.7.3: jcshin: increase MAX_TRAY value(temporary).
 * 2014.12.04: 1.7.4: jcshin: change MPT tray DB number.
 * 2014.12.05: 1.7.5: jcshin: change the tray information format.
 * 2014.12.12: 1.7.6: jcshin: remove fax date/time information.
 * 2014.12.30: 1.7.7: jcshin: remove SDAPL/change memory size value.
 * 2015.05.11: 1.7.8: inhwan5: add IS_PINETREEL
 * 2015.06.01: 1.7.9: inhwan5: add fax_thumbnail image on txrxresultreport.tmpl
 * 2015.06.02: 1.7.10: inhwan5: the size of fax_thumbnail image was changed (1800*1800)
 * 2015.06.05: 1.7.11: inhwan5: add IS_MFP, IS_TFT
 * 2016.11.08: 1.7.12: inhwan5: add FuserCount features
 * 2017.05.11: 1.7.13: inhwan5: Information of FuserCount was moved from system report to service report
 * 2017.06.28: 1.7.14: inhwan5: fix scalecustom value of copyset (8->6)
 * 2021.06.09: 1.8.1: inhwan5: add motor_info in spreport and so many changes
 */

char REPORT_VERSION[] = {"1.8.1"};
char * get_version_report(void)
{
	return &REPORT_VERSION[0];
}

#define NETWORK_TEMPLATE_STRING (const char *)config_templates_network_tmpl
#define NETWORK_PCL_PATH "/tmp/network.pcl"
#define NETWORK_JSON_PATH "/tmp/network.json"

#define SYSTEM_TEMPLATE_STRING (const char *)config_templates_system_tmpl
#define SYSTEM_PCL_PATH "/tmp/system.pcl"
#define SYSTEM_JSON_PATH "/tmp/system.json"

#define SERVICE_TEMPLATE_STRING (const char *)config_templates_service_tmpl
#define SERVICE_PCL_PATH "/tmp/service.pcl"
#define SERVICE_JSON_PATH "/tmp/service.json"

#ifndef HAVE_SDHFAX
#else // #ifndef HAVE_SDHFAX
#define TXRXRESULTREPORT_TEMPLATE_STRING (const char *)config_templates_txrxresultreport_tmpl
#define TXRXRESULTREPORT_PCL_PATH "/tmp/txrxresultreport.pcl"
#define TXRXRESULTREPORT_JSON_PATH "/tmp/txrxresultreport.json"

#define TCRREPORT_TEMPLATE_STRING (const char *)config_templates_tcrreport_tmpl
#define TCRREPORT_PCL_PATH "/tmp/tcrreport.pcl"
#define TCRREPORT_JSON_PATH "/tmp/tcrreport.json"

#define T30LOGREPORT_TEMPLATE_STRING (const char *)config_templates_t30logreport_tmpl
#define T30LOGREPORT_PCL_PATH "/tmp/t30logreport.pcl"
#define T30LOGREPORT_JSON_PATH "/tmp/t30logreport.json"

#define MEMTXJOBLIST_TEMPLATE_STRING (const char *)config_templates_memtxjoblist_tmpl
#define MEMTXJOBLIST_PCL_PATH "/tmp/memtxjoblist.pcl"
#define MEMTXJOBLIST_JSON_PATH "/tmp/memtxjoblist.json"

#define TCRFORFAXSERVICE_TEMPLATE_STRING (const char *)config_templates_tcrforfaxservice_tmpl
#define TCRFORFAXSERVICE_PCL_PATH "/tmp/tcrforfaxservice.pcl"
#define TCRFORFAXSERVICE_JSON_PATH "/tmp/tcrforfaxservice.json"

#define SPAMFAXLIST_TEMPLATE_STRING (const char *)config_templates_spamfaxlist_tmpl
#define SPAMFAXLIST_PCL_PATH "/tmp/spamfaxlist.pcl"
#define SPAMFAXLIST_JSON_PATH "/tmp/spamfaxlist.json"

#define BROADCAST_RESULT_REPORT_TEMPLATE_STRING (const char *)config_templates_broadcastresultreport_tmpl
#define BROADCASTRESULTREPORT_PCL_PATH "/tmp/broadcastresultreport.pcl"
#define BROADCASTRESULTREPORT_JSON_PATH "/tmp/broadcastresultreport.json"
#endif // #ifndef HAVE_SDHFAX

#define ERRLOG_TEMPLATE_STRING (const char *)config_templates_errlog_tmpl
#define ERRLOG_PCL_PATH "/tmp/errlog.pcl"
#define ERRLOG_JSON_PATH "/tmp/errlog.json"

#define ADDRESSBOOK_TEMPLATE_STRING (const char *)config_templates_addressbook_tmpl
#define ADDRESSBOOK_PCL_PATH "/tmp/addressbook.pcl"
#define ADDRESSBOOK_JSON_PATH "/tmp/addressbook.json"

void network_report(void);
void system_report(void);
void get_snapshot_version(char* snapshot_version);
void service_report(void);
#ifndef HAVE_SDHFAX
#else // #ifndef HAVE_SDHFAX
void tx_rx_result_report(void);
void tcr_report(void);
void t30_log_report(void);
void mem_tx_job_list(void);
void tcr_for_fax_service(void);
void spam_fax_list(void);
void broadcast_result_report(void);

extern UINT8 getCountryDateType(void);
#endif // #ifndef HAVE_SDHFAX

extern bool bSepCartridge;
void errlog_report(void);
void addressbook_report(void);
TMPL_varlist* add_db_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_db_str_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName, int strSize);
TMPL_varlist* add_db_minus1_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_err_code_value(TMPL_varlist* varlist, char* varName, int err_code);
TMPL_varlist* add_paper_size_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_paper_type_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_tray_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_tray_paper_size_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_onoff_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_content_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_quality_2lv_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_scale_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_nup_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_color_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_fileformat_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName_format, char* colName_type);
TMPL_varlist* add_quality_3lv_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_default_dest_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_page_type_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_resolution_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_fax_line_monitor_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_fax_image_type_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_fax_resolution_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_copy_duplex_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_scan_duplex_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_fax_print_mode_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_darkness_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_darkness_plus1_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_bgremove_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_db_version_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_trans_count_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName);
TMPL_varlist* add_tli_header(TMPL_varlist* varlist);
TMPL_varlist* add_datetime(TMPL_varlist* varlist);
/**
 *
 * \brief callback for notification when parser is done with demo page
 *
 */
static void _network_page_done( char *path, void *context )
{
    SDH_DEBUG("network page done!\n");
}

static void _system_page_done( char *path, void *context )
{
    SDH_DEBUG("system page done!\n");
}

static void _service_page_done( char *path, void *context )
{
    SDH_DEBUG("service page done!\n");
}

#ifndef HAVE_SDHFAX
#else // #ifndef HAVE_SDHFAX
static void _tx_rx_result_report_page_done( char *path, void *context )
{
    SDH_DEBUG("tx rx result report page done!\n");
}

static void _tcr_report_page_done( char *path, void *context )
{
    SDH_DEBUG("tcr report page done!\n");
}

static void _t30_log_report_page_done( char *path, void *context )
{
    SDH_DEBUG("t30 log report page done!\n");
}

static void _mem_tx_job_list_page_done( char *path, void *context )
{
    SDH_DEBUG("mem tx job list page done!\n");
}

static void _tcr_for_fax_service_page_done( char *path, void *context )
{
    SDH_DEBUG("mem tx job list page done!\n");
}

static void _spam_fax_list_page_done( char *path, void *context )
{
    SDH_DEBUG("spam fax list page done!\n");
}

static void _broadcast_result_report_page_done( char *path, void *context )
{
    SDH_DEBUG("broadcast result report page done!\n");
}
#endif // #ifndef HAVE_SDHFAX

static void _errlog_page_done( char *path, void *context )
{
    SDH_DEBUG("errlog page done!\n");
}

static void _addressbook_page_done( char *path, void *context )
{
    SDH_DEBUG("addressbook page done!\n");
}

static error_type_t _none_invoke_page( const char *name, void *context )
{
    error_type_t e_res;
    e_res = OK;
	SDH_ERR("DOC TYPE : none!!!!!!!!\n");
    return e_res;
}

static error_type_t _network_page_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    network_report();
    file_to_pipe(NETWORK_PCL_PATH, _network_page_done, NULL, e_NETWORKREPORT);

    return e_res;
}

static error_type_t _system_page_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    system_report();
    file_to_pipe(SYSTEM_PCL_PATH, _system_page_done, NULL, e_SYSTEMREPORT);

    return e_res;
}

static error_type_t _service_page_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    service_report();
    file_to_pipe(SERVICE_PCL_PATH, _service_page_done, NULL, e_SPREPORT);

    return e_res;
}

#ifndef HAVE_SDHFAX
#else // #ifndef HAVE_SDHFAX
static error_type_t _tx_rx_result_report_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    tx_rx_result_report();
    file_to_pipe(TXRXRESULTREPORT_PCL_PATH, _tx_rx_result_report_page_done, NULL, e_FAXTXRXREPORT);

    return e_res;
}

static error_type_t _tcr_report_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    tcr_report();
    file_to_pipe(TCRREPORT_PCL_PATH, _tcr_report_page_done, NULL, e_FAXTCR);

    return e_res;
}

static error_type_t _t30_log_report_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    t30_log_report();
    file_to_pipe(T30LOGREPORT_PCL_PATH, _t30_log_report_page_done, NULL, e_T30LOGREPORT);

    return e_res;
}

static error_type_t _mem_tx_job_list_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    mem_tx_job_list();
    file_to_pipe(MEMTXJOBLIST_PCL_PATH, _mem_tx_job_list_page_done, NULL, e_MEMTXJOBLIST);

    return e_res;
}

static error_type_t _tcr_for_fax_service_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    tcr_for_fax_service();
    file_to_pipe(TCRFORFAXSERVICE_PCL_PATH, _tcr_for_fax_service_page_done, NULL, e_TCRFORFAXSERVICE);

    return e_res;
}

static error_type_t _spam_fax_list_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    spam_fax_list();
    file_to_pipe(SPAMFAXLIST_PCL_PATH, _spam_fax_list_page_done, NULL, e_SPAMFAXLIST);

    return e_res;
}

static error_type_t _broadcast_result_report_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    broadcast_result_report();
    file_to_pipe(BROADCASTRESULTREPORT_PCL_PATH, _broadcast_result_report_page_done, NULL, e_BROADCASTRESULTREPORT);

    return e_res;
}
#endif // #ifndef HAVE_SDHFAX

static error_type_t _errlog_page_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    errlog_report();
    file_to_pipe(ERRLOG_PCL_PATH, _errlog_page_done, NULL, e_ERRLOGREPORT);

    return e_res;
}

static error_type_t _addressbook_page_invoke_page( const char *name, void *context )
{
    error_type_t e_res;

    e_res = OK;

    addressbook_report();
    file_to_pipe(ADDRESSBOOK_PCL_PATH, _addressbook_page_done, NULL, e_ADDRESSBOOK);

    return e_res;
}

void report_manager_init(void)
{
    error_type_t e_res;

	// Doc Type : e_NONE
    e_res = ipage_register_page( "none",
                                 true, /* I start my own job */
                                 _none_invoke_page,
                                 NULL );

    //system
    #define SYSTEM_PAGE_IPAGE_NAME "system"
    e_res = ipage_register_page( SYSTEM_PAGE_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _system_page_invoke_page,
                                 NULL );

	//network
    #define NETWORK_PAGE_IPAGE_NAME "network"
    e_res = ipage_register_page( NETWORK_PAGE_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _network_page_invoke_page,
                                 NULL );

    //service
    #define SERVICE_PAGE_IPAGE_NAME "service"
    e_res = ipage_register_page( SERVICE_PAGE_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _service_page_invoke_page,
                                 NULL );

#ifndef HAVE_SDHFAX
    e_res = ipage_register_page( "none4",
                                 true, /* I start my own job */
                                 _none_invoke_page,
                                 NULL );
    e_res = ipage_register_page( "none5",
                                 true, /* I start my own job */
                                 _none_invoke_page,
                                 NULL );
    e_res = ipage_register_page( "none6",
                                 true, /* I start my own job */
                                 _none_invoke_page,
                                 NULL );                                 
#else // #ifndef HAVE_SDHFAX
    // communication result report
    #define TX_RX_RESULT_REPORT_IPAGE_NAME "tx rx result report"
    e_res = ipage_register_page( TX_RX_RESULT_REPORT_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _tx_rx_result_report_invoke_page,
                                 NULL );
                                 
    #define TCR_REPORT_IPAGE_NAME "tcr report"
    e_res = ipage_register_page( TCR_REPORT_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _tcr_report_invoke_page,
                                 NULL );   
                                 
    #define MEM_TX_JOB_LIST_IPAGE_NAME "mem tx job list"
    e_res = ipage_register_page( MEM_TX_JOB_LIST_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _mem_tx_job_list_invoke_page,
                                 NULL );                                                               
#endif // #ifndef HAVE_SDHFAX

    //error log
    #define ERRLOG_PAGE_IPAGE_NAME "errlog"
    e_res = ipage_register_page( ERRLOG_PAGE_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _errlog_page_invoke_page,
                                 NULL );

#ifndef HAVE_SDHFAX
    e_res = ipage_register_page( "none8",
                                 true, /* I start my own job */
                                 _none_invoke_page,
                                 NULL );
    e_res = ipage_register_page( "none9",
                                 true, /* I start my own job */
                                 _none_invoke_page,
                                 NULL );                              
#else // #ifndef HAVE_SDHFAX     
    #define T30_LOG_REPORT_IPAGE_NAME "t30 log report"
    e_res = ipage_register_page( T30_LOG_REPORT_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _t30_log_report_invoke_page,
                                 NULL );
    #define TCR_FOR_FAX_SERVICE_IPAGE_NAME "tcr for fax service"
    e_res = ipage_register_page( TCR_FOR_FAX_SERVICE_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _tcr_for_fax_service_invoke_page,
                                 NULL );                             
#endif // #ifndef HAVE_SDHFAX

    //address book
    #define ADDRESSBOOK_PAGE_IPAGE_NAME "addressbook"
    e_res = ipage_register_page( ADDRESSBOOK_PAGE_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _addressbook_page_invoke_page,
                                 NULL );

#ifndef HAVE_SDHFAX
    e_res = ipage_register_page( "none11",
                                 true, /* I start my own job */
                                 _none_invoke_page,
                                 NULL );

    e_res = ipage_register_page( "none12",
                                 true, /* I start my own job */
                                 _none_invoke_page,
                                 NULL );                                                              
#else // #ifndef HAVE_SDHFAX     
    #define SPAM_FAX_LIST_IPAGE_NAME "spam fax list"
    e_res = ipage_register_page( SPAM_FAX_LIST_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _spam_fax_list_invoke_page,
                                 NULL );     

    #define BROADCAST_RESULT_REPORT_IPAGE_NAME "broadcast result report"
    e_res = ipage_register_page( BROADCAST_RESULT_REPORT_IPAGE_NAME,
                                 true, /* I start my own job */
                                 _broadcast_result_report_invoke_page,
                                 NULL );                                                        
#endif // #ifndef HAVE_SDHFAX

    ASSERT( OK == e_res );
}

void get_command(const char *cmd, char *result)
{
    FILE *fp;
    fp = popen(cmd, "r");
    if ( NULL == fp )
    {
		result[0] = ' '; result[1] = '\0';
        return;
    }
    fgets(result, 256, fp);
    result[strlen(result) - 1] = '\0';
    pclose(fp);
}

#define BUFFER_SIZE_REPORT 128
#define BUFFER_SIZE_REPORT_SMALL 30

#define BUFFER_SIZE_SNAPSHOT_VERSION 100
#define BUFFER_SIZE_VERSION 30
#define BUFFER_SIZE_ATTRNAME 30

void network_report(void) {
    char ip[BUFFER_SIZE_REPORT];
    char mask[BUFFER_SIZE_REPORT];
    char gateway[BUFFER_SIZE_REPORT];
    char mac[BUFFER_SIZE_REPORT];
    memset(ip, 0, sizeof(ip));
    memset(mask, 0, sizeof(mask));
    memset(gateway, 0, sizeof(gateway));
    memset(mac, 0, sizeof(mac));

	char buffer[BUFFER_SIZE_REPORT];
	memset(buffer, 0, sizeof(buffer));
    
	//get_command("ifconfig eth0 | grep inet | cut -d ':' -f 2 | cut -d ' ' -f 1", ip);
    //oid_get_string(OID_SINDOH_GET_IP, 1, ip, 20);
    //get_command("ifconfig eth0 | grep inet | cut -d ':' -f 4", mask);
    //get_command("ip route show default | awk '/default/ {print $3}'", gateway);
    //get_command("ifconfig | grep eth0 | cut -d 'H' -f 2 | cut -d ' ' -f 2", mac);
    
    SDH_ERR("========== Print network report ==========\n");
    SDH_ERR("OID set\n");

    FILE *fout = fopen(NETWORK_JSON_PATH, "w");
    TMPL_varlist *varlist;
    TMPL_fmtlist *fmtlist;

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
    varlist = TMPL_add_var(0, "c1", "300", 0);
    varlist = TMPL_add_var(varlist, "c2", "1200", 0);
	varlist = add_tli_header(varlist);

	unsigned int if_wired_index = 2;
	if_wired_index = if_nametoindex("eth0");

	// Wire
	// IP address
    oid_get_string(OID_NETWORK_IPV4_ADDRESS, (uint32_t)if_wired_index , buffer, BUFFER_SIZE_REPORT);
	SDH_ERR("Network IP : %s\n", buffer);
    varlist = TMPL_add_var(varlist, "ip", buffer, 0);
	// Subnet mask
	memset(buffer, 0, sizeof(buffer));
    oid_get_string(OID_NETWORK_IPV4_SUBNET_MASK, (uint32_t)if_wired_index, buffer, BUFFER_SIZE_REPORT);
	SDH_ERR("Network MASK : %s\n", buffer);
    varlist = TMPL_add_var(varlist, "netmask", buffer, 0);
	// Default gateway
	memset(buffer, 0, sizeof(buffer));
    oid_get_string(OID_NETWORK_IPV4_DEFAULT_GATEWAY, (uint32_t)if_wired_index, buffer, BUFFER_SIZE_REPORT);
	SDH_ERR("Network GATEWAY : %s\n", buffer);
    varlist = TMPL_add_var(varlist, "gateway", buffer, 0);
	// Mac Adress
	memset(buffer, 0, sizeof(buffer));
    oid_get_string(OID_NETWORK_MAC_ADDRESS, (uint32_t)if_wired_index, buffer, BUFFER_SIZE_REPORT);
	SDH_ERR("Network MAC : %s\n", buffer);
    varlist = TMPL_add_var(varlist, "mac_address", buffer, 0);

	if(gCountry != IS_CHINA)
	{
		varlist = TMPL_add_var(varlist, "is_not_china", "ok", 0);

		// Wireless
		// IP address
		oid_get_string(OID_WIFI_UAP_DHCP_ADDR, 1, buffer, BUFFER_SIZE_REPORT);
		SDH_ERR("Wifi Network IP : %s\n", buffer);
		varlist = TMPL_add_var(varlist, "wifi_ip", buffer, 0);
		// Subnet mask
		memset(buffer, 0, sizeof(buffer));
		oid_get_string(OID_WIFI_UAP_DHCP_SUBNET_MASK, 1, buffer, BUFFER_SIZE_REPORT);
		SDH_ERR("Wifi Network MASK : %s\n", buffer);
		varlist = TMPL_add_var(varlist, "wifi_netmask", buffer, 0);
		// Default gateway
		memset(buffer, 0, sizeof(buffer));
		oid_get_string(OID_WIFI_UAP_DHCP_ADDR_PREFIX, 1, buffer, BUFFER_SIZE_REPORT);
		SDH_ERR("Wifi Network GATEWAY : %s\n", buffer);
		varlist = TMPL_add_var(varlist, "wifi_gateway", buffer, 0);
		// Mac Adress
		memset(buffer, 0, sizeof(buffer));
		oid_get_string(OID_WIFI_INTF_MAC_ADDR, 0, buffer, BUFFER_SIZE_REPORT);
		SDH_ERR("Wifi Network MAC : %s\n", buffer);
		varlist = TMPL_add_var(varlist, "wifi_mac_address", buffer, 0);
	}
	varlist = add_datetime(varlist);

    TMPL_write(0, NETWORK_TEMPLATE_STRING, fmtlist, varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    /*
	// Host name
    oid_get_string(OID_NETWORK_HOST_NAME, 1, host_name, 20);
    strcpy( host_name_text, string_mgr_get_string( STRING_EWS_HOST, string_mgr_loc_full ) );
    strcat( host_name_text, " " );
    strcat( host_name_text, host_name );
	// Domain name
    oid_get_string(OID_NETWORK_IPV4_DOMAIN_NAME, 1, domain_name, 20);
    strcpy( domain_name_text, string_mgr_get_string( STRING_EWS_DOMAIN_IPV4, string_mgr_loc_full ) );
    strcat( domain_name_text, " " );
    strcat( domain_name_text, domain_name );
	*/

    SDH_ERR("DB set\n");

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(NETWORK_JSON_PATH, NETWORK_PCL_PATH);
}

#define MAX_STR_LENGTH_SYSTEM_REPORT 30
void system_report(void) {
	char snapshot_version[BUFFER_SIZE_SNAPSHOT_VERSION];
	int value = 0;
	int ret_db_value = 0;
	char buffer[BUFFER_SIZE_REPORT];
#define NUM_OF_PAPERTYPE 8
	char PAPERTYPE[NUM_OF_PAPERTYPE][20] = {"Plain", "Trans", "PaperLB", "PlasticLB", "Envel", "Card", "Post", "Recycled"};
    int i = 0;
    char attrName[BUFFER_SIZE_ATTRNAME];
    error_type_t db_retval = FAIL;
	float f_value = 0.0;
	memset(snapshot_version, 0, sizeof(snapshot_version));
	memset(buffer, 0, sizeof(buffer));
	memset(attrName, 0, sizeof(attrName));


    SDH_ERR("========== Print system report ==========\n");

    SDH_ERR("OID set\n");

    FILE *fout = fopen(SYSTEM_JSON_PATH, "w");
    TMPL_varlist *varlist;
	TMPL_varlist *loop_paper_size_list;
	TMPL_varlist *loop_paper_type_list;
    TMPL_fmtlist *fmtlist;
	TMPL_loop *loop_paper_size = 0;
	TMPL_loop *loop_paper_type = 0;

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
    varlist = TMPL_add_var(0, "c1", "300", 0);
    varlist = TMPL_add_var(varlist, "c2", "800", 0);
    varlist = TMPL_add_var(varlist, "c3", "1300", 0);
    varlist = TMPL_add_var(varlist, "c4", "600", 0);
    varlist = TMPL_add_var(varlist, "c5", "950", 0);
	varlist = add_tli_header(varlist);

	char tray_chrs[BUFFER_SIZE_REPORT_SMALL];
	char tray_vals[BUFFER_SIZE_REPORT_SMALL];
	char tray_label[BUFFER_SIZE_REPORT_SMALL];

	for(i = 0; i <= MAX_TRAY; i++)
	{
		memset(tray_chrs, 0, sizeof(tray_chrs));
		memset(tray_vals, 0, sizeof(tray_vals));
		memset(tray_label, 0, sizeof(tray_label));
		snprintf(tray_chrs, BUFFER_SIZE_REPORT_SMALL, "TrayConfig_mediaSize_%d", i);
		snprintf(tray_vals, BUFFER_SIZE_REPORT_SMALL, "tray_size");
		if(i == 0)
			snprintf(tray_label, BUFFER_SIZE_REPORT_SMALL, "MPT");
		else
			snprintf(tray_label, BUFFER_SIZE_REPORT_SMALL, "Tray%d", i);
		loop_paper_size_list = TMPL_add_var(0, "tray_label", tray_label, 0);
		loop_paper_size_list = add_paper_size_value(loop_paper_size_list, tray_vals, "CONF_PRNT", tray_chrs);
		loop_paper_size = TMPL_add_varlist(loop_paper_size, loop_paper_size_list);
	}
	varlist = TMPL_add_loop(varlist, "traysizeloop", loop_paper_size);

	for(i = 0; i <= MAX_TRAY; i++)
	{
		memset(tray_chrs, 0, sizeof(tray_chrs));
		memset(tray_vals, 0, sizeof(tray_vals));
		memset(tray_label, 0, sizeof(tray_label));
		snprintf(tray_chrs, BUFFER_SIZE_REPORT_SMALL, "TrayConfig_mediaType_%d", i);
		snprintf(tray_vals, BUFFER_SIZE_REPORT_SMALL, "tray_type");
		if(i == 0)
			snprintf(tray_label, BUFFER_SIZE_REPORT_SMALL, "MPT");
		else
			snprintf(tray_label, BUFFER_SIZE_REPORT_SMALL, "Tray%d", i);
		loop_paper_type_list = TMPL_add_var(0, "tray_label", tray_label, 0);
		loop_paper_type_list = add_paper_type_value(loop_paper_type_list, tray_vals, "CONF_PRNT", tray_chrs);
		loop_paper_type = TMPL_add_varlist(loop_paper_type, loop_paper_type_list);
	}
	varlist = TMPL_add_loop(varlist, "traytypeloop", loop_paper_type);

	memset(attrName, 0, sizeof(attrName));
	for(i = 0; i < NUM_OF_PAPERTYPE; i++)
	{
		ret_db_value = 0;
		memset(buffer, 0, sizeof(buffer));
		snprintf(attrName, BUFFER_SIZE_ATTRNAME, "Texture_%s", PAPERTYPE[i]);
		ret_db_value = db_get_var_int("CONF_PRNT", attrName, &db_retval);
		switch(ret_db_value)
		{
		case 1:
			snprintf(buffer, BUFFER_SIZE_REPORT, "Smooth");
			break;
		case 2:
			snprintf(buffer, BUFFER_SIZE_REPORT, "Normal");
			break;
		case 3:
			snprintf(buffer, BUFFER_SIZE_REPORT, "Rough");
			break;
		default:
			snprintf(buffer, BUFFER_SIZE_REPORT, " ");
			break;
		}
		SDH_DEBUG("%s : %d\n", buffer, ret_db_value);
		varlist = TMPL_add_var(varlist, attrName, buffer, 0);
	}

	memset(attrName, 0, sizeof(attrName));
	for(i = 0; i < NUM_OF_PAPERTYPE; i++)
	{
		ret_db_value = 0;
		memset(buffer, 0, sizeof(buffer));
		snprintf(attrName, BUFFER_SIZE_ATTRNAME, "Weight_%s", PAPERTYPE[i]);
		ret_db_value = db_get_var_int("CONF_PRNT", attrName, &db_retval);
		switch(ret_db_value)
		{
		case 1:
			snprintf(buffer, BUFFER_SIZE_REPORT, "Light");
			break;
		case 2:
			snprintf(buffer, BUFFER_SIZE_REPORT, "Normal");
			break;
		case 3:
			snprintf(buffer, BUFFER_SIZE_REPORT, "Heavy");
			break;
		default:
			snprintf(buffer, BUFFER_SIZE_REPORT, " ");
			break;
		}
		SDH_DEBUG("%s : %d\n", buffer, ret_db_value);
		varlist = TMPL_add_var(varlist, attrName, buffer, 0);
	}

	if ( !IS_A3DEVICE )
		varlist = TMPL_add_var(varlist, "is_not_a3", "ok", 0);
		
	varlist = add_tray_value(varlist, "tray_no", "CONF_PRNT", "Default_Src_Tray");

    varlist = add_db_value(varlist, "count_total", "INST_MACHINECOUNT", "TotalPrint");
    varlist = add_db_value(varlist, "count_host", "INST_MACHINECOUNT", "HostPrint");
    varlist = add_db_value(varlist, "count_report", "INST_MACHINECOUNT", "ReportPrint");
    varlist = add_db_value(varlist, "count_usb", "INST_MACHINECOUNT", "USBDrivePrint");
    varlist = add_db_value(varlist, "count_duplex", "INST_MACHINECOUNT", "DuplexPrint");
    
	if( IS_MFP )
	{
		varlist = add_db_value(varlist, "count_copyprint", "INST_MACHINECOUNT", "CopyPrint");
		varlist = add_db_value(varlist, "count_faxprint", "INST_MACHINECOUNT", "FaxPrint");
		varlist = add_db_value(varlist, "count_scan", "INST_MACHINECOUNT", "Scan");
		varlist = add_db_value(varlist, "count_faxsend", "INST_MACHINECOUNT", "FaxSend");
		varlist = add_db_value(varlist, "count_fbscan", "INST_MACHINECOUNT", "FBScan");
		varlist = add_db_value(varlist, "count_a3scan", "INST_MACHINECOUNT", "A3Scan");
		varlist = add_db_value(varlist, "count_adfscan", "INST_MACHINECOUNT", "ADFScan");
		varlist = add_db_value(varlist, "count_adfduplex", "INST_MACHINECOUNT", "ADFDuplexScan");
	}
    varlist = add_trans_count_value(varlist, "toner_count", "INST_TONERCOUNT", "Count");
    
    if (bSepCartridge)
		varlist = add_db_value(varlist, "toner_remain", "INST_TONERCOUNT", "CountInPercent");
	else
	{
		varlist = TMPL_add_var(varlist, "is_tonerdrum", "ok", 0);
		varlist = TMPL_add_var(varlist, "toner_remain", "0", 0);
		varlist = add_db_value(varlist, "tonerdrum_remain", "INST_TONERCOUNT", "CountInPercent");
	}
	
	varlist = add_db_str_value(varlist, "toner_tli", "INST_MACHINE", "TonerTLI", MAX_STR_LENGTH_SYSTEM_REPORT);
	varlist = add_db_str_value(varlist, "toner_serial", "INST_MACHINE", "TonerSerialNo", MAX_STR_LENGTH_SYSTEM_REPORT);

    varlist = add_trans_count_value(varlist, "drum_count", "INST_DRUMCOUNT", "DrumLifePageCount");
    
    if (bSepCartridge)
		varlist = add_db_value(varlist, "drum_remain", "INST_DRUMCOUNT", "CountInPercent");
	else
		varlist = TMPL_add_var(varlist, "drum_remain", "0", 0);
		
    varlist = add_trans_count_value(varlist, "pc_checker", "INST_DRUMCOUNT", "Count");
    
    //varlist = add_db_value(varlist, "count_fuser", "INST_FUSERCOUNT", "FuserLifePageCount");
    //varlist = add_db_value(varlist, "fuser_reset_action", "INST_FUSERCOUNT", "ResetAction");
    //varlist = add_trans_count_value(varlist, "fuser_checker", "INST_FUSERCOUNT", "count");
    
    //ret_db_value = 0;
	//memset(buffer, 0, sizeof(buffer));
	//ret_db_value = db_get_var_int("INST_FUSERCOUNT", "MotorNormal", &db_retval);
	//f_value = (float)ret_db_value / 100000;
	//snprintf(buffer, sizeof(buffer), "%0.6f", f_value);
    //varlist = TMPL_add_var(varlist, "fuser_motor_normal", buffer, 0);
    
    //ret_db_value = 0;
	//memset(buffer, 0, sizeof(buffer));
	//ret_db_value = db_get_var_int("INST_FUSERCOUNT", "MotorLow", &db_retval);
	//f_value = (float)ret_db_value / 100000;
	//snprintf(buffer, sizeof(buffer), "%0.6f", f_value);
    //varlist = TMPL_add_var(varlist, "fuser_motor_low", buffer, 0);

	ret_db_value = 0;
	memset(buffer, 0, sizeof(buffer));
	ret_db_value = db_get_var_int("INST_DRUMCOUNT", "Motor", &db_retval);
	f_value = (float)ret_db_value / 100000;
	snprintf(buffer, sizeof(buffer), "%0.6f", f_value);
    varlist = TMPL_add_var(varlist, "main_motor", buffer, 0);

	if( IS_MFP )
	{
		varlist = add_db_minus1_value(varlist, "print_darkness", "CONF_PRINTSET", "PrintDarkness");
	}
	else
	{
		varlist = add_db_value(varlist, "print_darkness", "CONF_PRINTSET", "PrintDarkness");
	}

	ret_db_value = 0;
	memset(buffer, 0, sizeof(buffer));
	ret_db_value = db_get_var_int("CONF_PRINTSET", "SettingPriority", &db_retval);
	switch(ret_db_value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Printer setting");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Driver setting");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, " ");
		break;
	}
    varlist = TMPL_add_var(varlist, "setting_priority", buffer, 0);
	varlist = add_db_str_value(varlist, "tli", "INST_MACHINE", "MachineTLI", MAX_STR_LENGTH_SYSTEM_REPORT);
	varlist = add_db_str_value(varlist, "serial", "INST_MACHINE", "MachineSerialNo", MAX_STR_LENGTH_SYSTEM_REPORT);
	varlist = add_db_str_value(varlist, "drum_tli", "INST_MACHINE", "DrumTLI", MAX_STR_LENGTH_SYSTEM_REPORT);
	varlist = add_db_str_value(varlist, "drum_serial", "INST_MACHINE", "DrumSerialNo", MAX_STR_LENGTH_SYSTEM_REPORT);
    varlist = add_db_value(varlist, "powersave_time", "CONF_SYSTEM", "SleepMin");

	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("CONF_SYSTEM", "SleepMin", &db_retval);
	if(value == -1)
		snprintf(buffer, BUFFER_SIZE_VERSION, "Intelligent");
	else
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "powersave_time", buffer, 0);

	memset(snapshot_version, 0, sizeof(snapshot_version));
	get_snapshot_version(snapshot_version);
	varlist = TMPL_add_var(varlist, "snapshot_version", snapshot_version, 0);

	FILE *memory;
	memory=popen("cat /proc/meminfo | grep MemTotal | awk '{print$2}'", "r");
	if (memory==NULL)
	{
		SDH_ERR("memory failed\n");
		varlist = TMPL_add_var(varlist, "mem_total", "Do not know", 0);
	}
	else
	{
		int nMemTotal = 0;
		char memTotal[80]={0,};
		fread(memTotal,1,sizeof(memTotal),memory);
		if (memTotal[strlen(memTotal)-1] == '\n')
			memTotal[strlen(memTotal)-1] = '\0';
		nMemTotal = (atoi(memTotal) + 500) / 1000; // MB
		memset(memTotal, 0, sizeof(memTotal));
		if(nMemTotal > 700)
			snprintf(memTotal, sizeof(memTotal), "1024");
		else
			snprintf(memTotal, sizeof(memTotal), "512");
		varlist = TMPL_add_var(varlist, "mem_total", memTotal, 0);
		pclose(memory);
	}

	FILE *uname;
	uname=popen("uname -sr","r");
	if (uname==NULL)
	{
		SDH_ERR("uname failed\n");
		varlist = TMPL_add_var(varlist, "os_name", "Do not know", 0);
	}
	else
	{
		char osName[80]={0,};
		fread(osName,1,sizeof(osName),uname);
		if (osName[strlen(osName)-1] == '\n')
			osName[strlen(osName)-1] = '\0';
		varlist = TMPL_add_var(varlist, "os_name", osName, 0);
		pclose(uname);
	}

	FILE *df;
	df=popen("df /","r");
	if (df==NULL)
	{
		SDH_ERR("df failed\n");
		varlist = TMPL_add_var(varlist, "fs_total", "256", 0);
		varlist = TMPL_add_var(varlist, "fs_used", "100", 0);
	}
	else
	{
		int tmpFs = 0;
		char tmpchar[160]={0,};
		char FSTotal[80]={0,};
		char FSUsed[80]={0,};
		fgets(tmpchar,sizeof(tmpchar),df);
		SDH_ERR("df: firstline : %s\n",tmpchar);
		fgets(tmpchar,sizeof(tmpchar),df);
		SDH_ERR("df: secondline: %s\n",tmpchar);
		SDH_ERR("parsing...\n");
		strtok(tmpchar," \t");
		snprintf(FSTotal,sizeof(FSTotal),strtok(NULL," \t"));
		snprintf(FSUsed,sizeof(FSUsed),strtok(NULL," \t"));
		pclose(df);

		tmpFs = (atoi(FSTotal) + 500) / 1024; // MB
		memset(FSTotal, 0, sizeof(FSTotal));
		snprintf(FSTotal, sizeof(FSTotal), "%d", tmpFs);
		tmpFs = (atoi(FSUsed) + 500) / 1024; // MB
		memset(FSUsed, 0, sizeof(FSUsed));
		snprintf(FSUsed, sizeof(FSUsed), "%d", tmpFs);

		varlist = TMPL_add_var(varlist, "fs_total", FSTotal, 0);
		varlist = TMPL_add_var(varlist, "fs_used", FSUsed, 0);
	}
	varlist = TMPL_add_var(varlist, "print_lang", "PCL 5e/6, PS3, PDF", 0);

	if( IS_MFP )
	{
		varlist = TMPL_add_var(varlist, "mseries", "ok", 0);
		varlist = add_db_value(varlist, "c_quantity", "CONF_COPYSET", "Quantity");
		varlist = add_paper_size_value(varlist, "c_original_size", "CONF_COPYSET", "OriginalSize");
		varlist = add_tray_value(varlist, "c_print_src_tray", "CONF_COPYSET", "PrintSrcTray");
		varlist = add_tray_paper_size_value(varlist, "c_print_dest_paper", "CONF_COPYSET", "PrintSrcTray");
		varlist = add_scale_value(varlist, "c_scale", "CONF_COPYSET", "Scale");
		varlist = add_onoff_value(varlist, "c_collation", "CONF_COPYSET", "Collation");
		varlist = add_nup_value(varlist, "c_nup", "CONF_COPYSET", "Nup");
		varlist = add_copy_duplex_value(varlist, "c_duplex", "CONF_COPYSET", "Duplex");
		varlist = add_content_value(varlist, "c_prcontents", "CONF_COPYSET", "PRContents");
		if( IS_TFTLCD_ATTACHED )
		{
			varlist = TMPL_add_var(varlist, "is_tftlcd", "ok", 0);
			varlist = add_db_value(varlist, "c_brightness", "CONF_COPYSET", "Brightness");
			varlist = add_db_minus1_value(varlist, "c_darkness", "CONF_COPYSET", "Darkness");
			varlist = add_db_value(varlist, "c_bgremove", "CONF_COPYSET", "BgRemove");
		}
		else
		{
			varlist = add_darkness_value(varlist, "c_brightness", "CONF_COPYSET", "Brightness");
			varlist = add_darkness_plus1_value(varlist, "c_darkness", "CONF_COPYSET", "Darkness");
			varlist = add_bgremove_value(varlist, "c_bgremove", "CONF_COPYSET", "BgRemove");
		}
		varlist = add_quality_2lv_value(varlist, "c_quality", "CONF_COPYSET", "Quality");
		varlist = add_onoff_value(varlist, "c_auto_darkness", "CONF_COPYSET", "AutoDarkness");
		varlist = add_db_value(varlist, "c_timeout_flatbed", "CONF_COPYSET", "TimeoutFlatbed");

		varlist = add_color_value(varlist, "s_color", "CONF_SCANSET", "Color");
		varlist = add_resolution_value(varlist, "s_resolution", "CONF_SCANSET", "Resolution");
		varlist = add_paper_size_value(varlist, "s_original_size", "CONF_SCANSET", "OriginalSize");
		varlist = add_fileformat_value(varlist, "s_fileformat", "CONF_SCANSET", "FileFormat", "PageType");
		varlist = add_quality_3lv_value(varlist, "s_jpeg_quality", "CONF_SCANSET", "JpegQuality");
		if( IS_TFTLCD_ATTACHED )
		{
			varlist = add_db_value(varlist, "s_brightness", "CONF_SCANSET", "Brightness");
		}
		else
		{
			varlist = add_darkness_value(varlist, "s_brightness", "CONF_SCANSET", "Brightness");
		}
		varlist = add_default_dest_value(varlist, "s_default_dest", "CONF_SCANSET", "DefaultDest");
		varlist = add_scan_duplex_value(varlist, "s_duplex", "CONF_SCANSET", "ScanDuplex");
		varlist = add_page_type_value(varlist, "s_page_type", "CONF_SCANSET", "PageType");
		varlist = add_db_value(varlist, "s_timeout_flatbed", "CONF_SCANSET", "TimeoutFlatbed");

		varlist = add_db_str_value(varlist, "f_station_id", "CONF_FAXUP", "StationID", ADDR_MAX_FAXNUMBER_LENGTH);
		varlist = add_db_str_value(varlist, "f_station_number", "CONF_FAXUP", "StationNum", ADDR_MAX_FAXNUMBER_LENGTH);
		varlist = add_onoff_value(varlist, "f_tti", "CONF_FAXUP", "TTI");
		varlist = add_fax_line_monitor_value(varlist, "f_line_monitor", "CONF_FAXUP", "LineMonitor");
		varlist = add_db_value(varlist, "f_volume", "CONF_FAXUP", "Volume");
		varlist = add_fax_resolution_value(varlist, "f_resolution", "CONF_FAXUP", "TxResolution");
		varlist = add_fax_image_type_value(varlist, "f_image_type", "CONF_FAXUP", "TxImageType");
		if( IS_TFTLCD_ATTACHED )
		{
			varlist = add_db_value(varlist, "f_contrast", "CONF_FAXUP", "TxContrast");
			varlist = add_db_value(varlist, "f_bgremove", "CONF_FAXUP", "BgRemove");
		}
		else
		{
			varlist = add_darkness_value(varlist, "f_contrast", "CONF_FAXUP", "TxContrast");
			varlist = add_bgremove_value(varlist, "f_bgremove", "CONF_FAXUP", "BgRemove");
		}
		varlist = add_onoff_value(varlist, "f_prefix_set", "CONF_FAXUP", "TxPrefixSet");
		varlist = add_db_str_value(varlist, "f_prefix", "CONF_FAXUP", "TxPrefixNum", ADDR_MAX_FAXNUMBER_LENGTH);
		varlist = add_db_value(varlist, "f_retry_num", "CONF_FAXUP", "TxRetryNum");
//		varlist = add_db_value(varlist, "", "CONF_FAXUP", "TxRetryInterval");
		varlist = add_onoff_value(varlist, "f_ecm", "CONF_FAXUP", "TxECM");
		varlist = add_onoff_value(varlist, "f_forward_set", "CONF_FAXUP", "RxForwardSet");
		varlist = add_db_str_value(varlist, "f_station_num", "CONF_FAXUP", "RxForwardNum", ADDR_MAX_FAXNUMBER_LENGTH);
		varlist = add_onoff_value(varlist, "f_rx_conf", "CONF_FAXUP", "RxConfSet");
		varlist = add_db_value(varlist, "f_ring_num", "CONF_FAXUP", "RxRingNum");
		varlist = add_onoff_value(varlist, "f_reduction_sel", "CONF_FAXUP", "RxReductionSel");
//		varlist = add_onoff_value(varlist, "", "CONF_FAXUP", "RepTCRSet");
//		varlist = add_onoff_value(varlist, "", "CONF_FAXUP", "RepTxRxSet");
		varlist = add_onoff_value(varlist, "f_tx_conf", "CONF_FAXUP", "TxConfSet");
		varlist = add_paper_size_value(varlist, "f_original_size", "CONF_FAXUP", "TxOriginalSize");
		varlist = add_db_value(varlist, "f_timeout_flatbed", "CONF_FAXUP", "TimeoutFlatbed");
		varlist = add_fax_print_mode_value(varlist, "f_print_mode", "CONF_FAXUP", "PrintMode");
//		varlist = add_db_value(varlist, "", "CONF_FAXUP", "DiscardLength");
		varlist = add_onoff_value(varlist, "f_duplex_print", "CONF_FAXUP", "DuplexPrint");
		varlist = add_tray_value(varlist, "f_input_tray", "CONF_FAXUP", "FaxInputTray");
		varlist = add_onoff_value(varlist, "f_duplex_scan", "CONF_FAXUP", "DuplexScan");
		varlist = add_onoff_value(varlist, "f_auto_darkness", "CONF_FAXUP", "AutoDarkness");
		varlist = add_onoff_value(varlist, "f_tx_alarm", "CONF_FAXUP", "TxAlarm");
		varlist = add_onoff_value(varlist, "f_rx_alarm", "CONF_FAXUP", "RxAlarm");
	}
	varlist = add_datetime(varlist);

    TMPL_write(0, SYSTEM_TEMPLATE_STRING, fmtlist, varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(SYSTEM_JSON_PATH, SYSTEM_PCL_PATH);
}

void get_snapshot_version(char* snapshot_version)
{
	FILE *snapshot_file;
	snapshot_file = popen("/snapshot_version.sh -v","r");
	if( snapshot_file == NULL)
	{
		SDH_ERR("Snapshot file not found\n");
		strncpy(snapshot_version, "not found", 9);
		snapshot_version[9] = 0;
	}
	else
	{
		int read_char_byte = 0;
		read_char_byte = fread(snapshot_version, 1, 99, snapshot_file);
		snapshot_version[read_char_byte-1] = 0;
		SDH_ERR("Snapshot V:%dB (%s)\n", read_char_byte, snapshot_version);
		pclose(snapshot_file);
	}
	return;
}

void service_report(void) {
    
    SDH_ERR("========== Print service report ==========\n");
    SDH_ERR("DB set\n");

	int total = 0;
	int value = 0;
	char buffer[BUFFER_SIZE_REPORT] = {0, };
	char* pTonerTLI = NULL;
	char* pSerial = NULL;
	char* pTonerSerial = NULL;
	int ret_db_value = 0;
    error_type_t db_retval = FAIL;
	int *pValueArray = NULL;
	char fw_version[BUFFER_SIZE_VERSION];
	float f_value = 0.0;

    FILE *fout = fopen(SERVICE_JSON_PATH, "w");
    TMPL_varlist *varlist;
    TMPL_fmtlist *fmtlist;

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
    varlist = TMPL_add_var(0, "c1", "300", 0);
    varlist = TMPL_add_var(varlist, "c2", "1200", 0);
	varlist = add_tli_header(varlist);
	if( IS_MFP )
	{
		varlist = TMPL_add_var(varlist, "mseries", "ok", 0);
	}
    varlist = add_db_value(varlist, "left_margin", "CONF_Margin", "LEFT");
    varlist = add_db_value(varlist, "top_margin", "CONF_Margin", "TOP");
    varlist = add_db_value(varlist, "back_left_margin", "CONF_DuplexMargin", "LEFT");
    varlist = add_db_value(varlist, "back_top_margin", "CONF_DuplexMargin", "TOP");
    varlist = add_db_value(varlist, "a4_lt_margin_gap", "CONF_DuplexMargin", "A4_LT_GAP");
	if( IS_MFP )
	{
		varlist = add_db_value(varlist, "copy_left_margin", "CONF_Margin", "COPY_LEFT");
		varlist = add_db_value(varlist, "copy_top_margin", "CONF_Margin", "COPY_TOP");
		varlist = add_db_value(varlist, "copy_back_left_margin", "CONF_DuplexMargin", "COPY_LEFT");
		varlist = add_db_value(varlist, "copy_back_top_margin", "CONF_DuplexMargin", "COPY_TOP");
		varlist = add_db_value(varlist, "scan_fb_left_margin", "CONF_SCANSUB", "FBLeftMargin");
		varlist = add_db_value(varlist, "scan_fb_top_margin", "CONF_SCANSUB", "FBTopMargin");
		varlist = add_db_value(varlist, "scan_adf_left_margin", "CONF_SCANSUB", "ADFLeftMargin");
		varlist = add_db_value(varlist, "scan_adf_top_margin", "CONF_SCANSUB", "ADFTopMargin");
		varlist = add_db_value(varlist, "scan_adf_back_left_margin", "CONF_SCANSUB", "ADFDuplexLeftMargin");
		varlist = add_db_value(varlist, "scan_adf_back_top_margin", "CONF_SCANSUB", "ADFDuplexTopMargin");
		varlist = add_db_value(varlist, "scan_fb_v_mag", "CONF_SCANSUB", "FBVMAG");
		varlist = add_db_value(varlist, "scan_fb_h_mag", "CONF_SCANSUB", "FBHMAG");
		varlist = add_db_value(varlist, "scan_adf_v_mag", "CONF_SCANSUB", "ADFVMAG");
		varlist = add_db_value(varlist, "scan_adf_h_mag", "CONF_SCANSUB", "ADFHMAG");
	}
    varlist = add_db_value(varlist, "v_mag", "CONF_Margin", "VERTICALMAGNIFICATION");
    varlist = add_db_value(varlist, "h_mag", "CONF_Margin", "HORIZONMAGNIFICATION");
    varlist = add_db_value(varlist, "beam_adjust", "CONF_LSUPWM", "B1_PWM1");
	
    pSerial = NULL;
	memset(buffer, 0, sizeof(buffer));
    pSerial = db_get_var("INST_MACHINE", "MachineSerialNo", &db_retval);
	if(pSerial)
	{
		snprintf(buffer, sizeof(buffer), "%s", pSerial);
		MEM_FREE_AND_NULL(pSerial);
    }
    varlist = TMPL_add_var(varlist, "serial_no", buffer, 0);

	pTonerTLI = NULL;
	pTonerSerial = NULL;
	memset(buffer, 0, sizeof(buffer));
    pTonerTLI = db_get_var("INST_MACHINE", "TonerTLI", &db_retval);
    pTonerSerial = db_get_var("INST_MACHINE", "TonerSerialNo", &db_retval);
	if(pTonerTLI && pTonerSerial)
	{
		snprintf(buffer, sizeof(buffer), "%s + %s", pTonerTLI, pTonerSerial);
		MEM_FREE_AND_NULL(pTonerTLI);
		MEM_FREE_AND_NULL(pTonerSerial);
	}
    varlist = TMPL_add_var(varlist, "toner", buffer, 0);

	total = 0;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode1p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp1", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode2p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp2", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode3p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp3", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode4p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp4", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode5p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp5", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode6p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp6", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode11p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp11", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode21p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp21", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINECOUNT", "CountMode50p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "mp50", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", total);
    varlist = TMPL_add_var(varlist, "mptotal", buffer, 0);

	total = 0;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode1p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp1", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode2p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp2", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode3p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp3", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode4p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp4", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode5p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp5", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode6p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp6", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode11p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp11", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode21p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp21", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_DRUMCOUNT", "CountMode50p", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "dp50", buffer, 0);
	total += value;
	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", total);
    varlist = TMPL_add_var(varlist, "dptotal", buffer, 0);

	memset(buffer, 0, sizeof(buffer));
	pValueArray = GetTonerJobByModes();
	if(pValueArray)
	{
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray));
		varlist = TMPL_add_var(varlist, "tp1", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+1));
		varlist = TMPL_add_var(varlist, "tp2", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+2));
		varlist = TMPL_add_var(varlist, "tp3", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+3));
		varlist = TMPL_add_var(varlist, "tp4", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+4));
		varlist = TMPL_add_var(varlist, "tp5", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+5));
		varlist = TMPL_add_var(varlist, "tp6", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+6));
		varlist = TMPL_add_var(varlist, "tp11", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+7));
		varlist = TMPL_add_var(varlist, "tp21", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+8));
		varlist = TMPL_add_var(varlist, "tp50", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+9));
		varlist = TMPL_add_var(varlist, "tptotal", buffer, 0);
	}
	else
	{
		SDH_ERR("TonerJobByModes value(pValueArray) is NULL");
	}

	pValueArray = NULL;
	total = 0;
	memset(buffer, 0, sizeof(buffer));
	pValueArray = GetTonerPrintedByCoverage();
	if(pValueArray)
	{
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray));
		varlist = TMPL_add_var(varlist, "tcover0", buffer, 0);
		total += *(pValueArray);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+1));
		varlist = TMPL_add_var(varlist, "tcover1", buffer, 0);
		total += *(pValueArray+1);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+2));
		varlist = TMPL_add_var(varlist, "tcover2", buffer, 0);
		total += *(pValueArray+2);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+3));
		varlist = TMPL_add_var(varlist, "tcover4", buffer, 0);
		total += *(pValueArray+3);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+4));
		varlist = TMPL_add_var(varlist, "tcover10", buffer, 0);
		total += *(pValueArray+4);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_VERSION, "%d", *(pValueArray+5));
		varlist = TMPL_add_var(varlist, "tcover50", buffer, 0);
		total += *(pValueArray+5);
	}
	else
	{
		SDH_ERR("TonerJobByModes value(pValueArray) is NULL");
	}
	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", total);
    varlist = TMPL_add_var(varlist, "tcovertotal", buffer, 0);

	ret_db_value = 0;
	memset(buffer, 0, sizeof(buffer));
	ret_db_value = db_get_var_int("INST_DRUMCOUNT", "FreeMotorOnDrumCount", &db_retval);
	snprintf(buffer, sizeof(buffer), "%d", ret_db_value);
    varlist = TMPL_add_var(varlist, "dfreemotor", buffer, 0);

	memset(buffer, 0, sizeof(buffer));
	value = GetFreeMotorOnCount();
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "tfreemotor", buffer, 0);

	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_TONERSTATS", "Toner3K", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "toner3k", buffer, 0);
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_TONERSTATS", "Toner6K", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "toner6k", buffer, 0);
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_TONERSTATS", "Toner9K", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "toner9k", buffer, 0);
	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_TONERSTATS", "Toner13K", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "toner13k", buffer, 0);
    value = db_get_var_int("INST_TONERSTATS", "TonerDrum6K", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "tonerdrum6k", buffer, 0);
    value = db_get_var_int("INST_TONERSTATS", "TonerDrum13K", &db_retval);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d", value);
    varlist = TMPL_add_var(varlist, "tonerdrum13k", buffer, 0);

	int row_count = 0;
    char tonertli[MAX_TONERHISTORY_ROW][20];
    char tonerserial[MAX_TONERHISTORY_ROW][20];
	memset(tonertli, 0, sizeof(tonertli));
	memset(tonerserial, 0, sizeof(tonerserial));

	db_retval = db_get_tonerhistory(tonertli, tonerserial, &row_count);
	if(db_retval == FAIL)
	{
		SDH_ERR("RECD_TONERHISTORY DB read fail!!!\n");
	}
	else
	{
		int i = 0;
		char varbuffer[20] = {0, };
		for(i = 0; i < row_count; i++)
		{
			memset(buffer, 0, sizeof(buffer));
			snprintf(buffer, sizeof(buffer), "%s + %s", tonertli[i], tonerserial[i]);
			memset(varbuffer, 0, sizeof(varbuffer));
			snprintf(varbuffer, sizeof(varbuffer), "tonerlog%d", i+1);
			varlist = TMPL_add_var(varlist, varbuffer, buffer, 0);
		}
	}
	
	char usedcount[8][8];
	memset(usedcount, 0, sizeof(usedcount));
	
	db_retval = db_get_fuserhistory(usedcount, &row_count);
	if(db_retval == FAIL)
	{
		SDH_ERR("RECD_FUSERHISTORY DB read fail!!!\n");
	}
	else
	{
		int i = 0;
		char varbuffer1[20] = {0, };
		for(i = 0; i < row_count; i++)
		{
			memset(buffer, 0, sizeof(buffer));
			snprintf(buffer, sizeof(buffer), "%s", usedcount[i]);
			memset(varbuffer1, 0, sizeof(varbuffer1));
			snprintf(varbuffer1, sizeof(varbuffer1), "fuserlog%d", i+1);
			varlist = TMPL_add_var(varlist, varbuffer1, buffer, 0);
		}
	}

	memset(fw_version, 0, sizeof(fw_version));
    // Printer FW version
    oid_get_string(OID_PLATFORM_FW_VERSION, 1, fw_version, 30);
    varlist = TMPL_add_var(varlist, "fw_version", fw_version, 0);
    varlist = add_db_version_value(varlist, "db_ver", "INST_MACHINE", "DBVERSION");

	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINE", "T2Version", &db_retval);
	SDH_ERR("T2 : %d %h", value);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d.%d.%d", (value & 0x00FF0000) >> 16 , (value & 0x0000FF00) >> 8, (value & 0x000000FF));
    varlist = TMPL_add_var(varlist, "t2_version", buffer, 0);

	memset(buffer, 0, sizeof(buffer));
	value = db_get_var_int("INST_MACHINE", "T3Version", &db_retval);
	SDH_ERR("T3 : %d %h", value);
	snprintf(buffer, BUFFER_SIZE_VERSION, "%d.%d.%d", (value & 0x00FF0000) >> 16 , (value & 0x0000FF00) >> 8, (value & 0x000000FF));
    varlist = TMPL_add_var(varlist, "t3_version", buffer, 0);

	FILE *uname_version;
	uname_version=popen("uname -v | awk '{print $5\"/\"$6\"/\"$9\" \"$7\" \"$8}'","r");
	if (uname_version==NULL)
	{
		SDH_ERR("uname_version failed\n");
		varlist = TMPL_add_var(varlist, "os_version", "Do not know", 0);
	}
	else
	{
		char osVersion[80]={0,};
		fread(osVersion,1,sizeof(osVersion),uname_version);
		if (osVersion[strlen(osVersion)-1] == '\n')
			osVersion[strlen(osVersion)-1] = '\0';
		varlist = TMPL_add_var(varlist, "os_version", osVersion, 0);
		pclose(uname_version);
	}

	FILE *pSqliteVer;
	pSqliteVer=popen("sqlite3 -version | awk {'print$1'}", "r");
	if (pSqliteVer==NULL)
	{
		SDH_ERR("sqlite3 version failed\n");
		varlist = TMPL_add_var(varlist, "sqlite_ver", "0.0.0", 0);
	}
	else
	{
		char sqliteVer[80]={0,};
		fread(sqliteVer,1,sizeof(sqliteVer),pSqliteVer);
		if (sqliteVer[strlen(sqliteVer)-1] == '\n')
			sqliteVer[strlen(sqliteVer)-3] = '\0'; // with erase last 2 char
		varlist = TMPL_add_var(varlist, "sqlite_ver", sqliteVer, 0);
		pclose(pSqliteVer);
	}

	char* version_buffer = NULL;
	version_buffer = get_version_UI();
	varlist = TMPL_add_var(varlist, "ui_ver", version_buffer, 0);
	version_buffer = get_version_report();
	varlist = TMPL_add_var(varlist, "report_ver", version_buffer, 0);
	version_buffer = get_version_engine();
	varlist = TMPL_add_var(varlist, "engine_ver", version_buffer, 0);
	version_buffer = get_version_PDLParser();
	varlist = TMPL_add_var(varlist, "pdlp_ver", version_buffer, 0);
	version_buffer = get_version_PJLParser();
	varlist = TMPL_add_var(varlist, "pjlp_ver", version_buffer, 0);
	version_buffer = get_version_GDIParser();
	varlist = TMPL_add_var(varlist, "gdip_ver", version_buffer, 0);
	version_buffer = get_version_SindohParser();
	varlist = TMPL_add_var(varlist, "sdhp_ver", version_buffer, 0);
	if( IS_MFP )
	{
		version_buffer = get_version_FaxApp();
		varlist = TMPL_add_var(varlist, "fax_ver", version_buffer, 0);
		if( IS_TFTLCD_ATTACHED )
		{
			varlist = TMPL_add_var(varlist, "is_tftlcd", "ok", 0);
			version_buffer = get_version_PSGUI();
			varlist = TMPL_add_var(varlist, "ps_gui_ver", version_buffer, 0);
			version_buffer = get_version_PSBrowser();
			varlist = TMPL_add_var(varlist, "ps_browser_ver", version_buffer, 0);
		}
	}
	
	varlist = add_db_value(varlist, "count_fuser", "INST_FUSERCOUNT", "FuserLifePageCount");
	varlist = add_db_value(varlist, "fuser_reset_action", "INST_FUSERCOUNT", "ResetAction");
    varlist = add_trans_count_value(varlist, "fuser_checker", "INST_FUSERCOUNT", "count");
    
    ret_db_value = 0;
	memset(buffer, 0, sizeof(buffer));
	ret_db_value = db_get_var_int("INST_FUSERCOUNT", "MotorNormal", &db_retval);
	f_value = (float)ret_db_value / 100000;
	snprintf(buffer, sizeof(buffer), "%0.6f", f_value);
    varlist = TMPL_add_var(varlist, "fuser_motor_normal", buffer, 0);
    
    ret_db_value = 0;
	memset(buffer, 0, sizeof(buffer));
	ret_db_value = db_get_var_int("INST_FUSERCOUNT", "MotorLow", &db_retval);
	f_value = (float)ret_db_value / 100000;
	snprintf(buffer, sizeof(buffer), "%0.6f", f_value);
    varlist = TMPL_add_var(varlist, "fuser_motor_low", buffer, 0);
    
    ret_db_value = 0;
    memset(buffer, 0, sizeof(buffer));
	ret_db_value = db_get_var_int("CONF_SYSTEMSP", "MotorType", &db_retval);
	if(ret_db_value)
		snprintf(buffer, 4, "New");
	else
		snprintf(buffer, 4, "Old");
	varlist = TMPL_add_var(varlist, "motor_info", buffer, 0);
    
	varlist = add_datetime(varlist);

    TMPL_write(0, SERVICE_TEMPLATE_STRING, fmtlist, varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(SERVICE_JSON_PATH, SERVICE_PCL_PATH);
}

#ifndef HAVE_SDHFAX
#else // #ifndef HAVE_SDHFAX
void tx_rx_result_report(void)
{
    SDH_ERR("========== Print Tx/Rx Result Report ==========\n");

    FILE *fout = fopen(TXRXRESULTREPORT_JSON_PATH, "w");
	if(fout == NULL)
	{
		ASSERT(0);
	}

    TMPL_varlist *varlist;
    TMPL_fmtlist *fmtlist;

    error_type_t db_retval = FAIL;

    struct tm *tm_ptr;
    time_t time;
    struct timeval CurrentTime;
    struct tm *ptm;

    typedef struct {
        char *pStationID;
        char *pStationNum;
        char CurrentTime[64];
        long TimeInSec;
        char Date[6];
        char Time[6];        
        int JobType;
        int ECM;
        int Resolution;
        char Mode[4];
        char *pRemoteNumber[1];
        char RemoteNum[41];
        int RecName;
        char ModulationName[8];
        int ConnectSpeed;
        char Speed[9];
        char ConnectedSpeed[20];
        int Duration;
        char OnReportDuration[6];
        int MinDuration;
        int SecDuration;
        int PageNum;
        char OnReportPageNum[4];
        int Result;
        char OnReportResult[20];
        char String[128];
    } TX_RX_RESULT_REPORT_t;

    TX_RX_RESULT_REPORT_t TxRxResultReport;

	int row_count = 0;
	
	UINT8 dateType = getCountryDateType();

    row_count = db_get_data_count("RECD_FAXLOG", NULL, &db_retval);
	if(db_retval == FAIL)
	{
		SDH_ERR("RECD_FAXLOG DB count fail!!!\n");

        if(fout)
        {
			fclose(fout);
        }

        faxapp_notify_SendFaxSCStatus(FAXAPP_SC_CODE_FAIL_TO_ACCESS_DB);

        return;
	}

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);

    varlist = TMPL_add_var(0, "c1", "10", 0);
    varlist = TMPL_add_var(varlist, "c2", "200", 0);
    varlist = TMPL_add_var(varlist, "c3", "300", 0);
	varlist = TMPL_add_var(varlist, "c4", "175", 0);
	varlist = TMPL_add_var(varlist, "c5", "375", 0);
	varlist = TMPL_add_var(varlist, "c6", "562", 0);	
	varlist = TMPL_add_var(varlist, "c7", "1175", 0);
	varlist = TMPL_add_var(varlist, "c8", "1550", 0);
	varlist = TMPL_add_var(varlist, "c9", "1787", 0);	
	varlist = TMPL_add_var(varlist, "c10", "1975", 0);
	varlist = TMPL_add_var(varlist, "c11", "960", 0);
	varlist = add_tli_header(varlist);

    // Station ID
    TxRxResultReport.pStationID = db_get_var("CONF_FAXUP", "StationID", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationID\n", __FUNCTION__);
    }
    else
    {
        varlist = TMPL_add_var(varlist, "StationID", TxRxResultReport.pStationID, 0);
    }

    // Station Num
    TxRxResultReport.pStationNum = db_get_var("CONF_FAXUP", "StationNum", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationNum\n", __FUNCTION__);
    }
    else
    {
        varlist = TMPL_add_var(varlist, "StationNum", TxRxResultReport.pStationNum, 0);
    }

    // Current Time
    gettimeofday(&CurrentTime, NULL);

    ptm = localtime(&CurrentTime.tv_sec);

    if(ptm == NULL)
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(TxRxResultReport.CurrentTime, sizeof(TxRxResultReport.CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{
			snprintf(TxRxResultReport.CurrentTime, sizeof(TxRxResultReport.CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(TxRxResultReport.CurrentTime, sizeof(TxRxResultReport.CurrentTime), "%04d-%02d-%02d   %02d:%02d", 1900, 1, 1, 0, 0);
		}
	}
    else
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(TxRxResultReport.CurrentTime, sizeof(TxRxResultReport.CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{
			snprintf(TxRxResultReport.CurrentTime, sizeof(TxRxResultReport.CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(TxRxResultReport.CurrentTime, sizeof(TxRxResultReport.CurrentTime), "%04d-%02d-%02d   %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
		}
        
    }

    varlist = TMPL_add_var(varlist, "CurrentTime", TxRxResultReport.CurrentTime, 0);

	if(row_count == 0)
	{
		varlist = TMPL_add_var(varlist, "nodata", "ok", 0);
	}
	else
	{
		// 1. Date, and 2. Time
		TxRxResultReport.TimeInSec = db_get_var_int("RECD_FAXLOG", "DateTime", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... DateTime\n", __FUNCTION__);
		}
		else
		{
			time = (time_t)TxRxResultReport.TimeInSec;

			tm_ptr = localtime(&time);
			if(tm_ptr == NULL)
			{
                snprintf(TxRxResultReport.Date, sizeof(TxRxResultReport.Date), "%02d-%02d", 1, 1);

                snprintf(TxRxResultReport.Time, sizeof(TxRxResultReport.Time), "%02d:%02d", 0, 0);
			}
            else
            {
                snprintf(TxRxResultReport.Date, sizeof(TxRxResultReport.Date), "%02d-%02d", tm_ptr->tm_mon + 1, tm_ptr->tm_mday);

                snprintf(TxRxResultReport.Time, sizeof(TxRxResultReport.Time), "%02d:%02d", tm_ptr->tm_hour, tm_ptr->tm_min);
            }
		}
		printf("(Taeyou) Date : %s, Time : %s\n",TxRxResultReport.Date, TxRxResultReport.Time);

		// 3. Mode
		// 3-1. Job Type
		TxRxResultReport.JobType = db_get_var_int("RECD_FAXLOG", "FaxMode", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... FaxMode\n", __FUNCTION__);
		}
		else
		{
			SDH_ERR("\n(YHCHO) %s() - TxRxResultReport.JobType: %d\n", __FUNCTION__, TxRxResultReport.JobType);

			switch (TxRxResultReport.JobType)
			{
				case 0: // Transmit
					TxRxResultReport.Mode[0] = 'T';
					break;

				case 1: // Receive
					TxRxResultReport.Mode[0] = 'R';
					break;

				case 2: // Poll Tx
					TxRxResultReport.Mode[0] = 'P';
					break;

				case 3: // Fax Forward
					TxRxResultReport.Mode[0] = 'F';
					break;

				case 4: // On-Hook
					TxRxResultReport.Mode[0] = 'O';
					break;

				default:
					TxRxResultReport.Mode[0] = 'T';
					break;
			}
		}

		// 3-2. ECM
		TxRxResultReport.ECM = db_get_var_int("RECD_FAXLOG", "ECM", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... ECM\n", __FUNCTION__);
		}
		else
		{
			switch (TxRxResultReport.ECM)
			{
				case 0: // non-ECM
					TxRxResultReport.Mode[1] = 'X';
					break;

				case 1: // ECM
					TxRxResultReport.Mode[1] = 'E';
					break;

				default:
					TxRxResultReport.Mode[1] = 'X';
					break;
			}
		}

		// 3-3. Resolution
		TxRxResultReport.Resolution = db_get_var_int("RECD_FAXLOG", "Resolution", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... ECM\n", __FUNCTION__);
		}
		else
		{
			switch (TxRxResultReport.Resolution)
			{
				case 0: // Standard
					TxRxResultReport.Mode[2] = 'S';
					break;

				case 1: // Fine
					TxRxResultReport.Mode[2] = 'D';
					break;

				case 3: // 300 DPI
					TxRxResultReport.Mode[2] = 'F';
					break;

				default:
					TxRxResultReport.Mode[2] = 'S';
					break;
			}
		}

		TxRxResultReport.Mode[3] = 0;

		// 4. Remote Num
		TxRxResultReport.pRemoteNumber[0] = db_get_var("RECD_FAXLOG", "RemoteNum", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... RemoteNum\n", __FUNCTION__);
		}
		
		SDH_ERR("\n(YHCHO) %s() - RemoteNum : %s\n", __FUNCTION__,TxRxResultReport.pRemoteNumber[0]);
#if 0
		if(strlen(TxRxResultReport.pRemoteNumber[0]) > 25)
		{
			memset(TxRxResultReport.RemoteNum,0x00,sizeof(TxRxResultReport.RemoteNum));
			strncpy(TxRxResultReport.RemoteNum,TxRxResultReport.pRemoteNumber[0],25);
		}
		else
		{
#endif
			memset(TxRxResultReport.RemoteNum,0x00,sizeof(TxRxResultReport.RemoteNum));
			strncpy(TxRxResultReport.RemoteNum,TxRxResultReport.pRemoteNumber[0],strlen(TxRxResultReport.pRemoteNumber[0])+1);
	//	}
		SDH_ERR("\n(YHCHO) %s() - RemoteNum : %s\n", __FUNCTION__,TxRxResultReport.RemoteNum);
#if 0
		// 5. Modulation Name, and 6. ConnectSpeed
		TxRxResultReport.RecName = db_get_var_int("RECD_FAXLOG", "RecName", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... RecName\n", __FUNCTION__);
		}
		else
		{
			switch (TxRxResultReport.RecName)
			{
                case 0:
                    snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.34");
					break;

                case 1:
                    snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.17");
					break;

                case 2:
                    snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.29");
					break;

                case 3:
                    snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.27ter");
					break;

				default:
					snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.34");
					break;
			}
		}

		TxRxResultReport.ConnectSpeed = db_get_var_int("RECD_FAXLOG", "ConnectSpeed", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... ConnectSpeed\n", __FUNCTION__);
		}
		else
		{
			switch (TxRxResultReport.ConnectSpeed)
			{
				case 0:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"2.4kbps");
					break;

				case 1:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"4.8kbps");
					break;

				case 2:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"7.2kbps");
					break;

				case 3:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"9.6kbps");
					break;

				case 4:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"12kbps");
					break;

				case 5:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"14.4kbps");
					break;

				case 6:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"16.8kbps");
					break;

				case 7:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"19.2kbps");
					break;

				case 8:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"21.6kbps");
					break;

				case 9:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"24kbps");
					break;

				case 10:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"26.4kbps");
					break;

				case 11:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"28.8kbps");
					break;

				case 12:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"31.2kbps");
					break;

				case 13:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"33.6kbps");
					break;

				default:
					snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"33.6kbps");
					break;
			}
		}

		snprintf(TxRxResultReport.ConnectedSpeed, sizeof(TxRxResultReport.ConnectedSpeed), "%s - %s", TxRxResultReport.ModulationName, TxRxResultReport.Speed);
#endif
		// 7. Duration
		TxRxResultReport.Duration = db_get_var_int("RECD_FAXLOG", "Duration", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... Duration\n", __FUNCTION__);
		}
		else
		{
			TxRxResultReport.MinDuration = (TxRxResultReport.Duration/60);
			TxRxResultReport.SecDuration = (TxRxResultReport.Duration%60);

			snprintf(TxRxResultReport.OnReportDuration, sizeof(TxRxResultReport.OnReportDuration), "%02d:%02d", TxRxResultReport.MinDuration, TxRxResultReport.SecDuration);
		}

		// 8. PageNum
		TxRxResultReport.PageNum = db_get_var_int("RECD_FAXLOG", "PageNum", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... PageNum\n", __FUNCTION__);
		}
		else
		{
			snprintf(TxRxResultReport.OnReportPageNum, sizeof(TxRxResultReport.OnReportPageNum), "%03d", TxRxResultReport.PageNum);
		}

		// 9. Result
		TxRxResultReport.Result = db_get_var_int("RECD_FAXLOG", "Result", &db_retval);
		if (db_retval == FAIL)
		{
			SDH_ERR("\n(YHCHO) %s() - db_get_var_int() Error... PageNum\n", __FUNCTION__);
		}
		else
		{
			switch(TxRxResultReport.Result)
			{
				case 0:
					snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "OK");
					break;

				case 2:
					snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "No Answer");
					break;

				case 3:
					snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Line Busy");
					break;

				case 4:
					snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Line Error");
					break;

                case 5:
                    snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Phone Abort");
                    break;

				case 11:
					snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Memory Full");
					break;

				case 14:
					snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Cancelled");
					break;

                case 68:
                    snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Document Jam");
                    break;

                case 71:
                    snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Spam Fax");
                    break;

				default:
					snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "ERROR-#%03d", TxRxResultReport.Result);
					break;
			}
		}

		// 10. Make String...
		varlist = TMPL_add_var(varlist, "TxRxDate", TxRxResultReport.Date, 0);
		varlist = TMPL_add_var(varlist, "TxRxTime", TxRxResultReport.Time, 0);
		varlist = TMPL_add_var(varlist, "TxRxMode", TxRxResultReport.Mode, 0);
		varlist = TMPL_add_var(varlist, "TxRxRemoteNum", TxRxResultReport.RemoteNum, 0);
		//varlist = TMPL_add_var(varlist, "TxRxConnSpeed", TxRxResultReport.ConnectedSpeed, 0);
		varlist = TMPL_add_var(varlist, "TxRxDuration", TxRxResultReport.OnReportDuration, 0);
		varlist = TMPL_add_var(varlist, "TxRxPages", TxRxResultReport.OnReportPageNum, 0);
		varlist = TMPL_add_var(varlist, "TxRxResult", TxRxResultReport.OnReportResult, 0);
	}
	
    TMPL_write(0, TXRXRESULTREPORT_TEMPLATE_STRING, fmtlist, varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    PclConvertJsonToPcl(TXRXRESULTREPORT_JSON_PATH, TXRXRESULTREPORT_PCL_PATH);
}
#endif // #ifndef HAVE_SDHFAX

#define PAGE_PER_ERRLOG 15

#define ErrorType_ERROR	0xE0EE
#define ErrorType_FATAL	0xE0FF

void errlog_report(void) {
    
	error_type_t db_retval = FAIL;
    SDH_ERR("========== Print errlog report ==========\n");
    SDH_ERR("DB set\n");

    FILE *fout = fopen(ERRLOG_JSON_PATH, "w");
    TMPL_varlist *varlist, *loopvarlist, *page_varlist;
    TMPL_fmtlist *fmtlist;
	TMPL_loop * loop, *page_loop;

    struct tm *tm_ptr;
    time_t time;
	char err_no[BUFFER_SIZE_REPORT];
	char err_type[BUFFER_SIZE_REPORT];
	char err_date[BUFFER_SIZE_REPORT];
	char buffer[BUFFER_SIZE_REPORT];
	int i = 0;

	ERRLOG_STRUCT err_result[MAX_ERRLOG_ROW];
	int row_count = 0;

	int current_page_count = 0;
	int total_page_count = 0;

	db_retval = db_get_errlog(err_result, &row_count);
	if(db_retval == FAIL)
	{
		SDH_ERR("RECD_ERRLOG DB read fail!!!\n");
		if(fout)
			fclose(fout);
		return;
	}

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);

	page_loop = 0;
	
	current_page_count = 1;
	if(row_count > MAX_ERRLOG_ROW)
		row_count = MAX_ERRLOG_ROW;

	if(row_count > 0)
		total_page_count = (row_count-1) / PAGE_PER_ERRLOG + 1;
	else
		total_page_count = 1; // no error log data page
	SDH_ERR("total : %d, curr : %d\n", total_page_count, current_page_count);
	if(row_count == 0)
	{
		varlist = TMPL_add_var(0, "nodata", "ok", 0);
		varlist = add_tli_header(varlist);
		varlist = add_datetime(varlist);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", total_page_count);
		varlist = TMPL_add_var(varlist, "total_pcount", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", current_page_count);
		varlist = TMPL_add_var(varlist, "current_pcount", buffer, 0);
		page_loop = TMPL_add_varlist(page_loop, varlist);
		page_varlist = TMPL_add_loop(0, "errtotalloop", page_loop);
	}
	else
	{
		for(current_page_count = 1; current_page_count <= total_page_count; current_page_count++)
		{
			memset(buffer, 0, sizeof(buffer));
			snprintf(buffer, BUFFER_SIZE_REPORT, "%d", total_page_count);
			varlist = TMPL_add_var(0, "total_pcount", buffer, 0);
			varlist = add_tli_header(varlist);
			varlist = add_datetime(varlist);
			memset(buffer, 0, sizeof(buffer));
			snprintf(buffer, BUFFER_SIZE_REPORT, "%d", current_page_count);
			varlist = TMPL_add_var(varlist, "current_pcount", buffer, 0);
			if(current_page_count > 1)
				varlist = TMPL_add_var(varlist, "ifvarname", "ok" , 0);

			loop = 0;

			for(i = (current_page_count-1) * PAGE_PER_ERRLOG; i < (current_page_count-1) * PAGE_PER_ERRLOG + PAGE_PER_ERRLOG; i++)
			{
				if(i >= row_count)
					break;
				SDH_ERR("REPORT DB DATA : %d %d %d\n", err_result[i].err_no, err_result[i].err_type, err_result[i].err_date);

				loopvarlist = TMPL_add_var(0, "c1", "300", 0);
				loopvarlist = TMPL_add_var(loopvarlist, "c2", "1200", 0);

				memset(err_no, 0, sizeof(err_no));
				memset(err_type, 0, sizeof(err_type));
				snprintf(err_no, BUFFER_SIZE_REPORT, "%02d-%03d", err_result[i].err_no/1000, err_result[i].err_no%1000);
				if(err_result[i].err_type == ErrorType_ERROR)
				{
					snprintf(err_type, sizeof(err_type), "Recoverable");
					loopvarlist = add_err_code_value(loopvarlist, "error_desc", err_result[i].err_no);
				}
				else if(err_result[i].err_type == ErrorType_FATAL)
				{
					snprintf(err_type, sizeof(err_type), "Non-Recoverable");
					loopvarlist = add_err_code_value(loopvarlist, "error_desc", err_result[i].err_no);
				}
				else
				{
					snprintf(err_type, sizeof(err_type), "Unknown Type");
					loopvarlist = add_err_code_value(loopvarlist, "error_desc", err_result[i].err_no);
				}
				loopvarlist = TMPL_add_var(loopvarlist, "error_no", err_no, 0);
				loopvarlist = TMPL_add_var(loopvarlist, "error_type", err_type, 0);

				memset(err_date, 0, sizeof(err_date));
				time = (time_t)err_result[i].err_date;
				tm_ptr = localtime(&time);
				if(tm_ptr != NULL)
				{
					snprintf(err_date, sizeof(err_date), "%04d-%02d-%02d   %02d:%02d:%02d", tm_ptr->tm_year + 1900, tm_ptr->tm_mon + 1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
				}
				else
				{
					SDH_ERR("Error log report tm_ptr is NULL!!!");
					snprintf(err_date, sizeof(err_date), "%d", (int)err_result[i].err_date);
				}
				loopvarlist = TMPL_add_var(loopvarlist, "error_date", err_date, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
			}
			varlist = TMPL_add_loop(varlist, "errloop", loop);
			page_loop = TMPL_add_varlist(page_loop, varlist);
		}
		page_varlist = TMPL_add_loop(0, "errtotalloop", page_loop);
	}
	TMPL_write(0, ERRLOG_TEMPLATE_STRING, fmtlist, page_varlist, fout, stderr);

    TMPL_free_varlist(page_varlist);
    TMPL_free_fmtlist(fmtlist);

	if(fout)
		fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(ERRLOG_JSON_PATH, ERRLOG_PCL_PATH);
}

#define PAGE_PER_LINE_ADDRESSBOOK 52

void addressbook_report(void) {
    
	error_type_t db_retval = FAIL;
    SDH_ERR("========== Print Addressbook report ==========\n");
    SDH_ERR("DB set\n");

    FILE *fout = fopen(ADDRESSBOOK_JSON_PATH, "w");
    TMPL_varlist *varlist, *loopvarlist, *page_varlist;
    TMPL_fmtlist *fmtlist;
	TMPL_loop * loop, *page_loop;

	char buffer[BUFFER_SIZE_REPORT];
	int i = 0;

	ADDRESSBOOK_STRUCT addr_result[MAX_ADDRESSBOOK_ENTRY];
	int row_count = 0;
	int line_count = 0;
	int total_line_count = 0;

	int current_page_count = 0;
	int total_page_count = 0;

	db_retval = db_get_addressbook(addr_result, &row_count, &total_line_count);
	if(db_retval == FAIL)
	{
		SDH_ERR("RECD_ERRLOG DB read fail!!!\n");
		if(fout)
			fclose(fout);
		return;
	}

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);

	page_loop = 0;

	if(row_count > MAX_ADDRESSBOOK_ENTRY)
		row_count = MAX_ADDRESSBOOK_ENTRY;

	// calculate total page count
	total_page_count = 1;
	line_count = 0;
	for(i = 0; i < row_count; i++)
	{
		if((line_count + addr_result[i].line_count) > PAGE_PER_LINE_ADDRESSBOOK)
		{
			total_page_count = total_page_count + 1;
			line_count = addr_result[i].line_count;
		}
		else
			line_count = line_count + addr_result[i].line_count;
	}

	SDH_ERR("total : %d, curr : %d\n", total_page_count, current_page_count);

	if(row_count == 0)
	{
		current_page_count = 1;
		varlist = TMPL_add_var(0, "c1", "50", 0);
		varlist = TMPL_add_var(varlist, "c2", "350", 0);
		varlist = add_tli_header(varlist);
		varlist = add_datetime(varlist);

		varlist = TMPL_add_var(varlist, "nodata", "ok", 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", total_page_count);
		varlist = TMPL_add_var(varlist, "total_pcount", buffer, 0);
		memset(buffer, 0, sizeof(buffer));
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", current_page_count);
		varlist = TMPL_add_var(varlist, "current_pcount", buffer, 0);
		page_loop = TMPL_add_varlist(page_loop, varlist);
		page_varlist = TMPL_add_loop(0, "addrtotalloop", page_loop);
	}
	else
	{
		line_count = 0;
		for(i = 0; i < row_count; i++)
		{
			SDH_DEBUG("REPORT DB DATA : |%d|%d|%s|%s|%s|%s|\n", line_count, addr_result[i].line_count, addr_result[i].name, addr_result[i].speeddial, addr_result[i].faxnumber, addr_result[i].email);
			if(((line_count + addr_result[i].line_count) > PAGE_PER_LINE_ADDRESSBOOK) || i == 0)
			{
				if(i > 0)
				{
					varlist = TMPL_add_loop(varlist, "addrloop", loop);
					page_loop = TMPL_add_varlist(page_loop, varlist);
				}
				loop = 0;

				current_page_count = current_page_count + 1;
				varlist = TMPL_add_var(0, "c0", "0", 0);
				varlist = TMPL_add_var(varlist, "c1", "110", 0);
				varlist = TMPL_add_var(varlist, "c2", "410", 0);
				varlist = TMPL_add_var(varlist, "c3", "1210", 0);
				varlist = add_tli_header(varlist);
				varlist = add_datetime(varlist);
				if(current_page_count > 1)
					varlist = TMPL_add_var(varlist, "ifvarname", "ok" , 0);

				memset(buffer, 0, sizeof(buffer));
				snprintf(buffer, BUFFER_SIZE_REPORT, "%d", total_page_count);
				varlist = TMPL_add_var(varlist, "total_pcount", buffer, 0);
				memset(buffer, 0, sizeof(buffer));
				snprintf(buffer, BUFFER_SIZE_REPORT, "%d", current_page_count);
				varlist = TMPL_add_var(varlist, "current_pcount", buffer, 0);

				line_count = addr_result[i].line_count;
			}
			else
				line_count = line_count + addr_result[i].line_count;

			memset(buffer, 0, sizeof(buffer));
			snprintf(buffer, BUFFER_SIZE_REPORT, "%d", i+1);
			loopvarlist = TMPL_add_var(0, "addr_no", buffer, 0);

			loopvarlist = TMPL_add_var(loopvarlist, "addr_name", addr_result[i].name, 0);
			if(strlen(addr_result[i].speeddial) == 0)
				loopvarlist = TMPL_add_var(loopvarlist, "nospeeddial", "ok", 0);
			else
				loopvarlist = TMPL_add_var(loopvarlist, "addr_speeddial", addr_result[i].speeddial, 0);
			if(strlen(addr_result[i].faxnumber) == 0)
				loopvarlist = TMPL_add_var(loopvarlist, "nofaxnumber", "ok", 0);
			else
				loopvarlist = TMPL_add_var(loopvarlist, "addr_faxnumber", addr_result[i].faxnumber, 0);
			if(strlen(addr_result[i].email) == 0)
				loopvarlist = TMPL_add_var(loopvarlist, "noemail", "ok", 0);
			else
				loopvarlist = TMPL_add_var(loopvarlist, "addr_email", addr_result[i].email, 0);
			loop = TMPL_add_varlist(loop, loopvarlist);
		}
		varlist = TMPL_add_loop(varlist, "addrloop", loop);
		page_loop = TMPL_add_varlist(page_loop, varlist);

		page_varlist = TMPL_add_loop(0, "addrtotalloop", page_loop);
	}
	TMPL_write(0, ADDRESSBOOK_TEMPLATE_STRING, fmtlist, page_varlist, fout, stderr);

    TMPL_free_varlist(page_varlist);
    TMPL_free_fmtlist(fmtlist);

	if(fout)
		fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(ADDRESSBOOK_JSON_PATH, ADDRESSBOOK_PCL_PATH);
}

#ifndef HAVE_SDHFAX
#else // #ifndef HAVE_SDHFAX
extern UINT8 gFaxT35CountryID[40];
void t30_log_report(void) 
{
    extern int getFAX_PageNum_ProtocolDumpList(void);
    extern int FAX_StringGenerator_ProtocolDumpList(T30_LOG_REPORT_t * DestBuf, unsigned int pageNum, unsigned int targetPageNum, T30_LOG_RESULT_t* ResultBuf);
    extern char * get_release_date_FaxApp(void);

    T30_LOG_REPORT_t T30LogReport[35];
    T30_LOG_RESULT_t T30Result;

	//error_type_t db_retval = FAIL;
    SDH_ERR("========== Print T30 Log report ==========\n");
    //SDH_ERR("DB set\n");

    FILE *fout = fopen(T30LOGREPORT_JSON_PATH, "w");
    if(fout == NULL)
    {
		ASSERT(0);
	}
    TMPL_varlist *main_varlist, *varlist, *loopvarlist = 0;
    TMPL_fmtlist *fmtlist;
	TMPL_loop * loop, *main_loop;
	
    //struct tm *tm_ptr;
    //time_t time;
    struct timeval CurrentTimeCheck;
    struct tm *ptm;
    
    int total_page = getFAX_PageNum_ProtocolDumpList();
    int current_page = 1;
    //int size = 0;
    int row_count = 0;
    int i = 0;

    UINT8 dateType = getCountryDateType();

    char CurrentTime[64];
    char ReportPage[20];

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);

	main_loop = 0;

	for(; current_page <= total_page; current_page++)
	{
		memset(T30LogReport, 0x00, sizeof(T30LogReport));
		
		if(current_page == 1)
		{
			varlist = TMPL_add_var(0, "c1", "10", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);			
		}
		else
		{
			varlist = TMPL_add_var(0, "ifvarname", "ok" , 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);		
			varlist = TMPL_add_var(varlist, "c1", "10", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);	
		}
			
		varlist = TMPL_add_var(varlist, "c2", "200", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);		
		varlist = TMPL_add_var(varlist, "c3", "300", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c4", "225", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c5", "313", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c6", "563", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c7", "1025", 0);
		main_loop = TMPL_add_varlist(main_loop, loopvarlist);		

		varlist = add_tli_header(varlist);
		main_loop = TMPL_add_varlist(main_loop, varlist);
						
		// Firmware Version
        varlist = TMPL_add_var(varlist, "FirmwareVersion", get_version_FaxApp(), 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		// Firmware Release Date
        varlist = TMPL_add_var(varlist, "FirmwareReleaseDate", get_release_date_FaxApp(), 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		// T.35 Identification
		varlist = TMPL_add_var(varlist, "Identification", gFaxT35CountryID, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		// Current Time
		gettimeofday(&CurrentTimeCheck, NULL);

		ptm = localtime(&CurrentTimeCheck.tv_sec);

		if (ptm == NULL)
		{
			if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
			{
				snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
			}
			else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
			{	
				snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
			}
			else //FAX_COUNTRY_DATE_TYPE_YMD
			{
				snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", 1900, 1, 1, 0, 0);
			}
		}
		else
		{
			if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
			{
				snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
			}
			else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
			{
				snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
			}
			else //FAX_COUNTRY_DATE_TYPE_YMD
			{
				snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
			}
		}

		varlist = TMPL_add_var(varlist, "CurrentTime", CurrentTime, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		//copy dump list
		//function call
		//row_count = FAX_StringGenerator_ProtocolDumpList((char *)ReportString, 70*128, current_page, total_page);
        row_count = FAX_StringGenerator_ProtocolDumpList((T30_LOG_REPORT_t *)T30LogReport, current_page, total_page, &T30Result);
		SDH_ERR(">> T30 Log row_count = %d\n",row_count);
		
		if(row_count == 0)
		{
			loop = 0; //NO PROTOCOL INFORMATION
		}
		else if(row_count > 0)
		{
			loop = 0;
			for(i = 0; i < row_count; i++)
			{		
				loopvarlist = TMPL_add_var(0, "c1", "10", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "c4", "225", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "c5", "313", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "c6", "563", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);

				loopvarlist = TMPL_add_var(loopvarlist, "T30Time", T30LogReport[i].Time, 0);			
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "T30SR", T30LogReport[i].S_R, 0);			
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "T30FCFData", T30LogReport[i].FCFData, 0);			
				loop = TMPL_add_varlist(loop, loopvarlist);

				if(strlen(T30LogReport[i].FIFData) > 0)
				{
					loopvarlist = TMPL_add_var(loopvarlist, "ifFIFData", "ok" , 0);
					loop = TMPL_add_varlist(loop, loopvarlist);
					loopvarlist = TMPL_add_var(loopvarlist, "T30FIFData", T30LogReport[i].FIFData, 0);			
					loop = TMPL_add_varlist(loop, loopvarlist);
					
					if(strlen(T30LogReport[i].FIFData2) > 0)
					{
						loopvarlist = TMPL_add_var(loopvarlist, "ifData2", "ok" , 0);
						loop = TMPL_add_varlist(loop, loopvarlist);
						loopvarlist = TMPL_add_var(loopvarlist, "T30FIFData2", T30LogReport[i].FIFData2, 0);			
						loop = TMPL_add_varlist(loop, loopvarlist);
					}
				}
			}						
		}
		loop = TMPL_add_varlist(loop, loopvarlist);	
	
		varlist = TMPL_add_loop(varlist, "t30loop", loop);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		if(current_page == total_page)
		{
			varlist = TMPL_add_var(varlist, "ifLastPage", "ok" , 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);
			varlist = TMPL_add_var(varlist, "T30Speed", T30Result.Speed, 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);
			varlist = TMPL_add_var(varlist, "T30Resolution", T30Result.Resolution, 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);
			varlist = TMPL_add_var(varlist, "T30Mode", T30Result.Mode, 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);
			varlist = TMPL_add_var(varlist, "T30Coding", T30Result.Coding, 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);
			varlist = TMPL_add_var(varlist, "T30Result", T30Result.Result, 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);																
		}

		snprintf (ReportPage, sizeof(ReportPage), "PAGE %d of %d", current_page, total_page);
		varlist = TMPL_add_var(varlist, "T30PageCount", ReportPage, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
	}

	main_varlist = TMPL_add_loop(0, "t30totalloop", main_loop);

    TMPL_write(0, T30LOGREPORT_TEMPLATE_STRING, fmtlist, main_varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(T30LOGREPORT_JSON_PATH, T30LOGREPORT_PCL_PATH);
}

void tcr_report(void) 
{
    typedef struct {
		char Num[4];
        char CurrentTime[64];
        long TimeInSec;
        char Date[6];
        char Time[6];        
        int JobType;
        int ECM;
        int Resolution;
        char Mode[4];
        char pRemoteNumber[41];
        int RecName;
        char ModulationName[8];
        int ConnectSpeed;
        char Speed[9];
        char ConnectedSpeed[20];
        int Duration;
        char OnReportDuration[6];
        int MinDuration;
        int SecDuration;
        int PageNum;
        char OnReportPageNum[4];
        int Result;
        char OnReportResult[20];
        char String[128];
    } TX_RX_RESULT_REPORT_t;

    TX_RX_RESULT_REPORT_t TxRxResultReport;

	//error_type_t db_retval = FAIL;
    SDH_ERR("========== Print TCR Report ==========\n");
    //SDH_ERR("DB set\n");

    FILE *fout = fopen(TCRREPORT_JSON_PATH, "w");
    if(fout == NULL)
    {
		ASSERT(0);
	}
    TMPL_varlist *main_varlist, *varlist, *loopvarlist;
    TMPL_fmtlist *fmtlist;
	TMPL_loop * loop, *main_loop;
	
    error_type_t db_retval = FAIL;
    
    struct timeval CurrentTimeCheck;
    struct tm *ptm;
    struct tm *tm_ptr;
    time_t time;
    
    char *pStationID;
    char *pStationNum;
    
    FAXLOG_STRUCT fax_result[MAX_FAXLOG_ROW];
    
    int total_page = 1;
    int current_page = 1;
    int row_count = 0;
    int i = 0;
    
    UINT8 dateType = getCountryDateType();
    
    char CurrentTime[64];
	//char ReportString[128];

	//function call
	db_get_faxlog(&fax_result[0], &row_count);
	
	SDH_ERR(">> TCR Report row_count = %d\n",row_count);
	
    // Station ID
    pStationID = db_get_var("CONF_FAXUP", "StationID", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationID\n", __FUNCTION__);
    }

    // Station Num
    pStationNum = db_get_var("CONF_FAXUP", "StationNum", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationNum\n", __FUNCTION__);
    }

	// Current Time
	gettimeofday(&CurrentTimeCheck, NULL);

	ptm = localtime(&CurrentTimeCheck.tv_sec);

    if (ptm == NULL)
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{	
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", 1900, 1, 1, 0, 0);
		}
    }
    else
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
		}
    }

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
	main_loop = 0;
	i = 0;

	for(; current_page <= total_page; current_page++)
	{
		//memset(ReportString, 0x00, sizeof(ReportString));

		varlist = TMPL_add_var(0, "c1", "0", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);			

		varlist = add_tli_header(varlist);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		varlist = TMPL_add_var(varlist, "c2", "200", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);		
		varlist = TMPL_add_var(varlist, "c3", "300", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c4", "150", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c5", "320", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c6", "490", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c7", "660", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c8", "1260", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c9", "1635", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c10", "1855", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c11", "2030", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c12", "960", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
												
		// Station ID
		varlist = TMPL_add_var(varlist, "StationID", pStationID, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// Station Num		
		varlist = TMPL_add_var(varlist, "StationNum", pStationNum, 0);	
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// CurrentTime
		varlist = TMPL_add_var(varlist, "CurrentTime", CurrentTime, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		if(row_count == 0)
		{
			varlist = TMPL_add_var(varlist, "nodata", "ok", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);
		}
		else
		{
			loop = 0;
#if 0
			for(; i < row_count; i++)
#else
			for(i=row_count-1; i >= 0; i--)
#endif
			{
				loopvarlist = TMPL_add_var(0, "c1", "0", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c4", "150", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c5", "320", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c6", "490", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c7", "660", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c8", "1260", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c9", "1635", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c10", "1855", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);				
				loopvarlist = TMPL_add_var(loopvarlist, "c11", "2030", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);		

#if 0
				snprintf(TxRxResultReport.Num, sizeof(TxRxResultReport.Num), "%d", (i+1));
#else
				snprintf(TxRxResultReport.Num, sizeof(TxRxResultReport.Num), "%d", (row_count - i));
#endif
				// 1. Date, and 2. Time
				time = (time_t)fax_result[i].datetime;
				tm_ptr = localtime(&time);
				if(tm_ptr == NULL)
				{
					snprintf(TxRxResultReport.Date, sizeof(TxRxResultReport.Date), "%02d-%02d", 1, 1);
					snprintf(TxRxResultReport.Time, sizeof(TxRxResultReport.Time), "%02d:%02d", 0, 0);					
				}
				else
				{
					snprintf(TxRxResultReport.Date, sizeof(TxRxResultReport.Date), "%02d-%02d", tm_ptr->tm_mon + 1, tm_ptr->tm_mday);
					snprintf(TxRxResultReport.Time, sizeof(TxRxResultReport.Time), "%02d:%02d", tm_ptr->tm_hour, tm_ptr->tm_min);
				}
				
				// 3. Mode
				// 3-1. Job Type
				switch (fax_result[i].faxmode)
				{
					case 0: // Transmit
						TxRxResultReport.Mode[0] = 'T';
						break;

					case 1: // Receive
						TxRxResultReport.Mode[0] = 'R';
						break;

					case 2: // Poll Tx
						TxRxResultReport.Mode[0] = 'P';
						break;

					case 3: // Fax Forward
						TxRxResultReport.Mode[0] = 'F';
						break;

					case 4: // On-Hook
						TxRxResultReport.Mode[0] = 'O';
						break;

					default:
						TxRxResultReport.Mode[0] = 'T';
						break;
				}


				// 3-2. ECM
				switch (fax_result[i].ecm)
				{
					case 0: // non-ECM
						TxRxResultReport.Mode[1] = 'X';
						break;

					case 1: // ECM
						TxRxResultReport.Mode[1] = 'E';
						break;

					default:
						TxRxResultReport.Mode[1] = 'X';
						break;
				}


				// 3-3. Resolution
				switch (fax_result[i].resolution)
				{
					case 0: // Standard
						TxRxResultReport.Mode[2] = 'S';
						break;

					case 1: // Fine
						TxRxResultReport.Mode[2] = 'D';
						break;

					case 3: // 300 DPI
						TxRxResultReport.Mode[2] = 'F';
						break;

					default:
						TxRxResultReport.Mode[2] = 'S';
						break;
				}

				TxRxResultReport.Mode[3] = 0;

				// 4. Remote Num
				memset(TxRxResultReport.pRemoteNumber,0x00,sizeof(TxRxResultReport.pRemoteNumber));
				memcpy(TxRxResultReport.pRemoteNumber, fax_result[i].remotenum, strlen(fax_result[i].remotenum));

				TxRxResultReport.MinDuration = (fax_result[i].duration/60);
				TxRxResultReport.SecDuration = (fax_result[i].duration%60);
				snprintf(TxRxResultReport.OnReportDuration, sizeof(TxRxResultReport.OnReportDuration), "%02d:%02d", TxRxResultReport.MinDuration, TxRxResultReport.SecDuration);

				snprintf(TxRxResultReport.OnReportPageNum, sizeof(TxRxResultReport.OnReportPageNum), "%03d", fax_result[i].pagenum);

				switch(fax_result[i].result)
				{
					case 0:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "OK");
						break;

					case 2:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "No Answer");
						break;

					case 3:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Line Busy");
						break;

					case 4:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Line Error");
						break;

                    case 5:
                        snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Phone Abort");
                        break;

					case 11:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Memory Full");
						break;

					case 14:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Cancelled");
						break;

                    case 68:
                        snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Document Jam");
                        break;

                    case 71:
                        snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Spam Fax");
                        break;

					default:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "ERROR-#%03d", fax_result[i].result);
						break;
				}

				loopvarlist = TMPL_add_var(loopvarlist, "Number", TxRxResultReport.Num, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRDate", TxRxResultReport.Date, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRTime", TxRxResultReport.Time, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRMode", TxRxResultReport.Mode, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRRemoteNum", TxRxResultReport.pRemoteNumber, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				//loopvarlist = TMPL_add_var(loopvarlist, "TCRConnSpeed", TxRxResultReport.ConnectedSpeed, 0);
				//loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRDuration", TxRxResultReport.OnReportDuration, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRPages", TxRxResultReport.OnReportPageNum, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRResult", TxRxResultReport.OnReportResult, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);																					
			}

			varlist = TMPL_add_loop(varlist, "tcrloop", loop);
			main_loop = TMPL_add_varlist(main_loop, varlist);
		}
	}

	main_varlist = TMPL_add_loop(0, "tcrtotalloop", main_loop);

    TMPL_write(0, TCRREPORT_TEMPLATE_STRING, fmtlist, main_varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(TCRREPORT_JSON_PATH, TCRREPORT_PCL_PATH);
}

#define MAX_MEM_LIST_ROW	41
void mem_tx_job_list(void) 
{
	typedef enum {
    FSU_STATE_NULL,     // 0
    FSU_STATE_SCANNING,     // 1
    FSU_STATE_SENDING_POLL_READY,     // 2
    FSU_STATE_SENDING_READY,     // 3
    FSU_STATE_RECEIVING,     // 4
    FSU_STATE_RECEIVE_DONE,     // 5
    FSU_STATE_PRINT_READY,     // 6
    FSU_STATE_RETRY_READY,     // 7

    FSU_STATE_LAST
	} FSU_STATE_t;
	
	extern UINT16 fsu_get_job_mem(int * num);
    extern char *report_manager_request_get_time_to_tx_from_Job_Queue_File(UINT16 DocId);
    extern char *report_manager_request_get_current_time_from_Job_Queue_File(UINT16 DocId);
	extern UINT8 fsu_get_resolution (UINT16 jnum);
	extern UINT8 *fsu_get_dial_no (UINT16 jnum);
	extern UINT8 fsu_get_total_pages (UINT16 jnum);
	extern UINT8 fsu_get_status (UINT16 jnum);
	extern UINT8 fsu_get_fax_forward (UINT16 jnum);
    extern UINT8 fsu_get_is_delay (UINT16 jnum);
	extern UINT16 fsu_get_max_job_num (void);
    extern UINT16 fsu_get_doc_id(UINT16 jnum);
	
    typedef struct {
        char Num[4];
        char CurrentTime[64];
        char Time[6];        
        char Resolution[10];
        char pRemoteNumber[41];
        char DelayTime[17];
        char OnReportPageNum[4];
        char JobType[20];
    } MEMORY_JOB_LIST_REPORT_t;

    MEMORY_JOB_LIST_REPORT_t MemJobReport;
    
    UINT8 *dial_no = NULL;
    
    UINT16 mem_job_num = 0;
    UINT8 dateType = getCountryDateType();

	//error_type_t db_retval = FAIL;
    SDH_ERR("========== Memory TX Job List ==========\n");
    //SDH_ERR("DB set\n");

    FILE *fout = fopen(MEMTXJOBLIST_JSON_PATH, "w");
	if(fout == NULL)
	{
		ASSERT(0);
	}

    TMPL_varlist *main_varlist, *varlist, *loopvarlist;
    TMPL_fmtlist *fmtlist;
	TMPL_loop * loop, *main_loop;
	
    error_type_t db_retval = FAIL;
    
    struct timeval CurrentTimeCheck;
    struct tm *ptm;
   // struct tm *tm_ptr;
   // time_t time;
    
    char *pStationID;
    char *pStationNum;
    
    int current_page = 1;
    int i = 0, j = 0;
    int mem_job_end = 0;
    int detect_job_num = 0;
    int mem_job_row_count = 0;
    int page_change = 0;
    
    char CurrentTime[64];
	
    // Station ID
    pStationID = db_get_var("CONF_FAXUP", "StationID", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationID\n", __FUNCTION__);
    }

    // Station Num
    pStationNum = db_get_var("CONF_FAXUP", "StationNum", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationNum\n", __FUNCTION__);
    }

	// Current Time
	gettimeofday(&CurrentTimeCheck, NULL);

	ptm = localtime(&CurrentTimeCheck.tv_sec);
    if (ptm == NULL)
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{	
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", 1900, 1, 1, 0, 0);
		}
    }
    else
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
		}
    }

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
	main_loop = 0;
	i = 0;

	while(1)
	{		
		if(current_page == 1)
		{
            varlist = TMPL_add_var(0, "c1", "0", 0); // No
			main_loop = TMPL_add_varlist(main_loop, varlist);			
		}
		else
		{
			varlist = TMPL_add_var(0, "ifvarname", "ok" , 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);		
            varlist = TMPL_add_var(varlist, "c1", "0", 0); // No
			main_loop = TMPL_add_varlist(main_loop, varlist);	
		}
		varlist = add_tli_header(varlist);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		varlist = TMPL_add_var(varlist, "c2", "200", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);		
		varlist = TMPL_add_var(varlist, "c3", "300", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c4", "80", 0); // Time
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c5", "250", 0); // Resolution
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c7", "480", 0); // Remote Num
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c8", "1400", 0); // Delay Time
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c9", "1800", 0); // Pages
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c10", "1950", 0); // Job Type
		main_loop = TMPL_add_varlist(main_loop, varlist);
												
		// Station ID
		varlist = TMPL_add_var(varlist, "StationID", pStationID, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// Station Num		
		varlist = TMPL_add_var(varlist, "StationNum", pStationNum, 0);	
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// CurrentTime
		varlist = TMPL_add_var(varlist, "CurrentTime", CurrentTime, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		loop = 0;

		while(1)
		{
			if(page_change == 0)
			{
				mem_job_num = fsu_get_job_mem(&detect_job_num);
				if(mem_job_num >= fsu_get_max_job_num())
				{
					mem_job_end = 1;
					break;
				}
				
				mem_job_row_count++;

				if(!(mem_job_row_count % MAX_MEM_LIST_ROW))
				{
					page_change = 1;
					break;
				}
			}
			else
			{
				page_change = 0;
			}
			
			loopvarlist = TMPL_add_var(0, "c1", "0", 0);
			loop = TMPL_add_varlist(loop, loopvarlist);	
			loopvarlist = TMPL_add_var(loopvarlist, "c4", "80", 0); // Time
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "c5", "250", 0); // Resolution
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "c7", "480", 0); // Remote Num
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "c8", "1400", 0); // Delay Time
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "c9", "1800", 0); // Pages
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "c10", "1950", 0); // Job Type
			loop = TMPL_add_varlist(loop, loopvarlist);
			
			// 1. No
			snprintf(MemJobReport.Num, sizeof(MemJobReport.Num), "%d", (i+1));		

			// 2. Time
           memset(MemJobReport.Time, 0x00, sizeof(MemJobReport.Time));

           snprintf(MemJobReport.Time, sizeof(MemJobReport.Time), "%s", report_manager_request_get_current_time_from_Job_Queue_File(fsu_get_doc_id(mem_job_num)));

			// 3-3. Resolution
			switch (fsu_get_resolution(mem_job_num))
			{
				case 0: // Standard
					snprintf(MemJobReport.Resolution, sizeof(MemJobReport.Resolution), "Standard");
					break;

				case 1: // Fine
					snprintf(MemJobReport.Resolution, sizeof(MemJobReport.Resolution), "Fine");
					break;

				case 3: // 300 DPI
					snprintf(MemJobReport.Resolution, sizeof(MemJobReport.Resolution), "Superfine");
					break;

				default:
					snprintf(MemJobReport.Resolution, sizeof(MemJobReport.Resolution), "Standard");
					break;
			}

			// 4. Remote Num
			//memcpy(MemJobReport.pRemoteNumber, MemTxJob->fax_num, sizeof(MemTxJob->fax_num));
			dial_no = fsu_get_dial_no(mem_job_num);
			if(dial_no == NULL)
			{
				j = 0;
			}
			else
			{
				for(j = 0; j<40; j++)
				{
					if(dial_no[j] != '\0')
					{
						if (dial_no[j] == 0x2C) // pause key
						{
							MemJobReport.pRemoteNumber[j] = 0x2D; // '-' key
						}
						else
						{
							MemJobReport.pRemoteNumber[j] = dial_no[j];
						}
					}
					else
					{
						break;
					}
				}
			}
			MemJobReport.pRemoteNumber[j] = '\0';

            // 5. Delay Time
            if ( (fsu_get_status(mem_job_num) == FSU_STATE_SENDING_READY) && (fsu_get_is_delay(mem_job_num) == 1) )
            {
                snprintf(MemJobReport.DelayTime, sizeof(MemJobReport.DelayTime), "%s", report_manager_request_get_time_to_tx_from_Job_Queue_File(fsu_get_doc_id(mem_job_num)));
            }
            else
            {
                snprintf(MemJobReport.DelayTime, sizeof(MemJobReport.DelayTime), "%c", 0x20);
            }
			
            // 6. Pages
			snprintf(MemJobReport.OnReportPageNum, sizeof(MemJobReport.OnReportPageNum), "%03d", fsu_get_total_pages(mem_job_num));
			
            //7. Job Type
			switch(fsu_get_status(mem_job_num))
			{
				case FSU_STATE_RECEIVING:
				case FSU_STATE_RECEIVE_DONE:
				case FSU_STATE_PRINT_READY:			
					snprintf(MemJobReport.JobType, 20, "Rx");				
					break;
				case FSU_STATE_SENDING_READY:			
					if(fsu_get_fax_forward(mem_job_num))
					{
						snprintf(MemJobReport.JobType, 20, "Forwarding");
					}
                    else if(fsu_get_is_delay(mem_job_num))
					{
						snprintf(MemJobReport.JobType, 20, "DelayTx");		
					}
					else
					{
						snprintf(MemJobReport.JobType, 20, "MemTx");	
					}	
					break;
				case FSU_STATE_SENDING_POLL_READY:			
					snprintf(MemJobReport.JobType, 20, "PollingTx");			
					break;
				case FSU_STATE_RETRY_READY:
					snprintf(MemJobReport.JobType, 20, "PendingTx");
					break;
				default:		
					snprintf(MemJobReport.JobType, 20, "XXXX");				
					break;
			}

			loopvarlist = TMPL_add_var(loopvarlist, "Number", MemJobReport.Num, 0);
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "Time", MemJobReport.Time, 0);
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "Resolution", MemJobReport.Resolution, 0);
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "RemoteNum", MemJobReport.pRemoteNumber, 0);
			loop = TMPL_add_varlist(loop, loopvarlist);
           loopvarlist = TMPL_add_var(loopvarlist, "DelayTime", MemJobReport.DelayTime, 0);
           loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "Pages", MemJobReport.OnReportPageNum, 0);
			loop = TMPL_add_varlist(loop, loopvarlist);
			loopvarlist = TMPL_add_var(loopvarlist, "JobType", MemJobReport.JobType, 0);
			loop = TMPL_add_varlist(loop, loopvarlist);				
			
			i++;																	
		}
		
		if(mem_job_row_count == 0)
		{
			varlist = TMPL_add_var(varlist, "nodata", "ok", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);			
			break;
		}
		
		varlist = TMPL_add_loop(varlist, "memjoblistloop", loop);
		main_loop = TMPL_add_varlist(main_loop, varlist);
	
		if(mem_job_end)
		{
			break;
		}
	
		current_page++;
	}

	main_varlist = TMPL_add_loop(0, "memjobtotalloop", main_loop);

    TMPL_write(0, MEMTXJOBLIST_TEMPLATE_STRING, fmtlist, main_varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(MEMTXJOBLIST_JSON_PATH, MEMTXJOBLIST_PCL_PATH);
}

void tcr_for_fax_service(void)
{
    typedef struct {
        char Num[4];
        char CurrentTime[64];
        long TimeInSec;
        char Date[6];
        char Time[6];        
        int JobType;
        int ECM;
        int Resolution;
        char Mode[4];
        char pRemoteNumber[26];
        int RecName;
        char ModulationName[8];
        int ConnectSpeed;
        char Speed[9];
        char ConnectedSpeed[20];
        int Duration;
        char OnReportDuration[6];
        int MinDuration;
        int SecDuration;
        int PageNum;
        char OnReportPageNum[4];
        int Result;
        char OnReportResult[20];
        char String[128];
    } TX_RX_RESULT_REPORT_t;

    TX_RX_RESULT_REPORT_t TxRxResultReport;

	//error_type_t db_retval = FAIL;
    SDH_ERR("========== Print TCR For Fax Service ==========\n");
    //SDH_ERR("DB set\n");

    FILE *fout = fopen(TCRFORFAXSERVICE_JSON_PATH, "w");
	if(fout == NULL)
	{
		ASSERT(0);
	}

    TMPL_varlist *main_varlist, *varlist, *loopvarlist;
    TMPL_fmtlist *fmtlist;
	TMPL_loop * loop, *main_loop;
	
    error_type_t db_retval = FAIL;
    
    struct timeval CurrentTimeCheck;
    struct tm *ptm;
    struct tm *tm_ptr;
    time_t time;
    
    char *pStationID;
    char *pStationNum;
    
    FAXSPLOG_STRUCT fax_result[MAX_FAXSPLOG_ROW];
    
    int total_page = 1;
    int current_page = 1;
    int row_count = 0;
    int row_check_count = 0;
    int i = 0;
 
    UINT8 dateType = getCountryDateType();
    
    char CurrentTime[64];
	//char ReportString[128];

	//function call
	db_get_faxsplog(&fax_result[0], &row_count);
	
	SDH_ERR(">> TCR for fax service row_count = %d\n",row_count);
	
	if(row_count == 0)
	{
		total_page = 1;
	}
	else if(row_count % MAX_FAXLOG_ROW)
	{
		total_page = (row_count/MAX_FAXLOG_ROW) + 1;
	}
	else
	{
		total_page = (row_count/MAX_FAXLOG_ROW);
	}
	
    // Station ID
    pStationID = db_get_var("CONF_FAXUP", "StationID", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationID\n", __FUNCTION__);
    }

    // Station Num
    pStationNum = db_get_var("CONF_FAXUP", "StationNum", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationNum\n", __FUNCTION__);
    }

	// Current Time
	gettimeofday(&CurrentTimeCheck, NULL);

	ptm = localtime(&CurrentTimeCheck.tv_sec);

    if (ptm == NULL)
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{	
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", 1900, 1, 1, 0, 0);
		}
    }
    else
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
		}
    }

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
	main_loop = 0;
	i = 0;

	for(; current_page <= total_page; current_page++)
	{
		//memset(ReportString, 0x00, sizeof(ReportString));
		
		if(current_page == 1)
		{
			varlist = TMPL_add_var(0, "c1", "0", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);			
		}
		else
		{
			varlist = TMPL_add_var(0, "ifvarname", "ok" , 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);		
			varlist = TMPL_add_var(varlist, "c1", "0", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);	
		}
		varlist = add_tli_header(varlist);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		varlist = TMPL_add_var(varlist, "c2", "200", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);		
		varlist = TMPL_add_var(varlist, "c3", "300", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c4", "150", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c5", "320", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c6", "490", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c7", "660", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c8", "1260", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c9", "1635", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c10", "1855", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c11", "2030", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c12", "960", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
												
		// Station ID
		varlist = TMPL_add_var(varlist, "StationID", pStationID, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// Station Num		
		varlist = TMPL_add_var(varlist, "StationNum", pStationNum, 0);	
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// CurrentTime
		varlist = TMPL_add_var(varlist, "CurrentTime", CurrentTime, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		if(row_count == 0)
		{
			varlist = TMPL_add_var(varlist, "nodata", "ok", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);			
		}
		else
		{
			loop = 0;
			
			if(current_page != total_page)
			{
				row_check_count = current_page*MAX_FAXLOG_ROW;
			}
			else
			{
				row_check_count = row_count;
			}
			SDH_ERR("\n(YHCHO) %s() - row_check_count = %d\n", __FUNCTION__,row_check_count);

			for(; i < row_check_count; i++)
			{
				loopvarlist = TMPL_add_var(0, "c1", "0", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c4", "150", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c5", "320", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c6", "490", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c7", "660", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c8", "1260", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c9", "1635", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c10", "1855", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);				
				loopvarlist = TMPL_add_var(loopvarlist, "c11", "2030", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);		
				
				snprintf(TxRxResultReport.Num, sizeof(TxRxResultReport.Num), "%d", (i+1));		

				// 1. Date, and 2. Time
				time = (time_t)fax_result[i].datetime;
				tm_ptr = localtime(&time);
				if(tm_ptr == NULL)
				{
					snprintf(TxRxResultReport.Date, sizeof(TxRxResultReport.Date), "%02d-%02d", 1, 1);
					snprintf(TxRxResultReport.Time, sizeof(TxRxResultReport.Time), "%02d:%02d", 0, 0);					
				}
				else
				{
					snprintf(TxRxResultReport.Date, sizeof(TxRxResultReport.Date), "%02d-%02d", tm_ptr->tm_mon + 1, tm_ptr->tm_mday);
					snprintf(TxRxResultReport.Time, sizeof(TxRxResultReport.Time), "%02d:%02d", tm_ptr->tm_hour, tm_ptr->tm_min);
				}

				// 3. Mode
				// 3-1. Job Type
				switch (fax_result[i].faxmode)
				{
					case 0: // Transmit
						TxRxResultReport.Mode[0] = 'T';
						break;

					case 1: // Receive
						TxRxResultReport.Mode[0] = 'R';
						break;

					case 2: // Poll Tx
						TxRxResultReport.Mode[0] = 'P';
						break;

					case 3: // Fax Forward
						TxRxResultReport.Mode[0] = 'F';
						break;

					case 4: // On-Hook
						TxRxResultReport.Mode[0] = 'O';
						break;

					default:
						TxRxResultReport.Mode[0] = 'T';
						break;
				}


				// 3-2. ECM
				switch (fax_result[i].ecm)
				{
					case 0: // non-ECM
						TxRxResultReport.Mode[1] = 'X';
						break;

					case 1: // ECM
						TxRxResultReport.Mode[1] = 'E';
						break;

					default:
						TxRxResultReport.Mode[1] = 'X';
						break;
				}


				// 3-3. Resolution
				switch (fax_result[i].resolution)
				{
					case 0: // Standard
						TxRxResultReport.Mode[2] = 'S';
						break;

					case 1: // Fine
						TxRxResultReport.Mode[2] = 'D';
						break;

					case 3: // 300 DPI
						TxRxResultReport.Mode[2] = 'F';
						break;

					default:
						TxRxResultReport.Mode[2] = 'S';
						break;
				}

				TxRxResultReport.Mode[3] = 0;

				// 4. Remote Num
				//memcpy(TxRxResultReport.pRemoteNumber, fax_result[i].remotenum, sizeof(fax_result[i].remotenum));
				if(strlen(fax_result[i].remotenum) > 25)
				{
					memset(TxRxResultReport.pRemoteNumber,0x00,sizeof(TxRxResultReport.pRemoteNumber));
					memcpy(TxRxResultReport.pRemoteNumber, fax_result[i].remotenum, 25);
				}
				else
				{
					memset(TxRxResultReport.pRemoteNumber,0x00,sizeof(TxRxResultReport.pRemoteNumber));
					memcpy(TxRxResultReport.pRemoteNumber, fax_result[i].remotenum, strlen(fax_result[i].remotenum));
				}

				// 5. Modulation Name, and 6. ConnectSpeed
				switch (fax_result[i].recname)
				{
                    case 0:
                        snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.34");
						break;

                    case 1:
                        snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.17");
						break;

                    case 2:
                        snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.29");
						break;

                    case 3:
                        snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.27ter");
						break;

					default:
						snprintf(TxRxResultReport.ModulationName, sizeof(TxRxResultReport.ModulationName),"V.34");
						break;
				}

				switch (fax_result[i].connectspeed)
				{
					case 0:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"2.4kbps");
						break;

					case 1:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"4.8kbps");
						break;

					case 2:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"7.2kbps");
						break;

					case 3:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"9.6kbps");
						break;

					case 4:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"12kbps");
						break;

					case 5:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"14.4kbps");
						break;

					case 6:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"16.8kbps");
						break;

					case 7:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"19.2kbps");
						break;

					case 8:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"21.6kbps");
						break;

					case 9:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"24kbps");
						break;

					case 10:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"26.4kbps");
						break;

					case 11:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"28.8kbps");
						break;

					case 12:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"31.2kbps");
						break;

					case 13:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"33.6kbps");
						break;

					default:
						snprintf(TxRxResultReport.Speed, sizeof(TxRxResultReport.Speed),"33.6kbps");
						break;
				}
				snprintf(TxRxResultReport.ConnectedSpeed, sizeof(TxRxResultReport.ConnectedSpeed), "%s - %s", TxRxResultReport.ModulationName, TxRxResultReport.Speed);

				TxRxResultReport.MinDuration = (fax_result[i].duration/60);
				TxRxResultReport.SecDuration = (fax_result[i].duration%60);
				snprintf(TxRxResultReport.OnReportDuration, sizeof(TxRxResultReport.OnReportDuration), "%02d:%02d", TxRxResultReport.MinDuration, TxRxResultReport.SecDuration);

				snprintf(TxRxResultReport.OnReportPageNum, sizeof(TxRxResultReport.OnReportPageNum), "%03d", fax_result[i].pagenum);

				switch(fax_result[i].result)
				{
					case 0:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "OK");
						break;

					case 2:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "No Answer");
						break;

					case 3:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Line Busy");
						break;

					case 4:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Line Error");
						break;

                    case 5:
                        snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Phone Abort");
                        break;

					case 11:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Memory Full");
						break;

					case 14:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Cancelled");
						break;

                    case 68:
                        snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Document Jam");
                        break;

                    case 71:
                        snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "Spam Fax");
                        break;

					default:
						snprintf(TxRxResultReport.OnReportResult, sizeof(TxRxResultReport.OnReportResult), "ERROR-#%03d", fax_result[i].result);
						break;
				}

				loopvarlist = TMPL_add_var(loopvarlist, "Number", TxRxResultReport.Num, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRDate", TxRxResultReport.Date, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRTime", TxRxResultReport.Time, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRMode", TxRxResultReport.Mode, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRRemoteNum", TxRxResultReport.pRemoteNumber, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRConnSpeed", TxRxResultReport.ConnectedSpeed, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRDuration", TxRxResultReport.OnReportDuration, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRPages", TxRxResultReport.OnReportPageNum, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "TCRResult", TxRxResultReport.OnReportResult, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);																					
			}
			
			varlist = TMPL_add_loop(varlist, "tcrloop", loop);
			main_loop = TMPL_add_varlist(main_loop, varlist);
		}
	
	}

	main_varlist = TMPL_add_loop(0, "tcrtotalloop", main_loop);

    TMPL_write(0, TCRFORFAXSERVICE_TEMPLATE_STRING, fmtlist, main_varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(TCRFORFAXSERVICE_JSON_PATH, TCRFORFAXSERVICE_PCL_PATH);
}

void spam_fax_list(void)
{
    SDH_ERR("========== Spam Fax List ==========\n");

	#define SPAMLIST_NUM_PER_PAGE 40
	#define MAX_SPAM_FAX_LIST 50
	
	extern char gFaxSpamFaxList[MAX_SPAM_FAX_LIST][41];
	extern UINT8 NoOfSpamFaxList;

    typedef struct {
        char Num[4];
        char SpamNumber[41];
    } SPAM_FAX_LIST_t;

    SPAM_FAX_LIST_t SpamFaxList;

    FILE *fout = fopen(SPAMFAXLIST_JSON_PATH, "w");
	if(fout == NULL)
	{
		ASSERT(0);
	}

    TMPL_varlist *main_varlist, *varlist, *loopvarlist;
    TMPL_fmtlist *fmtlist;
	TMPL_loop * loop, *main_loop;
	
    error_type_t db_retval = FAIL;
    
    struct timeval CurrentTimeCheck;
    struct tm *ptm;
   // struct tm *tm_ptr;
   // time_t time;
    
    char *pStationID;
    char *pStationNum;
    
    int current_page = 1;
    int i = 0;
    int page_count = (NoOfSpamFaxList>SPAMLIST_NUM_PER_PAGE)? 2:1;

    UINT8 dateType = getCountryDateType();
    
    char CurrentTime[64];
	
    // Station ID
    pStationID = db_get_var("CONF_FAXUP", "StationID", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationID\n", __FUNCTION__);
    }

    // Station Num
    pStationNum = db_get_var("CONF_FAXUP", "StationNum", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationNum\n", __FUNCTION__);
    }

	// Current Time
	gettimeofday(&CurrentTimeCheck, NULL);

	ptm = localtime(&CurrentTimeCheck.tv_sec);

    if (ptm == NULL)
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{	
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", 1900, 1, 1, 0, 0);
		}
    }
    else
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
		}
    }

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
	main_loop = 0;
	i = 0;

	while(1)
	{		
		if(current_page == 1)
		{
			varlist = TMPL_add_var(0, "c1", "0", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);			
		}
		else
		{
			varlist = TMPL_add_var(0, "ifvarname", "ok" , 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);		
			varlist = TMPL_add_var(varlist, "c1", "0", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);	
		}
		varlist = add_tli_header(varlist);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		varlist = TMPL_add_var(varlist, "c2", "200", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);		
		varlist = TMPL_add_var(varlist, "c3", "300", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c4", "150", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
												
		// Station ID
		varlist = TMPL_add_var(varlist, "StationID", pStationID, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// Station Num		
		varlist = TMPL_add_var(varlist, "StationNum", pStationNum, 0);	
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// CurrentTime
		varlist = TMPL_add_var(varlist, "CurrentTime", CurrentTime, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		loop = 0;

		if(NoOfSpamFaxList == 0)
		{
			varlist = TMPL_add_var(varlist, "nodata", "ok", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);		
			break;
		}
		else
		{
			do
			{
				loopvarlist = TMPL_add_var(0, "c1", "0", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c4", "150", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);			
				
				// 1. No
				snprintf(SpamFaxList.Num, sizeof(SpamFaxList.Num), "%d", (i+1));

				// 2. Spam Num
				snprintf(SpamFaxList.SpamNumber, 40, "%s", gFaxSpamFaxList[i]);

				loopvarlist = TMPL_add_var(loopvarlist, "Number", SpamFaxList.Num, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "SpamNum", SpamFaxList.SpamNumber, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				
				i++;
			}while(!(i == NoOfSpamFaxList || i == SPAMLIST_NUM_PER_PAGE));
		}
		
		varlist = TMPL_add_loop(varlist, "subloop", loop);
		main_loop = TMPL_add_varlist(main_loop, varlist);
	
		if(page_count > current_page)
		{
			current_page++;
		}
		else
		{
			break;
		}
	}

	main_varlist = TMPL_add_loop(0, "mainloop", main_loop);

    TMPL_write(0, SPAMFAXLIST_TEMPLATE_STRING, fmtlist, main_varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(SPAMFAXLIST_JSON_PATH, SPAMFAXLIST_PCL_PATH);	
}

#define MAX_BROADCAST_LOW 40
extern BROADCAST_RESULT_REPORT_t BroadcastResultReport;
void broadcast_result_report(void)
{
    typedef struct {
        char Num[4];
        char CurrentTime[64];
        long TimeInSec;
        char Date[14];
        char Mode[4];
        char pRemoteNumber[31];
        int RecName;
        char ModulationName[8];
        int Duration;
        char OnReportDuration[6];
        int MinDuration;
        int SecDuration;
        int PageNum;
        char OnReportPageNum[9];
        int Result;
        char OnReportResult[20];
        char String[128];
    } BC_RESULT_REPORT_t;

    BC_RESULT_REPORT_t BCResultReport;

	//error_type_t db_retval = FAIL;
    SDH_ERR("========== Broadcast Result Report ==========\n");
    //SDH_ERR("DB set\n");

    FILE *fout = fopen(BROADCASTRESULTREPORT_JSON_PATH, "w");
    if(fout == NULL)
    {
		ASSERT(0);
	}
    TMPL_varlist *main_varlist, *varlist, *loopvarlist;
    TMPL_fmtlist *fmtlist;
	TMPL_loop * loop, *main_loop;
	
    error_type_t db_retval = FAIL;
    
    struct timeval CurrentTimeCheck;
    struct tm *ptm;
    struct tm *tm_ptr;
    time_t time;
    
    char *pStationID;
    char *pStationNum;
    
    int total_page = 1;
    int current_page = 1;
    int row_count = 0;
    int row_check_count = 0;
    int i = 0;
 
    UINT8 dateType = getCountryDateType();
    
    char CurrentTime[64];
	//char ReportString[128];

	row_count = BroadcastResultReport.Count;
	
	SDH_ERR(">> broadcast result report row_count = %d\n",row_count);
	
	if(row_count == 0)
	{
		total_page = 1;
	}
	else if(row_count % MAX_BROADCAST_LOW)
	{
		total_page = (row_count/MAX_BROADCAST_LOW) + 1;
	}
	else
	{
		total_page = (row_count/MAX_BROADCAST_LOW);
	}
	
    // Station ID
    pStationID = db_get_var("CONF_FAXUP", "StationID", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationID\n", __FUNCTION__);
    }

    // Station Num
    pStationNum = db_get_var("CONF_FAXUP", "StationNum", &db_retval);
    if (db_retval == FAIL)
    {
        SDH_ERR("\n(YHCHO) %s() - db_get_var() Error... StationNum\n", __FUNCTION__);
    }

	// Current Time
	gettimeofday(&CurrentTimeCheck, NULL);

	ptm = localtime(&CurrentTimeCheck.tv_sec);

    if (ptm == NULL)
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{	
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", 1, 1, 1900, 0, 0);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", 1900, 1, 1, 0, 0);
		}
    }
    else
    {
		if(dateType == FAX_COUNTRY_DATE_TYPE_MDY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else if(dateType == FAX_COUNTRY_DATE_TYPE_DMY)
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%02d-%02d-%04d   %02d:%02d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900, ptm->tm_hour, ptm->tm_min);
		}
		else //FAX_COUNTRY_DATE_TYPE_YMD
		{
			snprintf(CurrentTime, sizeof(CurrentTime), "%04d-%02d-%02d   %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
		}
    }

    fmtlist = TMPL_add_fmt(0, "entity", TMPL_encode_entity);
	main_loop = 0;
	i = 0;

	for(; current_page <= total_page; current_page++)
	{
		//memset(ReportString, 0x00, sizeof(ReportString));
		
		if(current_page == 1)
		{
			varlist = TMPL_add_var(0, "c1", "0", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);			
		}
		else
		{
			varlist = TMPL_add_var(0, "ifvarname", "ok" , 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);		
			varlist = TMPL_add_var(varlist, "c1", "0", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);	
		}
		varlist = add_tli_header(varlist);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		varlist = TMPL_add_var(varlist, "c2", "200", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);		
		varlist = TMPL_add_var(varlist, "c3", "300", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		varlist = TMPL_add_var(varlist, "c4", "150", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c5", "1285", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c6", "1575", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c7", "1795", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
		varlist = TMPL_add_var(varlist, "c8", "2030", 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);	
												
		// Station ID
		varlist = TMPL_add_var(varlist, "StationID", pStationID, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// Station Num		
		varlist = TMPL_add_var(varlist, "StationNum", pStationNum, 0);	
		main_loop = TMPL_add_varlist(main_loop, varlist);
		
		// CurrentTime
		varlist = TMPL_add_var(varlist, "CurrentTime", CurrentTime, 0);
		main_loop = TMPL_add_varlist(main_loop, varlist);

		if(row_count == 0)
		{
			varlist = TMPL_add_var(varlist, "nodata", "ok", 0);
			main_loop = TMPL_add_varlist(main_loop, varlist);			
		}
		else
		{
			loop = 0;
			
			if(current_page != total_page)
			{
				row_check_count = current_page*MAX_BROADCAST_LOW;
			}
			else
			{
				row_check_count = row_count;
			}
			SDH_ERR("\n(YHCHO) %s() - row_check_count = %d\n", __FUNCTION__,row_check_count);

			for(; i < row_check_count; i++)
			{
				loopvarlist = TMPL_add_var(0, "c1", "0", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c4", "150", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c5", "1285", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c6", "1575", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c7", "1795", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				loopvarlist = TMPL_add_var(loopvarlist, "c8", "2030", 0);
				loop = TMPL_add_varlist(loop, loopvarlist);	
				
				snprintf(BCResultReport.Num, sizeof(BCResultReport.Num), "%d", (i+1));		

				// Remote Num
				//memcpy(TxRxResultReport.pRemoteNumber, fax_result[i].remotenum, sizeof(fax_result[i].remotenum));
				if(strlen(BroadcastResultReport.RemoteNo[i]) > 30)
				{
					memset(BCResultReport.pRemoteNumber,0x00,sizeof(BCResultReport.pRemoteNumber));
					memcpy(BCResultReport.pRemoteNumber, BroadcastResultReport.RemoteNo[i], 30);
				}
				else
				{
					memset(BCResultReport.pRemoteNumber,0x00,sizeof(BCResultReport.pRemoteNumber));
					memcpy(BCResultReport.pRemoteNumber, BroadcastResultReport.RemoteNo[i], strlen(BroadcastResultReport.RemoteNo[i]));
				}

				// 1. Date, and 2. Time
				time = (time_t)BroadcastResultReport.StartTime[i];
				tm_ptr = localtime(&time);
				if(tm_ptr == NULL)
				{
					snprintf(BCResultReport.Date, sizeof(BCResultReport.Date), "%02d-%02d %02d:%02d", 1, 1, 0, 0);				
				}
				else
				{
					snprintf(BCResultReport.Date, sizeof(BCResultReport.Date), "%02d-%02d %02d:%02d", 
										tm_ptr->tm_mon + 1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min);
				}

				BCResultReport.MinDuration = (BroadcastResultReport.Duration[i]/60);
				BCResultReport.SecDuration = (BroadcastResultReport.Duration[i]%60);
				snprintf(BCResultReport.OnReportDuration, sizeof(BCResultReport.OnReportDuration), "%02d:%02d", 
									BCResultReport.MinDuration, BCResultReport.SecDuration);

				snprintf(BCResultReport.OnReportPageNum, sizeof(BCResultReport.OnReportPageNum), "%03d/%03d", 
									BroadcastResultReport.SentPageNo[i], BroadcastResultReport.TotalPageNo[i]);

				switch(BroadcastResultReport.Result[i])
				{
					case 0:
						snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "OK");
						break;

					case 2:
						snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "No Answer");
						break;

					case 3:
						snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "Line Busy");
						break;

					case 4:
						snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "Line Error");
						break;

                    case 5:
                        snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "Phone Abort");
                        break;

					case 11:
						snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "Memory Full");
						break;

					case 14:
						snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "Cancelled");
						break;

                    case 68:
                        snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "Document Jam");
                        break;

                    case 71:
                        snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "Spam Fax");
                        break;

					default:
						snprintf(BCResultReport.OnReportResult, sizeof(BCResultReport.OnReportResult), "ERROR-#%03d", BroadcastResultReport.Result[i]);
						break;
				}

				loopvarlist = TMPL_add_var(loopvarlist, "Number", BCResultReport.Num, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "RemoteNum", BCResultReport.pRemoteNumber, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "StartTime", BCResultReport.Date, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "Duration", BCResultReport.OnReportDuration, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "Page", BCResultReport.OnReportPageNum, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);
				loopvarlist = TMPL_add_var(loopvarlist, "Result", BCResultReport.OnReportResult, 0);
				loop = TMPL_add_varlist(loop, loopvarlist);																				
			}
			
			varlist = TMPL_add_loop(varlist, "broadcastloop", loop);
			main_loop = TMPL_add_varlist(main_loop, varlist);
		}
	
	}

	main_varlist = TMPL_add_loop(0, "broadcasttotalloop", main_loop);

    TMPL_write(0, BROADCAST_RESULT_REPORT_TEMPLATE_STRING, fmtlist, main_varlist, fout, stderr);
    TMPL_free_varlist(varlist);
    TMPL_free_fmtlist(fmtlist);

    fclose(fout);

    SDH_ERR("Convert start\n");

    PclConvertJsonToPcl(BROADCASTRESULTREPORT_JSON_PATH, BROADCASTRESULTREPORT_PCL_PATH);
}
#endif

TMPL_varlist* add_db_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
    value = db_get_var_int(tblName, colName, &db_retval);
    snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
    varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_db_minus1_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
    value = db_get_var_int(tblName, colName, &db_retval);
    snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value - 1);
    varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_db_str_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName, int strSize) {
    error_type_t db_retval = FAIL;
	char* pBuffer = NULL;
	char buffer[BUFFER_SIZE_REPORT];
	memset(buffer, 0, sizeof(buffer));
    pBuffer = db_get_var(tblName, colName, &db_retval);
	if(pBuffer)
	{
		snprintf(buffer, sizeof(buffer), pBuffer);
		if(strlen(pBuffer) > strSize)
		{
			buffer[strSize-4] = '.';
			buffer[strSize-3] = '.';
			buffer[strSize-2] = '.';
			buffer[strSize-1] = 0;
		}
		MEM_FREE_AND_NULL(pBuffer);
	}
    varlist = TMPL_add_var(varlist, varName, buffer, 0);
	return varlist;
}

TMPL_varlist* add_paper_size_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case MEDIASIZE_A4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "A4");
		break;
	case MEDIASIZE_A5:
		snprintf(buffer, BUFFER_SIZE_REPORT, "A5 SEF");
		break;
	case MEDIASIZE_A5_ROTATED:
		snprintf(buffer, BUFFER_SIZE_REPORT, "A5 LEF");
		break;
	case MEDIASIZE_A6:
		snprintf(buffer, BUFFER_SIZE_REPORT, "A6 SEF");
		break;
	case MEDIASIZE_B5:
		snprintf(buffer, BUFFER_SIZE_REPORT, "B5 SEF");
		break;
	case MEDIASIZE_B6_JIS:
		snprintf(buffer, BUFFER_SIZE_REPORT, "B6 SEF");
		break;
	case MEDIASIZE_LETTER:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Letter");
		break;
	case MEDIASIZE_LEGAL:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Legal");
		break;
	case MEDIASIZE_EXECUTIVE:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Executive");
		break;
	case MEDIASIZE_STATEMENT:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Statement");
		break;
	case MEDIASIZE_CUSTOM_85x13:
		snprintf(buffer, BUFFER_SIZE_REPORT, "US Folio");
		break;
	case MEDIASIZE_CUSTOM_8x10:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Eng Quatro");
		break;
	case MEDIASIZE_CUSTOM_8x105:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Government Letter");
		break;
	case MEDIASIZE_CUSTOM_825x14:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Government Legal");
		break;
	case MEDIASIZE_JAPANESE_POSTCARD:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Card 100x148");
		break;
	case MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Card 148x200");
		break;
	case MEDIASIZE_DBL_JAPANESE_POSTCARD:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Card 200x148");
		break;
	case MEDIASIZE_ENV_MONARCH:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Monarch envelope");
		break;
	case MEDIASIZE_ENV_9:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Com9 envelope");
		break;
	case MEDIASIZE_ENV_10:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Com10 envelope");
		break;
	case MEDIASIZE_ENV_C5:
		snprintf(buffer, BUFFER_SIZE_REPORT, "C5 envelope");
		break;
	case MEDIASIZE_ENV_C6:
		snprintf(buffer, BUFFER_SIZE_REPORT, "C6 envelope");
		break;
	case MEDIASIZE_ENV_DL:
		snprintf(buffer, BUFFER_SIZE_REPORT, "DL envelope");
		break;
	case 554:   // AUTO value
		snprintf(buffer, BUFFER_SIZE_REPORT, "Auto detect");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, " ");
		break;
	}
    varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_paper_type_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case MEDIATYPE_PLAIN_PAPER:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Plain paper");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Paper label");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Envelope");
		break;
	case 4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Cardstock");
		break;
	case 5:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Transparency");
		break;
	case 6:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Vinyl label");
		break;
	case 7:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Postcard");
		break;
	case 8:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Recycled paper");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, " ");
		break;
	}
	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_tray_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case TRAY_1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "MPT");
		break;
	case TRAY_2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Tray 1");
		break;
	case TRAY_3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Tray 2");
		break;
	case TRAY_4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Tray 3");
		break;
	case TRAY_ANY:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Auto detect");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}
	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_tray_paper_size_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case TRAY_1:  // MPT : TrayConfig_mediaSize_0
		varlist = add_paper_size_value(varlist, varName, "CONF_PRNT", "TrayConfig_mediaSize_0");
	case TRAY_2:
		varlist = add_paper_size_value(varlist, varName, "CONF_PRNT", "TrayConfig_mediaSize_1");
		break;
	case TRAY_3:
		varlist = add_paper_size_value(varlist, varName, "CONF_PRNT", "TrayConfig_mediaSize_2");
		break;
	case TRAY_4:
		varlist = add_paper_size_value(varlist, varName, "CONF_PRNT", "TrayConfig_mediaSize_3");
		break;
		break;
	case TRAY_ANY:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Auto detect");
		varlist = TMPL_add_var(varlist, varName, buffer, 0);
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		varlist = TMPL_add_var(varlist, varName, buffer, 0);
		break;
	}
    return varlist;
}

TMPL_varlist* add_onoff_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Off");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "On");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_content_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Text");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Draft");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Text/Photo");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Photo");
		break;
	case 4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Picture");
		break;
	case 7:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Fax Text");
		break;
	case 8:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Fax Mixed");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_quality_2lv_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Normal");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "High quality");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_scale_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Automatic Resize");
		break;
	case 6:  // custom scale
		value = db_get_var_int(tblName, "ScaleCustom", &db_retval);
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d %%", value);
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d %%", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_nup_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Normal");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "2-up");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "4-up");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_color_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Color");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Gray");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "B/W Text");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "B/W Photo");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_fileformat_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName_format, char* colName_type) {
    error_type_t db_retval = FAIL;
    int value_format = 0;
	int value_type = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value_format = db_get_var_int(tblName, colName_format, &db_retval);
	value_type = db_get_var_int(tblName, colName_type, &db_retval);

	switch(value_format)
	{
	case 0:
		if(value_type == 0)
			snprintf(buffer, BUFFER_SIZE_REPORT, "TIFF files");
		else if(value_type == 1)
			snprintf(buffer, BUFFER_SIZE_REPORT, "TIFF 1 file");
		else
			snprintf(buffer, BUFFER_SIZE_REPORT, "%d-%d", value_format, value_type);
		break;
	case 1:
		if(value_type == 0)
			snprintf(buffer, BUFFER_SIZE_REPORT, "PDF files");
		else if(value_type == 1)
			snprintf(buffer, BUFFER_SIZE_REPORT, "PDF 1 file");
		else
			snprintf(buffer, BUFFER_SIZE_REPORT, "%d-%d", value_format, value_type);
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "JPEG");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value_format);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_quality_3lv_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Low");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Medium");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "High");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_default_dest_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Scan to USB");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Scan to E-mail");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Scan to FTP");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Scan to SMB");
		break;
	case 4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Scan to WebDAV");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_page_type_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Single-page");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Multi-page");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_resolution_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "75DPI * 75DPI");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "100DPI * 100DPI");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "150DPI * 150DPI");
		break;
	case 4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "200DPI * 200DPI");
		break;
	case 5:
		snprintf(buffer, BUFFER_SIZE_REPORT, "300DPI * 300DPI");
		break;
	case 6:
		snprintf(buffer, BUFFER_SIZE_REPORT, "600DPI * 600DPI");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_fax_line_monitor_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Off");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "On till connect");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "On");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_fax_image_type_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Text");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Photo");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_fax_resolution_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Standard");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Fine");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Superfine");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_copy_duplex_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "1-side > 1-side");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "1-side > 2-sides");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "2-sides > 1-side");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "2-sides > 2-sides");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_scan_duplex_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "1-side");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "2-sides");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_fax_print_mode_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 0:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Quick print");
		break;
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Print all");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_darkness_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Lighter+4");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Lighter+3");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Lighter+2");
		break;
	case 4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Lighter+1");
		break;
	case 5:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Normal");
		break;
	case 6:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Darker+1");
		break;
	case 7:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Darker+2");
		break;
	case 8:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Darker+3");
		break;
	case 9:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Darker+4");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_darkness_plus1_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Lighter+4");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Lighter+3");
		break;
	case 4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Lighter+2");
		break;
	case 5:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Lighter+1");
		break;
	case 6:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Normal");
		break;
	case 7:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Darker+1");
		break;
	case 8:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Darker+2");
		break;
	case 9:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Darker+3");
		break;
	case 10:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Darker+4");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_bgremove_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
	value = db_get_var_int(tblName, colName, &db_retval);

	switch(value)
	{
	case 1:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Weak+4");
		break;
	case 2:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Weak+3");
		break;
	case 3:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Weak+2");
		break;
	case 4:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Weak+1");
		break;
	case 5:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Normal");
		break;
	case 6:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Strong+1");
		break;
	case 7:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Strong+2");
		break;
	case 8:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Strong+3");
		break;
	case 9:
		snprintf(buffer, BUFFER_SIZE_REPORT, "Strong+4");
		break;
	default:
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d", value);
		break;
	}

	varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_db_version_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
    value = db_get_var_int(tblName, colName, &db_retval);
	if(value < 100)
		snprintf(buffer, BUFFER_SIZE_REPORT, "0.0.%d", value);
	else if(value < 10000)
		snprintf(buffer, BUFFER_SIZE_REPORT, "0.%d.%d", value/100, value%100);
	else
		snprintf(buffer, BUFFER_SIZE_REPORT, "%d.%d.%d", value/10000, value%10000/100, value%100);

    varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

TMPL_varlist* add_trans_count_value(TMPL_varlist* varlist, char* varName, char* tblName, char* colName) {
    error_type_t db_retval = FAIL;
	float f_value = 0;
    int value = 0;
    char buffer[BUFFER_SIZE_REPORT] = {0, };
    value = db_get_var_int(tblName, colName, &db_retval);
	f_value = (float)value / 1000;
	snprintf(buffer, BUFFER_SIZE_REPORT, "%0.6f", f_value);
    varlist = TMPL_add_var(varlist, varName, buffer, 0);
    return varlist;
}

#define BUFFER_SIZE_TLI 15
TMPL_varlist* add_tli_header(TMPL_varlist* varlist)
{
	char* pBuffer = NULL;
	char tli_prefix[BUFFER_SIZE_TLI];
    error_type_t db_retval = FAIL;
	memset(tli_prefix, 0, sizeof(tli_prefix));
	pBuffer = db_get_var("INST_MACHINE", "MachineTLI", &db_retval);
	if(pBuffer)
	{
		if(strncmp(pBuffer, "P247", 4) == 0 || strncmp(pBuffer, "M247", 4) == 0)
			varlist = TMPL_add_var(varlist, "parser_name", "Katusha parser", 0);
		else
			varlist = TMPL_add_var(varlist, "parser_name", "Sindoh parser", 0);
		
		snprintf(tli_prefix, 5, pBuffer);
		SDH_DEBUG("TLI_PREFIX : %s\n", tli_prefix);
		MEM_FREE_AND_NULL(pBuffer);
	}
	varlist = TMPL_add_var(varlist, "tli_prefix", tli_prefix, 0);
	return varlist;
}

TMPL_varlist* add_datetime(TMPL_varlist* varlist)
{
	char buffer[BUFFER_SIZE_REPORT];
	memset(buffer, 0, sizeof(buffer));
	struct timeval currentTime;
	struct tm *ptm;

    gettimeofday(&currentTime, NULL);
    ptm = localtime(&currentTime.tv_sec);

    if(ptm == NULL)
    {
        snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d   %02d:%02d", 1900, 1, 1, 0, 0);
	}
	else
	{
        snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d   %02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
	}
	SDH_ERR("DATETIME : %s\n", buffer);

	varlist = TMPL_add_var(varlist, "datetime", buffer, 0);
	return varlist;
}

TMPL_varlist* add_err_code_value(TMPL_varlist* varlist, char* varName, int err_code)
{
	switch(err_code)
	{
	case 100:
		varlist = TMPL_add_var(varlist, varName, "100 MPT Miss Feed Jam", 0);
		break;
	case 101:
		varlist = TMPL_add_var(varlist, varName, "101 Tray1 Miss Feed Jam", 0);
		break;
	case 102:
		varlist = TMPL_add_var(varlist, varName, "102 Tray2 Miss Feed Jam", 0);
		break;
	case 103:
		varlist = TMPL_add_var(varlist, varName, "103 Tray3 Miss Feed Jam", 0);
		break;
	case 104:
		varlist = TMPL_add_var(varlist, varName, "104 Paper Jam", 0);
		break;
	case 105:
		varlist = TMPL_add_var(varlist, varName, "105 Paper Jam", 0);
		break;
	case 106:
		varlist = TMPL_add_var(varlist, varName, "106 Paper Jam", 0);
		break;
	case 107:
		varlist = TMPL_add_var(varlist, varName, "107 Paper Jam", 0);
		break;
	case 120:
		varlist = TMPL_add_var(varlist, varName, "Re-Insert Cartridge", 0);
		break;
	case 121:
		varlist = TMPL_add_var(varlist, varName, "Toner Cartridge Change", 0);
		break;
	case 122:
		varlist = TMPL_add_var(varlist, varName, "Replace Cartridge Error", 0);
		break;
	case 201:
		varlist = TMPL_add_var(varlist, varName, "201 Scanner Jam", 0);
		break;
	case 202:
		varlist = TMPL_add_var(varlist, varName, "202 Scanner Jam", 0);
		break;
	case 211:
		varlist = TMPL_add_var(varlist, varName, "AFE offset adjust error", 0);
		break;
	case 212:
		varlist = TMPL_add_var(varlist, varName, "AFE gain adjust error", 0);
		break;
	case 213:
		varlist = TMPL_add_var(varlist, varName, "AFE register set error", 0);
		break;
	case 214:
		varlist = TMPL_add_var(varlist, varName, "CIS home position detect error", 0);
		break;
	case 215:
		varlist = TMPL_add_var(varlist, varName, "White strip read NG", 0);
		break;
	case 216:
		varlist = TMPL_add_var(varlist, varName, "Black strip read NG", 0);
		break;
	case 217:
		varlist = TMPL_add_var(varlist, varName, "Bad pixel error", 0);
		break;
	case 218:
		varlist = TMPL_add_var(varlist, varName, "Carriage failed leave home", 0);
		break;
	case 219:
		varlist = TMPL_add_var(varlist, varName, "Carriage failed arrive home", 0);
		break;
	case 220:
		varlist = TMPL_add_var(varlist, varName, "Scan pipe-line error", 0);
		break;
	case 300:
		varlist = TMPL_add_var(varlist, varName, "Fuser under temperature while printing", 0);
		break;
	case 301:
		varlist = TMPL_add_var(varlist, varName, "Fuser under temperature while at standby", 0);
		break;
	case 302:
		varlist = TMPL_add_var(varlist, varName, "Fuser failed to reach standby temperature", 0);
		break;
	case 303:
		varlist = TMPL_add_var(varlist, varName, "Fuser over temperature", 0);
		break;
	case 304:
		varlist = TMPL_add_var(varlist, varName, "Fuser thermistor failure", 0);
		break;
	case 305:
		varlist = TMPL_add_var(varlist, varName, "Fuser under temperature while heating", 0);
		break;
	case 310:
		varlist = TMPL_add_var(varlist, varName, "Printhead lost HSync", 0);
		break;
	case 311:
		varlist = TMPL_add_var(varlist, varName, "Mirror motor lock failure", 0);
		break;
	case 312:
		varlist = TMPL_add_var(varlist, varName, "Video done failure", 0);
		break;
	case 320:
		varlist = TMPL_add_var(varlist, varName, "Main motor lock failure", 0);
		break;
	case 330:
		varlist = TMPL_add_var(varlist, varName, "Fan motor lock failure", 0);
		break;
	case 331:
		varlist = TMPL_add_var(varlist, varName, "Sub Fan motor lock failure", 0);
		break;
	case 332:
		varlist = TMPL_add_var(varlist, varName, "Dev Fan motor lock failure", 0);
		break;
	case 340:
		varlist = TMPL_add_var(varlist, varName, "Option tray lost link error", 0);
		break;
	case 390:
		varlist = TMPL_add_var(varlist, varName, "Engine Software Error", 0);
		break;
	case 500:
		varlist = TMPL_add_var(varlist, varName, "Modem SPI reset failure", 0);
		break;
	case 501:
		varlist = TMPL_add_var(varlist, varName, "Modem SPI write failure", 0);
		break;
	case 502:
		varlist = TMPL_add_var(varlist, varName, "Modem SPI multi write failure", 0);
		break;
	case 503:
		varlist = TMPL_add_var(varlist, varName, "Modem SPI open failure(1)", 0);
		break;
	case 504:
		varlist = TMPL_add_var(varlist, varName, "Modem SPI open failure(2)", 0);
		break;
	case 505:
		varlist = TMPL_add_var(varlist, varName, "Modem SPI init failure", 0);
		break;
	case 506:
		varlist = TMPL_add_var(varlist, varName, "Fax total page setting error", 0);
		break;
	case 507:
		varlist = TMPL_add_var(varlist, varName, "Fax retry setting error", 0);
		break;
	case 508:
		varlist = TMPL_add_var(varlist, varName, "Invalid fax type", 0);
		break;
	case 509:
		varlist = TMPL_add_var(varlist, varName, "Fax print result error", 0);
		break;
	case 510:
		varlist = TMPL_add_var(varlist, varName, "Fax modem init error", 0);
		break;
	case 511:
		varlist = TMPL_add_var(varlist, varName, "DMB to Modem error", 0);
		break;
	case 512:
		varlist = TMPL_add_var(varlist, varName, "JBIG decode error(1)", 0);
		break;
	case 513:
		varlist = TMPL_add_var(varlist, varName, "JBIG decode error(2)", 0);
		break;
	case 514:
		varlist = TMPL_add_var(varlist, varName, "Transmit T4 image data error", 0);
		break;
	case 515:
		varlist = TMPL_add_var(varlist, varName, "CDB done failure", 0);
		break;
	case 516:
		varlist = TMPL_add_var(varlist, varName, "RX phase C EndOfPage failure", 0);
		break;
	case 517:
		varlist = TMPL_add_var(varlist, varName, "CDB write failure(1)", 0);
		break;
	case 518:
		varlist = TMPL_add_var(varlist, varName, "CDB write failure(2)", 0);
		break;
	case 519:
		varlist = TMPL_add_var(varlist, varName, "CDB read failure", 0);
		break;
	case 520:
		varlist = TMPL_add_var(varlist, varName, "Send image rescaling failure(1)", 0);
		break;
	case 521:
		varlist = TMPL_add_var(varlist, varName, "Send image rescaling failure(2)", 0);
		break;
	case 522:
		varlist = TMPL_add_var(varlist, varName, "Send image rescaling failure(3)", 0);
		break;
	case 523:
		varlist = TMPL_add_var(varlist, varName, "Send image rescaling failure(4)", 0);
		break;
	case 524:
		varlist = TMPL_add_var(varlist, varName, "Send image rescaling failure(5)", 0);
		break;
	case 525:
		varlist = TMPL_add_var(varlist, varName, "T4 decoding error", 0);
		break;
	case 526:
		varlist = TMPL_add_var(varlist, varName, "T4 encoding error", 0);
		break;
	case 527:
		varlist = TMPL_add_var(varlist, varName, "Invaild scan source", 0);
		break;
	case 528:
		varlist = TMPL_add_var(varlist, varName, "T4 encoding failure", 0);
		break;
	case 529:
		varlist = TMPL_add_var(varlist, varName, "Invalid TX coding type", 0);
		break;
	case 530:
		varlist = TMPL_add_var(varlist, varName, "RX image rescaling failure", 0);
		break;
	case 531:
		varlist = TMPL_add_var(varlist, varName, "Invalid RX total page", 0);
		break;
	case 532:
		varlist = TMPL_add_var(varlist, varName, "Create shared memory failure", 0);
		break;
	case 533:
		varlist = TMPL_add_var(varlist, varName, "Shared memory error", 0);
		break;
	case 534:
		varlist = TMPL_add_var(varlist, varName, "Error : Transfer image data from the HDLC buffer", 0);
		break;
	case 535:
		varlist = TMPL_add_var(varlist, varName, "Error : Transmits one FCD frame of image data", 0);
		break;
	case 536:
		varlist = TMPL_add_var(varlist, varName, "DB Read Error", 0);
		break;
	case 537:
		varlist = TMPL_add_var(varlist, varName, "DB Write Error", 0);
		break;
	case 538:
		varlist = TMPL_add_var(varlist, varName, "DB Access Error", 0);
		break;
	case 539:
		varlist = TMPL_add_var(varlist, varName, "Invalid Dial No.", 0);
		break;
	case 910:
		varlist = TMPL_add_var(varlist, varName, "Software Assert", 0);
		break;
	case 912:
		varlist = TMPL_add_var(varlist, varName, "Software Segmentation Fault", 0);
		break;
	case 920:
		varlist = TMPL_add_var(varlist, varName, "DB recovery success", 0);
		break;
	case 923:
		varlist = TMPL_add_var(varlist, varName, "DB recovery failure - No backup DB", 0);
		break;
	case 924:
		varlist = TMPL_add_var(varlist, varName, "DB recovery failure - backup DB integrity check fail", 0);
		break;
	case 925:
		varlist = TMPL_add_var(varlist, varName, "DB recovery failure - backup DB copy fail", 0);
		break;
	case 930:
		varlist = TMPL_add_var(varlist, varName, "Invalid RTC Time", 0);
		break;
	case 931:
		varlist = TMPL_add_var(varlist, varName, "ODMA wait failure", 0);
		break;
	default:
		varlist = TMPL_add_var(varlist, varName, "Unknown Error", 0);
		break;
	}

	return varlist;
}

