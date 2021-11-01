/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtGeneralTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

// debug headers
#define DBG_PRFX "snmp: "
//#define DBG_ERR
//#define DBG_VERBOSE_ON
//#define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "prtGeneralTable.h"

#include "db_api.h"
#include "memAPI.h"

static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid prtGeneralTable_oid[] = { prtGeneralTable_TABLE_OID };
size_t prtGeneralTable_oid_len = OID_LENGTH(prtGeneralTable_oid);

static int prtGeneralTable_row_copy(
        prtGeneralTable_context *, prtGeneralTable_context *);

static int prtGeneralTable_module_initialized = 0;


#if 0
/************************************************************
 * increment a numerical data in a row
 */
int inc_prtGeneralTable_data(long devNdx, unsigned long colFlags)
{
    int incAll = colFlags ? 0 : 1;
    prtGeneralTable_context *row;
    netsnmp_index hdr;
    oid ndx;
    int col;

    /** make sure the module is initialized first */
    if (!prtGeneralTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndx = devNdx;
    hdr.len = 1;
    hdr.oids = &ndx;

    /** find desired row */
    row = (prtGeneralTable_context *) prtGeneralTable_get_by_idx(&hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d] not found\n", __func__, (int) ndx);
        return -1;
    }

    /** increment desired data */
    for (col = prtGeneralTable_COL_MIN; col <= prtGeneralTable_COL_MAX; col++) {
        if (!incAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTGENERALCONFIGCHANGES:
            if (row->prtGeneralConfigChanges != 0xFFFFFFFF)
                row->prtGeneralConfigChanges++;
            if (!incAll)
                break;
        case COLUMN_PRTALERTCRITICALEVENTS:
            if (row->prtAlertCriticalEvents != 0xFFFFFFFF)
                row->prtAlertCriticalEvents++;
            if (!incAll)
                break;
        case COLUMN_PRTALERTALLEVENTS:
            if (row->prtAlertAllEvents != 0xFFFFFFFF)
                row->prtAlertAllEvents++;
            if (!incAll)
                break;

        default:
            /** all data set */
            return 0;
        }
    }

    return 0;
}

/************************************************************
 * get data from a row
 */
int get_prtGeneralTable_data(
                        prtGeneralTable_context *out, unsigned long colFlags)
{
    int getAll = colFlags ? 0 : 1;
    prtGeneralTable_context *row;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized first */
    if (!prtGeneralTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!out || !out->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }

    /** find desired row */
    hdr = &out->index;
    row = (prtGeneralTable_context *) prtGeneralTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d] not found\n", 
                                                __func__, (int) hdr->oids[0]);
        return -1;
    }

    /** get data */
    for (col = 1; col <= prtGeneralTable_COL_MAX; col++) {
        if (!getAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTGENERALCONFIGCHANGES:
            out->prtGeneralConfigChanges = row->prtGeneralConfigChanges;
            if (!getAll)
                break;
        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
            out->prtGeneralCurrentLocalization =
                                        row->prtGeneralCurrentLocalization;
            if (!getAll)
                break;
        case COLUMN_PRTGENERALRESET:
            out->prtGeneralReset = row->prtGeneralReset;
            if (!getAll)
                break;
        case COLUMN_PRTGENERALCURRENTOPERATOR:
            memcpy((void *) out->prtGeneralCurrentOperator,
                    (const void *) row->prtGeneralCurrentOperator,
                                        row->prtGeneralCurrentOperator_len);
            out->prtGeneralCurrentOperator_len =
                                        row->prtGeneralCurrentOperator_len;
            if (!getAll)
                break;
        case COLUMN_PRTGENERALSERVICEPERSON:
            memcpy((void *) out->prtGeneralServicePerson,
                    (const void *) row->prtGeneralServicePerson,
                                        row->prtGeneralServicePerson_len);
            out->prtGeneralServicePerson_len =
                                        row->prtGeneralServicePerson_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTDEFAULTINDEX:
            out->prtInputDefaultIndex = row->prtInputDefaultIndex;
            if (!getAll)
                break;
        case COLUMN_PRTOUTPUTDEFAULTINDEX:
            out->prtOutputDefaultIndex = row->prtOutputDefaultIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERDEFAULTINDEX:
            out->prtMarkerDefaultIndex = row->prtMarkerDefaultIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
            out->prtMediaPathDefaultIndex = row->prtMediaPathDefaultIndex;
            if (!getAll)
                break;
        case COLUMN_PRTCONSOLELOCALIZATION:
            out->prtConsoleLocalization = row->prtConsoleLocalization;
            if (!getAll)
                break;
        case COLUMN_PRTCONSOLENUMBEROFDISPLAYLINES:
            out->prtConsoleNumberOfDisplayLines = 
                                        row->prtConsoleNumberOfDisplayLines;
            if (!getAll)
                break;
        case COLUMN_PRTCONSOLENUMBEROFDISPLAYCHARS:
            out->prtConsoleNumberOfDisplayChars = 
                                        row->prtConsoleNumberOfDisplayChars;
            if (!getAll)
                break;
        case COLUMN_PRTCONSOLEDISABLE:
            out->prtConsoleDisable = row->prtConsoleDisable;
            if (!getAll)
                break;
        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
            out->prtAuxiliarySheetStartupPage = 
                                        row->prtAuxiliarySheetStartupPage;
            if (!getAll)
                break;
        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            out->prtAuxiliarySheetBannerPage = 
                                        row->prtAuxiliarySheetBannerPage;
            if (!getAll)
                break;
        case COLUMN_PRTGENERALPRINTERNAME:
            memcpy((void *) row->prtGeneralPrinterName,
                    (const void *) row->prtGeneralPrinterName,
                                            row->prtGeneralPrinterName_len);
            out->prtGeneralPrinterName_len = row->prtGeneralPrinterName_len;
            if (!getAll)
                break;
        case COLUMN_PRTGENERALSERIALNUMBER:
            memcpy((void *) row->prtGeneralSerialNumber,
                    (const void *) row->prtGeneralSerialNumber,
                                            row->prtGeneralSerialNumber_len);
            out->prtGeneralSerialNumber_len = row->prtGeneralSerialNumber_len;
            if (!getAll)
                break;
        case COLUMN_PRTALERTCRITICALEVENTS:
            out->prtAlertCriticalEvents = row->prtAlertCriticalEvents;
            if (!getAll)
                break;
        case COLUMN_PRTALERTALLEVENTS:
            out->prtAlertAllEvents = row->prtAlertAllEvents;
            if (!getAll)
                break;
        default:
            return 0;
        }
    }
    return 0;
}

