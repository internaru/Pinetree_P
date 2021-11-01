/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtInputTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#define DBG_PRFX "snmp: "
#include "net_debug.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "prtInputTable.h"

#include "SysServiceMgr.h"
#include "paper_size_config.h"

#include "db_api.h"
#include "memAPI.h"

#define PRT_INPUT_MPT_MAX_CAPACITY 50 // For Kara-S, by mspyo
#define PRT_INPUT_TRAY_MAX_CAPACITY 500 // For Kars-S, by mspyo
#define SNMP_STRING_SIZE_MAX 256

static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid prtInputTable_oid[] = { prtInputTable_TABLE_OID };
size_t prtInputTable_oid_len = OID_LENGTH(prtInputTable_oid);

static int prtInputTable_row_copy(
                    prtInputTable_context *dst, prtInputTable_context *src);

static int prtInputTable_module_initialized = 0;

/*
struct paper_size
{
    char *name;
    int width;
    int height;
};

struct paper_size paper_size_info [] =
{
    {"A4", 210000, 297000},	//0
    {"A5_SEF", 148000, 210000},
    {"A5_LEF", 210000, 148000},
    {"A6_SEF", 105000, 148000},
    {"B5", 182000, 257000},
    {"B6", 128000, 182000}, //5
    {"LETTER", 216000, 279000},
    {"LEGAL", 216000, 356000},
    {"EXECUTIVE", 184000, 267000},
    {"STATEMENT", 140000, 216000},
    {"FOLIO", 216000, 330000},//10
    {"QUATRO", 203000, 254000},
    {"G_LETTER", 203000, 267000},
    {"G_LEGAL", 210000, 356000},
    {"CARD_100X148", 100000, 148000},
    {"CARD_148X200", 148000, 200000},//15
    {"CARD_200X148", 200000, 148000},
    {"ENV_MONARCH", 98000, 191000},
    {"ENV_COM9", 98000, 225000},
    {"ENV_COM10", 105000, 241000},
    {"ENV_DL", 110000, 220000},//20
    {"ENV_C5", 162000, 229000},
    {"ENV_C6", 114000, 162000},
	{"Unknown", 0, 0} //23
};
*/

char* tray_type[] = {"TrayConfig_mediaType_0", "TrayConfig_mediaType_1", "TrayConfig_mediaType_2", "TrayConfig_mediaType_3"};
char* tray_size[] = {"TrayConfig_mediaSize_0", "TrayConfig_mediaSize_1", "TrayConfig_mediaSize_2", "TrayConfig_mediaSize_3"};

// #define TRAYTOTALCOUNT 4

error_type_t getMediaName(uint32_t mediaNameIndex, char *mediaName);
error_type_t getMediaType(uint32_t mediaTypeIndex, char *mediaType);

#if 0
/************************************************************
 * get data from a row
 */
