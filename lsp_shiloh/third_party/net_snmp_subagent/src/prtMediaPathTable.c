/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * prtMediaPathTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "prtMediaPathTable.h"


static     netsnmp_handler_registration *my_handler = NULL;
static     netsnmp_table_array_callbacks cb;

oid prtMediaPathTable_oid[] = { prtMediaPathTable_TABLE_OID };
size_t prtMediaPathTable_oid_len = OID_LENGTH(prtMediaPathTable_oid);

static int prtMediaPathTable_module_initialized = 0;


#if 0
/************************************************************
 * get data from a row
 */
int get_prtMediaPathTable_data(
                prtMediaPathTable_context *out, unsigned long colFlags)
{
    int getAll = colFlags ? 0 : 1;
    prtMediaPathTable_context *row;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtMediaPathTable_module_initialized) {
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
    row = (prtMediaPathTable_context *) prtMediaPathTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** get data */
    for (col = 1; col <= prtMediaPathTable_COL_MAX; col++) {
        if (!getAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMEDIAPATHINDEX:
            out->prtMediaPathIndex = row->prtMediaPathIndex;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXSPEEDPRINTUNIT:
            out->prtMediaPathMaxSpeedPrintUnit = 
                row->prtMediaPathMaxSpeedPrintUnit;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHMEDIASIZEUNIT:
            out->prtMediaPathMediaSizeUnit = row->prtMediaPathMediaSizeUnit;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXSPEED:
            out->prtMediaPathMaxSpeed = row->prtMediaPathMaxSpeed;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXMEDIAFEEDDIR:
            out->prtMediaPathMaxMediaFeedDir = 
                row->prtMediaPathMaxMediaFeedDir;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXMEDIAXFEEDDIR:
            out->prtMediaPathMaxMediaXFeedDir = 
                row->prtMediaPathMaxMediaXFeedDir;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHMINMEDIAFEEDDIR:
            out->prtMediaPathMinMediaFeedDir = 
                row->prtMediaPathMinMediaFeedDir;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHMINMEDIAXFEEDDIR:
            out->prtMediaPathMinMediaXFeedDir = 
                row->prtMediaPathMinMediaXFeedDir;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHTYPE:
            out->prtMediaPathType = row->prtMediaPathType;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHDESCRIPTION:
            memcpy((void *) out->prtMediaPathDescription,
                    (const void *) row->prtMediaPathDescription,
                    row->prtMediaPathDescription_len);
            out->prtMediaPathDescription_len =
                    row->prtMediaPathDescription_len;
            if (!getAll)
                break;
        case COLUMN_PRTMEDIAPATHSTATUS:
            out->prtMediaPathStatus = row->prtMediaPathStatus;
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
int set_prtMediaPathTable_data(
                prtMediaPathTable_context *data, unsigned long colFlags)
{
    int setAll = colFlags ? 0 : 1;
    prtMediaPathTable_context *row;
    netsnmp_index *hdr;
    int col;

    /** make sure the module is initialized */
    if (!prtMediaPathTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** validate output structure */
    if (!data || data->index.len != 2 || !data->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete output info\n", __func__);
        return -1;
    }
    if (chk_prtMediaPathTable_data(data, colFlags))
        return -1;

    /** find desired row */
    hdr = &data->index;
    row = (prtMediaPathTable_context *) prtMediaPathTable_get_by_idx(hdr);
    if (!row) {
        snmp_log(LOG_ERR, "%s: row[%d][%d] not found\n",
                            __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** set data */
    for (col = 1; col <= prtMediaPathTable_COL_MAX; col++) {
        if (!setAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMEDIAPATHINDEX:
            /** TODO: revisit
              * also a row index, must be consistent with index.oids[1]
              * do not allow changing this value for now
              * to change this value, another row must be created
              */
#if 0
            row->prtMediaPathIndex = data->prtMediaPathIndex;
#else
            snmp_log(LOG_ERR, 
                    "%s: cannot change media path index\n", __func__);
#endif
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXSPEEDPRINTUNIT:
            row->prtMediaPathMaxSpeedPrintUnit = 
                data->prtMediaPathMaxSpeedPrintUnit;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHMEDIASIZEUNIT:
            row->prtMediaPathMediaSizeUnit = data->prtMediaPathMediaSizeUnit;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXSPEED:
            row->prtMediaPathMaxSpeed = data->prtMediaPathMaxSpeed;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXMEDIAFEEDDIR:
            row->prtMediaPathMaxMediaFeedDir = 
                data->prtMediaPathMaxMediaFeedDir;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXMEDIAXFEEDDIR:
            row->prtMediaPathMaxMediaXFeedDir = 
                data->prtMediaPathMaxMediaXFeedDir;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHMINMEDIAFEEDDIR:
            row->prtMediaPathMinMediaFeedDir = 
                data->prtMediaPathMinMediaFeedDir;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHMINMEDIAXFEEDDIR:
            row->prtMediaPathMinMediaXFeedDir = 
                data->prtMediaPathMinMediaXFeedDir;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHTYPE:
            row->prtMediaPathType = data->prtMediaPathType;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHDESCRIPTION:
            memcpy((void *) row->prtMediaPathDescription,
                    (const void *) data->prtMediaPathDescription,
                    data->prtMediaPathDescription_len);
            row->prtMediaPathDescription_len =
                    data->prtMediaPathDescription_len;
            if (!setAll)
                break;
        case COLUMN_PRTMEDIAPATHSTATUS:
            row->prtMediaPathStatus = data->prtMediaPathStatus;
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
int del_prtMediaPathTable_entry(long devNdx, long mrkNdx)
{
    oid ndxs[2];
    netsnmp_index hdr;
    prtMediaPathTable_context *row;

    /** make sure the module is initialized */
    if (!prtMediaPathTable_module_initialized) {
        snmp_log(LOG_ERR, "%s: module uninitialized\n", __func__);
        return -1;
    }

    /** prepare row index tuple */
    ndxs[0] = devNdx;
    ndxs[1] = mrkNdx;
    hdr.len = 2;
    hdr.oids = ndxs;

    /** find desired row */
    row = (prtMediaPathTable_context *) prtMediaPathTable_get_by_idx(&hdr);
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
int add_prtMediaPathTable_entry(prtMediaPathTable_context *info)
{
    netsnmp_index *hdr;
    prtMediaPathTable_context *row;

    /** make sure the module is initialized first */
    if (!prtMediaPathTable_module_initialized)
        init_prtMediaPathTable();

    /** validate input info */
    if (!info || !info->index.oids) {
        snmp_log(LOG_ERR, "%s: incomplete input info\n", __func__);
        return -1;
    }
    if (chk_prtMediaPathTable_data(info, 0))
        return -1;
    hdr = &info->index;


    /** make sure desired index tuple are non-existent */
    row = (prtMediaPathTable_context *) prtMediaPathTable_get_by_idx(hdr);
    if (row) {
        snmp_log(LOG_ERR, "%s: index[%d][%d] already in use\n",
                    __func__, (int) hdr->oids[0], (int) hdr->oids[1]);
        return -1;
    }

    /** create row context and insert into container */
    row = prtMediaPathTable_create_row(hdr);
    if (!row || CONTAINER_INSERT(cb.container, row)) {
        snmp_log(LOG_ERR, "%s: add new row failed\n", __func__);
        return -1;
    }

    /** copy input column data */
    row->prtMediaPathIndex =
        info->prtMediaPathIndex;
    row->prtMediaPathMaxSpeedPrintUnit = 
        info->prtMediaPathMaxSpeedPrintUnit;
    row->prtMediaPathMediaSizeUnit =
        info->prtMediaPathMediaSizeUnit;
    row->prtMediaPathMaxSpeed =
        info->prtMediaPathMaxSpeed;
    row->prtMediaPathMaxMediaFeedDir =
        info->prtMediaPathMaxMediaFeedDir;
    row->prtMediaPathMaxMediaXFeedDir =
        info->prtMediaPathMaxMediaXFeedDir;
    row->prtMediaPathMinMediaFeedDir =
        info->prtMediaPathMinMediaFeedDir;
    row->prtMediaPathMinMediaXFeedDir =
        info->prtMediaPathMinMediaXFeedDir;
    row->prtMediaPathType =
        info->prtMediaPathType;
    memcpy((void *) row->prtMediaPathDescription,
            (const void *) info->prtMediaPathDescription,
                            info->prtMediaPathDescription_len);
    row->prtMediaPathDescription_len =
        info->prtMediaPathDescription_len;
    row->prtMediaPathStatus =
        info->prtMediaPathStatus;

    return 0;
}

/************************************************************
 * validate one or more column data
 */
int chk_prtMediaPathTable_data(
                    prtMediaPathTable_context *data, unsigned long colFlags)
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
        snmp_log(LOG_ERR, "invalid channel index\n");
        return -1;
    }

    /** check columns */
    for (col = 1; col <= prtMediaPathTable_COL_MAX; col++) {
        if (!checkAll && !(colFlags & (1 << col)))
            continue;
        switch (col) {

        case COLUMN_PRTMEDIAPATHINDEX:
            if (!data->prtMediaPathIndex)
                data->prtMediaPathIndex = data->index.oids[1];
            else if (data->prtMediaPathIndex != data->index.oids[1]) {
                snmp_log(LOG_ERR, "inconsistent media path index: %d and %d\n",
                            (int) data->index.oids[1], 
                            (int) data->prtMediaPathIndex);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXSPEEDPRINTUNIT:
            if (!IS_PRTMEDIAPATHMAXSPEEDPRINTUNIT_VALID(
                                    data->prtMediaPathMaxSpeedPrintUnit)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid media path max speed unit code (%d)\n",
                        __func__, (int) data->prtMediaPathMaxSpeedPrintUnit);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHMEDIASIZEUNIT:
            if (!IS_PRTMEDIAUNIT_VALID(data->prtMediaPathMediaSizeUnit)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid media path media size unit code (%d)\n",
                        __func__, (int) data->prtMediaPathMediaSizeUnit);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXSPEED:
            if (!IS_NEG2TO7FFFFFFF(data->prtMediaPathMaxSpeed)) {
                snmp_log(LOG_ERR, "%s: invalid media path max speed (%d)\n",
                        __func__, (int) data->prtMediaPathMaxSpeed);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXMEDIAFEEDDIR:
            if (!IS_NEG2TO7FFFFFFF(data->prtMediaPathMaxMediaFeedDir)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid media path max media feed (%d)\n",
                        __func__, (int) data->prtMediaPathMaxMediaFeedDir);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHMAXMEDIAXFEEDDIR:
            if (!IS_NEG2TO7FFFFFFF(data->prtMediaPathMaxMediaXFeedDir)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid media path max media xfeed (%d)\n",
                        __func__, (int) data->prtMediaPathMaxMediaXFeedDir);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHMINMEDIAFEEDDIR:
            if (!IS_NEG2TO7FFFFFFF(data->prtMediaPathMinMediaFeedDir)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid media path min media feed (%d)\n",
                        __func__, (int) data->prtMediaPathMinMediaFeedDir);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHMINMEDIAXFEEDDIR:
            if (!IS_NEG2TO7FFFFFFF(data->prtMediaPathMinMediaXFeedDir)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid media path min media xfeed (%d)\n",
                        __func__, (int) data->prtMediaPathMinMediaXFeedDir);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHTYPE:
            if (!IS_PRTMEDIAPATHTYPE_VALID(data->prtMediaPathType)) {
                snmp_log(LOG_ERR, "%s: invalid media path type code (%d)\n",
                    __func__, (int) data->prtMediaPathType);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHDESCRIPTION:
            if (!IS_0TO255(data->prtMediaPathDescription_len)) {
                snmp_log(LOG_ERR, 
                        "%s: invalid media path description length (%d)\n",
                        __func__, (int) data->prtMediaPathDescription_len);
                return -1;
            }
            if (!checkAll)
                break;
        case COLUMN_PRTMEDIAPATHSTATUS:
            if (!IS_SUBUNITSTATUS_VALID(data->prtMediaPathStatus)) {
                snmp_log(LOG_ERR, "%s: invalid media path status code (%d)\n",
                    __func__, (int) data->prtMediaPathStatus);
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
void populate_prtMediaPathTable(PRTMIB_OID_MAP *map)
{
    oid indexes[2];
    netsnmp_index hdr;
    prtMediaPathTable_context *row;
    uint32_t i, numOfMediaPaths;

    if (NULL == map)
        return;

    if (OK != get_prtMIB_OIDValue(map, 0, (void *) &numOfMediaPaths, NULL))
        numOfMediaPaths = 1;

    if (!prtMediaPathTable_module_initialized)
        initialize_table_prtMediaPathTable();

    hdr.len = 2;
    hdr.oids = indexes;
    indexes[0] = PRTMIB_THIS_HOST_DEVICE_INDEX;

    for (i = 0; i < numOfMediaPaths; i++) {

        indexes[1] = i + 1;
        row = prtMediaPathTable_create_row(&hdr);
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
void init_prtMediaPathTable(void)
{
    initialize_table_prtMediaPathTable();
    prtMediaPathTable_module_initialized = 1;
}

/************************************************************
 * initialize the table 
 */
void initialize_table_prtMediaPathTable(void)
{
    netsnmp_table_registration_info *table_info;

    if (my_handler) {
        snmp_log(LOG_ERR, 
                "initialize_table_prtMediaPathTable_handler called again\n");
        return;
    }
    memset(&cb, 0x00, sizeof(cb));

    /** create the table structure itself */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    if (!table_info) {
        snmp_log(LOG_ERR, "malloc table_info failed in "
                 "initialize_table_prtMediaPathTable_handler\n");
        return; /** mallocs failed */
    }

    my_handler = netsnmp_create_handler_registration("prtMediaPathTable",
                                         netsnmp_table_array_helper_handler,
                                         prtMediaPathTable_oid,
                                         prtMediaPathTable_oid_len,
                                         HANDLER_CAN_RONLY
                                         );
            
    if (!my_handler) {
        snmp_log(LOG_ERR, "malloc failed in "
                 "initialize_table_prtMediaPathTable_handler\n");
        SNMP_FREE(table_info);
        return; /** mallocs failed */
    }

    /***************************************************
     * Setting up the table's definition
     */
        /** index: hrDeviceIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);
        /** index: prtMediaPathIndex */
        netsnmp_table_helper_add_index(table_info, ASN_INTEGER);

    table_info->min_column = prtMediaPathTable_COL_MIN;
    table_info->max_column = prtMediaPathTable_COL_MAX;

    /***************************************************
     * registering the table with the master agent
     */
    cb.get_value = prtMediaPathTable_get_value;
    cb.container = netsnmp_container_find("prtMediaPathTable_primary:"
                                          "prtMediaPathTable:"
                                          "table_container");
    DEBUGMSGTL(("initialize_table_prtMediaPathTable",
                "Registering table prtMediaPathTable "
                "as a table array\n"));
    netsnmp_table_container_register(my_handler, table_info, &cb,
                                     cb.container, 1);
}

/************************************************************
 * prtMediaPathTable_get_value
 *
 * This routine is called for get requests to copy the data
 * from the context to the varbind for the request. If the
 * context has been properly maintained, you don't need to
 * change in code in this fuction.
 */
int prtMediaPathTable_get_value(
            netsnmp_request_info *request,
            netsnmp_index *item,
            netsnmp_table_request_info *table_info )
{
    netsnmp_variable_list *var = request->requestvb;
    prtMediaPathTable_context *ctx = (prtMediaPathTable_context *)item;

    PRTMIB_OID_MAP *map = ctx->columnInfo + table_info->colnum;
    uint32_t ndx = map->marvell_oid_index;

    if (ndx == INDEX_VARIABLE)
        ndx = ctx->index.oids[1] - 1;

    switch(table_info->colnum) {

#if 0
        case COLUMN_PRTMEDIAPATHMAXSPEEDPRINTUNIT:
            /** PrtMediaPathMaxSpeedPrintUnitTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathMaxSpeedPrintUnit,
                         sizeof(context->prtMediaPathMaxSpeedPrintUnit) );
        break;
    
        case COLUMN_PRTMEDIAPATHMEDIASIZEUNIT:
            /** PrtMediaUnitTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathMediaSizeUnit,
                         sizeof(context->prtMediaPathMediaSizeUnit) );
        break;
    
        case COLUMN_PRTMEDIAPATHMAXSPEED:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathMaxSpeed,
                         sizeof(context->prtMediaPathMaxSpeed) );
        break;
    
        case COLUMN_PRTMEDIAPATHMAXMEDIAFEEDDIR:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathMaxMediaFeedDir,
                         sizeof(context->prtMediaPathMaxMediaFeedDir) );
        break;
    
        case COLUMN_PRTMEDIAPATHMAXMEDIAXFEEDDIR:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathMaxMediaXFeedDir,
                         sizeof(context->prtMediaPathMaxMediaXFeedDir) );
        break;
    
        case COLUMN_PRTMEDIAPATHMINMEDIAFEEDDIR:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathMinMediaFeedDir,
                         sizeof(context->prtMediaPathMinMediaFeedDir) );
        break;
    
        case COLUMN_PRTMEDIAPATHMINMEDIAXFEEDDIR:
            /** INTEGER32 = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathMinMediaXFeedDir,
                         sizeof(context->prtMediaPathMinMediaXFeedDir) );
        break;
    
        case COLUMN_PRTMEDIAPATHTYPE:
            /** PrtMediaPathTypeTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathType,
                         sizeof(context->prtMediaPathType) );
        break;
    
        case COLUMN_PRTMEDIAPATHDESCRIPTION:
            /** PrtLocalizedDescriptionStringTC = ASN_OCTET_STR */
            snmp_set_var_typed_value(var, ASN_OCTET_STR,
                         (char*)&context->prtMediaPathDescription,
                         context->prtMediaPathDescription_len );
        break;
    
        case COLUMN_PRTMEDIAPATHSTATUS:
            /** PrtSubUnitStatusTC = ASN_INTEGER */
            snmp_set_var_typed_value(var, ASN_INTEGER,
                         (char*)&context->prtMediaPathStatus,
                         sizeof(context->prtMediaPathStatus) );
        break;
#else
        case COLUMN_PRTMEDIAPATHMAXSPEEDPRINTUNIT:
        case COLUMN_PRTMEDIAPATHMEDIASIZEUNIT:
        case COLUMN_PRTMEDIAPATHMAXSPEED:
        case COLUMN_PRTMEDIAPATHMAXMEDIAFEEDDIR:
        case COLUMN_PRTMEDIAPATHMAXMEDIAXFEEDDIR:
        case COLUMN_PRTMEDIAPATHMINMEDIAFEEDDIR:
        case COLUMN_PRTMEDIAPATHMINMEDIAXFEEDDIR:
        case COLUMN_PRTMEDIAPATHTYPE:
        case COLUMN_PRTMEDIAPATHSTATUS:
        {
            uint32_t num = 0;

            if (OK != get_prtMIB_OIDValue(map, ndx, (void *) &num, NULL))
                num = PRTMIB_NUM_UNIMPLEMENTED;
            snmp_set_var_typed_value(
                        var, ASN_INTEGER, (char *) &num, sizeof(num));
            break;
        } 
        case COLUMN_PRTMEDIAPATHDESCRIPTION:
        {
            char str[256] = {0};
            uint32_t str_sz = 255;

            if (OK != get_prtMIB_OIDValue(map, ndx, (void *) str, &str_sz))
                strcpy(str, PRTMIB_STR_UNIMPLEMENTED);
            snmp_set_var_typed_value(var, ASN_OCTET_STR, str, strlen(str));
            break;
        }
#endif
    
        default: /** We shouldn't get here */
            snmp_log(LOG_ERR, "unknown column in "
                     "prtMediaPathTable_get_value\n");
            return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

/************************************************************
 * look for row based on a set of indexes
 */
const prtMediaPathTable_context *prtMediaPathTable_get_by_idx(
                                                        netsnmp_index *hdr)
{
    return (const prtMediaPathTable_context *)
        CONTAINER_FIND(cb.container, hdr);
}

/************************************************************
 * create a new row
 */
prtMediaPathTable_context *prtMediaPathTable_create_row(
                                                        netsnmp_index *hdr)
{
    prtMediaPathTable_context *ctx;

    /** validate parameters */
    if (!hdr) {
        snmp_log(LOG_ERR, "%s: NULL parameter!\n", __func__);
        return NULL;
    }

    /** allocate memory for new context (row) */
    ctx = SNMP_MALLOC_TYPEDEF(prtMediaPathTable_context);
    if (!ctx) {
        snmp_log(LOG_ERR, "cannot allocate for marker colorant table\n");
        return NULL;
    }

    /** check indexes */
    if (prtMediaPathTable_extract_index(ctx, hdr)) {
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
int prtMediaPathTable_extract_index(
                    prtMediaPathTable_context *ctx, netsnmp_index *hdr)
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
        //ctx->prtMediaPathIndex = hdr->oids[1];
    }

    /** parsing may have allocated memory. free it. */
    snmp_reset_var_buffers(&var_hrDeviceIndex);

    return err;
}