/************************************************************
 * set data in a row
 */
int set_prtGeneralTable_data(
                        prtGeneralTable_context *data, unsigned long colFlags)
{
    int setAll = colFlags ? 0 : 1;
    prtGeneralTable_context *row;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized first */
    if (!prtGeneralTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** validate input structure */
    if (!data || !data->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete input info\n", __func__);
        return -1;
    }
    if (chk_prtGeneralTable_data(data, colFlags))
        return -1;

    /** find desired row */
    hdr = &data->index;
    row = (prtGeneralTable_context *) prtGeneralTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d] not found\n", 
                                                __func__, (int) hdr->oids[0]);
        return -1;
    }

    /** set data */
    for (col = 1; col <= prtGeneralTable_COL_MAX; col++) {
        if (!setAll && !(colFlags & (1 << col)))
            continue;

        switch (col) {

        case COLUMN_PRTGENERALCONFIGCHANGES:
            row->prtGeneralConfigChanges = data->prtGeneralConfigChanges;
            if (!setAll)
                break;
        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
            row->prtGeneralCurrentLocalization = 
                                        data->prtGeneralCurrentLocalization;
            if (!setAll)
                break;
        case COLUMN_PRTGENERALRESET:
            row->prtGeneralReset = data->prtGeneralReset;
            if (!setAll)
                break;
        case COLUMN_PRTGENERALCURRENTOPERATOR:
            memcpy((void *) row->prtGeneralCurrentOperator, 
                    (const void *) data->prtGeneralCurrentOperator,
                                        data->prtGeneralCurrentOperator_len);
            row->prtGeneralCurrentOperator_len =
                                        data->prtGeneralCurrentOperator_len;
            if (!setAll)
                break;
        case COLUMN_PRTGENERALSERVICEPERSON:
            memcpy((void *) row->prtGeneralServicePerson,
                    (const void *) data->prtGeneralServicePerson,
                                        data->prtGeneralServicePerson_len);
            row->prtGeneralServicePerson_len =
                                        data->prtGeneralServicePerson_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTDEFAULTINDEX:
            row->prtInputDefaultIndex = data->prtInputDefaultIndex;
            if (!setAll)
                break;
        case COLUMN_PRTOUTPUTDEFAULTINDEX:
            row->prtOutputDefaultIndex = data->prtOutputDefaultIndex;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERDEFAULTINDEX:
            row->prtMarkerDefaultIndex = data->prtMarkerDefaultIndex;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
            row->prtMediaPathDefaultIndex = data->prtMediaPathDefaultIndex;
            if (!setAll)
                break;
        case COLUMN_PRTCONSOLELOCALIZATION:
            row->prtConsoleLocalization = data->prtConsoleLocalization;
            if (!setAll)
                break;
        case COLUMN_PRTCONSOLENUMBEROFDISPLAYLINES:
            row->prtConsoleNumberOfDisplayLines = 
                                        data->prtConsoleNumberOfDisplayLines;
            if (!setAll)
                break;
        case COLUMN_PRTCONSOLENUMBEROFDISPLAYCHARS:
            row->prtConsoleNumberOfDisplayChars =
                                        data->prtConsoleNumberOfDisplayChars;
            if (!setAll)
                break;
        case COLUMN_PRTCONSOLEDISABLE:
            row->prtConsoleDisable = data->prtConsoleDisable;
            if (!setAll)
                break;
        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
            row->prtAuxiliarySheetStartupPage = 
                                        data->prtAuxiliarySheetStartupPage;
            if (!setAll)
                break;
        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            row->prtAuxiliarySheetBannerPage =
                                        data->prtAuxiliarySheetBannerPage;
            if (!setAll)
                break;
        case COLUMN_PRTGENERALPRINTERNAME:
            memcpy((void *) row->prtGeneralPrinterName,
                    (const void *) data->prtGeneralPrinterName,
                                            data->prtGeneralPrinterName_len);
            row->prtGeneralPrinterName_len =
                                            data->prtGeneralPrinterName_len;
            if (!setAll)
                break;
        case COLUMN_PRTGENERALSERIALNUMBER:
            memcpy((void *) row->prtGeneralSerialNumber,
                    (const void *) data->prtGeneralSerialNumber,
                                            data->prtGeneralSerialNumber_len);
            row->prtGeneralSerialNumber_len =
                                            data->prtGeneralSerialNumber_len;
            if (!setAll)
                break;
        case COLUMN_PRTALERTCRITICALEVENTS:
            row->prtAlertCriticalEvents = data->prtAlertCriticalEvents;
            if (!setAll)
                break;
        case COLUMN_PRTALERTALLEVENTS:
            row->prtAlertAllEvents = data->prtAlertAllEvents;
            if (!setAll)
                break;
        default:
            return 0;
        }
    }

    return 0;
}

/************************************************************
 * delete a row from a table
 */