int get_prtInputTable_data(
                    prtInputTable_context *out, unsigned long colFlags)
{
    prtInputTable_context *row;
    int getAll = colFlags ? 0 : 1;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtInputTable_module_initialized) {
        snmp_log(LOG_ERR, 
                "%s: input table module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!out || out->index.len != 2 || !out->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }

    /** find desired row */
    hdr = &out->index;
    row = (prtInputTable_context *) prtInputTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** get data */
    for (col = 1; col <= prtInputTable_COL_MAX; col++) {
        if (!getAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTINPUTINDEX:
            out->prtInputIndex = row->prtInputIndex;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTTYPE:
            out->prtInputType = row->prtInputType;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTDIMUNIT:
            out->prtInputDimUnit = row->prtInputDimUnit;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
            out->prtInputMediaDimFeedDirDeclared = 
                row->prtInputMediaDimFeedDirDeclared;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
            out->prtInputMediaDimXFeedDirDeclared = 
                row->prtInputMediaDimXFeedDirDeclared;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMFEEDDIRCHOSEN:
            out->prtInputMediaDimFeedDirChosen = 
                row->prtInputMediaDimFeedDirChosen;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRCHOSEN:
            out->prtInputMediaDimXFeedDirChosen = 
                row->prtInputMediaDimXFeedDirChosen;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTCAPACITYUNIT:
            out->prtInputCapacityUnit = row->prtInputCapacityUnit;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMAXCAPACITY:
            out->prtInputMaxCapacity = row->prtInputMaxCapacity;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTCURRENTLEVEL:
            out->prtInputCurrentLevel = row->prtInputCurrentLevel;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTSTATUS:
            out->prtInputStatus = row->prtInputStatus;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIANAME:
            memcpy((void*) out->prtInputMediaName,
                    (const void *) row->prtInputMediaName,
                    row->prtInputMediaName_len);
            out->prtInputMediaName_len = row->prtInputMediaName_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTNAME:
            memcpy((void*) out->prtInputName,
                    (const void *) row->prtInputName,
                    row->prtInputName_len);
            out->prtInputName_len = row->prtInputName_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTVENDORNAME:
            memcpy((void*) out->prtInputVendorName,
                    (const void *) row->prtInputVendorName,
                    row->prtInputVendorName_len);
            out->prtInputVendorName_len = row->prtInputVendorName_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMODEL:
            memcpy((void*) out->prtInputModel,
                    (const void *) row->prtInputModel,
                    row->prtInputModel_len);
            out->prtInputModel_len = row->prtInputModel_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTVERSION:
            memcpy((void*) out->prtInputVersion,
                    (const void *) row->prtInputVersion,
                    row->prtInputVersion_len);
            out->prtInputVersion_len = row->prtInputVersion_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTSERIALNUMBER:
            memcpy((void*) out->prtInputSerialNumber,
                    (const void *) row->prtInputSerialNumber,
                    row->prtInputSerialNumber_len);
            out->prtInputSerialNumber_len = row->prtInputSerialNumber_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTDESCRIPTION:
            memcpy((void*) out->prtInputDescription,
                    (const void *) row->prtInputDescription,
                    row->prtInputDescription_len);
            out->prtInputDescription_len = row->prtInputDescription_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTSECURITY:
            out->prtInputSecurity = row->prtInputSecurity;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIAWEIGHT:
            out->prtInputMediaWeight = row->prtInputMediaWeight;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIATYPE:
            memcpy((void*) out->prtInputMediaType,
                    (const void *) row->prtInputMediaType,
                    row->prtInputMediaType_len);
            out->prtInputMediaType_len = row->prtInputMediaType_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIACOLOR:
            memcpy((void*) out->prtInputMediaColor,
                    (const void *) row->prtInputMediaColor,
                    row->prtInputMediaColor_len);
            out->prtInputMediaColor_len = row->prtInputMediaColor_len;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIAFORMPARTS:
            out->prtInputMediaFormParts = row->prtInputMediaFormParts;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
            out->prtInputMediaLoadTimeout = row->prtInputMediaLoadTimeout;
            if (!getAll)
                break;
        case COLUMN_PRTINPUTNEXTINDEX:
            out->prtInputNextIndex = row->prtInputNextIndex;
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
int set_prtInputTable_data(
                    prtInputTable_context *data, unsigned long colFlags)
{
    prtInputTable_context *row;
    int setAll = colFlags ? 0 : 1;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtInputTable_module_initialized) {
        snmp_log(LOG_ERR, 
                "%s: input table module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!data || data->index.len != 2 || !data->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }
    if (chk_prtInputTable_data(data, colFlags))
        return -1;

    /** find desired row */
    hdr = &data->index;
    row = (prtInputTable_context *) prtInputTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** set data */
    for (col = 1; col <= prtInputTable_COL_MAX; col++) {
        if (!setAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTINPUTINDEX:
            snmp_log(LOG_ERR, "%s: cannot change input index\n", __func__);
            if (!setAll)
                break;
        case COLUMN_PRTINPUTTYPE:
            row->prtInputType = data->prtInputType;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTDIMUNIT:
            row->prtInputDimUnit = data->prtInputDimUnit;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
            row->prtInputMediaDimFeedDirDeclared = 
                data->prtInputMediaDimFeedDirDeclared;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
            row->prtInputMediaDimXFeedDirDeclared = 
                data->prtInputMediaDimXFeedDirDeclared;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMFEEDDIRCHOSEN:
            row->prtInputMediaDimFeedDirChosen = 
                data->prtInputMediaDimFeedDirChosen;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRCHOSEN:
            row->prtInputMediaDimXFeedDirChosen = 
                data->prtInputMediaDimXFeedDirChosen;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTCAPACITYUNIT:
            row->prtInputCapacityUnit = data->prtInputCapacityUnit;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMAXCAPACITY:
            row->prtInputMaxCapacity = data->prtInputMaxCapacity;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTCURRENTLEVEL:
            row->prtInputCurrentLevel = data->prtInputCurrentLevel;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTSTATUS:
            row->prtInputStatus = data->prtInputStatus;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIANAME:
            memcpy((void*) row->prtInputMediaName,
                    (const void *) data->prtInputMediaName,
                    data->prtInputMediaName_len);
            row->prtInputMediaName_len = data->prtInputMediaName_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTNAME:
            memcpy((void*) row->prtInputName,
                    (const void *) data->prtInputName,
                    data->prtInputName_len);
            row->prtInputName_len = data->prtInputName_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTVENDORNAME:
            memcpy((void*) row->prtInputVendorName,
                    (const void *) data->prtInputVendorName,
                    data->prtInputVendorName_len);
            row->prtInputVendorName_len = data->prtInputVendorName_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMODEL:
            memcpy((void*) row->prtInputModel,
                    (const void *) data->prtInputModel,
                    data->prtInputModel_len);
            row->prtInputModel_len = data->prtInputModel_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTVERSION:
            memcpy((void*) row->prtInputVersion,
                    (const void *) data->prtInputVersion,
                    data->prtInputVersion_len);
            row->prtInputVersion_len = data->prtInputVersion_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTSERIALNUMBER:
            memcpy((void*) row->prtInputSerialNumber,
                    (const void *) data->prtInputSerialNumber,
                    data->prtInputSerialNumber_len);
            row->prtInputSerialNumber_len = data->prtInputSerialNumber_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTDESCRIPTION:
            memcpy((void*) row->prtInputDescription,
                    (const void *) data->prtInputDescription,
                    data->prtInputDescription_len);
            row->prtInputDescription_len = data->prtInputDescription_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTSECURITY:
            row->prtInputSecurity = data->prtInputSecurity;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIAWEIGHT:
            row->prtInputMediaWeight = data->prtInputMediaWeight;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIATYPE:
            memcpy((void*) row->prtInputMediaType,
                    (const void *) data->prtInputMediaType,
                    data->prtInputMediaType_len);
            row->prtInputMediaType_len = data->prtInputMediaType_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIACOLOR:
            memcpy((void*) row->prtInputMediaColor,
                    (const void *) data->prtInputMediaColor,
                    data->prtInputMediaColor_len);
            row->prtInputMediaColor_len = data->prtInputMediaColor_len;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIAFORMPARTS:
            row->prtInputMediaFormParts = data->prtInputMediaFormParts;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
            row->prtInputMediaLoadTimeout = data->prtInputMediaLoadTimeout;
            if (!setAll)
                break;
        case COLUMN_PRTINPUTNEXTINDEX:
            row->prtInputNextIndex = data->prtInputNextIndex;
            if (!setAll)
                break;

        default:
            return 0;
        }
    }
    return 0;
}

/************************************************************
 * delete a row from the table
 */
int del_prtInputTable_entry(long devNdx, long inpNdx)
{
    oid ndxs[2];
    netsnmp_index hdr;
    prtInputTable_context *row;

    /** make sure the module is initialized */
    if (!prtInputTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: input table module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndxs[0] = devNdx;
    ndxs[1] = inpNdx;
    hdr.len = 2;
    hdr.oids = ndxs;

    /** find desired row */
    row = (prtInputTable_context *) 
                        prtInputTable_get_by_idx(&hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                                    __func__, (int) ndxs[0], (int) ndxs[1]);
        return -1;
    }

    /** remove row from table and free allocated memory */
    if (CONTAINER_REMOVE(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: error removing row[%d][%d]\n",
                                    __func__, (int) ndxs[0], (int) ndxs[1]);
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
int add_prtInputTable_entry(prtInputTable_context *info)
{
    netsnmp_index *hdr;
    prtInputTable_context *row;

    /** make sure the module is initialized */
    if (!prtInputTable_module_initialized)
        init_prtInputTable();

    /** validate input info */
    if (!info || !info->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete input info\n", __func__);
        return -1;
    }
    if (chk_prtInputTable_data(info, 0))
        return -1;

    /** make sure desired index tuple are non-existent */
    hdr = &info->index;
    row = (prtInputTable_context *) prtInputTable_get_by_idx(hdr);
    if (row) {
        snmp_log(LOG_ERR, "%s: index[%d][%d] already in use\n",
                    __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** create row context and insert into container */
    row = prtInputTable_create_row(hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: add new row failed\n", __func__);
        return -1;
    }

    /** copy column data */
    if (prtInputTable_row_copy(row, info)) {
        snmp_log(LOG_ERR, "%s: row content copying failed\n", __func__);
        return -1;
    }

    return 0;
}

/************************************************************
 * validate one or more column data
 */
int chk_prtInputTable_data(prtInputTable_context *data, unsigned long colFlags)
{
    int checkAll = colFlags ? 0 : 1;
    int col;

    if (!data) {
        snmp_log(LOG_ERR, "%s: no data to validate\n", __func__);
        return -1;
    }

    /** check indexes */
    if (!IS_1TO7FFFFFFF(data->index.oids[0])) {
        snmp_log(LOG_ERR, "invalid host device index\n");
        return -1;
    }
    if (!IS_1TOFFFF(data->index.oids[1])) {
        snmp_log(LOG_ERR, "invalid printer input index\n");
        return -1;
    }

    /** check columns */
    for (col = 1; col <= prtInputTable_COL_MAX; col++) {
        if (!checkAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTINPUTINDEX:
            if (!data->prtInputIndex)
                data->prtInputIndex = data->index.oids[1];
            else if (data->prtInputIndex != data->index.oids[1]) {
                snmp_log(LOG_ERR, "%s: inconsistent input index (%d)\n",
                    __func__, (int) data->prtInputIndex);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTTYPE:
            if (!IS_PRTINPUTTYPE_VALID(data->prtInputType)) {
                snmp_log(LOG_ERR, "%s: invalid input type code (%d)\n",
                    __func__, (int) data->prtInputType);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTDIMUNIT:
            if (!IS_PRTMEDIAUNIT_VALID(data->prtInputDimUnit)) {
                snmp_log(LOG_ERR, "%s: invalid input dimensions unit (%d)\n",
                    __func__, (int) data->prtInputDimUnit);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputMediaDimFeedDirDeclared)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid input feed dir dimension (%d)\n",
                        __func__, (int) data->prtInputMediaDimFeedDirDeclared);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputMediaDimXFeedDirDeclared)) {
                snmp_log(LOG_ERR, "%s: invalid input feed x dimension (%d)\n",
                    __func__, (int) data->prtInputMediaDimXFeedDirDeclared);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMFEEDDIRCHOSEN:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputMediaDimFeedDirChosen)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid input chosen media feed dimension (%d)\n",
                        __func__, (int) data->prtInputMediaDimFeedDirChosen);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRCHOSEN:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputMediaDimXFeedDirChosen)) {
                snmp_log(LOG_ERR, 
                    "%s: invalid input chosen media cross dimension (%d)\n",
                    __func__, (int) data->prtInputMediaDimXFeedDirChosen);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTCAPACITYUNIT:
            if (!IS_PRTCAPACITYUNIT_VALID(data->prtInputCapacityUnit)) {
                snmp_log(LOG_ERR, "%s: invalid media capacity code (%d)\n",
                    __func__, (int) data->prtInputCapacityUnit);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMAXCAPACITY:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputMaxCapacity)) {
                snmp_log(LOG_ERR, "%s: invalid maximum input capacity (%d)\n",
                    __func__, (int) data->prtInputMaxCapacity);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTCURRENTLEVEL:
            if (!IS_NEG3TO7FFFFFFF(data->prtInputCurrentLevel)) {
                snmp_log(LOG_ERR, "%s: invalid current input level (%d)\n",
                    __func__, (int) data->prtInputCurrentLevel);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTSTATUS:
            if (!IS_SUBUNITSTATUS_VALID(data->prtInputStatus)) {
                snmp_log(LOG_ERR, "%s: invalid input status code (%d)\n",
                    __func__, (int) data->prtInputStatus);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIANAME:
            if (!IS_0TO63(data->prtInputMediaName_len)) {
                snmp_log(LOG_ERR, "%s: invalid input media name length (%d)\n",
                    __func__, (int) data->prtInputMediaName_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTNAME:
            if (!IS_0TO63(data->prtInputName_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid input sub-unit name length (%d)\n",
                        __func__, (int) data->prtInputName_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTVENDORNAME:
            if (!IS_0TO63(data->prtInputVendorName_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid input vendor name length (%d)\n",
                        __func__, (int) data->prtInputVendorName_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMODEL:
            if (!IS_0TO63(data->prtInputModel_len)) {
                snmp_log(LOG_ERR, "%s: invalid input model length (%d)\n",
                    __func__, (int) data->prtInputModel_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTVERSION:
            if (!IS_0TO63(data->prtInputVersion_len)) {
                snmp_log(LOG_ERR, "%s: invalid input version length (%d)\n",
                    __func__, (int) data->prtInputVersion_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTSERIALNUMBER:
            if (!IS_0TO32(data->prtInputSerialNumber_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid input serial number length (%d)\n",
                        __func__, (int) data->prtInputSerialNumber_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTDESCRIPTION:
            if (!IS_0TO255(data->prtInputDescription_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid input description length (%d)\n",
                        __func__, (int) data->prtInputDescription_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTSECURITY:
            if (!IS_PRESENTONOFF_VALID(data->prtInputSecurity)) {
                snmp_log(LOG_ERR, "%s: invalid security presence code (%d)\n",
                    __func__, (int) data->prtInputSecurity);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIAWEIGHT:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputMediaWeight)) {
                snmp_log(LOG_ERR, "%s: invalid input media weight (%d)\n",
                    __func__, (int) data->prtInputMediaWeight);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIATYPE:
            if (!IS_0TO63(data->prtInputMediaType_len)) {
                snmp_log(LOG_ERR, "%s: invalid input media type length (%d)\n",
                    __func__, (int) data->prtInputMediaType_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIACOLOR:
            if (!IS_0TO63(data->prtInputMediaColor_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid input media color length (%d)\n",
                        __func__, (int) data->prtInputMediaColor_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIAFORMPARTS:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputMediaFormParts)) {
                snmp_log(LOG_ERR, "%s: invalid input media form parts (%d)\n",
                    __func__, (int) data->prtInputMediaFormParts);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputMediaLoadTimeout)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid input media load timeout (%d)\n",
                        __func__, (int) data->prtInputMediaLoadTimeout);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTINPUTNEXTINDEX:
            if (!IS_NEG2TO7FFFFFFF(data->prtInputNextIndex)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid next input sub-unit index (%d)\n",
                        __func__, (int) data->prtInputNextIndex);
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
void populate_prtInputTable(PRTMIB_OID_MAP *map)
{
    oid indexes[2];
    netsnmp_index hdr;
    prtInputTable_context *row;
    uint32_t i, numOfInputTrays;

    if (NULL == map)
        return;

    if (OK != get_prtMIB_OIDValue(map, 0, (void *) &numOfInputTrays, NULL))
        numOfInputTrays = 1;

    if (!prtInputTable_module_initialized)
        initialize_table_prtInputTable();

    hdr.len = 2;
    hdr.oids = indexes;
    indexes[0] = PRTMIB_THIS_HOST_DEVICE_INDEX;

    for (i = 0; i < numOfInputTrays; i++) {

        indexes[1] = i + 1;
        row = prtInputTable_create_row(&hdr);
        if (!row || CONTAINER_INSERT(cb.container, row)) {
            snmp_log(LOG_ERR, "%s: row addition failed\n", __func__);
            return;
        }
        row->columnInfo = map;
    }
}

/************************************************************
 * initialize the module
 */
void init_prtInputTable(void)
{
    initialize_table_prtInputTable();
    prtInputTable_module_initialized = 1;
}

/************************************************************
 * copy row contents
 */
static int prtInputTable_row_copy(
                    prtInputTable_context *dst, prtInputTable_context *src)
{
    if (!dst || !src)
        return 1;
        
    /** copy index, if provided */
    if (dst->index.oids)
        free(dst->index.oids);
    if (snmp_clone_mem( (void *) &dst->index.oids, src->index.oids,
                           src->index.len * sizeof(oid) )) {
        dst->index.oids = NULL;
        return 1;
    }
    dst->index.len = src->index.len;
    
#if 0
    /** copy components into the context structure */
    dst->prtInputIndex = src->prtInputIndex;
    dst->prtInputType = src->prtInputType;
    dst->prtInputDimUnit = src->prtInputDimUnit;
    dst->prtInputMediaDimFeedDirDeclared = 
        src->prtInputMediaDimFeedDirDeclared;
    dst->prtInputMediaDimXFeedDirDeclared = 
        src->prtInputMediaDimXFeedDirDeclared;
    dst->prtInputMediaDimFeedDirChosen = src->prtInputMediaDimFeedDirChosen;
    dst->prtInputMediaDimXFeedDirChosen = src->prtInputMediaDimXFeedDirChosen;
    dst->prtInputCapacityUnit = src->prtInputCapacityUnit;
    dst->prtInputMaxCapacity = src->prtInputMaxCapacity;
    dst->prtInputCurrentLevel = src->prtInputCurrentLevel;
    dst->prtInputStatus = src->prtInputStatus;
    memcpy(dst->prtInputMediaName, 
                    src->prtInputMediaName, src->prtInputMediaName_len);
    dst->prtInputMediaName_len = src->prtInputMediaName_len;
    memcpy(dst->prtInputName, src->prtInputName, src->prtInputName_len);
    dst->prtInputName_len = src->prtInputName_len;
    memcpy(dst->prtInputVendorName, 
                    src->prtInputVendorName, src->prtInputVendorName_len);
    dst->prtInputVendorName_len = src->prtInputVendorName_len;
    memcpy(dst->prtInputModel, src->prtInputModel, src->prtInputModel_len);
    dst->prtInputModel_len = src->prtInputModel_len;
    memcpy(dst->prtInputVersion, 
                    src->prtInputVersion, src->prtInputVersion_len);
    dst->prtInputVersion_len = src->prtInputVersion_len;
    memcpy(dst->prtInputSerialNumber, 
                    src->prtInputSerialNumber, src->prtInputSerialNumber_len);
    dst->prtInputSerialNumber_len = src->prtInputSerialNumber_len;
    memcpy(dst->prtInputDescription, 
                    src->prtInputDescription, src->prtInputDescription_len);
    dst->prtInputDescription_len = src->prtInputDescription_len;
    dst->prtInputSecurity = src->prtInputSecurity;
    dst->prtInputMediaWeight = src->prtInputMediaWeight;
    memcpy(dst->prtInputMediaType, 
                    src->prtInputMediaType, src->prtInputMediaType_len);
    dst->prtInputMediaType_len = src->prtInputMediaType_len;
    memcpy(dst->prtInputMediaColor, 
                    src->prtInputMediaColor, src->prtInputMediaColor_len);
    dst->prtInputMediaColor_len = src->prtInputMediaColor_len;
    dst->prtInputMediaFormParts = src->prtInputMediaFormParts;
    dst->prtInputMediaLoadTimeout = src->prtInputMediaLoadTimeout;
    dst->prtInputNextIndex = src->prtInputNextIndex;
#else
    dst->columnInfo = src->columnInfo;
#endif

    return 0;
}

/************************************************************
 * check if a row can be deleted
 * return 1 if the row can be deleted
 * return 0 if the row cannot be deleted
 */
int prtInputTable_can_delete(   prtInputTable_context *undo_ctx, 
                                prtInputTable_context *row_ctx, 
                                netsnmp_request_group * rg)
{
    return 0;
}


/************************************************************
 * duplicate a row
 */
prtInputTable_context *prtInputTable_duplicate_row(
                                        prtInputTable_context * row_ctx)
{
    prtInputTable_context * dup;

    if (!row_ctx)
        return NULL;

    dup = SNMP_MALLOC_TYPEDEF(prtInputTable_context);
    if (!dup)
        return NULL;
        
    if (prtInputTable_row_copy(dup,row_ctx)) {
        free(dup);
        dup = NULL;
    }

    return dup;
}

/************************************************************
 * delete a row
 */
netsnmp_index *prtInputTable_delete_row(prtInputTable_context *ctx)
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
void prtInputTable_set_reserve1 (netsnmp_request_group *rg)
{
    prtInputTable_context *row_ctx = (prtInputTable_context *)rg->existing_row;
    /*
    prtInputTable_context *undo_ctx = (prtInputTable_context *)rg->undo_info;
    */
    netsnmp_variable_list *var;
    netsnmp_request_group_item *current;
    int rc;

    for (current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;
        rc = SNMP_ERR_NOERROR;

        if (!IS_COLUMN_IMPLEMENTED(row_ctx->columnInfo, current->tri->colnum))
            continue; /** no checking needed for unimplemented columns */

        switch (current->tri->colnum) {

        case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
        case COLUMN_PRTINPUTMAXCAPACITY:
        case COLUMN_PRTINPUTMEDIAWEIGHT:
        case COLUMN_PRTINPUTMEDIAFORMPARTS:
        case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
            rc = netsnmp_check_vb_int_range(var, -2, 0x7FFFFFFF);
            break;

        case COLUMN_PRTINPUTCURRENTLEVEL:
        case COLUMN_PRTINPUTNEXTINDEX:
            rc = netsnmp_check_vb_int_range(var, -3, 0x7FFFFFFF);
            break;

        case COLUMN_PRTINPUTMEDIANAME:
        case COLUMN_PRTINPUTNAME:
        case COLUMN_PRTINPUTMEDIATYPE:
        case COLUMN_PRTINPUTMEDIACOLOR:
            rc = netsnmp_check_vb_type_and_max_size(var, ASN_OCTET_STR, 63);
            break;

        case COLUMN_PRTINPUTSECURITY:
            if (!IS_PRESENTONOFF_VALID(*var->val.integer))
                rc = SNMP_ERR_WRONGVALUE;
            break;

        default: /** We shouldn't get here */
            rc = SNMP_ERR_GENERR;
            snmp_log(LOG_ERR, "unknown column in "
                     "prtInputTable_set_reserve1\n");
        }

        if (rc)
           netsnmp_set_mode_request_error(MODE_SET_BEGIN, current->ri, rc );
        rg->status = SNMP_MAX( rg->status, current->ri->status );
    }
}

void prtInputTable_set_reserve2( netsnmp_request_group *rg )
{
    prtInputTable_context *row_ctx = (prtInputTable_context *)rg->existing_row;
    /*
    prtInputTable_context *undo_ctx = (prtInputTable_context *)rg->undo_info;
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

        case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
        case COLUMN_PRTINPUTMAXCAPACITY:
        case COLUMN_PRTINPUTMEDIAWEIGHT:
        case COLUMN_PRTINPUTMEDIAFORMPARTS:
        case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
            if (!IS_NEG2TO7FFFFFFF(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTINPUTCURRENTLEVEL:
        case COLUMN_PRTINPUTNEXTINDEX:
            if (!IS_NEG3TO7FFFFFFF(*var->val.integer))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTINPUTMEDIANAME:
        case COLUMN_PRTINPUTNAME:
        case COLUMN_PRTINPUTMEDIATYPE:
        case COLUMN_PRTINPUTMEDIACOLOR:
            if (!IS_0TO63(var->val_len))
                rc = SNMP_ERR_INCONSISTENTVALUE;
            break;

        case COLUMN_PRTINPUTSECURITY:
            if (!IS_PRESENTONOFF_VALID(*var->val.integer))
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
void prtInputTable_set_action( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    prtInputTable_context *row_ctx = (prtInputTable_context *)rg->existing_row;
    /*
    prtInputTable_context *undo_ctx = (prtInputTable_context *)rg->undo_info;
    */
    netsnmp_request_group_item *current;

    PRTMIB_OID_MAP *map;
    uint32_t ndx;

    for (current = rg->list; current; current = current->next) {

        var = current->ri->requestvb;

        map = row_ctx->columnInfo + current->tri->colnum;
        ndx = map->marvell_oid_index;
        if (ndx == INDEX_VARIABLE)
            ndx = row_ctx->index.oids[1] - 1;

        switch (current->tri->colnum) {

#if 0
        case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputMediaDimFeedDirDeclared = *var->val.integer;
        break;

        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputMediaDimXFeedDirDeclared = *var->val.integer;
        break;

        case COLUMN_PRTINPUTMAXCAPACITY:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputMaxCapacity = *var->val.integer;
        break;

        case COLUMN_PRTINPUTCURRENTLEVEL:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputCurrentLevel = *var->val.integer;
        break;

        case COLUMN_PRTINPUTMEDIANAME:
            /** OCTETSTR = ASN_OCTET_STR */
            memcpy(row_ctx->prtInputMediaName,var->val.string,var->val_len);
            row_ctx->prtInputMediaName_len = var->val_len;
        break;

        case COLUMN_PRTINPUTNAME:
            /** OCTETSTR = ASN_OCTET_STR */
            memcpy(row_ctx->prtInputName,var->val.string,var->val_len);
            row_ctx->prtInputName_len = var->val_len;
        break;

        case COLUMN_PRTINPUTSECURITY:
            /** PresentOnOff = ASN_INTEGER */
            row_ctx->prtInputSecurity = *var->val.integer;
        break;

        case COLUMN_PRTINPUTMEDIAWEIGHT:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputMediaWeight = *var->val.integer;
        break;

        case COLUMN_PRTINPUTMEDIATYPE:
            /** OCTETSTR = ASN_OCTET_STR */
            memcpy(row_ctx->prtInputMediaType,var->val.string,var->val_len);
            row_ctx->prtInputMediaType_len = var->val_len;
        break;

        case COLUMN_PRTINPUTMEDIACOLOR:
            /** OCTETSTR = ASN_OCTET_STR */
            memcpy(row_ctx->prtInputMediaColor,var->val.string,var->val_len);
            row_ctx->prtInputMediaColor_len = var->val_len;
        break;

        case COLUMN_PRTINPUTMEDIAFORMPARTS:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputMediaFormParts = *var->val.integer;
        break;

        case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputMediaLoadTimeout = *var->val.integer;
        break;

        case COLUMN_PRTINPUTNEXTINDEX:
            /** INTEGER32 = ASN_INTEGER */
            row_ctx->prtInputNextIndex = *var->val.integer;
        break;
#else
        case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
        case COLUMN_PRTINPUTMAXCAPACITY:
        case COLUMN_PRTINPUTCURRENTLEVEL:
        case COLUMN_PRTINPUTSECURITY:
        case COLUMN_PRTINPUTMEDIAWEIGHT:
        case COLUMN_PRTINPUTMEDIAFORMPARTS:
        case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
        case COLUMN_PRTINPUTNEXTINDEX:
            /** INTEGER32 = ASN_INTEGER */
            set_prtMIB_OIDValue(map, ndx, (void *) var->val.integer, NULL);
            break;

        case COLUMN_PRTINPUTMEDIANAME:
        case COLUMN_PRTINPUTNAME:
        case COLUMN_PRTINPUTMEDIATYPE:
        case COLUMN_PRTINPUTMEDIACOLOR:
            /** OCTETSTR = ASN_OCTET_STR */
            set_prtMIB_OIDValue(map, ndx, (void *) var->val.string, NULL);
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
void prtInputTable_set_commit( netsnmp_request_group *rg )
{
    netsnmp_variable_list *var;
    /* prtInputTable_context *row_ctx = 
            (prtInputTable_context *)rg->existing_row;
    prtInputTable_context *undo_ctx = 
            (prtInputTable_context *)rg->undo_info; */
    netsnmp_request_group_item *current;

    /*
     * loop through columns
     */
    for( current = rg->list; current; current = current->next ) {

        var = current->ri->requestvb;

        switch(current->tri->colnum) {

        case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTINPUTMAXCAPACITY:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTINPUTCURRENTLEVEL:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTINPUTMEDIANAME:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTINPUTNAME:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

        case COLUMN_PRTINPUTSECURITY:
            /** PresentOnOff = ASN_INTEGER */
        break;

        case COLUMN_PRTINPUTMEDIAWEIGHT:
            /** INTEGER32 = ASN_INTEGER */
        break;

        case COLUMN_PRTINPUTMEDIATYPE:
            /** OCTETSTR = ASN_OCTET_STR */
        break;

		case COLUMN_PRTINPUTMEDIACOLOR:
		/** OCTETSTR = ASN_OCTET_STR */
		break;

		case COLUMN_PRTINPUTMEDIAFORMPARTS:
		/** INTEGER32 = ASN_INTEGER */
		break;

		case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
		/** INTEGER32 = ASN_INTEGER */
		break;

		case COLUMN_PRTINPUTNEXTINDEX:
		/** INTEGER32 = ASN_INTEGER */
		break;

		default: /** We shouldn't get here */
		netsnmp_assert(0); /** why wasn't this caught in reserve1? */
		}
	}

	/*
	 * done with all the columns. Could check row related
	 * requirements here.
	 */
}

/************************************************************
 * If either of the RESERVE calls fail, the write routines
 * are called again with the FREE action, to release any resources
 * that have been allocated. The agent will then return a failure
 * response to the requesting application.
 *
 * AFTER calling this routine, the agent will delete undo_info.
 */
void prtInputTable_set_free( netsnmp_request_group *rg )
{
	netsnmp_variable_list *var;
	/* prtInputTable_context *row_ctx = 
	   (prtInputTable_context *)rg->existing_row;
	   prtInputTable_context *undo_ctx = 
	   (prtInputTable_context *)rg->undo_info; */
	netsnmp_request_group_item *current;

	/*
	 * loop through columns
	 */
	for( current = rg->list; current; current = current->next ) {

		var = current->ri->requestvb;

		switch(current->tri->colnum) {

			case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMAXCAPACITY:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTCURRENTLEVEL:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIANAME:
				/** OCTETSTR = ASN_OCTET_STR */
				break;

			case COLUMN_PRTINPUTNAME:
				/** OCTETSTR = ASN_OCTET_STR */
				break;

			case COLUMN_PRTINPUTSECURITY:
				/** PresentOnOff = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIAWEIGHT:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIATYPE:
				/** OCTETSTR = ASN_OCTET_STR */
				break;

			case COLUMN_PRTINPUTMEDIACOLOR:
				/** OCTETSTR = ASN_OCTET_STR */
				break;

			case COLUMN_PRTINPUTMEDIAFORMPARTS:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTNEXTINDEX:
				/** INTEGER32 = ASN_INTEGER */
				break;

		}
	}

	/*
	 * done with all the columns. Could check row related
	 * requirements here.
	 */
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
void prtInputTable_set_undo( netsnmp_request_group *rg )
{
	netsnmp_variable_list *var;
	/* prtInputTable_context *row_ctx = 
	   (prtInputTable_context *)rg->existing_row;
	   prtInputTable_context *undo_ctx = 
	   (prtInputTable_context *)rg->undo_info; */
	netsnmp_request_group_item *current;

	/*
	 * loop through columns
	 */
	for( current = rg->list; current; current = current->next ) {

		var = current->ri->requestvb;

		switch(current->tri->colnum) {

			case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMAXCAPACITY:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTCURRENTLEVEL:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIANAME:
				/** OCTETSTR = ASN_OCTET_STR */
				break;

			case COLUMN_PRTINPUTNAME:
				/** OCTETSTR = ASN_OCTET_STR */
				break;

			case COLUMN_PRTINPUTSECURITY:
				/** PresentOnOff = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIAWEIGHT:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIATYPE:
				/** OCTETSTR = ASN_OCTET_STR */
				break;

			case COLUMN_PRTINPUTMEDIACOLOR:
				/** OCTETSTR = ASN_OCTET_STR */
				break;

			case COLUMN_PRTINPUTMEDIAFORMPARTS:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTMEDIALOADTIMEOUT:
				/** INTEGER32 = ASN_INTEGER */
				break;

			case COLUMN_PRTINPUTNEXTINDEX:
				/** INTEGER32 = ASN_INTEGER */
				break;

			default: /** We shouldn't get here */
				netsnmp_assert(0); /** why wasn't this caught in reserve1? */
		}
	}

	/*
	 * done with all the columns. Could check row related
	 * requirements here.
	 */
}

/************************************************************
 * initialize the table
 */
void initialize_table_prtInputTable(void)
{
	netsnmp_table_registration_info *table_info;

	if (my_handler) {
		snmp_log(
				LOG_ERR, "initialize_table_prtInputTable_handler called again\n");
		return;
	}

	memset(&cb, 0x00, sizeof(cb));

	/** create the table structure itself */
	table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (!table_info) {
        snmp_log(LOG_ERR, "malloc table_info failed in "
                 "initialize_table_prtInputTable_handler\n");
        return; /** mallocs failed */
    }

    my_handler = netsnmp_create_handler_registration("prtInputTable",
                                             netsnmp_table_array_helper_handler,
                                             prtInputTable_oid,
                                             prtInputTable_oid_len,
                                             HANDLER_CAN_RWRITE
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_prtInputTable_handler\n");
        SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

	if (!my_handler || !table_info) {
		snmp_log(LOG_ERR, "malloc failed in "
				"initialize_table_prtInputTable_handler\n");
		return; /** mallocs failed */
	}

	/** index: hrDeviceIndex */
	netsnmp_table_helper_add_index(table_info, ASN_INTEGER);
	/** index: prtInputIndex */
	netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

	table_info->min_column = prtInputTable_COL_MIN;
	table_info->max_column = prtInputTable_COL_MAX;

	/***************************************************
	 * registering the table with the master agent
	 */
	cb.get_value = prtInputTable_get_value;
	cb.container = netsnmp_container_find("prtInputTable_primary:"
			"prtInputTable:"
			"table_container");
	cb.can_set = 1;
	cb.duplicate_row = (UserRowMethod*)prtInputTable_duplicate_row;
	cb.delete_row = (UserRowMethod*)prtInputTable_delete_row;
	cb.row_copy = (Netsnmp_User_Row_Operation *)prtInputTable_row_copy;

	cb.can_delete = (Netsnmp_User_Row_Action *)prtInputTable_can_delete;

	cb.set_reserve1 = prtInputTable_set_reserve1;
	cb.set_reserve2 = prtInputTable_set_reserve2;
	cb.set_action = prtInputTable_set_action;
	cb.set_commit = prtInputTable_set_commit;
	cb.set_free = prtInputTable_set_free;
	cb.set_undo = prtInputTable_set_undo;
	DEBUGMSGTL(("initialize_table_prtInputTable",
				"Registering table prtInputTable "
				"as a table array\n"));
	netsnmp_table_container_register(my_handler, table_info, &cb,
			cb.container, 1);
}

/************************************************************
 * get data from a column
 */
int prtInputTable_get_value(
		netsnmp_request_info *request,
		netsnmp_index *item,
		netsnmp_table_request_info *table_info )
{
	netsnmp_variable_list *var = request->requestvb;
	prtInputTable_context *ctx = (prtInputTable_context *)item;

	PRTMIB_OID_MAP *map = ctx->columnInfo + table_info->colnum;
	uint32_t ndx = map->marvell_oid_index;
	int numberOfTrays = GetTrayInform(SETTING_TRAY_COUNT, 0);
	numberOfTrays++; /// Be careful!!! tray-n number is start from internal-index number '2'.

	//if (ndx == INDEX_VARIABLE)
	ndx = ctx->index.oids[1] - 1;
	ndx += 1; /// What??? Why? -1 and +1??? Kidding? -_-;;;

	switch (table_info->colnum) {
		case COLUMN_PRTINPUTINDEX: // 1, Integer(internal value), Done, 2019-11-07
			{
				int prtInputIndex = 0;

				prtInputIndex = ndx;

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputIndex, sizeof(prtInputIndex));
			}
			break;

		case COLUMN_PRTINPUTTYPE: // 2, Integer, Done, 2019-11-07
			{
				// 2, ndx=1: MPT/4, ndx2~4: tray/3
				int prtInputType = 2; // Unknown

				switch (ndx) { // ndx: 1(MPT), 2~4
					case 1: // MPT
						prtInputType = 4; // 4: Non-removable (FYI, 5 is manual)
						break;

					case 2:
					case 3:
					case 4:
						prtInputType = (numberOfTrays >= ndx) ? 3 : 2; // 3: Removable, Should add 1 because tray index is start from 2
						break;

					default:
						prtInputType = 2; // Unknown
						break;
				}

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputType, sizeof(prtInputType));
			}
			break;

		case COLUMN_PRTINPUTDIMUNIT: // 3, Integer, Done, 2019-11-07
			{
				// Dimension unit: 4 = micrometer
				int prtInputDimUnit = 2; // Unknown. (Not in PrtMediaUnitTC)

				prtInputDimUnit = (numberOfTrays >= ndx) ? 4 : 2;

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputDimUnit, sizeof(prtInputDimUnit));
			}
			break;

		case COLUMN_PRTINPUTMEDIADIMFEEDDIRDECLARED: // 4, Integer, Check
		case COLUMN_PRTINPUTMEDIADIMFEEDDIRCHOSEN: // 6, Integer, Check
			{
				// int prtInputMediaDimFeedDirDeclared = 0;
				int prtInputMediaDimXFeedDirDeclared = 0;
				uint32_t paperSizeIndex = 0;
				uint32_t paperSizeFeedTemp = 0; /// Variable to get size from function
				uint32_t paperSizeXFeedTemp = 0; /// Variable to get size from function
				int paperSizeFeed = 0;
				int paperSizeXFeed = 0;
				error_type_t errorCode = 0;
				// error_type_t db_retval = FAIL;

				if (numberOfTrays >= ndx) {
					paperSizeIndex = GetTrayInform(TRAY_PAPER_SIZE, ndx-1); // tray index == ndx, 2017-01-11
					// printf("[Network/SNMP] tray-%d paper size: %d\n",(ndx-1), paperSizeIndex);
					errorCode = paper_get_media_size(paperSizeIndex, &paperSizeFeedTemp, &paperSizeXFeedTemp);
					if (errorCode == -1){
						paperSizeFeed = -2;
						paperSizeXFeed = -2;
					} else {
						paperSizeFeed = (int)paperSizeFeedTemp;
						paperSizeXFeed = (int)paperSizeXFeedTemp;
					}
				} else {
					paperSizeFeed = -2;
					paperSizeXFeed = -2;
				}

				// Kara, Pinetree: LEF (Differ from Juniper, BE CAREFUL!!!)
				// prtInputMediaDimFeedDirDeclared = (paperSizeFeed != -2) ? (paperSizeFeed * 10) : -2;
				prtInputMediaDimXFeedDirDeclared = (paperSizeXFeed != -2) ? (paperSizeXFeed * 10) : -2;
				// snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputMediaDimFeedDirDeclared, sizeof(prtInputMediaDimFeedDirDeclared));
				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputMediaDimXFeedDirDeclared, sizeof(prtInputMediaDimXFeedDirDeclared));
				// printf("SNMP> Paper size in tray-%d:, index: %d, X:%d, Y: %d\n", ndx, paperSizeIndex, prtInputMediaDimFeedDirDeclared, prtInputMediaDimXFeedDirDeclared);
			}

/*			{
				int page_type;
				int page_type_num;
				int horiSize;
				error_type_t db_retval = FAIL;

				page_type = db_get_var_int(CONF_PRINT,tray_size[ndx -1],&db_retval);

				if(db_retval == FAIL){
					horiSize = 0;
				}
				else{
					switch(page_type)
					{
						case MEDIASIZE_A4:
							page_type_num = 0; 
							break;
						case MEDIASIZE_A5:
							page_type_num =  1;
							break;
						case MEDIASIZE_A5_ROTATED:
							page_type_num =  2;
							break;
						case MEDIASIZE_A6:
							page_type_num =  3;
							break;
						case MEDIASIZE_B5:
							page_type_num =  4;
							break;
						case MEDIASIZE_B6_JIS:
							page_type_num =  5;
							break;
						case MEDIASIZE_LETTER:
							page_type_num = 6; 
							break;
						case MEDIASIZE_LEGAL:
							page_type_num = 7; 
							break;
						case MEDIASIZE_EXECUTIVE:
							page_type_num = 8; 
							break;
						case MEDIASIZE_STATEMENT:
							page_type_num = 9;
							break;
						case MEDIASIZE_CUSTOM_85x13:
							page_type_num = 10;
							break;
						case MEDIASIZE_CUSTOM_8x10:
							page_type_num = 11;
							break;
						case MEDIASIZE_CUSTOM_8x105:
							page_type_num = 12;
							break;
						case MEDIASIZE_CUSTOM_825x14:
							page_type_num = 13;
							break;
						case MEDIASIZE_JAPANESE_POSTCARD:
							page_type_num = 14;
							break;
						case MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED:
							page_type_num = 15;
							break;
						case MEDIASIZE_DBL_JAPANESE_POSTCARD:
							page_type_num = 16;
							break;
						case MEDIASIZE_ENV_MONARCH:
							page_type_num = 17; 
							break;
						case MEDIASIZE_ENV_9:
							page_type_num = 18;
							break;
						case MEDIASIZE_ENV_10:
							page_type_num = 19;
							break;
						case MEDIASIZE_ENV_C5:
							page_type_num = 21; 
							break;
						case MEDIASIZE_ENV_C6:
							page_type_num = 22;
							break;
						case MEDIASIZE_ENV_DL:
							page_type_num = 20;
							break;
						case 554:   // AUTO value
							page_type_num = 23; 
							break;
						default:
							page_type_num = 23;
							break;
					}
					horiSize = paper_size_info[page_type_num].height;
				}

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &horiSize, sizeof(horiSize));
			}
*/
			break;

		case COLUMN_PRTINPUTMEDIADIMXFEEDDIRDECLARED: // 5, Integer, Check
		case COLUMN_PRTINPUTMEDIADIMXFEEDDIRCHOSEN: // 7, Integer, Check
			{
				int prtInputMediaDimFeedDirDeclared = 0;
				// int prtInputMediaDimXFeedDirDeclared = 0;
				uint32_t paperSizeIndex = 0;
				uint32_t paperSizeFeedTemp = 0; /// Variable to get size from function
				uint32_t paperSizeXFeedTemp = 0; /// Variable to get size from function
				int paperSizeFeed = 0;
				int paperSizeXFeed = 0;
				error_type_t errorCode = 0;
				// error_type_t db_retval = FAIL;

				if (numberOfTrays >= ndx) {
					paperSizeIndex = GetTrayInform(TRAY_PAPER_SIZE, ndx-1); // tray index == ndx, 2017-01-11
					// printf("[Network/SNMP] tray-%d paper size: %d\n",(ndx-1), paperSizeIndex);
					errorCode = paper_get_media_size(paperSizeIndex, &paperSizeFeedTemp, &paperSizeXFeedTemp);
					if (errorCode == -1){
						paperSizeFeed = -2;
						paperSizeXFeed = -2;
					} else {
						paperSizeFeed = (int)paperSizeFeedTemp;
						paperSizeXFeed = (int)paperSizeXFeedTemp;
					}
				} else {
					paperSizeFeed = -2;
					paperSizeXFeed = -2;
				}

				// Kara, Pinetree: LEF (Differ from Juniper, BE CAREFUL!!!)
				prtInputMediaDimFeedDirDeclared = (paperSizeFeed != -2) ? (paperSizeFeed * 10) : -2;
				// prtInputMediaDimXFeedDirDeclared = (paperSizeXFeed != -2) ? (paperSizeXFeed * 10) : -2;
				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputMediaDimFeedDirDeclared, sizeof(prtInputMediaDimFeedDirDeclared));
				// snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputMediaDimXFeedDirDeclared, sizeof(prtInputMediaDimXFeedDirDeclared));
				// printf("SNMP> Paper size in tray-%d:, index: %d, X:%d, Y: %d\n", ndx, paperSizeIndex, prtInputMediaDimFeedDirDeclared, prtInputMediaDimXFeedDirDeclared);
				/*
				{
					int page_type;
					int page_type_num;
					int vertSize;
					error_type_t db_retval = FAIL;

					page_type = db_get_var_int(CONF_PRINT,tray_size[ndx -1],&db_retval);

					if(db_retval == FAIL){
						vertSize = 0;
					}
					else{
						switch(page_type)
						{
							case MEDIASIZE_A4:
								page_type_num = 0;
								break;
							case MEDIASIZE_A5:
								page_type_num =  1;
								break;
							case MEDIASIZE_A5_ROTATED:
								page_type_num =  2;
								break;
							case MEDIASIZE_A6:
								page_type_num =  3;
								break;
							case MEDIASIZE_B5:
								page_type_num =  4;
								break;
							case MEDIASIZE_B6_JIS:
								page_type_num =  5;
								break;
							case MEDIASIZE_LETTER:
								page_type_num = 6;
								break;
							case MEDIASIZE_LEGAL:
								page_type_num = 7;
								break;
							case MEDIASIZE_EXECUTIVE:
								page_type_num = 8;
								break;
							case MEDIASIZE_STATEMENT:
								page_type_num = 9;
								break;
							case MEDIASIZE_CUSTOM_85x13:
								page_type_num = 10;
								break;
							case MEDIASIZE_CUSTOM_8x10:
								page_type_num = 11;
								break;
							case MEDIASIZE_CUSTOM_8x105:
								page_type_num = 12;
								break;
							case MEDIASIZE_CUSTOM_825x14:
								page_type_num = 13;
								break;
							case MEDIASIZE_JAPANESE_POSTCARD:
								page_type_num = 14;
								break;
							case MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED:
								page_type_num = 15;
								break;
							case MEDIASIZE_DBL_JAPANESE_POSTCARD:
								page_type_num = 16;
								break;
							case MEDIASIZE_ENV_MONARCH:
								page_type_num = 17;
								break;
							case MEDIASIZE_ENV_9:
								page_type_num = 18;
								break;
							case MEDIASIZE_ENV_10:
								page_type_num = 19;
								break;
							case MEDIASIZE_ENV_C5:
								page_type_num = 21;
								break;
							case MEDIASIZE_ENV_C6:
								page_type_num = 22;
								break;
							case MEDIASIZE_ENV_DL:
								page_type_num = 20;
								break;
							case 554:   // AUTO value
								page_type_num = 23;
								break;
							default:
								page_type_num = 23;
								break;
						}

						vertSize = paper_size_info[page_type_num].width;
					}
				*/
			}
			break;

        case COLUMN_PRTINPUTCAPACITYUNIT: // 8, Integer, Done, 2019-11-07
			{
				int prtInputCapacityUnit = 2; // sheets: 8

				prtInputCapacityUnit = (numberOfTrays >= ndx) ? 8 : 2;

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputCapacityUnit, sizeof(prtInputCapacityUnit));
			}
			break;

        case COLUMN_PRTINPUTMAXCAPACITY: // 9, Integer, Done, 2019-11-07
			{
				// Kara-S: MPT 50, Tray 500
				int prtInputMaxCapacity = -2;

				switch (ndx) {
					case 1: // MPT: 50
						prtInputMaxCapacity = PRT_INPUT_MPT_MAX_CAPACITY; // 50
						break;

					case 2: // tray-1~3: 500
					case 3:
					case 4:
						prtInputMaxCapacity = (numberOfTrays >= ndx) ? PRT_INPUT_TRAY_MAX_CAPACITY : -2; // 500
						break;

					default:
						prtInputMaxCapacity = -2;
						break;
				}

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputMaxCapacity, sizeof(prtInputMaxCapacity));
			}
			break;

        case COLUMN_PRTINPUTCURRENTLEVEL:// 10, Integer, Done, 2019-11-07
			{
				int prtInputCurrentLevel = -2; // Unknown
				int paperAmount = 0;

				switch (ndx) {
					case 1: // MPT
						paperAmount = GetTrayInform(TRAY_PAPER_AMOUNT, (ndx-1)); // 0: Paper End, 1: Paper Low, 10: Paper Full
						// printf("[Network/SNMP] tray-%d paper level: %d\n",(ndx-1), paperAmount);
						switch(paperAmount) {
							case 0: // Paper end
								prtInputCurrentLevel = 0;
								break;

							case 1: // Paper low
							case 10: // Paper full
								prtInputCurrentLevel = -3; // At least 1 unit
								break;

							default:
								prtInputCurrentLevel = -2;
								break;
						}
						break;

					case 2: // tray-1~4
					case 3:
					case 4:
						if (numberOfTrays >= ndx) {
							paperAmount = GetTrayInform(TRAY_PAPER_AMOUNT, (ndx-1)); // 0: Paper End, 1: Paper Low, 10: Paper Full
							// printf("[Network/SNMP] tray-%d paper level: %d\n",(ndx-1), paperAmount);

							switch(paperAmount) {
								case 0: // Paper end
									prtInputCurrentLevel = 0;
									break;

								case 1: // Paper low
									prtInputCurrentLevel = 50; // Near end
									break;

								case 10: // Paper full
									prtInputCurrentLevel = PRT_INPUT_TRAY_MAX_CAPACITY; // 500
									break;

								default:
									prtInputCurrentLevel = -2;
									break;
							}
						} else {
							prtInputCurrentLevel = -2;
						}
						break;

					default:
						prtInputCurrentLevel = -2;
						break;
				}

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputCurrentLevel, sizeof(prtInputCurrentLevel));
			}
			break;

        case COLUMN_PRTINPUTSTATUS: // 11, Integer, Done, 2019-11-07
			{
				int prtInputStatus = 5; // Unknown
				int trayStatus = 0;
				int paperAmount = 0;

				switch (ndx) {
					case 1: // MPT: Doesn't have actual status.
						// Check the paper is set and set the warning bit if there is no paper.
						paperAmount = GetTrayInform(TRAY_PAPER_AMOUNT, (ndx-1)); // 0: Paper End, 1: Paper Low, 10: Paper Full
						switch (paperAmount) {
							case 0: // Paper end
								prtInputStatus = 9; // 1(Unavailable and on request) + 8(Non-critical alert)
								break;

							case 1: // Paper low
							case 10: // Paper full
								prtInputStatus = 0; // Online, Paper is set - no warning
								break;

							default:
								prtInputStatus = 5; // Unknown
								break;
						}
						break;

					case 2: // tray-1~3
					case 3:
					case 4:
						if (numberOfTrays >= ndx) {
							trayStatus = GetTrayInform(TRAY_STATUS, (ndx-1)); // 0: Close, 1: Open
							// printf("[Network/SNMP] tray-%d status: %d\n",(ndx-1), trayStatus);
							switch (trayStatus) {
								case 0: // Close
									paperAmount = GetTrayInform(TRAY_PAPER_AMOUNT, (ndx-1)); // 0: Paper End, 1: Paper Low, 10: Paper Full
									switch (paperAmount) {
										case 0: // Paper end
											prtInputStatus = 9; // 1(Unavailable and on request) + 8(Non-critical alert)
											break;

										case 1: // Paper low
											prtInputStatus = 8; // 8(Non-critical alert)
											break;

										case 10: // Paper full
											prtInputStatus = 0; // Online, Paper is set - no warning
											break;

										default:
											prtInputStatus = 5; // Unknown
											break;
									}
									break;

								case 1: // Open
									prtInputStatus = 32;
									break;

								default:
									prtInputStatus = 5;
									break;
							}
						} else {
							prtInputStatus = 5;
						}
						break;

					default:
						prtInputStatus = 5;
						break;
				}

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputStatus, sizeof(prtInputStatus));
			}
        	break;

        case COLUMN_PRTINPUTMEDIANAME: // 12, String64, Working
        	{
        		uint32_t mediaNameIndex = 0;
        		error_type_t errorCode = 0;
        		// error_type_t db_retval = FAIL;
        		char prtInputMediaName[64] = {0};

				if (numberOfTrays >= ndx) { // 1: MPT, 2~4: tray-1~3
					mediaNameIndex = GetTrayInform(TRAY_PAPER_SIZE, (ndx-1));
					// printf("[Network/SNMP] tray-%d paper name: %d\n",(ndx-1), mediaNameIndex);
					errorCode = getMediaName(mediaNameIndex, prtInputMediaName);
					// if (errorCode == -1) // do not use at now... because it append "" if error
				} else {
					strncpy(prtInputMediaName, PRTMIB_STR_UNIMPLEMENTED, strlen(PRTMIB_STR_UNIMPLEMENTED));
				}

				snmp_set_var_typed_value(var, ASN_OCTET_STR, prtInputMediaName, strlen(prtInputMediaName));
			}
			break;

        case COLUMN_PRTINPUTNAME: // 13, String64, Done, 2019-11-07
        case COLUMN_PRTINPUTDESCRIPTION: // 18, String256, Done
			{
				char inputDescription[SNMP_STRING_SIZE_MAX] = {0};

				switch(ndx)
				{
					case 1:
						snprintf(inputDescription, SNMP_STRING_SIZE_MAX, "MPT");
						break;

					case 2:
					case 3:
					case 4:
						if (numberOfTrays >= ndx) {
							snprintf(inputDescription, SNMP_STRING_SIZE_MAX, "tray-%d", (ndx-1)); // Tray number is ndx -1.
						} else {
							strncpy(inputDescription, PRTMIB_STR_UNIMPLEMENTED, strlen(PRTMIB_STR_UNIMPLEMENTED));
						}
						break;

					default:
						strncpy(inputDescription, PRTMIB_STR_UNIMPLEMENTED, strlen(PRTMIB_STR_UNIMPLEMENTED));
						break;
				}

				snmp_set_var_typed_value(var, ASN_OCTET_STR, inputDescription, strlen(inputDescription));
			}
			break;

        case COLUMN_PRTINPUTVENDORNAME: // 14, String64, Done, 2019-11-07
			{
				char inputVendorName[64] = {0};
				char *vendorName;
				error_type_t db_retval = FAIL;

				vendorName = db_get_var("INST_MACHINE_INFO","vendorName",&db_retval);

				if((vendorName == NULL) || (db_retval == FAIL)){
					strcpy(inputVendorName, PRTMIB_STR_UNIMPLEMENTED);
				} else {
					if (numberOfTrays >= ndx) {
						snprintf(inputVendorName, sizeof(inputVendorName), vendorName);
					} else {
						strcpy(inputVendorName, PRTMIB_STR_UNIMPLEMENTED);
					}
				}
				snmp_set_var_typed_value(var, ASN_OCTET_STR, inputVendorName, strlen(inputVendorName));

				MEM_FREE_AND_NULL(vendorName);
			}
			break;

        case COLUMN_PRTINPUTMODEL: // 15, String64, Done, 2019-11-07
        case COLUMN_PRTINPUTVERSION: // 16, String64, Done
        case COLUMN_PRTINPUTSERIALNUMBER: // 17, String32, Done
//        case COLUMN_PRTINPUTMEDIATYPE: // 21, String64, Done
        case COLUMN_PRTINPUTMEDIACOLOR: // 22, String64, Done
			{
				char tempString[SNMP_STRING_SIZE_MAX] = {0};

				// snprintf(tempString, SNMP_STRING_SIZE_MAX, PRTMIB_STR_UNIMPLEMENTED);
				strncpy(tempString, PRTMIB_STR_UNIMPLEMENTED, strlen(PRTMIB_STR_UNIMPLEMENTED));

				snmp_set_var_typed_value(var, ASN_OCTET_STR, tempString, strlen(tempString));
			}
			break;

        case COLUMN_PRTINPUTSECURITY: // 19, Integer, Done, 2019-11-07
			{
				int prtInputSecurity = 5; // Not present

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &prtInputSecurity, sizeof(prtInputSecurity));
			}
			break;

        case COLUMN_PRTINPUTMEDIAWEIGHT: // 20, Integer, Done, 2019-11-07
        case COLUMN_PRTINPUTMEDIAFORMPARTS: // 23, Integer, Done
        case COLUMN_PRTINPUTMEDIALOADTIMEOUT: // 24, Integer, Done
        case COLUMN_PRTINPUTNEXTINDEX: // 25, Integer, Done
			{
				int tempNumber = -2; // Unknown

				snmp_set_var_typed_value(var, ASN_INTEGER, (char *) &tempNumber, sizeof(tempNumber));
			}
			break;

        case COLUMN_PRTINPUTMEDIATYPE: // 21, String64, Done, 2019-11-07
			{
				uint32_t mediaTypeIndex;
				char prtInputMediaType[64] = {0};
				error_type_t db_retval = FAIL;
				error_type_t errorCode = 0;

				if (numberOfTrays >= ndx) {
					mediaTypeIndex = db_get_var_int(CONF_PRINT,tray_type[ndx -1],&db_retval); // tray. Use ndx-1 value to get a value from global variable at here.
					if (db_retval != FAIL ) {
						errorCode = getMediaType(mediaTypeIndex, prtInputMediaType);
						// if (errorCode == -1) // do not use at now... because it append "" if error
					} else {
						strncpy(prtInputMediaType, PRTMIB_STR_UNIMPLEMENTED, strlen(PRTMIB_STR_UNIMPLEMENTED));
					}
				} else {
					strncpy(prtInputMediaType, PRTMIB_STR_UNIMPLEMENTED, strlen(PRTMIB_STR_UNIMPLEMENTED));
				}

				snmp_set_var_typed_value(var, ASN_OCTET_STR, prtInputMediaType, strlen(prtInputMediaType));
			}
        	break;

        default: /** We shouldn't get here */
            snmp_log(LOG_ERR, "Unknown column in prtInputTable_get_value\n");
            return SNMP_ERR_GENERR;
            break;
    }
    return SNMP_ERR_NOERROR;
}

/************************************************************
 * look for row based on a set of indexes
 */
const prtInputTable_context *prtInputTable_get_by_idx(netsnmp_index *hdr)
{
    return (const prtInputTable_context *) CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
prtInputTable_context *prtInputTable_create_row(netsnmp_index *hdr)
{
    prtInputTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(prtInputTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR,
                "SNMP_MALLOC_TYPEDEF(prtInputTable_context) failed!\n");
        return NULL;
    }

    /** parse indexes */
    if (prtInputTable_extract_index(ctx, hdr)) {
        if (NULL != ctx->index.oids)
            free(ctx->index.oids);
        free(ctx);
        return NULL;
    }

    return ctx;
}

/************************************************************
 * extract a set of indexes into a row
 */
int prtInputTable_extract_index(prtInputTable_context *ctx, netsnmp_index *hdr)
{
    netsnmp_variable_list var_hrDeviceIndex;
    netsnmp_variable_list var_prtInputIndex;
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
    var_hrDeviceIndex.next_variable = &var_prtInputIndex;

    memset(&var_prtInputIndex, 0x00, sizeof(var_prtInputIndex));
    var_prtInputIndex.type = ASN_INTEGER;
    var_prtInputIndex.next_variable = NULL;

    /** parse the oid into the individual index components */
    err = parse_oid_indexes(hdr->oids, hdr->len, &var_hrDeviceIndex);
    if (err == SNMP_ERR_NOERROR) {
        /** copy index components into the context structure */
        if (snmp_clone_mem((void *) &ctx->index.oids,
                                        hdr->oids, hdr->len * sizeof(oid))) {
            return SNMP_ERR_GENERR;
        }
        ctx->index.len = hdr->len;
        // ctx->prtInputIndex = *var_prtInputIndex.val.integer;
    }

    /** parsing may have allocated memory. free it. */
    snmp_reset_var_buffers( &var_hrDeviceIndex );

    return err;
}

error_type_t getMediaName(uint32_t mediaNameIndex, char *mediaName) {
	char mediaName_temp[64] = {0};
	switch(mediaNameIndex) {
		case MEDIASIZE_ANY:
			strcpy(mediaName, PRTMIB_STR_UNIMPLEMENTED);
			// sprintf(mediaName_temp, " ");
			return 0;
		break;
		case MEDIASIZE_LETTER:
			sprintf(mediaName_temp, "na_letter_8.5x11in");
		break;
		case MEDIASIZE_LEGAL:
			sprintf(mediaName_temp, "na_legal_8.5x14in");
		break;
		case MEDIASIZE_STATEMENT:
			sprintf(mediaName_temp, "na_invoice_5.5x8.5in");
		break;
		case MEDIASIZE_EXECUTIVE:
			sprintf(mediaName_temp, "na_executive_7.25x10.5in");
		break;
		case MEDIASIZE_CUSTOM_85x13:
			sprintf(mediaName_temp, "na_foolscap_8.5x13in");
		break;
		case MEDIASIZE_A4:
			sprintf(mediaName_temp, "iso_a4_210x297mm");
		break;
		case MEDIASIZE_A5:
			sprintf(mediaName_temp, "iso_a5_148x210mm");
		break;
		case MEDIASIZE_A6:
			sprintf(mediaName_temp, "iso_a6_105x148mm");
		break;
		case MEDIASIZE_B5:
			sprintf(mediaName_temp, "jis_b5_182x257mm");
		break;
		case MEDIASIZE_B6_JIS:
			sprintf(mediaName_temp, "jis_b6_128x182mm");
		break;
		case MEDIASIZE_CUSTOM_POSTCARD_JIS:
			sprintf(mediaName_temp, "jpn_hagaki_100x148mm");
		break;
		case MEDIASIZE_CUSTOM_DBL_POSTCARD_JIS:
			sprintf(mediaName_temp, "jpn_oufuku_148x200mm");
		break;
		case MEDIASIZE_CUSTOM_16K:
			sprintf(mediaName_temp, "roc_16k_7.75x10.75in");
		break;
		case MEDIASIZE_ENV_9:
			sprintf(mediaName_temp, "na_number-9_3.875x8.875in");
		break;
		case MEDIASIZE_ENV_10:
			sprintf(mediaName_temp, "na_number-10_4.125x9.5in");
		break;
		case MEDIASIZE_ENV_MONARCH:
			sprintf(mediaName_temp, "na_monarch_3.875x7.5in");
		break;
		case MEDIASIZE_ENV_C5:
			sprintf(mediaName_temp, "iso_c5_162x229mm");
		break;
		case MEDIASIZE_ENV_C6:
			sprintf(mediaName_temp, "iso_c6_114x162mm");
		break;
		case MEDIASIZE_ENV_DL:
			sprintf(mediaName_temp, "iso_dl_110x220mm");
		break;
		case MEDIASIZE_ENV_B5:
			sprintf(mediaName_temp, "iso_b5_176x250mm");
		break;
		case MEDIASIZE_JAPANESE_POSTCARD:
			sprintf(mediaName_temp, "jpn_hagaki_100x148mm");
		break;
		case MEDIASIZE_DBL_JAPANESE_POSTCARD:
			sprintf(mediaName_temp, "jpn_oufuku_148x200mm");
		break;
		case MEDIASIZE_CUSTOM_16K_184_260:
			sprintf(mediaName_temp, "custom_16K-184x260_184x260mm");
		break;
		case MEDIASIZE_CUSTOM_16K_195_270:
			sprintf(mediaName_temp, "custom_16k-195x270_195x270mm");
		break;
		case MEDIASIZE_CUSTOM_4x6:
			sprintf(mediaName_temp, "na_index-4x6_4x6in");
		break;
		case MEDIASIZE_CUSTOM_5x8:
			sprintf(mediaName_temp, "na_index-5x8_5x8in");
		break;
		case MEDIASIZE_CUSTOM_8x10:
			sprintf(mediaName_temp, "na_govt-letter_8x10in");
		break;
		case MEDIASIZE_CUSTOM_8x105:
			sprintf(mediaName_temp, "custom_8x10.5_8x10.5in");
		break;
		case MEDIASIZE_CUSTOM_825x14:
			sprintf(mediaName_temp, "custom_8.25x14_8.25x14in");
		break;
		case MEDIASIZE_CUSTOM_10x15cm:
			sprintf(mediaName_temp, "custom_100x150_100x150mm");
		break;
		case MEDIASIZE_ENV_PERSONAL:
			sprintf(mediaName_temp, "na_personal_3.625x6.5in");
		break;
		case MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED:
			sprintf(mediaName_temp, "jpn_oufuku_148x200mm");
		break;
		case MEDIASIZE_A5_ROTATED:
			sprintf(mediaName_temp, "iso_a5_148x210mm");
		break;
		case MEDIASIZE_A3:
			sprintf(mediaName_temp, "iso_a3_297x420mm");
		break;
		case MEDIASIZE_11X17:
			sprintf(mediaName_temp, "na_ledger_11x17in");
		break;
		case MEDIASIZE_A4_ROTATED:
			sprintf(mediaName_temp, "iso_a4_210x297mm");
		break;
		case MEDIASIZE_B4:
			sprintf(mediaName_temp, "jis_b4_257x364mm");
		break;
		case MEDIASIZE_B5_JIS_ROTATED:
			sprintf(mediaName_temp, "jis_b5_182x257mm");
		break;
		case MEDIASIZE_LETTER_ROTATED:
			sprintf(mediaName_temp, "na_letter_8.5x11in");
		break;
//		case MEDIASIZE_STATEMENT_ROTATED:
//			sprintf(mediaName_temp, "na_invoice_5.5x8.5in");
//		break;
//		case MEDIASIZE_EXECUTIVE_ROTATED:
//			sprintf(mediaName_temp, "na_executive_7.25x10.5in");
//		break;
		case MEDIASIZE_CUSTOM_8K_270_390:
			sprintf(mediaName_temp, "custom_8K-270x390_270x390mm");
		break;
		// case MEDIASIZE_CUSTOM_16K_195_270:
		// 	sprintf(mediaName_temp, "custom_16K-195x270_195x270mm");
		// break;
//		case MEDIASIZE_CUSTOM_16K_270_195:
//			sprintf(mediaName_temp, "custom_16K-195x270_195x270mm");
//		break;
//		case MEDIASIZE_CUSTOM_8x13:
//			sprintf(mediaName_temp, "custom_8x13_8x13in");
//		break;
//		case MEDIASIZE_CUSTOM_825x13:
//			sprintf(mediaName_temp, "custom_8.25x13_8.25x13in");
//		break;
//		case MEDIASIZE_CUSTOM_85x135:
//			sprintf(mediaName_temp, "custom_8.5x13.5_8.5x13.5in");
//		break;
//		case MEDIASIZE_CUSTOM_865x13:
//			sprintf(mediaName_temp, "custom_8.65x13_8.65x13in");
//		break;
//		case MEDIASIZE_CUSTOM_8125x1325:
//			sprintf(mediaName_temp, "custom_8.125x13.25_8.125x13.25in");
//		break;
//		case MEDIASIZE_ENV_Y3:
//			sprintf(mediaName_temp, "custom_y3_98x148mm");
//		break;
//		case MEDIASIZE_ENV_Y4:
//			sprintf(mediaName_temp, "custom_y4_105x235mm");
//		break;
//		case MEDIASIZE_ENV_L3:
//			sprintf(mediaName_temp, "custom_l3_120x235mm");
//		break;
//		case MEDIASIZE_ENV_L4:
//			sprintf(mediaName_temp, "custom_l4_90x205mm");
//		break;
//		case MEDIASIZE_ENV_K1:
//			sprintf(mediaName_temp, "custom_k1_270x382mm");
//		break;
//		case MEDIASIZE_ENV_K2:
//			sprintf(mediaName_temp, "custom_k2_240x332mm");
//		break;
//		case MEDIASIZE_ENV_K3:
//			sprintf(mediaName_temp, "custom_k3_216x277mm");
//		break;
		case MEDIASIZE_ENV_C4:
			sprintf(mediaName_temp, "iso_c4_229x324mm");
		break;
		// SCAN USE ONLY
//		case MEDIASIZE_CUSTOM_220x330:
//			sprintf(mediaName_temp, "custom_220x330_220x330mm");
//		break;
//		case MEDIASIZE_CUSTOM_11x15:
//			sprintf(mediaName_temp, "custom_11x15_11x15in");
//		break;
		case MEDIASIZE_USER_DEFINED:
			strcpy(mediaName, PRTMIB_STR_UNIMPLEMENTED);
			// sprintf(mediaName_temp, " ");
			return 0;
		break;

		default:
			strcpy(mediaName, PRTMIB_STR_UNIMPLEMENTED);
			// sprintf(mediaName_temp, " ");
			// sprintf(mediaName, mediaName_temp);
			return -1; // Can not find media name
		break;
		}
	sprintf(mediaName, mediaName_temp);
	return 0;
}

error_type_t getMediaType(uint32_t mediaTypeIndex, char *mediaType){
	char mediaType_temp[64] = {0};
	switch(mediaTypeIndex)
	{
		case 1:// MEDIATYPE_PLAIN_PAPER:		//1
			sprintf(mediaType_temp, "Plain paper"); // Paper (Plain), stationery
			break;
		case 2:
			sprintf(mediaType_temp, "Paper label"); // Labels, labels
			break;
		case 3:
			sprintf(mediaType_temp, "Envelope"); // Envelope, envelope
			break;
		case 4:
			sprintf(mediaType_temp, "Cardstock"); // Card Stock, cardstock
			break;
		case 5:
			sprintf(mediaType_temp, "Transparency"); // Transparency, transparency
			break;
		case 6:
			sprintf(mediaType_temp, "Vinyl label"); // Plastic, plastic??? Self-Adhesive Film, self-adhesive-film???
			break;
		case 7:
			sprintf(mediaType_temp, "Postcard"); // ???
			break;
		case 8:
			sprintf(mediaType_temp, "Recycled paper"); // ???
			break;
		case 9:
			sprintf(mediaType_temp, "Special paper"); // ???
			break;
		case 10:
			sprintf(mediaType_temp, "Color paper"); // Paper (Colored), stationery-colored
			break;
		case 11:
			sprintf(mediaType_temp, "Letterhead"); // Paper (Letterhead), stationery-letterhead
			break;
		case 12:
			sprintf(mediaType_temp, "Preprinted paper"); // Paper (Preprinted), stationery-preprinted
			break;
		case 13:
			sprintf(mediaType_temp, "Prepunched paper"); // Paper (Prepunched), stationery-prepunched
			break;
		case 14:
			sprintf(mediaType_temp, "Bond paper"); // Paper (Bond), stationery-bond
			break;
		case 15:
			sprintf(mediaType_temp, "Thick1 paper");
			break;
		case 16:
			sprintf(mediaType_temp, "Thick1+ paper");
			break;
		case 17:
			sprintf(mediaType_temp, "Thick2 paper");
			break;
		case 18:
			sprintf(mediaType_temp, "Thick3 paper");
			break;
		case 19:
			sprintf(mediaType_temp, "Tab paper"); // Tab Stock???, tab-stock???
			break;
		case 20:
			sprintf(mediaType_temp, "Thin paper");
			break;
		case 21: // TYPE_SINGLESID: // Singleside paper
			sprintf(mediaType_temp, "Singleside paper");
		break;
		case 22: // TYPE_CUSTOM1SP: // Custom1 paper
			sprintf(mediaType_temp, "Custom1");
		break;
		case 23: // TYPE_CUSTOM2SP: // Custom2 paper
			sprintf(mediaType_temp, "Custom2");
		break;
		case 24: // TYPE_CUSTOM3SP: // Custom3 paper
			sprintf(mediaType_temp, "Custom3");
		break;
		case 25: // TYPE_CUSTOM4SP: // Custom4 paper
			sprintf(mediaType_temp, "Custom4");
		break;
		case 26: // TYPE_CUSTOM5SP: // Custom5 paper
			sprintf(mediaType_temp, "Custom5");
		break;
		case 27: // TYPE_CUSTOM6SP: // Custom6 paper
			sprintf(mediaType_temp, "Custom6");
		break;

		default:
			strcpy(mediaType, PRTMIB_STR_UNIMPLEMENTED);
			// sprintf(mediaType_temp, " ");
			// sprintf(mediaType, mediaType_temp);
			return -1; // Can not find media type
		break;
	}
	sprintf(mediaType, mediaType_temp);
	return 0;
}

