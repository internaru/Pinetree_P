/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtMarkerTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#include "net_debug.h" // DBG_ family macros

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "prtMarkerTable.h"

#include "db_api.h"
#include "memAPI.h"

static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid prtMarkerTable_oid[] = { prtMarkerTable_TABLE_OID };
size_t prtMarkerTable_oid_len = OID_LENGTH(prtMarkerTable_oid);

static int prtMarkerTable_module_initialized = 0;

#if 0
/************************************************************
 * increment a numerical data in a row
 */
int inc_prtMarkerTable_data(long devNdx, long mrkNdx, unsigned long colFlags)
{
    int incAll = colFlags ? 0 : 1;
    prtMarkerTable_context *row;
    netsnmp_index hdr;
    oid ndxs[2];
    int col;

    /** make sure the module is initialized first */
    if (!prtMarkerTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndxs[0] = devNdx;
    ndxs[1] = mrkNdx;
    hdr.len = 2;
    hdr.oids = ndxs;

    /** find desired row */
    row = (prtMarkerTable_context *) prtMarkerTable_get_by_idx(&hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n", 
                    __func__, (int) devNdx, (int) mrkNdx);
        return -1;
    }

    /** increment desired column */
    for (col = COLUMN_PRTMARKERLIFECOUNT; 
            col <= prtMarkerTable_COL_MAX; col++) {
        if (!incAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERLIFECOUNT:
            if (row->prtMarkerLifeCount != 0xFFFFFFFF)
                row->prtMarkerLifeCount++;
        case COLUMN_PRTMARKERPOWERONCOUNT:
            if (row->prtMarkerPowerOnCount != 0xFFFFFFFF)
                row->prtMarkerPowerOnCount++;
            if (!incAll)
                break;

        default:
            return 0;
        }
    }

    return 0;
}

/************************************************************
 * get data from a row
 */