int del_prtGeneralTable_entry(long devNdx)
{
    oid ndx;
    netsnmp_index hdr;
    prtGeneralTable_context *row;

    /** make sure the module is initialized first */
    if (!prtGeneralTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndx = devNdx;
    hdr.len = 1;
    hdr.oids = &ndx;

    /** find desired row */
    row = (prtGeneralTable_context *) prtGeneralTable_get_by_idx(&hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d] not found\n", __func__, (int) ndx);
        return -1;
    }

    /** remove row from table and free allocated memory */
    if (CONTAINER_REMOVE(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: error removing row[%d]\n", __func__, (int) ndx);
        return -1;
    }
    if (row->index.oids)
        free(row->index.oids);
    free(row);

    return 0;
}

/************************************************************
 * add a row to the table
 */
int add_prtGeneralTable_entry(prtGeneralTable_context *info)
{
    netsnmp_index *hdr;
    prtGeneralTable_context *row;

    /** make sure the module is initialized first */
    if (!prtGeneralTable_module_initialized)
        init_prtGeneralTable();

    /** validate input structure */
    if (!info || !info->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete input info\n", __func__);
        return -1;
    }
    if (!IS_1TO7FFFFFFF(info->index.oids[0])) {
        snmp_log(LOG_ERR, "%s: invalid device index\n", __func__);
        return -1;
    }

    /** make sure desired index tuple are non already in use */
    hdr = &(info->index);
    row = (prtGeneralTable_context *) prtGeneralTable_get_by_idx(hdr);
    if (row) {
        snmp_log(LOG_ERR, "%s: index[%d] already in use\n",
                                                __func__, (int) hdr->oids[0]);
        return -1;
    }

    /** create row context and insert into container */
    row = prtGeneralTable_create_row(hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: adding the row failed\n", __func__);
        return -1;
    }

    /** copy input row column data */
    if (prtGeneralTable_row_copy(row, info)) {
        snmp_log(LOG_ERR, "%s: copying row contents failed\n", __func__);
        return -1;
    }

    return 0;
}

/************************************************************
 * validate one or more column data
 */
int chk_prtGeneralTable_data(
                        prtGeneralTable_context *data, unsigned long colFlags)
{
    int checkAll = colFlags ? 0 : 1;
    int col;

    for (col = 1; col <= prtGeneralTable_COL_MAX; col++) {
        
        if (!checkAll && !(colFlags & (1 << col)))
            continue;

        switch (col) {

        case COLUMN_PRTGENERALCONFIGCHANGES:
            if (!IS_0TOFFFFFFFF(data->prtGeneralConfigChanges)) {
                snmp_log(LOG_ERR, "invalid config changes count\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
            if (!IS_1TOFFFF(data->prtGeneralCurrentLocalization)) {
                snmp_log(LOG_ERR, "invalid current localization code\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTGENERALRESET:
            if (!IS_PRTGENERALRESET_VALID(data->prtGeneralReset)) {
                snmp_log(LOG_ERR, "invalid general reset code [%d]\n", 
                                                (int) data->prtGeneralReset);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTGENERALCURRENTOPERATOR:
            if (data->prtGeneralCurrentOperator_len && 
                                data->prtGeneralCurrentOperator_len > 127) {
                snmp_log(LOG_ERR, "invalid operator string length\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTGENERALSERVICEPERSON:
            if (data->prtGeneralServicePerson_len && 
                                data->prtGeneralServicePerson_len > 127) {
                snmp_log(LOG_ERR, "invalid service person string length\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTDEFAULTINDEX:
            if (!IS_1TOFFFF(data->prtInputDefaultIndex)) {
                snmp_log(LOG_ERR, "invalid input default index\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTOUTPUTDEFAULTINDEX:
            if (!IS_1TOFFFF(data->prtOutputDefaultIndex)) {
                snmp_log(LOG_ERR, "invalid output default index\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERDEFAULTINDEX:
            if (!IS_1TOFFFF(data->prtMarkerDefaultIndex)) {
                snmp_log(LOG_ERR, "invalid marker default index\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
            if (!IS_1TOFFFF(data->prtMediaPathDefaultIndex)) {
                snmp_log(LOG_ERR, "invalid media path default index\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTCONSOLELOCALIZATION:
            if (!IS_1TOFFFF(data->prtConsoleLocalization)) {
                snmp_log(LOG_ERR, "invalid console localization code\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTCONSOLENUMBEROFDISPLAYLINES:
            if (!IS_1TOFFFF(data->prtConsoleNumberOfDisplayLines)) {
                snmp_log(LOG_ERR, "invalid number of display lines\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTCONSOLENUMBEROFDISPLAYCHARS:
            if (!IS_1TOFFFF(data->prtConsoleNumberOfDisplayChars)) {
                snmp_log(LOG_ERR, "invalid number of display characters\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTCONSOLEDISABLE:
            if (!IS_PRTCONSOLEDISABLE_VALID(data->prtConsoleDisable)) {
                snmp_log(LOG_ERR, "invalid console disable code [%d]\n", 
                                                (int) data->prtConsoleDisable);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
            if (!IS_PRESENTONOFF_VALID( data->prtAuxiliarySheetStartupPage)) {
                snmp_log(LOG_ERR, "invalid aux startup page status [%d]\n", 
                                    (int) data->prtAuxiliarySheetStartupPage);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            if (!IS_PRESENTONOFF_VALID(data->prtAuxiliarySheetBannerPage)) {
                snmp_log(LOG_ERR, "invalid aux banner page status [%d]\n", 
                                    (int) data->prtAuxiliarySheetBannerPage);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTGENERALPRINTERNAME:
            if (data->prtGeneralPrinterName_len && 
                                    data->prtGeneralPrinterName_len > 127) {
                snmp_log(LOG_ERR, "invalid printer name length\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTGENERALSERIALNUMBER:
            if (data->prtGeneralSerialNumber_len && 
                                    data->prtGeneralSerialNumber_len > 255) {
                snmp_log(LOG_ERR, "invalid serial number length\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTALERTCRITICALEVENTS:
            if (!IS_0TOFFFFFFFF(data->prtAlertCriticalEvents)) {
                snmp_log(LOG_ERR, "invalid critical events count\n");
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTALERTALLEVENTS:
            if (!IS_0TOFFFFFFFF(data->prtAlertAllEvents)) {
                snmp_log(LOG_ERR, "invalid all events count\n");
                return -1;
            }
            if (!checkAll)
                break;
        default:
            return 0;
        }
    }
    return 0;
}
#endif

/************************************************************
 * populate table with startup rows
 */
void populate_prtGeneralTable(PRTMIB_OID_MAP *map)
{
    netsnmp_index hdr;
    prtGeneralTable_context *row;
    oid hrDeviceIndex = PRTMIB_THIS_HOST_DEVICE_INDEX;

    if (NULL == map)
        return;

    if (!prtGeneralTable_module_initialized)
        initialize_table_prtGeneralTable();

    hdr.len = 1;
    hdr.oids = &hrDeviceIndex;
    row = prtGeneralTable_create_row(&hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: row addition failed\n", __func__);
        return;
    }
    row->columnInfo = map;
}

/************************************************************
 * initialize the module
 */
void init_prtGeneralTable(void)
{
    initialize_table_prtGeneralTable();
    prtGeneralTable_module_initialized = 1;
}

/************************************************************
 * copy the contents of a row into another
 */
static int prtGeneralTable_row_copy(
                prtGeneralTable_context *dst, prtGeneralTable_context *src)
{
    if (!dst||!src)
        return 1;
        
    /** copy index, if provided */
    if (dst->index.oids)
        free(dst->index.oids);
    if (snmp_clone_mem((void *) &dst->index.oids, src->index.oids,
                                       src->index.len * sizeof(oid))) {
        dst->index.oids = NULL;
        return 1;
    }
    dst->index.len = src->index.len;
    
    /** copy components into the context structure */
#if 0
    dst->prtGeneralConfigChanges = src->prtGeneralConfigChanges;

    dst->prtGeneralCurrentLocalization = src->prtGeneralCurrentLocalization;

    dst->prtGeneralReset = src->prtGeneralReset;

    memcpy(dst->prtGeneralCurrentOperator, src->prtGeneralCurrentOperator,
                                            src->prtGeneralCurrentOperator_len);
    dst->prtGeneralCurrentOperator_len = src->prtGeneralCurrentOperator_len;

    memcpy( dst->prtGeneralServicePerson, src->prtGeneralServicePerson,
                                            src->prtGeneralServicePerson_len);
    dst->prtGeneralServicePerson_len = src->prtGeneralServicePerson_len;

    dst->prtInputDefaultIndex = src->prtInputDefaultIndex;

    dst->prtOutputDefaultIndex = src->prtOutputDefaultIndex;

    dst->prtMarkerDefaultIndex = src->prtMarkerDefaultIndex;

    dst->prtMediaPathDefaultIndex = src->prtMediaPathDefaultIndex;

    dst->prtConsoleLocalization = src->prtConsoleLocalization;

    dst->prtConsoleNumberOfDisplayLines = src->prtConsoleNumberOfDisplayLines;

    dst->prtConsoleNumberOfDisplayChars = src->prtConsoleNumberOfDisplayChars;

    dst->prtConsoleDisable = src->prtConsoleDisable;

    dst->prtAuxiliarySheetStartupPage = src->prtAuxiliarySheetStartupPage;

    dst->prtAuxiliarySheetBannerPage = src->prtAuxiliarySheetBannerPage;

    memcpy(dst->prtGeneralPrinterName, src->prtGeneralPrinterName, 
                                        src->prtGeneralPrinterName_len );
    dst->prtGeneralPrinterName_len = src->prtGeneralPrinterName_len;

    memcpy(dst->prtGeneralSerialNumber, src->prtGeneralSerialNumber, 
                                        src->prtGeneralSerialNumber_len );
    dst->prtGeneralSerialNumber_len = src->prtGeneralSerialNumber_len;

    dst->prtAlertCriticalEvents = src->prtAlertCriticalEvents;

    dst->prtAlertAllEvents = src->prtAlertAllEvents;
#else
    dst->columnInfo = src->columnInfo;
#endif
    return 0;
}

/************************************************************
 * the *_can_delete routine is called to determine if a row
 * can be deleted.
 *
 * return 1 if the row can be deleted
 * return 0 if the row cannot be deleted
 */
int prtGeneralTable_can_delete(prtGeneralTable_context *undo_ctx,
                            prtGeneralTable_context *row_ctx,
                            netsnmp_request_group *rg)
{
    return 0;
}


/************************************************************
 * duplicate a row
 */
prtGeneralTable_context *prtGeneralTable_duplicate_row(
                                            prtGeneralTable_context *row_ctx)
{
    prtGeneralTable_context *dup;

    if (!row_ctx)
        return NULL;

    dup = SNMP_MALLOC_TYPEDEF(prtGeneralTable_context);
    if (!dup)
        return NULL;
        
    if (prtGeneralTable_row_copy(dup,row_ctx)) {
        free(dup);
        dup = NULL;
    }

    return dup;
}

/************************************************************
 * delete a row
 */
netsnmp_index *prtGeneralTable_delete_row(prtGeneralTable_context *ctx)
{
    if (ctx->index.oids)
        free(ctx->index.oids);
    free(ctx);

    return NULL;
}

/************************************************************
 * RESERVE is used to check the syntax of all the variables
 * provided, that the values being set are sensible and consistent,
 * and to allocate any resources required for performing the SET.
 * After this stage, the expectation is that the set ought to
 * succeed, though this is not guaranteed. (In fact, with the UCD
 * agent, this is done in two passes - RESERVE1, and
 * RESERVE2, to allow for dependancies between variables).
 *
 * BEFORE calling this routine, the agent will call duplicate_row
 * to create a copy of the row (unless this is a new row; i.e.
 * row_created == 1).
 *
 * next state -> SET_RESERVE2 || SET_FREE
 */
void prtGeneralTable_set_reserve1(netsnmp_request_group *rg)
{
    prtGeneralTable_context *row_ctx =
            (prtGeneralTable_context *) rg->existing_row;
    /*
    prtGeneralTable_context *undo_ctx =
            (prtGeneralTable_context *) rg->undo_info;
    */
    netsnmp_variable_list *var;
    netsnmp_request_group_item *current;
    int rc;

    for (current = rg->list; current; current = current->next) {

        var = current->ri->requestvb;
        rc = SNMP_ERR_NOERROR;

        if (!IS_COLUMN_IMPLEMENTED(row_ctx->columnInfo, current->tri->colnum))
            continue; /** no checking needed for unimplemented columns */

        switch (current->tri->colnum) {

		/* added checking for read-only */
		case COLUMN_PRTMARKERDEFAULTINDEX:	
			DBG_VERBOSE("Read Only\n");
			rc = SNMP_ERR_READONLY; 	
			break;
        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
        case COLUMN_PRTINPUTDEFAULTINDEX:
        case COLUMN_PRTOUTPUTDEFAULTINDEX:
        /*case COLUMN_PRTMARKERDEFAULTINDEX:*/
        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
        case COLUMN_PRTCONSOLELOCALIZATION:
            rc = netsnmp_check_vb_int_range(var, 1, 0xFFFF);
            break;

        case COLUMN_PRTGENERALRESET:
            if (!IS_PRTGENERALRESET_VALID(*var->val.integer))
                rc = SNMP_ERR_WRONGVALUE;
            break;

        case COLUMN_PRTGENERALCURRENTOPERATOR:
        case COLUMN_PRTGENERALSERVICEPERSON:
        case COLUMN_PRTGENERALPRINTERNAME:
            rc = netsnmp_check_vb_type_and_max_size(var, ASN_OCTET_STR, 127);
            break;

        case COLUMN_PRTCONSOLEDISABLE:
            if (!IS_PRTCONSOLEDISABLE_VALID(*var->val.integer))
                rc = SNMP_ERR_WRONGVALUE;
            break;

        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            if (!IS_PRESENTONOFF_VALID(*var->val.integer))
                rc = SNMP_ERR_WRONGVALUE;
            break;

        case COLUMN_PRTGENERALSERIALNUMBER:
            rc = netsnmp_check_vb_type_and_max_size(var, ASN_OCTET_STR, 255);
            break;

        default: /** We shouldn't get here */
            rc = SNMP_ERR_GENERR;
            snmp_log(LOG_ERR, "unknown column in "
                     "prtGeneralTable_set_reserve1\n");
        }

        if (rc)
           netsnmp_set_mode_request_error(MODE_SET_BEGIN, current->ri, rc);
        rg->status = SNMP_MAX(rg->status, current->ri->status);
    }
}

void prtGeneralTable_set_reserve2(netsnmp_request_group *rg)
{
    prtGeneralTable_context *row_ctx =
        (prtGeneralTable_context *) rg->existing_row;
    /*
    prtGeneralTable_context *undo_ctx =
        (prtGeneralTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;
    netsnmp_variable_list *var;
    int rc;

    rg->rg_void = rg->list->ri;

    for (current = rg->list; current; current = current->next) {

        var = current->ri->requestvb;
        rc = SNMP_ERR_NOERROR;

        if (!IS_COLUMN_IMPLEMENTED(row_ctx->columnInfo, current->tri->colnum))
            continue; /** no checking needed for unimplemented columns */

        switch (current->tri->colnum) {

        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
        case COLUMN_PRTINPUTDEFAULTINDEX:
        case COLUMN_PRTOUTPUTDEFAULTINDEX:
        case COLUMN_PRTMARKERDEFAULTINDEX:
        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
        case COLUMN_PRTCONSOLELOCALIZATION:
            if(!IS_1TOFFFF(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTGENERALRESET:
            if (!IS_PRTGENERALRESET_VALID(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTGENERALCURRENTOPERATOR:
        case COLUMN_PRTGENERALSERVICEPERSON:
        case COLUMN_PRTGENERALPRINTERNAME:
            /*
             * if ( XXX_check_value( var->val.string, XXX ) ) {
             *    rc = SNMP_ERR_INCONSISTENTVALUE;
             *    rc = SNMP_ERR_BADVALUE;
             * }
             */
            if (!IS_0TO127(var->val_len))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTCONSOLEDISABLE:
            if (!IS_PRTCONSOLEDISABLE_VALID(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            if (!IS_PRESENTONOFF_VALID(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTGENERALSERIALNUMBER:
            if (!IS_0TO255(var->val_len))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        default: /** We shouldn't get here */
            netsnmp_assert(0); /** why wasn't this caught in reserve1? */
        }

        if (rc)
           netsnmp_set_mode_request_error(MODE_SET_BEGIN, current->ri, rc);
    }
}

/************************************************************
 * Assuming that the RESERVE phases were successful, the next
 * stage is indicated by the action value ACTION. This is used
 * to actually implement the set operation. However, this must
 * either be done into temporary (persistent) storage, or the
 * previous value stored similarly, in case any of the subsequent
 * ACTION calls fail.
 *
 * In your case, changes should be made to row_ctx. A copy of
 * the original row is in undo_ctx.
 */
void prtGeneralTable_set_action(netsnmp_request_group *rg)
{
    netsnmp_variable_list *var;
    prtGeneralTable_context *row_ctx = 
        (prtGeneralTable_context *) rg->existing_row;
    /*
    prtGeneralTable_context *undo_ctx = 
        (prtGeneralTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for (current = rg->list; current; current = current->next) {

        var = current->ri->requestvb;

        switch (current->tri->colnum) {

#if 0
        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtGeneralCurrentLocalization = *var->val.integer;
            break;

        case COLUMN_PRTGENERALRESET:
            /** PrtGeneralResetTC = ASN_INTEGER */
            row_ctx->prtGeneralReset = *var->val.integer;
            break;

        case COLUMN_PRTGENERALCURRENTOPERATOR:
            /** OCTETSTR = ASN_OCTET_STR */
            memcpy(row_ctx->prtGeneralCurrentOperator, var->val.string,
                                                        var->val_len);
            row_ctx->prtGeneralCurrentOperator_len = var->val_len;
            break;

        case COLUMN_PRTGENERALSERVICEPERSON:
            /** OCTETSTR = ASN_OCTET_STR */
            memcpy(row_ctx->prtGeneralServicePerson, var->val.string,
                                                        var->val_len);
            row_ctx->prtGeneralServicePerson_len = var->val_len;
            break;

        case COLUMN_PRTINPUTDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputDefaultIndex = *var->val.integer;
            break;

        case COLUMN_PRTOUTPUTDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtOutputDefaultIndex = *var->val.integer;
            break;

        case COLUMN_PRTMARKERDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtMarkerDefaultIndex = *var->val.integer;
            break;

        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtMediaPathDefaultIndex = *var->val.integer;
            break;

        case COLUMN_PRTCONSOLELOCALIZATION:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtConsoleLocalization = *var->val.integer;
            break;

        case COLUMN_PRTCONSOLEDISABLE:
            /** PrtConsoleDisableTC = ASN_INTEGER */
            row_ctx->prtConsoleDisable = *var->val.integer;
            break;

        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
            /** PresentOnOff = ASN_INTEGER */
            row_ctx->prtAuxiliarySheetStartupPage = *var->val.integer;
            break;

        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            /** PresentOnOff = ASN_INTEGER */
            row_ctx->prtAuxiliarySheetBannerPage = *var->val.integer;
            break;

        case COLUMN_PRTGENERALPRINTERNAME:
            /** OCTETSTR = ASN_OCTET_STR */
            memcpy(row_ctx->prtGeneralPrinterName, var->val.string,
                                                    var->val_len);
            row_ctx->prtGeneralPrinterName_len = var->val_len;
            break;

        case COLUMN_PRTGENERALSERIALNUMBER:
            /** OCTETSTR = ASN_OCTET_STR */
            memcpy(row_ctx->prtGeneralSerialNumber, var->val.string,
                                                    var->val_len);
            row_ctx->prtGeneralSerialNumber_len = var->val_len;
            break;
#else
	
        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
        case COLUMN_PRTGENERALRESET:
        case COLUMN_PRTINPUTDEFAULTINDEX:
        case COLUMN_PRTOUTPUTDEFAULTINDEX:
        case COLUMN_PRTMARKERDEFAULTINDEX:
        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
        case COLUMN_PRTCONSOLELOCALIZATION:
        case COLUMN_PRTCONSOLEDISABLE:
        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            /** INTEGER32 = ASN_INTEGER */
            set_prtMIB_OIDValue(&row_ctx->columnInfo[current->tri->colnum], 0,
                                    (void *) var->val.integer, NULL);
            break;

        case COLUMN_PRTGENERALCURRENTOPERATOR:
        case COLUMN_PRTGENERALSERVICEPERSON:
        case COLUMN_PRTGENERALPRINTERNAME:
        case COLUMN_PRTGENERALSERIALNUMBER:
            /** OCTETSTR = ASN_OCTET_STR */
            set_prtMIB_OIDValue(&row_ctx->columnInfo[current->tri->colnum], 0,
                                    (void *) var->val.string, NULL);
            break;
#endif

        default: /** We shouldn't get here */
            netsnmp_assert(0); /** why wasn't this caught in reserve1? */
        }
    }
}

/************************************************************
 * Only once the ACTION phase has completed successfully, can
 * the final COMMIT phase be run. This is used to complete any
 * writes that were done into temporary storage, and then release
 * any allocated resources. Note that all the code in this phase
 * should be "safe" code that cannot possibly fail (cue
 * hysterical laughter). The whole intent of the ACTION/COMMIT
 * division is that all of the fallible code should be done in
 * the ACTION phase, so that it can be backed out if necessary.
 *
 * BEFORE calling this routine, the agent will update the
 * container (inserting a row if row_created == 1, or removing
 * the row if row_deleted == 1).
 *
 * AFTER calling this routine, the agent will delete the
 * undo_info.
 */
void prtGeneralTable_set_commit( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    prtGeneralTable_context *row_ctx = 
        (prtGeneralTable_context *) rg->existing_row;
    prtGeneralTable_context *undo_ctx = 
        (prtGeneralTable_context *) rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALRESET:
            /** PrtGeneralResetTC = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALCURRENTOPERATOR:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTGENERALSERVICEPERSON:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTINPUTDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTOUTPUTDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMARKERDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTCONSOLELOCALIZATION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTCONSOLEDISABLE:
            /** PrtConsoleDisableTC = ASN_INTEGER */
        break;

        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
            /** PresentOnOff = ASN_INTEGER */
        break;

        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            /** PresentOnOff = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALPRINTERNAME:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTGENERALSERIALNUMBER:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        default: /** We shouldn't get here */
            netsnmp_assert(0); /** why wasn't this caught in reserve1? */
        }
    }
}

/************************************************************
 * If either of the RESERVE calls fail, the write routines
 * are called again with the FREE action, to release any resources
 * that have been allocated. The agent will then return a failure
 * response to the requesting application.
 *
 * AFTER calling this routine, the agent will delete undo_info.
 */
void prtGeneralTable_set_free( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /*
    prtGeneralTable_context *row_ctx = 
        (prtGeneralTable_context *)rg->existing_row;
    prtGeneralTable_context *undo_ctx = 
        (prtGeneralTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALRESET:
            /** PrtGeneralResetTC = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALCURRENTOPERATOR:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTGENERALSERVICEPERSON:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTINPUTDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTOUTPUTDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMARKERDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTCONSOLELOCALIZATION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTCONSOLEDISABLE:
            /** PrtConsoleDisableTC = ASN_INTEGER */
        break;

        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
            /** PresentOnOff = ASN_INTEGER */
        break;

        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            /** PresentOnOff = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALPRINTERNAME:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTGENERALSERIALNUMBER:
            /** OCTETSTR = ASN_OCTET_STR */
        break;
        }
    }
}

/************************************************************
 * If the ACTION phase does fail (for example due to an apparently
 * valid, but unacceptable value, or an unforeseen problem), then
 * the list of write routines are called again, with the UNDO
 * action. This requires the routine to reset the value that was
 * changed to its previous value (assuming it was actually changed),
 * and then to release any resources that had been allocated. As
 * with the FREE phase, the agent will then return an indication
 * of the error to the requesting application.
 *
 * BEFORE calling this routine, the agent will update the container
 * (remove any newly inserted row, re-insert any removed row).
 *
 * AFTER calling this routing, the agent will call row_copy
 * to restore the data in existing_row from the date in undo_info.
 * Then undo_info will be deleted (or existing row, if row_created
 * == 1).
 */
void prtGeneralTable_set_undo( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /* 
    prtGeneralTable_context *row_ctx = 
        (prtGeneralTable_context *)rg->existing_row;
    prtGeneralTable_context *undo_ctx = 
        (prtGeneralTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALRESET:
            /** PrtGeneralResetTC = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALCURRENTOPERATOR:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTGENERALSERVICEPERSON:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTINPUTDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTOUTPUTDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMARKERDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTCONSOLELOCALIZATION:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTCONSOLEDISABLE:
            /** PrtConsoleDisableTC = ASN_INTEGER */
        break;

        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
            /** PresentOnOff = ASN_INTEGER */
        break;

        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            /** PresentOnOff = ASN_INTEGER */
        break;

        case COLUMN_PRTGENERALPRINTERNAME:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTGENERALSERIALNUMBER:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        default: /** We shouldn't get here */
            netsnmp_assert(0); /** why wasn't this caught in reserve1? */
        }
    }
}

/************************************************************
 * initialize the table
 */
void initialize_table_prtGeneralTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR,
                "initialize_table_prtGeneralTable_handler called again\n");
        return;
    }

    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (!table_info)
    {
        snmp_log(LOG_ERR, "malloc table_oinfo failed in "
                 "initialize_table_prtGeneralTable_handler\n");
        return;
    }

    my_handler = netsnmp_create_handler_registration("prtGeneralTable",
                                             netsnmp_table_array_helper_handler,
                                             prtGeneralTable_oid,
                                             prtGeneralTable_oid_len,
                                             HANDLER_CAN_RWRITE
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_prtGeneralTable_handler\n");
        SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
        /** index: hrDeviceIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = prtGeneralTable_COL_MIN;
    table_info->max_column = prtGeneralTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = prtGeneralTable_get_value;
    cb.container = netsnmp_container_find("prtGeneralTable_primary:"
                                          "prtGeneralTable:"
                                          "table_container");
    cb.can_set = 1;
    cb.duplicate_row = (UserRowMethod*)prtGeneralTable_duplicate_row;
    cb.delete_row = (UserRowMethod*)prtGeneralTable_delete_row;
    cb.row_copy = (Netsnmp_User_Row_Operation *)prtGeneralTable_row_copy;

    cb.can_delete = (Netsnmp_User_Row_Action *)prtGeneralTable_can_delete;

    cb.set_reserve1 = prtGeneralTable_set_reserve1;
    cb.set_reserve2 = prtGeneralTable_set_reserve2;
    cb.set_action = prtGeneralTable_set_action;
    cb.set_commit = prtGeneralTable_set_commit;
    cb.set_free = prtGeneralTable_set_free;
    cb.set_undo = prtGeneralTable_set_undo;
    DEBUGMSGTL(("initialize_table_prtGeneralTable",
                "Registering table prtGeneralTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * get data from a column
 */
int prtGeneralTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    char str[255] = {0};
    // uint32_t str_sz = 255;
    uint32_t num = 0;

    DBG_VERBOSE("%s() starts x%x col %d\n", __func__, (unsigned int)table_info, table_info->colnum);
	
    netsnmp_variable_list *var = request->requestvb;
    prtGeneralTable_context *ctx = (prtGeneralTable_context *) item;

    switch (table_info->colnum) {

#if 0
        case COLUMN_PRTGENERALCONFIGCHANGES:
#else
        case COLUMN_PRTGENERALCONFIGCHANGES:
        case COLUMN_PRTALERTCRITICALEVENTS:
        case COLUMN_PRTALERTALLEVENTS:
            if (OK != get_prtMIB_OIDValue(&ctx->columnInfo[table_info->colnum],
                                            0, (void *) &num, NULL))
                num = PRTMIB_NUM_UNIMPLEMENTED;
            snmp_set_var_typed_value(
                        var, ASN_COUNTER, (char *) &num, sizeof(num));
            break;
    
        case COLUMN_PRTGENERALCURRENTLOCALIZATION:
        case COLUMN_PRTINPUTDEFAULTINDEX:
        case COLUMN_PRTOUTPUTDEFAULTINDEX:
        case COLUMN_PRTMARKERDEFAULTINDEX:
        case COLUMN_PRTMEDIAPATHDEFAULTINDEX:
        case COLUMN_PRTCONSOLELOCALIZATION:
        case COLUMN_PRTCONSOLENUMBEROFDISPLAYLINES:
        case COLUMN_PRTCONSOLENUMBEROFDISPLAYCHARS:
        case COLUMN_PRTCONSOLEDISABLE:
        case COLUMN_PRTGENERALRESET:
        case COLUMN_PRTAUXILIARYSHEETSTARTUPPAGE:
        case COLUMN_PRTAUXILIARYSHEETBANNERPAGE:
            if (OK != get_prtMIB_OIDValue(&ctx->columnInfo[table_info->colnum],
                                            0, (void *) &num, NULL))
                num = PRTMIB_NUM_UNIMPLEMENTED;
            snmp_set_var_typed_value(
                        var, ASN_INTEGER, (char *) &num, sizeof(num));
            break;

        case COLUMN_PRTGENERALCURRENTOPERATOR:
        case COLUMN_PRTGENERALSERVICEPERSON:
			// if (OK != get_prtMIB_OIDValue(&ctx->columnInfo[table_info->colnum],
				//		0, (void *) str, &str_sz)) // blocked by mspyo, There may some conflict with MRVL oid system. 2020-12-23
				strcpy(str, PRTMIB_STR_UNIMPLEMENTED);
			snmp_set_var_typed_value(var, ASN_OCTET_STR, str, strlen(str));
            break;
        case COLUMN_PRTGENERALPRINTERNAME:
			{
				error_type_t db_retval = FAIL;
				char *MachineTLI;
				MachineTLI = db_get_var(INSTANCE_MACHINE,"MachineTLI",&db_retval);
				if((MachineTLI == NULL) || (db_retval == FAIL)){
					strcpy(str, "Unknown TLI");
				}
				else{
                	strcpy(str, MachineTLI);
					MEM_FREE_AND_NULL(MachineTLI);
				}
            	snmp_set_var_typed_value(var, ASN_OCTET_STR, str, strlen(str));
			}
			break;
        case COLUMN_PRTGENERALSERIALNUMBER:
			{
				error_type_t db_retval = FAIL;
				char *MachineSerial;
				MachineSerial = db_get_var(INSTANCE_MACHINE,"MachineSerialNo",&db_retval);
				if((MachineSerial == NULL) || (db_retval == FAIL)){
					strcpy(str, "Unknown Serial Number");
				}
				else{
                	strcpy(str, MachineSerial);
					MEM_FREE_AND_NULL(MachineSerial);
				}
            	snmp_set_var_typed_value(var, ASN_OCTET_STR, str, strlen(str));
			}
			break;
#endif

        default:
            snmp_log(LOG_ERR, "%s: unknown column\n", __func__);
            return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

/************************************************************
 * look for a row based on a set of indexes
 */
const prtGeneralTable_context *prtGeneralTable_get_by_idx(netsnmp_index *hdr)
{
    return (const prtGeneralTable_context *) CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
prtGeneralTable_context *prtGeneralTable_create_row(netsnmp_index *hdr)
{
    prtGeneralTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(prtGeneralTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR,
                "SNMP_MALLOC_TYPEDEF(prtGeneralTable_context) failed!\n");
        return NULL;
    }

    /** check indexes */
    if (prtGeneralTable_extract_index(ctx, hdr)) {
        if (NULL != ctx->index.oids)
            free(ctx->index.oids);
        free(ctx);
        return NULL;
    }

#if 0 /* All column data are stored in the OID block */
    /*
     * initialize fields to default values
     */
    ctx->prtGeneralConfigChanges = 0;
    ctx->prtGeneralCurrentLocalization = 1;
    ctx->prtGeneralReset = 1;
    memset((void *) ctx->prtGeneralCurrentOperator, 0, 128);
    ctx->prtGeneralCurrentOperator_len = 0;
    memset((void *) ctx->prtGeneralServicePerson, 0, 128);
    ctx->prtGeneralServicePerson_len = 0;
    ctx->prtInputDefaultIndex = 1;
    ctx->prtOutputDefaultIndex = 1;
    ctx->prtMarkerDefaultIndex = 1;
    ctx->prtMediaPathDefaultIndex = 1;
    ctx->prtConsoleLocalization = 1;
    ctx->prtConsoleNumberOfDisplayLines = 1;
    ctx->prtConsoleNumberOfDisplayChars = 1;
    ctx->prtConsoleDisable = 1;
    ctx->prtAuxiliarySheetStartupPage = 1;
    ctx->prtAuxiliarySheetBannerPage = 1;
    memset((void *) ctx->prtGeneralPrinterName, 0, 128);
    ctx->prtGeneralPrinterName_len = 0;
    memset((void *) ctx->prtGeneralSerialNumber, 0, 256);
    ctx->prtGeneralSerialNumber_len = 0;
    ctx->prtAlertCriticalEvents = 0;
    ctx->prtAlertAllEvents = 0;
#endif 

    return ctx;
}

/************************************************************
 * extract a set of indexes into a row
 */
int prtGeneralTable_extract_index(
                            prtGeneralTable_context *ctx, netsnmp_index *hdr)
{
    netsnmp_variable_list var_hrDeviceIndex;
    int err = SNMP_ERR_NOERROR;

    /** validate parameters */
    if (!ctx || !hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return SNMP_ERR_BADVALUE;
    }

    /** ensure target row is empty */
    if (NULL != ctx->index.oids) {
        snmp_log(LOG_ERR, "%s: non-empty row!\n", __func__);
        return SNMP_ERR_GENERR;
    }

    /*
     * initialize variable that will hold each component of the index.
     * If there are multiple indexes for the table, the variable_lists
     * need to be linked together, in order.
     */
    memset(&var_hrDeviceIndex, 0x00, sizeof(var_hrDeviceIndex));
    var_hrDeviceIndex.type = ASN_INTEGER;
    var_hrDeviceIndex.next_variable = NULL;

    /** parse the oid into the individual index components */
    err = parse_oid_indexes(hdr->oids, hdr->len, &var_hrDeviceIndex);
    if (err == SNMP_ERR_NOERROR) {
        /** copy index components into the context structure */
        if (snmp_clone_mem((void *) &ctx->index.oids,
                    hdr->oids, hdr->len * sizeof(oid))) {
            return SNMP_ERR_GENERR;
        }
        ctx->index.len = hdr->len;
    }

    /** parsing may have allocated memory. free it. */
    snmp_reset_var_buffers( &var_hrDeviceIndex );

    return err;
}