int get_prtMarkerTable_data(prtMarkerTable_context *out, unsigned long colFlags)
{
    int getAll = colFlags ? 0 : 1;
    prtMarkerTable_context *row;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtMarkerTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!out || out->index.len != 2 || !out->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }

    /** find desired row */
    hdr = &out->index;
    row = (prtMarkerTable_context *) prtMarkerTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** get data */
    for (col = 1; col <= prtMarkerTable_COL_MAX; col++) {
        if (!getAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERINDEX:
            out->prtMarkerIndex = row->prtMarkerIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERMARKTECH:
            out->prtMarkerMarkTech = row->prtMarkerMarkTech;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERCOUNTERUNIT:
            out->prtMarkerCounterUnit = row->prtMarkerCounterUnit;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERLIFECOUNT:
            out->prtMarkerLifeCount = row->prtMarkerLifeCount;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERPOWERONCOUNT:
            out->prtMarkerPowerOnCount = row->prtMarkerPowerOnCount;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERPROCESSCOLORANTS:
            out->prtMarkerProcessColorants = row->prtMarkerProcessColorants;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSPOTCOLORANTS:
            out->prtMarkerSpotColorants = row->prtMarkerSpotColorants;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYUNIT:
            out->prtMarkerAddressabilityUnit = 
                row->prtMarkerAddressabilityUnit;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYFEEDDIR:
            out->prtMarkerAddressabilityFeedDir = 
                row->prtMarkerAddressabilityFeedDir;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYXFEEDDIR:
            out->prtMarkerAddressabilityXFeedDir = 
                row->prtMarkerAddressabilityXFeedDir;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERNORTHMARGIN:
            out->prtMarkerNorthMargin = row->prtMarkerNorthMargin;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSOUTHMARGIN:
            out->prtMarkerSouthMargin = row->prtMarkerSouthMargin;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERWESTMARGIN:
            out->prtMarkerWestMargin = row->prtMarkerWestMargin;
            if (!getAll)
                break;
        case COLUMN_PRTMARKEREASTMARGIN:
            out->prtMarkerEastMargin = row->prtMarkerEastMargin;
            if (!getAll)
                break;
        case COLUMN_PRTMARKERSTATUS:
            out->prtMarkerStatus = row->prtMarkerStatus;
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
int set_prtMarkerTable_data(
                    prtMarkerTable_context *data, unsigned long colFlags)
{
    int setAll = colFlags ? 0 : 1;
    prtMarkerTable_context *row;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtMarkerTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!data || data->index.len != 2 || !data->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }
    if (chk_prtMarkerTable_data(data, colFlags))
        return -1;

    /** find desired row */
    hdr = &data->index;
    row = (prtMarkerTable_context *) prtMarkerTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** set data */
    for (col = 1; col <= prtMarkerTable_COL_MAX; col++) {
        if (!setAll && !(colFlags & (1 << col)))
            continue;

        switch (col) {

        case COLUMN_PRTMARKERINDEX:
            /** TODO: revisit
              * also a row index, must be consistent with index.oids[1]
              * do not allow changing this value for now
              * to change this value, another row must be created
              */
#if 0
            row->prtMarkerIndex = data->prtMarkerIndex;
#else
            snmp_log(LOG_ERR, "%s: cannot change marker index\n", __func__);
#endif
            if (!setAll)
                break;
        case COLUMN_PRTMARKERMARKTECH:
            row->prtMarkerMarkTech = data->prtMarkerMarkTech;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERCOUNTERUNIT:
            row->prtMarkerCounterUnit = data->prtMarkerCounterUnit;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERLIFECOUNT:
            row->prtMarkerLifeCount = data->prtMarkerLifeCount;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERPOWERONCOUNT:
            row->prtMarkerPowerOnCount = data->prtMarkerPowerOnCount;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERPROCESSCOLORANTS:
            row->prtMarkerProcessColorants = data->prtMarkerProcessColorants;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSPOTCOLORANTS:
            row->prtMarkerSpotColorants = data->prtMarkerSpotColorants;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYUNIT:
            row->prtMarkerAddressabilityUnit = 
                data->prtMarkerAddressabilityUnit;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYFEEDDIR:
            row->prtMarkerAddressabilityFeedDir = 
                data->prtMarkerAddressabilityFeedDir;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYXFEEDDIR:
            row->prtMarkerAddressabilityXFeedDir = 
                data->prtMarkerAddressabilityXFeedDir;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERNORTHMARGIN:
            row->prtMarkerNorthMargin = data->prtMarkerNorthMargin;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSOUTHMARGIN:
            row->prtMarkerSouthMargin = data->prtMarkerSouthMargin;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERWESTMARGIN:
            row->prtMarkerWestMargin = data->prtMarkerWestMargin;
            if (!setAll)
                break;
        case COLUMN_PRTMARKEREASTMARGIN:
            row->prtMarkerEastMargin = data->prtMarkerEastMargin;
            if (!setAll)
                break;
        case COLUMN_PRTMARKERSTATUS:
            row->prtMarkerStatus = data->prtMarkerStatus;
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
int del_prtMarkerTable_entry(long devNdx, long covNdx)
{
    oid ndxs[2];
    netsnmp_index hdr;
    prtMarkerTable_context *row;

    /** make sure the module is initialized */
    if (!prtMarkerTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndxs[0] = devNdx;
    ndxs[1] = covNdx;
    hdr.len = 2;
    hdr.oids = ndxs;

    /** find desired row */
    row = (prtMarkerTable_context *) prtMarkerTable_get_by_idx(&hdr);
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
int add_prtMarkerTable_entry(prtMarkerTable_context *info)
{
    netsnmp_index *hdr;
    prtMarkerTable_context *row;

    /** make sure the module is initialized first */
    if (!prtMarkerTable_module_initialized)
        init_prtMarkerTable();

    /** validate input info */
    if (!info || !info->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete input info\n", __func__);
        return -1;
    }
    if (chk_prtMarkerTable_data(info, 0))
        return -1;
    hdr = &info->index;


    /** make sure desired index tuple are non-existent */
    row = (prtMarkerTable_context *) prtMarkerTable_get_by_idx(hdr);
    if (row) {
        snmp_log(LOG_ERR, "%s: index[%d][%d] already in use\n",
                    __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** create row context and insert into container */
    row = prtMarkerTable_create_row(hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: add new row failed\n", __func__);
        return -1;
    }

    /** copy input column data */
    row->prtMarkerIndex = 
                info->prtMarkerIndex;
    row->prtMarkerMarkTech = 
                info->prtMarkerMarkTech;
    row->prtMarkerCounterUnit = 
                info->prtMarkerCounterUnit;
    row->prtMarkerLifeCount = 
                info->prtMarkerLifeCount;
    row->prtMarkerPowerOnCount = 
                info->prtMarkerPowerOnCount;
    row->prtMarkerProcessColorants = 
                info->prtMarkerProcessColorants;
    row->prtMarkerSpotColorants = 
                info->prtMarkerSpotColorants;
    row->prtMarkerAddressabilityUnit = 
                info->prtMarkerAddressabilityUnit;
    row->prtMarkerAddressabilityFeedDir = 
                info->prtMarkerAddressabilityFeedDir;
    row->prtMarkerAddressabilityXFeedDir = 
                info->prtMarkerAddressabilityXFeedDir;
    row->prtMarkerNorthMargin = 
                info->prtMarkerNorthMargin;
    row->prtMarkerSouthMargin = 
                info->prtMarkerSouthMargin;
    row->prtMarkerWestMargin = 
                info->prtMarkerWestMargin;
    row->prtMarkerEastMargin = 
                info->prtMarkerEastMargin;
    row->prtMarkerStatus = 
                info->prtMarkerStatus;

    return 0;
}

/************************************************************
 * validate one or more column data
 */
int chk_prtMarkerTable_data(
                    prtMarkerTable_context *data, unsigned long colFlags)
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
        snmp_log(LOG_ERR, "invalid marker index\n");
        return -1;
    }

    /** check columns */
    for (col = 1; col <= prtMarkerTable_COL_MAX; col++) {
        if (!checkAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMARKERINDEX:
            if (!data->prtMarkerIndex)
                data->prtMarkerIndex = data->index.oids[1];
            else if (data->prtMarkerIndex != data->index.oids[1]) {
                snmp_log(LOG_ERR, "inconsistent marker index: %d and %d\n",
                        (int) data->index.oids[1], (int) data->prtMarkerIndex);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERMARKTECH:
            if (!IS_PRTMARKERMARKTECH_VALID(data->prtMarkerMarkTech)) {
                snmp_log(LOG_ERR, "%s: invalid marking tech type code (%d)\n",
                    __func__, (int) data->prtMarkerMarkTech);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERCOUNTERUNIT:
            if (!IS_PRTMARKERCOUNTERUNIT_VALID(data->prtMarkerCounterUnit)) {
                snmp_log(LOG_ERR, "%s: invalid marker counter unit code (%d)\n",
                    __func__, (int) data->prtMarkerCounterUnit);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERLIFECOUNT:
            if (!IS_0TOFFFFFFFF(data->prtMarkerLifeCount)) {
                snmp_log(LOG_ERR, "%s: invalid marker life count (%d)\n",
                    __func__, (int) data->prtMarkerLifeCount);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERPOWERONCOUNT:
            if (!IS_0TOFFFFFFFF(data->prtMarkerPowerOnCount)) {
                snmp_log(LOG_ERR, "%s: invalid marker power-on count (%d)\n",
                    __func__, (int) data->prtMarkerPowerOnCount);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERPROCESSCOLORANTS:
            if (!IS_0TOFFFF(data->prtMarkerProcessColorants)) {
                snmp_log(LOG_ERR, "%s: invalid marker process colorants (%d)\n",
                    __func__, (int) data->prtMarkerProcessColorants);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSPOTCOLORANTS:
            if (!IS_0TOFFFF(data->prtMarkerSpotColorants)) {
                snmp_log(LOG_ERR, "%s: invalid marker spot colorants (%d)\n",
                    __func__, (int) data->prtMarkerSpotColorants);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYUNIT:
            if (!IS_PRTMARKERADDRESSABILITYUNIT_VALID(
                                    data->prtMarkerAddressabilityUnit)) {
                snmp_log(LOG_ERR, "%s: invalid marker resolution unit (%d)\n",
                    __func__, (int) data->prtMarkerAddressabilityUnit);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYFEEDDIR:
            if (!IS_NEG2TO7FFFFFFF(data->prtMarkerAddressabilityFeedDir)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid marker addressability feed dir (%d)\n",
                        __func__, (int) data->prtMarkerAddressabilityFeedDir);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERADDRESSABILITYXFEEDDIR:
            if (!IS_NEG2TO7FFFFFFF(data->prtMarkerAddressabilityXFeedDir)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid marker addressability xfeed dir (%d)\n",
                        __func__, (int) data->prtMarkerAddressabilityXFeedDir);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERNORTHMARGIN:
            if (!IS_NEG2TO7FFFFFFF(data->prtMarkerNorthMargin)) {
                snmp_log(LOG_ERR, "%s: invalid marker north margin (%d)\n",
                        __func__, (int) data->prtMarkerNorthMargin);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSOUTHMARGIN:
            if (!IS_NEG2TO7FFFFFFF(data->prtMarkerSouthMargin)) {
                snmp_log(LOG_ERR, "%s: invalid marker south margin (%d)\n",
                        __func__, (int) data->prtMarkerSouthMargin);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERWESTMARGIN:
            if (!IS_NEG2TO7FFFFFFF(data->prtMarkerWestMargin)) {
                snmp_log(LOG_ERR, "%s: invalid marker west margin (%d)\n",
                        __func__, (int) data->prtMarkerWestMargin);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKEREASTMARGIN:
            if (!IS_NEG2TO7FFFFFFF(data->prtMarkerEastMargin)) {
                snmp_log(LOG_ERR, "%s: invalid marker east margin (%d)\n",
                        __func__, (int) data->prtMarkerEastMargin);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMARKERSTATUS:
            if (!IS_SUBUNITSTATUS_VALID(data->prtMarkerStatus)) {
                snmp_log(LOG_ERR, "%s: invalid marker status code (%d)\n",
                    __func__, (int) data->prtMarkerStatus);
                return -1;
            }
            if (!checkAll)
                break;
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
void populate_prtMarkerTable(PRTMIB_OID_MAP *map)
{
    oid indexes[2];
    netsnmp_index hdr;
    prtMarkerTable_context *row;
    uint32_t i, numOfMarkers;

    if (NULL == map)
        return;

    if (OK != get_prtMIB_OIDValue(map, 0, (void *) &numOfMarkers, NULL))
        numOfMarkers = 1;

    if (!prtMarkerTable_module_initialized)
        initialize_table_prtMarkerTable();

    hdr.len = 2;
    hdr.oids = indexes;
    indexes[0] = PRTMIB_THIS_HOST_DEVICE_INDEX;

    for (i = 0; i < numOfMarkers; i++) {

        indexes[1] = i + 1;
        row = prtMarkerTable_create_row(&hdr);
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
void init_prtMarkerTable(void)
{
    initialize_table_prtMarkerTable();
    prtMarkerTable_module_initialized = 1;
}

/************************************************************
 * initialize the table
 */
void initialize_table_prtMarkerTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR, 
                "initialize_table_prtMarkerTable_handler called again\n");
        return;
    }
    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (!table_info) {
        snmp_log(LOG_ERR, "malloc table_info failed in "
                 "initialize_table_prtMarkerTable_handler\n");
        return; /** mallocs failed */
    }

    my_handler = netsnmp_create_handler_registration("prtMarkerTable",
                                             netsnmp_table_array_helper_handler,
                                             prtMarkerTable_oid,
                                             prtMarkerTable_oid_len,
                                             HANDLER_CAN_RONLY
                                             );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_prtMarkerTable_handler\n");
        SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
        /** index: hrDeviceIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);
        /** index: prtMarkerIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = prtMarkerTable_COL_MIN;
    table_info->max_column = prtMarkerTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = prtMarkerTable_get_value;
    cb.container = netsnmp_container_find("prtMarkerTable_primary:"
                                          "prtMarkerTable:"
                                          "table_container");
    DEBUGMSGTL(("initialize_table_prtMarkerTable",
                "Registering table prtMarkerTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * prtMarkerTable_get_value
 *
 * This routine is called for get requests to copy the data
 * from the context to the varbind for the request. If the
 * context has been properly maintained, you don't need to
 * change in code in this fuction.
 */
int prtMarkerTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    netsnmp_variable_list *var = request->requestvb;
    prtMarkerTable_context *ctx = (prtMarkerTable_context *)item;

    PRTMIB_OID_MAP *map = ctx->columnInfo + table_info->colnum;
    uint32_t ndx = map->marvell_oid_index;
    uint32_t num = 0;

    if (ndx == INDEX_VARIABLE)
        ndx = ctx->index.oids[1] - 1;
    if (OK != get_prtMIB_OIDValue(map, ndx, (void *) &num, NULL))
        num = PRTMIB_NUM_UNIMPLEMENTED;

    switch(table_info->colnum) {

#if 0
        case COLUMN_PRTMARKERMARKTECH:
            /** PrtMarkerMarkTechTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerMarkTech,
                         sizeof(context->prtMarkerMarkTech) );
        break;
    
        case COLUMN_PRTMARKERCOUNTERUNIT:
            /** PrtMarkerCounterUnitTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerCounterUnit,
                         sizeof(context->prtMarkerCounterUnit) );
        break;
    
        case COLUMN_PRTMARKERLIFECOUNT:
            /** COUNTER = ASN_COUNTER */
            snmp_set_var_typed_value(var, ASN_COUNTER,
                         (char*)&context->prtMarkerLifeCount,
                         sizeof(context->prtMarkerLifeCount) );
        break;
    
        case COLUMN_PRTMARKERPOWERONCOUNT:
            /** COUNTER = ASN_COUNTER */
            snmp_set_var_typed_value(var, ASN_COUNTER,
                         (char*)&context->prtMarkerPowerOnCount,
                         sizeof(context->prtMarkerPowerOnCount) );
        break;
    
        case COLUMN_PRTMARKERPROCESSCOLORANTS:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerProcessColorants,
                         sizeof(context->prtMarkerProcessColorants) );
        break;
    
        case COLUMN_PRTMARKERSPOTCOLORANTS:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSpotColorants,
                         sizeof(context->prtMarkerSpotColorants) );
        break;
    
        case COLUMN_PRTMARKERADDRESSABILITYUNIT:
            /** PrtMarkerAddressabilityUnitTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerAddressabilityUnit,
                         sizeof(context->prtMarkerAddressabilityUnit) );
        break;
    
        case COLUMN_PRTMARKERADDRESSABILITYFEEDDIR:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerAddressabilityFeedDir,
                         sizeof(context->prtMarkerAddressabilityFeedDir) );
        break;
    
        case COLUMN_PRTMARKERADDRESSABILITYXFEEDDIR:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerAddressabilityXFeedDir,
                         sizeof(context->prtMarkerAddressabilityXFeedDir) );
        break;
    
        case COLUMN_PRTMARKERNORTHMARGIN:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerNorthMargin,
                         sizeof(context->prtMarkerNorthMargin) );
        break;
    
        case COLUMN_PRTMARKERSOUTHMARGIN:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerSouthMargin,
                         sizeof(context->prtMarkerSouthMargin) );
        break;
    
        case COLUMN_PRTMARKERWESTMARGIN:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerWestMargin,
                         sizeof(context->prtMarkerWestMargin) );
        break;
    
        case COLUMN_PRTMARKEREASTMARGIN:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerEastMargin,
                         sizeof(context->prtMarkerEastMargin) );
        break;
    
        case COLUMN_PRTMARKERSTATUS:
            /** PrtSubUnitStatusTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMarkerStatus,
                         sizeof(context->prtMarkerStatus) );
        break;
#else
        case COLUMN_PRTMARKERMARKTECH:
        case COLUMN_PRTMARKERCOUNTERUNIT:
        case COLUMN_PRTMARKERPROCESSCOLORANTS:
        case COLUMN_PRTMARKERSPOTCOLORANTS:
        case COLUMN_PRTMARKERADDRESSABILITYUNIT:
        case COLUMN_PRTMARKERADDRESSABILITYFEEDDIR:
        case COLUMN_PRTMARKERADDRESSABILITYXFEEDDIR:
        case COLUMN_PRTMARKERNORTHMARGIN:
        case COLUMN_PRTMARKERSOUTHMARGIN:
        case COLUMN_PRTMARKERWESTMARGIN:
        case COLUMN_PRTMARKEREASTMARGIN:
        case COLUMN_PRTMARKERSTATUS:
            snmp_set_var_typed_value(
                        var, ASN_INTEGER, (char *) &num, sizeof(num));
            break;
        case COLUMN_PRTMARKERLIFECOUNT:
			{
				int page_count = 0;
				error_type_t db_retval = FAIL;

				page_count = db_get_var_int("INST_MACHINECOUNT","TotalPrint",&db_retval);
				if(db_retval == FAIL){
					page_count = 0;
				}
				snmp_set_var_typed_value(
						var, ASN_INTEGER, (char *) &page_count, sizeof(page_count));
			}
			break;
        case COLUMN_PRTMARKERPOWERONCOUNT:
            snmp_set_var_typed_value(
                        var, ASN_COUNTER, (char *) &num, sizeof(num));
            break;
#endif
    
    default: /** We shouldn't get here */
        snmp_log(LOG_ERR, "unknown column in "
                 "prtMarkerTable_get_value\n");
        return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

/************************************************************
 * look for row based on a set of indexes
 */
const prtMarkerTable_context *prtMarkerTable_get_by_idx(netsnmp_index * hdr)
{
    return (const prtMarkerTable_context *) CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
prtMarkerTable_context *prtMarkerTable_create_row(netsnmp_index *hdr)
{
    prtMarkerTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(prtMarkerTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR,
                "SNMP_MALLOC_TYPEDEF(prtMarkerTable_context) failed!\n");
        return NULL;
    }

    /** check indexes */
    if (prtMarkerTable_extract_index(ctx, hdr)) {
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
int prtMarkerTable_extract_index(
                            prtMarkerTable_context *ctx, netsnmp_index *hdr)
{
    netsnmp_variable_list var_hrDeviceIndex;
    netsnmp_variable_list var_prtMarkerIndex;
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
    var_hrDeviceIndex.next_variable = &var_prtMarkerIndex;

    memset(&var_prtMarkerIndex, 0x00, sizeof(var_prtMarkerIndex));
    var_prtMarkerIndex.type = ASN_INTEGER;
    var_prtMarkerIndex.next_variable = NULL;

    /** parse the oid into the individual index components */
    err = parse_oid_indexes(hdr->oids, hdr->len, &var_hrDeviceIndex);
    if (err == SNMP_ERR_NOERROR) {
        /** copy index components into the context structure */
        if (snmp_clone_mem((void *) &ctx->index.oids,
                    hdr->oids, hdr->len * sizeof(oid))) {
            return SNMP_ERR_GENERR;
        }
        ctx->index.len = hdr->len;
        //ctx->prtMarkerIndex = hdr->oids[1];
    }

    /** parsing may have allocated memory. free it. */
    snmp_reset_var_buffers(&var_hrDeviceIndex);

    return err;
}
